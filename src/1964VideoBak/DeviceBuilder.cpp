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

//========================================================================
CDeviceBuilder* CDeviceBuilder::m_pInstance=NULL;
SupportedDeviceType CDeviceBuilder::m_deviceType = DIRECTX_DEVICE;
SupportedDeviceType CDeviceBuilder::m_deviceGeneralType = DIRECTX_DEVICE;

CDeviceBuilder* CDeviceBuilder::GetBuilder(void)
{
	if( m_pInstance == NULL )
		CreateBuilder(m_deviceType);
	
	return m_pInstance;
}

void CDeviceBuilder::SelectDeviceType(SupportedDeviceType type)
{
	if( type != m_deviceType && m_pInstance != NULL )
	{
		DeleteBuilder();
	}

	CDeviceBuilder::m_deviceType = type;
	switch(type)
	{
	case OGL_DEVICE:
	case OGL_1_1_DEVICE:
	case OGL_1_2_DEVICE:
	case OGL_1_3_DEVICE:
	case OGL_1_4_DEVICE:
	case OGL_1_4_V2_DEVICE:
	case OGL_TNT2_DEVICE:
	case NVIDIA_OGL_DEVICE:
		CDeviceBuilder::m_deviceGeneralType = OGL_DEVICE;
		break;
	case DIRECTX_DEVICE:
	case DIRECTX_9_DEVICE:
	case DIRECTX_8_DEVICE:
	case DIRECTX_7_DEVICE:
	case DIRECTX_6_DEVICE:
	case DIRECTX_5_DEVICE:
	case XBOX_DIRECTX_DEVICE:
		CDeviceBuilder::m_deviceGeneralType = DIRECTX_DEVICE;
		break;
	}
}

SupportedDeviceType CDeviceBuilder::GetDeviceType(void)
{
	return CDeviceBuilder::m_deviceType;
}

SupportedDeviceType CDeviceBuilder::GetGeneralDeviceType(void)
{
	return CDeviceBuilder::m_deviceGeneralType;
}

CDeviceBuilder* CDeviceBuilder::CreateBuilder(SupportedDeviceType type)
{
	if( m_pInstance == NULL )
	{
		switch( type )
		{
		case 	OGL_DEVICE:
		case 	OGL_1_1_DEVICE:
		case 	OGL_1_2_DEVICE:
		case 	OGL_1_3_DEVICE:
		case 	OGL_1_4_DEVICE:
		case 	OGL_1_4_V2_DEVICE:
		case 	OGL_TNT2_DEVICE:
		case 	NVIDIA_OGL_DEVICE:
			m_pInstance = new OGLDeviceBuilder();
			break;
		case 	DIRECTX_DEVICE:
		case 	DIRECTX_9_DEVICE:
		case 	DIRECTX_8_DEVICE:
		case 	DIRECTX_7_DEVICE:
		case 	DIRECTX_6_DEVICE:
		case 	DIRECTX_5_DEVICE:
		case 	XBOX_DIRECTX_DEVICE:
			m_pInstance = new DirectXDeviceBuilder();
			break;
		default:
			ErrorMsg("Error builder type");
			exit(1);
		}

		SAFE_CHECK(m_pInstance);
	}

	return m_pInstance;
}

void CDeviceBuilder::DeleteBuilder(void)
{
	delete m_pInstance;
	m_pInstance = NULL;
}

CDeviceBuilder::CDeviceBuilder() :
	m_pRender(NULL),
	m_pGraphicsContext(NULL),
	m_pColorCombiner(NULL),
	m_pAlphaBlender(NULL)
{
}

CDeviceBuilder::~CDeviceBuilder()
{
	DeleteGraphicsContext();
	DeleteRender();
	DeleteColorCombiner();
	DeleteAlphaBlender();
}

void CDeviceBuilder::DeleteGraphicsContext(void)
{
	if( m_pGraphicsContext != NULL )
	{
		delete m_pGraphicsContext;
		CGraphicsContext::g_pGraphicsContext = m_pGraphicsContext = NULL;
	}
}

void CDeviceBuilder::DeleteRender(void)
{
	if( m_pRender != NULL )
	{
		delete m_pRender;
		CRender::g_pRender = m_pRender = NULL;
		CRender::gRenderReferenceCount = 0;
	}
}

void CDeviceBuilder::DeleteColorCombiner(void)
{
	if( m_pColorCombiner != NULL )
	{
		delete m_pColorCombiner;
		m_pColorCombiner = NULL;
	}
}

void CDeviceBuilder::DeleteAlphaBlender(void)
{
	if( m_pAlphaBlender != NULL )
	{
		delete m_pAlphaBlender;
		m_pAlphaBlender = NULL;
	}
}


//========================================================================


