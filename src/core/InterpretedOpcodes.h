#pragma once
#include "opcodemap.h"

namespace N64
{
	class CInterpretedOpcodes :
		public COpcodeMap
	{
	public:
		CInterpretedOpcodes(void);
		~CInterpretedOpcodes(void);

		void executeFunction(unsigned int opcode, CRegisters&, unsigned int instruction);


	private:

		unsigned __int32 opcode_;

		void instr(CRegisters& r, unsigned int instruction);
		void REGIMM_instr(CRegisters& r, unsigned int instruction);
		void j(CRegisters& r, unsigned int instruction);
		void jal(CRegisters& r, unsigned int instruction);
		void beq(CRegisters& r, unsigned int instruction);
		void bne(CRegisters& r, unsigned int instruction);
		void blez(CRegisters& r, unsigned int instruction);
		void bgtz(CRegisters& r, unsigned int instruction);
		void addi(CRegisters& r, unsigned int instruction);
		void addiu(CRegisters& r, unsigned int instruction);
		void slti(CRegisters& r, unsigned int instruction);
		void sltiu(CRegisters& r, unsigned int instruction);
		void andi(CRegisters& r, unsigned int instruction);
		void ori(CRegisters& r, unsigned int instruction);
		void xori(CRegisters& r, unsigned int instruction);
		void lui(CRegisters& r, unsigned int instruction);
		void COP0_instr(CRegisters& r, unsigned int instruction);
		void COP1_instr(CRegisters& r, unsigned int instruction);
		void UNUSED(CRegisters& r, unsigned int instruction);
		//unused
		void beql(CRegisters& r, unsigned int instruction);
		void bnel(CRegisters& r, unsigned int instruction);
		void blezl(CRegisters& r, unsigned int instruction);
		void bgtzl(CRegisters& r, unsigned int instruction);
		void daddi(CRegisters& r, unsigned int instruction);
		void daddiu(CRegisters& r, unsigned int instruction);
		void ldl(CRegisters& r, unsigned int instruction);
		void ldr(CRegisters& r, unsigned int instruction);
		//unused
		//unused
		//unused
		//unused
		void lb(CRegisters& r, unsigned int instruction);
		void lh(CRegisters& r, unsigned int instruction);
		void lwl(CRegisters& r, unsigned int instruction);
		void lw(CRegisters& r, unsigned int instruction);
		void lbu(CRegisters& r, unsigned int instruction);
		void lhu(CRegisters& r, unsigned int instruction);
		void lwr(CRegisters& r, unsigned int instruction);
		void lwu(CRegisters& r, unsigned int instruction);
		void sb(CRegisters& r, unsigned int instruction);
		void sh(CRegisters& r, unsigned int instruction);
		void swl(CRegisters& r, unsigned int instruction);
		void sw(CRegisters& r, unsigned int instruction);
		void sdl(CRegisters& r, unsigned int instruction);
		void sdr(CRegisters& r, unsigned int instruction);
		void swr(CRegisters& r, unsigned int instruction);
		void cache(CRegisters& r, unsigned int instruction);
		void ll(CRegisters& r, unsigned int instruction);
		void lwc1(CRegisters& r, unsigned int instruction);
		//unused
		//unused
		void lld(CRegisters& r, unsigned int instruction);
		void ldc1(CRegisters& r, unsigned int instruction);
		//unused
		void ld(CRegisters& r, unsigned int instruction);
		void sc(CRegisters& r, unsigned int instruction);
		void swc1(CRegisters& r, unsigned int instruction);
		//unused
		//unused
		void scd(CRegisters& r, unsigned int instruction);
		void sdc1(CRegisters& r, unsigned int instruction);
		//unused
		void sd(CRegisters& r, unsigned int instruction);
	};
}