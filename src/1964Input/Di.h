#ifndef _DI_H
#define _DI_H

void Close_DI();
void Open_DI(HWND hwnd);
bool IsOpen_DI();
void InitGlobals();

extern IDirectInput8*  			pDInput8;

#define DIDEVICE_KEYBOARD 1
#define DIDEVICE_JOYSTICK 2

typedef struct
{
	IDirectInputDevice8*	pDI8Device;
	GUID Guid;
	int iType;
	char szName[MAX_PATH];
} DInputDevice;

#define MAX_DEVICES 20
extern DInputDevice DInput8Devices[MAX_DEVICES];

extern unsigned long iGetKeysCounter;

extern HWND hWndInput;
extern HINSTANCE g_hInstance;

#endif //_DI_H
