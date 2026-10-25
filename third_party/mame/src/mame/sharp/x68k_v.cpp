// license:BSD-3-Clause
// copyright-holders:Barry Rodewald,Carl
/*

    Sharp X68000 video functions
    driver by Barry Rodewald

    X68000 video hardware (there are some minor revisions to these custom chips across various X680x0 models):
        Custom sprite controller "Cynthia"
        Custom CRT controller "Vinas / Vicon"
        Custom video controller "VSOP / VIPS"
        Custom video data selector "Cathy"

    In general terms:
        1 "Text" layer - effectively a 4bpp bitmap split into 4 planes at 1bpp each
                         512kB "text" VRAM
                         can write to multiple planes at once
                         can copy one character line to another character line
                         is 1024x1024 in size
        Up to 4 graphic layers - can be 4 layers with a 16 colour palette, 2 layers with a 256 colour palette,
                                 or 1 layer at 16-bit RGB.
                                 512k graphic VRAM
                                 all layers are 512x512, but at 16 colours, the 4 layers can be combined into 1 1024x1024 layer
                                 one or more layers can be cleared at once quickly with a simple hardware function
         2 tilemapped layers - can be 8x8 or 16x16, 16 colours per tile, max 256 colours overall
         1 sprite layer - up to 128 16x16 sprites, 16 colours per sprite, maximum 16 sprites per scanline (not yet implemented).

         Questions: What do the other bits in m_video.reg[2] do?
                    How is the intensity applied during blending if at all?
                    Black appears to be opaque only at priority 2 but not 3, is that right?
                    Are the gfx layers blended from the bottom up or all at once?
                    Special priority in 16bit color mode?

*/

#include "emu.h"
#include "x68k.h"
#include "kairo68_perf.h"

#include "input.h" // for sprite debug keys

//#define VERBOSE 0
#include "logmacro.h"


rgb_t x68k_state::GGGGGRRRRRBBBBBI(uint32_t raw)
{
	uint8_t const i = raw & 1;
	uint8_t const r = pal6bit(((raw >> 5) & 0x3e) | i);
	uint8_t const g = pal6bit(((raw >> 10) & 0x3e) | i);
	uint8_t const b = pal6bit(((raw >> 0) & 0x3e) | i);
	return rgb_t(r, g, b);
}

inline void x68k_state::plot_pixel(bitmap_rgb32 &bitmap, int x, int y, uint32_t color)
{
	bitmap.pix(y, x) = (uint16_t)color;
}
/*
bitmap_rgb32* ::x68k_get_gfx_page(int pri,int type)
{
    if(type == GFX16)
    {
        switch(pri)
        {
        case 0:
            return x68k_gfx_0_bitmap_16;
        case 1:
            return x68k_gfx_1_bitmap_16;
        case 2:
            return x68k_gfx_2_bitmap_16;
        case 3:
            return x68k_gfx_3_bitmap_16;
        default:
            return x68k_gfx_0_bitmap_16;  // should never reach here.
        }
    }
    if(type == GFX256)
    {
        switch(pri)
        {
        case 0:
        case 1:
            return x68k_gfx_0_bitmap_256;
        case 2:
        case 3:
            return x68k_gfx_1_bitmap_256;
        default:
            return x68k_gfx_0_bitmap_256;  // should never reach here.
        }
    }
    if(type == GFX65536)
        return x68k_gfx_0_bitmap_65536;

    return nullptr;  // should never reach here either.
}
*/

uint16_t x68k_state::tvram_read(offs_t offset)
{
	return m_tvram[offset];
}

void x68k_state::tvram_write(offs_t offset, uint16_t data, uint16_t mem_mask)
{
	COMBINE_DATA(&m_tvram[offset]);
}

uint16_t x68k_state::gvram_read(offs_t offset)
{
	return m_gvram[offset];
}

void x68k_state::gvram_write(offs_t offset, uint16_t data, uint16_t mem_mask)
{
	COMBINE_DATA(&m_gvram[offset]);
}

void x68k_state::spritereg_w(offs_t offset, uint16_t data, uint16_t mem_mask)
{
	m_crtc->flush_partial();
	COMBINE_DATA(&m_spritereg[offset]);
	switch(offset)
	{
	case 0x400:
		m_bg0_8->set_scrollx(0,(data - m_video.bg_hstart) & 0x3ff);
		m_bg0_16->set_scrollx(0,(data - m_video.bg_hstart) & 0x3ff);
		break;
	case 0x401:
		m_bg0_8->set_scrolly(0,(data - m_video.bg_vstart) & 0x3ff);
		m_bg0_16->set_scrolly(0,(data - m_video.bg_vstart) & 0x3ff);
		break;
	case 0x402:
		m_bg1_8->set_scrollx(0,(data - m_video.bg_hstart) & 0x3ff);
		m_bg1_16->set_scrollx(0,(data - m_video.bg_hstart) & 0x3ff);
		break;
	case 0x403:
		m_bg1_8->set_scrolly(0,(data - m_video.bg_vstart) & 0x3ff);
		m_bg1_16->set_scrolly(0,(data - m_video.bg_vstart) & 0x3ff);
		break;
	case 0x406:  // BG H-DISP (normally equals CRTC reg 2 value + 4)
		m_video.bg_hstart = (data - 4) * 8 + 1;
		break;
	case 0x407:  // BG V-DISP (like CRTC reg 6)
		m_video.bg_vstart = data;
		break;
	case 0x408:  // BG H/V-Res
		m_video.bg_hvres = data & 0x1f;
		break;
	}
}

uint16_t x68k_state::spritereg_r(offs_t offset)
{
	if(offset >= 0x400 && offset < 0x404)
		return m_spritereg[offset] & 0x3ff;
	return m_spritereg[offset];
}

void x68k_state::spriteram_w(offs_t offset, uint16_t data, uint16_t mem_mask)
{
	m_crtc->flush_partial();
	COMBINE_DATA(m_spriteram+offset);
	m_video.tile8_dirty[offset / 16] = 1;
	m_video.tile16_dirty[offset / 64] = 1;
	if(offset < 0x2000)
	{
		m_bg1_8->mark_all_dirty();
		m_bg1_16->mark_all_dirty();
		m_bg0_8->mark_all_dirty();
		m_bg0_16->mark_all_dirty();
	}
	if(offset >= 0x2000 && offset < 0x3000)
	{
		m_bg1_8->mark_tile_dirty(offset & 0x0fff);
		m_bg1_16->mark_tile_dirty(offset & 0x0fff);
	}
	if(offset >= 0x3000)
	{
		m_bg0_8->mark_tile_dirty(offset & 0x0fff);
		m_bg0_16->mark_tile_dirty(offset & 0x0fff);
	}
}

