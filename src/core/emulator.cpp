/*
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    This file boots the n64 and starts the emulation thread.
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


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

#include "stdafx.h"
#include "cheatcode.h"
#include "romlist.h"
#include "float.h"
#include "InterpretedOpcodes.h"

#ifdef _DEBUG
#include "win32/windebug.h"
extern char			*DebugPrintInstruction(uint32 instruction);
extern char		    *Get_Interrupt_Name(void);
#endif
extern char			*DebugPrintInstructionWithOutRefresh(uint32 Instruction);
extern char			*DebugPrintInstr(uint32 Instruction);
extern uint32		SetException_Interrupt(uint32 pc);
BOOL __cdecl		DisplayError_AskIfContinue(char *Message, ...);

void				CloseEmulator(void);
void				RunTheInterpreter(void);
void				RunTheRegCacheWithoutOpcodeDebugger(void);
static void			RunTheRegCacheNoCheck(void);
HANDLE				CPUThreadHandle = NULL;
HANDLE				AudioMuteThreadHandle = NULL;
HANDLE				AIThreadHandle = NULL;
extern HANDLE		StopEmulatorEvent;
extern HANDLE		ResumeEmulatorEvent;
extern HANDLE       PauseEmulatorEvent;

struct EmuStatus	emustatus;
struct EmuOptions	emuoptions;
uint32				CPUdelayPC;		/* the saved Program Counter at CPU load/branch delay mode */
uint32				CPUdelay;		/* Describer if the CPU is in load/branch delay mode */

BOOL				IsBooting = FALSE;
BOOL				NeedToApplyRomWriteHack = FALSE;

int pauseEntryCount=0;
int stopEntryCount=0;

void (__stdcall StartCPUThread) (void *pVoid);

//This only mutes when paused.
void __cdecl MuteTest(void* dummy)
{
	int k;

	//This is enough times to cleanup the azi audio
	//and enough times to keep jabo directsound 1.5
	//from crashing. This is a shortcoming of the spec.
	for (k=0;k<10;k++)
	{
		AUDIO_AiUpdate(TRUE);
	}
	AUDIO_AiUpdate(FALSE);
	ExitThread(0);
}

void Mute()
{
	DWORD ThreadID;
	AUDIO_AiUpdate(FALSE);
	AudioMuteThreadHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)MuteTest,NULL,0, &ThreadID);
}

HANDLE	AudioThreadHandle = NULL;
HANDLE	AudioThreadStopEvent = NULL;
HANDLE  AudioThreadEvent = NULL;

void (__cdecl AudioThread) (void *pVoid)
{
	DWORD signal;

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);

	SetEvent(AudioThreadEvent);	//Tell that this thread is created

	while(TRUE)
	{
		signal = WaitForSingleObject( AudioThreadStopEvent, 0 ); //fix: larger milliseconds keep from getting into wrong state
		if( signal == WAIT_OBJECT_0 )
		{
			TRACE0("Audio thread is stopped");
			break;
		}

		AUDIO_AiUpdate(TRUE);
		Sleep(1);
	}

	SetEvent(AudioThreadEvent);	//Tell that this thread is terminated
	ExitThread(0);
}

/*
=======================================================================================================================
=======================================================================================================================
*/
void StartAudio(void)
{
	TRACE0("To start audio thread");
	AudioThreadHandle = (HANDLE) _beginthread(AudioThread, 0, NULL);
	WaitForSingleObject(AudioThreadEvent, INFINITE);
	TRACE0("Audio thread is started");
}

void StopAudio(void)
{
	TRACE0("To stop audio thread");
	if( AudioThreadHandle != NULL )
	{
		DWORD signal;
		SetEvent(AudioThreadStopEvent);	//Tell that this thread is terminated
		Sleep(50);
		signal = WaitForSingleObject( AudioThreadEvent, 500 );
		if( signal == WAIT_TIMEOUT )
		{
			ResetEvent(AudioThreadStopEvent);
			TerminateThread(AudioThreadHandle,0);
			TRACE0("Audio thread is terminated");
		}

		AudioThreadHandle = NULL;
	}
}

/*
 =======================================================================================================================
    Called by GUI thread to pause emulating ? =======================================================================================================================
 */
extern int MenuCausedPause;
BOOL WindowMsgLoop();
BOOL PauseEmulator(void)
{
	if(emustatus.Emu_Is_Paused) return TRUE; //This line is VERY important when pausing at user menus,
                                             //otherwise sometimes stay paused after menu exit.
	if( pauseEntryCount > 0 )
		return FALSE;	//don't re-enter this function

	pauseEntryCount++;

		AUDIO_RomClosed(); //Fixes losing audio.

    
	emustatus.reason_to_stop = EMUPAUSE;
	//emustatus.Emu_Keep_Running = FALSE;
	SetEvent( StopEmulatorEvent );

	CPU_Task_To_String(generalmessage);
	TRACE1("Try to pause, CPU is busy doing: %s", generalmessage);

	while(!emustatus.Emu_Is_Paused && emustatus.Emu_Is_Running)
	{
		WindowMsgLoop();

		//emustatus.Emu_Keep_Running = FALSE;
		//emustatus.reason_to_stop = EMUPAUSE;
		Sleep(50);
	}

	if (!MenuCausedPause)
	{
		sprintf(generalmessage, "%s - %s", gui.szWindowTitle, TranslateStringByString("Paused"));
		SetStatusBarText(0, generalmessage);
		SetWindowText(gui.hwnd1964main, generalmessage);
	}

	if( !(Audio_Is_Initialized == 1 && CoreDoingAIUpdate == 0 ) )
	{
		Mute();
	}

	pauseEntryCount--;
	return TRUE;
}

