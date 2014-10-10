#ifndef _CONFIG_H
#define _CONFIG_H

#define R_JPAD			0x0001
#define L_JPAD			0x0002
#define D_JPAD			0x0004
#define U_JPAD			0x0008
#define START_BUTTON	0x0010
#define Z_TRIG			0x0020
#define B_BUTTON		0x0040
#define A_BUTTON		0x0080

#define R_CBUTTONS		0x0100
#define L_CBUTTONS		0x0200
#define D_CBUTTONS		0x0400
#define U_CBUTTONS		0x0800
#define L_TRIG			0x1000
#define R_TRIG			0x2000

#define STICK_LIMIT_MAX 127
#define STICK_LIMIT_MIN -127
#define STICK_SMOOTH	16

typedef struct
{
	// Don't modify the order of the variables below
	union {
		__int32 buttons[18];
		struct {
			__int32	ButtonA, ButtonB;
			__int32 TriggerZ;
			__int32	TriggerLeft, TriggerRight;
			__int32	ButtonStart;
			__int32	ButtonCUp, ButtonCDown, ButtonCLeft, ButtonCRight;
			__int32	PadUp, PadDown, PadLeft, PadRight;
			__int32	StickUp, StickDown;
			__int32	StickLeft, StickRight;
		};
	};

	int		YAxis;
	int		XAxis;
	int		SmoothStickY;
	int		SmoothStickX;

	int		deviceIdx;
} ControllerMap;

typedef struct PeripheralTyp
{
	int				deviceID;
    ControllerMap	Joystick;
    ControllerMap	Keyboard;
} PeripheralTyp;


extern PeripheralTyp N64Players[4];

void ReadConfiguration();
void WriteConfiguration();

void OpenConfigurationDialog(HWND hParent);

void SetDefaultController();
void ScanAxisAndFOVs(DIJOYSTATE *js, DIJOYSTATE *js_initial, BYTE buttons[32] );
void GetButtonName(int button, char *label);
void GetKeyName(int key, char* label);
void clearConflictKeyMap(ControllerMap* controller, int key);
int ScanKey(HWND hwnd);
HRESULT GetDI8DeviceState(IDirectInputDevice8* pDI8Device, int size, void* buf);
void SetDefaultJoystickMap(ControllerMap *controller);
void SetDefaultKeyboardMap(ControllerMap *controller);
void GUI_Do_Config(HWND hwnd);

#ifdef _DEBUG
extern void DebuggerMsgToEmuCore(char *msg);
extern void Warning(LPCSTR pszMessage);
#else
#define DebuggerMsgToEmuCore(msg)
#define Warning(msg)
#endif

#endif //_CONFIG_H
