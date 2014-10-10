#pragma once

#include "Registers.h"

namespace N64
{
	//OpcodeMap abstract base class.
	class COpcodeMap
	{

	protected:

		typedef void (COpcodeMap::*lpfnOpcode)(CRegisters& r, unsigned int instruction);

		virtual void instr(CRegisters& r, unsigned int instruction) = 0;
		virtual void REGIMM_instr(CRegisters& r, unsigned int instruction) = 0;
		virtual void j(CRegisters& r, unsigned int instruction) = 0;
		virtual void jal(CRegisters& r, unsigned int instruction) = 0;
		virtual void beq(CRegisters& r, unsigned int instruction) = 0;
		virtual void bne(CRegisters& r, unsigned int instruction) = 0;
		virtual void blez(CRegisters& r, unsigned int instruction) = 0;
		virtual void bgtz(CRegisters& r, unsigned int instruction) = 0;
		virtual void addi(CRegisters& r, unsigned int instruction) = 0;
		virtual void addiu(CRegisters& r, unsigned int instruction) = 0;
		virtual void slti(CRegisters& r, unsigned int instruction) = 0;
		virtual void sltiu(CRegisters& r, unsigned int instruction) = 0;
		virtual void andi(CRegisters& r, unsigned int instruction) = 0;
		virtual void ori(CRegisters& r, unsigned int instruction) = 0;
		virtual void xori(CRegisters& r, unsigned int instruction) = 0;
		virtual void lui(CRegisters& r, unsigned int instruction) = 0;
		virtual void COP0_instr(CRegisters& r, unsigned int instruction) = 0;
		virtual void COP1_instr(CRegisters& r, unsigned int instruction) = 0;
		virtual void UNUSED(CRegisters& r, unsigned int instruction) = 0;
		//unused
		virtual void beql(CRegisters& r, unsigned int instruction) = 0;
		virtual void bnel(CRegisters& r, unsigned int instruction) = 0;
		virtual void blezl(CRegisters& r, unsigned int instruction) = 0;
		virtual void bgtzl(CRegisters& r, unsigned int instruction) = 0;
		virtual void daddi(CRegisters& r, unsigned int instruction) = 0;
		virtual void daddiu(CRegisters& r, unsigned int instruction) = 0;
		virtual void ldl(CRegisters& r, unsigned int instruction) = 0;
		virtual void ldr(CRegisters& r, unsigned int instruction) = 0;
		//unused
		//unused
		//unused
		//unused
		virtual void lb(CRegisters& r, unsigned int instruction) = 0;
		virtual void lh(CRegisters& r, unsigned int instruction) = 0;
		virtual void lwl(CRegisters& r, unsigned int instruction) = 0;
		virtual void lw(CRegisters& r, unsigned int instruction) = 0;
		virtual void lbu(CRegisters& r, unsigned int instruction) = 0;
		virtual void lhu(CRegisters& r, unsigned int instruction) = 0;
		virtual void lwr(CRegisters& r, unsigned int instruction) = 0;
		virtual void lwu(CRegisters& r, unsigned int instruction) = 0;
		virtual void sb(CRegisters& r, unsigned int instruction) = 0;
		virtual void sh(CRegisters& r, unsigned int instruction) = 0;
		virtual void swl(CRegisters& r, unsigned int instruction) = 0;
		virtual void sw(CRegisters& r, unsigned int instruction) = 0;
		virtual void sdl(CRegisters& r, unsigned int instruction) = 0;
		virtual void sdr(CRegisters& r, unsigned int instruction) = 0;
		virtual void swr(CRegisters& r, unsigned int instruction) = 0;
		virtual void cache(CRegisters& r, unsigned int instruction) = 0;
		virtual void ll(CRegisters& r, unsigned int instruction) = 0;
		virtual void lwc1(CRegisters& r, unsigned int instruction) = 0;
		//unused
		//unused
		virtual void lld(CRegisters& r, unsigned int instruction) = 0;
		virtual void ldc1(CRegisters& r, unsigned int instruction) = 0;
		//unused
		virtual void ld(CRegisters& r, unsigned int instruction) = 0;
		virtual void sc(CRegisters& r, unsigned int instruction) = 0;
		virtual void swc1(CRegisters& r, unsigned int instruction) = 0;
		//unused
		//unused
		virtual void scd(CRegisters& r, unsigned int instruction) = 0;
		virtual void sdc1(CRegisters& r, unsigned int instruction) = 0;
		//unused
		virtual void sd(CRegisters& r, unsigned int instruction) = 0;

