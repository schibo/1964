/*$T OpcodeDebugger.c GC 1.136 03/09/02 17:49:31 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    The opcode debugger wil run interpretive and dynarec opcodes "side-by-side" and the states of each
    virtual machines can be compared either after each instruction, or at the end of each block. It is a great way to
    detect bugs, but it was a bitch to write. To use it, build the "Win32 - Release Opcode Debugger" MS VisualC
    configuration. The end-user build is just "Win32 Release". Be sure to not use "Assume 32bit" when running the
    debugger.
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

#include "../stdafx.h"
#include "opcodeDebugger.h"

char					*r4300i_RegNames[32];
char					*r4300i_COP0_RegNames[32];
char					*r4300i_COP1_RegNames[32];


dyn_cpu_instr		dyna_instruction[64] = {
	special, regimm, j,    jal,    beq,    bne,    blez,   bgtz,
	addi,    addiu,  slti, sltiu,  andi,   ori,    xori,   lui,
	cop0,    cop1,   cop2, resrvd, beql,   bnel,   blezl,  bgtzl,
	daddi,   daddiu, ldl,  ldr,    resrvd, resrvd, resrvd, resrvd,
	lb,      lh,     lwl,  lw,     lbu,    lhu,    lwr,    lwu,
	sb,      sh,     swl,  sw,     sdl,    sdr,    swr,    cache,
	ll,      lwc1,   lwc2, resrvd, lld,    ldc1,   ldc2,   ld,
	sc,      swc1,   swc2, resrvd, scd,    sdc1,   sdc2,   sd
};