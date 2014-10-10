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


// Don't include this file to any files other then r4300i.c

// Call this function when VI_ORIGIN_REG is changed which mean to switch to new frame buffer
// CI_Addr = VI_ORIGIN_REG - 2*VI_WIDTH_REG  or CI_Addr = VI_ORIGIN_REG&0xFFFFF000
// always starts at 4KB boundary

// This function will memory from CI_Addr to CI_Addr+width*height*2 under read/write protection
// so to watch any CPU read/write opcodes to this memory range and inform video plugin any of
// such read/write

// These frame buffer functions will keep a list of recent used frame buffer addr,
// supports upto 3 frame buffers
#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_
#include <math.h>

typedef struct
{
	// Frame buffer is always RGBA-16 format

	unsigned __int32	dwAddr;

	unsigned __int32	dwWidth;
	unsigned __int32	dwHeight;

	unsigned __int32	dwMemSize;
	unsigned __int32	dwHighAddr;

	BOOL				bCopied;
	BOOL				bHasBeenRead;
	BOOL				bHasBeenWritten;

	unsigned __int32	dwCRC;
} RecentCIInfo;


extern RecentCIInfo g_RecentCIInfo[3];
extern RecentCIInfo *g_uRecentCIInfoPtrs[3];
extern uint32 g_FrmBufWidth;
extern uint32 g_FrmBufHeight;

extern void protect_framebuffer_memory_set_func_array(uint32 pc);
extern void unprotect_framebuffer_memory_set_func_array(uint32 pc);
extern void unprotect_framebuffer_memory_from_READ_set_func_array(uint32 pc);

// ================================================================================
extern void InitFrameBufferProtection(void);

extern void SetVIScales();
extern void UpdateRecentFrmBufAddr();

extern int GetPtrIdex(uint32 addr);
extern void SetFrameBufferHasBeenRead(uint32 addr);
extern void SetFrameBufferHasBeenWritten(uint32 addr);
extern void ProtectFrameBufferMemory();

#endif
