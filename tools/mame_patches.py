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
# and their drives.
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

PATCHES = {'x68k-save-state': X68K_SAVE_STATE, 'upd765-save-state': UPD765_SAVE_STATE, 'rtc-save-state': RTC_SAVE_STATE,
           'floppy-save-state': FLOPPY_SAVE_STATE, 'hd63450-save-state': HD63450_SAVE_STATE}


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
