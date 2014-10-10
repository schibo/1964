// 1964Input.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

HINSTANCE g_hInstance = NULL;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
	g_hInstance = hinstDLL;

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		ReadConfiguration();
	}
	else if( dwReason == DLL_PROCESS_DETACH )
	{
		Close_DI();
	}
	return TRUE;
}

bool g_bRomIsOpened = false;
bool IsRomOpened()
{
	return g_bRomIsOpened;
}

/******************************************************************
  Function: GetDllInfo
  Purpose:  This function allows the emulator to gather information
    about the dll by filling in the PluginInfo structure.
  input:    a pointer to a PLUGIN_INFO stucture that needs to be
    filled by the function. (see def above)
  output:   none
*******************************************************************/ 
extern "C" __declspec(dllexport) void GetDllInfo ( PLUGIN_INFO * PluginInfo )
{
	if(PluginInfo)
	{
		char str[256];
#ifdef _DEBUG
		LoadString(g_hInstance, IDS_VERSION_DEBUG, str, 256);
#else
		LoadString(g_hInstance, IDS_VERSION, str, 256);
#endif
		strncpy(PluginInfo->Name, str, sizeof(PluginInfo->Name));
		PluginInfo->Version = 0x0100;
		PluginInfo->Type = PLUGIN_TYPE_CONTROLLER;
		PluginInfo->NormalMemory   = FALSE;
		PluginInfo->MemoryBswaped  = TRUE;
	}
}

/******************************************************************
  Function: CloseDLL
  Purpose:  This function is called when the emulator is closing
    down allowing the dll to de-initialise.
  input:    none
  output:   none
*******************************************************************/ 
extern "C" __declspec(dllexport) void CloseDLL (void)
{
	Close_DI();
}

/******************************************************************
  Function: DllAbout
  Purpose:  This function is optional function that is provided
    to give further information about the DLL.
  input:    a handle to the window that calls this function
  output:   none
*******************************************************************/ 
extern "C" __declspec(dllexport) void DllAbout ( HWND hParent )
{
	MessageBox(hParent,"1964 Input Plugin 1.0","About" ,MB_OK);
}

/******************************************************************
  Function: DllConfig
  Purpose:  This function is optional function that is provided
    to allow the user to configure the dll
  input:    a handle to the window that calls this function
  output:   none
*******************************************************************/ 
extern "C" __declspec(dllexport) void DllConfig ( HWND hParent )
{
	Open_DI(hParent);
	GUI_Do_Config(hParent);
	Close_DI();
}

/******************************************************************
  Function: DllTest
  Purpose:  This function is optional function that is provided
    to allow the user to test the dll
  input:    a handle to the window that calls this function
  output:   none
*******************************************************************/ 
extern "C" __declspec(dllexport) void DllTest ( HWND hParent )
{
	MessageBox(hParent,"Test function is not implemented","Test" ,MB_OK);
}

/******************************************************************
  Function: RomOpen
  Purpose:  This function is called when a rom is open. (from the 
    emulation thread)
  input:    none
  output:   none
*******************************************************************/ 
extern "C" __declspec(dllexport) void RomOpen (void)
{
	g_bRomIsOpened = true;
	Open_DI(hWndInput);
	iGetKeysCounter = 0;
	
}

/******************************************************************
  Function: RomClosed
  Purpose:  This function is called when a rom is closed.
  input:    none
  output:   none
*******************************************************************/ 
extern "C" __declspec(dllexport) void RomClosed (void)
{
	Close_DI();
	g_bRomIsOpened = false;
}

/******************************************************************
  Function: WM_KeyDown
  Purpose:  An optional function to pass the WM_KEYDOWN message 
            from the emulator to the plugin.
  input:    wParam and lParam of the WM_KEYDOWN message.
  output:   none
*******************************************************************/ 
extern "C" __declspec(dllexport) void WM_KeyDown( WPARAM wParam, LPARAM lParam )
{
}


/******************************************************************
  Function: WM_KeyUp
  Purpose:  An optional function to pass the WM_KEYUP message 
            from the emulator to the plugin.
  input:    wParam and lParam of the WM_KEYDOWN message.
  output:   none
*******************************************************************/ 
extern "C" __declspec(dllexport) void WM_KeyUp( WPARAM wParam, LPARAM lParam )
{
}

#ifdef _DEBUG
void (__cdecl *_DebuggerMsgCallBackFunc) (char *msg) = NULL;
extern "C" __declspec(dllexport) void SetDebuggerCallBack(void (_cdecl *DbgCallBackFun)(char *msg))
{
	_DebuggerMsgCallBackFunc = DbgCallBackFun;
}

void DebuggerMsgToEmuCore(char *msg)
{
	if( _DebuggerMsgCallBackFunc )
	{
		_DebuggerMsgCallBackFunc(msg);
	}
}
#endif