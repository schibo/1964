#include "stdafx.h"

DWORD IDMap[][2] = {
	{IDC_BUTTON_A,				IDC_LABEL_A },
	{IDC_BUTTON_B,				IDC_LABEL_B },
	{IDC_BUTTON_TRIGGER_Z,		IDC_LABEL_TRIGGER_Z },
	{IDC_BUTTON_TRIGGER_LEFT,	IDC_LABEL_TRIGGER_LEFT },
	{IDC_BUTTON_TRIGGER_RIGHT,	IDC_LABEL_TRIGGER_RIGHT },
	{IDC_BUTTON_START,			IDC_LABEL_START },
	{IDC_BUTTON_C_UP,			IDC_LABEL_C_UP },
	{IDC_BUTTON_C_DOWN,			IDC_LABEL_C_DOWN },
	{IDC_BUTTON_C_LEFT,			IDC_LABEL_C_LEFT },
	{IDC_BUTTON_C_RIGHT,		IDC_LABEL_C_RIGHT },
	{IDC_BUTTON_PAD_UP,			IDC_LABEL_PAD_UP },
	{IDC_BUTTON_PAD_DOWN,		IDC_LABEL_PAD_DOWN },
	{IDC_BUTTON_PAD_LEFT,		IDC_LABEL_PAD_LEFT },
	{IDC_BUTTON_PAD_RIGHT,		IDC_LABEL_PAD_RIGHT },
	{IDC_BUTTON_STICK_UP,		IDC_LABEL_STICK_UP },
	{IDC_BUTTON_STICK_DOWN,		IDC_LABEL_STICK_DOWN },
	{IDC_BUTTON_STICK_LEFT,		IDC_LABEL_STICK_LEFT },
	{IDC_BUTTON_STICK_RIGHT,	IDC_LABEL_STICK_RIGHT },
};
const int numOfIDMaps = sizeof(IDMap)/sizeof(DWORD)/2;

HWND g_hWndDlg = NULL;
bool g_scanning = false;
int  g_curPlayer = 0;

void EnableControls(BOOL flag)
{
	HWND item;
	for( int i=0; i<numOfIDMaps; i++ )
	{
		item = GetDlgItem(g_hWndDlg,IDMap[i][0]);	
		EnableWindow(item, flag);
		item = GetDlgItem(g_hWndDlg,IDMap[i][1]);	
		EnableWindow(item, flag);
	}

	item = GetDlgItem(g_hWndDlg,IDC_STICK_X);	EnableWindow(item, flag);
	item = GetDlgItem(g_hWndDlg,IDC_STICK_Y);	EnableWindow(item, flag);
	item = GetDlgItem(g_hWndDlg,IDC_CHECK_SMOOTHX);	EnableWindow(item, flag);
	item = GetDlgItem(g_hWndDlg,IDC_CHECK_SMOOTHY);	EnableWindow(item, flag);
	item = GetDlgItem(g_hWndDlg,IDC_AXIS_X_LABEL);	EnableWindow(item, flag);
	item = GetDlgItem(g_hWndDlg,IDC_AXIS_Y_LABEL);	EnableWindow(item, flag);
	item = GetDlgItem(g_hWndDlg,IDC_BUTTON_DEFAULT);	EnableWindow(item, flag);
}

void UpdateItemText(ControllerMap *controlMap, int idx)
{
	HWND item = GetDlgItem(g_hWndDlg,IDMap[idx][1]);	
	int key = controlMap->buttons[idx];

	char label[64];
	if( key >= 0x10000 )
		GetButtonName(key-0x10000, label);
	else
		GetKeyName(key, label);

	SetWindowText(item, label);
}

