#include <windows.h>
#include <dsound.h>
#include <stdio.h>
#include "audio.h"
#include "make/resource.h"

//#ifdef RSP_DECOMPILER 
//#define THREADED 1
//#endif

#define PLUGIN_VERSION "2.7"
#define UNDEFINED_UCODE 0xffffffff

#define AI_STATUS_FIFO_FULL	0x80000000		/* Bit 31: full */
#define AI_STATUS_DMA_BUSY	0x40000000		/* Bit 30: busy */
#define MI_INTR_AI			0x04			/* Bit 2: AI intr */
#define NUMCAPTUREEVENTS	3
#define BufferSize			0x2000
//int BufferSize;



#define Buffer_Empty		0
#define Buffer_Playing		1
#define Buffer_HalfFull		2
#define Buffer_Full			3

void FillBuffer            ( int buffer );
BOOL FillBufferWithSilence ( LPDIRECTSOUNDBUFFER lpDsb );
void FillSectionWithSilence( int buffer );
void SetupDSoundBuffers    ( void );
void Soundmemcpy           ( void * dest, const void * src, size_t count );
void ROM_ByteSwap_3210(void *v, DWORD dwLen);
void ROM_GetRomNameFromHeader(TCHAR * szName, ROMHeader * pHdr);

void AddEffect();

extern void rsp_run();
extern void rsp_reset();
extern void rsp_run_with_trace();

//BOOL ucodeDetected=FALSE;
char gameName[40];
HANDLE hMutex;
int SyncSpeed=1;
int ReverseStereo=0;
HANDLE handleAudioThread=NULL;
DWORD dwAudioThreadId;
int audioIsPlaying = FALSE;


DWORD Frequency, Dacrate = 0, Snd1Len, SpaceLeft, SndBuffer[3], Playing;
AUDIO_INFO AudioInfo;
BYTE *Snd1ReadPos;
extern BOOL ucodeDetected;

// ---------------- Needed for RSP --------------------------
char *pRDRAM;
char *pDMEM;
char *pIMEM;
// ---------------- Needed for RSP --------------------------

int DoOnce; //Execute boot code only once.
int MaxDumpCount;
int MinDumpCount;

LPDIRECTSOUNDBUFFER8  lpdsbuf=NULL;
LPDIRECTSOUND8        lpds;
LPDIRECTSOUNDNOTIFY8  lpdsNotify;
HANDLE               rghEvent[NUMCAPTUREEVENTS];
DSBPOSITIONNOTIFY    rgdscbpn[NUMCAPTUREEVENTS];

extern int gUcode;
EXPORT void CALL AiDacrateChanged (int SystemType) {
//	if (Dacrate != *AudioInfo.AI_DACRATE_REG) 
	{
		Dacrate = *AudioInfo.AI_DACRATE_REG;
        if (Dacrate == 0) Dacrate = 1;
		switch (SystemType) {
		case SYSTEM_NTSC: Frequency = 48681812 / (Dacrate ); break;
		case SYSTEM_PAL:  Frequency = 49656530 / (Dacrate ); break;
		case SYSTEM_MPAL: Frequency = 48628316 / (Dacrate ); break;
		}
/*
		{
			OSTask		*ptask = (OSTask*)(pDMEM + 0x0FC0);
			BufferSize = ptask->t.output_buff_size;
			if (BufferSize==0)
				BufferSize = 0x1800;
			else
				MessageBox(0, "Cool", "", 0);
		}
*/
		SetupDSoundBuffers();
	}
}

EXPORT BOOL CALL IsMusyX()
{	
	//This will only be true for MusyX. Cool!
	if (gUcode == UNDEFINED_UCODE)
	{
		return TRUE;
	}
	
	if (gUcode == 4)
		return TRUE;
	else
	{
		return FALSE;
	}


}

