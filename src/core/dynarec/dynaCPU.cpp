
// dynaCPU.c: Compile integer operation codes.


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


//#define SAFE_IMM 1
//#define NO_CONSTS 1
//#define SAFE_SLT 1
//#define SAFE_GATES 1
//#define SAFE_MATH 1
//#define SAFE_LOADSTORE 1
//#define SAFE_SHIFTS 1
//#define SAFE_DOUBLE_SHIFTS2 1
//#define SAFE_DOUBLE_IMM 1

#define SAFE_DOUBLE_SHIFTS 1
#define SAFE_DOUBLE_MATH 1


// opcode flow special cases

#define LOGIC_32BIT                               0x80000000
#define LOGIC_HI_IGNORE_IF_NONNEGATIVE            0x40000000
#define LOGIC_HI_IGNORE_IF_NEGATIVE               0x20000000
#define LOGIC_OP_IGNORE_IF_BOTH_OPERANDS_ARE_ZERO 0x10000000
#define LOGIC_XOR_IF_BOTH_OPERANDS_ARE_EQUAL      0x08000000

// special cases for individual opcodes
#define LOGIC_OR  (LOGIC_32BIT | LOGIC_HI_IGNORE_IF_NONNEGATIVE)
#define LOGIC_NOR (LOGIC_32BIT | LOGIC_HI_IGNORE_IF_NONNEGATIVE)
#define LOGIC_XOR (LOGIC_32BIT | LOGIC_HI_IGNORE_IF_NONNEGATIVE)
#define LOGIC_ADD (LOGIC_32BIT | LOGIC_HI_IGNORE_IF_NONNEGATIVE)
#define LOGIC_AND (LOGIC_32BIT | LOGIC_HI_IGNORE_IF_NEGATIVE)

#include "dynahelper.h"

extern unsigned long lCodeSize;

extern DWORD CompileSourceCode (char* _szSourceCode, BYTE* _pMachineCode, DWORD _dwMaxSize);
extern void DynaBufferOverrun(void);
void CommittTheSource(int NeedsFlush)
{
}

#include "dynabranch.h"


extern int __cdecl	SetStatusBar( char *debug, ...);
extern void			HELP_debug( unsigned long pc);
extern x86regtyp	x86reg[8];
extern void			FlushConstants( int TO_XMM_Or_Memory);
extern void			FlushRegister(int To_XMM_Or_Memory,  int k);
extern void			MapConst( x86regtyp *xMAP, int value);
extern void			RefreshOpList( char *opcode);
extern uint32		r4300i_lbu_faster( uint32 QuerAddr);
extern uint32		r4300i_lhu_faster( uint32 QuerAddr);
extern void			r4300i_sb_faster( uint32 QuerAddr);
extern void			r4300i_sh_faster( uint32 QuerAddr);
extern void			COP0_instr( uint32 Instruction);
extern void			(*cop1_Instruction[])( OP_PARAMS);
extern int			FindFreeRegister(void);
extern int			ItIsARegisterNotToUse(int k);
extern void			MapRegisterNew(int To_XMM_Or_Memory, x86regtyp *Conditions, x86regtyp *xFrom, int keep2, int keep3);

int				CheckWhereIsMipsReg( int mips_reg);
uint32			r4300i_sb_faster2( uint32 QuerAddr);
FlushedMap		FlushedRegistersMap[NUM_CONSTS];
x86regtyp		xRD[1];
x86regtyp		xRS[1];
x86regtyp		xRT[1];

dyn_cpu_instr		now_do_dyna_instruction[64] = {
	   special, regimm, j,    jal,    beq,    bne,    blez,   bgtz,
	   addi,    addiu,  slti, sltiu,  andi,   ori,    xori,   lui,
       cop0,    cop1,   cop2, resrvd, beql,   bnel,   blezl,  bgtzl,
       daddi,   daddiu, ldl,  ldr,    resrvd, resrvd, resrvd, resrvd,
       
       lb,      lh,     lwl,  lw,     lbu,    lhu,    lwr,    lwu,
       sb,      sh,     swl,  sw,     sdl,    sdr,    swr,    cache,
/*48*/ ll,      lwc1,   lwc2, resrvd, lld,    ldc1,   ldc2,   ld,
/*56*/ sc,      swc1,   swc2, resrvd, scd,    sdc1,   sdc2,   sd
};

dyn_cpu_instr		dyna_special_instruction[64] = {
	shift, resrvd, shift,  shift,  shiftv,  resrvd, shiftv, shiftv,
	jr,    jalr,   resrvd, resrvd, syscall, Break,  resrvd, sync,
	mf_mt, mf_mt,  mf_mt,  mf_mt,  dsllv,   resrvd, dsrlv,  dsrav,
	mul,   mul,    Div,    divu,   dmult,   dmultu, ddiv,   ddivu,

	add,   addu,   sub,    subu,   and,     or,     xor,    nor,
	resrvd,resrvd, slt,    sltu,   dadd,    daddu,  dsub,   dsubu,
	tge,   tgeu,   tlt,    tltu,   teq,     resrvd, tne,    resrvd,
	dsll,  resrvd, dsrl,   dsra,   dsll32,  resrvd, dsrl32, dsra32
};

dyn_cpu_instr		dyna_regimm_instruction[32] = {
	_bltz,   _bgez,   _bltzl,   _bgezl,   resrvd, resrvd, resrvd, resrvd,
	_tgei,   _tgeiu,  _tlti,    _tltiu,   _teqi,  resrvd, _tnei,  resrvd,
	_bltzal, _bgezal, _bltzall, _bgezall, resrvd, resrvd, resrvd, resrvd,
	resrvd,  resrvd,  resrvd,   resrvd,   resrvd, resrvd, resrvd, resrvd
};

dyn_cpu_instr		dyna_CP0_rs_instruction[32] = {
	mf,       dmf,    cf,     resrvd, mt,     dmt,    ct,     resrvd,
	bc,       resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
	cop0_tlb, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
	resrvd,   resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd
};

dyn_cpu_instr		dyna_CP0_rt_instruction[32] = {
	bcf,    bct,    bcfl,   bctl,   resrvd, resrvd, resrvd, resrvd,
	resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
	resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd,
	resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd
};

dyn_cpu_instr		dyna_tlb_instruction[64] = {
	invld,     cop0_tlbr, cop0_tlbwi, invld, invld, invld, cop0_tlbwr, invld,
	cop0_tlbp, invld,     invld,      invld, invld, invld, invld,      invld,
	resrvd,    invld,     invld,      invld, invld, invld, invld,      invld,
	cop0_eret, invld,     invld,      invld, invld, invld, invld,      invld,
	invld,     invld,     invld,      invld, invld, invld, invld,      invld,
	invld,     invld,     invld,      invld, invld, invld, invld,      invld,
	invld,     invld,     invld,      invld, invld, invld, invld,      invld,
	invld,     invld,     invld,      invld, invld, invld, invld,      invld
};

dyn_cpu_instr		dyna_cop2_rs_instruction[32] =
{
	no_cop2, no_cop2, no_cop2, no_cop2, no_cop2, no_cop2, no_cop2, no_cop2,
	no_cop2, no_cop2, no_cop2, no_cop2, no_cop2, no_cop2, no_cop2, no_cop2,
	no_cop2, no_cop2, no_cop2, no_cop2, no_cop2, no_cop2, no_cop2, no_cop2,
	no_cop2, no_cop2, no_cop2, no_cop2, no_cop2, no_cop2, no_cop2, no_cop2
};

// _____________________________________________________________________________
//

#define LOADSTORE_STEP 1
#define INTERPRET_LOADSTORE(OPCODE) \
	{ \
		int temp; \
		if( ConstMap[__RS].IsMapped) FlushOneConstant(FLUSH_TO_MEMORY, __RS); \
		if( ConstMap[__RT].IsMapped) FlushOneConstant(FLUSH_TO_MEMORY, __RT); \
		if( (temp = CheckWhereIsMipsReg( __RS)) > -1) FlushRegister(FLUSH_TO_MEMORY, temp); \
		if( (temp = CheckWhereIsMipsReg( __RT)) > -1) FlushRegister(FLUSH_TO_MEMORY, temp); \
		PushMap(); \
		MOV_ImmToReg(Reg_ECX, r.r_.code); \
		X86_CALL( (uint32)&OPCODE); \
		PopMap(); \
	}

#define INTERPRET_LOADSTORE_32BITOPTION(OPCODE_32BIT, OPCODE_64BIT)  \
{   \
    if (currentromoptions.Assume_32bit == ASSUME_32BIT_YES) \
        {INTERPRET_LOADSTORE(OPCODE_32BIT)}   \
    else    \
        {INTERPRET_LOADSTORE(OPCODE_64BIT)}   \
}


#define INTERPRET( OPCODE) { \
	FlushAllRegisters(FLUSH_TO_MEMORY); \
	MOV_ImmToReg(Reg_ECX, gHWS_code); \
	X86_CALL( (uint32)&OPCODE);	\
}

// _____________________________________________________________________________
//
void
cop0( OP_PARAMS)
{
	dyna_CP0_rs_instruction[__RS]( PASS_PARAMS);
}

// _____________________________________________________________________________
//
void
cop1( OP_PARAMS)
{
	cop1_Instruction[__RS]( PASS_PARAMS);
}

typedef void (*_x86Func2)(_u32, _u32);
typedef void (*_x86Func3)(_u32, _u32, _u32);
typedef void (*_x86Func4)(_u32, _u32, _u32, _u32 );

void OpcodePrologue(int clocks, void (__fastcall *FuncAddress)(unsigned __int32 ), int Use32, OP_PARAMS)
{
	compilerstatus.cp0Counter += clocks;

	if (Use32)
        SetRdRsRt32bit(PASS_PARAMS);
    else
        SetRdRsRt64bit(PASS_PARAMS);
}

// _____________________________________________________________________________
//
void
Set32bit()
{
	xRD->Is32bit = 1;
	xRS->Is32bit = 1;
	xRT->Is32bit = 1;
}

/*
 =======================================================================================================================
    This function has 3 advantages: - Reduces register pressure. - Doesn't need the mov instruction between 2
    registers. - Allows for better instruction pairing What it does: When RD != RT, we can use RT as RD (and free the
    old RD) when we know the following: - RT is not used in the remainder of the block - RT is not dirty Notes: - With
    this optimization, there can be no errors in AnalyzeBlock(). - You can only use this function when you've already
    confirmed that RT is not a constant, due to this function's use of MapRT_To() - Use this function in place of
    MapRT_To(xRD, 1, MOV_MemoryToReg)
	This is for 32bit instructions only!
 =======================================================================================================================
 */


int Map_RD_NE_RT2(x86regtyp* xRD, x86regtyp* xRT)
{
	int tempRT, tempRD;

	if
	(
		((tempRT = CheckWhereIsMipsReg(xRT->mips_reg)) > -1)
	&&	(currentromoptions.Advanced_Block_Analysis == USEBLOCKANALYSIS_YES)
	&&	(x86reg[tempRT].IsDirty == 0)
	&&	(ConstMap[xRT->mips_reg].FinalAddressUsedAt <= gHWS_pc)
	)
	{
		/* Free RT's hi. */
		if(x86reg[tempRT].HiWordLoc != tempRT)
		{
			x86reg[x86reg[tempRT].HiWordLoc].IsDirty = 0;
			x86reg[x86reg[tempRT].HiWordLoc].mips_reg = -1;
			x86reg[tempRT].Is32bit = 1;
			x86reg[tempRT].HiWordLoc = tempRT;
		}

		if(ConstMap[xRD->mips_reg].IsMapped) ConstMap[xRD->mips_reg].IsMapped = 0;
		if((tempRD = CheckWhereIsMipsReg(xRD->mips_reg)) > -1)
		{
			x86reg[tempRD].IsDirty = 0;
			FlushRegister(FLUSH_TO_MEMORY, tempRD);
		}

		/* Maps RT */
		x86reg[tempRT].x86reg = tempRT;
		x86reg[tempRT].mips_reg = xRD->mips_reg;
		x86reg[tempRT].HiWordLoc = tempRT;
		x86reg[tempRT].IsDirty = 1;
		xRD->x86reg = tempRT;
        xRD->HiWordLoc = tempRT;

        return 1;
	}
	else
	{
        return 0;
    }
}

void Map_RD_NE_RT()
{
	int tempRT, tempRD;

	if
	(
		((tempRT = CheckWhereIsMipsReg(xRT->mips_reg)) > -1)
	&&	(currentromoptions.Advanced_Block_Analysis == USEBLOCKANALYSIS_YES)
	&&	(x86reg[tempRT].IsDirty == 0)
	&&	(ConstMap[xRT->mips_reg].FinalAddressUsedAt <= gHWS_pc)
	)
	{
		/* Free RT's hi. */
		if(x86reg[tempRT].HiWordLoc != tempRT)
		{
			x86reg[x86reg[tempRT].HiWordLoc].IsDirty = 0;
			x86reg[x86reg[tempRT].HiWordLoc].mips_reg = -1;
			x86reg[tempRT].Is32bit = 1;
			x86reg[tempRT].HiWordLoc = tempRT;
		}

		if(ConstMap[xRD->mips_reg].IsMapped) ConstMap[xRD->mips_reg].IsMapped = 0;
		if((tempRD = CheckWhereIsMipsReg(xRD->mips_reg)) > -1)
		{
			x86reg[tempRD].IsDirty = 0;
			FlushRegister(FLUSH_TO_MEMORY, tempRD);
		}

		/* Maps RT */
		x86reg[tempRT].x86reg = tempRT;
		x86reg[tempRT].mips_reg = xRD->mips_reg;
		x86reg[tempRT].HiWordLoc = tempRT;
		x86reg[tempRT].IsDirty = 1;
		xRD->x86reg = tempRT;
	}
	else
	{
        xRD->IsDirty = 1;
		
        xRD->NoNeedToLoadTheLo = 0;
        xRD->NoNeedToLoadTheHi = 0;

        MapRegisterNew(FLUSH_TO_MEMORY, xRD, xRT, xRT->mips_reg, xRS->mips_reg);

	}
}

// _____________________________________________________________________________
//

//Currently for 32bit only. we can update it for 64bit.
void Map_RD_NE_RS(void)
{
	int temp, tempRD;

	if
	(
		((temp = CheckWhereIsMipsReg(xRS->mips_reg)) > -1)
	&&	(currentromoptions.Advanced_Block_Analysis == USEBLOCKANALYSIS_YES)
	&&	(x86reg[temp].IsDirty == 0)
	&&	(ConstMap[xRT->mips_reg].FinalAddressUsedAt <= gHWS_pc)
	)
	{
		/* Free RT's hi. */
		if(x86reg[temp].HiWordLoc != temp)
		{
			x86reg[x86reg[temp].HiWordLoc].IsDirty = 0;
			x86reg[x86reg[temp].HiWordLoc].mips_reg = -1;
			x86reg[temp].Is32bit = 1;
			x86reg[temp].HiWordLoc = temp;
		}

		if(ConstMap[xRD->mips_reg].IsMapped) ConstMap[xRD->mips_reg].IsMapped = 0;
		if((tempRD = CheckWhereIsMipsReg(xRD->mips_reg)) > -1)
		{
			x86reg[tempRD].IsDirty = 0;
			FlushRegister(FLUSH_TO_MEMORY, tempRD);
		}

		/* Maps RS */
		x86reg[temp].x86reg = temp;
		x86reg[temp].mips_reg = xRD->mips_reg;
		x86reg[temp].HiWordLoc = temp;
		x86reg[temp].IsDirty = 1;
		xRD->x86reg = temp;
	}
	else
	{
        xRD->IsDirty = 1;
		xRD->NoNeedToLoadTheLo = 0;
        xRD->NoNeedToLoadTheHi = 0;
        MapRegisterNew(FLUSH_TO_MEMORY, xRD, xRS, xRS->mips_reg, xRT->mips_reg);
    }
}

void Dummy2(_u32 dummy1, _u32 dummy2)
{
}

// __________________________________________________________________________________________________
// Immediate_LogicNew
//
void Immediate_LogicNew(x86regtyp* xRT, x86regtyp* xRS, int _Use32bit, int SpecialCases, int _iConstantResult, int _iImmediate, _x86Func3 px86FunctionA,
        _x86Func2 px86FunctionB)
{
	if(ConstMap[xRS->mips_reg].IsMapped == 1)
		MapConst(xRT, _iConstantResult);
	else 
	{
        xRT->Is32bit = _Use32bit;
		xRT->IsDirty = 1;

        if (!_Use32bit)
            if (CheckIs32Bit(xRS->mips_reg))
                xRT->Is32bit = 1;

        //map
        MapRegisterNew(FLUSH_TO_MEMORY, xRT, xRS, xRS->mips_reg, -1);
        
        //lo
        px86FunctionA(xRT->x86reg, _iImmediate, 0);

        //hi
        if (!xRT->Is32bit)
        {
            if (_iImmediate & 0x80000000) //negative
            {
                if (SpecialCases & LOGIC_HI_IGNORE_IF_NEGATIVE)
                    return;
            }
            else //nonnegative
                if ((SpecialCases & LOGIC_HI_IGNORE_IF_NONNEGATIVE) && (px86FunctionB == &Dummy2))
                    return;

            px86FunctionA(xRT->HiWordLoc, _iImmediate>>31, 0);
        }

        //not
        px86FunctionB(1, xRT->x86reg);
        if (!xRT->Is32bit)
            px86FunctionB(1, xRT->HiWordLoc);
	}
}

// __________________________________________________________________________________________________
// Helper_Immediate_else_rRegToReg
void Helper_Immediate_else_RegToReg(int SpecialCaseFlags, x86regtyp* xRD, x86regtyp* xRS, _s32 _iConstantResult, _x86Func2 px86FunctionA, _x86Func2  px86FunctionB, _x86Func3 px86FunctionImm)
{
    int rd = xRD->mips_reg;
    int rs = xRS->mips_reg;

	if ( ConstMap[rs].IsMapped )
	{
		MapConst(xRD, _iConstantResult);
	}
	else
	{
        xRD->IsDirty = 1;

        if (!xRD->Is32bit)
            if (CheckIs32Bit(xRS->mips_reg))
                xRD->Is32bit = 1;

        MapRegisterNew(FLUSH_TO_MEMORY, xRS, xRS, rd, -1);
		MapRegisterNew(FLUSH_TO_MEMORY, xRD, xRD, rs, -1);

		px86FunctionA(xRD->x86reg, xRS->x86reg);
        if (!xRD->Is32bit)
        {
            px86FunctionA(xRD->HiWordLoc, xRS->HiWordLoc);
        }

		px86FunctionB(1, xRD->x86reg);
        if (!xRD->Is32bit)
            px86FunctionB(1, xRD->HiWordLoc);

	}
}

void Gate_Logic(int _Use32bit, int SpecialCaseFlags, x86regtyp* xRD, x86regtyp* xRS, x86regtyp* xRT, int _iConstantResult, _x86Func2 px86FunctionA, _x86Func2 px86FunctionB, _x86Func3 px86FunctionImm)
{
    int rd = xRD->mips_reg;
    int rs = xRS->mips_reg;
    int rt = xRT->mips_reg;

    xRD->Is32bit = _Use32bit;
    
    if (ConstMap[rs].IsMapped && ConstMap[rt].IsMapped)
        MapConst(xRD, _iConstantResult);

    else if (ConstMap[rt].IsMapped)
        Immediate_LogicNew(xRD, xRS, _Use32bit, SpecialCaseFlags, _iConstantResult, ConstMap[rt].value,  px86FunctionImm, px86FunctionB);

    else if (ConstMap[rs].IsMapped)
        Immediate_LogicNew(xRD, xRT, _Use32bit, SpecialCaseFlags, _iConstantResult, ConstMap[rs].value,  px86FunctionImm, px86FunctionB);

    else if (rd == rt)
	{
		if (rs == rt)
		{
			if (px86FunctionB != &Dummy2)
			{
				Helper_Immediate_else_RegToReg(SpecialCaseFlags, xRD, xRS, 
				  _iConstantResult, px86FunctionA, px86FunctionB, px86FunctionImm);
			}
			//else the instruction is ignored
		}
	
        else if (rd != rs) 
		{
			Helper_Immediate_else_RegToReg(SpecialCaseFlags, xRD, xRS,
			  _iConstantResult, px86FunctionA, px86FunctionB, px86FunctionImm);
		}
	}

    else if (rd == rs) 
	{
		Helper_Immediate_else_RegToReg(SpecialCaseFlags, xRD, xRT, 
	      _iConstantResult, px86FunctionA, px86FunctionB, px86FunctionImm);
	}

    else // rd != rs, rd != rt
	{
        if (rs != rt)
        {
            xRD->IsDirty = 1;

            if (!_Use32bit)
            {
                if (CheckIs32Bit(xRS->mips_reg) && CheckIs32Bit(xRT->mips_reg))
                {
                     xRD->Is32bit = 1;
                     xRS->Is32bit = 1;
                     xRT->Is32bit = 1;
                }
            }

            MapRegisterNew(FLUSH_TO_MEMORY, xRT, xRT, rs, rd); 
            MapRegisterNew(FLUSH_TO_MEMORY, xRD, xRS, rs, rt); 

            px86FunctionA(xRD->x86reg, xRT->x86reg);
            if (!xRD->Is32bit)
                px86FunctionA(xRD->HiWordLoc, xRT->HiWordLoc);

            px86FunctionB(1, xRD->x86reg);
            if (!xRD->Is32bit)
                px86FunctionB(1, xRD->HiWordLoc);

        }

        else //rs = rt
        {
            xRD->IsDirty = 1;

            if (!_Use32bit)
            {
                if (CheckIs32Bit(xRS->mips_reg))
                {
                     xRD->Is32bit = 1;
                     xRS->Is32bit = 1;
                }
            }

            MapRegisterNew(FLUSH_TO_MEMORY, xRD, xRS, rs, -1);


            if (!xRD->Is32bit)
                px86FunctionA(xRD->HiWordLoc, xRS->HiWordLoc);

            px86FunctionB(1, xRD->x86reg);
            if (!xRD->Is32bit)
                px86FunctionB(1, xRD->HiWordLoc);
        }
	}
}


// _____________________________________________________________________________
//
void ori(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_ori, 0, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;

#ifdef SAFE_IMM
	INTERPRET(r4300i_ori);
	return;
#endif

	Immediate_LogicNew(xRT, xRS, 0, LOGIC_OR, ConstMap[xRS->mips_reg].value | (_u32)(_u16)__I, (_u32)(_u16)__I, &OR_ImmToReg, &Dummy2);
}

// _____________________________________________________________________________
//
void xori(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_xori, 0, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;

	/* 64bit */
#ifdef SAFE_IMM
	INTERPRET(r4300i_xori);
	return;
#endif

     Immediate_LogicNew(xRT, xRS, 0, LOGIC_XOR, ConstMap[xRS->mips_reg].value ^ (_u32)(_u16)__I, (_u32)(_u16)__I, &XOR_ImmToReg, &Dummy2);
}

// _____________________________________________________________________________
//
void addi(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_addi, 1, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;

#ifdef SAFE_IMM
	INTERPRET(r4300i_addi);
	return;
#endif

    Immediate_LogicNew(xRT, xRS, 1, LOGIC_ADD, ConstMap[xRS->mips_reg].value+__I, __I, &ADD_ImmToReg, &Dummy2);
}

// _____________________________________________________________________________
//
void addiu(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_addiu, 1, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;

#ifdef SAFE_IMM
	INTERPRET(r4300i_addiu);
	return;
#endif
    
    Immediate_LogicNew(xRT, xRS, 1, LOGIC_ADD, ConstMap[xRS->mips_reg].value+__I, __I, &ADD_ImmToReg, &Dummy2);
}

// _____________________________________________________________________________
//
void andi(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_andi, 1, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;


#ifdef SAFE_IMM
	INTERPRET(r4300i_andi);
	return;
#endif

    Immediate_LogicNew(xRT, xRS, 1, LOGIC_AND, ConstMap[xRS->mips_reg].value & (_u32)(_u16)__I, (_u32)(_u16)__I, &AND_ImmToReg, &Dummy2);
}

