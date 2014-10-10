/*$T interrupt.c GC 1.136 03/09/02 17:29:10 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    Interrupt and Exception service routines
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
#include <float.h>

extern void		Init_Timer_Event_List(void);
extern uint32	TLB_Error_Vector;
extern void		zlist_uncompress(OSTask_t *task);

uint32			sp_hle_task = 0;
LARGE_INTEGER	LastVITime = { 0, 0 };
LARGE_INTEGER	LastSecondTime = { 0, 0 };
LARGE_INTEGER	Freq;
LARGE_INTEGER	CurrentTime;
LARGE_INTEGER	Elapsed;
double			tempvips;


void rdp_fullsync()
//Hack..gets rsp working for basic video plugin+rsp+starfox.
{
	MI_INTR_REG_R |= MI_INTR_DP;

//	if((MI_INTR_MASK_REG_R) & MI_INTR_MASK_DP)
//	{
//		SET_EXCEPTION(EXC_INT) gHWS_COP0Reg[CAUSE] |= CAUSE_IP3;
//		HandleInterrupts(0x80000180);
//	}
}
/*
 =======================================================================================================================
    This function is called when an Exception Interrupt happens. It sets Coprocessor0 registers EPC and CAUSE, then
    returns the correct interrupt vector address
 =======================================================================================================================
 */
uint32 SetException_Interrupt(uint32 pc)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	newpc = 0x80000180;
	/*~~~~~~~~~~~~~~~~~~~~~~~*/

	Count_Down(4 * VICounterFactors[CounterFactor]);

	if(CPUdelay != 0)				/* are we in branch delay slot? */
	{								/* yes */
		gHWS_COP0Reg[CAUSE] |= BD;
		gHWS_COP0Reg[EPC] = pc - 4;
		CPUdelay = 0;
	}
	else
	{								/* no */
		gHWS_COP0Reg[CAUSE] &= NOT_BD;
		gHWS_COP0Reg[EPC] = pc;
	}

	gHWS_COP0Reg[STATUS] |= EXL;	/* set EXL = 1 */

	/* to disable further interrupts */
	if((gHWS_COP0Reg[CAUSE] & EXCCODE) == TLBL_Miss || (gHWS_COP0Reg[CAUSE] & EXCCODE) == TLBS_Miss)
	{
		newpc = TLB_Error_Vector;
	}

	return newpc;
}

extern uint32 HydroThunder;
/*
 =======================================================================================================================
    Plugins will call this function to trigger an Audio Interface Interrupt, Display Processor Interrupt, or Signal
    Processor Interrupt.
 =======================================================================================================================
 */
void CheckInterrupts(void)
{
	if((MI_INTR_REG_R & MI_INTR_SP) != 0)
	{
		OPCODE_DEBUGGER_EPILOGUE(Trigger_RSPBreak();)
	}

    
	if((MI_INTR_REG_R & MI_INTR_DP) != 0)
	{
		OPCODE_DEBUGGER_EPILOGUE(Trigger_DPInterrupt();)
	}

	/*
	 * if ((MI_INTR_REG_R & MI_INTR_VI) != 0) { OPCODE_DEBUGGER_BEGIN_EPILOGUE
	 * Trigger_VIInterrupt(); OPCODE_DEBUGGER_END_EPILOGUE }
	 */
	if((MI_INTR_REG_R & MI_INTR_AI) != 0)
	{
		if(currentromoptions.timing_Control != NO_DELAY )
		{
			MI_INTR_REG_R &= ~MI_INTR_AI;
			Set_Delay_AI_Interrupt_Timer_Event(200);
		}
		else
		{
			OPCODE_DEBUGGER_EPILOGUE(Trigger_AIInterrupt();)
		}
	}
}

void DummyCheckInterrupts()
{
	if( currentromoptions.timing_Control == DELAY_DMA_AI || currentromoptions.timing_Control == DELAY_DMA_SI_AI )
	{
		// CPU core will handle AI interrupts, don't accept AI interrupts from audio plugin
		if((MI_INTR_REG_R & MI_INTR_AI) != 0)
		{
			MI_INTR_REG_R &= ~MI_INTR_AI;
		}
	}
	else
	{
		CheckInterrupts();
	}
}

/*
 =======================================================================================================================
    A write to the MIPS?Interface Interrupt Mask Register will set or clear an interrupt bitmask flag(s) in this
    register.
 =======================================================================================================================
 */
void Handle_MI(uint32 value)
{
	if((value & MI_INTR_MASK_SP_SET)) MI_INTR_MASK_REG_R |= MI_INTR_SP;
	else if((value & MI_INTR_MASK_SP_CLR)) MI_INTR_MASK_REG_R &= ~MI_INTR_SP;

    if((value & MI_INTR_MASK_SI_SET)) MI_INTR_MASK_REG_R |= MI_INTR_SI;
	else if((value & MI_INTR_MASK_SI_CLR)) MI_INTR_MASK_REG_R &= ~MI_INTR_SI;

	if((value & MI_INTR_MASK_AI_SET)) MI_INTR_MASK_REG_R |= MI_INTR_AI;
    else if((value & MI_INTR_MASK_AI_CLR)) MI_INTR_MASK_REG_R &= ~MI_INTR_AI;

	if((value & MI_INTR_MASK_VI_SET)) MI_INTR_MASK_REG_R |= MI_INTR_VI;
    else if((value & MI_INTR_MASK_VI_CLR)) MI_INTR_MASK_REG_R &= ~MI_INTR_VI;

	if((value & MI_INTR_MASK_PI_SET)) MI_INTR_MASK_REG_R |= MI_INTR_PI;
    else if((value & MI_INTR_MASK_PI_CLR)) MI_INTR_MASK_REG_R &= ~MI_INTR_PI;

	if((value & MI_INTR_MASK_DP_SET)) MI_INTR_MASK_REG_R |= MI_INTR_DP;
    else if((value & MI_INTR_MASK_DP_CLR)) MI_INTR_MASK_REG_R &= ~MI_INTR_DP;

	/* Check MI interrupt again. This is important, otherwise we will lose interrupts. */
	if(MI_INTR_MASK_REG_R & 0x0000003F & MI_INTR_REG_R)
	{
		/* Trigger an MI interrupt since don't know what it is */
		SET_EXCEPTION(EXC_INT) gHWS_COP0Reg[CAUSE] |= CAUSE_IP3;
		HandleInterrupts(0x80000180);
	}
}

