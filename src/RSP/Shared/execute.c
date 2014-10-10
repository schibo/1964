#include "rsp_prefix.h"

#include <stdlib.h>
#include <math.h>
#include "../1964AudioLLE/Microcode/Microcode.h"
#include "../1964AudioLLE/audio.h"

extern AUDIO_INFO AudioInfo;

// TEMP - disable
//#define TEST_MODE // Enables logging
#define RSP_COMPILE	// enables decompiler


_s32			DoOnceAt = 10;
char			srcFileName[0xff];
char			srcBootFunctionName[0xff];
char			srcImemFunctionName[0xff];
UcodeInfo		lastUcodeInfo;
pMicrocodeFunc	functionAddress;
BOOL			ucodeDetected=FALSE;

int DisableMP3 = 0;

#ifdef TEST_MODE
#   define LogMe fprintf
    FILE	*dfile = NULL;
#else
#   define LogMe
    int dfile;
#endif


typedef struct {

	_u32	crc1;
	_u32	crc2;
	_u32	ucode;

} GameToUcodeMap;

GameToUcodeMap ucodeMaps[] =
{
	0xF0000F01,	0x201FE113,	0,	// SUPER MARIO 64
	0xF0000F01,	0xEE1DFC09,	1,	// DR.MARIO 64

	0x10EE13A0,	0xE4E8B064,	2,	// MarioTennis
	0x10EE13A0,	0x1D081B95,	2,	// BUCK BUMBLE

	0x17841831,	0xFDFDFEB7,	3,	// THE LEGEND OF ZELDA
	0x1780182D,	0xECECEEDF,	3,	// 1080 SNOWBOARDING

	0x0B3A66A6,	0xBAFCEA39,	4,	// GAUNTLET LEGENDS
	0xF0000F01,	0x91C1607A,	5,	// GOLDENEYE
	0x10F213A4,	0x111905D8,	6,	// Banjo-Kazooie
	0x17C0186D,	0xA4A4A3CA,	7,	// ZELDA MAJORA'S MASK
	0x17881835,	0xFDFDFDBA,	8,	// F-ZERO X
	0x1A8C1ADD,	0xF6F6F845,	9,	// MARIOKART64

	0xF0000F00,	0xEC1C0695,	10,	// BANJO TOOIE
	0xF0000F00,	0x5C8C6785,	10,	// JET FORCE GEMINI, MICKEY USA
	0xF0000F01,	0xEE1DFC09,	10,	// KILLER INSTINCT GOLD

	0xF0000F01,	0xBBFED702,	11,	// Diddy Kong Racing
	0x1780182D,	0x7A7A7A92,	12,	// YOSHI STORY
	0x10EA1390,	0xC8C8BCE9,	13,	// DONKEY KONG 64
	0x180418B1,	0xB3B3B473,	14,	// STARFOX64
	0xF0000F00,	0x7BAB71F2,	15,	// CONKER BFD

	0xF0000F00,	0x659567E0,	16,	// Perfect Dark
};

_u32 ComputeCRC32(const char *buf, int len)
{
	_u32	crc = 0;
	_u32	i;

	if (buf == NULL) return 0L;

#ifndef BIG_ENDIAN
	for( i=0; i<len/4; i++ )
	{
		crc += *(_u32*)(buf+i);
		crc ^= i;
	}
#else

	for (i = 0; i < (len / 4); i++) {
		_u8		buffer[8];
		_u32	c;
		_u16	temp;
		_u32	value;

		// create a word swapped buffer
		for (c = 0; c < 8; c++) buffer[(c ^ 1)] = buf[(i & 0xFFFFFFFC) + c];
		temp = ((_u16*)buffer)[0];
		((_u16*)buffer)[0] = ((_u16*)buffer)[1];
		((_u16*)buffer)[1] = temp;
		temp = ((_u16*)buffer)[2];
		((_u16*)buffer)[2] = ((_u16*)buffer)[3];
		((_u16*)buffer)[3] = temp;

		value = (((_u32)buffer[((i & 0x03) + 3)] << 24) | ((_u32)buffer[((i & 0x03) + 2)] << 16) | ((_u32)buffer[((i & 0x03) + 1)] << 8) | ((_u32)buffer[((i & 0x03) + 0)] << 0));

		crc += value;
		crc ^= i;
	}

#endif // BIG_ENDIAN

	return crc;
}