		lpfnOpcode OpcodeMap_[64];

	public:

		COpcodeMap(void)
		{
			OpcodeMap_[0] = &COpcodeMap::instr;
			OpcodeMap_[1] = &COpcodeMap::REGIMM_instr;
			OpcodeMap_[2] = &COpcodeMap::j;
			OpcodeMap_[3] = &COpcodeMap::jal;
			OpcodeMap_[4] = &COpcodeMap::beq;
			OpcodeMap_[5] = &COpcodeMap::bne;
			OpcodeMap_[6] = &COpcodeMap::blez;
			OpcodeMap_[7] = &COpcodeMap::bgtz;
			OpcodeMap_[8] = &COpcodeMap::addi;
			OpcodeMap_[9] = &COpcodeMap::addiu;
			OpcodeMap_[10] = &COpcodeMap::slti;
			OpcodeMap_[11] = &COpcodeMap::sltiu;
			OpcodeMap_[12] = &COpcodeMap::andi;
			OpcodeMap_[13] = &COpcodeMap::ori;
			OpcodeMap_[14] = &COpcodeMap::xori;
			OpcodeMap_[15] = &COpcodeMap::lui;
			OpcodeMap_[16] = &COpcodeMap::COP0_instr;
			OpcodeMap_[17] = &COpcodeMap::COP1_instr;
			OpcodeMap_[18] = &COpcodeMap::UNUSED;
			OpcodeMap_[19] = &COpcodeMap::UNUSED;
			OpcodeMap_[20] = &COpcodeMap::beql;
			OpcodeMap_[21] = &COpcodeMap::bnel;
			OpcodeMap_[22] = &COpcodeMap::blezl;
			OpcodeMap_[23] = &COpcodeMap::bgtzl;
			OpcodeMap_[24] = &COpcodeMap::daddi;
			OpcodeMap_[25] = &COpcodeMap::daddiu;
			OpcodeMap_[26] = &COpcodeMap::ldl;
			OpcodeMap_[27] = &COpcodeMap::ldr;
			OpcodeMap_[28] = &COpcodeMap::UNUSED;
			OpcodeMap_[29] = &COpcodeMap::UNUSED;
			OpcodeMap_[30] = &COpcodeMap::UNUSED;
			OpcodeMap_[31] = &COpcodeMap::UNUSED;
			OpcodeMap_[32] = &COpcodeMap::lb;
			OpcodeMap_[33] = &COpcodeMap::lh;
			OpcodeMap_[34] = &COpcodeMap::lwl;
			OpcodeMap_[35] = &COpcodeMap::lw;
			OpcodeMap_[36] = &COpcodeMap::lbu;
			OpcodeMap_[37] = &COpcodeMap::lhu;
			OpcodeMap_[38] = &COpcodeMap::lwr;
			OpcodeMap_[39] = &COpcodeMap::lwu;
			OpcodeMap_[40] = &COpcodeMap::sb;
			OpcodeMap_[41] = &COpcodeMap::sh;
			OpcodeMap_[42] = &COpcodeMap::swl;
			OpcodeMap_[43] = &COpcodeMap::sw;
			OpcodeMap_[44] = &COpcodeMap::sdl;
			OpcodeMap_[45] = &COpcodeMap::sdr;
			OpcodeMap_[46] = &COpcodeMap::swr;
			OpcodeMap_[47] = &COpcodeMap::cache;
			OpcodeMap_[48] = &COpcodeMap::ll;
			OpcodeMap_[49] = &COpcodeMap::lwc1;
			OpcodeMap_[50] = &COpcodeMap::UNUSED;
			OpcodeMap_[51] = &COpcodeMap::UNUSED;
			OpcodeMap_[52] = &COpcodeMap::lld;
			OpcodeMap_[53] = &COpcodeMap::ldc1;
			OpcodeMap_[54] = &COpcodeMap::UNUSED;
			OpcodeMap_[55] = &COpcodeMap::ld;
			OpcodeMap_[56] = &COpcodeMap::sc;
			OpcodeMap_[57] = &COpcodeMap::swc1;
			OpcodeMap_[58] = &COpcodeMap::UNUSED;
			OpcodeMap_[59] = &COpcodeMap::UNUSED;
			OpcodeMap_[60] = &COpcodeMap::scd;
			OpcodeMap_[61] = &COpcodeMap::sdc1;
			OpcodeMap_[62] = &COpcodeMap::UNUSED;
			OpcodeMap_[63] = &COpcodeMap::sd;
		}

		virtual ~COpcodeMap(void)
		{
		}
	};
}