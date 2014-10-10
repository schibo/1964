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

// A few ucode used in DKR and Others Special games

uint32 dwConkerVtxZAddr=0;

static void RDP_GFX_DumpVtxInfoDKR(uint32 dwAddr, uint32 dwV0, uint32 dwN);

void RDP_GFX_DLInMem(Gfx *gfx)
{
	uint32 dwLimit = ((gfx->words.w0) >> 16) & 0xFF;
	uint32 dwPush = RSP_DLIST_PUSH; //((gfx->words.w0) >> 16) & 0xFF;
	uint32 dwAddr = 0x00000000 | (gfx->words.w1); //RSPSegmentAddr((gfx->words.w1));

	switch (dwPush)
	{
	case RSP_DLIST_PUSH:
		gDlistStackPointer++;
		gDlistStack[gDlistStackPointer].pc = dwAddr;
		gDlistStack[gDlistStackPointer].countdown = dwLimit;

		break;
	case RSP_DLIST_NOPUSH:
		gDlistStack[gDlistStackPointer].pc = dwAddr;
		gDlistStack[gDlistStackPointer].countdown = dwLimit;
		break;
	}

}

extern Matrix dkrMatrixTransposed;
void RSP_Mtx_DKR(Gfx *gfx)
{	
	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));
	uint32 dwCommand = ((gfx->words.w0)>>16)&0xFF;
	uint32 dwLength  = ((gfx->words.w0))    &0xFFFF;

	dwAddr = (gfx->words.w1)+RSPSegmentAddr(gRSP.dwDKRMatrixAddr);

	//gRSP.DKRCMatrixIndex = ((gfx->words.w0)>>22)&3;
	bool mul=false;
	int index;
	switch( dwCommand )
	{
	case 0xC0:	// DKR
		gRSP.DKRCMatrixIndex = index = 3;
		break;
	case 0x80:	// DKR
		gRSP.DKRCMatrixIndex = index = 2;
		break;
	case 0x40:	// DKR
		gRSP.DKRCMatrixIndex = index = 1;
		break;
	case 0x20:	// DKR
		gRSP.DKRCMatrixIndex = index = 0;
		break;
	case 0x00:
		gRSP.DKRCMatrixIndex = index = 0;
		break;
	case 0x01:
		//mul = true;
		gRSP.DKRCMatrixIndex = index = 1;
		break;
	case 0x02:
		//mul = true;
		gRSP.DKRCMatrixIndex = index = 2;
		break;
	case 0x03:
		//mul = true;
		gRSP.DKRCMatrixIndex = index = 3;
		break;
	case 0x81:
		index = 1;
		mul = true;
		break;
	case 0x82:
		index = 2;
		mul = true;
		break;
	case 0x83:
		index = 3;
		mul = true;
		break;
	default:
		break;
	}

	// Load matrix from dwAddr
	Matrix &mat = gRSP.DKRMatrixes[index];
	LoadMatrix(dwAddr);

	if( mul )
	{
		mat = matToLoad*gRSP.DKRMatrixes[0];
	}
	else
	{
		mat = matToLoad;
	}

	if( status.isSSEEnabled )
		D3DXMatrixTranspose(&dkrMatrixTransposed, &mat);
}

void RSP_Vtx_DKR(Gfx *gfx)
{
	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));
	uint32 dwV0 = (((gfx->words.w0) >> 9 )&0x1F);
	uint32 dwN  = (((gfx->words.w0) >>19 )&0x1F)+1;

	if( gfx->words.w0 & 0x00010000 )
	{
		if( gRSP.DKRBillBoard )
			gRSP.DKRVtxCount = 1;
	}
	else
	{
		gRSP.DKRVtxCount = 0;
	}

	dwV0 += gRSP.DKRVtxCount;

	if (dwV0 >= 32)		dwV0 = 31;
	
	if ((dwV0 + dwN) > 32)
	{
		dwN = 32 - dwV0;
	}

	
	//if( dwAddr == 0 || dwAddr < 0x2000)
	{
		dwAddr = (gfx->words.w1)+RSPSegmentAddr(gRSP.dwDKRVtxAddr);
	}

	// Check that address is valid...
	if ((dwAddr + (dwN*16)) > g_dwRamSize)
	{
	}
	else
	{
		ProcessVertexDataDKR(dwAddr, dwV0, dwN);

		status.dwNumVertices += dwN;

		RDP_GFX_DumpVtxInfoDKR(dwAddr, dwV0, dwN);
	}
}


void RSP_Vtx_Gemini(Gfx *gfx)
{
	
	

	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));
	uint32 dwV0 =  (((gfx->words.w0)>>9)&0x1F);
	uint32 dwN  = (((gfx->words.w0) >>19 )&0x1F);

	if (dwV0 >= 32)
		dwV0 = 31;

	if ((dwV0 + dwN) > 32)
	{
		dwN = 32 - dwV0;
	}


	//if( dwAddr == 0 || dwAddr < 0x2000)
	{
		dwAddr = (gfx->words.w1)+RSPSegmentAddr(gRSP.dwDKRVtxAddr);
	}

	// Check that address is valid...
	if ((dwAddr + (dwN*16)) > g_dwRamSize)
	{
	}
	else
	{
		ProcessVertexDataDKR(dwAddr, dwV0, dwN);

		status.dwNumVertices += dwN;

		RDP_GFX_DumpVtxInfoDKR(dwAddr, dwV0, dwN);
	}
}

// DKR verts are extra 4 bytes
void RDP_GFX_DumpVtxInfoDKR(uint32 dwAddr, uint32 dwV0, uint32 dwN)
{
}

void DLParser_Set_Addr_Ucode6(Gfx *gfx)
{
	gRSP.dwDKRMatrixAddr = (gfx->words.w0)&0x00FFFFFF;
	gRSP.dwDKRVtxAddr = (gfx->words.w1)&0x00FFFFFF;
	gRSP.DKRVtxCount=0;
}



