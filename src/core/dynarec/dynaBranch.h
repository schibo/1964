//TODO: fix cpOCounter on _Redos.


/*$T dynaBranch.h GC 1.136 03/09/02 13:36:14 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Jump and branch opcodes
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


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
#ifndef __DYNABRANCH_H
#define __DYNABRANCH_H


//Compile_Slot_AndInterruptsNewTest = calls InterruptsNewTest to patch b4 the translatepc
//Compile_Slot_AndInterruptsNewTest = calls Interrupts to patches after the translatepc
//InterruptsNewTest = patches b4 the translatepc
//Interrupts = patches after the translat

#include "../debug_option.h"
#include "../1964ini.h"
#include "../compiler.h"
#include "../Registers.h"

extern N64::CRegisters r;

extern unsigned int HardwareStart;
extern void			MapRegisterNew(int To_XMM_Or_Memory, x86regtyp *Conditions, x86regtyp *xFrom, int keep2, int keep3);
extern BOOL IsBooting;
/*
 * These are defines for branches that set GPR[31] to the link value. ?
 * This does not refer to our 4k link :)
 */
#define LINK_YES			1
#define LINK_NO				0

#define SPEEDHACK_MISSING	DisplayError("Missing SpeedHack");

#define SAFE_NEAR_ONLY	0

enum { JUMP_TYPE_INDIRECT, JUMP_TYPE_DIRECT, JUMP_TYPE_CONTINUE };


//#define _OPCODE_DEBUG_BRANCH_(x) _SAFETY_CPU_(x)
#define _OPCODE_DEBUG_BRANCH_(x)

#define SET_TARGET_1_LINK(pc, target) \
	if(currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES) \
	{ \
		if(IsTargetPcInTheSame4KB(pc, target)) \
		{ \
			current_block_entry->need_target_1 = TRUE; \
			current_block_entry->target_1_pc = (target); \
		} \
	}

#define SET_TARGET_2_LINK(pc, target) \
	if(currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES) \
	{ \
		if(IsTargetPcInTheSame4KB(pc, target)) \
		{ \
			current_block_entry->need_target_2 = TRUE; \
			current_block_entry->target_2_pc = (target); \
		} \
	}

#define SPEED_HACK	if(__I == -1) \
	{ \
		if(compilerstatus.pcptr[1] == 0) \
		{ \
			PushMap(); \
			X86_CALL((_u32) DoSpeedHack); \
			PopMap(); \
		} \
	}

#define J_SPEED_HACK	if((r.r_.pc == aValue) && compilerstatus.pcptr[1] == 0) \
	{ \
		/* MOV_ImmToMemory(1, ModRM_disp32, (_u32)&gHWS_COP0Reg[COUNT], max_vi_count); */ \
		PushMap(); \
		X86_CALL((_u32) DoSpeedHack); \
		PopMap(); \
	}

extern MapConstant	ConstMap[NUM_CONSTS];
extern x86regtyp	x86reg[8];
extern FlushedMap	FlushedRegistersMap[NUM_CONSTS];
MapConstant			TempConstMap[NUM_CONSTS];
x86regtyp			Tempx86reg[8];
FlushedMap          TempFlushedRegistersMap[NUM_CONSTS];


MapConstant			TempConstMap_Debug[NUM_CONSTS];
x86regtyp			Tempx86reg_Debug[8];

BOOL				CompilingSlot = FALSE;
void				Interrupts(uint32 JumpType, uint32 targetpc, uint32 DoLink, uint32 LinkVal);
void				InterruptsSafe(uint32 JumpType, uint32 targetpc, uint32 DoLink, uint32 LinkVal);
void				InterruptsExperiment(uint32 JumpType, uint32 targetpc, uint32 DoLink, uint32 LinkVal);
void				InterruptsNewTest(uint32 JumpType, uint32 targetpc, uint32 DoLink, uint32 LinkVal);


/*
 =======================================================================================================================
 =======================================================================================================================
 */

