/*

  Copyright (C) 2003 Rice1964

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

#include "stdafx.h"

TextureBufferInfo gTextureBufferInfos[20];
const int numOfTxtBufInfos = sizeof(gTextureBufferInfos)/sizeof(TextureBufferInfo);
TextureBufferInfo *g_pTextureBufferInfo = NULL;

CGraphicsContext* CGraphicsContext::g_pGraphicsContext = NULL;
bool CGraphicsContext::m_deviceCapsIsInitialized = false;
bool CGraphicsContext::needCleanScene = false;
int	CGraphicsContext::m_maxFSAA = 16;
int	CGraphicsContext::m_maxAnisotropy = 16;
UINT CGraphicsContext::m_FullScreenRefreshRates[40] = {	0, 50, 55, 60, 65, 70, 72, 75, 80, 85, 90, 95, 100, 110, 120};
int CGraphicsContext::m_FullScreenResolutions[40][2] = {
	{320,200}, {400,300}, {480,360}, {512,384}, {640,480}, 
	{800,600}, {1024,768}, {1152,864}, {1280,960}, 
	{1400,1050}, {1600,1200}, {1920,1440}, {2048,1536}};
int CGraphicsContext::m_numOfResolutions = 0;
UINT CGraphicsContext::m_ColorBufferDepths[4] = {16, 32, 0, 0};

CGraphicsContext * CGraphicsContext::Get(void)
{	
	return CGraphicsContext::g_pGraphicsContext;
}
	
CGraphicsContext::CGraphicsContext() :
	m_bReady(false), 
	m_bActive(false),
	m_bWindowed(true),
	m_supportTextureMirror(false),
	m_backBufferIsSaved(true),
	m_isRenderingToTexture(false),
	m_lastTextureBufferIndex(-1),
	m_curBackBufferIndex(-1),
	m_curTextureBufferIndex(-1)
{
	memset(&m_textureColorBufferInfo,0,sizeof(TextureBufferShortInfo));
	memset(&m_textureDepthBufferInfo,0,sizeof(TextureBufferShortInfo));
}
CGraphicsContext::~CGraphicsContext()
{
	for( int i=0; i<numOfTxtBufInfos; i++ )
	{
		SAFE_DELETE(gTextureBufferInfos[i].pTxtBuffer);
	}
}

HWND		CGraphicsContext::m_hWnd=NULL;
HWND		CGraphicsContext::m_hWndStatus=NULL;
HWND		CGraphicsContext::m_hWndToolbar=NULL;
HMENU		CGraphicsContext::m_hMenu=NULL;
uint32		CGraphicsContext::m_dwWindowStyle=0;     // Saved window style for mode switches
uint32		CGraphicsContext::m_dwWindowExStyle=0;   // Saved window style for mode switches
uint32		CGraphicsContext::m_dwStatusWindowStyle=0;     // Saved window style for mode switches

BOOL CALLBACK MyEnumChildProc(HWND hwnd, LPARAM lParam)
{
	int id = GetDlgCtrlID(hwnd);
	if( id != 0 )
	{
		if( CGraphicsContext::m_hWndStatus != hwnd && IsWindow( hwnd ) )
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			if( rect.left == 0 && rect.top == 0 && rect.bottom < 40 && rect.right > 320 )
			{
				CGraphicsContext::m_hWndToolbar = hwnd;
			}
		}
	}

	return TRUE;
}

void CGraphicsContext::InitWindowInfo()
{
	m_hWnd = g_GraphicsInfo.hWnd;
	m_hWndStatus = g_GraphicsInfo.hStatusBar;
	m_hWndToolbar = NULL;
	EnumChildWindows(m_hWnd, MyEnumChildProc, 0);	// To find toolbar

	m_hMenu = GetMenu(m_hWnd);

	// Save window properties
	m_dwWindowStyle = GetWindowLong( m_hWnd, GWL_STYLE );
	m_dwWindowExStyle = GetWindowLong( m_hWnd, GWL_EXSTYLE );
	m_dwStatusWindowStyle = GetWindowLong( m_hWndStatus, GWL_STYLE );

	RECT rcStatus;

	// Add extra margin for the status bar
	windowSetting.statusBarHeight = 0;
	if ( IsWindow( m_hWndStatus ) )
	{
		// Add on enough space for the status bar
		GetClientRect(m_hWndStatus, &rcStatus);
		windowSetting.statusBarHeight = (rcStatus.bottom - rcStatus.top);
	}

	windowSetting.toolbarHeight = 0;
	if( IsWindow( m_hWndToolbar ) )
	{
		GetClientRect(m_hWndToolbar, &rcStatus);
		windowSetting.toolbarHeight = (rcStatus.bottom - rcStatus.top);
	}
}


bool CGraphicsContext::Initialize(HWND hWnd, HWND hWndStatus, uint32 dwWidth, uint32 dwHeight, BOOL bWindowed )
{
	if( windowSetting.bDisplayFullscreen )
	{
		windowSetting.uDisplayWidth = windowSetting.uFullScreenDisplayWidth;
		windowSetting.uDisplayHeight = windowSetting.uFullScreenDisplayHeight;
	}
	else
	{
		int maxidx = CGraphicsContext::m_numOfResolutions - 1;
		if( CGraphicsContext::m_FullScreenResolutions[maxidx][0] <= windowSetting.uWindowDisplayWidth ||
			CGraphicsContext::m_FullScreenResolutions[maxidx][1] <= windowSetting.uWindowDisplayHeight )
		{
			windowSetting.uWindowDisplayWidth = 640;
			windowSetting.uWindowDisplayHeight = 480;
		}

		windowSetting.uDisplayWidth = windowSetting.uWindowDisplayWidth;
		windowSetting.uDisplayHeight= windowSetting.uWindowDisplayHeight;
	}
	

	RECT rcScreen;
	SetRect(&rcScreen, 0,0, windowSetting.uDisplayWidth, windowSetting.uDisplayHeight);
	rcScreen.bottom += windowSetting.statusBarHeight;
	rcScreen.bottom += windowSetting.toolbarHeight;

	// Calculate window size to give desired window size...
	AdjustWindowRectEx(&rcScreen, m_dwWindowStyle & (~(WS_THICKFRAME|WS_MAXIMIZEBOX)), TRUE, m_dwWindowExStyle);
	SetWindowPos(m_hWnd, 0, rcScreen.left, rcScreen.top, rcScreen.right-rcScreen.left, rcScreen.bottom-rcScreen.top,
		SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);

	GetWindowRect( m_hWnd, &m_rcWindowBounds );

    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &m_DMsaved);

	if( !m_bWindowed )
	{
		ShowCursor( FALSE );
	}
	else
	{
		ShowCursor( TRUE );
	}


	memset(&gTextureBufferInfos, 0, sizeof(TextureBufferInfo)*numOfTxtBufInfos);

	return true;
}

void CGraphicsContext::CleanUp()
{
    m_bActive = false;
    m_bReady  = false;

	if ( IsWindow( m_hWnd ) )
	{
		SetWindowLong( m_hWnd, GWL_STYLE, m_dwWindowStyle );
	}

	if ( IsWindow(m_hWndStatus) )
	{
		SetWindowLong( m_hWndStatus, GWL_STYLE, m_dwStatusWindowStyle);
	}
}

bool FrameBufferInRDRAMCheckCRC();
bool ProcessFrameWriteRecord();
extern RECT frameWriteByCPURect;
extern std::vector<RECT> frameWriteByCPURects;
extern RECT frameWriteByCPURectArray[20][20];
extern bool frameWriteByCPURectFlag[20][20];
#define FRAMEBUFFER_IN_BLOCK

void CGraphicsContext::UpdateFrameBufferBeforeUpdateFrame()
{
	if( (frameBufferOptions.bProcessCPUWrite && status.frameWriteByCPU ) ||
		(frameBufferOptions.bLoadBackBufFromRDRAM && !FrameBufferInRDRAMCheckCRC() ) )		
		// Checks if frame buffer has been modified by CPU
		// Only happens to Dr. Mario
	{
		if( frameBufferOptions.bProcessCPUWrite )
		{
			if( ProcessFrameWriteRecord() )
			{
#ifdef FRAMEBUFFER_IN_BLOCK
				int i,j;
				for( i=0; i<20; i++)
				{
					for( j=0; j<20; j++ )
					{
						if( frameWriteByCPURectFlag[i][j] )
						{
							CRender::GetRender()->DrawFrameBuffer(false, frameWriteByCPURectArray[i][j].left, frameWriteByCPURectArray[i][j].top,
								frameWriteByCPURectArray[i][j].right-frameWriteByCPURectArray[i][j].left+1, frameWriteByCPURectArray[i][j].bottom-frameWriteByCPURectArray[i][j].top+1);
						}
					}
				}
				for( i=0; i<20; i++)
				{
					for( j=0; j<20; j++ )
					{
						if( frameWriteByCPURectFlag[i][j] )
						{
							ClearFrameBufferToBlack(frameWriteByCPURectArray[i][j].left, frameWriteByCPURectArray[i][j].top,
								frameWriteByCPURectArray[i][j].right-frameWriteByCPURectArray[i][j].left+1, frameWriteByCPURectArray[i][j].bottom-frameWriteByCPURectArray[i][j].top+1);
							frameWriteByCPURectFlag[i][j] = false;
						}
					}
				}
				//memset(frameWriteByCPURectArray, 0, sizeof(frameWriteByCPURectArray));
				//memset(frameWriteByCPURectFlag, 0, sizeof(frameWriteByCPURectFlag));
#else
				CRender::GetRender()->DrawFrameBuffer(false, frameWriteByCPURect.left, frameWriteByCPURect.top,
					frameWriteByCPURect.right-frameWriteByCPURect.left, frameWriteByCPURect.bottom-frameWriteByCPURect.top);
				ClearFrameBufferToBlack(frameWriteByCPURect.left, frameWriteByCPURect.top,
					frameWriteByCPURect.right-frameWriteByCPURect.left+1, frameWriteByCPURect.bottom-frameWriteByCPURect.top+1);

				/*
				int size = frameWriteByCPURects.size();
				for( int i=0; i<size; i++)
				{
					CRender::GetRender()->DrawFrameBuffer(false, frameWriteByCPURects[i].left, frameWriteByCPURects[i].top,
						frameWriteByCPURects[i].right-frameWriteByCPURects[i].left, frameWriteByCPURects[i].bottom-frameWriteByCPURects[i].top);
					ClearFrameBufferToBlack(frameWriteByCPURects[i].left, frameWriteByCPURects[i].top,
						frameWriteByCPURects[i].right-frameWriteByCPURects[i].left+1, frameWriteByCPURects[i].bottom-frameWriteByCPURects[i].top+1);
				}
				frameWriteByCPURects.clear();
				*/
