//Currently, this opcode map points to 64bit functions.
//However, there are 32bit functions in this file.
//Once we've written a complete set of 32bit interpreter functions, 
//then we can change this opcode map to point to the 32bit functions.
//At the moment, only the 32bit dyna core is calling these 32bit interpreter
//functions.

//This opcode map might also be very important when we have an exception in the 32bit core
//when Dyna_Exception_Execute_Handler calls interpreter functions.

//Maybe we can check some flag to see if the n64 is in 32bit or 64bit mode. Need to look into this.
//A mode change to 64bit will sign extend all general purpose registers. 
//Of course, they would need to be flushed b4 conversion.
//This probably only depends on the type of exception. For example, if we had an address error exception,
//I don't think the mode is important, but if we have an exception where the sign is 
//important, like with an overflow exception, then I would worry a little.



/*
 * 1964 Copyright (C) 1999-2004 Joel Middendorf, <schibo@emulation64.com> This
 * program is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. To contact the
 * authors: email: schibo@emulation64.com, rice1964@yahoo.com
 */
#include "stdafx.h"


void	instr32(uint32 Instruction);
void	REGIMM_instr32(uint32 Instruction);
void	COP0_instr32(uint32 Instruction);
void	COP1_instr32(uint32 Instruction);
void	TLB_instr32(uint32 Instruction);
void	COP1_BC_instr32(uint32 Instruction);
void	COP1_S_instr32(uint32 Instruction);
void	COP1_D_instr32(uint32 Instruction);
void	COP1_W_instr32(uint32 Instruction);
void	COP1_L_instr32(uint32 Instruction);

void (*CPU_instruction32[64]) (uint32 Instruction) =
{
	instr32, REGIMM_instr32, r4300i_j,	r4300i_jal,	r4300i_beq,	r4300i_bne,	r4300i_blez, r4300i_bgtz,
	r4300i_addi,	//8
	r4300i_addiu,	//9
	r4300i_slti,	//10
	r4300i_sltiu,	//11
	r4300i_andi,	//12
	r4300i_ori,		//13
	r4300i_xori,	//14
	r4300i_lui,		//15
	COP0_instr32,		//16
	COP1_instr32,		//17
	UNUSED,			//18
	UNUSED,			//19
	r4300i_beql,	//20
	r4300i_bnel,	//21
	r4300i_blezl,	//22
	r4300i_bgtzl,	//23
	r4300i_daddi,	//24
	r4300i_daddiu,	//25
	r4300i_ldl,		//26
	r4300i_ldr,		//27
	UNUSED,			//28
	UNUSED,			//29
	UNUSED,			//30
	UNUSED,			//31
	r4300i_lb,		//32
	r4300i_lh,		//33
	r4300i_lwl,		//34
	r4300i_lw,		//35
	r4300i_lbu,		//36
	r4300i_lhu,		//37
	r4300i_lwr,		//38
	r4300i_lwu,		//39
	r4300i_sb,		//40
	r4300i_sh,		//41
	r4300i_swl,		//42
	r4300i_sw,		//43
	r4300i_sdl,		//44
	r4300i_sdr,		//45
	r4300i_swr,		//46
	r4300i_cache,	//47
	r4300i_ll,		//48
	r4300i_lwc1,	//49
	UNUSED,			//50
	UNUSED,			//51
	r4300i_lld,		//52
	r4300i_ldc1,	//53
	UNUSED,			//54
	r4300i_ld,		//55
	r4300i_sc,		//56
	r4300i_swc1,	//57
	UNUSED,			//58
	UNUSED,			//59
	r4300i_scd,		//60
	r4300i_sdc1,	//61
	UNUSED,			//62
	r4300i_sd		//63
};

