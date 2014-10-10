#include <stdafx.h>

PeripheralTyp N64Players[4];

#define READ_AS_KEYBOARD 0
#define READ_AS_JOYSTICK 1


#define KEY_1964 "Software\\1964emu\\1964Input_10"
#define KEY_Controller "Controller%d"
#define KEY_ControllerJoy "ControllerJoy%d"
#define KEY_DIDevice "DIDevice"
#define KEY_StickRight "StickRight"
#define KEY_StickLeft "StickLeft"
#define KEY_StickUp "StickUp"
#define KEY_StickDown "StickDown"
#define KEY_StickLeftRight "StickLeftRight"
#define KEY_StickUpDown "StickUpDown"
#define KEY_SmoothStickLeftRight "SmoothStickX"
#define KEY_SmoothStickUpDown "SmoothStickY"
#define KEY_TriggerLeft "TriggerLeft"
#define KEY_TriggerRight "TriggerRight"
#define KEY_TriggerZ "TriggerZ"
#define KEY_ButtonA "ButtonA"
#define KEY_ButtonB "ButtonB"
#define KEY_ButtonStart "ButtonStart"
#define KEY_ButtonUp "ButtonUp"
#define KEY_ButtonDown "ButtonDown"
#define KEY_ButtonLeft "ButtonLeft"
#define KEY_ButtonRight "ButtonRight"
#define KEY_PadUp "PadUp"
#define KEY_PadDown "PadDown"
#define KEY_PadLeft "PadLeft"
#define KEY_PadRight "PadRight"

#define LABEL_Disable "Disable"
#define LABEL_N64Controller1 "N64 Controller 1"
#define LABEL_N64Controller2 "N64 Controller 2"
#define LABEL_N64Controller3 "N64 Controller 3"
#define LABEL_N64Controller4 "N64 Controller 4"
#define LABEL_Spectator "Spectator"
#define LABEL_JoystickAxeX "X Axis"
#define LABEL_JoystickInversedAxeX "Inversed X-Axis"
#define LABEL_JoystickAxeY "Y-Axis"
#define LABEL_JoystickInversedAxeY "Inversed Y-Axis"
#define LABEL_JoystickAxeZ "Z-Axis"
#define LABEL_JoystickInversedAxeZ "Inversed Z-Axis"
#define LABEL_UseButtons "Use Buttons"
#define LABEL_Unused "(unused)"

void SetDefaultKeyboardMap(ControllerMap *controller)
{
	controller->ButtonA = DIK_LSHIFT;
	controller->ButtonB = DIK_LCONTROL;
	
	controller->ButtonStart = DIK_SPACE;
	
	controller->ButtonCUp = DIK_NUMPAD8;
	controller->ButtonCDown = DIK_NUMPAD2;
	
	controller->ButtonCLeft = DIK_NUMPAD4;
	controller->ButtonCRight = DIK_NUMPAD6;
	
	controller->PadUp = DIK_HOME;
	controller->PadDown = DIK_END;
	controller->PadLeft = DIK_DELETE;
	controller->PadRight = DIK_NEXT;
	
	controller->StickRight = DIK_RIGHT;
	controller->StickLeft = DIK_LEFT;
	controller->SmoothStickX = FALSE;

	controller->StickUp = DIK_UP;
	controller->StickDown = DIK_DOWN;
	controller->SmoothStickY = FALSE;

	controller->TriggerZ = DIK_Z;
	controller->TriggerLeft = DIK_A;
	controller->TriggerRight = DIK_S;
}

void SetDefaultJoystickMap(ControllerMap *controller)
{
	controller->ButtonA = 0|0x10000;
	controller->ButtonB = 1|0x10000;
	controller->TriggerZ = 2|0x10000;
	
	controller->ButtonCUp = 3|0x10000;
	controller->ButtonCDown = 4|0x10000;
	controller->ButtonCLeft = 5|0x10000;
	controller->ButtonCRight = 6|0x10000;

	controller->ButtonStart = 7|0x10000;

	controller->TriggerLeft = 8|0x10000;
	controller->TriggerRight = 9|0x10000;
	
	controller->PadUp = 10|0x10000;
	controller->PadDown = 11|0x10000;
	controller->PadLeft = 12|0x10000;
	controller->PadRight = 13|0x10000;
	
	controller->YAxis = 3;
	controller->XAxis = 0;
	
	controller->StickUp = 0;
	controller->StickDown = 0;
	controller->SmoothStickY = FALSE;
	
	controller->StickLeft = 0;
	controller->StickRight = 0;
	controller->SmoothStickX = FALSE;
	
}

