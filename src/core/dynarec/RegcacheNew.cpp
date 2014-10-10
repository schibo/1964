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

#include "../stdafx.h"

#define IT_IS_HIGHWORD	- 2
#define IT_IS_UNUSED	- 1
#define NUM_X86REGS		8

extern FlushedMap	FlushedRegistersMap[NUM_CONSTS];
extern int          Map_RD_NE_RT2(x86regtyp* xRD, x86regtyp* xRT);

void				MapOneConstantToRegisterNew(x86regtyp *Conditions, int mips_from, int The2ndOneNotToFlush, int The3rdOneNotToFlush);
void				Map32bitNew(int To_XMM_Or_Memory, int k, x86regtyp *Conditions, int mips_from, int keep2, int keep3);
void				Map64bitNew(int To_XMM_Or_Memory, x86regtyp *Conditions, int _mips_from, int _Is32bit, int keep2, int keep3);
void				MapRegisterNew(int To_XMM_Or_Memory, x86regtyp *Conditions, x86regtyp* xFrom, int keep2, int keep3);
void                ConvertRegNew(int To_XMM_Or_Memory, int k, x86regtyp *Conditions, int mips_from, int The2ndOneNotToFlush, int The3rdOneNotToFlush);
void                ConvertRegTo32bitNew(int k);
void                LoadGPR_LO_New(int k, int mips_from);


extern x86regtyp	x86reg[8];
extern void x86reg_Delete(int k);
extern int ItIsARegisterNotToUse(int k);

void ConvertRegTo32bitNew(int k)
{
	if(x86reg[k].HiWordLoc == -1) DisplayError("MapRegister() bug: HiWordLoc should not be -1");

	x86reg_Delete(x86reg[k].HiWordLoc);

	x86reg[k].HiWordLoc = k;
	x86reg[k].Is32bit = 1;
	x86reg[k].BirthDate = ThisYear;
}

void LoadGPR_LO_New(int k, int mips_from)
{
	if(mips_from == 0)
		XOR_Reg2ToReg1((_u8) k, (_u8) k);
    else
	{ 
		int from = CheckWhereIsMipsReg(mips_from);
		if (from > -1)
			MOV_Reg2ToReg1(k, from);
		else
		{
			FetchEBP_Params(mips_from);
			MOV_MemoryToReg((uint8) k, x86params.ModRM, x86params.Address);
		}
	} 
}

void LoadGPR_HI_New(int lo, int hi, int mips_from) 
{
        if(mips_from == 0)
		    XOR_Reg2ToReg1(hi, hi);
		else
		{
            int from = CheckWhereIsMipsReg(mips_from);

            if (from > -1)
        {
            if (x86reg[from].HiWordLoc == from)  //from 32bit
            {
                MOV_Reg2ToReg1(hi, from);    
                SAR_RegByImm(hi, 31);
            }
            else if (x86reg[from].HiWordLoc > -1) //from 64bit
            {
                MOV_Reg2ToReg1(hi, x86reg[from].HiWordLoc);
            }
        	else
        	{
                if(currentromoptions.Assume_32bit == ASSUME_32BIT_YES)
                {
		       	FetchEBP_Params(mips_from);
                    MOV_MemoryToReg(hi, x86params.ModRM, x86params.Address);
                    SAR_RegByImm(hi, 31);
		    }
		    else
            {
                FetchEBP_Params(mips_from);
		        MOV_MemoryToReg(hi, x86params.ModRM, 4 + x86params.Address);
            }
	    }
	}
	else
	{
            if(currentromoptions.Assume_32bit == ASSUME_32BIT_YES)
        {
                FetchEBP_Params(mips_from);
                MOV_MemoryToReg(hi, x86params.ModRM, x86params.Address);
           SAR_RegByImm(hi, 31);
        }
            else
            {
                FetchEBP_Params(mips_from);
                MOV_MemoryToReg(hi, x86params.ModRM, 4 + x86params.Address);
            }
        }
    }
}