/*
 =======================================================================================================================
    A write to the MIPS?Interface Mode Register will set or clear an interrupt bitmask flag(s) in this register.
 =======================================================================================================================
 */
void WriteMI_ModeReg(uint32 value)
{
	if(value & MI_SET_RDRAM) MI_INIT_MODE_REG_R |= MI_MODE_RDRAM;
	else if(value & MI_CLR_RDRAM) MI_INIT_MODE_REG_R &= ~MI_MODE_RDRAM;

	if(value & MI_SET_INIT) MI_INIT_MODE_REG_R |= MI_MODE_INIT;
    else if(value & MI_CLR_INIT) MI_INIT_MODE_REG_R &= ~MI_MODE_INIT;

	if(value & MI_SET_EBUS) MI_INIT_MODE_REG_R |= MI_MODE_EBUS;
    else if(value & MI_CLR_EBUS) MI_INIT_MODE_REG_R &= ~MI_MODE_EBUS;

	if(value & MI_CLR_DP_INTR)
	{
		Clear_MIInterrupt(NOT_MI_INTR_DP);
	}
}

/*
 =======================================================================================================================
    A write to the Signal Processor Status Register will set or clear an interrupt bitmask flag(s) in this register.
 =======================================================================================================================
 */
void Handle_SP(uint32 value)
{
    if(value & SP_CLR_BROKE) {
        (SP_STATUS_REG) &= ~SP_STATUS_BROKE;
    }

    if(value & SP_SET_INTR)
    {
        (MI_INTR_REG_R) |= MI_INTR_SP;
	    if((MI_INTR_REG_R & MI_INTR_MASK_REG_R) != 0)
		{
			SET_EXCEPTION(EXC_INT) gHWS_COP0Reg[CAUSE] |= CAUSE_IP3;
			HandleInterrupts(0x80000180);
		}
	}
    //to use else if here is a possible bux fix
    else if(value & SP_CLR_INTR)
    {
        Clear_MIInterrupt(NOT_MI_INTR_SP);
    }

    if(value & SP_SET_SSTEP) (SP_STATUS_REG) |= SP_STATUS_SSTEP;
    else if(value & SP_CLR_SSTEP) (SP_STATUS_REG) &= ~SP_STATUS_SSTEP;

	if(value & SP_SET_INTR_BREAK) (SP_STATUS_REG) |= SP_STATUS_INTR_BREAK;
	else if(value & SP_CLR_INTR_BREAK) (SP_STATUS_REG) &= ~SP_STATUS_INTR_BREAK;

	if(value & SP_SET_YIELD) (SP_STATUS_REG) |= SP_STATUS_YIELD;
    else if(value & SP_CLR_YIELD) (SP_STATUS_REG) &= ~SP_STATUS_YIELD;
	
	if(value & SP_SET_YIELDED) (SP_STATUS_REG) |= SP_STATUS_YIELDED;
    else if(value & SP_CLR_YIELDED) (SP_STATUS_REG) &= ~SP_STATUS_YIELDED;
	
	if(value & SP_SET_TASKDONE) (SP_STATUS_REG) |= SP_STATUS_TASKDONE;
    else if(value & SP_CLR_YIELDED) (SP_STATUS_REG) &= ~SP_STATUS_YIELDED;
	
	if(value & SP_SET_SIG3) (SP_STATUS_REG) |= SP_STATUS_SIG3;
    else if(value & SP_CLR_SIG3) (SP_STATUS_REG) &= ~SP_STATUS_SIG3;
	
	if(value & SP_SET_SIG4) (SP_STATUS_REG) |= SP_STATUS_SIG4;
    else if(value & SP_CLR_SIG4) (SP_STATUS_REG) &= ~SP_STATUS_SIG4;
	
	if(value & SP_SET_SIG5) (SP_STATUS_REG) |= SP_STATUS_SIG5;
    else if(value & SP_CLR_SIG5) (SP_STATUS_REG) &= ~SP_STATUS_SIG5;
	
	if(value & SP_SET_SIG6) (SP_STATUS_REG) |= SP_STATUS_SIG6;
    else if(value & SP_CLR_SIG6) (SP_STATUS_REG) &= ~SP_STATUS_SIG6;
	
	if(value & SP_SET_SIG7) (SP_STATUS_REG) |= SP_STATUS_SIG7;
    else if(value & SP_CLR_SIG7) (SP_STATUS_REG) &= ~SP_STATUS_SIG7;

	if(value & SP_SET_HALT) (SP_STATUS_REG) |= SP_STATUS_HALT;
    else if(value & SP_CLR_HALT)
	{
		if ( ( SP_STATUS_REG & SP_STATUS_BROKE ) == 0 ) //bugfix.
		{
			(SP_STATUS_REG) &= ~SP_STATUS_HALT;
			DEBUG_SP_TASK_MACRO(TRACE0("SP Task is triggered"));
			sp_hle_task = HLE_DMEM_TASK;
			RunSPTask();
		}
 	}

	///* Add by Rice, 2001.08.10 */
	//SP_STATUS_REG |= SP_STATUS_HALT;  //why?
}

