// dynaCOP1.c: Compile floating point operation codes.


// 1964 Copyright (C) 1999-2004 Joel Middendorf, <schibo@emulation64.com>.  This
// program is free software;  you can redistribute it and/or modify it under the
// terms of the GNU  General Public  License as  published by  the Free Software
// Foundation; either version 2 of the License,  or (at your option)  any  later
// version.  This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.  You should have received a copy of the GNU  General Public  License
// along with this program; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place  -  Suite  330,  Boston, MA  02111-1307,  USA. To contact the
// authors: email: schibo@emulation64.com, rice1964@yahoo.com

#include "../stdafx.h"

//#define SAFE_LOADSTORE_FPU 1

x86regtyp xFS[1], xFT[1], xFD[1];
void SSE_FlushAllRegisters(int);
int SSE_CheckWhereIsMipsReg(int mips_reg);
void SSE_FlushRegister(int To_XMM_Or_Memory, int k);
void SSE_MapRegister(int To_XMM_Or_Memory, x86regtyp *Conditions, int keep2, int keep3);

//#define _SAFTY_COP1_(x) INTERPRET(x); return;
#define _SAFTY_COP1_(x) \
    SSE_FlushAllRegisters(FLUSH_TO_MEMORY);

//Pipeline clocks - cheezy estimated averages
#define DEFAULT_FPU_STEP 1
#define DEFAULT_C_COND_STEP 1
#define DEFAULT_ROUNDING_STEP 1
#define DEFAULT_ARITHMATIC_STEP 1
#define DEFAULT_MUL_S 4
#define DEFAULT_MUL_D 8
#define DEFAULT_DIV_S_STEP 28
#define DEFAULT_DIV_D_STEP 80
#define DEFAULT_SQRT_S_STEP 10
#define DEFAULT_SQRT_D_STEP 56


extern void OpcodePrologue(int clocks, void (__fastcall *FuncAddress)(unsigned __int32 ), int Use32, OP_PARAMS);
extern void CommittTheSource(int NeedsFlush);
extern unsigned int HardwareStart;
extern uint32 Experiment;
extern int iMXCSR;
extern int iMXCSR_FLOOR;
extern int iMXCSR_TRUNC;
extern x86regtyp fpureg[8];



/*
 =======================================================================================================================
    define _SAFTY_COP1_(x) INTERPRET(x); return;
 =======================================================================================================================
 */


#define MapFD	SSE_MapRegister(FLUSH_TO_MEMORY, xFD, xFS->mips_reg, xFT->mips_reg);
#define MapFS	SSE_MapRegister(FLUSH_TO_MEMORY, xFS, xFD->mips_reg, xFT->mips_reg);
#define MapFT	SSE_MapRegister(FLUSH_TO_MEMORY, xFT, xFS->mips_reg, xFD->mips_reg);


//_control87() and _isnan() are __cdecl, therefore, we need to push the map.
#define INTERPRET_NOFLUSH(OPCODE) \
    PushMap(); \
    MOV_ImmToReg(Reg_ECX, r.r_.code); \
    X86_CALL((uint32) & OPCODE); \
    PopMap();

#define INTERPRET(OPCODE) \
    FlushAllRegisters(FLUSH_TO_MEMORY); \
    MOV_ImmToReg(Reg_ECX, r.r_.code); \
    X86_CALL((uint32) & OPCODE);

extern void         r4300i_C_cond_fmt_s(uint32 Instruction);
extern void         reserved(OP_PARAMS);
extern void         HELP_cop1_bc1f(_u32 pc);
extern void         HELP_cop1_bc1t(_u32 pc);
extern void         HELP_cop1_bc1fl(_u32 pc);
extern void         HELP_cop1_bc1tl(_u32 pc);
extern void         LoadMipsCpuRegister(unsigned long iMipsReg, unsigned char iIntelReg1, unsigned char iIntelReg2);
extern void         Lea( uint32 rt, uint32 rs, uint32 imm);

extern MapConstant  ConstMap[NUM_CONSTS];
MapConstant         fpuConstMap[32];
extern x86regtyp    xRD[1];
extern x86regtyp    xRS[1];
extern x86regtyp    xRT[1];


void
SetFdFsFt32bit(OP_PARAMS)
{
	memset( xFD, 0, sizeof( xFD));
	memset( xFS, 0, sizeof( xFS));
	memset( xFT, 0, sizeof( xFT));
	xFD->mips_reg = __FD;
	xFS->mips_reg = __FS;
	xFT->mips_reg = __FT;
	xFD->Is32bit = 1;
	xFS->Is32bit = 1;
	xFT->Is32bit = 1;
}


extern void (*cop1_D_instruction[64]) (OP_PARAMS);
extern void    cop1_mfc1(OP_PARAMS);
extern void    cop1_dmfc1(OP_PARAMS);
extern void    cop1_cfc1(OP_PARAMS);
extern void    cop1_mtc1(OP_PARAMS);
extern void    cop1_dmtc1(OP_PARAMS);
extern void    cop1_ctc1(OP_PARAMS);
extern void    cop1_BC_instr(OP_PARAMS);
extern void    cop1_S_instr(OP_PARAMS);
extern void    cop1_D_instr(OP_PARAMS);
extern void    cop1_W_instr(OP_PARAMS);
extern void    cop1_L_instr(OP_PARAMS);
extern void    cop1_bc1f(OP_PARAMS);
extern void    cop1_bc1t(OP_PARAMS);
extern void    cop1_bc1fl(OP_PARAMS);
extern void    cop1_bc1tl(OP_PARAMS);
extern void    cop1_add_s(OP_PARAMS);
extern void    cop1_sub_s(OP_PARAMS);
extern void    cop1_mul_s(OP_PARAMS);
extern void    cop1_div_s(OP_PARAMS);
extern void    cop1_sqrt_s(OP_PARAMS);
extern void    cop1_abs_s(OP_PARAMS);
extern void    cop1_mov_s(OP_PARAMS);
extern void    cop1_neg_s(OP_PARAMS);
extern void    cop1_roundl_s(OP_PARAMS);
extern void    cop1_truncl_s(OP_PARAMS);
extern void    cop1_ceill_s(OP_PARAMS);
extern void    cop1_floorl_s(OP_PARAMS);
extern void    cop1_roundw_s(OP_PARAMS);
extern void    cop1_truncw_s(OP_PARAMS);
extern void    cop1_ceilw_s(OP_PARAMS);
extern void    cop1_floorw_s(OP_PARAMS);
extern void    cop1_cvtd_s(OP_PARAMS);
extern void    cop1_cvtw_s(OP_PARAMS);
extern void    cop1_cvtl_s(OP_PARAMS);
extern void    c_f_s(OP_PARAMS);
extern void    c_un_s(OP_PARAMS);
extern void    c_eq_s(OP_PARAMS);
extern void    c_ueq_s(OP_PARAMS);
extern void    c_olt_s(OP_PARAMS);
extern void    c_ult_s(OP_PARAMS);
extern void    c_ole_s(OP_PARAMS);
extern void    c_ule_s(OP_PARAMS);
extern void    c_sf_s(OP_PARAMS);
extern void    c_ngle_s(OP_PARAMS);
extern void    c_seq_s(OP_PARAMS);
extern void    c_ngl_s(OP_PARAMS);
extern void    c_lt_s(OP_PARAMS);
extern void    c_nge_s(OP_PARAMS);
extern void    c_le_s(OP_PARAMS);
extern void    c_ngt_s(OP_PARAMS);
extern void    cop1_add_d(OP_PARAMS);
extern void    cop1_sub_d(OP_PARAMS);
extern void    cop1_mul_d(OP_PARAMS);
extern void    cop1_div_d(OP_PARAMS);
extern void    cop1_sqrt_d(OP_PARAMS);
extern void    cop1_abs_d(OP_PARAMS);
extern void    cop1_mov_d(OP_PARAMS);
extern void    cop1_neg_d(OP_PARAMS);
extern void    cop1_roundl_d(OP_PARAMS);
extern void    cop1_truncl_d(OP_PARAMS);
extern void    cop1_ceill_d(OP_PARAMS);
extern void    cop1_floorl_d(OP_PARAMS);
extern void    cop1_roundw_d(OP_PARAMS);
extern void    cop1_truncw_d(OP_PARAMS);
extern void    cop1_ceilw_d(OP_PARAMS);
extern void    cop1_floorw_d(OP_PARAMS);
extern void    cop1_cvts_d(OP_PARAMS);
extern void    cop1_cvtw_d(OP_PARAMS);
extern void    cop1_cvtl_d(OP_PARAMS);
extern void    c_f_d(OP_PARAMS);
extern void    c_un_d(OP_PARAMS);
extern void    c_eq_d(OP_PARAMS);
extern void    c_ueq_d(OP_PARAMS);
extern void    c_olt_d(OP_PARAMS);
extern void    c_ult_d(OP_PARAMS);
extern void    c_ole_d(OP_PARAMS);
extern void    c_ule_d(OP_PARAMS);
extern void    c_sf_d(OP_PARAMS);
extern void    c_ngle_d(OP_PARAMS);
extern void    c_seq_d(OP_PARAMS);
extern void    c_ngl_d(OP_PARAMS);
extern void    c_lt_d(OP_PARAMS);
extern void    c_nge_d(OP_PARAMS);
extern void    c_le_d(OP_PARAMS);
extern void    c_ngt_d(OP_PARAMS);
extern void    cop1_cvts_w(OP_PARAMS);
extern void    cop1_cvtd_w(OP_PARAMS);
extern void    cop1_cvts_l(OP_PARAMS);
extern void    cop1_cvtd_l(OP_PARAMS);
extern void    c_s(OP_PARAMS);
extern void    c_d(OP_PARAMS);


