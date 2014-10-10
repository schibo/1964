#define MXCSR_TRUNC (0x00000003<<13)
#define MXCSR_FLOOR (0x00000001<<13)
#define MXCSR_CEIL  (0x00000002<<13)
#define MXCSR_NEAR  (0x00000000<<13)

#if _MSC_VER <= 1200
#define DENY_SSE
#else
//#define DENY_SSE
#endif

extern BOOL __cdecl IsSSESupported(void);


void MOVAPS_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address);
void MOVLPS_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address);
void MOVSS_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address);
void MOVSS_RegToMemory(unsigned int Reg, unsigned int ModRM, unsigned int Address);
void ADDSS_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address);
void SUBSS_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address);
void MULSS_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address);
void DIVSS_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address);
void COMISS_MemoryWithReg(unsigned int Reg, unsigned int ModRM, unsigned int Address);
void UCOMISS_MemoryWithReg(unsigned int Reg, unsigned int ModRM, unsigned int Address);

void CVTSS2SI_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address);
void CVTTSS2SI_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address);
void LDMXCSR(unsigned int Address);