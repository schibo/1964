/*$T wingui2.c GC 1.136 03/09/02 17:35:12 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    User Interface Client Dialogue Windows and Message Boxes
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

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include "../stdafx.h"
#include "DLL_Rsp.h"
#include <mbstring.h>


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void (__cdecl *GetDllInfo) (PLUGIN_INFO *) = NULL;
void (__cdecl *DLL_About) (HWND) = NULL;
char	temp_video_plugin[256];
char	temp_audio_plugin[256];
char	temp_input_plugin[256];
char	temp_rsp_plugin[256];
extern void	(__cdecl *_RSPDllAbout)			( HWND hWnd );
void SwitchLanguage(int id, BOOL refreshRomList);

/************************************************************************/
/*                                                                      */
/************************************************************************/

//typedef struct {
//	char	filename[MAX_PATH];
//	char	info[MAX_PATH];
//	int		type;
//	int		version;
//} PluginDLLInfo;

PluginDLLInfo *AllPluginDLLInfos=NULL;
int	PluginDLLCounts[4];
int TotalPluginDLLCount=0;

// Plugins that we wish users will have in their plugin folder
PluginDLLInfo DesiredPluginDLLInfos[] = 
{
	{	{0},	"Rice's Daedalus 5.2.0",						PLUGIN_TYPE_GFX,	-1},
	{	{0},	"Rice's Daedalus 4.6.0",						PLUGIN_TYPE_GFX,	-1},
	{	{0},	"Glide64 v0.5",									PLUGIN_TYPE_GFX,	-1},
	{	{0},	"glN64 v0.4",									PLUGIN_TYPE_GFX,	-1},
	{	{0},	"Jabo's Direct3D6 1.5",							PLUGIN_TYPE_GFX,	-1},
	{	{0},	"Jabo's Direct3D7 1.4",							PLUGIN_TYPE_GFX,	-1},
	{	{0},	"TR64 OpenGL v0.8.3",							PLUGIN_TYPE_GFX,	-1},
	{	{0},	"1964 OpenGL Graphics build 4.5.0",				PLUGIN_TYPE_GFX,	-1},

	{	{0},	"schibo's Audio Plugin 1.2 based on UltraHLE",	PLUGIN_TYPE_AUDIO,	-1},
	{	{0},	"Azimer's Audio v0.13b",						PLUGIN_TYPE_AUDIO,	-1},
	{	{0},	"Azimer's Audio v0.30 (Old Driver)",			PLUGIN_TYPE_AUDIO,	-1},
	{	{0},	"Azimer's Audio v0.30 (Driver Rev 2.2)",		PLUGIN_TYPE_AUDIO,	-1},
	{	{0},	"Azimer's Audio v0.40 Beta 2",					PLUGIN_TYPE_AUDIO,	-1},
	{	{0},	"Azimer's HLE Audio v0.55.1 Alpha",				PLUGIN_TYPE_AUDIO,	-1},
	{	{0},	"Basic Audio Plugin 2.0 for 1964",				PLUGIN_TYPE_AUDIO,	-1},
	{	{0},	"Jabo's DirectSound 1.5",						PLUGIN_TYPE_AUDIO,	-1},
	{	{0},	"No Audio Plugin 2.0 for 1964",					PLUGIN_TYPE_AUDIO,	-1},
	{	{0},	"No Sound",										PLUGIN_TYPE_AUDIO,	-1},

	{	{0},	"Basic Keyboard Plugin",						PLUGIN_TYPE_CONTROLLER,	-1},
	{	{0},	"Jabo's DirectInput7 1.5",						PLUGIN_TYPE_CONTROLLER,	-1},
	{	{0},	"NooTe_DI By NooTe version 0.4.2 build 17",		PLUGIN_TYPE_CONTROLLER,	-1},
	{	{0},	"N-Rage`s Direct-Input8 1.61",					PLUGIN_TYPE_CONTROLLER,	-1},
	{	{0},	"N-Rage`s Direct-Input8 V2 1.80a",				PLUGIN_TYPE_CONTROLLER,	-1},

	{	{0},	"Hacktarux hle rsp plugin",						PLUGIN_TYPE_RSP,	-1},
	{	{0},	"RSP emulation Plugin",							PLUGIN_TYPE_RSP,	-1},
};

/************************************************************************/
/* Search the plugin folder, fill all DLL info into the array           */
/************************************************************************/
void PopulateAllPluginInfos(int maxNo)
{
	PLUGIN_INFO		Plugin_Info;
	HINSTANCE		hinstLib = NULL;
	WIN32_FIND_DATA libaa;
	HANDLE			FindFirst;
	char			filename[300];
	char			StartPath[_MAX_PATH];
	char			SearchPath[_MAX_PATH];
	BOOL			KeepLooping=TRUE;
	int				i,j;

	if( AllPluginDLLInfos != NULL )
	{
		return;
	}

	AllPluginDLLInfos = (PluginDLLInfo*)VirtualAlloc(NULL, MAX_PLUGIN_DLL*sizeof(PluginDLLInfo), MEM_COMMIT, PAGE_READWRITE);
	TotalPluginDLLCount=0;

	PluginDLLCounts[0] = PluginDLLCounts[1] = PluginDLLCounts[2] = PluginDLLCounts[3] = 0;
	memset(AllPluginDLLInfos, 0, sizeof(PluginDLLInfo)*maxNo);

	GetPluginDir(StartPath);
	strcpy(SearchPath, StartPath);
	strcat(SearchPath, "*.dll");

	FindFirst = FindFirstFile(SearchPath, &libaa);
	if(FindFirst == INVALID_HANDLE_VALUE)
	{
		return ;
	}

	KeepLooping=TRUE;

	while(KeepLooping)
	{
		strcpy(filename, StartPath);
		strcat(filename, libaa.cFileName);

		hinstLib = LoadLibrary(filename);
		if( hinstLib )
		{
			GetDllInfo = (void(__cdecl *) (PLUGIN_INFO *)) GetProcAddress(hinstLib, "GetDllInfo");

			if( GetDllInfo )
			{
				__try
				{
					memset(&Plugin_Info,0,sizeof(Plugin_Info));
					GetDllInfo(&Plugin_Info);
					strcpy(AllPluginDLLInfos[TotalPluginDLLCount].filename, libaa.cFileName);
					strcpy(AllPluginDLLInfos[TotalPluginDLLCount].info, Plugin_Info.Name);
					AllPluginDLLInfos[TotalPluginDLLCount].version = Plugin_Info.Version;
					AllPluginDLLInfos[TotalPluginDLLCount].type = Plugin_Info.Type;
					PluginDLLCounts[Plugin_Info.Type-1]++;

					TotalPluginDLLCount++;
				}
				__except(NULL, EXCEPTION_EXECUTE_HANDLER)
				{
				}

				GetDllInfo = NULL;
			}
			FreeLibrary(hinstLib);
			hinstLib = NULL;
		}

		KeepLooping = FindNextFile(FindFirst, &libaa);
	}

	for( j=0; j<sizeof(DesiredPluginDLLInfos)/sizeof(PluginDLLInfo); j++ )
	{
		for(i=0; i<TotalPluginDLLCount;i++)
		{
			if( strnicmp(DesiredPluginDLLInfos[j].info, AllPluginDLLInfos[i].info, strlen(AllPluginDLLInfos[i].info)) == 0 )
			{
				strcpy(DesiredPluginDLLInfos[j].filename, AllPluginDLLInfos[i].filename);
				break;
			}
		}

		if( i==TotalPluginDLLCount )
		{
			DesiredPluginDLLInfos[j].filename[0] = 0;
		}
	}
}

const PluginDLLInfo *GetPluginDLLInfo(int index, int type)
{
	int i;

	if( index >= TotalPluginDLLCount )
		return NULL;

	for(i=0; i<TotalPluginDLLCount; i++)
	{
		if( AllPluginDLLInfos[i].type == type )
		{
			index--;
			if( index<0 )
			{
				return &AllPluginDLLInfos[i];
			}
		}
	}

	if( !guioptions.displayDefaultPlugins )
	{
		for( i=0; i<sizeof(DesiredPluginDLLInfos)/sizeof(PluginDLLInfo); i++ )
		{
			if( DesiredPluginDLLInfos[i].type == type && DesiredPluginDLLInfos[i].filename[0] == 0 )
			{
				index--;
				if( index<0 )
				{
					return &DesiredPluginDLLInfos[i];
				}
			}
		}
	}

	return NULL;
}