extern int (*cop1_SCHECK_ODD[64]) (OP_PARAMS);
extern int (*cop1_DCHECK_ODD[64]) (OP_PARAMS);
extern int (*cop1_BCCHECK_ODD[32]) (OP_PARAMS);
extern int call_cop1_SCHECKODD(OP_PARAMS);
extern int call_cop1_DCHECKODD(OP_PARAMS);



//All this extra shit is unnecessar. It was when I was going insane to fix the cp1 unusable problem!!
int OddFs(OP_PARAMS)
{
    if (__FS&1)
        return 1;
    
    return 1;
}
int OddFdFs(OP_PARAMS)
{
    if ((__FD|__FS)&1)
        return 1;

    return 1;
}
int OddFdFsFt(OP_PARAMS)
{
    if ((__FD|__FS|__FT)&1)
        return 1;

    return 1;
}
int OddFsFt(OP_PARAMS)
{
    if ((__FS|__FT)&1)
        return 1;

    return 1;
}


int OddEmpty(OP_PARAMS)
{
    return 1;
}

/* \ Function Tables \ */
void (*cop1_Instruction[32]) (OP_PARAMS) =
{
    cop1_mfc1,     cop1_dmfc1,   cop1_cfc1, resrvd, cop1_mtc1,    cop1_dmtc1,   cop1_ctc1, resrvd,
    cop1_BC_instr, resrvd,       resrvd,    resrvd, resrvd,       resrvd,       resrvd,    resrvd,
    cop1_S_instr,  cop1_D_instr, resrvd,    resrvd, cop1_W_instr, cop1_L_instr, resrvd,    resrvd,
    resrvd,        resrvd,       resrvd,    resrvd, resrvd,       resrvd,       resrvd,    resrvd
};
int (*cop1_InstructionCHECK_ODD[32]) (OP_PARAMS) =
{
    OddFs,         OddFs,        OddFs,OddEmpty,OddFs,OddFs,OddFs,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    call_cop1_SCHECKODD,call_cop1_DCHECKODD,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty
//    cop1_mfc1,     cop1_dmfc1,   cop1_cfc1, resrvd, cop1_mtc1,    cop1_dmtc1,   cop1_ctc1, resrvd,
//    cop1_BC_instr, resrvd,       resrvd,    resrvd, resrvd,       resrvd,       resrvd,    resrvd,
//    cop1_S_instr,  cop1_D_instr, resrvd,    resrvd, cop1_W_instr, cop1_L_instr, resrvd,    resrvd,
//    resrvd,        resrvd,       resrvd,    resrvd, resrvd,       resrvd,       resrvd,    resrvd
};


void (*cop1_BC_instruction[]) (OP_PARAMS) =
{
    cop1_bc1f, cop1_bc1t, cop1_bc1fl, cop1_bc1tl, resrvd, resrvd, resrvd, resrvd,    
    resrvd,    resrvd,    resrvd,     resrvd,     resrvd, resrvd, resrvd, resrvd,
    resrvd,    resrvd,    resrvd,     resrvd,     resrvd, resrvd, resrvd, resrvd,
    resrvd,    resrvd,    resrvd,     resrvd,     resrvd, resrvd, resrvd, resrvd
};
int (*cop1_BCCHECK_ODD[]) (OP_PARAMS) =
{
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty

//    cop1_bc1f, cop1_bc1t, cop1_bc1fl, cop1_bc1tl, resrvd, resrvd, resrvd, resrvd,    
//    resrvd,    resrvd,    resrvd,     resrvd,     resrvd, resrvd, resrvd, resrvd,
//    resrvd,    resrvd,    resrvd,     resrvd,     resrvd, resrvd, resrvd, resrvd,
//    resrvd,    resrvd,    resrvd,     resrvd,     resrvd, resrvd, resrvd, resrvd
};

void (*cop1_S_instruction[]) (OP_PARAMS) =
{
    cop1_add_s,    cop1_sub_s,    cop1_mul_s,   cop1_div_s,    cop1_sqrt_s,   cop1_abs_s,    cop1_mov_s,   cop1_neg_s,
    cop1_roundl_s, cop1_truncl_s, cop1_ceill_s, cop1_floorl_s, cop1_roundw_s, cop1_truncw_s, cop1_ceilw_s, cop1_floorw_s,
    resrvd,        resrvd,        resrvd,       resrvd,        resrvd,        resrvd,        resrvd,       resrvd,
    resrvd,        resrvd,        resrvd,       resrvd,        resrvd,        resrvd,        resrvd,       resrvd,
    resrvd,        cop1_cvtd_s,   resrvd,       resrvd,        cop1_cvtw_s,   cop1_cvtl_s,   resrvd,       resrvd,
    resrvd,        resrvd,        resrvd,       resrvd,        resrvd,        resrvd,        resrvd,       resrvd,
    c_f_s,         c_un_s,        c_eq_s,       c_ueq_s,       c_olt_s,       c_ult_s,       c_ole_s,      c_ule_s,
    c_sf_s,        c_ngle_s,      c_seq_s,      c_ngl_s,       c_lt_s,        c_nge_s,       c_le_s,       c_ngt_s
};
int (*cop1_SCHECK_ODD[])(OP_PARAMS) =
{
    OddFdFsFt,OddFdFsFt,OddFdFsFt,OddFdFsFt,OddFdFs,OddFdFs,OddFdFs,OddFdFs,
    OddFdFs,OddFdFs,OddFdFs,OddFdFs,OddFdFs,OddFdFs,OddFdFs,OddFdFs,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddFsFt,OddFsFt,OddFsFt,OddFsFt,OddFsFt,OddFsFt,OddFsFt,OddFsFt,
    OddFsFt,OddFsFt,OddFsFt,OddFsFt,OddEmpty,OddFsFt,OddFsFt,OddFsFt

//    cop1_add_s,    cop1_sub_s,    cop1_mul_s,   cop1_div_s,    cop1_sqrt_s,   cop1_abs_s,    cop1_mov_s,   cop1_neg_s,
//    cop1_roundl_s, cop1_truncl_s, cop1_ceill_s, cop1_floorl_s, cop1_roundw_s, cop1_truncw_s, cop1_ceilw_s, cop1_floorw_s,
//    resrvd,        resrvd,        resrvd,       resrvd,        resrvd,        resrvd,        resrvd,       resrvd,
//    resrvd,        resrvd,        resrvd,       resrvd,        resrvd,        resrvd,        resrvd,       resrvd,
//    resrvd,        cop1_cvtd_s,   resrvd,       resrvd,        cop1_cvtw_s,   cop1_cvtl_s,   resrvd,       resrvd,
//    resrvd,        resrvd,        resrvd,       resrvd,        resrvd,        resrvd,        resrvd,       resrvd,
//    c_f_s,         c_un_s,        c_eq_s,       c_ueq_s,       c_olt_s,       c_ult_s,       c_ole_s,      c_ule_s,
//    c_sf_s,        c_ngle_s,      c_seq_s,      c_ngl_s,       c_lt_s,        c_nge_s,       c_le_s,       c_ngt_s
};


int call_cop1_SCHECKODD(OP_PARAMS)
{
    return cop1_SCHECK_ODD[__F](PASS_PARAMS);
}
int call_cop1_DCHECKODD(OP_PARAMS)
{
    return cop1_DCHECK_ODD[__F](PASS_PARAMS);
}

