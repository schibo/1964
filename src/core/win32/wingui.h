/*$T wingui.h GC 1.136 02/28/02 07:53:40 */


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
#ifndef _WINGUI_H__1964_
#define _WINGUI_H__1964_
#include <stdlib.h>
#include "resource.h"

/* Functions in the wingui.c */
int APIENTRY		aWinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow);
HWND				InitWin98UI(HANDLE hInstance, int nCmdShow);
LRESULT APIENTRY	MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY	About(HWND hDlg, unsigned message, WORD wParam, LONG lParam);
void __cdecl		DisplayError(char *Message, ...);
void __cdecl		DisplayCriticalMessage(char *Message, ...);

BOOL				WinLoadRom(void);

BOOL				
__cdecl 
WinLoadRomStep2(char *szFileName);

void				Resume(void);
void				Kill(void);

void
__cdecl
Play(BOOL WithFullScreen);

void				Stop(void);
void				KailleraPlay(void);
void				OpenROM(void);
void				CloseROM(void);
void				ChangeDirectory(void);
void				SaveState(void);
void				LoadState(void);
void				SaveStateByNumber(WPARAM wparam);
void				LoadStateByNumber(WPARAM wparam);
void				SaveStateByDialog(int format);
void				LoadStateByDialog(int format);
void				EnableStateMenu(void);
void				DisableStateMenu(void);
void				PrepareBeforePlay(int IsFullScreen);
void				KillCPUThread(void);
void				SetCounterFactor(int);
void				SetCodeCheckMethod(int);
void				InitPluginData(void);
void				Set_1964_Directory(void);
void				__cdecl CountryCodeToCountryName_and_TVSystem(int countrycode, char *countryname, int *tvsystem);
void				CaptureScreenToFile(void);
void				Set_Ready_Message(void);
void				DisableDebugMenu(void);
void				SetupDebuger(void);
void				SaveCmdLineParameter(char *cmdline);
BOOL				StartGameByCommandLine();
void				SetupAdvancedMenus (void);
void				ManageMenus(void);

extern void			REGISTRY_WriteStringByIndex( DWORD dwIndex, char *szValue);
extern void			REGISTRY_WriteStringByName( char* szName, char *szValue);
extern DWORD		REGISTRY_ReadDWORD( char *szFieldName, DWORD dwDefaultValue);
extern int			REGISTRY_WriteVISync();
extern int			REGISTRY_WriteAutoFrameSkip();
extern int			REGISTRY_WriteSizeMove();
extern void			REGISTRY_WriteDWORD( char *szFieldName, DWORD dwValue);
extern BOOL			REGISTRY_OpenConnection( PHKEY hKey1,  PHKEY hKey2);
extern BOOL			REGISTRY_CloseConnection( PHKEY hMainKey, PHKEY hSubKey);
extern void			REGISTRY_ReadDebug();
extern int			REGISTRY_WriteDebug();


void				StateSetNumber(int number);
void				Exit1964(void);
void				GetPluginDir(char *Directory);
extern int			LoadGNUDistConditions(char *ConditionsBuf);
extern void         CreateOptionsDialog(int nStartPage);
LRESULT APIENTRY	ConditionsDialog(HWND hDlg, unsigned message, WORD wParam, LONG lParam);
LRESULT APIENTRY	CheatAndHackDialog(HWND hDlg, unsigned message, WORD wParam, LONG lParam);
LRESULT APIENTRY	CriticalMessageDialog(HWND hDlg, unsigned message, WORD wParam, LONG lParam);

#define MAXFILENAME 256					/* maximum length of file pathname */

/* the legal stuff */
extern unsigned char	MainDisclaimer[];
extern unsigned char	Scratch0[];
extern unsigned char	Scratch1[];
extern  unsigned char	Scratch2[];
extern unsigned char	DistConditions[];	/* GNU Redistribution Conditions */


struct EMU1964GUI
{
	char		*szBaseWindowTitle;
	HINSTANCE	hInst;
	HACCEL		hAccTable;				/* handle to accelerator table */
	HWND		hwnd1964main;			/* handle to main window */
	HWND		hwndRomList;			/* Handle to the rom list child window */
	HWND		hStatusBar;				/* Window Handle of the status bar */
	HWND		hToolBar;				/* Window Handle of the toolbar */
	HWND		hReBar;					/* Window Handle of the rebar */
	HWND		hClientWindow;			/* Window handle of the client child window */
	HWND		hCriticalMsgWnd;		/* handle to critical message window */
	HMENU		hMenu1964main;
	HMENU		hMenuRomListPopup;

	struct
	{
		/* Status Bar text fields */
		char	field_1[256];
		char	field_2[80];
		char	field_3[80];
		char	field_4[80];
		char	field_5[80];
	} staturbar_field;
	char	szWindowTitle[80];
};

extern struct EMU1964GUI	gui;

struct GUIOPTIONS
{
	BOOL	pause_at_inactive;
	BOOL	pause_at_menu;
	BOOL	ok_to_pause_at_menu;
	BOOL	use_default_save_directory;
	BOOL	use_default_state_save_directory;
	BOOL	use_default_plugin_directory;
	BOOL	use_last_rom_directory;
	BOOL	show_recent_rom_directory_list;
	BOOL	show_recent_game_list;
	BOOL	display_detail_status;
	BOOL	display_profiler_status;
	BOOL	show_critical_msg_window;
	BOOL	display_romlist;
	BOOL	display_boxart;
	int		boxart_image_width;
	int		boxart_image_height;
	int		boxart_image_x_spacing;
	int		boxart_image_y_spacing;
	BOOL	displayDefaultPlugins;		// To display default plugins or to display ROM specified plugins in the plugin dialog box
	int		noOfRecentFolders;
	int		noOfRecentROMs;
	char	language[80];
	int		totalLanguages;
	int		currentLanguageID;
};

