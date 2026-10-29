"""Kairo68's local changes to the imported MAME sources (third_party/mame), which
shared/tools/local_patches.py applies and checks, and shared/tools/import_mame.py applies after it copies
the subset. Each change replaces text that must occur exactly once in its file, so a MAME update
that moves the code stops the import instead of applying the change in the wrong place.
docs/source-import.md describes each change."""

BASE = 'mame'

# The x68000 driver registers only its video RAM and sprite registers for save states (MAME does
# not mark it MACHINE_SUPPORTS_SAVE). A loaded state then keeps the video controller, system
# ports, I/O controller, ADPCM, PPI and FDC control state, SRAM, and the PCG and BG RAM from before
# the load: the picture can stay black or show the wrong patterns until the game rewrites them.
# Register the rest of the machine's state, and redecode the patterns and redraw the BG layers
# after a load.
X68K_SAVE_STATE = [
    ('src/mame/sharp/x68k.h',
     '''	virtual void video_start() override ATTR_COLD;
''',
     '''	virtual void video_start() override ATTR_COLD;
	virtual void device_post_load() override ATTR_COLD;
''',),
    ('src/mame/sharp/x68k.cpp',
     '''	m_bus_error = false;
	m_led_state = 0;
}''',
     '''	m_bus_error = false;
	m_led_state = 0;

	// Kairo68: the rest of the machine's state, for save states.
	save_pointer(NAME(m_spriteram), memregion("user1")->bytes() / sizeof(m_spriteram[0]));
	save_item(NAME(m_nvram));
	save_item(NAME(m_sysport.sram_writeprotect));
	save_item(NAME(m_sysport.monitor));
	save_item(NAME(m_sysport.contrast));
	save_item(NAME(m_sysport.keyctrl));
	save_item(NAME(m_sysport.cputype));
	save_item(NAME(m_fdc.led_ctrl));
	save_item(NAME(m_fdc.led_eject));
	save_item(NAME(m_fdc.eject));
	save_item(NAME(m_fdc.motor));
	save_item(NAME(m_fdc.control_drives));
	save_item(NAME(m_fdc.select_drive));
	save_item(NAME(m_adpcm.rate));
	save_item(NAME(m_adpcm.pan));
	save_item(NAME(m_adpcm.clock));
	save_item(NAME(m_video.reg));
	save_item(NAME(m_video.text_pri));
	save_item(NAME(m_video.sprite_pri));
	save_item(NAME(m_video.gfx_pri));
	save_item(NAME(m_video.gfxlayer_pri));
	save_item(NAME(m_video.bg_hstart));
	save_item(NAME(m_video.bg_vstart));
	save_item(NAME(m_ioc.irqstatus));
	save_item(NAME(m_ioc.vector));
	save_item(NAME(m_ppi_portc));
	save_item(NAME(m_exp_irq2));
	save_item(NAME(m_exp_irq4));
	save_item(NAME(m_led_state));
	save_item(NAME(m_bus_error));
}''',),
    ('src/mame/sharp/x68k_v.cpp',
     '''//  m_scanline_timer->adjust(attotime::zero, 0, attotime::from_hz(55.45)/568);
}''',
     '''//  m_scanline_timer->adjust(attotime::zero, 0, attotime::from_hz(55.45)/568);
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
}''',),
]

# The uPD765 family does not save its live read/write state (MAME has those registrations commented
# out), the data separator PLL inside it, or which drive it reads: after a load a read in progress
# continues from the state, time and drive of before the load, so it loses the sector it was in the
# middle of (the X68000 then sees End of Cylinder and retries). Save the live states, their PLLs
# and their drives, and start both live states defined: the checkpoint is otherwise uninitialized
# until the first read.
UPD765_SAVE_STATE = [
    ('src/devices/machine/upd765.h',
     '''	upd765_family_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock);

	virtual void device_start() override ATTR_COLD;
	virtual void device_reset() override ATTR_COLD;
''',
     '''	upd765_family_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock);

	virtual void device_start() override ATTR_COLD;
	virtual void device_reset() override ATTR_COLD;
	virtual void device_pre_save() override;
	virtual void device_post_load() override;
''',),
    ('src/devices/machine/upd765.h',
     '''	live_info cur_live, checkpoint_live;
''',
     '''	live_info cur_live, checkpoint_live;
	int cur_live_drive, checkpoint_live_drive; // Kairo68: the flopi[] the live states read, for save states
''',),
    ('src/devices/machine/upd765.cpp',
     '''	save_item(NAME(drive_busy));

	for(int i=0; i != 4; i++) {
		char name[2];''',
     '''	save_item(NAME(drive_busy));

	// Kairo68: the live read/write states, their data separators and drives, for save states.
	live_info *const lives[2] = { &cur_live, &checkpoint_live };
	for(int i = 0; i != 2; i++) {
		live_info &live = *lives[i];
		save_item(NAME(live.tm), i);
		save_item(NAME(live.state), i);
		save_item(NAME(live.next_state), i);
		save_item(NAME(live.shift_reg), i);
		save_item(NAME(live.crc), i);
		save_item(NAME(live.bit_counter), i);
		save_item(NAME(live.byte_counter), i);
		save_item(NAME(live.previous_type), i);
		save_item(NAME(live.data_separator_phase), i);
		save_item(NAME(live.data_bit_context), i);
		save_item(NAME(live.data_reg), i);
		save_item(NAME(live.idbuf), i);
		fdc_pll_t &pll = live.pll;
		save_item(NAME(pll.ctime), i);
		save_item(NAME(pll.period), i);
		save_item(NAME(pll.min_period), i);
		save_item(NAME(pll.max_period), i);
		save_item(NAME(pll.period_adjust_base), i);
		save_item(NAME(pll.phase_adjust), i);
		save_item(NAME(pll.write_start_time), i);
		save_item(NAME(pll.write_buffer), i);
		save_item(NAME(pll.write_position), i);
		save_item(NAME(pll.freq_hist), i);
	}
	save_item(NAME(cur_live_drive));
	save_item(NAME(checkpoint_live_drive));

	for(int i=0; i != 4; i++) {
		char name[2];''',),
    ('src/devices/machine/upd765.cpp',
     '''	cur_live.tm = attotime::never;
	cur_live.state = IDLE;
	cur_live.next_state = -1;
	cur_live.fi = nullptr;

	if(ready_polled) {''',
     '''	cur_live = live_info(); // Kairo68: every field defined, as a saved state holds them
	cur_live.tm = attotime::never;
	cur_live.state = IDLE;
	cur_live.next_state = -1;
	cur_live.fi = nullptr;
	checkpoint_live = cur_live; // Kairo68

	if(ready_polled) {''',),
    ('src/devices/machine/upd765.cpp',
     '''void upd765_family_device::device_reset()
{''',
     '''// Kairo68: the live states point at their drive; a saved state holds its index.
void upd765_family_device::device_pre_save()
{
	cur_live_drive = cur_live.fi ? cur_live.fi->id : -1;
	checkpoint_live_drive = checkpoint_live.fi ? checkpoint_live.fi->id : -1;
}

void upd765_family_device::device_post_load()
{
	cur_live.fi = cur_live_drive >= 0 ? &flopi[cur_live_drive] : nullptr;
	checkpoint_live.fi = checkpoint_live_drive >= 0 ? &flopi[checkpoint_live_drive] : nullptr;
}

void upd765_family_device::device_reset()
{''',),
]

# device_rtc_interface counts the date and time in m_register, which no saved state holds: after a
# load the clock ticks on from the time before the load and overwrites the chip's restored time
# registers. The RP5C15 also leaves out its 13 bytes of RAM. Save both.
RTC_SAVE_STATE = [
    ('src/emu/dirtc.h',
     '''	virtual void rtc_clock_updated(int year, int month, int day, int day_of_week, int hour, int minute, int second) = 0;
''',
     '''	virtual void rtc_clock_updated(int year, int month, int day, int day_of_week, int hour, int minute, int second) = 0;

	// Kairo68: the date and time, for save states
	virtual void interface_post_start() override;
''',),
    ('src/emu/dirtc.cpp',
     '''device_rtc_interface::~device_rtc_interface()
{
}''',
     '''device_rtc_interface::~device_rtc_interface()
{
}


//-------------------------------------------------
//  interface_post_start - Kairo68: register the
//  date and time for save states
//-------------------------------------------------

void device_rtc_interface::interface_post_start()
{
	device().save_item(m_register, "rtc.m_register");
}''',),
    ('src/devices/machine/rp5c15.cpp',
     '''	save_item(NAME(m_clkout));''',
     '''	save_item(NAME(m_clkout));
	save_item(NAME(m_ram)); // Kairo68''',),
]

# The floppy drive saves its flux cache but not where the cache's weak (no-flux) zone starts, nor
# whether the current track has unwritten changes: after a load, reads in a weak zone time their
# transitions from the zone of before the load.
FLOPPY_SAVE_STATE = [
    ('src/devices/imagedev/floppy.cpp',
     '''	save_item(NAME(m_cache_weak));''',
     '''	save_item(NAME(m_cache_weak));
	save_item(NAME(m_cache_weak_start)); // Kairo68
	save_item(NAME(m_track_dirty)); // Kairo68''',),
]

# The HD63450 DMA controller leaves out its bus-error code, which stays latched until a reset and
# stops transfers, and its DTACK line.
HD63450_SAVE_STATE = [
    ('src/devices/machine/hd63450.cpp',
     '''	save_item(NAME(m_irq_channel));''',
     '''	save_item(NAME(m_irq_channel));
	save_item(NAME(m_bec)); // Kairo68
	save_item(NAME(m_dtack)); // Kairo68''',),
]

# The IPL boots from SASI hard disks only when SRAM says some are connected (byte 0x5a), which the
# owner sets with SWITCH.X; MAME's SRAM starts blank, so an attached .hdf never boots. Report the
# attached drive in that byte, as the driver already reports the configured RAM size at 0x08.
X68K_SASI_COUNT = [
    ('src/mame/sharp/x68k.h',
     '''	bool m_bus_error = false;
''',
     '''	bool m_bus_error = false;
	device_image_interface *m_sasi = nullptr; // Kairo68: the SASI hard disk, for the SRAM drive count
''',),
    ('src/mame/sharp/x68k.cpp',
     '''	m_fdc.motor = 0;
''',
     '''	m_fdc.motor = 0;
	m_sasi = subdevice<device_image_interface>("x68k_hdc");
''',),
    ('src/mame/sharp/x68k.cpp',
     '''	if(offset == 0x08/2)
		return m_ram->size() >> 16;  // RAM size
	return m_nvram[offset];''',
     '''	if(offset == 0x08/2)
		return m_ram->size() >> 16;  // RAM size
	if(offset == 0x5a/2)  // Kairo68: the number of SASI hard disks, in the high byte
		return (m_nvram[offset] & 0x00ff) | ((m_sasi && m_sasi->exists()) ? 0x0100 : 0x0000);
	return m_nvram[offset];''',),
]

# The SASI controller's REQ line requests DMA channel 1 for each data byte, and the IPL and Human68k
# move SASI data that way (external request mode), but MAME never connects it: a hard disk read
# waits forever. Drive DRQ from REQ during the data phases and connect it to the HD63450's DRQ1.
X68K_SASI_DMA = [
    ('src/mame/sharp/x68k_hdc.h',
     '''	void hdc_w(offs_t offset, u16 data);
	u16 hdc_r(offs_t offset);
''',
     '''	void hdc_w(offs_t offset, u16 data);
	u16 hdc_r(offs_t offset);

	// Kairo68: DMA request, REQ during a data phase
	auto drq_callback() { return m_drq_cb.bind(); }
''',),
    ('src/mame/sharp/x68k_hdc.h',
     '''	TIMER_CALLBACK_MEMBER(req_timer_callback);
''',
     '''	TIMER_CALLBACK_MEMBER(req_timer_callback);
	void hdc_access_w(offs_t offset, u16 data);
	u16 hdc_access_r(offs_t offset);
	void update_drq();
''',),
    ('src/mame/sharp/x68k_hdc.h',
     '''	emu_timer *m_req_timer = nullptr;
};''',
     '''	emu_timer *m_req_timer = nullptr;
	devcb_write_line m_drq_cb;
	bool m_drq = false;
};''',),
    ('src/mame/sharp/x68k_hdc.cpp',
     '''	: harddisk_image_base_device(mconfig, X68KHDC, tag, owner, clock)
{
}''',
     '''	: harddisk_image_base_device(mconfig, X68KHDC, tag, owner, clock)
	, m_drq_cb(*this)
{
}

// Kairo68: the DMA request follows REQ while data moves.
void x68k_hdc_image_device::update_drq()
{
	bool const drq = (m_phase == sasi_phase::READ || m_phase == sasi_phase::WRITE) && (m_status_port & SASI_STATUS_REQ);
	if(drq != m_drq)
	{
		m_drq = drq;
		m_drq_cb(drq);
	}
}''',),
    ('src/mame/sharp/x68k_hdc.cpp',
     '''	m_status_port |= SASI_STATUS_REQ;
}''',
     '''	m_status_port |= SASI_STATUS_REQ;
	update_drq();
}''',),
    ('src/mame/sharp/x68k_hdc.cpp',
     '''	save_item(NAME(m_sense));''',
     '''	save_item(NAME(m_sense));
	save_item(NAME(m_drq)); // Kairo68''',),
    ('src/mame/sharp/x68k_hdc.cpp',
     '''void x68k_hdc_image_device::hdc_w(offs_t offset, u16 data)
{''',
     '''void x68k_hdc_image_device::hdc_w(offs_t offset, u16 data)
{
	hdc_access_w(offset, data);
	update_drq();
}

void x68k_hdc_image_device::hdc_access_w(offs_t offset, u16 data)
{''',),
    ('src/mame/sharp/x68k_hdc.cpp',
     '''u16 x68k_hdc_image_device::hdc_r(offs_t offset)
{''',
     '''u16 x68k_hdc_image_device::hdc_r(offs_t offset)
{
	u16 const result = hdc_access_r(offset);
	update_drq();
	return result;
}

u16 x68k_hdc_image_device::hdc_access_r(offs_t offset)
{''',),
    ('src/mame/sharp/x68k_hdc.cpp',
     '''						fseek(lba * 256,SEEK_SET);
						logerror("SASI: READ (LBA 0x%06x, blocks = %i)\\n",lba,m_command[4]);''',
     '''						fseek(lba * 256,SEEK_SET);
						logerror("SASI: READ (LBA 0x%06x, blocks = %i)\\n",lba,m_command[4]);
						m_req_timer->adjust(ACCESS_TIME); // Kairo68''',),
    ('src/mame/sharp/x68k_hdc.cpp',
     '''						fseek(lba * 256,SEEK_SET);
						logerror("SASI: WRITE (LBA 0x%06x, blocks = %i)\\n",lba,m_command[4]);''',
     '''						fseek(lba * 256,SEEK_SET);
						logerror("SASI: WRITE (LBA 0x%06x, blocks = %i)\\n",lba,m_command[4]);
						m_req_timer->adjust(ACCESS_TIME); // Kairo68''',),
    ('src/mame/sharp/x68k_hdc.cpp',
     '''TIMER_CALLBACK_MEMBER(x68k_hdc_image_device::req_timer_callback)''',
     '''// Kairo68: the drive finds the first sector of a read or write before it requests data: the
// average rotational latency of a 3600 rpm disk. The host starts its DMA channel in that time;
// the HD63450 takes the request on its edge, so a request made sooner is never served.
static const attotime ACCESS_TIME = attotime::from_hz(120);

TIMER_CALLBACK_MEMBER(x68k_hdc_image_device::req_timer_callback)''',),
    ('src/mame/sharp/x68k.cpp',
     '''	X68KHDC(config, "x68k_hdc");''',
     '''	X68KHDC(config, "x68k_hdc").drq_callback().set(m_hd63450, FUNC(hd63450_device::drq1_w)); // Kairo68''',),
]

# The POSIX OSD library reaches the clipboard through SDL, which only MAME's SDL OSD links; Kairo68
# supplies its own OSD and has no SDL. Without OSD_SDL, use the clipboard stubs MAME already has
# for Android.
OSDLIB_WITHOUT_SDL = [
    ('src/osd/modules/lib/osdlib_unix.cpp',
     '''#ifdef SDLMAME_SDL3
#include <SDL3/SDL.h>
#else
#include <SDL2/SDL.h>
#endif''',
     '''#if !defined(OSD_SDL) // Kairo68: no SDL without the SDL OSD
#elif defined(SDLMAME_SDL3)
#include <SDL3/SDL.h>
#else
#include <SDL2/SDL.h>
#endif''',),
    ('src/osd/modules/lib/osdlib_unix.cpp',
     '''#ifdef SDLMAME_ANDROID
std::string osd_get_clipboard_text() noexcept''',
     '''#if defined(SDLMAME_ANDROID) || !defined(OSD_SDL) // Kairo68
std::string osd_get_clipboard_text() noexcept''',),
]

# Measurement counters (Kairo68#28, core/kairo68_perf.h): the 68000's instructions, the
# scheduler's CPU runs and timer callbacks, x68k_state::screen_update's calls and lines, and the
# sound manager's update, with their host time. Empty unless the build defines KAIRO68_PERF.
PERF_COUNTERS = [
    ('src/emu/schedule.cpp',
     """#include "emu.h"
#include "debugger.h"
""",
     """#include "emu.h"
#include "debugger.h"
#include "kairo68_perf.h"
""",),
    ('src/emu/schedule.cpp',
     """						m_executing_device = exec;
						*exec->m_icountptr = exec->m_cycles_running;
						if (!call_debugger)
							exec->run();""",
     """						m_executing_device = exec;
						*exec->m_icountptr = exec->m_cycles_running;
						KAIRO68_PERF_COUNT(slices, 1);
						KAIRO68_PERF_TIME(cpu_ns);
						if (!call_debugger)
							exec->run();""",),
    ('src/emu/schedule.cpp',
     """				LOG("execute_timers: timer callback %s\\n", timer.m_callback.name());
				timer.m_callback(timer.m_param);""",
     """				LOG("execute_timers: timer callback %s\\n", timer.m_callback.name());
#ifdef KAIRO68_PERF
				const uint64_t kairo68_start = kairo68::perf::now_ns();
#endif
				timer.m_callback(timer.m_param);
#ifdef KAIRO68_PERF
				const uint64_t kairo68_ns = kairo68::perf::now_ns() - kairo68_start;
				auto &kairo68_counters = kairo68::perf::counters();
				++kairo68_counters.timer_fires;
				kairo68_counters.timer_ns += kairo68_ns;
				auto &kairo68_timer = kairo68_counters.timers[timer.m_callback.name()];
				++kairo68_timer.first;
				kairo68_timer.second += kairo68_ns;
#endif""",),
    ('src/devices/cpu/m68000/m68000.cpp',
     """#include "emu.h"
#include "m68000.h"
""",
     """#include "emu.h"
#include "m68000.h"
#include "kairo68_perf.h"
""",),
    ('src/devices/cpu/m68000/m68000.cpp',
     """				m_ipc = m_pc - 2;
				m_irdi = m_ird;

				if(debugger_enabled())
					debugger_instruction_hook(m_ipc);""",
     """				m_ipc = m_pc - 2;
				m_irdi = m_ird;
				KAIRO68_PERF_COUNT(instructions, 1);
				KAIRO68_PERF_PC(m_ipc);

				if(debugger_enabled())
					debugger_instruction_hook(m_ipc);""",),
    ('src/emu/sound.cpp',
     """#include "emu.h"
""",
     """#include "emu.h"
#include "kairo68_perf.h"
""",),
    ('src/emu/sound.cpp',
     """void sound_manager::update(s32)
{
	auto profile = g_profiler.start(PROFILER_SOUND);
""",
     """void sound_manager::update(s32)
{
	auto profile = g_profiler.start(PROFILER_SOUND);
	KAIRO68_PERF_TIME(sound_ns);
""",),
    ('src/mame/sharp/x68k_v.cpp',
     """#include "emu.h"
#include "x68k.h"
""",
     """#include "emu.h"
#include "x68k.h"
#include "kairo68_perf.h"
""",),
    ('src/mame/sharp/x68k_v.cpp',
     """uint32_t x68k_state::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
""",
     """uint32_t x68k_state::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	KAIRO68_PERF_COUNT(screen_updates, 1);
	KAIRO68_PERF_COUNT(screen_lines, cliprect.height());
	KAIRO68_PERF_TIME(screen_ns);
""",),
]

# The MC68901's timers in delay mode (Kairo68#11). MAME fires a scheduler timer at every
# prescaled count and decrements the main counter, so each count ends the 68000's timeslice: a game
# running timer C at 200 kHz cuts every frame into thousands of slices. In delay mode the timer now
# fires only at the count where the counter elapses; a read of the data register, a control
# register write or a save computes the counter from the time since the last processed count. The
# counts, the elapse times and every value the guest reads stay the same. Event count and pulse
# width modes keep counting each event.
MFP_TIMER_ELAPSE = [
    ('src/devices/machine/mc68901.h',
     """	virtual void device_start() override ATTR_COLD;
	virtual void device_reset() override ATTR_COLD;
""",
     """	virtual void device_start() override ATTR_COLD;
	virtual void device_reset() override ATTR_COLD;
	virtual void device_pre_save() override ATTR_COLD;
""",),
    ('src/devices/machine/mc68901.h',
     """	TIMER_CALLBACK_MEMBER(timer_count);
""",
     """	TIMER_CALLBACK_MEMBER(timer_count);
	// Kairo68: delay mode fires only where the count elapses (tools/patches.py MFP_TIMER_ELAPSE).
	int timer_delay_prescale(int index) const;
	attotime timer_tick(int index) const;
	void sync_timer_count(int index);
	void start_timer_delay(int index);
	void schedule_timer_elapse(int index);
""",),
    ('src/devices/machine/mc68901.h',
     """	u8 m_tmc[4];                    // timer main counters
""",
     """	u8 m_tmc[4];                    // timer main counters
	attotime m_tick_base[4];        // Kairo68: the delay-mode count m_tmc was last brought up to
""",),
    ('src/devices/machine/mc68901.cpp',
     """	save_item(NAME(m_tmc));
""",
     """	save_item(NAME(m_tmc));
	save_item(NAME(m_tick_base));
""",),
    ('src/devices/machine/mc68901.cpp',
     """TIMER_CALLBACK_MEMBER(mc68901_device::timer_count)
{
	if (m_tmc[param] == 0x01)
	{""",
     """// Kairo68: the prescaler of a timer in delay mode, or 0 in any other mode.
int mc68901_device::timer_delay_prescale(int index) const
{
	int mode;
	switch (index)
	{
	case TIMER_A:   mode = m_tacr & 0x0f;           break;
	case TIMER_B:   mode = m_tbcr & 0x0f;           break;
	case TIMER_C:   mode = (m_tcdcr >> 4) & 0x07;   break;
	default:        mode = m_tcdcr & 0x07;          break;
	}
	return (mode >= TCR_TIMER_DELAY_4 && mode <= TCR_TIMER_DELAY_200) ? PRESCALER[mode] : 0;
}

// Kairo68: the time between a delay-mode timer's counts.
attotime mc68901_device::timer_tick(int index) const
{
	return attotime::from_hz(m_timer_clock / timer_delay_prescale(index));
}

// Kairo68: counts down a running delay-mode timer by the counts since m_tick_base. The elapsing
// count is the timer's own callback, so fewer counts than that have passed.
void mc68901_device::sync_timer_count(int index)
{
	if (!timer_delay_prescale(index) || !m_timer[index]->enabled())
		return;
	const attotime tick = timer_tick(index);
	const attoseconds_t elapsed = (machine().time() - m_tick_base[index]).as_attoseconds();
	const u32 counts = u32(elapsed / tick.as_attoseconds());
	if (counts == 0)
		return;
	m_tmc[index] -= u8(counts);
	m_tick_base[index] += tick * counts;
}

// Kairo68: a delay-mode timer counts from now, its first count one tick away.
void mc68901_device::start_timer_delay(int index)
{
	m_tick_base[index] = machine().time();
	schedule_timer_elapse(index);
}

// Kairo68: the count where the main counter is 1 elapses; a counter of 0 counts 256.
void mc68901_device::schedule_timer_elapse(int index)
{
	const u32 counts = m_tmc[index] ? m_tmc[index] : 256;
	m_timer[index]->adjust(timer_tick(index) * counts, index);
}

void mc68901_device::device_pre_save()
{
	for (int index = TIMER_A; index <= TIMER_D; index++)
		sync_timer_count(index);
}

TIMER_CALLBACK_MEMBER(mc68901_device::timer_count)
{
	// Kairo68: in delay mode this is the elapsing count.
	const bool delay = timer_delay_prescale(param) != 0 && !m_timer[param]->enabled();
	if (delay)
		m_tmc[param] = 0x01;

	if (m_tmc[param] == 0x01)
	{""",),
    ('src/devices/machine/mc68901.cpp',
     """		/* load main counter */
		m_tmc[param] = m_tdr[param];
	}
	else
	{
		/* count down */
		m_tmc[param]--;
	}
}""",
     """		/* load main counter */
		m_tmc[param] = m_tdr[param];
	}
	else
	{
		/* count down */
		m_tmc[param]--;
	}

	if (delay)
		start_timer_delay(param);
}""",),
    ('src/devices/machine/mc68901.cpp',
     """	case REGISTER_TADR:  return m_tmc[TIMER_A];
	case REGISTER_TBDR:  return m_tmc[TIMER_B];
	case REGISTER_TCDR:  return m_tmc[TIMER_C];
	case REGISTER_TDDR:  return m_tmc[TIMER_D];
""",
     """	case REGISTER_TADR:  sync_timer_count(TIMER_A); return m_tmc[TIMER_A];
	case REGISTER_TBDR:  sync_timer_count(TIMER_B); return m_tmc[TIMER_B];
	case REGISTER_TCDR:  sync_timer_count(TIMER_C); return m_tmc[TIMER_C];
	case REGISTER_TDDR:  sync_timer_count(TIMER_D); return m_tmc[TIMER_D];
""",),
    ('src/devices/machine/mc68901.cpp',
     """	case REGISTER_TACR:
		m_tacr = data & 0x1f;
""",
     """	case REGISTER_TACR:
		sync_timer_count(TIMER_A);
		m_tacr = data & 0x1f;
""",),
    ('src/devices/machine/mc68901.cpp',
     """				LOG("MC68901 Timer A Delay Mode : %u Prescale\\n", divisor);
				m_timer[TIMER_A]->adjust(attotime::from_hz(m_timer_clock / divisor), TIMER_A, attotime::from_hz(m_timer_clock / divisor));""",
     """				LOG("MC68901 Timer A Delay Mode : %u Prescale\\n", divisor);
				start_timer_delay(TIMER_A);""",),
    ('src/devices/machine/mc68901.cpp',
     """	case REGISTER_TBCR:
		m_tbcr = data & 0x1f;
""",
     """	case REGISTER_TBCR:
		sync_timer_count(TIMER_B);
		m_tbcr = data & 0x1f;
""",),
    ('src/devices/machine/mc68901.cpp',
     """				LOG("MC68901 Timer B Delay Mode : %u Prescale\\n", divisor);
				m_timer[TIMER_B]->adjust(attotime::from_hz(m_timer_clock / divisor), TIMER_B, attotime::from_hz(m_timer_clock / divisor));""",
     """				LOG("MC68901 Timer B Delay Mode : %u Prescale\\n", divisor);
				start_timer_delay(TIMER_B);""",),
    ('src/devices/machine/mc68901.cpp',
     """	case REGISTER_TCDCR:
		m_tcdcr = data & 0x77;
""",
     """	case REGISTER_TCDCR:
		sync_timer_count(TIMER_C);
		sync_timer_count(TIMER_D);
		m_tcdcr = data & 0x77;
""",),
    ('src/devices/machine/mc68901.cpp',
     """				LOG("MC68901 Timer D Delay Mode : %u Prescale\\n", divisor);
				m_timer[TIMER_D]->adjust(attotime::from_hz(m_timer_clock / divisor), TIMER_D, attotime::from_hz(m_timer_clock / divisor));""",
     """				LOG("MC68901 Timer D Delay Mode : %u Prescale\\n", divisor);
				start_timer_delay(TIMER_D);""",),
    ('src/devices/machine/mc68901.cpp',
     """				LOG("MC68901 Timer C Delay Mode : %u Prescale\\n", divisor);
				m_timer[TIMER_C]->adjust(attotime::from_hz(m_timer_clock / divisor), TIMER_C, attotime::from_hz(m_timer_clock / divisor));""",
     """				LOG("MC68901 Timer C Delay Mode : %u Prescale\\n", divisor);
				start_timer_delay(TIMER_C);""",),
]