/*
 =======================================================================================================================
    A write to the DPC Status Register will set or clear an interrupt bitmask flag(s) in this register.
 =======================================================================================================================
 */
void Handle_DPC(uint32 value)
{
	if(value & DPC_CLR_XBUS_DMEM_DMA) (DPC_STATUS_REG) &= ~DPC_STATUS_XBUS_DMEM_DMA;
	if(value & DPC_SET_XBUS_DMEM_DMA) (DPC_STATUS_REG) |= DPC_STATUS_XBUS_DMEM_DMA;

	if(value & DPC_CLR_FREEZE) (DPC_STATUS_REG) &= ~DPC_STATUS_FREEZE;
	if(value & DPC_SET_FREEZE) (DPC_STATUS_REG) |= DPC_STATUS_FREEZE;

	if(value & DPC_CLR_FLUSH) (DPC_STATUS_REG) &= ~DPC_STATUS_FLUSH;
	if(value & DPC_SET_FLUSH) (DPC_STATUS_REG) |= DPC_STATUS_FLUSH;
	
	/*
    if(value & DPC_CLR_TMEM_REG) (DPC_TMEM_REG) = 0;
	if(value & DPC_CLR_PIPEBUSY_REG) (DPC_PIPEBUSY_REG) = 0;
	if(value & DPC_CLR_BUFBUSY_REG) (DPC_BUFBUSY_REG) = 0;
	if(value & DPC_CLR_CLOCK_REG) (DPC_CLOCK_REG) = 0;
	*/
}

