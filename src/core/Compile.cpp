// __________________________________________________________________________________________________
//
//			$Archive:  $
//			$Revision: 1.2 $
//			$Author: tronic $ 
//			$Modtime:  $  
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
// M O D U L E  B E G I N ///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include "stdafx.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////
// D E F I N E S ////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
	unsigned short f_magic;         /* magic number             */
	unsigned short f_nscns;         /* number of sections       */
	unsigned long  f_timdat;        /* time & date stamp        */
	unsigned long  f_symptr;        /* file pointer to symtab   */
	unsigned long  f_nsyms;         /* number of symtab entries */
	unsigned short f_opthdr;        /* sizeof(optional hdr)     */
	unsigned short f_flags;         /* flags                    */
} FILHDR;

typedef struct {
	char           s_name[8];  /* section name                     */
	unsigned long  s_paddr;    /* physical address, aliased s_nlib */
	unsigned long  s_vaddr;    /* virtual address                  */
	unsigned long  s_size;     /* section size                     */
	unsigned long  s_scnptr;   /* file ptr to raw data for section */
	unsigned long  s_relptr;   /* file ptr to relocation           */
	unsigned long  s_lnnoptr;  /* file ptr to line numbers         */
	unsigned short s_nreloc;   /* number of relocation entries     */
	unsigned short s_nlnno;    /* number of line number entries    */
	unsigned long  s_flags;    /* flags                            */
} SCNHDR;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// I M P L E M E N T A T I O N //////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

// __________________________________________________________________________________________________
// LoadCOFF
//
// GCC and M$
//
DWORD LoadCOFF(char* _szFilename, BYTE* _pMachineCode, DWORD _dwMaxSize)
{
	DWORD dwCodeSize = 0;

	FILE* pFileObj = fopen(_szFilename, "rb");
	if (pFileObj != NULL)
	{
        int i;
		FILHDR fileHeader;
		SCNHDR* sectionHeaders;
		fread(&fileHeader, 1, sizeof(FILHDR), pFileObj);

		sectionHeaders = (SCNHDR* ) malloc(sizeof(SCNHDR)*fileHeader.f_nscns);

        for (i=0; i<fileHeader.f_nscns; i++)
		{
			fread(&sectionHeaders[i], 1, sizeof(SCNHDR), pFileObj);
		}

		// read all sections
		for (i=0; i<fileHeader.f_nscns; i++)
		{
			SCNHDR* sectionHeader = &sectionHeaders[i];
			if (!strcmp(sectionHeader->s_name, ".text"))
			{
				if (sectionHeader->s_flags & 0x20)
				{
					fseek(pFileObj, sectionHeader->s_scnptr ,SEEK_SET);
					fread(_pMachineCode, 1, sectionHeader->s_size, pFileObj);
					dwCodeSize += sectionHeader->s_size;					
					// code

				}
			}

		}
		fclose(pFileObj);
		free(sectionHeaders);
	}


    return dwCodeSize;
}

// __________________________________________________________________________________________________
// LoadOMF
//
// WATCOM
//
void LoadOMF(char* _szFilename, BYTE* _pMachineCode, DWORD _dwMaxSize)
{
	// load obj
	FILE* pFileObj = fopen(_szFilename, "rb");
	if (pFileObj != NULL)
	{
		DWORD pos = 0;
		BOOL bFirstCode = TRUE;
		DWORD length;
		fseek(pFileObj,0,SEEK_END);
		length = ftell(pFileObj);

		while(pos < length)
		{
			BYTE rec_cmd;
			unsigned short rec_length;
			fseek(pFileObj,pos,SEEK_SET);	
			fread(&rec_cmd, 1, sizeof(rec_cmd), pFileObj);
			fread(&rec_length, 1, sizeof(rec_length), pFileObj);

			if (rec_cmd == 0xa1)
			{
				BYTE seg_index;
				DWORD data_offset;
				DWORD code_size;

				fread(&seg_index, 1, sizeof(seg_index), pFileObj);
				fread(&data_offset, 1, sizeof(data_offset), pFileObj);

				code_size = rec_length - sizeof(seg_index) - sizeof(data_offset) - 1; // -1 for the checksum

				if (_dwMaxSize > code_size)
				{
					fread(_pMachineCode, 1, code_size, pFileObj);
					_pMachineCode += code_size;
				}
				else
					__asm int 3;
			}

			pos += rec_length + 3;  // +3 for rec_length and checksum
		}	
		fclose(pFileObj);
	}
}

// __________________________________________________________________________________________________
// CompileSourceCode
//
DWORD CompileSourceCode(char* _szSourceCode, BYTE* _pMachineCode, DWORD _dwMaxSize)
{	

	// COMPILE THE THING
	char path_buffer[_MAX_PATH], drive[_MAX_DRIVE] ,dir[_MAX_DIR];
	char fname[_MAX_FNAME],ext[_MAX_EXT], FullPath[_MAX_PATH];
	char execPath[MAX_PATH];
	char szCommadLine[MAX_PATH];
	char srcPath[MAX_PATH];
	FILE* pFileSrc;
	PROCESS_INFORMATION pi; 
	STARTUPINFO si; 

	GetModuleFileName(NULL,path_buffer,sizeof(path_buffer));
	_splitpath( path_buffer, drive, dir, fname, ext );
	_makepath( FullPath, drive, dir, NULL, NULL);

	sprintf (execPath, "%s%s", FullPath, "CompTemp");

    
	// WRITE FILE TO CPP
    strcpy(srcPath, execPath);
    strcat(srcPath, "\\src.c");
    pFileSrc = fopen(srcPath, "wt");
	if (pFileSrc != NULL)
	{
		fprintf(pFileSrc, _szSourceCode);
		fclose(pFileSrc);
	}
    
    
    
    
    sprintf (szCommadLine, "\"%s%s\"", execPath, "\\gccb.bat");


	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO); 
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE ;
	
	if (CreateProcess(NULL, szCommadLine, //szCommadLine, 
			0, 0, 0, HIGH_PRIORITY_CLASS, 
			0, execPath, &si, &pi)) 
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

    strcat (execPath, "\\src.o"); //allows exe to be executed from anywhere.
	return LoadCOFF(execPath, _pMachineCode, _dwMaxSize);
}




// __________________________________________________________________________________________________
// InsertEventString
//
DWORD InsertEventString(char* _szEventSourceCode)
{
	return 0;
}