void UpdateAllItemText()
{
	SendDlgItemMessage( g_hWndDlg, IDC_DEVICES, CB_SETCURSEL, N64Players[g_curPlayer].deviceID+1, 0 );

	if( N64Players[g_curPlayer].deviceID >= 0 )
	{
		EnableControls(TRUE);
		ControllerMap *controlMap;
		if( DInput8Devices[N64Players[g_curPlayer].deviceID].iType == DIDEVICE_KEYBOARD )
			controlMap = &(N64Players[g_curPlayer].Keyboard);
		else
			controlMap = &(N64Players[g_curPlayer].Joystick);

		HWND item;


		for( int i=0; i<numOfIDMaps; i++ )
		{
			UpdateItemText(controlMap, i);
		}

		SendDlgItemMessage( g_hWndDlg, IDC_CHECK_SMOOTHX, BM_SETCHECK, controlMap->SmoothStickX ? (LPARAM)BST_CHECKED : (LPARAM)BST_UNCHECKED, NULL );
		SendDlgItemMessage( g_hWndDlg, IDC_CHECK_SMOOTHY, BM_SETCHECK, controlMap->SmoothStickY ? (LPARAM)BST_CHECKED : (LPARAM)BST_UNCHECKED, NULL );


		if( DInput8Devices[N64Players[g_curPlayer].deviceID].iType == DIDEVICE_KEYBOARD )
		{
			item= GetDlgItem(g_hWndDlg,IDC_STICK_X);		EnableWindow(item, FALSE);
			item= GetDlgItem(g_hWndDlg,IDC_AXIS_X_LABEL);	EnableWindow(item, FALSE);
			item= GetDlgItem(g_hWndDlg,IDMap[16][0]);		EnableWindow(item, TRUE);
			item= GetDlgItem(g_hWndDlg,IDMap[16][1]);		EnableWindow(item, TRUE);
			item= GetDlgItem(g_hWndDlg,IDMap[17][0]);		EnableWindow(item, TRUE);
			item= GetDlgItem(g_hWndDlg,IDMap[17][1]);		EnableWindow(item, TRUE);
			item= GetDlgItem(g_hWndDlg,IDC_CHECK_SMOOTHX);	EnableWindow(item, TRUE);

			item= GetDlgItem(g_hWndDlg,IDC_STICK_Y);		EnableWindow(item, FALSE);
			item= GetDlgItem(g_hWndDlg,IDC_AXIS_Y_LABEL);	EnableWindow(item, FALSE);
			item= GetDlgItem(g_hWndDlg,IDMap[14][0]);		EnableWindow(item, TRUE);
			item= GetDlgItem(g_hWndDlg,IDMap[14][1]);		EnableWindow(item, TRUE);
			item= GetDlgItem(g_hWndDlg,IDMap[15][0]);		EnableWindow(item, TRUE);
			item= GetDlgItem(g_hWndDlg,IDMap[15][1]);		EnableWindow(item, TRUE);
			item= GetDlgItem(g_hWndDlg,IDC_CHECK_SMOOTHY);	EnableWindow(item, TRUE);
		}
		else
		{
			item= GetDlgItem(g_hWndDlg,IDC_STICK_X);		EnableWindow(item, TRUE);
			item= GetDlgItem(g_hWndDlg,IDC_AXIS_X_LABEL);	EnableWindow(item, TRUE);
			item= GetDlgItem(g_hWndDlg,IDC_STICK_Y);		EnableWindow(item, TRUE);
			item= GetDlgItem(g_hWndDlg,IDC_AXIS_Y_LABEL);	EnableWindow(item, TRUE);
			SendDlgItemMessage( g_hWndDlg, IDC_STICK_X, CB_SETCURSEL, controlMap->XAxis, 0 );
			SendDlgItemMessage( g_hWndDlg, IDC_STICK_Y, CB_SETCURSEL, controlMap->YAxis, 0 );

			BOOL flag = controlMap->XAxis < 6 ? FALSE : TRUE;
			item= GetDlgItem(g_hWndDlg,IDMap[16][0]);		EnableWindow(item, flag);
			item= GetDlgItem(g_hWndDlg,IDMap[16][1]);		EnableWindow(item, flag);
			item= GetDlgItem(g_hWndDlg,IDMap[17][0]);		EnableWindow(item, flag);
			item= GetDlgItem(g_hWndDlg,IDMap[17][1]);		EnableWindow(item, flag);
			item= GetDlgItem(g_hWndDlg,IDC_CHECK_SMOOTHX);	EnableWindow(item, flag);

			flag = controlMap->YAxis < 6 ? FALSE : TRUE;
			item= GetDlgItem(g_hWndDlg,IDMap[14][0]);		EnableWindow(item, flag);
			item= GetDlgItem(g_hWndDlg,IDMap[14][1]);		EnableWindow(item, flag);
			item= GetDlgItem(g_hWndDlg,IDMap[15][0]);		EnableWindow(item, flag);
			item= GetDlgItem(g_hWndDlg,IDMap[15][1]);		EnableWindow(item, flag);
			item= GetDlgItem(g_hWndDlg,IDC_CHECK_SMOOTHY);	EnableWindow(item, flag);
		}
	}
	else
	{
		EnableControls(FALSE);
	}
}



