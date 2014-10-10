/*
Copyright (C) 2002 Rice1964

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

// This file should be only included in RDP_GRX.h, this is just a collection of related
// functions, not a global header file.

// ===========================================================================

// fix me, make me not depend on DirectX
#include "DXGraphicsContext.h"
#include "Render.h"
#include "D3DRender.h"

// 0 keeps the most recent CI info
// 1 keeps the frame buffer CI info which is being displayed now
// 2 keeps the older frame buffer CI info. This can be used if we are using triple buffer


RecentCIInfo g_RecentCIInfo[5];
RecentCIInfo *g_uRecentCIInfoPtrs[5] =
{
	&g_RecentCIInfo[0],
	&g_RecentCIInfo[1],
	&g_RecentCIInfo[2],
	&g_RecentCIInfo[3],
	&g_RecentCIInfo[4],
};

const int numOfRecentCIInfos = 5;

RecentViOriginInfo g_RecentVIOriginInfo[numOfRecentCIInfos];
uint32 dwBackBufferSavedAtFrame=0;


// ===========================================================================
bool FrameBufferInRDRAMCheckCRC();

uint16 ConvertRGBATo555(uint8 r, uint8 g, uint8 b, uint8 a)
{
	uint8 ar = a>=0x20?1:0;
	return ((r>>3)<<RGBA5551_RedShift) | ((g>>3)<<RGBA5551_GreenShift) | ((b>>3)<<RGBA5551_BlueShift) | ar;//(a>>7);
}

uint16 ConvertRGBATo555(uint32 color32)
{
	return uint16((((color32>>19)&0x1F)<<RGBA5551_RedShift) | (((color32>>11)&0x1F)<<RGBA5551_GreenShift) | (((color32>>3)&0x1F)<<RGBA5551_BlueShift) | ((color32>>31)));;
}

void UpdateRecentCIAddr(SetImgInfo &ciinfo)
{
	int i=0;
	
	if( ciinfo.dwAddr == g_uRecentCIInfoPtrs[0]->dwAddr )
		return;
	
	RecentCIInfo *temp;

	for( i=1; i<numOfRecentCIInfos; i++ )
	{
		if( ciinfo.dwAddr == g_uRecentCIInfoPtrs[i]->dwAddr )
		{
			temp = g_uRecentCIInfoPtrs[i];

			for( int j=i; j>0; j-- )
			{
				g_uRecentCIInfoPtrs[j] = g_uRecentCIInfoPtrs[j-1];
			}
			break;
		}
	}

	if( i >= numOfRecentCIInfos )
	{
		temp = g_uRecentCIInfoPtrs[4];
		g_uRecentCIInfoPtrs[4] = g_uRecentCIInfoPtrs[3];
		g_uRecentCIInfoPtrs[3] = g_uRecentCIInfoPtrs[2];
		g_uRecentCIInfoPtrs[2] = g_uRecentCIInfoPtrs[1];
		g_uRecentCIInfoPtrs[1] = g_uRecentCIInfoPtrs[0];
		temp->dwCopiedAtFrame = 0;
		temp->bCopied = false;
	}

	g_uRecentCIInfoPtrs[0] = temp;

	// Fix me here for Mario Tennis
	temp->dwLastWidth = windowSetting.uViWidth;
	temp->dwLastHeight = windowSetting.uViHeight;

	temp->dwFormat = ciinfo.dwFormat;
	temp->dwAddr = ciinfo.dwAddr;
	temp->dwSize = ciinfo.dwSize;
	temp->dwWidth = ciinfo.dwWidth;
	temp->dwHeight = gRDP.scissor.bottom;
	temp->dwMemSize = (temp->dwWidth*temp->dwHeight/2)<<temp->dwSize;
	temp->bCopied = false;
	temp->lastUsedFrame = status.gDlistCount;
}


/************************************************************************/
/* Mark the ciinfo entry that the ciinfo is used by VI origin register  */
/* in another word, this is a real frame buffer, not a fake frame buffer*/
/* Fake frame buffers are never really used by VI origin				*/
/************************************************************************/
void SetAddrUsedByVIOrigin(uint32 addr)
{
	int i=0;
	uint32 viwidth = *g_GraphicsInfo.VI_WIDTH_REG;
	addr &= (g_dwRamSize-1);
	for( i=0; i<numOfRecentCIInfos; i++ )
	{
		if( g_uRecentCIInfoPtrs[i]->dwAddr+2*viwidth == addr )
		{
			g_uRecentCIInfoPtrs[i]->bUsedByVIAtFrame = status.gDlistCount;
		}
		else if( addr >= g_uRecentCIInfoPtrs[i]->dwAddr && addr < g_uRecentCIInfoPtrs[i]->dwAddr+0x1000 )
		{
			g_uRecentCIInfoPtrs[i]->bUsedByVIAtFrame = status.gDlistCount;
		}
	}

	for( i=0; i<numOfRecentCIInfos; i++ )
	{
		if( g_RecentVIOriginInfo[i].addr == addr )
		{
			g_RecentVIOriginInfo[i].FrameCount = status.gDlistCount;
			return;
		}
	}

	for( i=0; i<numOfRecentCIInfos; i++ )
	{
		if( g_RecentVIOriginInfo[i].addr == 0 )
		{
			// Never used
			g_RecentVIOriginInfo[i].addr = addr;
			g_RecentVIOriginInfo[i].FrameCount = status.gDlistCount;
			return;
		}
	}

	int index=0;
	uint32 minFrameCount = 0xffffffff;

	for( i=0; i<numOfRecentCIInfos; i++ )
	{
		if( g_RecentVIOriginInfo[i].FrameCount < minFrameCount )
		{
			index = i;
			minFrameCount = g_RecentVIOriginInfo[i].FrameCount;
		}
	}

	g_RecentVIOriginInfo[index].addr = addr;
	g_RecentVIOriginInfo[index].FrameCount = status.gDlistCount;
}