//This Cannot accept ebp, esp.
void LoadLowMipsCpuRegister(unsigned long mips_reg, unsigned char x86_reg)
{
	if(mips_reg == 0)
		XOR_Reg2ToReg1(x86_reg, x86_reg);
	else
	{
		// positive 32bit displacement. Only GPR_HI/GPR_LO use this. 
		x86params.ModRM = ModRM_disp32;
		x86params.Address = (_u32) & gHWS_GPR(0) + (((mips_reg)) << 3);
	//	FetchEBP_Params(mips_reg);
        MOV_MemoryToReg(x86_reg, x86params.ModRM, x86params.Address);

	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void LoadHighMipsCpuRegister(unsigned long mips_reg, unsigned char x86_reg)
{
	if(mips_reg == 0)
		XOR_Reg2ToReg1(x86_reg, x86_reg);
	else
	{
		FetchEBP_Params(mips_reg);
		MOV_MemoryToReg(x86_reg, x86params.ModRM, 4 + x86params.Address);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
extern x86regtyp fpureg[8];
extern MapConstant SSE_ConstMap[32];
extern MapConstant Temp_SSE_ConstMap[32];
extern FlushedMap    SSE_FlushedRegistersMap[8];
extern FlushedMap    Temp_SSE_FlushedRegistersMap[8];
extern x86regtyp        Temp_SSE_fpureg[8];

void Compile_Slot_And_InterruptsExperiment(_u32 pc, uint32 targetpc, uint32 DoLink, uint32 LinkVal, int PreserveMap)
{
	_u8 op;

	if(CompilingSlot)
	{
		TRACE1("Warning, Branch in branch delay slot PC=%08X", gHWS_pc);

		/*
		 * DisplayError("Branch in branch delay slot PC=%08X", gHWS_pc); ?
		 * return;
		 */
	}

	CompilingSlot = TRUE;

	/* this needs to be here, not below. */
	if( PreserveMap) {
		memcpy(TempConstMap, ConstMap, sizeof(ConstMap));
		memcpy(Tempx86reg, x86reg, sizeof(x86reg));
        memcpy(TempFlushedRegistersMap, FlushedRegistersMap, sizeof(FlushedRegistersMap));
    
		memcpy(Temp_SSE_ConstMap, SSE_ConstMap, sizeof(SSE_ConstMap));
		memcpy(Temp_SSE_fpureg, fpureg, sizeof(fpureg));
        memcpy(Temp_SSE_FlushedRegistersMap, SSE_FlushedRegistersMap, sizeof(SSE_FlushedRegistersMap));
	}

	/* Add by Rice. 2001-08-11 */
	if(currentromoptions.Use_Register_Caching == USEREGC_NO)
	{
		FlushAllRegisters(FLUSH_TO_MEMORY);
	}

	LOGGING_DYNA(LogDyna("** Compile Delay Slot\n", pc));
	{

		uint32	savedpc = gHWS_pc;

		gHWS_pc = pc;

		/* TRACE1("Branch Slot fetch code at PC=%08X", pc); */
        gHWS_code = DynaFetchInstruction2(gHWS_pc);
		gHWS_pc = savedpc;
	}

	op = (_u8) (gHWS_code >> 26);
    compilerstatus.BranchDelay = 1;
    dyna_instruction[op](&gHardwareState);
    compilerstatus.BranchDelay = 0;

	CompilingSlot = FALSE;
	InterruptsExperiment(JUMP_TYPE_DIRECT, targetpc, DoLink, LinkVal);

	if( PreserveMap) {
		memcpy(ConstMap, TempConstMap, sizeof(ConstMap));
		memcpy(x86reg, Tempx86reg, sizeof(x86reg));
        memcpy(FlushedRegistersMap, TempFlushedRegistersMap, sizeof(FlushedRegistersMap));

    
		memcpy(SSE_ConstMap, Temp_SSE_ConstMap, sizeof(SSE_ConstMap));
		memcpy(fpureg, Temp_SSE_fpureg,  sizeof(fpureg));
        memcpy(SSE_FlushedRegistersMap, Temp_SSE_FlushedRegistersMap, sizeof(SSE_FlushedRegistersMap));
    
    }
}

void Compile_Slot_And_InterruptsNewTest(_u32 pc, uint32 targetpc, uint32 DoLink, uint32 LinkVal, int PreserveMap)
{
	_u8 op;

	compilerstatus.KEEP_RECOMPILING = 0;

    if(CompilingSlot)
	{
		TRACE1("Warning, Branch in branch delay slot PC=%08X", gHWS_pc);

		/*
		 * DisplayError("Branch in branch delay slot PC=%08X", gHWS_pc); ?
		 * return;
		 */
	}

	CompilingSlot = TRUE;

	/* this needs to be here, not below. */
	if( PreserveMap) {
		memcpy(TempConstMap, ConstMap, sizeof(ConstMap));
		memcpy(Tempx86reg, x86reg, sizeof(x86reg));
        memcpy(TempFlushedRegistersMap, FlushedRegistersMap, sizeof(FlushedRegistersMap));

		memcpy(Temp_SSE_ConstMap, SSE_ConstMap, sizeof(SSE_ConstMap));
		memcpy(Temp_SSE_fpureg, fpureg, sizeof(fpureg));
        memcpy(Temp_SSE_FlushedRegistersMap, SSE_FlushedRegistersMap, sizeof(SSE_FlushedRegistersMap));

	}

	/* Add by Rice. 2001-08-11 */
	if(currentromoptions.Use_Register_Caching == USEREGC_NO)
	{
		FlushAllRegisters(FLUSH_TO_MEMORY);
	}

	LOGGING_DYNA(LogDyna("** Compile Delay Slot\n", pc));
	{

		uint32	savedpc = gHWS_pc;

		gHWS_pc = pc;

		/* TRACE1("Branch Slot fetch code at PC=%08X", pc); */
		gHWS_code = DynaFetchInstruction2(gHWS_pc);
		gHWS_pc = savedpc;
	}

	op = (_u8) (gHWS_code >> 26);
	compilerstatus.BranchDelay = 1;
    dyna_instruction[op](&gHardwareState);
    compilerstatus.BranchDelay = 0;

	CompilingSlot = FALSE;
	
        InterruptsNewTest(JUMP_TYPE_DIRECT, targetpc, DoLink, LinkVal);

	if( PreserveMap) {
		memcpy(ConstMap, TempConstMap, sizeof(ConstMap));
		memcpy(x86reg, Tempx86reg, sizeof(x86reg));
        memcpy(FlushedRegistersMap, TempFlushedRegistersMap, sizeof(FlushedRegistersMap));

		memcpy(SSE_ConstMap, Temp_SSE_ConstMap, sizeof(SSE_ConstMap));
		memcpy(fpureg, Temp_SSE_fpureg,  sizeof(fpureg));
        memcpy(SSE_FlushedRegistersMap, Temp_SSE_FlushedRegistersMap, sizeof(SSE_FlushedRegistersMap));


        compilerstatus.KEEP_RECOMPILING = 1;
	}
}


void Compile_Slot_And_Interrupts(_u32 pc, uint32 targetpc, uint32 DoLink, uint32 LinkVal, int PreserveMap)
{
	/*~~~*/
	_u8 op;
	/*~~~*/

	if(CompilingSlot)
	{
		TRACE1("Warning, Branch in branch delay slot PC=%08X", gHWS_pc);

		/*
		 * DisplayError("Branch in branch delay slot PC=%08X", gHWS_pc); ?
		 * return;
		 */
	}

	CompilingSlot = TRUE;

	/* this needs to be here, not below. */
	if( PreserveMap) {
		memcpy(TempConstMap, ConstMap, sizeof(ConstMap));
		memcpy(Tempx86reg, x86reg, sizeof(x86reg));
        memcpy(TempFlushedRegistersMap, FlushedRegistersMap, sizeof(FlushedRegistersMap));

		memcpy(Temp_SSE_ConstMap, SSE_ConstMap, sizeof(SSE_ConstMap));
		memcpy(Temp_SSE_fpureg, fpureg, sizeof(fpureg));
        memcpy(Temp_SSE_FlushedRegistersMap, SSE_FlushedRegistersMap, sizeof(SSE_FlushedRegistersMap));

	}

	/* Add by Rice. 2001-08-11 */
	if(currentromoptions.Use_Register_Caching == USEREGC_NO)
	{
		FlushAllRegisters(FLUSH_TO_MEMORY);
	}

	LOGGING_DYNA(LogDyna("** Compile Delay Slot\n", pc));
	{
		/*~~~~~~~~~~~~~~~~~~~~~~*/
		uint32	savedpc = gHWS_pc;
		/*~~~~~~~~~~~~~~~~~~~~~~*/

		gHWS_pc = pc;

		/* TRACE1("Branch Slot fetch code at PC=%08X", pc); */
		
        gHWS_code = DynaFetchInstruction2(gHWS_pc);
        gHWS_pc = savedpc;
	}

	op = (_u8) (gHWS_code >> 26);
	compilerstatus.BranchDelay = 1;
    dyna_instruction[op](&gHardwareState);
    compilerstatus.BranchDelay = 0;

	CompilingSlot = FALSE;
	Interrupts(JUMP_TYPE_DIRECT, targetpc, DoLink, LinkVal);

	if( PreserveMap) {
		memcpy(ConstMap, TempConstMap, sizeof(ConstMap));
        memcpy(x86reg, Tempx86reg, sizeof(x86reg));
        memcpy(FlushedRegistersMap, TempFlushedRegistersMap, sizeof(FlushedRegistersMap));

		memcpy(SSE_ConstMap, Temp_SSE_ConstMap, sizeof(SSE_ConstMap));
		memcpy(fpureg, Temp_SSE_fpureg,  sizeof(fpureg));
        memcpy(SSE_FlushedRegistersMap, Temp_SSE_FlushedRegistersMap, sizeof(SSE_FlushedRegistersMap));

    }
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Compile_Slot_Jump(_u32 pc)
{
	/*~~~*/
	_u8 op;
	/*~~~*/

	if(CompilingSlot)
	{
		DisplayError("Branch in branch delay slot PC=%08X", gHWS_pc);
		TRACE1("Branch in branch delay slot PC=%08X", gHWS_pc);

		/* return; */
	}

	CompilingSlot = TRUE;

	/* Add by Rice. 2001-08-11 */
	if(currentromoptions.Use_Register_Caching == USEREGC_NO)
        FlushAllRegisters(FLUSH_TO_MEMORY);

	LOGGING_DYNA(LogDyna("** Compile Delay Slot\n", pc));
	{
		/*~~~~~~~~~~~~~~~~~~~~~~*/
		uint32	savedpc = gHWS_pc;
		/*~~~~~~~~~~~~~~~~~~~~~~*/

		gHWS_pc = pc;

	    
        /* TRACE1("Branch Slot fetch code at PC=%08X", pc); */
		gHWS_code = DynaFetchInstruction2(gHWS_pc);
        
		gHWS_pc = savedpc;
	}

	op = (_u8) (gHWS_code >> 26);
	compilerstatus.BranchDelay = 1;
    dyna_instruction[op](&gHardwareState);
    compilerstatus.BranchDelay = 0;
	CompilingSlot = FALSE;
}

extern uint8	*Block;
uint32			BlockStartAddress;

void			UnwireMap(void);

/*
 =======================================================================================================================
 =======================================================================================================================
 */

extern unsigned int Dyna_Compile_Single_Block();
extern BOOL			IsBlockCompiled(uint32 pc);
extern void StoreImm_LO(int To_XMM_Or_Memory, int k, int i);

uint32				GetCompiledBlockPtr(uint32 pc);






int JIT_Dyna_Compile_Block(uint32 StubAddress, int targetpc)
{
	__int32	wTemp;
    uint32  wTemp8;
    long dwAddress;
    //char reer[200];
	//uint32 translatepc;
    unsigned int tmplCodePosition;
    unsigned char* tmpRecompCode;

    
    if (!IsBlockCompiled(gHWS_pc))
        dwAddress = Dyna_Compile_Block();
    else 
        dwAddress = GetCompiledBlockPtr(gHWS_pc);

    wTemp8 = (uint32)((__int32)dwAddress  - 1 - (__int32)StubAddress);
    wTemp =  (__int32)dwAddress - 4 - (__int32)StubAddress;
    
    tmplCodePosition = compilerstatus.lCodePosition;
    tmpRecompCode = &RecompCode[0];

    RecompCode = (unsigned char*)(StubAddress-4);
    compilerstatus.lCodePosition = 0;


    if  ( ((wTemp8 < 127) || (wTemp8 > 0xffffff81))
         )
       
    {
        CMP_RegWith0(Reg_EAX);
        Jcc(CC_G, wTemp8+1);
	}

	else
    {

        CMP_RegWith0(Reg_EAX);
        
        //Jcc_near. Remember not to compile with the x86.c function, because the offset is relative, not absolute.

        *(uint16*)(StubAddress-2) = 0x8F0F;
        *((__int32*)StubAddress) = wTemp;
        compilerstatus.lCodePosition += 6;
	}
    {
        int i = targetpc;
        
        StoreImm_LO(FLUSH_TO_MEMORY, __pc, i);
        TLB_TRANSLATE_PC(targetpc);
    }
    RET();

    compilerstatus.lCodePosition = tmplCodePosition;
    RecompCode = tmpRecompCode;
    
    return (uint32)dwAddress;
}

void InterruptsExperiment(uint32 JumpType, uint32 targetpc, uint32 DoLink, uint32 LinkVal)
{
	Interrupts(JumpType, targetpc, DoLink, LinkVal);
}

void InterruptsSafe(uint32 JumpType, uint32 targetpc, uint32 DoLink, uint32 LinkVal)
{
	uint32	viCounter;
    //int     StubAddr; //The retargetable stub for JIT linking
    //int     InjectStubAddrToParam;

    //viCounter is used if not using AutoCF
	if (emuoptions.AutoCF)
		viCounter = compilerstatus.cp0Counter;
	else
		viCounter = compilerstatus.cp0Counter * VICounterFactors[CounterFactor];


    compilerstatus.KEEP_RECOMPILING = FALSE;

	FlushAllRegisters(FLUSH_TO_MEMORY);

	/* BugFix: can't store the link val to memory until GPR[31] has been flushed! */
	if(DoLink)
	{
		MOV_ImmToMemory(1, ModRM_disp8_EBP, (((31-1) - 16)) << 3, (_s32) LinkVal);
		if(currentromoptions.Assume_32bit != ASSUME_32BIT_YES)
			MOV_ImmToMemory(1, ModRM_disp8_EBP, 4 + ((((31-1) - 16)) << 3), (_s32) (((__int32) LinkVal) >> 31));
	}

    if (JumpType == JUMP_TYPE_INDIRECT)
        TLB_TRANSLATE_PC_INDIRECT();
    else
    {
        int i = targetpc;
        StoreImm_LO(FLUSH_TO_MEMORY, __pc, i);
	 	TLB_TRANSLATE_PC(targetpc);
    }

    if (emuoptions.AutoCF)
    {
        MOV_MemoryToReg(Reg_EAX, ModRM_disp32, (uint32)&AutoCounterFactor);
        MOV_ImmToReg(Reg_ECX, viCounter);
        IMUL_EAXWithReg(Reg_ECX);
        MOV_MemoryToReg(Reg_EDX, ModRM_disp32, (uint32)&r.r_.countdown_counter);
        SUB_Reg1OfReg2(Reg_EAX, Reg_EDX);
        MOV_RegToMemory(1, Reg_EDX, ModRM_disp32, (uint32)&r.r_.countdown_counter);
        
    }
    else
    {
        MOV_MemoryToReg(Reg_EAX, ModRM_disp32, (uint32)&r.r_.countdown_counter);
        SUB_ImmFromReg(1, Reg_EAX, viCounter, 0);
        MOV_RegToMemory(1, Reg_EAX, ModRM_disp32, (uint32)&r.r_.countdown_counter);
    }

  
    RET();

	if(CompilingSlot)
		DisplayError("Ok, CompilingSlot is still TRUE when finishing compiling the block, PC=%08X", gHWS_pc);
}

void InterruptsNewTest(uint32 JumpType, uint32 targetpc, uint32 DoLink, uint32 LinkVal)
{
	uint32	viCounter;
    int     StubAddr; //The retargetable stub for JIT linking
    int     InjectStubAddrToParam;

    //viCounter is used if not using AutoCF
	if (emuoptions.AutoCF)
		viCounter = compilerstatus.cp0Counter;
	else
		viCounter = compilerstatus.cp0Counter * VICounterFactors[CounterFactor];

    compilerstatus.KEEP_RECOMPILING = FALSE;

	FlushAllRegisters(FLUSH_TO_MEMORY);

	/* BugFix: can't store the link val to memory until GPR[31] has been flushed! */
	if(DoLink)
	{
		MOV_ImmToMemory(1, ModRM_disp8_EBP, (((31-1) - 16)) << 3, (_s32) LinkVal);
		if(currentromoptions.Assume_32bit != ASSUME_32BIT_YES)
			MOV_ImmToMemory(1, ModRM_disp8_EBP, 4 + ((((31-1) - 16)) << 3), (_s32) (((__int32) LinkVal) >> 31));
	}

    if (emuoptions.AutoCF)
    {
        MOV_MemoryToReg(Reg_EAX, ModRM_disp32, (uint32)&AutoCounterFactor);
        MOV_ImmToReg(Reg_ECX, viCounter);
        IMUL_EAXWithReg(Reg_ECX);
        MOV_MemoryToReg(Reg_EDX, ModRM_disp32, (uint32)&r.r_.countdown_counter);
        SUB_Reg1OfReg2(Reg_EAX, Reg_EDX);
        MOV_RegToMemory(1, Reg_EDX, ModRM_disp32, (uint32)&r.r_.countdown_counter);
        MOV_Reg2ToReg1(Reg_EAX, Reg_EDX);
        
    }
    else
    {
        MOV_MemoryToReg(Reg_EAX, ModRM_disp32, (uint32)&r.r_.countdown_counter);
        SUB_ImmFromReg(1, Reg_EAX, viCounter, 0);
        MOV_RegToMemory(1, Reg_EAX, ModRM_disp32, (uint32)&r.r_.countdown_counter);
    }


    //Bottleneck
    if (JumpType == JUMP_TYPE_INDIRECT)
        TLB_TRANSLATE_PC_INDIRECT();

    else if (JumpType == JUMP_TYPE_DIRECT && currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES
        && !IsBooting && /*!DoLink && */(IsTargetPcInTheSame4KB(gHWS_pc, targetpc)) )
    {
        int i = targetpc;
        
        //TODO: Why can't line 3 precede line 1?

        //line 3
        //TODO: watch out for dnabufferoverruns!
        StubAddr = (int)(&dyna_RecompCode[compilerstatus.lCodePosition]+4);


        //line 1
        StoreImm_LO(FLUSH_TO_MEMORY, __pc, i);
        TLB_TRANSLATE_PC(targetpc);
        
       
        MOV_ImmToReg(Reg_ECX, 0xDEADBEEF);
        InjectStubAddrToParam = (int)(&dyna_RecompCode[compilerstatus.lCodePosition] - 4);
        MOV_ImmToReg(Reg_EDX, targetpc);
        CMP_RegWith0(Reg_EAX);
        /*StubAddr =*/ Jcc_Near(CC_G, (uint32)&JIT_Dyna_Compile_Block);
        *(__int32*)InjectStubAddrToParam = StubAddr;
    }
    else
    {
        int i = targetpc;
        StoreImm_LO(FLUSH_TO_MEMORY, __pc, i);
	 	TLB_TRANSLATE_PC(targetpc);
    }

    RET();

	if(CompilingSlot)
		DisplayError("Ok, CompilingSlot is still TRUE when finishing compiling the block, PC=%08X", gHWS_pc);

	compilerstatus.IsFirstFPUInstructionToTest = TRUE; //Fixes Kirby. Linked blocks.
}


void Interrupts(uint32 JumpType, uint32 targetpc, uint32 DoLink, uint32 LinkVal)
{
    InterruptsSafe( JumpType, targetpc, DoLink, LinkVal); return;
}

extern x86regtyp		xRD[1];
extern x86regtyp		xRS[1];
extern x86regtyp		xRT[1];





unsigned long			templCodePosition;
extern unsigned long	JumpTargets[100];
unsigned long			wPosition;
extern void SetRdRsRt64bit(OP_PARAMS);
extern void SwitchToOpcodePass(void);

void BranchOutEpilogue(int tempPC, int Likely)
{
                if ((!IsBooting) && (currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES)
                    
)
                {
                    gHWS_pc=tempPC;
                    compilerstatus.KEEP_RECOMPILING = 1;
                    if (!Likely)
                    {
                        uint32 op;
                        
                        gHWS_pc+=4;
                        
                        gHWS_code = DynaFetchInstruction2(gHWS_pc);
                        op = (_u8) (gHWS_code >> 26);
                    	
                        compilerstatus.BranchDelay = 1;
                        dyna_instruction[op](&gHardwareState);
                        compilerstatus.BranchDelay = 0;

                    }

                }
                else
                {
                    gHWS_pc=tempPC;
                    if (!Likely)
                    {
                        uint32 op;
                        
                        gHWS_pc+=4;
                        
                        gHWS_code = DynaFetchInstruction2(gHWS_pc);
                        op = (_u8) (gHWS_code >> 26);
                    	
                        compilerstatus.BranchDelay = 1;
                        dyna_instruction[op](&gHardwareState);
                        compilerstatus.BranchDelay = 0;
                        Interrupts(JUMP_TYPE_CONTINUE, tempPC + 8, LINK_NO, 0);

                    }
                    else
                        Interrupts(JUMP_TYPE_CONTINUE, tempPC + 4, LINK_NO, 0);
                }
}

void BranchOutEpilogueAndLink(int tempPC, int Likely, int LinkVal)
{
    
	//Never link these ops!!
	/*            
                if ((!IsBooting) && (currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES))
                {
                    gHWS_pc=tempPC;
                    compilerstatus.KEEP_RECOMPILING = 1;
                    if (!Likely)
                    {
                        uint32 op;
                        
                        gHWS_pc+=4;
                        
                        gHWS_code = DynaFetchInstruction2(gHWS_pc);
                        op = (_u8) (gHWS_code >> 26);
                    	
                        compilerstatus.BranchDelay = 1;
                        dyna_instruction[op](&gHardwareState);
                        compilerstatus.BranchDelay = 0;

                    }

                }
                else*/
                {
                    gHWS_pc=tempPC;
                    if (!Likely)
                    {
                        uint32 op;
                        
                        gHWS_pc+=4;
                        
                        gHWS_code = DynaFetchInstruction2(gHWS_pc);
                        op = (_u8) (gHWS_code >> 26);
                    	
                        compilerstatus.BranchDelay = 1;
                        dyna_instruction[op](&gHardwareState);
                        compilerstatus.BranchDelay = 0;
                        Interrupts(JUMP_TYPE_CONTINUE, tempPC + 8, LINK_YES, LinkVal);

                    }
                    else
                        Interrupts(JUMP_TYPE_CONTINUE, tempPC + 4, LINK_YES, LinkVal);
                }
}

void BNE(OP_PARAMS, int pC, int Likely);

void bne(OP_PARAMS)
{
	BNE(PASS_PARAMS, r.r_.pc, 0);
}
void bnel(OP_PARAMS)
{
	BNE(PASS_PARAMS, r.r_.pc+4, 1);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void BNE(OP_PARAMS, int pC, int Likely)
{
	_u32	aValue;
	int		tempPC = pC;
	int		IsNear = SAFE_NEAR_ONLY;			/* If short, range= -128 to +127 */
	int		Is32bit = 0;
	int		tempRSIs32bit = 0;
	int		tempRTIs32bit = 0;
	x86regtyp tempxRT[1];
	x86regtyp tempxRS[1];
	int		tempCounter;

	SetRdRsRt64bit(PASS_PARAMS);

	compilerstatus.cp0Counter += 1;
	tempCounter = compilerstatus.cp0Counter;

	_OPCODE_DEBUG_BRANCH_(r4300i_bne);

	aValue = (r.r_.pc + 4 + (__I << 2));

	tempRSIs32bit = CheckIs32Bit(xRS->mips_reg);
	tempRTIs32bit = CheckIs32Bit(xRT->mips_reg);


    Is32bit = ((currentromoptions.Assume_32bit == ASSUME_32BIT_YES) || (tempRSIs32bit&tempRTIs32bit));

    if (Is32bit)
    {
        xRT->Is32bit = 1;
        xRS->Is32bit = 1;
    }

    if(xRS->mips_reg == xRT->mips_reg)
	{
		// false
        BranchOutEpilogue(tempPC, Likely);
		return;
	}
	else if( ConstMap[xRS->mips_reg].IsMapped == 1) {
		if( ConstMap[xRT->mips_reg].IsMapped == 1) {
			if( ConstMap[xRS->mips_reg].value == ConstMap[xRT->mips_reg].value) {
				// false
				
                BranchOutEpilogue(tempPC, Likely);

            } else {
				// true
				SPEED_HACK
				r.r_.pc += 4;
//				Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);
    			Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 
                ((!IsBooting) && (currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES)) ? 1:0);


			}

			return;
		} else {
            
            MapRT;
			memcpy(tempxRT, xRT, sizeof(xRT));
			//Preserve RT in case we need to _Redo

			CMP_RegWithImm(xRT->x86reg, ConstMap[xRS->mips_reg].value);
			if(!Is32bit) {
				Jcc_auto(CC_NE, 90);					/* jmp true */
				CMP_RegWithImm(xRT->HiWordLoc, (_int32)(ConstMap[xRS->mips_reg].value)>>31);
			}

			goto _Redo;
		}
	}

	else if(ConstMap[xRT->mips_reg].IsMapped == 1)
	{
		MapRS;
		//Preserve RS in case we need to _Redo
		memcpy(tempxRS, xRS, sizeof(xRS));

		CMP_RegWithImm(xRS->x86reg, ConstMap[xRT->mips_reg].value);
		if(!Is32bit)
		{
			Jcc_auto(CC_NE, 90);					/* jmp true */
			CMP_RegWithImm(xRS->HiWordLoc, (_int32)(ConstMap[xRT->mips_reg].value)>>31);
		}

		goto _Redo;
	}


	if((tempRSIs32bit && tempRTIs32bit) || (currentromoptions.Assume_32bit == ASSUME_32BIT_YES))
	{
		Is32bit = 1;
		xRS->Is32bit = 1;
		xRT->Is32bit = 1;
	}

	MapRT;
	MapRS;
	//Preserve xRT, xRS in case we need to _Redo
	memcpy(tempxRT, xRT, sizeof(xRT));
	memcpy(tempxRS, xRS, sizeof(xRS));

	CMP_Reg1WithReg2(xRS->x86reg, xRT->x86reg);

	if(!Is32bit)
	{
		Jcc_auto(CC_NE, 90);					/* jmp true */
		CMP_Reg1WithReg2(xRS->HiWordLoc, xRT->HiWordLoc);
	}

_Redo:
	compilerstatus.cp0Counter = tempCounter;

	templCodePosition = compilerstatus.lCodePosition;
	if(IsNear == 1) Jcc_Near_auto(CC_E, 91);	/* jmp false */
	else
		Jcc_auto(CC_E, 91);						/* jmp false */

	/* true */
	if(!Is32bit)
		SetTarget(90);

	SPEED_HACK
	r.r_.pc += 4;


    Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);

	/* false */
	wPosition = compilerstatus.lCodePosition - JumpTargets[91];
	if((wPosition > 120) && (IsNear == 0))
	{
		compilerstatus.lCodePosition = templCodePosition;
		r.r_.pc -= 4;

		/* Rewrite the code as a near jump. Short jump won't cut it. */
		IsNear = 1;
		memcpy(xRT, tempxRT, sizeof(xRT));
		memcpy(xRS, tempxRS, sizeof(xRS));
		goto _Redo;
	}

	if(IsNear == 0)
	{
		SetTarget(91);
	}
	else
	{
		SetNearTarget(91);
	}

	/* end of compiled block */

    BranchOutEpilogue(tempPC, Likely);
}





void BEQ(OP_PARAMS, int pC, int Likely);

void beq(OP_PARAMS)
{
	BEQ(PASS_PARAMS, r.r_.pc, 0);
}
void beql(OP_PARAMS)
{
	BEQ(PASS_PARAMS, r.r_.pc+4, 1);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void BEQ(OP_PARAMS, int pC, int Likely)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	_u32				aValue;
	int					IsNear = SAFE_NEAR_ONLY;
	int					tempPC = pC;
	int					Use32bit = 0;
	int					arg=0;
	x86regtyp			tempxRS[1], 
						tempxRT[1];
	int					tempCounter;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	SetRdRsRt64bit(PASS_PARAMS);
	_OPCODE_DEBUG_BRANCH_(r4300i_beq);

    compilerstatus.cp0Counter += 1;
	tempCounter = compilerstatus.cp0Counter;

	aValue = (r.r_.pc + 4 + (__I << 2));

	if((CheckIs32Bit(__RT) && CheckIs32Bit(__RS)) || (currentromoptions.Assume_32bit == ASSUME_32BIT_YES))
	{
		xRS->Is32bit = 1;
		xRT->Is32bit = 1;
		Use32bit = 1;
	}

	if(xRS->mips_reg == xRT->mips_reg)
	{
		
//Not good for cp1 unusable in delay slot
//        if ((aValue == r.r_.pc + 8) && (IsTargetPcInTheSame4KB(gHWS_pc, aValue)))
//      {
//            return;
//        }
        
        /* true */
		SPEED_HACK 
        r.r_.pc += 4;
        
        //Needed for Conker. But Why ?
//        Compile_Slot_And_Interrupts(r.r_.pc, aValue, LINK_NO, 0, 0);
    	Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 
            ((!IsBooting) && (currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES))? 1:0);

		return;
	}
	else if((ConstMap[xRS->mips_reg].IsMapped == 1) && (ConstMap[xRT->mips_reg].IsMapped == 1))
	{
		if(ConstMap[xRS->mips_reg].value == ConstMap[xRT->mips_reg].value)
		{
			/* true */
			SPEED_HACK 
            r.r_.pc += 4;
//			Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);
  			Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 
                ((!IsBooting) && (currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES))? 1:0);

		}
		else
		{
                BranchOutEpilogue(tempPC, Likely);
		}

		return;
	}
	else if(ConstMap[xRS->mips_reg].IsMapped == 1)
	{
		MapRT;
		//Preserve xRT & xRS for _Redo
		memcpy(tempxRT, xRT, sizeof(xRT));
		memcpy(tempxRS, xRS, sizeof(xRS));
		arg=1;

		CMP_RegWithImm(xRT->x86reg, ConstMap[xRS->mips_reg].value);
_Redo1:	templCodePosition = compilerstatus.lCodePosition;
		compilerstatus.cp0Counter = tempCounter;

		if (IsNear == 1) Jcc_Near_auto(CC_NE, 91);		/* jmp false */
		else				  Jcc_auto(CC_NE, 91);		/* jmp false */

		if(!Use32bit)
		{
			CMP_RegWithImm(xRT->HiWordLoc, ((__int32) ConstMap[xRS->mips_reg].value >> 31));
			if (IsNear == 1) Jcc_Near_auto(CC_NE, 93);	/* jmp false */
			else				  Jcc_auto(CC_NE, 93);	/* jmp false */
		}
	}
	else if(ConstMap[xRT->mips_reg].IsMapped == 1)
	{
		MapRS;
		//Preserve xRS & xRT for _Redo
		memcpy(tempxRS, xRS, sizeof(xRS));
		memcpy(tempxRT, xRT, sizeof(xRT));
		arg=2;

		CMP_RegWithImm(xRS->x86reg, ConstMap[xRT->mips_reg].value);
_Redo2: templCodePosition = compilerstatus.lCodePosition;
		compilerstatus.cp0Counter = tempCounter;

		if (IsNear == 1) Jcc_Near_auto(CC_NE, 91);		/* jmp false */
		else				  Jcc_auto(CC_NE, 91);		/* jmp false */

		if(!Use32bit)
		{
			CMP_RegWithImm(xRS->HiWordLoc, ((__int32) ConstMap[xRT->mips_reg].value >> 31));
			if (IsNear == 1) Jcc_Near_auto(CC_NE, 93);	/* jmp false */
			else				  Jcc_auto(CC_NE, 93);	/* jmp false */
		}
	}
	else if(xRS->mips_reg != xRT->mips_reg)
	{
		MapRT;
        MapRS;
		//Preserve xRT & xRS for _Redo
		memcpy(tempxRT, xRT, sizeof(xRT));
		memcpy(tempxRS, xRS, sizeof(xRS));
		arg=3;
		CMP_Reg1WithReg2(xRS->x86reg, xRT->x86reg);
_Redo3: templCodePosition = compilerstatus.lCodePosition;
		compilerstatus.cp0Counter = tempCounter;

		if (IsNear == 1) Jcc_Near_auto(CC_NE, 91);		/* jmp false */
		else				  Jcc_auto(CC_NE, 91);		/* jmp false */

		if(!Use32bit)
		{
			CMP_Reg1WithReg2(xRS->HiWordLoc, xRT->HiWordLoc);
			if (IsNear == 1) Jcc_Near_auto(CC_NE, 93);	/* jmp false */
			else				  Jcc_auto(CC_NE, 93);	/* jmp false */
		}
	}

	/* true */
	SPEED_HACK
	r.r_.pc += 4;
	
    
    
    Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);



	/* false */
	wPosition = compilerstatus.lCodePosition - JumpTargets[91];
	if((wPosition > 120) && (IsNear == 0))
	{
		compilerstatus.lCodePosition = templCodePosition;
		r.r_.pc -= 4;

		/* Rewrite the code as a near jump. Short jump won't cut it. */
		IsNear = 1;
		//Restore xRT & xRS for _Redo
		memcpy(xRT, tempxRT, sizeof(xRT));
		memcpy(xRS, tempxRS, sizeof(xRS));
		switch (arg)
		{
			case 1: goto _Redo1; break;
			case 2: goto _Redo2; break;
			case 3: goto _Redo3; break;
		}
	}
	
	if(IsNear == 0)
	{
		SetTarget(91);
		if(!Use32bit) SetTarget(93);
	}
	else
	{
		SetNearTarget(91);
		if(!Use32bit) SetNearTarget(93);
	}

    BranchOutEpilogue(tempPC, Likely);

}

/*
 =======================================================================================================================
 =======================================================================================================================
 */

void BLEZ(OP_PARAMS, int pC, int Likely);

void blez(OP_PARAMS)
{
	BLEZ(PASS_PARAMS, r.r_.pc, 0);
}
void blezl(OP_PARAMS)
{
	BLEZ(PASS_PARAMS, r.r_.pc+4, 1);
}

void BLEZ(OP_PARAMS, int pC, int Likely)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	_u32	aValue;
	int		tempPC = pC;
	int		IsNear = SAFE_NEAR_ONLY;
	int		tempRSIs32bit = 0;
	int		Use32bit = 0;
	int		templCodePosition32;
	int		templCodePosition64;
	x86regtyp tempxRS[1];
	int		tempCounter;
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	compilerstatus.cp0Counter += 1;
	tempCounter = compilerstatus.cp0Counter;

    SetRdRsRt64bit(PASS_PARAMS);
	_OPCODE_DEBUG_BRANCH_(r4300i_blez);

	tempRSIs32bit = CheckIs32Bit(xRS->mips_reg);

	if((tempRSIs32bit) || (currentromoptions.Assume_32bit == ASSUME_32BIT_YES))
	{
		Use32bit = 1;
		xRS->Is32bit = 1;
		xRT->Is32bit = 1;
	}

	aValue = (r.r_.pc + 4 + (__I << 2));

	if(ConstMap[xRS->mips_reg].IsMapped == 1)
	{
		if((_int32) ConstMap[xRS->mips_reg].value <= 0)
		{
			/* true */
			SPEED_HACK 
            r.r_.pc += 4;
//			Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);
   			Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 
                ((!IsBooting) && (currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES))? 1:0);

		}
		else
		{
			/* false */
			BranchOutEpilogue(tempPC, Likely);
		}

		return;
	}

	MapRS;
	//Preserve xRS _Redo
	memcpy(tempxRS, xRS, sizeof(xRS));

