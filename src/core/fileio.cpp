/*$T fileio.c GC 1.136 03/09/02 17:36:25 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    File management
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
#include "stdafx.h"

BOOL	Is_Reading_Rom_File = FALSE;;
BOOL	To_Stop_Reading_Rom_File = FALSE;

#define CURRENT_SAVE_STATE_VERSION		(0x19640099)
#define SAVE_STATE_VERSION_085		(0x19640064)

uint32 SaveStateVersionList[] = {
    0x19640064,
    0x19640099,
	CURRENT_SAVE_STATE_VERSION
};


BYTE	gzipped_defaultm0[] =
{
	0x1F,
	0x8B,
	0x08,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x0B,
	0xED,
	0xCF,
	0xB1,
	0x09,
	0x80,
	0x40,
	0x0C,
	0x05,
	0xD0,
	0x9C,
	0xF6,
	0x4E,
	0xE5,
	0x42,
	0x8E,
	0xE0,
	0x58,
	0x0E,
	0x65,
	0x6B,
	0x75,
	0xA0,
	0x16,
	0x87,
	0x8D,
	0x60,
	0x2A,
	0xAB,
	0x7B,
	0x2F,
	0x4D,
	0xE0,
	0xF3,
	0x03,
	0x59,
	0xE3,
	0xDB,
	0x79,
	0x9B,
	0xE6,
	0x7D,
	0x7B,
	0x05,
	0xA5,
	0x44,
	0xAD,
	0xC3,
	0x91,
	0xD4,
	0xD3,
	0xFE,
	0xDF,
	0xF7,
	0xB3,
	0x7E,
	0x2C,
	0xCF,
	0x36,
	0x76,
	0x39,
	0xBD,
	0xFF,
	0x0F,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0xD0,
	0x5C,
	0xAE,
	0x72,
	0x7F,
	0x57,
	0x00,
	0x80,
	0x00,
	0x00
};

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void SwapRomName(uint8 *name)
{
	int		i;
	uint8	c;

	for(i = 0; i < 20; i += 4)
	{
		c = name[i];
		name[i] = name[i + 3];
		name[i + 3] = c;

		c = name[i + 2];
		name[i + 2] = name[i + 1];
		name[i + 1] = c;
	}

	for(i = 19; i >= 0; i--)
	{
		if(name[i] == ' ') 
			name[i] = 0;
		else
			break;
	}

	name[i + 1] = '\0';
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void SwapRomHeader(uint8 *romheader)
{
	/*~~~~~~*/
	int		i;
	uint8	c;
	/*~~~~~~*/

	for(i = 0; i < 0x40; i += 4)
	{
		c = romheader[i];
		romheader[i] = romheader[i + 3];
		romheader[i + 3] = c;

		c = romheader[i + 2];
		romheader[i + 2] = romheader[i + 1];
		romheader[i + 1] = c;
	}
}

/*
 =======================================================================================================================
    Read ROM Header information into the int_entry, return ROM size
 =======================================================================================================================
 */
