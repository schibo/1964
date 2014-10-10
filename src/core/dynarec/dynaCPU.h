/*$T dynaCPU.h GC 1.136 03/09/02 16:12:25 */


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
#ifndef __1964_DYNACPU_H
#define __1964_DYNACPU_H

#define _SAFETY_CPU_(x)

            

extern void resrvd(OP_PARAMS);
extern void invld(OP_PARAMS);
extern void special(OP_PARAMS);
extern void regimm(OP_PARAMS);
extern void j(OP_PARAMS);
extern void jal(OP_PARAMS);
extern void beq(OP_PARAMS);
extern void bne(OP_PARAMS);
extern void blez(OP_PARAMS);
extern void bgtz(OP_PARAMS);
extern void addi(OP_PARAMS);
extern void addiu(OP_PARAMS);
extern void slti(OP_PARAMS);
extern void sltiu(OP_PARAMS);
extern void andi(OP_PARAMS);
extern void ori(OP_PARAMS);
extern void xori(OP_PARAMS);
extern void lui(OP_PARAMS);
extern void cop0(OP_PARAMS);
extern void cop1(OP_PARAMS);
extern void cop1_with_exception(OP_PARAMS);
extern void cop2(OP_PARAMS);
extern void beql(OP_PARAMS);
extern void bnel(OP_PARAMS);
extern void blezl(OP_PARAMS);
extern void bgtzl(OP_PARAMS);
extern void daddi(OP_PARAMS);
extern void daddiu(OP_PARAMS);
extern void ldl(OP_PARAMS);
extern void ldr(OP_PARAMS);
extern void lb(OP_PARAMS);
extern void lh(OP_PARAMS);
extern void lwl(OP_PARAMS);
extern void lw(OP_PARAMS);
extern void lbu(OP_PARAMS);
extern void lhu(OP_PARAMS);
extern void lwr(OP_PARAMS);
extern void lwu(OP_PARAMS);
extern void sb(OP_PARAMS);
extern void sh(OP_PARAMS);
extern void swl(OP_PARAMS);
extern void sw(OP_PARAMS);
extern void sdl(OP_PARAMS);
extern void sdr(OP_PARAMS);
extern void swr(OP_PARAMS);
extern void cache(OP_PARAMS);
extern void ll(OP_PARAMS);
extern void lwc1(OP_PARAMS);
extern void lwc2(OP_PARAMS);
extern void lld(OP_PARAMS);
extern void ldc1(OP_PARAMS);
extern void ldc2(OP_PARAMS);
extern void ld(OP_PARAMS);
extern void sc(OP_PARAMS);
extern void swc1(OP_PARAMS);
extern void swc2(OP_PARAMS);
extern void scd(OP_PARAMS);
extern void sdc1(OP_PARAMS);
extern void sdc2(OP_PARAMS);
extern void sd(OP_PARAMS);
extern void shift(OP_PARAMS); /* sll, srl, sra */
extern void shiftv(OP_PARAMS);		/* sllv, srlv, srav */
extern void jr(OP_PARAMS);
extern void jalr(OP_PARAMS);
extern void syscall(OP_PARAMS);
extern void Break(OP_PARAMS);
extern void sync(OP_PARAMS);
extern void mf_mt(OP_PARAMS);
extern void dsllv(OP_PARAMS);
extern void dsrlv(OP_PARAMS);
extern void dsrav(OP_PARAMS);
extern void mul(OP_PARAMS);
extern void Div(OP_PARAMS);
extern void divides(OP_PARAMS);
extern void divu(OP_PARAMS);
extern void dmult(OP_PARAMS);
extern void dmultu(OP_PARAMS);
extern void ddiv(OP_PARAMS);
extern void ddivu(OP_PARAMS);
extern void add(OP_PARAMS);
extern void addu(OP_PARAMS);
extern void sub(OP_PARAMS);
extern void subu(OP_PARAMS);
extern void and(OP_PARAMS);
extern void or(OP_PARAMS);
extern void xor(OP_PARAMS);
extern void nor(OP_PARAMS);
extern void slt(OP_PARAMS);
extern void sltu(OP_PARAMS);
extern void dadd(OP_PARAMS);
extern void daddu(OP_PARAMS);
extern void dsub(OP_PARAMS);
extern void dsubu(OP_PARAMS);
extern void tge(OP_PARAMS);
extern void tgeu(OP_PARAMS);
extern void tlt(OP_PARAMS);
extern void tltu(OP_PARAMS);
extern void teq(OP_PARAMS);
extern void tne(OP_PARAMS);
extern void dsll(OP_PARAMS);
extern void dsrl(OP_PARAMS);
extern void dsra(OP_PARAMS);
extern void dsll32(OP_PARAMS);
extern void dsrl32(OP_PARAMS);
extern void dsra32(OP_PARAMS);
extern void _bltz(OP_PARAMS);
extern void _bgez(OP_PARAMS);
extern void _bltzl(OP_PARAMS);
extern void _bgezl(OP_PARAMS);
extern void _tgei(OP_PARAMS);
extern void _tgeiu(OP_PARAMS);
extern void _tlti(OP_PARAMS);
extern void _tltiu(OP_PARAMS);
extern void _teqi(OP_PARAMS);
extern void _tnei(OP_PARAMS);
extern void _bltzal(OP_PARAMS);
extern void _bgezal(OP_PARAMS);
extern void _bltzall(OP_PARAMS);
extern void _bgezall(OP_PARAMS);
extern void mf(OP_PARAMS);
extern void dmf(OP_PARAMS);
extern void cf(OP_PARAMS);
extern void mt(OP_PARAMS);
extern void dmt(OP_PARAMS);
extern void ct(OP_PARAMS);
extern void bc(OP_PARAMS);
extern void cop0_tlb(OP_PARAMS);
extern void bcf(OP_PARAMS);
extern void bct(OP_PARAMS);
extern void bcfl(OP_PARAMS);
extern void bctl(OP_PARAMS);
extern void cop0_tlbr(OP_PARAMS);
extern void cop0_tlbwi(OP_PARAMS);
extern void cop0_tlbwr(OP_PARAMS);
extern void cop0_tlbp(OP_PARAMS);
extern void cop0_eret(OP_PARAMS);
extern void no_cop2(OP_PARAMS);

typedef void (*dyn_cpu_instr) (OP_PARAMS);

extern dyn_cpu_instr	dyna_instruction[64];
extern dyn_cpu_instr	now_do_dyna_instruction[64];
extern dyn_cpu_instr	dyna_special_instruction[64];
extern dyn_cpu_instr	dyna_regimm_instruction[32];
extern dyn_cpu_instr	dyna_CP0_rs_instruction[32];
extern dyn_cpu_instr	dyna_CP0_rt_instruction[32];
extern dyn_cpu_instr	dyna_tlb_instruction[64];
extern dyn_cpu_instr	dyna_cop2_rs_instruction[32];

extern void SetRdRsRt32bit(HardwareState *reg);
extern void SetRdRsRt64bit(HardwareState *reg);
#endif
