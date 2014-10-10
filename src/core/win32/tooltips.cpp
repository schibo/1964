#include "..\stdafx.h"
#include <commctrl.h>

HWND WINAPI CreateTT(HWND hwndOwner)
{
    INITCOMMONCONTROLSEX icex;
    HWND        hwndTT;
    TOOLINFO    ti;
    // Load the ToolTip class from the DLL.
    icex.dwSize = sizeof(icex);
    icex.dwICC  = ICC_BAR_CLASSES;

    if(!InitCommonControlsEx(&icex))
       return NULL;
	   
    // Create the ToolTip control.
    hwndTT = CreateWindow(TOOLTIPS_CLASS, TEXT("Hello"),
                          WS_POPUP,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, (HMENU)NULL, gui.hInst,
                          NULL);

    // Prepare TOOLINFO structure for use as tracking ToolTip.
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
    ti.hwnd   = hwndOwner;
    ti.uId    = (UINT)gui.hwnd1964main;
    ti.hinst  = gui.hInst;
    ti.lpszText  = LPSTR_TEXTCALLBACK;
    ti.rect.left = ti.rect.top = ti.rect.bottom = ti.rect.right = 0; 

    // Add the tool to the control, displaying an error if needed.
    if(!SendMessage(hwndTT,TTM_ADDTOOL,0,(LPARAM)&ti)){
        MessageBox(hwndOwner,"Couldn't create the ToolTip control.",
                   "Error",MB_OK);
        return NULL;
    }

    // Activate (display) the tracking ToolTip. Then, set a global
    // flag value to indicate that the ToolTip is active, so other
    // functions can check to see if it's visible.
    SendMessage(hwndTT,TTM_TRACKACTIVATE,(WPARAM)TRUE,(LPARAM)&ti);
//    g_bIsVisible = TRUE;

    return(hwndTT);    
}

void ProcessToolTips(LPARAM lParam)
{
    LPTOOLTIPTEXT lpttt; 
	char str[256];

    lpttt = (LPTOOLTIPTEXT) lParam; 
    lpttt->hinst = gui.hInst; 

    // Specify the resource identifier of the descriptive 
    // text for the given button. 
    switch (lpttt->hdr.idFrom) 
	{ 
		case ID_BUTTON_OPEN_ROM:
			LoadString(gui.hInst, ID_BUTTON_OPEN_ROM, str, 256);
			break;

		case ID_BUTTON_PLAY:
			LoadString(gui.hInst, ID_BUTTON_PLAY, str, 256);
			break; 

		case ID_BUTTON_PAUSE:
			LoadString(gui.hInst, ID_BUTTON_PAUSE, str, 256);
			break; 

		case ID_BUTTON_STOP:
			LoadString(gui.hInst, ID_BUTTON_STOP, str, 256);
			break; 

		case ID_BUTTON_RESET:
			LoadString(gui.hInst, ID_BUTTON_RESET, str, 256);
			break; 

		case ID_BUTTON_SETUP_PLUGINS:
			LoadString(gui.hInst, ID_BUTTON_SETUP_PLUGINS, str, 256);
			break;

		case ID_BUTTON_ROM_PROPERTIES:
			LoadString(gui.hInst, ID_BUTTON_ROM_PROPERTIES, str, 256);
            break;

		case ID_BUTTON_HELP:
			LoadString(gui.hInst, ID_BUTTON_HELP, str, 256);
            break; 

		case ID_BUTTON_HOME_PAGE:
			LoadString(gui.hInst, ID_BUTTON_HOME_PAGE, str, 256);
            break; 

		case ID_BUTTON_FULL_SCREEN:
			LoadString(gui.hInst, ID_BUTTON_FULL_SCREEN, str, 256);
            break;

		case ID_BUTTON_FRAMESKIP:
			LoadString(gui.hInst, ID_BUTTON_FRAMESKIP, str, 256);
			break; 

		case ID_BUTTON_SYNC_SPEED:
			LoadString(gui.hInst, ID_BUTTON_SYNC_SPEED, str, 256);
			break; 

		case ID_BUTTON_AUTO_CF:
			LoadString(gui.hInst, ID_BUTTON_AUTO_CF, str, 256);
			break;
		default:
			return;
    }

	memset(lpttt->szText,0,80);
	strncpy(lpttt->szText, TranslateStringByString(str), 79);
}