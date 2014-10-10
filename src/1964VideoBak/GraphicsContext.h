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

#ifndef _AFX_GFXCONTEXT_H_
#define _AFX_GFXCONTEXT_H_

#include "CritSect.h"

enum ClearFlag
{
	CLEAR_COLOR_BUFFER=0x01,
	CLEAR_DEPTH_BUFFER=0x02,
	CLEAR_COLOR_AND_DEPTH_BUFFER=0x03,
};


typedef struct
{
	uint32	addr;	//N64 RDRAM address
	uint32	size;	//N64 buffer size
	uint32  format;	//N64 format
	uint32	width;
	uint32	height;
} TextureBufferShortInfo;


// This class basically provides an extra level of security for our
// multithreaded code. Threads can Grab the CGraphicsContext to prevent
// other threads from changing/releasing any of the pointers while it is
// running.

// It is based on CCritSect for Lock() and Unlock()

class CGraphicsContext : public CCritSect
{
	friend class CDeviceBuilder;
	
public:
	bool Ready() { return m_bReady; }
	bool IsWindowed() {return m_bWindowed;}

	virtual bool Initialize(HWND hWnd, HWND hWndStatus, uint32 dwWidth, uint32 dwHeight, BOOL bWindowed );
	virtual void CleanUp();

	virtual void Clear(ClearFlag flags, uint32 color=0xFF000000, float depth=1.0f) = 0;
	virtual void UpdateFrame(bool swaponly=false) = 0;
	virtual int ToggleFullscreen()=0;		// return 0 as the result is windowed

	// Render-To-Texture functions
	virtual void SetTextureBuffer(void);
	virtual int SetBackBufferAsTextureBuffer(SetImgInfo &CIinfo, int ciInfoIdx);
	virtual bool IsRenderingToTexture() {return m_isRenderingToTexture;}
	virtual void CloseTextureBuffer(bool toSave);
	virtual void RestoreNormalBackBuffer();					// restore the normal back buffer
	int			 CheckAddrInTxtrBufs(TxtrInfo &ti);
	void		 CheckTxtrBufsCRCInRDRAM(void);
	int			 CheckTxtrBufsWithNewCI(SetImgInfo &CIinfo, uint32 height, bool byNewTxtrBuf);
	void		 LoadTextureFromTextureBuffer(TxtrCacheEntry* pEntry, int infoIdx);
	virtual void StoreTextureBufferToRDRAM(int infoIdx = -1) {}
	virtual void FirstDrawToNewCI(void);
	uint32		 ComputeTextureBufferCRCInRDRAM(int infoIdx);

	// Frame buffer functions
	virtual void SaveBackBuffer(int ciInfoIdx, RECT* pRect=NULL) {}			// Copy the current back buffer to temp buffer

	virtual void ClearFrameBufferToBlack(uint32 left=0, uint32 top=0, uint32 width=0, uint32 height=0);
	virtual void CopyBackToFrameBufferIfReadByCPU(uint32 addr);
	virtual void CopyBackToRDRAM(uint32 addr, uint32 fmt, uint32 siz, uint32 width, 
		uint32 height, uint32 bufWidth, uint32 bufHeight, uint32 startaddr=0xFFFFFFFF, 
		uint32 memsize=0xFFFFFFFF, uint32 pitch=0, D3DFORMAT surf_fmt=D3DFMT_A8R8G8B8, IDirect3DSurface8 *surf=NULL) {}
	void UpdateFrameBufferBeforeUpdateFrame();

	static void InitWindowInfo();
	static void InitDeviceParameters();

	bool m_supportTextureMirror;
	TextureBufferShortInfo m_textureColorBufferInfo;
	TextureBufferShortInfo m_textureDepthBufferInfo;

public:
	static	HWND		m_hWnd;
	static	HWND		m_hWndStatus;
	static	HWND		m_hWndToolbar;
	static	HMENU		m_hMenu;
	static  int			m_maxFSAA;
	static  int			m_maxAnisotropy;
	static  UINT		m_FullScreenRefreshRates[40];
	static  UINT		m_ColorBufferDepths[4];
	static  int			m_FullScreenResolutions[40][2];
	static  int			m_numOfResolutions;

protected:
    static	uint32      m_dwWindowStyle;     // Saved window style for mode switches
    static	uint32      m_dwWindowExStyle;   // Saved window style for mode switches
	static	uint32      m_dwStatusWindowStyle;     // Saved window style for mode switches

	static  bool		m_deviceCapsIsInitialized;

	bool				m_backBufferIsSaved;

	bool				m_bReady;
	bool				m_bActive;
	
	bool				m_bWindowed;
	RECT				m_rcWindowBounds;

	DEVMODEA			m_DMsaved;
	
    char				m_strDeviceStats[90];

	int					m_curTextureBufferIndex;
	int					m_curBackBufferIndex;
	int					m_lastTextureBufferIndex;
	bool				m_isRenderingToTexture;

	virtual ~CGraphicsContext();
	CGraphicsContext();
	
public:
	static CGraphicsContext *g_pGraphicsContext;
	static CGraphicsContext * Get(void);
	inline const char* GetDeviceStr() {return m_strDeviceStats;}
	static bool needCleanScene;
};

class CTextureBuffer;
typedef struct {
	CTextureBuffer *pTxtBuffer;
	SetImgInfo	CI_Info;

	uint32		bufferWidth;
	uint32		bufferHeight;
	uint32		N64Width;
	uint32		N64Height;
	float		scaleX;
	float		scaleY;

	int			maxUsedHeight;
	uint32		updateAtFrame;
	uint32		updateAtUcodeCount;

	bool		isUsed;
	uint32		knownHeight;

	uint32		crcInRDRAM;
	uint32		crcCheckedAtFrame;

	TxtrCacheEntry txtEntry;
} TextureBufferInfo;


class CTextureBuffer
{
public:
	friend class CGraphicsContext;
	friend class CDXGraphicsContext;
	CTextureBuffer(int width, int height, TextureBufferInfo* pInfo, TextureUsage usage)
	{
		m_beingRendered = false;
		m_width = m_height = 0;
		m_pTexture = NULL;
		m_pInfo = pInfo;
		m_usage = usage;
	}
	virtual ~CTextureBuffer() {}

	virtual bool SetAsRenderTarget(bool enable)=0;
	virtual void LoadTexture(TxtrCacheEntry* pEntry)=0;

	virtual void SaveToN64RDRAM(uint32 addr, int width=0, int height=0, int ci_width=0) {};

	void GetDimension(int &width, int &height)
	{
		width = m_width;
		height = m_height;
	}

	bool IsBeingRendered()
	{
		return m_beingRendered;
	}

protected:
	int		m_width;
	int		m_height;
	bool	m_beingRendered;
	TextureUsage m_usage;

	CTexture* m_pTexture;
	TextureBufferInfo* m_pInfo;
};

extern TextureBufferInfo gTextureBufferInfos[];
extern TextureBufferInfo newTextureBufInfo;

#define NEW_TEXTURE_BUFFER

extern TextureBufferInfo g_ZI_saves[2];
extern TextureBufferInfo *g_pTextureBufferInfo;


#endif
