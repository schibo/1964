
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

#define IT_IS_HIGHWORD	- 2
#define IT_IS_UNUSED	- 1
#define NUM_fpuregS		8
#define SSE_NUM_CONSTS 32

MapConstant SSE_ConstMap[SSE_NUM_CONSTS];
MapConstant Temp_SSE_ConstMap[SSE_NUM_CONSTS];
FlushedMap  SSE_FlushedRegistersMap[NUM_fpuregS];
FlushedMap  Temp_SSE_FlushedRegistersMap[NUM_fpuregS];
x86regtyp   Temp_SSE_fpureg[NUM_fpuregS];

#define SSE_ItIsARegisterNotToUse(k)	((k == Reg_EBP) || (k == Reg_ESP) || (fpureg[k].mips_reg == 100) || (fpureg[k].mips_reg == 126))


#define LoadFPR_LO(To_XMM_Or_Memory, k) \
	MOVSS_MemoryToReg(k, ModRM_disp32, (unsigned long) &gHardwareState.fpr32[fpureg[k].mips_reg<<Experiment]);

#define StoreFPR_LO(To_XMM_Or_Memory, k) \
    MOVSS_RegToMemory(k, ModRM_disp32, (unsigned long) &gHardwareState.fpr32[fpureg[k].mips_reg<<Experiment]);

#define LoadFPR_HI(k) \
MessageBox(0, "StoreFPR_LO: This shouldn't be touched yet.", "", 0); \
    if(currentromoptions.Assume_32bit == ASSUME_32BIT_NO) \
	{ \
        if(fpureg[k].mips_reg == 0) \
			XOR_Reg2ToReg1((_u8) fpureg[k].HiWordLoc, (_u8) fpureg[k].HiWordLoc); \
		else \
		{ \
			FetchEBP_Params(fpureg[k].mips_reg); \
			MOV_MemoryToReg((uint8) fpureg[k].HiWordLoc, x86params.ModRM, 4 + x86params.Address); \
		} \
	} \
	else \
	{ \
		MOV_Reg2ToReg1(fpureg[k].HiWordLoc, (uint8) k); \
		SAR_RegByImm(fpureg[k].HiWordLoc, 31); \
	}

#define StoreFPR_HI(To_XMM_Or_Memory, k) \
	{ \
        MessageBox(0, "StoreFPR_HI: This shouldn't be touched yet.", "", 0); \
        if(fpureg[k].mips_reg == 0); \
		else \
		{ \
			FetchEBP_Params(fpureg[k].mips_reg); \
			MOV_RegToMemory(1, (uint8) fpureg[k].HiWordLoc, x86params.ModRM, 4 + x86params.Address); \
		} \
	}

#define SSE_StoreImm_LO(To_XMM_Or_Memory, k) \
	/*\
    { \
		if (k==0);	\
        else {		\
		if(k < 17) \
		{ \
			MOV_ImmToMemory(1, ModRM_disp8_EBP, (-128 + ((k-1) << 3)), i); \
		} \
		else if(k < 33) \
		{ \
			MOV_ImmToMemory(1, ModRM_disp8_EBP, (((k-1) - 16) << 3), i); \
		} \
		else \
		{		\
			MOV_ImmToMemory(1, ModRM_disp32_EBP, (((k-1) - 16) << 3), i);	\
        }   SSE_ConstMap[k].WasFlushedInBlock = 1; \
        }	\
	}*/



#define SSE_StoreImm_HI(k) \
	/*{ \
		if (k==0);	\
		else		\
		if(k < 17) \
		{ \
			MOV_ImmToMemory(1, ModRM_disp8_EBP, (-124 + ((k-1) << 3)), i); \
		} \
		else if(k < 33) \
		{ \
			MOV_ImmToMemory(1, ModRM_disp8_EBP, 4 + (((k-1) - 16) << 3), i); \
		} \
		else \
		{ \
			MOV_ImmToMemory(1, ModRM_disp32_EBP, 4 + (((k-1) - 16) << 3), i); \
		} \
	}*/