/*
 =======================================================================================================================
    Called by GUI thread to resume emulating from pausing ?    the global variable "needinit" is to pass information to the CPU thread ?    to do InitEmu() in CPU thread. Reason behind this is that OpenGL is thread-safe, ?	initialization must be done in the CPU thread ? =======================================================================================================================
 */
void ResumeEmulator(int action_after_pause)
{
	if(!emustatus.Emu_Is_Paused) return;

	emustatus.action_after_resume = action_after_pause;

	/* Apply the hack codes */
	if( emuoptions.auto_apply_cheat_code || kailleraAutoApplyCheat )
	{
		CodeList_ApplyAllCode(INGAME);
#ifdef CHEATCODE_LOCK_MEMORY
		InitCheatCodeEngineMemoryLock();
#endif
	}

	emustatus.Emu_Keep_Running = TRUE;
	CheckButton(ID_BUTTON_PLAY, TRUE);
	CheckButton(ID_BUTTON_PAUSE, FALSE);

	sprintf(generalmessage, "%s - %s", gui.szWindowTitle, TranslateStringByString("Running"));
	SetStatusBarText(0, generalmessage);
	SetWindowText(gui.hwnd1964main, generalmessage);
	SetEvent( ResumeEmulatorEvent );
	QueryPerformanceCounter(&LastSecondTime);
}

/*
 =======================================================================================================================
    Called by GUI thread to stop emulating ? =======================================================================================================================
 */
void StopEmulator(void)
{
	if( stopEntryCount > 0 )
	{
		SuspendThread(CPUThreadHandle);
		TerminateThread(CPUThreadHandle, 1);
		emustatus.Emu_Is_Running = FALSE;
		stop_profiling();
		CloseEmulator();
		ResetEvent(StopEmulatorEvent);
	}
	else
	{
		int waitcount;

		if( emustatus.Emu_Is_Paused )
		{
			ResumeEmulator(DO_NOTHING_AFTER_PAUSE);
			Sleep(10);
		}

		stopEntryCount++;

		CPU_Task_To_String(generalmessage);
		TRACE1("Try to pause, CPU is busy doing: %s", generalmessage);

		emustatus.reason_to_stop = EMUSTOP;
		SetEvent( StopEmulatorEvent );

		for( waitcount = 0; waitcount < 20; waitcount++ )
		{
			// wait up to 1 second
			WindowMsgLoop();

			//emustatus.Emu_Keep_Running = FALSE;
			//emustatus.reason_to_stop = EMUSTOP;
			Sleep(50);
			if( emustatus.Emu_Is_Running == FALSE )
				break;
		}

		if( emustatus.Emu_Is_Running )
		{
			// the CPU thread is still running after 1 second. It must be stuck inside plugins other than the CPU core.
			// here we kill the CPU thread anyway
			SuspendThread(CPUThreadHandle);
			TerminateThread(CPUThreadHandle, 1);
			emustatus.Emu_Is_Running = FALSE;
			stop_profiling();
			CloseEmulator();
		}

		stopEntryCount--;
	}

	if(Audio_Is_Initialized == 1 && CoreDoingAIUpdate == 0 )
	{
		StopAudio();	// Stop the audio thread
	}

	Sleep(50);

	AUDIO_RomClosed();
	
	if (!emustatus.Emu_Is_Resetting)
	{
		//VIDEO_RomClosed();
		CONTROLLER_RomClosed();
		if( emuoptions.UsingRspPlugin )
		{
			RSPRomClosed();
		}
		//netplay_rom_closed();
	}

	// Double confirm the video is closed. This line won't matter if the VIDEO_RomClosed is already closed
	// Just to make sure 1964 is getting out of possible fullscreen mode
	VIDEO_RomClosed();
}

/*
 =======================================================================================================================
    Called by GUI thread to switch CPU core while emulating ? =======================================================================================================================
 */
void EmulatorSetCore(int core)
{
	if(emustatus.Emu_Is_Running)
	{
		if(emustatus.cpucore != core)
		{
			if(PauseEmulator())
			{
				TRACE2("Switch CPU Core to %s, PC=%08X", emulator_type_names[core], gHWS_pc);
				emustatus.cpucore = core;
				ResumeEmulator(REFRESH_DYNA_AFTER_PAUSE);
			}
		}
	}
	else	/* Emulator is not running, then change default CPU core */
	{
		defaultoptions.Emulator = (EMULATORTYPE)core;
	}

	SetStatusBarText(4, core == DYNACOMPILER ? "D" : "I");
}

/*
 * All functions above are called by GUI ? * All functions below are used in the CPU thread ? * ? */
