// registry.c

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

#define MAIN_1964_KEY		"Software\\1964emu_099\\GUI"

char	szData[MAX_PATH];
RegSettingsTyp	gRegSettings;

//----------------------------------------------------------------
// Open connection to the system registry
BOOL
REGISTRY_OpenConnection( PHKEY hKey1, 
					     PHKEY hKey2)
{
	char	szBuffer[260];

	strcpy( szBuffer, MAIN_1964_KEY);
	if( RegConnectRegistry( NULL, HKEY_CURRENT_USER, hKey1) == ERROR_SUCCESS)
		if( RegOpenKey( *hKey1, szBuffer, hKey2) != ERROR_SUCCESS)
			if( RegCreateKey( *hKey1, szBuffer, hKey2) != ERROR_SUCCESS)
				return FALSE;

	return TRUE;
}

//----------------------------------------------------------------
// Close connection to system registry
BOOL REGISTRY_CloseConnection( PHKEY hMainKey, PHKEY hSubKey)
{
	RegCloseKey( *hSubKey);
	RegCloseKey( *hMainKey);
	return TRUE;
}

//----------------------------------------------------------------
// Read a string from the system registry
char* REGISTRY_ReadSTRING( char *Field, char *szDefaultValue)
{
	HKEY		hKey1, 
				hKey2;
	DWORD		cbData, 
				dwType = REG_SZ;

	if( REGISTRY_OpenConnection( &hKey1, &hKey2)) 
	{
		cbData = sizeof( szData);
		if( RegQueryValueEx( hKey2, Field, NULL, &dwType, (LPBYTE)szData,  &cbData)
			!= ERROR_SUCCESS) {
			strcpy(szData, szDefaultValue);
			cbData = strlen( szData)+1;
			RegSetValueEx( hKey2, Field, 0, REG_SZ, (LPBYTE)szData, cbData);
		}
		REGISTRY_CloseConnection( &hKey1, &hKey2);
	}

	return szData;
}

//----------------------------------------------------------------
// Write a string to the system registry
void REGISTRY_WriteStringByIndex( DWORD dwIndex, // Index from string table
	                      char *szValue)
{
	HKEY		hKey1, 
				hKey2;
	DWORD		cbData, 
				dwType = REG_SZ;

	if( REGISTRY_OpenConnection( &hKey1, &hKey2)) 
	{
		LoadString(gui.hInst, dwIndex, (LPSTR)Scratch1, _MAX_PATH);
		strcpy(szData, szValue);
		cbData = strlen( szData)+1;
		RegSetValueEx( hKey2, (LPCSTR)Scratch1, 0, REG_SZ, (LPBYTE)szData, cbData);
		REGISTRY_CloseConnection( &hKey1, &hKey2);
	}
}

void REGISTRY_WriteStringByName( char *name,char *szValue)
{
	HKEY		hKey1, 
				hKey2;
	DWORD		cbData, 
				dwType = REG_SZ;

	if( REGISTRY_OpenConnection( &hKey1, &hKey2)) 
	{
		strcpy(szData, szValue);
		cbData = strlen( szData)+1;
		RegSetValueEx( hKey2, name, 0, REG_SZ, (LPBYTE)szData, cbData);
		REGISTRY_CloseConnection( &hKey1, &hKey2);
	}
}

//----------------------------------------------------------------
// Read a DWORD from the system registry
DWORD REGISTRY_ReadDWORD( char *szFieldName, DWORD dwDefaultValue)
{
	HKEY		hKey1, 
				hKey2;
	DWORD		cbData = 4, 
				dwData,
				dwType = REG_DWORD;

	if( REGISTRY_OpenConnection( &hKey1, &hKey2)) {
		if( RegQueryValueEx( hKey2, szFieldName, NULL, &dwType, (LPBYTE)&dwData, &cbData)
			!= ERROR_SUCCESS) {
			dwData = dwDefaultValue;
			RegSetValueEx( hKey2, szFieldName, 0, REG_DWORD, (LPBYTE)dwData, cbData);
		}
		REGISTRY_CloseConnection( &hKey1, &hKey2);
	}

	return dwData;
}