void Map64bitNew(int To_XMM_Or_Memory, x86regtyp *Conditions, int _mips_from, int _Is32bit, int keep2, int keep3)
{    
	int hiword = 0;
    int NumRegsAvailable = 0;
	int MapSuccess = 0;
    int k;

    Conditions->NoNeedToLoadTheHi = _Is32bit;

    while(MapSuccess == 0)
	{
        hiword = 0;
        NumRegsAvailable = _Is32bit;

		for(k = NUM_X86REGS - 1; k >= 0; k--)
		{
			if(ItIsARegisterNotToUse(k));
			else if(x86reg[k].mips_reg == IT_IS_UNUSED)
			{
				if(++NumRegsAvailable == 1)
                    hiword = k;

				else if(NumRegsAvailable == 2)
				{
                    if (_Is32bit)
                        hiword = k;
                    
					if((Conditions->NoNeedToLoadTheLo == 0))
                        LoadGPR_LO_New(k, _mips_from);

                    if((Conditions->NoNeedToLoadTheHi == 0))
                        LoadGPR_HI_New(k, hiword, _mips_from);

					if (!_Is32bit)
                    {
                        x86reg[hiword].mips_reg = IT_IS_HIGHWORD;
                        x86reg[hiword].BirthDate = ThisYear;
                    }

                    x86reg[k].IsDirty   = Conditions->IsDirty;
                    x86reg[k].Is32bit   = _Is32bit;
					x86reg[k].BirthDate = ThisYear;
                    x86reg[k].mips_reg  = Conditions->mips_reg;
                    x86reg[k].HiWordLoc = hiword;

					Conditions->x86reg = k;
                    Conditions->HiWordLoc = hiword;

					FlushedRegistersMap[x86reg[k].mips_reg].Is32bit = _Is32bit;

					MapSuccess = 1;
					return;
				}
			}
		}

		if(MapSuccess == 0)
		{
			int temp;

            //don't assign the function return value to k.
            temp = FlushOneButNotThese3(To_XMM_Or_Memory, 99, keep2, keep3);

            if (temp==-1)
            {
                MessageBox(0, "Map64bit: Failed to flush", "", 0);
            }
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void ConvertRegTo64bitNew(int To_XMM_Or_Memory, int k, int mips_from, x86regtyp *Conditions, int keep2, int keep3)
{
	int i;

    LoadGPR_LO_New(k, mips_from);
    
    /* check if the HiWord is mapped to the same register where we are at now...k. */
	if((x86reg[k].HiWordLoc == k))
	{
		int HiMapped = 0;
		int couldntmap = 0;

		/* find a spot for the HiWord */
		while(HiMapped == 0)
		{
			if(couldntmap++ >= 100) DisplayError("I can't find a place to map.");

			for(i = NUM_X86REGS-1; i >=0; i--)
			{
				if(ItIsARegisterNotToUse(i));
				else if((x86reg[i].mips_reg == IT_IS_UNUSED))
				{
_MapIt:                    
                    HiMapped = 1;

                    //MessageBox(0, "Convert To 64bit", "", 0);

					//this reg was unmodified. get the hiword
//                    if((x86reg[k].IsDirty == 0))
                    {
                        LoadGPR_HI_New(k, i, mips_from);
                    }

                    //the reg was previously modified. get the hiword
  //                  else if(Conditions->NoNeedToLoadTheHi == 0)
	//				{
	//					MOV_Reg2ToReg1((_u8) i, (_u8) k);
	//					SAR_RegByImm((_u8) i, 31);
	//				}

					x86reg[k].HiWordLoc = i;
					x86reg[k].Is32bit = 0;

					x86reg[i].mips_reg = IT_IS_HIGHWORD;
					x86reg[i].Is32bit = 0;
					x86reg[i].IsDirty = 0;
					x86reg[i].HiWordLoc = -1;


					FlushedRegistersMap[x86reg[k].mips_reg].Is32bit = 0;
					i = -1;
            }
		}

			if(HiMapped == 0)
			{
				i = FlushOneButNotThese3(To_XMM_Or_Memory, x86reg[k].mips_reg, keep2, keep3);
                    if (i==-1)
                    {
                        MessageBox(0, "ConvertRegTo64Bit: nothing available to free", "", 0);
                    }
                    else
                        goto _MapIt;
			}
		}
	}
    else
    {
        LoadGPR_HI_New(k, x86reg[k].HiWordLoc, mips_from);
	}
}



void ConvertRegNew(int To_XMM_Or_Memory, int k, x86regtyp *Conditions, int mips_from, int The2ndOneNotToFlush, int The3rdOneNotToFlush)
{
	/* check if the register wants 32bit */
	if(Conditions->Is32bit == 1)
	{
		/*
		 * check if the HiWord is mapped to some other x86 register than where we are
		 * now...k.
		 */
		if((x86reg[k].HiWordLoc != k) && (Conditions->IsDirty == 1))
		{
            ConvertRegTo32bitNew(k);
            LoadGPR_LO_New(k, mips_from);
		}
		else if(x86reg[k].HiWordLoc != k)
        {
			Conditions->Is32bit = 0;	/* means do not convert. */
        }
        else
         LoadGPR_LO_New(k, mips_from);
	}
	else
	{
        ConvertRegTo64bitNew(To_XMM_Or_Memory, k, mips_from, Conditions, The2ndOneNotToFlush, The3rdOneNotToFlush);
	}

	/* set and return the map info */
	Conditions->x86reg = k;
	Conditions->HiWordLoc = x86reg[k].HiWordLoc;

	if(Conditions->IsDirty == 1)
        x86reg[k].IsDirty = 1;
	x86reg[k].Is32bit = Conditions->Is32bit;
	x86reg[k].BirthDate = ThisYear;
	x86reg[x86reg[k].HiWordLoc].BirthDate = ThisYear;

	if(x86reg[k].HiWordLoc == -1) DisplayError("Set&return map info: HiWord is -1!!!");
}

/*
 =======================================================================================================================
    Map a MIPS register to an x86 register.
 =======================================================================================================================
 */

void MapRegisterNew(int To_XMM_Or_Memory, x86regtyp *Conditions, x86regtyp *xFrom, int keep2, int keep3)
{
    int k;

    ThisYear++;
  
	// xFrom is a const
    if ((ConstMap[xFrom->mips_reg].IsMapped) && (Conditions->NoNeedToLoadTheLo == 0))
    {
		MapOneConstantToRegisterNew(Conditions, xFrom->mips_reg, keep2, keep3);
    }
        else if (Conditions->mips_reg != xFrom->mips_reg)
        {
        //when Map_RD_NE_RT2 handles 6bit, then we don't need the first check.
        if ((Conditions->Is32bit) && Map_RD_NE_RT2(Conditions, xFrom))
            return;
            else if((ConstMap[xFrom->mips_reg].FinalAddressUsedAt > gHWS_pc))
                MapRegisterNew(FLUSH_TO_MEMORY, xFrom, xFrom, Conditions->mips_reg, keep3);
        }

    if(Conditions->mips_reg > 0)
        ConstMap[Conditions->mips_reg].IsMapped = 0;

    // xTo is mapped
    if((k = CheckWhereIsMipsReg(Conditions->mips_reg)) > -1)
    {
        ConvertRegNew(To_XMM_Or_Memory, k, Conditions, xFrom->mips_reg, keep2, keep3);
    }
    else
        Map64bitNew(To_XMM_Or_Memory, Conditions, xFrom->mips_reg, Conditions->Is32bit, keep2, keep3);

	x86reg[k].BirthDate = ThisYear;
}

void MapOneConstantToRegisterNew(x86regtyp *Conditions, int mips_from, int The2ndOneNotToFlush, int The3rdOneNotToFlush)
{
    x86regtyp	xRJ[1];

	xRJ->Is32bit = Conditions->Is32bit;

	if(ConstMap[mips_from].IsMapped == 1)
	{
		int k = (_s32) ConstMap[mips_from].value;

		if (mips_from == Conditions->mips_reg)
			ConstMap[mips_from].IsMapped = 0;

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
#ifndef NO_CONSTS
		ConstMap[0].IsMapped = 1;
#endif
		MapRegisterNew(FLUSH_TO_MEMORY, xRJ, xRJ, The2ndOneNotToFlush, The3rdOneNotToFlush);
#ifndef NO_CONSTS
		ConstMap[0].IsMapped = 1;
#endif
		ConstMap[0].value = 0;

		{
			MOV_ImmToReg(xRJ->x86reg, k);
			if(x86reg[xRJ->x86reg].Is32bit == 0)
			{
				_asm sar k, 31

				if (k==0)
                    XOR_Reg2ToReg1(xRJ->HiWordLoc, xRJ->HiWordLoc);
                else
                    OR_ImmToReg(xRJ->HiWordLoc, -1, 0);
			}
		}
	}
}
