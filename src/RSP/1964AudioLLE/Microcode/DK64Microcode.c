//RSP microcode

#include "../../Shared/rsp_prefix.h"
#include "../../Shared/rsp_macros.h"
#include "Microcode.h"

void _DK64Data(_u32 StartAddress);


void _DK64BootCode(_u32 StartAddress)
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


_04001000:	// 4001000: <09000419>	J         04001064
	state.r[AT] = 0 + 0x0FC0;
	goto _04001064;


_04001004:	// 4001004: <20010fc0>	ADDI      AT = R0 + 0fc0
	state.r[AT] = 0 + 0x0FC0;


_04001008:	// 4001008: <8c220010>	LW        V0 = [AT+0010]
	state.r[V0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000010) & 0xfff);


_0400100C:	// 400100c: <20030f7f>	ADDI      V1 = R0 + 0f7f
	state.r[V1] = 0 + 0x0F7F;


_04001010:	// 4001010: <20071080>	ADDI      A3 = R0 + 1080
	state.r[A3] = 0 + 0x1080;


_04001014:	// 4001014: <40870000>	MTC0      SP memory address = A3
	DMEM_Address = state.r[A3];


_04001018:	// 4001018: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_0400101C:	// 400101c: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001020;
	_DK64Data(0x04001020);
	return;


_04001020:	// 4001020: <40043000>	MFC0      A2 = SP status
	state.r[A0] = 0;


_04001024:	// 4001024: <1480fffe>	BNE       (R0!=A0) --> 1020
	sp_reg_pc = 0x04001024;
	if (state.r[A0] != 0)
	{
		goto _04001020;
	}


_04001028:	// 4001028: <00000000>	NOP       


_0400102C:	// 400102c: <0d00040f>	JAL	    0400103c
	sp_reg_pc = 0x0400102C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400103C;


_04001030:	// 4001030: <00000000>	NOP       


_04001034:	// 4001034: <00e00008>	JR        A3
	{
		_u32	temp = ((state.r[A3] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001038:	// 4001038: <40803800>	MTC0      SP semaphore = R0


_0400103C:	// 400103c: <40082000>	MFC0      A0 = DP CMD DMA start
	state.r[T0] = 0;


_04001040:	// 4001040: <31080080>	ANDI      T0 = T0 & 0080
	state.r[T0] = (state.r[T0] & 0x00000080);


_04001044:	// 4001044: <15000002>	BNE       (R0!=T0) --> 1050
	sp_reg_pc = 0x04001044;
	if (state.r[T0] != 0)
	{
		goto _04001050;
	}


_04001048:	// 4001048: <00000000>	NOP       


_0400104C:	// 400104c: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001050:	// 4001050: <40803800>	MTC0      SP semaphore = R0


_04001054:	// 4001054: <34085200>	ORI       T0 = R0 | 0x5200
	state.r[T0] = (0 | 0x00005200);


_04001058:	// 4001058: <40882000>	MTC0      SP status = T0


_0400105C:	// 400105c: <0000000d>	BREAK
	state.halt = 1;
	return;


_04001060:	// 4001060: <00000000>	NOP       


_04001064:	// 4001064: <8c220004>	LW        V0 = [AT+0004]
	state.r[V0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000004) & 0xfff);


_04001068:	// 4001068: <30420002>	ANDI      V0 = V0 & 0002
	state.r[V0] = (state.r[V0] & 0x00000002);


_0400106C:	// 400106c: <10400007>	BEQ       (R0==V0) --> 108c
	sp_reg_pc = 0x0400106C;
	if (state.r[V0] == 0)
	{
		goto _0400108C;
	}


_04001070:	// 4001070: <00000000>	NOP       


_04001074:	// 4001074: <0d00040f>	JAL	    0400103c
	sp_reg_pc = 0x04001074;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400103C;


_04001078:	// 4001078: <00000000>	NOP       


_0400107C:	// 400107c: <40025800>	MFC0      T3 = SP read DMA length
	state.r[V0] = 0;


_04001080:	// 4001080: <30420100>	ANDI      V0 = V0 & 0100
	state.r[V0] = (state.r[V0] & 0x00000100);


_04001084:	// 4001084: <1c40ffed>	BGTZ      (R0>0) --> 103c
	sp_reg_pc = 0x04001084;
	if((_s32)state.r[V0] > 0)
	{
		goto _0400103C;
	}


_04001088:	// 4001088: <00000000>	NOP       


_0400108C:	// 400108c: <8c220018>	LW        V0 = [AT+0018]
	state.r[V0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000018) & 0xfff);


_04001090:	// 4001090: <8c23001c>	LW        V1 = [AT+001c]
	state.r[V1] = (_s32)Load32_DMEM((state.r[AT] + 0x0000001C) & 0xfff);


_04001094:	// 4001094: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001098:	// 4001098: <401e2800>	MFC0      A1 = unknown
	state.r[S8] = 0;


_0400109C:	// 400109c: <17c0fffe>	BNE       (R0!=S8) --> 1098
	sp_reg_pc = 0x0400109C;
	if (state.r[S8] != 0)
	{
		goto _04001098;
	}


_040010A0:	// 40010a0: <00000000>	NOP       


_040010A4:	// 40010a4: <40800000>	MTC0      SP memory address = R0
	DMEM_Address = 0;


_040010A8:	// 40010a8: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_040010AC:	// 40010ac: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x040010AC;
	DMARead(state.r[V1]);


_040010B0:	// 40010b0: <40043000>	MFC0      A2 = SP status
	state.r[A0] = 0;


_040010B4:	// 40010b4: <1480fffe>	BNE       (R0!=A0) --> 10b0
	sp_reg_pc = 0x040010B4;
	if (state.r[A0] != 0)
	{
		goto _040010B0;
	}


_040010B8:	// 40010b8: <00000000>	NOP       


_040010BC:	// 40010bc: <0d00040f>	JAL	    0400103c
	sp_reg_pc = 0x040010BC;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400103C;


_040010C0:	// 40010c0: <00000000>	NOP       


_040010C4:	// 40010c4: <09000402>	J         04001008
	goto _04001008;


_040010C8:	// 40010c8: <00000000>	NOP       


_040010CC:	// 40010cc: <00000000>	NOP       


_040010D0:	// 40010d0: <001a0dc2>	SRL       AT = K0 >> 23
	state.r[AT] = ((_u32)state.r[K0] >> 23);


_040010D4:	// 40010d4: <302100fe>	ANDI      AT = AT & 00fe
	state.r[AT] = (state.r[AT] & 0x000000FE);


_040010D8:	// 40010d8: <84210000>	LH        AT = [AT+0000]
	state.r[AT] = (_s32)(_s16)Load16_DMEM((state.r[AT] + 0x00000000) & 0xfff);


_040010DC:	// 40010dc: <00200008>	JR        AT
	{
		_u32	temp = ((state.r[AT] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[K1] = state.r[K1] + 0xFFFFFFF8;
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_040010E0:	// 40010e0: <237bfff8>	ADDI      K1 = K1 + fff8
	state.r[K1] = state.r[K1] + 0xFFFFFFF8;


_040010E4:	// 40010e4: <0000000d>	BREAK
	state.halt = 1;
	return;


_040010E8:	// 40010e8: <1fc0fff6>	BGTZ      (R0>0) --> 10c4
	sp_reg_pc = 0x040010E8;
	if((_s32)state.r[S8] > 0)
	{
		state.r[SP] = state.r[SP] + 0x0008;
		goto _040010C4;
	}


_040010EC:	// 40010ec: <23bd0008>	ADDI      SP = SP + 0008
	state.r[SP] = state.r[SP] + 0x0008;


_040010F0:	// 40010f0: <1b600005>	BLEZ      (R0<=0) --> 1108
	sp_reg_pc = 0x040010F0;
	if ((_s32)state.r[K1] <= 0)
	{
		state.r[AT] = (0 | 0x00004000);
		goto _04001108;
	}


_040010F4:	// 40010f4: <34014000>	ORI       AT = R0 | 0x4000
	state.r[AT] = (0 | 0x00004000);


_040010F8:	// 40010f8: <0d00044b>	JAL	    0400112c
	state.r[V0] = (0 + state.r[GP]);
	sp_reg_pc = 0x040010F8;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400112C;


_040010FC:	// 40010fc: <001c1020>	ADD       V0 = R0+GP
	state.r[V0] = (0 + state.r[GP]);


_04001100:	// 4001100: <0900042f>	J         040010bc
	state.r[V0] = 0;
	goto _040010BC;


_04001104:	// 4001104: <40023000>	MFC0      A2 = SP read DMA length
	state.r[V0] = 0;


_04001108:	// 4001108: <40023000>	MFC0      A2 = SP read DMA length
	state.r[V0] = 0;


_0400110C:	// 400110c: <1440fffe>	BNE       (R0!=V0) --> 1108
	sp_reg_pc = 0x0400110C;
	if (state.r[V0] != 0)
	{
		goto _04001108;
	}


_04001110:	// 4001110: <00000000>	NOP       


_04001114:	// 4001114: <40803800>	MTC0      SP semaphore = R0


_04001118:	// 4001118: <40812000>	MTC0      SP status = AT


_0400111C:	// 400111c: <0000000d>	BREAK
	state.halt = 1;
	return;


_04001120:	// 4001120: <00000000>	NOP       


_04001124:	// 4001124: <1000ffff>	BEQ       (R0==R0) --> 1124
	sp_reg_pc = 0x04001124;
	if (0 == 0)
	{
		goto _04001124;
	}


_04001128:	// 4001128: <00000000>	NOP       


_0400112C:	// 400112c: <23e50000>	ADDI      A1 = RA + 0000
	state.r[A1] = state.r[RA] + 0x0000;


_04001130:	// 4001130: <23630000>	ADDI      V1 = K1 + 0000
	state.r[V1] = state.r[K1] + 0x0000;


_04001134:	// 4001134: <2064fec0>	ADDI      A0 = V1 + fec0
	state.r[A0] = state.r[V1] + 0xFFFFFEC0;


_04001138:	// 4001138: <18800002>	BLEZ      (R0<=0) --> 1144
	sp_reg_pc = 0x04001138;
	if ((_s32)state.r[A0] <= 0)
	{
		state.r[AT] = 0 + 0x02B0;
		goto _04001144;
	}


_0400113C:	// 400113c: <200102b0>	ADDI      AT = R0 + 02b0
	state.r[AT] = 0 + 0x02B0;


_04001140:	// 4001140: <20030140>	ADDI      V1 = R0 + 0140
	state.r[V1] = 0 + 0x0140;


_04001144:	// 4001144: <207e0000>	ADDI      S8 = V1 + 0000
	state.r[S8] = state.r[V1] + 0x0000;


_04001148:	// 4001148: <0d000456>	JAL	    04001158
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x04001148;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001158;


_0400114C:	// 400114c: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001150:	// 4001150: <00a00008>	JR        A1
	{
		_u32	temp = ((state.r[A1] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[SP] = 0 + 0x02B0;
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001154:	// 4001154: <201d02b0>	ADDI      SP = R0 + 02b0
	state.r[SP] = 0 + 0x02B0;


_04001158:	// 4001158: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_0400115C:	// 400115c: <1480ffff>	BNE       (R0!=A0) --> 115c
	sp_reg_pc = 0x0400115C;
	if (state.r[A0] != 0)
	{
		state.r[A0] = 0;
		goto _0400115C;
	}


_04001160:	// 4001160: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001164:	// 4001164: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_04001168:	// 4001168: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_0400116C:	// 400116c: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		sp_reg_pc = 0x04001170;
		DMARead(state.r[V1]);
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001170:	// 4001170: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001170;
	DMARead(state.r[V1]);


_04001174:	// 4001174: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001178:	// 4001178: <1480ffff>	BNE       (R0!=A0) --> 1178
	sp_reg_pc = 0x04001178;
	if (state.r[A0] != 0)
	{
		state.r[A0] = 0;
		goto _04001178;
	}


_0400117C:	// 400117c: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001180:	// 4001180: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_04001184:	// 4001184: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001188:	// 4001188: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		DMAWrite(state.r[V1]);
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_0400118C:	// 400118c: <40831800>	MTC0      SP write DMA length = V1
	DMAWrite(state.r[V1]);


_04001190:	// 4001190: <3322ffff>	ANDI      V0 = T9 & ffff
	state.r[V0] = (state.r[T9] & 0x0000FFFF);


_04001194:	// 4001194: <4a01086c>	VXOR      vec01 = vec01 xor vec01[<none>]
	rsp_cop2_vxor(&state.v[1], &state.v[1], &state.v[1]);


_04001198:	// 4001198: <3341ffff>	ANDI      AT = K0 & ffff
	state.r[AT] = (state.r[K0] & 0x0000FFFF);


_0400119C:	// 400119c: <202104f0>	ADDI      AT = AT + 04f0
	state.r[AT] = state.r[AT] + 0x04F0;


_040011A0:	// 40011a0: <e8211800>	SDV       [AT + 0x00] = vec01 <00>
	// In function: SmashBros11AC_11B4
	//Call the function: SmashBros11AC_11B4
	SmashBros11AC_11B4();
	goto _040011AC;

_040011A4:	// 40011a4: <e8211801>	SDV       [AT + 0x04] = vec01 <00>
	ErrorToCallFunction(0x040011A4);

_040011A8:	// 40011a8: <2042fff0>	ADDI      V0 = V0 + fff0
	ErrorToCallFunction(0x040011A8);

_040011AC:	// 40011ac: <1c40fffc>	BGTZ      (R0>0) --> 11a0
	sp_reg_pc = 0x040011AC;
	if((_s32)state.r[V0] > 0)
	{
		state.r[AT] = state.r[AT] + 0x0010;
		goto _040011A0;
	}


_040011B0:	// 40011b0: <20210010>	ADDI      AT = AT + 0010
	state.r[AT] = state.r[AT] + 0x0010;


_040011B4:	// 40011b4: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_040011B8:	// 40011b8: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_040011BC:	// 40011bc: <001a1a00>	SLL       V1 = K0 << 8
	state.r[V1] = ((_u32)state.r[K0] << 8);


_040011C0:	// 40011c0: <00031d02>	SRL       V1 = V1 >> 20
	state.r[V1] = ((_u32)state.r[V1] >> 20);


_040011C4:	// 40011c4: <1060ffc8>	BEQ       (R0==V1) --> 10e8
	sp_reg_pc = 0x040011C4;
	if (state.r[V1] == 0)
	{
		state.r[S8] = state.r[S8] + 0xFFFFFFF8;
		goto _040010E8;
	}


_040011C8:	// 40011c8: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_040011CC:	// 40011cc: <33410fff>	ANDI      AT = K0 & 0fff
	state.r[AT] = (state.r[K0] & 0x00000FFF);


_040011D0:	// 40011d0: <202104f0>	ADDI      AT = AT + 04f0
	state.r[AT] = state.r[AT] + 0x04F0;


_040011D4:	// 40011d4: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_040011D8:	// 40011d8: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_040011DC:	// 40011dc: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_040011E0:	// 40011e0: <0d000456>	JAL	    04001158
	state.r[V0] = state.r[V0] + 0x0000;
	sp_reg_pc = 0x040011E0;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001158;


_040011E4:	// 40011e4: <20420000>	ADDI      V0 = V0 + 0000
	state.r[V0] = state.r[V0] + 0x0000;


_040011E8:	// 40011e8: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_040011EC:	// 40011ec: <1420ffff>	BNE       (R0!=AT) --> 11ec
	sp_reg_pc = 0x040011EC;
	if (state.r[AT] != 0)
	{
		state.r[AT] = 0;
		goto _040011EC;
	}


_040011F0:	// 40011f0: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_040011F4:	// 40011f4: <0900043a>	J         040010e8
	goto _040010E8;


_040011F8:	// 40011f8: <00000000>	NOP       


_040011FC:	// 40011fc: <001a1a00>	SLL       V1 = K0 << 8
	state.r[V1] = ((_u32)state.r[K0] << 8);


_04001200:	// 4001200: <00031d02>	SRL       V1 = V1 >> 20
	state.r[V1] = ((_u32)state.r[V1] >> 20);


_04001204:	// 4001204: <1060ffb8>	BEQ       (R0==V1) --> 10e8
	sp_reg_pc = 0x04001204;
	if (state.r[V1] == 0)
	{
		state.r[S8] = state.r[S8] + 0xFFFFFFF8;
		goto _040010E8;
	}


_04001208:	// 4001208: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_0400120C:	// 400120c: <33410fff>	ANDI      AT = K0 & 0fff
	state.r[AT] = (state.r[K0] & 0x00000FFF);


_04001210:	// 4001210: <202104f0>	ADDI      AT = AT + 04f0
	state.r[AT] = state.r[AT] + 0x04F0;


_04001214:	// 4001214: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001218:	// 4001218: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_0400121C:	// 400121c: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001220:	// 4001220: <0d00045d>	JAL	    04001174
	state.r[V0] = state.r[V0] + 0x0000;
	sp_reg_pc = 0x04001220;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001174;


_04001224:	// 4001224: <20420000>	ADDI      V0 = V0 + 0000
	state.r[V0] = state.r[V0] + 0x0000;


_04001228:	// 4001228: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_0400122C:	// 400122c: <1420ffff>	BNE       (R0!=AT) --> 122c
	sp_reg_pc = 0x0400122C;
	if (state.r[AT] != 0)
	{
		state.r[AT] = 0;
		goto _0400122C;
	}


_04001230:	// 4001230: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_04001234:	// 4001234: <0900043a>	J         040010e8
	goto _040010E8;


_04001238:	// 4001238: <00000000>	NOP       


_0400123C:	// 400123c: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001240:	// 4001240: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001244:	// 4001244: <20420000>	ADDI      V0 = V0 + 0000
	state.r[V0] = state.r[V0] + 0x0000;


_04001248:	// 4001248: <200103f0>	ADDI      AT = R0 + 03f0
	state.r[AT] = 0 + 0x03F0;


_0400124C:	// 400124c: <3343ffff>	ANDI      V1 = K0 & ffff
	state.r[V1] = (state.r[K0] & 0x0000FFFF);


_04001250:	// 4001250: <0d000456>	JAL	    04001158
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x04001250;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001158;


_04001254:	// 4001254: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001258:	// 4001258: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_0400125C:	// 400125c: <1420ffff>	BNE       (R0!=AT) --> 125c
	sp_reg_pc = 0x0400125C;
	if (state.r[AT] != 0)
	{
		state.r[AT] = 0;
		goto _0400125C;
	}


_04001260:	// 4001260: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_04001264:	// 4001264: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_04001268:	// 4001268: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_0400126C:	// 400126c: <001a1c02>	SRL       V1 = K0 >> 16
	state.r[V1] = ((_u32)state.r[K0] >> 16);


_04001270:	// 4001270: <30610004>	ANDI      AT = V1 & 0004
	state.r[AT] = (state.r[V1] & 0x00000004);


_04001274:	// 4001274: <1020000d>	BEQ       (R0==AT) --> 12ac
	sp_reg_pc = 0x04001274;
	if (state.r[AT] == 0)
	{
		state.r[AT] = (state.r[V1] & 0x00000002);
		goto _040012AC;
	}


_04001278:	// 4001278: <30610002>	ANDI      AT = V1 & 0002
	state.r[AT] = (state.r[V1] & 0x00000002);


_0400127C:	// 400127c: <10200006>	BEQ       (R0==AT) --> 1298
	sp_reg_pc = 0x0400127C;
	if (state.r[AT] == 0)
	{
		state.r[V0] = ((_u32)state.r[T9] >> 16);
		goto _04001298;
	}


_04001280:	// 4001280: <00191402>	SRL       V0 = T9 >> 16
	state.r[V0] = ((_u32)state.r[T9] >> 16);


_04001284:	// 4001284: <a71a0050>	SH        [T8+0050] = K0
	Save16_DMEM((_u16)state.r[K0], (state.r[T8] + 0x00000050) & 0xfff);


_04001288:	// 4001288: <a702004c>	SH        [T8+004c] = V0
	Save16_DMEM((_u16)state.r[V0], (state.r[T8] + 0x0000004C) & 0xfff);


_0400128C:	// 400128c: <a719004e>	SH        [T8+004e] = T9
	Save16_DMEM((_u16)state.r[T9], (state.r[T8] + 0x0000004E) & 0xfff);


_04001290:	// 4001290: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_04001294:	// 4001294: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001298:	// 4001298: <a71a0046>	SH        [T8+0046] = K0
	Save16_DMEM((_u16)state.r[K0], (state.r[T8] + 0x00000046) & 0xfff);


_0400129C:	// 400129c: <a7020048>	SH        [T8+0048] = V0
	Save16_DMEM((_u16)state.r[V0], (state.r[T8] + 0x00000048) & 0xfff);


_040012A0:	// 40012a0: <a719004a>	SH        [T8+004a] = T9
	Save16_DMEM((_u16)state.r[T9], (state.r[T8] + 0x0000004A) & 0xfff);


_040012A4:	// 40012a4: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_040012A8:	// 40012a8: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_040012AC:	// 40012ac: <00191402>	SRL       V0 = T9 >> 16
	state.r[V0] = ((_u32)state.r[T9] >> 16);


_040012B0:	// 40012b0: <a71a0040>	SH        [T8+0040] = K0
	Save16_DMEM((_u16)state.r[K0], (state.r[T8] + 0x00000040) & 0xfff);


_040012B4:	// 40012b4: <a7020042>	SH        [T8+0042] = V0
	Save16_DMEM((_u16)state.r[V0], (state.r[T8] + 0x00000042) & 0xfff);


_040012B8:	// 40012b8: <a7190044>	SH        [T8+0044] = T9
	Save16_DMEM((_u16)state.r[T9], (state.r[T8] + 0x00000044) & 0xfff);


_040012BC:	// 40012bc: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_040012C0:	// 40012c0: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_040012C4:	// 40012c4: <20010170>	ADDI      AT = R0 + 0170
	state.r[AT] = 0 + 0x0170;


_040012C8:	// 40012c8: <200404f0>	ADDI      A0 = R0 + 04f0
	state.r[A0] = 0 + 0x04F0;


_040012CC:	// 40012cc: <200209d0>	ADDI      V0 = R0 + 09d0
	state.r[V0] = 0 + 0x09D0;


_040012D0:	// 40012d0: <20030b40>	ADDI      V1 = R0 + 0b40
	state.r[V1] = 0 + 0x0B40;


_040012D4:	// 40012d4: <c8412000>	LQV       vec01 <00> = [V0 + 0x00]
	// In function: SmashBros12E4_1334
	//Call the function: SmashBros12E4_1334
	SmashBros12E4_1334();
	goto _04001328;

_040012D8:	// 40012d8: <c8622000>	LQV       vec02 <00> = [V1 + 0x00]
	ErrorToCallFunction(0x040012D8);

_040012DC:	// 40012dc: <2021fff0>	ADDI      AT = AT + fff0
	ErrorToCallFunction(0x040012DC);

_040012E0:	// 40012e0: <20420010>	ADDI      V0 = V0 + 0010
	ErrorToCallFunction(0x040012E0);

_040012E4:	// 40012e4: <20630010>	ADDI      V1 = V1 + 0010
	ErrorToCallFunction(0x040012E4);

_040012E8:	// 40012e8: <e8810800>	SSV       [A0 + 0x00] = vec01 <00>
	ErrorToCallFunction(0x040012E8);

_040012EC:	// 40012ec: <e8820801>	SSV       [A0 + 0x02] = vec02 <00>
	ErrorToCallFunction(0x040012EC);

_040012F0:	// 40012f0: <e8810902>	SSV       [A0 + 0x04] = vec01 <02>
	ErrorToCallFunction(0x040012F0);

_040012F4:	// 40012f4: <e8820903>	SSV       [A0 + 0x06] = vec02 <02>
	ErrorToCallFunction(0x040012F4);

_040012F8:	// 40012f8: <e8810a04>	SSV       [A0 + 0x08] = vec01 <04>
	ErrorToCallFunction(0x040012F8);

_040012FC:	// 40012fc: <e8820a05>	SSV       [A0 + 0x0a] = vec02 <04>
	ErrorToCallFunction(0x040012FC);

_04001300:	// 4001300: <e8810b06>	SSV       [A0 + 0x0c] = vec01 <06>
	ErrorToCallFunction(0x04001300);

_04001304:	// 4001304: <e8820b07>	SSV       [A0 + 0x0e] = vec02 <06>
	ErrorToCallFunction(0x04001304);

_04001308:	// 4001308: <e8810c08>	SSV       [A0 + 0x10] = vec01 <08>
	ErrorToCallFunction(0x04001308);

_0400130C:	// 400130c: <e8820c09>	SSV       [A0 + 0x12] = vec02 <08>
	ErrorToCallFunction(0x0400130C);

_04001310:	// 4001310: <e8810d0a>	SSV       [A0 + 0x14] = vec01 <10>
	ErrorToCallFunction(0x04001310);

_04001314:	// 4001314: <e8820d0b>	SSV       [A0 + 0x16] = vec02 <10>
	ErrorToCallFunction(0x04001314);

_04001318:	// 4001318: <e8810e0c>	SSV       [A0 + 0x18] = vec01 <12>
	ErrorToCallFunction(0x04001318);

_0400131C:	// 400131c: <e8820e0d>	SSV       [A0 + 0x1a] = vec02 <12>
	ErrorToCallFunction(0x0400131C);

_04001320:	// 4001320: <e8810f0e>	SSV       [A0 + 0x1c] = vec01 <14>
	ErrorToCallFunction(0x04001320);

_04001324:	// 4001324: <e8820f0f>	SSV       [A0 + 0x1e] = vec02 <14>
	ErrorToCallFunction(0x04001324);

_04001328:	// 4001328: <1c20ffea>	BGTZ      (R0>0) --> 12d4
	sp_reg_pc = 0x04001328;
	if((_s32)state.r[AT] > 0)
	{
		state.r[A0] = state.r[A0] + 0x0020;
		goto _040012D4;
	}


_0400132C:	// 400132c: <20840020>	ADDI      A0 = A0 + 0020
	state.r[A0] = state.r[A0] + 0x0020;


_04001330:	// 4001330: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_04001334:	// 4001334: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001338:	// 4001338: <3321ffff>	ANDI      AT = T9 & ffff
	state.r[AT] = (state.r[T9] & 0x0000FFFF);


_0400133C:	// 400133c: <3342ffff>	ANDI      V0 = K0 & ffff
	state.r[V0] = (state.r[K0] & 0x0000FFFF);


_04001340:	// 4001340: <204204f0>	ADDI      V0 = V0 + 04f0
	state.r[V0] = state.r[V0] + 0x04F0;


_04001344:	// 4001344: <00191c02>	SRL       V1 = T9 >> 16
	state.r[V1] = ((_u32)state.r[T9] >> 16);


_04001348:	// 4001348: <206304f0>	ADDI      V1 = V1 + 04f0
	state.r[V1] = state.r[V1] + 0x04F0;


_0400134C:	// 400134c: <c8411800>	LDV       vec01 <00> = [V0 + 0x00]
	// In function: Mario1424_1438
	//Call the function: Mario1424_1438
	Mario1424_1438();
	goto _04001364;

_04001350:	// 4001350: <c8421801>	LDV       vec02 <00> = [V0 + 0x08]
	ErrorToCallFunction(0x04001350);

_04001354:	// 4001354: <2021fff0>	ADDI      AT = AT + fff0
	ErrorToCallFunction(0x04001354);

_04001358:	// 4001358: <20420010>	ADDI      V0 = V0 + 0010
	ErrorToCallFunction(0x04001358);

_0400135C:	// 400135c: <e8611800>	SDV       [V1 + 0x00] = vec01 <00>
	ErrorToCallFunction(0x0400135C);

_04001360:	// 4001360: <e8621801>	SDV       [V1 + 0x04] = vec02 <00>
	ErrorToCallFunction(0x04001360);

_04001364:	// 4001364: <1c20fff9>	BGTZ      (R0>0) --> 134c
	sp_reg_pc = 0x04001364;
	if((_s32)state.r[AT] > 0)
	{
		state.r[V1] = state.r[V1] + 0x0010;
		goto _0400134C;
	}


_04001368:	// 4001368: <20630010>	ADDI      V1 = V1 + 0010
	state.r[V1] = state.r[V1] + 0x0010;


_0400136C:	// 400136c: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_04001370:	// 4001370: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001374:	// 4001374: <00190a00>	SLL       AT = T9 << 8
	state.r[AT] = ((_u32)state.r[T9] << 8);


_04001378:	// 4001378: <00010a02>	SRL       AT = AT >> 8
	state.r[AT] = ((_u32)state.r[AT] >> 8);


_0400137C:	// 400137c: <20210000>	ADDI      AT = AT + 0000
	state.r[AT] = state.r[AT] + 0x0000;


_04001380:	// 4001380: <ac01000e>	SW        [R0+000e] = AT
	Save32_DMEM((_u32)state.r[AT], (0 + 0x0000000E) & 0xfff);


_04001384:	// 4001384: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_04001388:	// 4001388: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_0400138C:	// 400138c: <c81f2005>	LQV       vec31 <00> = [R0 + 0x50]
	{
		_u32 addr = (0x00000050 + 0);
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


_04001390:	// 4001390: <0019bb02>	SRL       S7 = T9 >> 12
	state.r[S7] = ((_u32)state.r[T9] >> 12);


_04001394:	// 4001394: <4a19ce6c>	VXOR      vec25 = vec25 xor vec25[<none>]
	rsp_cop2_vxor(&state.v[25], &state.v[25], &state.v[25]);


_04001398:	// 4001398: <32f7000f>	ANDI      S7 = S7 & 000f
	state.r[S7] = (state.r[S7] & 0x0000000F);


_0400139C:	// 400139c: <4a18c62c>	VXOR      vec24 = vec24 xor vec24[<none>]
	rsp_cop2_vxor(&state.v[24], &state.v[24], &state.v[24]);


_040013A0:	// 40013a0: <22f704f0>	ADDI      S7 = S7 + 04f0
	state.r[S7] = state.r[S7] + 0x04F0;


_040013A4:	// 40013a4: <4a0d6b6c>	VXOR      vec13 = vec13 xor vec13[<none>]
	rsp_cop2_vxor(&state.v[13], &state.v[13], &state.v[13]);


_040013A8:	// 40013a8: <33210fff>	ANDI      AT = T9 & 0fff
	state.r[AT] = (state.r[T9] & 0x00000FFF);


_040013AC:	// 40013ac: <4a0e73ac>	VXOR      vec14 = vec14 xor vec14[<none>]
	rsp_cop2_vxor(&state.v[14], &state.v[14], &state.v[14]);


_040013B0:	// 40013b0: <202104f0>	ADDI      AT = AT + 04f0
	state.r[AT] = state.r[AT] + 0x04F0;


_040013B4:	// 40013b4: <4a0f7bec>	VXOR      vec15 = vec15 xor vec15[<none>]
	rsp_cop2_vxor(&state.v[15], &state.v[15], &state.v[15]);


_040013B8:	// 40013b8: <0019ac02>	SRL       S5 = T9 >> 16
	state.r[S5] = ((_u32)state.r[T9] >> 16);


_040013BC:	// 40013bc: <4a10842c>	VXOR      vec16 = vec16 xor vec16[<none>]
	rsp_cop2_vxor(&state.v[16], &state.v[16], &state.v[16]);


_040013C0:	// 40013c0: <32b50fff>	ANDI      S5 = S5 & 0fff
	state.r[S5] = (state.r[S5] & 0x00000FFF);


_040013C4:	// 40013c4: <4a118c6c>	VXOR      vec17 = vec17 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[17], &state.v[17]);


_040013C8:	// 40013c8: <001aa200>	SLL       S4 = K0 << 8
	state.r[S4] = ((_u32)state.r[K0] << 8);


_040013CC:	// 40013cc: <4a1294ac>	VXOR      vec18 = vec18 xor vec18[<none>]
	rsp_cop2_vxor(&state.v[18], &state.v[18], &state.v[18]);


_040013D0:	// 40013d0: <0014a202>	SRL       S4 = S4 >> 8
	state.r[S4] = ((_u32)state.r[S4] >> 8);


_040013D4:	// 40013d4: <4a139cec>	VXOR      vec19 = vec19 xor vec19[<none>]
	rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);


_040013D8:	// 40013d8: <2003001f>	ADDI      V1 = R0 + 001f
	state.r[V1] = 0 + 0x001F;


_040013DC:	// 40013dc: <00196f02>	SRL       T5 = T9 >> 28
	state.r[T5] = ((_u32)state.r[T9] >> 28);


_040013E0:	// 40013e0: <31a20001>	ANDI      V0 = T5 & 0001
	state.r[V0] = (state.r[T5] & 0x00000001);


_040013E4:	// 40013e4: <1c400017>	BGTZ      (R0>0) --> 1444
	sp_reg_pc = 0x040013E4;
	if((_s32)state.r[V0] > 0)
	{
		state.r[S6] = state.r[S7] + 0x0001;
		goto _04001444;
	}


_040013E8:	// 40013e8: <22f60001>	ADDI      S6 = S7 + 0001
	state.r[S6] = state.r[S7] + 0x0001;


_040013EC:	// 40013ec: <31a20002>	ANDI      V0 = T5 & 0002
	state.r[V0] = (state.r[T5] & 0x00000002);


_040013F0:	// 40013f0: <10400002>	BEQ       (R0==V0) --> 13fc
	sp_reg_pc = 0x040013F0;
	if (state.r[V0] == 0)
	{
		state.r[V0] = state.r[S4] + 0x0000;
		goto _040013FC;
	}


_040013F4:	// 40013f4: <22820000>	ADDI      V0 = S4 + 0000
	state.r[V0] = state.r[S4] + 0x0000;


_040013F8:	// 40013f8: <8c02000e>	LW        V0 = [R0+000e]
	state.r[V0] = (_s32)Load32_DMEM((0 + 0x0000000E) & 0xfff);


_040013FC:	// 40013fc: <400d2800>	MFC0      A1 = DP buffer busy counter
	state.r[T5] = 0;


_04001400:	// 4001400: <15a0ffff>	BNE       (R0!=T5) --> 1400
	sp_reg_pc = 0x04001400;
	if (state.r[T5] != 0)
	{
		state.r[T5] = 0;
		goto _04001400;
	}


_04001404:	// 4001404: <400d2800>	MFC0      A1 = DP buffer busy counter
	state.r[T5] = 0;


_04001408:	// 4001408: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_0400140C:	// 400140c: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001410:	// 4001410: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001410;
	DMARead(state.r[V1]);


_04001414:	// 4001414: <20130020>	ADDI      S3 = R0 + 0020
	state.r[S3] = 0 + 0x0020;


_04001418:	// 4001418: <201203f0>	ADDI      S2 = R0 + 03f0
	state.r[S2] = 0 + 0x03F0;


_0400141C:	// 400141c: <ca791800>	LDV       vec25 <00> = [S3 + 0x00]
	rsp_ldv_compiler((state.r[S3] & 0xFFF), 25, 8);


_04001420:	// 4001420: <ca781c00>	LDV       vec24 <08> = [S3 + 0x00]
	rsp_ldv_compiler((state.r[S3] & 0xFFF), 24, 0);


_04001424:	// 4001424: <ca771801>	LDV       vec23 <00> = [S3 + 0x08]
	rsp_ldv_compiler((state.r[S3] + 0x00000008) & 0xFFF, 23, 8);


_04001428:	// 4001428: <ca771c01>	LDV       vec23 <08> = [S3 + 0x08]
	rsp_ldv_compiler((state.r[S3] + 0x00000008) & 0xFFF, 23, 0);


_0400142C:	// 400142c: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001430:	// 4001430: <14a0ffff>	BNE       (R0!=A1) --> 1430
	sp_reg_pc = 0x04001430;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001430;
	}


_04001434:	// 4001434: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001438:	// 4001438: <00000020>	ADD       R0 = R0+R0
	; //0 = (0 + 0);



_0400143C:	// 400143c: <0900051a>	J         04001468
	{
		_u32 addr = (0x00000010 + state.r[AT]);
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
	goto _04001468;


_04001440:	// 4001440: <c83b2001>	LQV       vec27 <00> = [AT + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[AT]);
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


_04001444:	// 4001444: <20130020>	ADDI      S3 = R0 + 0020
	state.r[S3] = 0 + 0x0020;


_04001448:	// 4001448: <4a1bdeec>	VXOR      vec27 = vec27 xor vec27[<none>]
	rsp_cop2_vxor(&state.v[27], &state.v[27], &state.v[27]);


_0400144C:	// 400144c: <201203f0>	ADDI      S2 = R0 + 03f0
	state.r[S2] = 0 + 0x03F0;


_04001450:	// 4001450: <ca791800>	LDV       vec25 <00> = [S3 + 0x00]
	rsp_ldv_compiler((state.r[S3] & 0xFFF), 25, 8);


_04001454:	// 4001454: <ca781c00>	LDV       vec24 <08> = [S3 + 0x00]
	rsp_ldv_compiler((state.r[S3] & 0xFFF), 24, 0);


_04001458:	// 4001458: <ca771801>	LDV       vec23 <00> = [S3 + 0x08]
	rsp_ldv_compiler((state.r[S3] + 0x00000008) & 0xFFF, 23, 8);


_0400145C:	// 400145c: <ca771c01>	LDV       vec23 <08> = [S3 + 0x08]
	rsp_ldv_compiler((state.r[S3] + 0x00000008) & 0xFFF, 23, 0);


_04001460:	// 4001460: <e83b2000>	SQV       [AT + 0x00] = vec27 <00>
	{
		_u32    addr = (0x00000000 + state.r[AT]);
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


_04001464:	// 4001464: <e83b2001>	SQV       [AT + 0x10] = vec27 <00>
	{
		_u32    addr = (0x00000010 + state.r[AT]);
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


_04001468:	// 4001468: <12a0006b>	BEQ       (R0==S5) --> 1618
	sp_reg_pc = 0x04001468;
	if (state.r[S5] == 0)
	{
		state.r[AT] = state.r[AT] + 0x0020;
		goto _04001618;
	}


_0400146C:	// 400146c: <20210020>	ADDI      AT = AT + 0020
	state.r[AT] = state.r[AT] + 0x0020;


_04001470:	// 4001470: <cacc1800>	LDV       vec12 <00> = [S6 + 0x00]
	rsp_ldv_compiler((state.r[S6] & 0xFFF), 12, 8);


_04001474:	// 4001474: <92ea0000>	LBU       T2 = [S7+0000]
	state.r[T2] = (_s32)(_u8)pDMEM[((state.r[S7] + 0x00000000) & 0xfff) ^ 3];


_04001478:	// 4001478: <200d000c>	ADDI      T5 = R0 + 000c
	state.r[T5] = 0 + 0x000C;


_0400147C:	// 400147c: <200c0001>	ADDI      T4 = R0 + 0001
	state.r[T4] = 0 + 0x0001;


_04001480:	// 4001480: <314e000f>	ANDI      T6 = T2 & 000f
	state.r[T6] = (state.r[T2] & 0x0000000F);


_04001484:	// 4001484: <000e7140>	SLL       T6 = T6 << 5
	state.r[T6] = ((_u32)state.r[T6] << 5);


_04001488:	// 4001488: <4b0ccaa8>	VAND      vec10 = vec25 and vec12[0]
	rsp_cop2_vand_element(&state.v[10], &state.v[25], &state.v[12].U16[7]);


_0400148C:	// 400148c: <01d28020>	ADD       S0 = T6+S2
	state.r[S0] = (state.r[T6] + state.r[S2]);


_04001490:	// 4001490: <4b2cc268>	VAND      vec09 = vec24 and vec12[1]
	rsp_cop2_vand_element(&state.v[9], &state.v[24], &state.v[12].U16[6]);


_04001494:	// 4001494: <000a8902>	SRL       S1 = T2 >> 4
	state.r[S1] = ((_u32)state.r[T2] >> 4);


_04001498:	// 4001498: <4b4cca28>	VAND      vec08 = vec25 and vec12[2]
	rsp_cop2_vand_element(&state.v[8], &state.v[25], &state.v[12].U16[5]);


_0400149C:	// 400149c: <01b18822>	SUB       S1 = T5-S1
	state.r[S1] = (state.r[T5] - state.r[S1]);


_040014A0:	// 40014a0: <4b6cc1e8>	VAND      vec07 = vec24 and vec12[3]
	rsp_cop2_vand_element(&state.v[7], &state.v[24], &state.v[12].U16[4]);


_040014A4:	// 40014a4: <222dffff>	ADDI      T5 = S1 + ffff
	state.r[T5] = state.r[S1] + 0xFFFFFFFF;


_040014A8:	// 40014a8: <000c63c0>	SLL       T4 = T4 << 15
	state.r[T4] = ((_u32)state.r[T4] << 15);


_040014AC:	// 40014ac: <01ac5806>	SRLV      T4 = T3 >> T5
	state.r[T3] = ((_u32)state.r[T4] >> (state.r[T5] & 0x1F));


_040014B0:	// 40014b0: <488bb000>	MTC2      vec22 <00> = T3
	state.v[22].U16[7] = (_u16)state.r[T3];


_040014B4:	// 40014b4: <ca152000>	LQV       vec21 <00> = [S0 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S0]);
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


_040014B8:	// 40014b8: <ca142001>	LQV       vec20 <00> = [S0 + 0x10]
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


_040014BC:	// 40014bc: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_040014C0:	// 40014c0: <ca132802>	LRV       vec19 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[19].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014C4:	// 40014c4: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_040014C8:	// 40014c8: <ca122802>	LRV       vec18 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[18].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014CC:	// 40014cc: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_040014D0:	// 40014d0: <ca112802>	LRV       vec17 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[17].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014D4:	// 40014d4: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_040014D8:	// 40014d8: <ca102802>	LRV       vec16 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[16].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014DC:	// 40014dc: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_040014E0:	// 40014e0: <ca0f2802>	LRV       vec15 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[15].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014E4:	// 40014e4: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_040014E8:	// 40014e8: <ca0e2802>	LRV       vec14 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[14].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014EC:	// 40014ec: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_040014F0:	// 40014f0: <ca0d2802>	LRV       vec13 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[13].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014F4:	// 40014f4: <22d60009>	ADDI      S6 = S6 + 0009
	// In function: SmashBros1514_1630
	//Call the function: SmashBros1514_1630
	SmashBros1514_1630();
	goto _04001618;

_040014F8:	// 40014f8: <4a175786>	VMUDN     vec30 = ( acc = vec10 * vec23[<none>]      ) >> 16
	ErrorToCallFunction(0x040014F8);

_040014FC:	// 40014fc: <22f70009>	ADDI      S7 = S7 + 0009
	ErrorToCallFunction(0x040014FC);

_04001500:	// 4001500: <4a174f8e>	VMADN     vec30 = ( acc+= vec09 * vec23[<none>] )
	ErrorToCallFunction(0x04001500);

_04001504:	// 4001504: <92ea0000>	LBU       T2 = [S7+0000]
	ErrorToCallFunction(0x04001504);

_04001508:	// 4001508: <4a174746>	VMUDN     vec29 = ( acc = vec08 * vec23[<none>]      ) >> 16
	ErrorToCallFunction(0x04001508);

_0400150C:	// 400150c: <cacc1800>	LDV       vec12 <00> = [S6 + 0x00]
	ErrorToCallFunction(0x0400150C);

_04001510:	// 4001510: <4a173f4e>	VMADN     vec29 = ( acc+= vec07 * vec23[<none>] )
	ErrorToCallFunction(0x04001510);

_04001514:	// 4001514: <200d000c>	ADDI      T5 = R0 + 000c
	ErrorToCallFunction(0x04001514);

_04001518:	// 4001518: <1a200003>	BLEZ      (R0<=0) --> 1528
	ErrorToCallFunction(0x04001518);

_0400151C:	// 400151c: <314e000f>	ANDI      T6 = T2 & 000f
	ErrorToCallFunction(0x0400151C);

_04001520:	// 4001520: <4b16f785>	VMUDM     vec30 = ( acc = vec30 * vec22[0] ) >> 16
	ErrorToCallFunction(0x04001520);

_04001524:	// 4001524: <4b16ef45>	VMUDM     vec29 = ( acc = vec29 * vec22[0] ) >> 16
	ErrorToCallFunction(0x04001524);

_04001528:	// 4001528: <000e7140>	SLL       T6 = T6 << 5
	ErrorToCallFunction(0x04001528);

_0400152C:	// 400152c: <4bdbaac7>	VMUDH     vec11 = ( acc = (vec21 * vec27[6]) << 16) >> 16
	ErrorToCallFunction(0x0400152C);

_04001530:	// 4001530: <01d28020>	ADD       S0 = T6+S2
	ErrorToCallFunction(0x04001530);

_04001534:	// 4001534: <4bfba2cf>	VMADH     vec11 = ( acc+= (vec20 * vec27[7]) << 16) >> 16
	ErrorToCallFunction(0x04001534);

_04001538:	// 4001538: <4b1e9acf>	VMADH     vec11 = ( acc+= (vec19 * vec30[0]) << 16) >> 16
	ErrorToCallFunction(0x04001538);

_0400153C:	// 400153c: <4b3e92cf>	VMADH     vec11 = ( acc+= (vec18 * vec30[1]) << 16) >> 16
	ErrorToCallFunction(0x0400153C);

_04001540:	// 4001540: <000a8902>	SRL       S1 = T2 >> 4
	ErrorToCallFunction(0x04001540);

_04001544:	// 4001544: <4b5e8acf>	VMADH     vec11 = ( acc+= (vec17 * vec30[2]) << 16) >> 16
	ErrorToCallFunction(0x04001544);

_04001548:	// 4001548: <4b7e82cf>	VMADH     vec11 = ( acc+= (vec16 * vec30[3]) << 16) >> 16
	ErrorToCallFunction(0x04001548);

_0400154C:	// 400154c: <01b18822>	SUB       S1 = T5-S1
	ErrorToCallFunction(0x0400154C);

_04001550:	// 4001550: <4b9e7f0f>	VMADH     vec28 = ( acc+= (vec15 * vec30[4]) << 16) >> 16
	ErrorToCallFunction(0x04001550);

_04001554:	// 4001554: <222dffff>	ADDI      T5 = S1 + ffff
	ErrorToCallFunction(0x04001554);

_04001558:	// 4001558: <4bbe72cf>	VMADH     vec11 = ( acc+= (vec14 * vec30[5]) << 16) >> 16
	ErrorToCallFunction(0x04001558);

_0400155C:	// 400155c: <4bde6acf>	VMADH     vec11 = ( acc+= (vec13 * vec30[6]) << 16) >> 16
	ErrorToCallFunction(0x0400155C);

_04001560:	// 4001560: <4b7ff2cf>	VMADH     vec11 = ( acc+= (vec30 * vec31[3]) << 16) >> 16
	ErrorToCallFunction(0x04001560);

_04001564:	// 4001564: <01ac5806>	SRLV      T4 = T3 >> T5
	ErrorToCallFunction(0x04001564);

_04001568:	// 4001568: <4b3c369d>	VSAW $v26, $v6, $v28[[1]]
	ErrorToCallFunction(0x04001568);

_0400156C:	// 400156c: <488bb000>	MTC2      vec22 <00> = T3
	ErrorToCallFunction(0x0400156C);

_04001570:	// 4001570: <4b1c371d>	VSAW $v28, $v6, $v28[[0]]
	ErrorToCallFunction(0x04001570);

_04001574:	// 4001574: <4b0ccaa8>	VAND      vec10 = vec25 and vec12[0]
	ErrorToCallFunction(0x04001574);

_04001578:	// 4001578: <4b2cc268>	VAND      vec09 = vec24 and vec12[1]
	ErrorToCallFunction(0x04001578);

_0400157C:	// 400157c: <4b4cca28>	VAND      vec08 = vec25 and vec12[2]
	ErrorToCallFunction(0x0400157C);

_04001580:	// 4001580: <4b6cc1e8>	VAND      vec07 = vec24 and vec12[3]
	ErrorToCallFunction(0x04001580);

_04001584:	// 4001584: <4b3fd2c6>	VMUDN     vec11 = ( acc = vec26 * vec31[1]      ) >> 16
	ErrorToCallFunction(0x04001584);

_04001588:	// 4001588: <4b3fe70f>	VMADH     vec28 = ( acc+= (vec28 * vec31[1]) << 16) >> 16
	ErrorToCallFunction(0x04001588);

_0400158C:	// 400158c: <4b1d9ac7>	VMUDH     vec11 = ( acc = (vec19 * vec29[0]) << 16) >> 16
	ErrorToCallFunction(0x0400158C);

_04001590:	// 4001590: <220ffffe>	ADDI      T7 = S0 + fffe
	ErrorToCallFunction(0x04001590);

_04001594:	// 4001594: <4b3d92cf>	VMADH     vec11 = ( acc+= (vec18 * vec29[1]) << 16) >> 16
	ErrorToCallFunction(0x04001594);

_04001598:	// 4001598: <c9f32802>	LRV       vec19 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x04001598);

_0400159C:	// 400159c: <4b5d8acf>	VMADH     vec11 = ( acc+= (vec17 * vec29[2]) << 16) >> 16
	ErrorToCallFunction(0x0400159C);

_040015A0:	// 40015a0: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x040015A0);

_040015A4:	// 40015a4: <4b7d82cf>	VMADH     vec11 = ( acc+= (vec16 * vec29[3]) << 16) >> 16
	ErrorToCallFunction(0x040015A4);

_040015A8:	// 40015a8: <c9f22802>	LRV       vec18 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x040015A8);

_040015AC:	// 40015ac: <4b9d7acf>	VMADH     vec11 = ( acc+= (vec15 * vec29[4]) << 16) >> 16
	ErrorToCallFunction(0x040015AC);

_040015B0:	// 40015b0: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x040015B0);

_040015B4:	// 40015b4: <4bbd72cf>	VMADH     vec11 = ( acc+= (vec14 * vec29[5]) << 16) >> 16
	ErrorToCallFunction(0x040015B4);

_040015B8:	// 40015b8: <c9f12802>	LRV       vec17 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x040015B8);

_040015BC:	// 40015bc: <4bdd6acf>	VMADH     vec11 = ( acc+= (vec13 * vec29[6]) << 16) >> 16
	ErrorToCallFunction(0x040015BC);

_040015C0:	// 40015c0: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x040015C0);

_040015C4:	// 40015c4: <4b7feacf>	VMADH     vec11 = ( acc+= (vec29 * vec31[3]) << 16) >> 16
	ErrorToCallFunction(0x040015C4);

_040015C8:	// 40015c8: <c9f02802>	LRV       vec16 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x040015C8);

_040015CC:	// 40015cc: <4bdcaacf>	VMADH     vec11 = ( acc+= (vec21 * vec28[6]) << 16) >> 16
	ErrorToCallFunction(0x040015CC);

_040015D0:	// 40015d0: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x040015D0);

_040015D4:	// 40015d4: <4bfca2cf>	VMADH     vec11 = ( acc+= (vec20 * vec28[7]) << 16) >> 16
	ErrorToCallFunction(0x040015D4);

_040015D8:	// 40015d8: <c9ef2802>	LRV       vec15 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x040015D8);

_040015DC:	// 40015dc: <4b3b369d>	VSAW $v26, $v6, $v27[[1]]
	ErrorToCallFunction(0x040015DC);

_040015E0:	// 40015e0: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x040015E0);

_040015E4:	// 40015e4: <4b1b36dd>	VSAW $v27, $v6, $v27[[0]]
	ErrorToCallFunction(0x040015E4);

_040015E8:	// 40015e8: <c9ee2802>	LRV       vec14 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x040015E8);

_040015EC:	// 40015ec: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x040015EC);

_040015F0:	// 40015f0: <c9ed2802>	LRV       vec13 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x040015F0);

_040015F4:	// 40015f4: <ca152000>	LQV       vec21 <00> = [S0 + 0x00]
	ErrorToCallFunction(0x040015F4);

_040015F8:	// 40015f8: <4b3fd2c6>	VMUDN     vec11 = ( acc = vec26 * vec31[1]      ) >> 16
	ErrorToCallFunction(0x040015F8);

_040015FC:	// 40015fc: <ca142001>	LQV       vec20 <00> = [S0 + 0x10]
	ErrorToCallFunction(0x040015FC);

_04001600:	// 4001600: <4b3fdecf>	VMADH     vec27 = ( acc+= (vec27 * vec31[1]) << 16) >> 16
	ErrorToCallFunction(0x04001600);

_04001604:	// 4001604: <22b5ffe0>	ADDI      S5 = S5 + ffe0
	ErrorToCallFunction(0x04001604);

_04001608:	// 4001608: <e83c2000>	SQV       [AT + 0x00] = vec28 <00>
	ErrorToCallFunction(0x04001608);

_0400160C:	// 400160c: <20210020>	ADDI      AT = AT + 0020
	ErrorToCallFunction(0x0400160C);

_04001610:	// 4001610: <1ea0ffb8>	BGTZ      (R0>0) --> 14f4
	ErrorToCallFunction(0x04001610);

_04001614:	// 4001614: <e83b207f>	SQV       [AT + 0xf0] = vec27 <00>
	ErrorToCallFunction(0x04001614);

_04001618:	// 4001618: <2021ffe0>	ADDI      AT = AT + ffe0
	state.r[AT] = state.r[AT] + 0xFFFFFFE0;


_0400161C:	// 400161c: <0d00045d>	JAL	    04001174
	state.r[V0] = state.r[S4] + 0x0000;
	sp_reg_pc = 0x0400161C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001174;


_04001620:	// 4001620: <22820000>	ADDI      V0 = S4 + 0000
	state.r[V0] = state.r[S4] + 0x0000;


_04001624:	// 4001624: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001628:	// 4001628: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_0400162C:	// 400162c: <14a0ffff>	BNE       (R0!=A1) --> 162c
	sp_reg_pc = 0x0400162C;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _0400162C;
	}


_04001630:	// 4001630: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001634:	// 4001634: <0900043a>	J         040010e8
	; //0 = (0 & 0);

	goto _040010E8;


_04001638:	// 4001638: <00000024>	AND       R0 = R0 & R0
	; //0 = (0 & 0);



_0400163C:	// 400163c: <00199e02>	SRL       S3 = T9 >> 24
	state.r[S3] = ((_u32)state.r[T9] >> 24);


_04001640:	// 4001640: <201403f0>	ADDI      S4 = R0 + 03f0
	state.r[S4] = 0 + 0x03F0;


_04001644:	// 4001644: <4a15ad6c>	VXOR      vec21 = vec21 xor vec21[<none>]
	rsp_cop2_vxor(&state.v[21], &state.v[21], &state.v[21]);


_04001648:	// 4001648: <12600002>	BEQ       (R0==S3) --> 1654
	sp_reg_pc = 0x04001648;
	if (state.r[S3] == 0)
	{
		state.r[S7] = 0 + 0x04F0;
		goto _04001654;
	}


_0400164C:	// 400164c: <201704f0>	ADDI      S7 = R0 + 04f0
	state.r[S7] = 0 + 0x04F0;


_04001650:	// 4001650: <20170660>	ADDI      S7 = R0 + 0660
	state.r[S7] = 0 + 0x0660;


_04001654:	// 4001654: <ca9c2001>	LQV       vec28 <00> = [S4 + 0x10]
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


_04001658:	// 4001658: <4a16b5ac>	VXOR      vec22 = vec22 xor vec22[<none>]
	rsp_cop2_vxor(&state.v[22], &state.v[22], &state.v[22]);


_0400165C:	// 400165c: <489a9500>	MTC2      vec18 <10> = K0
	state.v[18].U16[2] = (_u16)state.r[K0];


_04001660:	// 4001660: <4a17bdec>	VXOR      vec23 = vec23 xor vec23[<none>]
	rsp_cop2_vxor(&state.v[23], &state.v[23], &state.v[23]);


_04001664:	// 4001664: <001ad080>	SLL       K0 = K0 << 2
	state.r[K0] = ((_u32)state.r[K0] << 2);


_04001668:	// 4001668: <4a18c62c>	VXOR      vec24 = vec24 xor vec24[<none>]
	rsp_cop2_vxor(&state.v[24], &state.v[24], &state.v[24]);


_0400166C:	// 400166c: <489aa000>	MTC2      vec20 <00> = K0
	state.v[20].U16[7] = (_u16)state.r[K0];


_04001670:	// 4001670: <4a19ce6c>	VXOR      vec25 = vec25 xor vec25[<none>]
	rsp_cop2_vxor(&state.v[25], &state.v[25], &state.v[25]);


_04001674:	// 4001674: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001678:	// 4001678: <4a1ad6ac>	VXOR      vec26 = vec26 xor vec26[<none>]
	rsp_cop2_vxor(&state.v[26], &state.v[26], &state.v[26]);


_0400167C:	// 400167c: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001680:	// 4001680: <4a1bdeec>	VXOR      vec27 = vec27 xor vec27[<none>]
	rsp_cop2_vxor(&state.v[27], &state.v[27], &state.v[27]);


_04001684:	// 4001684: <20420000>	ADDI      V0 = V0 + 0000
	state.r[V0] = state.r[V0] + 0x0000;


_04001688:	// 4001688: <20030007>	ADDI      V1 = R0 + 0007
	state.r[V1] = 0 + 0x0007;


_0400168C:	// 400168c: <20130004>	ADDI      S3 = R0 + 0004
	state.r[S3] = 0 + 0x0004;


_04001690:	// 4001690: <48939000>	MTC2      vec18 <00> = S3
	state.v[18].U16[7] = (_u16)state.r[S3];


_04001694:	// 4001694: <20160170>	ADDI      S6 = R0 + 0170
	state.r[S6] = 0 + 0x0170;


_04001698:	// 4001698: <4b14e505>	VMUDM     vec20 = ( acc = vec28 * vec20[0] ) >> 16
	{
		_u16		s2value = state.v[20].U16[7];
		VMUDM_operation(0, state.v[20].U16[0], state.v[28].U16[0], s2value, 1, 1, 1, 1)
		VMUDM_operation(1, state.v[20].U16[1], state.v[28].U16[1], s2value, 1, 1, 1, 1)
		VMUDM_operation(2, state.v[20].U16[2], state.v[28].U16[2], s2value, 1, 1, 1, 1)
		VMUDM_operation(3, state.v[20].U16[3], state.v[28].U16[3], s2value, 1, 1, 1, 1)
		VMUDM_operation(4, state.v[20].U16[4], state.v[28].U16[4], s2value, 1, 1, 1, 1)
		VMUDM_operation(5, state.v[20].U16[5], state.v[28].U16[5], s2value, 1, 1, 1, 1)
		VMUDM_operation(6, state.v[20].U16[6], state.v[28].U16[6], s2value, 1, 1, 1, 1)
		VMUDM_operation(7, state.v[20].U16[7], state.v[28].U16[7], s2value, 1, 1, 1, 1)
	}


_0400169C:	// 400169c: <001a9c82>	SRL       S3 = K0 >> 18
	state.r[S3] = ((_u32)state.r[K0] >> 18);


_040016A0:	// 40016a0: <32730001>	ANDI      S3 = S3 & 0001
	state.r[S3] = (state.r[S3] & 0x00000001);


_040016A4:	// 40016a4: <1e60001b>	BGTZ      (R0>0) --> 1714
	sp_reg_pc = 0x040016A4;
	if((_s32)state.r[S3] > 0)
	{
		{
			_u32    addr = (0x00000010 + state.r[S4]);
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
		goto _04001714;
	}


_040016A8:	// 40016a8: <ea942001>	SQV       [S4 + 0x10] = vec20 <00>
	{
		_u32    addr = (0x00000010 + state.r[S4]);
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


_040016AC:	// 40016ac: <23010000>	ADDI      AT = T8 + 0000
	state.r[AT] = state.r[T8] + 0x0000;


_040016B0:	// 40016b0: <40132800>	MFC0      A1 = unknown
	state.r[S3] = 0;


_040016B4:	// 40016b4: <1660ffff>	BNE       (R0!=S3) --> 16b4
	sp_reg_pc = 0x040016B4;
	if (state.r[S3] != 0)
	{
		state.r[S3] = 0;
		goto _040016B4;
	}


_040016B8:	// 40016b8: <40132800>	MFC0      A1 = unknown
	state.r[S3] = 0;


_040016BC:	// 40016bc: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_040016C0:	// 40016c0: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_040016C4:	// 40016c4: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x040016C4;
	DMARead(state.r[V1]);


_040016C8:	// 40016c8: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040016CC:	// 40016cc: <ca9b2802>	LRV       vec27 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[27].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040016D0:	// 40016d0: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040016D4:	// 40016d4: <ca9a2802>	LRV       vec26 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[26].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040016D8:	// 40016d8: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040016DC:	// 40016dc: <ca992802>	LRV       vec25 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[25].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040016E0:	// 40016e0: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040016E4:	// 40016e4: <ca982802>	LRV       vec24 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[24].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040016E8:	// 40016e8: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040016EC:	// 40016ec: <ca972802>	LRV       vec23 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[23].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040016F0:	// 40016f0: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040016F4:	// 40016f4: <ca962802>	LRV       vec22 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[22].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040016F8:	// 40016f8: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040016FC:	// 40016fc: <ca952802>	LRV       vec21 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[21].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001700:	// 4001700: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001704:	// 4001704: <14a0ffff>	BNE       (R0!=A1) --> 1704
	sp_reg_pc = 0x04001704;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001704;
	}


_04001708:	// 4001708: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_0400170C:	// 400170c: <090005d4>	J         04001750
	rsp_ldv_compiler((state.r[AT] & 0xFFF), 30, 0);
	goto _04001750;


_04001710:	// 4001710: <c83e1c00>	LDV       vec30 <08> = [AT + 0x00]
	rsp_ldv_compiler((state.r[AT] & 0xFFF), 30, 0);


_04001714:	// 4001714: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001718:	// 4001718: <4a1ef7ac>	VXOR      vec30 = vec30 xor vec30[<none>]
	rsp_cop2_vxor(&state.v[30], &state.v[30], &state.v[30]);


_0400171C:	// 400171c: <ca9b2802>	LRV       vec27 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[27].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001720:	// 4001720: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001724:	// 4001724: <ca9a2802>	LRV       vec26 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[26].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001728:	// 4001728: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_0400172C:	// 400172c: <ca992802>	LRV       vec25 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[25].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001730:	// 4001730: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001734:	// 4001734: <ca982802>	LRV       vec24 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[24].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001738:	// 4001738: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_0400173C:	// 400173c: <ca972802>	LRV       vec23 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[23].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001740:	// 4001740: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001744:	// 4001744: <ca962802>	LRV       vec22 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[22].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001748:	// 4001748: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_0400174C:	// 400174c: <ca952802>	LRV       vec21 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[21].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001750:	// 4001750: <caff2000>	LQV       vec31 <00> = [S7 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S7]);
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


_04001754:	// 4001754: <4bfee507>	VMUDH     vec20 = ( acc = (vec28 * vec30[7]) << 16) >> 16
	{
		_u16		s2value = state.v[30].U16[0];
		VMUDH_operation(0, state.v[20].U16[0], state.v[28].U16[0], s2value, 0, 1, 0, 1)
		VMUDH_operation(1, state.v[20].U16[1], state.v[28].U16[1], s2value, 0, 1, 0, 1)
		VMUDH_operation(2, state.v[20].U16[2], state.v[28].U16[2], s2value, 0, 1, 0, 1)
		VMUDH_operation(3, state.v[20].U16[3], state.v[28].U16[3], s2value, 0, 1, 0, 1)
		VMUDH_operation(4, state.v[20].U16[4], state.v[28].U16[4], s2value, 0, 1, 0, 1)
		VMUDH_operation(5, state.v[20].U16[5], state.v[28].U16[5], s2value, 0, 1, 0, 1)
		VMUDH_operation(6, state.v[20].U16[6], state.v[28].U16[6], s2value, 0, 1, 0, 1)
		VMUDH_operation(7, state.v[20].U16[7], state.v[28].U16[7], s2value, 0, 1, 0, 1)
	}


_04001758:	// 4001758: <4b1fdd0f>	VMADH     vec20 = ( acc+= (vec27 * vec31[0]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[7];
		VMADH_operation(0, state.v[20].U16[0], state.v[27].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[27].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[27].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[27].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[27].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[27].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[27].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[27].U16[7], s2value, 0, 1, 0, 1)
	}


_0400175C:	// 400175c: <22d6fff0>	ADDI      S6 = S6 + fff0
	state.r[S6] = state.r[S6] + 0xFFFFFFF0;


_04001760:	// 4001760: <4b3fd50f>	VMADH     vec20 = ( acc+= (vec26 * vec31[1]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[6];
		VMADH_operation(0, state.v[20].U16[0], state.v[26].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[26].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[26].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[26].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[26].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[26].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[26].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[26].U16[7], s2value, 0, 1, 0, 1)
	}


_04001764:	// 4001764: <4b5fcd0f>	VMADH     vec20 = ( acc+= (vec25 * vec31[2]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[5];
		VMADH_operation(0, state.v[20].U16[0], state.v[25].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[25].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[25].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[25].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[25].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[25].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[25].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[25].U16[7], s2value, 0, 1, 0, 1)
	}


_04001768:	// 4001768: <eafe207f>	SQV       [S7 + 0xf0] = vec30 <00>
	{
		_u32    addr = (0xFFFFFFF0 + state.r[S7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[30].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[30].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[30].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[30].U32[3];
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

			value1 |= (state.v[30].U64[0] & temp1);
			value2 |= (state.v[30].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_0400176C:	// 400176c: <4b7fc50f>	VMADH     vec20 = ( acc+= (vec24 * vec31[3]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[4];
		VMADH_operation(0, state.v[20].U16[0], state.v[24].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[24].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[24].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[24].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[24].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[24].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[24].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[24].U16[7], s2value, 0, 1, 0, 1)
	}


_04001770:	// 4001770: <4b9fbf8f>	VMADH     vec30 = ( acc+= (vec23 * vec31[4]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[3];
		VMADH_operation(0, state.v[30].U16[0], state.v[23].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[30].U16[1], state.v[23].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[30].U16[2], state.v[23].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[30].U16[3], state.v[23].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[30].U16[4], state.v[23].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[30].U16[5], state.v[23].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[30].U16[6], state.v[23].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[30].U16[7], state.v[23].U16[7], s2value, 0, 1, 0, 1)
	}


_04001774:	// 4001774: <4bbfb50f>	VMADH     vec20 = ( acc+= (vec22 * vec31[5]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[2];
		VMADH_operation(0, state.v[20].U16[0], state.v[22].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[22].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[22].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[22].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[22].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[22].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[22].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[22].U16[7], s2value, 0, 1, 0, 1)
	}


_04001778:	// 4001778: <4bdfad0f>	VMADH     vec20 = ( acc+= (vec21 * vec31[6]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[1];
		VMADH_operation(0, state.v[20].U16[0], state.v[21].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[21].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[21].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[21].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[21].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[21].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[21].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[21].U16[7], s2value, 0, 1, 0, 1)
	}


_0400177C:	// 400177c: <4bb2fd0f>	VMADH     vec20 = ( acc+= (vec31 * vec18[5]) << 16) >> 16
	{
		_u16		s2value = state.v[18].U16[2];
		VMADH_operation(0, state.v[20].U16[0], state.v[31].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[31].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[31].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[31].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[31].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[31].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[31].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[31].U16[7], s2value, 0, 1, 0, 1)
	}


_04001780:	// 4001780: <caff2001>	LQV       vec31 <00> = [S7 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S7]);
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


_04001784:	// 4001784: <4b3e9f5d>	VSAW $v29, $v19, $v30[[1]]
	state.v[29].U16[0] = state.accum[0].U16[2];
	state.v[29].U16[1] = state.accum[1].U16[2];
	state.v[29].U16[2] = state.accum[2].U16[2];
	state.v[29].U16[3] = state.accum[3].U16[2];
	state.v[29].U16[4] = state.accum[4].U16[2];
	state.v[29].U16[5] = state.accum[5].U16[2];
	state.v[29].U16[6] = state.accum[6].U16[2];
	state.v[29].U16[7] = state.accum[7].U16[2];


_04001788:	// 4001788: <4b1e9f9d>	VSAW $v30, $v19, $v30[[0]]
	state.v[30].U16[0] = state.accum[0].U16[3];
	state.v[30].U16[1] = state.accum[1].U16[3];
	state.v[30].U16[2] = state.accum[2].U16[3];
	state.v[30].U16[3] = state.accum[3].U16[3];
	state.v[30].U16[4] = state.accum[4].U16[3];
	state.v[30].U16[5] = state.accum[5].U16[3];
	state.v[30].U16[6] = state.accum[6].U16[3];
	state.v[30].U16[7] = state.accum[7].U16[3];


_0400178C:	// 400178c: <4b12ed06>	VMUDN     vec20 = ( acc = vec29 * vec18[0]      ) >> 16
	{
		_u16		s2value = state.v[18].U16[7];
		VMUDN_operation(0, state.v[20].U16[0], state.v[29].U16[0], s2value, 1, 1, 1, 1)
		VMUDN_operation(1, state.v[20].U16[1], state.v[29].U16[1], s2value, 1, 1, 1, 1)
		VMUDN_operation(2, state.v[20].U16[2], state.v[29].U16[2], s2value, 1, 1, 1, 1)
		VMUDN_operation(3, state.v[20].U16[3], state.v[29].U16[3], s2value, 1, 1, 1, 1)
		VMUDN_operation(4, state.v[20].U16[4], state.v[29].U16[4], s2value, 1, 1, 1, 1)
		VMUDN_operation(5, state.v[20].U16[5], state.v[29].U16[5], s2value, 1, 1, 1, 1)
		VMUDN_operation(6, state.v[20].U16[6], state.v[29].U16[6], s2value, 1, 1, 1, 1)
		VMUDN_operation(7, state.v[20].U16[7], state.v[29].U16[7], s2value, 1, 1, 1, 1)
	}


_04001790:	// 4001790: <4b12f78f>	VMADH     vec30 = ( acc+= (vec30 * vec18[0]) << 16) >> 16
	{
		_u16		s2value = state.v[18].U16[7];
		VMADH_operation(0, state.v[30].U16[0], state.v[30].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[30].U16[1], state.v[30].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[30].U16[2], state.v[30].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[30].U16[3], state.v[30].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[30].U16[4], state.v[30].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[30].U16[5], state.v[30].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[30].U16[6], state.v[30].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[30].U16[7], state.v[30].U16[7], s2value, 1, 1, 0, 1)
	}


_04001794:	// 4001794: <1ec0ffef>	BGTZ      (R0>0) --> 1754
	sp_reg_pc = 0x04001794;
	if((_s32)state.r[S6] > 0)
	{
		state.r[S7] = state.r[S7] + 0x0010;
		goto _04001754;
	}


_04001798:	// 4001798: <22f70010>	ADDI      S7 = S7 + 0010
	state.r[S7] = state.r[S7] + 0x0010;


_0400179C:	// 400179c: <22e1fff8>	ADDI      AT = S7 + fff8
	state.r[AT] = state.r[S7] + 0xFFFFFFF8;


_040017A0:	// 40017a0: <0d00045d>	JAL	    04001174
	{
		_u32    addr = (0xFFFFFFF0 + state.r[S7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[30].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[30].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[30].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[30].U32[3];
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

			value1 |= (state.v[30].U64[0] & temp1);
			value2 |= (state.v[30].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}
	sp_reg_pc = 0x040017A0;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001174;


_040017A4:	// 40017a4: <eafe207f>	SQV       [S7 + 0xf0] = vec30 <00>
	{
		_u32    addr = (0xFFFFFFF0 + state.r[S7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[30].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[30].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[30].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[30].U32[3];
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

			value1 |= (state.v[30].U64[0] & temp1);
			value2 |= (state.v[30].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040017A8:	// 40017a8: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_040017AC:	// 40017ac: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_040017B0:	// 40017b0: <14a0ffff>	BNE       (R0!=A1) --> 17b0
	sp_reg_pc = 0x040017B0;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _040017B0;
	}


_040017B4:	// 40017b4: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_040017B8:	// 40017b8: <0900043a>	J         040010e8
	goto _040010E8;


_040017BC:	// 40017bc: <00000000>	NOP       


_040017C0:	// 40017c0: <001a1200>	SLL       V0 = K0 << 8
	state.r[V0] = ((_u32)state.r[K0] << 8);


_040017C4:	// 40017c4: <4a17bdec>	VXOR      vec23 = vec23 xor vec23[<none>]
	rsp_cop2_vxor(&state.v[23], &state.v[23], &state.v[23]);


_040017C8:	// 40017c8: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_040017CC:	// 40017cc: <20420000>	ADDI      V0 = V0 + 0000
	state.r[V0] = state.r[V0] + 0x0000;


_040017D0:	// 40017d0: <2003000f>	ADDI      V1 = R0 + 000f
	state.r[V1] = 0 + 0x000F;


_040017D4:	// 40017d4: <0019af82>	SRL       S5 = T9 >> 30
	state.r[S5] = ((_u32)state.r[T9] >> 30);


_040017D8:	// 40017d8: <1ea00012>	BGTZ      (R0>0) --> 1824
	sp_reg_pc = 0x040017D8;
	if((_s32)state.r[S5] > 0)
	{
		state.r[AT] = state.r[T8] + 0x0000;
		goto _04001824;
	}


_040017DC:	// 40017dc: <23010000>	ADDI      AT = T8 + 0000
	state.r[AT] = state.r[T8] + 0x0000;


_040017E0:	// 40017e0: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_040017E4:	// 40017e4: <1480ffff>	BNE       (R0!=A0) --> 17e4
	sp_reg_pc = 0x040017E4;
	if (state.r[A0] != 0)
	{
		state.r[A0] = 0;
		goto _040017E4;
	}


_040017E8:	// 40017e8: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_040017EC:	// 40017ec: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_040017F0:	// 40017f0: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_040017F4:	// 40017f4: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x040017F4;
	DMARead(state.r[V1]);


_040017F8:	// 40017f8: <0019a082>	SRL       S4 = T9 >> 2
	state.r[S4] = ((_u32)state.r[T9] >> 2);


_040017FC:	// 40017fc: <32940fff>	ANDI      S4 = S4 & 0fff
	state.r[S4] = (state.r[S4] & 0x00000FFF);


_04001800:	// 4001800: <229404e8>	ADDI      S4 = S4 + 04e8
	state.r[S4] = state.r[S4] + 0x04E8;


_04001804:	// 4001804: <c81f2004>	LQV       vec31 <00> = [R0 + 0x40]
	{
		_u32 addr = (0x00000040 + 0);
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


_04001808:	// 4001808: <c8192003>	LQV       vec25 <00> = [R0 + 0x30]
	{
		_u32 addr = (0x00000030 + 0);
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


_0400180C:	// 400180c: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001810:	// 4001810: <14a0ffff>	BNE       (R0!=A1) --> 1810
	sp_reg_pc = 0x04001810;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001810;
	}


_04001814:	// 4001814: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001818:	// 4001818: <cb131800>	LDV       vec19 <00> = [T8 + 0x00]
	rsp_ldv_compiler((state.r[T8] & 0xFFF), 19, 8);


_0400181C:	// 400181c: <09000610>	J         04001840
	state.v[24].U16[0] = Load16_DMEM(0x00000008 + state.r[T8]);
	goto _04001840;


_04001820:	// 4001820: <cb180f04>	LSV       vec24 <14> = [T8 + 0x08]
	state.v[24].U16[0] = Load16_DMEM(0x00000008 + state.r[T8]);


_04001824:	// 4001824: <0019a082>	SRL       S4 = T9 >> 2
	state.r[S4] = ((_u32)state.r[T9] >> 2);


_04001828:	// 4001828: <32940fff>	ANDI      S4 = S4 & 0fff
	state.r[S4] = (state.r[S4] & 0x00000FFF);


_0400182C:	// 400182c: <229404e8>	ADDI      S4 = S4 + 04e8
	state.r[S4] = state.r[S4] + 0x04E8;


_04001830:	// 4001830: <c81f2004>	LQV       vec31 <00> = [R0 + 0x40]
	{
		_u32 addr = (0x00000040 + 0);
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


_04001834:	// 4001834: <4a139cec>	VXOR      vec19 = vec19 xor vec19[<none>]
	rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);


_04001838:	// 4001838: <c8192003>	LQV       vec25 <00> = [R0 + 0x30]
	{
		_u32 addr = (0x00000030 + 0);
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


_0400183C:	// 400183c: <4a18c62c>	VXOR      vec24 = vec24 xor vec24[<none>]
	rsp_cop2_vxor(&state.v[24], &state.v[24], &state.v[24]);


_04001840:	// 4001840: <4894aa00>	MTC2      vec21 <04> = S4
	state.v[21].U16[5] = (_u16)state.r[S4];


_04001844:	// 4001844: <200400b0>	ADDI      A0 = R0 + 00b0
	state.r[A0] = 0 + 0x00B0;


_04001848:	// 4001848: <4884ab00>	MTC2      vec21 <06> = A0
	state.v[21].U16[4] = (_u16)state.r[A0];


_0400184C:	// 400184c: <4a1fce51>	VSUB      vec25 = vec25 - vec31[<none>]
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


_04001850:	// 4001850: <00192382>	SRL       A0 = T9 >> 14
	state.r[A0] = ((_u32)state.r[T9] >> 14);


_04001854:	// 4001854: <4884ac00>	MTC2      vec21 <08> = A0
	state.v[21].U16[3] = (_u16)state.r[A0];


_04001858:	// 4001858: <20040040>	ADDI      A0 = R0 + 0040
	state.r[A0] = 0 + 0x0040;


_0400185C:	// 400185c: <4884ad00>	MTC2      vec21 <10> = A0
	state.v[21].U16[2] = (_u16)state.r[A0];


_04001860:	// 4001860: <4a1fce51>	VSUB      vec25 = vec25 - vec31[<none>]
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


_04001864:	// 4001864: <c81e2005>	LQV       vec30 <00> = [R0 + 0x50]
	{
		_u32 addr = (0x00000050 + 0);
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


_04001868:	// 4001868: <c81d2006>	LQV       vec29 <00> = [R0 + 0x60]
	{
		_u32 addr = (0x00000060 + 0);
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


_0400186C:	// 400186c: <c81c2007>	LQV       vec28 <00> = [R0 + 0x70]
	{
		_u32 addr = (0x00000070 + 0);
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


_04001870:	// 4001870: <4bf8fe05>	VMUDM     vec24 = ( acc = vec31 * vec24[7] ) >> 16
	{
		_u16		s2value = state.v[24].U16[0];
		VMUDM_operation(0, state.v[24].U16[0], state.v[31].U16[0], s2value, 0, 1, 1, 1)
		VMUDM_operation(1, state.v[24].U16[1], state.v[31].U16[1], s2value, 0, 1, 1, 1)
		VMUDM_operation(2, state.v[24].U16[2], state.v[31].U16[2], s2value, 0, 1, 1, 1)
		VMUDM_operation(3, state.v[24].U16[3], state.v[31].U16[3], s2value, 0, 1, 1, 1)
		VMUDM_operation(4, state.v[24].U16[4], state.v[31].U16[4], s2value, 0, 1, 1, 1)
		VMUDM_operation(5, state.v[24].U16[5], state.v[31].U16[5], s2value, 0, 1, 1, 1)
		VMUDM_operation(6, state.v[24].U16[6], state.v[31].U16[6], s2value, 0, 1, 1, 1)
		VMUDM_operation(7, state.v[24].U16[7], state.v[31].U16[7], s2value, 0, 1, 1, 1)
	}


_04001874:	// 4001874: <c81b2008>	LQV       vec27 <00> = [R0 + 0x80]
	{
		_u32 addr = (0x00000080 + 0);
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


_04001878:	// 4001878: <4b95cdcd>	VMADM     vec23 = ( acc+= vec25 * vec21[4] ) >> 16
	{
		_u16		s2value = state.v[21].U16[3];
		VMADM_operation(0, state.v[23].U16[0], state.v[25].U16[0], s2value, 1, 1, 1, 1)
		VMADM_operation(1, state.v[23].U16[1], state.v[25].U16[1], s2value, 1, 1, 1, 1)
		VMADM_operation(2, state.v[23].U16[2], state.v[25].U16[2], s2value, 1, 1, 1, 1)
		VMADM_operation(3, state.v[23].U16[3], state.v[25].U16[3], s2value, 1, 1, 1, 1)
		VMADM_operation(4, state.v[23].U16[4], state.v[25].U16[4], s2value, 1, 1, 1, 1)
		VMADM_operation(5, state.v[23].U16[5], state.v[25].U16[5], s2value, 1, 1, 1, 1)
		VMADM_operation(6, state.v[23].U16[6], state.v[25].U16[6], s2value, 1, 1, 1, 1)
		VMADM_operation(7, state.v[23].U16[7], state.v[25].U16[7], s2value, 1, 1, 1, 1)
	}


_0400187C:	// 400187c: <c81a2009>	LQV       vec26 <00> = [R0 + 0x90]
	{
		_u32 addr = (0x00000090 + 0);
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


_04001880:	// 4001880: <4b1efe0e>	VMADN     vec24 = ( acc+= vec31 * vec30[0] )
	{
		_u16		s2value = state.v[30].U16[7];
		VMADN_operation(0, state.v[24].U16[0], state.v[31].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[24].U16[1], state.v[31].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[24].U16[2], state.v[31].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[24].U16[3], state.v[31].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[24].U16[4], state.v[31].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[24].U16[5], state.v[31].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[24].U16[6], state.v[31].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[24].U16[7], state.v[31].U16[7], s2value, 1, 0, 0, 1)
	}


_04001884:	// 4001884: <ea931800>	SDV       [S4 + 0x00] = vec19 <00>
	{
		_u32	addr = (0x00000000 + state.r[S4]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[19].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[19].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[19].U32[0x00000003];
		}
	}


_04001888:	// 4001888: <c8192003>	LQV       vec25 <00> = [R0 + 0x30]
	{
		_u32 addr = (0x00000030 + 0);
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


_0400188C:	// 400188c: <4b55fd86>	VMUDN     vec22 = ( acc = vec31 * vec21[2]      ) >> 16
	{
		_u16		s2value = state.v[21].U16[5];
		VMUDN_operation(0, state.v[22].U16[0], state.v[31].U16[0], s2value, 0, 1, 1, 1)
		VMUDN_operation(1, state.v[22].U16[1], state.v[31].U16[1], s2value, 0, 1, 1, 1)
		VMUDN_operation(2, state.v[22].U16[2], state.v[31].U16[2], s2value, 0, 1, 1, 1)
		VMUDN_operation(3, state.v[22].U16[3], state.v[31].U16[3], s2value, 0, 1, 1, 1)
		VMUDN_operation(4, state.v[22].U16[4], state.v[31].U16[4], s2value, 0, 1, 1, 1)
		VMUDN_operation(5, state.v[22].U16[5], state.v[31].U16[5], s2value, 0, 1, 1, 1)
		VMUDN_operation(6, state.v[22].U16[6], state.v[31].U16[6], s2value, 0, 1, 1, 1)
		VMUDN_operation(7, state.v[22].U16[7], state.v[31].U16[7], s2value, 0, 1, 1, 1)
	}


_04001890:	// 4001890: <20160170>	ADDI      S6 = R0 + 0170
	state.r[S6] = 0 + 0x0170;


_04001894:	// 4001894: <4b5ebd8e>	VMADN     vec22 = ( acc+= vec23 * vec30[2] )
	{
		_u16		s2value = state.v[30].U16[5];
		VMADN_operation(0, state.v[22].U16[0], state.v[23].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[22].U16[1], state.v[23].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[22].U16[2], state.v[23].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[22].U16[3], state.v[23].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[22].U16[4], state.v[23].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[22].U16[5], state.v[23].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[22].U16[6], state.v[23].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[22].U16[7], state.v[23].U16[7], s2value, 1, 0, 0, 1)
	}


_04001898:	// 4001898: <33240003>	ANDI      A0 = T9 & 0003
	state.r[A0] = (state.r[T9] & 0x00000003);


_0400189C:	// 400189c: <4bb5c504>	VMUDL     vec20 = ( acc = (vec24 * vec21[5] >> 16)      )
	{
		_u16		s2value = state.v[21].U16[2];
		VMUDL_operation(0, state.v[20].U16[0], state.v[24].U16[0], s2value, 1, 1, 1, 1)
		VMUDL_operation(1, state.v[20].U16[1], state.v[24].U16[1], s2value, 1, 1, 1, 1)
		VMUDL_operation(2, state.v[20].U16[2], state.v[24].U16[2], s2value, 1, 1, 1, 1)
		VMUDL_operation(3, state.v[20].U16[3], state.v[24].U16[3], s2value, 1, 1, 1, 1)
		VMUDL_operation(4, state.v[20].U16[4], state.v[24].U16[4], s2value, 1, 1, 1, 1)
		VMUDL_operation(5, state.v[20].U16[5], state.v[24].U16[5], s2value, 1, 1, 1, 1)
		VMUDL_operation(6, state.v[20].U16[6], state.v[24].U16[6], s2value, 1, 1, 1, 1)
		VMUDL_operation(7, state.v[20].U16[7], state.v[24].U16[7], s2value, 1, 1, 1, 1)
	}


_040018A0:	// 40018a0: <10800002>	BEQ       (R0==A0) --> 18ac
	sp_reg_pc = 0x040018A0;
	if (state.r[A0] == 0)
	{
		state.r[S7] = 0 + 0x04F0;
		goto _040018AC;
	}


_040018A4:	// 40018a4: <201704f0>	ADDI      S7 = R0 + 04f0
	state.r[S7] = 0 + 0x04F0;


_040018A8:	// 40018a8: <20170660>	ADDI      S7 = R0 + 0660
	state.r[S7] = 0 + 0x0660;


_040018AC:	// 40018ac: <eb180b84>	SSV       [T8 + 0x08] = vec24 <07>
	Save16_DMEM(state.v[24].U16[4], (0x00000008 + state.r[T8]) & 0xfff);


_040018B0:	// 40018b0: <4b9ea506>	VMUDN     vec20 = ( acc = vec20 * vec30[4]      ) >> 16
	{
		_u16		s2value = state.v[30].U16[3];
		VMUDN_operation(0, state.v[20].U16[0], state.v[20].U16[0], s2value, 0, 1, 1, 1)
		VMUDN_operation(1, state.v[20].U16[1], state.v[20].U16[1], s2value, 0, 1, 1, 1)
		VMUDN_operation(2, state.v[20].U16[2], state.v[20].U16[2], s2value, 0, 1, 1, 1)
		VMUDN_operation(3, state.v[20].U16[3], state.v[20].U16[3], s2value, 0, 1, 1, 1)
		VMUDN_operation(4, state.v[20].U16[4], state.v[20].U16[4], s2value, 0, 1, 1, 1)
		VMUDN_operation(5, state.v[20].U16[5], state.v[20].U16[5], s2value, 0, 1, 1, 1)
		VMUDN_operation(6, state.v[20].U16[6], state.v[20].U16[6], s2value, 0, 1, 1, 1)
		VMUDN_operation(7, state.v[20].U16[7], state.v[20].U16[7], s2value, 0, 1, 1, 1)
	}


_040018B4:	// 40018b4: <e816207b>	SQV       [R0 + 0xb0] = vec22 <00>
	{
		_u32    addr = (0xFFFFFFB0 + 0);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[22].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[22].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[22].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[22].U32[3];
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

			value1 |= (state.v[22].U64[0] & temp1);
			value2 |= (state.v[22].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040018B8:	// 40018b8: <4b75fd0e>	VMADN     vec20 = ( acc+= vec31 * vec21[3] )
	{
		_u16		s2value = state.v[21].U16[4];
		VMADN_operation(0, state.v[20].U16[0], state.v[31].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[20].U16[1], state.v[31].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[20].U16[2], state.v[31].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[20].U16[3], state.v[31].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[20].U16[4], state.v[31].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[20].U16[5], state.v[31].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[20].U16[6], state.v[31].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[20].U16[7], state.v[31].U16[7], s2value, 1, 0, 0, 1)
	}


_040018BC:	// 40018bc: <e814207c>	SQV       [R0 + 0xc0] = vec20 <00>
	{
		_u32    addr = (0xFFFFFFC0 + 0);
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


_040018C0:	// 40018c0: <84150fb0>	LH        S5 = [R0+0fb0]
	state.r[S5] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FB0) & 0xfff);


_040018C4:	// 40018c4: <840d0fc0>	LH        T5 = [R0+0fc0]
	state.r[T5] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FC0) & 0xfff);


_040018C8:	// 40018c8: <84110fb8>	LH        S1 = [R0+0fb8]
	state.r[S1] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FB8) & 0xfff);


_040018CC:	// 40018cc: <84090fc8>	LH        T1 = [R0+0fc8]
	state.r[T1] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FC8) & 0xfff);


_040018D0:	// 40018d0: <84140fb2>	LH        S4 = [R0+0fb2]
	state.r[S4] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FB2) & 0xfff);


_040018D4:	// 40018d4: <840c0fc2>	LH        T4 = [R0+0fc2]
	state.r[T4] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FC2) & 0xfff);


_040018D8:	// 40018d8: <84100fba>	LH        S0 = [R0+0fba]
	state.r[S0] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FBA) & 0xfff);


_040018DC:	// 40018dc: <84080fca>	LH        T0 = [R0+0fca]
	state.r[T0] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FCA) & 0xfff);


_040018E0:	// 40018e0: <84130fb4>	LH        S3 = [R0+0fb4]
	state.r[S3] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FB4) & 0xfff);


_040018E4:	// 40018e4: <840b0fc4>	LH        T3 = [R0+0fc4]
	state.r[T3] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FC4) & 0xfff);


_040018E8:	// 40018e8: <840f0fbc>	LH        T7 = [R0+0fbc]
	state.r[T7] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FBC) & 0xfff);


_040018EC:	// 40018ec: <84070fcc>	LH        A3 = [R0+0fcc]
	state.r[A3] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FCC) & 0xfff);


_040018F0:	// 40018f0: <84120fb6>	LH        S2 = [R0+0fb6]
	state.r[S2] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FB6) & 0xfff);


_040018F4:	// 40018f4: <840a0fc6>	LH        T2 = [R0+0fc6]
	state.r[T2] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FC6) & 0xfff);


_040018F8:	// 40018f8: <840e0fbe>	LH        T6 = [R0+0fbe]
	state.r[T6] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FBE) & 0xfff);


_040018FC:	// 40018fc: <84060fce>	LH        A2 = [R0+0fce]
	state.r[A2] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FCE) & 0xfff);


_04001900:	// 4001900: <cab31800>	LDV       vec19 <00> = [S5 + 0x00]
	// In function: SmashBros193C_1A28
	//Call the function: SmashBros193C_1A28
	SmashBros193C_1A28();
	goto _040019F0;

_04001904:	// 4001904: <4bf8fe05>	VMUDM     vec24 = ( acc = vec31 * vec24[7] ) >> 16
	ErrorToCallFunction(0x04001904);

_04001908:	// 4001908: <c9b21800>	LDV       vec18 <00> = [T5 + 0x00]
	ErrorToCallFunction(0x04001908);

_0400190C:	// 400190c: <4bf7fe0f>	VMADH     vec24 = ( acc+= (vec31 * vec23[7]) << 16) >> 16
	ErrorToCallFunction(0x0400190C);

_04001910:	// 4001910: <ca331c00>	LDV       vec19 <08> = [S1 + 0x00]
	ErrorToCallFunction(0x04001910);

_04001914:	// 4001914: <4b95cdcd>	VMADM     vec23 = ( acc+= vec25 * vec21[4] ) >> 16
	ErrorToCallFunction(0x04001914);

_04001918:	// 4001918: <c9321c00>	LDV       vec18 <08> = [T1 + 0x00]
	ErrorToCallFunction(0x04001918);

_0400191C:	// 400191c: <4b1efe0e>	VMADN     vec24 = ( acc+= vec31 * vec30[0] )
	ErrorToCallFunction(0x0400191C);

_04001920:	// 4001920: <ca911800>	LDV       vec17 <00> = [S4 + 0x00]
	ErrorToCallFunction(0x04001920);

_04001924:	// 4001924: <4b55fd86>	VMUDN     vec22 = ( acc = vec31 * vec21[2]      ) >> 16
	ErrorToCallFunction(0x04001924);

_04001928:	// 4001928: <c9901800>	LDV       vec16 <00> = [T4 + 0x00]
	ErrorToCallFunction(0x04001928);

_0400192C:	// 400192c: <ca111c00>	LDV       vec17 <08> = [S0 + 0x00]
	ErrorToCallFunction(0x0400192C);

_04001930:	// 4001930: <4b5ebd8e>	VMADN     vec22 = ( acc+= vec23 * vec30[2] )
	ErrorToCallFunction(0x04001930);

_04001934:	// 4001934: <c9101c00>	LDV       vec16 <08> = [T0 + 0x00]
	ErrorToCallFunction(0x04001934);

_04001938:	// 4001938: <4bb5c504>	VMUDL     vec20 = ( acc = (vec24 * vec21[5] >> 16)      )
	ErrorToCallFunction(0x04001938);

_0400193C:	// 400193c: <ca6f1800>	LDV       vec15 <00> = [S3 + 0x00]
	ErrorToCallFunction(0x0400193C);

_04001940:	// 4001940: <c96e1800>	LDV       vec14 <00> = [T3 + 0x00]
	ErrorToCallFunction(0x04001940);

_04001944:	// 4001944: <c9ef1c00>	LDV       vec15 <08> = [T7 + 0x00]
	ErrorToCallFunction(0x04001944);

_04001948:	// 4001948: <c8ee1c00>	LDV       vec14 <08> = [A3 + 0x00]
	ErrorToCallFunction(0x04001948);

_0400194C:	// 400194c: <4b9ea506>	VMUDN     vec20 = ( acc = vec20 * vec30[4]      ) >> 16
	ErrorToCallFunction(0x0400194C);

_04001950:	// 4001950: <ca4d1800>	LDV       vec13 <00> = [S2 + 0x00]
	ErrorToCallFunction(0x04001950);

_04001954:	// 4001954: <4b75fd0e>	VMADN     vec20 = ( acc+= vec31 * vec21[3] )
	ErrorToCallFunction(0x04001954);

_04001958:	// 4001958: <c94c1800>	LDV       vec12 <00> = [T2 + 0x00]
	ErrorToCallFunction(0x04001958);

_0400195C:	// 400195c: <c9cd1c00>	LDV       vec13 <08> = [T6 + 0x00]
	ErrorToCallFunction(0x0400195C);

_04001960:	// 4001960: <4a129ac0>	VMULF     vec11 = ( acc = (vec19 * vec18[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001960);

_04001964:	// 4001964: <c8cc1c00>	LDV       vec12 <08> = [A2 + 0x00]
	ErrorToCallFunction(0x04001964);

_04001968:	// 4001968: <4a108a80>	VMULF     vec10 = ( acc = (vec17 * vec16[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001968);

_0400196C:	// 400196c: <e816207b>	SQV       [R0 + 0xb0] = vec22 <00>
	ErrorToCallFunction(0x0400196C);

_04001970:	// 4001970: <4a0e7a40>	VMULF     vec09 = ( acc = (vec15 * vec14[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001970);

_04001974:	// 4001974: <e814207c>	SQV       [R0 + 0xc0] = vec20 <00>
	ErrorToCallFunction(0x04001974);

_04001978:	// 4001978: <84150fb0>	LH        S5 = [R0+0fb0]
	ErrorToCallFunction(0x04001978);

_0400197C:	// 400197c: <840d0fc0>	LH        T5 = [R0+0fc0]
	ErrorToCallFunction(0x0400197C);

_04001980:	// 4001980: <4a0c6a00>	VMULF     vec08 = ( acc = (vec13 * vec12[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001980);

_04001984:	// 4001984: <84110fb8>	LH        S1 = [R0+0fb8]
	ErrorToCallFunction(0x04001984);

_04001988:	// 4001988: <4a6b5ad0>	VADD      vec11 = vec11 + vec11[1q]
	ErrorToCallFunction(0x04001988);

_0400198C:	// 400198c: <84090fc8>	LH        T1 = [R0+0fc8]
	ErrorToCallFunction(0x0400198C);

_04001990:	// 4001990: <4a6a5290>	VADD      vec10 = vec10 + vec10[1q]
	ErrorToCallFunction(0x04001990);

_04001994:	// 4001994: <84140fb2>	LH        S4 = [R0+0fb2]
	ErrorToCallFunction(0x04001994);

_04001998:	// 4001998: <4a694a50>	VADD      vec09 = vec09 + vec09[1q]
	ErrorToCallFunction(0x04001998);

_0400199C:	// 400199c: <840c0fc2>	LH        T4 = [R0+0fc2]
	ErrorToCallFunction(0x0400199C);

_040019A0:	// 40019a0: <4a684210>	VADD      vec08 = vec08 + vec08[1q]
	ErrorToCallFunction(0x040019A0);

_040019A4:	// 40019a4: <84100fba>	LH        S0 = [R0+0fba]
	ErrorToCallFunction(0x040019A4);

_040019A8:	// 40019a8: <4acb5ad0>	VADD      vec11 = vec11 + vec11[2h]
	ErrorToCallFunction(0x040019A8);

_040019AC:	// 40019ac: <84080fca>	LH        T0 = [R0+0fca]
	ErrorToCallFunction(0x040019AC);

_040019B0:	// 40019b0: <4aca5290>	VADD      vec10 = vec10 + vec10[2h]
	ErrorToCallFunction(0x040019B0);

_040019B4:	// 40019b4: <84130fb4>	LH        S3 = [R0+0fb4]
	ErrorToCallFunction(0x040019B4);

_040019B8:	// 40019b8: <4ac94a50>	VADD      vec09 = vec09 + vec09[2h]
	ErrorToCallFunction(0x040019B8);

_040019BC:	// 40019bc: <840b0fc4>	LH        T3 = [R0+0fc4]
	ErrorToCallFunction(0x040019BC);

_040019C0:	// 40019c0: <4ac84210>	VADD      vec08 = vec08 + vec08[2h]
	ErrorToCallFunction(0x040019C0);

_040019C4:	// 40019c4: <840f0fbc>	LH        T7 = [R0+0fbc]
	ErrorToCallFunction(0x040019C4);

_040019C8:	// 40019c8: <4a8be9c6>	VMUDN     vec07 = ( acc = vec29 * vec11[0h]      ) >> 16
	ErrorToCallFunction(0x040019C8);

_040019CC:	// 40019cc: <84070fcc>	LH        A3 = [R0+0fcc]
	ErrorToCallFunction(0x040019CC);

_040019D0:	// 40019d0: <4a8ae1ce>	VMADN     vec07 = ( acc+= vec28 * vec10[0h] )
	ErrorToCallFunction(0x040019D0);

_040019D4:	// 40019d4: <84120fb6>	LH        S2 = [R0+0fb6]
	ErrorToCallFunction(0x040019D4);

_040019D8:	// 40019d8: <4a89d9ce>	VMADN     vec07 = ( acc+= vec27 * vec09[0h] )
	ErrorToCallFunction(0x040019D8);

_040019DC:	// 40019dc: <840a0fc6>	LH        T2 = [R0+0fc6]
	ErrorToCallFunction(0x040019DC);

_040019E0:	// 40019e0: <4a88d1ce>	VMADN     vec07 = ( acc+= vec26 * vec08[0h] )
	ErrorToCallFunction(0x040019E0);

_040019E4:	// 40019e4: <840e0fbe>	LH        T6 = [R0+0fbe]
	ErrorToCallFunction(0x040019E4);

_040019E8:	// 40019e8: <84060fce>	LH        A2 = [R0+0fce]
	ErrorToCallFunction(0x040019E8);

_040019EC:	// 40019ec: <22d6fff0>	ADDI      S6 = S6 + fff0
	ErrorToCallFunction(0x040019EC);

_040019F0:	// 40019f0: <1ac00003>	BLEZ      (R0<=0) --> 1a00
	sp_reg_pc = 0x040019F0;
	if ((_s32)state.r[S6] <= 0)
	{
		{
			_u32    addr = (0x00000000 + state.r[S7]);
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
		goto _04001A00;
	}


_040019F4:	// 40019f4: <eae72000>	SQV       [S7 + 0x00] = vec07 <00>
	{
		_u32    addr = (0x00000000 + state.r[S7]);
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


_040019F8:	// 40019f8: <09000640>	J         04001900
	state.r[S7] = state.r[S7] + 0x0010;
	goto _04001900;


_040019FC:	// 40019fc: <22f70010>	ADDI      S7 = S7 + 0010
	state.r[S7] = state.r[S7] + 0x0010;


_04001A00:	// 4001a00: <cab31800>	LDV       vec19 <00> = [S5 + 0x00]
	rsp_ldv_compiler((state.r[S5] & 0xFFF), 19, 8);


_04001A04:	// 4001a04: <eb180804>	SSV       [T8 + 0x08] = vec24 <00>
	Save16_DMEM(state.v[24].U16[7], (0x00000008 + state.r[T8]) & 0xfff);


_04001A08:	// 4001a08: <0d00045d>	JAL	    04001174
	{
		_u32	addr = (0x00000000 + state.r[T8]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[19].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[19].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[19].U32[0x00000003];
		}
	}
	sp_reg_pc = 0x04001A08;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001174;


_04001A0C:	// 4001a0c: <eb131800>	SDV       [T8 + 0x00] = vec19 <00>
	{
		_u32	addr = (0x00000000 + state.r[T8]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[19].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[19].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[19].U32[0x00000003];
		}
	}


_04001A10:	// 4001a10: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001A14:	// 4001a14: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001A18:	// 4001a18: <14a0ffff>	BNE       (R0!=A1) --> 1a18
	sp_reg_pc = 0x04001A18;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001A18;
	}


_04001A1C:	// 4001a1c: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001A20:	// 4001a20: <0900043a>	J         040010e8
	goto _040010E8;


_04001A24:	// 4001a24: <00000000>	NOP       


_04001A28:	// 4001a28: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001A2C:	// 4001a2c: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001A30:	// 4001a30: <20420000>	ADDI      V0 = V0 + 0000
	state.r[V0] = state.r[V0] + 0x0000;


_04001A34:	// 4001a34: <c81f2004>	LQV       vec31 <00> = [R0 + 0x40]
	{
		_u32 addr = (0x00000040 + 0);
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


_04001A38:	// 4001a38: <c80a2005>	LQV       vec10 <00> = [R0 + 0x50]
	{
		_u32 addr = (0x00000050 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[10].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[10].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[10].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[10].U32[3] = *(_u32 *)&pDMEM[addr];
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

			state.v[10].U64[0] = state.v[10].U64[0] & ~temp1;
			state.v[10].U64[1] = state.v[10].U64[1] & ~temp2;
			state.v[10].U64[0] = state.v[10].U64[0] | value1;
			state.v[10].U64[1] = state.v[10].U64[1] | value2;
		}
	}


_04001A3C:	// 4001a3c: <c81e200a>	LQV       vec30 <00> = [R0 + 0xa0]
	{
		_u32 addr = (0x000000A0 + 0);
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


_04001A40:	// 4001a40: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_04001A44:	// 4001a44: <001a7402>	SRL       T6 = K0 >> 16
	state.r[T6] = ((_u32)state.r[K0] >> 16);


_04001A48:	// 4001a48: <31cf0001>	ANDI      T7 = T6 & 0001
	state.r[T7] = (state.r[T6] & 0x00000001);


_04001A4C:	// 4001a4c: <1de0000a>	BGTZ      (R0>0) --> 1a78
	sp_reg_pc = 0x04001A4C;
	if((_s32)state.r[T7] > 0)
	{
		state.r[AT] = state.r[T8] + 0x0000;
		goto _04001A78;
	}


_04001A50:	// 4001a50: <23010000>	ADDI      AT = T8 + 0000
	state.r[AT] = state.r[T8] + 0x0000;


_04001A54:	// 4001a54: <0d000456>	JAL	    04001158
	state.r[V1] = 0 + 0x004F;
	sp_reg_pc = 0x04001A54;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001158;


_04001A58:	// 4001a58: <2003004f>	ADDI      V1 = R0 + 004f
	state.r[V1] = 0 + 0x004F;


_04001A5C:	// 4001a5c: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001A60:	// 4001a60: <14a0ffff>	BNE       (R0!=A1) --> 1a60
	sp_reg_pc = 0x04001A60;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001A60;
	}


_04001A64:	// 4001a64: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001A68:	// 4001a68: <cb142000>	LQV       vec20 <00> = [T8 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T8]);
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


_04001A6C:	// 4001a6c: <cb152001>	LQV       vec21 <00> = [T8 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T8]);
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


_04001A70:	// 4001a70: <cb122002>	LQV       vec18 <00> = [T8 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[T8]);
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


_04001A74:	// 4001a74: <cb132003>	LQV       vec19 <00> = [T8 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[T8]);
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


_04001A78:	// 4001a78: <cb182004>	LQV       vec24 <00> = [T8 + 0x40]
	{
		_u32 addr = (0x00000040 + state.r[T8]);
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


_04001A7C:	// 4001a7c: <201004f0>	ADDI      S0 = R0 + 04f0
	state.r[S0] = 0 + 0x04F0;


_04001A80:	// 4001a80: <201509d0>	ADDI      S5 = R0 + 09d0
	state.r[S5] = 0 + 0x09D0;


_04001A84:	// 4001a84: <20140b40>	ADDI      S4 = R0 + 0b40
	state.r[S4] = 0 + 0x0B40;


_04001A88:	// 4001a88: <20130cb0>	ADDI      S3 = R0 + 0cb0
	state.r[S3] = 0 + 0x0CB0;


_04001A8C:	// 4001a8c: <20120e20>	ADDI      S2 = R0 + 0e20
	state.r[S2] = 0 + 0x0E20;


_04001A90:	// 4001a90: <20110170>	ADDI      S1 = R0 + 0170
	state.r[S1] = 0 + 0x0170;


_04001A94:	// 4001a94: <4816c400>	MFC2      S6 = vec24 <08>
	state.r[S6] = (_s32)state.v[24].S16[3];


_04001A98:	// 4001a98: <4bd8fa68>	VAND      vec09 = vec31 and vec24[6]
	rsp_cop2_vand_element(&state.v[9], &state.v[31], &state.v[24].U16[1]);


_04001A9C:	// 4001a9c: <4a090251>	VSUB      vec09 = vec00 - vec09[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[9].U16[0], state.v[0].U16[0], state.v[9].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[9].U16[1], state.v[0].U16[1], state.v[9].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[9].U16[2], state.v[0].U16[2], state.v[9].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[9].U16[3], state.v[0].U16[3], state.v[9].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[9].U16[4], state.v[0].U16[4], state.v[9].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[9].U16[5], state.v[0].U16[5], state.v[9].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[9].U16[6], state.v[0].U16[6], state.v[9].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[9].U16[7], state.v[0].U16[7], state.v[9].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001AA0:	// 4001aa0: <4bf8fa28>	VAND      vec08 = vec31 and vec24[7]
	rsp_cop2_vand_element(&state.v[8], &state.v[31], &state.v[24].U16[0]);


_04001AA4:	// 4001aa4: <4a080211>	VSUB      vec08 = vec00 - vec08[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[8].U16[0], state.v[0].U16[0], state.v[8].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[8].U16[1], state.v[0].U16[1], state.v[8].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[8].U16[2], state.v[0].U16[2], state.v[8].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[8].U16[3], state.v[0].U16[3], state.v[8].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[8].U16[4], state.v[0].U16[4], state.v[8].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[8].U16[5], state.v[0].U16[5], state.v[8].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[8].U16[6], state.v[0].U16[6], state.v[8].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[8].U16[7], state.v[0].U16[7], state.v[8].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001AA8:	// 4001aa8: <4a09422c>	VXOR      vec08 = vec08 xor vec09[<none>]
	rsp_cop2_vxor(&state.v[8], &state.v[8], &state.v[9]);


_04001AAC:	// 4001aac: <11e00035>	BEQ       (R0==T7) --> 1b84
	sp_reg_pc = 0x04001AAC;
	if (state.r[T7] == 0)
	{
		state.r[S7] = (_s32)state.v[24].S16[6];
		goto _04001B84;
	}


_04001AB0:	// 4001ab0: <4817c100>	MFC2      S7 = vec24 <02>
	state.r[S7] = (_s32)state.v[24].S16[6];


_04001AB4:	// 4001ab4: <2003004f>	ADDI      V1 = R0 + 004f
	state.r[V1] = 0 + 0x004F;


_04001AB8:	// 4001ab8: <4a14a52c>	VXOR      vec20 = vec20 xor vec20[<none>]
	rsp_cop2_vxor(&state.v[20], &state.v[20], &state.v[20]);


_04001ABC:	// 4001abc: <cb140f28>	LSV       vec20 <14> = [T8 + 0x10]
	state.v[20].U16[0] = Load16_DMEM(0x00000050 + state.r[T8]);


_04001AC0:	// 4001ac0: <4a15ad6c>	VXOR      vec21 = vec21 xor vec21[<none>]
	rsp_cop2_vxor(&state.v[21], &state.v[21], &state.v[21]);


_04001AC4:	// 4001ac4: <ca112000>	LQV       vec17 <00> = [S0 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S0]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[17].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[17].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[17].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[17].U32[3] = *(_u32 *)&pDMEM[addr];
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

			state.v[17].U64[0] = state.v[17].U64[0] & ~temp1;
			state.v[17].U64[1] = state.v[17].U64[1] & ~temp2;
			state.v[17].U64[0] = state.v[17].U64[0] | value1;
			state.v[17].U64[1] = state.v[17].U64[1] | value2;
		}
	}


_04001AC8:	// 4001ac8: <4a1294ac>	VXOR      vec18 = vec18 xor vec18[<none>]
	rsp_cop2_vxor(&state.v[18], &state.v[18], &state.v[18]);


_04001ACC:	// 4001acc: <489a9700>	MTC2      vec18 <14> = K0
	state.v[18].U16[0] = (_u16)state.r[K0];


_04001AD0:	// 4001ad0: <4b58f5c4>	VMUDL     vec23 = ( acc = (vec30 * vec24[2] >> 16)      )
	{
		_u16		s2value = state.v[24].U16[5];
		VMUDL_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 0, 1, 1, 1)
		VMUDL_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 0, 1, 1, 1)
		VMUDL_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 0, 1, 1, 1)
		VMUDL_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 0, 1, 1, 1)
		VMUDL_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 0, 1, 1, 1)
		VMUDL_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 0, 1, 1, 1)
		VMUDL_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 0, 1, 1, 1)
		VMUDL_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 0, 1, 1, 1)
	}


_04001AD4:	// 4001ad4: <cabd2000>	LQV       vec29 <00> = [S5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S5]);
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


_04001AD8:	// 4001ad8: <4b38f5ce>	VMADN     vec23 = ( acc+= vec30 * vec24[1] )
	{
		_u16		s2value = state.v[24].U16[6];
		VMADN_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 1, 1, 1, 1)
		VMADN_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 1, 1, 1, 1)
		VMADN_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 1, 1, 1, 1)
		VMADN_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 1, 1, 1, 1)
		VMADN_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 1, 1, 1, 1)
		VMADN_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 1, 1, 1, 1)
		VMADN_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 1, 1, 1, 1)
		VMADN_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 1, 1, 1, 1)
	}


_04001ADC:	// 4001adc: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
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


_04001AE0:	// 4001ae0: <4bf4fd0f>	VMADH     vec20 = ( acc+= (vec31 * vec20[7]) << 16) >> 16
	{
		_u16		s2value = state.v[20].U16[0];
		VMADH_operation(0, state.v[20].U16[0], state.v[31].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[31].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[31].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[31].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[31].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[31].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[31].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[31].U16[7], s2value, 1, 1, 0, 1)
	}


_04001AE4:	// 4001ae4: <ca9c2000>	LQV       vec28 <00> = [S4 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S4]);
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


_04001AE8:	// 4001ae8: <4b00fd4e>	VMADN     vec21 = ( acc+= vec31 * vec00[0] )
	{
		_u16		s2value = state.v[0].U16[7];
		VMADN_operation(0, state.v[21].U16[0], state.v[31].U16[0], s2value, 1, 1, 1, 1)
		VMADN_operation(1, state.v[21].U16[1], state.v[31].U16[1], s2value, 1, 1, 1, 1)
		VMADN_operation(2, state.v[21].U16[2], state.v[31].U16[2], s2value, 1, 1, 1, 1)
		VMADN_operation(3, state.v[21].U16[3], state.v[31].U16[3], s2value, 1, 1, 1, 1)
		VMADN_operation(4, state.v[21].U16[4], state.v[31].U16[4], s2value, 1, 1, 1, 1)
		VMADN_operation(5, state.v[21].U16[5], state.v[31].U16[5], s2value, 1, 1, 1, 1)
		VMADN_operation(6, state.v[21].U16[6], state.v[31].U16[6], s2value, 1, 1, 1, 1)
		VMADN_operation(7, state.v[21].U16[7], state.v[31].U16[7], s2value, 1, 1, 1, 1)
	}


_04001AEC:	// 4001aec: <06e10003>	BGEZ ?-?
	sp_reg_pc = 0x04001AEC;
	if (state.r[S7] >= 0)
	{
		rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);
		goto _04001AFC;
	}


_04001AF0:	// 4001af0: <4a139cec>	VXOR      vec19 = vec19 xor vec19[<none>]
	rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);


_04001AF4:	// 4001af4: <090006c0>	J         04001b00
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);
	goto _04001B00;


_04001AF8:	// 4001af8: <4b18a523>	VGE       vec20 = (vec20 >= vec24[0])
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001AFC:	// 4001afc: <4b18a520>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001B00:	// 4001b00: <4a114c6c>	VXOR      vec17 = vec09 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[9], &state.v[17]);


_04001B04:	// 4001b04: <4bb8f5c4>	VMUDL     vec23 = ( acc = (vec30 * vec24[5] >> 16)      )
	{
		_u16		s2value = state.v[24].U16[2];
		VMUDL_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 0, 1, 1, 1)
		VMUDL_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 0, 1, 1, 1)
		VMUDL_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 0, 1, 1, 1)
		VMUDL_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 0, 1, 1, 1)
		VMUDL_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 0, 1, 1, 1)
		VMUDL_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 0, 1, 1, 1)
		VMUDL_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 0, 1, 1, 1)
		VMUDL_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 0, 1, 1, 1)
	}


_04001B08:	// 4001b08: <ca5a2000>	LQV       vec26 <00> = [S2 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S2]);
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


_04001B0C:	// 4001b0c: <4b98f5ce>	VMADN     vec23 = ( acc+= vec30 * vec24[4] )
	{
		_u16		s2value = state.v[24].U16[3];
		VMADN_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 1, 1, 1, 1)
		VMADN_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 1, 1, 1, 1)
		VMADN_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 1, 1, 1, 1)
		VMADN_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 1, 1, 1, 1)
		VMADN_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 1, 1, 1, 1)
		VMADN_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 1, 1, 1, 1)
		VMADN_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 1, 1, 1, 1)
		VMADN_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 1, 1, 1, 1)
	}


_04001B10:	// 4001b10: <2231fff0>	ADDI      S1 = S1 + fff0
	state.r[S1] = state.r[S1] + 0xFFFFFFF0;


_04001B14:	// 4001b14: <4bf2fc8f>	VMADH     vec18 = ( acc+= (vec31 * vec18[7]) << 16) >> 16
	{
		_u16		s2value = state.v[18].U16[0];
		VMADH_operation(0, state.v[18].U16[0], state.v[31].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[18].U16[1], state.v[31].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[18].U16[2], state.v[31].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[18].U16[3], state.v[31].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[18].U16[4], state.v[31].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[18].U16[5], state.v[31].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[18].U16[6], state.v[31].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[18].U16[7], state.v[31].U16[7], s2value, 1, 1, 0, 1)
	}


_04001B18:	// 4001b18: <22100010>	ADDI      S0 = S0 + 0010
	state.r[S0] = state.r[S0] + 0x0010;


_04001B1C:	// 4001b1c: <4b00fcce>	VMADN     vec19 = ( acc+= vec31 * vec00[0] )
	{
		_u16		s2value = state.v[0].U16[7];
		VMADN_operation(0, state.v[19].U16[0], state.v[31].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[19].U16[1], state.v[31].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[19].U16[2], state.v[31].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[19].U16[3], state.v[31].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[19].U16[4], state.v[31].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[19].U16[5], state.v[31].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[19].U16[6], state.v[31].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[19].U16[7], state.v[31].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B20:	// 4001b20: <4bd8a400>	VMULF     vec16 = ( acc = (vec20 * vec24[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[1];
		VMULF_operation(0, state.v[16].U16[0], state.v[20].U16[0], s2value, 1, 1, 1, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[20].U16[1], s2value, 1, 1, 1, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[20].U16[2], s2value, 1, 1, 1, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[20].U16[3], s2value, 1, 1, 1, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[20].U16[4], s2value, 1, 1, 1, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[20].U16[5], s2value, 1, 1, 1, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[20].U16[6], s2value, 1, 1, 1, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[20].U16[7], s2value, 1, 1, 1, 1)
	}


_04001B24:	// 4001b24: <06c10003>	BGEZ ?-?
	sp_reg_pc = 0x04001B24;
	if (state.r[S6] >= 0)
	{
		{
			_u16		s2value = state.v[24].U16[0];
			VMULF_operation(0, state.v[15].U16[0], state.v[20].U16[0], s2value, 1, 1, 1, 1)
			VMULF_operation(1, state.v[15].U16[1], state.v[20].U16[1], s2value, 1, 1, 1, 1)
			VMULF_operation(2, state.v[15].U16[2], state.v[20].U16[2], s2value, 1, 1, 1, 1)
			VMULF_operation(3, state.v[15].U16[3], state.v[20].U16[3], s2value, 1, 1, 1, 1)
			VMULF_operation(4, state.v[15].U16[4], state.v[20].U16[4], s2value, 1, 1, 1, 1)
			VMULF_operation(5, state.v[15].U16[5], state.v[20].U16[5], s2value, 1, 1, 1, 1)
			VMULF_operation(6, state.v[15].U16[6], state.v[20].U16[6], s2value, 1, 1, 1, 1)
			VMULF_operation(7, state.v[15].U16[7], state.v[20].U16[7], s2value, 1, 1, 1, 1)
		}
		goto _04001B34;
	}


_04001B28:	// 4001b28: <4bf8a3c0>	VMULF     vec15 = ( acc = (vec20 * vec24[7]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[0];
		VMULF_operation(0, state.v[15].U16[0], state.v[20].U16[0], s2value, 1, 1, 1, 1)
		VMULF_operation(1, state.v[15].U16[1], state.v[20].U16[1], s2value, 1, 1, 1, 1)
		VMULF_operation(2, state.v[15].U16[2], state.v[20].U16[2], s2value, 1, 1, 1, 1)
		VMULF_operation(3, state.v[15].U16[3], state.v[20].U16[3], s2value, 1, 1, 1, 1)
		VMULF_operation(4, state.v[15].U16[4], state.v[20].U16[4], s2value, 1, 1, 1, 1)
		VMULF_operation(5, state.v[15].U16[5], state.v[20].U16[5], s2value, 1, 1, 1, 1)
		VMULF_operation(6, state.v[15].U16[6], state.v[20].U16[6], s2value, 1, 1, 1, 1)
		VMULF_operation(7, state.v[15].U16[7], state.v[20].U16[7], s2value, 1, 1, 1, 1)
	}


_04001B2C:	// 4001b2c: <090006ce>	J         04001b38
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);
	goto _04001B38;


_04001B30:	// 4001b30: <4b7894a3>	VGE       vec18 = (vec18 >= vec24[3])
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001B34:	// 4001b34: <4b7894a0>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001B38:	// 4001b38: <4baaef40>	VMULF     vec29 = ( acc = (vec29 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[29].U16[0], state.v[29].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[29].U16[1], state.v[29].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[29].U16[2], state.v[29].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[29].U16[3], state.v[29].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[29].U16[4], state.v[29].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[29].U16[5], state.v[29].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[29].U16[6], state.v[29].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[29].U16[7], state.v[29].U16[7], s2value, 0, 1, 1, 1)
	}


_04001B3C:	// 4001b3c: <4a108f48>	VMACF     vec29 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[29].U16[0], state.v[17].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VMACF_operation(1, state.v[29].U16[1], state.v[17].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VMACF_operation(2, state.v[29].U16[2], state.v[17].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VMACF_operation(3, state.v[29].U16[3], state.v[17].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VMACF_operation(4, state.v[29].U16[4], state.v[17].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VMACF_operation(5, state.v[29].U16[5], state.v[17].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VMACF_operation(6, state.v[29].U16[6], state.v[17].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VMACF_operation(7, state.v[29].U16[7], state.v[17].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
	}


_04001B40:	// 4001b40: <4baadec0>	VMULF     vec27 = ( acc = (vec27 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[27].U16[0], state.v[27].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[27].U16[1], state.v[27].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[27].U16[2], state.v[27].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[27].U16[3], state.v[27].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[27].U16[4], state.v[27].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[27].U16[5], state.v[27].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[27].U16[6], state.v[27].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[27].U16[7], state.v[27].U16[7], s2value, 0, 1, 1, 1)
	}


_04001B44:	// 4001b44: <4a0f8ec8>	VMACF     vec27 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[27].U16[0], state.v[17].U16[0], state.v[15].U16[0], 1, 0, 0, 1)
		VMACF_operation(1, state.v[27].U16[1], state.v[17].U16[1], state.v[15].U16[1], 1, 0, 0, 1)
		VMACF_operation(2, state.v[27].U16[2], state.v[17].U16[2], state.v[15].U16[2], 1, 0, 0, 1)
		VMACF_operation(3, state.v[27].U16[3], state.v[17].U16[3], state.v[15].U16[3], 1, 0, 0, 1)
		VMACF_operation(4, state.v[27].U16[4], state.v[17].U16[4], state.v[15].U16[4], 1, 0, 0, 1)
		VMACF_operation(5, state.v[27].U16[5], state.v[17].U16[5], state.v[15].U16[5], 1, 0, 0, 1)
		VMACF_operation(6, state.v[27].U16[6], state.v[17].U16[6], state.v[15].U16[6], 1, 0, 0, 1)
		VMACF_operation(7, state.v[27].U16[7], state.v[17].U16[7], state.v[15].U16[7], 1, 0, 0, 1)
	}


_04001B48:	// 4001b48: <4a11446c>	VXOR      vec17 = vec08 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[8], &state.v[17]);


_04001B4C:	// 4001b4c: <4bd89400>	VMULF     vec16 = ( acc = (vec18 * vec24[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[1];
		VMULF_operation(0, state.v[16].U16[0], state.v[18].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[18].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[18].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[18].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[18].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[18].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[18].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[18].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B50:	// 4001b50: <4bf893c0>	VMULF     vec15 = ( acc = (vec18 * vec24[7]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[0];
		VMULF_operation(0, state.v[15].U16[0], state.v[18].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[15].U16[1], state.v[18].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[15].U16[2], state.v[18].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[15].U16[3], state.v[18].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[15].U16[4], state.v[18].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[15].U16[5], state.v[18].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[15].U16[6], state.v[18].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[15].U16[7], state.v[18].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B54:	// 4001b54: <eabd2000>	SQV       [S5 + 0x00] = vec29 <00>
	{
		_u32    addr = (0x00000000 + state.r[S5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[29].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[29].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[29].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[29].U32[3];
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

			value1 |= (state.v[29].U64[0] & temp1);
			value2 |= (state.v[29].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001B58:	// 4001b58: <4baae700>	VMULF     vec28 = ( acc = (vec28 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[28].U16[0], state.v[28].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[28].U16[1], state.v[28].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[28].U16[2], state.v[28].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[28].U16[3], state.v[28].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[28].U16[4], state.v[28].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[28].U16[5], state.v[28].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[28].U16[6], state.v[28].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[28].U16[7], state.v[28].U16[7], s2value, 0, 1, 1, 1)
	}


_04001B5C:	// 4001b5c: <22b50010>	ADDI      S5 = S5 + 0010
	state.r[S5] = state.r[S5] + 0x0010;


_04001B60:	// 4001b60: <4a108f08>	VMACF     vec28 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[28].U16[0], state.v[17].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VMACF_operation(1, state.v[28].U16[1], state.v[17].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VMACF_operation(2, state.v[28].U16[2], state.v[17].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VMACF_operation(3, state.v[28].U16[3], state.v[17].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VMACF_operation(4, state.v[28].U16[4], state.v[17].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VMACF_operation(5, state.v[28].U16[5], state.v[17].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VMACF_operation(6, state.v[28].U16[6], state.v[17].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VMACF_operation(7, state.v[28].U16[7], state.v[17].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
	}


_04001B64:	// 4001b64: <ea7b2000>	SQV       [S3 + 0x00] = vec27 <00>
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


_04001B68:	// 4001b68: <4baad680>	VMULF     vec26 = ( acc = (vec26 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[26].U16[0], state.v[26].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[26].U16[1], state.v[26].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[26].U16[2], state.v[26].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[26].U16[3], state.v[26].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[26].U16[4], state.v[26].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[26].U16[5], state.v[26].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[26].U16[6], state.v[26].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[26].U16[7], state.v[26].U16[7], s2value, 0, 1, 1, 1)
	}


_04001B6C:	// 4001b6c: <22730010>	ADDI      S3 = S3 + 0010
	state.r[S3] = state.r[S3] + 0x0010;


_04001B70:	// 4001b70: <4a0f8e88>	VMACF     vec26 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[26].U16[0], state.v[17].U16[0], state.v[15].U16[0], 1, 1, 1, 1)
		VMACF_operation(1, state.v[26].U16[1], state.v[17].U16[1], state.v[15].U16[1], 1, 1, 1, 1)
		VMACF_operation(2, state.v[26].U16[2], state.v[17].U16[2], state.v[15].U16[2], 1, 1, 1, 1)
		VMACF_operation(3, state.v[26].U16[3], state.v[17].U16[3], state.v[15].U16[3], 1, 1, 1, 1)
		VMACF_operation(4, state.v[26].U16[4], state.v[17].U16[4], state.v[15].U16[4], 1, 1, 1, 1)
		VMACF_operation(5, state.v[26].U16[5], state.v[17].U16[5], state.v[15].U16[5], 1, 1, 1, 1)
		VMACF_operation(6, state.v[26].U16[6], state.v[17].U16[6], state.v[15].U16[6], 1, 1, 1, 1)
		VMACF_operation(7, state.v[26].U16[7], state.v[17].U16[7], state.v[15].U16[7], 1, 1, 1, 1)
	}


_04001B74:	// 4001b74: <ea9c2000>	SQV       [S4 + 0x00] = vec28 <00>
	{
		_u32    addr = (0x00000000 + state.r[S4]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[28].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[28].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[28].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[28].U32[3];
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

			value1 |= (state.v[28].U64[0] & temp1);
			value2 |= (state.v[28].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001B78:	// 4001b78: <22940010>	ADDI      S4 = S4 + 0010
	state.r[S4] = state.r[S4] + 0x0010;


_04001B7C:	// 4001b7c: <ea5a2000>	SQV       [S2 + 0x00] = vec26 <00>
	{
		_u32    addr = (0x00000000 + state.r[S2]);
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


_04001B80:	// 4001b80: <22520010>	ADDI      S2 = S2 + 0010
	state.r[S2] = state.r[S2] + 0x0010;


_04001B84:	// 4001b84: <4b58ad54>	VADDC	vec21 = vec21 + vec24[2]
	{
		_u16		flag0 = 0;
		_u16		s2value = state.v[24].U16[5];
		VADDC_operation(0, state.v[21].U16[0], state.v[21].U16[0], s2value, 1, 0, 0, 1)
		VADDC_operation(1, state.v[21].U16[1], state.v[21].U16[1], s2value, 1, 0, 0, 1)
		VADDC_operation(2, state.v[21].U16[2], state.v[21].U16[2], s2value, 1, 0, 0, 1)
		VADDC_operation(3, state.v[21].U16[3], state.v[21].U16[3], s2value, 1, 0, 0, 1)
		VADDC_operation(4, state.v[21].U16[4], state.v[21].U16[4], s2value, 1, 0, 0, 1)
		VADDC_operation(5, state.v[21].U16[5], state.v[21].U16[5], s2value, 1, 0, 0, 1)
		VADDC_operation(6, state.v[21].U16[6], state.v[21].U16[6], s2value, 1, 0, 0, 1)
		VADDC_operation(7, state.v[21].U16[7], state.v[21].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = flag0;
	}


_04001B88:	// 4001b88: <4b38a510>	VADD      vec20 = vec20 + vec24[1]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[24].U16[6];
		VADD_operation(0, state.v[20].U16[0], state.v[20].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[20].U16[1], state.v[20].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[20].U16[2], state.v[20].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[20].U16[3], state.v[20].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[20].U16[4], state.v[20].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[20].U16[5], state.v[20].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[20].U16[6], state.v[20].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[20].U16[7], state.v[20].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B8C:	// 4001b8c: <cabd2000>	LQV       vec29 <00> = [S5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S5]);
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


_04001B90:	// 4001b90: <4bb89cd4>	VADDC	vec19 = vec19 + vec24[5]
	{
		_u16		flag0 = 0;
		_u16		s2value = state.v[24].U16[2];
		VADDC_operation(0, state.v[19].U16[0], state.v[19].U16[0], s2value, 1, 0, 0, 1)
		VADDC_operation(1, state.v[19].U16[1], state.v[19].U16[1], s2value, 1, 0, 0, 1)
		VADDC_operation(2, state.v[19].U16[2], state.v[19].U16[2], s2value, 1, 0, 0, 1)
		VADDC_operation(3, state.v[19].U16[3], state.v[19].U16[3], s2value, 1, 0, 0, 1)
		VADDC_operation(4, state.v[19].U16[4], state.v[19].U16[4], s2value, 1, 0, 0, 1)
		VADDC_operation(5, state.v[19].U16[5], state.v[19].U16[5], s2value, 1, 0, 0, 1)
		VADDC_operation(6, state.v[19].U16[6], state.v[19].U16[6], s2value, 1, 0, 0, 1)
		VADDC_operation(7, state.v[19].U16[7], state.v[19].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = flag0;
	}


_04001B94:	// 4001b94: <ca112000>	LQV       vec17 <00> = [S0 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S0]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[17].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[17].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[17].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[17].U32[3] = *(_u32 *)&pDMEM[addr];
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

			state.v[17].U64[0] = state.v[17].U64[0] & ~temp1;
			state.v[17].U64[1] = state.v[17].U64[1] & ~temp2;
			state.v[17].U64[0] = state.v[17].U64[0] | value1;
			state.v[17].U64[1] = state.v[17].U64[1] | value2;
		}
	}


_04001B98:	// 4001b98: <06e10003>	BGEZ ?-?
	sp_reg_pc = 0x04001B98;
	if (state.r[S7] >= 0)
	{
		{
			_u16		flag0 = state.flag[0];
			_u16		s2value = state.v[24].U16[3];
			VADD_operation(0, state.v[18].U16[0], state.v[18].U16[0], s2value, 1, 1, 1, 1)
			VADD_operation(1, state.v[18].U16[1], state.v[18].U16[1], s2value, 1, 1, 1, 1)
			VADD_operation(2, state.v[18].U16[2], state.v[18].U16[2], s2value, 1, 1, 1, 1)
			VADD_operation(3, state.v[18].U16[3], state.v[18].U16[3], s2value, 1, 1, 1, 1)
			VADD_operation(4, state.v[18].U16[4], state.v[18].U16[4], s2value, 1, 1, 1, 1)
			VADD_operation(5, state.v[18].U16[5], state.v[18].U16[5], s2value, 1, 1, 1, 1)
			VADD_operation(6, state.v[18].U16[6], state.v[18].U16[6], s2value, 1, 1, 1, 1)
			VADD_operation(7, state.v[18].U16[7], state.v[18].U16[7], s2value, 1, 1, 1, 1)
			state.flag[0] = 0x0000;
		}
		goto _04001BA8;
	}


_04001B9C:	// 4001b9c: <4b989490>	VADD      vec18 = vec18 + vec24[4]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[24].U16[3];
		VADD_operation(0, state.v[18].U16[0], state.v[18].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[18].U16[1], state.v[18].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[18].U16[2], state.v[18].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[18].U16[3], state.v[18].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[18].U16[4], state.v[18].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[18].U16[5], state.v[18].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[18].U16[6], state.v[18].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[18].U16[7], state.v[18].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001BA0:	// 4001ba0: <090006eb>	J         04001bac
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);
	goto _04001BAC;


_04001BA4:	// 4001ba4: <4b18a523>	VGE       vec20 = (vec20 >= vec24[0])
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001BA8:	// 4001ba8: <4b18a520>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001BAC:	// 4001bac: <4a114c6c>	VXOR      vec17 = vec09 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[9], &state.v[17]);


_04001BB0:	// 4001bb0: <06c10003>	BGEZ ?-?
	sp_reg_pc = 0x04001BB0;
	if (state.r[S6] >= 0)
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
		goto _04001BC0;
	}


_04001BB4:	// 4001bb4: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
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


_04001BB8:	// 4001bb8: <090006f1>	J         04001bc4
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);
	goto _04001BC4;


_04001BBC:	// 4001bbc: <4b7894a3>	VGE       vec18 = (vec18 >= vec24[3])
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001BC0:	// 4001bc0: <4b7894a0>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001BC4:	// 4001bc4: <4bd8a400>	VMULF     vec16 = ( acc = (vec20 * vec24[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[1];
		VMULF_operation(0, state.v[16].U16[0], state.v[20].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[20].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[20].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[20].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[20].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[20].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[20].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[20].U16[7], s2value, 1, 0, 0, 1)
	}


_04001BC8:	// 4001bc8: <eb142000>	SQV       [T8 + 0x00] = vec20 <00>
	// In function: Tooie1F20_1F84
	//Call the function: Tooie1F20_1F84
	Tooie1F20_1F84();
	goto _04001C30;

_04001BCC:	// 4001bcc: <4bf8a3c0>	VMULF     vec15 = ( acc = (vec20 * vec24[7]) << 1 ) >> 16
	ErrorToCallFunction(0x04001BCC);

_04001BD0:	// 4001bd0: <eb152001>	SQV       [T8 + 0x10] = vec21 <00>
	ErrorToCallFunction(0x04001BD0);

_04001BD4:	// 4001bd4: <4baaef40>	VMULF     vec29 = ( acc = (vec29 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001BD4);

_04001BD8:	// 4001bd8: <4a108f48>	VMACF     vec29 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001BD8);

_04001BDC:	// 4001bdc: <ca9c2000>	LQV       vec28 <00> = [S4 + 0x00]
	ErrorToCallFunction(0x04001BDC);

_04001BE0:	// 4001be0: <4baadec0>	VMULF     vec27 = ( acc = (vec27 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001BE0);

_04001BE4:	// 4001be4: <ca5a2000>	LQV       vec26 <00> = [S2 + 0x00]
	ErrorToCallFunction(0x04001BE4);

_04001BE8:	// 4001be8: <4a0f8ec8>	VMACF     vec27 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001BE8);

_04001BEC:	// 4001bec: <4a11446c>	VXOR      vec17 = vec08 xor vec17[<none>]
	ErrorToCallFunction(0x04001BEC);

_04001BF0:	// 4001bf0: <2231fff0>	ADDI      S1 = S1 + fff0
	ErrorToCallFunction(0x04001BF0);

_04001BF4:	// 4001bf4: <4b58ad54>	VADDC	vec21 = vec21 + vec24[2]
	ErrorToCallFunction(0x04001BF4);

_04001BF8:	// 4001bf8: <22100010>	ADDI      S0 = S0 + 0010
	ErrorToCallFunction(0x04001BF8);

_04001BFC:	// 4001bfc: <4b38a510>	VADD      vec20 = vec20 + vec24[1]
	ErrorToCallFunction(0x04001BFC);

_04001C00:	// 4001c00: <eabd2000>	SQV       [S5 + 0x00] = vec29 <00>
	ErrorToCallFunction(0x04001C00);

_04001C04:	// 4001c04: <4bd89400>	VMULF     vec16 = ( acc = (vec18 * vec24[6]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C04);

_04001C08:	// 4001c08: <22b50010>	ADDI      S5 = S5 + 0010
	ErrorToCallFunction(0x04001C08);

_04001C0C:	// 4001c0c: <4bf893c0>	VMULF     vec15 = ( acc = (vec18 * vec24[7]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C0C);

_04001C10:	// 4001c10: <ea7b2000>	SQV       [S3 + 0x00] = vec27 <00>
	ErrorToCallFunction(0x04001C10);

_04001C14:	// 4001c14: <4baae700>	VMULF     vec28 = ( acc = (vec28 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C14);

_04001C18:	// 4001c18: <22730010>	ADDI      S3 = S3 + 0010
	ErrorToCallFunction(0x04001C18);

_04001C1C:	// 4001c1c: <4a108f08>	VMACF     vec28 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C1C);

_04001C20:	// 4001c20: <4baad680>	VMULF     vec26 = ( acc = (vec26 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C20);

_04001C24:	// 4001c24: <4a0f8e88>	VMACF     vec26 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C24);

_04001C28:	// 4001c28: <ea9c2000>	SQV       [S4 + 0x00] = vec28 <00>
	ErrorToCallFunction(0x04001C28);

_04001C2C:	// 4001c2c: <22940010>	ADDI      S4 = S4 + 0010
	ErrorToCallFunction(0x04001C2C);

_04001C30:	// 4001c30: <1a200003>	BLEZ      (R0<=0) --> 1c40
	sp_reg_pc = 0x04001C30;
	if ((_s32)state.r[S1] <= 0)
	{
		{
			_u32    addr = (0x00000000 + state.r[S2]);
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
		goto _04001C40;
	}


_04001C34:	// 4001c34: <ea5a2000>	SQV       [S2 + 0x00] = vec26 <00>
	{
		_u32    addr = (0x00000000 + state.r[S2]);
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


_04001C38:	// 4001c38: <090006e3>	J         04001b8c
	state.r[S2] = state.r[S2] + 0x0010;
	goto _04001B8C;


_04001C3C:	// 4001c3c: <22520010>	ADDI      S2 = S2 + 0010
	state.r[S2] = state.r[S2] + 0x0010;


_04001C40:	// 4001c40: <eb122002>	SQV       [T8 + 0x20] = vec18 <00>
	{
		_u32    addr = (0x00000020 + state.r[T8]);
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


_04001C44:	// 4001c44: <eb132003>	SQV       [T8 + 0x30] = vec19 <00>
	{
		_u32    addr = (0x00000030 + state.r[T8]);
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


_04001C48:	// 4001c48: <0d00045d>	JAL	    04001174
	{
		_u32    addr = (0x00000040 + state.r[T8]);
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
	sp_reg_pc = 0x04001C48;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001174;


_04001C4C:	// 4001c4c: <eb182004>	SQV       [T8 + 0x40] = vec24 <00>
	{
		_u32    addr = (0x00000040 + state.r[T8]);
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


_04001C50:	// 4001c50: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_04001C54:	// 4001c54: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001C58:	// 4001c58: <c81f2005>	LQV       vec31 <00> = [R0 + 0x50]
	// In function: SmashBros1C84_1CC8
	//Call the function: SmashBros1C84_1CC8
	SmashBros1C84_1CC8();
	goto _04001CA0;

_04001C5C:	// 4001c5c: <3336ffff>	ANDI      S6 = T9 & ffff
	ErrorToCallFunction(0x04001C5C);

_04001C60:	// 4001c60: <22d604f0>	ADDI      S6 = S6 + 04f0
	ErrorToCallFunction(0x04001C60);

_04001C64:	// 4001c64: <cadc2000>	LQV       vec28 <00> = [S6 + 0x00]
	ErrorToCallFunction(0x04001C64);

_04001C68:	// 4001c68: <0019bc02>	SRL       S7 = T9 >> 16
	ErrorToCallFunction(0x04001C68);

_04001C6C:	// 4001c6c: <22f704f0>	ADDI      S7 = S7 + 04f0
	ErrorToCallFunction(0x04001C6C);

_04001C70:	// 4001c70: <cafd2000>	LQV       vec29 <00> = [S7 + 0x00]
	ErrorToCallFunction(0x04001C70);

_04001C74:	// 4001c74: <489af000>	MTC2      vec30 <00> = K0
	ErrorToCallFunction(0x04001C74);

_04001C78:	// 4001c78: <20150170>	ADDI      S5 = R0 + 0170
	ErrorToCallFunction(0x04001C78);

_04001C7C:	// 4001c7c: <4bbfe6c0>	VMULF     vec27 = ( acc = (vec28 * vec31[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C7C);

_04001C80:	// 4001c80: <22b5fff0>	ADDI      S5 = S5 + fff0
	ErrorToCallFunction(0x04001C80);

_04001C84:	// 4001c84: <22f70010>	ADDI      S7 = S7 + 0010
	ErrorToCallFunction(0x04001C84);

_04001C88:	// 4001c88: <22d60010>	ADDI      S6 = S6 + 0010
	ErrorToCallFunction(0x04001C88);

_04001C8C:	// 4001c8c: <4b1eeec8>	VMACF     vec27 = ( acc += (vec29 * vec30[0]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C8C);

_04001C90:	// 4001c90: <cadc2000>	LQV       vec28 <00> = [S6 + 0x00]
	ErrorToCallFunction(0x04001C90);

_04001C94:	// 4001c94: <cafd2000>	LQV       vec29 <00> = [S7 + 0x00]
	ErrorToCallFunction(0x04001C94);

_04001C98:	// 4001c98: <1ea0fff8>	BGTZ      (R0>0) --> 1c7c
	ErrorToCallFunction(0x04001C98);

_04001C9C:	// 4001c9c: <eadb207f>	SQV       [S6 + 0xf0] = vec27 <00>
	ErrorToCallFunction(0x04001C9C);

_04001CA0:	// 4001ca0: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_04001CA4:	// 4001ca4: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001CA8:	// 4001ca8: <00000000>	NOP       


_04001CAC:	// 4001cac: <00000000>	NOP       


_04001CB0:	// 4001cb0: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_04001CB4:	// 4001cb4: <c81f201b>	LQV       vec31 <00> = [R0 + 0xb0]
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


_04001CB8:	// 4001cb8: <c81e201c>	LQV       vec30 <00> = [R0 + 0xc0]
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


_04001CBC:	// 4001cbc: <20170ba8>	ADDI      S7 = R0 + 0ba8
	state.r[S7] = 0 + 0x0BA8;


_04001CC0:	// 4001cc0: <20160d00>	ADDI      S6 = R0 + 0d00
	state.r[S6] = 0 + 0x0D00;


_04001CC4:	// 4001cc4: <4b1f0051>	VSUB      vec01 = vec00 - vec31[0]
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


_04001CC8:	// 4001cc8: <8c0b00f0>	LW        T3 = [R0+00f0]
	state.r[T3] = (_s32)Load32_DMEM((0 + 0x000000F0) & 0xfff);


_04001CCC:	// 4001ccc: <8c0c0fc4>	LW        T4 = [R0+0fc4]
	state.r[T4] = (_s32)Load32_DMEM((0 + 0x00000FC4) & 0xfff);


_04001CD0:	// 4001cd0: <20012800>	ADDI      AT = R0 + 2800
	state.r[AT] = 0 + 0x2800;


_04001CD4:	// 4001cd4: <11600006>	BEQ       (R0==T3) --> 1cf0
	sp_reg_pc = 0x04001CD4;
	if (state.r[T3] == 0)
	{
		goto _04001CF0;
	}


_04001CD8:	// 4001cd8: <40812000>	MTC0      SP status = AT


_04001CDC:	// 4001cdc: <318c0001>	ANDI      T4 = T4 & 0001
	state.r[T4] = (state.r[T4] & 0x00000001);


_04001CE0:	// 4001ce0: <1180001e>	BEQ       (R0==T4) --> 1d5c
	sp_reg_pc = 0x04001CE0;
	if (state.r[T4] == 0)
	{
		Save32_DMEM((_u32)0, (0 + 0x00000FC4) & 0xfff);
		goto _04001D5C;
	}


_04001CE4:	// 4001ce4: <ac000fc4>	SW        [R0+0fc4] = R0
	Save32_DMEM((_u32)0, (0 + 0x00000FC4) & 0xfff);


_04001CE8:	// 4001ce8: <08000459>	J         00001164
	state.r[K0] = (_s32)Load32_DMEM((0 + 0x00000BF8) & 0xfff);
	state.halt = 1;
	return;


_04001CEC:	// 4001cec: <8c1a0bf8>	LW        K0 = [R0+0bf8]
	state.r[K0] = (_s32)Load32_DMEM((0 + 0x00000BF8) & 0xfff);


_04001CF0:	// 4001cf0: <400b5800>	MFC0      T3 = DP CMD status
	state.r[T3] = 0;


_04001CF4:	// 4001cf4: <316b0001>	ANDI      T3 = T3 & 0001
	state.r[T3] = (state.r[T3] & 0x00000001);


_04001CF8:	// 4001cf8: <1560000b>	BNE       (R0!=T3) --> 1d28
	sp_reg_pc = 0x04001CF8;
	if (state.r[T3] != 0)
	{
		state.r[V0] = 0;
		goto _04001D28;
	}


_04001CFC:	// 4001cfc: <40024800>	MFC0      T1 = SP read DMA length
	state.r[V0] = 0;


_04001D00:	// 4001d00: <8c030fe8>	LW        V1 = [R0+0fe8]
	state.r[V1] = (_s32)Load32_DMEM((0 + 0x00000FE8) & 0xfff);


_04001D04:	// 4001d04: <00625822>	SUB       T3 = V1-V0
	state.r[T3] = (state.r[V1] - state.r[V0]);


_04001D08:	// 4001d08: <1d600007>	BGTZ      (R0>0) --> 1d28
	sp_reg_pc = 0x04001D08;
	if((_s32)state.r[T3] > 0)
	{
		state.r[AT] = 0;
		goto _04001D28;
	}


_04001D0C:	// 4001d0c: <40015000>	MFC0      T2 = SP DRAM DMA address
	state.r[AT] = 0;


_04001D10:	// 4001d10: <8c040fec>	LW        A0 = [R0+0fec]
	state.r[A0] = (_s32)Load32_DMEM((0 + 0x00000FEC) & 0xfff);


_04001D14:	// 4001d14: <10200004>	BEQ       (R0==AT) --> 1d28
	sp_reg_pc = 0x04001D14;
	if (state.r[AT] == 0)
	{
		state.r[T3] = (state.r[AT] - state.r[A0]);
		goto _04001D28;
	}


_04001D18:	// 4001d18: <00245822>	SUB       T3 = AT-A0
	state.r[T3] = (state.r[AT] - state.r[A0]);


_04001D1C:	// 4001d1c: <05610002>	BGEZ ?-?
	sp_reg_pc = 0x04001D1C;
	if (state.r[T3] >= 0)
	{
		goto _04001D28;
	}


_04001D20:	// 4001d20: <00000000>	NOP       


_04001D24:	// 4001d24: <14220008>	BNE       (V0!=AT) --> 1d48
	sp_reg_pc = 0x04001D24;
	if (state.r[AT] != state.r[V0])
	{
		state.r[T3] = 0;
		goto _04001D48;
	}


_04001D28:	// 4001d28: <400b5800>	MFC0      T3 = DP CMD status
	state.r[T3] = 0;


_04001D2C:	// 4001d2c: <316b0400>	ANDI      T3 = T3 & 0400
	state.r[T3] = (state.r[T3] & 0x00000400);


_04001D30:	// 4001d30: <1560fffd>	BNE       (R0!=T3) --> 1d28
	sp_reg_pc = 0x04001D30;
	if (state.r[T3] != 0)
	{
		state.r[T3] = 0 + 0x0001;
		goto _04001D28;
	}


_04001D34:	// 4001d34: <200b0001>	ADDI      T3 = R0 + 0001
	state.r[T3] = 0 + 0x0001;


_04001D38:	// 4001d38: <408b5800>	MTC0      DP CMD status = T3


_04001D3C:	// 4001d3c: <8c020fec>	LW        V0 = [R0+0fec]
	state.r[V0] = (_s32)Load32_DMEM((0 + 0x00000FEC) & 0xfff);


_04001D40:	// 4001d40: <40824000>	MTC0      DP CMD DMA start = V0


_04001D44:	// 4001d44: <40824800>	MTC0      DP CMD DMA end = V0


_04001D48:	// 4001d48: <ac0200f0>	SW        [R0+00f0] = V0
	Save32_DMEM((_u32)state.r[V0], (0 + 0x000000F0) & 0xfff);


_04001D4C:	// 4001d4c: <8c0b00f4>	LW        T3 = [R0+00f4]
	state.r[T3] = (_s32)Load32_DMEM((0 + 0x000000F4) & 0xfff);


_04001D50:	// 4001d50: <15600002>	BNE       (R0!=T3) --> 1d5c
	sp_reg_pc = 0x04001D50;
	if (state.r[T3] != 0)
	{
		state.r[T3] = (_s32)Load32_DMEM((0 + 0x00000FE0) & 0xfff);
		goto _04001D5C;
	}


_04001D54:	// 4001d54: <8c0b0fe0>	LW        T3 = [R0+0fe0]
	state.r[T3] = (_s32)Load32_DMEM((0 + 0x00000FE0) & 0xfff);


_04001D58:	// 4001d58: <ac0b00f4>	SW        [R0+00f4] = T3
	Save32_DMEM((_u32)state.r[T3], (0 + 0x000000F4) & 0xfff);


_04001D5C:	// 4001d5c: <8c010fd0>	LW        AT = [R0+0fd0]
	state.r[AT] = (_s32)Load32_DMEM((0 + 0x00000FD0) & 0xfff);


_04001D60:	// 4001d60: <8c0202e0>	LW        V0 = [R0+02e0]
	state.r[V0] = (_s32)Load32_DMEM((0 + 0x000002E0) & 0xfff);


_04001D64:	// 4001d64: <8c0302e8>	LW        V1 = [R0+02e8]
	state.r[V1] = (_s32)Load32_DMEM((0 + 0x000002E8) & 0xfff);


_04001D68:	// 4001d68: <8c040410>	LW        A0 = [R0+0410]
	state.r[A0] = (_s32)Load32_DMEM((0 + 0x00000410) & 0xfff);


_04001D6C:	// 4001d6c: <8c050418>	LW        A1 = [R0+0418]
	state.r[A1] = (_s32)Load32_DMEM((0 + 0x00000418) & 0xfff);


_04001D70:	// 4001d70: <00411020>	ADD       V0 = V0+AT
	state.r[V0] = (state.r[V0] + state.r[AT]);


_04001D74:	// 4001d74: <00611820>	ADD       V1 = V1+AT
	state.r[V1] = (state.r[V1] + state.r[AT]);


_04001D78:	// 4001d78: <ac0202e0>	SW        [R0+02e0] = V0
	Save32_DMEM((_u32)state.r[V0], (0 + 0x000002E0) & 0xfff);


_04001D7C:	// 4001d7c: <ac0302e8>	SW        [R0+02e8] = V1
	Save32_DMEM((_u32)state.r[V1], (0 + 0x000002E8) & 0xfff);


_04001D80:	// 4001d80: <00812020>	ADD       A0 = A0+AT
	state.r[A0] = (state.r[A0] + state.r[AT]);


_04001D84:	// 4001d84: <00a12820>	ADD       A1 = A1+AT
	state.r[A1] = (state.r[A1] + state.r[AT]);


_04001D88:	// 4001d88: <ac040410>	SW        [R0+0410] = A0
	Save32_DMEM((_u32)state.r[A0], (0 + 0x00000410) & 0xfff);


_04001D8C:	// 4001d8c: <ac050418>	SW        [R0+0418] = A1
	Save32_DMEM((_u32)state.r[A1], (0 + 0x00000418) & 0xfff);


_04001D90:	// 4001d90: <8c1a0ff0>	LW        K0 = [R0+0ff0]
	state.r[K0] = (_s32)Load32_DMEM((0 + 0x00000FF0) & 0xfff);


_04001D94:	// 4001d94: <200b02e8>	ADDI      T3 = R0 + 02e8
	state.r[T3] = 0 + 0x02E8;


_04001D98:	// 4001d98: <0c0007ed>	JAL	    00001fb4
	state.r[T4] = (state.r[RA] | 0x00000000);
	sp_reg_pc = 0x04001D98;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	state.halt = 1;
	return;


_04001D9C:	// 4001d9c: <37ec0000>	ORI       T4 = RA | 0x0000
	state.r[T4] = (state.r[RA] | 0x00000000);


_04001DA0:	// 4001da0: <201300a7>	ADDI      S3 = R0 + 00a7
	state.r[S3] = 0 + 0x00A7;


_04001DA4:	// 4001da4: <37580000>	ORI       T8 = K0 | 0x0000
	state.r[T8] = (state.r[K0] | 0x00000000);


_04001DA8:	// 4001da8: <0c0007f6>	JAL	    00001fd8
	state.r[S4] = 0 + 0x0920;
	sp_reg_pc = 0x04001DA8;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	state.halt = 1;
	return;


_04001DAC:	// 4001dac: <24140920>	ADDIU     S4 = R0 + 0920
	state.r[S4] = 0 + 0x0920;


_04001DB0:	// 4001db0: <275a00a8>	ADDIU     K0 = K0 + 00a8
	state.r[K0] = state.r[K0] + 0x00A8;


_04001DB4:	// 4001db4: <201bff58>	ADDI      K1 = R0 + ff58
	state.r[K1] = 0 + 0xFFFFFF58;


_04001DB8:	// 4001db8: <0c0007f2>	JAL	    00001fc8
	state.r[AT] = 0;
	sp_reg_pc = 0x04001DB8;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	state.halt = 1;
	return;


_04001DBC:	// 4001dbc: <40012000>	MFC0      A0 = SP DRAM DMA address
	state.r[AT] = 0;


_04001DC0:	// 4001dc0: <8f7909c8>	LW        T9 = [K1+09c8]
	state.r[T9] = (_s32)Load32_DMEM((state.r[K1] + 0x000009C8) & 0xfff);


_04001DC4:	// 4001dc4: <1360fff6>	BEQ       (R0==K1) --> 1da0
	sp_reg_pc = 0x04001DC4;
	if (state.r[K1] == 0)
	{
		state.r[AT] = (state.r[AT] & 0x00000080);
		goto _04001DA0;
	}


_04001DC8:	// 4001dc8: <30210080>	ANDI      AT = AT & 0080
	state.r[AT] = (state.r[AT] & 0x00000080);


_04001DCC:	// 4001dcc: <00196603>	SRA       T4 = T9 >> 24
	state.r[T4] = ((_s32)state.r[T9] >> 24);


_04001DD0:	// 4001dd0: <000c5840>	SLL       T3 = T4 << 1
	state.r[T3] = ((_u32)state.r[T4] << 1);


_04001DD4:	// 4001dd4: <956b036e>	LHU       T3 = [T3+036e]
	state.r[T3] = (_s32)(_u16)Load16_DMEM((state.r[T3] + 0x0000036E) & 0xfff);


_04001DD8:	// 4001dd8: <14200380>	BNE       (R0!=AT) --> 2bdc
	sp_reg_pc = 0x04001DD8;
	if (state.r[AT] != 0)
	{
		state.r[T8] = (_s32)Load32_DMEM((state.r[K1] + 0x000009CC) & 0xfff);
		state.halt = 1;
		return;
	}


_04001DDC:	// 4001ddc: <8f7809cc>	LW        T8 = [K1+09cc]
	state.r[T8] = (_s32)Load32_DMEM((state.r[K1] + 0x000009CC) & 0xfff);


_04001DE0:	// 4001de0: <01600008>	JR        T3
	{
		_u32	temp = ((state.r[T3] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[K1] = state.r[K1] + 0x0008;
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001DE4:	// 4001de4: <277b0008>	ADDIU     K1 = K1 + 0008
	state.r[K1] = state.r[K1] + 0x0008;


_04001DE8:	// 4001de8: <0c000487>	JAL	    0000121c
	state.r[S4] = (_s32)(_s16)Load16_DMEM((state.r[K1] + 0x000009C1) & 0xfff);
	sp_reg_pc = 0x04001DE8;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	state.halt = 1;
	return;


_04001DEC:	// 4001dec: <877409c1>	LH        S4 = [K1+09c1]
	state.r[S4] = (_s32)(_s16)Load16_DMEM((state.r[K1] + 0x000009C1) & 0xfff);


_04001DF0:	// 4001df0: <33330ff8>	ANDI      S3 = T9 & 0ff8
	state.r[S3] = (state.r[T9] & 0x00000FF8);


_04001DF4:	// 4001df4: <0014a083>	SRA       S4 = S4 >> 2
	state.r[S4] = ((_s32)state.r[S4] >> 2);


_04001DF8:	// 4001df8: <080007f6>	J         00001fd8
	state.r[RA] = 0 + 0x1188;
	state.halt = 1;
	return;


_04001DFC:	// 4001dfc: <201f1188>	ADDI      RA = R0 + 1188
	state.r[RA] = 0 + 0x1188;


_04001E00:	// 4001e00: <8c0b01ec>	LW        T3 = [R0+01ec]
	state.r[T3] = (_s32)Load32_DMEM((0 + 0x000001EC) & 0xfff);


_04001E04:	// 4001e04: <01795824>	AND       T3 = T3 & T9
	state.r[T3] = (state.r[T3] & state.r[T9]);


_04001E08:	// 4001e08: <01785825>	OR        T3 = T3 | T8
	state.r[T3] = (state.r[T3] | state.r[T8]);


_04001E0C:	// 4001e0c: <08000463>	J         0000118c
	Save32_DMEM((_u32)state.r[T3], (0 + 0x000001EC) & 0xfff);
	state.halt = 1;
	return;


_04001E10:	// 4001e10: <ac0b01ec>	SW        [R0+01ec] = T3
	Save32_DMEM((_u32)state.r[T3], (0 + 0x000001EC) & 0xfff);


_04001E14:	// 4001e14: <900100de>	LBU       AT = [R0+00de]
	state.r[AT] = (_s32)(_u8)pDMEM[((0 + 0x000000DE) & 0xfff) ^ 3];


_04001E18:	// 4001e18: <10200370>	BEQ       (R0==AT) --> 2bdc
	sp_reg_pc = 0x04001E18;
	if (state.r[AT] == 0)
	{
		state.r[AT] = state.r[AT] + 0xFFFFFFFC;
		state.halt = 1;
		return;
	}


_04001E1C:	// 4001e1c: <2021fffc>	ADDI      AT = AT + fffc
	state.r[AT] = state.r[AT] + 0xFFFFFFFC;


_04001E20:	// 4001e20: <08000408>	J         00001020
	state.r[K0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000138) & 0xfff);
	state.halt = 1;
	return;


_04001E24:	// 4001e24: <8c3a0138>	LW        K0 = [AT+0138]
	state.r[K0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000138) & 0xfff);


_04001E28:	// 4001e28: <c81d181a>	LDV       vec29 <00> = [R0 + 0x50]
	rsp_ldv_compiler((0 + 0x000000D0) & 0xFFF, 29, 8);


_04001E2C:	// 4001e2c: <8c1900d8>	LW        T9 = [R0+00d8]
	state.r[T9] = (_s32)Load32_DMEM((0 + 0x000000D8) & 0xfff);


_04001E30:	// 4001e30: <22f70008>	ADDI      S7 = S7 + 0008
	state.r[S7] = state.r[S7] + 0x0008;


_04001E34:	// 4001e34: <eafd187f>	SDV       [S7 + 0x3c] = vec29 <00>
	{
		_u32	addr = (0xFFFFFFF8 + state.r[S7]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[29].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[29].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[29].U32[0x00000003];
		}
	}


_04001E38:	// 4001e38: <aef80004>	SW        [S7+0004] = T8
	Save32_DMEM((_u32)state.r[T8], (state.r[S7] + 0x00000004) & 0xfff);


_04001E3C:	// 4001e3c: <aef90000>	SW        [S7+0000] = T9
	Save32_DMEM((_u32)state.r[T9], (state.r[S7] + 0x00000000) & 0xfff);


_04001E40:	// 4001e40: <08000494>	J         00001250
	state.r[S7] = state.r[S7] + 0x0008;
	state.halt = 1;
	return;


_04001E44:	// 4001e44: <22f70008>	ADDI      S7 = S7 + 0008
	state.r[S7] = state.r[S7] + 0x0008;


_04001E48:	// 4001e48: <201f1208>	ADDI      RA = R0 + 1208
	state.r[RA] = 0 + 0x1208;


_04001E4C:	// 4001e4c: <00185d82>	SRL       T3 = T8 >> 22
	state.r[T3] = ((_u32)state.r[T8] >> 22);


_04001E50:	// 4001e50: <316b003c>	ANDI      T3 = T3 & 003c
	state.r[T3] = (state.r[T3] & 0x0000003C);


_04001E54:	// 4001e54: <8d6b00f8>	LW        T3 = [T3+00f8]
	state.r[T3] = (_s32)Load32_DMEM((state.r[T3] + 0x000000F8) & 0xfff);


_04001E58:	// 4001e58: <0018c200>	SLL       T8 = T8 << 8
	state.r[T8] = ((_u32)state.r[T8] << 8);


_04001E5C:	// 4001e5c: <0018c202>	SRL       T8 = T8 >> 8
	state.r[T8] = ((_u32)state.r[T8] >> 8);


_04001E60:	// 4001e60: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[T8] = (state.r[T8] + state.r[T3]);
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001E64:	// 4001e64: <030bc020>	ADD       T8 = T8+T3
	state.r[T8] = (state.r[T8] + state.r[T3]);


_04001E68:	// 4001e68: <ac1900c8>	SW        [R0+00c8] = T9
	Save32_DMEM((_u32)state.r[T9], (0 + 0x000000C8) & 0xfff);


_04001E6C:	// 4001e6c: <08000482>	J         00001208
	Save32_DMEM((_u32)state.r[T8], (0 + 0x000000CC) & 0xfff);
	state.halt = 1;
	return;


_04001E70:	// 4001e70: <ac1800cc>	SW        [R0+00cc] = T8
	Save32_DMEM((_u32)state.r[T8], (0 + 0x000000CC) & 0xfff);


_04001E74:	// 4001e74: <ac1900c0>	SW        [R0+00c0] = T9
	Save32_DMEM((_u32)state.r[T9], (0 + 0x000000C0) & 0xfff);


_04001E78:	// 4001e78: <08000482>	J         00001208
	Save32_DMEM((_u32)state.r[T8], (0 + 0x000000C4) & 0xfff);
	state.halt = 1;
	return;


_04001E7C:	// 4001e7c: <ac1800c4>	SW        [R0+00c4] = T8
	Save32_DMEM((_u32)state.r[T8], (0 + 0x000000C4) & 0xfff);


_04001E80:	// 4001e80: <201f118c>	ADDI      RA = R0 + 118c
	state.r[RA] = 0 + 0x118C;


_04001E84:	// 4001e84: <02f65822>	SUB       T3 = S7-S6
	state.r[T3] = (state.r[S7] - state.r[S6]);


_04001E88:	// 4001e88: <1960035e>	BLEZ      (R0<=0) --> 2c04
	sp_reg_pc = 0x04001E88;
	if ((_s32)state.r[T3] <= 0)
	{
		state.r[T4] = 0;
		state.halt = 1;
		return;
	}


_04001E8C:	// 4001e8c: <400c3000>	MFC0      A2 = DP clock counter
	state.r[T4] = 0;


_04001E90:	// 4001e90: <8c1800f0>	LW        T8 = [R0+00f0]
	state.r[T8] = (_s32)Load32_DMEM((0 + 0x000000F0) & 0xfff);


_04001E94:	// 4001e94: <25730158>	ADDIU     S3 = T3 + 0158
	state.r[S3] = state.r[T3] + 0x0158;


_04001E98:	// 4001e98: <1580fffc>	BNE       (R0!=T4) --> 1e8c
	sp_reg_pc = 0x04001E98;
	if (state.r[T4] != 0)
	{
		state.r[T4] = (_s32)Load32_DMEM((0 + 0x00000FEC) & 0xfff);
		goto _04001E8C;
	}


_04001E9C:	// 4001e9c: <8c0c0fec>	LW        T4 = [R0+0fec]
	state.r[T4] = (_s32)Load32_DMEM((0 + 0x00000FEC) & 0xfff);


_04001EA0:	// 4001ea0: <40984800>	MTC0      DP CMD DMA end = T8


_04001EA4:	// 4001ea4: <03135820>	ADD       T3 = T8+S3
	state.r[T3] = (state.r[T8] + state.r[S3]);


_04001EA8:	// 4001ea8: <018b6022>	SUB       T4 = T4-T3
	state.r[T4] = (state.r[T4] - state.r[T3]);


_04001EAC:	// 4001eac: <05810008>	BGEZ ?-?
	sp_reg_pc = 0x04001EAC;
	if (state.r[T4] >= 0)
	{
		state.r[T3] = 0;
		goto _04001ED0;
	}


_04001EB0:	// 4001eb0: <400b5800>	MFC0      T3 = DP CMD status
	state.r[T3] = 0;


_04001EB4:	// 4001eb4: <316b0400>	ANDI      T3 = T3 & 0400
	state.r[T3] = (state.r[T3] & 0x00000400);


_04001EB8:	// 4001eb8: <1560fffd>	BNE       (R0!=T3) --> 1eb0
	sp_reg_pc = 0x04001EB8;
	if (state.r[T3] != 0)
	{
		state.r[T8] = (_s32)Load32_DMEM((0 + 0x00000FE8) & 0xfff);
		goto _04001EB0;
	}


_04001EBC:	// 4001ebc: <8c180fe8>	LW        T8 = [R0+0fe8]
	state.r[T8] = (_s32)Load32_DMEM((0 + 0x00000FE8) & 0xfff);


_04001EC0:	// 4001ec0: <400b5000>	MFC0      T2 = DP CMD status
	state.r[T3] = 0;


_04001EC4:	// 4001ec4: <1178fffe>	BEQ       (T8==T3) --> 1ec0
	sp_reg_pc = 0x04001EC4;
	if (state.r[T3] == state.r[T8])
	{
		goto _04001EC0;
	}


_04001EC8:	// 4001ec8: <00000000>	NOP       


_04001ECC:	// 4001ecc: <40984000>	MTC0      DP CMD DMA start = T8


_04001ED0:	// 4001ed0: <400b5000>	MFC0      T2 = DP CMD status
	state.r[T3] = 0;


_04001ED4:	// 4001ed4: <01785822>	SUB       T3 = T3-T8
	state.r[T3] = (state.r[T3] - state.r[T8]);


_04001ED8:	// 4001ed8: <19600002>	BLEZ      (R0<=0) --> 1ee4
	sp_reg_pc = 0x04001ED8;
	if ((_s32)state.r[T3] <= 0)
	{
		state.r[T3] = (state.r[T3] - state.r[S3]);
		goto _04001EE4;
	}


_04001EDC:	// 4001edc: <01735822>	SUB       T3 = T3-S3
	state.r[T3] = (state.r[T3] - state.r[S3]);


_04001EE0:	// 4001ee0: <1960fffb>	BLEZ      (R0<=0) --> 1ed0
	sp_reg_pc = 0x04001EE0;
	if ((_s32)state.r[T3] <= 0)
	{
		state.r[T3] = (state.r[T8] + state.r[S3]);
		goto _04001ED0;
	}


_04001EE4:	// 4001ee4: <03135820>	ADD       T3 = T8+S3
	state.r[T3] = (state.r[T8] + state.r[S3]);

_04001EE8:
_04001EEC:
_04001EF0:
_04001EF4:
_04001EF8:
_04001EFC:
_04001F00:
_04001F04:
_04001F08:
_04001F0C:
_04001F10:
_04001F14:
_04001F18:
_04001F1C:
_04001F20:
_04001F24:
_04001F28:
_04001F2C:
_04001F30:
_04001F34:
_04001F38:
_04001F3C:
_04001F40:
_04001F44:
_04001F48:
_04001F4C:
_04001F50:
_04001F54:
_04001F58:
_04001F5C:
_04001F60:
_04001F64:
_04001F68:
_04001F6C:
_04001F70:
_04001F74:
_04001F78:
_04001F7C:
_04001F80:
_04001F84:
_04001F88:
_04001F8C:
_04001F90:
_04001F94:
_04001F98:
_04001F9C:
_04001FA0:
_04001FA4:
_04001FA8:
_04001FAC:
_04001FB0:
_04001FB4:
_04001FB8:
_04001FBC:
_04001FC0:
_04001FC4:
_04001FC8:
_04001FCC:
_04001FD0:
_04001FD4:
_04001FD8:
_04001FDC:
_04001FE0:
_04001FE4:
_04001FE8:
_04001FEC:
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

void _DK64Data(_u32 StartAddress)
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


_04001000:	// 4001000: <09000419>	J         04001064
	state.r[AT] = 0 + 0x0FC0;
	goto _04001064;


_04001004:	// 4001004: <20010fc0>	ADDI      AT = R0 + 0fc0
	state.r[AT] = 0 + 0x0FC0;


_04001008:	// 4001008: <8c220010>	LW        V0 = [AT+0010]
	state.r[V0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000010) & 0xfff);


_0400100C:	// 400100c: <20030f7f>	ADDI      V1 = R0 + 0f7f
	state.r[V1] = 0 + 0x0F7F;


_04001010:	// 4001010: <20071080>	ADDI      A3 = R0 + 1080
	state.r[A3] = 0 + 0x1080;


_04001014:	// 4001014: <40870000>	MTC0      SP memory address = A3
	DMEM_Address = state.r[A3];


_04001018:	// 4001018: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_0400101C:	// 400101c: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001020;
	_DK64Data(0x04001020);
	return;


_04001020:	// 4001020: <40043000>	MFC0      A2 = SP status
	state.r[A0] = 0;


_04001024:	// 4001024: <1480fffe>	BNE       (R0!=A0) --> 1020
	sp_reg_pc = 0x04001024;
	if (state.r[A0] != 0)
	{
		goto _04001020;
	}


_04001028:	// 4001028: <00000000>	NOP       


_0400102C:	// 400102c: <0d00040f>	JAL	    0400103c
	sp_reg_pc = 0x0400102C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400103C;


_04001030:	// 4001030: <00000000>	NOP       


_04001034:	// 4001034: <00e00008>	JR        A3
	{
		_u32	temp = ((state.r[A3] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001038:	// 4001038: <40803800>	MTC0      SP semaphore = R0


_0400103C:	// 400103c: <40082000>	MFC0      A0 = DP CMD DMA start
	state.r[T0] = 0;


_04001040:	// 4001040: <31080080>	ANDI      T0 = T0 & 0080
	state.r[T0] = (state.r[T0] & 0x00000080);


_04001044:	// 4001044: <15000002>	BNE       (R0!=T0) --> 1050
	sp_reg_pc = 0x04001044;
	if (state.r[T0] != 0)
	{
		goto _04001050;
	}


_04001048:	// 4001048: <00000000>	NOP       


_0400104C:	// 400104c: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001050:	// 4001050: <40803800>	MTC0      SP semaphore = R0


_04001054:	// 4001054: <34085200>	ORI       T0 = R0 | 0x5200
	state.r[T0] = (0 | 0x00005200);


_04001058:	// 4001058: <40882000>	MTC0      SP status = T0


_0400105C:	// 400105c: <0000000d>	BREAK
	state.halt = 1;
	return;


_04001060:	// 4001060: <00000000>	NOP       


_04001064:	// 4001064: <8c220004>	LW        V0 = [AT+0004]
	state.r[V0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000004) & 0xfff);


_04001068:	// 4001068: <30420002>	ANDI      V0 = V0 & 0002
	state.r[V0] = (state.r[V0] & 0x00000002);


_0400106C:	// 400106c: <10400007>	BEQ       (R0==V0) --> 108c
	sp_reg_pc = 0x0400106C;
	if (state.r[V0] == 0)
	{
		goto _0400108C;
	}


_04001070:	// 4001070: <00000000>	NOP       


_04001074:	// 4001074: <0d00040f>	JAL	    0400103c
	sp_reg_pc = 0x04001074;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400103C;


_04001078:	// 4001078: <00000000>	NOP       


_0400107C:	// 400107c: <40025800>	MFC0      T3 = SP read DMA length
	state.r[V0] = 0;


_04001080:	// 4001080: <40055800>	MFC0      T3 = SP DMA full
	state.r[A1] = 0;


_04001084:	// 4001084: <8c3c0030>	LW        GP = [AT+0030]
	state.r[GP] = (_s32)Load32_DMEM((state.r[AT] + 0x00000030) & 0xfff);


_04001088:	// 4001088: <8c3b0034>	LW        K1 = [AT+0034]
	state.r[K1] = (_s32)Load32_DMEM((state.r[AT] + 0x00000034) & 0xfff);


_0400108C:	// 400108c: <30a40001>	ANDI      A0 = A1 & 0001
	state.r[A0] = (state.r[A1] & 0x00000001);


_04001090:	// 4001090: <10800006>	BEQ       (R0==A0) --> 10ac
	sp_reg_pc = 0x04001090;
	if (state.r[A0] == 0)
	{
		state.r[A0] = (state.r[A1] & 0x00000100);
		goto _040010AC;
	}


_04001094:	// 4001094: <30a40100>	ANDI      A0 = A1 & 0100
	state.r[A0] = (state.r[A1] & 0x00000100);


_04001098:	// 4001098: <10800004>	BEQ       (R0==A0) --> 10ac
	sp_reg_pc = 0x04001098;
	if (state.r[A0] == 0)
	{
		state.r[A0] = 0;
		goto _040010AC;
	}


_0400109C:	// 400109c: <40045800>	MFC0      T3 = SP status
	state.r[A0] = 0;


_040010A0:	// 40010a0: <30840100>	ANDI      A0 = A0 & 0100
	state.r[A0] = (state.r[A0] & 0x00000100);


_040010A4:	// 40010a4: <1c80fffe>	BGTZ      (R0>0) --> 10a0
	sp_reg_pc = 0x040010A4;
	if((_s32)state.r[A0] > 0)
	{
		state.r[A0] = 0;
		goto _040010A0;
	}


_040010A8:	// 40010a8: <40045800>	MFC0      T3 = SP status
	state.r[A0] = 0;


_040010AC:	// 40010ac: <20180fa0>	ADDI      T8 = R0 + 0fa0
	state.r[T8] = 0 + 0x0FA0;


_040010B0:	// 40010b0: <0d00044b>	JAL	    0400112c
	state.r[V0] = (0 + state.r[GP]);
	sp_reg_pc = 0x040010B0;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400112C;


_040010B4:	// 40010b4: <001c1020>	ADD       V0 = R0+GP
	state.r[V0] = (0 + state.r[GP]);


_040010B8:	// 40010b8: <40023000>	MFC0      A2 = SP read DMA length
	state.r[V0] = 0;


_040010BC:	// 40010bc: <1440ffff>	BNE       (R0!=V0) --> 10bc
	sp_reg_pc = 0x040010BC;
	if (state.r[V0] != 0)
	{
		state.r[V0] = 0;
		goto _040010BC;
	}


_040010C0:	// 40010c0: <40023000>	MFC0      A2 = SP read DMA length
	state.r[V0] = 0;


_040010C4:	// 40010c4: <8fba0000>	LW        K0 = [SP+0000]
	state.r[K0] = (_s32)Load32_DMEM((state.r[SP] + 0x00000000) & 0xfff);


_040010C8:	// 40010c8: <8fb90004>	LW        T9 = [SP+0004]
	state.r[T9] = (_s32)Load32_DMEM((state.r[SP] + 0x00000004) & 0xfff);


_040010CC:	// 40010cc: <239c0008>	ADDI      GP = GP + 0008
	state.r[GP] = state.r[GP] + 0x0008;


_040010D0:	// 40010d0: <001a0dc2>	SRL       AT = K0 >> 23
	state.r[AT] = ((_u32)state.r[K0] >> 23);


_040010D4:	// 40010d4: <302100fe>	ANDI      AT = AT & 00fe
	state.r[AT] = (state.r[AT] & 0x000000FE);


_040010D8:	// 40010d8: <84210000>	LH        AT = [AT+0000]
	state.r[AT] = (_s32)(_s16)Load16_DMEM((state.r[AT] + 0x00000000) & 0xfff);


_040010DC:	// 40010dc: <00200008>	JR        AT
	{
		_u32	temp = ((state.r[AT] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[K1] = state.r[K1] + 0xFFFFFFF8;
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_040010E0:	// 40010e0: <237bfff8>	ADDI      K1 = K1 + fff8
	state.r[K1] = state.r[K1] + 0xFFFFFFF8;


_040010E4:	// 40010e4: <0000000d>	BREAK
	state.halt = 1;
	return;


_040010E8:	// 40010e8: <1fc0fff6>	BGTZ      (R0>0) --> 10c4
	sp_reg_pc = 0x040010E8;
	if((_s32)state.r[S8] > 0)
	{
		state.r[SP] = state.r[SP] + 0x0008;
		goto _040010C4;
	}


_040010EC:	// 40010ec: <23bd0008>	ADDI      SP = SP + 0008
	state.r[SP] = state.r[SP] + 0x0008;


_040010F0:	// 40010f0: <1b600005>	BLEZ      (R0<=0) --> 1108
	sp_reg_pc = 0x040010F0;
	if ((_s32)state.r[K1] <= 0)
	{
		state.r[AT] = (0 | 0x00004000);
		goto _04001108;
	}


_040010F4:	// 40010f4: <34014000>	ORI       AT = R0 | 0x4000
	state.r[AT] = (0 | 0x00004000);


_040010F8:	// 40010f8: <0d00044b>	JAL	    0400112c
	state.r[V0] = (0 + state.r[GP]);
	sp_reg_pc = 0x040010F8;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400112C;


_040010FC:	// 40010fc: <001c1020>	ADD       V0 = R0+GP
	state.r[V0] = (0 + state.r[GP]);


_04001100:	// 4001100: <0900042f>	J         040010bc
	state.r[V0] = 0;
	goto _040010BC;


_04001104:	// 4001104: <40023000>	MFC0      A2 = SP read DMA length
	state.r[V0] = 0;


_04001108:	// 4001108: <40023000>	MFC0      A2 = SP read DMA length
	state.r[V0] = 0;


_0400110C:	// 400110c: <1440fffe>	BNE       (R0!=V0) --> 1108
	sp_reg_pc = 0x0400110C;
	if (state.r[V0] != 0)
	{
		goto _04001108;
	}


_04001110:	// 4001110: <00000000>	NOP       


_04001114:	// 4001114: <40803800>	MTC0      SP semaphore = R0


_04001118:	// 4001118: <40812000>	MTC0      SP status = AT


_0400111C:	// 400111c: <0000000d>	BREAK
	state.halt = 1;
	return;


_04001120:	// 4001120: <00000000>	NOP       


_04001124:	// 4001124: <1000ffff>	BEQ       (R0==R0) --> 1124
	sp_reg_pc = 0x04001124;
	if (0 == 0)
	{
		goto _04001124;
	}


_04001128:	// 4001128: <00000000>	NOP       


_0400112C:	// 400112c: <23e50000>	ADDI      A1 = RA + 0000
	state.r[A1] = state.r[RA] + 0x0000;


_04001130:	// 4001130: <23630000>	ADDI      V1 = K1 + 0000
	state.r[V1] = state.r[K1] + 0x0000;


_04001134:	// 4001134: <2064fec0>	ADDI      A0 = V1 + fec0
	state.r[A0] = state.r[V1] + 0xFFFFFEC0;


_04001138:	// 4001138: <18800002>	BLEZ      (R0<=0) --> 1144
	sp_reg_pc = 0x04001138;
	if ((_s32)state.r[A0] <= 0)
	{
		state.r[AT] = 0 + 0x02B0;
		goto _04001144;
	}


_0400113C:	// 400113c: <200102b0>	ADDI      AT = R0 + 02b0
	state.r[AT] = 0 + 0x02B0;


_04001140:	// 4001140: <20030140>	ADDI      V1 = R0 + 0140
	state.r[V1] = 0 + 0x0140;


_04001144:	// 4001144: <207e0000>	ADDI      S8 = V1 + 0000
	state.r[S8] = state.r[V1] + 0x0000;


_04001148:	// 4001148: <0d000456>	JAL	    04001158
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x04001148;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001158;


_0400114C:	// 400114c: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001150:	// 4001150: <00a00008>	JR        A1
	{
		_u32	temp = ((state.r[A1] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[SP] = 0 + 0x02B0;
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001154:	// 4001154: <201d02b0>	ADDI      SP = R0 + 02b0
	state.r[SP] = 0 + 0x02B0;


_04001158:	// 4001158: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_0400115C:	// 400115c: <1480ffff>	BNE       (R0!=A0) --> 115c
	sp_reg_pc = 0x0400115C;
	if (state.r[A0] != 0)
	{
		state.r[A0] = 0;
		goto _0400115C;
	}


_04001160:	// 4001160: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001164:	// 4001164: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_04001168:	// 4001168: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_0400116C:	// 400116c: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		sp_reg_pc = 0x04001170;
		DMARead(state.r[V1]);
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001170:	// 4001170: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001170;
	DMARead(state.r[V1]);


_04001174:	// 4001174: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001178:	// 4001178: <1480ffff>	BNE       (R0!=A0) --> 1178
	sp_reg_pc = 0x04001178;
	if (state.r[A0] != 0)
	{
		state.r[A0] = 0;
		goto _04001178;
	}


_0400117C:	// 400117c: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001180:	// 4001180: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_04001184:	// 4001184: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001188:	// 4001188: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		DMAWrite(state.r[V1]);
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_0400118C:	// 400118c: <40831800>	MTC0      SP write DMA length = V1
	DMAWrite(state.r[V1]);


_04001190:	// 4001190: <3322ffff>	ANDI      V0 = T9 & ffff
	state.r[V0] = (state.r[T9] & 0x0000FFFF);


_04001194:	// 4001194: <4a01086c>	VXOR      vec01 = vec01 xor vec01[<none>]
	rsp_cop2_vxor(&state.v[1], &state.v[1], &state.v[1]);


_04001198:	// 4001198: <3341ffff>	ANDI      AT = K0 & ffff
	state.r[AT] = (state.r[K0] & 0x0000FFFF);


_0400119C:	// 400119c: <202104f0>	ADDI      AT = AT + 04f0
	state.r[AT] = state.r[AT] + 0x04F0;


_040011A0:	// 40011a0: <e8211800>	SDV       [AT + 0x00] = vec01 <00>
	// In function: SmashBros11AC_11B4
	//Call the function: SmashBros11AC_11B4
	SmashBros11AC_11B4();
	goto _040011AC;

_040011A4:	// 40011a4: <e8211801>	SDV       [AT + 0x04] = vec01 <00>
	ErrorToCallFunction(0x040011A4);

_040011A8:	// 40011a8: <2042fff0>	ADDI      V0 = V0 + fff0
	ErrorToCallFunction(0x040011A8);

_040011AC:	// 40011ac: <1c40fffc>	BGTZ      (R0>0) --> 11a0
	sp_reg_pc = 0x040011AC;
	if((_s32)state.r[V0] > 0)
	{
		state.r[AT] = state.r[AT] + 0x0010;
		goto _040011A0;
	}


_040011B0:	// 40011b0: <20210010>	ADDI      AT = AT + 0010
	state.r[AT] = state.r[AT] + 0x0010;


_040011B4:	// 40011b4: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_040011B8:	// 40011b8: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_040011BC:	// 40011bc: <001a1a00>	SLL       V1 = K0 << 8
	state.r[V1] = ((_u32)state.r[K0] << 8);


_040011C0:	// 40011c0: <00031d02>	SRL       V1 = V1 >> 20
	state.r[V1] = ((_u32)state.r[V1] >> 20);


_040011C4:	// 40011c4: <1060ffc8>	BEQ       (R0==V1) --> 10e8
	sp_reg_pc = 0x040011C4;
	if (state.r[V1] == 0)
	{
		state.r[S8] = state.r[S8] + 0xFFFFFFF8;
		goto _040010E8;
	}


_040011C8:	// 40011c8: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_040011CC:	// 40011cc: <33410fff>	ANDI      AT = K0 & 0fff
	state.r[AT] = (state.r[K0] & 0x00000FFF);


_040011D0:	// 40011d0: <202104f0>	ADDI      AT = AT + 04f0
	state.r[AT] = state.r[AT] + 0x04F0;


_040011D4:	// 40011d4: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_040011D8:	// 40011d8: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_040011DC:	// 40011dc: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_040011E0:	// 40011e0: <0d000456>	JAL	    04001158
	state.r[V0] = state.r[V0] + 0x0000;
	sp_reg_pc = 0x040011E0;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001158;


_040011E4:	// 40011e4: <20420000>	ADDI      V0 = V0 + 0000
	state.r[V0] = state.r[V0] + 0x0000;


_040011E8:	// 40011e8: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_040011EC:	// 40011ec: <1420ffff>	BNE       (R0!=AT) --> 11ec
	sp_reg_pc = 0x040011EC;
	if (state.r[AT] != 0)
	{
		state.r[AT] = 0;
		goto _040011EC;
	}


_040011F0:	// 40011f0: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_040011F4:	// 40011f4: <0900043a>	J         040010e8
	goto _040010E8;


_040011F8:	// 40011f8: <00000000>	NOP       


_040011FC:	// 40011fc: <001a1a00>	SLL       V1 = K0 << 8
	state.r[V1] = ((_u32)state.r[K0] << 8);


_04001200:	// 4001200: <00031d02>	SRL       V1 = V1 >> 20
	state.r[V1] = ((_u32)state.r[V1] >> 20);


_04001204:	// 4001204: <1060ffb8>	BEQ       (R0==V1) --> 10e8
	sp_reg_pc = 0x04001204;
	if (state.r[V1] == 0)
	{
		state.r[S8] = state.r[S8] + 0xFFFFFFF8;
		goto _040010E8;
	}


_04001208:	// 4001208: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_0400120C:	// 400120c: <33410fff>	ANDI      AT = K0 & 0fff
	state.r[AT] = (state.r[K0] & 0x00000FFF);


_04001210:	// 4001210: <202104f0>	ADDI      AT = AT + 04f0
	state.r[AT] = state.r[AT] + 0x04F0;


_04001214:	// 4001214: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001218:	// 4001218: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_0400121C:	// 400121c: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001220:	// 4001220: <0d00045d>	JAL	    04001174
	state.r[V0] = state.r[V0] + 0x0000;
	sp_reg_pc = 0x04001220;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001174;


_04001224:	// 4001224: <20420000>	ADDI      V0 = V0 + 0000
	state.r[V0] = state.r[V0] + 0x0000;


_04001228:	// 4001228: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_0400122C:	// 400122c: <1420ffff>	BNE       (R0!=AT) --> 122c
	sp_reg_pc = 0x0400122C;
	if (state.r[AT] != 0)
	{
		state.r[AT] = 0;
		goto _0400122C;
	}


_04001230:	// 4001230: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_04001234:	// 4001234: <0900043a>	J         040010e8
	goto _040010E8;


_04001238:	// 4001238: <00000000>	NOP       


_0400123C:	// 400123c: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001240:	// 4001240: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001244:	// 4001244: <20420000>	ADDI      V0 = V0 + 0000
	state.r[V0] = state.r[V0] + 0x0000;


_04001248:	// 4001248: <200103f0>	ADDI      AT = R0 + 03f0
	state.r[AT] = 0 + 0x03F0;


_0400124C:	// 400124c: <3343ffff>	ANDI      V1 = K0 & ffff
	state.r[V1] = (state.r[K0] & 0x0000FFFF);


_04001250:	// 4001250: <0d000456>	JAL	    04001158
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x04001250;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001158;


_04001254:	// 4001254: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001258:	// 4001258: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_0400125C:	// 400125c: <1420ffff>	BNE       (R0!=AT) --> 125c
	sp_reg_pc = 0x0400125C;
	if (state.r[AT] != 0)
	{
		state.r[AT] = 0;
		goto _0400125C;
	}


_04001260:	// 4001260: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_04001264:	// 4001264: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_04001268:	// 4001268: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_0400126C:	// 400126c: <001a1c02>	SRL       V1 = K0 >> 16
	state.r[V1] = ((_u32)state.r[K0] >> 16);


_04001270:	// 4001270: <30610004>	ANDI      AT = V1 & 0004
	state.r[AT] = (state.r[V1] & 0x00000004);


_04001274:	// 4001274: <1020000d>	BEQ       (R0==AT) --> 12ac
	sp_reg_pc = 0x04001274;
	if (state.r[AT] == 0)
	{
		state.r[AT] = (state.r[V1] & 0x00000002);
		goto _040012AC;
	}


_04001278:	// 4001278: <30610002>	ANDI      AT = V1 & 0002
	state.r[AT] = (state.r[V1] & 0x00000002);


_0400127C:	// 400127c: <10200006>	BEQ       (R0==AT) --> 1298
	sp_reg_pc = 0x0400127C;
	if (state.r[AT] == 0)
	{
		state.r[V0] = ((_u32)state.r[T9] >> 16);
		goto _04001298;
	}


_04001280:	// 4001280: <00191402>	SRL       V0 = T9 >> 16
	state.r[V0] = ((_u32)state.r[T9] >> 16);


_04001284:	// 4001284: <a71a0050>	SH        [T8+0050] = K0
	Save16_DMEM((_u16)state.r[K0], (state.r[T8] + 0x00000050) & 0xfff);


_04001288:	// 4001288: <a702004c>	SH        [T8+004c] = V0
	Save16_DMEM((_u16)state.r[V0], (state.r[T8] + 0x0000004C) & 0xfff);


_0400128C:	// 400128c: <a719004e>	SH        [T8+004e] = T9
	Save16_DMEM((_u16)state.r[T9], (state.r[T8] + 0x0000004E) & 0xfff);


_04001290:	// 4001290: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_04001294:	// 4001294: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001298:	// 4001298: <a71a0046>	SH        [T8+0046] = K0
	Save16_DMEM((_u16)state.r[K0], (state.r[T8] + 0x00000046) & 0xfff);


_0400129C:	// 400129c: <a7020048>	SH        [T8+0048] = V0
	Save16_DMEM((_u16)state.r[V0], (state.r[T8] + 0x00000048) & 0xfff);


_040012A0:	// 40012a0: <a719004a>	SH        [T8+004a] = T9
	Save16_DMEM((_u16)state.r[T9], (state.r[T8] + 0x0000004A) & 0xfff);


_040012A4:	// 40012a4: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_040012A8:	// 40012a8: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_040012AC:	// 40012ac: <00191402>	SRL       V0 = T9 >> 16
	state.r[V0] = ((_u32)state.r[T9] >> 16);


_040012B0:	// 40012b0: <a71a0040>	SH        [T8+0040] = K0
	Save16_DMEM((_u16)state.r[K0], (state.r[T8] + 0x00000040) & 0xfff);


_040012B4:	// 40012b4: <a7020042>	SH        [T8+0042] = V0
	Save16_DMEM((_u16)state.r[V0], (state.r[T8] + 0x00000042) & 0xfff);


_040012B8:	// 40012b8: <a7190044>	SH        [T8+0044] = T9
	Save16_DMEM((_u16)state.r[T9], (state.r[T8] + 0x00000044) & 0xfff);


_040012BC:	// 40012bc: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_040012C0:	// 40012c0: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_040012C4:	// 40012c4: <20010170>	ADDI      AT = R0 + 0170
	state.r[AT] = 0 + 0x0170;


_040012C8:	// 40012c8: <200404f0>	ADDI      A0 = R0 + 04f0
	state.r[A0] = 0 + 0x04F0;


_040012CC:	// 40012cc: <200209d0>	ADDI      V0 = R0 + 09d0
	state.r[V0] = 0 + 0x09D0;


_040012D0:	// 40012d0: <20030b40>	ADDI      V1 = R0 + 0b40
	state.r[V1] = 0 + 0x0B40;


_040012D4:	// 40012d4: <c8412000>	LQV       vec01 <00> = [V0 + 0x00]
	// In function: SmashBros12E4_1334
	//Call the function: SmashBros12E4_1334
	SmashBros12E4_1334();
	goto _04001328;

_040012D8:	// 40012d8: <c8622000>	LQV       vec02 <00> = [V1 + 0x00]
	ErrorToCallFunction(0x040012D8);

_040012DC:	// 40012dc: <2021fff0>	ADDI      AT = AT + fff0
	ErrorToCallFunction(0x040012DC);

_040012E0:	// 40012e0: <20420010>	ADDI      V0 = V0 + 0010
	ErrorToCallFunction(0x040012E0);

_040012E4:	// 40012e4: <20630010>	ADDI      V1 = V1 + 0010
	ErrorToCallFunction(0x040012E4);

_040012E8:	// 40012e8: <e8810800>	SSV       [A0 + 0x00] = vec01 <00>
	ErrorToCallFunction(0x040012E8);

_040012EC:	// 40012ec: <e8820801>	SSV       [A0 + 0x02] = vec02 <00>
	ErrorToCallFunction(0x040012EC);

_040012F0:	// 40012f0: <e8810902>	SSV       [A0 + 0x04] = vec01 <02>
	ErrorToCallFunction(0x040012F0);

_040012F4:	// 40012f4: <e8820903>	SSV       [A0 + 0x06] = vec02 <02>
	ErrorToCallFunction(0x040012F4);

_040012F8:	// 40012f8: <e8810a04>	SSV       [A0 + 0x08] = vec01 <04>
	ErrorToCallFunction(0x040012F8);

_040012FC:	// 40012fc: <e8820a05>	SSV       [A0 + 0x0a] = vec02 <04>
	ErrorToCallFunction(0x040012FC);

_04001300:	// 4001300: <e8810b06>	SSV       [A0 + 0x0c] = vec01 <06>
	ErrorToCallFunction(0x04001300);

_04001304:	// 4001304: <e8820b07>	SSV       [A0 + 0x0e] = vec02 <06>
	ErrorToCallFunction(0x04001304);

_04001308:	// 4001308: <e8810c08>	SSV       [A0 + 0x10] = vec01 <08>
	ErrorToCallFunction(0x04001308);

_0400130C:	// 400130c: <e8820c09>	SSV       [A0 + 0x12] = vec02 <08>
	ErrorToCallFunction(0x0400130C);

_04001310:	// 4001310: <e8810d0a>	SSV       [A0 + 0x14] = vec01 <10>
	ErrorToCallFunction(0x04001310);

_04001314:	// 4001314: <e8820d0b>	SSV       [A0 + 0x16] = vec02 <10>
	ErrorToCallFunction(0x04001314);

_04001318:	// 4001318: <e8810e0c>	SSV       [A0 + 0x18] = vec01 <12>
	ErrorToCallFunction(0x04001318);

_0400131C:	// 400131c: <e8820e0d>	SSV       [A0 + 0x1a] = vec02 <12>
	ErrorToCallFunction(0x0400131C);

_04001320:	// 4001320: <e8810f0e>	SSV       [A0 + 0x1c] = vec01 <14>
	ErrorToCallFunction(0x04001320);

_04001324:	// 4001324: <e8820f0f>	SSV       [A0 + 0x1e] = vec02 <14>
	ErrorToCallFunction(0x04001324);

_04001328:	// 4001328: <1c20ffea>	BGTZ      (R0>0) --> 12d4
	sp_reg_pc = 0x04001328;
	if((_s32)state.r[AT] > 0)
	{
		state.r[A0] = state.r[A0] + 0x0020;
		goto _040012D4;
	}


_0400132C:	// 400132c: <20840020>	ADDI      A0 = A0 + 0020
	state.r[A0] = state.r[A0] + 0x0020;


_04001330:	// 4001330: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_04001334:	// 4001334: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001338:	// 4001338: <3321ffff>	ANDI      AT = T9 & ffff
	state.r[AT] = (state.r[T9] & 0x0000FFFF);


_0400133C:	// 400133c: <3342ffff>	ANDI      V0 = K0 & ffff
	state.r[V0] = (state.r[K0] & 0x0000FFFF);


_04001340:	// 4001340: <204204f0>	ADDI      V0 = V0 + 04f0
	state.r[V0] = state.r[V0] + 0x04F0;


_04001344:	// 4001344: <00191c02>	SRL       V1 = T9 >> 16
	state.r[V1] = ((_u32)state.r[T9] >> 16);


_04001348:	// 4001348: <206304f0>	ADDI      V1 = V1 + 04f0
	state.r[V1] = state.r[V1] + 0x04F0;


_0400134C:	// 400134c: <c8411800>	LDV       vec01 <00> = [V0 + 0x00]
	// In function: Mario1424_1438
	//Call the function: Mario1424_1438
	Mario1424_1438();
	goto _04001364;

_04001350:	// 4001350: <c8421801>	LDV       vec02 <00> = [V0 + 0x08]
	ErrorToCallFunction(0x04001350);

_04001354:	// 4001354: <2021fff0>	ADDI      AT = AT + fff0
	ErrorToCallFunction(0x04001354);

_04001358:	// 4001358: <20420010>	ADDI      V0 = V0 + 0010
	ErrorToCallFunction(0x04001358);

_0400135C:	// 400135c: <e8611800>	SDV       [V1 + 0x00] = vec01 <00>
	ErrorToCallFunction(0x0400135C);

_04001360:	// 4001360: <e8621801>	SDV       [V1 + 0x04] = vec02 <00>
	ErrorToCallFunction(0x04001360);

_04001364:	// 4001364: <1c20fff9>	BGTZ      (R0>0) --> 134c
	sp_reg_pc = 0x04001364;
	if((_s32)state.r[AT] > 0)
	{
		state.r[V1] = state.r[V1] + 0x0010;
		goto _0400134C;
	}


_04001368:	// 4001368: <20630010>	ADDI      V1 = V1 + 0010
	state.r[V1] = state.r[V1] + 0x0010;


_0400136C:	// 400136c: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_04001370:	// 4001370: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001374:	// 4001374: <00190a00>	SLL       AT = T9 << 8
	state.r[AT] = ((_u32)state.r[T9] << 8);


_04001378:	// 4001378: <00010a02>	SRL       AT = AT >> 8
	state.r[AT] = ((_u32)state.r[AT] >> 8);


_0400137C:	// 400137c: <20210000>	ADDI      AT = AT + 0000
	state.r[AT] = state.r[AT] + 0x0000;


_04001380:	// 4001380: <ac01000e>	SW        [R0+000e] = AT
	Save32_DMEM((_u32)state.r[AT], (0 + 0x0000000E) & 0xfff);


_04001384:	// 4001384: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_04001388:	// 4001388: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_0400138C:	// 400138c: <c81f2005>	LQV       vec31 <00> = [R0 + 0x50]
	{
		_u32 addr = (0x00000050 + 0);
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


_04001390:	// 4001390: <0019bb02>	SRL       S7 = T9 >> 12
	state.r[S7] = ((_u32)state.r[T9] >> 12);


_04001394:	// 4001394: <4a19ce6c>	VXOR      vec25 = vec25 xor vec25[<none>]
	rsp_cop2_vxor(&state.v[25], &state.v[25], &state.v[25]);


_04001398:	// 4001398: <32f7000f>	ANDI      S7 = S7 & 000f
	state.r[S7] = (state.r[S7] & 0x0000000F);


_0400139C:	// 400139c: <4a18c62c>	VXOR      vec24 = vec24 xor vec24[<none>]
	rsp_cop2_vxor(&state.v[24], &state.v[24], &state.v[24]);


_040013A0:	// 40013a0: <22f704f0>	ADDI      S7 = S7 + 04f0
	state.r[S7] = state.r[S7] + 0x04F0;


_040013A4:	// 40013a4: <4a0d6b6c>	VXOR      vec13 = vec13 xor vec13[<none>]
	rsp_cop2_vxor(&state.v[13], &state.v[13], &state.v[13]);


_040013A8:	// 40013a8: <33210fff>	ANDI      AT = T9 & 0fff
	state.r[AT] = (state.r[T9] & 0x00000FFF);


_040013AC:	// 40013ac: <4a0e73ac>	VXOR      vec14 = vec14 xor vec14[<none>]
	rsp_cop2_vxor(&state.v[14], &state.v[14], &state.v[14]);


_040013B0:	// 40013b0: <202104f0>	ADDI      AT = AT + 04f0
	state.r[AT] = state.r[AT] + 0x04F0;


_040013B4:	// 40013b4: <4a0f7bec>	VXOR      vec15 = vec15 xor vec15[<none>]
	rsp_cop2_vxor(&state.v[15], &state.v[15], &state.v[15]);


_040013B8:	// 40013b8: <0019ac02>	SRL       S5 = T9 >> 16
	state.r[S5] = ((_u32)state.r[T9] >> 16);


_040013BC:	// 40013bc: <4a10842c>	VXOR      vec16 = vec16 xor vec16[<none>]
	rsp_cop2_vxor(&state.v[16], &state.v[16], &state.v[16]);


_040013C0:	// 40013c0: <32b50fff>	ANDI      S5 = S5 & 0fff
	state.r[S5] = (state.r[S5] & 0x00000FFF);


_040013C4:	// 40013c4: <4a118c6c>	VXOR      vec17 = vec17 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[17], &state.v[17]);


_040013C8:	// 40013c8: <001aa200>	SLL       S4 = K0 << 8
	state.r[S4] = ((_u32)state.r[K0] << 8);


_040013CC:	// 40013cc: <4a1294ac>	VXOR      vec18 = vec18 xor vec18[<none>]
	rsp_cop2_vxor(&state.v[18], &state.v[18], &state.v[18]);


_040013D0:	// 40013d0: <0014a202>	SRL       S4 = S4 >> 8
	state.r[S4] = ((_u32)state.r[S4] >> 8);


_040013D4:	// 40013d4: <4a139cec>	VXOR      vec19 = vec19 xor vec19[<none>]
	rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);


_040013D8:	// 40013d8: <2003001f>	ADDI      V1 = R0 + 001f
	state.r[V1] = 0 + 0x001F;


_040013DC:	// 40013dc: <00196f02>	SRL       T5 = T9 >> 28
	state.r[T5] = ((_u32)state.r[T9] >> 28);


_040013E0:	// 40013e0: <31a20001>	ANDI      V0 = T5 & 0001
	state.r[V0] = (state.r[T5] & 0x00000001);


_040013E4:	// 40013e4: <1c400017>	BGTZ      (R0>0) --> 1444
	sp_reg_pc = 0x040013E4;
	if((_s32)state.r[V0] > 0)
	{
		state.r[S6] = state.r[S7] + 0x0001;
		goto _04001444;
	}


_040013E8:	// 40013e8: <22f60001>	ADDI      S6 = S7 + 0001
	state.r[S6] = state.r[S7] + 0x0001;


_040013EC:	// 40013ec: <31a20002>	ANDI      V0 = T5 & 0002
	state.r[V0] = (state.r[T5] & 0x00000002);


_040013F0:	// 40013f0: <10400002>	BEQ       (R0==V0) --> 13fc
	sp_reg_pc = 0x040013F0;
	if (state.r[V0] == 0)
	{
		state.r[V0] = state.r[S4] + 0x0000;
		goto _040013FC;
	}


_040013F4:	// 40013f4: <22820000>	ADDI      V0 = S4 + 0000
	state.r[V0] = state.r[S4] + 0x0000;


_040013F8:	// 40013f8: <8c02000e>	LW        V0 = [R0+000e]
	state.r[V0] = (_s32)Load32_DMEM((0 + 0x0000000E) & 0xfff);


_040013FC:	// 40013fc: <400d2800>	MFC0      A1 = DP buffer busy counter
	state.r[T5] = 0;


_04001400:	// 4001400: <15a0ffff>	BNE       (R0!=T5) --> 1400
	sp_reg_pc = 0x04001400;
	if (state.r[T5] != 0)
	{
		state.r[T5] = 0;
		goto _04001400;
	}


_04001404:	// 4001404: <400d2800>	MFC0      A1 = DP buffer busy counter
	state.r[T5] = 0;


_04001408:	// 4001408: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_0400140C:	// 400140c: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001410:	// 4001410: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001410;
	DMARead(state.r[V1]);


_04001414:	// 4001414: <20130020>	ADDI      S3 = R0 + 0020
	state.r[S3] = 0 + 0x0020;


_04001418:	// 4001418: <201203f0>	ADDI      S2 = R0 + 03f0
	state.r[S2] = 0 + 0x03F0;


_0400141C:	// 400141c: <ca791800>	LDV       vec25 <00> = [S3 + 0x00]
	rsp_ldv_compiler((state.r[S3] & 0xFFF), 25, 8);


_04001420:	// 4001420: <ca781c00>	LDV       vec24 <08> = [S3 + 0x00]
	rsp_ldv_compiler((state.r[S3] & 0xFFF), 24, 0);


_04001424:	// 4001424: <ca771801>	LDV       vec23 <00> = [S3 + 0x08]
	rsp_ldv_compiler((state.r[S3] + 0x00000008) & 0xFFF, 23, 8);


_04001428:	// 4001428: <ca771c01>	LDV       vec23 <08> = [S3 + 0x08]
	rsp_ldv_compiler((state.r[S3] + 0x00000008) & 0xFFF, 23, 0);


_0400142C:	// 400142c: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001430:	// 4001430: <14a0ffff>	BNE       (R0!=A1) --> 1430
	sp_reg_pc = 0x04001430;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001430;
	}


_04001434:	// 4001434: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001438:	// 4001438: <00000020>	ADD       R0 = R0+R0
	; //0 = (0 + 0);



_0400143C:	// 400143c: <0900051a>	J         04001468
	{
		_u32 addr = (0x00000010 + state.r[AT]);
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
	goto _04001468;


_04001440:	// 4001440: <c83b2001>	LQV       vec27 <00> = [AT + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[AT]);
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


_04001444:	// 4001444: <20130020>	ADDI      S3 = R0 + 0020
	state.r[S3] = 0 + 0x0020;


_04001448:	// 4001448: <4a1bdeec>	VXOR      vec27 = vec27 xor vec27[<none>]
	rsp_cop2_vxor(&state.v[27], &state.v[27], &state.v[27]);


_0400144C:	// 400144c: <201203f0>	ADDI      S2 = R0 + 03f0
	state.r[S2] = 0 + 0x03F0;


_04001450:	// 4001450: <ca791800>	LDV       vec25 <00> = [S3 + 0x00]
	rsp_ldv_compiler((state.r[S3] & 0xFFF), 25, 8);


_04001454:	// 4001454: <ca781c00>	LDV       vec24 <08> = [S3 + 0x00]
	rsp_ldv_compiler((state.r[S3] & 0xFFF), 24, 0);


_04001458:	// 4001458: <ca771801>	LDV       vec23 <00> = [S3 + 0x08]
	rsp_ldv_compiler((state.r[S3] + 0x00000008) & 0xFFF, 23, 8);


_0400145C:	// 400145c: <ca771c01>	LDV       vec23 <08> = [S3 + 0x08]
	rsp_ldv_compiler((state.r[S3] + 0x00000008) & 0xFFF, 23, 0);


_04001460:	// 4001460: <e83b2000>	SQV       [AT + 0x00] = vec27 <00>
	{
		_u32    addr = (0x00000000 + state.r[AT]);
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


_04001464:	// 4001464: <e83b2001>	SQV       [AT + 0x10] = vec27 <00>
	{
		_u32    addr = (0x00000010 + state.r[AT]);
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


_04001468:	// 4001468: <12a0006b>	BEQ       (R0==S5) --> 1618
	sp_reg_pc = 0x04001468;
	if (state.r[S5] == 0)
	{
		state.r[AT] = state.r[AT] + 0x0020;
		goto _04001618;
	}


_0400146C:	// 400146c: <20210020>	ADDI      AT = AT + 0020
	state.r[AT] = state.r[AT] + 0x0020;


_04001470:	// 4001470: <cacc1800>	LDV       vec12 <00> = [S6 + 0x00]
	rsp_ldv_compiler((state.r[S6] & 0xFFF), 12, 8);


_04001474:	// 4001474: <92ea0000>	LBU       T2 = [S7+0000]
	state.r[T2] = (_s32)(_u8)pDMEM[((state.r[S7] + 0x00000000) & 0xfff) ^ 3];


_04001478:	// 4001478: <200d000c>	ADDI      T5 = R0 + 000c
	state.r[T5] = 0 + 0x000C;


_0400147C:	// 400147c: <200c0001>	ADDI      T4 = R0 + 0001
	state.r[T4] = 0 + 0x0001;


_04001480:	// 4001480: <314e000f>	ANDI      T6 = T2 & 000f
	state.r[T6] = (state.r[T2] & 0x0000000F);


_04001484:	// 4001484: <000e7140>	SLL       T6 = T6 << 5
	state.r[T6] = ((_u32)state.r[T6] << 5);


_04001488:	// 4001488: <4b0ccaa8>	VAND      vec10 = vec25 and vec12[0]
	rsp_cop2_vand_element(&state.v[10], &state.v[25], &state.v[12].U16[7]);


_0400148C:	// 400148c: <01d28020>	ADD       S0 = T6+S2
	state.r[S0] = (state.r[T6] + state.r[S2]);


_04001490:	// 4001490: <4b2cc268>	VAND      vec09 = vec24 and vec12[1]
	rsp_cop2_vand_element(&state.v[9], &state.v[24], &state.v[12].U16[6]);


_04001494:	// 4001494: <000a8902>	SRL       S1 = T2 >> 4
	state.r[S1] = ((_u32)state.r[T2] >> 4);


_04001498:	// 4001498: <4b4cca28>	VAND      vec08 = vec25 and vec12[2]
	rsp_cop2_vand_element(&state.v[8], &state.v[25], &state.v[12].U16[5]);


_0400149C:	// 400149c: <01b18822>	SUB       S1 = T5-S1
	state.r[S1] = (state.r[T5] - state.r[S1]);


_040014A0:	// 40014a0: <4b6cc1e8>	VAND      vec07 = vec24 and vec12[3]
	rsp_cop2_vand_element(&state.v[7], &state.v[24], &state.v[12].U16[4]);


_040014A4:	// 40014a4: <222dffff>	ADDI      T5 = S1 + ffff
	state.r[T5] = state.r[S1] + 0xFFFFFFFF;


_040014A8:	// 40014a8: <000c63c0>	SLL       T4 = T4 << 15
	state.r[T4] = ((_u32)state.r[T4] << 15);


_040014AC:	// 40014ac: <01ac5806>	SRLV      T4 = T3 >> T5
	state.r[T3] = ((_u32)state.r[T4] >> (state.r[T5] & 0x1F));


_040014B0:	// 40014b0: <488bb000>	MTC2      vec22 <00> = T3
	state.v[22].U16[7] = (_u16)state.r[T3];


_040014B4:	// 40014b4: <ca152000>	LQV       vec21 <00> = [S0 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S0]);
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


_040014B8:	// 40014b8: <ca142001>	LQV       vec20 <00> = [S0 + 0x10]
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


_040014BC:	// 40014bc: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_040014C0:	// 40014c0: <ca132802>	LRV       vec19 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[19].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014C4:	// 40014c4: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_040014C8:	// 40014c8: <ca122802>	LRV       vec18 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[18].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014CC:	// 40014cc: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_040014D0:	// 40014d0: <ca112802>	LRV       vec17 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[17].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014D4:	// 40014d4: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_040014D8:	// 40014d8: <ca102802>	LRV       vec16 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[16].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014DC:	// 40014dc: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_040014E0:	// 40014e0: <ca0f2802>	LRV       vec15 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[15].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014E4:	// 40014e4: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_040014E8:	// 40014e8: <ca0e2802>	LRV       vec14 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[14].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014EC:	// 40014ec: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_040014F0:	// 40014f0: <ca0d2802>	LRV       vec13 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[13].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040014F4:	// 40014f4: <22d60009>	ADDI      S6 = S6 + 0009
	// In function: SmashBros1514_1630
	//Call the function: SmashBros1514_1630
	SmashBros1514_1630();
	goto _04001618;

_040014F8:	// 40014f8: <4a175786>	VMUDN     vec30 = ( acc = vec10 * vec23[<none>]      ) >> 16
	ErrorToCallFunction(0x040014F8);

_040014FC:	// 40014fc: <22f70009>	ADDI      S7 = S7 + 0009
	ErrorToCallFunction(0x040014FC);

_04001500:	// 4001500: <4a174f8e>	VMADN     vec30 = ( acc+= vec09 * vec23[<none>] )
	ErrorToCallFunction(0x04001500);

_04001504:	// 4001504: <92ea0000>	LBU       T2 = [S7+0000]
	ErrorToCallFunction(0x04001504);

_04001508:	// 4001508: <4a174746>	VMUDN     vec29 = ( acc = vec08 * vec23[<none>]      ) >> 16
	ErrorToCallFunction(0x04001508);

_0400150C:	// 400150c: <cacc1800>	LDV       vec12 <00> = [S6 + 0x00]
	ErrorToCallFunction(0x0400150C);

_04001510:	// 4001510: <4a173f4e>	VMADN     vec29 = ( acc+= vec07 * vec23[<none>] )
	ErrorToCallFunction(0x04001510);

_04001514:	// 4001514: <200d000c>	ADDI      T5 = R0 + 000c
	ErrorToCallFunction(0x04001514);

_04001518:	// 4001518: <1a200003>	BLEZ      (R0<=0) --> 1528
	ErrorToCallFunction(0x04001518);

_0400151C:	// 400151c: <314e000f>	ANDI      T6 = T2 & 000f
	ErrorToCallFunction(0x0400151C);

_04001520:	// 4001520: <4b16f785>	VMUDM     vec30 = ( acc = vec30 * vec22[0] ) >> 16
	ErrorToCallFunction(0x04001520);

_04001524:	// 4001524: <4b16ef45>	VMUDM     vec29 = ( acc = vec29 * vec22[0] ) >> 16
	ErrorToCallFunction(0x04001524);

_04001528:	// 4001528: <000e7140>	SLL       T6 = T6 << 5
	ErrorToCallFunction(0x04001528);

_0400152C:	// 400152c: <4bdbaac7>	VMUDH     vec11 = ( acc = (vec21 * vec27[6]) << 16) >> 16
	ErrorToCallFunction(0x0400152C);

_04001530:	// 4001530: <01d28020>	ADD       S0 = T6+S2
	ErrorToCallFunction(0x04001530);

_04001534:	// 4001534: <4bfba2cf>	VMADH     vec11 = ( acc+= (vec20 * vec27[7]) << 16) >> 16
	ErrorToCallFunction(0x04001534);

_04001538:	// 4001538: <4b1e9acf>	VMADH     vec11 = ( acc+= (vec19 * vec30[0]) << 16) >> 16
	ErrorToCallFunction(0x04001538);

_0400153C:	// 400153c: <4b3e92cf>	VMADH     vec11 = ( acc+= (vec18 * vec30[1]) << 16) >> 16
	ErrorToCallFunction(0x0400153C);

_04001540:	// 4001540: <000a8902>	SRL       S1 = T2 >> 4
	ErrorToCallFunction(0x04001540);

_04001544:	// 4001544: <4b5e8acf>	VMADH     vec11 = ( acc+= (vec17 * vec30[2]) << 16) >> 16
	ErrorToCallFunction(0x04001544);

_04001548:	// 4001548: <4b7e82cf>	VMADH     vec11 = ( acc+= (vec16 * vec30[3]) << 16) >> 16
	ErrorToCallFunction(0x04001548);

_0400154C:	// 400154c: <01b18822>	SUB       S1 = T5-S1
	ErrorToCallFunction(0x0400154C);

_04001550:	// 4001550: <4b9e7f0f>	VMADH     vec28 = ( acc+= (vec15 * vec30[4]) << 16) >> 16
	ErrorToCallFunction(0x04001550);

_04001554:	// 4001554: <222dffff>	ADDI      T5 = S1 + ffff
	ErrorToCallFunction(0x04001554);

_04001558:	// 4001558: <4bbe72cf>	VMADH     vec11 = ( acc+= (vec14 * vec30[5]) << 16) >> 16
	ErrorToCallFunction(0x04001558);

_0400155C:	// 400155c: <4bde6acf>	VMADH     vec11 = ( acc+= (vec13 * vec30[6]) << 16) >> 16
	ErrorToCallFunction(0x0400155C);

_04001560:	// 4001560: <4b7ff2cf>	VMADH     vec11 = ( acc+= (vec30 * vec31[3]) << 16) >> 16
	ErrorToCallFunction(0x04001560);

_04001564:	// 4001564: <01ac5806>	SRLV      T4 = T3 >> T5
	ErrorToCallFunction(0x04001564);

_04001568:	// 4001568: <4b3c369d>	VSAW $v26, $v6, $v28[[1]]
	ErrorToCallFunction(0x04001568);

_0400156C:	// 400156c: <488bb000>	MTC2      vec22 <00> = T3
	ErrorToCallFunction(0x0400156C);

_04001570:	// 4001570: <4b1c371d>	VSAW $v28, $v6, $v28[[0]]
	ErrorToCallFunction(0x04001570);

_04001574:	// 4001574: <4b0ccaa8>	VAND      vec10 = vec25 and vec12[0]
	ErrorToCallFunction(0x04001574);

_04001578:	// 4001578: <4b2cc268>	VAND      vec09 = vec24 and vec12[1]
	ErrorToCallFunction(0x04001578);

_0400157C:	// 400157c: <4b4cca28>	VAND      vec08 = vec25 and vec12[2]
	ErrorToCallFunction(0x0400157C);

_04001580:	// 4001580: <4b6cc1e8>	VAND      vec07 = vec24 and vec12[3]
	ErrorToCallFunction(0x04001580);

_04001584:	// 4001584: <4b3fd2c6>	VMUDN     vec11 = ( acc = vec26 * vec31[1]      ) >> 16
	ErrorToCallFunction(0x04001584);

_04001588:	// 4001588: <4b3fe70f>	VMADH     vec28 = ( acc+= (vec28 * vec31[1]) << 16) >> 16
	ErrorToCallFunction(0x04001588);

_0400158C:	// 400158c: <4b1d9ac7>	VMUDH     vec11 = ( acc = (vec19 * vec29[0]) << 16) >> 16
	ErrorToCallFunction(0x0400158C);

_04001590:	// 4001590: <220ffffe>	ADDI      T7 = S0 + fffe
	ErrorToCallFunction(0x04001590);

_04001594:	// 4001594: <4b3d92cf>	VMADH     vec11 = ( acc+= (vec18 * vec29[1]) << 16) >> 16
	ErrorToCallFunction(0x04001594);

_04001598:	// 4001598: <c9f32802>	LRV       vec19 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x04001598);

_0400159C:	// 400159c: <4b5d8acf>	VMADH     vec11 = ( acc+= (vec17 * vec29[2]) << 16) >> 16
	ErrorToCallFunction(0x0400159C);

_040015A0:	// 40015a0: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x040015A0);

_040015A4:	// 40015a4: <4b7d82cf>	VMADH     vec11 = ( acc+= (vec16 * vec29[3]) << 16) >> 16
	ErrorToCallFunction(0x040015A4);

_040015A8:	// 40015a8: <c9f22802>	LRV       vec18 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x040015A8);

_040015AC:	// 40015ac: <4b9d7acf>	VMADH     vec11 = ( acc+= (vec15 * vec29[4]) << 16) >> 16
	ErrorToCallFunction(0x040015AC);

_040015B0:	// 40015b0: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x040015B0);

_040015B4:	// 40015b4: <4bbd72cf>	VMADH     vec11 = ( acc+= (vec14 * vec29[5]) << 16) >> 16
	ErrorToCallFunction(0x040015B4);

_040015B8:	// 40015b8: <c9f12802>	LRV       vec17 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x040015B8);

_040015BC:	// 40015bc: <4bdd6acf>	VMADH     vec11 = ( acc+= (vec13 * vec29[6]) << 16) >> 16
	ErrorToCallFunction(0x040015BC);

_040015C0:	// 40015c0: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x040015C0);

_040015C4:	// 40015c4: <4b7feacf>	VMADH     vec11 = ( acc+= (vec29 * vec31[3]) << 16) >> 16
	ErrorToCallFunction(0x040015C4);

_040015C8:	// 40015c8: <c9f02802>	LRV       vec16 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x040015C8);

_040015CC:	// 40015cc: <4bdcaacf>	VMADH     vec11 = ( acc+= (vec21 * vec28[6]) << 16) >> 16
	ErrorToCallFunction(0x040015CC);

_040015D0:	// 40015d0: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x040015D0);

_040015D4:	// 40015d4: <4bfca2cf>	VMADH     vec11 = ( acc+= (vec20 * vec28[7]) << 16) >> 16
	ErrorToCallFunction(0x040015D4);

_040015D8:	// 40015d8: <c9ef2802>	LRV       vec15 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x040015D8);

_040015DC:	// 40015dc: <4b3b369d>	VSAW $v26, $v6, $v27[[1]]
	ErrorToCallFunction(0x040015DC);

_040015E0:	// 40015e0: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x040015E0);

_040015E4:	// 40015e4: <4b1b36dd>	VSAW $v27, $v6, $v27[[0]]
	ErrorToCallFunction(0x040015E4);

_040015E8:	// 40015e8: <c9ee2802>	LRV       vec14 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x040015E8);

_040015EC:	// 40015ec: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x040015EC);

_040015F0:	// 40015f0: <c9ed2802>	LRV       vec13 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x040015F0);

_040015F4:	// 40015f4: <ca152000>	LQV       vec21 <00> = [S0 + 0x00]
	ErrorToCallFunction(0x040015F4);

_040015F8:	// 40015f8: <4b3fd2c6>	VMUDN     vec11 = ( acc = vec26 * vec31[1]      ) >> 16
	ErrorToCallFunction(0x040015F8);

_040015FC:	// 40015fc: <ca142001>	LQV       vec20 <00> = [S0 + 0x10]
	ErrorToCallFunction(0x040015FC);

_04001600:	// 4001600: <4b3fdecf>	VMADH     vec27 = ( acc+= (vec27 * vec31[1]) << 16) >> 16
	ErrorToCallFunction(0x04001600);

_04001604:	// 4001604: <22b5ffe0>	ADDI      S5 = S5 + ffe0
	ErrorToCallFunction(0x04001604);

_04001608:	// 4001608: <e83c2000>	SQV       [AT + 0x00] = vec28 <00>
	ErrorToCallFunction(0x04001608);

_0400160C:	// 400160c: <20210020>	ADDI      AT = AT + 0020
	ErrorToCallFunction(0x0400160C);

_04001610:	// 4001610: <1ea0ffb8>	BGTZ      (R0>0) --> 14f4
	ErrorToCallFunction(0x04001610);

_04001614:	// 4001614: <e83b207f>	SQV       [AT + 0xf0] = vec27 <00>
	ErrorToCallFunction(0x04001614);

_04001618:	// 4001618: <2021ffe0>	ADDI      AT = AT + ffe0
	state.r[AT] = state.r[AT] + 0xFFFFFFE0;


_0400161C:	// 400161c: <0d00045d>	JAL	    04001174
	state.r[V0] = state.r[S4] + 0x0000;
	sp_reg_pc = 0x0400161C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001174;


_04001620:	// 4001620: <22820000>	ADDI      V0 = S4 + 0000
	state.r[V0] = state.r[S4] + 0x0000;


_04001624:	// 4001624: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001628:	// 4001628: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_0400162C:	// 400162c: <14a0ffff>	BNE       (R0!=A1) --> 162c
	sp_reg_pc = 0x0400162C;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _0400162C;
	}


_04001630:	// 4001630: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001634:	// 4001634: <0900043a>	J         040010e8
	; //0 = (0 & 0);

	goto _040010E8;


_04001638:	// 4001638: <00000024>	AND       R0 = R0 & R0
	; //0 = (0 & 0);



_0400163C:	// 400163c: <00199e02>	SRL       S3 = T9 >> 24
	state.r[S3] = ((_u32)state.r[T9] >> 24);


_04001640:	// 4001640: <201403f0>	ADDI      S4 = R0 + 03f0
	state.r[S4] = 0 + 0x03F0;


_04001644:	// 4001644: <4a15ad6c>	VXOR      vec21 = vec21 xor vec21[<none>]
	rsp_cop2_vxor(&state.v[21], &state.v[21], &state.v[21]);


_04001648:	// 4001648: <12600002>	BEQ       (R0==S3) --> 1654
	sp_reg_pc = 0x04001648;
	if (state.r[S3] == 0)
	{
		state.r[S7] = 0 + 0x04F0;
		goto _04001654;
	}


_0400164C:	// 400164c: <201704f0>	ADDI      S7 = R0 + 04f0
	state.r[S7] = 0 + 0x04F0;


_04001650:	// 4001650: <20170660>	ADDI      S7 = R0 + 0660
	state.r[S7] = 0 + 0x0660;


_04001654:	// 4001654: <ca9c2001>	LQV       vec28 <00> = [S4 + 0x10]
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


_04001658:	// 4001658: <4a16b5ac>	VXOR      vec22 = vec22 xor vec22[<none>]
	rsp_cop2_vxor(&state.v[22], &state.v[22], &state.v[22]);


_0400165C:	// 400165c: <489a9500>	MTC2      vec18 <10> = K0
	state.v[18].U16[2] = (_u16)state.r[K0];


_04001660:	// 4001660: <4a17bdec>	VXOR      vec23 = vec23 xor vec23[<none>]
	rsp_cop2_vxor(&state.v[23], &state.v[23], &state.v[23]);


_04001664:	// 4001664: <001ad080>	SLL       K0 = K0 << 2
	state.r[K0] = ((_u32)state.r[K0] << 2);


_04001668:	// 4001668: <4a18c62c>	VXOR      vec24 = vec24 xor vec24[<none>]
	rsp_cop2_vxor(&state.v[24], &state.v[24], &state.v[24]);


_0400166C:	// 400166c: <489aa000>	MTC2      vec20 <00> = K0
	state.v[20].U16[7] = (_u16)state.r[K0];


_04001670:	// 4001670: <4a19ce6c>	VXOR      vec25 = vec25 xor vec25[<none>]
	rsp_cop2_vxor(&state.v[25], &state.v[25], &state.v[25]);


_04001674:	// 4001674: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001678:	// 4001678: <4a1ad6ac>	VXOR      vec26 = vec26 xor vec26[<none>]
	rsp_cop2_vxor(&state.v[26], &state.v[26], &state.v[26]);


_0400167C:	// 400167c: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001680:	// 4001680: <4a1bdeec>	VXOR      vec27 = vec27 xor vec27[<none>]
	rsp_cop2_vxor(&state.v[27], &state.v[27], &state.v[27]);


_04001684:	// 4001684: <20420000>	ADDI      V0 = V0 + 0000
	state.r[V0] = state.r[V0] + 0x0000;


_04001688:	// 4001688: <20030007>	ADDI      V1 = R0 + 0007
	state.r[V1] = 0 + 0x0007;


_0400168C:	// 400168c: <20130004>	ADDI      S3 = R0 + 0004
	state.r[S3] = 0 + 0x0004;


_04001690:	// 4001690: <48939000>	MTC2      vec18 <00> = S3
	state.v[18].U16[7] = (_u16)state.r[S3];


_04001694:	// 4001694: <20160170>	ADDI      S6 = R0 + 0170
	state.r[S6] = 0 + 0x0170;


_04001698:	// 4001698: <4b14e505>	VMUDM     vec20 = ( acc = vec28 * vec20[0] ) >> 16
	{
		_u16		s2value = state.v[20].U16[7];
		VMUDM_operation(0, state.v[20].U16[0], state.v[28].U16[0], s2value, 1, 1, 1, 1)
		VMUDM_operation(1, state.v[20].U16[1], state.v[28].U16[1], s2value, 1, 1, 1, 1)
		VMUDM_operation(2, state.v[20].U16[2], state.v[28].U16[2], s2value, 1, 1, 1, 1)
		VMUDM_operation(3, state.v[20].U16[3], state.v[28].U16[3], s2value, 1, 1, 1, 1)
		VMUDM_operation(4, state.v[20].U16[4], state.v[28].U16[4], s2value, 1, 1, 1, 1)
		VMUDM_operation(5, state.v[20].U16[5], state.v[28].U16[5], s2value, 1, 1, 1, 1)
		VMUDM_operation(6, state.v[20].U16[6], state.v[28].U16[6], s2value, 1, 1, 1, 1)
		VMUDM_operation(7, state.v[20].U16[7], state.v[28].U16[7], s2value, 1, 1, 1, 1)
	}


_0400169C:	// 400169c: <001a9c82>	SRL       S3 = K0 >> 18
	state.r[S3] = ((_u32)state.r[K0] >> 18);


_040016A0:	// 40016a0: <32730001>	ANDI      S3 = S3 & 0001
	state.r[S3] = (state.r[S3] & 0x00000001);


_040016A4:	// 40016a4: <1e60001b>	BGTZ      (R0>0) --> 1714
	sp_reg_pc = 0x040016A4;
	if((_s32)state.r[S3] > 0)
	{
		{
			_u32    addr = (0x00000010 + state.r[S4]);
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
		goto _04001714;
	}


_040016A8:	// 40016a8: <ea942001>	SQV       [S4 + 0x10] = vec20 <00>
	{
		_u32    addr = (0x00000010 + state.r[S4]);
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


_040016AC:	// 40016ac: <23010000>	ADDI      AT = T8 + 0000
	state.r[AT] = state.r[T8] + 0x0000;


_040016B0:	// 40016b0: <40132800>	MFC0      A1 = unknown
	state.r[S3] = 0;


_040016B4:	// 40016b4: <1660ffff>	BNE       (R0!=S3) --> 16b4
	sp_reg_pc = 0x040016B4;
	if (state.r[S3] != 0)
	{
		state.r[S3] = 0;
		goto _040016B4;
	}


_040016B8:	// 40016b8: <40132800>	MFC0      A1 = unknown
	state.r[S3] = 0;


_040016BC:	// 40016bc: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_040016C0:	// 40016c0: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_040016C4:	// 40016c4: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x040016C4;
	DMARead(state.r[V1]);


_040016C8:	// 40016c8: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040016CC:	// 40016cc: <ca9b2802>	LRV       vec27 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[27].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040016D0:	// 40016d0: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040016D4:	// 40016d4: <ca9a2802>	LRV       vec26 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[26].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040016D8:	// 40016d8: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040016DC:	// 40016dc: <ca992802>	LRV       vec25 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[25].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040016E0:	// 40016e0: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040016E4:	// 40016e4: <ca982802>	LRV       vec24 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[24].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040016E8:	// 40016e8: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040016EC:	// 40016ec: <ca972802>	LRV       vec23 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[23].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040016F0:	// 40016f0: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040016F4:	// 40016f4: <ca962802>	LRV       vec22 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[22].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040016F8:	// 40016f8: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040016FC:	// 40016fc: <ca952802>	LRV       vec21 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[21].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001700:	// 4001700: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001704:	// 4001704: <14a0ffff>	BNE       (R0!=A1) --> 1704
	sp_reg_pc = 0x04001704;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001704;
	}


_04001708:	// 4001708: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_0400170C:	// 400170c: <090005d4>	J         04001750
	rsp_ldv_compiler((state.r[AT] & 0xFFF), 30, 0);
	goto _04001750;


_04001710:	// 4001710: <c83e1c00>	LDV       vec30 <08> = [AT + 0x00]
	rsp_ldv_compiler((state.r[AT] & 0xFFF), 30, 0);


_04001714:	// 4001714: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001718:	// 4001718: <4a1ef7ac>	VXOR      vec30 = vec30 xor vec30[<none>]
	rsp_cop2_vxor(&state.v[30], &state.v[30], &state.v[30]);


_0400171C:	// 400171c: <ca9b2802>	LRV       vec27 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[27].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001720:	// 4001720: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001724:	// 4001724: <ca9a2802>	LRV       vec26 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[26].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001728:	// 4001728: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_0400172C:	// 400172c: <ca992802>	LRV       vec25 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[25].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001730:	// 4001730: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001734:	// 4001734: <ca982802>	LRV       vec24 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[24].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001738:	// 4001738: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_0400173C:	// 400173c: <ca972802>	LRV       vec23 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[23].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001740:	// 4001740: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001744:	// 4001744: <ca962802>	LRV       vec22 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[22].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001748:	// 4001748: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_0400174C:	// 400174c: <ca952802>	LRV       vec21 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[21].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001750:	// 4001750: <caff2000>	LQV       vec31 <00> = [S7 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S7]);
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


_04001754:	// 4001754: <4bfee507>	VMUDH     vec20 = ( acc = (vec28 * vec30[7]) << 16) >> 16
	{
		_u16		s2value = state.v[30].U16[0];
		VMUDH_operation(0, state.v[20].U16[0], state.v[28].U16[0], s2value, 0, 1, 0, 1)
		VMUDH_operation(1, state.v[20].U16[1], state.v[28].U16[1], s2value, 0, 1, 0, 1)
		VMUDH_operation(2, state.v[20].U16[2], state.v[28].U16[2], s2value, 0, 1, 0, 1)
		VMUDH_operation(3, state.v[20].U16[3], state.v[28].U16[3], s2value, 0, 1, 0, 1)
		VMUDH_operation(4, state.v[20].U16[4], state.v[28].U16[4], s2value, 0, 1, 0, 1)
		VMUDH_operation(5, state.v[20].U16[5], state.v[28].U16[5], s2value, 0, 1, 0, 1)
		VMUDH_operation(6, state.v[20].U16[6], state.v[28].U16[6], s2value, 0, 1, 0, 1)
		VMUDH_operation(7, state.v[20].U16[7], state.v[28].U16[7], s2value, 0, 1, 0, 1)
	}


_04001758:	// 4001758: <4b1fdd0f>	VMADH     vec20 = ( acc+= (vec27 * vec31[0]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[7];
		VMADH_operation(0, state.v[20].U16[0], state.v[27].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[27].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[27].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[27].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[27].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[27].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[27].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[27].U16[7], s2value, 0, 1, 0, 1)
	}


_0400175C:	// 400175c: <22d6fff0>	ADDI      S6 = S6 + fff0
	state.r[S6] = state.r[S6] + 0xFFFFFFF0;


_04001760:	// 4001760: <4b3fd50f>	VMADH     vec20 = ( acc+= (vec26 * vec31[1]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[6];
		VMADH_operation(0, state.v[20].U16[0], state.v[26].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[26].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[26].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[26].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[26].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[26].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[26].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[26].U16[7], s2value, 0, 1, 0, 1)
	}


_04001764:	// 4001764: <4b5fcd0f>	VMADH     vec20 = ( acc+= (vec25 * vec31[2]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[5];
		VMADH_operation(0, state.v[20].U16[0], state.v[25].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[25].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[25].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[25].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[25].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[25].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[25].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[25].U16[7], s2value, 0, 1, 0, 1)
	}


_04001768:	// 4001768: <eafe207f>	SQV       [S7 + 0xf0] = vec30 <00>
	{
		_u32    addr = (0xFFFFFFF0 + state.r[S7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[30].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[30].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[30].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[30].U32[3];
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

			value1 |= (state.v[30].U64[0] & temp1);
			value2 |= (state.v[30].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_0400176C:	// 400176c: <4b7fc50f>	VMADH     vec20 = ( acc+= (vec24 * vec31[3]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[4];
		VMADH_operation(0, state.v[20].U16[0], state.v[24].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[24].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[24].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[24].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[24].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[24].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[24].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[24].U16[7], s2value, 0, 1, 0, 1)
	}


_04001770:	// 4001770: <4b9fbf8f>	VMADH     vec30 = ( acc+= (vec23 * vec31[4]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[3];
		VMADH_operation(0, state.v[30].U16[0], state.v[23].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[30].U16[1], state.v[23].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[30].U16[2], state.v[23].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[30].U16[3], state.v[23].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[30].U16[4], state.v[23].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[30].U16[5], state.v[23].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[30].U16[6], state.v[23].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[30].U16[7], state.v[23].U16[7], s2value, 0, 1, 0, 1)
	}


_04001774:	// 4001774: <4bbfb50f>	VMADH     vec20 = ( acc+= (vec22 * vec31[5]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[2];
		VMADH_operation(0, state.v[20].U16[0], state.v[22].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[22].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[22].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[22].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[22].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[22].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[22].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[22].U16[7], s2value, 0, 1, 0, 1)
	}


_04001778:	// 4001778: <4bdfad0f>	VMADH     vec20 = ( acc+= (vec21 * vec31[6]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[1];
		VMADH_operation(0, state.v[20].U16[0], state.v[21].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[21].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[21].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[21].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[21].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[21].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[21].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[21].U16[7], s2value, 0, 1, 0, 1)
	}


_0400177C:	// 400177c: <4bb2fd0f>	VMADH     vec20 = ( acc+= (vec31 * vec18[5]) << 16) >> 16
	{
		_u16		s2value = state.v[18].U16[2];
		VMADH_operation(0, state.v[20].U16[0], state.v[31].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[31].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[31].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[31].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[31].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[31].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[31].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[31].U16[7], s2value, 0, 1, 0, 1)
	}


_04001780:	// 4001780: <caff2001>	LQV       vec31 <00> = [S7 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S7]);
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


_04001784:	// 4001784: <4b3e9f5d>	VSAW $v29, $v19, $v30[[1]]
	state.v[29].U16[0] = state.accum[0].U16[2];
	state.v[29].U16[1] = state.accum[1].U16[2];
	state.v[29].U16[2] = state.accum[2].U16[2];
	state.v[29].U16[3] = state.accum[3].U16[2];
	state.v[29].U16[4] = state.accum[4].U16[2];
	state.v[29].U16[5] = state.accum[5].U16[2];
	state.v[29].U16[6] = state.accum[6].U16[2];
	state.v[29].U16[7] = state.accum[7].U16[2];


_04001788:	// 4001788: <4b1e9f9d>	VSAW $v30, $v19, $v30[[0]]
	state.v[30].U16[0] = state.accum[0].U16[3];
	state.v[30].U16[1] = state.accum[1].U16[3];
	state.v[30].U16[2] = state.accum[2].U16[3];
	state.v[30].U16[3] = state.accum[3].U16[3];
	state.v[30].U16[4] = state.accum[4].U16[3];
	state.v[30].U16[5] = state.accum[5].U16[3];
	state.v[30].U16[6] = state.accum[6].U16[3];
	state.v[30].U16[7] = state.accum[7].U16[3];


_0400178C:	// 400178c: <4b12ed06>	VMUDN     vec20 = ( acc = vec29 * vec18[0]      ) >> 16
	{
		_u16		s2value = state.v[18].U16[7];
		VMUDN_operation(0, state.v[20].U16[0], state.v[29].U16[0], s2value, 1, 1, 1, 1)
		VMUDN_operation(1, state.v[20].U16[1], state.v[29].U16[1], s2value, 1, 1, 1, 1)
		VMUDN_operation(2, state.v[20].U16[2], state.v[29].U16[2], s2value, 1, 1, 1, 1)
		VMUDN_operation(3, state.v[20].U16[3], state.v[29].U16[3], s2value, 1, 1, 1, 1)
		VMUDN_operation(4, state.v[20].U16[4], state.v[29].U16[4], s2value, 1, 1, 1, 1)
		VMUDN_operation(5, state.v[20].U16[5], state.v[29].U16[5], s2value, 1, 1, 1, 1)
		VMUDN_operation(6, state.v[20].U16[6], state.v[29].U16[6], s2value, 1, 1, 1, 1)
		VMUDN_operation(7, state.v[20].U16[7], state.v[29].U16[7], s2value, 1, 1, 1, 1)
	}


_04001790:	// 4001790: <4b12f78f>	VMADH     vec30 = ( acc+= (vec30 * vec18[0]) << 16) >> 16
	{
		_u16		s2value = state.v[18].U16[7];
		VMADH_operation(0, state.v[30].U16[0], state.v[30].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[30].U16[1], state.v[30].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[30].U16[2], state.v[30].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[30].U16[3], state.v[30].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[30].U16[4], state.v[30].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[30].U16[5], state.v[30].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[30].U16[6], state.v[30].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[30].U16[7], state.v[30].U16[7], s2value, 1, 1, 0, 1)
	}


_04001794:	// 4001794: <1ec0ffef>	BGTZ      (R0>0) --> 1754
	sp_reg_pc = 0x04001794;
	if((_s32)state.r[S6] > 0)
	{
		state.r[S7] = state.r[S7] + 0x0010;
		goto _04001754;
	}


_04001798:	// 4001798: <22f70010>	ADDI      S7 = S7 + 0010
	state.r[S7] = state.r[S7] + 0x0010;


_0400179C:	// 400179c: <22e1fff8>	ADDI      AT = S7 + fff8
	state.r[AT] = state.r[S7] + 0xFFFFFFF8;


_040017A0:	// 40017a0: <0d00045d>	JAL	    04001174
	{
		_u32    addr = (0xFFFFFFF0 + state.r[S7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[30].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[30].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[30].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[30].U32[3];
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

			value1 |= (state.v[30].U64[0] & temp1);
			value2 |= (state.v[30].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}
	sp_reg_pc = 0x040017A0;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001174;


_040017A4:	// 40017a4: <eafe207f>	SQV       [S7 + 0xf0] = vec30 <00>
	{
		_u32    addr = (0xFFFFFFF0 + state.r[S7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[30].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[30].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[30].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[30].U32[3];
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

			value1 |= (state.v[30].U64[0] & temp1);
			value2 |= (state.v[30].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040017A8:	// 40017a8: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_040017AC:	// 40017ac: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_040017B0:	// 40017b0: <14a0ffff>	BNE       (R0!=A1) --> 17b0
	sp_reg_pc = 0x040017B0;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _040017B0;
	}


_040017B4:	// 40017b4: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_040017B8:	// 40017b8: <0900043a>	J         040010e8
	goto _040010E8;


_040017BC:	// 40017bc: <00000000>	NOP       


_040017C0:	// 40017c0: <001a1200>	SLL       V0 = K0 << 8
	state.r[V0] = ((_u32)state.r[K0] << 8);


_040017C4:	// 40017c4: <4a17bdec>	VXOR      vec23 = vec23 xor vec23[<none>]
	rsp_cop2_vxor(&state.v[23], &state.v[23], &state.v[23]);


_040017C8:	// 40017c8: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_040017CC:	// 40017cc: <20420000>	ADDI      V0 = V0 + 0000
	state.r[V0] = state.r[V0] + 0x0000;


_040017D0:	// 40017d0: <2003000f>	ADDI      V1 = R0 + 000f
	state.r[V1] = 0 + 0x000F;


_040017D4:	// 40017d4: <0019af82>	SRL       S5 = T9 >> 30
	state.r[S5] = ((_u32)state.r[T9] >> 30);


_040017D8:	// 40017d8: <1ea00012>	BGTZ      (R0>0) --> 1824
	sp_reg_pc = 0x040017D8;
	if((_s32)state.r[S5] > 0)
	{
		state.r[AT] = state.r[T8] + 0x0000;
		goto _04001824;
	}


_040017DC:	// 40017dc: <23010000>	ADDI      AT = T8 + 0000
	state.r[AT] = state.r[T8] + 0x0000;


_040017E0:	// 40017e0: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_040017E4:	// 40017e4: <1480ffff>	BNE       (R0!=A0) --> 17e4
	sp_reg_pc = 0x040017E4;
	if (state.r[A0] != 0)
	{
		state.r[A0] = 0;
		goto _040017E4;
	}


_040017E8:	// 40017e8: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_040017EC:	// 40017ec: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_040017F0:	// 40017f0: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_040017F4:	// 40017f4: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x040017F4;
	DMARead(state.r[V1]);


_040017F8:	// 40017f8: <0019a082>	SRL       S4 = T9 >> 2
	state.r[S4] = ((_u32)state.r[T9] >> 2);


_040017FC:	// 40017fc: <32940fff>	ANDI      S4 = S4 & 0fff
	state.r[S4] = (state.r[S4] & 0x00000FFF);


_04001800:	// 4001800: <229404e8>	ADDI      S4 = S4 + 04e8
	state.r[S4] = state.r[S4] + 0x04E8;


_04001804:	// 4001804: <c81f2004>	LQV       vec31 <00> = [R0 + 0x40]
	{
		_u32 addr = (0x00000040 + 0);
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


_04001808:	// 4001808: <c8192003>	LQV       vec25 <00> = [R0 + 0x30]
	{
		_u32 addr = (0x00000030 + 0);
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


_0400180C:	// 400180c: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001810:	// 4001810: <14a0ffff>	BNE       (R0!=A1) --> 1810
	sp_reg_pc = 0x04001810;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001810;
	}


_04001814:	// 4001814: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001818:	// 4001818: <cb131800>	LDV       vec19 <00> = [T8 + 0x00]
	rsp_ldv_compiler((state.r[T8] & 0xFFF), 19, 8);


_0400181C:	// 400181c: <09000610>	J         04001840
	state.v[24].U16[0] = Load16_DMEM(0x00000008 + state.r[T8]);
	goto _04001840;


_04001820:	// 4001820: <cb180f04>	LSV       vec24 <14> = [T8 + 0x08]
	state.v[24].U16[0] = Load16_DMEM(0x00000008 + state.r[T8]);


_04001824:	// 4001824: <0019a082>	SRL       S4 = T9 >> 2
	state.r[S4] = ((_u32)state.r[T9] >> 2);


_04001828:	// 4001828: <32940fff>	ANDI      S4 = S4 & 0fff
	state.r[S4] = (state.r[S4] & 0x00000FFF);


_0400182C:	// 400182c: <229404e8>	ADDI      S4 = S4 + 04e8
	state.r[S4] = state.r[S4] + 0x04E8;


_04001830:	// 4001830: <c81f2004>	LQV       vec31 <00> = [R0 + 0x40]
	{
		_u32 addr = (0x00000040 + 0);
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


_04001834:	// 4001834: <4a139cec>	VXOR      vec19 = vec19 xor vec19[<none>]
	rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);


_04001838:	// 4001838: <c8192003>	LQV       vec25 <00> = [R0 + 0x30]
	{
		_u32 addr = (0x00000030 + 0);
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


_0400183C:	// 400183c: <4a18c62c>	VXOR      vec24 = vec24 xor vec24[<none>]
	rsp_cop2_vxor(&state.v[24], &state.v[24], &state.v[24]);


_04001840:	// 4001840: <4894aa00>	MTC2      vec21 <04> = S4
	state.v[21].U16[5] = (_u16)state.r[S4];


_04001844:	// 4001844: <200400b0>	ADDI      A0 = R0 + 00b0
	state.r[A0] = 0 + 0x00B0;


_04001848:	// 4001848: <4884ab00>	MTC2      vec21 <06> = A0
	state.v[21].U16[4] = (_u16)state.r[A0];


_0400184C:	// 400184c: <4a1fce51>	VSUB      vec25 = vec25 - vec31[<none>]
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


_04001850:	// 4001850: <00192382>	SRL       A0 = T9 >> 14
	state.r[A0] = ((_u32)state.r[T9] >> 14);


_04001854:	// 4001854: <4884ac00>	MTC2      vec21 <08> = A0
	state.v[21].U16[3] = (_u16)state.r[A0];


_04001858:	// 4001858: <20040040>	ADDI      A0 = R0 + 0040
	state.r[A0] = 0 + 0x0040;


_0400185C:	// 400185c: <4884ad00>	MTC2      vec21 <10> = A0
	state.v[21].U16[2] = (_u16)state.r[A0];


_04001860:	// 4001860: <4a1fce51>	VSUB      vec25 = vec25 - vec31[<none>]
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


_04001864:	// 4001864: <c81e2005>	LQV       vec30 <00> = [R0 + 0x50]
	{
		_u32 addr = (0x00000050 + 0);
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


_04001868:	// 4001868: <c81d2006>	LQV       vec29 <00> = [R0 + 0x60]
	{
		_u32 addr = (0x00000060 + 0);
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


_0400186C:	// 400186c: <c81c2007>	LQV       vec28 <00> = [R0 + 0x70]
	{
		_u32 addr = (0x00000070 + 0);
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


_04001870:	// 4001870: <4bf8fe05>	VMUDM     vec24 = ( acc = vec31 * vec24[7] ) >> 16
	{
		_u16		s2value = state.v[24].U16[0];
		VMUDM_operation(0, state.v[24].U16[0], state.v[31].U16[0], s2value, 0, 1, 1, 1)
		VMUDM_operation(1, state.v[24].U16[1], state.v[31].U16[1], s2value, 0, 1, 1, 1)
		VMUDM_operation(2, state.v[24].U16[2], state.v[31].U16[2], s2value, 0, 1, 1, 1)
		VMUDM_operation(3, state.v[24].U16[3], state.v[31].U16[3], s2value, 0, 1, 1, 1)
		VMUDM_operation(4, state.v[24].U16[4], state.v[31].U16[4], s2value, 0, 1, 1, 1)
		VMUDM_operation(5, state.v[24].U16[5], state.v[31].U16[5], s2value, 0, 1, 1, 1)
		VMUDM_operation(6, state.v[24].U16[6], state.v[31].U16[6], s2value, 0, 1, 1, 1)
		VMUDM_operation(7, state.v[24].U16[7], state.v[31].U16[7], s2value, 0, 1, 1, 1)
	}


_04001874:	// 4001874: <c81b2008>	LQV       vec27 <00> = [R0 + 0x80]
	{
		_u32 addr = (0x00000080 + 0);
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


_04001878:	// 4001878: <4b95cdcd>	VMADM     vec23 = ( acc+= vec25 * vec21[4] ) >> 16
	{
		_u16		s2value = state.v[21].U16[3];
		VMADM_operation(0, state.v[23].U16[0], state.v[25].U16[0], s2value, 1, 1, 1, 1)
		VMADM_operation(1, state.v[23].U16[1], state.v[25].U16[1], s2value, 1, 1, 1, 1)
		VMADM_operation(2, state.v[23].U16[2], state.v[25].U16[2], s2value, 1, 1, 1, 1)
		VMADM_operation(3, state.v[23].U16[3], state.v[25].U16[3], s2value, 1, 1, 1, 1)
		VMADM_operation(4, state.v[23].U16[4], state.v[25].U16[4], s2value, 1, 1, 1, 1)
		VMADM_operation(5, state.v[23].U16[5], state.v[25].U16[5], s2value, 1, 1, 1, 1)
		VMADM_operation(6, state.v[23].U16[6], state.v[25].U16[6], s2value, 1, 1, 1, 1)
		VMADM_operation(7, state.v[23].U16[7], state.v[25].U16[7], s2value, 1, 1, 1, 1)
	}


_0400187C:	// 400187c: <c81a2009>	LQV       vec26 <00> = [R0 + 0x90]
	{
		_u32 addr = (0x00000090 + 0);
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


_04001880:	// 4001880: <4b1efe0e>	VMADN     vec24 = ( acc+= vec31 * vec30[0] )
	{
		_u16		s2value = state.v[30].U16[7];
		VMADN_operation(0, state.v[24].U16[0], state.v[31].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[24].U16[1], state.v[31].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[24].U16[2], state.v[31].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[24].U16[3], state.v[31].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[24].U16[4], state.v[31].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[24].U16[5], state.v[31].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[24].U16[6], state.v[31].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[24].U16[7], state.v[31].U16[7], s2value, 1, 0, 0, 1)
	}


_04001884:	// 4001884: <ea931800>	SDV       [S4 + 0x00] = vec19 <00>
	{
		_u32	addr = (0x00000000 + state.r[S4]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[19].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[19].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[19].U32[0x00000003];
		}
	}


_04001888:	// 4001888: <c8192003>	LQV       vec25 <00> = [R0 + 0x30]
	{
		_u32 addr = (0x00000030 + 0);
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


_0400188C:	// 400188c: <4b55fd86>	VMUDN     vec22 = ( acc = vec31 * vec21[2]      ) >> 16
	{
		_u16		s2value = state.v[21].U16[5];
		VMUDN_operation(0, state.v[22].U16[0], state.v[31].U16[0], s2value, 0, 1, 1, 1)
		VMUDN_operation(1, state.v[22].U16[1], state.v[31].U16[1], s2value, 0, 1, 1, 1)
		VMUDN_operation(2, state.v[22].U16[2], state.v[31].U16[2], s2value, 0, 1, 1, 1)
		VMUDN_operation(3, state.v[22].U16[3], state.v[31].U16[3], s2value, 0, 1, 1, 1)
		VMUDN_operation(4, state.v[22].U16[4], state.v[31].U16[4], s2value, 0, 1, 1, 1)
		VMUDN_operation(5, state.v[22].U16[5], state.v[31].U16[5], s2value, 0, 1, 1, 1)
		VMUDN_operation(6, state.v[22].U16[6], state.v[31].U16[6], s2value, 0, 1, 1, 1)
		VMUDN_operation(7, state.v[22].U16[7], state.v[31].U16[7], s2value, 0, 1, 1, 1)
	}


_04001890:	// 4001890: <20160170>	ADDI      S6 = R0 + 0170
	state.r[S6] = 0 + 0x0170;


_04001894:	// 4001894: <4b5ebd8e>	VMADN     vec22 = ( acc+= vec23 * vec30[2] )
	{
		_u16		s2value = state.v[30].U16[5];
		VMADN_operation(0, state.v[22].U16[0], state.v[23].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[22].U16[1], state.v[23].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[22].U16[2], state.v[23].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[22].U16[3], state.v[23].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[22].U16[4], state.v[23].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[22].U16[5], state.v[23].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[22].U16[6], state.v[23].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[22].U16[7], state.v[23].U16[7], s2value, 1, 0, 0, 1)
	}


_04001898:	// 4001898: <33240003>	ANDI      A0 = T9 & 0003
	state.r[A0] = (state.r[T9] & 0x00000003);


_0400189C:	// 400189c: <4bb5c504>	VMUDL     vec20 = ( acc = (vec24 * vec21[5] >> 16)      )
	{
		_u16		s2value = state.v[21].U16[2];
		VMUDL_operation(0, state.v[20].U16[0], state.v[24].U16[0], s2value, 1, 1, 1, 1)
		VMUDL_operation(1, state.v[20].U16[1], state.v[24].U16[1], s2value, 1, 1, 1, 1)
		VMUDL_operation(2, state.v[20].U16[2], state.v[24].U16[2], s2value, 1, 1, 1, 1)
		VMUDL_operation(3, state.v[20].U16[3], state.v[24].U16[3], s2value, 1, 1, 1, 1)
		VMUDL_operation(4, state.v[20].U16[4], state.v[24].U16[4], s2value, 1, 1, 1, 1)
		VMUDL_operation(5, state.v[20].U16[5], state.v[24].U16[5], s2value, 1, 1, 1, 1)
		VMUDL_operation(6, state.v[20].U16[6], state.v[24].U16[6], s2value, 1, 1, 1, 1)
		VMUDL_operation(7, state.v[20].U16[7], state.v[24].U16[7], s2value, 1, 1, 1, 1)
	}


_040018A0:	// 40018a0: <10800002>	BEQ       (R0==A0) --> 18ac
	sp_reg_pc = 0x040018A0;
	if (state.r[A0] == 0)
	{
		state.r[S7] = 0 + 0x04F0;
		goto _040018AC;
	}


_040018A4:	// 40018a4: <201704f0>	ADDI      S7 = R0 + 04f0
	state.r[S7] = 0 + 0x04F0;


_040018A8:	// 40018a8: <20170660>	ADDI      S7 = R0 + 0660
	state.r[S7] = 0 + 0x0660;


_040018AC:	// 40018ac: <eb180b84>	SSV       [T8 + 0x08] = vec24 <07>
	Save16_DMEM(state.v[24].U16[4], (0x00000008 + state.r[T8]) & 0xfff);


_040018B0:	// 40018b0: <4b9ea506>	VMUDN     vec20 = ( acc = vec20 * vec30[4]      ) >> 16
	{
		_u16		s2value = state.v[30].U16[3];
		VMUDN_operation(0, state.v[20].U16[0], state.v[20].U16[0], s2value, 0, 1, 1, 1)
		VMUDN_operation(1, state.v[20].U16[1], state.v[20].U16[1], s2value, 0, 1, 1, 1)
		VMUDN_operation(2, state.v[20].U16[2], state.v[20].U16[2], s2value, 0, 1, 1, 1)
		VMUDN_operation(3, state.v[20].U16[3], state.v[20].U16[3], s2value, 0, 1, 1, 1)
		VMUDN_operation(4, state.v[20].U16[4], state.v[20].U16[4], s2value, 0, 1, 1, 1)
		VMUDN_operation(5, state.v[20].U16[5], state.v[20].U16[5], s2value, 0, 1, 1, 1)
		VMUDN_operation(6, state.v[20].U16[6], state.v[20].U16[6], s2value, 0, 1, 1, 1)
		VMUDN_operation(7, state.v[20].U16[7], state.v[20].U16[7], s2value, 0, 1, 1, 1)
	}


_040018B4:	// 40018b4: <e816207b>	SQV       [R0 + 0xb0] = vec22 <00>
	{
		_u32    addr = (0xFFFFFFB0 + 0);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[22].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[22].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[22].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[22].U32[3];
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

			value1 |= (state.v[22].U64[0] & temp1);
			value2 |= (state.v[22].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040018B8:	// 40018b8: <4b75fd0e>	VMADN     vec20 = ( acc+= vec31 * vec21[3] )
	{
		_u16		s2value = state.v[21].U16[4];
		VMADN_operation(0, state.v[20].U16[0], state.v[31].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[20].U16[1], state.v[31].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[20].U16[2], state.v[31].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[20].U16[3], state.v[31].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[20].U16[4], state.v[31].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[20].U16[5], state.v[31].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[20].U16[6], state.v[31].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[20].U16[7], state.v[31].U16[7], s2value, 1, 0, 0, 1)
	}


_040018BC:	// 40018bc: <e814207c>	SQV       [R0 + 0xc0] = vec20 <00>
	{
		_u32    addr = (0xFFFFFFC0 + 0);
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


_040018C0:	// 40018c0: <84150fb0>	LH        S5 = [R0+0fb0]
	state.r[S5] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FB0) & 0xfff);


_040018C4:	// 40018c4: <840d0fc0>	LH        T5 = [R0+0fc0]
	state.r[T5] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FC0) & 0xfff);


_040018C8:	// 40018c8: <84110fb8>	LH        S1 = [R0+0fb8]
	state.r[S1] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FB8) & 0xfff);


_040018CC:	// 40018cc: <84090fc8>	LH        T1 = [R0+0fc8]
	state.r[T1] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FC8) & 0xfff);


_040018D0:	// 40018d0: <84140fb2>	LH        S4 = [R0+0fb2]
	state.r[S4] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FB2) & 0xfff);


_040018D4:	// 40018d4: <840c0fc2>	LH        T4 = [R0+0fc2]
	state.r[T4] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FC2) & 0xfff);


_040018D8:	// 40018d8: <84100fba>	LH        S0 = [R0+0fba]
	state.r[S0] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FBA) & 0xfff);


_040018DC:	// 40018dc: <84080fca>	LH        T0 = [R0+0fca]
	state.r[T0] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FCA) & 0xfff);


_040018E0:	// 40018e0: <84130fb4>	LH        S3 = [R0+0fb4]
	state.r[S3] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FB4) & 0xfff);


_040018E4:	// 40018e4: <840b0fc4>	LH        T3 = [R0+0fc4]
	state.r[T3] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FC4) & 0xfff);


_040018E8:	// 40018e8: <840f0fbc>	LH        T7 = [R0+0fbc]
	state.r[T7] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FBC) & 0xfff);


_040018EC:	// 40018ec: <84070fcc>	LH        A3 = [R0+0fcc]
	state.r[A3] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FCC) & 0xfff);


_040018F0:	// 40018f0: <84120fb6>	LH        S2 = [R0+0fb6]
	state.r[S2] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FB6) & 0xfff);


_040018F4:	// 40018f4: <840a0fc6>	LH        T2 = [R0+0fc6]
	state.r[T2] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FC6) & 0xfff);


_040018F8:	// 40018f8: <840e0fbe>	LH        T6 = [R0+0fbe]
	state.r[T6] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FBE) & 0xfff);


_040018FC:	// 40018fc: <84060fce>	LH        A2 = [R0+0fce]
	state.r[A2] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FCE) & 0xfff);


_04001900:	// 4001900: <cab31800>	LDV       vec19 <00> = [S5 + 0x00]
	// In function: SmashBros193C_1A28
	//Call the function: SmashBros193C_1A28
	SmashBros193C_1A28();
	goto _040019F0;

_04001904:	// 4001904: <4bf8fe05>	VMUDM     vec24 = ( acc = vec31 * vec24[7] ) >> 16
	ErrorToCallFunction(0x04001904);

_04001908:	// 4001908: <c9b21800>	LDV       vec18 <00> = [T5 + 0x00]
	ErrorToCallFunction(0x04001908);

_0400190C:	// 400190c: <4bf7fe0f>	VMADH     vec24 = ( acc+= (vec31 * vec23[7]) << 16) >> 16
	ErrorToCallFunction(0x0400190C);

_04001910:	// 4001910: <ca331c00>	LDV       vec19 <08> = [S1 + 0x00]
	ErrorToCallFunction(0x04001910);

_04001914:	// 4001914: <4b95cdcd>	VMADM     vec23 = ( acc+= vec25 * vec21[4] ) >> 16
	ErrorToCallFunction(0x04001914);

_04001918:	// 4001918: <c9321c00>	LDV       vec18 <08> = [T1 + 0x00]
	ErrorToCallFunction(0x04001918);

_0400191C:	// 400191c: <4b1efe0e>	VMADN     vec24 = ( acc+= vec31 * vec30[0] )
	ErrorToCallFunction(0x0400191C);

_04001920:	// 4001920: <ca911800>	LDV       vec17 <00> = [S4 + 0x00]
	ErrorToCallFunction(0x04001920);

_04001924:	// 4001924: <4b55fd86>	VMUDN     vec22 = ( acc = vec31 * vec21[2]      ) >> 16
	ErrorToCallFunction(0x04001924);

_04001928:	// 4001928: <c9901800>	LDV       vec16 <00> = [T4 + 0x00]
	ErrorToCallFunction(0x04001928);

_0400192C:	// 400192c: <ca111c00>	LDV       vec17 <08> = [S0 + 0x00]
	ErrorToCallFunction(0x0400192C);

_04001930:	// 4001930: <4b5ebd8e>	VMADN     vec22 = ( acc+= vec23 * vec30[2] )
	ErrorToCallFunction(0x04001930);

_04001934:	// 4001934: <c9101c00>	LDV       vec16 <08> = [T0 + 0x00]
	ErrorToCallFunction(0x04001934);

_04001938:	// 4001938: <4bb5c504>	VMUDL     vec20 = ( acc = (vec24 * vec21[5] >> 16)      )
	ErrorToCallFunction(0x04001938);

_0400193C:	// 400193c: <ca6f1800>	LDV       vec15 <00> = [S3 + 0x00]
	ErrorToCallFunction(0x0400193C);

_04001940:	// 4001940: <c96e1800>	LDV       vec14 <00> = [T3 + 0x00]
	ErrorToCallFunction(0x04001940);

_04001944:	// 4001944: <c9ef1c00>	LDV       vec15 <08> = [T7 + 0x00]
	ErrorToCallFunction(0x04001944);

_04001948:	// 4001948: <c8ee1c00>	LDV       vec14 <08> = [A3 + 0x00]
	ErrorToCallFunction(0x04001948);

_0400194C:	// 400194c: <4b9ea506>	VMUDN     vec20 = ( acc = vec20 * vec30[4]      ) >> 16
	ErrorToCallFunction(0x0400194C);

_04001950:	// 4001950: <ca4d1800>	LDV       vec13 <00> = [S2 + 0x00]
	ErrorToCallFunction(0x04001950);

_04001954:	// 4001954: <4b75fd0e>	VMADN     vec20 = ( acc+= vec31 * vec21[3] )
	ErrorToCallFunction(0x04001954);

_04001958:	// 4001958: <c94c1800>	LDV       vec12 <00> = [T2 + 0x00]
	ErrorToCallFunction(0x04001958);

_0400195C:	// 400195c: <c9cd1c00>	LDV       vec13 <08> = [T6 + 0x00]
	ErrorToCallFunction(0x0400195C);

_04001960:	// 4001960: <4a129ac0>	VMULF     vec11 = ( acc = (vec19 * vec18[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001960);

_04001964:	// 4001964: <c8cc1c00>	LDV       vec12 <08> = [A2 + 0x00]
	ErrorToCallFunction(0x04001964);

_04001968:	// 4001968: <4a108a80>	VMULF     vec10 = ( acc = (vec17 * vec16[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001968);

_0400196C:	// 400196c: <e816207b>	SQV       [R0 + 0xb0] = vec22 <00>
	ErrorToCallFunction(0x0400196C);

_04001970:	// 4001970: <4a0e7a40>	VMULF     vec09 = ( acc = (vec15 * vec14[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001970);

_04001974:	// 4001974: <e814207c>	SQV       [R0 + 0xc0] = vec20 <00>
	ErrorToCallFunction(0x04001974);

_04001978:	// 4001978: <84150fb0>	LH        S5 = [R0+0fb0]
	ErrorToCallFunction(0x04001978);

_0400197C:	// 400197c: <840d0fc0>	LH        T5 = [R0+0fc0]
	ErrorToCallFunction(0x0400197C);

_04001980:	// 4001980: <4a0c6a00>	VMULF     vec08 = ( acc = (vec13 * vec12[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001980);

_04001984:	// 4001984: <84110fb8>	LH        S1 = [R0+0fb8]
	ErrorToCallFunction(0x04001984);

_04001988:	// 4001988: <4a6b5ad0>	VADD      vec11 = vec11 + vec11[1q]
	ErrorToCallFunction(0x04001988);

_0400198C:	// 400198c: <84090fc8>	LH        T1 = [R0+0fc8]
	ErrorToCallFunction(0x0400198C);

_04001990:	// 4001990: <4a6a5290>	VADD      vec10 = vec10 + vec10[1q]
	ErrorToCallFunction(0x04001990);

_04001994:	// 4001994: <84140fb2>	LH        S4 = [R0+0fb2]
	ErrorToCallFunction(0x04001994);

_04001998:	// 4001998: <4a694a50>	VADD      vec09 = vec09 + vec09[1q]
	ErrorToCallFunction(0x04001998);

_0400199C:	// 400199c: <840c0fc2>	LH        T4 = [R0+0fc2]
	ErrorToCallFunction(0x0400199C);

_040019A0:	// 40019a0: <4a684210>	VADD      vec08 = vec08 + vec08[1q]
	ErrorToCallFunction(0x040019A0);

_040019A4:	// 40019a4: <84100fba>	LH        S0 = [R0+0fba]
	ErrorToCallFunction(0x040019A4);

_040019A8:	// 40019a8: <4acb5ad0>	VADD      vec11 = vec11 + vec11[2h]
	ErrorToCallFunction(0x040019A8);

_040019AC:	// 40019ac: <84080fca>	LH        T0 = [R0+0fca]
	ErrorToCallFunction(0x040019AC);

_040019B0:	// 40019b0: <4aca5290>	VADD      vec10 = vec10 + vec10[2h]
	ErrorToCallFunction(0x040019B0);

_040019B4:	// 40019b4: <84130fb4>	LH        S3 = [R0+0fb4]
	ErrorToCallFunction(0x040019B4);

_040019B8:	// 40019b8: <4ac94a50>	VADD      vec09 = vec09 + vec09[2h]
	ErrorToCallFunction(0x040019B8);

_040019BC:	// 40019bc: <840b0fc4>	LH        T3 = [R0+0fc4]
	ErrorToCallFunction(0x040019BC);

_040019C0:	// 40019c0: <4ac84210>	VADD      vec08 = vec08 + vec08[2h]
	ErrorToCallFunction(0x040019C0);

_040019C4:	// 40019c4: <840f0fbc>	LH        T7 = [R0+0fbc]
	ErrorToCallFunction(0x040019C4);

_040019C8:	// 40019c8: <4a8be9c6>	VMUDN     vec07 = ( acc = vec29 * vec11[0h]      ) >> 16
	ErrorToCallFunction(0x040019C8);

_040019CC:	// 40019cc: <84070fcc>	LH        A3 = [R0+0fcc]
	ErrorToCallFunction(0x040019CC);

_040019D0:	// 40019d0: <4a8ae1ce>	VMADN     vec07 = ( acc+= vec28 * vec10[0h] )
	ErrorToCallFunction(0x040019D0);

_040019D4:	// 40019d4: <84120fb6>	LH        S2 = [R0+0fb6]
	ErrorToCallFunction(0x040019D4);

_040019D8:	// 40019d8: <4a89d9ce>	VMADN     vec07 = ( acc+= vec27 * vec09[0h] )
	ErrorToCallFunction(0x040019D8);

_040019DC:	// 40019dc: <840a0fc6>	LH        T2 = [R0+0fc6]
	ErrorToCallFunction(0x040019DC);

_040019E0:	// 40019e0: <4a88d1ce>	VMADN     vec07 = ( acc+= vec26 * vec08[0h] )
	ErrorToCallFunction(0x040019E0);

_040019E4:	// 40019e4: <840e0fbe>	LH        T6 = [R0+0fbe]
	ErrorToCallFunction(0x040019E4);

_040019E8:	// 40019e8: <84060fce>	LH        A2 = [R0+0fce]
	ErrorToCallFunction(0x040019E8);

_040019EC:	// 40019ec: <22d6fff0>	ADDI      S6 = S6 + fff0
	ErrorToCallFunction(0x040019EC);

_040019F0:	// 40019f0: <1ac00003>	BLEZ      (R0<=0) --> 1a00
	sp_reg_pc = 0x040019F0;
	if ((_s32)state.r[S6] <= 0)
	{
		{
			_u32    addr = (0x00000000 + state.r[S7]);
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
		goto _04001A00;
	}


_040019F4:	// 40019f4: <eae72000>	SQV       [S7 + 0x00] = vec07 <00>
	{
		_u32    addr = (0x00000000 + state.r[S7]);
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


_040019F8:	// 40019f8: <09000640>	J         04001900
	state.r[S7] = state.r[S7] + 0x0010;
	goto _04001900;


_040019FC:	// 40019fc: <22f70010>	ADDI      S7 = S7 + 0010
	state.r[S7] = state.r[S7] + 0x0010;


_04001A00:	// 4001a00: <cab31800>	LDV       vec19 <00> = [S5 + 0x00]
	rsp_ldv_compiler((state.r[S5] & 0xFFF), 19, 8);


_04001A04:	// 4001a04: <eb180804>	SSV       [T8 + 0x08] = vec24 <00>
	Save16_DMEM(state.v[24].U16[7], (0x00000008 + state.r[T8]) & 0xfff);


_04001A08:	// 4001a08: <0d00045d>	JAL	    04001174
	{
		_u32	addr = (0x00000000 + state.r[T8]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[19].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[19].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[19].U32[0x00000003];
		}
	}
	sp_reg_pc = 0x04001A08;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001174;


_04001A0C:	// 4001a0c: <eb131800>	SDV       [T8 + 0x00] = vec19 <00>
	{
		_u32	addr = (0x00000000 + state.r[T8]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[19].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[19].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[19].U32[0x00000003];
		}
	}


_04001A10:	// 4001a10: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001A14:	// 4001a14: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001A18:	// 4001a18: <14a0ffff>	BNE       (R0!=A1) --> 1a18
	sp_reg_pc = 0x04001A18;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001A18;
	}


_04001A1C:	// 4001a1c: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001A20:	// 4001a20: <0900043a>	J         040010e8
	goto _040010E8;


_04001A24:	// 4001a24: <00000000>	NOP       


_04001A28:	// 4001a28: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001A2C:	// 4001a2c: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001A30:	// 4001a30: <20420000>	ADDI      V0 = V0 + 0000
	state.r[V0] = state.r[V0] + 0x0000;


_04001A34:	// 4001a34: <c81f2004>	LQV       vec31 <00> = [R0 + 0x40]
	{
		_u32 addr = (0x00000040 + 0);
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


_04001A38:	// 4001a38: <c80a2005>	LQV       vec10 <00> = [R0 + 0x50]
	{
		_u32 addr = (0x00000050 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[10].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[10].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[10].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[10].U32[3] = *(_u32 *)&pDMEM[addr];
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

			state.v[10].U64[0] = state.v[10].U64[0] & ~temp1;
			state.v[10].U64[1] = state.v[10].U64[1] & ~temp2;
			state.v[10].U64[0] = state.v[10].U64[0] | value1;
			state.v[10].U64[1] = state.v[10].U64[1] | value2;
		}
	}


_04001A3C:	// 4001a3c: <c81e200a>	LQV       vec30 <00> = [R0 + 0xa0]
	{
		_u32 addr = (0x000000A0 + 0);
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


_04001A40:	// 4001a40: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_04001A44:	// 4001a44: <001a7402>	SRL       T6 = K0 >> 16
	state.r[T6] = ((_u32)state.r[K0] >> 16);


_04001A48:	// 4001a48: <31cf0001>	ANDI      T7 = T6 & 0001
	state.r[T7] = (state.r[T6] & 0x00000001);


_04001A4C:	// 4001a4c: <1de0000a>	BGTZ      (R0>0) --> 1a78
	sp_reg_pc = 0x04001A4C;
	if((_s32)state.r[T7] > 0)
	{
		state.r[AT] = state.r[T8] + 0x0000;
		goto _04001A78;
	}


_04001A50:	// 4001a50: <23010000>	ADDI      AT = T8 + 0000
	state.r[AT] = state.r[T8] + 0x0000;


_04001A54:	// 4001a54: <0d000456>	JAL	    04001158
	state.r[V1] = 0 + 0x004F;
	sp_reg_pc = 0x04001A54;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001158;


_04001A58:	// 4001a58: <2003004f>	ADDI      V1 = R0 + 004f
	state.r[V1] = 0 + 0x004F;


_04001A5C:	// 4001a5c: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001A60:	// 4001a60: <14a0ffff>	BNE       (R0!=A1) --> 1a60
	sp_reg_pc = 0x04001A60;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001A60;
	}


_04001A64:	// 4001a64: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001A68:	// 4001a68: <cb142000>	LQV       vec20 <00> = [T8 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T8]);
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


_04001A6C:	// 4001a6c: <cb152001>	LQV       vec21 <00> = [T8 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T8]);
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


_04001A70:	// 4001a70: <cb122002>	LQV       vec18 <00> = [T8 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[T8]);
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


_04001A74:	// 4001a74: <cb132003>	LQV       vec19 <00> = [T8 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[T8]);
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


_04001A78:	// 4001a78: <cb182004>	LQV       vec24 <00> = [T8 + 0x40]
	{
		_u32 addr = (0x00000040 + state.r[T8]);
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


_04001A7C:	// 4001a7c: <201004f0>	ADDI      S0 = R0 + 04f0
	state.r[S0] = 0 + 0x04F0;


_04001A80:	// 4001a80: <201509d0>	ADDI      S5 = R0 + 09d0
	state.r[S5] = 0 + 0x09D0;


_04001A84:	// 4001a84: <20140b40>	ADDI      S4 = R0 + 0b40
	state.r[S4] = 0 + 0x0B40;


_04001A88:	// 4001a88: <20130cb0>	ADDI      S3 = R0 + 0cb0
	state.r[S3] = 0 + 0x0CB0;


_04001A8C:	// 4001a8c: <20120e20>	ADDI      S2 = R0 + 0e20
	state.r[S2] = 0 + 0x0E20;


_04001A90:	// 4001a90: <20110170>	ADDI      S1 = R0 + 0170
	state.r[S1] = 0 + 0x0170;


_04001A94:	// 4001a94: <4816c400>	MFC2      S6 = vec24 <08>
	state.r[S6] = (_s32)state.v[24].S16[3];


_04001A98:	// 4001a98: <4bd8fa68>	VAND      vec09 = vec31 and vec24[6]
	rsp_cop2_vand_element(&state.v[9], &state.v[31], &state.v[24].U16[1]);


_04001A9C:	// 4001a9c: <4a090251>	VSUB      vec09 = vec00 - vec09[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[9].U16[0], state.v[0].U16[0], state.v[9].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[9].U16[1], state.v[0].U16[1], state.v[9].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[9].U16[2], state.v[0].U16[2], state.v[9].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[9].U16[3], state.v[0].U16[3], state.v[9].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[9].U16[4], state.v[0].U16[4], state.v[9].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[9].U16[5], state.v[0].U16[5], state.v[9].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[9].U16[6], state.v[0].U16[6], state.v[9].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[9].U16[7], state.v[0].U16[7], state.v[9].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001AA0:	// 4001aa0: <4bf8fa28>	VAND      vec08 = vec31 and vec24[7]
	rsp_cop2_vand_element(&state.v[8], &state.v[31], &state.v[24].U16[0]);


_04001AA4:	// 4001aa4: <4a080211>	VSUB      vec08 = vec00 - vec08[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[8].U16[0], state.v[0].U16[0], state.v[8].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[8].U16[1], state.v[0].U16[1], state.v[8].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[8].U16[2], state.v[0].U16[2], state.v[8].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[8].U16[3], state.v[0].U16[3], state.v[8].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[8].U16[4], state.v[0].U16[4], state.v[8].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[8].U16[5], state.v[0].U16[5], state.v[8].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[8].U16[6], state.v[0].U16[6], state.v[8].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[8].U16[7], state.v[0].U16[7], state.v[8].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001AA8:	// 4001aa8: <4a09422c>	VXOR      vec08 = vec08 xor vec09[<none>]
	rsp_cop2_vxor(&state.v[8], &state.v[8], &state.v[9]);


_04001AAC:	// 4001aac: <11e00035>	BEQ       (R0==T7) --> 1b84
	sp_reg_pc = 0x04001AAC;
	if (state.r[T7] == 0)
	{
		state.r[S7] = (_s32)state.v[24].S16[6];
		goto _04001B84;
	}


_04001AB0:	// 4001ab0: <4817c100>	MFC2      S7 = vec24 <02>
	state.r[S7] = (_s32)state.v[24].S16[6];


_04001AB4:	// 4001ab4: <2003004f>	ADDI      V1 = R0 + 004f
	state.r[V1] = 0 + 0x004F;


_04001AB8:	// 4001ab8: <4a14a52c>	VXOR      vec20 = vec20 xor vec20[<none>]
	rsp_cop2_vxor(&state.v[20], &state.v[20], &state.v[20]);


_04001ABC:	// 4001abc: <cb140f28>	LSV       vec20 <14> = [T8 + 0x10]
	state.v[20].U16[0] = Load16_DMEM(0x00000050 + state.r[T8]);


_04001AC0:	// 4001ac0: <4a15ad6c>	VXOR      vec21 = vec21 xor vec21[<none>]
	rsp_cop2_vxor(&state.v[21], &state.v[21], &state.v[21]);


_04001AC4:	// 4001ac4: <ca112000>	LQV       vec17 <00> = [S0 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S0]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[17].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[17].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[17].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[17].U32[3] = *(_u32 *)&pDMEM[addr];
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

			state.v[17].U64[0] = state.v[17].U64[0] & ~temp1;
			state.v[17].U64[1] = state.v[17].U64[1] & ~temp2;
			state.v[17].U64[0] = state.v[17].U64[0] | value1;
			state.v[17].U64[1] = state.v[17].U64[1] | value2;
		}
	}


_04001AC8:	// 4001ac8: <4a1294ac>	VXOR      vec18 = vec18 xor vec18[<none>]
	rsp_cop2_vxor(&state.v[18], &state.v[18], &state.v[18]);


_04001ACC:	// 4001acc: <489a9700>	MTC2      vec18 <14> = K0
	state.v[18].U16[0] = (_u16)state.r[K0];


_04001AD0:	// 4001ad0: <4b58f5c4>	VMUDL     vec23 = ( acc = (vec30 * vec24[2] >> 16)      )
	{
		_u16		s2value = state.v[24].U16[5];
		VMUDL_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 0, 1, 1, 1)
		VMUDL_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 0, 1, 1, 1)
		VMUDL_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 0, 1, 1, 1)
		VMUDL_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 0, 1, 1, 1)
		VMUDL_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 0, 1, 1, 1)
		VMUDL_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 0, 1, 1, 1)
		VMUDL_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 0, 1, 1, 1)
		VMUDL_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 0, 1, 1, 1)
	}


_04001AD4:	// 4001ad4: <cabd2000>	LQV       vec29 <00> = [S5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S5]);
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


_04001AD8:	// 4001ad8: <4b38f5ce>	VMADN     vec23 = ( acc+= vec30 * vec24[1] )
	{
		_u16		s2value = state.v[24].U16[6];
		VMADN_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 1, 1, 1, 1)
		VMADN_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 1, 1, 1, 1)
		VMADN_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 1, 1, 1, 1)
		VMADN_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 1, 1, 1, 1)
		VMADN_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 1, 1, 1, 1)
		VMADN_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 1, 1, 1, 1)
		VMADN_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 1, 1, 1, 1)
		VMADN_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 1, 1, 1, 1)
	}


_04001ADC:	// 4001adc: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
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


_04001AE0:	// 4001ae0: <4bf4fd0f>	VMADH     vec20 = ( acc+= (vec31 * vec20[7]) << 16) >> 16
	{
		_u16		s2value = state.v[20].U16[0];
		VMADH_operation(0, state.v[20].U16[0], state.v[31].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[31].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[31].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[31].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[31].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[31].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[31].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[31].U16[7], s2value, 1, 1, 0, 1)
	}


_04001AE4:	// 4001ae4: <ca9c2000>	LQV       vec28 <00> = [S4 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S4]);
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


_04001AE8:	// 4001ae8: <4b00fd4e>	VMADN     vec21 = ( acc+= vec31 * vec00[0] )
	{
		_u16		s2value = state.v[0].U16[7];
		VMADN_operation(0, state.v[21].U16[0], state.v[31].U16[0], s2value, 1, 1, 1, 1)
		VMADN_operation(1, state.v[21].U16[1], state.v[31].U16[1], s2value, 1, 1, 1, 1)
		VMADN_operation(2, state.v[21].U16[2], state.v[31].U16[2], s2value, 1, 1, 1, 1)
		VMADN_operation(3, state.v[21].U16[3], state.v[31].U16[3], s2value, 1, 1, 1, 1)
		VMADN_operation(4, state.v[21].U16[4], state.v[31].U16[4], s2value, 1, 1, 1, 1)
		VMADN_operation(5, state.v[21].U16[5], state.v[31].U16[5], s2value, 1, 1, 1, 1)
		VMADN_operation(6, state.v[21].U16[6], state.v[31].U16[6], s2value, 1, 1, 1, 1)
		VMADN_operation(7, state.v[21].U16[7], state.v[31].U16[7], s2value, 1, 1, 1, 1)
	}


_04001AEC:	// 4001aec: <06e10003>	BGEZ ?-?
	sp_reg_pc = 0x04001AEC;
	if (state.r[S7] >= 0)
	{
		rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);
		goto _04001AFC;
	}


_04001AF0:	// 4001af0: <4a139cec>	VXOR      vec19 = vec19 xor vec19[<none>]
	rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);


_04001AF4:	// 4001af4: <090006c0>	J         04001b00
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);
	goto _04001B00;


_04001AF8:	// 4001af8: <4b18a523>	VGE       vec20 = (vec20 >= vec24[0])
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001AFC:	// 4001afc: <4b18a520>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001B00:	// 4001b00: <4a114c6c>	VXOR      vec17 = vec09 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[9], &state.v[17]);


_04001B04:	// 4001b04: <4bb8f5c4>	VMUDL     vec23 = ( acc = (vec30 * vec24[5] >> 16)      )
	{
		_u16		s2value = state.v[24].U16[2];
		VMUDL_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 0, 1, 1, 1)
		VMUDL_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 0, 1, 1, 1)
		VMUDL_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 0, 1, 1, 1)
		VMUDL_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 0, 1, 1, 1)
		VMUDL_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 0, 1, 1, 1)
		VMUDL_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 0, 1, 1, 1)
		VMUDL_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 0, 1, 1, 1)
		VMUDL_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 0, 1, 1, 1)
	}


_04001B08:	// 4001b08: <ca5a2000>	LQV       vec26 <00> = [S2 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S2]);
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


_04001B0C:	// 4001b0c: <4b98f5ce>	VMADN     vec23 = ( acc+= vec30 * vec24[4] )
	{
		_u16		s2value = state.v[24].U16[3];
		VMADN_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 1, 1, 1, 1)
		VMADN_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 1, 1, 1, 1)
		VMADN_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 1, 1, 1, 1)
		VMADN_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 1, 1, 1, 1)
		VMADN_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 1, 1, 1, 1)
		VMADN_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 1, 1, 1, 1)
		VMADN_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 1, 1, 1, 1)
		VMADN_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 1, 1, 1, 1)
	}


_04001B10:	// 4001b10: <2231fff0>	ADDI      S1 = S1 + fff0
	state.r[S1] = state.r[S1] + 0xFFFFFFF0;


_04001B14:	// 4001b14: <4bf2fc8f>	VMADH     vec18 = ( acc+= (vec31 * vec18[7]) << 16) >> 16
	{
		_u16		s2value = state.v[18].U16[0];
		VMADH_operation(0, state.v[18].U16[0], state.v[31].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[18].U16[1], state.v[31].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[18].U16[2], state.v[31].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[18].U16[3], state.v[31].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[18].U16[4], state.v[31].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[18].U16[5], state.v[31].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[18].U16[6], state.v[31].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[18].U16[7], state.v[31].U16[7], s2value, 1, 1, 0, 1)
	}


_04001B18:	// 4001b18: <22100010>	ADDI      S0 = S0 + 0010
	state.r[S0] = state.r[S0] + 0x0010;


_04001B1C:	// 4001b1c: <4b00fcce>	VMADN     vec19 = ( acc+= vec31 * vec00[0] )
	{
		_u16		s2value = state.v[0].U16[7];
		VMADN_operation(0, state.v[19].U16[0], state.v[31].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[19].U16[1], state.v[31].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[19].U16[2], state.v[31].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[19].U16[3], state.v[31].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[19].U16[4], state.v[31].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[19].U16[5], state.v[31].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[19].U16[6], state.v[31].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[19].U16[7], state.v[31].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B20:	// 4001b20: <4bd8a400>	VMULF     vec16 = ( acc = (vec20 * vec24[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[1];
		VMULF_operation(0, state.v[16].U16[0], state.v[20].U16[0], s2value, 1, 1, 1, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[20].U16[1], s2value, 1, 1, 1, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[20].U16[2], s2value, 1, 1, 1, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[20].U16[3], s2value, 1, 1, 1, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[20].U16[4], s2value, 1, 1, 1, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[20].U16[5], s2value, 1, 1, 1, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[20].U16[6], s2value, 1, 1, 1, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[20].U16[7], s2value, 1, 1, 1, 1)
	}


_04001B24:	// 4001b24: <06c10003>	BGEZ ?-?
	sp_reg_pc = 0x04001B24;
	if (state.r[S6] >= 0)
	{
		{
			_u16		s2value = state.v[24].U16[0];
			VMULF_operation(0, state.v[15].U16[0], state.v[20].U16[0], s2value, 1, 1, 1, 1)
			VMULF_operation(1, state.v[15].U16[1], state.v[20].U16[1], s2value, 1, 1, 1, 1)
			VMULF_operation(2, state.v[15].U16[2], state.v[20].U16[2], s2value, 1, 1, 1, 1)
			VMULF_operation(3, state.v[15].U16[3], state.v[20].U16[3], s2value, 1, 1, 1, 1)
			VMULF_operation(4, state.v[15].U16[4], state.v[20].U16[4], s2value, 1, 1, 1, 1)
			VMULF_operation(5, state.v[15].U16[5], state.v[20].U16[5], s2value, 1, 1, 1, 1)
			VMULF_operation(6, state.v[15].U16[6], state.v[20].U16[6], s2value, 1, 1, 1, 1)
			VMULF_operation(7, state.v[15].U16[7], state.v[20].U16[7], s2value, 1, 1, 1, 1)
		}
		goto _04001B34;
	}


_04001B28:	// 4001b28: <4bf8a3c0>	VMULF     vec15 = ( acc = (vec20 * vec24[7]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[0];
		VMULF_operation(0, state.v[15].U16[0], state.v[20].U16[0], s2value, 1, 1, 1, 1)
		VMULF_operation(1, state.v[15].U16[1], state.v[20].U16[1], s2value, 1, 1, 1, 1)
		VMULF_operation(2, state.v[15].U16[2], state.v[20].U16[2], s2value, 1, 1, 1, 1)
		VMULF_operation(3, state.v[15].U16[3], state.v[20].U16[3], s2value, 1, 1, 1, 1)
		VMULF_operation(4, state.v[15].U16[4], state.v[20].U16[4], s2value, 1, 1, 1, 1)
		VMULF_operation(5, state.v[15].U16[5], state.v[20].U16[5], s2value, 1, 1, 1, 1)
		VMULF_operation(6, state.v[15].U16[6], state.v[20].U16[6], s2value, 1, 1, 1, 1)
		VMULF_operation(7, state.v[15].U16[7], state.v[20].U16[7], s2value, 1, 1, 1, 1)
	}


_04001B2C:	// 4001b2c: <090006ce>	J         04001b38
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);
	goto _04001B38;


_04001B30:	// 4001b30: <4b7894a3>	VGE       vec18 = (vec18 >= vec24[3])
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001B34:	// 4001b34: <4b7894a0>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001B38:	// 4001b38: <4baaef40>	VMULF     vec29 = ( acc = (vec29 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[29].U16[0], state.v[29].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[29].U16[1], state.v[29].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[29].U16[2], state.v[29].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[29].U16[3], state.v[29].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[29].U16[4], state.v[29].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[29].U16[5], state.v[29].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[29].U16[6], state.v[29].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[29].U16[7], state.v[29].U16[7], s2value, 0, 1, 1, 1)
	}


_04001B3C:	// 4001b3c: <4a108f48>	VMACF     vec29 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[29].U16[0], state.v[17].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VMACF_operation(1, state.v[29].U16[1], state.v[17].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VMACF_operation(2, state.v[29].U16[2], state.v[17].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VMACF_operation(3, state.v[29].U16[3], state.v[17].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VMACF_operation(4, state.v[29].U16[4], state.v[17].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VMACF_operation(5, state.v[29].U16[5], state.v[17].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VMACF_operation(6, state.v[29].U16[6], state.v[17].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VMACF_operation(7, state.v[29].U16[7], state.v[17].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
	}


_04001B40:	// 4001b40: <4baadec0>	VMULF     vec27 = ( acc = (vec27 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[27].U16[0], state.v[27].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[27].U16[1], state.v[27].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[27].U16[2], state.v[27].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[27].U16[3], state.v[27].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[27].U16[4], state.v[27].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[27].U16[5], state.v[27].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[27].U16[6], state.v[27].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[27].U16[7], state.v[27].U16[7], s2value, 0, 1, 1, 1)
	}


_04001B44:	// 4001b44: <4a0f8ec8>	VMACF     vec27 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[27].U16[0], state.v[17].U16[0], state.v[15].U16[0], 1, 0, 0, 1)
		VMACF_operation(1, state.v[27].U16[1], state.v[17].U16[1], state.v[15].U16[1], 1, 0, 0, 1)
		VMACF_operation(2, state.v[27].U16[2], state.v[17].U16[2], state.v[15].U16[2], 1, 0, 0, 1)
		VMACF_operation(3, state.v[27].U16[3], state.v[17].U16[3], state.v[15].U16[3], 1, 0, 0, 1)
		VMACF_operation(4, state.v[27].U16[4], state.v[17].U16[4], state.v[15].U16[4], 1, 0, 0, 1)
		VMACF_operation(5, state.v[27].U16[5], state.v[17].U16[5], state.v[15].U16[5], 1, 0, 0, 1)
		VMACF_operation(6, state.v[27].U16[6], state.v[17].U16[6], state.v[15].U16[6], 1, 0, 0, 1)
		VMACF_operation(7, state.v[27].U16[7], state.v[17].U16[7], state.v[15].U16[7], 1, 0, 0, 1)
	}


_04001B48:	// 4001b48: <4a11446c>	VXOR      vec17 = vec08 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[8], &state.v[17]);


_04001B4C:	// 4001b4c: <4bd89400>	VMULF     vec16 = ( acc = (vec18 * vec24[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[1];
		VMULF_operation(0, state.v[16].U16[0], state.v[18].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[18].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[18].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[18].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[18].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[18].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[18].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[18].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B50:	// 4001b50: <4bf893c0>	VMULF     vec15 = ( acc = (vec18 * vec24[7]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[0];
		VMULF_operation(0, state.v[15].U16[0], state.v[18].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[15].U16[1], state.v[18].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[15].U16[2], state.v[18].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[15].U16[3], state.v[18].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[15].U16[4], state.v[18].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[15].U16[5], state.v[18].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[15].U16[6], state.v[18].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[15].U16[7], state.v[18].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B54:	// 4001b54: <eabd2000>	SQV       [S5 + 0x00] = vec29 <00>
	{
		_u32    addr = (0x00000000 + state.r[S5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[29].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[29].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[29].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[29].U32[3];
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

			value1 |= (state.v[29].U64[0] & temp1);
			value2 |= (state.v[29].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001B58:	// 4001b58: <4baae700>	VMULF     vec28 = ( acc = (vec28 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[28].U16[0], state.v[28].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[28].U16[1], state.v[28].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[28].U16[2], state.v[28].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[28].U16[3], state.v[28].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[28].U16[4], state.v[28].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[28].U16[5], state.v[28].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[28].U16[6], state.v[28].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[28].U16[7], state.v[28].U16[7], s2value, 0, 1, 1, 1)
	}


_04001B5C:	// 4001b5c: <22b50010>	ADDI      S5 = S5 + 0010
	state.r[S5] = state.r[S5] + 0x0010;


_04001B60:	// 4001b60: <4a108f08>	VMACF     vec28 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[28].U16[0], state.v[17].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VMACF_operation(1, state.v[28].U16[1], state.v[17].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VMACF_operation(2, state.v[28].U16[2], state.v[17].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VMACF_operation(3, state.v[28].U16[3], state.v[17].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VMACF_operation(4, state.v[28].U16[4], state.v[17].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VMACF_operation(5, state.v[28].U16[5], state.v[17].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VMACF_operation(6, state.v[28].U16[6], state.v[17].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VMACF_operation(7, state.v[28].U16[7], state.v[17].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
	}


_04001B64:	// 4001b64: <ea7b2000>	SQV       [S3 + 0x00] = vec27 <00>
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


_04001B68:	// 4001b68: <4baad680>	VMULF     vec26 = ( acc = (vec26 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[26].U16[0], state.v[26].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[26].U16[1], state.v[26].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[26].U16[2], state.v[26].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[26].U16[3], state.v[26].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[26].U16[4], state.v[26].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[26].U16[5], state.v[26].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[26].U16[6], state.v[26].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[26].U16[7], state.v[26].U16[7], s2value, 0, 1, 1, 1)
	}


_04001B6C:	// 4001b6c: <22730010>	ADDI      S3 = S3 + 0010
	state.r[S3] = state.r[S3] + 0x0010;


_04001B70:	// 4001b70: <4a0f8e88>	VMACF     vec26 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[26].U16[0], state.v[17].U16[0], state.v[15].U16[0], 1, 1, 1, 1)
		VMACF_operation(1, state.v[26].U16[1], state.v[17].U16[1], state.v[15].U16[1], 1, 1, 1, 1)
		VMACF_operation(2, state.v[26].U16[2], state.v[17].U16[2], state.v[15].U16[2], 1, 1, 1, 1)
		VMACF_operation(3, state.v[26].U16[3], state.v[17].U16[3], state.v[15].U16[3], 1, 1, 1, 1)
		VMACF_operation(4, state.v[26].U16[4], state.v[17].U16[4], state.v[15].U16[4], 1, 1, 1, 1)
		VMACF_operation(5, state.v[26].U16[5], state.v[17].U16[5], state.v[15].U16[5], 1, 1, 1, 1)
		VMACF_operation(6, state.v[26].U16[6], state.v[17].U16[6], state.v[15].U16[6], 1, 1, 1, 1)
		VMACF_operation(7, state.v[26].U16[7], state.v[17].U16[7], state.v[15].U16[7], 1, 1, 1, 1)
	}


_04001B74:	// 4001b74: <ea9c2000>	SQV       [S4 + 0x00] = vec28 <00>
	{
		_u32    addr = (0x00000000 + state.r[S4]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[28].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[28].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[28].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[28].U32[3];
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

			value1 |= (state.v[28].U64[0] & temp1);
			value2 |= (state.v[28].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001B78:	// 4001b78: <22940010>	ADDI      S4 = S4 + 0010
	state.r[S4] = state.r[S4] + 0x0010;


_04001B7C:	// 4001b7c: <ea5a2000>	SQV       [S2 + 0x00] = vec26 <00>
	{
		_u32    addr = (0x00000000 + state.r[S2]);
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


_04001B80:	// 4001b80: <22520010>	ADDI      S2 = S2 + 0010
	state.r[S2] = state.r[S2] + 0x0010;


_04001B84:	// 4001b84: <4b58ad54>	VADDC	vec21 = vec21 + vec24[2]
	{
		_u16		flag0 = 0;
		_u16		s2value = state.v[24].U16[5];
		VADDC_operation(0, state.v[21].U16[0], state.v[21].U16[0], s2value, 1, 0, 0, 1)
		VADDC_operation(1, state.v[21].U16[1], state.v[21].U16[1], s2value, 1, 0, 0, 1)
		VADDC_operation(2, state.v[21].U16[2], state.v[21].U16[2], s2value, 1, 0, 0, 1)
		VADDC_operation(3, state.v[21].U16[3], state.v[21].U16[3], s2value, 1, 0, 0, 1)
		VADDC_operation(4, state.v[21].U16[4], state.v[21].U16[4], s2value, 1, 0, 0, 1)
		VADDC_operation(5, state.v[21].U16[5], state.v[21].U16[5], s2value, 1, 0, 0, 1)
		VADDC_operation(6, state.v[21].U16[6], state.v[21].U16[6], s2value, 1, 0, 0, 1)
		VADDC_operation(7, state.v[21].U16[7], state.v[21].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = flag0;
	}


_04001B88:	// 4001b88: <4b38a510>	VADD      vec20 = vec20 + vec24[1]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[24].U16[6];
		VADD_operation(0, state.v[20].U16[0], state.v[20].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[20].U16[1], state.v[20].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[20].U16[2], state.v[20].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[20].U16[3], state.v[20].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[20].U16[4], state.v[20].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[20].U16[5], state.v[20].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[20].U16[6], state.v[20].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[20].U16[7], state.v[20].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B8C:	// 4001b8c: <cabd2000>	LQV       vec29 <00> = [S5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S5]);
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


_04001B90:	// 4001b90: <4bb89cd4>	VADDC	vec19 = vec19 + vec24[5]
	{
		_u16		flag0 = 0;
		_u16		s2value = state.v[24].U16[2];
		VADDC_operation(0, state.v[19].U16[0], state.v[19].U16[0], s2value, 1, 0, 0, 1)
		VADDC_operation(1, state.v[19].U16[1], state.v[19].U16[1], s2value, 1, 0, 0, 1)
		VADDC_operation(2, state.v[19].U16[2], state.v[19].U16[2], s2value, 1, 0, 0, 1)
		VADDC_operation(3, state.v[19].U16[3], state.v[19].U16[3], s2value, 1, 0, 0, 1)
		VADDC_operation(4, state.v[19].U16[4], state.v[19].U16[4], s2value, 1, 0, 0, 1)
		VADDC_operation(5, state.v[19].U16[5], state.v[19].U16[5], s2value, 1, 0, 0, 1)
		VADDC_operation(6, state.v[19].U16[6], state.v[19].U16[6], s2value, 1, 0, 0, 1)
		VADDC_operation(7, state.v[19].U16[7], state.v[19].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = flag0;
	}


_04001B94:	// 4001b94: <ca112000>	LQV       vec17 <00> = [S0 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S0]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[17].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[17].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[17].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[17].U32[3] = *(_u32 *)&pDMEM[addr];
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

			state.v[17].U64[0] = state.v[17].U64[0] & ~temp1;
			state.v[17].U64[1] = state.v[17].U64[1] & ~temp2;
			state.v[17].U64[0] = state.v[17].U64[0] | value1;
			state.v[17].U64[1] = state.v[17].U64[1] | value2;
		}
	}


_04001B98:	// 4001b98: <06e10003>	BGEZ ?-?
	sp_reg_pc = 0x04001B98;
	if (state.r[S7] >= 0)
	{
		{
			_u16		flag0 = state.flag[0];
			_u16		s2value = state.v[24].U16[3];
			VADD_operation(0, state.v[18].U16[0], state.v[18].U16[0], s2value, 1, 1, 1, 1)
			VADD_operation(1, state.v[18].U16[1], state.v[18].U16[1], s2value, 1, 1, 1, 1)
			VADD_operation(2, state.v[18].U16[2], state.v[18].U16[2], s2value, 1, 1, 1, 1)
			VADD_operation(3, state.v[18].U16[3], state.v[18].U16[3], s2value, 1, 1, 1, 1)
			VADD_operation(4, state.v[18].U16[4], state.v[18].U16[4], s2value, 1, 1, 1, 1)
			VADD_operation(5, state.v[18].U16[5], state.v[18].U16[5], s2value, 1, 1, 1, 1)
			VADD_operation(6, state.v[18].U16[6], state.v[18].U16[6], s2value, 1, 1, 1, 1)
			VADD_operation(7, state.v[18].U16[7], state.v[18].U16[7], s2value, 1, 1, 1, 1)
			state.flag[0] = 0x0000;
		}
		goto _04001BA8;
	}


_04001B9C:	// 4001b9c: <4b989490>	VADD      vec18 = vec18 + vec24[4]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[24].U16[3];
		VADD_operation(0, state.v[18].U16[0], state.v[18].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[18].U16[1], state.v[18].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[18].U16[2], state.v[18].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[18].U16[3], state.v[18].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[18].U16[4], state.v[18].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[18].U16[5], state.v[18].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[18].U16[6], state.v[18].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[18].U16[7], state.v[18].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001BA0:	// 4001ba0: <090006eb>	J         04001bac
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);
	goto _04001BAC;


_04001BA4:	// 4001ba4: <4b18a523>	VGE       vec20 = (vec20 >= vec24[0])
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001BA8:	// 4001ba8: <4b18a520>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001BAC:	// 4001bac: <4a114c6c>	VXOR      vec17 = vec09 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[9], &state.v[17]);


_04001BB0:	// 4001bb0: <06c10003>	BGEZ ?-?
	sp_reg_pc = 0x04001BB0;
	if (state.r[S6] >= 0)
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
		goto _04001BC0;
	}


_04001BB4:	// 4001bb4: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
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


_04001BB8:	// 4001bb8: <090006f1>	J         04001bc4
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);
	goto _04001BC4;


_04001BBC:	// 4001bbc: <4b7894a3>	VGE       vec18 = (vec18 >= vec24[3])
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001BC0:	// 4001bc0: <4b7894a0>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001BC4:	// 4001bc4: <4bd8a400>	VMULF     vec16 = ( acc = (vec20 * vec24[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[1];
		VMULF_operation(0, state.v[16].U16[0], state.v[20].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[20].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[20].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[20].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[20].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[20].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[20].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[20].U16[7], s2value, 1, 0, 0, 1)
	}


_04001BC8:	// 4001bc8: <eb142000>	SQV       [T8 + 0x00] = vec20 <00>
	// In function: Tooie1F20_1F84
	//Call the function: Tooie1F20_1F84
	Tooie1F20_1F84();
	goto _04001C30;

_04001BCC:	// 4001bcc: <4bf8a3c0>	VMULF     vec15 = ( acc = (vec20 * vec24[7]) << 1 ) >> 16
	ErrorToCallFunction(0x04001BCC);

_04001BD0:	// 4001bd0: <eb152001>	SQV       [T8 + 0x10] = vec21 <00>
	ErrorToCallFunction(0x04001BD0);

_04001BD4:	// 4001bd4: <4baaef40>	VMULF     vec29 = ( acc = (vec29 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001BD4);

_04001BD8:	// 4001bd8: <4a108f48>	VMACF     vec29 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001BD8);

_04001BDC:	// 4001bdc: <ca9c2000>	LQV       vec28 <00> = [S4 + 0x00]
	ErrorToCallFunction(0x04001BDC);

_04001BE0:	// 4001be0: <4baadec0>	VMULF     vec27 = ( acc = (vec27 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001BE0);

_04001BE4:	// 4001be4: <ca5a2000>	LQV       vec26 <00> = [S2 + 0x00]
	ErrorToCallFunction(0x04001BE4);

_04001BE8:	// 4001be8: <4a0f8ec8>	VMACF     vec27 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001BE8);

_04001BEC:	// 4001bec: <4a11446c>	VXOR      vec17 = vec08 xor vec17[<none>]
	ErrorToCallFunction(0x04001BEC);

_04001BF0:	// 4001bf0: <2231fff0>	ADDI      S1 = S1 + fff0
	ErrorToCallFunction(0x04001BF0);

_04001BF4:	// 4001bf4: <4b58ad54>	VADDC	vec21 = vec21 + vec24[2]
	ErrorToCallFunction(0x04001BF4);

_04001BF8:	// 4001bf8: <22100010>	ADDI      S0 = S0 + 0010
	ErrorToCallFunction(0x04001BF8);

_04001BFC:	// 4001bfc: <4b38a510>	VADD      vec20 = vec20 + vec24[1]
	ErrorToCallFunction(0x04001BFC);

_04001C00:	// 4001c00: <eabd2000>	SQV       [S5 + 0x00] = vec29 <00>
	ErrorToCallFunction(0x04001C00);

_04001C04:	// 4001c04: <4bd89400>	VMULF     vec16 = ( acc = (vec18 * vec24[6]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C04);

_04001C08:	// 4001c08: <22b50010>	ADDI      S5 = S5 + 0010
	ErrorToCallFunction(0x04001C08);

_04001C0C:	// 4001c0c: <4bf893c0>	VMULF     vec15 = ( acc = (vec18 * vec24[7]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C0C);

_04001C10:	// 4001c10: <ea7b2000>	SQV       [S3 + 0x00] = vec27 <00>
	ErrorToCallFunction(0x04001C10);

_04001C14:	// 4001c14: <4baae700>	VMULF     vec28 = ( acc = (vec28 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C14);

_04001C18:	// 4001c18: <22730010>	ADDI      S3 = S3 + 0010
	ErrorToCallFunction(0x04001C18);

_04001C1C:	// 4001c1c: <4a108f08>	VMACF     vec28 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C1C);

_04001C20:	// 4001c20: <4baad680>	VMULF     vec26 = ( acc = (vec26 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C20);

_04001C24:	// 4001c24: <4a0f8e88>	VMACF     vec26 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C24);

_04001C28:	// 4001c28: <ea9c2000>	SQV       [S4 + 0x00] = vec28 <00>
	ErrorToCallFunction(0x04001C28);

_04001C2C:	// 4001c2c: <22940010>	ADDI      S4 = S4 + 0010
	ErrorToCallFunction(0x04001C2C);

_04001C30:	// 4001c30: <1a200003>	BLEZ      (R0<=0) --> 1c40
	sp_reg_pc = 0x04001C30;
	if ((_s32)state.r[S1] <= 0)
	{
		{
			_u32    addr = (0x00000000 + state.r[S2]);
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
		goto _04001C40;
	}


_04001C34:	// 4001c34: <ea5a2000>	SQV       [S2 + 0x00] = vec26 <00>
	{
		_u32    addr = (0x00000000 + state.r[S2]);
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


_04001C38:	// 4001c38: <090006e3>	J         04001b8c
	state.r[S2] = state.r[S2] + 0x0010;
	goto _04001B8C;


_04001C3C:	// 4001c3c: <22520010>	ADDI      S2 = S2 + 0010
	state.r[S2] = state.r[S2] + 0x0010;


_04001C40:	// 4001c40: <eb122002>	SQV       [T8 + 0x20] = vec18 <00>
	{
		_u32    addr = (0x00000020 + state.r[T8]);
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


_04001C44:	// 4001c44: <eb132003>	SQV       [T8 + 0x30] = vec19 <00>
	{
		_u32    addr = (0x00000030 + state.r[T8]);
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


_04001C48:	// 4001c48: <0d00045d>	JAL	    04001174
	{
		_u32    addr = (0x00000040 + state.r[T8]);
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
	sp_reg_pc = 0x04001C48;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001174;


_04001C4C:	// 4001c4c: <eb182004>	SQV       [T8 + 0x40] = vec24 <00>
	{
		_u32    addr = (0x00000040 + state.r[T8]);
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


_04001C50:	// 4001c50: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_04001C54:	// 4001c54: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001C58:	// 4001c58: <c81f2005>	LQV       vec31 <00> = [R0 + 0x50]
	// In function: SmashBros1C84_1CC8
	//Call the function: SmashBros1C84_1CC8
	SmashBros1C84_1CC8();
	goto _04001CA0;

_04001C5C:	// 4001c5c: <3336ffff>	ANDI      S6 = T9 & ffff
	ErrorToCallFunction(0x04001C5C);

_04001C60:	// 4001c60: <22d604f0>	ADDI      S6 = S6 + 04f0
	ErrorToCallFunction(0x04001C60);

_04001C64:	// 4001c64: <cadc2000>	LQV       vec28 <00> = [S6 + 0x00]
	ErrorToCallFunction(0x04001C64);

_04001C68:	// 4001c68: <0019bc02>	SRL       S7 = T9 >> 16
	ErrorToCallFunction(0x04001C68);

_04001C6C:	// 4001c6c: <22f704f0>	ADDI      S7 = S7 + 04f0
	ErrorToCallFunction(0x04001C6C);

_04001C70:	// 4001c70: <cafd2000>	LQV       vec29 <00> = [S7 + 0x00]
	ErrorToCallFunction(0x04001C70);

_04001C74:	// 4001c74: <489af000>	MTC2      vec30 <00> = K0
	ErrorToCallFunction(0x04001C74);

_04001C78:	// 4001c78: <20150170>	ADDI      S5 = R0 + 0170
	ErrorToCallFunction(0x04001C78);

_04001C7C:	// 4001c7c: <4bbfe6c0>	VMULF     vec27 = ( acc = (vec28 * vec31[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C7C);

_04001C80:	// 4001c80: <22b5fff0>	ADDI      S5 = S5 + fff0
	ErrorToCallFunction(0x04001C80);

_04001C84:	// 4001c84: <22f70010>	ADDI      S7 = S7 + 0010
	ErrorToCallFunction(0x04001C84);

_04001C88:	// 4001c88: <22d60010>	ADDI      S6 = S6 + 0010
	ErrorToCallFunction(0x04001C88);

_04001C8C:	// 4001c8c: <4b1eeec8>	VMACF     vec27 = ( acc += (vec29 * vec30[0]) << 1 ) >> 16
	ErrorToCallFunction(0x04001C8C);

_04001C90:	// 4001c90: <cadc2000>	LQV       vec28 <00> = [S6 + 0x00]
	ErrorToCallFunction(0x04001C90);

_04001C94:	// 4001c94: <cafd2000>	LQV       vec29 <00> = [S7 + 0x00]
	ErrorToCallFunction(0x04001C94);

_04001C98:	// 4001c98: <1ea0fff8>	BGTZ      (R0>0) --> 1c7c
	ErrorToCallFunction(0x04001C98);

_04001C9C:	// 4001c9c: <eadb207f>	SQV       [S6 + 0xf0] = vec27 <00>
	ErrorToCallFunction(0x04001C9C);

_04001CA0:	// 4001ca0: <0900043a>	J         040010e8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _040010E8;


_04001CA4:	// 4001ca4: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001CA8:	// 4001ca8: <00000000>	NOP       


_04001CAC:	// 4001cac: <00000000>	NOP       


_04001CB0:	// 4001cb0: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_04001CB4:	// 4001cb4: <c81f201b>	LQV       vec31 <00> = [R0 + 0xb0]
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


_04001CB8:	// 4001cb8: <c81e201c>	LQV       vec30 <00> = [R0 + 0xc0]
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


_04001CBC:	// 4001cbc: <20170ba8>	ADDI      S7 = R0 + 0ba8
	state.r[S7] = 0 + 0x0BA8;


_04001CC0:	// 4001cc0: <20160d00>	ADDI      S6 = R0 + 0d00
	state.r[S6] = 0 + 0x0D00;


_04001CC4:	// 4001cc4: <4b1f0051>	VSUB      vec01 = vec00 - vec31[0]
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


_04001CC8:	// 4001cc8: <8c0b00f0>	LW        T3 = [R0+00f0]
	state.r[T3] = (_s32)Load32_DMEM((0 + 0x000000F0) & 0xfff);


_04001CCC:	// 4001ccc: <8c0c0fc4>	LW        T4 = [R0+0fc4]
	state.r[T4] = (_s32)Load32_DMEM((0 + 0x00000FC4) & 0xfff);


_04001CD0:	// 4001cd0: <20012800>	ADDI      AT = R0 + 2800
	state.r[AT] = 0 + 0x2800;


_04001CD4:	// 4001cd4: <11600006>	BEQ       (R0==T3) --> 1cf0
	sp_reg_pc = 0x04001CD4;
	if (state.r[T3] == 0)
	{
		goto _04001CF0;
	}


_04001CD8:	// 4001cd8: <40812000>	MTC0      SP status = AT


_04001CDC:	// 4001cdc: <318c0001>	ANDI      T4 = T4 & 0001
	state.r[T4] = (state.r[T4] & 0x00000001);


_04001CE0:	// 4001ce0: <1180001e>	BEQ       (R0==T4) --> 1d5c
	sp_reg_pc = 0x04001CE0;
	if (state.r[T4] == 0)
	{
		Save32_DMEM((_u32)0, (0 + 0x00000FC4) & 0xfff);
		goto _04001D5C;
	}


_04001CE4:	// 4001ce4: <ac000fc4>	SW        [R0+0fc4] = R0
	Save32_DMEM((_u32)0, (0 + 0x00000FC4) & 0xfff);


_04001CE8:	// 4001ce8: <08000459>	J         00001164
	state.r[K0] = (_s32)Load32_DMEM((0 + 0x00000BF8) & 0xfff);
	state.halt = 1;
	return;


_04001CEC:	// 4001cec: <8c1a0bf8>	LW        K0 = [R0+0bf8]
	state.r[K0] = (_s32)Load32_DMEM((0 + 0x00000BF8) & 0xfff);


_04001CF0:	// 4001cf0: <400b5800>	MFC0      T3 = DP CMD status
	state.r[T3] = 0;


_04001CF4:	// 4001cf4: <316b0001>	ANDI      T3 = T3 & 0001
	state.r[T3] = (state.r[T3] & 0x00000001);


_04001CF8:	// 4001cf8: <1560000b>	BNE       (R0!=T3) --> 1d28
	sp_reg_pc = 0x04001CF8;
	if (state.r[T3] != 0)
	{
		state.r[V0] = 0;
		goto _04001D28;
	}


_04001CFC:	// 4001cfc: <40024800>	MFC0      T1 = SP read DMA length
	state.r[V0] = 0;


_04001D00:	// 4001d00: <8c030fe8>	LW        V1 = [R0+0fe8]
	state.r[V1] = (_s32)Load32_DMEM((0 + 0x00000FE8) & 0xfff);


_04001D04:	// 4001d04: <00625822>	SUB       T3 = V1-V0
	state.r[T3] = (state.r[V1] - state.r[V0]);


_04001D08:	// 4001d08: <1d600007>	BGTZ      (R0>0) --> 1d28
	sp_reg_pc = 0x04001D08;
	if((_s32)state.r[T3] > 0)
	{
		state.r[AT] = 0;
		goto _04001D28;
	}


_04001D0C:	// 4001d0c: <40015000>	MFC0      T2 = SP DRAM DMA address
	state.r[AT] = 0;


_04001D10:	// 4001d10: <8c040fec>	LW        A0 = [R0+0fec]
	state.r[A0] = (_s32)Load32_DMEM((0 + 0x00000FEC) & 0xfff);


_04001D14:	// 4001d14: <10200004>	BEQ       (R0==AT) --> 1d28
	sp_reg_pc = 0x04001D14;
	if (state.r[AT] == 0)
	{
		state.r[T3] = (state.r[AT] - state.r[A0]);
		goto _04001D28;
	}


_04001D18:	// 4001d18: <00245822>	SUB       T3 = AT-A0
	state.r[T3] = (state.r[AT] - state.r[A0]);


_04001D1C:	// 4001d1c: <05610002>	BGEZ ?-?
	sp_reg_pc = 0x04001D1C;
	if (state.r[T3] >= 0)
	{
		goto _04001D28;
	}


_04001D20:	// 4001d20: <00000000>	NOP       


_04001D24:	// 4001d24: <14220008>	BNE       (V0!=AT) --> 1d48
	sp_reg_pc = 0x04001D24;
	if (state.r[AT] != state.r[V0])
	{
		state.r[T3] = 0;
		goto _04001D48;
	}


_04001D28:	// 4001d28: <400b5800>	MFC0      T3 = DP CMD status
	state.r[T3] = 0;


_04001D2C:	// 4001d2c: <316b0400>	ANDI      T3 = T3 & 0400
	state.r[T3] = (state.r[T3] & 0x00000400);


_04001D30:	// 4001d30: <1560fffd>	BNE       (R0!=T3) --> 1d28
	sp_reg_pc = 0x04001D30;
	if (state.r[T3] != 0)
	{
		state.r[T3] = 0 + 0x0001;
		goto _04001D28;
	}


_04001D34:	// 4001d34: <200b0001>	ADDI      T3 = R0 + 0001
	state.r[T3] = 0 + 0x0001;


_04001D38:	// 4001d38: <408b5800>	MTC0      DP CMD status = T3


_04001D3C:	// 4001d3c: <8c020fec>	LW        V0 = [R0+0fec]
	state.r[V0] = (_s32)Load32_DMEM((0 + 0x00000FEC) & 0xfff);


_04001D40:	// 4001d40: <40824000>	MTC0      DP CMD DMA start = V0


_04001D44:	// 4001d44: <40824800>	MTC0      DP CMD DMA end = V0


_04001D48:	// 4001d48: <ac0200f0>	SW        [R0+00f0] = V0
	Save32_DMEM((_u32)state.r[V0], (0 + 0x000000F0) & 0xfff);


_04001D4C:	// 4001d4c: <8c0b00f4>	LW        T3 = [R0+00f4]
	state.r[T3] = (_s32)Load32_DMEM((0 + 0x000000F4) & 0xfff);


_04001D50:	// 4001d50: <15600002>	BNE       (R0!=T3) --> 1d5c
	sp_reg_pc = 0x04001D50;
	if (state.r[T3] != 0)
	{
		state.r[T3] = (_s32)Load32_DMEM((0 + 0x00000FE0) & 0xfff);
		goto _04001D5C;
	}


_04001D54:	// 4001d54: <8c0b0fe0>	LW        T3 = [R0+0fe0]
	state.r[T3] = (_s32)Load32_DMEM((0 + 0x00000FE0) & 0xfff);


_04001D58:	// 4001d58: <ac0b00f4>	SW        [R0+00f4] = T3
	Save32_DMEM((_u32)state.r[T3], (0 + 0x000000F4) & 0xfff);


_04001D5C:	// 4001d5c: <8c010fd0>	LW        AT = [R0+0fd0]
	state.r[AT] = (_s32)Load32_DMEM((0 + 0x00000FD0) & 0xfff);


_04001D60:	// 4001d60: <8c0202e0>	LW        V0 = [R0+02e0]
	state.r[V0] = (_s32)Load32_DMEM((0 + 0x000002E0) & 0xfff);


_04001D64:	// 4001d64: <8c0302e8>	LW        V1 = [R0+02e8]
	state.r[V1] = (_s32)Load32_DMEM((0 + 0x000002E8) & 0xfff);


_04001D68:	// 4001d68: <8c040410>	LW        A0 = [R0+0410]
	state.r[A0] = (_s32)Load32_DMEM((0 + 0x00000410) & 0xfff);


_04001D6C:	// 4001d6c: <8c050418>	LW        A1 = [R0+0418]
	state.r[A1] = (_s32)Load32_DMEM((0 + 0x00000418) & 0xfff);


_04001D70:	// 4001d70: <00411020>	ADD       V0 = V0+AT
	state.r[V0] = (state.r[V0] + state.r[AT]);


_04001D74:	// 4001d74: <00611820>	ADD       V1 = V1+AT
	state.r[V1] = (state.r[V1] + state.r[AT]);


_04001D78:	// 4001d78: <ac0202e0>	SW        [R0+02e0] = V0
	Save32_DMEM((_u32)state.r[V0], (0 + 0x000002E0) & 0xfff);


_04001D7C:	// 4001d7c: <ac0302e8>	SW        [R0+02e8] = V1
	Save32_DMEM((_u32)state.r[V1], (0 + 0x000002E8) & 0xfff);


_04001D80:	// 4001d80: <00812020>	ADD       A0 = A0+AT
	state.r[A0] = (state.r[A0] + state.r[AT]);


_04001D84:	// 4001d84: <00a12820>	ADD       A1 = A1+AT
	state.r[A1] = (state.r[A1] + state.r[AT]);


_04001D88:	// 4001d88: <ac040410>	SW        [R0+0410] = A0
	Save32_DMEM((_u32)state.r[A0], (0 + 0x00000410) & 0xfff);


_04001D8C:	// 4001d8c: <ac050418>	SW        [R0+0418] = A1
	Save32_DMEM((_u32)state.r[A1], (0 + 0x00000418) & 0xfff);


_04001D90:	// 4001d90: <8c1a0ff0>	LW        K0 = [R0+0ff0]
	state.r[K0] = (_s32)Load32_DMEM((0 + 0x00000FF0) & 0xfff);


_04001D94:	// 4001d94: <200b02e8>	ADDI      T3 = R0 + 02e8
	state.r[T3] = 0 + 0x02E8;


_04001D98:	// 4001d98: <0c0007ed>	JAL	    00001fb4
	state.r[T4] = (state.r[RA] | 0x00000000);
	sp_reg_pc = 0x04001D98;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	state.halt = 1;
	return;


_04001D9C:	// 4001d9c: <37ec0000>	ORI       T4 = RA | 0x0000
	state.r[T4] = (state.r[RA] | 0x00000000);


_04001DA0:	// 4001da0: <201300a7>	ADDI      S3 = R0 + 00a7
	state.r[S3] = 0 + 0x00A7;


_04001DA4:	// 4001da4: <37580000>	ORI       T8 = K0 | 0x0000
	state.r[T8] = (state.r[K0] | 0x00000000);


_04001DA8:	// 4001da8: <0c0007f6>	JAL	    00001fd8
	state.r[S4] = 0 + 0x0920;
	sp_reg_pc = 0x04001DA8;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	state.halt = 1;
	return;


_04001DAC:	// 4001dac: <24140920>	ADDIU     S4 = R0 + 0920
	state.r[S4] = 0 + 0x0920;


_04001DB0:	// 4001db0: <275a00a8>	ADDIU     K0 = K0 + 00a8
	state.r[K0] = state.r[K0] + 0x00A8;


_04001DB4:	// 4001db4: <201bff58>	ADDI      K1 = R0 + ff58
	state.r[K1] = 0 + 0xFFFFFF58;


_04001DB8:	// 4001db8: <0c0007f2>	JAL	    00001fc8
	state.r[AT] = 0;
	sp_reg_pc = 0x04001DB8;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	state.halt = 1;
	return;


_04001DBC:	// 4001dbc: <40012000>	MFC0      A0 = SP DRAM DMA address
	state.r[AT] = 0;


_04001DC0:	// 4001dc0: <8f7909c8>	LW        T9 = [K1+09c8]
	state.r[T9] = (_s32)Load32_DMEM((state.r[K1] + 0x000009C8) & 0xfff);


_04001DC4:	// 4001dc4: <1360fff6>	BEQ       (R0==K1) --> 1da0
	sp_reg_pc = 0x04001DC4;
	if (state.r[K1] == 0)
	{
		state.r[AT] = (state.r[AT] & 0x00000080);
		goto _04001DA0;
	}


_04001DC8:	// 4001dc8: <30210080>	ANDI      AT = AT & 0080
	state.r[AT] = (state.r[AT] & 0x00000080);


_04001DCC:	// 4001dcc: <00196603>	SRA       T4 = T9 >> 24
	state.r[T4] = ((_s32)state.r[T9] >> 24);


_04001DD0:	// 4001dd0: <000c5840>	SLL       T3 = T4 << 1
	state.r[T3] = ((_u32)state.r[T4] << 1);


_04001DD4:	// 4001dd4: <956b036e>	LHU       T3 = [T3+036e]
	state.r[T3] = (_s32)(_u16)Load16_DMEM((state.r[T3] + 0x0000036E) & 0xfff);


_04001DD8:	// 4001dd8: <14200380>	BNE       (R0!=AT) --> 2bdc
	sp_reg_pc = 0x04001DD8;
	if (state.r[AT] != 0)
	{
		state.r[T8] = (_s32)Load32_DMEM((state.r[K1] + 0x000009CC) & 0xfff);
		state.halt = 1;
		return;
	}


_04001DDC:	// 4001ddc: <8f7809cc>	LW        T8 = [K1+09cc]
	state.r[T8] = (_s32)Load32_DMEM((state.r[K1] + 0x000009CC) & 0xfff);


_04001DE0:	// 4001de0: <01600008>	JR        T3
	{
		_u32	temp = ((state.r[T3] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[K1] = state.r[K1] + 0x0008;
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001DE4:	// 4001de4: <277b0008>	ADDIU     K1 = K1 + 0008
	state.r[K1] = state.r[K1] + 0x0008;


_04001DE8:	// 4001de8: <0c000487>	JAL	    0000121c
	state.r[S4] = (_s32)(_s16)Load16_DMEM((state.r[K1] + 0x000009C1) & 0xfff);
	sp_reg_pc = 0x04001DE8;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	state.halt = 1;
	return;


_04001DEC:	// 4001dec: <877409c1>	LH        S4 = [K1+09c1]
	state.r[S4] = (_s32)(_s16)Load16_DMEM((state.r[K1] + 0x000009C1) & 0xfff);


_04001DF0:	// 4001df0: <33330ff8>	ANDI      S3 = T9 & 0ff8
	state.r[S3] = (state.r[T9] & 0x00000FF8);


_04001DF4:	// 4001df4: <0014a083>	SRA       S4 = S4 >> 2
	state.r[S4] = ((_s32)state.r[S4] >> 2);


_04001DF8:	// 4001df8: <080007f6>	J         00001fd8
	state.r[RA] = 0 + 0x1188;
	state.halt = 1;
	return;


_04001DFC:	// 4001dfc: <201f1188>	ADDI      RA = R0 + 1188
	state.r[RA] = 0 + 0x1188;


_04001E00:	// 4001e00: <8c0b01ec>	LW        T3 = [R0+01ec]
	state.r[T3] = (_s32)Load32_DMEM((0 + 0x000001EC) & 0xfff);


_04001E04:	// 4001e04: <01795824>	AND       T3 = T3 & T9
	state.r[T3] = (state.r[T3] & state.r[T9]);


_04001E08:	// 4001e08: <01785825>	OR        T3 = T3 | T8
	state.r[T3] = (state.r[T3] | state.r[T8]);


_04001E0C:	// 4001e0c: <08000463>	J         0000118c
	Save32_DMEM((_u32)state.r[T3], (0 + 0x000001EC) & 0xfff);
	state.halt = 1;
	return;


_04001E10:	// 4001e10: <ac0b01ec>	SW        [R0+01ec] = T3
	Save32_DMEM((_u32)state.r[T3], (0 + 0x000001EC) & 0xfff);


_04001E14:	// 4001e14: <900100de>	LBU       AT = [R0+00de]
	state.r[AT] = (_s32)(_u8)pDMEM[((0 + 0x000000DE) & 0xfff) ^ 3];


_04001E18:	// 4001e18: <10200370>	BEQ       (R0==AT) --> 2bdc
	sp_reg_pc = 0x04001E18;
	if (state.r[AT] == 0)
	{
		state.r[AT] = state.r[AT] + 0xFFFFFFFC;
		state.halt = 1;
		return;
	}


_04001E1C:	// 4001e1c: <2021fffc>	ADDI      AT = AT + fffc
	state.r[AT] = state.r[AT] + 0xFFFFFFFC;


_04001E20:	// 4001e20: <08000408>	J         00001020
	state.r[K0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000138) & 0xfff);
	state.halt = 1;
	return;


_04001E24:	// 4001e24: <8c3a0138>	LW        K0 = [AT+0138]
	state.r[K0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000138) & 0xfff);


_04001E28:	// 4001e28: <c81d181a>	LDV       vec29 <00> = [R0 + 0x50]
	rsp_ldv_compiler((0 + 0x000000D0) & 0xFFF, 29, 8);


_04001E2C:	// 4001e2c: <8c1900d8>	LW        T9 = [R0+00d8]
	state.r[T9] = (_s32)Load32_DMEM((0 + 0x000000D8) & 0xfff);


_04001E30:	// 4001e30: <22f70008>	ADDI      S7 = S7 + 0008
	state.r[S7] = state.r[S7] + 0x0008;


_04001E34:	// 4001e34: <eafd187f>	SDV       [S7 + 0x3c] = vec29 <00>
	{
		_u32	addr = (0xFFFFFFF8 + state.r[S7]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[29].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[29].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[29].U32[0x00000003];
		}
	}


_04001E38:	// 4001e38: <aef80004>	SW        [S7+0004] = T8
	Save32_DMEM((_u32)state.r[T8], (state.r[S7] + 0x00000004) & 0xfff);


_04001E3C:	// 4001e3c: <aef90000>	SW        [S7+0000] = T9
	Save32_DMEM((_u32)state.r[T9], (state.r[S7] + 0x00000000) & 0xfff);


_04001E40:	// 4001e40: <08000494>	J         00001250
	state.r[S7] = state.r[S7] + 0x0008;
	state.halt = 1;
	return;


_04001E44:	// 4001e44: <22f70008>	ADDI      S7 = S7 + 0008
	state.r[S7] = state.r[S7] + 0x0008;


_04001E48:	// 4001e48: <201f1208>	ADDI      RA = R0 + 1208
	state.r[RA] = 0 + 0x1208;


_04001E4C:	// 4001e4c: <00185d82>	SRL       T3 = T8 >> 22
	state.r[T3] = ((_u32)state.r[T8] >> 22);


_04001E50:	// 4001e50: <316b003c>	ANDI      T3 = T3 & 003c
	state.r[T3] = (state.r[T3] & 0x0000003C);


_04001E54:	// 4001e54: <8d6b00f8>	LW        T3 = [T3+00f8]
	state.r[T3] = (_s32)Load32_DMEM((state.r[T3] + 0x000000F8) & 0xfff);


_04001E58:	// 4001e58: <0018c200>	SLL       T8 = T8 << 8
	state.r[T8] = ((_u32)state.r[T8] << 8);


_04001E5C:	// 4001e5c: <0018c202>	SRL       T8 = T8 >> 8
	state.r[T8] = ((_u32)state.r[T8] >> 8);


_04001E60:	// 4001e60: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[T8] = (state.r[T8] + state.r[T3]);
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001E64:	// 4001e64: <030bc020>	ADD       T8 = T8+T3
	state.r[T8] = (state.r[T8] + state.r[T3]);


_04001E68:	// 4001e68: <ac1900c8>	SW        [R0+00c8] = T9
	Save32_DMEM((_u32)state.r[T9], (0 + 0x000000C8) & 0xfff);


_04001E6C:	// 4001e6c: <08000482>	J         00001208
	Save32_DMEM((_u32)state.r[T8], (0 + 0x000000CC) & 0xfff);
	state.halt = 1;
	return;


_04001E70:	// 4001e70: <ac1800cc>	SW        [R0+00cc] = T8
	Save32_DMEM((_u32)state.r[T8], (0 + 0x000000CC) & 0xfff);


_04001E74:	// 4001e74: <ac1900c0>	SW        [R0+00c0] = T9
	Save32_DMEM((_u32)state.r[T9], (0 + 0x000000C0) & 0xfff);


_04001E78:	// 4001e78: <08000482>	J         00001208
	Save32_DMEM((_u32)state.r[T8], (0 + 0x000000C4) & 0xfff);
	state.halt = 1;
	return;


_04001E7C:	// 4001e7c: <ac1800c4>	SW        [R0+00c4] = T8
	Save32_DMEM((_u32)state.r[T8], (0 + 0x000000C4) & 0xfff);


_04001E80:	// 4001e80: <201f118c>	ADDI      RA = R0 + 118c
	state.r[RA] = 0 + 0x118C;


_04001E84:	// 4001e84: <02f65822>	SUB       T3 = S7-S6
	state.r[T3] = (state.r[S7] - state.r[S6]);


_04001E88:	// 4001e88: <1960035e>	BLEZ      (R0<=0) --> 2c04
	sp_reg_pc = 0x04001E88;
	if ((_s32)state.r[T3] <= 0)
	{
		state.r[T4] = 0;
		state.halt = 1;
		return;
	}


_04001E8C:	// 4001e8c: <400c3000>	MFC0      A2 = DP clock counter
	state.r[T4] = 0;


_04001E90:	// 4001e90: <8c1800f0>	LW        T8 = [R0+00f0]
	state.r[T8] = (_s32)Load32_DMEM((0 + 0x000000F0) & 0xfff);


_04001E94:	// 4001e94: <25730158>	ADDIU     S3 = T3 + 0158
	state.r[S3] = state.r[T3] + 0x0158;


_04001E98:	// 4001e98: <1580fffc>	BNE       (R0!=T4) --> 1e8c
	sp_reg_pc = 0x04001E98;
	if (state.r[T4] != 0)
	{
		state.r[T4] = (_s32)Load32_DMEM((0 + 0x00000FEC) & 0xfff);
		goto _04001E8C;
	}


_04001E9C:	// 4001e9c: <8c0c0fec>	LW        T4 = [R0+0fec]
	state.r[T4] = (_s32)Load32_DMEM((0 + 0x00000FEC) & 0xfff);


_04001EA0:	// 4001ea0: <40984800>	MTC0      DP CMD DMA end = T8


_04001EA4:	// 4001ea4: <03135820>	ADD       T3 = T8+S3
	state.r[T3] = (state.r[T8] + state.r[S3]);


_04001EA8:	// 4001ea8: <018b6022>	SUB       T4 = T4-T3
	state.r[T4] = (state.r[T4] - state.r[T3]);


_04001EAC:	// 4001eac: <05810008>	BGEZ ?-?
	sp_reg_pc = 0x04001EAC;
	if (state.r[T4] >= 0)
	{
		state.r[T3] = 0;
		goto _04001ED0;
	}


_04001EB0:	// 4001eb0: <400b5800>	MFC0      T3 = DP CMD status
	state.r[T3] = 0;


_04001EB4:	// 4001eb4: <316b0400>	ANDI      T3 = T3 & 0400
	state.r[T3] = (state.r[T3] & 0x00000400);


_04001EB8:	// 4001eb8: <1560fffd>	BNE       (R0!=T3) --> 1eb0
	sp_reg_pc = 0x04001EB8;
	if (state.r[T3] != 0)
	{
		state.r[T8] = (_s32)Load32_DMEM((0 + 0x00000FE8) & 0xfff);
		goto _04001EB0;
	}


_04001EBC:	// 4001ebc: <8c180fe8>	LW        T8 = [R0+0fe8]
	state.r[T8] = (_s32)Load32_DMEM((0 + 0x00000FE8) & 0xfff);


_04001EC0:	// 4001ec0: <400b5000>	MFC0      T2 = DP CMD status
	state.r[T3] = 0;


_04001EC4:	// 4001ec4: <1178fffe>	BEQ       (T8==T3) --> 1ec0
	sp_reg_pc = 0x04001EC4;
	if (state.r[T3] == state.r[T8])
	{
		goto _04001EC0;
	}


_04001EC8:	// 4001ec8: <00000000>	NOP       


_04001ECC:	// 4001ecc: <40984000>	MTC0      DP CMD DMA start = T8


_04001ED0:	// 4001ed0: <400b5000>	MFC0      T2 = DP CMD status
	state.r[T3] = 0;


_04001ED4:	// 4001ed4: <01785822>	SUB       T3 = T3-T8
	state.r[T3] = (state.r[T3] - state.r[T8]);


_04001ED8:	// 4001ed8: <19600002>	BLEZ      (R0<=0) --> 1ee4
	sp_reg_pc = 0x04001ED8;
	if ((_s32)state.r[T3] <= 0)
	{
		state.r[T3] = (state.r[T3] - state.r[S3]);
		goto _04001EE4;
	}


_04001EDC:	// 4001edc: <01735822>	SUB       T3 = T3-S3
	state.r[T3] = (state.r[T3] - state.r[S3]);


_04001EE0:	// 4001ee0: <1960fffb>	BLEZ      (R0<=0) --> 1ed0
	sp_reg_pc = 0x04001EE0;
	if ((_s32)state.r[T3] <= 0)
	{
		state.r[T3] = (state.r[T8] + state.r[S3]);
		goto _04001ED0;
	}


_04001EE4:	// 4001ee4: <03135820>	ADD       T3 = T8+S3
	state.r[T3] = (state.r[T8] + state.r[S3]);

_04001EE8:
_04001EEC:
_04001EF0:
_04001EF4:
_04001EF8:
_04001EFC:
_04001F00:
_04001F04:
_04001F08:
_04001F0C:
_04001F10:
_04001F14:
_04001F18:
_04001F1C:
_04001F20:
_04001F24:
_04001F28:
_04001F2C:
_04001F30:
_04001F34:
_04001F38:
_04001F3C:
_04001F40:
_04001F44:
_04001F48:
_04001F4C:
_04001F50:
_04001F54:
_04001F58:
_04001F5C:
_04001F60:
_04001F64:
_04001F68:
_04001F6C:
_04001F70:
_04001F74:
_04001F78:
_04001F7C:
_04001F80:
_04001F84:
_04001F88:
_04001F8C:
_04001F90:
_04001F94:
_04001F98:
_04001F9C:
_04001FA0:
_04001FA4:
_04001FA8:
_04001FAC:
_04001FB0:
_04001FB4:
_04001FB8:
_04001FBC:
_04001FC0:
_04001FC4:
_04001FC8:
_04001FCC:
_04001FD0:
_04001FD4:
_04001FD8:
_04001FDC:
_04001FE0:
_04001FE4:
_04001FE8:
_04001FEC:
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
