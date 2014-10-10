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
#include "stdafx.h"
#include <math.h>


RecentCIInfo g_RecentCIInfo[3];
RecentCIInfo *g_uRecentCIInfoPtrs[3] =
{
	&g_RecentCIInfo[0],
	&g_RecentCIInfo[1],
	&g_RecentCIInfo[2],
};

FrameBufferInfo curFrameBufInfos[6];

uint32 g_FrmBufWidth;
uint32 g_FrmBufHeight;

void protect_framebuffer_memory_set_func_array(uint32 pc);
void unprotect_framebuffer_memory_set_func_array(uint32 pc);

// ================================================================================
void InitFrameBufferProtection(void)
{
	memset(g_RecentCIInfo, 0, sizeof(g_RecentCIInfo));
	memset(&curFrameBufInfos, 0, sizeof(curFrameBufInfos));
}

void SetVIScales()
{
	float xscale, yscale;
	float fViWidth, fViHeight;
	float fRatio = 0.75;

	uint32 val, start, end, width;

	val = VI_X_SCALE_REG & 0xFFF;
	start = VI_H_START_REG >> 16;
	end = VI_H_START_REG&0xFFFF;
	width = VI_WIDTH_REG;

	if(rominfo.TV_System != TV_SYSTEM_NTSC) fRatio = 9/11.0f;

	xscale = (float)val / (1<<10);
	fViWidth = (end-start)*xscale;
	if( fViWidth > width && fViWidth - width < 8 ) 
		fViWidth = (float)width;
	else
	{
		//DebuggerAppendMsg("fViWidth = %f, Width Reg=%d", fViWidth, width);
	}

	val = (VI_Y_SCALE_REG & 0xFFF);// - ((VI_Y_SCALE_REG>>16) & 0xFFF);
	yscale = (float)val / (1<<10);
	start = VI_V_START_REG >> 16;
	end = VI_V_START_REG&0xFFFF;
	fViHeight = (end-start)/2*yscale;
	
	
	if( yscale == 0 )
	{
		fViHeight = fViWidth*fRatio;
	}
	else
	{
		if( VI_WIDTH_REG > 0x300 ) fViHeight *= 2;

		if( fViWidth*fRatio > fViHeight )
		{
			if( fViWidth*fRatio - fViHeight < 8 )
			{
				fViHeight = fViWidth*fRatio;
			}
			else
			{
				if( fabs(fViWidth*fRatio-fViHeight) > fViWidth*0.1f )
				{
					if( fRatio > 0.8 )
						fViHeight = fViWidth*3/4;
					//fViHeight = (VI_V_SYNC_REG - 0x2C)/2;
				}
			}
		}
		
		if( fViHeight<100 || fViWidth<100 )
		{
			//At sometime, value in VI_H_START_REG or VI_V_START_REG are 0
			fViWidth = (float)VI_WIDTH_REG;
			fViHeight = fViWidth*fRatio;
		}
	}

	g_FrmBufWidth = (uint32)(fViWidth/4)*4;;
	g_FrmBufHeight = (uint32)(fViHeight/4)*4;
}


void UpdateRecentFrmBufAddr()
{
	RecentCIInfo *temp;

	uint32 addr = VI_ORIGIN_REG - 2*VI_WIDTH_REG;
	SetVIScales();

	if( addr < 2*g_FrmBufWidth ) return;
	
	temp = g_uRecentCIInfoPtrs[2];
	g_uRecentCIInfoPtrs[2] = g_uRecentCIInfoPtrs[1];
	g_uRecentCIInfoPtrs[1] = g_uRecentCIInfoPtrs[0];

	g_uRecentCIInfoPtrs[0] = temp;
	temp->dwAddr = addr;
	temp->dwWidth = g_FrmBufWidth;
	temp->dwHeight = g_FrmBufHeight;
	temp->dwMemSize = g_FrmBufWidth*g_FrmBufHeight*2;
	temp->dwHighAddr = temp->dwAddr+temp->dwMemSize;
	temp->bCopied = FALSE;
}

int GetPtrIdex(uint32 addr)
{
	if( (addr & 0x80000000) == 0 )	return -1;

	addr &= 0x3FFFFFFF;
	if( addr >= current_rdram_size )	return -1;

	if( emustatus.VideoPluginProvideFrameBufferInfo )
	{
		int i;
		for( i=0; i<6; i++ )
		{
			if( curFrameBufInfos[i].addr && (addr >= curFrameBufInfos[i].addr && addr < curFrameBufInfos[i].addr+curFrameBufInfos[i].size*curFrameBufInfos[i].width*curFrameBufInfos[i].height ) )
				return i;
		}

		return -1;
	}
	else
	{
		if( g_uRecentCIInfoPtrs[1]->dwAddr <= addr && addr < g_uRecentCIInfoPtrs[1]->dwHighAddr )
			return 1;
		else if( g_uRecentCIInfoPtrs[0]->dwAddr <= addr && addr < g_uRecentCIInfoPtrs[0]->dwHighAddr )
			return 0;
		else if( g_uRecentCIInfoPtrs[2]->dwAddr <= addr && addr < g_uRecentCIInfoPtrs[2]->dwHighAddr )
			return 2;
		else
			return -1;
	}
}


/******************************************************************
Function: FrameBufferRead
Purpose:  This function is called to notify the dll that the
frame buffer memory is beening read at the given address.
DLL should copy content from its render buffer to the frame buffer
in N64 RDRAM
DLL is responsible to maintain its own frame buffer memory addr list
DLL should copy 4KB block content back to RDRAM frame buffer.
Emulator should not call this function again if other memory
is read within the same 4KB range
input:    addr		rdram address
output:   none
*******************************************************************/ 

