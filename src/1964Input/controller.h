/**********************************************************************************
Common Controller plugin spec, version #1.0 maintained by 
zilmar (zilmar@emulation64.com)

All questions or suggestions should go through the mailing list.
http://www.egroups.com/group/Plugin64-Dev
**********************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/* Note: BOOL, BYTE, WORD, DWORD, TRUE, FALSE are defined in windows.h */

#define PLUGIN_TYPE_RSP				1
#define PLUGIN_TYPE_GFX				2
#define PLUGIN_TYPE_AUDIO			3
#define PLUGIN_TYPE_CONTROLLER		4

/***** Structures *****/
typedef struct {
	WORD Version;        /* Should be set to 1 */
	WORD Type;           /* Set to PLUGIN_TYPE_CONTROLLER */
	char Name[100];      /* Name of the DLL */
	BOOL NormalMemory;   
	BOOL MemoryBswaped;  
} PLUGIN_INFO;

/*** Conteroller plugin's ****/
#define PLUGIN_NONE					1
#define PLUGIN_MEMPAK				2
#define PLUGIN_RUMBLE_PAK			3 // not implemeted for non raw data
#define PLUGIN_TRANSFER_PAK			4 // not implemeted for non raw data

typedef struct {
	BOOL Present;
	BOOL RawData;
	int  Plugin;
} CONTROL;

typedef union {
	DWORD Value;
	struct {
		unsigned __int16 button;

		signed char   X_AXIS;

		signed char   Y_AXIS;
	};
} BUTTONS;

/******************************************************************
  Function: CloseDLL
  Purpose:  This function is called when the emulator is closing
            down allowing the dll to de-initialise.
  input:    none
  output:   none
*******************************************************************/ 
__declspec(dllexport) void CloseDLL (void);

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
__declspec(dllexport) void ControllerCommand ( int Control, BYTE * Command);

/******************************************************************
  Function: DllAbout
  Purpose:  This function is optional function that is provided
            to give further information about the DLL.
  input:    a handle to the window that calls this function
  output:   none
*******************************************************************/ 
__declspec(dllexport) void DllAbout ( HWND hParent );

/******************************************************************
  Function: DllConfig
  Purpose:  This function is optional function that is provided
            to allow the user to configure the dll
  input:    a handle to the window that calls this function
  output:   none
*******************************************************************/ 
__declspec(dllexport) void DllConfig ( HWND hParent );

/******************************************************************
  Function: DllTest
  Purpose:  This function is optional function that is provided
            to allow the user to test the dll
  input:    a handle to the window that calls this function
  output:   none
*******************************************************************/ 
__declspec(dllexport) void DllTest ( HWND hParent );

/******************************************************************
  Function: GetDllInfo
  Purpose:  This function allows the emulator to gather information
            about the dll by filling in the PluginInfo structure.
  input:    a pointer to a PLUGIN_INFO stucture that needs to be
            filled by the function. (see def above)
  output:   none
*******************************************************************/ 
__declspec(dllexport) void GetDllInfo ( PLUGIN_INFO * PluginInfo );

/******************************************************************
  Function: GetKeys
  Purpose:  To get the current state of the controllers buttons.
  input:    - Controller Number (0 to 3)
            - A pointer to a BUTTONS structure to be filled with 
			the controller state.
  output:   none
*******************************************************************/  	
__declspec(dllexport) void GetKeys(int Control, BUTTONS * Keys );

/******************************************************************
  Function: InitiateControllers
  Purpose:  This function initialises how each of the controllers 
            should be handled.
  input:    - The handle to the main window.
            - A controller structure that needs to be filled for 
			  the emulator to know how to handle each controller.
  output:   none
*******************************************************************/  
__declspec(dllexport) void InitiateControllers (HWND hMainWindow, CONTROL Controls[4]);

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
__declspec(dllexport) void ReadController ( int Control, BYTE * Command );

/******************************************************************
  Function: RomClosed
  Purpose:  This function is called when a rom is closed.
  input:    none
  output:   none
*******************************************************************/ 
__declspec(dllexport) void RomClosed (void);

/******************************************************************
  Function: RomOpen
  Purpose:  This function is called when a rom is open. (from the 
            emulation thread)
  input:    none
  output:   none
*******************************************************************/ 
__declspec(dllexport) void RomOpen (void);

/******************************************************************
  Function: WM_KeyDown
  Purpose:  An optional function to pass the WM_KEYDOWN message 
            from the emulator to the plugin.
  input:    wParam and lParam of the WM_KEYDOWN message.
  output:   none
*******************************************************************/ 
__declspec(dllexport) void WM_KeyDown( WPARAM wParam, LPARAM lParam );

/******************************************************************
  Function: WM_KeyUp
  Purpose:  An optional function to pass the WM_KEYUP message 
            from the emulator to the plugin.
  input:    wParam and lParam of the WM_KEYDOWN message.
  output:   none
*******************************************************************/ 
__declspec(dllexport) void WM_KeyUp( WPARAM wParam, LPARAM lParam );

#if defined(__cplusplus)
}
#endif