uint32	RDRamSizeHackSavedDWord1 = 0;
uint32	RDRamSizeHackSavedDWord2 = 0;

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void CloseEmulator(void)
{
	if(currentromoptions.Code_Check == CODE_CHECK_PROTECT_MEMORY) UnprotectAllBlocks();

	/*
	if(emustatus.reason_to_stop == VIDEOCRASH || emustatus.reason_to_stop == CPUCRASH)
	{
		PostMessage(gui.hwnd1964main, WM_COMMAND, ID_ROM_STOP, 0);
		emustatus.Emu_Keep_Running = TRUE;
		while(emustatus.Emu_Keep_Running)
		{
			Sleep(50);					// wait until wingui processing the STOP command
		}
	}
	*/

	emustatus.Emu_Is_Running = FALSE;
	Free_Dynarec();

	*(uint32 *) &gMS_RDRAM[rominfo.RDRam_Size_Hack] = RDRamSizeHackSavedDWord1;
	*(uint32 *) &gMS_RDRAM[0x2FE1C0] = RDRamSizeHackSavedDWord2;

	if (!emustatus.Emu_Is_Resetting)
	{
		VIDEO_RomClosed();
	}
}

/*
 =======================================================================================================================
    ? =======================================================================================================================
 */

void RefreshDynaDuringGamePlay(void)
{
    Init_Dynarec();
	Set_Translate_PC();
}

extern uint32 Experiment;
uint32 HydroThunder;

/*
 =======================================================================================================================
 =======================================================================================================================
 */
extern BOOL write_to_rom_flag;

void InitEmu(void)
{
	//MessageBox(0, "InitEmu", "", 0);
	FR_reg_offset = (gHWS_COP0Reg[STATUS] & 0x04000000) ? 32 : 1;
    
    Experiment = (gHWS_COP0Reg[STATUS] & 0x04000000) ? 1 : 0;
    
    if((stricmp(currentromoptions.Game_Name, "HYDRO THUNDER") == 0) ||
		(stricmp(currentromoptions.Game_Name, "TARZAN") == 0))
        HydroThunder = 1;
    else
        HydroThunder = 0;

	CPUdelay = 0;
	CPUdelayPC = 0;

	CPUNeedToDoOtherTask = FALSE;
	CPUNeedToCheckInterrupt = FALSE;
	emustatus.Emu_Is_Paused = FALSE;
	write_to_rom_flag = FALSE;
	emustatus.exception_entry_count = 0;
	emustatus.action_after_resume = DO_NOTHING_AFTER_PAUSE;
	compilerstatus.lCodePosition = 0;
	Block = 0;
	FPU_Is_Enabled = FALSE;
	memset(&compilerstatus, 0, sizeof(compilerstatus));

	vips = (float)vips_speed_limits[currentromoptions.Max_FPS];
	framecounter = 0;
	viCountPerSecond = 0;
	viTotalCount = 0;
	vi_field_number = 0;
	QueryPerformanceCounter(&LastVITime);
	QueryPerformanceCounter(&LastSecondTime);

	Init_Count_Down_Counters();

	memcpy(&HeaderDllPass[0], &gMS_ROM_Image[0], 0x40);
	
	if (!emustatus.Emu_Is_Resetting)
	{
		VIDEO_RomOpen();
		CONTROLLER_RomOpen();
		//netplay_rom_open();
	}
    

	RefreshDynaDuringGamePlay();
    emustatus.Emu_Is_Resetting = 0;
}

/*
 =======================================================================================================================
    This function is called at the beginning of emulating, runs until boot successfully ? =======================================================================================================================
 */
void N64_Boot(void)
{
	int		RDRam_Hacked = 0;
	uint32	bootaddr = (*(uint32 *) (gMS_ROM_Image + 8) & 0x007FFFFF) + 0x80000000;
    static int remember_debug_opcode = 0;

   
	SetCounterFactor(CounterFactor);
	emustatus.Emu_Is_Running = TRUE;
	IsBooting = TRUE;

	NeedToApplyRomWriteHack = FALSE;
	if( strnicmp(currentromoptions.Game_Name, "A Bug's Life", 12) == 0 ||
		strnicmp(currentromoptions.Game_Name, "Toy Story 2", 11) == 0 )
	{
		NeedToApplyRomWriteHack = TRUE;
		TRACE0("Using Rom Write Hack");
	}

	while
	(
		(emustatus.Emu_Keep_Running && gHWS_pc != bootaddr && (gHWS_pc & 0x00FFFFFF) < 0x2000)
	||	((uint32) (*Dyna_Check_Codes) == (uint32) Dyna_Code_Check_None_Boot)
	)
	{
		if(emustatus.cpucore == INTERPRETER) 
		{
			InterpreterStepCPU();
		} 
		else 
           RunDynaBlock();
            
		if(RDRam_Hacked == 0)
		{
			if( (currentromoptions.timing_Control != NO_DELAY && DMAInProgress) 
				||	(emuoptions.dma_in_segments == FALSE && (MI_INTR_REG_R & MI_INTR_PI)) )
			{
				RDRam_Hacked = 1;

				RDRamSizeHackSavedDWord1 = *(uint32 *) &gMS_RDRAM[rominfo.RDRam_Size_Hack];

				OPCODE_DEBUGGER_EPILOGUE(*(uint32 *) &gMS_RDRAM[rominfo.RDRam_Size_Hack] = current_rdram_size;);
				RDRamSizeHackSavedDWord2 = *(uint32 *) &gMS_RDRAM[0x2FE1C0];

				/*
				 * Azimer - DK64 Hack to break out of infinite loop ?				 * I believe this memory location is some sort of copyright protection which ?				 * is written to using the RSP on bootup. The only issue I see is if it ?				 * affects any other roms?
				 */
				if(strncmp(currentromoptions.Game_Name, "DONKEY KONG 64", 14) == 0)
				/*
				 * if( currentromoptions.crc1 == 0xEC58EABF && currentromoptions.crc2 ==
				 * 0xAD7C7169 ) //DK64
				 */
				{
					OPCODE_DEBUGGER_EPILOGUE(*(uint32 *) &gMS_RDRAM[0x2FE1C0] = 0xAD170014;)
				}
                
                //Banjo Tooie Hack. Thanks Hacktarux
                else if ((strncmp(currentromoptions.Game_Name, "BANJO TOOIE", 11) == 0))
                {
                    int i,j;
                    
                    memcpy((uint8*)&SP_IMEM + 0x120, gMS_RDRAM + 0x1e8, 0x1e8);
                    for(j=0;j<0xfc;j++) 
                        for (i=0; i<8; i++) 
                            *(uint8*)(gMS_RDRAM+((0x2fb1f0+j*0xff0+i)^3))=
                            *(uint8*)((uint8*)&SP_IMEM+((0x120+j*8+i)^3));
                }
			}
		}
	}

	if(emuoptions.auto_apply_cheat_code || kailleraAutoApplyCheat)
	{
		CodeList_ApplyAllCode(BOOTUPONCE);
#ifdef CHEATCODE_LOCK_MEMORY
		InitCheatCodeEngineMemoryLock();
#endif
	}

	emustatus.Emu_Is_Running = FALSE;
	IsBooting = FALSE;

	if(gHWS_pc == bootaddr)
	{
		TRACE1("N64 boot successfully, start run from %08X", bootaddr)
	}
	else
	{
		TRACE1("N64 boot failed, start run from %08X", gHWS_pc)
	}
}

