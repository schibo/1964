#include "stdafx.h"

IDirectInput8*  pDInput8 = NULL;
DInputDevice	DInput8Devices[MAX_DEVICES];

HWND hWndInput = NULL;
bool bIsDIOpened = false;
unsigned long iGetKeysCounter=0;
int totalDeviceCount=0;

DIJOYSTATE curJoystickState[4];
DIJOYSTATE initialJoystickState[4];
BYTE extraButtons[64];

void InitGlobals()
{
	memset(curJoystickState, 0, sizeof(DIJOYSTATE)*4);
	memset(initialJoystickState, 0, sizeof(DIJOYSTATE)*4);

	for(int i=0; i<MAX_DEVICES; i++)
	{
		DInput8Devices[i].pDI8Device = NULL;
		DInput8Devices[i].iType = 0;
	}
}

void Close_DI()
{
	for(int i=0; i<MAX_DEVICES; i++)
	{
		if (DInput8Devices[i].pDI8Device != NULL)
		{
			DInput8Devices[i].pDI8Device->Unacquire();
			DInput8Devices[i].pDI8Device->Release();
			DInput8Devices[i].pDI8Device = NULL;
		}
		DInput8Devices[i].iType = 0;
	}
	
	if (pDInput8)
	{
		pDInput8->Release();
		pDInput8 = NULL;
	}
	
	bIsDIOpened = false;
}

