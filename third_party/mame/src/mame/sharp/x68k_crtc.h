// license:BSD-3-Clause
// copyright-holders:Barry Rodewald,Carl

#ifndef MAME_SHARP_X68K_CRTC_H
#define MAME_SHARP_X68K_CRTC_H

class x68k_crtc_device : public device_t, public device_video_interface
{
public:
	// device callback configuration
	auto vdisp_cb() { return m_vdisp_callback.bind(); }
	auto rint_cb() { return m_rint_callback.bind(); }
	auto hsync_cb() { return m_hsync_callback.bind(); }
	auto tvram_read_cb() { return m_tvram_read_callback.bind(); }
	auto tvram_write_cb() { return m_tvram_write_callback.bind(); }
	auto gvram_read_cb() { return m_gvram_read_callback.bind(); }
	auto gvram_write_cb() { return m_gvram_write_callback.bind(); }

	// clock configuration
	void set_clock_69m(uint32_t clock) { m_clock_69m = clock; }
	void set_clock_69m(const XTAL &xtal) { set_clock_69m(xtal.value()); }
	void set_clock_50m(uint32_t clock) { m_clock_50m = clock; }
	void set_clock_50m(const XTAL &xtal) { set_clock_50m(xtal.value()); }
	u32 clock_39m() const { return clock(); }
	u32 clock_69m() const { return m_clock_69m; }
	u32 clock_50m() const { return m_clock_50m; }

	void crtc_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	u16 crtc_r(offs_t offset);
	void gvram_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	u16 gvram_r(offs_t offset);
	void tvram_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	u16 tvram_r(offs_t offset);

	// Kairo68: draws the lines the beam has passed and are not drawn yet; call before anything the
	// picture shows changes (mame_patches.py X68K_DEFERRED_PARTIAL).
	void flush_partial();
	void draw_passed_lines();  // Kairo68 (x68k-unchanged-frame)
	// Kairo68: the scroll registers (R10 to R19) a line is drawn with: those in effect when the beam
	// passed it, recorded by a scroll register write that leaves the line waiting, or the current
	// ones (mame_patches.py X68K_LINE_SCROLL).
	bool has_scroll_records(int first, int last) const { return m_scroll_last >= 0 && first <= m_scroll_last && last >= m_scroll_first; }
	// Kairo68: the driver's test of whether lines can wait for drawing with their scroll registers
	// recorded; without one, every write draws them first.
	void set_scroll_can_wait(std::function<bool ()> test) { m_scroll_can_wait = std::move(test); }
	const u16 *scroll_regs(int line) const { return (line >= m_scroll_first && line <= m_scroll_last) ? m_line_scroll[line] : &m_reg[10]; }
	// Kairo68 (x68k-unchanged-frame): a count of the changes to anything the picture shows
	// (flush_partial and record_scroll run before each), the count when the frame began (line 0),
	// and whether the picture is interlaced, whose fields alternate by themselves.
	uint64_t picture_generation() const { return m_picture_generation; }
	uint64_t frame_start_generation() const { return m_frame_start_generation; }
	bool interlaced() const { return m_interlace; }

	// getters
	u16 xscr_text() const { return m_reg[10]; }
	u16 xscr_text(int line) const { return scroll_regs(line)[0]; }
	u16 yscr_text(int line) const { return scroll_regs(line)[1]; }
	u16 xscr_gfx(int line, int page) const { return scroll_regs(line)[2 + page * 2]; }
	u16 yscr_gfx(int line, int page) const { return scroll_regs(line)[3 + page * 2]; }
	u16 yscr_text() const { return m_reg[11]; }
	u16 xscr_gfx(int page) const { return m_reg[12 + page * 2]; }
	u16 yscr_gfx(int page) const { return m_reg[13 + page * 2]; }
	u8 vfactor() const { return (m_reg[20] & 0x0c) >> 2; }
	bool is_1024x1024() const { return BIT(m_reg[20], 10); }
	// Kairo68 (x68k-gvram-1024): in the 1024x1024 16-colour mode, the pixel at word offset
	// y * 1024 + x of G-VRAM is in the page of its quadrant (0 top left, 1 top right, 2 bottom left,
	// 3 bottom right), at (y & 511) * 512 + (x & 511) of that page, as on the X68000.
	static u8 gvram_1024_page(offs_t offset) { return u8(BIT(offset, 19) << 1 | BIT(offset, 9)); }
	static offs_t gvram_1024_word(offs_t offset) { return ((offset >> 1) & 0x3fe00) | (offset & 0x1ff); }
	bool gfx_double_scan() const { return (m_reg[20] & 0x1e) == 0x10; }
	bool gfx_layer_buffer() const { return BIT(m_reg[20], 11); }
	u8 gfx_color_mode() const { return (m_reg[20] >> 8) & 3; }
	bool text_layer_buffer() const { return BIT(m_reg[20], 12); }
	u16 hbegin() const { return m_hbegin; }
	u16 vbegin() const { return m_vbegin; }
	u16 hend() const { return m_hend; }
	u16 vend() const { return m_vend; }
	u16 visible_height() const { return m_visible_height; }
	u16 visible_width() const { return m_visible_width; }

protected:
	// base constructor
	x68k_crtc_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock);

	// device_t implementation
	virtual void device_start() override ATTR_COLD;
	virtual void device_reset() override ATTR_COLD;
	virtual void device_pre_save() override ATTR_COLD;
	virtual void device_post_load() override ATTR_COLD;