void __cdecl	LogDyna(char *debug, ...);

//Don't Put any MessageBoxes in this function or it will crash.

#define iMxcsr  (0x1f80|0x8000)
int iMXCSR = iMxcsr; //Mask all floating point exceptions, and set flush to zero mode in SSE
int iMXCSR_TRUNC = iMxcsr|MXCSR_TRUNC;
int iMXCSR_NEAR =  iMxcsr|MXCSR_NEAR;
int iMXCSR_CEIL =  iMxcsr|MXCSR_CEIL;
int iMXCSR_FLOOR=  iMxcsr|MXCSR_FLOOR;

extern uint32 HardwareStart;

//It is critical to use a separate function for __try and __except,
//because it uses EBP, and EBP is part of our dyna.
void (__stdcall StartCPUThreadLoop) (void *pVoid);
void (__stdcall StartCPUThread) (void *pVoid)
{
#ifdef	_DEBUG
	StartCPUThreadLoop(pVoid);
#else
    __try{
        StartCPUThreadLoop(pVoid);
    }
    __except(NULL, EXCEPTION_CONTINUE_EXECUTION)
    {
        char op_str[0xff];
        
			sprintf (op_str, "%08X: %s", gHWS_pc, TranslateStringByString("Unknown Exception in emulation thread"));
        MessageBox(0, op_str, TranslateStringByString("Error"), MB_ICONINFORMATION);
        PostMessage(gui.hwnd1964main, WM_COMMAND, ID_ROM_STOP, 0);
        WaitForSingleObject(StopEmulatorEvent, 1000);

		if(Audio_Is_Initialized == 1 && CoreDoingAIUpdate == 0 )
		{
			StopAudio();	// Stop the audio thread
		}

		Sleep(50);

		AUDIO_RomClosed();

		if (!emustatus.Emu_Is_Resetting)
		{
			//VIDEO_RomClosed();
			CONTROLLER_RomClosed();
			if( emuoptions.UsingRspPlugin )
			{
				RSPRomClosed();
			}
		}

    }
#endif

    stop_profiling();
	CloseEmulator();
	ExitThread(0);
}


void (__stdcall StartCPUThreadLoop) (void *pVoid)
{
	int lEnd1964 = 0; //unknown exception exits 1964

	TRACE0("");
	TRACE0("");
	TRACE0("");
	TRACE1("Starting ROM %s", rominfo.name)
	TRACE0("");
	TRACE0("");
	TRACE0("");

	pauseEntryCount=0;
	stopEntryCount=0;

	InitEmu();
    RestoreOldRoundingMode(((uint32) cCON31 & 0x00000003) << 8);
	
    N64_Boot();

    //BugFix: Do not remove this! Jet Force Gemini needs it.
    RefreshDynaDuringGamePlay();


	emustatus.reason_to_stop = EMURUNNING;
	DO_PROFILIER_R4300I
	{
START_CPU_THREAD:
		emustatus.Emu_Is_Running = TRUE;
		switch(emustatus.cpucore)
		{
		case INTERPRETER:
			TRACE0("Start Interpreter");
			RunTheInterpreter();
			break;
		case DYNACOMPILER:

_begin:
//#ifndef _DEBUG	
			__try 
//#endif
			{
				if((uint32) (*Dyna_Check_Codes) == (uint32) Dyna_Code_Check_None)
				{
					TRACE0("Start RunTheRegCacheNoCheck");
					RunTheRegCacheNoCheck();
					lEnd1964 = 1;
				}
				else
				{
					TRACE0("Start RunTheRegCacheWithoutOpcodeDebugger");
					RunTheRegCacheWithoutOpcodeDebugger();
					lEnd1964 = 1;
				}
				}
				// 1964 exception vector
//#ifndef _DEBUG
				__except(NULL, EXCEPTION_EXECUTE_HANDLER)
				{
					lEnd1964 = 0;
					switch (compilerstatus.schiboException)
					{
					case EXCEPTION_INTERRUPT:
					case EXCEPTION_CP1_UNUSABLE:
						Set_Translate_PC();

						break;
					default:
						lEnd1964 = 1;
					}
					compilerstatus.schiboException = EXCEPTION_NONE;
				}
				if (!lEnd1964)
					goto _begin;
//#endif
			break;
		}
		if(emustatus.reason_to_stop == EMUPAUSE)
		{
			/*
			 * If user is saving state after pausing, need to update the COUNT register here ?			 * so we can return back to the original timer value ?			 * This make Donkey Kong can be saved state
			 */
			gHWS_COP0Reg[COUNT] = Get_COUNT_Register();

            PauseEmulating();
			goto START_CPU_THREAD;
		}
	}
}