//----------------------------------------------------------------
// Write a DWORD to the system registry
void
REGISTRY_WriteDWORD( char *szFieldName, 
				     DWORD dwValue)
{
	HKEY	hKey1 = NULL, 
			hKey2 = NULL;
	DWORD	cbData = 4, 
			dwData;

	REGISTRY_OpenConnection( &hKey1, &hKey2);

	dwData = dwValue;
	RegSetValueEx(hKey2, szFieldName, 0, REG_DWORD, (LPBYTE) & dwData, cbData);

	REGISTRY_CloseConnection(&hKey1, &hKey2);
}

//----------------------------------------------------------------
// Acquires string from command line or system registry
void
GetStringParameter(	char* szDest,			   // Returns the string 
					DWORD dwParam,		       // Command line parameter
					DWORD dwStringName,		   // Value from stringtable which
											   // Holds string name
					DWORD dwDefaultStringName) // Default string name
{
	char	szTemp[_MAX_PATH];

	LoadString(gui.hInst, dwStringName, (LPSTR)Scratch1, _MAX_PATH);
	LoadString(gui.hInst, dwDefaultStringName, (LPSTR)Scratch2, _MAX_PATH);

	GetCmdLineParameter( (CmdLineParameterType)dwParam, szTemp);		
	strcpy( szDest, (strlen( szTemp) != 0) ? szTemp : // Get parameter from command line
		REGISTRY_ReadSTRING( (char*)Scratch1, (char*)Scratch2));// Get parameter from registry
}

//----------------------------------------------------------------
// Gets a 1964 folder
void REGISTRY_GetDirectory( char* szDest,						// Returns the string
		               char* default_plugin_directory, 
					   char* user_set_plugin_directory,
					   BOOL* bUseDefault,					// Returns flag for default path
					   DWORD dwIDS_PLUGIN_DEFAULT_PATH,
					   DWORD dwIDS_KEY_PLUGIN_PATH,
					   DWORD dwIDS_PLUGIN_USE_DEFAULT,
					   int GettingROMPath)					//The logic is reversed for ROMPath
{
	if( !GettingROMPath) {
		// Build default directory name
		LoadString(gui.hInst, dwIDS_PLUGIN_DEFAULT_PATH, (LPSTR)Scratch0, 700);
		strcpy( default_plugin_directory, directories.main_directory);
		strcat( default_plugin_directory, (const char*)Scratch0);
	}

	// Get user-defined directory name
	LoadString(gui.hInst, dwIDS_KEY_PLUGIN_PATH, (LPSTR)Scratch1, 700);
	strcpy( user_set_plugin_directory,
		REGISTRY_ReadSTRING( (char*)Scratch1, default_plugin_directory));

	// Are we using the default or user-defined?
	LoadString(gui.hInst, dwIDS_PLUGIN_USE_DEFAULT, (LPSTR)Scratch1, 700);
	*bUseDefault = REGISTRY_ReadDWORD( (char*)Scratch1, TRUE);

	strcpy( szDest,	*bUseDefault^GettingROMPath ?
			default_plugin_directory :		// Use default directory
				user_set_plugin_directory); // Use user-defined directory
}