#endif
			}
			status.frameWriteByCPU = FALSE;
		}
		else
		{
			extern RecentCIInfo *g_uRecentCIInfoPtrs[3];
			RecentCIInfo &p = *(g_uRecentCIInfoPtrs[0]);
			CRender::GetRender()->DrawFrameBuffer(false, 0,0,p.dwWidth,p.dwHeight);
			ClearFrameBufferToBlack();
		}
	}
}

uint32 ComputeCImgHeight(SetImgInfo &info, uint32 &height)
{
	uint32 dwPC = gDlistStack[gDlistStackPointer].pc;		// This points to the next instruction

	for( int i=0; i<10; i++ )
	{
		uint32 w0 = *(uint32 *)(g_pRDRAMu8 + dwPC + i*8);
		uint32 w1 = *(uint32 *)(g_pRDRAMu8 + dwPC + 4 + i*8);

		if( (w0>>24) == RDP_SETSCISSOR )
		{
			height   = ((w1>>0 )&0xFFF)/4;
			return RDP_SETSCISSOR;
		}

		if( (w0>>24) == RDP_FILLRECT )
		{
			uint32 x0   = ((w1>>12)&0xFFF)/4;
			uint32 y0   = ((w1>>0 )&0xFFF)/4;
			uint32 x1   = ((w0>>12)&0xFFF)/4;
			uint32 y1   = ((w0>>0 )&0xFFF)/4;

			if( x0 == 0 && y0 == 0 )
			{
				if( x1 == info.dwWidth )
				{
					height = y1;
					return RDP_FILLRECT;
				}

				if( x1 == info.dwWidth - 1 )
				{
					height = y1+1;
					return RDP_FILLRECT;
				}
			}
		}	

		if( (w0>>24) == RDP_SETCIMG )
		{
			goto step2;
		}

		if( (w0>>24) == RDP_SETCIMG )
		{
			goto step2;
		}
	}

	if( gRDP.scissor.left == 0 && gRDP.scissor.top == 0 && gRDP.scissor.right == info.dwWidth )
	{
		height = gRDP.scissor.bottom;
		return RDP_SETSCISSOR+1;
	}

step2:

	height = info.dwWidth*3/4;
	if( status.dwTvSystem == TV_SYSTEM_PAL )
	{
		height = info.dwWidth*9/11;
	}

	if( gRDP.scissor.bottom < (int)height && gRDP.scissor.bottom != 0 )
	{
		height = gRDP.scissor.bottom;
	}

	if( info.dwAddr + height*info.dwWidth*info.dwSize >= g_dwRamSize )
	{
		height = info.dwWidth*3/4;
		if( status.dwTvSystem == TV_SYSTEM_PAL )
		{
			height = info.dwWidth*9/11;
		}

		if( gRDP.scissor.bottom < (int)height && gRDP.scissor.bottom != 0 )
		{
			height = gRDP.scissor.bottom;
		}

		if( info.dwAddr + height*info.dwWidth*info.dwSize >= g_dwRamSize )
		{
			height = ( g_dwRamSize - info.dwAddr ) / info.dwWidth;
		}
	}

	return 0;
}