void RSP_Vtx_WRUS(Gfx *gfx)
{
	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));
	uint32 dwLength = ((gfx->words.w0))&0xFFFF;

	uint32 dwN= (dwLength + 1) / 0x210;
	//uint32 dwN= (dwLength >> 9);
	//uint32 dwV0 = (((gfx->words.w0)>>16)&0x3f)/5;
	uint32 dwV0 = (((gfx->words.w0)>>16)&0xFF)/5;

	if (dwV0 >= 32)
		dwV0 = 31;
	
	if ((dwV0 + dwN) > 32)
	{
		dwN = 32 - dwV0;
	}

	ProcessVertexData(dwAddr, dwV0, dwN);

	status.dwNumVertices += dwN;

	DisplayVertexInfo(dwAddr, dwV0, dwN);
}

void RSP_Vtx_ShadowOfEmpire(Gfx *gfx)
{

	

	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));
	uint32 dwLength = ((gfx->words.w0))&0xFFFF;

	uint32 dwN= (((gfx->words.w0) >> 4) & 0xFFF) / 33 + 1;
	uint32 dwV0 = 0;

	if (dwV0 >= 32)
		dwV0 = 31;
	
	if ((dwV0 + dwN) > 32)
	{
		dwN = 32 - dwV0;
	}

	ProcessVertexData(dwAddr, dwV0, dwN);

	status.dwNumVertices += dwN;

	DisplayVertexInfo(dwAddr, dwV0, dwN);
}


void RSP_DL_In_MEM_DKR(Gfx *gfx)
{
	
	

	// This cmd is likely to execute number of ucode at the given address
	uint32 dwAddr = (gfx->words.w1);//RSPSegmentAddr((gfx->words.w1));
	{
		gDlistStackPointer++;
		gDlistStack[gDlistStackPointer].pc = dwAddr;
		gDlistStack[gDlistStackPointer].countdown = (((gfx->words.w0)>>16)&0xFF);
	}
}
uint16 ConvertYUVtoR5G5B5X1(int y, int u, int v)
{
	float r = y + (1.370705f * (v-128));
	float g = y - (0.698001f * (v-128)) - (0.337633f * (u-128));
	float b = y + (1.732446f * (u-128));
	r *= 0.125f;
	g *= 0.125f;
	b *= 0.125f;

	//clipping the result
	if (r > 32) r = 32;
	if (g > 32) g = 32;
	if (b > 32) b = 32;
	if (r < 0) r = 0;
	if (g < 0) g = 0;
	if (b < 0) b = 0;

	uint16 c = (uint16)(((uint16)(r) << 11) |
		((uint16)(g) << 6) |
		((uint16)(b) << 1) | 1);
	return c;
}

void TexRectToN64FrameBuffer_YUV_16b(uint32 x0, uint32 y0, uint32 width, uint32 height)
{
	// Convert YUV image at TImg and Copy the texture into the N64 RDRAM framebuffer memory

	uint32 n64CIaddr = g_CI.dwAddr;
	uint32 n64CIwidth = g_CI.dwWidth;

	for (uint32 y = 0; y < height; y++)
	{
		uint32* pN64Src = (uint32*)(g_pRDRAMu8+(g_TI.dwAddr&(g_dwRamSize-1)))+y*(g_TI.dwWidth>>1);
		uint16* pN64Dst = (uint16*)(g_pRDRAMu8+(n64CIaddr&(g_dwRamSize-1)))+(y+y0)*n64CIwidth;

		for (uint32 x = 0; x < width; x+=2)
		{
			uint32 val = *pN64Src++;
			int y0 = (uint8)val&0xFF;
			int v  = (uint8)(val>>8)&0xFF;
			int y1 = (uint8)(val>>16)&0xFF;
			int u  = (uint8)(val>>24)&0xFF;

			pN64Dst[x+x0] = ConvertYUVtoR5G5B5X1(y0,u,v);
			pN64Dst[x+x0+1] = ConvertYUVtoR5G5B5X1(y1,u,v);
		}
	}
}

extern uObjMtxReal gObjMtxReal;
void DLParser_OgreBatter64BG(Gfx *gfx)
{
	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));
	uObjTxSprite *ptr = (uObjTxSprite*)(g_pRDRAMu8+dwAddr);
	//CRender::g_pRender->LoadObjSprite(*ptr,true);
	PrepareTextures();

	CTexture *ptexture = g_textures[0].m_pCTexture;
	TexRectToN64FrameBuffer_16b( (uint32)gObjMtxReal.X, (uint32)gObjMtxReal.Y, ptexture->m_dwWidth, ptexture->m_dwHeight, gRSP.curTile);

}

void DLParser_Bomberman2TextRect(Gfx *gfx)
{
	// Bomberman 64 - The Second Attack! (U) [!]
	// The 0x02 cmd, list a TexRect cmd

	if( options.enableHackForGames == HACK_FOR_OGRE_BATTLE && gRDP.tiles[7].dwFormat == TXT_FMT_YUV )
	{
		TexRectToN64FrameBuffer_YUV_16b( (uint32)gObjMtxReal.X, (uint32)gObjMtxReal.Y, 16, 16);
		//DLParser_OgreBatter64BG((gfx->words.w0), (gfx->words.w1));
		return;
	}

	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));
	uObjSprite *info = (uObjSprite*)(g_pRDRAMu8+dwAddr);

	uint32 dwTile	= gRSP.curTile;

	PrepareTextures();
	
	//CRender::g_pRender->SetCombinerAndBlender();

	uObjTxSprite drawinfo;
	memcpy( &(drawinfo.sprite), info, sizeof(uObjSprite));
	CRender::g_pRender->DrawSpriteR(drawinfo, false, dwTile, 0, 0, drawinfo.sprite.imageW/32, drawinfo.sprite.imageH/32);
}