void (__stdcall StartAIInterruptThread)(void* pVoid)
{
	for(;;)
    {
    /* set the interrupt to fire */
	(MI_INTR_REG_R) |= MI_INTR_AI;
	if((MI_INTR_MASK_REG_R) & MI_INTR_AI)
	{
//		SET_EXCEPTION(EXC_INT) gHWS_COP0Reg[CAUSE] |= CAUSE_IP3;
//		HandleInterrupts(0x80000180);
        AUDIO_AiUpdate(FALSE);
    }
    Sleep(1);
    }
}


/*
 =======================================================================================================================
    Use in CPU thread, will pausing emu and wait for resume ? =======================================================================================================================
 */
void PauseEmulating(void)
{
	DWORD signal;

	emustatus.Emu_Is_Paused = TRUE;

    SetEvent(PauseEmulatorEvent);
    while(TRUE)
	{
		signal = WaitForSingleObject( ResumeEmulatorEvent, 1000 ); //fix: larger milliseconds keep from getting into wrong state

		if( signal == WAIT_OBJECT_0 )
		{
			break;	// go back to run
		}

		emustatus.Emu_Is_Paused = TRUE;
		Sleep(200);
	}

	emustatus.Emu_Is_Paused = FALSE;

	if(emustatus.Emu_Keep_Running)
	{
		Free_Dynarec();

		if(emustatus.action_after_resume == INIT_EMU_AFTER_PAUSE)
			InitEmu();
		else if(emustatus.action_after_resume == REFRESH_DYNA_AFTER_PAUSE)
			RefreshDynaDuringGamePlay();

		emustatus.exception_entry_count = 0;
	}
}

/*
 =======================================================================================================================
    This is a main loop for emulating in interpreter 
 =======================================================================================================================
 */

N64::CRegisters r;
void RunTheInterpreter(void)
{
	register uint32 Instruction;
	N64::CInterpretedOpcodes* interpreter = new N64::CInterpretedOpcodes;

_DoOtherTask:
	Instruction = FetchInstruction();	/* Fetch instruction at PC */
	interpreter->executeFunction(_OPCODE_, r, Instruction);
	gHWS_GPR(0) = 0;
	INTERPRETER_DEBUG_INSTRUCTION(Instruction);

	if(CPUNeedToCheckException)
	{
		gHWS_pc = SetException_Interrupt(gHWS_pc);
		TRACE3
		(
			"Start Exception %d, EPC=%08X, PC=%08X",
			(gHWS_COP0Reg[CAUSE] & EXCCODE) >> 2,
			gHWS_COP0Reg[EPC],
			gHWS_pc
		) CPUNeedToCheckException = FALSE;
	}
	else
	{
		switch(CPUdelay)
		{
		case 0:		gHWS_pc += 4; break;
		case 1:		gHWS_pc += 4; CPUdelay = 2; break;
		default:	gHWS_pc = CPUdelayPC; CPUdelay = 0; if(!emustatus.Emu_Keep_Running) goto out; break;
		}

		if(r.r_.countdown_counter <= 0) Trigger_Timer_Event();
	}

	r.r_.countdown_counter -= VICounterFactors[CounterFactor];
	goto _DoOtherTask;

out:
	if(Is_CPU_Doing_Other_Tasks() || CPUdelay != 0) goto _DoOtherTask;

	if (interpreter)
		delete interpreter;
}

/*
 * This is a main loop for emulating in Dynarec ? */