//----------------------------------------------------------------
// This function is called only when 1964 starts
void ReadConfiguration( void)
{
	char	str[_MAX_PATH];
	int		idx;
	//char	tempstr[_MAX_PATH];
	int		i;

	emuoptions.auto_apply_cheat_code = FALSE;
	defaultoptions.Code_Check = CODE_CHECK_PROTECT_MEMORY;
	defaultoptions.Comments[0] = '\0';
	defaultoptions.Alt_Title[0] = '\0';
	defaultoptions.countrycode = 0;
	defaultoptions.crc1 = 0;
	defaultoptions.crc2 = 0;
	defaultoptions.Emulator = DYNACOMPILER;
	defaultoptions.Game_Name[0] = '\0';
	defaultoptions.Max_FPS = MAXFPS_AUTO_SYNC;
	defaultoptions.RDRAM_Size = RDRAMSIZE_4MB;
	defaultoptions.Save_Type = ANYUSED_SAVETYPE;
	defaultoptions.Use_TLB = USETLB_YES;
	defaultoptions.Eeprom_size = EEPROMSIZE_4KB;
	defaultoptions.Use_Register_Caching = USEREGC_YES;
	defaultoptions.Counter_Factor = COUTERFACTOR_3;
	defaultoptions.FPU_Hack = USEFPUHACK_YES;
	defaultoptions.timing_Control = DELAY_DMA;
	defaultoptions.Link_4KB_Blocks = USE4KBLINKBLOCK_YES;
	defaultoptions.Advanced_Block_Analysis = USEBLOCKANALYSIS_YES;
	defaultoptions.Assume_32bit = ASSUME_32BIT_NO;
	defaultoptions.Use_HLE = USEHLE_NO;
	defaultoptions.RSP_RDP_Timing = USE_RSP_RDP_TIMING_NO;
	defaultoptions.frame_buffer_rw = USECFBRW_NO;
	defaultoptions.numberOfPlayers = 1;
	defaultoptions.videoPluginName[0] = '\0';
	defaultoptions.audioPluginName[0] = '\0';
	defaultoptions.inputPluginName[0] = '\0';
	defaultoptions.rspPluginName[0] = '\0';
	defaultoptions.iconFilename[0] = '\0';

#ifdef _DEBUG
	REGISTRY_ReadDebug();
#endif

	REGISTRY_GetDirectory( directories.plugin_directory_to_use,
						   default_plugin_directory,
						   user_set_plugin_directory,
						   &guioptions.use_default_plugin_directory,
						   IDS_DEFAULT_PLUGIN_PATH,
						   IDS_KEY_PLUGIN_PATH,
						   IDS_KEY_USE_DEFAULT_PLUGIN_PATH,
						   0);

	REGISTRY_GetDirectory( directories.save_directory_to_use,
						   default_save_directory,
						   user_set_save_directory,
						   &guioptions.use_default_save_directory,
						   IDS_DEFAULT_SAVE_PATH,
						   IDS_KEY_SAVE_PATH,
						   IDS_KEY_USE_DEFAULT_SAVE_PATH, 
						   0);

	if( !FileIO_CheckAndCreateFolder(directories.save_directory_to_use) )
	{
		if( strcmp(directories.save_directory_to_use, default_save_directory) != 0 )
		{
			strcpy(directories.save_directory_to_use, default_save_directory);
			if( !FileIO_CheckAndCreateFolder(directories.save_directory_to_use) )
			{
				DisplayError("Can not create save directory: %s, please check it manually", default_save_directory);
			}
		}
	}

	REGISTRY_GetDirectory( directories.state_save_directory_to_use,
						   default_state_save_directory,
						   user_set_state_save_directory,
						   &guioptions.use_default_state_save_directory,
						   IDS_DEFAULT_STATE_SAVE_PATH,
						   IDS_KEY_STATE_SAVE_PATH,
						   IDS_KEY_USE_DEFAULT_STATE_SAVE_PATH, 
						   0);

	/*
	if( !FileIO_CheckAndCreateFolder(directories.state_save_directory_to_use) )
	{
		if( strcmp(directories.state_save_directory_to_use, default_state_save_directory) != 0 )
		{
			strcpy(directories.state_save_directory_to_use, default_state_save_directory);
			if( !FileIO_CheckAndCreateFolder(directories.state_save_directory_to_use) )
			{
				DisplayError("Can not create save directory: %s, please check it manually", default_state_save_directory);
			}
		}
	}
	*/

	GetCmdLineParameter(CMDLINE_ROM_DIR, directories.rom_directory_to_use);
	if( strlen( directories.rom_directory_to_use) == 0)
		REGISTRY_GetDirectory( directories.rom_directory_to_use,
						   directories.last_rom_directory,
						   user_set_rom_directory,
						   &guioptions.use_last_rom_directory,
						   IDS_DEFAULT_ROM_PATH,
						   IDS_KEY_ROM_PATH,
						   IDS_KEY_USE_LAST_USED_ROM_PATH,
						   1);

	//REGISTRY_ReadSTRING("LastROMDirectory",directories.last_rom_directory);
	strcpy(directories.last_rom_directory,REGISTRY_ReadSTRING("LastROMDirectory",user_set_rom_directory));
	//REGISTRY_ReadSTRING("RomDirectory",directories.last_rom_directory);
	strcpy(directories.rom_directory_to_use, guioptions.use_last_rom_directory?
		directories.last_rom_directory:user_set_rom_directory);

	GetStringParameter( gRegSettings.AudioPlugin,
						CMDLINE_AUDIO_PLUGIN,
						IDS_KEY_AUDIO_PLUGIN,
						IDS_DEFAULT_AUDIO_PLUGIN);

	GetStringParameter( gRegSettings.VideoPlugin,
						CMDLINE_VIDEO_PLUGIN,
						IDS_KEY_VIDEO_PLUGIN,
						IDS_DEFAULT_VIDEO_PLUGIN);

	GetStringParameter( gRegSettings.InputPlugin,
						CMDLINE_CONTROLLER_PLUGIN,
						IDS_KEY_INPUT_PLUGIN,
						IDS_DEFAULT_INPUT_PLUGIN);

	GetStringParameter( gRegSettings.RSPPlugin,
						CMDLINE_RSP_PLUGIN,
						IDS_KEY_RSP_PLUGIN,
						IDS_DEFAULT_RSP_PLUGIN);
	if( stricmp(gRegSettings.RSPPlugin, "none") == 0 )	gRegSettings.RSPPlugin[0]=0;

	emuoptions.auto_run_rom = 1;
	emuoptions.auto_apply_cheat_code = REGISTRY_ReadDWORD( "AutoApplyCheat", 0);

	emuoptions.auto_full_screen =
		REGISTRY_ReadDWORD( "AutoFullScreen", 0);

	emuoptions.UsingRspPlugin =
		REGISTRY_ReadDWORD( "UsingRspPlugin", 0);

	//Don't change this. It messes with the boot code for Conker and DK.
	emuoptions.dma_in_segments =
		REGISTRY_ReadDWORD( "DmaInSegments", 0);

	emuoptions.SyncVI =
		REGISTRY_ReadDWORD( "SyncVI", TRUE);

	emuoptions.AutoFrameSkip =
		REGISTRY_ReadDWORD( "AutoFrameSkip", FALSE);

	emuoptions.AutoCF =
		REGISTRY_ReadDWORD( "AutoCF", FALSE);

	guioptions.pause_at_inactive =
		REGISTRY_ReadDWORD( "PauseWhenInactive", TRUE);
	guioptions.pause_at_inactive = FALSE;	// This feature is disabled

	guioptions.pause_at_menu =
		REGISTRY_ReadDWORD( "PauseAtMenu", TRUE);

	guioptions.show_recent_rom_directory_list =
		REGISTRY_ReadDWORD( "RomDirectoryListMenu", TRUE);

	guioptions.show_recent_game_list =
		REGISTRY_ReadDWORD( "GameListMenu", TRUE);

	guioptions.noOfRecentFolders =
		REGISTRY_ReadDWORD( "NoRomDirectoryListMenu", 8);

	guioptions.noOfRecentROMs =
		REGISTRY_ReadDWORD( "NoGameListMenu", 8);

	guioptions.boxart_image_width =
		REGISTRY_ReadDWORD( "BoxartImageWidth", 256);

	guioptions.boxart_image_height =
		REGISTRY_ReadDWORD( "BoxartImageHeight", 180);

	guioptions.boxart_image_x_spacing =
		REGISTRY_ReadDWORD( "BoxartImageXSpacing", 50);

	guioptions.boxart_image_y_spacing =
		REGISTRY_ReadDWORD( "BoxartImageYSpacing", 50);

	guioptions.display_detail_status =
		REGISTRY_ReadDWORD( "DisplayDetailStatus", TRUE);

	guioptions.display_profiler_status =
		REGISTRY_ReadDWORD( "DisplayProfilerStatus", TRUE);

    //A little hack. It would be better to have a single
    //registr value for statusbar statistics, instead of 2 registry values.
    if (guioptions.display_profiler_status && guioptions.display_detail_status)
            guioptions.display_detail_status = 0;

	guioptions.show_critical_msg_window =
		REGISTRY_ReadDWORD( "DisplayCriticalMessageWindow", FALSE);

	strcpy( guioptions.language, REGISTRY_ReadSTRING("Language", "english"));
	guioptions.display_romlist = REGISTRY_ReadDWORD( "DisplayRomList", TRUE);
	guioptions.display_boxart = REGISTRY_ReadDWORD( "Displayboxart", FALSE);

	guioptions.displayDefaultPlugins = TRUE;//REGISTRY_ReadDWORD( "DisplayDefaultPlugins", TRUE);

	rlstatus.romlist_sort_method = REGISTRY_ReadDWORD( "SortRomList", ROMLIST_GAMENAME);

	rlstatus.romlistNameToDisplay = REGISTRY_ReadDWORD( "RomNameToDisplay", ROMLIST_DISPLAY_ALTER_NAME);

	for( i=0, idx=0; i<MAX_RECENT_ROM_DIR; i++) 
	{
		sprintf(str, "RecentRomDirectory%d", i);
		strcpy(recent_rom_directory_lists[idx], REGISTRY_ReadSTRING(str, "Empty Rom Folder Slot"));
		if( strcmp(recent_rom_directory_lists[idx], "Empty Rom Folder Slot") != 0 && PathFileExists(recent_rom_directory_lists[idx]) )
		{
			idx++;
		}
	}
	for( ;idx<MAX_RECENT_ROM_DIR; idx++)
	{
		strcpy(recent_rom_directory_lists[idx], "Empty Rom Folder Slot");
	}

	for( i=0, idx=0; i<MAX_RECENT_GAME_LIST; i++) 
	{
		sprintf( str, "RecentGame%d", i);
		strcpy( recent_game_lists[idx], REGISTRY_ReadSTRING(str, "Empty Game Slot"));
		if( strcmp(recent_game_lists[idx], "Empty Game Slot") != 0 && PathFileExists(recent_game_lists[idx]) )
		{
			idx++;
		}
	}
	for( ;idx<MAX_RECENT_GAME_LIST; idx++)
	{
		strcpy(recent_game_lists[idx], "Empty Game Slot");
	}

	for(i = 0; i < numberOfRomListColumns; i++)	
	{
		int width;

		sprintf( str, "RomListColumn%dWidth", i);
		width = REGISTRY_ReadDWORD( str, romListColumns[i].colWidth);
		if( width >= 0 && width <= 500)
			romListColumns[i].colWidth = width;

		sprintf( str, "RomListColumn%dEnabled", i);
		romListColumns[i].enabled = REGISTRY_ReadDWORD( str, romListColumns[i].enabled);
		if( romListColumns[i].enabled != TRUE)
			romListColumns[i].enabled = FALSE;
	}
}