void SetDefaultController()
{
	N64Players[0].deviceID = -1;
	N64Players[1].deviceID = -1;
	N64Players[2].deviceID = -1;
	N64Players[3].deviceID = -1;
}

void RegReadValue(HKEY hKey, char *name, int &variable )
{
	char szData[MAX_PATH];
	DWORD	rc;
	DWORD cbData, dwType;

	cbData = sizeof(szData);
	rc = RegQueryValueEx(	hKey, name, NULL, &dwType, (LPBYTE)szData, &cbData);
    if (rc == ERROR_SUCCESS && cbData != 0)
		variable = atoi(szData);
}

void ReadDevice(ControllerMap *controller, HKEY hKey)
{
    SetDefaultKeyboardMap(controller);

	RegReadValue(hKey, KEY_StickUpDown, controller->YAxis);
	RegReadValue(hKey, KEY_StickLeftRight, controller->XAxis);
	
	RegReadValue(hKey, KEY_StickUp, controller->StickUp);
	RegReadValue(hKey, KEY_StickDown, controller->StickDown);
	RegReadValue(hKey, KEY_SmoothStickUpDown, controller->SmoothStickY);
	RegReadValue(hKey, KEY_StickLeft, controller->StickLeft);
	RegReadValue(hKey, KEY_StickRight, controller->StickRight);
	RegReadValue(hKey, KEY_SmoothStickLeftRight, controller->SmoothStickX);
	
	RegReadValue(hKey, KEY_TriggerLeft, controller->TriggerLeft);
	RegReadValue(hKey, KEY_TriggerRight, controller->TriggerRight);
	RegReadValue(hKey, KEY_TriggerZ, controller->TriggerZ);
	
	RegReadValue(hKey, KEY_ButtonA, controller->ButtonA);
	RegReadValue(hKey, KEY_ButtonB, controller->ButtonB);
	RegReadValue(hKey, KEY_ButtonStart, controller->ButtonStart);
	
	RegReadValue(hKey, KEY_ButtonUp, controller->ButtonCUp);
	RegReadValue(hKey, KEY_ButtonDown, controller->ButtonCDown);
	RegReadValue(hKey, KEY_ButtonLeft, controller->ButtonCLeft);
	RegReadValue(hKey, KEY_ButtonRight, controller->ButtonCRight);
	
	RegReadValue(hKey, KEY_PadUp, controller->PadUp);
	RegReadValue(hKey, KEY_PadDown, controller->PadDown);
	RegReadValue(hKey, KEY_PadLeft, controller->PadLeft);
	RegReadValue(hKey, KEY_PadRight, controller->PadRight);
}

void RegWriteValue(HKEY hKey, char *name, int value)
{
	char temp[256];
	sprintf(temp, "%d", value);
	RegSetValueEx(hKey, name, NULL, REG_SZ, (BYTE *)temp, sizeof(temp));
}
void WriteDevice(ControllerMap *controller, HKEY hKey)
{
	RegWriteValue(hKey, KEY_StickUpDown, controller->YAxis);
	RegWriteValue(hKey, KEY_StickLeftRight, controller->XAxis);
	
	RegWriteValue(hKey, KEY_StickUp, controller->StickUp);
	RegWriteValue(hKey, KEY_StickDown, controller->StickDown);
	RegWriteValue(hKey, KEY_SmoothStickUpDown, controller->SmoothStickY);
	
	RegWriteValue(hKey, KEY_StickLeft, controller->StickLeft);
	RegWriteValue(hKey, KEY_StickRight, controller->StickRight);
	RegWriteValue(hKey, KEY_SmoothStickLeftRight, controller->SmoothStickX);
	
	RegWriteValue(hKey, KEY_TriggerLeft, controller->TriggerLeft);
	RegWriteValue(hKey, KEY_TriggerRight, controller->TriggerRight);
	RegWriteValue(hKey, KEY_TriggerZ, controller->TriggerZ);
	
	RegWriteValue(hKey, KEY_ButtonA, controller->ButtonA);
	RegWriteValue(hKey, KEY_ButtonB, controller->ButtonB);
	RegWriteValue(hKey, KEY_ButtonStart, controller->ButtonStart);
	
	RegWriteValue(hKey, KEY_ButtonUp, controller->ButtonCUp);
	RegWriteValue(hKey, KEY_ButtonDown, controller->ButtonCDown);
	RegWriteValue(hKey, KEY_ButtonLeft, controller->ButtonCLeft);
	RegWriteValue(hKey, KEY_ButtonRight, controller->ButtonCRight);
	
	RegWriteValue(hKey, KEY_PadUp, controller->PadUp);
	RegWriteValue(hKey, KEY_PadDown, controller->PadDown);
	RegWriteValue(hKey, KEY_PadLeft, controller->PadLeft);
	RegWriteValue(hKey, KEY_PadRight, controller->PadRight);
}

