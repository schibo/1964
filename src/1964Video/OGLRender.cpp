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
#include "glh_genext.h"

// Fix me, use OGL internal L/T and matrix stack
// Fix me, use OGL lookupAt function
// Fix me, use OGL DisplayList

UVFlagMap OGLXUVFlagMaps[] =
{
	TEXTURE_UV_FLAG_WRAP,	GL_REPEAT,
	TEXTURE_UV_FLAG_MIRROR,	GL_REPEAT,		// Fix me, OGL does not have MIRROR
	TEXTURE_UV_FLAG_CLAMP,	GL_CLAMP,
};

//===================================================================
OGLRender::OGLRender()
{
	COGLGraphicsContext *pcontext = (COGLGraphicsContext *)(CGraphicsContext::g_pGraphicsContext);
	m_bSupportFogCoordExt = pcontext->m_bSupportFogCoord;
	m_bMultiTexture = pcontext->m_bSupportMultiTexture;
	m_bSupportClampToEdge = false;
	m_bClampS[0] = m_bClampS[0] = false;
	m_bClampT[0] = m_bClampT[1] = false;
	for( int i=0; i<8; i++ )
	{
		m_curBoundTex[i]=0;
		m_texUnitEnabled[i]=FALSE;
	}
	m_bEnableMultiTexture = false;
}

OGLRender::~OGLRender()
{
	ClearDeviceObjects();
}

bool OGLRender::InitDeviceObjects()
{
	return true;
}

bool OGLRender::ClearDeviceObjects()
{
	return true;
}

void OGLRender::Initialize(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewportWrapper(0, windowSetting.statusBarHeightToUse, windowSetting.uDisplayWidth, windowSetting.uDisplayHeight);

	COGLGraphicsContext *pcontext = (COGLGraphicsContext *)(CGraphicsContext::g_pGraphicsContext);
	if( pcontext->IsExtensionSupported("GL_IBM_texture_mirrored_repeat") )
	{
		OGLXUVFlagMaps[TEXTURE_UV_FLAG_MIRROR].realFlag = GL_MIRRORED_REPEAT_IBM;
	}
	else
	{
		OGLXUVFlagMaps[TEXTURE_UV_FLAG_MIRROR].realFlag = GL_REPEAT;
	}

	if( pcontext->IsExtensionSupported("GL_ARB_texture_border_clamp") || pcontext->IsExtensionSupported("GL_EXT_texture_edge_clamp") )
	{
		m_bSupportClampToEdge = true;
		OGLXUVFlagMaps[TEXTURE_UV_FLAG_CLAMP].realFlag = GL_CLAMP_TO_EDGE;
	}
	else
	{
		m_bSupportClampToEdge = false;
		OGLXUVFlagMaps[TEXTURE_UV_FLAG_CLAMP].realFlag = GL_CLAMP;
	}

	glVertexPointer( 4, GL_FLOAT, sizeof(float)*5, &(g_vtxProjected5[0][0]) );
	glEnableClientState( GL_VERTEX_ARRAY );

	if( m_bMultiTexture )
	{
		glClientActiveTextureARB( GL_TEXTURE0_ARB );
		glTexCoordPointer( 2, GL_FLOAT, sizeof( TLITVERTEX ), &(g_vtxBuffer[0].tcord[0].u) );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );

		glClientActiveTextureARB( GL_TEXTURE1_ARB );
		glTexCoordPointer( 2, GL_FLOAT, sizeof( TLITVERTEX ), &(g_vtxBuffer[0].tcord[1].u) );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	}
	else
	{
		glTexCoordPointer( 2, GL_FLOAT, sizeof( TLITVERTEX ), &(g_vtxBuffer[0].tcord[0].u) );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	}

	if (m_bSupportFogCoordExt)
	{
		glFogCoordPointerEXT( GL_FLOAT, sizeof(float)*5, &(g_vtxProjected5[0][4]) );
		glEnableClientState( GL_FOG_COORDINATE_ARRAY_EXT );
		glFogi( GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT );
		glFogi(GL_FOG_MODE, GL_LINEAR); // Fog Mode
		glFogf(GL_FOG_DENSITY, 1.0f); // How Dense Will The Fog Be
		glHint(GL_FOG_HINT, GL_FASTEST); // Fog Hint Value
		glFogi( GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT );
		glFogf( GL_FOG_START, 0.0f );
		glFogf( GL_FOG_END, 1.0f );
	}

	//glColorPointer( 1, GL_UNSIGNED_BYTE, sizeof(TLITVERTEX), &g_vtxBuffer[0].r);
	glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof(uint8)*4, &(g_oglVtxColors[0][0]) );
	glEnableClientState( GL_COLOR_ARRAY );
}
//===================================================================
TextureFilterMap OglTexFilterMap[2]=
{
	{FILTER_POINT, GL_NEAREST},
	{FILTER_LINEAR, GL_LINEAR},
};