void (*cop1_D_instruction[64]) (OP_PARAMS) =
{
    cop1_add_d,    cop1_sub_d,    cop1_mul_d,   cop1_div_d,    cop1_sqrt_d,   cop1_abs_d,    cop1_mov_d,   cop1_neg_d,
    cop1_roundl_d, cop1_truncl_d, cop1_ceill_d, cop1_floorl_d, cop1_roundw_d, cop1_truncw_d, cop1_ceilw_d, cop1_floorw_d,
    resrvd,        resrvd,        resrvd,       resrvd,        resrvd,        resrvd,        resrvd,       resrvd,
    resrvd,        resrvd,        resrvd,       resrvd,        resrvd,        resrvd,        resrvd,       resrvd,
    cop1_cvts_d,   resrvd,        resrvd,       resrvd,        cop1_cvtw_d,   cop1_cvtl_d,   resrvd,       resrvd,
    resrvd,        resrvd,        resrvd,       resrvd,        resrvd,        resrvd,        resrvd,       resrvd,
    c_f_d,         c_un_d,        c_eq_d,       c_ueq_d,       c_olt_d,       c_ult_d,       c_ole_d,      c_ule_d,
    c_sf_d,        c_ngle_d,      c_seq_d,      c_ngl_d,       c_lt_d,        c_nge_d,       c_le_d,       c_ngt_d
};
int (*cop1_DCHECK_ODD[64]) (OP_PARAMS) =
{
    OddFdFsFt,OddFdFsFt,OddFdFsFt,OddFdFsFt,OddFdFs,OddFdFs,OddFdFs,OddFdFs,
    OddFdFs,OddFdFs,OddFdFs,OddFdFs,OddFdFs,OddFdFs,OddFdFs,OddFdFs,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddFsFt,OddFsFt,OddFsFt,OddFsFt,OddFsFt,OddFsFt,OddFsFt,OddFsFt,
    OddFsFt,OddFsFt,OddFsFt,OddFsFt,OddEmpty,OddFsFt,OddFsFt,OddFsFt

//    cop1_add_d,    cop1_sub_d,    cop1_mul_d,   cop1_div_d,    cop1_sqrt_d,   cop1_abs_d,    cop1_mov_d,   cop1_neg_d,
//    cop1_roundl_d, cop1_truncl_d, cop1_ceill_d, cop1_floorl_d, cop1_roundw_d, cop1_truncw_d, cop1_ceilw_d, cop1_floorw_d,
//    resrvd,        resrvd,        resrvd,       resrvd,        resrvd,        resrvd,        resrvd,       resrvd,
//    resrvd,        resrvd,        resrvd,       resrvd,        resrvd,        resrvd,        resrvd,       resrvd,
//    cop1_cvts_d,   resrvd,        resrvd,       resrvd,        cop1_cvtw_d,   cop1_cvtl_d,   resrvd,       resrvd,
//    resrvd,        resrvd,        resrvd,       resrvd,        resrvd,        resrvd,        resrvd,       resrvd,
//    c_f_d,         c_un_d,        c_eq_d,       c_ueq_d,       c_olt_d,       c_ult_d,       c_ole_d,      c_ule_d,
//    c_sf_d,        c_ngle_d,      c_seq_d,      c_ngl_d,       c_lt_d,        c_nge_d,       c_le_d,       c_ngt_d
};

void (*cop1_W_instruction[64]) (OP_PARAMS) =
{
    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
    cop1_cvts_w, cop1_cvtd_w, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd
};
int (*cop1_W_instructionCHECK_ODD[64]) (OP_PARAMS) =
{
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty

//    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
//    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
//    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
//    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
//    cop1_cvts_w, cop1_cvtd_w, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
//    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
//    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
//    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd
};

void (*cop1_L_instruction[]) (OP_PARAMS) =
{
    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
    cop1_cvts_l, cop1_cvtd_l, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd
};
int (*cop1_L_instructionCHECK_ODD[]) (OP_PARAMS) =
{
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,
    OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty,OddEmpty

//    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
//    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
//    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
//    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
//    cop1_cvts_l, cop1_cvtd_l, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
//    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
//    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
//    resrvd,      resrvd,      resrvd, resrvd, resrvd, resrvd, resrvd, resrvd
};

/*
 =======================================================================================================================
    \ CoProcessor1 (COP1) \ ?
    void (*cop1_Instruction[])(OP_PARAMS)= //
 =======================================================================================================================
 */

void cop1_mfc1(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    SSE_FlushAllRegisters(FLUSH_TO_MEMORY);
    OpcodePrologue(1, &r4300i_COP1_mfc1, 1, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;

    ConstMap[xRT->mips_reg].IsMapped = 0;
    xRT->IsDirty = 1;
    xRT->NoNeedToLoadTheLo = 1;
    MapRT;
    MOV_MemoryToReg((_u8) xRT->x86reg, ModRM_disp32, (unsigned long) &reg->fpr32[__FS<<Experiment]);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_dmfc1(OP_PARAMS)
{
    int RememberAssume32bit;

    cop1_with_exception(PASS_PARAMS);
    
    SSE_FlushAllRegisters(FLUSH_TO_MEMORY);    
    OpcodePrologue(1, &r4300i_COP1_dmfc1, 0, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;


    RememberAssume32bit = currentromoptions.Assume_32bit;
    currentromoptions.Assume_32bit = ASSUME_32BIT_NO;

    xRT->IsDirty = 1;
    xRT->NoNeedToLoadTheLo = 1;
    xRT->NoNeedToLoadTheHi = 1;
    MapRT;
    MOV_MemoryToReg((_u8) xRT->x86reg, ModRM_disp32, (unsigned long) &reg->fpr32[__FS<<Experiment]);
    MOV_MemoryToReg((_u8) xRT->HiWordLoc, ModRM_disp32, 4 + (unsigned long) &reg->fpr32[__FS<<Experiment]);

    currentromoptions.Assume_32bit = RememberAssume32bit;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_cfc1(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    SSE_FlushAllRegisters(FLUSH_TO_MEMORY);    
    OpcodePrologue(1, &r4300i_COP1_cfc1, 1, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;

    if(((__FS == 0) || (__FS == 31)))   /* This operation is only defined if */
    {
        ConstMap[xRT->mips_reg].IsMapped = 0;
        xRT->IsDirty = 1;
        MapRT;
        MOV_MemoryToReg((_u8) xRT->x86reg, ModRM_disp32, (unsigned long) &reg->COP1Con[__FS]);
    }
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_ctc1(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    SSE_FlushAllRegisters(FLUSH_TO_MEMORY);
    OpcodePrologue(1, &r4300i_COP1_ctc1, 1, PASS_PARAMS);


    INTERPRET(r4300i_COP1_ctc1); /* because I have added rounding control */
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_mtc1(OP_PARAMS)
{
    int where;

    cop1_with_exception(PASS_PARAMS);
    
    where = SSE_CheckWhereIsMipsReg(__FS);
    if (where > -1)
    {
        fpureg[where].IsDirty = 0;
        SSE_FlushRegister(FLUSH_TO_MEMORY, where);
    }

    OpcodePrologue(1, &r4300i_COP1_mtc1, 1, PASS_PARAMS);


    if(ConstMap[xRT->mips_reg].IsMapped == 0)
    {
        MapRT;
        fpuConstMap[__FS].IsMapped = 0;
        MOV_RegToMemory(1, (_u8) xRT->x86reg, ModRM_disp32, (unsigned long) &reg->fpr32[__FS<<Experiment]);
    }
    else
    {
        fpuConstMap[__FS].value = ConstMap[xRT->mips_reg].value;
        fpuConstMap[__FS].IsMapped = 1;
        MOV_ImmToMemory(1, ModRM_disp32, (unsigned long) &reg->fpr32[__FS<<Experiment], ConstMap[xRT->mips_reg].value);
    }
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_dmtc1(OP_PARAMS)
{
    int RememberAssume32bit;

    cop1_with_exception(PASS_PARAMS);

    SSE_FlushAllRegisters(FLUSH_TO_MEMORY);
    OpcodePrologue(1, &r4300i_COP1_dmtc1, 0, PASS_PARAMS);

     RememberAssume32bit = currentromoptions.Assume_32bit;
     currentromoptions.Assume_32bit = ASSUME_32BIT_NO;

     MapRT;
     MOV_RegToMemory(1, (_u8) xRT->x86reg, ModRM_disp32, (unsigned long) &reg->fpr32[__FS<<Experiment]);
     MOV_RegToMemory(1, (_u8) xRT->HiWordLoc, ModRM_disp32, 4 + (unsigned long) &reg->fpr32[__FS<<Experiment]);

     currentromoptions.Assume_32bit = RememberAssume32bit;

}

extern void COP1_instr(uint32 Instruction);

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_BC_instr(OP_PARAMS)
{
    cop1_BC_instruction[__RT](PASS_PARAMS);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_S_instr(OP_PARAMS)
{
    cop1_S_instruction[__F](PASS_PARAMS);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_D_instr(OP_PARAMS)
{
    cop1_D_instruction[__F](PASS_PARAMS);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_W_instr(OP_PARAMS)
{
    cop1_W_instruction[__F](PASS_PARAMS);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_L_instr(OP_PARAMS)
{
    cop1_L_instruction[__F](PASS_PARAMS);
}

int pseudoregcache = 0;
/*
 =======================================================================================================================
    void (*cop1_S_instruction[])(OP_PARAMS)= //
 =======================================================================================================================
 */
void cop1_add_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += DEFAULT_ARITHMATIC_STEP;
//   _SAFTY_COP1_(r4300i_COP1_add_s) 

    if (!IsSSESupported())
    {
        FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FS<<Experiment]);
        FADD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FT<<Experiment]);
        FSTP_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FD<<Experiment]);
        return;
    }

    
    SetFdFsFt32bit(PASS_PARAMS);
    

    if (xFD->mips_reg != xFT->mips_reg)
    {
        xFD->IsDirty = 1;
        if (xFD->x86reg != xFS->x86reg)
            xFD->NoNeedToLoadTheLo = 1;
        MapFS;
        MapFT;
        MapFD;

        MOVSS_MemoryToReg(xFD->x86reg, 0xC0|xFS->x86reg, 0);
        ADDSS_MemoryToReg(xFD->x86reg, 0xC0|xFT->x86reg, 0);
    }
    else
    {
        xFT->IsDirty = 1;
        MapFS;
        MapFT;
        ADDSS_MemoryToReg(xFT->x86reg, 0xC0|xFS->x86reg, 0);
    }
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_sub_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ARITHMATIC_STEP;
//   _SAFTY_COP1_(r4300i_COP1_sub_s) 

    if (!IsSSESupported())
    {
        FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FS<<Experiment]);
        FSUB_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FT<<Experiment]);
        FSTP_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FD<<Experiment]);
        return;
    }

    
    SetFdFsFt32bit(PASS_PARAMS);
    

    if (xFD->mips_reg != xFT->mips_reg)
    {
        xFD->IsDirty = 1;
        xFD->NoNeedToLoadTheLo = 1;
        MapFS;
        MapFT;
        MapFD;

        MOVSS_MemoryToReg(xFD->x86reg, 0xC0|xFS->x86reg, 0);
        SUBSS_MemoryToReg(xFD->x86reg, 0xC0|xFT->x86reg, 0);
    }
    else
    {
        SSE_FlushAllRegisters(FLUSH_TO_MEMORY);
        MOVSS_MemoryToReg(Reg_EAX, ModRM_disp32, (unsigned long) &reg->fpr32[__FS<<Experiment]);
        SUBSS_MemoryToReg(Reg_EAX, ModRM_disp32, (unsigned long) &reg->fpr32[__FT<<Experiment]);
        MOVSS_RegToMemory(Reg_EAX, ModRM_disp32, (unsigned long) &reg->fpr32[__FD<<Experiment]);    
    }

    }


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_mul_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += DEFAULT_MUL_S;
//   _SAFTY_COP1_(r4300i_COP1_mul_s) 

    if (!IsSSESupported())
    {
        FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FS<<Experiment]);
        FMUL_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FT<<Experiment]);
        FSTP_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FD<<Experiment]);
        return;
    }

    
    SetFdFsFt32bit(PASS_PARAMS);
    

    if (xFD->mips_reg != xFT->mips_reg)
    {
        xFD->IsDirty = 1;
        if (xFD->x86reg != xFS->x86reg)
            xFD->NoNeedToLoadTheLo = 1;
        MapFS;
        MapFT;
        MapFD;

        MOVSS_MemoryToReg(xFD->x86reg, 0xC0|xFS->x86reg, 0);
        MULSS_MemoryToReg(xFD->x86reg, 0xC0|xFT->x86reg, 0);
    }
    else
    {
        xFT->IsDirty = 1;
        MapFS;
        MapFT;
        MULSS_MemoryToReg(xFT->x86reg, 0xC0|xFS->x86reg, 0);
    }
}

