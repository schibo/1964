
// regcache.c: Register caching routines. Map/unmap MIPS GPRs to/from x86 GPRs.


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

//#define DEBUG_REGCACHE
//#define SAFE_32_TO_64 1
//#define SAFE_64_TO_32 1
//#define SAFE_FLUSHING 1
//#define SAFE_PUSHPOP 1



#include "../stdafx.h"
#include "../Registers.h"

extern N64::CRegisters r;

#define IT_IS_HIGHWORD	- 2
#define IT_IS_UNUSED	- 1
#define NUM_X86REGS		8
#define R0_COMPENSATION

void				FlushRegister(int To_XMM_Or_Memory, int k);
void				WriteBackDirty(int To_XMM_Or_Memory, _int8 k);
void				FlushOneConstant(int To_XMM_Or_Memory, int k);
void				FlushOneConstantLo(int k);
void				FlushOneConstantHi(int To_XMM_Or_Memory, int k);
void				MapOneConstantToRegister(x86regtyp *Conditions, int The2ndOneNotToFlush, int The3rdOneNotToFlush);
extern FlushedMap	FlushedRegistersMap[NUM_CONSTS];

x86regtyp			x86reg[8];
extern char			*r4300i_RegNames[];
uint32		ThisYear;
MapConstant ConstMap[NUM_CONSTS];
x86paramstyp	x86params;


uint32 Registers_In_Use;
uint32 Map_Capacity;