uint16_t x68k_state::spriteram_r(offs_t offset)
{
	return m_spriteram[offset];
}

bool x68k_state::get_text_pixel(int line, int pixel, uint16_t *pix)
{
	uint32_t loc;  // location in TVRAM
	int bit;
	int divisor = 1;
	if(m_crtc->gfx_double_scan())
		divisor = 2;
	int xscr = (m_crtc->xscr_text() & 0x3ff) + (pixel - m_crtc->hbegin());
	int yscr = (m_crtc->yscr_text() & 0x3ff);

	// adjust for scroll registers
	loc = ((((line- m_crtc->vbegin()) / divisor) + yscr) & 0x3ff) * 64;
	loc += (xscr / 16) & 0x7f;
	loc &= 0xffff;
	bit = 15 - (xscr & 0x0f);
	*pix = (((m_tvram[loc] >> bit) & 0x01) ? 1 : 0)
						+ (((m_tvram[loc+0x10000] >> bit) & 0x01) ? 2 : 0)
						+ (((m_tvram[loc+0x20000] >> bit) & 0x01) ? 4 : 0)
						+ (((m_tvram[loc+0x30000] >> bit) & 0x01) ? 8 : 0);
	// Colour 0 is displayable if the text layer is at the priority level 2
	if(*pix || ((m_video.reg[1] & 0x0c00) == 0x0800))
		return true;
	return false;
}

// Kairo68: get_text_pixel for a whole line, into m_text_line[pixel].
void x68k_state::decode_text_line(int line)
{
	if (m_crtc->hend() >= std::size(m_text_line))
		throw emu_fatalerror("Kairo68: the text layer is %d pixels wide", m_crtc->hend() + 1);
	int divisor = 1;
	if(m_crtc->gfx_double_scan())
		divisor = 2;
	const int yscr = (m_crtc->yscr_text(line) & 0x3ff);
	const uint32_t row = ((((line - m_crtc->vbegin()) / divisor) + yscr) & 0x3ff) * 64;
	const int xorigin = (m_crtc->xscr_text(line) & 0x3ff) - m_crtc->hbegin();
	// Sixteen pixels share each plane word (bit 15 the leftmost): each byte of a plane word gives
	// eight pixels' bit of that plane at once, spread one bit a byte by a table. The words the line
	// covers are decoded whole, then the line is copied from its first pixel's place in the first
	// word (Kairo68#29). xscr is never negative, so xscr / 16 is xscr >> 4.
	static const auto spread = [] {
		std::array<uint64_t, 256> table{};
		for (int b = 0; b < 256; b++)
			for (int i = 0; i < 8; i++)
				if (b & (0x80 >> i))
					table[b] |= uint64_t(1) << (8 * i);
		return table;
	}();
	const int first = m_crtc->hbegin(), last = m_crtc->hend();
	if (last < first)
		return;
	const int first_word = (xorigin + first) >> 4, last_word = (xorigin + last) >> 4;
	uint8_t words[sizeof(m_text_line) + 32];
	if (size_t(last_word - first_word + 1) * 16 > std::size(words))
		throw emu_fatalerror("Kairo68: the text layer line spans %d words", last_word - first_word + 1);
	uint8_t *out = words;
	for (int word = first_word; word <= last_word; word++, out += 16)
	{
		const uint32_t loc = (row + (word & 0x7f)) & 0xffff;
		const uint16_t plane0 = m_tvram[loc], plane1 = m_tvram[loc+0x10000], plane2 = m_tvram[loc+0x20000], plane3 = m_tvram[loc+0x30000];
		const uint64_t left = spread[plane0 >> 8] | (spread[plane1 >> 8] << 1) | (spread[plane2 >> 8] << 2) | (spread[plane3 >> 8] << 3);
		const uint64_t right = spread[plane0 & 0xff] | (spread[plane1 & 0xff] << 1) | (spread[plane2 & 0xff] << 2) | (spread[plane3 & 0xff] << 3);
		for (int i = 0; i < 8; i++)
		{
			out[i] = uint8_t(left >> (8 * i));
			out[8 + i] = uint8_t(right >> (8 * i));
		}
	}
	std::copy_n(words + ((xorigin + first) & 15), last - first + 1, m_text_line + first);
}

