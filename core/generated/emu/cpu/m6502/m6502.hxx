void m6502_device::adc_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::adc_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
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
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::adc_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::adc_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::adc_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::adc_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	do_adc(m_TMP);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	do_adc(m_TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::adc_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::adc_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::and_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::and_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::and_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::and_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
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
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::and_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::and_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::and_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A &= m_TMP;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::and_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_A &= m_TMP;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::and_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::and_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::and_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::and_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::and_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::and_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::and_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::and_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::asl_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::asl_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::asl_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::asl_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::asl_acc_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_A = do_asl(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::asl_acc_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_A = do_asl(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::asl_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::asl_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::asl_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::asl_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::bcc_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(!(m_P & F_C)) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::bcc_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(!(m_P & F_C)) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
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
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::bcs_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(m_P & F_C) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::bcs_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(m_P & F_C) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
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
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::beq_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(m_P & F_Z) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::beq_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(m_P & F_Z) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
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
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::bit_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_bit(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::bit_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_bit(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::bit_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	do_bit(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::bit_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_bit(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::bmi_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(m_P & F_N) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::bmi_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(m_P & F_N) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
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
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::bne_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(!(m_P & F_Z)) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::bne_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(!(m_P & F_Z)) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
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
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::bpl_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(!(m_P & F_N)) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::bpl_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(!(m_P & F_N)) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
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
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::brk_imp_full()
{
	// The 6502 bug when a nmi occurs in a brk is reproduced (case !irq_taken && nmi_pending)
	if(m_irq_taken) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	} else {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		m_PC++;
	}
	write(m_SP, m_PC >> 8);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	dec_SP();
	write(m_SP, m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	dec_SP();
	write(m_SP, m_irq_taken ? m_P & ~F_B : m_P);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	dec_SP();
	if(m_nmi_pending) {
		standard_irq_callback(NMI_LINE, m_PC);
		m_PC = read_arg(0xfffa);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
		m_PC = set_h(m_PC, read_arg(0xfffb));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
		m_nmi_pending = false;
	} else {
		if(m_irq_taken)
			standard_irq_callback(IRQ_LINE, m_PC);
		m_PC = read_arg(0xfffe);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
		m_PC = set_h(m_PC, read_arg(0xffff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 17;
		} else
			m_inst_substate = 18;
		return;
	}
	}
	m_irq_taken = false;
	m_P |= F_I; // Do *not* move after the prefetch
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 19;
		} else
			m_inst_substate = 20;
		return;
	}
	prefetch_end();
	m_inst_state = -1;
}

void m6502_device::brk_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	// The 6502 bug when a nmi occurs in a brk is reproduced (case !irq_taken && nmi_pending)
	if(m_irq_taken) {
	[[fallthrough]];
case 1:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	} else {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		m_PC++;
	}
	[[fallthrough]];
case 5:
	write(m_SP, m_PC >> 8);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	dec_SP();
	[[fallthrough]];
case 7:
	write(m_SP, m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	dec_SP();
	[[fallthrough]];
case 9:
	write(m_SP, m_irq_taken ? m_P & ~F_B : m_P);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	dec_SP();
	if(m_nmi_pending) {
		standard_irq_callback(NMI_LINE, m_PC);
	[[fallthrough]];
case 11:
		m_PC = read_arg(0xfffa);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	[[fallthrough]];
case 13:
		m_PC = set_h(m_PC, read_arg(0xfffb));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
		m_nmi_pending = false;
	} else {
		if(m_irq_taken)
			standard_irq_callback(IRQ_LINE, m_PC);
	[[fallthrough]];
case 15:
		m_PC = read_arg(0xfffe);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	[[fallthrough]];
case 17:
		m_PC = set_h(m_PC, read_arg(0xffff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 17;
		} else
			m_inst_substate = 18;
		return;
	}
	[[fallthrough]];
case 18:;
	}
	m_irq_taken = false;
	m_P |= F_I; // Do *not* move after the prefetch
	prefetch_start();
	[[fallthrough]];
case 19:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 19;
		} else
			m_inst_substate = 20;
		return;
	}
	[[fallthrough]];
case 20:;
	prefetch_end();
	m_inst_state = -1;
	break;
}
	m_inst_substate = 0;
}

void m6502_device::bvc_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(!(m_P & F_V)) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::bvc_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(!(m_P & F_V)) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
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
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::bvs_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(m_P & F_V) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::bvs_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(m_P & F_V) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
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
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::clc_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_P &= ~F_C;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::clc_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_P &= ~F_C;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cld_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_P &= ~F_D;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::cld_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_P &= ~F_D;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cli_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
	m_P &= ~F_I; // Do *not* move it before the prefetch
}

void m6502_device::cli_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	m_P &= ~F_I; // Do *not* move it before the prefetch
	break;
}
	m_inst_substate = 0;
}

