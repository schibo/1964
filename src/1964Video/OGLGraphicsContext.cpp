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
#define GLH_EXT_SINGLE_FILE
//#define WGL_WGLEXT_PROTOTYPES
#include "glh_genext.h"

const char * ( WINAPI *wglGetExtensionsStringARB)(HDC)=NULL;
BOOL (WINAPI * wglSwapIntervalEXT) (int interval)=NULL;

COGLGraphicsContext::COGLGraphicsContext() :
	m_hDC(0),
	m_hGLRC(0),
	m_pVendorStr(NULL),
	m_pRenderStr(NULL),
	m_pExtensionStr(NULL),
	m_pVersionStr(NULL),
	m_bSupportMultiTexture(false),
	m_bSupportTextureEnvCombine(false),
	m_bSupportSeparateSpecularColor(false),
	m_bSupportSecondColor(false),
	m_bSupportFogCoord(false),
	m_bSupportTextureObject(false),
	m_bSupportRescaleNormal(false),
	m_bSupportLODBias(false),
	m_bSupportTextureMirrorRepeat(false),
	m_bSupportTextureLOD(false),
	m_bSupportNVRegisterCombiner(false),
	m_bSupportBlendColor(false),
	m_bSupportBlendSubtract(false),
	m_bSupportNVTextureEnvCombine4(false)
{
}


COGLGraphicsContext::~COGLGraphicsContext()
{
}

bool COGLGraphicsContext::Initialize(HWND hWnd, HWND hWndStatus, uint32 dwWidth, uint32 dwHeight, BOOL bWindowed )
{
	if( g_GraphicsInfo.hStatusBar )
	{
		SetWindowText(g_GraphicsInfo.hStatusBar,"Initializing OpenGL Device Context");
	}
	Lock();

	CGraphicsContext::Get()->m_supportTextureMirror = false;
	CGraphicsContext::Initialize(hWnd, hWndStatus, dwWidth, dwHeight, bWindowed );

	if( bWindowed )
	{
		windowSetting.statusBarHeightToUse = windowSetting.statusBarHeight;
		windowSetting.toolbarHeightToUse = windowSetting.toolbarHeight;
	}
	else
	{
		windowSetting.statusBarHeightToUse = 0;
		windowSetting.toolbarHeightToUse = 0;
	}

	int  pfm;
	int	 depthBufferDepth = options.OpenglDepthBufferSetting;
	int	 colorBufferDepth = 32;
	if( options.colorQuality == TEXTURE_FMT_A4R4G4B4 ) colorBufferDepth = 16;

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                       // version number
		PFD_DRAW_TO_WINDOW |     // support window
		PFD_SUPPORT_OPENGL |     // support OpenGL
		//PFD_GENERIC_ACCELERATED | PFD_SWAP_COPY | PFD_SWAP_EXCHANGE |
		PFD_DOUBLEBUFFER,        // double buffered
		PFD_TYPE_RGBA,           // RGBA type
		colorBufferDepth,
		0, 0, 0, 0, 0, 0,        // color bits ignored
		0,                       // no alpha buffer
		0,                       // shift bit ignored
		0,                       // no accumulation buffer
		0, 0, 0, 0,              // accum bits ignored
		depthBufferDepth,        // z-buffer      
		0,                       // no stencil buffer
		0,						 // no auxiliary buffer
		PFD_MAIN_PLANE,          // main layer
		0,                       // reserved
		0, 0, 0};                // layer masks ignored

	if ((m_hDC = GetDC(hWnd)) == NULL)
	{
		ErrorMsg("GetDC on main window failed");
		return false;
	}

	if ((pfm = ChoosePixelFormat(m_hDC, &pfd)) == 0)
	{
		ErrorMsg("ChoosePixelFormat failed");
		return false;
	}
	if (SetPixelFormat(m_hDC, pfm, &pfd) == FALSE)
	{
		ErrorMsg("SetPixelFormat failed");
		return false;
	}

	DescribePixelFormat(m_hDC, pfm, sizeof(pfd), &pfd);
	
	if ((m_hGLRC = wglCreateContext(m_hDC)) == 0)
	{
		ErrorMsg("wglCreateContext failed!");
		CleanUp();
		return false;
	}

	if (!wglMakeCurrent(m_hDC, m_hGLRC))
	{
		ErrorMsg("wglMakeCurrent failed!");
		CleanUp();
		return false;
	}

	InitState();
	InitOGLExtension();
	sprintf(m_strDeviceStats, "%s - %s : %s", m_pVendorStr, m_pRenderStr, m_pVersionStr);
	SetWindowText(m_hWndStatus, m_strDeviceStats);

	Unlock();

	Clear(CLEAR_COLOR_AND_DEPTH_BUFFER);	// Clear buffers
	UpdateFrame();
	Clear(CLEAR_COLOR_AND_DEPTH_BUFFER);
	UpdateFrame();
	
	m_bReady = true;

	return true;
}