void RSP_MoveWord_DKR(Gfx *gfx)
{
	SP_Timing(RSP_GBI1_MoveWord);
	uint32 dwNumLights;

	switch ((gfx->words.w0) & 0xFF)
	{
	case RSP_MOVE_WORD_NUMLIGHT:
		dwNumLights = (gfx->words.w1)&0x7;

		gRSP.ambientLightIndex = dwNumLights;
		SetNumLights(dwNumLights);
		//gRSP.DKRBillBoard = (gfx->words.w1)&0x1 ? true : false;
		gRSP.DKRBillBoard = (gfx->words.w1)&0x7 ? true : false;

		break;
	case RSP_MOVE_WORD_LIGHTCOL:
		gRSP.DKRCMatrixIndex = ((gfx->words.w1)>>6)&7;
		//gRSP.DKRCMatrixIndex = ((gfx->words.w1)>>6)&3;
		break;
	default:
		RSP_GBI1_MoveWord(gfx);
		break;
	}

}


void RSP_DMA_Tri_DKR(Gfx *gfx)
{
	BOOL bTrisAdded = FALSE;
	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));
	uint32 flag = ((gfx->words.w0) & 0xFF0000) >> 16;
	if (flag&1) 
		CRender::g_pRender->SetCullMode(false,true);
	else
		CRender::g_pRender->SetCullMode(false,false);


	uint32 dwNum = (((gfx->words.w0) &  0xFFF0) >>4 );
	uint32 i;
	uint32 * pData = (uint32*)&g_pRDRAMu32[dwAddr/4];

	if( dwAddr+16*dwNum >= g_dwRamSize )
	{
		return;
	}

	status.primitiveType = PRIM_DMA_TRI;

	for (i = 0; i < dwNum; i++)
	{
		uint32 dwInfo = pData[0];

		uint32 dwV0 = (dwInfo >> 16) & 0x1F;
		uint32 dwV1 = (dwInfo >>  8) & 0x1F;
		uint32 dwV2 = (dwInfo      ) & 0x1F;

		//if (IsTriangleVisible(dwV0, dwV1, dwV2))
		{
			if (!bTrisAdded )//&& CRender::g_pRender->IsTextureEnabled())
			{
				PrepareTextures();
				InitVertexTextureConstants();
			}

			// Generate texture coordinates
			short s0 = ((short)(pData[1]>>16));
			short t0 = ((short)(pData[1]&0xFFFF));
			short s1 = ((short)(pData[2]>>16));
			short t1 = ((short)(pData[2]&0xFFFF));
			short s2 = ((short)(pData[3]>>16));
			short t2 = ((short)(pData[3]&0xFFFF));

			CRender::g_pRender->SetVtxTextureCoord(dwV0, s0, t0);
			CRender::g_pRender->SetVtxTextureCoord(dwV1, s1, t1);
			CRender::g_pRender->SetVtxTextureCoord(dwV2, s2, t2);

			if( !bTrisAdded )
			{
				CRender::g_pRender->SetCombinerAndBlender();
			}

			bTrisAdded = true;
			PrepareTriangle(dwV0, dwV1, dwV2);
		}

		pData += 4;

	}

	if (bTrisAdded)	
	{
		CRender::g_pRender->DrawTriangles();
	}
	gRSP.DKRVtxCount=0;
}

uint32 dwPDCIAddr = 0;
void ProcessVertexDataPD(uint32 dwAddr, uint32 dwV0, uint32 dwNum);
void RSP_Vtx_PD(Gfx *gfx)
{
	
	

	SP_Timing(RSP_GBI0_Vtx);

	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));
	uint32 dwV0 =  ((gfx->words.w0)>>16)&0x0F;
	uint32 dwN  = (((gfx->words.w0)>>20)&0x0F)+1;
	uint32 dwLength = ((gfx->words.w0))&0xFFFF;

	ProcessVertexDataPD(dwAddr, dwV0, dwN);
	status.dwNumVertices += dwN;
}

void RSP_Set_Vtx_CI_PD(Gfx *gfx)
{
	
	

	// Color index buf address
	dwPDCIAddr = RSPSegmentAddr((gfx->words.w1));
}

void RSP_Tri4_PD(Gfx *gfx)
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
		for( uint32 i=0; i<4; i++)
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

	} while ((w0>>24) == (uint8)RSP_TRI2);

	gDlistStack[gDlistStackPointer].pc = dwPC-8;

	if (bTrisAdded)	
	{
		CRender::g_pRender->DrawTriangles();
	}
}


void DLParser_Tri4_Conker(Gfx *gfx)
{
	uint32 w0 = gfx->words.w0;
	uint32 w1 = gfx->words.w1;

	status.primitiveType = PRIM_TRI2;

	// While the next command pair is Tri2, add vertices
	uint32 dwPC = gDlistStack[gDlistStackPointer].pc;

	BOOL bTrisAdded = FALSE;

	do {
		uint32 idx[12];
		idx[0] = (w1   )&0x1F;
		idx[1] = (w1>> 5)&0x1F;
		idx[2] = (w1>>10)&0x1F;
		idx[3] = (w1>>15)&0x1F;
		idx[4] = (w1>>20)&0x1F;
		idx[5] = (w1>>25)&0x1F;

		idx[6] = (w0    )&0x1F;
		idx[7] = (w0>> 5)&0x1F;
		idx[8] = (w0>>10)&0x1F;

		idx[ 9] = (((w0>>15)&0x7)<<2)|(w1>>30);
		idx[10] = (w0>>18)&0x1F;
		idx[11] = (w0>>23)&0x1F;

		BOOL bVisible;
		for( uint32 i=0; i<4; i++)
		{
			uint32 v0=idx[i*3  ];
			uint32 v1=idx[i*3+1];
			uint32 v2=idx[i*3+2];
			bVisible = IsTriangleVisible(v0, v1, v2);
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
				PrepareTriangle(v0, v1, v2);
			}
		}

		w0			= *(uint32 *)(g_pRDRAMu8 + dwPC+0);
		w1			= *(uint32 *)(g_pRDRAMu8 + dwPC+4);
		dwPC += 8;

	} while ((w0>>28) == 1);

	gDlistStack[gDlistStackPointer].pc = dwPC-8;

	if (bTrisAdded)	
	{
		CRender::g_pRender->DrawTriangles();
	}
}