# The HD63450's external-request channels (Kairo68#11). A transfer started in burst mode, or a
# cycle-steal request, runs the channel's timer at its bus clock, and in external-request mode each
# firing with DRQ low does nothing: the X68000's floppy channel polls every 2 microseconds while it
# waits for the controller, so every 2 microseconds ends the 68000's timeslice. A rising DRQ
# already transfers and restarts the timer (drq_w) and a falling one stops it, so the timer now
# stops at the first firing that finds DRQ low instead of polling. Every transfer happens at the
# same time as before.
DMA_DRQ_POLL = [
    ('src/devices/machine/hd63450.cpp',
     """TIMER_CALLBACK_MEMBER(hd63450_device::dma_transfer_timer)
{
	if (((m_reg[param].ocr & 3) == 2) && !m_drq_state[param])
		return;
	single_transfer(param);
}""",
     """TIMER_CALLBACK_MEMBER(hd63450_device::dma_transfer_timer)
{
	if (((m_reg[param].ocr & 3) == 2) && !m_drq_state[param])
	{
		// Kairo68: DRQ's rising edge transfers and restarts the timer (drq_w).
		m_timer[param]->adjust(attotime::never);
		return;
	}
	single_transfer(param);
}""",),
]

# The X68000's picture drawn when it changes (Kairo68#18). The CRTC's hsync timer drew every
# visible line as the beam passed it (screen().update_partial(scan - 1)): about 460 calls to
# x68k_state::screen_update per frame, each repeating its setup for one line. Now the hsync only
# records the line the beam has passed, and the lines waiting are drawn before anything they show
# changes: video, text and graphics RAM, the CRTC and video controller registers, the palettes,
# sprites and BG, the system port (contrast), and the CRTC's raster copy. The end of the frame draws
# the rest, as before. Every line is drawn from the same state as at its own hsync, so the picture
# is the same.
X68K_DEFERRED_PARTIAL = [
    ('src/mame/sharp/x68k_crtc.h',
     """	void tvram_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	u16 tvram_r(offs_t offset);
""",
     """	void tvram_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	u16 tvram_r(offs_t offset);

	// Kairo68: draws the lines the beam has passed and are not drawn yet; call before anything the
	// picture shows changes (tools/patches.py X68K_DEFERRED_PARTIAL).
	void flush_partial();
""",),
    ('src/mame/sharp/x68k_crtc.h',
     """	emu_timer *m_scanline_timer;
""",
     """	emu_timer *m_scanline_timer;
	int m_partial_line = -1;    // Kairo68: the last line the beam passed that is not drawn yet
""",),
    ('src/mame/sharp/x68k_crtc.cpp',
     """	save_item(NAME(m_oddscanline));
}""",
     """	save_item(NAME(m_oddscanline));
	save_item(NAME(m_partial_line));
}""",),
    ('src/mame/sharp/x68k_crtc.cpp',
     """	if (m_operation & 8)
		text_copy((m_reg[22] & 0xff00) >> 8, (m_reg[22] & 0x00ff), (m_reg[21] & 0xf));

	int scan = screen().vpos();
	if (hstate == 1)
	{
		hsync_time = screen().time_until_pos(scan, m_hend);
		m_scanline_timer->adjust(hsync_time);
		if ((scan != 0) && (scan < m_vend))
			screen().update_partial(scan - 1);
	}""",
     """	if (m_operation & 8)
	{
		flush_partial();
		text_copy((m_reg[22] & 0xff00) >> 8, (m_reg[22] & 0x00ff), (m_reg[21] & 0xf));
	}

	int scan = screen().vpos();
	if (hstate == 1)
	{
		hsync_time = screen().time_until_pos(scan, m_hend);
		m_scanline_timer->adjust(hsync_time);
		if ((scan != 0) && (scan < m_vend))
			m_partial_line = scan - 1;
	}""",),
    ('src/mame/sharp/x68k_crtc.cpp',
     """void x68k_crtc_device::crtc_w(offs_t offset, u16 data, u16 mem_mask)
{
""",
     """void x68k_crtc_device::flush_partial()
{
	if (m_partial_line >= 0)
	{
		const int line = m_partial_line;
		m_partial_line = -1;
		// The line is the frame's while the beam is below it. Once the beam starts the next frame,
		// the frame that ended has drawn every line, and the next frame's are not due yet.
		if (screen().vpos() > line)
			screen().update_partial(line);
	}
}

void x68k_crtc_device::crtc_w(offs_t offset, u16 data, u16 mem_mask)
{
	flush_partial();
""",),
    ('src/mame/sharp/x68k_crtc.cpp',
     """void x68k_crtc_device::gvram_w(offs_t offset, u16 data, u16 mem_mask)
{
""",
     """void x68k_crtc_device::gvram_w(offs_t offset, u16 data, u16 mem_mask)
{
	flush_partial();
""",),
    ('src/mame/sharp/x68k_crtc.cpp',
     """void x68k_crtc_device::tvram_w(offs_t offset, u16 data, u16 mem_mask)
{
""",
     """void x68k_crtc_device::tvram_w(offs_t offset, u16 data, u16 mem_mask)
{
	flush_partial();
""",),
    ('src/mame/sharp/x68k_crtc.cpp',
     """		m_rint_callback(0);
		screen().update_partial(scan - 1);""",
     """		m_rint_callback(0);
		flush_partial();
		screen().update_partial(scan - 1);""",),
    ('src/mame/sharp/x68k.cpp',
     """void x68k_state::sysport_w(offs_t offset, uint16_t data, uint16_t mem_mask)
{
""",
     """void x68k_state::sysport_w(offs_t offset, uint16_t data, uint16_t mem_mask)
{
	m_crtc->flush_partial();
""",),
    ('src/mame/sharp/x68k.cpp',
     """void x68k_state::vid_w(offs_t offset, uint16_t data, uint16_t mem_mask)
{
""",
     """void x68k_state::vid_w(offs_t offset, uint16_t data, uint16_t mem_mask)
{
	m_crtc->flush_partial();
""",),
    ('src/mame/sharp/x68k.cpp',
     """void x68k_state::x68k_map(address_map &map)
{
	x68k_base_map(map);
	map(0xe82000, 0xe821ff).rw(m_gfxpalette, FUNC(palette_device::read16), FUNC(palette_device::write16)).share("gfxpalette");
	map(0xe82200, 0xe823ff).rw(m_pcgpalette, FUNC(palette_device::read16), FUNC(palette_device::write16)).share("pcgpalette");""",
     """void x68k_state::x68k_map(address_map &map)
{
	x68k_base_map(map);
	// Kairo68: lines waiting to be drawn show the colours before the write.
	map(0xe82000, 0xe821ff).r(m_gfxpalette, FUNC(palette_device::read16)).lw16(NAME([this] (offs_t offset, u16 data, u16 mem_mask) {
		m_crtc->flush_partial(); m_gfxpalette->write16(offset, data, mem_mask); })).share("gfxpalette");
	map(0xe82200, 0xe823ff).r(m_pcgpalette, FUNC(palette_device::read16)).lw16(NAME([this] (offs_t offset, u16 data, u16 mem_mask) {
		m_crtc->flush_partial(); m_pcgpalette->write16(offset, data, mem_mask); })).share("pcgpalette");""",),
    ('src/mame/sharp/x68k.cpp',
     """void x68ksupr_state::x68kxvi_map(address_map &map)
{
	x68k_base_map(map);
	map(0xe82000, 0xe821ff).rw(m_gfxpalette, FUNC(palette_device::read16), FUNC(palette_device::write16)).share("gfxpalette");
	map(0xe82200, 0xe823ff).rw(m_pcgpalette, FUNC(palette_device::read16), FUNC(palette_device::write16)).share("pcgpalette");""",
     """void x68ksupr_state::x68kxvi_map(address_map &map)
{
	x68k_base_map(map);
	// Kairo68: lines waiting to be drawn show the colours before the write.
	map(0xe82000, 0xe821ff).r(m_gfxpalette, FUNC(palette_device::read16)).lw16(NAME([this] (offs_t offset, u16 data, u16 mem_mask) {
		m_crtc->flush_partial(); m_gfxpalette->write16(offset, data, mem_mask); })).share("gfxpalette");
	map(0xe82200, 0xe823ff).r(m_pcgpalette, FUNC(palette_device::read16)).lw16(NAME([this] (offs_t offset, u16 data, u16 mem_mask) {
		m_crtc->flush_partial(); m_pcgpalette->write16(offset, data, mem_mask); })).share("pcgpalette");""",),
    ('src/mame/sharp/x68k.cpp',
     """	map(0xe82000, 0xe821ff).rw(m_gfxpalette, FUNC(palette_device::read32), FUNC(palette_device::write32)).share("gfxpalette");
	map(0xe82200, 0xe823ff).rw(m_pcgpalette, FUNC(palette_device::read32), FUNC(palette_device::write32)).share("pcgpalette");""",
     """	// Kairo68: lines waiting to be drawn show the colours before the write.
	map(0xe82000, 0xe821ff).r(m_gfxpalette, FUNC(palette_device::read32)).lw32(NAME([this] (offs_t offset, u32 data, u32 mem_mask) {
		m_crtc->flush_partial(); m_gfxpalette->write32(offset, data, mem_mask); })).share("gfxpalette");
	map(0xe82200, 0xe823ff).r(m_pcgpalette, FUNC(palette_device::read32)).lw32(NAME([this] (offs_t offset, u32 data, u32 mem_mask) {
		m_crtc->flush_partial(); m_pcgpalette->write32(offset, data, mem_mask); })).share("pcgpalette");""",),
    ('src/mame/sharp/x68k_v.cpp',
     """void x68k_state::spritereg_w(offs_t offset, uint16_t data, uint16_t mem_mask)
{
""",
     """void x68k_state::spritereg_w(offs_t offset, uint16_t data, uint16_t mem_mask)
{
	m_crtc->flush_partial();
""",),
    ('src/mame/sharp/x68k_v.cpp',
     """void x68k_state::spriteram_w(offs_t offset, uint16_t data, uint16_t mem_mask)
{
""",
     """void x68k_state::spriteram_w(offs_t offset, uint16_t data, uint16_t mem_mask)
{
	m_crtc->flush_partial();
""",),
]

# The screen's render colour and brightness, read back (Kairo68#20). Kairo68 copies the finished
# picture from the screen's bitmap instead of rendering it, and applies the colour MAME's renderer
# would: the X68000 driver sets the brightness from the monitor contrast for its fades.
SCREEN_COLOR_GETTERS = [
    ('src/emu/screen.h',
     """	void set_brightness(u8 brightness) { m_brightness = brightness; }
""",
     """	void set_brightness(u8 brightness) { m_brightness = brightness; }
	u8 brightness() const { return m_brightness; }   // Kairo68
	rgb_t color() const { return m_color; }           // Kairo68
""",),
]

# Measurement counters for x68k_state::screen_update's stages (Kairo68#18): the layer under BG, the
# graphics pages, BG and sprites, and the per-pixel mix with the text layer. Empty unless the
# build defines KAIRO68_PERF.
PERF_SCREEN_STAGES = [
    ('src/mame/sharp/x68k_v.cpp',
     """	KAIRO68_PERF_TIME(screen_ns);
""",
     """	KAIRO68_PERF_TIME(screen_ns);
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
""",),
    ('src/mame/sharp/x68k_v.cpp',
     """	else
		bitmap.fill(m_pcgpalette->pen(0), rect);
""",
     """	else
		bitmap.fill(m_pcgpalette->pen(0), rect);
	KAIRO68_SCREEN_LAP(screen_under_ns);
""",),
    ('src/mame/sharp/x68k_v.cpp',
     """	gfx16bcol = draw_gfx(bitmap,rect);
""",
     """	gfx16bcol = draw_gfx(bitmap,rect);
	KAIRO68_SCREEN_LAP(screen_gfx_ns);
""",),
    ('src/mame/sharp/x68k_v.cpp',
     """	int gfxprio = -1, textprio = -1, pcgprio = -1, currprio = 0;
""",
     """	KAIRO68_SCREEN_LAP(screen_pcg_ns);
	int gfxprio = -1, textprio = -1, pcgprio = -1, currprio = 0;
""",),
    ('src/mame/sharp/x68k_v.cpp',
     """			bitmap.pix(scanline, pixel) = outpix;
		}
	}
""",
     """			bitmap.pix(scanline, pixel) = outpix;
		}
	}
	KAIRO68_SCREEN_LAP(screen_mix_ns);
#undef KAIRO68_SCREEN_LAP
""",),
]

# The text layer decoded once per line (Kairo68#18). screen_update's mix looked up the text layer
# pixel by pixel through get_text_pixel, which reread the scroll registers and recomputed the line's
# address for every pixel. Each line's text colours are now decoded once, with get_text_pixel's
# expressions, into a buffer the mix reads; whether colour 0 shows is decided once per update.
X68K_TEXT_LINE = [
    ('src/mame/sharp/x68k.h',
     """	bool get_text_pixel(int line, int pixel, uint16_t *pix);
""",
     """	bool get_text_pixel(int line, int pixel, uint16_t *pix);
	// Kairo68: the text layer's colour at each pixel of one line, as get_text_pixel computes it.
	void decode_text_line(int line);
	uint8_t m_text_line[2048];
""",),
    ('src/mame/sharp/x68k_v.cpp',
     """bool x68k_state::draw_gfx_scanline( bitmap_ind16 &bitmap, rectangle cliprect, uint8_t priority)
{""",
     """// Kairo68: get_text_pixel for a whole line, into m_text_line[pixel].
void x68k_state::decode_text_line(int line)
{
	if (m_crtc->hend() >= std::size(m_text_line))
		throw emu_fatalerror("Kairo68: the text layer is %d pixels wide", m_crtc->hend() + 1);
	int divisor = 1;
	if(m_crtc->gfx_double_scan())
		divisor = 2;
	const int yscr = (m_crtc->yscr_text() & 0x3ff);
	const uint32_t row = ((((line - m_crtc->vbegin()) / divisor) + yscr) & 0x3ff) * 64;
	const int xorigin = (m_crtc->xscr_text() & 0x3ff) - m_crtc->hbegin();
	// Sixteen pixels share each plane word: read the four once per word. xscr is never negative,
	// so xscr / 16 is xscr >> 4.
	int word = -1;
	uint16_t plane0 = 0, plane1 = 0, plane2 = 0, plane3 = 0;
	for (int pixel = m_crtc->hbegin(); pixel <= m_crtc->hend(); pixel++)
	{
		const int xscr = xorigin + pixel;
		if ((xscr >> 4) != word)
		{
			word = xscr >> 4;
			const uint32_t loc = (row + (word & 0x7f)) & 0xffff;
			plane0 = m_tvram[loc];
			plane1 = m_tvram[loc+0x10000];
			plane2 = m_tvram[loc+0x20000];
			plane3 = m_tvram[loc+0x30000];
		}
		const int bit = 15 - (xscr & 0x0f);
		m_text_line[pixel] = ((plane0 >> bit) & 0x01) | (((plane1 >> bit) & 0x01) << 1)
							| (((plane2 >> bit) & 0x01) << 2) | (((plane3 >> bit) & 0x01) << 3);
	}
}

bool x68k_state::draw_gfx_scanline( bitmap_ind16 &bitmap, rectangle cliprect, uint8_t priority)
{""",),
    ('src/mame/sharp/x68k_v.cpp',
     """	bool blend = (((m_video.reg[2] & 0x1900) == 0x1900) && (m_video.gfx_pri != 2));
	for(scanline=rect.min_y;scanline<=rect.max_y;scanline++)
	{
		for(pixel=m_crtc->hbegin();pixel<=m_crtc->hend();pixel++)""",
     """	bool blend = (((m_video.reg[2] & 0x1900) == 0x1900) && (m_video.gfx_pri != 2));
	// Kairo68: colour 0 of the text layer shows at priority level 2 (get_text_pixel).
	const bool text_zero_shows = ((m_video.reg[1] & 0x0c00) == 0x0800);
	for(scanline=rect.min_y;scanline<=rect.max_y;scanline++)
	{
		if(textprio != -1)
			decode_text_line(scanline);
		for(pixel=m_crtc->hbegin();pixel<=m_crtc->hend();pixel++)""",),
    ('src/mame/sharp/x68k_v.cpp',
     """				if(blend)
				{
					if((priority == textprio) && !pcgpix)
					{
						uint16_t colour;
						if(get_text_pixel(scanline, pixel, &colour))""",
     """				if(blend)
				{
					if((priority == textprio) && !pcgpix)
					{
						const uint16_t colour = m_text_line[pixel];
						if(colour || text_zero_shows)""",),
    ('src/mame/sharp/x68k_v.cpp',
     """				else
				{
					if((priority == textprio) && !pcgpix)
					{
						uint16_t colour;
						if(get_text_pixel(scanline, pixel, &colour))""",
     """				else
				{
					if((priority == textprio) && !pcgpix)
					{
						const uint16_t colour = m_text_line[pixel];
						if(colour || text_zero_shows)""",),
]

# screen_update's mix with what it reads taken once (Kairo68#18). For every pixel the mix reread
# the video registers, the bitmaps' bases and pitches and the palettes through their accessors, and
# get_gfx_pixel re-derived the graphics layer's double scan, mode and row with a division. These are
# now taken once per update or line; each pixel takes the same steps on the same values as before.
X68K_MIX_HOIST = [
    ('src/mame/sharp/x68k_v.cpp',
     '\tbool blend = (((m_video.reg[2] & 0x1900) == 0x1900) && (m_video.gfx_pri != 2));\n\t// Kairo68: colour 0 of the text layer shows at priority level 2 (get_text_pixel).\n\tconst bool text_zero_shows = ((m_video.reg[1] & 0x0c00) == 0x0800);\n\tfor(scanline=rect.min_y;scanline<=rect.max_y;scanline++)\n\t{\n\t\tif(textprio != -1)\n\t\t\tdecode_text_line(scanline);\n\t\tfor(pixel=m_crtc->hbegin();pixel<=m_crtc->hend();pixel++)\n\t\t{\n\t\t\tif((m_video.reg[2] & 0x1800) == 0x1000) // special priority\n\t\t\t{\n\t\t\t\tuint16_t colour = m_special.pix(scanline / divisor, pixel) & 0xff;\n\t\t\t\t// XXX: this might check the pen color not the palette index\n\t\t\t\tif(colour & ~1)\n\t\t\t\t{\n\t\t\t\t\tbitmap.pix(scanline, pixel) = m_gfxpalette->pen(colour & ~1);\n\t\t\t\t\tcontinue;\n\t\t\t\t}\n\t\t\t}\n\t\t\trgb_t outpix = bitmap.pix(scanline, pixel), pix = 0;\n\t\t\tbool pcgpix = false, gfxpix = false;\n\t\t\tfor(priority = 0; priority < 3; priority++)\n\t\t\t{\n\t\t\t\tif(blend)\n\t\t\t\t{\n\t\t\t\t\tif((priority == textprio) && !pcgpix)\n\t\t\t\t\t{\n\t\t\t\t\t\tconst uint16_t colour = m_text_line[pixel];\n\t\t\t\t\t\tif(colour || text_zero_shows)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tpix = m_pcgpalette->pen(colour);\n\t\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t\t}\n\t\t\t\t\t}\n\t\t\t\t\telse if((priority == pcgprio) && !pcgpix)\n\t\t\t\t\t{\n\t\t\t\t\t\tuint16_t colour = m_pcgbitmap.pix(scanline / divisor, pixel) & 0xff;\n\t\t\t\t\t\tif(colour)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tpix = m_pcgpalette->pen(colour);\n\t\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t\t}\n\t\t\t\t\t\telse if(outpix && (pcgprio > textprio) && (textprio != -1))\n\t\t\t\t\t\t\tpix = outpix;\n\t\t\t\t\t}\n\t\t\t\t\telse if(priority == gfxprio)\n\t\t\t\t\t\tgfxpix = true;\n\t\t\t\t\tif((pix & 0xffffff) || ((priority == 2) && gfxpix))\n\t\t\t\t\t{\n\t\t\t\t\t\tif(!(pix & 0xffffff))\n\t\t\t\t\t\t\tpix = outpix;\n\t\t\t\t\t\trgb_t blendpix = get_gfx_pixel<true>(scanline, pixel, gfx16bcol, pix);\n\t\t\t\t\t\toutpix = blendpix & 0xffffff ? blendpix : pix;\n\t\t\t\t\t\tbreak;\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t\telse\n\t\t\t\t{\n\t\t\t\t\tif((priority == textprio) && !pcgpix)\n\t\t\t\t\t{\n\t\t\t\t\t\tconst uint16_t colour = m_text_line[pixel];\n\t\t\t\t\t\tif(colour || text_zero_shows)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tpix = m_pcgpalette->pen(colour);\n\t\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t\t}\n\t\t\t\t\t}\n\t\t\t\t\telse if((priority == pcgprio) && !pcgpix)\n\t\t\t\t\t{\n\t\t\t\t\t\tuint16_t colour = m_pcgbitmap.pix(scanline / divisor, pixel) & 0xff;\n\t\t\t\t\t\tif(colour)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tpix = m_pcgpalette->pen(colour);\n\t\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t\t}\n\t\t\t\t\t\telse if(outpix && (pcgprio > textprio) && (textprio != -1))\n\t\t\t\t\t\t\tpix = outpix;\n\t\t\t\t\t}\n\t\t\t\t\telse if(priority == gfxprio)\n\t\t\t\t\t\tpix = get_gfx_pixel<false>(scanline, pixel, gfx16bcol, 0);\n\t\t\t\t\tif(pix & 0xffffff)\n\t\t\t\t\t{\n\t\t\t\t\t\toutpix = pix;\n\t\t\t\t\t\tbreak;\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t\tbitmap.pix(scanline, pixel) = outpix;\n\t\t}\n\t}\n',
     '\tbool blend = (((m_video.reg[2] & 0x1900) == 0x1900) && (m_video.gfx_pri != 2));\n\t// Kairo68: colour 0 of the text layer shows at priority level 2 (get_text_pixel).\n\tconst bool text_zero_shows = ((m_video.reg[1] & 0x0c00) == 0x0800);\n\t// Kairo68: what the mix reads for every pixel, taken once: nothing it reads changes while it runs.\n\tconst bool special = ((m_video.reg[2] & 0x1800) == 0x1000);\n\tconst pen_t *const pcgpens = m_pcgpalette->pens();\n\tconst pen_t *const gfxpens = m_gfxpalette->pens();\n\t// get_gfx_pixel, decided once: 65,536 colours, 16-bit colours (gfx16bcol), or the palette.\n\tconst int gfxmode = ((m_video.reg[0] & 0x03) == 3) ? 2 : (gfx16bcol ? 1 : 0);\n\tconst bool gfxzero = (m_video.gfx_pri == 2);\n\tconst int gfxdivisor = m_crtc->gfx_double_scan() ? 2 : 1;\n\t// get_gfx_pixel<false>.\n\tauto gfx_plain = [&](uint16_t raw) -> rgb_t\n\t{\n\t\tif(gfxmode == 2)\n\t\t\treturn (raw || gfxzero) ? GGGGGRRRRRBBBBBI(raw) : rgb_t(0);\n\t\tif(gfxmode == 1)\n\t\t\treturn (raw || gfxzero) ? pal555(raw, 6, 11, 1) : rgb_t(0);\n\t\tconst pen_t pen = gfxpens[raw & 0xff];\n\t\treturn ((pen & 0xffffff) || gfxzero) ? rgb_t(pen) : rgb_t(0);\n\t};\n\t// get_gfx_pixel<true>.\n\tauto gfx_blend = [&](uint16_t raw, rgb_t blendpix) -> rgb_t\n\t{\n\t\tif(gfxmode == 2)\n\t\t\treturn (raw || gfxzero) ? GGGGGRRRRRBBBBBI(raw) : rgb_t(0);\n\t\tif(gfxmode == 1)\n\t\t{\n\t\t\tif(!(raw || gfxzero))\n\t\t\t\treturn rgb_t(0);\n\t\t\tif(raw & 1)\n\t\t\t\treturn ((blendpix >> 1) & 0xff7f7f7f) + ((pal555(raw, 6, 11, 1) >> 2) & 0x3f3f3f);\n\t\t\treturn pal555(raw, 6, 11, 1);\n\t\t}\n\t\tuint16_t colour = raw & 0xff;\n\t\tconst bool half = colour & 1;\n\t\tif(half)\n\t\t\tcolour &= 0xfe;\n\t\tconst pen_t pen = gfxpens[colour];\n\t\tif(!((pen & 0xffffff) || gfxzero))\n\t\t\treturn rgb_t(0);\n\t\tif(half)\n\t\t\treturn ((blendpix >> 1) & 0xff7f7f7f) + ((pen >> 2) & 0x3f3f3f);\n\t\treturn rgb_t(pen);\n\t};\n\tfor(scanline=rect.min_y;scanline<=rect.max_y;scanline++)\n\t{\n\t\tif(textprio != -1)\n\t\t\tdecode_text_line(scanline);\n\t\tuint32_t *const dest = &bitmap.pix(scanline, 0);\n\t\tconst uint16_t *const pcgrow = &m_pcgbitmap.pix(scanline / divisor, 0);\n\t\tconst uint16_t *const specialrow = &m_special.pix(scanline / divisor, 0);\n\t\tconst uint16_t *const gfxrow = &m_gfxbitmap.pix(scanline / gfxdivisor, 0);\n\t\tfor(pixel=m_crtc->hbegin();pixel<=m_crtc->hend();pixel++)\n\t\t{\n\t\t\tif(special) // special priority\n\t\t\t{\n\t\t\t\tuint16_t colour = specialrow[pixel] & 0xff;\n\t\t\t\t// XXX: this might check the pen color not the palette index\n\t\t\t\tif(colour & ~1)\n\t\t\t\t{\n\t\t\t\t\tdest[pixel] = gfxpens[colour & ~1];\n\t\t\t\t\tcontinue;\n\t\t\t\t}\n\t\t\t}\n\t\t\trgb_t outpix = dest[pixel], pix = 0;\n\t\t\tbool pcgpix = false, gfxpix = false;\n\t\t\tfor(priority = 0; priority < 3; priority++)\n\t\t\t{\n\t\t\t\tif(blend)\n\t\t\t\t{\n\t\t\t\t\tif((priority == textprio) && !pcgpix)\n\t\t\t\t\t{\n\t\t\t\t\t\tconst uint16_t colour = m_text_line[pixel];\n\t\t\t\t\t\tif(colour || text_zero_shows)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tpix = pcgpens[colour];\n\t\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t\t}\n\t\t\t\t\t}\n\t\t\t\t\telse if((priority == pcgprio) && !pcgpix)\n\t\t\t\t\t{\n\t\t\t\t\t\tuint16_t colour = pcgrow[pixel] & 0xff;\n\t\t\t\t\t\tif(colour)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tpix = pcgpens[colour];\n\t\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t\t}\n\t\t\t\t\t\telse if(outpix && (pcgprio > textprio) && (textprio != -1))\n\t\t\t\t\t\t\tpix = outpix;\n\t\t\t\t\t}\n\t\t\t\t\telse if(priority == gfxprio)\n\t\t\t\t\t\tgfxpix = true;\n\t\t\t\t\tif((pix & 0xffffff) || ((priority == 2) && gfxpix))\n\t\t\t\t\t{\n\t\t\t\t\t\tif(!(pix & 0xffffff))\n\t\t\t\t\t\t\tpix = outpix;\n\t\t\t\t\t\trgb_t blendpix = gfx_blend(gfxrow[pixel], pix);\n\t\t\t\t\t\toutpix = blendpix & 0xffffff ? blendpix : pix;\n\t\t\t\t\t\tbreak;\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t\telse\n\t\t\t\t{\n\t\t\t\t\tif((priority == textprio) && !pcgpix)\n\t\t\t\t\t{\n\t\t\t\t\t\tconst uint16_t colour = m_text_line[pixel];\n\t\t\t\t\t\tif(colour || text_zero_shows)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tpix = pcgpens[colour];\n\t\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t\t}\n\t\t\t\t\t}\n\t\t\t\t\telse if((priority == pcgprio) && !pcgpix)\n\t\t\t\t\t{\n\t\t\t\t\t\tuint16_t colour = pcgrow[pixel] & 0xff;\n\t\t\t\t\t\tif(colour)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tpix = pcgpens[colour];\n\t\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t\t}\n\t\t\t\t\t\telse if(outpix && (pcgprio > textprio) && (textprio != -1))\n\t\t\t\t\t\t\tpix = outpix;\n\t\t\t\t\t}\n\t\t\t\t\telse if(priority == gfxprio)\n\t\t\t\t\t\tpix = gfx_plain(gfxrow[pixel]);\n\t\t\t\t\tif(pix & 0xffffff)\n\t\t\t\t\t{\n\t\t\t\t\t\toutpix = pix;\n\t\t\t\t\t\tbreak;\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t\tdest[pixel] = outpix;\n\t\t}\n\t}\n',),
]

