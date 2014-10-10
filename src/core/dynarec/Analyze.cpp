
//Be careful with div, ddiv, ddivu, divu. and shitvs. They protect register.

// Analyze.c: 1st dyna pass


// 1964 Copyright (C) 1999-2004 Joel Middendorf, <schibo@emulation64.com>.  This
// program is free software;  you can redistribute it and/or modify it under the
// terms of the GNU  General Public  License as  published by  the Free Software
// Foundation; either version 2 of the License,  or (at your option)  any  later
// version.  This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.  You should have received a copy of the GNU  General Public  License
// along with this program; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place  -  Suite  330,  Boston, MA  02111-1307,  USA. To contact the
// authors: email: schibo@emulation64.com, rice1964@yahoo.com

#include "../stdafx.h"

uint32 EndOfBlock;
uint32 Delay;
extern BOOL IsBooting;


void InvalidateAnalysis()
{
    int k;

    for(k = NUM_CONSTS-1; k >= 0; k--)
    {
        ConstMap[k].FinalAddressUsedAt = 0xffffffff;
    }
    EndOfBlock = 1;
}

// example:  rt = rs + immediate
int I_Type(unsigned __int32 Instruction)
{
//    InvalidateAnalysis(); return;

    ConstMap[RT_FT].FinalAddressUsedAt = gHWS_pc;
    ConstMap[RS_BASE_FMT].FinalAddressUsedAt  = gHWS_pc;
    
    return 1;
}

int BR_Type(unsigned __int32 Instruction)
{
//    InvalidateAnalysis(); return;

    ConstMap[RS_BASE_FMT].FinalAddressUsedAt  = gHWS_pc; //The +4 will help pairing at the flush
    
    
    if ((currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES) && (!IsBooting) 
        && ( (Instruction >>26 == 6) || (Instruction >>26 == 7)|| (Instruction >>26 == 22)|| (Instruction >>26 == 23)) )
    ;
    else
    if ((currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES) && (!IsBooting) 
        && (Instruction >>26 == 1) && ( (RT_FT == 0/*bltz*/)|| (RT_FT == 2/*bltzl*/)|| (RT_FT == 1/*bgez*/)|| (RT_FT == 3/*bgezl*/) ) ) 
   ;


    else

    Delay = 1;
    
    return 1;
}

int J_Type(unsigned __int32 Instruction)
{
//    InvalidateAnalysis(); return;

    if ((currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES) && (!IsBooting) 
        && (Instruction >>26 == 17) && (RS_BASE_FMT == 8) && (  ((Instruction & RT_FT) == 0)|| ((Instruction & RT_FT) == 1)/*bc1t*/)
        ||((Instruction & RT_FT) == 2)/*bc1fl*/||((Instruction & RT_FT) == 3)/*bc1tl*/)

    ;
    else if (Instruction >> 26 == 3) //jal
    ;
    else
    Delay = 1;
    
    //Note: jal writes to RA, and Interrupts() uses RA very late. 
    //So, do not set ConstMap[RA].FinalAddressUsedAt here.
    return 1;
}

int R_Type(unsigned __int32 Instruction)
{
//    InvalidateAnalysis(); return;
    
    ConstMap[RT_FT].FinalAddressUsedAt = gHWS_pc;
    ConstMap[RS_BASE_FMT].FinalAddressUsedAt  = gHWS_pc;
    ConstMap[RD_FS].FinalAddressUsedAt = gHWS_pc;

    return -1;
}

int ST_Type(unsigned __int32 Instruction)
{
//    InvalidateAnalysis(); return;
    
    ConstMap[RT_FT].FinalAddressUsedAt = gHWS_pc;
    ConstMap[RS_BASE_FMT].FinalAddressUsedAt  = gHWS_pc;

    return -1;
}


int B_Type(unsigned __int32 Instruction)
{
//    InvalidateAnalysis(); return;

    ConstMap[RT_FT].FinalAddressUsedAt = gHWS_pc; //The +4 will help pairing at the flush
    ConstMap[RS_BASE_FMT].FinalAddressUsedAt = gHWS_pc; //The +4 will help pairing at the flush
    
    if ((currentromoptions.Link_4KB_Blocks == USE4KBLINKBLOCK_YES) && (!IsBooting) 
        && ( (Instruction >>26 == 5) || (Instruction >>26 == 4)||(Instruction >>26 == 20)|| (Instruction >>26 == 21) ) )
    ;

    else

    Delay = 1;

    return -1;
}

