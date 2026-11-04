#!/usr/bin/env python3
"""Writes core/test/x68k/mouse.xdf, the shared PC boot test's X68000 mouse program disk
(Kairo68#81): a 1232 KB floppy with MOUSE68.R, 68000 code for Human68k that sets the IOCS mouse up
(_MS_INIT, so Kairo68's direct touch steers it) and then, with interrupts off, reads the mouse
itself, only every 8th frame: it raises RTS on SCC channel B (WR5), the mouse sends its packet
(status, dx, dy), and the next frame takes it from the receiver. Motion given faster than that
piles up in the mouse, which sends at most 127 a packet and drops the rest: what Kairo68's
x68k-mouse-pending patch lets direct touch wait for.

It draws its pointer, an 8 x 8 block in colour 15 on the text planes ($E00000, 128 bytes a line,
a plane every $20000), moving a pixel per count over 768 x 512 and stopping at the edges. A press of
the left button fills the screen with a colour (of the text palette it sets) from where the pointer is:
1 + (x / 128 + 5 * (y / 128)) mod 14. It never ends.

    make_x68k_mouse_disk.py
"""
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "shared/tools"))
from fat_disk import add_file, floppy_2hd  # noqa: E402
from m68k_assembler import BEQ, BNE, BPL, BLE, BRA, BSR, Assembler  # noqa: E402

OUTPUT = ROOT / "core/test/x68k/mouse.xdf"
WIDTH, HEIGHT = 768, 512
POLL_EVERY = 8  # frames
POINTER_COLOUR = 15
TEXT_PALETTE = 0xE82200
# The text palette the program sets, as (green, red, blue) of 0 to 31: black, 14 colours apart
# from each other, and white for the pointer.
PALETTE = [(0, 0, 0), (0, 31, 0), (31, 0, 0), (0, 0, 31), (31, 31, 0), (0, 31, 31), (31, 0, 31), (16, 16, 16),
           (16, 0, 0), (0, 16, 0), (0, 0, 16), (16, 31, 0), (31, 16, 0), (0, 16, 31), (16, 0, 31), (31, 31, 31)]
# Variables, from a5.
X, Y, PREV, BACKGROUND, BUTTON, NOW, PACKET = 0, 2, 4, 8, 9, 10, 12


