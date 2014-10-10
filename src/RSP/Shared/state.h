#ifndef __RSP_REGISTERS_H
#define __RSP_REGISTERS_H

//////////////////////////////////////////////////////////////////////
// Data Types
//////////////////////////////////////////////////////////////////////

typedef union {
	_s64			S64[2];
	_u64			U64[2];
	_s32			S32[4];
	_u32			U32[4];
	_s16			S16[8];
	_u16			U16[8];
	_s8				U8[16];
	_u8				S8[16];
} VECTOR;

typedef union {
	_s64			S64;
	_u64			U64;
	_s32			S32[2];
	_u32			U32[2];
	_s16			S16[4];
	_u16			U16[4];
	_s8				U8[8];
	_u8				S8[8];
} ALUType;

typedef struct
{
        _s32	r[32];			/* general purpose registers */
		VECTOR	v[32];		/* vector registers */
		ALUType	accum[8];
        _s16	flag[4];		/* cop2 control registers */

		VECTOR	dummy;		/* dummy source 2 vector registers */

        _u32	pc_delay;		/* delayed program counter */
        int		delay;			/* this is for correct emulation of the 6 stage pipeline */
								/* if(delay ==  0) { next instr is pc + 4 } */
								/* if(delay ==  1) { next instr is delayed pc} */
								/* if(delay == -1) { exec delayed pc} */

		_u32	code;			/* current code (needed for speed) */
        int		halt;			/* if '!0' then stop execution */

} t_state;



/* rsp_reg.delay */

#define NO_RSP_DELAY    0
#define DO_RSP_DELAY    1
#define EXEC_RSP_DELAY  2

// GPR Regs

enum {
    R0 = 0, AT, V0, V1, A0, A1, A2, A3,
    T0    , T1, T2, T3, T4, T5, T6, T7,
    S0    , S1, S2, S3, S4, S5, S6, S7,
    T8    , T9, K0, K1, GP, SP, S8, RA
};

#endif // __RSP_REGISTERS_H