int CGraphicsContext::CheckTxtrBufsWithNewCI(SetImgInfo &CIinfo, uint32 height, bool byNewTxtrBuf)
{
	int matchidx = -1;
	uint32 memsize = ((height*CIinfo.dwWidth)>>1)<<CIinfo.dwSize;

	for( int i=0; i<numOfTxtBufInfos; i++ )
	{
		TextureBufferInfo &info = gTextureBufferInfos[i];
		if( !info.isUsed )	continue;

		bool covered = false;

		if( info.CI_Info.dwAddr == CIinfo.dwAddr )
		{
			if( info.CI_Info.dwSize == CIinfo.dwSize &&
				info.CI_Info.dwWidth == CIinfo.dwWidth &&
				info.CI_Info.dwFormat == CIinfo.dwFormat &&
				info.N64Height == height )
			{
				// This is the same texture at the same address
				if( byNewTxtrBuf )
				{
					matchidx = i;
					break;
				}
			}

			// At the same address, but not the same size
			//SAFE_DELETE(info.psurf);
			covered = true;
		}

		if( !covered )
		{
			uint32 memsize2 = ((info.N64Height*info.N64Width)>>1)<<info.CI_Info.dwSize;

			if( info.CI_Info.dwAddr > CIinfo.dwAddr && info.CI_Info.dwAddr < CIinfo.dwAddr + memsize)
				covered = true;
			else if( info.CI_Info.dwAddr+memsize2 > CIinfo.dwAddr && info.CI_Info.dwAddr+memsize2 < CIinfo.dwAddr + memsize)
				covered = true;
			else if( CIinfo.dwAddr > info.CI_Info.dwAddr && CIinfo.dwAddr < info.CI_Info.dwAddr + memsize2 )
				covered = true;
			else if( CIinfo.dwAddr+ memsize > info.CI_Info.dwAddr && CIinfo.dwAddr+ memsize < info.CI_Info.dwAddr + memsize2 )
				covered = true;
		}

		if( covered )
		{
			//SAFE_DELETE(info.psurf);
			info.isUsed = false;
			SAFE_DELETE(info.pTxtBuffer);
			info.txtEntry.pTexture = NULL;
			continue;
		}
	}

	return matchidx;
}