extern unsigned int cpuIdlePercentages[4];
extern int cpuIdlePercentageIdx;
BOOL newSecond = FALSE;
void Set_AutoCF()
{
	BOOL oldViFrameSkip;
	BOOL oldCounterFactor;
	int cpuIdlePercentage=0;
	int i;

	if( newSecond == FALSE )
		return;
	else
		newSecond = FALSE;

	// Only change AutoCF and AutoFrameSkip setting at a second boundary

	for( i=0; i<4; i++ )
	{
		cpuIdlePercentage += cpuIdlePercentages[i];
	}
	cpuIdlePercentage += cpuIdlePercentages[cpuIdlePercentageIdx];	// extra weight for the last second
	cpuIdlePercentage /= 5;	// Average the idle percentage over the past 4 seconds

	oldViFrameSkip = emustatus.viframeskip;
	oldCounterFactor = CounterFactor;


	/* Possible combinations:                                               */
	/*
	 * 1) AutoCF=On, AutoFrameSkip=On
	 *     CF=5,  emustatus.viframeskip=1    -----> (1)  ^
	 *     CF=5,  emustatus.viframeskip=0    -----> (2)  |
	 *     CF=3,  emustatus.viframeskip=0    -----> (3)  |
	 *     CF=1,  emustatus.viframeskip=0    -----> (4)  |
	 *
	 * 2) AutoCF=On, AutoFrameSkip=Off
	 *     CF=5     ----> (1)  ^
	 *     CF=3     ----> (2)  |
	 *     CF=1     ----> (3)  |
	 *
	 * 3) AutoCF=Off,AutoFrameSkip=On
	 *     emustatus.viframeskip=1     ----> (1)
	 *     emustatus.viframeskip=0     ----> (2)
	 *
	 * 4) Both off, do nothing
	 *	
	 */

	if( emuoptions.AutoCF && emuoptions.AutoFrameSkip )
	{
		// Case 1, both features are on

		if( oldViFrameSkip == 1 )
		{
			if( oldCounterFactor != 5 )
			{
				// We will check again frame rate at the next second
				emustatus.viframeskip = 0;
				AutoCounterFactor = VICounterFactors[5];
				CounterFactor = 5;
			}
			else if( cpuIdlePercentage > 15 || vips > vips_speed_limits[MAXFPS_AUTO_SYNC]*1.1f )
			{
				if( cpuIdlePercentage > 50 )
				{
					emustatus.viframeskip = 0;
					AutoCounterFactor = VICounterFactors[3];
					CounterFactor = 3;
				}
				else
				{
					emustatus.viframeskip = 0;
					AutoCounterFactor = VICounterFactors[5];
					CounterFactor = 5;
				}
			}
			else
			{
				AutoCounterFactor = VICounterFactors[5];
				CounterFactor = 5;
			}
		}
		else
		{
			// oldViFrameSkip = 0
			if( oldCounterFactor > 4 )
			{
				if( vips < vips_speed_limits[MAXFPS_AUTO_SYNC]*0.85f )
				{
					emustatus.viframeskip = 1;
					AutoCounterFactor = VICounterFactors[5];
					CounterFactor = 5;
				}

//This one is too slow                
//				else if (cpuIdlePercentage > 50 )
//				{
//					AutoCounterFactor = VICounterFactors[1];
//					CounterFactor = 1;
//				}
				else if (cpuIdlePercentage > 20 )
				{
					AutoCounterFactor = VICounterFactors[3];
					CounterFactor = 3;
				}
				else
				{
					AutoCounterFactor = VICounterFactors[5];
					CounterFactor = 5;
				}
			}
			else if( oldCounterFactor >2 )
			{
				if( vips < vips_speed_limits[MAXFPS_AUTO_SYNC]*0.6f )
				{
					emustatus.viframeskip = 1;
					AutoCounterFactor = VICounterFactors[5];
					CounterFactor = 5;
				}
				else if( vips < vips_speed_limits[MAXFPS_AUTO_SYNC]*0.9f )
				{
					AutoCounterFactor = VICounterFactors[5];
					CounterFactor = 5;
				}
//Too slow
//				else if ( cpuIdlePercentage > 20 )
//				{
//					AutoCounterFactor = VICounterFactors[1];
//					CounterFactor = 1;
//				}
				else
				{
					AutoCounterFactor = VICounterFactors[3];
					CounterFactor = 3;
				}
			}
			else
			{
				if( vips < vips_speed_limits[MAXFPS_AUTO_SYNC]*0.4f )
				{
					emustatus.viframeskip = 1;
					AutoCounterFactor = VICounterFactors[5];
					CounterFactor = 5;
				}
				else if( vips < vips_speed_limits[MAXFPS_AUTO_SYNC]*0.6f )
				{
					AutoCounterFactor = VICounterFactors[5];
					CounterFactor = 5;
				}
				else if( vips < vips_speed_limits[MAXFPS_AUTO_SYNC]*0.9f )
				{
					AutoCounterFactor = VICounterFactors[3];
					CounterFactor = 3;
				}
//Too slow
//				else
//				{
//					AutoCounterFactor = VICounterFactors[1];
//					CounterFactor = 1;
//				}
			}
		}
	}
	else if( emuoptions.AutoCF )
	{
		// case 2: AutoCF=on, AutoFrameSkip=Off

		if( oldCounterFactor > 4 )
		{
			if( vips < vips_speed_limits[MAXFPS_AUTO_SYNC]*0.9f )
			{
				// We cannot do anything else to speed up
				AutoCounterFactor = VICounterFactors[5];
				CounterFactor = 5;
			}
			else if (cpuIdlePercentage > 20 )
			{
				AutoCounterFactor = VICounterFactors[3];
				CounterFactor = 3;
			}
		}
		else if( oldCounterFactor >2 )
		{
			if( vips < vips_speed_limits[MAXFPS_AUTO_SYNC]*0.9f )
			{
				AutoCounterFactor = VICounterFactors[5];
				CounterFactor = 5;
			}
//Too slow
//			else if ( cpuIdlePercentage > 20 )
//			{
//				AutoCounterFactor = VICounterFactors[1];
//				CounterFactor = 1;
//			}
			else
			{
				AutoCounterFactor = VICounterFactors[3];
				CounterFactor = 3;
			}
		}
		else
		{
			if( vips < vips_speed_limits[MAXFPS_AUTO_SYNC]*0.6f )
			{
				AutoCounterFactor = VICounterFactors[5];
				CounterFactor = 5;
			}
			else if( vips < vips_speed_limits[MAXFPS_AUTO_SYNC]*0.9f )
			{
				AutoCounterFactor = VICounterFactors[3];
				CounterFactor = 3;
			}
//Too slow
//			else
//			{
//				AutoCounterFactor = VICounterFactors[1];
//				CounterFactor = 1;
//			}
		}
	}
	else if( emuoptions.AutoFrameSkip )
	{
		// case 3: AutoCF=off, AutoFrameSkip=on

		if( vips < vips_speed_limits[MAXFPS_AUTO_SYNC]*0.85f )
		{
			emustatus.viframeskip = 1;
		}
		else if( cpuIdlePercentage > 15 || vips > vips_speed_limits[MAXFPS_AUTO_SYNC]*1.1f )
		{
			emustatus.viframeskip = 0;
		}
	}
	else
	{
		// Both features are off
	}
}


/*
 =======================================================================================================================
    Process a Signal Processor task. This is where we call audio/video plugin execution routines.
 =======================================================================================================================
 */
extern int UsingInternalVideo;

