/*$T regcache.h GC 1.136 03/09/02 16:50:59 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
#ifndef _REGCACHE_H__1964_
#define _REGCACHE_H__1964_

/* The following are macros that help us debug the dynarec */

//#ifdef _DEBUG
//#define CRASHABLE 1 //so i can go to MSVC disassembler at crash. Disable for release. */
//#endif

#define NUM_CONSTS	37
//NUM_CONSTS = __HI + 1

extern unsigned char	*RecompCode;



#define FLUSH_TO_MEMORY 0
#define FLUSH_TO_XMM    1

typedef struct	x86regtyp
{
	uint32	BirthDate;
	_int32	HiWordLoc;
	_int32	Is32bit;
	_int32	IsDirty;
	_int32	mips_reg;
	_int32	NoNeedToLoadTheLo;
	_int32	NoNeedToLoadTheHi;
	_int32	x86reg;
    _int32  FlushToXMM;
} x86regtyp;

/*
 -----------------------------------------------------------------------------------------------------------------------
    Keeps status of constants
 -----------------------------------------------------------------------------------------------------------------------
 */
typedef struct	MapConstant
{
	_int32	value;
	_int32	IsMapped;
	uint32	FinalAddressUsedAt; /* Analysis: at what PC the last Mips reg (rd,rs, or rt) is */
    _int32  WasFlushedInBlock; //Was this register flushed once alread in this block ? Used for registers
                               //needing preserve
} MapConstant;

typedef struct	x86paramstyp
{
	unsigned __int32	Address;
	unsigned char		ModRM;
} x86paramstyp;
extern x86paramstyp	x86params;

/*
 -----------------------------------------------------------------------------------------------------------------------
    Keeps status of registers stored to memory
 -----------------------------------------------------------------------------------------------------------------------
 */

extern uint32		ThisYear;
extern MapConstant ConstMap[NUM_CONSTS];
typedef struct	FlushedMap
{
	uint32	Is32bit;
} FlushedMap;

/*
 * Multi-Pass definitions ? */


enum PASSTYPE { COMPILE_MAP_ONLY, COMPILE_OPCODES_ONLY, COMPILE_ALL };

typedef struct MultiPass
{
	int PhysAddrAfterMap;	/* Phyical Start address of 1st instruction after the block's first mappings. */

} MultiPass;
extern MultiPass	gMultiPass;

/*
 * Mnemonics ? */
#define __OPCODE	((_u8) (r.r_.code >> 26))
#define __RS		(((_u8) (r.r_.code >> 21)) & 0x1f)
#define __RT		(((_u8) (r.r_.code >> 16)) & 0x1f)
#define __RD		(((_u8) (r.r_.code >> 11)) & 0x1f)
#define __SA		(((_u8) (r.r_.code >> 6)) & 0x1f)
#define __F			((_u8) (r.r_.code) & 0x3f)

/* define __I ( (_s32)(_s16)r.r_.code ) */
#define __I			((_s32) (_s16) (r.r_.code & 0xFFFF))
#define __O			(r.r_.pc + 4 + (__I << 2))
#define ____T		(r.r_.code & 0x3ffffff)
#define __FS		__RD
#define __FT		__RT
#define __FD		__SA

#define __dotRS		(((_u8) (gHWS_code >> 21)) & 0x1f)
#define __dotRT		(((_u8) (gHWS_code >> 16)) & 0x1f)
#define __dotRD		(((_u8) (gHWS_code >> 11)) & 0x1f)
#define __dotSA		(((_u8) (gHWS_code >> 6)) & 0x1f)
#define __dotI		((_s32) (_s16) gHWS_code)
#define __dotO		(gHWS_pc + 4 + (__dotI << 2))
#define ____dotT	(gHWS_code & 0x3ffffff)
#define __dotT		((gHWS_pc & 0xf0000000) | (____dotT << 2))
#define __dotF		((_u8) (gHWS_code) & 0x3f)
#define __ND		((uint8) ((r.r_.code >> 17) & 0x1))
#define __TF		((uint8) ((r.r_.code >> 16) & 0x1))

/*
 * Function Declarations ? */