__int32 ButtonScanning(LONG idButton)
{
	IDirectInputDevice8*	pDevice = NULL;
	IDirectInput7*  		pInput = NULL;
	HRESULT hres;
	__int32 button = -1;

	HWND item = GetDlgItem(g_hWndDlg, idButton);
	EnableWindow(item, FALSE);

	hres = DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&pInput, NULL);
	if (FAILED(hres))
	{
		Warning("DirectInputCreate - couldn't initialize...");
		return -1;
	}

	hres = pInput->CreateDevice(DInput8Devices[N64Players[g_curPlayer].deviceID].Guid, (LPDIRECTINPUTDEVICEA*)&pDevice, NULL);

	if (FAILED(hres))
	{
		Warning("CreateDevice : Joystick");
		::EnableWindow(item, TRUE);
		pInput->Release();
		return -1;
	}

	if (pDevice) pDevice->Acquire();
	hres = pDevice->SetDataFormat(&c_dfDIJoystick);

	if (FAILED(hres))
	{
		Warning("SetDataFormat : Joystick");
		::EnableWindow(item, TRUE);
		pDevice->Unacquire();
		pDevice->Release();
		pInput->Release();
		return -1;
	}

	hres = pDevice->SetCooperativeLevel(g_hWndDlg, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
	if (FAILED(hres))
	{
		hres = pDevice->SetCooperativeLevel(g_hWndDlg, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE); 
		if (FAILED(hres))
		{
			Warning("SetCooperativeLevel : Joystick");
			EnableWindow(item, TRUE);
			pDevice->Unacquire();
			pDevice->Release();
			pInput->Release();
			return -1;
		}
	}

	DIPROPRANGE diprg;
	diprg.diph.dwSize       = sizeof(diprg); 
	diprg.diph.dwHeaderSize = sizeof(diprg.diph); 
	diprg.diph.dwObj        = DIJOFS_X;
	diprg.diph.dwHow        = DIPH_BYOFFSET; 
	diprg.lMin              = STICK_LIMIT_MIN;
	diprg.lMax              = STICK_LIMIT_MAX;
	if (FAILED(pDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{ 
		Warning("SetProperty(DIPH_RANGE_X) : Joystick");
	}
	diprg.diph.dwObj        = DIJOFS_Y;
	if (FAILED(pDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{ 
		Warning("SetProperty(DIPH_RANGE_Y) : Joystick");
	}
	diprg.diph.dwObj        = DIJOFS_Z;
	if (FAILED(pDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{ 
		Warning("SetProperty(DIPH_RANGE_Y) : Joystick");
	}

	DIJOYSTATE js;
	DIJOYSTATE js_initial;

	memset (&js_initial, 0, sizeof(js_initial));
	memset (&js, 0, sizeof(js));
	pDevice->Poll();
	GetDI8DeviceState(pDevice, sizeof(DIJOYSTATE), &js_initial);

	js.rgdwPOV[0] = 0xFFFFFFFF;
	js.rgdwPOV[1] = 0xFFFFFFFF;
	js.rgdwPOV[2] = 0xFFFFFFFF;
	js.rgdwPOV[3] = 0xFFFFFFFF;

	bool find = false;
	int count=0;

	LARGE_INTEGER CurrentTime;
	LARGE_INTEGER LastTime;
	LARGE_INTEGER Freq;
	LARGE_INTEGER Elapsed;

	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&LastTime);
	g_scanning = true;

	while(g_scanning && !find )
	{
		MSG msg;
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE ))
		{
			GetMessage(&msg, NULL, 0, 0);
			DispatchMessage(&msg);
		}

		pDevice->Poll();
		QueryPerformanceCounter(&CurrentTime);
		Elapsed.QuadPart = CurrentTime.QuadPart - LastTime.QuadPart;
		double timeElapsed = double(Elapsed.QuadPart)/double(Freq.QuadPart);
		if( timeElapsed > 5 )
			break;

		GetDI8DeviceState(pDevice, sizeof(DIJOYSTATE), &js);

		// Scan the buttons
		int i;
		for( i=0; i<32; i++)
		{
			if(js.rgbButtons[i] & 0x80 && js.rgbButtons[i] != js_initial.rgbButtons[i] )
			{
				button = i+0x10000;
				find = true;
				break;
			}
		}

		if( !find )
		{
			BYTE buttons[32];
			ScanAxisAndFOVs(&js, &js_initial, buttons );
			for( i=0; i<32; i++ )
			{
				if( buttons[i]&0x80 )
				{
					button = i+32+0x10000;
					find = true;
					break;
				}
			}
		}

		if( !find )
		{
			// Scan keyboard
			button = ScanKey(g_hWndDlg);
			if( button != -1 )
				find = true;
		}
	}

	pDevice->Unacquire();
	pDevice->Release();
	pInput->Release();

	EnableWindow(item, TRUE);

	g_scanning = false;
	return button;
}

__int32 KeyScanning(LONG idButton)
{
	int key;
	HWND item = GetDlgItem(g_hWndDlg, idButton);
	EnableWindow(item, FALSE);
	g_scanning = true;

	LARGE_INTEGER CurrentTime;
	LARGE_INTEGER LastTime;
	LARGE_INTEGER Freq;
	LARGE_INTEGER Elapsed;

	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&LastTime);

	do
	{
		MSG msg;
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE ))
		{
			GetMessage(&msg, NULL, 0, 0);
			//char str[256];
			//sprintf(str,"Msg %08X: %08X, %08X", msg.message, msg.wParam, msg.lParam);
			//DebuggerMsgToEmuCore(str);
			DispatchMessage(&msg);
		}

		QueryPerformanceCounter(&CurrentTime);
		Elapsed.QuadPart = CurrentTime.QuadPart - LastTime.QuadPart;
		double timeElapsed = double(Elapsed.QuadPart)/double(Freq.QuadPart);
		if( timeElapsed > 5 )
			break;

		Sleep(1);
		key = ScanKey(g_hWndDlg);
	}
	while(g_scanning && key==-1);

	::EnableWindow(item, TRUE);

	g_scanning = false;
	return key;
}

void MapAButton(DWORD UID)
{
	if( g_scanning )
		return;

	int idx = -1;
	for( int i=0; i<numOfIDMaps; i++ )
	{
		if( IDMap[i][0] == UID )
		{
			idx = i;
			break;
		}
	}

	if( idx < 0 )
		return;

	// Get the N64 controller number
	int player = g_curPlayer;
	//int player = SendDlgItemMessage( g_hWndDlg, IDC_N64_CONTROLLER, CB_GETCURSEL, 0, 0 );

	// Scan buttons and keys
	int key;
	ControllerMap *controller;
	if( DInput8Devices[N64Players[g_curPlayer].deviceID].iType == DIDEVICE_KEYBOARD )
	{
		key = KeyScanning(UID);
		controller = &N64Players[player].Keyboard;
	}
	else
	{
		key = ButtonScanning(UID);
		controller = &N64Players[player].Joystick;
	}

	if( key >= 0 )
	{
		clearConflictKeyMap(controller, key);
		controller->buttons[idx] = key;
		UpdateAllItemText();
	}
}

