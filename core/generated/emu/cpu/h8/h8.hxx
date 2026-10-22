void h8_device::nop_full()
{
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::nop_partial()
{
switch(m_inst_substate) {
case 0:
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::sleep_full()
{
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	if(m_standby_pending) {
		m_standby_time = total_cycles();
		notify_standby(1);
		suspend(SUSPEND_REASON_CLOCK, true);
		m_standby_cb(1);
	} else {
		while(!m_irq_vector) {
	if(m_icount) { m_icount = m_bcount; } m_inst_substate = 3; return;
		}
		prefetch_done();
	}
}

void h8_device::sleep_partial()
{
switch(m_inst_substate) {
case 0:
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	if(m_standby_pending) {
		m_standby_time = total_cycles();
		notify_standby(1);
		suspend(SUSPEND_REASON_CLOCK, true);
		m_standby_cb(1);
	} else {
		while(!m_irq_vector) {
	if(m_icount) { m_icount = m_bcount; } m_inst_substate = 3; return;
case 3:;
		}
		prefetch_done();
	}
	break;
}
	m_inst_substate = 0;
}

void h8_device::stc_ccr_r8l_full()
{
	r8_w(m_IR[0], m_CCR);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::stc_ccr_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], m_CCR);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::ldc_r8l_ccr_full()
{
	m_CCR = r8_r(m_IR[0]);
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done_noirq();
}

void h8_device::ldc_r8l_ccr_partial()
{
switch(m_inst_substate) {
case 0:
	m_CCR = r8_r(m_IR[0]);
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done_noirq();
	break;
}
	m_inst_substate = 0;
}

void h8_device::orc_imm8_ccr_full()
{
	m_CCR |= m_IR[0];
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done_noirq();
}

void h8_device::orc_imm8_ccr_partial()
{
switch(m_inst_substate) {
case 0:
	m_CCR |= m_IR[0];
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done_noirq();
	break;
}
	m_inst_substate = 0;
}

void h8_device::xorc_imm8_ccr_full()
{
	m_CCR ^= m_IR[0];
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done_noirq();
}

void h8_device::xorc_imm8_ccr_partial()
{
switch(m_inst_substate) {
case 0:
	m_CCR ^= m_IR[0];
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done_noirq();
	break;
}
	m_inst_substate = 0;
}

void h8_device::andc_imm8_ccr_full()
{
	m_CCR &= m_IR[0];
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done_noirq();
}

void h8_device::andc_imm8_ccr_partial()
{
switch(m_inst_substate) {
case 0:
	m_CCR &= m_IR[0];
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done_noirq();
	break;
}
	m_inst_substate = 0;
}

void h8_device::ldc_imm8_ccr_full()
{
	m_CCR = m_IR[0];
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done_noirq();
}

void h8_device::ldc_imm8_ccr_partial()
{
switch(m_inst_substate) {
case 0:
	m_CCR = m_IR[0];
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done_noirq();
	break;
}
	m_inst_substate = 0;
}

void h8_device::add_b_r8h_r8l_full()
{
	r8_w(m_IR[0], do_add8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::add_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_add8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::add_w_r16h_r16l_full()
{
	r16_w(m_IR[0], do_add16(r16_r(m_IR[0]), r16_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::add_w_r16h_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], do_add16(r16_r(m_IR[0]), r16_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::inc_b_one_r8l_full()
{
	r8_w(m_IR[0], do_inc8(r8_r(m_IR[0]), 1));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::inc_b_one_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_inc8(r8_r(m_IR[0]), 1));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::adds_l_one_r16l_full()
{
	r16_w(m_IR[0], r16_r(m_IR[0])+1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::adds_l_one_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], r16_r(m_IR[0])+1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::adds_l_two_r16l_full()
{
	r16_w(m_IR[0], r16_r(m_IR[0])+2);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::adds_l_two_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], r16_r(m_IR[0])+2);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_b_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0] >> 4);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::mov_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0] >> 4);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_w_r16h_r16l_full()
{
	m_TMP1 = r16_r(m_IR[0] >> 4);
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::mov_w_r16h_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r16_r(m_IR[0] >> 4);
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::addx_b_r8h_r8l_full()
{
	r8_w(m_IR[0], do_addx8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::addx_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_addx8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::daa_b_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP2 = 0;
	if(m_CCR & F_C) {
		if(m_has_hc && (m_CCR & F_H)) {
			if((m_TMP1 & 0xf0) <= 0x30 && (m_TMP1 & 0x0f) <= 3)
				m_TMP2 = 0x66;
		} else {
			if((m_TMP1 & 0xf0) <= 0x20)
				m_TMP2 = (m_TMP1 & 0x0f) <= 9 ? 0x60 : 0x66;
		}
	} else {
		if(m_has_hc && (m_CCR & F_H)) {
			if((m_TMP1 & 0x0f) <= 3)
				m_TMP2 = (m_TMP1 & 0xf0) <= 0x90 ? 0x06 : 0x66;
		} else {
			if((m_TMP1 & 0x0f) <= 9)
				m_TMP2 = (m_TMP1 & 0xf0) <= 0x90 ? 0x00 : 0x60;
			else
				m_TMP2 = (m_TMP1 & 0xf0) <= 0x80 ? 0x06 : 0x66;
		}
	}
	r8_w(m_IR[0], do_add8(m_TMP1, m_TMP2));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::daa_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP2 = 0;
	if(m_CCR & F_C) {
		if(m_has_hc && (m_CCR & F_H)) {
			if((m_TMP1 & 0xf0) <= 0x30 && (m_TMP1 & 0x0f) <= 3)
				m_TMP2 = 0x66;
		} else {
			if((m_TMP1 & 0xf0) <= 0x20)
				m_TMP2 = (m_TMP1 & 0x0f) <= 9 ? 0x60 : 0x66;
		}
	} else {
		if(m_has_hc && (m_CCR & F_H)) {
			if((m_TMP1 & 0x0f) <= 3)
				m_TMP2 = (m_TMP1 & 0xf0) <= 0x90 ? 0x06 : 0x66;
		} else {
			if((m_TMP1 & 0x0f) <= 9)
				m_TMP2 = (m_TMP1 & 0xf0) <= 0x90 ? 0x00 : 0x60;
			else
				m_TMP2 = (m_TMP1 & 0xf0) <= 0x80 ? 0x06 : 0x66;
		}
	}
	r8_w(m_IR[0], do_add8(m_TMP1, m_TMP2));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::shll_b_r8l_full()
{
	r8_w(m_IR[0], do_shll8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::shll_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_shll8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::shal_b_r8l_full()
{
	r8_w(m_IR[0], do_shal8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::shal_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_shal8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::shlr_b_r8l_full()
{
	r8_w(m_IR[0], do_shlr8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::shlr_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_shlr8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::shar_b_r8l_full()
{
	r8_w(m_IR[0], do_shar8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::shar_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_shar8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::rotxl_b_r8l_full()
{
	r8_w(m_IR[0], do_rotxl8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::rotxl_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_rotxl8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::rotl_b_r8l_full()
{
	r8_w(m_IR[0], do_rotl8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::rotl_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_rotl8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::rotxr_b_r8l_full()
{
	r8_w(m_IR[0], do_rotxr8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::rotxr_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_rotxr8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::rotr_b_r8l_full()
{
	r8_w(m_IR[0], do_rotr8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::rotr_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_rotr8(r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::or_b_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0] >> 4) | r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::or_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0] >> 4) | r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::xor_b_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0] >> 4) ^ r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::xor_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0] >> 4) ^ r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::and_b_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0] >> 4) & r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::and_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0] >> 4) & r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::not_b_r8l_full()
{
	m_TMP1 = ~r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::not_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = ~r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::neg_b_r8l_full()
{
	r8_w(m_IR[0], do_sub8(0, r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::neg_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_sub8(0, r8_r(m_IR[0])));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::sub_b_r8h_r8l_full()
{
	r8_w(m_IR[0], do_sub8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::sub_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_sub8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::sub_w_r16h_r16l_full()
{
	r16_w(m_IR[0], do_sub16(r16_r(m_IR[0]), r16_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::sub_w_r16h_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], do_sub16(r16_r(m_IR[0]), r16_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::dec_b_one_r8l_full()
{
	r8_w(m_IR[0], do_dec8(r8_r(m_IR[0]), 1));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::dec_b_one_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_dec8(r8_r(m_IR[0]), 1));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::subs_l_one_r16l_full()
{
	r16_w(m_IR[0], r16_r(m_IR[0])-1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::subs_l_one_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], r16_r(m_IR[0])-1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::subs_l_two_r16l_full()
{
	r16_w(m_IR[0], r16_r(m_IR[0])-2);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::subs_l_two_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], r16_r(m_IR[0])-2);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::cmp_b_r8h_r8l_full()
{
	do_sub8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::cmp_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	do_sub8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::cmp_w_r16h_r16l_full()
{
	do_sub16(r16_r(m_IR[0]), r16_r(m_IR[0] >> 4));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::cmp_w_r16h_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	do_sub16(r16_r(m_IR[0]), r16_r(m_IR[0] >> 4));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::subx_b_r8h_r8l_full()
{
	r8_w(m_IR[0], do_subx8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::subx_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_subx8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::das_b_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP2 = 0;
	if(m_CCR & F_C) {
		if(m_has_hc && (m_CCR & F_H)) {
			if((m_TMP1 & 0xf0) >= 0x60 && (m_TMP1 & 0x0f) >= 6)
				m_TMP2 = 0x9a;
		} else {
			if((m_TMP1 & 0xf0) >= 0x70 && (m_TMP1 & 0x0f) <= 9)
				m_TMP2 = 0xa0;
		}
	} else {
		if(m_has_hc && (m_CCR & F_H)) {
			if((m_TMP1 & 0xf0) <= 0x80 && (m_TMP1 & 0x0f) >= 6)
				m_TMP2 = 0xfa;
		}
	}
	r8_w(m_IR[0], do_add8(m_TMP1, m_TMP2));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::das_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP2 = 0;
	if(m_CCR & F_C) {
		if(m_has_hc && (m_CCR & F_H)) {
			if((m_TMP1 & 0xf0) >= 0x60 && (m_TMP1 & 0x0f) >= 6)
				m_TMP2 = 0x9a;
		} else {
			if((m_TMP1 & 0xf0) >= 0x70 && (m_TMP1 & 0x0f) <= 9)
				m_TMP2 = 0xa0;
		}
	} else {
		if(m_has_hc && (m_CCR & F_H)) {
			if((m_TMP1 & 0xf0) <= 0x80 && (m_TMP1 & 0x0f) >= 6)
				m_TMP2 = 0xfa;
		}
	}
	r8_w(m_IR[0], do_add8(m_TMP1, m_TMP2));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_b_abs8_r8u_full()
{
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP1 = read8(0xffffff00 | m_IR[0]);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	prefetch_done();
}

void h8_device::mov_b_abs8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP1 = read8(0xffffff00 | m_IR[0]);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_b_r8u_abs8_full()
{
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP1 = r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	write8(0xffffff00 | m_IR[0], m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_done();
}

void h8_device::mov_b_r8u_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_TMP1 = r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	[[fallthrough]];
case 3:;
	write8(0xffffff00 | m_IR[0], m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bt_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(true)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::bt_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(true)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bf_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(false)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::bf_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(false)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bhi_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(!(m_CCR & (F_C|F_Z)))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::bhi_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(!(m_CCR & (F_C|F_Z)))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bls_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(m_CCR & (F_C|F_Z))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::bls_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(m_CCR & (F_C|F_Z))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bcc_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(!(m_CCR & F_C))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::bcc_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(!(m_CCR & F_C))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bcs_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(m_CCR & F_C)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::bcs_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(m_CCR & F_C)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bne_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(!(m_CCR & F_Z))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::bne_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(!(m_CCR & F_Z))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::beq_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(m_CCR & F_Z)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::beq_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(m_CCR & F_Z)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bvc_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(!(m_CCR & F_V))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::bvc_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(!(m_CCR & F_V))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bvs_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(m_CCR & F_V)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::bvs_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(m_CCR & F_V)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bpl_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(!(m_CCR & F_N))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::bpl_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(!(m_CCR & F_N))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bmi_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(m_CCR & F_N)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::bmi_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(m_CCR & F_N)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bge_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(!((m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::bge_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(!((m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::blt_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if((m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::blt_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if((m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bgt_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(!((m_CCR & F_Z) || (m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::bgt_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(!((m_CCR & F_Z) || (m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::ble_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if((m_CCR & F_Z) || (m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void h8_device::ble_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if((m_CCR & F_Z) || (m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mulxu_b_r8h_r16l_full()
{
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	r16_w(m_IR[0], u8(r16_r(m_IR[0])) * r8_r(m_IR[0] >> 4));
	internal(m_has_mac ? 2 : 11);
	prefetch_done();
}

void h8_device::mulxu_b_r8h_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	r16_w(m_IR[0], u8(r16_r(m_IR[0])) * r8_r(m_IR[0] >> 4));
	internal(m_has_mac ? 2 : 11);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::divxu_b_r8h_r16l_full()
{
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	internal(11);
	m_TMP1 = r16_r(m_IR[0]);
	m_TMP2 = r8_r(m_IR[0] >> 4);
	m_CCR &= ~(F_Z|F_N);
	if(m_TMP2 & 0x80)
		m_CCR |= F_N;
	if(!m_TMP2) {
		m_CCR |= F_Z;
	} else {
		int q = m_TMP1 / m_TMP2;
		int r = m_TMP1 % m_TMP2;
		r16_w(m_IR[0], (q & 0xff) | ((r & 0xff) << 8));
	}
	prefetch_done();
}

void h8_device::divxu_b_r8h_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	internal(11);
	m_TMP1 = r16_r(m_IR[0]);
	m_TMP2 = r8_r(m_IR[0] >> 4);
	m_CCR &= ~(F_Z|F_N);
	if(m_TMP2 & 0x80)
		m_CCR |= F_N;
	if(!m_TMP2) {
		m_CCR |= F_Z;
	} else {
		int q = m_TMP1 / m_TMP2;
		int r = m_TMP1 % m_TMP2;
		r16_w(m_IR[0], (q & 0xff) | ((r & 0xff) << 8));
	}
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::rts_full()
{
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP1 = r16_r(7);
	m_TMP2 = read16(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	r16_w(7, m_TMP1+2);
	internal(1);
	m_PC = m_TMP2;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::rts_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_TMP1 = r16_r(7);
	[[fallthrough]];
case 3:;
	m_TMP2 = read16(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	r16_w(7, m_TMP1+2);
	internal(1);
	m_PC = m_TMP2;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 5:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bsr_rel8_full()
{
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP2 = m_PC;
	m_PC += s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = r16_r(7) - 2;
	r16_w(7, m_TMP1);
	write16(m_TMP1, m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bsr_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_TMP2 = m_PC;
	m_PC += s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	m_TMP1 = r16_r(7) - 2;
	r16_w(7, m_TMP1);
	[[fallthrough]];
case 5:;
	write16(m_TMP1, m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::rte_full()
{
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP1 = r16_r(7);
	m_CCR = read16(m_TMP1) >> 8;
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 = read16(m_TMP1+2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	r16_w(7, m_TMP1+4);
	internal(1);
	m_PC = m_TMP2;
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_PC += 2;
	prefetch_done_notrace();
}

void h8_device::rte_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_TMP1 = r16_r(7);
	[[fallthrough]];
case 3:;
	m_CCR = read16(m_TMP1) >> 8;
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:;
	m_TMP2 = read16(m_TMP1+2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	r16_w(7, m_TMP1+4);
	internal(1);
	m_PC = m_TMP2;
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 7:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_PC += 2;
	prefetch_done_notrace();
	break;
}
	m_inst_substate = 0;
}

void h8_device::jmp_r16h_full()
{
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_PC = r16_r(m_IR[0] >> 4);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::jmp_r16h_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_PC = r16_r(m_IR[0] >> 4);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::jmp_abs16e_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	internal(1);
	m_PC = m_IR[1];
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::jmp_abs16e_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	internal(1);
	m_PC = m_IR[1];
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::jmp_abs8i_full()
{
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_PC = read16(m_IR[0] & 0xff);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	internal(1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::jmp_abs8i_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_PC = read16(m_IR[0] & 0xff);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	internal(1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 5:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::jsr_r16h_full()
{
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP2 = m_PC;
	m_PC = r16_r(m_IR[0] >> 4);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = r16_r(7) - 2;
	r16_w(7, m_TMP1);
	write16(m_TMP1, m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::jsr_r16h_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_TMP2 = m_PC;
	m_PC = r16_r(m_IR[0] >> 4);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	m_TMP1 = r16_r(7) - 2;
	r16_w(7, m_TMP1);
	[[fallthrough]];
case 5:;
	write16(m_TMP1, m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::jsr_abs16e_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	internal(1);
	m_TMP2 = m_PC;
	m_PC = m_IR[1];
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = r16_r(7) - 2;
	r16_w(7, m_TMP1);
	write16(m_TMP1, m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::jsr_abs16e_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	internal(1);
	m_TMP2 = m_PC;
	m_PC = m_IR[1];
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	m_TMP1 = r16_r(7) - 2;
	r16_w(7, m_TMP1);
	[[fallthrough]];
case 5:;
	write16(m_TMP1, m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::jsr_abs8i_full()
{
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP2 = m_PC;
	m_PC = read16(m_IR[0] & 0xff);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_PC += 2;
	m_TMP1 = r16_r(7) - 2;
	r16_w(7, m_TMP1);
	write16(m_TMP1, m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_done();
}

void h8_device::jsr_abs8i_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_TMP2 = m_PC;
	[[fallthrough]];
case 3:;
	m_PC = read16(m_IR[0] & 0xff);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 5:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_PC += 2;
	m_TMP1 = r16_r(7) - 2;
	r16_w(7, m_TMP1);
	[[fallthrough]];
case 7:;
	write16(m_TMP1, m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bset_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 |= 1 << ((r8_r(m_IR[0] >> 4)) & 7);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bset_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 |= 1 << ((r8_r(m_IR[0] >> 4)) & 7);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bnot_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 ^= 1 << ((r8_r(m_IR[0] >> 4)) & 7);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bnot_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 ^= 1 << ((r8_r(m_IR[0] >> 4)) & 7);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bclr_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 &= ~(1 << ((r8_r(m_IR[0] >> 4)) & 7));
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bclr_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 &= ~(1 << ((r8_r(m_IR[0] >> 4)) & 7));
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::btst_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((r8_r(m_IR[0] >> 4)) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::btst_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((r8_r(m_IR[0] >> 4)) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bst_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(m_CCR & F_C)
		m_TMP1 |= 1 << ((m_IR[0] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[0] >> 4) & 7));
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bst_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(m_CCR & F_C)
		m_TMP1 |= 1 << ((m_IR[0] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[0] >> 4) & 7));
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bist_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_CCR & F_C))
		m_TMP1 |= 1 << ((m_IR[0] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[0] >> 4) & 7));
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bist_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_CCR & F_C))
		m_TMP1 |= 1 << ((m_IR[0] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[0] >> 4) & 7));
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_b_r16ih_r8l_full()
{
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP1 = read8(r16_r(m_IR[0] >> 4));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	prefetch_done();
}

void h8_device::mov_b_r16ih_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP1 = read8(r16_r(m_IR[0] >> 4));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_b_r8l_r16ih_full()
{
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP1 = r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	write8(r16_r((m_IR[0] >> 4) & 7), m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_done();
}

void h8_device::mov_b_r8l_r16ih_partial()
{
switch(m_inst_substate) {
case 0:
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_TMP1 = r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	[[fallthrough]];
case 3:;
	write8(r16_r((m_IR[0] >> 4) & 7), m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_w_r16ih_r16l_full()
{
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP1 = read16(r16_r(m_IR[0] >> 4));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
	prefetch_done();
}

void h8_device::mov_w_r16ih_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP1 = read16(r16_r(m_IR[0] >> 4));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_w_r16l_r16ih_full()
{
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP1 = r16_r(m_IR[0]);
	set_nzv16(m_TMP1);
	write16(r16_r((m_IR[0] >> 4) & 7), m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_done();
}

void h8_device::mov_w_r16l_r16ih_partial()
{
switch(m_inst_substate) {
case 0:
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_TMP1 = r16_r(m_IR[0]);
	set_nzv16(m_TMP1);
	[[fallthrough]];
case 3:;
	write16(r16_r((m_IR[0] >> 4) & 7), m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_b_abs16_r8l_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = read8(s16(m_IR[1]));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	prefetch_done();
}

void h8_device::mov_b_abs16_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	m_TMP1 = read8(s16(m_IR[1]));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::movfpe_abs16_r8l_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	// TODO: timing
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = read8(s16(m_IR[1]));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	prefetch_done();
}

void h8_device::movfpe_abs16_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	// TODO: timing
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	m_TMP1 = read8(s16(m_IR[1]));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_b_r8l_abs16_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	write8(s16(m_IR[1]), m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::mov_b_r8l_abs16_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	m_TMP1 = r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	[[fallthrough]];
case 5:;
	write8(s16(m_IR[1]), m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::movtpe_r8l_abs16_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	// TODO: timing
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	write8(s16(m_IR[1]), m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::movtpe_r8l_abs16_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	// TODO: timing
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	m_TMP1 = r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	[[fallthrough]];
case 5:;
	write8(s16(m_IR[1]), m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_w_abs16_r16l_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = read16(s16(m_IR[1]));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
	prefetch_done();
}

void h8_device::mov_w_abs16_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	m_TMP1 = read16(s16(m_IR[1]));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_w_r16l_abs16_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = r16_r(m_IR[0]);
	set_nzv16(m_TMP1);
	write16(s16(m_IR[1]), m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::mov_w_r16l_abs16_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	m_TMP1 = r16_r(m_IR[0]);
	set_nzv16(m_TMP1);
	[[fallthrough]];
case 5:;
	write16(s16(m_IR[1]), m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_b_r16ph_r8l_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	internal(1);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += 1;
	r16_w(m_IR[0] >> 4, m_TMP2);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	prefetch_done();
}

void h8_device::mov_b_r16ph_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	internal(1);
	[[fallthrough]];
case 3:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += 1;
	r16_w(m_IR[0] >> 4, m_TMP2);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_b_r8l_pr16h_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP2 = r16_r((m_IR[0] >> 4) & 7);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	internal(1);
	m_TMP2 -= 1;
	r16_w((m_IR[0] >> 4) & 7, m_TMP2);
	set_nzv8(m_TMP1);
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_done();
}

void h8_device::mov_b_r8l_pr16h_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP2 = r16_r((m_IR[0] >> 4) & 7);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	internal(1);
	m_TMP2 -= 1;
	r16_w((m_IR[0] >> 4) & 7, m_TMP2);
	set_nzv8(m_TMP1);
	[[fallthrough]];
case 3:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_w_r16ph_r16l_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	internal(1);
	m_TMP1 = read16(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += 2;
	r16_w(m_IR[0] >> 4, m_TMP2);
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
	prefetch_done();
}

void h8_device::mov_w_r16ph_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	internal(1);
	[[fallthrough]];
case 3:;
	m_TMP1 = read16(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += 2;
	r16_w(m_IR[0] >> 4, m_TMP2);
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_w_r16l_pr16h_full()
{
	m_TMP1 = r16_r(m_IR[0]);
	m_TMP2 = r16_r((m_IR[0] >> 4) & 7);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	internal(1);
	m_TMP2 -= 2;
	r16_w((m_IR[0] >> 4) & 7, m_TMP2);
	set_nzv16(m_TMP1);
	write16(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_done();
}

void h8_device::mov_w_r16l_pr16h_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r16_r(m_IR[0]);
	m_TMP2 = r16_r((m_IR[0] >> 4) & 7);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	internal(1);
	m_TMP2 -= 2;
	r16_w((m_IR[0] >> 4) & 7, m_TMP2);
	set_nzv16(m_TMP1);
	[[fallthrough]];
case 3:;
	write16(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_b_r16d16h_r8l_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = u16(r16_r(m_IR[0] >> 4) + s16(m_IR[1]));
	m_TMP2 = read8(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nzv8(m_TMP2);
	r8_w(m_IR[0], m_TMP2);
	prefetch_done();
}

void h8_device::mov_b_r16d16h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	m_TMP1 = u16(r16_r(m_IR[0] >> 4) + s16(m_IR[1]));
	[[fallthrough]];
case 5:;
	m_TMP2 = read8(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nzv8(m_TMP2);
	r8_w(m_IR[0], m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_b_r8l_r16d16h_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = u16(r16_r((m_IR[0] >> 4) & 7) + s16(m_IR[1]));
	m_TMP2 = r8_r(m_IR[0]);
	set_nzv8(m_TMP2);
	write8(m_TMP1, m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::mov_b_r8l_r16d16h_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	m_TMP1 = u16(r16_r((m_IR[0] >> 4) & 7) + s16(m_IR[1]));
	m_TMP2 = r8_r(m_IR[0]);
	set_nzv8(m_TMP2);
	[[fallthrough]];
case 5:;
	write8(m_TMP1, m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_w_r16d16h_r16l_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = u16(r16_r(m_IR[0] >> 4) + s16(m_IR[1]));
	m_TMP2 = read16(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nzv16(m_TMP2);
	r16_w(m_IR[0], m_TMP2);
	prefetch_done();
}

void h8_device::mov_w_r16d16h_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	m_TMP1 = u16(r16_r(m_IR[0] >> 4) + s16(m_IR[1]));
	[[fallthrough]];
case 5:;
	m_TMP2 = read16(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nzv16(m_TMP2);
	r16_w(m_IR[0], m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_w_r16l_r16d16h_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = u16(r16_r((m_IR[0] >> 4) & 7) + s16(m_IR[1]));
	m_TMP2 = r16_r(m_IR[0]);
	set_nzv16(m_TMP2);
	write16(m_TMP1, m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::mov_w_r16l_r16d16h_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	m_TMP1 = u16(r16_r((m_IR[0] >> 4) & 7) + s16(m_IR[1]));
	m_TMP2 = r16_r(m_IR[0]);
	set_nzv16(m_TMP2);
	[[fallthrough]];
case 5:;
	write16(m_TMP1, m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bset_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 |= 1 << ((m_IR[0] >> 4) & 7);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bset_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 |= 1 << ((m_IR[0] >> 4) & 7);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bnot_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 ^= 1 << ((m_IR[0] >> 4) & 7);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bnot_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 ^= 1 << ((m_IR[0] >> 4) & 7);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bclr_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 &= ~(1 << ((m_IR[0] >> 4) & 7));
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bclr_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 &= ~(1 << ((m_IR[0] >> 4) & 7));
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::btst_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::btst_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bor_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR |= F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bor_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR |= F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bior_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR |= F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bior_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR |= F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bxor_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR ^= F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bxor_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR ^= F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bixor_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR ^= F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bixor_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR ^= F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::band_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::band_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::biand_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::biand_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bld_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bld_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bild_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bild_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_w_imm16_r16l_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	set_nzv16(m_IR[1]);
	r16_w(m_IR[0], m_IR[1]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::mov_w_imm16_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	set_nzv16(m_IR[1]);
	r16_w(m_IR[0], m_IR[1]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::eepmov_b_full()
{
	while(r8_r(4+8)) {
		m_TMP1 = read8(r16_r(5));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
		write8(r16_r(6), m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		r16_w(5, r16_r(5)+1);
		r16_w(6, r16_r(6)+1);
		r8_w(4+8, r8_r(4+8)-1);
	}
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::eepmov_b_partial()
{
switch(m_inst_substate) {
case 0:
	while(r8_r(4+8)) {
	[[fallthrough]];
case 1:;
		m_TMP1 = read8(r16_r(5));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:;
		write8(r16_r(6), m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		r16_w(5, r16_r(5)+1);
		r16_w(6, r16_r(6)+1);
		r8_w(4+8, r8_r(4+8)-1);
	}
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 5:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::btst_r8h_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((r8_r(m_IR[1] >> 4)) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::btst_r8h_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(m_TMP1 & (1 << ((r8_r(m_IR[1] >> 4)) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::btst_imm3_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::btst_imm3_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bor_imm3_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR |= F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bor_imm3_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR |= F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bior_imm3_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR |= F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bior_imm3_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR |= F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bxor_imm3_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR ^= F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bxor_imm3_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR ^= F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bixor_imm3_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR ^= F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bixor_imm3_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR ^= F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::band_imm3_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::band_imm3_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::biand_imm3_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::biand_imm3_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bld_imm3_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bld_imm3_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bild_imm3_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bild_imm3_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bset_r8h_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 |= 1 << ((r8_r(m_IR[1] >> 4)) & 7);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bset_r8h_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_TMP1 |= 1 << ((r8_r(m_IR[1] >> 4)) & 7);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bnot_r8h_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 ^= 1 << ((r8_r(m_IR[1] >> 4)) & 7);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bnot_r8h_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_TMP1 ^= 1 << ((r8_r(m_IR[1] >> 4)) & 7);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bclr_r8h_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 &= ~(1 << ((r8_r(m_IR[1] >> 4)) & 7));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bclr_r8h_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_TMP1 &= ~(1 << ((r8_r(m_IR[1] >> 4)) & 7));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bst_imm3_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_CCR & F_C)
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bst_imm3_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(m_CCR & F_C)
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bist_imm3_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(!(m_CCR & F_C))
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bist_imm3_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(!(m_CCR & F_C))
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bset_imm3_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bset_imm3_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bnot_imm3_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 ^= 1 << ((m_IR[1] >> 4) & 7);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bnot_imm3_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_TMP1 ^= 1 << ((m_IR[1] >> 4) & 7);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bclr_imm3_r16ihh_full()
{
	m_TMP2 = r16_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bclr_imm3_r16ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r16_r(m_IR[0] >> 4);
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::btst_r8h_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((r8_r(m_IR[1] >> 4)) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::btst_r8h_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(m_TMP1 & (1 << ((r8_r(m_IR[1] >> 4)) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::btst_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::btst_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bor_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR |= F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bor_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR |= F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bior_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR |= F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bior_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR |= F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bxor_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR ^= F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bxor_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR ^= F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bixor_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR ^= F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bixor_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR ^= F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::band_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::band_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::biand_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::biand_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bld_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bld_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bild_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::bild_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bset_r8h_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 |= 1 << ((r8_r(m_IR[1] >> 4)) & 7);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bset_r8h_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_TMP1 |= 1 << ((r8_r(m_IR[1] >> 4)) & 7);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bnot_r8h_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 ^= 1 << ((r8_r(m_IR[1] >> 4)) & 7);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bnot_r8h_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_TMP1 ^= 1 << ((r8_r(m_IR[1] >> 4)) & 7);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bclr_r8h_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 &= ~(1 << ((r8_r(m_IR[1] >> 4)) & 7));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bclr_r8h_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_TMP1 &= ~(1 << ((r8_r(m_IR[1] >> 4)) & 7));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bst_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	if(m_CCR & F_C)
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bst_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	if(m_CCR & F_C)
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bist_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	if(!(m_CCR & F_C))
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bist_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	if(!(m_CCR & F_C))
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bset_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bset_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bnot_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 ^= 1 << ((m_IR[1] >> 4) & 7);
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bnot_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	m_TMP1 ^= 1 << ((m_IR[1] >> 4) & 7);
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::bclr_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void h8_device::bclr_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::add_b_imm8_r8u_full()
{
	r8_w(m_IR[0] >> 8, do_add8(r8_r(m_IR[0] >> 8), m_IR[0]));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::add_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0] >> 8, do_add8(r8_r(m_IR[0] >> 8), m_IR[0]));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::addx_b_imm8_r8u_full()
{
	r8_w(m_IR[0] >> 8, do_addx8(r8_r(m_IR[0] >> 8), m_IR[0]));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::addx_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0] >> 8, do_addx8(r8_r(m_IR[0] >> 8), m_IR[0]));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::cmp_b_imm8_r8u_full()
{
	do_sub8(r8_r(m_IR[0] >> 8), m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::cmp_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	do_sub8(r8_r(m_IR[0] >> 8), m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::subx_b_imm8_r8u_full()
{
	r8_w(m_IR[0] >> 8, do_subx8(r8_r(m_IR[0] >> 8), m_IR[0]));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::subx_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0] >> 8, do_subx8(r8_r(m_IR[0] >> 8), m_IR[0]));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::or_b_imm8_r8u_full()
{
	m_TMP1 = m_IR[0] | r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::or_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_IR[0] | r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::xor_b_imm8_r8u_full()
{
	m_TMP1 = m_IR[0] ^ r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::xor_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_IR[0] ^ r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::and_b_imm8_r8u_full()
{
	m_TMP1 = m_IR[0] & r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::and_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_IR[0] & r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::mov_b_imm8_r8u_full()
{
	set_nzv8(m_IR[0]);
	r8_w(m_IR[0] >> 8, m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void h8_device::mov_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	set_nzv8(m_IR[0]);
	r8_w(m_IR[0] >> 8, m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::state_reset_full()
{
	m_CCR |= (m_has_hc ? F_I : F_H);
	m_EXR = EXR_I | EXR_NC;
	if(m_mode_advanced) {
		m_IR[0] = read16i(0);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
		m_IR[1] = read16i(2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		m_PC = (m_IR[0] << 16) | m_IR[1];
	} else {
		m_PC = read16i(0);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_PC += 2;
	prefetch_done_noirq();
}

void h8_device::state_reset_partial()
{
switch(m_inst_substate) {
case 0:
	m_CCR |= (m_has_hc ? F_I : F_H);
	m_EXR = EXR_I | EXR_NC;
	if(m_mode_advanced) {
	[[fallthrough]];
case 1:;
		m_IR[0] = read16i(0);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:;
		m_IR[1] = read16i(2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		m_PC = (m_IR[0] << 16) | m_IR[1];
	} else {
	[[fallthrough]];
case 5:;
		m_PC = read16i(0);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 7:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_PC += 2;
	prefetch_done_noirq();
	break;
}
	m_inst_substate = 0;
}

void h8_device::state_irq_full()
{
	internal(1);
	m_TMP1 = r16_r(7) - 2;
	r16_w(7, m_TMP1);
	write16(m_TMP1, m_NPC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 = r16_r(7) - 2;
	r16_w(7, m_TMP1);
	write16(m_TMP1, (m_CCR << 8) | ((m_PC >> 16) & 0xff));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	debugger_exception_hook(m_taken_irq_vector);
	m_PC = read16i(2*m_taken_irq_vector);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	internal(1);
	irq_setup();
	update_irq_filter();
	interrupt_taken();
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_PC += 2;
	prefetch_done_noirq();
}

void h8_device::state_irq_partial()
{
switch(m_inst_substate) {
case 0:
	internal(1);
	m_TMP1 = r16_r(7) - 2;
	r16_w(7, m_TMP1);
	[[fallthrough]];
case 1:;
	write16(m_TMP1, m_NPC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_TMP1 = r16_r(7) - 2;
	r16_w(7, m_TMP1);
	[[fallthrough]];
case 3:;
	write16(m_TMP1, (m_CCR << 8) | ((m_PC >> 16) & 0xff));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	debugger_exception_hook(m_taken_irq_vector);
	[[fallthrough]];
case 5:;
	m_PC = read16i(2*m_taken_irq_vector);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	internal(1);
	irq_setup();
	update_irq_filter();
	interrupt_taken();
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 7:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_PC += 2;
	prefetch_done_noirq();
	break;
}
	m_inst_substate = 0;
}

void h8_device::state_dma_full()
{
	m_TMP2 = m_current_dma;
	if(m_dma_channel[m_TMP2]->m_flags & h8_dma_state::MODE_16) {
		m_TMP1 = read16(m_dma_channel[m_TMP2]->m_source);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
		if(m_dma_channel[m_TMP2]->m_count == 1 && !(m_dma_channel[m_TMP2]->m_flags & (h8_dma_state::BLOCK|h8_dma_state::REPEAT)))
			m_dma_device->count_last(m_dma_channel[m_TMP2]->m_id);
		write16(m_dma_channel[m_TMP2]->m_dest, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	} else {
		m_TMP1 = read8(m_dma_channel[m_TMP2]->m_source);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		if(m_dma_channel[m_TMP2]->m_count == 1 && !(m_dma_channel[m_TMP2]->m_flags & (h8_dma_state::BLOCK|h8_dma_state::REPEAT)))
			m_dma_device->count_last(m_dma_channel[m_TMP2]->m_id);
		write8(m_dma_channel[m_TMP2]->m_dest, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_dma_channel[m_TMP2]->m_source += m_dma_channel[m_TMP2]->m_incs;
	m_dma_channel[m_TMP2]->m_dest   += m_dma_channel[m_TMP2]->m_incd;
	m_dma_channel[m_TMP2]->m_count--;
	if(m_dma_channel[m_TMP2]->m_flags & h8_dma_state::SUSPEND_AFTER_TRANSFER) {
		m_dma_channel[m_TMP2]->m_flags |= h8_dma_state::SUSPENDED;
		update_active_dma_channel();
	}
	if(!m_dma_channel[m_TMP2]->m_count)
		m_dma_device->count_done(m_TMP2);
	prefetch_done();
}

void h8_device::state_dma_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = m_current_dma;
	if(m_dma_channel[m_TMP2]->m_flags & h8_dma_state::MODE_16) {
	[[fallthrough]];
case 1:;
		m_TMP1 = read16(m_dma_channel[m_TMP2]->m_source);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
		if(m_dma_channel[m_TMP2]->m_count == 1 && !(m_dma_channel[m_TMP2]->m_flags & (h8_dma_state::BLOCK|h8_dma_state::REPEAT)))
			m_dma_device->count_last(m_dma_channel[m_TMP2]->m_id);
	[[fallthrough]];
case 3:;
		write16(m_dma_channel[m_TMP2]->m_dest, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	} else {
	[[fallthrough]];
case 5:;
		m_TMP1 = read8(m_dma_channel[m_TMP2]->m_source);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		if(m_dma_channel[m_TMP2]->m_count == 1 && !(m_dma_channel[m_TMP2]->m_flags & (h8_dma_state::BLOCK|h8_dma_state::REPEAT)))
			m_dma_device->count_last(m_dma_channel[m_TMP2]->m_id);
	[[fallthrough]];
case 7:;
		write8(m_dma_channel[m_TMP2]->m_dest, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	m_dma_channel[m_TMP2]->m_source += m_dma_channel[m_TMP2]->m_incs;
	m_dma_channel[m_TMP2]->m_dest   += m_dma_channel[m_TMP2]->m_incd;
	m_dma_channel[m_TMP2]->m_count--;
	if(m_dma_channel[m_TMP2]->m_flags & h8_dma_state::SUSPEND_AFTER_TRANSFER) {
		m_dma_channel[m_TMP2]->m_flags |= h8_dma_state::SUSPENDED;
		update_active_dma_channel();
	}
	if(!m_dma_channel[m_TMP2]->m_count)
		m_dma_device->count_done(m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_0100_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x20000 | m_IR[1];
}

void h8_device::dispatch_0100_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x20000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_01007800_full()
{
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x30000 | m_IR[2];
}

void h8_device::dispatch_01007800_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x30000 | m_IR[2];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_0110_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x40000 | m_IR[1];
}

void h8_device::dispatch_0110_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x40000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_0120_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x50000 | m_IR[1];
}

void h8_device::dispatch_0120_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x50000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_0130_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x60000 | m_IR[1];
}

void h8_device::dispatch_0130_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x60000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_0140_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x70000 | m_IR[1];
}

void h8_device::dispatch_0140_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x70000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_01407800_full()
{
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x80000 | m_IR[2];
}

void h8_device::dispatch_01407800_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x80000 | m_IR[2];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_01407880_full()
{
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x90000 | m_IR[2];
}

void h8_device::dispatch_01407880_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x90000 | m_IR[2];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_0141_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0xa0000 | m_IR[1];
}

void h8_device::dispatch_0141_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0xa0000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_01417800_full()
{
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0xb0000 | m_IR[2];
}

void h8_device::dispatch_01417800_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0xb0000 | m_IR[2];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_01417880_full()
{
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0xc0000 | m_IR[2];
}

void h8_device::dispatch_01417880_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0xc0000 | m_IR[2];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_0160_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0xd0000 | m_IR[1];
}

void h8_device::dispatch_0160_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0xd0000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_01c0_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0xe0000 | m_IR[1];
}

void h8_device::dispatch_01c0_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0xe0000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_01d0_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0xf0000 | m_IR[1];
}

void h8_device::dispatch_01d0_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0xf0000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_01e0_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x100000 | m_IR[1];
}

void h8_device::dispatch_01e0_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x100000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_01f0_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x110000 | m_IR[1];
}

void h8_device::dispatch_01f0_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x110000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_6a10_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x120000 | m_IR[2];
}

void h8_device::dispatch_6a10_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	m_inst_state = 0x120000 | m_IR[2];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_6a18_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x130000 | m_IR[2];
}

void h8_device::dispatch_6a18_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	m_inst_state = 0x130000 | m_IR[2];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_6a30_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_IR[3] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x140000 | m_IR[3];
}

void h8_device::dispatch_6a30_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	m_IR[3] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_PC += 2;
	m_inst_state = 0x140000 | m_IR[3];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_6a38_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_IR[3] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x150000 | m_IR[3];
}

void h8_device::dispatch_6a38_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	[[fallthrough]];
case 5:;
	m_IR[3] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_PC += 2;
	m_inst_state = 0x150000 | m_IR[3];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_7800_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x160000 | m_IR[1];
}

void h8_device::dispatch_7800_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x160000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_7b5c_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x170000 | m_IR[1];
}

void h8_device::dispatch_7b5c_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x170000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_7bd4_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x180000 | m_IR[1];
}

void h8_device::dispatch_7bd4_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x180000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_7c00_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x190000 | m_IR[1];
}

void h8_device::dispatch_7c00_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x190000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_7d00_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x1a0000 | m_IR[1];
}

void h8_device::dispatch_7d00_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x1a0000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_7e00_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x1b0000 | m_IR[1];
}

void h8_device::dispatch_7e00_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x1b0000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::dispatch_7f00_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x1c0000 | m_IR[1];
}

void h8_device::dispatch_7f00_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x1c0000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void h8_device::do_exec_full()
{
	switch(m_inst_state >> 16) {
	case 0x00: {
		switch((m_inst_state >> 8) & 0xff) {
		case 0x00: {
			switch(m_inst_state & 0xff) {
			case 0x00: nop_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x01: {
			switch(m_inst_state & 0xff) {
			case 0x80: sleep_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x02: {
			switch(m_inst_state & 0xf0) {
			case 0x00: stc_ccr_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x03: {
			switch(m_inst_state & 0xf0) {
			case 0x00: ldc_r8l_ccr_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x04: {
			orc_imm8_ccr_full();
			break;
		}
		case 0x05: {
			xorc_imm8_ccr_full();
			break;
		}
		case 0x06: {
			andc_imm8_ccr_full();
			break;
		}
		case 0x07: {
			ldc_imm8_ccr_full();
			break;
		}
		case 0x08: {
			add_b_r8h_r8l_full();
			break;
		}
		case 0x09: {
			switch(m_inst_state & 0x88) {
			case 0x00: add_w_r16h_r16l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x0a: {
			switch(m_inst_state & 0xf0) {
			case 0x00: inc_b_one_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x0b: {
			switch(m_inst_state & 0xf8) {
			case 0x00: adds_l_one_r16l_full(); break;
			case 0x80: adds_l_two_r16l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x0c: {
			mov_b_r8h_r8l_full();
			break;
		}
		case 0x0d: {
			switch(m_inst_state & 0x88) {
			case 0x00: mov_w_r16h_r16l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x0e: {
			addx_b_r8h_r8l_full();
			break;
		}
		case 0x0f: {
			switch(m_inst_state & 0xf0) {
			case 0x00: daa_b_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x10: {
			switch(m_inst_state & 0xf0) {
			case 0x00: shll_b_r8l_full(); break;
			case 0x80: shal_b_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x11: {
			switch(m_inst_state & 0xf0) {
			case 0x00: shlr_b_r8l_full(); break;
			case 0x80: shar_b_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x12: {
			switch(m_inst_state & 0xf0) {
			case 0x00: rotxl_b_r8l_full(); break;
			case 0x80: rotl_b_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x13: {
			switch(m_inst_state & 0xf0) {
			case 0x00: rotxr_b_r8l_full(); break;
			case 0x80: rotr_b_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x14: {
			or_b_r8h_r8l_full();
			break;
		}
		case 0x15: {
			xor_b_r8h_r8l_full();
			break;
		}
		case 0x16: {
			and_b_r8h_r8l_full();
			break;
		}
		case 0x17: {
			switch(m_inst_state & 0xf0) {
			case 0x00: not_b_r8l_full(); break;
			case 0x80: neg_b_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x18: {
			sub_b_r8h_r8l_full();
			break;
		}
		case 0x19: {
			switch(m_inst_state & 0x88) {
			case 0x00: sub_w_r16h_r16l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x1a: {
			switch(m_inst_state & 0xf0) {
			case 0x00: dec_b_one_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x1b: {
			switch(m_inst_state & 0xf8) {
			case 0x00: subs_l_one_r16l_full(); break;
			case 0x80: subs_l_two_r16l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x1c: {
			cmp_b_r8h_r8l_full();
			break;
		}
		case 0x1d: {
			switch(m_inst_state & 0x88) {
			case 0x00: cmp_w_r16h_r16l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x1e: {
			subx_b_r8h_r8l_full();
			break;
		}
		case 0x1f: {
			switch(m_inst_state & 0xf0) {
			case 0x00: das_b_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27: case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f: {
			mov_b_abs8_r8u_full();
			break;
		}
		case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f: {
			mov_b_r8u_abs8_full();
			break;
		}
		case 0x40: {
			bt_rel8_full();
			break;
		}
		case 0x41: {
			bf_rel8_full();
			break;
		}
		case 0x42: {
			bhi_rel8_full();
			break;
		}
		case 0x43: {
			bls_rel8_full();
			break;
		}
		case 0x44: {
			bcc_rel8_full();
			break;
		}
		case 0x45: {
			bcs_rel8_full();
			break;
		}
		case 0x46: {
			bne_rel8_full();
			break;
		}
		case 0x47: {
			beq_rel8_full();
			break;
		}
		case 0x48: {
			bvc_rel8_full();
			break;
		}
		case 0x49: {
			bvs_rel8_full();
			break;
		}
		case 0x4a: {
			bpl_rel8_full();
			break;
		}
		case 0x4b: {
			bmi_rel8_full();
			break;
		}
		case 0x4c: {
			bge_rel8_full();
			break;
		}
		case 0x4d: {
			blt_rel8_full();
			break;
		}
		case 0x4e: {
			bgt_rel8_full();
			break;
		}
		case 0x4f: {
			ble_rel8_full();
			break;
		}
		case 0x50: {
			switch(m_inst_state & 0x08) {
			case 0x00: mulxu_b_r8h_r16l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x51: {
			switch(m_inst_state & 0x08) {
			case 0x00: divxu_b_r8h_r16l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x54: {
			switch(m_inst_state & 0xff) {
			case 0x70: rts_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x55: {
			bsr_rel8_full();
			break;
		}
		case 0x56: {
			switch(m_inst_state & 0xff) {
			case 0x70: rte_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x59: {
			switch(m_inst_state & 0x8f) {
			case 0x00: jmp_r16h_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x5a: {
			switch(m_inst_state & 0xff) {
			case 0x00: jmp_abs16e_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x5b: {
			jmp_abs8i_full();
			break;
		}
		case 0x5d: {
			switch(m_inst_state & 0x8f) {
			case 0x00: jsr_r16h_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x5e: {
			switch(m_inst_state & 0xff) {
			case 0x00: jsr_abs16e_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x5f: {
			jsr_abs8i_full();
			break;
		}
		case 0x60: {
			bset_r8h_r8l_full();
			break;
		}
		case 0x61: {
			bnot_r8h_r8l_full();
			break;
		}
		case 0x62: {
			bclr_r8h_r8l_full();
			break;
		}
		case 0x63: {
			btst_r8h_r8l_full();
			break;
		}
		case 0x67: {
			switch(m_inst_state & 0x80) {
			case 0x00: bst_imm3_r8l_full(); break;
			case 0x80: bist_imm3_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x68: {
			switch(m_inst_state & 0x80) {
			case 0x00: mov_b_r16ih_r8l_full(); break;
			case 0x80: mov_b_r8l_r16ih_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x69: {
			switch(m_inst_state & 0x88) {
			case 0x00: mov_w_r16ih_r16l_full(); break;
			case 0x80: mov_w_r16l_r16ih_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x6a: {
			switch(m_inst_state & 0xf0) {
			case 0x00: mov_b_abs16_r8l_full(); break;
			case 0x40: movfpe_abs16_r8l_full(); break;
			case 0x80: mov_b_r8l_abs16_full(); break;
			case 0xc0: movtpe_r8l_abs16_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x6b: {
			switch(m_inst_state & 0xf8) {
			case 0x00: mov_w_abs16_r16l_full(); break;
			case 0x80: mov_w_r16l_abs16_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x6c: {
			switch(m_inst_state & 0x80) {
			case 0x00: mov_b_r16ph_r8l_full(); break;
			case 0x80: mov_b_r8l_pr16h_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x6d: {
			switch(m_inst_state & 0x88) {
			case 0x00: mov_w_r16ph_r16l_full(); break;
			case 0x80: mov_w_r16l_pr16h_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x6e: {
			switch(m_inst_state & 0x80) {
			case 0x00: mov_b_r16d16h_r8l_full(); break;
			case 0x80: mov_b_r8l_r16d16h_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x6f: {
			switch(m_inst_state & 0x88) {
			case 0x00: mov_w_r16d16h_r16l_full(); break;
			case 0x80: mov_w_r16l_r16d16h_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x70: {
			switch(m_inst_state & 0x80) {
			case 0x00: bset_imm3_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x71: {
			switch(m_inst_state & 0x80) {
			case 0x00: bnot_imm3_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x72: {
			switch(m_inst_state & 0x80) {
			case 0x00: bclr_imm3_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x73: {
			switch(m_inst_state & 0x80) {
			case 0x00: btst_imm3_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x74: {
			switch(m_inst_state & 0x80) {
			case 0x00: bor_imm3_r8l_full(); break;
			case 0x80: bior_imm3_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x75: {
			switch(m_inst_state & 0x80) {
			case 0x00: bxor_imm3_r8l_full(); break;
			case 0x80: bixor_imm3_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x76: {
			switch(m_inst_state & 0x80) {
			case 0x00: band_imm3_r8l_full(); break;
			case 0x80: biand_imm3_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x77: {
			switch(m_inst_state & 0x80) {
			case 0x00: bld_imm3_r8l_full(); break;
			case 0x80: bild_imm3_r8l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x79: {
			switch(m_inst_state & 0xf8) {
			case 0x00: mov_w_imm16_r16l_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x7b: {
			switch(m_inst_state & 0xff) {
			case 0x5c: dispatch_7b5c_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x7c: {
			switch(m_inst_state & 0x8f) {
			case 0x00: dispatch_7c00_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x7d: {
			switch(m_inst_state & 0x8f) {
			case 0x00: dispatch_7d00_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x7e: {
			dispatch_7e00_full();
			break;
		}
		case 0x7f: {
			dispatch_7f00_full();
			break;
		}
		case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f: {
			add_b_imm8_r8u_full();
			break;
		}
		case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f: {
			addx_b_imm8_r8u_full();
			break;
		}
		case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7: case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf: {
			cmp_b_imm8_r8u_full();
			break;
		}
		case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7: case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf: {
			subx_b_imm8_r8u_full();
			break;
		}
		case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7: case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf: {
			or_b_imm8_r8u_full();
			break;
		}
		case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7: case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf: {
			xor_b_imm8_r8u_full();
			break;
		}
		case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7: case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef: {
			and_b_imm8_r8u_full();
			break;
		}
		case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7: case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff: {
			mov_b_imm8_r8u_full();
			break;
		}
		default: illegal(); break;
		}
		break;
	}
	case 0x01: {
		switch(m_inst_state & 0xffff) {
		case 0x00: state_reset_full(); break;
		case 0x01: state_irq_full(); break;
		case 0x03: state_dma_full(); break;
		}
		break;
	}
	case 0x17: {
		switch((m_inst_state >> 8) & 0xff) {
		case 0x59: {
			switch(m_inst_state & 0xff) {
			case 0x8f: eepmov_b_full(); break;
			default: illegal(); break;
			}
			break;
		}
		default: illegal(); break;
		}
		break;
	}
	case 0x19: {
		switch((m_inst_state >> 8) & 0xff) {
		case 0x63: {
			switch(m_inst_state & 0x0f) {
			case 0x00: btst_r8h_r16ihh_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x73: {
			switch(m_inst_state & 0x8f) {
			case 0x00: btst_imm3_r16ihh_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x74: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bor_imm3_r16ihh_full(); break;
			case 0x80: bior_imm3_r16ihh_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x75: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bxor_imm3_r16ihh_full(); break;
			case 0x80: bixor_imm3_r16ihh_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x76: {
			switch(m_inst_state & 0x8f) {
			case 0x00: band_imm3_r16ihh_full(); break;
			case 0x80: biand_imm3_r16ihh_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x77: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bld_imm3_r16ihh_full(); break;
			case 0x80: bild_imm3_r16ihh_full(); break;
			default: illegal(); break;
			}
			break;
		}
		default: illegal(); break;
		}
		break;
	}
	case 0x1a: {
		switch((m_inst_state >> 8) & 0xff) {
		case 0x60: {
			switch(m_inst_state & 0x0f) {
			case 0x00: bset_r8h_r16ihh_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x61: {
			switch(m_inst_state & 0x0f) {
			case 0x00: bnot_r8h_r16ihh_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x62: {
			switch(m_inst_state & 0x0f) {
			case 0x00: bclr_r8h_r16ihh_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x67: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bst_imm3_r16ihh_full(); break;
			case 0x80: bist_imm3_r16ihh_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x70: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bset_imm3_r16ihh_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x71: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bnot_imm3_r16ihh_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x72: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bclr_imm3_r16ihh_full(); break;
			default: illegal(); break;
			}
			break;
		}
		default: illegal(); break;
		}
		break;
	}
	case 0x1b: {
		switch((m_inst_state >> 8) & 0xff) {
		case 0x63: {
			switch(m_inst_state & 0x0f) {
			case 0x00: btst_r8h_abs8_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x73: {
			switch(m_inst_state & 0x8f) {
			case 0x00: btst_imm3_abs8_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x74: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bor_imm3_abs8_full(); break;
			case 0x80: bior_imm3_abs8_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x75: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bxor_imm3_abs8_full(); break;
			case 0x80: bixor_imm3_abs8_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x76: {
			switch(m_inst_state & 0x8f) {
			case 0x00: band_imm3_abs8_full(); break;
			case 0x80: biand_imm3_abs8_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x77: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bld_imm3_abs8_full(); break;
			case 0x80: bild_imm3_abs8_full(); break;
			default: illegal(); break;
			}
			break;
		}
		default: illegal(); break;
		}
		break;
	}
	case 0x1c: {
		switch((m_inst_state >> 8) & 0xff) {
		case 0x60: {
			switch(m_inst_state & 0x0f) {
			case 0x00: bset_r8h_abs8_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x61: {
			switch(m_inst_state & 0x0f) {
			case 0x00: bnot_r8h_abs8_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x62: {
			switch(m_inst_state & 0x0f) {
			case 0x00: bclr_r8h_abs8_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x67: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bst_imm3_abs8_full(); break;
			case 0x80: bist_imm3_abs8_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x70: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bset_imm3_abs8_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x71: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bnot_imm3_abs8_full(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x72: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bclr_imm3_abs8_full(); break;
			default: illegal(); break;
			}
			break;
		}
		default: illegal(); break;
		}
		break;
	}
	}
}
void h8_device::do_exec_partial()
{
	switch(m_inst_state >> 16) {
	case 0x00: {
		switch((m_inst_state >> 8) & 0xff) {
		case 0x00: {
			switch(m_inst_state & 0xff) {
			case 0x00: nop_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x01: {
			switch(m_inst_state & 0xff) {
			case 0x80: sleep_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x02: {
			switch(m_inst_state & 0xf0) {
			case 0x00: stc_ccr_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x03: {
			switch(m_inst_state & 0xf0) {
			case 0x00: ldc_r8l_ccr_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x04: {
			orc_imm8_ccr_partial();
			break;
		}
		case 0x05: {
			xorc_imm8_ccr_partial();
			break;
		}
		case 0x06: {
			andc_imm8_ccr_partial();
			break;
		}
		case 0x07: {
			ldc_imm8_ccr_partial();
			break;
		}
		case 0x08: {
			add_b_r8h_r8l_partial();
			break;
		}
		case 0x09: {
			switch(m_inst_state & 0x88) {
			case 0x00: add_w_r16h_r16l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x0a: {
			switch(m_inst_state & 0xf0) {
			case 0x00: inc_b_one_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x0b: {
			switch(m_inst_state & 0xf8) {
			case 0x00: adds_l_one_r16l_partial(); break;
			case 0x80: adds_l_two_r16l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x0c: {
			mov_b_r8h_r8l_partial();
			break;
		}
		case 0x0d: {
			switch(m_inst_state & 0x88) {
			case 0x00: mov_w_r16h_r16l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x0e: {
			addx_b_r8h_r8l_partial();
			break;
		}
		case 0x0f: {
			switch(m_inst_state & 0xf0) {
			case 0x00: daa_b_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x10: {
			switch(m_inst_state & 0xf0) {
			case 0x00: shll_b_r8l_partial(); break;
			case 0x80: shal_b_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x11: {
			switch(m_inst_state & 0xf0) {
			case 0x00: shlr_b_r8l_partial(); break;
			case 0x80: shar_b_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x12: {
			switch(m_inst_state & 0xf0) {
			case 0x00: rotxl_b_r8l_partial(); break;
			case 0x80: rotl_b_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x13: {
			switch(m_inst_state & 0xf0) {
			case 0x00: rotxr_b_r8l_partial(); break;
			case 0x80: rotr_b_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x14: {
			or_b_r8h_r8l_partial();
			break;
		}
		case 0x15: {
			xor_b_r8h_r8l_partial();
			break;
		}
		case 0x16: {
			and_b_r8h_r8l_partial();
			break;
		}
		case 0x17: {
			switch(m_inst_state & 0xf0) {
			case 0x00: not_b_r8l_partial(); break;
			case 0x80: neg_b_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x18: {
			sub_b_r8h_r8l_partial();
			break;
		}
		case 0x19: {
			switch(m_inst_state & 0x88) {
			case 0x00: sub_w_r16h_r16l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x1a: {
			switch(m_inst_state & 0xf0) {
			case 0x00: dec_b_one_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x1b: {
			switch(m_inst_state & 0xf8) {
			case 0x00: subs_l_one_r16l_partial(); break;
			case 0x80: subs_l_two_r16l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x1c: {
			cmp_b_r8h_r8l_partial();
			break;
		}
		case 0x1d: {
			switch(m_inst_state & 0x88) {
			case 0x00: cmp_w_r16h_r16l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x1e: {
			subx_b_r8h_r8l_partial();
			break;
		}
		case 0x1f: {
			switch(m_inst_state & 0xf0) {
			case 0x00: das_b_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27: case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f: {
			mov_b_abs8_r8u_partial();
			break;
		}
		case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f: {
			mov_b_r8u_abs8_partial();
			break;
		}
		case 0x40: {
			bt_rel8_partial();
			break;
		}
		case 0x41: {
			bf_rel8_partial();
			break;
		}
		case 0x42: {
			bhi_rel8_partial();
			break;
		}
		case 0x43: {
			bls_rel8_partial();
			break;
		}
		case 0x44: {
			bcc_rel8_partial();
			break;
		}
		case 0x45: {
			bcs_rel8_partial();
			break;
		}
		case 0x46: {
			bne_rel8_partial();
			break;
		}
		case 0x47: {
			beq_rel8_partial();
			break;
		}
		case 0x48: {
			bvc_rel8_partial();
			break;
		}
		case 0x49: {
			bvs_rel8_partial();
			break;
		}
		case 0x4a: {
			bpl_rel8_partial();
			break;
		}
		case 0x4b: {
			bmi_rel8_partial();
			break;
		}
		case 0x4c: {
			bge_rel8_partial();
			break;
		}
		case 0x4d: {
			blt_rel8_partial();
			break;
		}
		case 0x4e: {
			bgt_rel8_partial();
			break;
		}
		case 0x4f: {
			ble_rel8_partial();
			break;
		}
		case 0x50: {
			switch(m_inst_state & 0x08) {
			case 0x00: mulxu_b_r8h_r16l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x51: {
			switch(m_inst_state & 0x08) {
			case 0x00: divxu_b_r8h_r16l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x54: {
			switch(m_inst_state & 0xff) {
			case 0x70: rts_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x55: {
			bsr_rel8_partial();
			break;
		}
		case 0x56: {
			switch(m_inst_state & 0xff) {
			case 0x70: rte_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x59: {
			switch(m_inst_state & 0x8f) {
			case 0x00: jmp_r16h_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x5a: {
			switch(m_inst_state & 0xff) {
			case 0x00: jmp_abs16e_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x5b: {
			jmp_abs8i_partial();
			break;
		}
		case 0x5d: {
			switch(m_inst_state & 0x8f) {
			case 0x00: jsr_r16h_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x5e: {
			switch(m_inst_state & 0xff) {
			case 0x00: jsr_abs16e_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x5f: {
			jsr_abs8i_partial();
			break;
		}
		case 0x60: {
			bset_r8h_r8l_partial();
			break;
		}
		case 0x61: {
			bnot_r8h_r8l_partial();
			break;
		}
		case 0x62: {
			bclr_r8h_r8l_partial();
			break;
		}
		case 0x63: {
			btst_r8h_r8l_partial();
			break;
		}
		case 0x67: {
			switch(m_inst_state & 0x80) {
			case 0x00: bst_imm3_r8l_partial(); break;
			case 0x80: bist_imm3_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x68: {
			switch(m_inst_state & 0x80) {
			case 0x00: mov_b_r16ih_r8l_partial(); break;
			case 0x80: mov_b_r8l_r16ih_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x69: {
			switch(m_inst_state & 0x88) {
			case 0x00: mov_w_r16ih_r16l_partial(); break;
			case 0x80: mov_w_r16l_r16ih_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x6a: {
			switch(m_inst_state & 0xf0) {
			case 0x00: mov_b_abs16_r8l_partial(); break;
			case 0x40: movfpe_abs16_r8l_partial(); break;
			case 0x80: mov_b_r8l_abs16_partial(); break;
			case 0xc0: movtpe_r8l_abs16_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x6b: {
			switch(m_inst_state & 0xf8) {
			case 0x00: mov_w_abs16_r16l_partial(); break;
			case 0x80: mov_w_r16l_abs16_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x6c: {
			switch(m_inst_state & 0x80) {
			case 0x00: mov_b_r16ph_r8l_partial(); break;
			case 0x80: mov_b_r8l_pr16h_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x6d: {
			switch(m_inst_state & 0x88) {
			case 0x00: mov_w_r16ph_r16l_partial(); break;
			case 0x80: mov_w_r16l_pr16h_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x6e: {
			switch(m_inst_state & 0x80) {
			case 0x00: mov_b_r16d16h_r8l_partial(); break;
			case 0x80: mov_b_r8l_r16d16h_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x6f: {
			switch(m_inst_state & 0x88) {
			case 0x00: mov_w_r16d16h_r16l_partial(); break;
			case 0x80: mov_w_r16l_r16d16h_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x70: {
			switch(m_inst_state & 0x80) {
			case 0x00: bset_imm3_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x71: {
			switch(m_inst_state & 0x80) {
			case 0x00: bnot_imm3_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x72: {
			switch(m_inst_state & 0x80) {
			case 0x00: bclr_imm3_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x73: {
			switch(m_inst_state & 0x80) {
			case 0x00: btst_imm3_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x74: {
			switch(m_inst_state & 0x80) {
			case 0x00: bor_imm3_r8l_partial(); break;
			case 0x80: bior_imm3_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x75: {
			switch(m_inst_state & 0x80) {
			case 0x00: bxor_imm3_r8l_partial(); break;
			case 0x80: bixor_imm3_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x76: {
			switch(m_inst_state & 0x80) {
			case 0x00: band_imm3_r8l_partial(); break;
			case 0x80: biand_imm3_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x77: {
			switch(m_inst_state & 0x80) {
			case 0x00: bld_imm3_r8l_partial(); break;
			case 0x80: bild_imm3_r8l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x79: {
			switch(m_inst_state & 0xf8) {
			case 0x00: mov_w_imm16_r16l_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x7b: {
			switch(m_inst_state & 0xff) {
			case 0x5c: dispatch_7b5c_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x7c: {
			switch(m_inst_state & 0x8f) {
			case 0x00: dispatch_7c00_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x7d: {
			switch(m_inst_state & 0x8f) {
			case 0x00: dispatch_7d00_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x7e: {
			dispatch_7e00_partial();
			break;
		}
		case 0x7f: {
			dispatch_7f00_partial();
			break;
		}
		case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f: {
			add_b_imm8_r8u_partial();
			break;
		}
		case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f: {
			addx_b_imm8_r8u_partial();
			break;
		}
		case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7: case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf: {
			cmp_b_imm8_r8u_partial();
			break;
		}
		case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7: case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf: {
			subx_b_imm8_r8u_partial();
			break;
		}
		case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7: case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf: {
			or_b_imm8_r8u_partial();
			break;
		}
		case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7: case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf: {
			xor_b_imm8_r8u_partial();
			break;
		}
		case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7: case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef: {
			and_b_imm8_r8u_partial();
			break;
		}
		case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7: case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff: {
			mov_b_imm8_r8u_partial();
			break;
		}
		default: illegal(); break;
		}
		break;
	}
	case 0x01: {
		switch(m_inst_state & 0xffff) {
		case 0x00: state_reset_partial(); break;
		case 0x01: state_irq_partial(); break;
		case 0x03: state_dma_partial(); break;
		}
		break;
	}
	case 0x17: {
		switch((m_inst_state >> 8) & 0xff) {
		case 0x59: {
			switch(m_inst_state & 0xff) {
			case 0x8f: eepmov_b_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		default: illegal(); break;
		}
		break;
	}
	case 0x19: {
		switch((m_inst_state >> 8) & 0xff) {
		case 0x63: {
			switch(m_inst_state & 0x0f) {
			case 0x00: btst_r8h_r16ihh_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x73: {
			switch(m_inst_state & 0x8f) {
			case 0x00: btst_imm3_r16ihh_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x74: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bor_imm3_r16ihh_partial(); break;
			case 0x80: bior_imm3_r16ihh_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x75: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bxor_imm3_r16ihh_partial(); break;
			case 0x80: bixor_imm3_r16ihh_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x76: {
			switch(m_inst_state & 0x8f) {
			case 0x00: band_imm3_r16ihh_partial(); break;
			case 0x80: biand_imm3_r16ihh_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x77: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bld_imm3_r16ihh_partial(); break;
			case 0x80: bild_imm3_r16ihh_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		default: illegal(); break;
		}
		break;
	}
	case 0x1a: {
		switch((m_inst_state >> 8) & 0xff) {
		case 0x60: {
			switch(m_inst_state & 0x0f) {
			case 0x00: bset_r8h_r16ihh_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x61: {
			switch(m_inst_state & 0x0f) {
			case 0x00: bnot_r8h_r16ihh_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x62: {
			switch(m_inst_state & 0x0f) {
			case 0x00: bclr_r8h_r16ihh_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x67: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bst_imm3_r16ihh_partial(); break;
			case 0x80: bist_imm3_r16ihh_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x70: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bset_imm3_r16ihh_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x71: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bnot_imm3_r16ihh_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x72: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bclr_imm3_r16ihh_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		default: illegal(); break;
		}
		break;
	}
	case 0x1b: {
		switch((m_inst_state >> 8) & 0xff) {
		case 0x63: {
			switch(m_inst_state & 0x0f) {
			case 0x00: btst_r8h_abs8_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x73: {
			switch(m_inst_state & 0x8f) {
			case 0x00: btst_imm3_abs8_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x74: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bor_imm3_abs8_partial(); break;
			case 0x80: bior_imm3_abs8_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x75: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bxor_imm3_abs8_partial(); break;
			case 0x80: bixor_imm3_abs8_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x76: {
			switch(m_inst_state & 0x8f) {
			case 0x00: band_imm3_abs8_partial(); break;
			case 0x80: biand_imm3_abs8_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x77: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bld_imm3_abs8_partial(); break;
			case 0x80: bild_imm3_abs8_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		default: illegal(); break;
		}
		break;
	}
	case 0x1c: {
		switch((m_inst_state >> 8) & 0xff) {
		case 0x60: {
			switch(m_inst_state & 0x0f) {
			case 0x00: bset_r8h_abs8_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x61: {
			switch(m_inst_state & 0x0f) {
			case 0x00: bnot_r8h_abs8_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x62: {
			switch(m_inst_state & 0x0f) {
			case 0x00: bclr_r8h_abs8_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x67: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bst_imm3_abs8_partial(); break;
			case 0x80: bist_imm3_abs8_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x70: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bset_imm3_abs8_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x71: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bnot_imm3_abs8_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		case 0x72: {
			switch(m_inst_state & 0x8f) {
			case 0x00: bclr_imm3_abs8_partial(); break;
			default: illegal(); break;
			}
			break;
		}
		default: illegal(); break;
		}
		break;
	}
	}
}
