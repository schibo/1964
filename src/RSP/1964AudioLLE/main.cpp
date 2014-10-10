#include <windows.h>
#include <commctrl.h>
#include "AudioSpec.h"
#include "AudioCode.h"
//#include "audiohle.h"
#include "make/resource.h"


#define PLUGIN_VERSION "2.6"

// New Plugin Specification

extern "C" { char *pRDRAM;  } //merged
extern "C" { char *pDMEM;   } //merged
extern "C" { char *pIMEM;   } //merged
extern "C" { extern int remainingBytes;}
AUDIO_INFO AudioInfo;
AudioCode  snd;
DWORD Dacrate = 0;
void ResetSoundAtRuntime (void);



// Dialogs

extern int SyncSpeed;
extern int ReverseStereo;
HWND hWnd=NULL;
BOOL CALLBACK DeleteItemProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
	switch (message) 
    { 
		case WM_ACTIVATE:
			CheckDlgButton(hwndDlg, 
				IDC_SYNC, 
				SyncSpeed);
			CheckDlgButton(hwndDlg, 
				IDC_REVERSE_STEREO, 
				ReverseStereo);

			break;
	
		case WM_COMMAND: 
            switch (LOWORD(wParam)) 
            { 
                case IDOK: 
//                    if (!GetDlgItemText(hwndDlg, ID_ITEMNAME, 
  //                           szItemName, 80)) 
    //                     *szItemName=0; 
 
                    // Fall through. 
 
                case IDCANCEL: 
                    EndDialog(hwndDlg, wParam); 
					DestroyWindow(hWnd);
					hWnd = NULL;
                    return TRUE; 

				case IDC_ABOUT:
					DllAbout(hWnd);
					break;

            }
		break;
		case WM_NOTIFY:
		switch(LOWORD(wParam))
		{
			case IDC_SYNC:
			if(	SyncSpeed
				!= ( SendDlgItemMessage( hwndDlg, IDC_SYNC, BM_GETCHECK, 0, 0)
				== BST_CHECKED))
			{
				HANDLE hMutex = CreateMutex(NULL,FALSE,NULL);
				WaitForSingleObject (hMutex, INFINITE);
					
				SyncSpeed = ( SendDlgItemMessage( hwndDlg, IDC_SYNC, BM_GETCHECK, 0, 0)	== BST_CHECKED);

				ResetSoundAtRuntime();			
				ReleaseMutex(hMutex);

				//	REGISTRY_WriteDWORD( "AutoFullScreen", emuoptions.auto_full_screen);
			}
			break;
			case IDC_REVERSE_STEREO:
			if(	ReverseStereo
				!= ( SendDlgItemMessage( hwndDlg, IDC_REVERSE_STEREO, BM_GETCHECK, 0, 0)
				== BST_CHECKED))
			{
				HANDLE hMutex = CreateMutex(NULL,FALSE,NULL);
				WaitForSingleObject (hMutex, INFINITE);
					
				ReverseStereo = ( SendDlgItemMessage( hwndDlg, IDC_REVERSE_STEREO, BM_GETCHECK, 0, 0)	== BST_CHECKED);

				ReleaseMutex(hMutex);

				//	REGISTRY_WriteDWORD( "AutoFullScreen", emuoptions.auto_full_screen);
			}
			break;			
		}
		break;
    } 
    return FALSE; 
} 

EXPORT void CALL DllAbout ( HWND hParent ){
	char Scratch[700];
	char Caption[0x80];
	strcpy(Caption, "About 1964 Audio version ");
	strcat(Caption, PLUGIN_VERSION);
	LoadString(AudioInfo.hinst, IDS_ABOUT, Scratch, 700);
	MessageBox (hParent, Scratch, Caption, MB_OK);
}


EXPORT void CALL DllTest ( HWND hParent ){
	MessageBox (hParent, "Test!", "Test!", MB_OK);
}

// Initialization / Deinitalization Functions

// Note: We call CloseDLL just in case the audio plugin was already initialized...
void AiCallBack (DWORD n) {}

extern "C" void rsp_reset();
EXPORT BOOL CALL InitiateAudio (AUDIO_INFO Audio_Info){
	CloseDLL ();
	memcpy (&AudioInfo, &Audio_Info, sizeof(AUDIO_INFO));
	snd.Initialize (AiCallBack, AudioInfo.hwnd);

	pIMEM  = (char*)Audio_Info.IMEM; //merged
	pRDRAM = (char*)Audio_Info.RDRAM; //merged
	pDMEM  = (char*)Audio_Info.DMEM; //merged
	rsp_reset();

	return TRUE;
}

