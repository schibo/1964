#pragma once

class CState
{
private:
	static const int SHIFTER1_READ = 2;
	static const int SHIFTER1_WRITE = 0;
	static const int MAXTLB = 32;

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

	typedef struct sRegisterState
	{
		tInteger  gpr[32];
		unsigned __int32 pc;// program counter
		unsigned __int32 *g_LookupPtr;
		unsigned __int32 write_mem_rt;
		__int32 countdown_counter;
		unsigned __int32 g_pc_is_rdram;			/* This will be set when returning from a block */
		unsigned __int32 code;					/* The instruction */
		__int64  grlo;
		__int64	grhi;
		unsigned __int32 COP0Reg[32];			/* Coprocessor0 Registers */
		unsigned __int32 fpr32[64];				/* 32bit 64 items needed! */
		unsigned __int32 LLbit;					/* LoadLinked Bit */
		unsigned __int32 COP1Con[32];			/* FPControl Registers, only 0 and 31 is used */
		unsigned __int32 COP0Con[64];			
		unsigned __int32 *(*memory_read_functions[0x10000>>SHIFTER1_READ])();
		unsigned __int32 *(*memory_write_fun_eax_only[0x100000>>SHIFTER1_WRITE])();
		unsigned __int8 *sDYN_PC_LOOKUP[0x10000];
	} sRegisterState;

	typedef struct TLB
	{
		unsigned __int32	valid;
		unsigned __int32	EntryHi;
		unsigned __int32	EntryLo1;
		unsigned __int32	EntryLo0;
		unsigned __int64	PageMask;
		unsigned __int32	LoCompare;
		unsigned __int32	MyHiMask;
	} tlb_struct;

	typedef struct	MemorySTATE
	{
		unsigned __int32* ramRegs0;
		unsigned __int32* ramRegs4;
		unsigned __int32* ramRegs8;
		unsigned __int32* SP_MEM;
		unsigned __int32* SP_REG_1;
		unsigned __int32* SP_REG_2;
		unsigned __int32* DPC;
		unsigned __int32* DPS;
		unsigned __int32* MI;
		unsigned __int32* VI;
		unsigned __int32* AI;
		unsigned __int32* PI;
		unsigned __int32* RI;
		unsigned __int32* SI;
		unsigned __int8* RDRAM;				/* Size = 4MB */
		unsigned __int32* C2A1;
		unsigned __int32* C1A1;
		unsigned __int32* C1A3;
		unsigned __int32* C2A2;
		unsigned __int8* ROM_Image;
		unsigned __int32* GIO_REG;
		unsigned __int8* PIF;
		unsigned __int8* ExRDRAM;			/* Size = 4MB */
		unsigned __int8* dummyNoAccess;		/* handles crap pointers for now..band-aid'ish */
		unsigned __int8* dummyReadWrite;	/* handles crap pointers for now..band-aid'ish */
		unsigned __int8* dummyAllZero;		/* handles crap pointers for all zeros */
		tlb_struct	TLB[MAXTLB];
	} sMemoryState;

public:
	CState(void);
	virtual ~CState(void);

	sRegisterState regs;
	sMemoryState mem;

	void saveState();
	void loadState();
};