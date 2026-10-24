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
     '\t// read access: update the streams before performing the read\n\tvirtual u8 read(offs_t offset) override { return update_streams().read(offset); }\n\tvirtual u8 status_r() override { return update_streams().read_status(); }\n\n\t// write access: update the strams before performing the write\n\t// Kairo68: and hand each write to the synthesis worker\'s chip (synthesis_event)\n\tvirtual void write(offs_t offset, u8 data) override { m_checked_now = false; update_streams().write(offset, data); synthesis_event(SYNTH_WRITE, data, offset); }\n\tvirtual void address_w(u8 data) override { m_checked_now = false; update_streams().write_address(data); synthesis_event(SYNTH_ADDRESS, data); }\n\tvirtual void data_w(u8 data) override { m_checked_now = false; update_streams().write_data(data); synthesis_event(SYNTH_DATA, data); }\n\n\t// Kairo68: synthesize on a worker thread, in parallel with the emulation (Kairo68#22). The\n\t// guest reads the chip\'s status, timers, IRQ and ports, which stay with m_chip on the emulation\n\t// thread; m_chip no longer generates, and only counts the clocks it would have. A second chip\n\t// on the worker is given the same calls in the same order, generates every sample at the\n\t// position the stream asked for it, and the samples are written into the stream before\n\t// anything reads it. Output is identical to synthesizing here.\n\tvoid enable_synthesis_worker()\n\t{\n\t\tstatic_assert(requires(ChipClass &chip) { chip.advance_generated(1u); }, "the chip cannot count clocks it does not generate");\n\t\tif (m_synth_worker)\n\t\t\treturn;\n\t\tm_synth_intf = std::make_unique<synthesis_interface>();\n\t\tm_synth_chip = std::make_unique<ChipClass>(*m_synth_intf);\n\t\t// The worker\'s chip starts as m_chip is now.\n\t\tstd::vector<uint8_t> blob;\n\t\tymfm::ymfm_saved_state save(blob, true);\n\t\tm_chip.save_restore(save);\n\t\tymfm::ymfm_saved_state load(blob, false);\n\t\tm_synth_chip->save_restore(load);\n\t\tm_synth_worker = std::make_unique<kairo::OrderedWorker<synthesis_batch, synthesis_results, 16>>(\n\t\t\t\t[this](const synthesis_batch &batch) {\n\t\t\t\t\tsynthesis_results results;\n\t\t\t\t\tfor (const synthesis_command &command : batch)\n\t\t\t\t\t\tif (command.op == SYNTH_GENERATE)\n\t\t\t\t\t\t\tresults.push_back(synthesize(command));\n\t\t\t\t\t\telse\n\t\t\t\t\t\t\tsynthesize(command);\n\t\t\t\t\treturn results;\n\t\t\t\t});\n\t\tm_stream->set_deferred_finish([this] { finish_synthesis(); });\n\t}\n',),
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

PATCHES = {'x68k-save-state': X68K_SAVE_STATE, 'x68k-sasi-count': X68K_SASI_COUNT, 'x68k-sasi-dma': X68K_SASI_DMA,
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
           'm68000-direct-memory': M68000_DIRECT_MEMORY}


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
