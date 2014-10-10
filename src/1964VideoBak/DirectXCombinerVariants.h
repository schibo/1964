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

#ifndef _DIRECTX_COMBINER_VARIANTS_H_
#define _DIRECTX_COMBINER_VARIANTS_H_

#include "DirectXCombiner.h"
#include "OGLCombinerTNT2.h"
#include "CNvTNTDirectXCombiner.h"

class CDirectXColorCombinerLowEnd : public CDirectXColorCombiner
{
protected:
	friend class DirectXDeviceBuilder;
	CDirectXColorCombinerLowEnd(CRender *pRender);
	bool GenerateD3DCombineInfo(SetCombineInfo & sci);
};

typedef struct {
	uint64  mux64;
	uint32 dwShaderID;
	ID3DXBuffer* pVS;
	char *pShaderText;
} PixelShaderEntry;

class CDirectXPixelShaderCombiner : public CDirectXColorCombiner
{
public:
	void CleanUp(void);

protected:
	friend class DirectXDeviceBuilder;
	CDirectXPixelShaderCombiner(CRender *pRender);
	~CDirectXPixelShaderCombiner();

	void InitCombinerCycle12(void);
	int GeneratePixelShaderFromMux(void);
	int FindCompiledShader(void);

	std::vector<PixelShaderEntry> m_pixelShaderList;

#define PIXELSHADERTEXTBUFSIZE	16000
	char m_textBuf[PIXELSHADERTEXTBUFSIZE];
};

class CDirectXSemiPixelShaderCombiner : public CDirectXPixelShaderCombiner
{
protected:
	friend class DirectXDeviceBuilder;
	CDirectXSemiPixelShaderCombiner(CRender *pRender);

	void InitCombinerCycle12(void);
};

#endif
