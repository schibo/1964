/*$T opcodeDebugger.h GC 1.136 02/28/02 08:33:48 */


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
#ifndef __OPCODE_DEBUGGER_H
#define __OPCODE_DEBUGGER_H

extern uint8			*TLB_sDWord[0x100000];
extern uint8			*sDWord[0x10000];

#define sDWORD_R			sDWord
#define TLB_sDWORD_R		TLB_sDWord
#define gHWS_GPR(n)			r.r_.gpr[n].s64
#define gHWS_COP0Reg		gHardwareState.COP0Reg
#define gHWS_fpr32			gHardwareState.fpr32
#define gHWS_COP1Reg		gHardwareState.COP1Reg
#define gHWS_COP1Con		gHardwareState.COP1Con
#define gHWS_COP0Con		gHardwareState.COP0Con
#define gHWS_LLbit			gHardwareState.LLbit
#define gHWS_pc				r.r_.pc
#define gHWS_code			r.r_.code
#define gMS_ramRegs0		gMemoryState.ramRegs0
#define gMS_ramRegs4		gMemoryState.ramRegs4
#define gMS_ramRegs8		gMemoryState.ramRegs8
#define gMS_SP_MEM			gMemoryState.SP_MEM
#define gMS_SP_REG_1		gMemoryState.SP_REG_1
#define gMS_SP_REG_2		gMemoryState.SP_REG_2
#define gMS_DPC				gMemoryState.DPC
#define gMS_DPS				gMemoryState.DPS
#define gMS_MI				gMemoryState.MI
#define gMS_VI				gMemoryState.VI
#define gMS_AI				gMemoryState.AI
#define gMS_PI				gMemoryState.PI
#define gMS_RI				gMemoryState.RI
#define gMS_SI				gMemoryState.SI
#define gMS_RDRAM			gMemoryState.RDRAM
#define gMS_C2A1			gMemoryState.C2A1
#define gMS_C1A1			gMemoryState.C1A1
#define gMS_C1A3			gMemoryState.C1A3
#define gMS_C2A2			gMemoryState.C2A2
#define gMS_ROM_Image		gMemoryState.ROM_Image
#define gMS_GIO_REG			gMemoryState.GIO_REG
#define gMS_PIF				gMemoryState.PIF
#define gMS_ExRDRAM			gMemoryState.ExRDRAM
#define gMS_dummyNoAccess	gMemoryState.dummyNoAccess
#define gMS_dummyReadWrite	gMemoryState.dummyReadWrite
#define gMS_dummyAllZero	gMemoryState.dummyAllZero
#define gMS_TLB				gMemoryState.TLB
#define OPCODE_DEBUGGER_EPILOGUE(x) x

extern uint32			PcBeforeBranch;
extern uint32			BlockStartPC;
#endif
