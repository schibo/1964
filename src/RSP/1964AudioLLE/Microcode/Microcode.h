#ifndef _MICROCODE_H_
#define _MICROCODE_H_

#include "../../Shared/rsp_mnemonic.h"
#include "../../Shared/rsp_opcodes.h"
#include "../../Shared/state.h"


extern _u32			DMEM_Address;
extern _u32			RDRAM_Address;
extern char			*pDMEM;
extern char			*pIMEM;
extern char			*pRDRAM;
extern _u32			sp_reg_pc;
extern t_state		state;

extern _u32			gUcode;
extern _s32			DoOnce;


// functions called by recompiled microcode
void DMAWrite(_u32 length);
char DMARead(_u32 length);
void ErrorToCallFunction(_u32 addr);

__forceinline _u8 Load8_DMEM(_u32 offset);
__forceinline _u16 Load16_DMEM(_u32 offset);
__forceinline _u32 Load32_DMEM(_u32 offset);
__forceinline _u64 Load64_DMEM(_u32 offset);
__forceinline void Save8_DMEM(_u8 what, _u32 addr);
__forceinline void Save16_DMEM(_u16 what, _u32 addr);
__forceinline void Save32_DMEM(_u32 what, _u32 addr);
__forceinline void Save64_DMEM(_u64 what, _u32 addr);

void rsp_ldv_compiler(_u32 addr, _u32 rt, _u32 offset);

// microcode hle functions
void Main_Parser(void);
void Mario13A8_13F4(void);
void Mario1424_1438(void);
void Mario15B8_16DC(void);
void Mario19D8_1AC4(void);
void Mario1D08_1D24(void);
void Mario1D30_1D4C(void);
void Mario1D50_1DE8(void);
void Mario1E5C_1E88(void);

void Zelda11A4_11D4(void);
void Zelda11F8_120C(void);
void Zelda13A4_14D8(void);
void Zelda192C_19A8(void);
void Zelda1B60_1B8C(void);
void Zelda1DC4_1E14(void);

void SmashBros11AC_11B4(void);
void SmashBros12E4_1334(void);
void SmashBros135C_1370(void);
void SmashBros1514_1630(void);
void SmashBros193C_1A28(void);
void SmashBros1BB0_1C48(void);
void SmashBros1C84_1CC8(void);

void DKR1864_18B0(void);
void Main_Parser_Tooie(void);
void Tooie1F20_1F84(void);

#endif