void RDP_GFX_Force_Vertex_Z_Conker(uint32 dwAddr)
{
	dwConkerVtxZAddr = dwAddr;
}



void DLParser_MoveMem_Conker(Gfx *gfx)
{
	uint32 dwType    = ((gfx->words.w0)     ) & 0xFE;
	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));
	if( dwType == RSP_GBI2_MV_MEM__MATRIX )
	{
		RDP_GFX_Force_Vertex_Z_Conker(dwAddr);
	}
	else if( dwType == RSP_GBI2_MV_MEM__LIGHT )
	{
		uint32 dwOffset2 = ((gfx->words.w0) >> 5) & 0x3FFF;
		uint32 dwLight=0xFF;
		if( dwOffset2 >= 0x30 )
		{
			dwLight = (dwOffset2 - 0x30)/0x30;
			RSP_MoveMemLight(dwLight, dwAddr);
		}
		else
		{
			// fix me
		}
	}
	else
	{
		RSP_GBI2_MoveMem(gfx);
	}
}

extern void ProcessVertexDataConker(uint32 dwAddr, uint32 dwV0, uint32 dwNum);
void RSP_Vtx_Conker(Gfx *gfx)
{
	
	

	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));
	uint32 dwVEnd    = (((gfx->words.w0)   )&0xFFF)/2;
	uint32 dwN      = (((gfx->words.w0)>>12)&0xFFF);
	uint32 dwV0		= dwVEnd - dwN;

	ProcessVertexDataConker(dwAddr, dwV0, dwN);
	status.dwNumVertices += dwN;
	DisplayVertexInfo(dwAddr, dwV0, dwN);
}


void DLParser_MoveWord_Conker(Gfx *gfx)
{
	uint32 dwType   = ((gfx->words.w0) >> 16) & 0xFF;
	if( dwType != RSP_MOVE_WORD_NUMLIGHT )
	{
		RSP_GBI2_MoveWord(gfx);
	}
	else
	{
		uint32 dwNumLights = ((gfx->words.w1)/48);
		gRSP.ambientLightIndex = dwNumLights+1;
		SetNumLights(dwNumLights);
	}
}

void DLParser_Ucode8_0x0(Gfx *gfx)
{
	if( (gfx->words.w0) == 0 && (gfx->words.w1) )
	{
		uint32 newaddr = RSPSegmentAddr((gfx->words.w1));

		if( newaddr && newaddr < g_dwRamSize)
		{
			if( gDlistStackPointer < MAX_DL_STACK_SIZE-1 )
			{
				gDlistStackPointer++;
				gDlistStack[gDlistStackPointer].pc = newaddr+8;	// Always skip the first 2 entries
				gDlistStack[gDlistStackPointer].countdown = MAX_DL_COUNT;
			}
			else
			{
			}
		}
	}
	else
	{
		gDlistStack[gDlistStackPointer].pc += 8;
	}
}


uint32 Rogue_Squadron_Vtx_XYZ_Cmd;
uint32 Rogue_Squadron_Vtx_XYZ_Addr;
uint32 Rogue_Squadron_Vtx_Color_Cmd;
uint32 Rogue_Squadron_Vtx_Color_Addr;
uint32 GSBlkAddrSaves[100][2];

void ProcessVertexData_Rogue_Squadron(uint32 dwXYZAddr, uint32 dwColorAddr, uint32 dwXYZCmd, uint32 dwColorCmd);

void DLParser_RS_Color_Buffer(Gfx *gfx)
{
	
	

	uint32 dwPC = gDlistStack[gDlistStackPointer].pc-8;
	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));

	if( dwAddr > g_dwRamSize )
	{
		dwAddr = (gfx->words.w1)&(g_dwRamSize-1);
	}

	Rogue_Squadron_Vtx_Color_Cmd = (gfx->words.w0);
	Rogue_Squadron_Vtx_Color_Addr = dwAddr;

	ProcessVertexData_Rogue_Squadron(Rogue_Squadron_Vtx_XYZ_Addr, Rogue_Squadron_Vtx_Color_Addr, Rogue_Squadron_Vtx_XYZ_Cmd, Rogue_Squadron_Vtx_Color_Cmd);

}


void DLParser_RS_Vtx_Buffer(Gfx *gfx)
{
	
	

	uint32 dwPC = gDlistStack[gDlistStackPointer].pc-8;
	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));
	if( dwAddr > g_dwRamSize )
	{
		dwAddr = (gfx->words.w1)&(g_dwRamSize-1);
	}

	Rogue_Squadron_Vtx_XYZ_Cmd = (gfx->words.w0);
	Rogue_Squadron_Vtx_XYZ_Addr = dwAddr;
}


void DLParser_RS_Block(Gfx *gfx)
{
	uint32 dwPC = gDlistStack[gDlistStackPointer].pc-8;
}

void DLParser_RS_MoveMem(Gfx *gfx)
{
	
	

	uint32 dwPC = gDlistStack[gDlistStackPointer].pc;
	uint32 cmd1 = ((dwPC)&0x00FFFFFF)|0x80000000;
	RSP_GBI1_MoveMem(gfx);
}

void DLParser_RS_0xbe(Gfx *gfx)
{
	uint32 dwPC = gDlistStack[gDlistStackPointer].pc-8;
	dwPC+=8;
	uint32 dwCmd2 = *(uint32 *)(g_pRDRAMu8 + dwPC);
	uint32 dwCmd3 = *(uint32 *)(g_pRDRAMu8 + dwPC+4);
	gDlistStack[gDlistStackPointer].pc += 8;

}


void DLParser_Ucode8_EndDL(Gfx *gfx)
{
	
	

	uint32 dwPC = gDlistStack[gDlistStackPointer].pc-8;
	RDP_GFX_PopDL();
}

