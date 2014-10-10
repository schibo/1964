#pragma once

namespace N64
{
	class CRegisters
	{
	public:
		CRegisters(void);
		~CRegisters(void);

		typedef union Integer
		{
			__int64 s64;
			__int32 s32;
			__int16 s16;
			__int8 s8;
			unsigned __int64 u64;
			unsigned __int32 u32;
			unsigned __int16 u16;
			unsigned __int8 u8;
		} tInteger;

		typedef struct state
		{
			tInteger gpr[32];
			unsigned __int32 pc;
			unsigned __int32 *g_LookupPtr;
			unsigned __int32 write_mem_rt;
			__int32 countdown_counter;
			unsigned __int32 g_pc_is_rdram;			/* This will be set when returning from a block */
			unsigned __int32 code;					/* The instruction */
			__int64 grlo;
			__int64 grhi;
			unsigned __int32 COP0Reg[32];
			unsigned __int32 fpr32[64];
			unsigned __int32 LLbit;
			unsigned __int32 COP1Con[32];
			unsigned __int32 COP0Con[64];			
		} tRegisters;

		tRegisters r_;
	};
}