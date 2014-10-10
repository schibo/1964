/*$T Dll_Video.c GC 1.136 03/09/02 17:41:40 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    Video plugin interface functions
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
#include "../stdafx.h"
#include <float.h>

uint16		GfxPluginVersion;
HINSTANCE	hinstLibVideo = NULL;
GFX_INFO	Gfx_Info;
int			UsingInternalVideo=0;
BOOL		bRomIsOpened = FALSE;

BOOL ToCaptureScreen=FALSE;
char *CaptureScreenDirectory=NULL;

BOOL (__cdecl *_VIDEO_InitiateGFX) (GFX_INFO) = NULL;
void (__cdecl *_VIDEO_ProcessDList) (void) = NULL;
DWORD (__cdecl *_VIDEO_ProcessDList_Count_Cycles) (void) = NULL;
void (__cdecl *_VIDEO_RomOpen) (void) = NULL;
void (__cdecl *_VIDEO_RomClosed) (void) = NULL;
void (__cdecl *_VIDEO_DllClose) () = NULL;
void (__cdecl *_VIDEO_UpdateScreen) () = NULL;
void (__cdecl *_VIDEO_GetDllInfo) (PLUGIN_INFO *) = NULL;
void (__cdecl *_VIDEO_ExtraChangeResolution) (HWND, long, HWND) = NULL;
void (__cdecl *_VIDEO_DllConfig) (HWND hParent) = NULL;
void (__cdecl *_VIDEO_Test) (HWND) = NULL;
void (__cdecl *_VIDEO_About) (HWND) = NULL;
void (__cdecl *_VIDEO_MoveScreen) (int, int) = NULL;
void (__cdecl *_VIDEO_DrawScreen) (void) = NULL;
void (__cdecl *_VIDEO_ViStatusChanged) (void) = NULL;
void (__cdecl *_VIDEO_ViWidthChanged) (void) = NULL;
void (__cdecl *_VIDEO_ChangeWindow) (int) = NULL;

/* For spec 1.3 */
void (__cdecl *_VIDEO_ChangeWindow_1_3) (void) = NULL;
void (__cdecl *_VIDEO_CaptureScreen) (char *Directory) = NULL;
void (__cdecl *_VIDEO_ProcessRDPList) (void) = NULL;
void (__cdecl *_VIDEO_ShowCFB) (void) = NULL;

/* Used when selecting plugins */
void (__cdecl *_VIDEO_Under_Selecting_Test) (HWND) = NULL;
void (__cdecl *_VIDEO_Under_Selecting_About) (HWND) = NULL;