void (*r4300i_Instruction32[64]) (uint32 Instruction) =
{
	r4300i_sll,		//0
	UNUSED,			//1
	r4300i_srl,		//2
	r4300i_sra,		//3
	r4300i_sllv,	//4
	UNUSED,			//5
	r4300i_srlv,	//6
	r4300i_srav,	//7
	r4300i_jr,		//8
	r4300i_jalr,	//9
	UNUSED,			//10
	UNUSED,			//11
	r4300i_syscall,	//12
	r4300i_break,	//13
	UNUSED,			//14
	r4300i_sync,	//15
	r4300i_mfhi,	//16
	r4300i_mthi,	//17
	r4300i_mflo,	//18
	r4300i_mtlo,	//19
	r4300i_dsllv,	//20
	UNUSED,			//21
	r4300i_dsrlv,	//22
	r4300i_dsrav,	//23
	r4300i_mult,	//24
	r4300i_multu,	//25
	r4300i_div,		//26
	r4300i_divu,	//27
	r4300i_dmult,	//28
	r4300i_dmultu,	//29
	r4300i_ddiv,	//30
	r4300i_ddivu,	//31
	r4300i_add,		//32
	r4300i_addu,	//33
	r4300i_sub,		//34
	r4300i_subu,	//35
	r4300i_and,		//36
	r4300i_or,		//37
	r4300i_xor,		//38
	r4300i_nor,		//39
	UNUSED,			//40
	UNUSED,			//41
	r4300i_slt,		//42
	r4300i_sltu,	//43
	r4300i_dadd,	//44
	r4300i_daddu,	//45
	r4300i_dsub,	//46
	r4300i_dsubu,	//47
	r4300i_tge,		//48
	r4300i_tgeu,	//49
	r4300i_tlt,		//50
	r4300i_tltu,	//51
	r4300i_teq,		//52
	UNUSED,			//53
	r4300i_tne,		//54
	UNUSED,			//55
	r4300i_dsll,	//56
	UNUSED,			//57
	r4300i_dsrl,	//58
	r4300i_dsra,	//59
	r4300i_dsll32,	//60
	UNUSED,			//61
	r4300i_dsrl32,	//62
	r4300i_dsra32	//63
};

void (*REGIMM_Instruction32[32]) (uint32 Instruction) =
{
	r4300i_bltz,
	r4300i_bgez,
	r4300i_bltzl,
	r4300i_bgezl,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	r4300i_tgei,
	r4300i_tgeiu,
	r4300i_tlti,
	r4300i_tltiu,
	r4300i_teqi,
	UNUSED,
	r4300i_tnei,
	UNUSED,
	r4300i_bltzal,
	r4300i_bgezal,
	r4300i_bltzall,
	r4300i_bgezall,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED
};

void (*COP0_Instruction32[32]) (uint32 Instruction) =
{
	r4300i_COP0_mfc0,
	UNUSED,
	UNUSED,
	UNUSED,
	r4300i_COP0_mtc0,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	TLB_instr32,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED
};

void (*TLB_Instruction32[64]) (uint32 Instruction) =
{
	UNUSED,				//0
	r4300i_COP0_tlbr,	//1
	r4300i_COP0_tlbwi,	//2
	UNUSED,				//3
	UNUSED,				//4
	UNUSED,				//5
	r4300i_COP0_tlbwr,	//6
	UNUSED,				//7
	r4300i_COP0_tlbp,	//8
	UNUSED,				//9
	UNUSED,				//10
	UNUSED,				//11
	UNUSED,				//12
	UNUSED,				//13
	UNUSED,				//14
	UNUSED,				//15
	UNUSED,				//16
	UNUSED,				//17
	UNUSED,				//18
	UNUSED,				//19
	UNUSED,				//20
	UNUSED,				//21
	UNUSED,				//22
	UNUSED,				//23
	r4300i_COP0_eret,	//24
	UNUSED,				//25
	UNUSED,				//26
	UNUSED,				//27
	UNUSED,				//28
	UNUSED,				//29
	UNUSED,				//30
	UNUSED,				//31
	UNUSED,				//32
	UNUSED,				//33
	UNUSED,				//34
	UNUSED,				//35
	UNUSED,				//36
	UNUSED,				//37
	UNUSED,				//38
	UNUSED,				//39
	UNUSED,				//40
	UNUSED,				//41
	UNUSED,				//42
	UNUSED,				//43
	UNUSED,				//44
	UNUSED,				//45
	UNUSED,				//46
	UNUSED,				//47
	UNUSED,				//48
	UNUSED,				//49
	UNUSED,				//50
	UNUSED,				//51
	UNUSED,				//52
	UNUSED,				//53
	UNUSED,				//54
	UNUSED,				//55
	UNUSED,				//56
	UNUSED,				//57
	UNUSED,				//58
	UNUSED,				//59
	UNUSED,				//60
	UNUSED,				//61
	UNUSED,				//62
	UNUSED 				//63
};