uint32	HardwareStart = (uint32) & r.r_.gpr + 128+8;

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void RunTheRegCacheWithoutOpcodeDebugger(void)
{
	/*
	 * __asm pushad // nono. This causes stack overflow in Minimize size compiler
	 * option.
	 */

	__asm mov ebp, HardwareStart
	while(emustatus.Emu_Keep_Running)
	{
_DoOtherTask:
		Block = (uint8 *) *r.r_.g_LookupPtr;
		if(Block != NULL && r.r_.g_pc_is_rdram) 
			Dyna_Check_Codes();

		if(Block == NULL)
		{
			Dyna_Compile_Block();
		}

		/* Run the compiled code in the Block */
		DEBUG_PRINT_DYNA_EXECUTION_INFO;
		__asm call Block;
		
		if(r.r_.countdown_counter > 0)
			goto _DoOtherTask;

		Trigger_Timer_Event();
	}

	if( emustatus.reason_to_stop != EMUSTOP && Is_CPU_Doing_Other_Tasks() ) 
		goto _DoOtherTask;

	/* __asm popad //nono. */
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */

int StartTimeHi;
int StartTimeLo;
extern int ProcessingComplete;

	static int tempzor;
	int peecee;
	static int increment=0;
static void RunTheRegCacheNoCheck(void)
{
	__asm mov tempzor, ebp
	__asm mov ebp, HardwareStart

	__asm align 16
    

    
	while(emustatus.Emu_Keep_Running)
	{
_NextBlock:
		__asm
		{
l1:        mov eax, r.r_.g_LookupPtr

#ifdef PROFILE_BLOCKS
			mov ebx, gHWS_pc;
			mov peecee, ebx
#endif

			mov ebx, [eax]
			mov ecx, dword ptr[ebp-128]//r.r_.gpr[1]//dword ptr [ebp-120] //dummy load gpr[1].lo for cache
			and ebx, ebx
			je l2
#ifdef PROFILE_BLOCKS
			rdtsc			; debug
			push edx		; debug
			push eax		; debug*/
#endif
			call ebx
		}

#ifdef PROFILE_BLOCKS
			
		__asm {
			pop esi			; debug
			pop edi			; debug
			rdtsc			; debug
			sub edx, edi	; debug
			sub eax, esi	; debug*/
			mov StartTimeLo, eax
		}

		if (increment++ > 5000000)
		{
			DisplayError("%08X: %d clocks.", peecee, StartTimeLo);
			increment = 5000001;
		}
#endif
		__asm{
			cmp r.r_.countdown_counter, 0
			jg l1
			jmp l3
l2 :		call Dyna_Compile_Block
			call eax
			cmp r.r_.countdown_counter, 0
			jg l1
l3 :
		}

		Trigger_Timer_Event();
	}

	if( emustatus.reason_to_stop != EMUSTOP && Is_CPU_Doing_Other_Tasks() ) 
		goto _NextBlock;

	__asm mov ebp, tempzor
}

// _____________________________________________________________________________
// This function performs routine interrupt checking.
// Only interrupts will enter here, exceptions will be served elsewhere.
//
void
rc_Intr_Common( void)
{
	gHWS_COP0Reg[EPC] = gHWS_pc;
	gHWS_COP0Reg[STATUS] |= EXL;
	gHWS_pc = 0x80000180;
	gHWS_COP0Reg[CAUSE] &= NOT_BD;
}


/*
 =======================================================================================================================
    Check and execute all other tasks, called by emulating main loop ?    Will do DMA, interrupt checking and so on. ? =======================================================================================================================
 */
void CPU_Check_Interrupts(void)
{
	if(emustatus.cpucore == INTERPRETER)	/* intepreter mode */
	{
		if
		(
			(gHWS_COP0Reg[STATUS] & EXL_OR_ERL /* 0x00000004 */ ) == 0	/* No in another interrupt routine */
		&&	(
				(gHWS_COP0Reg[STATUS] & 0x00000001) != 0				/* Interrupts are enabled */
			&&	((gHWS_COP0Reg[CAUSE] & gHWS_COP0Reg[STATUS] & 0x0000FF00) != 0)
			)
		)
		{
			gHWS_pc = SetException_Interrupt(gHWS_pc);
			CPUNeedToCheckInterrupt = FALSE;
			DEBUG_INTERRUPT_TRACE(TRACE1("Interrupt is being served, Interrupt=%s", Get_Interrupt_Name()));
		}
	}
	else	/* Dyna mode */
	{
		if
		(
			(gHWS_COP0Reg[STATUS] & EXL_OR_ERL) == 0		/* No in another interrupt routine */
		&&	(
				(gHWS_COP0Reg[STATUS] & 0x00000001) != 0	/* Interrupts are enabled */
			&&	((gHWS_COP0Reg[CAUSE] & gHWS_COP0Reg[STATUS] & 0x0000FF00) != 0)
			)
		)
		{
			DEBUG_INTERRUPT_TRACE(TRACE1("Interrupt is being served, Interrupt=%s", Get_Interrupt_Name()));
			rc_Intr_Common();
			Set_Translate_PC();
			CPUNeedToCheckInterrupt = FALSE;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void CPUDoOtherTasks(void)
{
	CPUNeedToDoOtherTask = FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void CPU_Task_To_String(char *str)
{
	if(!CPUNeedToDoOtherTask)
	{
		strcpy(str, "Nothing");
		return;
	}

	str[0] = '\0';

	if(DMAInProgress || CPUNeedToCheckInterrupt)
	{
		if(DMAInProgress) strcat(str, " DMA");
		if(CPUNeedToCheckInterrupt) strcat(str, " Interrupt");
	}
	else
		CPUNeedToDoOtherTask = FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void ClearCPUTasks(void)
{
	CPUNeedToDoOtherTask = FALSE;
	if(currentromoptions.timing_Control != NO_DELAY )
	{
		DMAInProgress = FALSE;
		PIDMAInProgress = NO_DMA_IN_PROGRESS;
		SIDMAInProgress = NO_DMA_IN_PROGRESS;
		SPDMAInProgress = NO_DMA_IN_PROGRESS;
	}

	CPUNeedToCheckInterrupt = FALSE;
}

/*
 =======================================================================================================================
    Step and run one opcode in interpreter mode ? =======================================================================================================================
 */
void InterpreterStepCPU(void)
{
	uint32	Instruction;

	Instruction = FetchInstruction();
	CPU_instruction[_OPCODE_](Instruction);
	INTERPRETER_DEBUG_INSTRUCTION(Instruction);

    gHWS_GPR(0) = 0;
    
    if(CPUNeedToCheckException)
	{
		gHWS_pc = SetException_Interrupt(gHWS_pc);
		TRACE3
		(
			"Start Exception %d, EPC=%08X, PC=%08X",
			(gHWS_COP0Reg[CAUSE] & EXCCODE) >> 2,
			gHWS_COP0Reg[EPC],
			gHWS_pc
		) CPUNeedToCheckException = FALSE;

       
        r.r_.countdown_counter -= VICounterFactors[CounterFactor];
	    if(r.r_.countdown_counter <= 0)
            Trigger_Timer_Event();
	    if(Is_CPU_Doing_Other_Tasks())
            CPUDoOtherTasks();
    }
	else
	{
        switch(CPUdelay)
		{
		case 0:

            gHWS_pc += 4; 

            r.r_.countdown_counter -= VICounterFactors[CounterFactor];
	        if(r.r_.countdown_counter <= 0)
                Trigger_Timer_Event();
            if(Is_CPU_Doing_Other_Tasks())
                CPUDoOtherTasks();            
            break;
		case 1:	

            gHWS_pc += 4;
            CPUdelay = 2; 

            //no delay slot interrupt processing. Risky.
            r.r_.countdown_counter -= VICounterFactors[CounterFactor];
            break;
		default:

            gHWS_pc = CPUdelayPC; 
            CPUdelay = 0; 

	        r.r_.countdown_counter -= VICounterFactors[CounterFactor];
	        if(r.r_.countdown_counter <= 0)
                Trigger_Timer_Event();
            if(Is_CPU_Doing_Other_Tasks())
                CPUDoOtherTasks();            
            
            break;
		}
	}

}

/*
 =======================================================================================================================
    Return the instruction at current PC ?    This is a utility function, called by some debug function ? =======================================================================================================================
 */
uint32 FetchInstruction(void)
{
	if(NOT_IN_KO_K1_SEG(gHWS_pc))
	{
		register uint32 translatepc;

		ITLB_Error = FALSE;
		translatepc = Direct_TLB_Lookup(gHWS_pc, TLB_INST);
		if(ITLB_Error)
		{
			if((gHWS_COP0Reg[STATUS] & EXL) == 0)	/* Exception not in exception */
			{
				if(CPUdelay != 0)					/* are we in branch delay slot? */
				{	/* yes */
					TLB_TRACE(TRACE1("ITLB exception happens in CPU delay slot, pc=%08X", gHWS_pc));
					gHWS_COP0Reg[CAUSE] |= BD;
					gHWS_COP0Reg[EPC] = gHWS_pc - 4;
					CPUdelay = 0;
				}
				else
				{	/* no */
					gHWS_COP0Reg[CAUSE] &= NOT_BD;
					gHWS_COP0Reg[EPC] = gHWS_pc;
				}

				gHWS_COP0Reg[STATUS] |= EXL;	/* set EXL = 1 */
			}
			else
			{
				TLB_TRACE(TRACE0("ITLB exception happens in exception"));
			}

			TLB_TRACE(TRACE1("ITLB exception, PC=%08X", gHWS_pc));
			gHWS_pc = TLB_Error_Vector;
			gHWS_COP0Reg[CAUSE] &= NOT_BD;		/* clear BD */
			return MEM_READ_UWORD(gHWS_pc);
		}
		else
		{
			return MEM_READ_UWORD(translatepc);
		}
	}
	else
		return MEM_READ_UWORD(gHWS_pc);
}

/*
 =======================================================================================================================
    This inline function will run a block of code in Dyna, will compile it first if needed 
 =======================================================================================================================
 */
void RunDynaBlock(void)
{

		Block = (uint8 *) *r.r_.g_LookupPtr;
		if(Block != NULL && r.r_.g_pc_is_rdram) Dyna_Check_Codes();
		if(Block == NULL) { Dyna_Compile_Block(); }
		DEBUG_PRINT_DYNA_EXECUTION_INFO 
		__asm pushad
		__asm mov ebp, HardwareStart
		__asm call Block
		__asm popad


		if(r.r_.countdown_counter <= 0) 
		{
			Trigger_Timer_Event();
		}
}

#define EXCEPTION_MAX_ENTRY 10

extern void __cdecl error(char *Message, ...);
/*
 =======================================================================================================================
    This routine serves exceptions in dynarec ?    This service routine could be re-entered
 =======================================================================================================================
 */


extern void __cdecl Set_Translate_PC_No_Check(void);
void New_Dyna_Exception_Service_Routine(int ReturnPC)
{
	if((gHWS_COP0Reg[STATUS] & EXL) == 0)	/* Exception not in exception code */
	{
        if (gHWS_COP0Reg[CAUSE] & BD)
            gHWS_COP0Reg[EPC] = ReturnPC-4;
        else
            gHWS_COP0Reg[EPC] = ReturnPC;

		gHWS_COP0Reg[STATUS] |= EXL;		/* set EXL = 1 */
	}

	else
	{
		//
		// If exception is within another exception, service the exception ?
		// without setting EPC


        if (gHWS_COP0Reg[CAUSE] & BD)
            gHWS_COP0Reg[EPC] = ReturnPC-4;
        else
            gHWS_COP0Reg[EPC] = ReturnPC;

        gHWS_COP0Reg[STATUS] |= EXL;		// set EXL = 1
	}
}

void Dyna_Exception_Service_Routine(uint32 vector)
{
	uint8	*SavedBlock = Block;
	uint32	temppc = gHWS_pc;

	emustatus.exception_entry_count++;
	emustatus.processing_exception = TRUE;

	if(emustatus.exception_entry_count == 1) SetStatusBarText(4, "E");

	if((gHWS_COP0Reg[STATUS] & EXL) == 0)	// Exception not in exception code
	{
//		if (gHWS_COP0Reg[CAUSE] & BD)
//			gHWS_COP0Reg[EPC] = gHWS_pc-4;
//		else
			gHWS_COP0Reg[EPC] = gHWS_pc;

		gHWS_COP0Reg[STATUS] |= EXL;		// set EXL = 1
	}
	else
	{
		//
		// If exception is within another exception, service the exception ?
		// without setting EPC
		//
//		if (gHWS_COP0Reg[CAUSE] & BD)
//			gHWS_COP0Reg[EPC] = gHWS_pc-4;
//		else
			gHWS_COP0Reg[EPC] = gHWS_pc;

		// temppc = gHWS_COP0Reg[EPC];
	}

	if(gHWS_COP0Reg[STATUS] & BEV)
	{
		//if( !DisplayError_AskIfContinue("Exception happens during boot.") )
		//{
		//	emustatus.Emu_Keep_Running=FALSE;
			//PostMessage(gui.hwnd1964main, WM_COMMAND, ID_ROM_STOP, 0);
			//emustatus.exception_entry_count--;
			//return;
		//}
	}

	// How about branch delay ??
	gHWS_pc = vector;

	Set_Translate_PC();
	gHWS_COP0Reg[CAUSE] &= NOT_BD;			// clear BD

	DEBUG_EXCEPTION_TRACE
		(
		TRACE2
		(
		"Start Exception Service in Dyna, exception=%d, EPC=%08X",
		(gHWS_COP0Reg[CAUSE] & EXCCODE) >> 2,
		gHWS_COP0Reg[EPC]
		)
			);


		// We need to run this loop because TLB exception could be generated during compiling time
		// need to be handled right away.
		// Other time of exception is actually don't need to be run in this loop
		while(temppc != gHWS_pc && (emustatus.Emu_Keep_Running))
		{
			//WindowMsgLoop();

	//			if( (emustatus.cpucore == INTERPRETER) || (compilerstatus.Is_Compiling > 0) )
      //              MessageBox(0, "Interpret", "", 0);
                
                if( (emustatus.cpucore == INTERPRETER) || (compilerstatus.Is_Compiling > 0) )
				{
					//MessageBox(0, "Exception During Compile!", "", 0);
					
                    InterpreterStepCPU();
				}
				else
				{
					//MessageBox(0, "Exception During Execution!", "", 0);
					//				__asm int 3;
					RunDynaBlock();
				}
		}

		Block = SavedBlock;

		DEBUG_EXCEPTION_TRACE(TRACE0("Finish Exception Service in Dyna"));

		emustatus.exception_entry_count--;
		if(emustatus.exception_entry_count == 0)
			SetStatusBarText(4, emustatus.cpucore == DYNACOMPILER ? "D" : "I");

}


void DoOthersBeforeSaveState()
{
	AI_LEN_REG = AUDIO_AiReadLength();
}

extern void InitFrameBufferProtection(void);
void DoOthersAfterLoadState()
{
	emustatus.Emu_Is_Resetting = 1;//well, it's not resetting, but we don't want InitEmu() to reload the video plugin.
    InitEmu();
    
	CPUNeedToCheckInterrupt = TRUE;
	CPUNeedToDoOtherTask = TRUE;
	Set_Check_Interrupt_Timer_Event();


	RestoreOldRoundingMode(((uint32) cCON31 & 0x00000003) << 8);
    Experiment = (gHWS_COP0Reg[STATUS] & 0x04000000) ? 1 : 0;
    
    /* Check FPU usage bit */
	if(currentromoptions.FPU_Hack == USEFPUHACK_YES )
	{
		if( gHWS_COP0Reg[STATUS] & SR_CU1)
		{
			DisableFPUUnusableException();
		}
		else
		{
			EnableFPUUnusableException();
		}
	}

    
    if(rominfo.TV_System == TV_SYSTEM_NTSC)
		AUDIO_AiDacrateChanged(0);
	else
		AUDIO_AiDacrateChanged(1);

    AI_LEN_REG = AUDIO_AiReadLength();

	if( CoreDoingAIUpdate )
	{
		AUDIO_AiUpdate(FALSE);
	}
	
	if( emustatus.VideoPluginSupportingFrameBuffer )
	{
		InitFrameBufferProtection();
	}
}