# The 68000 skips the passes of a busy-wait loop (Kairo68#12): when a pass of a loop starts in the
# same state as the last one, within one execute_run, with no write or read outside main RAM in
# between, the cycles of the remaining whole passes are used up at once. Games wait for the next
# frame this way: Alien Syndrome spent 52% of its instructions in one two-instruction loop on a RAM
# byte. Every frame's picture and sound stay the same.
M68000_IDLE_SKIP = [
    ('src/devices/cpu/m68000/m68000.h',
     '\t// Dynamic specifics, depending on supervisor state\n\tmemory_access<24, 1, 0, ENDIANNESS_BIG>::specific m_program, m_opcodes;\n',
     '\t// Kairo68: the data accesses, counting those that may have a side effect for the idle-loop\n\t// skip (enable_idle_skip): every write, and every read at or above free_end.\n\tstruct counted_program {\n\t\tmemory_access<24, 1, 0, ENDIANNESS_BIG>::specific inner;\n\t\tu32 side_effects = 0;\n\t\toffs_t free_end = 0;\n\t\tcounted_program &operator=(const memory_access<24, 1, 0, ENDIANNESS_BIG>::specific &s) { inner = s; return *this; }\n\t\tu16 read_interruptible(offs_t address, u16 mask = 0xffff) {\n\t\t\tif(address >= free_end)\n\t\t\t\tside_effects++;\n\t\t\treturn inner.read_interruptible(address, mask);\n\t\t}\n\t\tvoid write_interruptible(offs_t address, u16 data, u16 mask = 0xffff) {\n\t\t\tside_effects++;\n\t\t\tinner.write_interruptible(address, data, mask);\n\t\t}\n\t};\n\n\t// Dynamic specifics, depending on supervisor state\n\tcounted_program m_program;\n\tmemory_access<24, 1, 0, ENDIANNESS_BIG>::specific m_opcodes;\n',),
    ('src/devices/cpu/m68000/m68000.h',
     '\tvoid enable_mmu(bool disable_spaces = true);\n\tvoid set_current_mmu(mmu *m);\n',
     '\tvoid enable_mmu(bool disable_spaces = true);\n\tvoid set_current_mmu(mmu *m);\n\n\t// Kairo68: skip the passes of a loop that only reads memory below free_end, which nothing but\n\t// this CPU changes while it runs (Kairo68#12). The guest sees no difference.\n\tvoid enable_idle_skip(offs_t free_end);\n',),
    ('src/devices/cpu/m68000/m68000.h',
     '\tu32 m_post_run;\n\tint m_post_run_cycles;\n',
     "\tu32 m_post_run;\n\tint m_post_run_cycles;\n\n\t// Kairo68: the idle-loop skip (enable_idle_skip, idle_check).\n\tstatic constexpr int IDLE_STATE_WORDS = 56;\n\tbool m_idle_enabled = false;\n\tu32 m_idle_run = 0;            // execute_run's calls: a pass only repeats within one\n\tu32 m_idle_last_pc = 0;        // the previous instruction's address\n\tu32 m_idle_pc = 0xffffffff;    // the last pass's start, and its execute_run call\n\tu32 m_idle_pass_run = 0;\n\tu32 m_idle_side_effects = 0;   // m_program.side_effects at its start\n\tint m_idle_icount = 0;         // and the cycles left\n\tbool m_idle_have_state = false; // m_idle_state holds the processor state at its start\n\tu32 m_idle_state[IDLE_STATE_WORDS];\n\tvoid idle_capture(u32 *state) const;\n\tvoid idle_check();\n",),
    ('src/devices/cpu/m68000/m68000.cpp',
     'void m68000_device::execute_run()\n{\n\tm_icount -= m_count_before_instruction_step;\n',
     'void m68000_device::execute_run()\n{\n\tm_idle_run++;\n\tm_icount -= m_count_before_instruction_step;\n',),
    ('src/devices/cpu/m68000/m68000.cpp',
     '\t\t\t\tKAIRO68_PERF_PC(m_ipc);\n',
     '\t\t\t\tKAIRO68_PERF_PC(m_ipc);\n\t\t\t\tif(m_idle_enabled) {\n\t\t\t\t\tif(m_ipc < m_idle_last_pc)\n\t\t\t\t\t\tidle_check();\n\t\t\t\t\tm_idle_last_pc = m_ipc;\n\t\t\t\t}\n',),
    ('src/devices/cpu/m68000/m68000.cpp',
     'device_memory_interface::space_config_vector m68000_device::memory_logical_space_config() const\n',
     'void m68000_device::enable_idle_skip(offs_t free_end)\n{\n\t// Host callbacks and an MMU act outside m_program, where no side effect would be counted.\n\tif(m_mmu || !m_tas_write_callback.isnull() || !m_cmpild_instr_callback.isnull() || !m_rte_instr_callback.isnull())\n\t\tthrow emu_fatalerror("%s: the idle-loop skip cannot see this CPU\'s callbacks or MMU", tag());\n\tm_program.free_end = free_end;\n\tm_idle_enabled = true;\n}\n\n// Everything a pass of a loop starts with: the registers, the prefetch and bus latches, the\n// pending interrupt and the instruction\'s state.\nvoid m68000_device::idle_capture(u32 *state) const\n{\n\tconst u32 words[] = {\n\t\tm_ipc, m_pc, m_au, m_at, m_aob, m_dt, m_int_vector, m_sp,\n\t\tu32(m_bcount), u32(m_count_before_instruction_step), u32(m_t), m_movems,\n\t\tm_isr, m_sr, m_new_sr, m_dbin, m_dbout, m_edb, m_irc, m_ir, m_ird, m_ftu, m_aluo, m_alue, m_alub,\n\t\tm_movemr, m_irdi, m_base_ssw, m_ssw, m_dcr,\n\t\tm_virq_state, m_nmi_pending, m_int_level, m_int_next_state, m_next_state, m_post_run,\n\t\tu32(m_post_run_cycles), m_inst_state, m_inst_substate };\n\tstatic_assert(std::size(words) + std::size(decltype(m_da){}) == IDLE_STATE_WORDS);\n\tstd::copy(std::begin(m_da), std::end(m_da), state);\n\tstd::copy(std::begin(words), std::end(words), state + std::size(m_da));\n}\n\n// Kairo68: the idle-loop skip (Kairo68#12). Called when an instruction starts below the previous\n// one, as every pass of a loop does. Nothing outside this CPU runs during execute_run, so when a\n// pass starts at the same address as the last one in the same call, with no access since that\n// may have a side effect (m_program) and the whole processor state the same, every following pass\n// is the same as well. The cycles of the whole passes that fit in the slice are then used up at\n// once and the rest runs normally, so at the end of the slice the CPU is at the same instruction,\n// with the same cycles, as if it had run them.\nvoid m68000_device::idle_check()\n{\n\tif(m_ipc != m_idle_pc || m_idle_pass_run != m_idle_run || m_program.side_effects != m_idle_side_effects) {\n\t\tm_idle_pc = m_ipc;\n\t\tm_idle_pass_run = m_idle_run;\n\t\tm_idle_side_effects = m_program.side_effects;\n\t\tm_idle_icount = m_icount;\n\t\tm_idle_have_state = false;\n\t\treturn;\n\t}\n\tu32 state[IDLE_STATE_WORDS];\n\tidle_capture(state);\n\tif(!m_idle_have_state || memcmp(state, m_idle_state, sizeof(state)) || debugger_enabled()) {\n\t\tmemcpy(m_idle_state, state, sizeof(state));\n\t\tm_idle_have_state = true;\n\t\tm_idle_icount = m_icount;\n\t\treturn;\n\t}\n\tconst int period = m_idle_icount - m_icount;\n\tif(period > 0 && m_icount > period) {\n\t\tconst int passes = (m_icount - 1) / period;\n\t\tm_icount -= passes * period;\n\t\tKAIRO68_PERF_COUNT(idle_skips, 1);\n\t\tKAIRO68_PERF_COUNT(idle_cycles, u64(passes) * u64(period));\n\t}\n\tm_idle_icount = m_icount;\n}\n\ndevice_memory_interface::space_config_vector m68000_device::memory_logical_space_config() const\n',),
]

# Measurement: host time in ymfm's synthesis, and the part of it done inside the sound manager's
# update at the end of a frame, which a synthesis worker could not overlap with emulation
# (Kairo68#22). Empty outside a KAIRO68_PERF build.
PERF_FM_STAGE = [
    ('src/devices/sound/ymfm_mame.h',
     '#include "ymfm/src/ymfm.h"\n#include "ymfm/src/ymfm_ssg.h"\n',
     '#include "ymfm/src/ymfm.h"\n#include "ymfm/src/ymfm_ssg.h"\n#include "kairo68_perf.h"\n',),
    ('src/devices/sound/ymfm_mame.h',
     '\tvoid update_internal(sound_stream &stream, int output_shift = 0)\n\t{\n',
     '\tvoid update_internal(sound_stream &stream, int output_shift = 0)\n\t{\n\t\tKAIRO68_PERF_FM_SCOPE();\n',),
    ('src/emu/sound.cpp',
     '\tKAIRO68_PERF_TIME(sound_ns);\n\n\tmapping_update();\n',
     '\tKAIRO68_PERF_TIME(sound_ns);\n\tKAIRO68_PERF_SOUND_UPDATE_SCOPE();\n\n\tmapping_update();\n',),
]

# ymfm synthesis on a worker thread, in parallel with the emulation (Kairo68#22). The guest reads
# the chip's status, timers, IRQ and ports, which stay on the emulation thread with the device's
# chip; that chip stops generating and counts the clocks it would have (timer B's start depends on
# them). A second chip on the worker (kairo/ordered_worker.h) is given the same calls in the same
# order and generates each update's samples; a sound stream can leave its samples to be written
# later, and every stream reading from it has them written first. Off unless a host calls
# enable_synthesis_worker. Every frame's picture and sound stay the same.
YMFM_SYNTHESIS_WORKER = [
    ('src/emu/sound.h',
     '\ts32 samples() const { return m_samples_to_update; }\n',
     '\ts32 samples() const { return m_samples_to_update; }\n\n\t// Kairo68: a stream whose callback leaves its samples to be written later sets the function\n\t// that writes them (put_at); every stream reading from it calls it first (Kairo68#22).\n\tvoid set_deferred_finish(std::function<void()> finish) { m_deferred_finish = std::move(finish); }\n\tvoid finish_deferred() { if(m_deferred_finish) m_deferred_finish(); }\n\t// Kairo68: writes a sample of an earlier update, by its absolute index, before it is read.\n\tvoid put_at(s32 output, u64 index, sample_t sample) { *m_output_buffer.ptrw(output, s32(s64(index) - s64(m_output_buffer.write_sample()))) = sample; }\n',),
    ('src/emu/sound.h',
     '\tstd::vector<sound_stream *> m_dependant_streams;\n',
     '\tstd::vector<sound_stream *> m_dependant_streams;\n\tstd::function<void()> m_deferred_finish; // Kairo68: set_deferred_finish\n',),
    ('src/emu/sound.cpp',
     'void sound_stream::do_update()\n{\n\t// Mix in all the inputs (if any)\n\tif(m_input_count) {\n',
     'void sound_stream::do_update()\n{\n\t// Kairo68: samples a source left to be written later are written before they are read here\n\tfor(const auto &r : m_bw_routes)\n\t\tr.m_source->finish_deferred();\n\n\t// Mix in all the inputs (if any)\n\tif(m_input_count) {\n',),
    ('3rdparty/ymfm/src/ymfm_fm.h',
     '\tuint32_t clock_prescale() const { return m_clock_prescale; }\n',
     '\tuint32_t clock_prescale() const { return m_clock_prescale; }\n\n\t// Kairo68: count clocks generated elsewhere (clock() counts them in m_total_clocks, which\n\t// engine_mode_write uses to start timer B), for a chip whose synthesis runs on another copy.\n\tvoid advance_total_clocks(uint32_t clocks) { m_total_clocks += clocks; }\n',),
    ('3rdparty/ymfm/src/ymfm_opm.h',
     '\t// generate one sample of sound\n\tvoid generate(output_data *output, uint32_t numsamples = 1);\n',
     '\t// generate one sample of sound\n\tvoid generate(output_data *output, uint32_t numsamples = 1);\n\n\t// Kairo68: the clocks generate() would count for numsamples, without generating.\n\tvoid advance_generated(uint32_t numsamples) { m_fm.advance_total_clocks(numsamples); }\n',),
    ('src/devices/sound/ymfm_mame.h',
     '#include "ymfm/src/ymfm.h"\n#include "ymfm/src/ymfm_ssg.h"\n#include "kairo68_perf.h"\n',
     '#include "ymfm/src/ymfm.h"\n#include "ymfm/src/ymfm_ssg.h"\n#include "kairo68_perf.h"\n#include "kairo/ordered_worker.h"\n\n#include <memory>\n',),
    ('src/devices/sound/ymfm_mame.h',
     '\t\tif (scheduler.currently_executing())\n\t\t\tscheduler.synchronize(timer_expired_delegate(FUNC(ym_generic_device::fm_check_interrupts), this));\n\t\telse\n\t\t\tm_engine->engine_check_interrupts();\n\t}\n',
     "\t\tif (scheduler.currently_executing())\n\t\t\tscheduler.synchronize(timer_expired_delegate(FUNC(ym_generic_device::fm_check_interrupts), this));\n\t\telse\n\t\t{\n\t\t\tm_engine->engine_check_interrupts();\n\t\t\tm_checked_now = true; // Kairo68: the synthesis worker's chip checks now too\n\t\t}\n\t}\n",),
    ('src/devices/sound/ymfm_mame.h',
     '\t// timer callbacks\n\tvoid fm_mode_write(s32 param) { m_engine->engine_mode_write(param); }\n\tvoid fm_check_interrupts(s32 param) { m_engine->engine_check_interrupts(); }\n\tvoid fm_timer_handler(s32 param) { m_engine->engine_timer_expired(param); }\n',
     '\t// Kairo68: what the chip was given, for a copy of it synthesizing on a worker thread\n\t// (ymfm_device_base::enable_synthesis_worker): the same calls, in the same order.\n\tenum : u8 { SYNTH_GENERATE, SYNTH_WRITE, SYNTH_ADDRESS, SYNTH_DATA, SYNTH_MODE, SYNTH_CHECK, SYNTH_TIMER, SYNTH_RESET };\n\tvirtual void synthesis_event(u8 op, u8 data, offs_t offset = 0) { }\n\tbool m_checked_now = false; // a call to the chip checked its interrupts at once\n\n\t// timer callbacks\n\tvoid fm_mode_write(s32 param) { m_checked_now = false; m_engine->engine_mode_write(param); synthesis_event(SYNTH_MODE, param); }\n\tvoid fm_check_interrupts(s32 param) { m_checked_now = false; m_engine->engine_check_interrupts(); synthesis_event(SYNTH_CHECK, 0); }\n\tvoid fm_timer_handler(s32 param) { m_checked_now = false; m_engine->engine_timer_expired(param); synthesis_event(SYNTH_TIMER, param); }\n',),
    ('src/devices/sound/ymfm_mame.h',
     '\t// read access: update the streams before performing the read\n\tvirtual u8 read(offs_t offset) override { return update_streams().read(offset); }\n\tvirtual u8 status_r() override { return update_streams().read_status(); }\n\n\t// write access: update the strams before performing the write\n\tvirtual void write(offs_t offset, u8 data) override { update_streams().write(offset, data); }\n\tvirtual void address_w(u8 data) override { update_streams().write_address(data); }\n\tvirtual void data_w(u8 data) override { update_streams().write_data(data); }\n',
     '\t// read access: update the streams before performing the read\n\tvirtual u8 read(offs_t offset) override { return update_streams().read(offset); }\n\tvirtual u8 status_r() override { return update_streams().read_status(); }\n\n\t// write access: update the strams before performing the write\n\t// Kairo68: and hand each write to the synthesis worker\'s chip (synthesis_event)\n\tvirtual void write(offs_t offset, u8 data) override { m_checked_now = false; update_streams().write(offset, data); synthesis_event(SYNTH_WRITE, data, offset); }\n\tvirtual void address_w(u8 data) override { m_checked_now = false; update_streams().write_address(data); synthesis_event(SYNTH_ADDRESS, data); }\n\tvirtual void data_w(u8 data) override { m_checked_now = false; update_streams().write_data(data); synthesis_event(SYNTH_DATA, data); }\n\n\t// Kairo68: synthesize on a worker thread, in parallel with the emulation (Kairo68#22). The\n\t// guest reads the chip\'s status, timers, IRQ and ports, which stay with m_chip on the emulation\n\t// thread; m_chip no longer generates, and only counts the clocks it would have. A second chip\n\t// on the worker is given the same calls in the same order, generates every sample at the\n\t// position the stream asked for it, and the samples are written into the stream before\n\t// anything reads it. Output is identical to synthesizing here.\n\tvoid enable_synthesis_worker()\n\t{\n\t\tstatic_assert(requires(ChipClass &chip) { chip.advance_generated(1u); }, "the chip cannot count clocks it does not generate");\n\t\tif (m_synth_worker)\n\t\t\treturn;\n\t\tm_synth_intf = std::make_unique<synthesis_interface>();\n\t\tm_synth_chip = std::make_unique<ChipClass>(*m_synth_intf);\n\t\t// The worker\'s chip starts as m_chip is now.\n\t\tstd::vector<uint8_t> blob;\n\t\tymfm::ymfm_saved_state save(blob, true);\n\t\tm_chip.save_restore(save);\n\t\tymfm::ymfm_saved_state load(blob, false);\n\t\tm_synth_chip->save_restore(load);\n\t\tm_synth_worker = std::make_unique<kairo::OrderedWorker<synthesis_batch, synthesis_results, 16>>(\n\t\t\t\t[this](const synthesis_batch &batch) {\n\t\t\t\t\tsynthesis_results results;\n\t\t\t\t\tfor (const synthesis_command &command : batch)\n\t\t\t\t\t\tif (command.op == SYNTH_GENERATE)\n\t\t\t\t\t\t\tresults.push_back(synthesize(command));\n\t\t\t\t\t\telse\n\t\t\t\t\t\t\tsynthesize(command);\n\t\t\t\t\treturn results;\n\t\t\t\t}, "kairo-synth");\n\t\tm_stream->set_deferred_finish([this] { finish_synthesis(); });\n\t}\n',),
    ('src/devices/sound/ymfm_mame.h',
     '\t// device reset\n\tvirtual void device_reset() override\n\t{\n\t\tm_chip.reset();\n\t}\n\n\t// handle clock changed\n\tvirtual void device_clock_changed() override\n\t{\n\t\tif (m_stream != nullptr)\n\t\t\tm_stream->set_sample_rate(m_chip.sample_rate(device_t::clock()));\n\t}\n',
     "\t// device reset\n\tvirtual void device_reset() override\n\t{\n\t\treset_chip();\n\t}\n\n\t// Kairo68: resets the chip, and the synthesis worker's chip at the same point\n\tvoid reset_chip()\n\t{\n\t\tm_checked_now = false;\n\t\tm_chip.reset();\n\t\tsynthesis_event(SYNTH_RESET, 0);\n\t}\n\n\t// handle clock changed\n\tvirtual void device_clock_changed() override\n\t{\n\t\tif (m_stream != nullptr)\n\t\t{\n\t\t\tfinish_synthesis(); // Kairo68: the samples are in the stream before it resamples them\n\t\t\tm_stream->set_sample_rate(m_chip.sample_rate(device_t::clock()));\n\t\t}\n\t}\n",),
    ('src/devices/sound/ymfm_mame.h',
     "\t\t// save the state\n\t\tymfm::ymfm_saved_state state(m_save_blob, true);\n\t\tm_chip.save_restore(state);\n\n\t\t// ensure that the size didn't change since we first allocated\n",
     "\t\t// save the state\n\t\t// Kairo68: with a synthesis worker, its chip holds the whole state once it has caught up\n\t\tfinish_synthesis();\n\t\tymfm::ymfm_saved_state state(m_save_blob, true);\n\t\t(m_synth_chip ? *m_synth_chip : m_chip).save_restore(state);\n\n\t\t// ensure that the size didn't change since we first allocated\n",),
    ('src/devices/sound/ymfm_mame.h',
     '\t\t// populate the state from the blob\n\t\tymfm::ymfm_saved_state state(m_save_blob, false);\n\t\tm_chip.save_restore(state);\n\t}\n',
     "\t\t// populate the state from the blob\n\t\tymfm::ymfm_saved_state state(m_save_blob, false);\n\t\tm_chip.save_restore(state);\n\n\t\t// Kairo68: the worker's pending samples belong to the time before the load\n\t\tif (m_synth_worker)\n\t\t{\n\t\t\tm_synth_batch.clear();\n\t\t\tm_synth_worker->drain([](synthesis_results &&) { });\n\t\t\tymfm::ymfm_saved_state worker_state(m_save_blob, false);\n\t\t\tm_synth_chip->save_restore(worker_state);\n\t\t}\n\t}\n",),
    ('src/devices/sound/ymfm_mame.h',
     '\tvoid update_internal(sound_stream &stream, int output_shift = 0)\n\t{\n\t\tKAIRO68_PERF_FM_SCOPE();\n',
     '\tvoid update_internal(sound_stream &stream, int output_shift = 0)\n\t{\n\t\tKAIRO68_PERF_FM_SCOPE();\n\t\t// Kairo68: the synthesis worker generates these samples; finish_synthesis writes them\n\t\tif (m_synth_worker)\n\t\t{\n\t\t\tsynthesis_command command;\n\t\t\tcommand.op = SYNTH_GENERATE;\n\t\t\tcommand.shift = output_shift;\n\t\t\tcommand.start = stream.start_index();\n\t\t\tcommand.count = stream.samples();\n\t\t\tcommand.outputs = std::min(stream.output_count(), u32(OUTPUTS));\n\t\t\tif constexpr (requires(ChipClass &chip) { chip.advance_generated(1u); })\n\t\t\t\tm_chip.advance_generated(command.count);\n\t\t\tsubmit_synthesis(command);\n\t\t\treturn;\n\t\t}\n',),
    ('src/devices/sound/ymfm_mame.h',
     '\t// internal state\n\tsound_stream *m_stream;           // sound stream\n\tChipClass m_chip;                 // core chip implementation\n\tstd::vector<uint8_t> m_save_blob; // state saving buffer\n};\n',
     "\t// Kairo68: the synthesis worker (enable_synthesis_worker)\n\tstruct synthesis_command\n\t{\n\t\tu8 op = SYNTH_GENERATE;\n\t\tu8 data = 0;\n\t\tu32 offset = 0;\n\t\tbool checked_now = false; // the call checked m_chip's interrupts at once\n\t\tu8 shift = 0;\n\t\tu32 count = 0;\n\t\tu32 outputs = 0;\n\t\tu64 start = 0;\n\t};\n\tstruct synthesis_result\n\t{\n\t\tu64 start = 0;\n\t\tu32 count = 0;\n\t\tu32 outputs = 0;\n\t\tu8 shift = 0;\n\t\tstd::vector<s32> samples; // count samples of each output, output by output\n\t};\n\tusing synthesis_batch = std::vector<synthesis_command>;\n\tusing synthesis_results = std::vector<synthesis_result>;\n\tstatic constexpr size_t SYNTH_BATCH = 32;\n\n\t// The worker chip's interface: timers, IRQ, busy and ports belong to m_chip. Mode writes and\n\t// interrupt checks that m_chip's interface deferred arrive as their own events; one it made at\n\t// once happens here at once too.\n\tclass synthesis_interface : public ymfm::ymfm_interface\n\t{\n\tpublic:\n\t\tbool checked_now = false;\n\t\tvirtual void ymfm_sync_mode_write(uint8_t data) override { }\n\t\tvirtual void ymfm_sync_check_interrupts() override { if (checked_now) m_engine->engine_check_interrupts(); }\n\t\tymfm::ymfm_engine_callbacks &engine() { return *m_engine; }\n\t};\n\n\tvirtual void synthesis_event(u8 op, u8 data, offs_t offset = 0) override\n\t{\n\t\tif (!m_synth_worker)\n\t\t\treturn;\n\t\tsynthesis_command command;\n\t\tcommand.op = op;\n\t\tcommand.data = data;\n\t\tcommand.offset = offset;\n\t\tcommand.checked_now = m_checked_now;\n\t\tsubmit_synthesis(command);\n\t}\n\n\t// Commands go to the worker in batches: one hand-over per command cost as much as the synthesis\n\t// it saved when a game polls the chip between every write.\n\tvoid submit_synthesis(const synthesis_command &command)\n\t{\n\t\tm_synth_batch.push_back(command);\n\t\tif (m_synth_batch.size() >= SYNTH_BATCH)\n\t\t\tsend_synthesis_batch();\n\t}\n\n\tvoid send_synthesis_batch()\n\t{\n\t\tif (m_synth_batch.empty())\n\t\t\treturn;\n\t\tm_synth_worker->submit(std::move(m_synth_batch), [this](synthesis_results &&results) { write_synthesis(results); });\n\t\tm_synth_batch = synthesis_batch();\n\t\tm_synth_batch.reserve(SYNTH_BATCH);\n\t}\n\n\t// Writes every sample the worker has been asked for into the stream.\n\tvoid finish_synthesis()\n\t{\n\t\tif (!m_synth_worker)\n\t\t\treturn;\n\t\tsend_synthesis_batch();\n\t\tm_synth_worker->drain([this](synthesis_results &&results) { write_synthesis(results); });\n\t}\n\n\tvoid write_synthesis(const synthesis_results &results)\n\t{\n\t\tfor (const synthesis_result &result : results)\n\t\t\tfor (u32 outnum = 0; outnum < result.outputs; outnum++)\n\t\t\t{\n\t\t\t\tint const eff_outnum = (outnum + result.shift) % OUTPUTS;\n\t\t\t\tconst s32 *samples = &result.samples[size_t(outnum) * result.count];\n\t\t\t\tfor (u32 index = 0; index < result.count; index++)\n\t\t\t\t\tm_stream->put_at(eff_outnum, result.start + index, double(samples[index]) / 32768);\n\t\t\t}\n\t}\n\n\t// On the worker thread: the same call m_chip was given, or the samples it would have generated.\n\tsynthesis_result synthesize(const synthesis_command &command)\n\t{\n\t\tsynthesis_result result;\n\t\tm_synth_intf->checked_now = command.checked_now;\n\t\tswitch (command.op)\n\t\t{\n\t\tcase SYNTH_GENERATE:\n\t\t{\n\t\t\tconstexpr u32 MAX_SAMPLES = 256;\n\t\t\ttypename ChipClass::output_data output[MAX_SAMPLES];\n\t\t\tresult.start = command.start;\n\t\t\tresult.count = command.count;\n\t\t\tresult.outputs = command.outputs;\n\t\t\tresult.shift = command.shift;\n\t\t\tresult.samples.resize(size_t(command.outputs) * command.count);\n\t\t\tfor (u32 sampindex = 0; sampindex < command.count; sampindex += MAX_SAMPLES)\n\t\t\t{\n\t\t\t\tu32 const cursamples = std::min(command.count - sampindex, MAX_SAMPLES);\n\t\t\t\tm_synth_chip->generate(output, cursamples);\n\t\t\t\tfor (u32 outnum = 0; outnum < command.outputs; outnum++)\n\t\t\t\t\tfor (u32 index = 0; index < cursamples; index++)\n\t\t\t\t\t\tresult.samples[size_t(outnum) * command.count + sampindex + index] = output[index].data[outnum];\n\t\t\t}\n\t\t\tbreak;\n\t\t}\n\t\tcase SYNTH_WRITE: m_synth_chip->write(command.offset, command.data); break;\n\t\tcase SYNTH_ADDRESS: m_synth_chip->write_address(command.data); break;\n\t\tcase SYNTH_DATA: m_synth_chip->write_data(command.data); break;\n\t\tcase SYNTH_MODE: m_synth_intf->engine().engine_mode_write(command.data); break;\n\t\tcase SYNTH_CHECK: m_synth_intf->engine().engine_check_interrupts(); break;\n\t\tcase SYNTH_TIMER: m_synth_intf->engine().engine_timer_expired(command.data); break;\n\t\tcase SYNTH_RESET: m_synth_chip->reset(); break;\n\t\t}\n\t\treturn result;\n\t}\n\n\t// internal state\n\tsound_stream *m_stream;           // sound stream\n\tChipClass m_chip;                 // core chip implementation\n\tstd::vector<uint8_t> m_save_blob; // state saving buffer\n\tstd::unique_ptr<synthesis_interface> m_synth_intf; // Kairo68: the synthesis worker's chip\n\tstd::unique_ptr<ChipClass> m_synth_chip;\n\tsynthesis_batch m_synth_batch;   // commands not yet handed to the worker\n\tstd::unique_ptr<kairo::OrderedWorker<synthesis_batch, synthesis_results, 16>> m_synth_worker;\n};\n",),
    ('src/emu/sound.cpp',
     'void sound_stream::sync(attotime now)\n{\n\tm_sync_time = now;\n\tm_output_buffer.sync();\n}\n',
     'void sound_stream::sync(attotime now)\n{\n\tm_sync_time = now;\n\t// Kairo68: samples left to be written later are in the buffer before it moves on\n\tfinish_deferred();\n\tm_output_buffer.sync();\n}\n',),
    ('src/devices/sound/ymopm.cpp',
     '\t\tm_reset_state = state;\n\t\tif (state != 0)\n\t\t\tm_chip.reset();\n',
     "\t\tm_reset_state = state;\n\t\tif (state != 0)\n\t\t\treset_chip(); // Kairo68: the synthesis worker's chip too\n",),
]

