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

void COGLExtRender::Initialize(void)
{
	OGLRender::Initialize();

	// Initialize multitexture
	glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&m_maxTexUnits);

	for( int i=0; i<8; i++ )
		m_textureUnitMap[i] = -1;
	m_textureUnitMap[0] = 0;	// T0 is usually using texture unit 0
	m_textureUnitMap[1] = 1;	// T1 is usually using texture unit 1
}


void COGLExtRender::BindTexture(GLuint texture, int unitno)
{
	if( m_bEnableMultiTexture )
	{
		if( unitno < m_maxTexUnits )
		{
			if( m_curBoundTex[unitno] != texture )
			{
				glActiveTextureARB(GL_TEXTURE0_ARB+unitno);
				glBindTexture(GL_TEXTURE_2D,texture);
				m_curBoundTex[unitno] = texture;
			}
		}
	}
	else
	{
		OGLRender::BindTexture(texture, unitno);
	}
}

void COGLExtRender::DisBindTexture(GLuint texture, int unitno)
{
	if( m_bEnableMultiTexture )
	{
		glActiveTextureARB(GL_TEXTURE0_ARB+unitno);
		glBindTexture(GL_TEXTURE_2D, 0);	//Not to bind any texture
	}
	else
		OGLRender::DisBindTexture(texture, unitno);
}

void COGLExtRender::TexCoord2f(float u, float v)
{
	if( m_bEnableMultiTexture )
	{
		for( int i=0; i<8; i++ )
		{
			if( m_textureUnitMap[i] >= 0 )
			{
				glMultiTexCoord2fARB(GL_TEXTURE0_ARB+i, u, v);
			}
		}
	}
	else
		OGLRender::TexCoord2f(u,v);
}

void COGLExtRender::TexCoord(TLITVERTEX &vtxInfo)
{
	if( m_bEnableMultiTexture )
	{
		for( int i=0; i<8; i++ )
		{
			if( m_textureUnitMap[i] >= 0 )
			{
				glMultiTexCoord2fvARB(GL_TEXTURE0_ARB+i, &(vtxInfo.tcord[m_textureUnitMap[i]].u));
			}
		}
	}
	else
		OGLRender::TexCoord(vtxInfo);
}


void COGLExtRender::SetTexWrapS(int unitno,GLuint flag)
{
	static GLuint mflag[8];
	static GLuint mtex[8];
	if( m_curBoundTex[unitno] != mtex[unitno] || mflag[unitno] != flag )
	{
		mtex[unitno] = m_curBoundTex[0];
		mflag[unitno] = flag;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, flag);
	}
}
void COGLExtRender::SetTexWrapT(int unitno,GLuint flag)
{
	static GLuint mflag[8];
	static GLuint mtex[8];
	if( m_curBoundTex[unitno] != mtex[unitno] || mflag[unitno] != flag )
	{
		mtex[unitno] = m_curBoundTex[0];
		mflag[unitno] = flag;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, flag);
	}
}

extern UVFlagMap OGLXUVFlagMaps[];
void COGLExtRender::SetTextureUFlag(TextureUVFlag dwFlag, uint32 dwTile)
{
	TileUFlags[dwTile] = dwFlag;
	if( !m_bEnableMultiTexture )
	{
		OGLRender::SetTextureUFlag(dwFlag, dwTile);
		return;
	}

	int tex;
	if( dwTile == gRSP.curTile )
		tex=0;
	else if( dwTile == ((gRSP.curTile+1)&7) )
		tex=1;
	else
	{
		if( dwTile == ((gRSP.curTile+2)&7) )
			tex=2;
		else if( dwTile == ((gRSP.curTile+3)&7) )
			tex=3;
		else
		{
			return;
		}
	}

	for( int textureNo=0; textureNo<8; textureNo++)
	{
		if( m_textureUnitMap[textureNo] == tex )
		{
			glActiveTextureARB(GL_TEXTURE0_ARB+textureNo);
			COGLTexture* pTexture = g_textures[(gRSP.curTile+tex)&7].m_pCOGLTexture;
			if( pTexture ) 
			{
				EnableTexUnit(textureNo,TRUE);
				BindTexture(pTexture->m_dwTextureName, textureNo);
			}
			SetTexWrapS(textureNo, OGLXUVFlagMaps[dwFlag].realFlag);
			m_bClampS[textureNo] = dwFlag==TEXTURE_UV_FLAG_CLAMP?true:false;
		}
	}
}
void COGLExtRender::SetTextureVFlag(TextureUVFlag dwFlag, uint32 dwTile)
{
	TileVFlags[dwTile] = dwFlag;
	if( !m_bEnableMultiTexture )
	{
		OGLRender::SetTextureVFlag(dwFlag, dwTile);
		return;
	}

	int tex;
	if( dwTile == gRSP.curTile )
		tex=0;
	else if( dwTile == ((gRSP.curTile+1)&7) )
		tex=1;
	else
	{
		if( dwTile == ((gRSP.curTile+2)&7) )
			tex=2;
		else if( dwTile == ((gRSP.curTile+3)&7) )
			tex=3;
		else
		{
			return;
		}
	}
	
	for( int textureNo=0; textureNo<8; textureNo++)
	{
		if( m_textureUnitMap[textureNo] == tex )
		{
			COGLTexture* pTexture = g_textures[(gRSP.curTile+tex)&7].m_pCOGLTexture;
			if( pTexture )
			{
				EnableTexUnit(textureNo,TRUE);
				BindTexture(pTexture->m_dwTextureName, textureNo);
			}
			SetTexWrapT(textureNo, OGLXUVFlagMaps[dwFlag].realFlag);
			m_bClampT[textureNo] = dwFlag==TEXTURE_UV_FLAG_CLAMP?true:false;
		}
	}
}

void COGLExtRender::EnableTexUnit(int unitno, BOOL flag)
{
	if( m_texUnitEnabled[unitno] != flag )
	{
		m_texUnitEnabled[unitno] = flag;
		glActiveTextureARB(GL_TEXTURE0_ARB+unitno);
		if( flag == TRUE )
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
	}
}

void COGLExtRender::ApplyTextureFilter()
{
	static uint32 minflag[8], magflag[8];
	static uint32 mtex[8];
	for( int i=0; i<m_maxTexUnits; i++ )
	{
		if( m_texUnitEnabled[i] )
		{
			if( mtex[i] != m_curBoundTex[i] )
			{
				mtex[i] = m_curBoundTex[i];
				glActiveTextureARB(GL_TEXTURE0_ARB+i);
				minflag[i] = m_dwMinFilter;
				magflag[i] = m_dwMagFilter;
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_dwMinFilter);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_dwMagFilter);
			}
			else
			{
				if( minflag[i] != m_dwMinFilter )
				{
					minflag[i] = m_dwMinFilter;
					glActiveTextureARB(GL_TEXTURE0_ARB+i);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_dwMinFilter);
				}
				if( magflag[i] != m_dwMagFilter )
				{
					magflag[i] = m_dwMagFilter;
					glActiveTextureARB(GL_TEXTURE0_ARB+i);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_dwMagFilter);
				}
			}
		}
	}
}

void COGLExtRender::SetTextureToTextureUnitMap(int tex, int unit)
{
	if( unit < 8 && (tex >= -1 || tex <= 1))
		m_textureUnitMap[unit] = tex;
}