extern RecentCIInfo *g_uRecentCIInfoPtrs[5];
TextureBufferInfo newTextureBufInfo;

int FindASlot(void)
{
	int idx;

	// Find an empty slot
	bool found = false;
	for( int i=0; i<numOfTxtBufInfos; i++ )
	{
		if( !gTextureBufferInfos[i].isUsed )
		{
			found = true;
			idx = i;
			break;
		}
	}

	// If cannot find an empty slot, find the oldest slot and reuse the slot
	if( !found )
	{
		uint32 oldestCount=0xFFFFFFFF;
		uint32 oldestIdx = 0;
		for( int i=0; i<numOfTxtBufInfos; i++ )
		{
			if( gTextureBufferInfos[i].updateAtUcodeCount < oldestCount )
			{
				oldestCount = gTextureBufferInfos[i].updateAtUcodeCount;
				oldestIdx = i;
			}
		}

		idx = oldestIdx;
	}

	SAFE_DELETE(gTextureBufferInfos[idx].pTxtBuffer) ;

	return idx;
}


void CGraphicsContext::SetTextureBuffer(void)
{
	memcpy(&(newTextureBufInfo.CI_Info), &g_CI, sizeof(SetImgInfo));

	newTextureBufInfo.N64Width = newTextureBufInfo.CI_Info.dwWidth;
	newTextureBufInfo.knownHeight = ComputeCImgHeight(g_CI, newTextureBufInfo.N64Height);

	status.bHandleN64TextureBuffer = true;
	newTextureBufInfo.maxUsedHeight = 0;

	if( defaultRomOptions.bInN64Resolution )
	{
		newTextureBufInfo.bufferWidth = newTextureBufInfo.N64Width;
		newTextureBufInfo.bufferHeight = newTextureBufInfo.N64Height;
	}
	else if( defaultRomOptions.bDoubleSizeForSmallTxtrBuf && newTextureBufInfo.N64Width<=128 && newTextureBufInfo.N64Height<=128)
	{
		newTextureBufInfo.bufferWidth = newTextureBufInfo.N64Width*2;
		newTextureBufInfo.bufferHeight = newTextureBufInfo.N64Height*2;
	}
	else
	{
		newTextureBufInfo.bufferWidth = newTextureBufInfo.N64Width;
		newTextureBufInfo.bufferHeight = newTextureBufInfo.N64Height;
	}

	newTextureBufInfo.scaleX = newTextureBufInfo.bufferWidth / float(newTextureBufInfo.N64Width);
	newTextureBufInfo.scaleY = newTextureBufInfo.bufferHeight / float(newTextureBufInfo.N64Height);

	status.bFrameBufferIsDrawn = false;
	status.bFrameBufferDrawnByTriangles = false;

	newTextureBufInfo.updateAtFrame = status.gDlistCount;
	newTextureBufInfo.updateAtUcodeCount = status.gUcodeCount;

	// Delay activation of the texture buffer until the 1st rendering

}

