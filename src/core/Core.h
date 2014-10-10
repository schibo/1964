#pragma once

#include "Registers.h"
#include "Memory.h"

namespace N64
{
	class CCore
	{
	public:
		CCore(const int numCores);
		~CCore(void);
		
		//VCR interface here.
		//

	private:
		CRegisters* Registers_;
		CMemory Memory_;

		static const __int32 SHIFTER1_READ = 2;
		static const __int32 SHIFTER1_WRITE = 0;

		unsigned __int32 programCounter_;
		unsigned __int32 *lookupPtr_;
		unsigned __int32 writeMemRT_;
		__int32 countDownCounter_;
		unsigned __int32 isPCInRDRAM_;
		unsigned __int32 code_;
		unsigned __int32 *(*memory_read_functions[0x10000>>SHIFTER1_READ])();
		unsigned __int32 *(*memory_write_fun_eax_only[0x100000>>SHIFTER1_WRITE])();
		unsigned __int8 *sDYN_PC_LOOKUP[0x10000];
	};
}