void	SSE_FlushRegister(int To_XMM_Or_Memory, int k);
void	SSE_WriteBackDirty(int To_XMM_Or_Memory, _int8 k);
void	SSE_FlushOneConstant(int To_XMM_Or_Memory, int k);
void	SSE_FlushOneConstantLo(int k);
void	SSE_FlushOneConstantHi(int To_XMM_Or_Memory, int k);
void	SSE_MapOneConstantToRegister(x86regtyp *Conditions, int The2ndOneNotToFlush, int The3rdOneNotToFlush);
int		SSE_FlushOneButNotThese3 (int To_XMM_Or_Memory, int The1stOneNotToFlush, int The2ndOneNotToFlush, int The3rdOneNotToFlush);

x86regtyp			fpureg[8];

extern uint32 Experiment;
uint32 SSE_Map_Capacity;



/*
 =======================================================================================================================
 =======================================================================================================================
 */
extern x86regtyp xFS[1], xFT[1], xFD[1];
void SSE_InitRegisterMap(void)
{
	int		k;
	const int Num_fpuregs = NUM_fpuregS;

   
//    Registers_In_Use = 0;

//    SSE_Map_Capacity = 0xffffffff;
    //Don't allow these registers to be used:
//    Map_Capacity ^= (1<<Reg_ESP);
  //  Map_Capacity ^= (1<<Reg_EBP);


    ThisYear = 2001;
	for(k = 0; k < Num_fpuregs; k++)
	{
		fpureg[k].mips_reg = -1;
		fpureg[k].HiWordLoc = -1;
		fpureg[k].BirthDate = ThisYear;
		fpureg[k].Is32bit = 0;		/* If 1, this register uses 32bit */

		fpureg[k].IsDirty = 0;		/* If 1, this register will eventually need */

		SSE_FlushedRegistersMap[k].Is32bit = 0;
        SSE_ConstMap[k].FinalAddressUsedAt = 0xffffffff;
	}

	for(k = NUM_fpuregS; k < SSE_NUM_CONSTS; k++)
	{
		SSE_ConstMap[k].IsMapped = 0;
		SSE_ConstMap[k].value = 0;
		SSE_FlushedRegistersMap[k].Is32bit = 0;
		SSE_ConstMap[k].FinalAddressUsedAt = 0xffffffff;
	}

	memset( xFS, 0, sizeof( xFS));
	memset( xFT, 0, sizeof( xFT));
	memset( xFD, 0, sizeof( xFD));


}

/*
 =======================================================================================================================
    Check if a register is in need of be written back to memory (It is dirty)
 =======================================================================================================================
 */
int SSE_CheckIsDirty(int mips_reg)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	_s32	k;
	const int	Num_fpuregs = NUM_fpuregS;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(k = 0; k < Num_fpuregs; k++)
	{
		if(SSE_ItIsARegisterNotToUse(k));
		else if(fpureg[k].mips_reg == mips_reg) /* if mapped */
		{
			return(fpureg[k].IsDirty);
		}
	}

	return(0);
}

/*
 =======================================================================================================================
    If a MIPS?reg is mapped to an x86 reg, tell us where it is.
 =======================================================================================================================
 */
int SSE_CheckWhereIsMipsReg(int mips_reg)
{
	if(fpureg[Reg_EDI].mips_reg == mips_reg) return(Reg_EDI);
	if(fpureg[Reg_ESI].mips_reg == mips_reg) return(Reg_ESI);
	if(fpureg[Reg_EBX].mips_reg == mips_reg) return(Reg_EBX);
	if(fpureg[Reg_EDX].mips_reg == mips_reg) return(Reg_EDX);
	if(fpureg[Reg_ECX].mips_reg == mips_reg) return(Reg_ECX);
	if(fpureg[Reg_EAX].mips_reg == mips_reg) return(Reg_EAX);

	return(-1);
}