// _____________________________________________________________________________
//
void lui(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_lui, 1, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;

#ifdef SAFE_IMM
	INTERPRET(r4300i_lui);
	return;
#endif
	
	MapConst(xRT, (_s32) ((__I) << 16));
}


// _____________________________________________________________________________
//
void or(OP_PARAMS)
{
	int Use32bit = 0;

    OpcodePrologue(1, &r4300i_or, 0, PASS_PARAMS);
    if(xRD->mips_reg == 0) return;

#ifdef SAFE_GATES
	INTERPRET(r4300i_or);
	return;
#endif

    Gate_Logic( (currentromoptions.Assume_32bit==ASSUME_32BIT_YES), 
        LOGIC_OR,
        xRD, xRS, xRT, ConstMap[xRS->mips_reg].value|ConstMap[xRT->mips_reg].value,
        &OR_Reg2ToReg1, &Dummy2, &OR_ImmToReg);
        return;
}


// _____________________________________________________________________________
//
void SSE_InitRegisterMap(void);
BOOL Init_Dynarec( void)
{
	int i;

	compilerstatus.lCodePosition = 32768;


	CompilingSlot = FALSE;
	compilerstatus.DynaBufferOverError = FALSE;

	SetTranslator( dyna_RecompCode, 0, RECOMPCODE_SIZE);

	InitRegisterMap();
    SSE_InitRegisterMap();
#ifdef LOG_DYNA
	InitLogDyna();
#endif

    memset( xRD, 0, sizeof( xRD));
	memset( xRS, 0, sizeof( xRS));
	memset( xRT, 0, sizeof( xRT));

	for( i = 0; i < 0x10000; i++)
		if( dynarommap[i] != NULL)
			memset( dynarommap[i], 0, 0x10000);

	memset( RDRAM_Copy, 0xEE, 0x00800000);
	return( TRUE);
}


// _____________________________________________________________________________
//
void
Free_Dynarec( void)
{
#ifdef LOG_DYNA
	CloseLogDyna();
#endif
}


// _____________________________________________________________________________
//
void Lea( uint32 rt, uint32 rs, uint32 imm)
{
	//lea rt, [rs+imm]
	if( imm == 0) 
		MOV_Reg2ToReg1(rt, rs);
	else if (rt == rs)
	{
		ADD_ImmToReg(rt, imm, 1); //flags matter for overflog check.
	}
	else {
		WC8( 0x8D);
		WC8( ModRM_disp32_EAX + (rs|(rt<<3)));
		WC32( imm);
	}
}


void SmartFreeRegisters()
{
    int k;

    if (currentromoptions.Advanced_Block_Analysis == USEBLOCKANALYSIS_YES)
    {
        for (k=0; k<8; k++)
            if (ItIsARegisterNotToUse(k));
            else if ((x86reg[k].mips_reg > -1) && (ConstMap[x86reg[k].mips_reg].FinalAddressUsedAt < gHWS_pc))
                FlushRegister(FLUSH_TO_MEMORY, k);
    }
}


// _____________________________________________________________________________
//
void
SetRdRsRt32bit(OP_PARAMS)
{
	memset( xRD, 0, sizeof( xRD));
	memset( xRS, 0, sizeof( xRS));
	memset( xRT, 0, sizeof( xRT));
	xRD->mips_reg = __RD;
	xRT->mips_reg = __RT;
	xRS->mips_reg = __RS;
	xRD->Is32bit = 1;
	xRS->Is32bit = 1;
	xRT->Is32bit = 1;

    SmartFreeRegisters();

	if(xRS->mips_reg == 0)
		MapConst(xRS, 0);
	else if(xRT->mips_reg == 0)
		MapConst(xRT, 0);
	else if(xRD->mips_reg == 0)
		MapConst(xRD, 0);
}


// _____________________________________________________________________________
//
void
SetRdRsRt64bit(OP_PARAMS)
{
	memset(xRD, 0, sizeof(xRD));
	memset(xRS, 0, sizeof(xRS));
	memset(xRT, 0, sizeof(xRT));
	xRD->mips_reg = __RD;
	xRT->mips_reg = __RT;
	xRS->mips_reg = __RS;


    SmartFreeRegisters();
    
    if(xRS->mips_reg == 0)
		MapConst(xRS, 0);
	else if(xRT->mips_reg == 0)
		MapConst(xRT, 0);
	else if(xRD->mips_reg == 0)
		MapConst(xRD, 0);
}

// _____________________________________________________________________________
//
void
SetRsRt32bit(OP_PARAMS)
{
	memset(xRS, 0, sizeof(xRS));
	memset(xRT, 0, sizeof(xRT));
	xRS->mips_reg = __RS;
	xRT->mips_reg = __RT;

	
    SmartFreeRegisters();
    
    if(xRS->mips_reg == 0)
		MapConst(xRS, 0);
	else if(xRT->mips_reg == 0)
		MapConst(xRT, 0);

	xRS->Is32bit = 1;
	xRT->Is32bit = 1;
}


// _____________________________________________________________________________
//
void
SetRsRt64bit(OP_PARAMS)
{
	memset(xRS, 0, sizeof(xRS));
	memset(xRT, 0, sizeof(xRT));
	xRS->mips_reg = __RS;
	xRT->mips_reg = __RT;

	if(xRS->mips_reg == 0)
		MapConst(xRS, 0);
	else if(xRT->mips_reg == 0)
		MapConst(xRT, 0);
}

// _____________________________________________________________________________
//
void
special(OP_PARAMS)
{
	dyna_special_instruction[__F](PASS_PARAMS);
}


// _____________________________________________________________________________
//
void
regimm(OP_PARAMS)
{
	dyna_regimm_instruction[__RT](PASS_PARAMS);
}


// _____________________________________________________________________________
//
void
cop2(OP_PARAMS)
{
	dyna_cop2_rs_instruction[__RS](PASS_PARAMS);
}


// _____________________________________________________________________________
//
void
bc(OP_PARAMS)
{
	dyna_CP0_rt_instruction[__RT](PASS_PARAMS);
}


// _____________________________________________________________________________
//
void
cop0_tlb(OP_PARAMS)
{
	dyna_tlb_instruction[__F](PASS_PARAMS);
}


// _____________________________________________________________________________
//
void
invld(OP_PARAMS)
{
	DisplayError("invalid instruction");
}


// _____________________________________________________________________________
//
void
resrvd(OP_PARAMS)
{
#ifdef _DEBUG
	INTERPRET(UNUSED);
#endif
}


// _____________________________________________________________________________
//

void
lwl(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_lwl, 0, PASS_PARAMS);

    INTERPRET_LOADSTORE_32BITOPTION(r4300i_lwl_32bit, r4300i_lwl);
    if (xRT->mips_reg == 0)
    {
        MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0), 0);
        MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0)+4, 0);
    }
}


// _____________________________________________________________________________
//
void lwr(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_lwr, 0, PASS_PARAMS);

    INTERPRET_LOADSTORE_32BITOPTION(r4300i_lwr_32bit, r4300i_lwr);
    if (xRT->mips_reg == 0)
    {
        MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0), 0);
        MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0)+4, 0);
    }
}


// _____________________________________________________________________________
//
void
lwu(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_lwu, 0, PASS_PARAMS);

	INTERPRET_LOADSTORE(r4300i_lwu);
    if (xRT->mips_reg == 0)
    {
        MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0), 0);
        MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0)+4, 0);
    }
}


// _____________________________________________________________________________
//
void
ldl(OP_PARAMS)
{
	int RememberAssume32bit;
    
    OpcodePrologue(1, &r4300i_ldl, 0, PASS_PARAMS);

    RememberAssume32bit = currentromoptions.Assume_32bit;
    currentromoptions.Assume_32bit = ASSUME_32BIT_NO;
    
    INTERPRET_LOADSTORE(r4300i_ldl);
    if (xRT->mips_reg == 0)
    {
        MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0), 0);
        MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0)+4, 0);
    }

    currentromoptions.Assume_32bit = RememberAssume32bit;
}


// _____________________________________________________________________________
//
void
ldr(OP_PARAMS)
{
    int RememberAssume32bit;

    OpcodePrologue(1, &r4300i_ldr, 0, PASS_PARAMS);

    RememberAssume32bit = currentromoptions.Assume_32bit;
    currentromoptions.Assume_32bit = ASSUME_32BIT_NO;


	INTERPRET_LOADSTORE(r4300i_ldr);
    if (xRT->mips_reg == 0)
    {
        MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0), 0);
        MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0)+4, 0);
    }

    currentromoptions.Assume_32bit = RememberAssume32bit;
}

// _____________________________________________________________________________
//
void
swl(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_swl, 0, PASS_PARAMS);

	INTERPRET_LOADSTORE(r4300i_swl);
}

// _____________________________________________________________________________
//
void
sdl(OP_PARAMS)
{
	int RememberAssume32bit;
    
    OpcodePrologue(1, &r4300i_sdl, 0, PASS_PARAMS);

    RememberAssume32bit = currentromoptions.Assume_32bit;
    currentromoptions.Assume_32bit = ASSUME_32BIT_NO;

    INTERPRET_LOADSTORE(r4300i_sdl);

    currentromoptions.Assume_32bit = RememberAssume32bit;
}

// _____________________________________________________________________________
//
void
sdr(OP_PARAMS)
{
	int RememberAssume32bit;
    
    OpcodePrologue(1, &r4300i_sdr, 0, PASS_PARAMS);

    RememberAssume32bit = currentromoptions.Assume_32bit;
    currentromoptions.Assume_32bit = ASSUME_32BIT_NO;

   
    INTERPRET_LOADSTORE(r4300i_sdr);

    currentromoptions.Assume_32bit = RememberAssume32bit;
}

// _____________________________________________________________________________
//
void swr(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_swr, 0, PASS_PARAMS);

	INTERPRET_LOADSTORE(r4300i_swr);
}

// _____________________________________________________________________________
//
void ll(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_ll, 0, PASS_PARAMS);

	INTERPRET_LOADSTORE(r4300i_ll)
    if (xRT->mips_reg == 0)
    {
        MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0), 0);
        MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0)+4, 0);
    }
}

// _____________________________________________________________________________
//
void lwc2(OP_PARAMS)
{
	compilerstatus.cp0Counter += 1;
	TRACE1("Unhandled LWC2, pc=%08X", gHWS_pc);
}

// _____________________________________________________________________________
//
void lld(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_lld, 0, PASS_PARAMS);

	INTERPRET_LOADSTORE(r4300i_lld);
    if (xRT->mips_reg == 0)
    {
        MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0), 0);
        MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0)+4, 0);
    }
}

// _____________________________________________________________________________
//
void ldc2(OP_PARAMS)
{
	compilerstatus.cp0Counter += 1;
	TRACE1("Unhandled ldc2, pc=%08X", gHWS_pc);
}

// _____________________________________________________________________________
//
void sc(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_sc, 0, PASS_PARAMS);

    INTERPRET_LOADSTORE(r4300i_sc);
}

// _____________________________________________________________________________
//
void swc2(OP_PARAMS)
{
	compilerstatus.cp0Counter += 1;
	TRACE1("Unhandled swc2, pc=%08X", gHWS_pc);
}

// _____________________________________________________________________________
//
void scd(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_scd, 0, PASS_PARAMS);

	INTERPRET_LOADSTORE(r4300i_scd);
}

// _____________________________________________________________________________
//
void sdc2(OP_PARAMS)
{
	/* INTERPRET_LOADSTORE(r4300i_sdc2); */
}

// _____________________________________________________________________________
//
void daddi(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_daddi, 0, PASS_PARAMS);
    if (xRT->mips_reg == 0) return;

#ifdef SAFE_DOUBLE_IMM
	INTERPRET(r4300i_daddi);
	return;
#endif

	if((CheckIs32Bit(xRS->mips_reg)) || (currentromoptions.Assume_32bit == ASSUME_32BIT_YES))
	{
		INTERPRET(r4300i_daddi_32bit);
	}
	else
	{
		INTERPRET(r4300i_daddi);
	}
}

// _____________________________________________________________________________
//
void daddiu(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_daddiu, 0, PASS_PARAMS);
    if (xRT->mips_reg == 0) return;

#ifdef SAFE_DOUBLE_IMM
	INTERPRET(r4300i_daddiu);
	return;
#endif SAFE_DOUBLE_IMM

    if((CheckIs32Bit(xRS->mips_reg)) || (currentromoptions.Assume_32bit == ASSUME_32BIT_YES))
	{
		INTERPRET(r4300i_daddiu_32bit);
	}
	else
	{
		INTERPRET(r4300i_daddiu);
	}
}

// _____________________________________________________________________________
//
void dadd(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_dadd, 0, PASS_PARAMS);
    if (xRD->mips_reg == 0) return;


#ifdef SAFE_DOUBLE_MATH
	INTERPRET(r4300i_dadd);
#else
	if
	(
		(CheckIs32Bit(xRS->mips_reg) && CheckIs32Bit(xRT->mips_reg))
	||	(currentromoptions.Assume_32bit == ASSUME_32BIT_YES)
	)
	{
        {INTERPRET(r4300i_dadd_32bit);}
	}
	else
	{
        {INTERPRET(r4300i_dadd);}
	}
#endif
}

// _____________________________________________________________________________
//
void daddu(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_daddu, 0, PASS_PARAMS);
    if (xRD->mips_reg == 0) return;

#ifdef SAFE_DOUBLE_MATH
	INTERPRET(r4300i_daddu);
#else
	if
	(
		(CheckIs32Bit(xRS->mips_reg) && CheckIs32Bit(xRT->mips_reg))
	||	(currentromoptions.Assume_32bit == ASSUME_32BIT_YES)
	)
	{
		INTERPRET(r4300i_daddu_32bit);
	}
	else
	{
		INTERPRET(r4300i_daddu);
	}
#endif
}

// _____________________________________________________________________________
//
void dsub(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_dsub, 0, PASS_PARAMS);
    if (xRD->mips_reg == 0) return;

#ifdef SAFE_DOUBLE_MATH
	INTERPRET(r4300i_dsub);
#else

	if
	(
		(CheckIs32Bit(xRS->mips_reg) && CheckIs32Bit(xRT->mips_reg))
	||	(currentromoptions.Assume_32bit == ASSUME_32BIT_YES)
	)
	{
		sub(PASS_PARAMS);
	}
	else
	{
		INTERPRET(r4300i_dsub);
	}

#endif
}

// _____________________________________________________________________________
//
void dsubu(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_dsubu, 0, PASS_PARAMS);
    if (xRD->mips_reg == 0) return;


#ifdef SAFE_DOUBLE_MATH
	INTERPRET(r4300i_dsubu);
#else

	if
	(
		(CheckIs32Bit(xRS->mips_reg) && CheckIs32Bit(xRT->mips_reg))
	||	(currentromoptions.Assume_32bit == ASSUME_32BIT_YES)
	)
	{
		subu(PASS_PARAMS);
	}
	else
	{
		INTERPRET(r4300i_dsubu);
	}

#endif
}

// _____________________________________________________________________________
//
void tge(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_tge, 0, PASS_PARAMS);

    INTERPRET(r4300i_tge);
}

// _____________________________________________________________________________
//
void tgeu(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_tgeu, 0, PASS_PARAMS);

    INTERPRET(r4300i_tgeu);
}

// _____________________________________________________________________________
//
void tlt(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_tlt, 0, PASS_PARAMS);

	INTERPRET(r4300i_tlt);
}

// _____________________________________________________________________________
//
void tltu(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_tltu, 0, PASS_PARAMS);

    INTERPRET(r4300i_tltu);
}

// _____________________________________________________________________________
//
void teq(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_teq, 0, PASS_PARAMS);

    INTERPRET(r4300i_teq);
}

// _____________________________________________________________________________
//
void tne(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_tne, 0, PASS_PARAMS);

    INTERPRET(r4300i_tne);
}

// _____________________________________________________________________________
//
void _tgei(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_tgei, 0, PASS_PARAMS);

    INTERPRET(r4300i_tgei);
}

// _____________________________________________________________________________
//
void _tgeiu(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_tgeiu, 0, PASS_PARAMS);

    INTERPRET(r4300i_tgeiu);
}

// _____________________________________________________________________________
//
void _tlti(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_tlti, 0, PASS_PARAMS);

	INTERPRET(r4300i_tlti);
}

// _____________________________________________________________________________
//
void _tltiu(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_tltiu, 0, PASS_PARAMS);

	INTERPRET(r4300i_tltiu);
}

// _____________________________________________________________________________
//
void _teqi(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_teqi, 0, PASS_PARAMS);

	INTERPRET(r4300i_teqi);
}

// _____________________________________________________________________________
//
void _tnei(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_tnei, 0, PASS_PARAMS);

	INTERPRET(r4300i_tnei);
}

// _____________________________________________________________________________
//
void dsllv(OP_PARAMS)
{
    int temp2;
    int RememberAssume32bit;
    int k;
    
    OpcodePrologue(1, &r4300i_dsllv, 0, PASS_PARAMS);
    if (xRD->mips_reg == 0) return;


#ifdef SAFE_DOUBLE_SHIFTS2
	INTERPRET(r4300i_dsllv);
#else

        RememberAssume32bit = currentromoptions.Assume_32bit;
        currentromoptions.Assume_32bit = ASSUME_32BIT_NO;

        if(ConstMap[xRT->mips_reg].IsMapped)
            FlushOneConstant(FLUSH_TO_MEMORY, xRT->mips_reg);
		else if((temp2 = CheckWhereIsMipsReg(xRT->mips_reg)) > -1)
		    FlushRegister(FLUSH_TO_MEMORY, temp2);


        if(ConstMap[xRS->mips_reg].IsMapped)
            FlushOneConstant(FLUSH_TO_MEMORY, xRS->mips_reg);
		else if((temp2 = CheckWhereIsMipsReg(xRS->mips_reg)) > -1)
		    FlushRegister(FLUSH_TO_MEMORY, temp2);


        /* Protect ECX */
    	if(x86reg[Reg_ECX].mips_reg > -1)
        {
         //   if( (x86reg[Reg_ECX].mips_reg != xRS->mips_reg) /*|| (xRD->mips_reg == xRS->mips_reg)*/)
            {
                FlushRegister(FLUSH_TO_MEMORY, Reg_ECX);
                x86reg[Reg_ECX].mips_reg = 100;				/* protect ecx. */
                x86reg[Reg_ECX].HiWordLoc = Reg_ECX;
                x86reg[Reg_ECX].Is32bit = 1;
                x86reg[Reg_ECX].IsDirty = 0;
            }
        }
        else if(x86reg[1].mips_reg == -2)
        {
        	for(k = 0; k < 8; k++)
		    {
        		if(ItIsARegisterNotToUse(k));
                else if(x86reg[k].HiWordLoc == Reg_ECX)
		        {
		            FlushRegister(FLUSH_TO_MEMORY, k);
                    x86reg[Reg_ECX].mips_reg = 100;				/* protect ecx. */
                    x86reg[Reg_ECX].HiWordLoc = Reg_ECX;
                    x86reg[Reg_ECX].Is32bit = 1;
                    x86reg[Reg_ECX].IsDirty = 0;
			        k = 9;
                }
            }
        }
        else
        {
            //it is unused. STILL Protect it.  
            x86reg[Reg_ECX].mips_reg = 100;				/* protect ecx. */
            x86reg[Reg_ECX].HiWordLoc = Reg_ECX;
            x86reg[Reg_ECX].Is32bit = 1;
            x86reg[Reg_ECX].IsDirty = 0;
        }


        xRD->NoNeedToLoadTheLo = 1;
        xRD->NoNeedToLoadTheHi = 1;
        xRD->IsDirty = 1;

        MapRD;

        MOV_MemoryToReg(Reg_ECX, ModRM_disp32, (uint32)&gHWS_GPR(xRS->mips_reg));
        AND_ImmToReg(Reg_ECX, 0x3f, 0);
        CMP_RegWithImm(Reg_ECX, 32);
        Jcc_auto(CC_GE, 2); //jmp dsll32-like behavior


        MOV_MemoryToReg(xRD->x86reg, ModRM_disp32, (uint32)&gHWS_GPR(xRT->mips_reg));
        MOV_MemoryToReg(xRD->HiWordLoc, ModRM_disp32, (uint32)&gHWS_GPR(xRT->mips_reg)+4);

        //Do nothing else if a shift is 0.
        CMP_RegWith0(Reg_ECX);
        Jcc_Near_auto(CC_E, 5);

        //dsll-like behavior for  1 <= shift_range <= 31

        MOV_Reg2ToReg1(Reg_EBP, xRD->x86reg); //temp register.
        SHL_RegByCL(xRD->HiWordLoc);
        SHL_RegByCL(xRD->x86reg);        

        // shr ebp, 32 - shift amount in ecx

        //negate ecx and add 32.
        XOR_ShortToReg(Reg_ECX, 0xffffffff);
        INC_Reg(1, Reg_ECX);
//        ADD_ImmToReg(Reg_ECX, 32);

        SHR_RegByCL(Reg_EBP);

        OR_Reg2ToReg1(xRD->HiWordLoc, Reg_EBP);

        MOV_ImmToReg(Reg_EBP, HardwareStart);
        JMP_Short_auto(6);

     SetTarget(2);

        //dsll32-like behavior for 32 <= shift_range <= 63
        SUB_ImmFromReg(1, Reg_ECX, 32, 0);
        MOV_MemoryToReg(xRD->HiWordLoc, ModRM_disp32, (uint32)&gHWS_GPR(xRT->mips_reg));
        XOR_Reg2ToReg1(xRD->x86reg, xRD->x86reg);
        SHL_RegByCL(xRD->HiWordLoc);


	/* Unprotect ECX */
	if( x86reg[1].mips_reg == 100)
        x86reg[1].mips_reg = -1;

    SetNearTarget(5);
    SetTarget(6);


    FlushRegister(FLUSH_TO_MEMORY, xRD->x86reg);
    currentromoptions.Assume_32bit = RememberAssume32bit;

#endif
}