void m6502_device::clv_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_P &= ~F_V;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::clv_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_P &= ~F_V;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cmp_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cmp_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
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
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cmp_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cmp_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cmp_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cmp_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	do_cmp(m_A, m_TMP);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	do_cmp(m_A, m_TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cmp_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cmp_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cpx_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_cmp(m_X, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::cpx_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_cmp(m_X, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cpx_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	do_cmp(m_X, m_TMP);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::cpx_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	do_cmp(m_X, m_TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cpx_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	do_cmp(m_X, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::cpx_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_cmp(m_X, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cpy_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_cmp(m_Y, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::cpy_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_cmp(m_Y, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cpy_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	do_cmp(m_Y, m_TMP);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::cpy_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	do_cmp(m_Y, m_TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::cpy_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	do_cmp(m_Y, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::cpy_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_cmp(m_Y, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::dec_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2--;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::dec_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2--;
	set_nz(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::dec_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2--;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::dec_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2--;
	set_nz(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::dec_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2--;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::dec_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2--;
	set_nz(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::dec_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2--;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::dec_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2--;
	set_nz(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::dex_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_X--;
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::dex_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_X--;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::dey_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_Y--;
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::dey_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_Y--;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::eor_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::eor_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
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
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::eor_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::eor_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::eor_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::eor_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A ^= m_TMP;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_A ^= m_TMP;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::eor_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::eor_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::inc_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2++;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::inc_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2++;
	set_nz(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::inc_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2++;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::inc_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2++;
	set_nz(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::inc_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2++;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::inc_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2++;
	set_nz(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::inc_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2++;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::inc_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2++;
	set_nz(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::inx_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_X++;
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::inx_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_X++;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::iny_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_Y++;
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::iny_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_Y++;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::jmp_adr_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC = m_TMP;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::jmp_adr_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC = m_TMP;
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::jmp_ind_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_PC = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_PC = set_h(m_PC, read(set_l(m_TMP, m_TMP+1)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::jmp_ind_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_PC = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_PC = set_h(m_PC, read(set_l(m_TMP, m_TMP+1)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::jsr_adr_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_SP, m_PC>>8);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	dec_SP();
	write(m_SP, m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	dec_SP();
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_PC = m_TMP;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::jsr_adr_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(m_SP, m_PC>>8);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	dec_SP();
	[[fallthrough]];
case 7:
	write(m_SP, m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	dec_SP();
	[[fallthrough]];
case 9:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_PC = m_TMP;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lda_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_A = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::lda_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_A = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lda_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_A = read(m_TMP + m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::lda_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
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
	[[fallthrough]];
case 7:
	m_A = read(m_TMP + m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lda_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_A = read(m_TMP + m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::lda_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	[[fallthrough]];
case 7:
	m_A = read(m_TMP + m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lda_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::lda_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_A = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lda_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_A = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::lda_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	[[fallthrough]];
case 9:
	m_A = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lda_imm_full()
{
	m_A = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::lda_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_A = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lda_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::lda_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_A = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lda_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_A = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::lda_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_A = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ldx_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::ldx_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ldx_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_X = read(m_TMP + m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::ldx_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	[[fallthrough]];
case 7:
	m_X = read(m_TMP + m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ldx_imm_full()
{
	m_X = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::ldx_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_X = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ldx_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::ldx_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ldx_zpy_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_X = read(uint8_t(m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::ldx_zpy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_X = read(uint8_t(m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ldy_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_Y = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::ldy_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_Y = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ldy_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_X;
	m_Y = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::ldy_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
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
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_Y = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ldy_imm_full()
{
	m_Y = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::ldy_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_Y = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ldy_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_Y = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::ldy_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_Y = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ldy_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_Y = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::ldy_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_Y = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lsr_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::lsr_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lsr_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::lsr_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lsr_acc_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_A = do_lsr(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::lsr_acc_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_A = do_lsr(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lsr_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::lsr_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lsr_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::lsr_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::nop_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::nop_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ora_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ora_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
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
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ora_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ora_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A |= m_TMP;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_A |= m_TMP;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ora_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ora_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ora_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ora_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::pha_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	write(m_SP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	dec_SP();
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::pha_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
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
case 3:
	write(m_SP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	dec_SP();
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::php_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	write(m_SP, m_P);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	dec_SP();
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::php_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
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
case 3:
	write(m_SP, m_P);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	dec_SP();
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::pla_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	inc_SP();
	m_A = read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::pla_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
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
case 3:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	inc_SP();
	[[fallthrough]];
case 5:
	m_A = read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::plp_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	inc_SP();
	m_TMP = read(m_SP) | (F_B|F_E);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
	m_P = m_TMP; // Do *not* move it before the prefetch
}

void m6502_device::plp_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
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
case 3:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	inc_SP();
	[[fallthrough]];
case 5:
	m_TMP = read(m_SP) | (F_B|F_E);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	m_P = m_TMP; // Do *not* move it before the prefetch
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rol_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rol_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rol_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::rol_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rol_acc_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_A = do_rol(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::rol_acc_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_A = do_rol(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rol_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::rol_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rol_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rol_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ror_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::ror_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ror_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::ror_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ror_acc_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_A = do_ror(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::ror_acc_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_A = do_ror(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ror_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::ror_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ror_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::ror_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rti_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	inc_SP();
	m_P = read(m_SP) | (F_B|F_E);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	inc_SP();
	m_PC = read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	inc_SP();
	m_PC = set_h(m_PC, read(m_SP));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rti_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
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
case 3:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	inc_SP();
	[[fallthrough]];
case 5:
	m_P = read(m_SP) | (F_B|F_E);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	inc_SP();
	[[fallthrough]];
case 7:
	m_PC = read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	inc_SP();
	[[fallthrough]];
case 9:
	m_PC = set_h(m_PC, read(m_SP));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rts_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	inc_SP();
	m_PC = read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	inc_SP();
	m_PC = set_h(m_PC, read(m_SP));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_PC++;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rts_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
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
case 3:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	inc_SP();
	[[fallthrough]];
case 5:
	m_PC = read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	inc_SP();
	[[fallthrough]];
case 7:
	m_PC = set_h(m_PC, read(m_SP));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_PC++;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sbc_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sbc_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
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
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sbc_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sbc_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sbc_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sbc_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	do_sbc(m_TMP);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	do_sbc(m_TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sbc_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sbc_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sec_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_P |= F_C;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::sec_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_P |= F_C;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sed_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_P |= F_D;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::sed_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_P |= F_D;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sei_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
	m_P |= F_I; // Do *not* move it before the prefetch
}

void m6502_device::sei_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	m_P |= F_I; // Do *not* move it before the prefetch
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sta_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	write(m_TMP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::sta_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	write(m_TMP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sta_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP+m_X, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::sta_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP+m_X, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sta_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP+m_Y, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::sta_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP+m_Y, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sta_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sta_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sta_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP+m_Y, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sta_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP+m_Y, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sta_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	write(m_TMP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::sta_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	write(m_TMP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sta_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(uint8_t(m_TMP+m_X), m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::sta_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(uint8_t(m_TMP+m_X), m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::stx_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	write(m_TMP, m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::stx_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	write(m_TMP, m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::stx_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	write(m_TMP, m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::stx_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	write(m_TMP, m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::stx_zpy_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(uint8_t(m_TMP+m_Y), m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::stx_zpy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(uint8_t(m_TMP+m_Y), m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sty_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	write(m_TMP, m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::sty_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	write(m_TMP, m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sty_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	write(m_TMP, m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::sty_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	write(m_TMP, m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sty_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(uint8_t(m_TMP+m_X), m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::sty_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(uint8_t(m_TMP+m_X), m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::tax_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_X = m_A;
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::tax_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_X = m_A;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::tay_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_Y = m_A;
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::tay_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_Y = m_A;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::tsx_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_X = m_SP;
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::tsx_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_X = m_SP;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::txa_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_A = m_X;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::txa_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_A = m_X;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::txs_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_SP = set_l(m_SP, m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::txs_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_SP = set_l(m_SP, m_X);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::tya_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_A = m_Y;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::tya_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_A = m_Y;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::reset_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	dec_SP();
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	dec_SP();
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	dec_SP();
	m_P |= F_I;
	m_PC = read_arg(0xfffc);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_PC = set_h(m_PC, read_arg(0xfffd));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
	m_inst_state = -1;
}

void m6502_device::reset_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
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
case 3:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	dec_SP();
	[[fallthrough]];
case 7:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	dec_SP();
	[[fallthrough]];
case 9:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	dec_SP();
	m_P |= F_I;
	[[fallthrough]];
case 11:
	m_PC = read_arg(0xfffc);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	[[fallthrough]];
case 13:
	m_PC = set_h(m_PC, read_arg(0xfffd));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	m_inst_state = -1;
	break;
}
	m_inst_substate = 0;
}

void m6502_device::dcp_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2--;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::dcp_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2--;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::dcp_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2--;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::dcp_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2--;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::dcp_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2--;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::dcp_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2--;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::dcp_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2--;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::dcp_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2--;
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::dcp_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2--;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::dcp_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP += m_Y;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2--;
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::dcp_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2--;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::dcp_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2--;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::dcp_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2--;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::dcp_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2--;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::isb_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2++;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::isb_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2++;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::isb_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2++;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::isb_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2++;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::isb_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2++;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::isb_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2++;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::isb_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2++;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::isb_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2++;
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::isb_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2++;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::isb_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP += m_Y;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2++;
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::isb_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2++;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::isb_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2++;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::isb_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2++;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::isb_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2++;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lax_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::lax_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lax_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_A = m_X = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::lax_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	[[fallthrough]];
case 7:
	m_A = m_X = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lax_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::lax_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lax_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_A = m_X = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::lax_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	[[fallthrough]];
case 9:
	m_A = m_X = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lax_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::lax_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lax_zpy_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_Y);
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::lax_zpy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_Y);
	[[fallthrough]];
case 5:
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rla_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rla_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rla_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::rla_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rla_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::rla_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rla_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::rla_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rla_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::rla_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP += m_Y;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rla_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::rla_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rla_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rla_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rra_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rra_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rra_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::rra_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rra_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::rra_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rra_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::rra_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rra_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::rra_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP += m_Y;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rra_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::rra_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::rra_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rra_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sax_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = m_A & m_X;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::sax_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	m_TMP2 = m_A & m_X;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sax_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = m_A & m_X;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sax_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = m_A & m_X;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sax_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = m_A & m_X;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::sax_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_TMP2 = m_A & m_X;
	[[fallthrough]];
case 3:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sax_zpy_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_Y);
	m_TMP2 = m_A & m_X;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::sax_zpy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_Y);
	m_TMP2 = m_A & m_X;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sbx_imm_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_X &= m_A;
	if(m_X < m_TMP2)
		m_P &= ~F_C;
	else
		m_P |= F_C;
	m_X -= m_TMP2;
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::sbx_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_X &= m_A;
	if(m_X < m_TMP2)
		m_P &= ~F_C;
	else
		m_P |= F_C;
	m_X -= m_TMP2;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sha_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = m_A & m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::sha_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = m_A & m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sha_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = m_A & m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sha_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = m_A & m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::shs_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_SP = set_l(m_SP, m_A & m_X);
	m_TMP2 = m_A & m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::shs_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_SP = set_l(m_SP, m_A & m_X);
	m_TMP2 = m_A & m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::shx_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::shx_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::shy_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = m_Y & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_X))
		m_TMP = set_h(m_TMP+m_X, m_TMP2);
	else
		m_TMP += m_X;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::shy_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = m_Y & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_X))
		m_TMP = set_h(m_TMP+m_X, m_TMP2);
	else
		m_TMP += m_X;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::slo_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::slo_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::slo_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::slo_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::slo_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::slo_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::slo_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::slo_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::slo_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::slo_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP += m_Y;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::slo_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::slo_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::slo_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::slo_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sre_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sre_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sre_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::sre_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sre_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::sre_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sre_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::sre_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sre_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::sre_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP += m_Y;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sre_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::sre_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::sre_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sre_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::anc_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A &= m_TMP;
	set_nz(m_A);
	if(m_A & 0x80)
		m_P |= F_C;
	else
		m_P &= ~F_C;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::anc_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_A &= m_TMP;
	set_nz(m_A);
	if(m_A & 0x80)
		m_P |= F_C;
	else
		m_P &= ~F_C;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::ane_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A &= m_TMP & m_X;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::ane_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_A &= m_TMP & m_X;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::asr_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A = do_lsr(m_A & m_TMP);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::asr_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_A = do_lsr(m_A & m_TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::arr_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A &= m_TMP;
	do_arr();
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::arr_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_A &= m_TMP;
	do_arr();
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::las_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A = m_TMP2 | 0x51;
	m_X = 0xff;
	set_nz(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::las_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
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
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A = m_TMP2 | 0x51;
	m_X = 0xff;
	set_nz(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::lxa_imm_full()
{
	m_A = m_X = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::lxa_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_A = m_X = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::nop_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::nop_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::nop_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	read(m_TMP + m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::nop_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
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
	[[fallthrough]];
case 7:
	read(m_TMP + m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::nop_imm_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::nop_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::nop_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::nop_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::nop_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::nop_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void m6502_device::kil_non_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(0xffff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	read(0xfffe);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(0xfffe);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	for(;;) {
		read(0xffff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	}
}

void m6502_device::kil_non_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(0xffff);
	m_icount--;
	if(m_icount <= 0) {
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
case 5:
	read(0xfffe);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(0xfffe);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	for(;;) {
	[[fallthrough]];
case 9:
		read(0xffff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	}
	break;
}
	m_inst_substate = 0;
}



void m6502_device::do_exec_full()
{
	switch(m_inst_state) {

	case 0x00: brk_imp_full(); break;
	case 0x01: ora_idx_full(); break;
	case 0x02: kil_non_full(); break;
	case 0x03: slo_idx_full(); break;
	case 0x04: nop_zpg_full(); break;
	case 0x05: ora_zpg_full(); break;
	case 0x06: asl_zpg_full(); break;
	case 0x07: slo_zpg_full(); break;
	case 0x08: php_imp_full(); break;
	case 0x09: ora_imm_full(); break;
	case 0x0a: asl_acc_full(); break;
	case 0x0b: anc_imm_full(); break;
	case 0x0c: nop_aba_full(); break;
	case 0x0d: ora_aba_full(); break;
	case 0x0e: asl_aba_full(); break;
	case 0x0f: slo_aba_full(); break;
	case 0x10: bpl_rel_full(); break;
	case 0x11: ora_idy_full(); break;
	case 0x12: kil_non_full(); break;
	case 0x13: slo_idy_full(); break;
	case 0x14: nop_zpx_full(); break;
	case 0x15: ora_zpx_full(); break;
	case 0x16: asl_zpx_full(); break;
	case 0x17: slo_zpx_full(); break;
	case 0x18: clc_imp_full(); break;
	case 0x19: ora_aby_full(); break;
	case 0x1a: nop_imp_full(); break;
	case 0x1b: slo_aby_full(); break;
	case 0x1c: nop_abx_full(); break;
	case 0x1d: ora_abx_full(); break;
	case 0x1e: asl_abx_full(); break;
	case 0x1f: slo_abx_full(); break;
	case 0x20: jsr_adr_full(); break;
	case 0x21: and_idx_full(); break;
	case 0x22: kil_non_full(); break;
	case 0x23: rla_idx_full(); break;
	case 0x24: bit_zpg_full(); break;
	case 0x25: and_zpg_full(); break;
	case 0x26: rol_zpg_full(); break;
	case 0x27: rla_zpg_full(); break;
	case 0x28: plp_imp_full(); break;
	case 0x29: and_imm_full(); break;
	case 0x2a: rol_acc_full(); break;
	case 0x2b: anc_imm_full(); break;
	case 0x2c: bit_aba_full(); break;
	case 0x2d: and_aba_full(); break;
	case 0x2e: rol_aba_full(); break;
	case 0x2f: rla_aba_full(); break;
	case 0x30: bmi_rel_full(); break;
	case 0x31: and_idy_full(); break;
	case 0x32: kil_non_full(); break;
	case 0x33: rla_idy_full(); break;
	case 0x34: nop_zpx_full(); break;
	case 0x35: and_zpx_full(); break;
	case 0x36: rol_zpx_full(); break;
	case 0x37: rla_zpx_full(); break;
	case 0x38: sec_imp_full(); break;
	case 0x39: and_aby_full(); break;
	case 0x3a: nop_imp_full(); break;
	case 0x3b: rla_aby_full(); break;
	case 0x3c: nop_abx_full(); break;
	case 0x3d: and_abx_full(); break;
	case 0x3e: rol_abx_full(); break;
	case 0x3f: rla_abx_full(); break;
	case 0x40: rti_imp_full(); break;
	case 0x41: eor_idx_full(); break;
	case 0x42: kil_non_full(); break;
	case 0x43: sre_idx_full(); break;
	case 0x44: nop_zpg_full(); break;
	case 0x45: eor_zpg_full(); break;
	case 0x46: lsr_zpg_full(); break;
	case 0x47: sre_zpg_full(); break;
	case 0x48: pha_imp_full(); break;
	case 0x49: eor_imm_full(); break;
	case 0x4a: lsr_acc_full(); break;
	case 0x4b: asr_imm_full(); break;
	case 0x4c: jmp_adr_full(); break;
	case 0x4d: eor_aba_full(); break;
	case 0x4e: lsr_aba_full(); break;
	case 0x4f: sre_aba_full(); break;
	case 0x50: bvc_rel_full(); break;
	case 0x51: eor_idy_full(); break;
	case 0x52: kil_non_full(); break;
	case 0x53: sre_idy_full(); break;
	case 0x54: nop_zpx_full(); break;
	case 0x55: eor_zpx_full(); break;
	case 0x56: lsr_zpx_full(); break;
	case 0x57: sre_zpx_full(); break;
	case 0x58: cli_imp_full(); break;
	case 0x59: eor_aby_full(); break;
	case 0x5a: nop_imp_full(); break;
	case 0x5b: sre_aby_full(); break;
	case 0x5c: nop_abx_full(); break;
	case 0x5d: eor_abx_full(); break;
	case 0x5e: lsr_abx_full(); break;
	case 0x5f: sre_abx_full(); break;
	case 0x60: rts_imp_full(); break;
	case 0x61: adc_idx_full(); break;
	case 0x62: kil_non_full(); break;
	case 0x63: rra_idx_full(); break;
	case 0x64: nop_zpg_full(); break;
	case 0x65: adc_zpg_full(); break;
	case 0x66: ror_zpg_full(); break;
	case 0x67: rra_zpg_full(); break;
	case 0x68: pla_imp_full(); break;
	case 0x69: adc_imm_full(); break;
	case 0x6a: ror_acc_full(); break;
	case 0x6b: arr_imm_full(); break;
	case 0x6c: jmp_ind_full(); break;
	case 0x6d: adc_aba_full(); break;
	case 0x6e: ror_aba_full(); break;
	case 0x6f: rra_aba_full(); break;
	case 0x70: bvs_rel_full(); break;
	case 0x71: adc_idy_full(); break;
	case 0x72: kil_non_full(); break;
	case 0x73: rra_idy_full(); break;
	case 0x74: nop_zpx_full(); break;
	case 0x75: adc_zpx_full(); break;
	case 0x76: ror_zpx_full(); break;
	case 0x77: rra_zpx_full(); break;
	case 0x78: sei_imp_full(); break;
	case 0x79: adc_aby_full(); break;
	case 0x7a: nop_imp_full(); break;
	case 0x7b: rra_aby_full(); break;
	case 0x7c: nop_abx_full(); break;
	case 0x7d: adc_abx_full(); break;
	case 0x7e: ror_abx_full(); break;
	case 0x7f: rra_abx_full(); break;
	case 0x80: nop_imm_full(); break;
	case 0x81: sta_idx_full(); break;
	case 0x82: nop_imm_full(); break;
	case 0x83: sax_idx_full(); break;
	case 0x84: sty_zpg_full(); break;
	case 0x85: sta_zpg_full(); break;
	case 0x86: stx_zpg_full(); break;
	case 0x87: sax_zpg_full(); break;
	case 0x88: dey_imp_full(); break;
	case 0x89: nop_imm_full(); break;
	case 0x8a: txa_imp_full(); break;
	case 0x8b: ane_imm_full(); break;
	case 0x8c: sty_aba_full(); break;
	case 0x8d: sta_aba_full(); break;
	case 0x8e: stx_aba_full(); break;
	case 0x8f: sax_aba_full(); break;
	case 0x90: bcc_rel_full(); break;
	case 0x91: sta_idy_full(); break;
	case 0x92: kil_non_full(); break;
	case 0x93: sha_idy_full(); break;
	case 0x94: sty_zpx_full(); break;
	case 0x95: sta_zpx_full(); break;
	case 0x96: stx_zpy_full(); break;
	case 0x97: sax_zpy_full(); break;
	case 0x98: tya_imp_full(); break;
	case 0x99: sta_aby_full(); break;
	case 0x9a: txs_imp_full(); break;
	case 0x9b: shs_aby_full(); break;
	case 0x9c: shy_abx_full(); break;
	case 0x9d: sta_abx_full(); break;
	case 0x9e: shx_aby_full(); break;
	case 0x9f: sha_aby_full(); break;
	case 0xa0: ldy_imm_full(); break;
	case 0xa1: lda_idx_full(); break;
	case 0xa2: ldx_imm_full(); break;
	case 0xa3: lax_idx_full(); break;
	case 0xa4: ldy_zpg_full(); break;
	case 0xa5: lda_zpg_full(); break;
	case 0xa6: ldx_zpg_full(); break;
	case 0xa7: lax_zpg_full(); break;
	case 0xa8: tay_imp_full(); break;
	case 0xa9: lda_imm_full(); break;
	case 0xaa: tax_imp_full(); break;
	case 0xab: lxa_imm_full(); break;
	case 0xac: ldy_aba_full(); break;
	case 0xad: lda_aba_full(); break;
	case 0xae: ldx_aba_full(); break;
	case 0xaf: lax_aba_full(); break;
	case 0xb0: bcs_rel_full(); break;
	case 0xb1: lda_idy_full(); break;
	case 0xb2: kil_non_full(); break;
	case 0xb3: lax_idy_full(); break;
	case 0xb4: ldy_zpx_full(); break;
	case 0xb5: lda_zpx_full(); break;
	case 0xb6: ldx_zpy_full(); break;
	case 0xb7: lax_zpy_full(); break;
	case 0xb8: clv_imp_full(); break;
	case 0xb9: lda_aby_full(); break;
	case 0xba: tsx_imp_full(); break;
	case 0xbb: las_aby_full(); break;
	case 0xbc: ldy_abx_full(); break;
	case 0xbd: lda_abx_full(); break;
	case 0xbe: ldx_aby_full(); break;
	case 0xbf: lax_aby_full(); break;
	case 0xc0: cpy_imm_full(); break;
	case 0xc1: cmp_idx_full(); break;
	case 0xc2: nop_imm_full(); break;
	case 0xc3: dcp_idx_full(); break;
	case 0xc4: cpy_zpg_full(); break;
	case 0xc5: cmp_zpg_full(); break;
	case 0xc6: dec_zpg_full(); break;
	case 0xc7: dcp_zpg_full(); break;
	case 0xc8: iny_imp_full(); break;
	case 0xc9: cmp_imm_full(); break;
	case 0xca: dex_imp_full(); break;
	case 0xcb: sbx_imm_full(); break;
	case 0xcc: cpy_aba_full(); break;
	case 0xcd: cmp_aba_full(); break;
	case 0xce: dec_aba_full(); break;
	case 0xcf: dcp_aba_full(); break;
	case 0xd0: bne_rel_full(); break;
	case 0xd1: cmp_idy_full(); break;
	case 0xd2: kil_non_full(); break;
	case 0xd3: dcp_idy_full(); break;
	case 0xd4: nop_zpx_full(); break;
	case 0xd5: cmp_zpx_full(); break;
	case 0xd6: dec_zpx_full(); break;
	case 0xd7: dcp_zpx_full(); break;
	case 0xd8: cld_imp_full(); break;
	case 0xd9: cmp_aby_full(); break;
	case 0xda: nop_imp_full(); break;
	case 0xdb: dcp_aby_full(); break;
	case 0xdc: nop_abx_full(); break;
	case 0xdd: cmp_abx_full(); break;
	case 0xde: dec_abx_full(); break;
	case 0xdf: dcp_abx_full(); break;
	case 0xe0: cpx_imm_full(); break;
	case 0xe1: sbc_idx_full(); break;
	case 0xe2: nop_imm_full(); break;
	case 0xe3: isb_idx_full(); break;
	case 0xe4: cpx_zpg_full(); break;
	case 0xe5: sbc_zpg_full(); break;
	case 0xe6: inc_zpg_full(); break;
	case 0xe7: isb_zpg_full(); break;
	case 0xe8: inx_imp_full(); break;
	case 0xe9: sbc_imm_full(); break;
	case 0xea: nop_imp_full(); break;
	case 0xeb: sbc_imm_full(); break;
	case 0xec: cpx_aba_full(); break;
	case 0xed: sbc_aba_full(); break;
	case 0xee: inc_aba_full(); break;
	case 0xef: isb_aba_full(); break;
	case 0xf0: beq_rel_full(); break;
	case 0xf1: sbc_idy_full(); break;
	case 0xf2: kil_non_full(); break;
	case 0xf3: isb_idy_full(); break;
	case 0xf4: nop_zpx_full(); break;
	case 0xf5: sbc_zpx_full(); break;
	case 0xf6: inc_zpx_full(); break;
	case 0xf7: isb_zpx_full(); break;
	case 0xf8: sed_imp_full(); break;
	case 0xf9: sbc_aby_full(); break;
	case 0xfa: nop_imp_full(); break;
	case 0xfb: isb_aby_full(); break;
	case 0xfc: nop_abx_full(); break;
	case 0xfd: sbc_abx_full(); break;
	case 0xfe: inc_abx_full(); break;
	case 0xff: isb_abx_full(); break;
	case STATE_RESET: reset_full(); break;
	}
}

void m6502_device::do_exec_partial()
{
	switch(m_inst_state) {

	case 0x00: brk_imp_partial(); break;
	case 0x01: ora_idx_partial(); break;
	case 0x02: kil_non_partial(); break;
	case 0x03: slo_idx_partial(); break;
	case 0x04: nop_zpg_partial(); break;
	case 0x05: ora_zpg_partial(); break;
	case 0x06: asl_zpg_partial(); break;
	case 0x07: slo_zpg_partial(); break;
	case 0x08: php_imp_partial(); break;
	case 0x09: ora_imm_partial(); break;
	case 0x0a: asl_acc_partial(); break;
	case 0x0b: anc_imm_partial(); break;
	case 0x0c: nop_aba_partial(); break;
	case 0x0d: ora_aba_partial(); break;
	case 0x0e: asl_aba_partial(); break;
	case 0x0f: slo_aba_partial(); break;
	case 0x10: bpl_rel_partial(); break;
	case 0x11: ora_idy_partial(); break;
	case 0x12: kil_non_partial(); break;
	case 0x13: slo_idy_partial(); break;
	case 0x14: nop_zpx_partial(); break;
	case 0x15: ora_zpx_partial(); break;
	case 0x16: asl_zpx_partial(); break;
	case 0x17: slo_zpx_partial(); break;
	case 0x18: clc_imp_partial(); break;
	case 0x19: ora_aby_partial(); break;
	case 0x1a: nop_imp_partial(); break;
	case 0x1b: slo_aby_partial(); break;
	case 0x1c: nop_abx_partial(); break;
	case 0x1d: ora_abx_partial(); break;
	case 0x1e: asl_abx_partial(); break;
	case 0x1f: slo_abx_partial(); break;
	case 0x20: jsr_adr_partial(); break;
	case 0x21: and_idx_partial(); break;
	case 0x22: kil_non_partial(); break;
	case 0x23: rla_idx_partial(); break;
	case 0x24: bit_zpg_partial(); break;
	case 0x25: and_zpg_partial(); break;
	case 0x26: rol_zpg_partial(); break;
	case 0x27: rla_zpg_partial(); break;
	case 0x28: plp_imp_partial(); break;
	case 0x29: and_imm_partial(); break;
	case 0x2a: rol_acc_partial(); break;
	case 0x2b: anc_imm_partial(); break;
	case 0x2c: bit_aba_partial(); break;
	case 0x2d: and_aba_partial(); break;
	case 0x2e: rol_aba_partial(); break;
	case 0x2f: rla_aba_partial(); break;
	case 0x30: bmi_rel_partial(); break;
	case 0x31: and_idy_partial(); break;
	case 0x32: kil_non_partial(); break;
	case 0x33: rla_idy_partial(); break;
	case 0x34: nop_zpx_partial(); break;
	case 0x35: and_zpx_partial(); break;
	case 0x36: rol_zpx_partial(); break;
	case 0x37: rla_zpx_partial(); break;
	case 0x38: sec_imp_partial(); break;
	case 0x39: and_aby_partial(); break;
	case 0x3a: nop_imp_partial(); break;
	case 0x3b: rla_aby_partial(); break;
	case 0x3c: nop_abx_partial(); break;
	case 0x3d: and_abx_partial(); break;
	case 0x3e: rol_abx_partial(); break;
	case 0x3f: rla_abx_partial(); break;
	case 0x40: rti_imp_partial(); break;
	case 0x41: eor_idx_partial(); break;
	case 0x42: kil_non_partial(); break;
	case 0x43: sre_idx_partial(); break;
	case 0x44: nop_zpg_partial(); break;
	case 0x45: eor_zpg_partial(); break;
	case 0x46: lsr_zpg_partial(); break;
	case 0x47: sre_zpg_partial(); break;
	case 0x48: pha_imp_partial(); break;
	case 0x49: eor_imm_partial(); break;
	case 0x4a: lsr_acc_partial(); break;
	case 0x4b: asr_imm_partial(); break;
	case 0x4c: jmp_adr_partial(); break;
	case 0x4d: eor_aba_partial(); break;
	case 0x4e: lsr_aba_partial(); break;
	case 0x4f: sre_aba_partial(); break;
	case 0x50: bvc_rel_partial(); break;
	case 0x51: eor_idy_partial(); break;
	case 0x52: kil_non_partial(); break;
	case 0x53: sre_idy_partial(); break;
	case 0x54: nop_zpx_partial(); break;
	case 0x55: eor_zpx_partial(); break;
	case 0x56: lsr_zpx_partial(); break;
	case 0x57: sre_zpx_partial(); break;
	case 0x58: cli_imp_partial(); break;
	case 0x59: eor_aby_partial(); break;
	case 0x5a: nop_imp_partial(); break;
	case 0x5b: sre_aby_partial(); break;
	case 0x5c: nop_abx_partial(); break;
	case 0x5d: eor_abx_partial(); break;
	case 0x5e: lsr_abx_partial(); break;
	case 0x5f: sre_abx_partial(); break;
	case 0x60: rts_imp_partial(); break;
	case 0x61: adc_idx_partial(); break;
	case 0x62: kil_non_partial(); break;
	case 0x63: rra_idx_partial(); break;
	case 0x64: nop_zpg_partial(); break;
	case 0x65: adc_zpg_partial(); break;
	case 0x66: ror_zpg_partial(); break;
	case 0x67: rra_zpg_partial(); break;
	case 0x68: pla_imp_partial(); break;
	case 0x69: adc_imm_partial(); break;
	case 0x6a: ror_acc_partial(); break;
	case 0x6b: arr_imm_partial(); break;
	case 0x6c: jmp_ind_partial(); break;
	case 0x6d: adc_aba_partial(); break;
	case 0x6e: ror_aba_partial(); break;
	case 0x6f: rra_aba_partial(); break;
	case 0x70: bvs_rel_partial(); break;
	case 0x71: adc_idy_partial(); break;
	case 0x72: kil_non_partial(); break;
	case 0x73: rra_idy_partial(); break;
	case 0x74: nop_zpx_partial(); break;
	case 0x75: adc_zpx_partial(); break;
	case 0x76: ror_zpx_partial(); break;
	case 0x77: rra_zpx_partial(); break;
	case 0x78: sei_imp_partial(); break;
	case 0x79: adc_aby_partial(); break;
	case 0x7a: nop_imp_partial(); break;
	case 0x7b: rra_aby_partial(); break;
	case 0x7c: nop_abx_partial(); break;
	case 0x7d: adc_abx_partial(); break;
	case 0x7e: ror_abx_partial(); break;
	case 0x7f: rra_abx_partial(); break;
	case 0x80: nop_imm_partial(); break;
	case 0x81: sta_idx_partial(); break;
	case 0x82: nop_imm_partial(); break;
	case 0x83: sax_idx_partial(); break;
	case 0x84: sty_zpg_partial(); break;
	case 0x85: sta_zpg_partial(); break;
	case 0x86: stx_zpg_partial(); break;
	case 0x87: sax_zpg_partial(); break;
	case 0x88: dey_imp_partial(); break;
	case 0x89: nop_imm_partial(); break;
	case 0x8a: txa_imp_partial(); break;
	case 0x8b: ane_imm_partial(); break;
	case 0x8c: sty_aba_partial(); break;
	case 0x8d: sta_aba_partial(); break;
	case 0x8e: stx_aba_partial(); break;
	case 0x8f: sax_aba_partial(); break;
	case 0x90: bcc_rel_partial(); break;
	case 0x91: sta_idy_partial(); break;
	case 0x92: kil_non_partial(); break;
	case 0x93: sha_idy_partial(); break;
	case 0x94: sty_zpx_partial(); break;
	case 0x95: sta_zpx_partial(); break;
	case 0x96: stx_zpy_partial(); break;
	case 0x97: sax_zpy_partial(); break;
	case 0x98: tya_imp_partial(); break;
	case 0x99: sta_aby_partial(); break;
	case 0x9a: txs_imp_partial(); break;
	case 0x9b: shs_aby_partial(); break;
	case 0x9c: shy_abx_partial(); break;
	case 0x9d: sta_abx_partial(); break;
	case 0x9e: shx_aby_partial(); break;
	case 0x9f: sha_aby_partial(); break;
	case 0xa0: ldy_imm_partial(); break;
	case 0xa1: lda_idx_partial(); break;
	case 0xa2: ldx_imm_partial(); break;
	case 0xa3: lax_idx_partial(); break;
	case 0xa4: ldy_zpg_partial(); break;
	case 0xa5: lda_zpg_partial(); break;
	case 0xa6: ldx_zpg_partial(); break;
	case 0xa7: lax_zpg_partial(); break;
	case 0xa8: tay_imp_partial(); break;
	case 0xa9: lda_imm_partial(); break;
	case 0xaa: tax_imp_partial(); break;
	case 0xab: lxa_imm_partial(); break;
	case 0xac: ldy_aba_partial(); break;
	case 0xad: lda_aba_partial(); break;
	case 0xae: ldx_aba_partial(); break;
	case 0xaf: lax_aba_partial(); break;
	case 0xb0: bcs_rel_partial(); break;
	case 0xb1: lda_idy_partial(); break;
	case 0xb2: kil_non_partial(); break;
	case 0xb3: lax_idy_partial(); break;
	case 0xb4: ldy_zpx_partial(); break;
	case 0xb5: lda_zpx_partial(); break;
	case 0xb6: ldx_zpy_partial(); break;
	case 0xb7: lax_zpy_partial(); break;
	case 0xb8: clv_imp_partial(); break;
	case 0xb9: lda_aby_partial(); break;
	case 0xba: tsx_imp_partial(); break;
	case 0xbb: las_aby_partial(); break;
	case 0xbc: ldy_abx_partial(); break;
	case 0xbd: lda_abx_partial(); break;
	case 0xbe: ldx_aby_partial(); break;
	case 0xbf: lax_aby_partial(); break;
	case 0xc0: cpy_imm_partial(); break;
	case 0xc1: cmp_idx_partial(); break;
	case 0xc2: nop_imm_partial(); break;
	case 0xc3: dcp_idx_partial(); break;
	case 0xc4: cpy_zpg_partial(); break;
	case 0xc5: cmp_zpg_partial(); break;
	case 0xc6: dec_zpg_partial(); break;
	case 0xc7: dcp_zpg_partial(); break;
	case 0xc8: iny_imp_partial(); break;
	case 0xc9: cmp_imm_partial(); break;
	case 0xca: dex_imp_partial(); break;
	case 0xcb: sbx_imm_partial(); break;
	case 0xcc: cpy_aba_partial(); break;
	case 0xcd: cmp_aba_partial(); break;
	case 0xce: dec_aba_partial(); break;
	case 0xcf: dcp_aba_partial(); break;
	case 0xd0: bne_rel_partial(); break;
	case 0xd1: cmp_idy_partial(); break;
	case 0xd2: kil_non_partial(); break;
	case 0xd3: dcp_idy_partial(); break;
	case 0xd4: nop_zpx_partial(); break;
	case 0xd5: cmp_zpx_partial(); break;
	case 0xd6: dec_zpx_partial(); break;
	case 0xd7: dcp_zpx_partial(); break;
	case 0xd8: cld_imp_partial(); break;
	case 0xd9: cmp_aby_partial(); break;
	case 0xda: nop_imp_partial(); break;
	case 0xdb: dcp_aby_partial(); break;
	case 0xdc: nop_abx_partial(); break;
	case 0xdd: cmp_abx_partial(); break;
	case 0xde: dec_abx_partial(); break;
	case 0xdf: dcp_abx_partial(); break;
	case 0xe0: cpx_imm_partial(); break;
	case 0xe1: sbc_idx_partial(); break;
	case 0xe2: nop_imm_partial(); break;
	case 0xe3: isb_idx_partial(); break;
	case 0xe4: cpx_zpg_partial(); break;
	case 0xe5: sbc_zpg_partial(); break;
	case 0xe6: inc_zpg_partial(); break;
	case 0xe7: isb_zpg_partial(); break;
	case 0xe8: inx_imp_partial(); break;
	case 0xe9: sbc_imm_partial(); break;
	case 0xea: nop_imp_partial(); break;
	case 0xeb: sbc_imm_partial(); break;
	case 0xec: cpx_aba_partial(); break;
	case 0xed: sbc_aba_partial(); break;
	case 0xee: inc_aba_partial(); break;
	case 0xef: isb_aba_partial(); break;
	case 0xf0: beq_rel_partial(); break;
	case 0xf1: sbc_idy_partial(); break;
	case 0xf2: kil_non_partial(); break;
	case 0xf3: isb_idy_partial(); break;
	case 0xf4: nop_zpx_partial(); break;
	case 0xf5: sbc_zpx_partial(); break;
	case 0xf6: inc_zpx_partial(); break;
	case 0xf7: isb_zpx_partial(); break;
	case 0xf8: sed_imp_partial(); break;
	case 0xf9: sbc_aby_partial(); break;
	case 0xfa: nop_imp_partial(); break;
	case 0xfb: isb_aby_partial(); break;
	case 0xfc: nop_abx_partial(); break;
	case 0xfd: sbc_abx_partial(); break;
	case 0xfe: inc_abx_partial(); break;
	case 0xff: isb_abx_partial(); break;
	case STATE_RESET: reset_partial(); break;
	}
}