EXPORT void CALL AiLenChanged (void) {
	int count, offset=0, temp;
	DWORD dwStatus;

	if (!lpdsbuf) {
		*AudioInfo.AI_STATUS_REG &= ~AI_STATUS_FIFO_FULL;
		*AudioInfo.MI_INTR_REG |= MI_INTR_AI;
		AudioInfo.CheckInterrupts();
		return;}
	else if (gUcode == UNDEFINED_UCODE)
	{
		gUcode = 88;
		ucodeDetected = TRUE;
	}

//	if (gUcode == UNDEFINED_UCODE) {DetectMicrocode();  }

	if ((gUcode != 4) /*&& (gUcode != UNDEFINED_UCODE)*/)//Hack for MusyX
        *AudioInfo.AI_STATUS_REG |= AI_STATUS_FIFO_FULL; 
	
	//if (*AudioInfo.AI_LEN_REG == 0) { return; } //this breaks sound in Zelda load states!!




	Snd1Len = (*AudioInfo.AI_LEN_REG & 0x3FFF8);

	temp = Snd1Len;
	Snd1ReadPos = AudioInfo.RDRAM + (*AudioInfo.AI_DRAM_ADDR_REG & 0x00FFFFF8);
	if (Playing) {
		for (count = 0; count < 3; count ++) {
			if (SndBuffer[count] == Buffer_Playing) {
				offset = (count + 1) & 3;
			}
		}
	} else {
		offset = 0;
	}

	for (count = 0; count < 3; count ++) {
		if (SndBuffer[(count + offset) & 3] == Buffer_HalfFull) {
			FillBuffer((count + offset) & 3);
			count = 3;
		}
	}
	for (count = 0; count < 3; count ++) {
		if (SndBuffer[(count + offset) & 3] == Buffer_Full) {
			FillBuffer((count + offset + 1) & 3);
			FillBuffer((count + offset + 2) & 3);
			count = 20;
		}
	}
	if (count < 10) {
		FillBuffer((0 + offset) & 3);
		FillBuffer((1 + offset) & 3);
		FillBuffer((2 + offset) & 3);
	}
}

EXPORT DWORD CALL AiReadLength (void) {
	return Snd1Len;
}


void PlayIt()
{
int count=0;
DWORD dwStatus;

	if (!Playing) {
		for (count = 0; count < 3; count ++) {
			if (SndBuffer[count] == Buffer_Full) 
			{
				Playing = TRUE;
				IDirectSoundBuffer8_Play(lpdsbuf, 0, 0, DSBPLAY_LOOPING );
				return;
			}
		}
	} else {
		IDirectSoundBuffer8_GetStatus(lpdsbuf,&dwStatus);
		if ((dwStatus & DSBSTATUS_PLAYING) == 0) {
			IDirectSoundBuffer8_Play(lpdsbuf, 0, 0, DSBPLAY_LOOPING );
		}
	}

}

EXPORT void CALL Update (BOOL Wait) {
	DWORD dwEvt;


	if (!lpdsbuf) {
		*AudioInfo.AI_STATUS_REG &= ~AI_STATUS_FIFO_FULL;
		*AudioInfo.MI_INTR_REG |= MI_INTR_AI;
		AudioInfo.CheckInterrupts();
		return;
	}
		else if (gUcode == UNDEFINED_UCODE)
	{
		gUcode = 88;
		ucodeDetected = TRUE;
	}



	if (Wait) {
		dwEvt = MsgWaitForMultipleObjects(NUMCAPTUREEVENTS,rghEvent,FALSE,
			INFINITE,QS_ALLINPUT);
	} else {
		dwEvt = MsgWaitForMultipleObjects(NUMCAPTUREEVENTS,rghEvent,FALSE,
			0,QS_ALLINPUT);
	}
	dwEvt -= WAIT_OBJECT_0;
	if (dwEvt == NUMCAPTUREEVENTS) {
		return;
	}

	switch (dwEvt) {
	case 0: 
		SndBuffer[0] = Buffer_Empty;
		FillSectionWithSilence(0);
		SndBuffer[1] = Buffer_Playing;
		FillBuffer(2);
		FillBuffer(0);
		break;
	case 1: 
		SndBuffer[1] = Buffer_Empty;
		FillSectionWithSilence(1);
		SndBuffer[2] = Buffer_Playing;
		FillBuffer(0);
		FillBuffer(1);
		break;
	case 2: 
		SndBuffer[2] = Buffer_Empty;
		FillSectionWithSilence(2);
		SndBuffer[0] = Buffer_Playing;
		FillBuffer(1);
		FillBuffer(2);		
		break;
	}
}

HWND hWndConfig;
CRITICAL_SECTION CriticalSection;
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
					DestroyWindow(hWndConfig);
					hWndConfig = NULL;
                    return TRUE; 

				case IDC_ABOUT:
					DllAbout(AudioInfo.hwnd);
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
				int TempgUcode = gUcode;
				InitializeCriticalSection(&CriticalSection);
				SyncSpeed = ( SendDlgItemMessage( hwndDlg, IDC_SYNC, BM_GETCHECK, 0, 0)	== BST_CHECKED);
				EnterCriticalSection(&CriticalSection);
				RomClosed();
				gUcode = TempgUcode;
				LeaveCriticalSection(&CriticalSection);
				DeleteCriticalSection(&CriticalSection);


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