int LookupUcode()
{
	OSTask *ptask = (OSTask *)(pDMEM + 0x0FC0);
	_u32 UCData= *(_u32*)&*(_u8*)&pDMEM[0xFD8];
	_u8 *UData = (_u8*)(pRDRAM + UCData);

	_u32 crc1= *(_u32*)(UData+(0x30))+*(_u32*)(UData);
	_u32 crc2 = ComputeCRC32( (char*)UData, ptask->t.ucode_data_size );


	int i;

	for( i=0; i<sizeof(ucodeMaps)/sizeof(GameToUcodeMap); i++ )
	{
		if( ucodeMaps[i].crc1 == crc1 && ucodeMaps[i].crc2 == crc2 )
			return ucodeMaps[i].ucode;
	}

#ifdef LOG_UCODES
	{
		FILE	*fp = File_Open("audioUcodes.log","at");
		if( fp )
		{
			fprintf(fp, "0x%08X,\t0x%08X,\t1,\t// %s\n", crc1, crc2, gameName);
			fclose(fp);
		}
	}
#endif

	return 0;

	if (((_u32*)UData)[0] != 0x1) {
		if (*(_u32*)(UData+(0x30)) == 0x0B396696)
		{
			//ABI 5
			MessageBox(0, "ABI5: MusyX", "", 0);
			return 6;
		}
		else
		{
			// ABI 3
			MessageBox(0, "ABI3", "", 0);
			return 1;
		}
	} else {
		if (*(_u32*)(UData+(0x30)) == 0xF0000F00) // Should be common in ABI 1
		{
			// ABI 1
			MessageBox(0, "ABI1", "", 0);
			return 0;
		}
		else {
			// ABI 2
			MessageBox(0, "ABI2", "", 0);
			return 2;
		}
	}
}


void DetectMicrocode()
{
	OSTask *ptask = (OSTask *)(pDMEM + 0x0FC0);

	gUcode = LookupUcode();

	functionAddress = ucodeInfos[gUcode].functionAddress;

	// create the microcode filenames
	strcpy(srcFileName, ucodeInfos[gUcode].filename);
	strcpy(srcBootFunctionName, ucodeInfos[gUcode].functionBootCode);
	strcpy(srcImemFunctionName, ucodeInfos[gUcode].functionData);

	lastUcodeInfo.ucDStart = ptask->t.ucode_data;
	lastUcodeInfo.ucDSize = ptask->t.ucode_data_size;
	lastUcodeInfo.ucStart = ptask->t.ucode;
	lastUcodeInfo.ucSize = ptask->t.ucode_size;

	ucodeDetected = TRUE;
}

//_____________________________________________________________________
// rsp_reset
//
void rsp_reset()
{
#ifdef TEST_MODE
	if( !dfile )
		dfile = File_Open("DynaAudioLog.log","wt");
#endif // TEST_MODE

    memset(&state, 0, sizeof(state));
    state.halt = 1;
}

//_____________________________________________________________________
// rsp_run
//

#define BAD_TASK					0
#define GFX_TASK					1
#define SND_TASK					2
#define JPG_TASK					4

void rsp_run()
{
	OSTask	*ptask = (OSTask *)(pDMEM + 0x0FC0);

	static int k=0;
	static int Count=0;
	static int snd_decompiled = 0;
	static int jpg_decompiled = 0;
	static int mp3_decompiled = 0;
	int rsp_task = 0;

    sp_reg_pc = 0x04001000;
    state.halt = 0;
    LogMe (dfile, "RSP Started at %08X\n", sp_reg_pc);

	// Uncomment the next line to run the rsp interpreter and do nothing else special.
	//while (rsp_step()); return;

	if( !ucodeDetected )
		DetectMicrocode();

	if( lastUcodeInfo.ucDStart != ptask->t.ucode_data || lastUcodeInfo.ucDSize != ptask->t.ucode_data_size ||
		lastUcodeInfo.ucStart != ptask->t.ucode || lastUcodeInfo.ucSize != ptask->t.ucode_size )
	{
		DetectMicrocode();
	}

#ifdef RSP_DECOMPILER

	rsp_task = AudioInfo.DMEM[0x3f0*4];

	if (Count++ >= 10)
	{
		if (!k)
		{
			char tempstr[0xff];
			_snprintf(tempstr, sizeof(tempstr), "Decompile to C:\\%s?", srcFileName);
			k=1;
			if (MessageBox(AudioInfo.hwnd, tempstr, "Decompile RSP", MB_YESNO) == IDYES)
				k=2;
		}

	if (k==2)
	switch (rsp_task)
	{
	case SND_TASK:

		if (!snd_decompiled)
		{
			rsp_decompile(SND_TASK);
			snd_decompiled = 1;
			return;
		}
		break;

		//Perfect Dark
		DisableMP3 = 1;
		if ( (strcmp(ucodeInfos[gUcode].functionData, "_PDData")==0) && (*(_u32 *)&pIMEM[0x04001334 & 0xfff]==0x4a192950))
		{
			rsp_decompile_mp3();
			mp3_decompiled = 1;
			return;
		}

	//case JPG_TASK:
	//	if (!jpg_decompiled)
	//	{
	//		rsp_decompile(JPG_TASK);
	//		jpg_decompiled = 1;
	//		return;
	//	}
	//	break;
	}
	}

#endif // RSP_DECOMPILER


	rsp_task = AudioInfo.DMEM[0x3f0*4];
	switch (rsp_task)
	{
		case JPG_TASK:
		// run the rsp interpreter
		//sp_reg_pc = 0x04001000;
		//state.halt = 0;
		//while( rsp_step() ) { }
		break;

		case SND_TASK:
		functionAddress(sp_reg_pc);
		break;

		default:
		break;
	}

	__asm emms;
	return;
//	if (!DoOnce) DoOnce++; functionAddress(sp_reg_pc); return;

	LogMe (dfile, "RSP Left at %08X", sp_reg_pc);
}