void DLParser_Ucode8_DL(Gfx *gfx)	// DL Function Call
{

	

	uint32 dwPC = gDlistStack[gDlistStackPointer].pc-8;

	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));
	uint32 dwCmd2 = *(uint32 *)(g_pRDRAMu8 + dwAddr);
	uint32 dwCmd3 = *(uint32 *)(g_pRDRAMu8 + dwAddr+4);

	if( dwAddr > g_dwRamSize )
	{
		dwAddr = (gfx->words.w1)&(g_dwRamSize-1);
	}

	if( gDlistStackPointer < MAX_DL_STACK_SIZE-1 )
	{
		gDlistStackPointer++;
		gDlistStack[gDlistStackPointer].pc = dwAddr+16;
		gDlistStack[gDlistStackPointer].countdown = MAX_DL_COUNT;
	}
	else
	{
		RDP_GFX_PopDL();
	}

	GSBlkAddrSaves[gDlistStackPointer][0]=GSBlkAddrSaves[gDlistStackPointer][1]=0;
	if( (dwCmd2>>24) == 0x80 )
	{
		GSBlkAddrSaves[gDlistStackPointer][0] = dwCmd2;
		GSBlkAddrSaves[gDlistStackPointer][1] = dwCmd3;
	}

}

void DLParser_Ucode8_JUMP(Gfx *gfx)	// DL Function Call
{
	
	

	if( ((gfx->words.w0)&0x00FFFFFF) == 0 )
	{
		uint32 dwPC = gDlistStack[gDlistStackPointer].pc-8;
		uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));

		if( dwAddr > g_dwRamSize )
		{
			dwAddr = (gfx->words.w1)&(g_dwRamSize-1);
		}

		uint32 dwCmd2 = *(uint32 *)(g_pRDRAMu8 + dwAddr);
		uint32 dwCmd3 = *(uint32 *)(g_pRDRAMu8 + dwAddr+4);

		gDlistStack[gDlistStackPointer].pc = dwAddr+8;		// Jump to new address
	}
	else
	{
		uint32 dwPC = gDlistStack[gDlistStackPointer].pc-8;
	}
}



void DLParser_Ucode8_Unknown(Gfx *gfx)
{
	uint32 dwPC = gDlistStack[gDlistStackPointer].pc-8;
}

void DLParser_Unknown_Skip1(Gfx *gfx)
{
	uint32 dwPC = gDlistStack[gDlistStackPointer].pc-8;
	gfx++;
	dwPC+=8;
	gfx++;
	gDlistStack[gDlistStackPointer].pc += 8;
}

void DLParser_Unknown_Skip2(Gfx *gfx)
{
	uint32 dwPC = gDlistStack[gDlistStackPointer].pc-8;
	gfx++;
	dwPC+=8;
	gfx++;
	dwPC+=8;
	gfx++;
	gDlistStack[gDlistStackPointer].pc += 16;
}

void DLParser_Unknown_Skip3(Gfx *gfx)
{
	uint32 dwPC = gDlistStack[gDlistStackPointer].pc-8;
	gfx++;
	dwPC+=8;
	gfx++;
	dwPC+=8;
	gfx++;
	dwPC+=8;
	gfx++;
	gDlistStack[gDlistStackPointer].pc += 24;
}

void DLParser_Unknown_Skip4(Gfx *gfx)
{
	uint32 dwPC = gDlistStack[gDlistStackPointer].pc-8;
	gfx++;
	dwPC+=8;
	gfx++;
	dwPC+=8;
	gfx++;
	dwPC+=8;
	gfx++;
	dwPC+=8;
	gfx++;
	gDlistStack[gDlistStackPointer].pc += 32;
}

void DLParser_Ucode8_0x05(Gfx *gfx)
{
	// Be careful, 0x05 is variable length ucode
	/*
	0028E4E0: 05020088, 04D0000F - Reserved1
	0028E4E8: 6BDC0306, 00000000 - G_NOTHING
	0028E4F0: 05010130, 01B0000F - Reserved1
	0028E4F8: 918A01CA, 1EC5FF3B - G_NOTHING
	0028E500: 05088C68, F5021809 - Reserved1
	0028E508: 04000405, 00000000 - RSP_VTX
	0028E510: 102ECE60, 202F2AA0 - G_NOTHING
	0028E518: 05088C90, F5021609 - Reserved1
	0028E520: 04050405, F0F0F0F0 - RSP_VTX
	0028E528: 102ED0C0, 202F2D00 - G_NOTHING
	0028E530: B5000000, 00000000 - RSP_LINE3D
	0028E538: 8028E640, 8028E430 - G_NOTHING
	0028E540: 00000000, 00000000 - RSP_SPNOOP
	*/

	if( (gfx->words.w1) == 0 )
	{
		return;
	}
	else
	{
		DLParser_Unknown_Skip4(gfx);
	}
}

void DLParser_Ucode8_0xb4(Gfx *gfx)
{
	uint32 dwPC = gDlistStack[gDlistStackPointer].pc;

	if( ((gfx->words.w0)&0xFF) == 0x06 )
		DLParser_Unknown_Skip3(gfx);
	else if( ((gfx->words.w0)&0xFF) == 0x04 )
		DLParser_Unknown_Skip1(gfx);
	else if( ((gfx->words.w0)&0xFFF) == 0x600 )
		DLParser_Unknown_Skip3(gfx);
	else
	{
		DLParser_Unknown_Skip3(gfx);
	}
}