void cop1_div_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ARITHMATIC_STEP;
   _SAFTY_COP1_(r4300i_COP1_div_s) 

	INTERPRET_NOFLUSH(r4300i_COP1_div_s);
	return;

    if (!IsSSESupported())
    {
        FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FS<<Experiment]);
        FDIV_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FT<<Experiment]);
        FSTP_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FD<<Experiment]);
        return;
    }

    
    SetFdFsFt32bit(PASS_PARAMS);
    

    if (xFD->mips_reg != xFT->mips_reg)
    {
        xFD->IsDirty = 1;
        xFD->NoNeedToLoadTheLo = 1;
        MapFS;
        MapFT;
        MapFD;

        MOVSS_MemoryToReg(xFD->x86reg, 0xC0|xFS->x86reg, 0);
        DIVSS_MemoryToReg(xFD->x86reg, 0xC0|xFT->x86reg, 0);
    }
    else
    {
        SSE_FlushAllRegisters(FLUSH_TO_MEMORY);
        MOVSS_MemoryToReg(Reg_EAX, ModRM_disp32, (unsigned long) &reg->fpr32[__FS<<Experiment]);
        DIVSS_MemoryToReg(Reg_EAX, ModRM_disp32, (unsigned long) &reg->fpr32[__FT<<Experiment]);
        MOVSS_RegToMemory(Reg_EAX, ModRM_disp32, (unsigned long) &reg->fpr32[__FD<<Experiment]);    
    }

}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_sqrt_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += DEFAULT_SQRT_S_STEP;
    _SAFTY_COP1_(r4300i_COP1_sqrt_s);

    FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FS<<Experiment]);
    FSQRT();
    FSTP_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FD<<Experiment]);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_abs_s(OP_PARAMS)
{
    int ExtraRegister = 0;
    
    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += 1;
    _SAFTY_COP1_(r4300i_COP1_abs_s)


    if (__FS ==__FD) 
        AND_ImmToMemory((unsigned long) &reg->fpr32[__FS<<Experiment], 0x7fffffff); 
    else
    {
        ExtraRegister = FindFreeRegister();
        if (ExtraRegister < 0)
            ExtraRegister = Reg_EBP;

        MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (unsigned long) &reg->fpr32[__FS<<Experiment]);
        AND_ImmToReg(ExtraRegister, 0x7fffffff, 0);
        MOV_RegToMemory(1, ExtraRegister, ModRM_disp32,(unsigned long) &reg->fpr32[__FD<<Experiment]);

        if (ExtraRegister == Reg_EBP)
            MOV_ImmToReg(Reg_EBP, HardwareStart);
    }
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_mov_s(OP_PARAMS)
{
    int ExtraRegister = 0;  
    
    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += 1;
    if (__FD == __FS) return;

    if (IsSSESupported())
    {
        SetFdFsFt32bit(PASS_PARAMS);
        xFD->IsDirty = 1;
        MapFS;
        MapFD;
        MOVSS_MemoryToReg(xFD->x86reg, 0xC0|xFS->x86reg, 0);
    }
    else
    {
    _SAFTY_COP1_(r4300i_COP1_mov_s)
    
   
    ExtraRegister = FindFreeRegister();
    if (ExtraRegister < 0)
        ExtraRegister = Reg_EBP;

    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (unsigned __int32) &reg->fpr32[__FS<<Experiment]);
    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (unsigned __int32) &reg->fpr32[__FD<<Experiment]);


    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);
    }
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_neg_s(OP_PARAMS)
{
    int ExtraRegister = 0;
    
    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += 1;
    _SAFTY_COP1_(r4300i_COP1_neg_s)
        
    ExtraRegister = FindFreeRegister();
    if (ExtraRegister < 0)
        ExtraRegister = Reg_EBP;

    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (unsigned long) &reg->fpr32[__FS<<Experiment]);
    XOR_ImmToReg(ExtraRegister, 0x80000000, 0);
    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (unsigned long) &reg->fpr32[__FD<<Experiment]);

    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_roundl_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_roundl_s)
        
	INTERPRET_NOFLUSH(r4300i_COP1_roundl_s);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_truncl_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_truncl_s)
        
	INTERPRET_NOFLUSH(r4300i_COP1_truncl_s);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_ceill_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_ceill_s)
        
	INTERPRET_NOFLUSH(r4300i_COP1_ceill_s);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_floorl_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_floorl_s)
        
	INTERPRET_NOFLUSH(r4300i_COP1_floorl_s);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_roundw_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_roundw_s)

	INTERPRET_NOFLUSH(r4300i_COP1_roundw_s);
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_truncw_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_truncw_s)

    
	if (IsSSESupported())
    {
        PUSH_RegToStack(Reg_ECX);
        CVTTSS2SI_MemoryToReg(Reg_ECX, ModRM_disp32, (unsigned long) &reg->fpr32[__FS<<Experiment]);
        MOV_RegToMemory(1, Reg_ECX, ModRM_disp32, (unsigned long) &reg->fpr32[__FD<<Experiment]);
        POP_RegFromStack(Reg_ECX);
    }
    else
    {
        INTERPRET_NOFLUSH(r4300i_COP1_truncw_s);
    }
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_ceilw_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_ceilw_s)

    INTERPRET_NOFLUSH(r4300i_COP1_ceilw_s);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_floorw_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_floorw_s)

    if (IsSSESupported())
    {
        PUSH_RegToStack(Reg_ECX);
        LDMXCSR((uint32)&iMXCSR_FLOOR);

        CVTSS2SI_MemoryToReg(Reg_ECX, ModRM_disp32, (unsigned long) &reg->fpr32[__FS<<Experiment]);
        MOV_RegToMemory(1, Reg_ECX, ModRM_disp32, (unsigned long) &reg->fpr32[__FD<<Experiment]);
        LDMXCSR((uint32)&iMXCSR);
        POP_RegFromStack(Reg_ECX);
    }
    else
    {
        INTERPRET_NOFLUSH(r4300i_COP1_floorw_s);
    }
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_cvtd_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += 1;
    _SAFTY_COP1_(r4300i_COP1_cvtd_s) 
        

    FLD_Memory(FORMAT_SINGLE, (_u32) & reg->fpr32[__FS<<Experiment]);
    FSTP_Memory(FORMAT_QUAD, (_u32) & reg->fpr32[__FD<<Experiment]);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */

//interpreted version uses ftol.
void cop1_cvtw_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_cvtw_s)

