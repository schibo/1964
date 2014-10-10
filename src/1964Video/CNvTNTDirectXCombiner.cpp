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


#include "stdafx.h"

CNvTNTDirectXCombiner::CNvTNTDirectXCombiner(CRender *pRender)
	:CDirectXColorCombiner(pRender)
{
	m_ppDecodedMux = &m_pDecodedMux;
	m_dwLastMux0 = m_dwLastMux1 = 0;
}


uint32 MuxToTNTD3DMap[][2] = {
	D3DTA_TFACTOR,	D3DTA_TFACTOR,						//MUX_0 = 0,
	D3DTA_TFACTOR|D3DTA_COMPLEMENT,	D3DTA_TFACTOR,		//MUX_1,
	D3DTA_CURRENT,	D3DTA_DIFFUSE,						//MUX_COMBINED,
	D3DTA_TEXTURE,	D3DTA_DIFFUSE,						//MUX_TEXEL0,
	D3DTA_TEXTURE,	D3DTA_TEXTURE,						//MUX_TEXEL1,
	D3DTA_TFACTOR,	D3DTA_DIFFUSE,						//MUX_PRIM,
	D3DTA_DIFFUSE,	D3DTA_DIFFUSE,						//MUX_SHADE,
	D3DTA_TFACTOR,	D3DTA_DIFFUSE,						//MUX_ENV,
	D3DTA_CURRENT|D3DTA_ALPHAREPLICATE,	D3DTA_DIFFUSE,	//MUX_COMBALPHA,
	D3DTA_TEXTURE|D3DTA_ALPHAREPLICATE,	D3DTA_DIFFUSE,	//MUX_T0_ALPHA,
	D3DTA_TEXTURE|D3DTA_ALPHAREPLICATE,	D3DTA_TEXTURE,	//MUX_T1_ALPHA,
	D3DTA_TFACTOR|D3DTA_ALPHAREPLICATE,	D3DTA_DIFFUSE,	//MUX_PRIM_ALPHA,
	D3DTA_DIFFUSE|D3DTA_ALPHAREPLICATE,	D3DTA_DIFFUSE,	//MUX_SHADE_ALPHA,
	D3DTA_TFACTOR|D3DTA_ALPHAREPLICATE,	D3DTA_DIFFUSE,	//MUX_ENV_ALPHA,
	D3DTA_TFACTOR,	D3DTA_DIFFUSE,						//MUX_LODFRAC,
	D3DTA_TFACTOR,	D3DTA_DIFFUSE,						//MUX_PRIMLODFRAC,
	D3DTA_TFACTOR|D3DTA_COMPLEMENT,	D3DTA_TFACTOR,		//MUX_K5,
	D3DTA_TFACTOR,	D3DTA_TFACTOR,						//MUX_UNK,
};

void CNvTNTDirectXCombiner::SetD3DArgument(uint8 val, uint32 &arg0, uint32 &arg1)
{
	int index = val&MUX_MASK;
	if( index > MUX_PRIMLODFRAC )
	{
		arg0 = D3DTA_DIFFUSE;
		arg1 = D3DTA_DIFFUSE;
		return;
	}
	else
	{
		arg0 = MuxToTNTD3DMap[index][0];
		arg1 = MuxToTNTD3DMap[index][1];
		if( val&MUX_COMPLEMENT )
		{
			arg0 |= D3DTA_COMPLEMENT;
		}
		if( val&MUX_ALPHAREPLICATE )
		{
			arg0 |= D3DTA_ALPHAREPLICATE;
		}
	}
}