int CGraphicsContext::SetBackBufferAsTextureBuffer(SetImgInfo &CIinfo, int ciInfoIdx)
{
	TextureUsage usage = AS_BACK_BUFFER_SAVE;
	TextureBufferInfo tempTxtrBufInfo;

	memcpy(&(tempTxtrBufInfo.CI_Info), &CIinfo, sizeof(SetImgInfo));

	tempTxtrBufInfo.N64Width = g_uRecentCIInfoPtrs[ciInfoIdx]->dwLastWidth;
	tempTxtrBufInfo.N64Height = g_uRecentCIInfoPtrs[ciInfoIdx]->dwLastHeight;
	tempTxtrBufInfo.knownHeight = true;
	tempTxtrBufInfo.maxUsedHeight = 0;

	tempTxtrBufInfo.bufferWidth = windowSetting.uDisplayWidth;
	tempTxtrBufInfo.bufferHeight = windowSetting.uDisplayHeight;

	tempTxtrBufInfo.scaleX = tempTxtrBufInfo.bufferWidth / float(tempTxtrBufInfo.N64Width);
	tempTxtrBufInfo.scaleY = tempTxtrBufInfo.bufferHeight / float(tempTxtrBufInfo.N64Height);

	status.bFrameBufferIsDrawn = false;
	status.bFrameBufferDrawnByTriangles = false;

	tempTxtrBufInfo.updateAtFrame = status.gDlistCount;
	tempTxtrBufInfo.updateAtUcodeCount = status.gUcodeCount;

	// Checking against previous texture buffer infos
	int matchidx = -1;

	uint32 memsize = ((tempTxtrBufInfo.N64Height*tempTxtrBufInfo.N64Width)>>1)<<tempTxtrBufInfo.CI_Info.dwSize;

	matchidx = CheckTxtrBufsWithNewCI(CIinfo,tempTxtrBufInfo.N64Height,false);

	int idxToUse=-1;
	if( matchidx >= 0 )
	{
		// Reuse the matched slot
		idxToUse = matchidx;
		if( gTextureBufferInfos[matchidx].pTxtBuffer == NULL )
		{
			gTextureBufferInfos[matchidx].pTxtBuffer = 
				new CDXTextureBuffer(tempTxtrBufInfo.bufferWidth, tempTxtrBufInfo.bufferHeight, &gTextureBufferInfos[matchidx], usage);
		}
	}
	else
	{
		idxToUse = FindASlot();

		// After get the slot, create a new texture buffer and assign it to this slot
		gTextureBufferInfos[idxToUse].pTxtBuffer = 
			new CDXTextureBuffer(tempTxtrBufInfo.bufferWidth, tempTxtrBufInfo.bufferHeight, &gTextureBufferInfos[idxToUse], usage); 
	}

	// Need to set all variables for gTextureBufferInfos[idxToUse]
	CTextureBuffer *pTxtBuffer = gTextureBufferInfos[idxToUse].pTxtBuffer;
	memcpy(&gTextureBufferInfos[idxToUse], &tempTxtrBufInfo, sizeof(TextureBufferInfo) );
	gTextureBufferInfos[idxToUse].pTxtBuffer = pTxtBuffer;
	gTextureBufferInfos[idxToUse].isUsed = true;
	gTextureBufferInfos[idxToUse].txtEntry.pTexture = pTxtBuffer->m_pTexture;
	gTextureBufferInfos[idxToUse].txtEntry.txtrBufIdx = idxToUse+1;

	return idxToUse;
}

void CGraphicsContext::CloseTextureBuffer(bool toSave)
{
	status.bHandleN64TextureBuffer = false;
	if( status.bDirectWriteIntoRDRAM )
	{
	}
	else 
	{
		if( !toSave || !status.bFrameBufferIsDrawn || !status.bFrameBufferDrawnByTriangles )
		{
			RestoreNormalBackBuffer();

		}
		else
		{
			RestoreNormalBackBuffer();
			StoreTextureBufferToRDRAM();
		}
		//g_pTextureBufferInfo->pTxtBuffer->m_pTexture->RestoreAlphaChannel();

		g_pTextureBufferInfo->crcInRDRAM = ComputeTextureBufferCRCInRDRAM(m_curTextureBufferIndex);
		g_pTextureBufferInfo->crcCheckedAtFrame = status.gDlistCount;
	}

	SetScreenMult(windowSetting.uDisplayWidth/windowSetting.fViWidth, windowSetting.uDisplayHeight/windowSetting.fViHeight);
	CRender::g_pRender->UpdateClipRectangle();
	CRender::g_pRender->ApplyScissorWithClipRatio();

}

int FindRecentCIInfoIndex (uint32 addr);

void CGraphicsContext::ClearFrameBufferToBlack(uint32 left, uint32 top, uint32 width, uint32 height)
{
	RecentCIInfo &p = *(g_uRecentCIInfoPtrs[0]);
	uint16 *frameBufferBase = (uint16*)(g_pRDRAMu8+p.dwAddr);
	uint32 pitch = p.dwWidth;

	if( width == 0 || height == 0 )
	{
		uint32 len = p.dwHeight*p.dwWidth*p.dwSize;
		if( p.dwSize == TXT_SIZE_4b ) len = (p.dwHeight*p.dwWidth)>>1;
		memset(frameBufferBase, 0, len);
	}
	else
	{
		for( uint32 y=0; y<height; y++)
		{
			for( uint32 x=0; x<width; x++ )
			{
				*(frameBufferBase+(y+top)*pitch+x+left) = 0;
			}
		}
	}
}

uint8 RevTlutTable[0x10000];
bool RevTlutTableNeedUpdate = false;
void InitTlutReverseLookup(void)
{
	if( RevTlutTableNeedUpdate )
	{
		memset(RevTlutTable, 0, 0x10000);
		for( int i=0; i<=0xFF; i++ )
		{
			RevTlutTable[g_wRDPTlut[i]] = uint8(i);
		}

		RevTlutTableNeedUpdate = false;
	}
}