void RunSPTask(void)
{
	DWORD SPcycleUsed = SPTASKPCLOCKS;
	DWORD DPcycleUsed = 0;

    switch(sp_hle_task)
	{
	
	case BAD_TASK:
		break;
	
	case GFX_TASK:
		{
			static int viframecount=0;
			static DWORD viorg = 0;

			DO_PROFILIER_VIDEO;

			if( viorg != VI_ORIGIN_REG )
			{
				viframecount++;
				viorg = VI_ORIGIN_REG;
			}

			if( emuoptions.AutoFrameSkip && emustatus.viframeskip == 1 && viframecount%2 == 0 )
			{
				// Skip the frame
				Trigger_DPInterrupt();
			}
			else
			{
				if( rsp_plugin_is_loaded == TRUE && emuoptions.UsingRspPlugin == TRUE )
				{
					SPcycleUsed = DoRspCycles(100);
				}
				else
				{
					DWORD cycleUsed = VIDEO_ProcessDList();
					SPcycleUsed = cycleUsed&0xFFFF;
					DPcycleUsed = cycleUsed>>16;
				}
			}

			emustatus.DListCount++;
			DPC_STATUS_REG = 0x801; /* Makes Banjo Kazooie work - Azimer */
	//		if((MI_INTR_REG_R & MI_INTR_DP) != 0) Trigger_DPInterrupt();
		}
		DEBUG_SP_TASK_MACRO(TRACE0("SP GRX Task finished"));
		break;
	case SND_TASK:
		__try
		{
			DO_PROFILIER_AUDIO;

			if( rsp_plugin_is_loaded == TRUE && emuoptions.UsingRspPlugin == TRUE )
			{
				//AUDIO_ProcessAList();
				SPcycleUsed = DoRspCycles(100);
				SPcycleUsed *=4;
			}
			else
			{
				SPcycleUsed = AUDIO_ProcessAList();   // Plugin
			}

			emustatus.AListCount++;

			if( Kaillera_Thread_Is_Running )
			{
				/* set the interrupt to fire */
				(MI_INTR_REG_R) |= MI_INTR_AI;
				if((MI_INTR_MASK_REG_R) & MI_INTR_AI)
				{
					SET_EXCEPTION(EXC_INT) gHWS_COP0Reg[CAUSE] |= CAUSE_IP3;
					HandleInterrupts(0x80000180);
				}
			}
		}

		__except(NULL, EXCEPTION_EXECUTE_HANDLER)
		{
			/* DisplayError("Memory exception fires to process AUDIO DList"); */
			DO_PROFILIER_R4300I
		}

//		if((MI_INTR_REG_R & MI_INTR_DP) != 0) Trigger_DPInterrupt();
		DEBUG_SP_TASK_MACRO(TRACE0("SP SND Task finished"));
		break;

    case JPG_TASK:
		if( rsp_plugin_is_loaded == TRUE && emuoptions.UsingRspPlugin == TRUE )
		{
			__try
			{
				SPcycleUsed = DoRspCycles(100);
			}

			__except(NULL, EXCEPTION_EXECUTE_HANDLER)
			{
				/* DisplayError("Memory exception fires to process RDP List"); */
			}
		}
		else
		{
			TRACE0("Uncompress JPEG");
			zlist_uncompress((OSTask_t *)&HLE_DMEM_TASK);
//			SPcycleUsed = AUDIO_ProcessAList();   // Plugin
		}
        break;

	default:
		//MessageBox(0, "Default", "", 0);
        __try
		{
			DO_PROFILIER_RDP;
			if( rsp_plugin_is_loaded == TRUE && emuoptions.UsingRspPlugin == TRUE )
			{
				SPcycleUsed = DoRspCycles(100);
				//VIDEO_ProcessRDPList();
			}
			else
			{
				//TODO: HLE JPEG (maybe)
 //               RunDSP(10000000);
				SPcycleUsed = 800;
			}
		}

		__except(NULL, EXCEPTION_EXECUTE_HANDLER)
		{
			/* DisplayError("Memory exception fires to process RDP List"); */
		}

//		if((MI_INTR_REG_R & MI_INTR_DP) != 0) Trigger_DPInterrupt();

		DEBUG_SP_TASK_MACRO(TRACE0("Unknown SP Task"));
		break;
	}

	//if((MI_INTR_REG_R & MI_INTR_DP) != 0) Trigger_DPInterrupt();
	DO_PROFILIER_R4300I;

	// Audio hack according to Azimer for Hydro Thunder ...
	//if( SP_STATUS_REG == 0x100)
	//{
	//	Trigger_SPInterrupt();
	//}


	if((MI_INTR_REG_R & MI_INTR_DP) != 0)
	{
		if(currentromoptions.timing_Control != NO_DELAY && emuoptions.UsingRspPlugin == FALSE && DPcycleUsed > 0 ) 
		{
			MI_INTR_REG_R &= ~MI_INTR_DP;
			DPC_STATUS_REG |= DPC_STATUS_CMD_BUSY;
			DPC_STATUS_REG |= DPC_STATUS_DMA_BUSY;
			DPC_STATUS_REG &= ~DPC_STATUS_CBUF_READY;

			Set_DP_DLIST_Timer_Event(DPcycleUsed);
		}
		else
			Trigger_DPInterrupt();
	}

	if(currentromoptions.timing_Control != NO_DELAY && emuoptions.UsingRspPlugin == FALSE && SPcycleUsed > 0) 
	{
		//if( SPcycleUsed < SPTASKPCLOCKS )
		//	SPcycleUsed = SPTASKPCLOCKS;

		//(SP_STATUS_REG) &= ~0x00000203;
		SP_STATUS_REG &= ~SP_STATUS_HALT;
		SP_STATUS_REG |= SP_STATUS_DMA_BUSY;
		SP_STATUS_REG |= SP_STATUS_IO_FULL;
		SP_STATUS_REG |= SP_STATUS_DMA_FULL;
		MI_INTR_REG_R &= ~MI_INTR_SP;
		Set_SP_DLIST_Timer_Event(SPcycleUsed);
	}
	else
		Trigger_RSPBreak();
}

/*
 =======================================================================================================================
    Flags an Audio Interface Interrupt for later execution.
 =======================================================================================================================
 */
void Trigger_AIInterrupt(void)
{
	DEBUG_AI_INTERRUPT_TRACE(TRACE0("AI Interrupt is triggered"););

	/* set the interrupt to fire */
	(MI_INTR_REG_R) |= MI_INTR_AI;
	if((MI_INTR_MASK_REG_R) & MI_INTR_AI)
	{
		SET_EXCEPTION(EXC_INT) gHWS_COP0Reg[CAUSE] |= CAUSE_IP3;
		HandleInterrupts(0x80000180);
	}

	//KAILLERA_LOG(fprintf(ktracefile, "AI at VI=%d, compare=%08X\n", viTotalCount, Get_COUNT_Register()));
}

/*
 =======================================================================================================================
    Flags a Compare Interrupt for later execution. (COUNT register = COMPARE register)
 =======================================================================================================================
 */
