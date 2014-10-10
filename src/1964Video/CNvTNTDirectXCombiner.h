/*
Copyright (C) 2003 Rice1964

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef _NV_TNT_DX_COMBINER_H_
#define _NV_TNT_DX_COMBINER_H_

#include "DirectXCombiner.h"
#include "CNvTNTCombiner.h"

class CNvTNTDirectXCombiner : public CDirectXColorCombiner, CNvTNTCombiner
{
	friend class DirectXDeviceBuilder;
public:
	bool GenerateD3DCombineInfo(SetCombineInfo &cinfo);
	void CheckConstants(SetCombineInfo &cinfo);
	void InitCombinerCycle12(void);

protected:
	CNvTNTDirectXCombiner(CRender *pRender);
	void SetD3DArgument(uint8 val, uint32 &arg0, uint32 &arg1);
	//void ParseDecodedMuxForConstants(TNT2CombinerSaveType &res);
	uint32 m_dwLastMux0;
	uint32 m_dwLastMux1;
};

#endif


