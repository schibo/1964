/*$T romlist.h GC 1.136 02/28/02 07:51:10 */


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
#ifndef _ROMLIST_H__1964_
#define _ROMLIST_H__1964_

#include <windows.h>
#include <commctrl.h>
#include "globals.h"
#include "1964ini.h"

typedef struct ROMLIST_ENTRY_STRUCT
{
	INI_ENTRY	*pinientry;
	char		romfilename[_MAX_FNAME];
	long		size;
}ROMLIST_ENTRY ;

#define NUM_SORT_METHODS 9 //this should be the same as the number of columns
enum
{
	ROMLIST_GAMENAME,
	ROMLIST_COUNTRY,
	ROMLIST_SIZE,
	ROMLIST_COMMENT,
    ROMLIST_SAVETYPE,
    ROMLIST_CICCHIP,
    ROMLIST_CRC1,
    ROMLIST_CRC2,
	ROMLIST_PLAYERS,
	ROMLIST_GAMENAME_INVERT,
	ROMLIST_COUNTRY_INVERT,
	ROMLIST_SIZE_INVERT,
	ROMLIST_COMMENT_INVERT,
    ROMLIST_SAVETYPE_INVERT,
    ROMLIST_CICCHIP_INVERT,
    ROMLIST_CRC1_INVERT,
    ROMLIST_CRC2_INVERT,
	ROMLIST_PLAYERS_INVERT,
};

enum
{
	ROMLIST_DISPLAY_INTERNAL_NAME,
	ROMLIST_DISPLAY_ALTER_NAME,
	ROMLIST_DISPLAY_FILENAME,
};

typedef enum {
	ROMLIST_COL_GAMENAME=0,
	ROMLIST_COL_COUNTRY,
	ROMLIST_COL_SIZE,
	ROMLIST_COL_STATUS,
	ROMLIST_COL_GAMESAVE,
	ROMLIST_COL_CICCHIP,
	ROMLIST_COL_CRC1,
	ROMLIST_COL_CRC2,
	ROMLIST_COL_PLAYERS
} ColumnID;

typedef struct{
	ColumnID	colID;
	char		*text;
	int			colPos;
	BOOL		enabled;
	int			colWidth;
	HTREEITEM	treeViewID;
} ColumnType;

#define MAX_ROMLIST			2000

extern ROMLIST_ENTRY *		romlist[MAX_ROMLIST];
extern ColumnType			romListColumns[];
extern const int			numberOfRomListColumns;

typedef struct {
	int			iconWidth;
	int			iconHeight;
	int			iconXSpacing;
	int			iconYSpacing;
	HIMAGELIST	hLargeBitmaps;
	HIMAGELIST	hSmallIcons;
	int			romlist_count;
	int			romlist_sort_method;
	int			romlistNameToDisplay;
	int			romListHeaderClickedColumn;
	int			selected_rom_index;
} ROMLIST_STATUS;

extern ROMLIST_STATUS rlstatus;


BOOL __cdecl RomListReadDirectory(const char *path, BOOL usecached);
void __cdecl ClearRomList(void);
void __cdecl InitRomList(void);
int	__cdecl RomListAddEntry(INI_ENTRY *newentry, char *romfilename, long int filesize);
void __cdecl RomListOpenRom(int index, BOOL RunThisRom);
void __cdecl RomListSelectRom(int index);
void __cdecl RomListRomOptions(int index);
void __cdecl RomListSaveCurrentPos(void);
void __cdecl RomListUseSavedPos(void);

int
__cdecl
RomListGetSelectedIndex(void);

HWND 
__cdecl 
NewRomList_CreateListViewControl(HWND hwndParent);

void __cdecl NewRomList_ListViewHideHeader(HWND hwnd);
void __cdecl NewRomList_ListViewShowHeader(HWND hwnd);
void __cdecl NewRomList_ListViewFreshRomList(void);
void __cdecl NewRomList_ListViewChangeWindowRect(void);
void __cdecl NewRomList_Sort(void);
void __cdecl RomListRememberColumnWidth(void);

ROMLIST_ENTRY *
__cdecl
RomListGet_Selected_Entry(void);

void
__cdecl
RomListSelectLoadedRomEntry(void);

LRESULT 
APIENTRY 
RomListDialog(HWND hDlg, unsigned message, WORD wParam, LONG lParam);

LRESULT 
APIENTRY 
ColumnSelectDialog(HWND hDlg, unsigned message, WORD wParam, LONG lParam);

void
__cdecl
ReadRomHeaderInMemory(INI_ENTRY *ini_entry);

ROMLIST_ENTRY *
__cdecl
RomListSelectedEntry(void);

void
__cdecl
ConvertInvalidInternalName(char *oldname, char *newname);

BOOL
__cdecl
InternalNameIsValid(char *name);

long
__cdecl
OnNotifyRomList(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

extern long
__cdecl
OnNotifyRomListHeader(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

extern void
__cdecl
CheckButton(int nID, BOOL bCheck);

extern void __cdecl EnableButton(int nID, BOOL bEnable);
extern void __cdecl EnableRadioButtons(BOOL bEnable);
extern void __cdecl SetupToolBar();
extern BYTE __cdecl ChangeButtonState(int nID);

#endif /* ROMLIST_H */
