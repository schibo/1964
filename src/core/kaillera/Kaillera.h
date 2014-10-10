/*$T Kaillera.h GC 1.136 02/28/02 08:18:16 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef _KAILLERADLL_H
#define _KAILLERADLL_H

typedef struct
{
	BUTTONS			b;
	uint32			viCount;
	unsigned char	optimalDelay;
} kPlayerEntry;

extern volatile BOOL	Kaillera_Is_Running;
extern volatile BOOL	Kaillera_Thread_Is_Running;
extern int				Kaillera_Players;
extern unsigned int		Kaillera_Counter;
extern BOOL				kailleraClientStatus[4];
extern int				kailleraLocalPlayerNumber;
extern HANDLE			kailleraThreadHandle;

int					LoadDllKaillera(void);
BOOL				IsKailleraDllLoaded(void);
int					UnloadDllKaillera(void);

/*
 -----------------------------------------------------------------------------------------------------------------------
    Kaillera 0.84 spec
 -----------------------------------------------------------------------------------------------------------------------
 */
typedef struct
{
	char	*appName;
	char	*gameList;

	int (WINAPI *gameCallback) (char *game, int player, int numplayers);

	void (WINAPI *chatReceivedCallback) (char *nick, char *text);
	void (WINAPI *clientDroppedCallback) (char *nick, int playernb);

	void (WINAPI *moreInfosCallback) (char *gamename);
} kailleraInfos;

typedef enum 
{
	DLL_NOT_LOADED,		// DLL is not loaded
	GAME_IDLE,			// Is idling
	TRANSFER_DATA,		// Game is starting, transfering data between players
	INPLAY,				// Game is playing
}KailleraStateType;
extern KailleraStateType	KailleraState;
extern FILE *ktracefile;
extern FILE *ktracefile2;

extern BOOL kailleraAutoApplyCheat;

#define KAILLERA_LOG(macro)	{if( ktracefile ) macro;}
#define KAILLERA_LOG2(macro)	{if( ktracefile2 ) macro;}
//#define KAILLERA_LOG(macro)	{}
//#define KAILLERA_LOG2(macro)	{}

int kailleraGetVersion(char *version);
int kailleraInit(void);
int kailleraShutdown(void);
int kailleraSetInfos(kailleraInfos *infos);
int kailleraSelectServerDialog(HWND parent);
int kailleraModifyPlayValues(void *values, int size);
int kailleraChatSend(char *text);
int kailleraEndGame(void);

void KailleraGetPlayerKeyValues(kPlayerEntry keyvalues[4], int player);
void KailleraGetPlayerKeyValuesFor1Player(BUTTONS *pKeyValue, int player);
void __stdcall StartKailleraThread(void *pVoid);
void StopKailleraThread(void);
void SetKailleraLagness(int lag);

void KailleraResetSaveFiles(void);

// To be called when before starting the game
void KailleraRomOpen(void);

// To be called after the game is stopped
void KailleraRomClosed(void);


#endif /* _KAILLERADLL_H */
