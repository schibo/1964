/*$T compiler.c GC 1.136 03/09/02 17:28:30 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    Compiles blocks of code to native x86 code for speed. A block is terminated by a MIPS?jump, branch, or ERET
    instruction. Blocks can be "linked" if they reside in the same 4KB page. Linking blocks avoids the need for a block
    of code to return to the compiler to fetch the next block when it is done executing. This is achieved by setting a
    jump target at the end of the block to the head of the next destination block. When the destination block's start
    address becomes known, the jump target is filled with that address. With the help of Protected Memory, we know when
    these blocks need to be invalidated. (when there is a store opcode, or dma write, etc)
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
#include "stdafx.h"

#ifdef _DEBUG
#include "win32/windebug.h"
extern char				*DebugPrintInstruction(uint32 instruction);
#endif
extern char				*DebugPrintInstructionWithOutRefresh(uint32 Instruction);
extern char				*DebugPrintInstr(uint32 Instruction);
extern uint32			TLB_Error_Vector;
uint8					*dyna_RecompCode = NULL;

uint8					*Block;
uint8					*RDRAM_Copy;

struct CompilerStatus	compilerstatus;

void					Set_Translate_PC(void);
void					Interrupts(uint32 JumpType, uint32 targetpc, uint32 DoLink, uint32 LinkVal);
void					DisplayLinkPC(void);
void					DisplayPC(void);
void					RefreshDynaDuringGamePlay(void);
void					AnalyzeBlock(void);
void					Dyna_Code_Check_None(void);
void					Dyna_Code_Check_QWORD(void);
void					Dyna_Code_Check_DWORD(void);
void					Dyna_Code_Check_BLOCK(void);
void					Dyna_Code_Check_None_Boot(void);
void (*Dyna_Code_Check[]) () =
{
	Dyna_Code_Check_None,
	Dyna_Code_Check_None,
	Dyna_Code_Check_DWORD,
	Dyna_Code_Check_QWORD,
	Dyna_Code_Check_QWORD,
	Dyna_Code_Check_BLOCK,
	Dyna_Code_Check_BLOCK,
	Dyna_Code_Check_None
};
void (*Dyna_Check_Codes) () = NULL;

BLOCK_ENTRY *block_queue_head = NULL;
BLOCK_ENTRY *current_block_entry = NULL;
void		dequeue_heading_block_entry(void);
BLOCK_ENTRY *get_new_block_entry(uint32 pc);
BOOL		IsBlockCompiled(uint32 pc);
BLOCK_ENTRY *add_new_block_entry(uint32 pc);
uint32		GetCompiledBlockPtr(uint32 pc);


extern HANDLE StopEmulatorEvent;
/*
 =======================================================================================================================
    Returns the 32bit MIPS instruction at the current address in PC (Program Counter register)
 =======================================================================================================================
 */