def program():
    a = Assembler()
    e = a.emit

    e(0x42A7, 0xFF20, 0x588F)           # clr.l -(sp); DOS _SUPER; addq.l #4, sp
    e(0x7070, 0x4E4F)                   # moveq #_MS_INIT, d0; trap #15
    e(0x46FC, 0x2700)                   # move.w #$2700, sr: no interrupts, so IOCS reads no mouse
    a.lea_pc("variables", 5)            # lea variables(pc), a5
    e(0x41F9, TEXT_PALETTE >> 16, TEXT_PALETTE & 0xFFFF)   # lea TEXT_PALETTE, a0
    for green, red, blue in PALETTE:
        e(0x30FC, green << 11 | red << 6 | blue << 1)      # move.w #GRB, (a0)+
    e(0x49F9, 0x00E0, 0x0000)           # lea $E00000, a4: text plane 0
    e(0x47F9, 0x00E9, 0x8001)           # lea $E98001, a3: SCC channel B command
    e(0x45F9, 0x00E8, 0x8001)           # lea $E88001, a2: MFP GPIP
    e(0x3ABC, WIDTH // 2)               # move.w #384, (a5)
    e(0x3B7C, HEIGHT // 2, Y)           # move.w #256, Y(a5)
    e(0x42AD, PREV)                     # clr.l PREV(a5)
    e(0x422D, BACKGROUND)               # clr.b BACKGROUND(a5)
    e(0x422D, BUTTON)                   # clr.b BUTTON(a5): released
    e(0x422D, NOW)                      # clr.b NOW(a5)
    e(0x7000)                           # moveq #0, d0
    a.branch(BSR, "fill")
    # What the receiver holds from IOCS's last poll, cut off when interrupts went off, is no packet
    # of this program's: two frames for it to end, then it is dropped.
    a.branch(BSR, "vsync")
    a.branch(BSR, "vsync")
    e(0x7EFF)                           # moveq #-1, d7: frames, the first a request

    a.label("frame")
    a.branch(BSR, "vsync")
    e(0x5207)                           # addq.b #1, d7
    e(0x1007, 0x0200, POLL_EVERY - 1)   # move.b d7, d0; andi.b #7, d0
    a.branch(BNE, "draw")
    a.branch(BSR, "drain")
    e(0x16BC, 5, 0x16BC, 0x60)          # WR5 = $60: RTS off
    e(0x16BC, 5, 0x16BC, 0x62)          # WR5 = $62: RTS on, the mouse sends its packet
    # The packet's bytes, each taken as it comes: the receiver holds two, and a third unread
    # overruns it. A mouse that has nothing to tell sends none, so the frame's end ends the wait.
    e(0x7800, 0x7C00, 0x41ED, PACKET)   # moveq #0, d4: bytes; moveq #0, d6: display seen; lea PACKET(a5), a0
    a.label("collect")
    e(0x0813, 0); a.branch(BEQ, "no_byte")   # btst #0, (a3)
    e(0x11AB, 2, 0x4000)                # move.b 2(a3), 0(a0, d4.w)
    e(0x5244, 0x0C44, 3); a.branch(BEQ, "packet")  # addq.w #1, d4; cmpi.w #3, d4
    a.branch(BRA, "collect")
    a.label("no_byte")
    e(0x0812, 4); a.branch(BEQ, "blanking")  # btst #4, (a2)
    e(0x7C01); a.branch(BRA, "collect")      # moveq #1, d6
    a.label("blanking")
    e(0x4A06); a.branch(BEQ, "collect")      # tst.b d6: still the blank the request was made in
    a.branch(BRA, "draw")
    a.label("packet")
    e(0x122D, PACKET, 0x142D, PACKET + 1, 0x162D, PACKET + 2)  # d1 = status, d2 = dx, d3 = dy
    e(0x4882, 0xD455)                   # ext.w d2; add.w (a5), d2
    a.branch(BPL, "x_low")
    e(0x7400)                           # moveq #0, d2
    a.label("x_low")
    e(0x0C42, WIDTH - 1); a.branch(BLE, "x_high")
    e(0x343C, WIDTH - 1)
    a.label("x_high")
    e(0x3A82)                           # move.w d2, (a5)
    e(0x4883, 0xD66D, Y)                # ext.w d3; add.w Y(a5), d3
    a.branch(BPL, "y_low")
    e(0x7600)                           # moveq #0, d3
    a.label("y_low")
    e(0x0C43, HEIGHT - 1); a.branch(BLE, "y_high")
    e(0x363C, HEIGHT - 1)
    a.label("y_high")
    e(0x3B43, Y)                        # move.w d3, Y(a5)
    e(0x0201, 0x0002, 0x1B41, NOW)      # andi.b #2, d1 (the left button); move.b d1, NOW(a5)

    # The pointer's place: $E00000 + min(y, 504) * 128 + x / 8.
    a.label("draw")
    e(0x7000, 0x302D, Y)                # moveq #0, d0; move.w Y(a5), d0
    e(0x0C40, HEIGHT - 8); a.branch(BLE, "top")
    e(0x303C, HEIGHT - 8)
    a.label("top")
    e(0xEF88)                           # lsl.l #7, d0
    e(0x7200, 0x3215, 0xE649, 0xD081)   # moveq #0, d1; move.w (a5), d1; lsr.w #3, d1; add.l d1, d0
    e(0x41F4, 0x0800)                   # lea 0(a4, d0.l), a0
    e(0xB1ED, PREV); a.branch(BEQ, "drawn")  # cmpa.l PREV(a5), a0
    e(0x202D, PREV); a.branch(BEQ, "unsaved")  # move.l PREV(a5), d0
    e(0x2240, 0x102D, BACKGROUND)       # movea.l d0, a1; move.b BACKGROUND(a5), d0
    a.branch(BSR, "block")
    a.label("unsaved")
    e(0x2B48, PREV, 0x2248, 0x7000 | POINTER_COLOUR)  # move.l a0, PREV(a5); movea.l a0, a1; moveq #15, d0
    a.branch(BSR, "block")
    a.label("drawn")

    # A press of the left button: the screen filled with the pointer's colour, the pointer over it.
    e(0x102D, NOW, 0xB02D, BUTTON); a.branch(BEQ, "next")  # move.b NOW(a5), d0; cmp.b BUTTON(a5), d0
    e(0x1B40, BUTTON, 0x4A00); a.branch(BEQ, "next")       # move.b d0, BUTTON(a5); tst.b d0
    e(0x7000, 0x302D, Y, 0xEE48, 0xC0FC, 5)  # moveq #0, d0; move.w Y(a5), d0; lsr.w #7, d0; mulu #5, d0
    e(0x7200, 0x3215, 0xEE49, 0xD041)   # moveq #0, d1; move.w (a5), d1; lsr.w #7, d1; add.w d1, d0
    e(0x80FC, 14, 0x4840, 0x5240)       # divu #14, d0; swap d0; addq.w #1, d0
    e(0x1B40, BACKGROUND)               # move.b d0, BACKGROUND(a5)
    a.branch(BSR, "fill")
    e(0x226D, PREV, 0x7000 | POINTER_COLOUR)  # movea.l PREV(a5), a1; moveq #15, d0
    a.branch(BSR, "block")
    a.label("next")
    a.branch(BRA, "frame")

    # vsync: waits for the display period, then the vertical blank after it (MFP GPIP bit 4).
    a.label("vsync")
    e(0x0812, 4); a.branch(BEQ, "vsync")     # btst #4, (a2)
    a.label("blank")
    e(0x0812, 4); a.branch(BNE, "blank")
    e(0x4E75)

    # drain: drops what the receiver holds, an overrun's locked byte too (WR0 error reset).
    a.label("drain")
    e(0x16BC, 0x30)                     # move.b #$30, (a3): error reset
    e(0x0813, 0); a.branch(BEQ, "drained")   # btst #0, (a3)
    e(0x102B, 2); a.branch(BRA, "drain")     # move.b 2(a3), d0
    a.label("drained")
    e(0x4E75)

    # block: 8 lines of 8 pixels from a1 in colour d0.
    a.label("block")
    e(0x7407)                           # moveq #7, d2
    a.label("block_line")
    e(0x2C49)                           # movea.l a1, a6
    for plane in range(4):
        e(0x0800, plane, 0x56C5, 0x1C85)    # btst #plane, d0; sne d5; move.b d5, (a6)
        e(0xDDFC, 0x0002, 0x0000)           # adda.l #$20000, a6
    e(0x43E9, 128)                      # lea 128(a1), a1
    a.dbra(2, "block_line")
    e(0x4E75)

    # fill: the whole 768 x 512 (1024 x 512 of each plane) in colour d0.
    a.label("fill")
    e(0x7600)                           # moveq #0, d3: the plane
    a.label("fill_plane")
    e(0x0700, 0x56C5, 0x4885, 0x48C5)   # btst d3, d0; sne d5; ext.w d5; ext.l d5
    e(0x2C4C, 0x2803, 0x4844, 0x4244, 0xE38C, 0xDDC4)  # movea.l a4,a6; move.l d3,d4; swap d4; clr.w d4; lsl.l #1,d4; adda.l d4,a6
    e(0x3C3C, 128 * HEIGHT // 4 - 1)    # move.w #16383, d6
    a.label("fill_long")
    e(0x2CC5)                           # move.l d5, (a6)+
    a.dbra(6, "fill_long")
    e(0x5243, 0x0C43, 4); a.branch(BNE, "fill_plane")  # addq.w #1, d3; cmpi.w #4, d3
    e(0x4E75)

    a.data("variables", bytes(16))
    return a.build()


def main(argv):
    if len(argv) != 1:
        raise SystemExit(__doc__)
    code = program()
    image = floppy_2hd()
    add_file(image, 0, "MOUSE68.R", code)
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT.write_bytes(bytes(image))
    print(f"{OUTPUT}: MOUSE68.R, {len(code)} bytes")


if __name__ == "__main__":
    main(sys.argv)