void ScanAxisAndFOVs(DIJOYSTATE *js, DIJOYSTATE *js_initial, BYTE buttons[32] )
{
	// Scan all Axis and sliders
	DWORD *axis = (DWORD*)js;
	DWORD *axis_initial = (DWORD*)js_initial;

	memset(buttons, 0, 32);

	int btnIdx = 0;
	int i;

	for( i=0; i<3; i++)
	{
		int x = (char)axis[i];
		int x0 = (char)axis_initial[i];
		if( abs(x-x0) > 30 )
		{
			if( x < -60 )
			{
				buttons[i*2+1] = 0x80;
			}
			else if( x > 60 )
			{
				buttons[i*2] = 0x80;
			}
		}
	}

	for( i=3; i<8; i++)
	{
		if( (axis[i] > axis_initial[i] && axis[i] -  axis_initial[i] > 0x2000) ||
			(axis_initial[i] > axis[i] && axis_initial[i] -  axis[i] > 0x2000) )
		{
			if( axis[i] > 0xC000 )
			{
				buttons[i*2+1] = 0x80;
			}
			else if( axis[i] < 0x4000 )
			{
				buttons[i*2] = 0x80;
			}
		}
	}


	// Scan the POV
	for( int i=0; i<4; i++ )
	{
		WORD val = (WORD)js->rgdwPOV[i];
		if( val != 0xFFFF && js->rgdwPOV[i] != js_initial->rgdwPOV[i] )
		{
			if (( val > 31500 ) || ( val < 4500 ))
				buttons[16+i*4+0] = 0x80;
			if (( val > 4500 ) && ( val < 13500 ))
				buttons[16+i*4+3] = 0x80;
			if (( val > 13500 ) && ( val < 22500 ))
				buttons[16+i*4+1] = 0x80;
			if (( val > 22500 ) && ( val < 31500 ))
				buttons[16+i*4+2] = 0x80;

			break;
		}
	}

}

char *buttonNames[] = {
	"X-",		"X+",
		"Y-",		"Y+",
		"Z-",		"Z+",
		"X R-",		"X R+",
		"Y R-",		"Y R+",
		"Z R-",		"Z R+",
		"S1-",			"S1+",
		"S2-",			"S2+",
		"POV1 ^",		"POV1 v",
		"POV1 <",		"POV1 >",
		"POV2 ^",		"POV2 v",
		"POV2 <",		"POV2 >",
		"POV3 ^",		"POV3 v",
		"POV3 <",		"POV3 >",
		"POV4 ^",		"POV4 v",
		"POV4 <",		"POV4 >",

};
void GetButtonName(int button, char *label)
{
	if( button < 32 )
	{
		sprintf(label, "Btn %2d", button);
	}
	else
	{
		sprintf(label, "%s", buttonNames[button-32]);
	}
}