//    if (IsSSESupported())
//    {
//        PUSH_RegToStack(Reg_EAX);
//        CVTSS2SI_MemoryToReg(Reg_EAX, ModRM_disp32, (unsigned long) &reg->fpr32[__FS<<Experiment]);
//        MOV_RegToMemory(1, Reg_EAX, ModRM_disp32, (unsigned long) &reg->fpr32[__FD<<Experiment]);
//        POP_RegFromStack(Reg_EAX);
//    }
//    else
    {
        INTERPRET_NOFLUSH(r4300i_COP1_cvtw_s);
    }
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_cvtl_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_cvtl_s)

    INTERPRET_NOFLUSH(r4300i_COP1_cvtl_s);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_f_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_F_S)

    INTERPRET_NOFLUSH(r4300i_C_cond_fmt_s);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_un_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_UN_S)


    INTERPRET_NOFLUSH(r4300i_C_cond_fmt_s);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_ueq_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_UEQ_S)

    INTERPRET_NOFLUSH(r4300i_C_cond_fmt_s);
}


//This function also works works for checking for FCMOVcc support.
BOOL IsFCOMISupported() 
{ 
    static int IsSupported = FALSE;
    static int AlreadyTested = FALSE;



    if (AlreadyTested)
        return IsSupported;

    AlreadyTested = TRUE;

    __asm 
    { 
        mov eax,1                   // CPUID level 1 
        cpuid                       // EDX = feature flag 
        and edx, 0x8001             // test bit 15 (CMOV support) of feature flag and bit 0 (FPU).
        mov IsSupported,edx
    } 
    if (IsSupported == 0x8001) 
    {
        IsSupported = TRUE;
    }
    else 
        IsSupported = FALSE;

        return IsSupported; 
} 


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_olt_s(OP_PARAMS)
{
    int ExtraRegister = 0;

    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_OLT_S)

    
    if (IsFCOMISupported() == FALSE)
    {
        INTERPRET_NOFLUSH(r4300i_C_cond_fmt_s);
        return;
    }

    ExtraRegister = FindFreeRegister();
    if(ExtraRegister < Reg_EAX)
        ExtraRegister = Reg_EBP;

    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FT<<Experiment]);
    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FS<<Experiment]);

    //Using FCOMI saves over 10 cycles!

    //fcomi (no pop)
    WC8(0xDB);
    WC8(0xf1);
    //end fcomi

    Jcc_auto(CC_B, 14);

    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);

    SetTarget(14);
    
    //Use fcomip again to pop the register stack
    WC8(0xDF);
    WC8(0xf1);

    
    //Check for unordered. If unordered, then clear the bit.
    Jcc_auto(CC_AE|CC_NZ|CC_NP, 15);
    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);
    
    SetTarget(15);

    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);


    //Use fcomip again to pop the register stack
    WC8(0xDF);
    WC8(0xf1);
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_ole_s(OP_PARAMS)
{
    int ExtraRegister = 0;

    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_OLE_S)

    
    if (IsFCOMISupported() == FALSE)
    {
        INTERPRET_NOFLUSH(r4300i_C_cond_fmt_s);
        return;
    }


    ExtraRegister = FindFreeRegister();
    if(ExtraRegister < Reg_EAX)
        ExtraRegister = Reg_EBP;

    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FT<<Experiment]);
    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FS<<Experiment]);

    //Using FCOMI saves over 10 cycles!

    //fcomi (no pop)
    WC8(0xDB);
    WC8(0xf1);
    //end fcomi

    Jcc_auto(CC_BE, 14);

    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);

    SetTarget(14);
    
    //Use fcomip again to pop the register stack
    WC8(0xDF);
    WC8(0xf1);

    
    //Check for unordered. Clear bit if unordered
    Jcc_auto(CC_AE|CC_NZ|CC_NP, 15);
    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);
    
    SetTarget(15);

    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);


    //Use fcomip again to pop the register stack
    WC8(0xDF);
    WC8(0xf1);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_ule_s(OP_PARAMS)
{
    int ExtraRegister = 0;

    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_ULE_S)

    
    if (IsFCOMISupported() == FALSE)
    {
        INTERPRET_NOFLUSH(r4300i_C_cond_fmt_s);
        return;
    }


    ExtraRegister = FindFreeRegister();
    if(ExtraRegister < Reg_EAX)
        ExtraRegister = Reg_EBP;

    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FT<<Experiment]);
    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FS<<Experiment]);

    //Using FCOMI saves over 10 cycles!

    //fcomi (no pop)
    WC8( (0xDB));
    WC8(0xf1);
    //end fcomi

    Jcc_auto(CC_BE, 14);

    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);

    SetTarget(14);
    
    //Use fcomip again to pop the register stack
    WC8( (0xDF));
    WC8(0xf1);

    
    //Check for unordered
    Jcc_auto(CC_AE|CC_NZ|CC_NP, 15);
    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    
    SetTarget(15);

    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);


    //Use fcomip again to pop the register stack
    WC8( (0xDF));
    WC8(0xf1);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_sf_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_SF_S)

    
    INTERPRET_NOFLUSH(r4300i_C_cond_fmt_s);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_ngle_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_NGLE_S)

    INTERPRET_NOFLUSH(r4300i_C_cond_fmt_s);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_seq_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_SEQ_S)

    INTERPRET_NOFLUSH(r4300i_C_cond_fmt_s);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_ngl_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_NGL_S)

    INTERPRET_NOFLUSH(r4300i_C_cond_fmt_s);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void _do_c_(HardwareState *reg, _u8 format, _u8 testval)
{
    PUSH_RegIfMapped(Reg_EAX, 0, -1);
    FLD_Memory(format, (unsigned long) &reg->fpr32[__FS<<Experiment]);
    FCOMP(format, (unsigned long) &reg->fpr32[__FT<<Experiment]);

    // * 00431CA5 DF E0 fnstsw ax ?
    // * 00431CA7 F6 C4 01 test ah,1
    
    FNSTSW();
    WC16(0xC4F6);
    WC8(testval);
    LOGGING_DYNA(LogDyna("  TEST AH, 0x%x\n", testval););

    Jcc_auto(CC_E, 13);

    MOV_MemoryToReg(Reg_EAX, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    OR_ImmToReg(Reg_EAX, COP1_CONDITION_BIT, 0);

    JMP_Short_auto(14);

    SetTarget(13);

    MOV_MemoryToReg(Reg_EAX, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    AND_ImmToReg(Reg_EAX, (uint32)~COP1_CONDITION_BIT, 0);

    SetTarget(14);
    MOV_RegToMemory(1, Reg_EAX, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    POP_RegIfMapped(Reg_EAX);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */

void c_lt_s__XMM(OP_PARAMS)
{
    int ExtraRegister = 0;
    
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
//    _SAFTY_COP1_(r4300i_C_LT_S)


    ExtraRegister = FindFreeRegister();
    if(ExtraRegister < Reg_EAX)
        ExtraRegister = Reg_EBP;

    SetFdFsFt32bit(PASS_PARAMS);

    MapFS;
    MapFT;
    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    
    
    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    COMISS_MemoryWithReg(xFS->x86reg, 0xC0|xFT->x86reg, 0);

    Jcc_auto(CC_B, 14);

    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);

    SetTarget(14);
    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);
    
}

void c_ult_s__XMM(OP_PARAMS)
{
    int ExtraRegister = 0;

    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
//    _SAFTY_COP1_(r4300i_C_ULT_S)

    ExtraRegister = FindFreeRegister();
    if(ExtraRegister < Reg_EAX)
        ExtraRegister = Reg_EBP;

    SetFdFsFt32bit(PASS_PARAMS);
    MapFS;
    MapFT;
    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);

    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    UCOMISS_MemoryWithReg(xFS->x86reg, 0xC0|xFT->x86reg, 0);

    Jcc_auto(CC_B, 14);

    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);

    SetTarget(14);
    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);
    
}


void c_lt_s(OP_PARAMS)
{
    int ExtraRegister = 0;
    
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    


    if (IsFCOMISupported() == FALSE)
    {
        _SAFTY_COP1_(r4300i_C_LT_S)
        _do_c_(reg, FORMAT_SINGLE, 0x01);
        return;
    }
    if (IsSSESupported())
    {
        compilerstatus.cp0Counter -= DEFAULT_C_COND_STEP;
        c_lt_s__XMM(PASS_PARAMS);
        return;
    }


    _SAFTY_COP1_(r4300i_C_LT_S)
    ExtraRegister = FindFreeRegister();
    if(ExtraRegister < Reg_EAX)
        ExtraRegister = Reg_EBP;

    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FT<<Experiment]);
    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FS<<Experiment]);


    //Using FCOMI saves over 10 cycles!

    //fcomi
    WC8( (0xDF));
    WC8(0xf1);
    //end fcomi

    Jcc_auto(CC_B, 14);

    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);

    SetTarget(14);
    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);
    
    //Use fcomip again to pop the register stack
    WC8(0xDF);
    WC8(0xf1);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_ult_s(OP_PARAMS)
{
    int ExtraRegister = 0;

    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;

    
    if (IsFCOMISupported() == FALSE)
    {
        _SAFTY_COP1_(r4300i_C_ULT_S)
        INTERPRET_NOFLUSH(r4300i_C_cond_fmt_s);
        return;
    }
    if (IsSSESupported())
    {
        compilerstatus.cp0Counter -= DEFAULT_C_COND_STEP;
        c_ult_s__XMM(PASS_PARAMS);
        return;
    }



    _SAFTY_COP1_(r4300i_C_ULT_S)
    ExtraRegister = FindFreeRegister();
    if(ExtraRegister < Reg_EAX)
        ExtraRegister = Reg_EBP;

    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FT<<Experiment]);
    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FS<<Experiment]);

    //Using FCOMI saves over 10 cycles!

    //fcomi (no pop)
    WC8( (0xDB));
    WC8(0xf1);
    //end fcomi

    Jcc_auto(CC_B, 14);

    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);

    SetTarget(14);
    
    //Use fcomip again to pop the register stack
    WC8( (0xDF));
    WC8(0xf1);

    
    //Check for unordered. If unordered, then set the bit.
    Jcc_auto(CC_AE|CC_NZ|CC_NP, 15);
    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    
    SetTarget(15);

    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);


    //Use fcomip again to pop the register stack
    WC8( (0xDF));
    WC8(0xf1);
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */


void c_nge_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;

    c_lt_s(PASS_PARAMS);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */




void c_eq_s__XMM(OP_PARAMS)
{
    int ExtraRegister = 0;
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
//    _SAFTY_COP1_(r4300i_C_EQ_S)

    ExtraRegister = FindFreeRegister();
    if(ExtraRegister < Reg_EAX)
        ExtraRegister = Reg_EBP;

    SetFdFsFt32bit(PASS_PARAMS);
    MapFS;
    MapFT;
    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);

    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    COMISS_MemoryWithReg(xFS->x86reg, 0xC0|xFT->x86reg, 0);
    
    
    Jcc_auto(CC_Z, 14);

    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);

    SetTarget(14);
    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);
    
}


void c_eq_s(OP_PARAMS)
{
    int ExtraRegister = 0;
    
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;

    if (IsFCOMISupported() == FALSE)
    {
        _SAFTY_COP1_(r4300i_C_EQ_S)        
        _do_c_(reg, FORMAT_SINGLE, 0x40);
        return;
    }
    if (IsSSESupported())
    {
        compilerstatus.cp0Counter -= DEFAULT_C_COND_STEP;
        c_eq_s__XMM(PASS_PARAMS);
        return;
    }

  
    _SAFTY_COP1_(r4300i_C_EQ_S)
    ExtraRegister = FindFreeRegister();
    if(ExtraRegister < Reg_EAX)
        ExtraRegister = Reg_EBP;

    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FT<<Experiment]);
    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FS<<Experiment]);


    //Using FCOMI saves over 10 cycles!
    
    //fcomi
    WC8( (0xDF));
    WC8(0xf1);
    //end fcomi

    Jcc_auto(CC_Z, 14);

    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);

    SetTarget(14);
    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);
    
    //FSTP(FORMAT_SINGLE, ModRM_disp32, (unsigned long) &reg->fpr32[__FT]);
    //Use fcomip again to pop the register stack
    WC8( (0xDF));
    WC8(0xf1);

}


void c_le_s__XMM(OP_PARAMS)
{
    int ExtraRegister = 0;
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
//    _SAFTY_COP1_(r4300i_C_LE_S)

    ExtraRegister = FindFreeRegister();
    if(ExtraRegister < Reg_EAX)
        ExtraRegister = Reg_EBP;

    SetFdFsFt32bit(PASS_PARAMS);
    MapFS;
    MapFT;
    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);

    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    COMISS_MemoryWithReg(xFS->x86reg, 0xC0|xFT->x86reg, 0);

    Jcc_auto(CC_BE, 14);

    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);

    SetTarget(14);
    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);
    
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_le_s(OP_PARAMS)
{
    int ExtraRegister = 0;
    
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter -= DEFAULT_C_COND_STEP;

    if (IsFCOMISupported() == FALSE)
    {
        _SAFTY_COP1_(r4300i_C_LE_S)
        _do_c_(reg, FORMAT_SINGLE, 0x41);
        return;
    }
    if (IsSSESupported())
    {
        compilerstatus.cp0Counter -= DEFAULT_C_COND_STEP;
        c_le_s__XMM(PASS_PARAMS);
        return;
    }


    _SAFTY_COP1_(r4300i_C_LE_S)
    ExtraRegister = FindFreeRegister();
    if(ExtraRegister < Reg_EAX)
        ExtraRegister = Reg_EBP;

    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FT<<Experiment]);
    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    FLD_Memory(FORMAT_SINGLE, (unsigned long) &reg->fpr32[__FS<<Experiment]);

    //Using FCOMI saves over 10 cycles!

    //fcomi
    WC8( (0xDF));
    WC8(0xf1);
    //end fcomi

    Jcc_auto(CC_BE, 14);

    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);

    SetTarget(14);
    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);
    
    //Use fcomip again to pop the register stack
    WC8( (0xDF));
    WC8(0xf1);
}







/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_ngt_s(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
//    _SAFTY_COP1_(r4300i_C_NGT_S) 

    compilerstatus.cp0Counter -= DEFAULT_C_COND_STEP;
    c_le_s(PASS_PARAMS);
}        


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_lt_d(OP_PARAMS)
{
    int ExtraRegister = 0;

    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_LT_D) 
        
    if (IsFCOMISupported() == FALSE)
    {
        _do_c_(reg, FORMAT_QUAD, 0x01);
    }

//TODO:
//    if (IsSSESupported())
//    {
//        compilerstatus.cp0Counter -= 1;
//        c_lt_d__XMM(PASS_PARAMS);
//        return;
//    }


    ExtraRegister = FindFreeRegister();
    if(ExtraRegister < Reg_EAX)
        ExtraRegister = Reg_EBP;

    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    FLD_Memory(FORMAT_QUAD, (unsigned long) &reg->fpr32[__FT<<Experiment]);
    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    FLD_Memory(FORMAT_QUAD, (unsigned long) &reg->fpr32[__FS<<Experiment]);


    //Using FCOMI saves over 10 cycles!

    //fcomi
    WC8( (0xDF));
    WC8(0xf1);
    //end fcomi

    Jcc_auto(CC_B, 14);

    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);

    SetTarget(14);
    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);
    
    //Use fcomip again to pop the register stack
    WC8( (0xDF));
    WC8(0xf1);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_nge_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_NGE_D)

    compilerstatus.cp0Counter -= DEFAULT_C_COND_STEP;
    c_lt_d(PASS_PARAMS);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_eq_d(OP_PARAMS)
{
    int ExtraRegister = 0;

    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_EQ_D)
    

    if (IsFCOMISupported() == FALSE)
    {
        _do_c_(reg, FORMAT_QUAD, 0x40);
        return;
    }

//TODO:
//    if (IsSSESupported())
//    {
//        compilerstatus.cp0Counter -= 1;
//        c_eq_d__XMM(PASS_PARAMS);
//        return;
//    }

  
    ExtraRegister = FindFreeRegister();
    if(ExtraRegister < Reg_EAX)
        ExtraRegister = Reg_EBP;

    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    FLD_Memory(FORMAT_QUAD, (unsigned long) &reg->fpr32[__FT<<Experiment]);
    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    FLD_Memory(FORMAT_QUAD, (unsigned long) &reg->fpr32[__FS<<Experiment]);


    //Using FCOMI saves over 10 cycles!
    
    //fcomi
    WC8( (0xDF));
    WC8(0xf1);
    //end fcomi

    Jcc_auto(CC_Z, 14);

    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);

    SetTarget(14);
    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);
    
    //FSTP(FORMAT_SINGLE, ModRM_disp32, (unsigned long) &reg->fpr32[__FT]);
    //Use fcomip again to pop the register stack
    WC8( (0xDF));
    WC8(0xf1);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_le_d(OP_PARAMS)
{
    int ExtraRegister = 0;

    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_LE_D)


    if (IsFCOMISupported() == FALSE)
    {
        _do_c_(reg, FORMAT_QUAD, 0x41);
        return;
    }

//    if (IsSSESupported())
//    {
//        compilerstatus.cp0Counter -= 1;
//        c_le_d__XMM(PASS_PARAMS);
//        return;
//    }

    ExtraRegister = FindFreeRegister();
    if(ExtraRegister < Reg_EAX)
        ExtraRegister = Reg_EBP;

    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    FLD_Memory(FORMAT_QUAD, (unsigned long) &reg->fpr32[__FT<<Experiment]);
    OR_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
    FLD_Memory(FORMAT_QUAD, (unsigned long) &reg->fpr32[__FS<<Experiment]);

    //Using FCOMI saves over 10 cycles!

    //fcomi
    WC8( (0xDF));
    WC8(0xf1);
    //end fcomi

    Jcc_auto(CC_BE, 14);

    AND_ImmToReg(ExtraRegister, (uint32)~COP1_CONDITION_BIT, 0);

    SetTarget(14);
    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);
    
    //Use fcomip again to pop the register stack
    WC8( (0xDF));
    WC8(0xf1);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_ngt_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_NGT_D)
    
    compilerstatus.cp0Counter -= DEFAULT_C_COND_STEP;
    c_le_d(PASS_PARAMS);
}

#define fdLOGIC(Sum, Operand1, OPERATOR16, Operand2) \
    WC16(0x05DD); \
    WC32((_u32) & Operand1); \
    WC16(OPERATOR16); \
    WC32((_u32) & Operand2); \
    WC16(0x1DDD); \
    WC32((_u32) & Sum);

#define fdoubleLOGICAL(OPERATOR16)  fdLOGIC(reg->fpr32[__FD<<Experiment], reg->fpr32[__FS<<Experiment], OPERATOR16, reg->fpr32[__FT<<Experiment])

