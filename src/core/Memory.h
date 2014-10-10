#pragma once

class CMemory
{
public:
	CMemory(void);
	~CMemory(void);

	static const __int32 MAXTLB = 32;

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
		unsigned __int8* RDRAM;
		unsigned __int32* C2A1;
		unsigned __int32* C1A1;
		unsigned __int32* C1A3;
		unsigned __int32* C2A2;
		unsigned __int8* ROM_Image;
		unsigned __int32* GIO_REG;
		unsigned __int8* PIF;
		unsigned __int8* ExRDRAM;
		unsigned __int8* dummyNoAccess;
		unsigned __int8* dummyReadWrite;
		unsigned __int8* dummyAllZero;
		tlb_struct	TLB[MAXTLB];
	} sMemoryState;

};