void GetKeyName(int key, char* label)
{
	switch(key)
	{
	case DIK_ESCAPE: sprintf(label, "K_ESC"); break;
	case DIK_1: sprintf(label, "K_1"); break;
	case DIK_2: sprintf(label, "K_2"); break;
	case DIK_3: sprintf(label, "K_3"); break;
	case DIK_4: sprintf(label, "K_4"); break;
	case DIK_5: sprintf(label, "K_5"); break;
	case DIK_6: sprintf(label, "K_6"); break;
	case DIK_7: sprintf(label, "K_7"); break;
	case DIK_8: sprintf(label, "K_8"); break;
	case DIK_9: sprintf(label, "K_9"); break;
	case DIK_0: sprintf(label, "K_0"); break;
	case DIK_MINUS: sprintf(label, "K_-"); break;
	case DIK_EQUALS: sprintf(label, "K_="); break;
	case DIK_BACK: sprintf(label, "K_BACK"); break;
	case DIK_TAB: sprintf(label, "K_TAB"); break;
	case DIK_Q: sprintf(label, "K_Q"); break;
	case DIK_W: sprintf(label, "K_W"); break;
	case DIK_E: sprintf(label, "K_E"); break;
	case DIK_R: sprintf(label, "K_R"); break;
	case DIK_T: sprintf(label, "K_T"); break;
	case DIK_Y: sprintf(label, "K_Y"); break;
	case DIK_U: sprintf(label, "K_U"); break;
	case DIK_I: sprintf(label, "K_I"); break;
	case DIK_O: sprintf(label, "K_O"); break;
	case DIK_P: sprintf(label, "K_P"); break;
	case DIK_LBRACKET: sprintf(label, "K_["); break;
	case DIK_RBRACKET: sprintf(label, "K_]"); break;
	case DIK_RETURN: sprintf(label, "K_RETURN"); break;
	case DIK_LCONTROL: sprintf(label, "K_LCTRL"); break;
	case DIK_A: sprintf(label, "K_A"); break;
	case DIK_S: sprintf(label, "K_S"); break;
	case DIK_D: sprintf(label, "K_D"); break;
	case DIK_F: sprintf(label, "K_F"); break;
	case DIK_G: sprintf(label, "K_G"); break;
	case DIK_H: sprintf(label, "K_H"); break;
	case DIK_J: sprintf(label, "K_J"); break;
	case DIK_K: sprintf(label, "K_K"); break;
	case DIK_L: sprintf(label, "K_L"); break;
	case DIK_SEMICOLON: sprintf(label, "K_;"); break;
	case DIK_APOSTROPHE: sprintf(label, "APOSTROPHE"); break;
	case DIK_GRAVE: sprintf(label, "K_GRAVE"); break;
	case DIK_LSHIFT: sprintf(label, "K_LSHIFT"); break;
	case DIK_BACKSLASH: sprintf(label, "K_\\"); break;
	case DIK_Z: sprintf(label, "K_Z"); break;
	case DIK_X: sprintf(label, "K_X"); break;
	case DIK_C: sprintf(label, "K_C"); break;
	case DIK_V: sprintf(label, "K_V"); break;
	case DIK_B: sprintf(label, "K_B"); break;
	case DIK_N: sprintf(label, "K_N"); break;
	case DIK_M: sprintf(label, "K_M"); break;
	case DIK_COMMA: sprintf(label, "K_,"); break;
	case DIK_PERIOD: sprintf(label, "K_."); break;
	case DIK_SLASH: sprintf(label, "K_/"); break;
	case DIK_RSHIFT: sprintf(label, "K_RSHIFT"); break;
	case DIK_MULTIPLY: sprintf(label, "K_*"); break;
	case DIK_LMENU: sprintf(label, "K_LMENU"); break;
	case DIK_SPACE: sprintf(label, "K_SPACE"); break;
	case DIK_CAPITAL: sprintf(label, "K_CAP"); break;
	case DIK_F1: sprintf(label, "K_F1"); break;
	case DIK_F2: sprintf(label, "K_F2"); break;
	case DIK_F3: sprintf(label, "K_F3"); break;
	case DIK_F4: sprintf(label, "K_F4"); break;
	case DIK_F5: sprintf(label, "K_F5"); break;
	case DIK_F6: sprintf(label, "K_F6"); break;
	case DIK_F7: sprintf(label, "K_F7"); break;
	case DIK_F8: sprintf(label, "K_F8"); break;
	case DIK_F9: sprintf(label, "K_F9"); break;
	case DIK_F10: sprintf(label, "DIKF_10"); break;
	case DIK_SCROLL: sprintf(label, "SCROLL"); break;
	case DIK_NUMLOCK: sprintf(label, "NUMLOCK"); break;
	case DIK_NUMPAD7: sprintf(label, "NUMPAD7"); break;
	case DIK_NUMPAD8: sprintf(label, "NUMPAD8"); break;
	case DIK_NUMPAD9: sprintf(label, "NUMPAD9"); break;
	case DIK_NUMPAD4: sprintf(label, "NUMPAD4"); break;
	case DIK_NUMPAD5: sprintf(label, "NUMPAD5"); break;
	case DIK_NUMPAD6: sprintf(label, "NUMPAD6"); break;
	case DIK_NUMPAD1: sprintf(label, "NUMPAD1"); break;
	case DIK_NUMPAD2: sprintf(label, "NUMPAD2"); break;
	case DIK_NUMPAD3: sprintf(label, "NUMPAD3"); break;
	case DIK_NUMPAD0: sprintf(label, "NUMPAD0"); break;
	case DIK_DECIMAL: sprintf(label, "NUMPAD."); break;
	case DIK_ADD: sprintf(label, "NUMPAD+"); break;
	case DIK_SUBTRACT: sprintf(label, "NUMPAD-"); break;
	case DIK_OEM_102: sprintf(label, "OEM_102"); break;
	case DIK_F11: sprintf(label, "K_F11"); break;
	case DIK_F12: sprintf(label, "K_F12"); break;
	case DIK_F13: sprintf(label, "K_F13"); break;
	case DIK_F14: sprintf(label, "K_F14"); break;
	case DIK_F15: sprintf(label, "K_F15"); break;
	case DIK_KANA: sprintf(label, "K_KANA"); break;
	case DIK_ABNT_C1: sprintf(label, "K_ADNT_C1"); break;
	case DIK_CONVERT: sprintf(label, "K_CONVERT"); break;
	case DIK_NOCONVERT: sprintf(label, "NOCONVERT"); break;
	case DIK_YEN: sprintf(label, "K_YEN"); break;
	case DIK_ABNT_C2: sprintf(label, "K_ABNT_C2"); break;
	case DIK_NUMPADEQUALS: sprintf(label, "NUMPADEQUALS"); break;
	case DIK_PREVTRACK: sprintf(label, "PREVTRACK"); break;
	case DIK_AT: sprintf(label, "K_AT"); break;
	case DIK_COLON: sprintf(label, "K_COLON"); break;
	case DIK_UNDERLINE: sprintf(label, "UNDERLINE"); break;
	case DIK_KANJI: sprintf(label, "K_KANJI"); break;
	case DIK_STOP: sprintf(label, "K_STOP"); break;
	case DIK_AX: sprintf(label, "K_AX"); break;
	case DIK_UNLABELED: sprintf(label, "UNLABELED"); break;
	case DIK_NEXTTRACK: sprintf(label, "NEXTTRACK"); break;
	case DIK_NUMPADENTER: sprintf(label, "NUMPADENTER"); break;
	case DIK_RCONTROL: sprintf(label, "K_RCONTROL"); break;
	case DIK_MUTE: sprintf(label, "K_MUTE"); break;
	case DIK_CALCULATOR: sprintf(label, "CALC"); break;
	case DIK_PLAYPAUSE: sprintf(label, "PLAYPAUSE"); break;
	case DIK_MEDIASTOP: sprintf(label, "MEDIASTOP"); break;
	case DIK_VOLUMEDOWN: sprintf(label, "VOLDOWN"); break;
	case DIK_VOLUMEUP: sprintf(label, "VOLUP"); break;
	case DIK_WEBHOME: sprintf(label, "WEBHOME"); break;
	case DIK_NUMPADCOMMA: sprintf(label, "NUMPAD,"); break;
	case DIK_DIVIDE: sprintf(label, "K_DIVIDE"); break;
	case DIK_SYSRQ: sprintf(label, "K_SYSRQ"); break;
	case DIK_RMENU: sprintf(label, "K_RMENU"); break;
	case DIK_PAUSE: sprintf(label, "K_PAUSE"); break;
	case DIK_HOME: sprintf(label, "K_HOME"); break;
	case DIK_UP: sprintf(label, "K_UP"); break;
	case DIK_PRIOR: sprintf(label, "K_PRIOR"); break;
	case DIK_LEFT: sprintf(label, "K_LEFT"); break;
	case DIK_RIGHT: sprintf(label, "K_RIGHT"); break;
	case DIK_END: sprintf(label, "K_END"); break;
	case DIK_DOWN: sprintf(label, "K_DOWN"); break;
	case DIK_NEXT: sprintf(label, "K_NEXT"); break;
	case DIK_INSERT: sprintf(label, "INSERT"); break;
	case DIK_DELETE: sprintf(label, "DELETE"); break;
	case DIK_LWIN: sprintf(label, "LWIN"); break;
	case DIK_RWIN: sprintf(label, "RWIN"); break;
	case DIK_APPS: sprintf(label, "APPS"); break;
	case DIK_POWER: sprintf(label, "POWER"); break;
	case DIK_SLEEP: sprintf(label, "SLEEP"); break;
	case DIK_WAKE: sprintf(label, "WAKE"); break;
	case DIK_WEBSEARCH: sprintf(label, "WEBSEARCH"); break;
	case DIK_WEBFAVORITES: sprintf(label, "WEBFAV"); break;
	case DIK_WEBREFRESH: sprintf(label, "WEBREFRESH"); break;
	case DIK_WEBSTOP: sprintf(label, "WEBSTOP"); break;
	case DIK_WEBFORWARD: sprintf(label, "WEBFORWARD"); break;
	case DIK_WEBBACK: sprintf(label, "WEBBACK"); break;
	case DIK_MYCOMPUTER: sprintf(label, "MYCOMPUTER"); break;
	case DIK_MAIL: sprintf(label, "MAIL"); break;
	case DIK_MEDIASELECT: sprintf(label, "MEDIASELECT"); break;
	case 0: sprintf(label, "---", key); break;
	default:
		sprintf(label, "K_(%3d)", key);
		break;
	}
}