// _____________________________________________________________________________
//
void dsrlv(OP_PARAMS)
{
    int temp2;
    int RememberAssume32bit;
    int k, ExtraRegister;
    
    OpcodePrologue(1, &r4300i_dsrlv, 0, PASS_PARAMS);
    if (xRD->mips_reg == 0) return;


#ifdef SAFE_DOUBLE_SHIFTS2
	INTERPRET(r4300i_dsrlv);
#else

        RememberAssume32bit = currentromoptions.Assume_32bit;
        currentromoptions.Assume_32bit = ASSUME_32BIT_NO;

        if(ConstMap[xRT->mips_reg].IsMapped)
            FlushOneConstant(FLUSH_TO_MEMORY, xRT->mips_reg);
		else if((temp2 = CheckWhereIsMipsReg(xRT->mips_reg)) > -1)
		    FlushRegister(FLUSH_TO_MEMORY, temp2);


        if(ConstMap[xRS->mips_reg].IsMapped)
            FlushOneConstant(FLUSH_TO_MEMORY, xRS->mips_reg);
		else if((temp2 = CheckWhereIsMipsReg(xRS->mips_reg)) > -1)
		    FlushRegister(FLUSH_TO_MEMORY, temp2);


        /* Protect ECX */
    	if(x86reg[Reg_ECX].mips_reg > -1)
        {
         //   if( (x86reg[Reg_ECX].mips_reg != xRS->mips_reg) /*|| (xRD->mips_reg == xRS->mips_reg)*/)
            {
                FlushRegister(FLUSH_TO_MEMORY, Reg_ECX);
                x86reg[Reg_ECX].mips_reg = 100;				/* protect ecx. */
                x86reg[Reg_ECX].HiWordLoc = Reg_ECX;
                x86reg[Reg_ECX].Is32bit = 1;
                x86reg[Reg_ECX].IsDirty = 0;
            }
        }
        else if(x86reg[1].mips_reg == -2)
        {
        	for(k = 0; k < 8; k++)
		    {
        		if(ItIsARegisterNotToUse(k));
                else if(x86reg[k].HiWordLoc == Reg_ECX)
		        {
		            FlushRegister(FLUSH_TO_MEMORY, k);
                    x86reg[Reg_ECX].mips_reg = 100;				/* protect ecx. */
                    x86reg[Reg_ECX].HiWordLoc = Reg_ECX;
                    x86reg[Reg_ECX].Is32bit = 1;
                    x86reg[Reg_ECX].IsDirty = 0;
			        k = 9;
                }
            }
        }
        else
        {
            //it is unused. STILL Protect it.  
            x86reg[Reg_ECX].mips_reg = 100;				/* protect ecx. */
            x86reg[Reg_ECX].HiWordLoc = Reg_ECX;
            x86reg[Reg_ECX].Is32bit = 1;
            x86reg[Reg_ECX].IsDirty = 0;
        }


        xRD->NoNeedToLoadTheLo = 1;
        xRD->NoNeedToLoadTheHi = 1;
        xRD->IsDirty = 1;

        MapRD;

        MOV_MemoryToReg(Reg_ECX, ModRM_disp32, (uint32)&gHWS_GPR(xRS->mips_reg));
        AND_ImmToReg(Reg_ECX, 0x3f, 0);
        CMP_RegWithImm(Reg_ECX, 32);
        Jcc_auto(CC_GE, 2); //jmp dslr32-like behavior


        MOV_MemoryToReg(xRD->x86reg, ModRM_disp32, (uint32)&gHWS_GPR(xRT->mips_reg));
        MOV_MemoryToReg(xRD->HiWordLoc, ModRM_disp32, (uint32)&gHWS_GPR(xRT->mips_reg)+4);

        //Do nothing else if a shift is 0.
        CMP_RegWith0(Reg_ECX);
        Jcc_Near_auto(CC_E, 5);

        //dsrl-like behavior for  1 <= shift_range <= 31

        ExtraRegister=FindFreeRegister();
        if (ExtraRegister < Reg_ECX)
            ExtraRegister = Reg_EBP;

        MOV_Reg2ToReg1(ExtraRegister, xRD->HiWordLoc); //temp register.
        SHR_RegByCL(xRD->HiWordLoc);
        SHR_RegByCL(xRD->x86reg);        

        // shr ebp, 32 - shift amount in ecx

        //negate ecx and add 32.
        XOR_ShortToReg(Reg_ECX, 0xffffffff);
        INC_Reg(1, Reg_ECX);
//        ADD_ImmToReg(Reg_ECX, 32);

        SHL_RegByCL(ExtraRegister);

        OR_Reg2ToReg1(xRD->x86reg, ExtraRegister);

        if (ExtraRegister == Reg_EBP)
            MOV_ImmToReg(Reg_EBP, HardwareStart);
        JMP_Short_auto(6);

     SetTarget(2);

        //dslr32-like behavior for 32 <= shift_range <= 63
        SUB_ImmFromReg(1, Reg_ECX, 32, 0);
        MOV_MemoryToReg(xRD->x86reg, ModRM_disp32, (uint32)&gHWS_GPR(xRT->mips_reg)+4);
        XOR_Reg2ToReg1(xRD->HiWordLoc, xRD->HiWordLoc);
        SHR_RegByCL(xRD->x86reg);


	/* Unprotect ECX */
	if( x86reg[1].mips_reg == 100)
        x86reg[1].mips_reg = -1;

    SetNearTarget(5);
    SetTarget(6);

    FlushRegister(FLUSH_TO_MEMORY, xRD->x86reg);
    currentromoptions.Assume_32bit = RememberAssume32bit;


#endif
}
// _____________________________________________________________________________
//
void dsrav(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_dsrav, 0, PASS_PARAMS);
    if (xRD->mips_reg == 0) return;

#ifdef SAFE_DOUBLE_SHIFTS
	INTERPRET(r4300i_dsrav);
#else
	
	INTERPRET(r4300i_dsrav);

#endif
}

// _____________________________________________________________________________
//
void dsll(OP_PARAMS)
{
    int temp2;
    int RememberAssume32bit;
    
    OpcodePrologue(1, &r4300i_dsll, 0, PASS_PARAMS);
    if (xRD->mips_reg == 0) return;

#ifdef SAFE_DOUBLE_SHIFTS2
	INTERPRET(r4300i_dsll);
#else

        RememberAssume32bit = currentromoptions.Assume_32bit;
        currentromoptions.Assume_32bit = ASSUME_32BIT_NO;

        if(ConstMap[xRT->mips_reg].IsMapped)
            FlushOneConstant(FLUSH_TO_MEMORY, xRT->mips_reg);
		else if((temp2 = CheckWhereIsMipsReg(xRT->mips_reg)) > -1)
		    FlushRegister(FLUSH_TO_MEMORY, temp2);

        xRD->NoNeedToLoadTheLo = 1;
        xRD->NoNeedToLoadTheHi = 1;
        xRD->IsDirty = 1;
        MapRD;


        MOV_MemoryToReg(xRD->x86reg, ModRM_disp32, (uint32)&gHWS_GPR(xRT->mips_reg));
        MOV_MemoryToReg(xRD->HiWordLoc, ModRM_disp32, (uint32)&gHWS_GPR(xRT->mips_reg)+4);
        MOV_Reg2ToReg1(Reg_EBP, xRD->x86reg); //temp register.

        if (__SA != 0) //Important
        {
            SHL_RegByImm(xRD->HiWordLoc, __SA);
            SHR_RegByImm(Reg_EBP, 32-(__SA));
            SHL_RegByImm(xRD->x86reg, __SA);
            OR_Reg2ToReg1(xRD->HiWordLoc, Reg_EBP);
        }


        MOV_ImmToReg(Reg_EBP, HardwareStart);
        FlushRegister(FLUSH_TO_MEMORY, xRD->x86reg);

        currentromoptions.Assume_32bit = RememberAssume32bit;

#endif
}

// _____________________________________________________________________________
//
void dsrl(OP_PARAMS)
{
    int temp2;
    int RememberAssume32bit;
    
    OpcodePrologue(1, &r4300i_dsrl, 0, PASS_PARAMS);
    if (xRD->mips_reg == 0) return;


#ifdef SAFE_DOUBLE_SHIFTS2
	INTERPRET(r4300i_dsrl);
#else

        RememberAssume32bit = currentromoptions.Assume_32bit;
        currentromoptions.Assume_32bit = ASSUME_32BIT_NO;

        if(ConstMap[xRT->mips_reg].IsMapped)
            FlushOneConstant(FLUSH_TO_MEMORY, xRT->mips_reg);
		else if((temp2 = CheckWhereIsMipsReg(xRT->mips_reg)) > -1)
		    FlushRegister(FLUSH_TO_MEMORY, temp2);

        xRD->NoNeedToLoadTheLo = 1;
        xRD->NoNeedToLoadTheHi = 1;
        xRD->IsDirty = 1;
        MapRD;


        MOV_MemoryToReg(xRD->x86reg, ModRM_disp32, (uint32)&gHWS_GPR(xRT->mips_reg));
        MOV_MemoryToReg(xRD->HiWordLoc, ModRM_disp32, (uint32)&gHWS_GPR(xRT->mips_reg)+4);
        MOV_Reg2ToReg1(Reg_EBP, xRD->HiWordLoc); //temp register.

        if (__SA != 0) //Important
        {
            SHR_RegByImm(xRD->HiWordLoc, __SA);
            SHL_RegByImm(Reg_EBP, 32-(__SA));
            SHR_RegByImm(xRD->x86reg, __SA);
            OR_Reg2ToReg1(xRD->x86reg, Reg_EBP);
        }


        MOV_ImmToReg(Reg_EBP, HardwareStart);
        FlushRegister(FLUSH_TO_MEMORY, xRD->x86reg);

        currentromoptions.Assume_32bit = RememberAssume32bit;

#endif
}

// _____________________________________________________________________________
//
void dsra(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_dsra, 0, PASS_PARAMS);
    if (xRD->mips_reg == 0) return;

#ifdef SAFE_DOUBLE_SHIFTS
	INTERPRET(r4300i_dsra);
#else

	if((CheckIs32Bit(xRT->mips_reg)) || (currentromoptions.Assume_32bit == ASSUME_32BIT_YES))
	{
		_SAFETY_CPU_(r4300i_dsra) 
		compilerstatus.cp0Counter -= 1;	/* shift() adds 1 to counter. */
		shift(PASS_PARAMS);
	}
	else
	{
		_SAFETY_CPU_(r4300i_dsra) 
		INTERPRET(r4300i_dsra);
	}

#endif
}

// _____________________________________________________________________________
//
void dmult(OP_PARAMS)
{
    OpcodePrologue(2, &r4300i_dmult, 0, PASS_PARAMS);

#ifdef SAFE_DOUBLE_MATH
	INTERPRET(r4300i_dmult);
#else

	INTERPRET(r4300i_dmult);

#endif
}

// _____________________________________________________________________________
//
void dmultu(OP_PARAMS)
{
    OpcodePrologue(5, &r4300i_dmultu, 0, PASS_PARAMS);

#ifdef SAFE_DOUBLE_MATH
	INTERPRET(r4300i_dmultu);
#else

	INTERPRET(r4300i_dmultu);

#endif
}

// _____________________________________________________________________________
//
void ddiv(OP_PARAMS)
{
    OpcodePrologue(139, &r4300i_ddiv, 0, PASS_PARAMS);

#ifdef SAFE_DOUBLE_MATH
	INTERPRET(r4300i_ddiv);
#else

	if
	(
		(CheckIs32Bit(xRS->mips_reg))
	&&	(CheckIs32Bit(xRT->mips_reg))
	||	(currentromoptions.Assume_32bit == ASSUME_32BIT_YES)
	)
	{
		compilerstatus.cp0Counter += 1;	/* no overlap. */
		_SAFETY_CPU_(r4300i_ddiv) compilerstatus.cp0Counter -= 75;	/* div() adds 75 to count. */
		Div(PASS_PARAMS);
	}
	else
	{
		compilerstatus.cp0Counter += 1;	/* no overlap. */
		_SAFETY_CPU_(r4300i_ddiv) INTERPRET(r4300i_ddiv);
	}

#endif
}

// _____________________________________________________________________________
//
void ddivu(OP_PARAMS)
{
    OpcodePrologue(139, &r4300i_ddivu, 0, PASS_PARAMS);

#ifdef SAFE_DOUBLE_MATH
	INTERPRET(r4300i_ddivu);
#else

	if
	(
		(CheckIs32Bit(xRS->mips_reg))
	&&	(CheckIs32Bit(xRT->mips_reg))
	||	(currentromoptions.Assume_32bit == ASSUME_32BIT_YES)
	)
	{
		compilerstatus.cp0Counter += 139;
		_SAFETY_CPU_(r4300i_ddivu)
        compilerstatus.cp0Counter -= 75;	// divu() adds 75 to count.
		divu(PASS_PARAMS);
	}
	else
	{
		compilerstatus.cp0Counter += 139;
		_SAFETY_CPU_(r4300i_ddivu) INTERPRET(r4300i_ddivu);
	}

#endif
}

// _____________________________________________________________________________
//
void mf(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_COP0_mfc0, 1, PASS_PARAMS);

	switch(__RD)
	{
      case RANDOM:
	  case COUNT:
        INTERPRET(r4300i_COP0_mfc0);
        if (xRT->mips_reg == 0)
        {
          MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0), 0);
          MOV_ImmToMemory(1, ModRM_disp32, (uint32)&gHWS_GPR(0)+4, 0);
        }
	  break;

	  case INDEX:
      case EPC:
      case ENTRYLO0:
      case ENTRYLO1:
      case PAGEMASK:
      case ENTRYHI:
	  default:
	    if (xRT->mips_reg != 0)
        {
          xRT->NoNeedToLoadTheLo = 1;
		  xRT->IsDirty = 1;
		  MapRT;
		  MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (uint32)&gHWS_COP0Reg[__RD]);
        }
	  break;
	}
}

// _____________________________________________________________________________
//
void dmf(OP_PARAMS)
{
	_u32	rd = (unsigned long) &(_u32) reg->COP0Reg[__RD];

//TODO: Finish Prologue
    OpcodePrologue(1, &UNUSED, 0, PASS_PARAMS);
	if(xRT->mips_reg == 0) return;

	xRT->NoNeedToLoadTheLo = 1;
	xRT->IsDirty = 1;
	MapRT;
	MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, rd);
}

// _____________________________________________________________________________
//
void cf(OP_PARAMS)
{
//TODO: Finish Prologue
    OpcodePrologue(1, &UNUSED, 1, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;

	xRT->IsDirty = 1;
	xRT->NoNeedToLoadTheLo = 1;
	MapRT;
	MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (unsigned long) &reg->COP0Con[__FS]);
}

// _____________________________________________________________________________
//
extern void Preserve();			
extern void Restore();
extern void TriggerCPUInterruptExceptionNewer(int ReturnPC, int DelaySlot);

void mt(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_COP0_mtc0, 1, PASS_PARAMS);

	switch(__RD)
	{
		case EPC:		/* The INDEX Register */
			MapRT;
			MOV_RegToMemory(1, xRT->x86reg, ModRM_disp32, (uint32)&gHardwareState.COP0Reg[EPC]);
			break;
		case CAUSE:

			{
			int Reg_k = 0;
			int pc = 0;

			int RememberKeepCompiling = compilerstatus.KEEP_RECOMPILING;

            MapRT;
			compilerstatus.IsFirstFPUInstructionToTest = TRUE;
			if (xRT->x86reg == Reg_EAX)
				Reg_k = Reg_ECX;
			else
				Reg_k = Reg_EAX;

			PUSH_RegIfMapped(Reg_k, 1, xRT->x86reg);
			PUSH_RegToStack(xRT->x86reg);
			AND_ImmToReg(Reg_k, ~0x300, 0);
			AND_ImmToReg(xRT->x86reg, 0x300, 1);
			Jcc_Near_auto(CC_NE, 1);

			OR_Reg2ToReg1(Reg_k, xRT->x86reg);
			MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (uint32)&gHWS_COP0Reg[STATUS]);
			MOV_RegToMemory(1, Reg_k, ModRM_disp32, (uint32)&gHWS_COP0Reg[CAUSE]);
			AND_ImmToReg(xRT->x86reg, 0x0000ff00, 0);
			TEST_Reg2WithReg1(xRT->x86reg, Reg_k);
			POP_RegFromStack(xRT->x86reg);
			POP_RegIfMapped(Reg_k);
			Jcc_Near_auto(CC_NE, 2);

			Preserve();
			FlushAllRegisters(FLUSH_TO_MEMORY);
			MOV_ImmToReg(Reg_ECX, gHWS_pc);
			XOR_Reg2ToReg1(Reg_EDX, Reg_EDX);
			if (compilerstatus.BranchDelay)
				INC_Reg(1, Reg_EDX);

			X86_CALL((uint32)&TriggerCPUInterruptExceptionNewer); //this raises an exception for our new 1964 exception vector.
			Restore();

			compilerstatus.KEEP_RECOMPILING = RememberKeepCompiling;

			JMP_Short_auto(3);
			SetNearTarget(1);

			OR_Reg2ToReg1(Reg_k, xRT->x86reg);
			MOV_RegToMemory(1, Reg_k, ModRM_disp32, (uint32)&gHWS_COP0Reg[CAUSE]);
			POP_RegFromStack(xRT->x86reg);
			POP_RegIfMapped(Reg_k);

			SetNearTarget(2);
			SetTarget(3);
			}
			break;

		case INDEX:
		case WIRED:
		case COUNT:
		case COMPARE:
            INTERPRET(r4300i_COP0_mtc0);
			break;

		case STATUS:
            INTERPRET(r4300i_COP0_mtc0);
            compilerstatus.IsFirstFPUInstructionToTest = TRUE;
            MOV_ImmToMemory(1, ModRM_disp32, (unsigned long) &r.r_.pc, gHWS_pc+4);
            Interrupts(JUMP_TYPE_INDIRECT, 0, LINK_NO, 0);

			break;

		//read-only registers
		case RANDOM:
		case PREVID:
		case BADVADDR:
			return;
		case PAGEMASK:
			//MessageBox(0, "PageMask", "", 0);
			if (ConstMap[xRT->mips_reg].IsMapped)
				MOV_ImmToMemory(1, ModRM_disp32, 
					(uint32)&gHardwareState.COP0Reg[__RD],
					ConstMap[xRT->mips_reg].value&0x01ffe000);
			else
			{
				MapRT;
				PUSH_RegToStack(xRT->x86reg);
				AND_ImmToReg(xRT->x86reg, 0x01ffe000, 0);
				MOV_RegToMemory(1, xRT->x86reg, ModRM_disp32, (uint32)&gHardwareState.COP0Reg[__RD]);
				POP_RegFromStack(xRT->x86reg);
			}
			break;
		case ENTRYHI:
			//	gHWS_COP0Reg[ENTRYHI] = (uint32) gRT & 0xFFFFE0FF;
			if( ConstMap[xRT->mips_reg].IsMapped)
				MOV_ImmToMemory( 1, ModRM_disp32, 
					(uint32)&gHardwareState.COP0Reg[__RD],
					ConstMap[xRT->mips_reg].value&0xFFFFE0FF);
			else {
				MapRT;
				PUSH_RegToStack(xRT->x86reg);
				AND_ImmToReg(xRT->x86reg, 0xFFFFE0FF, 0);
				MOV_RegToMemory(1, xRT->x86reg, ModRM_disp32, (uint32)&gHardwareState.COP0Reg[__RD]);
				POP_RegFromStack(xRT->x86reg);
			}
			break;
		case ENTRYLO0:
		case ENTRYLO1:
			if (ConstMap[xRT->mips_reg].IsMapped)
				MOV_ImmToMemory(1, ModRM_disp32, 
					(uint32)&gHardwareState.COP0Reg[__RD],
					ConstMap[xRT->mips_reg].value&0x3fffffff);
			else
			{
				MapRT;
				PUSH_RegToStack(xRT->x86reg);
				AND_ImmToReg(xRT->x86reg, 0x3fffffff, 0);
				MOV_RegToMemory(1, xRT->x86reg, ModRM_disp32, (uint32)&gHardwareState.COP0Reg[__RD]);
				POP_RegFromStack(xRT->x86reg);
			}
			break;

		default:
			
			if (ConstMap[xRT->mips_reg].IsMapped)
				MOV_ImmToMemory(1, ModRM_disp32, 
					(uint32)&gHardwareState.COP0Reg[__RD],
					ConstMap[xRT->mips_reg].value);
			else
			{
				MapRT;
				MOV_RegToMemory(1, xRT->x86reg, ModRM_disp32, (uint32)&gHardwareState.COP0Reg[__RD]);
			}
			break;
	}
}

// _____________________________________________________________________________
//
void dmt(OP_PARAMS)
{
//TODO: Prologue
    compilerstatus.cp0Counter += 1;
	DisplayError("%08X: Unhandled dmtc0", r.r_.pc);
}

// _____________________________________________________________________________
//
void ct(OP_PARAMS)
{
	_u32	fs = (unsigned long) &reg->COP0Con[__FS];


//TODO: Prologue
    compilerstatus.cp0Counter += 1;
	SetRdRsRt64bit(PASS_PARAMS);

	if(currentromoptions.Assume_32bit == ASSUME_32BIT_YES)
	{
		DisplayError("Need to compile ctc0 for 32bit. Please use 64bit for now.");
	}

	MapRT;
	MOV_RegToMemory(1, xRT->x86reg, ModRM_disp32, fs);
	MOV_RegToMemory(1, xRT->HiWordLoc, ModRM_disp32, fs + 4);
}

// _____________________________________________________________________________
//
void bcf(OP_PARAMS)
{
	compilerstatus.cp0Counter += 1;
	DisplayError("%08X: Unhandled BCFC0", r.r_.pc);
}

// _____________________________________________________________________________
//
void bct(OP_PARAMS)
{
	compilerstatus.cp0Counter += 1;
	DisplayError("%08X: Unhandled BCTC0", r.r_.pc);
}

// _____________________________________________________________________________
//
void bcfl(OP_PARAMS)
{
	compilerstatus.cp0Counter += 1;
	DisplayError("%08X: Unhandled BCFL0", r.r_.pc);
}

// _____________________________________________________________________________
//
void bctl(OP_PARAMS)
{
	compilerstatus.cp0Counter += 1;
	DisplayError("%08X: Unhandled BCTL0", r.r_.pc);
}

// _____________________________________________________________________________
//
void cop0_tlbr(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_COP0_tlbr, 0, PASS_PARAMS);

	INTERPRET(r4300i_COP0_tlbr);
}

// _____________________________________________________________________________
//
void cop0_tlbwi(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_COP0_tlbwi, 0, PASS_PARAMS);

	INTERPRET(r4300i_COP0_tlbwi);
}

// _____________________________________________________________________________
//
void cop0_tlbwr(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_COP0_tlbwr, 0, PASS_PARAMS);

	INTERPRET(r4300i_COP0_tlbwr);
}

// _____________________________________________________________________________
//
void cop0_tlbp(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_COP0_tlbp, 0, PASS_PARAMS);

	INTERPRET(r4300i_COP0_tlbp);
}

// _____________________________________________________________________________
//
void no_cop2(OP_PARAMS)
{
	compilerstatus.cp0Counter += 1;
	TRACE1("%08X: There isnt a COP2\n", r.r_.pc);
}

// _____________________________________________________________________________
//
void slt(OP_PARAMS)
{
	int Flag = 0;
/*
	__asm { 
		mov eax, 1
		cpuid
		and edx, (1<<15)  //CMOV supported?
		mov temp, edx
	}
*/
    OpcodePrologue(1, &r4300i_slt, 0, PASS_PARAMS);
    if(xRD->mips_reg == 0) return;


#ifdef SAFE_SLT
	INTERPRET(r4300i_slt);
	return;
#endif
	if(xRS->mips_reg == xRT->mips_reg)
	{
		MapConst(xRD, 0);
	}

	// 32bit
	else if
		(
		(currentromoptions.Assume_32bit == ASSUME_32BIT_YES)
		||	(CheckIs32Bit(xRS->mips_reg) && CheckIs32Bit(xRT->mips_reg))
		)
	{

		xRD->Is32bit = 1;
		xRS->Is32bit = 1;
		xRT->Is32bit = 1;


    	if (ConstMap[xRS->mips_reg].IsMapped)
		{

            int Const = ConstMap[xRS->mips_reg].value;
            
            if (ConstMap[xRT->mips_reg].IsMapped)
			{
				if ((_int32)ConstMap[xRS->mips_reg].value < (_int32)ConstMap[xRT->mips_reg].value)
					MapConst(xRD, 1);
				else
					MapConst(xRD, 0);
				return;
			}
			else
			{

				//rs!=rt
				xRD->IsDirty = 1;
				
                MapRegisterNew(FLUSH_TO_MEMORY, xRD, xRT, xRT->mips_reg, -1);

				XOR_ImmToReg(xRD->x86reg, 0xffffffff, 0);
				ADD_ImmToReg(xRD->x86reg, Const+1, 0);
				SHR_RegByImm(xRD->x86reg, 31);
				return;
			}
		}

		if((xRD->mips_reg != xRS->mips_reg) && (xRD->mips_reg != xRT->mips_reg))
		{
			xRD->NoNeedToLoadTheLo = 1;
			Flag = 1;
		}

		xRD->IsDirty = 1;
        MapRS;
        MapRT;
        MapRD;

        if ((xRD->x86reg < 4) && (Flag))
			XOR_Reg2ToReg1(xRD->x86reg, xRD->x86reg);

		if (xRD->x86reg < 4)
		{
			if (Flag)
			{
				CMP_Reg2WithReg1(xRS->x86reg, xRT->x86reg);
				SETcc_Reg(CC_L, xRD->x86reg);
			}
			else
			{
				CMP_Reg2WithReg1(xRS->x86reg, xRT->x86reg);
				SETcc_Reg(CC_L, xRD->x86reg);
				AND_ImmToReg(xRD->x86reg, 1, 0);			
			}
			return;
		}
		else
		{
			CMP_Reg2WithReg1(xRS->x86reg, xRT->x86reg);
			Jcc_auto(CC_GE, 5);
		}

		MOV_ImmToReg(xRD->x86reg, 1);
		JMP_Short_auto(6);

		SetTarget(5);

		XOR_Reg2ToReg1(xRD->x86reg, xRD->x86reg);

		SetTarget(6);
	}
	// 64bit
	else
	{
        xRD->IsDirty = 1;

		if((xRD->mips_reg != xRS->mips_reg) && (xRD->mips_reg != xRT->mips_reg))
		{
			xRD->Is32bit = 1;
			xRD->NoNeedToLoadTheLo = 1;
            xRD->NoNeedToLoadTheLo = 1;
		}

		MapRS;
		MapRT;
		MapRD;

		CMP_Reg2WithReg1(xRS->HiWordLoc, xRT->HiWordLoc);

		Jcc_auto(CC_G, 2);
		Jcc_auto(CC_L, 1);

		CMP_Reg2WithReg1(xRS->x86reg, xRT->x86reg);
		Jcc_auto(CC_AE, 3);

		SetTarget(1);
		MOV_ImmToReg(xRD->x86reg, 1);
		JMP_Short_auto(4);

		SetTarget(2);
		SetTarget(3);

		XOR_Reg2ToReg1(xRD->x86reg, xRD->x86reg);

		SetTarget(4);

		if(xRD->Is32bit == 0)
		{
			xRD->Is32bit = 1;
			MapRD;	/* converts to 32bit */
		}
	}
}