EXPORT void CALL DllConfig ( HWND hParent )
{
	if (hWndConfig == NULL)
	{
	hWndConfig = CreateDialog(AudioInfo.hinst, MAKEINTRESOURCE(IDD_DIALOG1), AudioInfo.hwnd, (DLGPROC)DeleteItemProc);
	ShowWindow(hWndConfig, SW_SHOW);
	}
}

CRITICAL_SECTION CriticalSection2;
EXPORT void CALL AiUpdate (BOOL Wait) 
{

#ifndef THREADED
	Update (Wait&Playing);
	PlayIt();
#endif
	

	if (SyncSpeed && Playing)
		if (
			(SndBuffer[2] == Buffer_Full)
			||(SndBuffer[1] == Buffer_Full)
			||(SndBuffer[0]==Buffer_Full)
			)
		{
			Sleep(10);

		}
}

EXPORT void CALL CloseDLL (void)
{
#ifdef THREADED
	TerminateThread (handleAudioThread, 0);
#endif

	if (lpdsbuf) { 
        IDirectSoundBuffer8_Stop(lpdsbuf);
		IDirectSoundBuffer8_Release(lpdsbuf);
        lpdsbuf = NULL;
	}
    if ( lpds ) {
		IDirectSound8_Release(lpds);
        lpds = NULL;
	}
}

void __cdecl DisplayError (char * Message, ...) {
	char Msg[400];
	va_list ap;

	va_start( ap, Message );
	vsprintf( Msg, Message, ap );
	va_end( ap );
	MessageBox(AudioInfo.hwnd,Msg,"Error",MB_OK|MB_ICONEXCLAMATION);
}
extern void PlayIt();

DWORD WINAPI AudioThreadProc (void) 
{
	//HANDLE hMutex = CreateMutex(NULL,FALSE,NULL);
	SetThreadPriority(handleAudioThread, THREAD_PRIORITY_TIME_CRITICAL);

	while (1)
	{
		EnterCriticalSection(&CriticalSection);
		Update (0);
		
		PlayIt();
		LeaveCriticalSection(&CriticalSection);
		Sleep(1);
		
	}
}

EXPORT void CALL DllAbout ( HWND hParent ) {
	char Scratch[700];
	char Caption[0x80];
	strcpy(Caption, "About 1964 Audio version ");
	strcat(Caption, PLUGIN_VERSION);
	LoadString(AudioInfo.hinst, IDS_ABOUT, Scratch, 700);
	MessageBox (hParent, Scratch, Caption, MB_OK);
}

void StartAudio () {
	if (audioIsPlaying) return;
	audioIsPlaying = TRUE;
#ifdef THREADED
		InitializeCriticalSection(&CriticalSection);
	handleAudioThread = CreateThread (NULL, NULL, (LPTHREAD_START_ROUTINE)AudioThreadProc, NULL, NULL, &dwAudioThreadId);
#endif
}