void clearConflictKeyMap(ControllerMap* controller, int key)
{
	if( controller->StickUp			== key )	controller->StickUp			 = 0;
	if( controller->StickDown		== key )	controller->StickDown		 = 0;
	if( controller->StickLeft		== key )	controller->StickLeft		 = 0;
	if( controller->StickRight		== key )	controller->StickRight		 = 0;
	if( controller->PadUp			== key )	controller->PadUp			 = 0;
	if( controller->PadDown			== key )	controller->PadDown			 = 0;
	if( controller->PadLeft			== key )	controller->PadLeft			 = 0;
	if( controller->PadRight		== key )	controller->PadRight		 = 0;
	if( controller->ButtonA			== key )	controller->ButtonA			 = 0;
	if( controller->ButtonB			== key )	controller->ButtonB			 = 0;
	if( controller->ButtonCUp		== key )	controller->ButtonCUp		 = 0;
	if( controller->ButtonCDown		== key )	controller->ButtonCDown		 = 0;
	if( controller->ButtonCLeft		== key )	controller->ButtonCLeft		 = 0;
	if( controller->ButtonCRight	== key )	controller->ButtonCRight	 = 0;
	if( controller->TriggerLeft		== key )	controller->TriggerLeft		 = 0;
	if( controller->TriggerRight	== key )	controller->TriggerRight	 = 0;
	if( controller->TriggerZ		== key )	controller->TriggerZ		 = 0;
	if( controller->ButtonStart		== key )	controller->ButtonStart			 = 0;
}

