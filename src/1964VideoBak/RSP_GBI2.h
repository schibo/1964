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

void RSP_GBI2_Vtx(Gfx *gfx)
{
	uint32 addr = RSPSegmentAddr((gfx->gbi2vtx.addr));
	int vend	= gfx->gbi2vtx.vend/2;
	int n		= gfx->gbi2vtx.n;
	int v0		= vend - n;

	if( vend > 64 )
	{
		return;
	}

	if ((addr + (n*16)) > g_dwRamSize)
	{
	}
	else
	{
		ProcessVertexData(addr, v0, n);
		status.dwNumVertices += n;
		DisplayVertexInfo(addr, v0, n);
	}
}

void RSP_GBI2_EndDL(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_EndDL);

	RDP_GFX_PopDL();
}

void RSP_GBI2_CullDL(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_CullDL);

	if( g_curRomInfo.bDisableCulling )
	{
		return;	//Disable Culling
	}

	uint32 i;
	uint32 dwVFirst = (((gfx->words.w0)) & 0xfff) / gRSP.vertexMult;
	uint32 dwVLast  = (((gfx->words.w1)) & 0xfff) / gRSP.vertexMult;

	// Mask into range
	dwVFirst &= 0x1f;
	dwVLast &= 0x1f;

	if( dwVLast < dwVFirst )	return;
	if( !gRSP.bRejectVtx )	return;

	for (i = dwVFirst; i <= dwVLast; i++)
	{
		//if (g_dwVtxFlags[i] == 0)
		if (g_clipFlag[i] == 0)
		{
			return;
		}
	}

	status.dwNumDListsCulled++;

	RDP_GFX_PopDL();
}

void RSP_GBI2_MoveWord(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_MoveWord);

	switch (gfx->gbi2moveword.type)
	{
	case RSP_MOVE_WORD_MATRIX:
		RSP_RDP_InsertMatrix(gfx);
		break;
	case RSP_MOVE_WORD_NUMLIGHT:
		{
			uint32 dwNumLights = gfx->gbi2moveword.value/24;
			gRSP.ambientLightIndex = dwNumLights;
			SetNumLights(dwNumLights);
		}
		break;

	case RSP_MOVE_WORD_CLIP:
		{
			switch (gfx->gbi2moveword.offset)
			{
			case RSP_MV_WORD_OFFSET_CLIP_RNX:
			case RSP_MV_WORD_OFFSET_CLIP_RNY:
			case RSP_MV_WORD_OFFSET_CLIP_RPX:
			case RSP_MV_WORD_OFFSET_CLIP_RPY:
				CRender::g_pRender->SetClipRatio(gfx->gbi2moveword.offset, gfx->gbi2moveword.value);
			default:
				break;
			}
		}
		break;

	case RSP_MOVE_WORD_SEGMENT:
		{
			uint32 dwSeg     = gfx->gbi2moveword.offset / 4;
			uint32 dwAddr = gfx->gbi2moveword.value & 0x00FFFFFF;			// Hack - convert to physical

			if( dwAddr > g_dwRamSize )
			{
				gRSP.segments[dwSeg] = dwAddr;
			}
			else
			{
				gRSP.segments[dwSeg] = dwAddr;
			}
		}
		break;
	case RSP_MOVE_WORD_FOG:
		{
			uint16 wMult = (uint16)((gfx->gbi2moveword.value >> 16) & 0xFFFF);
			uint16 wOff  = (uint16)((gfx->gbi2moveword.value      ) & 0xFFFF);

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
			uint32 dwLight = gfx->gbi2moveword.offset / 0x18;
			uint32 dwField = (gfx->gbi2moveword.offset & 0x7);

			switch (dwField)
			{
			case 0:
				if (dwLight == gRSP.ambientLightIndex)
				{
					SetAmbientLight( (gfx->gbi2moveword.value>>8) );
				}
				else
				{
					SetLightCol(dwLight, gfx->gbi2moveword.value);
				}
				break;

			case 4:
				break;

			default:
				break;
			}


		}
		break;

	case RSP_MOVE_WORD_PERSPNORM:
		break;

	case RSP_MOVE_WORD_POINTS:
		break;

	default:
		break;
	}
}