void SetFrameBufferHasBeenRead(uint32 addr)
{
	if( GetPtrIdex(addr) != -1 )
	{
		DEBUG_FRAMEBUFFER_TRACE(TRACE1("FrameBuffer is read at %08X", addr));
		if( emustatus.VideoPluginSupportingFrameBuffer )
		{
			VIDEO_FrameBufferRead(addr);
		}
		else
		{
			unprotect_framebuffer_memory_set_func_array(addr);
		}
		//unprotect_framebuffer_memory_from_READ_set_func_array(addr);
		emustatus.FrameBufferProtectionNeedToBeUpdate = TRUE;
	}
}

void SetFrameBufferHasBeenWritten(uint32 addr)
{
	int i;

	if( (i = GetPtrIdex(addr)) != -1 )
	{
		DEBUG_FRAMEBUFFER_TRACE(TRACE1("FrameBuffer is written at %08X", addr));
		if( emustatus.VideoPluginSupportingFrameBuffer )
		{
			VIDEO_FrameBufferWrite(addr, 4);
			emustatus.FrameBufferProtectionNeedToBeUpdate = TRUE;
		}
		else
		{
			if( !emustatus.VideoPluginProvideFrameBufferInfo )
				g_uRecentCIInfoPtrs[i]->bHasBeenWritten = TRUE;
		}
	}
}

void ProtectFrameBufferMemory()
{
	uint32 addr, num, i;

	if( emustatus.VideoPluginProvideFrameBufferInfo )
	{
		FrameBufferInfo tempinfo[6];
		memset(&tempinfo,0,sizeof(FrameBufferInfo)*6);
		VIDEO_GetFrameBufferInfo(&tempinfo);

		for( i=0; i<6; i++ )
		{
			uint32 k;
			if( tempinfo[i].addr != 0 )
			{
				if( curFrameBufInfos[i].addr != tempinfo[i].addr || curFrameBufInfos[i].height != tempinfo[i].height || curFrameBufInfos[i].size != tempinfo[i].size
					|| curFrameBufInfos[i].width != tempinfo[i].width || emustatus.FrameBufferProtectionNeedToBeUpdate == TRUE )
				{
					if( curFrameBufInfos[i].addr != 0 )
					{
						addr = curFrameBufInfos[i].addr;
						num = curFrameBufInfos[i].size*curFrameBufInfos[i].width*curFrameBufInfos[i].height/0x1000;
						DEBUG_FRAMEBUFFER_TRACE(TRACE1("Unprotect old frame buffer at: %08X", curFrameBufInfos[i].addr));

						for( k=0; k<num; k++, addr+=0x1000 )
						{
							unprotect_framebuffer_memory_set_func_array(addr);
						}
					}

					curFrameBufInfos[i].addr = tempinfo[i].addr;
					curFrameBufInfos[i].height = tempinfo[i].height;
					curFrameBufInfos[i].size = tempinfo[i].size;
					curFrameBufInfos[i].width = tempinfo[i].width;

					addr = curFrameBufInfos[i].addr;
					num = (curFrameBufInfos[i].size*curFrameBufInfos[i].width*curFrameBufInfos[i].height+0xfff)/0x1000;
					DEBUG_FRAMEBUFFER_TRACE(TRACE1("Protect new frame buffer at: %08X", curFrameBufInfos[i].addr));

					for( k=0; k<num; k++, addr+=0x1000 )
					{
						protect_framebuffer_memory_set_func_array(addr);
					}
				}

			}
		}

		emustatus.FrameBufferProtectionNeedToBeUpdate = FALSE;
	}
	else
	{
		RecentCIInfo *info = g_uRecentCIInfoPtrs[0];
		addr = VI_ORIGIN_REG - 2*VI_WIDTH_REG;
		if( addr < 0x400 ) return;

		i = GetPtrIdex(addr);
		if( i == -1 )
		{
			addr = g_uRecentCIInfoPtrs[2]->dwAddr;
			if( addr != 0 || emustatus.FrameBufferProtectionNeedToBeUpdate == TRUE )
			{
				num = g_uRecentCIInfoPtrs[2]->dwMemSize/0x1000;
				TRACE1("Unprotect old frame buffer at: %08X", g_uRecentCIInfoPtrs[2]->dwAddr);

				for( i=0; i<num; i++, addr+=0x1000 )
				{
					unprotect_framebuffer_memory_set_func_array(addr);
				}
			}

			UpdateRecentFrmBufAddr();
			addr = VI_ORIGIN_REG&0xFFFFF000;
			g_uRecentCIInfoPtrs[0]->bHasBeenRead = FALSE;
			g_uRecentCIInfoPtrs[0]->bHasBeenWritten = FALSE;
			g_uRecentCIInfoPtrs[0]->bCopied = FALSE;
			num = (g_uRecentCIInfoPtrs[0]->dwMemSize+0xfff)/0x1000;
			TRACE1("Protect new frame buffer at: %08X", g_uRecentCIInfoPtrs[0]->dwAddr);

			for( i=0; i<num; i++, addr+=0x1000 )
			{
				protect_framebuffer_memory_set_func_array(addr);
			}
		}
		else
		{
			RecentCIInfo *temp = g_uRecentCIInfoPtrs[i];
			if( i == 1 )
			{
				g_uRecentCIInfoPtrs[1] = g_uRecentCIInfoPtrs[0];
				g_uRecentCIInfoPtrs[0] = temp;
			}
			else if( i == 2 )
			{
				g_uRecentCIInfoPtrs[2] = g_uRecentCIInfoPtrs[1];
				g_uRecentCIInfoPtrs[1] = g_uRecentCIInfoPtrs[0];
				g_uRecentCIInfoPtrs[0] = temp;
			}
		}
	}
}