int ScanKey(HWND hwnd)
{
	IDirectInputDevice8*	pDevice = NULL;
	IDirectInput8*  		pInput = NULL;
	HRESULT hres;

	hres = DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pInput, NULL);
	if (FAILED(hres))
	{
		Warning("DirectInputCreate - couldn't initialize...");
		return -1;
	}

	hres = pInput->CreateDevice(GUID_SysKeyboard, &pDevice, NULL); 
	if (FAILED(hres))
	{
		Warning("CreateDevice : Keyboard");
		pInput->Release();
		return -1;
	}
	hres = pDevice->SetDataFormat(&c_dfDIKeyboard); 
	if (FAILED(hres))
	{
		Warning("SetDataFormat : Keyboard");
		pDevice->Unacquire();
		pDevice->Release();
		pInput->Release();
		return -1;
	}
	hres = pDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hres))
	{
		hres = pDevice->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hres))
		{
			Warning("SetCooperativeLevel : Keyboard");
			pDevice->Unacquire();
			pDevice->Release();
			pInput->Release();
			return FALSE;
		}
	}

	if (pDevice) pDevice->Acquire();

	int		key = -1;
	char	buffer[256]; 

	memset (&buffer,0,sizeof(buffer));
	GetDI8DeviceState(pDevice, sizeof(buffer),(LPVOID)&buffer);

	for(int i=0; i<256; i++)
	{
		if( (buffer[i] & 0x80 ))
		{
			key = i;
			break;
		}
	}

	pDevice->Unacquire();
	pDevice->Release();
	pInput->Release();

	return key;
}

