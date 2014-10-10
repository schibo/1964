/*
* 1964 Copyright (C) 1999-2004 Joel Middendorf, <schibo@emulation64.com> This
* program is free software; you can redistribute it and/or modify it under the
* terms of the GNU General Public License as published by the Free Software
* Foundation; either version 2 of the License, or (at your option) any later
* version. This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
* details. You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. To contact the
* authors: email: schibo@emulation64.com, rice1964@yahoo.com
*/

#include "../stdafx.h"

volatile BOOL	Kaillera_Is_Running = FALSE;
int				Kaillera_Players = 0;
unsigned int	Kaillera_Counter = 0;

HINSTANCE		hKailleraDll = NULL;
BOOL			bKailleraDllLoaded = FALSE;

int (__stdcall *pDllKailleraGetVersion) (char *version) = NULL;
int (__stdcall *pDllKailleraInit) (void) = NULL;
int (__stdcall *pDllKailleraShutdown) (void) = NULL;
int (__stdcall *pDllKailleraSetInfos) (kailleraInfos * infos) = NULL;
int (__stdcall *pDllKailleraSelectServerDialog) (HWND parent) = NULL;
int (__stdcall *pDllKailleraModifyPlayValues) (void *values, int size) = NULL;
int (__stdcall *pDllKailleraChatSend) (char *text) = NULL;
int (__stdcall *pDllKailleraEndGame) (void) = NULL;

extern int NumOfActiveCheatGroups(void);
void KailleraReadDownloadedCheatCodes();
extern void GetFileName (char *filenameToReturn, char *Ext);
void KailleraUploadSaveFiles();
void KailleraDownloadFiles_SaveStrings(char *line);


#define KAILLERA_KEY_VALUE_QUETE_LEN	100
kPlayerEntry	kailleraKeyQueues[4][KAILLERA_KEY_VALUE_QUETE_LEN];
kPlayerEntry	kailleraLocalSendKeys[KAILLERA_KEY_VALUE_QUETE_LEN];
BOOL			kailleraKeyValid[4][KAILLERA_KEY_VALUE_QUETE_LEN];
BOOL			kailleraLocalSendKeysValid[KAILLERA_KEY_VALUE_QUETE_LEN];
//int				kailleraKeyQueueIndex[4];
kPlayerEntry	kBuffers[8];
kPlayerEntry	savedKeyValues[4];
BOOL			kailleraClientStatus[4];

int				kailleraLocalPlayerNumber=0;
HANDLE			kailleraThreadHandle = NULL;

extern HANDLE	kailleraThreadStopEvent;
extern HANDLE	kailleraThreadEvent;
volatile int	Kaillera_Thread_Keep_Running = FALSE;
volatile BOOL	Kaillera_Thread_Is_Running = FALSE;

int				kailleraLagness = 4;
unsigned int	kVIdelay = 12;
unsigned int	kVIdelayToUse = 12;
unsigned int	kLastKeyUpdateVI = 0;
int				kKeyUpdateStep = 3;

KailleraStateType	KailleraState = DLL_NOT_LOADED;

DWORD			kailleraLastUpdateKeysAtVI=0;
BUTTONS			kailleraKeyValuesToUpdate;
BOOL			kailleraAutoApplyCheat=FALSE;

FILE	*ktracefile=NULL;
FILE	*ktracefile2=NULL;

//#define KAILLERA_LOG_KEY_DATA
FILE			*kailleralogfp;

// Automatic retransmit request
#define NETPLAY_ARQ_REQ		0x80000000
#define NETPLAY_ARQ_REPLY	0x40000000
#define NETPLAY_ARQ_MASK	0x3FFFFFFF
#define NETPLAY_ARQ_BOTH	0xC0000000

/*
 =======================================================================================================================
    Dynamic Loading of Kaillera 0.84 //
 =======================================================================================================================
 */