bool x68k_state::draw_gfx_scanline( bitmap_ind16 &bitmap, rectangle cliprect, uint8_t priority, int scroll_line)
{
	int pixel;
	uint32_t loc;  // location in GVRAM
	uint32_t lineoffset;
	uint16_t xscr,yscr;
	uint16_t colour = 0;
	int shift;
	bool blend, ret = (m_video.reg[2] & 0x1a00) == 0x1a00;
	uint16_t *pal = (uint16_t *)m_gfxpalette->basemem().base();
	int divisor = 1;
	if(m_crtc->gfx_double_scan())
		divisor = 2;

	for(int scanline=cliprect.min_y;scanline<=cliprect.max_y;scanline++)  // per scanline
	{
		if(m_crtc->is_1024x1024())  // 1024x1024 "real" screen size - use 1024x1024 16-colour gfx layer
		{
			// adjust for scroll registers
			if(m_video.reg[2] & 0x0010 && priority == m_video.gfxlayer_pri[0])
			{
				xscr = (m_crtc->xscr_gfx(scroll_line >= 0 ? scroll_line : scanline, 0) & 0x3ff);
				yscr = (m_crtc->yscr_gfx(scroll_line >= 0 ? scroll_line : scanline, 0) & 0x3ff);
				lineoffset = (((scanline - m_crtc->vbegin()) + yscr) & 0x3ff) * 1024;
				loc = xscr & 0x3ff;
				for(pixel=m_crtc->hbegin();pixel<=m_crtc->hend();pixel++)
				{
					switch(lineoffset & 0xc0000)
					{
					case 0x00000:
						colour = m_gvram[lineoffset + (loc & 0x3ff)] & 0x000f;
						break;
					case 0x40000:
						colour = (m_gvram[(lineoffset - 0x40000) + (loc & 0x3ff)] & 0x00f0) >> 4;
						break;
					case 0x80000:
						colour = (m_gvram[(lineoffset - 0x80000) + (loc & 0x3ff)] & 0x0f00) >> 8;
						break;
					case 0xc0000:
						colour = (m_gvram[(lineoffset - 0xc0000) + (loc & 0x3ff)] & 0xf000) >> 12;
						break;
					}
					if(colour || (priority == 3))
					{
						if(((m_video.reg[2] & 0x1800) == 0x1000) && (colour & 1))
							m_special.pix(scanline, pixel) = colour;
						else
						{
							bitmap.pix(scanline, pixel) = colour;
							m_special.pix(scanline, pixel) = 0;
						}
					}
					loc++;
					loc &= 0x3ff;
				}
				ret = false;
			}
		}
		else  // else 512x512 "real" screen size
		{
			if(m_video.reg[2] & (1 << priority))
			{
				int page = m_video.gfxlayer_pri[priority];
				// Kairo68: the rows each pixel reads and writes, taken once a line; each pixel takes the
				// same steps on the same values as before (Kairo68#18).
				const bool special = ((m_video.reg[2] & 0x1800) == 0x1000);
				uint16_t *const dest = &bitmap.pix(scanline, 0);
				uint16_t *const spec = &m_special.pix(scanline, 0);
				const int first = m_crtc->hbegin(), last = m_crtc->hend();
				// adjust for scroll registers
				switch(m_video.reg[0] & 0x03)
				{
				case 0x00: // 16 colours
				{
					xscr = m_crtc->xscr_gfx(scroll_line >= 0 ? scroll_line : scanline, page) & 0x1ff;
					yscr = m_crtc->yscr_gfx(scroll_line >= 0 ? scroll_line : scanline, page) & 0x1ff;
					lineoffset = (((scanline - m_crtc->vbegin() / divisor) + yscr) & 0x1ff) * 512;
					loc = xscr & 0x1ff;
					shift = 4;
					const uint16_t *const row = &m_gvram[lineoffset];
					const int plane = page*shift;
					if(!ret && !special)
					{
						// Kairo68: without blending or the special priority, a pixel whose colour shows
						// (any, on the bottom page) stores it and clears its special pixel. The same
						// stores in runs that don't wrap GVRAM's row, a form the compiler vectorizes
						// (Kairo68#29).
						uint32_t at = loc;
						for(pixel=first;pixel<=last;)
						{
							const int run = std::min<int>(last - pixel + 1, 0x200 - at);
							// Three different bitmaps: GVRAM, the graphics layer and the special layer.
							const uint16_t *__restrict const src = row + at;
							uint16_t *__restrict const d = dest + pixel;
							uint16_t *__restrict const sp = spec + pixel;
							if(priority == 3)
							{
								for(int i = 0; i < run; i++)
								{
									d[i] = (src[i] >> plane) & 0x000f;
									sp[i] = 0;
								}
							}
							else
							{
								for(int i = 0; i < run; i++)
								{
									// keep is all ones where the colour is 0: the pixel and its special pixel stay.
									const uint16_t c = (src[i] >> plane) & 0x000f;
									const uint16_t keep = uint16_t(-uint16_t(c == 0));
									d[i] = (d[i] & keep) | c;
									sp[i] &= keep;
								}
							}
							pixel += run;
							at = 0;
						}
						break;
					}
					for(pixel=first;pixel<=last;pixel++)
					{
						colour = ((row[loc] >> plane) & 0x000f);
						if(ret && (colour & 1))
						{
							blend = true;
							colour &= 0xfe;
						}
						else
							blend = false;
						if(colour || (priority == 3))
						{
							if(ret)
							{
								if(blend && dest[pixel])
									dest[pixel] = ((dest[pixel] >> 1) & 0x7bde) + ((pal[colour] >> 1) & 0x7bde) + 1;
								else
									dest[pixel] = (pal[colour] & 0xfffe) + blend;
							}
							else if(special && (colour & 1))
								spec[pixel] = colour;
							else
							{
								spec[pixel] = 0;
								dest[pixel] = colour;
							}
						}
						else if(special && spec[pixel])
						{
							dest[pixel] = spec[pixel];
							spec[pixel] = 0;
						}
						loc++;
						loc &= 0x1ff;
					}
					break;
				}
				case 0x01: // 256 colours
					if(page == 0 || page == 2)
					{
						// What effect do priorites and plane disable have here? Does it work in 16bit color mode?
						uint16_t xscr0, yscr0, xscr1, yscr1;
						uint32_t loc0, loc1, lineoffset0, lineoffset1;
						xscr0 = m_crtc->xscr_gfx(scroll_line >= 0 ? scroll_line : scanline, page) & 0x1ff;
						yscr0 = m_crtc->yscr_gfx(scroll_line >= 0 ? scroll_line : scanline, page) & 0x1ff;
						xscr1 = m_crtc->xscr_gfx(scroll_line >= 0 ? scroll_line : scanline, page + 1) & 0x1ff;
						yscr1 = m_crtc->yscr_gfx(scroll_line >= 0 ? scroll_line : scanline, page + 1) & 0x1ff;

						lineoffset0 = (((scanline - m_crtc->vbegin() / divisor) + yscr0) & 0x1ff) * 512;
						loc0 = xscr0 & 0x1ff;
						lineoffset1 = (((scanline - m_crtc->vbegin() / divisor) + yscr1) & 0x1ff) * 512;
						loc1 = xscr1 & 0x1ff;
						shift = 4;
						const uint16_t *const row0 = &m_gvram[lineoffset0];
						const uint16_t *const row1 = &m_gvram[lineoffset1];
						const int plane = page*shift;
						for(pixel=first;pixel<=last;pixel++)
						{
							colour = ((row0[loc0] >> plane) & 0x000f);
							colour |= ((row1[loc1] >> plane) & 0x00f0);
							if(ret && (colour & 1))
							{
								blend = true;
								colour &= 0xfe;
							}
							else
								blend = false;
							if(colour || (priority == 3))
							{
								if(ret)
								{
									if(blend && dest[pixel])
										dest[pixel] = ((dest[pixel] >> 1) & 0x7bde) + ((pal[colour] >> 1) & 0x7bde) + 1;
									else
										dest[pixel] = (pal[colour] & 0xfffe) + blend;
								}
								else if(special && (colour & 1))
									spec[pixel] = colour;
								else
								{
									dest[pixel] = colour;
									spec[pixel] = 0;
								}
							}
							else if(special && spec[pixel])
							{
								dest[pixel] = spec[pixel];
								spec[pixel] = 0;
							}
							loc0++;
							loc0 &= 0x1ff;
							loc1++;
							loc1 &= 0x1ff;
						}
					}
					break;
				case 0x03: // 65536 colours
				{
					xscr = m_crtc->xscr_gfx(scroll_line >= 0 ? scroll_line : scanline, 0) & 0x1ff;
					yscr = m_crtc->yscr_gfx(scroll_line >= 0 ? scroll_line : scanline, 0) & 0x1ff;
					lineoffset = (((scanline - m_crtc->vbegin() / divisor) + yscr) & 0x1ff) * 512;
					loc = xscr & 0x1ff;
					const uint16_t *const row = &m_gvram[lineoffset];
					for(pixel=first;pixel<=last;pixel++)
					{
						colour = row[loc];
						if(colour || (priority == 3))
							dest[pixel] = colour;
						loc++;
						loc &= 0x1ff;
					}
					ret = false;
					break;
				}
				}
			}
		}
	}
	return ret;
}