int SSE_FindFreeRegister()
{
	if(fpureg[Reg_ECX].mips_reg == IT_IS_UNUSED) return(Reg_ECX);
	if(fpureg[Reg_EDX].mips_reg == IT_IS_UNUSED) return(Reg_EDX);
	if(fpureg[Reg_EBX].mips_reg == IT_IS_UNUSED) return(Reg_EBX);
	if(fpureg[Reg_ESI].mips_reg == IT_IS_UNUSED) return(Reg_ESI);
	if(fpureg[Reg_EDI].mips_reg == IT_IS_UNUSED) return(Reg_EDI);
	if(fpureg[Reg_EAX].mips_reg == IT_IS_UNUSED) return(Reg_EAX);

	return(-1);
}



/*
 =======================================================================================================================
 =======================================================================================================================
 */
void fpureg_Delete(int k)
{
	fpureg[k].mips_reg = IT_IS_UNUSED;
	fpureg[k].Is32bit = 0;
	fpureg[k].IsDirty = 0;
	fpureg[k].HiWordLoc = -1;
	fpureg[k].BirthDate = ThisYear;
}


/*
 =======================================================================================================================
    Map a register as 32bit.
 =======================================================================================================================
 */
void SSE_Map32bit(int To_XMM_Or_Memory, int k, x86regtyp *Conditions, int keep2, int keep3)
{
	/*~~~~~~~~~~~~~~~*/
	int MapSuccess = 0;
	/*~~~~~~~~~~~~~~~*/

	while(MapSuccess == 0)
	{
        for(k = NUM_fpuregS - 1; k >= 0; k--)
		{
			if(SSE_ItIsARegisterNotToUse(k));
			else if(fpureg[k].mips_reg == IT_IS_UNUSED)
			{
__MapIt:				
                /* map it */
				fpureg[k].mips_reg = Conditions->mips_reg;
				fpureg[k].Is32bit = 1;
				fpureg[k].HiWordLoc = k;

				if(Conditions->IsDirty == 1) fpureg[k].IsDirty = 1;

				Conditions->x86reg = k;
				Conditions->HiWordLoc = k;	/* ok.. */

				if((Conditions->NoNeedToLoadTheLo == 0))
				{
					LoadFPR_LO(To_XMM_Or_Memory, k);
				}

				if(fpureg[k].HiWordLoc == -1) DisplayError("Do32bit: Hiword = -1 Not expected.");

				fpureg[k].BirthDate = ThisYear;
				MapSuccess = 1;

				return;
			}
		}

		if(MapSuccess == 0)
		{
			k = SSE_FlushOneButNotThese3(To_XMM_Or_Memory, 99, keep2, keep3);
                    if (k==-1)
                    {
                        DisplayError("SSE_Map32bit: Failed to map", "", 0);
                    }
                    else
                        goto __MapIt;

		}
	}
}

/*
 =======================================================================================================================
    Map a register as 64bit.
 =======================================================================================================================
 */