CGraphicsContext * OGLDeviceBuilder::CreateGraphicsContext(void)
{
	if( g_GraphicsInfo.hStatusBar )
	{
		SetWindowText(g_GraphicsInfo.hStatusBar,"Creating OpenGL Device Context");
	}
	if( m_pGraphicsContext == NULL )
	{
		m_pGraphicsContext = new COGLGraphicsContext();
		SAFE_CHECK(m_pGraphicsContext);
		CGraphicsContext::g_pGraphicsContext = m_pGraphicsContext;
	}

	return m_pGraphicsContext;
}

CRender * OGLDeviceBuilder::CreateRender(void)
{
	if( m_pRender == NULL )
	{
		if( CGraphicsContext::g_pGraphicsContext == NULL && CGraphicsContext::g_pGraphicsContext->Ready() )
		{
			ErrorMsg("Can not create ColorCombiner before creating and initializing GraphicsContext");
			m_pRender = NULL;
			SAFE_CHECK(m_pRender);
		}

		COGLGraphicsContext &context = *((COGLGraphicsContext*)CGraphicsContext::g_pGraphicsContext);

		if( context.m_bSupportMultiTexture )
		{
			// OGL extension render
			m_pRender = new COGLExtRender();
		}
		else
		{
			// Basic OGL Render
			m_pRender = new OGLRender();
		}
		SAFE_CHECK(m_pRender);
		CRender::g_pRender = m_pRender;
	}

	return m_pRender;
}

CTexture * OGLDeviceBuilder::CreateTexture(uint32 dwWidth, uint32 dwHeight, TextureUsage usage)
{
	return new COGLTexture(dwWidth, dwHeight, usage);
}

CColorCombiner * OGLDeviceBuilder::CreateColorCombiner(CRender *pRender)
{
	if( m_pColorCombiner == NULL )
	{
		if( CGraphicsContext::g_pGraphicsContext == NULL && CGraphicsContext::g_pGraphicsContext->Ready() )
		{
			ErrorMsg("Can not create ColorCombiner before creating and initializing GraphicsContext");
		}
		else
		{
			m_deviceType = (SupportedDeviceType)options.OpenglRenderSetting;
			if( m_deviceType == OGL_DEVICE )	// Best fit
			{
				int maxUnit = 2;
				COGLGraphicsContext *pcontext = (COGLGraphicsContext *)(CGraphicsContext::g_pGraphicsContext);
				glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxUnit);

				if( pcontext->IsExtensionSupported("GL_NV_texture_env_combine4") || 
					pcontext->IsExtensionSupported("GL_NV_register_combiners") )
				{
					m_pColorCombiner = new COGLColorCombinerNvidia(pRender);
				}
				else if( pcontext->IsExtensionSupported("GL_NV_texture_env_combine4") )
				{
					m_pColorCombiner = new COGLColorCombinerTNT2(pRender);
				}
				else if( pcontext->IsExtensionSupported("GL_EXT_texture_env_combine") ||
						 pcontext->IsExtensionSupported("GL_ARB_texture_env_combine") )
				{
					if( pcontext->IsExtensionSupported("GL_ARB_texture_env_crossbar") )
					{
						if( maxUnit > 2 )
							m_pColorCombiner = new COGLColorCombiner4v2(pRender);
						else
							m_pColorCombiner = new COGLColorCombiner4(pRender);
					}
					else
					{
						if( maxUnit > 2 )
							m_pColorCombiner = new COGLColorCombiner4v2(pRender);
						else
							m_pColorCombiner = new COGLColorCombiner2(pRender);
					}
				}
				else
				{
					m_pColorCombiner = new COGLColorCombiner(pRender);
				}
			}
			else
			{
				switch(m_deviceType)
				{
				case OGL_1_1_DEVICE:
					m_pColorCombiner = new COGLColorCombiner(pRender);
					break;
				case OGL_1_2_DEVICE:
				case OGL_1_3_DEVICE:
					m_pColorCombiner = new COGLColorCombiner2(pRender);
					break;
				case OGL_1_4_DEVICE:
					m_pColorCombiner = new COGLColorCombiner4(pRender);
					break;
				case OGL_1_4_V2_DEVICE:
					m_pColorCombiner = new COGLColorCombiner4v2(pRender);
					break;
				case OGL_TNT2_DEVICE:
					m_pColorCombiner = new COGLColorCombinerTNT2(pRender);
					break;
				case NVIDIA_OGL_DEVICE:
					m_pColorCombiner = new COGLColorCombinerNvidia(pRender);
					break;

				}
			}
		}

		SAFE_CHECK(m_pColorCombiner);
	}

	return m_pColorCombiner;
}

CBlender * OGLDeviceBuilder::CreateAlphaBlender(CRender *pRender)
{
	if( m_pAlphaBlender == NULL )
	{
		m_pAlphaBlender = new COGLBlender(pRender);
		SAFE_CHECK(m_pAlphaBlender);
	}

	return m_pAlphaBlender;
}