// Returns an x86 register corresponding to the MSB of the 
// Registers_In_Use variable. 
uint8 Find_Available_Register[256] = {
    -1, 
    0,
    1, 1, 
    2, 2, 2, 2, 
    3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,

    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,

    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,

    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

typedef struct StackAllocation
{
    int IsPushed;
    int IsInARegister;
    int OldStatus;
} StackAllocation;

StackAllocation PopFlag[8];


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void InitRegisterMap(void)
{
	int		k;
	const int	Num_x86regs = NUM_X86REGS;

    
    Registers_In_Use = 0;
    
    Map_Capacity = 0xffffffff;
    //Don't allow these registers to be used:
    Map_Capacity ^= (1<<Reg_ESP);
    Map_Capacity ^= (1<<Reg_EBP);


    ThisYear = 2001;
	for(k = 0; k < Num_x86regs; k++)
	{
		x86reg[k].mips_reg = -1;
		x86reg[k].HiWordLoc = -1;
		x86reg[k].BirthDate = ThisYear;
		x86reg[k].Is32bit = 0;		/* If 1, this register uses 32bit */

		x86reg[k].IsDirty = 0;		/* If 1, this register will eventually need */

		/*
		 * to be unmapped (written back to memory-- ?
		 * "flushed")
		 */
		ConstMap[k].IsMapped = 0;
		ConstMap[k].value = 0;
		FlushedRegistersMap[k].Is32bit = 0;
        ConstMap[k].FinalAddressUsedAt = 0xffffffff;
	}

	for(k = NUM_X86REGS; k < NUM_CONSTS; k++)
	{
		ConstMap[k].IsMapped = 0;
		ConstMap[k].value = 0;
		FlushedRegistersMap[k].Is32bit = 0;
		ConstMap[k].FinalAddressUsedAt = 0xffffffff;
	}

    for (k=0;k<NUM_X86REGS;k++)
    {
        PopFlag[k].IsInARegister = -1;
        PopFlag[k].IsPushed = 0;
    }

#ifndef NO_CONSTS
	ConstMap[0].IsMapped = 1;
#endif
	FlushedRegistersMap[0].Is32bit = 1;
}

//--------------------------------------------------------------
//Don't use these registers for mapping. 100 is a protected reg.
//
int ItIsARegisterNotToUse(int k)
{
    if ((k==Reg_ESP)|| (k==Reg_EBP))
        return 1;
    else 

    switch (x86reg[k].mips_reg)
    {
        case 100:       // protected reg
        case 126:       // temp reg
            return 1;
            break;
        
        default:
            return 0;
            break;
    }
}

void StoreGPR_LO(int To_XMM_Or_Memory, int k)
{
	if(x86reg[k].mips_reg == 0);
	else
	{
		FetchEBP_Params(x86reg[k].mips_reg);
		MOV_RegToMemory(1, k, x86params.ModRM, x86params.Address);
        ConstMap[k].WasFlushedInBlock = 1;
	}
}

void StoreGPR_HI(int To_XMM_Or_Memory, int k)
{
	if(x86reg[k].mips_reg == 0); \
	else
	{
		FetchEBP_Params(x86reg[k].mips_reg);
		MOV_RegToMemory(1, (uint8) x86reg[k].HiWordLoc, x86params.ModRM, 4 + x86params.Address);
	}
}

void StoreImm_LO(int To_XMM_Or_Memory, int k, int i)
{
	if (k==0);
    else if(k < 17)
		MOV_ImmToMemory(1, ModRM_disp8_EBP, (-128 + ((k-1) << 3)), i);

    else if(k < 33)
		MOV_ImmToMemory(1, ModRM_disp8_EBP, (((k-1) - 16) << 3), i);
	else

		MOV_ImmToMemory(1, ModRM_disp32_EBP, (((k-1) - 16) << 3), i);

    ConstMap[k].WasFlushedInBlock = 1;
}

void StoreImm_HI(int k, int i)
{
	if (k==0);

    else if(k < 17)
		MOV_ImmToMemory(1, ModRM_disp8_EBP, (-124 + ((k-1) << 3)), i);

    else if(k < 33)
		MOV_ImmToMemory(1, ModRM_disp8_EBP, 4 + (((k-1) - 16) << 3), i);

    else
		MOV_ImmToMemory(1, ModRM_disp32_EBP, 4 + (((k-1) - 16) << 3), i);
}



/*
 =======================================================================================================================
    Check if a register is mapped as a 32bit register
 =======================================================================================================================
 */
int CheckIs32Bit(int mips_reg)
{
	_s32	k;
	const int	Num_x86regs = NUM_X86REGS;

	if(ConstMap[mips_reg].IsMapped) return(1);
	for(k = 0; k < Num_x86regs; k++)
	{
		if(ItIsARegisterNotToUse(k));
		else if(x86reg[k].mips_reg == mips_reg) /* if mapped */
		{
			return(x86reg[k].Is32bit);
		}
        else 
            return (FlushedRegistersMap[mips_reg].Is32bit);
	}

	return(0);
}

/*
 =======================================================================================================================
    Check if a register is in need of be written back to memory (It is dirty)
 =======================================================================================================================
 */
int CheckIsDirty(int mips_reg)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	_s32	k;
	const int	Num_x86regs = NUM_X86REGS;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(k = 0; k < Num_x86regs; k++)
	{
		if(ItIsARegisterNotToUse(k));
		else if(x86reg[k].mips_reg == mips_reg) /* if mapped */
		{
			return(x86reg[k].IsDirty);
		}
	}

	return(0);
}

/*
 =======================================================================================================================
    If a MIPS?reg is mapped to an x86 reg, tell us where it is.
 =======================================================================================================================
 */
int CheckWhereIsMipsReg(int mips_reg)
{
	if(x86reg[Reg_EDI].mips_reg == mips_reg) return(Reg_EDI);
	if(x86reg[Reg_ESI].mips_reg == mips_reg) return(Reg_ESI);
	if(x86reg[Reg_EBX].mips_reg == mips_reg) return(Reg_EBX);
	if(x86reg[Reg_EDX].mips_reg == mips_reg) return(Reg_EDX);
	if(x86reg[Reg_ECX].mips_reg == mips_reg) return(Reg_ECX);
	if(x86reg[Reg_EAX].mips_reg == mips_reg) return(Reg_EAX);

	return(-1);
}