BOOL CALLBACK GUIDlgProc( HWND hWndDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message) 
	{ 
	case WM_INITDIALOG:
		g_hWndDlg = hWndDlg;
		g_scanning = false;
		UpdateAllItemText();
		SendDlgItemMessage( hWndDlg, IDC_N64_CONTROLLER, CB_RESETCONTENT, 0, 0 );
		SendDlgItemMessage( hWndDlg, IDC_N64_CONTROLLER, CB_ADDSTRING, 0, (LPARAM)"N64 Controller 1" );
		SendDlgItemMessage( hWndDlg, IDC_N64_CONTROLLER, CB_ADDSTRING, 0, (LPARAM)"N64 Controller 2" );
		SendDlgItemMessage( hWndDlg, IDC_N64_CONTROLLER, CB_ADDSTRING, 0, (LPARAM)"N64 Controller 3" );
		SendDlgItemMessage( hWndDlg, IDC_N64_CONTROLLER, CB_ADDSTRING, 0, (LPARAM)"N64 Controller 4" );
		SendDlgItemMessage( hWndDlg, IDC_N64_CONTROLLER, CB_SETCURSEL, 0, 0 );

		SendDlgItemMessage( hWndDlg, IDC_DEVICES, CB_RESETCONTENT, 0, 0 );
		SendDlgItemMessage( hWndDlg, IDC_DEVICES, CB_ADDSTRING, 0, (LPARAM)"Not plugged in" );
		for(int i=0; i<MAX_DEVICES; i++)
		{
			if( DInput8Devices[i].pDI8Device != NULL )
			{
				SendDlgItemMessage( hWndDlg, IDC_DEVICES, CB_ADDSTRING, 0, (LPARAM)DInput8Devices[i].szName );
			}
		}
		SendDlgItemMessage( hWndDlg, IDC_DEVICES, CB_SETCURSEL, N64Players[0].deviceID+1, 0 );

		SendDlgItemMessage( hWndDlg, IDC_STICK_X, CB_RESETCONTENT, 0, 0 );
		SendDlgItemMessage( hWndDlg, IDC_STICK_X, CB_ADDSTRING, 0, (LPARAM)"X Axis" );
		SendDlgItemMessage( hWndDlg, IDC_STICK_X, CB_ADDSTRING, 0, (LPARAM)"Inversed X-Axis" );
		SendDlgItemMessage( hWndDlg, IDC_STICK_X, CB_ADDSTRING, 0, (LPARAM)"Y Axis" );
		SendDlgItemMessage( hWndDlg, IDC_STICK_X, CB_ADDSTRING, 0, (LPARAM)"Inversed Y-Axis" );
		SendDlgItemMessage( hWndDlg, IDC_STICK_X, CB_ADDSTRING, 0, (LPARAM)"Z Axis" );
		SendDlgItemMessage( hWndDlg, IDC_STICK_X, CB_ADDSTRING, 0, (LPARAM)"Inversed Z-Axis" );
		SendDlgItemMessage( hWndDlg, IDC_STICK_X, CB_ADDSTRING, 0, (LPARAM)"Use Buttons" );
		SendDlgItemMessage( hWndDlg, IDC_STICK_X, CB_SETCURSEL, N64Players[0].Joystick.XAxis, 0 );

		SendDlgItemMessage( hWndDlg, IDC_STICK_Y, CB_RESETCONTENT, 0, 0 );
		SendDlgItemMessage( hWndDlg, IDC_STICK_Y, CB_ADDSTRING, 0, (LPARAM)"X Axis" );
		SendDlgItemMessage( hWndDlg, IDC_STICK_Y, CB_ADDSTRING, 0, (LPARAM)"Inversed X-Axis" );
		SendDlgItemMessage( hWndDlg, IDC_STICK_Y, CB_ADDSTRING, 0, (LPARAM)"Y Axis" );
		SendDlgItemMessage( hWndDlg, IDC_STICK_Y, CB_ADDSTRING, 0, (LPARAM)"Inversed Y-Axis" );
		SendDlgItemMessage( hWndDlg, IDC_STICK_Y, CB_ADDSTRING, 0, (LPARAM)"Z Axis" );
		SendDlgItemMessage( hWndDlg, IDC_STICK_Y, CB_ADDSTRING, 0, (LPARAM)"Inversed Z-Axis" );
		SendDlgItemMessage( hWndDlg, IDC_STICK_Y, CB_ADDSTRING, 0, (LPARAM)"Use Buttons" );
		SendDlgItemMessage( hWndDlg, IDC_STICK_Y, CB_SETCURSEL, N64Players[0].Joystick.YAxis, 0 );

		return TRUE;

	case WM_COMMAND: 
		switch (LOWORD(wParam)) 
		{ 
		case IDC_BUTTON_A:
		case IDC_BUTTON_B:
		case IDC_BUTTON_TRIGGER_Z:
		case IDC_BUTTON_TRIGGER_LEFT:
		case IDC_BUTTON_TRIGGER_RIGHT:
		case IDC_BUTTON_START:
		case IDC_BUTTON_C_UP:
		case IDC_BUTTON_C_DOWN:
		case IDC_BUTTON_C_LEFT:
		case IDC_BUTTON_C_RIGHT:
		case IDC_BUTTON_PAD_UP:
		case IDC_BUTTON_PAD_DOWN:
		case IDC_BUTTON_PAD_LEFT:
		case IDC_BUTTON_PAD_RIGHT:
		case IDC_BUTTON_STICK_UP:
		case IDC_BUTTON_STICK_DOWN:
		case IDC_BUTTON_STICK_LEFT:
		case IDC_BUTTON_STICK_RIGHT:
			MapAButton(LOWORD(wParam));
			return TRUE;
		case IDC_CHECK_SMOOTHX:
			{
				ControllerMap *controlMap;
				if( DInput8Devices[N64Players[g_curPlayer].deviceID].iType == DIDEVICE_KEYBOARD )
					controlMap = &(N64Players[g_curPlayer].Keyboard);
				else
					controlMap = &(N64Players[g_curPlayer].Joystick);

				controlMap->SmoothStickX = 
					(SendDlgItemMessage( g_hWndDlg, IDC_CHECK_SMOOTHX, BM_GETCHECK, NULL, NULL ) == BST_CHECKED);
			}
			return TRUE;
		case IDC_CHECK_SMOOTHY:
			{
				ControllerMap *controlMap;
				if( DInput8Devices[N64Players[g_curPlayer].deviceID].iType == DIDEVICE_KEYBOARD )
					controlMap = &(N64Players[g_curPlayer].Keyboard);
				else
					controlMap = &(N64Players[g_curPlayer].Joystick);

				controlMap->SmoothStickY = 
					(SendDlgItemMessage( g_hWndDlg, IDC_CHECK_SMOOTHY, BM_GETCHECK, NULL, NULL ) == BST_CHECKED);
			}
			return TRUE;
		case IDC_N64_CONTROLLER:
			g_curPlayer = SendDlgItemMessage( g_hWndDlg, IDC_N64_CONTROLLER, CB_GETCURSEL, 0, 0 );
			UpdateAllItemText();
			break;
		case IDC_DEVICES:
			N64Players[g_curPlayer].deviceID = SendDlgItemMessage( g_hWndDlg, IDC_DEVICES, CB_GETCURSEL, 0, 0 );
			N64Players[g_curPlayer].deviceID--;
			UpdateAllItemText();
			break;
		case IDC_STICK_X:
			{
				ControllerMap *controlMap;
				if( DInput8Devices[N64Players[g_curPlayer].deviceID].iType == DIDEVICE_KEYBOARD )
					controlMap = &(N64Players[g_curPlayer].Keyboard);
				else
					controlMap = &(N64Players[g_curPlayer].Joystick);

				controlMap->XAxis = SendDlgItemMessage( g_hWndDlg, IDC_STICK_X, CB_GETCURSEL, 0, 0 );
				UpdateAllItemText();
			}
			break;
		case IDC_STICK_Y:
			{
				ControllerMap *controlMap;
				if( DInput8Devices[N64Players[g_curPlayer].deviceID].iType == DIDEVICE_KEYBOARD )
					controlMap = &(N64Players[g_curPlayer].Keyboard);
				else
					controlMap = &(N64Players[g_curPlayer].Joystick);

				controlMap->YAxis = SendDlgItemMessage( g_hWndDlg, IDC_STICK_Y, CB_GETCURSEL, 0, 0 );
				UpdateAllItemText();
			}
			break;
		case IDC_BUTTON_DEFAULT:
			if( DInput8Devices[N64Players[g_curPlayer].deviceID].iType == DIDEVICE_KEYBOARD )
			{
				SetDefaultKeyboardMap(&(N64Players[g_curPlayer].Keyboard));
			}
			else
			{
				SetDefaultJoystickMap(&(N64Players[g_curPlayer].Joystick));
			}
			UpdateAllItemText();
			return TRUE;

		case IDOK: 
			g_scanning = false;
			EndDialog( hWndDlg, wParam );
			WriteConfiguration();
			return TRUE;
		case IDCANCEL: 
			g_scanning = false;
			EndDialog( hWndDlg, wParam );
			return TRUE;
		} 
	} 
	return FALSE; 
}

void GUI_Do_Config(HWND hwnd)
{
	DialogBox( g_hInstance, MAKEINTRESOURCE(IDD_GUI), hwnd, (DLGPROC)GUIDlgProc );
}