void SSE_Map64bit(int To_XMM_Or_Memory, int k, x86regtyp *Conditions, int The2ndOneNotToFlush, int The3rdOneNotToFlush)
{
	int TheFirstAvailableReg = 0;
	int NumRegsAvailable = 0;
	/* make sure we have 2 registers available for mapping */
	int MapSuccess = 0;

	MessageBox(0, "SSEMap64bit should not be touched yet", "", 0);
    while(MapSuccess == 0)
	{
		TheFirstAvailableReg = 0;
		NumRegsAvailable = 0;
		for(k = NUM_fpuregS - 1; k >= 0; k--)
		{
			if(fpureg[k].HiWordLoc == Reg_ESP) DisplayError("4: fpureg[%d]: Write to esp: cannot do that!", k);

			if(SSE_ItIsARegisterNotToUse(k));
			else if(fpureg[k].mips_reg == IT_IS_UNUSED)
			{
//_MapIt:				
                
                NumRegsAvailable += 1;
				if(NumRegsAvailable == 1) TheFirstAvailableReg = k; /* This will represent the hiword */

				if(NumRegsAvailable == 2)
				{
					/* map lo */
					fpureg[k].mips_reg = Conditions->mips_reg;
					fpureg[k].Is32bit = 0;

					if(Conditions->IsDirty == 1) fpureg[k].IsDirty = 1;

					Conditions->x86reg = k;

					if((Conditions->NoNeedToLoadTheLo == 0))
                        LoadFPR_LO(To_XMM_Or_Memory, k);

					/* map hi */
					fpureg[k].HiWordLoc = TheFirstAvailableReg;
					Conditions->HiWordLoc = TheFirstAvailableReg;

					if(fpureg[k].HiWordLoc == Reg_ESP)
						DisplayError("3: fpureg[%d]: Write to esp: cannot do that!", k);

					fpureg[TheFirstAvailableReg].mips_reg = IT_IS_HIGHWORD;

					if((Conditions->NoNeedToLoadTheHi == 0))
					{
						LoadFPR_HI(k);
					}

					SSE_FlushedRegistersMap[fpureg[k].mips_reg].Is32bit = 0;

					if(fpureg[k].HiWordLoc == -1) DisplayError("Do64bit: Hiword = -1 Not expected.");

					fpureg[k].BirthDate = ThisYear;
					fpureg[TheFirstAvailableReg].BirthDate = ThisYear;
					MapSuccess = 1;
					return;
				}
			}
		}

		if(MapSuccess == 0)
		{
			int temp;
            
            //don't assign the function return value to k.
            temp = SSE_FlushOneButNotThese3(To_XMM_Or_Memory, 99, The2ndOneNotToFlush, The3rdOneNotToFlush);

                    if (temp==-1)
                    {
                        DisplayError("SSE_Map64bit: Failed to flush", "", 0);
                    }

		}
	}
}

x86regtyp	Targetfpureg[8];
MapConstant TargetSSE_ConstMap[SSE_NUM_CONSTS];

void SSE_ConvertRegTo32bit(int k)
{
	if(fpureg[k].HiWordLoc == -1) DisplayError("SSE_ConvertRegTo32bit() bug: HiWordLoc should not be -1");

    fpureg_Delete(fpureg[k].HiWordLoc);

	fpureg[k].HiWordLoc = k;
	fpureg[k].Is32bit = 1;
	fpureg[k].BirthDate = ThisYear;
}

void SSE_ConvertRegTo64bit(int To_XMM_Or_Memory, int k, x86regtyp *Conditions, int The2ndOneNotToFlush, int The3rdOneNotToFlush)
{
    MessageBox(0, "SSE_ConvertRegTo64bit: Unimplemented", "", 0);
}

void SSE_ConvertReg(int To_XMM_Or_Memory, int k, x86regtyp *Conditions, int The2ndOneNotToFlush, int The3rdOneNotToFlush)
{
    

    /* check if the register wants 32bit */
	if(Conditions->Is32bit == 1)
	{
        /*
		 * check if the HiWord is mapped to some other x86 register than where we are
		 * now...k.
		 */
		if((fpureg[k].HiWordLoc != k) && (Conditions->IsDirty == 1))
		{
			SSE_ConvertRegTo32bit(k);
		}
        else if(fpureg[k].HiWordLoc != k)
			Conditions->Is32bit = 0;	/* means do not convert. */
	}
	else
	{
		SSE_ConvertRegTo64bit(To_XMM_Or_Memory, k, Conditions, The2ndOneNotToFlush, The3rdOneNotToFlush);

    }

	/* set and return the map info */
	Conditions->x86reg = k;
	Conditions->HiWordLoc = fpureg[k].HiWordLoc;

	if(Conditions->IsDirty == 1) fpureg[k].IsDirty = 1;
	fpureg[k].Is32bit = Conditions->Is32bit;
	fpureg[k].BirthDate = ThisYear;
	fpureg[fpureg[k].HiWordLoc].BirthDate = ThisYear;

	if(fpureg[k].HiWordLoc == -1) DisplayError("Set&return map info: HiWord is -1!!!");
}


/*
 =======================================================================================================================
    Map a MIPS?register to an x86 register.
 =======================================================================================================================
 */