__forceinline uint32 DynaFetchInstruction(uint32 pc)
{
	/*~~~~~~~~~~~~~*/
	uint32	code = 0;
	/*~~~~~~~~~~~~~*/

	compilerstatus.realpc_fetched = pc;
	if(NOT_IN_KO_K1_SEG(compilerstatus.realpc_fetched))
	{
		compilerstatus.realpc_fetched = TranslateITLBAddress(compilerstatus.realpc_fetched);
		if(ITLB_Error)
		{
			return code;
		}
	}

	__try
	{
		compilerstatus.pcptr = pLOAD_UWORD_PARAM(compilerstatus.realpc_fetched);
		code = *compilerstatus.pcptr;

		if((compilerstatus.realpc_fetched & 0x1FFFFFFF) < current_rdram_size)
		{
			if(currentromoptions.Code_Check == CODE_CHECK_PROTECT_MEMORY)
			{
				ProtectBlock(compilerstatus.realpc_fetched);
			}

			* (uint32 *) &RDRAM_Copy[compilerstatus.realpc_fetched & 0x1FFFFFFF] = code;
			if(currentromoptions.Link_4KB_Blocks != USE4KBLINKBLOCK_YES)
			{
				if(gHardwareState.sDYN_PC_LOOKUP[compilerstatus.realpc_fetched >> 16] == gMemoryState.dummyAllZero)
					UnmappedMemoryExceptionHelper(compilerstatus.realpc_fetched);
				*(uint32 *)
					(
						(uint8 *) gHardwareState.sDYN_PC_LOOKUP[compilerstatus.realpc_fetched >> 16] +
						(uint16) compilerstatus.realpc_fetched
					) = 0;
			}
		}
	}

	__except(NULL, EXCEPTION_EXECUTE_HANDLER)
	{
		DisplayError("%08X: Dyna PC out of range", pc);
        emustatus.Emu_Is_Running = 1;
        PostMessage(gui.hwnd1964main, WM_COMMAND, ID_ROM_STOP, 0);
        WaitForSingleObject(StopEmulatorEvent, 1000);
        TerminateThread(CPUThreadHandle, 0);
	}

	return code;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
unsigned __int32 DynaFetchInstruction2(uint32 pc)
{
	/*~~~~~~~~~~~~~~~~*/
	uint32	code = 0;
	uint32	savepc = pc;
	/*~~~~~~~~~~~~~~~~*/

	compilerstatus.realpc_fetched = savepc;
	if(NOT_IN_KO_K1_SEG(compilerstatus.realpc_fetched))
	{
		ITLB_Error = FALSE;
		compilerstatus.realpc_fetched = TranslateITLBAddress(compilerstatus.realpc_fetched);
		if(ITLB_Error)
		{
         //   DisplayError("Warning, ITLB error happens during Dyna instruction fetch, and this is not the beginning of the block.");
			TRACE1("ITLB error happens when fetching branch delay slot opcode, pc=%08X", compilerstatus.realpc_fetched);
			HandleExceptions(TLB_Error_Vector);

			ITLB_Error = FALSE;
			compilerstatus.realpc_fetched = savepc;
			compilerstatus.realpc_fetched = TranslateITLBAddress(compilerstatus.realpc_fetched);
			if(ITLB_Error)
			{
			//	DisplayError("Warning, ITLB error happens during Dyna instruction fetch, and this is not the beginning of the block");
				TRACE1
				(
					"Warning, ITLB error happens when fetching branch delay slot opcode the 2nd time, pc=%08X",
					compilerstatus.realpc_fetched
				);
				HandleExceptions(TLB_Error_Vector);

				ITLB_Error = FALSE;
				compilerstatus.realpc_fetched = savepc;
				compilerstatus.realpc_fetched = TranslateITLBAddress(compilerstatus.realpc_fetched);
				if(ITLB_Error)
				{
                    TRACE1
					(
						"Warning, ITLB error happens when fetching branch delay slot opcode the 3rd time, pc=%08X",
						compilerstatus.realpc_fetched
					);
					DisplayError("Cannot solve ITLB exception");
					compilerstatus.realpc_fetched = savepc - 4;
					compilerstatus.realpc_fetched = TranslateITLBAddress(compilerstatus.realpc_fetched);
					compilerstatus.realpc_fetched += 4;
					ITLB_Error = FALSE;
					goto step2;
				}
			}
		}

		ITLB_Error = FALSE;
	}

step2:
	__try
	{
		code = LOAD_UWORD_PARAM(compilerstatus.realpc_fetched);

		if((compilerstatus.realpc_fetched & 0x1FFFFFFF) < current_rdram_size)
		{
			if(currentromoptions.Code_Check == CODE_CHECK_PROTECT_MEMORY)
			{
				ProtectBlock(compilerstatus.realpc_fetched);
			}

			* (uint32 *) &RDRAM_Copy[compilerstatus.realpc_fetched & 0x1FFFFFFF] = code;
			if(currentromoptions.Link_4KB_Blocks != USE4KBLINKBLOCK_YES)
			{
				if(gHardwareState.sDYN_PC_LOOKUP[compilerstatus.realpc_fetched >> 16] == gMemoryState.dummyAllZero)
					UnmappedMemoryExceptionHelper(compilerstatus.realpc_fetched);
				*(uint32 *)
					(
						(uint8 *) gHardwareState.sDYN_PC_LOOKUP[compilerstatus.realpc_fetched >> 16] +
						(uint16) compilerstatus.realpc_fetched
					) = 0;
			}
		}
	}

	__except(NULL, EXCEPTION_EXECUTE_HANDLER)
	{
        DisplayError("%08X: Dyna PC out of range", pc);
        emustatus.Emu_Is_Running = 1;
        PostMessage(gui.hwnd1964main, WM_COMMAND, ID_ROM_STOP, 0);
        WaitForSingleObject(StopEmulatorEvent, 1000);
        TerminateThread(CPUThreadHandle, 0);
	}

	return code;
}

/*
 =======================================================================================================================
    Compiles a block of native x86 machine code. Compilation of a block ends at a MIPS?jump, branch, or eret
    instruction.
 =======================================================================================================================
 */
uint32 Dyna_Compile_Single_Block(void)
{
	uint32	*LookupPtr;
	int		templCodePosition;
	int		k;	/* variable for the instruction reordering */

	
    if (compilerstatus.DynaBufferOverError)
    {
        RefreshDynaDuringGamePlay();
        compilerstatus.DynaBufferOverError = FALSE;
    }
    
    
    compilerstatus.Is_Compiling++;
begin:
	LOGGING_DYNA(LogDyna("\n\n** Compile Single Block at PC=%08X\n", gHWS_pc));

	if(ITLB_Error)
	{
        /*
		 * DisplayError("TLB error happens during compiling, PC=%08X",
		 * compilerstatus.TempPC);
		 */
		if((gHWS_COP0Reg[STATUS] & EXL) == 0)	/* Exception not in exception */
		{
			gHWS_COP0Reg[EPC] = gHWS_pc;
			gHWS_COP0Reg[STATUS] |= EXL;		/* set EXL = 1 */
		}
		else
		{
			gHWS_COP0Reg[EPC] = gHWS_pc;

			/* DisplayError("Warning, Exception happens in exception"); */
			TRACE1("Warning, Exception happens in exception, pc=%08X", compilerstatus.TempPC);
		}

		/*
		 * TLB_TRACE(TRACE2("ITLB exception at Instruction fetching, PC=%08X,
		 * Vector=%08X", gHWS_pc, TLB_Error_Vector));
		 */
		gHWS_pc = TLB_Error_Vector;
		Set_Translate_PC();
		gHWS_COP0Reg[CAUSE] &= NOT_BD;			/* clear BD */

		Block = (uint8 *) *r.r_.g_LookupPtr;
		ITLB_Error = FALSE;

		if(Block != NULL && r.r_.g_pc_is_rdram) Dyna_Check_Codes();
		if(Block == NULL)
		{
			goto start_compile;
		}
		else
		{
			compilerstatus.Is_Compiling--;
			return(uint32) Block;
		}
	}




start_compile:

    
	if(currentromoptions.Advanced_Block_Analysis == USEBLOCKANALYSIS_YES)
	{
        AnalyzeBlock();
	}
    
    
    k = 0;
	if(compilerstatus.Is_Compiling > 1) DisplayError("Compiler is re-entered, cannot support it.");

	templCodePosition = compilerstatus.lCodePosition;
	ThisYear = 2001;
	ITLB_Error = FALSE;					/* this is moved to here. */
	compilerstatus.TempPC = gHWS_pc;
	compilerstatus.realpc_fetched = gHWS_pc;

	gMultiPass.PhysAddrAfterMap = compilerstatus.BlockStart;

	compilerstatus.KEEP_RECOMPILING = 1;
	compilerstatus.cp0Counter = 0;
	compilerstatus.InstructionCount = 0;

	/* align block to qword */
	//while((compilerstatus.lCodePosition & 0xffffff80) != compilerstatus.lCodePosition) WC8(0x90);
	//if((compilerstatus.lCodePosition - templCodePosition) >= 2) compilerstatus.lCodePosition -= 2;
	compilerstatus.lCodePosition += 2;	/* increase the compilerstatus.lCodePosition by 2 leave two bytes in front of
										 * the block */

	/* to store block size */
	if (compilerstatus.lCodePosition >= 2)
    compilerstatus.BlockStart = (uint32) (&dyna_RecompCode[compilerstatus.lCodePosition]);


	//Dummy Load of GPR[1] for better caching
//	FetchEBP_Params(30);
//	MOV_MemoryToReg(Reg_ECX, x86params.ModRM,  x86params.Address);
	
	/* get instruction */
	gHWS_code = DynaFetchInstruction(gHWS_pc);

	if(ITLB_Error)
	{
		/*
		 * DisplayError("TLB error happens during compiling, PC=%08X",
		 * compilerstatus.TempPC);
		 */
		if((gHWS_COP0Reg[STATUS] & EXL) == 0)	/* Exception not in exception */
		{
			gHWS_COP0Reg[EPC] = compilerstatus.TempPC;
			gHWS_COP0Reg[STATUS] |= EXL;		/* set EXL = 1 */
		}
		else
		{
			DisplayError("Warning, Exception happens in exception");
			TRACE1("Warning, Exception happens in exception, pc=%08X", compilerstatus.TempPC);
		}

		gHWS_pc = TLB_Error_Vector;

		/*
		 * TLB_TRACE(TRACE2("ITLB exception at Instruction fetching, PC=%08X,
		 * Vector=%08X", compilerstatus.TempPC, TLB_Error_Vector));
		 */
		Set_Translate_PC();
		gHWS_COP0Reg[CAUSE] &= NOT_BD;			/* clear BD */

		Block = (uint8 *) *r.r_.g_LookupPtr;
		ITLB_Error = FALSE;

		if(Block != NULL && r.r_.g_pc_is_rdram) Dyna_Check_Codes();
		if(Block == NULL)
		{
			goto begin;					/* redo_compile; */
		}
		else
		{
			compilerstatus.Is_Compiling--;
			return(uint32) Block;
		}
	}
	else
	{
		if(gHardwareState.sDYN_PC_LOOKUP[compilerstatus.realpc_fetched >> 16] == gMemoryState.dummyAllZero)
		{
			UnmappedMemoryExceptionHelper(compilerstatus.realpc_fetched);
		}

     
		LookupPtr = (uint32 *) ((uint8 *) gHardwareState.sDYN_PC_LOOKUP[compilerstatus.realpc_fetched >> 16] + (uint16) compilerstatus.realpc_fetched);
		Block = (uint8 *) compilerstatus.BlockStart;
		if (compilerstatus.lCodePosition >= 2)
        *(uint16 *) (Block - 2) = 0;	/* store block size */
	}

	if(currentromoptions.Use_Register_Caching == USEREGC_NO) 
        FlushAllRegisters(FLUSH_TO_MEMORY);
	DYNA_DEBUG_INSTRUCTION(gHWS_code);
	DYNA_LOG_INSTRUCTION(gHWS_code);

#ifdef _DEBUG
	MOV_ImmToMemory(1, ModRM_disp32, (unsigned long) &gHWS_pc, gHWS_pc);
#endif

	/*
	 * Rice: Right now, i'm only doing some HLE for Mario(US), and i know the exact
	 * address ?
	 * of the functions. This needs to be replaced with a call to a crc detection
	 * algortithm. ?
	 * Then HLE will be stable for other games. HLE default is "no" at the moment, and
	 * user-disabled.
	 */
	gHWS_code = DynaFetchInstruction(gHWS_pc);

	dyna_instruction[((unsigned) (gHWS_code >> 26))](&gHardwareState);
	compilerstatus.InstructionCount++;

	while(compilerstatus.KEEP_RECOMPILING)
	{
		/* This code is disabled for multipass because it does not work. */
		{
			/*
			 * Need to break out at the end of 4KB block if we are using protected memory or
			 * we are in TLB mapped address
			 */
			if
			(
				((gHWS_pc + 4) / 0x1000) != (gHWS_pc / 0x1000)
			&&	(NOT_IN_KO_K1_SEG(gHWS_pc) || currentromoptions.Code_Check == CODE_CHECK_PROTECT_MEMORY)
			)
			{
				MOV_ImmToMemory(1, ModRM_disp32, (unsigned long) &gHWS_pc, gHWS_pc + 4);

				/* end of compiled block */
				compilerstatus.KEEP_RECOMPILING = FALSE;
				FlushAllRegisters(FLUSH_TO_MEMORY);
				Interrupts(0, 0, 0, 0);				/* JUMP_TYPE_INDIRECT); */ //looks like a possible bug?

				/*
				 * TRACE2("Block at %08x covers 4KB boundry at pc=%08X, breaks out",
				 * compilerstatus.TempPC, gHWS_pc+4);
				 */
				break;
			}
		}

		gHWS_pc += 4;
		compilerstatus.InstructionCount++;

		gHWS_code = DynaFetchInstruction(gHWS_pc);

		if(ITLB_Error)
		{
			/*
			 * DisplayError("TLB error happens during compiling, PC=%08X",
			 * compilerstatus.TempPC); ?
			 * TLB_TRACE(TRACE1("ITLB exception at Instruction fetching during compiling,
			 * PC=%08X", compilerstatus.TempPC));
			 */
			HandleExceptions(TLB_Error_Vector);
			gHWS_code = DynaFetchInstruction(gHWS_pc);

		}

		k++;

		if(currentromoptions.Use_Register_Caching == USEREGC_NO)
            FlushAllRegisters(FLUSH_TO_MEMORY);
		DYNA_DEBUG_INSTRUCTION(gHWS_code);

#ifdef _DEBUG
		MOV_ImmToMemory(1, ModRM_disp32, (unsigned long) &gHWS_pc, gHWS_pc);
#endif
		dyna_instruction[((unsigned) (gHWS_code >> 26))](&gHardwareState);
	}

#ifdef _DEBUG
	if(compilerstatus.InstructionCount > 255 && debugoptions.debug_dyna_compiler )
	{
		TRACE3
		(
			"Compiled Block is too large, size=%d, pc=%08X, end at %08X",
			compilerstatus.cp0Counter + 1,
			compilerstatus.TempPC,
			compilerstatus.TempPC + (compilerstatus.cp0Counter + 1) * 4
		);
	}
#endif

	/* Save info for dyna code check/check block to use */
	*LookupPtr = (_u32) Block;				/* Need to assign the value after compiling, otherwise the value will */

	/* be set to 0 when doing DynaFetchInstruction() */
	*(uint16 *) (Block - 2) = (uint16) compilerstatus.InstructionCount + 1;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/* store block size */
	DEBUG_PRINT_DYNA_COMPILE_INFO	gHWS_pc = compilerstatus.TempPC;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/*
	 * I am trying to map TLB address also into gHardwareState.sDYN_PC_LOOKUP ?
	 * if( NOT_IN_KO_K1_SEG(gHWS_pc) ) { uint32 ptr; if( gHardwareState.sDYN_PC_LOOKUP[gHWS_pc>>16]
	 * == gMemoryState.dummyAllZero ) UnmappedMemoryExceptionHelper(gHWS_pc); ptr
	 * (uint32)gHardwareState.sDYN_PC_LOOKUP[gHWS_pc>>16]; (uint32*)(ptr + (uint16)gHWS_pc) = Block;
	 * } ?
	 * Not sure if necessary.
	 */
	compilerstatus.Is_Compiling--;

	return(uint32) Block;
}

/*
 =======================================================================================================================
    type = 0 WORD ?
    1 HALFWORD ?
    2 BYTE ?
    3 DWORD
 =======================================================================================================================
 */
void Invalidate4KBlock(uint32 addr, char *opcodename, int type, uint64 newvalue)
{
#ifdef _DEBUG
	if(addr / 0x1000 == gHWS_pc / 0x1000)
	{
		TRACE1("Warning, invalidate the block while PC=%08X is in the block", gHWS_pc);
	}
#endif
	if(IN_KO_K1_SEG(addr))
		addr = addr & 0xDFFFFFFF;
	else
	{
		CODE_DETECT_TRACE(TRACE0("Warning, cannot invalidate a block not in RDRAM"));
		return;
	}

	if(addr < 0x80000000 + current_rdram_size)
	{

        uint32	offset = addr & 0x7fffffff;
		
		switch(type)
		{
		case WORDTYPE:
			{
				if((uint32) newvalue == *(uint32 *) (RDRAM_Copy + offset));
				else if(*(uint32 *) (RDRAM_Copy + offset) == DUMMYOPCODE);
				else
					*(uint32 *) (RDRAM_Copy + offset) = DUMMYOPCODE;
			}
			break;
		case HWORDTYPE:
			{
				if((uint16) newvalue == *(uint16 *) (RDRAM_Copy + offset));
				else if(*(uint32 *) (RDRAM_Copy + offset) == DUMMYOPCODE);
				else
					*(uint32 *) (RDRAM_Copy + offset) = DUMMYOPCODE;
			}
			break;
		case BYTETYPE:
			{
				if((uint8) newvalue == *(uint8 *) (RDRAM_Copy + offset));
				else if(*(uint32 *) (RDRAM_Copy + offset) == DUMMYOPCODE);
				else
					*(uint32 *) (RDRAM_Copy + offset) = DUMMYOPCODE;
			}
			break;
		case DWORDTYPE:
			{
				uint64	dummy = (newvalue >> 32) | (newvalue << 32);

				if(dummy == *(uint64 *) (RDRAM_Copy + offset));
				else if
					(
						*(uint32 *) (RDRAM_Copy + offset) == DUMMYOPCODE
					&&	*(uint32 *) (RDRAM_Copy + offset + 4) == DUMMYOPCODE
					);
				else
				{
					*(uint32 *) (RDRAM_Copy + offset) = DUMMYOPCODE;
					*(uint32 *) (RDRAM_Copy + offset + 4) = DUMMYOPCODE;
				}
			}
			break;
		case NOCHECKTYPE:
			break;
		default:
			CODE_DETECT_TRACE(TRACE0("Warning, incorrect data type"));
		}

		InvalidateOneBlock(addr);
		UnprotectBlock(addr);
		CODE_DETECT_TRACE(TRACE2("Protect Memory in %s found self-mod code at %08X, invalidate the block", opcodename, addr));
	}

	/*
	 * else ?
	 * { ?
	 * CODE_DETECT_TRACE(TRACE0("Warning, protected memory is no in RDRAM")); ?
	 * }
	 */
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void InvalidateOneBlock(uint32 pc)
{
	/*~~~~~~~~~~~*/
	uint32	offset;
	/*~~~~~~~~~~~*/


	/*
	 * up to here, we have identified the new value as different from the old value ?
	 * and the old value was used as code, not as data, so we need to invalidate
	 * the whole 4KB block and unprotect the 4KB block
	 */
	for(offset = (pc & 0xFFFFF000); offset < (pc & 0xFFFFF000) + 0x1000; offset += 4)
	{
		if(gHardwareState.sDYN_PC_LOOKUP[offset >> 16] != gMemoryState.dummyAllZero)
			*(uint32 *) ((uint8 *) gHardwareState.sDYN_PC_LOOKUP[offset >> 16] + (uint16) offset) = 0;
		else
			break;
	}
}

/*
 =======================================================================================================================
    Validate the compiled block, and doing dyna code checking ?
 =======================================================================================================================
 */
void Dyna_Code_Check_None(void)
{
}

/*
 =======================================================================================================================
    Validate the compiled block, and doing dyna code checking by checking QWORD method ?
 =======================================================================================================================
 */
void Dyna_Code_Check_QWORD(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	register uint32 pc = r.r_.g_pc_is_rdram;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(*(uint32 *) (RDRAM_Copy + pc) != *(uint32 *) (gMS_RDRAM + pc))
	{
		Block = NULL;
		DEBUG_DYNA_MOD_CODE_TRACE(TRACE1("Found mod-code at %08x", r.r_.g_pc_is_rdram));
		*(uint32 *) ((uint8 *) gHardwareState.sDYN_PC_LOOKUP[(pc | 0x80000000) >> 16] + (uint16) pc) = 0;
		*(uint32 *) (RDRAM_Copy + pc) = *(uint32 *) (gMS_RDRAM + pc);
	}
	else if(*(uint32 *) (RDRAM_Copy + pc + 4) != *(uint32 *) (gMS_RDRAM + pc + 4))
	{
		Block = NULL;
		DEBUG_DYNA_MOD_CODE_TRACE(TRACE1("Found mod-code at %08x", r.r_.g_pc_is_rdram));
		*(uint32 *) ((uint8 *) gHardwareState.sDYN_PC_LOOKUP[((pc + 4) | 0x80000000) >> 16] + (uint16) (pc + 4)) = 0;
		*(uint32 *) (RDRAM_Copy + pc + 4) = *(uint32 *) (gMS_RDRAM + pc + 4);
	}
}

/*
 =======================================================================================================================
    Validate the compiled block, and doing dyna code checking by checking DWORD method ?
 =======================================================================================================================
 */
void Dyna_Code_Check_DWORD(void)
{
	if(*(uint32 *) (RDRAM_Copy + r.r_.g_pc_is_rdram) != *(uint32 *) (gMS_RDRAM + r.r_.g_pc_is_rdram))
	{
		Block = NULL;
		DEBUG_DYNA_MOD_CODE_TRACE(TRACE1("Found mod-code at %08x", r.r_.g_pc_is_rdram));
		*(uint32 *) (RDRAM_Copy + r.r_.g_pc_is_rdram) = *(uint32 *) (gMS_RDRAM + r.r_.g_pc_is_rdram);
	}
}

/*
 =======================================================================================================================
    Validate the compiled block, and doing dyna code checking by checking whole block method ?
 =======================================================================================================================
 */
void Dyna_Code_Check_BLOCK(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	register int	length; /* How to get the length of the block */
	uint32			pc = r.r_.g_pc_is_rdram;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(length = *(uint16 *) (Block - 2) - 1; length >= 0; length--, pc += 4)
	{
		if(*(uint32 *) (RDRAM_Copy + pc) != *(uint32 *) (gMS_RDRAM + pc))
		{
			Block = NULL;
			DEBUG_DYNA_MOD_CODE_TRACE(TRACE1("Found mod-code at %08x", r.r_.g_pc_is_rdram));
			while(length >= 0)
			{
				*(uint32 *) ((uint8 *) gHardwareState.sDYN_PC_LOOKUP[(pc | 0x80000000) >> 16] + (uint16) pc) = 0;
				*(uint32 *) (RDRAM_Copy + pc) = *(uint32 *) (gMS_RDRAM + pc);
				length--;
				pc += 4;
			}
			break;
		}
	}
}

/*
 =======================================================================================================================
    Validate the compiled block, and doing dyna code checking by checking QWORD method ?
 =======================================================================================================================
 */
void Dyna_Code_Check_None_Boot(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	register uint32 pc = r.r_.g_pc_is_rdram;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(*(uint32 *) (RDRAM_Copy + pc) != *(uint32 *) (gMS_RDRAM + pc))
	{
		Block = NULL;
		DEBUG_DYNA_MOD_CODE_TRACE(TRACE1("Found mod-code at %08x", r.r_.g_pc_is_rdram));

		/*
		 * if( currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES ) ?
		 * { ?
		 * InvalidateOneBlock(r.r_.g_pc_is_rdram|0x80000000); ?
		 * } ?
		 * else
		 */
		{
			*(uint32 *) ((uint8 *) gHardwareState.sDYN_PC_LOOKUP[(pc | 0x80000000) >> 16] + (uint16) pc) = 0;
			*(uint32 *) (RDRAM_Copy + pc) = *(uint32 *) (gMS_RDRAM + pc);
		}
	}
	else if(*(uint32 *) (RDRAM_Copy + pc + 4) != *(uint32 *) (gMS_RDRAM + pc + 4))
	{
		Block = NULL;
		DEBUG_DYNA_MOD_CODE_TRACE(TRACE1("Found mod-code at %08x", r.r_.g_pc_is_rdram));

		/*
		 * if( currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES ) ?
		 * { ?
		 * InvalidateOneBlock(r.r_.g_pc_is_rdram|0x80000000); ?
		 * } ?
		 * else
		 */
		{
			*(uint32 *) ((uint8 *) gHardwareState.sDYN_PC_LOOKUP[((pc + 4) | 0x80000000) >> 16] + (uint16) (pc + 4)) = 0;
			*(uint32 *) (RDRAM_Copy + pc + 4) = *(uint32 *) (gMS_RDRAM + pc + 4);
		}
	}

	if(emustatus.DListCount > 80)	/* yes, we have detect the first self-modify code */
	{
		Dyna_Check_Codes = Dyna_Code_Check[emustatus.CodeCheckMethod - 1];
		TRACE0("Reset the Dyna Code Check Method => None");
	}
}

/*
 =======================================================================================================================
    Link block, by target1
 =======================================================================================================================
 */
void Link1(void)
{
	uint32	offset = (uint32) current_block_entry->block_ptr - 1 - (uint32)
		(&RecompCode[block_queue_head->jmp_to_target_1_code_addr]);


	// JMP Short

	if  ( ((offset < 127) || (offset > 0xffffff81))
        && ( *(uint8 *)&RecompCode[block_queue_head->jmp_to_target_1_code_addr] == 0x90   ) 
        && ( *(uint8 *)&RecompCode[block_queue_head->jmp_to_target_1_code_addr-1] & 0x80  )
        && ( *(uint8 *)&RecompCode[block_queue_head->jmp_to_target_1_code_addr-2] == 0x0F )
        )
        
    {
        (*((uint8 *) (&RecompCode[block_queue_head->jmp_to_target_1_code_addr-2]))) = 
            0x70|(*(uint8 *)&RecompCode[block_queue_head->jmp_to_target_1_code_addr-1]^0x80);
		(*((uint8 *) (&RecompCode[block_queue_head->jmp_to_target_1_code_addr-1]))) = (uint8) offset+1;
	}
	else

   
    {	// JMP Long (Near)
		(*((unsigned _int32 *) (&RecompCode[block_queue_head->jmp_to_target_1_code_addr]))) =
				(uint32) current_block_entry->block_ptr -
			4 -
			(uint32) (&RecompCode[block_queue_head->jmp_to_target_1_code_addr]);
	}
}

/*
 =======================================================================================================================
    Link block, by target2
 =======================================================================================================================
 */
void Link2(void)
{
	uint32	offset = (uint32) current_block_entry->block_ptr - 1 - (uint32)
		(&RecompCode[block_queue_head->jmp_to_target_2_code_addr]);


	/* JMP Short */
	if  ( ((offset < 127) || (offset > 0xffffff81))
        && ( *(uint8 *)&RecompCode[block_queue_head->jmp_to_target_2_code_addr] == 0x90   ) 
        && ( *(uint8 *)&RecompCode[block_queue_head->jmp_to_target_2_code_addr-1] & 0x80  )
        && ( *(uint8 *)&RecompCode[block_queue_head->jmp_to_target_2_code_addr-2] == 0x0F )
        )
        
    {
        (*((uint8 *) (&RecompCode[block_queue_head->jmp_to_target_2_code_addr-2]))) = 
            0x70|(*(uint8 *)&RecompCode[block_queue_head->jmp_to_target_2_code_addr-1]^0x80);
		(*((uint8 *) (&RecompCode[block_queue_head->jmp_to_target_2_code_addr-1]))) = (uint8) offset+1;
	}
	else if((offset < 127) || (offset > 0xffffff81))
	{
       
        (*((uint8 *) (&RecompCode[block_queue_head->jmp_to_target_2_code_addr - 1]))) = 0xEB;
		(*((uint8 *) (&RecompCode[block_queue_head->jmp_to_target_2_code_addr]))) = (uint8) offset;
	}
	else
    
    {	/* JMP Long (Near) */
		(*((unsigned _int32 *) (&RecompCode[block_queue_head->jmp_to_target_2_code_addr]))) =
				(uint32) current_block_entry->block_ptr -
			4 -
			(uint32) (&RecompCode[block_queue_head->jmp_to_target_2_code_addr]);
	}
}

/*
 =======================================================================================================================
    Link block, by pointer.
 =======================================================================================================================
 */
void LinkPtr(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	ptr = GetCompiledBlockPtr(block_queue_head->target_2_pc);
	uint32	offset = (uint32) ptr - 1 - (uint32) (&RecompCode[block_queue_head->jmp_to_target_2_code_addr]);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/* JMP Short */

	if  ( ((offset < 127) || (offset > 0xffffff81))
        && ( *(uint8 *)&RecompCode[block_queue_head->jmp_to_target_2_code_addr] == 0x90   ) 
        && ( *(uint8 *)&RecompCode[block_queue_head->jmp_to_target_2_code_addr-1] & 0x80  )
        && ( *(uint8 *)&RecompCode[block_queue_head->jmp_to_target_2_code_addr-2] == 0x0F )
        )
        
    {
        (*((uint8 *) (&RecompCode[block_queue_head->jmp_to_target_2_code_addr-2]))) = 
            0x70|(*(uint8 *)&RecompCode[block_queue_head->jmp_to_target_2_code_addr-1]^0x80);
		(*((uint8 *) (&RecompCode[block_queue_head->jmp_to_target_2_code_addr-1]))) = (uint8) offset+1;
	}
	else if((offset < 127) || (offset > 0xffffff81))
	{
        (*((unsigned _int8 *) (&RecompCode[block_queue_head->jmp_to_target_2_code_addr - 1]))) = 0xEB;
		(*((unsigned _int8 *) (&RecompCode[block_queue_head->jmp_to_target_2_code_addr]))) = (uint8) offset;
	}

	// JMP Long (Near)
	else

    {
		(*((_int32 *) (&RecompCode[block_queue_head->jmp_to_target_2_code_addr]))) = (_int32) ptr - 4 - (_int32) (&RecompCode[block_queue_head->jmp_to_target_2_code_addr]);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL IsBlockCompiled(uint32 pc)
{
	/*~~~~~~~~~~~~~~~~~*/
	uint32	maptopc = pc;
	/*~~~~~~~~~~~~~~~~~*/

	if(NOT_IN_KO_K1_SEG(maptopc))
	{
		ITLB_Error = FALSE;
		maptopc = TranslateITLBAddress(maptopc);
		if(ITLB_Error)
		{
			return FALSE;
		}
	}

	return(*(uint32 *) ((uint8 *) gHardwareState.sDYN_PC_LOOKUP[maptopc >> 16] + (uint16) maptopc)) ? TRUE : FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
uint32 GetCompiledBlockPtr(uint32 pc)
{
	/*~~~~~~~~~~~~~~~~~*/
	uint32	maptopc = pc;
	/*~~~~~~~~~~~~~~~~~*/

	if(NOT_IN_KO_K1_SEG(maptopc))
	{
		ITLB_Error = FALSE;
		maptopc = TranslateITLBAddress(maptopc);
		if(ITLB_Error)
		{
			return 0;
		}
	}

	return *(uint32 *) ((uint8 *) gHardwareState.sDYN_PC_LOOKUP[maptopc >> 16] + (uint16) maptopc);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
uint32 Dyna_Compile_Block(void)
{
	/*~~~~~~~~*/
	uint32	blk;
	/*~~~~~~~~*/

	DO_PROFILIER_COMPILER 
 
        
    compilerstatus.IsFirstFPUInstructionToTest = TRUE;
		blk = Dyna_Compile_Single_Block();
		ITLB_Error = FALSE;

	DO_PROFILIER_R4300I
	return blk;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL IsTargetPcInTheSame4KB(uint32 pc, uint32 target)
{
	/*
	 * static int k = 0; if (k == 0) { InitLogDyna(); k = 1; } LogDyna("%08X %08X\n",
	 * pc, target);
	 */
	return(pc / 0x1000 == target / 0x1000) ? TRUE : FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Check_And_Invalidate_Compiled_Blocks_By_DMA(uint32 startaddr, uint32 len, char *op)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	register uint32 addr = startaddr;
	register uint32 endaddr = startaddr + len;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(startaddr <= 0x80000000 || startaddr >= 0x80000000 + current_rdram_size)
	{
		return;
	}

	if(endaddr >= 0x80000000 + current_rdram_size) endaddr = 0x80000000 + current_rdram_size;

	addr &= 0x007FFFFF;
	endaddr &= 0x00FFFFFF;

	for(; addr < endaddr; addr += 4)
	{
		if(*(uint32 *) (RDRAM_Copy + addr) != DUMMYOPCODE)
		{
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			uint32	offset = (addr & 0xFFFFF000) | 0x80000000;
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			UnprotectBlock(offset);
			Invalidate4KBlock(offset, op, NOCHECKTYPE, 0);
			addr = (addr & 0xFFFFF000) + 0x1000;
			if(addr >= endaddr)
				break;
			else
			{
				addr -= 4;
				continue;
			}
		}
	}
}
