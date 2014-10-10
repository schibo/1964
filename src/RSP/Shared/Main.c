#include "rsp_prefix.h"

#include <stdio.h>
#include "rsp_mnemonic.h"
#include "state.h"

_s32		DoOnce;
t_state		state;
_u32		sp_reg_pc;
_u32		DMEM_Address;
_u32		RDRAM_Address;
_u32		gUcode;
_u32		imem_DMA_dst = 0;
_u32		imem_DMA_src = 0;
int gSwitchToFunction = -1;

extern char	gameName[40];
extern char	*pDMEM;
extern char	*pIMEM;
extern char	*pRDRAM;

//
// These are externals for our compiled functions
//
extern void _MarioBootCode(_u32 StartAddress);
extern void _MarioData(_u32 StartAddress);
extern void _ZeldaBootCode(_u32 StartAddress);
extern void _ZeldaData(_u32 StartAddress);
extern void _SmashBrosBootCode(_u32 StartAddress);
extern void _SmashBrosData(_u32 StartAddress);
extern void _GauntletBootCode(_u32 StartAddress);
extern void _gauntletData(_u32 StartAddress);
extern void _BanjoBootCode(_u32 StartAddress);
extern void _BanjoData(_u32 StartAddress);
extern void _GoldenEyeBootCode(_u32 StartAddress);
extern void _GoldenEyeData(_u32 StartAddress);
extern void _ZeldaMMBootCode(_u32 StartAddress);
extern void _ZeldaMMData(_u32 StartAddress);
extern void _FZeroBootCode(_u32 StartAddress);
extern void _FZeroData(_u32 StartAddress);
extern void _MarioKartBootCode(_u32 StartAddress);
extern void _MarioKartData(_u32 StartAddress);
extern void _TooieBootCode(_u32 StartAddress);
extern void _TooieData(_u32 StartAddress);
extern void _DKRBootCode(_u32 StartAddress);
extern void _DKRData(_u32 StartAddress);
extern void _YoshiBootCode(_u32 StartAddress);
extern void _DK64BootCode(_u32 StartAddress);
extern void _StarFoxBootCode(_u32 StartAddress);
extern void _DrMarioBootCode(_u32 StartAddress);
extern void _PDBootCode(_u32 StartAddress);
extern void _PDData(_u32 StartAddress);
extern void _PD_MP3(_u32 StartAddress);
extern void _ConkerBootCode(_u32 StartAddress);
extern void _ConkerData(_u32 StartAddress);
//extern void _Conker_MP3(_u32 StartAddress);

UcodeFunctionEntry MarioFuncs[] = {
	0x040010E4,	0x04001108,	"Main_Parser",
	0x040013A8,	0x040013F4,	"Mario13A8_13F4",
	0x04001424,	0x04001438,	"Mario1424_1438",
	0x040015B8,	0x040016DC,	"Mario15B8_16DC",
	0x040019D8,	0x04001AC4,	"Mario19D8_1AC4",
	0x04001D08,	0x04001D24,	"Mario1D08_1D24",
	0x04001D30,	0x04001D4C,	"Mario1D30_1D4C",
	0x04001D50,	0x04001DE8,	"Mario1D50_1DE8",
	0x04001E5C,	0x04001E88,	"Mario1E5C_1E88"
};


UcodeFunctionEntry SmashBrosFuncs[] = {
	0x040011AC,	0x040011B4,	"SmashBros11AC_11B4",
	0x040012E4,	0x04001334,	"SmashBros12E4_1334",
	0x0400135C,	0x04001370,	"SmashBros135C_1370",
	0x04001510,	0x04001630,	"SmashBros1514_1630",
	0x0400193C,	0x04001A28,	"SmashBros193C_1A28",
	0x04001BB0,	0x04001C48,	"SmashBros1BB0_1C48",
	0x04001C84,	0x04001CC8,	"SmashBros1C84_1CC8"
};

UcodeFunctionEntry ZeldaFuncs[] = {
	0x040011A4,	0x040011D4,	"Zelda11A4_11D4",
	0x040011F8,	0x0400120C,	"Zelda11F8_120C",
	0x040013A4,	0x040014D8,	"Zelda13A4_14D8",
	0x0400192C,	0x040019A8,	"Zelda192C_19A8",
	0x04001B60,	0x04001B8C,	"Zelda1B60_1B8C",
	0x04001DC4,	0x04001E14,	"Zelda1DC4_1E14"
};