void SSE_MapRegister(int To_XMM_Or_Memory, x86regtyp *Conditions, int keep2, int keep3)
{
	int		k;

	ThisYear++;

//	if(Conditions->NoNeedToLoadTheLo == 0)
//		SSE_MapOneConstantToRegister(Conditions, keep2, keep3);

	if((k = SSE_CheckWhereIsMipsReg(Conditions->mips_reg)) > -1)
    {
		SSE_ConvertReg(To_XMM_Or_Memory, k, Conditions, keep2, keep3);
    }
	else if(Conditions->Is32bit == 1)
		SSE_Map32bit(To_XMM_Or_Memory, k, Conditions, keep2, keep3);
	else
		SSE_Map64bit(To_XMM_Or_Memory, k, Conditions, keep2, keep3);

	fpureg[k].BirthDate = ThisYear;
}

/*
 =======================================================================================================================
    If the MIPS?register is mapped, this will free it without writing it back to memory.
 =======================================================================================================================
 */
void SSE_FreeMipsRegister(int mips_reg)
{
	/*~~~~~*/
	int temp;
	/*~~~~~*/

	SSE_ConstMap[mips_reg].IsMapped = 0;
	if((temp = SSE_CheckWhereIsMipsReg(mips_reg)) > -1)
	{
		fpureg[temp].IsDirty = 0;
		SSE_FlushRegister(FLUSH_TO_MEMORY, temp);
	}
}

/*
 =======================================================================================================================
    Map a constant to our constant array. Constants are known values that don't require a register. Examples: r0 is
    always 0, the LUI instruction always generates constants. 1964 does a lot of constant detection.
 =======================================================================================================================
 */