void RSP_GBI2_Tri1(Gfx *gfx)
{
	if( gfx->words.w0 == 0x05000017 && gfx->gbi2tri1.flag == 0x80 )
	{
		// The ObjLoadTxtr / Tlut cmd for Evangelion.v64
		RSP_S2DEX_SPObjLoadTxtr(gfx);
	}
	else
	{
		status.primitiveType = PRIM_TRI1;
		bool bTrisAdded = false;
		bool bTexturesAreEnabled = CRender::g_pRender->IsTextureEnabled();

		// While the next command pair is Tri1, add vertices
		uint32 dwPC = gDlistStack[gDlistStackPointer].pc;
		uint32 * pCmdBase = (uint32 *)(g_pRDRAMu8 + dwPC);

		do
		{
			uint32 dwV2 = gfx->gbi2tri1.v2/gRSP.vertexMult;
			uint32 dwV1 = gfx->gbi2tri1.v1/gRSP.vertexMult;
			uint32 dwV0 = gfx->gbi2tri1.v0/gRSP.vertexMult;

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

		}
		while( gfx->words.cmd == (uint8)RSP_ZELDATRI1);

		gDlistStack[gDlistStackPointer].pc = dwPC-8;

		if (bTrisAdded)	
		{
			CRender::g_pRender->DrawTriangles();
		}
	}
}

void RSP_GBI2_Tri2(Gfx *gfx)
{
	if( gfx->words.w0 == 0x0600002f && gfx->gbi2tri2.flag == 0x80 )
	{
		// The ObjTxSprite cmd for Evangelion.v64
		RSP_S2DEX_SPObjLoadTxSprite(gfx);
	}
	else
	{
		status.primitiveType = PRIM_TRI2;
		BOOL bTrisAdded = FALSE;

		// While the next command pair is Tri2, add vertices
		uint32 dwPC = gDlistStack[gDlistStackPointer].pc;
		bool bTexturesAreEnabled = CRender::g_pRender->IsTextureEnabled();

		do {
			uint32 dwV2 = gfx->gbi2tri2.v2;
			uint32 dwV1 = gfx->gbi2tri2.v1;
			uint32 dwV0 = gfx->gbi2tri2.v0;

			uint32 dwV5 = gfx->gbi2tri2.v5;
			uint32 dwV4 = gfx->gbi2tri2.v4;
			uint32 dwV3 = gfx->gbi2tri2.v3;

			// Do first tri
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

			// Do second tri
			if (IsTriangleVisible(dwV3, dwV4, dwV5))
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

				PrepareTriangle(dwV3, dwV4, dwV5);
			}
			
			gfx++;
			dwPC += 8;

		} while ( gfx->words.cmd == (uint8)RSP_ZELDATRI2 );//&& status.dwNumTrisRendered < 50);

		gDlistStack[gDlistStackPointer].pc = dwPC-8;

		if (bTrisAdded)	
		{
			CRender::g_pRender->DrawTriangles();
		}
	}
}