// _____________________________________________________________________________
//
void sltu(OP_PARAMS)
{
	int Use32bit = 0;
	int Flag = 0;

    OpcodePrologue(1, &r4300i_sltu, 0, PASS_PARAMS);
    if(xRD->mips_reg == 0) return;


#ifdef SAFE_SLT
    INTERPRET(r4300i_sltu);
	return;
#endif

	Use32bit = ( (currentromoptions.Assume_32bit == ASSUME_32BIT_YES) || (CheckIs32Bit(xRS->mips_reg) && CheckIs32Bit(xRT->mips_reg))  );
	if (Use32bit)Set32bit();
    
    if(xRS->mips_reg == xRT->mips_reg)
	{
		MapConst(xRD, 0);
	}

	/* 32bit */
	else if	(Use32bit)
	{
        if (ConstMap[xRS->mips_reg].IsMapped)
		{
			if (ConstMap[xRT->mips_reg].IsMapped)
			{
				if ((uint32)ConstMap[xRS->mips_reg].value < (uint32)ConstMap[xRT->mips_reg].value)
					MapConst(xRD, 1);
				else
					MapConst(xRD, 0);
				return;
			}
			else
			{	//rs!=rt
				xRD->IsDirty = 1;
				if((xRD->mips_reg != xRS->mips_reg) && (xRD->mips_reg != xRT->mips_reg))
				{
					xRD->NoNeedToLoadTheLo = 1;
					Flag = 1;
				}

				//can't reorder these.
                MapRT;
				MapRD;

				if( xRD->x86reg < 4) {
					if ( Flag) {
						XOR_Reg2ToReg1(xRD->x86reg, xRD->x86reg);
						CMP_RegWithImm(xRT->x86reg, ConstMap[xRS->mips_reg].value);
						SETcc_Reg( CC_A, xRD->x86reg);
					} else {
						CMP_RegWithImm(xRT->x86reg, ConstMap[xRS->mips_reg].value);
						SETcc_Reg( CC_A, xRD->x86reg);
						AND_ImmToReg(xRD->x86reg, 1, 0);
					}
					return;
				}

				{
					int ExtraRegister = 0;

                    if (xRD->mips_reg != xRT->mips_reg)
                        ExtraRegister = xRD->x86reg;
                    else 
                    {
                        ExtraRegister = FindFreeRegister();
                        if (ExtraRegister < Reg_EAX)
                            ExtraRegister = Reg_EBP;
                    }
                    
                    MOV_ImmToReg(ExtraRegister, ConstMap[xRS->mips_reg].value);
					SUB_Reg1OfReg2(xRT->x86reg, ExtraRegister);
					SBB_Reg2FromReg1(ExtraRegister, ExtraRegister);
					AND_ImmToReg(ExtraRegister, 0x1, 0);
					MOV_Reg2ToReg1(xRD->x86reg, ExtraRegister);
					
                    if (ExtraRegister == Reg_EBP)
                        MOV_ImmToReg(Reg_EBP, HardwareStart);
				}
				return;
			}
		}



		else if (ConstMap[xRT->mips_reg].IsMapped)
		{
            {	//rs!=rt
				int k = ConstMap[xRT->mips_reg].value;
				xRD->IsDirty = 1;
//				if((xRD->mips_reg != xRS->mips_reg) && (xRD->mips_reg != xRT->mips_reg))
//				{
//					xRD->NoNeedToLoadTheLo = 1;
//					Flag = 1;
//				}
				
                //can't reorder these.
/*				MapRS;
				MapRD;

				if ( xRD->x86reg < 4) {
					if( Flag) {
						XOR_Reg2ToReg1(xRD->x86reg, xRD->x86reg);
						CMP_RegWithImm(xRS->x86reg, k);
						SETcc_Reg( CC_B, xRD->x86reg);
					} else {
						CMP_RegWithImm(xRS->x86reg, k);
						SETcc_Reg( CC_B, xRD->x86reg);
						AND_ImmToReg( 1, xRD->x86reg, 1);					
					}
					return;
				}
*/
				if (xRD->mips_reg != xRT->mips_reg)
				{
					//Branchless sltu function: z = (u32)x < (u32)y ? 1 : 0;
					//	MOV ECX, EAX
					//	SUB ECX, EBX ;eax < ebx ? CF : NC ; x - y
					//	SBB ECX, ECX ;eax < ebx ? 0xffffffff : 0
					//	AND ECX, 1
					
					//1964 1.0 new stuff!!
				  MapRT;
                  MapRegisterNew(FLUSH_TO_MEMORY, xRD, xRS, xRS->mips_reg, xRT->mips_reg);

    				SUB_Reg1OfReg2(xRT->x86reg, xRD->x86reg);
					SBB_Reg2FromReg1(xRD->x86reg, xRD->x86reg);
					AND_ImmToReg(xRD->x86reg, 0x1, 0);
				}
				else
				{
                    int ExtraRegister = 0; 
                    int Flag = 0;
                    
				if((xRD->mips_reg != xRS->mips_reg) && (xRD->mips_reg != xRT->mips_reg))
				{
					xRD->NoNeedToLoadTheLo = 1;
                    Flag = 1;
				}


                    MapRS;
					MapRD;					
					MapRT;

                    if (Flag  == 1)
                        ExtraRegister = xRD->x86reg;
                    else
                    {
                        ExtraRegister = FindFreeRegister();
                        if (ExtraRegister < Reg_EAX)
                            ExtraRegister = Reg_EBP;
                    }

                    MOV_Reg2ToReg1(ExtraRegister, xRS->x86reg);
					SUB_Reg1OfReg2(xRT->x86reg, ExtraRegister);
					SBB_Reg2FromReg1(ExtraRegister, ExtraRegister);
					AND_ImmToReg(ExtraRegister, 0x1, 0);
					MOV_Reg2ToReg1(xRD->x86reg, ExtraRegister);
					if (ExtraRegister == Reg_EBP)
                        MOV_ImmToReg(Reg_EBP, HardwareStart);
				}
				return;
			}
		}

		xRD->IsDirty = 1;
		if((xRD->mips_reg != xRS->mips_reg) && (xRD->mips_reg != xRT->mips_reg))
		{
			xRD->NoNeedToLoadTheLo = 1;
			Flag = 1;
		}

		MapRS;
        MapRT;		
        MapRD;
		if ((xRD->x86reg < 4) && (Flag))
			XOR_Reg2ToReg1(xRD->x86reg, xRD->x86reg);

		if (xRD->x86reg < 4)
		{
			CMP_Reg2WithReg1(xRS->x86reg, xRT->x86reg);
			SETcc_Reg(CC_B, xRD->x86reg);
			if (!Flag)
				AND_ImmToReg(xRD->x86reg, 0xff, 0);
			return;
		}

		//Branchless sltu function: z = (u32)x < (u32)y ? 1 : 0;
		//	MOV ECX, EAX
		//	SUB ECX, EBX ;eax < ebx ? CF : NC ; x - y
		//	SBB ECX, ECX ;eax < ebx ? 0xffffffff : 0
		//	AND ECX, 1
		if (xRT->mips_reg != xRD->mips_reg)
		{
			MOV_Reg2ToReg1(xRD->x86reg, xRS->x86reg);
			SUB_Reg1OfReg2(xRT->x86reg, xRD->x86reg);
			SBB_Reg2FromReg1(xRD->x86reg, xRD->x86reg);
			AND_ImmToReg(xRD->x86reg, 0x1, 0);
		}
		else //rd=rt.
		{
			CMP_Reg2WithReg1(xRS->x86reg, xRT->x86reg);
			Jcc_auto(CC_AE, 5);

			MOV_ImmToReg(xRD->x86reg, 1);
			JMP_Short_auto(6);

			SetTarget(5);

			XOR_Reg2ToReg1(xRD->x86reg, xRD->x86reg);

			SetTarget(6);
		}
	}

	/* 64bit */
	else
	{

		if((xRD->mips_reg != xRS->mips_reg) && (xRD->mips_reg != xRT->mips_reg))
		{
			xRD->Is32bit = 1;
			xRD->NoNeedToLoadTheLo = 1;
		}

		xRD->IsDirty = 1;
		MapRS;
		MapRT;
		MapRD;

		CMP_Reg2WithReg1(xRS->HiWordLoc, xRT->HiWordLoc);

		Jcc_auto(CC_A, 2);
		Jcc_auto(CC_B, 1);

		CMP_Reg2WithReg1(xRS->x86reg, xRT->x86reg);
		Jcc_auto(CC_AE, 3);

		SetTarget(1);
		MOV_ImmToReg(xRD->x86reg, 1);
		JMP_Short_auto(4);

		SetTarget(2);
		SetTarget(3);

		XOR_Reg2ToReg1(xRD->x86reg, xRD->x86reg);

		SetTarget(4);

		if(xRD->Is32bit == 0)
		{
			xRD->Is32bit = 1;
			MapRD;
		}
	}
}

int IsCMOVSupported()
{
	int temp;

    __asm { 
		mov eax, 1
		cpuid
		and edx, (1<<15)  //CMOV supported?
		mov temp, edx
	}
    return temp;
}
// _____________________________________________________________________________
//
void slti(OP_PARAMS)
{
	_int32	ConstInt = (_int32) __I;
	int		Is32bit = 0;

	OpcodePrologue(1, &r4300i_slti, 0, PASS_PARAMS);
	if(xRT->mips_reg == 0) return;


#ifdef SAFE_SLT
	INTERPRET(r4300i_slti);
	return;
#endif

	Is32bit = ( (currentromoptions.Assume_32bit == ASSUME_32BIT_YES)) || CheckIs32Bit(xRS->mips_reg) ;
	if (Is32bit)Set32bit();

	if(Is32bit) {
		xRT->Is32bit = 1;
		xRS->Is32bit = 1;
	}

	if(ConstMap[xRS->mips_reg].IsMapped) {
		if((_int32) ConstMap[xRS->mips_reg].value >= (_int32) ConstInt)
			MapConst(xRT, 0);
		else
			MapConst(xRT, 1);
	} else if(xRT->mips_reg == xRS->mips_reg) {

		xRT->IsDirty = 1;
		MapRT;

		if(!Is32bit) {
			CMP_RegWithImm(xRT->HiWordLoc, (uint32) ((_int32) ConstInt >> 31));
			Jcc_auto(CC_G, 2);
			Jcc_auto(CC_L, 1);
			CMP_RegWithImm(xRT->x86reg, (_u32) ConstInt);
			Jcc_auto(CC_AE, 3);
		} else {
			CMP_RegWithImm(xRT->x86reg, (_u32) ConstInt);
			Jcc_auto(CC_GE, 3);
		}

		if(!Is32bit)
			SetTarget(1);

		MOV_ImmToReg(xRT->x86reg, 1);

		JMP_Short_auto(4);

		if(!Is32bit)
			SetTarget(2);

		SetTarget(3);

		XOR_Reg2ToReg1(xRT->x86reg, xRT->x86reg);

		SetTarget(4);
		if(!Is32bit)
			XOR_Reg2ToReg1(xRT->HiWordLoc, xRT->HiWordLoc);
	} else { 	/* rt != rs */


		if(!Is32bit) {



			if (xRS->Is32bit)
				MessageBox(0, "Error", "", 0);

			xRT->Is32bit = 1;
			xRT->NoNeedToLoadTheLo = 1;
			xRT->IsDirty = 1;
			MapRT;

            
            MapRS;

			XOR_Reg2ToReg1(xRT->x86reg, xRT->x86reg);
			CMP_RegWithImm(xRS->HiWordLoc, (uint32) ((_int32) ConstInt >> 31));
			Jcc_auto(CC_G, 2);
			Jcc_auto(CC_L, 1);
			CMP_RegWithImm(xRS->x86reg, (_u32) ConstInt);
			Jcc_auto(CC_AE, 4);
			SetTarget(1);
			INC_Reg(1, xRT->x86reg);
			SetTarget(2);
			SetTarget(4);
		} 


			// rt != rs 
		if (!IsCMOVSupported()) //cmov supported?
		{

			xRT->Is32bit = 1;
			xRT->NoNeedToLoadTheLo = 1;
			xRT->IsDirty = 1;
			MapRT;
            
            
            MapRS;


			XOR_Reg2ToReg1(xRT->x86reg, xRT->x86reg);
			{
				CMP_RegWithImm(xRS->x86reg, (_u32) ConstInt);
				Jcc_auto(CC_GE, 4);
			}

			INC_Reg(1, xRT->x86reg);

			SetTarget(4);
		}
        else
        {
			int ExtraRegister;

			xRT->Is32bit = 1;
			xRT->NoNeedToLoadTheLo = 1;
			xRT->IsDirty = 1;
			MapRT;

            MapRS;


            ExtraRegister = FindFreeRegister();
            if (ExtraRegister < Reg_EAX)
                ExtraRegister = Reg_EBP;

			MOV_ImmToReg(ExtraRegister, 1);
            XOR_Reg2ToReg1(xRT->x86reg, xRT->x86reg);
    		CMP_RegWithImm(xRS->x86reg, (_u32) ConstInt);
            CMOVcc_Reg2ToReg1(CC_L, xRT->x86reg, ExtraRegister);

            if (ExtraRegister == Reg_EBP)
                MOV_ImmToReg(Reg_EBP, HardwareStart);        
        }
	}
}


// _____________________________________________________________________________
//
void sltiu(OP_PARAMS)
{
	_s64	ConstInt = (_s64) (_s32) __I;
	int		Is32bit = 0;

    OpcodePrologue(1, &r4300i_sltiu, 0, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;


#ifdef SAFE_SLT
	INTERPRET(r4300i_sltiu);
	return;
#endif

    Is32bit = (  (currentromoptions.Assume_32bit == ASSUME_32BIT_YES) || CheckIs32Bit(xRS->mips_reg) );
	if (Is32bit)
        Set32bit();

    if( ConstMap[xRS->mips_reg].IsMapped)
	{
		if( (uint32)ConstMap[xRS->mips_reg].value >= (uint32)ConstInt)
			MapConst( xRT, 0);
		else
			MapConst( xRT, 1);
	}
	else if( xRT->mips_reg == xRS->mips_reg)
	{
		xRT->IsDirty = 1;
		MapRT;

		if ( Is32bit) {
			//Branchless sltiu function: z = (u32)x < (u32)y ? 1 : 0;
			//	MOV ECX, EAX
			//	SUB ECX, EBX ;eax < ebx ? CF : NC ; x - y
			//	SBB ECX, ECX ;eax < ebx ? 0xffffffff : 0
			//	AND ECX, 1


			SUB_ImmFromReg(1, xRT->x86reg, (uint32)ConstInt, 1);
			SBB_Reg2FromReg1(xRT->x86reg, xRT->x86reg);
			AND_ImmToReg(xRT->x86reg, 0x1, 0);

			return;
		} else {
			CMP_RegWithImm(xRT->HiWordLoc, (_u32)(ConstInt >> 32));
			Jcc_auto( CC_A, 2);
			Jcc_auto( CC_B, 1);
		}

		CMP_RegWithImm(xRT->x86reg, (_u32)ConstInt);
		Jcc_auto( CC_AE, 3);

		if( !Is32bit)
			SetTarget( 1);

		MOV_ImmToReg(xRT->x86reg, 1);

		JMP_Short_auto( 4);

		if( !Is32bit)
			SetTarget( 2);

		SetTarget( 3);

		XOR_Reg2ToReg1(xRT->x86reg, xRT->x86reg);

		SetTarget( 4);
		if( !Is32bit)
			XOR_Reg2ToReg1(xRT->HiWordLoc, xRT->HiWordLoc);
	}
	else	/* rt != rs :) */
	{
		if (Is32bit)
		{
			// Branchless sltiu function: z = (u32)x < (u32)y ? 1 : 0;
			//	MOV ECX, EAX
			//	SUB ECX, EBX ;eax < ebx ? CF : NC ; x - y
			//	SBB ECX, ECX ;eax < ebx ? 0xffffffff : 0
			//	AND ECX, 1


       
        xRT->IsDirty = 1;
		xRT->Is32bit = 1;
        MapRegisterNew(FLUSH_TO_MEMORY, xRT, xRS, xRS->mips_reg, -1);            
            
		SUB_ImmFromReg(1, xRT->x86reg, (_s32)ConstInt, 1);
		SBB_Reg2FromReg1(xRT->x86reg, xRT->x86reg);
        AND_ImmToReg(xRT->x86reg, 0x1, 0);

		} else {
		MapRS;
        
        xRT->IsDirty = 1;
		xRT->Is32bit = 1;
		xRT->NoNeedToLoadTheLo = 1;
		MapRT;

            
            XOR_Reg2ToReg1(xRT->x86reg, xRT->x86reg);
			CMP_RegWithImm(xRS->HiWordLoc, (_u32)(ConstInt >> 32));
			Jcc_auto( CC_A, 2);
			Jcc_auto( CC_B, 1);
			CMP_RegWithImm(xRS->x86reg, (_u32) ConstInt);
			Jcc_auto( CC_AE, 4);
			SetTarget( 1);
			INC_Reg( 1, xRT->x86reg);
			SetTarget( 2);
			SetTarget( 4);
		}
	}
}

extern _int32	r4300i_lh_faster(uint32 QuerAddr);

// _____________________________________________________________________________
//
void lh(OP_PARAMS)
{
	_u32	QuerAddr;
	_u16	value;

    OpcodePrologue(1, &r4300i_lh, 1, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;

#ifdef SAFE_LOADSTORE
    INTERPRET(r4300i_lh);
    return;
#endif


	if(ConstMap[xRS->mips_reg].IsMapped == 1)
	{
        __try
		{
			QuerAddr = (_u32) ((((_s32) ConstMap[xRS->mips_reg].value + (_s32) (_s16) __I)));
			if(NOT_IN_KO_K1_SEG(QuerAddr)) goto _Default;

			value = LOAD_SHALF_PARAM(QuerAddr);
			if(xRT->mips_reg != 0)	/* mandatory */
			{
				if(ConstMap[xRT->mips_reg].IsMapped == 1)
				{
					ConstMap[xRT->mips_reg].IsMapped = 0;
				}

				xRT->IsDirty = 1;
				xRT->NoNeedToLoadTheLo = 1;
				MapRT;

				/* loads aligned, and loads 32bit. very cool. */
				switch(QuerAddr & 3)
				{
				case 0:
					MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_UWORD_PARAM(QuerAddr & 0xfffffffc));
					SAR_RegByImm(xRT->x86reg, 16);
					break;
				case 2:
					MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_UWORD_PARAM(QuerAddr & 0xfffffffc));
					SHL_RegByImm(xRT->x86reg, 16);
					SAR_RegByImm(xRT->x86reg, 16);
					break;
				default:
					MOVSX_Memory16ToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_SHALF_PARAM(QuerAddr));
				}
			}
			else
				goto _Default;
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

            xRT->IsDirty = 1;
			xRT->NoNeedToLoadTheLo = 1;
			MapRT;
			if(xRT->x86reg != Reg_EAX) PUSH_RegIfMapped(Reg_EAX, 1, Reg_ECX);
			if(xRT->x86reg != Reg_ECX) PUSH_RegIfMapped(Reg_ECX, 1, Reg_EAX);

			MOV_ImmToReg(Reg_EAX, (temp + (_s32) (_s16) __dotI) ^ 2);
			MOV_ImmToReg(Reg_ECX, ((uint32) (temp + (_s32) (_s16) __dotI)) >> SHIFTER2_READ);

			WC16(0x14FF);
			WC8(0x8D);
			WC32((uint32) & gHardwareState.memory_read_functions);
			LOGGING_DYNA(LogDyna("	CALL memory_read_functions[]\n"););
			if(xRT->mips_reg != 0) MOVSX_Memory16ToReg(xRT->x86reg, ModRM_EAX, 0);
			if(xRT->x86reg != Reg_ECX) POP_RegIfMapped(Reg_ECX);
			if(xRT->x86reg != Reg_EAX) POP_RegIfMapped(Reg_EAX);
		}
		else
		{
			int temp;
			int ExtraRegister;

            xRT->IsDirty = 1;
			if(xRT->mips_reg != xRS->mips_reg)
                xRT->NoNeedToLoadTheLo = 1;
			MapRT;

			/* we checked if rs is a const above already. good. */
			if
			(
				((temp = CheckWhereIsMipsReg(xRS->mips_reg)) == -1)
			&&	(ConstMap[xRS->mips_reg].FinalAddressUsedAt <= gHWS_pc)
			)
			{
				if(xRT->x86reg != Reg_EAX) PUSH_RegIfMapped(Reg_EAX, 1, Reg_EAX);

                FetchEBP_Params(xRS->mips_reg);
                //feasible for loads only

                if ((xRT->x86reg != Reg_EAX) && ( xRT->mips_reg != 0)) 
                    ExtraRegister = xRT->x86reg;
                else
                {
                    ExtraRegister = FindFreeRegister();
    		        if (ExtraRegister < Reg_ECX)
                        ExtraRegister = Reg_EBP;
                }

                MOV_MemoryToReg(ExtraRegister, x86params.ModRM, x86params.Address);

                
                //eax is NEVER equal to ExtraRegister, but this order is necessary.
                Lea(Reg_EAX, ExtraRegister, (_s32) (_s16) __dotI);
                ADD_ImmToReg(ExtraRegister, (_s32) (_s16) __dotI, 0);
			}
			else
			{
				
                //Conker bug.
                
                MapRS;

				if( xRT->x86reg != Reg_EAX)
					PUSH_RegIfMapped(Reg_EAX, 1, Reg_EAX);

                //feasible for loads only
                if ((xRT->x86reg != Reg_EAX) && ( xRT->mips_reg != 0)) 
                    ExtraRegister = xRT->x86reg;
                else
                {
                    ExtraRegister = FindFreeRegister();
    		        if (ExtraRegister < Reg_ECX)
                        ExtraRegister = Reg_EBP;
                }

                //ExtraRegister MAY BE equal to RS. This check is necessary.
                //ExtraRegister is never EAX.
                if (xRS->x86reg != Reg_EAX) 
                {
            	    Lea( Reg_EAX, xRS->x86reg, (_s32) (_s16) __dotI);
                    Lea( ExtraRegister, xRS->x86reg, (_s32) (_s16) __dotI);
                }
                else
                {
                    Lea( ExtraRegister, xRS->x86reg, (_s32) (_s16) __dotI);
            	    Lea( Reg_EAX, xRS->x86reg, (_s32) (_s16) __dotI);
                }
            }

			XOR_ImmToReg(Reg_EAX, 2, 0);
			SHR_RegByImm(ExtraRegister, SHIFTER2_READ);
			WC16(0x14FF);
			WC8(0x85|(ExtraRegister<<3));
			WC32((uint32) & gHardwareState.memory_read_functions);
			LOGGING_DYNA(LogDyna("	CALL memory_read_functions[]\n"););
			if( xRT->mips_reg != 0) 
				MOVSX_Memory16ToReg(xRT->x86reg, ModRM_EAX, 0);
            else
                XOR_Reg2ToReg1(xRT->x86reg, xRT->x86reg);

			if( ExtraRegister == Reg_EBP)
				MOV_ImmToReg(Reg_EBP, HardwareStart);
			if( xRT->x86reg != Reg_EAX)
				POP_RegIfMapped(Reg_EAX);
		}
	}
}