void SSE_MapConst(x86regtyp *xMAP, int value)
{
	
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#ifndef NO_CONSTS
	int where = SSE_CheckWhereIsMipsReg(xMAP->mips_reg);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    MessageBox(0, "SSE_MapConstant: Hi", "", 0);
    if(where > -1)
	{
		fpureg[where].IsDirty = 0;
		SSE_FlushRegister(FLUSH_TO_MEMORY, where);
	}

    SSE_ConstMap[xMAP->mips_reg].IsMapped = 1;
	SSE_ConstMap[xMAP->mips_reg].value = value;

#else

		xMAP->IsDirty = 1;	/* bug fix */
		xMAP->Is32bit = 1;
		xMAP->NoNeedToLoadTheLo = 1;
		SSE_MapRegister(xMAP, 99, 99);
		MOV_ImmToReg(xMAP->x86reg, value);
#endif
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void SSE_MapOneConstantToRegister(x86regtyp *Conditions, int The2ndOneNotToFlush, int The3rdOneNotToFlush)
{
	/*~~~~~~~~~~~~~~~*/
	x86regtyp	xRJ[1];
	/*~~~~~~~~~~~~~~~*/

    MessageBox(0, "SSE_MapOneConstantToRegister: Hi", "", 0);
    xRJ->Is32bit = Conditions->Is32bit;

	if(SSE_ConstMap[Conditions->mips_reg].IsMapped == 1)
	{
		SSE_ConstMap[Conditions->mips_reg].IsMapped = 0;
		if(Conditions->mips_reg != 0)
		{
			xRJ->IsDirty = 1;
			Conditions->IsDirty = 1;
		}

		xRJ->mips_reg = Conditions->mips_reg;
		xRJ->NoNeedToLoadTheLo = 1;
		xRJ->NoNeedToLoadTheHi = 1;
		Conditions->NoNeedToLoadTheLo = 1;
		Conditions->NoNeedToLoadTheHi = 1;

        SSE_MapRegister(FLUSH_TO_MEMORY, xRJ, The2ndOneNotToFlush, The3rdOneNotToFlush);

		{
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			int k = (_s32) SSE_ConstMap[Conditions->mips_reg].value;
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			MOV_ImmToReg(xRJ->x86reg, SSE_ConstMap[Conditions->mips_reg].value);
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void SSE_FlushOneConstantLo(int k)
{
	int where;

	if(SSE_ConstMap[k].IsMapped == 1)
	{
		SSE_FlushedRegistersMap[k].Is32bit = 1;
		where = SSE_CheckWhereIsMipsReg(k);
		if(where > -1)
		{
#ifdef DEBUG_REGCACHE
			MessageBox(0, "That's not right..", "", 0);
#endif
			fpureg[where].IsDirty = 0;
			SSE_FlushRegister(FLUSH_TO_MEMORY, where);
		}
		{
			
/**************************************************************/                        
            _u32	off = (_u32) & gHWS_fpr32[0] + (k << 3);
			_s32	i = (_s32) SSE_ConstMap[k].value;
/**************************************************************/            

			SSE_StoreImm_LO(To_XMM_Or_Memory, k);

		}
	}
}

void SSE_FlushOneConstantHi(int To_XMM_Or_Memory, int k)
{
	int where;

	if(SSE_ConstMap[k].IsMapped == 1)
	{
		SSE_FlushedRegistersMap[k].Is32bit = 1;
		SSE_ConstMap[k].IsMapped = 0;
		where = SSE_CheckWhereIsMipsReg(k);
		if(where > -1)
		{
#ifdef DEBUG_REGCACHE
			DisplayError("Odd");
#endif
			fpureg[where].IsDirty = 0;
			SSE_FlushRegister(To_XMM_Or_Memory, where);
		}
		{
			
			_s32	i = (_s32) SSE_ConstMap[k].value;

//			_asm sar i, 31

//			if(currentromoptions.Assume_32bit == ASSUME_32BIT_NO)
//				SSE_StoreImm_HI(k);
		}
	}
}

void SSE_FlushOneConstant(int To_XMM_Or_Memory, int k)
{
	SSE_FlushOneConstantLo(k);
	SSE_FlushOneConstantHi(To_XMM_Or_Memory, k);
}


/*
 =======================================================================================================================
    Store our constants to the mips gpr array in memory.
 =======================================================================================================================
 */
void SSE_FlushConstants(int To_XMM_Or_Memory)
{
	int k;

	for(k = 0; k < SSE_NUM_CONSTS; k++)
	{
		SSE_FlushOneConstantLo(k);
	}

	//Lo and Hi are split up to avoid some bank conflicts
	for(k = 0; k < SSE_NUM_CONSTS; k++)
	{
		SSE_FlushOneConstantHi(To_XMM_Or_Memory, k);
	}
}

/*
 =======================================================================================================================
    Store the dirty register to the mips gpr array in memory.
 =======================================================================================================================
 */
void SSE_WriteBackDirty(int To_XMM_Or_Memory, _int8 k)
{
    /* 32 bit register */
	if((fpureg[k].HiWordLoc == k))
	{
		if(fpureg[k].Is32bit != 1) DisplayError("Bug");

        StoreFPR_LO(To_XMM_Or_Memory, k);
//
//		if((currentromoptions.Assume_32bit == ASSUME_32BIT_NO) /*&& 
  //          ((uint32)SSE_ConstMap[k].FinalAddressUsedAt <= (uint32)gHWS_pc)
    //        || ((uint32)SSE_ConstMap[k].FinalAddressUsedAt == 0xffffffff)*/)
	//	{
	//		SAR_RegByImm(k, 31);
	//		StoreFPR_HI(To_XMM_Or_Memory, k);
	//	}

	//	SSE_FlushedRegistersMap[fpureg[k].mips_reg].Is32bit = 1;
	}
	else
	/* 64 bit register */
	{
		
        MessageBox(0, "SSE_WriteBackDirty: This should not be touched yet", "", 0);
        if(fpureg[k].Is32bit == 1) DisplayError("Bug");

		StoreFPR_LO(To_XMM_Or_Memory, k);

		if
		(
		(currentromoptions.Assume_32bit == ASSUME_32BIT_NO)
		)
		{
            StoreFPR_HI(To_XMM_Or_Memory, k);
		}

		SSE_FlushedRegistersMap[fpureg[k].mips_reg].Is32bit = 0;
	}
}

/*
 =======================================================================================================================
    Unmaps a mips register from our fpureg[] register map. If the mips reg is dirty, it will be stored back to memory.
 =======================================================================================================================
 */
void SSE_FlushRegister(int To_XMM_Or_Memory, int k)
{

#ifdef DEBUG_REGCACHE
	/* paranoid error check */
	if(fpureg[k].HiWordLoc == -1) MessageBox(0, "SSE_FlushRegister: The HiWord was not set!", "", 0);
#endif

    if(fpureg[k].IsDirty == 1)
	    SSE_WriteBackDirty(To_XMM_Or_Memory, (_s8) k);


	fpureg_Delete(fpureg[k].HiWordLoc);
	fpureg_Delete(k);
}

/*
 =======================================================================================================================
    This function is just a combination of FlushRegister() and WriteBackDirty(). ?
    It was written so that FlushAllRegisters() can interleave the register flushing ?
    so that instructions will become paired for the processor.
 =======================================================================================================================
 */
void SSE_WriteBackDirtyLoOrHi(int To_XMM_Or_Memory, _int8 k, int Lo)
{
	/* 32 bit register */
	if(fpureg[k].IsDirty == 1)
	{
		if((fpureg[k].HiWordLoc == k))
		{
			if(fpureg[k].Is32bit != 1) DisplayError("Bug");

			if(Lo == 0)
			{
				StoreFPR_LO(To_XMM_Or_Memory, k);
			}
/*			else if(Lo == 1)
			{
				if(currentromoptions.Assume_32bit == ASSUME_32BIT_NO) 
					SAR_RegByImm(k, 31);
			}
			else if(currentromoptions.Assume_32bit == ASSUME_32BIT_NO)
			{
				if(Lo == 2)
					StoreFPR_HI(To_XMM_Or_Memory, k);
			}
*/
//			SSE_FlushedRegistersMap[fpureg[k].mips_reg].Is32bit = 1;
		}
		else
		/* 64 bit register */
		{
			if(fpureg[k].Is32bit == 1) DisplayError("Bug");

			if(!Lo)
			{
				StoreFPR_LO(To_XMM_Or_Memory, k);
			}
			else if(currentromoptions.Assume_32bit == ASSUME_32BIT_NO)
			{
				if(Lo == 2) 
					StoreFPR_HI(To_XMM_Or_Memory, k);
			}

			SSE_FlushedRegistersMap[fpureg[k].mips_reg].Is32bit = 0;
		}
	}
}

/*
 =======================================================================================================================
    Function: FlushAllRegisters() Purpose: This function is used when you want to "flush" all the General Purpose
    Registers (GPR) that are mapped so that no registers are mapped. An unmapped array entry = -1. input: output:
 =======================================================================================================================
 */

void SSE_FlushAllRegisters(int To_XMM_Or_Memory)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	_int8	k;
	int		Lo = 0;
	int		iterations = 3;
	const int	Num_fpuregs = NUM_fpuregS;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	ThisYear = 2001;

	/* TODO: __LO, __HI flush 64bit. */
	if(currentromoptions.Assume_32bit == ASSUME_32BIT_NO) iterations = 3;
	for(Lo = 0; Lo < iterations; Lo++)
	{
		for(k = 0; k < Num_fpuregs; k++)
		{
			if(SSE_ItIsARegisterNotToUse(k));
			else if(fpureg[k].mips_reg > -1)
            {
                SSE_WriteBackDirtyLoOrHi(To_XMM_Or_Memory, k, Lo);
            
            }

			if (Lo == 0)
            {
                SSE_FlushOneConstantLo(k+1);
            }
			else if( Lo == 1)
            {
                SSE_FlushOneConstantLo(k+8+1);
            }
			else if( Lo == 2)
            {
                SSE_FlushOneConstantLo(k+16+1);
            }

		}
	}




	for(k = (Num_fpuregs*3)+1; k < SSE_NUM_CONSTS; k++)
	{
            SSE_FlushOneConstantLo(k);
       	    SSE_FlushOneConstantHi(To_XMM_Or_Memory, k);
	}


	for(k = 0; k < Num_fpuregs; k++)
	{
		if(SSE_ItIsARegisterNotToUse(k));
		else if(fpureg[k].mips_reg > -1)
		{
			fpureg_Delete(fpureg[k].HiWordLoc);
			fpureg_Delete(k);
		}
	}



	//Lo and Hi are split up to avoid some bank conflicts
	for(k = 1; k < (Num_fpuregs*3)+1; k++)
	{
        SSE_FlushOneConstantHi(To_XMM_Or_Memory, k);
	}


	for(k = 1; k < SSE_NUM_CONSTS; k++) 
    {
        SSE_FlushedRegistersMap[k].Is32bit = 0;
        SSE_ConstMap[k].WasFlushedInBlock = 0;
    }
}




/*
 =======================================================================================================================
 =======================================================================================================================
 */
int SSE_FlushOneButNotThese3(int To_XMM_Or_Memory, int The1stOneNotToFlush, int The2ndOneNotToFlush, int The3rdOneNotToFlush)
{
	const _u8	Num_fpuregs = NUM_fpuregS;
	_u32		paranoid = 0;
	_u32		init = 0;
	_u32		EarliestYear;
	__int32		k;
	__int32		MarkedForDeletion;

   
    /* Flush by least recently used */
	for(k = Num_fpuregs - 1; k >= 0; k--)
	{
		if(SSE_ItIsARegisterNotToUse(k));
		else if
			(
				(fpureg[k].mips_reg > -1)
			&&	(fpureg[k].mips_reg != The1stOneNotToFlush)
			&&	(fpureg[k].mips_reg != The2ndOneNotToFlush)
			&&	(fpureg[k].mips_reg != The3rdOneNotToFlush)
			)
		{
#ifdef DEBUG_REGCACHE
			if(fpureg[k].HiWordLoc == -1) DisplayError("FlushOne: The HiWord was not set!");
#endif
			if(SSE_ConstMap[fpureg[k].mips_reg].FinalAddressUsedAt <= gHWS_pc)
			{
                if(currentromoptions.Advanced_Block_Analysis == USEBLOCKANALYSIS_YES)
				{
					MarkedForDeletion = k;
					EarliestYear = fpureg[k].BirthDate;
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
					EarliestYear = fpureg[k].BirthDate;
				}
				else if(fpureg[k].mips_reg == 0)
				{
					MarkedForDeletion = k;
					EarliestYear = fpureg[k].BirthDate;
					k = -1; /* exits */
				}
				else if(fpureg[k].BirthDate <= EarliestYear)
				{
					/*
					 * If they have same birth year, a nondirty has ?
					 * priority over a dirty for flushing
					 */
					if(fpureg[k].BirthDate == EarliestYear)
					{
						if((fpureg[MarkedForDeletion].IsDirty == 1) && (fpureg[k].IsDirty == 0))
							MarkedForDeletion = k;
					}
					else
					{
						MarkedForDeletion = k;
						EarliestYear = fpureg[k].BirthDate;
					}
				}
			}

			paranoid = 1;
		}
	}

	if(SSE_ConstMap[fpureg[MarkedForDeletion].mips_reg].IsMapped == 1)
	{
#ifdef DEBUG_REGCACHE
		if(fpureg[MarkedForDeletion].mips_reg != 0)
			DisplayError("This should not happen. Mips = %d", fpureg[MarkedForDeletion].mips_reg);
#endif
		SSE_FlushOneConstantLo(fpureg[MarkedForDeletion].mips_reg);	/* Unmaps corr. reg also */
		SSE_FlushOneConstantHi(To_XMM_Or_Memory, fpureg[MarkedForDeletion].mips_reg);	/* Unmaps corr. reg also */
	}


	SSE_FlushRegister(To_XMM_Or_Memory, MarkedForDeletion);
    if(paranoid == 0)
    {
        MarkedForDeletion = -1; //Failed.
    }

    return (MarkedForDeletion);
}