UcodeFunctionEntry DKRFuncs[] = {
	0x04001458,	0x04001474,	"DKR1458_1474",
	0x04001864,	0x040018B0,	"DKR1864_18B0",
	0x04001D74,	0x04001D84,	"DKR1D74_1D84",
	0x04001D88,	0x04001E0C,	"DKR1D88_1E0C",
	0x04001E3C,	0x04001F30,	"DKR1E3C_1F30"
};

UcodeFunctionEntry TooieFuncs[] = {
	0x040010E8,	0x040010FC,	"Main_Parser_Tooie",
	0x04001B9C,	0x04001C58,	"Tooie1B9C_1C58",
	0x04001F20,	0x04001F84,	"Tooie1F20_1F84",
	0x04001FB0,	0x04001FF4,	"Tooie1FB0_1FF4"
};

UcodeFunctionEntry BanjoFuncs[] = {
	0x04001BE8,	0x04001C4C,	"Banjo1BE8_1C4C"
};

UcodeFunctionEntry GoldenEyeFuncs[] = {
	0x04001800,	0x0400184C,	"GoldenEye1800_184C",
	0x04001CF4,	0x04001D04,	"GoldenEye1CF4_1D04",
	0x04001D08,	0x04001D2C,	"GoldenEye1D08_1D2C",
	0x04001D60,	0x04001D8C,	"GoldenEye1D60_1D8C"
};

IMEM_DMA_FunctionMapType Conker_IMEM_DMA_Maps[]= {
	{ 0x1080,	0x000291A0,	0x0F80,	0x40055800,	0x8C3C0030,	0x8C3B0034,	0x30A40001,	_ConkerData },
	{ 0x1238,	0x0002A110,	0x09C0,	0x8C110FF4,	0x8C100FF8,	0x8C0F0FFC,	0x02201020,	_PD_MP3 },	// MP3?
	{ 0x1238,	0x00029358,	0x0DB8,	0x00000000,	0x00000000,	0x3322FFFF,	0x4A01086C,	_ConkerData },	// Reload
	{ 0,	0,	0,	0,	0,	0,	0,	0 }
};

IMEM_DMA_FunctionMapType PD_IMEM_DMA_Maps[]= {
	{ 0x1080,	0x0005b4d0,	0xF80,	0x40055800,	0x8c3c0030,	0x8c3b0034,	0x30a40001,	_PDData },
	{ 0x1238,	0x0005B688,	0x0DC8,	0x00000000,	0x00000000,	0x3322FFFF,	0x4A01086C,	_PDData },
	{ 0x1238,	0x0005B688,	0x09C0,	0x00000000,	0x00000000,	0x3322FFFF,	0x4A01086C,	_PDData },
	{ 0x1238,	0x0005C450,	0x09C0,	0x8C110FF4,	0x8C100FF8,	0x8C0F0FFC,	0x02201020, _PD_MP3 },	// The mp3 decoding code
	{ 0,	0,	0,	0,	0,	0,	0,	0 }
};