void CGraphicsContext::CopyBackToFrameBufferIfReadByCPU(uint32 addr)
{
	int i = FindRecentCIInfoIndex(addr);
	if( i != -1 )
	{
		//if( i == 0 ) CGraphicsContext::Get()->UpdateFrame();
		RecentCIInfo *info = g_uRecentCIInfoPtrs[i];
		CopyBackToRDRAM( info->dwAddr, info->dwFormat, info->dwSize, info->dwWidth, info->dwHeight, 
			windowSetting.uDisplayWidth, windowSetting.uDisplayHeight, addr, 0x1000-addr%0x1000);
	}
}
void CGraphicsContext::CheckTxtrBufsCRCInRDRAM(void)
{
	for( int i=0; i<numOfTxtBufInfos; i++ )
	{
		if( !gTextureBufferInfos[i].isUsed )	
			continue;

		if( gTextureBufferInfos[i].pTxtBuffer->IsBeingRendered() )
			continue;

		if( gTextureBufferInfos[i].crcCheckedAtFrame < status.gDlistCount )
		{
			uint32 crc = ComputeTextureBufferCRCInRDRAM(i);
			if( gTextureBufferInfos[i].crcInRDRAM != crc )
			{
				// RDRAM has been modified by CPU core
				SAFE_DELETE(gTextureBufferInfos[i].pTxtBuffer);
				gTextureBufferInfos[i].isUsed = false;
				continue;
			}
			else
			{
				gTextureBufferInfos[i].crcCheckedAtFrame = status.gDlistCount;
			}
		}
	}
}

int	CGraphicsContext::CheckAddrInTxtrBufs(TxtrInfo &ti)
{
	for( int i=0; i<numOfTxtBufInfos; i++ )
	{
		if( !gTextureBufferInfos[i].isUsed )	
			continue;

		if( gTextureBufferInfos[i].pTxtBuffer->IsBeingRendered() )
			continue;

		uint32 bufHeight = gTextureBufferInfos[i].knownHeight ? gTextureBufferInfos[i].N64Height : gTextureBufferInfos[i].maxUsedHeight;
		uint32 bufMemSize = gTextureBufferInfos[i].CI_Info.dwSize*gTextureBufferInfos[i].N64Width*bufHeight;
		if( ti.Address >=gTextureBufferInfos[i].CI_Info.dwAddr && ti.Address < gTextureBufferInfos[i].CI_Info.dwAddr+bufMemSize)
		{
			// Check the CRC in RDRAM
			if( gTextureBufferInfos[i].crcCheckedAtFrame < status.gDlistCount )
			{
				uint32 crc = ComputeTextureBufferCRCInRDRAM(i);
				if( gTextureBufferInfos[i].crcInRDRAM != crc )
				{
					// RDRAM has been modified by CPU core
					SAFE_DELETE(gTextureBufferInfos[i].pTxtBuffer);
					gTextureBufferInfos[i].isUsed = false;
					continue;
				}
				else
				{
					gTextureBufferInfos[i].crcCheckedAtFrame = status.gDlistCount;
				}
			}

			return i;
		}
	}

	return -1;
}

void CGraphicsContext::LoadTextureFromTextureBuffer(TxtrCacheEntry* pEntry, int infoIdx)
{
	if( infoIdx < 0 || infoIdx >= numOfTxtBufInfos )
	{
		infoIdx = CheckAddrInTxtrBufs(pEntry->ti);
	}

	if( infoIdx >= 0 && gTextureBufferInfos[infoIdx].isUsed && gTextureBufferInfos[infoIdx].pTxtBuffer )
	{
		gTextureBufferInfos[infoIdx].pTxtBuffer->LoadTexture(pEntry);
	}
}

void CGraphicsContext::RestoreNormalBackBuffer()
{
	if( m_curTextureBufferIndex >= 0 && m_curTextureBufferIndex < numOfTxtBufInfos )
	{
		if( gTextureBufferInfos[m_curTextureBufferIndex].pTxtBuffer )
			gTextureBufferInfos[m_curTextureBufferIndex].pTxtBuffer->SetAsRenderTarget(false);
		m_isRenderingToTexture = false;
		m_lastTextureBufferIndex = m_curTextureBufferIndex;
	}

	if( !status.bFrameBufferIsDrawn || !status.bFrameBufferDrawnByTriangles )
	{
		gTextureBufferInfos[m_curTextureBufferIndex].isUsed = false;
		SAFE_DELETE(gTextureBufferInfos[m_curTextureBufferIndex].pTxtBuffer);
	}
}

extern uint32 CalculateRDRAMCRC(void *pAddr, uint32 left, uint32 top, uint32 width, uint32 height, uint32 size, uint32 pitchInBytes );
uint32 CGraphicsContext::ComputeTextureBufferCRCInRDRAM(int infoIdx)
{
	if( infoIdx >= numOfTxtBufInfos || infoIdx < 0 || !gTextureBufferInfos[infoIdx].isUsed )
		return 0;

	TextureBufferInfo &info = gTextureBufferInfos[infoIdx];
	uint32 height = info.knownHeight ? info.N64Height : info.maxUsedHeight;
	uint8 *pAddr = (uint8*)(g_pRDRAMu8+info.CI_Info.dwAddr);
	uint32 pitch = (info.N64Width << info.CI_Info.dwSize ) >> 1;
	return CalculateRDRAMCRC(pAddr, 0, 0, info.N64Width, height, info.CI_Info.dwSize, pitch);
}