void OGLRender::ApplyTextureFilter()
{
	static uint32 minflag=0xFFFF, magflag=0xFFFF;
	static uint32 mtex;

	if( m_texUnitEnabled[0] )
	{
		if( mtex != m_curBoundTex[0] )
		{
			mtex = m_curBoundTex[0];
			minflag = m_dwMinFilter;
			magflag = m_dwMagFilter;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, OglTexFilterMap[m_dwMinFilter].realFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, OglTexFilterMap[m_dwMagFilter].realFilter);
		}
		else
		{
			if( minflag != m_dwMinFilter )
			{
				minflag = m_dwMinFilter;
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, OglTexFilterMap[m_dwMinFilter].realFilter);
			}
			if( magflag != m_dwMagFilter )
			{
				magflag = m_dwMagFilter;
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, OglTexFilterMap[m_dwMagFilter].realFilter);
			}	
		}
	}
}

void OGLRender::SetShadeMode(RenderShadeMode mode)
{
	if( mode == SHADE_SMOOTH )
		glShadeModel(GL_SMOOTH);
	else
		glShadeModel(GL_FLAT);
}

void OGLRender::ZBufferEnable(BOOL bZBuffer)
{
	gRSP.bZBufferEnabled = bZBuffer;
	if( g_curRomInfo.bForceDepthBuffer )
		bZBuffer = TRUE;

	if( bZBuffer )
	{
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
	}
}

void OGLRender::ClearBuffer(bool cbuffer, bool zbuffer)
{
	uint32 flag=0;
	if( cbuffer )	flag |= GL_COLOR_BUFFER_BIT;
	if( zbuffer )	flag |= GL_DEPTH_BUFFER_BIT;
	float depth = ((gRDP.originalFillColor&0xFFFF)>>2)/(float)0x3FFF;
	glClearDepth(depth);
	glClear(flag);
}

void OGLRender::ClearZBuffer(float depth)
{
	uint32 flag=GL_DEPTH_BUFFER_BIT;
	glClearDepth(depth);
	glClear(flag);
}

void OGLRender::SetZCompare(BOOL bZCompare)
{
	if( g_curRomInfo.bForceDepthBuffer )
		bZCompare = TRUE;

	gRSP.bZBufferEnabled = bZCompare;
	if( bZCompare == TRUE )
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void OGLRender::SetZUpdate(BOOL bZUpdate)
{
	if( g_curRomInfo.bForceDepthBuffer )
		bZUpdate = TRUE;

	if( bZUpdate )
	{
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
	}
	else
	{
		glDepthMask(GL_FALSE);
	}
}

void OGLRender::SetZBias(int bias)
{
	if (m_dwZBias != bias)
	{
		if( bias != 0 )
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0, -40.0f);
		}
		else
		{
			glDisable(GL_POLYGON_OFFSET_FILL);
		}
		m_dwZBias = bias;
	}
}