// Kairo68: scroll_line is the screen line whose scroll registers draw every row of cliprect, or -1
// for each row's own line (Kairo68#29).
bool x68k_state::draw_gfx(bitmap_rgb32 &bitmap,rectangle cliprect, int scroll_line)
{
	int priority;
	bool gfxblend=false;
	rectangle gfxrect = cliprect;
	if(m_crtc->gfx_double_scan())
	{
		gfxrect.max_y >>= 1;
		gfxrect.min_y >>= 1;
	}

	if(m_crtc->gfx_layer_buffer())  // if graphic layers are set to buffer, then they aren't visible
		return false;

	m_gfxbitmap.fill(0, gfxrect);
	if((m_video.reg[2] & 0x1800) == 0x1000)
		m_special.fill(0, gfxrect);

	for(priority=3;priority>=0;priority--)
	{
		gfxblend = draw_gfx_scanline(m_gfxbitmap,gfxrect,priority,scroll_line);
	}
	return gfxblend;
}

template <bool Blend> rgb_t x68k_state::get_gfx_pixel(int scanline, int pixel, bool gfxblend, rgb_t blendpix)
{
	uint16_t colour;
	int divisor = 1;
	bool blend = false;
	if(m_crtc->gfx_double_scan())
		divisor = 2;
	if((m_video.reg[0] & 0x03) == 3)
	{
		colour = m_gfxbitmap.pix(scanline / divisor, pixel);
		if(colour || (m_video.gfx_pri == 2))
			return GGGGGRRRRRBBBBBI(colour);
	}
	else if(gfxblend)
	{
		colour = m_gfxbitmap.pix(scanline / divisor, pixel);
		if(Blend && (colour & 1))
			blend = true;
		else
			blend = false;
		if(colour || (m_video.gfx_pri == 2))
		{
			if(blend)
				return ((blendpix >> 1) & 0xff7f7f7f) + ((pal555(colour, 6, 11, 1) >> 2) & 0x3f3f3f);
			else
				return pal555(colour, 6, 11, 1);
		}
	}
	else
	{
		colour = m_gfxbitmap.pix(scanline / divisor, pixel) & 0xff;
		if(Blend && (colour & 1))
		{
			blend = true;
			colour &= 0xfe;
		}
		else
			blend = false;
		if((m_gfxpalette->pen(colour) & 0xffffff) || (m_video.gfx_pri == 2))
		{
			if(blend)
				return ((blendpix >> 1) & 0xff7f7f7f) + ((m_gfxpalette->pen(colour) >> 2) & 0x3f3f3f);
			else
				return m_gfxpalette->pen(colour);
		}
	}
	return 0;
}

// Sprite controller "Cynthia" at 0xeb0000
void x68k_state::draw_sprites(bitmap_ind16 &bitmap, screen_device &screen, rectangle cliprect)
{
	/*
	   0xeb0000 - 0xeb07ff - Sprite registers (up to 128)
	       + 00 : b9-0,  Sprite X position
	       + 02 : b9-0,  Sprite Y position
	       + 04 : b15,   Vertical Reversing (flipping?)
	              b14,   Horizontal Reversing
	              b11-8, Sprite colour
	              b7-0,  Sprite tile code (in PCG)
	       + 06 : b1-0,  Priority
	                     00 = Sprite not displayed

	   0xeb0800 - BG0 X Scroll  (10-bit)
	   0xeb0802 - BG0 Y Scroll
	   0xeb0804 - BG1 X Scroll
	   0xeb0806 - BG1 Y Scroll
	   0xeb0808 - BG control
	              b9,    BG/Sprite display (RAM and register access is faster if 1)
	              b4,    PCG area 1 available
	              b3,    BG1 display enable
	              b1,    PCG area 0 available
	              b0,    BG0 display enable
	   0xeb080a - Horizontal total (like CRTC reg 0 - is 0xff if in 256x256?)
	   0xeb080c - Horizontal display position (like CRTC reg 2 - +4)
	   0xeb080e - Vertical display position (like CRTC reg 6)
	   0xeb0810 - Resolution setting
	              b4,    "L/H" (apparently 15kHz/31kHz switch for sprites/BG?)
	              b3-2,  V-Res
	              b1-0,  H-Res (0 = 8x8 tilemaps, 1 = 16x16 tilemaps, 2 or 3 = unknown)
	*/
	int ptr,pri;
	int divisor = 1;
	if((!(m_video.bg_hvres & 0x0c) && m_crtc->gfx_double_scan()) || ((m_video.bg_hvres & 0x1c) == 0x10 && m_crtc->vfactor() == 1))
		divisor = 2;

	for(ptr=0;ptr<=508;ptr+=4)  // stepping through sprites
	{
		pri = m_spritereg[ptr+3] & 0x03;
		if(!pri) continue;
#ifdef MAME_DEBUG
		if(machine().input().code_pressed(KEYCODE_I)) continue;
#endif
		rectangle rect;
		int code = m_spritereg[ptr+2] & 0x00ff;
		int colour = (m_spritereg[ptr+2] & 0x0f00) >> 8;
		int xflip = m_spritereg[ptr+2] & 0x4000;
		int yflip = m_spritereg[ptr+2] & 0x8000;
		int sx = (m_spritereg[ptr+0] & 0x3ff) - 16;
		int sy = (m_spritereg[ptr+1] & 0x3ff) - 16;
		// Kairo68: a sprite whose 16 rows all lie outside the lines being drawn draws nothing; skip
		// it before prio_transpen, which would clip it whole (Kairo68#29).
		const int top = (m_video.bg_vstart / divisor) + sy;
		if (top > cliprect.max_y || top + 15 < cliprect.min_y) continue;

		rect.min_x=m_video.bg_hstart;
		rect.min_y=m_video.bg_vstart;
		rect.max_x=rect.min_x + m_crtc->visible_width()-1;
		rect.max_y=rect.min_y + m_crtc->visible_height()-1;
		int pmask = (pri == 1) ? 0xfffe : (pri == 2) ? 0xfffc : 0xfff0;

		m_gfxdecode->gfx(1)->prio_transpen(bitmap,cliprect,code,colour,xflip,yflip,m_video.bg_hstart+sx,(m_video.bg_vstart / divisor)+sy,screen.priority(),pmask,0x00);
	}
}