bool IsAddrUsedByVIorigin(uint32 addr, uint32 width)
{
	int i=0;
	
	addr &= (g_dwRamSize-1);
	for( i=0; i<numOfRecentCIInfos; i++ )
	{
		if( g_uRecentCIInfoPtrs[i]->dwAddr == 0 )
			continue;

		if( g_uRecentCIInfoPtrs[i]->dwAddr == addr )
		{
			if( status.gDlistCount-g_uRecentCIInfoPtrs[i]->bUsedByVIAtFrame < 20 )
			//if( g_uRecentCIInfoPtrs[i]->bUsedByVIAtFrame != 0 )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	for( i=0; i<numOfRecentCIInfos; i++ )
	{
		if( g_RecentVIOriginInfo[i].addr != 0 )
		{
			if( g_RecentVIOriginInfo[i].addr > addr && 
				(g_RecentVIOriginInfo[i].addr - addr)%width == 0 &&
				(g_RecentVIOriginInfo[i].addr - addr)/width <= 4)
			{
				if( status.gDlistCount-g_RecentVIOriginInfo[i].FrameCount < 20 )
				//if( g_RecentVIOriginInfo[i].FrameCount != 0 )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
	
	if( status.gDlistCount > 20 )
		return false;
	else
	{
		return true;
	}
}

int FindRecentCIInfoIndex(uint32 addr)
{
	for( int i=0; i<numOfRecentCIInfos; i++ )
	{
		if( g_uRecentCIInfoPtrs[i]->dwAddr <= addr && addr < g_uRecentCIInfoPtrs[i]->dwAddr+g_uRecentCIInfoPtrs[i]->dwMemSize )
		{
			return i;
		}
	}
	return -1;
}

bool IsDIATextureBuffer()
{
	// Knowing g_CI and g_ZI

	//if( g_CI.dwWidth )

	bool foundSetScissor=false;
	bool foundFillRect=false;
	bool foundSetFillColor=false;
	bool foundSetCImg=false;
	bool foundTxtRect=false;
	int ucodeLength=10;
	int height;
	uint32 newFillColor;

	uint32 dwPC = gDlistStack[gDlistStackPointer].pc;		// This points to the next instruction

	for( int i=0; i<10; i++ )
	{
		uint32 w0 = *(uint32 *)(g_pRDRAMu8 + dwPC + i*8);
		uint32 w1 = *(uint32 *)(g_pRDRAMu8 + dwPC + 4 + i*8);

		if( (w0>>24) == RDP_SETSCISSOR )
		{
			height   = ((w1>>0 )&0xFFF)/4;
			foundSetScissor = true;
			continue;
		}

		if( (w0>>24) == RDP_SETFILLCOLOR )
		{
			height   = ((w1>>0 )&0xFFF)/4;
			foundSetFillColor = true;
			newFillColor = w1;
			continue;
		}

		if( (w0>>24) == RDP_FILLRECT )
		{
			uint32 x0   = ((w1>>12)&0xFFF)/4;
			uint32 y0   = ((w1>>0 )&0xFFF)/4;
			uint32 x1   = ((w0>>12)&0xFFF)/4;
			uint32 y1   = ((w0>>0 )&0xFFF)/4;

			if( x0 == 0 && y0 == 0 )
			{
				if( x1 == g_CI.dwWidth )
				{
					height = y1;
					foundFillRect = true;
					continue;
				}

				if( x1 == g_CI.dwWidth - 1 )
				{
					height = y1+1;
					foundFillRect = true;
					continue;
				}
			}
		}	

		if( (w0>>24) == RDP_TEXRECT )
		{
			foundTxtRect = true;
			break;
		}

		if( (w0>>24) == RDP_SETCIMG )
		{
			foundSetCImg = true;
			break;
		}
	}

	/*
	bool foundSetScissor=false;
	bool foundFillRect=false;
	bool foundSetFillColor=false;
	bool foundSetCImg=false;
	bool foundTxtRect=false;
	int ucodeLength=10;
	uint32 newFillColor;
	*/

	if( foundFillRect )
	{
		if( foundSetFillColor )
		{
			if( newFillColor != 0xFFFCFFFC )
				return true;	// this is a texture buffer
			else
				return false;
		}

		if( gRDP.fillColor != 0x00FFFFF7 )
			return true;	// this is a texture buffer
		else
			return false;	// this is a normal ZImg
	}
	else
		return true;

	if( !foundSetCImg )
		return true;

	if( foundSetScissor )
		return true;
}

// Return -1 if the addr is not in recent frame buffer addr
// Return 1 if the addr is in the frame buffer which is currently being displayed
// Return 2 if the addr is in the 3rd frame buffer if we are using triple buffer
// this function will not return 0 which means the addr is within the current rendering frame buffer
//     this should not happen
extern BufferSettingInfo DirectXRenderBufferSettings[];
int IsAddrInRecentFrameBuffers(uint32 addr)
{
	if( addr >= g_uRecentCIInfoPtrs[1]->dwAddr && addr < g_uRecentCIInfoPtrs[1]->dwAddr+g_uRecentCIInfoPtrs[1]->dwMemSize )
		return 1;
	else if( DirectXRenderBufferSettings[options.RenderBufferSetting].number > 1 &&
		addr >= g_uRecentCIInfoPtrs[2]->dwAddr && addr < g_uRecentCIInfoPtrs[2]->dwAddr+g_uRecentCIInfoPtrs[1]->dwMemSize )
	{
		return 2;
	}
	else if( addr >= g_uRecentCIInfoPtrs[0]->dwAddr && addr < g_uRecentCIInfoPtrs[0]->dwAddr+g_uRecentCIInfoPtrs[0]->dwMemSize && status.bHandleN64TextureBuffer )
	{
		return 1;
	}
	{
		return -1;
	}
}

int CheckAndSaveBackBuffer(uint32 addr, uint32 memsize, bool copyToRDRAM = false);
int CheckAndSaveBackBuffer(uint32 addr, uint32 memsize, bool copyToRDRAM)
{
	if( CDeviceBuilder::GetGeneralDeviceType() != DIRECTX_DEVICE ) 
		return -1;

	int r = FindRecentCIInfoIndex(addr);
	if( r >= 0 && status.gDlistCount - g_uRecentCIInfoPtrs[r]->lastUsedFrame <= 3  && g_uRecentCIInfoPtrs[r]->bCopied == false )
	{
		CGraphicsContext::g_pGraphicsContext->SaveBackBuffer(r);
	}		

	return r;
}


uint8 CIFindIndex(uint16 val)
{
	for( int i=0; i<=0xFF; i++ )
	{
		if( val == g_wRDPTlut[i] )
		{
			return (uint8)i;
		}
	}
	return 0;
}



extern uint32 ComputeCImgHeight(SetImgInfo &info, uint32 &height);

uint32 dwOldCIAddr;
bool LastCIIsNewCI=false;
SetImgInfo g_LastCI = { TXT_FMT_RGBA, TXT_SIZE_16b, 1, 0 };
#define STORE_CI	{g_CI.dwAddr = dwNewAddr;g_CI.dwFormat = dwFmt;g_CI.dwSize = dwSiz;g_CI.dwWidth = dwWidth;g_CI.bpl=dwBpl;}
void DLParser_SetCImg(Gfx *gfx)
{
	uint32 dwFmt		= gfx->setimg.fmt;
	uint32 dwSiz		= gfx->setimg.siz;
	uint32 dwWidth		= gfx->setimg.width + 1;
	uint32 dwNewAddr	= RSPSegmentAddr((gfx->setimg.addr)) & 0x00FFFFFF ;
	uint32 dwBpl		= dwWidth << dwSiz >> 1;

	{
		if( dwFmt == TXT_FMT_YUV || dwFmt == TXT_FMT_IA )
		{
		}

	}

	if( g_CI.dwAddr == dwNewAddr && g_CI.dwFormat == dwFmt && g_CI.dwSize == dwSiz && g_CI.dwWidth == dwWidth )
	{
		return;
	}

	if( status.bVIOriginIsUpdated == true && currentRomOptions.screenUpdateSetting==SCREEN_UPDATE_AT_1ST_CI_CHANGE )
	{
		status.bVIOriginIsUpdated=false;
		CGraphicsContext::Get()->UpdateFrame();
	}

	if( !frameBufferOptions.bUpdateCIInfo )
	{
		dwOldCIAddr = g_CI.dwAddr;
		STORE_CI;
		status.bCIBufferIsRendered = false;
		status.bN64IsDrawingTextureBuffer = false;

		return;
	}

	if( dwNewAddr == g_ZI.dwAddr )
	{
		uint32 height;
		if( ComputeCImgHeight(g_CI, height) != RDP_SETSCISSOR )
		{
			// This is a render-to-texture buffer
			dwOldCIAddr = g_CI.dwAddr;
			STORE_CI;
			//UpdateRecentCIAddr(g_CI);
			status.bCIBufferIsRendered = false;
			status.leftRendered = status.topRendered = status.rightRendered = status.bottomRendered = -1;

			return;
		}

	}

	if( options.enableHackForGames == HACK_FOR_SUPER_BOWLING )
	{
		if( dwNewAddr%0x100 == 0 )
		{
			if( dwWidth < 320 )
			{
				// Left half screen
				gRDP.scissor.left = 0;
				gRDP.scissor.right = 160;
				CRender::g_pRender->SetViewport(0, 0, 160, 240, 0xFFFF);
				CRender::g_pRender->UpdateClipRectangle();
				CRender::g_pRender->UpdateScissor();
			}
			else
			{
				gRDP.scissor.left = 0;
				gRDP.scissor.right = 320;
				CRender::g_pRender->SetViewport(0, 0, 320, 240, 0xFFFF);
				CRender::g_pRender->UpdateClipRectangle();
				CRender::g_pRender->UpdateScissor();
			}
		}
		else
		{
			// right half screen
			gRDP.scissor.left = 160;
			gRDP.scissor.right = 320;
			gRSP.nVPLeftN = 160;
			gRSP.nVPRightN = 320;
			CRender::g_pRender->UpdateClipRectangle();
			CRender::g_pRender->UpdateScissor();
			CRender::g_pRender->SetViewport(160, 0, 320, 240, 0xFFFF);
		}
	}

	bool wasDrawingTextureBuffer = status.bN64IsDrawingTextureBuffer;
	status.bN64IsDrawingTextureBuffer = ( dwSiz != TXT_SIZE_16b || dwFmt != TXT_FMT_RGBA || dwWidth < 200 || ( !IsAddrUsedByVIorigin(dwNewAddr, dwWidth) && dwWidth != 512 && dwNewAddr != g_ZI.dwAddr) );
	status.bN64FrameBufferIsUsed = status.bN64IsDrawingTextureBuffer;

	if( !wasDrawingTextureBuffer && g_CI.dwAddr == g_ZI.dwAddr && status.bCIBufferIsRendered )
	{
		if( options.enableHackForGames != HACK_FOR_CONKER && g_uRecentCIInfoPtrs[0]->bCopied == false )
		{
			// Conker is not actually using a backbuffer
			//UpdateRecentCIAddr(g_CI);
			if( status.leftRendered != -1 && status.topRendered != -1 && status.rightRendered != -1 && status.bottomRendered != -1 )
			{
				RECT rect={status.leftRendered,status.topRendered,status.rightRendered,status.bottomRendered};
				CGraphicsContext::g_pGraphicsContext->SaveBackBuffer(0,&rect);
			}
			else
			{
				CGraphicsContext::g_pGraphicsContext->SaveBackBuffer(0,NULL);
			}
		}
	}

	frameBufferOptions.bFillRectNextTextureBuffer = false;
	if( g_CI.dwAddr == dwNewAddr && status.bHandleN64TextureBuffer && (g_CI.dwFormat != dwFmt || g_CI.dwSize != dwSiz || g_CI.dwWidth != dwWidth ) )
	{
		// Mario Tennis player shadow
		CGraphicsContext::g_pGraphicsContext->CloseTextureBuffer(true);
		if( options.enableHackForGames == HACK_FOR_MARIO_TENNIS )
			frameBufferOptions.bFillRectNextTextureBuffer = true;	// Hack for Mario Tennis
	}

	dwOldCIAddr = g_CI.dwAddr;
	STORE_CI;
	memcpy(&g_LastCI, &g_CI, sizeof(g_CI));

	if( g_CI.dwAddr == g_ZI.dwAddr && !status.bN64IsDrawingTextureBuffer )
	{
		if( IsDIATextureBuffer() )
		{
			status.bN64IsDrawingTextureBuffer = true;
			status.bN64FrameBufferIsUsed = status.bN64IsDrawingTextureBuffer;
		}
	}

	status.bCIBufferIsRendered = false;
	status.leftRendered = status.topRendered = status.rightRendered = status.bottomRendered = -1;

	if( currentRomOptions.screenUpdateSetting==SCREEN_UPDATE_AT_CI_CHANGE && !status.bN64IsDrawingTextureBuffer )
	{
		if( status.curRenderBuffer == NULL )
		{
			status.curRenderBuffer = g_CI.dwAddr;
		}
		else if( status.curRenderBuffer != g_CI.dwAddr )
		{
			status.curDisplayBuffer = status.curRenderBuffer;
			CGraphicsContext::Get()->UpdateFrame();
			status.curRenderBuffer = g_CI.dwAddr;
		}
	}

	if( frameBufferOptions.bAtEachFrameUpdate && !status.bHandleN64TextureBuffer )
	{
		if( status.curRenderBuffer != g_CI.dwAddr )
		{
			if( status.gDlistCount%(currentRomOptions.N64FrameBufferWriteBackControl+1) == 0 )
			{
				CDXGraphicsContext::g_pGraphicsContext->CopyBackToRDRAM(status.curRenderBuffer, 
					dwFmt, dwSiz, windowSetting.uViWidth, windowSetting.uViHeight,
					windowSetting.uDisplayWidth, windowSetting.uDisplayHeight);
			}
		}

		//status.curDisplayBuffer = status.curRenderBuffer;
		status.curRenderBuffer = g_CI.dwAddr;
	}


	switch( currentRomOptions.N64RenderToTextureEmuType )
	{
	case TXT_BUF_NONE:
		if( status.bHandleN64TextureBuffer )
			CGraphicsContext::g_pGraphicsContext->CloseTextureBuffer(false);
		status.bHandleN64TextureBuffer = false;	// Don't handle N64 texture buffer stuffs
		if( !status.bN64IsDrawingTextureBuffer )
			UpdateRecentCIAddr(g_CI);
		break;
	default:
		if( status.bHandleN64TextureBuffer )
		{
			CGraphicsContext::g_pGraphicsContext->CloseTextureBuffer(true);
		}

		status.bHandleN64TextureBuffer = status.bN64IsDrawingTextureBuffer;
		if( status.bHandleN64TextureBuffer )
		{
			if( options.enableHackForGames != HACK_FOR_BANJO_TOOIE )
			{
				CGraphicsContext::g_pGraphicsContext->SetTextureBuffer();
			}
		}
		else
		{
			//UpdateRecentCIAddr(g_CI);		// Delay this until the CI buffer is actally drawn
		}
		break;
	}

}


void TexRectToFrameBuffer_8b(uint32 dwXL, uint32 dwYL, uint32 dwXH, uint32 dwYH, float t0u0, float t0v0, float t0u1, float t0v1, uint32 dwTile)
{
	// Copy the texture into the N64 framebuffer memory
	// Used in Yoshi

	/*
	uint32 maxW = g_pTextureBufferInfo->CI_Info.dwWidth;
	uint32 maxH = maxW*3/4;
	if( status.dwTvSystem == TV_SYSTEM_PAL )
	{
		maxH = maxW*9/11;
	}
	*/

	uint32 maxW = g_pTextureBufferInfo->N64Width;
	uint32 maxH = g_pTextureBufferInfo->N64Height;

	uint32 maxOff = maxW*maxH;
	
	TMEMLoadMapInfo &info = g_tmemLoadAddrMap[gRDP.tiles[dwTile].dwTMem];
	uint32 dwWidth = dwXH-dwXL;
	uint32 dwHeight = dwYH-dwYL;

	float xScale = (t0u1-t0u0)/dwWidth;
	float yScale = (t0v1-t0v0)/dwHeight;

	uint8* dwSrc = g_pRDRAMu8 + info.dwLoadAddress;
	uint8* dwDst = g_pRDRAMu8 + g_pTextureBufferInfo->CI_Info.dwAddr;

	uint32 dwSrcPitch = gRDP.tiles[dwTile].dwPitch;
	uint32 dwDstPitch = g_pTextureBufferInfo->CI_Info.dwWidth;

	uint32 dwSrcOffX = gRDP.tiles[dwTile].hilite_sl;
	uint32 dwSrcOffY = gRDP.tiles[dwTile].hilite_tl;

	uint32 dwLeft = dwXL;
	uint32 dwTop = dwYL;

	dwWidth = min(dwWidth,maxW-dwLeft);
	dwHeight = min(dwHeight, maxH-dwTop);
	if( maxH <= dwTop )	return;

	for (uint32 y = 0; y < dwHeight; y++)
	{
		uint32 dwByteOffset = (uint32)(((y*yScale+dwSrcOffY) * dwSrcPitch) + dwSrcOffX);
		
		for (uint32 x = 0; x < dwWidth; x++)
		{
			if( (((y+dwTop)*dwDstPitch+x+dwLeft)^0x3) > maxOff )
			{
				continue;
			}
			dwDst[((y+dwTop)*dwDstPitch+x+dwLeft)^0x3] = dwSrc[(uint32)(dwByteOffset+x*xScale) ^ 0x3];
		}
	}
}

void TexRectToN64FrameBuffer_16b(uint32 x0, uint32 y0, uint32 width, uint32 height, uint32 dwTile)
{
	// Copy the texture into the N64 RDRAM framebuffer memory

	DrawInfo srcInfo;	
	if( g_textures[dwTile].m_pCTexture->StartUpdate(&srcInfo) == false )
	{
		return;
	}

	uint32 n64CIaddr = g_CI.dwAddr;
	uint32 n64CIwidth = g_CI.dwWidth;

	for (uint32 y = 0; y < height; y++)
	{
		uint32* pSrc = (uint32*)((uint8*)srcInfo.lpSurface + y * srcInfo.lPitch);
		uint16* pN64Buffer = (uint16*)(g_pRDRAMu8+(n64CIaddr&(g_dwRamSize-1)))+(y+y0)*n64CIwidth;
		
		for (uint32 x = 0; x < width; x++)
		{
			pN64Buffer[x+x0] = ConvertRGBATo555(pSrc[x]);
		}
	}

	g_textures[dwTile].m_pCTexture->EndUpdate(&srcInfo);
}

/*
uint32 CalculateRDRAMCRC(uint32 addr, uint32 width, uint32 height, uint32 size, uint32 pitch)
{
	uint32 crc=0;

	uint32 len = height*pitch*size;
	if( size == TXT_SIZE_4b ) len = (height*pitch)>>1;

	len >>= 2;

	uint32 *frameBufferBase = (uint32*)(g_pRDRAMu8+addr);


	for( uint32 i=0; i<len; i++ )
	{
		crc += frameBufferBase[i];
	}

	return crc;
}
*/

#define FAST_CRC_CHECKING_INC_X	13
#define FAST_CRC_CHECKING_INC_Y	11
#define FAST_CRC_MIN_Y_INC		2
#define FAST_CRC_MIN_X_INC		2
#define FAST_CRC_MAX_X_INC		7
#define FAST_CRC_MAX_Y_INC		3
extern uint32 dwAsmHeight;
extern uint32 dwAsmPitch;
extern uint32 dwAsmdwBytesPerLine;
extern uint32 dwAsmCRC;
extern uint8* pAsmStart;

uint32 CalculateRDRAMCRC(void *pPhysicalAddress, uint32 left, uint32 top, uint32 width, uint32 height, uint32 size, uint32 pitchInBytes )
{
	dwAsmCRC = 0;
	dwAsmdwBytesPerLine = ((width<<size)+1)/2;

	if( currentRomOptions.bFastTexCRC && (height>=32 || (dwAsmdwBytesPerLine>>2)>=16))
	{
		uint32 realWidthInDWORD = dwAsmdwBytesPerLine>>2;
		uint32 xinc = realWidthInDWORD / FAST_CRC_CHECKING_INC_X;	
		if( xinc < FAST_CRC_MIN_X_INC )
		{
			xinc = min(FAST_CRC_MIN_X_INC, width);
		}
		if( xinc > FAST_CRC_MAX_X_INC )
		{
			xinc = FAST_CRC_MAX_X_INC;
		}

		uint32 yinc = height / FAST_CRC_CHECKING_INC_Y;	
		if( yinc < FAST_CRC_MIN_Y_INC ) 
		{
			yinc = min(FAST_CRC_MIN_Y_INC, height);
		}
		if( yinc > FAST_CRC_MAX_Y_INC )
		{
			yinc = FAST_CRC_MAX_Y_INC;
		}

		uint32 pitch = pitchInBytes>>2;
		register uint32 *pStart = (uint32*)(pPhysicalAddress);
		pStart += (top * pitch) + (((left<<size)+1)>>3);

		/*
		uint32 x,y;
		for (y = 0; y < height; y+=yinc)		// Do every nth line?
		{
		for (x = 0; x < realWidthInDWORD; x+=xinc)
		{
		dwAsmCRC += *(pStart+x);
		dwAsmCRC ^= x;
		}
		pStart += pitch;
		dwAsmCRC ^= y;
		}
		*/


		__asm
		{
			push	esi;
			mov		esi, DWORD PTR [xinc]; 
			mov		ebx, DWORD PTR [pStart];
			mov		eax,0;	// EAX = the CRC
			mov		edx,0x0;
loop1:
			cmp		edx, height;
			jae		endloop1;
			mov		ecx, 0x0;
loop2:
			add		eax, ecx;
			cmp		ecx, DWORD PTR [realWidthInDWORD]
			jae		endloop2;

			add		eax, DWORD PTR [ebx][ecx*4];

			add		ecx, esi;
			jmp		loop2;
endloop2:
			xor		eax, edx;
			add		edx, DWORD PTR [yinc];
			add		ebx, DWORD PTR [pitch];
			jmp		loop1;
endloop1:
			mov		DWORD PTR [dwAsmCRC], eax;
			pop		esi;
		}
	}
	else
	{
		try{
			dwAsmdwBytesPerLine = ((width<<size)+1)/2;

			pAsmStart = (uint8*)(pPhysicalAddress);
			pAsmStart += (top * pitchInBytes) + (((left<<size)+1)>>1);

			dwAsmHeight = height - 1;
			dwAsmPitch = pitchInBytes - 1;
			__asm 
			{
				push eax
					push ebx
					push ecx
					push edx
					push esi

					mov  ecx, pAsmStart;	// = pStart
				mov  edx, 0			// The CRC
					mov  eax, dwAsmHeight	// = y
l2:	mov	 ebx, dwAsmdwBytesPerLine	// = x
l1:	mov esi, [ecx+ebx]
				xor esi, ebx
					xor esi, eax
					add edx, esi
					sub	ebx, 4
					jge l1
					add ecx, dwAsmPitch
					dec eax
					jge l2

					mov	dwAsmCRC, edx

					pop esi
					pop edx
					pop ecx
					pop ebx
					pop	eax
			}

			/*
			uint32 x,y;
			for (y = 0; y < height; y++)		// Do every nth line?
			{
			// Byte fiddling won't work, but this probably doesn't matter
			// Now process 4 bytes at a time
			for (x = 0; x < dwBytesPerLine; x+=4)
			{
			dwCRC += (((*(uint32*)(pStart+x))^y)^x);
			//dwCRC += x;
			}
			pStart += pitchInBytes;
			}
			*/
		}
		catch(...)
		{
		}
	}
	return dwAsmCRC;
}


bool FrameBufferInRDRAMCheckCRC()
{
	RecentCIInfo &p = *(g_uRecentCIInfoPtrs[0]);
	uint8 *pFrameBufferBase = (uint8*)(g_pRDRAMu8+p.dwAddr);
	uint32 pitch = (p.dwWidth << p.dwSize ) >> 1;
	uint32 crc = CalculateRDRAMCRC(pFrameBufferBase, 0, 0, p.dwWidth, p.dwHeight, p.dwSize, pitch);
	if( crc != p.dwCRC )
	{
		p.dwCRC = crc;
		return false;
	}
	else
	{
		return true;
	}
}

extern std::vector<uint32> frameWriteRecord;
void FrameBufferWriteByCPU(uint32 addr, uint32 size)
{
	if( !frameBufferOptions.bProcessCPUWrite )	return;
	status.frameWriteByCPU = TRUE;
	frameWriteRecord.push_back(addr&(g_dwRamSize-1));
}

extern RECT frameWriteByCPURect;
extern std::vector<RECT> frameWriteByCPURects;
extern RECT frameWriteByCPURectArray[20][20];
extern bool frameWriteByCPURectFlag[20][20];
#define FRAMEBUFFER_IN_BLOCK
bool ProcessFrameWriteRecord()
{
	int size = frameWriteRecord.size();
	if( size == 0 ) return false;

	int index = FindRecentCIInfoIndex(frameWriteRecord[0]);
	if( index == -1 )
	{
		frameWriteRecord.clear();
		return false;
	}
	else
	{
		uint32 base = g_uRecentCIInfoPtrs[index]->dwAddr;
		uint32 uwidth = g_uRecentCIInfoPtrs[index]->dwWidth;
		uint32 uheight = g_uRecentCIInfoPtrs[index]->dwHeight;
		uint32 upitch = uwidth<<1;

		frameWriteByCPURect.left=uwidth-1;
		frameWriteByCPURect.top = uheight-1;

		frameWriteByCPURect.right=0;
		frameWriteByCPURect.bottom = 0;

		int x, y, off;

		for( int i=0; i<size; i++ )
		{
			off = frameWriteRecord[i]-base;
			if( off < (int)g_uRecentCIInfoPtrs[index]->dwMemSize )
			{
				y = off/upitch;
				x = (off - y*upitch)>>1;

#ifdef FRAMEBUFFER_IN_BLOCK
				int xidx=x/32;
				int yidx=y/24;

				RECT &rect = frameWriteByCPURectArray[xidx][yidx];

				if( !frameWriteByCPURectFlag[xidx][yidx] )
				{
					rect.left=rect.right=x;
					rect.top=rect.bottom=y;
					frameWriteByCPURectFlag[xidx][yidx]=true;
				}
				else
				{
					if( x < rect.left )	rect.left = x;
					if( x > rect.right ) rect.right = x;
					if( y < rect.top )	rect.top = y;
					if( y > rect.bottom ) rect.bottom = y;
				}
#else

				/*
				int index = -1;
				int rectsize = frameWriteByCPURects.size();

				if( rectsize == 0 )
				{
					RECT rect;
					rect.left=rect.right=x;
					rect.top=rect.bottom=y;
					frameWriteByCPURects.push_back(rect);
					continue;
				}

				for( int j=0; j<rectsize; j++ )
				{
					if( ( (x >= frameWriteByCPURects[j].left && (x<=frameWriteByCPURects[j].right || x-frameWriteByCPURects[j].left<=30)) ||
						  (x < frameWriteByCPURects[j].left && frameWriteByCPURects[j].right-x <= 30) ) &&
						( (y >= frameWriteByCPURects[j].top && (x<=frameWriteByCPURects[j].bottom || x-frameWriteByCPURects[j].top<=30)) ||
						  (y < frameWriteByCPURects[j].top && frameWriteByCPURects[j].bottom-y <= 30) ) )
					{
						index = j;
						break;
					}
				}

				if( index < 0 )
				{
					RECT rect;
					rect.left=rect.right=x;
					rect.top=rect.bottom=y;
					frameWriteByCPURects.push_back(rect);
					continue;
				}

				RECT &rect = frameWriteByCPURects[index];
				if( x < rect.left )	rect.left = x;
				if( x > rect.right ) rect.right = x;
				if( y < rect.top )	rect.top = y;
				if( y > rect.bottom ) rect.bottom = y;
				*/

				if( x < frameWriteByCPURect.left )	frameWriteByCPURect.left = x;
				if( x > frameWriteByCPURect.right ) frameWriteByCPURect.right = x;
				if( y < frameWriteByCPURect.top )	frameWriteByCPURect.top = y;
				if( y > frameWriteByCPURect.bottom ) frameWriteByCPURect.bottom = y;
#endif
			}
		}

		frameWriteRecord.clear();
		return true;
	}
}

void FrameBufferReadByCPU( uint32 addr )
{
	///return;	// it does not work very well anyway

	if( !frameBufferOptions.bProcessCPURead )	return;
	
	addr &= (g_dwRamSize-1);
	int index = FindRecentCIInfoIndex(addr);
	if( index == -1 ) 
	{
		// Check if this is the depth buffer
		uint32 size = 2*g_RecentCIInfo[0].dwWidth*g_RecentCIInfo[0].dwHeight;
		addr &= 0x3FFFFFFF;

		if( addr >= g_ZI.dwAddr && addr < g_ZI.dwAddr + size )
		{
		}
		else
		{
			return;
		}
	}

	if( status.gDlistCount - g_uRecentCIInfoPtrs[index]->lastUsedFrame > 3 )
	{
		// Ok, we don't have this frame anymore
		return;
	}

	if( g_uRecentCIInfoPtrs[index]->bCopied )	return;

	//if( status.frameReadByCPU == FALSE )
	{
		uint32 size = 0x1000 - addr%0x1000;
		CheckAndSaveBackBuffer(addr, size, true);

	}
}