/*
 =======================================================================================================================
 =======================================================================================================================
 */

void cop1_add_d(OP_PARAMS)
{
    
    cop1_with_exception(PASS_PARAMS);    

    compilerstatus.cp0Counter += DEFAULT_ARITHMATIC_STEP;
    _SAFTY_COP1_(r4300i_COP1_add_d)

    fdoubleLOGICAL(0x05DC)
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_sub_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ARITHMATIC_STEP;
    _SAFTY_COP1_(r4300i_COP1_sub_d)
    
    fdoubleLOGICAL(0x25DC)
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_mul_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    

    compilerstatus.cp0Counter += DEFAULT_MUL_D;
    _SAFTY_COP1_(r4300i_COP1_mul_d)

    fdoubleLOGICAL(0x0DDC)

}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_div_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_DIV_D_STEP;
    _SAFTY_COP1_(r4300i_COP1_div_d)

    INTERPRET_NOFLUSH(r4300i_COP1_div_d);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_sqrt_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_SQRT_D_STEP;
    _SAFTY_COP1_(r4300i_COP1_sqrt_d)

    INTERPRET_NOFLUSH(r4300i_COP1_sqrt_d);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_abs_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += 1;
    _SAFTY_COP1_(r4300i_COP1_abs_d)

    INTERPRET_NOFLUSH(r4300i_COP1_abs_d);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_mov_d(OP_PARAMS)
{
    int ExtraRegister = 0;  
    
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += 1;
    
    if (__FD == __FS) return;
    _SAFTY_COP1_(r4300i_COP1_mov_d)

    ExtraRegister = FindFreeRegister();
    if (ExtraRegister < 0)
        ExtraRegister = Reg_EBP;

        
    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (unsigned long) &reg->fpr32[__FS<<Experiment]);
    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (unsigned long) &reg->fpr32[__FD<<Experiment]);
    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (unsigned long) &reg->fpr32[(__FS<<Experiment)+1]);
    MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (unsigned long) &reg->fpr32[(__FD<<Experiment)+1]);

    if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
//I can't find a game that uses this opcode a lot, we can optimize it if we find one.
void cop1_neg_d(OP_PARAMS)
{
    
    cop1_with_exception(PASS_PARAMS);    

    compilerstatus.cp0Counter += 1;
    _SAFTY_COP1_(r4300i_COP1_neg_d)

    INTERPRET_NOFLUSH(r4300i_COP1_neg_d);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_roundl_d(OP_PARAMS)
{

    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_roundl_d)

        INTERPRET_NOFLUSH(r4300i_COP1_roundl_d);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_truncl_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_truncl_d)

    INTERPRET_NOFLUSH(r4300i_COP1_truncl_d);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_ceill_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_ceill_d)

    INTERPRET_NOFLUSH(r4300i_COP1_ceill_d);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_floorl_d(OP_PARAMS)
{
    
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_floorl_d)

    INTERPRET_NOFLUSH(r4300i_COP1_floorl_d);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_roundw_d(OP_PARAMS)
{
    
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_roundw_d)

    INTERPRET_NOFLUSH(r4300i_COP1_roundw_d);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_truncw_d(OP_PARAMS)
{
    
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_truncw_d)

        
    INTERPRET_NOFLUSH(r4300i_COP1_truncw_d);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_ceilw_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_ceilw_d)

    INTERPRET_NOFLUSH(r4300i_COP1_ceilw_d);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_floorw_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_floorw_d)

    INTERPRET_NOFLUSH(r4300i_COP1_floorw_d);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_cvts_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += 2; //this is right.
    _SAFTY_COP1_(r4300i_COP1_cvts_d);

    FLD_Memory(FORMAT_QUAD, (unsigned long) &reg->fpr32[__FS<<Experiment]);
    FSTP_Memory(FORMAT_SINGLE, (_u32) & reg->fpr32[__FD<<Experiment]);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_cvtw_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;;
    _SAFTY_COP1_(r4300i_COP1_cvtw_d)

    INTERPRET_NOFLUSH(r4300i_COP1_cvtw_d);

}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_cvtl_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;;
    _SAFTY_COP1_(r4300i_COP1_cvtl_d)

    INTERPRET_NOFLUSH(r4300i_COP1_cvtl_d);
//    FLD_Memory(FORMAT_QUAD, (_u32) & reg->fpr32[__FS<<Experiment]);
//    FISTP_Memory(FORMAT_QUAD, (_u32) & reg->fpr32[__FD<<Experiment]);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_f_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_F_D)

    INTERPRET_NOFLUSH(r4300i_C_F_D);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_un_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_UN_D)

    INTERPRET_NOFLUSH(r4300i_C_UN_D);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_ueq_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_UEQ_D)

    INTERPRET_NOFLUSH(r4300i_C_UEQ_D);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_olt_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_OLT_D)

    INTERPRET_NOFLUSH(r4300i_C_OLT_D);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_ult_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_ULT_D)

    INTERPRET_NOFLUSH(r4300i_C_ULT_D);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_ole_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_OLE_D)

    INTERPRET_NOFLUSH(r4300i_C_OLE_D);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_ule_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_ULE_D)

    INTERPRET_NOFLUSH(r4300i_C_ULE_D);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_sf_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);    
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_SF_D)

    INTERPRET_NOFLUSH(r4300i_C_SF_D);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_ngle_d(OP_PARAMS)
{
    
    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_NGLE_D)

    INTERPRET_NOFLUSH(r4300i_C_NGLE_D);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_seq_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_SEQ_D)

    INTERPRET_NOFLUSH(r4300i_C_SEQ_D);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void c_ngl_d(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += DEFAULT_C_COND_STEP;
    _SAFTY_COP1_(r4300i_C_NGL_D)

    INTERPRET_NOFLUSH(r4300i_C_NGL_D);
}

/*
 =======================================================================================================================
    void (*cop1_W_instruction[])(OP_PARAMS)= //
 =======================================================================================================================
 */
void cop1_cvts_w(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_cvts_w)

    FILD_Memory(FORMAT_SINGLE, (_u32) & reg->fpr32[__FS<<Experiment]);
    FSTP_Memory(FORMAT_SINGLE, (_u32) & reg->fpr32[__FD<<Experiment]);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_cvtd_w(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_cvtd_w)

    FILD_Memory(FORMAT_SINGLE, (_u32) & reg->fpr32[__FS<<Experiment]);
    FSTP_Memory(FORMAT_QUAD, (_u32) & reg->fpr32[__FD<<Experiment]);
}

/*
 =======================================================================================================================
    void (*cop1_L_instruction[])(OP_PARAMS)= //
 =======================================================================================================================
 */
void cop1_cvts_l(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);
    
    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_cvts_l)

//004669EC DF 6D F0         fild        qword ptr [fs] 
//004669EF D9 5D F8         fstp        dword ptr [fd] 
    FILD_Memory(FORMAT_QUAD, (uint32)&reg->fpr32[__FS<<Experiment]);
    FSTP_Memory(FORMAT_SINGLE, (uint32)&reg->fpr32[__FD<<Experiment]);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_cvtd_l(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);

    compilerstatus.cp0Counter += DEFAULT_ROUNDING_STEP;
    _SAFTY_COP1_(r4300i_COP1_cvtd_l)

    FILD_Memory(FORMAT_QUAD, (uint32)&reg->fpr32[__FS<<Experiment]);
    FSTP_Memory(FORMAT_QUAD, (uint32)&reg->fpr32[__FD<<Experiment]);

}

/* Load From / Store To CPR[Reg] // */
extern void HELP_Cop1(unsigned long pc);

/*
 =======================================================================================================================
 =======================================================================================================================
 */