void x68k_state::draw_bg(bitmap_ind16 &bitmap, screen_device &screen, int layer, bool opaque, rectangle rect)
{
	int sclx = layer ? m_spritereg[0x402] : m_spritereg[0x400];
	int scly = layer ? m_spritereg[0x403] : m_spritereg[0x401];
	tilemap_t* x68k_bg0;
	tilemap_t* x68k_bg1;
	tilemap_t* map;
	int divisor = 1;
	if((!(m_video.bg_hvres & 0x0c) && m_crtc->gfx_double_scan()) || ((m_video.bg_hvres & 0x1c) == 0x10 && m_crtc->vfactor() == 1))
		divisor = 2;

	if((m_spritereg[0x408] & 0x03) == 0x00)  // Sprite/BG H-Res 0=8x8, 1=16x16, 2 or 3 = undefined.
	{
		x68k_bg0 = m_bg0_8;
		x68k_bg1 = m_bg1_8;
	}
	else
	{
		x68k_bg0 = m_bg0_16;
		x68k_bg1 = m_bg1_16;
	}

	if(layer)
		map = (m_spritereg[0x404] & 0x0030) == 0x10 ? x68k_bg0 : x68k_bg1;
	else
		map = (m_spritereg[0x404] & 0x0006) == 0x02 ? x68k_bg0 : x68k_bg1;

	map->set_scrollx(0,(sclx - m_video.bg_hstart) & 0x3ff);
	map->set_scrolly(0,(scly - (m_video.bg_vstart / divisor)) & 0x3ff);
	map->draw(screen, bitmap, rect, opaque ? TILEMAP_DRAW_OPAQUE : 0, !layer ? 2 : 1);
}

static const gfx_layout x68k_pcg_8 =
{
	8,8,
	256,
	4,
	{ 0,1,2,3 },
	{ 8,12,0,4,24,28,16,20 },
	{ 0*32, 1*32, 2*32, 3*32, 4*32, 5*32, 6*32, 7*32 },
	32*8
};

static const gfx_layout x68k_pcg_16 =
{
	16,16,
	256,
	4,
	{ 0,1,2,3 },
	{ 8,12,0,4,24,28,16,20,8+64*8,12+64*8,64*8,4+64*8,24+64*8,28+64*8,16+64*8,20+64*8 },
	{ 0*32, 1*32, 2*32, 3*32, 4*32, 5*32, 6*32, 7*32,
	8*32, 9*32, 10*32, 11*32, 12*32, 13*32, 14*32, 15*32 },
	128*8
};

TILE_GET_INFO_MEMBER(x68k_state::get_bg0_tile)
{
	int code = m_spriteram[0x3000+tile_index] & 0x00ff;
	int colour = (m_spriteram[0x3000+tile_index] & 0x0f00) >> 8;
	int flags = (m_spriteram[0x3000+tile_index] & 0xc000) >> 14;
	tileinfo.set(0,code,colour,flags);
}

TILE_GET_INFO_MEMBER(x68k_state::get_bg1_tile)
{
	int code = m_spriteram[0x2000+tile_index] & 0x00ff;
	int colour = (m_spriteram[0x2000+tile_index] & 0x0f00) >> 8;
	int flags = (m_spriteram[0x2000+tile_index] & 0xc000) >> 14;
	tileinfo.set(0,code,colour,flags);
}

TILE_GET_INFO_MEMBER(x68k_state::get_bg0_tile_16)
{
	int code = m_spriteram[0x3000+tile_index] & 0x00ff;
	int colour = (m_spriteram[0x3000+tile_index] & 0x0f00) >> 8;
	int flags = (m_spriteram[0x3000+tile_index] & 0xc000) >> 14;
	tileinfo.set(1,code,colour,flags);
}

TILE_GET_INFO_MEMBER(x68k_state::get_bg1_tile_16)
{
	int code = m_spriteram[0x2000+tile_index] & 0x00ff;
	int colour = (m_spriteram[0x2000+tile_index] & 0x0f00) >> 8;
	int flags = (m_spriteram[0x2000+tile_index] & 0xc000) >> 14;
	tileinfo.set(1,code,colour,flags);
}

void x68k_state::video_start()
{
	int gfx_index;

	// Kairo68: scroll register writes may leave lines waiting, with the registers recorded, except
	// in the special priority mode (screen_update).
	m_crtc->set_scroll_can_wait([this] { return (m_video.reg[2] & 0x1800) != 0x1000; });

	for (gfx_index = 0; gfx_index < MAX_GFX_ELEMENTS; gfx_index++)
		if (m_gfxdecode->gfx(gfx_index) == nullptr)
			break;

	/* create the char set (gfx will then be updated dynamically from RAM) */
	m_gfxdecode->set_gfx(gfx_index, std::make_unique<gfx_element>(m_pcgpalette, x68k_pcg_8, memregion("user1")->base(), 0, 32, 0));

	gfx_index++;

	m_gfxdecode->set_gfx(gfx_index, std::make_unique<gfx_element>(m_pcgpalette, x68k_pcg_16, memregion("user1")->base(), 0, 32, 0));
	m_gfxdecode->gfx(gfx_index)->set_colors(32);

	/* Tilemaps */
	m_bg0_8 =  &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(x68k_state::get_bg0_tile)),    TILEMAP_SCAN_ROWS,  8,  8, 64, 64);
	m_bg1_8 =  &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(x68k_state::get_bg1_tile)),    TILEMAP_SCAN_ROWS,  8,  8, 64, 64);
	m_bg0_16 = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(x68k_state::get_bg0_tile_16)), TILEMAP_SCAN_ROWS, 16, 16, 64, 64);
	m_bg1_16 = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(x68k_state::get_bg1_tile_16)), TILEMAP_SCAN_ROWS, 16, 16, 64, 64);

	m_bg0_8->set_transparent_pen(0);
	m_bg1_8->set_transparent_pen(0);
	m_bg0_16->set_transparent_pen(0);
	m_bg1_16->set_transparent_pen(0);

	m_screen->register_screen_bitmap(m_pcgbitmap);
	m_screen->register_screen_bitmap(m_gfxbitmap);
	m_screen->register_screen_bitmap(m_special);