extern char * ((**phys_read_fast) (_u32 addr));
extern char * ((**phys_write_fast) (_u32 addr));

// _____________________________________________________________________________
//
void MemoryCase1(void)
{
	if((xRT->x86reg != Reg_EAX) && (xRT->HiWordLoc != Reg_EAX))
	{
		PUSH_RegIfMapped(Reg_EAX, 1, Reg_ECX);

		//lea rt, [rs+imm]
		Lea(Reg_EAX, xRS->x86reg, __dotI);
        Lea(xRT->x86reg, xRS->x86reg, __dotI);

		SHR_RegByImm(xRT->x86reg, SHIFTER2_READ);
		WC16(0x14FF);
		WC8((uint8) (0x85 | (xRT->x86reg << 3)));
		WC32((uint32) & gHardwareState.memory_read_functions);
	} else {
       
        if(xRS->x86reg != xRT->x86reg) PUSH_RegToStack(xRS->x86reg);

        Lea(Reg_EAX, xRS->x86reg, (_s32) (_s16)__dotI);
        ADD_ImmToReg(xRS->x86reg, (_s32) (_s16)__dotI, 0);
        SHR_RegByImm(xRS->x86reg, SHIFTER2_READ);
		WC16(0x14FF);
		WC8((uint8) (0x85 | (xRS->x86reg << 3)));
		WC32((uint32) & gHardwareState.memory_read_functions);
	}

	LOGGING_DYNA(LogDyna("	CALL memory_read_functions[]\n"););
}

// _____________________________________________________________________________
//
void MemoryCase2(void)
{
	if(xRT->x86reg != Reg_EAX)
	{
		/* we'd better have checked if rs is mapped above. */
		MapRS_To__WithLoadOnlyFunction(xRT, 1, MOV_MemoryToReg);
		
        PUSH_RegIfMapped(Reg_EAX, 1, Reg_EAX);
        Lea(Reg_EAX, xRT->x86reg, (_s32) (_s16) __dotI);
        ADD_ImmToReg(xRT->x86reg, (_s32) (_s16) __dotI, 0);
		SHR_RegByImm(xRT->x86reg, SHIFTER2_READ);
		WC16(0x14FF);
		WC8((uint8) (0x85 | (xRT->x86reg << 3)));
		WC32((uint32) & gHardwareState.memory_read_functions);
	}
	else
	{
		MapRS;
		if(xRS->x86reg != xRT->x86reg)
			PUSH_RegToStack(xRS->x86reg);

		Lea(Reg_EAX, xRS->x86reg, (_s32) (_s16) __dotI);
        ADD_ImmToReg(xRS->x86reg, (_s32) (_s16) __dotI, 0);
        SHR_RegByImm(xRS->x86reg, SHIFTER2_READ);
		WC16(0x14FF);
		WC8((uint8) (0x85 | (xRS->x86reg << 3)));
		WC32((uint32) & gHardwareState.memory_read_functions);
	}

	LOGGING_DYNA(LogDyna("	CALL memory_read_functions[]\n"););
}


void lw(OP_PARAMS)
{
	_s32	value;

    OpcodePrologue(1, &r4300i_lw, 1, PASS_PARAMS);

#ifdef SAFE_LOADSTORE
    INTERPRET(r4300i_lw);
    return;
#endif


	if(ConstMap[xRS->mips_reg].IsMapped == 1)
	{
		uint32	QuerAddr;

		QuerAddr = (_u32) ((_s32) ConstMap[xRS->mips_reg].value + (_s32) (_s16) __I);

		if(!(IN_K0_SEG( QuerAddr)))

        {        
            goto _Default;
        }

		__try {
			value = LOAD_SWORD_PARAM(QuerAddr);
		} __except(NULL, EXCEPTION_EXECUTE_HANDLER)	{
			// TRACE1("DYNA LW: Addr=%08X", QuerAddr);
			goto _Default;
		}

		if( xRT->mips_reg != 0) { 	// mandatory
			ConstMap[xRT->mips_reg].IsMapped = 0;
			xRT->IsDirty = 1;
			xRT->NoNeedToLoadTheLo = 1;
			MapRegisterNew(FLUSH_TO_MEMORY, xRT, xRT, 99, 99);
			MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_SWORD_PARAM(QuerAddr));
		}
	} else {
_Default:

        if(ConstMap[xRS->mips_reg].IsMapped) {
			int		temp = ConstMap[xRS->mips_reg].value;
			uint32	vAddr = (uint32) ((_int32) temp + (_s32) (_s16) __dotI);

            if (xRT->mips_reg != 0)
            {
                xRT->IsDirty = 1;
			    xRT->NoNeedToLoadTheLo = 1;
    			MapRT;
            }
            else
                xRT->x86reg = Reg_EDI; //a little trick. rt isn't used when rt is 0.

			if(xRT->x86reg != Reg_EAX)
				PUSH_RegIfMapped(Reg_EAX, 1, Reg_EAX);

			MOV_ImmToReg(Reg_EAX, vAddr);
			X86_CALL((uint32) gHardwareState.memory_read_functions[vAddr >> SHIFTER2_READ]);

			LOGGING_DYNA(LogDyna("	CALL memory_read_functions[]\n"););
			if(xRT->mips_reg != 0) MOV_ModRMToReg(xRT->x86reg, ModRM_EAX);
			if(xRT->x86reg != Reg_EAX) POP_RegIfMapped(Reg_EAX);
		} else {
            {            
            xRT->IsDirty = 1;
			if(xRT->mips_reg != xRS->mips_reg)
                xRT->NoNeedToLoadTheLo = 1;

           
            MapRegisterNew(FLUSH_TO_MEMORY, xRT, xRT, xRS->mips_reg, -1);

			if(xRT->x86reg != Reg_EAX) {

		/* we'd better have checked if rs is mapped above. */
			MapRS_To__WithLoadOnlyFunction(xRT, 1, MOV_MemoryToReg);

	        PUSH_RegIfMapped(Reg_EAX, 1, Reg_EAX);
		    Lea(Reg_EAX, xRT->x86reg, (_s32) (_s16) __dotI);
			ADD_ImmToReg(xRT->x86reg, (_s32) (_s16) __dotI, 0);
			SHR_RegByImm(xRT->x86reg, SHIFTER2_READ);
			WC16(0x14FF);
			WC8((uint8) (0x85 | (xRT->x86reg << 3)));
			WC32((uint32) & gHardwareState.memory_read_functions);

				if(xRT->mips_reg != 0) 
					MOV_ModRMToReg(xRT->x86reg, ModRM_EAX);
                else
                    XOR_Reg2ToReg1(xRT->x86reg, xRT->x86reg);

				POP_RegIfMapped( Reg_EAX);
			} else {
				int ExtraRegister;

                
                // we checked if rs is a const above already. good.
				if( (CheckWhereIsMipsReg(xRS->mips_reg) == -1)
				&&	(ConstMap[xRS->mips_reg].FinalAddressUsedAt <= gHWS_pc)) {

					if( xRT->x86reg != Reg_EAX)
						PUSH_RegIfMapped(Reg_EAX, 1, Reg_EAX);

                    
                    //feasible for loads only
                if ((xRT->x86reg != Reg_EAX) && ( xRT->mips_reg != 0)) 
                    ExtraRegister = xRT->x86reg;
                else
                {
                    ExtraRegister = FindFreeRegister();
    		        if (ExtraRegister < Reg_ECX)
                        ExtraRegister = Reg_EBP;
                }

					FetchEBP_Params( xRS->mips_reg);
					
                    MOV_MemoryToReg( ExtraRegister, x86params.ModRM, x86params.Address);
					
                    //eax is NEVER equal to ExtraRegister. No check necessary.
                    Lea( Reg_EAX, ExtraRegister, __dotI);
                    ADD_ImmToReg(ExtraRegister, __dotI, 0);
				} else {
					MapRS;

					if( xRT->x86reg != Reg_EAX)
						PUSH_RegIfMapped(Reg_EAX, 1, Reg_EAX);

                    
                    //feasible for loads only.
                //To improve pairing for this op, addded check for (xRT->mips_reg) != (xRS->mips_reg)
                if ((xRT->x86reg != Reg_EAX) && ( xRT->mips_reg != 0) && (xRT->mips_reg) != (xRS->mips_reg)) 
                    ExtraRegister = xRT->x86reg;
                else
                {
                    ExtraRegister = FindFreeRegister();
    		        if (ExtraRegister < Reg_ECX)
                    {
            			if ((ConstMap[xRS->mips_reg].FinalAddressUsedAt <= gHWS_pc) && !x86reg[xRS->x86reg].IsDirty && (xRS->x86reg != Reg_EAX))
                        {
                            ExtraRegister = xRS->x86reg;
                        	Lea( Reg_EAX, xRS->x86reg, (_s32) (_s16) __dotI);
                            Lea( ExtraRegister, xRS->x86reg, (_s32) (_s16) __dotI);
                            goto _next;

                        }
                        else
                        ExtraRegister = Reg_EBP;
                }

                }

					
                    //From the check above for (xRT->mips_reg) != (xRS->mips_reg),
                    //ExtraRegister is NEVER equal to RS. This makes the order of the next 2 LEA's 
                    //arbitrary. The order is reversed for better pairing with the following SHR op.
                    Lea( ExtraRegister, xRS->x86reg, (_s32) (_s16) __dotI);
                	Lea( Reg_EAX, xRS->x86reg, (_s32) (_s16) __dotI);
				}
_next:

				SHR_RegByImm(ExtraRegister, SHIFTER2_READ);
				WC16( 0x14FF);
				WC8( 0x85 | (ExtraRegister << 3));
				WC32( (uint32)&gHardwareState.memory_read_functions);

				LOGGING_DYNA(LogDyna("	CALL memory_read_functions[]\n"););
				if( __RT != 0)
					MOV_ModRMToReg(xRT->x86reg, ModRM_EAX);
                else
                    XOR_Reg2ToReg1(xRT->x86reg, xRT->x86reg);

				if( ExtraRegister == Reg_EBP)
					MOV_ImmToReg(Reg_EBP, HardwareStart);
				if( xRT->x86reg != Reg_EAX)
					POP_RegIfMapped( Reg_EAX);
			}
            }
		}
	}
}

// _____________________________________________________________________________
//
void sw(OP_PARAMS)
{
	_u32	vAddr;
    int		temp;
	int		temp2;
	int		rt = __dotRT;
	int		rs = __dotRS;

    OpcodePrologue(1, &r4300i_sw, 1, PASS_PARAMS);

#ifdef SAFE_LOADSTORE
	INTERPRET(r4300i_sw); return;
#endif


	if(ConstMap[rs].IsMapped)
	{
       
        temp = ConstMap[rs].value;
		vAddr = (uint32) ((_int32) temp + (_s32) (_s16) __dotI);

		if(ConstMap[rt].IsMapped)
		    FlushOneConstantLo( rt);
		if((temp2 = CheckWhereIsMipsReg(rt)) > -1)
		    FlushRegister(FLUSH_TO_MEMORY, temp2);

		PUSH_RegIfMapped(Reg_EAX, 1, Reg_ECX);
		PUSH_RegIfMapped(Reg_ECX, 1, Reg_EAX);
		if(ConstMap[rt].IsMapped) FlushOneConstantHi(FLUSH_TO_MEMORY, rt);

		MOV_ImmToMemory(1, ModRM_disp32, (_u32) & r.r_.write_mem_rt, rt);
		MOV_ImmToReg(Reg_EAX, vAddr);
		X86_CALL((uint32) gHardwareState.memory_write_fun_eax_only[vAddr >> SHIFTER2_WRITE]);
		LOGGING_DYNA(LogDyna("	CALL gHardwareState.memory_write_fun_eax_only[]\n"););

		if(xRT->mips_reg != 0)
        {
			LoadLowMipsCpuRegister(xRT->mips_reg, Reg_ECX);
		MOV_RegToModRM(1, Reg_ECX, ModRM_EAX);
        }
		else
        {
            MOV_ImmToMemory(1, ModRM_EAX, 0, 0);
        }


		POP_RegIfMapped(Reg_ECX);
		POP_RegIfMapped(Reg_EAX);
	}
	else
	{
        int NoPushECX=0;

    	if(ConstMap[rt].IsMapped) FlushOneConstantLo(rt);
		if((temp = CheckWhereIsMipsReg(rt)) > -1)
            FlushRegister(FLUSH_TO_MEMORY, temp);



		if(ConstMap[rt].IsMapped) FlushOneConstantHi(FLUSH_TO_MEMORY, rt);
		if((temp = CheckWhereIsMipsReg(rs)) > -1)
        {
			if ((ConstMap[rs].FinalAddressUsedAt <= gHWS_pc) && !x86reg[temp].IsDirty && (temp != Reg_EAX))
            {
		        PUSH_RegIfMapped(Reg_EAX, 1, Reg_EAX);
                NoPushECX=1;
            
            }
            else
            {
        		PUSH_RegIfMapped(Reg_EAX, 1, Reg_ECX);
                PUSH_RegIfMapped(Reg_ECX, 1, Reg_EAX);
			MOV_Reg2ToReg1(Reg_ECX, (unsigned char) temp);
                temp = Reg_ECX;
            }

        }
		else
        {
       		PUSH_RegIfMapped(Reg_EAX, 1, Reg_ECX);
            PUSH_RegIfMapped(Reg_ECX, 1, Reg_EAX);
			LoadLowMipsCpuRegister(rs, Reg_ECX);
            temp = Reg_ECX;
        }
		MOV_ImmToMemory(1, ModRM_disp32, (_u32) & r.r_.write_mem_rt, rt);

        Lea(Reg_EAX, temp, (_s32) (_s16) __dotI);
        ADD_ImmToReg(temp, (_s32) (_s16) __dotI, 0);
		SHR_RegByImm(temp, SHIFTER2_WRITE);
		WC16(0x14FF);
		WC8(0x85|(temp<<3));
		WC32((uint32) & gHardwareState.memory_write_fun_eax_only);
		LOGGING_DYNA(LogDyna("	CALL gHardwareState.memory_write_fun_eax_only[]\n"););

		if(xRT->mips_reg != 0)
        {
			LoadLowMipsCpuRegister(xRT->mips_reg, temp);
            MOV_RegToModRM(1, temp, ModRM_EAX);
        }
		else
        {
            MOV_ImmToMemory(1, ModRM_EAX, 0, 0);
        }

		if (!NoPushECX)
		POP_RegIfMapped(Reg_ECX);
		POP_RegIfMapped(Reg_EAX);
	}
}

// _____________________________________________________________________________
//
void ld(OP_PARAMS)
{
    int ExtraRegister;
    
    OpcodePrologue(1, &r4300i_ld, 0, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;

#ifdef SAFE_LOADSTORE
    INTERPRET(r4300i_ld);
    return;
#endif


	xRT->IsDirty = 1;
	if(xRT->mips_reg != xRS->mips_reg)
	{
		xRT->NoNeedToLoadTheLo = 1;
		xRT->NoNeedToLoadTheHi = 1;
	}

	MapRS;
    MapRT;

	if( (xRT->x86reg != Reg_EAX) &&
		(xRT->HiWordLoc != Reg_EAX)) 
			PUSH_RegIfMapped(Reg_EAX, 1, Reg_EAX);

	ExtraRegister = FindFreeRegister();
	if (ExtraRegister < Reg_ECX)
	    ExtraRegister = Reg_EBP;

    Lea(ExtraRegister, xRS->x86reg, __dotI);
	Lea(Reg_EAX, xRS->x86reg, __dotI);
	SHR_RegByImm(ExtraRegister, SHIFTER2_READ);
	WC16(0x14FF);
	WC8(0x85|(ExtraRegister<<3));
	WC32((uint32) & gHardwareState.memory_read_functions);
	LOGGING_DYNA(LogDyna("	CALL memory_read_functions[]\n"););

	if(xRT->x86reg == Reg_EAX)
	{
		MOV_MemoryToReg(xRT->HiWordLoc, ModRM_EAX, 0);
		MOV_MemoryToReg(xRT->x86reg, ModRM_disp8_EAX, 4);
	}
	else
	{
		MOV_MemoryToReg(xRT->x86reg, ModRM_disp8_EAX, 4);
		MOV_MemoryToReg(xRT->HiWordLoc, ModRM_EAX, 0);
	}

	if (ExtraRegister == Reg_EBP)
        MOV_ImmToReg(Reg_EBP, HardwareStart);
	if((xRT->x86reg != Reg_EAX) && (xRT->HiWordLoc != Reg_EAX))
        POP_RegIfMapped(Reg_EAX);


    //Core anomaly: Need to flush RT as 64bit 
    if (currentromoptions.Assume_32bit == ASSUME_32BIT_YES)
    {
        currentromoptions.Assume_32bit = ASSUME_32BIT_NO;
        FlushRegister(FLUSH_TO_MEMORY, xRT->x86reg);
        currentromoptions.Assume_32bit = ASSUME_32BIT_YES;
    }
}

extern void r4300i_sd_faster(uint32 QuerAddr, uint32 rt_ft);

// _____________________________________________________________________________
//
void sd(OP_PARAMS)
{
	int		temp;
	uint32	vaddr;

    OpcodePrologue(1, &r4300i_sd, 0, PASS_PARAMS);


#ifdef SAFE_LOADSTORE
	INTERPRET_LOADSTORE(r4300i_sd);
	return;
#endif
	
	if(ConstMap[xRS->mips_reg].IsMapped)
	{
      
        vaddr = (uint32) ((_int32) ConstMap[xRS->mips_reg].value + (_s32) (_s16) __I);
		if(ConstMap[xRT->mips_reg].IsMapped)
            FlushOneConstant(FLUSH_TO_MEMORY, xRT->mips_reg);

		if((temp = CheckWhereIsMipsReg(xRT->mips_reg)) > -1)
            FlushRegister(FLUSH_TO_MEMORY, temp);
		PUSH_RegIfMapped(Reg_EAX, 1, Reg_ECX);
		PUSH_RegIfMapped(Reg_ECX, 1, Reg_EAX);

		MOV_ImmToReg(Reg_ECX, vaddr);
		MOV_Reg2ToReg1(Reg_EAX, Reg_ECX);
		SHR_RegByImm(Reg_ECX, SHIFTER2_WRITE);

		WC16(0x14FF);
		WC8(0x85|(Reg_ECX<<3));
		WC32((uint32) & gHardwareState.memory_write_fun_eax_only);
		LOGGING_DYNA(LogDyna("	CALL gHardwareState.memory_write_fun_eax_only[]\n"););

		LoadLowMipsCpuRegister(xRT->mips_reg, Reg_ECX);
		MOV_RegToMemory(1, Reg_ECX, ModRM_disp8_EAX, 4);

		LoadHighMipsCpuRegister(xRT->mips_reg, Reg_ECX);
		MOV_RegToMemory(1, Reg_ECX, ModRM_EAX, 0);
		POP_RegIfMapped(Reg_ECX);
		POP_RegIfMapped(Reg_EAX);
	}
	else
	{
		//Flush the Lo RT constant
        if(ConstMap[xRT->mips_reg].IsMapped)
            FlushOneConstantLo(xRT->mips_reg);

		if((temp = CheckWhereIsMipsReg(xRT->mips_reg)) > -1)
            FlushRegister(FLUSH_TO_MEMORY, temp);
		PUSH_RegIfMapped(Reg_EAX, 1, Reg_ECX);
		PUSH_RegIfMapped(Reg_ECX, 1, Reg_EAX);

		//Flush the hi RT constant
        if(ConstMap[xRT->mips_reg].IsMapped)
            FlushOneConstantHi(FLUSH_TO_MEMORY, xRT->mips_reg);

        //Load the RS into ECX (for parameter)
        if((temp = CheckWhereIsMipsReg(xRS->mips_reg)) > -1)
			MOV_Reg2ToReg1(Reg_ECX, (unsigned char) temp);
		else
			LoadLowMipsCpuRegister(xRS->mips_reg, Reg_ECX);

		Lea(Reg_EAX, Reg_ECX, (_s32) (_s16) __dotI);
        ADD_ImmToReg(Reg_ECX, (_s32) (_s16) __dotI, 0);
		SHR_RegByImm(Reg_ECX, SHIFTER2_WRITE);

		WC16(0x14FF);
		WC8(0x85|(Reg_ECX<<3));
		WC32((uint32) & gHardwareState.memory_write_fun_eax_only);
		LOGGING_DYNA(LogDyna("	CALL gHardwareState.memory_write_fun_eax_only[]\n"););

		LoadLowMipsCpuRegister(xRT->mips_reg, Reg_ECX);
		MOV_RegToMemory(1, Reg_ECX, ModRM_disp8_EAX, 4);

		LoadHighMipsCpuRegister(xRT->mips_reg, Reg_ECX);
		MOV_RegToMemory(1, Reg_ECX, ModRM_EAX, 0);
		POP_RegIfMapped(Reg_ECX);
		POP_RegIfMapped(Reg_EAX);
	}
}

extern _int32	r4300i_lb_faster(uint32 QuerAddr);

// _____________________________________________________________________________
//
void lb(OP_PARAMS)
{
	_s8 value;

    OpcodePrologue(1, &r4300i_lb, 1, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;

#ifdef SAFE_LOADSTORE
    INTERPRET(r4300i_lb);
    return;
#endif


	if( ConstMap[xRS->mips_reg].IsMapped == 1)
	{
		__try
		{
			_u32	QuerAddr;

			/* TLB range */
			QuerAddr = (_u32) (((_s32) ConstMap[xRS->mips_reg].value + (_s32) (_s16) __I));

            if(NOT_IN_KO_K1_SEG(QuerAddr)) goto _Default;

			value = LOAD_UWORD_PARAM(QuerAddr);
			if(xRT->mips_reg != 0)	/* mandatory */
			{
				if(ConstMap[xRT->mips_reg].IsMapped == 1)
				{
					ConstMap[xRT->mips_reg].IsMapped = 0;
				}

				xRT->IsDirty = 1;
				xRT->NoNeedToLoadTheLo = 1;
				MapRT;

				/* loads aligned, and loads 32bit. very cool. */
				switch(QuerAddr & 3)
				{
				case 0:
					MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_UWORD_PARAM(QuerAddr & 0xfffffffc));
					SAR_RegByImm(xRT->x86reg, 24);
					break;
				case 1:
					MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_UWORD_PARAM(QuerAddr & 0xfffffffc));
					SHL_RegByImm(xRT->x86reg, 8);
					SAR_RegByImm(xRT->x86reg, 24);
					break;
				case 2:
					MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_UWORD_PARAM(QuerAddr & 0xfffffffc));
					SHL_RegByImm(xRT->x86reg, 16);
					SAR_RegByImm(xRT->x86reg, 24);
					break;
				case 3:
					MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_UWORD_PARAM(QuerAddr & 0xfffffffc));
					SHL_RegByImm(xRT->x86reg, 24);
					SAR_RegByImm(xRT->x86reg, 24);
					break;
				default:
					MOVSX_Memory8ToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_SBYTE_PARAM(QuerAddr));
				}
			}
			else
				goto _Default;
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

			
            xRT->IsDirty = 1;
			xRT->NoNeedToLoadTheLo = 1;
			MapRT;
			if(xRT->x86reg != Reg_EAX) PUSH_RegIfMapped(Reg_EAX, 0, -1);
			if(xRT->x86reg != Reg_ECX) PUSH_RegIfMapped(Reg_ECX, 0, -1);

			MOV_ImmToReg(Reg_EAX, (temp + (_s32) (_s16) __dotI) ^ 3);
			MOV_ImmToReg(Reg_ECX, ((uint32) (temp + (_s32) (_s16) __dotI)) >> SHIFTER2_READ);

			WC16(0x14FF);
			WC8(0x8D);
			WC32((uint32) & gHardwareState.memory_read_functions);
			LOGGING_DYNA(LogDyna("	CALL memory_read_functions[]\n"););
			if(__RT != 0) MOVSX_Memory8ToReg(xRT->x86reg, ModRM_EAX, 0);
			if(xRT->x86reg != Reg_ECX) POP_RegIfMapped(Reg_ECX);
			if(xRT->x86reg != Reg_EAX) POP_RegIfMapped(Reg_EAX);
		}
		else
		{
			int ExtraRegister = 0;

            xRT->IsDirty = 1;
			if(xRT->mips_reg != xRS->mips_reg) xRT->NoNeedToLoadTheLo = 1;
			MapRS;
			MapRT;

			if(xRS->x86reg != Reg_EAX)
			{
            	if((xRT->x86reg != Reg_EAX) && (xRT->HiWordLoc != Reg_EAX))
            	{
            		PUSH_RegIfMapped(Reg_EAX, 1, Reg_EAX);

            		//lea rt, [rs+imm]
            		Lea(Reg_EAX, xRS->x86reg, __dotI);
                    Lea(xRT->x86reg, xRS->x86reg, __dotI);

            		SHR_RegByImm(xRT->x86reg, SHIFTER2_READ);
            		WC16(0x14FF);
            		WC8((uint8) (0x85 | (xRT->x86reg << 3)));
            		WC32((uint32) & gHardwareState.memory_read_functions);
            	} else {
                //feasible for loads only
                if ((xRT->x86reg != Reg_EAX) && ( xRT->mips_reg != 0) && (xRT->mips_reg != xRS->mips_reg)) 
                    ExtraRegister = xRT->x86reg;
                else
                {
                    ExtraRegister = FindFreeRegister();
    		        if (ExtraRegister < Reg_ECX)
                        ExtraRegister = Reg_EBP;
                }

                    //From the check above for (xRT->mips_reg) != (xRS->mips_reg),
                    //ExtraRegister is NEVER equal to RS. This makes the order of the next 2 LEA's 
                    //arbitrary. The order is reversed for better pairing with the following SHR op.
                    Lea( ExtraRegister, xRS->x86reg, (_s32) (_s16) __dotI);
                	Lea( Reg_EAX, xRS->x86reg, (_s32) (_s16) __dotI);

                    SHR_RegByImm(ExtraRegister, SHIFTER2_READ);
            		WC16(0x14FF);
            		WC8((uint8) (0x85 | (ExtraRegister << 3)));
            		WC32((uint32) & gHardwareState.memory_read_functions);
            	}

            	LOGGING_DYNA(LogDyna("	CALL memory_read_functions[]\n"););

				if(__RT != 0)
				{
					XOR_ImmToReg(Reg_EAX, 3, 0);
					MOVSX_Memory8ToReg(xRT->x86reg, ModRM_EAX, 0);
				}
				else
					XOR_Reg2ToReg1(xRT->x86reg, xRT->x86reg);

				if(xRT->x86reg != Reg_EAX)
					POP_RegIfMapped(Reg_EAX);
                if (ExtraRegister == Reg_EBP)
                    MOV_ImmToReg(Reg_EBP, HardwareStart);
			}
			else
			{
				int ExtraRegister;

                /* MapRS; */
				if(xRT->x86reg != Reg_EAX) PUSH_RegIfMapped(Reg_EAX, 1, Reg_EAX);

                //feasible for loads only
                if ((xRT->x86reg != Reg_EAX) && ( xRT->mips_reg != 0)) 
                    ExtraRegister = xRT->x86reg;
                else
                {
                    ExtraRegister = FindFreeRegister();
    		        if (ExtraRegister < Reg_ECX)
                        ExtraRegister = Reg_EBP;
                }

                //ExtraRegister MAY BE equal to RS. This check is necessary.
                //ExtraRegister is never EAX.
                if (xRS->x86reg != Reg_EAX) 
                {
            	    Lea( Reg_EAX, xRS->x86reg, (_s32) (_s16) __dotI);
                    Lea( ExtraRegister, xRS->x86reg, (_s32) (_s16) __dotI);
                }
                else
                {
                    Lea( ExtraRegister, xRS->x86reg, (_s32) (_s16) __dotI);
            	    Lea( Reg_EAX, xRS->x86reg, (_s32) (_s16) __dotI);
                }

                SHR_RegByImm(ExtraRegister, SHIFTER2_READ);

				WC16(0x14FF);
				WC8(0x85 | (ExtraRegister<<3));
				WC32((uint32) & gHardwareState.memory_read_functions);
				LOGGING_DYNA(LogDyna("	CALL memory_read_functions[]\n"););
				if(__RT != 0)
				{
					XOR_ImmToReg(Reg_EAX, 3, 0);
					MOVSX_Memory8ToReg(xRT->x86reg, ModRM_EAX, 0);
				}
				else if(xRT->x86reg == Reg_EAX)
					XOR_Reg2ToReg1(xRT->x86reg, xRT->x86reg);

				if (ExtraRegister == Reg_EBP) MOV_ImmToReg(Reg_EBP, HardwareStart);
				if(xRT->x86reg != Reg_EAX) POP_RegIfMapped(Reg_EAX);
			}
		}
	}
}