# The lofi resampler reads its source samples through an inlined lambda instead of a
# std::function: the same reads and sums in the same order, without an indirect call per source
# sample on the emulation thread (Kairo68#23).
RESAMPLER_INLINE_READER = [
    ('src/emu/resampler.cpp',
     "void audio_resampler_lofi::apply(const emu::detail::output_buffer_flat<sample_t> &src, std::vector<sample_t> &dest, u64 dest_sample, u32 srcc, float gain, u32 samples) const\n{\n\tu32 seconds = dest_sample / m_ft;\n\tu32 dsamp = dest_sample % m_ft;\n\tu64 ssamp = (u64(dsamp) * m_fs * 0x1000) / m_ft;\n\tu64 ssample = (ssamp >> 12) + u64(m_fs) * seconds;\n\tu32 phase = ssamp & 0xfff;\n\tif(m_source_divide > 1) {\n\t\tu32 delta = ssample % m_source_divide;\n\t\tphase = (phase | (delta << 12)) / m_source_divide;\n\t\tssample -= delta;\n\t}\n\n\t// We're getting 2 samples latency, which is small enough\n\n\tssample -= 4*m_source_divide;\n\n\tconst sample_t *s = src.ptrs(srcc, ssample - src.sync_sample());\n\n\tstd::function<sample_t()> reader;\n\tif(m_source_divide == 1)\n\t\treader = [s]() mutable -> sample_t { return *s++; };\n\telse\n\t\treader = [s, count = m_source_divide]() mutable -> sample_t { sample_t sm = 0; for(u32 i=0; i != count; i++) { sm += *s++; } return sm / count; };\n",
     "void audio_resampler_lofi::apply(const emu::detail::output_buffer_flat<sample_t> &src, std::vector<sample_t> &dest, u64 dest_sample, u32 srcc, float gain, u32 samples) const\n{\n\tu32 seconds = dest_sample / m_ft;\n\tu32 dsamp = dest_sample % m_ft;\n\tu64 ssamp = (u64(dsamp) * m_fs * 0x1000) / m_ft;\n\tu64 ssample = (ssamp >> 12) + u64(m_fs) * seconds;\n\tu32 phase = ssamp & 0xfff;\n\tif(m_source_divide > 1) {\n\t\tu32 delta = ssample % m_source_divide;\n\t\tphase = (phase | (delta << 12)) / m_source_divide;\n\t\tssample -= delta;\n\t}\n\n\t// We're getting 2 samples latency, which is small enough\n\n\tssample -= 4*m_source_divide;\n\n\tconst sample_t *s = src.ptrs(srcc, ssample - src.sync_sample());\n\n\t// Kairo68: the same reads and sums as before, inlined; through a std::function every source\n\t// sample was an indirect call on the emulation thread (Kairo68#23).\n\tauto reader = [&s, count = m_source_divide]() -> sample_t {\n\t\tif(count == 1)\n\t\t\treturn *s++;\n\t\tsample_t sm = 0;\n\t\tfor(u32 i=0; i != count; i++) { sm += *s++; }\n\t\treturn sm / count;\n\t};\n",),
    ('src/emu/resampler.cpp',
     "void audio_resampler_lofi::apply_copy(const emu::detail::output_buffer_flat<sample_t> &src, std::vector<s16> &dest, u32 destc, int dchannels, u64 dest_sample, u32 srcc, float gain, u32 samples) const\n{\n\tu32 seconds = dest_sample / m_ft;\n\tu32 dsamp = dest_sample % m_ft;\n\tu64 ssamp = (u64(dsamp) * m_fs * 0x1000) / m_ft;\n\tu64 ssample = (ssamp >> 12) + u64(m_fs) * seconds;\n\tu32 phase = ssamp & 0xfff;\n\tif(m_source_divide > 1) {\n\t\tu32 delta = ssample % m_source_divide;\n\t\tphase = (phase | (delta << 12)) / m_source_divide;\n\t\tssample -= delta;\n\t}\n\n\tgain *= 32768;\n\n\t// We're getting 2 samples latency, which is small enough\n\n\tssample -= 4*m_source_divide;\n\n\tconst sample_t *s = src.ptrs(srcc, ssample - src.sync_sample());\n\n\tstd::function<sample_t()> reader;\n\tif(m_source_divide == 1)\n\t\treader = [s]() mutable -> sample_t { return *s++; };\n\telse\n\t\treader = [s, count = m_source_divide]() mutable -> sample_t { sample_t sm = 0; for(u32 i=0; i != count; i++) { sm += *s++; } return sm / count; };\n",
     "void audio_resampler_lofi::apply_copy(const emu::detail::output_buffer_flat<sample_t> &src, std::vector<s16> &dest, u32 destc, int dchannels, u64 dest_sample, u32 srcc, float gain, u32 samples) const\n{\n\tu32 seconds = dest_sample / m_ft;\n\tu32 dsamp = dest_sample % m_ft;\n\tu64 ssamp = (u64(dsamp) * m_fs * 0x1000) / m_ft;\n\tu64 ssample = (ssamp >> 12) + u64(m_fs) * seconds;\n\tu32 phase = ssamp & 0xfff;\n\tif(m_source_divide > 1) {\n\t\tu32 delta = ssample % m_source_divide;\n\t\tphase = (phase | (delta << 12)) / m_source_divide;\n\t\tssample -= delta;\n\t}\n\n\tgain *= 32768;\n\n\t// We're getting 2 samples latency, which is small enough\n\n\tssample -= 4*m_source_divide;\n\n\tconst sample_t *s = src.ptrs(srcc, ssample - src.sync_sample());\n\n\t// Kairo68: the same reads and sums as before, inlined; through a std::function every source\n\t// sample was an indirect call on the emulation thread (Kairo68#23).\n\tauto reader = [&s, count = m_source_divide]() -> sample_t {\n\t\tif(count == 1)\n\t\t\treturn *s++;\n\t\tsample_t sm = 0;\n\t\tfor(u32 i=0; i != count; i++) { sm += *s++; }\n\t\treturn sm / count;\n\t};\n",),
    ('src/emu/resampler.cpp',
     "void audio_resampler_lofi::apply_add(const emu::detail::output_buffer_flat<sample_t> &src, std::vector<s16> &dest, u32 destc, int dchannels, u64 dest_sample, u32 srcc, float gain, u32 samples) const\n{\n\tu32 seconds = dest_sample / m_ft;\n\tu32 dsamp = dest_sample % m_ft;\n\tu64 ssamp = (u64(dsamp) * m_fs * 0x1000) / m_ft;\n\tu64 ssample = (ssamp >> 12) + u64(m_fs) * seconds;\n\tu32 phase = ssamp & 0xfff;\n\tif(m_source_divide > 1) {\n\t\tu32 delta = ssample % m_source_divide;\n\t\tphase = (phase | (delta << 12)) / m_source_divide;\n\t\tssample -= delta;\n\t}\n\n\tgain *= 32768;\n\n\t// We're getting 2 samples latency, which is small enough\n\n\tssample -= 4*m_source_divide;\n\n\tconst sample_t *s = src.ptrs(srcc, ssample - src.sync_sample());\n\n\tstd::function<sample_t()> reader;\n\tif(m_source_divide == 1)\n\t\treader = [s]() mutable -> sample_t { return *s++; };\n\telse\n\t\treader = [s, count = m_source_divide]() mutable -> sample_t { sample_t sm = 0; for(u32 i=0; i != count; i++) { sm += *s++; } return sm / count; };\n",
     "void audio_resampler_lofi::apply_add(const emu::detail::output_buffer_flat<sample_t> &src, std::vector<s16> &dest, u32 destc, int dchannels, u64 dest_sample, u32 srcc, float gain, u32 samples) const\n{\n\tu32 seconds = dest_sample / m_ft;\n\tu32 dsamp = dest_sample % m_ft;\n\tu64 ssamp = (u64(dsamp) * m_fs * 0x1000) / m_ft;\n\tu64 ssample = (ssamp >> 12) + u64(m_fs) * seconds;\n\tu32 phase = ssamp & 0xfff;\n\tif(m_source_divide > 1) {\n\t\tu32 delta = ssample % m_source_divide;\n\t\tphase = (phase | (delta << 12)) / m_source_divide;\n\t\tssample -= delta;\n\t}\n\n\tgain *= 32768;\n\n\t// We're getting 2 samples latency, which is small enough\n\n\tssample -= 4*m_source_divide;\n\n\tconst sample_t *s = src.ptrs(srcc, ssample - src.sync_sample());\n\n\t// Kairo68: the same reads and sums as before, inlined; through a std::function every source\n\t// sample was an indirect call on the emulation thread (Kairo68#23).\n\tauto reader = [&s, count = m_source_divide]() -> sample_t {\n\t\tif(count == 1)\n\t\t\treturn *s++;\n\t\tsample_t sm = 0;\n\t\tfor(u32 i=0; i != count; i++) { sm += *s++; }\n\t\treturn sm / count;\n\t};\n",),
]

# The 68000 reads and writes plain memory through host pointers (Kairo68#13, #14): each 4 KB page
# that is the same plain memory throughout, in order, is read and written as the space's memory
# handler would, without the dispatch; devices, unmapped space and ROM writes go through the space
# as before. Pages are forgotten whenever the address map changes. Off unless a host calls
# enable_direct_memory, which must not when something taps that memory.
M68000_DIRECT_MEMORY = [
    ('src/devices/cpu/m68000/m68000.h',
     '\t// Kairo68: the data accesses, counting those that may have a side effect for the idle-loop\n\t// skip (enable_idle_skip): every write, and every read at or above free_end.\n\tstruct counted_program {\n\t\tmemory_access<24, 1, 0, ENDIANNESS_BIG>::specific inner;\n\t\tu32 side_effects = 0;\n\t\toffs_t free_end = 0;\n\t\tcounted_program &operator=(const memory_access<24, 1, 0, ENDIANNESS_BIG>::specific &s) { inner = s; return *this; }\n\t\tu16 read_interruptible(offs_t address, u16 mask = 0xffff) {\n\t\t\tif(address >= free_end)\n\t\t\t\tside_effects++;\n\t\t\treturn inner.read_interruptible(address, mask);\n\t\t}\n\t\tvoid write_interruptible(offs_t address, u16 data, u16 mask = 0xffff) {\n\t\t\tside_effects++;\n\t\t\tinner.write_interruptible(address, data, mask);\n\t\t}\n\t};\n\n\t// Dynamic specifics, depending on supervisor state\n\tcounted_program m_program;\n\tmemory_access<24, 1, 0, ENDIANNESS_BIG>::specific m_opcodes;\n',
     "\t// Kairo68: plain memory read and written through host pointers instead of the address space's\n\t// dispatch (enable_direct_memory, Kairo68#13, #14). A 4 KB page is direct when every word of it\n\t// is the same plain memory, in order, exactly as the space's memory handler reads and writes it;\n\t// anything else (devices, unmapped space, ROM for writes) goes through the space as before.\n\t// Pages are looked at when first used and forgotten whenever the address map changes.\n\t// Only a table load and a test are inline in the instruction handlers: inlining more into\n\t// thousands of them cost more than the dispatch it saved.\n\tstruct direct_pages {\n\t\tstatic constexpr int PAGE_BITS = 12;\n\t\tstatic constexpr u32 PAGES = 1 << (24 - PAGE_BITS);\n\t\tstatic constexpr offs_t PAGE_MASK = (1 << PAGE_BITS) - 1;\n\t\tenum : u8 { UNKNOWN, DIRECT, OTHER };\n\t\taddress_space *space = nullptr;\n\t\tstd::array<u16 *, PAGES> base[2] = {};  // a direct page's first host word, else null\n\t\tstd::array<u8, PAGES> state[2];\n\t\tutil::notifier_subscription subscription;\n\t\tvoid forget() { base[0].fill(nullptr); base[1].fill(nullptr); state[0].fill(UNKNOWN); state[1].fill(UNKNOWN); }\n\t\t// The host word at address, looking at its page when it is first used; null when not direct.\n\t\tu16 *word(bool write, offs_t address);\n\t\tstatic u16 *const no_pages[PAGES]; // the table without enable_direct_memory: nothing is direct\n\t};\n\tstatic u32 direct_page(offs_t address) { return (address >> direct_pages::PAGE_BITS) & (direct_pages::PAGES - 1); }\n\tstatic u32 direct_word(offs_t address) { return (address & direct_pages::PAGE_MASK) >> 1; }\n\n\t// Kairo68: the data accesses, counting those that may have a side effect for the idle-loop\n\t// skip (enable_idle_skip): every write, and every read at or above free_end.\n\tstruct counted_program {\n\t\tmemory_access<24, 1, 0, ENDIANNESS_BIG>::specific inner;\n\t\tu32 side_effects = 0;\n\t\toffs_t free_end = 0;\n\t\tdirect_pages *direct = nullptr;\n\t\tu16 *const *read_pages = direct_pages::no_pages;\n\t\tu16 *const *write_pages = direct_pages::no_pages;\n\t\tcounted_program &operator=(const memory_access<24, 1, 0, ENDIANNESS_BIG>::specific &s) { inner = s; return *this; }\n\t\tu16 read_interruptible(offs_t address, u16 mask = 0xffff) {\n\t\t\tif(address >= free_end)\n\t\t\t\tside_effects++;\n\t\t\tif(u16 const *page = read_pages[direct_page(address)])\n\t\t\t\treturn page[direct_word(address)];\n\t\t\treturn read_other(address, mask);\n\t\t}\n\t\tvoid write_interruptible(offs_t address, u16 data, u16 mask = 0xffff) {\n\t\t\tside_effects++;\n\t\t\tif(u16 *page = write_pages[direct_page(address)]) {\n\t\t\t\tu16 &w = page[direct_word(address)];\n\t\t\t\tw = (w & ~mask) | (data & mask);\n\t\t\t\treturn;\n\t\t\t}\n\t\t\twrite_other(address, data, mask);\n\t\t}\n\t\tu16 read_other(offs_t address, u16 mask);\n\t\tvoid write_other(offs_t address, u16 data, u16 mask);\n\t};\n\n\t// Kairo68: the opcode fetches, through direct pages too (enable_direct_memory)\n\tstruct direct_opcodes {\n\t\tmemory_access<24, 1, 0, ENDIANNESS_BIG>::specific inner;\n\t\tdirect_pages *direct = nullptr;\n\t\tu16 *const *pages = direct_pages::no_pages;\n\t\tdirect_opcodes &operator=(const memory_access<24, 1, 0, ENDIANNESS_BIG>::specific &s) { inner = s; return *this; }\n\t\tu16 read_interruptible(offs_t address, u16 mask = 0xffff) {\n\t\t\tif(u16 const *page = pages[direct_page(address)])\n\t\t\t\treturn page[direct_word(address)];\n\t\t\treturn read_other(address, mask);\n\t\t}\n\t\tu16 read_other(offs_t address, u16 mask);\n\t};\n\n\t// Dynamic specifics, depending on supervisor state\n\tcounted_program m_program;\n\tdirect_opcodes m_opcodes;\n\tstd::unique_ptr<direct_pages> m_direct; // Kairo68: enable_direct_memory\n",),
    ('src/devices/cpu/m68000/m68000.h',
     '\tvoid enable_idle_skip(offs_t free_end);\n',
     "\tvoid enable_idle_skip(offs_t free_end);\n\n\t// Kairo68: read and write plain memory through host pointers (direct_pages). Nothing may tap\n\t// that memory: a tap's accesses would be skipped.\n\tvoid enable_direct_memory();\n",),
    ('src/devices/cpu/m68000/m68000.cpp',
     '// Everything a pass of a loop starts with: the registers, the prefetch and bus latches, the\n',
     'void m68000_device::enable_direct_memory()\n{\n\t// Every access goes to the program space here; other spaces or an MMU would need their own pages.\n\tif(m_mmu || m_s_opcodes != m_s_program || m_s_uprogram != m_s_program || m_s_uopcodes != m_s_program)\n\t\tthrow emu_fatalerror("%s: direct memory needs one program space and no MMU", tag());\n\tm_direct = std::make_unique<direct_pages>();\n\tm_direct->space = m_s_program;\n\tm_direct->forget();\n\tm_direct->subscription = m_s_program->add_change_notifier([this](read_or_write) { m_direct->forget(); });\n\tm_program.direct = m_direct.get();\n\tm_program.read_pages = m_direct->base[0].data();\n\tm_program.write_pages = m_direct->base[1].data();\n\tm_opcodes.direct = m_direct.get();\n\tm_opcodes.pages = m_direct->base[0].data();\n}\n\nu16 *const m68000_device::direct_pages::no_pages[direct_pages::PAGES] = {};\n\n// A page is direct when each of its words is at the host address after the one before.\nu16 *m68000_device::direct_pages::word(bool write, offs_t address)\n{\n\tu32 const page = direct_page(address);\n\tif(state[write][page] == UNKNOWN) {\n\t\toffs_t const first = page << PAGE_BITS;\n\t\tauto const pointer = [this, write](offs_t a) { return static_cast<u8 *>(write ? space->get_write_ptr(a) : space->get_read_ptr(a)); };\n\t\tu8 *const start = pointer(first);\n\t\tbool direct = start != nullptr;\n\t\tfor(offs_t offset = 2; direct && offset <= PAGE_MASK; offset += 2)\n\t\t\tdirect = pointer(first + offset) == start + offset;\n\t\tstate[write][page] = direct ? DIRECT : OTHER;\n\t\tbase[write][page] = direct ? reinterpret_cast<u16 *>(start) : nullptr;\n\t}\n\treturn base[write][page] ? base[write][page] + direct_word(address) : nullptr;\n}\n\n// Out of line: an access to a page not known to be direct.\nu16 m68000_device::counted_program::read_other(offs_t address, u16 mask)\n{\n\tif(direct)\n\t\tif(u16 const *w = direct->word(false, address))\n\t\t\treturn *w;\n\treturn inner.read_interruptible(address, mask);\n}\n\nvoid m68000_device::counted_program::write_other(offs_t address, u16 data, u16 mask)\n{\n\tif(direct)\n\t\tif(u16 *w = direct->word(true, address)) {\n\t\t\t*w = (*w & ~mask) | (data & mask);\n\t\t\treturn;\n\t\t}\n\tinner.write_interruptible(address, data, mask);\n}\n\nu16 m68000_device::direct_opcodes::read_other(offs_t address, u16 mask)\n{\n\tif(direct)\n\t\tif(u16 const *w = direct->word(false, address))\n\t\t\treturn *w;\n\treturn inner.read_interruptible(address, mask);\n}\n\n// Everything a pass of a loop starts with: the registers, the prefetch and bus latches, the\n',),
]