//shifts: address in SA field
int SH_Type(unsigned __int32 Instruction)
{
  //InvalidateAnalysis(); return;

    ConstMap[RD_FS].FinalAddressUsedAt = gHWS_pc;
    ConstMap[RT_FT].FinalAddressUsedAt = gHWS_pc;
    return -1;
}

int RT_Type(unsigned __int32 Instruction)
{
//    InvalidateAnalysis(); return;

    ConstMap[RT_FT].FinalAddressUsedAt = gHWS_pc;
    return -1;
}

int CFC1_Type(unsigned __int32 Instruction)
{
    
//    InvalidateAnalysis(); return;
    
    //Conker freezes if this is used
    
    if ( (RD_FS==0) || (RD_FS==31) ) //make sure this check is consistent with the op.
    {
        ConstMap[RT_FT].FinalAddressUsedAt = gHWS_pc;
    }
    return -1;
}




int RS_Type(unsigned __int32 Instruction)
{
//    InvalidateAnalysis(); return;
    
    ConstMap[RS_BASE_FMT].FinalAddressUsedAt = gHWS_pc;
    return -1;
}

int RD_Type(unsigned __int32 Instruction)
{
//    InvalidateAnalysis(); return;
    
    ConstMap[RD_FS].FinalAddressUsedAt = gHWS_pc;
    return -1;
}


int Empty(unsigned __int32 Instruction)
{
    return -1;
}

int ToDo(unsigned __int32 Instruction)
{
    InvalidateAnalysis();
    return -1;
}

int Special(unsigned __int32 Instruction);
int CP0_(unsigned __int32 Instruction);
int CP1_(unsigned __int32 Instruction);
int Regimm_(unsigned __int32 Instruction);
int _BC0_(unsigned __int32  Instruction);
int _BC1_(unsigned __int32  Instruction);

//Conker problem at:
//0: addiu
//4: slti


//Use this line to debug a row below.
//ToDo,ToDo,ToDo,ToDo,ToDo,ToDo,ToDo,ToDo,


int (*Get_NormalInstruction_Format[64]) (unsigned __int32 Instruction) =
{
//  special, regimm,  j,      jal,    beq,    bne,     blez,    bgtz,
    Special, Regimm_, J_Type, J_Type, B_Type, B_Type,  BR_Type, BR_Type,

//  addi,    addiu,   slti,   sltiu,  andi,   ori,     xori,    lui,
    I_Type,  I_Type,  I_Type, I_Type, I_Type, I_Type,  I_Type,  RT_Type,

//  cop0,    cop1,    cop2,   resrvd, beql,   bnel,    blezl,   bgtzl,
    CP0_,    CP1_,    ToDo,   Empty,  B_Type, B_Type,  BR_Type, BR_Type,


//  daddi,   daddiu,  ldl,    ldr,    resrvd, resrvd,  resrvd,  resrvd, 
    I_Type,  I_Type,  I_Type, I_Type, Empty,  Empty,   Empty,   Empty,

//  lb,      lh,      lwl,    lw,     lbu,    lhu,     lwr,     lwu,
    I_Type,  I_Type,  I_Type, I_Type, I_Type, I_Type,  I_Type,  I_Type,

//  sb,      sh,      swl,    sw,     sdl,    sdr,     swr,     cache,
    I_Type,  I_Type,  I_Type, I_Type, I_Type, I_Type,  I_Type,  ToDo,

//  ll,      lwc1,    lwc2,   resrvd, lld,    ldc1,    ldc2,    ld,
    I_Type,  RS_Type, Empty,  Empty,  I_Type, RS_Type, Empty,   I_Type,

//  sc,      swc1,    swc2,   resrvd, scd,    sdc1,    sdc2,    sd
    I_Type,  RS_Type, Empty,  Empty,  I_Type, RS_Type, Empty,   I_Type
};

