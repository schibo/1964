//RSP microcode
//Generated Sunday, December 10, 2006 at 1930 UTC.

#include "../../Shared/rsp_prefix.h"
#include "../../Shared/rsp_macros.h"
#include "Microcode.h"
#include "TestMacros.h"

void _ZeldaData(_u32 StartAddress);

void Zelda11A4_11D4(void);
void Zelda11F8_120C(void);
void Zelda13A4_14D8(void);
void Zelda192C_19A8(void);
void Zelda1B60_1B8C(void);
void Zelda1DC4_1E14(void);

void _ZeldaBootCode(_u32 StartAddress)
{
	static _u32	JumpTable[(0x1000 >> 2)];
	static _u32	BuildJumpTable = 0;

	if (!BuildJumpTable)
	{
		BuildJumpTable = 1;
		goto _BuildJumpTable;
	}

	_Label:
	StartAddress = JumpTable[(StartAddress & 0x0FFF) >> 2];
	__asm mov eax, StartAddress
	__asm jmp eax


_04001000:	// 4001000: <340a0fc0>	ORI       T2 = R0 | 0x0fc0
	state.r[T2] = (0 | 0x00000FC0);


_04001004:	// 4001004: <8d420018>	LW        V0 = [T2+0018]
	state.r[V0] = (_s32)Load32_DMEM((state.r[T2] + 0x00000018) & 0xfff);


_04001008:	// 4001008: <8d43001c>	LW        V1 = [T2+001c]
	state.r[V1] = (_s32)Load32_DMEM((state.r[T2] + 0x0000001C) & 0xfff);


_0400100C:	// 400100c: <40803800>	MTC0      SP semaphore = R0


_04001010:	// 4001010: <0d0006b5>	JAL	    04001ad4
	state.r[AT] = 0 + 0x0000;
	sp_reg_pc = 0x04001010;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_04001014:	// 4001014: <20010000>	ADDI      AT = R0 + 0000
	state.r[AT] = 0 + 0x0000;


_04001018:	// 4001018: <201802e0>	ADDI      T8 = R0 + 02e0
	state.r[T8] = 0 + 0x02E0;


_0400101C:	// 400101c: <20170fb0>	ADDI      S7 = R0 + 0fb0
	state.r[S7] = 0 + 0x0FB0;


_04001020:	// 4001020: <8d5c0030>	LW        GP = [T2+0030]
	state.r[GP] = (_s32)Load32_DMEM((state.r[T2] + 0x00000030) & 0xfff);


_04001024:	// 4001024: <8d5b0034>	LW        K1 = [T2+0034]
	state.r[K1] = (_s32)Load32_DMEM((state.r[T2] + 0x00000034) & 0xfff);


_04001028:	// 4001028: <40055800>	MFC0      T3 = SP DMA full
	state.r[A1] = 0;


_0400102C:	// 400102c: <30a40001>	ANDI      A0 = A1 & 0001
	state.r[A0] = (state.r[A1] & 0x00000001);


_04001030:	// 4001030: <10800006>	BEQ       (R0==A0) --> 104c
	sp_reg_pc = 0x04001030;
	if (state.r[A0] == 0)
	{
		state.r[A0] = (state.r[A1] & 0x00000100);
		goto _0400104C;
	}


_04001034:	// 4001034: <30a40100>	ANDI      A0 = A1 & 0100
	state.r[A0] = (state.r[A1] & 0x00000100);


_04001038:	// 4001038: <10800004>	BEQ       (R0==A0) --> 104c
	sp_reg_pc = 0x04001038;
	if (state.r[A0] == 0)
	{
		goto _0400104C;
	}


_0400103C:	// 400103c: <00000000>	NOP       


_04001040:	// 4001040: <40045800>	MFC0      T3 = SP status
	state.r[A0] = 0;


_04001044:	// 4001044: <30840100>	ANDI      A0 = A0 & 0100
	state.r[A0] = (state.r[A0] & 0x00000100);


_04001048:	// 4001048: <1c80fffd>	BGTZ      (R0>0) --> 1040
	sp_reg_pc = 0x04001048;
	if((_s32)state.r[A0] > 0)
	{
		goto _04001040;
	}


_0400104C:	// 400104c: <00000000>	NOP       


_04001050:	// 4001050: <0d000431>	JAL	    040010c4
	sp_reg_pc = 0x04001050;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _040010C4;


_04001054:	// 4001054: <00000000>	NOP       


_04001058:	// 4001058: <8fba0000>	LW        K0 = [SP+0000]
	// In function: Main_Parser
	//Call the function: Main_Parser
	Main_Parser();
	goto _04001080;

_0400105C:	// 400105c: <8fb90004>	LW        T9 = [SP+0004]
	ErrorToCallFunction(0x0400105C);

_04001060:	// 4001060: <001a0dc2>	SRL       AT = K0 >> 23
	ErrorToCallFunction(0x04001060);

_04001064:	// 4001064: <302100fe>	ANDI      AT = AT & 00fe
	ErrorToCallFunction(0x04001064);

_04001068:	// 4001068: <239c0008>	ADDI      GP = GP + 0008
	ErrorToCallFunction(0x04001068);

_0400106C:	// 400106c: <237bfff8>	ADDI      K1 = K1 + fff8
	ErrorToCallFunction(0x0400106C);

_04001070:	// 4001070: <23bd0008>	ADDI      SP = SP + 0008
	ErrorToCallFunction(0x04001070);

_04001074:	// 4001074: <23defff8>	ADDI      S8 = S8 + fff8
	ErrorToCallFunction(0x04001074);

_04001078:	// 4001078: <00011020>	ADD       V0 = R0+AT
	ErrorToCallFunction(0x04001078);

_0400107C:	// 400107c: <84420010>	LH        V0 = [V0+0010]
	ErrorToCallFunction(0x0400107C);

_04001080:	// 4001080: <00400008>	JR        V0
	{
		_u32	temp = ((state.r[V0] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001084:	// 4001084: <00000000>	NOP       


_04001088:	// 4001088: <0000000d>	BREAK
	state.halt = 1;
	return;


_0400108C:	// 400108c: <1fc0fff2>	BGTZ      (R0>0) --> 1058
	sp_reg_pc = 0x0400108C;
	if((_s32)state.r[S8] > 0)
	{
		goto _04001058;
	}


_04001090:	// 4001090: <00000000>	NOP       


_04001094:	// 4001094: <1b600005>	BLEZ      (R0<=0) --> 10ac
	sp_reg_pc = 0x04001094;
	if ((_s32)state.r[K1] <= 0)
	{
		goto _040010AC;
	}


_04001098:	// 4001098: <00000000>	NOP       


_0400109C:	// 400109c: <0d000431>	JAL	    040010c4
	sp_reg_pc = 0x0400109C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _040010C4;


_040010A0:	// 40010a0: <00000000>	NOP       


_040010A4:	// 40010a4: <09000416>	J         04001058
	goto _04001058;


_040010A8:	// 40010a8: <00000000>	NOP       


_040010AC:	// 40010ac: <34014000>	ORI       AT = R0 | 0x4000
	state.r[AT] = (0 | 0x00004000);


_040010B0:	// 40010b0: <40812000>	MTC0      SP status = AT


_040010B4:	// 40010b4: <0000000d>	BREAK
	state.halt = 1;
	return;


_040010B8:	// 40010b8: <00000000>	NOP       


_040010BC:	// 40010bc: <1000ffff>	BEQ       (R0==R0) --> 10bc
	sp_reg_pc = 0x040010BC;
	if (0 == 0)
	{
		goto _040010BC;
	}


_040010C0:	// 40010c0: <00000000>	NOP       


_040010C4:	// 40010c4: <23e50000>	ADDI      A1 = RA + 0000
	state.r[A1] = state.r[RA] + 0x0000;


_040010C8:	// 40010c8: <001c1020>	ADD       V0 = R0+GP
	state.r[V0] = (0 + state.r[GP]);


_040010CC:	// 40010cc: <23630000>	ADDI      V1 = K1 + 0000
	state.r[V1] = state.r[K1] + 0x0000;


_040010D0:	// 40010d0: <2064ffc0>	ADDI      A0 = V1 + ffc0
	state.r[A0] = state.r[V1] + 0xFFFFFFC0;


_040010D4:	// 40010d4: <18800002>	BLEZ      (R0<=0) --> 10e0
	sp_reg_pc = 0x040010D4;
	if ((_s32)state.r[A0] <= 0)
	{
		state.r[AT] = 0 + 0x02F0;
		goto _040010E0;
	}


_040010D8:	// 40010d8: <200102f0>	ADDI      AT = R0 + 02f0
	state.r[AT] = 0 + 0x02F0;


_040010DC:	// 40010dc: <20030040>	ADDI      V1 = R0 + 0040
	state.r[V1] = 0 + 0x0040;


_040010E0:	// 40010e0: <207e0000>	ADDI      S8 = V1 + 0000
	state.r[S8] = state.r[V1] + 0x0000;


_040010E4:	// 40010e4: <0d0006b5>	JAL	    04001ad4
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x040010E4;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_040010E8:	// 40010e8: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_040010EC:	// 40010ec: <00a00008>	JR        A1
	{
		_u32	temp = ((state.r[A1] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[SP] = 0 + 0x02F0;
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_040010F0:	// 40010f0: <201d02f0>	ADDI      SP = R0 + 02f0
	state.r[SP] = 0 + 0x02F0;


_040010F4:	// 40010f4: <40043800>	MFC0      A3 = SP status
	state.r[A0] = 0;


_040010F8:	// 40010f8: <1480fffe>	BNE       (R0!=A0) --> 10f4
	sp_reg_pc = 0x040010F8;
	if (state.r[A0] != 0)
	{
		goto _040010F4;
	}


_040010FC:	// 40010fc: <00000000>	NOP       


_04001100:	// 4001100: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001104:	// 4001104: <1480fffe>	BNE       (R0!=A0) --> 1100
	sp_reg_pc = 0x04001104;
	if (state.r[A0] != 0)
	{
		goto _04001100;
	}


_04001108:	// 4001108: <00000000>	NOP       


_0400110C:	// 400110c: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_04001110:	// 4001110: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001114:	// 4001114: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001114;
	DMARead(state.r[V1]);


_04001118:	// 4001118: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_0400111C:	// 400111c: <00000000>	NOP       


_04001120:	// 4001120: <40043800>	MFC0      A3 = SP status
	state.r[A0] = 0;


_04001124:	// 4001124: <1480fffe>	BNE       (R0!=A0) --> 1120
	sp_reg_pc = 0x04001124;
	if (state.r[A0] != 0)
	{
		goto _04001120;
	}


_04001128:	// 4001128: <00000000>	NOP       


_0400112C:	// 400112c: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001130:	// 4001130: <1480fffe>	BNE       (R0!=A0) --> 112c
	sp_reg_pc = 0x04001130;
	if (state.r[A0] != 0)
	{
		goto _0400112C;
	}


_04001134:	// 4001134: <00000000>	NOP       


_04001138:	// 4001138: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_0400113C:	// 400113c: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001140:	// 4001140: <40831800>	MTC0      SP write DMA length = V1
	DMAWrite(state.r[V1]);


_04001144:	// 4001144: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001148:	// 4001148: <00000000>	NOP       


_0400114C:	// 400114c: <3323ffff>	ANDI      V1 = T9 & ffff
	state.r[V1] = (state.r[T9] & 0x0000FFFF);


_04001150:	// 4001150: <1060ffce>	BEQ       (R0==V1) --> 108c
	sp_reg_pc = 0x04001150;
	if (state.r[V1] == 0)
	{
		state.r[V0] = (state.r[K0] & 0x0000FFFF);
		goto _0400108C;
	}


_04001154:	// 4001154: <3342ffff>	ANDI      V0 = K0 & ffff
	state.r[V0] = (state.r[K0] & 0x0000FFFF);


_04001158:	// 4001158: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_0400115C:	// 400115c: <2063fff0>	ADDI      V1 = V1 + fff0
	state.r[V1] = state.r[V1] + 0xFFFFFFF0;


_04001160:	// 4001160: <e8401800>	SDV       [V0 + 0x00] = vec00 <00>
	{
		_u32	addr = (0x00000000 + state.r[V0]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[0].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[0].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[0].U32[0x00000003];
		}
	}


_04001164:	// 4001164: <e8401801>	SDV       [V0 + 0x04] = vec00 <00>
	{
		_u32	addr = (0x00000008 + state.r[V0]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[0].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[0].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[0].U32[0x00000003];
		}
	}


_04001168:	// 4001168: <20420010>	ADDI      V0 = V0 + 0010
	state.r[V0] = state.r[V0] + 0x0010;


_0400116C:	// 400116c: <1c60fffc>	BGTZ      (R0>0) --> 1160
	sp_reg_pc = 0x0400116C;
	if((_s32)state.r[V1] > 0)
	{
		state.r[V1] = state.r[V1] + 0xFFFFFFF0;
		goto _04001160;
	}


_04001170:	// 4001170: <2063fff0>	ADDI      V1 = V1 + fff0
	state.r[V1] = state.r[V1] + 0xFFFFFFF0;


_04001174:	// 4001174: <09000423>	J         0400108c
	goto _0400108C;


_04001178:	// 4001178: <00000000>	NOP       


_0400117C:	// 400117c: <00191402>	SRL       V0 = T9 >> 16
	state.r[V0] = ((_u32)state.r[T9] >> 16);


_04001180:	// 4001180: <a71a0000>	SH        [T8+0000] = K0
	Save16_DMEM((_u16)state.r[K0], (state.r[T8] + 0x00000000) & 0xfff);


_04001184:	// 4001184: <a7020002>	SH        [T8+0002] = V0
	Save16_DMEM((_u16)state.r[V0], (state.r[T8] + 0x00000002) & 0xfff);


_04001188:	// 4001188: <09000423>	J         0400108c
	Save16_DMEM((_u16)state.r[T9], (state.r[T8] + 0x00000004) & 0xfff);
	goto _0400108C;


_0400118C:	// 400118c: <a7190004>	SH        [T8+0004] = T9
	Save16_DMEM((_u16)state.r[T9], (state.r[T8] + 0x00000004) & 0xfff);


_04001190:	// 4001190: <3344ffff>	ANDI      A0 = K0 & ffff
	state.r[A0] = (state.r[K0] & 0x0000FFFF);


_04001194:	// 4001194: <001a0b02>	SRL       AT = K0 >> 12
	state.r[AT] = ((_u32)state.r[K0] >> 12);


_04001198:	// 4001198: <30210ff0>	ANDI      AT = AT & 0ff0
	state.r[AT] = (state.r[AT] & 0x00000FF0);


_0400119C:	// 400119c: <3323ffff>	ANDI      V1 = T9 & ffff
	state.r[V1] = (state.r[T9] & 0x0000FFFF);


_040011A0:	// 40011a0: <00191402>	SRL       V0 = T9 >> 16
	state.r[V0] = ((_u32)state.r[T9] >> 16);


_040011A4:	// 40011a4: <c8411800>	LDV       vec01 <00> = [V0 + 0x00]

	// [start function: Zelda11A4_11D4]

	Zelda11A4_11D4();
	goto _040011D8;

_040011A8:	// 40011a8: <c8621800>	LDV       vec02 <00> = [V1 + 0x00]

_040011AC:	// 40011ac: <2021fff8>	ADDI      AT = AT + fff8

_040011B0:	// 40011b0: <20840010>	ADDI      A0 = A0 + 0010

_040011B4:	// 40011b4: <e8810878>	SSV       [A0 + 0x10] = vec01 <00>

_040011B8:	// 40011b8: <e881097a>	SSV       [A0 + 0x14] = vec01 <02>

_040011BC:	// 40011bc: <20420008>	ADDI      V0 = V0 + 0008

_040011C0:	// 40011c0: <e8810a7c>	SSV       [A0 + 0x18] = vec01 <04>

_040011C4:	// 40011c4: <e8810b7e>	SSV       [A0 + 0x1c] = vec01 <06>

_040011C8:	// 40011c8: <e8820879>	SSV       [A0 + 0x12] = vec02 <00>

_040011CC:	// 40011cc: <20630008>	ADDI      V1 = V1 + 0008

_040011D0:	// 40011d0: <e882097b>	SSV       [A0 + 0x16] = vec02 <02>

_040011D4:	// 40011d4: <e8820a7d>	SSV       [A0 + 0x1a] = vec02 <04>
	ErrorToCallFunction(0x040011D4);

	// [end of function: Zelda11A4_11D4]

_040011D8:	// 40011d8: <1c20fff2>	BGTZ      (R0>0) --> 11a4
	sp_reg_pc = 0x040011D8;
	if((_s32)state.r[AT] > 0)
	{
		Save16_DMEM(state.v[2].U16[4], (0xFFFFFFFE + state.r[A0]) & 0xfff);
		goto _040011A4;
	}


_040011DC:	// 40011dc: <e8820b7f>	SSV       [A0 + 0x1e] = vec02 <06>
	Save16_DMEM(state.v[2].U16[4], (0xFFFFFFFE + state.r[A0]) & 0xfff);


_040011E0:	// 40011e0: <09000423>	J         0400108c
	goto _0400108C;


_040011E4:	// 40011e4: <00000000>	NOP       


_040011E8:	// 40011e8: <3321ffff>	ANDI      AT = T9 & ffff
	state.r[AT] = (state.r[T9] & 0x0000FFFF);


_040011EC:	// 40011ec: <1020ffa7>	BEQ       (R0==AT) --> 108c
	sp_reg_pc = 0x040011EC;
	if (state.r[AT] == 0)
	{
		state.r[V0] = (state.r[K0] & 0x0000FFFF);
		goto _0400108C;
	}


_040011F0:	// 40011f0: <3342ffff>	ANDI      V0 = K0 & ffff
	state.r[V0] = (state.r[K0] & 0x0000FFFF);


_040011F4:	// 40011f4: <00191c02>	SRL       V1 = T9 >> 16
	state.r[V1] = ((_u32)state.r[T9] >> 16);


_040011F8:	// 40011f8: <c8411800>	LDV       vec01 <00> = [V0 + 0x00]

	// [start function: Zelda11F8_120C]

	Zelda11F8_120C();
	goto _04001210;

_040011FC:	// 40011fc: <c8421801>	LDV       vec02 <00> = [V0 + 0x08]

_04001200:	// 4001200: <2021fff0>	ADDI      AT = AT + fff0

_04001204:	// 4001204: <20420010>	ADDI      V0 = V0 + 0010

_04001208:	// 4001208: <e8611800>	SDV       [V1 + 0x00] = vec01 <00>

_0400120C:	// 400120c: <e8621801>	SDV       [V1 + 0x04] = vec02 <00>
	ErrorToCallFunction(0x0400120C);

	// [end of function: Zelda11F8_120C]

_04001210:	// 4001210: <1c20fff9>	BGTZ      (R0>0) --> 11f8
	sp_reg_pc = 0x04001210;
	if((_s32)state.r[AT] > 0)
	{
		state.r[V1] = state.r[V1] + 0x0010;
		goto _040011F8;
	}


_04001214:	// 4001214: <20630010>	ADDI      V1 = V1 + 0010
	state.r[V1] = state.r[V1] + 0x0010;


_04001218:	// 4001218: <09000423>	J         0400108c
	goto _0400108C;


_0400121C:	// 400121c: <00000000>	NOP       


_04001220:	// 4001220: <00190a00>	SLL       AT = T9 << 8
	state.r[AT] = ((_u32)state.r[T9] << 8);


_04001224:	// 4001224: <00010a02>	SRL       AT = AT >> 8
	state.r[AT] = ((_u32)state.r[AT] >> 8);


_04001228:	// 4001228: <09000423>	J         0400108c
	Save32_DMEM((_u32)state.r[AT], (state.r[T8] + 0x00000008) & 0xfff);
	goto _0400108C;


_0400122C:	// 400122c: <af010008>	SW        [T8+0008] = AT
	Save32_DMEM((_u32)state.r[AT], (state.r[T8] + 0x00000008) & 0xfff);


_04001230:	// 4001230: <c81f2000>	LQV       vec31 <00> = [R0 + 0x00]
	{
		_u32 addr = (0x00000000 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001234:	// 4001234: <4a1bdeec>	VXOR      vec27 = vec27 xor vec27[<none>]
	rsp_cop2_vxor(&state.v[27], &state.v[27], &state.v[27]);


_04001238:	// 4001238: <97150000>	LHU       S5 = [T8+0000]
	state.r[S5] = (_s32)(_u16)Load16_DMEM((state.r[T8] + 0x00000000) & 0xfff);


_0400123C:	// 400123c: <4a19ce6c>	VXOR      vec25 = vec25 xor vec25[<none>]
	rsp_cop2_vxor(&state.v[25], &state.v[25], &state.v[25]);


_04001240:	// 4001240: <4a18c62c>	VXOR      vec24 = vec24 xor vec24[<none>]
	rsp_cop2_vxor(&state.v[24], &state.v[24], &state.v[24]);


_04001244:	// 4001244: <22b40001>	ADDI      S4 = S5 + 0001
	state.r[S4] = state.r[S5] + 0x0001;


_04001248:	// 4001248: <97130002>	LHU       S3 = [T8+0002]
	state.r[S3] = (_s32)(_u16)Load16_DMEM((state.r[T8] + 0x00000002) & 0xfff);


_0400124C:	// 400124c: <4a0d6b6c>	VXOR      vec13 = vec13 xor vec13[<none>]
	rsp_cop2_vxor(&state.v[13], &state.v[13], &state.v[13]);


_04001250:	// 4001250: <4a0e73ac>	VXOR      vec14 = vec14 xor vec14[<none>]
	rsp_cop2_vxor(&state.v[14], &state.v[14], &state.v[14]);


_04001254:	// 4001254: <97120004>	LHU       S2 = [T8+0004]
	state.r[S2] = (_s32)(_u16)Load16_DMEM((state.r[T8] + 0x00000004) & 0xfff);


_04001258:	// 4001258: <4a0f7bec>	VXOR      vec15 = vec15 xor vec15[<none>]
	rsp_cop2_vxor(&state.v[15], &state.v[15], &state.v[15]);


_0400125C:	// 400125c: <4a10842c>	VXOR      vec16 = vec16 xor vec16[<none>]
	rsp_cop2_vxor(&state.v[16], &state.v[16], &state.v[16]);


_04001260:	// 4001260: <00198a00>	SLL       S1 = T9 << 8
	state.r[S1] = ((_u32)state.r[T9] << 8);


_04001264:	// 4001264: <4a118c6c>	VXOR      vec17 = vec17 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[17], &state.v[17]);


_04001268:	// 4001268: <4a1294ac>	VXOR      vec18 = vec18 xor vec18[<none>]
	rsp_cop2_vxor(&state.v[18], &state.v[18], &state.v[18]);


_0400126C:	// 400126c: <00118a02>	SRL       S1 = S1 >> 8
	state.r[S1] = ((_u32)state.r[S1] >> 8);


_04001270:	// 4001270: <4a139cec>	VXOR      vec19 = vec19 xor vec19[<none>]
	rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);


_04001274:	// 4001274: <ea7b2000>	SQV       [S3 + 0x00] = vec27 <00>
	{
		_u32    addr = (0x00000000 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[27].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[27].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[27].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[27].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[27].U64[0] & temp1);
			value2 |= (state.v[27].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001278:	// 4001278: <ea7b2001>	SQV       [S3 + 0x10] = vec27 <00>
	{
		_u32    addr = (0x00000010 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[27].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[27].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[27].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[27].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[27].U64[0] & temp1);
			value2 |= (state.v[27].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_0400127C:	// 400127c: <20100040>	ADDI      S0 = R0 + 0040
	state.r[S0] = 0 + 0x0040;


_04001280:	// 4001280: <200f0330>	ADDI      T7 = R0 + 0330
	state.r[T7] = 0 + 0x0330;


_04001284:	// 4001284: <001a0c02>	SRL       AT = K0 >> 16
	state.r[AT] = ((_u32)state.r[K0] >> 16);


_04001288:	// 4001288: <30280004>	ANDI      T0 = AT & 0004
	state.r[T0] = (state.r[AT] & 0x00000004);


_0400128C:	// 400128c: <1100000a>	BEQ       (R0==T0) --> 12b8
	sp_reg_pc = 0x0400128C;
	if (state.r[T0] == 0)
	{
		goto _040012B8;
	}


_04001290:	// 4001290: <00000000>	NOP       


_04001294:	// 4001294: <200a0005>	ADDI      T2 = R0 + 0005
	state.r[T2] = 0 + 0x0005;


_04001298:	// 4001298: <2009000e>	ADDI      T1 = R0 + 000e
	state.r[T1] = 0 + 0x000E;


_0400129C:	// 400129c: <ca191802>	LDV       vec25 <00> = [S0 + 0x10]
	rsp_ldv_compiler((state.r[S0] + 0x00000010) & 0xFFF, 25, 8);


_040012A0:	// 40012a0: <ca171803>	LDV       vec23 <00> = [S0 + 0x18]
	rsp_ldv_compiler((state.r[S0] + 0x00000018) & 0xFFF, 23, 8);


_040012A4:	// 40012a4: <2210ffff>	ADDI      S0 = S0 + ffff
	state.r[S0] = state.r[S0] + 0xFFFFFFFF;


_040012A8:	// 40012a8: <ca191c02>	LDV       vec25 <08> = [S0 + 0x10]
	rsp_ldv_compiler((state.r[S0] + 0x00000010) & 0xFFF, 25, 0);


_040012AC:	// 40012ac: <22100002>	ADDI      S0 = S0 + 0002
	state.r[S0] = state.r[S0] + 0x0002;


_040012B0:	// 40012b0: <090004b4>	J         040012d0
	rsp_ldv_compiler((state.r[S0] + 0x00000018) & 0xFFF, 23, 0);
	goto _040012D0;


_040012B4:	// 40012b4: <ca171c03>	LDV       vec23 <08> = [S0 + 0x18]
	rsp_ldv_compiler((state.r[S0] + 0x00000018) & 0xFFF, 23, 0);


_040012B8:	// 40012b8: <200a0009>	ADDI      T2 = R0 + 0009
	state.r[T2] = 0 + 0x0009;


_040012BC:	// 40012bc: <2009000c>	ADDI      T1 = R0 + 000c
	state.r[T1] = 0 + 0x000C;


_040012C0:	// 40012c0: <ca191800>	LDV       vec25 <00> = [S0 + 0x00]
	rsp_ldv_compiler((state.r[S0] & 0xFFF), 25, 8);


_040012C4:	// 40012c4: <ca181c00>	LDV       vec24 <08> = [S0 + 0x00]
	rsp_ldv_compiler((state.r[S0] & 0xFFF), 24, 0);


_040012C8:	// 40012c8: <ca171801>	LDV       vec23 <00> = [S0 + 0x08]
	rsp_ldv_compiler((state.r[S0] + 0x00000008) & 0xFFF, 23, 8);


_040012CC:	// 40012cc: <ca171c01>	LDV       vec23 <08> = [S0 + 0x08]
	rsp_ldv_compiler((state.r[S0] + 0x00000008) & 0xFFF, 23, 0);


_040012D0:	// 40012d0: <001a0c02>	SRL       AT = K0 >> 16
	state.r[AT] = ((_u32)state.r[K0] >> 16);


_040012D4:	// 40012d4: <30210001>	ANDI      AT = AT & 0001
	state.r[AT] = (state.r[AT] & 0x00000001);


_040012D8:	// 40012d8: <1c200008>	BGTZ      (R0>0) --> 12fc
	sp_reg_pc = 0x040012D8;
	if((_s32)state.r[AT] > 0)
	{
		state.r[AT] = ((_u32)state.r[K0] >> 16);
		goto _040012FC;
	}


_040012DC:	// 40012dc: <001a0c02>	SRL       AT = K0 >> 16
	state.r[AT] = ((_u32)state.r[K0] >> 16);


_040012E0:	// 40012e0: <30210002>	ANDI      AT = AT & 0002
	state.r[AT] = (state.r[AT] & 0x00000002);


_040012E4:	// 40012e4: <10010002>	BEQ       (AT==R0) --> 12f0
	sp_reg_pc = 0x040012E4;
	if (0 == state.r[AT])
	{
		state.r[V0] = state.r[S1] + 0x0000;
		goto _040012F0;
	}


_040012E8:	// 40012e8: <22220000>	ADDI      V0 = S1 + 0000
	state.r[V0] = state.r[S1] + 0x0000;


_040012EC:	// 40012ec: <8f020008>	LW        V0 = [T8+0008]
	state.r[V0] = (_s32)Load32_DMEM((state.r[T8] + 0x00000008) & 0xfff);


_040012F0:	// 40012f0: <22610000>	ADDI      AT = S3 + 0000
	state.r[AT] = state.r[S3] + 0x0000;


_040012F4:	// 40012f4: <0d0006b5>	JAL	    04001ad4
	state.r[V1] = 0 + 0x001F;
	sp_reg_pc = 0x040012F4;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_040012F8:	// 40012f8: <2003001f>	ADDI      V1 = R0 + 001f
	state.r[V1] = 0 + 0x001F;


_040012FC:	// 40012fc: <ca7b2001>	LQV       vec27 <00> = [S3 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_04001300:	// 4001300: <22730020>	ADDI      S3 = S3 + 0020
	state.r[S3] = state.r[S3] + 0x0020;


_04001304:	// 4001304: <12400077>	BEQ       (R0==S2) --> 14e4
	sp_reg_pc = 0x04001304;
	if (state.r[S2] == 0)
	{
		rsp_ldv_compiler((state.r[S4] & 0xFFF), 1, 8);
		goto _040014E4;
	}


_04001308:	// 4001308: <ca811800>	LDV       vec01 <00> = [S4 + 0x00]
	rsp_ldv_compiler((state.r[S4] & 0xFFF), 1, 8);


_0400130C:	// 400130c: <92a10000>	LBU       AT = [S5+0000]
	state.r[AT] = (_s32)(_u8)pDMEM[((state.r[S5] + 0x00000000) & 0xfff) ^ 3];


_04001310:	// 4001310: <302b000f>	ANDI      T3 = AT & 000f
	state.r[T3] = (state.r[AT] & 0x0000000F);


_04001314:	// 4001314: <000b5940>	SLL       T3 = T3 << 5
	state.r[T3] = ((_u32)state.r[T3] << 5);


_04001318:	// 4001318: <4b01c8e8>	VAND      vec03 = vec25 and vec01[0]
	rsp_cop2_vand_element(&state.v[3], &state.v[25], &state.v[1].U16[7]);


_0400131C:	// 400131c: <016f6820>	ADD       T5 = T3+T7
	state.r[T5] = (state.r[T3] + state.r[T7]);


_04001320:	// 4001320: <4a04212c>	VXOR      vec04 = vec04 xor vec04[<none>]
	rsp_cop2_vxor(&state.v[4], &state.v[4], &state.v[4]);


_04001324:	// 4001324: <4a0631ac>	VXOR      vec06 = vec06 xor vec06[<none>]
	rsp_cop2_vxor(&state.v[6], &state.v[6], &state.v[6]);


_04001328:	// 4001328: <15000004>	BNE       (R0!=T0) --> 133c
	sp_reg_pc = 0x04001328;
	if (state.r[T0] != 0)
	{
		rsp_cop2_vand_element(&state.v[5], &state.v[25], &state.v[1].U16[6]);
		goto _0400133C;
	}


_0400132C:	// 400132c: <4b21c968>	VAND      vec05 = vec25 and vec01[1]
	rsp_cop2_vand_element(&state.v[5], &state.v[25], &state.v[1].U16[6]);


_04001330:	// 4001330: <4b21c128>	VAND      vec04 = vec24 and vec01[1]
	rsp_cop2_vand_element(&state.v[4], &state.v[24], &state.v[1].U16[6]);


_04001334:	// 4001334: <4b41c968>	VAND      vec05 = vec25 and vec01[2]
	rsp_cop2_vand_element(&state.v[5], &state.v[25], &state.v[1].U16[5]);


_04001338:	// 4001338: <4b61c1a8>	VAND      vec06 = vec24 and vec01[3]
	rsp_cop2_vand_element(&state.v[6], &state.v[24], &state.v[1].U16[4]);


_0400133C:	// 400133c: <00017102>	SRL       T6 = AT >> 4
	state.r[T6] = ((_u32)state.r[AT] >> 4);


_04001340:	// 4001340: <00091020>	ADD       V0 = R0+T1
	state.r[V0] = (0 + state.r[T1]);


_04001344:	// 4001344: <004e7022>	SUB       T6 = V0-T6
	state.r[T6] = (state.r[V0] - state.r[T6]);


_04001348:	// 4001348: <21c2ffff>	ADDI      V0 = T6 + ffff
	state.r[V0] = state.r[T6] + 0xFFFFFFFF;


_0400134C:	// 400134c: <34038000>	ORI       V1 = R0 | 0x8000
	state.r[V1] = (0 | 0x00008000);


_04001350:	// 4001350: <11c00002>	BEQ       (R0==T6) --> 135c
	sp_reg_pc = 0x04001350;
	if (state.r[T6] == 0)
	{
		state.r[A0] = 0 + 0xFFFFFFFF;
		goto _0400135C;
	}


_04001354:	// 4001354: <2004ffff>	ADDI      A0 = R0 + ffff
	state.r[A0] = 0 + 0xFFFFFFFF;


_04001358:	// 4001358: <00432006>	SRLV      V1 = A0 >> V0
	state.r[A0] = ((_u32)state.r[V1] >> (state.r[V0] & 0x1F));


_0400135C:	// 400135c: <4884b000>	MTC2      vec22 <00> = A0
	state.v[22].U16[7] = (_u16)state.r[A0];


_04001360:	// 4001360: <c9b52000>	LQV       vec21 <00> = [T5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[21].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[21].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[21].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[21].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[21].U64[0] = state.v[21].U64[0] & ~temp1;
			state.v[21].U64[1] = state.v[21].U64[1] & ~temp2;
			state.v[21].U64[0] = state.v[21].U64[0] | value1;
			state.v[21].U64[1] = state.v[21].U64[1] | value2;
		}
	}


_04001364:	// 4001364: <c9b42001>	LQV       vec20 <00> = [T5 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[20].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[20].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[20].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[20].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[20].U64[0] = state.v[20].U64[0] & ~temp1;
			state.v[20].U64[1] = state.v[20].U64[1] & ~temp2;
			state.v[20].U64[0] = state.v[20].U64[0] | value1;
			state.v[20].U64[1] = state.v[20].U64[1] | value2;
		}
	}


_04001368:	// 4001368: <21adfffe>	ADDI      T5 = T5 + fffe
	state.r[T5] = state.r[T5] + 0xFFFFFFFE;


_0400136C:	// 400136c: <c9b32802>	LRV       vec19 <00> = [T5 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[19].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001370:	// 4001370: <21adfffe>	ADDI      T5 = T5 + fffe
	state.r[T5] = state.r[T5] + 0xFFFFFFFE;


_04001374:	// 4001374: <c9b22802>	LRV       vec18 <00> = [T5 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[18].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001378:	// 4001378: <21adfffe>	ADDI      T5 = T5 + fffe
	state.r[T5] = state.r[T5] + 0xFFFFFFFE;


_0400137C:	// 400137c: <c9b12802>	LRV       vec17 <00> = [T5 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[17].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001380:	// 4001380: <21adfffe>	ADDI      T5 = T5 + fffe
	state.r[T5] = state.r[T5] + 0xFFFFFFFE;


_04001384:	// 4001384: <c9b02802>	LRV       vec16 <00> = [T5 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[16].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001388:	// 4001388: <21adfffe>	ADDI      T5 = T5 + fffe
	state.r[T5] = state.r[T5] + 0xFFFFFFFE;


_0400138C:	// 400138c: <c9af2802>	LRV       vec15 <00> = [T5 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[15].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001390:	// 4001390: <21adfffe>	ADDI      T5 = T5 + fffe
	state.r[T5] = state.r[T5] + 0xFFFFFFFE;


_04001394:	// 4001394: <c9ae2802>	LRV       vec14 <00> = [T5 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[14].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001398:	// 4001398: <21adfffe>	ADDI      T5 = T5 + fffe
	state.r[T5] = state.r[T5] + 0xFFFFFFFE;


_0400139C:	// 400139c: <c9ad2802>	LRV       vec13 <00> = [T5 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[13].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040013A0:	// 40013a0: <028aa020>	ADD       S4 = S4+T2
	state.r[S4] = (state.r[S4] + state.r[T2]);


_040013A4:	// 40013a4: <4a171f86>	VMUDN     vec30 = ( acc = vec03 * vec23[<none>]      ) >> 16

	// [start function: Zelda13A4_14D8]

	Zelda13A4_14D8();
	goto _040014DC;

_040013A8:	// 40013a8: <02aaa820>	ADD       S5 = S5+T2

_040013AC:	// 40013ac: <4a17278e>	VMADN     vec30 = ( acc+= vec04 * vec23[<none>] )

_040013B0:	// 40013b0: <ca811800>	LDV       vec01 <00> = [S4 + 0x00]

_040013B4:	// 40013b4: <4a172f46>	VMUDN     vec29 = ( acc = vec05 * vec23[<none>]      ) >> 16

_040013B8:	// 40013b8: <92a10000>	LBU       AT = [S5+0000]

_040013BC:	// 40013bc: <4a17374e>	VMADN     vec29 = ( acc+= vec06 * vec23[<none>] )

_040013C0:	// 40013c0: <19c00003>	BLEZ      (R0<=0) --> 13d0

_040013C4:	// 40013c4: <302b000f>	ANDI      T3 = AT & 000f

_040013C8:	// 40013c8: <4b16f785>	VMUDM     vec30 = ( acc = vec30 * vec22[0] ) >> 16

_040013CC:	// 40013cc: <4b16ef45>	VMUDM     vec29 = ( acc = vec29 * vec22[0] ) >> 16

_040013D0:	// 40013d0: <000b5940>	SLL       T3 = T3 << 5

_040013D4:	// 40013d4: <4b01c8e8>	VAND      vec03 = vec25 and vec01[0]

_040013D8:	// 40013d8: <016f6820>	ADD       T5 = T3+T7

_040013DC:	// 40013dc: <15000004>	BNE       (R0!=T0) --> 13f0

_040013E0:	// 40013e0: <4b21c968>	VAND      vec05 = vec25 and vec01[1]

_040013E4:	// 40013e4: <4b21c128>	VAND      vec04 = vec24 and vec01[1]

_040013E8:	// 40013e8: <4b41c968>	VAND      vec05 = vec25 and vec01[2]

_040013EC:	// 40013ec: <4b61c1a8>	VAND      vec06 = vec24 and vec01[3]

_040013F0:	// 40013f0: <00017102>	SRL       T6 = AT >> 4

_040013F4:	// 40013f4: <4bdba887>	VMUDH     vec02 = ( acc = (vec21 * vec27[6]) << 16) >> 16

_040013F8:	// 40013f8: <00091020>	ADD       V0 = R0+T1

_040013FC:	// 40013fc: <4bfba08f>	VMADH     vec02 = ( acc+= (vec20 * vec27[7]) << 16) >> 16

_04001400:	// 4001400: <004e7022>	SUB       T6 = V0-T6

_04001404:	// 4001404: <4b1e988f>	VMADH     vec02 = ( acc+= (vec19 * vec30[0]) << 16) >> 16

_04001408:	// 4001408: <21c2ffff>	ADDI      V0 = T6 + ffff

_0400140C:	// 400140c: <4b3e908f>	VMADH     vec02 = ( acc+= (vec18 * vec30[1]) << 16) >> 16

_04001410:	// 4001410: <20030001>	ADDI      V1 = R0 + 0001

_04001414:	// 4001414: <4b5e888f>	VMADH     vec02 = ( acc+= (vec17 * vec30[2]) << 16) >> 16

_04001418:	// 4001418: <00031bc0>	SLL       V1 = V1 << 15

_0400141C:	// 400141c: <4b7e808f>	VMADH     vec02 = ( acc+= (vec16 * vec30[3]) << 16) >> 16

_04001420:	// 4001420: <11c00002>	BEQ       (R0==T6) --> 142c

_04001424:	// 4001424: <2004ffff>	ADDI      A0 = R0 + ffff

_04001428:	// 4001428: <00432006>	SRLV      V1 = A0 >> V0

_0400142C:	// 400142c: <4b9e7f0f>	VMADH     vec28 = ( acc+= (vec15 * vec30[4]) << 16) >> 16

_04001430:	// 4001430: <4884b000>	MTC2      vec22 <00> = A0

_04001434:	// 4001434: <4bbe708f>	VMADH     vec02 = ( acc+= (vec14 * vec30[5]) << 16) >> 16

_04001438:	// 4001438: <4bde688f>	VMADH     vec02 = ( acc+= (vec13 * vec30[6]) << 16) >> 16

_0400143C:	// 400143c: <4bbff08f>	VMADH     vec02 = ( acc+= (vec30 * vec31[5]) << 16) >> 16

_04001440:	// 4001440: <4b3c3e9d>	VSAW $v26, $v7, $v28[[1]]

_04001444:	// 4001444: <4b1c3f1d>	VSAW $v28, $v7, $v28[[0]]

_04001448:	// 4001448: <4b9fd086>	VMUDN     vec02 = ( acc = vec26 * vec31[4]      ) >> 16

_0400144C:	// 400144c: <4b9fe70f>	VMADH     vec28 = ( acc+= (vec28 * vec31[4]) << 16) >> 16

_04001450:	// 4001450: <4b1d9887>	VMUDH     vec02 = ( acc = (vec19 * vec29[0]) << 16) >> 16

_04001454:	// 4001454: <21acfffe>	ADDI      T4 = T5 + fffe

_04001458:	// 4001458: <4b3d908f>	VMADH     vec02 = ( acc+= (vec18 * vec29[1]) << 16) >> 16

_0400145C:	// 400145c: <c9932802>	LRV       vec19 <00> = [T4 + 0x20]

_04001460:	// 4001460: <4b5d888f>	VMADH     vec02 = ( acc+= (vec17 * vec29[2]) << 16) >> 16

_04001464:	// 4001464: <218cfffe>	ADDI      T4 = T4 + fffe

_04001468:	// 4001468: <4b7d808f>	VMADH     vec02 = ( acc+= (vec16 * vec29[3]) << 16) >> 16

_0400146C:	// 400146c: <c9922802>	LRV       vec18 <00> = [T4 + 0x20]

_04001470:	// 4001470: <4b9d788f>	VMADH     vec02 = ( acc+= (vec15 * vec29[4]) << 16) >> 16

_04001474:	// 4001474: <218cfffe>	ADDI      T4 = T4 + fffe

_04001478:	// 4001478: <4bbd708f>	VMADH     vec02 = ( acc+= (vec14 * vec29[5]) << 16) >> 16

_0400147C:	// 400147c: <c9912802>	LRV       vec17 <00> = [T4 + 0x20]

_04001480:	// 4001480: <4bdd688f>	VMADH     vec02 = ( acc+= (vec13 * vec29[6]) << 16) >> 16

_04001484:	// 4001484: <218cfffe>	ADDI      T4 = T4 + fffe

_04001488:	// 4001488: <4bbfe88f>	VMADH     vec02 = ( acc+= (vec29 * vec31[5]) << 16) >> 16

_0400148C:	// 400148c: <c9902802>	LRV       vec16 <00> = [T4 + 0x20]

_04001490:	// 4001490: <4bdca88f>	VMADH     vec02 = ( acc+= (vec21 * vec28[6]) << 16) >> 16

_04001494:	// 4001494: <218cfffe>	ADDI      T4 = T4 + fffe

_04001498:	// 4001498: <4bfca08f>	VMADH     vec02 = ( acc+= (vec20 * vec28[7]) << 16) >> 16

_0400149C:	// 400149c: <c98f2802>	LRV       vec15 <00> = [T4 + 0x20]

_040014A0:	// 40014a0: <4b3b3e9d>	VSAW $v26, $v7, $v27[[1]]

_040014A4:	// 40014a4: <218cfffe>	ADDI      T4 = T4 + fffe

_040014A8:	// 40014a8: <4b1b3edd>	VSAW $v27, $v7, $v27[[0]]

_040014AC:	// 40014ac: <c98e2802>	LRV       vec14 <00> = [T4 + 0x20]

_040014B0:	// 40014b0: <218cfffe>	ADDI      T4 = T4 + fffe

_040014B4:	// 40014b4: <c98d2802>	LRV       vec13 <00> = [T4 + 0x20]

_040014B8:	// 40014b8: <c9b52000>	LQV       vec21 <00> = [T5 + 0x00]

_040014BC:	// 40014bc: <4b9fd086>	VMUDN     vec02 = ( acc = vec26 * vec31[4]      ) >> 16

_040014C0:	// 40014c0: <c9b42001>	LQV       vec20 <00> = [T5 + 0x10]

_040014C4:	// 40014c4: <4b9fdecf>	VMADH     vec27 = ( acc+= (vec27 * vec31[4]) << 16) >> 16

_040014C8:	// 40014c8: <2252ffe0>	ADDI      S2 = S2 + ffe0

_040014CC:	// 40014cc: <ea7c1800>	SDV       [S3 + 0x00] = vec28 <00>

_040014D0:	// 40014d0: <ea7c1c01>	SDV       [S3 + 0x04] = vec28 <08>

_040014D4:	// 40014d4: <ea7b1802>	SDV       [S3 + 0x08] = vec27 <00>

_040014D8:	// 40014d8: <ea7b1c03>	SDV       [S3 + 0x0c] = vec27 <08>
	ErrorToCallFunction(0x040014D8);

	// [end of function: Zelda13A4_14D8]

_040014DC:	// 40014dc: <1e40ffb0>	BGTZ      (R0>0) --> 13a0
	sp_reg_pc = 0x040014DC;
	if((_s32)state.r[S2] > 0)
	{
		state.r[S3] = state.r[S3] + 0x0020;
		goto _040013A0;
	}


_040014E0:	// 40014e0: <22730020>	ADDI      S3 = S3 + 0020
	state.r[S3] = state.r[S3] + 0x0020;


_040014E4:	// 40014e4: <2261ffe0>	ADDI      AT = S3 + ffe0
	state.r[AT] = state.r[S3] + 0xFFFFFFE0;


_040014E8:	// 40014e8: <22220000>	ADDI      V0 = S1 + 0000
	state.r[V0] = state.r[S1] + 0x0000;


_040014EC:	// 40014ec: <090006b1>	J         04001ac4
	state.r[V1] = 0 + 0x0020;
	goto _04001AC4;


_040014F0:	// 40014f0: <20030020>	ADDI      V1 = R0 + 0020
	state.r[V1] = 0 + 0x0020;


_040014F4:	// 40014f4: <87080000>	LH        T0 = [T8+0000]
	state.r[T0] = (_s32)(_s16)Load16_DMEM((state.r[T8] + 0x00000000) & 0xfff);


_040014F8:	// 40014f8: <87130002>	LH        S3 = [T8+0002]
	state.r[S3] = (_s32)(_s16)Load16_DMEM((state.r[T8] + 0x00000002) & 0xfff);


_040014FC:	// 40014fc: <87120004>	LH        S2 = [T8+0004]
	state.r[S2] = (_s32)(_s16)Load16_DMEM((state.r[T8] + 0x00000004) & 0xfff);


_04001500:	// 4001500: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001504:	// 4001504: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001508:	// 4001508: <22e10000>	ADDI      AT = S7 + 0000
	state.r[AT] = state.r[S7] + 0x0000;


_0400150C:	// 400150c: <0002b020>	ADD       S6 = R0+V0
	state.r[S6] = (0 + state.r[V0]);


_04001510:	// 4001510: <2003001f>	ADDI      V1 = R0 + 001f
	state.r[V1] = 0 + 0x001F;


_04001514:	// 4001514: <001a3c02>	SRL       A3 = K0 >> 16
	state.r[A3] = ((_u32)state.r[K0] >> 16);


_04001518:	// 4001518: <30ea0001>	ANDI      T2 = A3 & 0001
	state.r[T2] = (state.r[A3] & 0x00000001);


_0400151C:	// 400151c: <1d400005>	BGTZ      (R0>0) --> 1534
	sp_reg_pc = 0x0400151C;
	if((_s32)state.r[T2] > 0)
	{
		goto _04001534;
	}


_04001520:	// 4001520: <00000000>	NOP       


_04001524:	// 4001524: <0d0006b5>	JAL	    04001ad4
	sp_reg_pc = 0x04001524;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_04001528:	// 4001528: <00000000>	NOP       


_0400152C:	// 400152c: <09000550>	J         04001540
	goto _04001540;


_04001530:	// 4001530: <00000000>	NOP       


_04001534:	// 4001534: <a6e00008>	SH        [S7+0008] = R0
	Save16_DMEM((_u16)0, (state.r[S7] + 0x00000008) & 0xfff);


_04001538:	// 4001538: <4a10842c>	VXOR      vec16 = vec16 xor vec16[<none>]
	rsp_cop2_vxor(&state.v[16], &state.v[16], &state.v[16]);


_0400153C:	// 400153c: <eaf01800>	SDV       [S7 + 0x00] = vec16 <00>
	{
		_u32	addr = (0x00000000 + state.r[S7]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[16].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[16].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[16].U32[0x00000003];
		}
	}


_04001540:	// 4001540: <30ea0002>	ANDI      T2 = A3 & 0002
	state.r[T2] = (state.r[A3] & 0x00000002);


_04001544:	// 4001544: <11400006>	BEQ       (R0==T2) --> 1560
	sp_reg_pc = 0x04001544;
	if (state.r[T2] == 0)
	{
		rsp_ldv_compiler((state.r[S7] & 0xFFF), 16, 8);
		goto _04001560;
	}


_04001548:	// 4001548: <caf01800>	LDV       vec16 <00> = [S7 + 0x00]
	rsp_ldv_compiler((state.r[S7] & 0xFFF), 16, 8);


_0400154C:	// 400154c: <2108fffc>	ADDI      T0 = T0 + fffc
	state.r[T0] = state.r[T0] + 0xFFFFFFFC;


_04001550:	// 4001550: <e9100800>	SSV       [T0 + 0x00] = vec16 <00>
	Save16_DMEM(state.v[16].U16[7], (0x00000000 + state.r[T0]) & 0xfff);


_04001554:	// 4001554: <e9100a01>	SSV       [T0 + 0x02] = vec16 <04>
	Save16_DMEM(state.v[16].U16[5], (0x00000002 + state.r[T0]) & 0xfff);


_04001558:	// 4001558: <09000568>	J         040015a0
	goto _040015A0;


_0400155C:	// 400155c: <00000000>	NOP       


_04001560:	// 4001560: <30ea0004>	ANDI      T2 = A3 & 0004
	state.r[T2] = (state.r[A3] & 0x00000004);


_04001564:	// 4001564: <1140000c>	BEQ       (R0==T2) --> 1598
	sp_reg_pc = 0x04001564;
	if (state.r[T2] == 0)
	{
		goto _04001598;
	}


_04001568:	// 4001568: <00000000>	NOP       


_0400156C:	// 400156c: <2108fff0>	ADDI      T0 = T0 + fff0
	state.r[T0] = state.r[T0] + 0xFFFFFFF0;


_04001570:	// 4001570: <e9100800>	SSV       [T0 + 0x00] = vec16 <00>
	Save16_DMEM(state.v[16].U16[7], (0x00000000 + state.r[T0]) & 0xfff);


_04001574:	// 4001574: <e9100801>	SSV       [T0 + 0x02] = vec16 <00>
	Save16_DMEM(state.v[16].U16[7], (0x00000002 + state.r[T0]) & 0xfff);


_04001578:	// 4001578: <e9100902>	SSV       [T0 + 0x04] = vec16 <02>
	Save16_DMEM(state.v[16].U16[6], (0x00000004 + state.r[T0]) & 0xfff);


_0400157C:	// 400157c: <e9100903>	SSV       [T0 + 0x06] = vec16 <02>
	Save16_DMEM(state.v[16].U16[6], (0x00000006 + state.r[T0]) & 0xfff);


_04001580:	// 4001580: <e9100a04>	SSV       [T0 + 0x08] = vec16 <04>
	Save16_DMEM(state.v[16].U16[5], (0x00000008 + state.r[T0]) & 0xfff);


_04001584:	// 4001584: <e9100a05>	SSV       [T0 + 0x0a] = vec16 <04>
	Save16_DMEM(state.v[16].U16[5], (0x0000000A + state.r[T0]) & 0xfff);


_04001588:	// 4001588: <e9100b06>	SSV       [T0 + 0x0c] = vec16 <06>
	Save16_DMEM(state.v[16].U16[4], (0x0000000C + state.r[T0]) & 0xfff);


_0400158C:	// 400158c: <e9100b07>	SSV       [T0 + 0x0e] = vec16 <06>
	Save16_DMEM(state.v[16].U16[4], (0x0000000E + state.r[T0]) & 0xfff);


_04001590:	// 4001590: <09000568>	J         040015a0
	goto _040015A0;


_04001594:	// 4001594: <00000000>	NOP       


_04001598:	// 4001598: <2108fff8>	ADDI      T0 = T0 + fff8
	state.r[T0] = state.r[T0] + 0xFFFFFFF8;


_0400159C:	// 400159c: <e9101800>	SDV       [T0 + 0x00] = vec16 <00>
	{
		_u32	addr = (0x00000000 + state.r[T0]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[16].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[16].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[16].U32[0x00000003];
		}
	}


_040015A0:	// 40015a0: <caf70f04>	LSV       vec23 <14> = [S7 + 0x08]
	state.v[23].U16[0] = Load16_DMEM(0x00000008 + state.r[S7]);


_040015A4:	// 40015a4: <c9101800>	LDV       vec16 <00> = [T0 + 0x00]
	rsp_ldv_compiler((state.r[T0] & 0xFFF), 16, 8);


_040015A8:	// 40015a8: <48889200>	MTC2      vec18 <04> = T0
	state.v[18].U16[5] = (_u16)state.r[T0];


_040015AC:	// 40015ac: <200a00e0>	ADDI      T2 = R0 + 00e0
	state.r[T2] = 0 + 0x00E0;


_040015B0:	// 40015b0: <488a9300>	MTC2      vec18 <06> = T2
	state.v[18].U16[4] = (_u16)state.r[T2];


_040015B4:	// 40015b4: <489a9400>	MTC2      vec18 <08> = K0
	state.v[18].U16[3] = (_u16)state.r[K0];


_040015B8:	// 40015b8: <200a0040>	ADDI      T2 = R0 + 0040
	state.r[T2] = 0 + 0x0040;


_040015BC:	// 40015bc: <488a9500>	MTC2      vec18 <10> = T2
	state.v[18].U16[2] = (_u16)state.r[T2];


_040015C0:	// 40015c0: <20090060>	ADDI      T1 = R0 + 0060
	state.r[T1] = 0 + 0x0060;


_040015C4:	// 40015c4: <c93f2001>	LQV       vec31 <00> = [T1 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_040015C8:	// 40015c8: <c9392000>	LQV       vec25 <00> = [T1 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[25].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[25].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[25].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[25].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[25].U64[0] = state.v[25].U64[0] & ~temp1;
			state.v[25].U64[1] = state.v[25].U64[1] & ~temp2;
			state.v[25].U64[0] = state.v[25].U64[0] | value1;
			state.v[25].U64[1] = state.v[25].U64[1] | value2;
		}
	}


_040015CC:	// 40015cc: <4a1fce51>	VSUB      vec25 = vec25 - vec31[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[25].U16[0], state.v[25].U16[0], state.v[31].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[25].U16[1], state.v[25].U16[1], state.v[31].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[25].U16[2], state.v[25].U16[2], state.v[31].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[25].U16[3], state.v[25].U16[3], state.v[31].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[25].U16[4], state.v[25].U16[4], state.v[31].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[25].U16[5], state.v[25].U16[5], state.v[31].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[25].U16[6], state.v[25].U16[6], state.v[31].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[25].U16[7], state.v[25].U16[7], state.v[31].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040015D0:	// 40015d0: <c93e2002>	LQV       vec30 <00> = [T1 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[30].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[30].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[30].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[30].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[30].U64[0] = state.v[30].U64[0] & ~temp1;
			state.v[30].U64[1] = state.v[30].U64[1] & ~temp2;
			state.v[30].U64[0] = state.v[30].U64[0] | value1;
			state.v[30].U64[1] = state.v[30].U64[1] | value2;
		}
	}


_040015D4:	// 40015d4: <c93d2003>	LQV       vec29 <00> = [T1 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[29].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[29].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[29].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[29].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[29].U64[0] = state.v[29].U64[0] & ~temp1;
			state.v[29].U64[1] = state.v[29].U64[1] & ~temp2;
			state.v[29].U64[0] = state.v[29].U64[0] | value1;
			state.v[29].U64[1] = state.v[29].U64[1] | value2;
		}
	}


_040015D8:	// 40015d8: <c93c2004>	LQV       vec28 <00> = [T1 + 0x40]
	{
		_u32 addr = (0x00000040 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[28].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[28].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[28].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[28].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[28].U64[0] = state.v[28].U64[0] & ~temp1;
			state.v[28].U64[1] = state.v[28].U64[1] & ~temp2;
			state.v[28].U64[0] = state.v[28].U64[0] | value1;
			state.v[28].U64[1] = state.v[28].U64[1] | value2;
		}
	}


_040015DC:	// 40015dc: <c93b2005>	LQV       vec27 <00> = [T1 + 0x50]
	{
		_u32 addr = (0x00000050 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_040015E0:	// 40015e0: <c93a2006>	LQV       vec26 <00> = [T1 + 0x60]
	{
		_u32 addr = (0x00000060 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[26].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[26].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[26].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[26].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[26].U64[0] = state.v[26].U64[0] & ~temp1;
			state.v[26].U64[1] = state.v[26].U64[1] & ~temp2;
			state.v[26].U64[0] = state.v[26].U64[0] | value1;
			state.v[26].U64[1] = state.v[26].U64[1] | value2;
		}
	}


_040015E4:	// 40015e4: <4a1fce51>	VSUB      vec25 = vec25 - vec31[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[25].U16[0], state.v[25].U16[0], state.v[31].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[25].U16[1], state.v[25].U16[1], state.v[31].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[25].U16[2], state.v[25].U16[2], state.v[31].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[25].U16[3], state.v[25].U16[3], state.v[31].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[25].U16[4], state.v[25].U16[4], state.v[31].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[25].U16[5], state.v[25].U16[5], state.v[31].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[25].U16[6], state.v[25].U16[6], state.v[31].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[25].U16[7], state.v[25].U16[7], state.v[31].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040015E8:	// 40015e8: <c9382007>	LQV       vec24 <00> = [T1 + 0x70]
	{
		_u32 addr = (0x00000070 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[24].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[24].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[24].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[24].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[24].U64[0] = state.v[24].U64[0] & ~temp1;
			state.v[24].U64[1] = state.v[24].U64[1] & ~temp2;
			state.v[24].U64[0] = state.v[24].U64[0] | value1;
			state.v[24].U64[1] = state.v[24].U64[1] | value2;
		}
	}


_040015EC:	// 40015ec: <22f50020>	ADDI      S5 = S7 + 0020
	state.r[S5] = state.r[S7] + 0x0020;


_040015F0:	// 40015f0: <22f40030>	ADDI      S4 = S7 + 0030
	state.r[S4] = state.r[S7] + 0x0030;


_040015F4:	// 40015f4: <4a16b5ac>	VXOR      vec22 = vec22 xor vec22[<none>]
	rsp_cop2_vxor(&state.v[22], &state.v[22], &state.v[22]);


_040015F8:	// 40015f8: <4bf7fdc5>	VMUDM     vec23 = ( acc = vec31 * vec23[7] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[31*16].U16[0], 0xE4
			MOVQ mm1, state.v[31*16].U16[4*2]
			PSHUFW mm4, state.v[23*16].U16[0*2], 0
			PSHUFW mm6, mm0, 0xE4
			MOVQ mm7, mm1
			PMULLW mm6, mm4
			PMULLW mm7, mm4
			MOVQ accumulator[0*4], mm6
			MOVQ accumulator[2*4], mm7
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_040015FC:	// 40015fc: <4b92cd8d>	VMADM     vec22 = ( acc+= vec25 * vec18[4] ) >> 16
	{
		_u16		s2value = state.v[18].U16[3];
		VMADM_operation(0, state.v[22].U16[0], state.v[25].U16[0], s2value, 1, 1, 1, 1)
		VMADM_operation(1, state.v[22].U16[1], state.v[25].U16[1], s2value, 1, 1, 1, 1)
		VMADM_operation(2, state.v[22].U16[2], state.v[25].U16[2], s2value, 1, 1, 1, 1)
		VMADM_operation(3, state.v[22].U16[3], state.v[25].U16[3], s2value, 1, 1, 1, 1)
		VMADM_operation(4, state.v[22].U16[4], state.v[25].U16[4], s2value, 1, 1, 1, 1)
		VMADM_operation(5, state.v[22].U16[5], state.v[25].U16[5], s2value, 1, 1, 1, 1)
		VMADM_operation(6, state.v[22].U16[6], state.v[25].U16[6], s2value, 1, 1, 1, 1)
		VMADM_operation(7, state.v[22].U16[7], state.v[25].U16[7], s2value, 1, 1, 1, 1)
	}


_04001600:	// 4001600: <4b1efdce>	VMADN     vec23 = ( acc+= vec31 * vec30[0] )
	{
		_u16		s2value = state.v[30].U16[7];
		VMADN_operation(0, state.v[23].U16[0], state.v[31].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[23].U16[1], state.v[31].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[23].U16[2], state.v[31].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[23].U16[3], state.v[31].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[23].U16[4], state.v[31].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[23].U16[5], state.v[31].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[23].U16[6], state.v[31].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[23].U16[7], state.v[31].U16[7], s2value, 1, 0, 0, 1)
	}


_04001604:	// 4001604: <4b52fd46>	VMUDN     vec21 = ( acc = vec31 * vec18[2]      ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[31*16].U16[0], 0xE4
			MOVQ mm1, state.v[31*16].U16[4*2]
			PSHUFW mm4, state.v[18*16].U16[5*2], 0
			PSHUFW mm6, mm0, 0xE4
			MOVQ mm7, mm1
			PMULLW mm6, mm4
			PMULLW mm7, mm4
			MOVQ accumulator[0*4], mm6
			MOVQ accumulator[2*4], mm7
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm2, mm4
			PMULHUW mm3, mm4
			PSRAW mm0, 15
			PSRAW mm1, 15
			PMULLW mm0, mm4
			PMULLW mm1, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_04001608:	// 4001608: <4b5eb54e>	VMADN     vec21 = ( acc+= vec22 * vec30[2] )
	{
		_u16		s2value = state.v[30].U16[5];
		VMADN_operation(0, state.v[21].U16[0], state.v[22].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[21].U16[1], state.v[22].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[21].U16[2], state.v[22].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[21].U16[3], state.v[22].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[21].U16[4], state.v[22].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[21].U16[5], state.v[22].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[21].U16[6], state.v[22].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[21].U16[7], state.v[22].U16[7], s2value, 1, 0, 0, 1)
	}


_0400160C:	// 400160c: <4bb2bc44>	VMUDL     vec17 = ( acc = (vec23 * vec18[5] >> 16)      )
	{
		__asm {
			MOVQ mm0, state.v[23*16].U16[0]
			MOVQ mm1, state.v[23*16].U16[4*2]
			PSHUFW mm4, state.v[18*16].U16[2*2], 0
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			MOVQ state.v[17*16].U16[0], mm0
			MOVQ state.v[17*16].U16[4*2], mm1
		}
	}


_04001610:	// 4001610: <4b9e8c46>	VMUDN     vec17 = ( acc = vec17 * vec30[4]      ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[17*16].U16[0], 0xE4
			MOVQ mm1, state.v[17*16].U16[4*2]
			PSHUFW mm4, state.v[30*16].U16[3*2], 0
			PSHUFW mm6, mm0, 0xE4
			MOVQ mm7, mm1
			PMULLW mm6, mm4
			PMULLW mm7, mm4
			MOVQ accumulator[0*4], mm6
			MOVQ accumulator[2*4], mm7
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm2, mm4
			PMULHUW mm3, mm4
			PSRAW mm0, 15
			PSRAW mm1, 15
			PMULLW mm0, mm4
			PMULLW mm1, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_04001614:	// 4001614: <4b72fc4e>	VMADN     vec17 = ( acc+= vec31 * vec18[3] )
	{
		_u16		s2value = state.v[18].U16[4];
		VMADN_operation(0, state.v[17].U16[0], state.v[31].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[17].U16[1], state.v[31].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[17].U16[2], state.v[31].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[17].U16[3], state.v[31].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[17].U16[4], state.v[31].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[17].U16[5], state.v[31].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[17].U16[6], state.v[31].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[17].U16[7], state.v[31].U16[7], s2value, 1, 0, 0, 1)
	}


_04001618:	// 4001618: <c9392000>	LQV       vec25 <00> = [T1 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[25].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[25].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[25].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[25].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[25].U64[0] = state.v[25].U64[0] & ~temp1;
			state.v[25].U64[1] = state.v[25].U64[1] & ~temp2;
			state.v[25].U64[0] = state.v[25].U64[0] | value1;
			state.v[25].U64[1] = state.v[25].U64[1] | value2;
		}
	}


_0400161C:	// 400161c: <eab52000>	SQV       [S5 + 0x00] = vec21 <00>
	{
		_u32    addr = (0x00000000 + state.r[S5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[21].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[21].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[21].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[21].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[21].U64[0] & temp1);
			value2 |= (state.v[21].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001620:	// 4001620: <ea912000>	SQV       [S4 + 0x00] = vec17 <00>
	{
		_u32    addr = (0x00000000 + state.r[S4]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[17].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[17].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[17].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[17].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[17].U64[0] & temp1);
			value2 |= (state.v[17].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001624:	// 4001624: <eaf70b84>	SSV       [S7 + 0x08] = vec23 <07>
	Save16_DMEM(state.v[23].U16[4], (0x00000008 + state.r[S7]) & 0xfff);


_04001628:	// 4001628: <86b10000>	LH        S1 = [S5+0000]
	state.r[S1] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x00000000) & 0xfff);


_0400162C:	// 400162c: <86890000>	LH        T1 = [S4+0000]
	state.r[T1] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x00000000) & 0xfff);


_04001630:	// 4001630: <86ad0008>	LH        T5 = [S5+0008]
	state.r[T5] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x00000008) & 0xfff);


_04001634:	// 4001634: <86850008>	LH        A1 = [S4+0008]
	state.r[A1] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x00000008) & 0xfff);


_04001638:	// 4001638: <86b00002>	LH        S0 = [S5+0002]
	state.r[S0] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x00000002) & 0xfff);


_0400163C:	// 400163c: <86880002>	LH        T0 = [S4+0002]
	state.r[T0] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x00000002) & 0xfff);


_04001640:	// 4001640: <86ac000a>	LH        T4 = [S5+000a]
	state.r[T4] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x0000000A) & 0xfff);


_04001644:	// 4001644: <8684000a>	LH        A0 = [S4+000a]
	state.r[A0] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x0000000A) & 0xfff);


_04001648:	// 4001648: <86af0004>	LH        T7 = [S5+0004]
	state.r[T7] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x00000004) & 0xfff);


_0400164C:	// 400164c: <86870004>	LH        A3 = [S4+0004]
	state.r[A3] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x00000004) & 0xfff);


_04001650:	// 4001650: <86ab000c>	LH        T3 = [S5+000c]
	state.r[T3] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x0000000C) & 0xfff);


_04001654:	// 4001654: <8683000c>	LH        V1 = [S4+000c]
	state.r[V1] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x0000000C) & 0xfff);


_04001658:	// 4001658: <86ae0006>	LH        T6 = [S5+0006]
	state.r[T6] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x00000006) & 0xfff);


_0400165C:	// 400165c: <86860006>	LH        A2 = [S4+0006]
	state.r[A2] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x00000006) & 0xfff);


_04001660:	// 4001660: <86aa000e>	LH        T2 = [S5+000e]
	state.r[T2] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x0000000E) & 0xfff);


_04001664:	// 4001664: <8682000e>	LH        V0 = [S4+000e]
	state.r[V0] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x0000000E) & 0xfff);


_04001668:	// 4001668: <ca301800>	LDV       vec16 <00> = [S1 + 0x00]
	// In function: Mario19D8_1AC4
	//Call the function: Mario19D8_1AC4
	Mario19D8_1AC4();
	goto _04001758;

_0400166C:	// 400166c: <4bf7fdc5>	VMUDM     vec23 = ( acc = vec31 * vec23[7] ) >> 16
	ErrorToCallFunction(0x0400166C);

_04001670:	// 4001670: <c92f1800>	LDV       vec15 <00> = [T1 + 0x00]
	ErrorToCallFunction(0x04001670);

_04001674:	// 4001674: <4bf6fdcf>	VMADH     vec23 = ( acc+= (vec31 * vec22[7]) << 16) >> 16
	ErrorToCallFunction(0x04001674);

_04001678:	// 4001678: <c9b01c00>	LDV       vec16 <08> = [T5 + 0x00]
	ErrorToCallFunction(0x04001678);

_0400167C:	// 400167c: <4b92cd8d>	VMADM     vec22 = ( acc+= vec25 * vec18[4] ) >> 16
	ErrorToCallFunction(0x0400167C);

_04001680:	// 4001680: <c8af1c00>	LDV       vec15 <08> = [A1 + 0x00]
	ErrorToCallFunction(0x04001680);

_04001684:	// 4001684: <4b1efdce>	VMADN     vec23 = ( acc+= vec31 * vec30[0] )
	ErrorToCallFunction(0x04001684);

_04001688:	// 4001688: <ca0e1800>	LDV       vec14 <00> = [S0 + 0x00]
	ErrorToCallFunction(0x04001688);

_0400168C:	// 400168c: <4b52fd46>	VMUDN     vec21 = ( acc = vec31 * vec18[2]      ) >> 16
	ErrorToCallFunction(0x0400168C);

_04001690:	// 4001690: <c90d1800>	LDV       vec13 <00> = [T0 + 0x00]
	ErrorToCallFunction(0x04001690);

_04001694:	// 4001694: <4b5eb54e>	VMADN     vec21 = ( acc+= vec22 * vec30[2] )
	ErrorToCallFunction(0x04001694);

_04001698:	// 4001698: <c98e1c00>	LDV       vec14 <08> = [T4 + 0x00]
	ErrorToCallFunction(0x04001698);

_0400169C:	// 400169c: <4bb2bc44>	VMUDL     vec17 = ( acc = (vec23 * vec18[5] >> 16)      )
	ErrorToCallFunction(0x0400169C);

_040016A0:	// 40016a0: <c88d1c00>	LDV       vec13 <08> = [A0 + 0x00]
	ErrorToCallFunction(0x040016A0);

_040016A4:	// 40016a4: <c9ec1800>	LDV       vec12 <00> = [T7 + 0x00]
	ErrorToCallFunction(0x040016A4);

_040016A8:	// 40016a8: <c8eb1800>	LDV       vec11 <00> = [A3 + 0x00]
	ErrorToCallFunction(0x040016A8);

_040016AC:	// 40016ac: <c96c1c00>	LDV       vec12 <08> = [T3 + 0x00]
	ErrorToCallFunction(0x040016AC);

_040016B0:	// 40016b0: <4b9e8c46>	VMUDN     vec17 = ( acc = vec17 * vec30[4]      ) >> 16
	ErrorToCallFunction(0x040016B0);

_040016B4:	// 40016b4: <c86b1c00>	LDV       vec11 <08> = [V1 + 0x00]
	ErrorToCallFunction(0x040016B4);

_040016B8:	// 40016b8: <c9ca1800>	LDV       vec10 <00> = [T6 + 0x00]
	ErrorToCallFunction(0x040016B8);

_040016BC:	// 40016bc: <c8c91800>	LDV       vec09 <00> = [A2 + 0x00]
	ErrorToCallFunction(0x040016BC);

_040016C0:	// 40016c0: <4b72fc4e>	VMADN     vec17 = ( acc+= vec31 * vec18[3] )
	ErrorToCallFunction(0x040016C0);

_040016C4:	// 40016c4: <c94a1c00>	LDV       vec10 <08> = [T2 + 0x00]
	ErrorToCallFunction(0x040016C4);

_040016C8:	// 40016c8: <4a0f8200>	VMULF     vec08 = ( acc = (vec16 * vec15[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x040016C8);

_040016CC:	// 40016cc: <c8491c00>	LDV       vec09 <08> = [V0 + 0x00]
	ErrorToCallFunction(0x040016CC);

_040016D0:	// 40016d0: <4a0d71c0>	VMULF     vec07 = ( acc = (vec14 * vec13[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x040016D0);

_040016D4:	// 40016d4: <eab52000>	SQV       [S5 + 0x00] = vec21 <00>
	ErrorToCallFunction(0x040016D4);

_040016D8:	// 40016d8: <4a0b6180>	VMULF     vec06 = ( acc = (vec12 * vec11[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x040016D8);

_040016DC:	// 40016dc: <ea912000>	SQV       [S4 + 0x00] = vec17 <00>
	ErrorToCallFunction(0x040016DC);

_040016E0:	// 40016e0: <86b10000>	LH        S1 = [S5+0000]
	ErrorToCallFunction(0x040016E0);

_040016E4:	// 40016e4: <4a095140>	VMULF     vec05 = ( acc = (vec10 * vec09[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x040016E4);

_040016E8:	// 40016e8: <86890000>	LH        T1 = [S4+0000]
	ErrorToCallFunction(0x040016E8);

_040016EC:	// 40016ec: <4a684210>	VADD      vec08 = vec08 + vec08[1q]
	ErrorToCallFunction(0x040016EC);

_040016F0:	// 40016f0: <86ad0008>	LH        T5 = [S5+0008]
	ErrorToCallFunction(0x040016F0);

_040016F4:	// 40016f4: <4a6739d0>	VADD      vec07 = vec07 + vec07[1q]
	ErrorToCallFunction(0x040016F4);

_040016F8:	// 40016f8: <86850008>	LH        A1 = [S4+0008]
	ErrorToCallFunction(0x040016F8);

_040016FC:	// 40016fc: <4a663190>	VADD      vec06 = vec06 + vec06[1q]
	ErrorToCallFunction(0x040016FC);

_04001700:	// 4001700: <86b00002>	LH        S0 = [S5+0002]
	ErrorToCallFunction(0x04001700);

_04001704:	// 4001704: <4a652950>	VADD      vec05 = vec05 + vec05[1q]
	ErrorToCallFunction(0x04001704);

_04001708:	// 4001708: <86880002>	LH        T0 = [S4+0002]
	ErrorToCallFunction(0x04001708);

_0400170C:	// 400170c: <4ac84210>	VADD      vec08 = vec08 + vec08[2h]
	ErrorToCallFunction(0x0400170C);

_04001710:	// 4001710: <86ac000a>	LH        T4 = [S5+000a]
	ErrorToCallFunction(0x04001710);

_04001714:	// 4001714: <4ac739d0>	VADD      vec07 = vec07 + vec07[2h]
	ErrorToCallFunction(0x04001714);

_04001718:	// 4001718: <8684000a>	LH        A0 = [S4+000a]
	ErrorToCallFunction(0x04001718);

_0400171C:	// 400171c: <4ac63190>	VADD      vec06 = vec06 + vec06[2h]
	ErrorToCallFunction(0x0400171C);

_04001720:	// 4001720: <86af0004>	LH        T7 = [S5+0004]
	ErrorToCallFunction(0x04001720);

_04001724:	// 4001724: <4ac52950>	VADD      vec05 = vec05 + vec05[2h]
	ErrorToCallFunction(0x04001724);

_04001728:	// 4001728: <86870004>	LH        A3 = [S4+0004]
	ErrorToCallFunction(0x04001728);

_0400172C:	// 400172c: <4a88e906>	VMUDN     vec04 = ( acc = vec29 * vec08[0h]      ) >> 16
	ErrorToCallFunction(0x0400172C);

_04001730:	// 4001730: <86ab000c>	LH        T3 = [S5+000c]
	ErrorToCallFunction(0x04001730);

_04001734:	// 4001734: <4a87e10e>	VMADN     vec04 = ( acc+= vec28 * vec07[0h] )
	ErrorToCallFunction(0x04001734);

_04001738:	// 4001738: <8683000c>	LH        V1 = [S4+000c]
	ErrorToCallFunction(0x04001738);

_0400173C:	// 400173c: <4a86d90e>	VMADN     vec04 = ( acc+= vec27 * vec06[0h] )
	ErrorToCallFunction(0x0400173C);

_04001740:	// 4001740: <86ae0006>	LH        T6 = [S5+0006]
	ErrorToCallFunction(0x04001740);

_04001744:	// 4001744: <4a85d10e>	VMADN     vec04 = ( acc+= vec26 * vec05[0h] )
	ErrorToCallFunction(0x04001744);

_04001748:	// 4001748: <86860006>	LH        A2 = [S4+0006]
	ErrorToCallFunction(0x04001748);

_0400174C:	// 400174c: <86aa000e>	LH        T2 = [S5+000e]
	ErrorToCallFunction(0x0400174C);

_04001750:	// 4001750: <2252fff0>	ADDI      S2 = S2 + fff0
	ErrorToCallFunction(0x04001750);

_04001754:	// 4001754: <ea642000>	SQV       [S3 + 0x00] = vec04 <00>
	ErrorToCallFunction(0x04001754);

_04001758:	// 4001758: <1a400003>	BLEZ      (R0<=0) --> 1768
	sp_reg_pc = 0x04001758;
	if ((_s32)state.r[S2] <= 0)
	{
		state.r[V0] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x0000000E) & 0xfff);
		goto _04001768;
	}


_0400175C:	// 400175c: <8682000e>	LH        V0 = [S4+000e]
	state.r[V0] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x0000000E) & 0xfff);


_04001760:	// 4001760: <0900059a>	J         04001668
	state.r[S3] = state.r[S3] + 0x0010;
	goto _04001668;


_04001764:	// 4001764: <22730010>	ADDI      S3 = S3 + 0010
	state.r[S3] = state.r[S3] + 0x0010;


_04001768:	// 4001768: <eaf70804>	SSV       [S7 + 0x08] = vec23 <00>
	Save16_DMEM(state.v[23].U16[7], (0x00000008 + state.r[S7]) & 0xfff);


_0400176C:	// 400176c: <ca301800>	LDV       vec16 <00> = [S1 + 0x00]
	rsp_ldv_compiler((state.r[S1] & 0xFFF), 16, 8);


_04001770:	// 4001770: <eaf01800>	SDV       [S7 + 0x00] = vec16 <00>
	{
		_u32	addr = (0x00000000 + state.r[S7]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[16].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[16].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[16].U32[0x00000003];
		}
	}


_04001774:	// 4001774: <00161020>	ADD       V0 = R0+S6
	state.r[V0] = (0 + state.r[S6]);


_04001778:	// 4001778: <22e10000>	ADDI      AT = S7 + 0000
	state.r[AT] = state.r[S7] + 0x0000;


_0400177C:	// 400177c: <090006b1>	J         04001ac4
	state.r[V1] = 0 + 0x0020;
	goto _04001AC4;


_04001780:	// 4001780: <20030020>	ADDI      V1 = R0 + 0020
	state.r[V1] = 0 + 0x0020;


_04001784:	// 4001784: <001a7c02>	SRL       T7 = K0 >> 16
	state.r[T7] = ((_u32)state.r[K0] >> 16);


_04001788:	// 4001788: <31ef00ff>	ANDI      T7 = T7 & 00ff
	state.r[T7] = (state.r[T7] & 0x000000FF);


_0400178C:	// 400178c: <334dffff>	ANDI      T5 = K0 & ffff
	state.r[T5] = (state.r[K0] & 0x0000FFFF);


_04001790:	// 4001790: <00197402>	SRL       T6 = T9 >> 16
	state.r[T6] = ((_u32)state.r[T9] >> 16);


_04001794:	// 4001794: <21efffff>	ADDI      T7 = T7 + ffff
	state.r[T7] = state.r[T7] + 0xFFFFFFFF;


_04001798:	// 4001798: <332cffff>	ANDI      T4 = T9 & ffff
	state.r[T4] = (state.r[T9] & 0x0000FFFF);


_0400179C:	// 400179c: <c9a12000>	LQV       vec01 <00> = [T5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[1].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[1].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[1].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[1].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[1].U64[0] = state.v[1].U64[0] & ~temp1;
			state.v[1].U64[1] = state.v[1].U64[1] & ~temp2;
			state.v[1].U64[0] = state.v[1].U64[0] | value1;
			state.v[1].U64[1] = state.v[1].U64[1] | value2;
		}
	}


_040017A0:	// 40017a0: <c9a22001>	LQV       vec02 <00> = [T5 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[2].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[2].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[2].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[2].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[2].U64[0] = state.v[2].U64[0] & ~temp1;
			state.v[2].U64[1] = state.v[2].U64[1] & ~temp2;
			state.v[2].U64[0] = state.v[2].U64[0] | value1;
			state.v[2].U64[1] = state.v[2].U64[1] | value2;
		}
	}


_040017A4:	// 40017a4: <218cffe0>	ADDI      T4 = T4 + ffe0
	state.r[T4] = state.r[T4] + 0xFFFFFFE0;


_040017A8:	// 40017a8: <21ad0020>	ADDI      T5 = T5 + 0020
	state.r[T5] = state.r[T5] + 0x0020;


_040017AC:	// 40017ac: <e9c12000>	SQV       [T6 + 0x00] = vec01 <00>
	{
		_u32    addr = (0x00000000 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[1].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[1].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[1].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[1].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[1].U64[0] & temp1);
			value2 |= (state.v[1].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040017B0:	// 40017b0: <e9c22001>	SQV       [T6 + 0x10] = vec02 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[2].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[2].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[2].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[2].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[2].U64[0] & temp1);
			value2 |= (state.v[2].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040017B4:	// 40017b4: <1d80fff9>	BGTZ      (R0>0) --> 179c
	sp_reg_pc = 0x040017B4;
	if((_s32)state.r[T4] > 0)
	{
		state.r[T6] = state.r[T6] + 0x0020;
		goto _0400179C;
	}


_040017B8:	// 40017b8: <21ce0020>	ADDI      T6 = T6 + 0020
	state.r[T6] = state.r[T6] + 0x0020;


_040017BC:	// 40017bc: <1de0fff5>	BGTZ      (R0>0) --> 1794
	sp_reg_pc = 0x040017BC;
	if((_s32)state.r[T7] > 0)
	{
		goto _04001794;
	}


_040017C0:	// 40017c0: <00000000>	NOP       


_040017C4:	// 40017c4: <09000423>	J         0400108c
	goto _0400108C;


_040017C8:	// 40017c8: <00000000>	NOP       


_040017CC:	// 40017cc: <001a7c02>	SRL       T7 = K0 >> 16
	state.r[T7] = ((_u32)state.r[K0] >> 16);


_040017D0:	// 40017d0: <31ef00ff>	ANDI      T7 = T7 & 00ff
	state.r[T7] = (state.r[T7] & 0x000000FF);


_040017D4:	// 40017d4: <334dffff>	ANDI      T5 = K0 & ffff
	state.r[T5] = (state.r[K0] & 0x0000FFFF);


_040017D8:	// 40017d8: <00197402>	SRL       T6 = T9 >> 16
	state.r[T6] = ((_u32)state.r[T9] >> 16);


_040017DC:	// 40017dc: <c9a12000>	LQV       vec01 <00> = [T5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[1].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[1].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[1].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[1].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[1].U64[0] = state.v[1].U64[0] & ~temp1;
			state.v[1].U64[1] = state.v[1].U64[1] & ~temp2;
			state.v[1].U64[0] = state.v[1].U64[0] | value1;
			state.v[1].U64[1] = state.v[1].U64[1] | value2;
		}
	}


_040017E0:	// 40017e0: <c9a22001>	LQV       vec02 <00> = [T5 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[2].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[2].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[2].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[2].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[2].U64[0] = state.v[2].U64[0] & ~temp1;
			state.v[2].U64[1] = state.v[2].U64[1] & ~temp2;
			state.v[2].U64[0] = state.v[2].U64[0] | value1;
			state.v[2].U64[1] = state.v[2].U64[1] | value2;
		}
	}


_040017E4:	// 40017e4: <c9a32002>	LQV       vec03 <00> = [T5 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[3].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[3].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[3].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[3].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[3].U64[0] = state.v[3].U64[0] & ~temp1;
			state.v[3].U64[1] = state.v[3].U64[1] & ~temp2;
			state.v[3].U64[0] = state.v[3].U64[0] | value1;
			state.v[3].U64[1] = state.v[3].U64[1] | value2;
		}
	}


_040017E8:	// 40017e8: <c9a42003>	LQV       vec04 <00> = [T5 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[4].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[4].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[4].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[4].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[4].U64[0] = state.v[4].U64[0] & ~temp1;
			state.v[4].U64[1] = state.v[4].U64[1] & ~temp2;
			state.v[4].U64[0] = state.v[4].U64[0] | value1;
			state.v[4].U64[1] = state.v[4].U64[1] | value2;
		}
	}


_040017EC:	// 40017ec: <c9a52004>	LQV       vec05 <00> = [T5 + 0x40]
	{
		_u32 addr = (0x00000040 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[5].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[5].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[5].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[5].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[5].U64[0] = state.v[5].U64[0] & ~temp1;
			state.v[5].U64[1] = state.v[5].U64[1] & ~temp2;
			state.v[5].U64[0] = state.v[5].U64[0] | value1;
			state.v[5].U64[1] = state.v[5].U64[1] | value2;
		}
	}


_040017F0:	// 40017f0: <c9a62005>	LQV       vec06 <00> = [T5 + 0x50]
	{
		_u32 addr = (0x00000050 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[6].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[6].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[6].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[6].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[6].U64[0] = state.v[6].U64[0] & ~temp1;
			state.v[6].U64[1] = state.v[6].U64[1] & ~temp2;
			state.v[6].U64[0] = state.v[6].U64[0] | value1;
			state.v[6].U64[1] = state.v[6].U64[1] | value2;
		}
	}


_040017F4:	// 40017f4: <c9a72006>	LQV       vec07 <00> = [T5 + 0x60]
	{
		_u32 addr = (0x00000060 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[7].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[7].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[7].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[7].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[7].U64[0] = state.v[7].U64[0] & ~temp1;
			state.v[7].U64[1] = state.v[7].U64[1] & ~temp2;
			state.v[7].U64[0] = state.v[7].U64[0] | value1;
			state.v[7].U64[1] = state.v[7].U64[1] | value2;
		}
	}


_040017F8:	// 40017f8: <c9a82007>	LQV       vec08 <00> = [T5 + 0x70]
	{
		_u32 addr = (0x00000070 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[8].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[8].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[8].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[8].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[8].U64[0] = state.v[8].U64[0] & ~temp1;
			state.v[8].U64[1] = state.v[8].U64[1] & ~temp2;
			state.v[8].U64[0] = state.v[8].U64[0] | value1;
			state.v[8].U64[1] = state.v[8].U64[1] | value2;
		}
	}


_040017FC:	// 40017fc: <21efffff>	ADDI      T7 = T7 + ffff
	state.r[T7] = state.r[T7] + 0xFFFFFFFF;


_04001800:	// 4001800: <e9c12000>	SQV       [T6 + 0x00] = vec01 <00>
	{
		_u32    addr = (0x00000000 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[1].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[1].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[1].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[1].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[1].U64[0] & temp1);
			value2 |= (state.v[1].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001804:	// 4001804: <e9c22001>	SQV       [T6 + 0x10] = vec02 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[2].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[2].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[2].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[2].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[2].U64[0] & temp1);
			value2 |= (state.v[2].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001808:	// 4001808: <e9c32002>	SQV       [T6 + 0x20] = vec03 <00>
	{
		_u32    addr = (0x00000020 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[3].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[3].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[3].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[3].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[3].U64[0] & temp1);
			value2 |= (state.v[3].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_0400180C:	// 400180c: <e9c42003>	SQV       [T6 + 0x30] = vec04 <00>
	{
		_u32    addr = (0x00000030 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[4].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[4].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[4].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[4].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[4].U64[0] & temp1);
			value2 |= (state.v[4].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001810:	// 4001810: <e9c52004>	SQV       [T6 + 0x40] = vec05 <00>
	{
		_u32    addr = (0x00000040 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[5].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[5].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[5].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[5].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[5].U64[0] & temp1);
			value2 |= (state.v[5].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001814:	// 4001814: <e9c62005>	SQV       [T6 + 0x50] = vec06 <00>
	{
		_u32    addr = (0x00000050 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[6].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[6].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[6].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[6].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[6].U64[0] & temp1);
			value2 |= (state.v[6].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001818:	// 4001818: <e9c72006>	SQV       [T6 + 0x60] = vec07 <00>
	{
		_u32    addr = (0x00000060 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[7].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[7].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[7].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[7].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[7].U64[0] & temp1);
			value2 |= (state.v[7].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_0400181C:	// 400181c: <e9c82007>	SQV       [T6 + 0x70] = vec08 <00>
	{
		_u32    addr = (0x00000070 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[8].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[8].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[8].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[8].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[8].U64[0] & temp1);
			value2 |= (state.v[8].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001820:	// 4001820: <1de0fff6>	BGTZ      (R0>0) --> 17fc
	sp_reg_pc = 0x04001820;
	if((_s32)state.r[T7] > 0)
	{
		state.r[T6] = state.r[T6] + 0x0080;
		goto _040017FC;
	}


_04001824:	// 4001824: <21ce0080>	ADDI      T6 = T6 + 0080
	state.r[T6] = state.r[T6] + 0x0080;


_04001828:	// 4001828: <09000423>	J         0400108c
	goto _0400108C;


_0400182C:	// 400182c: <00000000>	NOP       


_04001830:	// 4001830: <334cffff>	ANDI      T4 = K0 & ffff
	state.r[T4] = (state.r[K0] & 0x0000FFFF);


_04001834:	// 4001834: <332effff>	ANDI      T6 = T9 & ffff
	state.r[T6] = (state.r[T9] & 0x0000FFFF);


_04001838:	// 4001838: <00196c02>	SRL       T5 = T9 >> 16
	state.r[T5] = ((_u32)state.r[T9] >> 16);


_0400183C:	// 400183c: <c9a10800>	LSV       vec01 <00> = [T5 + 0x00]
	state.v[1].U16[7] = Load16_DMEM(0x00000000 + state.r[T5]);


_04001840:	// 4001840: <c9a20804>	LSV       vec02 <00> = [T5 + 0x08]
	state.v[2].U16[7] = Load16_DMEM(0x00000008 + state.r[T5]);


_04001844:	// 4001844: <c9a30808>	LSV       vec03 <00> = [T5 + 0x10]
	state.v[3].U16[7] = Load16_DMEM(0x00000010 + state.r[T5]);


_04001848:	// 4001848: <c9a4080c>	LSV       vec04 <00> = [T5 + 0x18]
	state.v[4].U16[7] = Load16_DMEM(0x00000018 + state.r[T5]);


_0400184C:	// 400184c: <c9a10902>	LSV       vec01 <02> = [T5 + 0x04]
	state.v[1].U16[6] = Load16_DMEM(0x00000004 + state.r[T5]);


_04001850:	// 4001850: <c9a20906>	LSV       vec02 <02> = [T5 + 0x0c]
	state.v[2].U16[6] = Load16_DMEM(0x0000000C + state.r[T5]);


_04001854:	// 4001854: <c9a3090a>	LSV       vec03 <02> = [T5 + 0x14]
	state.v[3].U16[6] = Load16_DMEM(0x00000014 + state.r[T5]);


_04001858:	// 4001858: <c9a4090e>	LSV       vec04 <02> = [T5 + 0x1c]
	state.v[4].U16[6] = Load16_DMEM(0x0000001C + state.r[T5]);


_0400185C:	// 400185c: <21ad0020>	ADDI      T5 = T5 + 0020
	state.r[T5] = state.r[T5] + 0x0020;


_04001860:	// 4001860: <218cfff8>	ADDI      T4 = T4 + fff8
	state.r[T4] = state.r[T4] + 0xFFFFFFF8;


_04001864:	// 4001864: <e9c11000>	SLV       [T6 + 0x00] = vec01 <00>
	Save32_DMEM (state.v[1].U32[3], 0x00000000 + state.r[T6]);


_04001868:	// 4001868: <e9c21001>	SLV       [T6 + 0x08] = vec02 <00>
	Save32_DMEM (state.v[2].U32[3], 0x00000004 + state.r[T6]);


_0400186C:	// 400186c: <e9c31002>	SLV       [T6 + 0x10] = vec03 <00>
	Save32_DMEM (state.v[3].U32[3], 0x00000008 + state.r[T6]);


_04001870:	// 4001870: <e9c41003>	SLV       [T6 + 0x18] = vec04 <00>
	Save32_DMEM (state.v[4].U32[3], 0x0000000C + state.r[T6]);


_04001874:	// 4001874: <1d80fff1>	BGTZ      (R0>0) --> 183c
	sp_reg_pc = 0x04001874;
	if((_s32)state.r[T4] > 0)
	{
		state.r[T6] = state.r[T6] + 0x0010;
		goto _0400183C;
	}


_04001878:	// 4001878: <21ce0010>	ADDI      T6 = T6 + 0010
	state.r[T6] = state.r[T6] + 0x0010;


_0400187C:	// 400187c: <09000423>	J         0400108c
	goto _0400108C;


_04001880:	// 4001880: <00000000>	NOP       


_04001884:	// 4001884: <4a04212c>	VXOR      vec04 = vec04 xor vec04[<none>]
	rsp_cop2_vxor(&state.v[4], &state.v[4], &state.v[4]);


_04001888:	// 4001888: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_0400188C:	// 400188c: <c8032000>	LQV       vec03 <00> = [R0 + 0x00]
	{
		_u32 addr = (0x00000000 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[3].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[3].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[3].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[3].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[3].U64[0] = state.v[3].U64[0] & ~temp1;
			state.v[3].U64[1] = state.v[3].U64[1] & ~temp2;
			state.v[3].U64[0] = state.v[3].U64[0] | value1;
			state.v[3].U64[1] = state.v[3].U64[1] | value2;
		}
	}


_04001890:	// 4001890: <02b5a820>	ADD       S5 = S5+S5
	state.r[S5] = (state.r[S5] + state.r[S5]);


_04001894:	// 4001894: <48952000>	MTC2      vec4 <00> = S5
	state.v[4].U16[7] = (_u16)state.r[S5];


_04001898:	// 4001898: <48952100>	MTC2      vec4 <02> = S5
	state.v[4].U16[6] = (_u16)state.r[S5];


_0400189C:	// 400189c: <001a6302>	SRL       T4 = K0 >> 12
	state.r[T4] = ((_u32)state.r[K0] >> 12);


_040018A0:	// 40018a0: <31930ff0>	ANDI      S3 = T4 & 0ff0
	state.r[S3] = (state.r[T4] & 0x00000FF0);


_040018A4:	// 40018a4: <02d6b020>	ADD       S6 = S6+S6
	state.r[S6] = (state.r[S6] + state.r[S6]);


_040018A8:	// 40018a8: <48962200>	MTC2      vec4 <04> = S6
	state.v[4].U16[5] = (_u16)state.r[S6];


_040018AC:	// 40018ac: <48962300>	MTC2      vec4 <06> = S6
	state.v[4].U16[4] = (_u16)state.r[S6];


_040018B0:	// 40018b0: <00196502>	SRL       T4 = T9 >> 20
	state.r[T4] = ((_u32)state.r[T9] >> 20);


_040018B4:	// 40018b4: <318e0ff0>	ANDI      T6 = T4 & 0ff0
	state.r[T6] = (state.r[T4] & 0x00000FF0);


_040018B8:	// 40018b8: <016b5820>	ADD       T3 = T3+T3
	state.r[T3] = (state.r[T3] + state.r[T3]);


_040018BC:	// 40018bc: <488b2400>	MTC2      vec4 <08> = T3
	state.v[4].U16[3] = (_u16)state.r[T3];


_040018C0:	// 40018c0: <488b2500>	MTC2      vec4 <10> = T3
	state.v[4].U16[2] = (_u16)state.r[T3];


_040018C4:	// 40018c4: <00196302>	SRL       T4 = T9 >> 12
	state.r[T4] = ((_u32)state.r[T9] >> 12);


_040018C8:	// 40018c8: <318f0ff0>	ANDI      T7 = T4 & 0ff0
	state.r[T7] = (state.r[T4] & 0x00000FF0);


_040018CC:	// 40018cc: <00196102>	SRL       T4 = T9 >> 4
	state.r[T4] = ((_u32)state.r[T9] >> 4);


_040018D0:	// 40018d0: <31900ff0>	ANDI      S0 = T4 & 0ff0
	state.r[S0] = (state.r[T4] & 0x00000FF0);


_040018D4:	// 40018d4: <00196100>	SLL       T4 = T9 << 4
	state.r[T4] = ((_u32)state.r[T9] << 4);


_040018D8:	// 40018d8: <31910ff0>	ANDI      S1 = T4 & 0ff0
	state.r[S1] = (state.r[T4] & 0x00000FF0);


_040018DC:	// 40018dc: <334c0002>	ANDI      T4 = K0 & 0002
	state.r[T4] = (state.r[K0] & 0x00000002);


_040018E0:	// 40018e0: <000c6042>	SRL       T4 = T4 >> 1
	state.r[T4] = ((_u32)state.r[T4] >> 1);


_040018E4:	// 40018e4: <000c6022>	SUB       T4 = R0-T4
	state.r[T4] = (0 - state.r[T4]);


_040018E8:	// 40018e8: <488c1000>	MTC2      vec2 <00> = T4
	state.v[2].U16[7] = (_u16)state.r[T4];


_040018EC:	// 40018ec: <334c0001>	ANDI      T4 = K0 & 0001
	state.r[T4] = (state.r[K0] & 0x00000001);


_040018F0:	// 40018f0: <000c6022>	SUB       T4 = R0-T4
	state.r[T4] = (0 - state.r[T4]);


_040018F4:	// 40018f4: <488c1100>	MTC2      vec2 <02> = T4
	state.v[2].U16[6] = (_u16)state.r[T4];


_040018F8:	// 40018f8: <334c0008>	ANDI      T4 = K0 & 0008
	state.r[T4] = (state.r[K0] & 0x00000008);


_040018FC:	// 40018fc: <000c6042>	SRL       T4 = T4 >> 1
	state.r[T4] = ((_u32)state.r[T4] >> 1);


_04001900:	// 4001900: <000c6022>	SUB       T4 = R0-T4
	state.r[T4] = (0 - state.r[T4]);


_04001904:	// 4001904: <488c1200>	MTC2      vec2 <04> = T4
	state.v[2].U16[5] = (_u16)state.r[T4];


_04001908:	// 4001908: <334c0004>	ANDI      T4 = K0 & 0004
	state.r[T4] = (state.r[K0] & 0x00000004);


_0400190C:	// 400190c: <000c6042>	SRL       T4 = T4 >> 1
	state.r[T4] = ((_u32)state.r[T4] >> 1);


_04001910:	// 4001910: <000c6022>	SUB       T4 = R0-T4
	state.r[T4] = (0 - state.r[T4]);


_04001914:	// 4001914: <488c1300>	MTC2      vec2 <06> = T4
	state.v[2].U16[4] = (_u16)state.r[T4];


_04001918:	// 4001918: <001a6202>	SRL       T4 = K0 >> 8
	state.r[T4] = ((_u32)state.r[K0] >> 8);


_0400191C:	// 400191c: <319400ff>	ANDI      S4 = T4 & 00ff
	state.r[S4] = (state.r[T4] & 0x000000FF);


_04001920:	// 4001920: <4a000010>	VADD      vec00 = vec00 + vec00[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[0].U16[0], state.v[0].U16[0], state.v[0].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[0].U16[1], state.v[0].U16[1], state.v[0].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[0].U16[2], state.v[0].U16[2], state.v[0].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[0].U16[3], state.v[0].U16[3], state.v[0].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[0].U16[4], state.v[0].U16[4], state.v[0].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[0].U16[5], state.v[0].U16[5], state.v[0].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[0].U16[6], state.v[0].U16[6], state.v[0].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[0].U16[7], state.v[0].U16[7], state.v[0].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001924:	// 4001924: <334a0010>	ANDI      T2 = K0 & 0010
	state.r[T2] = (state.r[K0] & 0x00000010);


_04001928:	// 4001928: <ca682000>	LQV       vec08 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[8].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[8].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[8].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[8].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[8].U64[0] = state.v[8].U64[0] & ~temp1;
			state.v[8].U64[1] = state.v[8].U64[1] & ~temp2;
			state.v[8].U64[0] = state.v[8].U64[0] | value1;
			state.v[8].U64[1] = state.v[8].U64[1] | value2;
		}
	}


_0400192C:	// 400192c: <ca6f2001>	LQV       vec15 <00> = [S3 + 0x10]

	// [start function: Zelda192C_19A8]

	Zelda192C_19A8();
	goto _040019AC;

_04001930:	// 4001930: <22730020>	ADDI      S3 = S3 + 0020

_04001934:	// 4001934: <4b014245>	VMUDM     vec09 = ( acc = vec08 * vec01[0] ) >> 16

_04001938:	// 4001938: <4b414285>	VMUDM     vec10 = ( acc = vec08 * vec01[2] ) >> 16

_0400193C:	// 400193c: <2294fff0>	ADDI      S4 = S4 + fff0

_04001940:	// 4001940: <c9cb2000>	LQV       vec11 <00> = [T6 + 0x00]

_04001944:	// 4001944: <c9ec2000>	LQV       vec12 <00> = [T7 + 0x00]

_04001948:	// 4001948: <4b217c05>	VMUDM     vec16 = ( acc = vec15 * vec01[1] ) >> 16

_0400194C:	// 400194c: <4b617c45>	VMUDM     vec17 = ( acc = vec15 * vec01[3] ) >> 16

_04001950:	// 4001950: <c9d22001>	LQV       vec18 <00> = [T6 + 0x10]

_04001954:	// 4001954: <c9f32001>	LQV       vec19 <00> = [T7 + 0x10]

_04001958:	// 4001958: <4b024a6c>	VXOR      vec09 = vec09 xor vec02[0]

_0400195C:	// 400195c: <4b2252ac>	VXOR      vec10 = vec10 xor vec02[1]

_04001960:	// 4001960: <ca0d2000>	LQV       vec13 <00> = [S0 + 0x00]

_04001964:	// 4001964: <ca2e2000>	LQV       vec14 <00> = [S1 + 0x00]

_04001968:	// 4001968: <4a095ad0>	VADD      vec11 = vec11 + vec09[<none>]

_0400196C:	// 400196c: <4a0a6310>	VADD      vec12 = vec12 + vec10[<none>]

_04001970:	// 4001970: <4b814a45>	VMUDM     vec09 = ( acc = vec09 * vec01[4] ) >> 16

_04001974:	// 4001974: <4b815285>	VMUDM     vec10 = ( acc = vec10 * vec01[4] ) >> 16

_04001978:	// 4001978: <4b02842c>	VXOR      vec16 = vec16 xor vec02[0]

_0400197C:	// 400197c: <4b228c6c>	VXOR      vec17 = vec17 xor vec02[1]

_04001980:	// 4001980: <ca142001>	LQV       vec20 <00> = [S0 + 0x10]

_04001984:	// 4001984: <ca352001>	LQV       vec21 <00> = [S1 + 0x10]

_04001988:	// 4001988: <4a109490>	VADD      vec18 = vec18 + vec16[<none>]

_0400198C:	// 400198c: <4a119cd0>	VADD      vec19 = vec19 + vec17[<none>]

_04001990:	// 4001990: <4ba18405>	VMUDM     vec16 = ( acc = vec16 * vec01[5] ) >> 16

_04001994:	// 4001994: <4ba18c45>	VMUDM     vec17 = ( acc = vec17 * vec01[5] ) >> 16

_04001998:	// 4001998: <4b424a6c>	VXOR      vec09 = vec09 xor vec02[2]

_0400199C:	// 400199c: <4b6252ac>	VXOR      vec10 = vec10 xor vec02[3]

_040019A0:	// 40019a0: <e9cb2000>	SQV       [T6 + 0x00] = vec11 <00>

_040019A4:	// 40019a4: <4b42842c>	VXOR      vec16 = vec16 xor vec02[2]

_040019A8:	// 40019a8: <4b628c6c>	VXOR      vec17 = vec17 xor vec02[3]
	ErrorToCallFunction(0x040019A8);

	// [end of function: Zelda192C_19A8]

_040019AC:	// 40019ac: <15400015>	BNE       (R0!=T2) --> 1a04
	sp_reg_pc = 0x040019AC;
	if (state.r[T2] != 0)
	{
		{
			_u32    addr = (0x00000000 + state.r[T7]);
			int     shifter = ((addr & 0xf) << 3);

			if (shifter == 0 && (addr&3) == 0 )
			{
				addr &= 0xfff;
				*(_u32 *)&pDMEM[addr+12] = state.v[12].U32[0];
				*(_u32 *)&pDMEM[addr+8 ] = state.v[12].U32[1];
				*(_u32 *)&pDMEM[addr+4 ] = state.v[12].U32[2];
				*(_u32 *)&pDMEM[addr   ] = state.v[12].U32[3];
			}
			else
			{
				_u64    value1 = Load64_DMEM(addr+8);
				_u64    value2 = Load64_DMEM(addr);
				_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
				_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

				if (shifter > 0x40)
				{
					temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
					temp1 = 0;
				} else {
					temp1 = ((temp1 >> shifter) << shifter);
				}
				value1 = value1 & ~temp1;
				value2 = value2 & ~temp2;

				value1 |= (state.v[12].U64[0] & temp1);
				value2 |= (state.v[12].U64[1] & temp2);

				Save64_DMEM(value1, addr+8);
				Save64_DMEM(value2, addr  );
			}
		}
		goto _04001A04;
	}


_040019B0:	// 40019b0: <e9ec2000>	SQV       [T7 + 0x00] = vec12 <00>
	{
		_u32    addr = (0x00000000 + state.r[T7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[12].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[12].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[12].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[12].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[12].U64[0] & temp1);
			value2 |= (state.v[12].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019B4:	// 40019b4: <4a096b50>	VADD      vec13 = vec13 + vec09[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[13].U16[0], state.v[13].U16[0], state.v[9].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[13].U16[1], state.v[13].U16[1], state.v[9].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[13].U16[2], state.v[13].U16[2], state.v[9].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[13].U16[3], state.v[13].U16[3], state.v[9].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[13].U16[4], state.v[13].U16[4], state.v[9].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[13].U16[5], state.v[13].U16[5], state.v[9].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[13].U16[6], state.v[13].U16[6], state.v[9].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[13].U16[7], state.v[13].U16[7], state.v[9].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040019B8:	// 40019b8: <4a0a7390>	VADD      vec14 = vec14 + vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[14].U16[0], state.v[14].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[14].U16[1], state.v[14].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[14].U16[2], state.v[14].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[14].U16[3], state.v[14].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[14].U16[4], state.v[14].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[14].U16[5], state.v[14].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[14].U16[6], state.v[14].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[14].U16[7], state.v[14].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040019BC:	// 40019bc: <e9d22001>	SQV       [T6 + 0x10] = vec18 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[18].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[18].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[18].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[18].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[18].U64[0] & temp1);
			value2 |= (state.v[18].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019C0:	// 40019c0: <e9f32001>	SQV       [T7 + 0x10] = vec19 <00>
	{
		_u32    addr = (0x00000010 + state.r[T7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[19].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[19].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[19].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[19].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[19].U64[0] & temp1);
			value2 |= (state.v[19].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019C4:	// 40019c4: <4a10a510>	VADD      vec20 = vec20 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[20].U16[0], state.v[20].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[20].U16[1], state.v[20].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[20].U16[2], state.v[20].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[20].U16[3], state.v[20].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[20].U16[4], state.v[20].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[20].U16[5], state.v[20].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[20].U16[6], state.v[20].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[20].U16[7], state.v[20].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040019C8:	// 40019c8: <4a11ad50>	VADD      vec21 = vec21 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[21].U16[0], state.v[21].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[21].U16[1], state.v[21].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[21].U16[2], state.v[21].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[21].U16[3], state.v[21].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[21].U16[4], state.v[21].U16[4], state.v[17].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[21].U16[5], state.v[21].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[21].U16[6], state.v[21].U16[6], state.v[17].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[21].U16[7], state.v[21].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040019CC:	// 40019cc: <21ce0020>	ADDI      T6 = T6 + 0020
	state.r[T6] = state.r[T6] + 0x0020;


_040019D0:	// 40019d0: <ea0d2000>	SQV       [S0 + 0x00] = vec13 <00>
	{
		_u32    addr = (0x00000000 + state.r[S0]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[13].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[13].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[13].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[13].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[13].U64[0] & temp1);
			value2 |= (state.v[13].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019D4:	// 40019d4: <ea2e2000>	SQV       [S1 + 0x00] = vec14 <00>
	{
		_u32    addr = (0x00000000 + state.r[S1]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[14].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[14].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[14].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[14].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[14].U64[0] & temp1);
			value2 |= (state.v[14].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019D8:	// 40019d8: <21ef0020>	ADDI      T7 = T7 + 0020
	state.r[T7] = state.r[T7] + 0x0020;


_040019DC:	// 40019dc: <ca682000>	LQV       vec08 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[8].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[8].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[8].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[8].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[8].U64[0] = state.v[8].U64[0] & ~temp1;
			state.v[8].U64[1] = state.v[8].U64[1] & ~temp2;
			state.v[8].U64[0] = state.v[8].U64[0] | value1;
			state.v[8].U64[1] = state.v[8].U64[1] | value2;
		}
	}


_040019E0:	// 40019e0: <ea142001>	SQV       [S0 + 0x10] = vec20 <00>
	{
		_u32    addr = (0x00000010 + state.r[S0]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[20].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[20].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[20].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[20].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[20].U64[0] & temp1);
			value2 |= (state.v[20].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019E4:	// 40019e4: <22100020>	ADDI      S0 = S0 + 0020
	state.r[S0] = state.r[S0] + 0x0020;


_040019E8:	// 40019e8: <4a040854>	VADDC	vec01 = vec01 + vec04[<none>]
	{
		_u16		flag0 = 0;
		VADDC_operation(0, state.v[1].U16[0], state.v[1].U16[0], state.v[4].U16[0], 1, 0, 0, 1)
		VADDC_operation(1, state.v[1].U16[1], state.v[1].U16[1], state.v[4].U16[1], 1, 0, 0, 1)
		VADDC_operation(2, state.v[1].U16[2], state.v[1].U16[2], state.v[4].U16[2], 1, 0, 0, 1)
		VADDC_operation(3, state.v[1].U16[3], state.v[1].U16[3], state.v[4].U16[3], 1, 0, 0, 1)
		VADDC_operation(4, state.v[1].U16[4], state.v[1].U16[4], state.v[4].U16[4], 1, 0, 0, 1)
		VADDC_operation(5, state.v[1].U16[5], state.v[1].U16[5], state.v[4].U16[5], 1, 0, 0, 1)
		VADDC_operation(6, state.v[1].U16[6], state.v[1].U16[6], state.v[4].U16[6], 1, 0, 0, 1)
		VADDC_operation(7, state.v[1].U16[7], state.v[1].U16[7], state.v[4].U16[7], 1, 0, 0, 1)
		state.flag[0] = flag0;
	}


_040019EC:	// 40019ec: <ea352001>	SQV       [S1 + 0x10] = vec21 <00>
	{
		_u32    addr = (0x00000010 + state.r[S1]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[21].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[21].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[21].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[21].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[21].U64[0] & temp1);
			value2 |= (state.v[21].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019F0:	// 40019f0: <22310020>	ADDI      S1 = S1 + 0020
	state.r[S1] = state.r[S1] + 0x0020;


_040019F4:	// 40019f4: <1e80ffcd>	BGTZ      (R0>0) --> 192c
	sp_reg_pc = 0x040019F4;
	if((_s32)state.r[S4] > 0)
	{
		{
			_u16		flag0 = state.flag[0];
			VADD_operation(0, state.v[0].U16[0], state.v[0].U16[0], state.v[0].U16[0], 1, 1, 1, 1)
			VADD_operation(1, state.v[0].U16[1], state.v[0].U16[1], state.v[0].U16[1], 1, 1, 1, 1)
			VADD_operation(2, state.v[0].U16[2], state.v[0].U16[2], state.v[0].U16[2], 1, 1, 1, 1)
			VADD_operation(3, state.v[0].U16[3], state.v[0].U16[3], state.v[0].U16[3], 1, 1, 1, 1)
			VADD_operation(4, state.v[0].U16[4], state.v[0].U16[4], state.v[0].U16[4], 1, 1, 1, 1)
			VADD_operation(5, state.v[0].U16[5], state.v[0].U16[5], state.v[0].U16[5], 1, 1, 1, 1)
			VADD_operation(6, state.v[0].U16[6], state.v[0].U16[6], state.v[0].U16[6], 1, 1, 1, 1)
			VADD_operation(7, state.v[0].U16[7], state.v[0].U16[7], state.v[0].U16[7], 1, 1, 1, 1)
			state.flag[0] = 0x0000;
		}
		goto _0400192C;
	}


_040019F8:	// 40019f8: <4a000010>	VADD      vec00 = vec00 + vec00[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[0].U16[0], state.v[0].U16[0], state.v[0].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[0].U16[1], state.v[0].U16[1], state.v[0].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[0].U16[2], state.v[0].U16[2], state.v[0].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[0].U16[3], state.v[0].U16[3], state.v[0].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[0].U16[4], state.v[0].U16[4], state.v[0].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[0].U16[5], state.v[0].U16[5], state.v[0].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[0].U16[6], state.v[0].U16[6], state.v[0].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[0].U16[7], state.v[0].U16[7], state.v[0].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040019FC:	// 40019fc: <09000423>	J         0400108c
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);
	goto _0400108C;


_04001A00:	// 4001a00: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_04001A04:	// 4001a04: <4a0a6b50>	VADD      vec13 = vec13 + vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[13].U16[0], state.v[13].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[13].U16[1], state.v[13].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[13].U16[2], state.v[13].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[13].U16[3], state.v[13].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[13].U16[4], state.v[13].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[13].U16[5], state.v[13].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[13].U16[6], state.v[13].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[13].U16[7], state.v[13].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001A08:	// 4001a08: <4a097390>	VADD      vec14 = vec14 + vec09[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[14].U16[0], state.v[14].U16[0], state.v[9].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[14].U16[1], state.v[14].U16[1], state.v[9].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[14].U16[2], state.v[14].U16[2], state.v[9].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[14].U16[3], state.v[14].U16[3], state.v[9].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[14].U16[4], state.v[14].U16[4], state.v[9].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[14].U16[5], state.v[14].U16[5], state.v[9].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[14].U16[6], state.v[14].U16[6], state.v[9].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[14].U16[7], state.v[14].U16[7], state.v[9].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001A0C:	// 4001a0c: <e9d22001>	SQV       [T6 + 0x10] = vec18 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[18].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[18].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[18].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[18].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[18].U64[0] & temp1);
			value2 |= (state.v[18].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001A10:	// 4001a10: <e9f32001>	SQV       [T7 + 0x10] = vec19 <00>
	{
		_u32    addr = (0x00000010 + state.r[T7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[19].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[19].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[19].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[19].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[19].U64[0] & temp1);
			value2 |= (state.v[19].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001A14:	// 4001a14: <4a11a510>	VADD      vec20 = vec20 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[20].U16[0], state.v[20].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[20].U16[1], state.v[20].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[20].U16[2], state.v[20].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[20].U16[3], state.v[20].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[20].U16[4], state.v[20].U16[4], state.v[17].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[20].U16[5], state.v[20].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[20].U16[6], state.v[20].U16[6], state.v[17].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[20].U16[7], state.v[20].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001A18:	// 4001a18: <09000673>	J         040019cc
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[21].U16[0], state.v[21].U16[0], state.v[16].U16[0], 1, 1, 1, 1)
		VADD_operation(1, state.v[21].U16[1], state.v[21].U16[1], state.v[16].U16[1], 1, 1, 1, 1)
		VADD_operation(2, state.v[21].U16[2], state.v[21].U16[2], state.v[16].U16[2], 1, 1, 1, 1)
		VADD_operation(3, state.v[21].U16[3], state.v[21].U16[3], state.v[16].U16[3], 1, 1, 1, 1)
		VADD_operation(4, state.v[21].U16[4], state.v[21].U16[4], state.v[16].U16[4], 1, 1, 1, 1)
		VADD_operation(5, state.v[21].U16[5], state.v[21].U16[5], state.v[16].U16[5], 1, 1, 1, 1)
		VADD_operation(6, state.v[21].U16[6], state.v[21].U16[6], state.v[16].U16[6], 1, 1, 1, 1)
		VADD_operation(7, state.v[21].U16[7], state.v[21].U16[7], state.v[16].U16[7], 1, 1, 1, 1)
		state.flag[0] = 0x0000;
	}
	goto _040019CC;


_04001A1C:	// 4001a1c: <4a10ad50>	VADD      vec21 = vec21 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[21].U16[0], state.v[21].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[21].U16[1], state.v[21].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[21].U16[2], state.v[21].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[21].U16[3], state.v[21].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[21].U16[4], state.v[21].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[21].U16[5], state.v[21].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[21].U16[6], state.v[21].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[21].U16[7], state.v[21].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001A20:	// 4001a20: <4a01086c>	VXOR      vec01 = vec01 xor vec01[<none>]
	rsp_cop2_vxor(&state.v[1], &state.v[1], &state.v[1]);


_04001A24:	// 4001a24: <334bffff>	ANDI      T3 = K0 & ffff
	state.r[T3] = (state.r[K0] & 0x0000FFFF);


_04001A28:	// 4001a28: <001a6202>	SRL       T4 = K0 >> 8
	state.r[T4] = ((_u32)state.r[K0] >> 8);


_04001A2C:	// 4001a2c: <318cff00>	ANDI      T4 = T4 & ff00
	state.r[T4] = (state.r[T4] & 0x0000FF00);


_04001A30:	// 4001a30: <488c0c00>	MTC2      vec1 <08> = T4
	state.v[1].U16[3] = (_u16)state.r[T4];


_04001A34:	// 4001a34: <018b6020>	ADD       T4 = T4+T3
	state.r[T4] = (state.r[T4] + state.r[T3]);


_04001A38:	// 4001a38: <488c0d00>	MTC2      vec1 <10> = T4
	state.v[1].U16[2] = (_u16)state.r[T4];


_04001A3C:	// 4001a3c: <0019ac02>	SRL       S5 = T9 >> 16
	state.r[S5] = ((_u32)state.r[T9] >> 16);


_04001A40:	// 4001a40: <09000423>	J         0400108c
	state.r[S6] = (state.r[T9] & 0x0000FFFF);
	goto _0400108C;


_04001A44:	// 4001a44: <3336ffff>	ANDI      S6 = T9 & ffff
	state.r[S6] = (state.r[T9] & 0x0000FFFF);


_04001A48:	// 4001a48: <00196402>	SRL       T4 = T9 >> 16
	state.r[T4] = ((_u32)state.r[T9] >> 16);


_04001A4C:	// 4001a4c: <488c0800>	MTC2      vec1 <00> = T4
	state.v[1].U16[7] = (_u16)state.r[T4];


_04001A50:	// 4001a50: <01956020>	ADD       T4 = T4+S5
	state.r[T4] = (state.r[T4] + state.r[S5]);


_04001A54:	// 4001a54: <488c0900>	MTC2      vec1 <02> = T4
	state.v[1].U16[6] = (_u16)state.r[T4];


_04001A58:	// 4001a58: <332cffff>	ANDI      T4 = T9 & ffff
	state.r[T4] = (state.r[T9] & 0x0000FFFF);


_04001A5C:	// 4001a5c: <488c0a00>	MTC2      vec1 <04> = T4
	state.v[1].U16[5] = (_u16)state.r[T4];


_04001A60:	// 4001a60: <01966020>	ADD       T4 = T4+S6
	state.r[T4] = (state.r[T4] + state.r[S6]);


_04001A64:	// 4001a64: <09000423>	J         0400108c
	state.v[1].U16[4] = (_u16)state.r[T4];
	goto _0400108C;


_04001A68:	// 4001a68: <488c0b00>	MTC2      vec1 <06> = T4
	state.v[1].U16[4] = (_u16)state.r[T4];


_04001A6C:	// 4001a6c: <001a1b02>	SRL       V1 = K0 >> 12
	state.r[V1] = ((_u32)state.r[K0] >> 12);


_04001A70:	// 4001a70: <30630ff0>	ANDI      V1 = V1 & 0ff0
	state.r[V1] = (state.r[V1] & 0x00000FF0);


_04001A74:	// 4001a74: <3341ffff>	ANDI      AT = K0 & ffff
	state.r[AT] = (state.r[K0] & 0x0000FFFF);


_04001A78:	// 4001a78: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001A7C:	// 4001a7c: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[V0] = ((_u32)state.r[V0] >> 8);
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001A80:	// 4001a80: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001A84:	// 4001a84: <0d00069b>	JAL	    04001a6c
	sp_reg_pc = 0x04001A84;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001A6C;


_04001A88:	// 4001a88: <00000000>	NOP       


_04001A8C:	// 4001a8c: <090006ad>	J         04001ab4
	goto _04001AB4;


_04001A90:	// 4001a90: <00000000>	NOP       


_04001A94:	// 4001a94: <0d00069b>	JAL	    04001a6c
	sp_reg_pc = 0x04001A94;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001A6C;


_04001A98:	// 4001a98: <00000000>	NOP       


_04001A9C:	// 4001a9c: <090006b1>	J         04001ac4
	goto _04001AC4;


_04001AA0:	// 4001aa0: <00000000>	NOP       


_04001AA4:	// 4001aa4: <0d00069e>	JAL	    04001a78
	state.r[AT] = 0 + 0x0330;
	sp_reg_pc = 0x04001AA4;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001A78;


_04001AA8:	// 4001aa8: <20010330>	ADDI      AT = R0 + 0330
	state.r[AT] = 0 + 0x0330;


_04001AAC:	// 4001aac: <090006ad>	J         04001ab4
	state.r[V1] = (state.r[K0] & 0x0000FFFF);
	goto _04001AB4;


_04001AB0:	// 4001ab0: <3343ffff>	ANDI      V1 = K0 & ffff
	state.r[V1] = (state.r[K0] & 0x0000FFFF);


_04001AB4:	// 4001ab4: <0d0006b5>	JAL	    04001ad4
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x04001AB4;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_04001AB8:	// 4001ab8: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001ABC:	// 4001abc: <09000423>	J         0400108c
	goto _0400108C;


_04001AC0:	// 4001ac0: <00000000>	NOP       


_04001AC4:	// 4001ac4: <0d0006bf>	JAL	    04001afc
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x04001AC4;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AFC;


_04001AC8:	// 4001ac8: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001ACC:	// 4001acc: <09000423>	J         0400108c
	goto _0400108C;


_04001AD0:	// 4001ad0: <00000000>	NOP       


_04001AD4:	// 4001ad4: <40043800>	MFC0      A3 = SP status
	state.r[A0] = 0;


_04001AD8:	// 4001ad8: <1480fd86>	BNE       (R0!=A0) --> 10f4
	sp_reg_pc = 0x04001AD8;
	if (state.r[A0] != 0)
	{
		goto _040010F4;
	}


_04001ADC:	// 4001adc: <00000000>	NOP       


_04001AE0:	// 4001ae0: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001AE4:	// 4001ae4: <1480fffe>	BNE       (R0!=A0) --> 1ae0
	sp_reg_pc = 0x04001AE4;
	if (state.r[A0] != 0)
	{
		goto _04001AE0;
	}


_04001AE8:	// 4001ae8: <00000000>	NOP       


_04001AEC:	// 4001aec: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_04001AF0:	// 4001af0: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001AF4:	// 4001af4: <090006c8>	J         04001b20
	sp_reg_pc = 0x04001AF8;
	DMARead(state.r[V1]);
	goto _04001B20;


_04001AF8:	// 4001af8: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001AF8;
	DMARead(state.r[V1]);


_04001AFC:	// 4001afc: <40043800>	MFC0      A3 = SP status
	state.r[A0] = 0;


_04001B00:	// 4001b00: <1480fd87>	BNE       (R0!=A0) --> 1120
	sp_reg_pc = 0x04001B00;
	if (state.r[A0] != 0)
	{
		goto _04001120;
	}


_04001B04:	// 4001b04: <00000000>	NOP       


_04001B08:	// 4001b08: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001B0C:	// 4001b0c: <1480fffe>	BNE       (R0!=A0) --> 1b08
	sp_reg_pc = 0x04001B0C;
	if (state.r[A0] != 0)
	{
		goto _04001B08;
	}


_04001B10:	// 4001b10: <00000000>	NOP       


_04001B14:	// 4001b14: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_04001B18:	// 4001b18: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001B1C:	// 4001b1c: <40831800>	MTC0      SP write DMA length = V1
	DMAWrite(state.r[V1]);


_04001B20:	// 4001b20: <20040001>	ADDI      A0 = R0 + 0001
	state.r[A0] = 0 + 0x0001;


_04001B24:	// 4001b24: <1480ffff>	BNE       (R0!=A0) --> 1b24
	sp_reg_pc = 0x04001B24;
	if (state.r[A0] != 0)
	{
		state.r[A0] = 0;
		goto _04001B24;
	}


_04001B28:	// 4001b28: <40043000>	MFC0      A2 = SP status
	state.r[A0] = 0;


_04001B2C:	// 4001b2c: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001B30:	// 4001b30: <40803800>	MTC0      SP semaphore = R0


_04001B34:	// 4001b34: <c81f2000>	LQV       vec31 <00> = [R0 + 0x00]
	{
		_u32 addr = (0x00000000 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001B38:	// 4001b38: <001a9302>	SRL       S2 = K0 >> 12
	state.r[S2] = ((_u32)state.r[K0] >> 12);


_04001B3C:	// 4001b3c: <32520ff0>	ANDI      S2 = S2 & 0ff0
	state.r[S2] = (state.r[S2] & 0x00000FF0);


_04001B40:	// 4001b40: <3333ffff>	ANDI      S3 = T9 & ffff
	state.r[S3] = (state.r[T9] & 0x0000FFFF);


_04001B44:	// 4001b44: <0019a402>	SRL       S4 = T9 >> 16
	state.r[S4] = ((_u32)state.r[T9] >> 16);


_04001B48:	// 4001b48: <3351ffff>	ANDI      S1 = K0 & ffff
	state.r[S1] = (state.r[K0] & 0x0000FFFF);


_04001B4C:	// 4001b4c: <4891f000>	MTC2      vec30 <00> = S1
	state.v[30].U16[7] = (_u16)state.r[S1];


_04001B50:	// 4001b50: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_04001B54:	// 4001b54: <ca9d2000>	LQV       vec29 <00> = [S4 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[29].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[29].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[29].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[29].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[29].U64[0] = state.v[29].U64[0] & ~temp1;
			state.v[29].U64[1] = state.v[29].U64[1] & ~temp2;
			state.v[29].U64[0] = state.v[29].U64[0] | value1;
			state.v[29].U64[1] = state.v[29].U64[1] | value2;
		}
	}


_04001B58:	// 4001b58: <ca7a2001>	LQV       vec26 <00> = [S3 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[26].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[26].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[26].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[26].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[26].U64[0] = state.v[26].U64[0] & ~temp1;
			state.v[26].U64[1] = state.v[26].U64[1] & ~temp2;
			state.v[26].U64[0] = state.v[26].U64[0] | value1;
			state.v[26].U64[1] = state.v[26].U64[1] | value2;
		}
	}


_04001B5C:	// 4001b5c: <ca9c2001>	LQV       vec28 <00> = [S4 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[28].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[28].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[28].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[28].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[28].U64[0] = state.v[28].U64[0] & ~temp1;
			state.v[28].U64[1] = state.v[28].U64[1] & ~temp2;
			state.v[28].U64[0] = state.v[28].U64[0] | value1;
			state.v[28].U64[1] = state.v[28].U64[1] | value2;
		}
	}


_04001B60:	// 4001b60: <4bdfdec0>	VMULF     vec27 = ( acc = (vec27 * vec31[6]) << 1 ) >> 16

	// [start function: Zelda1B60_1B8C]

	Zelda1B60_1B8C();
	goto _04001B90;

_04001B64:	// 4001b64: <2252ffe0>	ADDI      S2 = S2 + ffe0

_04001B68:	// 4001b68: <4b1eeec8>	VMACF     vec27 = ( acc += (vec29 * vec30[0]) << 1 ) >> 16

_04001B6C:	// 4001b6c: <22940020>	ADDI      S4 = S4 + 0020

_04001B70:	// 4001b70: <4bdfd680>	VMULF     vec26 = ( acc = (vec26 * vec31[6]) << 1 ) >> 16

_04001B74:	// 4001b74: <4b1ee688>	VMACF     vec26 = ( acc += (vec28 * vec30[0]) << 1 ) >> 16

_04001B78:	// 4001b78: <ca9d2000>	LQV       vec29 <00> = [S4 + 0x00]

_04001B7C:	// 4001b7c: <ea7b2000>	SQV       [S3 + 0x00] = vec27 <00>

_04001B80:	// 4001b80: <ca7b2002>	LQV       vec27 <00> = [S3 + 0x20]

_04001B84:	// 4001b84: <ea7a2001>	SQV       [S3 + 0x10] = vec26 <00>

_04001B88:	// 4001b88: <ca9c2001>	LQV       vec28 <00> = [S4 + 0x10]

_04001B8C:	// 4001b8c: <22730020>	ADDI      S3 = S3 + 0020
	ErrorToCallFunction(0x04001B8C);

	// [end of function: Zelda1B60_1B8C]

_04001B90:	// 4001b90: <1e40fff3>	BGTZ      (R0>0) --> 1b60
	sp_reg_pc = 0x04001B90;
	if((_s32)state.r[S2] > 0)
	{
		{
			_u32 addr = (0x00000010 + state.r[S3]);
			int shifter = ((addr & 0xf) << 3);

			if (shifter == 0 && (addr&3) == 0 )
			{
				addr &= 0xfff;
				state.v[26].U32[0] = *(_u32 *)&pDMEM[addr+12];
				state.v[26].U32[1] = *(_u32 *)&pDMEM[addr+8];
				state.v[26].U32[2] = *(_u32 *)&pDMEM[addr+4];
				state.v[26].U32[3] = *(_u32 *)&pDMEM[addr];
			}
			else
			{
				_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
				_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
				_u64		value1 = Load64_DMEM(addr+8);
				_u64		value2 = Load64_DMEM(addr);
				if (shifter > 0x40)
				{
					temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
					temp1 = 0;
				} else {
					temp1 = ((temp1 >> shifter) << shifter);
				}
				value1 = value1 & temp1;
				value2 = value2 & temp2;

				state.v[26].U64[0] = state.v[26].U64[0] & ~temp1;
				state.v[26].U64[1] = state.v[26].U64[1] & ~temp2;
				state.v[26].U64[0] = state.v[26].U64[0] | value1;
				state.v[26].U64[1] = state.v[26].U64[1] | value2;
			}
		}
		goto _04001B60;
	}


_04001B94:	// 4001b94: <ca7a2001>	LQV       vec26 <00> = [S3 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[26].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[26].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[26].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[26].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[26].U64[0] = state.v[26].U64[0] & ~temp1;
			state.v[26].U64[1] = state.v[26].U64[1] & ~temp2;
			state.v[26].U64[0] = state.v[26].U64[0] | value1;
			state.v[26].U64[1] = state.v[26].U64[1] | value2;
		}
	}


_04001B98:	// 4001b98: <09000423>	J         0400108c
	goto _0400108C;


_04001B9C:	// 4001b9c: <00000000>	NOP       


_04001BA0:	// 4001ba0: <970d0000>	LHU       T5 = [T8+0000]
	state.r[T5] = (_s32)(_u16)Load16_DMEM((state.r[T8] + 0x00000000) & 0xfff);


_04001BA4:	// 4001ba4: <4a0210ac>	VXOR      vec02 = vec02 xor vec02[<none>]
	rsp_cop2_vxor(&state.v[2], &state.v[2], &state.v[2]);


_04001BA8:	// 4001ba8: <970e0002>	LHU       T6 = [T8+0002]
	state.r[T6] = (_s32)(_u16)Load16_DMEM((state.r[T8] + 0x00000002) & 0xfff);


_04001BAC:	// 4001bac: <4a0318ec>	VXOR      vec03 = vec03 xor vec03[<none>]
	rsp_cop2_vxor(&state.v[3], &state.v[3], &state.v[3]);


_04001BB0:	// 4001bb0: <970c0004>	LHU       T4 = [T8+0004]
	state.r[T4] = (_s32)(_u16)Load16_DMEM((state.r[T8] + 0x00000004) & 0xfff);


_04001BB4:	// 4001bb4: <00198a00>	SLL       S1 = T9 << 8
	state.r[S1] = ((_u32)state.r[T9] << 8);


_04001BB8:	// 4001bb8: <00118a02>	SRL       S1 = S1 >> 8
	state.r[S1] = ((_u32)state.r[S1] >> 8);


_04001BBC:	// 4001bbc: <e9c22000>	SQV       [T6 + 0x00] = vec02 <00>
	{
		_u32    addr = (0x00000000 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[2].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[2].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[2].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[2].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[2].U64[0] & temp1);
			value2 |= (state.v[2].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001BC0:	// 4001bc0: <e9c32001>	SQV       [T6 + 0x10] = vec03 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[3].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[3].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[3].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[3].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[3].U64[0] & temp1);
			value2 |= (state.v[3].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001BC4:	// 4001bc4: <001a0c02>	SRL       AT = K0 >> 16
	state.r[AT] = ((_u32)state.r[K0] >> 16);


_04001BC8:	// 4001bc8: <30210001>	ANDI      AT = AT & 0001
	state.r[AT] = (state.r[AT] & 0x00000001);


_04001BCC:	// 4001bcc: <1c200008>	BGTZ      (R0>0) --> 1bf0
	sp_reg_pc = 0x04001BCC;
	if((_s32)state.r[AT] > 0)
	{
		state.r[AT] = ((_u32)state.r[K0] >> 16);
		goto _04001BF0;
	}


_04001BD0:	// 4001bd0: <001a0c02>	SRL       AT = K0 >> 16
	state.r[AT] = ((_u32)state.r[K0] >> 16);


_04001BD4:	// 4001bd4: <30210002>	ANDI      AT = AT & 0002
	state.r[AT] = (state.r[AT] & 0x00000002);


_04001BD8:	// 4001bd8: <10010002>	BEQ       (AT==R0) --> 1be4
	sp_reg_pc = 0x04001BD8;
	if (0 == state.r[AT])
	{
		state.r[V0] = state.r[S1] + 0x0000;
		goto _04001BE4;
	}


_04001BDC:	// 4001bdc: <22220000>	ADDI      V0 = S1 + 0000
	state.r[V0] = state.r[S1] + 0x0000;


_04001BE0:	// 4001be0: <8f020008>	LW        V0 = [T8+0008]
	state.r[V0] = (_s32)Load32_DMEM((state.r[T8] + 0x00000008) & 0xfff);


_04001BE4:	// 4001be4: <21c10000>	ADDI      AT = T6 + 0000
	state.r[AT] = state.r[T6] + 0x0000;


_04001BE8:	// 4001be8: <0d0006b5>	JAL	    04001ad4
	state.r[V1] = 0 + 0x001F;
	sp_reg_pc = 0x04001BE8;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_04001BEC:	// 4001bec: <2003001f>	ADDI      V1 = R0 + 001f
	state.r[V1] = 0 + 0x001F;


_04001BF0:	// 4001bf0: <21ce0020>	ADDI      T6 = T6 + 0020
	state.r[T6] = state.r[T6] + 0x0020;


_04001BF4:	// 4001bf4: <1180000b>	BEQ       (R0==T4) --> 1c24
	sp_reg_pc = 0x04001BF4;
	if (state.r[T4] == 0)
	{
		goto _04001C24;
	}


_04001BF8:	// 4001bf8: <00000000>	NOP       


_04001BFC:	// 4001bfc: <c9a23000>	LPV
	rsp_lpv(0xC9A23000);


_04001C00:	// 4001c00: <c9a33001>	LPV
	rsp_lpv(0xC9A33001);


_04001C04:	// 4001c04: <21ad0010>	ADDI      T5 = T5 + 0010
	state.r[T5] = state.r[T5] + 0x0010;


_04001C08:	// 4001c08: <218cffe0>	ADDI      T4 = T4 + ffe0
	state.r[T4] = state.r[T4] + 0xFFFFFFE0;


_04001C0C:	// 4001c0c: <c9a43000>	LPV
	rsp_lpv(0xC9A43000);


_04001C10:	// 4001c10: <e9c22000>	SQV       [T6 + 0x00] = vec02 <00>
	{
		_u32    addr = (0x00000000 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[2].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[2].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[2].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[2].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[2].U64[0] & temp1);
			value2 |= (state.v[2].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001C14:	// 4001c14: <c9a53001>	LPV
	rsp_lpv(0xC9A53001);


_04001C18:	// 4001c18: <e9c32001>	SQV       [T6 + 0x10] = vec03 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[3].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[3].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[3].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[3].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[3].U64[0] & temp1);
			value2 |= (state.v[3].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001C1C:	// 4001c1c: <1d800005>	BGTZ      (R0>0) --> 1c34
	sp_reg_pc = 0x04001C1C;
	if((_s32)state.r[T4] > 0)
	{
		state.r[T6] = state.r[T6] + 0x0020;
		goto _04001C34;
	}


_04001C20:	// 4001c20: <21ce0020>	ADDI      T6 = T6 + 0020
	state.r[T6] = state.r[T6] + 0x0020;


_04001C24:	// 4001c24: <21c1ffe0>	ADDI      AT = T6 + ffe0
	state.r[AT] = state.r[T6] + 0xFFFFFFE0;


_04001C28:	// 4001c28: <22220000>	ADDI      V0 = S1 + 0000
	state.r[V0] = state.r[S1] + 0x0000;


_04001C2C:	// 4001c2c: <090006b1>	J         04001ac4
	state.r[V1] = 0 + 0x0020;
	goto _04001AC4;


_04001C30:	// 4001c30: <20030020>	ADDI      V1 = R0 + 0020
	state.r[V1] = 0 + 0x0020;


_04001C34:	// 4001c34: <218cffe0>	ADDI      T4 = T4 + ffe0
	state.r[T4] = state.r[T4] + 0xFFFFFFE0;


_04001C38:	// 4001c38: <c9a23002>	LPV
	rsp_lpv(0xC9A23002);


_04001C3C:	// 4001c3c: <e9c42000>	SQV       [T6 + 0x00] = vec04 <00>
	{
		_u32    addr = (0x00000000 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[4].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[4].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[4].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[4].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[4].U64[0] & temp1);
			value2 |= (state.v[4].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001C40:	// 4001c40: <c9a33003>	LPV
	rsp_lpv(0xC9A33003);


_04001C44:	// 4001c44: <e9c52001>	SQV       [T6 + 0x10] = vec05 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[5].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[5].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[5].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[5].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[5].U64[0] & temp1);
			value2 |= (state.v[5].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001C48:	// 4001c48: <21ad0020>	ADDI      T5 = T5 + 0020
	state.r[T5] = state.r[T5] + 0x0020;


_04001C4C:	// 4001c4c: <1d80ffee>	BGTZ      (R0>0) --> 1c08
	sp_reg_pc = 0x04001C4C;
	if((_s32)state.r[T4] > 0)
	{
		state.r[T6] = state.r[T6] + 0x0020;
		goto _04001C08;
	}


_04001C50:	// 4001c50: <21ce0020>	ADDI      T6 = T6 + 0020
	state.r[T6] = state.r[T6] + 0x0020;


_04001C54:	// 4001c54: <09000709>	J         04001c24
	goto _04001C24;


_04001C58:	// 4001c58: <00000000>	NOP       


_04001C5C:	// 4001c5c: <334cffff>	ANDI      T4 = K0 & ffff
	state.r[T4] = (state.r[K0] & 0x0000FFFF);


_04001C60:	// 4001c60: <00196c02>	SRL       T5 = T9 >> 16
	state.r[T5] = ((_u32)state.r[T9] >> 16);


_04001C64:	// 4001c64: <001a7902>	SRL       T7 = K0 >> 4
	state.r[T7] = ((_u32)state.r[K0] >> 4);


_04001C68:	// 4001c68: <31eff000>	ANDI      T7 = T7 & f000
	state.r[T7] = (state.r[T7] & 0x0000F000);


_04001C6C:	// 4001c6c: <488f1900>	MTC2      vec3 <02> = T7
	state.v[3].U16[6] = (_u16)state.r[T7];


_04001C70:	// 4001c70: <001a7d02>	SRL       T7 = K0 >> 20
	state.r[T7] = ((_u32)state.r[K0] >> 20);


_04001C74:	// 4001c74: <31ef000f>	ANDI      T7 = T7 & 000f
	state.r[T7] = (state.r[T7] & 0x0000000F);


_04001C78:	// 4001c78: <488f1800>	MTC2      vec3 <00> = T7
	state.v[3].U16[7] = (_u16)state.r[T7];


_04001C7C:	// 4001c7c: <c9a12000>	LQV       vec01 <00> = [T5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[1].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[1].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[1].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[1].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[1].U64[0] = state.v[1].U64[0] & ~temp1;
			state.v[1].U64[1] = state.v[1].U64[1] & ~temp2;
			state.v[1].U64[0] = state.v[1].U64[0] | value1;
			state.v[1].U64[1] = state.v[1].U64[1] | value2;
		}
	}


_04001C80:	// 4001c80: <c9a22001>	LQV       vec02 <00> = [T5 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[2].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[2].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[2].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[2].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[2].U64[0] = state.v[2].U64[0] & ~temp1;
			state.v[2].U64[1] = state.v[2].U64[1] & ~temp2;
			state.v[2].U64[0] = state.v[2].U64[0] | value1;
			state.v[2].U64[1] = state.v[2].U64[1] | value2;
		}
	}


_04001C84:	// 4001c84: <4b230905>	VMUDM     vec04 = ( acc = vec01 * vec03[1] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[1*16].U16[0], 0xE4
			MOVQ mm1, state.v[1*16].U16[4*2]
			PSHUFW mm4, state.v[3*16].U16[6*2], 0
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_04001C88:	// 4001c88: <4b03090f>	VMADH     vec04 = ( acc+= (vec01 * vec03[0]) << 16) >> 16
	{
		_u16		s2value = state.v[3].U16[7];
		VMADH_operation(0, state.v[4].U16[0], state.v[1].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[4].U16[1], state.v[1].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[4].U16[2], state.v[1].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[4].U16[3], state.v[1].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[4].U16[4], state.v[1].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[4].U16[5], state.v[1].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[4].U16[6], state.v[1].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[4].U16[7], state.v[1].U16[7], s2value, 1, 0, 0, 1)
	}


_04001C8C:	// 4001c8c: <4b231145>	VMUDM     vec05 = ( acc = vec02 * vec03[1] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[2*16].U16[0], 0xE4
			MOVQ mm1, state.v[2*16].U16[4*2]
			PSHUFW mm4, state.v[3*16].U16[6*2], 0
			PSHUFW mm6, mm0, 0xE4
			MOVQ mm7, mm1
			PMULLW mm6, mm4
			PMULLW mm7, mm4
			MOVQ accumulator[0*4], mm6
			MOVQ accumulator[2*4], mm7
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_04001C90:	// 4001c90: <4b03114f>	VMADH     vec05 = ( acc+= (vec02 * vec03[0]) << 16) >> 16
	{
		_u16		s2value = state.v[3].U16[7];
		VMADH_operation(0, state.v[5].U16[0], state.v[2].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[5].U16[1], state.v[2].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[5].U16[2], state.v[2].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[5].U16[3], state.v[2].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[5].U16[4], state.v[2].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[5].U16[5], state.v[2].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[5].U16[6], state.v[2].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[5].U16[7], state.v[2].U16[7], s2value, 1, 1, 0, 1)
	}


_04001C94:	// 4001c94: <e9a42000>	SQV       [T5 + 0x00] = vec04 <00>
	{
		_u32    addr = (0x00000000 + state.r[T5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[4].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[4].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[4].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[4].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[4].U64[0] & temp1);
			value2 |= (state.v[4].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001C98:	// 4001c98: <e9a52001>	SQV       [T5 + 0x10] = vec05 <00>
	{
		_u32    addr = (0x00000010 + state.r[T5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[5].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[5].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[5].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[5].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[5].U64[0] & temp1);
			value2 |= (state.v[5].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001C9C:	// 4001c9c: <218cffe0>	ADDI      T4 = T4 + ffe0
	state.r[T4] = state.r[T4] + 0xFFFFFFE0;


_04001CA0:	// 4001ca0: <1d80fff6>	BGTZ      (R0>0) --> 1c7c
	sp_reg_pc = 0x04001CA0;
	if((_s32)state.r[T4] > 0)
	{
		state.r[T5] = state.r[T5] + 0x0020;
		goto _04001C7C;
	}


_04001CA4:	// 4001ca4: <21ad0020>	ADDI      T5 = T5 + 0020
	state.r[T5] = state.r[T5] + 0x0020;


_04001CA8:	// 4001ca8: <09000423>	J         0400108c
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);
	goto _0400108C;


_04001CAC:	// 4001cac: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_04001CB0:	// 4001cb0: <22ed0000>	ADDI      T5 = S7 + 0000
	state.r[T5] = state.r[S7] + 0x0000;


_04001CB4:	// 4001cb4: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_04001CB8:	// 4001cb8: <22ee0020>	ADDI      T6 = S7 + 0020
	state.r[T6] = state.r[S7] + 0x0020;


_04001CBC:	// 4001cbc: <e9a02000>	SQV       [T5 + 0x00] = vec00 <00>
	{
		_u32    addr = (0x00000000 + state.r[T5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[0].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[0].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[0].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[0].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[0].U64[0] & temp1);
			value2 |= (state.v[0].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001CC0:	// 4001cc0: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001CC4:	// 4001cc4: <e9a02001>	SQV       [T5 + 0x10] = vec00 <00>
	{
		_u32    addr = (0x00000010 + state.r[T5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[0].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[0].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[0].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[0].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[0].U64[0] & temp1);
			value2 |= (state.v[0].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001CC8:	// 4001cc8: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001CCC:	// 4001ccc: <001a6402>	SRL       T4 = K0 >> 16
	state.r[T4] = ((_u32)state.r[K0] >> 16);


_04001CD0:	// 4001cd0: <318c00ff>	ANDI      T4 = T4 & 00ff
	state.r[T4] = (state.r[T4] & 0x000000FF);


_04001CD4:	// 4001cd4: <1180000b>	BEQ       (R0==T4) --> 1d04
	sp_reg_pc = 0x04001CD4;
	if (state.r[T4] == 0)
	{
		goto _04001D04;
	}


_04001CD8:	// 4001cd8: <00000000>	NOP       


_04001CDC:	// 4001cdc: <218cffff>	ADDI      T4 = T4 + ffff
	state.r[T4] = state.r[T4] + 0xFFFFFFFF;


_04001CE0:	// 4001ce0: <1180000a>	BEQ       (R0==T4) --> 1d0c
	sp_reg_pc = 0x04001CE0;
	if (state.r[T4] == 0)
	{
		goto _04001D0C;
	}


_04001CE4:	// 4001ce4: <00000000>	NOP       


_04001CE8:	// 4001ce8: <334fffff>	ANDI      T7 = K0 & ffff
	state.r[T7] = (state.r[K0] & 0x0000FFFF);


_04001CEC:	// 4001cec: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_04001CF0:	// 4001cf0: <e9c02000>	SQV       [T6 + 0x00] = vec00 <00>
	{
		_u32    addr = (0x00000000 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[0].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[0].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[0].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[0].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[0].U64[0] & temp1);
			value2 |= (state.v[0].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001CF4:	// 4001cf4: <e9c02002>	SQV       [T6 + 0x20] = vec00 <00>
	{
		_u32    addr = (0x00000020 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[0].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[0].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[0].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[0].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[0].U64[0] & temp1);
			value2 |= (state.v[0].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001CF8:	// 4001cf8: <21c10010>	ADDI      AT = T6 + 0010
	state.r[AT] = state.r[T6] + 0x0010;


_04001CFC:	// 4001cfc: <090006ad>	J         04001ab4
	state.r[V1] = 0 + 0x0010;
	goto _04001AB4;


_04001D00:	// 4001d00: <20030010>	ADDI      V1 = R0 + 0010
	state.r[V1] = 0 + 0x0010;


_04001D04:	// 4001d04: <01a00820>	ADD       AT = T5+R0
	state.r[AT] = (state.r[T5] + 0);


_04001D08:	// 4001d08: <0d0006b5>	JAL	    04001ad4
	state.r[V1] = 0 + 0x001F;
	sp_reg_pc = 0x04001D08;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_04001D0C:	// 4001d0c: <2003001f>	ADDI      V1 = R0 + 001f
	state.r[V1] = 0 + 0x001F;


_04001D10:	// 4001d10: <c9d82001>	LQV       vec24 <00> = [T6 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T6]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[24].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[24].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[24].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[24].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[24].U64[0] = state.v[24].U64[0] & ~temp1;
			state.v[24].U64[1] = state.v[24].U64[1] & ~temp2;
			state.v[24].U64[0] = state.v[24].U64[0] | value1;
			state.v[24].U64[1] = state.v[24].U64[1] | value2;
		}
	}


_04001D14:	// 4001d14: <c9b92001>	LQV       vec25 <00> = [T5 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[25].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[25].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[25].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[25].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[25].U64[0] = state.v[25].U64[0] & ~temp1;
			state.v[25].U64[1] = state.v[25].U64[1] & ~temp2;
			state.v[25].U64[0] = state.v[25].U64[0] | value1;
			state.v[25].U64[1] = state.v[25].U64[1] | value2;
		}
	}


_04001D18:	// 4001d18: <4a0e73ac>	VXOR      vec14 = vec14 xor vec14[<none>]
	rsp_cop2_vxor(&state.v[14], &state.v[14], &state.v[14]);


_04001D1C:	// 4001d1c: <4a000000>	VMULF     vec00 = ( acc = (vec00 * vec00[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[0].U16[0], state.v[0].U16[0], state.v[0].U16[0], 1, 1, 1, 1)
		VMULF_operation(1, state.v[0].U16[1], state.v[0].U16[1], state.v[0].U16[1], 1, 1, 1, 1)
		VMULF_operation(2, state.v[0].U16[2], state.v[0].U16[2], state.v[0].U16[2], 1, 1, 1, 1)
		VMULF_operation(3, state.v[0].U16[3], state.v[0].U16[3], state.v[0].U16[3], 1, 1, 1, 1)
		VMULF_operation(4, state.v[0].U16[4], state.v[0].U16[4], state.v[0].U16[4], 1, 1, 1, 1)
		VMULF_operation(5, state.v[0].U16[5], state.v[0].U16[5], state.v[0].U16[5], 1, 1, 1, 1)
		VMULF_operation(6, state.v[0].U16[6], state.v[0].U16[6], state.v[0].U16[6], 1, 1, 1, 1)
		VMULF_operation(7, state.v[0].U16[7], state.v[0].U16[7], state.v[0].U16[7], 1, 1, 1, 1)
	}


_04001D20:	// 4001d20: <200c4000>	ADDI      T4 = R0 + 4000
	state.r[T4] = 0 + 0x4000;


_04001D24:	// 4001d24: <488c7800>	MTC2      vec15 <00> = T4
	state.v[15].U16[7] = (_u16)state.r[T4];


_04001D28:	// 4001d28: <4b0fc388>	VMACF     vec14 = ( acc += (vec24 * vec15[0]) << 1 ) >> 16
	{
		_u16		s2value = state.v[15].U16[7];
		VMACF_operation(0, state.v[14].U16[0], state.v[24].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[24].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[24].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[24].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[24].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[24].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[24].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[24].U16[7], s2value, 0, 1, 1, 1)
	}


_04001D2C:	// 4001d2c: <4b0fcb88>	VMACF     vec14 = ( acc += (vec25 * vec15[0]) << 1 ) >> 16
	{
		_u16		s2value = state.v[15].U16[7];
		VMACF_operation(0, state.v[14].U16[0], state.v[25].U16[0], s2value, 1, 0, 0, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[25].U16[1], s2value, 1, 0, 0, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[25].U16[2], s2value, 1, 0, 0, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[25].U16[3], s2value, 1, 0, 0, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[25].U16[4], s2value, 1, 0, 0, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[25].U16[5], s2value, 1, 0, 0, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[25].U16[6], s2value, 1, 0, 0, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[25].U16[7], s2value, 1, 0, 0, 1)
	}


_04001D30:	// 4001d30: <e9ce2001>	SQV       [T6 + 0x10] = vec14 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[14].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[14].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[14].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[14].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[14].U64[0] & temp1);
			value2 |= (state.v[14].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001D34:	// 4001d34: <e9ae2001>	SQV       [T5 + 0x10] = vec14 <00>
	{
		_u32    addr = (0x00000010 + state.r[T5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[14].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[14].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[14].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[14].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[14].U64[0] & temp1);
			value2 |= (state.v[14].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001D38:	// 4001d38: <01a00820>	ADD       AT = T5+R0
	state.r[AT] = (state.r[T5] + 0);


_04001D3C:	// 4001d3c: <334bffff>	ANDI      T3 = K0 & ffff
	state.r[T3] = (state.r[K0] & 0x0000FFFF);


_04001D40:	// 4001d40: <c9d82001>	LQV       vec24 <00> = [T6 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T6]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[24].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[24].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[24].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[24].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[24].U64[0] = state.v[24].U64[0] & ~temp1;
			state.v[24].U64[1] = state.v[24].U64[1] & ~temp2;
			state.v[24].U64[0] = state.v[24].U64[0] | value1;
			state.v[24].U64[1] = state.v[24].U64[1] | value2;
		}
	}


_04001D44:	// 4001d44: <c9dc1801>	LDV       vec28 <00> = [T6 + 0x08]
	rsp_ldv_compiler((state.r[T6] + 0x00000008) & 0xFFF, 28, 8);


_04001D48:	// 4001d48: <c9dc1c02>	LDV       vec28 <08> = [T6 + 0x10]
	rsp_ldv_compiler((state.r[T6] + 0x00000010) & 0xFFF, 28, 0);


_04001D4C:	// 4001d4c: <c9d41803>	LDV       vec20 <00> = [T6 + 0x18]
	rsp_ldv_compiler((state.r[T6] + 0x00000018) & 0xFFF, 20, 8);


_04001D50:	// 4001d50: <c9d41c04>	LDV       vec20 <08> = [T6 + 0x20]
	rsp_ldv_compiler((state.r[T6] + 0x00000020) & 0xFFF, 20, 0);


_04001D54:	// 4001d54: <21ce0002>	ADDI      T6 = T6 + 0002
	state.r[T6] = state.r[T6] + 0x0002;


_04001D58:	// 4001d58: <c9df1800>	LDV       vec31 <00> = [T6 + 0x00]
	rsp_ldv_compiler((state.r[T6] & 0xFFF), 31, 8);


_04001D5C:	// 4001d5c: <c9df1c01>	LDV       vec31 <08> = [T6 + 0x08]
	rsp_ldv_compiler((state.r[T6] + 0x00000008) & 0xFFF, 31, 0);


_04001D60:	// 4001d60: <c9d11802>	LDV       vec17 <00> = [T6 + 0x10]
	rsp_ldv_compiler((state.r[T6] + 0x00000010) & 0xFFF, 17, 8);


_04001D64:	// 4001d64: <c9d11c03>	LDV       vec17 <08> = [T6 + 0x18]
	rsp_ldv_compiler((state.r[T6] + 0x00000018) & 0xFFF, 17, 0);


_04001D68:	// 4001d68: <c9db1801>	LDV       vec27 <00> = [T6 + 0x08]
	rsp_ldv_compiler((state.r[T6] + 0x00000008) & 0xFFF, 27, 8);


_04001D6C:	// 4001d6c: <c9db1c02>	LDV       vec27 <08> = [T6 + 0x10]
	rsp_ldv_compiler((state.r[T6] + 0x00000010) & 0xFFF, 27, 0);


_04001D70:	// 4001d70: <c9d51803>	LDV       vec21 <00> = [T6 + 0x18]
	rsp_ldv_compiler((state.r[T6] + 0x00000018) & 0xFFF, 21, 8);


_04001D74:	// 4001d74: <c9d51c04>	LDV       vec21 <08> = [T6 + 0x20]
	rsp_ldv_compiler((state.r[T6] + 0x00000020) & 0xFFF, 21, 0);


_04001D78:	// 4001d78: <21ce0002>	ADDI      T6 = T6 + 0002
	state.r[T6] = state.r[T6] + 0x0002;


_04001D7C:	// 4001d7c: <c9de1800>	LDV       vec30 <00> = [T6 + 0x00]
	rsp_ldv_compiler((state.r[T6] & 0xFFF), 30, 8);


_04001D80:	// 4001d80: <c9de1c01>	LDV       vec30 <08> = [T6 + 0x08]
	rsp_ldv_compiler((state.r[T6] + 0x00000008) & 0xFFF, 30, 0);


_04001D84:	// 4001d84: <c9da1801>	LDV       vec26 <00> = [T6 + 0x08]
	rsp_ldv_compiler((state.r[T6] + 0x00000008) & 0xFFF, 26, 8);


_04001D88:	// 4001d88: <c9da1c02>	LDV       vec26 <08> = [T6 + 0x10]
	rsp_ldv_compiler((state.r[T6] + 0x00000010) & 0xFFF, 26, 0);


_04001D8C:	// 4001d8c: <c9d21802>	LDV       vec18 <00> = [T6 + 0x10]
	rsp_ldv_compiler((state.r[T6] + 0x00000010) & 0xFFF, 18, 8);


_04001D90:	// 4001d90: <c9d21c03>	LDV       vec18 <08> = [T6 + 0x18]
	rsp_ldv_compiler((state.r[T6] + 0x00000018) & 0xFFF, 18, 0);


_04001D94:	// 4001d94: <c9d61803>	LDV       vec22 <00> = [T6 + 0x18]
	rsp_ldv_compiler((state.r[T6] + 0x00000018) & 0xFFF, 22, 8);


_04001D98:	// 4001d98: <c9d61c04>	LDV       vec22 <08> = [T6 + 0x20]
	rsp_ldv_compiler((state.r[T6] + 0x00000020) & 0xFFF, 22, 0);


_04001D9C:	// 4001d9c: <21ce0002>	ADDI      T6 = T6 + 0002
	state.r[T6] = state.r[T6] + 0x0002;


_04001DA0:	// 4001da0: <c9dd1800>	LDV       vec29 <00> = [T6 + 0x00]
	rsp_ldv_compiler((state.r[T6] & 0xFFF), 29, 8);


_04001DA4:	// 4001da4: <c9dd1c01>	LDV       vec29 <08> = [T6 + 0x08]
	rsp_ldv_compiler((state.r[T6] + 0x00000008) & 0xFFF, 29, 0);


_04001DA8:	// 4001da8: <c9d91801>	LDV       vec25 <00> = [T6 + 0x08]
	rsp_ldv_compiler((state.r[T6] + 0x00000008) & 0xFFF, 25, 8);


_04001DAC:	// 4001dac: <c9d91c02>	LDV       vec25 <08> = [T6 + 0x10]
	rsp_ldv_compiler((state.r[T6] + 0x00000010) & 0xFFF, 25, 0);


_04001DB0:	// 4001db0: <c9d31802>	LDV       vec19 <00> = [T6 + 0x10]
	rsp_ldv_compiler((state.r[T6] + 0x00000010) & 0xFFF, 19, 8);


_04001DB4:	// 4001db4: <c9d31c03>	LDV       vec19 <08> = [T6 + 0x18]
	rsp_ldv_compiler((state.r[T6] + 0x00000018) & 0xFFF, 19, 0);


_04001DB8:	// 4001db8: <c9d71803>	LDV       vec23 <00> = [T6 + 0x18]
	rsp_ldv_compiler((state.r[T6] + 0x00000018) & 0xFFF, 23, 8);


_04001DBC:	// 4001dbc: <c9d71c04>	LDV       vec23 <08> = [T6 + 0x20]
	rsp_ldv_compiler((state.r[T6] + 0x00000020) & 0xFFF, 23, 0);


_04001DC0:	// 4001dc0: <c9af2000>	LQV       vec15 <00> = [T5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[15].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[15].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[15].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[15].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[15].U64[0] = state.v[15].U64[0] & ~temp1;
			state.v[15].U64[1] = state.v[15].U64[1] & ~temp2;
			state.v[15].U64[0] = state.v[15].U64[0] | value1;
			state.v[15].U64[1] = state.v[15].U64[1] | value2;
		}
	}


_04001DC4:	// 4001dc4: <c9702000>	LQV       vec16 <00> = [T3 + 0x00]

	// [start function: Zelda1DC4_1E14]

	Zelda1DC4_1E14();
	goto _04001E18;

_04001DC8:	// 4001dc8: <4a0e73ac>	VXOR      vec14 = vec14 xor vec14[<none>]

_04001DCC:	// 4001dcc: <4a000000>	VMULF     vec00 = ( acc = (vec00 * vec00[<none>]) << 1 ) >> 16

_04001DD0:	// 4001dd0: <4b2fbb88>	VMACF     vec14 = ( acc += (vec23 * vec15[1]) << 1 ) >> 16

_04001DD4:	// 4001dd4: <4b4fb388>	VMACF     vec14 = ( acc += (vec22 * vec15[2]) << 1 ) >> 16

_04001DD8:	// 4001dd8: <4b6fab88>	VMACF     vec14 = ( acc += (vec21 * vec15[3]) << 1 ) >> 16

_04001DDC:	// 4001ddc: <4b8fa388>	VMACF     vec14 = ( acc += (vec20 * vec15[4]) << 1 ) >> 16

_04001DE0:	// 4001de0: <4baf9b88>	VMACF     vec14 = ( acc += (vec19 * vec15[5]) << 1 ) >> 16

_04001DE4:	// 4001de4: <4bcf9388>	VMACF     vec14 = ( acc += (vec18 * vec15[6]) << 1 ) >> 16

_04001DE8:	// 4001de8: <4bef8b88>	VMACF     vec14 = ( acc += (vec17 * vec15[7]) << 1 ) >> 16

_04001DEC:	// 4001dec: <4b10c388>	VMACF     vec14 = ( acc += (vec24 * vec16[0]) << 1 ) >> 16

_04001DF0:	// 4001df0: <4b30cb88>	VMACF     vec14 = ( acc += (vec25 * vec16[1]) << 1 ) >> 16

_04001DF4:	// 4001df4: <4b50d388>	VMACF     vec14 = ( acc += (vec26 * vec16[2]) << 1 ) >> 16

_04001DF8:	// 4001df8: <4b70db88>	VMACF     vec14 = ( acc += (vec27 * vec16[3]) << 1 ) >> 16

_04001DFC:	// 4001dfc: <4b90e388>	VMACF     vec14 = ( acc += (vec28 * vec16[4]) << 1 ) >> 16

_04001E00:	// 4001e00: <4bb0eb88>	VMACF     vec14 = ( acc += (vec29 * vec16[5]) << 1 ) >> 16

_04001E04:	// 4001e04: <4bd0f388>	VMACF     vec14 = ( acc += (vec30 * vec16[6]) << 1 ) >> 16

_04001E08:	// 4001e08: <4bf0fb88>	VMACF     vec14 = ( acc += (vec31 * vec16[7]) << 1 ) >> 16

_04001E0C:	// 4001e0c: <21effff0>	ADDI      T7 = T7 + fff0

_04001E10:	// 4001e10: <e96e2000>	SQV       [T3 + 0x00] = vec14 <00>

_04001E14:	// 4001e14: <216b0010>	ADDI      T3 = T3 + 0010
	ErrorToCallFunction(0x04001E14);

	// [end of function: Zelda1DC4_1E14]

_04001E18:	// 4001e18: <1de0ffea>	BGTZ      (R0>0) --> 1dc4
	sp_reg_pc = 0x04001E18;
	if((_s32)state.r[T7] > 0)
	{
		{
			_u16		flag0 = 0;
			VADDC_operation(0, state.v[15].U16[0], state.v[0].U16[0], state.v[16].U16[0], 1, 1, 1, 1)
			VADDC_operation(1, state.v[15].U16[1], state.v[0].U16[1], state.v[16].U16[1], 1, 1, 1, 1)
			VADDC_operation(2, state.v[15].U16[2], state.v[0].U16[2], state.v[16].U16[2], 1, 1, 1, 1)
			VADDC_operation(3, state.v[15].U16[3], state.v[0].U16[3], state.v[16].U16[3], 1, 1, 1, 1)
			VADDC_operation(4, state.v[15].U16[4], state.v[0].U16[4], state.v[16].U16[4], 1, 1, 1, 1)
			VADDC_operation(5, state.v[15].U16[5], state.v[0].U16[5], state.v[16].U16[5], 1, 1, 1, 1)
			VADDC_operation(6, state.v[15].U16[6], state.v[0].U16[6], state.v[16].U16[6], 1, 1, 1, 1)
			VADDC_operation(7, state.v[15].U16[7], state.v[0].U16[7], state.v[16].U16[7], 1, 1, 1, 1)
			state.flag[0] = flag0;
		}
		goto _04001DC4;
	}


_04001E1C:	// 4001e1c: <4a1003d4>	VADDC	vec15 = vec00 + vec16[<none>]
	{
		_u16		flag0 = 0;
		VADDC_operation(0, state.v[15].U16[0], state.v[0].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADDC_operation(1, state.v[15].U16[1], state.v[0].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADDC_operation(2, state.v[15].U16[2], state.v[0].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADDC_operation(3, state.v[15].U16[3], state.v[0].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADDC_operation(4, state.v[15].U16[4], state.v[0].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADDC_operation(5, state.v[15].U16[5], state.v[0].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADDC_operation(6, state.v[15].U16[6], state.v[0].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADDC_operation(7, state.v[15].U16[7], state.v[0].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = flag0;
	}


_04001E20:	// 4001e20: <e9b02000>	SQV       [T5 + 0x00] = vec16 <00>
	{
		_u32    addr = (0x00000000 + state.r[T5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[16].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[16].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[16].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[16].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[16].U64[0] & temp1);
			value2 |= (state.v[16].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001E24:	// 4001e24: <090006b1>	J         04001ac4
	state.r[V1] = 0 + 0x001F;
	goto _04001AC4;


_04001E28:	// 4001e28: <2003001f>	ADDI      V1 = R0 + 001f
	state.r[V1] = 0 + 0x001F;


_04001E2C:	// 4001e2c: <4a1fffd4>	VADDC	vec31 = vec31 + vec31[<none>]
	{
		_u16		flag0 = 0;
		VADDC_operation(0, state.v[31].U16[0], state.v[31].U16[0], state.v[31].U16[0], 1, 0, 0, 1)
		VADDC_operation(1, state.v[31].U16[1], state.v[31].U16[1], state.v[31].U16[1], 1, 0, 0, 1)
		VADDC_operation(2, state.v[31].U16[2], state.v[31].U16[2], state.v[31].U16[2], 1, 0, 0, 1)
		VADDC_operation(3, state.v[31].U16[3], state.v[31].U16[3], state.v[31].U16[3], 1, 0, 0, 1)
		VADDC_operation(4, state.v[31].U16[4], state.v[31].U16[4], state.v[31].U16[4], 1, 0, 0, 1)
		VADDC_operation(5, state.v[31].U16[5], state.v[31].U16[5], state.v[31].U16[5], 1, 0, 0, 1)
		VADDC_operation(6, state.v[31].U16[6], state.v[31].U16[6], state.v[31].U16[6], 1, 0, 0, 1)
		VADDC_operation(7, state.v[31].U16[7], state.v[31].U16[7], state.v[31].U16[7], 1, 0, 0, 1)
		state.flag[0] = flag0;
	}


_04001E30:	// 4001e30: <001a9302>	SRL       S2 = K0 >> 12
	state.r[S2] = ((_u32)state.r[K0] >> 12);


_04001E34:	// 4001e34: <32520ff0>	ANDI      S2 = S2 & 0ff0
	state.r[S2] = (state.r[S2] & 0x00000FF0);


_04001E38:	// 4001e38: <3333ffff>	ANDI      S3 = T9 & ffff
	state.r[S3] = (state.r[T9] & 0x0000FFFF);


_04001E3C:	// 4001e3c: <0019a402>	SRL       S4 = T9 >> 16
	state.r[S4] = ((_u32)state.r[T9] >> 16);


_04001E40:	// 4001e40: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_04001E44:	// 4001e44: <ca9d2000>	LQV       vec29 <00> = [S4 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[29].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[29].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[29].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[29].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[29].U64[0] = state.v[29].U64[0] & ~temp1;
			state.v[29].U64[1] = state.v[29].U64[1] & ~temp2;
			state.v[29].U64[0] = state.v[29].U64[0] | value1;
			state.v[29].U64[1] = state.v[29].U64[1] | value2;
		}
	}


_04001E48:	// 4001e48: <ca7a2001>	LQV       vec26 <00> = [S3 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[26].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[26].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[26].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[26].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[26].U64[0] = state.v[26].U64[0] & ~temp1;
			state.v[26].U64[1] = state.v[26].U64[1] & ~temp2;
			state.v[26].U64[0] = state.v[26].U64[0] | value1;
			state.v[26].U64[1] = state.v[26].U64[1] | value2;
		}
	}


_04001E4C:	// 4001e4c: <ca9c2001>	LQV       vec28 <00> = [S4 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[28].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[28].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[28].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[28].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[28].U64[0] = state.v[28].U64[0] & ~temp1;
			state.v[28].U64[1] = state.v[28].U64[1] & ~temp2;
			state.v[28].U64[0] = state.v[28].U64[0] | value1;
			state.v[28].U64[1] = state.v[28].U64[1] | value2;
		}
	}


_04001E50:	// 4001e50: <ca792002>	LQV       vec25 <00> = [S3 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[25].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[25].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[25].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[25].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[25].U64[0] = state.v[25].U64[0] & ~temp1;
			state.v[25].U64[1] = state.v[25].U64[1] & ~temp2;
			state.v[25].U64[0] = state.v[25].U64[0] | value1;
			state.v[25].U64[1] = state.v[25].U64[1] | value2;
		}
	}


_04001E54:	// 4001e54: <ca972002>	LQV       vec23 <00> = [S4 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[23].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[23].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[23].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[23].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[23].U64[0] = state.v[23].U64[0] & ~temp1;
			state.v[23].U64[1] = state.v[23].U64[1] & ~temp2;
			state.v[23].U64[0] = state.v[23].U64[0] | value1;
			state.v[23].U64[1] = state.v[23].U64[1] | value2;
		}
	}


_04001E58:	// 4001e58: <ca782003>	LQV       vec24 <00> = [S3 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[24].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[24].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[24].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[24].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[24].U64[0] = state.v[24].U64[0] & ~temp1;
			state.v[24].U64[1] = state.v[24].U64[1] & ~temp2;
			state.v[24].U64[0] = state.v[24].U64[0] | value1;
			state.v[24].U64[1] = state.v[24].U64[1] | value2;
		}
	}


_04001E5C:	// 4001e5c: <ca962003>	LQV       vec22 <00> = [S4 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[22].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[22].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[22].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[22].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[22].U64[0] = state.v[22].U64[0] & ~temp1;
			state.v[22].U64[1] = state.v[22].U64[1] & ~temp2;
			state.v[22].U64[0] = state.v[22].U64[0] | value1;
			state.v[22].U64[1] = state.v[22].U64[1] | value2;
		}
	}


_04001E60:	// 4001e60: <22940040>	ADDI      S4 = S4 + 0040
	state.r[S4] = state.r[S4] + 0x0040;


_04001E64:	// 4001e64: <4a1dded0>	VADD      vec27 = vec27 + vec29[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[27].U16[0], state.v[27].U16[0], state.v[29].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[27].U16[1], state.v[27].U16[1], state.v[29].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[27].U16[2], state.v[27].U16[2], state.v[29].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[27].U16[3], state.v[27].U16[3], state.v[29].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[27].U16[4], state.v[27].U16[4], state.v[29].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[27].U16[5], state.v[27].U16[5], state.v[29].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[27].U16[6], state.v[27].U16[6], state.v[29].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[27].U16[7], state.v[27].U16[7], state.v[29].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001E68:	// 4001e68: <4a1cd690>	VADD      vec26 = vec26 + vec28[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[26].U16[0], state.v[26].U16[0], state.v[28].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[26].U16[1], state.v[26].U16[1], state.v[28].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[26].U16[2], state.v[26].U16[2], state.v[28].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[26].U16[3], state.v[26].U16[3], state.v[28].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[26].U16[4], state.v[26].U16[4], state.v[28].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[26].U16[5], state.v[26].U16[5], state.v[28].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[26].U16[6], state.v[26].U16[6], state.v[28].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[26].U16[7], state.v[26].U16[7], state.v[28].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001E6C:	// 4001e6c: <4a17ce50>	VADD      vec25 = vec25 + vec23[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[25].U16[0], state.v[25].U16[0], state.v[23].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[25].U16[1], state.v[25].U16[1], state.v[23].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[25].U16[2], state.v[25].U16[2], state.v[23].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[25].U16[3], state.v[25].U16[3], state.v[23].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[25].U16[4], state.v[25].U16[4], state.v[23].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[25].U16[5], state.v[25].U16[5], state.v[23].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[25].U16[6], state.v[25].U16[6], state.v[23].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[25].U16[7], state.v[25].U16[7], state.v[23].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001E70:	// 4001e70: <4a16c610>	VADD      vec24 = vec24 + vec22[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[24].U16[0], state.v[24].U16[0], state.v[22].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[24].U16[1], state.v[24].U16[1], state.v[22].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[24].U16[2], state.v[24].U16[2], state.v[22].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[24].U16[3], state.v[24].U16[3], state.v[22].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[24].U16[4], state.v[24].U16[4], state.v[22].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[24].U16[5], state.v[24].U16[5], state.v[22].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[24].U16[6], state.v[24].U16[6], state.v[22].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[24].U16[7], state.v[24].U16[7], state.v[22].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001E74:	// 4001e74: <2252ffc0>	ADDI      S2 = S2 + ffc0
	state.r[S2] = state.r[S2] + 0xFFFFFFC0;


_04001E78:	// 4001e78: <ea7b2000>	SQV       [S3 + 0x00] = vec27 <00>
	{
		_u32    addr = (0x00000000 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[27].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[27].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[27].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[27].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[27].U64[0] & temp1);
			value2 |= (state.v[27].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001E7C:	// 4001e7c: <ea7a2001>	SQV       [S3 + 0x10] = vec26 <00>
	{
		_u32    addr = (0x00000010 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[26].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[26].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[26].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[26].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[26].U64[0] & temp1);
			value2 |= (state.v[26].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001E80:	// 4001e80: <ea792002>	SQV       [S3 + 0x20] = vec25 <00>
	{
		_u32    addr = (0x00000020 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[25].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[25].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[25].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[25].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[25].U64[0] & temp1);
			value2 |= (state.v[25].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001E84:	// 4001e84: <ea782003>	SQV       [S3 + 0x30] = vec24 <00>
	{
		_u32    addr = (0x00000030 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[24].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[24].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[24].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[24].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[24].U64[0] & temp1);
			value2 |= (state.v[24].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001E88:	// 4001e88: <22730040>	ADDI      S3 = S3 + 0040
	state.r[S3] = state.r[S3] + 0x0040;


_04001E8C:	// 4001e8c: <1e40ffed>	BGTZ      (R0>0) --> 1e44
	sp_reg_pc = 0x04001E8C;
	if((_s32)state.r[S2] > 0)
	{
		{
			_u32 addr = (0x00000000 + state.r[S3]);
			int shifter = ((addr & 0xf) << 3);

			if (shifter == 0 && (addr&3) == 0 )
			{
				addr &= 0xfff;
				state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
				state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
				state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
				state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
			}
			else
			{
				_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
				_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
				_u64		value1 = Load64_DMEM(addr+8);
				_u64		value2 = Load64_DMEM(addr);
				if (shifter > 0x40)
				{
					temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
					temp1 = 0;
				} else {
					temp1 = ((temp1 >> shifter) << shifter);
				}
				value1 = value1 & temp1;
				value2 = value2 & temp2;

				state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
				state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
				state.v[27].U64[0] = state.v[27].U64[0] | value1;
				state.v[27].U64[1] = state.v[27].U64[1] | value2;
			}
		}
		goto _04001E44;
	}


_04001E90:	// 4001e90: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_04001E94:	// 4001e94: <09000423>	J         0400108c
	goto _0400108C;


_04001E98:	// 4001e98: <00000000>	NOP       


_04001E9C:	// 4001e9c: <870e0000>	LH        T6 = [T8+0000]
	state.r[T6] = (_s32)(_s16)Load16_DMEM((state.r[T8] + 0x00000000) & 0xfff);


_04001EA0:	// 4001ea0: <870f0002>	LH        T7 = [T8+0002]
	state.r[T7] = (_s32)(_s16)Load16_DMEM((state.r[T8] + 0x00000002) & 0xfff);


_04001EA4:	// 4001ea4: <870d0004>	LH        T5 = [T8+0004]
	state.r[T5] = (_s32)(_s16)Load16_DMEM((state.r[T8] + 0x00000004) & 0xfff);


_04001EA8:	// 4001ea8: <334cffff>	ANDI      T4 = K0 & ffff
	state.r[T4] = (state.r[K0] & 0x0000FFFF);


_04001EAC:	// 4001eac: <000c6080>	SLL       T4 = T4 << 2
	state.r[T4] = ((_u32)state.r[T4] << 2);


_04001EB0:	// 4001eb0: <332affff>	ANDI      T2 = T9 & ffff
	state.r[T2] = (state.r[T9] & 0x0000FFFF);


_04001EB4:	// 4001eb4: <000e7400>	SLL       T6 = T6 << 16
	state.r[T6] = ((_u32)state.r[T6] << 16);


_04001EB8:	// 4001eb8: <014e5025>	OR        T2 = T2 | T6
	state.r[T2] = (state.r[T2] | state.r[T6]);


_04001EBC:	// 4001ebc: <000a5c02>	SRL       T3 = T2 >> 16
	state.r[T3] = ((_u32)state.r[T2] >> 16);


_04001EC0:	// 4001ec0: <316bfffe>	ANDI      T3 = T3 & fffe
	state.r[T3] = (state.r[T3] & 0x0000FFFE);


_04001EC4:	// 4001ec4: <c9610800>	LSV       vec01 <00> = [T3 + 0x00]
	state.v[1].U16[7] = Load16_DMEM(0x00000000 + state.r[T3]);


_04001EC8:	// 4001ec8: <014c5020>	ADD       T2 = T2+T4
	state.r[T2] = (state.r[T2] + state.r[T4]);


_04001ECC:	// 4001ecc: <000a5c02>	SRL       T3 = T2 >> 16
	state.r[T3] = ((_u32)state.r[T2] >> 16);


_04001ED0:	// 4001ed0: <316bfffe>	ANDI      T3 = T3 & fffe
	state.r[T3] = (state.r[T3] & 0x0000FFFE);


_04001ED4:	// 4001ed4: <c9610900>	LSV       vec01 <02> = [T3 + 0x00]
	state.v[1].U16[6] = Load16_DMEM(0x00000000 + state.r[T3]);


_04001ED8:	// 4001ed8: <014c5020>	ADD       T2 = T2+T4
	state.r[T2] = (state.r[T2] + state.r[T4]);


_04001EDC:	// 4001edc: <000a5c02>	SRL       T3 = T2 >> 16
	state.r[T3] = ((_u32)state.r[T2] >> 16);


_04001EE0:	// 4001ee0: <316bfffe>	ANDI      T3 = T3 & fffe
	state.r[T3] = (state.r[T3] & 0x0000FFFE);


_04001EE4:	// 4001ee4: <c9610a00>	LSV       vec01 <04> = [T3 + 0x00]
	state.v[1].U16[5] = Load16_DMEM(0x00000000 + state.r[T3]);


_04001EE8:	// 4001ee8: <014c5020>	ADD       T2 = T2+T4
	state.r[T2] = (state.r[T2] + state.r[T4]);


_04001EEC:	// 4001eec: <000a5c02>	SRL       T3 = T2 >> 16
	state.r[T3] = ((_u32)state.r[T2] >> 16);


_04001EF0:	// 4001ef0: <316bfffe>	ANDI      T3 = T3 & fffe
	state.r[T3] = (state.r[T3] & 0x0000FFFE);


_04001EF4:	// 4001ef4: <c9610b00>	LSV       vec01 <06> = [T3 + 0x00]
	state.v[1].U16[4] = Load16_DMEM(0x00000000 + state.r[T3]);


_04001EF8:	// 4001ef8: <014c5020>	ADD       T2 = T2+T4
	state.r[T2] = (state.r[T2] + state.r[T4]);


_04001EFC:	// 4001efc: <21adfff8>	ADDI      T5 = T5 + fff8
	state.r[T5] = state.r[T5] + 0xFFFFFFF8;


_04001F00:	// 4001f00: <e9e11800>	SDV       [T7 + 0x00] = vec01 <00>
	{
		_u32	addr = (0x00000000 + state.r[T7]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[1].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[1].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[1].U32[0x00000003];
		}
	}


_04001F04:	// 4001f04: <21ef0008>	ADDI      T7 = T7 + 0008
	state.r[T7] = state.r[T7] + 0x0008;


_04001F08:	// 4001f08: <1da0ffec>	BGTZ      (R0>0) --> 1ebc
	sp_reg_pc = 0x04001F08;
	if((_s32)state.r[T5] > 0)
	{
		goto _04001EBC;
	}


_04001F0C:	// 4001f0c: <00000000>	NOP       


_04001F10:	// 4001f10: <0d000423>	JAL	    0400108c
	sp_reg_pc = 0x04001F10;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400108C;


_04001F14:	// 4001f14: <00000000>	NOP       


_04001F18:	// 4001f18: <c81f2000>	LQV       vec31 <00> = [R0 + 0x00]
	{
		_u32 addr = (0x00000000 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001F1C:	// 4001f1c: <3352ffff>	ANDI      S2 = K0 & ffff
	state.r[S2] = (state.r[K0] & 0x0000FFFF);


_04001F20:	// 4001f20: <3333ffff>	ANDI      S3 = T9 & ffff
	state.r[S3] = (state.r[T9] & 0x0000FFFF);


_04001F24:	// 4001f24: <0019a402>	SRL       S4 = T9 >> 16
	state.r[S4] = ((_u32)state.r[T9] >> 16);


_04001F28:	// 4001f28: <ca9d2000>	LQV       vec29 <00> = [S4 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[29].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[29].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[29].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[29].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[29].U64[0] = state.v[29].U64[0] & ~temp1;
			state.v[29].U64[1] = state.v[29].U64[1] & ~temp2;
			state.v[29].U64[0] = state.v[29].U64[0] | value1;
			state.v[29].U64[1] = state.v[29].U64[1] | value2;
		}
	}


_04001F2C:	// 4001f2c: <ca9c2001>	LQV       vec28 <00> = [S4 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[28].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[28].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[28].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[28].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[28].U64[0] = state.v[28].U64[0] & ~temp1;
			state.v[28].U64[1] = state.v[28].U64[1] & ~temp2;
			state.v[28].U64[0] = state.v[28].U64[0] | value1;
			state.v[28].U64[1] = state.v[28].U64[1] | value2;
		}
	}


_04001F30:	// 4001f30: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_04001F34:	// 4001f34: <ca7a2001>	LQV       vec26 <00> = [S3 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[26].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[26].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[26].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[26].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[26].U64[0] = state.v[26].U64[0] & ~temp1;
			state.v[26].U64[1] = state.v[26].U64[1] & ~temp2;
			state.v[26].U64[0] = state.v[26].U64[0] | value1;
			state.v[26].U64[1] = state.v[26].U64[1] | value2;
		}
	}


_04001F38:	// 4001f38: <2252ffe0>	ADDI      S2 = S2 + ffe0
	state.r[S2] = state.r[S2] + 0xFFFFFFE0;


_04001F3C:	// 4001f3c: <22940020>	ADDI      S4 = S4 + 0020
	state.r[S4] = state.r[S4] + 0x0020;


_04001F40:	// 4001f40: <4a1de6c6>	VMUDN     vec27 = ( acc = vec28 * vec29[<none>]      ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[28*16].U16[0], 0xE4
			MOVQ mm1, state.v[28*16].U16[4*2]
			PSHUFW mm4, state.v[29*16].U16[0], 0xE4
			MOVQ mm5, state.v[29*16].U16[4*2]
			PSHUFW mm6, mm0, 0xE4
			MOVQ mm7, mm1
			PMULLW mm6, mm4
			PMULLW mm7, mm5
			MOVQ accumulator[0*4], mm6
			MOVQ accumulator[2*4], mm7
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm2, mm4
			PMULHUW mm3, mm5
			PSRAW mm0, 15
			PSRAW mm1, 15
			PMULLW mm0, mm4
			PMULLW mm1, mm5
			PADDW mm0, mm2
			PADDW mm1, mm3
			MOVQ state.v[27*16].U16[0], mm0
			MOVQ state.v[27*16].U16[4*2], mm1
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_04001F44:	// 4001f44: <4a1de68e>	VMADN     vec26 = ( acc+= vec28 * vec29[<none>] )
	{
		VMADN_operation(0, state.v[26].U16[0], state.v[28].U16[0], state.v[29].U16[0], 1, 1, 1, 1)
		VMADN_operation(1, state.v[26].U16[1], state.v[28].U16[1], state.v[29].U16[1], 1, 1, 1, 1)
		VMADN_operation(2, state.v[26].U16[2], state.v[28].U16[2], state.v[29].U16[2], 1, 1, 1, 1)
		VMADN_operation(3, state.v[26].U16[3], state.v[28].U16[3], state.v[29].U16[3], 1, 1, 1, 1)
		VMADN_operation(4, state.v[26].U16[4], state.v[28].U16[4], state.v[29].U16[4], 1, 1, 1, 1)
		VMADN_operation(5, state.v[26].U16[5], state.v[28].U16[5], state.v[29].U16[5], 1, 1, 1, 1)
		VMADN_operation(6, state.v[26].U16[6], state.v[28].U16[6], state.v[29].U16[6], 1, 1, 1, 1)
		VMADN_operation(7, state.v[26].U16[7], state.v[28].U16[7], state.v[29].U16[7], 1, 1, 1, 1)
	}


_04001F48:	// 4001f48: <ea7b2001>	SQV       [S3 + 0x10] = vec27 <00>
	{
		_u32    addr = (0x00000010 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[27].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[27].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[27].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[27].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[27].U64[0] & temp1);
			value2 |= (state.v[27].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001F4C:	// 4001f4c: <ea7a2000>	SQV       [S3 + 0x00] = vec26 <00>
	{
		_u32    addr = (0x00000000 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[26].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[26].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[26].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[26].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[26].U64[0] & temp1);
			value2 |= (state.v[26].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001F50:	// 4001f50: <ca9d2000>	LQV       vec29 <00> = [S4 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[29].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[29].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[29].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[29].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[29].U64[0] = state.v[29].U64[0] & ~temp1;
			state.v[29].U64[1] = state.v[29].U64[1] & ~temp2;
			state.v[29].U64[0] = state.v[29].U64[0] | value1;
			state.v[29].U64[1] = state.v[29].U64[1] | value2;
		}
	}


_04001F54:	// 4001f54: <ca9c2001>	LQV       vec28 <00> = [S4 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[28].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[28].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[28].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[28].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[28].U64[0] = state.v[28].U64[0] & ~temp1;
			state.v[28].U64[1] = state.v[28].U64[1] & ~temp2;
			state.v[28].U64[0] = state.v[28].U64[0] | value1;
			state.v[28].U64[1] = state.v[28].U64[1] | value2;
		}
	}


_04001F58:	// 4001f58: <1e40fff7>	BGTZ      (R0>0) --> 1f38
	sp_reg_pc = 0x04001F58;
	if((_s32)state.r[S2] > 0)
	{
		state.r[S3] = state.r[S3] + 0x0020;
		goto _04001F38;
	}


_04001F5C:	// 4001f5c: <22730020>	ADDI      S3 = S3 + 0020
	state.r[S3] = state.r[S3] + 0x0020;


_04001F60:	// 4001f60: <09000423>	J         0400108c
	goto _0400108C;


_04001F64:	// 4001f64: <00000000>	NOP       


_04001F68:	// 4001f68: <0d00069b>	JAL	    04001a6c
	sp_reg_pc = 0x04001F68;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001A6C;


_04001F6C:	// 4001f6c: <00000000>	NOP       


_04001F70:	// 4001f70: <22e10000>	ADDI      AT = S7 + 0000
	state.r[AT] = state.r[S7] + 0x0000;


_04001F74:	// 4001f74: <0d0006b5>	JAL	    04001ad4
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x04001F74;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_04001F78:	// 4001f78: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001F7C:	// 4001f7c: <8c260004>	LW        A2 = [AT+0004]
	state.r[A2] = (_s32)Load32_DMEM((state.r[AT] + 0x00000004) & 0xfff);


_04001F80:	// 4001f80: <20c60001>	ADDI      A2 = A2 + 0001
	state.r[A2] = state.r[A2] + 0x0001;


_04001F84:	// 4001f84: <ac260004>	SW        [AT+0004] = A2
	Save32_DMEM((_u32)state.r[A2], (state.r[AT] + 0x00000004) & 0xfff);


_04001F88:	// 4001f88: <ac3b0008>	SW        [AT+0008] = K1
	Save32_DMEM((_u32)state.r[K1], (state.r[AT] + 0x00000008) & 0xfff);


_04001F8C:	// 4001f8c: <ac3e000c>	SW        [AT+000c] = S8
	Save32_DMEM((_u32)state.r[S8], (state.r[AT] + 0x0000000C) & 0xfff);


_04001F90:	// 4001f90: <3346ffff>	ANDI      A2 = K0 & ffff
	state.r[A2] = (state.r[K0] & 0x0000FFFF);


_04001F94:	// 4001f94: <0d0006bf>	JAL	    04001afc
	Save16_DMEM((_u16)state.r[A2], (state.r[AT] + 0x00000000) & 0xfff);
	sp_reg_pc = 0x04001F94;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AFC;


_04001F98:	// 4001f98: <a4260000>	SH        [AT+0000] = A2
	Save16_DMEM((_u16)state.r[A2], (state.r[AT] + 0x00000000) & 0xfff);


_04001F9C:	// 4001f9c: <09000423>	J         0400108c
	goto _0400108C;


_04001FA0:	// 4001fa0: <00000000>	NOP       


_04001FA4:	// 4001fa4: <00000000>	NOP       


_04001FA8:	// 4001fa8: <00000000>	NOP       


_04001FAC:	// 4001fac: <00000000>	NOP       


_04001FB0:	// 4001fb0: <4a10802a>	VOR       vec00 = vec16 or vec16[<none>]
	rsp_cop2_vor(&state.v[0], &state.v[16], &state.v[16]);


_04001FB4:	// 4001fb4: <c81f201b>	LQV       vec31 <00> = [R0 + 0xb0]
	{
		_u32 addr = (0x000001B0 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001FB8:	// 4001fb8: <c81e201c>	LQV       vec30 <00> = [R0 + 0xc0]
	{
		_u32 addr = (0x000001C0 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[30].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[30].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[30].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[30].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[30].U64[0] = state.v[30].U64[0] & ~temp1;
			state.v[30].U64[1] = state.v[30].U64[1] & ~temp2;
			state.v[30].U64[0] = state.v[30].U64[0] | value1;
			state.v[30].U64[1] = state.v[30].U64[1] | value2;
		}
	}


_04001FBC:	// 4001fbc: <20170ba8>	ADDI      S7 = R0 + 0ba8
	state.r[S7] = 0 + 0x0BA8;


_04001FC0:	// 4001fc0: <20160d00>	ADDI      S6 = R0 + 0d00
	state.r[S6] = 0 + 0x0D00;


_04001FC4:	// 4001fc4: <4b1f0051>	VSUB      vec01 = vec00 - vec31[0]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[31].U16[7];
		VSUB_operation(0, state.v[1].U16[0], state.v[0].U16[0], s2value, 1, 0, 0, 1)
		VSUB_operation(1, state.v[1].U16[1], state.v[0].U16[1], s2value, 1, 0, 0, 1)
		VSUB_operation(2, state.v[1].U16[2], state.v[0].U16[2], s2value, 1, 0, 0, 1)
		VSUB_operation(3, state.v[1].U16[3], state.v[0].U16[3], s2value, 1, 0, 0, 1)
		VSUB_operation(4, state.v[1].U16[4], state.v[0].U16[4], s2value, 1, 0, 0, 1)
		VSUB_operation(5, state.v[1].U16[5], state.v[0].U16[5], s2value, 1, 0, 0, 1)
		VSUB_operation(6, state.v[1].U16[6], state.v[0].U16[6], s2value, 1, 0, 0, 1)
		VSUB_operation(7, state.v[1].U16[7], state.v[0].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001FC8:	// 4001fc8: <8c0b00f0>	LW        T3 = [R0+00f0]
	state.r[T3] = (_s32)Load32_DMEM((0 + 0x000000F0) & 0xfff);


_04001FCC:	// 4001fcc: <8c0c0fc4>	LW        T4 = [R0+0fc4]
	state.r[T4] = (_s32)Load32_DMEM((0 + 0x00000FC4) & 0xfff);


_04001FD0:	// 4001fd0: <20012800>	ADDI      AT = R0 + 2800
	state.r[AT] = 0 + 0x2800;


_04001FD4:	// 4001fd4: <11600006>	BEQ       (R0==T3) --> 1ff0
	sp_reg_pc = 0x04001FD4;
	if (state.r[T3] == 0)
	{
		goto _04001FF0;
	}


_04001FD8:	// 4001fd8: <40812000>	MTC0      SP status = AT


_04001FDC:	// 4001fdc: <318c0001>	ANDI      T4 = T4 & 0001
	state.r[T4] = (state.r[T4] & 0x00000001);


_04001FE0:	// 4001fe0: <1180001e>	BEQ       (R0==T4) --> 205c
	sp_reg_pc = 0x04001FE0;
	if (state.r[T4] == 0)
	{
		Save32_DMEM((_u32)0, (0 + 0x00000FC4) & 0xfff);
		state.halt = 1;
		return;
	}


_04001FE4:	// 4001fe4: <ac000fc4>	SW        [R0+0fc4] = R0
	Save32_DMEM((_u32)0, (0 + 0x00000FC4) & 0xfff);


_04001FE8:	// 4001fe8: <08000459>	J         00001164
	state.r[K0] = (_s32)Load32_DMEM((0 + 0x00000BF8) & 0xfff);
	state.halt = 1;
	return;


_04001FEC:	// 4001fec: <8c1a0bf8>	LW        K0 = [R0+0bf8]
	state.r[K0] = (_s32)Load32_DMEM((0 + 0x00000BF8) & 0xfff);

_04001FF0:
_04001FF4:
_04001FF8:
_04001FFC:
	state.halt = 1;
	return;

	_BuildJumpTable:
	#include "jumptable.h"
	goto _Label;
}

void _ZeldaData(_u32 StartAddress)
{
	static _u32	JumpTable[(0x1000 >> 2)];
	static _u32	BuildJumpTable = 0;

	if (!BuildJumpTable)
	{
		BuildJumpTable = 1;
		goto _BuildJumpTable;
	}

	_Label:
	StartAddress = JumpTable[(StartAddress & 0x0FFF) >> 2];
	__asm mov eax, StartAddress
	__asm jmp eax


_04001000:	// 4001000: <340a0fc0>	ORI       T2 = R0 | 0x0fc0
	state.r[T2] = (0 | 0x00000FC0);


_04001004:	// 4001004: <8d420018>	LW        V0 = [T2+0018]
	state.r[V0] = (_s32)Load32_DMEM((state.r[T2] + 0x00000018) & 0xfff);


_04001008:	// 4001008: <8d43001c>	LW        V1 = [T2+001c]
	state.r[V1] = (_s32)Load32_DMEM((state.r[T2] + 0x0000001C) & 0xfff);


_0400100C:	// 400100c: <40803800>	MTC0      SP semaphore = R0


_04001010:	// 4001010: <0d0006b5>	JAL	    04001ad4
	state.r[AT] = 0 + 0x0000;
	sp_reg_pc = 0x04001010;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_04001014:	// 4001014: <20010000>	ADDI      AT = R0 + 0000
	state.r[AT] = 0 + 0x0000;


_04001018:	// 4001018: <201802e0>	ADDI      T8 = R0 + 02e0
	state.r[T8] = 0 + 0x02E0;


_0400101C:	// 400101c: <20170fb0>	ADDI      S7 = R0 + 0fb0
	state.r[S7] = 0 + 0x0FB0;


_04001020:	// 4001020: <8d5c0030>	LW        GP = [T2+0030]
	state.r[GP] = (_s32)Load32_DMEM((state.r[T2] + 0x00000030) & 0xfff);


_04001024:	// 4001024: <8d5b0034>	LW        K1 = [T2+0034]
	state.r[K1] = (_s32)Load32_DMEM((state.r[T2] + 0x00000034) & 0xfff);


_04001028:	// 4001028: <40055800>	MFC0      T3 = SP DMA full
	state.r[A1] = 0;


_0400102C:	// 400102c: <30a40001>	ANDI      A0 = A1 & 0001
	state.r[A0] = (state.r[A1] & 0x00000001);


_04001030:	// 4001030: <10800006>	BEQ       (R0==A0) --> 104c
	sp_reg_pc = 0x04001030;
	if (state.r[A0] == 0)
	{
		state.r[A0] = (state.r[A1] & 0x00000100);
		goto _0400104C;
	}


_04001034:	// 4001034: <30a40100>	ANDI      A0 = A1 & 0100
	state.r[A0] = (state.r[A1] & 0x00000100);


_04001038:	// 4001038: <10800004>	BEQ       (R0==A0) --> 104c
	sp_reg_pc = 0x04001038;
	if (state.r[A0] == 0)
	{
		goto _0400104C;
	}


_0400103C:	// 400103c: <00000000>	NOP       


_04001040:	// 4001040: <40045800>	MFC0      T3 = SP status
	state.r[A0] = 0;


_04001044:	// 4001044: <30840100>	ANDI      A0 = A0 & 0100
	state.r[A0] = (state.r[A0] & 0x00000100);


_04001048:	// 4001048: <1c80fffd>	BGTZ      (R0>0) --> 1040
	sp_reg_pc = 0x04001048;
	if((_s32)state.r[A0] > 0)
	{
		goto _04001040;
	}


_0400104C:	// 400104c: <00000000>	NOP       


_04001050:	// 4001050: <0d000431>	JAL	    040010c4
	sp_reg_pc = 0x04001050;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _040010C4;


_04001054:	// 4001054: <00000000>	NOP       


_04001058:	// 4001058: <8fba0000>	LW        K0 = [SP+0000]
	// In function: Main_Parser
	//Call the function: Main_Parser
	Main_Parser();
	goto _04001080;

_0400105C:	// 400105c: <8fb90004>	LW        T9 = [SP+0004]
	ErrorToCallFunction(0x0400105C);

_04001060:	// 4001060: <001a0dc2>	SRL       AT = K0 >> 23
	ErrorToCallFunction(0x04001060);

_04001064:	// 4001064: <302100fe>	ANDI      AT = AT & 00fe
	ErrorToCallFunction(0x04001064);

_04001068:	// 4001068: <239c0008>	ADDI      GP = GP + 0008
	ErrorToCallFunction(0x04001068);

_0400106C:	// 400106c: <237bfff8>	ADDI      K1 = K1 + fff8
	ErrorToCallFunction(0x0400106C);

_04001070:	// 4001070: <23bd0008>	ADDI      SP = SP + 0008
	ErrorToCallFunction(0x04001070);

_04001074:	// 4001074: <23defff8>	ADDI      S8 = S8 + fff8
	ErrorToCallFunction(0x04001074);

_04001078:	// 4001078: <00011020>	ADD       V0 = R0+AT
	ErrorToCallFunction(0x04001078);

_0400107C:	// 400107c: <84420010>	LH        V0 = [V0+0010]
	ErrorToCallFunction(0x0400107C);

_04001080:	// 4001080: <00400008>	JR        V0
	{
		_u32	temp = ((state.r[V0] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001084:	// 4001084: <00000000>	NOP       


_04001088:	// 4001088: <0000000d>	BREAK
	state.halt = 1;
	return;


_0400108C:	// 400108c: <1fc0fff2>	BGTZ      (R0>0) --> 1058
	sp_reg_pc = 0x0400108C;
	if((_s32)state.r[S8] > 0)
	{
		goto _04001058;
	}


_04001090:	// 4001090: <00000000>	NOP       


_04001094:	// 4001094: <1b600005>	BLEZ      (R0<=0) --> 10ac
	sp_reg_pc = 0x04001094;
	if ((_s32)state.r[K1] <= 0)
	{
		goto _040010AC;
	}


_04001098:	// 4001098: <00000000>	NOP       


_0400109C:	// 400109c: <0d000431>	JAL	    040010c4
	sp_reg_pc = 0x0400109C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _040010C4;


_040010A0:	// 40010a0: <00000000>	NOP       


_040010A4:	// 40010a4: <09000416>	J         04001058
	goto _04001058;


_040010A8:	// 40010a8: <00000000>	NOP       


_040010AC:	// 40010ac: <34014000>	ORI       AT = R0 | 0x4000
	state.r[AT] = (0 | 0x00004000);


_040010B0:	// 40010b0: <40812000>	MTC0      SP status = AT


_040010B4:	// 40010b4: <0000000d>	BREAK
	state.halt = 1;
	return;


_040010B8:	// 40010b8: <00000000>	NOP       


_040010BC:	// 40010bc: <1000ffff>	BEQ       (R0==R0) --> 10bc
	sp_reg_pc = 0x040010BC;
	if (0 == 0)
	{
		goto _040010BC;
	}


_040010C0:	// 40010c0: <00000000>	NOP       


_040010C4:	// 40010c4: <23e50000>	ADDI      A1 = RA + 0000
	state.r[A1] = state.r[RA] + 0x0000;


_040010C8:	// 40010c8: <001c1020>	ADD       V0 = R0+GP
	state.r[V0] = (0 + state.r[GP]);


_040010CC:	// 40010cc: <23630000>	ADDI      V1 = K1 + 0000
	state.r[V1] = state.r[K1] + 0x0000;


_040010D0:	// 40010d0: <2064ffc0>	ADDI      A0 = V1 + ffc0
	state.r[A0] = state.r[V1] + 0xFFFFFFC0;


_040010D4:	// 40010d4: <18800002>	BLEZ      (R0<=0) --> 10e0
	sp_reg_pc = 0x040010D4;
	if ((_s32)state.r[A0] <= 0)
	{
		state.r[AT] = 0 + 0x02F0;
		goto _040010E0;
	}


_040010D8:	// 40010d8: <200102f0>	ADDI      AT = R0 + 02f0
	state.r[AT] = 0 + 0x02F0;


_040010DC:	// 40010dc: <20030040>	ADDI      V1 = R0 + 0040
	state.r[V1] = 0 + 0x0040;


_040010E0:	// 40010e0: <207e0000>	ADDI      S8 = V1 + 0000
	state.r[S8] = state.r[V1] + 0x0000;


_040010E4:	// 40010e4: <0d0006b5>	JAL	    04001ad4
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x040010E4;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_040010E8:	// 40010e8: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_040010EC:	// 40010ec: <00a00008>	JR        A1
	{
		_u32	temp = ((state.r[A1] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[SP] = 0 + 0x02F0;
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_040010F0:	// 40010f0: <201d02f0>	ADDI      SP = R0 + 02f0
	state.r[SP] = 0 + 0x02F0;


_040010F4:	// 40010f4: <40043800>	MFC0      A3 = SP status
	state.r[A0] = 0;


_040010F8:	// 40010f8: <1480fffe>	BNE       (R0!=A0) --> 10f4
	sp_reg_pc = 0x040010F8;
	if (state.r[A0] != 0)
	{
		goto _040010F4;
	}


_040010FC:	// 40010fc: <00000000>	NOP       


_04001100:	// 4001100: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001104:	// 4001104: <1480fffe>	BNE       (R0!=A0) --> 1100
	sp_reg_pc = 0x04001104;
	if (state.r[A0] != 0)
	{
		goto _04001100;
	}


_04001108:	// 4001108: <00000000>	NOP       


_0400110C:	// 400110c: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_04001110:	// 4001110: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001114:	// 4001114: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001114;
	DMARead(state.r[V1]);


_04001118:	// 4001118: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_0400111C:	// 400111c: <00000000>	NOP       


_04001120:	// 4001120: <40043800>	MFC0      A3 = SP status
	state.r[A0] = 0;


_04001124:	// 4001124: <1480fffe>	BNE       (R0!=A0) --> 1120
	sp_reg_pc = 0x04001124;
	if (state.r[A0] != 0)
	{
		goto _04001120;
	}


_04001128:	// 4001128: <00000000>	NOP       


_0400112C:	// 400112c: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001130:	// 4001130: <1480fffe>	BNE       (R0!=A0) --> 112c
	sp_reg_pc = 0x04001130;
	if (state.r[A0] != 0)
	{
		goto _0400112C;
	}


_04001134:	// 4001134: <00000000>	NOP       


_04001138:	// 4001138: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_0400113C:	// 400113c: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001140:	// 4001140: <40831800>	MTC0      SP write DMA length = V1
	DMAWrite(state.r[V1]);


_04001144:	// 4001144: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001148:	// 4001148: <00000000>	NOP       


_0400114C:	// 400114c: <3323ffff>	ANDI      V1 = T9 & ffff
	state.r[V1] = (state.r[T9] & 0x0000FFFF);


_04001150:	// 4001150: <1060ffce>	BEQ       (R0==V1) --> 108c
	sp_reg_pc = 0x04001150;
	if (state.r[V1] == 0)
	{
		state.r[V0] = (state.r[K0] & 0x0000FFFF);
		goto _0400108C;
	}


_04001154:	// 4001154: <3342ffff>	ANDI      V0 = K0 & ffff
	state.r[V0] = (state.r[K0] & 0x0000FFFF);


_04001158:	// 4001158: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_0400115C:	// 400115c: <2063fff0>	ADDI      V1 = V1 + fff0
	state.r[V1] = state.r[V1] + 0xFFFFFFF0;


_04001160:	// 4001160: <e8401800>	SDV       [V0 + 0x00] = vec00 <00>
	{
		_u32	addr = (0x00000000 + state.r[V0]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[0].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[0].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[0].U32[0x00000003];
		}
	}


_04001164:	// 4001164: <e8401801>	SDV       [V0 + 0x04] = vec00 <00>
	{
		_u32	addr = (0x00000008 + state.r[V0]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[0].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[0].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[0].U32[0x00000003];
		}
	}


_04001168:	// 4001168: <20420010>	ADDI      V0 = V0 + 0010
	state.r[V0] = state.r[V0] + 0x0010;


_0400116C:	// 400116c: <1c60fffc>	BGTZ      (R0>0) --> 1160
	sp_reg_pc = 0x0400116C;
	if((_s32)state.r[V1] > 0)
	{
		state.r[V1] = state.r[V1] + 0xFFFFFFF0;
		goto _04001160;
	}


_04001170:	// 4001170: <2063fff0>	ADDI      V1 = V1 + fff0
	state.r[V1] = state.r[V1] + 0xFFFFFFF0;


_04001174:	// 4001174: <09000423>	J         0400108c
	goto _0400108C;


_04001178:	// 4001178: <00000000>	NOP       


_0400117C:	// 400117c: <00191402>	SRL       V0 = T9 >> 16
	state.r[V0] = ((_u32)state.r[T9] >> 16);


_04001180:	// 4001180: <a71a0000>	SH        [T8+0000] = K0
	Save16_DMEM((_u16)state.r[K0], (state.r[T8] + 0x00000000) & 0xfff);


_04001184:	// 4001184: <a7020002>	SH        [T8+0002] = V0
	Save16_DMEM((_u16)state.r[V0], (state.r[T8] + 0x00000002) & 0xfff);


_04001188:	// 4001188: <09000423>	J         0400108c
	Save16_DMEM((_u16)state.r[T9], (state.r[T8] + 0x00000004) & 0xfff);
	goto _0400108C;


_0400118C:	// 400118c: <a7190004>	SH        [T8+0004] = T9
	Save16_DMEM((_u16)state.r[T9], (state.r[T8] + 0x00000004) & 0xfff);


_04001190:	// 4001190: <3344ffff>	ANDI      A0 = K0 & ffff
	state.r[A0] = (state.r[K0] & 0x0000FFFF);


_04001194:	// 4001194: <001a0b02>	SRL       AT = K0 >> 12
	state.r[AT] = ((_u32)state.r[K0] >> 12);


_04001198:	// 4001198: <30210ff0>	ANDI      AT = AT & 0ff0
	state.r[AT] = (state.r[AT] & 0x00000FF0);


_0400119C:	// 400119c: <3323ffff>	ANDI      V1 = T9 & ffff
	state.r[V1] = (state.r[T9] & 0x0000FFFF);


_040011A0:	// 40011a0: <00191402>	SRL       V0 = T9 >> 16
	state.r[V0] = ((_u32)state.r[T9] >> 16);


_040011A4:	// 40011a4: <c8411800>	LDV       vec01 <00> = [V0 + 0x00]

	// [start function: Zelda11A4_11D4]

	Zelda11A4_11D4();
	goto _040011D8;

_040011A8:	// 40011a8: <c8621800>	LDV       vec02 <00> = [V1 + 0x00]

_040011AC:	// 40011ac: <2021fff8>	ADDI      AT = AT + fff8

_040011B0:	// 40011b0: <20840010>	ADDI      A0 = A0 + 0010

_040011B4:	// 40011b4: <e8810878>	SSV       [A0 + 0x10] = vec01 <00>

_040011B8:	// 40011b8: <e881097a>	SSV       [A0 + 0x14] = vec01 <02>

_040011BC:	// 40011bc: <20420008>	ADDI      V0 = V0 + 0008

_040011C0:	// 40011c0: <e8810a7c>	SSV       [A0 + 0x18] = vec01 <04>

_040011C4:	// 40011c4: <e8810b7e>	SSV       [A0 + 0x1c] = vec01 <06>

_040011C8:	// 40011c8: <e8820879>	SSV       [A0 + 0x12] = vec02 <00>

_040011CC:	// 40011cc: <20630008>	ADDI      V1 = V1 + 0008

_040011D0:	// 40011d0: <e882097b>	SSV       [A0 + 0x16] = vec02 <02>

_040011D4:	// 40011d4: <e8820a7d>	SSV       [A0 + 0x1a] = vec02 <04>
	ErrorToCallFunction(0x040011D4);

	// [end of function: Zelda11A4_11D4]

_040011D8:	// 40011d8: <1c20fff2>	BGTZ      (R0>0) --> 11a4
	sp_reg_pc = 0x040011D8;
	if((_s32)state.r[AT] > 0)
	{
		Save16_DMEM(state.v[2].U16[4], (0xFFFFFFFE + state.r[A0]) & 0xfff);
		goto _040011A4;
	}


_040011DC:	// 40011dc: <e8820b7f>	SSV       [A0 + 0x1e] = vec02 <06>
	Save16_DMEM(state.v[2].U16[4], (0xFFFFFFFE + state.r[A0]) & 0xfff);


_040011E0:	// 40011e0: <09000423>	J         0400108c
	goto _0400108C;


_040011E4:	// 40011e4: <00000000>	NOP       


_040011E8:	// 40011e8: <3321ffff>	ANDI      AT = T9 & ffff
	state.r[AT] = (state.r[T9] & 0x0000FFFF);


_040011EC:	// 40011ec: <1020ffa7>	BEQ       (R0==AT) --> 108c
	sp_reg_pc = 0x040011EC;
	if (state.r[AT] == 0)
	{
		state.r[V0] = (state.r[K0] & 0x0000FFFF);
		goto _0400108C;
	}


_040011F0:	// 40011f0: <3342ffff>	ANDI      V0 = K0 & ffff
	state.r[V0] = (state.r[K0] & 0x0000FFFF);


_040011F4:	// 40011f4: <00191c02>	SRL       V1 = T9 >> 16
	state.r[V1] = ((_u32)state.r[T9] >> 16);


_040011F8:	// 40011f8: <c8411800>	LDV       vec01 <00> = [V0 + 0x00]

	// [start function: Zelda11F8_120C]

	Zelda11F8_120C();
	goto _04001210;

_040011FC:	// 40011fc: <c8421801>	LDV       vec02 <00> = [V0 + 0x08]

_04001200:	// 4001200: <2021fff0>	ADDI      AT = AT + fff0

_04001204:	// 4001204: <20420010>	ADDI      V0 = V0 + 0010

_04001208:	// 4001208: <e8611800>	SDV       [V1 + 0x00] = vec01 <00>

_0400120C:	// 400120c: <e8621801>	SDV       [V1 + 0x04] = vec02 <00>
	ErrorToCallFunction(0x0400120C);

	// [end of function: Zelda11F8_120C]

_04001210:	// 4001210: <1c20fff9>	BGTZ      (R0>0) --> 11f8
	sp_reg_pc = 0x04001210;
	if((_s32)state.r[AT] > 0)
	{
		state.r[V1] = state.r[V1] + 0x0010;
		goto _040011F8;
	}


_04001214:	// 4001214: <20630010>	ADDI      V1 = V1 + 0010
	state.r[V1] = state.r[V1] + 0x0010;


_04001218:	// 4001218: <09000423>	J         0400108c
	goto _0400108C;


_0400121C:	// 400121c: <00000000>	NOP       


_04001220:	// 4001220: <00190a00>	SLL       AT = T9 << 8
	state.r[AT] = ((_u32)state.r[T9] << 8);


_04001224:	// 4001224: <00010a02>	SRL       AT = AT >> 8
	state.r[AT] = ((_u32)state.r[AT] >> 8);


_04001228:	// 4001228: <09000423>	J         0400108c
	Save32_DMEM((_u32)state.r[AT], (state.r[T8] + 0x00000008) & 0xfff);
	goto _0400108C;


_0400122C:	// 400122c: <af010008>	SW        [T8+0008] = AT
	Save32_DMEM((_u32)state.r[AT], (state.r[T8] + 0x00000008) & 0xfff);


_04001230:	// 4001230: <c81f2000>	LQV       vec31 <00> = [R0 + 0x00]
	{
		_u32 addr = (0x00000000 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001234:	// 4001234: <4a1bdeec>	VXOR      vec27 = vec27 xor vec27[<none>]
	rsp_cop2_vxor(&state.v[27], &state.v[27], &state.v[27]);


_04001238:	// 4001238: <97150000>	LHU       S5 = [T8+0000]
	state.r[S5] = (_s32)(_u16)Load16_DMEM((state.r[T8] + 0x00000000) & 0xfff);


_0400123C:	// 400123c: <4a19ce6c>	VXOR      vec25 = vec25 xor vec25[<none>]
	rsp_cop2_vxor(&state.v[25], &state.v[25], &state.v[25]);


_04001240:	// 4001240: <4a18c62c>	VXOR      vec24 = vec24 xor vec24[<none>]
	rsp_cop2_vxor(&state.v[24], &state.v[24], &state.v[24]);


_04001244:	// 4001244: <22b40001>	ADDI      S4 = S5 + 0001
	state.r[S4] = state.r[S5] + 0x0001;


_04001248:	// 4001248: <97130002>	LHU       S3 = [T8+0002]
	state.r[S3] = (_s32)(_u16)Load16_DMEM((state.r[T8] + 0x00000002) & 0xfff);


_0400124C:	// 400124c: <4a0d6b6c>	VXOR      vec13 = vec13 xor vec13[<none>]
	rsp_cop2_vxor(&state.v[13], &state.v[13], &state.v[13]);


_04001250:	// 4001250: <4a0e73ac>	VXOR      vec14 = vec14 xor vec14[<none>]
	rsp_cop2_vxor(&state.v[14], &state.v[14], &state.v[14]);


_04001254:	// 4001254: <97120004>	LHU       S2 = [T8+0004]
	state.r[S2] = (_s32)(_u16)Load16_DMEM((state.r[T8] + 0x00000004) & 0xfff);


_04001258:	// 4001258: <4a0f7bec>	VXOR      vec15 = vec15 xor vec15[<none>]
	rsp_cop2_vxor(&state.v[15], &state.v[15], &state.v[15]);


_0400125C:	// 400125c: <4a10842c>	VXOR      vec16 = vec16 xor vec16[<none>]
	rsp_cop2_vxor(&state.v[16], &state.v[16], &state.v[16]);


_04001260:	// 4001260: <00198a00>	SLL       S1 = T9 << 8
	state.r[S1] = ((_u32)state.r[T9] << 8);


_04001264:	// 4001264: <4a118c6c>	VXOR      vec17 = vec17 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[17], &state.v[17]);


_04001268:	// 4001268: <4a1294ac>	VXOR      vec18 = vec18 xor vec18[<none>]
	rsp_cop2_vxor(&state.v[18], &state.v[18], &state.v[18]);


_0400126C:	// 400126c: <00118a02>	SRL       S1 = S1 >> 8
	state.r[S1] = ((_u32)state.r[S1] >> 8);


_04001270:	// 4001270: <4a139cec>	VXOR      vec19 = vec19 xor vec19[<none>]
	rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);


_04001274:	// 4001274: <ea7b2000>	SQV       [S3 + 0x00] = vec27 <00>
	{
		_u32    addr = (0x00000000 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[27].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[27].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[27].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[27].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[27].U64[0] & temp1);
			value2 |= (state.v[27].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001278:	// 4001278: <ea7b2001>	SQV       [S3 + 0x10] = vec27 <00>
	{
		_u32    addr = (0x00000010 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[27].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[27].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[27].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[27].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[27].U64[0] & temp1);
			value2 |= (state.v[27].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_0400127C:	// 400127c: <20100040>	ADDI      S0 = R0 + 0040
	state.r[S0] = 0 + 0x0040;


_04001280:	// 4001280: <200f0330>	ADDI      T7 = R0 + 0330
	state.r[T7] = 0 + 0x0330;


_04001284:	// 4001284: <001a0c02>	SRL       AT = K0 >> 16
	state.r[AT] = ((_u32)state.r[K0] >> 16);


_04001288:	// 4001288: <30280004>	ANDI      T0 = AT & 0004
	state.r[T0] = (state.r[AT] & 0x00000004);


_0400128C:	// 400128c: <1100000a>	BEQ       (R0==T0) --> 12b8
	sp_reg_pc = 0x0400128C;
	if (state.r[T0] == 0)
	{
		goto _040012B8;
	}


_04001290:	// 4001290: <00000000>	NOP       


_04001294:	// 4001294: <200a0005>	ADDI      T2 = R0 + 0005
	state.r[T2] = 0 + 0x0005;


_04001298:	// 4001298: <2009000e>	ADDI      T1 = R0 + 000e
	state.r[T1] = 0 + 0x000E;


_0400129C:	// 400129c: <ca191802>	LDV       vec25 <00> = [S0 + 0x10]
	rsp_ldv_compiler((state.r[S0] + 0x00000010) & 0xFFF, 25, 8);


_040012A0:	// 40012a0: <ca171803>	LDV       vec23 <00> = [S0 + 0x18]
	rsp_ldv_compiler((state.r[S0] + 0x00000018) & 0xFFF, 23, 8);


_040012A4:	// 40012a4: <2210ffff>	ADDI      S0 = S0 + ffff
	state.r[S0] = state.r[S0] + 0xFFFFFFFF;


_040012A8:	// 40012a8: <ca191c02>	LDV       vec25 <08> = [S0 + 0x10]
	rsp_ldv_compiler((state.r[S0] + 0x00000010) & 0xFFF, 25, 0);


_040012AC:	// 40012ac: <22100002>	ADDI      S0 = S0 + 0002
	state.r[S0] = state.r[S0] + 0x0002;


_040012B0:	// 40012b0: <090004b4>	J         040012d0
	rsp_ldv_compiler((state.r[S0] + 0x00000018) & 0xFFF, 23, 0);
	goto _040012D0;


_040012B4:	// 40012b4: <ca171c03>	LDV       vec23 <08> = [S0 + 0x18]
	rsp_ldv_compiler((state.r[S0] + 0x00000018) & 0xFFF, 23, 0);


_040012B8:	// 40012b8: <200a0009>	ADDI      T2 = R0 + 0009
	state.r[T2] = 0 + 0x0009;


_040012BC:	// 40012bc: <2009000c>	ADDI      T1 = R0 + 000c
	state.r[T1] = 0 + 0x000C;


_040012C0:	// 40012c0: <ca191800>	LDV       vec25 <00> = [S0 + 0x00]
	rsp_ldv_compiler((state.r[S0] & 0xFFF), 25, 8);


_040012C4:	// 40012c4: <ca181c00>	LDV       vec24 <08> = [S0 + 0x00]
	rsp_ldv_compiler((state.r[S0] & 0xFFF), 24, 0);


_040012C8:	// 40012c8: <ca171801>	LDV       vec23 <00> = [S0 + 0x08]
	rsp_ldv_compiler((state.r[S0] + 0x00000008) & 0xFFF, 23, 8);


_040012CC:	// 40012cc: <ca171c01>	LDV       vec23 <08> = [S0 + 0x08]
	rsp_ldv_compiler((state.r[S0] + 0x00000008) & 0xFFF, 23, 0);


_040012D0:	// 40012d0: <001a0c02>	SRL       AT = K0 >> 16
	state.r[AT] = ((_u32)state.r[K0] >> 16);


_040012D4:	// 40012d4: <30210001>	ANDI      AT = AT & 0001
	state.r[AT] = (state.r[AT] & 0x00000001);


_040012D8:	// 40012d8: <1c200008>	BGTZ      (R0>0) --> 12fc
	sp_reg_pc = 0x040012D8;
	if((_s32)state.r[AT] > 0)
	{
		state.r[AT] = ((_u32)state.r[K0] >> 16);
		goto _040012FC;
	}


_040012DC:	// 40012dc: <001a0c02>	SRL       AT = K0 >> 16
	state.r[AT] = ((_u32)state.r[K0] >> 16);


_040012E0:	// 40012e0: <30210002>	ANDI      AT = AT & 0002
	state.r[AT] = (state.r[AT] & 0x00000002);


_040012E4:	// 40012e4: <10010002>	BEQ       (AT==R0) --> 12f0
	sp_reg_pc = 0x040012E4;
	if (0 == state.r[AT])
	{
		state.r[V0] = state.r[S1] + 0x0000;
		goto _040012F0;
	}


_040012E8:	// 40012e8: <22220000>	ADDI      V0 = S1 + 0000
	state.r[V0] = state.r[S1] + 0x0000;


_040012EC:	// 40012ec: <8f020008>	LW        V0 = [T8+0008]
	state.r[V0] = (_s32)Load32_DMEM((state.r[T8] + 0x00000008) & 0xfff);


_040012F0:	// 40012f0: <22610000>	ADDI      AT = S3 + 0000
	state.r[AT] = state.r[S3] + 0x0000;


_040012F4:	// 40012f4: <0d0006b5>	JAL	    04001ad4
	state.r[V1] = 0 + 0x001F;
	sp_reg_pc = 0x040012F4;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_040012F8:	// 40012f8: <2003001f>	ADDI      V1 = R0 + 001f
	state.r[V1] = 0 + 0x001F;


_040012FC:	// 40012fc: <ca7b2001>	LQV       vec27 <00> = [S3 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_04001300:	// 4001300: <22730020>	ADDI      S3 = S3 + 0020
	state.r[S3] = state.r[S3] + 0x0020;


_04001304:	// 4001304: <12400077>	BEQ       (R0==S2) --> 14e4
	sp_reg_pc = 0x04001304;
	if (state.r[S2] == 0)
	{
		rsp_ldv_compiler((state.r[S4] & 0xFFF), 1, 8);
		goto _040014E4;
	}


_04001308:	// 4001308: <ca811800>	LDV       vec01 <00> = [S4 + 0x00]
	rsp_ldv_compiler((state.r[S4] & 0xFFF), 1, 8);


_0400130C:	// 400130c: <92a10000>	LBU       AT = [S5+0000]
	state.r[AT] = (_s32)(_u8)pDMEM[((state.r[S5] + 0x00000000) & 0xfff) ^ 3];


_04001310:	// 4001310: <302b000f>	ANDI      T3 = AT & 000f
	state.r[T3] = (state.r[AT] & 0x0000000F);


_04001314:	// 4001314: <000b5940>	SLL       T3 = T3 << 5
	state.r[T3] = ((_u32)state.r[T3] << 5);


_04001318:	// 4001318: <4b01c8e8>	VAND      vec03 = vec25 and vec01[0]
	rsp_cop2_vand_element(&state.v[3], &state.v[25], &state.v[1].U16[7]);


_0400131C:	// 400131c: <016f6820>	ADD       T5 = T3+T7
	state.r[T5] = (state.r[T3] + state.r[T7]);


_04001320:	// 4001320: <4a04212c>	VXOR      vec04 = vec04 xor vec04[<none>]
	rsp_cop2_vxor(&state.v[4], &state.v[4], &state.v[4]);


_04001324:	// 4001324: <4a0631ac>	VXOR      vec06 = vec06 xor vec06[<none>]
	rsp_cop2_vxor(&state.v[6], &state.v[6], &state.v[6]);


_04001328:	// 4001328: <15000004>	BNE       (R0!=T0) --> 133c
	sp_reg_pc = 0x04001328;
	if (state.r[T0] != 0)
	{
		rsp_cop2_vand_element(&state.v[5], &state.v[25], &state.v[1].U16[6]);
		goto _0400133C;
	}


_0400132C:	// 400132c: <4b21c968>	VAND      vec05 = vec25 and vec01[1]
	rsp_cop2_vand_element(&state.v[5], &state.v[25], &state.v[1].U16[6]);


_04001330:	// 4001330: <4b21c128>	VAND      vec04 = vec24 and vec01[1]
	rsp_cop2_vand_element(&state.v[4], &state.v[24], &state.v[1].U16[6]);


_04001334:	// 4001334: <4b41c968>	VAND      vec05 = vec25 and vec01[2]
	rsp_cop2_vand_element(&state.v[5], &state.v[25], &state.v[1].U16[5]);


_04001338:	// 4001338: <4b61c1a8>	VAND      vec06 = vec24 and vec01[3]
	rsp_cop2_vand_element(&state.v[6], &state.v[24], &state.v[1].U16[4]);


_0400133C:	// 400133c: <00017102>	SRL       T6 = AT >> 4
	state.r[T6] = ((_u32)state.r[AT] >> 4);


_04001340:	// 4001340: <00091020>	ADD       V0 = R0+T1
	state.r[V0] = (0 + state.r[T1]);


_04001344:	// 4001344: <004e7022>	SUB       T6 = V0-T6
	state.r[T6] = (state.r[V0] - state.r[T6]);


_04001348:	// 4001348: <21c2ffff>	ADDI      V0 = T6 + ffff
	state.r[V0] = state.r[T6] + 0xFFFFFFFF;


_0400134C:	// 400134c: <34038000>	ORI       V1 = R0 | 0x8000
	state.r[V1] = (0 | 0x00008000);


_04001350:	// 4001350: <11c00002>	BEQ       (R0==T6) --> 135c
	sp_reg_pc = 0x04001350;
	if (state.r[T6] == 0)
	{
		state.r[A0] = 0 + 0xFFFFFFFF;
		goto _0400135C;
	}


_04001354:	// 4001354: <2004ffff>	ADDI      A0 = R0 + ffff
	state.r[A0] = 0 + 0xFFFFFFFF;


_04001358:	// 4001358: <00432006>	SRLV      V1 = A0 >> V0
	state.r[A0] = ((_u32)state.r[V1] >> (state.r[V0] & 0x1F));


_0400135C:	// 400135c: <4884b000>	MTC2      vec22 <00> = A0
	state.v[22].U16[7] = (_u16)state.r[A0];


_04001360:	// 4001360: <c9b52000>	LQV       vec21 <00> = [T5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[21].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[21].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[21].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[21].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[21].U64[0] = state.v[21].U64[0] & ~temp1;
			state.v[21].U64[1] = state.v[21].U64[1] & ~temp2;
			state.v[21].U64[0] = state.v[21].U64[0] | value1;
			state.v[21].U64[1] = state.v[21].U64[1] | value2;
		}
	}


_04001364:	// 4001364: <c9b42001>	LQV       vec20 <00> = [T5 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[20].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[20].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[20].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[20].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[20].U64[0] = state.v[20].U64[0] & ~temp1;
			state.v[20].U64[1] = state.v[20].U64[1] & ~temp2;
			state.v[20].U64[0] = state.v[20].U64[0] | value1;
			state.v[20].U64[1] = state.v[20].U64[1] | value2;
		}
	}


_04001368:	// 4001368: <21adfffe>	ADDI      T5 = T5 + fffe
	state.r[T5] = state.r[T5] + 0xFFFFFFFE;


_0400136C:	// 400136c: <c9b32802>	LRV       vec19 <00> = [T5 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[19].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001370:	// 4001370: <21adfffe>	ADDI      T5 = T5 + fffe
	state.r[T5] = state.r[T5] + 0xFFFFFFFE;


_04001374:	// 4001374: <c9b22802>	LRV       vec18 <00> = [T5 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[18].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001378:	// 4001378: <21adfffe>	ADDI      T5 = T5 + fffe
	state.r[T5] = state.r[T5] + 0xFFFFFFFE;


_0400137C:	// 400137c: <c9b12802>	LRV       vec17 <00> = [T5 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[17].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001380:	// 4001380: <21adfffe>	ADDI      T5 = T5 + fffe
	state.r[T5] = state.r[T5] + 0xFFFFFFFE;


_04001384:	// 4001384: <c9b02802>	LRV       vec16 <00> = [T5 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[16].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001388:	// 4001388: <21adfffe>	ADDI      T5 = T5 + fffe
	state.r[T5] = state.r[T5] + 0xFFFFFFFE;


_0400138C:	// 400138c: <c9af2802>	LRV       vec15 <00> = [T5 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[15].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001390:	// 4001390: <21adfffe>	ADDI      T5 = T5 + fffe
	state.r[T5] = state.r[T5] + 0xFFFFFFFE;


_04001394:	// 4001394: <c9ae2802>	LRV       vec14 <00> = [T5 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[14].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001398:	// 4001398: <21adfffe>	ADDI      T5 = T5 + fffe
	state.r[T5] = state.r[T5] + 0xFFFFFFFE;


_0400139C:	// 400139c: <c9ad2802>	LRV       vec13 <00> = [T5 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[13].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040013A0:	// 40013a0: <028aa020>	ADD       S4 = S4+T2
	state.r[S4] = (state.r[S4] + state.r[T2]);


_040013A4:	// 40013a4: <4a171f86>	VMUDN     vec30 = ( acc = vec03 * vec23[<none>]      ) >> 16

	// [start function: Zelda13A4_14D8]

	Zelda13A4_14D8();
	goto _040014DC;

_040013A8:	// 40013a8: <02aaa820>	ADD       S5 = S5+T2

_040013AC:	// 40013ac: <4a17278e>	VMADN     vec30 = ( acc+= vec04 * vec23[<none>] )

_040013B0:	// 40013b0: <ca811800>	LDV       vec01 <00> = [S4 + 0x00]

_040013B4:	// 40013b4: <4a172f46>	VMUDN     vec29 = ( acc = vec05 * vec23[<none>]      ) >> 16

_040013B8:	// 40013b8: <92a10000>	LBU       AT = [S5+0000]

_040013BC:	// 40013bc: <4a17374e>	VMADN     vec29 = ( acc+= vec06 * vec23[<none>] )

_040013C0:	// 40013c0: <19c00003>	BLEZ      (R0<=0) --> 13d0

_040013C4:	// 40013c4: <302b000f>	ANDI      T3 = AT & 000f

_040013C8:	// 40013c8: <4b16f785>	VMUDM     vec30 = ( acc = vec30 * vec22[0] ) >> 16

_040013CC:	// 40013cc: <4b16ef45>	VMUDM     vec29 = ( acc = vec29 * vec22[0] ) >> 16

_040013D0:	// 40013d0: <000b5940>	SLL       T3 = T3 << 5

_040013D4:	// 40013d4: <4b01c8e8>	VAND      vec03 = vec25 and vec01[0]

_040013D8:	// 40013d8: <016f6820>	ADD       T5 = T3+T7

_040013DC:	// 40013dc: <15000004>	BNE       (R0!=T0) --> 13f0

_040013E0:	// 40013e0: <4b21c968>	VAND      vec05 = vec25 and vec01[1]

_040013E4:	// 40013e4: <4b21c128>	VAND      vec04 = vec24 and vec01[1]

_040013E8:	// 40013e8: <4b41c968>	VAND      vec05 = vec25 and vec01[2]

_040013EC:	// 40013ec: <4b61c1a8>	VAND      vec06 = vec24 and vec01[3]

_040013F0:	// 40013f0: <00017102>	SRL       T6 = AT >> 4

_040013F4:	// 40013f4: <4bdba887>	VMUDH     vec02 = ( acc = (vec21 * vec27[6]) << 16) >> 16

_040013F8:	// 40013f8: <00091020>	ADD       V0 = R0+T1

_040013FC:	// 40013fc: <4bfba08f>	VMADH     vec02 = ( acc+= (vec20 * vec27[7]) << 16) >> 16

_04001400:	// 4001400: <004e7022>	SUB       T6 = V0-T6

_04001404:	// 4001404: <4b1e988f>	VMADH     vec02 = ( acc+= (vec19 * vec30[0]) << 16) >> 16

_04001408:	// 4001408: <21c2ffff>	ADDI      V0 = T6 + ffff

_0400140C:	// 400140c: <4b3e908f>	VMADH     vec02 = ( acc+= (vec18 * vec30[1]) << 16) >> 16

_04001410:	// 4001410: <20030001>	ADDI      V1 = R0 + 0001

_04001414:	// 4001414: <4b5e888f>	VMADH     vec02 = ( acc+= (vec17 * vec30[2]) << 16) >> 16

_04001418:	// 4001418: <00031bc0>	SLL       V1 = V1 << 15

_0400141C:	// 400141c: <4b7e808f>	VMADH     vec02 = ( acc+= (vec16 * vec30[3]) << 16) >> 16

_04001420:	// 4001420: <11c00002>	BEQ       (R0==T6) --> 142c

_04001424:	// 4001424: <2004ffff>	ADDI      A0 = R0 + ffff

_04001428:	// 4001428: <00432006>	SRLV      V1 = A0 >> V0

_0400142C:	// 400142c: <4b9e7f0f>	VMADH     vec28 = ( acc+= (vec15 * vec30[4]) << 16) >> 16

_04001430:	// 4001430: <4884b000>	MTC2      vec22 <00> = A0

_04001434:	// 4001434: <4bbe708f>	VMADH     vec02 = ( acc+= (vec14 * vec30[5]) << 16) >> 16

_04001438:	// 4001438: <4bde688f>	VMADH     vec02 = ( acc+= (vec13 * vec30[6]) << 16) >> 16

_0400143C:	// 400143c: <4bbff08f>	VMADH     vec02 = ( acc+= (vec30 * vec31[5]) << 16) >> 16

_04001440:	// 4001440: <4b3c3e9d>	VSAW $v26, $v7, $v28[[1]]

_04001444:	// 4001444: <4b1c3f1d>	VSAW $v28, $v7, $v28[[0]]

_04001448:	// 4001448: <4b9fd086>	VMUDN     vec02 = ( acc = vec26 * vec31[4]      ) >> 16

_0400144C:	// 400144c: <4b9fe70f>	VMADH     vec28 = ( acc+= (vec28 * vec31[4]) << 16) >> 16

_04001450:	// 4001450: <4b1d9887>	VMUDH     vec02 = ( acc = (vec19 * vec29[0]) << 16) >> 16

_04001454:	// 4001454: <21acfffe>	ADDI      T4 = T5 + fffe

_04001458:	// 4001458: <4b3d908f>	VMADH     vec02 = ( acc+= (vec18 * vec29[1]) << 16) >> 16

_0400145C:	// 400145c: <c9932802>	LRV       vec19 <00> = [T4 + 0x20]

_04001460:	// 4001460: <4b5d888f>	VMADH     vec02 = ( acc+= (vec17 * vec29[2]) << 16) >> 16

_04001464:	// 4001464: <218cfffe>	ADDI      T4 = T4 + fffe

_04001468:	// 4001468: <4b7d808f>	VMADH     vec02 = ( acc+= (vec16 * vec29[3]) << 16) >> 16

_0400146C:	// 400146c: <c9922802>	LRV       vec18 <00> = [T4 + 0x20]

_04001470:	// 4001470: <4b9d788f>	VMADH     vec02 = ( acc+= (vec15 * vec29[4]) << 16) >> 16

_04001474:	// 4001474: <218cfffe>	ADDI      T4 = T4 + fffe

_04001478:	// 4001478: <4bbd708f>	VMADH     vec02 = ( acc+= (vec14 * vec29[5]) << 16) >> 16

_0400147C:	// 400147c: <c9912802>	LRV       vec17 <00> = [T4 + 0x20]

_04001480:	// 4001480: <4bdd688f>	VMADH     vec02 = ( acc+= (vec13 * vec29[6]) << 16) >> 16

_04001484:	// 4001484: <218cfffe>	ADDI      T4 = T4 + fffe

_04001488:	// 4001488: <4bbfe88f>	VMADH     vec02 = ( acc+= (vec29 * vec31[5]) << 16) >> 16

_0400148C:	// 400148c: <c9902802>	LRV       vec16 <00> = [T4 + 0x20]

_04001490:	// 4001490: <4bdca88f>	VMADH     vec02 = ( acc+= (vec21 * vec28[6]) << 16) >> 16

_04001494:	// 4001494: <218cfffe>	ADDI      T4 = T4 + fffe

_04001498:	// 4001498: <4bfca08f>	VMADH     vec02 = ( acc+= (vec20 * vec28[7]) << 16) >> 16

_0400149C:	// 400149c: <c98f2802>	LRV       vec15 <00> = [T4 + 0x20]

_040014A0:	// 40014a0: <4b3b3e9d>	VSAW $v26, $v7, $v27[[1]]

_040014A4:	// 40014a4: <218cfffe>	ADDI      T4 = T4 + fffe

_040014A8:	// 40014a8: <4b1b3edd>	VSAW $v27, $v7, $v27[[0]]

_040014AC:	// 40014ac: <c98e2802>	LRV       vec14 <00> = [T4 + 0x20]

_040014B0:	// 40014b0: <218cfffe>	ADDI      T4 = T4 + fffe

_040014B4:	// 40014b4: <c98d2802>	LRV       vec13 <00> = [T4 + 0x20]

_040014B8:	// 40014b8: <c9b52000>	LQV       vec21 <00> = [T5 + 0x00]

_040014BC:	// 40014bc: <4b9fd086>	VMUDN     vec02 = ( acc = vec26 * vec31[4]      ) >> 16

_040014C0:	// 40014c0: <c9b42001>	LQV       vec20 <00> = [T5 + 0x10]

_040014C4:	// 40014c4: <4b9fdecf>	VMADH     vec27 = ( acc+= (vec27 * vec31[4]) << 16) >> 16

_040014C8:	// 40014c8: <2252ffe0>	ADDI      S2 = S2 + ffe0

_040014CC:	// 40014cc: <ea7c1800>	SDV       [S3 + 0x00] = vec28 <00>

_040014D0:	// 40014d0: <ea7c1c01>	SDV       [S3 + 0x04] = vec28 <08>

_040014D4:	// 40014d4: <ea7b1802>	SDV       [S3 + 0x08] = vec27 <00>

_040014D8:	// 40014d8: <ea7b1c03>	SDV       [S3 + 0x0c] = vec27 <08>
	ErrorToCallFunction(0x040014D8);

	// [end of function: Zelda13A4_14D8]

_040014DC:	// 40014dc: <1e40ffb0>	BGTZ      (R0>0) --> 13a0
	sp_reg_pc = 0x040014DC;
	if((_s32)state.r[S2] > 0)
	{
		state.r[S3] = state.r[S3] + 0x0020;
		goto _040013A0;
	}


_040014E0:	// 40014e0: <22730020>	ADDI      S3 = S3 + 0020
	state.r[S3] = state.r[S3] + 0x0020;


_040014E4:	// 40014e4: <2261ffe0>	ADDI      AT = S3 + ffe0
	state.r[AT] = state.r[S3] + 0xFFFFFFE0;


_040014E8:	// 40014e8: <22220000>	ADDI      V0 = S1 + 0000
	state.r[V0] = state.r[S1] + 0x0000;


_040014EC:	// 40014ec: <090006b1>	J         04001ac4
	state.r[V1] = 0 + 0x0020;
	goto _04001AC4;


_040014F0:	// 40014f0: <20030020>	ADDI      V1 = R0 + 0020
	state.r[V1] = 0 + 0x0020;


_040014F4:	// 40014f4: <87080000>	LH        T0 = [T8+0000]
	state.r[T0] = (_s32)(_s16)Load16_DMEM((state.r[T8] + 0x00000000) & 0xfff);


_040014F8:	// 40014f8: <87130002>	LH        S3 = [T8+0002]
	state.r[S3] = (_s32)(_s16)Load16_DMEM((state.r[T8] + 0x00000002) & 0xfff);


_040014FC:	// 40014fc: <87120004>	LH        S2 = [T8+0004]
	state.r[S2] = (_s32)(_s16)Load16_DMEM((state.r[T8] + 0x00000004) & 0xfff);


_04001500:	// 4001500: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001504:	// 4001504: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001508:	// 4001508: <22e10000>	ADDI      AT = S7 + 0000
	state.r[AT] = state.r[S7] + 0x0000;


_0400150C:	// 400150c: <0002b020>	ADD       S6 = R0+V0
	state.r[S6] = (0 + state.r[V0]);


_04001510:	// 4001510: <2003001f>	ADDI      V1 = R0 + 001f
	state.r[V1] = 0 + 0x001F;


_04001514:	// 4001514: <001a3c02>	SRL       A3 = K0 >> 16
	state.r[A3] = ((_u32)state.r[K0] >> 16);


_04001518:	// 4001518: <30ea0001>	ANDI      T2 = A3 & 0001
	state.r[T2] = (state.r[A3] & 0x00000001);


_0400151C:	// 400151c: <1d400005>	BGTZ      (R0>0) --> 1534
	sp_reg_pc = 0x0400151C;
	if((_s32)state.r[T2] > 0)
	{
		goto _04001534;
	}


_04001520:	// 4001520: <00000000>	NOP       


_04001524:	// 4001524: <0d0006b5>	JAL	    04001ad4
	sp_reg_pc = 0x04001524;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_04001528:	// 4001528: <00000000>	NOP       


_0400152C:	// 400152c: <09000550>	J         04001540
	goto _04001540;


_04001530:	// 4001530: <00000000>	NOP       


_04001534:	// 4001534: <a6e00008>	SH        [S7+0008] = R0
	Save16_DMEM((_u16)0, (state.r[S7] + 0x00000008) & 0xfff);


_04001538:	// 4001538: <4a10842c>	VXOR      vec16 = vec16 xor vec16[<none>]
	rsp_cop2_vxor(&state.v[16], &state.v[16], &state.v[16]);


_0400153C:	// 400153c: <eaf01800>	SDV       [S7 + 0x00] = vec16 <00>
	{
		_u32	addr = (0x00000000 + state.r[S7]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[16].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[16].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[16].U32[0x00000003];
		}
	}


_04001540:	// 4001540: <30ea0002>	ANDI      T2 = A3 & 0002
	state.r[T2] = (state.r[A3] & 0x00000002);


_04001544:	// 4001544: <11400006>	BEQ       (R0==T2) --> 1560
	sp_reg_pc = 0x04001544;
	if (state.r[T2] == 0)
	{
		rsp_ldv_compiler((state.r[S7] & 0xFFF), 16, 8);
		goto _04001560;
	}


_04001548:	// 4001548: <caf01800>	LDV       vec16 <00> = [S7 + 0x00]
	rsp_ldv_compiler((state.r[S7] & 0xFFF), 16, 8);


_0400154C:	// 400154c: <2108fffc>	ADDI      T0 = T0 + fffc
	state.r[T0] = state.r[T0] + 0xFFFFFFFC;


_04001550:	// 4001550: <e9100800>	SSV       [T0 + 0x00] = vec16 <00>
	Save16_DMEM(state.v[16].U16[7], (0x00000000 + state.r[T0]) & 0xfff);


_04001554:	// 4001554: <e9100a01>	SSV       [T0 + 0x02] = vec16 <04>
	Save16_DMEM(state.v[16].U16[5], (0x00000002 + state.r[T0]) & 0xfff);


_04001558:	// 4001558: <09000568>	J         040015a0
	goto _040015A0;


_0400155C:	// 400155c: <00000000>	NOP       


_04001560:	// 4001560: <30ea0004>	ANDI      T2 = A3 & 0004
	state.r[T2] = (state.r[A3] & 0x00000004);


_04001564:	// 4001564: <1140000c>	BEQ       (R0==T2) --> 1598
	sp_reg_pc = 0x04001564;
	if (state.r[T2] == 0)
	{
		goto _04001598;
	}


_04001568:	// 4001568: <00000000>	NOP       


_0400156C:	// 400156c: <2108fff0>	ADDI      T0 = T0 + fff0
	state.r[T0] = state.r[T0] + 0xFFFFFFF0;


_04001570:	// 4001570: <e9100800>	SSV       [T0 + 0x00] = vec16 <00>
	Save16_DMEM(state.v[16].U16[7], (0x00000000 + state.r[T0]) & 0xfff);


_04001574:	// 4001574: <e9100801>	SSV       [T0 + 0x02] = vec16 <00>
	Save16_DMEM(state.v[16].U16[7], (0x00000002 + state.r[T0]) & 0xfff);


_04001578:	// 4001578: <e9100902>	SSV       [T0 + 0x04] = vec16 <02>
	Save16_DMEM(state.v[16].U16[6], (0x00000004 + state.r[T0]) & 0xfff);


_0400157C:	// 400157c: <e9100903>	SSV       [T0 + 0x06] = vec16 <02>
	Save16_DMEM(state.v[16].U16[6], (0x00000006 + state.r[T0]) & 0xfff);


_04001580:	// 4001580: <e9100a04>	SSV       [T0 + 0x08] = vec16 <04>
	Save16_DMEM(state.v[16].U16[5], (0x00000008 + state.r[T0]) & 0xfff);


_04001584:	// 4001584: <e9100a05>	SSV       [T0 + 0x0a] = vec16 <04>
	Save16_DMEM(state.v[16].U16[5], (0x0000000A + state.r[T0]) & 0xfff);


_04001588:	// 4001588: <e9100b06>	SSV       [T0 + 0x0c] = vec16 <06>
	Save16_DMEM(state.v[16].U16[4], (0x0000000C + state.r[T0]) & 0xfff);


_0400158C:	// 400158c: <e9100b07>	SSV       [T0 + 0x0e] = vec16 <06>
	Save16_DMEM(state.v[16].U16[4], (0x0000000E + state.r[T0]) & 0xfff);


_04001590:	// 4001590: <09000568>	J         040015a0
	goto _040015A0;


_04001594:	// 4001594: <00000000>	NOP       


_04001598:	// 4001598: <2108fff8>	ADDI      T0 = T0 + fff8
	state.r[T0] = state.r[T0] + 0xFFFFFFF8;


_0400159C:	// 400159c: <e9101800>	SDV       [T0 + 0x00] = vec16 <00>
	{
		_u32	addr = (0x00000000 + state.r[T0]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[16].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[16].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[16].U32[0x00000003];
		}
	}


_040015A0:	// 40015a0: <caf70f04>	LSV       vec23 <14> = [S7 + 0x08]
	state.v[23].U16[0] = Load16_DMEM(0x00000008 + state.r[S7]);


_040015A4:	// 40015a4: <c9101800>	LDV       vec16 <00> = [T0 + 0x00]
	rsp_ldv_compiler((state.r[T0] & 0xFFF), 16, 8);


_040015A8:	// 40015a8: <48889200>	MTC2      vec18 <04> = T0
	state.v[18].U16[5] = (_u16)state.r[T0];


_040015AC:	// 40015ac: <200a00e0>	ADDI      T2 = R0 + 00e0
	state.r[T2] = 0 + 0x00E0;


_040015B0:	// 40015b0: <488a9300>	MTC2      vec18 <06> = T2
	state.v[18].U16[4] = (_u16)state.r[T2];


_040015B4:	// 40015b4: <489a9400>	MTC2      vec18 <08> = K0
	state.v[18].U16[3] = (_u16)state.r[K0];


_040015B8:	// 40015b8: <200a0040>	ADDI      T2 = R0 + 0040
	state.r[T2] = 0 + 0x0040;


_040015BC:	// 40015bc: <488a9500>	MTC2      vec18 <10> = T2
	state.v[18].U16[2] = (_u16)state.r[T2];


_040015C0:	// 40015c0: <20090060>	ADDI      T1 = R0 + 0060
	state.r[T1] = 0 + 0x0060;


_040015C4:	// 40015c4: <c93f2001>	LQV       vec31 <00> = [T1 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_040015C8:	// 40015c8: <c9392000>	LQV       vec25 <00> = [T1 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[25].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[25].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[25].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[25].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[25].U64[0] = state.v[25].U64[0] & ~temp1;
			state.v[25].U64[1] = state.v[25].U64[1] & ~temp2;
			state.v[25].U64[0] = state.v[25].U64[0] | value1;
			state.v[25].U64[1] = state.v[25].U64[1] | value2;
		}
	}


_040015CC:	// 40015cc: <4a1fce51>	VSUB      vec25 = vec25 - vec31[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[25].U16[0], state.v[25].U16[0], state.v[31].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[25].U16[1], state.v[25].U16[1], state.v[31].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[25].U16[2], state.v[25].U16[2], state.v[31].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[25].U16[3], state.v[25].U16[3], state.v[31].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[25].U16[4], state.v[25].U16[4], state.v[31].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[25].U16[5], state.v[25].U16[5], state.v[31].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[25].U16[6], state.v[25].U16[6], state.v[31].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[25].U16[7], state.v[25].U16[7], state.v[31].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040015D0:	// 40015d0: <c93e2002>	LQV       vec30 <00> = [T1 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[30].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[30].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[30].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[30].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[30].U64[0] = state.v[30].U64[0] & ~temp1;
			state.v[30].U64[1] = state.v[30].U64[1] & ~temp2;
			state.v[30].U64[0] = state.v[30].U64[0] | value1;
			state.v[30].U64[1] = state.v[30].U64[1] | value2;
		}
	}


_040015D4:	// 40015d4: <c93d2003>	LQV       vec29 <00> = [T1 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[29].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[29].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[29].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[29].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[29].U64[0] = state.v[29].U64[0] & ~temp1;
			state.v[29].U64[1] = state.v[29].U64[1] & ~temp2;
			state.v[29].U64[0] = state.v[29].U64[0] | value1;
			state.v[29].U64[1] = state.v[29].U64[1] | value2;
		}
	}


_040015D8:	// 40015d8: <c93c2004>	LQV       vec28 <00> = [T1 + 0x40]
	{
		_u32 addr = (0x00000040 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[28].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[28].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[28].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[28].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[28].U64[0] = state.v[28].U64[0] & ~temp1;
			state.v[28].U64[1] = state.v[28].U64[1] & ~temp2;
			state.v[28].U64[0] = state.v[28].U64[0] | value1;
			state.v[28].U64[1] = state.v[28].U64[1] | value2;
		}
	}


_040015DC:	// 40015dc: <c93b2005>	LQV       vec27 <00> = [T1 + 0x50]
	{
		_u32 addr = (0x00000050 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_040015E0:	// 40015e0: <c93a2006>	LQV       vec26 <00> = [T1 + 0x60]
	{
		_u32 addr = (0x00000060 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[26].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[26].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[26].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[26].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[26].U64[0] = state.v[26].U64[0] & ~temp1;
			state.v[26].U64[1] = state.v[26].U64[1] & ~temp2;
			state.v[26].U64[0] = state.v[26].U64[0] | value1;
			state.v[26].U64[1] = state.v[26].U64[1] | value2;
		}
	}


_040015E4:	// 40015e4: <4a1fce51>	VSUB      vec25 = vec25 - vec31[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[25].U16[0], state.v[25].U16[0], state.v[31].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[25].U16[1], state.v[25].U16[1], state.v[31].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[25].U16[2], state.v[25].U16[2], state.v[31].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[25].U16[3], state.v[25].U16[3], state.v[31].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[25].U16[4], state.v[25].U16[4], state.v[31].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[25].U16[5], state.v[25].U16[5], state.v[31].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[25].U16[6], state.v[25].U16[6], state.v[31].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[25].U16[7], state.v[25].U16[7], state.v[31].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040015E8:	// 40015e8: <c9382007>	LQV       vec24 <00> = [T1 + 0x70]
	{
		_u32 addr = (0x00000070 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[24].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[24].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[24].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[24].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[24].U64[0] = state.v[24].U64[0] & ~temp1;
			state.v[24].U64[1] = state.v[24].U64[1] & ~temp2;
			state.v[24].U64[0] = state.v[24].U64[0] | value1;
			state.v[24].U64[1] = state.v[24].U64[1] | value2;
		}
	}


_040015EC:	// 40015ec: <22f50020>	ADDI      S5 = S7 + 0020
	state.r[S5] = state.r[S7] + 0x0020;


_040015F0:	// 40015f0: <22f40030>	ADDI      S4 = S7 + 0030
	state.r[S4] = state.r[S7] + 0x0030;


_040015F4:	// 40015f4: <4a16b5ac>	VXOR      vec22 = vec22 xor vec22[<none>]
	rsp_cop2_vxor(&state.v[22], &state.v[22], &state.v[22]);


_040015F8:	// 40015f8: <4bf7fdc5>	VMUDM     vec23 = ( acc = vec31 * vec23[7] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[31*16].U16[0], 0xE4
			MOVQ mm1, state.v[31*16].U16[4*2]
			PSHUFW mm4, state.v[23*16].U16[0*2], 0
			PSHUFW mm6, mm0, 0xE4
			MOVQ mm7, mm1
			PMULLW mm6, mm4
			PMULLW mm7, mm4
			MOVQ accumulator[0*4], mm6
			MOVQ accumulator[2*4], mm7
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_040015FC:	// 40015fc: <4b92cd8d>	VMADM     vec22 = ( acc+= vec25 * vec18[4] ) >> 16
	{
		_u16		s2value = state.v[18].U16[3];
		VMADM_operation(0, state.v[22].U16[0], state.v[25].U16[0], s2value, 1, 1, 1, 1)
		VMADM_operation(1, state.v[22].U16[1], state.v[25].U16[1], s2value, 1, 1, 1, 1)
		VMADM_operation(2, state.v[22].U16[2], state.v[25].U16[2], s2value, 1, 1, 1, 1)
		VMADM_operation(3, state.v[22].U16[3], state.v[25].U16[3], s2value, 1, 1, 1, 1)
		VMADM_operation(4, state.v[22].U16[4], state.v[25].U16[4], s2value, 1, 1, 1, 1)
		VMADM_operation(5, state.v[22].U16[5], state.v[25].U16[5], s2value, 1, 1, 1, 1)
		VMADM_operation(6, state.v[22].U16[6], state.v[25].U16[6], s2value, 1, 1, 1, 1)
		VMADM_operation(7, state.v[22].U16[7], state.v[25].U16[7], s2value, 1, 1, 1, 1)
	}


_04001600:	// 4001600: <4b1efdce>	VMADN     vec23 = ( acc+= vec31 * vec30[0] )
	{
		_u16		s2value = state.v[30].U16[7];
		VMADN_operation(0, state.v[23].U16[0], state.v[31].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[23].U16[1], state.v[31].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[23].U16[2], state.v[31].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[23].U16[3], state.v[31].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[23].U16[4], state.v[31].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[23].U16[5], state.v[31].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[23].U16[6], state.v[31].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[23].U16[7], state.v[31].U16[7], s2value, 1, 0, 0, 1)
	}


_04001604:	// 4001604: <4b52fd46>	VMUDN     vec21 = ( acc = vec31 * vec18[2]      ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[31*16].U16[0], 0xE4
			MOVQ mm1, state.v[31*16].U16[4*2]
			PSHUFW mm4, state.v[18*16].U16[5*2], 0
			PSHUFW mm6, mm0, 0xE4
			MOVQ mm7, mm1
			PMULLW mm6, mm4
			PMULLW mm7, mm4
			MOVQ accumulator[0*4], mm6
			MOVQ accumulator[2*4], mm7
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm2, mm4
			PMULHUW mm3, mm4
			PSRAW mm0, 15
			PSRAW mm1, 15
			PMULLW mm0, mm4
			PMULLW mm1, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_04001608:	// 4001608: <4b5eb54e>	VMADN     vec21 = ( acc+= vec22 * vec30[2] )
	{
		_u16		s2value = state.v[30].U16[5];
		VMADN_operation(0, state.v[21].U16[0], state.v[22].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[21].U16[1], state.v[22].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[21].U16[2], state.v[22].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[21].U16[3], state.v[22].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[21].U16[4], state.v[22].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[21].U16[5], state.v[22].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[21].U16[6], state.v[22].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[21].U16[7], state.v[22].U16[7], s2value, 1, 0, 0, 1)
	}


_0400160C:	// 400160c: <4bb2bc44>	VMUDL     vec17 = ( acc = (vec23 * vec18[5] >> 16)      )
	{
		__asm {
			MOVQ mm0, state.v[23*16].U16[0]
			MOVQ mm1, state.v[23*16].U16[4*2]
			PSHUFW mm4, state.v[18*16].U16[2*2], 0
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			MOVQ state.v[17*16].U16[0], mm0
			MOVQ state.v[17*16].U16[4*2], mm1
		}
	}


_04001610:	// 4001610: <4b9e8c46>	VMUDN     vec17 = ( acc = vec17 * vec30[4]      ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[17*16].U16[0], 0xE4
			MOVQ mm1, state.v[17*16].U16[4*2]
			PSHUFW mm4, state.v[30*16].U16[3*2], 0
			PSHUFW mm6, mm0, 0xE4
			MOVQ mm7, mm1
			PMULLW mm6, mm4
			PMULLW mm7, mm4
			MOVQ accumulator[0*4], mm6
			MOVQ accumulator[2*4], mm7
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm2, mm4
			PMULHUW mm3, mm4
			PSRAW mm0, 15
			PSRAW mm1, 15
			PMULLW mm0, mm4
			PMULLW mm1, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_04001614:	// 4001614: <4b72fc4e>	VMADN     vec17 = ( acc+= vec31 * vec18[3] )
	{
		_u16		s2value = state.v[18].U16[4];
		VMADN_operation(0, state.v[17].U16[0], state.v[31].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[17].U16[1], state.v[31].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[17].U16[2], state.v[31].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[17].U16[3], state.v[31].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[17].U16[4], state.v[31].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[17].U16[5], state.v[31].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[17].U16[6], state.v[31].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[17].U16[7], state.v[31].U16[7], s2value, 1, 0, 0, 1)
	}


_04001618:	// 4001618: <c9392000>	LQV       vec25 <00> = [T1 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[25].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[25].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[25].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[25].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[25].U64[0] = state.v[25].U64[0] & ~temp1;
			state.v[25].U64[1] = state.v[25].U64[1] & ~temp2;
			state.v[25].U64[0] = state.v[25].U64[0] | value1;
			state.v[25].U64[1] = state.v[25].U64[1] | value2;
		}
	}


_0400161C:	// 400161c: <eab52000>	SQV       [S5 + 0x00] = vec21 <00>
	{
		_u32    addr = (0x00000000 + state.r[S5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[21].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[21].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[21].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[21].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[21].U64[0] & temp1);
			value2 |= (state.v[21].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001620:	// 4001620: <ea912000>	SQV       [S4 + 0x00] = vec17 <00>
	{
		_u32    addr = (0x00000000 + state.r[S4]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[17].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[17].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[17].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[17].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[17].U64[0] & temp1);
			value2 |= (state.v[17].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001624:	// 4001624: <eaf70b84>	SSV       [S7 + 0x08] = vec23 <07>
	Save16_DMEM(state.v[23].U16[4], (0x00000008 + state.r[S7]) & 0xfff);


_04001628:	// 4001628: <86b10000>	LH        S1 = [S5+0000]
	state.r[S1] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x00000000) & 0xfff);


_0400162C:	// 400162c: <86890000>	LH        T1 = [S4+0000]
	state.r[T1] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x00000000) & 0xfff);


_04001630:	// 4001630: <86ad0008>	LH        T5 = [S5+0008]
	state.r[T5] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x00000008) & 0xfff);


_04001634:	// 4001634: <86850008>	LH        A1 = [S4+0008]
	state.r[A1] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x00000008) & 0xfff);


_04001638:	// 4001638: <86b00002>	LH        S0 = [S5+0002]
	state.r[S0] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x00000002) & 0xfff);


_0400163C:	// 400163c: <86880002>	LH        T0 = [S4+0002]
	state.r[T0] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x00000002) & 0xfff);


_04001640:	// 4001640: <86ac000a>	LH        T4 = [S5+000a]
	state.r[T4] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x0000000A) & 0xfff);


_04001644:	// 4001644: <8684000a>	LH        A0 = [S4+000a]
	state.r[A0] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x0000000A) & 0xfff);


_04001648:	// 4001648: <86af0004>	LH        T7 = [S5+0004]
	state.r[T7] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x00000004) & 0xfff);


_0400164C:	// 400164c: <86870004>	LH        A3 = [S4+0004]
	state.r[A3] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x00000004) & 0xfff);


_04001650:	// 4001650: <86ab000c>	LH        T3 = [S5+000c]
	state.r[T3] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x0000000C) & 0xfff);


_04001654:	// 4001654: <8683000c>	LH        V1 = [S4+000c]
	state.r[V1] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x0000000C) & 0xfff);


_04001658:	// 4001658: <86ae0006>	LH        T6 = [S5+0006]
	state.r[T6] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x00000006) & 0xfff);


_0400165C:	// 400165c: <86860006>	LH        A2 = [S4+0006]
	state.r[A2] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x00000006) & 0xfff);


_04001660:	// 4001660: <86aa000e>	LH        T2 = [S5+000e]
	state.r[T2] = (_s32)(_s16)Load16_DMEM((state.r[S5] + 0x0000000E) & 0xfff);


_04001664:	// 4001664: <8682000e>	LH        V0 = [S4+000e]
	state.r[V0] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x0000000E) & 0xfff);


_04001668:	// 4001668: <ca301800>	LDV       vec16 <00> = [S1 + 0x00]
	// In function: Mario19D8_1AC4
	//Call the function: Mario19D8_1AC4
	Mario19D8_1AC4();
	goto _04001758;

_0400166C:	// 400166c: <4bf7fdc5>	VMUDM     vec23 = ( acc = vec31 * vec23[7] ) >> 16
	ErrorToCallFunction(0x0400166C);

_04001670:	// 4001670: <c92f1800>	LDV       vec15 <00> = [T1 + 0x00]
	ErrorToCallFunction(0x04001670);

_04001674:	// 4001674: <4bf6fdcf>	VMADH     vec23 = ( acc+= (vec31 * vec22[7]) << 16) >> 16
	ErrorToCallFunction(0x04001674);

_04001678:	// 4001678: <c9b01c00>	LDV       vec16 <08> = [T5 + 0x00]
	ErrorToCallFunction(0x04001678);

_0400167C:	// 400167c: <4b92cd8d>	VMADM     vec22 = ( acc+= vec25 * vec18[4] ) >> 16
	ErrorToCallFunction(0x0400167C);

_04001680:	// 4001680: <c8af1c00>	LDV       vec15 <08> = [A1 + 0x00]
	ErrorToCallFunction(0x04001680);

_04001684:	// 4001684: <4b1efdce>	VMADN     vec23 = ( acc+= vec31 * vec30[0] )
	ErrorToCallFunction(0x04001684);

_04001688:	// 4001688: <ca0e1800>	LDV       vec14 <00> = [S0 + 0x00]
	ErrorToCallFunction(0x04001688);

_0400168C:	// 400168c: <4b52fd46>	VMUDN     vec21 = ( acc = vec31 * vec18[2]      ) >> 16
	ErrorToCallFunction(0x0400168C);

_04001690:	// 4001690: <c90d1800>	LDV       vec13 <00> = [T0 + 0x00]
	ErrorToCallFunction(0x04001690);

_04001694:	// 4001694: <4b5eb54e>	VMADN     vec21 = ( acc+= vec22 * vec30[2] )
	ErrorToCallFunction(0x04001694);

_04001698:	// 4001698: <c98e1c00>	LDV       vec14 <08> = [T4 + 0x00]
	ErrorToCallFunction(0x04001698);

_0400169C:	// 400169c: <4bb2bc44>	VMUDL     vec17 = ( acc = (vec23 * vec18[5] >> 16)      )
	ErrorToCallFunction(0x0400169C);

_040016A0:	// 40016a0: <c88d1c00>	LDV       vec13 <08> = [A0 + 0x00]
	ErrorToCallFunction(0x040016A0);

_040016A4:	// 40016a4: <c9ec1800>	LDV       vec12 <00> = [T7 + 0x00]
	ErrorToCallFunction(0x040016A4);

_040016A8:	// 40016a8: <c8eb1800>	LDV       vec11 <00> = [A3 + 0x00]
	ErrorToCallFunction(0x040016A8);

_040016AC:	// 40016ac: <c96c1c00>	LDV       vec12 <08> = [T3 + 0x00]
	ErrorToCallFunction(0x040016AC);

_040016B0:	// 40016b0: <4b9e8c46>	VMUDN     vec17 = ( acc = vec17 * vec30[4]      ) >> 16
	ErrorToCallFunction(0x040016B0);

_040016B4:	// 40016b4: <c86b1c00>	LDV       vec11 <08> = [V1 + 0x00]
	ErrorToCallFunction(0x040016B4);

_040016B8:	// 40016b8: <c9ca1800>	LDV       vec10 <00> = [T6 + 0x00]
	ErrorToCallFunction(0x040016B8);

_040016BC:	// 40016bc: <c8c91800>	LDV       vec09 <00> = [A2 + 0x00]
	ErrorToCallFunction(0x040016BC);

_040016C0:	// 40016c0: <4b72fc4e>	VMADN     vec17 = ( acc+= vec31 * vec18[3] )
	ErrorToCallFunction(0x040016C0);

_040016C4:	// 40016c4: <c94a1c00>	LDV       vec10 <08> = [T2 + 0x00]
	ErrorToCallFunction(0x040016C4);

_040016C8:	// 40016c8: <4a0f8200>	VMULF     vec08 = ( acc = (vec16 * vec15[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x040016C8);

_040016CC:	// 40016cc: <c8491c00>	LDV       vec09 <08> = [V0 + 0x00]
	ErrorToCallFunction(0x040016CC);

_040016D0:	// 40016d0: <4a0d71c0>	VMULF     vec07 = ( acc = (vec14 * vec13[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x040016D0);

_040016D4:	// 40016d4: <eab52000>	SQV       [S5 + 0x00] = vec21 <00>
	ErrorToCallFunction(0x040016D4);

_040016D8:	// 40016d8: <4a0b6180>	VMULF     vec06 = ( acc = (vec12 * vec11[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x040016D8);

_040016DC:	// 40016dc: <ea912000>	SQV       [S4 + 0x00] = vec17 <00>
	ErrorToCallFunction(0x040016DC);

_040016E0:	// 40016e0: <86b10000>	LH        S1 = [S5+0000]
	ErrorToCallFunction(0x040016E0);

_040016E4:	// 40016e4: <4a095140>	VMULF     vec05 = ( acc = (vec10 * vec09[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x040016E4);

_040016E8:	// 40016e8: <86890000>	LH        T1 = [S4+0000]
	ErrorToCallFunction(0x040016E8);

_040016EC:	// 40016ec: <4a684210>	VADD      vec08 = vec08 + vec08[1q]
	ErrorToCallFunction(0x040016EC);

_040016F0:	// 40016f0: <86ad0008>	LH        T5 = [S5+0008]
	ErrorToCallFunction(0x040016F0);

_040016F4:	// 40016f4: <4a6739d0>	VADD      vec07 = vec07 + vec07[1q]
	ErrorToCallFunction(0x040016F4);

_040016F8:	// 40016f8: <86850008>	LH        A1 = [S4+0008]
	ErrorToCallFunction(0x040016F8);

_040016FC:	// 40016fc: <4a663190>	VADD      vec06 = vec06 + vec06[1q]
	ErrorToCallFunction(0x040016FC);

_04001700:	// 4001700: <86b00002>	LH        S0 = [S5+0002]
	ErrorToCallFunction(0x04001700);

_04001704:	// 4001704: <4a652950>	VADD      vec05 = vec05 + vec05[1q]
	ErrorToCallFunction(0x04001704);

_04001708:	// 4001708: <86880002>	LH        T0 = [S4+0002]
	ErrorToCallFunction(0x04001708);

_0400170C:	// 400170c: <4ac84210>	VADD      vec08 = vec08 + vec08[2h]
	ErrorToCallFunction(0x0400170C);

_04001710:	// 4001710: <86ac000a>	LH        T4 = [S5+000a]
	ErrorToCallFunction(0x04001710);

_04001714:	// 4001714: <4ac739d0>	VADD      vec07 = vec07 + vec07[2h]
	ErrorToCallFunction(0x04001714);

_04001718:	// 4001718: <8684000a>	LH        A0 = [S4+000a]
	ErrorToCallFunction(0x04001718);

_0400171C:	// 400171c: <4ac63190>	VADD      vec06 = vec06 + vec06[2h]
	ErrorToCallFunction(0x0400171C);

_04001720:	// 4001720: <86af0004>	LH        T7 = [S5+0004]
	ErrorToCallFunction(0x04001720);

_04001724:	// 4001724: <4ac52950>	VADD      vec05 = vec05 + vec05[2h]
	ErrorToCallFunction(0x04001724);

_04001728:	// 4001728: <86870004>	LH        A3 = [S4+0004]
	ErrorToCallFunction(0x04001728);

_0400172C:	// 400172c: <4a88e906>	VMUDN     vec04 = ( acc = vec29 * vec08[0h]      ) >> 16
	ErrorToCallFunction(0x0400172C);

_04001730:	// 4001730: <86ab000c>	LH        T3 = [S5+000c]
	ErrorToCallFunction(0x04001730);

_04001734:	// 4001734: <4a87e10e>	VMADN     vec04 = ( acc+= vec28 * vec07[0h] )
	ErrorToCallFunction(0x04001734);

_04001738:	// 4001738: <8683000c>	LH        V1 = [S4+000c]
	ErrorToCallFunction(0x04001738);

_0400173C:	// 400173c: <4a86d90e>	VMADN     vec04 = ( acc+= vec27 * vec06[0h] )
	ErrorToCallFunction(0x0400173C);

_04001740:	// 4001740: <86ae0006>	LH        T6 = [S5+0006]
	ErrorToCallFunction(0x04001740);

_04001744:	// 4001744: <4a85d10e>	VMADN     vec04 = ( acc+= vec26 * vec05[0h] )
	ErrorToCallFunction(0x04001744);

_04001748:	// 4001748: <86860006>	LH        A2 = [S4+0006]
	ErrorToCallFunction(0x04001748);

_0400174C:	// 400174c: <86aa000e>	LH        T2 = [S5+000e]
	ErrorToCallFunction(0x0400174C);

_04001750:	// 4001750: <2252fff0>	ADDI      S2 = S2 + fff0
	ErrorToCallFunction(0x04001750);

_04001754:	// 4001754: <ea642000>	SQV       [S3 + 0x00] = vec04 <00>
	ErrorToCallFunction(0x04001754);

_04001758:	// 4001758: <1a400003>	BLEZ      (R0<=0) --> 1768
	sp_reg_pc = 0x04001758;
	if ((_s32)state.r[S2] <= 0)
	{
		state.r[V0] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x0000000E) & 0xfff);
		goto _04001768;
	}


_0400175C:	// 400175c: <8682000e>	LH        V0 = [S4+000e]
	state.r[V0] = (_s32)(_s16)Load16_DMEM((state.r[S4] + 0x0000000E) & 0xfff);


_04001760:	// 4001760: <0900059a>	J         04001668
	state.r[S3] = state.r[S3] + 0x0010;
	goto _04001668;


_04001764:	// 4001764: <22730010>	ADDI      S3 = S3 + 0010
	state.r[S3] = state.r[S3] + 0x0010;


_04001768:	// 4001768: <eaf70804>	SSV       [S7 + 0x08] = vec23 <00>
	Save16_DMEM(state.v[23].U16[7], (0x00000008 + state.r[S7]) & 0xfff);


_0400176C:	// 400176c: <ca301800>	LDV       vec16 <00> = [S1 + 0x00]
	rsp_ldv_compiler((state.r[S1] & 0xFFF), 16, 8);


_04001770:	// 4001770: <eaf01800>	SDV       [S7 + 0x00] = vec16 <00>
	{
		_u32	addr = (0x00000000 + state.r[S7]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[16].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[16].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[16].U32[0x00000003];
		}
	}


_04001774:	// 4001774: <00161020>	ADD       V0 = R0+S6
	state.r[V0] = (0 + state.r[S6]);


_04001778:	// 4001778: <22e10000>	ADDI      AT = S7 + 0000
	state.r[AT] = state.r[S7] + 0x0000;


_0400177C:	// 400177c: <090006b1>	J         04001ac4
	state.r[V1] = 0 + 0x0020;
	goto _04001AC4;


_04001780:	// 4001780: <20030020>	ADDI      V1 = R0 + 0020
	state.r[V1] = 0 + 0x0020;


_04001784:	// 4001784: <001a7c02>	SRL       T7 = K0 >> 16
	state.r[T7] = ((_u32)state.r[K0] >> 16);


_04001788:	// 4001788: <31ef00ff>	ANDI      T7 = T7 & 00ff
	state.r[T7] = (state.r[T7] & 0x000000FF);


_0400178C:	// 400178c: <334dffff>	ANDI      T5 = K0 & ffff
	state.r[T5] = (state.r[K0] & 0x0000FFFF);


_04001790:	// 4001790: <00197402>	SRL       T6 = T9 >> 16
	state.r[T6] = ((_u32)state.r[T9] >> 16);


_04001794:	// 4001794: <21efffff>	ADDI      T7 = T7 + ffff
	state.r[T7] = state.r[T7] + 0xFFFFFFFF;


_04001798:	// 4001798: <332cffff>	ANDI      T4 = T9 & ffff
	state.r[T4] = (state.r[T9] & 0x0000FFFF);


_0400179C:	// 400179c: <c9a12000>	LQV       vec01 <00> = [T5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[1].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[1].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[1].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[1].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[1].U64[0] = state.v[1].U64[0] & ~temp1;
			state.v[1].U64[1] = state.v[1].U64[1] & ~temp2;
			state.v[1].U64[0] = state.v[1].U64[0] | value1;
			state.v[1].U64[1] = state.v[1].U64[1] | value2;
		}
	}


_040017A0:	// 40017a0: <c9a22001>	LQV       vec02 <00> = [T5 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[2].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[2].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[2].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[2].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[2].U64[0] = state.v[2].U64[0] & ~temp1;
			state.v[2].U64[1] = state.v[2].U64[1] & ~temp2;
			state.v[2].U64[0] = state.v[2].U64[0] | value1;
			state.v[2].U64[1] = state.v[2].U64[1] | value2;
		}
	}


_040017A4:	// 40017a4: <218cffe0>	ADDI      T4 = T4 + ffe0
	state.r[T4] = state.r[T4] + 0xFFFFFFE0;


_040017A8:	// 40017a8: <21ad0020>	ADDI      T5 = T5 + 0020
	state.r[T5] = state.r[T5] + 0x0020;


_040017AC:	// 40017ac: <e9c12000>	SQV       [T6 + 0x00] = vec01 <00>
	{
		_u32    addr = (0x00000000 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[1].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[1].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[1].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[1].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[1].U64[0] & temp1);
			value2 |= (state.v[1].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040017B0:	// 40017b0: <e9c22001>	SQV       [T6 + 0x10] = vec02 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[2].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[2].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[2].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[2].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[2].U64[0] & temp1);
			value2 |= (state.v[2].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040017B4:	// 40017b4: <1d80fff9>	BGTZ      (R0>0) --> 179c
	sp_reg_pc = 0x040017B4;
	if((_s32)state.r[T4] > 0)
	{
		state.r[T6] = state.r[T6] + 0x0020;
		goto _0400179C;
	}


_040017B8:	// 40017b8: <21ce0020>	ADDI      T6 = T6 + 0020
	state.r[T6] = state.r[T6] + 0x0020;


_040017BC:	// 40017bc: <1de0fff5>	BGTZ      (R0>0) --> 1794
	sp_reg_pc = 0x040017BC;
	if((_s32)state.r[T7] > 0)
	{
		goto _04001794;
	}


_040017C0:	// 40017c0: <00000000>	NOP       


_040017C4:	// 40017c4: <09000423>	J         0400108c
	goto _0400108C;


_040017C8:	// 40017c8: <00000000>	NOP       


_040017CC:	// 40017cc: <001a7c02>	SRL       T7 = K0 >> 16
	state.r[T7] = ((_u32)state.r[K0] >> 16);


_040017D0:	// 40017d0: <31ef00ff>	ANDI      T7 = T7 & 00ff
	state.r[T7] = (state.r[T7] & 0x000000FF);


_040017D4:	// 40017d4: <334dffff>	ANDI      T5 = K0 & ffff
	state.r[T5] = (state.r[K0] & 0x0000FFFF);


_040017D8:	// 40017d8: <00197402>	SRL       T6 = T9 >> 16
	state.r[T6] = ((_u32)state.r[T9] >> 16);


_040017DC:	// 40017dc: <c9a12000>	LQV       vec01 <00> = [T5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[1].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[1].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[1].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[1].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[1].U64[0] = state.v[1].U64[0] & ~temp1;
			state.v[1].U64[1] = state.v[1].U64[1] & ~temp2;
			state.v[1].U64[0] = state.v[1].U64[0] | value1;
			state.v[1].U64[1] = state.v[1].U64[1] | value2;
		}
	}


_040017E0:	// 40017e0: <c9a22001>	LQV       vec02 <00> = [T5 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[2].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[2].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[2].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[2].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[2].U64[0] = state.v[2].U64[0] & ~temp1;
			state.v[2].U64[1] = state.v[2].U64[1] & ~temp2;
			state.v[2].U64[0] = state.v[2].U64[0] | value1;
			state.v[2].U64[1] = state.v[2].U64[1] | value2;
		}
	}


_040017E4:	// 40017e4: <c9a32002>	LQV       vec03 <00> = [T5 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[3].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[3].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[3].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[3].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[3].U64[0] = state.v[3].U64[0] & ~temp1;
			state.v[3].U64[1] = state.v[3].U64[1] & ~temp2;
			state.v[3].U64[0] = state.v[3].U64[0] | value1;
			state.v[3].U64[1] = state.v[3].U64[1] | value2;
		}
	}


_040017E8:	// 40017e8: <c9a42003>	LQV       vec04 <00> = [T5 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[4].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[4].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[4].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[4].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[4].U64[0] = state.v[4].U64[0] & ~temp1;
			state.v[4].U64[1] = state.v[4].U64[1] & ~temp2;
			state.v[4].U64[0] = state.v[4].U64[0] | value1;
			state.v[4].U64[1] = state.v[4].U64[1] | value2;
		}
	}


_040017EC:	// 40017ec: <c9a52004>	LQV       vec05 <00> = [T5 + 0x40]
	{
		_u32 addr = (0x00000040 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[5].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[5].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[5].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[5].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[5].U64[0] = state.v[5].U64[0] & ~temp1;
			state.v[5].U64[1] = state.v[5].U64[1] & ~temp2;
			state.v[5].U64[0] = state.v[5].U64[0] | value1;
			state.v[5].U64[1] = state.v[5].U64[1] | value2;
		}
	}


_040017F0:	// 40017f0: <c9a62005>	LQV       vec06 <00> = [T5 + 0x50]
	{
		_u32 addr = (0x00000050 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[6].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[6].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[6].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[6].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[6].U64[0] = state.v[6].U64[0] & ~temp1;
			state.v[6].U64[1] = state.v[6].U64[1] & ~temp2;
			state.v[6].U64[0] = state.v[6].U64[0] | value1;
			state.v[6].U64[1] = state.v[6].U64[1] | value2;
		}
	}


_040017F4:	// 40017f4: <c9a72006>	LQV       vec07 <00> = [T5 + 0x60]
	{
		_u32 addr = (0x00000060 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[7].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[7].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[7].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[7].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[7].U64[0] = state.v[7].U64[0] & ~temp1;
			state.v[7].U64[1] = state.v[7].U64[1] & ~temp2;
			state.v[7].U64[0] = state.v[7].U64[0] | value1;
			state.v[7].U64[1] = state.v[7].U64[1] | value2;
		}
	}


_040017F8:	// 40017f8: <c9a82007>	LQV       vec08 <00> = [T5 + 0x70]
	{
		_u32 addr = (0x00000070 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[8].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[8].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[8].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[8].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[8].U64[0] = state.v[8].U64[0] & ~temp1;
			state.v[8].U64[1] = state.v[8].U64[1] & ~temp2;
			state.v[8].U64[0] = state.v[8].U64[0] | value1;
			state.v[8].U64[1] = state.v[8].U64[1] | value2;
		}
	}


_040017FC:	// 40017fc: <21efffff>	ADDI      T7 = T7 + ffff
	state.r[T7] = state.r[T7] + 0xFFFFFFFF;


_04001800:	// 4001800: <e9c12000>	SQV       [T6 + 0x00] = vec01 <00>
	{
		_u32    addr = (0x00000000 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[1].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[1].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[1].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[1].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[1].U64[0] & temp1);
			value2 |= (state.v[1].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001804:	// 4001804: <e9c22001>	SQV       [T6 + 0x10] = vec02 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[2].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[2].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[2].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[2].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[2].U64[0] & temp1);
			value2 |= (state.v[2].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001808:	// 4001808: <e9c32002>	SQV       [T6 + 0x20] = vec03 <00>
	{
		_u32    addr = (0x00000020 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[3].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[3].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[3].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[3].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[3].U64[0] & temp1);
			value2 |= (state.v[3].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_0400180C:	// 400180c: <e9c42003>	SQV       [T6 + 0x30] = vec04 <00>
	{
		_u32    addr = (0x00000030 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[4].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[4].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[4].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[4].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[4].U64[0] & temp1);
			value2 |= (state.v[4].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001810:	// 4001810: <e9c52004>	SQV       [T6 + 0x40] = vec05 <00>
	{
		_u32    addr = (0x00000040 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[5].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[5].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[5].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[5].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[5].U64[0] & temp1);
			value2 |= (state.v[5].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001814:	// 4001814: <e9c62005>	SQV       [T6 + 0x50] = vec06 <00>
	{
		_u32    addr = (0x00000050 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[6].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[6].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[6].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[6].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[6].U64[0] & temp1);
			value2 |= (state.v[6].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001818:	// 4001818: <e9c72006>	SQV       [T6 + 0x60] = vec07 <00>
	{
		_u32    addr = (0x00000060 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[7].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[7].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[7].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[7].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[7].U64[0] & temp1);
			value2 |= (state.v[7].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_0400181C:	// 400181c: <e9c82007>	SQV       [T6 + 0x70] = vec08 <00>
	{
		_u32    addr = (0x00000070 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[8].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[8].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[8].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[8].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[8].U64[0] & temp1);
			value2 |= (state.v[8].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001820:	// 4001820: <1de0fff6>	BGTZ      (R0>0) --> 17fc
	sp_reg_pc = 0x04001820;
	if((_s32)state.r[T7] > 0)
	{
		state.r[T6] = state.r[T6] + 0x0080;
		goto _040017FC;
	}


_04001824:	// 4001824: <21ce0080>	ADDI      T6 = T6 + 0080
	state.r[T6] = state.r[T6] + 0x0080;


_04001828:	// 4001828: <09000423>	J         0400108c
	goto _0400108C;


_0400182C:	// 400182c: <00000000>	NOP       


_04001830:	// 4001830: <334cffff>	ANDI      T4 = K0 & ffff
	state.r[T4] = (state.r[K0] & 0x0000FFFF);


_04001834:	// 4001834: <332effff>	ANDI      T6 = T9 & ffff
	state.r[T6] = (state.r[T9] & 0x0000FFFF);


_04001838:	// 4001838: <00196c02>	SRL       T5 = T9 >> 16
	state.r[T5] = ((_u32)state.r[T9] >> 16);


_0400183C:	// 400183c: <c9a10800>	LSV       vec01 <00> = [T5 + 0x00]
	state.v[1].U16[7] = Load16_DMEM(0x00000000 + state.r[T5]);


_04001840:	// 4001840: <c9a20804>	LSV       vec02 <00> = [T5 + 0x08]
	state.v[2].U16[7] = Load16_DMEM(0x00000008 + state.r[T5]);


_04001844:	// 4001844: <c9a30808>	LSV       vec03 <00> = [T5 + 0x10]
	state.v[3].U16[7] = Load16_DMEM(0x00000010 + state.r[T5]);


_04001848:	// 4001848: <c9a4080c>	LSV       vec04 <00> = [T5 + 0x18]
	state.v[4].U16[7] = Load16_DMEM(0x00000018 + state.r[T5]);


_0400184C:	// 400184c: <c9a10902>	LSV       vec01 <02> = [T5 + 0x04]
	state.v[1].U16[6] = Load16_DMEM(0x00000004 + state.r[T5]);


_04001850:	// 4001850: <c9a20906>	LSV       vec02 <02> = [T5 + 0x0c]
	state.v[2].U16[6] = Load16_DMEM(0x0000000C + state.r[T5]);


_04001854:	// 4001854: <c9a3090a>	LSV       vec03 <02> = [T5 + 0x14]
	state.v[3].U16[6] = Load16_DMEM(0x00000014 + state.r[T5]);


_04001858:	// 4001858: <c9a4090e>	LSV       vec04 <02> = [T5 + 0x1c]
	state.v[4].U16[6] = Load16_DMEM(0x0000001C + state.r[T5]);


_0400185C:	// 400185c: <21ad0020>	ADDI      T5 = T5 + 0020
	state.r[T5] = state.r[T5] + 0x0020;


_04001860:	// 4001860: <218cfff8>	ADDI      T4 = T4 + fff8
	state.r[T4] = state.r[T4] + 0xFFFFFFF8;


_04001864:	// 4001864: <e9c11000>	SLV       [T6 + 0x00] = vec01 <00>
	Save32_DMEM (state.v[1].U32[3], 0x00000000 + state.r[T6]);


_04001868:	// 4001868: <e9c21001>	SLV       [T6 + 0x08] = vec02 <00>
	Save32_DMEM (state.v[2].U32[3], 0x00000004 + state.r[T6]);


_0400186C:	// 400186c: <e9c31002>	SLV       [T6 + 0x10] = vec03 <00>
	Save32_DMEM (state.v[3].U32[3], 0x00000008 + state.r[T6]);


_04001870:	// 4001870: <e9c41003>	SLV       [T6 + 0x18] = vec04 <00>
	Save32_DMEM (state.v[4].U32[3], 0x0000000C + state.r[T6]);


_04001874:	// 4001874: <1d80fff1>	BGTZ      (R0>0) --> 183c
	sp_reg_pc = 0x04001874;
	if((_s32)state.r[T4] > 0)
	{
		state.r[T6] = state.r[T6] + 0x0010;
		goto _0400183C;
	}


_04001878:	// 4001878: <21ce0010>	ADDI      T6 = T6 + 0010
	state.r[T6] = state.r[T6] + 0x0010;


_0400187C:	// 400187c: <09000423>	J         0400108c
	goto _0400108C;


_04001880:	// 4001880: <00000000>	NOP       


_04001884:	// 4001884: <4a04212c>	VXOR      vec04 = vec04 xor vec04[<none>]
	rsp_cop2_vxor(&state.v[4], &state.v[4], &state.v[4]);


_04001888:	// 4001888: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_0400188C:	// 400188c: <c8032000>	LQV       vec03 <00> = [R0 + 0x00]
	{
		_u32 addr = (0x00000000 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[3].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[3].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[3].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[3].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[3].U64[0] = state.v[3].U64[0] & ~temp1;
			state.v[3].U64[1] = state.v[3].U64[1] & ~temp2;
			state.v[3].U64[0] = state.v[3].U64[0] | value1;
			state.v[3].U64[1] = state.v[3].U64[1] | value2;
		}
	}


_04001890:	// 4001890: <02b5a820>	ADD       S5 = S5+S5
	state.r[S5] = (state.r[S5] + state.r[S5]);


_04001894:	// 4001894: <48952000>	MTC2      vec4 <00> = S5
	state.v[4].U16[7] = (_u16)state.r[S5];


_04001898:	// 4001898: <48952100>	MTC2      vec4 <02> = S5
	state.v[4].U16[6] = (_u16)state.r[S5];


_0400189C:	// 400189c: <001a6302>	SRL       T4 = K0 >> 12
	state.r[T4] = ((_u32)state.r[K0] >> 12);


_040018A0:	// 40018a0: <31930ff0>	ANDI      S3 = T4 & 0ff0
	state.r[S3] = (state.r[T4] & 0x00000FF0);


_040018A4:	// 40018a4: <02d6b020>	ADD       S6 = S6+S6
	state.r[S6] = (state.r[S6] + state.r[S6]);


_040018A8:	// 40018a8: <48962200>	MTC2      vec4 <04> = S6
	state.v[4].U16[5] = (_u16)state.r[S6];


_040018AC:	// 40018ac: <48962300>	MTC2      vec4 <06> = S6
	state.v[4].U16[4] = (_u16)state.r[S6];


_040018B0:	// 40018b0: <00196502>	SRL       T4 = T9 >> 20
	state.r[T4] = ((_u32)state.r[T9] >> 20);


_040018B4:	// 40018b4: <318e0ff0>	ANDI      T6 = T4 & 0ff0
	state.r[T6] = (state.r[T4] & 0x00000FF0);


_040018B8:	// 40018b8: <016b5820>	ADD       T3 = T3+T3
	state.r[T3] = (state.r[T3] + state.r[T3]);


_040018BC:	// 40018bc: <488b2400>	MTC2      vec4 <08> = T3
	state.v[4].U16[3] = (_u16)state.r[T3];


_040018C0:	// 40018c0: <488b2500>	MTC2      vec4 <10> = T3
	state.v[4].U16[2] = (_u16)state.r[T3];


_040018C4:	// 40018c4: <00196302>	SRL       T4 = T9 >> 12
	state.r[T4] = ((_u32)state.r[T9] >> 12);


_040018C8:	// 40018c8: <318f0ff0>	ANDI      T7 = T4 & 0ff0
	state.r[T7] = (state.r[T4] & 0x00000FF0);


_040018CC:	// 40018cc: <00196102>	SRL       T4 = T9 >> 4
	state.r[T4] = ((_u32)state.r[T9] >> 4);


_040018D0:	// 40018d0: <31900ff0>	ANDI      S0 = T4 & 0ff0
	state.r[S0] = (state.r[T4] & 0x00000FF0);


_040018D4:	// 40018d4: <00196100>	SLL       T4 = T9 << 4
	state.r[T4] = ((_u32)state.r[T9] << 4);


_040018D8:	// 40018d8: <31910ff0>	ANDI      S1 = T4 & 0ff0
	state.r[S1] = (state.r[T4] & 0x00000FF0);


_040018DC:	// 40018dc: <334c0002>	ANDI      T4 = K0 & 0002
	state.r[T4] = (state.r[K0] & 0x00000002);


_040018E0:	// 40018e0: <000c6042>	SRL       T4 = T4 >> 1
	state.r[T4] = ((_u32)state.r[T4] >> 1);


_040018E4:	// 40018e4: <000c6022>	SUB       T4 = R0-T4
	state.r[T4] = (0 - state.r[T4]);


_040018E8:	// 40018e8: <488c1000>	MTC2      vec2 <00> = T4
	state.v[2].U16[7] = (_u16)state.r[T4];


_040018EC:	// 40018ec: <334c0001>	ANDI      T4 = K0 & 0001
	state.r[T4] = (state.r[K0] & 0x00000001);


_040018F0:	// 40018f0: <000c6022>	SUB       T4 = R0-T4
	state.r[T4] = (0 - state.r[T4]);


_040018F4:	// 40018f4: <488c1100>	MTC2      vec2 <02> = T4
	state.v[2].U16[6] = (_u16)state.r[T4];


_040018F8:	// 40018f8: <334c0008>	ANDI      T4 = K0 & 0008
	state.r[T4] = (state.r[K0] & 0x00000008);


_040018FC:	// 40018fc: <000c6042>	SRL       T4 = T4 >> 1
	state.r[T4] = ((_u32)state.r[T4] >> 1);


_04001900:	// 4001900: <000c6022>	SUB       T4 = R0-T4
	state.r[T4] = (0 - state.r[T4]);


_04001904:	// 4001904: <488c1200>	MTC2      vec2 <04> = T4
	state.v[2].U16[5] = (_u16)state.r[T4];


_04001908:	// 4001908: <334c0004>	ANDI      T4 = K0 & 0004
	state.r[T4] = (state.r[K0] & 0x00000004);


_0400190C:	// 400190c: <000c6042>	SRL       T4 = T4 >> 1
	state.r[T4] = ((_u32)state.r[T4] >> 1);


_04001910:	// 4001910: <000c6022>	SUB       T4 = R0-T4
	state.r[T4] = (0 - state.r[T4]);


_04001914:	// 4001914: <488c1300>	MTC2      vec2 <06> = T4
	state.v[2].U16[4] = (_u16)state.r[T4];


_04001918:	// 4001918: <001a6202>	SRL       T4 = K0 >> 8
	state.r[T4] = ((_u32)state.r[K0] >> 8);


_0400191C:	// 400191c: <319400ff>	ANDI      S4 = T4 & 00ff
	state.r[S4] = (state.r[T4] & 0x000000FF);


_04001920:	// 4001920: <4a000010>	VADD      vec00 = vec00 + vec00[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[0].U16[0], state.v[0].U16[0], state.v[0].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[0].U16[1], state.v[0].U16[1], state.v[0].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[0].U16[2], state.v[0].U16[2], state.v[0].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[0].U16[3], state.v[0].U16[3], state.v[0].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[0].U16[4], state.v[0].U16[4], state.v[0].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[0].U16[5], state.v[0].U16[5], state.v[0].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[0].U16[6], state.v[0].U16[6], state.v[0].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[0].U16[7], state.v[0].U16[7], state.v[0].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001924:	// 4001924: <334a0010>	ANDI      T2 = K0 & 0010
	state.r[T2] = (state.r[K0] & 0x00000010);


_04001928:	// 4001928: <ca682000>	LQV       vec08 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[8].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[8].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[8].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[8].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[8].U64[0] = state.v[8].U64[0] & ~temp1;
			state.v[8].U64[1] = state.v[8].U64[1] & ~temp2;
			state.v[8].U64[0] = state.v[8].U64[0] | value1;
			state.v[8].U64[1] = state.v[8].U64[1] | value2;
		}
	}


_0400192C:	// 400192c: <ca6f2001>	LQV       vec15 <00> = [S3 + 0x10]

	// [start function: Zelda192C_19A8]

	Zelda192C_19A8();
	goto _040019AC;

_04001930:	// 4001930: <22730020>	ADDI      S3 = S3 + 0020

_04001934:	// 4001934: <4b014245>	VMUDM     vec09 = ( acc = vec08 * vec01[0] ) >> 16

_04001938:	// 4001938: <4b414285>	VMUDM     vec10 = ( acc = vec08 * vec01[2] ) >> 16

_0400193C:	// 400193c: <2294fff0>	ADDI      S4 = S4 + fff0

_04001940:	// 4001940: <c9cb2000>	LQV       vec11 <00> = [T6 + 0x00]

_04001944:	// 4001944: <c9ec2000>	LQV       vec12 <00> = [T7 + 0x00]

_04001948:	// 4001948: <4b217c05>	VMUDM     vec16 = ( acc = vec15 * vec01[1] ) >> 16

_0400194C:	// 400194c: <4b617c45>	VMUDM     vec17 = ( acc = vec15 * vec01[3] ) >> 16

_04001950:	// 4001950: <c9d22001>	LQV       vec18 <00> = [T6 + 0x10]

_04001954:	// 4001954: <c9f32001>	LQV       vec19 <00> = [T7 + 0x10]

_04001958:	// 4001958: <4b024a6c>	VXOR      vec09 = vec09 xor vec02[0]

_0400195C:	// 400195c: <4b2252ac>	VXOR      vec10 = vec10 xor vec02[1]

_04001960:	// 4001960: <ca0d2000>	LQV       vec13 <00> = [S0 + 0x00]

_04001964:	// 4001964: <ca2e2000>	LQV       vec14 <00> = [S1 + 0x00]

_04001968:	// 4001968: <4a095ad0>	VADD      vec11 = vec11 + vec09[<none>]

_0400196C:	// 400196c: <4a0a6310>	VADD      vec12 = vec12 + vec10[<none>]

_04001970:	// 4001970: <4b814a45>	VMUDM     vec09 = ( acc = vec09 * vec01[4] ) >> 16

_04001974:	// 4001974: <4b815285>	VMUDM     vec10 = ( acc = vec10 * vec01[4] ) >> 16

_04001978:	// 4001978: <4b02842c>	VXOR      vec16 = vec16 xor vec02[0]

_0400197C:	// 400197c: <4b228c6c>	VXOR      vec17 = vec17 xor vec02[1]

_04001980:	// 4001980: <ca142001>	LQV       vec20 <00> = [S0 + 0x10]

_04001984:	// 4001984: <ca352001>	LQV       vec21 <00> = [S1 + 0x10]

_04001988:	// 4001988: <4a109490>	VADD      vec18 = vec18 + vec16[<none>]

_0400198C:	// 400198c: <4a119cd0>	VADD      vec19 = vec19 + vec17[<none>]

_04001990:	// 4001990: <4ba18405>	VMUDM     vec16 = ( acc = vec16 * vec01[5] ) >> 16

_04001994:	// 4001994: <4ba18c45>	VMUDM     vec17 = ( acc = vec17 * vec01[5] ) >> 16

_04001998:	// 4001998: <4b424a6c>	VXOR      vec09 = vec09 xor vec02[2]

_0400199C:	// 400199c: <4b6252ac>	VXOR      vec10 = vec10 xor vec02[3]

_040019A0:	// 40019a0: <e9cb2000>	SQV       [T6 + 0x00] = vec11 <00>

_040019A4:	// 40019a4: <4b42842c>	VXOR      vec16 = vec16 xor vec02[2]

_040019A8:	// 40019a8: <4b628c6c>	VXOR      vec17 = vec17 xor vec02[3]
	ErrorToCallFunction(0x040019A8);

	// [end of function: Zelda192C_19A8]

_040019AC:	// 40019ac: <15400015>	BNE       (R0!=T2) --> 1a04
	sp_reg_pc = 0x040019AC;
	if (state.r[T2] != 0)
	{
		{
			_u32    addr = (0x00000000 + state.r[T7]);
			int     shifter = ((addr & 0xf) << 3);

			if (shifter == 0 && (addr&3) == 0 )
			{
				addr &= 0xfff;
				*(_u32 *)&pDMEM[addr+12] = state.v[12].U32[0];
				*(_u32 *)&pDMEM[addr+8 ] = state.v[12].U32[1];
				*(_u32 *)&pDMEM[addr+4 ] = state.v[12].U32[2];
				*(_u32 *)&pDMEM[addr   ] = state.v[12].U32[3];
			}
			else
			{
				_u64    value1 = Load64_DMEM(addr+8);
				_u64    value2 = Load64_DMEM(addr);
				_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
				_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

				if (shifter > 0x40)
				{
					temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
					temp1 = 0;
				} else {
					temp1 = ((temp1 >> shifter) << shifter);
				}
				value1 = value1 & ~temp1;
				value2 = value2 & ~temp2;

				value1 |= (state.v[12].U64[0] & temp1);
				value2 |= (state.v[12].U64[1] & temp2);

				Save64_DMEM(value1, addr+8);
				Save64_DMEM(value2, addr  );
			}
		}
		goto _04001A04;
	}


_040019B0:	// 40019b0: <e9ec2000>	SQV       [T7 + 0x00] = vec12 <00>
	{
		_u32    addr = (0x00000000 + state.r[T7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[12].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[12].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[12].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[12].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[12].U64[0] & temp1);
			value2 |= (state.v[12].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019B4:	// 40019b4: <4a096b50>	VADD      vec13 = vec13 + vec09[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[13].U16[0], state.v[13].U16[0], state.v[9].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[13].U16[1], state.v[13].U16[1], state.v[9].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[13].U16[2], state.v[13].U16[2], state.v[9].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[13].U16[3], state.v[13].U16[3], state.v[9].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[13].U16[4], state.v[13].U16[4], state.v[9].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[13].U16[5], state.v[13].U16[5], state.v[9].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[13].U16[6], state.v[13].U16[6], state.v[9].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[13].U16[7], state.v[13].U16[7], state.v[9].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040019B8:	// 40019b8: <4a0a7390>	VADD      vec14 = vec14 + vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[14].U16[0], state.v[14].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[14].U16[1], state.v[14].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[14].U16[2], state.v[14].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[14].U16[3], state.v[14].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[14].U16[4], state.v[14].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[14].U16[5], state.v[14].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[14].U16[6], state.v[14].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[14].U16[7], state.v[14].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040019BC:	// 40019bc: <e9d22001>	SQV       [T6 + 0x10] = vec18 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[18].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[18].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[18].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[18].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[18].U64[0] & temp1);
			value2 |= (state.v[18].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019C0:	// 40019c0: <e9f32001>	SQV       [T7 + 0x10] = vec19 <00>
	{
		_u32    addr = (0x00000010 + state.r[T7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[19].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[19].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[19].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[19].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[19].U64[0] & temp1);
			value2 |= (state.v[19].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019C4:	// 40019c4: <4a10a510>	VADD      vec20 = vec20 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[20].U16[0], state.v[20].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[20].U16[1], state.v[20].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[20].U16[2], state.v[20].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[20].U16[3], state.v[20].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[20].U16[4], state.v[20].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[20].U16[5], state.v[20].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[20].U16[6], state.v[20].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[20].U16[7], state.v[20].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040019C8:	// 40019c8: <4a11ad50>	VADD      vec21 = vec21 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[21].U16[0], state.v[21].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[21].U16[1], state.v[21].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[21].U16[2], state.v[21].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[21].U16[3], state.v[21].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[21].U16[4], state.v[21].U16[4], state.v[17].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[21].U16[5], state.v[21].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[21].U16[6], state.v[21].U16[6], state.v[17].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[21].U16[7], state.v[21].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040019CC:	// 40019cc: <21ce0020>	ADDI      T6 = T6 + 0020
	state.r[T6] = state.r[T6] + 0x0020;


_040019D0:	// 40019d0: <ea0d2000>	SQV       [S0 + 0x00] = vec13 <00>
	{
		_u32    addr = (0x00000000 + state.r[S0]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[13].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[13].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[13].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[13].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[13].U64[0] & temp1);
			value2 |= (state.v[13].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019D4:	// 40019d4: <ea2e2000>	SQV       [S1 + 0x00] = vec14 <00>
	{
		_u32    addr = (0x00000000 + state.r[S1]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[14].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[14].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[14].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[14].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[14].U64[0] & temp1);
			value2 |= (state.v[14].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019D8:	// 40019d8: <21ef0020>	ADDI      T7 = T7 + 0020
	state.r[T7] = state.r[T7] + 0x0020;


_040019DC:	// 40019dc: <ca682000>	LQV       vec08 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[8].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[8].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[8].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[8].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[8].U64[0] = state.v[8].U64[0] & ~temp1;
			state.v[8].U64[1] = state.v[8].U64[1] & ~temp2;
			state.v[8].U64[0] = state.v[8].U64[0] | value1;
			state.v[8].U64[1] = state.v[8].U64[1] | value2;
		}
	}


_040019E0:	// 40019e0: <ea142001>	SQV       [S0 + 0x10] = vec20 <00>
	{
		_u32    addr = (0x00000010 + state.r[S0]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[20].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[20].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[20].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[20].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[20].U64[0] & temp1);
			value2 |= (state.v[20].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019E4:	// 40019e4: <22100020>	ADDI      S0 = S0 + 0020
	state.r[S0] = state.r[S0] + 0x0020;


_040019E8:	// 40019e8: <4a040854>	VADDC	vec01 = vec01 + vec04[<none>]
	{
		_u16		flag0 = 0;
		VADDC_operation(0, state.v[1].U16[0], state.v[1].U16[0], state.v[4].U16[0], 1, 0, 0, 1)
		VADDC_operation(1, state.v[1].U16[1], state.v[1].U16[1], state.v[4].U16[1], 1, 0, 0, 1)
		VADDC_operation(2, state.v[1].U16[2], state.v[1].U16[2], state.v[4].U16[2], 1, 0, 0, 1)
		VADDC_operation(3, state.v[1].U16[3], state.v[1].U16[3], state.v[4].U16[3], 1, 0, 0, 1)
		VADDC_operation(4, state.v[1].U16[4], state.v[1].U16[4], state.v[4].U16[4], 1, 0, 0, 1)
		VADDC_operation(5, state.v[1].U16[5], state.v[1].U16[5], state.v[4].U16[5], 1, 0, 0, 1)
		VADDC_operation(6, state.v[1].U16[6], state.v[1].U16[6], state.v[4].U16[6], 1, 0, 0, 1)
		VADDC_operation(7, state.v[1].U16[7], state.v[1].U16[7], state.v[4].U16[7], 1, 0, 0, 1)
		state.flag[0] = flag0;
	}


_040019EC:	// 40019ec: <ea352001>	SQV       [S1 + 0x10] = vec21 <00>
	{
		_u32    addr = (0x00000010 + state.r[S1]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[21].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[21].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[21].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[21].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[21].U64[0] & temp1);
			value2 |= (state.v[21].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019F0:	// 40019f0: <22310020>	ADDI      S1 = S1 + 0020
	state.r[S1] = state.r[S1] + 0x0020;


_040019F4:	// 40019f4: <1e80ffcd>	BGTZ      (R0>0) --> 192c
	sp_reg_pc = 0x040019F4;
	if((_s32)state.r[S4] > 0)
	{
		{
			_u16		flag0 = state.flag[0];
			VADD_operation(0, state.v[0].U16[0], state.v[0].U16[0], state.v[0].U16[0], 1, 1, 1, 1)
			VADD_operation(1, state.v[0].U16[1], state.v[0].U16[1], state.v[0].U16[1], 1, 1, 1, 1)
			VADD_operation(2, state.v[0].U16[2], state.v[0].U16[2], state.v[0].U16[2], 1, 1, 1, 1)
			VADD_operation(3, state.v[0].U16[3], state.v[0].U16[3], state.v[0].U16[3], 1, 1, 1, 1)
			VADD_operation(4, state.v[0].U16[4], state.v[0].U16[4], state.v[0].U16[4], 1, 1, 1, 1)
			VADD_operation(5, state.v[0].U16[5], state.v[0].U16[5], state.v[0].U16[5], 1, 1, 1, 1)
			VADD_operation(6, state.v[0].U16[6], state.v[0].U16[6], state.v[0].U16[6], 1, 1, 1, 1)
			VADD_operation(7, state.v[0].U16[7], state.v[0].U16[7], state.v[0].U16[7], 1, 1, 1, 1)
			state.flag[0] = 0x0000;
		}
		goto _0400192C;
	}


_040019F8:	// 40019f8: <4a000010>	VADD      vec00 = vec00 + vec00[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[0].U16[0], state.v[0].U16[0], state.v[0].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[0].U16[1], state.v[0].U16[1], state.v[0].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[0].U16[2], state.v[0].U16[2], state.v[0].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[0].U16[3], state.v[0].U16[3], state.v[0].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[0].U16[4], state.v[0].U16[4], state.v[0].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[0].U16[5], state.v[0].U16[5], state.v[0].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[0].U16[6], state.v[0].U16[6], state.v[0].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[0].U16[7], state.v[0].U16[7], state.v[0].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040019FC:	// 40019fc: <09000423>	J         0400108c
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);
	goto _0400108C;


_04001A00:	// 4001a00: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_04001A04:	// 4001a04: <4a0a6b50>	VADD      vec13 = vec13 + vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[13].U16[0], state.v[13].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[13].U16[1], state.v[13].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[13].U16[2], state.v[13].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[13].U16[3], state.v[13].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[13].U16[4], state.v[13].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[13].U16[5], state.v[13].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[13].U16[6], state.v[13].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[13].U16[7], state.v[13].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001A08:	// 4001a08: <4a097390>	VADD      vec14 = vec14 + vec09[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[14].U16[0], state.v[14].U16[0], state.v[9].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[14].U16[1], state.v[14].U16[1], state.v[9].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[14].U16[2], state.v[14].U16[2], state.v[9].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[14].U16[3], state.v[14].U16[3], state.v[9].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[14].U16[4], state.v[14].U16[4], state.v[9].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[14].U16[5], state.v[14].U16[5], state.v[9].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[14].U16[6], state.v[14].U16[6], state.v[9].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[14].U16[7], state.v[14].U16[7], state.v[9].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001A0C:	// 4001a0c: <e9d22001>	SQV       [T6 + 0x10] = vec18 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[18].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[18].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[18].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[18].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[18].U64[0] & temp1);
			value2 |= (state.v[18].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001A10:	// 4001a10: <e9f32001>	SQV       [T7 + 0x10] = vec19 <00>
	{
		_u32    addr = (0x00000010 + state.r[T7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[19].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[19].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[19].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[19].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[19].U64[0] & temp1);
			value2 |= (state.v[19].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001A14:	// 4001a14: <4a11a510>	VADD      vec20 = vec20 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[20].U16[0], state.v[20].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[20].U16[1], state.v[20].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[20].U16[2], state.v[20].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[20].U16[3], state.v[20].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[20].U16[4], state.v[20].U16[4], state.v[17].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[20].U16[5], state.v[20].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[20].U16[6], state.v[20].U16[6], state.v[17].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[20].U16[7], state.v[20].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001A18:	// 4001a18: <09000673>	J         040019cc
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[21].U16[0], state.v[21].U16[0], state.v[16].U16[0], 1, 1, 1, 1)
		VADD_operation(1, state.v[21].U16[1], state.v[21].U16[1], state.v[16].U16[1], 1, 1, 1, 1)
		VADD_operation(2, state.v[21].U16[2], state.v[21].U16[2], state.v[16].U16[2], 1, 1, 1, 1)
		VADD_operation(3, state.v[21].U16[3], state.v[21].U16[3], state.v[16].U16[3], 1, 1, 1, 1)
		VADD_operation(4, state.v[21].U16[4], state.v[21].U16[4], state.v[16].U16[4], 1, 1, 1, 1)
		VADD_operation(5, state.v[21].U16[5], state.v[21].U16[5], state.v[16].U16[5], 1, 1, 1, 1)
		VADD_operation(6, state.v[21].U16[6], state.v[21].U16[6], state.v[16].U16[6], 1, 1, 1, 1)
		VADD_operation(7, state.v[21].U16[7], state.v[21].U16[7], state.v[16].U16[7], 1, 1, 1, 1)
		state.flag[0] = 0x0000;
	}
	goto _040019CC;


_04001A1C:	// 4001a1c: <4a10ad50>	VADD      vec21 = vec21 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[21].U16[0], state.v[21].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[21].U16[1], state.v[21].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[21].U16[2], state.v[21].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[21].U16[3], state.v[21].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[21].U16[4], state.v[21].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[21].U16[5], state.v[21].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[21].U16[6], state.v[21].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[21].U16[7], state.v[21].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001A20:	// 4001a20: <4a01086c>	VXOR      vec01 = vec01 xor vec01[<none>]
	rsp_cop2_vxor(&state.v[1], &state.v[1], &state.v[1]);


_04001A24:	// 4001a24: <334bffff>	ANDI      T3 = K0 & ffff
	state.r[T3] = (state.r[K0] & 0x0000FFFF);


_04001A28:	// 4001a28: <001a6202>	SRL       T4 = K0 >> 8
	state.r[T4] = ((_u32)state.r[K0] >> 8);


_04001A2C:	// 4001a2c: <318cff00>	ANDI      T4 = T4 & ff00
	state.r[T4] = (state.r[T4] & 0x0000FF00);


_04001A30:	// 4001a30: <488c0c00>	MTC2      vec1 <08> = T4
	state.v[1].U16[3] = (_u16)state.r[T4];


_04001A34:	// 4001a34: <018b6020>	ADD       T4 = T4+T3
	state.r[T4] = (state.r[T4] + state.r[T3]);


_04001A38:	// 4001a38: <488c0d00>	MTC2      vec1 <10> = T4
	state.v[1].U16[2] = (_u16)state.r[T4];


_04001A3C:	// 4001a3c: <0019ac02>	SRL       S5 = T9 >> 16
	state.r[S5] = ((_u32)state.r[T9] >> 16);


_04001A40:	// 4001a40: <09000423>	J         0400108c
	state.r[S6] = (state.r[T9] & 0x0000FFFF);
	goto _0400108C;


_04001A44:	// 4001a44: <3336ffff>	ANDI      S6 = T9 & ffff
	state.r[S6] = (state.r[T9] & 0x0000FFFF);


_04001A48:	// 4001a48: <00196402>	SRL       T4 = T9 >> 16
	state.r[T4] = ((_u32)state.r[T9] >> 16);


_04001A4C:	// 4001a4c: <488c0800>	MTC2      vec1 <00> = T4
	state.v[1].U16[7] = (_u16)state.r[T4];


_04001A50:	// 4001a50: <01956020>	ADD       T4 = T4+S5
	state.r[T4] = (state.r[T4] + state.r[S5]);


_04001A54:	// 4001a54: <488c0900>	MTC2      vec1 <02> = T4
	state.v[1].U16[6] = (_u16)state.r[T4];


_04001A58:	// 4001a58: <332cffff>	ANDI      T4 = T9 & ffff
	state.r[T4] = (state.r[T9] & 0x0000FFFF);


_04001A5C:	// 4001a5c: <488c0a00>	MTC2      vec1 <04> = T4
	state.v[1].U16[5] = (_u16)state.r[T4];


_04001A60:	// 4001a60: <01966020>	ADD       T4 = T4+S6
	state.r[T4] = (state.r[T4] + state.r[S6]);


_04001A64:	// 4001a64: <09000423>	J         0400108c
	state.v[1].U16[4] = (_u16)state.r[T4];
	goto _0400108C;


_04001A68:	// 4001a68: <488c0b00>	MTC2      vec1 <06> = T4
	state.v[1].U16[4] = (_u16)state.r[T4];


_04001A6C:	// 4001a6c: <001a1b02>	SRL       V1 = K0 >> 12
	state.r[V1] = ((_u32)state.r[K0] >> 12);


_04001A70:	// 4001a70: <30630ff0>	ANDI      V1 = V1 & 0ff0
	state.r[V1] = (state.r[V1] & 0x00000FF0);


_04001A74:	// 4001a74: <3341ffff>	ANDI      AT = K0 & ffff
	state.r[AT] = (state.r[K0] & 0x0000FFFF);


_04001A78:	// 4001a78: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001A7C:	// 4001a7c: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[V0] = ((_u32)state.r[V0] >> 8);
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001A80:	// 4001a80: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001A84:	// 4001a84: <0d00069b>	JAL	    04001a6c
	sp_reg_pc = 0x04001A84;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001A6C;


_04001A88:	// 4001a88: <00000000>	NOP       


_04001A8C:	// 4001a8c: <090006ad>	J         04001ab4
	goto _04001AB4;


_04001A90:	// 4001a90: <00000000>	NOP       


_04001A94:	// 4001a94: <0d00069b>	JAL	    04001a6c
	sp_reg_pc = 0x04001A94;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001A6C;


_04001A98:	// 4001a98: <00000000>	NOP       


_04001A9C:	// 4001a9c: <090006b1>	J         04001ac4
	goto _04001AC4;


_04001AA0:	// 4001aa0: <00000000>	NOP       


_04001AA4:	// 4001aa4: <0d00069e>	JAL	    04001a78
	state.r[AT] = 0 + 0x0330;
	sp_reg_pc = 0x04001AA4;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001A78;


_04001AA8:	// 4001aa8: <20010330>	ADDI      AT = R0 + 0330
	state.r[AT] = 0 + 0x0330;


_04001AAC:	// 4001aac: <090006ad>	J         04001ab4
	state.r[V1] = (state.r[K0] & 0x0000FFFF);
	goto _04001AB4;


_04001AB0:	// 4001ab0: <3343ffff>	ANDI      V1 = K0 & ffff
	state.r[V1] = (state.r[K0] & 0x0000FFFF);


_04001AB4:	// 4001ab4: <0d0006b5>	JAL	    04001ad4
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x04001AB4;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_04001AB8:	// 4001ab8: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001ABC:	// 4001abc: <09000423>	J         0400108c
	goto _0400108C;


_04001AC0:	// 4001ac0: <00000000>	NOP       


_04001AC4:	// 4001ac4: <0d0006bf>	JAL	    04001afc
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x04001AC4;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AFC;


_04001AC8:	// 4001ac8: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001ACC:	// 4001acc: <09000423>	J         0400108c
	goto _0400108C;


_04001AD0:	// 4001ad0: <00000000>	NOP       


_04001AD4:	// 4001ad4: <40043800>	MFC0      A3 = SP status
	state.r[A0] = 0;


_04001AD8:	// 4001ad8: <1480fd86>	BNE       (R0!=A0) --> 10f4
	sp_reg_pc = 0x04001AD8;
	if (state.r[A0] != 0)
	{
		goto _040010F4;
	}


_04001ADC:	// 4001adc: <00000000>	NOP       


_04001AE0:	// 4001ae0: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001AE4:	// 4001ae4: <1480fffe>	BNE       (R0!=A0) --> 1ae0
	sp_reg_pc = 0x04001AE4;
	if (state.r[A0] != 0)
	{
		goto _04001AE0;
	}


_04001AE8:	// 4001ae8: <00000000>	NOP       


_04001AEC:	// 4001aec: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_04001AF0:	// 4001af0: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001AF4:	// 4001af4: <090006c8>	J         04001b20
	sp_reg_pc = 0x04001AF8;
	DMARead(state.r[V1]);
	goto _04001B20;


_04001AF8:	// 4001af8: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001AF8;
	DMARead(state.r[V1]);


_04001AFC:	// 4001afc: <40043800>	MFC0      A3 = SP status
	state.r[A0] = 0;


_04001B00:	// 4001b00: <1480fd87>	BNE       (R0!=A0) --> 1120
	sp_reg_pc = 0x04001B00;
	if (state.r[A0] != 0)
	{
		goto _04001120;
	}


_04001B04:	// 4001b04: <00000000>	NOP       


_04001B08:	// 4001b08: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001B0C:	// 4001b0c: <1480fffe>	BNE       (R0!=A0) --> 1b08
	sp_reg_pc = 0x04001B0C;
	if (state.r[A0] != 0)
	{
		goto _04001B08;
	}


_04001B10:	// 4001b10: <00000000>	NOP       


_04001B14:	// 4001b14: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_04001B18:	// 4001b18: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001B1C:	// 4001b1c: <40831800>	MTC0      SP write DMA length = V1
	DMAWrite(state.r[V1]);


_04001B20:	// 4001b20: <20040001>	ADDI      A0 = R0 + 0001
	state.r[A0] = 0 + 0x0001;


_04001B24:	// 4001b24: <1480ffff>	BNE       (R0!=A0) --> 1b24
	sp_reg_pc = 0x04001B24;
	if (state.r[A0] != 0)
	{
		state.r[A0] = 0;
		goto _04001B24;
	}


_04001B28:	// 4001b28: <40043000>	MFC0      A2 = SP status
	state.r[A0] = 0;


_04001B2C:	// 4001b2c: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001B30:	// 4001b30: <40803800>	MTC0      SP semaphore = R0


_04001B34:	// 4001b34: <c81f2000>	LQV       vec31 <00> = [R0 + 0x00]
	{
		_u32 addr = (0x00000000 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001B38:	// 4001b38: <001a9302>	SRL       S2 = K0 >> 12
	state.r[S2] = ((_u32)state.r[K0] >> 12);


_04001B3C:	// 4001b3c: <32520ff0>	ANDI      S2 = S2 & 0ff0
	state.r[S2] = (state.r[S2] & 0x00000FF0);


_04001B40:	// 4001b40: <3333ffff>	ANDI      S3 = T9 & ffff
	state.r[S3] = (state.r[T9] & 0x0000FFFF);


_04001B44:	// 4001b44: <0019a402>	SRL       S4 = T9 >> 16
	state.r[S4] = ((_u32)state.r[T9] >> 16);


_04001B48:	// 4001b48: <3351ffff>	ANDI      S1 = K0 & ffff
	state.r[S1] = (state.r[K0] & 0x0000FFFF);


_04001B4C:	// 4001b4c: <4891f000>	MTC2      vec30 <00> = S1
	state.v[30].U16[7] = (_u16)state.r[S1];


_04001B50:	// 4001b50: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_04001B54:	// 4001b54: <ca9d2000>	LQV       vec29 <00> = [S4 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[29].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[29].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[29].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[29].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[29].U64[0] = state.v[29].U64[0] & ~temp1;
			state.v[29].U64[1] = state.v[29].U64[1] & ~temp2;
			state.v[29].U64[0] = state.v[29].U64[0] | value1;
			state.v[29].U64[1] = state.v[29].U64[1] | value2;
		}
	}


_04001B58:	// 4001b58: <ca7a2001>	LQV       vec26 <00> = [S3 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[26].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[26].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[26].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[26].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[26].U64[0] = state.v[26].U64[0] & ~temp1;
			state.v[26].U64[1] = state.v[26].U64[1] & ~temp2;
			state.v[26].U64[0] = state.v[26].U64[0] | value1;
			state.v[26].U64[1] = state.v[26].U64[1] | value2;
		}
	}


_04001B5C:	// 4001b5c: <ca9c2001>	LQV       vec28 <00> = [S4 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[28].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[28].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[28].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[28].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[28].U64[0] = state.v[28].U64[0] & ~temp1;
			state.v[28].U64[1] = state.v[28].U64[1] & ~temp2;
			state.v[28].U64[0] = state.v[28].U64[0] | value1;
			state.v[28].U64[1] = state.v[28].U64[1] | value2;
		}
	}


_04001B60:	// 4001b60: <4bdfdec0>	VMULF     vec27 = ( acc = (vec27 * vec31[6]) << 1 ) >> 16

	// [start function: Zelda1B60_1B8C]

	Zelda1B60_1B8C();
	goto _04001B90;

_04001B64:	// 4001b64: <2252ffe0>	ADDI      S2 = S2 + ffe0

_04001B68:	// 4001b68: <4b1eeec8>	VMACF     vec27 = ( acc += (vec29 * vec30[0]) << 1 ) >> 16

_04001B6C:	// 4001b6c: <22940020>	ADDI      S4 = S4 + 0020

_04001B70:	// 4001b70: <4bdfd680>	VMULF     vec26 = ( acc = (vec26 * vec31[6]) << 1 ) >> 16

_04001B74:	// 4001b74: <4b1ee688>	VMACF     vec26 = ( acc += (vec28 * vec30[0]) << 1 ) >> 16

_04001B78:	// 4001b78: <ca9d2000>	LQV       vec29 <00> = [S4 + 0x00]

_04001B7C:	// 4001b7c: <ea7b2000>	SQV       [S3 + 0x00] = vec27 <00>

_04001B80:	// 4001b80: <ca7b2002>	LQV       vec27 <00> = [S3 + 0x20]

_04001B84:	// 4001b84: <ea7a2001>	SQV       [S3 + 0x10] = vec26 <00>

_04001B88:	// 4001b88: <ca9c2001>	LQV       vec28 <00> = [S4 + 0x10]

_04001B8C:	// 4001b8c: <22730020>	ADDI      S3 = S3 + 0020
	ErrorToCallFunction(0x04001B8C);

	// [end of function: Zelda1B60_1B8C]

_04001B90:	// 4001b90: <1e40fff3>	BGTZ      (R0>0) --> 1b60
	sp_reg_pc = 0x04001B90;
	if((_s32)state.r[S2] > 0)
	{
		{
			_u32 addr = (0x00000010 + state.r[S3]);
			int shifter = ((addr & 0xf) << 3);

			if (shifter == 0 && (addr&3) == 0 )
			{
				addr &= 0xfff;
				state.v[26].U32[0] = *(_u32 *)&pDMEM[addr+12];
				state.v[26].U32[1] = *(_u32 *)&pDMEM[addr+8];
				state.v[26].U32[2] = *(_u32 *)&pDMEM[addr+4];
				state.v[26].U32[3] = *(_u32 *)&pDMEM[addr];
			}
			else
			{
				_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
				_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
				_u64		value1 = Load64_DMEM(addr+8);
				_u64		value2 = Load64_DMEM(addr);
				if (shifter > 0x40)
				{
					temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
					temp1 = 0;
				} else {
					temp1 = ((temp1 >> shifter) << shifter);
				}
				value1 = value1 & temp1;
				value2 = value2 & temp2;

				state.v[26].U64[0] = state.v[26].U64[0] & ~temp1;
				state.v[26].U64[1] = state.v[26].U64[1] & ~temp2;
				state.v[26].U64[0] = state.v[26].U64[0] | value1;
				state.v[26].U64[1] = state.v[26].U64[1] | value2;
			}
		}
		goto _04001B60;
	}


_04001B94:	// 4001b94: <ca7a2001>	LQV       vec26 <00> = [S3 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[26].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[26].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[26].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[26].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[26].U64[0] = state.v[26].U64[0] & ~temp1;
			state.v[26].U64[1] = state.v[26].U64[1] & ~temp2;
			state.v[26].U64[0] = state.v[26].U64[0] | value1;
			state.v[26].U64[1] = state.v[26].U64[1] | value2;
		}
	}


_04001B98:	// 4001b98: <09000423>	J         0400108c
	goto _0400108C;


_04001B9C:	// 4001b9c: <00000000>	NOP       


_04001BA0:	// 4001ba0: <970d0000>	LHU       T5 = [T8+0000]
	state.r[T5] = (_s32)(_u16)Load16_DMEM((state.r[T8] + 0x00000000) & 0xfff);


_04001BA4:	// 4001ba4: <4a0210ac>	VXOR      vec02 = vec02 xor vec02[<none>]
	rsp_cop2_vxor(&state.v[2], &state.v[2], &state.v[2]);


_04001BA8:	// 4001ba8: <970e0002>	LHU       T6 = [T8+0002]
	state.r[T6] = (_s32)(_u16)Load16_DMEM((state.r[T8] + 0x00000002) & 0xfff);


_04001BAC:	// 4001bac: <4a0318ec>	VXOR      vec03 = vec03 xor vec03[<none>]
	rsp_cop2_vxor(&state.v[3], &state.v[3], &state.v[3]);


_04001BB0:	// 4001bb0: <970c0004>	LHU       T4 = [T8+0004]
	state.r[T4] = (_s32)(_u16)Load16_DMEM((state.r[T8] + 0x00000004) & 0xfff);


_04001BB4:	// 4001bb4: <00198a00>	SLL       S1 = T9 << 8
	state.r[S1] = ((_u32)state.r[T9] << 8);


_04001BB8:	// 4001bb8: <00118a02>	SRL       S1 = S1 >> 8
	state.r[S1] = ((_u32)state.r[S1] >> 8);


_04001BBC:	// 4001bbc: <e9c22000>	SQV       [T6 + 0x00] = vec02 <00>
	{
		_u32    addr = (0x00000000 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[2].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[2].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[2].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[2].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[2].U64[0] & temp1);
			value2 |= (state.v[2].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001BC0:	// 4001bc0: <e9c32001>	SQV       [T6 + 0x10] = vec03 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[3].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[3].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[3].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[3].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[3].U64[0] & temp1);
			value2 |= (state.v[3].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001BC4:	// 4001bc4: <001a0c02>	SRL       AT = K0 >> 16
	state.r[AT] = ((_u32)state.r[K0] >> 16);


_04001BC8:	// 4001bc8: <30210001>	ANDI      AT = AT & 0001
	state.r[AT] = (state.r[AT] & 0x00000001);


_04001BCC:	// 4001bcc: <1c200008>	BGTZ      (R0>0) --> 1bf0
	sp_reg_pc = 0x04001BCC;
	if((_s32)state.r[AT] > 0)
	{
		state.r[AT] = ((_u32)state.r[K0] >> 16);
		goto _04001BF0;
	}


_04001BD0:	// 4001bd0: <001a0c02>	SRL       AT = K0 >> 16
	state.r[AT] = ((_u32)state.r[K0] >> 16);


_04001BD4:	// 4001bd4: <30210002>	ANDI      AT = AT & 0002
	state.r[AT] = (state.r[AT] & 0x00000002);


_04001BD8:	// 4001bd8: <10010002>	BEQ       (AT==R0) --> 1be4
	sp_reg_pc = 0x04001BD8;
	if (0 == state.r[AT])
	{
		state.r[V0] = state.r[S1] + 0x0000;
		goto _04001BE4;
	}


_04001BDC:	// 4001bdc: <22220000>	ADDI      V0 = S1 + 0000
	state.r[V0] = state.r[S1] + 0x0000;


_04001BE0:	// 4001be0: <8f020008>	LW        V0 = [T8+0008]
	state.r[V0] = (_s32)Load32_DMEM((state.r[T8] + 0x00000008) & 0xfff);


_04001BE4:	// 4001be4: <21c10000>	ADDI      AT = T6 + 0000
	state.r[AT] = state.r[T6] + 0x0000;


_04001BE8:	// 4001be8: <0d0006b5>	JAL	    04001ad4
	state.r[V1] = 0 + 0x001F;
	sp_reg_pc = 0x04001BE8;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_04001BEC:	// 4001bec: <2003001f>	ADDI      V1 = R0 + 001f
	state.r[V1] = 0 + 0x001F;


_04001BF0:	// 4001bf0: <21ce0020>	ADDI      T6 = T6 + 0020
	state.r[T6] = state.r[T6] + 0x0020;


_04001BF4:	// 4001bf4: <1180000b>	BEQ       (R0==T4) --> 1c24
	sp_reg_pc = 0x04001BF4;
	if (state.r[T4] == 0)
	{
		goto _04001C24;
	}


_04001BF8:	// 4001bf8: <00000000>	NOP       


_04001BFC:	// 4001bfc: <c9a23000>	LPV
	rsp_lpv(0xC9A23000);


_04001C00:	// 4001c00: <c9a33001>	LPV
	rsp_lpv(0xC9A33001);


_04001C04:	// 4001c04: <21ad0010>	ADDI      T5 = T5 + 0010
	state.r[T5] = state.r[T5] + 0x0010;


_04001C08:	// 4001c08: <218cffe0>	ADDI      T4 = T4 + ffe0
	state.r[T4] = state.r[T4] + 0xFFFFFFE0;


_04001C0C:	// 4001c0c: <c9a43000>	LPV
	rsp_lpv(0xC9A43000);


_04001C10:	// 4001c10: <e9c22000>	SQV       [T6 + 0x00] = vec02 <00>
	{
		_u32    addr = (0x00000000 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[2].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[2].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[2].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[2].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[2].U64[0] & temp1);
			value2 |= (state.v[2].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001C14:	// 4001c14: <c9a53001>	LPV
	rsp_lpv(0xC9A53001);


_04001C18:	// 4001c18: <e9c32001>	SQV       [T6 + 0x10] = vec03 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[3].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[3].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[3].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[3].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[3].U64[0] & temp1);
			value2 |= (state.v[3].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001C1C:	// 4001c1c: <1d800005>	BGTZ      (R0>0) --> 1c34
	sp_reg_pc = 0x04001C1C;
	if((_s32)state.r[T4] > 0)
	{
		state.r[T6] = state.r[T6] + 0x0020;
		goto _04001C34;
	}


_04001C20:	// 4001c20: <21ce0020>	ADDI      T6 = T6 + 0020
	state.r[T6] = state.r[T6] + 0x0020;


_04001C24:	// 4001c24: <21c1ffe0>	ADDI      AT = T6 + ffe0
	state.r[AT] = state.r[T6] + 0xFFFFFFE0;


_04001C28:	// 4001c28: <22220000>	ADDI      V0 = S1 + 0000
	state.r[V0] = state.r[S1] + 0x0000;


_04001C2C:	// 4001c2c: <090006b1>	J         04001ac4
	state.r[V1] = 0 + 0x0020;
	goto _04001AC4;


_04001C30:	// 4001c30: <20030020>	ADDI      V1 = R0 + 0020
	state.r[V1] = 0 + 0x0020;


_04001C34:	// 4001c34: <218cffe0>	ADDI      T4 = T4 + ffe0
	state.r[T4] = state.r[T4] + 0xFFFFFFE0;


_04001C38:	// 4001c38: <c9a23002>	LPV
	rsp_lpv(0xC9A23002);


_04001C3C:	// 4001c3c: <e9c42000>	SQV       [T6 + 0x00] = vec04 <00>
	{
		_u32    addr = (0x00000000 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[4].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[4].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[4].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[4].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[4].U64[0] & temp1);
			value2 |= (state.v[4].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001C40:	// 4001c40: <c9a33003>	LPV
	rsp_lpv(0xC9A33003);


_04001C44:	// 4001c44: <e9c52001>	SQV       [T6 + 0x10] = vec05 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[5].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[5].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[5].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[5].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[5].U64[0] & temp1);
			value2 |= (state.v[5].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001C48:	// 4001c48: <21ad0020>	ADDI      T5 = T5 + 0020
	state.r[T5] = state.r[T5] + 0x0020;


_04001C4C:	// 4001c4c: <1d80ffee>	BGTZ      (R0>0) --> 1c08
	sp_reg_pc = 0x04001C4C;
	if((_s32)state.r[T4] > 0)
	{
		state.r[T6] = state.r[T6] + 0x0020;
		goto _04001C08;
	}


_04001C50:	// 4001c50: <21ce0020>	ADDI      T6 = T6 + 0020
	state.r[T6] = state.r[T6] + 0x0020;


_04001C54:	// 4001c54: <09000709>	J         04001c24
	goto _04001C24;


_04001C58:	// 4001c58: <00000000>	NOP       


_04001C5C:	// 4001c5c: <334cffff>	ANDI      T4 = K0 & ffff
	state.r[T4] = (state.r[K0] & 0x0000FFFF);


_04001C60:	// 4001c60: <00196c02>	SRL       T5 = T9 >> 16
	state.r[T5] = ((_u32)state.r[T9] >> 16);


_04001C64:	// 4001c64: <001a7902>	SRL       T7 = K0 >> 4
	state.r[T7] = ((_u32)state.r[K0] >> 4);


_04001C68:	// 4001c68: <31eff000>	ANDI      T7 = T7 & f000
	state.r[T7] = (state.r[T7] & 0x0000F000);


_04001C6C:	// 4001c6c: <488f1900>	MTC2      vec3 <02> = T7
	state.v[3].U16[6] = (_u16)state.r[T7];


_04001C70:	// 4001c70: <001a7d02>	SRL       T7 = K0 >> 20
	state.r[T7] = ((_u32)state.r[K0] >> 20);


_04001C74:	// 4001c74: <31ef000f>	ANDI      T7 = T7 & 000f
	state.r[T7] = (state.r[T7] & 0x0000000F);


_04001C78:	// 4001c78: <488f1800>	MTC2      vec3 <00> = T7
	state.v[3].U16[7] = (_u16)state.r[T7];


_04001C7C:	// 4001c7c: <c9a12000>	LQV       vec01 <00> = [T5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[1].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[1].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[1].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[1].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[1].U64[0] = state.v[1].U64[0] & ~temp1;
			state.v[1].U64[1] = state.v[1].U64[1] & ~temp2;
			state.v[1].U64[0] = state.v[1].U64[0] | value1;
			state.v[1].U64[1] = state.v[1].U64[1] | value2;
		}
	}


_04001C80:	// 4001c80: <c9a22001>	LQV       vec02 <00> = [T5 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[2].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[2].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[2].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[2].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[2].U64[0] = state.v[2].U64[0] & ~temp1;
			state.v[2].U64[1] = state.v[2].U64[1] & ~temp2;
			state.v[2].U64[0] = state.v[2].U64[0] | value1;
			state.v[2].U64[1] = state.v[2].U64[1] | value2;
		}
	}


_04001C84:	// 4001c84: <4b230905>	VMUDM     vec04 = ( acc = vec01 * vec03[1] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[1*16].U16[0], 0xE4
			MOVQ mm1, state.v[1*16].U16[4*2]
			PSHUFW mm4, state.v[3*16].U16[6*2], 0
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_04001C88:	// 4001c88: <4b03090f>	VMADH     vec04 = ( acc+= (vec01 * vec03[0]) << 16) >> 16
	{
		_u16		s2value = state.v[3].U16[7];
		VMADH_operation(0, state.v[4].U16[0], state.v[1].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[4].U16[1], state.v[1].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[4].U16[2], state.v[1].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[4].U16[3], state.v[1].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[4].U16[4], state.v[1].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[4].U16[5], state.v[1].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[4].U16[6], state.v[1].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[4].U16[7], state.v[1].U16[7], s2value, 1, 0, 0, 1)
	}


_04001C8C:	// 4001c8c: <4b231145>	VMUDM     vec05 = ( acc = vec02 * vec03[1] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[2*16].U16[0], 0xE4
			MOVQ mm1, state.v[2*16].U16[4*2]
			PSHUFW mm4, state.v[3*16].U16[6*2], 0
			PSHUFW mm6, mm0, 0xE4
			MOVQ mm7, mm1
			PMULLW mm6, mm4
			PMULLW mm7, mm4
			MOVQ accumulator[0*4], mm6
			MOVQ accumulator[2*4], mm7
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_04001C90:	// 4001c90: <4b03114f>	VMADH     vec05 = ( acc+= (vec02 * vec03[0]) << 16) >> 16
	{
		_u16		s2value = state.v[3].U16[7];
		VMADH_operation(0, state.v[5].U16[0], state.v[2].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[5].U16[1], state.v[2].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[5].U16[2], state.v[2].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[5].U16[3], state.v[2].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[5].U16[4], state.v[2].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[5].U16[5], state.v[2].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[5].U16[6], state.v[2].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[5].U16[7], state.v[2].U16[7], s2value, 1, 1, 0, 1)
	}


_04001C94:	// 4001c94: <e9a42000>	SQV       [T5 + 0x00] = vec04 <00>
	{
		_u32    addr = (0x00000000 + state.r[T5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[4].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[4].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[4].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[4].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[4].U64[0] & temp1);
			value2 |= (state.v[4].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001C98:	// 4001c98: <e9a52001>	SQV       [T5 + 0x10] = vec05 <00>
	{
		_u32    addr = (0x00000010 + state.r[T5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[5].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[5].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[5].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[5].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[5].U64[0] & temp1);
			value2 |= (state.v[5].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001C9C:	// 4001c9c: <218cffe0>	ADDI      T4 = T4 + ffe0
	state.r[T4] = state.r[T4] + 0xFFFFFFE0;


_04001CA0:	// 4001ca0: <1d80fff6>	BGTZ      (R0>0) --> 1c7c
	sp_reg_pc = 0x04001CA0;
	if((_s32)state.r[T4] > 0)
	{
		state.r[T5] = state.r[T5] + 0x0020;
		goto _04001C7C;
	}


_04001CA4:	// 4001ca4: <21ad0020>	ADDI      T5 = T5 + 0020
	state.r[T5] = state.r[T5] + 0x0020;


_04001CA8:	// 4001ca8: <09000423>	J         0400108c
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);
	goto _0400108C;


_04001CAC:	// 4001cac: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_04001CB0:	// 4001cb0: <22ed0000>	ADDI      T5 = S7 + 0000
	state.r[T5] = state.r[S7] + 0x0000;


_04001CB4:	// 4001cb4: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_04001CB8:	// 4001cb8: <22ee0020>	ADDI      T6 = S7 + 0020
	state.r[T6] = state.r[S7] + 0x0020;


_04001CBC:	// 4001cbc: <e9a02000>	SQV       [T5 + 0x00] = vec00 <00>
	{
		_u32    addr = (0x00000000 + state.r[T5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[0].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[0].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[0].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[0].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[0].U64[0] & temp1);
			value2 |= (state.v[0].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001CC0:	// 4001cc0: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001CC4:	// 4001cc4: <e9a02001>	SQV       [T5 + 0x10] = vec00 <00>
	{
		_u32    addr = (0x00000010 + state.r[T5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[0].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[0].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[0].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[0].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[0].U64[0] & temp1);
			value2 |= (state.v[0].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001CC8:	// 4001cc8: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001CCC:	// 4001ccc: <001a6402>	SRL       T4 = K0 >> 16
	state.r[T4] = ((_u32)state.r[K0] >> 16);


_04001CD0:	// 4001cd0: <318c00ff>	ANDI      T4 = T4 & 00ff
	state.r[T4] = (state.r[T4] & 0x000000FF);


_04001CD4:	// 4001cd4: <1180000b>	BEQ       (R0==T4) --> 1d04
	sp_reg_pc = 0x04001CD4;
	if (state.r[T4] == 0)
	{
		goto _04001D04;
	}


_04001CD8:	// 4001cd8: <00000000>	NOP       


_04001CDC:	// 4001cdc: <218cffff>	ADDI      T4 = T4 + ffff
	state.r[T4] = state.r[T4] + 0xFFFFFFFF;


_04001CE0:	// 4001ce0: <1180000a>	BEQ       (R0==T4) --> 1d0c
	sp_reg_pc = 0x04001CE0;
	if (state.r[T4] == 0)
	{
		goto _04001D0C;
	}


_04001CE4:	// 4001ce4: <00000000>	NOP       


_04001CE8:	// 4001ce8: <334fffff>	ANDI      T7 = K0 & ffff
	state.r[T7] = (state.r[K0] & 0x0000FFFF);


_04001CEC:	// 4001cec: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_04001CF0:	// 4001cf0: <e9c02000>	SQV       [T6 + 0x00] = vec00 <00>
	{
		_u32    addr = (0x00000000 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[0].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[0].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[0].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[0].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[0].U64[0] & temp1);
			value2 |= (state.v[0].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001CF4:	// 4001cf4: <e9c02002>	SQV       [T6 + 0x20] = vec00 <00>
	{
		_u32    addr = (0x00000020 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[0].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[0].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[0].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[0].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[0].U64[0] & temp1);
			value2 |= (state.v[0].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001CF8:	// 4001cf8: <21c10010>	ADDI      AT = T6 + 0010
	state.r[AT] = state.r[T6] + 0x0010;


_04001CFC:	// 4001cfc: <090006ad>	J         04001ab4
	state.r[V1] = 0 + 0x0010;
	goto _04001AB4;


_04001D00:	// 4001d00: <20030010>	ADDI      V1 = R0 + 0010
	state.r[V1] = 0 + 0x0010;


_04001D04:	// 4001d04: <01a00820>	ADD       AT = T5+R0
	state.r[AT] = (state.r[T5] + 0);


_04001D08:	// 4001d08: <0d0006b5>	JAL	    04001ad4
	state.r[V1] = 0 + 0x001F;
	sp_reg_pc = 0x04001D08;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_04001D0C:	// 4001d0c: <2003001f>	ADDI      V1 = R0 + 001f
	state.r[V1] = 0 + 0x001F;


_04001D10:	// 4001d10: <c9d82001>	LQV       vec24 <00> = [T6 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T6]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[24].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[24].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[24].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[24].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[24].U64[0] = state.v[24].U64[0] & ~temp1;
			state.v[24].U64[1] = state.v[24].U64[1] & ~temp2;
			state.v[24].U64[0] = state.v[24].U64[0] | value1;
			state.v[24].U64[1] = state.v[24].U64[1] | value2;
		}
	}


_04001D14:	// 4001d14: <c9b92001>	LQV       vec25 <00> = [T5 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[25].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[25].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[25].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[25].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[25].U64[0] = state.v[25].U64[0] & ~temp1;
			state.v[25].U64[1] = state.v[25].U64[1] & ~temp2;
			state.v[25].U64[0] = state.v[25].U64[0] | value1;
			state.v[25].U64[1] = state.v[25].U64[1] | value2;
		}
	}


_04001D18:	// 4001d18: <4a0e73ac>	VXOR      vec14 = vec14 xor vec14[<none>]
	rsp_cop2_vxor(&state.v[14], &state.v[14], &state.v[14]);


_04001D1C:	// 4001d1c: <4a000000>	VMULF     vec00 = ( acc = (vec00 * vec00[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[0].U16[0], state.v[0].U16[0], state.v[0].U16[0], 1, 1, 1, 1)
		VMULF_operation(1, state.v[0].U16[1], state.v[0].U16[1], state.v[0].U16[1], 1, 1, 1, 1)
		VMULF_operation(2, state.v[0].U16[2], state.v[0].U16[2], state.v[0].U16[2], 1, 1, 1, 1)
		VMULF_operation(3, state.v[0].U16[3], state.v[0].U16[3], state.v[0].U16[3], 1, 1, 1, 1)
		VMULF_operation(4, state.v[0].U16[4], state.v[0].U16[4], state.v[0].U16[4], 1, 1, 1, 1)
		VMULF_operation(5, state.v[0].U16[5], state.v[0].U16[5], state.v[0].U16[5], 1, 1, 1, 1)
		VMULF_operation(6, state.v[0].U16[6], state.v[0].U16[6], state.v[0].U16[6], 1, 1, 1, 1)
		VMULF_operation(7, state.v[0].U16[7], state.v[0].U16[7], state.v[0].U16[7], 1, 1, 1, 1)
	}


_04001D20:	// 4001d20: <200c4000>	ADDI      T4 = R0 + 4000
	state.r[T4] = 0 + 0x4000;


_04001D24:	// 4001d24: <488c7800>	MTC2      vec15 <00> = T4
	state.v[15].U16[7] = (_u16)state.r[T4];


_04001D28:	// 4001d28: <4b0fc388>	VMACF     vec14 = ( acc += (vec24 * vec15[0]) << 1 ) >> 16
	{
		_u16		s2value = state.v[15].U16[7];
		VMACF_operation(0, state.v[14].U16[0], state.v[24].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[24].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[24].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[24].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[24].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[24].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[24].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[24].U16[7], s2value, 0, 1, 1, 1)
	}


_04001D2C:	// 4001d2c: <4b0fcb88>	VMACF     vec14 = ( acc += (vec25 * vec15[0]) << 1 ) >> 16
	{
		_u16		s2value = state.v[15].U16[7];
		VMACF_operation(0, state.v[14].U16[0], state.v[25].U16[0], s2value, 1, 0, 0, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[25].U16[1], s2value, 1, 0, 0, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[25].U16[2], s2value, 1, 0, 0, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[25].U16[3], s2value, 1, 0, 0, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[25].U16[4], s2value, 1, 0, 0, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[25].U16[5], s2value, 1, 0, 0, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[25].U16[6], s2value, 1, 0, 0, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[25].U16[7], s2value, 1, 0, 0, 1)
	}


_04001D30:	// 4001d30: <e9ce2001>	SQV       [T6 + 0x10] = vec14 <00>
	{
		_u32    addr = (0x00000010 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[14].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[14].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[14].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[14].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[14].U64[0] & temp1);
			value2 |= (state.v[14].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001D34:	// 4001d34: <e9ae2001>	SQV       [T5 + 0x10] = vec14 <00>
	{
		_u32    addr = (0x00000010 + state.r[T5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[14].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[14].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[14].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[14].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[14].U64[0] & temp1);
			value2 |= (state.v[14].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001D38:	// 4001d38: <01a00820>	ADD       AT = T5+R0
	state.r[AT] = (state.r[T5] + 0);


_04001D3C:	// 4001d3c: <334bffff>	ANDI      T3 = K0 & ffff
	state.r[T3] = (state.r[K0] & 0x0000FFFF);


_04001D40:	// 4001d40: <c9d82001>	LQV       vec24 <00> = [T6 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T6]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[24].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[24].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[24].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[24].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[24].U64[0] = state.v[24].U64[0] & ~temp1;
			state.v[24].U64[1] = state.v[24].U64[1] & ~temp2;
			state.v[24].U64[0] = state.v[24].U64[0] | value1;
			state.v[24].U64[1] = state.v[24].U64[1] | value2;
		}
	}


_04001D44:	// 4001d44: <c9dc1801>	LDV       vec28 <00> = [T6 + 0x08]
	rsp_ldv_compiler((state.r[T6] + 0x00000008) & 0xFFF, 28, 8);


_04001D48:	// 4001d48: <c9dc1c02>	LDV       vec28 <08> = [T6 + 0x10]
	rsp_ldv_compiler((state.r[T6] + 0x00000010) & 0xFFF, 28, 0);


_04001D4C:	// 4001d4c: <c9d41803>	LDV       vec20 <00> = [T6 + 0x18]
	rsp_ldv_compiler((state.r[T6] + 0x00000018) & 0xFFF, 20, 8);


_04001D50:	// 4001d50: <c9d41c04>	LDV       vec20 <08> = [T6 + 0x20]
	rsp_ldv_compiler((state.r[T6] + 0x00000020) & 0xFFF, 20, 0);


_04001D54:	// 4001d54: <21ce0002>	ADDI      T6 = T6 + 0002
	state.r[T6] = state.r[T6] + 0x0002;


_04001D58:	// 4001d58: <c9df1800>	LDV       vec31 <00> = [T6 + 0x00]
	rsp_ldv_compiler((state.r[T6] & 0xFFF), 31, 8);


_04001D5C:	// 4001d5c: <c9df1c01>	LDV       vec31 <08> = [T6 + 0x08]
	rsp_ldv_compiler((state.r[T6] + 0x00000008) & 0xFFF, 31, 0);


_04001D60:	// 4001d60: <c9d11802>	LDV       vec17 <00> = [T6 + 0x10]
	rsp_ldv_compiler((state.r[T6] + 0x00000010) & 0xFFF, 17, 8);


_04001D64:	// 4001d64: <c9d11c03>	LDV       vec17 <08> = [T6 + 0x18]
	rsp_ldv_compiler((state.r[T6] + 0x00000018) & 0xFFF, 17, 0);


_04001D68:	// 4001d68: <c9db1801>	LDV       vec27 <00> = [T6 + 0x08]
	rsp_ldv_compiler((state.r[T6] + 0x00000008) & 0xFFF, 27, 8);


_04001D6C:	// 4001d6c: <c9db1c02>	LDV       vec27 <08> = [T6 + 0x10]
	rsp_ldv_compiler((state.r[T6] + 0x00000010) & 0xFFF, 27, 0);


_04001D70:	// 4001d70: <c9d51803>	LDV       vec21 <00> = [T6 + 0x18]
	rsp_ldv_compiler((state.r[T6] + 0x00000018) & 0xFFF, 21, 8);


_04001D74:	// 4001d74: <c9d51c04>	LDV       vec21 <08> = [T6 + 0x20]
	rsp_ldv_compiler((state.r[T6] + 0x00000020) & 0xFFF, 21, 0);


_04001D78:	// 4001d78: <21ce0002>	ADDI      T6 = T6 + 0002
	state.r[T6] = state.r[T6] + 0x0002;


_04001D7C:	// 4001d7c: <c9de1800>	LDV       vec30 <00> = [T6 + 0x00]
	rsp_ldv_compiler((state.r[T6] & 0xFFF), 30, 8);


_04001D80:	// 4001d80: <c9de1c01>	LDV       vec30 <08> = [T6 + 0x08]
	rsp_ldv_compiler((state.r[T6] + 0x00000008) & 0xFFF, 30, 0);


_04001D84:	// 4001d84: <c9da1801>	LDV       vec26 <00> = [T6 + 0x08]
	rsp_ldv_compiler((state.r[T6] + 0x00000008) & 0xFFF, 26, 8);


_04001D88:	// 4001d88: <c9da1c02>	LDV       vec26 <08> = [T6 + 0x10]
	rsp_ldv_compiler((state.r[T6] + 0x00000010) & 0xFFF, 26, 0);


_04001D8C:	// 4001d8c: <c9d21802>	LDV       vec18 <00> = [T6 + 0x10]
	rsp_ldv_compiler((state.r[T6] + 0x00000010) & 0xFFF, 18, 8);


_04001D90:	// 4001d90: <c9d21c03>	LDV       vec18 <08> = [T6 + 0x18]
	rsp_ldv_compiler((state.r[T6] + 0x00000018) & 0xFFF, 18, 0);


_04001D94:	// 4001d94: <c9d61803>	LDV       vec22 <00> = [T6 + 0x18]
	rsp_ldv_compiler((state.r[T6] + 0x00000018) & 0xFFF, 22, 8);


_04001D98:	// 4001d98: <c9d61c04>	LDV       vec22 <08> = [T6 + 0x20]
	rsp_ldv_compiler((state.r[T6] + 0x00000020) & 0xFFF, 22, 0);


_04001D9C:	// 4001d9c: <21ce0002>	ADDI      T6 = T6 + 0002
	state.r[T6] = state.r[T6] + 0x0002;


_04001DA0:	// 4001da0: <c9dd1800>	LDV       vec29 <00> = [T6 + 0x00]
	rsp_ldv_compiler((state.r[T6] & 0xFFF), 29, 8);


_04001DA4:	// 4001da4: <c9dd1c01>	LDV       vec29 <08> = [T6 + 0x08]
	rsp_ldv_compiler((state.r[T6] + 0x00000008) & 0xFFF, 29, 0);


_04001DA8:	// 4001da8: <c9d91801>	LDV       vec25 <00> = [T6 + 0x08]
	rsp_ldv_compiler((state.r[T6] + 0x00000008) & 0xFFF, 25, 8);


_04001DAC:	// 4001dac: <c9d91c02>	LDV       vec25 <08> = [T6 + 0x10]
	rsp_ldv_compiler((state.r[T6] + 0x00000010) & 0xFFF, 25, 0);


_04001DB0:	// 4001db0: <c9d31802>	LDV       vec19 <00> = [T6 + 0x10]
	rsp_ldv_compiler((state.r[T6] + 0x00000010) & 0xFFF, 19, 8);


_04001DB4:	// 4001db4: <c9d31c03>	LDV       vec19 <08> = [T6 + 0x18]
	rsp_ldv_compiler((state.r[T6] + 0x00000018) & 0xFFF, 19, 0);


_04001DB8:	// 4001db8: <c9d71803>	LDV       vec23 <00> = [T6 + 0x18]
	rsp_ldv_compiler((state.r[T6] + 0x00000018) & 0xFFF, 23, 8);


_04001DBC:	// 4001dbc: <c9d71c04>	LDV       vec23 <08> = [T6 + 0x20]
	rsp_ldv_compiler((state.r[T6] + 0x00000020) & 0xFFF, 23, 0);


_04001DC0:	// 4001dc0: <c9af2000>	LQV       vec15 <00> = [T5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[15].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[15].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[15].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[15].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[15].U64[0] = state.v[15].U64[0] & ~temp1;
			state.v[15].U64[1] = state.v[15].U64[1] & ~temp2;
			state.v[15].U64[0] = state.v[15].U64[0] | value1;
			state.v[15].U64[1] = state.v[15].U64[1] | value2;
		}
	}


_04001DC4:	// 4001dc4: <c9702000>	LQV       vec16 <00> = [T3 + 0x00]

	// [start function: Zelda1DC4_1E14]

	Zelda1DC4_1E14();
	goto _04001E18;

_04001DC8:	// 4001dc8: <4a0e73ac>	VXOR      vec14 = vec14 xor vec14[<none>]

_04001DCC:	// 4001dcc: <4a000000>	VMULF     vec00 = ( acc = (vec00 * vec00[<none>]) << 1 ) >> 16

_04001DD0:	// 4001dd0: <4b2fbb88>	VMACF     vec14 = ( acc += (vec23 * vec15[1]) << 1 ) >> 16

_04001DD4:	// 4001dd4: <4b4fb388>	VMACF     vec14 = ( acc += (vec22 * vec15[2]) << 1 ) >> 16

_04001DD8:	// 4001dd8: <4b6fab88>	VMACF     vec14 = ( acc += (vec21 * vec15[3]) << 1 ) >> 16

_04001DDC:	// 4001ddc: <4b8fa388>	VMACF     vec14 = ( acc += (vec20 * vec15[4]) << 1 ) >> 16

_04001DE0:	// 4001de0: <4baf9b88>	VMACF     vec14 = ( acc += (vec19 * vec15[5]) << 1 ) >> 16

_04001DE4:	// 4001de4: <4bcf9388>	VMACF     vec14 = ( acc += (vec18 * vec15[6]) << 1 ) >> 16

_04001DE8:	// 4001de8: <4bef8b88>	VMACF     vec14 = ( acc += (vec17 * vec15[7]) << 1 ) >> 16

_04001DEC:	// 4001dec: <4b10c388>	VMACF     vec14 = ( acc += (vec24 * vec16[0]) << 1 ) >> 16

_04001DF0:	// 4001df0: <4b30cb88>	VMACF     vec14 = ( acc += (vec25 * vec16[1]) << 1 ) >> 16

_04001DF4:	// 4001df4: <4b50d388>	VMACF     vec14 = ( acc += (vec26 * vec16[2]) << 1 ) >> 16

_04001DF8:	// 4001df8: <4b70db88>	VMACF     vec14 = ( acc += (vec27 * vec16[3]) << 1 ) >> 16

_04001DFC:	// 4001dfc: <4b90e388>	VMACF     vec14 = ( acc += (vec28 * vec16[4]) << 1 ) >> 16

_04001E00:	// 4001e00: <4bb0eb88>	VMACF     vec14 = ( acc += (vec29 * vec16[5]) << 1 ) >> 16

_04001E04:	// 4001e04: <4bd0f388>	VMACF     vec14 = ( acc += (vec30 * vec16[6]) << 1 ) >> 16

_04001E08:	// 4001e08: <4bf0fb88>	VMACF     vec14 = ( acc += (vec31 * vec16[7]) << 1 ) >> 16

_04001E0C:	// 4001e0c: <21effff0>	ADDI      T7 = T7 + fff0

_04001E10:	// 4001e10: <e96e2000>	SQV       [T3 + 0x00] = vec14 <00>

_04001E14:	// 4001e14: <216b0010>	ADDI      T3 = T3 + 0010
	ErrorToCallFunction(0x04001E14);

	// [end of function: Zelda1DC4_1E14]

_04001E18:	// 4001e18: <1de0ffea>	BGTZ      (R0>0) --> 1dc4
	sp_reg_pc = 0x04001E18;
	if((_s32)state.r[T7] > 0)
	{
		{
			_u16		flag0 = 0;
			VADDC_operation(0, state.v[15].U16[0], state.v[0].U16[0], state.v[16].U16[0], 1, 1, 1, 1)
			VADDC_operation(1, state.v[15].U16[1], state.v[0].U16[1], state.v[16].U16[1], 1, 1, 1, 1)
			VADDC_operation(2, state.v[15].U16[2], state.v[0].U16[2], state.v[16].U16[2], 1, 1, 1, 1)
			VADDC_operation(3, state.v[15].U16[3], state.v[0].U16[3], state.v[16].U16[3], 1, 1, 1, 1)
			VADDC_operation(4, state.v[15].U16[4], state.v[0].U16[4], state.v[16].U16[4], 1, 1, 1, 1)
			VADDC_operation(5, state.v[15].U16[5], state.v[0].U16[5], state.v[16].U16[5], 1, 1, 1, 1)
			VADDC_operation(6, state.v[15].U16[6], state.v[0].U16[6], state.v[16].U16[6], 1, 1, 1, 1)
			VADDC_operation(7, state.v[15].U16[7], state.v[0].U16[7], state.v[16].U16[7], 1, 1, 1, 1)
			state.flag[0] = flag0;
		}
		goto _04001DC4;
	}


_04001E1C:	// 4001e1c: <4a1003d4>	VADDC	vec15 = vec00 + vec16[<none>]
	{
		_u16		flag0 = 0;
		VADDC_operation(0, state.v[15].U16[0], state.v[0].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADDC_operation(1, state.v[15].U16[1], state.v[0].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADDC_operation(2, state.v[15].U16[2], state.v[0].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADDC_operation(3, state.v[15].U16[3], state.v[0].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADDC_operation(4, state.v[15].U16[4], state.v[0].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADDC_operation(5, state.v[15].U16[5], state.v[0].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADDC_operation(6, state.v[15].U16[6], state.v[0].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADDC_operation(7, state.v[15].U16[7], state.v[0].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = flag0;
	}


_04001E20:	// 4001e20: <e9b02000>	SQV       [T5 + 0x00] = vec16 <00>
	{
		_u32    addr = (0x00000000 + state.r[T5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[16].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[16].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[16].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[16].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[16].U64[0] & temp1);
			value2 |= (state.v[16].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001E24:	// 4001e24: <090006b1>	J         04001ac4
	state.r[V1] = 0 + 0x001F;
	goto _04001AC4;


_04001E28:	// 4001e28: <2003001f>	ADDI      V1 = R0 + 001f
	state.r[V1] = 0 + 0x001F;


_04001E2C:	// 4001e2c: <4a1fffd4>	VADDC	vec31 = vec31 + vec31[<none>]
	{
		_u16		flag0 = 0;
		VADDC_operation(0, state.v[31].U16[0], state.v[31].U16[0], state.v[31].U16[0], 1, 0, 0, 1)
		VADDC_operation(1, state.v[31].U16[1], state.v[31].U16[1], state.v[31].U16[1], 1, 0, 0, 1)
		VADDC_operation(2, state.v[31].U16[2], state.v[31].U16[2], state.v[31].U16[2], 1, 0, 0, 1)
		VADDC_operation(3, state.v[31].U16[3], state.v[31].U16[3], state.v[31].U16[3], 1, 0, 0, 1)
		VADDC_operation(4, state.v[31].U16[4], state.v[31].U16[4], state.v[31].U16[4], 1, 0, 0, 1)
		VADDC_operation(5, state.v[31].U16[5], state.v[31].U16[5], state.v[31].U16[5], 1, 0, 0, 1)
		VADDC_operation(6, state.v[31].U16[6], state.v[31].U16[6], state.v[31].U16[6], 1, 0, 0, 1)
		VADDC_operation(7, state.v[31].U16[7], state.v[31].U16[7], state.v[31].U16[7], 1, 0, 0, 1)
		state.flag[0] = flag0;
	}


_04001E30:	// 4001e30: <001a9302>	SRL       S2 = K0 >> 12
	state.r[S2] = ((_u32)state.r[K0] >> 12);


_04001E34:	// 4001e34: <32520ff0>	ANDI      S2 = S2 & 0ff0
	state.r[S2] = (state.r[S2] & 0x00000FF0);


_04001E38:	// 4001e38: <3333ffff>	ANDI      S3 = T9 & ffff
	state.r[S3] = (state.r[T9] & 0x0000FFFF);


_04001E3C:	// 4001e3c: <0019a402>	SRL       S4 = T9 >> 16
	state.r[S4] = ((_u32)state.r[T9] >> 16);


_04001E40:	// 4001e40: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_04001E44:	// 4001e44: <ca9d2000>	LQV       vec29 <00> = [S4 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[29].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[29].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[29].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[29].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[29].U64[0] = state.v[29].U64[0] & ~temp1;
			state.v[29].U64[1] = state.v[29].U64[1] & ~temp2;
			state.v[29].U64[0] = state.v[29].U64[0] | value1;
			state.v[29].U64[1] = state.v[29].U64[1] | value2;
		}
	}


_04001E48:	// 4001e48: <ca7a2001>	LQV       vec26 <00> = [S3 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[26].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[26].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[26].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[26].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[26].U64[0] = state.v[26].U64[0] & ~temp1;
			state.v[26].U64[1] = state.v[26].U64[1] & ~temp2;
			state.v[26].U64[0] = state.v[26].U64[0] | value1;
			state.v[26].U64[1] = state.v[26].U64[1] | value2;
		}
	}


_04001E4C:	// 4001e4c: <ca9c2001>	LQV       vec28 <00> = [S4 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[28].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[28].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[28].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[28].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[28].U64[0] = state.v[28].U64[0] & ~temp1;
			state.v[28].U64[1] = state.v[28].U64[1] & ~temp2;
			state.v[28].U64[0] = state.v[28].U64[0] | value1;
			state.v[28].U64[1] = state.v[28].U64[1] | value2;
		}
	}


_04001E50:	// 4001e50: <ca792002>	LQV       vec25 <00> = [S3 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[25].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[25].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[25].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[25].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[25].U64[0] = state.v[25].U64[0] & ~temp1;
			state.v[25].U64[1] = state.v[25].U64[1] & ~temp2;
			state.v[25].U64[0] = state.v[25].U64[0] | value1;
			state.v[25].U64[1] = state.v[25].U64[1] | value2;
		}
	}


_04001E54:	// 4001e54: <ca972002>	LQV       vec23 <00> = [S4 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[23].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[23].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[23].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[23].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[23].U64[0] = state.v[23].U64[0] & ~temp1;
			state.v[23].U64[1] = state.v[23].U64[1] & ~temp2;
			state.v[23].U64[0] = state.v[23].U64[0] | value1;
			state.v[23].U64[1] = state.v[23].U64[1] | value2;
		}
	}


_04001E58:	// 4001e58: <ca782003>	LQV       vec24 <00> = [S3 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[24].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[24].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[24].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[24].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[24].U64[0] = state.v[24].U64[0] & ~temp1;
			state.v[24].U64[1] = state.v[24].U64[1] & ~temp2;
			state.v[24].U64[0] = state.v[24].U64[0] | value1;
			state.v[24].U64[1] = state.v[24].U64[1] | value2;
		}
	}


_04001E5C:	// 4001e5c: <ca962003>	LQV       vec22 <00> = [S4 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[22].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[22].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[22].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[22].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[22].U64[0] = state.v[22].U64[0] & ~temp1;
			state.v[22].U64[1] = state.v[22].U64[1] & ~temp2;
			state.v[22].U64[0] = state.v[22].U64[0] | value1;
			state.v[22].U64[1] = state.v[22].U64[1] | value2;
		}
	}


_04001E60:	// 4001e60: <22940040>	ADDI      S4 = S4 + 0040
	state.r[S4] = state.r[S4] + 0x0040;


_04001E64:	// 4001e64: <4a1dded0>	VADD      vec27 = vec27 + vec29[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[27].U16[0], state.v[27].U16[0], state.v[29].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[27].U16[1], state.v[27].U16[1], state.v[29].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[27].U16[2], state.v[27].U16[2], state.v[29].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[27].U16[3], state.v[27].U16[3], state.v[29].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[27].U16[4], state.v[27].U16[4], state.v[29].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[27].U16[5], state.v[27].U16[5], state.v[29].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[27].U16[6], state.v[27].U16[6], state.v[29].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[27].U16[7], state.v[27].U16[7], state.v[29].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001E68:	// 4001e68: <4a1cd690>	VADD      vec26 = vec26 + vec28[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[26].U16[0], state.v[26].U16[0], state.v[28].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[26].U16[1], state.v[26].U16[1], state.v[28].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[26].U16[2], state.v[26].U16[2], state.v[28].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[26].U16[3], state.v[26].U16[3], state.v[28].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[26].U16[4], state.v[26].U16[4], state.v[28].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[26].U16[5], state.v[26].U16[5], state.v[28].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[26].U16[6], state.v[26].U16[6], state.v[28].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[26].U16[7], state.v[26].U16[7], state.v[28].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001E6C:	// 4001e6c: <4a17ce50>	VADD      vec25 = vec25 + vec23[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[25].U16[0], state.v[25].U16[0], state.v[23].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[25].U16[1], state.v[25].U16[1], state.v[23].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[25].U16[2], state.v[25].U16[2], state.v[23].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[25].U16[3], state.v[25].U16[3], state.v[23].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[25].U16[4], state.v[25].U16[4], state.v[23].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[25].U16[5], state.v[25].U16[5], state.v[23].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[25].U16[6], state.v[25].U16[6], state.v[23].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[25].U16[7], state.v[25].U16[7], state.v[23].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001E70:	// 4001e70: <4a16c610>	VADD      vec24 = vec24 + vec22[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[24].U16[0], state.v[24].U16[0], state.v[22].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[24].U16[1], state.v[24].U16[1], state.v[22].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[24].U16[2], state.v[24].U16[2], state.v[22].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[24].U16[3], state.v[24].U16[3], state.v[22].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[24].U16[4], state.v[24].U16[4], state.v[22].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[24].U16[5], state.v[24].U16[5], state.v[22].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[24].U16[6], state.v[24].U16[6], state.v[22].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[24].U16[7], state.v[24].U16[7], state.v[22].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001E74:	// 4001e74: <2252ffc0>	ADDI      S2 = S2 + ffc0
	state.r[S2] = state.r[S2] + 0xFFFFFFC0;


_04001E78:	// 4001e78: <ea7b2000>	SQV       [S3 + 0x00] = vec27 <00>
	{
		_u32    addr = (0x00000000 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[27].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[27].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[27].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[27].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[27].U64[0] & temp1);
			value2 |= (state.v[27].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001E7C:	// 4001e7c: <ea7a2001>	SQV       [S3 + 0x10] = vec26 <00>
	{
		_u32    addr = (0x00000010 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[26].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[26].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[26].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[26].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[26].U64[0] & temp1);
			value2 |= (state.v[26].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001E80:	// 4001e80: <ea792002>	SQV       [S3 + 0x20] = vec25 <00>
	{
		_u32    addr = (0x00000020 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[25].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[25].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[25].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[25].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[25].U64[0] & temp1);
			value2 |= (state.v[25].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001E84:	// 4001e84: <ea782003>	SQV       [S3 + 0x30] = vec24 <00>
	{
		_u32    addr = (0x00000030 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[24].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[24].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[24].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[24].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[24].U64[0] & temp1);
			value2 |= (state.v[24].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001E88:	// 4001e88: <22730040>	ADDI      S3 = S3 + 0040
	state.r[S3] = state.r[S3] + 0x0040;


_04001E8C:	// 4001e8c: <1e40ffed>	BGTZ      (R0>0) --> 1e44
	sp_reg_pc = 0x04001E8C;
	if((_s32)state.r[S2] > 0)
	{
		{
			_u32 addr = (0x00000000 + state.r[S3]);
			int shifter = ((addr & 0xf) << 3);

			if (shifter == 0 && (addr&3) == 0 )
			{
				addr &= 0xfff;
				state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
				state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
				state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
				state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
			}
			else
			{
				_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
				_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
				_u64		value1 = Load64_DMEM(addr+8);
				_u64		value2 = Load64_DMEM(addr);
				if (shifter > 0x40)
				{
					temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
					temp1 = 0;
				} else {
					temp1 = ((temp1 >> shifter) << shifter);
				}
				value1 = value1 & temp1;
				value2 = value2 & temp2;

				state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
				state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
				state.v[27].U64[0] = state.v[27].U64[0] | value1;
				state.v[27].U64[1] = state.v[27].U64[1] | value2;
			}
		}
		goto _04001E44;
	}


_04001E90:	// 4001e90: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_04001E94:	// 4001e94: <09000423>	J         0400108c
	goto _0400108C;


_04001E98:	// 4001e98: <00000000>	NOP       


_04001E9C:	// 4001e9c: <870e0000>	LH        T6 = [T8+0000]
	state.r[T6] = (_s32)(_s16)Load16_DMEM((state.r[T8] + 0x00000000) & 0xfff);


_04001EA0:	// 4001ea0: <870f0002>	LH        T7 = [T8+0002]
	state.r[T7] = (_s32)(_s16)Load16_DMEM((state.r[T8] + 0x00000002) & 0xfff);


_04001EA4:	// 4001ea4: <870d0004>	LH        T5 = [T8+0004]
	state.r[T5] = (_s32)(_s16)Load16_DMEM((state.r[T8] + 0x00000004) & 0xfff);


_04001EA8:	// 4001ea8: <334cffff>	ANDI      T4 = K0 & ffff
	state.r[T4] = (state.r[K0] & 0x0000FFFF);


_04001EAC:	// 4001eac: <000c6080>	SLL       T4 = T4 << 2
	state.r[T4] = ((_u32)state.r[T4] << 2);


_04001EB0:	// 4001eb0: <332affff>	ANDI      T2 = T9 & ffff
	state.r[T2] = (state.r[T9] & 0x0000FFFF);


_04001EB4:	// 4001eb4: <000e7400>	SLL       T6 = T6 << 16
	state.r[T6] = ((_u32)state.r[T6] << 16);


_04001EB8:	// 4001eb8: <014e5025>	OR        T2 = T2 | T6
	state.r[T2] = (state.r[T2] | state.r[T6]);


_04001EBC:	// 4001ebc: <000a5c02>	SRL       T3 = T2 >> 16
	state.r[T3] = ((_u32)state.r[T2] >> 16);


_04001EC0:	// 4001ec0: <316bfffe>	ANDI      T3 = T3 & fffe
	state.r[T3] = (state.r[T3] & 0x0000FFFE);


_04001EC4:	// 4001ec4: <c9610800>	LSV       vec01 <00> = [T3 + 0x00]
	state.v[1].U16[7] = Load16_DMEM(0x00000000 + state.r[T3]);


_04001EC8:	// 4001ec8: <014c5020>	ADD       T2 = T2+T4
	state.r[T2] = (state.r[T2] + state.r[T4]);


_04001ECC:	// 4001ecc: <000a5c02>	SRL       T3 = T2 >> 16
	state.r[T3] = ((_u32)state.r[T2] >> 16);


_04001ED0:	// 4001ed0: <316bfffe>	ANDI      T3 = T3 & fffe
	state.r[T3] = (state.r[T3] & 0x0000FFFE);


_04001ED4:	// 4001ed4: <c9610900>	LSV       vec01 <02> = [T3 + 0x00]
	state.v[1].U16[6] = Load16_DMEM(0x00000000 + state.r[T3]);


_04001ED8:	// 4001ed8: <014c5020>	ADD       T2 = T2+T4
	state.r[T2] = (state.r[T2] + state.r[T4]);


_04001EDC:	// 4001edc: <000a5c02>	SRL       T3 = T2 >> 16
	state.r[T3] = ((_u32)state.r[T2] >> 16);


_04001EE0:	// 4001ee0: <316bfffe>	ANDI      T3 = T3 & fffe
	state.r[T3] = (state.r[T3] & 0x0000FFFE);


_04001EE4:	// 4001ee4: <c9610a00>	LSV       vec01 <04> = [T3 + 0x00]
	state.v[1].U16[5] = Load16_DMEM(0x00000000 + state.r[T3]);


_04001EE8:	// 4001ee8: <014c5020>	ADD       T2 = T2+T4
	state.r[T2] = (state.r[T2] + state.r[T4]);


_04001EEC:	// 4001eec: <000a5c02>	SRL       T3 = T2 >> 16
	state.r[T3] = ((_u32)state.r[T2] >> 16);


_04001EF0:	// 4001ef0: <316bfffe>	ANDI      T3 = T3 & fffe
	state.r[T3] = (state.r[T3] & 0x0000FFFE);


_04001EF4:	// 4001ef4: <c9610b00>	LSV       vec01 <06> = [T3 + 0x00]
	state.v[1].U16[4] = Load16_DMEM(0x00000000 + state.r[T3]);


_04001EF8:	// 4001ef8: <014c5020>	ADD       T2 = T2+T4
	state.r[T2] = (state.r[T2] + state.r[T4]);


_04001EFC:	// 4001efc: <21adfff8>	ADDI      T5 = T5 + fff8
	state.r[T5] = state.r[T5] + 0xFFFFFFF8;


_04001F00:	// 4001f00: <e9e11800>	SDV       [T7 + 0x00] = vec01 <00>
	{
		_u32	addr = (0x00000000 + state.r[T7]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[1].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[1].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[1].U32[0x00000003];
		}
	}


_04001F04:	// 4001f04: <21ef0008>	ADDI      T7 = T7 + 0008
	state.r[T7] = state.r[T7] + 0x0008;


_04001F08:	// 4001f08: <1da0ffec>	BGTZ      (R0>0) --> 1ebc
	sp_reg_pc = 0x04001F08;
	if((_s32)state.r[T5] > 0)
	{
		goto _04001EBC;
	}


_04001F0C:	// 4001f0c: <00000000>	NOP       


_04001F10:	// 4001f10: <0d000423>	JAL	    0400108c
	sp_reg_pc = 0x04001F10;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400108C;


_04001F14:	// 4001f14: <00000000>	NOP       


_04001F18:	// 4001f18: <c81f2000>	LQV       vec31 <00> = [R0 + 0x00]
	{
		_u32 addr = (0x00000000 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001F1C:	// 4001f1c: <3352ffff>	ANDI      S2 = K0 & ffff
	state.r[S2] = (state.r[K0] & 0x0000FFFF);


_04001F20:	// 4001f20: <3333ffff>	ANDI      S3 = T9 & ffff
	state.r[S3] = (state.r[T9] & 0x0000FFFF);


_04001F24:	// 4001f24: <0019a402>	SRL       S4 = T9 >> 16
	state.r[S4] = ((_u32)state.r[T9] >> 16);


_04001F28:	// 4001f28: <ca9d2000>	LQV       vec29 <00> = [S4 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[29].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[29].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[29].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[29].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[29].U64[0] = state.v[29].U64[0] & ~temp1;
			state.v[29].U64[1] = state.v[29].U64[1] & ~temp2;
			state.v[29].U64[0] = state.v[29].U64[0] | value1;
			state.v[29].U64[1] = state.v[29].U64[1] | value2;
		}
	}


_04001F2C:	// 4001f2c: <ca9c2001>	LQV       vec28 <00> = [S4 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[28].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[28].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[28].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[28].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[28].U64[0] = state.v[28].U64[0] & ~temp1;
			state.v[28].U64[1] = state.v[28].U64[1] & ~temp2;
			state.v[28].U64[0] = state.v[28].U64[0] | value1;
			state.v[28].U64[1] = state.v[28].U64[1] | value2;
		}
	}


_04001F30:	// 4001f30: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_04001F34:	// 4001f34: <ca7a2001>	LQV       vec26 <00> = [S3 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[26].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[26].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[26].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[26].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[26].U64[0] = state.v[26].U64[0] & ~temp1;
			state.v[26].U64[1] = state.v[26].U64[1] & ~temp2;
			state.v[26].U64[0] = state.v[26].U64[0] | value1;
			state.v[26].U64[1] = state.v[26].U64[1] | value2;
		}
	}


_04001F38:	// 4001f38: <2252ffe0>	ADDI      S2 = S2 + ffe0
	state.r[S2] = state.r[S2] + 0xFFFFFFE0;


_04001F3C:	// 4001f3c: <22940020>	ADDI      S4 = S4 + 0020
	state.r[S4] = state.r[S4] + 0x0020;


_04001F40:	// 4001f40: <4a1de6c6>	VMUDN     vec27 = ( acc = vec28 * vec29[<none>]      ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[28*16].U16[0], 0xE4
			MOVQ mm1, state.v[28*16].U16[4*2]
			PSHUFW mm4, state.v[29*16].U16[0], 0xE4
			MOVQ mm5, state.v[29*16].U16[4*2]
			PSHUFW mm6, mm0, 0xE4
			MOVQ mm7, mm1
			PMULLW mm6, mm4
			PMULLW mm7, mm5
			MOVQ accumulator[0*4], mm6
			MOVQ accumulator[2*4], mm7
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm2, mm4
			PMULHUW mm3, mm5
			PSRAW mm0, 15
			PSRAW mm1, 15
			PMULLW mm0, mm4
			PMULLW mm1, mm5
			PADDW mm0, mm2
			PADDW mm1, mm3
			MOVQ state.v[27*16].U16[0], mm0
			MOVQ state.v[27*16].U16[4*2], mm1
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_04001F44:	// 4001f44: <4a1de68e>	VMADN     vec26 = ( acc+= vec28 * vec29[<none>] )
	{
		VMADN_operation(0, state.v[26].U16[0], state.v[28].U16[0], state.v[29].U16[0], 1, 1, 1, 1)
		VMADN_operation(1, state.v[26].U16[1], state.v[28].U16[1], state.v[29].U16[1], 1, 1, 1, 1)
		VMADN_operation(2, state.v[26].U16[2], state.v[28].U16[2], state.v[29].U16[2], 1, 1, 1, 1)
		VMADN_operation(3, state.v[26].U16[3], state.v[28].U16[3], state.v[29].U16[3], 1, 1, 1, 1)
		VMADN_operation(4, state.v[26].U16[4], state.v[28].U16[4], state.v[29].U16[4], 1, 1, 1, 1)
		VMADN_operation(5, state.v[26].U16[5], state.v[28].U16[5], state.v[29].U16[5], 1, 1, 1, 1)
		VMADN_operation(6, state.v[26].U16[6], state.v[28].U16[6], state.v[29].U16[6], 1, 1, 1, 1)
		VMADN_operation(7, state.v[26].U16[7], state.v[28].U16[7], state.v[29].U16[7], 1, 1, 1, 1)
	}


_04001F48:	// 4001f48: <ea7b2001>	SQV       [S3 + 0x10] = vec27 <00>
	{
		_u32    addr = (0x00000010 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[27].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[27].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[27].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[27].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[27].U64[0] & temp1);
			value2 |= (state.v[27].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001F4C:	// 4001f4c: <ea7a2000>	SQV       [S3 + 0x00] = vec26 <00>
	{
		_u32    addr = (0x00000000 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[26].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[26].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[26].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[26].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[26].U64[0] & temp1);
			value2 |= (state.v[26].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001F50:	// 4001f50: <ca9d2000>	LQV       vec29 <00> = [S4 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[29].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[29].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[29].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[29].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[29].U64[0] = state.v[29].U64[0] & ~temp1;
			state.v[29].U64[1] = state.v[29].U64[1] & ~temp2;
			state.v[29].U64[0] = state.v[29].U64[0] | value1;
			state.v[29].U64[1] = state.v[29].U64[1] | value2;
		}
	}


_04001F54:	// 4001f54: <ca9c2001>	LQV       vec28 <00> = [S4 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[28].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[28].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[28].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[28].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[28].U64[0] = state.v[28].U64[0] & ~temp1;
			state.v[28].U64[1] = state.v[28].U64[1] & ~temp2;
			state.v[28].U64[0] = state.v[28].U64[0] | value1;
			state.v[28].U64[1] = state.v[28].U64[1] | value2;
		}
	}


_04001F58:	// 4001f58: <1e40fff7>	BGTZ      (R0>0) --> 1f38
	sp_reg_pc = 0x04001F58;
	if((_s32)state.r[S2] > 0)
	{
		state.r[S3] = state.r[S3] + 0x0020;
		goto _04001F38;
	}


_04001F5C:	// 4001f5c: <22730020>	ADDI      S3 = S3 + 0020
	state.r[S3] = state.r[S3] + 0x0020;


_04001F60:	// 4001f60: <09000423>	J         0400108c
	goto _0400108C;


_04001F64:	// 4001f64: <00000000>	NOP       


_04001F68:	// 4001f68: <0d00069b>	JAL	    04001a6c
	sp_reg_pc = 0x04001F68;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001A6C;


_04001F6C:	// 4001f6c: <00000000>	NOP       


_04001F70:	// 4001f70: <22e10000>	ADDI      AT = S7 + 0000
	state.r[AT] = state.r[S7] + 0x0000;


_04001F74:	// 4001f74: <0d0006b5>	JAL	    04001ad4
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x04001F74;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AD4;


_04001F78:	// 4001f78: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001F7C:	// 4001f7c: <8c260004>	LW        A2 = [AT+0004]
	state.r[A2] = (_s32)Load32_DMEM((state.r[AT] + 0x00000004) & 0xfff);


_04001F80:	// 4001f80: <20c60001>	ADDI      A2 = A2 + 0001
	state.r[A2] = state.r[A2] + 0x0001;


_04001F84:	// 4001f84: <ac260004>	SW        [AT+0004] = A2
	Save32_DMEM((_u32)state.r[A2], (state.r[AT] + 0x00000004) & 0xfff);


_04001F88:	// 4001f88: <ac3b0008>	SW        [AT+0008] = K1
	Save32_DMEM((_u32)state.r[K1], (state.r[AT] + 0x00000008) & 0xfff);


_04001F8C:	// 4001f8c: <ac3e000c>	SW        [AT+000c] = S8
	Save32_DMEM((_u32)state.r[S8], (state.r[AT] + 0x0000000C) & 0xfff);


_04001F90:	// 4001f90: <3346ffff>	ANDI      A2 = K0 & ffff
	state.r[A2] = (state.r[K0] & 0x0000FFFF);


_04001F94:	// 4001f94: <0d0006bf>	JAL	    04001afc
	Save16_DMEM((_u16)state.r[A2], (state.r[AT] + 0x00000000) & 0xfff);
	sp_reg_pc = 0x04001F94;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AFC;


_04001F98:	// 4001f98: <a4260000>	SH        [AT+0000] = A2
	Save16_DMEM((_u16)state.r[A2], (state.r[AT] + 0x00000000) & 0xfff);


_04001F9C:	// 4001f9c: <09000423>	J         0400108c
	goto _0400108C;


_04001FA0:	// 4001fa0: <00000000>	NOP       


_04001FA4:	// 4001fa4: <00000000>	NOP       


_04001FA8:	// 4001fa8: <00000000>	NOP       


_04001FAC:	// 4001fac: <00000000>	NOP       


_04001FB0:	// 4001fb0: <4a10802a>	VOR       vec00 = vec16 or vec16[<none>]
	rsp_cop2_vor(&state.v[0], &state.v[16], &state.v[16]);


_04001FB4:	// 4001fb4: <c81f201b>	LQV       vec31 <00> = [R0 + 0xb0]
	{
		_u32 addr = (0x000001B0 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001FB8:	// 4001fb8: <c81e201c>	LQV       vec30 <00> = [R0 + 0xc0]
	{
		_u32 addr = (0x000001C0 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[30].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[30].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[30].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[30].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[30].U64[0] = state.v[30].U64[0] & ~temp1;
			state.v[30].U64[1] = state.v[30].U64[1] & ~temp2;
			state.v[30].U64[0] = state.v[30].U64[0] | value1;
			state.v[30].U64[1] = state.v[30].U64[1] | value2;
		}
	}


_04001FBC:	// 4001fbc: <20170ba8>	ADDI      S7 = R0 + 0ba8
	state.r[S7] = 0 + 0x0BA8;


_04001FC0:	// 4001fc0: <20160d00>	ADDI      S6 = R0 + 0d00
	state.r[S6] = 0 + 0x0D00;


_04001FC4:	// 4001fc4: <4b1f0051>	VSUB      vec01 = vec00 - vec31[0]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[31].U16[7];
		VSUB_operation(0, state.v[1].U16[0], state.v[0].U16[0], s2value, 1, 0, 0, 1)
		VSUB_operation(1, state.v[1].U16[1], state.v[0].U16[1], s2value, 1, 0, 0, 1)
		VSUB_operation(2, state.v[1].U16[2], state.v[0].U16[2], s2value, 1, 0, 0, 1)
		VSUB_operation(3, state.v[1].U16[3], state.v[0].U16[3], s2value, 1, 0, 0, 1)
		VSUB_operation(4, state.v[1].U16[4], state.v[0].U16[4], s2value, 1, 0, 0, 1)
		VSUB_operation(5, state.v[1].U16[5], state.v[0].U16[5], s2value, 1, 0, 0, 1)
		VSUB_operation(6, state.v[1].U16[6], state.v[0].U16[6], s2value, 1, 0, 0, 1)
		VSUB_operation(7, state.v[1].U16[7], state.v[0].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001FC8:	// 4001fc8: <8c0b00f0>	LW        T3 = [R0+00f0]
	state.r[T3] = (_s32)Load32_DMEM((0 + 0x000000F0) & 0xfff);


_04001FCC:	// 4001fcc: <8c0c0fc4>	LW        T4 = [R0+0fc4]
	state.r[T4] = (_s32)Load32_DMEM((0 + 0x00000FC4) & 0xfff);


_04001FD0:	// 4001fd0: <20012800>	ADDI      AT = R0 + 2800
	state.r[AT] = 0 + 0x2800;


_04001FD4:	// 4001fd4: <11600006>	BEQ       (R0==T3) --> 1ff0
	sp_reg_pc = 0x04001FD4;
	if (state.r[T3] == 0)
	{
		goto _04001FF0;
	}


_04001FD8:	// 4001fd8: <40812000>	MTC0      SP status = AT


_04001FDC:	// 4001fdc: <318c0001>	ANDI      T4 = T4 & 0001
	state.r[T4] = (state.r[T4] & 0x00000001);


_04001FE0:	// 4001fe0: <1180001e>	BEQ       (R0==T4) --> 205c
	sp_reg_pc = 0x04001FE0;
	if (state.r[T4] == 0)
	{
		Save32_DMEM((_u32)0, (0 + 0x00000FC4) & 0xfff);
		state.halt = 1;
		return;
	}


_04001FE4:	// 4001fe4: <ac000fc4>	SW        [R0+0fc4] = R0
	Save32_DMEM((_u32)0, (0 + 0x00000FC4) & 0xfff);


_04001FE8:	// 4001fe8: <08000459>	J         00001164
	state.r[K0] = (_s32)Load32_DMEM((0 + 0x00000BF8) & 0xfff);
	state.halt = 1;
	return;


_04001FEC:	// 4001fec: <8c1a0bf8>	LW        K0 = [R0+0bf8]
	state.r[K0] = (_s32)Load32_DMEM((0 + 0x00000BF8) & 0xfff);

_04001FF0:
_04001FF4:
_04001FF8:
_04001FFC:
	state.halt = 1;
	return;

	_BuildJumpTable:
	#include "jumptable.h"
	goto _Label;
}

void Zelda11A4_11D4(void)
{

	// 40011a4: <c8411800>	LDV       vec01 <00> = [V0 + 0x00]
	rsp_ldv_compiler((state.r[V0] & 0xFFF), 1, 8);


	// 40011a8: <c8621800>	LDV       vec02 <00> = [V1 + 0x00]
	rsp_ldv_compiler((state.r[V1] & 0xFFF), 2, 8);


	// 40011ac: <2021fff8>	ADDI      AT = AT + fff8
	state.r[AT] = state.r[AT] + 0xFFFFFFF8;


	// 40011b0: <20840010>	ADDI      A0 = A0 + 0010
	state.r[A0] = state.r[A0] + 0x0010;


	// 40011b4: <e8810878>	SSV       [A0 + 0x10] = vec01 <00>
	Save16_DMEM(state.v[1].U16[7], (0xFFFFFFF0 + state.r[A0]) & 0xfff);


	// 40011b8: <e881097a>	SSV       [A0 + 0x14] = vec01 <02>
	Save16_DMEM(state.v[1].U16[6], (0xFFFFFFF4 + state.r[A0]) & 0xfff);


	// 40011bc: <20420008>	ADDI      V0 = V0 + 0008
	state.r[V0] = state.r[V0] + 0x0008;


	// 40011c0: <e8810a7c>	SSV       [A0 + 0x18] = vec01 <04>
	Save16_DMEM(state.v[1].U16[5], (0xFFFFFFF8 + state.r[A0]) & 0xfff);


	// 40011c4: <e8810b7e>	SSV       [A0 + 0x1c] = vec01 <06>
	Save16_DMEM(state.v[1].U16[4], (0xFFFFFFFC + state.r[A0]) & 0xfff);


	// 40011c8: <e8820879>	SSV       [A0 + 0x12] = vec02 <00>
	Save16_DMEM(state.v[2].U16[7], (0xFFFFFFF2 + state.r[A0]) & 0xfff);


	// 40011cc: <20630008>	ADDI      V1 = V1 + 0008
	state.r[V1] = state.r[V1] + 0x0008;


	// 40011d0: <e882097b>	SSV       [A0 + 0x16] = vec02 <02>
	Save16_DMEM(state.v[2].U16[6], (0xFFFFFFF6 + state.r[A0]) & 0xfff);


	// 40011d4: <e8820a7d>	SSV       [A0 + 0x1a] = vec02 <04>
	Save16_DMEM(state.v[2].U16[5], (0xFFFFFFFA + state.r[A0]) & 0xfff);

_040011D8:
	;
}

void Zelda11F8_120C(void)
{

	// 40011f8: <c8411800>	LDV       vec01 <00> = [V0 + 0x00]
	rsp_ldv_compiler((state.r[V0] & 0xFFF), 1, 8);


	// 40011fc: <c8421801>	LDV       vec02 <00> = [V0 + 0x08]
	rsp_ldv_compiler((state.r[V0] + 0x00000008) & 0xFFF, 2, 8);


	// 4001200: <2021fff0>	ADDI      AT = AT + fff0
	state.r[AT] = state.r[AT] + 0xFFFFFFF0;


	// 4001204: <20420010>	ADDI      V0 = V0 + 0010
	state.r[V0] = state.r[V0] + 0x0010;


	// 4001208: <e8611800>	SDV       [V1 + 0x00] = vec01 <00>
	{
		_u32	addr = (0x00000000 + state.r[V1]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[1].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[1].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[1].U32[0x00000003];
		}
	}


	// 400120c: <e8621801>	SDV       [V1 + 0x04] = vec02 <00>
	{
		_u32	addr = (0x00000008 + state.r[V1]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[2].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[2].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[2].U32[0x00000003];
		}
	}

_04001210:
	;
}

void Zelda13A4_14D8(void)
{

_040013A4:	// 40013a4: <4a171f86>	VMUDN     vec30 = ( acc = vec03 * vec23[<none>]      ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[3*16].U16[0], 0xE4
			MOVQ mm1, state.v[3*16].U16[4*2]
			PSHUFW mm4, state.v[23*16].U16[0], 0xE4
			MOVQ mm5, state.v[23*16].U16[4*2]
			PSHUFW mm6, mm0, 0xE4
			MOVQ mm7, mm1
			PMULLW mm6, mm4
			PMULLW mm7, mm5
			MOVQ accumulator[0*4], mm6
			MOVQ accumulator[2*4], mm7
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm2, mm4
			PMULHUW mm3, mm5
			PSRAW mm0, 15
			PSRAW mm1, 15
			PMULLW mm0, mm4
			PMULLW mm1, mm5
			PADDW mm0, mm2
			PADDW mm1, mm3
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_040013A8:	// 40013a8: <02aaa820>	ADD       S5 = S5+T2
	state.r[S5] = (state.r[S5] + state.r[T2]);


_040013AC:	// 40013ac: <4a17278e>	VMADN     vec30 = ( acc+= vec04 * vec23[<none>] )
	{
		VMADN_operation(0, state.v[30].U16[0], state.v[4].U16[0], state.v[23].U16[0], 1, 0, 0, 1)
		VMADN_operation(1, state.v[30].U16[1], state.v[4].U16[1], state.v[23].U16[1], 1, 0, 0, 1)
		VMADN_operation(2, state.v[30].U16[2], state.v[4].U16[2], state.v[23].U16[2], 1, 0, 0, 1)
		VMADN_operation(3, state.v[30].U16[3], state.v[4].U16[3], state.v[23].U16[3], 1, 0, 0, 1)
		VMADN_operation(4, state.v[30].U16[4], state.v[4].U16[4], state.v[23].U16[4], 1, 0, 0, 1)
		VMADN_operation(5, state.v[30].U16[5], state.v[4].U16[5], state.v[23].U16[5], 1, 0, 0, 1)
		VMADN_operation(6, state.v[30].U16[6], state.v[4].U16[6], state.v[23].U16[6], 1, 0, 0, 1)
		VMADN_operation(7, state.v[30].U16[7], state.v[4].U16[7], state.v[23].U16[7], 1, 0, 0, 1)
	}


_040013B0:	// 40013b0: <ca811800>	LDV       vec01 <00> = [S4 + 0x00]
	rsp_ldv_compiler((state.r[S4] & 0xFFF), 1, 8);


_040013B4:	// 40013b4: <4a172f46>	VMUDN     vec29 = ( acc = vec05 * vec23[<none>]      ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[5*16].U16[0], 0xE4
			MOVQ mm1, state.v[5*16].U16[4*2]
			PSHUFW mm4, state.v[23*16].U16[0], 0xE4
			MOVQ mm5, state.v[23*16].U16[4*2]
			PSHUFW mm6, mm0, 0xE4
			MOVQ mm7, mm1
			PMULLW mm6, mm4
			PMULLW mm7, mm5
			MOVQ accumulator[0*4], mm6
			MOVQ accumulator[2*4], mm7
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm2, mm4
			PMULHUW mm3, mm5
			PSRAW mm0, 15
			PSRAW mm1, 15
			PMULLW mm0, mm4
			PMULLW mm1, mm5
			PADDW mm0, mm2
			PADDW mm1, mm3
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_040013B8:	// 40013b8: <92a10000>	LBU       AT = [S5+0000]
	state.r[AT] = (_s32)(_u8)pDMEM[((state.r[S5] + 0x00000000) & 0xfff) ^ 3];


_040013BC:	// 40013bc: <4a17374e>	VMADN     vec29 = ( acc+= vec06 * vec23[<none>] )
	{
		VMADN_operation(0, state.v[29].U16[0], state.v[6].U16[0], state.v[23].U16[0], 1, 1, 1, 1)
		VMADN_operation(1, state.v[29].U16[1], state.v[6].U16[1], state.v[23].U16[1], 1, 1, 1, 1)
		VMADN_operation(2, state.v[29].U16[2], state.v[6].U16[2], state.v[23].U16[2], 1, 1, 1, 1)
		VMADN_operation(3, state.v[29].U16[3], state.v[6].U16[3], state.v[23].U16[3], 1, 1, 1, 1)
		VMADN_operation(4, state.v[29].U16[4], state.v[6].U16[4], state.v[23].U16[4], 1, 1, 1, 1)
		VMADN_operation(5, state.v[29].U16[5], state.v[6].U16[5], state.v[23].U16[5], 1, 1, 1, 1)
		VMADN_operation(6, state.v[29].U16[6], state.v[6].U16[6], state.v[23].U16[6], 1, 1, 1, 1)
		VMADN_operation(7, state.v[29].U16[7], state.v[6].U16[7], state.v[23].U16[7], 1, 1, 1, 1)
	}


_040013C0:	// 40013c0: <19c00003>	BLEZ      (R0<=0) --> 13d0
	sp_reg_pc = 0x040013C0;
	if ((_s32)state.r[T6] <= 0)
	{
		state.r[T3] = (state.r[AT] & 0x0000000F);
		goto _040013D0;
	}


_040013C4:	// 40013c4: <302b000f>	ANDI      T3 = AT & 000f
	state.r[T3] = (state.r[AT] & 0x0000000F);


_040013C8:	// 40013c8: <4b16f785>	VMUDM     vec30 = ( acc = vec30 * vec22[0] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[30*16].U16[0], 0xE4
			MOVQ mm1, state.v[30*16].U16[4*2]
			PSHUFW mm4, state.v[22*16].U16[7*2], 0
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			MOVQ state.v[30*16].U16[0], mm0
			MOVQ state.v[30*16].U16[4*2], mm1
		}
	}


_040013CC:	// 40013cc: <4b16ef45>	VMUDM     vec29 = ( acc = vec29 * vec22[0] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[29*16].U16[0], 0xE4
			MOVQ mm1, state.v[29*16].U16[4*2]
			PSHUFW mm4, state.v[22*16].U16[7*2], 0
			PSHUFW mm6, mm0, 0xE4
			MOVQ mm7, mm1
			PMULLW mm6, mm4
			PMULLW mm7, mm4
			MOVQ accumulator[0*4], mm6
			MOVQ accumulator[2*4], mm7
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			MOVQ state.v[29*16].U16[0], mm0
			MOVQ state.v[29*16].U16[4*2], mm1
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_040013D0:	// 40013d0: <000b5940>	SLL       T3 = T3 << 5
	state.r[T3] = ((_u32)state.r[T3] << 5);


_040013D4:	// 40013d4: <4b01c8e8>	VAND      vec03 = vec25 and vec01[0]
	rsp_cop2_vand_element(&state.v[3], &state.v[25], &state.v[1].U16[7]);


_040013D8:	// 40013d8: <016f6820>	ADD       T5 = T3+T7
	state.r[T5] = (state.r[T3] + state.r[T7]);


_040013DC:	// 40013dc: <15000004>	BNE       (R0!=T0) --> 13f0
	sp_reg_pc = 0x040013DC;
	if (state.r[T0] != 0)
	{
		rsp_cop2_vand_element(&state.v[5], &state.v[25], &state.v[1].U16[6]);
		goto _040013F0;
	}


_040013E0:	// 40013e0: <4b21c968>	VAND      vec05 = vec25 and vec01[1]
	rsp_cop2_vand_element(&state.v[5], &state.v[25], &state.v[1].U16[6]);


_040013E4:	// 40013e4: <4b21c128>	VAND      vec04 = vec24 and vec01[1]
	rsp_cop2_vand_element(&state.v[4], &state.v[24], &state.v[1].U16[6]);


_040013E8:	// 40013e8: <4b41c968>	VAND      vec05 = vec25 and vec01[2]
	rsp_cop2_vand_element(&state.v[5], &state.v[25], &state.v[1].U16[5]);


_040013EC:	// 40013ec: <4b61c1a8>	VAND      vec06 = vec24 and vec01[3]
	rsp_cop2_vand_element(&state.v[6], &state.v[24], &state.v[1].U16[4]);


_040013F0:	// 40013f0: <00017102>	SRL       T6 = AT >> 4
	state.r[T6] = ((_u32)state.r[AT] >> 4);


_040013F4:	// 40013f4: <4bdba887>	VMUDH     vec02 = ( acc = (vec21 * vec27[6]) << 16) >> 16
	{
		_u16		s2value = state.v[27].U16[1];
		VMUDH_operation(0, state.v[2].U16[0], state.v[21].U16[0], s2value, 0, 1, 1, 1)
		VMUDH_operation(1, state.v[2].U16[1], state.v[21].U16[1], s2value, 0, 1, 1, 1)
		VMUDH_operation(2, state.v[2].U16[2], state.v[21].U16[2], s2value, 0, 1, 1, 1)
		VMUDH_operation(3, state.v[2].U16[3], state.v[21].U16[3], s2value, 0, 1, 1, 1)
		VMUDH_operation(4, state.v[2].U16[4], state.v[21].U16[4], s2value, 0, 1, 1, 1)
		VMUDH_operation(5, state.v[2].U16[5], state.v[21].U16[5], s2value, 0, 1, 1, 1)
		VMUDH_operation(6, state.v[2].U16[6], state.v[21].U16[6], s2value, 0, 1, 1, 1)
		VMUDH_operation(7, state.v[2].U16[7], state.v[21].U16[7], s2value, 0, 1, 1, 1)
	}


_040013F8:	// 40013f8: <00091020>	ADD       V0 = R0+T1
	state.r[V0] = (0 + state.r[T1]);


_040013FC:	// 40013fc: <4bfba08f>	VMADH     vec02 = ( acc+= (vec20 * vec27[7]) << 16) >> 16
	{
		_u16		s2value = state.v[27].U16[0];
		VMADH_operation(0, state.v[2].U16[0], state.v[20].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[20].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[20].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[20].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[20].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[20].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[20].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[20].U16[7], s2value, 0, 1, 0, 1)
	}


_04001400:	// 4001400: <004e7022>	SUB       T6 = V0-T6
	state.r[T6] = (state.r[V0] - state.r[T6]);


_04001404:	// 4001404: <4b1e988f>	VMADH     vec02 = ( acc+= (vec19 * vec30[0]) << 16) >> 16
	{
		_u16		s2value = state.v[30].U16[7];
		VMADH_operation(0, state.v[2].U16[0], state.v[19].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[19].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[19].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[19].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[19].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[19].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[19].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[19].U16[7], s2value, 0, 1, 0, 1)
	}


_04001408:	// 4001408: <21c2ffff>	ADDI      V0 = T6 + ffff
	state.r[V0] = state.r[T6] + 0xFFFFFFFF;


_0400140C:	// 400140c: <4b3e908f>	VMADH     vec02 = ( acc+= (vec18 * vec30[1]) << 16) >> 16
	{
		_u16		s2value = state.v[30].U16[6];
		VMADH_operation(0, state.v[2].U16[0], state.v[18].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[18].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[18].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[18].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[18].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[18].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[18].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[18].U16[7], s2value, 0, 1, 0, 1)
	}


_04001410:	// 4001410: <20030001>	ADDI      V1 = R0 + 0001
	state.r[V1] = 0 + 0x0001;


_04001414:	// 4001414: <4b5e888f>	VMADH     vec02 = ( acc+= (vec17 * vec30[2]) << 16) >> 16
	{
		_u16		s2value = state.v[30].U16[5];
		VMADH_operation(0, state.v[2].U16[0], state.v[17].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[17].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[17].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[17].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[17].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[17].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[17].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[17].U16[7], s2value, 0, 1, 0, 1)
	}


_04001418:	// 4001418: <00031bc0>	SLL       V1 = V1 << 15
	state.r[V1] = ((_u32)state.r[V1] << 15);


_0400141C:	// 400141c: <4b7e808f>	VMADH     vec02 = ( acc+= (vec16 * vec30[3]) << 16) >> 16
	{
		_u16		s2value = state.v[30].U16[4];
		VMADH_operation(0, state.v[2].U16[0], state.v[16].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[16].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[16].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[16].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[16].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[16].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[16].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[16].U16[7], s2value, 1, 1, 0, 1)
	}


_04001420:	// 4001420: <11c00002>	BEQ       (R0==T6) --> 142c
	sp_reg_pc = 0x04001420;
	if (state.r[T6] == 0)
	{
		state.r[A0] = 0 + 0xFFFFFFFF;
		goto _0400142C;
	}


_04001424:	// 4001424: <2004ffff>	ADDI      A0 = R0 + ffff
	state.r[A0] = 0 + 0xFFFFFFFF;


_04001428:	// 4001428: <00432006>	SRLV      V1 = A0 >> V0
	state.r[A0] = ((_u32)state.r[V1] >> (state.r[V0] & 0x1F));


_0400142C:	// 400142c: <4b9e7f0f>	VMADH     vec28 = ( acc+= (vec15 * vec30[4]) << 16) >> 16
	{
		_u16		s2value = state.v[30].U16[3];
		VMADH_operation(0, state.v[28].U16[0], state.v[15].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[28].U16[1], state.v[15].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[28].U16[2], state.v[15].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[28].U16[3], state.v[15].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[28].U16[4], state.v[15].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[28].U16[5], state.v[15].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[28].U16[6], state.v[15].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[28].U16[7], state.v[15].U16[7], s2value, 0, 1, 0, 1)
	}


_04001430:	// 4001430: <4884b000>	MTC2      vec22 <00> = A0
	state.v[22].U16[7] = (_u16)state.r[A0];


_04001434:	// 4001434: <4bbe708f>	VMADH     vec02 = ( acc+= (vec14 * vec30[5]) << 16) >> 16
	{
		_u16		s2value = state.v[30].U16[2];
		VMADH_operation(0, state.v[2].U16[0], state.v[14].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[14].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[14].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[14].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[14].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[14].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[14].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[14].U16[7], s2value, 0, 1, 0, 1)
	}


_04001438:	// 4001438: <4bde688f>	VMADH     vec02 = ( acc+= (vec13 * vec30[6]) << 16) >> 16
	{
		_u16		s2value = state.v[30].U16[1];
		VMADH_operation(0, state.v[2].U16[0], state.v[13].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[13].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[13].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[13].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[13].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[13].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[13].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[13].U16[7], s2value, 0, 1, 0, 1)
	}


_0400143C:	// 400143c: <4bbff08f>	VMADH     vec02 = ( acc+= (vec30 * vec31[5]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[2];
		VMADH_operation(0, state.v[2].U16[0], state.v[30].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[30].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[30].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[30].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[30].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[30].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[30].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[30].U16[7], s2value, 0, 1, 0, 1)
	}


_04001440:	// 4001440: <4b3c3e9d>	VSAW $v26, $v7, $v28[[1]]
	state.v[26].U16[0] = accumulator[0];
	state.v[26].U16[1] = accumulator[1];
	state.v[26].U16[2] = accumulator[2];
	state.v[26].U16[3] = accumulator[3];
	state.v[26].U16[4] = accumulator[4];
	state.v[26].U16[5] = accumulator[5];
	state.v[26].U16[6] = accumulator[6];
	state.v[26].U16[7] = accumulator[7];


_04001444:	// 4001444: <4b1c3f1d>	VSAW $v28, $v7, $v28[[0]]
	state.v[28].U16[0] = accumulator_hi[0].U16[1];
	state.v[28].U16[1] = accumulator_hi[1].U16[1];
	state.v[28].U16[2] = accumulator_hi[2].U16[1];
	state.v[28].U16[3] = accumulator_hi[3].U16[1];
	state.v[28].U16[4] = accumulator_hi[4].U16[1];
	state.v[28].U16[5] = accumulator_hi[5].U16[1];
	state.v[28].U16[6] = accumulator_hi[6].U16[1];
	state.v[28].U16[7] = accumulator_hi[7].U16[1];


_04001448:	// 4001448: <4b9fd086>	VMUDN     vec02 = ( acc = vec26 * vec31[4]      ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[26*16].U16[0], 0xE4
			MOVQ mm1, state.v[26*16].U16[4*2]
			PSHUFW mm4, state.v[31*16].U16[3*2], 0
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm2, mm4
			PMULHUW mm3, mm4
			PSRAW mm0, 15
			PSRAW mm1, 15
			PMULLW mm0, mm4
			PMULLW mm1, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_0400144C:	// 400144c: <4b9fe70f>	VMADH     vec28 = ( acc+= (vec28 * vec31[4]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[3];
		VMADH_operation(0, state.v[28].U16[0], state.v[28].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[28].U16[1], state.v[28].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[28].U16[2], state.v[28].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[28].U16[3], state.v[28].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[28].U16[4], state.v[28].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[28].U16[5], state.v[28].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[28].U16[6], state.v[28].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[28].U16[7], state.v[28].U16[7], s2value, 1, 0, 0, 1)
	}


_04001450:	// 4001450: <4b1d9887>	VMUDH     vec02 = ( acc = (vec19 * vec29[0]) << 16) >> 16
	{
		_u16		s2value = state.v[29].U16[7];
		VMUDH_operation(0, state.v[2].U16[0], state.v[19].U16[0], s2value, 0, 1, 0, 1)
		VMUDH_operation(1, state.v[2].U16[1], state.v[19].U16[1], s2value, 0, 1, 0, 1)
		VMUDH_operation(2, state.v[2].U16[2], state.v[19].U16[2], s2value, 0, 1, 0, 1)
		VMUDH_operation(3, state.v[2].U16[3], state.v[19].U16[3], s2value, 0, 1, 0, 1)
		VMUDH_operation(4, state.v[2].U16[4], state.v[19].U16[4], s2value, 0, 1, 0, 1)
		VMUDH_operation(5, state.v[2].U16[5], state.v[19].U16[5], s2value, 0, 1, 0, 1)
		VMUDH_operation(6, state.v[2].U16[6], state.v[19].U16[6], s2value, 0, 1, 0, 1)
		VMUDH_operation(7, state.v[2].U16[7], state.v[19].U16[7], s2value, 0, 1, 0, 1)
	}


_04001454:	// 4001454: <21acfffe>	ADDI      T4 = T5 + fffe
	state.r[T4] = state.r[T5] + 0xFFFFFFFE;


_04001458:	// 4001458: <4b3d908f>	VMADH     vec02 = ( acc+= (vec18 * vec29[1]) << 16) >> 16
	{
		_u16		s2value = state.v[29].U16[6];
		VMADH_operation(0, state.v[2].U16[0], state.v[18].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[18].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[18].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[18].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[18].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[18].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[18].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[18].U16[7], s2value, 0, 1, 0, 1)
	}


_0400145C:	// 400145c: <c9932802>	LRV       vec19 <00> = [T4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[19].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001460:	// 4001460: <4b5d888f>	VMADH     vec02 = ( acc+= (vec17 * vec29[2]) << 16) >> 16
	{
		_u16		s2value = state.v[29].U16[5];
		VMADH_operation(0, state.v[2].U16[0], state.v[17].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[17].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[17].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[17].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[17].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[17].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[17].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[17].U16[7], s2value, 0, 1, 0, 1)
	}


_04001464:	// 4001464: <218cfffe>	ADDI      T4 = T4 + fffe
	state.r[T4] = state.r[T4] + 0xFFFFFFFE;


_04001468:	// 4001468: <4b7d808f>	VMADH     vec02 = ( acc+= (vec16 * vec29[3]) << 16) >> 16
	{
		_u16		s2value = state.v[29].U16[4];
		VMADH_operation(0, state.v[2].U16[0], state.v[16].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[16].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[16].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[16].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[16].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[16].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[16].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[16].U16[7], s2value, 0, 1, 0, 1)
	}


_0400146C:	// 400146c: <c9922802>	LRV       vec18 <00> = [T4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[18].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001470:	// 4001470: <4b9d788f>	VMADH     vec02 = ( acc+= (vec15 * vec29[4]) << 16) >> 16
	{
		_u16		s2value = state.v[29].U16[3];
		VMADH_operation(0, state.v[2].U16[0], state.v[15].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[15].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[15].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[15].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[15].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[15].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[15].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[15].U16[7], s2value, 0, 1, 0, 1)
	}


_04001474:	// 4001474: <218cfffe>	ADDI      T4 = T4 + fffe
	state.r[T4] = state.r[T4] + 0xFFFFFFFE;


_04001478:	// 4001478: <4bbd708f>	VMADH     vec02 = ( acc+= (vec14 * vec29[5]) << 16) >> 16
	{
		_u16		s2value = state.v[29].U16[2];
		VMADH_operation(0, state.v[2].U16[0], state.v[14].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[14].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[14].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[14].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[14].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[14].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[14].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[14].U16[7], s2value, 0, 1, 0, 1)
	}


_0400147C:	// 400147c: <c9912802>	LRV       vec17 <00> = [T4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[17].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001480:	// 4001480: <4bdd688f>	VMADH     vec02 = ( acc+= (vec13 * vec29[6]) << 16) >> 16
	{
		_u16		s2value = state.v[29].U16[1];
		VMADH_operation(0, state.v[2].U16[0], state.v[13].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[13].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[13].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[13].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[13].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[13].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[13].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[13].U16[7], s2value, 0, 1, 0, 1)
	}


_04001484:	// 4001484: <218cfffe>	ADDI      T4 = T4 + fffe
	state.r[T4] = state.r[T4] + 0xFFFFFFFE;


_04001488:	// 4001488: <4bbfe88f>	VMADH     vec02 = ( acc+= (vec29 * vec31[5]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[2];
		VMADH_operation(0, state.v[2].U16[0], state.v[29].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[29].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[29].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[29].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[29].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[29].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[29].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[29].U16[7], s2value, 0, 1, 0, 1)
	}


_0400148C:	// 400148c: <c9902802>	LRV       vec16 <00> = [T4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[16].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001490:	// 4001490: <4bdca88f>	VMADH     vec02 = ( acc+= (vec21 * vec28[6]) << 16) >> 16
	{
		_u16		s2value = state.v[28].U16[1];
		VMADH_operation(0, state.v[2].U16[0], state.v[21].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[21].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[21].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[21].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[21].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[21].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[21].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[21].U16[7], s2value, 0, 1, 0, 1)
	}


_04001494:	// 4001494: <218cfffe>	ADDI      T4 = T4 + fffe
	state.r[T4] = state.r[T4] + 0xFFFFFFFE;


_04001498:	// 4001498: <4bfca08f>	VMADH     vec02 = ( acc+= (vec20 * vec28[7]) << 16) >> 16
	{
		_u16		s2value = state.v[28].U16[0];
		VMADH_operation(0, state.v[2].U16[0], state.v[20].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[20].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[20].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[20].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[20].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[20].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[20].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[20].U16[7], s2value, 0, 1, 0, 1)
	}


_0400149C:	// 400149c: <c98f2802>	LRV       vec15 <00> = [T4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[15].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014A0:	// 40014a0: <4b3b3e9d>	VSAW $v26, $v7, $v27[[1]]
	state.v[26].U16[0] = accumulator[0];
	state.v[26].U16[1] = accumulator[1];
	state.v[26].U16[2] = accumulator[2];
	state.v[26].U16[3] = accumulator[3];
	state.v[26].U16[4] = accumulator[4];
	state.v[26].U16[5] = accumulator[5];
	state.v[26].U16[6] = accumulator[6];
	state.v[26].U16[7] = accumulator[7];


_040014A4:	// 40014a4: <218cfffe>	ADDI      T4 = T4 + fffe
	state.r[T4] = state.r[T4] + 0xFFFFFFFE;


_040014A8:	// 40014a8: <4b1b3edd>	VSAW $v27, $v7, $v27[[0]]
	state.v[27].U16[0] = accumulator_hi[0].U16[1];
	state.v[27].U16[1] = accumulator_hi[1].U16[1];
	state.v[27].U16[2] = accumulator_hi[2].U16[1];
	state.v[27].U16[3] = accumulator_hi[3].U16[1];
	state.v[27].U16[4] = accumulator_hi[4].U16[1];
	state.v[27].U16[5] = accumulator_hi[5].U16[1];
	state.v[27].U16[6] = accumulator_hi[6].U16[1];
	state.v[27].U16[7] = accumulator_hi[7].U16[1];


_040014AC:	// 40014ac: <c98e2802>	LRV       vec14 <00> = [T4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[14].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014B0:	// 40014b0: <218cfffe>	ADDI      T4 = T4 + fffe
	state.r[T4] = state.r[T4] + 0xFFFFFFFE;


_040014B4:	// 40014b4: <c98d2802>	LRV       vec13 <00> = [T4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[13].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014B8:	// 40014b8: <c9b52000>	LQV       vec21 <00> = [T5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[21].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[21].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[21].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[21].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[21].U64[0] = state.v[21].U64[0] & ~temp1;
			state.v[21].U64[1] = state.v[21].U64[1] & ~temp2;
			state.v[21].U64[0] = state.v[21].U64[0] | value1;
			state.v[21].U64[1] = state.v[21].U64[1] | value2;
		}
	}


_040014BC:	// 40014bc: <4b9fd086>	VMUDN     vec02 = ( acc = vec26 * vec31[4]      ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[26*16].U16[0], 0xE4
			MOVQ mm1, state.v[26*16].U16[4*2]
			PSHUFW mm4, state.v[31*16].U16[3*2], 0
			PSHUFW mm6, mm0, 0xE4
			MOVQ mm7, mm1
			PMULLW mm6, mm4
			PMULLW mm7, mm4
			MOVQ accumulator[0*4], mm6
			MOVQ accumulator[2*4], mm7
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm2, mm4
			PMULHUW mm3, mm4
			PSRAW mm0, 15
			PSRAW mm1, 15
			PMULLW mm0, mm4
			PMULLW mm1, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			MOVQ state.v[2*16].U16[0], mm0
			MOVQ state.v[2*16].U16[4*2], mm1
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


_040014C0:	// 40014c0: <c9b42001>	LQV       vec20 <00> = [T5 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[20].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[20].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[20].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[20].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[20].U64[0] = state.v[20].U64[0] & ~temp1;
			state.v[20].U64[1] = state.v[20].U64[1] & ~temp2;
			state.v[20].U64[0] = state.v[20].U64[0] | value1;
			state.v[20].U64[1] = state.v[20].U64[1] | value2;
		}
	}


_040014C4:	// 40014c4: <4b9fdecf>	VMADH     vec27 = ( acc+= (vec27 * vec31[4]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[3];
		VMADH_operation(0, state.v[27].U16[0], state.v[27].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[27].U16[1], state.v[27].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[27].U16[2], state.v[27].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[27].U16[3], state.v[27].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[27].U16[4], state.v[27].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[27].U16[5], state.v[27].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[27].U16[6], state.v[27].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[27].U16[7], state.v[27].U16[7], s2value, 1, 1, 0, 1)
	}


_040014C8:	// 40014c8: <2252ffe0>	ADDI      S2 = S2 + ffe0
	state.r[S2] = state.r[S2] + 0xFFFFFFE0;


_040014CC:	// 40014cc: <ea7c1800>	SDV       [S3 + 0x00] = vec28 <00>
	{
		_u32	addr = (0x00000000 + state.r[S3]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[28].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[28].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[28].U32[0x00000003];
		}
	}


_040014D0:	// 40014d0: <ea7c1c01>	SDV       [S3 + 0x04] = vec28 <08>
	{
		_u32	addr = (0x00000008 + state.r[S3]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[28].U64[0x00000000], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[28].U32[0x00000000];
			*(_u32 *)&pDMEM[addr  ] = state.v[28].U32[0x00000001];
		}
	}


_040014D4:	// 40014d4: <ea7b1802>	SDV       [S3 + 0x08] = vec27 <00>
	{
		_u32	addr = (0x00000010 + state.r[S3]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[27].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[27].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[27].U32[0x00000003];
		}
	}


_040014D8:	// 40014d8: <ea7b1c03>	SDV       [S3 + 0x0c] = vec27 <08>
	{
		_u32	addr = (0x00000018 + state.r[S3]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[27].U64[0x00000000], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[27].U32[0x00000000];
			*(_u32 *)&pDMEM[addr  ] = state.v[27].U32[0x00000001];
		}
	}

_040014DC:
	;
}

void Zelda192C_19A8(void)
{

	// 400192c: <ca6f2001>	LQV       vec15 <00> = [S3 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[15].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[15].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[15].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[15].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[15].U64[0] = state.v[15].U64[0] & ~temp1;
			state.v[15].U64[1] = state.v[15].U64[1] & ~temp2;
			state.v[15].U64[0] = state.v[15].U64[0] | value1;
			state.v[15].U64[1] = state.v[15].U64[1] | value2;
		}
	}


	// 4001930: <22730020>	ADDI      S3 = S3 + 0020
	state.r[S3] = state.r[S3] + 0x0020;


	// 4001934: <4b014245>	VMUDM     vec09 = ( acc = vec08 * vec01[0] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[8*16].U16[0], 0xE4
			MOVQ mm1, state.v[8*16].U16[4*2]
			PSHUFW mm4, state.v[1*16].U16[7*2], 0
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			MOVQ state.v[9*16].U16[0], mm0
			MOVQ state.v[9*16].U16[4*2], mm1
		}
	}


	// 4001938: <4b414285>	VMUDM     vec10 = ( acc = vec08 * vec01[2] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[8*16].U16[0], 0xE4
			MOVQ mm1, state.v[8*16].U16[4*2]
			PSHUFW mm4, state.v[1*16].U16[5*2], 0
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			MOVQ state.v[10*16].U16[0], mm0
			MOVQ state.v[10*16].U16[4*2], mm1
		}
	}


	// 400193c: <2294fff0>	ADDI      S4 = S4 + fff0
	state.r[S4] = state.r[S4] + 0xFFFFFFF0;


	// 4001940: <c9cb2000>	LQV       vec11 <00> = [T6 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T6]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[11].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[11].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[11].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[11].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[11].U64[0] = state.v[11].U64[0] & ~temp1;
			state.v[11].U64[1] = state.v[11].U64[1] & ~temp2;
			state.v[11].U64[0] = state.v[11].U64[0] | value1;
			state.v[11].U64[1] = state.v[11].U64[1] | value2;
		}
	}


	// 4001944: <c9ec2000>	LQV       vec12 <00> = [T7 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T7]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[12].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[12].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[12].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[12].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[12].U64[0] = state.v[12].U64[0] & ~temp1;
			state.v[12].U64[1] = state.v[12].U64[1] & ~temp2;
			state.v[12].U64[0] = state.v[12].U64[0] | value1;
			state.v[12].U64[1] = state.v[12].U64[1] | value2;
		}
	}


	// 4001948: <4b217c05>	VMUDM     vec16 = ( acc = vec15 * vec01[1] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[15*16].U16[0], 0xE4
			MOVQ mm1, state.v[15*16].U16[4*2]
			PSHUFW mm4, state.v[1*16].U16[6*2], 0
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			MOVQ state.v[16*16].U16[0], mm0
			MOVQ state.v[16*16].U16[4*2], mm1
		}
	}


	// 400194c: <4b617c45>	VMUDM     vec17 = ( acc = vec15 * vec01[3] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[15*16].U16[0], 0xE4
			MOVQ mm1, state.v[15*16].U16[4*2]
			PSHUFW mm4, state.v[1*16].U16[4*2], 0
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			MOVQ state.v[17*16].U16[0], mm0
			MOVQ state.v[17*16].U16[4*2], mm1
		}
	}


	// 4001950: <c9d22001>	LQV       vec18 <00> = [T6 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T6]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[18].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[18].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[18].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[18].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[18].U64[0] = state.v[18].U64[0] & ~temp1;
			state.v[18].U64[1] = state.v[18].U64[1] & ~temp2;
			state.v[18].U64[0] = state.v[18].U64[0] | value1;
			state.v[18].U64[1] = state.v[18].U64[1] | value2;
		}
	}


	// 4001954: <c9f32001>	LQV       vec19 <00> = [T7 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T7]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[19].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[19].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[19].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[19].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[19].U64[0] = state.v[19].U64[0] & ~temp1;
			state.v[19].U64[1] = state.v[19].U64[1] & ~temp2;
			state.v[19].U64[0] = state.v[19].U64[0] | value1;
			state.v[19].U64[1] = state.v[19].U64[1] | value2;
		}
	}


	// 4001958: <4b024a6c>	VXOR      vec09 = vec09 xor vec02[0]
	rsp_cop2_vxor_element(&state.v[9], &state.v[9], &state.v[2].U16[7]);


	// 400195c: <4b2252ac>	VXOR      vec10 = vec10 xor vec02[1]
	rsp_cop2_vxor_element(&state.v[10], &state.v[10], &state.v[2].U16[6]);


	// 4001960: <ca0d2000>	LQV       vec13 <00> = [S0 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S0]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[13].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[13].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[13].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[13].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[13].U64[0] = state.v[13].U64[0] & ~temp1;
			state.v[13].U64[1] = state.v[13].U64[1] & ~temp2;
			state.v[13].U64[0] = state.v[13].U64[0] | value1;
			state.v[13].U64[1] = state.v[13].U64[1] | value2;
		}
	}


	// 4001964: <ca2e2000>	LQV       vec14 <00> = [S1 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[14].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[14].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[14].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[14].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[14].U64[0] = state.v[14].U64[0] & ~temp1;
			state.v[14].U64[1] = state.v[14].U64[1] & ~temp2;
			state.v[14].U64[0] = state.v[14].U64[0] | value1;
			state.v[14].U64[1] = state.v[14].U64[1] | value2;
		}
	}


	// 4001968: <4a095ad0>	VADD      vec11 = vec11 + vec09[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[11].U16[0], state.v[11].U16[0], state.v[9].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[11].U16[1], state.v[11].U16[1], state.v[9].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[11].U16[2], state.v[11].U16[2], state.v[9].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[11].U16[3], state.v[11].U16[3], state.v[9].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[11].U16[4], state.v[11].U16[4], state.v[9].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[11].U16[5], state.v[11].U16[5], state.v[9].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[11].U16[6], state.v[11].U16[6], state.v[9].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[11].U16[7], state.v[11].U16[7], state.v[9].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


	// 400196c: <4a0a6310>	VADD      vec12 = vec12 + vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[12].U16[0], state.v[12].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[12].U16[1], state.v[12].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[12].U16[2], state.v[12].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[12].U16[3], state.v[12].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[12].U16[4], state.v[12].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[12].U16[5], state.v[12].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[12].U16[6], state.v[12].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[12].U16[7], state.v[12].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


	// 4001970: <4b814a45>	VMUDM     vec09 = ( acc = vec09 * vec01[4] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[9*16].U16[0], 0xE4
			MOVQ mm1, state.v[9*16].U16[4*2]
			PSHUFW mm4, state.v[1*16].U16[3*2], 0
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			MOVQ state.v[9*16].U16[0], mm0
			MOVQ state.v[9*16].U16[4*2], mm1
		}
	}


	// 4001974: <4b815285>	VMUDM     vec10 = ( acc = vec10 * vec01[4] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[10*16].U16[0], 0xE4
			MOVQ mm1, state.v[10*16].U16[4*2]
			PSHUFW mm4, state.v[1*16].U16[3*2], 0
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			MOVQ state.v[10*16].U16[0], mm0
			MOVQ state.v[10*16].U16[4*2], mm1
		}
	}


	// 4001978: <4b02842c>	VXOR      vec16 = vec16 xor vec02[0]
	rsp_cop2_vxor_element(&state.v[16], &state.v[16], &state.v[2].U16[7]);


	// 400197c: <4b228c6c>	VXOR      vec17 = vec17 xor vec02[1]
	rsp_cop2_vxor_element(&state.v[17], &state.v[17], &state.v[2].U16[6]);


	// 4001980: <ca142001>	LQV       vec20 <00> = [S0 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S0]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[20].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[20].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[20].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[20].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[20].U64[0] = state.v[20].U64[0] & ~temp1;
			state.v[20].U64[1] = state.v[20].U64[1] & ~temp2;
			state.v[20].U64[0] = state.v[20].U64[0] | value1;
			state.v[20].U64[1] = state.v[20].U64[1] | value2;
		}
	}


	// 4001984: <ca352001>	LQV       vec21 <00> = [S1 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[21].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[21].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[21].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[21].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[21].U64[0] = state.v[21].U64[0] & ~temp1;
			state.v[21].U64[1] = state.v[21].U64[1] & ~temp2;
			state.v[21].U64[0] = state.v[21].U64[0] | value1;
			state.v[21].U64[1] = state.v[21].U64[1] | value2;
		}
	}


	// 4001988: <4a109490>	VADD      vec18 = vec18 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[18].U16[0], state.v[18].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[18].U16[1], state.v[18].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[18].U16[2], state.v[18].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[18].U16[3], state.v[18].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[18].U16[4], state.v[18].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[18].U16[5], state.v[18].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[18].U16[6], state.v[18].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[18].U16[7], state.v[18].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


	// 400198c: <4a119cd0>	VADD      vec19 = vec19 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[19].U16[0], state.v[19].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[19].U16[1], state.v[19].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[19].U16[2], state.v[19].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[19].U16[3], state.v[19].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[19].U16[4], state.v[19].U16[4], state.v[17].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[19].U16[5], state.v[19].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[19].U16[6], state.v[19].U16[6], state.v[17].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[19].U16[7], state.v[19].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


	// 4001990: <4ba18405>	VMUDM     vec16 = ( acc = vec16 * vec01[5] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[16*16].U16[0], 0xE4
			MOVQ mm1, state.v[16*16].U16[4*2]
			PSHUFW mm4, state.v[1*16].U16[2*2], 0
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			MOVQ state.v[16*16].U16[0], mm0
			MOVQ state.v[16*16].U16[4*2], mm1
		}
	}


	// 4001994: <4ba18c45>	VMUDM     vec17 = ( acc = vec17 * vec01[5] ) >> 16
	{
		__asm {
			PSHUFW mm0, state.v[17*16].U16[0], 0xE4
			MOVQ mm1, state.v[17*16].U16[4*2]
			PSHUFW mm4, state.v[1*16].U16[2*2], 0
			PSHUFW mm6, mm0, 0xE4
			MOVQ mm7, mm1
			PMULLW mm6, mm4
			PMULLW mm7, mm4
			MOVQ accumulator[0*4], mm6
			MOVQ accumulator[2*4], mm7
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm1
			PMULHUW mm0, mm4
			PMULHUW mm1, mm4
			PSRAW mm2, 15
			PSRAW mm3, 15
			PMULLW mm2, mm4
			PMULLW mm3, mm4
			PADDW mm0, mm2
			PADDW mm1, mm3
			MOVQ state.v[17*16].U16[0], mm0
			MOVQ state.v[17*16].U16[4*2], mm1
			PSHUFW mm2, mm0, 0xE4
			MOVQ mm3, mm0
			PSHUFW mm4, mm1, 0xE4
			MOVQ mm5, mm1
			PSRAW mm0, 15
			PSRAW mm1, 15
			PUNPCKLWD mm2, mm0
			PUNPCKLWD mm4, mm1
			PUNPCKHWD mm3, mm0
			PUNPCKHWD mm5, mm1
			MOVQ accumulator_hi[0*4], mm2
			MOVQ accumulator_hi[2*4], mm3
			MOVQ accumulator_hi[4*4], mm4
			MOVQ accumulator_hi[6*4], mm5
		}
	}


	// 4001998: <4b424a6c>	VXOR      vec09 = vec09 xor vec02[2]
	rsp_cop2_vxor_element(&state.v[9], &state.v[9], &state.v[2].U16[5]);


	// 400199c: <4b6252ac>	VXOR      vec10 = vec10 xor vec02[3]
	rsp_cop2_vxor_element(&state.v[10], &state.v[10], &state.v[2].U16[4]);


	// 40019a0: <e9cb2000>	SQV       [T6 + 0x00] = vec11 <00>
	{
		_u32    addr = (0x00000000 + state.r[T6]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[11].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[11].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[11].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[11].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[11].U64[0] & temp1);
			value2 |= (state.v[11].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


	// 40019a4: <4b42842c>	VXOR      vec16 = vec16 xor vec02[2]
	rsp_cop2_vxor_element(&state.v[16], &state.v[16], &state.v[2].U16[5]);


	// 40019a8: <4b628c6c>	VXOR      vec17 = vec17 xor vec02[3]
	rsp_cop2_vxor_element(&state.v[17], &state.v[17], &state.v[2].U16[4]);

_040019AC:
	;
}

void Zelda1B60_1B8C(void)
{

	// 4001b60: <4bdfdec0>	VMULF     vec27 = ( acc = (vec27 * vec31[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[31].U16[1];
		VMULF_operation(0, state.v[27].U16[0], state.v[27].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[27].U16[1], state.v[27].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[27].U16[2], state.v[27].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[27].U16[3], state.v[27].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[27].U16[4], state.v[27].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[27].U16[5], state.v[27].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[27].U16[6], state.v[27].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[27].U16[7], state.v[27].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001b64: <2252ffe0>	ADDI      S2 = S2 + ffe0
	state.r[S2] = state.r[S2] + 0xFFFFFFE0;


	// 4001b68: <4b1eeec8>	VMACF     vec27 = ( acc += (vec29 * vec30[0]) << 1 ) >> 16
	{
		_u16		s2value = state.v[30].U16[7];
		VMACF_operation(0, state.v[27].U16[0], state.v[29].U16[0], s2value, 1, 0, 0, 1)
		VMACF_operation(1, state.v[27].U16[1], state.v[29].U16[1], s2value, 1, 0, 0, 1)
		VMACF_operation(2, state.v[27].U16[2], state.v[29].U16[2], s2value, 1, 0, 0, 1)
		VMACF_operation(3, state.v[27].U16[3], state.v[29].U16[3], s2value, 1, 0, 0, 1)
		VMACF_operation(4, state.v[27].U16[4], state.v[29].U16[4], s2value, 1, 0, 0, 1)
		VMACF_operation(5, state.v[27].U16[5], state.v[29].U16[5], s2value, 1, 0, 0, 1)
		VMACF_operation(6, state.v[27].U16[6], state.v[29].U16[6], s2value, 1, 0, 0, 1)
		VMACF_operation(7, state.v[27].U16[7], state.v[29].U16[7], s2value, 1, 0, 0, 1)
	}


	// 4001b6c: <22940020>	ADDI      S4 = S4 + 0020
	state.r[S4] = state.r[S4] + 0x0020;


	// 4001b70: <4bdfd680>	VMULF     vec26 = ( acc = (vec26 * vec31[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[31].U16[1];
		VMULF_operation(0, state.v[26].U16[0], state.v[26].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[26].U16[1], state.v[26].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[26].U16[2], state.v[26].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[26].U16[3], state.v[26].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[26].U16[4], state.v[26].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[26].U16[5], state.v[26].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[26].U16[6], state.v[26].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[26].U16[7], state.v[26].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001b74: <4b1ee688>	VMACF     vec26 = ( acc += (vec28 * vec30[0]) << 1 ) >> 16
	{
		_u16		s2value = state.v[30].U16[7];
		VMACF_operation(0, state.v[26].U16[0], state.v[28].U16[0], s2value, 1, 1, 1, 1)
		VMACF_operation(1, state.v[26].U16[1], state.v[28].U16[1], s2value, 1, 1, 1, 1)
		VMACF_operation(2, state.v[26].U16[2], state.v[28].U16[2], s2value, 1, 1, 1, 1)
		VMACF_operation(3, state.v[26].U16[3], state.v[28].U16[3], s2value, 1, 1, 1, 1)
		VMACF_operation(4, state.v[26].U16[4], state.v[28].U16[4], s2value, 1, 1, 1, 1)
		VMACF_operation(5, state.v[26].U16[5], state.v[28].U16[5], s2value, 1, 1, 1, 1)
		VMACF_operation(6, state.v[26].U16[6], state.v[28].U16[6], s2value, 1, 1, 1, 1)
		VMACF_operation(7, state.v[26].U16[7], state.v[28].U16[7], s2value, 1, 1, 1, 1)
	}


	// 4001b78: <ca9d2000>	LQV       vec29 <00> = [S4 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[29].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[29].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[29].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[29].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[29].U64[0] = state.v[29].U64[0] & ~temp1;
			state.v[29].U64[1] = state.v[29].U64[1] & ~temp2;
			state.v[29].U64[0] = state.v[29].U64[0] | value1;
			state.v[29].U64[1] = state.v[29].U64[1] | value2;
		}
	}


	// 4001b7c: <ea7b2000>	SQV       [S3 + 0x00] = vec27 <00>
	{
		_u32    addr = (0x00000000 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[27].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[27].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[27].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[27].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[27].U64[0] & temp1);
			value2 |= (state.v[27].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


	// 4001b80: <ca7b2002>	LQV       vec27 <00> = [S3 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


	// 4001b84: <ea7a2001>	SQV       [S3 + 0x10] = vec26 <00>
	{
		_u32    addr = (0x00000010 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[26].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[26].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[26].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[26].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[26].U64[0] & temp1);
			value2 |= (state.v[26].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


	// 4001b88: <ca9c2001>	LQV       vec28 <00> = [S4 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[28].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[28].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[28].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[28].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[28].U64[0] = state.v[28].U64[0] & ~temp1;
			state.v[28].U64[1] = state.v[28].U64[1] & ~temp2;
			state.v[28].U64[0] = state.v[28].U64[0] | value1;
			state.v[28].U64[1] = state.v[28].U64[1] | value2;
		}
	}


	// 4001b8c: <22730020>	ADDI      S3 = S3 + 0020
	state.r[S3] = state.r[S3] + 0x0020;

_04001B90:
	;
}

void Zelda1DC4_1E14(void)
{

	// 4001dc4: <c9702000>	LQV       vec16 <00> = [T3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[16].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[16].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[16].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[16].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[16].U64[0] = state.v[16].U64[0] & ~temp1;
			state.v[16].U64[1] = state.v[16].U64[1] & ~temp2;
			state.v[16].U64[0] = state.v[16].U64[0] | value1;
			state.v[16].U64[1] = state.v[16].U64[1] | value2;
		}
	}


	// 4001dc8: <4a0e73ac>	VXOR      vec14 = vec14 xor vec14[<none>]
	rsp_cop2_vxor(&state.v[14], &state.v[14], &state.v[14]);


	// 4001dcc: <4a000000>	VMULF     vec00 = ( acc = (vec00 * vec00[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[0].U16[0], state.v[0].U16[0], state.v[0].U16[0], 1, 1, 1, 1)
		VMULF_operation(1, state.v[0].U16[1], state.v[0].U16[1], state.v[0].U16[1], 1, 1, 1, 1)
		VMULF_operation(2, state.v[0].U16[2], state.v[0].U16[2], state.v[0].U16[2], 1, 1, 1, 1)
		VMULF_operation(3, state.v[0].U16[3], state.v[0].U16[3], state.v[0].U16[3], 1, 1, 1, 1)
		VMULF_operation(4, state.v[0].U16[4], state.v[0].U16[4], state.v[0].U16[4], 1, 1, 1, 1)
		VMULF_operation(5, state.v[0].U16[5], state.v[0].U16[5], state.v[0].U16[5], 1, 1, 1, 1)
		VMULF_operation(6, state.v[0].U16[6], state.v[0].U16[6], state.v[0].U16[6], 1, 1, 1, 1)
		VMULF_operation(7, state.v[0].U16[7], state.v[0].U16[7], state.v[0].U16[7], 1, 1, 1, 1)
	}


	// 4001dd0: <4b2fbb88>	VMACF     vec14 = ( acc += (vec23 * vec15[1]) << 1 ) >> 16
	{
		_u16		s2value = state.v[15].U16[6];
		VMACF_operation(0, state.v[14].U16[0], state.v[23].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[23].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[23].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[23].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[23].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[23].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[23].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[23].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001dd4: <4b4fb388>	VMACF     vec14 = ( acc += (vec22 * vec15[2]) << 1 ) >> 16
	{
		_u16		s2value = state.v[15].U16[5];
		VMACF_operation(0, state.v[14].U16[0], state.v[22].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[22].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[22].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[22].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[22].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[22].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[22].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[22].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001dd8: <4b6fab88>	VMACF     vec14 = ( acc += (vec21 * vec15[3]) << 1 ) >> 16
	{
		_u16		s2value = state.v[15].U16[4];
		VMACF_operation(0, state.v[14].U16[0], state.v[21].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[21].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[21].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[21].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[21].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[21].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[21].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[21].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001ddc: <4b8fa388>	VMACF     vec14 = ( acc += (vec20 * vec15[4]) << 1 ) >> 16
	{
		_u16		s2value = state.v[15].U16[3];
		VMACF_operation(0, state.v[14].U16[0], state.v[20].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[20].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[20].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[20].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[20].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[20].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[20].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[20].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001de0: <4baf9b88>	VMACF     vec14 = ( acc += (vec19 * vec15[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[15].U16[2];
		VMACF_operation(0, state.v[14].U16[0], state.v[19].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[19].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[19].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[19].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[19].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[19].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[19].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[19].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001de4: <4bcf9388>	VMACF     vec14 = ( acc += (vec18 * vec15[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[15].U16[1];
		VMACF_operation(0, state.v[14].U16[0], state.v[18].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[18].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[18].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[18].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[18].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[18].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[18].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[18].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001de8: <4bef8b88>	VMACF     vec14 = ( acc += (vec17 * vec15[7]) << 1 ) >> 16
	{
		_u16		s2value = state.v[15].U16[0];
		VMACF_operation(0, state.v[14].U16[0], state.v[17].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[17].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[17].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[17].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[17].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[17].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[17].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[17].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001dec: <4b10c388>	VMACF     vec14 = ( acc += (vec24 * vec16[0]) << 1 ) >> 16
	{
		_u16		s2value = state.v[16].U16[7];
		VMACF_operation(0, state.v[14].U16[0], state.v[24].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[24].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[24].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[24].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[24].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[24].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[24].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[24].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001df0: <4b30cb88>	VMACF     vec14 = ( acc += (vec25 * vec16[1]) << 1 ) >> 16
	{
		_u16		s2value = state.v[16].U16[6];
		VMACF_operation(0, state.v[14].U16[0], state.v[25].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[25].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[25].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[25].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[25].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[25].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[25].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[25].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001df4: <4b50d388>	VMACF     vec14 = ( acc += (vec26 * vec16[2]) << 1 ) >> 16
	{
		_u16		s2value = state.v[16].U16[5];
		VMACF_operation(0, state.v[14].U16[0], state.v[26].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[26].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[26].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[26].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[26].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[26].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[26].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[26].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001df8: <4b70db88>	VMACF     vec14 = ( acc += (vec27 * vec16[3]) << 1 ) >> 16
	{
		_u16		s2value = state.v[16].U16[4];
		VMACF_operation(0, state.v[14].U16[0], state.v[27].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[27].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[27].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[27].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[27].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[27].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[27].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[27].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001dfc: <4b90e388>	VMACF     vec14 = ( acc += (vec28 * vec16[4]) << 1 ) >> 16
	{
		_u16		s2value = state.v[16].U16[3];
		VMACF_operation(0, state.v[14].U16[0], state.v[28].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[28].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[28].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[28].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[28].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[28].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[28].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[28].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001e00: <4bb0eb88>	VMACF     vec14 = ( acc += (vec29 * vec16[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[16].U16[2];
		VMACF_operation(0, state.v[14].U16[0], state.v[29].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[29].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[29].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[29].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[29].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[29].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[29].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[29].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001e04: <4bd0f388>	VMACF     vec14 = ( acc += (vec30 * vec16[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[16].U16[1];
		VMACF_operation(0, state.v[14].U16[0], state.v[30].U16[0], s2value, 0, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[30].U16[1], s2value, 0, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[30].U16[2], s2value, 0, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[30].U16[3], s2value, 0, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[30].U16[4], s2value, 0, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[30].U16[5], s2value, 0, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[30].U16[6], s2value, 0, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[30].U16[7], s2value, 0, 1, 1, 1)
	}


	// 4001e08: <4bf0fb88>	VMACF     vec14 = ( acc += (vec31 * vec16[7]) << 1 ) >> 16
	{
		_u16		s2value = state.v[16].U16[0];
		VMACF_operation(0, state.v[14].U16[0], state.v[31].U16[0], s2value, 1, 1, 1, 1)
		VMACF_operation(1, state.v[14].U16[1], state.v[31].U16[1], s2value, 1, 1, 1, 1)
		VMACF_operation(2, state.v[14].U16[2], state.v[31].U16[2], s2value, 1, 1, 1, 1)
		VMACF_operation(3, state.v[14].U16[3], state.v[31].U16[3], s2value, 1, 1, 1, 1)
		VMACF_operation(4, state.v[14].U16[4], state.v[31].U16[4], s2value, 1, 1, 1, 1)
		VMACF_operation(5, state.v[14].U16[5], state.v[31].U16[5], s2value, 1, 1, 1, 1)
		VMACF_operation(6, state.v[14].U16[6], state.v[31].U16[6], s2value, 1, 1, 1, 1)
		VMACF_operation(7, state.v[14].U16[7], state.v[31].U16[7], s2value, 1, 1, 1, 1)
	}


	// 4001e0c: <21effff0>	ADDI      T7 = T7 + fff0
	state.r[T7] = state.r[T7] + 0xFFFFFFF0;


	// 4001e10: <e96e2000>	SQV       [T3 + 0x00] = vec14 <00>
	{
		_u32    addr = (0x00000000 + state.r[T3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[14].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[14].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[14].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[14].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[14].U64[0] & temp1);
			value2 |= (state.v[14].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


	// 4001e14: <216b0010>	ADDI      T3 = T3 + 0010
	state.r[T3] = state.r[T3] + 0x0010;

_04001E18:
	;
}