# The MFP's timer B clocks the keyboard USART: about 1,360 firings a frame, half of the 68000's
# timeslices (Kairo68#11). While the USART is idle (line marking, nothing to send, no interrupt),
# its clock edges only advance its internal counters, so timer B fires every 256 elapses and the
# edges between are replayed through the same callbacks before anything can see or change the
# MFP: a register access, a change on the serial input, a save. Every frame stays the same.
MFP_USART_CLOCK_REPLAY = [
    ('src/devices/machine/mc68901.h',
     '\tauto out_tbo_cb() { return m_out_tbo_cb.bind(); }\n',
     "\tauto out_tbo_cb() { return m_out_tbo_cb.bind(); }\n\t// Kairo68: timer B's output is this MFP's own USART clock and drives nothing else (it is bound to\n\t// tc_w and rc_w only), so while the USART is idle its clock edges can be replayed later\n\t// (usart_idle, Kairo68#11).\n\tvoid set_timer_b_usart_clock() { m_usart_clock = true; }\n",),
    ('src/devices/machine/mc68901.h',
     '\tvirtual void device_pre_save() override ATTR_COLD;\n',
     '\tvirtual void device_pre_save() override ATTR_COLD;\n\tvirtual void device_post_load() override ATTR_COLD;\n',),
    ('src/devices/machine/mc68901.h',
     '\tvoid start_timer_delay(int index);\n\tvoid schedule_timer_elapse(int index);\n',
     "\tvoid start_timer_delay(int index);\n\tvoid schedule_timer_elapse(int index);\n\t// Kairo68: the USART's clock edges replayed (set_timer_b_usart_clock).\n\tstatic constexpr int USART_HORIZON = 256; // elapses between timer B's firings while idle\n\tbool m_usart_clock = false;\n\tbool m_usart_deferred = false;\n\tattotime m_usart_next;                    // the next elapse not yet replayed\n\tbool usart_idle() const;\n\tvoid usart_catch_up();\n\tvoid usart_defer_or_run();\n\tvoid usart_resume();\n",),
    ('src/devices/machine/mc68901.cpp',
     '// Kairo68: the count where the main counter is 1 elapses; a counter of 0 counts 256.\nvoid mc68901_device::schedule_timer_elapse(int index)\n',
     "// Kairo68: whether timer B's elapses change nothing outside the USART's own counters: the\n// receiver's line marks with nothing in progress, the transmitter sends nothing and its output\n// already holds the level tx_clock would set, and no interrupt or loopback is involved. Every\n// rx_clock and tx_clock call then only advances m_rclk, m_si_scan, m_tclk and the like.\nbool mc68901_device::usart_idle() const\n{\n\tif ((m_ucr & UCR_START_STOP_1_2) == UCR_START_STOP_0_0)\n\t\treturn false;\n\tif ((m_tsr & TSR_OUTPUT_MASK) == TSR_OUTPUT_LOOP)\n\t\treturn false;\n\tif (!m_si || !m_last_si || (m_rsr & (RSR_CHAR_IN_PROGRESS | RSR_BREAK)))\n\t\treturn false;\n\tif (m_tbits != 0)\n\t\treturn false;\n\tif (m_tsr & TSR_XMIT_ENABLE)\n\t\treturn (m_tsr & TSR_BUFFER_EMPTY) && !(m_tsr & TSR_BREAK) && m_so;\n\treturn m_so == ((m_tsr & TSR_OUTPUT_MASK) != TSR_OUTPUT_LOW);\n}\n\n// Replays the elapses due by now, each as timer_count would have run it: the output toggles\n// (clocking the USART through tc_w and rc_w), the counter reloads and counts from that elapse.\nvoid mc68901_device::usart_catch_up()\n{\n\tif (!m_usart_deferred)\n\t\treturn;\n\tconst attotime now = machine().time();\n\twhile (m_usart_next <= now)\n\t{\n\t\tm_to[TIMER_B] = !m_to[TIMER_B];\n\t\tm_out_tbo_cb(m_to[TIMER_B]);\n\t\tm_tmc[TIMER_B] = m_tdr[TIMER_B];\n\t\tm_tick_base[TIMER_B] = m_usart_next;\n\t\tm_usart_next += timer_tick(TIMER_B) * (m_tdr[TIMER_B] ? m_tdr[TIMER_B] : 256);\n\t}\n}\n\n// After an elapse: while the USART stays idle, timer B fires only every USART_HORIZON elapses and\n// the ones between are replayed when due; otherwise it fires at every elapse as before.\nvoid mc68901_device::usart_defer_or_run()\n{\n\tconst bool eligible = m_usart_clock && timer_delay_prescale(TIMER_B) && !(m_ier & INT_MASK_TIMER[TIMER_B]) && usart_idle();\n\tif (!eligible)\n\t{\n\t\tusart_resume();\n\t\treturn;\n\t}\n\tif (!m_usart_deferred)\n\t{\n\t\tm_usart_deferred = true;\n\t\tm_usart_next = m_timer[TIMER_B]->expire();\n\t}\n\tconst attotime period = timer_tick(TIMER_B) * (m_tdr[TIMER_B] ? m_tdr[TIMER_B] : 256);\n\tm_timer[TIMER_B]->adjust(m_usart_next + period * (USART_HORIZON - 1) - machine().time(), TIMER_B);\n}\n\n// Fires timer B at its next elapse again, as if it had never been deferred.\nvoid mc68901_device::usart_resume()\n{\n\tif (!m_usart_deferred)\n\t\treturn;\n\tusart_catch_up();\n\tm_usart_deferred = false;\n\tm_timer[TIMER_B]->adjust(m_usart_next - machine().time(), TIMER_B);\n}\n\n// Kairo68: the count where the main counter is 1 elapses; a counter of 0 counts 256.\nvoid mc68901_device::schedule_timer_elapse(int index)\n",),
    ('src/devices/machine/mc68901.cpp',
     'void mc68901_device::device_pre_save()\n{\n',
     'void mc68901_device::device_pre_save()\n{\n\t// Kairo68: a saved state holds timer B as it runs without the replay\n\tusart_resume();\n',),
    ('src/devices/machine/mc68901.cpp',
     'TIMER_CALLBACK_MEMBER(mc68901_device::timer_count)\n{\n',
     '// Kairo68: a loaded state runs timer B at every elapse until the next one defers it again.\nvoid mc68901_device::device_post_load()\n{\n\tm_usart_deferred = false;\n}\n\nTIMER_CALLBACK_MEMBER(mc68901_device::timer_count)\n{\n\t// Kairo68: a deferred timer B fires at its horizon, which is an elapse: replay up to it\n\tif (param == TIMER_B && m_usart_deferred)\n\t{\n\t\tusart_catch_up();\n\t\tusart_defer_or_run();\n\t\treturn;\n\t}\n\n',),
    ('src/devices/machine/mc68901.cpp',
     '\tif (delay)\n\t\tstart_timer_delay(param);\n}\n',
     '\tif (delay)\n\t\tstart_timer_delay(param);\n\n\t// Kairo68: see whether timer B can defer its next elapses\n\tif (param == TIMER_B && m_usart_clock)\n\t\tusart_defer_or_run();\n}\n',),
    ('src/devices/machine/mc68901.cpp',
     'u8 mc68901_device::read(offs_t offset)\n{\n\tswitch (offset)\n',
     'u8 mc68901_device::read(offs_t offset)\n{\n\tusart_catch_up(); // Kairo68: the USART and timer B as they are now\n\tswitch (offset)\n',),
    ('src/devices/machine/mc68901.cpp',
     'void mc68901_device::write(offs_t offset, u8 data)\n{\n\tswitch (offset)\n',
     "void mc68901_device::write(offs_t offset, u8 data)\n{\n\tusart_resume(); // Kairo68: a write may end the USART's idle state or reprogram timer B\n\tswitch (offset)\n",),
    ('src/devices/machine/mc68901.cpp',
     'void mc68901_device::si_w(int state)\n{\n\tm_si = state;\n}\n',
     'void mc68901_device::si_w(int state)\n{\n\t// Kairo68: the clocks before the change saw the old level\n\tif (state != m_si)\n\t\tusart_resume();\n\tm_si = state;\n}\n',),
    ('src/devices/machine/mc68901.cpp',
     'void mc68901_device::device_reset()\n{\n',
     'void mc68901_device::device_reset()\n{\n\tm_usart_deferred = false; // Kairo68: the reset reprograms timer B\n',),
    ('src/mame/sharp/x68k.cpp',
     '\tm_mfpdev->out_tbo_cb().set(m_mfpdev, FUNC(mc68901_device::tc_w));\n\tm_mfpdev->out_tbo_cb().append(m_mfpdev, FUNC(mc68901_device::rc_w));\n',
     "\tm_mfpdev->out_tbo_cb().set(m_mfpdev, FUNC(mc68901_device::tc_w));\n\tm_mfpdev->out_tbo_cb().append(m_mfpdev, FUNC(mc68901_device::rc_w));\n\tm_mfpdev->set_timer_b_usart_clock(); // Kairo68: timer B clocks the keyboard's USART and nothing else\n",),
]

# draw_gfx_scanline's 512x512 modes with what each pixel reads taken once (Kairo68#18): the
# special-priority test once a call, the destination and special rows, the GVRAM rows and the hbegin
# and hend bounds once a line, instead of every pixel rereading the video registers and computing
# its row addresses through pix(). Each pixel takes the same steps on the same values.
X68K_GFX_HOIST = [
    ('src/mame/sharp/x68k_v.cpp',
     '\t\telse  // else 512x512 "real" screen size\n\t\t{\n\t\t\tif(m_video.reg[2] & (1 << priority))\n\t\t\t{\n\t\t\t\tint page = m_video.gfxlayer_pri[priority];\n\t\t\t\t// adjust for scroll registers\n\t\t\t\tswitch(m_video.reg[0] & 0x03)\n\t\t\t\t{\n\t\t\t\tcase 0x00: // 16 colours\n\t\t\t\t\txscr = m_crtc->xscr_gfx(page) & 0x1ff;\n\t\t\t\t\tyscr = m_crtc->yscr_gfx(page) & 0x1ff;\n\t\t\t\t\tlineoffset = (((scanline - m_crtc->vbegin() / divisor) + yscr) & 0x1ff) * 512;\n\t\t\t\t\tloc = xscr & 0x1ff;\n\t\t\t\t\tshift = 4;\n\t\t\t\t\tfor(pixel=m_crtc->hbegin();pixel<=m_crtc->hend();pixel++)\n\t\t\t\t\t{\n\t\t\t\t\t\tcolour = ((m_gvram[lineoffset + loc] >> page*shift) & 0x000f);\n\t\t\t\t\t\tif(ret && (colour & 1))\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tblend = true;\n\t\t\t\t\t\t\tcolour &= 0xfe;\n\t\t\t\t\t\t}\n\t\t\t\t\t\telse\n\t\t\t\t\t\t\tblend = false;\n\t\t\t\t\t\tif(colour || (priority == 3))\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tif(ret)\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tif(blend && bitmap.pix(scanline, pixel))\n\t\t\t\t\t\t\t\t\tbitmap.pix(scanline, pixel) = ((bitmap.pix(scanline, pixel) >> 1) & 0x7bde) + ((pal[colour] >> 1) & 0x7bde) + 1;\n\t\t\t\t\t\t\t\telse\n\t\t\t\t\t\t\t\t\tbitmap.pix(scanline, pixel) = (pal[colour] & 0xfffe) + blend;\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\telse if(((m_video.reg[2] & 0x1800) == 0x1000) && (colour & 1))\n\t\t\t\t\t\t\t\tm_special.pix(scanline, pixel) = colour;\n\t\t\t\t\t\t\telse\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tm_special.pix(scanline, pixel) = 0;\n\t\t\t\t\t\t\t\tbitmap.pix(scanline, pixel) = colour;\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t}\n\t\t\t\t\t\telse if(((m_video.reg[2] & 0x1800) == 0x1000) && m_special.pix(scanline, pixel))\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tbitmap.pix(scanline, pixel) = m_special.pix(scanline, pixel);\n\t\t\t\t\t\t\tm_special.pix(scanline, pixel) = 0;\n\t\t\t\t\t\t}\n\t\t\t\t\t\tloc++;\n\t\t\t\t\t\tloc &= 0x1ff;\n\t\t\t\t\t}\n\t\t\t\t\tbreak;\n\t\t\t\tcase 0x01: // 256 colours\n\t\t\t\t\tif(page == 0 || page == 2)\n\t\t\t\t\t{\n\t\t\t\t\t\t// What effect do priorites and plane disable have here? Does it work in 16bit color mode?\n\t\t\t\t\t\tuint16_t xscr0, yscr0, xscr1, yscr1;\n\t\t\t\t\t\tuint32_t loc0, loc1, lineoffset0, lineoffset1;\n\t\t\t\t\t\txscr0 = m_crtc->xscr_gfx(page) & 0x1ff;\n\t\t\t\t\t\tyscr0 = m_crtc->yscr_gfx(page) & 0x1ff;\n\t\t\t\t\t\txscr1 = m_crtc->xscr_gfx(page + 1) & 0x1ff;\n\t\t\t\t\t\tyscr1 = m_crtc->yscr_gfx(page + 1) & 0x1ff;\n\n\t\t\t\t\t\tlineoffset0 = (((scanline - m_crtc->vbegin() / divisor) + yscr0) & 0x1ff) * 512;\n\t\t\t\t\t\tloc0 = xscr0 & 0x1ff;\n\t\t\t\t\t\tlineoffset1 = (((scanline - m_crtc->vbegin() / divisor) + yscr1) & 0x1ff) * 512;\n\t\t\t\t\t\tloc1 = xscr1 & 0x1ff;\n\t\t\t\t\t\tshift = 4;\n\t\t\t\t\t\tfor(pixel=m_crtc->hbegin();pixel<=m_crtc->hend();pixel++)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tcolour = ((m_gvram[lineoffset0 + loc0] >> page*shift) & 0x000f);\n\t\t\t\t\t\t\tcolour |= ((m_gvram[lineoffset1 + loc1] >> page*shift) & 0x00f0);\n\t\t\t\t\t\t\tif(ret && (colour & 1))\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tblend = true;\n\t\t\t\t\t\t\t\tcolour &= 0xfe;\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\telse\n\t\t\t\t\t\t\t\tblend = false;\n\t\t\t\t\t\t\tif(colour || (priority == 3))\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tif(ret)\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tif(blend && bitmap.pix(scanline, pixel))\n\t\t\t\t\t\t\t\t\t\tbitmap.pix(scanline, pixel) = ((bitmap.pix(scanline, pixel) >> 1) & 0x7bde) + ((pal[colour] >> 1) & 0x7bde) + 1;\n\t\t\t\t\t\t\t\t\telse\n\t\t\t\t\t\t\t\t\t\tbitmap.pix(scanline, pixel) = (pal[colour] & 0xfffe) + blend;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\telse if(((m_video.reg[2] & 0x1800) == 0x1000) && (colour & 1))\n\t\t\t\t\t\t\t\t\tm_special.pix(scanline, pixel) = colour;\n\t\t\t\t\t\t\t\telse\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tbitmap.pix(scanline, pixel) = colour;\n\t\t\t\t\t\t\t\t\tm_special.pix(scanline, pixel) = 0;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\telse if(((m_video.reg[2] & 0x1800) == 0x1000) && m_special.pix(scanline, pixel))\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tbitmap.pix(scanline, pixel) = m_special.pix(scanline, pixel);\n\t\t\t\t\t\t\t\tm_special.pix(scanline, pixel) = 0;\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\tloc0++;\n\t\t\t\t\t\t\tloc0 &= 0x1ff;\n\t\t\t\t\t\t\tloc1++;\n\t\t\t\t\t\t\tloc1 &= 0x1ff;\n\t\t\t\t\t\t}\n\t\t\t\t\t}\n\t\t\t\t\tbreak;\n\t\t\t\tcase 0x03: // 65536 colours\n\t\t\t\t\txscr = m_crtc->xscr_gfx(0) & 0x1ff;\n\t\t\t\t\tyscr = m_crtc->yscr_gfx(0) & 0x1ff;\n\t\t\t\t\tlineoffset = (((scanline - m_crtc->vbegin() / divisor) + yscr) & 0x1ff) * 512;\n\t\t\t\t\tloc = xscr & 0x1ff;\n\t\t\t\t\tfor(pixel=m_crtc->hbegin();pixel<=m_crtc->hend();pixel++)\n\t\t\t\t\t{\n\t\t\t\t\t\tcolour = m_gvram[lineoffset + loc];\n\t\t\t\t\t\tif(colour || (priority == 3))\n\t\t\t\t\t\t\tbitmap.pix(scanline, pixel) = colour;\n\t\t\t\t\t\tloc++;\n\t\t\t\t\t\tloc &= 0x1ff;\n\t\t\t\t\t}\n\t\t\t\t\tret = false;\n\t\t\t\t\tbreak;\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\t}\n',
     '\t\telse  // else 512x512 "real" screen size\n\t\t{\n\t\t\tif(m_video.reg[2] & (1 << priority))\n\t\t\t{\n\t\t\t\tint page = m_video.gfxlayer_pri[priority];\n\t\t\t\t// Kairo68: the rows each pixel reads and writes, taken once a line; each pixel takes the\n\t\t\t\t// same steps on the same values as before (Kairo68#18).\n\t\t\t\tconst bool special = ((m_video.reg[2] & 0x1800) == 0x1000);\n\t\t\t\tuint16_t *const dest = &bitmap.pix(scanline, 0);\n\t\t\t\tuint16_t *const spec = &m_special.pix(scanline, 0);\n\t\t\t\tconst int first = m_crtc->hbegin(), last = m_crtc->hend();\n\t\t\t\t// adjust for scroll registers\n\t\t\t\tswitch(m_video.reg[0] & 0x03)\n\t\t\t\t{\n\t\t\t\tcase 0x00: // 16 colours\n\t\t\t\t{\n\t\t\t\t\txscr = m_crtc->xscr_gfx(page) & 0x1ff;\n\t\t\t\t\tyscr = m_crtc->yscr_gfx(page) & 0x1ff;\n\t\t\t\t\tlineoffset = (((scanline - m_crtc->vbegin() / divisor) + yscr) & 0x1ff) * 512;\n\t\t\t\t\tloc = xscr & 0x1ff;\n\t\t\t\t\tshift = 4;\n\t\t\t\t\tconst uint16_t *const row = &m_gvram[lineoffset];\n\t\t\t\t\tconst int plane = page*shift;\n\t\t\t\t\tfor(pixel=first;pixel<=last;pixel++)\n\t\t\t\t\t{\n\t\t\t\t\t\tcolour = ((row[loc] >> plane) & 0x000f);\n\t\t\t\t\t\tif(ret && (colour & 1))\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tblend = true;\n\t\t\t\t\t\t\tcolour &= 0xfe;\n\t\t\t\t\t\t}\n\t\t\t\t\t\telse\n\t\t\t\t\t\t\tblend = false;\n\t\t\t\t\t\tif(colour || (priority == 3))\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tif(ret)\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tif(blend && dest[pixel])\n\t\t\t\t\t\t\t\t\tdest[pixel] = ((dest[pixel] >> 1) & 0x7bde) + ((pal[colour] >> 1) & 0x7bde) + 1;\n\t\t\t\t\t\t\t\telse\n\t\t\t\t\t\t\t\t\tdest[pixel] = (pal[colour] & 0xfffe) + blend;\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\telse if(special && (colour & 1))\n\t\t\t\t\t\t\t\tspec[pixel] = colour;\n\t\t\t\t\t\t\telse\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tspec[pixel] = 0;\n\t\t\t\t\t\t\t\tdest[pixel] = colour;\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t}\n\t\t\t\t\t\telse if(special && spec[pixel])\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tdest[pixel] = spec[pixel];\n\t\t\t\t\t\t\tspec[pixel] = 0;\n\t\t\t\t\t\t}\n\t\t\t\t\t\tloc++;\n\t\t\t\t\t\tloc &= 0x1ff;\n\t\t\t\t\t}\n\t\t\t\t\tbreak;\n\t\t\t\t}\n\t\t\t\tcase 0x01: // 256 colours\n\t\t\t\t\tif(page == 0 || page == 2)\n\t\t\t\t\t{\n\t\t\t\t\t\t// What effect do priorites and plane disable have here? Does it work in 16bit color mode?\n\t\t\t\t\t\tuint16_t xscr0, yscr0, xscr1, yscr1;\n\t\t\t\t\t\tuint32_t loc0, loc1, lineoffset0, lineoffset1;\n\t\t\t\t\t\txscr0 = m_crtc->xscr_gfx(page) & 0x1ff;\n\t\t\t\t\t\tyscr0 = m_crtc->yscr_gfx(page) & 0x1ff;\n\t\t\t\t\t\txscr1 = m_crtc->xscr_gfx(page + 1) & 0x1ff;\n\t\t\t\t\t\tyscr1 = m_crtc->yscr_gfx(page + 1) & 0x1ff;\n\n\t\t\t\t\t\tlineoffset0 = (((scanline - m_crtc->vbegin() / divisor) + yscr0) & 0x1ff) * 512;\n\t\t\t\t\t\tloc0 = xscr0 & 0x1ff;\n\t\t\t\t\t\tlineoffset1 = (((scanline - m_crtc->vbegin() / divisor) + yscr1) & 0x1ff) * 512;\n\t\t\t\t\t\tloc1 = xscr1 & 0x1ff;\n\t\t\t\t\t\tshift = 4;\n\t\t\t\t\t\tconst uint16_t *const row0 = &m_gvram[lineoffset0];\n\t\t\t\t\t\tconst uint16_t *const row1 = &m_gvram[lineoffset1];\n\t\t\t\t\t\tconst int plane = page*shift;\n\t\t\t\t\t\tfor(pixel=first;pixel<=last;pixel++)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tcolour = ((row0[loc0] >> plane) & 0x000f);\n\t\t\t\t\t\t\tcolour |= ((row1[loc1] >> plane) & 0x00f0);\n\t\t\t\t\t\t\tif(ret && (colour & 1))\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tblend = true;\n\t\t\t\t\t\t\t\tcolour &= 0xfe;\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\telse\n\t\t\t\t\t\t\t\tblend = false;\n\t\t\t\t\t\t\tif(colour || (priority == 3))\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tif(ret)\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tif(blend && dest[pixel])\n\t\t\t\t\t\t\t\t\t\tdest[pixel] = ((dest[pixel] >> 1) & 0x7bde) + ((pal[colour] >> 1) & 0x7bde) + 1;\n\t\t\t\t\t\t\t\t\telse\n\t\t\t\t\t\t\t\t\t\tdest[pixel] = (pal[colour] & 0xfffe) + blend;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\telse if(special && (colour & 1))\n\t\t\t\t\t\t\t\t\tspec[pixel] = colour;\n\t\t\t\t\t\t\t\telse\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tdest[pixel] = colour;\n\t\t\t\t\t\t\t\t\tspec[pixel] = 0;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\telse if(special && spec[pixel])\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tdest[pixel] = spec[pixel];\n\t\t\t\t\t\t\t\tspec[pixel] = 0;\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\tloc0++;\n\t\t\t\t\t\t\tloc0 &= 0x1ff;\n\t\t\t\t\t\t\tloc1++;\n\t\t\t\t\t\t\tloc1 &= 0x1ff;\n\t\t\t\t\t\t}\n\t\t\t\t\t}\n\t\t\t\t\tbreak;\n\t\t\t\tcase 0x03: // 65536 colours\n\t\t\t\t{\n\t\t\t\t\txscr = m_crtc->xscr_gfx(0) & 0x1ff;\n\t\t\t\t\tyscr = m_crtc->yscr_gfx(0) & 0x1ff;\n\t\t\t\t\tlineoffset = (((scanline - m_crtc->vbegin() / divisor) + yscr) & 0x1ff) * 512;\n\t\t\t\t\tloc = xscr & 0x1ff;\n\t\t\t\t\tconst uint16_t *const row = &m_gvram[lineoffset];\n\t\t\t\t\tfor(pixel=first;pixel<=last;pixel++)\n\t\t\t\t\t{\n\t\t\t\t\t\tcolour = row[loc];\n\t\t\t\t\t\tif(colour || (priority == 3))\n\t\t\t\t\t\t\tdest[pixel] = colour;\n\t\t\t\t\t\tloc++;\n\t\t\t\t\t\tloc &= 0x1ff;\n\t\t\t\t\t}\n\t\t\t\t\tret = false;\n\t\t\t\t\tbreak;\n\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\t}\n',),
]

# The floppy's flux cache checks the cell after the cached one before searching the whole track
# (Kairo68#5): a read asks for the next transition cell after cell, and each miss searched the
# track with std::upper_bound. The check accepts only the index that search returns.
FLOPPY_NEXT_CELL = [
    ('src/devices/imagedev/floppy.cpp',
     '\tattotime base;\n\tuint32_t position = find_position(base, when);\n\n\tauto const it = std::upper_bound(\n\t\t\tbuf.begin(), buf.end(), position,\n\t\t\t[] (uint32_t a, uint32_t b) { return a < (b & floppy_image::TIME_MASK); });\n\n\tint index;\n\tif(buf.begin() == it) {\n\t\tbase -= m_rev_time;\n\t\tindex = buf.size() - 1;\n\t} else {\n\t\tindex = int(it - buf.begin()) - 1;\n\t}\n',
     "\tattotime base;\n\tuint32_t position = find_position(base, when);\n\n\t// Kairo68: a read moves forward one flux cell at a time, so the cell after the cached one is\n\t// usually the one wanted. It is the cell the search below finds exactly when its position is at\n\t// most position and the next cell's is above it (Kairo68#5); otherwise search.\n\tint index;\n\tuint32_t const next = m_cache_index + 1;\n\tif(next < cells && (buf[next] & floppy_image::TIME_MASK) <= position &&\n\t\t\t(next + 1 == cells || position < (buf[next + 1] & floppy_image::TIME_MASK))) {\n\t\tindex = int(next);\n\t} else {\n\t\tauto const it = std::upper_bound(\n\t\t\t\tbuf.begin(), buf.end(), position,\n\t\t\t\t[] (uint32_t a, uint32_t b) { return a < (b & floppy_image::TIME_MASK); });\n\n\t\tif(buf.begin() == it) {\n\t\t\tbase -= m_rev_time;\n\t\t\tindex = buf.size() - 1;\n\t\t} else {\n\t\t\tindex = int(it - buf.begin()) - 1;\n\t\t}\n\t}\n",),
]