private:
	// internal helpers
	void text_copy(unsigned src, unsigned dest, u8 planes);
	bool text_copy_changes(unsigned src, unsigned dest, u8 planes);  // Kairo68 (x68k-unchanged-frame)
	TIMER_CALLBACK_MEMBER(operation_end);
	void refresh_mode();
	TIMER_CALLBACK_MEMBER(hsync);
	void record_scroll();
	void forget_scroll();
	TIMER_CALLBACK_MEMBER(raster_end);
	TIMER_CALLBACK_MEMBER(raster_irq);
	TIMER_CALLBACK_MEMBER(vblank_irq);

	// device callbacks
	devcb_write_line m_vdisp_callback;
	devcb_write_line m_rint_callback;
	devcb_write_line m_hsync_callback;
	devcb_read16 m_tvram_read_callback;
	devcb_read16 m_gvram_read_callback;
	devcb_write16 m_tvram_write_callback;
	devcb_write16 m_gvram_write_callback;

	// software-selectable oscillators
	u32 m_clock_69m;
	u32 m_clock_50m;

	// internal state
	u16 m_reg[24];  // registers
	u8 m_operation;  // operation port (0xe80481)
	bool m_vblank;  // true if in VBlank
	bool m_hblank;  // true if in HBlank
	u16 m_htotal;  // Horizontal Total (in characters)
	u16 m_vtotal;  // Vertical Total
	u16 m_hbegin;  // Horizontal Begin
	u16 m_vbegin;  // Vertical Begin
	u16 m_hend;    // Horizontal End
	u16 m_vend;    // Vertical End
	u16 m_hsync_end;  // Horizontal Sync End
	u16 m_vsync_end;  // Vertical Sync End
	u16 m_hsyncadjust;  // Horizontal Sync Adjustment
	//float m_hmultiple;  // Horizontal pixel multiplier
	float m_vmultiple;  // Vertical scanline multiplier (x2 for doublescan modes)
	u16 m_height;
	u16 m_width;
	u16 m_visible_height;
	u16 m_visible_width;
	//u16 m_video_width;  // horizontal total (in pixels)
	//u16 m_video_height; // vertical total
	bool m_interlace;  // 1024 vertical resolution is interlaced
	//u16 m_scanline;
	bool m_oddscanline;

	emu_timer *m_scanline_timer;
	int m_partial_line = -1;    // Kairo68: the last line the beam passed that is not drawn yet
	// Kairo68: the scroll registers recorded for lines m_scroll_first to m_scroll_last (none when
	// m_scroll_last < 0), and the last line flush_partial drew this frame.
	u16 m_line_scroll[1024][10];
	int m_scroll_first = 0;
	int m_scroll_last = -1;
	int m_drawn_through = -1;
	uint64_t m_picture_generation = 1, m_frame_start_generation = 0;  // Kairo68 (x68k-unchanged-frame)
	std::function<bool ()> m_scroll_can_wait;
	emu_timer *m_raster_irq_timer;
	emu_timer *m_vblank_irq_timer;
	emu_timer *m_raster_end_timer;
	emu_timer *m_operation_end_timer;
};

class vinas_device : public x68k_crtc_device
{
public:
	vinas_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);
};

class vicon_device : public x68k_crtc_device
{
public:
	vicon_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);
};

// device type definitions
DECLARE_DEVICE_TYPE(VINAS, vinas_device)
DECLARE_DEVICE_TYPE(VICON, vicon_device)

#endif // MAME_SHARP_X68K_CRTC_H