void DLParser_Ucode8_0xb5(Gfx *gfx)
{
	uint32 dwPC = gDlistStack[gDlistStackPointer].pc-8;

	uint32 dwCmd2 = *(uint32 *)(g_pRDRAMu8 + dwPC+8);
	uint32 dwCmd3 = *(uint32 *)(g_pRDRAMu8 + dwPC+12);

	//if( dwCmd2 == 0 && dwCmd3 == 0 )
	{
		DLParser_Ucode8_EndDL(gfx);	// Check me
		return;
	}

	gDlistStack[gDlistStackPointer].pc += 8;
	return;


	if( GSBlkAddrSaves[gDlistStackPointer][0] == 0 || GSBlkAddrSaves[gDlistStackPointer][1] == 0 )
	{
		DLParser_Ucode8_EndDL(gfx);	// Check me
		return;
	}

	if( ((dwCmd2>>24)!=0x80 && (dwCmd2>>24)!=0x00 ) || ((dwCmd3>>24)!=0x80 && (dwCmd3>>24)!=0x00 ) )
	{
		DLParser_Ucode8_EndDL(gfx);	// Check me
		return;
	}

	if( (dwCmd2>>24)!= (dwCmd3>>24) )
	{
		DLParser_Ucode8_EndDL(gfx);	// Check me
		return;
	}


	if( (dwCmd2>>24)==0x80 && (dwCmd3>>24)==0x80 )
	{
		if( dwCmd2 < dwCmd3  )
		{
			// All right, the next block is not ucode, but data
			DLParser_Ucode8_EndDL(gfx);	// Check me
			return;
		}

		uint32 dwCmd4 = *(uint32 *)(g_pRDRAMu8 + (dwCmd2&0x00FFFFFF));
		uint32 dwCmd5 = *(uint32 *)(g_pRDRAMu8 + (dwCmd2&0x00FFFFFF)+4);
		uint32 dwCmd6 = *(uint32 *)(g_pRDRAMu8 + (dwCmd3&0x00FFFFFF));
		uint32 dwCmd7 = *(uint32 *)(g_pRDRAMu8 + (dwCmd3&0x00FFFFFF)+4);
		if( (dwCmd4>>24) != 0x80 || (dwCmd5>>24) != 0x80 || (dwCmd6>>24) != 0x80 || (dwCmd7>>24) != 0x80 || dwCmd4 < dwCmd5 || dwCmd6 < dwCmd7 )
		{
			// All right, the next block is not ucode, but data
			DLParser_Ucode8_EndDL(gfx);	// Check me
			return;
		}

		gDlistStack[gDlistStackPointer].pc += 8;
		return;
	}
	else if( (dwCmd2>>24)==0x00 && (dwCmd3>>24)==0x00 )
	{
		DLParser_Ucode8_EndDL(gfx);	// Check me
		return;
	}
	else if( (dwCmd2>>24)==0x00 && (dwCmd3>>24)==0x00 )
	{
		dwCmd2 = *(uint32 *)(g_pRDRAMu8 + dwPC+16);
		dwCmd3 = *(uint32 *)(g_pRDRAMu8 + dwPC+20);
		if( (dwCmd2>>24)==0x80 && (dwCmd3>>24)==0x80 && dwCmd2 < dwCmd3 )
		{
			// All right, the next block is not ucode, but data
			DLParser_Ucode8_EndDL(gfx);	// Check me
			return;
		}
		else
		{
			gDlistStack[gDlistStackPointer].pc += 8;
			return;
		}
	}

	uint32 dwAddr1 = RSPSegmentAddr(dwCmd2);
	uint32 dwAddr2 = RSPSegmentAddr(dwCmd3);
	return;
}

void DLParser_Ucode8_0xbc(Gfx *gfx)
{
	
	

	if( ((gfx->words.w0)&0xFFF) == 0x58C )
	{
		DLParser_Ucode8_DL(gfx);
	}
	else
	{
		uint32 dwPC = gDlistStack[gDlistStackPointer].pc-8;
	}
}

void DLParser_Ucode8_0xbd(Gfx *gfx)
{
	
	

	/*
	00359A68: BD000000, DB5B0077 - RSP_POPMTX
	00359A70: C8C0A000, 00240024 - RDP_TriFill
	00359A78: 01000100, 00000000 - RSP_MTX
	00359A80: BD000501, DB5B0077 - RSP_POPMTX
	00359A88: C8C0A000, 00240024 - RDP_TriFill
	00359A90: 01000100, 00000000 - RSP_MTX
	00359A98: BD000A02, DB5B0077 - RSP_POPMTX
	00359AA0: C8C0A000, 00240024 - RDP_TriFill
	00359AA8: 01000100, 00000000 - RSP_MTX
	00359AB0: BD000F04, EB6F0087 - RSP_POPMTX
	00359AB8: C8C0A000, 00280028 - RDP_TriFill
	00359AC0: 01000100, 00000000 - RSP_MTX
	00359AC8: BD001403, DB5B0077 - RSP_POPMTX
	00359AD0: C8C0A000, 00240024 - RDP_TriFill
	00359AD8: 01000100, 00000000 - RSP_MTX
	00359AE0: B5000000, 00000000 - RSP_LINE3D
	00359AE8: 1A000000, 16000200 - G_NOTHING
	 */

	if( (gfx->words.w1) != 0 )
	{
		DLParser_Unknown_Skip2(gfx);
		return;
	}

	uint32 dwPC = gDlistStack[gDlistStackPointer].pc;
}

void DLParser_Ucode8_0xbf(Gfx *gfx)
{
	
	

	if( ((gfx->words.w0)&0xFF) == 0x02 )
		DLParser_Unknown_Skip3(gfx);
	else
		DLParser_Unknown_Skip1(gfx);
}