void COGLGraphicsContext::InitState(void)
{
	m_pRenderStr = glGetString(GL_RENDERER);;
	m_pExtensionStr = glGetString(GL_EXTENSIONS);;
	m_pVersionStr = glGetString(GL_VERSION);;
	m_pVendorStr = glGetString(GL_VENDOR);
	wglGetExtensionsStringARB = (const char *(WINAPI *)(HDC))wglGetProcAddress( "wglGetExtensionsStringARB" );
	if( wglGetExtensionsStringARB )
	{
		m_pWglExtensionStr = wglGetExtensionsStringARB(m_hDC);
	}
	else
	{
		m_pWglExtensionStr = NULL;
	}

	if( IsWglExtensionSupported("WGL_EXT_swap_control") )
	{
		wglSwapIntervalEXT = (BOOL (WINAPI *) (int))wglGetProcAddress( "wglSwapIntervalEXT" );
		wglSwapIntervalEXT(0);	// Disable buffer swapping sync to increase speed
	}


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	glShadeModel(GL_SMOOTH);

    //position viewer 
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glDisable(GL_ALPHA_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);

	glFrontFace(GL_CCW);
	glDisable(GL_CULL_FACE);
	glDisable(GL_NORMALIZE);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glDepthRange(-1, 1);
}

void COGLGraphicsContext::InitOGLExtension(void)
{

	// important extension features, it is very bad not to have these feature
	m_bSupportMultiTexture = IsExtensionSupported("GL_ARB_multitexture");
	m_bSupportTextureEnvCombine = IsExtensionSupported("GL_EXT_texture_env_combine");
	
	m_bSupportSeparateSpecularColor = IsExtensionSupported("GL_EXT_separate_specular_color");
	m_bSupportSecondColor = IsExtensionSupported("GL_EXT_secondary_color");
	m_bSupportFogCoord = IsExtensionSupported("GL_EXT_fog_coord");
	m_bSupportTextureObject = IsExtensionSupported("GL_EXT_texture_object");

	// Optional extension features
	m_bSupportRescaleNormal = IsExtensionSupported("GL_EXT_rescale_normal");
	m_bSupportLODBias = IsExtensionSupported("GL_EXT_texture_lod_bias");

	// Nvidia only extension features (optional)
	m_bSupportNVRegisterCombiner = IsExtensionSupported("GL_NV_register_combiners");
	m_bSupportTextureMirrorRepeat = IsExtensionSupported("GL_IBM_texture_mirrored_repeat");
	m_supportTextureMirror = m_bSupportTextureMirrorRepeat;
	m_bSupportTextureLOD = IsExtensionSupported("GL_EXT_texture_lod");
	m_bSupportBlendColor = IsExtensionSupported("GL_EXT_blend_color");
	m_bSupportBlendSubtract = IsExtensionSupported("GL_EXT_blend_subtract");
	m_bSupportNVTextureEnvCombine4 = IsExtensionSupported("GL_NV_texture_env_combine4");

	glh_init_extension("GL_ARB_multitexture");
	glh_init_extension("GL_EXT_texture_env_combine");
	glh_init_extension("GL_EXT_separate_specular_color");
	glh_init_extension("GL_EXT_secondary_color");
	glh_init_extension("GL_EXT_fog_coord");
	glh_init_extension("GL_EXT_texture_object");
	glh_init_extension("GL_EXT_rescale_normal");
	glh_init_extension("GL_EXT_texture_lod_bias");
	glh_init_extension("GL_NV_register_combiners");
	glh_init_extension("GL_IBM_texture_mirrored_repeat");
	glh_init_extension("GL_EXT_texture_lod");
	glh_init_extension("GL_EXT_blend_color");
	glh_init_extension("GL_EXT_blend_subtract");
	glh_init_extension("GL_NV_texture_env_combine4");
}

