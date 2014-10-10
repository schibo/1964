#include ".\interpretedopcodes.h"
#include "state.h"
#include "r4300i.h"

extern void (*CPU_instruction[64]) (unsigned int);

namespace N64
{
	CInterpretedOpcodes::CInterpretedOpcodes(void)
	{
	}

	CInterpretedOpcodes::~CInterpretedOpcodes(void)
	{
	}

	void CInterpretedOpcodes::executeFunction(unsigned int opcode, CRegisters& r, unsigned int instruction)
	{
		opcode_ = opcode;
		(this->*OpcodeMap_[opcode_])(r, instruction);
	}

	void CInterpretedOpcodes::instr(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::REGIMM_instr(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::j(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::jal(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::beq(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::bne(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::blez(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::bgtz(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::addi(CRegisters& r, unsigned int instruction)
	{
			r4300i_addi(instruction);
	}

	void CInterpretedOpcodes::addiu(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::slti(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::sltiu(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::andi(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::ori(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::xori(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::lui(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::COP0_instr(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::COP1_instr(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::UNUSED(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::beql(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::bnel(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::blezl(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::bgtzl(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::daddi(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::daddiu(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::ldl(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::ldr(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::lb(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::lh(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::lwl(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::lw(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::lbu(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::lhu(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::lwr(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::lwu(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::sb(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::sh(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::swl(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::sw(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::sdl(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::sdr(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::swr(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::cache(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::ll(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::lwc1(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::lld(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::ldc1(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::ld(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::sc(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::swc1(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::scd(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::sdc1(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}

	void CInterpretedOpcodes::sd(CRegisters& r, unsigned int instruction)
	{
		CPU_instruction[opcode_](instruction);
	}
}