void PD_LoadMatrix_0xb4(uint32 addr)
{
	int i, j;

	uint32 data[16];
	data[0] =  *(uint32*)(g_pRDRAMu8+addr+4+ 0);
	data[1] =  *(uint32*)(g_pRDRAMu8+addr+4+ 8);
	data[2] =  *(uint32*)(g_pRDRAMu8+addr+4+16);
	data[3] =  *(uint32*)(g_pRDRAMu8+addr+4+24);

	data[8] =  *(uint32*)(g_pRDRAMu8+addr+4+32);
	data[9] =  *(uint32*)(g_pRDRAMu8+addr+4+40);
	data[10] = *(uint32*)(g_pRDRAMu8+addr+4+48);
	data[11] = *(uint32*)(g_pRDRAMu8+addr+4+56);

	data[4] =  *(uint32*)(g_pRDRAMu8+addr+4+ 0+64);
	data[5] =  *(uint32*)(g_pRDRAMu8+addr+4+ 8+64);
	data[6] =  *(uint32*)(g_pRDRAMu8+addr+4+16+64);
	data[7] =  *(uint32*)(g_pRDRAMu8+addr+4+24+64);

	data[12] = *(uint32*)(g_pRDRAMu8+addr+4+32+64);
	data[13] = *(uint32*)(g_pRDRAMu8+addr+4+40+64);
	data[14] = *(uint32*)(g_pRDRAMu8+addr+4+48+64);
	data[15] = *(uint32*)(g_pRDRAMu8+addr+4+56+64);


	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++) 
		{
			int     hi = *(short *)((BYTE*)data + (((i<<3)+(j<<1)     )^0x2));
			uint16  lo = *(uint16*)((BYTE*)data + (((i<<3)+(j<<1) + 32)^0x2));
			matToLoad.m[i][j] = (float)((hi<<16) | (lo))/ 65536.0f;
		}
	}
}	

void DLParser_RDPHalf_1_0xb4_GoldenEye(Gfx *gfx)		
{
	SP_Timing(RSP_GBI1_RDPHalf_1);
	if( ((gfx->words.w1)>>24) == 0xce )
	{
		PrepareTextures();
		CRender::g_pRender->SetCombinerAndBlender();

		uint32 dwPC = gDlistStack[gDlistStackPointer].pc;		// This points to the next instruction

		//PD_LoadMatrix_0xb4(dwPC + 8*16 - 8);

		uint32 dw1 = *(uint32 *)(g_pRDRAMu8 + dwPC+8*0+4);
		uint32 dw2 = *(uint32 *)(g_pRDRAMu8 + dwPC+8*1+4);
		uint32 dw3 = *(uint32 *)(g_pRDRAMu8 + dwPC+8*2+4);
		uint32 dw4 = *(uint32 *)(g_pRDRAMu8 + dwPC+8*3+4);
		uint32 dw5 = *(uint32 *)(g_pRDRAMu8 + dwPC+8*4+4);
		uint32 dw6 = *(uint32 *)(g_pRDRAMu8 + dwPC+8*5+4);
		uint32 dw7 = *(uint32 *)(g_pRDRAMu8 + dwPC+8*6+4);
		uint32 dw8 = *(uint32 *)(g_pRDRAMu8 + dwPC+8*7+4);
		uint32 dw9 = *(uint32 *)(g_pRDRAMu8 + dwPC+8*8+4);

		uint32 r = (dw8>>16)&0xFF;
		uint32 g = (dw8    )&0xFF;
		uint32 b = (dw9>>16)&0xFF;
		uint32 a = (dw9    )&0xFF;
		uint32 color = COLOR_RGBA(r, g, b, a);

		//int x0 = 0;
		//int x1 = gRDP.scissor.right;
		int x0 = gRSP.nVPLeftN;
		int x1 = gRSP.nVPRightN;
		int y0 = int(dw1&0xFFFF)/4;
		int y1 = int(dw1>>16)/4;

		float xscale = g_textures[0].m_pCTexture->m_dwWidth / (float)(x1-x0);
		float yscale = g_textures[0].m_pCTexture->m_dwHeight / (float)(y1-y0);
		float fs0 = (short)(dw3&0xFFFF)/32768.0f*g_textures[0].m_pCTexture->m_dwWidth;
		float ft0 = (short)(dw3>>16)/32768.0f*256;
		CRender::g_pRender->TexRect(x0,y0,x1,y1,0,0,xscale,yscale,true,color);

		gDlistStack[gDlistStackPointer].pc += 312;


	}
}

void DLParser_RSP_DL_WorldDriver(Gfx *gfx)
{
	uint32 dwAddr = RSPSegmentAddr((gfx->words.w1));
	if( dwAddr > g_dwRamSize )
	{
		dwAddr &= (g_dwRamSize-1);
	}

	gDlistStackPointer++;
	gDlistStack[gDlistStackPointer].pc = dwAddr;
	gDlistStack[gDlistStackPointer].countdown = MAX_DL_COUNT;
}

void DLParser_RSP_Pop_DL_WorldDriver(Gfx *gfx)
{
	
	

	RDP_GFX_PopDL();
}

void DLParser_RSP_Last_Legion_0x80(Gfx *gfx)
{
	
	

	gDlistStack[gDlistStackPointer].pc += 16;
}

void DLParser_RSP_Last_Legion_0x00(Gfx *gfx)
{
	gDlistStack[gDlistStackPointer].pc += 16;

	if( (gfx->words.w0) == 0 && (gfx->words.w1) )
	{
		uint32 newaddr = RSPSegmentAddr((gfx->words.w1));
		if( newaddr >= g_dwRamSize )
		{
			RDP_GFX_PopDL();
			return;
		}

		uint32 dw1 = *(uint32 *)(g_pRDRAMu8 + newaddr+8*0+4);
		uint32 pc1 = *(uint32 *)(g_pRDRAMu8 + newaddr+8*1+4);
		uint32 pc2 = *(uint32 *)(g_pRDRAMu8 + newaddr+8*4+4);
		pc1 = RSPSegmentAddr(pc1);
		pc2 = RSPSegmentAddr(pc2);

		if( pc1 && pc1 != 0xffffff && pc1 < g_dwRamSize)
		{
			// Need to call both DL
			gDlistStackPointer++;
			gDlistStack[gDlistStackPointer].pc = pc1;
			gDlistStack[gDlistStackPointer].countdown = MAX_DL_COUNT;
		}

		if( pc2 && pc2 != 0xffffff && pc2 < g_dwRamSize )
		{
			gDlistStackPointer++;
			gDlistStack[gDlistStackPointer].pc = pc2;
			gDlistStack[gDlistStackPointer].countdown = MAX_DL_COUNT;
		}
	}
	else if( (gfx->words.w1) == 0 )
	{
		RDP_GFX_PopDL();
	}
	else
	{
		// (gfx->words.w0) != 0
		RSP_RDP_Nothing(gfx);
		RDP_GFX_PopDL();
	}
}