void OGLRender::SetAlphaRef(uint32 dwAlpha)
{
	if (m_dwAlpha != dwAlpha)
	{
		m_dwAlpha = dwAlpha;
		glAlphaFunc(GL_GEQUAL, (float)dwAlpha);
	}
}

void OGLRender::ForceAlphaRef(uint32 dwAlpha)
{
	float ref = dwAlpha/255.0f;
	glAlphaFunc(GL_GEQUAL, ref);
}

void OGLRender::SetFillMode(FillMode mode)
{
	if( mode == RICE_FILLMODE_WINFRAME )
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OGLRender::SetCullMode(bool bCullFront, bool bCullBack)
{
	CRender::SetCullMode(bCullFront, bCullBack);
	if( bCullFront && bCullBack )
	{
        glCullFace(GL_FRONT_AND_BACK);
		glEnable(GL_CULL_FACE);
	}
	else if( bCullFront )
	{
        glCullFace(GL_FRONT);
		glEnable(GL_CULL_FACE);
	}
	else if( bCullBack )
	{
        glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

bool OGLRender::SetCurrentTexture(int tile, CTexture *handler,uint32 dwTileWidth, uint32 dwTileHeight, TxtrCacheEntry *pTextureEntry)
{
	RenderTexture &texture = g_textures[tile];
	texture.pTextureEntry = pTextureEntry;

	if( handler!= NULL  && texture.m_lpsTexturePtr != handler->GetTexture() )
	{
		texture.m_pCTexture = handler;
		texture.m_lpsTexturePtr = handler->GetTexture();

		texture.m_dwTileWidth = dwTileWidth;
		texture.m_dwTileHeight = dwTileHeight;

		texture.m_fTexWidth = (float)handler->m_dwCreatedTextureWidth;
		texture.m_fTexHeight = (float)handler->m_dwCreatedTextureHeight;
	}
	
	return true;
}

bool OGLRender::SetCurrentTexture(int tile, TxtrCacheEntry *pEntry)
{
	if (pEntry != NULL && pEntry->pTexture != NULL)
	{	
		SetCurrentTexture( tile, pEntry->pTexture,	pEntry->ti.WidthToCreate, pEntry->ti.HeightToCreate, pEntry);
		return true;
	}
	else
	{
		SetCurrentTexture( tile, NULL, 64, 64, NULL );
		return false;
	}
	return true;
}

void OGLRender::SetAddressUAllStages(uint32 dwTile, TextureUVFlag dwFlag)
{
	SetTextureUFlag(dwFlag, dwTile);
}

void OGLRender::SetAddressVAllStages(uint32 dwTile, TextureUVFlag dwFlag)
{
	SetTextureVFlag(dwFlag, dwTile);
}

void OGLRender::SetTexWrapS(int unitno,GLuint flag)
{
	static GLuint mflag;
	static GLuint mtex;
	if( m_curBoundTex[0] != mtex || mflag != flag )
	{
		mtex = m_curBoundTex[0];
		mflag = flag;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, flag);
	}
}
void OGLRender::SetTexWrapT(int unitno,GLuint flag)
{
	static GLuint mflag;
	static GLuint mtex;
	if( m_curBoundTex[0] != mtex || mflag != flag )
	{
		mtex = m_curBoundTex[0];
		mflag = flag;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, flag);
	}
}

void OGLRender::SetTextureUFlag(TextureUVFlag dwFlag, uint32 dwTile)
{
	TileUFlags[dwTile] = dwFlag;
	if( dwTile == gRSP.curTile )	// For basic OGL, only support the 1st texel
	{
		COGLTexture* pTexture = g_textures[gRSP.curTile].m_pCOGLTexture;
		if( pTexture )
		{
			EnableTexUnit(0,TRUE);
			BindTexture(pTexture->m_dwTextureName, 0);
		}
		SetTexWrapS(0, OGLXUVFlagMaps[dwFlag].realFlag);
		m_bClampS[0] = dwFlag==TEXTURE_UV_FLAG_CLAMP?true:false;
	}
}
void OGLRender::SetTextureVFlag(TextureUVFlag dwFlag, uint32 dwTile)
{
	TileVFlags[dwTile] = dwFlag;
	if( dwTile == gRSP.curTile )	// For basic OGL, only support the 1st texel
	{
		COGLTexture* pTexture = g_textures[gRSP.curTile].m_pCOGLTexture;
		if( pTexture ) 
		{
			EnableTexUnit(0,TRUE);
			BindTexture(pTexture->m_dwTextureName, 0);
		}
		SetTexWrapT(0, OGLXUVFlagMaps[dwFlag].realFlag);
		m_bClampT[0] = dwFlag==TEXTURE_UV_FLAG_CLAMP?true:false;
	}
}

// Basic render drawing functions

bool OGLRender::RenderTexRect()
{
	glViewportWrapper(0, windowSetting.statusBarHeightToUse, windowSetting.uDisplayWidth, windowSetting.uDisplayHeight);
	if( m_dwZBias!=0 )	glPolygonOffset(1.0, 0);

	GLboolean cullface = glIsEnabled(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

	glBegin(GL_TRIANGLE_FAN);

	float depth = -(m_texRectTVtx[3].z*2-1);

	glColor4f(m_texRectTVtx[3].r, m_texRectTVtx[3].g, m_texRectTVtx[3].b, m_texRectTVtx[3].a);
	TexCoord(m_texRectTVtx[3]);
	glVertex3f(m_texRectTVtx[3].x, m_texRectTVtx[3].y, depth);
	
	glColor4f(m_texRectTVtx[2].r, m_texRectTVtx[2].g, m_texRectTVtx[2].b, m_texRectTVtx[2].a);
	TexCoord(m_texRectTVtx[2]);
	glVertex3f(m_texRectTVtx[2].x, m_texRectTVtx[2].y, depth);

	glColor4f(m_texRectTVtx[1].r, m_texRectTVtx[1].g, m_texRectTVtx[1].b, m_texRectTVtx[1].a);
	TexCoord(m_texRectTVtx[1]);
	glVertex3f(m_texRectTVtx[1].x, m_texRectTVtx[1].y, depth);

	glColor4f(m_texRectTVtx[0].r, m_texRectTVtx[0].g, m_texRectTVtx[0].b, m_texRectTVtx[0].a);
	TexCoord(m_texRectTVtx[0]);
	glVertex3f(m_texRectTVtx[0].x, m_texRectTVtx[0].y, depth);

	glEnd();

	if( cullface ) glEnable(GL_CULL_FACE);

	return true;
}

bool OGLRender::RenderFillRect(uint32 dwColor, float depth)
{
	float a = (dwColor>>24)/255.0f;
	float r = ((dwColor>>16)&0xFF)/255.0f;
	float g = ((dwColor>>8)&0xFF)/255.0f;
	float b = (dwColor&0xFF)/255.0f;

	glViewportWrapper(0, windowSetting.statusBarHeightToUse, windowSetting.uDisplayWidth, windowSetting.uDisplayHeight);
	if( m_dwZBias!=0 )	glPolygonOffset(1.0, 0);

	GLboolean cullface = glIsEnabled(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

	glBegin(GL_TRIANGLE_FAN);
	glColor4f(r,g,b,a);
	glVertex4f(m_fillRectVtx[0].x, m_fillRectVtx[1].y, depth, 1);
	glVertex4f(m_fillRectVtx[1].x, m_fillRectVtx[1].y, depth, 1);
	glVertex4f(m_fillRectVtx[1].x, m_fillRectVtx[0].y, depth, 1);
	glVertex4f(m_fillRectVtx[0].x, m_fillRectVtx[0].y, depth, 1);
	glEnd();

	if( cullface ) glEnable(GL_CULL_FACE);

	return true;
}

bool OGLRender::RenderLine3D()
{
	if( m_dwZBias!=0 )	glPolygonOffset(1.0, 0);
	glBegin(GL_TRIANGLE_FAN);

	glColor4f(m_line3DVtx[1].r, m_line3DVtx[1].g, m_line3DVtx[1].b, m_line3DVtx[1].a);
	glVertex3f(m_line3DVector[3].x, m_line3DVector[3].y, -m_line3DVtx[3].z);
	glVertex3f(m_line3DVector[2].x, m_line3DVector[2].y, -m_line3DVtx[2].z);
	
	glColor4ub(m_line3DVtx[0].r, m_line3DVtx[0].g, m_line3DVtx[0].b, m_line3DVtx[0].a);
	glVertex3f(m_line3DVector[1].x, m_line3DVector[1].y, -m_line3DVtx[1].z);
	glVertex3f(m_line3DVector[0].x, m_line3DVector[0].y, -m_line3DVtx[0].z);

	glEnd();
	return true;
}

extern FiddledVtx * g_pVtxBase;

// This is so weired that I can not do vertex transform by myself. I have to use
// OpenGL internal transform
bool OGLRender::RenderFlushTris()
{
	if( !m_bSupportFogCoordExt )	
		SetFogFlagForNegativeW();
	else
	{
		if( !gRDP.bFogEnableInBlender && gRSP.bFogEnabled )
		{
			glDisable(GL_FOG);
		}
	}

	if( m_dwZBias!=0 )	glPolygonOffset(1.0, -60.0f*m_dwZBias);

	glViewportWrapper(windowSetting.vpLeftW, windowSetting.uDisplayHeight-windowSetting.vpTopW-windowSetting.vpHeightW+windowSetting.statusBarHeightToUse, windowSetting.vpWidthW, windowSetting.vpHeightW, false);
	glDrawElements( GL_TRIANGLES, gRSP.numVertices, GL_UNSIGNED_INT, g_vtxIndex );

	if( !m_bSupportFogCoordExt )	
		RestoreFogFlag();
	else
	{
		if( !gRDP.bFogEnableInBlender && gRSP.bFogEnabled )
		{
			glEnable(GL_FOG);
		}
	}
	return true;
}

void OGLRender::DrawSimple2DTexture(float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, D3DCOLOR dif, D3DCOLOR spe, float z, float rhw)
{
	if( status.bVIOriginIsUpdated == true && currentRomOptions.screenUpdateSetting==SCREEN_UPDATE_AT_1ST_PRIMITIVE )
	{
		status.bVIOriginIsUpdated=false;
		CGraphicsContext::Get()->UpdateFrame();
	}

	StartDrawSimple2DTexture(x0, y0, x1, y1, u0, v0, u1, v1, dif, spe, z, rhw);

	GLboolean cullface = glIsEnabled(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

	glViewportWrapper(0, windowSetting.statusBarHeightToUse, windowSetting.uDisplayWidth, windowSetting.uDisplayHeight);

	glBegin(GL_TRIANGLES);
	float a = (m_texRectTVtx[0].dcDiffuse >>24)/255.0f;
	float r = ((m_texRectTVtx[0].dcDiffuse>>16)&0xFF)/255.0f;
	float g = ((m_texRectTVtx[0].dcDiffuse>>8)&0xFF)/255.0f;
	float b = (m_texRectTVtx[0].dcDiffuse&0xFF)/255.0f;
	glColor4f(r,g,b,a);

	OGLRender::TexCoord(m_texRectTVtx[0]);
	glVertex3f(m_texRectTVtx[0].x, m_texRectTVtx[0].y, -m_texRectTVtx[0].z);

	OGLRender::TexCoord(m_texRectTVtx[1]);
	glVertex3f(m_texRectTVtx[1].x, m_texRectTVtx[1].y, -m_texRectTVtx[1].z);

	OGLRender::TexCoord(m_texRectTVtx[2]);
	glVertex3f(m_texRectTVtx[2].x, m_texRectTVtx[2].y, -m_texRectTVtx[2].z);

	OGLRender::TexCoord(m_texRectTVtx[0]);
	glVertex3f(m_texRectTVtx[0].x, m_texRectTVtx[0].y, -m_texRectTVtx[0].z);

	OGLRender::TexCoord(m_texRectTVtx[2]);
	glVertex3f(m_texRectTVtx[2].x, m_texRectTVtx[2].y, -m_texRectTVtx[2].z);

	OGLRender::TexCoord(m_texRectTVtx[3]);
	glVertex3f(m_texRectTVtx[3].x, m_texRectTVtx[3].y, -m_texRectTVtx[3].z);
	
	glEnd();

	if( cullface ) glEnable(GL_CULL_FACE);
}

void OGLRender::DrawSimpleRect(LONG nX0, LONG nY0, LONG nX1, LONG nY1, uint32 dwColor, float depth, float rhw)
{
	StartDrawSimpleRect(nX0, nY0, nX1, nY1, dwColor, depth, rhw);

	GLboolean cullface = glIsEnabled(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

	glBegin(GL_TRIANGLE_FAN);

	float a = (dwColor>>24)/255.0f;
	float r = ((dwColor>>16)&0xFF)/255.0f;
	float g = ((dwColor>>8)&0xFF)/255.0f;
	float b = (dwColor&0xFF)/255.0f;
	glColor4f(r,g,b,a);
	glVertex3f(m_simpleRectVtx[1].x, m_simpleRectVtx[0].y, -depth);
	glVertex3f(m_simpleRectVtx[1].x, m_simpleRectVtx[1].y, -depth);
	glVertex3f(m_simpleRectVtx[0].x, m_simpleRectVtx[1].y, -depth);
	glVertex3f(m_simpleRectVtx[0].x, m_simpleRectVtx[0].y, -depth);
	
	glEnd();

	if( cullface ) glEnable(GL_CULL_FACE);
}

void OGLRender::InitCombinerBlenderForSimpleRectDraw(uint32 tile)
{
	//glEnable(GL_CULL_FACE);
	EnableTexUnit(0,FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_ALPHA_TEST);
}

D3DCOLOR OGLRender::PostProcessDiffuseColor(D3DCOLOR curDiffuseColor)
{
	uint32 color = curDiffuseColor;
	uint32 colorflag = m_pColorCombiner->m_pDecodedMux->m_dwShadeColorChannelFlag;
	uint32 alphaflag = m_pColorCombiner->m_pDecodedMux->m_dwShadeAlphaChannelFlag;
	if( colorflag+alphaflag != MUX_0 )
	{
		if( (colorflag & 0xFFFFFF00) == 0 && (alphaflag & 0xFFFFFF00) == 0 )
		{
			color = (m_pColorCombiner->GetConstFactor(colorflag, alphaflag, curDiffuseColor));
		}
		else
			color = (CalculateConstFactor(colorflag, alphaflag, curDiffuseColor));
	}

	//return (color<<8)|(color>>24);
	return color;
}

D3DCOLOR OGLRender::PostProcessSpecularColor()
{
	return 0;
}

void OGLRender::SetViewportRender()
{
	glViewportWrapper(windowSetting.vpLeftW, windowSetting.uDisplayHeight-windowSetting.vpTopW-windowSetting.vpHeightW+windowSetting.statusBarHeightToUse, windowSetting.vpWidthW, windowSetting.vpHeightW);
}

void OGLRender::RenderReset()
{
	CRender::RenderReset();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, windowSetting.uDisplayWidth, windowSetting.uDisplayHeight, 0, -1, 1);

    // position viewer 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void OGLRender::SetAlphaTestEnable(BOOL bAlphaTestEnable)
{
	if( bAlphaTestEnable )
		glEnable(GL_ALPHA_TEST);
	else
		glDisable(GL_ALPHA_TEST);
}

void OGLRender::BindTexture(GLuint texture, int unitno)
{
	if( m_curBoundTex[0] != texture )
	{
		glBindTexture(GL_TEXTURE_2D,texture);
		m_curBoundTex[0] = texture;
	}
}

void OGLRender::DisBindTexture(GLuint texture, int unitno)
{
	//EnableTexUnit(0,FALSE);
	//glBindTexture(GL_TEXTURE_2D, 0);	//Not to bind any texture
}

void OGLRender::EnableTexUnit(int unitno, BOOL flag)
{
	if( m_texUnitEnabled[0] != flag )
	{
		m_texUnitEnabled[0] = flag;
		if( flag == TRUE )
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
	}
}

void OGLRender::TexCoord2f(float u, float v)
{
	glTexCoord2f(u, v);
}

void OGLRender::TexCoord(TLITVERTEX &vtxInfo)
{
	glTexCoord2f(vtxInfo.tcord[0].u, vtxInfo.tcord[0].v);
}

void OGLRender::UpdateScissor()
{
	if( options.bEnableHacks && g_CI.dwWidth == 0x200 && gRDP.scissor.right == 0x200 && g_CI.dwWidth>(*g_GraphicsInfo.VI_WIDTH_REG & 0xFFF) )
	{
		// Hack for RE2
		uint32 width = *g_GraphicsInfo.VI_WIDTH_REG & 0xFFF;
		uint32 height = (gRDP.scissor.right*gRDP.scissor.bottom)/width;
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, int(height*windowSetting.fMultY+windowSetting.statusBarHeightToUse),
			int(width*windowSetting.fMultX), int(height*windowSetting.fMultY) );
	}
	else
	{
		UpdateScissorWithClipRatio();
	}
}

void OGLRender::ApplyRDPScissor(bool force)
{
	if( !force && status.curScissor == RDP_SCISSOR )	return;

	if( options.bEnableHacks && g_CI.dwWidth == 0x200 && gRDP.scissor.right == 0x200 && g_CI.dwWidth>(*g_GraphicsInfo.VI_WIDTH_REG & 0xFFF) )
	{
		// Hack for RE2
		uint32 width = *g_GraphicsInfo.VI_WIDTH_REG & 0xFFF;
		uint32 height = (gRDP.scissor.right*gRDP.scissor.bottom)/width;
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, int(height*windowSetting.fMultY+windowSetting.statusBarHeightToUse),
			int(width*windowSetting.fMultX), int(height*windowSetting.fMultY) );
	}
	else
	{
		glScissor(int(gRDP.scissor.left*windowSetting.fMultX), int((windowSetting.uViHeight-gRDP.scissor.bottom)*windowSetting.fMultY+windowSetting.statusBarHeightToUse),
			int((gRDP.scissor.right-gRDP.scissor.left)*windowSetting.fMultX), int((gRDP.scissor.bottom-gRDP.scissor.top)*windowSetting.fMultY ));
	}

	status.curScissor = RDP_SCISSOR;
}

void OGLRender::ApplyScissorWithClipRatio(bool force)
{
	if( !force && status.curScissor == RSP_SCISSOR )	return;

	glEnable(GL_SCISSOR_TEST);
	glScissor(int(gRSP.real_clip_scissor_left*windowSetting.fMultX), int((windowSetting.uViHeight-gRSP.real_clip_scissor_bottom)*windowSetting.fMultY)+windowSetting.statusBarHeightToUse,
		int((gRSP.real_clip_scissor_right-gRSP.real_clip_scissor_left+1)*windowSetting.fMultX), 
		int((gRSP.real_clip_scissor_bottom-gRSP.real_clip_scissor_top+1)*windowSetting.fMultY));

	status.curScissor = RSP_SCISSOR;
}

void OGLRender::SetFogMinMax(float fMin, float fMax)
{
	glFogf(GL_FOG_START, gRSPfFogMin); // Fog Start Depth
	glFogf(GL_FOG_END, gRSPfFogMax); // Fog End Depth
}

void OGLRender::TurnFogOnOff(bool flag)
{
	if( flag )
		glEnable(GL_FOG);
	else
		glDisable(GL_FOG);
}

void OGLRender::SetFogEnable(bool bEnable)
{
	gRSP.bFogEnabled = bEnable&&options.bEnableFog;

	if( gRSP.bFogEnabled )
	{
		glFogfv(GL_FOG_COLOR, gRDP.fvFogColor); // Set Fog Color
		glFogf(GL_FOG_START, gRSPfFogMin); // Fog Start Depth
		glFogf(GL_FOG_END, gRSPfFogMax); // Fog End Depth
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
	}
}

void OGLRender::SetFogColor(uint32 r, uint32 g, uint32 b, uint32 a)
{
	gRDP.fogColor = COLOR_RGBA(r, g, b, a); 
	gRDP.fvFogColor[0] = r/255.0f;		//r
	gRDP.fvFogColor[1] = g/255.0f;		//g
	gRDP.fvFogColor[2] = b/255.0f;			//b
	gRDP.fvFogColor[3] = a/255.0f;		//a
	glFogfv(GL_FOG_COLOR, gRDP.fvFogColor); // Set Fog Color
}

void OGLRender::DisableMultiTexture()
{
	glActiveTextureARB(GL_TEXTURE1_ARB);
	EnableTexUnit(1,FALSE);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	EnableTexUnit(0,FALSE);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	EnableTexUnit(0,TRUE);
}

void OGLRender::EndRendering(void)
{
	glFlush();
	if( CRender::gRenderReferenceCount > 0 ) 
		CRender::gRenderReferenceCount--;
}

void OGLRender::glViewportWrapper(GLint x, GLint y, GLsizei width, GLsizei height, bool flag)
{
	static GLint mx=0,my=0;
	static GLsizei m_width=0, m_height=0;
	static bool mflag=true;

	if( x!=mx || y!=my || width!=m_width || height!=m_height || mflag!=flag)
	{
		mx=x;
		my=y;
		m_width=width;
		m_height=height;
		mflag=flag;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if( flag )	glOrtho(0, windowSetting.uDisplayWidth, windowSetting.uDisplayHeight, 0, -1, 1);
		glViewport(x,y,width,height);
	}
}

void OGLRender::CaptureScreen(char *filename)
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	HANDLE hBitmapFile;

	unsigned char *buffer = (unsigned char*)malloc( windowSetting.uDisplayWidth * windowSetting.uDisplayHeight * 3 );

	glReadBuffer( GL_FRONT );
	glReadPixels( 0, windowSetting.statusBarHeightToUse, windowSetting.uDisplayWidth, windowSetting.uDisplayHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer );
	glReadBuffer( GL_BACK );

	infoHeader.biSize = sizeof( BITMAPINFOHEADER );
	infoHeader.biWidth = windowSetting.uDisplayWidth;
	infoHeader.biHeight = windowSetting.uDisplayHeight;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 24;
	infoHeader.biCompression = BI_RGB;
	infoHeader.biSizeImage = windowSetting.uDisplayWidth * windowSetting.uDisplayHeight * 3;
	infoHeader.biXPelsPerMeter = 0;
	infoHeader.biYPelsPerMeter = 0;
	infoHeader.biClrUsed = 0;
	infoHeader.biClrImportant = 0;

	fileHeader.bfType = 19778;
	fileHeader.bfSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + infoHeader.biSizeImage;
	fileHeader.bfReserved1 = fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );


	uint32 res;
	hBitmapFile = CreateFile( filename, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hBitmapFile != INVALID_HANDLE_VALUE )
	{
		WriteFile( hBitmapFile, &fileHeader, sizeof( BITMAPFILEHEADER ), (DWORD*)&res, NULL );
		WriteFile( hBitmapFile, &infoHeader, sizeof( BITMAPINFOHEADER ), (DWORD*)&res, NULL );
		WriteFile( hBitmapFile, buffer, infoHeader.biSizeImage, (DWORD*)&res, NULL );

		CloseHandle( hBitmapFile );
	}
	else
	{
		// Do something
	}
	free( buffer );
}