extern unsigned int HardwareStart;
void lwc1(OP_PARAMS)
{
    /*~~~~~~~~~~*/
    _s32    value;
    /*~~~~~~~~~~*/

    cop1_with_exception(PASS_PARAMS);
    
    SSE_FlushAllRegisters(FLUSH_TO_MEMORY);    
    OpcodePrologue(1, &r4300i_lwc1, 1, PASS_PARAMS);

#ifdef SAFE_LOADSTORE_FPU
    INTERPRET(r4300i_lwc1);
	return;
#endif
    if(ConstMap[xRS->mips_reg].IsMapped == 1)
    {
        __try
        {
            _u32    QuerAddr;
            int ExtraRegister;

            /* TLB range */
            QuerAddr = (_u32) ((_s32) ConstMap[xRS->mips_reg].value + (_s32) (_s16) __I);

            if(NOT_IN_KO_K1_SEG(QuerAddr)) goto _Default;

            value = LOAD_UWORD_PARAM(QuerAddr);

            ExtraRegister = FindFreeRegister();
            if(ExtraRegister < Reg_EAX) ExtraRegister = Reg_EBP;
            MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) pLOAD_UWORD_PARAM(QuerAddr));
            MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) & gHWS_fpr32[__FT<<Experiment]);
            if (ExtraRegister == Reg_EBP) MOV_ImmToReg(Reg_EBP, HardwareStart);

        }

        __except(NULL, EXCEPTION_EXECUTE_HANDLER)
        {
            goto _Default;
        }
    }
    else
    {
_Default:
        if(ConstMap[xRS->mips_reg].IsMapped)
        {
            int temp = ConstMap[xRS->mips_reg].value;

           
            PUSH_RegIfMapped(Reg_EAX, 0, -1);
            PUSH_RegIfMapped(Reg_ECX, 0, -1);

            MOV_ImmToReg(Reg_EAX, temp + (_s32) (_s16) __dotI);
            MOV_ImmToReg(Reg_ECX, ((uint32) (temp + (_s32) (_s16) __dotI)) >> SHIFTER2_READ);
            WC16(0x14FF);
            WC8(0x8D);
            WC32((uint32) & gHardwareState.memory_read_functions);
            LOGGING_DYNA(LogDyna("  CALL memory_read_functions[]\n"););

            MOV_MemoryToReg(Reg_EAX, ModRM_EAX, 0);
            MOV_RegToMemory(1, Reg_EAX, ModRM_disp32, (uint32) & gHWS_fpr32[__FT<<Experiment]);

            POP_RegIfMapped(Reg_ECX);
            POP_RegIfMapped(Reg_EAX);
        }
        else
        {
            int ExtraRegister;

            MapRS;
            PUSH_RegIfMapped(Reg_EAX, 1, xRS->x86reg);
            ExtraRegister = FindFreeRegister();
            if (ExtraRegister < Reg_ECX)
                ExtraRegister = Reg_EBP;

            Lea(ExtraRegister, xRS->x86reg, (_s32) (_s16) __dotI);
            Lea(Reg_EAX, xRS->x86reg, (_s32) (_s16) __dotI);
            SHR_RegByImm(ExtraRegister, SHIFTER2_READ);
            WC16(0x14FF);
            WC8(0x85 | (ExtraRegister<<3));
            WC32((uint32) & gHardwareState.memory_read_functions);
            LOGGING_DYNA(LogDyna("  CALL memory_read_functions[]\n"););

            MOV_MemoryToReg(Reg_EAX, ModRM_EAX, 0);
            if( ExtraRegister == Reg_EBP)
                MOV_ImmToReg(Reg_EBP, HardwareStart);
            MOV_RegToMemory(1, Reg_EAX, ModRM_disp32, (uint32) & gHWS_fpr32[__FT<<Experiment]);

            POP_RegIfMapped(Reg_EAX);

        }
    }
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void ldc1(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);
    
    SSE_FlushAllRegisters(FLUSH_TO_MEMORY);
    OpcodePrologue(1, &r4300i_ldc1, 0, PASS_PARAMS);

#ifdef SAFE_LOADSTORE_FPU
    INTERPRET(r4300i_ldc1);
	return;
#endif

{

    int ExtraRegister=0;

			
    //fixed.
    //Core anomaly: Need to FlushAllRegisters before ldc1 in 32bit mode, or Goldenee will have graphics errors
    //fixed.


    MapRS;



				ExtraRegister = FindFreeRegister();
				if (ExtraRegister < Reg_ECX)
					ExtraRegister = Reg_EBP;
                
                PUSH_RegIfMapped(Reg_EAX, 0, Reg_EAX);

                Lea(ExtraRegister, xRS->x86reg, __dotI);
				Lea(Reg_EAX, xRS->x86reg, __dotI);
				SHR_RegByImm(ExtraRegister, SHIFTER2_READ);
				WC16(0x14FF);
				WC8(0x85|(ExtraRegister<<3));
				WC32((uint32) & gHardwareState.memory_read_functions);
				LOGGING_DYNA(LogDyna("	CALL memory_read_functions[]\n"););

                MOV_MemoryToReg(ExtraRegister, ModRM_disp8_EAX, 4);
                MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (uint32)&gHWS_fpr32[__RT<<Experiment]);
                MOV_MemoryToReg(ExtraRegister, ModRM_EAX, 0);
                MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (uint32)&gHWS_fpr32[__RT<<Experiment]+4);

				if (ExtraRegister == Reg_EBP)
                    MOV_ImmToReg(Reg_EBP, HardwareStart);

                POP_RegIfMapped(Reg_EAX);
			}

}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void swc1(OP_PARAMS)
{
    /*~~~~~~~~~~*/
    _s32    value;
    /*~~~~~~~~~~*/

    cop1_with_exception(PASS_PARAMS);
    
    SSE_FlushAllRegisters(FLUSH_TO_MEMORY);    
    OpcodePrologue(1, &r4300i_swc1, 1, PASS_PARAMS);

#ifdef SAFE_LOADSTORE_FPU
    INTERPRET(r4300i_swc1);
	return;
#endif

    if(ConstMap[xRS->mips_reg].IsMapped == 1 && currentromoptions.Code_Check != CODE_CHECK_PROTECT_MEMORY)
    {
        __try
        {
            int ExtraRegister = 0;  
            _u32    QuerAddr;

            /* TLB range */
            QuerAddr = (_u32) ((_s32) ConstMap[xRS->mips_reg].value + (_s32) (_s16) __I);

            if(NOT_IN_KO_K1_SEG(QuerAddr)) goto _Default;

            value = LOAD_UWORD_PARAM(QuerAddr);
            
            ExtraRegister = FindFreeRegister();
            if (ExtraRegister < 0)
                ExtraRegister = Reg_EBP;
            
            
            MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & gHWS_fpr32[__FT<<Experiment]);
            MOV_RegToMemory(1, ExtraRegister, ModRM_disp32, (_u32) pLOAD_UWORD_PARAM(QuerAddr));

            if (ExtraRegister == Reg_EBP)
                MOV_ImmToReg(Reg_EBP, HardwareStart);

        }

        __except(NULL, EXCEPTION_EXECUTE_HANDLER)
        {
            goto _Default;
        }
    }
    else
    {
_Default:
        
        if(ConstMap[xRS->mips_reg].IsMapped)
        {
            int temp = ConstMap[xRS->mips_reg].value;

            PUSH_RegIfMapped(Reg_EAX, 0, -1);
            PUSH_RegIfMapped(Reg_ECX, 0, -1);

            MOV_ImmToReg(Reg_EAX, temp + (_s32) (_s16) __dotI);
            MOV_ImmToReg(Reg_ECX, ((uint32) (temp + (_s32) (_s16) __dotI)) >> SHIFTER2_WRITE);

            WC16(0x14FF);
            WC8(0x85|(Reg_ECX<<3));
            WC32((uint32) & gHardwareState.memory_write_fun_eax_only);
            LOGGING_DYNA(LogDyna("  CALL gHardwareState.memory_write_fun_eax_only[]\n"););

            MOV_MemoryToReg(Reg_ECX, ModRM_disp32, (uint32) & gHWS_fpr32[__FT<<Experiment]);
            MOV_RegToMemory(1, Reg_ECX, ModRM_EAX, 0);

            POP_RegIfMapped(Reg_ECX);
            POP_RegIfMapped(Reg_EAX);
        }
        else
        {
            MapRS;
            PUSH_RegIfMapped(Reg_EAX, 1, Reg_ECX);
            PUSH_RegIfMapped(Reg_ECX, 1, Reg_EAX);

            Lea(Reg_EAX, xRS->x86reg, (_s32) (_s16) __dotI);
            MOV_Reg2ToReg1(Reg_ECX, Reg_EAX);
            SHR_RegByImm(Reg_ECX, SHIFTER2_WRITE);
            WC16(0x14FF);
            WC8(0x85|(Reg_ECX<<3));
            WC32((uint32) & gHardwareState.memory_write_fun_eax_only);
            LOGGING_DYNA(LogDyna("  CALL gHardwareState.memory_write_fun_eax_only[]\n"););
            MOV_MemoryToReg(Reg_ECX, ModRM_disp32, (uint32) & gHWS_fpr32[__FT<<Experiment]);
            MOV_RegToMemory(1, Reg_ECX, ModRM_EAX, 0);

            POP_RegIfMapped(Reg_ECX);
            POP_RegIfMapped(Reg_EAX);
        }
    }
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void sdc1(OP_PARAMS)
{
    cop1_with_exception(PASS_PARAMS);
    
    OpcodePrologue(1, &r4300i_sdc1, 0, PASS_PARAMS);

#ifdef SAFE_LOADSTORE_FPU
    INTERPRET(r4300i_sdc1);
	return;
#endif

  
    FlushAllRegisters(FLUSH_TO_MEMORY);

    MOV_MemoryToReg(Reg_EDX, ModRM_disp32, (uint32)&gHWS_GPR(__RS));

    Lea(Reg_ECX, Reg_EDX, __dotI);
    Lea(Reg_EAX, Reg_EDX, __dotI);
    SHR_RegByImm(Reg_ECX, SHIFTER2_WRITE);

    WC16(0x14FF);
    WC8(0x85 | (Reg_ECX<<3));
    WC32((uint32) & gHardwareState.memory_write_fun_eax_only);

    
    MOV_MemoryToReg(Reg_ECX, ModRM_disp32, (uint32)&gHWS_fpr32[__RT<<Experiment]);
    MOV_MemoryToReg(Reg_EDX, ModRM_disp32, (uint32)&gHWS_fpr32[__RT<<Experiment]+4);
    MOV_RegToMemory(1, Reg_ECX, ModRM_disp8_EAX, 4);
    MOV_RegToMemory(1, Reg_EDX, ModRM_EAX, 0);
}