int (*Get_SpecialInstruction_Format[64]) (uint32 Instruction) =
{
//  shift,   resrvd,  shift,   shift,   shiftv,  resrvd, shiftv,  shiftv,
    SH_Type, Empty,   SH_Type, SH_Type, R_Type,  Empty,  R_Type,  R_Type,

//  jr,      jalr,    resrvd,  resrvd,  syscall, Break,  resrvd,  sync,
    BR_Type, BR_Type, Empty,   Empty,   ToDo,    ToDo,   Empty,   ToDo,

//  mfhi,    mthi,    mflo,    mtlo,    dsllv,   resrvd, dsrlv,   dsrav,    
    RD_Type, RS_Type, RD_Type, RS_Type, R_Type,  Empty,  R_Type,  R_Type,

//  mul,     mul,     Div,      divu,   dmult,   dmultu, ddiv,    ddivu,
    ST_Type, ST_Type, ST_Type, ST_Type, ST_Type, ST_Type,ST_Type, ST_Type,

//  add,     addu,    sub,     subu,    and,     or,     xor,     nor,
    R_Type,  R_Type,  R_Type,  R_Type,  R_Type,  R_Type, R_Type,  R_Type,

//  resrvd,  resrvd,  slt,     sltu,    dadd,    daddu,  dsub,    dsubu,
    Empty,   Empty,   R_Type,  R_Type,  R_Type,  R_Type, R_Type,  R_Type,

//  tge,     tgeu,    tlt,     tltu,    teq,     resrvd, tne,     resrvd,
    ToDo,    ToDo,    ToDo,    ToDo,    ToDo,    Empty,  ToDo,    Empty,

//  dsll,    resrvd,  dsrl,    dsra,    dsll32,  resrvd, dsrl32,  dsra32
    SH_Type, Empty,   SH_Type, SH_Type, SH_Type, Empty,  SH_Type, SH_Type
};

int (*Get_CP0_Instruction_Format[32]) (uint32 Instruction) =
{
//  mf,       dmf,     cf,     resrvd, mt,      dmt,     ct,     resrvd,
    RT_Type,  RT_Type, ToDo,   Empty,  RT_Type, RT_Type, ToDo,   Empty,

//  bc,       resrvd,  resrvd, resrvd, resrvd,  resrvd,  resrvd, resrvd,        
    _BC0_,    Empty,   Empty,  Empty,  Empty,    Empty,  Empty,  Empty,
    
//  cop0_tlb, resrvd,  resrvd, resrvd, resrvd,  resrvd,  resrvd, resrvd,
    Empty,     Empty,   Empty,  Empty,  Empty,    Empty,  Empty,  Empty,
    
//  resrvd,   resrvd,  resrvd, resrvd, resrvd,  resrvd,  resrvd, resrvd
    Empty,    Empty,   Empty,  Empty,  Empty,    Empty,  Empty,  Empty
};

int (*Get_RegimmInstruction_Format[32]) (uint32 Instruction) =
{
//  _bltz,   _bgez,   _bltzl,   _bgezl,  resrvd, resrvd, resrvd, resrvd,
    BR_Type, BR_Type, BR_Type,  BR_Type, Empty,  Empty,  Empty,  Empty,

//  _tgei,   _tgeiu,  _tlti,    _tltiu,   _teqi,  resrvd, _tnei,  resrvd,        
    ToDo,    ToDo,    ToDo,     ToDo,    ToDo,    Empty,  ToDo,   Empty,

//  _bltzal, _bgezal, _bltzall, _bgezall, resrvd, resrvd, resrvd, resrvd,
    BR_Type, BR_Type, BR_Type,  BR_Type,  Empty,  Empty,  Empty,  Empty,

//  resrvd,  resrvd,  resrvd,   resrvd,   resrvd, resrvd, resrvd, resrvd
    Empty,   Empty,   Empty,    Empty,    Empty,  Empty,  Empty,  Empty
};


//-----------------------------
// COP0

int (*Get_CP0_RT_Instruction_Format[32]) (uint32 Instruction) =
{
//  bcf,    bct,    bcfl,   bctl,   resrvd, resrvd, resrvd, resrvd
    ToDo,   ToDo,   ToDo,   ToDo,   Empty,  Empty,  Empty,  Empty,

//  resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd
    Empty,  Empty,  Empty,  Empty,  Empty,  Empty,  Empty,  Empty,

//  resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd
    Empty,  Empty,  Empty,  Empty,  Empty,  Empty,  Empty,  Empty,

//  resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd
    Empty,  Empty,  Empty,  Empty,  Empty,  Empty,  Empty,  Empty
};