//  m_scanline_timer->adjust(attotime::zero, 0, attotime::from_hz(55.45)/568);
}

// Kairo68: after a saved state, connect the FDC to the drive the state selected, and redecode the
// PCG patterns and redraw both BG layers from the PCG and BG RAM it held.
void x68k_state::device_post_load()
{
	m_upd72065->set_floppy(m_fdc.floppy[m_fdc.select_drive]);
	std::fill(std::begin(m_video.tile8_dirty), std::end(m_video.tile8_dirty), 1);
	std::fill(std::begin(m_video.tile16_dirty), std::end(m_video.tile16_dirty), 1);
	m_bg0_8->mark_all_dirty();
	m_bg1_8->mark_all_dirty();
	m_bg0_16->mark_all_dirty();
	m_bg1_16->mark_all_dirty();
}

namespace {

// Kairo68: what screen_update's mix reads for every pixel, taken once a call: nothing it reads
// changes while it runs (Kairo68#18).
struct x68k_mix
{
	bool text_zero_shows, special, gfxzero;
	int gfxmode; // get_gfx_pixel, decided once: 65,536 colours, 16-bit colours (gfx16bcol), or the palette
	const pen_t *pcgpens, *gfxpens;
	const uint8_t *text;

	// get_gfx_pixel<false>.
	ATTR_FORCE_INLINE rgb_t gfx_plain(uint16_t raw) const
	{
		if(gfxmode == 2)
			return (raw || gfxzero) ? x68k_state::GGGGGRRRRRBBBBBI(raw) : rgb_t(0);
		if(gfxmode == 1)
			return (raw || gfxzero) ? pal555(raw, 6, 11, 1) : rgb_t(0);
		const pen_t pen = gfxpens[raw & 0xff];
		return ((pen & 0xffffff) || gfxzero) ? rgb_t(pen) : rgb_t(0);
	}

	// get_gfx_pixel<true>.
	ATTR_FORCE_INLINE rgb_t gfx_blend(uint16_t raw, rgb_t blendpix) const
	{
		if(gfxmode == 2)
			return (raw || gfxzero) ? x68k_state::GGGGGRRRRRBBBBBI(raw) : rgb_t(0);
		if(gfxmode == 1)
		{
			if(!(raw || gfxzero))
				return rgb_t(0);
			if(raw & 1)
				return ((blendpix >> 1) & 0xff7f7f7f) + ((pal555(raw, 6, 11, 1) >> 2) & 0x3f3f3f);
			return pal555(raw, 6, 11, 1);
		}
		uint16_t colour = raw & 0xff;
		const bool half = colour & 1;
		if(half)
			colour &= 0xfe;
		const pen_t pen = gfxpens[colour];
		if(!((pen & 0xffffff) || gfxzero))
			return rgb_t(0);
		if(half)
			return ((blendpix >> 1) & 0xff7f7f7f) + ((pen >> 2) & 0x3f3f3f);
		return rgb_t(pen);
	}
};

// Kairo68: one line of screen_update's mix, with the layers' priority levels (-1 for a hidden
// layer) and the blend mode as constants: the same steps on the same values for every pixel, with
// the per-pixel tests of which layer a level holds decided by the compiler (Kairo68#29).
template <int TextPrio, int PcgPrio, int GfxPrio, bool Blend>
void x68k_mix_line(const x68k_mix &m, uint32_t *dest, const uint16_t *pcgrow, const uint16_t *specialrow, const uint16_t *gfxrow, int first, int last)
{
	for(int pixel=first;pixel<=last;pixel++)
	{
		if(m.special) // special priority
		{
			uint16_t colour = specialrow[pixel] & 0xff;
			// XXX: this might check the pen color not the palette index
			if(colour & ~1)
			{
				dest[pixel] = m.gfxpens[colour & ~1];
				continue;
			}
		}
		rgb_t outpix = dest[pixel], pix = 0;
		bool pcgpix = false, gfxpix = false;
		for(int priority = 0; priority < 3; priority++)
		{
			if(Blend)
			{
				if((priority == TextPrio) && !pcgpix)
				{
					const uint16_t colour = m.text[pixel];
					if(colour || m.text_zero_shows)
					{
						pix = m.pcgpens[colour];
						pcgpix = true;
					}
				}
				else if((priority == PcgPrio) && !pcgpix)
				{
					uint16_t colour = pcgrow[pixel] & 0xff;
					if(colour)
					{
						pix = m.pcgpens[colour];
						pcgpix = true;
					}
					else if(outpix && (PcgPrio > TextPrio) && (TextPrio != -1))
						pix = outpix;
				}
				else if(priority == GfxPrio)
					gfxpix = true;
				if((pix & 0xffffff) || ((priority == 2) && gfxpix))
				{
					if(!(pix & 0xffffff))
						pix = outpix;
					rgb_t blendpix = m.gfx_blend(gfxrow[pixel], pix);
					outpix = blendpix & 0xffffff ? blendpix : pix;
					break;
				}
			}
			else
			{
				if((priority == TextPrio) && !pcgpix)
				{
					const uint16_t colour = m.text[pixel];
					if(colour || m.text_zero_shows)
					{
						pix = m.pcgpens[colour];
						pcgpix = true;
					}
				}
				else if((priority == PcgPrio) && !pcgpix)
				{
					uint16_t colour = pcgrow[pixel] & 0xff;
					if(colour)
					{
						pix = m.pcgpens[colour];
						pcgpix = true;
					}
					else if(outpix && (PcgPrio > TextPrio) && (TextPrio != -1))
						pix = outpix;
				}
				else if(priority == GfxPrio)
					pix = m.gfx_plain(gfxrow[pixel]);
				if(pix & 0xffffff)
				{
					outpix = pix;
					break;
				}
			}
		}
		dest[pixel] = outpix;
	}
}

using x68k_mix_fn = void (*)(const x68k_mix &, uint32_t *, const uint16_t *, const uint16_t *, const uint16_t *, int, int);

// The levels screen_update gives the shown layers are 0, 1, 2 in order, so these are every case.
x68k_mix_fn x68k_mixer(int textprio, int pcgprio, int gfxprio, bool blend)
{
	switch((textprio + 1) * 16 + (pcgprio + 1) * 4 + (gfxprio + 1))
	{
#define X68K_MIXER(t, p, g) case (t + 1) * 16 + (p + 1) * 4 + (g + 1): 		return blend ? &x68k_mix_line<t, p, g, true> : &x68k_mix_line<t, p, g, false>;
	X68K_MIXER(-1, -1, -1)
	X68K_MIXER(0, -1, -1) X68K_MIXER(-1, 0, -1) X68K_MIXER(-1, -1, 0)
	X68K_MIXER(0, 1, -1) X68K_MIXER(1, 0, -1) X68K_MIXER(0, -1, 1) X68K_MIXER(1, -1, 0) X68K_MIXER(-1, 0, 1) X68K_MIXER(-1, 1, 0)
	X68K_MIXER(0, 1, 2) X68K_MIXER(0, 2, 1) X68K_MIXER(1, 0, 2) X68K_MIXER(1, 2, 0) X68K_MIXER(2, 0, 1) X68K_MIXER(2, 1, 0)
#undef X68K_MIXER
	}
	throw emu_fatalerror("Kairo68: layer priority levels text %d, sprites %d, graphics %d", textprio, pcgprio, gfxprio);
}

} // anonymous namespace