UcodeBaseMaps ucodeInfos[] = {
	"MarioMicrocode.c",			"_MarioBootCode",			"_MarioData",			_MarioBootCode,			MarioFuncs,		sizeof(MarioFuncs)/sizeof(UcodeFunctionEntry),		NULL,	0,
	"DrMarioMicrocode.c",		"_DrMarioBootCode",			"_DrMarioData",			_DrMarioBootCode,		NULL,	0,		NULL,	0,
	"SmashBrosMicrocode.c",		"_SmashBrosBootCode",		"_SmashBrosData",		_SmashBrosBootCode,		SmashBrosFuncs,	sizeof(SmashBrosFuncs)/sizeof(UcodeFunctionEntry),	NULL,	0,
	"ZeldaMicrocode.c",			"_ZeldaBootCode",			"_ZeldaData",			_ZeldaBootCode,			ZeldaFuncs,		sizeof(ZeldaFuncs)/sizeof(UcodeFunctionEntry),		NULL,	0,
	"GauntletMicrocode.c",		"_GauntletBootCode",		"_GauntletData",		_GauntletBootCode,		NULL,	0,		NULL,	0,
	"GoldenEyeMicrocode.c",		"_GoldenEyeBootCode",		"_GoldenEyeData",		_GoldenEyeBootCode,		GoldenEyeFuncs,	sizeof(GoldenEyeFuncs)/sizeof(UcodeFunctionEntry),	NULL,	0,
	"BanjoMicrocode.c",			"_BanjoBootCode",			"_BanjoData",			_BanjoBootCode,			BanjoFuncs,		sizeof(BanjoFuncs)/sizeof(UcodeFunctionEntry),		NULL,	0,
	"ZeldaMMMicrocode.c",		"_ZeldaMMBootCode",			"_ZeldaMMData",			_ZeldaMMBootCode,		NULL,	0,		NULL,	0,
	"FZeroMicrocode.c",			"_FZeroBootCode",			"_FZeroData",			_FZeroBootCode,			NULL,	0,		NULL,	0,
	"MarioKartMicrocode.c",		"_MarioKartBootCode",		"_MarioKartData",		_MarioKartBootCode,		NULL,	0,		NULL,	0,
	"TooieMicrocode.c",			"_TooieBootCode",			"_TooieData",			_TooieBootCode,			TooieFuncs,		sizeof(TooieFuncs)/sizeof(UcodeFunctionEntry),		NULL,	0,
	"DKRMicrocode.c",			"_DKRBootCode",				"_DKRData",				_DKRBootCode,			DKRFuncs,		sizeof(DKRFuncs)/sizeof(UcodeFunctionEntry),		NULL,	0,
	"YoshiMicrocode.c",			"_YoshiBootCode",			"_YoshiData",			_YoshiBootCode,			NULL,	0,		NULL,	0,
	"DK64Microcode.c",			"_DK64BootCode",			"_DK64Data",			_DK64BootCode,			NULL,	0,		NULL,	0,
	"StarFoxMicrocode.c",		"_StarFoxBootCode",			"_StarFoxData",			_StarFoxBootCode,		NULL,	0,		NULL,	0,
	"ConkerMicrocode.c",		"_ConkerBootCode",			"_ConkerData",			_ConkerBootCode,		NULL,	0,		Conker_IMEM_DMA_Maps,	sizeof(Conker_IMEM_DMA_Maps)/sizeof(IMEM_DMA_FunctionMapType),
	"PDMicrocode.c",			"_PDBootCode",				"_PDData",				_PDBootCode,			NULL,	0,		PD_IMEM_DMA_Maps,	sizeof(PD_IMEM_DMA_Maps)/sizeof(IMEM_DMA_FunctionMapType)
};


void ErrorToCallFunction(_u32 addr)
{
	char buf[200];
	sprintf(buf, "Error to call function at: %08X in file: %s",addr, ucodeInfos[gUcode].filename);
	MessageBox(0,buf, "Error", MB_OK);
}

#ifndef BIG_ENDIAN

_u8 Load8_DMEM(_u32 offset)
{
    return pDMEM[(offset & 0xFFF) ^ 3];
}

_u16 Load16_DMEM(_u32 offset)
{
	union {
		_u16 u16;
		_u8 u8[2];
	} value;

    if (offset & 0x1)
	{
        value.u8[0] = Load8_DMEM(offset+1);
        value.u8[1] = Load8_DMEM(offset);
        return value.u16;
    }
	else
	{
        return (*(_u16 *)&pDMEM[offset^2]);
    }
}

_u32 Load32_DMEM(_u32 offset)
{
	offset &= 0xfff;

	if (offset & 0x3)
	{
		union {
			_u32 u32;
			_u8  u8[4];
		} value;

		value.u8[0] = Load8_DMEM(offset+3);
		value.u8[1] = Load8_DMEM(offset+2);
		value.u8[2] = Load8_DMEM(offset+1);
		value.u8[3] = Load8_DMEM(offset  );
        return value.u32;
    }
	else
	{
        return (*(_u32 *)&pDMEM[offset]);
    }
}

