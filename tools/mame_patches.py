"""Kairo68's local changes to the imported MAME sources, applied by tools/import_mame.py after it
copies the subset. Each change replaces text that must occur exactly once in its file, so a MAME
update that moves the code stops the import instead of applying the change in the wrong place.
docs/source-import.md describes each change."""
from pathlib import Path

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
	// Kairo68: delay mode fires only where the count elapses (mame_patches.py MFP_TIMER_ELAPSE).
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
	// picture shows changes (mame_patches.py X68K_DEFERRED_PARTIAL).
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

PATCHES = {'x68k-save-state': X68K_SAVE_STATE, 'x68k-sasi-count': X68K_SASI_COUNT, 'x68k-sasi-dma': X68K_SASI_DMA,
           'osdlib-without-sdl': OSDLIB_WITHOUT_SDL, 'upd765-save-state': UPD765_SAVE_STATE, 'rtc-save-state': RTC_SAVE_STATE,
           'floppy-save-state': FLOPPY_SAVE_STATE, 'hd63450-save-state': HD63450_SAVE_STATE,
           'perf-counters': PERF_COUNTERS,
           'mfp-timer-elapse': MFP_TIMER_ELAPSE,
           'dma-drq-poll': DMA_DRQ_POLL,
           'x68k-deferred-partial': X68K_DEFERRED_PARTIAL,
           'screen-color-getters': SCREEN_COLOR_GETTERS,
           'perf-screen-stages': PERF_SCREEN_STAGES,
           'x68k-text-line': X68K_TEXT_LINE}


def apply(root):
    """Applies every change to the MAME tree at root."""
    for name, changes in PATCHES.items():
        for rel, old, new in changes:
            path = Path(root) / rel
            # MAME's sources have LF line endings; keep the file's bytes as they are.
            with open(path, encoding='utf-8', newline='') as f:
                text = f.read()
            if text.count(old) != 1:
                raise SystemExit(f'patch {name}: {rel} does not contain the text to replace exactly once')
            with open(path, 'w', encoding='utf-8', newline='') as f:
                f.write(text.replace(old, new))