# Drawing that no longer takes a screen_update call per scroll change, and its per-pixel work in
# shorter forms that store the same values (Kairo68#29). First Blow rewrites the graphics scroll about
# 96 times a frame per page, which took 265 screen_update calls a frame.
# - A scroll register write (CRTC R10 to R19) records the registers for the lines the beam has
#   passed and leaves them waiting instead of drawing them; screen_update draws each line with its
#   own registers, and in the double-scanned graphics mode draws each line's graphics row just
#   before mixing the line. Every other picture change still draws first; nothing is recorded in
#   the special priority mode, whose mix reads rows by another divisor; a save draws waiting lines.
# - draw_sprites skips sprites whose rows lie outside the lines being drawn before prio_transpen
#   clips them; the 16-colour graphics page without blending or the special priority draws in runs
#   that don't wrap GVRAM's row, as masks the compiler vectorizes; the layer under BG takes its rows
#   and pens once a line; the text layer decodes a byte of each plane at a time through a table;
#   the mix runs as a function with the layers' priority levels and the blend mode as constants.
X68K_LINE_SCROLL = [
    ('src/mame/sharp/x68k_crtc.h',
     '\tvoid flush_partial();\n\n\t// getters\n\tu16 xscr_text() const { return m_reg[10]; }\n\tu16 yscr_text() const { return m_reg[11]; }\n',
     "\tvoid flush_partial();\n\t// Kairo68: the scroll registers (R10 to R19) a line is drawn with: those in effect when the beam\n\t// passed it, recorded by a scroll register write that leaves the line waiting, or the current\n\t// ones (tools/patches.py X68K_LINE_SCROLL).\n\tbool has_scroll_records(int first, int last) const { return m_scroll_last >= 0 && first <= m_scroll_last && last >= m_scroll_first; }\n\t// Kairo68: the driver's test of whether lines can wait for drawing with their scroll registers\n\t// recorded; without one, every write draws them first.\n\tvoid set_scroll_can_wait(std::function<bool ()> test) { m_scroll_can_wait = std::move(test); }\n\tconst u16 *scroll_regs(int line) const { return (line >= m_scroll_first && line <= m_scroll_last) ? m_line_scroll[line] : &m_reg[10]; }\n\n\t// getters\n\tu16 xscr_text() const { return m_reg[10]; }\n\tu16 xscr_text(int line) const { return scroll_regs(line)[0]; }\n\tu16 yscr_text(int line) const { return scroll_regs(line)[1]; }\n\tu16 xscr_gfx(int line, int page) const { return scroll_regs(line)[2 + page * 2]; }\n\tu16 yscr_gfx(int line, int page) const { return scroll_regs(line)[3 + page * 2]; }\n\tu16 yscr_text() const { return m_reg[11]; }\n",),
    ('src/mame/sharp/x68k_crtc.h',
     '\tvirtual void device_reset() override ATTR_COLD;\n\n',
     '\tvirtual void device_reset() override ATTR_COLD;\n\tvirtual void device_pre_save() override ATTR_COLD;\n\tvirtual void device_post_load() override ATTR_COLD;\n\n',),
    ('src/mame/sharp/x68k_crtc.h',
     '\tTIMER_CALLBACK_MEMBER(hsync);\n\tTIMER_CALLBACK_MEMBER(raster_end);\n',
     '\tTIMER_CALLBACK_MEMBER(hsync);\n\tvoid record_scroll();\n\tvoid forget_scroll();\n\tTIMER_CALLBACK_MEMBER(raster_end);\n',),
    ('src/mame/sharp/x68k_crtc.h',
     '\tint m_partial_line = -1;    // Kairo68: the last line the beam passed that is not drawn yet\n\temu_timer *m_raster_irq_timer;\n',
     '\tint m_partial_line = -1;    // Kairo68: the last line the beam passed that is not drawn yet\n\t// Kairo68: the scroll registers recorded for lines m_scroll_first to m_scroll_last (none when\n\t// m_scroll_last < 0), and the last line flush_partial drew this frame.\n\tu16 m_line_scroll[1024][10];\n\tint m_scroll_first = 0;\n\tint m_scroll_last = -1;\n\tint m_drawn_through = -1;\n\tstd::function<bool ()> m_scroll_can_wait;\n\temu_timer *m_raster_irq_timer;\n',),
    ('src/mame/sharp/x68k_crtc.cpp',
     '\tm_hsync_callback(1);\n}\n',
     '\tm_hsync_callback(1);\n\tforget_scroll();\n}\n\n// Kairo68: a saved state holds no recorded scroll registers: the lines that use them are drawn\n// first. A loaded state starts with none.\nvoid x68k_crtc_device::device_pre_save()\n{\n\tif (m_scroll_last >= 0)\n\t\tflush_partial();\n}\n\nvoid x68k_crtc_device::device_post_load()\n{\n\tforget_scroll();\n}\n\nvoid x68k_crtc_device::forget_scroll()\n{\n\tm_scroll_last = -1;\n\tm_drawn_through = -1;\n}\n\n// Kairo68: a write to a scroll register would draw the lines the beam has passed (flush_partial)\n// before it changes them. Instead, record the registers for those lines and leave them waiting:\n// they are drawn with what they would have been drawn with now, by the next flush_partial or at\n// the end of the frame. Each such write then costs a copy instead of a screen_update call.\n// Every other write that changes the picture still calls flush_partial, which draws the waiting\n// lines and forgets the records, so the records are always the newest state of those lines.\nvoid x68k_crtc_device::record_scroll()\n{\n\tif (m_partial_line < 0)\n\t\treturn;\n\tconst int line = m_partial_line;\n\tif (screen().vpos() <= line)\n\t{\n\t\t// As flush_partial: the frame that ended drew the line.\n\t\tm_partial_line = -1;\n\t\tforget_scroll();\n\t\treturn;\n\t}\n\tif (line >= int(std::size(m_line_scroll)))\n\t\tthrow emu_fatalerror("Kairo68: scroll registers recorded for line %d", line);\n\tint from = m_scroll_last >= 0 ? m_scroll_last + 1 : m_drawn_through + 1;\n\tif (m_scroll_last < 0)\n\t\tm_scroll_first = from;\n\tfor (; from <= line; from++)\n\t\tstd::copy_n(&m_reg[10], 10, m_line_scroll[from]);\n\tif (line > m_scroll_last)\n\t\tm_scroll_last = line;\n}\n',),
    ('src/mame/sharp/x68k_crtc.cpp',
     '\t{\n\t\thsync_time = screen().time_until_pos(scan, m_hend);\n',
     '\t{\n\t\t// Kairo68: a new frame; the records were for the frame that ended, which drew their lines.\n\t\tif (scan == 0)\n\t\t\tforget_scroll();\n\t\thsync_time = screen().time_until_pos(scan, m_hend);\n',),
    ('src/mame/sharp/x68k_crtc.cpp',
     '\t\tif (screen().vpos() > line)\n\t\t\tscreen().update_partial(line);\n\t}\n',
     '\t\tif (screen().vpos() > line)\n\t\t{\n\t\t\tscreen().update_partial(line);\n\t\t\tm_drawn_through = line;\n\t\t}\n\t\t// Kairo68: the records were for lines up to this one, now drawn (or drawn by the frame\n\t\t// that ended).\n\t\tm_scroll_last = -1;\n\t}\n',),
    ('src/mame/sharp/x68k_crtc.cpp',
     '{\n\tflush_partial();\n\tif (offset < 0x24)\n',
     '{\n\t// Kairo68: R10 to R19 only move the layers.\n\tif (offset >= 10 && offset <= 19 && m_scroll_can_wait && m_scroll_can_wait())\n\t\trecord_scroll();\n\telse\n\t\tflush_partial();\n\tif (offset < 0x24)\n',),
    ('src/mame/sharp/x68k.h',
     '\tuint8_t m_text_line[2048];\n\tbool draw_gfx_scanline(bitmap_ind16 &bitmap, rectangle cliprect, uint8_t priority);\n\tbool draw_gfx(bitmap_rgb32 &bitmap,rectangle cliprect);\n\tvoid draw_sprites(bitmap_ind16 &bitmap, screen_device &screen, rectangle cliprect);\n',
     '\tuint8_t m_text_line[2048];\n\tbool draw_gfx_scanline(bitmap_ind16 &bitmap, rectangle cliprect, uint8_t priority, int scroll_line);\n\tbool draw_gfx(bitmap_rgb32 &bitmap,rectangle cliprect, int scroll_line = -1);\n\tvoid draw_sprites(bitmap_ind16 &bitmap, screen_device &screen, rectangle cliprect);\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\t\tdivisor = 2;\n\tconst int yscr = (m_crtc->yscr_text() & 0x3ff);\n\tconst uint32_t row = ((((line - m_crtc->vbegin()) / divisor) + yscr) & 0x3ff) * 64;\n\tconst int xorigin = (m_crtc->xscr_text() & 0x3ff) - m_crtc->hbegin();\n\t// Sixteen pixels share each plane word: read the four once per word. xscr is never negative,\n\t// so xscr / 16 is xscr >> 4.\n\tint word = -1;\n\tuint16_t plane0 = 0, plane1 = 0, plane2 = 0, plane3 = 0;\n\tfor (int pixel = m_crtc->hbegin(); pixel <= m_crtc->hend(); pixel++)\n\t{\n\t\tconst int xscr = xorigin + pixel;\n\t\tif ((xscr >> 4) != word)\n\t\t{\n\t\t\tword = xscr >> 4;\n\t\t\tconst uint32_t loc = (row + (word & 0x7f)) & 0xffff;\n\t\t\tplane0 = m_tvram[loc];\n\t\t\tplane1 = m_tvram[loc+0x10000];\n\t\t\tplane2 = m_tvram[loc+0x20000];\n\t\t\tplane3 = m_tvram[loc+0x30000];\n\t\t}\n\t\tconst int bit = 15 - (xscr & 0x0f);\n\t\tm_text_line[pixel] = ((plane0 >> bit) & 0x01) | (((plane1 >> bit) & 0x01) << 1)\n\t\t\t\t\t\t\t| (((plane2 >> bit) & 0x01) << 2) | (((plane3 >> bit) & 0x01) << 3);\n\t}\n}\n\nbool x68k_state::draw_gfx_scanline( bitmap_ind16 &bitmap, rectangle cliprect, uint8_t priority)\n{\n',
     '\t\tdivisor = 2;\n\tconst int yscr = (m_crtc->yscr_text(line) & 0x3ff);\n\tconst uint32_t row = ((((line - m_crtc->vbegin()) / divisor) + yscr) & 0x3ff) * 64;\n\tconst int xorigin = (m_crtc->xscr_text(line) & 0x3ff) - m_crtc->hbegin();\n\t// Sixteen pixels share each plane word (bit 15 the leftmost): each byte of a plane word gives\n\t// eight pixels\' bit of that plane at once, spread one bit a byte by a table. The words the line\n\t// covers are decoded whole, then the line is copied from its first pixel\'s place in the first\n\t// word (Kairo68#29). xscr is never negative, so xscr / 16 is xscr >> 4.\n\tstatic const auto spread = [] {\n\t\tstd::array<uint64_t, 256> table{};\n\t\tfor (int b = 0; b < 256; b++)\n\t\t\tfor (int i = 0; i < 8; i++)\n\t\t\t\tif (b & (0x80 >> i))\n\t\t\t\t\ttable[b] |= uint64_t(1) << (8 * i);\n\t\treturn table;\n\t}();\n\tconst int first = m_crtc->hbegin(), last = m_crtc->hend();\n\tif (last < first)\n\t\treturn;\n\tconst int first_word = (xorigin + first) >> 4, last_word = (xorigin + last) >> 4;\n\tuint8_t words[sizeof(m_text_line) + 32];\n\tif (size_t(last_word - first_word + 1) * 16 > std::size(words))\n\t\tthrow emu_fatalerror("Kairo68: the text layer line spans %d words", last_word - first_word + 1);\n\tuint8_t *out = words;\n\tfor (int word = first_word; word <= last_word; word++, out += 16)\n\t{\n\t\tconst uint32_t loc = (row + (word & 0x7f)) & 0xffff;\n\t\tconst uint16_t plane0 = m_tvram[loc], plane1 = m_tvram[loc+0x10000], plane2 = m_tvram[loc+0x20000], plane3 = m_tvram[loc+0x30000];\n\t\tconst uint64_t left = spread[plane0 >> 8] | (spread[plane1 >> 8] << 1) | (spread[plane2 >> 8] << 2) | (spread[plane3 >> 8] << 3);\n\t\tconst uint64_t right = spread[plane0 & 0xff] | (spread[plane1 & 0xff] << 1) | (spread[plane2 & 0xff] << 2) | (spread[plane3 & 0xff] << 3);\n\t\tfor (int i = 0; i < 8; i++)\n\t\t{\n\t\t\tout[i] = uint8_t(left >> (8 * i));\n\t\t\tout[8 + i] = uint8_t(right >> (8 * i));\n\t\t}\n\t}\n\tstd::copy_n(words + ((xorigin + first) & 15), last - first + 1, m_text_line + first);\n}\n\nbool x68k_state::draw_gfx_scanline( bitmap_ind16 &bitmap, rectangle cliprect, uint8_t priority, int scroll_line)\n{\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\t\t\t{\n\t\t\t\txscr = (m_crtc->xscr_gfx(0) & 0x3ff);\n\t\t\t\tyscr = (m_crtc->yscr_gfx(0) & 0x3ff);\n\t\t\t\tlineoffset = (((scanline - m_crtc->vbegin()) + yscr) & 0x3ff) * 1024;\n',
     '\t\t\t{\n\t\t\t\txscr = (m_crtc->xscr_gfx(scroll_line >= 0 ? scroll_line : scanline, 0) & 0x3ff);\n\t\t\t\tyscr = (m_crtc->yscr_gfx(scroll_line >= 0 ? scroll_line : scanline, 0) & 0x3ff);\n\t\t\t\tlineoffset = (((scanline - m_crtc->vbegin()) + yscr) & 0x3ff) * 1024;\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\t\t\t\t{\n\t\t\t\t\txscr = m_crtc->xscr_gfx(page) & 0x1ff;\n\t\t\t\t\tyscr = m_crtc->yscr_gfx(page) & 0x1ff;\n\t\t\t\t\tlineoffset = (((scanline - m_crtc->vbegin() / divisor) + yscr) & 0x1ff) * 512;\n',
     '\t\t\t\t{\n\t\t\t\t\txscr = m_crtc->xscr_gfx(scroll_line >= 0 ? scroll_line : scanline, page) & 0x1ff;\n\t\t\t\t\tyscr = m_crtc->yscr_gfx(scroll_line >= 0 ? scroll_line : scanline, page) & 0x1ff;\n\t\t\t\t\tlineoffset = (((scanline - m_crtc->vbegin() / divisor) + yscr) & 0x1ff) * 512;\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\t\t\t\t\tconst int plane = page*shift;\n\t\t\t\t\tfor(pixel=first;pixel<=last;pixel++)\n',
     "\t\t\t\t\tconst int plane = page*shift;\n\t\t\t\t\tif(!ret && !special)\n\t\t\t\t\t{\n\t\t\t\t\t\t// Kairo68: without blending or the special priority, a pixel whose colour shows\n\t\t\t\t\t\t// (any, on the bottom page) stores it and clears its special pixel. The same\n\t\t\t\t\t\t// stores in runs that don't wrap GVRAM's row, a form the compiler vectorizes\n\t\t\t\t\t\t// (Kairo68#29).\n\t\t\t\t\t\tuint32_t at = loc;\n\t\t\t\t\t\tfor(pixel=first;pixel<=last;)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tconst int run = std::min<int>(last - pixel + 1, 0x200 - at);\n\t\t\t\t\t\t\t// Three different bitmaps: GVRAM, the graphics layer and the special layer.\n\t\t\t\t\t\t\tconst uint16_t *__restrict const src = row + at;\n\t\t\t\t\t\t\tuint16_t *__restrict const d = dest + pixel;\n\t\t\t\t\t\t\tuint16_t *__restrict const sp = spec + pixel;\n\t\t\t\t\t\t\tif(priority == 3)\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tfor(int i = 0; i < run; i++)\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\td[i] = (src[i] >> plane) & 0x000f;\n\t\t\t\t\t\t\t\t\tsp[i] = 0;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\telse\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tfor(int i = 0; i < run; i++)\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\t// keep is all ones where the colour is 0: the pixel and its special pixel stay.\n\t\t\t\t\t\t\t\t\tconst uint16_t c = (src[i] >> plane) & 0x000f;\n\t\t\t\t\t\t\t\t\tconst uint16_t keep = uint16_t(-uint16_t(c == 0));\n\t\t\t\t\t\t\t\t\td[i] = (d[i] & keep) | c;\n\t\t\t\t\t\t\t\t\tsp[i] &= keep;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\tpixel += run;\n\t\t\t\t\t\t\tat = 0;\n\t\t\t\t\t\t}\n\t\t\t\t\t\tbreak;\n\t\t\t\t\t}\n\t\t\t\t\tfor(pixel=first;pixel<=last;pixel++)\n",),
    ('src/mame/sharp/x68k_v.cpp',
     '\t\t\t\t\t\tuint32_t loc0, loc1, lineoffset0, lineoffset1;\n\t\t\t\t\t\txscr0 = m_crtc->xscr_gfx(page) & 0x1ff;\n\t\t\t\t\t\tyscr0 = m_crtc->yscr_gfx(page) & 0x1ff;\n\t\t\t\t\t\txscr1 = m_crtc->xscr_gfx(page + 1) & 0x1ff;\n\t\t\t\t\t\tyscr1 = m_crtc->yscr_gfx(page + 1) & 0x1ff;\n\n',
     '\t\t\t\t\t\tuint32_t loc0, loc1, lineoffset0, lineoffset1;\n\t\t\t\t\t\txscr0 = m_crtc->xscr_gfx(scroll_line >= 0 ? scroll_line : scanline, page) & 0x1ff;\n\t\t\t\t\t\tyscr0 = m_crtc->yscr_gfx(scroll_line >= 0 ? scroll_line : scanline, page) & 0x1ff;\n\t\t\t\t\t\txscr1 = m_crtc->xscr_gfx(scroll_line >= 0 ? scroll_line : scanline, page + 1) & 0x1ff;\n\t\t\t\t\t\tyscr1 = m_crtc->yscr_gfx(scroll_line >= 0 ? scroll_line : scanline, page + 1) & 0x1ff;\n\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\t\t\t\t{\n\t\t\t\t\txscr = m_crtc->xscr_gfx(0) & 0x1ff;\n\t\t\t\t\tyscr = m_crtc->yscr_gfx(0) & 0x1ff;\n\t\t\t\t\tlineoffset = (((scanline - m_crtc->vbegin() / divisor) + yscr) & 0x1ff) * 512;\n',
     '\t\t\t\t{\n\t\t\t\t\txscr = m_crtc->xscr_gfx(scroll_line >= 0 ? scroll_line : scanline, 0) & 0x1ff;\n\t\t\t\t\tyscr = m_crtc->yscr_gfx(scroll_line >= 0 ? scroll_line : scanline, 0) & 0x1ff;\n\t\t\t\t\tlineoffset = (((scanline - m_crtc->vbegin() / divisor) + yscr) & 0x1ff) * 512;\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\nbool x68k_state::draw_gfx(bitmap_rgb32 &bitmap,rectangle cliprect)\n{\n',
     "\n// Kairo68: scroll_line is the screen line whose scroll registers draw every row of cliprect, or -1\n// for each row's own line (Kairo68#29).\nbool x68k_state::draw_gfx(bitmap_rgb32 &bitmap,rectangle cliprect, int scroll_line)\n{\n",),
    ('src/mame/sharp/x68k_v.cpp',
     '\t{\n\t\tgfxblend = draw_gfx_scanline(m_gfxbitmap,gfxrect,priority);\n\t}\n',
     '\t{\n\t\tgfxblend = draw_gfx_scanline(m_gfxbitmap,gfxrect,priority,scroll_line);\n\t}\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\t\tint sy = (m_spritereg[ptr+1] & 0x3ff) - 16;\n\n',
     '\t\tint sy = (m_spritereg[ptr+1] & 0x3ff) - 16;\n\t\t// Kairo68: a sprite whose 16 rows all lie outside the lines being drawn draws nothing; skip\n\t\t// it before prio_transpen, which would clip it whole (Kairo68#29).\n\t\tconst int top = (m_video.bg_vstart / divisor) + sy;\n\t\tif (top > cliprect.max_y || top + 15 < cliprect.min_y) continue;\n\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\n\tfor (gfx_index = 0; gfx_index < MAX_GFX_ELEMENTS; gfx_index++)\n',
     '\n\t// Kairo68: scroll register writes may leave lines waiting, with the registers recorded, except\n\t// in the special priority mode (screen_update).\n\tm_crtc->set_scroll_can_wait([this] { return (m_video.reg[2] & 0x1800) != 0x1000; });\n\n\tfor (gfx_index = 0; gfx_index < MAX_GFX_ELEMENTS; gfx_index++)\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\tm_bg1_16->mark_all_dirty();\n}\n\nuint32_t x68k_state::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)\n',
     '\tm_bg1_16->mark_all_dirty();\n}\n\nnamespace {\n\n// Kairo68: what screen_update\'s mix reads for every pixel, taken once a call: nothing it reads\n// changes while it runs (Kairo68#18).\nstruct x68k_mix\n{\n\tbool text_zero_shows, special, gfxzero;\n\tint gfxmode; // get_gfx_pixel, decided once: 65,536 colours, 16-bit colours (gfx16bcol), or the palette\n\tconst pen_t *pcgpens, *gfxpens;\n\tconst uint8_t *text;\n\n\t// get_gfx_pixel<false>.\n\tATTR_FORCE_INLINE rgb_t gfx_plain(uint16_t raw) const\n\t{\n\t\tif(gfxmode == 2)\n\t\t\treturn (raw || gfxzero) ? x68k_state::GGGGGRRRRRBBBBBI(raw) : rgb_t(0);\n\t\tif(gfxmode == 1)\n\t\t\treturn (raw || gfxzero) ? pal555(raw, 6, 11, 1) : rgb_t(0);\n\t\tconst pen_t pen = gfxpens[raw & 0xff];\n\t\treturn ((pen & 0xffffff) || gfxzero) ? rgb_t(pen) : rgb_t(0);\n\t}\n\n\t// get_gfx_pixel<true>.\n\tATTR_FORCE_INLINE rgb_t gfx_blend(uint16_t raw, rgb_t blendpix) const\n\t{\n\t\tif(gfxmode == 2)\n\t\t\treturn (raw || gfxzero) ? x68k_state::GGGGGRRRRRBBBBBI(raw) : rgb_t(0);\n\t\tif(gfxmode == 1)\n\t\t{\n\t\t\tif(!(raw || gfxzero))\n\t\t\t\treturn rgb_t(0);\n\t\t\tif(raw & 1)\n\t\t\t\treturn ((blendpix >> 1) & 0xff7f7f7f) + ((pal555(raw, 6, 11, 1) >> 2) & 0x3f3f3f);\n\t\t\treturn pal555(raw, 6, 11, 1);\n\t\t}\n\t\tuint16_t colour = raw & 0xff;\n\t\tconst bool half = colour & 1;\n\t\tif(half)\n\t\t\tcolour &= 0xfe;\n\t\tconst pen_t pen = gfxpens[colour];\n\t\tif(!((pen & 0xffffff) || gfxzero))\n\t\t\treturn rgb_t(0);\n\t\tif(half)\n\t\t\treturn ((blendpix >> 1) & 0xff7f7f7f) + ((pen >> 2) & 0x3f3f3f);\n\t\treturn rgb_t(pen);\n\t}\n};\n\n// Kairo68: one line of screen_update\'s mix, with the layers\' priority levels (-1 for a hidden\n// layer) and the blend mode as constants: the same steps on the same values for every pixel, with\n// the per-pixel tests of which layer a level holds decided by the compiler (Kairo68#29).\ntemplate <int TextPrio, int PcgPrio, int GfxPrio, bool Blend>\nvoid x68k_mix_line(const x68k_mix &m, uint32_t *dest, const uint16_t *pcgrow, const uint16_t *specialrow, const uint16_t *gfxrow, int first, int last)\n{\n\tfor(int pixel=first;pixel<=last;pixel++)\n\t{\n\t\tif(m.special) // special priority\n\t\t{\n\t\t\tuint16_t colour = specialrow[pixel] & 0xff;\n\t\t\t// XXX: this might check the pen color not the palette index\n\t\t\tif(colour & ~1)\n\t\t\t{\n\t\t\t\tdest[pixel] = m.gfxpens[colour & ~1];\n\t\t\t\tcontinue;\n\t\t\t}\n\t\t}\n\t\trgb_t outpix = dest[pixel], pix = 0;\n\t\tbool pcgpix = false, gfxpix = false;\n\t\tfor(int priority = 0; priority < 3; priority++)\n\t\t{\n\t\t\tif(Blend)\n\t\t\t{\n\t\t\t\tif((priority == TextPrio) && !pcgpix)\n\t\t\t\t{\n\t\t\t\t\tconst uint16_t colour = m.text[pixel];\n\t\t\t\t\tif(colour || m.text_zero_shows)\n\t\t\t\t\t{\n\t\t\t\t\t\tpix = m.pcgpens[colour];\n\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t\telse if((priority == PcgPrio) && !pcgpix)\n\t\t\t\t{\n\t\t\t\t\tuint16_t colour = pcgrow[pixel] & 0xff;\n\t\t\t\t\tif(colour)\n\t\t\t\t\t{\n\t\t\t\t\t\tpix = m.pcgpens[colour];\n\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t}\n\t\t\t\t\telse if(outpix && (PcgPrio > TextPrio) && (TextPrio != -1))\n\t\t\t\t\t\tpix = outpix;\n\t\t\t\t}\n\t\t\t\telse if(priority == GfxPrio)\n\t\t\t\t\tgfxpix = true;\n\t\t\t\tif((pix & 0xffffff) || ((priority == 2) && gfxpix))\n\t\t\t\t{\n\t\t\t\t\tif(!(pix & 0xffffff))\n\t\t\t\t\t\tpix = outpix;\n\t\t\t\t\trgb_t blendpix = m.gfx_blend(gfxrow[pixel], pix);\n\t\t\t\t\toutpix = blendpix & 0xffffff ? blendpix : pix;\n\t\t\t\t\tbreak;\n\t\t\t\t}\n\t\t\t}\n\t\t\telse\n\t\t\t{\n\t\t\t\tif((priority == TextPrio) && !pcgpix)\n\t\t\t\t{\n\t\t\t\t\tconst uint16_t colour = m.text[pixel];\n\t\t\t\t\tif(colour || m.text_zero_shows)\n\t\t\t\t\t{\n\t\t\t\t\t\tpix = m.pcgpens[colour];\n\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t\telse if((priority == PcgPrio) && !pcgpix)\n\t\t\t\t{\n\t\t\t\t\tuint16_t colour = pcgrow[pixel] & 0xff;\n\t\t\t\t\tif(colour)\n\t\t\t\t\t{\n\t\t\t\t\t\tpix = m.pcgpens[colour];\n\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t}\n\t\t\t\t\telse if(outpix && (PcgPrio > TextPrio) && (TextPrio != -1))\n\t\t\t\t\t\tpix = outpix;\n\t\t\t\t}\n\t\t\t\telse if(priority == GfxPrio)\n\t\t\t\t\tpix = m.gfx_plain(gfxrow[pixel]);\n\t\t\t\tif(pix & 0xffffff)\n\t\t\t\t{\n\t\t\t\t\toutpix = pix;\n\t\t\t\t\tbreak;\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\t\tdest[pixel] = outpix;\n\t}\n}\n\nusing x68k_mix_fn = void (*)(const x68k_mix &, uint32_t *, const uint16_t *, const uint16_t *, const uint16_t *, int, int);\n\n// The levels screen_update gives the shown layers are 0, 1, 2 in order, so these are every case.\nx68k_mix_fn x68k_mixer(int textprio, int pcgprio, int gfxprio, bool blend)\n{\n\tswitch((textprio + 1) * 16 + (pcgprio + 1) * 4 + (gfxprio + 1))\n\t{\n#define X68K_MIXER(t, p, g) case (t + 1) * 16 + (p + 1) * 4 + (g + 1): \t\treturn blend ? &x68k_mix_line<t, p, g, true> : &x68k_mix_line<t, p, g, false>;\n\tX68K_MIXER(-1, -1, -1)\n\tX68K_MIXER(0, -1, -1) X68K_MIXER(-1, 0, -1) X68K_MIXER(-1, -1, 0)\n\tX68K_MIXER(0, 1, -1) X68K_MIXER(1, 0, -1) X68K_MIXER(0, -1, 1) X68K_MIXER(1, -1, 0) X68K_MIXER(-1, 0, 1) X68K_MIXER(-1, 1, 0)\n\tX68K_MIXER(0, 1, 2) X68K_MIXER(0, 2, 1) X68K_MIXER(1, 0, 2) X68K_MIXER(1, 2, 0) X68K_MIXER(2, 0, 1) X68K_MIXER(2, 1, 0)\n#undef X68K_MIXER\n\t}\n\tthrow emu_fatalerror("Kairo68: layer priority levels text %d, sprites %d, graphics %d", textprio, pcgprio, gfxprio);\n}\n\n} // anonymous namespace\n\nuint32_t x68k_state::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\t\t\tdivisor = 2;\n\t\tfor(scanline=rect.min_y;scanline<=rect.max_y;scanline++)\n\t\t{\n\t\t\tfor(pixel=m_crtc->hbegin();pixel<=m_crtc->hend();pixel++)\n\t\t\t{\n\t\t\t\tuint8_t colour = m_pcgbitmap.pix(scanline / divisor, pixel) & 0xff;\n\t\t\t\tbitmap.pix(scanline, pixel) = m_pcgpalette->pen(colour);\n\t\t\t}\n\t\t}\n',
     '\t\t\tdivisor = 2;\n\t\t// Kairo68: the rows and pens taken once a line (Kairo68#29).\n\t\tconst pen_t *const pens = m_pcgpalette->pens();\n\t\tfor(scanline=rect.min_y;scanline<=rect.max_y;scanline++)\n\t\t{\n\t\t\tconst uint16_t *const src = &m_pcgbitmap.pix(scanline / divisor, 0);\n\t\t\tuint32_t *const dst = &bitmap.pix(scanline, 0);\n\t\t\tfor(pixel=m_crtc->hbegin();pixel<=m_crtc->hend();pixel++)\n\t\t\t\tdst[pixel] = pens[src[pixel] & 0xff];\n\t\t}\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\tint divisor = 1;\n\t// Graphics screen(s)\n\tgfx16bcol = draw_gfx(bitmap,rect);\n\tKAIRO68_SCREEN_LAP(screen_gfx_ns);\n',
     '\tint divisor = 1;\n\t// Kairo68: lines of rect can have scroll registers recorded from when the beam passed them\n\t// (x68k_crtc_device::record_scroll). Each graphics row is drawn with its line\'s registers. When\n\t// the graphics are double-scanned, two screen lines share a row but can have different\n\t// registers, so each line\'s row is drawn just before the line is mixed, as a screen_update\n\t// call for that line alone would have drawn it (Kairo68#29). Records are only made outside\n\t// the special priority mode, whose mix reads graphics rows by another divisor.\n\tconst bool gfx_per_line = m_crtc->gfx_double_scan() && m_crtc->has_scroll_records(rect.min_y, rect.max_y);\n\tif (m_crtc->has_scroll_records(rect.min_y, rect.max_y) && ((m_video.reg[2] & 0x1800) == 0x1000))\n\t\tthrow emu_fatalerror("Kairo68: scroll registers recorded in the special priority mode");\n\t// Graphics screen(s)\n\tif (gfx_per_line)\n\t\tgfx16bcol = draw_gfx(bitmap, rectangle(rect.min_x, rect.max_x, rect.min_y, rect.min_y), rect.min_y);\n\telse\n\t\tgfx16bcol = draw_gfx(bitmap, rect, m_crtc->gfx_double_scan() ? rect.min_y : -1);\n\tKAIRO68_SCREEN_LAP(screen_gfx_ns);\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\tbool blend = (((m_video.reg[2] & 0x1900) == 0x1900) && (m_video.gfx_pri != 2));\n\t// Kairo68: colour 0 of the text layer shows at priority level 2 (get_text_pixel).\n\tconst bool text_zero_shows = ((m_video.reg[1] & 0x0c00) == 0x0800);\n\t// Kairo68: what the mix reads for every pixel, taken once: nothing it reads changes while it runs.\n\tconst bool special = ((m_video.reg[2] & 0x1800) == 0x1000);\n\tconst pen_t *const pcgpens = m_pcgpalette->pens();\n\tconst pen_t *const gfxpens = m_gfxpalette->pens();\n\t// get_gfx_pixel, decided once: 65,536 colours, 16-bit colours (gfx16bcol), or the palette.\n\tconst int gfxmode = ((m_video.reg[0] & 0x03) == 3) ? 2 : (gfx16bcol ? 1 : 0);\n\tconst bool gfxzero = (m_video.gfx_pri == 2);\n\tconst int gfxdivisor = m_crtc->gfx_double_scan() ? 2 : 1;\n\t// get_gfx_pixel<false>.\n\tauto gfx_plain = [&](uint16_t raw) -> rgb_t\n\t{\n\t\tif(gfxmode == 2)\n\t\t\treturn (raw || gfxzero) ? GGGGGRRRRRBBBBBI(raw) : rgb_t(0);\n\t\tif(gfxmode == 1)\n\t\t\treturn (raw || gfxzero) ? pal555(raw, 6, 11, 1) : rgb_t(0);\n\t\tconst pen_t pen = gfxpens[raw & 0xff];\n\t\treturn ((pen & 0xffffff) || gfxzero) ? rgb_t(pen) : rgb_t(0);\n\t};\n\t// get_gfx_pixel<true>.\n\tauto gfx_blend = [&](uint16_t raw, rgb_t blendpix) -> rgb_t\n\t{\n\t\tif(gfxmode == 2)\n\t\t\treturn (raw || gfxzero) ? GGGGGRRRRRBBBBBI(raw) : rgb_t(0);\n\t\tif(gfxmode == 1)\n\t\t{\n\t\t\tif(!(raw || gfxzero))\n\t\t\t\treturn rgb_t(0);\n\t\t\tif(raw & 1)\n\t\t\t\treturn ((blendpix >> 1) & 0xff7f7f7f) + ((pal555(raw, 6, 11, 1) >> 2) & 0x3f3f3f);\n\t\t\treturn pal555(raw, 6, 11, 1);\n\t\t}\n\t\tuint16_t colour = raw & 0xff;\n\t\tconst bool half = colour & 1;\n\t\tif(half)\n\t\t\tcolour &= 0xfe;\n\t\tconst pen_t pen = gfxpens[colour];\n\t\tif(!((pen & 0xffffff) || gfxzero))\n\t\t\treturn rgb_t(0);\n\t\tif(half)\n\t\t\treturn ((blendpix >> 1) & 0xff7f7f7f) + ((pen >> 2) & 0x3f3f3f);\n\t\treturn rgb_t(pen);\n\t};\n\tfor(scanline=rect.min_y;scanline<=rect.max_y;scanline++)\n\t{\n\t\tif(textprio != -1)\n',
     '\tbool blend = (((m_video.reg[2] & 0x1900) == 0x1900) && (m_video.gfx_pri != 2));\n\tx68k_mix mix;\n\t// Kairo68: colour 0 of the text layer shows at priority level 2 (get_text_pixel).\n\tmix.text_zero_shows = ((m_video.reg[1] & 0x0c00) == 0x0800);\n\tmix.special = ((m_video.reg[2] & 0x1800) == 0x1000);\n\tmix.pcgpens = m_pcgpalette->pens();\n\tmix.gfxpens = m_gfxpalette->pens();\n\tmix.gfxmode = ((m_video.reg[0] & 0x03) == 3) ? 2 : (gfx16bcol ? 1 : 0);\n\tmix.gfxzero = (m_video.gfx_pri == 2);\n\tmix.text = m_text_line;\n\tconst x68k_mix_fn mixer = x68k_mixer(textprio, pcgprio, gfxprio, blend);\n\tconst int gfxdivisor = m_crtc->gfx_double_scan() ? 2 : 1;\n\tint gfx_row_line = rect.min_y; // Kairo68: the line whose registers drew the graphics row last\n\tfor(scanline=rect.min_y;scanline<=rect.max_y;scanline++)\n\t{\n\t\tif(gfx_per_line && scanline != rect.min_y)\n\t\t{\n\t\t\tconst uint16_t *const regs = m_crtc->scroll_regs(scanline);\n\t\t\tif(((scanline >> 1) != (gfx_row_line >> 1)) || !std::equal(regs, regs + 10, m_crtc->scroll_regs(gfx_row_line)))\n\t\t\t{\n\t\t\t\tdraw_gfx(bitmap, rectangle(rect.min_x, rect.max_x, scanline, scanline), scanline);\n\t\t\t\tgfx_row_line = scanline;\n\t\t\t}\n\t\t}\n\t\tif(textprio != -1)\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\t\tconst uint16_t *const gfxrow = &m_gfxbitmap.pix(scanline / gfxdivisor, 0);\n\t\tfor(pixel=m_crtc->hbegin();pixel<=m_crtc->hend();pixel++)\n\t\t{\n\t\t\tif(special) // special priority\n\t\t\t{\n\t\t\t\tuint16_t colour = specialrow[pixel] & 0xff;\n\t\t\t\t// XXX: this might check the pen color not the palette index\n\t\t\t\tif(colour & ~1)\n\t\t\t\t{\n\t\t\t\t\tdest[pixel] = gfxpens[colour & ~1];\n\t\t\t\t\tcontinue;\n\t\t\t\t}\n\t\t\t}\n\t\t\trgb_t outpix = dest[pixel], pix = 0;\n\t\t\tbool pcgpix = false, gfxpix = false;\n\t\t\tfor(priority = 0; priority < 3; priority++)\n\t\t\t{\n\t\t\t\tif(blend)\n\t\t\t\t{\n\t\t\t\t\tif((priority == textprio) && !pcgpix)\n\t\t\t\t\t{\n\t\t\t\t\t\tconst uint16_t colour = m_text_line[pixel];\n\t\t\t\t\t\tif(colour || text_zero_shows)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tpix = pcgpens[colour];\n\t\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t\t}\n\t\t\t\t\t}\n\t\t\t\t\telse if((priority == pcgprio) && !pcgpix)\n\t\t\t\t\t{\n\t\t\t\t\t\tuint16_t colour = pcgrow[pixel] & 0xff;\n\t\t\t\t\t\tif(colour)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tpix = pcgpens[colour];\n\t\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t\t}\n\t\t\t\t\t\telse if(outpix && (pcgprio > textprio) && (textprio != -1))\n\t\t\t\t\t\t\tpix = outpix;\n\t\t\t\t\t}\n\t\t\t\t\telse if(priority == gfxprio)\n\t\t\t\t\t\tgfxpix = true;\n\t\t\t\t\tif((pix & 0xffffff) || ((priority == 2) && gfxpix))\n\t\t\t\t\t{\n\t\t\t\t\t\tif(!(pix & 0xffffff))\n\t\t\t\t\t\t\tpix = outpix;\n\t\t\t\t\t\trgb_t blendpix = gfx_blend(gfxrow[pixel], pix);\n\t\t\t\t\t\toutpix = blendpix & 0xffffff ? blendpix : pix;\n\t\t\t\t\t\tbreak;\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t\telse\n\t\t\t\t{\n\t\t\t\t\tif((priority == textprio) && !pcgpix)\n\t\t\t\t\t{\n\t\t\t\t\t\tconst uint16_t colour = m_text_line[pixel];\n\t\t\t\t\t\tif(colour || text_zero_shows)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tpix = pcgpens[colour];\n\t\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t\t}\n\t\t\t\t\t}\n\t\t\t\t\telse if((priority == pcgprio) && !pcgpix)\n\t\t\t\t\t{\n\t\t\t\t\t\tuint16_t colour = pcgrow[pixel] & 0xff;\n\t\t\t\t\t\tif(colour)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tpix = pcgpens[colour];\n\t\t\t\t\t\t\tpcgpix = true;\n\t\t\t\t\t\t}\n\t\t\t\t\t\telse if(outpix && (pcgprio > textprio) && (textprio != -1))\n\t\t\t\t\t\t\tpix = outpix;\n\t\t\t\t\t}\n\t\t\t\t\telse if(priority == gfxprio)\n\t\t\t\t\t\tpix = gfx_plain(gfxrow[pixel]);\n\t\t\t\t\tif(pix & 0xffffff)\n\t\t\t\t\t{\n\t\t\t\t\t\toutpix = pix;\n\t\t\t\t\t\tbreak;\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t\tdest[pixel] = outpix;\n\t\t}\n\t}\n',
     '\t\tconst uint16_t *const gfxrow = &m_gfxbitmap.pix(scanline / gfxdivisor, 0);\n\t\tmixer(mix, dest, pcgrow, specialrow, gfxrow, m_crtc->hbegin(), m_crtc->hend());\n\t}\n',),
]