void (__cdecl *_VIDEO_FrameBufferWrite) (DWORD addr, DWORD size) = NULL;
void (__cdecl *_VIDEO_FrameBufferWriteList) (FrameBufferModifyEntry *plist, DWORD size) = NULL;
void (__cdecl *_VIDEO_FrameBufferRead) (DWORD addr) = NULL;
void (__cdecl *_VIDEO_GetFrameBufferInfo) (void *pinfo) = NULL;
void (__cdecl *_VIDEO_SetOnScreenText) (char *msg) = NULL;
BOOL (__cdecl *_VIDEO_GetFullScreenStatus) (void) = NULL;

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL LoadVideoPlugin(char *libname)
{
	/* Release the video plug-in if it has already been loaded */
	if(hinstLibVideo != NULL)
	{
		FreeLibrary(hinstLibVideo);
	}

	hinstLibVideo = LoadLibrary(libname);

	if(hinstLibVideo != NULL)						/* Here the library is loaded successfully */
	{
		/* Get the VIDEO_GetDllInfo function address in the loaded DLL file */
		_VIDEO_GetDllInfo = (void(__cdecl *) (PLUGIN_INFO *)) GetProcAddress(hinstLibVideo, "GetDllInfo");

		if(_VIDEO_GetDllInfo != NULL)
		{
			/*~~~~~~~~~~~~~~~~~~~~*/
			PLUGIN_INFO Plugin_Info;
			/*~~~~~~~~~~~~~~~~~~~~*/

			ZeroMemory(&Plugin_Info, sizeof(Plugin_Info));

			VIDEO_GetDllInfo(&Plugin_Info);
			GfxPluginVersion = Plugin_Info.Version;

			if(Plugin_Info.Type == PLUGIN_TYPE_GFX) /* Check if this is a video plugin */
			{
				_VIDEO_DllClose = (void(__cdecl *) (void)) GetProcAddress(hinstLibVideo, "CloseDLL");
				_VIDEO_ExtraChangeResolution = (void(__cdecl *) (HWND, long, HWND)) GetProcAddress
					(
						hinstLibVideo,
						"ChangeWinSize"
					);
				_VIDEO_Test = (void(__cdecl *) (HWND)) GetProcAddress(hinstLibVideo, "DllTest");
				_VIDEO_About = (void(__cdecl *) (HWND)) GetProcAddress(hinstLibVideo, "DllAbout");
				_VIDEO_DllConfig = (void(__cdecl *) (HWND)) GetProcAddress(hinstLibVideo, "DllConfig");
				_VIDEO_MoveScreen = (void(__cdecl *) (int, int)) GetProcAddress(hinstLibVideo, "MoveScreen");
				_VIDEO_DrawScreen = (void(__cdecl *) (void)) GetProcAddress(hinstLibVideo, "DrawScreen");
				_VIDEO_ViStatusChanged = (void(__cdecl *) (void)) GetProcAddress(hinstLibVideo, "ViStatusChanged");
				_VIDEO_ViWidthChanged = (void(__cdecl *) (void)) GetProcAddress(hinstLibVideo, "ViWidthChanged");
				_VIDEO_InitiateGFX = (BOOL(__cdecl *) (GFX_INFO)) GetProcAddress(hinstLibVideo, "InitiateGFX");
				_VIDEO_RomOpen = (void(__cdecl *) (void)) GetProcAddress(hinstLibVideo, "RomOpen");
				_VIDEO_RomClosed = (void(__cdecl *) (void)) GetProcAddress(hinstLibVideo, "RomClosed");
				_VIDEO_ProcessDList = (void(__cdecl *) (void)) GetProcAddress(hinstLibVideo, "ProcessDList");
				_VIDEO_ProcessDList_Count_Cycles = (DWORD(__cdecl *) (void)) GetProcAddress(hinstLibVideo, "ProcessDListCountCycles");
				_VIDEO_UpdateScreen = (void(__cdecl *) (void)) GetProcAddress(hinstLibVideo, "UpdateScreen");
				_VIDEO_ChangeWindow = (void(__cdecl *) (int)) GetProcAddress(hinstLibVideo, "ChangeWindow");

				/* for spec 1.3 */
				_VIDEO_ChangeWindow_1_3 = (void(__cdecl *) (void)) GetProcAddress(hinstLibVideo, "ChangeWindow");
				_VIDEO_CaptureScreen = (void(__cdecl *) (char *)) GetProcAddress(hinstLibVideo, "CaptureScreen");
				_VIDEO_ProcessRDPList = (void(__cdecl *) (void)) GetProcAddress(hinstLibVideo, "ProcessRDPList");
				_VIDEO_ShowCFB = (void(__cdecl *) (void)) GetProcAddress(hinstLibVideo, "ShowCFB");

				_VIDEO_FrameBufferWriteList = (void(__cdecl *) (FrameBufferModifyEntry *, DWORD)) GetProcAddress(hinstLibVideo, "FBWList");
				_VIDEO_FrameBufferRead = (void(__cdecl *) (DWORD)) GetProcAddress(hinstLibVideo, "FBRead");
				_VIDEO_FrameBufferWrite = (void(__cdecl *) (DWORD, DWORD)) GetProcAddress(hinstLibVideo, "FBWrite");
				_VIDEO_GetFrameBufferInfo = (void(__cdecl *) (void *pinfo)) GetProcAddress(hinstLibVideo, "FBGetFrameBufferInfo");
				_VIDEO_GetFullScreenStatus = (BOOL (__cdecl *) (void)) GetProcAddress(hinstLibVideo, "GetFullScreenStatus");
				_VIDEO_SetOnScreenText = (void (__cdecl *)(char *msg)) GetProcAddress(hinstLibVideo, "SetOnScreenText");

				if( _VIDEO_FrameBufferRead == NULL && _VIDEO_FrameBufferWrite == NULL )
				{
					emustatus.VideoPluginSupportingFrameBuffer = FALSE;
				}
				else
				{
					emustatus.VideoPluginSupportingFrameBuffer = TRUE;
				}

				if( _VIDEO_GetFrameBufferInfo == NULL )
				{
					emustatus.VideoPluginProvideFrameBufferInfo = FALSE;
				}
				else
				{
					emustatus.VideoPluginProvideFrameBufferInfo = TRUE;
				}

				return(TRUE);
			}
		}
	}

	return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void VIDEO_GetDllInfo(PLUGIN_INFO *Plugin_Info)
{
	if(_VIDEO_GetDllInfo != NULL)
	{
		__try
		{
			_VIDEO_GetDllInfo(Plugin_Info);
		}

		__except(NULL, EXCEPTION_EXECUTE_HANDLER)
		{
			DisplayError("GettDllInfo Failed.");
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */

g_GFX_PluginRECT GFX_PluginRECT;

// If the plugin wants to resize the window, respect its wishes,
// but not until after the rom is loaded.
void GetPluginsResizeRequest(LPRECT lpRect)
{
	RECT RequestRect;
	GetWindowRect(gui.hwnd1964main, &RequestRect);
	
	
	if ( (RequestRect.right  != lpRect->right)  || 
	   (RequestRect.left   != lpRect->left)   ||
	   (RequestRect.top    != lpRect->top)    || 
	   (RequestRect.bottom != lpRect->bottom) )

		if ( ((RequestRect.right - RequestRect.left) > 300) && 
			 ((RequestRect.bottom - RequestRect.top) > 200) )
		{
			GFX_PluginRECT.rect.left   = RequestRect.left;
			GFX_PluginRECT.rect.right  = RequestRect.right;
			GFX_PluginRECT.rect.top    = RequestRect.top;
			GFX_PluginRECT.rect.bottom = RequestRect.bottom;
			GFX_PluginRECT.UseThis     = TRUE;
		}
}

BOOL VIDEO_InitiateGFX(GFX_INFO Gfx_Info)
{
	RECT Rect;
	
	GFX_PluginRECT.UseThis = FALSE;
	bRomIsOpened = FALSE;

	__try
	{
		if (!UsingInternalVideo) //Make this a guistatus or emustatus or something.
		{
			GetWindowRect(gui.hwnd1964main, &Rect);
			_VIDEO_InitiateGFX(Gfx_Info);
			GetPluginsResizeRequest(&Rect);
		}
		else
		{
		//	Internal_VIDEO_InitiateGFX(Gfx_Info);
		}
	}
	__except(NULL, EXCEPTION_EXECUTE_HANDLER)
	{
		/* DisplayError("Cannot Initialize Graphics"); */
	}

	return(1);	/* why not for now.. */
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
DWORD VIDEO_ProcessDList(void)
{
	int k = 100;

    _control87(_RC_NEAR|_PC_64, _MCW_RC|_MCW_PC);    

    /* try/except is handled from the call */
	
#ifndef _DEBUG
	__try
#endif
	{
		if(_VIDEO_ProcessDList_Count_Cycles != NULL && currentromoptions.RSP_RDP_Timing)
			k = _VIDEO_ProcessDList_Count_Cycles();
		else if(_VIDEO_ProcessDList != NULL) 
			_VIDEO_ProcessDList();
	}
#ifndef _DEBUG
	__except(NULL, EXCEPTION_EXECUTE_HANDLER)
	{
		DisplayError("Exception in VIDEO_ProcessDList");
		Trigger_DPInterrupt();
	}
#endif

    RestoreOldRoundingMode(((uint32) cCON31 & 0x00000003) << 8);

	if( ToCaptureScreen && CaptureScreenDirectory != NULL )
	{
		__try
		{
			_VIDEO_CaptureScreen(CaptureScreenDirectory);
		}

		__except(NULL, EXCEPTION_EXECUTE_HANDLER)
		{
			DisplayError("Exception in Capture Screen");
		}
		ToCaptureScreen = FALSE;
	}

	return k;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void VIDEO_RomOpen(void)
{
	if(_VIDEO_RomOpen != NULL)
	{
		__try
		{
			if (!UsingInternalVideo)
			{
				RECT Rect;
				GetWindowRect(gui.hwnd1964main, &Rect);
				_VIDEO_RomOpen();
				bRomIsOpened = TRUE;
				GetPluginsResizeRequest(&Rect);
			}
			else
			{
//				Internal_VIDEO_RomOpen();
			}
		}

		__except(NULL, EXCEPTION_EXECUTE_HANDLER)
		{
			DisplayError("Video RomOpen Failed.");
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void VIDEO_RomClosed(void)
{
	if(_VIDEO_RomClosed != NULL)
	{
		__try
		{
			if( bRomIsOpened )
			{
				bRomIsOpened = FALSE;
				_VIDEO_RomClosed();
			}
		}

		__except(NULL, EXCEPTION_EXECUTE_HANDLER)
		{
			DisplayError("Video RomClosed Failed.");
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void VIDEO_ChangeWindow(int window)
{
	int passed = 0;

	if(GfxPluginVersion == 0x0103)
	{
		if(_VIDEO_ChangeWindow_1_3 != NULL)
		{
			__try
			{
				_VIDEO_ChangeWindow_1_3();
				guistatus.IsFullScreen ^= 1;
				passed = 1;
			}

			__except(NULL, EXCEPTION_EXECUTE_HANDLER)
			{
				DisplayError("VIDEO ChangeWindow failed");
				passed = 0;
			}
		}
	}
	else
	{
		if(_VIDEO_ChangeWindow != NULL)
		{
			__try
			{
				_VIDEO_ChangeWindow(window);
				guistatus.IsFullScreen ^= 1;
				passed = 1;
			}

			__except(NULL, EXCEPTION_EXECUTE_HANDLER)
			{
				DisplayError("VIDEO ChangeWindow failed");
				passed = 0;
			}
		}
	}

	if( guistatus.IsFullScreen && (passed==1))
	{
		EnableWindow(gui.hToolBar, FALSE);
		ShowWindow(gui.hToolBar, SW_HIDE);
		EnableWindow(gui.hReBar, FALSE);
		ShowWindow(gui.hReBar, SW_HIDE);
		EnableWindow((HWND)gui.hMenu1964main, FALSE);
		ShowWindow((HWND)gui.hMenu1964main, FALSE);
		ShowWindow(gui.hStatusBar, SW_HIDE);
		ShowCursor(FALSE);
	}
	else
	{
		ShowWindow(gui.hReBar, SW_SHOW);
		EnableWindow(gui.hReBar, TRUE);
		EnableWindow(gui.hToolBar, TRUE);
		EnableWindow((HWND)gui.hMenu1964main, TRUE);
		ShowWindow(gui.hToolBar, SW_SHOW);
		ShowWindow(gui.hStatusBar, SW_SHOW);
		ShowWindow((HWND)gui.hMenu1964main, TRUE);
		ShowCursor(TRUE);
		DockStatusBar();
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void VIDEO_DllClose(void)
{
	if(_VIDEO_DllClose != NULL)
	{
		__try
		{
			if (!UsingInternalVideo)
			_VIDEO_DllClose();
		}

		__except(NULL, EXCEPTION_EXECUTE_HANDLER)
		{
			DisplayError("VIDEO DllClose failed");
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void CloseVideoPlugin(void)
{
	VIDEO_DllClose();

	if(hinstLibVideo) FreeLibrary(hinstLibVideo);

	hinstLibVideo = NULL;

	_VIDEO_InitiateGFX = NULL;
	_VIDEO_ProcessDList = NULL;
	_VIDEO_ProcessDList_Count_Cycles = NULL;
	_VIDEO_RomOpen = NULL;
	_VIDEO_DllClose = NULL;
	_VIDEO_DllConfig = NULL;
	_VIDEO_GetDllInfo = NULL;
	_VIDEO_UpdateScreen = NULL;
	_VIDEO_ExtraChangeResolution = NULL;

	_VIDEO_ChangeWindow = NULL;
	_VIDEO_Test = NULL;
	_VIDEO_About = NULL;
	_VIDEO_MoveScreen = NULL;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void VIDEO_DllConfig(HWND hParent)
{
	RECT Rect;

	if(_VIDEO_DllConfig != NULL)
	{
		GetWindowRect(gui.hwnd1964main, &Rect);
		_VIDEO_DllConfig(hParent);
		GetPluginsResizeRequest(&Rect);
		if (Rom_Loaded == FALSE)
		SetWindowPos(gui.hwnd1964main, NULL, Rect.left, Rect.top, 
			Rect.right-Rect.left, 
			Rect.bottom-Rect.top,
			SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	else
	{
		DisplayError("%s cannot be configured.", "Video Plugin");
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void VIDEO_About(HWND hParent)
{
	if(_VIDEO_About != NULL)
	{
		if (!UsingInternalVideo)
		_VIDEO_About(hParent);
	}
	else
	{
		DisplayError("%s: About information is not available for this plug-in.", "Video Plugin");
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void VIDEO_Test(HWND hParent)
{
	if(_VIDEO_Test != NULL)
	{
		if (!UsingInternalVideo)
			_VIDEO_Test(hParent);
	}
	else
	{
		DisplayError("%s: Test function is not available for this plug-in.", "Video Plugin");
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void VIDEO_MoveScreen(int x, int y)
{
	if(_VIDEO_MoveScreen != NULL)
	{
		if (!UsingInternalVideo)
			_VIDEO_MoveScreen(x, y);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
#include "../n64rcp.h"
void VIDEO_UpdateScreen(void)
{
	//static int recall = 0x04000000+307200*2;
	//static int k=0;


	if(_VIDEO_UpdateScreen != NULL) __try
	{
		if (!UsingInternalVideo)
		{
			_VIDEO_UpdateScreen();
		}
	}
	__except(NULL, EXCEPTION_EXECUTE_HANDLER)
	{
		DisplayError("Video UpdateScreen failed.");
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void VIDEO_DrawScreen(void)
{
	if(_VIDEO_DrawScreen != NULL) __try
	{
		if (!UsingInternalVideo)
			_VIDEO_DrawScreen();
	}

	__except(NULL, EXCEPTION_EXECUTE_HANDLER)
	{
		DisplayError("Video DrawScreen failed.");
	}

	//VIDEO_UpdateScreen();
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void VIDEO_ViStatusChanged(void)
{
	if(_VIDEO_ViStatusChanged != NULL)
	{
		__try
		{
			if (!UsingInternalVideo)
				_VIDEO_ViStatusChanged();
		}

		__except(NULL, EXCEPTION_EXECUTE_HANDLER)
		{
			DisplayError("Exception in ViStatusChanged");
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void VIDEO_ViWidthChanged(void)
{
	if(_VIDEO_ViWidthChanged != NULL)
	{
		__try
		{
			if (!UsingInternalVideo)
				_VIDEO_ViWidthChanged();
		}

		__except(NULL, EXCEPTION_EXECUTE_HANDLER)
		{
			DisplayError("Exception in ViWidthChanged");
		}
	}
}

/*
 =======================================================================================================================
    changes for spec 1.3
 =======================================================================================================================
 */
void VIDEO_CaptureScreen(char *Directory)
{
	// Call from GUI thread
	if(_VIDEO_CaptureScreen != NULL)
	{
		ToCaptureScreen = TRUE;
		CaptureScreenDirectory = Directory;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void VIDEO_ProcessRDPList(void)
{
	if(_VIDEO_ProcessRDPList != NULL)
	{
		__try
		{
			_VIDEO_ProcessRDPList();
		}

		__except(NULL, EXCEPTION_EXECUTE_HANDLER)
		{
			DisplayError("Exception in Processing RDP List");
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void VIDEO_ShowCFB(void)
{
	if(_VIDEO_ShowCFB != NULL)
	{
		__try
		{
			_VIDEO_ShowCFB();
		}

		__except(NULL, EXCEPTION_EXECUTE_HANDLER)
		{
			DisplayError("Exception in VIDEO_ShowCFB");
		}
	}
}

/*
 =======================================================================================================================
    Used when selecting plugins
 =======================================================================================================================
 */
void VIDEO_Under_Selecting_About(HWND hParent)
{
	if(_VIDEO_Under_Selecting_About != NULL)
	{
		_VIDEO_Under_Selecting_About(hParent);
	}
	else
	{
		DisplayError("%s: About information is not available for this plug-in.", "Video Plugin");
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void VIDEO_Under_Selecting_Test(HWND hParent)
{
	if(_VIDEO_Under_Selecting_Test != NULL)
	{
		_VIDEO_Under_Selecting_Test(hParent);
	}
	else
	{
		DisplayError("%s: Test function is not available for this plug-in.", "Video Plugin");
	}
}


/******************************************************************
  Function: FrameBufferWrite
  Purpose:  This function is called to notify the dll that the
            frame buffer has been modified by CPU at the given address.
  input:    addr		rdram address
			val			val
			size		1 = BYTE, 2 = WORD, 4 = DWORD
  output:   none
*******************************************************************/ 
void VIDEO_FrameBufferWrite(DWORD addr, DWORD size)
{
	if( _VIDEO_FrameBufferWrite != NULL  )
	{
		_VIDEO_FrameBufferWrite( addr, size);
	}
	//TRACE0("Write into frame buffer");
}

/******************************************************************
  Function: FrameBufferWriteList
  Purpose:  This function is called to notify the dll that the
            frame buffer has been modified by CPU at the given address.
  input:    FrameBufferModifyEntry *plist
			size = size of the plist, max = 1024
  output:   none
*******************************************************************/ 
void VIDEO_FrameBufferWriteList(FrameBufferModifyEntry *plist, DWORD size);

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
			val			val
			size		1 = BYTE, 2 = WORD, 4 = DWORD
  output:   none
*******************************************************************/ 
void VIDEO_FrameBufferRead(DWORD addr)
{
	if( _VIDEO_FrameBufferRead != NULL )
	{
		_VIDEO_FrameBufferRead( addr );
	}
	//TRACE0("Read from frame buffer");
}


/************************************************************************
Function: FBGetFrameBufferInfo
Purpose:  This function is called by the emulator core to retrieve frame
buffer information from the video plugin in order to be able
to notify the video plugin about CPU frame buffer read/write
operations

size:
= 1		byte
= 2		word (16 bit) <-- this is N64 default depth buffer format
= 4		dword (32 bit)

when frame buffer information is not available yet, set all values
in the FrameBufferInfo structure to 0

input:		FrameBufferInfo *pinfo
			pinfo is pointed to a FrameBufferInfo structure array which to be
filled in by this function
			Upto to 6 FrameBufferInfo structure
output:		Values are return in the FrameBufferInfo structure
/************************************************************************/

void VIDEO_GetFrameBufferInfo(void *pinfo)
{
	if( _VIDEO_GetFrameBufferInfo != NULL )
	{
		_VIDEO_GetFrameBufferInfo(pinfo);
	}
}



BOOL VIDEO_FrameBufferSupportRead()
{
	return _VIDEO_FrameBufferRead != NULL;
}

BOOL VIDEO_FrameBufferSupportWrite()
{
	return _VIDEO_FrameBufferWrite != NULL;
}

BOOL VIDEO_FrameBufferSupport()
{
	return VIDEO_FrameBufferSupportRead() || VIDEO_FrameBufferSupportWrite();
}

/******************************************************************
Function: GetFullScreenStatus
Purpose:  
Input:    
Output:   TRUE if current display is in full screen
FALSE if current display is in windowed mode

Attention: After the CPU core call the ChangeWindow function to request
the video plugin to switch between full screen and window mode,
the plugin may not carry out the request at the function call.
The video plugin may want to delay and do the screen switching later.

*******************************************************************/ 
int VIDEO_GetFullScreenStatus(void)
{
	if( _VIDEO_GetFullScreenStatus )
	{
		return _VIDEO_GetFullScreenStatus();
	}
	else
		return -1;
}


/******************************************************************
Function: SetOnScreenText
Purpose:  
Input:    char *msg
Output:   

*******************************************************************/ 
void VIDEO_SetOnScreenText(char *msg)
{
	if( _VIDEO_SetOnScreenText )
	{
		_VIDEO_SetOnScreenText(msg);
	}
}
