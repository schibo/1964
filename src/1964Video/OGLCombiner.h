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

#ifndef _OGL_COMBINER_H_
#define _OGL_COMBINER_H_

#include "Combiner.h"

class OGLRender;

class COGLColorCombiner : public CColorCombiner
{
public:
	bool Initialize(void);
	void InitCombinerBlenderForSimpleTextureDraw(uint32 tile=0);
protected:
	friend class OGLDeviceBuilder;

	void DisableCombiner(void);
	void InitCombinerCycleCopy(void);
	void InitCombinerCycleFill(void);
	void InitCombinerCycle12(void);

	COGLColorCombiner(CRender *pRender);
	~COGLColorCombiner();
	OGLRender *m_pOGLRender;
	
	bool	m_bSupportAdd;
	bool	m_bSupportSubtract;
};

class COGLFragmentShaderCombiner : public COGLColorCombiner
{
public:
	bool Initialize(void);
	void InitCombinerBlenderForSimpleTextureDraw(uint32 tile=0);
protected:
	friend class OGLDeviceBuilder;

	void DisableCombiner(void);
	void InitCombinerCycleCopy(void);
	void InitCombinerCycleFill(void);
	void InitCombinerCycle12(void);

	COGLFragmentShaderCombiner(CRender *pRender);
	~COGLFragmentShaderCombiner();

	bool m_bShaderIsSupported;
};

class COGLBlender : public CBlender
{
public:
	void NormalAlphaBlender(void);
	void DisableAlphaBlender(void);
	void BlendFunc(uint32 srcFunc, uint32 desFunc);
	void Enable();
	void Disable();

protected:
	friend class OGLDeviceBuilder;
	COGLBlender(CRender *pRender) : CBlender(pRender), m_pOGLRender((OGLRender*)pRender) {};
	~COGLBlender() {};

	OGLRender *m_pOGLRender;
};


#endif