void RSP_GBI2_Line3D(Gfx *gfx)
{
	if( gfx->words.w0 == 0x0700002f && (gfx->words.w1>>24) == 0x80 )
	{
		// The ObjTxSprite cmd for Evangelion.v64
		RSP_S2DEX_SPObjLoadTxRect(gfx);
	}
	else
	{
		status.primitiveType = PRIM_TRI3;

		uint32 dwPC = gDlistStack[gDlistStackPointer].pc;

		BOOL bTrisAdded = FALSE;

		do {
			uint32 dwV0 = gfx->gbi2line3d.v0/gRSP.vertexMult;
			uint32 dwV1 = gfx->gbi2line3d.v1/gRSP.vertexMult;
			uint32 dwV2 = gfx->gbi2line3d.v2/gRSP.vertexMult;

			uint32 dwV3 = gfx->gbi2line3d.v3/gRSP.vertexMult;
			uint32 dwV4 = gfx->gbi2line3d.v4/gRSP.vertexMult;
			uint32 dwV5 = gfx->gbi2line3d.v5/gRSP.vertexMult;

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
			if (IsTriangleVisible(dwV3, dwV4, dwV5))
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
				PrepareTriangle(dwV3, dwV4, dwV5);
			}
			
			gfx++;
			dwPC += 8;
		} while ( gfx->words.cmd == (uint8)RSP_LINE3D);

		gDlistStack[gDlistStackPointer].pc = dwPC-8;


		if (bTrisAdded)	
		{
			CRender::g_pRender->DrawTriangles();
		}
	}
}

void RSP_GBI2_Texture(Gfx *gfx)
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

	CRender::g_pRender->SetTextureEnableAndScale(gfx->texture.tile, gfx->texture.enable_gbi2, fTextureScaleS, fTextureScaleT);

	/*
	if( g_curRomInfo.bTextureScaleHack )
	{
	// Hack, need to verify, refer to N64 programming manual
	// that if scale = 0.5 (1/64), vtx s,t are also doubled

		if( ((word1>>16)&0xFFFF) == 0x8000 )
		{
			fTextureScaleS = 1/128.0f;
			if( ((word1)&0xFFFF) == 0xFFFF )
			{
				fTextureScaleT = 1/64.0f;
			}
		}

		if( ((word1    )&0xFFFF) == 0x8000 )
		{
			fTextureScaleT = 1/128.0f;
			if( ((word1>>16)&0xFFFF) == 0xFFFF )
			{
				fTextureScaleS = 1/64.0f;
			}
		}
	}
	*/

	CRender::g_pRender->SetTextureEnableAndScale(gfx->texture.tile, gfx->texture.enable_gbi2, fTextureScaleS, fTextureScaleT);
}



void RSP_GBI2_PopMtx(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_PopMtx);

	uint8 nCommand = (uint8)(gfx->words.w0 & 0xFF);


/*	if (nCommand & RSP_ZELDA_MTX_PROJECTION)
	{
		CRender::g_pRender->PopProjection();
	}
	else*/
	{
		CRender::g_pRender->PopWorldView();
	}
}


#define RSP_ZELDA_ZBUFFER				0x00000001		// Guess
#define RSP_ZELDA_CULL_BACK			0x00000200
#define RSP_ZELDA_CULL_FRONT			0x00000400
#define RSP_ZELDA_FOG					0x00010000
#define RSP_ZELDA_LIGHTING			0x00020000
#define RSP_ZELDA_TEXTURE_GEN			0x00040000
#define RSP_ZELDA_TEXTURE_GEN_LINEAR	0x00080000
#define RSP_ZELDA_SHADING_SMOOTH		0x00200000