CGraphicsContext * DirectXDeviceBuilder::CreateGraphicsContext(void)
{
	if( g_GraphicsInfo.hStatusBar )
	{
		SetWindowText(g_GraphicsInfo.hStatusBar,"Creating DirectX Device Context");
	}
	if( m_pGraphicsContext == NULL )
	{
		m_pGraphicsContext = new CDXGraphicsContext();
		SAFE_CHECK(m_pGraphicsContext);
		CGraphicsContext::g_pGraphicsContext = m_pGraphicsContext;
	}

	return m_pGraphicsContext;
}

CRender * DirectXDeviceBuilder::CreateRender(void)
{
	if( m_pRender == NULL )
	{
		m_pRender = new D3DRender();
		SAFE_CHECK(m_pRender);
		CRender::g_pRender = m_pRender;
	}

	return m_pRender;
}

CTexture * DirectXDeviceBuilder::CreateTexture(uint32 dwWidth, uint32 dwHeight, TextureUsage usage)
{
	return new CDirectXTexture(dwWidth, dwHeight, usage);
}

CColorCombiner * DirectXDeviceBuilder::CreateColorCombiner(CRender *pRender)
{
	if( m_pColorCombiner == NULL )
	{
		extern D3DCAPS8 g_D3DDeviceCaps;
		int type = options.DirectXCombiner;
		int m_dwCapsMaxStages=g_D3DDeviceCaps.MaxTextureBlendStages;
		bool canUsePixelShader = g_D3DDeviceCaps.PixelShaderVersion >= D3DPS_VERSION(1, 1);

		if( (options.DirectXCombiner == DX_PIXEL_SHADER || options.DirectXCombiner == DX_SEMI_PIXEL_SHADER)
			&& !canUsePixelShader )
		{
			ErrorMsg("Video card does not support pixel shader, will use BEST_FIT");
			options.DirectXCombiner = DX_BEST_FIT;
		}

		if( options.DirectXCombiner == DX_BEST_FIT )
		{
			char buf[200];
			strcpy(buf, CGraphicsContext::g_pGraphicsContext->GetDeviceStr());
			strlwr(buf);
			if( canUsePixelShader )
			{
				type = DX_PIXEL_SHADER;
			}
			else if( strstr(buf, "tnt") != NULL || strstr(buf, "geforce256") != NULL )
			{
				type = DX_2_STAGES;
			}
			else if( strstr(buf, "geforce2") != NULL && strstr(buf,"ti") == NULL )
			{
				type = DX_2_STAGES;
			}
			else
			{
				bool m_bCapsTxtOpAdd = (g_D3DDeviceCaps.TextureOpCaps & D3DTEXOPCAPS_ADD)!=0;
				bool m_bCapsTxtOpLerp = (g_D3DDeviceCaps.TextureOpCaps & D3DTEXOPCAPS_LERP )!=0;
				bool m_bCapsTxtOpSub = (g_D3DDeviceCaps.TextureOpCaps & D3DTEXOPCAPS_SUBTRACT )!=0;

				int m_dwCapsMaxTxt=g_D3DDeviceCaps.MaxSimultaneousTextures;

				if( m_dwCapsMaxTxt == 1 || m_dwCapsMaxStages == 1)
				{
					type = DX_LOW_END;
				}
				else
				{
					type = DX_HIGH_END;
				}
			}
		}

		switch( type )
		{
		case DX_BEST_FIT:
			m_pColorCombiner = new CDirectXColorCombiner(pRender);
			break;
		case DX_LOW_END:
			m_pColorCombiner = new CDirectXColorCombinerLowEnd(pRender);
			break;
		case DX_HIGH_END:
			m_pColorCombiner = new CDirectXColorCombiner(pRender);
			break;
		case DX_NVIDIA_TNT:
			m_pColorCombiner = new CNvTNTDirectXCombiner(pRender);
			break;
		case DX_2_STAGES:
			m_pColorCombiner = new CDirectXColorCombiner(pRender);
			((CDirectXColorCombiner*)m_pColorCombiner)->SetStageLimit(2);
			break;
		case DX_3_STAGES:
			m_pColorCombiner = new CDirectXColorCombiner(pRender);
			((CDirectXColorCombiner*)m_pColorCombiner)->SetStageLimit(3);
			break;
		case DX_4_STAGES:
			m_pColorCombiner = new CDirectXColorCombiner(pRender);
			((CDirectXColorCombiner*)m_pColorCombiner)->SetStageLimit(4);
			break;
		case DX_PIXEL_SHADER:
			m_pColorCombiner = new CDirectXPixelShaderCombiner(pRender);
			break;
		case DX_SEMI_PIXEL_SHADER:
			m_pColorCombiner = new CDirectXSemiPixelShaderCombiner(pRender);
			break;
		}
	
		SAFE_CHECK(m_pColorCombiner);
	}

	return m_pColorCombiner;
}

CBlender * DirectXDeviceBuilder::CreateAlphaBlender(CRender *pRender)
{
	if( m_pAlphaBlender == NULL )
	{
		m_pAlphaBlender = new CDirectXBlender(pRender);
		SAFE_CHECK(m_pAlphaBlender);
	}

	return m_pAlphaBlender;
}