int _cdecl SortFrequenciesCallback( const VOID* arg1, const VOID* arg2 )
{
	UINT* p1 = (UINT*)arg1;
	UINT* p2 = (UINT*)arg2;

	if( *p1 < *p2 )   
		return -1;
	else if( *p1 > *p2 )   
		return 1;
	else 
		return 0;
}
int _cdecl SortResolutionsCallback( const VOID* arg1, const VOID* arg2 )
{
	UINT* p1 = (UINT*)arg1;
	UINT* p2 = (UINT*)arg2;

	if( *p1 < *p2 )   
		return -1;
	else if( *p1 > *p2 )   
		return 1;
	else 
	{
		if( p1[1] < p2[1] )   
			return -1;
		else if( p1[1] > p2[1] )   
			return 1;
		else
			return 0;
	}
}

// This is a static function, will be called when the plugin DLL is initialized
void CGraphicsContext::InitDeviceParameters(void)
{
	// Initialize common device parameters
	int i=0, j;
	DEVMODE deviceMode;
	int	numOfFrequency=0, numOfColorDepth = 0;
	CGraphicsContext::m_numOfResolutions=0;
	memset(&CGraphicsContext::m_FullScreenRefreshRates,0,40*sizeof(UINT));
	memset(&CGraphicsContext::m_FullScreenResolutions, 0, 40*2*sizeof(int));
	memset(&CGraphicsContext::m_ColorBufferDepths, 0, 4*sizeof(UINT));

	while (EnumDisplaySettings( NULL, i, &deviceMode ) != 0)
	{
		for (j = 0; j < numOfColorDepth; j++)
		{
			if (deviceMode.dmBitsPerPel == CGraphicsContext::m_ColorBufferDepths[j])
				break;
		}

		if( j == numOfColorDepth && deviceMode.dmBitsPerPel > 8 )
		{
			CGraphicsContext::m_ColorBufferDepths[numOfColorDepth++] = deviceMode.dmBitsPerPel;
		}

		for (j = 0; j < CGraphicsContext::m_numOfResolutions; j++)
		{
			if ((deviceMode.dmPelsWidth == CGraphicsContext::m_FullScreenResolutions[j][0]) &&
				(deviceMode.dmPelsHeight == CGraphicsContext::m_FullScreenResolutions[j][1]))
			{
				break;
			}
		}

		if( j == CGraphicsContext::m_numOfResolutions )
		{
			CGraphicsContext::m_FullScreenResolutions[CGraphicsContext::m_numOfResolutions][0] = deviceMode.dmPelsWidth;
			CGraphicsContext::m_FullScreenResolutions[CGraphicsContext::m_numOfResolutions][1] = deviceMode.dmPelsHeight;
			CGraphicsContext::m_numOfResolutions++;
		}

		for( j=0; j<numOfFrequency; j++)
		{
			if( CGraphicsContext::m_FullScreenRefreshRates[j] == deviceMode.dmDisplayFrequency )
				break;
		}

		if( j == numOfFrequency && deviceMode.dmDisplayFrequency >= 50 )
		{
			CGraphicsContext::m_FullScreenRefreshRates[numOfFrequency++] = deviceMode.dmDisplayFrequency;
		}

		i++;
	}

	qsort( &CGraphicsContext::m_FullScreenRefreshRates, numOfFrequency, sizeof(UINT), SortFrequenciesCallback );
	qsort( &CGraphicsContext::m_FullScreenResolutions, CGraphicsContext::m_numOfResolutions, sizeof(int)*2, SortResolutionsCallback );

	// To initialze device parameters for DirectX
	CDXGraphicsContext::InitDeviceParameters();

	// To initialze device parameters for DirectX
	COGLGraphicsContext::InitDeviceParameters();
}

