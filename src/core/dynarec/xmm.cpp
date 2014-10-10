//Floating point SSE opcodes that use the XMM registers.


#include "../stdafx.h"

char SSEReason[0xff];

//This function checks for support of SSE
BOOL __cdecl IsSSESupported()
{
    static int IsSupported = FALSE;
    static int AlreadyChecked = 0;

    if (AlreadyChecked)
        return IsSupported;

    AlreadyChecked = 1;

    strcpy(SSEReason, "No Hardware Support Detected");

//#define DENY_SSE
#ifdef DENY_SSE
    IsSupported = FALSE;
    strcpy(SSEReason, "Disabled in this build");
    return IsSupported;
#else
    
    //Check for Hardware Support

    __asm
    {
        mov eax, 1
        cpuid
        test edx, 0x02000000 ;bit 25 in feature flags equal to 1
        jz NotFound
    }

    //Check for OS Support
    __try {
        __asm xorps xmm0, xmm0 ;Streaming SIMD Extension
        IsSupported = TRUE;
        strcpy(SSEReason, "Hardware and OS Support Detected");
    }
    __except(NULL, EXCEPTION_EXECUTE_HANDLER) {
        IsSupported = FALSE;
        strcpy(SSEReason, "Hardware Support, but No OS Support Detected");
    }

NotFound:
    return IsSupported;
#endif
}

void MOVAPS_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address)
{
    WC8(0x0f);
    WC8(0x28);
    Encode_Slash_R(Reg, ModRM, Address);
}


void MOVLPS_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address)
{
    WC8(0x0f);
    WC8(0x12);
    Encode_Slash_R(Reg, ModRM, Address);
}

void MOVSS_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address)
{
    if ((0xC0|Reg) == ModRM) 
    {
        return;
    }

    WC8(0xf3);
    WC8(0x0f);
    WC8(0x10);
    Encode_Slash_R(Reg, ModRM, Address);
}


void MOVSS_RegToMemory(unsigned int Reg, unsigned int ModRM, unsigned int Address)
{

    if ((0xC0|Reg) == ModRM) 
    {
        return;
    }


    WC8(0xf3);
    WC8(0x0f);
    WC8(0x11);
    Encode_Slash_R(Reg, ModRM, Address);
}


void ADDSS_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address)
{
    WC8(0xF3);
    WC8(0x0F);
    WC8(0x58);
    Encode_Slash_R(Reg, ModRM, Address);
}

void SUBSS_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address)
{
    WC8(0xF3);
    WC8(0x0F);
    WC8(0x5C);
    Encode_Slash_R(Reg, ModRM, Address);
}

void MULSS_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address)
{
    WC8(0xF3);
    WC8(0x0F);
    WC8(0x59);
    Encode_Slash_R(Reg, ModRM, Address);
}

void DIVSS_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address)
{
    WC8(0xF3);
    WC8(0x0F);
    WC8(0x5E);
    Encode_Slash_R(Reg, ModRM, Address);
}

void COMISS_MemoryWithReg(unsigned int Reg, unsigned int ModRM, unsigned int Address)
{
    WC8(0x0F);
    WC8(0x2F);
    Encode_Slash_R(Reg, ModRM, Address);
}

void UCOMISS_MemoryWithReg(unsigned int Reg, unsigned int ModRM, unsigned int Address)
{
    WC8(0x0F);
    WC8(0x2E);
    Encode_Slash_R(Reg, ModRM, Address);
}

void CVTSS2SI_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address)
{
    WC8(0xF3);
    WC8(0x0F);
    WC8(0x2D);
    Encode_Slash_R(Reg, ModRM, Address);
}

void CVTTSS2SI_MemoryToReg(unsigned int Reg, unsigned int ModRM, unsigned int Address)
{
    WC8(0xF3);
    WC8(0x0F);
    WC8(0x2C);
    Encode_Slash_R(Reg, ModRM, Address);
}


void LDMXCSR(unsigned int Address)
{
    WC8(0x0F);
    WC8(0xAE);
    WC8(0x15); // slash2
    WC32(Address);
}