void FillBuffer ( int buffer ) {
	//void AddToBuffer (void *sndptr, DWORD sndlen);
    DWORD dwBytesLocked;
    VOID *lpvData;

	if (!audioIsPlaying)
		StartAudio();


	if (gUcode != 4)
	if (Snd1Len == 0) { 
		
		*AudioInfo.AI_STATUS_REG &= ~AI_STATUS_FIFO_FULL;
		*AudioInfo.MI_INTR_REG |= MI_INTR_AI;
		AudioInfo.CheckInterrupts();

		return; }
	if (SndBuffer[buffer] == Buffer_Empty) {
		if (Snd1Len >= BufferSize) {
			if (FAILED( IDirectSoundBuffer8_Lock(lpdsbuf, BufferSize * buffer,BufferSize, &lpvData, &dwBytesLocked,
				NULL, NULL, 0  ) ) )
			{
				IDirectSoundBuffer8_Unlock(lpdsbuf, lpvData, dwBytesLocked, NULL, 0 );
				//DisplayError("FAILED lock");
				return;
			}
			Soundmemcpy(lpvData,Snd1ReadPos,dwBytesLocked);
			//AddToBuffer (lpvData, dwBytesLocked);
			SndBuffer[buffer] = Buffer_Full;
			Snd1ReadPos += dwBytesLocked;
			Snd1Len -= dwBytesLocked;
			IDirectSoundBuffer8_Unlock(lpdsbuf, lpvData, dwBytesLocked, NULL, 0 );
		} else {
			if (FAILED( IDirectSoundBuffer8_Lock(lpdsbuf, BufferSize * buffer,Snd1Len, &lpvData, &dwBytesLocked,
				NULL, NULL, 0  ) ) )
			{
				IDirectSoundBuffer8_Unlock(lpdsbuf, lpvData, dwBytesLocked, NULL, 0 );
				//DisplayError("FAILED lock");
				return;
			}
			Soundmemcpy(lpvData,Snd1ReadPos,dwBytesLocked);
			//AddToBuffer (lpvData, dwBytesLocked);
			SndBuffer[buffer] = Buffer_HalfFull;
			Snd1ReadPos += dwBytesLocked;
			SpaceLeft = BufferSize - Snd1Len;
			Snd1Len = 0;
			IDirectSoundBuffer8_Unlock(lpdsbuf, lpvData, dwBytesLocked, NULL, 0 );
		}
	} else if (SndBuffer[buffer] == Buffer_HalfFull) {
		if (Snd1Len >= SpaceLeft) {
			if (FAILED( IDirectSoundBuffer8_Lock(lpdsbuf, (BufferSize * (buffer + 1)) - SpaceLeft ,SpaceLeft, &lpvData,
				&dwBytesLocked, NULL, NULL, 0  ) ) )
			{
				IDirectSoundBuffer8_Unlock(lpdsbuf, lpvData, dwBytesLocked, NULL, 0 );
				//DisplayError("FAILED lock");
				return;
			}
			Soundmemcpy(lpvData,Snd1ReadPos,dwBytesLocked);
			//AddToBuffer (lpvData, dwBytesLocked);
			SndBuffer[buffer] = Buffer_Full;
			Snd1ReadPos += dwBytesLocked;
			Snd1Len -= dwBytesLocked;
			IDirectSoundBuffer8_Unlock(lpdsbuf, lpvData, dwBytesLocked, NULL, 0 );
		} else {
			if (FAILED( IDirectSoundBuffer8_Lock(lpdsbuf, (BufferSize * (buffer + 1)) - SpaceLeft,Snd1Len, &lpvData, &dwBytesLocked,
				NULL, NULL, 0  ) ) )
			{
				IDirectSoundBuffer8_Unlock(lpdsbuf, lpvData, dwBytesLocked, NULL, 0 );
				//DisplayError("FAILED lock");
				return;
			}
			Soundmemcpy(lpvData,Snd1ReadPos,dwBytesLocked);
			//AddToBuffer (lpvData, dwBytesLocked);
			SndBuffer[buffer] = Buffer_HalfFull;
			Snd1ReadPos += dwBytesLocked;
			SpaceLeft = SpaceLeft - Snd1Len;
			Snd1Len = 0;
			IDirectSoundBuffer8_Unlock(lpdsbuf, lpvData, dwBytesLocked, NULL, 0 );
		}
	}

	if (gUcode != 4)
	if ((Snd1Len == 0) /*&& (gUcode != UNDEFINED_UCODE)*/) {
		*AudioInfo.AI_STATUS_REG &= ~AI_STATUS_FIFO_FULL;
		*AudioInfo.MI_INTR_REG |= MI_INTR_AI;
		AudioInfo.CheckInterrupts();
	}
}

BOOL FillBufferWithSilence( LPDIRECTSOUNDBUFFER lpDsb ) {
    WAVEFORMATEX    wfx;
    DWORD           dwSizeWritten;

    PBYTE   pb1;
    DWORD   cb1;

    if ( FAILED( IDirectSoundBuffer8_GetFormat(lpDsb, &wfx, sizeof( WAVEFORMATEX ), &dwSizeWritten ) ) ) {
        return FALSE;
	}

    if ( SUCCEEDED( IDirectSoundBuffer8_Lock(lpDsb,0,0,(LPVOID*)&pb1,&cb1,NULL,NULL,DSBLOCK_ENTIREBUFFER))) {
        FillMemory( pb1, cb1, ( wfx.wBitsPerSample == 8 ) ? 128 : 0 );

        IDirectSoundBuffer8_Unlock(lpDsb, pb1, cb1, NULL, 0 );
        return TRUE;
    }

    return FALSE;
}

