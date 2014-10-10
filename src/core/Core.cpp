#include "Core.h"

namespace N64
{
	CCore::CCore(const int numCores)
	{
		Registers_ = new CRegisters[numCores];
	}

	CCore::~CCore(void)
	{
		if (Registers_)
		{
			delete[] Registers_;
			Registers_ = 0;
		}
	}
}