void Trigger_CompareInterrupt(void)
{
	DEBUG_COMPARE_INTERRUPT_TRACE
	(
	TRACE0("COUNT Interrupt is triggered"); TRACE3
		(
			"COUNT = %08X, COMPARE= %08X, PC = %08X",
			gHWS_COP0Reg[COUNT],
			gHWS_COP0Reg[COMPARE],
			gHWS_pc
		);
	);

	/* set the compare interrupt flag (ip7) */
	SET_EXCEPTION(EXC_INT) gHWS_COP0Reg[CAUSE] |= CAUSE_IP8;

	HandleInterrupts(0x80000180);
	//KAILLERA_LOG(fprintf(ktracefile, "CI at VI=%d, compare=%08X\n", viTotalCount, Get_COUNT_Register()));
}

/*
 =======================================================================================================================
    Flags an Display Processor Interrupt for later execution.
 =======================================================================================================================
 */
void Trigger_DPInterrupt(void)
{
	/* set the interrupt to fire */
	(MI_INTR_REG_R) |= MI_INTR_DP;
	if((MI_INTR_MASK_REG_R) & MI_INTR_DP)
	{
		SET_EXCEPTION(EXC_INT) gHWS_COP0Reg[CAUSE] |= CAUSE_IP3;
		HandleInterrupts(0x80000180);
	}
	//KAILLERA_LOG(fprintf(ktracefile, "DP at VI=%d, compare=%08X\n", viTotalCount, Get_COUNT_Register()));
}

/*
 =======================================================================================================================
    Flags a Video Interface Interrupt for later execution. (color frame buffer (CFB) display)
 =======================================================================================================================
 */
extern HANDLE		StopEmulatorEvent;
extern HANDLE		ResumeEmulatorEvent;
double				tempvips2;
double				tempvips3;
extern	BOOL		screenIsUpdated;

void Trigger_VIInterrupt(void)
{
	//KAILLERA_LOG(fprintf(ktracefile, "VI at compare=%08X\n", Get_COUNT_Register() ));

	{
		if( !screenIsUpdated )	// If screen is not updated by write to VIOrigin register
		{

			DO_PROFILIER_VIDEO;
			VIDEO_UpdateScreen();		// For CFB
			DO_PROFILIER_R4300I;
		}

		DEBUG_VI_INTERRUPT_TRACE(TRACE0("VI Interrupt is triggered"););
		viCountPerSecond++;
		viTotalCount++;
	}

	// safety check, the value of viCountPerSecond should not be very high, otherwise
	// the emulator must have been looped
	if( viCountPerSecond > 400 )
	{
		Sleep(2);	// Allow other threads to run
	}

	{
		// Check for GUI pause/stop/resume events
		DWORD tostop = WaitForSingleObject( StopEmulatorEvent, 0 );
		if( tostop == WAIT_OBJECT_0 )
		{
			if( emustatus.reason_to_stop == EMUPAUSE && emustatus.processing_exception == TRUE )
			{
				SetEvent(StopEmulatorEvent);
			}
			else
			{
				// To stop the emulator
				emustatus.Emu_Keep_Running = FALSE;
			}
		}
	}

	if(currentromoptions.Max_FPS != MAXFPS_NONE && emuoptions.SyncVI)
	{
		// Speed Sync
		QueryPerformanceCounter(&CurrentTime);
		Elapsed.QuadPart = CurrentTime.QuadPart - LastSecondTime.QuadPart;

        _control87(_RC_UP, _MCW_RC);
        tempvips2 = (double)(viCountPerSecond * Freq.QuadPart) / (double)Elapsed.QuadPart;

        Elapsed.QuadPart = CurrentTime.QuadPart - LastVITime.QuadPart;
		tempvips = (double)Freq.QuadPart / (double)Elapsed.QuadPart;

		
        
        if( tempvips > (double)vips_speed_limits[currentromoptions.Max_FPS] && tempvips2 > (double)vips_speed_limits[currentromoptions.Max_FPS] )
		{
			DO_PROFILIER_CPU_IDLE;

			Elapsed.QuadPart = CurrentTime.QuadPart - LastVITime.QuadPart;
			tempvips3 = (double)Freq.QuadPart / (double)Elapsed.QuadPart;
            while(tempvips3 > (double)vips_speed_limits[currentromoptions.Max_FPS])
			{
				if(tempvips3 > vips_speed_limits[currentromoptions.Max_FPS])
				{
					Sleep(1);
				}
				QueryPerformanceCounter(&CurrentTime);
				Elapsed.QuadPart = CurrentTime.QuadPart - LastVITime.QuadPart;
				tempvips3 = (double)Freq.QuadPart / (double)Elapsed.QuadPart;
			}

			DO_PROFILIER_R4300I
		}

		LastVITime = CurrentTime;
	}

	if(Audio_Is_Initialized == 1)
	{
		// Update AI
		DO_PROFILIER_AUDIO;
		if( CoreDoingAIUpdate )
		{
			AUDIO_AiUpdate(FALSE);
			if (HydroThunder)
				MI_INTR_REG_R |= MI_INTR_SP; //Fixes Hydro Thunder and Tarzan :)

		}

		DO_PROFILIER_R4300I;
	}

	/*
	{
		// Experiment for AI interrupts
		extern uint64 gAIDMAEndTimer;
		extern uint64 current_counter;

		DWORD ailen = AUDIO_AiReadLength();
		AI_STATUS_REG = gMemoryState.AI[3];

		if( ailen == 0 || gAIDMAEndTimer < current_counter )
		{
			if( (AI_STATUS_REG&0x80000001) != 0 )
			{
				Set_Delay_AI_Interrupt_Timer_Event(100);
				AI_STATUS_REG = 0;
			}
		}
	}
	*/

	if ((VI_STATUS_REG & 0x00000010) != 0) // Bit [4]     = divot_enable (normally on if antialiased, unless decal lines)
	{
		vi_field_number = 1 - vi_field_number;
	} 
	else 
	{
		vi_field_number = 0;
	}

	if(emuoptions.auto_apply_cheat_code || kailleraAutoApplyCheat)
	{
		// Apply the hack codes
#ifndef CHEATCODE_LOCK_MEMORY
		CodeList_ApplyAllCode(INGAME);
#endif
	}

	Set_AutoCF();
    
	/* set the interrupt to fire */
	(MI_INTR_REG_R) |= MI_INTR_VI;
	if((MI_INTR_MASK_REG_R) & MI_INTR_MASK_VI)
	{
		SET_EXCEPTION(EXC_INT) gHWS_COP0Reg[CAUSE] |= CAUSE_IP3;
		HandleInterrupts(0x80000180);
	}

	screenIsUpdated = FALSE;
}