bool COGLGraphicsContext::IsExtensionSupported(const char* pExtName)
{
	if( strstr((const char*)m_pExtensionStr, pExtName) != NULL )
		return true;
	else
		return false;
}

bool COGLGraphicsContext::IsWglExtensionSupported(const char* pExtName)
{
	if( m_pWglExtensionStr == NULL )
		return false;

	if( strstr((const char*)m_pWglExtensionStr, pExtName) != NULL )
		return true;
	else
		return false;
}


void COGLGraphicsContext::CleanUp()
{
	if (m_hGLRC)
	{
		//UpdateFrame();

		if (!wglMakeCurrent(m_hDC, m_hGLRC))
		//if (!wglMakeCurrent(NULL, NULL))
		{
			uint32 error = GetLastError();
		}

		//DelTexture();

		if (!wglDeleteContext(m_hGLRC))
		{
			uint32 error = GetLastError();
		}
		m_hGLRC = NULL;
	}

	if (m_hDC)
	{
		if (!ReleaseDC(m_hWnd, m_hDC))
		{
			uint32 error = GetLastError();
		}
		m_hDC = NULL;
	}

	m_bReady = false;
}


void COGLGraphicsContext::Clear(ClearFlag dwFlags, uint32 color, float depth)
{
	uint32 flag=0;
	if( dwFlags&CLEAR_COLOR_BUFFER )	flag |= GL_COLOR_BUFFER_BIT;
	if( dwFlags&CLEAR_DEPTH_BUFFER )	flag |= GL_DEPTH_BUFFER_BIT;

	float r = ((color>>16)&0xFF)/255.0f;
	float g = ((color>> 8)&0xFF)/255.0f;
	float b = ((color    )&0xFF)/255.0f;
	float a = ((color>>24)&0xFF)/255.0f;
	glClearColor(r, g, b, a);
	glClearDepth(depth);
	glClear(flag);	//Clear color buffer and depth buffer
}


void COGLGraphicsContext::UpdateFrame(bool swaponly)
{
	status.gFrameCount++;

	glFlush();
	//glFinish();
	//wglSwapIntervalEXT(0);
	SwapBuffers(m_hDC);
	glDepthMask(GL_TRUE);
	glClearDepth(1.0);
	if( !g_curRomInfo.bForceScreenClear ) 
		glClear(GL_DEPTH_BUFFER_BIT);
	else
		needCleanScene = true;

	m_backBufferIsSaved = false;
	status.bScreenIsDrawn = false;
}