#ifdef _DEBUG
void Warning(LPCSTR pszMessage)
{
	DebuggerMsgToEmuCore((char*)"1964Input Error!");
	DebuggerMsgToEmuCore((char*)pszMessage);
}
#endif

void ReadConfiguration()
{
	HKEY	hKey1, hKey2, hKey3;
	DWORD	rc;
	
	if (RegConnectRegistry(NULL, HKEY_CURRENT_USER, &hKey1) == ERROR_SUCCESS)
	{
		char	szBuffer[260];
		
		strcpy(szBuffer, KEY_1964);
		
		rc = RegOpenKeyEx(hKey1, szBuffer, 0, KEY_ALL_ACCESS, &hKey2);
		if(rc == ERROR_SUCCESS)
		{
			RegReadValue(hKey2, "Player0DeviceID", N64Players[0].deviceID);
			RegReadValue(hKey2, "Player1DeviceID", N64Players[1].deviceID);
			RegReadValue(hKey2, "Player2DeviceID", N64Players[2].deviceID);
			RegReadValue(hKey2, "Player3DeviceID", N64Players[3].deviceID);

			for(int c=0; c<4; c++)
			{
				char	szController[260];
				
				sprintf(szController, KEY_Controller, c);
				
				rc = RegOpenKeyEx(hKey2, szController, 0, KEY_ALL_ACCESS, &hKey3);
				if(rc == ERROR_SUCCESS)
                {
					ReadDevice(&N64Players[c].Keyboard, hKey3);
                }
				RegCloseKey(hKey3);
			}

			for(int c=0; c<4; c++)
			{
				char	szController[260];
				
				sprintf(szController, KEY_ControllerJoy, c);
				
				rc = RegOpenKeyEx(hKey2, szController, 0, KEY_ALL_ACCESS, &hKey3);
				if(rc == ERROR_SUCCESS)
                {
					ReadDevice(&N64Players[c].Joystick, hKey3);
                }
				RegCloseKey(hKey3);
			}
            
            RegCloseKey(hKey2);
		}
		RegCloseKey(hKey1);
	}
}

void WriteConfiguration()
{
	HKEY	hKey1, hKey2, hKey3;
	DWORD	rc;
	
	if (RegConnectRegistry(NULL, HKEY_CURRENT_USER, &hKey1) == ERROR_SUCCESS)
	{
		char	szBuffer[260];
		
		strcpy(szBuffer, KEY_1964);
		
		rc = RegOpenKeyEx(hKey1, szBuffer, 0, KEY_ALL_ACCESS, &hKey2);
		if(rc != ERROR_SUCCESS)
			rc = RegCreateKeyEx(hKey1, szBuffer, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey2, 0);
		if(rc == ERROR_SUCCESS)
		{
			RegWriteValue(hKey2, "Player0DeviceID", N64Players[0].deviceID);
			RegWriteValue(hKey2, "Player1DeviceID", N64Players[1].deviceID);
			RegWriteValue(hKey2, "Player2DeviceID", N64Players[2].deviceID);
			RegWriteValue(hKey2, "Player3DeviceID", N64Players[3].deviceID);

			for(int c=0; c<4; c++)
			{
				char	szController[260];
				
				sprintf(szController, KEY_Controller, c);
				
				rc = RegOpenKeyEx(hKey2, szController, 0, KEY_ALL_ACCESS, &hKey3);
				if(rc != ERROR_SUCCESS)
					rc = RegCreateKeyEx(hKey2, szController, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey3, 0);
				if(rc == ERROR_SUCCESS)
					WriteDevice(&N64Players[c].Keyboard, hKey3);
				
				RegCloseKey(hKey3);
			}

			for(int c=0; c<4; c++)
			{
				char	szController[260];
				
				sprintf(szController, KEY_ControllerJoy, c);
				
				rc = RegOpenKeyEx(hKey2, szController, 0, KEY_ALL_ACCESS, &hKey3);
				if(rc != ERROR_SUCCESS)
					rc = RegCreateKeyEx(hKey2, szController, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey3, 0);
				if(rc == ERROR_SUCCESS)
					WriteDevice(&N64Players[c].Joystick, hKey3);
				
				RegCloseKey(hKey3);
			}
            
            
            RegCloseKey(hKey2);
		}
		RegCloseKey(hKey1);
	}
}