bool CNvTNTDirectXCombiner::GenerateD3DCombineInfo(SetCombineInfo &cinfo)
{
	if( m_pDecodedMux->m_dwMux0 != m_dwLastMux0 || m_pDecodedMux->m_dwMux1 != m_dwLastMux1 )
	{
		m_lastIndex = CNvTNTCombiner::FindCompiledMux();
		if( m_lastIndex < 0 )		// Can not found
		{
			m_lastIndex = m_lastIndex = CNvTNTCombiner::ParseDecodedMux();
		}
		m_dwLastMux0 = m_pDecodedMux->m_dwMux0;
		m_dwLastMux1 = m_pDecodedMux->m_dwMux1;
	}

	TNT2CombinerSaveType &res = m_vCompiledTNTSettings[m_lastIndex];

	int i;
	for( i=0; i<8; i++ )
	{
		cinfo.stages[i].dwTexture = 0;
		cinfo.stages[i].alphaOp.Arg0 = D3DTA_IGNORE;
		cinfo.stages[i].colorOp.Arg0 = D3DTA_IGNORE;
	}

	for( i=0; i<2; i++ )
	{
		// i is cycle
		for( int j=0; j<4; j++)
		{
			// each cycle has four flags
			CombineStage &op = cinfo.stages[i*4+j];
			SetD3DArgument(res.units[i].args[0][j], op.colorOp.Arg1, op.colorOp.Arg2);
			SetD3DArgument(res.units[i].args[1][j], op.alphaOp.Arg1, op.alphaOp.Arg2);
		}

		cinfo.stages[i*4+0].colorOp.op = D3DTOP_MODULATE;
		cinfo.stages[i*4+1].colorOp.op = D3DTOP_ADD;
		if( res.units[i].rgbOp == 0x0104 )	//Add
			cinfo.stages[i*4+1].colorOp.op = D3DTOP_ADD;
		else	// Subtract
			cinfo.stages[i*4+1].colorOp.op = D3DTOP_SUBTRACT;
		cinfo.stages[i*4+2].colorOp.op = D3DTOP_MODULATE;
		cinfo.stages[i*4+3].colorOp.op = D3DTOP_SELECTARG1;

		cinfo.stages[i*4+0].alphaOp.op = D3DTOP_MODULATE;
		if( res.units[i].alphaOp == 0x0104 )	//Add
			cinfo.stages[i*4+1].alphaOp.op = D3DTOP_ADD;
		else	// Subtract
		cinfo.stages[i*4+1].alphaOp.op = D3DTOP_SUBTRACT;

		cinfo.stages[i*4+2].alphaOp.op = D3DTOP_MODULATE;
		cinfo.stages[i*4+3].alphaOp.op = D3DTOP_SELECTARG1;

		cinfo.stages[i].dwTexture = i;	// Only for stage 0 and 1
	}

	cinfo.nStages = 8;
	cinfo.blendingFunc = ENABLE_BOTH;
	cinfo.m_dwShadeAlphaChannelFlag = m_pDecodedMux->m_dwShadeAlphaChannelFlag;
	cinfo.m_dwShadeColorChannelFlag = m_pDecodedMux->m_dwShadeColorChannelFlag;
	cinfo.specularPostOp = MUX_0;

	CheckConstants(cinfo);

	for( i=0;i<8;i++)
	{
		cinfo.stages[i].bTextureUsed = false;
	}
	
	cinfo.stages[0].bTextureUsed = true;
	cinfo.stages[1].bTextureUsed = true;

	return true;
}

void CNvTNTDirectXCombiner::CheckConstants(SetCombineInfo &cinfo)
{
	TNT2CombinerSaveType &res = m_vCompiledTNTSettings[m_lastIndex];

	if( m_pDecodedMux->isUsed(MUX_PRIM) && m_pDecodedMux->isUsed(MUX_ENV) )
	{
		// Both are used
		// For Banjo Tooie
		if( m_pDecodedMux->splitType[0] == CM_FMT_TYPE_A_MOD_C)
		{
			if(	DecodedMux::IsConstFactor(m_pDecodedMux->aRGB0) )
			{
				cinfo.TFactor = (m_pDecodedMux->aRGB0)&MUX_MASK;
				return;
			}
			else if( DecodedMux::IsConstFactor(m_pDecodedMux->cRGB0) )
			{
				cinfo.TFactor = (m_pDecodedMux->cRGB0)&MUX_MASK;
				return;
			}
		}

		if( m_pDecodedMux->splitType[0] == CM_FMT_TYPE_A_LERP_B_C)
		{
			if( DecodedMux::IsConstFactor(m_pDecodedMux->cRGB0) )
			{
				cinfo.TFactor = (m_pDecodedMux->cRGB0)&MUX_MASK;
				return;
			}
		}

	}

	if( res.unit1.constant != MUX_0 )
		cinfo.TFactor = res.unit1.constant;
	else
		cinfo.TFactor = res.unit2.constant;
}