_Redo64:
	compilerstatus.cp0Counter = tempCounter;
	if(!Use32bit)
	{
		templCodePosition64 = compilerstatus.lCodePosition;
		CMP_RegWith0(xRS->HiWordLoc);

		if(IsNear == 1) Jcc_Near_auto(CC_G, 91);	/* jmp false */
		else
			Jcc_auto(CC_G, 91);						/* jmp false */
		Jcc_auto(CC_L, 90); /* jmp true */

		CMP_RegWith0(xRS->x86reg);
		if(IsNear == 1) Jcc_Near_auto(CC_A, 93);	/* jmp false */
		else
			Jcc_auto(CC_A, 93);						/* jmp false */

		/* true */
		SetTarget(90);
	}
	else
	{
		CMP_RegWith0(xRS->x86reg);
		templCodePosition32 = compilerstatus.lCodePosition;
_Redo32:
		compilerstatus.cp0Counter = tempCounter;
		if(IsNear == 1) Jcc_Near_auto(CC_G, 91);	/* jmp false */
		else
			Jcc_auto(CC_G, 91);						/* jmp false */
	}

	SPEED_HACK //why was this commented?
	r.r_.pc += 4;

    
    Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);


	wPosition = compilerstatus.lCodePosition - JumpTargets[91];
	if((wPosition > 120) && (IsNear == 0))
	{
		r.r_.pc -= 4;

		/* Rewrite the code as a near jump. Short jump won't cut it. */
		IsNear = 1;
		if(!Use32bit)
		{
			compilerstatus.lCodePosition = templCodePosition64;
			//Restore xRS for _Redo
			memcpy(xRS, tempxRS, sizeof(xRS));
			goto _Redo64;
		}
		else
		{
			compilerstatus.lCodePosition = templCodePosition32;
			//Restore xRS for _Redo
			memcpy(xRS, tempxRS, sizeof(xRS));
			goto _Redo32;
		}
	}
	else
	{
		/* false */
		if(IsNear == 0)
		{
			SetTarget(91);
			if(!Use32bit) SetTarget(93);
		}
		else
		{
			SetNearTarget(91);
			if(!Use32bit) SetNearTarget(93);
		}
	}

    BranchOutEpilogue(tempPC, Likely);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */

void BGTZ(OP_PARAMS, int pC, int Likely);

void bgtz(OP_PARAMS)
{
	BGTZ(PASS_PARAMS, r.r_.pc, 0);
}
void bgtzl(OP_PARAMS)
{
	BGTZ(PASS_PARAMS, r.r_.pc+4, 1);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void BGTZ(OP_PARAMS, int pC, int Likely)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	_u32	aValue;
	int		tempPC = pC;
	int		IsNear = SAFE_NEAR_ONLY;
	int		tempRSIs32bit = 0;
	int		Use32bit = 0;
	int		templCodePosition32;
	int		templCodePosition64;
	x86regtyp	tempxRS[1];
	int		tempCounter;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	compilerstatus.cp0Counter += 1;
	tempCounter = compilerstatus.cp0Counter;

	SetRdRsRt64bit(PASS_PARAMS);
	_OPCODE_DEBUG_BRANCH_(r4300i_bgtz);

	tempRSIs32bit = CheckIs32Bit(xRS->mips_reg);
	if((tempRSIs32bit) || (currentromoptions.Assume_32bit == ASSUME_32BIT_YES))
	{
		Use32bit = 1;
		xRS->Is32bit = 1;
		xRT->Is32bit = 1;
	}

	aValue = (r.r_.pc + 4 + (__I << 2));

	if(ConstMap[xRS->mips_reg].IsMapped == 1)
	{
		if((_int32) ConstMap[xRS->mips_reg].value > 0)
		{
			/* true */
			SPEED_HACK 
            r.r_.pc += 4;
//			Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);
   			Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 
                ((!IsBooting) && (currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES))? 1:0);

		}
		else
		{
            /* false */
			BranchOutEpilogue(tempPC, Likely);
		}

		return;
	}

	MapRS;
	//Preserve xRS for _Redo
	memcpy(tempxRS, xRS, sizeof(xRS));