_u64 Load64_DMEM(_u32 offset)
{
    _u64 value;
    _u64 value2;
    int i;

	if (offset & 0x3)
	{
        value = value2 = 0;
        for (i = 0; i < 4; i++)
		{
            value |= (Load8_DMEM((offset+i)&0xfff) << ((3-i)<<3));
        }
        for (i = 0; i < 4; i++)
		{
            value2 |= (Load8_DMEM((offset+i+4)&0xfff) << ((3-i)<<3));
        }
        value = (_u32)value;
        value2 = (_u32)value2;
        return ((value2) | (value << 32));
    }
	else
	{
        value = *(_u64 *)&pDMEM[offset & 0xfff];
        return( (value >> 32) | (value << 32) );
    }
}

#else

_u8 Load8_DMEM(_u32 offset)
{
    return pDMEM[offset];
}

_u16 Load16_DMEM(_u32 offset)
{
	return (*(_u16*)&pDMEM[offset]);
}

_u32 Load32_DMEM(_u32 offset)
{
	return (*(_u32 *)&pDMEM[(offset & 0xFFF)]);
}

_u64 Load64_DMEM(_u32 offset)
{
	return (*(_u64*)&pDMEM[offset & 0xFFF]);
}

#endif // BIG_ENDIAN


#ifndef BIG_ENDIAN

void Save8_DMEM(_u8 what, _u32 addr)
{
	addr &= 0xfff;
    pDMEM[addr ^ 3] = what;
}

void Save16_DMEM(_u16 what, _u32 addr)
{
    if (addr & 0x1)
	{
        Save8_DMEM ((_u8)(what>>8), addr);
        Save8_DMEM ((_u8)(what)   , addr+1);
    }
	else
	{
		addr &= 0xfff;
        *((_u16 *)(&pDMEM[addr^2])) = what;
    }
}

void Save32_DMEM(_u32 what, _u32 addr)
{
    if (addr & 0x3)
	{
		int i;
        for (i = 0; i < 4; i++)
		{
            Save8_DMEM ((_u8)(what>>((3-i)<<3)), (addr+i));
        }
    }
	else
	{
		addr &= 0xfff;
        *((_u32 *)(&pDMEM[addr])) = what;
    }
}

void Save64_DMEM(_u64 what, _u32 addr)
{
    int i;
    //_assert_ ((addr & 0xffffff) == (addr & 0xfff));
    if (addr & 0x3) {
        for (i = 0; i < 4; i++) {
            Save8_DMEM ((_u8)(what>>((3-i)<<3)), ((addr+i+4)));
        }
        for (i = 0; i < 4; i++) {
            Save8_DMEM ((_u8)(what>>((7-i)<<3)), ((addr+i)));
        }

    } else {
		*((_u64 *)(&pDMEM[addr & 0xfff])) = (what << 32) | (what >> 32);
    }
}

#else

void Save8_DMEM(_u8 what, _u32 addr)
{
    pDMEM[addr & 0xfff] = what;
}

void Save16_DMEM(_u16 what, _u32 addr)
{
	(*((_u16*)&pDMEM[addr & 0xfff])) = what;
}

void Save32_DMEM(_u32 what, _u32 addr)
{
	(*((_u32*)&pDMEM[addr & 0xfff])) = what;
}

void Save64_DMEM(_u64 what, _u32 addr)
{
	(*((_u64*)&pDMEM[addr & 0xfff])) = what;
}

#endif // BIG_ENDIAN

void DMAWrite(_u32 length)
{
	_u32		rdramOffset = (RDRAM_Address & 0x00FFFFFF);
	_u32		spOffset = (DMEM_Address & 0x00001FFF);

	length += 1;
	SPCYCLE(4 * length);

	// safety check the copy
	// TODO - this should use the real rdram size and not assume expansion pak
	if ((spOffset >= 0x2000) || (rdramOffset >= 0x00800000)) return;
	if ((spOffset + length) > 0x2000) length = (0x2000 - spOffset);
	if ((rdramOffset + length) > 0x00800000) length = (0x00800000 - rdramOffset);

#ifndef BIG_ENDIAN
	if ((length | rdramOffset | spOffset) & 3)
	{
		_u32	i;
		for(i = 0; i < length; i++)
		{
			pRDRAM[(i + rdramOffset) ^ 3] = pDMEM[(i + spOffset) ^ 3];
		}
		return;
	}
#endif // BIG_ENDIAN

	memcpy(&pRDRAM[rdramOffset], &pDMEM[spOffset], length);
}