BOOL CALLBACK DIEnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	int *pnDevices = (int *)pvRef;
	int nDevices = *pnDevices;
	HRESULT hres;
	
	if(nDevices>=MAX_DEVICES)
		return DIENUM_STOP;
	
	BOOL bOk = true;
	if( LOBYTE(lpddi->dwDevType) != DI8DEVTYPE_KEYBOARD )
	{
		// This is a joystick or gamepad type device

		memcpy(&DInput8Devices[nDevices].Guid, &lpddi->guidInstance, sizeof(GUID));
		strcpy(DInput8Devices[nDevices].szName, lpddi->tszProductName);
		
		hres = pDInput8->CreateDevice(lpddi->guidInstance, &DInput8Devices[nDevices].pDI8Device, NULL);
		if (!FAILED(hres) && DInput8Devices[nDevices].pDI8Device != NULL)
		{
			hres = DInput8Devices[nDevices].pDI8Device->SetDataFormat(&c_dfDIJoystick); 
			if (FAILED(hres))
			{
				Warning("SetDataFormat : Joystick");
				bOk = false;
			}
			hres = DInput8Devices[nDevices].pDI8Device->SetCooperativeLevel(hWndInput, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
			if (FAILED(hres))
			{
				hres = DInput8Devices[nDevices].pDI8Device->SetCooperativeLevel(hWndInput, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
				if (FAILED(hres))
				{
					Warning("SetCooperativeLevel : Joystick");
					bOk = false;
				}
			}
			DIPROPRANGE diprg;
			diprg.diph.dwSize       = sizeof(diprg); 
			diprg.diph.dwHeaderSize = sizeof(diprg.diph); 
			diprg.diph.dwObj        = DIJOFS_X;
			diprg.diph.dwHow        = DIPH_BYOFFSET; 
			diprg.lMin              = STICK_LIMIT_MIN;
			diprg.lMax              = STICK_LIMIT_MAX;
			if (FAILED(DInput8Devices[nDevices].pDI8Device->SetProperty(DIPROP_RANGE, &diprg.diph)))
			{ 
				Warning("SetProperty(DIPH_RANGE_X) : Joystick");
				bOk = false;
			}
			diprg.diph.dwObj        = DIJOFS_Y;
			if (FAILED(DInput8Devices[nDevices].pDI8Device->SetProperty(DIPROP_RANGE, &diprg.diph)))
			{ 
				Warning("SetProperty(DIPH_RANGE_Y) : Joystick");
				bOk = false;
			}
			diprg.diph.dwObj        = DIJOFS_Z;
			if (FAILED(DInput8Devices[nDevices].pDI8Device->SetProperty(DIPROP_RANGE, &diprg.diph)))
			{ 
				Warning("SetProperty(DIPH_RANGE_Y) : Joystick");
				bOk = false;
			}
			DInput8Devices[nDevices].iType = DIDEVICE_JOYSTICK;
		}
		else
			Warning("CreateDevice : Joystick");
	}
	else if( LOBYTE(lpddi->dwDevType) == DI8DEVTYPE_KEYBOARD)
	{
		memcpy(&DInput8Devices[nDevices].Guid, &lpddi->guidInstance, sizeof(GUID));
		strcpy(DInput8Devices[nDevices].szName, lpddi->tszProductName);
		
		hres = pDInput8->CreateDevice(lpddi->guidInstance, &DInput8Devices[nDevices].pDI8Device, NULL);
		if (!FAILED(hres) && DInput8Devices[nDevices].pDI8Device != NULL)
		{
			hres = DInput8Devices[nDevices].pDI8Device->SetDataFormat(&c_dfDIKeyboard); 
			if (FAILED(hres))
			{
				Warning("SetDataFormat : Keyboard");
				bOk = false;
			}
			hres = DInput8Devices[nDevices].pDI8Device->SetCooperativeLevel(hWndInput, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
			if (FAILED(hres))
			{
				hres = DInput8Devices[nDevices].pDI8Device->SetCooperativeLevel(hWndInput, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
				if (FAILED(hres))
				{
					Warning("SetCooperativeLevel : Keyboard");
					bOk = false;
				}
			}
			DInput8Devices[nDevices].iType = DIDEVICE_KEYBOARD;
		}
		else
			Warning("CreateDevice : Keyboard");
	}

	if(bOk)
	{
		DInput8Devices[nDevices].pDI8Device->Acquire();
		(*pnDevices)++;
	}
	else
	{
		DInput8Devices[nDevices].pDI8Device->Release();
		DInput8Devices[nDevices].pDI8Device = NULL;
		DInput8Devices[nDevices].iType = 0;
	}
	
	return DIENUM_CONTINUE;
}

HRESULT GetDI8DeviceState(IDirectInputDevice8* pDI8Device, int size, void* buf)
{
	HRESULT res;

again:
	res = pDI8Device->GetDeviceState(size,buf); 
	if (res == DIERR_INPUTLOST || res == DIERR_NOTACQUIRED)
	{
		res = pDI8Device->Acquire();
		if(res != DIERR_OTHERAPPHASPRIO )
		{
			if(res == DIERR_NOTINITIALIZED || res == DIERR_INVALIDPARAM)
			{
				MessageBox(hWndInput, "Input Device : NotInitialized or InvalideParam!!!", "Error", MB_OK|MB_ICONERROR);
				return res;
			}

			Sleep(1);
			goto again;
		}
	}
	return res;
}

void Open_DI(HWND hwnd)
{
	HRESULT hres;
	
	if(bIsDIOpened)
		return;
	
	Close_DI();
	
	hres = DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput8, NULL);
	if (FAILED(hres))
	{
		Warning("DirectInputCreate - couldn't initialize...");
		return;
	}
	
	int nDevices = 0;
	pDInput8->EnumDevices(DI8DEVTYPE_KEYBOARD, DIEnumDevicesCallback, (void*)&nDevices, DIEDFL_ATTACHEDONLY);
	pDInput8->EnumDevices(DI8DEVTYPE_JOYSTICK, DIEnumDevicesCallback, (void*)&nDevices, DIEDFL_ATTACHEDONLY);
	pDInput8->EnumDevices(DI8DEVTYPE_GAMEPAD, DIEnumDevicesCallback, (void*)&nDevices, DIEDFL_ATTACHEDONLY);
	pDInput8->EnumDevices(DI8DEVTYPE_1STPERSON, DIEnumDevicesCallback, (void*)&nDevices, DIEDFL_ATTACHEDONLY);
	pDInput8->EnumDevices(DI8DEVTYPE_DRIVING, DIEnumDevicesCallback, (void*)&nDevices, DIEDFL_ATTACHEDONLY);
	pDInput8->EnumDevices(DI8DEVTYPE_MOUSE, DIEnumDevicesCallback, (void*)&nDevices, DIEDFL_ATTACHEDONLY);
	if(nDevices == 0)
		MessageBox(hWndInput, "No device found", "Warning", MB_OK);

	for( int player=0; player<4; player++ )
	{
		// Read initial key values
		DInputDevice *pSelectedDevice = &DInput8Devices[N64Players[player].deviceID];
		if(pSelectedDevice->iType == DIDEVICE_JOYSTICK)
		{
			pSelectedDevice->pDI8Device->Poll();
			GetDI8DeviceState(pSelectedDevice->pDI8Device, sizeof(DIJOYSTATE), &initialJoystickState[player]);
			GetDI8DeviceState(pSelectedDevice->pDI8Device, sizeof(DIJOYSTATE), &curJoystickState[player]);
		}
	}

	
	totalDeviceCount = nDevices;
	bIsDIOpened = true;
}


bool KeyDown(BYTE *KBbuffer, BYTE* OtherBuffer,int key)
{
	if( key < 0x10000 )
	{
		return (KBbuffer[key]&0x80) ? true : false;
	}
	else if( OtherBuffer )
	{
		key -= 0x10000;
		return (OtherBuffer[key]&0x80) ? true : false;
	}
	else
	{
		return false;
	}
}
void SetButtonValues(BYTE *KBbuffer, BYTE* OtherBuffer, ControllerMap *pCtrlMap, WORD &button )
{
	if(KeyDown(KBbuffer, OtherBuffer, pCtrlMap->PadUp))		button |= U_JPAD;
	if(KeyDown(KBbuffer, OtherBuffer, pCtrlMap->PadDown))		button |= D_JPAD;
	if(KeyDown(KBbuffer, OtherBuffer, pCtrlMap->PadLeft))		button |= L_JPAD;
	if(KeyDown(KBbuffer, OtherBuffer, pCtrlMap->PadRight))		button |= R_JPAD;

	if(KeyDown(KBbuffer, OtherBuffer, pCtrlMap->ButtonA))	    button |= A_BUTTON;
	if(KeyDown(KBbuffer, OtherBuffer, pCtrlMap->ButtonB))		button |= B_BUTTON;

	if(KeyDown(KBbuffer, OtherBuffer, pCtrlMap->ButtonCUp))	button |= U_CBUTTONS;
	if(KeyDown(KBbuffer, OtherBuffer, pCtrlMap->ButtonCDown))	button |= D_CBUTTONS;
	if(KeyDown(KBbuffer, OtherBuffer, pCtrlMap->ButtonCLeft))	button |= L_CBUTTONS;
	if(KeyDown(KBbuffer, OtherBuffer, pCtrlMap->ButtonCRight))	button |= R_CBUTTONS;

	if(KeyDown(KBbuffer, OtherBuffer, pCtrlMap->ButtonStart))	button |= START_BUTTON;

	if(KeyDown(KBbuffer, OtherBuffer, pCtrlMap->TriggerLeft))	button |= L_TRIG;
	if(KeyDown(KBbuffer, OtherBuffer, pCtrlMap->TriggerRight))	button |= R_TRIG;
	if(KeyDown(KBbuffer, OtherBuffer, pCtrlMap->TriggerZ))		button |= Z_TRIG;
}

int SetAxisValue(BYTE *KBbuffer, BYTE* OtherBuffer, int left, int right, int smooth, int &value)
{
	if(smooth)
	{
		bool bDowned = false;

		if(KeyDown(KBbuffer, OtherBuffer, right))
		{
			bDowned = true;
			value += STICK_SMOOTH;
		}
		if(KeyDown(KBbuffer, OtherBuffer, left))
		{
			bDowned = true;
			value -= STICK_SMOOTH;
		}

		if(!bDowned)
		{
			if(value>0)		
			{
				if( value > STICK_SMOOTH )
					value -= STICK_SMOOTH;
				else if( value > 2 )
					value /= 2;
				else
					value = 0;
			}
			else if(value<0)	
			{
				if( value < -STICK_SMOOTH )
					value += STICK_SMOOTH;
				else if( value < -2 )
					value /= 2;
				else 
					value = 0;
			}
		}

		if(value>STICK_LIMIT_MAX) value = STICK_LIMIT_MAX;
		else if(value<STICK_LIMIT_MIN) value = STICK_LIMIT_MIN;
	}
	else
	{
		if(KeyDown(KBbuffer, OtherBuffer, left))	value = STICK_LIMIT_MIN;
		if(KeyDown(KBbuffer, OtherBuffer, right))	value = STICK_LIMIT_MAX;
		if( !KeyDown(KBbuffer, OtherBuffer, left) && !KeyDown(KBbuffer, OtherBuffer, right) )  value = 0;
	}

	return value;
}

void GetButtonsFrom(int player, BUTTONS *pKeys)
{
	ControllerMap *pCtrlMap;

	if(DInput8Devices[N64Players[player].deviceID].iType == DIDEVICE_JOYSTICK)
		pCtrlMap = &N64Players[player].Joystick;
	else if(DInput8Devices[N64Players[player].deviceID].iType == DIDEVICE_KEYBOARD)
		pCtrlMap = &N64Players[player].Keyboard;
	else
		return;

	DInputDevice *pKeyboardDevice = &DInput8Devices[0];
	DInputDevice *pSelectedDevice = &DInput8Devices[N64Players[player].deviceID];
	
	WORD		button = 0;
	signed char stick_x = 0;
	signed char stick_y = 0;
	static signed int s_curXLevel = 0;
	static signed int s_curYLevel = 0;
	
	BYTE KeyboardBuffer[256];
	memset (&KeyboardBuffer,0,sizeof(KeyboardBuffer));
	GetDI8DeviceState(pKeyboardDevice->pDI8Device, sizeof(KeyboardBuffer),(LPVOID)&KeyboardBuffer);

	if(pSelectedDevice->iType == DIDEVICE_KEYBOARD)
	{
		stick_y = SetAxisValue(KeyboardBuffer, NULL, pCtrlMap->StickDown, pCtrlMap->StickUp, pCtrlMap->SmoothStickY, s_curYLevel);
		stick_x = SetAxisValue(KeyboardBuffer, NULL, pCtrlMap->StickLeft, pCtrlMap->StickRight, pCtrlMap->SmoothStickX, s_curXLevel);
		SetButtonValues(KeyboardBuffer, NULL, pCtrlMap, button);
	}
	else
	{
		pSelectedDevice->pDI8Device->Poll();
		GetDI8DeviceState(pSelectedDevice->pDI8Device, sizeof(DIJOYSTATE), &curJoystickState[player]);
		memcpy(extraButtons, curJoystickState[player].rgbButtons, 32);
		ScanAxisAndFOVs(&curJoystickState[player], &initialJoystickState[player], extraButtons+32 );

		// todo, remap the value in struct js,
		// to support FOV, and extra axis for the purposes of buttons
		
		switch(pCtrlMap->XAxis)
		{
		case 0: stick_x = (signed char)curJoystickState[player].lX; break;
		case 1: stick_x = (signed char)-curJoystickState[player].lX; break;
		case 2: stick_x = (signed char)curJoystickState[player].lY; break;
		case 3: stick_x = (signed char)-curJoystickState[player].lY; break;
		case 4: stick_x = (signed char)curJoystickState[player].lZ; break;
		case 5: stick_x = (signed char)-curJoystickState[player].lZ; break;
		case -1:
			stick_x = SetAxisValue(KeyboardBuffer, extraButtons, pCtrlMap->StickLeft, pCtrlMap->StickRight, pCtrlMap->SmoothStickX, s_curXLevel);
			break;
		}
		switch(pCtrlMap->YAxis)
		{
		case 0: stick_y = (signed char)curJoystickState[player].lX; break;
		case 1: stick_y = (signed char)-curJoystickState[player].lX; break;
		case 2: stick_y = (signed char)curJoystickState[player].lY; break;
		case 3: stick_y = (signed char)-curJoystickState[player].lY; break;
		case 4: stick_y = (signed char)curJoystickState[player].lZ; break;
		case 5: stick_y = (signed char)-curJoystickState[player].lZ; break;
		case -1:
			stick_y = SetAxisValue(KeyboardBuffer, extraButtons, pCtrlMap->StickDown, pCtrlMap->StickUp, pCtrlMap->SmoothStickY, s_curYLevel);
			break;
		}
		
		SetButtonValues(KeyboardBuffer, extraButtons, pCtrlMap, button);
	}
	
	pKeys->button = button;
	pKeys->X_AXIS = stick_x;
	pKeys->Y_AXIS = stick_y;
}
/******************************************************************
Function: GetKeys
Purpose:  To get the current state of the controllers buttons.
input:    - Controller Number (0 to 3)
- A pointer to a BUTTONS structure to be filled with 
the controller state.
output:   none
*******************************************************************/  	
extern "C" __declspec(dllexport) void GetKeys(int player, BUTTONS * pKeys )
{
	if(pKeys == NULL)
		return;
	
	Open_DI(hWndInput);		// Create DI8 if it is not created
	
	pKeys->button = 0;
	pKeys->X_AXIS = 0;
	pKeys->Y_AXIS = 0;
	
	if(player >= 0 && player <= 3)
	{
		GetButtonsFrom(player, pKeys);
	}

	iGetKeysCounter++;
}

/******************************************************************
Function: InitiateControllers
Purpose:  This function initialises how each of the controllers 
should be handled.
input:    - The handle to the main window.
- A controller structure that needs to be filled for 
the emulator to know how to handle each controller.
output:   none
*******************************************************************/  
extern "C" __declspec(dllexport) void InitiateControllers (HWND hwnd, CONTROL N64Controllers[4])
{
	N64Controllers[0].Present = N64Controllers[1].Present = N64Controllers[2].Present = N64Controllers[3].Present = TRUE;
	N64Controllers[0].RawData = N64Controllers[1].RawData = N64Controllers[2].RawData = N64Controllers[3].RawData = FALSE;
	N64Controllers[0].Plugin  = N64Controllers[1].Plugin  = N64Controllers[2].Plugin  = N64Controllers[3].Plugin  = PLUGIN_NONE;
	
	hWndInput = hwnd;
	if(hWndInput == NULL)
		MessageBox(hWndInput, "Hum, bad news : InitiateControllers() have hwnd = NULL!!!", "Error", MB_OK|MB_ICONERROR);
}

/******************************************************************
Function: ReadController
Purpose:  To process the raw data in the pif ram that is about to
be read.
input:    - Controller Number (0 to 3) and -1 signalling end of 
processing the pif ram.
- Pointer of data to be processed.
output:   none  
note:     This function is only needed if the DLL is allowing raw
data.
*******************************************************************/
extern "C" __declspec(dllexport) void ReadController ( int Control, BYTE * Command )
{
	// Nothing yet
}

/******************************************************************
Function: ControllerCommand
Purpose:  To process the raw data that has just been sent to a 
specific controller.
input:    - Controller Number (0 to 3) and -1 signalling end of 
processing the pif ram.
- Pointer of data to be processed.
output:   none

  note:     This function is only needed if the DLL is allowing raw
  data.
  
	the data that is being processed looks like this:
	initilize controller: 01 03 00 FF FF FF 
	read controller:      01 04 01 FF FF FF FF
*******************************************************************/
extern "C" __declspec(dllexport) void ControllerCommand ( int Control, BYTE * Command)
{
  // Not used
}