_Redo64:
	compilerstatus.cp0Counter = tempCounter;
	if(!Use32bit)
	{
		templCodePosition64 = compilerstatus.lCodePosition;
		CMP_RegWith0(xRS->HiWordLoc);

		if(IsNear == 1) Jcc_Near_auto(CC_L, 91);	/* jmp false */
		else
			Jcc_auto(CC_L, 91);						/* jmp false */
		Jcc_auto(CC_G, 90); /* jmp true */
		CMP_RegWith0(xRS->x86reg);
		if(IsNear == 1) Jcc_Near_auto(CC_BE, 93);	/* jmp false2 */
		else
			Jcc_auto(CC_BE, 93);					/* jmp false2 */
	}
	else
	{
		CMP_RegWith0(xRS->x86reg);
_Redo32:
		compilerstatus.cp0Counter = tempCounter;
		templCodePosition32 = compilerstatus.lCodePosition;
		if(IsNear == 1) Jcc_Near_auto(CC_LE, 91);	/* jmp false2 */
		else
			Jcc_auto(CC_LE, 91);					/* jmp false2 */
	}

	if(!Use32bit) SetTarget(90);

	SPEED_HACK 
    r.r_.pc += 4;
	Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);

	wPosition = compilerstatus.lCodePosition - JumpTargets[91];
	if((wPosition > 120) && (IsNear == 0))
	{
		r.r_.pc -= 4;

		/* Rewrite the code as a near jump. Short jump won't cut it. */
		IsNear = 1;
		if(!Use32bit)
		{
			compilerstatus.lCodePosition = templCodePosition64;
			//Restore xRS for _Redo
			memcpy(xRS, tempxRS, sizeof(xRS));
			goto _Redo64;
		}
		else
		{
			compilerstatus.lCodePosition = templCodePosition32;
			//Restore xRS for _Redo
			memcpy(xRS, tempxRS, sizeof(xRS));
			goto _Redo32;
		}
	}

	/* false */
	if(IsNear == 0)
	{
		SetTarget(91);
		if(!Use32bit) SetTarget(93);
	}
	else
	{
		SetNearTarget(91);
		if(!Use32bit) SetNearTarget(93);
	}

    BranchOutEpilogue(tempPC, Likely);
}