char DMARead(_u32 length)
{
	_u32	i;
	char	*location;
	BOOL	dumpFunction = FALSE;

	SPCYCLE(4*length);

	location = pDMEM;
	if (DMEM_Address & 0x1000)
	{
		location = pIMEM;

		if( ucodeInfos[gUcode].noOfDMAMaps > 0 )
		{
			_u32	i;
			BOOL	found = FALSE;

			for( i = 0; i < ucodeInfos[gUcode].noOfDMAMaps; i++ )
			{
				IMEM_DMA_FunctionMapType *pmap = ucodeInfos[gUcode].DMAMaps;
				if( length+1 == pmap[i].DMALength && (DMEM_Address & 0x1FFF) == pmap[i].IMEMaddr &&
						pmap[i].RDRAMaddr == (RDRAM_Address & 0x7FFFFF) )
				{
					found = TRUE;
					break;
				}
			}

			if( !found )
			{
#ifdef LOG_UCODES
				FILE		*fp = File_Open("IMEM_DMA.log","at");
				if( fp )
				{
					fprintf(fp,"Game Name=%s\n", gameName);
					fprintf(fp,"0x%04X,\t0x%08X,\t0x%04X,\t0x%08X,\t0x%08X,\t0x%08X,\t0x%08X\n",
							(DMEM_Address&0x1FFF), (RDRAM_Address&0x7FFFFF), length+1,
							*(_u32*)&pRDRAM[(0+RDRAM_Address)],
							*(_u32*)&pRDRAM[(4+RDRAM_Address)],
							*(_u32*)&pRDRAM[(8+RDRAM_Address)],
							*(_u32*)&pRDRAM[(12+RDRAM_Address)]
							);
					fclose(fp);
				}
#endif
				ucodeInfos[gUcode].DMAMaps[i-1].DMALength = length+1;
				ucodeInfos[gUcode].DMAMaps[i-1].functionAddress = ucodeInfos[gUcode].DMAMaps[0].functionAddress;
				ucodeInfos[gUcode].DMAMaps[i-1].IMEMaddr = (DMEM_Address & 0x1FFF);
				ucodeInfos[gUcode].DMAMaps[i-1].RDRAMaddr = (RDRAM_Address & 0x7FFFFF);
				ucodeInfos[gUcode].DMAMaps[i-1].dword1 = *(_u32*)&pRDRAM[(0+RDRAM_Address)];
				ucodeInfos[gUcode].DMAMaps[i-1].dword2 = *(_u32*)&pRDRAM[(4+RDRAM_Address)];
				ucodeInfos[gUcode].DMAMaps[i-1].dword3 = *(_u32*)&pRDRAM[(8+RDRAM_Address)];
				ucodeInfos[gUcode].DMAMaps[i-1].dword4 = *(_u32*)&pRDRAM[(12+RDRAM_Address)];

				gSwitchToFunction = i-1;
				dumpFunction = TRUE;
			}
			else
				gSwitchToFunction = i;
		}
		else
		{
			if(imem_DMA_dst == 0)
			{
				imem_DMA_dst = DMEM_Address;
				imem_DMA_src = RDRAM_Address;
			}
			else if( imem_DMA_dst != DMEM_Address )
			{
				// To detect ucode changes
				char msg[256];
				sprintf(msg, "Warning, DMA to new IMEM address %08X, was %08X", DMEM_Address, imem_DMA_dst);
				//MessageBox(NULL, msg, "Warning", MB_OK);
				imem_DMA_dst = DMEM_Address;
				imem_DMA_src = RDRAM_Address;
			}
			else if( imem_DMA_src != RDRAM_Address )
			{
				// To detect ucode changes
				char msg[256];
				sprintf(msg, "Warning, DMA from new RDRAM address %08X to IMEM, was %08X", RDRAM_Address, imem_DMA_src);
				//MessageBox(NULL, msg, "Warning", MB_OK);
				imem_DMA_src = RDRAM_Address;
			}

			//sprintf(tempStr, "%08X: To IMEM: %08X\nFrom RDRAM: %08X\nlength=%08X", sp_reg_pc, DMEM_Address, RDRAM_Address, length+1);
			//MessageBox(0, tempStr, "", 0);
		}
	}

	DMEM_Address &= 0xfff;
	location += DMEM_Address;

	if ((RDRAM_Address & DMEM_Address & 4) == 0)
	{
		for(i = 0; i <= length; i += 4)
		{
			*(_u32*)&location[i] = *(_u32*)&pRDRAM[(i + RDRAM_Address)];
		}
	}
	else
	{
#ifndef BIG_ENDIAN
		for(i = 0; i <= length; i++)
		{
			location[i ^ 3] = pRDRAM[(i+RDRAM_Address) ^ 3];
		}
#else
		for(i=0; i<=length; i++)
		{
			location[i] = pRDRAM[(i + RDRAM_Address)];
		}
#endif // BIG_ENDIAN
	}

	return dumpFunction;
}

