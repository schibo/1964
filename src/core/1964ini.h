/*$T 1964ini.h GC 1.136 02/28/02 07:55:22 */


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
#ifndef _1964INI_H__1964_
#define _1964INI_H__1964_

#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

enum GAMESAVETYPE
{
	DEFAULT_SAVETYPE,
	EEPROM_SAVETYPE,
	MEMPAK_SAVETYPE,
	SRAM_SAVETYPE,
	FLASHRAM_SAVETYPE,
	FIRSTUSE_SAVETYPE,
	ANYUSED_SAVETYPE
};

enum EMULATORTYPE { DEFAULT_EMULATORTYPE, DYNACOMPILER, INTERPRETER };

enum EEPROMSIZE { EEPROMSIZE_DEFAULT, EEPROMSIZE_NONE, EEPROMSIZE_4KB, EEPROMSIZE_16KB };

enum CODECHECKTYPE
{
	CODE_CHECK_DEFAULT,
	CODE_CHECK_NONE,
	CODE_CHECK_DMA_ONLY,
	CODE_CHECK_MEMORY_DWORD,
	CODE_CHECK_MEMORY_QWORD,
	CODE_CHECK_MEMORY_QWORD_AND_DMA,
	CODE_CHECK_MEMORY_BLOCK,
	CODE_CHECK_MEMORY_BLOCK_AND_DMA,
	CODE_CHECK_PROTECT_MEMORY
};

enum USETLBTYPE { USETLB_DEFAULT, USETLB_YES, USETLB_NO };

enum MAXFPSTYPE {	MAXFPS_DEFAULT, MAXFPS_NONE, MAXFPS_NTSC_60, MAXFPS_PAL_50, MAXFPS_AUTO_SYNC, MAXFPS_10, 
					MAXFPS_15, MAXFPS_20, MAXFPS_25, MAXFPS_30, MAXFPS_40, MAXFPS_70, MAXFPS_80, MAXFPS_90,
					MAXFPS_100, MAXFPS_120, MAXFPS_150, MAXFPS_180, MAXFPS_210};

enum RDRAMSIZETYPE { RDRAMSIZE_DEFAULT, RDRAMSIZE_4MB, RDRAMSIZE_8MB };

enum USEREGISTERCACHING { USEREGC_DEFAULT, USEREGC_YES, USEREGC_NO };

enum COUNTERFACTOR
{
	COUTERFACTOR_DEFAULT			= 0,
	COUTERFACTOR_1,
	COUTERFACTOR_2,
	COUTERFACTOR_3,
	COUTERFACTOR_4,
	COUTERFACTOR_5,
	COUTERFACTOR_6,
	COUTERFACTOR_7,
	COUTERFACTOR_8
};

enum USEFPUHACK { USEFPUHACK_DEFAULT, USEFPUHACK_YES, USEFPUHACK_NO };

enum USEDMASEGMENTATION { USEDMASEG_DEFAULT, DELAY_DMA, NO_DELAY, DELAY_DMA_SI, DELAY_DMA_AI, DELAY_DMA_SI_AI };

enum USE4KBLINKBLOCK { USE4KBLINKBLOCK_DEFAULT, USE4KBLINKBLOCK_YES, USE4KBLINKBLOCK_NO };

enum USEADVANCEDBLOCKANALYSIS { USEBLOCKANALYSIS_DEFAULT, USEBLOCKANALYSIS_YES, BLOCKANALYSIS_NO };

enum ASSUME32BIT { ASSUME_32BIT_DEFAULT, ASSUME_32BIT_YES, ASSUME_32BIT_NO };

enum USEHLE { USEHLE_DEFAULT, USEHLE_YES, USEHLE_NO };

enum USERSPRDPTIMING { USE_RSP_RDP_TIMING_DEFAULT, USE_RSP_RDP_TIMING_YES, USE_RSP_RDP_TIMING_NO };

enum USECFBRW { USECFBRW_DEFAULT, USECFBRW_YES, USECFBRW_NO };