uint32_t x68k_state::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	KAIRO68_PERF_COUNT(screen_updates, 1);
	KAIRO68_PERF_COUNT(screen_lines, cliprect.height());
	KAIRO68_PERF_TIME(screen_ns);
#ifdef KAIRO68_PERF
	uint64_t kairo68_stage = kairo68::perf::now_ns();
	auto kairo68_lap = [&kairo68_stage](uint64_t &into) {
		const uint64_t now = kairo68::perf::now_ns();
		into += now - kairo68_stage;
		kairo68_stage = now;
	};
#define KAIRO68_SCREEN_LAP(field) kairo68_lap(kairo68::perf::counters().field)
#else
#define KAIRO68_SCREEN_LAP(field) ((void)0)
#endif
	rectangle rect(0,0,0,0);
	int priority;
	int x;
	int pixel = 0, scanline = 0;
	//uint8_t *rom;

	if(m_sysport.contrast == 0)  // if monitor contrast is 0, then don't bother displaying anything
		return 0;

	rect.min_x=m_crtc->hbegin();
	rect.min_y=m_crtc->vbegin();
	rect.max_x=m_crtc->hend();
	rect.max_y=m_crtc->vend();

	if(rect.min_y < cliprect.min_y)
		rect.min_y = cliprect.min_y;
	if(rect.max_y > cliprect.max_y)
		rect.max_y = cliprect.max_y;

	// update tiles
	for(x=0;x<256;x++)
	{
		if(m_video.tile16_dirty[x] != 0)
		{
			m_gfxdecode->gfx(1)->mark_dirty(x);
			m_video.tile16_dirty[x] = 0;
		}
		if(m_video.tile8_dirty[x] != 0)
		{
			m_gfxdecode->gfx(0)->mark_dirty(x);
			m_video.tile8_dirty[x] = 0;
		}
	}

	bool clear = false;
	if(m_video.reg[2] & 0x0040)
	{
		rectangle pcgrect = rect;
		if(!(m_video.bg_hvres & 0x0c) && m_crtc->gfx_double_scan())
		{
			pcgrect.max_y >>= 1;
			pcgrect.min_y >>= 1;
		}
		if(m_spritereg[0x404] & 0x0008)
		{
			clear = true;
			draw_bg(m_pcgbitmap, screen, 1, true, pcgrect);
		}
		else if(m_spritereg[0x404] & 0x0001)
		{
			clear = true;
			draw_bg(m_pcgbitmap, screen, 0, true, pcgrect);
		}
	}
	if(clear)
	{
		int divisor = 1;
		if(!(m_video.bg_hvres & 0x0c) && m_crtc->gfx_double_scan())
			divisor = 2;
		// Kairo68: the rows and pens taken once a line (Kairo68#29).
		const pen_t *const pens = m_pcgpalette->pens();
		for(scanline=rect.min_y;scanline<=rect.max_y;scanline++)
		{
			const uint16_t *const src = &m_pcgbitmap.pix(scanline / divisor, 0);
			uint32_t *const dst = &bitmap.pix(scanline, 0);
			for(pixel=m_crtc->hbegin();pixel<=m_crtc->hend();pixel++)
				dst[pixel] = pens[src[pixel] & 0xff];
		}
	}
	else
		bitmap.fill(m_pcgpalette->pen(0), rect);
	KAIRO68_SCREEN_LAP(screen_under_ns);

	bool gfx16bcol = false; // if the gfx layer was blended so gfxbitmap is 16bit color
	int divisor = 1;
	// Kairo68: lines of rect can have scroll registers recorded from when the beam passed them
	// (x68k_crtc_device::record_scroll). Each graphics row is drawn with its line's registers. When
	// the graphics are double-scanned, two screen lines share a row but can have different
	// registers, so each line's row is drawn just before the line is mixed, as a screen_update
	// call for that line alone would have drawn it (Kairo68#29). Records are only made outside
	// the special priority mode, whose mix reads graphics rows by another divisor.
	const bool gfx_per_line = m_crtc->gfx_double_scan() && m_crtc->has_scroll_records(rect.min_y, rect.max_y);
	if (m_crtc->has_scroll_records(rect.min_y, rect.max_y) && ((m_video.reg[2] & 0x1800) == 0x1000))
		throw emu_fatalerror("Kairo68: scroll registers recorded in the special priority mode");
	// Graphics screen(s)
	if (gfx_per_line)
		gfx16bcol = draw_gfx(bitmap, rectangle(rect.min_x, rect.max_x, rect.min_y, rect.min_y), rect.min_y);
	else
		gfx16bcol = draw_gfx(bitmap, rect, m_crtc->gfx_double_scan() ? rect.min_y : -1);
	KAIRO68_SCREEN_LAP(screen_gfx_ns);

	// Sprite / BG Tiles
	if(/*(m_spritereg[0x404] & 0x0200) &&*/ (m_video.reg[2] & 0x0040))
	{
		rectangle pcgrect = rect;
		if((!(m_video.bg_hvres & 0x0c) && m_crtc->gfx_double_scan()) || ((m_video.bg_hvres & 0x1c) == 0x10 && m_crtc->vfactor() == 1) || (m_video.bg_hvres == 1 && m_crtc->vfactor() == 2))
		{
			pcgrect.max_y >>= 1;
			pcgrect.min_y >>= 1;
			divisor = 2;
		}
		m_pcgbitmap.fill(0, pcgrect);
		screen.priority().fill(0, pcgrect);
		if(m_spritereg[0x404] & 0x0008)
			draw_bg(m_pcgbitmap, screen, 1, false, pcgrect);

		if(m_spritereg[0x404] & 0x0001)
			draw_bg(m_pcgbitmap, screen, 0, false, pcgrect);

		draw_sprites(m_pcgbitmap, screen, pcgrect);
	}

	KAIRO68_SCREEN_LAP(screen_pcg_ns);
	int gfxprio = -1, textprio = -1, pcgprio = -1, currprio = 0;

	for(int priority = 0; priority < 3; priority++)
	{
		if((m_video.text_pri == priority) && (m_video.reg[2] & 0x0020) && !m_crtc->text_layer_buffer())
		{
			textprio = currprio;
			currprio++;
		}
		if((m_video.sprite_pri == priority) /*&& (m_spritereg[0x404] & 0x0200)*/ && (m_video.reg[2] & 0x0040))
		{
			pcgprio = currprio;
			currprio++;
		}
		if((m_video.gfx_pri == priority) && !m_crtc->gfx_layer_buffer() && (m_video.reg[2] & 0x1f))
		{
			gfxprio = currprio;
			currprio++;
		}
	}

	bool blend = (((m_video.reg[2] & 0x1900) == 0x1900) && (m_video.gfx_pri != 2));
	x68k_mix mix;
	// Kairo68: colour 0 of the text layer shows at priority level 2 (get_text_pixel).
	mix.text_zero_shows = ((m_video.reg[1] & 0x0c00) == 0x0800);
	mix.special = ((m_video.reg[2] & 0x1800) == 0x1000);
	mix.pcgpens = m_pcgpalette->pens();
	mix.gfxpens = m_gfxpalette->pens();
	mix.gfxmode = ((m_video.reg[0] & 0x03) == 3) ? 2 : (gfx16bcol ? 1 : 0);
	mix.gfxzero = (m_video.gfx_pri == 2);
	mix.text = m_text_line;
	const x68k_mix_fn mixer = x68k_mixer(textprio, pcgprio, gfxprio, blend);
	const int gfxdivisor = m_crtc->gfx_double_scan() ? 2 : 1;
	int gfx_row_line = rect.min_y; // Kairo68: the line whose registers drew the graphics row last
	for(scanline=rect.min_y;scanline<=rect.max_y;scanline++)
	{
		if(gfx_per_line && scanline != rect.min_y)
		{
			const uint16_t *const regs = m_crtc->scroll_regs(scanline);
			if(((scanline >> 1) != (gfx_row_line >> 1)) || !std::equal(regs, regs + 10, m_crtc->scroll_regs(gfx_row_line)))
			{
				draw_gfx(bitmap, rectangle(rect.min_x, rect.max_x, scanline, scanline), scanline);
				gfx_row_line = scanline;
			}
		}
		if(textprio != -1)
			decode_text_line(scanline);
		uint32_t *const dest = &bitmap.pix(scanline, 0);
		const uint16_t *const pcgrow = &m_pcgbitmap.pix(scanline / divisor, 0);
		const uint16_t *const specialrow = &m_special.pix(scanline / divisor, 0);
		const uint16_t *const gfxrow = &m_gfxbitmap.pix(scanline / gfxdivisor, 0);
		mixer(mix, dest, pcgrow, specialrow, gfxrow, m_crtc->hbegin(), m_crtc->hend());
	}
	KAIRO68_SCREEN_LAP(screen_mix_ns);
