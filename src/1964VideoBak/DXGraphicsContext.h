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

#ifndef _DX_GRAPHICS_CONTEXT_H_
#define _DX_GRAPHICS_CONTEXT_H_


//-----------------------------------------------------------------------------
// Error codes
//-----------------------------------------------------------------------------
enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHEDTOREF };

#define D3DAPPERR_NODIRECT3D          0x82000001
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE    0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE    0x82000006
#define D3DAPPERR_NOWINDOWEDHAL       0x82000007
#define D3DAPPERR_NODESKTOPHAL        0x82000008
#define D3DAPPERR_NOHALTHISMODE       0x82000009
#define D3DAPPERR_NONZEROREFCOUNT     0x8200000a
#define D3DAPPERR_RESIZEFAILED        0x8200000c




//-----------------------------------------------------------------------------
// Name: struct D3DModeInfo
// Desc: Structure for holding information about a display mode
//-----------------------------------------------------------------------------
struct D3DModeInfo
{
    uint32      Width;      // Screen width in this mode
    uint32      Height;     // Screen height in this mode
    D3DFORMAT  Format;     // Pixel format in this mode
    uint32	   RefreshRate;     // Pixel format in this mode
    uint32      dwBehavior; // Hardware / Software / Mixed vertex processing
    D3DFORMAT  DepthStencilFormat; // Which depth/stencil format to use with this mode
};




//-----------------------------------------------------------------------------
// Name: struct D3DDeviceInfo
// Desc: Structure for holding information about a Direct3D device, including
//       a list of modes compatible with this device
//-----------------------------------------------------------------------------
struct D3DDeviceInfo
{
    // Device data
    D3DDEVTYPE   DeviceType;      // Reference, HAL, etc.
    D3DCAPS8     d3dCaps;         // Capabilities of this device
    const TCHAR* strDesc;         // Name of this device
    bool		 bCanDoWindowed;  // Whether this device can work in windowed mode
	
    // Modes for this device
    int			dwNumModes;
    D3DModeInfo  modes[150];
	
    // Current state
    bool		bWindowed;
    D3DMULTISAMPLE_TYPE MultiSampleType;
};




//-----------------------------------------------------------------------------
// Name: struct D3DAdapterInfo
// Desc: Structure for holding information about an adapter, including a list
//       of devices available on this adapter
//-----------------------------------------------------------------------------
struct D3DAdapterInfo
{
    // Adapter data
    D3DADAPTER_IDENTIFIER8 d3dAdapterIdentifier;
    D3DDISPLAYMODE d3ddmDesktop;      // Desktop display mode for this adapter
	
    // Devices for this adapter
    int				dwNumDevices;
    D3DDeviceInfo  devices[5];
	
    // Current state
    int				dwCurrentDevice;
};

extern BufferSettingInfo DirectXRenderBufferSettings[];
extern BufferSettingInfo DirectXDepthBufferSetting[];
extern BufferSettingInfo DirectXCombinerSettings[];
extern int numberOfDirectXRenderBufferSettings;
extern int numberOfDirectXCombinerSettings;
extern int numberOfDirectXDepthBufferSettings;
struct TxtrCacheEntry;

// Implementation
class CDXGraphicsContext : public CGraphicsContext
{
public:
	virtual ~CDXGraphicsContext();
	
	
	bool Initialize(HWND hWnd, HWND hWndStatus, uint32 dwWidth, uint32 dwHeight, BOOL bWindowed );
	void CleanUp();
	
	void Clear(ClearFlag dwFlags, uint32 color=0xFF000000, float depth=1.0f);
	void UpdateFrame(bool swaponly=false);

	void SaveBackBuffer(int ciInfoIdx, RECT* pRect=NULL);				// Copy the current back buffer to temp buffer
	bool IsFSAAEnable() {return m_FSAAIsEnabled;}
	virtual void StoreTextureBufferToRDRAM(int infoIdx = -1);
	virtual void CopyBackToRDRAM(uint32 addr, uint32 fmt, uint32 siz, uint32 width, 
		uint32 height, uint32 bufWidth, uint32 bufHeight, uint32 startaddr=0xFFFFFFFF, 
		uint32 memsize=0xFFFFFFFF, uint32 pitch=0, D3DFORMAT surf_fmt=D3DFMT_A8R8G8B8, IDirect3DSurface8 *surf=NULL);