void _BLTZ(OP_PARAMS, int pC, int Likely);

void _bltz(OP_PARAMS)
{
  //  MessageBox(0, "dyna : bltz", "", 0);
    _BLTZ(PASS_PARAMS, r.r_.pc, 0);
}
void _bltzl(OP_PARAMS)
{
	_BLTZ(PASS_PARAMS, r.r_.pc+4, 1);
}



/*
 =======================================================================================================================
 =======================================================================================================================
 */
void _BLTZ(OP_PARAMS, int pC, int Likely)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	_u32	aValue;
	int		tempPC = pC;
	int		IsNear = SAFE_NEAR_ONLY;
	int		tempRSIs32bit = 0;
	int		Use32bit = 0;
	int		templCodePosition;
	x86regtyp tempxRS[1];
	int		tempCounter;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	compilerstatus.cp0Counter += 1;
	tempCounter = compilerstatus.cp0Counter;

	SetRdRsRt64bit(PASS_PARAMS);
	_OPCODE_DEBUG_BRANCH_(r4300i_bltz);

	tempRSIs32bit = CheckIs32Bit(xRS->mips_reg);
	if((tempRSIs32bit) || (currentromoptions.Assume_32bit == ASSUME_32BIT_YES))
	{
		Use32bit = 1;
		xRS->Is32bit = 1;
	}

	aValue = (r.r_.pc + 4 + (__I << 2));

	if(ConstMap[xRS->mips_reg].IsMapped == 1)
	{
		if((_int32) ConstMap[xRS->mips_reg].value < 0)
		{
			/* true */
			SPEED_HACK
			r.r_.pc += 4;
//			Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);
  			Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 
                ((!IsBooting) && (currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES))? 1:0);

		}
		else
		{
			/* false */

            BranchOutEpilogue(tempPC, Likely);
        }

		return;
	}

	MapRS;
	//Preserve xRS for _Redo
	memcpy(tempxRS, xRS, sizeof(xRS));

	if(!Use32bit)
		CMP_RegWith0(xRS->HiWordLoc);
	else
		CMP_RegWith0(xRS->x86reg);
        
_Redo:
	templCodePosition = compilerstatus.lCodePosition;
	compilerstatus.cp0Counter = tempCounter;
	if(IsNear == 1) Jcc_Near_auto(CC_GE, 91);	/* jmp false */
	else
		Jcc_auto(CC_GE, 91);

	/* delay Slot */
	SPEED_HACK 
    r.r_.pc += 4;
	Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);
	wPosition = compilerstatus.lCodePosition - JumpTargets[91];
	if((wPosition > 120) && (IsNear == 0))
	{
		r.r_.pc -= 4;

		/* Rewrite the code as a near jump. Short jump won't cut it. */
		IsNear = 1;
		compilerstatus.lCodePosition = templCodePosition;
		//Restore xRS for _Redo
		memcpy(xRS, tempxRS, sizeof(xRS));
		goto _Redo;
	}

	if(IsNear == 1)
		SetNearTarget(91);
	else
		SetTarget(91);
	
    
    BranchOutEpilogue(tempPC, Likely);
}


void _BGEZ(OP_PARAMS, int pC, int Likely);