void (*COP1_Instruction32[32]) (uint32 Instruction) =
{
	r4300i_COP1_mfc1,
	r4300i_COP1_dmfc1,
	r4300i_COP1_cfc1,
	UNUSED,
	r4300i_COP1_mtc1,
	r4300i_COP1_dmtc1,
	r4300i_COP1_ctc1,
	UNUSED,
	COP1_BC_instr32,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	COP1_S_instr32,
	COP1_D_instr32,
	UNUSED,
	UNUSED,
	COP1_W_instr32,
	COP1_L_instr32,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED
};

void (*COP1_BC_Instruction32[32]) (uint32 Instruction) =
{
	r4300i_COP1_bc1f, r4300i_COP1_bc1t, r4300i_COP1_bc1fl, r4300i_COP1_bc1tl, UNUSED, UNUSED, UNUSED, UNUSED,
	UNUSED,           UNUSED,           UNUSED,            UNUSED,            UNUSED, UNUSED, UNUSED, UNUSED,
	UNUSED,           UNUSED,           UNUSED,            UNUSED,            UNUSED, UNUSED, UNUSED, UNUSED,
	UNUSED,           UNUSED,           UNUSED,            UNUSED,            UNUSED, UNUSED, UNUSED, UNUSED
};

void (*COP1_S_Instruction32[64]) (uint32 Instruction) =
{
	r4300i_COP1_add_s,
	r4300i_COP1_sub_s,
	r4300i_COP1_mul_s,
	r4300i_COP1_div_s,
	r4300i_COP1_sqrt_s,
	r4300i_COP1_abs_s,
	r4300i_COP1_mov_s,
	r4300i_COP1_neg_s,
	r4300i_COP1_roundl_s,
	r4300i_COP1_truncl_s,
	r4300i_COP1_ceill_s,
	r4300i_COP1_floorl_s,
	r4300i_COP1_roundw_s,
	r4300i_COP1_truncw_s,
	r4300i_COP1_ceilw_s,
	r4300i_COP1_floorw_s,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	r4300i_COP1_cvtd_s,
	UNUSED,
	UNUSED,
	r4300i_COP1_cvtw_s,
	r4300i_COP1_cvtl_s,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	r4300i_C_F_S,
	r4300i_C_UN_S,
	r4300i_C_EQ_S,
	r4300i_C_UEQ_S,
	r4300i_C_OLT_S,
	r4300i_C_ULT_S,
	r4300i_C_OLE_S,
	r4300i_C_ULE_S,
	r4300i_C_SF_S,
	r4300i_C_NGLE_S,
	r4300i_C_SEQ_S,
	r4300i_C_NGL_S,
	r4300i_C_LT_S,
	r4300i_C_NGE_S,
	r4300i_C_LE_S,
	r4300i_C_NGT_S
};

void (*COP1_D_Instruction32[64]) (uint32 Instruction) =
{
	r4300i_COP1_add_d,
	r4300i_COP1_sub_d,
	r4300i_COP1_mul_d,
	r4300i_COP1_div_d,
	r4300i_COP1_sqrt_d,
	r4300i_COP1_abs_d,
	r4300i_COP1_mov_d,
	r4300i_COP1_neg_d,
	r4300i_COP1_roundl_d,
	r4300i_COP1_truncl_d,
	r4300i_COP1_ceill_d,
	r4300i_COP1_floorl_d,
	r4300i_COP1_roundw_d,
	r4300i_COP1_truncw_d,
	r4300i_COP1_ceilw_d,
	r4300i_COP1_floorw_d,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	r4300i_COP1_cvts_d,
	UNUSED,
	UNUSED,
	UNUSED,
	r4300i_COP1_cvtw_d,
	r4300i_COP1_cvtl_d,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	r4300i_C_F_D,
	r4300i_C_UN_D,
	r4300i_C_EQ_D,
	r4300i_C_UEQ_D,
	r4300i_C_OLT_D,
	r4300i_C_ULT_D,
	r4300i_C_OLE_D,
	r4300i_C_ULE_D,
	r4300i_C_SF_D,
	r4300i_C_NGLE_D,
	r4300i_C_SEQ_D,
	r4300i_C_NGL_D,
	r4300i_C_LT_D,
	r4300i_C_NGE_D,
	r4300i_C_LE_D,
	r4300i_C_NGT_D
};