extern struct GUIOPTIONS	guioptions;

struct GUISTATUS
{
	int		clientwidth;				/* Client window width */
	int		clientheight;				/* Client window height */
	RECT	window_position;			/* 1964 main window location */
	BOOL	WindowIsMaximized;
	BOOL	window_is_maximized;
	BOOL	window_is_minimized;
	BOOL	block_menu;					/* Block all menu command while 1964 is busy */
	int		IsFullScreen;
};
extern struct GUISTATUS guistatus;

extern void DockStatusBar(void);
extern void InitStatusBarParts(void);
extern void	__cdecl		SetStatusBarText(int, char * );

#define MAX_RECENT_ROM_DIR		16
#define MAX_RECENT_GAME_LIST	16
extern char					recent_rom_directory_lists[MAX_RECENT_ROM_DIR][260];
extern char					recent_game_lists[MAX_RECENT_GAME_LIST][260];

struct DIRECTORIES
{
	char	main_directory[_MAX_PATH];
	char	plugin_directory_to_use[_MAX_PATH];
	char	state_save_directory_to_use[_MAX_PATH];
	char	save_directory_to_use[_MAX_PATH];
	char	rom_directory_to_use[_MAX_PATH];
	char	last_rom_directory[_MAX_PATH];
	char	boxart_directory_to_use[_MAX_PATH];
};

extern struct DIRECTORIES	directories;

extern char					game_country_name[10];
extern int					game_country_tvsystem;

enum { LOAD_ALL_PLUGIN, LOAD_VIDEO_PLUGIN, LOAD_AUDIO_PLUGIN, LOAD_INPUT_PLUGIN, LOAD_RSP_PLUGIN };

void FreePlugins(void);
BOOL LoadPlugins(int type);

enum { SAVE_STATE_1964_FORMAT, SAVE_STATE_PJ64_FORMAT, SAVE_STATE_1964_085_FORMAT};

typedef enum {
	CMDLINE_AUDIO_PLUGIN,
	CMDLINE_VIDEO_PLUGIN,
	CMDLINE_CONTROLLER_PLUGIN,
	CMDLINE_ROM_DIR,
	CMDLINE_GAME_FILENAME,
	CMDLINE_FULL_SCREEN_FLAG,
	CMDLINE_MAX_NUMBER,
	CMDLINE_RSP_PLUGIN,
}CmdLineParameterType;

//Change the order of this if you want to rearrange the property sheets
typedef enum {
    PSH_WINDOW=0,
    PSH_CHEAT_CODES,
    PSH_PLUGINS,
    PSH_FOLDERS,
    PSH_ROM_BROWSER,
    PSH_ROM_INFORMATION,
    PSH_ROM_OPTIONS,
    PSH_ABOUT

}EnumPropPagesType;


typedef struct {
	char	filename[MAX_PATH];
	char	info[MAX_PATH];
	int		type;
	int		version;
} PluginDLLInfo;

#define MAX_PLUGIN_DLL	100

extern PluginDLLInfo *AllPluginDLLInfos;
extern int	PluginDLLCounts[4];
extern int TotalPluginDLLCount;


void GetCmdLineParameter(CmdLineParameterType arg, char *buf);


#define MAX_LANG_ENTRIES	1000
extern char *languageFileNames[200];

#define NEW_LANGUAGE_MENU_START	20000
void CheckLanguages();


typedef struct {
	DWORD	resID;
	int		langID;
	char	*langStr;
} DlgLangMapEntry;

typedef struct {
	DWORD	resID;
	int		langID;
	char	*langStr;
	char	*oriStr;	// original string in Engish
} StringMapEntry;

#define MAX_BITMAPS	1024
typedef struct {
	int		idxInImageList;
	int		bitmapIndex;
	uint32	crc1;
	uint32	crc2;
} BitmapIdxMap;

typedef struct {
	char	fullPath[_MAX_PATH];
	char	filename[_MAX_PATH];
	char	shortname[_MAX_PATH];
} BitmapFileInfo;

HBITMAP LoadAnImage(char* FileName);
HBITMAP ScaleBitmap(HBITMAP hBmp, WORD wNewWidth, WORD wNewHeight, BOOL bKeepAspectRatio);


#define MAX_CONTROL_PER_DIALOG	30
#define MAX_NUM_OF_DIALOGS	12

extern const int NumOfDialogs;
extern DlgLangMapEntry DlgLangMapEntries[][MAX_CONTROL_PER_DIALOG];
extern const int	DlgLangMapEntryCounts[];

void FreeDlgLangMapEntries(void);
void TranslateDialag(HWND hDlg, char *dlgName);
extern char * TranslateStringByID(DWORD id);
extern char * TranslateStringByString(char *oriStr);

#define VirtualSafeFree(ptr) {if(ptr) {VirtualFree((ptr),0,MEM_RELEASE);(ptr)=NULL;}}
#endif