/*
 =======================================================================================================================
    Flags a Serial Interface Controller interrupt for later execution
 =======================================================================================================================
 */
void Trigger_SIInterrupt(void)
{
	MI_INTR_REG_R |= MI_INTR_SI;

	if((MI_INTR_MASK_REG_R) & MI_INTR_MASK_SI)
	{
		SET_EXCEPTION(EXC_INT) gHWS_COP0Reg[CAUSE] |= CAUSE_IP3;
		HandleInterrupts(0x80000180);
	}
	//KAILLERA_LOG(fprintf(ktracefile, "SI at VI=%d, compare=%08X\n", viTotalCount, Get_COUNT_Register()));
}

/*
 =======================================================================================================================
    Flags a Peripheral Interface interrupt for later execution
 =======================================================================================================================
 */
void Trigger_PIInterrupt(void)
{
	MI_INTR_REG_R |= MI_INTR_PI;	/* Set PI Interrupt */

	if((MI_INTR_MASK_REG_R) & MI_INTR_MASK_PI)
	{
		SET_EXCEPTION(EXC_INT) gHWS_COP0Reg[CAUSE] |= CAUSE_IP3;
		HandleInterrupts(0x80000180);
	}
	//KAILLERA_LOG(fprintf(ktracefile, "PI at VI=%d, compare=%08X\n", viTotalCount, Get_COUNT_Register()));
}

/*
 =======================================================================================================================
    Flags a Signal Processor interrupt for later execution
 =======================================================================================================================
 */
void Trigger_SPInterrupt(void)
{
	MI_INTR_REG_R |= MI_INTR_SP;

	if((MI_INTR_MASK_REG_R) & MI_INTR_MASK_SP)
	{
		SET_EXCEPTION(EXC_INT) gHWS_COP0Reg[CAUSE] |= CAUSE_IP3;
		HandleInterrupts(0x80000180);
	}
	//KAILLERA_LOG(fprintf(ktracefile, "SP at VI=%d, compare=%08X\n", viTotalCount, Get_COUNT_Register()));
}

/*
 =======================================================================================================================
    Halts the Signal Processor
 =======================================================================================================================
 */
void Trigger_RSPBreak(void)
{
	/* set the status flags */
	(SP_STATUS_REG) |= 0x00000203;

	/* set the sp interrupt when wanted */
	if((SP_STATUS_REG) & SP_STATUS_INTR_BREAK)
	{
		Trigger_SPInterrupt();
	}

	/* Add by Rice 2001.08.10 */
	SP_STATUS_REG |= SP_STATUS_HALT;
}

/*
 =======================================================================================================================
    Call this function to clear AI/VI/SI/SP/DP interrupts
 =======================================================================================================================
 */
void Clear_MIInterrupt(uint32 clear_mask)
{
	MI_INTR_REG_R &= clear_mask;
	if((MI_INTR_REG_R & MI_INTR_MASK_REG_R) == 0)
	{
		gHWS_COP0Reg[CAUSE] &= NOT_CAUSE_IP3;
		if((gHWS_COP0Reg[CAUSE] & gHWS_COP0Reg[STATUS] & SR_IMASK) == 0)
            CPUNeedToCheckInterrupt = FALSE;
	}
}

/*
 =======================================================================================================================
    Should flag an Address Error exception, but just displays an error right now.
 =======================================================================================================================
 */
void Trigger_Address_Error_Exception(uint32 addr, char *opcode, int exception)
{
	DisplayError("Memory access not aligned, PC=%08X, opcode=%s", gHWS_pc, opcode);
	TRACE3("Memory access not aligned, PC=%08X, opcode=%s, Bad Vaddr=%08X", gHWS_pc, opcode, addr);

	TRACE0("Should fire Address Error Exception, but we skipped here");
#ifdef _DEBUG
	/*
	 * SET_EXCEPTION(exception) ?
	 * gHWS_COP0Reg[BADVADDR] = addr; ?
	 * HandleExceptions(0x80000180);
	 */
#endif
}

/*
 =======================================================================================================================
    Call this function to clear AI/VI/SI/SP/DP interrupts
 =======================================================================================================================
 */

int tempfpr32[64];
int tempgpr[64]; 
extern BOOL IsBooting;

void New_Dyna_Exception_Service_Routine (int ReturnPC) ;