void FillSectionWithSilence( int buffer ) {
    DWORD dwBytesLocked;
    VOID *lpvData;

	if (FAILED( IDirectSoundBuffer8_Lock(lpdsbuf, BufferSize * buffer,BufferSize, &lpvData, &dwBytesLocked,
		NULL, NULL, 0  ) ) )
	{
		IDirectSoundBuffer8_Unlock(lpdsbuf, lpvData, dwBytesLocked, NULL, 0 );
		//DisplayError("IDirectSoundBuffer8_Unlock");
		return;
	}
    FillMemory( lpvData, dwBytesLocked, 0 );
	IDirectSoundBuffer8_Unlock(lpdsbuf, lpvData, dwBytesLocked, NULL, 0 );
}

EXPORT void CALL GetDllInfo ( PLUGIN_INFO * PluginInfo )
{ 
	PluginInfo->Version = 0x0101;
	PluginInfo->Type    = PLUGIN_TYPE_AUDIO;

	strcpy (PluginInfo->Name, "1964 Audio version ");
	strcat (PluginInfo->Name, PLUGIN_VERSION);
#ifdef RSP_DECOMPILER 
	strcat (PluginInfo->Name, " PRIVATE DECOMPILER");
#endif

#ifdef _DEBUG
	strcat (PluginInfo->Name, " (Debug)");
#endif
}


extern DWORD imem_DMA_dst;
extern DWORD imem_DMA_src;

EXPORT BOOL CALL InitiateAudio (AUDIO_INFO Audio_Info) 
{
	HRESULT hr;
	int count;
	ROMHeader header;
	//void InitLogging ();
	static int initvariables=0;
	gUcode = UNDEFINED_UCODE;

    AudioInfo = Audio_Info;
	audioIsPlaying = FALSE;

	memcpy(&header, Audio_Info.HEADER, sizeof(ROMHeader));
	ROM_ByteSwap_3210( &header, sizeof(ROMHeader) );
	memset(gameName,0,sizeof(gameName));
	ROM_GetRomNameFromHeader(gameName, &header);
	
    if (!initvariables)
    {
        initvariables = 1;
	Dacrate = 0;
    Playing = FALSE;	
	SndBuffer[0] = Buffer_Empty;
	SndBuffer[1] = Buffer_Empty;
	SndBuffer[2] = Buffer_Empty;
	pIMEM  = (char*)Audio_Info.IMEM;
	pRDRAM = (char*)Audio_Info.RDRAM;
	pDMEM  = (char*)Audio_Info.DMEM;
	}

	//InitLogging ();

	DoOnce = 0;
	imem_DMA_dst = 0;
	imem_DMA_src = 0;
	rsp_reset();

	return TRUE;
}

EXPORT void CALL ProcessAList(void) 
{
#ifdef ENABLE_TRACE_COMPARE
	rsp_run_with_trace();
#else
	rsp_run ();

#endif
}

DWORD SPCycleCount=0;
EXPORT DWORD CALL ProcessAListCountCycles(void) 
{
#define CF 8

	SPCycleCount = 0;
#ifdef ENABLE_TRACE_COMPARE
	rsp_run_with_trace();
#else
	rsp_run ();
#endif

	if( SPCycleCount < 1600*CF )
		SPCycleCount = 1600*CF;
	return SPCycleCount/CF;
}

EXPORT void CALL RomClosed (void) 
{
//	ucodeDetected = FALSE;
//	gUcode = UNDEFINED_UCODE;
	
	if (!audioIsPlaying) return;
	audioIsPlaying = FALSE;
#ifdef THREADED
	TerminateThread (handleAudioThread, 0);
#endif


    IDirectSoundBuffer8_Stop(lpdsbuf);
	Dacrate = 0;
	Playing = FALSE;	
	SndBuffer[0] = Buffer_Empty;
	SndBuffer[1] = Buffer_Empty;
	SndBuffer[2] = Buffer_Empty;
	
	*AudioInfo.AI_STATUS_REG &= ~AI_STATUS_FIFO_FULL;
	*AudioInfo.MI_INTR_REG |= MI_INTR_AI;
}