// _____________________________________________________________________________
//
void lbu(OP_PARAMS)
{
	_u32	QuerAddr;
	_u8		value;

    OpcodePrologue(1, &r4300i_lbu, 1, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;

#ifdef SAFE_LOADSTORE
    INTERPRET(r4300i_lbu);
    return;
#endif



	if(ConstMap[xRS->mips_reg].IsMapped == 1)
	{
		__try
		{
			QuerAddr = (_u32) ((((_s32) ConstMap[xRS->mips_reg].value + (_s32) (_s16) __I)));
			if(NOT_IN_KO_K1_SEG(QuerAddr)) goto _Default;

			value = LOAD_UBYTE_PARAM(QuerAddr);
			if(xRT->mips_reg != 0)	// mandatory
			{
				if(ConstMap[xRT->mips_reg].IsMapped == 1)
				{
					ConstMap[xRT->mips_reg].IsMapped = 0;
				}

				xRT->IsDirty = 1;
				xRT->NoNeedToLoadTheLo = 1;
				MapRT;

				// loads aligned, and loads 32bit. very cool.
				switch(QuerAddr & 3)
				{
				case 0:
					MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_UWORD_PARAM(QuerAddr & 0xfffffffc));
					ROL_RegByImm(xRT->x86reg, 8);
					AND_ImmToReg(xRT->x86reg, 0x000000ff, 0);
					break;
				case 1:
					MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_UWORD_PARAM(QuerAddr & 0xfffffffc));
					SHR_RegByImm(xRT->x86reg, 16);
					AND_ImmToReg(xRT->x86reg, 0x000000ff, 0);
					break;
				case 2:
					MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_UWORD_PARAM(QuerAddr & 0xfffffffc));
					//WC8(0x66);
					SHR_RegByImm(xRT->x86reg, 8);
					AND_ImmToReg(xRT->x86reg, 0x000000ff, 0);
					break;
				case 3:
					MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_UWORD_PARAM(QuerAddr & 0xfffffffc));
					AND_ImmToReg(xRT->x86reg, 0x000000ff, 0);
					break;

				default:
					MOVZX_Memory8ToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_SBYTE_PARAM(QuerAddr));
				}
			}
			else
				goto _Default;
		}

		__except(NULL, EXCEPTION_EXECUTE_HANDLER)
		{
			goto _Default;
		}
	}
	else
	{
_Default:
		if(ConstMap[xRS->mips_reg].IsMapped) {
			int temp = ConstMap[xRS->mips_reg].value;

			xRT->IsDirty = 1;
			xRT->NoNeedToLoadTheLo = 1;
			MapRT;

			if( xRT->x86reg != Reg_EAX) PUSH_RegIfMapped( Reg_EAX, 0, -1);
			if( xRT->x86reg != Reg_ECX) PUSH_RegIfMapped( Reg_ECX, 0, -1);

			MOV_ImmToReg(Reg_EAX, (temp + (_s32) (_s16) __dotI) ^ 3);
			MOV_ImmToReg(Reg_ECX, ((uint32) (temp + (_s32) (_s16) __dotI)) >> SHIFTER2_READ);

			WC16( 0x14FF);
			WC8( 0x8D);
			WC32( (uint32)&gHardwareState.memory_read_functions);
			LOGGING_DYNA(LogDyna("	CALL memory_read_functions[]\n"););
			if( __RT != 0) MOVZX_Memory8ToReg(xRT->x86reg, ModRM_EAX, 0);
			if( xRT->x86reg != Reg_ECX) POP_RegIfMapped( Reg_ECX);
			if( xRT->x86reg != Reg_EAX) POP_RegIfMapped( Reg_EAX);
		} else {

            xRT->IsDirty = 1;
			if( xRT->mips_reg != xRS->mips_reg) 
                xRT->NoNeedToLoadTheLo = 1;

			MapRS;
			MapRT;

			if( xRS->x86reg != Reg_EAX) {
				MemoryCase1();
				if( __RT != 0) {
{
						XOR_ImmToReg(Reg_EAX, 3, 0);
						MOVZX_Memory8ToReg(xRT->x86reg, ModRM_EAX, 0);
					}
				} else if ( xRT->x86reg == Reg_EAX)
					XOR_Reg2ToReg1(xRT->x86reg, xRT->x86reg);

				if( xRT->x86reg != Reg_EAX)
					POP_RegIfMapped( Reg_EAX);
				else if( xRS->x86reg != xRT->x86reg)
					POP_RegFromStack(xRS->x86reg);
			} else {
				int ExtraRegister;

				if(xRT->x86reg != Reg_EAX) 
                    PUSH_RegIfMapped(Reg_EAX, 1, Reg_EAX);

                //feasible for loads only
                if ((xRT->x86reg != Reg_EAX) && ( xRT->mips_reg != 0) && (xRT->mips_reg != xRS->mips_reg)) 
                    ExtraRegister = xRT->x86reg;
                else
                {
                    ExtraRegister = FindFreeRegister();
    		        if (ExtraRegister < Reg_ECX)
                        ExtraRegister = Reg_EBP;
                }

                //From the check above for (xRT->mips_reg) != (xRS->mips_reg),
                //ExtraRegister is NEVER equal to RS. This makes the order of the next 2 LEA's 
                //arbitrary. The order is reversed for better pairing with the following SHR op.
                Lea( ExtraRegister, xRS->x86reg, (_s32) (_s16) __dotI);
            	Lea( Reg_EAX, xRS->x86reg, (_s32) (_s16) __dotI);

				SHR_RegByImm(ExtraRegister, SHIFTER2_READ);
				WC16(0x14FF);
				WC8(0x85|(ExtraRegister<<3));
				WC32((uint32) & gHardwareState.memory_read_functions);
				LOGGING_DYNA(LogDyna("	CALL memory_read_functions[]\n"););

				if( __RT != 0) 
                {
						XOR_ImmToReg(Reg_EAX, 3, 0);
						MOVZX_Memory8ToReg(xRT->x86reg, ModRM_EAX, 0);
				}

				if (ExtraRegister==Reg_EBP) MOV_ImmToReg(Reg_EBP, HardwareStart);
				if( xRT->x86reg != Reg_EAX)
					POP_RegIfMapped( Reg_EAX);
			}
		}
	}
}

// _____________________________________________________________________________
//
void lhu(OP_PARAMS)
{
	_u32	QuerAddr;
	_u16	value;

    OpcodePrologue(1, &r4300i_lhu, 1, PASS_PARAMS);
    if(xRT->mips_reg == 0) return;


#ifdef SAFE_LOADSTORE
    INTERPRET(r4300i_lhu);
    return;
#endif


	if(ConstMap[xRS->mips_reg].IsMapped == 1)
	{
		__try
		{
			QuerAddr = (_u32) ((((_s32) ConstMap[xRS->mips_reg].value + (_s32) (_s16) __I)));
			if(NOT_IN_KO_K1_SEG(QuerAddr)) goto _Default;

			value = LOAD_UHALF_PARAM(QuerAddr);
			if(xRT->mips_reg != 0)	/* mandatory */
			{
				if(ConstMap[xRT->mips_reg].IsMapped == 1)
				{
					ConstMap[xRT->mips_reg].IsMapped = 0;
				}

				xRT->IsDirty = 1;
				xRT->NoNeedToLoadTheLo = 1;
				MapRT;

				/* loads aligned, and loads 32bit. very cool. */
				switch(QuerAddr & 3)
				{
				case 0:
					MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_UWORD_PARAM(QuerAddr & 0xfffffffc));
					SHR_RegByImm(xRT->x86reg, 16);
					break;
				case 2:
					MOV_MemoryToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_UWORD_PARAM(QuerAddr & 0xfffffffc));
					AND_ImmToReg(xRT->x86reg, 0x0000ffff, 0);
					break;
				default:
					/* hmm..this is never touched, but looks wrong. */
					MOVZX_Memory16ToReg(xRT->x86reg, ModRM_disp32, (_u32) pLOAD_UHALF_PARAM(QuerAddr));
					break;
				}
			}
			else
				goto _Default;
		}

		__except(NULL, EXCEPTION_EXECUTE_HANDLER)
		{
			goto _Default;
		}
	}
	else
	{
_Default:
		{
			if(ConstMap[xRS->mips_reg].IsMapped)
			{
				int temp = (ConstMap[xRS->mips_reg].value+(_s32)(_s16)__dotI)^2;

			
                xRT->IsDirty = 1;
				xRT->NoNeedToLoadTheLo = 1;
				MapRT;
				if(xRT->x86reg != Reg_EAX) PUSH_RegIfMapped(Reg_EAX, 1, Reg_ECX);
				if(xRT->x86reg != Reg_ECX) PUSH_RegIfMapped(Reg_ECX, 1, Reg_EAX);

				MOV_ImmToReg(Reg_EAX, temp);
				MOV_ImmToReg(Reg_ECX, ((uint32)(temp)) >> SHIFTER2_READ);
				WC16(0x14FF);
				WC8(0x8D);
				WC32((uint32) & gHardwareState.memory_read_functions);
				LOGGING_DYNA(LogDyna("	CALL memory_read_functions[]\n"););
				if(__RT != 0) MOVZX_Memory16ToReg(xRT->x86reg, ModRM_EAX, 0);
				if(xRT->x86reg != Reg_ECX) POP_RegIfMapped(Reg_ECX);
				if(xRT->x86reg != Reg_EAX) POP_RegIfMapped(Reg_EAX);
			}
			else
			{
		{
			int temp;
			int ExtraRegister;

			xRT->IsDirty = 1;
			if(xRT->mips_reg != xRS->mips_reg) xRT->NoNeedToLoadTheLo = 1;
			MapRT;

			/* we checked if rs is a const above already. good. */
			if
			(
				((temp = CheckWhereIsMipsReg(xRS->mips_reg)) == -1)
			&&	(ConstMap[xRS->mips_reg].FinalAddressUsedAt <= gHWS_pc)
			)
			{
				FetchEBP_Params(xRS->mips_reg);
                //feasible for loads only
                if ((xRT->x86reg != Reg_EAX) && ( xRT->mips_reg != 0)) 
                    ExtraRegister = xRT->x86reg;
                else
                {
                    ExtraRegister = FindFreeRegister();
    		        if (ExtraRegister < Reg_ECX)
                        ExtraRegister = Reg_EBP;
                }

                MOV_MemoryToReg(ExtraRegister, x86params.ModRM, x86params.Address);
				if(xRT->x86reg != Reg_EAX) PUSH_RegIfMapped(Reg_EAX, 1, Reg_EAX);
                
                //eax is NEVER equal to ExtraRegister, but this order is necessary.
                Lea(Reg_EAX, ExtraRegister, (_s32) (_s16) __dotI);
                ADD_ImmToReg(ExtraRegister, (_s32) (_s16) __dotI, 0);
			}
			else
			{
				MapRS;

                //feasible for loads only
                if ((xRT->x86reg != Reg_EAX) && ( xRT->mips_reg != 0)) 
                    ExtraRegister = xRT->x86reg;
                else
                {
                    ExtraRegister = FindFreeRegister();
    		        if (ExtraRegister < Reg_ECX)
                        ExtraRegister = Reg_EBP;
                }

				if( xRT->x86reg != Reg_EAX)
					PUSH_RegIfMapped(Reg_EAX, 1, Reg_EAX);

                //ExtraRegister MAY BE equal to RS. This check is necessary.
                //ExtraRegister is never EAX.
                if (xRS->x86reg != Reg_EAX) 
                {
            	    Lea( Reg_EAX, xRS->x86reg, (_s32) (_s16) __dotI);
                    Lea( ExtraRegister, xRS->x86reg, (_s32) (_s16) __dotI);
                }
                else
                {
                    Lea( ExtraRegister, xRS->x86reg, (_s32) (_s16) __dotI);
            	    Lea( Reg_EAX, xRS->x86reg, (_s32) (_s16) __dotI);
                }
            }

			XOR_ImmToReg(Reg_EAX, 2, 0);
			SHR_RegByImm(ExtraRegister, SHIFTER2_READ);
			WC16(0x14FF);
			WC8(0x85|(ExtraRegister<<3));
			WC32((uint32) & gHardwareState.memory_read_functions);
			LOGGING_DYNA(LogDyna("	CALL memory_read_functions[]\n"););
			if( xRT->mips_reg != 0) 
				MOVZX_Memory16ToReg(xRT->x86reg, ModRM_EAX, 0);
            else
                XOR_Reg2ToReg1(xRT->x86reg, xRT->x86reg);

			if( ExtraRegister == Reg_EBP)
				MOV_ImmToReg(Reg_EBP, HardwareStart);
			if( xRT->x86reg != Reg_EAX)
				POP_RegIfMapped(Reg_EAX);
		}
			}
		}
	}
}

// _____________________________________________________________________________
//
void sb(OP_PARAMS)
{
	int		temp;
	uint32	vaddr;

    OpcodePrologue(1, &r4300i_sb, 1, PASS_PARAMS);

#ifdef SAFE_LOADSTORE
	INTERPRET_LOADSTORE(r4300i_sb);
	return;
#endif
	if(ConstMap[xRS->mips_reg].IsMapped)
	{
        vaddr = (uint32) ((_int32) ConstMap[xRS->mips_reg].value + (_s32) (_s16) __I);
		if(ConstMap[xRT->mips_reg].IsMapped)
            FlushOneConstantLo(xRT->mips_reg);
		if((temp = CheckWhereIsMipsReg(xRT->mips_reg)) > -1)
            FlushRegister(FLUSH_TO_MEMORY, temp);

		PUSH_RegIfMapped(Reg_EAX, 1, Reg_ECX);
		PUSH_RegIfMapped(Reg_ECX, 1, Reg_EAX);

		if(ConstMap[xRT->mips_reg].IsMapped) 
		{	
            FlushOneConstantHi(FLUSH_TO_MEMORY, xRT->mips_reg);
		}

        MOV_ImmToReg(Reg_ECX, vaddr ^ 3);
        MOV_ImmToReg(Reg_EAX, vaddr ^ 3);
		SHR_RegByImm(Reg_ECX, SHIFTER2_WRITE);

		WC16(0x14FF);
		WC8(0x85|(Reg_ECX<<3));
		WC32((uint32) & gHardwareState.memory_write_fun_eax_only);
		LOGGING_DYNA(LogDyna("	CALL gHardwareState.memory_write_fun_eax_only[]\n"););

		if(xRT->mips_reg != 0)
			LoadLowMipsCpuRegister(xRT->mips_reg, Reg_ECX);
		else
			XOR_Reg2ToReg1(Reg_ECX, Reg_ECX);

		MOV_RegToMemory(0, Reg_ECX, ModRM_EAX, 0);
		POP_RegIfMapped(Reg_ECX);
		POP_RegIfMapped(Reg_EAX);
	}
	else
	{
		int ExtraRegister = 0;

        //INTERPRET_LOADSTORE(r4300i_sb); return;

        if(ConstMap[xRT->mips_reg].IsMapped) 
		{
			FlushOneConstantLo(xRT->mips_reg);
		}
		if((temp = CheckWhereIsMipsReg(xRT->mips_reg)) > -1)
            FlushRegister(FLUSH_TO_MEMORY, temp);


		if(ConstMap[xRT->mips_reg].IsMapped) 
		{
			FlushOneConstantHi(FLUSH_TO_MEMORY, xRT->mips_reg);
		}

		PUSH_RegIfMapped(Reg_EAX, 1, Reg_ECX);
        ExtraRegister = FindFreeRegister();
        if (ExtraRegister < Reg_EDX)
            ExtraRegister = Reg_EBP;


		if((temp = CheckWhereIsMipsReg(xRS->mips_reg)) > -1)
			MOV_Reg2ToReg1(Reg_EAX, (unsigned char) temp);
		else
			LoadLowMipsCpuRegister(xRS->mips_reg, Reg_EAX);

		Lea(ExtraRegister, Reg_EAX, (_s32) (_s16) __dotI);
        ADD_ImmToReg(Reg_EAX, (_s32) (_s16) __dotI, 0);
		SHR_RegByImm(ExtraRegister, SHIFTER2_WRITE);
		XOR_ImmToReg(Reg_EAX, 3, 0);

        WC16(0x14FF);
		WC8(0x85|(ExtraRegister<<3));
		WC32((uint32) & gHardwareState.memory_write_fun_eax_only);
		LOGGING_DYNA(LogDyna("	CALL gHardwareState.memory_write_fun_eax_only[]\n"););

        PUSH_RegIfMapped(Reg_ECX, 1, Reg_EAX);
        if(xRT->mips_reg != 0)
			LoadLowMipsCpuRegister(xRT->mips_reg, Reg_ECX);
		else
			XOR_Reg2ToReg1(Reg_ECX, Reg_ECX);

		MOV_RegToMemory(0, Reg_ECX, ModRM_EAX, 0);

        if (ExtraRegister == Reg_EBP)
            MOV_ImmToReg(Reg_EBP, HardwareStart);
        POP_RegIfMapped(Reg_ECX);
		POP_RegIfMapped(Reg_EAX);

	}
}

// _____________________________________________________________________________
//
void sh(OP_PARAMS)
{
	int		temp;
	uint32	vaddr;

    OpcodePrologue(1, &r4300i_sh, 1, PASS_PARAMS);

#ifdef SAFE_LOADSTORE
	INTERPRET_LOADSTORE(r4300i_sh);
	return;
#endif
	if(ConstMap[xRS->mips_reg].IsMapped)
	{
        vaddr = (uint32) ((_int32) ConstMap[xRS->mips_reg].value + (_s32) (_s16) __I);
		vaddr ^= 2;
		if(ConstMap[xRT->mips_reg].IsMapped) 
		{
			FlushOneConstantLo(xRT->mips_reg);
			FlushOneConstantHi(FLUSH_TO_MEMORY, xRT->mips_reg);
		}
		if((temp = CheckWhereIsMipsReg(xRT->mips_reg)) > -1)
            FlushRegister(FLUSH_TO_MEMORY, temp);
		PUSH_RegIfMapped(Reg_EAX, 1, Reg_ECX);
		PUSH_RegIfMapped(Reg_ECX, 1, Reg_EAX);

		MOV_ImmToReg(Reg_EAX, vaddr);
		MOV_ImmToReg(Reg_ECX, (vaddr >> SHIFTER2_WRITE));

		WC16(0x14FF);
		WC8(0x85|(Reg_ECX<<3));
		WC32((uint32) & gHardwareState.memory_write_fun_eax_only);
		LOGGING_DYNA(LogDyna("	CALL gHardwareState.memory_write_fun_eax_only[]\n"););

		if(xRT->mips_reg != 0)
			LoadLowMipsCpuRegister(xRT->mips_reg, Reg_ECX);
		else
			XOR_Reg2ToReg1(Reg_ECX, Reg_ECX);
		WC8(0x66);
		MOV_RegToMemory(1, Reg_ECX, ModRM_EAX, 0);

		POP_RegIfMapped(Reg_ECX);
		POP_RegIfMapped(Reg_EAX);
	}
	else
	{
		if(ConstMap[xRT->mips_reg].IsMapped) 
		{
			FlushOneConstantLo(xRT->mips_reg);
			FlushOneConstantHi(FLUSH_TO_MEMORY, xRT->mips_reg);
		}
		if((temp = CheckWhereIsMipsReg(xRT->mips_reg)) > -1)
            FlushRegister(FLUSH_TO_MEMORY, temp);
		PUSH_RegIfMapped(Reg_EAX, 1, Reg_ECX);
		PUSH_RegIfMapped(Reg_ECX, 1, Reg_EAX);

		if((temp = CheckWhereIsMipsReg(xRS->mips_reg)) > -1)
			MOV_Reg2ToReg1(Reg_ECX, (unsigned char) temp);
		else
			LoadLowMipsCpuRegister(xRS->mips_reg, Reg_ECX); /* some things like this will need rethinking for 2-pass. */

		Lea(Reg_EAX, Reg_ECX, (_s32) (_s16) __dotI);
        ADD_ImmToReg(Reg_ECX, (_s32) (_s16) __dotI, 0);
		XOR_ImmToReg(Reg_EAX, 2, 0);
        SHR_RegByImm(Reg_ECX, SHIFTER2_WRITE);

		WC16(0x14FF);
		WC8(0x85|(Reg_ECX<<3));
		WC32((uint32) & gHardwareState.memory_write_fun_eax_only);
		LOGGING_DYNA(LogDyna("	CALL gHardwareState.memory_write_fun_eax_only[]\n"););

		if(xRT->mips_reg != 0)
			LoadLowMipsCpuRegister(xRT->mips_reg, Reg_ECX);
		else
			XOR_Reg2ToReg1(Reg_ECX, Reg_ECX);

		WC8(0x66);
		MOV_RegToMemory(1, Reg_ECX, ModRM_EAX, 0);
		POP_RegIfMapped(Reg_ECX);
		POP_RegIfMapped(Reg_EAX);
	}
}