extern int	CheckIs32Bit(int mips_reg);
extern void FlushAllRegisters(int To_XMM_Or_Memory);
extern int  FlushOneButNotThese3(int To_XMM_Or_Memory, int The1stOneNotToFlush, int The2ndOneNotToFlush, int The3rdOneNotToFlush);
extern int	CheckRegStatus(x86regtyp *Query);
extern void InitRegisterMap(void);
extern void FlushRegister(int To_XMM_Or_Memory, int k);
extern void WriteBackDirty(int To_XMM_Or_Memory, _int8 k);
extern void FlushOneConstantLo(int k);
extern void FlushOneConstantHi(int To_XMM_Or_Memory, int k);
extern void MapOneConstantToRegister(x86regtyp *Conditions, int The2ndOneNotToFlush, int The3rdOneNotToFlush);
extern int	CheckWhereIsMipsReg(int mips_reg);
extern void PUSH_RegIfMapped(int k, int TryStoringToARegisterFirst, int RegisterToProtect);
void		POP_RegIfMapped(int k);
void		FetchEBP_Params(int mips_reg);
void		FreeMipsRegister(int mips_reg);
extern int  FindFreeRegister();

extern void MapRegisterNew(int To_XMM_Or_Memory, x86regtyp *Conditions, x86regtyp *xFrom, int keep2, int keep3);
#define MapRD	MapRegisterNew(FLUSH_TO_MEMORY, xRD, xRD, xRS->mips_reg, xRT->mips_reg);
#define MapRS	MapRegisterNew(FLUSH_TO_MEMORY, xRS, xRS, xRD->mips_reg, xRT->mips_reg);
#define MapRT	MapRegisterNew(FLUSH_TO_MEMORY, xRT, xRT, xRD->mips_reg, xRS->mips_reg);



//Intel version, don't use LoadAndExecute Functions
//On AMD, LoadAndExecute functions are faster to use them, on Intel chips they are slower to use them.
#define MapRS_To(xDst, Use32bit, LoadAndExecute_Function) \
	{ \
		{ \
			MapRS \
 \
			/* register to register */ \
			LoadAndExecute_Function(xDst->x86reg, (uint8) (0xC0 | xRS->x86reg), 0); \
			if(!Use32bit) LoadAndExecute_Function(xDst->HiWordLoc, (uint8) (0xC0 | xRS->HiWordLoc), 0); \
		} \
	}

#define MapRT_To(xDst, Use32bit, LoadAndExecute_Function) \
	{ \
		{ \
			MapRT \
 \
			/* register to register */ \
			LoadAndExecute_Function(xDst->x86reg, (uint8) (0xC0 | xRT->x86reg), 0); \
			if(!Use32bit) LoadAndExecute_Function(xDst->HiWordLoc, (uint8) (0xC0 | xRT->HiWordLoc), 0); \
		} \
	}

#define MapRS_To__WithLoadOnlyFunction(xDst, Use32bit, LoadAndExecute_Function) \
	{ \
		if(((CheckWhereIsMipsReg(xRS->mips_reg)) == -1) && (ConstMap[xRS->mips_reg].FinalAddressUsedAt <= gHWS_pc)) \
		{ \
			FetchEBP_Params(xRS->mips_reg); \
 \
			/* memory to register */ \
			LoadAndExecute_Function(xDst->x86reg, x86params.ModRM, x86params.Address); \
			if(!Use32bit) LoadAndExecute_Function(xDst->HiWordLoc, x86params.ModRM, 4 + x86params.Address); \
		} \
		else \
		{ \
			MapRS \
 \
			/* register to register */ \
			LoadAndExecute_Function(xDst->x86reg, (uint8) (0xC0 | xRS->x86reg), 0); \
			if(!Use32bit) LoadAndExecute_Function(xDst->HiWordLoc, (uint8) (0xC0 | xRS->HiWordLoc), 0); \
		} \
	}


#define NEGATE(REG)			{ XOR_ShortToReg((_u8) REG->x86reg, 0xFF); INC_Reg(1, (_u8) REG->x86reg); }


#endif /* REGCACHE_H__1964_ */