# set_ftu_const, which the instruction handlers call once an instruction, inline (Kairo68#29).
M68000_FTU_INLINE = [
    ('src/devices/cpu/m68000/m68000.h',
     '\tinline int map_sp(int r) { return r == 15 ? m_sp : r; }\n\tvoid set_ftu_const();\n\n',
     '\tinline int map_sp(int r) { return r == 15 ? m_sp : r; }\n\t// Kairo68: inline, as the instruction handlers call it once an instruction (Kairo68#29).\n\tinline void set_ftu_const() {\n\t\tswitch(m_ird >> 12) {\n\t\tcase 0x4: m_ftu = 0x80; break;\n\t\tcase 0x5: case 0xe: m_ftu = (m_ird >> 9) & 7; if(!m_ftu) m_ftu = 8; break;\n\t\tcase 0x6: case 0x7: m_ftu = s8(m_ird); break;\n\t\tcase 0x8: case 0xc: m_ftu = 0xf; break;\n\t\tdefault: m_ftu = 0; break;\n\t\t}\n\t}\n\n',),
    ('src/devices/cpu/m68000/m68000.cpp',
     'void m68000_device::set_ftu_const()\n{\n\tswitch(m_ird >> 12) {\n\tcase 0x4: m_ftu = 0x80; break;\n\tcase 0x5: case 0xe: m_ftu = (m_ird >> 9) & 7; if(!m_ftu) m_ftu = 8; break;\n\tcase 0x6: case 0x7: m_ftu = s8(m_ird); break;\n\tcase 0x8: case 0xc: m_ftu = 0xf; break;\n\tdefault: m_ftu = 0; break;\n\t}\n}\n\nvoid m68000_device::start_interrupt_vector_lookup()',
     'void m68000_device::start_interrupt_vector_lookup()',),
]

# The layer under BG and sprites is drawn to its own bitmap, and the mix starts each pixel from its
# pen there, or from pen 0 without it, instead of a pass that filled the screen bitmap with those
# pens for the mix to read back (Kairo68#29).
X68K_UNDER_ROW = [
    ('src/mame/sharp/x68k.h',
     '\tbitmap_ind16 m_pcgbitmap;\n\tbitmap_ind16 m_gfxbitmap;\n\tbitmap_ind16 m_special;\n',
     '\tbitmap_ind16 m_pcgbitmap;\n\tbitmap_ind16 m_gfxbitmap;\n\tbitmap_ind16 m_special;\n\tbitmap_ind16 m_underbitmap; // Kairo68: the BG layer drawn under everything (screen_update)\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\tm_screen->register_screen_bitmap(m_special);\n',
     '\tm_screen->register_screen_bitmap(m_special);\n\tm_screen->register_screen_bitmap(m_underbitmap);\n',),
    ('src/mame/sharp/x68k_v.cpp',
     'void x68k_mix_line(const x68k_mix &m, uint32_t *dest, const uint16_t *pcgrow, const uint16_t *specialrow, const uint16_t *gfxrow, int first, int last)\n{',
     'void x68k_mix_line(const x68k_mix &m, uint32_t *dest, const uint16_t *underrow, const uint16_t *pcgrow, const uint16_t *specialrow, const uint16_t *gfxrow, int first, int last)\n{',),
    ('src/mame/sharp/x68k_v.cpp',
     '\t\trgb_t outpix = dest[pixel], pix = 0;\n\t\tbool pcgpix = false, gfxpix = false;\n',
     '\t\trgb_t outpix = m.pcgpens[underrow[pixel] & 0xff], pix = 0;\n\t\tbool pcgpix = false, gfxpix = false;\n',),
    ('src/mame/sharp/x68k_v.cpp',
     'using x68k_mix_fn = void (*)(const x68k_mix &, uint32_t *, const uint16_t *, const uint16_t *, const uint16_t *, int, int);\n',
     'using x68k_mix_fn = void (*)(const x68k_mix &, uint32_t *, const uint16_t *, const uint16_t *, const uint16_t *, const uint16_t *, int, int);\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\t\tif(m_spritereg[0x404] & 0x0008)\n\t\t{\n\t\t\tclear = true;\n\t\t\tdraw_bg(m_pcgbitmap, screen, 1, true, pcgrect);\n\t\t}\n\t\telse if(m_spritereg[0x404] & 0x0001)\n\t\t{\n\t\t\tclear = true;\n\t\t\tdraw_bg(m_pcgbitmap, screen, 0, true, pcgrect);\n\t\t}\n\t}\n\tif(clear)\n\t{\n\t\tint divisor = 1;\n\t\tif(!(m_video.bg_hvres & 0x0c) && m_crtc->gfx_double_scan())\n\t\t\tdivisor = 2;\n\t\t// Kairo68: the rows and pens taken once a line (Kairo68#29).\n\t\tconst pen_t *const pens = m_pcgpalette->pens();\n\t\tfor(scanline=rect.min_y;scanline<=rect.max_y;scanline++)\n\t\t{\n\t\t\tconst uint16_t *const src = &m_pcgbitmap.pix(scanline / divisor, 0);\n\t\t\tuint32_t *const dst = &bitmap.pix(scanline, 0);\n\t\t\tfor(pixel=m_crtc->hbegin();pixel<=m_crtc->hend();pixel++)\n\t\t\t\tdst[pixel] = pens[src[pixel] & 0xff];\n\t\t}\n\t}\n\telse\n\t\tbitmap.fill(m_pcgpalette->pen(0), rect);\n\tKAIRO68_SCREEN_LAP(screen_under_ns);\n',
     '\t\tif(m_spritereg[0x404] & 0x0008)\n\t\t{\n\t\t\tclear = true;\n\t\t\tdraw_bg(m_underbitmap, screen, 1, true, pcgrect);\n\t\t}\n\t\telse if(m_spritereg[0x404] & 0x0001)\n\t\t{\n\t\t\tclear = true;\n\t\t\tdraw_bg(m_underbitmap, screen, 0, true, pcgrect);\n\t\t}\n\t}\n\t// Kairo68: the layer under everything is drawn to its own bitmap, and the mix starts each pixel\n\t// from its pen there (or from pen 0 without it) instead of reading back the screen bitmap this\n\t// pass filled with those pens (Kairo68#29).\n\tint under_divisor = 1;\n\tif(!(m_video.bg_hvres & 0x0c) && m_crtc->gfx_double_scan())\n\t\tunder_divisor = 2;\n\tstatic const uint16_t no_under[2048] = {};\n\tif(m_crtc->hend() >= int(std::size(no_under)))\n\t\tthrow emu_fatalerror("Kairo68: the screen is %d pixels wide", m_crtc->hend() + 1);\n\tKAIRO68_SCREEN_LAP(screen_under_ns);\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\t\tconst uint16_t *const gfxrow = &m_gfxbitmap.pix(scanline / gfxdivisor, 0);\n\t\tmixer(mix, dest, pcgrow, specialrow, gfxrow, m_crtc->hbegin(), m_crtc->hend());\n',
     '\t\tconst uint16_t *const gfxrow = &m_gfxbitmap.pix(scanline / gfxdivisor, 0);\n\t\tconst uint16_t *const underrow = clear ? &m_underbitmap.pix(scanline / under_divisor, 0) : no_under;\n\t\tmixer(mix, dest, underrow, pcgrow, specialrow, gfxrow, m_crtc->hbegin(), m_crtc->hend());\n',),
]

# In the 1024x1024 16-colour mode the four graphics pages are the quadrants of one picture, which
# the CPU addresses as y * 1024 + x; G-VRAM access and the renderer reach each word's page
# (Kairo68#31).
X68K_GVRAM_1024 = [
    ('src/mame/sharp/x68k_crtc.cpp',
     '\t\t\t\t}\n\t\t\t\tbreak;\n\t\t\tcase 0x0000:\n\t\t\t\tif (offset < 0x40000)\n\t\t\t\t{\n\t\t\t\t\tm_gvram_write_callback(offset, data & 0x000f, 0x000f);\n\t\t\t\t}\n',
     '\t\t\t\t}\n\t\t\t\tbreak;\n\t\t\tcase 0x0000:\n\t\t\t\tif (is_1024x1024())\n\t\t\t\t{\n\t\t\t\t\t// Kairo68 (x68k-gvram-1024): the 1024x1024 picture is the four pages as its quadrants.\n\t\t\t\t\tconst u8 shift = gvram_1024_page(offset) * 4;\n\t\t\t\t\tm_gvram_write_callback(gvram_1024_word(offset), (data & 0x000f) << shift, 0x000f << shift);\n\t\t\t\t}\n\t\t\t\telse if (offset < 0x40000)\n\t\t\t\t{\n\t\t\t\t\tm_gvram_write_callback(offset, data & 0x000f, 0x000f);\n\t\t\t\t}\n',),
    ('src/mame/sharp/x68k_crtc.cpp',
     '\t\t\t\tret = 0xffff;\n\t\t\tbreak;\n\t\tcase 0x0000:  // 16 colour (paletted) - 4 bits per word\n\t\t\tif (offset < 0x40000)\n\t\t\t\tret = m_gvram_read_callback(offset) & 0x000f;\n\t\t\telse if (offset >= 0x40000 && offset < 0x80000)\n\t\t\t\tret = (m_gvram_read_callback(offset - 0x40000) & 0x00f0) >> 4;\n',
     '\t\t\t\tret = 0xffff;\n\t\t\tbreak;\n\t\tcase 0x0000:  // 16 colour (paletted) - 4 bits per word\n\t\t\tif (is_1024x1024())\n\t\t\t{\n\t\t\t\t// Kairo68 (x68k-gvram-1024): the 1024x1024 picture is the four pages as its quadrants.\n\t\t\t\tconst u8 shift = gvram_1024_page(offset) * 4;\n\t\t\t\tret = (m_gvram_read_callback(gvram_1024_word(offset)) >> shift) & 0x000f;\n\t\t\t}\n\t\t\telse if (offset < 0x40000)\n\t\t\t\tret = m_gvram_read_callback(offset) & 0x000f;\n\t\t\telse if (offset >= 0x40000 && offset < 0x80000)\n\t\t\t\tret = (m_gvram_read_callback(offset - 0x40000) & 0x00f0) >> 4;\n',),
    ('src/mame/sharp/x68k_crtc.h',
     '\tu16 yscr_gfx(int page) const { return m_reg[13 + page * 2]; }\n\tu8 vfactor() const { return (m_reg[20] & 0x0c) >> 2; }\n\tbool is_1024x1024() const { return BIT(m_reg[20], 10); }\n\tbool gfx_double_scan() const { return (m_reg[20] & 0x1e) == 0x10; }\n\tbool gfx_layer_buffer() const { return BIT(m_reg[20], 11); }\n\tu8 gfx_color_mode() const { return (m_reg[20] >> 8) & 3; }\n',
     '\tu16 yscr_gfx(int page) const { return m_reg[13 + page * 2]; }\n\tu8 vfactor() const { return (m_reg[20] & 0x0c) >> 2; }\n\tbool is_1024x1024() const { return BIT(m_reg[20], 10); }\n\t// Kairo68 (x68k-gvram-1024): in the 1024x1024 16-colour mode, the pixel at word offset\n\t// y * 1024 + x of G-VRAM is in the page of its quadrant (0 top left, 1 top right, 2 bottom left,\n\t// 3 bottom right), at (y & 511) * 512 + (x & 511) of that page, as on the X68000.\n\tstatic u8 gvram_1024_page(offs_t offset) { return u8(BIT(offset, 19) << 1 | BIT(offset, 9)); }\n\tstatic offs_t gvram_1024_word(offs_t offset) { return ((offset >> 1) & 0x3fe00) | (offset & 0x1ff); }\n\tbool gfx_double_scan() const { return (m_reg[20] & 0x1e) == 0x10; }\n\tbool gfx_layer_buffer() const { return BIT(m_reg[20], 11); }\n\tu8 gfx_color_mode() const { return (m_reg[20] >> 8) & 3; }\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\t\t\t{\n\t\t\t\txscr = (m_crtc->xscr_gfx(scroll_line >= 0 ? scroll_line : scanline, 0) & 0x3ff);\n\t\t\t\tyscr = (m_crtc->yscr_gfx(scroll_line >= 0 ? scroll_line : scanline, 0) & 0x3ff);\n\t\t\t\tlineoffset = (((scanline - m_crtc->vbegin()) + yscr) & 0x3ff) * 1024;\n\t\t\t\tloc = xscr & 0x3ff;\n\t\t\t\tfor(pixel=m_crtc->hbegin();pixel<=m_crtc->hend();pixel++)\n\t\t\t\t{\n\t\t\t\t\tswitch(lineoffset & 0xc0000)\n\t\t\t\t\t{\n\t\t\t\t\tcase 0x00000:\n\t\t\t\t\t\tcolour = m_gvram[lineoffset + (loc & 0x3ff)] & 0x000f;\n\t\t\t\t\t\tbreak;\n\t\t\t\t\tcase 0x40000:\n\t\t\t\t\t\tcolour = (m_gvram[(lineoffset - 0x40000) + (loc & 0x3ff)] & 0x00f0) >> 4;\n\t\t\t\t\t\tbreak;\n\t\t\t\t\tcase 0x80000:\n\t\t\t\t\t\tcolour = (m_gvram[(lineoffset - 0x80000) + (loc & 0x3ff)] & 0x0f00) >> 8;\n\t\t\t\t\t\tbreak;\n\t\t\t\t\tcase 0xc0000:\n\t\t\t\t\t\tcolour = (m_gvram[(lineoffset - 0xc0000) + (loc & 0x3ff)] & 0xf000) >> 12;\n\t\t\t\t\t\tbreak;\n\t\t\t\t\t}\n\t\t\t\t\tif(colour || (priority == 3))\n\t\t\t\t\t{\n\t\t\t\t\t\tif(((m_video.reg[2] & 0x1800) == 0x1000) && (colour & 1))\n',
     "\t\t\t{\n\t\t\t\txscr = (m_crtc->xscr_gfx(scroll_line >= 0 ? scroll_line : scanline, 0) & 0x3ff);\n\t\t\t\tyscr = (m_crtc->yscr_gfx(scroll_line >= 0 ? scroll_line : scanline, 0) & 0x3ff);\n\t\t\t\t// Kairo68 (x68k-gvram-1024): the 1024x1024 picture is the four pages as its quadrants;\n\t\t\t\t// the row is (y & 511) of the two pages beside each other at the line's half.\n\t\t\t\tconst uint32_t y = ((scanline - m_crtc->vbegin()) + yscr) & 0x3ff;\n\t\t\t\tlineoffset = (y & 0x1ff) * 512;\n\t\t\t\tconst int ypage = (y >> 9) << 1;\n\t\t\t\tloc = xscr & 0x3ff;\n\t\t\t\tif((m_video.reg[2] & 0x1800) != 0x1000)\n\t\t\t\t{\n\t\t\t\t\t// Kairo68 (x68k-gvram-1024): without the special priority, a pixel whose colour\n\t\t\t\t\t// shows (any, on the bottom page) stores it and clears its special pixel. The same\n\t\t\t\t\t// stores in runs within one page's half of the row, a form the compiler vectorizes,\n\t\t\t\t\t// as the 512x512 16-colour page draws (Kairo68#30).\n\t\t\t\t\tconst uint16_t *const row = &m_gvram[lineoffset];\n\t\t\t\t\tuint16_t *const dest = &bitmap.pix(scanline, 0);\n\t\t\t\t\tuint16_t *const spec = &m_special.pix(scanline, 0);\n\t\t\t\t\tconst int last = m_crtc->hend();\n\t\t\t\t\tuint32_t at = loc;\n\t\t\t\t\tfor(pixel=m_crtc->hbegin();pixel<=last;)\n\t\t\t\t\t{\n\t\t\t\t\t\tconst int run = std::min<int>(last - pixel + 1, 0x200 - (at & 0x1ff));\n\t\t\t\t\t\tconst int plane = (ypage | (at >> 9)) * 4;\n\t\t\t\t\t\tconst uint16_t *__restrict const src = row + (at & 0x1ff);\n\t\t\t\t\t\tuint16_t *__restrict const d = dest + pixel;\n\t\t\t\t\t\tuint16_t *__restrict const sp = spec + pixel;\n\t\t\t\t\t\tif(priority == 3)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tfor(int i = 0; i < run; i++)\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\td[i] = (src[i] >> plane) & 0x000f;\n\t\t\t\t\t\t\t\tsp[i] = 0;\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t}\n\t\t\t\t\t\telse\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tfor(int i = 0; i < run; i++)\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t// keep is all ones where the colour is 0: the pixel and its special pixel stay.\n\t\t\t\t\t\t\t\tconst uint16_t c = (src[i] >> plane) & 0x000f;\n\t\t\t\t\t\t\t\tconst uint16_t keep = uint16_t(-uint16_t(c == 0));\n\t\t\t\t\t\t\t\td[i] = (d[i] & keep) | c;\n\t\t\t\t\t\t\t\tsp[i] &= keep;\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t}\n\t\t\t\t\t\tpixel += run;\n\t\t\t\t\t\tat = (at + run) & 0x3ff;\n\t\t\t\t\t}\n\t\t\t\t\tret = false;\n\t\t\t\t\tcontinue;\n\t\t\t\t}\n\t\t\t\tfor(pixel=m_crtc->hbegin();pixel<=m_crtc->hend();pixel++)\n\t\t\t\t{\n\t\t\t\t\tcolour = (m_gvram[lineoffset + (loc & 0x1ff)] >> ((ypage | (loc >> 9)) * 4)) & 0x000f;\n\t\t\t\t\tif(colour || (priority == 3))\n\t\t\t\t\t{\n\t\t\t\t\t\tif(((m_video.reg[2] & 0x1800) == 0x1000) && (colour & 1))\n",),
]