EXPORT void CALL DllConfig ( HWND hParent ){

	if (hWnd == NULL)
	{
	hWnd = CreateDialog(AudioInfo.hinst, MAKEINTRESOURCE(IDD_DIALOG1), AudioInfo.hwnd, (DLGPROC)DeleteItemProc);
	ShowWindow(hWnd, SW_SHOW);
	}
}


EXPORT void CALL CloseDLL (void){
	//ChangeABI (0);
	snd.DeInitialize ();
}

EXPORT void CALL GetDllInfo ( PLUGIN_INFO * PluginInfo ){
	PluginInfo->MemoryBswaped = TRUE;
	PluginInfo->NormalMemory  = FALSE;
	strcpy (PluginInfo->Name, "1964 Audio version ");
	strcat (PluginInfo->Name, PLUGIN_VERSION);
#ifdef RSP_DECOMPILER 
	strcat (PluginInfo->Name, " PRIVATE DECOMPILER");
#endif
	PluginInfo->Type = PLUGIN_TYPE_AUDIO;
	PluginInfo->Version = 0x0101;
}

extern "C" void rsp_run();
EXPORT void CALL ProcessAList(void){
#ifdef ENABLE_TRACE_COMPARE
	rsp_run_with_trace();
#else
	rsp_run ();
#endif
}




EXPORT void CALL RomClosed (void){
	snd.StopAudio ();
	Dacrate = 0;
	snd.Initialize(AiCallBack, AudioInfo.hwnd);
}

void ResetSoundAtRuntime (void){
	snd.StopAudio ();
//	Dacrate = 0;
	snd.Initialize(AiCallBack, AudioInfo.hwnd);
	//StartAudio();
	snd.StartAudio();
}

extern "C" int gUcode;
extern "C" int dwBytes1, int dwBytes2;

void UpdateFifoFlag()
{
	if (gUcode == UNDEFINED_UCODE)
		return;
	
	if (gUcode == 1)
	{
		*AudioInfo.AI_STATUS_REG |= AI_STATUS_FIFO_FULL;

		if (remainingBytes <= MAXBUFFER-LOCK_SIZE) {
			*AudioInfo.AI_STATUS_REG &= ~AI_STATUS_FIFO_FULL;
			*AudioInfo.MI_INTR_REG |= MI_INTR_AI;
			AudioInfo.CheckInterrupts();
		}
	}
	else
	{
		

		if (*AudioInfo.AI_LEN_REG <= MAXBUFFER-LOCK_SIZE) {
			*AudioInfo.AI_STATUS_REG &= ~AI_STATUS_FIFO_FULL;
			*AudioInfo.MI_INTR_REG |= MI_INTR_AI;
			AudioInfo.CheckInterrupts();
		}
		else
			*AudioInfo.AI_STATUS_REG |= AI_STATUS_FIFO_FULL;

	}
}


EXPORT void CALL AiDacrateChanged (int  SystemType) {
	DWORD Frequency;
	if (Dacrate != *AudioInfo.AI_DACRATE_REG) {
		Dacrate = *AudioInfo.AI_DACRATE_REG;
		if (Dacrate == 0) Dacrate = 1;
		switch (SystemType) {
			case SYSTEM_NTSC: Frequency = 48681812 / Dacrate; break;
			case SYSTEM_PAL:  Frequency = 49656530 / Dacrate; break;
			case SYSTEM_MPAL: Frequency = 48628316 / Dacrate; break;
		}
		snd.SetFrequency (Frequency);
	}
}

EXPORT void CALL AiLenChanged (void){
	DWORD retVal;
	
	if (gUcode == UNDEFINED_UCODE)
	{
	//	*AudioInfo.AI_STATUS_REG &= ~AI_STATUS_FIFO_FULL;
	//			*AudioInfo.MI_INTR_REG |= MI_INTR_AI;

		return;
	}

	if (*AudioInfo.AI_LEN_REG == 0) return;

//	if (gUcode != 4)
//		*AudioInfo.AI_STATUS_REG |= AI_STATUS_FIFO_FULL;

	UpdateFifoFlag();
	retVal = snd.AddBuffer (
		(AudioInfo.RDRAM + (*AudioInfo.AI_DRAM_ADDR_REG & 0x00FFFFF8)), 
		*AudioInfo.AI_LEN_REG & 0x3FFF8);


}



EXPORT DWORD CALL AiReadLength (void){
	
	*AudioInfo.AI_LEN_REG = snd.GetReadStatus ();

	UpdateFifoFlag();

	return *AudioInfo.AI_LEN_REG;
}

// Deprecated Functions

EXPORT void CALL AiUpdate (BOOL Wait){
	
	if (gUcode == UNDEFINED_UCODE)
		return;

//	UpdateFifoFlag();

	if (Wait)
		WaitMessage ();
}