int FindFreeRegister()
{
	if(x86reg[Reg_ECX].mips_reg == IT_IS_UNUSED) return(Reg_ECX);
	if(x86reg[Reg_EDX].mips_reg == IT_IS_UNUSED) return(Reg_EDX);
	if(x86reg[Reg_EBX].mips_reg == IT_IS_UNUSED) return(Reg_EBX);
	if(x86reg[Reg_ESI].mips_reg == IT_IS_UNUSED) return(Reg_ESI);
	if(x86reg[Reg_EDI].mips_reg == IT_IS_UNUSED) return(Reg_EDI);
	if(x86reg[Reg_EAX].mips_reg == IT_IS_UNUSED) return(Reg_EAX);

	return(-1);
}



/*
 =======================================================================================================================
    This function gets the params needed to pass to the x86.c functions ?
    when loading/storing MIPS?GPR registers from/to the array in memory. ?
    EBP points to the middle of the GPR array..pretty clever, eh ;)
 =======================================================================================================================
 */
void FetchEBP_Params(int mips_reg)
{
	if (mips_reg == 0)
		DisplayError("Fetch Error");
	else

	if(mips_reg < 17)
	{
		// negative 8bit displacement 
		x86params.ModRM = ModRM_disp8_EBP;
		x86params.Address = -128 + ((mips_reg-1) << 3);
	}
	else if(mips_reg < 32)
	{
		// positive 8bit displacement 
		x86params.ModRM = ModRM_disp8_EBP;
		x86params.Address = (((mips_reg-1) - 16) << 3);
	}
	else
	{
		// positive 32bit displacement. Only GPR_HI/GPR_LO use this. 
		x86params.ModRM = ModRM_disp32;
		x86params.Address = (_u32) & gHWS_GPR(0) + (((mips_reg)) << 3);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void x86reg_Delete(int k)
{
	x86reg[k].mips_reg = IT_IS_UNUSED;
	x86reg[k].Is32bit = 0;
	x86reg[k].IsDirty = 0;
	x86reg[k].HiWordLoc = -1;
	x86reg[k].BirthDate = ThisYear;
}

x86regtyp	Targetx86reg[8];
MapConstant TargetConstMap[NUM_CONSTS];

/*
 =======================================================================================================================
    If the MIPS?register is mapped, this will free it without writing it back to memory.
 =======================================================================================================================
 */
void FreeMipsRegister(int mips_reg)
{
	/*~~~~~*/
	int temp;
	/*~~~~~*/

	ConstMap[mips_reg].IsMapped = 0;
	if((temp = CheckWhereIsMipsReg(mips_reg)) > -1)
	{
		x86reg[temp].IsDirty = 0;
		FlushRegister(FLUSH_TO_MEMORY, temp);
	}
}

/*
 =======================================================================================================================
    Map a constant to our constant array. Constants are known values that don't require a register. Examples: r0 is
    always 0, the LUI instruction always generates constants. 1964 does a lot of constant detection.
 =======================================================================================================================
 */
void MapConst(x86regtyp *xMAP, int value)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#ifndef NO_CONSTS
	int where = CheckWhereIsMipsReg(xMAP->mips_reg);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(where > -1)
	{
		x86reg[where].IsDirty = 0;
		FlushRegister(FLUSH_TO_MEMORY, where);
	}

    ConstMap[xMAP->mips_reg].IsMapped = 1;
	ConstMap[xMAP->mips_reg].value = value;
	ConstMap[0].value = 0;
#else
	if(xMAP->mips_reg != 0)
	{
		xMAP->IsDirty = 1;	/* bug fix */
		xMAP->Is32bit = 1;
		xMAP->NoNeedToLoadTheLo = 1;
		MapRegister(xMAP, 99, 99);
		MOV_ImmToReg(xMAP->x86reg, value);
	}
#endif
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void FlushOneConstantLo(int k)
{
	int where;

	if(k == 0) return;
	if(ConstMap[k].IsMapped == 1)
	{
		FlushedRegistersMap[k].Is32bit = 1;
		if(k == 0) return;
		where = CheckWhereIsMipsReg(k);
		if(where > -1)
		{
#ifdef DEBUG_REGCACHE
			MessageBox(0, "That's not right..", "", 0);
#endif
			x86reg[where].IsDirty = 0;
			FlushRegister(FLUSH_TO_MEMORY, where);
		}
		{
			
/**************************************************************/                        
            _u32	off = (_u32) & gHWS_GPR(0) + (k << 3);
			_s32	i = (_s32) ConstMap[k].value;
/**************************************************************/            

			StoreImm_LO(0, k, i);

#ifndef NO_CONSTS
			ConstMap[0].IsMapped = 1;
#endif
		}
	}
}

void FlushOneConstantHi(int To_XMM_Or_Memory, int k)
{
	int where;

	if(k == 0) return;
	if(ConstMap[k].IsMapped == 1)
	{
		FlushedRegistersMap[k].Is32bit = 1;
		if(k == 0) return;
		ConstMap[k].IsMapped = 0;
		where = CheckWhereIsMipsReg(k);
		if(where > -1)
		{
#ifdef DEBUG_REGCACHE
			DisplayError("Odd");
#endif
			x86reg[where].IsDirty = 0;
			FlushRegister(To_XMM_Or_Memory, where);
		}
		{
			
			_s32	i = (_s32) ConstMap[k].value;

			_asm sar i, 31

			if(currentromoptions.Assume_32bit == ASSUME_32BIT_NO)
				StoreImm_HI(k, i);
#ifndef NO_CONSTS
			ConstMap[0].IsMapped = 1;
#endif
		}
	}
}

void FlushOneConstant(int To_XMM_Or_Memory, int k)
{
	FlushOneConstantLo(k);
	FlushOneConstantHi(To_XMM_Or_Memory, k);
}


/*
 =======================================================================================================================
    Store our constants to the mips gpr array in memory.
 =======================================================================================================================
 */
void FlushConstants(int To_XMM_Or_Memory)
{
	int k;

	for(k = 1; k < NUM_CONSTS; k++)
	{
		FlushOneConstantLo(k);
	}

	//Lo and Hi are split up to avoid some bank conflicts
	for(k = 1; k < NUM_CONSTS; k++)
	{
		FlushOneConstantHi(To_XMM_Or_Memory, k);
	}

#ifndef NO_CONSTS
	ConstMap[0].IsMapped = 1;
#endif
	ConstMap[0].value = 0;
}

/*
 =======================================================================================================================
    Store the dirty register to the mips gpr array in memory.
 =======================================================================================================================
 */
void WriteBackDirty(int To_XMM_Or_Memory, _int8 k)
{
	/* 32 bit register */
	if((x86reg[k].HiWordLoc == k))
	{
		if(x86reg[k].Is32bit != 1) DisplayError("Bug");

		StoreGPR_LO(To_XMM_Or_Memory, k);

		if((currentromoptions.Assume_32bit == ASSUME_32BIT_NO) /*&& 
            ((uint32)ConstMap[k].FinalAddressUsedAt <= (uint32)gHWS_pc)
            || ((uint32)ConstMap[k].FinalAddressUsedAt == 0xffffffff)*/)
		{
			SAR_RegByImm(k, 31);
			StoreGPR_HI(To_XMM_Or_Memory, k);
		}

		FlushedRegistersMap[x86reg[k].mips_reg].Is32bit = 1;
	}
	else
	/* 64 bit register */
	{
		if(x86reg[k].Is32bit == 1) DisplayError("Bug");

		StoreGPR_LO(To_XMM_Or_Memory, k);

		if
		(
		(currentromoptions.Assume_32bit == ASSUME_32BIT_NO)
		)
		{
            StoreGPR_HI(To_XMM_Or_Memory, k);
		}

		FlushedRegistersMap[x86reg[k].mips_reg].Is32bit = 0;
	}
}

/*
 =======================================================================================================================
    Unmaps a mips register from our x86reg[] register map. If the mips reg is dirty, it will be stored back to memory.
 =======================================================================================================================
 */
void FlushRegister(int To_XMM_Or_Memory, int k)
{
#ifdef DEBUG_REGCACHE
	/* paranoid error check */
	if(x86reg[k].HiWordLoc == -1) MessageBox(0, "FlushRegister: The HiWord was not set!", "", 0);
#endif
#ifdef R0_COMPENSATION
	/*
	 * Until we don't map all r0's, we'll need this check ?
	 * if (ConstMap[0].IsMapped == 0) DisplayError("How did Const[0] get
	 * unmapped???");
	 */
	if(x86reg[k].mips_reg == 0)
        x86reg[k].IsDirty = 0;
#endif
	if(x86reg[k].IsDirty == 1)
	    WriteBackDirty(To_XMM_Or_Memory, (_s8) k);


	x86reg_Delete(x86reg[k].HiWordLoc);
	x86reg_Delete(k);
}

/*
 =======================================================================================================================
    This function is just a combination of FlushRegister() and WriteBackDirty(). ?
    It was written so that FlushAllRegisters() can interleave the register flushing ?
    so that instructions will become paired for the processor.
 =======================================================================================================================
 */
void WriteBackDirtyLoOrHi(int To_XMM_Or_Memory, _int8 k, int Lo)
{
	/* 32 bit register */
#ifdef R0_COMPENSATION
	/*
	 * Until we don't map all r0's, we'll need this check ?
	 * if (ConstMap[0].IsMapped == 0) DisplayError("How did Const[0] get
	 * unmapped???");
	 */
	if(x86reg[k].mips_reg == 0) x86reg[k].IsDirty = 0;
#endif
	if(x86reg[k].IsDirty == 1)
	{
		if((x86reg[k].HiWordLoc == k))
		{
			if(x86reg[k].Is32bit != 1) DisplayError("Bug");

			if(Lo == 0)
			{
				StoreGPR_LO(To_XMM_Or_Memory, k);
			}
			else if(Lo == 1)
			{
				if(currentromoptions.Assume_32bit == ASSUME_32BIT_NO) 
					SAR_RegByImm(k, 31);
			}
			else if(currentromoptions.Assume_32bit == ASSUME_32BIT_NO)
			{
				if(Lo == 2)
					StoreGPR_HI(To_XMM_Or_Memory, k);
			}

			FlushedRegistersMap[x86reg[k].mips_reg].Is32bit = 1;
		}
		else
		/* 64 bit register */
		{
			if(x86reg[k].Is32bit == 1) DisplayError("Bug");

			if(!Lo)
			{
				StoreGPR_LO(To_XMM_Or_Memory, k);
			}
			else if(currentromoptions.Assume_32bit == ASSUME_32BIT_NO)
			{
				if(Lo == 2) 
					StoreGPR_HI(To_XMM_Or_Memory, k);
			}

			FlushedRegistersMap[x86reg[k].mips_reg].Is32bit = 0;
		}
	}
}

/*
 =======================================================================================================================
    Function: FlushAllRegisters() Purpose: This function is used when you want to "flush" all the General Purpose
    Registers (GPR) that are mapped so that no registers are mapped. An unmapped array entry = -1. input: output:
 =======================================================================================================================
 */
extern void SSE_FlushAllRegisters(int To_XMM_Or_Memory);
void FlushAllRegisters(int To_XMM_Or_Memory)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	_int8	k;
	int		Lo = 0;
	int		iterations = 3;
	const	int Num_x86regs = NUM_X86REGS;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    SSE_FlushAllRegisters(FLUSH_TO_MEMORY);

	ThisYear = 2001;

	/* TODO: __LO, __HI flush 64bit. */
	if(currentromoptions.Assume_32bit == ASSUME_32BIT_NO) iterations = 3;
	for(Lo = 0; Lo < iterations; Lo++)
	{
		for(k = 0; k < Num_x86regs; k++)
		{
			if(ItIsARegisterNotToUse(k));
			else if(x86reg[k].mips_reg > -1)
            {
                WriteBackDirtyLoOrHi(To_XMM_Or_Memory, k, Lo);
            
            }

			if (Lo == 0)
            {
                FlushOneConstantLo(k+1);
            }
			else if( Lo == 1)
            {
                FlushOneConstantLo(k+8+1);
            }
			else if( Lo == 2)
            {
                FlushOneConstantLo(k+16+1);
            }

		}
	}




	for(k = (Num_x86regs*3)+1; k < NUM_CONSTS; k++)
	{
            FlushOneConstantLo(k);
       	    FlushOneConstantHi(To_XMM_Or_Memory, k);
	}


	for(k = 0; k < Num_x86regs; k++)
	{
		if(ItIsARegisterNotToUse(k));
		else if(x86reg[k].mips_reg > -1)
		{
			x86reg_Delete(x86reg[k].HiWordLoc);
			x86reg_Delete(k);
		}
	}



	//Lo and Hi are split up to avoid some bank conflicts
	for(k = 1; k < (Num_x86regs*3)+1; k++)
	{
        FlushOneConstantHi(To_XMM_Or_Memory, k);
	}


	for(k = 1; k < NUM_CONSTS; k++) 
    {
        FlushedRegistersMap[k].Is32bit = 0;
        ConstMap[k].WasFlushedInBlock = 0;
    }


#ifndef NO_CONSTS
	ConstMap[0].IsMapped = 1;
#endif
	ConstMap[0].value = 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void PushMap(void)	/* Alternative to PUSHAD. */

/* This is ok to use in the regcache, but don't use it in the opcode debugger code! £Use PUSHAD() in opcode debugger code. */
{
#ifdef SAFE_PUSHPOP
	PUSHAD();
	return;
#endif
	PUSH_RegIfMapped(Reg_EAX, 0, -1);
	PUSH_RegIfMapped(Reg_ECX, 0, -1);
	PUSH_RegIfMapped(Reg_EDX, 0, -1);
}


extern x86regtyp xRD[1];
extern x86regtyp xRT[1];
extern x86regtyp xRS[1];
void DoItNow2(int k, int lo, int TryStoringToARegisterFirst, int RegisterToProtect)
{
    int FreeRegister;


//    TryStoringToARegisterFirst = 0; //Debugs this function. lwc1 messing up in mario with this set to 1.

    //Important NOTE: Do NOT try to store to a register if an opcode needs 6 registers.
    //Be VERY careful to know your maximum register allocation. This includes protected registers.

    if (TryStoringToARegisterFirst)
    {
        FreeRegister = FindFreeRegister();

        if ((FreeRegister > -1) && (FreeRegister != RegisterToProtect) && (FreeRegister != k))
        {
            MOV_Reg2ToReg1(FreeRegister, k);
            //transfer the map
            memcpy((uint8*)&x86reg[FreeRegister], (uint8*)&x86reg[k], sizeof(x86regtyp));


            //if we moved a hiword
            if (x86reg[k].mips_reg == IT_IS_HIGHWORD)
            {
                x86reg[lo].HiWordLoc = FreeRegister;
                if (xRT->HiWordLoc == k) xRT->HiWordLoc = FreeRegister;
                if (xRS->HiWordLoc == k) xRS->HiWordLoc = FreeRegister;
                if (xRD->HiWordLoc == k) xRD->HiWordLoc = FreeRegister;
            }
            else //we moved a loword
            {
                if (x86reg[k].HiWordLoc == k)
                    x86reg[FreeRegister].HiWordLoc = FreeRegister;

                if (xRT->x86reg == k) {
                    xRT->x86reg = FreeRegister;
                    xRT->HiWordLoc = x86reg[FreeRegister].HiWordLoc;
                }
                if (xRS->x86reg == k) {
                    xRS->x86reg = FreeRegister;
                    xRS->HiWordLoc = x86reg[FreeRegister].HiWordLoc;
                }
                if (xRD->x86reg == k) {
                    xRD->x86reg = FreeRegister;
                    xRD->HiWordLoc = x86reg[FreeRegister].HiWordLoc;
                }
            }

            x86reg[FreeRegister].x86reg = FreeRegister;

            x86reg_Delete(k);

            PopFlag[k].IsPushed = 0;
            PopFlag[k].IsInARegister = -1;
        }
        else
            PUSH_RegToStack((uint8) k);
    }
    else
        PUSH_RegToStack((uint8) k);
}

void PUSH_RegIfMapped(int k, int TryStoringToARegisterFirst, int RegisterToProtect)
{
	int lo;
    int found=0;
 
	PopFlag[k].IsPushed = 1;
    PopFlag[k].IsInARegister = -1;

	if(x86reg[k].mips_reg != -1)
	{
		if(currentromoptions.Advanced_Block_Analysis == USEBLOCKANALYSIS_YES)
		{
            //is it a protected register needing to be preserved?
            if ((x86reg[k].mips_reg == 100) || (x86reg[k].mips_reg == 126))
            {
                DoItNow2(k, 0, 0, -1);
            }
            //or is it a hiword needing to be preserved?
            else if(x86reg[k].mips_reg == IT_IS_HIGHWORD)
			{
				//search for the corresponding loword
                for(lo = 0; lo < 8; lo++)
				{
					if(ItIsARegisterNotToUse(lo));
					else if(x86reg[lo].mips_reg > -1)
                    {
                        //is it this one?
                        if (x86reg[lo].HiWordLoc == k)
					    {
                            //is this mips register used in the remainder of the block or in this instruction?
                            if(ConstMap[x86reg[lo].mips_reg].FinalAddressUsedAt >= gHWS_pc)
                            {
                                //move this hiword to temporary storage 
                                DoItNow2(k, lo, TryStoringToARegisterFirst, RegisterToProtect);
                            }
                            //otherwise, flush it. 
						    else
						    {
							    PopFlag[k].IsPushed = 0;
                                PopFlag[k].IsInARegister = -1;
							    FlushRegister(FLUSH_TO_MEMORY, lo);
						    }

                            found = 1;
						    lo = 20;	/* exit loop */
                        }
					}
				}
                if (!found)
                    DisplayError("PUSH_RegIfMapped2():  loword not found!");
			}
            //or is it a loword needing to be preserved?
            else if((x86reg[k].mips_reg > -1) && (ConstMap[x86reg[k].mips_reg].FinalAddressUsedAt >= gHWS_pc))
				DoItNow2(k, k, TryStoringToARegisterFirst, RegisterToProtect);
            //otherwise, it's something that we can free
            else
			{
                PopFlag[k].IsPushed = 0;
                PopFlag[k].IsInARegister = -1;
			    FlushRegister(FLUSH_TO_MEMORY, k);
			}
		}
		else
			PUSH_RegToStack((uint8) k);
	}
	else
		PopFlag[k].IsPushed = 0;
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void POP_RegIfMapped(int k)
{
	if(PopFlag[k].IsPushed)
        POP_RegFromStack((uint8) k);
    else if (PopFlag[k].IsInARegister > -1)
    {
        MOV_Reg2ToReg1(k, PopFlag[k].IsInARegister);
        
        if (x86reg[ PopFlag[k].IsInARegister ].mips_reg == 126)
        x86reg[ PopFlag[k].IsInARegister ].mips_reg = IT_IS_UNUSED;
    }

    PopFlag[k].IsPushed = 0;
    PopFlag[k].IsInARegister = -1;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void PopMap(void)	/* Alternative to POPAD */
{
    int _esp=0;

#ifdef SAFE_PUSHPOP
	POPAD();
	return;
#endif

    if(PopFlag[Reg_EDX].IsPushed) 
    {
        PopFlag[Reg_EDX].IsPushed = 0;
        //mov edx, [esp]
        WC8(0x8B); WC8(0x14); WC8(0x24);
        _esp += 4;
    }
    
//      mov edx, [esp]
//      mov ecx, [esp+4]
//      mov eax, [esp+8]

    if(PopFlag[Reg_ECX].IsPushed) 
    {
        PopFlag[Reg_ECX].IsPushed = 0;
        //mov ecx, [esp+4]
        WC8(0x8B); WC8(ModRM_disp8_SIB|(Reg_ECX<<3)); WC8(0x24); WC8(_esp);
        _esp += 4;
    }

    if(PopFlag[Reg_EAX].IsPushed) 
    {
        PopFlag[Reg_EAX].IsPushed = 0;
        
        //mov eax, [esp+8]
        WC8(0x8B); WC8(ModRM_disp8_SIB|(Reg_EAX<<3)); WC8(0x24); WC8(_esp);
        _esp += 4;
    }

    ADD_ImmToReg(Reg_ESP, _esp, 0);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int FlushOneButNotThese3(int To_XMM_Or_Memory, int The1stOneNotToFlush, int The2ndOneNotToFlush, int The3rdOneNotToFlush)
{
	const _u8	Num_x86regs = NUM_X86REGS;
	_u32		paranoid = 0;
	_u32		init = 0;
	_u32		EarliestYear;
	__int32		k;
	__int32		MarkedForDeletion;

	/* Flush by least recently used */
	for(k = Num_x86regs - 1; k >= 0; k--)
	{
		if(ItIsARegisterNotToUse(k));
		else if
			(
				(x86reg[k].mips_reg > -1)
			&&	(x86reg[k].mips_reg != The1stOneNotToFlush)
			&&	(x86reg[k].mips_reg != The2ndOneNotToFlush)
			&&	(x86reg[k].mips_reg != The3rdOneNotToFlush)
			)
		{
#ifdef DEBUG_REGCACHE
			if(x86reg[k].HiWordLoc == -1) DisplayError("FlushOne: The HiWord was not set!");
#endif
			if(ConstMap[x86reg[k].mips_reg].FinalAddressUsedAt <= gHWS_pc)
			{
                if(currentromoptions.Advanced_Block_Analysis == USEBLOCKANALYSIS_YES)
				{
					MarkedForDeletion = k;
					EarliestYear = x86reg[k].BirthDate;
					k = -1; /* exits */
				}
				else
					goto _next;
			}
			else
			{
_next:
				if(init == 0)
				{
					init = 1;
					MarkedForDeletion = k;
					EarliestYear = x86reg[k].BirthDate;
				}
				else if(x86reg[k].mips_reg == 0)
				{
					MarkedForDeletion = k;
					EarliestYear = x86reg[k].BirthDate;
					k = -1; /* exits */
				}
				else if(x86reg[k].BirthDate <= EarliestYear)
				{
					/*
					 * If they have same birth year, a nondirty has ?
					 * priority over a dirty for flushing
					 */
					if(x86reg[k].BirthDate == EarliestYear)
					{
						if((x86reg[MarkedForDeletion].IsDirty == 1) && (x86reg[k].IsDirty == 0))
							MarkedForDeletion = k;
					}
					else
					{
						MarkedForDeletion = k;
						EarliestYear = x86reg[k].BirthDate;
					}
				}
			}

			paranoid = 1;
		}
	}

	if(ConstMap[x86reg[MarkedForDeletion].mips_reg].IsMapped == 1)
	{
#ifdef DEBUG_REGCACHE
		if(x86reg[MarkedForDeletion].mips_reg != 0)
			DisplayError("This should not happen. Mips = %d", x86reg[MarkedForDeletion].mips_reg);
#endif
		FlushOneConstantLo(x86reg[MarkedForDeletion].mips_reg);	/* Unmaps corr. reg also */
		FlushOneConstantHi(To_XMM_Or_Memory, x86reg[MarkedForDeletion].mips_reg);	/* Unmaps corr. reg also */
	}

#ifdef R0_COMPENSATION
	if(x86reg[MarkedForDeletion].mips_reg == 0) 
		x86reg[MarkedForDeletion].IsDirty = 0;
#endif

	FlushRegister(To_XMM_Or_Memory, MarkedForDeletion);
    if(paranoid == 0)
    {
        MarkedForDeletion = -1; //Failed.
    }

    return (MarkedForDeletion);
}