//----------------------------------------------------------------
// Write values to the system registry.
// I'm working to make this function obsolete.
void WriteConfiguration( void)
{
	HKEY	hKey1 = NULL, 
			hKey2 = NULL;
	DWORD	cbData, dwData;
	char str[260];
	int		i;

	cbData = 4;
	REGISTRY_OpenConnection( &hKey1, &hKey2);

	REGISTRY_WriteDWORD( "UsingRspPlugin", emuoptions.UsingRspPlugin);
	
	REGISTRY_WriteDWORD( "SortRomList", rlstatus.romlist_sort_method);
	REGISTRY_WriteDWORD( "RomNameToDisplay", rlstatus.romlistNameToDisplay);
	REGISTRY_WriteDWORD( "UseDefaultStateSaveDirectory", guioptions.use_default_state_save_directory);

#ifdef _DEBUG
	REGISTRY_WriteDebug();
#endif

	for(i = 0; i < MAX_RECENT_ROM_DIR; i++)	{
		sprintf(str, "RecentRomDirectory%d", i);
		REGISTRY_WriteStringByName( str, recent_rom_directory_lists[i]);
	}

	for(i = 0; i < MAX_RECENT_GAME_LIST; i++) {
		sprintf(str, "RecentGame%d", i);
		REGISTRY_WriteStringByName( str, recent_game_lists[i]);
	}

	for(i = 0; i < numberOfRomListColumns; i++)	{
		sprintf(str, "RomListColumn%dWidth", i);
		cbData = sizeof(dwData);
		dwData = romListColumns[i].colWidth;
		RegSetValueEx(hKey2, str, 0, REG_DWORD, (LPBYTE) & dwData, cbData);
		sprintf(str, "RomListColumn%dEnabled", i);
		cbData = sizeof(dwData);
		dwData = romListColumns[i].enabled;
		RegSetValueEx(hKey2, str, 0, REG_DWORD, (LPBYTE) & dwData, cbData);
	}

	REGISTRY_CloseConnection(&hKey1, &hKey2);
}

