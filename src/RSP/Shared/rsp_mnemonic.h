#ifndef __RSP_MNEMONIC_H
#define __RSP_MNEMONIC_H

typedef struct sJAL
{
	int FunctionStartAddress; //if 0, not a function at this address
	int FunctionEndAddress;

} sJAL;

typedef struct
{
	_u32	type;
	_u32	flags;

	_u32	ucode_boot;
	_u32	ucode_boot_size;

	_u32	ucode;
	_u32	ucode_size;

	_u32	ucode_data;
	_u32	ucode_data_size;

	_u32	dram_stack;
	_u32	dram_stack_size;

	_u32	output_buff;
	_u32	output_buff_size;

	_u32	data_ptr;
	_u32	data_size;

	_u32	yield_data_ptr;
	_u32	yield_data_size;
} OSTask_t;

typedef union {
	OSTask_t		t;
	_u64	force_structure_alignment;
} OSTask;

typedef struct {
	BOOL	used;
	_u32	crc_size;
	_u32	crc_800;
	_u32	ucode;
	_u32	minor_ver;
	_u32	variant;
	char	rspstr[200];

	_u32	ucStart;
	_u32	ucSize;
	_u32	ucDStart;
	_u32	ucDSize;
	_u32	ucCRC;
	_u32	ucDWORD1;
	_u32	ucDWORD2;
	_u32	ucDWORD3;
	_u32	ucDWORD4;
} UcodeInfo;

extern UcodeInfo	lastUcodeInfo;
extern BOOL			ucodeDetected;
extern _u32			gUcode;

typedef void (*pMicrocodeFunc)(_u32 Instruction);

typedef struct {

	_u32	startAddr;
	_u32	endAddr;
	char*	functionName;

} UcodeFunctionEntry;

typedef struct {

	_u32 IMEMaddr;
	_u32 RDRAMaddr;
	_u32 DMALength;
	_u32 dword1;
	_u32 dword2;
	_u32 dword3;
	_u32 dword4;
	pMicrocodeFunc functionAddress;

} IMEM_DMA_FunctionMapType;


typedef struct {

	char				*filename;
	char				*functionBootCode;
	char				*functionData;
	pMicrocodeFunc		functionAddress;
	UcodeFunctionEntry	*functions;
	int					noOfFunctions;
	IMEM_DMA_FunctionMapType *DMAMaps;
	int					noOfDMAMaps;

} UcodeBaseMaps;

extern UcodeBaseMaps ucodeInfos[];

typedef struct {

	_u32 *array;
	_u32 size;
	char *funcname;

} HLEHexEntry;

extern HLEHexEntry HLEHexData[];


//#define SPCYCLE(n)	{ SPCycleCount += n;}
#define SPCYCLE(n)

#endif // __RSP_MNEMONIC_H