//_____________________________________________________________________
//
//
void rsp_srv(_u32 instruction)
{
    //MessageBox (NULL, "Need to implement SRV", "SRV", MB_OK);
    //_assert_(0);
#define __RSP_O(x) ( ((_s32)(_s8)((instruction & 0x40) ? ((instruction & 0x7f) | 0x80) : (instruction & 0x7f))) << x )
#define __F		( (BYTE)(instruction) & 0x3F )
#define __SA	( ((BYTE)(instruction >> 6)) & 0x1F )
#define __RD	( ((BYTE)(instruction >> 11)) & 0x1F )
#define __RT	( ((BYTE)(instruction >> 16)) & 0x1F )
#define __RS	( ((BYTE)(instruction >> 21)) & 0x1F )


	_u32    offset = 6-(__SA >> 2);
	_u32    addr = __RSP_O(4) + state.r[__RS];


	Save32_DMEM(state.v[__RT].U32[offset/2], addr);
	SPCYCLE(4);
}


//_____________________________________________________________________
// LDV loads a double (doubleword, 64-bits)
//
void rsp_ldv_compiler(_u32 addr, _u32 rt, _u32 offset)
{
	SPCYCLE(32);
	if( addr & 1 )
	{
		state.v[rt].U8[offset  ] = Load8_DMEM(addr+7);
		state.v[rt].U8[offset+1] = Load8_DMEM(addr+6);
		state.v[rt].U8[offset+2] = Load8_DMEM(addr+5);
		state.v[rt].U8[offset+3] = Load8_DMEM(addr+4);
		state.v[rt].U8[offset+4] = Load8_DMEM(addr+3);
		state.v[rt].U8[offset+5] = Load8_DMEM(addr+2);
		state.v[rt].U8[offset+6] = Load8_DMEM(addr+1);
		state.v[rt].U8[offset+7] = Load8_DMEM(addr  );
	}
	else if( addr & 2 )
	{
		offset>>=1;
		state.v[rt].U16[offset  ] = *(_u16 *)&pDMEM[(addr+6)^2];
		state.v[rt].U16[offset+1] = *(_u16 *)&pDMEM[(addr+4)^2];
		state.v[rt].U16[offset+2] = *(_u16 *)&pDMEM[(addr+2)^2];
		state.v[rt].U16[offset+3] = *(_u16 *)&pDMEM[(addr  )^2];
	}
	else
	{
		offset>>=2;
		state.v[rt].U32[offset] = *(_u32 *)&pDMEM[addr+4];
		state.v[rt].U32[offset+1] = *(_u32 *)&pDMEM[addr];
	}
}

//_____________________________________________________________________
//
//
void rsp_lpv(_u32 Instruction)
{
    MessageBox(NULL, "LPV not implemented", "TODO", MB_OK);
}

//_____________________________________________________________________
//
//
void rsp_cop2_vectop_vrcpl(_u32 Instruction)
{
    MessageBox (NULL, "VRCPL not implemented", "TODO", MB_OK);
}

//_____________________________________________________________________
//
//
void rsp_cop2_vectop_vrcph(_u32 Instruction)
{
    MessageBox (0, "VRCPH not implemented", "TODO", MB_OK);
}

//_____________________________________________________________________
//
//
void rsp_cop2_vectop_vabs(_u32 Instruction)
{
	MessageBox (NULL, "VABS not implemented", "TODO", MB_OK);
}

//_____________________________________________________________________
// File_Open
//

#ifndef BIG_ENDIAN

FILE *File_Open(char *filename, char *mode)
{
	char		fullPath[1024];
	// update your path here...
	strcpy(fullPath, "C://");
	strcat(fullPath, filename);
	return fopen(fullPath, mode);
}

#endif // BIG_ENDIAN

//_____________________________________________________________________