//----------------------------------------------------------------
// In response to a WM_EXITSIZEMOVE event, write window coordinate
// data to system registry
int
REGISTRY_WriteSizeMove()
{
	WINDOWPLACEMENT placement;

	placement.length = sizeof( WINDOWPLACEMENT);
	GetWindowPlacement( gui.hwnd1964main, &placement);

	REGISTRY_WriteDWORD("1964WindowTOP", placement.rcNormalPosition.top);
	REGISTRY_WriteDWORD("1964WindowLeft", placement.rcNormalPosition.left);
	REGISTRY_WriteDWORD("ClientWindowHeight", 
		placement.rcNormalPosition.bottom - placement.rcNormalPosition.top);
	REGISTRY_WriteDWORD("ClientWindowWidth", 
		placement.rcNormalPosition.right - placement.rcNormalPosition.left);

	return TRUE;
}

//----------------------------------------------------------------
int REGISTRY_WriteAutoFrameSkip()
{
	DWORD			dwData;
	HKEY			hKey1 = NULL,
	hKey2 = NULL;

	REGISTRY_OpenConnection(&hKey1, &hKey2);

	dwData = emuoptions.AutoFrameSkip;
	RegSetValueEx( hKey2, "AutoFrameSkip",  0, REG_DWORD, (LPBYTE) & dwData, 4);

	REGISTRY_CloseConnection(&hKey1, &hKey2);
	return TRUE;
}
//----------------------------------------------------------------
int REGISTRY_WriteVISync()
{
	DWORD			dwData;
	HKEY			hKey1 = NULL,
					hKey2 = NULL;

	REGISTRY_OpenConnection(&hKey1, &hKey2);

	dwData = emuoptions.SyncVI;
	RegSetValueEx( hKey2, "SyncVI",  0, REG_DWORD, (LPBYTE) & dwData, 4);

	REGISTRY_CloseConnection(&hKey1, &hKey2);
	return TRUE;
}

//----------------------------------------------------------------
int REGISTRY_WriteAutoCF()
{
	DWORD			dwData;
	HKEY			hKey1 = NULL,
	hKey2 = NULL;

	REGISTRY_OpenConnection(&hKey1, &hKey2);

	dwData = emuoptions.AutoCF;
	RegSetValueEx( hKey2, "AutoCF",  0, REG_DWORD, (LPBYTE) & dwData, 4);

	REGISTRY_CloseConnection(&hKey1, &hKey2);
	return TRUE;
}