void SetupDSoundBuffers(void) {
	LPDIRECTSOUNDBUFFER lpdsb;
    DSBUFFERDESC        dsPrimaryBuff, dsbdesc;
    WAVEFORMATEX        wfm;
    HRESULT             hr;
	int count;

    if (lpdsbuf) { CloseDLL(); InitiateAudio(AudioInfo);}

    if ( FAILED( hr = DirectSoundCreate( NULL, &lpds, NULL ) ) ) {
        return FALSE;
	}

    if ( FAILED( hr = IDirectSound8_SetCooperativeLevel(lpds, AudioInfo.hwnd, DSSCL_PRIORITY   ))) {
        return FALSE;
	}
    for ( count = 0; count < NUMCAPTUREEVENTS; count++ ) {
        rghEvent[count] = CreateEvent( NULL, FALSE, FALSE, NULL );
        if (rghEvent[count] == NULL ) { return FALSE; }
    }

	memset( &dsPrimaryBuff, 0, sizeof( DSBUFFERDESC ) ); 
    
	dsPrimaryBuff.dwSize        = sizeof( DSBUFFERDESC ); 
    dsPrimaryBuff.dwFlags       = DSBCAPS_PRIMARYBUFFER; 
    dsPrimaryBuff.dwBufferBytes = 0;  
    dsPrimaryBuff.lpwfxFormat   = NULL; 
    memset( &wfm, 0, sizeof( WAVEFORMATEX ) ); 

	wfm.wFormatTag = WAVE_FORMAT_PCM;
	wfm.nChannels = 2;
	wfm.nSamplesPerSec = 44100;
	wfm.wBitsPerSample = 16;
	wfm.nBlockAlign = wfm.wBitsPerSample / 8 * wfm.nChannels;
	wfm.nAvgBytesPerSec = wfm.nSamplesPerSec * wfm.nBlockAlign;

	hr = IDirectSound8_CreateSoundBuffer(lpds,&dsPrimaryBuff, &lpdsb, NULL);
	
	if (SUCCEEDED ( hr ) ) {
		IDirectSoundBuffer8_SetFormat(lpdsb, &wfm );
	    IDirectSoundBuffer8_Play(lpdsb, 0, 0, DSBPLAY_LOOPING );
	}

	wfm.nSamplesPerSec = Frequency;
	wfm.wBitsPerSample = 16;
	wfm.nBlockAlign = wfm.wBitsPerSample / 8 * wfm.nChannels;
	wfm.nAvgBytesPerSec = wfm.nSamplesPerSec * wfm.nBlockAlign;

    memset( &dsbdesc, 0, sizeof( DSBUFFERDESC ) ); 
    dsbdesc.dwSize = sizeof( DSBUFFERDESC ); 
    dsbdesc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY;
    dsbdesc.dwBufferBytes = BufferSize * 3;  
    dsbdesc.lpwfxFormat = &wfm; 

	if ( FAILED( hr = IDirectSound8_CreateSoundBuffer(lpds, &dsbdesc, &lpdsbuf, NULL ) ) ) {
		DisplayError("Failed in creation of Play buffer 1");	
	}
	FillBufferWithSilence( lpdsbuf );

    rgdscbpn[0].dwOffset = ( BufferSize ) - 1;
    rgdscbpn[0].hEventNotify = rghEvent[0];
    rgdscbpn[1].dwOffset = ( BufferSize * 2) - 1;
    rgdscbpn[1].hEventNotify = rghEvent[1];
    rgdscbpn[2].dwOffset = ( BufferSize * 3) - 1;
    rgdscbpn[2].hEventNotify = rghEvent[2];
    rgdscbpn[3].dwOffset = DSBPN_OFFSETSTOP;
    rgdscbpn[3].hEventNotify = rghEvent[3];

    if ( FAILED( hr = IDirectSound8_QueryInterface(lpdsbuf, &IID_IDirectSoundNotify, ( VOID ** )&lpdsNotify ) ) ) {
		DisplayError("IDirectSound8_QueryInterface: Failed\n");
		return;
	}

    // Set capture buffer notifications.
    if ( FAILED( hr = IDirectSoundNotify_SetNotificationPositions(lpdsNotify, NUMCAPTUREEVENTS, rgdscbpn ) ) ) {
		DisplayError("IDirectSoundNotify_SetNotificationPositions: Failed");
		return;
    }

	//AddEffect();
}