typedef struct INI_ENTRY_STRUCT
{
	char				Game_Name[40];
	char				Comments[80];
	char				Alt_Title[51];
	uint32				crc1;
	uint32				crc2;
	uint8				countrycode;
	int					RDRAM_Size;
	enum EMULATORTYPE	Emulator;
	enum GAMESAVETYPE	Save_Type;
	enum CODECHECKTYPE	Code_Check;
	int					Max_FPS;
	int					Use_TLB;
	int					Eeprom_size;
	int					Counter_Factor;
	int					Use_Register_Caching;
	int					FPU_Hack;
	int					timing_Control;
	int					Link_4KB_Blocks;
	int					Advanced_Block_Analysis;
	int					Assume_32bit;
	int					Use_HLE;

	// By Rice, 05/18/2003
	// New options, working together with video/audio plugins
	// Working with RiceDaedalus plugins only
	int					RSP_RDP_Timing;		// Plugin will provide more accurate RSP/RDP timing info
	int					frame_buffer_rw;	// to tell video plugin about CPU direct frame buffer read/write

	int					numberOfPlayers;

	// ROM specified plugin names
	char				videoPluginName[40];
	char				audioPluginName[40];
	char				inputPluginName[40];
	char				rspPluginName[40];

	char				iconFilename[256];
} INI_ENTRY;

/* Support update to 3000 entries, should be enough for all the N64 Games */
#define MAX_INI_ENTRIES 3000

/* Globals definition */
extern char				*rdram_size_names[];
extern char				*save_type_names[];
extern char				*emulator_type_names[];
extern char				*codecheck_type_names[];
extern char				*maxfps_type_names[];
extern char				*usetlb_type_names[];
extern char				*eepromsize_type_names[];
extern char				*counter_factor_names[];
extern char				*register_caching_names[];
extern char				*use_fpu_hack_names[];
extern char				*timing_control_names[];
extern char				*use_4kb_link_block_names[];
extern char				*use_block_analysis_names[];
extern char				*use_HLE_names[];
extern char				*assume_32bit_names[];
extern char				*use_RSP_RDP_Timing_names[];
extern char				*use_Frame_Buffer_R_W_names[];

extern double				vips_speed_limits[];

extern INI_ENTRY	currentromoptions;
extern INI_ENTRY	*ini_entries[MAX_INI_ENTRIES];	/* Only allocate memory for entry pointers */

/* entries will be dynamically allocated */
extern int			ini_entry_count;

void				InitIniEntries(void);
INI_ENTRY			*GetNewIniEntry(void);
int					__cdecl AddIniEntry(const INI_ENTRY *);
void				DeleteIniEntry(const int index);
void				DeleteAllIniEntries(void);
int					FindIniEntry(const char *gamename, const uint32 crc1, const uint32 crc2, const uint8 country);

int
__cdecl
FindIniEntry2(const INI_ENTRY *);

int					ReadIniEntry(FILE *, INI_ENTRY *);
int					WriteIniEntry(FILE *, const INI_ENTRY *);
int					ReadAllIniEntries(FILE *);
int					WriteAllIniEntries(FILE *);
void				CopyIniEntry(INI_ENTRY *, const INI_ENTRY *);
void				DeleteIniEntryByEntry(INI_ENTRY *pentry);
void				SetDefaultOptions(void);
void				GenerateCurrentRomOptions(void);
int					Write1964DefaultOptionsEntry(FILE *pstream);
int					Read1964DefaultOptionsEntry(FILE *pstream);
void				chopm(char *str);
uint32				ConvertHexStringToInt(const char *str, int nchars);

/* 1964 default options */
extern INI_ENTRY	defaultoptions;

extern char			default_rom_directory[_MAX_PATH];
extern char			default_save_directory[_MAX_PATH];
extern char			default_state_save_directory[_MAX_PATH];
extern char			default_plugin_directory[_MAX_PATH];
extern char			user_set_rom_directory[_MAX_PATH];
extern char			user_set_save_directory[_MAX_PATH];
extern char			state_save_directory[_MAX_PATH];
extern char			user_set_plugin_directory[_MAX_PATH];
extern char			user_set_state_save_directory[_MAX_PATH];
#endif