void TriggerFPUUnusableExceptionNewer(int ReturnPC, int InDelaySlot)
{
	compilerstatus.schiboException = EXCEPTION_CP1_UNUSABLE;

	if (InDelaySlot)
		gHWS_COP0Reg[CAUSE] |= BD;
	else
		gHWS_COP0Reg[CAUSE] &= ~BD;

	SET_EXCEPTION(EXC_CPU) 
	gHWS_COP0Reg[CAUSE] &= 0xCFFFFFFF;
	gHWS_COP0Reg[CAUSE] |= CAUSE_CE1;
    New_Dyna_Exception_Service_Routine(ReturnPC);
	
	gHWS_pc = 0x80000180;

	//Raise an exception to trigger the exception vector. 
	RaiseException(1,         // exception code 
	    0,                    // continuable exception 
        0, NULL);             // no arguments 
}

void TriggerCPUInterruptExceptionNewer(int ReturnPC, int InDelaySlot)
{
	compilerstatus.schiboException = EXCEPTION_INTERRUPT;

	if (InDelaySlot)
		gHWS_COP0Reg[CAUSE] |= BD;
	else
		gHWS_COP0Reg[CAUSE] &= ~BD;

	SET_EXCEPTION(EXC_INT) 
    New_Dyna_Exception_Service_Routine(ReturnPC);

	gHWS_pc = 0x80000180;

	//Raise an exception to trigger the exception vector. 
	RaiseException(1,         // exception code 
	    0,                    // continuable exception 
        0, NULL);             // no arguments 
}

//Only the interpreter uses this function.
void TriggerFPUUnusableException(void)
{
	SET_EXCEPTION(EXC_CPU) 

    gHWS_COP0Reg[CAUSE] &= 0xCFFFFFFF;
    gHWS_COP0Reg[CAUSE] |= CAUSE_CE1;


    //Rice, this is for interpreter, but because it is a delay slot, it might be a little tricky.
    //I think if CPUdelay == 1 (and it's not 2 or zero) we will be in the delay slot.
    if (CPUdelay==1)
        gHWS_COP0Reg[CAUSE] |= BD;
    else
        gHWS_COP0Reg[CAUSE] &= ~BD;


	DEBUG_EXCEPTION_TRACE(TRACE0("FPU Unusable Exception"));

	
	if((emustatus.cpucore == DYNACOMPILER) && (compilerstatus.Is_Compiling == 0))
	{
		Dyna_Exception_Service_Routine(0x80000180);
	}
	else
	{
		CPUNeedToDoOtherTask = TRUE;
		CPUNeedToCheckException = TRUE;
	}
//	HandleExceptions(0x80000180);
//	gHWS_COP0Reg[CAUSE] &=~CAUSE_CE1;

}

extern void cop0_with_exception(OP_PARAMS);
extern void CU1_COP1_instr(uint32 Instruction);
extern void CU1_r4300i_lwc1(uint32 Instruction);
extern void CU1_r4300i_ldc1(uint32 Instruction);
extern void CU1_r4300i_swc1(uint32 Instruction);
extern void CU1_r4300i_sdc1(uint32 Instruction);

/*
 =======================================================================================================================
    Calling this function will enable detection of Coprocessor Unusable Exceptions.
 =======================================================================================================================
 */
void EnableFPUUnusableException(void)
{
    CPU_instruction[0x11] = CU1_COP1_instr;		/* this is for interpreter */
    CPU_instruction[49] = CU1_r4300i_lwc1;
    CPU_instruction[53] = CU1_r4300i_ldc1;
    CPU_instruction[57] = CU1_r4300i_swc1;
    CPU_instruction[61] = CU1_r4300i_sdc1;
}

/*
 =======================================================================================================================
    Calling this function will disable detection of Coprocessor Unusable Exceptions.
 =======================================================================================================================
 */
void DisableFPUUnusableException(void)
{
	/* this is for interpreter */	
    CPU_instruction[0x11] = COP1_instr;			
    CPU_instruction[49] = r4300i_lwc1;
    CPU_instruction[53] = r4300i_ldc1;
    CPU_instruction[57] = r4300i_swc1;
    CPU_instruction[61] = r4300i_sdc1;
}

/*
 =======================================================================================================================
    Call this function to setup interrupt triggers
 =======================================================================================================================
 */
void HandleInterrupts(uint32 vt)
{
	/* Set flag for interrupt service */
	CPUNeedToCheckInterrupt = TRUE;
	CPUNeedToDoOtherTask = TRUE;
	Set_Check_Interrupt_Timer_Event();
}

/*
 =======================================================================================================================
    Call this function to setup exception triggers
 =======================================================================================================================
 */
void HandleExceptions(uint32 evt)
{
	if(gHWS_COP0Reg[STATUS] & EXL_OR_ERL)	/* Exception in exception */
	{
		TRACE1("Warning, Exception happens in exception, the new exception is %d", (0x7C & gHWS_COP0Reg[CAUSE]) >> 2);
	}

	if(emustatus.cpucore == DYNACOMPILER)
	{
		Dyna_Exception_Service_Routine(evt);
	}
	else
	{
		CPUNeedToDoOtherTask = TRUE;
		CPUNeedToCheckException = TRUE;
	}
}

/*
 =======================================================================================================================
    This function is for debug purposes
 =======================================================================================================================
 */
void Trigger_Interrupt_Without_Mask(uint32 interrupt)
{
	/* set the interrupt to fire */
	(MI_INTR_REG_R) |= interrupt;
	SET_EXCEPTION(EXC_INT) gHWS_COP0Reg[CAUSE] |= CAUSE_IP3;
	HandleInterrupts(0x80000180);
}

