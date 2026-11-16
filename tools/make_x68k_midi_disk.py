#!/usr/bin/env python3
"""Writes core/test/x68k/midi.xdf, the shared PC boot test's X68000 MIDI program disk (Kairo68#97): a
1232 KB floppy with MIDI68.R, 68000 code for Human68k that drives the MIDI interface's YM3802 (CZ-6BM1,
registers on the odd bytes from $EAFA01): it selects register group 5, turns the transmitter on (TCR),
writes the MIDI bytes of SEQUENCE to the transmit FIFO (TDR), each when the FIFO has room (TSR bit 6),
waits for the last to leave (TSR bit 7 set, bit 0 clear), and ends.

    make_x68k_midi_disk.py

The sequence holds every kind of message the shared cutter (kairo/midi_bytes.h) tells apart: a SysEx,
channel messages of two and three bytes, data bytes under running status, and a real-time byte.
MESSAGES is what the shared sender gets for it.
"""
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "shared/tools"))
from fat_disk import add_file, floppy_2hd  # noqa: E402
from m68k_assembler import BEQ, BNE, Assembler  # noqa: E402

OUTPUT = ROOT / "core/test/x68k/midi.xdf"
YM3802 = 0xEAFA01
# Register offsets, two bytes apart: RGR (group select) at 1; in group 5, TSR at 4, TCR at 5, TDR at 6.
RGR, TSR, TCR, TDR = 1, 4, 5, 6
TX_GROUP, TX_ON = 5, 0x01
ROOM, EMPTY, SENDING = 6, 7, 0  # TSR bits

MESSAGES = [
    "F0 7E 7F 09 01 F7",  # General MIDI on
    "C1 05",              # program change, channel 2
    "91 3C 64",           # note on
    "3C 00",              # under running status: note off as a note on of velocity 0
    "3E 64",              # and another note on
    "F8",                 # timing clock, alone
    "B1 07 7F",           # volume
    "81 3E 00",           # note off
]
SEQUENCE = bytes(int(byte, 16) for message in MESSAGES for byte in message.split())


def program():
    a = Assembler()
    e = a.emit

    e(0x42A7, 0xFF20, 0x588F)                       # clr.l -(sp); DOS _SUPER; addq.l #4, sp
    e(0x41F9, YM3802 >> 16, YM3802 & 0xFFFF)        # lea $EAFA01, a0
    e(0x117C, TX_GROUP, 2 * RGR)                    # move.b #5, 2(a0): register group 5
    e(0x117C, TX_ON, 2 * TCR)                       # move.b #1, 10(a0): the transmitter on
    a.lea_pc("sequence", 1)                         # lea sequence(pc), a1
    e(0x323C, len(SEQUENCE) - 1)                    # move.w #length-1, d1
    a.label("next")
    a.label("room")
    e(0x0828, ROOM, 2 * TSR); a.branch(BEQ, "room")  # btst #6, 8(a0): the FIFO has room
    e(0x1159, 2 * TDR)                              # move.b (a1)+, 12(a0)
    a.dbra(1, "next")
    a.label("drain")
    e(0x0828, EMPTY, 2 * TSR); a.branch(BEQ, "drain")    # btst #7, 8(a0): the FIFO is empty
    a.label("sent")
    e(0x0828, SENDING, 2 * TSR); a.branch(BNE, "sent")   # btst #0, 8(a0): the last byte has left
    e(0xFF00)                                       # DOS _EXIT
    a.data("sequence", SEQUENCE)
    return a.build()


def main(argv):
    if len(argv) != 1:
        raise SystemExit(__doc__)
    code = program()
    image = floppy_2hd()
    add_file(image, 0, "MIDI68.R", code)
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT.write_bytes(bytes(image))
    print(f"{OUTPUT}: MIDI68.R, {len(code)} bytes")


if __name__ == "__main__":
    main(sys.argv)