void CNvTNTDirectXCombiner::InitCombinerCycle12(void)
{
	bool combinerIsChanged = FindAndFillCombineMode(m_pD3DRender->m_curCombineInfo);

	if( m_bTex0Enabled )	m_pD3DRender->SetTexelRepeatFlags(gRSP.curTile);
	if( m_bTex1Enabled )	m_pD3DRender->SetTexelRepeatFlags((gRSP.curTile+1)&7);

	if( combinerIsChanged || m_bCycleChanged || gRDP.texturesAreReloaded )
	{
		if( m_bTex0Enabled )
		{
			gD3DDevWrapper.SetTexture( 0, LPDIRECT3DTEXTURE8(g_textures[gRSP.curTile].m_lpsTexturePtr) );
		}
		else if(m_bTex1Enabled)
		{
			gD3DDevWrapper.SetTexture( 0, LPDIRECT3DTEXTURE8(g_textures[(gRSP.curTile+1)&7].m_lpsTexturePtr) );
		}

		if( m_bTex1Enabled )
		{
			gD3DDevWrapper.SetTexture( 1, LPDIRECT3DTEXTURE8(g_textures[(gRSP.curTile+1)&7].m_lpsTexturePtr) );
		}
		else
		{
			gD3DDevWrapper.SetTexture( 1, LPDIRECT3DTEXTURE8(g_textures[gRSP.curTile].m_lpsTexturePtr) );
		}
		gRDP.texturesAreReloaded = false;
	}

	if( m_pD3DRender->m_curCombineInfo.TFactor != MUX_0 )
	{
		m_pD3DRender->SetBlendFactor(m_pD3DRender->m_curCombineInfo.TFactor);
	}

	if( combinerIsChanged || m_bCycleChanged )
	{
		for (int i = 0; i < m_pD3DRender->m_curCombineInfo.nStages; i++)
		{
			if( i>1 ) gD3DDevWrapper.SetTexture( i, 0 );
			gD3DDevWrapper.SetTextureStageState( i, D3DTSS_TEXCOORDINDEX, i );

			gD3DDevWrapper.SetTextureStageState( i, D3DTSS_COLOROP, m_pD3DRender->m_curCombineInfo.stages[i].colorOp.op );
			gD3DDevWrapper.SetTextureStageState( i, D3DTSS_COLORARG1, m_pD3DRender->m_curCombineInfo.stages[i].colorOp.Arg1 );
			gD3DDevWrapper.SetTextureStageState( i, D3DTSS_COLORARG2, m_pD3DRender->m_curCombineInfo.stages[i].colorOp.Arg2 );

			gD3DDevWrapper.SetTextureStageState( i, D3DTSS_ALPHAOP, m_pD3DRender->m_curCombineInfo.stages[i].alphaOp.op );
			gD3DDevWrapper.SetTextureStageState( i, D3DTSS_ALPHAARG1, m_pD3DRender->m_curCombineInfo.stages[i].alphaOp.Arg1 );
			gD3DDevWrapper.SetTextureStageState( i, D3DTSS_ALPHAARG2, m_pD3DRender->m_curCombineInfo.stages[i].alphaOp.Arg2 );
		}
	}

}

