/*
Copyright (C) 2002 Rice1964

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

void RSP_GBI1_SpNoop(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_SpNoop);

	if( (gfx+1)->words.cmd == 0x00 && gRSP.ucode >= 17 )
	{
		RDP_GFX_PopDL();
	}
}

void RSP_GBI0_Mtx(Gfx *gfx)
{	
	SP_Timing(RSP_GBI0_Mtx);

	uint32 addr = RSPSegmentAddr((gfx->gbi0matrix.addr));

	if (addr + 64 > g_dwRamSize)
	{
		return;
	}

	LoadMatrix(addr);
	
	if (gfx->gbi0matrix.projection)
	{
		CRender::g_pRender->SetProjection(matToLoad, gfx->gbi0matrix.push, gfx->gbi0matrix.load);
	}
	else
	{
		CRender::g_pRender->SetWorldView(matToLoad, gfx->gbi0matrix.push, gfx->gbi0matrix.load);
	}
}




void RSP_GBI1_Reserved(Gfx *gfx)
{		
	SP_Timing(RSP_GBI1_Reserved);
}



void RSP_GBI1_MoveMem(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_MoveMem);

	uint32 type    = ((gfx->words.w0)>>16)&0xFF;
	uint32 dwLength  = ((gfx->words.w0))&0xFFFF;
	uint32 addr = RSPSegmentAddr((gfx->words.w1));

	switch (type) 
	{
		case RSP_GBI1_MV_MEM_VIEWPORT:
			{
				RSP_MoveMemViewport(addr);
			}
			break;
		case RSP_GBI1_MV_MEM_LOOKATY:
			break;
		case RSP_GBI1_MV_MEM_LOOKATX:
			break;
		case RSP_GBI1_MV_MEM_L0:
		case RSP_GBI1_MV_MEM_L1:
		case RSP_GBI1_MV_MEM_L2:
		case RSP_GBI1_MV_MEM_L3:
		case RSP_GBI1_MV_MEM_L4:
		case RSP_GBI1_MV_MEM_L5:
		case RSP_GBI1_MV_MEM_L6:
		case RSP_GBI1_MV_MEM_L7:
			{
				uint32 dwLight = (type-RSP_GBI1_MV_MEM_L0)/2;
				RSP_MoveMemLight(dwLight, addr);
			}
			break;
		case RSP_GBI1_MV_MEM_TXTATT:
			break;
		case RSP_GBI1_MV_MEM_MATRIX_1:
			RSP_GFX_Force_Matrix(addr);
			break;
		case RSP_GBI1_MV_MEM_MATRIX_2:
			break;
		case RSP_GBI1_MV_MEM_MATRIX_3:
			break;
		case RSP_GBI1_MV_MEM_MATRIX_4:
			break;
		default:
			break;
	}
}


void RSP_GBI0_Vtx(Gfx *gfx)
{
	SP_Timing(RSP_GBI0_Vtx);

	int n = gfx->gbi0vtx.n + 1;
	int v0 = gfx->gbi0vtx.v0;
	uint32 addr = RSPSegmentAddr((gfx->gbi0vtx.addr));

	if ((v0 + n) > 80)
	{
		n = 32 - v0;
	}

	// Check that address is valid...
	if ((addr + n*16) > g_dwRamSize)
	{
	}
	else
	{
		ProcessVertexData(addr, v0, n);
		status.dwNumVertices += n;
		DisplayVertexInfo(addr, v0, n);
	}
}


void RSP_GBI0_DL(Gfx *gfx)
{	
	SP_Timing(RSP_GBI0_DL);

	uint32 addr = RSPSegmentAddr((gfx->gbi0dlist.addr)) & (g_dwRamSize-1);

	if( addr > g_dwRamSize )
	{
		addr &= (g_dwRamSize-1);
	}

	if( gfx->gbi0dlist.param == RSP_DLIST_PUSH )
		gDlistStackPointer++;

	gDlistStack[gDlistStackPointer].pc = addr;
	gDlistStack[gDlistStackPointer].countdown = MAX_DL_COUNT;

}


void RSP_GBI1_RDPHalf_Cont(Gfx *gfx)	
{
	SP_Timing(RSP_GBI1_RDPHalf_Cont);

}
void RSP_GBI1_RDPHalf_2(Gfx *gfx)		
{ 
	SP_Timing(RSP_GBI1_RDPHalf_2);

}

void RSP_GBI1_RDPHalf_1(Gfx *gfx)		
{
	SP_Timing(RSP_GBI1_RDPHalf_1);

}

void RSP_GBI1_Line3D(Gfx *gfx)
{
	status.primitiveType = PRIM_LINE3D;

	uint32 dwPC = gDlistStack[gDlistStackPointer].pc;

	BOOL bTrisAdded = FALSE;

	if( gfx->ln3dtri2.v3 == 0 )
	{
		// Flying Dragon
		uint32 dwV0		= gfx->ln3dtri2.v0/gRSP.vertexMult;
		uint32 dwV1		= gfx->ln3dtri2.v1/gRSP.vertexMult;
		uint32 dwWidth  = gfx->ln3dtri2.v2;
		uint32 dwFlag	= gfx->ln3dtri2.v3/gRSP.vertexMult;	
		
		CRender::g_pRender->SetCombinerAndBlender();

		status.dwNumTrisRendered++;

		CRender::g_pRender->Line3D(dwV0, dwV1, dwWidth);
		SP_Timing(RSP_GBI1_Line3D);
		DP_Timing(RSP_GBI1_Line3D);
	}
	else
	{
		do {
			uint32 dwV3  = gfx->ln3dtri2.v3/gRSP.vertexMult;		
			uint32 dwV0  = gfx->ln3dtri2.v0/gRSP.vertexMult;
			uint32 dwV1  = gfx->ln3dtri2.v1/gRSP.vertexMult;
			uint32 dwV2  = gfx->ln3dtri2.v2/gRSP.vertexMult;

			// Do first tri
			if (IsTriangleVisible(dwV0, dwV1, dwV2))
			{
				if (!bTrisAdded && CRender::g_pRender->IsTextureEnabled())
				{
					PrepareTextures();
					InitVertexTextureConstants();
				}

				if( !bTrisAdded )
				{
					CRender::g_pRender->SetCombinerAndBlender();
				}

				bTrisAdded = true;
				PrepareTriangle(dwV0, dwV1, dwV2);
			}

			// Do second tri
			if (IsTriangleVisible(dwV2, dwV3, dwV0))
			{
				if (!bTrisAdded && CRender::g_pRender->IsTextureEnabled())
				{
					PrepareTextures();
					InitVertexTextureConstants();
				}

				if( !bTrisAdded )
				{
					CRender::g_pRender->SetCombinerAndBlender();
				}

				bTrisAdded = true;
				PrepareTriangle(dwV2, dwV3, dwV0);
			}

			gfx++;
			dwPC += 8;
		} while (gfx->words.cmd == (uint8)RSP_LINE3D);

		gDlistStack[gDlistStackPointer].pc = dwPC-8;

		if (bTrisAdded)	
		{
			CRender::g_pRender->DrawTriangles();
		}
	}
}


void RSP_GBI1_ClearGeometryMode(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_ClearGeometryMode);
	uint32 dwMask = ((gfx->words.w1));

	gRDP.geometryMode &= ~dwMask;
	RSP_GFX_InitGeometryMode();
}



void RSP_GBI1_SetGeometryMode(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_SetGeometryMode);
	uint32 dwMask = ((gfx->words.w1));

	gRDP.geometryMode |= dwMask;
	RSP_GFX_InitGeometryMode();
}




void RSP_GBI1_EndDL(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_EndDL);
	RDP_GFX_PopDL();
}


static const char * sc_szBlClr[4] = { "In", "Mem", "Bl", "Fog" };
static const char * sc_szBlA1[4] = { "AIn", "AFog", "AShade", "0" };
static const char * sc_szBlA2[4] = { "1-A", "AMem", "1", "?" };

void RSP_GBI1_SetOtherModeL(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_SetOtherModeL);

	uint32 dwShift = ((gfx->words.w0)>>8)&0xFF;
	uint32 dwLength= ((gfx->words.w0)   )&0xFF;
	uint32 dwData  = (gfx->words.w1);

	uint32 dwMask = ((1<<dwLength)-1)<<dwShift;

	uint32 modeL = gRDP.otherModeL;
	modeL = (modeL&(~dwMask)) | dwData;

	Gfx tempgfx;
	tempgfx.words.w0 = gRDP.otherModeH;
	tempgfx.words.w1 = modeL;
	DLParser_RDPSetOtherMode(&tempgfx);
}


void RSP_GBI1_SetOtherModeH(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_SetOtherModeH);

	uint32 dwShift = ((gfx->words.w0)>>8)&0xFF;
	uint32 dwLength= ((gfx->words.w0)   )&0xFF;
	uint32 dwData  = (gfx->words.w1);

	uint32 dwMask = ((1<<dwLength)-1)<<dwShift;
	uint32 dwModeH = gRDP.otherModeH;

	dwModeH = (dwModeH&(~dwMask)) | dwData;
	Gfx tempgfx;
	tempgfx.words.w0 = dwModeH;
	tempgfx.words.w1 = gRDP.otherModeL;
	DLParser_RDPSetOtherMode(&tempgfx );
}


void RSP_GBI1_Texture(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_Texture);

	float fTextureScaleS = (float)(gfx->texture.scaleS) / (65536.0f * 32.0f);
	float fTextureScaleT = (float)(gfx->texture.scaleT) / (65536.0f * 32.0f);

	if( (((gfx->words.w1)>>16)&0xFFFF) == 0xFFFF )
	{
		fTextureScaleS = 1/32.0f;
	}
	else if( (((gfx->words.w1)>>16)&0xFFFF) == 0x8000 )
	{
		fTextureScaleS = 1/64.0f;
	}

	if( (((gfx->words.w1)    )&0xFFFF) == 0xFFFF )
	{
		fTextureScaleT = 1/32.0f;
	}
	else if( (((gfx->words.w1)    )&0xFFFF) == 0x8000 )
	{
		fTextureScaleT = 1/64.0f;
	}

	if( gRSP.ucode == 6 )
	{
		if( fTextureScaleS == 0 )	fTextureScaleS = 1.0f/32.0f;
		if( fTextureScaleT == 0 )	fTextureScaleT = 1.0f/32.0f;
	}

	CRender::g_pRender->SetTextureEnableAndScale(gfx->texture.tile, gfx->texture.enable_gbi0, fTextureScaleS, fTextureScaleT);
}

extern void RSP_RDP_InsertMatrix(uint32 word0, uint32 word1);
void RSP_GBI1_MoveWord(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_MoveWord);

	switch (gfx->gbi0moveword.type)
	{
	case RSP_MOVE_WORD_MATRIX:
		RSP_RDP_InsertMatrix(gfx);
		break;
	case RSP_MOVE_WORD_NUMLIGHT:
		{
			uint32 dwNumLights = (((gfx->gbi0moveword.value)-0x80000000)/32)-1;
			gRSP.ambientLightIndex = dwNumLights;
			SetNumLights(dwNumLights);
		}
		break;
	case RSP_MOVE_WORD_CLIP:
		{
			switch (gfx->gbi0moveword.offset)
			{
			case RSP_MV_WORD_OFFSET_CLIP_RNX:
			case RSP_MV_WORD_OFFSET_CLIP_RNY:
			case RSP_MV_WORD_OFFSET_CLIP_RPX:
			case RSP_MV_WORD_OFFSET_CLIP_RPY:
				CRender::g_pRender->SetClipRatio(gfx->gbi0moveword.offset, gfx->gbi0moveword.value);
				break;
			default:
				break;
			}
		}
		break;
	case RSP_MOVE_WORD_SEGMENT:
		{
			uint32 dwSegment = (gfx->gbi0moveword.offset >> 2) & 0xF;
			uint32 dwBase = (gfx->gbi0moveword.value)&0x00FFFFFF;
			if( dwBase > g_dwRamSize )
			{
				gRSP.segments[dwSegment] = dwBase;
			}
			else
			{
				gRSP.segments[dwSegment] = dwBase;
			}
		}
		break;
	case RSP_MOVE_WORD_FOG:
		{
			uint16 wMult = (uint16)(((gfx->gbi0moveword.value) >> 16) & 0xFFFF);
			uint16 wOff  = (uint16)(((gfx->gbi0moveword.value)      ) & 0xFFFF);

			float fMult = (float)(short)wMult;
			float fOff = (float)(short)wOff;

			float rng = 128000.0f / fMult;
			float fMin = 500.0f - (fOff*rng/256.0f);
			float fMax = rng + fMin;

			//if( fMult <= 0 || fMin > fMax || fMax < 0 || fMin > 1000 )
			if( fMult <= 0 || fMax < 0 )
			{
				// Hack
				fMin = 996;
				fMax = 1000;
				fMult = 0;
				fOff = 1;
			}

			SetFogMinMax(fMin, fMax, fMult, fOff);
		}
		break;
	case RSP_MOVE_WORD_LIGHTCOL:
		{
			uint32 dwLight = gfx->gbi0moveword.offset / 0x20;
			uint32 dwField = (gfx->gbi0moveword.offset & 0x7);

			switch (dwField)
			{
			case 0:
				if (dwLight == gRSP.ambientLightIndex)
				{
					SetAmbientLight( ((gfx->gbi0moveword.value)>>8) );
				}
				else
				{
					SetLightCol(dwLight, gfx->gbi0moveword.value);
				}
				break;

			case 4:
				break;

			default:
				break;
			}
		}
		break;
	case RSP_MOVE_WORD_POINTS:
		{
			uint32 vtx = gfx->gbi0moveword.offset/40;
			uint32 where = gfx->gbi0moveword.offset - vtx*40;
			ModifyVertexInfo(where, vtx, gfx->gbi0moveword.value);
		}
		break;
	case RSP_MOVE_WORD_PERSPNORM:
		break;
	default:
		break;
	}

}


void RSP_GBI1_PopMtx(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_PopMtx);

	if (gfx->gbi0popmatrix.projection)
	{
		CRender::g_pRender->PopProjection();
	}
	else
	{
		CRender::g_pRender->PopWorldView();
	}
}



void RSP_GBI1_CullDL(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_CullDL);

	if( g_curRomInfo.bDisableCulling )
	{
		return;	//Disable Culling
	}

	uint32 i;
	uint32 dwVFirst = ((gfx->words.w0) & 0xFFF) / gRSP.vertexMult;
	uint32 dwVLast  = (((gfx->words.w1)) & 0xFFF) / gRSP.vertexMult;

	// Mask into range
	dwVFirst &= 0x1f;
	dwVLast &= 0x1f;

	if( dwVLast < dwVFirst )	return;
	if( !gRSP.bRejectVtx )	return;

	for (i = dwVFirst; i <= dwVLast; i++)
	{
		if (g_clipFlag[i] == 0)
		{
			return;
		}
	}

	status.dwNumDListsCulled++;

	RDP_GFX_PopDL();
}



void RSP_GBI1_Tri1(Gfx *gfx)
{
	status.primitiveType = PRIM_TRI1;
	bool bTrisAdded = false;
	bool bTexturesAreEnabled = CRender::g_pRender->IsTextureEnabled();

	// While the next command pair is Tri1, add vertices
	uint32 dwPC = gDlistStack[gDlistStackPointer].pc;
	uint32 * pCmdBase = (uint32 *)(g_pRDRAMu8 + dwPC);
	
	do
	{
		uint32 dwV0 = gfx->tri1.v0/gRSP.vertexMult;
		uint32 dwV1 = gfx->tri1.v1/gRSP.vertexMult;
		uint32 dwV2 = gfx->tri1.v2/gRSP.vertexMult;

		if (IsTriangleVisible(dwV0, dwV1, dwV2))
		{
			if (!bTrisAdded)
			{
				if( bTexturesAreEnabled )
				{
					PrepareTextures();
					InitVertexTextureConstants();
				}
				CRender::g_pRender->SetCombinerAndBlender();
				bTrisAdded = true;
			}
			PrepareTriangle(dwV0, dwV1, dwV2);
		}

		gfx++;
		dwPC += 8;

	} while (gfx->words.cmd == (uint8)RSP_TRI1);

	gDlistStack[gDlistStackPointer].pc = dwPC-8;

	if (bTrisAdded)	
	{
		CRender::g_pRender->DrawTriangles();
	}
}


void RSP_GBI0_Tri4(Gfx *gfx)
{
	uint32 w0 = gfx->words.w0;
	uint32 w1 = gfx->words.w1;

	status.primitiveType = PRIM_TRI2;

	// While the next command pair is Tri2, add vertices
	uint32 dwPC = gDlistStack[gDlistStackPointer].pc;

	BOOL bTrisAdded = FALSE;

	do {
		uint32 dwFlag = (w0>>16)&0xFF;

		BOOL bVisible;
		for( int i=0; i<4; i++)
		{
			uint32 v0 = (w1>>(4+(i<<3))) & 0xF;
			uint32 v1 = (w1>>(  (i<<3))) & 0xF;
			uint32 v2 = (w0>>(  (i<<2))) & 0xF;
			bVisible = IsTriangleVisible(v0, v2, v1);
			if (bVisible)
			{
				if (!bTrisAdded && CRender::g_pRender->IsTextureEnabled())
				{
					PrepareTextures();
					InitVertexTextureConstants();
				}

				if( !bTrisAdded )
				{
					CRender::g_pRender->SetCombinerAndBlender();
				}

				bTrisAdded = true;
				PrepareTriangle(v0, v2, v1);
			}
		}
		
		w0			= *(uint32 *)(g_pRDRAMu8 + dwPC+0);
		w1			= *(uint32 *)(g_pRDRAMu8 + dwPC+4);
		dwPC += 8;

	} while (((w0)>>24) == (uint8)RSP_TRI2);


	gDlistStack[gDlistStackPointer].pc = dwPC-8;


	if (bTrisAdded)	
	{
		CRender::g_pRender->DrawTriangles();
	}
	
	gRSP.DKRVtxCount=0;
}

//Nintro64 uses Sprite2d 


void RSP_RDP_Nothing(Gfx *gfx)
{
	SP_Timing(RSP_RDP_Nothing);

	if( options.bEnableHacks )
		return;
	
	gDlistStackPointer=-1;
}


void RSP_RDP_InsertMatrix(Gfx *gfx)
{
	float fraction;

	UpdateCombinedMatrix();

	if ((gfx->words.w0) & 0x20)
	{
		int x = ((gfx->words.w0) & 0x1F) >> 1;
		int y = x >> 2;
		x &= 3;

		fraction = ((gfx->words.w1)>>16)/65536.0f;
		gRSPworldProject.m[y][x] = (float)(int)gRSPworldProject.m[y][x];
		gRSPworldProject.m[y][x] += fraction;

		fraction = ((gfx->words.w1)&0xFFFF)/65536.0f;
		gRSPworldProject.m[y][x+1] = (float)(int)gRSPworldProject.m[y][x+1];
		gRSPworldProject.m[y][x+1] += fraction;
	}
	else
	{
		int x = ((gfx->words.w0) & 0x1F) >> 1;
		int y = x >> 2;
		x &= 3;

		fraction = (float)fabs(gRSPworldProject.m[y][x] - (int)gRSPworldProject.m[y][x]);
		gRSPworldProject.m[y][x] = (short)((gfx->words.w1)>>16) + fraction;

		fraction = (float)fabs(gRSPworldProject.m[y][x+1] - (int)gRSPworldProject.m[y][x+1]);
		gRSPworldProject.m[y][x+1] = (short)((gfx->words.w1)&0xFFFF) + fraction;
	}

	gRSP.bMatrixIsUpdated = false;
	gRSP.bCombinedMatrixIsUpdated = true;
}