void CGraphicsContext::FirstDrawToNewCI(void)
{
	status.bCIBufferIsRendered = true;
	if( status.bHandleN64TextureBuffer )
	{
		// Checking against previous texture buffer infos
		int matchidx = -1;

		uint32 memsize = ((newTextureBufInfo.N64Height*newTextureBufInfo.N64Width)>>1)<<newTextureBufInfo.CI_Info.dwSize;

		matchidx = CheckTxtrBufsWithNewCI(g_CI,newTextureBufInfo.N64Height,true);

		int idxToUse=-1;
		if( matchidx >= 0 )
		{
			// Reuse the matched slot
			idxToUse = matchidx;
			if( gTextureBufferInfos[matchidx].pTxtBuffer == NULL )
			{
				if( newTextureBufInfo.knownHeight == RDP_SETSCISSOR && newTextureBufInfo.CI_Info.dwAddr == g_ZI.dwAddr )
				{
					gTextureBufferInfos[matchidx].pTxtBuffer = 
						new CDXTextureBuffer(gRDP.scissor.right, newTextureBufInfo.bufferHeight, &gTextureBufferInfos[matchidx], AS_RENDER_TARGET);
				}
				else
				{
					gTextureBufferInfos[matchidx].pTxtBuffer = 
						new CDXTextureBuffer(newTextureBufInfo.bufferWidth, newTextureBufInfo.bufferHeight, &gTextureBufferInfos[matchidx], AS_RENDER_TARGET);
				}
			}
		}
		else
		{
			idxToUse = FindASlot();

			// After get the slot
			// create a new texture buffer and assign it to this slot
			if( newTextureBufInfo.knownHeight == RDP_SETSCISSOR && newTextureBufInfo.CI_Info.dwAddr == g_ZI.dwAddr )
			{
				gTextureBufferInfos[idxToUse].pTxtBuffer = 
					new CDXTextureBuffer(gRDP.scissor.right, newTextureBufInfo.bufferHeight, &gTextureBufferInfos[idxToUse], AS_RENDER_TARGET);
			}
			else
			{
				gTextureBufferInfos[idxToUse].pTxtBuffer = 
					new CDXTextureBuffer(newTextureBufInfo.bufferWidth, newTextureBufInfo.bufferHeight, &gTextureBufferInfos[idxToUse], AS_RENDER_TARGET); 
			}
		}

		// Need to set all variables for gTextureBufferInfos[idxToUse]
		CTextureBuffer *pTxtBuffer = gTextureBufferInfos[idxToUse].pTxtBuffer;
		memcpy(&gTextureBufferInfos[idxToUse], &newTextureBufInfo, sizeof(TextureBufferInfo) );
		gTextureBufferInfos[idxToUse].pTxtBuffer = pTxtBuffer;
		gTextureBufferInfos[idxToUse].isUsed = true;
		gTextureBufferInfos[idxToUse].txtEntry.pTexture = pTxtBuffer->m_pTexture;
		gTextureBufferInfos[idxToUse].txtEntry.txtrBufIdx = idxToUse+1;

		g_pTextureBufferInfo = &gTextureBufferInfos[idxToUse];

		// Active the texture buffer
		if( m_curTextureBufferIndex >= 0 && gTextureBufferInfos[m_curTextureBufferIndex].isUsed && gTextureBufferInfos[m_curTextureBufferIndex].pTxtBuffer )
		{
			gTextureBufferInfos[m_curTextureBufferIndex].pTxtBuffer->SetAsRenderTarget(false);
			m_isRenderingToTexture = false;
		}

		if( gTextureBufferInfos[idxToUse].pTxtBuffer->SetAsRenderTarget(true) )
		{
			m_isRenderingToTexture = true;

			//Clear(CLEAR_COLOR_AND_DEPTH_BUFFER,0x80808080,1.0f);
			if( frameBufferOptions.bFillRectNextTextureBuffer )
				Clear(CLEAR_COLOR_BUFFER,gRDP.fillColor,1.0f);
			else if( options.enableHackForGames == HACK_FOR_MARIO_TENNIS && g_pTextureBufferInfo->N64Width > 64 && g_pTextureBufferInfo->N64Width < 300 )
			{
				Clear(CLEAR_COLOR_BUFFER,0,1.0f);
			}
			else if( options.enableHackForGames == HACK_FOR_MARIO_TENNIS && g_pTextureBufferInfo->N64Width < 64 && g_pTextureBufferInfo->N64Width > 32 )
			{
				Clear(CLEAR_COLOR_BUFFER,0,1.0f);
			}

			m_curTextureBufferIndex = idxToUse;

			status.bDirectWriteIntoRDRAM = false;

			//SetScreenMult(1, 1);
			SetScreenMult(gTextureBufferInfos[m_curTextureBufferIndex].scaleX, gTextureBufferInfos[m_curTextureBufferIndex].scaleY);
			CRender::g_pRender->UpdateClipRectangle();
			D3DVIEWPORT8 vp = {0,0,gTextureBufferInfos[idxToUse].bufferWidth,gTextureBufferInfos[idxToUse].bufferHeight};
			gD3DDevWrapper.SetViewport(&vp);

			// If needed, draw RDRAM into the texture buffer
			//if( frameBufferOptions.bLoadRDRAMIntoTxtBuf )
			//{
			//	CRender::GetRender()->LoadTxtrBufFromRDRAM();
			//}
		}
		else
		{
		}	
	}
	else
	{
		//CGraphicsContext::g_pGraphicsContext->CheckTxtrBufsWithNewCI(g_CI,g_uRecentCIInfoPtrs[0]->dwHeight,false);
		extern void UpdateRecentCIAddr(SetImgInfo &ciinfo);
		UpdateRecentCIAddr(g_CI);
		CGraphicsContext::g_pGraphicsContext->CheckTxtrBufsWithNewCI(g_CI,gRDP.scissor.bottom,false);
	}
}