// _____________________________________________________________________________
//
void cache(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_cache, 0, PASS_PARAMS);

//#define CHECK_CACHE
#ifdef CHECK_CACHE
	INTERPRET(r4300i_cache);
#endif
}

#define _SLL_	0
#define _SRL_	2
#define _SRA_	3
#define _DSLL_	56
#define _DSRL_	58
#define _DSRA_	59

// _____________________________________________________________________________
//
void r4300i_shift(uint32 Instruction)
{
	/* Interpret the shift. (For debugging purposes.) */
	switch(Instruction & 0x3F)
	{
	case 0:
		r4300i_sll(Instruction);
		break;
	case 2:
		r4300i_srl(Instruction);
		break;
	case 3:
	case _DSRA_:
		r4300i_sra(Instruction);
		break;
	}
}

// _____________________________________________________________________________
//
void LogSomething(void)
{
	static int	k = 0;

	if(k == 0)
	{
		InitLogDyna();
		k = 1;
	}

	LogDyna("%08X\n", gHWS_pc);
}


// _____________________________________________________________________________
//
void shift(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_shift, 1, PASS_PARAMS);
    if(xRD->mips_reg == 0) return;


#ifdef SAFE_SHIFTS
	INTERPRET(r4300i_shift);
	return;
#endif

	if(ConstMap[xRT->mips_reg].IsMapped == 1)
	/*
	 * RT is a constant (includes r0) ?
	 */
	{
		uint8	sa = (_u8) __SA;

		switch(r.r_.code & 0x3F)
		{
		case _SLL_:
			/* case _DSLL_: (never use dsll as 32bit. the hiword changes) */
			MapConst(xRD, ((_u32) ConstMap[xRT->mips_reg].value << (_u32) (_u8) sa));
			break;
		case _SRL_:
		case _DSRL_:
			MapConst(xRD, ((_u32) ConstMap[xRT->mips_reg].value >> (_u32) (_u8) sa));
			break;
		case _DSRA_:
		case _SRA_:
			MapConst(xRD, ((_s32) ConstMap[xRT->mips_reg].value >> (_s32) (_u32) (_u8) sa));
			break;
		}
	}
	else
	{
		uint8	sa = (_u8) __SA;

		if(xRD->mips_reg == xRT->mips_reg)
		{
			xRD->IsDirty = 1;
			MapRD;
			if(sa != 0)
			{
				switch(r.r_.code & 0x3F)
				{
				case 0:
					/* case _DSLL_: (never use dsll as 32bit. the hiword changes) */
                    SHL_RegByImm((_u8) xRD->x86reg, sa);
					break;
				case 2:
				case _DSRL_:
					SHR_RegByImm((_u8) xRD->x86reg, sa);
					break;
				case 3:
					SAR_RegByImm((_u8) xRD->x86reg, sa);
					break;
				case _DSRA_:
					SAR_RegByImm((_u8) xRD->x86reg, sa);
					break;
				}
			}
		}
		else
		{
            /* we checked if rt is a const above already. good. */
			Map_RD_NE_RT();
			{
				switch(r.r_.code & 0x3F)
				{
				case 0:
					/* case _DSLL_: (never use dsll as 32bit. the hiword changes) */
					SHL_RegByImm((_u8) xRD->x86reg, (_u8) sa);
					break;
				case 2:
				case _DSRL_:
					SHR_RegByImm((_u8) xRD->x86reg, (_u8) sa);
					break;
				case 3:
					/*
					 * Conker!! ?
					 * LogSomething();
					 */
					SAR_RegByImm((_u8) xRD->x86reg, (_u8) sa);
					break;
				case _DSRA_:
					SAR_RegByImm((_u8) xRD->x86reg, (_u8) sa);
					break;
				}
			}
		}
	}
}

// _____________________________________________________________________________
//
void r4300i_shiftv(uint32 Instruction)
{
	/* Interpret the shift. (For debugging purposes.) */
	switch(Instruction & 0x3f)
	{
	case 4: r4300i_sllv(Instruction); break;
	case 6: r4300i_srlv(Instruction); break;
	case 7: r4300i_srav(Instruction); break;
	}
}

// _____________________________________________________________________________
//


void shiftv(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_shiftv, 1, PASS_PARAMS);
    if(xRD->mips_reg == 0) return;


#ifdef SAFE_SHIFTS
	INTERPRET(r4300i_shiftv);
	return;
#endif

    if( ConstMap[xRS->mips_reg].IsMapped) 
    {
       
        if (ConstMap[xRT->mips_reg].IsMapped) 
        {
            switch (r.r_.code & 0x3f) {
			case 4:
                MapConst(xRD, (uint32)ConstMap[xRT->mips_reg].value<<(uint32)(ConstMap[xRS->mips_reg].value&0x1f));
                break;
			case 6:
                MapConst(xRD, (uint32)ConstMap[xRT->mips_reg].value>>(uint32)(ConstMap[xRS->mips_reg].value&0x1f));
                break;
			case 7:
                MapConst(xRD, (_int32)ConstMap[xRT->mips_reg].value>>(_int32)(ConstMap[xRS->mips_reg].value&0x1f));
                break;
            }
            return;
        }

        xRD->IsDirty = 1;
        MapRegisterNew(FLUSH_TO_MEMORY, xRD, xRT, xRT->mips_reg, -1);

        switch (r.r_.code & 0x3f)
        {
			case 4:
                SHL_RegByImm(xRD->x86reg, ConstMap[xRS->mips_reg].value&0x1f);
                break;
			case 6:
                SHR_RegByImm(xRD->x86reg, ConstMap[xRS->mips_reg].value&0x1f);
                break;
			case 7:
                SAR_RegByImm(xRD->x86reg, ConstMap[xRS->mips_reg].value&0x1f); 
                break;
		}
    }
    else // rs is not a constant
    {
        int i,k;

        //protect ecx: do we need to move this register?
        if ((x86reg[Reg_ECX].mips_reg != -1))
        {
            
            k = FindFreeRegister();

            //try to move it
            if ((k >= 0) && (k!=Reg_ECX))
            {
                memcpy((uint8*)&x86reg[k], (uint8*)&x86reg[Reg_ECX], sizeof(x86regtyp));
                   
                MOV_Reg2ToReg1(k, Reg_ECX);

                
                //Bugfix: need to free the old register!!
                x86reg[Reg_ECX].IsDirty = 0;
                x86reg[Reg_ECX].HiWordLoc = Reg_ECX;
                FlushRegister(FLUSH_TO_MEMORY, Reg_ECX);
                


                //if we moved a loword
                if (x86reg[Reg_ECX].mips_reg >= 0)
                {
                    //if 32bit
                    if (x86reg[Reg_ECX].HiWordLoc == Reg_ECX)
                        x86reg[k].HiWordLoc = k;
                    //else we already copied the correct HiWordLoc
                }
                else //we moved a hiword
                {
                    //search for corr. loword
                    for (i=0;i<8;i++)
                    {
                        if (ItIsARegisterNotToUse(i));
                        else if (x86reg[i].HiWordLoc == Reg_ECX)
                        {
                            x86reg[i].HiWordLoc = k;
                            i = 9;
                        }
                    }
                }
            }
            else
            {
               
               
                /* Protect ECX */
 	            if(x86reg[Reg_ECX].mips_reg > -1)
                {
                    FlushRegister(FLUSH_TO_MEMORY, Reg_ECX);
                }
                else if(x86reg[1].mips_reg == -2)
                {
      	            for(k = 0; k < 8; k++)
	                {
      		            if(ItIsARegisterNotToUse(k));
                        else if(x86reg[k].HiWordLoc == Reg_ECX)
	                    {
	                        FlushRegister(FLUSH_TO_MEMORY, k);
		                    k = 9;
                        }
                    }
                }
            }
        }

        //protect ecx
        x86reg[Reg_ECX].mips_reg = 100;				/* protect ecx. */

	xRD->IsDirty = 1;
	if(xRD->mips_reg == xRT->mips_reg)
	{
        
        if(xRS->mips_reg == xRT->mips_reg)	/* rd=rs=rt */
		{
    		MapRD;
            MOV_Reg2ToReg1(Reg_ECX, xRD->x86reg);
			switch(r.r_.code & 0x3f)
			{
			case 4: SHL_RegByCL(xRD->x86reg); break;
			case 6: SHR_RegByCL(xRD->x86reg); break;
			case 7: SAR_RegByCL(xRD->x86reg); break;
			}
		}
		else	/* rd=rt, rs!=rt */
		{
			MapRS;
		    MapRD;

			MOV_Reg2ToReg1(Reg_ECX, xRS->x86reg);
			switch(r.r_.code & 0x3f)
			{
			case 4: SHL_RegByCL(xRD->x86reg); break;
			case 6: SHR_RegByCL(xRD->x86reg); break;
			case 7: SAR_RegByCL(xRD->x86reg); break;
			}
		}
	}
	/* rd != rt */
    else if(xRD->mips_reg == xRS->mips_reg)
	{

        if( ConstMap[xRT->mips_reg].IsMapped) {
            int k = ConstMap[xRT->mips_reg].value;

            MapRD;
            MOV_Reg2ToReg1(Reg_ECX, xRD->x86reg);
 			MOV_ImmToReg(xRD->x86reg, k);
        } else {
            //can't reorder because rd = rs!

            MapRS;
            MOV_Reg2ToReg1(Reg_ECX, xRS->x86reg);
            Map_RD_NE_RT();

        }
		switch(r.r_.code & 0x3f) {
   			case 4: SHL_RegByCL(xRD->x86reg); break;
   			case 6: SHR_RegByCL(xRD->x86reg); break;
  			case 7: SAR_RegByCL(xRD->x86reg); break;
		}
	}
	else	/* rd != rs, rd!=rt */
    {
           
        xRD->NoNeedToLoadTheLo = 1;

		if(xRS->mips_reg == xRT->mips_reg)
		{
            if(xRT->mips_reg == 0)	/* rd = (rt=0) >> whatever */
            {
		        MapRD;				
				XOR_Reg2ToReg1(xRD->x86reg, xRD->x86reg);
            }
			else
			{
                MapRS;
	    	    MapRD;				
				MOV_Reg2ToReg1(Reg_ECX, xRS->x86reg);
				MOV_Reg2ToReg1(xRD->x86reg, xRS->x86reg);
				switch(r.r_.code & 0x3f)
				{
				case 4: SHL_RegByCL(xRD->x86reg); break;
				case 6: SHR_RegByCL(xRD->x86reg); break;
				case 7: SAR_RegByCL(xRD->x86reg); break;
				}
			}
		}
		else						/* rs != rt */
		{
			
            if(xRS->mips_reg != 0)
			{
                if(ConstMap[xRT->mips_reg].IsMapped)
		    	{
                    MapRS;
                    MapRD;

		   			/*
		    		 * if rt == 0..what here ? ?
			    	 * rs!=0, rt!=0 , rs!=rt
    				 */
	    			MOV_Reg2ToReg1(Reg_ECX, xRS->x86reg);
					MOV_ImmToReg(xRD->x86reg, ConstMap[xRT->mips_reg].value);
				}
        		else
                {
                    //rd!=rs!=rt. arbitrary order.
                    MapRS;
                    Map_RD_NE_RT();
                    MOV_Reg2ToReg1(Reg_ECX, xRS->x86reg);
                    /* we checked if rt is a const above already. good. */
                }

				switch(r.r_.code & 0x3f)
				{
				case 4: SHL_RegByCL(xRD->x86reg); break;
				case 6: SHR_RegByCL(xRD->x86reg); break;
				case 7: SAR_RegByCL(xRD->x86reg); break;
				}
			}
			else					/* rs=0, rt!=0 */
			{
		        Map_RD_NE_RT();
			}
		}
	}

	/* Unprotect ECX */
	if( x86reg[1].mips_reg == 100)
        x86reg[1].mips_reg = -1;
    }
}

// _____________________________________________________________________________
//
void prepare_run_exception(uint32 exception_code)
{
	gHWS_COP0Reg[CAUSE] &= NOT_EXCCODE;
	gHWS_COP0Reg[CAUSE] |= exception_code;
	gHWS_COP0Reg[EPC] = gHWS_pc;
	gHWS_COP0Reg[STATUS] |= EXL;	/* set EXL = 1 */
	gHWS_COP0Reg[CAUSE] &= NOT_BD;	/* clear BD */

	if(exception_code == EXC_CPU)
	{
		gHWS_COP0Reg[CAUSE] &= 0xCFFFFFFF;
		gHWS_COP0Reg[CAUSE] |= CAUSE_CE1;
	}

	TRACE0("Exception in Dyna");
}

void Hello()
{
    MessageBox(0, "Break", "", 0);
}

// _____________________________________________________________________________
//
void syscall(OP_PARAMS)
{
	if(debug_opcode) DisplayError("TODO: OpcodeDebugger: syscall");

    OpcodePrologue(1, &r4300i_syscall, 0, PASS_PARAMS);

	MOV_ImmToMemory(1, ModRM_disp32, (unsigned long) &r.r_.pc, r.r_.pc);
	INTERPRET(r4300i_syscall);
	MOV_ImmToMemory(1, ModRM_disp32, (unsigned long) &r.r_.pc, r.r_.pc);

	// end of compiled block 
	compilerstatus.KEEP_RECOMPILING = FALSE;
	FlushAllRegisters(FLUSH_TO_MEMORY);
	Interrupts(JUMP_TYPE_INDIRECT, r.r_.pc, LINK_NO, 0);
}

// _____________________________________________________________________________
//
void Break(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_break, 0, PASS_PARAMS);

    MOV_ImmToMemory(1, ModRM_disp32, (unsigned long) &r.r_.pc, r.r_.pc);
	INTERPRET(r4300i_break);
	MOV_ImmToMemory(1, ModRM_disp32, (unsigned long) &r.r_.pc, r.r_.pc);

	// end of compiled block
	compilerstatus.KEEP_RECOMPILING = FALSE;
	FlushAllRegisters(FLUSH_TO_MEMORY);
	Interrupts(JUMP_TYPE_INDIRECT, r.r_.pc, LINK_NO, 0);
}

extern void TriggerFPUUnusableException(void);
extern void (*Dyna_Code_Check[]) ();
extern void (*Dyna_Check_Codes) ();

// _____________________________________________________________________________
//
extern BOOL IsBooting;
extern void TriggerFPUUnusableExceptionNewer(int ReturnPC, int InDelaySlot);
extern void TriggerCP0UnusableException(void);


extern int (*cop1_InstructionCHECK_ODD[32]) (OP_PARAMS);


void Preserve()
{
		memcpy(TempConstMap, ConstMap, sizeof(ConstMap));
		memcpy(Tempx86reg, x86reg, sizeof(x86reg));
        memcpy(TempFlushedRegistersMap, FlushedRegistersMap, sizeof(FlushedRegistersMap));

		memcpy(Temp_SSE_ConstMap, SSE_ConstMap, sizeof(SSE_ConstMap));
		memcpy(Temp_SSE_fpureg, fpureg, sizeof(fpureg));
        memcpy(Temp_SSE_FlushedRegistersMap, SSE_FlushedRegistersMap, sizeof(SSE_FlushedRegistersMap));
}

void Restore()
{
		memcpy(ConstMap, TempConstMap, sizeof(ConstMap));
        memcpy(x86reg, Tempx86reg, sizeof(x86reg));
        memcpy(FlushedRegistersMap, TempFlushedRegistersMap, sizeof(FlushedRegistersMap));

		memcpy(SSE_ConstMap, Temp_SSE_ConstMap, sizeof(SSE_ConstMap));
		memcpy(fpureg, Temp_SSE_fpureg,  sizeof(fpureg));
        memcpy(SSE_FlushedRegistersMap, Temp_SSE_FlushedRegistersMap, sizeof(SSE_FlushedRegistersMap));
}


void cop1_with_exception(OP_PARAMS)
{
    
	if( (currentromoptions.FPU_Hack == USEFPUHACK_YES) && compilerstatus.IsFirstFPUInstructionToTest)
	{
		int RememberKeepCompiling = compilerstatus.KEEP_RECOMPILING;
		compilerstatus.IsFirstFPUInstructionToTest = 0;
		TEST_ImmWithMemory((uint32) & gHWS_COP0Reg[STATUS], SR_CU1);
		Jcc_Near_auto(CC_NE, 8);
		Preserve();

		FlushAllRegisters(FLUSH_TO_MEMORY);

		XOR_Reg2ToReg1(Reg_EDX, Reg_EDX);
		if (compilerstatus.BranchDelay)
			INC_Reg(1, Reg_EDX);

		MOV_ImmToReg(Reg_ECX, gHWS_pc);
		X86_CALL((uint32)TriggerFPUUnusableExceptionNewer); //this raises an exception for our new 1964 exception vector.

#ifdef _DEBUG
		InterruptsSafe(JUMP_TYPE_DIRECT, 0x80000180, LINK_NO, 0); 
#endif

		Restore();
		SetNearTarget(8);
		compilerstatus.KEEP_RECOMPILING = RememberKeepCompiling;
	}
}

// _____________________________________________________________________________
//
void sync(OP_PARAMS)
{
	compilerstatus.cp0Counter += 1;
}

// _____________________________________________________________________________
//
void r4300i_mf_mt(uint32 Instruction)
{
	switch(Instruction & 0x1f)
	{
	case 16:	r4300i_mfhi(Instruction); break;
	case 18:	r4300i_mflo(Instruction); break;
	case 17:	r4300i_mthi(Instruction); break;
	case 19:	r4300i_mtlo(Instruction); break;
	}
}

// _____________________________________________________________________________
//
void mf_mt(OP_PARAMS)
{
	int Use32bit = 0;

    OpcodePrologue(1, &r4300i_mf_mt, 0, PASS_PARAMS);

//Interpreted because Goldeneye won't work in 32bit without this (due to MFLO).
#ifdef SAFE_LOADSTORE

	switch(r.r_.code & 0x1f)
	{
	case 16:	INTERPRET(r4300i_mfhi); break;
	case 18:	INTERPRET(r4300i_mflo); break;
	case 17:	INTERPRET(r4300i_mthi); break;
	case 19:	INTERPRET(r4300i_mtlo); break;
	}    
    return;
#endif

    /*
	 * note: RD (or RS) is a misnomer. It really corresponds to LO (or HI), ?
	 * but it works in our Mapping macros ?
	 * MFHI(16) | MTHI(17) | MFLO(18) | MTLO(19)
	 */
	switch(r.r_.code & 0x1f)
	{
    case 16:	if(xRD->mips_reg == 0) {MapConst(xRD, 0);return;} else xRS->mips_reg = __HI; break;	/* mfhi */
    case 18:	if(xRD->mips_reg == 0) {MapConst(xRD, 0);return;} else xRS->mips_reg = __LO; break;	/* mflo */
	case 17:	xRD->mips_reg = __HI; break;	/* mthi */
	case 19:	xRD->mips_reg = __LO; break;	/* mtlo */
	}

  
    //Strange anomaly:  We can't regcache MFLO in the 32bit core, or Goldeneye will break.
    if ( ((r.r_.code & 0x1f) == 18) && (currentromoptions.Assume_32bit == ASSUME_32BIT_YES))
    {
        FlushAllRegisters(FLUSH_TO_MEMORY);
        MOV_MemoryToReg(Reg_EAX, ModRM_disp32, (uint32)&gHWS_GPR(xRS->mips_reg));
        MOV_MemoryToReg(Reg_ECX, ModRM_disp32, (uint32)&gHWS_GPR(xRS->mips_reg)+4);
        MOV_RegToMemory(1, Reg_EAX, ModRM_disp32, (uint32)&gHWS_GPR(xRD->mips_reg));
        MOV_RegToMemory(1, Reg_ECX, ModRM_disp32, (uint32)&gHWS_GPR(xRD->mips_reg)+4);
    } 
    else
    {
        xRD->IsDirty = 1;
        xRD->NoNeedToLoadTheLo = 1;
	    xRD->NoNeedToLoadTheHi = 1;

    	MapRD;

	    if(ConstMap[xRS->mips_reg].IsMapped)
	    {
		    __int32 temp = ConstMap[xRS->mips_reg].value;
        
            MOV_ImmToReg(xRD->x86reg, temp);
	    	if(!Use32bit) MOV_ImmToReg(xRD->HiWordLoc, ((_int32)temp >> 31));
	    }
	    else
	    {
	    	// we checked if rs is a const above already. good.
    	    MapRS_To__WithLoadOnlyFunction(xRD, Use32bit, MOV_MemoryToReg);
        }
    }
}

// _____________________________________________________________________________
//
void r4300i_mul(uint32 Instruction)
{
	switch(Instruction & 0x3f)
	{
	case 24:	r4300i_mult(Instruction); break;
	case 25:	r4300i_multu(Instruction); break;
	}
}