//-----------------------------
// COP1


int (*Get_CP1_Instruction_Format[32]) (uint32 Instruction) =
{
//  mfc1,    dmfc1,   cfc1,   resrvd, mtc1,    dmtc1,   ctc1,    resrvd,
    RT_Type, RT_Type, CFC1_Type,   Empty,  RT_Type, RT_Type, RT_Type, Empty,

//  _ BC_,   resrvd,  resrvd, resrvd, resrvd,  resrvd,  resrvd,  resrvd,    
    _BC1_,   Empty,   Empty,  Empty,  Empty,   Empty,   Empty,   Empty,

//  _S_,     _D_,     resrvd, resrvd, _W_,     _L_,     resrvd,  resrvd,
    Empty,   Empty,   Empty,  Empty,  Empty,   Empty,   Empty,   Empty,

//  resrvd,  resrvd,  resrvd, resrvd, resrvd,  resrvd,  resrvd,  resrvd
    Empty,   Empty,   Empty,  Empty,  Empty,   Empty,   Empty,   Empty
};

int (*Get_CP1_RT_Instruction_Format[32]) (uint32 Instruction) =
{
//  bc1f,   bc1t,   bc1fl,  bc1tl,  resrvd, resrvd, resrvd, resrvd,    
    J_Type, J_Type, J_Type, J_Type, Empty,  Empty,  Empty,  Empty,

//  resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd
    Empty,  Empty,  Empty,  Empty,  Empty,  Empty,  Empty,  Empty,
    
//  resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd
    Empty,  Empty,  Empty,  Empty,  Empty,  Empty,  Empty,  Empty,
    
//  resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd, resrvd
    Empty,  Empty,  Empty,  Empty,  Empty,  Empty,  Empty,  Empty
};


int Special(unsigned __int32 Instruction)
{
    return Get_SpecialInstruction_Format[_FUNCTION_](Instruction);
}

int CP0_(unsigned __int32 Instruction)
{
    return Get_CP0_Instruction_Format[RS_BASE_FMT](Instruction);
}

int CP1_(unsigned __int32 Instruction)
{
    return Get_CP1_Instruction_Format[RS_BASE_FMT](Instruction);
}


int Regimm_(unsigned __int32 Instruction)
{
    return Get_RegimmInstruction_Format[RT_FT](Instruction);
}

int _BC0_(unsigned __int32 Instruction)
{
    return Get_CP0_RT_Instruction_Format[RT_FT](Instruction);
}

int _BC1_(unsigned __int32 Instruction)
{
    return Get_CP1_RT_Instruction_Format[RT_FT](Instruction);
}



extern uint32   FetchInstruction(void);
extern uint32   DynaFetchInstruction(uint32 pc);

extern void __cdecl error(char *Message, ...);
void AnalyzeBlock(void)
{
    uint32  Instruction;
    uint32  tempPC = gHWS_pc;

    EndOfBlock = 0;

    if(ITLB_Error) 
    {
//        MessageBox(0, "Ok Then", "", 0);
        return;
    }

    while(!EndOfBlock)
    {
        Instruction = DynaFetchInstruction(gHWS_pc);
        if (ITLB_Error)
        {
            gHWS_pc = tempPC;
            ConstMap[RA].FinalAddressUsedAt = 0xffffffff;
            InvalidateAnalysis();
            return;
        }

        Get_NormalInstruction_Format[_OPCODE_](Instruction);
        gHWS_pc += 4;
        if (Delay)
        {
            Delay = 0;
            Instruction = DynaFetchInstruction(gHWS_pc);
            if (ITLB_Error)
            {
                InvalidateAnalysis();
                gHWS_pc = tempPC;
                ConstMap[RA].FinalAddressUsedAt = 0xffffffff;
                return;
            }

            Get_NormalInstruction_Format[_OPCODE_](Instruction);
            EndOfBlock = 1;
        }
    }

    gHWS_pc = tempPC;
    ConstMap[RA].FinalAddressUsedAt = 0xffffffff;
}