#undef KAIRO68_SCREEN_LAP

#ifdef MAME_DEBUG
	if(machine().input().code_pressed(KEYCODE_9))
	{
		m_sprite_shift--;
		popmessage("Sprite shift = %i",m_sprite_shift);
	}
	if(machine().input().code_pressed(KEYCODE_0))
	{
		m_sprite_shift++;
		popmessage("Sprite shift = %i",m_sprite_shift);
	}

#endif

#ifdef MAME_DEBUG
//  popmessage("Layer priorities [%04x] - Txt: %i  Spr: %i  Gfx: %i  Layer Pri0-3: %i %i %i %i",m_video.reg[1],m_video.text_pri,m_video.sprite_pri,
//      m_video.gfx_pri,m_video.gfxlayer_pri[0],m_video.gfxlayer_pri[1],m_video.gfxlayer_pri[2],m_video.gfxlayer_pri[3]);
//  popmessage("CRTC regs - %i %i %i %i  - %i %i %i %i - %i - %i",m_crtc.reg[0],m_crtc.reg[1],m_crtc.reg[2],m_crtc.reg[3],
//      m_crtc.reg[4],m_crtc.reg[5],m_crtc.reg[6],m_crtc.reg[7],m_crtc.reg[8],m_crtc.reg[9]);
//  popmessage("Visible resolution = %ix%i (%s) Screen size = %ix%i",m_crtc.visible_width,m_crtc.visible_height,m_crtc.interlace ? "Interlaced" : "Non-interlaced",m_crtc.video_width,m_crtc.video_height);
//  popmessage("VBlank : scanline = %i",m_scanline);
//  popmessage("CRTC/BG compare H-TOTAL %i/%i H-DISP %i/%i V-DISP %i/%i BG Res %02x",m_crtc.reg[0],m_spritereg[0x405],m_crtc.reg[2],m_spritereg[0x406],
//      m_crtc.reg[6],m_spritereg[0x407],m_spritereg[0x408]);
//  popmessage("BG Scroll - BG0 X %i Y %i  BG1 X %i Y %i",m_spriteram[0x400],m_spriteram[0x401],m_spriteram[0x402],m_spriteram[0x403]);
//  popmessage("uPD72065 status = %02x",upd765_status_r(machine(), space, 0));
//  popmessage("Layer enable - 0x%02x",m_video.reg[2] & 0xff);
//  popmessage("Graphic layer scroll - %i, %i - %i, %i - %i, %i - %i, %i",
//      m_crtc.reg[12],m_crtc.reg[13],m_crtc.reg[14],m_crtc.reg[15],m_crtc.reg[16],m_crtc.reg[17],m_crtc.reg[18],m_crtc.reg[19]);
//  popmessage("IOC IRQ status - %02x",m_ioc.irqstatus);
//  popmessage("RAM: mouse data - %02x %02x %02x %02x",m_ram->pointer()[0x931],m_ram->pointer()[0x930],m_ram->pointer()[0x933],m_ram->pointer()[0x932]);
#endif
	return 0;
}
