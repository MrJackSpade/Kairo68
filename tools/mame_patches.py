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

// Kairo68: the PCG and BG RAM came from a saved state, so redecode every pattern and redraw both
// BG layers.
void x68k_state::device_post_load()
{
	std::fill(std::begin(m_video.tile8_dirty), std::end(m_video.tile8_dirty), 1);
	std::fill(std::begin(m_video.tile16_dirty), std::end(m_video.tile16_dirty), 1);
	m_bg0_8->mark_all_dirty();
	m_bg1_8->mark_all_dirty();
	m_bg0_16->mark_all_dirty();
	m_bg1_16->mark_all_dirty();
}''',),
]

PATCHES = {'x68k-save-state': X68K_SAVE_STATE}


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