void _bgez(OP_PARAMS)
{
	_BGEZ(PASS_PARAMS, r.r_.pc, 0);
}
void _bgezl(OP_PARAMS)
{
	_BGEZ(PASS_PARAMS, r.r_.pc+4, 1);
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void _BGEZ(OP_PARAMS, int pC, int Likely)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	_u32	aValue;
	int		tempPC = pC;
	int		IsNear = SAFE_NEAR_ONLY;
	int		tempRSIs32bit = 0;
	int		Use32bit = 0;
	int		templCodePosition32;
	int		templCodePosition64;
	x86regtyp tempxRS[1];
	int		tempCounter;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	compilerstatus.cp0Counter += 1;
	tempCounter = compilerstatus.cp0Counter;

	SetRdRsRt64bit(PASS_PARAMS);
	_OPCODE_DEBUG_BRANCH_(r4300i_bgez);

	tempRSIs32bit = CheckIs32Bit(xRS->mips_reg);
	if((tempRSIs32bit) || (currentromoptions.Assume_32bit == ASSUME_32BIT_YES))
	{
		Use32bit = 1;
		xRS->Is32bit = 1;
	}

	aValue = (r.r_.pc + 4 + (__I << 2));

	if(ConstMap[xRS->mips_reg].IsMapped == 1)
	{
		if((_int32) ConstMap[xRS->mips_reg].value >= 0)
		{

            // true
			SPEED_HACK 
			r.r_.pc += 4;
//			Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);
   			Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 
                ((!IsBooting) && (currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES))? 1:0);

		}
		else
		{
			// false
            BranchOutEpilogue(tempPC, Likely);		
        }

		return;
	}


	MapRS;
	//Preserve xRS for _Redo
	memcpy(tempxRS, xRS, sizeof(xRS));


	if(!Use32bit)
	{
		
		CMP_RegWith0(xRS->HiWordLoc);

		templCodePosition64 = compilerstatus.lCodePosition;	
_Redo64:
		compilerstatus.cp0Counter = tempCounter;
		if(IsNear == 1) Jcc_Near_auto(CC_L, 91);
		else			     Jcc_auto(CC_L, 91);

		Jcc_auto(CC_G, 90);
		CMP_RegWith0(xRS->x86reg);

		if(IsNear == 1)	Jcc_Near_auto(CC_B, 93);
		else                 Jcc_auto(CC_B, 93);

		SetTarget(90);
	}
	else
	{
		CMP_RegWith0(xRS->x86reg);
_Redo32:
		compilerstatus.cp0Counter = tempCounter;
		templCodePosition32 = compilerstatus.lCodePosition;
		if(IsNear == 1) Jcc_Near_auto(CC_L, 91);	/* jmp false2 */
		else			     Jcc_auto(CC_L, 91);	/* jmp false2 */
	}

	SPEED_HACK 
	r.r_.pc += 4;
	Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);

	wPosition = compilerstatus.lCodePosition - JumpTargets[91];
	if((wPosition > 120) && (IsNear == 0))
	{
		r.r_.pc -= 4;

		/* Rewrite the code as a near jump. Short jump won't cut it. */
		IsNear = 1;
		if(!Use32bit)
		{
			compilerstatus.lCodePosition = templCodePosition64;
			memcpy(xRS, tempxRS, sizeof(xRS));
			goto _Redo64;
		}
		else
		{
			compilerstatus.lCodePosition = templCodePosition32;
			memcpy(xRS, tempxRS, sizeof(xRS));
			goto _Redo32;
		}
	}

	if(IsNear == 0) {
		SetTarget(91);
		if(!Use32bit) SetTarget(93);
	} else {
		SetNearTarget(91);
		if(!Use32bit) SetNearTarget(93);
	}

    BranchOutEpilogue(tempPC, Likely);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void _bgezal(OP_PARAMS)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	aValue;
	_int32	LinkVal = r.r_.pc + 8;
	int		tempPC = r.r_.pc;
	int		tempRSIs32bit = 0;
	int		tempRTIs32bit = 0;
	int		Use32bit = 0;
	int		tempCounter;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//	MessageBox(0, "Bgezal", "", 0);
    compilerstatus.cp0Counter += 1;
	tempCounter = compilerstatus.cp0Counter;

	SetRdRsRt64bit(PASS_PARAMS);
	_OPCODE_DEBUG_BRANCH_(r4300i_bgezal);

	tempRSIs32bit = CheckIs32Bit(xRS->mips_reg);
	tempRTIs32bit = CheckIs32Bit(xRT->mips_reg);

	if((tempRSIs32bit && tempRTIs32bit) || (currentromoptions.Assume_32bit == ASSUME_32BIT_YES))
	{
		Use32bit = 1;
		xRS->Is32bit = 1;
		xRT->Is32bit = 1;
	}

	aValue = (r.r_.pc + 4 + (__I << 2));

	if(ConstMap[xRS->mips_reg].IsMapped == 1)
	{
		if((_int32) ConstMap[xRS->mips_reg].value >= 0)
		{
			/* true */
			SPEED_HACK
			r.r_.pc += 4;
			Compile_Slot_And_Interrupts(r.r_.pc, aValue, LINK_YES, LinkVal, 0);
		}
		else
		{
			/* false */
			BranchOutEpilogueAndLink(tempPC, LINK_YES, LinkVal);
		}

		return;
	}

	MapRS;
	if(!Use32bit)
	{
		CMP_RegWith0(xRS->HiWordLoc);
		Jcc_Near_auto(CC_L, 91);
		Jcc_auto(CC_G, 90);

		CMP_RegWith0(xRS->x86reg);
		Jcc_Near_auto(CC_B, 93);
	}
	else
	{
		CMP_RegWith0(xRS->x86reg);
		Jcc_Near_auto(CC_L, 91);	/* jmp false */
	}

	/* delay Slot */
	if(!Use32bit) SetTarget(90);
	SPEED_HACK 
    r.r_.pc += 4;
	Compile_Slot_And_Interrupts(r.r_.pc, aValue, LINK_YES, LinkVal, 1);

	SetNearTarget(91);
	if(!Use32bit) SetNearTarget(93);

    BranchOutEpilogueAndLink(tempPC, LINK_YES, LinkVal);

}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void _bgezall(OP_PARAMS)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	aValue;
	_int32	LinkVal = r.r_.pc + 8;
	int		tempPC = r.r_.pc + 4;
	int		tempRSIs32bit = 0;
	int		tempRTIs32bit = 0;
	int		Use32bit = 0;
	int		tempCounter;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    compilerstatus.cp0Counter += 1;
	tempCounter = compilerstatus.cp0Counter;

	SetRdRsRt64bit(PASS_PARAMS);
	_OPCODE_DEBUG_BRANCH_(r4300i_bgezall);

	tempRSIs32bit = CheckIs32Bit(xRS->mips_reg);
	tempRTIs32bit = CheckIs32Bit(xRT->mips_reg);

	if((tempRSIs32bit && tempRTIs32bit) || (currentromoptions.Assume_32bit == ASSUME_32BIT_YES))
	{
		Use32bit = 1;
		xRS->Is32bit = 1;
		xRT->Is32bit = 1;
	}

	aValue = (r.r_.pc + 4 + (__I << 2));

	MapRS;
	if(!Use32bit)
	{
		CMP_RegWith0(xRS->HiWordLoc);
		Jcc_Near_auto(CC_L, 91);
		Jcc_auto(CC_G, 90);

		CMP_RegWith0(xRS->x86reg);
		Jcc_Near_auto(CC_B, 93);
	}
	else
	{
		CMP_RegWith0(xRS->x86reg);
		Jcc_Near_auto(CC_L, 91);	/* jmp false */
	}

	/* delay Slot */
	if(!Use32bit) SetTarget(90);
	SPEED_HACK r.r_.pc += 4;
	Compile_Slot_And_Interrupts(r.r_.pc, aValue, LINK_YES, LinkVal, 1);

	SetNearTarget(91);
	if(!Use32bit) SetNearTarget(93);
	
    
     BranchOutEpilogueAndLink(tempPC, LINK_YES, LinkVal);

}

/*
 =======================================================================================================================
    I don't know any games that use bltzal, bltzall? If I find any, I'll optimize these ops. 
    Otherwise, I will keep them the way they are now, so that I don't risk breaking the games.
 =======================================================================================================================
 */
void _bltzal(OP_PARAMS)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	aValue;
	int		tempPC = r.r_.pc;
	_int32	LinkVal = r.r_.pc + 8;
	int		tempCounter;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    compilerstatus.cp0Counter += 1;
	tempCounter = compilerstatus.cp0Counter;

	SetRdRsRt64bit(PASS_PARAMS);
	_OPCODE_DEBUG_BRANCH_(r4300i_bltzal);

	aValue = (r.r_.pc + 4 + (__I << 2));

	MapRS;
	CMP_RegWith0(xRS->HiWordLoc);
	Jcc_Near_auto(CC_GE, 91);	/* jmp false */

	/* delay Slot */
	SPEED_HACK 
    r.r_.pc += 4;
	Compile_Slot_And_Interrupts(r.r_.pc, aValue, LINK_YES, LinkVal, 1);

	SetNearTarget(91);

    BranchOutEpilogueAndLink(tempPC, LINK_YES, LinkVal);

}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void _bltzall(OP_PARAMS)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	aValue;
	int		tempPC = r.r_.pc + 4;
	_int32	LinkVal = r.r_.pc + 8;
	int		tempCounter;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    compilerstatus.cp0Counter += 1;
	tempCounter = compilerstatus.cp0Counter;

	SetRdRsRt64bit(PASS_PARAMS);
	_OPCODE_DEBUG_BRANCH_(r4300i_bltzall);

	aValue = (r.r_.pc + 4 + (__I << 2));

	MapRS;
	CMP_RegWith0(xRS->HiWordLoc);
	Jcc_Near_auto(CC_GE, 91);	/* jmp false */

	/* delay Slot */
	SPEED_HACK 
    r.r_.pc += 4;
	Compile_Slot_And_Interrupts(r.r_.pc, aValue, LINK_YES, LinkVal, 1);

	SetNearTarget(91);

    BranchOutEpilogueAndLink(tempPC, LINK_YES, LinkVal);

}