void RSP_GBI2_GeometryMode(Gfx *gfx)
{
	SP_Timing(RSP_GBI2_GeometryMode);

	uint32 dwAnd = ((gfx->words.w0)) & 0x00FFFFFF;
	uint32 dwOr  = ((gfx->words.w1)) & 0x00FFFFFF;

		gRDP.geometryMode &= dwAnd;
	gRDP.geometryMode |= dwOr;


	bool bCullFront		= (gRDP.geometryMode & RSP_ZELDA_CULL_FRONT) ? true : false;
	bool bCullBack		= (gRDP.geometryMode & RSP_ZELDA_CULL_BACK) ? true : false;
	
	BOOL bShade			= (gRDP.geometryMode & G_SHADE) ? TRUE : FALSE;
	//BOOL bFlatShade		= (gRDP.geometryMode & RSP_ZELDA_SHADING_SMOOTH) ? TRUE : FALSE;
	BOOL bFlatShade		= (gRDP.geometryMode & RSP_ZELDA_TEXTURE_GEN_LINEAR) ? TRUE : FALSE;
	if( options.enableHackForGames == HACK_FOR_TIGER_HONEY_HUNT )
		bFlatShade		= FALSE;
	
	bool bFog			= (gRDP.geometryMode & RSP_ZELDA_FOG) ? true : false;
	bool bTextureGen	= (gRDP.geometryMode & RSP_ZELDA_TEXTURE_GEN) ? true : false;

	bool bLighting      = (gRDP.geometryMode & RSP_ZELDA_LIGHTING) ? true : false;
	BOOL bZBuffer		= (gRDP.geometryMode & RSP_ZELDA_ZBUFFER)	? TRUE : FALSE;	

	CRender::g_pRender->SetCullMode(bCullFront, bCullBack);
	
	//if (bFlatShade||!bShade)	CRender::g_pRender->SetShadeMode( SHADE_FLAT );
	if (bFlatShade)	CRender::g_pRender->SetShadeMode( SHADE_FLAT );
	else			CRender::g_pRender->SetShadeMode( SHADE_SMOOTH );
	
	SetTextureGen(bTextureGen);

	SetLighting( bLighting );
	CRender::g_pRender->ZBufferEnable( bZBuffer );
	CRender::g_pRender->SetFogEnable( bFog );
}


int dlistMtxCount=0;
extern uint32 dwConkerVtxZAddr;

void RSP_GBI2_Mtx(Gfx *gfx)
{	
	SP_Timing(RSP_GBI0_Mtx);
	dwConkerVtxZAddr = 0;	// For Conker BFD

	uint32 addr = RSPSegmentAddr((gfx->gbi2matrix.addr));

	if( gfx->gbi2matrix.param == 0 && gfx->gbi2matrix.len == 0 )
	{
		DLParser_Bomberman2TextRect(gfx);
		return;
	}

	if (addr + 64 > g_dwRamSize)
	{
		return;
	}

	LoadMatrix(addr);

	if (gfx->gbi2matrix.projection)
	{
		// So far only Extreme-G seems to Push/Pop projection matrices	
		CRender::g_pRender->SetProjection(matToLoad, gfx->gbi2matrix.nopush==0, gfx->gbi2matrix.load);
	}
	else
	{
		CRender::g_pRender->SetWorldView(matToLoad, gfx->gbi2matrix.nopush==0, gfx->gbi2matrix.load);

		if( options.enableHackForGames == HACK_FOR_SOUTH_PARK_RALLY )
		{
			dlistMtxCount++;
			if( dlistMtxCount == 2 )
			{
				CRender::g_pRender->ClearZBuffer(1.0f);
			}
		}
	}
}

void RSP_GBI2_MoveMem(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_MoveMem);

	uint32 addr = RSPSegmentAddr((gfx->words.w1));
	uint32 type    = ((gfx->words.w0)     ) & 0xFE;

	//uint32 dwLen = ((gfx->words.w0) >> 16) & 0xFF;
	//uint32 dwOffset = ((gfx->words.w0) >> 8) & 0xFFFF;

	switch (type)
	{
	case RSP_GBI2_MV_MEM__VIEWPORT:
		{
			RSP_MoveMemViewport(addr);
		}
		break;
	case RSP_GBI2_MV_MEM__LIGHT:
		{
			uint32 dwOffset2 = ((gfx->words.w0) >> 5) & 0x3FFF;
		switch (dwOffset2)
		{
		case 0x00:
			{
				s8 * pcBase = g_pRDRAMs8 + addr;
			}
			break;
		case 0x18:
			{
				s8 * pcBase = g_pRDRAMs8 + addr;
			}
			break;
		default:		//0x30/48/60
			{
				uint32 dwLight = (dwOffset2 - 0x30)/0x18;
					RSP_MoveMemLight(dwLight, addr);
			}
			break;
		}
		break;

		}
	case RSP_GBI2_MV_MEM__MATRIX:
		RSP_GFX_Force_Matrix(addr);
		break;
	case RSP_GBI2_MV_MEM_O_L0:
	case RSP_GBI2_MV_MEM_O_L1:
	case RSP_GBI2_MV_MEM_O_L2:
	case RSP_GBI2_MV_MEM_O_L3:
	case RSP_GBI2_MV_MEM_O_L4:
	case RSP_GBI2_MV_MEM_O_L5:
	case RSP_GBI2_MV_MEM_O_L6:
	case RSP_GBI2_MV_MEM_O_L7:
		break;

	case RSP_GBI2_MV_MEM__POINT:
		break;

	case RSP_GBI2_MV_MEM_O_LOOKATX:
		if( (gfx->words.w0) == 0xDC170000 && ((gfx->words.w1)&0xFF000000) == 0x80000000 )
		{
			// Ucode for Evangelion.v64, the ObjMatrix cmd
			RSP_S2DEX_OBJ_MOVEMEM(gfx);
		}
		break;
	case RSP_GBI2_MV_MEM_O_LOOKATY:
		break;
	case 0x02:
		if( (gfx->words.w0) == 0xDC070002 && ((gfx->words.w1)&0xFF000000) == 0x80000000 )
		{
			RSP_S2DEX_OBJ_MOVEMEM(gfx);
			break;
		}
	default:
		break;
	}
}