// _____________________________________________________________________________
//
void mul(OP_PARAMS)
{
	int High = __HI;
	int Low = __LO;

//TODO: Prologue	
    OpcodePrologue(2, &r4300i_mul, 1, PASS_PARAMS);

#ifdef SAFE_MATH
	INTERPRET(r4300i_mul);
	return;
#endif



	FreeMipsRegister(Low);
	FreeMipsRegister(High);

	xRD->IsDirty = 1;
	xRD->mips_reg = __LO;
	xRD->Is32bit = 0;
	xRD->NoNeedToLoadTheLo = 1;
	xRD->NoNeedToLoadTheHi = 1;


    if ( (ConstMap[xRS->mips_reg].IsMapped && (ConstMap[xRS->mips_reg].value == 0)) 
      || (ConstMap[xRT->mips_reg].IsMapped && (ConstMap[xRT->mips_reg].value == 0)) )
    {
        

        MapRD;
        XOR_Reg2ToReg1(xRD->x86reg, xRD->x86reg);
        XOR_Reg2ToReg1(xRD->HiWordLoc, xRD->HiWordLoc);
        // split up the Lo and Hi into 2 registers.
        	x86reg[xRD->HiWordLoc].IsDirty = 1;
	        x86reg[xRD->HiWordLoc].Is32bit = 1;
	        x86reg[xRD->HiWordLoc].mips_reg = High;
        	x86reg[xRD->HiWordLoc].HiWordLoc = xRD->HiWordLoc;

        	x86reg[xRD->x86reg].IsDirty = 1;
        	x86reg[xRD->x86reg].Is32bit = 1;
        	x86reg[xRD->x86reg].mips_reg = Low;
        	x86reg[xRD->x86reg].HiWordLoc = xRD->x86reg;
        return;
    }

//    if ( ConstMap[xRS->mips_reg].IsMapped || ConstMap[xRT->mips_reg].IsMapped  )
  //     DisplayError("Hi");




    MapRS;
	MapRT;
    MapRD;

	if((xRD->x86reg != Reg_EAX) && (xRD->HiWordLoc != Reg_EAX)) PUSH_RegIfMapped(Reg_EAX, 1, Reg_EDX);
	if((xRD->x86reg != Reg_EDX) && (xRD->HiWordLoc != Reg_EDX)) PUSH_RegIfMapped(Reg_EDX, 1, Reg_EAX);

	if(xRT->x86reg == Reg_EAX)
	{
		switch(__F)
		{
		case 24: /* case 32: */IMUL_EAXWithReg(xRS->x86reg); break;
		case 25: /* case 33: */MUL_EAXWithReg(xRS->x86reg); break;
		}
	}
	else
	{
		MOV_Reg2ToReg1(Reg_EAX, xRS->x86reg);
		switch(__F)
		{
		case 24: /* case 32: */IMUL_EAXWithReg(xRT->x86reg); break;
		case 25: /* case 33: */MUL_EAXWithReg(xRT->x86reg); break;
		}
	}

	if((xRD->HiWordLoc == Reg_EAX) || (xRD->x86reg == Reg_EDX))
	{
		High = __LO;
		Low = __HI;
		MOV_Reg2ToReg1(xRD->HiWordLoc, Reg_EAX);
		MOV_Reg2ToReg1(xRD->x86reg, Reg_EDX);
	}
	else
	{
		MOV_Reg2ToReg1(xRD->x86reg, Reg_EAX);
		MOV_Reg2ToReg1(xRD->HiWordLoc, Reg_EDX);
	}

	if((xRD->x86reg != Reg_EDX) && (xRD->HiWordLoc != Reg_EDX)) POP_RegIfMapped(Reg_EDX);
	if((xRD->x86reg != Reg_EAX) && (xRD->HiWordLoc != Reg_EAX)) POP_RegIfMapped(Reg_EAX);

	/* split up the Lo and Hi into 2 registers. */
	x86reg[xRD->HiWordLoc].IsDirty = 1;
	x86reg[xRD->HiWordLoc].Is32bit = 1;
	x86reg[xRD->HiWordLoc].mips_reg = High;
	x86reg[xRD->HiWordLoc].HiWordLoc = xRD->HiWordLoc;

	x86reg[xRD->x86reg].IsDirty = 1;
	x86reg[xRD->x86reg].Is32bit = 1;
	x86reg[xRD->x86reg].mips_reg = Low;
	x86reg[xRD->x86reg].HiWordLoc = xRD->x86reg;
}

// _____________________________________________________________________________
//
void Div(OP_PARAMS)
{
	int temp, k;

    OpcodePrologue(75, &r4300i_div, 1, PASS_PARAMS);


#ifdef SAFE_MATH
	INTERPRET(r4300i_div);
	return;
#endif
    
    FreeMipsRegister(__LO);
	FreeMipsRegister(__HI);

	if(ConstMap[__RS].IsMapped) FlushOneConstantLo(__RS);
	if(ConstMap[__RT].IsMapped) FlushOneConstantLo(__RT);
	if(ConstMap[__RS].IsMapped) FlushOneConstantHi(FLUSH_TO_MEMORY, __RS);
	if(ConstMap[__RT].IsMapped) FlushOneConstantHi(FLUSH_TO_MEMORY, __RT);

	if((temp = CheckWhereIsMipsReg(__RS)) > -1) FlushRegister(FLUSH_TO_MEMORY, temp);
	if((temp = CheckWhereIsMipsReg(__RT)) > -1) FlushRegister(FLUSH_TO_MEMORY, temp);
	if(x86reg[0].mips_reg > -1)
		FlushRegister(FLUSH_TO_MEMORY, 0);
	else if(x86reg[0].mips_reg == -2)
	{
		for(k = 0; k < 8; k++)
		{
			if(ItIsARegisterNotToUse(k));

			/* Don't use these registers for mapping */
			else if(x86reg[k].HiWordLoc == 0)
			{
				FlushRegister(FLUSH_TO_MEMORY, k);
				k = 9;
			}
		}
	}

	if(x86reg[2].mips_reg > -1)
		FlushRegister(FLUSH_TO_MEMORY, 2);
	else if(x86reg[2].mips_reg == -2)
	{
		for(k = 0; k < 8; k++)
		{
			if(ItIsARegisterNotToUse(k));

			/* Don't use these registers for mapping */
			else if(x86reg[k].HiWordLoc == 2)
			{
				FlushRegister(FLUSH_TO_MEMORY, k);
				k = 9;
			}
		}
	}

	//using 3 registers here..can't relocate (yet).
    
    PUSH_RegIfMapped(Reg_ECX, 0, -1);
	LoadLowMipsCpuRegister(__RT, Reg_ECX);
	TEST_Reg2WithReg1(Reg_ECX, Reg_ECX);

	Jcc_auto(CC_E, 12);
	LoadLowMipsCpuRegister(__RS, Reg_EAX);
	MOV_Reg2ToReg1(Reg_EDX, Reg_EAX);
	SAR_RegByImm(Reg_EDX, 0x1F);

	IDIV_EAXWithReg(1, Reg_ECX);

	x86reg[0].IsDirty = 1;
	x86reg[0].Is32bit = 1;
	x86reg[0].mips_reg = __LO;
	x86reg[0].BirthDate = ThisYear;
	x86reg[0].HiWordLoc = 0;

	x86reg[2].IsDirty = 1;
	x86reg[2].Is32bit = 1;
	x86reg[2].mips_reg = __HI;
	x86reg[2].BirthDate = ThisYear;
	x86reg[2].HiWordLoc = 2;

	SetTarget(12);
	POP_RegIfMapped(Reg_ECX);

	SAVE_OP_COUNTER_INCREASE(PCLOCKDIV * 2 * VICounterFactors[CounterFactor]);

}

// _____________________________________________________________________________
//
void divu(OP_PARAMS)
{
	int temp, k;

    OpcodePrologue(25, &r4300i_divu, 1, PASS_PARAMS);


#ifdef SAFE_MATH
	INTERPRET(r4300i_divu);
	return;
#endif

	/*
	 * Cheesy alternative to FlushAllRegisters() but more efficient.
	 */
	FreeMipsRegister(__LO);
	FreeMipsRegister(__HI);
	if(ConstMap[__RS].IsMapped) FlushOneConstantLo(__RS); //Doesn't unmap yet
	if(ConstMap[__RT].IsMapped) FlushOneConstantLo(__RT); //Doesn't unmap yet
	if(ConstMap[__RS].IsMapped) FlushOneConstantHi(FLUSH_TO_MEMORY, __RS);
	if(ConstMap[__RT].IsMapped) FlushOneConstantHi(FLUSH_TO_MEMORY, __RT);

	if((temp = CheckWhereIsMipsReg(__RS)) > -1) FlushRegister(FLUSH_TO_MEMORY, temp);
	if((temp = CheckWhereIsMipsReg(__RT)) > -1) FlushRegister(FLUSH_TO_MEMORY, temp);
	if(x86reg[0].mips_reg > -1)
		FlushRegister(FLUSH_TO_MEMORY, 0);
	else if(x86reg[0].mips_reg == -2)
	{
		for(k = 0; k < 8; k++)
		{
			if(ItIsARegisterNotToUse(k));

			/* Don't use these registers for mapping */
			else if(x86reg[k].HiWordLoc == 0)
			{
				FlushRegister(FLUSH_TO_MEMORY, k);
				k = 9;
			}
		}
	}

	if(x86reg[2].mips_reg > -1)
		FlushRegister(FLUSH_TO_MEMORY, 2);
	else if(x86reg[2].mips_reg == -2)
	{
		for(k = 0; k < 8; k++)
		{
			if(ItIsARegisterNotToUse(k));

			/* Don't use these registers for mapping */
			else if(x86reg[k].HiWordLoc == 2)
			{
				FlushRegister(FLUSH_TO_MEMORY, k);
				k = 9;
			}
		}
	}

	PUSH_RegIfMapped(Reg_ECX, 0, -1);
	LoadLowMipsCpuRegister(__RT, Reg_ECX);
	TEST_Reg2WithReg1(Reg_ECX, Reg_ECX);

	Jcc_auto(CC_E, 0);
	LoadLowMipsCpuRegister(__RS, Reg_EAX);
	XOR_Reg2ToReg1(Reg_EDX, Reg_EDX);
	DIV_EAXWithReg(1, Reg_ECX);

	x86reg[0].IsDirty = 1;
	x86reg[0].Is32bit = 1;
	x86reg[0].mips_reg = __LO;
	x86reg[0].BirthDate = ThisYear;
	x86reg[0].HiWordLoc = 0;

	x86reg[2].IsDirty = 1;
	x86reg[2].Is32bit = 1;
	x86reg[2].mips_reg = __HI;
	x86reg[2].BirthDate = ThisYear;
	x86reg[2].HiWordLoc = 2;

	SetTarget(0);
	POP_RegIfMapped(Reg_ECX);

	SAVE_OP_COUNTER_INCREASE(PCLOCKDIV * 2 * VICounterFactors[CounterFactor]);
}

void Negate(_u32 OperandSize, _u32 reg)
{
    XOR_ShortToReg((_u8) reg, 0xFF); 
    INC_Reg(1, (_u8) reg);
}

// _____________________________________________________________________________
//
void MipsAdd(int subtraction)
{
	if( (ConstMap[xRT->mips_reg].IsMapped == 1) && (ConstMap[xRS->mips_reg].IsMapped == 1)) {
		if( !subtraction)
			MapConst( xRD, (ConstMap[xRS->mips_reg].value + ConstMap[xRT->mips_reg].value));
		else
			MapConst( xRD, (ConstMap[xRS->mips_reg].value - ConstMap[xRT->mips_reg].value));
	} else {
		if( xRD->mips_reg == xRT->mips_reg) {
			if( xRS->mips_reg == xRT->mips_reg) {	/* rd=rs=rt */
    			if( subtraction)	{
					MapConst(xRD, 0);
				} else {
					xRD->IsDirty = 1;
					MapRD;
					ADD_Reg2ToReg1(xRD->x86reg, xRD->x86reg);
				}
			} else { /* rd=rt, rs!=rt */
				if( xRS->mips_reg == 0)
					if( subtraction) {
						xRD->IsDirty = 1;
						MapRD;
						if( subtraction) {
							NEGATE( xRD)
						}
						return;
					} else
						return;

				xRD->IsDirty = 1;
				MapRD;
				if( subtraction) {
					NEGATE( xRD)
				}
				if( ConstMap[xRS->mips_reg].IsMapped) {
					ADD_ImmToReg(xRD->x86reg, ConstMap[xRS->mips_reg].value, 0);
				} else {
					/* we checked if rs is a const above, good. */
					MapRS_To( xRD, 1, ADD_MemoryToReg);
				}
			}
        // rd != rt
		} else if( xRD->mips_reg == xRS->mips_reg) {
			
			if( xRT->mips_reg == 0)
            {
                if (!CheckIs32Bit(xRD->mips_reg))
                {
                    xRD->IsDirty = 1;
                    MapRD;
                }
				return;
            }
			if( ConstMap[xRT->mips_reg].IsMapped) {
				xRD->IsDirty = 1;
				MapRD;

				if( !subtraction)	/* add */
					ADD_ImmToReg(xRD->x86reg, ConstMap[xRT->mips_reg].value, 0);
				else {
					SUB_ImmFromReg( 1, xRD->x86reg, ConstMap[xRT->mips_reg].value, 0);
				}
			} else {
				if( ConstMap[xRS->mips_reg].IsMapped) {

                    
                    int tempRS = ConstMap[xRS->mips_reg].value;

                    xRD->NoNeedToLoadTheLo = 1;
					xRD->IsDirty = 1;
					MapRT;
                    MapRD;
					if( subtraction) 
						tempRS = -tempRS;


                    //when subtraction, this is RD=RT-RS, which needs to be converted to -RD=RS-RT)
                    Lea(xRD->x86reg, xRT->x86reg, tempRS);
					if( subtraction)
                    {
						NEGATE( xRD);
                    }
					return;
				}

//                if (subtraction){INTERPRET(r4300i_sub); return; }
  //              else {INTERPRET(r4300i_add); return;}

/*
                xRD->IsDirty = 1;
                MapRD;
                MapRT;
				if( !subtraction) {	// add
                    ADD_Reg2ToReg1(xRD->x86reg, xRT->x86reg);
				} else {
                    SUB_Reg1OfReg2(xRT->x86reg, xRD->x86reg);
				}
                return;
  */              

                //if ((xRD->mips_reg == _t1)&& (!subtraction)){ INTERPRET(r4300i_addu); return;}

                xRD->IsDirty = 1;
				MapRD;
				if( !subtraction) {	/* add */
					//Conker problem (addu)
                    MapRT_To( xRD, 1, ADD_MemoryToReg);
				} else {
					MapRT_To( xRD, 1, SUB_MemoryToReg);
				}
			}
		// rd != rs
        } else {
			xRD->IsDirty = 1;
			xRD->NoNeedToLoadTheLo = 1;
			MapRD;
			if( xRS->mips_reg == xRT->mips_reg) {
				if( xRT->mips_reg == 0) {	/* rd = 0+0 */
					XOR_Reg2ToReg1(xRD->x86reg, xRD->x86reg);	/* xor rd, rd */
				} else if( subtraction) { /* rd = rs-rs */
					XOR_Reg2ToReg1(xRD->x86reg, xRD->x86reg);
				} else { /* rd = rs+rs */
					Map_RD_NE_RT();
					ADD_Reg2ToReg1(xRD->x86reg, xRD->x86reg);
				}
			} else { /* rs != rt */
				if( xRS->mips_reg != 0) {
					if( ConstMap[xRS->mips_reg].IsMapped) {
						MOV_ImmToReg(xRD->x86reg, ConstMap[xRS->mips_reg].value);
					} else {
						Map_RD_NE_RS();
					}
					if( xRT->mips_reg == 0)
						return;
					if( ConstMap[xRT->mips_reg].IsMapped) {
						/* rs!=0, rt!=0 , rs!=rt */
						if( !subtraction)	/* add */
							ADD_ImmToReg(xRD->x86reg, ConstMap[xRT->mips_reg].value, 0);
						else {
							SUB_ImmFromReg( 1, xRD->x86reg, ConstMap[xRT->mips_reg].value, 0);
						}
					} else {
						if( !subtraction) {	/* add */
							MapRT_To( xRD, 1, ADD_MemoryToReg);
						} else {
							MapRT_To( xRD, 1, SUB_MemoryToReg);
						}
					}
				} else {						/* rs=0, rt!=0 */
					if( ConstMap[xRT->mips_reg].IsMapped) {
						if( !subtraction)
							MapConst( xRD, ConstMap[xRT->mips_reg].value);
						else
							MapConst( xRD, -ConstMap[xRT->mips_reg].value);
						return;
					}
					Map_RD_NE_RT();
					if( subtraction) {
						NEGATE( xRD)
					}
				}
			}
		}
	}
}


extern DWORD CompileSourceCode(char* _szSourceCode, BYTE* _pMachineCode, DWORD _dwMaxSize);

int TemporaryVariable;

// _____________________________________________________________________________
//
void add(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_add, 1, PASS_PARAMS);
    if(xRD->mips_reg == 0) return;

#ifdef SAFE_MATH
    INTERPRET(r4300i_add);
    return;
#endif


    if (xRT->mips_reg == xRS->mips_reg)
        MipsAdd(0);
    else
    Gate_Logic(1, 
      LOGIC_ADD,
      xRD, xRS, xRT, ConstMap[xRS->mips_reg].value+ConstMap[xRT->mips_reg].value,
      &ADD_Reg2ToReg1, &Dummy2, &ADD_ImmToReg);
}

// _____________________________________________________________________________
//
void addu(OP_PARAMS)
{


    OpcodePrologue(1, &r4300i_addu, 1, PASS_PARAMS);
    if(xRD->mips_reg == 0) return;

#ifdef SAFE_MATH
    INTERPRET(r4300i_addu);
    return;
#endif

    if (xRT->mips_reg == xRS->mips_reg)
        MipsAdd(0);
    else
    Gate_Logic(1, 
      LOGIC_ADD,
      xRD, xRS, xRT, ConstMap[xRS->mips_reg].value+ConstMap[xRT->mips_reg].value,
      &ADD_Reg2ToReg1, &Dummy2, &ADD_ImmToReg);
}

// _____________________________________________________________________________
//
void sub(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_sub, 1, PASS_PARAMS);
    if(xRD->mips_reg == 0) return;
    MipsAdd(1);
}

// _____________________________________________________________________________
//
void subu(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_subu, 1, PASS_PARAMS);
    if(xRD->mips_reg == 0) return;
    MipsAdd(1);
}

// _____________________________________________________________________________
//
void and(OP_PARAMS)
{
	int Use32bit = 0;

    OpcodePrologue(1, &r4300i_and, 0, PASS_PARAMS);
    if(xRD->mips_reg == 0) return;

#ifdef SAFE_GATES
	INTERPRET(r4300i_and);
	return;
#endif

        Gate_Logic(currentromoptions.Assume_32bit == ASSUME_32BIT_YES, 
          LOGIC_AND,  
          xRD, xRS, xRT, ConstMap[xRS->mips_reg].value&ConstMap[xRT->mips_reg].value,
          &AND_Reg2ToReg1, &Dummy2, &AND_ImmToReg);
}


// _____________________________________________________________________________
//
void xor(OP_PARAMS)
{
	int Use32bit = 0;

    OpcodePrologue(1, &r4300i_xor, 0, PASS_PARAMS);
    if(xRD->mips_reg == 0) return;

#ifdef SAFE_GATES
	INTERPRET(r4300i_xor);
	return;
#endif

	if (xRS->mips_reg == xRT->mips_reg)
        MapConst(xRD, 0);
    else

    Gate_Logic(currentromoptions.Assume_32bit == ASSUME_32BIT_YES, 
      LOGIC_XOR,
      xRD, xRS, xRT, ConstMap[xRS->mips_reg].value^ConstMap[xRT->mips_reg].value,
      &XOR_Reg2ToReg1, &Dummy2, &XOR_ImmToReg);
}

// _____________________________________________________________________________
//
void nor(OP_PARAMS)
{
    OpcodePrologue(1, &r4300i_nor, 0, PASS_PARAMS);
    if(xRD->mips_reg == 0) return;

#ifdef SAFE_GATES
	INTERPRET(r4300i_nor);
	return;
#endif

    //special case
    if(xRS->mips_reg == xRT->mips_reg)
	{
		int Use32bit = (currentromoptions.Assume_32bit == ASSUME_32BIT_YES);
        xRD->IsDirty = 1;
        xRD->Is32bit |= Use32bit;
		MapRegisterNew(FLUSH_TO_MEMORY, xRD, xRD, xRS->mips_reg, -1);

		XOR_ImmToReg(xRD->x86reg, 0xffffffff, 0);
		if(!Use32bit) XOR_ImmToReg(xRD->HiWordLoc, 0xffffffff, 0);
	}
    //normal behavior
    else
        Gate_Logic(currentromoptions.Assume_32bit == ASSUME_32BIT_YES, 
            LOGIC_NOR,
            xRD, xRS, xRT, ~(ConstMap[xRS->mips_reg].value|ConstMap[xRT->mips_reg].value),
            &OR_Reg2ToReg1, &NOT_Reg, &OR_ImmToReg);
}

// _____________________________________________________________________________
//
void dsll32(OP_PARAMS)
{
    int temp2;
    int RememberAssume32bit;
    
    OpcodePrologue(1, &r4300i_dsll32, 0, PASS_PARAMS);
    if(xRD->mips_reg == 0) return;


#ifdef SAFE_DOUBLE_SHIFTS2
	INTERPRET(r4300i_dsll32);
#else

        RememberAssume32bit = currentromoptions.Assume_32bit;
        currentromoptions.Assume_32bit = ASSUME_32BIT_NO;

        if(ConstMap[xRT->mips_reg].IsMapped)
            FlushOneConstant(FLUSH_TO_MEMORY, xRT->mips_reg);
		else if((temp2 = CheckWhereIsMipsReg(xRT->mips_reg)) > -1)
		    FlushRegister(FLUSH_TO_MEMORY, temp2);

        xRD->NoNeedToLoadTheLo = 1;
        xRD->NoNeedToLoadTheHi = 1;
        xRD->IsDirty = 1;
        MapRD;


        MOV_MemoryToReg(xRD->HiWordLoc, ModRM_disp32, (uint32)&gHWS_GPR(xRT->mips_reg));
        XOR_Reg2ToReg1(xRD->x86reg, xRD->x86reg);
        SHL_RegByImm(xRD->HiWordLoc, __SA);

        FlushRegister(FLUSH_TO_MEMORY, xRD->x86reg);

        currentromoptions.Assume_32bit = RememberAssume32bit;

#endif
}

// _____________________________________________________________________________
//
void dsrl32(OP_PARAMS)
{
    int temp2;
    int RememberAssume32bit;
    
    OpcodePrologue(1, &r4300i_dsrl32, 0, PASS_PARAMS);
    if(xRD->mips_reg == 0) return;


#ifdef SAFE_DOUBLE_SHIFTS2
	INTERPRET(r4300i_dsrl32);
#else

        RememberAssume32bit = currentromoptions.Assume_32bit;
        currentromoptions.Assume_32bit = ASSUME_32BIT_NO;

        if(ConstMap[xRT->mips_reg].IsMapped)
            FlushOneConstant(FLUSH_TO_MEMORY, xRT->mips_reg);
		else if((temp2 = CheckWhereIsMipsReg(xRT->mips_reg)) > -1)
		    FlushRegister(FLUSH_TO_MEMORY, temp2);

        xRD->NoNeedToLoadTheLo = 1;
        xRD->NoNeedToLoadTheHi = 1;
        xRD->IsDirty = 1;
        MapRD;


        MOV_MemoryToReg(xRD->x86reg, ModRM_disp32, (uint32)&gHWS_GPR(xRT->mips_reg)+4);
        XOR_Reg2ToReg1(xRD->HiWordLoc, xRD->HiWordLoc);
        SHR_RegByImm(xRD->x86reg, __SA);

        FlushRegister(FLUSH_TO_MEMORY, xRD->x86reg);

        currentromoptions.Assume_32bit = RememberAssume32bit;

#endif
}

// _____________________________________________________________________________
//
void dsra32(OP_PARAMS)
{
    int temp2;
    int RememberAssume32bit;
    
    OpcodePrologue(1, &r4300i_dsra32, 0, PASS_PARAMS);
    if(xRD->mips_reg == 0) return;


#ifdef SAFE_DOUBLE_SHIFTS2
	INTERPRET(r4300i_dsra32);
#else

        RememberAssume32bit = currentromoptions.Assume_32bit;
        currentromoptions.Assume_32bit = ASSUME_32BIT_NO;

        if(ConstMap[xRT->mips_reg].IsMapped)
            FlushOneConstant(FLUSH_TO_MEMORY, xRT->mips_reg);
		else if((temp2 = CheckWhereIsMipsReg(xRT->mips_reg)) > -1)
		    FlushRegister(FLUSH_TO_MEMORY, temp2);

        xRD->NoNeedToLoadTheLo = 1;
        xRD->NoNeedToLoadTheHi = 1;
        xRD->IsDirty = 1;
        MapRD;


        MOV_MemoryToReg(xRD->x86reg, ModRM_disp32, (uint32)&gHWS_GPR(xRT->mips_reg)+4);
        MOV_Reg2ToReg1(xRD->HiWordLoc, xRD->x86reg);
        SAR_RegByImm(xRD->x86reg, __SA);
        SAR_RegByImm(xRD->HiWordLoc, 31);

        FlushRegister(FLUSH_TO_MEMORY, xRD->x86reg);

        currentromoptions.Assume_32bit = RememberAssume32bit;

#endif
}