/* For JAL finder */
extern void LogSomething(void);

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void jal(OP_PARAMS)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	aValue;
	_int32	LinkVal = r.r_.pc + 8;
	int i;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	compilerstatus.cp0Counter += 1;
	_OPCODE_DEBUG_BRANCH_(r4300i_jal)
    aValue = ((r.r_.pc & 0xf0000000) | (____T << 2));

	r.r_.pc += 4;
	compilerstatus.FlagJAL = TRUE;

    Compile_Slot_Jump(r.r_.pc);

	i = aValue;
	StoreImm_LO(FLUSH_TO_MEMORY, __pc, i);
	//MOV_ImmToMemory(1, ModRM_disp32, (unsigned long) &r.r_.pc, aValue);

	/* end of compiled block */
	compilerstatus.KEEP_RECOMPILING = FALSE;

	InterruptsNewTest(JUMP_TYPE_DIRECT, aValue, LINK_YES, LinkVal);
    
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void j(OP_PARAMS)
{
	/*~~~~~~~~~~~*/
	_u32	aValue;
	int i;
	/*~~~~~~~~~~~*/

	compilerstatus.cp0Counter += 1;
	_OPCODE_DEBUG_BRANCH_(r4300i_j);

	aValue = ((r.r_.pc & 0xf0000000) | (____T << 2));

//Not good for cp1 unusable exceptions in delay slot 
//        if ((aValue == r.r_.pc + 8) && (IsTargetPcInTheSame4KB(gHWS_pc, aValue)))
//      {
//            return;
//        }


	J_SPEED_HACK
    r.r_.pc += 4;
	
    Compile_Slot_Jump(r.r_.pc);
	
	
	i = aValue;
	StoreImm_LO(FLUSH_TO_MEMORY, __pc, i);
	//MOV_ImmToMemory(1, ModRM_disp32, (unsigned long) &r.r_.pc, aValue);

	/* end of compiled block */
	compilerstatus.KEEP_RECOMPILING = FALSE;
	FlushAllRegisters(FLUSH_TO_MEMORY);
	
    InterruptsSafe(JUMP_TYPE_DIRECT, aValue, LINK_NO, 0);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */

extern void			FlushOneConstant(int TO_XMM_Or_Memory, int k);
void jr(OP_PARAMS)
{
   
    SetRdRsRt32bit(PASS_PARAMS);
    
    compilerstatus.cp0Counter += 1;
	_OPCODE_DEBUG_BRANCH_(r4300i_jr)
    
    
    if( ConstMap[xRS->mips_reg].IsMapped)
    {
        MOV_ImmToMemory(1, ModRM_disp32, (unsigned long) &r.r_.pc, ConstMap[xRS->mips_reg].value);
    }
    else
    {
        MapRS;
	    MOV_RegToMemory(1, xRS->x86reg, ModRM_disp32, (unsigned long) &r.r_.pc);
    }

	r.r_.pc += 4;
	
    Compile_Slot_Jump(r.r_.pc);

	/* end of compiled block */
	compilerstatus.KEEP_RECOMPILING = FALSE;

    FlushAllRegisters(FLUSH_TO_MEMORY);
    InterruptsSafe(JUMP_TYPE_INDIRECT, 0, LINK_NO, 0);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_bc1f(OP_PARAMS)
{
	_u32	aValue;
    int     ExtraRegister;
    int tempPC = r.r_.pc;

	compilerstatus.cp0Counter += 1;
	_OPCODE_DEBUG_BRANCH_(r4300i_COP1_bc1f);
	aValue = (r.r_.pc + 4 + (__I << 2));

    ExtraRegister = FindFreeRegister();
    if (ExtraRegister < 0)
        ExtraRegister = Reg_EBP;


	MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
	AND_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
	if (ExtraRegister == Reg_EBP) MOV_ImmToReg(Reg_EBP, HardwareStart);

	Jcc_Near_auto(CC_NZ, 91);	/* jmp false */

	/* delay Slot */
    SPEED_HACK
    r.r_.pc += 4;
	Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);

	SetNearTarget(91);
	
    
    BranchOutEpilogue(tempPC, 0);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_bc1fl(OP_PARAMS)
{
	_u32	aValue;
    int     ExtraRegister;
    int tempPC = r.r_.pc+4;

	compilerstatus.cp0Counter += 1;
	_OPCODE_DEBUG_BRANCH_(r4300i_COP1_bc1fl);
	aValue = (r.r_.pc + 4 + (__I << 2));

    ExtraRegister = FindFreeRegister();
    if (ExtraRegister < 0)
        ExtraRegister = Reg_EBP;
    
    
	MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
	AND_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
	if (ExtraRegister == Reg_EBP) MOV_ImmToReg(Reg_EBP, HardwareStart);

	Jcc_Near_auto(CC_NZ, 91);	/* jmp false */

	/* delay Slot */
	SPEED_HACK 
	r.r_.pc += 4;
	Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);

	SetNearTarget(91);

    BranchOutEpilogue(tempPC, 1);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_bc1t(OP_PARAMS)
{
	_u32	aValue;
    int     ExtraRegister;
    int tempPC = r.r_.pc;

	compilerstatus.cp0Counter += 1;
	_OPCODE_DEBUG_BRANCH_(r4300i_COP1_bc1t);
	aValue = (r.r_.pc + 4 + (__I << 2));

    ExtraRegister = FindFreeRegister();
    if (ExtraRegister < 0)
        ExtraRegister = Reg_EBP;

    
    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
	AND_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
	if (ExtraRegister == Reg_EBP) MOV_ImmToReg(Reg_EBP, HardwareStart);

	Jcc_Near_auto(CC_Z, 91);	/* jmp false */

	/* delay Slot */
	SPEED_HACK
    r.r_.pc += 4;
	Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);

	SetNearTarget(91);
	
    BranchOutEpilogue(tempPC, 0);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop1_bc1tl(OP_PARAMS)
{
	_u32	aValue;
    int     ExtraRegister;
    int tempPC = r.r_.pc+4;

	compilerstatus.cp0Counter += 1;
	_OPCODE_DEBUG_BRANCH_(r4300i_COP1_bc1tl);
	aValue = (r.r_.pc + 4 + (__I << 2));

    ExtraRegister = FindFreeRegister();
    if (ExtraRegister < 0)
        ExtraRegister = Reg_EBP;

    
    MOV_MemoryToReg(ExtraRegister, ModRM_disp32, (_u32) & reg->COP1Con[31]);
	AND_ImmToReg(ExtraRegister, COP1_CONDITION_BIT, 0);
	if (ExtraRegister == Reg_EBP) MOV_ImmToReg(Reg_EBP, HardwareStart);

	Jcc_Near_auto(CC_Z, 91);	/* jmp false */

	/* delay Slot */
    SPEED_HACK
    r.r_.pc += 4;
	Compile_Slot_And_InterruptsNewTest(r.r_.pc, aValue, LINK_NO, 0, 1);

	SetNearTarget(91);

    BranchOutEpilogue(tempPC, 1);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void jalr(OP_PARAMS)
{
	/*~~~~~~~~~~~*/
	_u32	aValue;
	/*~~~~~~~~~~~*/


	compilerstatus.cp0Counter += 1;
	_OPCODE_DEBUG_BRANCH_(r4300i_jalr)
#ifdef _DEBUG
	if(__RD == __RS)
	{
		DisplayError("In JALR, __RD == __RS");
	}
#endif
	aValue = ((r.r_.pc & 0xf0000000) | (____T << 2));

    FlushAllRegisters(FLUSH_TO_MEMORY);
	if (__RD==0);
	else
	if(__RD < 17)
	{
		MOV_ImmToMemory(1, ModRM_disp8_EBP, (-128 + (((__RD)-1) << 3)), (r.r_.pc + 8));
		MOV_ImmToMemory(1, ModRM_disp8_EBP, (-124 + (((__RD)-1) << 3)), ((_int32) (r.r_.pc + 8)) >> 31);
	}
	else
	{
		MOV_ImmToMemory(1, ModRM_disp8_EBP, ((((__RD)-1) - 16)) << 3, (r.r_.pc + 8));
		MOV_ImmToMemory(1, ModRM_disp8_EBP, 4 + (((((__RD)-1) - 16)) << 3), ((_int32) (r.r_.pc + 8)) >> 31);
	}

	LoadLowMipsCpuRegister(__RS, Reg_EAX);
	MOV_EAXToMemory(1, (unsigned long) &r.r_.pc);

	r.r_.pc += 4;
	Compile_Slot_Jump(r.r_.pc);

	/* end of compiled block */
	compilerstatus.KEEP_RECOMPILING = FALSE;

	/* Already calculated the link above, so LINK_NO. */
    InterruptsSafe(JUMP_TYPE_INDIRECT, 0, LINK_NO, 0);
}


void Riight()
{
    MessageBox(0, "Riight", "", 0);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void cop0_eret(OP_PARAMS)
{
    compilerstatus.cp0Counter += 1;
	_OPCODE_DEBUG_BRANCH_(r4300i_COP0_eret) /* questionable. */

	
    FlushAllRegisters(FLUSH_TO_MEMORY);

	MOV_MemoryToReg(Reg_EAX, ModRM_disp32, (_u32) & reg->COP0Reg[STATUS]);
	AND_ImmToReg(Reg_EAX, 0x0004, 0);
	CMP_RegWith0(Reg_EAX); //test erl
	Jcc_auto(CC_E, 26);
	MOV_MemoryToReg(Reg_EAX, ModRM_disp32, (_u32) & reg->COP0Reg[ERROREPC]);
	AND_ImmToMemory((unsigned long) &reg->COP0Reg[STATUS], 0xFFFFFFFB); //clear ERL
	JMP_Short_auto(27);
	SetTarget(26);
	MOV_MemoryToReg(Reg_EAX, ModRM_disp32, (_u32) & reg->COP0Reg[EPC]);
    AND_ImmToMemory((unsigned long) &reg->COP0Reg[STATUS], 0xFFFFFFFD); // clear EXL
	MOV_ImmToMemory(1,ModRM_disp32,(uint32)&emustatus.processing_exception, FALSE);
	SetTarget(27);

    MOV_ImmToMemory(1, ModRM_disp32, (unsigned long) &reg->LLbit, 0);
	MOV_RegToMemory(1, Reg_EAX, ModRM_disp32, (unsigned long) &r.r_.pc);

	/* end of compiled block */
	compilerstatus.KEEP_RECOMPILING = FALSE;

	
    
    Interrupts(JUMP_TYPE_INDIRECT, 0, LINK_NO, 0);
}
#endif