void DLParser_TexRect_Last_Legion(Gfx *gfx)
{
	if( !status.bCIBufferIsRendered ) CGraphicsContext::g_pGraphicsContext->FirstDrawToNewCI();

	status.primitiveType = PRIM_TEXTRECT;

	// This command used 128bits, and not 64 bits. This means that we have to look one 
	// Command ahead in the buffer, and update the PC.
	uint32 dwPC = gDlistStack[gDlistStackPointer].pc;		// This points to the next instruction
	uint32 dwCmd2 = *(uint32 *)(g_pRDRAMu8 + dwPC);
	uint32 dwCmd3 = *(uint32 *)(g_pRDRAMu8 + dwPC+4);

	gDlistStack[gDlistStackPointer].pc += 8;


	uint32 dwXH		= (((gfx->words.w0)>>12)&0x0FFF)/4;
	uint32 dwYH		= (((gfx->words.w0)    )&0x0FFF)/4;
	uint32 tileno	= ((gfx->words.w1)>>24)&0x07;
	uint32 dwXL		= (((gfx->words.w1)>>12)&0x0FFF)/4;
	uint32 dwYL		= (((gfx->words.w1)    )&0x0FFF)/4;


	if( (int)dwXL >= gRDP.scissor.right || (int)dwYL >= gRDP.scissor.bottom || (int)dwXH < gRDP.scissor.left || (int)dwYH < gRDP.scissor.top )
	{
		// Clipping
		return;
	}

	uint16 uS		= (uint16)(  dwCmd2>>16)&0xFFFF;
	uint16 uT		= (uint16)(  dwCmd2    )&0xFFFF;
	short s16S = *(short*)(&uS);
	short s16T = *(short*)(&uT);

	uint16  uDSDX 	= (uint16)((  dwCmd3>>16)&0xFFFF);
	uint16  uDTDY	    = (uint16)((  dwCmd3    )&0xFFFF);
	short	 s16DSDX  = *(short*)(&uDSDX);
	short  s16DTDY	= *(short*)(&uDTDY);

	uint32 curTile = gRSP.curTile;
	ForceMainTextureIndex(tileno);

	float fS0 = s16S / 32.0f;
	float fT0 = s16T / 32.0f;

	float fDSDX = s16DSDX / 1024.0f;
	float fDTDY = s16DTDY / 1024.0f;

	uint32 cycletype = gRDP.otherMode.cycle_type;

	if (cycletype == CYCLE_TYPE_COPY)
	{
		fDSDX /= 4.0f;	// In copy mode 4 pixels are copied at once.
		dwXH++;
		dwYH++;
	}
	else if (cycletype == CYCLE_TYPE_FILL)
	{
		dwXH++;
		dwYH++;
	}

	if( fDSDX == 0 )	fDSDX = 1;
	if( fDTDY == 0 )	fDTDY = 1;

	float fS1 = fS0 + (fDSDX * (dwXH - dwXL));
	float fT1 = fT0 + (fDTDY * (dwYH - dwYL));

	float t0u0 = (fS0-gRDP.tiles[tileno].hilite_sl) * gRDP.tiles[tileno].fShiftScaleS;
	float t0v0 = (fT0-gRDP.tiles[tileno].hilite_tl) * gRDP.tiles[tileno].fShiftScaleT;
	float t0u1 = t0u0 + (fDSDX * (dwXH - dwXL))*gRDP.tiles[tileno].fShiftScaleS;
	float t0v1 = t0v0 + (fDTDY * (dwYH - dwYL))*gRDP.tiles[tileno].fShiftScaleT;

	if( dwXL==0 && dwYL==0 && dwXH==windowSetting.fViWidth-1 && dwYH==windowSetting.fViHeight-1 &&
		t0u0 == 0 && t0v0==0 && t0u1==0 && t0v1==0 )
	{
		//Using TextRect to clear the screen
	}
	else
	{
		if( status.bHandleN64TextureBuffer && //status.bDirectWriteIntoRDRAM && 
			g_pTextureBufferInfo->CI_Info.dwFormat == gRDP.tiles[tileno].dwFormat && 
			g_pTextureBufferInfo->CI_Info.dwSize == gRDP.tiles[tileno].dwSize && 
			gRDP.tiles[tileno].dwFormat == TXT_FMT_CI && gRDP.tiles[tileno].dwSize == TXT_SIZE_8b )
		{
			if( options.enableHackForGames == HACK_FOR_YOSHI )
			{
				// Hack for Yoshi background image
				PrepareTextures();
				TexRectToFrameBuffer_8b(dwXL, dwYL, dwXH, dwYH, t0u0, t0v0, t0u1, t0v1, tileno);
			}
			else
			{
				if( frameBufferOptions.bUpdateCIInfo )
				{
					PrepareTextures();
					TexRectToFrameBuffer_8b(dwXL, dwYL, dwXH, dwYH, t0u0, t0v0, t0u1, t0v1, tileno);
				}

				if( !status.bDirectWriteIntoRDRAM )
				{
					CRender::g_pRender->TexRect(dwXL, dwYL, dwXH, dwYH, fS0, fT0, fDSDX, fDTDY);

					status.dwNumTrisRendered += 2;
				}
			}
		}
		else
		{
			CRender::g_pRender->TexRect(dwXL, dwYL, dwXH, dwYH, fS0, fT0, fDSDX, fDTDY);
			status.bFrameBufferDrawnByTriangles = true;

			status.dwNumTrisRendered += 2;
		}
	}

	if( status.bHandleN64TextureBuffer )	g_pTextureBufferInfo->maxUsedHeight = max(g_pTextureBufferInfo->maxUsedHeight,(int)dwYH);

	ForceMainTextureIndex(curTile);
}