void (*COP1_W_Instruction32[64]) (uint32 Instruction) =
{
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	r4300i_COP1_cvts_w,
	r4300i_COP1_cvtd_w,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED
};

void (*COP1_L_Instruction32[64]) (uint32 Instruction) =
{
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	r4300i_COP1_cvts_l,
	r4300i_COP1_cvtd_l,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED,
	UNUSED
};

void instr32(uint32 Instruction){
	r4300i_Instruction32[_FUNCTION_](Instruction);
}
void REGIMM_instr32(uint32 Instruction){
	REGIMM_Instruction32[RT_FT](Instruction);
}
void COP0_instr32(uint32 Instruction){
	COP0_Instruction32[RS_BASE_FMT](Instruction);
}
void COP1_instr32(uint32 Instruction){
	COP1_Instruction32[RS_BASE_FMT](Instruction);
}
void TLB_instr32(uint32 Instruction){
	TLB_Instruction32[_FUNCTION_](Instruction);
}
void COP1_BC_instr32(uint32 Instruction){
	COP1_BC_Instruction32[RT_FT](Instruction);
}
void COP1_S_instr32(uint32 Instruction){
	COP1_S_Instruction32[_FUNCTION_](Instruction);
}
void COP1_D_instr32(uint32 Instruction){
	COP1_D_Instruction32[_FUNCTION_](Instruction);
}
void COP1_W_instr32(uint32 Instruction){
	COP1_W_Instruction32[_FUNCTION_](Instruction);
}
void COP1_L_instr32(uint32 Instruction){
	COP1_L_Instruction32[_FUNCTION_](Instruction);
}

void r4300i_lwl_32bit(uint32 Instruction)
{
	uint32			LoadWord1;
	uint32			rt_ft = RT_FT;
    
    LOAD_TLB_FUN

        _asm {
		mov eax, QuerAddr
		mov ecx, QuerAddr
        and eax, 0xfffffffc
        shr ecx, SHIFTER2_READ
		call gHardwareState.memory_read_functions[ecx * 4]
        mov eax, [eax]
        mov dword ptr LoadWord1, eax
    }

	switch(QuerAddr & 3)
	{
	case 0:
		gHWS_GPR(RT_FT) = (unsigned __int32) LoadWord1;
		break;
	case 1:
		gHWS_GPR(RT_FT) = (((uint32) gHWS_GPR(RT_FT)) & 0x000000ff) | (LoadWord1 << 8);
		break;
	case 2:
		gHWS_GPR(RT_FT) = (((uint32) gHWS_GPR(RT_FT)) & 0x0000ffff) | (LoadWord1 << 16);
		break;
	case 3:
		gHWS_GPR(RT_FT) = (((uint32) gHWS_GPR(RT_FT)) & 0x00ffffff) | (LoadWord1 << 24);
		break;
	}
}

//------------------------------------------------------------------------

void r4300i_lwr_32bit(uint32 Instruction)
{
	uint32			LoadWord1;
	uint32			rt_ft = RT_FT;
	uint32			vAddr;
	LOAD_TLB_FUN	vAddr = QuerAddr & 0xfffffffc;

    _asm {
		mov eax, QuerAddr
		mov ecx, QuerAddr
        and eax, 0xfffffffc
        shr ecx, SHIFTER2_READ
		call gHardwareState.memory_read_functions[ecx * 4]
        mov eax, [eax]
        mov dword ptr LoadWord1, eax
    }


	switch(QuerAddr & 3)
	{
	case 3:
		gHWS_GPR(RT_FT) = (uint32) LoadWord1;
		break;
	case 2:
		gHWS_GPR(RT_FT) = ((((uint32) gHWS_GPR(RT_FT)) & 0xff000000) | (LoadWord1 >> 8));
		break;
	case 1:
		gHWS_GPR(RT_FT) = ((((uint32) gHWS_GPR(RT_FT)) & 0xffff0000) | (LoadWord1 >> 16));
		break;
	case 0:
		gHWS_GPR(RT_FT) = ((((uint32) gHWS_GPR(RT_FT)) & 0xffffff00) | (LoadWord1 >> 24));
		break;
	}
}