long __cdecl ReadRomHeader(	char		*rompath, INI_ENTRY	*ini_entry)
{
	/*~~~~~~~~~~~~~~~~~~~~~~*/
	uint8		buffer[0x100];
	long int	filesize;
	FILE		*fp;
	/*~~~~~~~~~~~~~~~~~~~~~~*/

	fp = fopen(rompath, "rb");
	if(fp != NULL)
	{
		/* Get file size */
		fseek(fp, 0, SEEK_END);
		filesize = ftell(fp);

		/* pad with zeros to fill the displacement */
		if(filesize & 0xFFFF) filesize = filesize + (0x10000 - (filesize & 0xFFFF));
		fseek(fp, 0, SEEK_SET); /* set pointer to beginning of file */

		fread(buffer, sizeof(uint8), 0x40, fp);
		if(ByteSwap(0x40, buffer) == TRUE)
		{
			strncpy((char*)ini_entry->Game_Name, (const char*)(buffer + 0x20), 0x14);
			SwapRomName((unsigned char*)ini_entry->Game_Name);

			/* SwapRomHeader(buffer); */
			ini_entry->crc1 = *((uint32 *) (buffer + 0x10));
			ini_entry->crc2 = *((uint32 *) (buffer + 0x14));
			ini_entry->countrycode = buffer[0x3D];

			/* ini_entry->countrycode = buffer[0x3E]; */
			fclose(fp);
			return filesize;
		}
		else
		{
			/* This file is not a ROM file, skipped */
			fclose(fp);
			return 0;
		}
	}
	else
	{
		/* Cannot open this file, skipped it */
		return 0;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
long
__cdecl
ReadZippedRomHeader(char *rompath, INI_ENTRY *ini_entry)
{
	long int	filesize;
	unzFile		fp;
	char		szFileName[256];
	char		ext[_MAX_EXT];
	uint8		buffer[0x100];

	/* DisplayError("Read Zipped Rom Header: %s",rompath); */
	fp = unzOpen(rompath);
	if(fp == NULL)
	{
		return FALSE;	/* Cannot open this ZIP file */
	}

	if(unzGoToFirstFile(fp) == UNZ_OK)
	{
		do
		{
			/*~~~~~~~~~~~~~~~~~~~~~~*/
			unz_file_info	file_info;
			/*~~~~~~~~~~~~~~~~~~~~~~*/

			if(unzGetCurrentFileInfo(fp, &file_info, szFileName, 256, NULL, 0, NULL, 0) == UNZ_OK)
			{
				filesize = file_info.uncompressed_size;
				if(filesize & 0xFFFF) filesize = filesize + (0x10000 - (filesize & 0xFFFF));

				strcpy(ext, szFileName + strlen(szFileName) - 4);
				if
				(
					stricmp(ext, ".bin") == 0
				||	stricmp(ext, ".v64") == 0
				||	stricmp(ext, ".rom") == 0
				||	stricmp(ext, ".usa") == 0
				||	stricmp(ext, ".j64") == 0
				||	stricmp(ext, ".pal") == 0
				||	stricmp(ext, ".z64") == 0
				||	stricmp(ext, ".n64") == 0
				)
				{
					if(unzOpenCurrentFile(fp) == UNZ_OK)
					{
						if(unzReadCurrentFile(fp, buffer, 0x40) == 0x40)
						{
							if(ByteSwap(0x40, buffer))
							{
								strncpy((char*)ini_entry->Game_Name, (const char*)(buffer + 0x20), 0x14);
								SwapRomName((unsigned char*)ini_entry->Game_Name);

								/* SwapRomHeader(buffer); */
								ini_entry->crc1 = *((uint32 *) (buffer + 0x10));
								ini_entry->crc2 = *((uint32 *) (buffer + 0x14));
								ini_entry->countrycode = buffer[0x3D];

								/* ini_entry->countrycode = buffer[0x3E]; */
								unzCloseCurrentFile(fp);
								unzClose(fp);
								return filesize;
							}
						}

						unzCloseCurrentFile(fp);
					}
				}
			}
		} while(unzGoToNextFile(fp) == UNZ_OK);
	}

	unzClose(fp);
	return 0;			/* Read ZIP file fails for some reason */
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL WindowMsgLoop(void);
BOOL ReadRomData(char *rompath)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~*/
	FILE			*fp;
	unsigned long	gROMLength; /* size in bytes of the ROM */
	/*~~~~~~~~~~~~~~~~~~~~~~~*/

	if(stricmp(&rompath[strlen(rompath) - 4], ".zip") == 0)
	{
		return ReadZippedRomData(rompath);
	}

	fp = fopen(rompath, "rb");
	if(fp == NULL)
	{
		MessageBox
		(
			gui.hwnd1964main,
			TranslateStringByString("Error opening this file. Please refresh the ROM list."),
			TranslateStringByString("Error"),
			MB_ICONINFORMATION
		);
		return FALSE;
	}

	rewind(fp);
	fseek(fp, 0, SEEK_END);
	gROMLength = ftell(fp);

	/* pad with zeros to fill the displacement */
	if(((gROMLength & 0xFFFF)) == 0)
		gAllocationLength = gROMLength;
	else
		gAllocationLength = gROMLength + (0x10000 - (gROMLength & 0xFFFF));

	fseek(fp, 0, SEEK_SET);		/* set pointer to beginning of file */
	if(fp != NULL)
	{
		uint64	i;

		InitVirtualRomMemory(gAllocationLength);
		InitMemoryLookupTables();
		InitTLB();

		Is_Reading_Rom_File = TRUE;;
		To_Stop_Reading_Rom_File = FALSE;

		for(i = 0; i < gROMLength && To_Stop_Reading_Rom_File == FALSE; i += 65536)
		{
			WindowMsgLoop();


			if(To_Stop_Reading_Rom_File == TRUE)
			{
				CloseROM();
				To_Stop_Reading_Rom_File = FALSE;
				Is_Reading_Rom_File = FALSE;

				fclose(fp);
				return FALSE;
			}

			fread(gMS_ROM_Image + i, sizeof(uint8), 65536, fp);
			{
				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
				char	drive[_MAX_DRIVE], dir[_MAX_DIR];
				char	fname[_MAX_FNAME], ext[_MAX_EXT];
				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

				_splitpath(rompath, drive, dir, fname, ext);
				strcat(fname, ext);
				sprintf(generalmessage, "%s [%s] %d%%", TranslateStringByString("Loading"), fname, i * 100 / gROMLength);
			}

			SetStatusBarText(0, generalmessage);
		}

		Set_Ready_Message();

		Is_Reading_Rom_File = FALSE;

		ByteSwap(gAllocationLength, gMS_ROM_Image);
		memcpy((uint8 *) &rominfo.validation, gMS_ROM_Image, 0x40);
		SwapRomHeader((uint8 *) &rominfo.validation);

		/* Copy boot code to SP_DMEM */
		memcpy((uint8 *) &SP_DMEM, gMS_ROM_Image, 0x1000);
		memcpy(rominfo.name, gMS_ROM_Image + 0x20, 20);
		SwapRomName(rominfo.name);
	}
	else
	{
		MessageBox(0, TranslateStringByString("File could not be opened."), TranslateStringByString("Error"), 0);
		return FALSE;
	}

	fclose(fp);

	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL ReadZippedRomData(char *rompath)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~*/
	unzFile			fp;
	unsigned long	gROMLength; /* size in bytes of the ROM */
	/*~~~~~~~~~~~~~~~~~~~~~~~*/

	if(fp = unzOpen(rompath))
	{
		/*~~~~~~~~~~~~~~~~~~~~*/
		char	szFileName[256];
		/*~~~~~~~~~~~~~~~~~~~~*/

		if(unzGoToFirstFile(fp) == UNZ_OK)
		{
			do
			{
				/*~~~~~~~~~~~~~~~~~~~~~~*/
				unz_file_info	file_info;
				/*~~~~~~~~~~~~~~~~~~~~~~*/

				if(unzGetCurrentFileInfo(fp, &file_info, szFileName, 256, NULL, 0, NULL, 0) == UNZ_OK)
				{
					if
					(
						stricmp(&szFileName[strlen(szFileName) - 4], ".bin") == 0
					||	stricmp(&szFileName[strlen(szFileName) - 4], ".v64") == 0
					||	stricmp(&szFileName[strlen(szFileName) - 4], ".rom") == 0
					||	stricmp(&szFileName[strlen(szFileName) - 4], ".usa") == 0
					||	stricmp(&szFileName[strlen(szFileName) - 4], ".z64") == 0
					||	stricmp(&szFileName[strlen(szFileName) - 4], ".j64") == 0
					||	stricmp(&szFileName[strlen(szFileName) - 4], ".pal") == 0
					||	stricmp(&szFileName[strlen(szFileName) - 4], ".n64") == 0
					)
					{
						gROMLength = file_info.uncompressed_size;	/* get size of ROM */

						/* pad with zeros to fill the displacement */
						if(((gROMLength & 0xFFFF)) == 0)
							gAllocationLength = gROMLength;
						else
							gAllocationLength = gROMLength + (0x10000 - (gROMLength & 0xFFFF));

						if(unzOpenCurrentFile(fp) == UNZ_OK)
						{
							uint64	i;

							InitVirtualRomMemory(gAllocationLength);
							InitMemoryLookupTables();
							InitTLB();

							Is_Reading_Rom_File = TRUE;;
							To_Stop_Reading_Rom_File = FALSE;

							sprintf(generalmessage, "%s [%s] ", TranslateStringByString("Loading"), szFileName);
							SetStatusBarText(0, generalmessage);

							for(i = 0; i < gROMLength && To_Stop_Reading_Rom_File == FALSE; i += 65536)
							/* for( i=0; i<gROMLength; i+=65536) */
							{
								WindowMsgLoop();

								if(To_Stop_Reading_Rom_File == TRUE)
								{
									CloseROM();
									To_Stop_Reading_Rom_File = FALSE;
									Is_Reading_Rom_File = FALSE;

									unzClose(fp);
									return FALSE;
								}

								/* fread(gMS_ROM_Image+i, sizeof(uint8), 65536, fp); */
								if(unzReadCurrentFile(fp, gMS_ROM_Image + i, sizeof(uint8) * 65536) == 65536)
								{
									sprintf(generalmessage, "%s [%s] %d%%",  TranslateStringByString("Loading"), szFileName, i * 100 / gROMLength);
									SetStatusBarText(0, generalmessage);
								}
								else if(unzReadCurrentFile(fp, gMS_ROM_Image + i, 1) == 0)
								{
									sprintf(generalmessage, "%s [%s] %d%%",  TranslateStringByString("Loading"), szFileName, i * 100 / gROMLength);
									SetStatusBarText(0, generalmessage);
								}
								else
								{
									DisplayError("File could not be read. gROMLength = %08X\n", gROMLength);
									CloseROM();
									unzCloseCurrentFile(fp);
									unzClose(fp);
									Set_Ready_Message();
									Is_Reading_Rom_File = FALSE;
									return FALSE;
								}
							}

							ByteSwap(gAllocationLength, gMS_ROM_Image);
							memcpy((uint8 *) &rominfo.validation, gMS_ROM_Image, 0x40);
							SwapRomHeader((uint8 *) &rominfo.validation);

							/* Copy boot code to SP_DMEM */
							memcpy((uint8 *) &SP_DMEM, gMS_ROM_Image, 0x1000);
							memcpy(rominfo.name, gMS_ROM_Image + 0x20, 20);
							SwapRomName(rominfo.name);
							Set_Ready_Message();
							unzCloseCurrentFile(fp);
							unzClose(fp);
							Is_Reading_Rom_File = FALSE;
							return TRUE;
						}
						else
						{
							DisplayError("File could not be read: CRC Error in zip.");
							unzClose(fp);
							return FALSE;
						}
					}
				}
				else
				{
					DisplayError("File could not unzipped.");
					unzClose(fp);
					return FALSE;
				}
			} while(unzGoToNextFile(fp) == UNZ_OK);
		}

		unzClose(fp);
	}

	return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL ByteSwap(uint32 Size, uint8 *Image)
{
	/*~~~~~~*/
	uint32	k;
	/*~~~~~~*/

	if(Image[0] == 0x80 && Image[1] == 0x37)
	{
		for(k = 0; k < Size; k += 4)
		{
			/* Converts 40123780 to 0x80371240 */
			_asm
			{
				mov edx, Image
				add edx, k
				mov eax, dword ptr[edx]
				bswap eax
				mov dword ptr[edx], eax
			}
		}

		return TRUE;
	}
	else if(Image[0] == 0x37 && Image[1] == 0x80)
	/* for (k=Size;k != 0; k-=4) { */
	{
		/* Converts 0x12408037 to 0x80371240 */
		k = Size;
		_asm
		{
			mov edx, Image
			mov ebx, dword ptr[k]
			jmp _LABEL3
		}

_LABEL2:
		_asm
		{
			sub ebx, 8
			and ebx, ebx
			jz _LABELEXIT
		}

_LABEL3:
		_asm
		{
			/* Yup i copied this asm routine twice..to cut down on the looping by 50% */
			mov eax, dword ptr[edx]
			mov ecx, eax
			shr ecx, 0x10
			shl eax, 0x10
			or ecx, eax
			mov dword ptr[edx], ecx
			add edx, 4

			mov eax, dword ptr[edx]
			mov ecx, eax
			shr ecx, 0x10
			shl eax, 0x10
			or ecx, eax
			mov dword ptr[edx], ecx
			add edx, 4
			jmp _LABEL2
		}

_LABELEXIT: ;
		return TRUE;
	}
	else
	{
		if(Image[2] == 0x37 && Image[3] == 0x80)
		{
			/* OK, this rom has already been swapped */
			return TRUE;
		}
		else
		{
			/* This is not a ROM file */
			return FALSE;
		}
	}
}

/*
 =======================================================================================================================
    Load GNU Redistribution Conditions from file
 =======================================================================================================================
 */
int LoadGNUDistConditions(char *ConditionsBuf)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	long	filesize;
	FILE	*fp;
	char	temp_path[MAX_PATH];	/* used for storing application path */
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	_getcwd(temp_path, MAX_PATH);
	strcat(temp_path, "\\dist.txt");

	if((fp = fopen(temp_path, "rb")) == NULL)
	{
		sprintf(ConditionsBuf, "Error: %s not found.", temp_path);
		return(0);
	}

	rewind(fp);
	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if(fp != NULL)
	{
		fread(ConditionsBuf, sizeof(uint8), 11201, fp);
		ConditionsBuf[11201] = '\0';
	}
	else
	{
		sprintf(ConditionsBuf, "Error getting fp");
		return(0);
	}

	fclose(fp);
	return(1);
}

/*
 =======================================================================================================================
    Analyze String //
 =======================================================================================================================
 */
void AnalyzeString(char *temp)
{
	while(*temp!=0)
	{
		if(*temp == '*' || *temp == '?' || *temp == '<' || *temp == '>' ) 
		{
			*temp = '_';
		}

		temp++;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL FileIO_CreateMempakFile(char *filename)
{
	/*~~~~~~~~~~~~~~~~~~~~*/
	FILE	*dest;
	char	temp[1024 * 32];
	gzFile	*gztempfile;
	/*~~~~~~~~~~~~~~~~~~~~*/

	dest = fopen(filename, "wb");
	if(dest)
	{
		fwrite(&gzipped_defaultm0[0], 113, 1, dest);
		fclose(dest);
	}
	else
	{
		DisplayError("Unable to create new mempak file, please check your save directory setting.");
		return FALSE;
	}

	gztempfile = (gzFile*)gzopen(filename, "rb");
	gzread(gztempfile, temp, 1024 * 32);
	gzclose(gztempfile);

	dest = fopen(filename, "wb");
	if(dest)
	{
		fwrite(temp, 1024 * 32, 1, dest);
		fclose(dest);
	}
	else
	{
		DisplayError("Unable to create new mempak file, please check your save directory setting.");
		return FALSE;
	}

	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL FileIO_CreateFile(char *filename, int size)
{
	/*~~~~~~~~~~~~*/
	FILE	*stream;
	_u8		tmp[0x1000] = {0}; //bugfix: A single character will cause fwrite to crash in NULL stream, careful! :)
	/*~~~~~~~~~~~~*/

	stream = fopen(filename, "wb");
	if(stream != NULL)
	{
		fwrite(tmp, 1, size, stream);
		fclose(stream);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*
 =======================================================================================================================
    Get Directory // ?    To create a complete filename to load/save mempak/sram/eeprom //
 =======================================================================================================================
 */
void GetFileName(char *filenameToReturn, char *Ext)
{
	char	CRC[8];
	char	romname[260];
	int		i;

	if( Kaillera_Is_Running && kailleraLocalPlayerNumber != 0 )	// special handling for player 0 hasn't been implemented yet
	//if( Kaillera_Is_Running )
	{
		// See comments for function KailleraResetSaveFiles in Kaillera.c
		strcpy(filenameToReturn, directories.save_directory_to_use);
		strcat(filenameToReturn, "kaillera.");
		strcat(filenameToReturn, Ext);
		return;
	}

	for(i = 0; i < 8; i++)
	{
		CRC[i] = ((char *) &rominfo.crc1)[i ^ 3];
	}

	strcpy(romname, (const char*)rominfo.name);
	for(i = 0; i < (int) strlen(romname); i++)
	{
		if(romname[i] == ':' || romname[i] == '/' )
		{
			romname[i] = '-';
		}
	}

	sprintf
	(
		filenameToReturn,
		"%s%s-%X%X.%s",
		directories.save_directory_to_use,
		romname,
		((_u32 *) &CRC)[0],
		((_u32 *) &CRC)[1],
		Ext
	);
	AnalyzeString(filenameToReturn);
}

/*
 =======================================================================================================================
    Write MemPak Data to File //
 =======================================================================================================================
 */
void FileIO_WriteMemPak(int pak_no)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~*/
	char	temp[1024], ext[5];
	FILE	*stream;
	/*~~~~~~~~~~~~~~~~~~~~~~~*/

	sprintf(ext, "m%i", pak_no);
	GetFileName(temp, ext);

	stream = fopen(temp, "wb");
	if(stream == NULL)
	{
		DisplayError("Cannot Write MEMPAK to file %s", temp);
	}
	else
	{
#ifdef _DEBUG
		if( debugoptions.debug_si_mempak )
		{
			TRACE1("Write MEMPAK to file: %s", temp);
		}
#endif

	fseek(stream, (1024 * 32 * pak_no), SEEK_SET);
    fwrite(gamesave.mempak[pak_no], (1024 * 32), 1, stream);
		fclose(stream);
	}
}

/*
 =======================================================================================================================
    Load MemPak Data from File //
 =======================================================================================================================
 */
void FileIO_LoadMemPak(int pak_no)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~*/
	char	temp[1024], ext[5];
	FILE	*stream;
	/*~~~~~~~~~~~~~~~~~~~~~~~*/

	sprintf(ext, "m%i", pak_no);
	GetFileName(temp, ext);
	stream = fopen(temp, "rb");

	if(stream == NULL)
	{
		if(!FileIO_CreateMempakFile(temp))
		{
			DisplayError("Cannot create an empty MEMPAK file: ", temp);
			return;
		}
		else
		{
			stream = fopen(temp, "rb");
			if(stream == NULL)
			{
				DisplayError("Cannot Load MEMPAK from file %s", temp);
				return;
			}
		}
	}

#ifdef _DEBUG
	if( debugoptions.debug_si_mempak )
	{
		TRACE1("Load MEMPAK from file: %s", temp);
	}
#endif
	fseek(stream, (1024 * 32 * pak_no), SEEK_SET);
    fread(gamesave.mempak[pak_no], (1024 * 32), 1, stream);
	fclose(stream);
}

/*
 =======================================================================================================================
    Write EEprom Data to File //
 =======================================================================================================================
 */
void FileIO_WriteEEprom(void)
{
	/*~~~~~~~~~~~~~~~*/
	char	temp[1024];
	FILE	*stream;
	/*~~~~~~~~~~~~~~~*/

	GetFileName(temp, "eep");
	stream = fopen(temp, "wb");

	if(stream != NULL)
	{
		EEPROM_TRACE(TRACE1("Write EEPROM to file: %s", temp));
		fwrite(gamesave.EEprom, currentromoptions.Eeprom_size == EEPROMSIZE_4KB ? 0x800 : 0x1000, 1, stream);
		fclose(stream);
	}
	else
	{
		EEPROM_TRACE(TRACE0("Cannot write EEPROM to file"));
	}
}

/*
 =======================================================================================================================
    Load EEprom Data from File //
 =======================================================================================================================
 */
void FileIO_LoadEEprom(void)
{
	/*~~~~~~~~~~~~~~~*/
	char	temp[1024];
	FILE	*stream;
	/*~~~~~~~~~~~~~~~*/

	GetFileName(temp, "eep");
	stream = fopen(temp, "rb");


    if(stream == NULL)
	{
		if(!FileIO_CreateFile(temp, currentromoptions.Eeprom_size == EEPROMSIZE_4KB ? 0x800 : 0x1000))
		{
			DisplayError("Cannot Load EEPROM from file %s", temp);
			return;
		}
		else
		{
			stream = fopen(temp, "rb");
			if(stream == NULL)
			{
				DisplayError("Cannot Load EEPROM from file %s", temp);
				return;
			}
		}
	}
	else
	{
		TRACE1("Load EEPROM from file: %s", temp);

        fread(gamesave.EEprom, currentromoptions.Eeprom_size == EEPROMSIZE_4KB ? 0x800 : 0x1000, 1, stream);
        fclose(stream);
	}
}

HANDLE hFlashRam;

void Close_Save()
{
	if (hFlashRam != NULL)
		CloseHandle(hFlashRam);
	hFlashRam = NULL;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void FileIO_WriteFLASHRAM(int FileOffset, int MemOffset, int len)
{
	/*~~~~~~~~~~~~~~*/
	char	temp[256];
	/*~~~~~~~~~~~~~~*/
 
	if ((currentromoptions.Save_Type == ANYUSED_SAVETYPE) ||
		(currentromoptions.Save_Type == SRAM_SAVETYPE))
		GetFileName(temp, "sra");
	else
		GetFileName(temp, "fla");

	if( hFlashRam == NULL )
		hFlashRam = CreateFile(temp,GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);

	DEBUG_SRAM_TRACE(TRACE1("Save Flashram to file: %s", temp));
	
	if ((currentromoptions.Save_Type == ANYUSED_SAVETYPE) ||
		(currentromoptions.Save_Type == SRAM_SAVETYPE))
	{		
		DWORD dwWritten;
		SetFilePointer(hFlashRam,FileOffset&(SRAM_SIZE-1),NULL,FILE_BEGIN);
		WriteFile(hFlashRam, gMS_RDRAM+(MemOffset&0x007FFFFF), len, &dwWritten, 0);
	}
	else
	{
		DWORD dwWritten;

		SetFilePointer(hFlashRam,0,NULL,FILE_BEGIN);
		WriteFile(hFlashRam, pLOAD_UBYTE_PARAM(0xA8000000), 1024 * 128, &dwWritten, 0);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void FileIO_ReadFLASHRAM(int FileOffset, int MemOffset, int len)
{
	/*~~~~~~~~~~~~~~*/
	char	temp[256];
	/*~~~~~~~~~~~~~~*/

	if ((currentromoptions.Save_Type == ANYUSED_SAVETYPE) ||
		(currentromoptions.Save_Type == SRAM_SAVETYPE))
	{	
		GetFileName(temp, "sra");
	}
	else
	{
		GetFileName(temp, "fla");
	}

	if(hFlashRam == NULL)
	{
		hFlashRam = CreateFile(temp,GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
	}
	DEBUG_SRAM_TRACE(TRACE1("Load Flashram from file: %s", temp));

	if ((currentromoptions.Save_Type == ANYUSED_SAVETYPE) ||
		(currentromoptions.Save_Type == SRAM_SAVETYPE))
	{
		DWORD dwWritten;
	
		SetFilePointer(hFlashRam,FileOffset&(SRAM_SIZE-1),NULL,FILE_BEGIN);
		ReadFile(hFlashRam, gMS_RDRAM+(MemOffset&0x007FFFFF), len, &dwWritten, 0);
	}
	else
	{
		DWORD dwWritten;
		
		SetFilePointer(hFlashRam,0,NULL,FILE_BEGIN);
		ReadFile(hFlashRam, pLOAD_UBYTE_PARAM(0xA8000000), 1024 * 128, &dwWritten, 0);
	}
}

extern int	StateFileNumber;

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void FileIO_gzSaveState(void)
{
	/*~~~~~~~~~~~~~~~*/
	/* Open File to write, file should be named as rom name */
	char	temp[1024];
	char	ext[10];
	/*~~~~~~~~~~~~~~~*/

	sprintf(ext, "sav%d", StateFileNumber);
	GetFileName(temp, ext);
	FileIO_gzSaveStateFile_099(temp);
}

/* SaveState layout */
typedef struct	sHardwareSaveState
{
	__int64	GPR[32];				/* General Purpose Registers */
    __int64  grlo;
    __int64  grhi;
	uint32	COP0Reg[32];			/* Coprocessor0 Registers */
	uint32	fpr32[64];				/* 32bit 64 items needed! */
	uint32	LLbit;					/* LoadLinked Bit */
	uint32	pc;						/* program counter */
	uint32	COP1Con[32];			/* FPControl Registers, only 0 and 31 is used */
    uint32	COP0Con[64];			/* Unused? */
} HardwareSaveState;
HardwareSaveState gHardwareSaveState;


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void FileIO_gzReadHardwareState(gzFile *stream, uint32 magic1964)
{
    gzread(stream, &r.r_.gpr,     sizeof(r.r_.gpr));
    gzread(stream, &r.r_.grlo,    sizeof(r.r_.grlo));
    gzread(stream, &r.r_.grhi,    sizeof(r.r_.grhi));
    gzread(stream, &gHardwareState.COP0Reg, sizeof(gHardwareState.COP0Reg));
    gzread(stream, &gHardwareState.fpr32,   sizeof(gHardwareState.fpr32));
    gzread(stream, &gHardwareState.LLbit,   sizeof(gHardwareState.LLbit));
    gzread(stream, &r.r_.pc,      sizeof(r.r_.pc));
    gzread(stream, &gHardwareState.COP1Con, sizeof(gHardwareState.COP1Con));
    

    //new in version 0.9.9
    if (magic1964 >= 0x19640099)
        gzread(stream, &gHardwareState.COP0Con, sizeof(gHardwareState.COP0Con));
}

/*
=======================================================================================================================
=======================================================================================================================
*/
/*
The old sHardwareState struct in version 0.85
*/
typedef struct	sHardwareState_085
{
_int64	GPR[34];				// General Purpose Registers GPR[32] = lo, GPR[33] = hi
uint32	COP0Reg[32];			// Coprocessor0 Registers
uint32	fpr32[64];				// 32bit 64 items needed!
uint32	LLbit;					// LoadLinked Bit
uint32	pc;						// program counter
uint32	COP1Con[32];			// FPControl Registers, only 0 and 31 is used



uint32	COP0Con[64];			// FPControl Registers
uint32	RememberFprHi[32];
uint32	code;					// The instruction
} HardwareSaveState_085;
HardwareSaveState_085 gHardwareSaveState_085;


void FileIO_gzReadHardwareState_085(gzFile *stream)
{
	memset((uint8 *) (&gHardwareSaveState_085), 0, sizeof(gHardwareSaveState_085));
	gzread(
		stream,	(uint8 *) (&gHardwareSaveState_085),
		(
		sizeof(gHardwareSaveState_085) - sizeof(gHardwareSaveState_085.COP0Con) - sizeof(gHardwareSaveState_085.RememberFprHi) - sizeof
		(gHardwareSaveState_085.code)
		)
	);

	memcpy(&r.r_.gpr, &gHardwareSaveState_085.GPR, sizeof(r.r_.gpr));
	r.r_.grlo=gHardwareSaveState_085.GPR[32];
	r.r_.grhi=gHardwareSaveState_085.GPR[33];
	memcpy(&gHardwareState.COP0Reg, &gHardwareSaveState_085.COP0Reg, sizeof(gHardwareState.COP0Reg));
	memcpy(&gHardwareState.fpr32, &gHardwareSaveState_085.fpr32,   sizeof(gHardwareState.fpr32));
	gHardwareState.LLbit=gHardwareSaveState_085.LLbit;
	r.r_.pc=gHardwareSaveState_085.pc;
	memcpy(&gHardwareState.COP1Con, &gHardwareSaveState_085.COP1Con, sizeof(gHardwareState.COP1Con));

	/*
	*/
}


void FileIO_gzWriteHardwareState_085(gzFile *stream)
{
	gHWS_COP0Reg[COUNT] = Get_COUNT_Register();	//Refresh the COUNT register

#ifndef _DEBUG
	if(currentromoptions.Assume_32bit == ASSUME_32BIT_YES)
	{
		int k;
        for(k = 0; k < 32; k++)
            if (k != _t1) /* The weird return value */
			    gHardwareSaveState.GPR[k] = (_int64) (_int32) gHardwareSaveState.GPR[k];

		gHardwareSaveState.grlo = (_int64) (_int32)gHardwareSaveState.grlo;
		gHardwareSaveState.grhi = (_int64) (_int32)gHardwareSaveState.grhi;
	}
#endif

	memcpy(&gHardwareSaveState_085.GPR, &r.r_.gpr, sizeof(gHardwareSaveState_085.GPR));
	gHardwareSaveState_085.GPR[32]=r.r_.grlo;
	gHardwareSaveState_085.GPR[33]=r.r_.grhi;
	memcpy(&gHardwareSaveState_085.COP0Reg, &gHardwareState.COP0Reg, sizeof(gHardwareSaveState_085.COP0Reg));
	memcpy(&gHardwareSaveState_085.fpr32, &gHardwareState.fpr32,   sizeof(gHardwareSaveState_085.fpr32));
	gHardwareSaveState_085.LLbit=gHardwareState.LLbit;
	gHardwareSaveState_085.pc=r.r_.pc;
	memcpy(&gHardwareSaveState_085.COP1Con, &gHardwareState.COP1Con, sizeof(gHardwareSaveState_085.COP1Con));

	gzwrite
		(
		stream,
		(uint8 *) (&gHardwareSaveState_085),
		(
		sizeof(gHardwareSaveState_085) - sizeof(gHardwareSaveState_085.COP0Con) - sizeof(gHardwareSaveState_085.RememberFprHi) - sizeof
		(gHardwareSaveState_085.code)
		)
		);
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */
uint32	Get_COUNT_Register(void);
void DoOthersBeforeSaveState();
void DoOthersAfterLoadState();

void FileIO_gzWriteHardwareState(gzFile *stream)
{

    gHWS_COP0Reg[COUNT] = Get_COUNT_Register();	//Refresh the COUNT register

    memcpy(&gHardwareSaveState.GPR, &r.r_.gpr, sizeof(r.r_.gpr));
    memcpy(&gHardwareSaveState.grlo, &r.r_.grlo, sizeof(r.r_.grlo));
    memcpy(&gHardwareSaveState.grhi, &r.r_.grhi, sizeof(r.r_.grhi));
    memcpy(&gHardwareSaveState.COP0Reg, &gHardwareState.COP0Reg, sizeof(gHardwareState.COP0Reg));
    memcpy(&gHardwareSaveState.fpr32, &gHardwareState.fpr32, sizeof(gHardwareState.fpr32));
    memcpy(&gHardwareSaveState.LLbit, &gHardwareState.LLbit, sizeof(gHardwareState.LLbit));
    memcpy(&gHardwareSaveState.pc, &r.r_.pc, sizeof(r.r_.pc));
    memcpy(&gHardwareSaveState.COP1Con, &gHardwareState.COP1Con, sizeof(gHardwareState.COP1Con));
    
    //new in version 0.9.9
    memcpy(&gHardwareSaveState.COP0Con, &gHardwareState.COP0Con, sizeof(gHardwareState.COP0Con));

#ifndef _DEBUG
    if(currentromoptions.Assume_32bit == ASSUME_32BIT_YES)
    {
		int k;
		/* convert to 32bit sign-extended to 64bit */
        for(k = 0; k < 32; k++)
            if (k != _t1) /* The weird return value */
			    gHardwareSaveState.GPR[k] = (_int64) (_int32) gHardwareSaveState.GPR[k];

        gHardwareSaveState.grlo = (_int64) (_int32)gHardwareSaveState.grlo;
        gHardwareSaveState.grhi = (_int64) (_int32)gHardwareSaveState.grhi;
    }
#endif

    gzwrite(stream, (uint8 *)(&gHardwareSaveState), (sizeof(gHardwareSaveState)));
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void FileIO_gzSaveStateFile(const char *filename, DWORD	magic1964)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	gzFile	*stream;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	stream = (gzFile*)gzopen(filename, "wb");
	if(stream == NULL)
	{
		DisplayError("Cannot create/open gzip file to save state, please check the save directory setting and make sure the directory is created.");
		return;
	}

	TRACE1("Save state into gzip file: %s", filename);

	DoOthersBeforeSaveState();

	gzwrite(stream, (uint8 *) (&magic1964), sizeof(DWORD));
	gzwrite(stream, (uint8 *) (&current_rdram_size), sizeof(uint32));

	/* All CPU registers */
	if( magic1964 == 0x19640064 )
		FileIO_gzWriteHardwareState_085(stream);
	else
		FileIO_gzWriteHardwareState(stream);

	/* All IO registers should be saved */
	gzwrite(stream, (uint8 *) gMS_ramRegs0, 0x30);	/* ramRegs0 */
	gzwrite(stream, (uint8 *) gMS_ramRegs4, 0x30);	/* ramRegs4 */
	gzwrite(stream, (uint8 *) gMS_ramRegs8, 0x30);	/* ramRegs8 */
	gzwrite(stream, (uint8 *) gMS_SP_MEM, MEMORY_SIZE_SPMEM);
	gzwrite(stream, (uint8 *) gMS_SP_REG_1, MEMORY_SIZE_SPREG_1);
	gzwrite(stream, (uint8 *) gMS_SP_REG_2, MEMORY_SIZE_SPREG_2);
	gzwrite(stream, (uint8 *) gMS_DPC, 0x20);		/* DPC */
	gzwrite(stream, (uint8 *) gMS_DPS, 0x10);		/* DPS */
	gzwrite(stream, (uint8 *) gMS_MI, 0x10);		/* MI */
	gzwrite(stream, (uint8 *) gMS_VI, 0x50);		/* VI */
	gzwrite(stream, (uint8 *) gMS_AI, 0x18);		/* AI */
	gzwrite(stream, (uint8 *) gMS_PI, 0x4C);		/* PI */
	gzwrite(stream, (uint8 *) gMS_RI, 0x20);		/* RI */
	gzwrite(stream, (uint8 *) gMS_SI, 0x1C);		/* SI */

	gzwrite(stream, (uint8 *) gMS_RDRAM, current_rdram_size);	/* RDRAM */
	gzwrite(stream, (uint8 *) gMS_GIO_REG, 0x804);				/* GIO_REG */
	gzwrite(stream, (uint8 *) gMS_PIF, 0x800);					/* PIF */

	gzwrite(stream, (uint8 *) gMS_TLB, sizeof(tlb_struct) * MAXTLB);

	gzwrite(stream, (uint8 *) gMS_C2A1, MEMORY_SIZE_C2A1);		/* C2A1 */
	gzwrite(stream, (uint8 *) gMS_C1A1, MEMORY_SIZE_C1A1);		/* C1A1 */
	gzwrite(stream, (uint8 *) gMS_C2A2, MEMORY_SIZE_C2A2);		/* C2A2 */
	gzwrite(stream, (uint8 *) gMS_C1A3, MEMORY_SIZE_C1A3);		/* C1A3 */

	gzclose(stream);
}

void FileIO_gzSaveStateFile_099(const char *filename)
{
	FileIO_gzSaveStateFile(filename, CURRENT_SAVE_STATE_VERSION);
}
void FileIO_gzSaveStateFile_085(const char *filename)
{
	FileIO_gzSaveStateFile(filename, SAVE_STATE_VERSION_085);
}
/*
 =======================================================================================================================
 =======================================================================================================================
 */
void FileIO_gzLoadState(void)
{
	/* Open File to write, file should be named as rom name */
	char	temp[1024];
	char	ext[10];

	sprintf(ext, "sav%d", StateFileNumber);

    GetFileName(temp, ext);
	FileIO_gzLoadStateFile(temp);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */

void FileIO_gzLoadStateFile(const char *filename)
{
    int k, isCompatible;
    
    gzFile	*stream;
	uint32	magic1964;
	uint32	rdram_size;

	Init_Dynarec();
	
	AUDIO_RomClosed(); //Fixes losing audio on a loadstate.

    
    stream = (gzFile*)gzopen(filename, "rb");
	if(stream == NULL)
	{
		DisplayError("Cannot open gzip file to load state");
		return;
	}

	TRACE1("Load state from file: %s", filename);

	
    gzread(stream, (uint8 *) (&magic1964), sizeof(DWORD));
	
    isCompatible = 0;
    for (k=0; k< sizeof(SaveStateVersionList)/4; k++)
        if(magic1964 == SaveStateVersionList[k])
	    {
            isCompatible = 1;
        }

    if (!isCompatible)
    {
        MessageBox(0, TranslateStringByString("This savestate is incompatible with this version of 1964."), TranslateStringByString("Version Conflict"), MB_ICONINFORMATION);
        return;
    }

    gzread(stream, (uint8 *) (&rdram_size), sizeof(uint32));
	ResetRdramSize(rdram_size == 0x400000 ? RDRAMSIZE_4MB : RDRAMSIZE_8MB);

	if( magic1964 == 0x19640064 )
		FileIO_gzReadHardwareState_085(stream);
	else
		FileIO_gzReadHardwareState(stream, magic1964);

	/* All IO registers should be saved */
	gzread(stream, (uint8 *) gMS_ramRegs0, 0x30);					/* ramRegs0 */
	gzread(stream, (uint8 *) gMS_ramRegs4, 0x30);					/* ramRegs4 */
	gzread(stream, (uint8 *) gMS_ramRegs8, 0x30);					/* ramRegs8 */
	gzread(stream, (uint8 *) gMS_SP_MEM, MEMORY_SIZE_SPMEM);		/* SPMEM and SP_REG, size?? */

//    memcpy(tmpROMName, (uint8*)&SP_DMEM + 0x20, 1);
  //  SwapRomName(tmpROMName);
    //strcat(tmpROMName, '\0');
   // DisplayError("%s", tmpROMName);
    
    
    gzread(stream, (uint8 *) gMS_SP_REG_1, MEMORY_SIZE_SPREG_1);	/* SPMEM and SP_REG, size?? */
	gzread(stream, (uint8 *) gMS_SP_REG_2, MEMORY_SIZE_SPREG_2);	/* SPMEM and SP_REG, size?? */
	gzread(stream, (uint8 *) gMS_DPC, 0x20);						/* DPC */
	gzread(stream, (uint8 *) gMS_DPS, 0x10);						/* DPS */
	gzread(stream, (uint8 *) gMS_MI, 0x10);					/* MI */
	gzread(stream, (uint8 *) gMS_VI, 0x50);					/* VI */
	gzread(stream, (uint8 *) gMS_AI, 0x18);					/* AI */
	gzread(stream, (uint8 *) gMS_PI, 0x4C);					/* PI */
	gzread(stream, (uint8 *) gMS_RI, 0x20);					/* RI */
	gzread(stream, (uint8 *) gMS_SI, 0x1C);					/* SI */

	if(currentromoptions.Code_Check == CODE_CHECK_PROTECT_MEMORY)
	{
		UnprotectAllBlocks();
	}

	gzread(stream, (uint8 *) gMS_RDRAM, rdram_size);		/* RDRAM */
	gzread(stream, (uint8 *) gMS_GIO_REG, 0x804);			/* GIO_REG */
	gzread(stream, (uint8 *) gMS_PIF, 0x800);				/* PIF */

	gzread(stream, (uint8 *) gMS_TLB, sizeof(tlb_struct) * MAXTLB);
	Build_Whole_Direct_TLB_Lookup_Table();

	gzread(stream, (uint8 *) gMS_C2A1, MEMORY_SIZE_C2A1);	/* C2A1 */
	gzread(stream, (uint8 *) gMS_C1A1, MEMORY_SIZE_C1A1);	/* C1A1 */
	gzread(stream, (uint8 *) gMS_C2A2, MEMORY_SIZE_C2A2);	/* C2A2 */
	gzread(stream, (uint8 *) gMS_C1A3, MEMORY_SIZE_C1A3);	/* C1A3 */

	gzclose(stream);

	DoOthersAfterLoadState();
	AUDIO_AiLenChanged(); //to fix losing audio on a zelda load state
	Trigger_AIInterrupt(); //to fix losing audio on a zelda (and Hydro Thunder) load state
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL FileIO_Load1964Ini(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	char	inifilepath[_MAX_PATH];
	FILE	*stream;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	strcpy(inifilepath, directories.main_directory);
	strcat(inifilepath, "ROM_Properties.ini");

	stream = fopen(inifilepath, "rt");
	if(stream == NULL)
	{
		DisplayError("Cannot find ROM_Properties.ini file.");
		return FALSE;
	}

	if(ReadAllIniEntries(stream) == FALSE)
	{
		DisplayError("Error reading information from ROM_Properties.ini");
		fclose(stream);
		return FALSE;
	}

	fclose(stream);
	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL FileIO_Write1964Ini(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	char	inifilepath[_MAX_PATH], bakfilepath[_MAX_PATH];
	char	line[1000];
	FILE	*stream, *stream2;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	strcpy(inifilepath, directories.main_directory);
	strcat(inifilepath, "ROM_Properties.ini");

	strcpy(bakfilepath, directories.main_directory);
	strcat(bakfilepath, "ROM_Properties.ini.bak");

	stream2 = fopen(bakfilepath, "wt");
	if(stream2 == NULL)
	{
		DisplayError("Cannot open ROM_Properties.ini file to write.");
		return FALSE;
	}

	stream = fopen(inifilepath, "rt");
	if(stream != NULL)
	{
		do
		{
			if((fgets(line, 255, stream) != NULL) && strncmp(line, "[", 1) != 0)
				fprintf(stream2, "%s", line);
			else
				break;
		} while(feof(stream) != 0);
		fclose(stream);
	}
	else
	{
		DisplayError("ROM_Properties.ini does not exist, create a new one");
	}

	if(WriteAllIniEntries(stream2) == FALSE)
	{
		DisplayError("Error to write information from ROM_Properties.ini");
		fclose(stream2);
		return FALSE;
	}

	fclose(stream2);

	remove(inifilepath);
	rename(bakfilepath, inifilepath);
	return TRUE;
}

BOOL __cdecl	DisplayError_AskIfContinue(char *Message, ...);
const DWORD		SAVESTATE_PROJECT64_MAGIC_NUMBER = 0x23D8A6C8;

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void FileIO_ImportPJ64State(const char *filename)
{
	/*~~~~~~~~~~~~~~~~~*/
	FILE	*stream;
	int		i;
	char	header[0x40];
	uint32	rdram_size;
	DWORD	dummy;
	/*~~~~~~~~~~~~~~~~~*/

	stream = fopen(filename, "rb");
	if(stream == NULL)
	{
		DisplayError("Cannot create/open PJ64 state file to save state");
		return;
	}

	TRACE1("Export PJ64 state: %s", filename);

	fread(&dummy, sizeof(DWORD), 1, stream);
	fread(&rdram_size, sizeof(DWORD), 1, stream);
	ResetRdramSize(rdram_size == 0x400000 ? RDRAMSIZE_4MB : RDRAMSIZE_8MB);

	fread(header, 64, 1, stream);
	if(memcmp(header, gMS_ROM_Image, 0x40) != 0)
	{
		if(!DisplayError_AskIfContinue("Rom header mismatched, this PJ64 save state could be for another ROM"))
		{
			fclose(stream);
			return;
		}
	}

	fread(&gHWS_COP0Reg[COUNT], sizeof(DWORD), 1, stream);
	fread(&gHWS_pc, sizeof(DWORD), 1, stream);
	fread(&gHWS_GPR(0), 256, 1, stream);
	fread(&gHWS_fpr32, 256, 1, stream);
	fread(&gHWS_COP0Reg, 128, 1, stream);
	fread(&gHWS_COP1Con, 128, 1, stream);
	fread(&gHWS_GPR(__HI), 2 * sizeof(DWORD), 1, stream); /* HI */
	fread(&gHWS_GPR(__LO), 2 * sizeof(DWORD), 1, stream); /* LO */

	fread(gMS_ramRegs0, 0x28, 1, stream);
	fread(gMS_SP_REG_1, 0x28, 1, stream);
	fread(gMS_DPC, 0x28, 1, stream);
	fread(gMS_MI, 0x10, 1, stream);
	fread(gMS_VI, 0x38, 1, stream);
	fread(gMS_AI, 0x18, 1, stream);
	fread(gMS_PI, 0x34, 1, stream);
	fread(gMS_RI, 0x20, 1, stream);

	fread(&SI_DRAM_ADDR_REG, sizeof(DWORD), 1, stream);
	fread(&SI_PIF_ADDR_RD64B_REG, sizeof(DWORD), 1, stream);
	fread(&SI_PIF_ADDR_WR64B_REG, sizeof(DWORD), 1, stream);
	fread(&SI_STATUS_REG, sizeof(DWORD), 1, stream);

	for(i = 0; i < MAXTLB; i++)
	{
		/*~~~~~~~~~~*/
		DWORD	valid;
		/*~~~~~~~~~~*/

		fread(&valid, sizeof(DWORD), 1, stream);
		fread(&(gMS_TLB[i].PageMask), sizeof(DWORD), 1, stream);
		fread(&(gMS_TLB[i].EntryHi), sizeof(DWORD), 1, stream);
		fread(&(gMS_TLB[i].EntryLo0), sizeof(DWORD), 1, stream);
		fread(&(gMS_TLB[i].EntryLo1), sizeof(DWORD), 1, stream);
		if(valid)
		{
			gMS_TLB[i].EntryLo1 |= valid;
			gMS_TLB[i].EntryLo0 |= valid;
		}
		else
		{
			gMS_TLB[i].EntryLo1 &= 0xFFFFFFFE;
			gMS_TLB[i].EntryLo0 &= 0xFFFFFFFE;
		}
	}
	Build_Whole_Direct_TLB_Lookup_Table();


	fread(gMS_PIF, 0x40, 1, stream);
	fread(gMS_RDRAM, rdram_size, 1, stream);
	fread(gMS_SP_MEM, MEMORY_SIZE_SPMEM, 1, stream);

	fclose(stream);

	if(currentromoptions.Code_Check == CODE_CHECK_PROTECT_MEMORY)
	{
		UnprotectAllBlocks();
	}

	DoOthersAfterLoadState();
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void FileIO_ExportPJ64State(const char *filename)
{
	/*~~~~~~~~~~~~~~~~~*/
	FILE	*stream;
	int		i;
	char	header[0x40];
	/*~~~~~~~~~~~~~~~~~*/

	stream = fopen(filename, "wb");
	if(stream == NULL)
	{
		DisplayError("Cannot create/open PJ64 state file to save state");
		return;
	}

	TRACE1("Export PJ64 state: %s", filename);

	fwrite(&SAVESTATE_PROJECT64_MAGIC_NUMBER, sizeof(DWORD), 1, stream);
	fwrite(&current_rdram_size, sizeof(DWORD), 1, stream);

	memcpy(header, gMS_ROM_Image, 0x40);
	fwrite(header, 64, 1, stream);

	gHWS_COP0Reg[COUNT] = Get_COUNT_Register();	//Refresh the COUNT register
	fwrite(&gHWS_COP0Reg[COUNT], sizeof(DWORD), 1, stream);
	fwrite(&gHWS_pc, sizeof(DWORD), 1, stream);
	fwrite(&gHWS_GPR(0), 256, 1, stream);
	fwrite(&gHWS_fpr32, 256, 1, stream);
	fwrite(&gHWS_COP0Reg, 128, 1, stream);
	fwrite(&gHWS_COP1Con, 128, 1, stream);
	fwrite(&gHWS_GPR(__HI), 2 * sizeof(DWORD), 1, stream);	/* HI */
	fwrite(&gHWS_GPR(__LO), 2 * sizeof(DWORD), 1, stream);	/* LO */

	fwrite(gMS_ramRegs0, 0x28, 1, stream);
	fwrite(gMS_SP_REG_1, 0x28, 1, stream);
	fwrite(gMS_DPC, 0x28, 1, stream);
	fwrite(gMS_MI, 0x10, 1, stream);
	fwrite(gMS_VI, 0x38, 1, stream);
	fwrite(gMS_AI, 0x18, 1, stream);
	fwrite(gMS_PI, 0x34, 1, stream);
	fwrite(gMS_RI, 0x20, 1, stream);

	fwrite(&SI_DRAM_ADDR_REG, sizeof(DWORD), 1, stream);
	fwrite(&SI_PIF_ADDR_RD64B_REG, sizeof(DWORD), 1, stream);
	fwrite(&SI_PIF_ADDR_WR64B_REG, sizeof(DWORD), 1, stream);
	fwrite(&SI_STATUS_REG, sizeof(DWORD), 1, stream);

	for(i = 0; i < MAXTLB; i++)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		DWORD	valid = (gMS_TLB[i].EntryLo0 & TLBLO_G & gMS_TLB[i].EntryLo1);
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		fwrite(&valid, sizeof(DWORD), 1, stream);
		fwrite(&(gMS_TLB[i].PageMask), sizeof(DWORD), 1, stream);
		fwrite(&(gMS_TLB[i].EntryHi), sizeof(DWORD), 1, stream);
		fwrite(&(gMS_TLB[i].EntryLo0), sizeof(DWORD), 1, stream);
		fwrite(&(gMS_TLB[i].EntryLo1), sizeof(DWORD), 1, stream);
	}

	fwrite(gMS_PIF, 0x40, 1, stream);
	fwrite(gMS_RDRAM, current_rdram_size, 1, stream);
	fwrite(gMS_SP_MEM, MEMORY_SIZE_SPMEM, 1, stream);

	fclose(stream);
}

BOOL FileIO_CheckAndCreateFolder(const char* pathname)
{
	if( !PathFileExists(pathname) )
	{
		if( !CreateDirectory(pathname, NULL) )
		{
			DisplayError("Can not create new folder: %s", pathname);
			return FALSE;
		}
	}

	return TRUE;
}


char *myfgets(char *line, int maxlen, FILE *file)
{
	int i=0;
	int c;
	char *ptr=line;

	for( i=0; i<maxlen-1; i++ )
	{
		c=fgetc(file);
		if( c == EOF )
		{
			*ptr=0;

			if( line == ptr )
				return NULL;
			else
				return line;
		}

		*ptr++ = c;
		if( c == '\n' )
		{
			*ptr=0;
			return line;
		}
	}

	*ptr=0;
	return line;
}