void __cdecl CountryCodeToCountryName_and_TVSystem(int countrycode, char *countryname, int *tvsystem)
{
	//Keep Country Name < 10 characters!
	switch(countrycode)
	{
	/* Demo */
	case 0:
		*tvsystem = TV_SYSTEM_NTSC;
		strcpy(countryname, "Demo");
		break;

	case '7':
		*tvsystem = TV_SYSTEM_NTSC;
		strcpy(countryname, "Beta");
		break;

	case 0x41:
		*tvsystem = TV_SYSTEM_NTSC;
		strcpy(countryname, "USA/Japan");
		break;

	/* Germany */
	case 0x44:
		*tvsystem = TV_SYSTEM_PAL;
		strcpy(countryname, "German");
		break;

	/* USA */
	case 0x45:
		*tvsystem = TV_SYSTEM_NTSC;
		strcpy(countryname, "USA");
		break;

	/* France */
	case 0x46:
		*tvsystem = TV_SYSTEM_PAL;
		strcpy(countryname, "France");
		break;

	/* Italy */
	case 'I':
		*tvsystem = TV_SYSTEM_PAL;
		strcpy(countryname, "Italy");
		break;

	/* Japan */
	case 0x4A:
		*tvsystem = TV_SYSTEM_NTSC;
		strcpy(countryname, "Japan");
		break;

	/* Europe - PAL */
	case 0x50:
		*tvsystem = TV_SYSTEM_PAL;
		strcpy(countryname, "Europe");
		break;

	case 'S':	/* Spain */
		*tvsystem = TV_SYSTEM_PAL;
		strcpy(countryname, "Spain");
		break;

	/* Australia */
	case 0x55:
		*tvsystem = TV_SYSTEM_PAL;
		strcpy(countryname, "Australia");
		break;

	case 0x58:
		*tvsystem = TV_SYSTEM_PAL;
		strcpy(countryname, "Europe");
		break;

	/* Australia */
	case 0x59:
		*tvsystem = TV_SYSTEM_PAL;
		strcpy(countryname, "Australia");
		break;

	case 0x20:
	case 0x21:
	case 0x38:
	case 0x70:
		*tvsystem = TV_SYSTEM_PAL;
		strcpy(countryname, "Europe");
		break;

	/* ??? */
	default:
		*tvsystem = TV_SYSTEM_PAL;
		strcpy(countryname, "PAL");
		break;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void InitPluginData(void)
{
	Gfx_Info.hWnd = gui.hwnd1964main;
	Gfx_Info.hStatusBar = gui.hStatusBar;
	Gfx_Info.MemoryBswaped = TRUE;
	Gfx_Info.HEADER = (__int8 *) &HeaderDllPass[0];
	Gfx_Info.RDRAM = (__int8 *) &gMemoryState.RDRAM[0];
	Gfx_Info.DMEM = (__int8 *) &SP_DMEM;
	Gfx_Info.IMEM = (__int8 *) &SP_IMEM;
	Gfx_Info.MI_INTR_RG = &MI_INTR_REG_R;
	Gfx_Info.DPC_START_RG = &DPC_START_REG;
	Gfx_Info.DPC_END_RG = &DPC_END_REG;
	Gfx_Info.DPC_CURRENT_RG = &DPC_CURRENT_REG;
	Gfx_Info.DPC_STATUS_RG = &DPC_STATUS_REG;
	Gfx_Info.DPC_CLOCK_RG = &DPC_CLOCK_REG;
	Gfx_Info.DPC_BUFBUSY_RG = &DPC_BUFBUSY_REG;
	Gfx_Info.DPC_PIPEBUSY_RG = &DPC_PIPEBUSY_REG;
	Gfx_Info.DPC_TMEM_RG = &DPC_TMEM_REG;
	Gfx_Info.VI_STATUS_RG = &VI_STATUS_REG;
	Gfx_Info.VI_ORIGIN_RG = &VI_ORIGIN_REG;
	Gfx_Info.VI_WIDTH_RG = &VI_WIDTH_REG;
	Gfx_Info.VI_INTR_RG = &VI_INTR_REG;
	Gfx_Info.VI_V_CURRENT_LINE_RG = &VI_CURRENT_REG;
	Gfx_Info.VI_TIMING_RG = &VI_BURST_REG;
	Gfx_Info.VI_V_SYNC_RG = &VI_V_SYNC_REG;
	Gfx_Info.VI_H_SYNC_RG = &VI_H_SYNC_REG;
	Gfx_Info.VI_LEAP_RG = &VI_LEAP_REG;
	Gfx_Info.VI_H_START_RG = &VI_H_START_REG;
	Gfx_Info.VI_V_START_RG = &VI_V_START_REG;
	Gfx_Info.VI_V_BURST_RG = &VI_V_BURST_REG;
	Gfx_Info.VI_X_SCALE_RG = &VI_X_SCALE_REG;
	Gfx_Info.VI_Y_SCALE_RG = &VI_Y_SCALE_REG;
	Gfx_Info.CheckInterrupts = CheckInterrupts;

	Audio_Info.hwnd = gui.hwnd1964main;
	Audio_Info.hinst = hinstLibAudio;
	Audio_Info.MemoryBswaped = 1;
	Audio_Info.HEADER = (BYTE *) &HeaderDllPass[0];
	Audio_Info.__RDRAM = (BYTE*) &gMemoryState.RDRAM[0];
	Audio_Info.__DMEM = (BYTE*) &SP_DMEM;
	Audio_Info.__IMEM = (BYTE*) &SP_IMEM;
	Audio_Info.__MI_INTR_REG = (DWORD*)&MI_INTR_REG_R;
	Audio_Info.__AI_DRAM_ADDR_REG = (DWORD*)&AI_DRAM_ADDR_REG;;
	Audio_Info.__AI_LEN_REG = (DWORD*)&AI_LEN_REG;
	Audio_Info.__AI_CONTROL_REG = (DWORD*)&AI_CONTROL_REG;
	Audio_Info.__AI_STATUS_REG = (DWORD*)&AI_STATUS_REG;
	Audio_Info.__AI_DACRATE_REG = (DWORD*)&AI_DACRATE_REG;
	Audio_Info.__AI_BITRATE_REG = (DWORD*)&AI_BITRATE_REG;
	Audio_Info.CheckInterrupts = DummyCheckInterrupts;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Set_1964_Directory(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	char	path_buffer[_MAX_PATH], drive[_MAX_DRIVE], dir[_MAX_DIR];
	char	fname[_MAX_FNAME], ext[_MAX_EXT];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	GetModuleFileName(NULL, path_buffer, sizeof(path_buffer));
	_splitpath(path_buffer, drive, dir, fname, ext);

	/* Set the main 1964.exe directory */
	strcpy(directories.main_directory, drive);
	strcat(directories.main_directory, dir);

	sprintf(directories.boxart_directory_to_use, "%sboxart\\", directories.main_directory);
	if( !FileIO_CheckAndCreateFolder(directories.boxart_directory_to_use) )
	{
		DisplayError("Can not create boxart directory: %s, please check it manually", directories.save_directory_to_use);
	}
}

char	critical_msg_buffer[32 * 1024]; /* 32KB */

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void __cdecl DisplayCriticalMessage(char *Message, ...)
{
	if(guioptions.show_critical_msg_window)
	{
		/*~~~~~~~~~~~~~*/
		char	Msg[400];
		va_list ap;
		char crlf[3];
		/*~~~~~~~~~~~~~*/

		va_start(ap, Message);
		vsprintf(Msg, Message, ap);
		va_end(ap);

		crlf[0] = 13;
		crlf[1] = 10;
		crlf[2] = '\0';
		if(strlen(critical_msg_buffer) + strlen(Msg) + 2 < 32 * 1024)
		{
			strcat(critical_msg_buffer, Msg);
			strcat(critical_msg_buffer, crlf);
			SendDlgItemMessage
			(
				gui.hCriticalMsgWnd,
				IDC_CRITICAL_MESSAGE_TEXTBOX,
				WM_SETTEXT,
				0,
				(LPARAM) critical_msg_buffer
			);
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */


extern HANDLE StopEmulatorEvent;
void __cdecl DisplayError(char *format, ...)
{
	char	Msg[400];
	va_list ap;
	char *formatTranslated;

#ifdef _DEBUG
	RefreshDebugger();
#endif

    //disabled
    return;

	formatTranslated = TranslateStringByString(format);

	va_start(ap, formatTranslated);
	vsprintf(Msg, formatTranslated, ap);
	va_end(ap);

	//DisplayCriticalMessage(Msg);
	//MessageBox(0, Msg, "Error",MB_OK);
	//return;

	if( emustatus.Emu_Is_Running )
	{
		strcat(Msg, "\n\n[Retry] to continue\n[Cancel] to stop emulation.");
		if (MessageBox(0, Msg, "Error", MB_RETRYCANCEL) == IDCANCEL)
		{
			PostMessage(gui.hwnd1964main, WM_COMMAND, ID_ROM_STOP, 0);
		}
	}
	else
	{
		MessageBox(0, Msg, "Error",MB_OK);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL __cdecl DisplayError_AskIfContinue(char *Message, ...)
{
	/*~~~~~~~~~~~~~*/
	char	Msg[400];
	int		val;
	va_list ap;
	/*~~~~~~~~~~~~~*/

	return TRUE;

#ifdef _DEBUG
	RefreshDebugger();
#endif
	va_start(ap, Message);
	vsprintf(Msg, Message, ap);
	va_end(ap);
	strcat(Msg, "\nDo you want to continue emulation ?");

	val = MessageBox(NULL, Msg, "Error", MB_YESNO | MB_ICONINFORMATION | MB_SYSTEMMODAL);
	if(val == IDYES)
		return TRUE;
	else
		return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void UpdateCIC(void)
{
	/*~~~~~~~~~~~~~~~~*/
	/* Math CIC */
	__int64 CIC_CRC = 0;
	int		i;
	/*~~~~~~~~~~~~~~~~*/

	for(i = 0; i < 0xFC0; i++)
	{
		CIC_CRC = CIC_CRC + (uint8) gMemoryState.ROM_Image[0x40 + i];
	}

	switch(CIC_CRC)
	{
	/* CIC-NUS-6101 (starfox) */
	case 0x33a27:
	case 0x3421e:
		/* DisplayError("Using CIC-NUS-6101\n"); */
		TRACE0("Using CIC-NUS-6101 for starfox\n");
		rominfo.CIC = (uint64) 0x3f;
		rominfo.RDRam_Size_Hack = (uint32) 0x318;
		break;

	/* CIC-NUS-6102 (mario) */
	case 0x34044:
		/* DisplayError("Using CIC-NUS-6102\n"); */
		TRACE0("Using CIC-NUS-6102 for mario\n");
		rominfo.CIC = (uint64) 0x3f;
		rominfo.RDRam_Size_Hack = (uint32) 0x318;
		ROM_CheckSumMario();

		break;

	/* CIC-NUS-6103 (Banjo) */
	case 0x357d0:
		/* DisplayError("Using CIC-NUS-6103\n"); */
		TRACE0("Using CIC-NUS-6103 for Banjo\n");
		rominfo.CIC = (uint64) 0x78;
		rominfo.RDRam_Size_Hack = (uint32) 0x318;
		break;

	/* CIC-NUS-6105 (Zelda) */
	case 0x47a81:
		/* DisplayError("Using CIC-NUS-6105\n"); */
		TRACE0("Using CIC-NUS-6105 for Zelda\n");
		rominfo.CIC = 0x91;
		rominfo.RDRam_Size_Hack = (uint32) 0x3F0;
		ROM_CheckSumZelda();
		break;

	/* CIC-NUS-6106 (F-Zero X) */
	case 0x371cc:
		/* DisplayError("Using CIC-NUS-6106\n"); */
		TRACE0("Using CIC-NUS-6106 for F-Zero/Yoshi Story\n");
		rominfo.CIC = (uint64) 0x85;
		rominfo.RDRam_Size_Hack = (uint32) 0x318;
		break;

	/* Using F1 World Grand Prix */
	case 0x343c9:
		/*
		 * LogDirectToFile("Using f1 World Grand Prix\n"); ?		 * DisplayError("F1 World Grand Prix ... i never saw ths BootCode before");
		 */
		TRACE0("Using Boot Code for F1 World Grand Prix\n");
		rominfo.CIC = (uint64) 0x85;
		rominfo.RDRam_Size_Hack = (uint32) 0x3F0;
		break;

	default:
		/*
		 * DisplayError("unknown CIC %08x!!!", (uint32)CIC_CRC); ?		 * SystemFailure(FILEIO_EXIT); ?		 * Use Mario for unknown boot code
		 */
		TRACE0("Unknown boot code, using Mario boot code instead");
		rominfo.CIC = (uint64) 0x3f;
		rominfo.RDRam_Size_Hack = (uint32) 0x318;
		break;
	}

	TRACE1("Rom CIC=%02X", rominfo.CIC);

	rominfo.countrycode = HeaderDllPass[0x3D];

	CountryCodeToCountryName_and_TVSystem(rominfo.countrycode, game_country_name, &game_country_tvsystem);
	rominfo.TV_System = game_country_tvsystem;
	Init_VI_Counter(game_country_tvsystem);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
LRESULT APIENTRY CriticalMessageDialog(HWND hDlg, unsigned message, WORD wParam, LONG lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		TranslateDialag(hDlg, "CRITICAL_MESSAGE");
		return(TRUE);
	case WM_COMMAND:
		if(wParam == IDOK)
		{
			guioptions.show_critical_msg_window = 0;
			EndDialog(hDlg, TRUE);
			gui.hCriticalMsgWnd = NULL;
			SetActiveWindow(gui.hwnd1964main);
			return(TRUE);
		}
		else if(wParam == ID_CLEAR_MESSAGE)
		{
			SendDlgItemMessage(hDlg, IDC_CRITICAL_MESSAGE_TEXTBOX, WM_SETTEXT, 0, (LPARAM) "");
			critical_msg_buffer[0] = '\0';	/* clear the critical message buffer */
		}
		break;
	}

	return(FALSE);
}

/************************************************************************/
/* Before play, load ROM specified plugins                              */
/************************************************************************/
LPDIRECTSOUND lpds;
void LoadROMSpecificPlugins()
{
	char	AudioPath[_MAX_PATH];					/* _MAX_PATH = 260 */
	char	VideoPath[_MAX_PATH];
	char	InputPath[_MAX_PATH];
	char	StartPath[_MAX_PATH];
	BOOL	result=TRUE;
	char	errormsg[500];
	static int		Audio = 0;

	errormsg[0]=0;

	GetPluginDir(StartPath);

	if( strcmp(currentromoptions.rspPluginName,emustatus.lastRSPPluginLoaded) != 0 )
	{
		if(strcmp(emustatus.lastRSPPluginLoaded, "") != 0 )
		{
			CloseRSPPlugin();
			rsp_plugin_is_loaded = FALSE;
			emuoptions.UsingRspPlugin = FALSE;
		}


		TRACE1("Loading RSP plugin: %s", currentromoptions.rspPluginName);
		strcpy(emustatus.lastRSPPluginLoaded, currentromoptions.rspPluginName);

		if( currentromoptions.rspPluginName[0] != 0 )
		{
			/* Set RSP plugin path */
			strcpy(InputPath, StartPath);

			strcpy(generalmessage,TranslateStringByString("Loading RSP Plugin:"));
			strcat(generalmessage, currentromoptions.rspPluginName);
			SetStatusBarText(0, generalmessage);
			TRACE0(generalmessage);
			strcat(InputPath, currentromoptions.rspPluginName);

			/* Load RSP plugin DLL */
			if(LoadRSPPlugin(InputPath) == FALSE)
			{
				// Cannot load ROM specified RSP plugin, we then use the system default RSP plugin
				CloseRSPPlugin();
				LoadPlugins(LOAD_RSP_PLUGIN);	
			}
			else
			{
				rsp_plugin_is_loaded = TRUE;
				emuoptions.UsingRspPlugin = TRUE;
				EnableMenuItem(gui.hMenu1964main, ID_RSP_CONFIG, MF_ENABLED);
				SetStatusBarText(0, TranslateStringByString("Init RSP Plugin ..."));
				InitializeRSP();
			}
		}
		else
		{
			emuoptions.UsingRspPlugin = FALSE;
			rsp_plugin_is_loaded = FALSE;
			EnableMenuItem(gui.hMenu1964main, ID_RSP_CONFIG, MF_GRAYED);
		}
	}

	if( strcmp(currentromoptions.videoPluginName,emustatus.lastVideoPluginLoaded) != 0 )
	{
		TRACE1("Loading video plugin: %s", currentromoptions.videoPluginName);
		CloseVideoPlugin();	// Close the current video plugin

		strcpy(emustatus.lastVideoPluginLoaded, currentromoptions.videoPluginName);

		strcpy(generalmessage,TranslateStringByString("Loading Video Plugin:"));
		strcat(generalmessage, currentromoptions.videoPluginName);
		SetStatusBarText(0, generalmessage);
		TRACE0(generalmessage);

		/* Set Video plugin path */
		strcpy(VideoPath, StartPath);
		strcat(VideoPath, currentromoptions.videoPluginName);

		/* Load Video plugin */
		if(LoadVideoPlugin(VideoPath) == FALSE)
		{
			LoadPlugins(LOAD_VIDEO_PLUGIN);	
		}
		else
		{
			strcpy(generalmessage, currentromoptions.videoPluginName);
			_strlwr(generalmessage);					/* convert to lower case */
			if(strstr(generalmessage, "gl") > 0) 
			{		/* Check if the plugin is opengl plugin */
				guioptions.ok_to_pause_at_menu = FALSE; /* We should not pause game by menu if using opengl plugin */
			}
			else if( strstr(generalmessage, "debug") > 0 || strstr(generalmessage, "dbg") > 0 )
			{
				guioptions.ok_to_pause_at_menu = FALSE;
			}
			else
			{
#ifdef _DEBUG
				guioptions.ok_to_pause_at_menu = FALSE;
#else
				guioptions.ok_to_pause_at_menu = TRUE;	/* if using D3D or other plugins, we can do it. */
#endif
			}

			SetStatusBarText(0, TranslateStringByString("Init Video Plugin ..."));
			InitPluginData();
			VIDEO_InitiateGFX(Gfx_Info);
			MoveWindow
				(
				gui.hwnd1964main,
				guistatus.window_position.left,
				guistatus.window_position.top+2,
				guistatus.clientwidth,
				guistatus.clientheight,
				TRUE
				);
			if(guistatus.WindowIsMaximized) 
				ShowWindow(gui.hwnd1964main, SW_SHOWMAXIMIZED);
		}
	}

	if( strcmp(currentromoptions.inputPluginName,emustatus.lastInputPluginLoaded) != 0 )
	{
		CloseControllerPlugin();

		strcpy(emustatus.lastInputPluginLoaded, currentromoptions.inputPluginName);
		TRACE1("Loading Input plugin: %s", currentromoptions.inputPluginName);

		strcpy(generalmessage,TranslateStringByString("Loading Input Plugin:"));
		strcat(generalmessage, currentromoptions.inputPluginName);
		SetStatusBarText(0, generalmessage);
		TRACE0(generalmessage);

		/* Set Input plugin path */
		strcpy(InputPath, StartPath);
		strcat(InputPath, currentromoptions.inputPluginName);

		/* Load Input plugin DLL */
		if(LoadControllerPlugin(InputPath) == FALSE) 
		{
			LoadPlugins(LOAD_INPUT_PLUGIN);	
		}
		else
		{
			SetStatusBarText(0, TranslateStringByString("Init Input Plugin ..."));
			CONTROLLER_InitiateControllers(gui.hwnd1964main, Controls);
		}
	}

    if( strcmp(currentromoptions.audioPluginName,emustatus.lastAudioPluginLoaded) != 0 )
	{
		CloseAudioPlugin();
		strcpy(emustatus.lastAudioPluginLoaded, currentromoptions.audioPluginName);
		TRACE1("Loading Audio plugin: %s", currentromoptions.audioPluginName);

		strcpy(generalmessage,TranslateStringByString("Loading Audio Plugin:"));
		strcat(generalmessage, currentromoptions.audioPluginName);
		SetStatusBarText(0, generalmessage);
		TRACE0(generalmessage);

		/* Set path for the Audio plugin */
		strcpy(AudioPath, StartPath);
		strcat(AudioPath, currentromoptions.audioPluginName);

		Audio_Is_Initialized = 0;
		Audio = 0;
		if(LoadAudioPlugin(AudioPath) == FALSE)
		{
			LoadPlugins(LOAD_AUDIO_PLUGIN);	
		}
		else
		{
			Audio = 1;
			if( _AUDIO_Initialize != NULL) 
			{
				if( AUDIO_Initialize( Audio_Info) == TRUE) 
				{
					Audio = 1;
					Audio_Is_Initialized = 1;
				} 
				else
				{
					Audio = 0;
					Audio_Is_Initialized = 0;
				}
			}

			if ( Audio_Is_Initialized )
			{
				HRESULT hr;

				__try {
					if ( FAILED( hr = DirectSoundCreate( NULL, &lpds, NULL ) ) ) 
					{
						Audio = 0;
					}
					if ( lpds) 
					{
						IDirectSound_Release(lpds);
					}
				}
				__except( NULL, EXCEPTION_EXECUTE_HANDLER) 
				{
					Audio = 0;
					__try 
					{
						if( lpds)
							IDirectSound_Release(lpds);
					} __except( NULL, EXCEPTION_EXECUTE_HANDLER){}
				}
			}

			if(Audio == 0)
			{
				CloseAudioPlugin();
				LoadPlugins(LOAD_AUDIO_PLUGIN);	
			}		
		}
	}
}

/************************************************************************/
/* After stop, reload system default plugins                            */
/************************************************************************/
void ReloadDefaultPlugins()
{
	if( strcmp(currentromoptions.rspPluginName,gRegSettings.RSPPlugin) != 0 && currentromoptions.rspPluginName[0] != 0 )
	{
		CloseRSPPlugin();
		LoadPlugins(LOAD_RSP_PLUGIN);	
	}

	if( strcmp(currentromoptions.inputPluginName,gRegSettings.InputPlugin) != 0 && currentromoptions.inputPluginName[0] != 0 )
	{
		CloseControllerPlugin();
		LoadPlugins(LOAD_INPUT_PLUGIN);	
	}

	if( strcmp(currentromoptions.audioPluginName,gRegSettings.AudioPlugin) != 0 && currentromoptions.audioPluginName[0] != 0 )
	{
		CloseAudioPlugin();
		LoadPlugins(LOAD_AUDIO_PLUGIN);	
	}

	if( strcmp(currentromoptions.videoPluginName,gRegSettings.VideoPlugin) != 0 && currentromoptions.videoPluginName[0] != 0 )
	{
		CloseVideoPlugin();	// Close the current video plugin
		LoadPlugins(LOAD_VIDEO_PLUGIN);
	}
}
/*
 =======================================================================================================================
    type = 0 Load all plugins type = 1 Load video plugin type = 2 Load audio plugin type = 3 Load input plugin
 =======================================================================================================================
 */
BOOL LoadPlugins(int type)
{
	char	AudioPath[_MAX_PATH];					/* _MAX_PATH = 260 */
	char	VideoPath[_MAX_PATH];
	char	InputPath[_MAX_PATH];
	char	StartPath[_MAX_PATH];
	BOOL	result=TRUE;
	char	errormsg[500];
	static int		Audio = 0;

	errormsg[0]=0;

	GetPluginDir(StartPath);

	if( type == LOAD_ALL_PLUGIN || type == LOAD_RSP_PLUGIN )
	{
		SetStatusBarText(0, TranslateStringByString("Loading RSP Plugin:"));

		/* Set RSP plugin path */
		strcpy(InputPath, StartPath);
		
		if(strcmp(gRegSettings.RSPPlugin, "") == 0 || strcmp(gRegSettings.RSPPlugin, "none") == 0 )
		{
			emuoptions.UsingRspPlugin = FALSE;
			rsp_plugin_is_loaded = FALSE;
			EnableMenuItem(gui.hMenu1964main, ID_RSP_CONFIG, MF_GRAYED);
			memset(emustatus.lastRSPPluginLoaded,0,sizeof(emustatus.lastRSPPluginLoaded));
		}
		else 
		{
			strcat(InputPath, gRegSettings.RSPPlugin);

			/* Load RSP plugin DLL */
			if(LoadRSPPlugin(InputPath) == FALSE)
			{
				CloseRSPPlugin();
				strcpy(gRegSettings.RSPPlugin,"");
				REGISTRY_WriteStringByIndex( IDS_KEY_RSP_PLUGIN, gRegSettings.RSPPlugin);
				rsp_plugin_is_loaded = FALSE;
				emuoptions.UsingRspPlugin = FALSE;
				EnableMenuItem(gui.hMenu1964main, ID_RSP_CONFIG, MF_GRAYED);
				result = FALSE;
				strcat(errormsg,"Failed to load RSP plugin\n");
				memset(emustatus.lastRSPPluginLoaded,0,sizeof(emustatus.lastRSPPluginLoaded));
			}
			else
			{
				rsp_plugin_is_loaded = TRUE;
				emuoptions.UsingRspPlugin = TRUE;
				EnableMenuItem(gui.hMenu1964main, ID_RSP_CONFIG, MF_ENABLED);
				SetStatusBarText(0, TranslateStringByString("Init RSP Plugin ..."));
				InitializeRSP();
				strcpy(emustatus.lastRSPPluginLoaded, gRegSettings.RSPPlugin);
			}
		}

	}

	if(type == LOAD_ALL_PLUGIN || type == LOAD_VIDEO_PLUGIN)
	{
		strcpy(VideoPath, StartPath);
		SetStatusBarText(0, TranslateStringByString("Loading Video Plugin:"));

		/* Set Video plugin path */
		if(strcmp(gRegSettings.VideoPlugin, "") == 0) 
		{
			strcpy(gRegSettings.VideoPlugin, "1964Video.dll");
			strcat(VideoPath, gRegSettings.VideoPlugin);
			REGISTRY_WriteStringByIndex( IDS_KEY_VIDEO_PLUGIN, gRegSettings.VideoPlugin);
		} 
		else 
		{
			strcat(VideoPath, gRegSettings.VideoPlugin);
		}

		/* Load Video plugin */
		if(LoadVideoPlugin(VideoPath) == FALSE)
		{
			DisplayError("Cannot load video plugin, check the file path or the plugin directory setting");
			//strcpy(gRegSettings.VideoPlugin, "");
			strcpy(gRegSettings.VideoPlugin, "1964Video.dll");
			REGISTRY_WriteStringByIndex( IDS_KEY_VIDEO_PLUGIN, gRegSettings.VideoPlugin);
			CloseVideoPlugin();
			result = FALSE;
			strcat(errormsg,"Failed to load video plugin:");
			strcat(errormsg, VideoPath);
			strcat(errormsg,"\n");
			memset(emustatus.lastVideoPluginLoaded,0,sizeof(emustatus.lastVideoPluginLoaded));
		}
		else
		{
			strcpy(generalmessage, gRegSettings.VideoPlugin);
			strcpy(emustatus.lastVideoPluginLoaded, gRegSettings.VideoPlugin);
			_strlwr(generalmessage);					/* convert to lower case */
			if(strstr(generalmessage, "gl") > 0) 
			{		/* Check if the plugin is opengl plugin */
				guioptions.ok_to_pause_at_menu = FALSE; /* We should not pause game by menu if using opengl plugin */
			}
			else if( strstr(generalmessage, "debug") > 0 || strstr(generalmessage, "dbg") > 0 )
			{
				guioptions.ok_to_pause_at_menu = FALSE;
			}
			else
			{
#ifdef _DEBUG
				guioptions.ok_to_pause_at_menu = FALSE;
#else
				guioptions.ok_to_pause_at_menu = TRUE;	/* if using D3D or other plugins, we can do it. */
#endif
			}
		}

	}

	if(type == LOAD_ALL_PLUGIN || type == LOAD_INPUT_PLUGIN)
	{
		SetStatusBarText(0, TranslateStringByString("Loading Input Plugin:"));

		/* Set Input plugin path */
		strcpy(InputPath, StartPath);
		if(strcmp(gRegSettings.InputPlugin, "") == 0) 
		{
			strcpy(gRegSettings.InputPlugin, "1964Input.dll");
			strcat(InputPath, gRegSettings.InputPlugin);
			REGISTRY_WriteStringByIndex( IDS_KEY_INPUT_PLUGIN, gRegSettings.InputPlugin);
		} 
		else 
		{
			strcat(InputPath, gRegSettings.InputPlugin);
		}

		/* Load Input plugin DLL */
		if(LoadControllerPlugin(InputPath) == FALSE) 
		{
			//strcpy(gRegSettings.InputPlugin, "");
			strcpy(gRegSettings.InputPlugin, "1964Input.dll");
			REGISTRY_WriteStringByIndex( IDS_KEY_INPUT_PLUGIN, gRegSettings.InputPlugin);
			result = FALSE;
			strcat(errormsg,"Failed to load input plugin:");
			strcat(errormsg, InputPath);
			strcat(errormsg,"\n");
			memset(emustatus.lastInputPluginLoaded,0,sizeof(emustatus.lastInputPluginLoaded));
		}
		else
		{
			/*
			* Call the CONTROLLER_InitiateControllers function in the input DLL to initiate
			* the controllers
			*/
			SetStatusBarText(0, TranslateStringByString("Init Input Plugin ..."));
			CONTROLLER_InitiateControllers(gui.hwnd1964main, Controls);
			strcpy(emustatus.lastInputPluginLoaded, gRegSettings.InputPlugin);
		}

	}

	if((type == LOAD_ALL_PLUGIN) || (type == LOAD_AUDIO_PLUGIN))
	{
		SetStatusBarText(0, TranslateStringByString("Loading Audio Plugin:"));

		/* Set path for the Audio plugin */
		strcpy(AudioPath, StartPath);
		if(strcmp(gRegSettings.AudioPlugin, "") == 0) 
		{
			strcpy(gRegSettings.AudioPlugin, "1964Aud.dll");
			strcat(AudioPath, gRegSettings.AudioPlugin);
			REGISTRY_WriteStringByIndex( IDS_KEY_AUDIO_PLUGIN, gRegSettings.AudioPlugin);
		}
		else
		{
			strcat(AudioPath, gRegSettings.AudioPlugin);
		}

		Audio_Is_Initialized = 0;
		if(LoadAudioPlugin(AudioPath) == TRUE)
		{
			Audio = 1;
			strcpy(emustatus.lastAudioPluginLoaded, gRegSettings.AudioPlugin);
		}
		else
		{
			Audio = 0;
			strcpy(gRegSettings.AudioPlugin, "1964Aud.dll");
			REGISTRY_WriteStringByIndex( IDS_KEY_AUDIO_PLUGIN, gRegSettings.AudioPlugin);
			result = FALSE;
			strcat(errormsg,"Failed to load audio plugin:");
			strcat(errormsg, AudioPath);
			strcat(errormsg,"\n");
		}

		if( _AUDIO_Initialize != NULL) 
		{
			if( AUDIO_Initialize( Audio_Info) == TRUE) 
			{
				Audio = 1;
				Audio_Is_Initialized = 1;
			} 
			else
			{
				Audio = 0;
				Audio_Is_Initialized = 0;
			}
		}

		if ( Audio_Is_Initialized )
		{
			HRESULT hr;

			__try {
				    if ( FAILED( hr = DirectSoundCreate( NULL, &lpds, NULL ) ) ) 
					{
						Audio = 0;
					}
				    if ( lpds) 
					{
						IDirectSound_Release(lpds);
					}
				}
			__except( NULL, EXCEPTION_EXECUTE_HANDLER) 
			{
				Audio = 0;
				__try 
				{
					if( lpds)
						IDirectSound_Release(lpds);
				} 
				__except( NULL, EXCEPTION_EXECUTE_HANDLER){}
			}
		}

		if(Audio == 0)
		{
			//DisplayError("Cannot load audio plugin, check the file path or the plugin directory setting");
			CloseAudioPlugin();
			memset(emustatus.lastAudioPluginLoaded,0,sizeof(emustatus.lastAudioPluginLoaded));
			//strcpy(gRegSettings.AudioPlugin, "");
			//strcpy(AudioPath, StartPath);
			//strcpy(gRegSettings.AudioPlugin, "No Audio 1964.dll");
			//strcat(AudioPath, gRegSettings.AudioPlugin);
			//REGISTRY_WriteStringByIndex( IDS_KEY_AUDIO_PLUGIN, gRegSettings.AudioPlugin);
			//LoadAudioPlugin(AudioPath);
		}
	}

	if(type == LOAD_ALL_PLUGIN )//|| type == LOAD_VIDEO_PLUGIN)
	{
		SetStatusBarText(0, TranslateStringByString("Init Video Plugin ..."));
		InitPluginData();
		VIDEO_InitiateGFX(Gfx_Info);

		MoveWindow
		(
			gui.hwnd1964main,
			guistatus.window_position.left,
			guistatus.window_position.top+2,
			guistatus.clientwidth,
			guistatus.clientheight,
			TRUE
		);
		if(guistatus.WindowIsMaximized) 
			ShowWindow(gui.hwnd1964main, SW_SHOWMAXIMIZED);

		ShowWindow(gui.hwnd1964main, SW_SHOW);

		NewRomList_ListViewChangeWindowRect();
		DockStatusBar();
	}
	else if( type == LOAD_VIDEO_PLUGIN )
	{
		SetStatusBarText(0, TranslateStringByString("Init Video Plugin ..."));
		InitPluginData();
		VIDEO_InitiateGFX(Gfx_Info);
	}

	Set_Ready_Message();

	//if (Audio == 0)
	//{
	//	SetStatusBarText(1, "dsound fail");
	//}

	if( result == FALSE )
	{
		MessageBox(0,errormsg,"Error", MB_OK);
	}
	return result;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void FreePlugins(void)
{
	CloseControllerPlugin();
	CloseAudioPlugin();
	CloseVideoPlugin();
	CloseRSPPlugin();
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void FillPluginComboList(HWND hDlg, DWORD id, int type, char* current, BOOL useDefaultPlugin)
{
	int n=0;
	int index;

	if( !useDefaultPlugin )
	{
		index = SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM) TranslateStringByString("Use system default setting"));
		if( current[0] == 0 )
		{
			SendDlgItemMessage(hDlg, id, CB_SETCURSEL, (WPARAM) index, (LPARAM) 0);
		}
	}

	if( type == PLUGIN_TYPE_RSP )
	{
		index = SendDlgItemMessage(hDlg, IDC_COMBO_RSP, CB_ADDSTRING, 0, (LPARAM) "None");
		if(_stricmp("none", current) == 0)
			SendDlgItemMessage(hDlg, id, CB_SETCURSEL, (WPARAM) index, (LPARAM) 0);
		if( current[0] == 0 && useDefaultPlugin )
			SendDlgItemMessage(hDlg, id, CB_SETCURSEL, (WPARAM) index, (LPARAM) 0);
	}

	for( n=0; n<TotalPluginDLLCount; n++ )
	{
		if(AllPluginDLLInfos[n].type == type)
		{
			index = SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM) AllPluginDLLInfos[n].info);
			if(_stricmp(AllPluginDLLInfos[n].filename, current) == 0)
				SendDlgItemMessage(hDlg, id, CB_SETCURSEL, (WPARAM) index, (LPARAM) 0);
		}
	}

	if( !useDefaultPlugin )
	{
		int num = sizeof(DesiredPluginDLLInfos)/sizeof(PluginDLLInfo);
		for( n=0; n<num; n++ )
		{
			if( DesiredPluginDLLInfos[n].type == type && DesiredPluginDLLInfos[n].filename[0] == 0 )
			{
				// User does not have this plugin in his folder
				sprintf(generalmessage,"%s (%s)", DesiredPluginDLLInfos[n].info, TranslateStringByString("not available"));
				index = SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM) generalmessage);
				if(_stricmp(DesiredPluginDLLInfos[n].info, current) == 0)
					SendDlgItemMessage(hDlg, id, CB_SETCURSEL, (WPARAM) index, (LPARAM) 0);
			}
		}
	}
}

void StorePluginSetting(void)
{
	if( strcmp( gRegSettings.VideoPlugin, temp_video_plugin) != 0) 
	{
		strcpy( gRegSettings.VideoPlugin, temp_video_plugin);
		CloseVideoPlugin();
		LoadPlugins(LOAD_VIDEO_PLUGIN);
		NewRomList_ListViewChangeWindowRect();
		DockStatusBar();
		REGISTRY_WriteStringByIndex( IDS_KEY_VIDEO_PLUGIN, gRegSettings.VideoPlugin);
	}

	if( strcmp( gRegSettings.AudioPlugin, temp_audio_plugin) != 0) 
	{
		strcpy( gRegSettings.AudioPlugin, temp_audio_plugin);
		CloseAudioPlugin();
		LoadPlugins(LOAD_AUDIO_PLUGIN);
		REGISTRY_WriteStringByIndex( IDS_KEY_AUDIO_PLUGIN, gRegSettings.AudioPlugin);
	}

	if(strcmp(gRegSettings.InputPlugin, temp_input_plugin) != 0) 
	{
		strcpy(gRegSettings.InputPlugin, temp_input_plugin);
		CloseControllerPlugin();
		LoadPlugins(LOAD_INPUT_PLUGIN);
		CONTROLLER_InitiateControllers(gui.hwnd1964main, Controls); /* Input DLL Initialization */
		REGISTRY_WriteStringByIndex( IDS_KEY_INPUT_PLUGIN, gRegSettings.InputPlugin);
	}

	if(strcmp(gRegSettings.RSPPlugin, temp_rsp_plugin) != 0) 
	{
		strcpy(gRegSettings.RSPPlugin, temp_rsp_plugin);
		CloseRSPPlugin();
		LoadPlugins(LOAD_RSP_PLUGIN);
		REGISTRY_WriteStringByIndex( IDS_KEY_RSP_PLUGIN, gRegSettings.RSPPlugin);
	}

	if( strlen(temp_rsp_plugin) < 5 )
	{
		emuoptions.UsingRspPlugin = FALSE;
	}
	else
	{
		emuoptions.UsingRspPlugin = TRUE;
	}
}

LRESULT APIENTRY PluginsDialogForROMSpecifiedPlugins(HWND hDlg, unsigned message, WORD wParam, LONG lParam);
void OnInitPluginsDialog(HWND hDlg)
{
	HWND	tempHandle;
		
	strcpy(temp_video_plugin, gRegSettings.VideoPlugin);
	strcpy(temp_audio_plugin, gRegSettings.AudioPlugin);
	strcpy(temp_input_plugin, gRegSettings.InputPlugin);
	strcpy(temp_rsp_plugin, gRegSettings.RSPPlugin);
	
	/* Reset combo boxes content */
	SendDlgItemMessage(hDlg, IDC_COMBO_VIDEO, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hDlg, IDC_COMBO_INPUT, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hDlg, IDC_COMBO_AUDIO, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hDlg, IDC_COMBO_RSP, CB_RESETCONTENT, 0, 0);
	
	tempHandle = GetDlgItem(hDlg,IDC_RSP_ABOUT);
	if( _RSPDllAbout )
		EnableWindow(tempHandle,TRUE);
	else
		EnableWindow(tempHandle,FALSE);
	
	CheckRadioButton(hDlg, IDC_DEFAULT_PLUGIN_RADIO, IDC_ROM_PLUGIN_RADIO, IDC_DEFAULT_PLUGIN_RADIO);
	SetDlgItemText(hDlg,IDC_STATIC_GROUP_VIDEO,TranslateStringByString("System Default Video Plugin"));
	SetDlgItemText(hDlg,IDC_STATIC_GROUP_AUDIO,TranslateStringByString("System Default Audio Plugin"));
	SetDlgItemText(hDlg,IDC_STATIC_GROUP_INPUT,TranslateStringByString("System Default Input Plugin"));
	SetDlgItemText(hDlg,IDC_STATIC_GROUP_RSP,TranslateStringByString("System Default RSP Plugin"));
	
	FillPluginComboList(hDlg, IDC_COMBO_VIDEO, PLUGIN_TYPE_GFX, gRegSettings.VideoPlugin, TRUE);
	FillPluginComboList(hDlg, IDC_COMBO_INPUT, PLUGIN_TYPE_CONTROLLER, gRegSettings.InputPlugin, TRUE);
	FillPluginComboList(hDlg, IDC_COMBO_AUDIO, PLUGIN_TYPE_AUDIO, gRegSettings.AudioPlugin, TRUE);
	FillPluginComboList(hDlg, IDC_COMBO_RSP, PLUGIN_TYPE_RSP, gRegSettings.RSPPlugin, TRUE);
	
	_CONTROLLER_Under_Selecting_DllAbout = _CONTROLLER_DllAbout;
	_CONTROLLER_Under_Selecting_DllTest = _CONTROLLER_DllTest;
	_VIDEO_Under_Selecting_About = _VIDEO_About;
	_VIDEO_Under_Selecting_Test = _VIDEO_Test;
	_AUDIO_Under_Selecting_About = _AUDIO_About;
	_AUDIO_Under_Selecting_Test = _AUDIO_Test;
	_RSP_Under_Selecting_DllAbout = _RSPDllAbout;
	
	tempHandle = GetDlgItem(hDlg,IDC_VID_ABOUT);
	EnableWindow(tempHandle,TRUE);
	tempHandle = GetDlgItem(hDlg,IDC_AUD_ABOUT);
	EnableWindow(tempHandle,TRUE);
	tempHandle = GetDlgItem(hDlg,IDC_DI_ABOUT);
	EnableWindow(tempHandle,TRUE);
	
}

extern void __cdecl RomListGetGoodRomNameToDisplay(char *buf, int index);

void OnInitPluginsDialogPerRom(HWND hDlg)
{
	HWND	button;
	char	buf[256];	

	/* Reset combo boxes content */
	SendDlgItemMessage(hDlg, IDC_COMBO_VIDEO, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hDlg, IDC_COMBO_INPUT, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hDlg, IDC_COMBO_AUDIO, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hDlg, IDC_COMBO_RSP, CB_RESETCONTENT, 0, 0);
	
	CheckRadioButton(hDlg, IDC_DEFAULT_PLUGIN_RADIO, IDC_ROM_PLUGIN_RADIO, IDC_ROM_PLUGIN_RADIO);
	
	if( romlist[rlstatus.selected_rom_index] )
	{
		FillPluginComboList(hDlg, IDC_COMBO_VIDEO, PLUGIN_TYPE_GFX, romlist[rlstatus.selected_rom_index]->pinientry->videoPluginName, FALSE);
		FillPluginComboList(hDlg, IDC_COMBO_AUDIO, PLUGIN_TYPE_AUDIO, romlist[rlstatus.selected_rom_index]->pinientry->audioPluginName, FALSE);
		FillPluginComboList(hDlg, IDC_COMBO_INPUT, PLUGIN_TYPE_CONTROLLER, romlist[rlstatus.selected_rom_index]->pinientry->inputPluginName, FALSE);
		FillPluginComboList(hDlg, IDC_COMBO_RSP, PLUGIN_TYPE_RSP, romlist[rlstatus.selected_rom_index]->pinientry->rspPluginName, FALSE);
	}
	
	// Disable all About buttons
	button = GetDlgItem(hDlg,IDC_RSP_ABOUT);	EnableWindow(button,FALSE);
	button = GetDlgItem(hDlg,IDC_VID_ABOUT);	EnableWindow(button,FALSE);
	button = GetDlgItem(hDlg,IDC_AUD_ABOUT);	EnableWindow(button,FALSE);
	button = GetDlgItem(hDlg,IDC_DI_ABOUT);		EnableWindow(button,FALSE);
	
	RomListGetGoodRomNameToDisplay(buf,rlstatus.selected_rom_index);
	sprintf(generalmessage,"%s : [%s]", TranslateStringByString("Video Plugin For Current ROM"), buf);
	SetDlgItemText(hDlg,IDC_STATIC_GROUP_VIDEO,generalmessage);
	sprintf(generalmessage,"%s : [%s]", TranslateStringByString("Audio Plugin For Current ROM"), buf);
	SetDlgItemText(hDlg,IDC_STATIC_GROUP_AUDIO,generalmessage);
	sprintf(generalmessage,"%s : [%s]", TranslateStringByString("Input Plugin For Current ROM"), buf);
	SetDlgItemText(hDlg,IDC_STATIC_GROUP_INPUT,generalmessage);
	sprintf(generalmessage,"%s : [%s]", TranslateStringByString("RSP Plugin For Current ROM"), buf);
	SetDlgItemText(hDlg,IDC_STATIC_GROUP_RSP,generalmessage);
}

LRESULT APIENTRY PluginsDialog(HWND hDlg, unsigned message, WORD wParam, LONG lParam)
{
	HINSTANCE		hinstLib = NULL;
	int				ComboItemNum;
	char			PluginName[300];
	char			StartPath[_MAX_PATH];
	const PluginDLLInfo	*dllinfo = NULL;
	HWND			tempHandle;

	if( !romlist[rlstatus.selected_rom_index] )
	{
		guioptions.displayDefaultPlugins = TRUE;
	}

	if( guioptions.displayDefaultPlugins == FALSE )
	{
		return PluginsDialogForROMSpecifiedPlugins(hDlg, message, wParam, lParam);
	}

	GetPluginDir(StartPath);

	switch(message)
	{
	case WM_INITDIALOG:

		TranslateDialag(hDlg, "FLUGINS");
		PopulateAllPluginInfos(MAX_PLUGIN_DLL);

		OnInitPluginsDialog(hDlg);
		if( !romlist[rlstatus.selected_rom_index] )
		{
			tempHandle = GetDlgItem(hDlg,IDC_ROM_PLUGIN_RADIO);	
			EnableWindow(tempHandle,FALSE);
			
			tempHandle = GetDlgItem(hDlg,IDC_DEFAULT_PLUGIN_RADIO);	
			EnableWindow(tempHandle,FALSE);
		}

		return(TRUE);

	
    //Propertysheet handling
	case WM_NOTIFY:
		{
		LPNMHDR lpnm = (LPNMHDR) lParam;

        switch (lpnm->code)
            {
			case PSN_APPLY:
				SendMessage(hDlg, WM_COMMAND, IDOK, lParam);
                EndDialog(lpnm->hwndFrom, TRUE);
				break;

            case PSN_RESET :
                //Handle a Cancel button click, if necessary
                EndDialog(lpnm->hwndFrom, TRUE);
				break;
			}
		}
        return(TRUE);

	case WM_COMMAND:
		{
			switch(wParam)
			{
			case IDOK:
				{
					FreeLibrary(hinstLib);
					EndDialog(hDlg, TRUE);

					StorePluginSetting();

					REGISTRY_WriteDWORD( "DisplayDefaultPlugins", guioptions.displayDefaultPlugins);

					return(TRUE);
				}

			case IDCANCEL:
				{
					EndDialog(hDlg, TRUE);
					return(TRUE);
				}

			case IDC_DOWNLOAD_PLUGINS:
				ShellExecute
				(
					gui.hwnd1964main,
					"open",
					"http://1964emu.emulation64.com/plugins.htm",
					NULL,
					NULL,
					SW_MAXIMIZE
				);
				break;

			case IDC_RSP_ABOUT:
				RSP_Under_Selecting_DllAbout(hDlg);
				break;
			case IDC_DI_ABOUT:
				CONTROLLER_Under_Selecting_DllAbout(hDlg);
				break;
			case IDC_DI_TEST:
				CONTROLLER_Under_Selecting_DllTest(hDlg);
				break;
			case IDC_VID_ABOUT:
				VIDEO_Under_Selecting_About(hDlg);
				break;
			case IDC_VID_TEST:
				VIDEO_Under_Selecting_Test(hDlg);
				break;
			case IDC_AUD_ABOUT:
				AUDIO_Under_Selecting_About(hDlg);
				break;
			case IDC_AUD_TEST:
				AUDIO_Under_Selecting_Test(hDlg);
				break;
			case IDC_ROM_PLUGIN_RADIO:
				// To display ROM specified plugins
				StorePluginSetting();
				guioptions.displayDefaultPlugins = FALSE;
				REGISTRY_WriteDWORD( "DisplayDefaultPlugins", guioptions.displayDefaultPlugins);
				
				//SendMessage(hDlg, WM_INITDIALOG, wParam, lParam);
				OnInitPluginsDialogPerRom(hDlg);
				
				break;
			}

		case CBN_SELCHANGE:
			switch(LOWORD(wParam))
			{
			case IDC_COMBO_VIDEO:
				/* Video */
				ComboItemNum = SendDlgItemMessage(hDlg, IDC_COMBO_VIDEO, CB_GETCURSEL, 0, 0);
				dllinfo = GetPluginDLLInfo(ComboItemNum,PLUGIN_TYPE_GFX);
				if( dllinfo != NULL )
				{
					strcpy(temp_video_plugin, dllinfo->filename);
					strcpy(PluginName, StartPath);
					strcat(PluginName, dllinfo->filename);
					FreeLibrary(hinstLib);
					hinstLib = LoadLibrary(PluginName);
					_VIDEO_Under_Selecting_Test = (void(__cdecl *) (HWND)) GetProcAddress(hinstLib, "DllTest");
					_VIDEO_Under_Selecting_About = (void(__cdecl *) (HWND)) GetProcAddress(hinstLib,"DllAbout");

				}
				break;
			case IDC_COMBO_AUDIO:
				// Audio
				ComboItemNum = SendDlgItemMessage(hDlg, IDC_COMBO_AUDIO, CB_GETCURSEL, 0, 0);
				dllinfo = GetPluginDLLInfo(ComboItemNum,PLUGIN_TYPE_AUDIO);
				if( dllinfo != NULL )
				{
					strcpy(temp_audio_plugin, dllinfo->filename);
					strcpy(PluginName, StartPath);
					strcat(PluginName, dllinfo->filename);
					FreeLibrary(hinstLib);
					hinstLib = LoadLibrary(PluginName);
					_AUDIO_Under_Selecting_Test = (void(__cdecl *) (HWND)) GetProcAddress(hinstLib, "DllTest");
					_AUDIO_Under_Selecting_About = (void(__cdecl *) (HWND)) GetProcAddress(hinstLib,"DllAbout");
				}

				break;
			case IDC_COMBO_INPUT:
				// Input
				ComboItemNum = SendDlgItemMessage(hDlg, IDC_COMBO_INPUT, CB_GETCURSEL, 0, 0);
				dllinfo = GetPluginDLLInfo(ComboItemNum,PLUGIN_TYPE_CONTROLLER);
				if( dllinfo != NULL )
				{
					strcpy(temp_input_plugin, dllinfo->filename);
					strcpy(PluginName, StartPath);
					strcat(PluginName, dllinfo->filename);
					FreeLibrary(hinstLib);
					hinstLib = LoadLibrary(PluginName);
					_CONTROLLER_Under_Selecting_DllTest = (void(__cdecl *) (HWND)) GetProcAddress(hinstLib, "DllTest");
					_CONTROLLER_Under_Selecting_DllAbout = (void(__cdecl *) (HWND)) GetProcAddress(hinstLib,"DllAbout");
				}

				break;
			case IDC_COMBO_RSP:
				/* RSP */
				ComboItemNum = SendDlgItemMessage(hDlg, IDC_COMBO_RSP, CB_GETCURSEL, 0, 0);
				if( ComboItemNum != 0 )
				{
					HWND but = GetDlgItem(hDlg,IDC_RSP_ABOUT);
					EnableWindow(but,TRUE);
					ComboItemNum--;
					dllinfo = GetPluginDLLInfo(ComboItemNum,PLUGIN_TYPE_RSP);
					if( dllinfo != NULL )
					{
						strcpy(temp_rsp_plugin, dllinfo->filename);
						strcpy(PluginName, StartPath);
						strcat(PluginName, dllinfo->filename);
						FreeLibrary(hinstLib);
						hinstLib = LoadLibrary(PluginName);
						_RSP_Under_Selecting_DllAbout = (void(__cdecl *) (HWND)) GetProcAddress(hinstLib,"DllAbout");
					}
				}
				else
				{
					HWND but = GetDlgItem(hDlg,IDC_RSP_ABOUT);
					_RSP_Under_Selecting_DllAbout = NULL;
					EnableWindow(but,FALSE);
					strcpy(temp_rsp_plugin, "");
				}

				break;
			}
		}
	}

	return(FALSE);
}


void StoreROMSpecifiedPluginSetting(HWND hDlg)
{
	int				ComboItemNum;
	const PluginDLLInfo	*dllinfo = NULL;

	if( !romlist[rlstatus.selected_rom_index] )
		return;

	/* Video */
	ComboItemNum = SendDlgItemMessage(hDlg, IDC_COMBO_VIDEO, CB_GETCURSEL, 0, 0);
	if( ComboItemNum == 0 )
	{
		romlist[rlstatus.selected_rom_index]->pinientry->videoPluginName[0] = 0;
	}
	else
	{
		ComboItemNum--;
		dllinfo = GetPluginDLLInfo(ComboItemNum,PLUGIN_TYPE_GFX);
		if( dllinfo != NULL )
		{
			if( dllinfo->filename[0] != 0 )
				strcpy( romlist[rlstatus.selected_rom_index]->pinientry->videoPluginName, dllinfo->filename);
			else
				strcpy( romlist[rlstatus.selected_rom_index]->pinientry->videoPluginName, dllinfo->info);
		}
	}

	ComboItemNum = SendDlgItemMessage(hDlg, IDC_COMBO_AUDIO, CB_GETCURSEL, 0, 0);
	if( ComboItemNum == 0 )
	{
		romlist[rlstatus.selected_rom_index]->pinientry->audioPluginName[0] = 0;
	}
	else
	{
		ComboItemNum--;
		dllinfo = GetPluginDLLInfo(ComboItemNum,PLUGIN_TYPE_AUDIO);
		if( dllinfo != NULL )
		{
			if( dllinfo->filename[0] != 0 )
				strcpy( romlist[rlstatus.selected_rom_index]->pinientry->audioPluginName, dllinfo->filename);
			else
				strcpy( romlist[rlstatus.selected_rom_index]->pinientry->audioPluginName, dllinfo->info);
		}
	}

	ComboItemNum = SendDlgItemMessage(hDlg, IDC_COMBO_INPUT, CB_GETCURSEL, 0, 0);
	if( ComboItemNum == 0 )
	{
		romlist[rlstatus.selected_rom_index]->pinientry->inputPluginName[0] = 0;
	}
	else
	{
		ComboItemNum--;
		dllinfo = GetPluginDLLInfo(ComboItemNum,PLUGIN_TYPE_CONTROLLER);
		if( dllinfo != NULL )
		{
			if( dllinfo->filename[0] != 0 )
				strcpy(romlist[rlstatus.selected_rom_index]->pinientry->inputPluginName, dllinfo->filename);
			else
				strcpy(romlist[rlstatus.selected_rom_index]->pinientry->inputPluginName, dllinfo->info);
		}
	}

	ComboItemNum = SendDlgItemMessage(hDlg, IDC_COMBO_RSP, CB_GETCURSEL, 0, 0);
	if( ComboItemNum == 0 )
	{
		romlist[rlstatus.selected_rom_index]->pinientry->rspPluginName[0] = 0;
	}
	else if( ComboItemNum == 1 )
	{
		strcpy(romlist[rlstatus.selected_rom_index]->pinientry->rspPluginName, "none");
	}
	else
	{
		ComboItemNum -= 2;
		dllinfo = GetPluginDLLInfo(ComboItemNum,PLUGIN_TYPE_RSP);
		if( dllinfo != NULL )
		{
			if( dllinfo->filename[0] != 0 )
				strcpy(romlist[rlstatus.selected_rom_index]->pinientry->rspPluginName, dllinfo->filename);
			else
				strcpy(romlist[rlstatus.selected_rom_index]->pinientry->rspPluginName, dllinfo->info);
		}
	}

	FileIO_Write1964Ini();
}

LRESULT APIENTRY PluginsDialogForROMSpecifiedPlugins(HWND hDlg, unsigned message, WORD wParam, LONG lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:

		TranslateDialag(hDlg, "FLUGINS");
		PopulateAllPluginInfos(MAX_PLUGIN_DLL);
	
		OnInitPluginsDialogPerRom(hDlg);
		return(TRUE);

		//Propertysheet handling
	case WM_NOTIFY:
		{
			LPNMHDR lpnm = (LPNMHDR) lParam;

			switch (lpnm->code)
			{
			case PSN_APPLY:
				SendMessage(hDlg, WM_COMMAND, IDOK, lParam);
				EndDialog(lpnm->hwndFrom, TRUE);
				break;

			case PSN_RESET :
				//Handle a Cancel button click, if necessary
				EndDialog(lpnm->hwndFrom, TRUE);
				break;
			}
		}
		return(TRUE);

	case WM_COMMAND:
		{
			switch(wParam)
			{
			case IDOK:
				{
					StoreROMSpecifiedPluginSetting(hDlg);

					EndDialog(hDlg, TRUE);
					REGISTRY_WriteDWORD( "DisplayDefaultPlugins", guioptions.displayDefaultPlugins);

					return(TRUE);
				}

			case IDCANCEL:
				{
					if( AllPluginDLLInfos ) 
					{
						free(AllPluginDLLInfos); 
						AllPluginDLLInfos=NULL;
					}
					EndDialog(hDlg, TRUE);
					return(TRUE);
				}

			case IDC_DOWNLOAD_PLUGINS:
				ShellExecute
					(
					gui.hwnd1964main,
					"open",
					"http://1964emu.emulation64.com/plugins.htm",
					NULL,
					NULL,
					SW_MAXIMIZE
					);
				break;

			case IDC_DEFAULT_PLUGIN_RADIO:
				StoreROMSpecifiedPluginSetting(hDlg);
				guioptions.displayDefaultPlugins = TRUE;
				REGISTRY_WriteDWORD( "DisplayDefaultPlugins", guioptions.displayDefaultPlugins);

				//SendMessage(hDlg, WM_INITDIALOG, wParam, lParam);
				OnInitPluginsDialog(hDlg);
				
				break;
			}
		}
	}

	return(FALSE);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void GetPluginDir(char *Directory)
{
	/*
	 * strcpy(Directory,main_directory); ?	 * strcat(Directory,"Plugin\\");
	 */
	strcpy(Directory, directories.plugin_directory_to_use);
}

/*
 =======================================================================================================================
    Redistribution Conditions Window
 =======================================================================================================================
 */
LRESULT APIENTRY ConditionsDialog(HWND hDlg, unsigned message, WORD wParam, LONG lParam)
{
	/*~~~~~~~~~~~~~~~~~~~~~~*/
	char	Conditions[11201];
	/*~~~~~~~~~~~~~~~~~~~~~~*/

	switch(message)
	{
	case WM_INITDIALOG:
		TranslateDialag(hDlg, "REDISTRIB_DIALOG");
		/* LoadString(gui.hInst, IDS_REDIST0, temp_buf, 4096); */
		LoadGNUDistConditions(Conditions);
		SetDlgItemText(hDlg, IDC_EDIT0, Conditions);
		return(TRUE);

	case WM_COMMAND:
		if(wParam == IDOK || wParam == IDCANCEL)
		{
			EndDialog(hDlg, TRUE);
			return(TRUE);
		}
		break;
	}

	return(FALSE);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
extern char SSEReason[0xff];
LRESULT APIENTRY About(HWND hDlg, unsigned message, WORD wParam, LONG lParam)
{
	switch(message)
	{
	case WM_INITDIALOG: 
		TranslateDialag(hDlg, "ABOUTBOX");
        if (IsSSESupported())
            SetDlgItemText(hDlg, IDC_IS_SSE_SUPPORTED, TranslateStringByString("Yes"));

        else
            SetDlgItemText(hDlg, IDC_IS_SSE_SUPPORTED, TranslateStringByString("No"));

        SetDlgItemText(hDlg, IDC_SSE_CAUSE, TranslateStringByString(SSEReason));
        
        return(TRUE);

	
    //Propertysheet handling
	case WM_NOTIFY:
		{
		LPNMHDR lpnm = (LPNMHDR) lParam;

        switch (lpnm->code)
            {
			case PSN_APPLY:
				SendMessage(hDlg, WM_COMMAND, IDOK, lParam);
                EndDialog(lpnm->hwndFrom, TRUE);
				break;

            case PSN_RESET :
                //Handle a Cancel button click, if necessary
                EndDialog(lpnm->hwndFrom, TRUE);
				break;
			}
		}
        return(TRUE);
    

    
    case WM_COMMAND:
        if(wParam == IDOK || wParam == IDCANCEL)
            { 
                EndDialog(hDlg, TRUE); 
                return(TRUE); 
            }
            break;
	}

	return(FALSE);
}


char cmdLineParameterBuf[250] = {0};
void SaveCmdLineParameter(char *cmdline)
{
	strcpy(cmdLineParameterBuf, cmdline);
	if( strlen(cmdLineParameterBuf) > 0 )
	{
		int i;
		int len = strlen(cmdLineParameterBuf);
		for( i=0; i<len; i++ )
		{
			if( isupper(cmdLineParameterBuf[i]) )
			{
				cmdLineParameterBuf[i] = tolower(cmdLineParameterBuf[i]);
			}
		}
	}
}

//To get a command line parameter if available, please pass a flag
// Flags:
//	"-v"	-> return video plugin name
//	"-a"	-> return audio plugin name
//  "-c"	-> return controller plugin name
//  "-g"	-> return game name to run
//	"-f"	-> return play-in-full-screen flag
//	"-r"	-> return rom path
char *CmdLineArgFlags[] =
{
	"-a",
	"-v",
	"-c",
	"-r",
	"-g",
	"-f"
};

void GetCmdLineParameter(CmdLineParameterType arg, char *buf)
{
	char *ptr1;
	char *ptr2 = buf;

	if( arg >= CMDLINE_MAX_NUMBER || (ptr1 = strstr(cmdLineParameterBuf,CmdLineArgFlags[arg]))==NULL )
	{
		buf[0] = 0;
		return;
	}
	
	if( arg == CMDLINE_FULL_SCREEN_FLAG )
	{
		strcpy(buf, "1");
		return;
	}

	ptr1 += strlen(CmdLineArgFlags[arg]);	//Skip the flag
	ptr1++;	//Skip a white space
	if (strncmp (ptr1, "\"",1)==0) {
	    ptr1++;   //skipping first "
	    while( !(strncmp (ptr1, "\"",1)==0) && (*ptr1 != 0)) {
	        *ptr2++ = *ptr1++;  
	}
        }
    else {
	while( !isspace(*ptr1) && *ptr1 != 0)
	{
		*ptr2++ = *ptr1++;
	};
	 }
	*ptr2 = 0;
}


/************************************************************************/
/* Multiple language support date structures                            */
/************************************************************************/
#define MAX_LANGUAGE_MENU_ENTRY		100
#define MAX_LANGUAGE_DIALOG_ENTRY	30
#define MAX_LANGUAGE_DIALOG_BOXS	10

enum {
	FILE_MENU=10000,
	LANGUAGE_MENU,
	SAVESTATE_MENU,
	LOADSTATE_MENU,
	RECENT_FOLDER_MENU,
	RECENT_ROM_MENU,
	EDIT_MENU,
	EMULATION_MENU,
	SWITCH_COMPILER_MENU,
	COUNTER_FACTOR_MENU,
	VARIABLE_SPEED_MENU,
	PLUGIN_MENU,
	NETPLAY_MENU,
	NETPLAY_LAG_MENU,
	HELP_MENU,
	POPUP_MENU1,
	POPUP_MENU2,
};

// Map item # from language file to resource id and accelerator
typedef struct {
	int	langID;			// item # in the language file
	DWORD	resID;		// resource id
	char*	defaultStr;
	char*	acc;		// Accelerator
	char*	langStr;
} LangMapEntry;

LangMapEntry langMapEntries[] =
{
	{1,		FILE_MENU,	"&File",				0,	0},
	{2,		ID_OPENROM,	"&Open ROM...",			"Ctrl+O",	0},
	{3,		ID_CLOSEROM,	"Close ROM",		0,	0},
	{4,		LANGUAGE_MENU,	"Language",			0,	0},
	{5,		ID_LANGUAGE_ENGLISH,	"English",	0,	0},
	{6,		SAVESTATE_MENU,	"Save State",		0,	0},
	{7,		ID_SAVESTATE,	"To File...",		"Ctrl+F5",	0},
	{8,		ID_CPU_EXPORTPJ64STATE,	"To  Project64 Save State File...",		0,	0},
	{9,		ID_SAVE_1964085,	"To 1964.085 File...",						0,	0},
	{10,	ID_SAVE_STATE_0,	"To Slot 0",	0,	0},
	{11,	ID_SAVE_STATE_1,	"To Slot 1",	0,	0},
	{12,	ID_SAVE_STATE_2,	"To Slot 2",	0,	0},
	{13,	ID_SAVE_STATE_3,	"To Slot 3",	0,	0},
	{14,	ID_SAVE_STATE_4,	"To Slot 4",	"F5",	0},
	{15,	ID_SAVE_STATE_5,	"To Slot 5",	0,	0},
	{16,	ID_SAVE_STATE_6,	"To Slot 6",	0,	0},
	{17,	ID_SAVE_STATE_7,	"To Slot 7",	0,	0},
	{18,	ID_SAVE_STATE_8,	"To Slot 8",	0,	0},
	{19,	ID_SAVE_STATE_9,	"To Slot 9",	0,	0},
	{20,	LOADSTATE_MENU,		"Load State",	0,	0},
	{21,	ID_LOADSTATE,	"From File...",		"Ctrl+F7",	0},
	{22,	ID_CPU_IMPORTPJ64STATE,	"From Project64 Save State File...",	0,	0},
	{23,	ID_LOAD_STATE_0,	"From Slot 0",		0,	0},
	{24,	ID_LOAD_STATE_1,	"From Slot 1",		0,	0},
	{25,	ID_LOAD_STATE_2,	"From Slot 2",		0,	0},
	{26,	ID_LOAD_STATE_3,	"From Slot 3",		0,	0},
	{27,	ID_LOAD_STATE_4,	"From Slot 4",	"F7",	0},
	{28,	ID_LOAD_STATE_5,	"From Slot 5",		0,	0},
	{29,	ID_LOAD_STATE_6,	"From Slot 6",		0,	0},
	{30,	ID_LOAD_STATE_7,	"From Slot 7",		0,	0},
	{31,	ID_LOAD_STATE_8,	"From Slot 8",		0,	0},
	{32,	ID_LOAD_STATE_9,	"From Slot 9",		0,	0},
	{33,	ID_CHANGEDIRECTORY,	"Change ROM Folder...",	"Ctrl+D",	0},
	{34,	ID_FILE_FRESHROMLIST,	"Refresh ROM List",	"Ctrl+L",	0},
	{35,	RECENT_FOLDER_MENU,	"Recent ROM Folders",	0,	0},
	{36,	RECENT_ROM_MENU,	"Recent ROMs",			0,	0},
	{37,	ID_EXIT,	"E&xit",	0,	0},

	{38,	EDIT_MENU,	"&Edit",	0,	0},
	{39,	ID_FILE_CHEAT,	"Cheat Codes...",				"Ctrl+C",	0},
	{40,	ID_PREFERENCE_OPTIONS,	"Window Options...",	"Ctrl+W",	0},
	{41,	ID_FILE_ROMINFO,	"ROM Properties...",		"Ctrl+S",	0},

	{42,	EMULATION_MENU,		"E&mulation",	0,	0},
	{43,	ID_ROM_PAUSE,	"&Pause/Play",		"F3",	0},
	{44,	ID_ROM_START,	"Reset",			"F2",	0},
	{45,	ID_ROM_STOP,	"&Stop",			"F4",	0},
	{46,	ID_CPU_KILL,	"Kill CPU Thread",	"Ctrl+F4",	0},
	{47,	SWITCH_COMPILER_MENU,	"Switch Compiler",			0,	0},
	{48,	ID_INTERPRETER,	"Interpreter",						0,	0},
	{49,	ID_DYNAMICCOMPILER,	"Dynamic Compiler (Default)",	0,	0},
	{50,	ID_CHEATS_APPLY,	"Inject Gameshark Green Button Cheats",	"F8",	0},
	{51,	COUNTER_FACTOR_MENU,	"Counter Factor",			0,	0},
	{52,	ID_EMULATION_AUTOCFTIMING,	"Automatic",			"F10",	0},
	{53,	ID_CF_CF1,	"1 x .5 Half Rate",				"Ctrl+1",	0},
	{54,	ID_CF_CF2,	"1 x 1 Full Rate",				"Ctrl+2",	0},
	{55,	ID_CF_CF3,	"2 x 1 Half Rate (Default)",	"Ctrl+3",	0},
	{56,	ID_CF_CF4,	"2 x 2 Full Rate",				"Ctrl+4",	0},
	{57,	ID_CF_CF5,	"3 x 1.5 Half Rate",			"Ctrl+5",	0},
	{58,	ID_CF_CF6,	"3 x 3 Full Rate",				"Ctrl+6",	0},
	{59,	ID_CF_CF7,	"4 x 2 Half Rate",				"Ctrl+7",	0},
	{60,	ID_CF_CF8,	"8 x 4 Half Rate",				"Ctrl+8",	0},
	{61,	ID_CPU_AUDIOSYNC,	"Speed Limit",			"F9",	0},
	{62,	ID_EMULATION_AUTOFRAMESKIP,	"Automatic Frame Skip",	"F11",	0},
	{63,	ID_PLUGINS_SCREENSHOTS,	"Screen Shot",				"F12",	0},
	{64,	VARIABLE_SPEED_MENU,	"Variable Speed Limits",				0,	0},
	{65,	ID_VARIABLESPEEDLIMITS_INCREASESPEED,	"Increase Speed",				"Num (+)",	0},
	{66,	ID_VARIABLESPEEDLIMITS_DECREASESPEED,	"Decrease Speed",				"Num (-)",	0},
	{67,	ID_VARIABLESPEEDLIMITS_RESTORECORRECTSPEED,	"Restore correct Speed",	"Num (*)",	0},
	{68,	PLUGIN_MENU,	"&Plug-ins",				0,	0},
	{69,	IDM_PLUGINS,	"Change Plug-ins...",		"Ctrl+P",	0},
	{70,	ID_VIDEO_CONFIG,	"&Video Settings...",	"Ctrl+V",	0},
	{71,	ID_AUD_CONFIG,	"&Audio Settings...",		"Ctrl+A",	0},
	{72,	ID_DI_CONFIG,	"&Input Settings...",		"Ctrl+I",	0},
	{73,	ID_RSP_CONFIG,	"RSP Settings...",			"Ctrl+R",	0},
	{74,	IDM_FULLSCREEN,	"Full Screen",				"Alt+Enter",	0},
	{75,	NETPLAY_MENU,	"Net Play",		0,	0},
	{76,	ID_KAILLERA_MODE,	"Connect to Kaillera Server ...",	0,	0},
	{77,	NETPLAY_LAG_MENU,	"Lag",		0,	0},
	{78,	ID_LAGNESS_1,	"1 (less)",		0,	0},
	{79,	ID_LAGNESS_2,	"2",			0,	0},
	{80,	ID_LAGNESS_3,	"3",			0,	0},
	{81,	ID_LAGNESS_4,	"4 (default)",	0,	0},
	{82,	ID_LAGNESS_5,	"5",			0,	0},
	{83,	ID_LAGNESS_6,	"6",			0,	0},
	{84,	ID_LAGNESS_7,	"7 (more)",		0,	0},
	{85,	HELP_MENU,		"&Help",			0,	0},
	{86,	ID_ONLINE_HELP,	"Online Help",	0,	0},
	{87,	ID_HELP_HELP,	"Help Manual",	"F1",	0},
	{88,	ID_CHECKWEB,		"&1964 Home Page",			0,	0},
	{89,	ID_REDISTRIBUTE,	"GPL Information...",	0,	0},
	{90,	ID_ABOUT_WARRANTY,	"About Warranty...",	0,	0},
	{91,	ID_ABOUT,			"&About 1964...",				0,	0},
	{92,	ID_POPUP_LOADPLAYINWINDOWMODE,	"Load && Play in Window",	0,	0},
	{95,	ID_POPUP_LOADPLAYINFULLSCREEN,	"Load && Play in Full Screen",	0,	0},
	{96,	ID_POPUP_CHEATCODE,				"Cheat Codes...",	0,	0},
	{97,	ID_POPUPMENU_CHANGEROMDIRECTORY,	"Change Rom Folder...",	0,	0},
	{98,	ID_POPUP_ROM_SETTING,	"ROM Properties...",	0,	0},
	{99,	POPUP_MENU1,			"Header Popup 1",		0,	0},
	{100,	ID_HEADERPOPUP_SHOWALTERNATEROMNAME,	"Show Alternate ROM Name",	0,	0},
	{101,	ID_HEADERPOPUP_SHOW_INTERNAL_NAME,	"Show Internal ROM Name",		0,	0},
	{102,	ID_HEADERPOPUP_SHOWROMFILENAME,		"Show ROM File Name",	0,	0},
	{103,	ID_HEADERPOPUP_1_SORT_ASCENDING,	"Sort Ascending",		0,	0},
	{104,	ID_HEADERPOPUP_1_SORT_DESCENDING,	"Sort Descending",		0,	0},
	{105,	ID_HEADERPOPUP_1_SELECTING,			"Add/Remove Columns...",	0,	0},
	{106,	POPUP_MENU2,						"Header Popup 2",						0,	0},
	{107,	ID_HEADERPOPUP_2_SORT_ASCENDING,	"Sort Asccending",		0,	0},
	{108,	ID_HEADERPOPUP_2_SORT_DESCENDING,	"Sort Descending",		0,	0},
	{109,	ID_HEADERPOPUP_2_SELECTING,			"Add/Remove Columns...",	0,	0},
	{110,	ID_POPUPMENU_PLUGINSETTING,			"ROM Plugin Setting...",	0,	0},
	{111,	ID_POPUPMENU_ROMINFORMATION,		"ROM Information...",	0,	0},

	// Add by 1964 1.0
	{112,	ID_POPUPMENU_ASSIGNBOXARTIMAGE,		"Assign Box Art Image ...",	0,	0},
	{113,	ID_POPUPMENU_REMOVEBOXARTIMAGELINK,	"Remove Box Art Image Link",	0,	0},
	{114,	ID_HEADERPOPUP3_SHOWROMLIST,		"Show Rom List",	0,	0},
	{115,	ID_HEADERPOPUP3_SHOWBOXART,			"Show Box Art",	0,	0},
	{116,	ID_BOXARTIMAGE_ZOOM_IN,				"Increase Image Size",	0,	0},
	{117,	ID_BOXARTIMAGE_ZOOM_OUT,			"Decrease Image Size",	0,	0},
	{118,	ID_BOXARTIMAGE_INCREASEXSPACING,	"Increase X Spacing",	0,	0},
	{119,	ID_BOXARTIMAGE_DECREASEXSPACING,	"Decrease X Spacing",	0,	0},
	{120,	ID_BOXARTIMAGE_INCREASEYSPACING,	"Increase Y Spacing",	0,	0},
	{121,	ID_BOXARTIMAGE_DECREASEYSPACING,	"Decrease Y Spacing",	0,	0},
	{122,	ID_HEADERPOPUP_SHOWALTERNATEROMNAME,	"Show Alternate ROM Name",	0,	0},
	{123,	ID_HEADERPOPUP_SHOW_INTERNAL_NAME,	"Show Internal ROM Name",	0,	0},
	{124,	ID_HEADERPOPUP_SHOWROMFILENAME,		"Show ROM File Name",	0,	0},
	{125,	ID_HEADERPOPUP_1_SORT_ASCENDING,	"Sort Asccending",	0,	0},
	{126,	ID_HEADERPOPUP_1_SORT_DESCENDING,	"Sort Descending",	0,	0},
	{127,	ID_HEADERPOPUP_1_SELECTING,			"Add/Remove Columns...",	0,	0},
	{128,	ID_HEADERPOPUP3_REFRESH,			"Refresh ROM List",	0,	0},
};

int totalLangMapEntries= sizeof(langMapEntries)/sizeof(LangMapEntry);

char* TranslateMenuStr(DWORD resid, char** acc)
{
	int i;
	for(i=0; i<totalLangMapEntries; i++)
	{
		if( langMapEntries[i].resID == resid )
		{
			*acc = langMapEntries[i].acc;
			if( langMapEntries[i].langStr )
				return langMapEntries[i].langStr;
			else
				return langMapEntries[i].defaultStr;
		}
	}

	acc = NULL;
	return NULL;
}

/************************************************************************/
/* Multiple language support routines                                   */
/************************************************************************/
char *languageFileNames[200];	// upto 200 languages
extern HMENU file_submenu;
void CheckLanguages()
{
	/************************************************************************/
	/* Examine the language file folder, create the language menu for each  */
	/* available languages.                                                 */
	/************************************************************************/
	
	HMENU file_submenu = GetSubMenu(gui.hMenu1964main, 0);
	WIN32_FIND_DATA libaa;
	HANDLE			FindFirst;
	char			SearchPath[_MAX_PATH];
	char			fullpath[MAX_PATH];
	HMENU			language_submenu = GetSubMenu(file_submenu, 3);


	sprintf(SearchPath, "%slanguage\\", directories.main_directory);
	strcat(SearchPath, "*.lng");

	guioptions.totalLanguages = 0;
	guioptions.currentLanguageID = -1;

	FindFirst = FindFirstFile(SearchPath, &libaa);
	if(FindFirst != INVALID_HANDLE_VALUE)
	{
		int currentLanguageID = -1;

		do{
			if( stricmp(libaa.cFileName,"english.lng") == 0 ) continue;
			sprintf(fullpath, "%slanguage\\%s",directories.main_directory, libaa.cFileName);
			if( PathFileExists(fullpath) == FALSE )	continue;
			{
				FILE *fp = fopen(fullpath,"r");
				if( fp )
					fclose(fp);
				else
					continue;
			}

			if( !languageFileNames[guioptions.totalLanguages]) 
				languageFileNames[guioptions.totalLanguages] = (char*)VirtualAlloc(0,256,MEM_COMMIT, PAGE_READWRITE);

			if( languageFileNames[guioptions.totalLanguages] )
			{
				strcpy(languageFileNames[guioptions.totalLanguages], libaa.cFileName);

				strcpy(generalmessage, libaa.cFileName);
				generalmessage[strlen(generalmessage)-4]=0;;
				AppendMenu
					(
					language_submenu,
					MF_ENABLED|MF_STRING,
					NEW_LANGUAGE_MENU_START+guioptions.totalLanguages,
					generalmessage
					);

				if( stricmp(guioptions.language,generalmessage) == 0 )
				{
					currentLanguageID = guioptions.totalLanguages;
				}

				guioptions.totalLanguages++;
			}
		}
		while(FindNextFile(FindFirst, &libaa));

		if( currentLanguageID >= 0 )
		{
			// Force to use new language
			strcpy(guioptions.language,"english");
			SwitchLanguage(currentLanguageID, FALSE);
		}

	}
}


char *DialogNameList[] = {
	"ABOUTBOX",
	"CHEAT_HACK",
	"COL_SELECT",
	"CRITICAL_MESSAGE",
	"FOLDERS",
	"OPTIONS",
	"FLUGINS",
	"REDISTRIB_DIALOG",
	"ROM_INFO",
	"ROM_OPTIONS",
	"UNAVAILABLE",
};

DlgLangMapEntry AboutBoxDialogItems[]=
{
	{IDC_STATIC_TEXT1,0,0},
	{IDC_STATIC_TEXT4,0,0},
	{IDC_STATIC_TEXT3,0,0},
	{IDC_STATIC_TEXT2,0,0},
	{IDC_STATIC_BOX_TITLE,0,0},
	{IDC_STATIC_GROUP_2,0,0},
	{IDC_STATIC_TEXT5,0,0},
	{IDC_STATIC_TEXT6,0,0},
};

DlgLangMapEntry CheatHackDialogItems[]=
{
	{IDC_DEFAULTOPTIONS_AUTOCHEAT,0,0},
	{IDC_STATIC_CODEFRAME,0,0},
	{IDC_GAMESHARK_ACTIVATE,0,0},
	{IDC_GAMESHARK_DEACTIVATE,0,0},
	{IDC_GAMESHARK_DEACTIVATEALL,0,0},
	{IDC_STATIC_NEWCODEFRAME,0,0},
	{IDC_STATIC_NAME,0,0},
	{IDC_STATIC_CHEAT_COUNTRY,0,0},
	{IDC_GAMESHARK_ADD,0,0},
	{IDC_GAMESHARK_DELETE,0,0},
	{IDC_STATIC_CHEAT_NOTE1,0,0},
};

DlgLangMapEntry ColSelectDialogItems[]=
{
	{IDC_OPTION_ROMBROWSER,0,0},
	{IDC_STATIC_GROUP1,0,0},
	{IDC_ALT_NAME,0,0},
	{IDC_INTERNAL_NAME,0,0},
	{IDC_ROM_FILENAME,0,0},
	{IDC_STATIC_GROUP2,0,0},
};


DlgLangMapEntry CriticalMessageDialogItems[]=
{
	{IDOK,0,0},
	{ID_CLEAR_MESSAGE,0,0},
};



DlgLangMapEntry FolderDialogItems[]=
{
	{IDC_STATIC_GROUP1,0,0},
	{IDC_DEFAULTOPTIONS_UseLastRomDirectory,0,0},
	{IDC_STATIC_GROUP2,0,0},
	{IDC_DEFAULTOPTIONS_USE1964STATESAVEDIRECTORY,0,0},
	{IDC_STATIC_GROUP3,0,0},
	{IDC_DEFAULTOPTIONS_USE1964PLUGINDIRECTORY,0,0},
};

DlgLangMapEntry OptionsDialogItems[]=
{
	{IDC_STATIC_GROUP1,0,0},
	{IDC_ENABLE_DIRECTORY_LIST,0,0},
	{IDC_STATIC_TXT1,0,0},
	{IDC_ENABLE_GAME_LIST,0,0},
	{IDC_STATIC_TXT2,0,0},
	{IDC_STATIC_GROUP2,0,0},
	{IDC_DEFAULTOPTIONS_PAUSEWHENINACTIVE,0,0},
	{IDC_OPTION_AUTOFULLSCREEN,0,0},
	{IDC_OPTION_ERROR_WINDOW,0,0},
	{IDC_STATIC_GROUP3,0,0},
	{IDC_ENABLE_DETAIL_STATUS,0,0},
	{IDC_ENABLE_PROFILER,0,0},
	{IDC_NONE,0,0},
};


DlgLangMapEntry PluginsDialogItems[]=
{
	{IDC_VID_ABOUT,0,0},
	{IDC_AUD_ABOUT,0,0},
	{IDC_DI_ABOUT,0,0},
	{IDC_RSP_ABOUT,0,0},
	{IDC_DOWNLOAD_PLUGINS,0,0},
	{IDC_STATIC_GROUP_AUDIO,0,0},
	{IDC_STATIC_GROUP_VIDEO,0,0},
	{IDC_STATIC_GROUP_RSP,0,0},
	{IDC_STATIC_GROUP_INPUT,0,0},
	{IDC_DEFAULT_PLUGIN_RADIO,0,0},
	{IDC_ROM_PLUGIN_RADIO,0,0},
};

DlgLangMapEntry RedistribDialogItems[]=
{
	{IDOK,0,0},
};


DlgLangMapEntry ROMInfoDialogItems[]=
{
	{IDC_STATIC_TXT1,0,0},
	{IDC_STATIC_TXT3,0,0},
	{IDC_STATIC_TXT6,0,0},
	{IDC_STATIC_TXT2,0,0},
};


DlgLangMapEntry ROMOptionsDialogItems[]=
{
	{IDC_STATIC_GROUP1,0,0},
	{IDC_STATIC_TXT1,0,0},
	{IDC_STATIC_TXT3,0,0},
	{IDC_STATIC_TXT5,0,0},
	{IDC_STATIC_TXT2,0,0},
	{IDC_STATIC_TXT4,0,0},
	{IDC_STATIC_TXT6,0,0},
	{IDC_STATIC_GROUP2,0,0},
	{IDC_STATIC_TXT7,0,0},
	{IDC_STATIC_TXT9,0,0},
	{IDC_STATIC_TXT11,0,0},
	{IDC_STATIC_TXT13,0,0},
	{IDC_STATIC_TXT8,0,0},
	{IDC_STATIC_TXT10,0,0},
	{IDC_STATIC_TXT12,0,0},
	{IDC_STATIC_TXT14,0,0},
	{IDC_STATIC_GROUP3,0,0},
	{IDC_STATIC_TXT15,0,0},
	{IDC_STATIC_TXT16,0,0},
	{IDC_STATIC_GROUP4,0,0},
	{IDC_STATIC_GROUP5,0,0},
};

DlgLangMapEntry UnavailableDialogItems[]=
{
	{IDC_STATIC_TXT1,0,0},
};


const int NumOfDialogs = sizeof(DialogNameList)/sizeof(char*);
DlgLangMapEntry *DlgLangMapEntryGroups[]=
{
	AboutBoxDialogItems,
	CheatHackDialogItems,
	ColSelectDialogItems,
	CriticalMessageDialogItems,
	FolderDialogItems,
	OptionsDialogItems,
	PluginsDialogItems,
	RedistribDialogItems,
	ROMInfoDialogItems,
	ROMOptionsDialogItems,
	UnavailableDialogItems,
};

const int DlgLangMapEntryCounts[] =
{
	sizeof(AboutBoxDialogItems)/sizeof(DlgLangMapEntry),
	sizeof(CheatHackDialogItems)/sizeof(DlgLangMapEntry),
	sizeof(ColSelectDialogItems)/sizeof(DlgLangMapEntry),
	sizeof(CriticalMessageDialogItems)/sizeof(DlgLangMapEntry),
	sizeof(FolderDialogItems)/sizeof(DlgLangMapEntry),
	sizeof(OptionsDialogItems)/sizeof(DlgLangMapEntry),
	sizeof(PluginsDialogItems)/sizeof(DlgLangMapEntry),
	sizeof(RedistribDialogItems)/sizeof(DlgLangMapEntry),
	sizeof(ROMInfoDialogItems)/sizeof(DlgLangMapEntry),
	sizeof(ROMOptionsDialogItems)/sizeof(DlgLangMapEntry),
	sizeof(UnavailableDialogItems)/sizeof(DlgLangMapEntry),
};



StringMapEntry StringMapEntries[] = {
	{0,0,0, "Cheat Codes"},
	{1,0,0, "Window"},
	{2,0,0, "Plug-ins"},
	{3,0,0, "ROM Properties"},
	{4,0,0, "ROM Browser"},
	{5,0,0, "ROM Information"},
	{6,0,0, "Folders"},
	{7,0,0, "About 1964"},
	{8,0,0, "ROM Name"},
	{9,0,0, "Country"},
	{10,0,0, "Size"},
	{11,0,0, "Compatibility Status"},
	{12,0,0, "Save Type"},
	{13,0,0, "CIC Chip"},
	{14,0,0, "Players"},
	{15,0,0, "Open ROM"},
	{16,0,0, "Select a ROM folder"},
	{17,0,0, "Exit"},
	{18,0,0, "Could not get a windows handle."},
	{19,0,0, "Load Rom Setting from ROM_Properties.ini"},
	{20,0,0, "Looking for ROM files in the ROM directory and Generating List"},
	{21,0,0, "Error"},
	{22,0,0, "CreateWindow() failed: Cannot create a window."},
	{23,0,0, "Running"},
	{24,0,0, "Loaded"},
	{25,0,0, "Kaillera player %d dropped"},
	{26,0,0, "Saving State"},
	{27,0,0, "Loading State"},
	{28,0,0, "State File"},
	{29,0,0, "All Files"},
	{30,0,0, "Select Game Save Folder"},
	{31,0,0, "Select Plugin Folder"},
	{32,0,0, "Select ROM Folder"},
	{33,0,0, "Stopped"},
	{34,0,0, "Ready"},
	{35,0,0, "Loading RSP Plugin:"},
	{36,0,0, "Init RSP Plugin ..."},
	{37,0,0, "Loading Video Plugin:"},
	{38,0,0, "Init Video Plugin ..."},
	{39,0,0, "Loading Input Plugin:"},
	{40,0,0, "Init Input Plugin ..."},
	{41,0,0, "Loading Audio Plugin:"},
	{42,0,0, "not available"},
	{43,0,0, "System Default Video Plugin"},
	{44,0,0, "System Default Audio Plugin"},
	{45,0,0, "System Default Input Plugin"},
	{46,0,0, "System Default RSP Plugin"},
	{47,0,0, "Video Plugin For Current ROM"},
	{48,0,0, "Audio Plugin For Current ROM"},
	{49,0,0, "Input Plugin For Current ROM"},
	{50,0,0, "RSP Plugin For Current ROM"},
	{51,0,0, "Error opening this file. Please refresh the ROM list."},
	{52,0,0, "Exception in emulation thread"},
	{53,0,0, "File could not be opened."},
	{54,0,0, "This savestate is incompatible with this version of 1964."},
	{55,0,0, "Version Conflict"},
	{56,0,0, "Loading"},
	{57,0,0, "Kaillera timeout"},
	{58,0,0, "Upload game setting to other players"},
	{59,0,0, "Upload cheat codes to other players"},
	{60,0,0, "Kaillera starts the game"},
	{61,0,0, "Kaillera wait for 1 second"},
	{62,0,0, "default"},
	{63,0,0, "Default"},
	{64,0,0, "No"},
	{65,0,0, "Yes"},
	{66,0,0, "Dyna-Compiler"},
	{67,0,0, "Interpreter"},
	{68,0,0, "EEPROM"},
	{69,0,0, "MEMPAK"},
	{70,0,0, "SRAM"},
	{71,0,0, "FLASHRAM"},
	{72,0,0, "First Used"},
	{73,0,0, "All Used Types"},
	{74,0,0, "No Check"},
	{75,0,0, "Check DMA only"},
	{76,0,0, "Check DWORD"},
	{77,0,0, "Check QWORD"},
	{78,0,0, "Check QWORD & DMA"},
	{79,0,0, "Check Block"},
	{80,0,0, "Check Block & DMA"},
	{81,0,0, "Protect Memory"},
	{82,0,0, "No Limit"},
	{83,0,0, "Auto Sync"},
	{84,0,0, "No EEPROM"},
	{85,0,0, "4Kb EEPROM"},
	{86,0,0, "16Kb EEPROM"},
	{87,0,0, "All Countries"},
	{88,0,0, "USA - NTSC"},
	{89,0,0, "Japan - NTSC"},
	{90,0,0, "USA & Japan - NTSC"},
	{91,0,0, "Europe - PAL"},
	{92,0,0, "Australia - PAL"},
	{93,0,0, "France - PAL"},
	{94,0,0, "Germany - PAL"},
	{95,0,0, "(country code mismatch)"},
	{96,0,0, "Updating ROM browser..."},
	{97,0,0, "All Countries"},
	{98,0,0, "USA - NTSC"},
	{99,0,0, "Japan - NTSC"},
	{100,0,0,	"Are you sure to delete this code?"},
	{101,0,0,	"No Hardware Support Detected"},
	{102,0,0,	"Disabled in this build"},
	{103,0,0,	"Hardware and OS Support Detected"},
	{104,0,0,	"Hardware Support, but No OS Support Detected"},
	{105,0,0,	"Use system default setting"},
	{106,0,0,	"Full Screen (Alt+Enter)"},
	{107,0,0,	"1964 Home Page"},
	{108,0,0,	"ROM Properties (Ctrl+S)"},
	{109,0,0,	"Open (Ctrl+O)"},
	{110,0,0,	"Pause (F3)"},
	{111,0,0,	"Play (F3)"},
	{112,0,0,	"Stop (F4)"},
	{113,0,0,	"Change Plugins (Ctrl+P)"},
	{114,0,0,	"Reset (F2)"},
	{115,0,0,	"Help (F1)"},
	{116,0,0,	"Automatic Frame Skip (F11)"},
	{117,0,0,	"Speed Limit (F9)"},
	{118,0,0,	"Automatic CF Timing Adjust  (F10)"},
	{119,0,0,	"Inject Green Button Cheat (F8)"},
	{120,0,0,	"Paused"},
	{121,0,0,	"core"},
	{122,0,0,	"video"},
	{123,0,0,	"audio"},
	{124,0,0,	"compiler"},
	{125,0,0,	"idle"},
	{126,0,0,	"netplay"},
	{127,0,0,	"kaillera"},
	{128,0,0,	"Demo"},
	{129,0,0,	"Beta"},
	{130,0,0,	"USA/Japan"},
	{131,0,0,	"German"},
	{132,0,0,	"France"},
	{133,0,0,	"Italy"},
	{134,0,0,	"Japan"},
	{135,0,0,	"Europe"},
	{136,0,0,	"Spain"},
	{137,0,0,	"Australia"},
};

const int NumOfStringMapEntries = sizeof(StringMapEntries)/sizeof(StringMapEntry);

char* ConvertSpecialCharacters(char *str)
{
	char temp[1024];
	int i,j;
	int len=strlen(str);
	temp[0]=0;

	for(i=0,j=0; i<len; i++)
	{
		switch(str[i])
		{
		case '\\':
			if( str[i+1] == 'n' )
			{
				temp[j++]='\n';
				i++;
			}
			else if( str[i+1] == '\\' )
			{
				temp[j++]='\\';
				i++;
			}
			else if( str[i+1] == 't')
			{
				temp[j++]='\t';
				i++;
			}
			break;
		default:
			temp[j++]=str[i];
			break;
		}
	}

	temp[j]=0;

	strcpy(str,temp);
	return str;
}

BOOL LoadLanguageIntoMemory(char *filename)
{
	/************************************************************************/
	/* Given the filename, load language strings into memory, to the        */
	/* id-string matching table.                                            */
	/************************************************************************/

	/************************************************************************/
	/* Language file format:                                                */
	/* =====================                                                */
	/* 1) comments lines start with '!', all lines starting with '!' will   */
	/*    be ignored. All lines starting with ' ' will be ignored.          */
	/* 2) A new dialog section as "Dialog n", n=1,2,3,4,5,6,...             */
	/* 3) data line format: "n,string", n is the id of the string           */
	/* 4) The whole file is arranged as:                                    */
	/*    comments                                                          */
	/*    menu data                                                         */
	/*    dialog data                                                       */
	/* 5) File name will be used as the language name                       */
	/************************************************************************/

	FILE *file;
	char line[1024];
	char *text;
	int count=0;
	int id;
	int dialogID = -1;
	int dialogItemCount=0;
	int menuEntryCount=0;
	int stringCount=0;

	char path[MAX_PATH];

	sprintf(path, "%slanguage\\%s",directories.main_directory, filename);

	file = fopen(path,"r");
	if( file )
	{
		while( fgets(line, 1023, file) )
		{
			int len=strlen(line);
			if( len > 0 && line[len-1] == '\n' )	len--;
			line[len]=0;	// chop the newline character

			if( line[0] == '!' ) continue;	// this is a comment line, skip it
			if( line[0] == ' ' ) continue;	// this is an empty line, skip it
			if( line[0] == 0 ) continue;	// this is an empty line, skip it
			if( strncmp(line, "Dialog=",7) == 0 )
			{
				// Start a new dialog section
				dialogID = atol(line+7);
				dialogItemCount = 0;
				continue;
			}

			if( strncmp(line, "String Section", 14) == 0 )
			{
				dialogID = NumOfDialogs;	// Start the String section
				continue;
			}

			if( line[0] < '0' || line[0] > '9' ) continue;	// this is not a number-leading line, skip it


			id = atoi(line);
			text = strchr(line,',');
			if( text != NULL )
			{
				text++;

				ConvertSpecialCharacters(text);

				if( dialogID<0 )
				{
					// Menu strings
					if( menuEntryCount<totalLangMapEntries )
					{
						VirtualSafeFree( langMapEntries[menuEntryCount].langStr );
						langMapEntries[menuEntryCount].langID = id;
						langMapEntries[menuEntryCount].langStr = (char*)VirtualAlloc(0, strlen(text)+2, MEM_COMMIT, PAGE_READWRITE);
						if( langMapEntries[menuEntryCount].langStr )
						{
							strcpy(langMapEntries[menuEntryCount].langStr, text);
							menuEntryCount++;
						}
					}
				}
				else if( dialogID < NumOfDialogs )
				{
					// Dialog box item string
					if( dialogItemCount < DlgLangMapEntryCounts[dialogID] )
					{
						VirtualSafeFree( DlgLangMapEntryGroups[dialogID][dialogItemCount].langStr );
						DlgLangMapEntryGroups[dialogID][dialogItemCount].langStr = (char*)VirtualAlloc(0, strlen(text)+2, MEM_COMMIT, PAGE_READWRITE);

						if( DlgLangMapEntryGroups[dialogID][dialogItemCount].langStr )
						{
							DlgLangMapEntryGroups[dialogID][dialogItemCount].langID = id;
							strcpy(DlgLangMapEntryGroups[dialogID][dialogItemCount].langStr, text);
							dialogItemCount++;
						}
					}
				}
				else if( dialogID == NumOfDialogs )
				{
					// String section
					if( stringCount<NumOfStringMapEntries )
					{
						VirtualSafeFree( StringMapEntries[stringCount].langStr );
						StringMapEntries[stringCount].langID = id;
						StringMapEntries[stringCount].langStr = (char*)VirtualAlloc(0, strlen(text)+2, MEM_COMMIT, PAGE_READWRITE);
						if( StringMapEntries[stringCount].langStr )
						{
							strcpy(StringMapEntries[stringCount].langStr, text);
							stringCount++;
						}
					}

				}
			}
		}

		fclose(file);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void UpdateAllMenus()
{
	/************************************************************************/
	/* Update all menus by using the language string loaded in the memory   */
	/************************************************************************/
}

void UpdateDialogBoxTexts(int dialogboxid)
{
	/************************************************************************/
	/* This function should be called when the dialogbox is initialized.    */
	/* Given the dialogbox id, this function will update all item texts.    */
	/************************************************************************/
}



char * TranslateStringByID(DWORD id)
{
	/************************************************************************/
	/* Given an id, this function returns a constant string pointer.        */
	/* The string is loaded from language files, and to be used as messages */
	/************************************************************************/

	int i;

	for( i=0; i<NumOfStringMapEntries; i++ )
	{
		if( StringMapEntries[i].resID == id )
		{
			if( StringMapEntries[i].langStr )
				return StringMapEntries[i].langStr;
			else
				return StringMapEntries[i].oriStr;
		}
	}

	return NULL;
}

char * TranslateStringByString(char *oriStr)
{
	int i;

	for( i=0; i<NumOfStringMapEntries; i++ )
	{
		if( strcmp(StringMapEntries[i].oriStr, oriStr) == 0 )
		{
			if( StringMapEntries[i].langStr )
				return StringMapEntries[i].langStr;
			else
				return StringMapEntries[i].oriStr;
		}
	}

	return oriStr;
}

void SetMenuTranslatedString(HMENU hMenu,int elementID, DWORD resid)
{
	char *acc;
	char *langStr;
	char str[80];
	MENUITEMINFO menuinfo;

	langStr = TranslateMenuStr(resid,&acc);

	if(langStr)
	{
		strcpy(str, langStr);
		if ( acc && acc[0] ) sprintf(str, "%s\t%s", langStr, acc);

		memset(&menuinfo, 0, sizeof(MENUITEMINFO));
		menuinfo.cbSize = sizeof(MENUITEMINFO);
		menuinfo.fMask = MIIM_TYPE;
		menuinfo.fType = MFT_STRING;
		menuinfo.dwTypeData = str;
		menuinfo.cch = strlen(str);
		SetMenuItemInfo(hMenu,elementID,TRUE,&menuinfo);
	}
}
void TranslateMenu(HMENU hMenu, HWND mainHWND)
{
	HMENU submenu,subsubmenu;

	//Main menu
	SetMenuTranslatedString(hMenu,0,FILE_MENU);
	SetMenuTranslatedString(hMenu,1,EDIT_MENU);
	SetMenuTranslatedString(hMenu,2,EMULATION_MENU);
	SetMenuTranslatedString(hMenu,3,PLUGIN_MENU);
	SetMenuTranslatedString(hMenu,4,NETPLAY_MENU);
	SetMenuTranslatedString(hMenu,5,HELP_MENU);

	//File menu
	submenu = GetSubMenu(hMenu,0) ;
	SetMenuTranslatedString(submenu,0,ID_OPENROM);
	SetMenuTranslatedString(submenu,1,ID_CLOSEROM);

	SetMenuTranslatedString(submenu,3,LANGUAGE_MENU);
	subsubmenu = GetSubMenu(submenu,3) ;
	//SetMenuTranslatedString(subsubmenu,0,ID_LANGUAGE_ENGLISH);

	SetMenuTranslatedString(submenu,5,SAVESTATE_MENU);
	subsubmenu = GetSubMenu(submenu,5) ;
	SetMenuTranslatedString(subsubmenu,0,ID_SAVESTATE);
	SetMenuTranslatedString(subsubmenu,1,ID_CPU_EXPORTPJ64STATE);
	SetMenuTranslatedString(subsubmenu,2,ID_SAVE_1964085);
	SetMenuTranslatedString(subsubmenu,4,ID_SAVE_STATE_0);
	SetMenuTranslatedString(subsubmenu,5,ID_SAVE_STATE_1);
	SetMenuTranslatedString(subsubmenu,6,ID_SAVE_STATE_2);
	SetMenuTranslatedString(subsubmenu,7,ID_SAVE_STATE_3);
	SetMenuTranslatedString(subsubmenu,8,ID_SAVE_STATE_4);
	SetMenuTranslatedString(subsubmenu,9,ID_SAVE_STATE_5);
	SetMenuTranslatedString(subsubmenu,10,ID_SAVE_STATE_6);
	SetMenuTranslatedString(subsubmenu,11,ID_SAVE_STATE_7);
	SetMenuTranslatedString(subsubmenu,12,ID_SAVE_STATE_8);
	SetMenuTranslatedString(subsubmenu,13,ID_SAVE_STATE_9);

	SetMenuTranslatedString(submenu,6,LOADSTATE_MENU);
	subsubmenu = GetSubMenu(submenu,6) ;
	SetMenuTranslatedString(subsubmenu,0,ID_LOADSTATE);
	SetMenuTranslatedString(subsubmenu,1,ID_CPU_IMPORTPJ64STATE);
	SetMenuTranslatedString(subsubmenu,3,ID_LOAD_STATE_0);
	SetMenuTranslatedString(subsubmenu,4,ID_LOAD_STATE_1);
	SetMenuTranslatedString(subsubmenu,5,ID_LOAD_STATE_2);
	SetMenuTranslatedString(subsubmenu,6,ID_LOAD_STATE_3);
	SetMenuTranslatedString(subsubmenu,7,ID_LOAD_STATE_4);
	SetMenuTranslatedString(subsubmenu,8,ID_LOAD_STATE_5);
	SetMenuTranslatedString(subsubmenu,9,ID_LOAD_STATE_6);
	SetMenuTranslatedString(subsubmenu,10,ID_LOAD_STATE_7);
	SetMenuTranslatedString(subsubmenu,11,ID_LOAD_STATE_8);
	SetMenuTranslatedString(subsubmenu,12,ID_LOAD_STATE_9);

	SetMenuTranslatedString(submenu,8,ID_CHANGEDIRECTORY);
	SetMenuTranslatedString(submenu,9,ID_FILE_FRESHROMLIST);
	SetMenuTranslatedString(submenu,10,RECENT_FOLDER_MENU);
	SetMenuTranslatedString(submenu,11,RECENT_ROM_MENU);
	
	SetMenuTranslatedString(submenu,13,ID_EXIT);

	//Edit menu
	submenu = GetSubMenu(hMenu,1) ;
	SetMenuTranslatedString(submenu,0,ID_FILE_CHEAT);
	SetMenuTranslatedString(submenu,1,ID_PREFERENCE_OPTIONS);
	SetMenuTranslatedString(submenu,3,ID_FILE_ROMINFO);

	//Emulation menu
	submenu = GetSubMenu(hMenu,2) ;
	SetMenuTranslatedString(submenu,0,ID_ROM_PAUSE);
	SetMenuTranslatedString(submenu,1,ID_ROM_START);
	SetMenuTranslatedString(submenu,2,ID_ROM_STOP);
	SetMenuTranslatedString(submenu,3,ID_CPU_KILL);

	SetMenuTranslatedString(submenu,5,SWITCH_COMPILER_MENU);
	subsubmenu = GetSubMenu(submenu,5) ;
	SetMenuTranslatedString(subsubmenu,0,ID_INTERPRETER);
	SetMenuTranslatedString(subsubmenu,1,ID_DYNAMICCOMPILER);

	SetMenuTranslatedString(submenu,6,ID_CHEATS_APPLY);

	SetMenuTranslatedString(submenu,8,COUNTER_FACTOR_MENU);
	subsubmenu = GetSubMenu(submenu,8) ;
	SetMenuTranslatedString(subsubmenu,0,ID_EMULATION_AUTOCFTIMING);
	SetMenuTranslatedString(subsubmenu,2,ID_CF_CF1);
	SetMenuTranslatedString(subsubmenu,3,ID_CF_CF2);
	SetMenuTranslatedString(subsubmenu,4,ID_CF_CF3);
	SetMenuTranslatedString(subsubmenu,5,ID_CF_CF4);
	SetMenuTranslatedString(subsubmenu,6,ID_CF_CF5);
	SetMenuTranslatedString(subsubmenu,7,ID_CF_CF6);
	SetMenuTranslatedString(subsubmenu,8,ID_CF_CF7);
	SetMenuTranslatedString(subsubmenu,9,ID_CF_CF8);

	SetMenuTranslatedString(submenu,9,ID_CPU_AUDIOSYNC);
	SetMenuTranslatedString(submenu,10,ID_EMULATION_AUTOFRAMESKIP);
	SetMenuTranslatedString(submenu,11,ID_PLUGINS_SCREENSHOTS);

	SetMenuTranslatedString(submenu,12,VARIABLE_SPEED_MENU);
	subsubmenu = GetSubMenu(submenu,12) ;
	SetMenuTranslatedString(subsubmenu,0,ID_VARIABLESPEEDLIMITS_INCREASESPEED);
	SetMenuTranslatedString(subsubmenu,1,ID_VARIABLESPEEDLIMITS_DECREASESPEED);
	SetMenuTranslatedString(subsubmenu,2,ID_VARIABLESPEEDLIMITS_RESTORECORRECTSPEED);

	// Plugin menu
	submenu = GetSubMenu(hMenu,3) ;
	SetMenuTranslatedString(submenu,0,IDM_PLUGINS);
	SetMenuTranslatedString(submenu,2,ID_VIDEO_CONFIG);
	SetMenuTranslatedString(submenu,3,ID_AUD_CONFIG);
	SetMenuTranslatedString(submenu,4,ID_DI_CONFIG);
	SetMenuTranslatedString(submenu,5,ID_RSP_CONFIG);
	SetMenuTranslatedString(submenu,7,IDM_FULLSCREEN);

	// Netplay menu
	submenu = GetSubMenu(hMenu,4) ;
	SetMenuTranslatedString(submenu,0,ID_KAILLERA_MODE);
	SetMenuTranslatedString(submenu,2,NETPLAY_LAG_MENU);
	subsubmenu = GetSubMenu(submenu,2) ;
	SetMenuTranslatedString(subsubmenu,0,ID_LAGNESS_1);
	SetMenuTranslatedString(subsubmenu,1,ID_LAGNESS_2);
	SetMenuTranslatedString(subsubmenu,2,ID_LAGNESS_3);
	SetMenuTranslatedString(subsubmenu,3,ID_LAGNESS_4);
	SetMenuTranslatedString(subsubmenu,4,ID_LAGNESS_5);
	SetMenuTranslatedString(subsubmenu,5,ID_LAGNESS_6);
	SetMenuTranslatedString(subsubmenu,6,ID_LAGNESS_7);

	// Help menu
	submenu = GetSubMenu(hMenu,5) ;
	SetMenuTranslatedString(submenu,0,ID_HELP_HELP);
	SetMenuTranslatedString(submenu,1,ID_ONLINE_HELP);
	SetMenuTranslatedString(submenu,2,ID_CHECKWEB);
	//SetMenuTranslatedString(submenu,4,ID_REDISTRIBUTE);
	SetMenuTranslatedString(submenu,4,ID_ABOUT_WARRANTY);
	SetMenuTranslatedString(submenu,5,ID_ABOUT);

	DrawMenuBar(mainHWND);
}


void TranslateRomPopupMenu(HMENU hMenu)
{
	if( guioptions.currentLanguageID >= 0 )
	{
		SetMenuTranslatedString(hMenu,0,ID_POPUP_LOADPLAYINWINDOWMODE);
		SetMenuTranslatedString(hMenu,1,ID_POPUP_LOADPLAYINFULLSCREEN);
		SetMenuTranslatedString(hMenu,3,ID_POPUP_CHEATCODE);
		SetMenuTranslatedString(hMenu,5,ID_POPUPMENU_CHANGEROMDIRECTORY);
		SetMenuTranslatedString(hMenu,7,ID_POPUPMENU_ROMINFORMATION);
		SetMenuTranslatedString(hMenu,8,ID_POPUPMENU_PLUGINSETTING);
		SetMenuTranslatedString(hMenu,9,ID_POPUPMENU_ASSIGNBOXARTIMAGE);
		SetMenuTranslatedString(hMenu,10,ID_POPUPMENU_REMOVEBOXARTIMAGELINK);
		SetMenuTranslatedString(hMenu,12,ID_POPUP_ROM_SETTING);
	}
}

void TranslateColumnPopupMenu1(HMENU hMenu)
{
	if( guioptions.currentLanguageID >= 0 )
	{
		SetMenuTranslatedString(hMenu,0,ID_HEADERPOPUP_SHOWALTERNATEROMNAME);
		SetMenuTranslatedString(hMenu,1,ID_HEADERPOPUP_SHOW_INTERNAL_NAME);
		SetMenuTranslatedString(hMenu,2,ID_HEADERPOPUP_SHOWROMFILENAME);
		SetMenuTranslatedString(hMenu,4,ID_HEADERPOPUP_1_SORT_ASCENDING);
		SetMenuTranslatedString(hMenu,5,ID_HEADERPOPUP_1_SORT_DESCENDING);
		SetMenuTranslatedString(hMenu,7,ID_HEADERPOPUP_1_SELECTING);
	}
}

void TranslateColumnPopupMenu2(HMENU hMenu)
{
	if( guioptions.currentLanguageID >= 0 )
	{
		SetMenuTranslatedString(hMenu,0,ID_HEADERPOPUP_2_SORT_ASCENDING);
		SetMenuTranslatedString(hMenu,1,ID_HEADERPOPUP_2_SORT_DESCENDING);
		SetMenuTranslatedString(hMenu,3,ID_HEADERPOPUP_2_SELECTING);
	}
}

void TranslatePopupMenu3(HMENU hMenu)
{
	if( guioptions.currentLanguageID >= 0 )
	{
		SetMenuTranslatedString(hMenu,0,ID_HEADERPOPUP3_SHOWROMLIST);
		SetMenuTranslatedString(hMenu,1,ID_HEADERPOPUP3_SHOWBOXART);
		SetMenuTranslatedString(hMenu,3,ID_BOXARTIMAGE_ZOOM_IN);
		SetMenuTranslatedString(hMenu,4,ID_BOXARTIMAGE_ZOOM_OUT);
		SetMenuTranslatedString(hMenu,5,ID_BOXARTIMAGE_INCREASEXSPACING);
		SetMenuTranslatedString(hMenu,6,ID_BOXARTIMAGE_DECREASEXSPACING);
		SetMenuTranslatedString(hMenu,7,ID_BOXARTIMAGE_INCREASEYSPACING);
		SetMenuTranslatedString(hMenu,8,ID_BOXARTIMAGE_DECREASEYSPACING);
		SetMenuTranslatedString(hMenu,10,ID_HEADERPOPUP_SHOWALTERNATEROMNAME);
		SetMenuTranslatedString(hMenu,11,ID_HEADERPOPUP_SHOW_INTERNAL_NAME);
		SetMenuTranslatedString(hMenu,12,ID_HEADERPOPUP_SHOWROMFILENAME);
		SetMenuTranslatedString(hMenu,13,ID_HEADERPOPUP_1_SORT_ASCENDING);
		SetMenuTranslatedString(hMenu,14,ID_HEADERPOPUP_1_SORT_DESCENDING);
		SetMenuTranslatedString(hMenu,16,ID_HEADERPOPUP_1_SELECTING);
		SetMenuTranslatedString(hMenu,18,ID_HEADERPOPUP3_REFRESH);
	}
}

extern void OnFreshRomList (BOOL reload);
void ResetToDefaultLanguage()
{
	/************************************************************************/
	/* Reset to English, reload all menus from 1964.exe resource            */
	/************************************************************************/
	
	if( guioptions.currentLanguageID >= 0 )
	{
		strcpy(guioptions.language,"english");
		REGISTRY_WriteStringByName("Language", guioptions.language);

		LoadLanguageIntoMemory("english.lng");
		TranslateMenu(gui.hMenu1964main,gui.hwnd1964main);
		CheckMenuItem(gui.hMenu1964main, NEW_LANGUAGE_MENU_START+guioptions.currentLanguageID, MF_UNCHECKED);
		CheckMenuItem(gui.hMenu1964main, ID_LANGUAGE_ENGLISH, MF_CHECKED);
		guioptions.currentLanguageID = -1;

		OnFreshRomList(FALSE);
	}
}

void SwitchLanguage(int id, BOOL refreshRomList)
{
	/************************************************************************/
	/* This function is called from the WINGUI when a user is clicked on    */
	/* a language menu to switch to another language.                       */
	/************************************************************************/

	if( id < guioptions.totalLanguages && id >= 0 )
	{
		if( strcmp(guioptions.language,languageFileNames[id]) != 0 )
		{
			if( LoadLanguageIntoMemory(languageFileNames[id]) )
			{
				strcpy(generalmessage, languageFileNames[id]);
				generalmessage[strlen(generalmessage)-4]=0;
				strcpy(guioptions.language,generalmessage);
				REGISTRY_WriteStringByName("Language", guioptions.language);

				TranslateMenu(gui.hMenu1964main,gui.hwnd1964main);

				if( guioptions.currentLanguageID == -1 )
				{
					CheckMenuItem(gui.hMenu1964main, ID_LANGUAGE_ENGLISH, MF_UNCHECKED);
				}
				else
				{
					CheckMenuItem(gui.hMenu1964main, NEW_LANGUAGE_MENU_START+guioptions.currentLanguageID, MF_UNCHECKED);
				}

				guioptions.currentLanguageID = id;
				CheckMenuItem(gui.hMenu1964main, NEW_LANGUAGE_MENU_START+guioptions.currentLanguageID, MF_CHECKED);

				if( refreshRomList )
					OnFreshRomList(FALSE);
			}
		}
	}
}


void ManageMenus(void)
{
	SetupAdvancedMenus();
	SetupDebuger();

	DeleteMenu(gui.hMenu1964main, ID_OPCODEDEBUGGER, MF_BYCOMMAND);
	DeleteMenu(gui.hMenu1964main, ID_OPCODEDEBUGGER_BLOCK_ONLY, MF_BYCOMMAND);
	DeleteMenu(gui.hMenu1964main, ID_DIRTYONLY, MF_BYCOMMAND);
}

void FreeDlgLangMapEntries(void)
{
	int i;
	for( i=0; i<NumOfDialogs; i++ )
	{
		int j;
		for( j=0; j<DlgLangMapEntryCounts[i]; j++ )
		{
			VirtualSafeFree( DlgLangMapEntryGroups[i][j].langStr );
		}
	}


	for( i=0; i<totalLangMapEntries; i++ )
	{
		VirtualSafeFree( langMapEntries[i].langStr );
	}

	for( i=0; i<NumOfStringMapEntries; i++ )
	{
		VirtualSafeFree( StringMapEntries[i].langStr );
	}
}

void TranslateDialag(HWND hDlg, char *dlgName)
{
	int dlg;
	int i;

	if( stricmp(guioptions.language,"english") != 0 )
	{
		for( dlg = 0; dlg < NumOfDialogs; dlg++ )
		{
			if( strcmp(DialogNameList[dlg], dlgName) == 0 )
				break;
		}

		if( dlg == NumOfDialogs )
		{
			TRACE1("Cannot find dialog box name = %s", dlgName);
			return;	// Can not find the dialog box by the name
		}

		if( DlgLangMapEntryGroups[dlg] && DlgLangMapEntryCounts[dlg] )
		{
			for( i=0; i<DlgLangMapEntryCounts[dlg]; i++ )
			{
				if( DlgLangMapEntryGroups[dlg][i].langStr )
					SetDlgItemText(hDlg, DlgLangMapEntryGroups[dlg][i].resID, DlgLangMapEntryGroups[dlg][i].langStr);
			}
		}	
	}

}

void SetXPThemes(DWORD flag)
{
	void (__cdecl *g_pfnSetThemeAppProperties)(DWORD) = NULL;
	HMODULE hModUXTheme = LoadLibrary("uxtheme.dll");

	if ( hModUXTheme != NULL )
	{
		g_pfnSetThemeAppProperties =	(void (__cdecl *)(DWORD))GetProcAddress(hModUXTheme,"SetThemeAppProperties");
	}
	else
	{
		g_pfnSetThemeAppProperties = NULL;
	}

	if (g_pfnSetThemeAppProperties != NULL ) 
	{
		g_pfnSetThemeAppProperties(flag);
	} 
}