void RSP_GBI2_DL(Gfx *gfx)
{
	SP_Timing(RSP_GBI0_DL);

	uint32 dwPush = ((gfx->words.w0) >> 16) & 0xFF;
	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));

	if( dwAddr > g_dwRamSize )
	{
		dwAddr &= (g_dwRamSize-1);
	}

	
	switch (dwPush)
	{
	case RSP_DLIST_PUSH:
		gDlistStackPointer++;
		gDlistStack[gDlistStackPointer].pc = dwAddr;
		gDlistStack[gDlistStackPointer].countdown = MAX_DL_COUNT;

		break;
	case RSP_DLIST_NOPUSH:
		if( gDlistStack[gDlistStackPointer].pc == dwAddr+8 )	//Is this a loop
		{
			//Hack for Gauntlet Legends
			gDlistStack[gDlistStackPointer].pc = dwAddr+8;
		}
		else
			gDlistStack[gDlistStackPointer].pc = dwAddr;
		gDlistStack[gDlistStackPointer].countdown = MAX_DL_COUNT;
		break;
	}

}



void RSP_GBI2_SetOtherModeL(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_SetOtherModeL);

	uint32 dwShift = ((gfx->words.w0)>>8)&0xFF;
	uint32 dwLength= ((gfx->words.w0)   )&0xFF;
	uint32 dwData  = (gfx->words.w1);

	// Mask is constructed slightly differently
	uint32 dwMask = (uint32)((s32)(0x80000000)>>dwLength)>>dwShift;
	dwData &= dwMask;

	uint32 modeL = gRDP.otherModeL;
	modeL = (modeL&(~dwMask)) | dwData;

	Gfx tempgfx;
	tempgfx.words.w0 = gRDP.otherModeH;
	tempgfx.words.w1 = modeL;
	DLParser_RDPSetOtherMode(&tempgfx );
}



void RSP_GBI2_SetOtherModeH(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_SetOtherModeH);

	uint32 dwLength= (((gfx->words.w0))&0xFF)+1;
	uint32 dwShift = 32 - (((gfx->words.w0)>>8)&0xFF) - dwLength;
	uint32 dwData  = (gfx->words.w1);

	uint32 dwMask2 = ((1<<dwLength)-1)<<dwShift;
	uint32 dwModeH = gRDP.otherModeH;
	dwModeH = (dwModeH&(~dwMask2)) | dwData;

	Gfx tempgfx;
	tempgfx.words.w0 = dwModeH;
	tempgfx.words.w1 = gRDP.otherModeL;
	DLParser_RDPSetOtherMode(&tempgfx );
}


void RSP_GBI2_SubModule(Gfx *gfx)
{
	SP_Timing(RSP_GBI2_SubModule);

}