# A write to R9 or R21 to R23, which nothing the picture shows reads, draws no waiting lines
# (Kairo68#30).
X68K_CRTC_UNSEEN_REGISTERS = [
    ('src/mame/sharp/x68k_crtc.cpp',
     '\nvoid x68k_crtc_device::crtc_w(offs_t offset, u16 data, u16 mem_mask)\n{\n\t// Kairo68: R10 to R19 only move the layers.\n\tif (offset >= 10 && offset <= 19 && m_scroll_can_wait && m_scroll_can_wait())\n\t\trecord_scroll();\n\telse\n\t\tflush_partial();\n',
     "\nvoid x68k_crtc_device::crtc_w(offs_t offset, u16 data, u16 mem_mask)\n{\n\t// Kairo68 (x68k-crtc-unseen-registers): R9 (the raster interrupt's line) and R21 to R23 (the\n\t// text access masks and the copy and clear setup) are read only when the CPU writes VRAM or\n\t// starts an operation, which draw the lines the beam has passed first themselves; nothing the\n\t// picture shows changes, so a write to them draws nothing. A game whose raster interrupt\n\t// rewrites them on every line (Carat) drew each line on its own (Kairo68#30).\n\tif (offset == 9 || (offset >= 21 && offset <= 23))\n\t\t;\n\t// Kairo68: R10 to R19 only move the layers.\n\telse if (offset >= 10 && offset <= 19 && m_scroll_can_wait && m_scroll_can_wait())\n\t\trecord_scroll();\n\telse\n\t\tflush_partial();\n",),
]

# A frame with nothing the picture shows changing, after one drawn whole the same way, is
# reported unchanged and the screen keeps its picture (Kairo68#30).
X68K_UNCHANGED_FRAME = [
    ('src/mame/sharp/x68k.h',
     '\tbitmap_ind16 m_gfxbitmap;\n\tbitmap_ind16 m_special;\n\tbitmap_ind16 m_underbitmap; // Kairo68: the BG layer drawn under everything (screen_update)\n\n\tvoid floppy_load_unload(bool load, floppy_image_device *dev);\n\tvoid floppy_load(floppy_image_device *dev);\n',
     "\tbitmap_ind16 m_gfxbitmap;\n\tbitmap_ind16 m_special;\n\tbitmap_ind16 m_underbitmap; // Kairo68: the BG layer drawn under everything (screen_update)\n\t// Kairo68 (x68k-unchanged-frame): the frame screen_update last saw, whether it left that\n\t// frame's lines so far undrawn, and the CRTC's change count of the last frame drawn whole\n\t// without a change during it (0 for none): the picture the screen keeps.\n\tuint64_t m_update_frame = ~uint64_t(0);\n\tbool m_frame_skipped = false;\n\tuint64_t m_drawn_generation = 0;\n\n\tvoid floppy_load_unload(bool load, floppy_image_device *dev);\n\tvoid floppy_load(floppy_image_device *dev);\n",),
    ('src/mame/sharp/x68k_crtc.cpp',
     'void x68k_crtc_device::device_post_load()\n{\n\tforget_scroll();\n}\n\nvoid x68k_crtc_device::forget_scroll()\n',
     'void x68k_crtc_device::device_post_load()\n{\n\tforget_scroll();\n\tm_picture_generation++;\n}\n\nvoid x68k_crtc_device::forget_scroll()\n',),
    ('src/mame/sharp/x68k_crtc.cpp',
     '// lines and forgets the records, so the records are always the newest state of those lines.\nvoid x68k_crtc_device::record_scroll()\n{\n\tif (m_partial_line < 0)\n\t\treturn;\n\tconst int line = m_partial_line;\n',
     '// lines and forgets the records, so the records are always the newest state of those lines.\nvoid x68k_crtc_device::record_scroll()\n{\n\tm_picture_generation++;\n\tif (m_partial_line < 0)\n\t\treturn;\n\tconst int line = m_partial_line;\n',),
    ('src/mame/sharp/x68k_crtc.cpp',
     '\t\tm_scroll_last = line;\n}\n\nvoid x68k_crtc_device::text_copy(unsigned src, unsigned dest, u8 planes)\n{\n\t// copys one raster in T-VRAM to another raster\n',
     '\t\tm_scroll_last = line;\n}\n\n// Kairo68 (x68k-unchanged-frame): whether text_copy would write any word the destination raster\n// does not already hold.\nbool x68k_crtc_device::text_copy_changes(unsigned src, unsigned dest, u8 planes)\n{\n\tconst offs_t src_ram = src * 256, dest_ram = dest * 256;\n\tfor (u8 plane = 0; plane <= 3; plane++)\n\t\tif (BIT(planes, plane))\n\t\t\tfor (offs_t word = 0; word < 256; word++)\n\t\t\t\tif (m_tvram_read_callback(src_ram + word + 0x10000 * plane, 0xffff) !=\n\t\t\t\t\t\tm_tvram_read_callback(dest_ram + word + 0x10000 * plane, 0xffff))\n\t\t\t\t\treturn true;\n\treturn false;\n}\n\nvoid x68k_crtc_device::text_copy(unsigned src, unsigned dest, u8 planes)\n{\n\t// copys one raster in T-VRAM to another raster\n',),
    ('src/mame/sharp/x68k_crtc.cpp',
     '\n\tif (m_operation & 8)\n\t{\n\t\tflush_partial();\n\t\ttext_copy((m_reg[22] & 0xff00) >> 8, (m_reg[22] & 0x00ff), (m_reg[21] & 0xf));\n\t}\n\n\tint scan = screen().vpos();\n',
     "\n\tif (m_operation & 8)\n\t{\n\t\t// Kairo68 (x68k-unchanged-frame): a copy onto rows that already hold the same words changes\n\t\t// nothing; a game that copies every line (Carat's raster effect) would otherwise change its\n\t\t// still picture every frame (Kairo68#30).\n\t\tconst unsigned src = (m_reg[22] & 0xff00) >> 8, dest = m_reg[22] & 0x00ff;\n\t\tconst u8 planes = m_reg[21] & 0xf;\n\t\tif (text_copy_changes(src, dest, planes))\n\t\t{\n\t\t\tflush_partial();\n\t\t\ttext_copy(src, dest, planes);\n\t\t}\n\t}\n\n\tint scan = screen().vpos();\n",),
    ('src/mame/sharp/x68k_crtc.cpp',
     '\t{\n\t\t// Kairo68: a new frame; the records were for the frame that ended, which drew their lines.\n\t\tif (scan == 0)\n\t\t\tforget_scroll();\n\t\thsync_time = screen().time_until_pos(scan, m_hend);\n\t\tm_scanline_timer->adjust(hsync_time);\n\t\tif ((scan != 0) && (scan < m_vend))\n',
     '\t{\n\t\t// Kairo68: a new frame; the records were for the frame that ended, which drew their lines.\n\t\tif (scan == 0)\n\t\t{\n\t\t\tforget_scroll();\n\t\t\tm_frame_start_generation = m_picture_generation;\n\t\t}\n\t\thsync_time = screen().time_until_pos(scan, m_hend);\n\t\tm_scanline_timer->adjust(hsync_time);\n\t\tif ((scan != 0) && (scan < m_vend))\n',),
    ('src/mame/sharp/x68k_crtc.cpp',
     '\tif (scan <= m_vtotal)\n\t{\n\t\tm_rint_callback(0);\n\t\tflush_partial();\n\t\tscreen().update_partial(scan - 1);\n\t\tirq_time = screen().time_until_pos(scan, m_hbegin);\n\t\t// end of HBlank period clears GPIP6 also?\n',
     '\tif (scan <= m_vtotal)\n\t{\n\t\tm_rint_callback(0);\n\t\tdraw_passed_lines();\n\t\tscreen().update_partial(scan - 1);\n\t\tirq_time = screen().time_until_pos(scan, m_hbegin);\n\t\t// end of HBlank period clears GPIP6 also?\n',),
    ('src/mame/sharp/x68k_crtc.cpp',
     ' *    operation is completed.\n */\nvoid x68k_crtc_device::flush_partial()\n{\n\tif (m_partial_line >= 0)\n\t{\n',
     " *    operation is completed.\n */\nvoid x68k_crtc_device::flush_partial()\n{\n\tm_picture_generation++;\n\tdraw_passed_lines();\n}\n\n// Kairo68 (x68k-unchanged-frame): flush_partial's drawing, for a caller that changes nothing the\n// picture shows (the raster interrupt draws the lines above it).\nvoid x68k_crtc_device::draw_passed_lines()\n{\n\tif (m_partial_line >= 0)\n\t{\n",),
    ('src/mame/sharp/x68k_crtc.h',
     '\t// Kairo68: draws the lines the beam has passed and are not drawn yet; call before anything the\n\t// picture shows changes (tools/patches.py X68K_DEFERRED_PARTIAL).\n\tvoid flush_partial();\n\t// Kairo68: the scroll registers (R10 to R19) a line is drawn with: those in effect when the beam\n\t// passed it, recorded by a scroll register write that leaves the line waiting, or the current\n\t// ones (tools/patches.py X68K_LINE_SCROLL).\n',
     '\t// Kairo68: draws the lines the beam has passed and are not drawn yet; call before anything the\n\t// picture shows changes (tools/patches.py X68K_DEFERRED_PARTIAL).\n\tvoid flush_partial();\n\tvoid draw_passed_lines();  // Kairo68 (x68k-unchanged-frame)\n\t// Kairo68: the scroll registers (R10 to R19) a line is drawn with: those in effect when the beam\n\t// passed it, recorded by a scroll register write that leaves the line waiting, or the current\n\t// ones (tools/patches.py X68K_LINE_SCROLL).\n',),
    ('src/mame/sharp/x68k_crtc.h',
     '\t// recorded; without one, every write draws them first.\n\tvoid set_scroll_can_wait(std::function<bool ()> test) { m_scroll_can_wait = std::move(test); }\n\tconst u16 *scroll_regs(int line) const { return (line >= m_scroll_first && line <= m_scroll_last) ? m_line_scroll[line] : &m_reg[10]; }\n\n\t// getters\n\tu16 xscr_text() const { return m_reg[10]; }\n',
     '\t// recorded; without one, every write draws them first.\n\tvoid set_scroll_can_wait(std::function<bool ()> test) { m_scroll_can_wait = std::move(test); }\n\tconst u16 *scroll_regs(int line) const { return (line >= m_scroll_first && line <= m_scroll_last) ? m_line_scroll[line] : &m_reg[10]; }\n\t// Kairo68 (x68k-unchanged-frame): a count of the changes to anything the picture shows\n\t// (flush_partial and record_scroll run before each), the count when the frame began (line 0),\n\t// and whether the picture is interlaced, whose fields alternate by themselves.\n\tuint64_t picture_generation() const { return m_picture_generation; }\n\tuint64_t frame_start_generation() const { return m_frame_start_generation; }\n\tbool interlaced() const { return m_interlace; }\n\n\t// getters\n\tu16 xscr_text() const { return m_reg[10]; }\n',),
    ('src/mame/sharp/x68k_crtc.h',
     'private:\n\t// internal helpers\n\tvoid text_copy(unsigned src, unsigned dest, u8 planes);\n\tTIMER_CALLBACK_MEMBER(operation_end);\n\tvoid refresh_mode();\n\tTIMER_CALLBACK_MEMBER(hsync);\n',
     'private:\n\t// internal helpers\n\tvoid text_copy(unsigned src, unsigned dest, u8 planes);\n\tbool text_copy_changes(unsigned src, unsigned dest, u8 planes);  // Kairo68 (x68k-unchanged-frame)\n\tTIMER_CALLBACK_MEMBER(operation_end);\n\tvoid refresh_mode();\n\tTIMER_CALLBACK_MEMBER(hsync);\n',),
    ('src/mame/sharp/x68k_crtc.h',
     '\tint m_scroll_first = 0;\n\tint m_scroll_last = -1;\n\tint m_drawn_through = -1;\n\tstd::function<bool ()> m_scroll_can_wait;\n\temu_timer *m_raster_irq_timer;\n\temu_timer *m_vblank_irq_timer;\n',
     '\tint m_scroll_first = 0;\n\tint m_scroll_last = -1;\n\tint m_drawn_through = -1;\n\tuint64_t m_picture_generation = 1, m_frame_start_generation = 0;  // Kairo68 (x68k-unchanged-frame)\n\tstd::function<bool ()> m_scroll_can_wait;\n\temu_timer *m_raster_irq_timer;\n\temu_timer *m_vblank_irq_timer;\n',),
    ('src/mame/sharp/x68k_v.cpp',
     '\n} // anonymous namespace\n\nuint32_t x68k_state::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)\n{\n\tKAIRO68_PERF_COUNT(screen_updates, 1);\n\tKAIRO68_PERF_COUNT(screen_lines, cliprect.height());\n\tKAIRO68_PERF_TIME(screen_ns);\n',
     "\n} // anonymous namespace\n\nuint32_t x68k_state::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect_in)\n{\n\t// Kairo68 (x68k-unchanged-frame): a frame drawn whole with nothing the picture shows changing\n\t// during it is the picture of every frame after it until something changes. Meanwhile the\n\t// screen keeps showing it (UPDATE_HAS_NOT_CHANGED: it swaps neither bitmap) and nothing is\n\t// drawn. A change later in a frame draws its lines from the frame's top, which are what they\n\t// were up to the change. An interlaced picture is drawn every frame (Kairo68#30).\n\tconst rectangle &visible = screen.visible_area();\n\tconst uint64_t generation = m_crtc->picture_generation();\n\tif (screen.frame_number() != m_update_frame)\n\t{\n\t\tm_update_frame = screen.frame_number();\n\t\tm_frame_skipped = false;\n\t}\n\tif (!m_crtc->interlaced() && generation == m_drawn_generation)\n\t{\n\t\tm_frame_skipped = true;\n\t\treturn UPDATE_HAS_NOT_CHANGED;\n\t}\n\trectangle cliprect = cliprect_in;\n\tif (m_frame_skipped)\n\t{\n\t\tcliprect.min_y = visible.min_y;\n\t\tm_frame_skipped = false;\n\t}\n\tif (cliprect.max_y >= visible.max_y)\n\t\tm_drawn_generation = m_crtc->frame_start_generation() == generation ? generation : 0;\n\tKAIRO68_PERF_COUNT(screen_updates, 1);\n\tKAIRO68_PERF_COUNT(screen_lines, cliprect.height());\n\tKAIRO68_PERF_TIME(screen_ns);\n",),
]

# Whether the SASI controller is in a command, which ends the boot's fast-forward once a hard disk
# answers (core/x68k_machine.cpp). It was made in the snapshot directly with Kairo68 1eacf55.
X68K_SASI_BUSY = [
    ('src/mame/sharp/x68k_hdc.h',
     '''	// Kairo68: DMA request, REQ during a data phase
	auto drq_callback() { return m_drq_cb.bind(); }
''',
     '''	// Kairo68: DMA request, REQ during a data phase
	auto drq_callback() { return m_drq_cb.bind(); }
	// Kairo68: a command in progress, which ends the boot's fast-forward
	bool busy() const { return m_phase != sasi_phase::BUSFREE; }
'''),
]

# Auto touch mode (Kairo#78) hears how a program reads its input from its IOCS calls (trap #15, the
# call in D0): the ROM polls the mouse on its own, used or not, so the mouse's polls say nothing.
# The CPU reports each TRAP to a hook (X68kMachine::guest_input).
# Typed startup commands (Kairo68#70) wait for Human68k's command prompt, which reads a line through a
# DOS call: an F-line instruction ($FFxx), which the CPU reports to a hook (X68kMachine::prompt).
M68000_LINEF_HOOK = [
    ('src/devices/cpu/m68000/m68000.h',
     '\tvoid enable_idle_skip(offs_t free_end);\n',
     '\tvoid enable_idle_skip(offs_t free_end);\n\n'
     '\t// Kairo68: called on each line-F instruction (a Human68k DOS call) with its opcode, before the exception.\n'
     '\tvoid set_linef_hook(std::function<void (u16)> hook) { m_linef_hook = std::move(hook); }\n'),
    ('src/devices/cpu/m68000/m68000.h',
     '\tbool m_idle_enabled = false;\n',
     '\tstd::function<void (u16)> m_linef_hook; // Kairo68: set_linef_hook\n\tbool m_idle_enabled = false;\n'),
    ('src/devices/cpu/m68000/m68000-sdf.cpp',
     'void m68000_device::state_linef_df()\n{\n\t// 1c0 trac1\n',
     'void m68000_device::state_linef_df()\n{\n\tif(m_linef_hook) m_linef_hook(m_ird); // Kairo68: set_linef_hook\n\t// 1c0 trac1\n'),
    ('src/devices/cpu/m68000/m68000-sif.cpp',
     'void m68000_device::state_linef_if()\n{\n\t// 1c0 trac1\n',
     'void m68000_device::state_linef_if()\n{\n\tif(m_linef_hook) m_linef_hook(m_ird); // Kairo68: set_linef_hook\n\t// 1c0 trac1\n'),
    ('src/devices/cpu/m68000/m68000-sdp.cpp',
     'void m68000_device::state_linef_dp()\n{\n\tswitch(m_inst_substate) {\n\tcase 0:\n\t// 1c0 trac1\n',
     'void m68000_device::state_linef_dp()\n{\n\tswitch(m_inst_substate) {\n\tcase 0:\n\tif(m_linef_hook) m_linef_hook(m_ird); // Kairo68: set_linef_hook\n\t// 1c0 trac1\n'),
    ('src/devices/cpu/m68000/m68000-sip.cpp',
     'void m68000_device::state_linef_ip()\n{\n\tswitch(m_inst_substate) {\n\tcase 0:\n\t// 1c0 trac1\n',
     'void m68000_device::state_linef_ip()\n{\n\tswitch(m_inst_substate) {\n\tcase 0:\n\tif(m_linef_hook) m_linef_hook(m_ird); // Kairo68: set_linef_hook\n\t// 1c0 trac1\n'),
]

M68000_TRAP_HOOK = [
    ('src/devices/cpu/m68000/m68000.h',
     '\tvoid enable_direct_memory();\n',
     '\tvoid enable_direct_memory();\n\n'
     '\t// Kairo68: called on each TRAP instruction with its number and D0, before the exception.\n'
     '\tvoid set_trap_hook(std::function<void (int, u32)> hook) { m_trap_hook = std::move(hook); }\n'),
    ('src/devices/cpu/m68000/m68000.h',
     '\t// Kairo68: the idle-loop skip (enable_idle_skip, idle_check).\n',
     '\tstd::function<void (int, u32)> m_trap_hook; // Kairo68: set_trap_hook\n\n'
     '\t// Kairo68: the idle-loop skip (enable_idle_skip, idle_check).\n'),
    ('src/devices/cpu/m68000/m68000-sdf.cpp',
     'void m68000_device::trap_imm4_df() // 4e40 fff0\n{\n\t// 1d0 trap1\n',
     'void m68000_device::trap_imm4_df() // 4e40 fff0\n{\n\tif(m_trap_hook) m_trap_hook(m_ird & 15, m_da[0]); // Kairo68: set_trap_hook\n\t// 1d0 trap1\n'),
    ('src/devices/cpu/m68000/m68000-sif.cpp',
     'void m68000_device::trap_imm4_if() // 4e40 fff0\n{\n\t// 1d0 trap1\n',
     'void m68000_device::trap_imm4_if() // 4e40 fff0\n{\n\tif(m_trap_hook) m_trap_hook(m_ird & 15, m_da[0]); // Kairo68: set_trap_hook\n\t// 1d0 trap1\n'),
    ('src/devices/cpu/m68000/m68000-sdp.cpp',
     'void m68000_device::trap_imm4_dp() // 4e40 fff0\n{\n\tswitch(m_inst_substate) {\n\tcase 0:\n',
     'void m68000_device::trap_imm4_dp() // 4e40 fff0\n{\n\tswitch(m_inst_substate) {\n\tcase 0:\n\tif(m_trap_hook) m_trap_hook(m_ird & 15, m_da[0]); // Kairo68: set_trap_hook\n'),
    ('src/devices/cpu/m68000/m68000-sip.cpp',
     'void m68000_device::trap_imm4_ip() // 4e40 fff0\n{\n\tswitch(m_inst_substate) {\n\tcase 0:\n',
     'void m68000_device::trap_imm4_ip() // 4e40 fff0\n{\n\tswitch(m_inst_substate) {\n\tcase 0:\n\tif(m_trap_hook) m_trap_hook(m_ird & 15, m_da[0]); // Kairo68: set_trap_hook\n'),
]

# Kairo68#33: the split check draws a frame of the machine's state whole and in pieces
# (X68kMachine::split_check), through screen_update without the frame's own bookkeeping.
X68K_SPLIT_CHECK = [
    ('src/mame/sharp/x68k.h', '\tuint32_t screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);\n', "\tuint32_t screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);\npublic:\n\t// Kairo68#33: draws cliprect of the machine's current state into bitmap as screen_update does,\n\t// whatever this frame drew before, and leaves the frame's drawing as it was: for the check that\n\t// a frame drawn whole and drawn in pieces gives the same picture.\n\tvoid kairo68_draw(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)\n\t{\n\t\tconst uint64_t frame = m_update_frame, drawn = m_drawn_generation;\n\t\tconst bool skipped = m_frame_skipped;\n\t\tm_update_frame = screen.frame_number();\n\t\tm_frame_skipped = false;\n\t\tm_drawn_generation = 0;\n\t\tscreen_update(screen, bitmap, cliprect);\n\t\tm_update_frame = frame;\n\t\tm_frame_skipped = skipped;\n\t\tm_drawn_generation = drawn;\n\t}\nprotected:\n"),
]

PATCHES = {'x68k-save-state': X68K_SAVE_STATE, 'x68k-sasi-count': X68K_SASI_COUNT, 'x68k-sasi-dma': X68K_SASI_DMA,
           'x68k-sasi-busy': X68K_SASI_BUSY,
           'osdlib-without-sdl': OSDLIB_WITHOUT_SDL, 'upd765-save-state': UPD765_SAVE_STATE, 'rtc-save-state': RTC_SAVE_STATE,
           'floppy-save-state': FLOPPY_SAVE_STATE, 'hd63450-save-state': HD63450_SAVE_STATE,
           'perf-counters': PERF_COUNTERS,
           'mfp-timer-elapse': MFP_TIMER_ELAPSE,
           'dma-drq-poll': DMA_DRQ_POLL,
           'x68k-deferred-partial': X68K_DEFERRED_PARTIAL,
           'screen-color-getters': SCREEN_COLOR_GETTERS,
           'perf-screen-stages': PERF_SCREEN_STAGES,
           'x68k-text-line': X68K_TEXT_LINE,
           'x68k-mix-hoist': X68K_MIX_HOIST,
           'm68000-idle-skip': M68000_IDLE_SKIP,
           'perf-fm-stage': PERF_FM_STAGE,
           'ymfm-synthesis-worker': YMFM_SYNTHESIS_WORKER,
           'resampler-inline-reader': RESAMPLER_INLINE_READER,
           'm68000-direct-memory': M68000_DIRECT_MEMORY,
           'mfp-usart-clock-replay': MFP_USART_CLOCK_REPLAY,
           'x68k-gfx-hoist': X68K_GFX_HOIST,
           'floppy-next-cell': FLOPPY_NEXT_CELL,
           'x68k-line-scroll': X68K_LINE_SCROLL,
           'm68000-ftu-inline': M68000_FTU_INLINE,
           'x68k-under-row': X68K_UNDER_ROW,
           'x68k-gvram-1024': X68K_GVRAM_1024,
           'x68k-crtc-unseen-registers': X68K_CRTC_UNSEEN_REGISTERS,
           'x68k-unchanged-frame': X68K_UNCHANGED_FRAME,
           'm68000-trap-hook': M68000_TRAP_HOOK,
           'm68000-linef-hook': M68000_LINEF_HOOK,
           'x68k-split-check': X68K_SPLIT_CHECK
}