void Soundmemcpy(void * dest, const void * src, size_t count) {
	if (AudioInfo.MemoryBswaped) {
		if (ReverseStereo)
		{
			_asm {
				mov edi, dest
				mov ecx, src
				mov edx, 0		
			memcpyloop1:
				mov bx, word ptr [ecx + edx]
				mov ax, word ptr [ecx + edx + 2]
				mov  word ptr [edi + edx + 2],ax
				mov  word ptr [edi + edx],bx
				add edx, 4
				mov bx, word ptr [ecx + edx]
				mov ax, word ptr [ecx + edx + 2]
				mov  word ptr [edi + edx + 2],ax
				mov  word ptr [edi + edx],bx
				add edx, 4
				cmp edx, count
				jb memcpyloop1
			}
		}
		else
		{
			_asm {
				mov edi, dest
				mov ecx, src
				mov edx, 0		
			memcpyloop3:
				mov ax, word ptr [ecx + edx]
				mov bx, word ptr [ecx + edx + 2]
				mov  word ptr [edi + edx + 2],ax
				mov  word ptr [edi + edx],bx
				add edx, 4
				mov ax, word ptr [ecx + edx]
				mov bx, word ptr [ecx + edx + 2]
				mov  word ptr [edi + edx + 2],ax
				mov  word ptr [edi + edx],bx
				add edx, 4
				cmp edx, count
				jb memcpyloop3
			}
		}
	} else {
		_asm {
			mov edi, dest
			mov ecx, src
			mov edx, 0		
		memcpyloop2:
			mov ax, word ptr [ecx + edx]
			xchg ah,al
			mov  word ptr [edi + edx],ax
			add edx, 2
			mov ax, word ptr [ecx + edx]
			xchg ah,al
			mov  word ptr [edi + edx],ax
			add edx, 2
			mov ax, word ptr [ecx + edx]
			xchg ah,al
			mov  word ptr [edi + edx],ax
			add edx, 2
			mov ax, word ptr [ecx + edx]
			xchg ah,al
			mov  word ptr [edi + edx],ax
			add edx, 2
			cmp edx, count
			jb memcpyloop2
		}
	}
}


void ROM_ByteSwap_3210(void *v, DWORD dwLen)
{
	__asm
	{
		mov		esi, v
			mov		edi, v
			mov		ecx, dwLen

			add		edi, ecx

top:
		mov		al, byte ptr [esi + 0]
		mov		bl, byte ptr [esi + 1]
		mov		cl, byte ptr [esi + 2]
		mov		dl, byte ptr [esi + 3]

		mov		byte ptr [esi + 0], dl		//3
			mov		byte ptr [esi + 1], cl		//2
			mov		byte ptr [esi + 2], bl		//1
			mov		byte ptr [esi + 3], al		//0

			add		esi, 4
			cmp		esi, edi
			jne		top

	}
}


void ROM_GetRomNameFromHeader(TCHAR * szName, ROMHeader * pHdr)
{
	TCHAR * p;

	memcpy(szName, pHdr->szName, 20);
	szName[20] = '\0';

	p = szName + (lstrlen(szName) -1);		// -1 to skip null
	while (p >= szName && *p == ' ')
	{
		*p = 0;
		p--;
	}
}

void rdp_enddl(int val)
{
//empty.
}

#ifdef _DEBUG
void (__cdecl *_DebuggerMsgCallBackFunc) (char *msg) = NULL;
EXPORT void CALL SetDebuggerCallBack(void (_cdecl *DbgCallBackFun)(char *msg))
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

//Adding effects fails for Sim City 2000 because of rate.
void AddEffect(void)
{
HRESULT                 hr;
DWORD dwResults;
 
DSEFFECTDESC dsEffect;
dsEffect.dwSize = sizeof(DSEFFECTDESC);
dsEffect.dwFlags = 0; //creates temp buffer
dsEffect.guidDSFXClass = GUID_DSFX_STANDARD_ECHO;
dsEffect.guidDSFXClass = GUID_DSFX_WAVES_REVERB;
dsEffect.guidDSFXClass = GUID_DSFX_STANDARD_CHORUS;
dsEffect.guidDSFXClass = GUID_DSFX_STANDARD_FLANGER;
dsEffect.dwReserved1 = 0;
dsEffect.dwReserved2 = 0;
 
// Set the effect
//if (FAILED(hr = IDirectSoundBuffer8_SetFX(lpdsbuf, 1, &dsEffect, &dwResults)))
//    MessageBox(0, "Add effect failed", "", 0);//return hr;
}