bool COGLGraphicsContext::SetFullscreenMode()
{
	DEVMODEA devMode;

	ZeroMemory(&devMode, sizeof(DEVMODE));
	devMode.dmSize       = sizeof(DEVMODE);
	devMode.dmPelsWidth	 = windowSetting.uFullScreenDisplayWidth;
	devMode.dmPelsHeight = windowSetting.uFullScreenDisplayHeight;
	devMode.dmBitsPerPel = options.colorQuality==TEXTURE_FMT_A8R8G8B8?32:16;
	if( windowSetting.uFullScreenRefreshRate == 0 )
		devMode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL ;//| DM_DISPLAYFREQUENCY;
	else
	{
		devMode.dmDisplayFrequency = windowSetting.uFullScreenRefreshRate;
		devMode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;
	}
	
	if (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		return false;
	}

	ShowCursor(0);

	m_WindowPlacement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(m_hWnd, &m_WindowPlacement);
	if (m_hWndStatus)
	{
		ShowWindow(m_hWndStatus,SW_HIDE); 
	}
	if (m_hMenu) SetMenu(m_hWnd,NULL); 

	SetWindowLong(m_hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
	SetWindowLong(m_hWnd, GWL_STYLE, WS_VISIBLE|WS_POPUP);
	
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, windowSetting.uFullScreenDisplayWidth,windowSetting.uFullScreenDisplayHeight, SWP_SHOWWINDOW);

	windowSetting.uDisplayWidth = windowSetting.uFullScreenDisplayWidth;
	windowSetting.uDisplayHeight = windowSetting.uFullScreenDisplayHeight;
	windowSetting.statusBarHeightToUse = 0;
	windowSetting.toolbarHeightToUse = 0;

	return true;
}

bool COGLGraphicsContext::SetWindowMode()
{
	SetWindowLong(m_hWnd, GWL_STYLE, 0);
	ShowWindow(m_hWnd, SW_SHOWNORMAL);

	if (!ChangeDisplaySettings(NULL,CDS_TEST)) 
	{
		// If The Shortcut Doesn't Work
		ChangeDisplaySettings(NULL,CDS_RESET);		// Do It Anyway (To Get The Values Out Of The Registry)
		ChangeDisplaySettings(&m_DMsaved,CDS_RESET);	// Change It To The Saved Settings
	} 
	else 
	{
		ChangeDisplaySettings(NULL,CDS_RESET);		// If It Works, Go Right Ahead
	}

	if (m_hWndStatus)	ShowWindow(m_hWndStatus,SW_SHOW);
	if (m_hMenu)		SetMenu(m_hWnd,m_hMenu); 
	SetWindowLong(m_hWnd, GWL_STYLE, m_dwWindowStyle);
	SetWindowPos(m_hWnd, HWND_NOTOPMOST, m_WindowPlacement.rcNormalPosition.left, 
		m_WindowPlacement.rcNormalPosition.top, 640, 480, SWP_NOSIZE|SWP_SHOWWINDOW);
	ShowCursor(TRUE);

	SetWindowPos(m_hWnd, HWND_TOP, m_rcWindowBounds.left, m_rcWindowBounds.top, 
		m_rcWindowBounds.right - m_rcWindowBounds.left,
		m_rcWindowBounds.bottom - m_rcWindowBounds.top,
		SWP_SHOWWINDOW);

	GetWindowRect(m_hWnd, &m_rcWindowBounds);

	windowSetting.uDisplayWidth = windowSetting.uWindowDisplayWidth;
	windowSetting.uDisplayHeight = windowSetting.uWindowDisplayHeight;
	windowSetting.statusBarHeightToUse = windowSetting.statusBarHeight;
	windowSetting.toolbarHeightToUse = windowSetting.toolbarHeight;

	return true;
}
int COGLGraphicsContext::ToggleFullscreen()
{
    if( m_bWindowed ) 
    {
    	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &m_DMsaved);
        m_bWindowed = !SetFullscreenMode();
    }
    else
    {
        m_bWindowed = SetWindowMode();
    }

	if( !m_bWindowed )
	{
		ShowCursor( FALSE );
	}
	else
	{
		ShowCursor( TRUE );
	}

	return m_bWindowed?0:1;
}

// This is a static function, will be called when the plugin DLL is initialized
void COGLGraphicsContext::InitDeviceParameters()
{
}

