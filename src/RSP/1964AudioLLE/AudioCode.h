#ifndef _AUDIOCODE_DOT_H_
#define _AUDIOCODE_DOT_H_

#include <windows.h>
#include <dsound.h>

#define SND_IS_NOT_EMPTY 0x4000000
#define SND_IS_FULL		 0x8000000
#define UNDEFINED_UCODE  0xFFFFFFFF

#define SEGMENTS	 3
#define LOCK_SIZE 0x1000
#define MAXBUFFER (LOCK_SIZE*SEGMENTS+LOCK_SIZE)
// LOCKSIZE must not be fractional
//#define LOCK_SIZE    (ac->SegmentSize)

#define BUFFSIZE (writeLoc-readLoc)

class AudioCode {
	protected:
		DWORD dwFreqTarget; // Frequency of the Nintendo64 Game Audio
		void (*CallBack)( DWORD );
		BOOL audioIsPlaying;
		HANDLE handleAudioThread;
		DWORD  dwAudioThreadId;
		HANDLE hMutex;
		LPDIRECTSOUNDBUFFER  lpdsbuf;
		LPDIRECTSOUND        lpds;
		BOOL audioIsDone;
		// Buffer Variables
		BYTE SoundBuffer[MAXBUFFER];
		DWORD readLoc;
		DWORD writeLoc;
		DWORD SampleRate;
		DWORD SegmentSize;


	public:
		friend DWORD WINAPI AudioThreadProc (AudioCode *ac);

		AudioCode () { lpdsbuf=NULL; lpds=NULL; audioIsDone = false;};
		~AudioCode () { };

		// Setup and Teardown Functions
		BOOL Initialize (void (*AiCallBack)( DWORD ), HWND hwnd);
		BOOL Initialize (HWND hwnd);
		void DeInitialize ();

		// Buffer Functions for the Audio Code
		void SetFrequency (DWORD Frequency);		// Sets the Nintendo64 Game Audio Frequency
		DWORD AddBuffer (BYTE *start, DWORD length);	// Uploads a new buffer and returns status
		void FillBuffer (BYTE *buff, DWORD len);
		void SetSegmentSize (DWORD length);

		// Management functions
		void StopAudio ();							// Stops the Audio PlayBack (as if paused)
		void StartAudio ();							// Starts the Audio PlayBack (as if unpaused)

		DWORD GetReadStatus ();						// Returns the status on the read pointer

};

#endif