	int ToggleFullscreen();		// return 0 as the result is windowed
	D3DFORMAT GetFormat() {return m_desktopFormat;}
	static bool IsResultGood(HRESULT hr, bool displayError = false);
	static void InitDeviceParameters();
	static D3DAdapterInfo& GetAdapterInfo()	{return m_1stAdapters;}

protected:
	friend class DirectXDeviceBuilder;
	CDXGraphicsContext();

	BOOL FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
		D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat );

	HRESULT BuildDeviceList();
	int		FindCurrentDisplayModeIndex();
	static HRESULT ConfirmDevice( D3DCAPS8* pCaps, uint32 dwBehavior, D3DFORMAT Format );

	
	HRESULT Create3D( BOOL bWindowed );
	HRESULT InitializeD3D();

	HRESULT ResizeD3DEnvironment();
	HRESULT DoToggleFullscreen();
	HRESULT ForceWindowed();
	HRESULT AdjustWindowForChange();
	
	void Pause( bool bPause );
	
	HRESULT InitDeviceObjects();
	HRESULT CleanDeviceObjects();

	bool	CreateFontObjects();
	bool	DrawText(char *str, RECT &rect, int alignment=0);

	// Device information
	static int				m_dwNumAdapters;
	static D3DAdapterInfo	m_1stAdapters;
	static D3DCAPS8			m_d3dCaps;           // Caps for the device
	static bool				m_bSupportAnisotropy;
	static HRESULT			DisplayD3DErrorMsg( HRESULT hr, uint32 dwType );


	// Graphic context
	LPDIRECT3D8			m_pD3D;
	LPDIRECT3DDEVICE8	m_pd3dDevice;

    uint32				m_dwCreateFlags;     // Indicate sw or hw vertex processing

	D3DPRESENT_PARAMETERS m_d3dpp;

    D3DSURFACE_DESC		m_d3dsdBackBuffer;   // Surface desc of the backbuffer
	
	uint32				m_dwAdapter;
	
	uint32				m_dwMinDepthBits;
	uint32				m_dwMinStencilBits;
	D3DAdapterInfo		m_Adapters[10];

	D3DFORMAT			m_desktopFormat;

	bool				m_FSAAIsEnabled;

	HFONT				m_hFont;
	ID3DXFont*			m_pID3DFont;
	bool				m_bFontIsCreated;
};


class CDXTextureBuffer : public CTextureBuffer
{
public:
	friend class CDXGraphicsContext;
	CDXTextureBuffer(int width, int height, TextureBufferInfo* pInfo, TextureUsage usage);
	~CDXTextureBuffer();

	bool SetAsRenderTarget(bool enable);
	void LoadTexture(TxtrCacheEntry* pEntry);

protected:
	LPDIRECT3DSURFACE8 m_pColorBufferSave;
	LPDIRECT3DSURFACE8 m_pDepthBufferSave;
};

#define MAX_RENDER_STATE					152
#define MAX_NUM_OF_PIXEL_SHADER_CONSTANT	30
#define MAX_TEXTURE_STAGE_STATE				25
class CD3DDevWrapper
{
public:
	CD3DDevWrapper() 
	{
		m_pD3DDev = NULL;
	}

	~CD3DDevWrapper()
	{
		m_pD3DDev = NULL;
	}

	void SetD3DDev(LPDIRECT3DDEVICE8 pD3DDev);

	HRESULT SetRenderState(D3DRENDERSTATETYPE State,DWORD Value);
	HRESULT SetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value);
	HRESULT SetPixelShader(DWORD Handle);
	HRESULT SetPixelShaderConstant (DWORD Register,float* pfdata);
	HRESULT SetViewport (D3DVIEWPORT8* pViewport);
	HRESULT SetTexture (DWORD Stage,IDirect3DBaseTexture8* pTexture);
	HRESULT SetVertexShader (DWORD Handle);
	void    Initalize(void);

private:
	LPDIRECT3DDEVICE8 m_pD3DDev;
	DWORD m_savedRenderStates[MAX_RENDER_STATE];
	DWORD m_savedTextureStageStates[8][MAX_TEXTURE_STAGE_STATE];
	DWORD m_savedPixelShaderHander;
	DWORD m_savedVertexShaderHander;
	float m_savedPixelShaderConstants[MAX_NUM_OF_PIXEL_SHADER_CONSTANT][4];
	D3DVIEWPORT8 m_savedViewport;
	IDirect3DBaseTexture8* m_savedTexturePointers[8];
};

#endif



