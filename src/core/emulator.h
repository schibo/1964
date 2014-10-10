/*$T emulator.h GC 1.136 02/28/02 07:57:45 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
#ifndef _EMULATOR_H__1964_
#define _EMULATOR_H__1964_

#include "debug_option.h"
#include "dynarec/dynalog.h"

enum { WORDTYPE, HWORDTYPE, BYTETYPE, DWORDTYPE, NOCHECKTYPE };
enum { INIT_EMU_AFTER_PAUSE, REFRESH_DYNA_AFTER_PAUSE, DO_NOTHING_AFTER_PAUSE };
/* 1964 exception types */
enum { EXCEPTION_NONE=0, EXCEPTION_INTERRUPT, EXCEPTION_CP1_UNUSABLE };
enum GAME_STOP_REASON { EMURUNNING, EMUSTOP, EMUPAUSE, EMUSWITCHCORE, EMURESUME, VIDEOCRASH, CPUCRASH };

extern HANDLE CPUThreadHandle;
extern HANDLE AIThreadHandle;
extern int Audio_Is_Initialized;

struct EmuStatus
{
	int						DListCount;
	int						AListCount;
	int						PIDMACount;
	int						ControllerReadCount;
	BOOL					Emu_Is_Running;
	BOOL					Emu_Is_Paused;	/* means the emu is still in running state, but just paused */
	BOOL					Emu_Is_Resetting;	/* means the emu is still in running state, but just paused */
	int						CodeCheckMethod;
	int						exception_entry_count;
	int						schiboException;
	BOOL					processing_exception;
	int						cpucore;
	enum GAME_STOP_REASON	reason_to_stop;
	int						action_after_resume;
	volatile BOOL			Emu_Keep_Running;
	BOOL					VideoPluginSupportingFrameBuffer;
	BOOL					FrameBufferProtectionNeedToBeUpdate;
	BOOL					VideoPluginProvideFrameBufferInfo;
	BOOL					viframeskip;

	char					lastVideoPluginLoaded[256];
	char					lastAudioPluginLoaded[256];
	char					lastInputPluginLoaded[256];
	char					lastRSPPluginLoaded[256];
};
extern struct EmuStatus emustatus;

struct EmuOptions
{
	BOOL	auto_apply_cheat_code;
	BOOL	auto_run_rom;
	BOOL	auto_full_screen;
	BOOL	dma_in_segments;
	BOOL	SyncVI;
	BOOL	AutoFrameSkip;
    BOOL	AutoCF; //Automatic Counter Factor Timing, If TRUE, CF constantly changes automatically during gameplay.
	BOOL	UsingRspPlugin;
	BOOL	UsingInternalRSP;
};
extern struct EmuOptions	emuoptions;

extern uint8				*RDRAM_Copy;
extern uint8				HeaderDllPass[0x40];

void						ClearCPUTasks(void);
void						InterpreterStepCPU(void);
uint32						FetchInstruction(void);
__forceinline void			RunDynaBlock(void);
void						PauseEmulating(void);
BOOL						PauseEmulator(void);
void						ResumeEmulator(int action_after_pause);
void						StopEmulator(void);
void						EmulatorSetCore(int core);
extern void (*Dyna_Code_Check[]) ();
extern void (*Dyna_Check_Codes) ();
void	Dyna_Code_Check_None_Boot(void);
void	Dyna_Code_Check_QWORD(void);
void	Dyna_Exception_Service_Routine(uint32 vector);
void	Invalidate4KBlock(uint32 addr, char *opcodename, int type, uint64 newvalue);
void	CPU_Task_To_String(char *str);
void    RefreshDynaDuringGamePlay(void);

#ifdef _DEBUG
#define INTERPRETER_DEBUG_INSTRUCTION(inst) \
	if(DebuggerActive) \
	{ \
		HandleBreakpoint(inst); \
		if(DebuggerOpcodeTraceEnabled) \
		{ \
			DebugPrintInstruction(inst); \
			RefreshDebugger(); \
		} \
	}
#else
#define INTERPRETER_DEBUG_INSTRUCTION(inst)
#endif
#ifdef _DEBUG
#define DYNA_DEBUG_INSTRUCTION(inst) \
	/* FlushAllRegisters(); */ \
	if(DebuggerActive && (DebuggerOpcodeTraceEnabled || DebuggerBreakPointActive)) \
	{ \
		rc_DYNDEBUG_UPDATE(inst) DEBUG_BPT(inst) \
	}
#else
#define DYNA_DEBUG_INSTRUCTION(inst)
#endif
#ifdef _DEBUG
#ifdef LOG_DYNA
#define DYNA_LOG_INSTRUCTION	if(debugoptions.debug_dyna_log) \
		LogDyna("\n%s\n", DebugPrintInstructionWithOutRefresh(gHWS_code));
#else
#define DYNA_LOG_INSTRUCTION
#endif
#else /* release */
#ifdef LOG_DYNA
#define DYNA_LOG_INSTRUCTION LogDyna("\n%s\n", DebugPrintInstructionWithOutRefresh(gHWS_code));
#else
#define DYNA_LOG_INSTRUCTION
#endif
#endif
#ifdef DEBUG_DYNAEXECUTION
#define DEBUG_PRINT_DYNA_EXECUTION_INFO if(debugoptions.debug_dyna_execution) \
	{ \
		sprintf(generalmessage, "Dyna execution: PC = %08X", gHWS_pc); \
		RefreshOpList(generalmessage); \
	}
#else
#define DEBUG_PRINT_DYNA_EXECUTION_INFO
#endif
#ifdef DEBUG_DYNA
#define DEBUG_PRINT_DYNA_COMPILE_INFO	if(debugoptions.debug_dyna_compiler) \
	{ \
		sprintf(generalmessage, "Dyna compile: memory %08X - %08X", compilerstatus.TempPC, gHWS_pc); \
		RefreshOpList(generalmessage); \
	}
#else
#define DEBUG_PRINT_DYNA_COMPILE_INFO
#endif
#define C_CALL( /* address */ OPCODE) \
	MOV_ImmToReg(Reg_EAX, (uint32) /* & */ OPCODE); \
	CALL_Reg(Reg_EAX);

#ifdef _DEBUG
#define rc_DYNDEBUG_UPDATE(Inst) \
	/* FlushAllRegisters(); */ \
	MOV_ImmToMemory(1, ModRM_disp32, (_u32) & gHWS_pc, gHWS_pc); \
	MOV_ImmToReg(Reg_ECX, Inst); \
	C_CALL((uint32) & WinDynDebugPrintInstruction)
#define DEBUG_BPT(inst) \
	MOV_ImmToReg(Reg_ECX, inst); \
	C_CALL((uint32) & HandleBreakpoint);

#else
#define rc_DYNDEBUG_UPDATE
#define DEBUG_BPT
#endif
#endif
