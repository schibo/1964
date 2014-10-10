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

extern Matrix gD3DMtxReal;
extern uObjMtxReal gObjMtxReal;
extern Matrix gD3DObjOffset;

//========================================================================
void OGLRender::DrawText(const char* str, RECT *rect)
{
	return;

	// code below does not work

	glFlush();
	COGLGraphicsContext *pcontext = (COGLGraphicsContext*)(CGraphicsContext::g_pGraphicsContext);

	wglUseFontBitmaps(pcontext->m_hDC, 0, 256, 1000); 

	// move bottom left, southwest of the red triangle 
	glRasterPos2f(30.0F, 30.0F); 

	// set up for a string-drawing display list call 
	glListBase(1000); 

	// draw a string using font display lists 
	glCallLists(12, GL_UNSIGNED_BYTE, "Red Triangle"); 

}

void OGLRender::DrawSpriteR_Render()	// With Rotation
{
	glViewportWrapper(0, windowSetting.statusBarHeightToUse, windowSetting.uDisplayWidth, windowSetting.uDisplayHeight);

	GLboolean cullface = glIsEnabled(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

	glBegin(GL_TRIANGLES);
	glColor4fv(gRDP.fvPrimitiveColor);

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


void OGLRender::DrawObjBGCopy(uObjBg &info)
{
	if( IsUsedAsDI(g_CI.dwAddr) )
	{
#ifndef SUPPORT_LOCKABLE_ZBUFFER
#ifndef SUPPORT_ZBUFFER_IMG
		//g_pD3DDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0, 0);	//Check me
#else
		if( g_CI.dwWidth == 0x200 && info.imageFmt == g_CI.dwFormat && info.imageSiz == g_CI.dwSize &&
				info.frameW == 0x800 )
		{
			m_pColorCombiner->InitCombinerBlenderForSimpleTextureDraw(0);
			ZBufferEnable( TRUE );
			//SetColorArg1( 0, D3DTA_TFACTOR );
			//SetAlphaArg1( 0, D3DTA_TFACTOR );
			//SetTextureFactor(0);
			//SetTextureFactor(0x80808080);
			
			// Resident Evil2
			uint32 width = *g_GraphicsInfo.VI_WIDTH_REG & 0xFFF;
			uint32 height = (info.frameW/4*info.frameH/4)/width;
			uint32 pitch = width;

			//LoadObjBGCopy(info);

			// We don't support lockable Zbuffer, but we can simular it by splitting the image
			// to small pieces and render the piece with depth from the depth image

			uint16 *Base = (uint16*)(g_pRDRAMu8+RSPSegmentAddr(info.imagePtr));
			float depth;

			const uint32 inc=10;

			for( uint32 i=0; i<height; i+=inc )
			{
				uint16 *pSrc = Base + i * pitch;
				depth = (*pSrc + 1.0f ) / 0x10000;
				for( uint32 j=0; j<width; j+=inc)
				{
					DrawSimpleRect(j, i, j+inc, i+inc, gRDP.primitiveColor, depth, 1/depth);
				}
			}

			/*
			if (IsResultGood(pDepthBufferSurf->LockRect(&dlre, NULL, D3DLOCK_NOSYSLOCK)))
			{
				int x, y;
				for( int i=0; i<windowSetting.uDisplayHeight; i++ )
				{

					uint16 *pDst = (uint16*)((uint8 *)dlre.pBits + i * dlre.Pitch);

					for( int j=0; j<windowSetting.uDisplayWidth; j++ )
					{
						pDst[j] = pSrc[x];
					}
				}
				pDepthBufferSurf->UnlockRect();
			}
			*/


		}
#endif
#else
		/*
		if( g_pLockableBackBuffer == NULL && status.bHandleN64TextureBuffer == false )
		{
			if( IsResultGood(g_pD3DDev->CreateDepthStencilSurface(windowSetting.uDisplayWidth, windowSetting.uDisplayHeight, D3DFMT_D16_LOCKABLE, D3DMULTISAMPLE_NONE, &g_pLockableBackBuffer)) && g_pLockableBackBuffer )
			{
				g_pD3DDev->SetRenderTarget(NULL, g_pLockableBackBuffer);
				TRACE0("Created and use lockable depth buffer");
			}
			else
			{
				if( g_pLockableBackBuffer )
				{
					g_pLockableBackBuffer->Release();
					g_pLockableBackBuffer = NULL;
				}
				TRACE0("Can not create lockable depth buffer");
			}
		}

		// I can not finish this function because Z buffer is not lockable
		// and lockable zbuffer does not work
		LPDIRECT3DSURFACE8 pDepthBufferSurf = NULL;
		g_pD3DDev->GetDepthStencilSurface(&pDepthBufferSurf);

		D3DLOCKED_RECT dlre;
		ZeroMemory( &dlre, sizeof(D3DLOCKED_RECT) );
		//int pixSize = GetPixelSize();
		uint32 width = *g_GraphicsInfo.VI_WIDTH_REG & 0xFFF;
		uint32 height = (info.frameW/4*info.frameH/4)/width;
		uint32 pitch = width;

		if (IsResultGood(pDepthBufferSurf->LockRect(&dlre, NULL, D3DLOCK_NOSYSLOCK)))
		{
			uint16 *Base = (uint16*)(g_pRDRAMu8+RSPSegmentAddr(g_ZI.dwAddr));
			int x, y;
			for( int i=0; i<windowSetting.uDisplayHeight; i++ )
			{
				y = i*height/windowSetting.uDisplayHeight;

				uint16 *pSrc = Base + y * pitch;
				uint16 *pDst = (uint16*)((uint8 *)dlre.pBits + i * dlre.Pitch);

				for( int j=0; j<windowSetting.uDisplayWidth; j++ )
				{
					x = j*width/windowSetting.uDisplayWidth;
					pDst[j] = pSrc[x];
				}
			}
			pDepthBufferSurf->UnlockRect();
		}
		
		pDepthBufferSurf->Release();
		*/
#endif
		return;
	}
	else
	{
		CRender::LoadObjBGCopy(info);
		CRender::DrawObjBGCopy(info);
	}
}


