const h8_disassembler::disasm_entry h8_disassembler::disasm_entries[] = {
	{ 0, 0x00000000, 0x0000ffff, 0x0000, 0x0000, "nop", DASM_none, DASM_none, 2 }, // needed
	{ 0, 0x00000180, 0x0000ffff, 0x0000, 0x0000, "sleep", DASM_none, DASM_none, 2 }, // needed
	{ 0, 0x00000200, 0x0000fff0, 0x0000, 0x0000, "stc", DASM_ccr, DASM_r8l, 2 }, // needed
	{ 0, 0x00000300, 0x0000fff0, 0x0000, 0x0000, "ldc", DASM_r8l, DASM_ccr, 2 }, // needed
	{ 0, 0x00000400, 0x0000ff00, 0x0000, 0x0000, "orc", DASM_imm8, DASM_ccr, 2 }, // needed
	{ 0, 0x00000500, 0x0000ff00, 0x0000, 0x0000, "xorc", DASM_imm8, DASM_ccr, 2 }, // needed
	{ 0, 0x00000600, 0x0000ff00, 0x0000, 0x0000, "andc", DASM_imm8, DASM_ccr, 2 }, // needed
	{ 0, 0x00000700, 0x0000ff00, 0x0000, 0x0000, "ldc", DASM_imm8, DASM_ccr, 2 }, // needed
	{ 0, 0x00000800, 0x0000ff00, 0x0000, 0x0000, "add.b", DASM_r8h, DASM_r8l, 2 }, // needed
	{ 0, 0x00000900, 0x0000ff88, 0x0000, 0x0000, "add.w", DASM_r16h, DASM_r16l, 2 }, // needed
	{ 0, 0x00000a00, 0x0000fff0, 0x0000, 0x0000, "inc.b", DASM_one, DASM_r8l, 2 }, // needed
	{ 0, 0x00000b00, 0x0000fff8, 0x0000, 0x0000, "adds.l", DASM_one, DASM_r16l, 2 }, // needed
	{ 0, 0x00000b80, 0x0000fff8, 0x0000, 0x0000, "adds.l", DASM_two, DASM_r16l, 2 }, // needed
	{ 0, 0x00000c00, 0x0000ff00, 0x0000, 0x0000, "mov.b", DASM_r8h, DASM_r8l, 2 }, // needed
	{ 0, 0x00000d00, 0x0000ff88, 0x0000, 0x0000, "mov.w", DASM_r16h, DASM_r16l, 2 }, // needed
	{ 0, 0x00000e00, 0x0000ff00, 0x0000, 0x0000, "addx.b", DASM_r8h, DASM_r8l, 2 }, // needed
	{ 0, 0x00000f00, 0x0000fff0, 0x0000, 0x0000, "daa.b", DASM_r8l, DASM_none, 2 }, // needed
	{ 0, 0x00001000, 0x0000fff0, 0x0000, 0x0000, "shll.b", DASM_r8l, DASM_none, 2 }, // needed
	{ 0, 0x00001080, 0x0000fff0, 0x0000, 0x0000, "shal.b", DASM_r8l, DASM_none, 2 }, // needed
	{ 0, 0x00001100, 0x0000fff0, 0x0000, 0x0000, "shlr.b", DASM_r8l, DASM_none, 2 }, // needed
	{ 0, 0x00001180, 0x0000fff0, 0x0000, 0x0000, "shar.b", DASM_r8l, DASM_none, 2 }, // needed
	{ 0, 0x00001200, 0x0000fff0, 0x0000, 0x0000, "rotxl.b", DASM_r8l, DASM_none, 2 }, // needed
	{ 0, 0x00001280, 0x0000fff0, 0x0000, 0x0000, "rotl.b", DASM_r8l, DASM_none, 2 }, // needed
	{ 0, 0x00001300, 0x0000fff0, 0x0000, 0x0000, "rotxr.b", DASM_r8l, DASM_none, 2 }, // needed
	{ 0, 0x00001380, 0x0000fff0, 0x0000, 0x0000, "rotr.b", DASM_r8l, DASM_none, 2 }, // needed
	{ 0, 0x00001400, 0x0000ff00, 0x0000, 0x0000, "or.b", DASM_r8h, DASM_r8l, 2 }, // needed
	{ 0, 0x00001500, 0x0000ff00, 0x0000, 0x0000, "xor.b", DASM_r8h, DASM_r8l, 2 }, // needed
	{ 0, 0x00001600, 0x0000ff00, 0x0000, 0x0000, "and.b", DASM_r8h, DASM_r8l, 2 }, // needed
	{ 0, 0x00001700, 0x0000fff0, 0x0000, 0x0000, "not.b", DASM_r8l, DASM_none, 2 }, // needed
	{ 0, 0x00001780, 0x0000fff0, 0x0000, 0x0000, "neg.b", DASM_r8l, DASM_none, 2 }, // needed
	{ 0, 0x00001800, 0x0000ff00, 0x0000, 0x0000, "sub.b", DASM_r8h, DASM_r8l, 2 }, // needed
	{ 0, 0x00001900, 0x0000ff88, 0x0000, 0x0000, "sub.w", DASM_r16h, DASM_r16l, 2 }, // needed
	{ 0, 0x00001a00, 0x0000fff0, 0x0000, 0x0000, "dec.b", DASM_one, DASM_r8l, 2 }, // needed
	{ 0, 0x00001b00, 0x0000fff8, 0x0000, 0x0000, "subs.l", DASM_one, DASM_r16l, 2 }, // needed
	{ 0, 0x00001b80, 0x0000fff8, 0x0000, 0x0000, "subs.l", DASM_two, DASM_r16l, 2 }, // needed
	{ 0, 0x00001c00, 0x0000ff00, 0x0000, 0x0000, "cmp.b", DASM_r8h, DASM_r8l, 2 }, // needed
	{ 0, 0x00001d00, 0x0000ff88, 0x0000, 0x0000, "cmp.w", DASM_r16h, DASM_r16l, 2 }, // needed
	{ 0, 0x00001e00, 0x0000ff00, 0x0000, 0x0000, "subx.b", DASM_r8h, DASM_r8l, 2 }, // needed
	{ 0, 0x00001f00, 0x0000fff0, 0x0000, 0x0000, "das.b", DASM_r8l, DASM_none, 2 }, // needed
	{ 0, 0x00002000, 0x0000f000, 0x0000, 0x0000, "mov.b", DASM_abs8, DASM_r8u, 2 }, // needed
	{ 0, 0x00003000, 0x0000f000, 0x0000, 0x0000, "mov.b", DASM_r8u, DASM_abs8, 2 }, // needed
	{ 0, 0x00004000, 0x0000ff00, 0x0000, 0x0000, "bt", DASM_rel8, DASM_none, 2 }, // needed
	{ 0, 0x00004100, 0x0000ff00, 0x0000, 0x0000, "bf", DASM_rel8, DASM_none, 2 }, // needed
	{ 0, 0x00004200, 0x0000ff00, 0x0000, 0x0000, "bhi", DASM_rel8, DASM_none, 2 | STEP_COND }, // needed
	{ 0, 0x00004300, 0x0000ff00, 0x0000, 0x0000, "bls", DASM_rel8, DASM_none, 2 | STEP_COND }, // needed
	{ 0, 0x00004400, 0x0000ff00, 0x0000, 0x0000, "bcc", DASM_rel8, DASM_none, 2 | STEP_COND }, // needed
	{ 0, 0x00004500, 0x0000ff00, 0x0000, 0x0000, "bcs", DASM_rel8, DASM_none, 2 | STEP_COND }, // needed
	{ 0, 0x00004600, 0x0000ff00, 0x0000, 0x0000, "bne", DASM_rel8, DASM_none, 2 | STEP_COND }, // needed
	{ 0, 0x00004700, 0x0000ff00, 0x0000, 0x0000, "beq", DASM_rel8, DASM_none, 2 | STEP_COND }, // needed
	{ 0, 0x00004800, 0x0000ff00, 0x0000, 0x0000, "bvc", DASM_rel8, DASM_none, 2 | STEP_COND }, // needed
	{ 0, 0x00004900, 0x0000ff00, 0x0000, 0x0000, "bvs", DASM_rel8, DASM_none, 2 | STEP_COND }, // needed
	{ 0, 0x00004a00, 0x0000ff00, 0x0000, 0x0000, "bpl", DASM_rel8, DASM_none, 2 | STEP_COND }, // needed
	{ 0, 0x00004b00, 0x0000ff00, 0x0000, 0x0000, "bmi", DASM_rel8, DASM_none, 2 | STEP_COND }, // needed
	{ 0, 0x00004c00, 0x0000ff00, 0x0000, 0x0000, "bge", DASM_rel8, DASM_none, 2 | STEP_COND }, // needed
	{ 0, 0x00004d00, 0x0000ff00, 0x0000, 0x0000, "blt", DASM_rel8, DASM_none, 2 | STEP_COND }, // needed
	{ 0, 0x00004e00, 0x0000ff00, 0x0000, 0x0000, "bgt", DASM_rel8, DASM_none, 2 | STEP_COND }, // needed
	{ 0, 0x00004f00, 0x0000ff00, 0x0000, 0x0000, "ble", DASM_rel8, DASM_none, 2 | STEP_COND }, // needed
	{ 0, 0x00005000, 0x0000ff08, 0x0000, 0x0000, "mulxu.b", DASM_r8h, DASM_r16l, 2 }, // needed
	{ 0, 0x00005100, 0x0000ff08, 0x0000, 0x0000, "divxu.b", DASM_r8h, DASM_r16l, 2 }, // needed
	{ 0, 0x00005470, 0x0000ffff, 0x0000, 0x0000, "rts", DASM_none, DASM_none, 2 | STEP_OUT }, // needed
	{ 0, 0x00005500, 0x0000ff00, 0x0000, 0x0000, "bsr", DASM_rel8, DASM_none, 2 | STEP_OVER }, // needed
	{ 0, 0x00005670, 0x0000ffff, 0x0000, 0x0000, "rte", DASM_none, DASM_none, 2 | STEP_OUT }, // needed
	{ 0, 0x00005900, 0x0000ff8f, 0x0000, 0x0000, "jmp", DASM_r16h, DASM_none, 2 }, // needed
	{ 0, 0x00005a00, 0x0000ffff, 0x0000, 0x0000, "jmp", DASM_abs16e, DASM_none, 4 }, // needed
	{ 0, 0x00005b00, 0x0000ff00, 0x0000, 0x0000, "jmp", DASM_abs8i, DASM_none, 2 }, // needed
	{ 0, 0x00005d00, 0x0000ff8f, 0x0000, 0x0000, "jsr", DASM_r16h, DASM_none, 2 | STEP_OVER }, // needed
	{ 0, 0x00005e00, 0x0000ffff, 0x0000, 0x0000, "jsr", DASM_abs16e, DASM_none, 4 | STEP_OVER }, // needed
	{ 0, 0x00005f00, 0x0000ff00, 0x0000, 0x0000, "jsr", DASM_abs8i, DASM_none, 2 | STEP_OVER }, // needed
	{ 0, 0x00006000, 0x0000ff00, 0x0000, 0x0000, "bset", DASM_r8h, DASM_r8l, 2 }, // needed
	{ 0, 0x00006100, 0x0000ff00, 0x0000, 0x0000, "bnot", DASM_r8h, DASM_r8l, 2 }, // needed
	{ 0, 0x00006200, 0x0000ff00, 0x0000, 0x0000, "bclr", DASM_r8h, DASM_r8l, 2 }, // needed
	{ 0, 0x00006300, 0x0000ff00, 0x0000, 0x0000, "btst", DASM_r8h, DASM_r8l, 2 }, // needed
	{ 0, 0x00006700, 0x0000ff80, 0x0000, 0x0000, "bst", DASM_imm3, DASM_r8l, 2 }, // needed
	{ 0, 0x00006780, 0x0000ff80, 0x0000, 0x0000, "bist", DASM_imm3, DASM_r8l, 2 }, // needed
	{ 0, 0x00006800, 0x0000ff80, 0x0000, 0x0000, "mov.b", DASM_r16ih, DASM_r8l, 2 }, // needed
	{ 0, 0x00006880, 0x0000ff80, 0x0000, 0x0000, "mov.b", DASM_r8l, DASM_r16ih, 2 }, // needed
	{ 0, 0x00006900, 0x0000ff88, 0x0000, 0x0000, "mov.w", DASM_r16ih, DASM_r16l, 2 }, // needed
	{ 0, 0x00006980, 0x0000ff88, 0x0000, 0x0000, "mov.w", DASM_r16l, DASM_r16ih, 2 }, // needed
	{ 0, 0x00006a00, 0x0000fff0, 0x0000, 0x0000, "mov.b", DASM_abs16, DASM_r8l, 4 }, // needed
	{ 0, 0x00006a40, 0x0000fff0, 0x0000, 0x0000, "movfpe", DASM_abs16, DASM_r8l, 4 }, // needed
	{ 0, 0x00006a80, 0x0000fff0, 0x0000, 0x0000, "mov.b", DASM_r8l, DASM_abs16, 4 }, // needed
	{ 0, 0x00006ac0, 0x0000fff0, 0x0000, 0x0000, "movtpe", DASM_r8l, DASM_abs16, 4 }, // needed
	{ 0, 0x00006b00, 0x0000fff8, 0x0000, 0x0000, "mov.w", DASM_abs16, DASM_r16l, 4 }, // needed
	{ 0, 0x00006b80, 0x0000fff8, 0x0000, 0x0000, "mov.w", DASM_r16l, DASM_abs16, 4 }, // needed
	{ 0, 0x00006c00, 0x0000ff80, 0x0000, 0x0000, "mov.b", DASM_r16ph, DASM_r8l, 2 }, // needed
	{ 0, 0x00006c80, 0x0000ff80, 0x0000, 0x0000, "mov.b", DASM_r8l, DASM_pr16h, 2 }, // needed
	{ 0, 0x00006d00, 0x0000ff88, 0x0000, 0x0000, "mov.w", DASM_r16ph, DASM_r16l, 2 }, // needed
	{ 0, 0x00006d80, 0x0000ff88, 0x0000, 0x0000, "mov.w", DASM_r16l, DASM_pr16h, 2 }, // needed
	{ 0, 0x00006e00, 0x0000ff80, 0x0000, 0x0000, "mov.b", DASM_r16d16h, DASM_r8l, 4 }, // needed
	{ 0, 0x00006e80, 0x0000ff80, 0x0000, 0x0000, "mov.b", DASM_r8l, DASM_r16d16h, 4 }, // needed
	{ 0, 0x00006f00, 0x0000ff88, 0x0000, 0x0000, "mov.w", DASM_r16d16h, DASM_r16l, 4 }, // needed
	{ 0, 0x00006f80, 0x0000ff88, 0x0000, 0x0000, "mov.w", DASM_r16l, DASM_r16d16h, 4 }, // needed
	{ 0, 0x00007000, 0x0000ff80, 0x0000, 0x0000, "bset", DASM_imm3, DASM_r8l, 2 }, // needed
	{ 0, 0x00007100, 0x0000ff80, 0x0000, 0x0000, "bnot", DASM_imm3, DASM_r8l, 2 }, // needed
	{ 0, 0x00007200, 0x0000ff80, 0x0000, 0x0000, "bclr", DASM_imm3, DASM_r8l, 2 }, // needed
	{ 0, 0x00007300, 0x0000ff80, 0x0000, 0x0000, "btst", DASM_imm3, DASM_r8l, 2 }, // needed
	{ 0, 0x00007400, 0x0000ff80, 0x0000, 0x0000, "bor", DASM_imm3, DASM_r8l, 2 }, // needed
	{ 0, 0x00007480, 0x0000ff80, 0x0000, 0x0000, "bior", DASM_imm3, DASM_r8l, 2 }, // needed
	{ 0, 0x00007500, 0x0000ff80, 0x0000, 0x0000, "bxor", DASM_imm3, DASM_r8l, 2 }, // needed
	{ 0, 0x00007580, 0x0000ff80, 0x0000, 0x0000, "bixor", DASM_imm3, DASM_r8l, 2 }, // needed
	{ 0, 0x00007600, 0x0000ff80, 0x0000, 0x0000, "band", DASM_imm3, DASM_r8l, 2 }, // needed
	{ 0, 0x00007680, 0x0000ff80, 0x0000, 0x0000, "biand", DASM_imm3, DASM_r8l, 2 }, // needed
	{ 0, 0x00007700, 0x0000ff80, 0x0000, 0x0000, "bld", DASM_imm3, DASM_r8l, 2 }, // needed
	{ 0, 0x00007780, 0x0000ff80, 0x0000, 0x0000, "bild", DASM_imm3, DASM_r8l, 2 }, // needed
	{ 0, 0x00007900, 0x0000fff8, 0x0000, 0x0000, "mov.w", DASM_imm16, DASM_r16l, 4 }, // needed
	{ 1, 0x7b5c598f, 0xffffffff, 0x0000, 0x0000, "eepmov.b", DASM_none, DASM_none, 4 }, // needed
	{ 1, 0x7c006300, 0xff8fff0f, 0x0000, 0x0000, "btst", DASM_r8h, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7c007300, 0xff8fff8f, 0x0000, 0x0000, "btst", DASM_imm3, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7c007400, 0xff8fff8f, 0x0000, 0x0000, "bor", DASM_imm3, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7c007480, 0xff8fff8f, 0x0000, 0x0000, "bior", DASM_imm3, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7c007500, 0xff8fff8f, 0x0000, 0x0000, "bxor", DASM_imm3, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7c007580, 0xff8fff8f, 0x0000, 0x0000, "bixor", DASM_imm3, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7c007600, 0xff8fff8f, 0x0000, 0x0000, "band", DASM_imm3, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7c007680, 0xff8fff8f, 0x0000, 0x0000, "biand", DASM_imm3, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7c007700, 0xff8fff8f, 0x0000, 0x0000, "bld", DASM_imm3, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7c007780, 0xff8fff8f, 0x0000, 0x0000, "bild", DASM_imm3, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7d006000, 0xff8fff0f, 0x0000, 0x0000, "bset", DASM_r8h, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7d006100, 0xff8fff0f, 0x0000, 0x0000, "bnot", DASM_r8h, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7d006200, 0xff8fff0f, 0x0000, 0x0000, "bclr", DASM_r8h, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7d006700, 0xff8fff8f, 0x0000, 0x0000, "bst", DASM_imm3, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7d006780, 0xff8fff8f, 0x0000, 0x0000, "bist", DASM_imm3, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7d007000, 0xff8fff8f, 0x0000, 0x0000, "bset", DASM_imm3, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7d007100, 0xff8fff8f, 0x0000, 0x0000, "bnot", DASM_imm3, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7d007200, 0xff8fff8f, 0x0000, 0x0000, "bclr", DASM_imm3, DASM_r16ihh, 4 }, // needed
	{ 1, 0x7e006300, 0xff00ff0f, 0x0000, 0x0000, "btst", DASM_r8h, DASM_abs8, 4 }, // needed
	{ 1, 0x7e007300, 0xff00ff8f, 0x0000, 0x0000, "btst", DASM_imm3, DASM_abs8, 4 }, // needed
	{ 1, 0x7e007400, 0xff00ff8f, 0x0000, 0x0000, "bor", DASM_imm3, DASM_abs8, 4 }, // needed
	{ 1, 0x7e007480, 0xff00ff8f, 0x0000, 0x0000, "bior", DASM_imm3, DASM_abs8, 4 }, // needed
	{ 1, 0x7e007500, 0xff00ff8f, 0x0000, 0x0000, "bxor", DASM_imm3, DASM_abs8, 4 }, // needed
	{ 1, 0x7e007580, 0xff00ff8f, 0x0000, 0x0000, "bixor", DASM_imm3, DASM_abs8, 4 }, // needed
	{ 1, 0x7e007600, 0xff00ff8f, 0x0000, 0x0000, "band", DASM_imm3, DASM_abs8, 4 }, // needed
	{ 1, 0x7e007680, 0xff00ff8f, 0x0000, 0x0000, "biand", DASM_imm3, DASM_abs8, 4 }, // needed
	{ 1, 0x7e007700, 0xff00ff8f, 0x0000, 0x0000, "bld", DASM_imm3, DASM_abs8, 4 }, // needed
	{ 1, 0x7e007780, 0xff00ff8f, 0x0000, 0x0000, "bild", DASM_imm3, DASM_abs8, 4 }, // needed
	{ 1, 0x7f006000, 0xff00ff0f, 0x0000, 0x0000, "bset", DASM_r8h, DASM_abs8, 4 }, // needed
	{ 1, 0x7f006100, 0xff00ff0f, 0x0000, 0x0000, "bnot", DASM_r8h, DASM_abs8, 4 }, // needed
	{ 1, 0x7f006200, 0xff00ff0f, 0x0000, 0x0000, "bclr", DASM_r8h, DASM_abs8, 4 }, // needed
	{ 1, 0x7f006700, 0xff00ff8f, 0x0000, 0x0000, "bst", DASM_imm3, DASM_abs8, 4 }, // needed
	{ 1, 0x7f006780, 0xff00ff8f, 0x0000, 0x0000, "bist", DASM_imm3, DASM_abs8, 4 }, // needed
	{ 1, 0x7f007000, 0xff00ff8f, 0x0000, 0x0000, "bset", DASM_imm3, DASM_abs8, 4 }, // needed
	{ 1, 0x7f007100, 0xff00ff8f, 0x0000, 0x0000, "bnot", DASM_imm3, DASM_abs8, 4 }, // needed
	{ 1, 0x7f007200, 0xff00ff8f, 0x0000, 0x0000, "bclr", DASM_imm3, DASM_abs8, 4 }, // needed
	{ 0, 0x00008000, 0x0000f000, 0x0000, 0x0000, "add.b", DASM_imm8, DASM_r8u, 2 }, // needed
	{ 0, 0x00009000, 0x0000f000, 0x0000, 0x0000, "addx.b", DASM_imm8, DASM_r8u, 2 }, // needed
	{ 0, 0x0000a000, 0x0000f000, 0x0000, 0x0000, "cmp.b", DASM_imm8, DASM_r8u, 2 }, // needed
	{ 0, 0x0000b000, 0x0000f000, 0x0000, 0x0000, "subx.b", DASM_imm8, DASM_r8u, 2 }, // needed
	{ 0, 0x0000c000, 0x0000f000, 0x0000, 0x0000, "or.b", DASM_imm8, DASM_r8u, 2 }, // needed
	{ 0, 0x0000d000, 0x0000f000, 0x0000, 0x0000, "xor.b", DASM_imm8, DASM_r8u, 2 }, // needed
	{ 0, 0x0000e000, 0x0000f000, 0x0000, 0x0000, "and.b", DASM_imm8, DASM_r8u, 2 }, // needed
	{ 0, 0x0000f000, 0x0000f000, 0x0000, 0x0000, "mov.b", DASM_imm8, DASM_r8u, 2 }, // needed
	{ 0, 0, 0, 0, 0, "illegal", 0, 0, 2 },
};