int LoadDllKaillera(void)
{
	UnloadDllKaillera();

	hKailleraDll = LoadLibrary("KailleraClient.dll");
	if(hKailleraDll == NULL) 
		return 0;

	pDllKailleraGetVersion = (int(__stdcall *) (char *)) GetProcAddress(hKailleraDll, "_kailleraGetVersion@4");
	if(pDllKailleraGetVersion == NULL) 
		return 0;

	pDllKailleraInit = (int(__stdcall *) (void)) GetProcAddress(hKailleraDll, "_kailleraInit@0");
	if(pDllKailleraInit == NULL) 
		return 0;

	pDllKailleraShutdown = (int(__stdcall *) (void)) GetProcAddress(hKailleraDll, "_kailleraShutdown@0");
	if(pDllKailleraShutdown == NULL) 
		return 0;

	pDllKailleraSetInfos = (int(__stdcall *) (kailleraInfos *)) GetProcAddress(hKailleraDll, "_kailleraSetInfos@4");
	if(pDllKailleraSetInfos == NULL) 
		return 0;

	pDllKailleraSelectServerDialog = (int(__stdcall *) (HWND)) GetProcAddress
		(
			hKailleraDll,
			"_kailleraSelectServerDialog@4"
		);
	if(pDllKailleraSelectServerDialog == NULL) 
		return 0;

	pDllKailleraModifyPlayValues = (int(__stdcall *) (void *values, int size)) GetProcAddress
		(
			hKailleraDll,
			"_kailleraModifyPlayValues@8"
		);
	if(pDllKailleraModifyPlayValues == NULL) 
		return 0;

	pDllKailleraChatSend = (int(__stdcall *) (char *)) GetProcAddress(hKailleraDll, "_kailleraChatSend@4");
	if(pDllKailleraChatSend == NULL) 
		return 0;

	pDllKailleraEndGame = (int(__stdcall *) (void)) GetProcAddress(hKailleraDll, "_kailleraEndGame@0");
	if(pDllKailleraEndGame == NULL) 
		return 0;

	bKailleraDllLoaded = TRUE;
	return 1;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int UnloadDllKaillera(void)
{
	//kailleraShutdown();
	bKailleraDllLoaded = FALSE;

	pDllKailleraGetVersion = NULL;
	pDllKailleraInit = NULL;
	pDllKailleraShutdown = NULL;
	pDllKailleraSetInfos = NULL;
	pDllKailleraSelectServerDialog = NULL;
	pDllKailleraModifyPlayValues = NULL;
	pDllKailleraChatSend = NULL;
	pDllKailleraEndGame = NULL;

	if(hKailleraDll != NULL) 
		FreeLibrary(hKailleraDll);
	hKailleraDll = NULL;

	return 1;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int IsKailleraDllLoaded(void)
{
	return bKailleraDllLoaded;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int kailleraGetVersion(char *version)
{
	if(bKailleraDllLoaded && pDllKailleraGetVersion != NULL) 
		return pDllKailleraGetVersion(version);
	else
	{
		strcpy(version, "No Kaillera Dll");
		return 0;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int kailleraInit(void)
{
	if(bKailleraDllLoaded && pDllKailleraInit != NULL) 
		return pDllKailleraInit();
	else
		return 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int kailleraShutdown(void)
{
	if(bKailleraDllLoaded && pDllKailleraShutdown != NULL) 
		return pDllKailleraShutdown();
	else
		return 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int kailleraSetInfos(kailleraInfos *infos)
{
	if(bKailleraDllLoaded && pDllKailleraSetInfos != NULL) 
		return pDllKailleraSetInfos(infos);
	else
		return 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int kailleraSelectServerDialog(HWND parent)
{
	if(bKailleraDllLoaded && pDllKailleraSelectServerDialog != NULL) 
		return pDllKailleraSelectServerDialog(parent);
	else
		return 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int kailleraModifyPlayValues(void *values, int size)
{
	if(bKailleraDllLoaded && pDllKailleraModifyPlayValues != NULL) 
		return pDllKailleraModifyPlayValues(values, size);
	else
		return 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int kailleraChatSend(char *text)
{
	if(bKailleraDllLoaded && pDllKailleraChatSend != NULL) 
		return pDllKailleraChatSend(text);
	else
		return 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int kailleraEndGame(void)
{
	if(bKailleraDllLoaded && pDllKailleraEndGame != NULL) 
		return pDllKailleraEndGame();
	else
		return 0;
}

/************************************************************************/
/*
New ideas about Kaillera implementation in 1964:

1. Using kaillera in a separated thread.
-	Create a thread for kaillera.
-	The thread is responsible to modify key values at fixed (?) time intervals
-	The thread keeps a list of recent key value entries (indexed by
	kaillera counter value)
-	Will merge key values from players by using the kaillera counter value and
	provide correct combined key values to emulator thread.
-	There will be a delay, key input will have lag.
-	The fixed time interval is configurable, should it be adaptive adjusted?
-	Key entries are tagged by the controller reading count
-	For fast network connection client, don't need to send key updates too fast,
	no need to be faster than VI rate (Need further study)
-	Compute other clients' average connection speed by using adjacent VI counter
	values of each clients. No need to update key values at speed faster than 3
	times of the slowest client
-	Limit of the maximam key update rate at fast client
-	Adaptative delay = to make the slowest client and the slowest connection client
	to get their best performance. Each client should compute its own optimal delay

-	delay is depend on network connection speed (in time domain), and then
	compute to VI counter domain

-	All clients will wait to sync at the first key update
-	the usage of delay is essential to achieve real time speed game play for
	slow connection players
-	0.2s delay shouldn't be significant for user
-	Even for local client, don't use its local key values, but use its values
	from the Kaillera server

2.  Implementation details
1)	Queues of key entries of each client
2)	Size requirement of each queues. It should not need to have very large
	queues since network connection ping speed isn't very fast even for
	the fastest connection
3)	Content of key entry
	-> keys, joy stick values
	-> current VI count
	-> computed optimal delay

3.  Controller key read
	- Use key entries
	- Sync at VI to the smallest VI counter value in all player key entries + delay
	- Freeze the emulator thread if CPU is running too fast
*/

/************************************************************************/
/* Call this function to stop the Kaillera thread                       */
/************************************************************************/
BOOL WindowMsgLoop();
void StopKailleraThread(void)
{
	TRACE0("To stop Kaillera thread");
	if( kailleraThreadHandle != NULL )
	{
		DWORD signal;
		SetEvent(kailleraThreadStopEvent);	//Tell that this thread is terminated
		Kaillera_Thread_Keep_Running = FALSE;
		signal = WaitForSingleObject( kailleraThreadEvent, 300 );
		if( signal == WAIT_OBJECT_0 )
		{
			TRACE0("kaillera thread is successfully stopped");
		}
		else
		{
			ResetEvent(kailleraThreadStopEvent);
			TerminateThread(kailleraThreadHandle,0);
			Kaillera_Thread_Is_Running = FALSE;
			Kaillera_Thread_Keep_Running = FALSE;
			TRACE0("kaillera thread is terminated");
		}

		kailleraThreadHandle = NULL;
	}
}

/************************************************************************/
/* This function is called by the Kaillera thread infinite loop to      */
/* perodically update the players values:                               */
/*    -> Update local player key values to Kaillera server              */
/*    -> Read the other players' latest key values from Kaillera server */
/*    -> Sync all players                                               */
/*    -> Lost packet detection and ARQ                                  */
/************************************************************************/
BUTTONS Keys;
kPlayerEntry LastEntries[4];
void KailleraUpdatePlayerKeyValues()
{
	// Called in the Kaillera thread
	int		reclen;
	int		i;
	DWORD signal;
	unsigned int VItouse;
	kPlayerEntry *entry;
	int idx;

	kLastKeyUpdateVI += kKeyUpdateStep;
	CONTROLLER_GetKeys(0, &Keys );		// Read key status of the local player
	DEBUG_NETPLAY_MACRO(TRACE3("P%d upload key (%X) for SyncVI=%d", kailleraLocalPlayerNumber, Keys.Value, kLastKeyUpdateVI));
	KAILLERA_LOG2(fprintf(ktracefile2, "P%d upload key (%08X) for (SyncVI=%d)\n", kailleraLocalPlayerNumber, Keys.Value, kLastKeyUpdateVI));

	memcpy(&kBuffers[0].b, &Keys, sizeof(BUTTONS));

label_Jump:
	signal = WaitForSingleObject( kailleraThreadStopEvent, 0 );
	if( signal == WAIT_OBJECT_0 )
	{
		DEBUG_NETPLAY_TRACE0("kaillera thread is stopped in KailleraUpdatePlayerKeyValues");
		SetEvent(kailleraThreadStopEvent);
		return;
	}


	kBuffers[0].viCount = kLastKeyUpdateVI;
	kBuffers[0].optimalDelay = kVIdelay;
	idx = kLastKeyUpdateVI/kKeyUpdateStep%KAILLERA_KEY_VALUE_QUETE_LEN;
	memcpy(&kailleraLocalSendKeys[idx], &kBuffers[0], sizeof(kPlayerEntry));
	kailleraLocalSendKeysValid[idx]=TRUE;

	/*
	if( rand() < 0x400 )	
	{
		// to simulate package lose
		// Disable here in formal release
		int badplayer = rand()%4;
		idx = kBuffers[badplayer].viCount/kKeyUpdateStep%KAILLERA_KEY_VALUE_QUETE_LEN;
		idx--;
		if( idx < 0 ) idx=0;
		memcpy(&kBuffers[badplayer], &kailleraKeyQueues[badplayer][idx], sizeof(kPlayerEntry));
	}
	*/

	// Update local player key values to server and get key values of other players
	reclen = kailleraModifyPlayValues((void *) kBuffers, sizeof(kPlayerEntry));

check_again:
	if( memcmp(&kBuffers, &LastEntries, sizeof(LastEntries)) == 0 )
	{
		Sleep(2);
		goto label_Jump;
	}
	else
	{
		memcpy(&LastEntries, &kBuffers, sizeof(LastEntries));
	}

	if(reclen == -1)
	{
		SetStatusBarText(0, TranslateStringByString("Kaillera timeout"));
		DEBUG_NETPLAY_TRACE0("Kaillera timeout");
		SetEvent(kailleraThreadStopEvent);
		Kaillera_Thread_Keep_Running = FALSE;
		return;
	}
	else if(reclen > 0)
	{
		DWORD minVI=0xFFFFFFFF;
		BOOL  ARQRequest=FALSE;

		// Check optimal delay values
		kVIdelayToUse = kVIdelay;
		for(i = 0; i < Kaillera_Players; i++)
		{
			if( kailleraClientStatus[i] )
			{
				if( kBuffers[i].optimalDelay > kVIdelay )
				{
					kVIdelayToUse = kBuffers[i].optimalDelay;
				}
			}
		}

		VItouse = (viTotalCount-kVIdelayToUse)/kKeyUpdateStep*kKeyUpdateStep;	// emulator thread will try to sync at this VIcount


		// Store the keys in the queue
		for(i = 0; i < Kaillera_Players; i++)
		{
			if( kailleraClientStatus[i] && (kBuffers[i].viCount&NETPLAY_ARQ_BOTH) == 0 )
			{
				idx = kBuffers[i].viCount/kKeyUpdateStep%KAILLERA_KEY_VALUE_QUETE_LEN;
				entry = &(kailleraKeyQueues[i][idx]);
				if( entry->viCount != kBuffers[i].viCount || entry->viCount == 0 || kailleraKeyValid[i][idx] == FALSE )
				{
					entry->b.Value = kBuffers[i].b.Value;
					entry->optimalDelay = kBuffers[i].optimalDelay;
					entry->viCount = kBuffers[i].viCount;
					kailleraKeyValid[i][idx] = TRUE;
				}

			}
		}


		// Check ARQ request
		for(i = 0; i < Kaillera_Players; i++)
		{
			if( kailleraClientStatus[i] )
			{
				// Check ARQ first

				// Is this player request an earlier packet
				if( (kBuffers[i].viCount&NETPLAY_ARQ_REQ) != 0 && kailleraLocalPlayerNumber == kBuffers[i].b.Value )
				{
					// The ARQ request is for this local player
					
					int arq_vicount = kBuffers[i].viCount&NETPLAY_ARQ_MASK;
					idx = arq_vicount/kKeyUpdateStep%KAILLERA_KEY_VALUE_QUETE_LEN;

					if( arq_vicount == kailleraLocalSendKeys[idx].viCount && kailleraLocalSendKeysValid[idx] )
					{
						memcpy(&kBuffers[0], &kailleraLocalSendKeys[idx], sizeof(kPlayerEntry));
						kBuffers[0].viCount |= NETPLAY_ARQ_REPLY;

						DEBUG_NETPLAY_TRACE1("To reply ARQ for syncVI=%d", arq_vicount);
						KAILLERA_LOG(fprintf(ktracefile, "P%d to reply ARQ for VI=%d, key=%08X\n", kailleraLocalPlayerNumber, arq_vicount, kBuffers[0].b.Value));
						kailleraModifyPlayValues((void *) kBuffers, sizeof(kPlayerEntry));
						Sleep(6);

						goto check_again;

						break;	// Ignore request from all other players
					}
					else
					{
						DEBUG_NETPLAY_TRACE1("Error: ARQ for VI=%d, but haven't get there yet", arq_vicount);
					}
				}
			}
		}

		// Check ARQ reply
		for(i = 0; i < Kaillera_Players; i++)
		{
			if( kailleraClientStatus[i] )
			{
				if( (kBuffers[i].viCount & NETPLAY_ARQ_REPLY) != 0 )	// Is this a ARQ reply 
				{
					uint32 tempcount = (kBuffers[i].viCount&NETPLAY_ARQ_MASK);
					idx = tempcount/kKeyUpdateStep%KAILLERA_KEY_VALUE_QUETE_LEN;
					
					// save the ARQ reply
					entry = &(kailleraKeyQueues[i][idx]);
					if( entry->viCount != tempcount || entry->viCount == 0 || kailleraKeyValid[i][idx] == FALSE )
					{
						entry->b.Value = kBuffers[i].b.Value;
						entry->optimalDelay = kBuffers[i].optimalDelay;
						entry->viCount = tempcount;
						kailleraKeyValid[i][idx] = TRUE;
						KAILLERA_LOG(fprintf(ktracefile, "P%d replied ARQ for VI=%d, key=%08X\n", i, tempcount, kBuffers[i].b.Value));
					}

					DEBUG_NETPLAY_TRACE1("ARQ replied at syncVI = %d", tempcount);
				}
			}
		}

		// Check if we need to send ARQ
		for(i = 0; i < Kaillera_Players; i++)
		{
			if( kailleraClientStatus[i] && (kBuffers[i].viCount & NETPLAY_ARQ_BOTH) == 0 )
			{
				// Need to search backward 10 record to see if we have missed some keys values
				// while later values have been received successfully.
				{
					// ARQ check
					int k;
					int viToCheck = kBuffers[i].viCount;

					if( viToCheck-VItouse > 2*kKeyUpdateStep)
						viToCheck -= kKeyUpdateStep;	// Don't check 1 step below, UDP packet may arrive slightly out of order

					for( k=0; k<10; k++ )
					{
						viToCheck -= kKeyUpdateStep;

						if( viToCheck < VItouse )	// Don't need to request key for VIs that we have passed
							break;

						if( viToCheck >= kKeyUpdateStep )
						{
							int idx2 = viToCheck/kKeyUpdateStep%KAILLERA_KEY_VALUE_QUETE_LEN;
							if( kailleraKeyValid[i][idx2] == FALSE || kailleraKeyQueues[i][idx2].viCount != viToCheck )
							{
								if( i == kailleraLocalPlayerNumber )
								{
									// lost packet from myself
									int idx3 = 	viToCheck/kKeyUpdateStep%KAILLERA_KEY_VALUE_QUETE_LEN;
									memcpy(&kBuffers[0], &kailleraLocalSendKeys[idx3], sizeof(kPlayerEntry));
									kBuffers[0].viCount |= NETPLAY_ARQ_REPLY;
									DEBUG_NETPLAY_TRACE1("To resend Keys for syncVI=%d", viToCheck);
									kailleraModifyPlayValues((void *) kBuffers, sizeof(kPlayerEntry));
									Sleep(3);
									goto check_again;
								}
								else
								{
									// we haven't receive key for VIcount = viToCheck;
									// Need to request ARQ from player #i
									kBuffers[0].b.Value = i;
									kBuffers[i].viCount = viToCheck|NETPLAY_ARQ_REQ;

									DEBUG_NETPLAY_MACRO(TRACE3("ARQ P%d for syncVI = %d, Got (%08X)", i,viToCheck, kBuffers[i].viCount));
									KAILLERA_LOG(fprintf(ktracefile, "P%d ARQ to P%d for syncVI=%d\n", kailleraLocalPlayerNumber, i, viToCheck));

									kailleraModifyPlayValues((void *) kBuffers, sizeof(kPlayerEntry));
									ARQRequest = TRUE;
									Sleep(50);
									goto check_again;
								}

							}
						}
					}
				}
			}
		}


		for(i = 0; i < Kaillera_Players; i++)
		{
			if( kailleraClientStatus[i] && (kBuffers[i].viCount&NETPLAY_ARQ_BOTH) == 0 )
			{
				// This is normal reply
				if( kLastKeyUpdateVI > kVIdelayToUse+kKeyUpdateStep && kBuffers[i].viCount < (kLastKeyUpdateVI-kVIdelayToUse+kKeyUpdateStep)/kKeyUpdateStep*kKeyUpdateStep )
				{
					// Sync vi to at least "kLastKeyUpdateVI-kVIdelayToUse"
					Sleep(2);
					goto label_Jump;
				}

				if( VItouse > kVIdelayToUse+kKeyUpdateStep && VItouse < kLastKeyUpdateVI && kBuffers[i].viCount < VItouse )
				{
					// Sync vi to at least "kLastKeyUpdateVI-kVIdelayToUse"
					Sleep(2);
					goto label_Jump;
				}

				if( minVI>kBuffers[i].viCount ) 
					minVI = kBuffers[i].viCount;
			}
		}

		for(i = 0; i < Kaillera_Players; i++)
		{
			// Check again from the point of minVI
			if( kailleraClientStatus[i] )
			{
				int k;
				int viToCheck = minVI;

					viToCheck -= kKeyUpdateStep;	// Don't check 1 step below, UDP packet may arrive slightly out of order

				for( k=0; k<10; k++ )
				{
					if( viToCheck >= 0 && viToCheck >= VItouse)
					{
						int idx3 = 	viToCheck/kKeyUpdateStep%KAILLERA_KEY_VALUE_QUETE_LEN;
						if( kailleraKeyValid[i][idx3] == FALSE || kailleraKeyQueues[i][idx3].viCount != viToCheck )
						{
							Sleep(4);
							goto label_Jump;
						}
					}
				}

			}
		}


		if( minVI == 0xFFFFFFFF )
		{
			Sleep(4);
			goto label_Jump;
		}

		// Update the latest key values from the server to my key value queues
		DEBUG_NETPLAY_TRACE1("Kailler synced at syncVI = %d", minVI);
	}
	else	// reclen == 0
	{
		Sleep(2);
		goto label_Jump;
	}
}

/************************************************************************/
/* Kaillera thread                                                      */
/************************************************************************/
extern int get_current_profiling_process(void);
void (__stdcall StartKailleraThread)(void *pVoid)
{
	DWORD signal;

	LARGE_INTEGER CurrentCPUTime;
	LARGE_INTEGER LastCPUTime;
	LARGE_INTEGER Elapsed;
	double elapsedtime;
	int processid;

#ifdef KAILLERA_LOG_KEY_DATA
	char filename[300];
	sprintf(filename,"\\kaillera_log_%d.log", kailleraLocalPlayerNumber);
	kailleralogfp = fopen(filename, "wt");
#endif

	QueryPerformanceCounter(&LastCPUTime);

	memset(kailleraKeyQueues, 0, sizeof(kailleraKeyQueues));
	memset(kailleraKeyValid, 0, sizeof(kailleraKeyValid));
	memset(kBuffers, 0, sizeof(kBuffers));
	memset(savedKeyValues, 0, sizeof(savedKeyValues));
	memset(kailleraLocalSendKeys, 0, sizeof(kailleraLocalSendKeys));
	memset(kailleraLocalSendKeysValid, 0, sizeof(kailleraLocalSendKeysValid));
	memset(LastEntries, 0xFF, sizeof(LastEntries));

	//kailleraKeyQueueIndex[0] = kailleraKeyQueueIndex[1] = kailleraKeyQueueIndex[2] = kailleraKeyQueueIndex[3] = -1;
	kLastKeyUpdateVI = 0;

	TRACE0("");
	TRACE0("Starting Kaillera thread")
	TRACE0("");

	kailleraLastUpdateKeysAtVI=0;
	//kailleraAutoApplyCheat = FALSE;

	Kaillera_Thread_Keep_Running = TRUE;
	Kaillera_Thread_Is_Running = TRUE;
	processid = get_current_profiling_process();
	DO_PROFILIER(KAILLERA_PROF);

	ResetEvent(kailleraThreadStopEvent);
	signal = WaitForSingleObject( kailleraThreadStopEvent, 0 );

	memset(&Keys,0,sizeof(Keys));

    while( Kaillera_Thread_Keep_Running )
	{
		signal = WaitForSingleObject( kailleraThreadStopEvent, 0 );
		if( signal == WAIT_OBJECT_0 )
		{
			TRACE0("Get kaillera stop event");
			break;
		}

		QueryPerformanceCounter(&CurrentCPUTime);
		Elapsed.QuadPart = CurrentCPUTime.QuadPart - LastCPUTime.QuadPart;
		elapsedtime = (double)Elapsed.QuadPart/(double)Freq.QuadPart;

		//if( elapsedtime > 0.10 || viTotalCount>=kLastKeyUpdateVI+kKeyUpdateStep)
		if( viTotalCount>=kLastKeyUpdateVI+kKeyUpdateStep)
		{
			// Update key values at least every 60 ms
			// I am constantly updating player key values every other 3 ms, no matter key values are read or not
			// key values are updated together with current VI count
			KailleraUpdatePlayerKeyValues();
			QueryPerformanceCounter(&LastCPUTime);
		}
		else
		{
			DO_PROFILIER(processid);
			Sleep(4);
			processid = get_current_profiling_process();
			DO_PROFILIER(KAILLERA_PROF);
		}
	}
	TRACE0("kaillera thread is stopped");
	
	Kaillera_Thread_Is_Running = FALSE;

	SetEvent(kailleraThreadEvent);	//Tell that this thread is terminated

#ifdef KAILLERA_LOG_KEY_DATA
	fclose(kailleralogfp);
#endif
	ExitThread(0);
}

/************************************************************************/
/* The emulator thread will call this function to read players key      */
/* values. If Kaillera is running, emulator thread should not use       */
/* controller plugin functions anymore, instead, Kaillera functions     */
/* should be used even for the local player. Kaillera functions will    */
/* manage all player control inputs - even for the local player         */
/************************************************************************/
void KailleraGetPlayerKeyValues(kPlayerEntry keyvalues[4], int player)
{
	// We have 4 queues
	// delay up to 10 VI

	//kPlayerEntry	kailleraKeyQueues[4][KAILLERA_KEY_VALUE_QUETE_LEN];
	//int			kailleraKeyQueueIndex[4];
	//kPlayerEntry	kBuffers[8];

	unsigned int VItouse;
	int idx;

	if( kailleraClientStatus[player] == FALSE )//&& (player == kailleraLocalPlayerNumber )
	{
		CONTROLLER_GetKeys(0, &keyvalues[player].b );	// Read key status of the local player
		return;
	}

	if( viTotalCount < kVIdelay || kailleraClientStatus[player] == FALSE )
	{
		memset(&keyvalues[player].b,0, sizeof(BUTTONS));
		return;
	}

	VItouse = (viTotalCount-kVIdelayToUse)/kKeyUpdateStep*kKeyUpdateStep;
	DEBUG_NETPLAY_MACRO(TRACE3("Read player %d at VI %d by syncVI(%d)", player, viTotalCount, VItouse));

CheckKeyValuesAgain:
	if( Kaillera_Thread_Is_Running == FALSE || Kaillera_Thread_Keep_Running == FALSE )
	{
		if( player == kailleraLocalPlayerNumber )
			CONTROLLER_GetKeys(0, &keyvalues[player].b );	// Read key status of the local player
		else
			memset(&keyvalues[player].b,0, sizeof(BUTTONS));
		return;
	}

	idx = VItouse/kKeyUpdateStep%KAILLERA_KEY_VALUE_QUETE_LEN;

	if( (kailleraKeyValid[player][idx] == FALSE || kailleraKeyQueues[player][idx].viCount != VItouse ) && kailleraClientStatus[player] )
	{
		DEBUG_NETPLAY_MACRO(TRACE2("Wait for player %d to sync to key VI=%d", player, VItouse));
		Sleep(4);
		goto CheckKeyValuesAgain;
	}

	DEBUG_NETPLAY_MACRO(TRACE4("Player %d get key=%X, VI (%d), syncVI (%d)", player, kailleraKeyQueues[player][idx].b.Value, viTotalCount, kailleraKeyQueues[player][idx].viCount));
	KAILLERA_LOG(fprintf(ktracefile, "P%d get key %08X at VI %d by syncVI(%d), key VI count=%d\n", player, kailleraKeyQueues[player][idx].b.Value, viTotalCount, VItouse, kailleraKeyQueues[player][idx].viCount));
	memcpy( keyvalues+player, kailleraKeyQueues[player]+idx, sizeof(kPlayerEntry));
}

void KailleraGetPlayerKeyValuesFor1Player(BUTTONS *pKeyValue, int player)
{
	DO_PROFILIER_NETPLAY;
	if( Kaillera_Thread_Is_Running )
	{
		KailleraGetPlayerKeyValues(savedKeyValues, player);
	}

	memcpy(pKeyValue, &savedKeyValues[player].b, sizeof(BUTTONS));
	DO_PROFILIER_R4300I;
}

unsigned int lagnessMenuList[7] = {
	ID_LAGNESS_1,	ID_LAGNESS_2,	ID_LAGNESS_3,	ID_LAGNESS_4,
	ID_LAGNESS_5,	ID_LAGNESS_6,	ID_LAGNESS_7};

void SetKailleraLagness(int lag)
{
	if( lag >= 1 && lag <= 7 && kailleraLagness != lag)
	{
		CheckMenuItem(gui.hMenu1964main, lagnessMenuList[kailleraLagness - 1], MF_UNCHECKED);
		kailleraLagness = lag;
		kVIdelay = lag*3;
		CheckMenuItem(gui.hMenu1964main, lagnessMenuList[kailleraLagness - 1], MF_CHECKED);
	}
}


AUDIO_INFO	Audio_Info_For_Kaillera;
uint32 AIRegK[10];
void CheckInterruptsKaillera(void)
{
}

void KailleraInitAudioPlugin(void)
{
	memcpy(&Audio_Info_For_Kaillera, &Audio_Info, sizeof(Audio_Info));

	Audio_Info_For_Kaillera.__MI_INTR_REG = (DWORD*)&AIRegK[9];
	//Audio_Info_For_Kaillera.__AI_DRAM_ADDR_REG = &AIRegK[0];;
	Audio_Info_For_Kaillera.__AI_DRAM_ADDR_REG = (DWORD*)&AI_DRAM_ADDR_REG;
	Audio_Info_For_Kaillera.__AI_LEN_REG = (DWORD*)&AIRegK[1];
	//Audio_Info_For_Kaillera.__AI_LEN_REG = &AI_LEN_REG;
	Audio_Info_For_Kaillera.__AI_CONTROL_REG = (DWORD*)&AIRegK[2];
	//Audio_Info_For_Kaillera.__AI_CONTROL_REG = &AI_CONTROL_REG;
	Audio_Info_For_Kaillera.__AI_STATUS_REG = (DWORD*)&AIRegK[3];
	Audio_Info_For_Kaillera.__AI_DACRATE_REG = (DWORD*)&AIRegK[4];
	Audio_Info_For_Kaillera.__AI_BITRATE_REG = (DWORD*)&AIRegK[5];
	//Audio_Info_For_Kaillera.__AI_BITRATE_REG = &AI_BITRATE_REG;
	Audio_Info_For_Kaillera.CheckInterrupts = CheckInterruptsKaillera;

	/*
	Audio_Info.__AI_DRAM_ADDR_REG = &AI_DRAM_ADDR_REG;;
	Audio_Info.__AI_LEN_REG = &AI_LEN_REG;
	Audio_Info.__AI_CONTROL_REG = &AI_CONTROL_REG;
	Audio_Info.__AI_STATUS_REG = &AI_STATUS_REG;
	Audio_Info.__AI_DACRATE_REG = &AI_DACRATE_REG;
	Audio_Info.__AI_BITRATE_REG = &AI_BITRATE_REG;
	*/


	AUDIO_Initialize(Audio_Info_For_Kaillera);
}

void WINAPI kailleraClientDroppedCallback(char *nick, int playernb);

/************************************************************************/
/* We are using ChatSend and ChatReceiveCallBack to transfer data       */
/* between players. See the KailleraRomOpen() function comment about    */
/* data to be transferred.                                              */
/************************************************************************/
void WINAPI kailleraChatReceivedCallback(char *nick, char *text)
{
	if( KailleraState == TRANSFER_DATA || (KailleraState == INPLAY && viTotalCount == 0 ) )
	{
		/************************************************************************/
		/* Data format:                                                         */
		/*    !Setting=3,....    (CF,and then other setting)                    */
		/*    !Cheats=10                                                        */
		/*    !Cheat#1=803233230320,809832331023,....                           */
		/*    !Cheat#2=.....                                                    */
		/*    !SaveType=eep(or sra, fla, mpk)                                   */
		/*    !SaveLines=15                                                     */
		/*    !Save#1=xkdksjeidj;adjdjs;akljdks (encoded string)                */
		/************************************************************************/

		if( text[0] == '!' && kailleraLocalPlayerNumber != 0 )
		{
			FILE *stream = NULL;
			char *line = text+1;

			if( strncmp(line, "cf=", 3) == 0 )
			{
				INI_ENTRY *c = &currentromoptions;
				sscanf(line,"cf=%d,32bit=%d,reg=%d,fpu=%d,rdram=%d,save=%d,tlb=%d,eeprom=%d,2pass=%d,rsp=%d,link=%d",
					&(c->Counter_Factor),&(c->Assume_32bit),&(c->Use_Register_Caching),&(c->FPU_Hack),&(c->RDRAM_Size),&(c->Save_Type),
					&(c->Use_TLB),&(c->Eeprom_size),&(c->Advanced_Block_Analysis),&(c->RSP_RDP_Timing),&(c->Link_4KB_Blocks));
				CounterFactor = currentromoptions.Counter_Factor;
#ifdef _DEBUG
				currentromoptions.Assume_32bit=ASSUME_32BIT_NO;
#endif
			}
			else if( strncmp(line, "DontApplyCheat", 14) == 0 )
			{
				if( kailleraLocalPlayerNumber != 0 )
					kailleraAutoApplyCheat = FALSE;
			}
			else if( strncmp(line, "ApplyCheat", 10) == 0 )
			{
				if( kailleraLocalPlayerNumber != 0 )
					kailleraAutoApplyCheat = TRUE;
			}
			else if( strncmp(line, "Cheat=", 6) == 0 )
			{
				char codefilepath[256];
				strcpy(codefilepath, directories.main_directory);
				strcat(codefilepath, "kaillera.cht");
				if( line[6]=='[' )
					stream = fopen(codefilepath, "wt");
				else
					stream = fopen(codefilepath, "at");

				if( stream ) 
				{
					fprintf(stream,"%s\n",line+6);
					fclose(stream);
					stream = NULL;
				}
			}
			else if( strncmp(line, "CheatEnd", 8) == 0 )
			{
				//if( stream != NULL )
				//	fclose(stream);

				// Load the kaillera.cht file
				KailleraReadDownloadedCheatCodes();
			}
			else if( strncmp(line, "!!StartGame!!!", 14) == 0 )
			{
				KailleraState = INPLAY;
			}
			else if( strncmp(text, "!EEPROM", 7) == 0 )
			{
				KailleraDownloadFiles_SaveStrings(text);
			}
			else if( strncmp(text, "!SRAM", 5) == 0 )
			{
				KailleraDownloadFiles_SaveStrings(text);
			}
			else if( strncmp(text, "!FLASH", 6) == 0 )
			{
				KailleraDownloadFiles_SaveStrings(text);
			}
			else if( strncmp(text, "!MEMPAK", 7) == 0 )
			{
				KailleraDownloadFiles_SaveStrings(text);
			}
		}
		else if( strncmp(text, "!!!StartGame!!!", 15) == 0 )
		{
			KailleraState = INPLAY;
		}
	}
	else
	{
		if( strncmp(text, "* Player 1", 10) == 0 && strstr(text,"dropped from the current game.") != 0 )
		{
			kailleraClientDroppedCallback("Player 1", 1);
		}

	}
	DEBUG_NETPLAY_MACRO(TRACE2("Kaillera msg from %s : %s", nick, text));
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void WINAPI kailleraMoreInfosCallback(char *gamename)
{
	/* Do what you want with this :) */
}

/************************************************************************/
/* If the local player is not the player 0 (the player in control)      */
/* game load/save will be done over dummy files, not the regular save   */
/* files for the game on this local computer. Otherwise game will be    */
/* out of sync if the save states are different across the netplay.     */
/* In the further, we want to automatically transfer the save file from */
/* the player #0 to all other players to overwrite the dummy save file. */
/************************************************************************/
void KailleraResetSaveFiles(void)
{
	/************************************************************************/
	/* Need to delete (if exist) and create new dummy save files            */
	/* EEPROM-4KB, EEPROM-16KB, MEMPAK, SRAM, FlashRAM                      */
	/************************************************************************/

	char filename[256];

	strcpy(filename, directories.save_directory_to_use);
	strcat(filename, "kaillera.eep");
	DeleteFile(filename);

	strcpy(filename, directories.save_directory_to_use);
	strcat(filename, "kaillera.m0");
	DeleteFile(filename);

	strcpy(filename, directories.save_directory_to_use);
	strcat(filename, "kaillera.sra");
	DeleteFile(filename);

	strcpy(filename, directories.save_directory_to_use);
	strcat(filename, "kaillera.fla");
	DeleteFile(filename);
}

// To be called after the game is stopped
void KailleraRomClosed(void)
{
}

void KailleraUploadCheatCodes()
{
	char			codefilepath[_MAX_PATH];
	char			line[2048], romname[256];	//changed line length to 2048 previous was 256
	BOOL			found;
	FILE			*stream;
	int				count;

	char *intername_rom_name = currentromoptions.Game_Name;

	if( !emuoptions.auto_apply_cheat_code || NumOfActiveCheatGroups() == 0 )
	{
		// Only upload codes if cheats are turned on
		kailleraChatSend("!DontApplyCheat");
		return;
	}

	kailleraChatSend("!ApplyCheat");

	strcpy(codefilepath, directories.main_directory);
	strcat(codefilepath, "1964.cht");

	stream = fopen(codefilepath, "rt");
	if(stream == NULL)
	{
		return;
	}

	/* Locate the entry for current rom by searching for rominfo.name */
	sprintf(romname, "[%s]", intername_rom_name);
	found = FALSE;

	while(fgets(line, 256, stream))
	{
		chopm(line);
		if(strcmp(line, romname) != 0)
			continue;
		else
		{
			found = TRUE;
			break;
		}
	}

	if(found)
	{
		int numberofgroups;

		sprintf(generalmessage, "!Cheat=%s",romname);
		kailleraChatSend(generalmessage);

		if(fgets(line, 256, stream))
		{
			chopm(line);
			if(strncmp(line, "NumberOfGroups=", 15) == 0)
			{
				numberofgroups = atoi(line + 15);
				if( numberofgroups > MAX_CHEATCODE_GROUP_PER_ROM )
				{
					numberofgroups = MAX_CHEATCODE_GROUP_PER_ROM;
				}
			}
			else
			{
				numberofgroups = MAX_CHEATCODE_GROUP_PER_ROM;
			}
		}
		else
		{
			fclose(stream);
			return;
		}

		sprintf(generalmessage, "!Cheat=%s",line);
		kailleraChatSend(generalmessage);

		count = 0;
		while(count < numberofgroups && fgets(line, 32767, stream) && strlen(line) > 8)	//changed by Witten
		{																							//32767 makes sure the entire line is read
			char *p = strstr(line,"=");
			char *p2 = strstr(p,"\"");
			chopm(line);
			if( p2 )
			{
				p2 = strstr(p,",");
				if( p2[1] == '1' )
				{
					// This code is activated
					sprintf(generalmessage, "!Cheat=C%d,%c=%s",count,p[-1],p2+1);
					kailleraChatSend(generalmessage);
					kailleraModifyPlayValues((void *) kBuffers, sizeof(kPlayerEntry));
					Sleep(40);
					count++;
				}
			}
			else if( p[1] == '1' )
			{
				// This code is activated
				sprintf(generalmessage, "!Cheat=C%d,%c=%s",count,p[-1],p+1);
				kailleraChatSend(generalmessage);
				kailleraModifyPlayValues((void *) kBuffers, sizeof(kPlayerEntry));
				Sleep(40);
				count++;
			}
		}

		kailleraChatSend("!CheatEnd");
	}

	fclose(stream);
}

void KailleraReadDownloadedCheatCodes()
{
	CodeList_ReadCode(currentromoptions.Game_Name,"kaillera.cht");
}

char *eepromBuf=NULL;
char *sramBuf=NULL;
char *mempakBuf=NULL;
char *flashramBuf=NULL;

/************************************************************************/
/* This function is called before the emulating thread                  */
/* What could be done here is:                                          */
/*   > All players                                                      */
/*     Measure the average ping response time, so to set the netplay    */
/*     optimized lagness automatically                                  */
/*   > From player 0:                                                   */
/*     Broadcast current rom setting to all other players               */
/*     Broadcast current activated cheatcode to all other players       */
/*     Encode and broadcast current save files to all other players     */
/*     wait response from other players (do we need to do this)         */
/*   > Other players                                                    */
/*     Wait rom setting and cheatcode setting from player 0             */
/*     Receive and decode current rom setting and store to save file    */
/************************************************************************/
void KailleraRomOpen(void)
{
	int i;

	KailleraState = TRANSFER_DATA;
	KailleraResetSaveFiles();
	viTotalCount = 0;

	//KailleraState = INPLAY;
	//return;

	if( kailleraLocalPlayerNumber != 0 )
	{
		eepromBuf=(char*)malloc(256*1024);
		sramBuf=(char*)malloc(256*1024);
		mempakBuf=(char*)malloc(256*1024);
		flashramBuf=(char*)malloc(256*1024);
	}

	for( i=0; i<20; i++ )
	{
		// To clear the buffer stored in the Kaillera
		memset(&kBuffers[0], 0, sizeof(kPlayerEntry) );
		kailleraModifyPlayValues((void *) kBuffers, sizeof(kPlayerEntry));
		Sleep(40);
	}

	//KailleraState = INPLAY;
	//return;

	// Broadcast current rom setting to all other players
	if( kailleraLocalPlayerNumber == 0 )
	{
		INI_ENTRY *c = &currentromoptions;
		SetStatusBarText(0,TranslateStringByString("Upload game setting to other players"));
		sprintf(generalmessage, "!cf=%d,32bit=%d,reg=%d,fpu=%d,rdram=%d,save=%d,tlb=%d,eeprom=%d,2pass=%d,rsp=%d,link=%d",
			c->Counter_Factor,c->Assume_32bit,c->Use_Register_Caching,c->FPU_Hack,c->RDRAM_Size,c->Save_Type,
			c->Use_TLB,c->Eeprom_size,c->Advanced_Block_Analysis,c->RSP_RDP_Timing,c->Link_4KB_Blocks);
		kailleraChatSend(generalmessage);
		DEBUG_NETPLAY_MACRO(TRACE1("P0 send msg: %s", generalmessage));

		SetStatusBarText(0,TranslateStringByString("Upload cheat codes to other players"));
		KailleraUploadCheatCodes();
		kailleraAutoApplyCheat = emuoptions.auto_apply_cheat_code;
		if( NumOfActiveCheatGroups() == 0 )
			kailleraAutoApplyCheat = FALSE;

		KailleraUploadSaveFiles();


		if( kailleraAutoApplyCheat )
			Sleep(1000);
		else
			Sleep(200);
	}

	if( kailleraLocalPlayerNumber == 0 )
	{
		SetStatusBarText(0,TranslateStringByString("Kaillera wait for 1 second"));
		//for( i=0; i<25; i++ )
		//{
			// To clear the buffer stored in the Kaillera
			//kailleraModifyPlayValues((void *) kBuffers, sizeof(kPlayerEntry));
			//Sleep(40);
		//}

		SetStatusBarText(0,TranslateStringByString("Kaillera starts the game"));
		kailleraChatSend("!!!StartGame!!!");	// Tell other players to start
		kailleraModifyPlayValues((void *) kBuffers, sizeof(kPlayerEntry));
		Sleep(100);

		//KailleraState = INPLAY;
	}

	{
		// Other players should wait to receive game setting and cheat codes
		// How can I know how long time to wait
		//SetStatusBarText(0,"Waiting and receiving data from player 0");
		while( 	KailleraState != INPLAY )
		{
			kailleraModifyPlayValues((void *) kBuffers, sizeof(kPlayerEntry));
			Sleep(10);
		}
	}

	if( eepromBuf ) free(eepromBuf);
	if( sramBuf ) free(sramBuf);
	if( mempakBuf ) free(mempakBuf);
	if( flashramBuf ) free(flashramBuf);
}


char base64[64] =
{  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

int Decode64(char *input, char *output)
{
	int x;
	int y;
	int group = 0;
	int g = 18;
	char *savepos = output;

	for (x = 0; x < strlen(input); x++ )
	{
		for (y = 0; y < 64; y++)
		{
			if (base64[y] == input[x])
				break;
		}

		if (y != 64)
			group = ( group | (y<<g) );

		g -= 6;
		if ( g  < 0 )
		{
			*output ++ = (char)((group & 0xff0000)>>16);
			*output ++ = (char)((group & 0xff00)>>8);
			*output ++ = (char)((group & 0xff));
			group = 0;
			g = 18;
		}
	}

	*output = 0;

	return output-savepos;
}


void Encode64(char *input, char *output, int len)
{
	int   state = 0;
	int   i, x, old, new1, new2;

	for (i = 0; i < len; i++ )
	{
		state++;
		new2 = 32;
		switch (state)
		{
		case 1: x = input[i];
			new1 = base64[((x>>2) & 0x3F)];
			*output ++ = (char)new1;
			break;
		case 2: x = input[i];
			new1 = base64[ ( ((old<<4) & 0x30) | ( ((x>>4) & 0xF) ) )];
			*output ++ = (char)new1;
			break;
		case 3: x = input[i];
			new1  = base64[ ( ((old<<2) & 0x3C) | ( ((x>>6) & 0x3) ) )];
			new2 = base64[ (x & 0x3F)];
			*output ++ = (char)new1;
			*output ++ = (char)new2;
			state = 0;
			break;
		} /* end of switch statement */
		old = x;
	}

	switch (state)
	{
	case 0: 
		break;
	case 1: 
		new1 = base64[  ((old<<4) & 0x30) ];
		*output ++ = (char)new1;
		*output ++ = '=';
		*output ++ = '=';
		break;
	case 2: x = input[i];
		new1  = base64[ ((old<<2) & 0x3C) ];
		*output ++ = (char)new1;
		*output ++ = '=';
		break;
	}

	*output = 0;
}

void KailleraUploadFile(char* filename, char* type)
{
	char buf[256*1024];
	char zipbuf[256*1024];
	unsigned long ziplen;
	char outbuf[256*1024];
	FILE *fp = fopen(filename, "rb");

	if( fp )
	{
		LONG len;

		fseek(fp, 0, SEEK_END);
		len = ftell(fp);
		fseek(fp, 0, SEEK_SET); 

		if( len > 0 )
		{
			fread(buf, len, 1, fp);
			ziplen = 256*1024;
			compress((Bytef*)zipbuf, &ziplen, (const Bytef*)buf, len);
			Encode64(zipbuf, outbuf, ziplen);
			len = strlen(outbuf);

			{
				int num=len/100;
				int i;

				for( i=0; i<num; i++)
				{
					sprintf(buf, "!%s%d=", type,i);
					strncat(buf, outbuf+i*100, 100);
					kailleraChatSend(buf);
				}

				if( len%100 != 0 )
				{
					sprintf(buf, "!%s%d=", type,i);
					strcat(buf, outbuf+i*100);
					kailleraChatSend(buf);
				}

				sprintf(buf, "!%sEND", type);
				kailleraChatSend(buf);
			}
		}
		fclose(fp);
	}
}



void KailleraDownloadFiles_SaveStrings(char *line)
{
	char **buf=0;
	int pos=0;
	int seq;
	char *startp;
	char *ext="fal";

	if( strncmp(line, "!EEPROM", 7) == 0 )
	{
		buf = &eepromBuf;
		pos = 7;
		ext = "eep";
	}
	else if( strncmp(line, "!SRAM", 5) == 0 )
	{
		buf = &sramBuf;
		pos = 5;
		ext = "sra";
	}
	else if( strncmp(line, "!FLASH", 6) == 0 )
	{
		buf = &flashramBuf;
		pos = 6;
		ext = "fla";
	}
	else if( strncmp(line, "!MEMPAK", 7) == 0 )
	{
		buf = &mempakBuf;
		pos = 7;
		ext = "m0";
	}

	if( !buf )  
		return;	// Buffer is not allocated

	if( strncmp(line+pos, "END", 3) == 0 )
	{
		char outbuf[256*1024];
		char filename[256];
		int ziplen;
		char zipbuf[256*1024];
		unsigned long len;

		GetFileName(filename,ext);
		ziplen = Decode64(*buf, zipbuf);
		len = 256*1024;
		uncompress((Bytef*)outbuf, &len, (const Bytef*)zipbuf, ziplen);

		{
			FILE *fp=fopen(filename,"wb");
			if( fp )
			{
				fwrite(outbuf,len,1,fp);
				fclose(fp);
			}
		}

		free(*buf);
		*buf=NULL;
	}
	else
	{
		sscanf(line+pos,"%d", &seq);

		startp = strstr(line+pos,"=");
		if( !startp )
			return;

		startp++;
		if( *startp == 0 )
			return;

		if( strlen(startp) == 100 )
			memcpy(*buf+seq*100, startp, strlen(startp));
		else
			strcpy(*buf+seq*100, startp);
	}
}

void KailleraUploadSaveFiles()
{
	if( kailleraLocalPlayerNumber == 0 )
	{
		char filename[256];
		GetFileName(filename,"eep");
		if( PathFileExists(filename) )
		{
			// Upload EEPROM file
			KailleraUploadFile(filename, "EEPROM");
		}

		GetFileName(filename,"sra");
		if( PathFileExists(filename) )
		{
			// Upload SRAM file
			KailleraUploadFile(filename, "SRAM");
		}

		GetFileName(filename,"fla");
		if( PathFileExists(filename) )
		{
			// Upload FlashRAM file
			KailleraUploadFile(filename, "FLASH");
		}

		GetFileName(filename,"m0");
		if( PathFileExists(filename) )
		{
			// Upload Mempak file
			KailleraUploadFile(filename, "MEMPAK");
		}	
	}
}