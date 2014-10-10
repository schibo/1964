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
#include "TextureFilters.h"


/************************************************************************/
/* 2X filters                                                           */
/************************************************************************/
// Basic 2x R8G8B8A8 filter with interpolation
void Texture2x_32( DrawInfo &srcInfo, DrawInfo &destInfo)
{
	uint32 *pDst1, *pDst2;
	uint32 *pSrc, *pSrc2;
	uint32 nWidth = srcInfo.dwWidth;
	uint32 nHeight = srcInfo.dwHeight;

	uint32 b1;
	uint32 g1;
	uint32 r1;
	uint32 a1;
	uint32 b2;
	uint32 g2;
	uint32 r2;
	uint32 a2;
	uint32 b3;
	uint32 g3;
	uint32 r3;
	uint32 a3;
	uint32 b4;
	uint32 g4;
	uint32 r4;
	uint32 a4;


	for (uint32 ySrc = 0; ySrc < nHeight; ySrc++)
	{
		pSrc = (uint32*)(((uint8*)srcInfo.lpSurface)+ySrc*srcInfo.lPitch);
		pSrc2 = (uint32*)(((uint8*)srcInfo.lpSurface)+(ySrc+1)*srcInfo.lPitch);
		pDst1 = (uint32*)(((uint8*)destInfo.lpSurface)+(ySrc*2)*destInfo.lPitch);
		pDst2 = (uint32*)(((uint8*)destInfo.lpSurface)+(ySrc*2+1)*destInfo.lPitch);

		for (uint32 xSrc = 0; xSrc < nWidth; xSrc++)
		{
			b1 = (pSrc[xSrc]>>0)&0xFF;
			g1 = (pSrc[xSrc]>>8)&0xFF;
			r1 = (pSrc[xSrc]>>16)&0xFF;
			a1 = (pSrc[xSrc]>>24)&0xFF;

			if( xSrc<nWidth-1 )
			{
				b2 = (pSrc[xSrc+1]>>0)&0xFF;
				g2 = (pSrc[xSrc+1]>>8)&0xFF;
				r2 = (pSrc[xSrc+1]>>16)&0xFF;
				a2 = (pSrc[xSrc+1]>>24)&0xFF;
			}

			if( ySrc<nHeight-1 )
			{
				b3 = (pSrc2[xSrc]>>0)&0xFF;
				g3 = (pSrc2[xSrc]>>8)&0xFF;
				r3 = (pSrc2[xSrc]>>16)&0xFF;
				a3 = (pSrc2[xSrc]>>24)&0xFF;
				if( xSrc<nWidth-1 )
				{
					b4 = (pSrc2[xSrc+1]>>0)&0xFF;
					g4 = (pSrc2[xSrc+1]>>8)&0xFF;
					r4 = (pSrc2[xSrc+1]>>16)&0xFF;
					a4 = (pSrc2[xSrc+1]>>24)&0xFF;
				}
			}


			// Pixel 1
			pDst1[xSrc*2] = pSrc[xSrc];

			// Pixel 2
			if( xSrc<nWidth-1 )
			{
				pDst1[xSrc*2+1] = DWORD_MAKE((r1+r2)/2, (g1+g2)/2, (b1+b2)/2, (a1+a2)/2);
			}
			else
				pDst1[xSrc*2+1] = pSrc[xSrc];


			// Pixel 3
			if( ySrc<nHeight-1 )
			{
				pDst2[xSrc*2] = DWORD_MAKE((r1+r3)/2, (g1+g3)/2, (b1+b3)/2, (a1+a3)/2);
			}
			else
				pDst2[xSrc*2] = pSrc[xSrc];

			// Pixel 4
			if( xSrc<nWidth-1 )
			{
				if( ySrc<nHeight-1 )
				{
					pDst2[xSrc*2+1] = DWORD_MAKE((r1+r2+r3+r4)/4, (g1+g2+g3+g4)/4, (b1+b2+b3+b4)/4, (a1+a2+a3+a4)/4);
				}
				else
				{
					pDst2[xSrc*2+1] = DWORD_MAKE((r1+r2)/2, (g1+g2)/2, (b1+b2)/2, (a1+a2)/2);
				}
			}
			else
			{
				if( ySrc<nHeight-1 )
				{
					pDst2[xSrc*2+1] = DWORD_MAKE((r1+r3)/2, (g1+g3)/2, (b1+b3)/2, (a1+a3)/2);
				}
				else
					pDst2[xSrc*2+1] = pSrc[xSrc];
			}
		}
	}
}

// Basic 2x R4G4B4A4 filter with interpolation
void Texture2x_16( DrawInfo &srcInfo, DrawInfo &destInfo )
{
	uint16 *pDst1, *pDst2;
	uint16 *pSrc, *pSrc2;
	uint32 nWidth = srcInfo.dwWidth;
	uint32 nHeight = srcInfo.dwHeight;

	uint16 b1;
	uint16 g1;
	uint16 r1;
	uint16 a1;
	uint16 b2;
	uint16 g2;
	uint16 r2;
	uint16 a2;
	uint16 b3;
	uint16 g3;
	uint16 r3;
	uint16 a3;
	uint16 b4;
	uint16 g4;
	uint16 r4;
	uint16 a4;

	for (uint16 ySrc = 0; ySrc < nHeight; ySrc++)
	{
		pSrc = (uint16*)(((uint8*)srcInfo.lpSurface)+ySrc*srcInfo.lPitch);
		pSrc2 = (uint16*)(((uint8*)srcInfo.lpSurface)+(ySrc+1)*srcInfo.lPitch);
		pDst1 = (uint16*)(((uint8*)destInfo.lpSurface)+(ySrc*2)*destInfo.lPitch);
		pDst2 = (uint16*)(((uint8*)destInfo.lpSurface)+(ySrc*2+1)*destInfo.lPitch);

		for (uint16 xSrc = 0; xSrc < nWidth; xSrc++)
		{
			b1 = (pSrc[xSrc]>> 0)&0xF;
			g1 = (pSrc[xSrc]>> 4)&0xF;
			r1 = (pSrc[xSrc]>> 8)&0xF;
			a1 = (pSrc[xSrc]>>12)&0xF;

			if( xSrc<nWidth-1 )
			{
				b2 = (pSrc[xSrc+1]>> 0)&0xF;
				g2 = (pSrc[xSrc+1]>> 4)&0xF;
				r2 = (pSrc[xSrc+1]>> 8)&0xF;
				a2 = (pSrc[xSrc+1]>>12)&0xF;
			}

			if( ySrc<nHeight-1 )
			{
				b3 = (pSrc2[xSrc]>> 0)&0xF;
				g3 = (pSrc2[xSrc]>> 4)&0xF;
				r3 = (pSrc2[xSrc]>> 8)&0xF;
				a3 = (pSrc2[xSrc]>>12)&0xF;
				if( xSrc<nWidth-1 )
				{
					b4 = (pSrc2[xSrc+1]>> 0)&0xF;
					g4 = (pSrc2[xSrc+1]>> 4)&0xF;
					r4 = (pSrc2[xSrc+1]>> 8)&0xF;
					a4 = (pSrc2[xSrc+1]>>12)&0xF;
				}
			}

			// Pixel 1
			pDst1[xSrc*2] = pSrc[xSrc];

			// Pixel 2
			if( xSrc<nWidth-1 )
			{
				pDst1[xSrc*2+1] = WORD_MAKE((r1+r2)/2, (g1+g2)/2, (b1+b2)/2, (a1+a2)/2);
			}
			else
				pDst1[xSrc*2+1] = pSrc[xSrc];


			// Pixel 3
			if( ySrc<nHeight-1 )
			{
				pDst2[xSrc*2] = WORD_MAKE((r1+r3)/2, (g1+g3)/2, (b1+b3)/2, (a1+a3)/2);
			}
			else
				pDst2[xSrc*2] = pSrc[xSrc];

			// Pixel 4
			if( xSrc<nWidth-1 )
			{
				if( ySrc<nHeight-1 )
				{
					pDst2[xSrc*2+1] = WORD_MAKE((r1+r2+r3+r4)/4, (g1+g2+g3+g4)/4, (b1+b2+b3+b4)/4, (a1+a2+a3+a4)/4);
				}
				else
				{
					pDst2[xSrc*2+1] = WORD_MAKE((r1+r2)/2, (g1+g2)/2, (b1+b2)/2, (a1+a2)/2);
				}
			}
			else
			{
				if( ySrc<nHeight-1 )
				{
					pDst2[xSrc*2+1] = WORD_MAKE((r1+r3)/2, (g1+g3)/2, (b1+b3)/2, (a1+a3)/2);
				}
				else
					pDst2[xSrc*2+1] = pSrc[xSrc];
			}
		}
	}
}

/************************************************************************/
/* Sharpen filters                                                      */
/************************************************************************/
void SharpenFilter_32(uint32 *pdata, uint32 width, uint32 height, uint32 pitch, uint32 filter)
{
	uint32 len=height*pitch;
	uint32 *pcopy = new uint32[len];
	memcpy(pcopy, pdata, len<<2);

	uint32 mul1, mul2, mul3, shift4;
	switch( filter )
	{
	case TEXTURE_SHARPEN_MORE_ENHANCEMENT:
		mul1=1;
		mul2=8;
		mul3=12;
		shift4=2;
		break;
	case TEXTURE_SHARPEN_ENHANCEMENT:
	default:
		mul1=1;
		mul2=8;
		mul3=16;
		shift4=3;
		break;
	}

	uint32 x,y,z;
	uint32 *src1, *src2, *src3, *dest;
	uint32 val[4];
	uint32 t1,t2,t3,t4,t5,t6,t7,t8,t9;

	for( y=1; y<height-1; y++)
	{
		dest = pdata+y*pitch;
		src1 = pcopy+(y-1)*pitch;
		src2 = src1 + pitch;
		src3 = src2 + pitch;
		for( x=1; x<width-1; x++)
		{
			for( z=0; z<4; z++ )
			{
				t1 = *((uint8*)(src1+x-1)+z);
				t2 = *((uint8*)(src1+x  )+z);
				t3 = *((uint8*)(src1+x+1)+z);
				t4 = *((uint8*)(src2+x-1)+z);
				t5 = *((uint8*)(src2+x  )+z);
				t6 = *((uint8*)(src2+x+1)+z);
				t7 = *((uint8*)(src3+x-1)+z);
				t8 = *((uint8*)(src3+x  )+z);
				t9 = *((uint8*)(src3+x+1)+z);
				val[z]=t5;
				if( (t5*mul2) > (t1+t3+t7+t9+t2+t4+t6+t8)*mul1 )
				{
					val[z]= min((((t5*mul3) - (t1+t3+t7+t9+t2+t4+t6+t8)*mul1)>>shift4),0xFF);
				}
			}
			dest[x] = val[0]|(val[1]<<8)|(val[2]<<16)|(val[3]<<24);
		}
	}
	delete [] pcopy;
}

void SharpenFilter_16(uint16 *pdata, uint32 width, uint32 height, uint32 pitch, uint32 filter)
{
	//return;	// Sharpen does not make sense for 16 bits

	uint32 len=height*pitch;
	uint16 *pcopy = new uint16[len];
	memcpy(pcopy, pdata, len<<1);

	uint16 mul1, mul2, mul3, shift4;
	switch( filter )
	{
	case TEXTURE_SHARPEN_MORE_ENHANCEMENT:
		mul1=1;
		mul2=8;
		mul3=12;
		shift4=2;
		break;
	case TEXTURE_SHARPEN_ENHANCEMENT:
	default:
		mul1=1;
		mul2=8;
		mul3=16;
		shift4=3;
		break;
	}

	uint32 x,y,z;
	uint16 *src1, *src2, *src3, *dest;
	uint16 val[4];
	uint16 t1,t2,t3,t4,t5,t6,t7,t8,t9;

	for( y=1; y<height-1; y++)
	{
		dest = pdata+y*pitch;
		src1 = pcopy+(y-1)*pitch;
		src2 = src1 + pitch;
		src3 = src2 + pitch;
		for( x=1; x<width-1; x++)
		{
			for( z=0; z<4; z++ )
			{
				uint32 shift = (z%1)?4:0;
				t1 = (*((uint8*)(src1+x-1)+(z>>1)))>>shift;
				t2 = (*((uint8*)(src1+x  )+(z>>1)))>>shift;
				t3 = (*((uint8*)(src1+x+1)+(z>>1)))>>shift;
				t4 = (*((uint8*)(src2+x-1)+(z>>1)))>>shift;
				t5 = (*((uint8*)(src2+x  )+(z>>1)))>>shift;
				t6 = (*((uint8*)(src2+x+1)+(z>>1)))>>shift;
				t7 = (*((uint8*)(src3+x-1)+(z>>1)))>>shift;
				t8 = (*((uint8*)(src3+x  )+(z>>1)))>>shift;
				t9 = (*((uint8*)(src3+x+1)+(z>>1)))>>shift;
				val[z]=t5;
				if( (t5*mul2) > (t1+t3+t7+t9+t2+t4+t6+t8)*mul1 )
				{
					val[z] = (((t5*mul3) - (t1+t3+t7+t9+t2+t4+t6+t8)*mul1)>>shift4);
					val[z]= min(val[z],0xF);
				}
			}
			dest[x] = val[0]|(val[1]<<4)|(val[2]<<8)|(val[3]<<12);
		}
	}
	delete [] pcopy;
}

/************************************************************************/
/* Smooth filters                                                       */
/************************************************************************/
void SmoothFilter_32(uint32 *pdata, uint32 width, uint32 height, uint32 pitch, uint32 filter)
{
	uint32 len=height*pitch;
	uint32 *pcopy = new uint32[len];
	memcpy(pcopy, pdata, len<<2);

	uint32 mul1, mul2, mul3, shift4;
	switch( filter )
	{
	case TEXTURE_ENHANCEMENT_WITH_SMOOTH_FILTER_1:
		mul1=1;
		mul2=2;
		mul3=4;
		shift4=4;
		break;
	case TEXTURE_ENHANCEMENT_WITH_SMOOTH_FILTER_2:
		mul1=1;
		mul2=1;
		mul3=8;
		shift4=4;
		break;
	case TEXTURE_ENHANCEMENT_WITH_SMOOTH_FILTER_3:
		mul1=1;
		mul2=1;
		mul3=2;
		shift4=2;
		break;
	case TEXTURE_ENHANCEMENT_WITH_SMOOTH_FILTER_4:
	default:
		mul1=1;
		mul2=1;
		mul3=6;
		shift4=3;
		break;
	}

	uint32 x,y,z;
	uint32 *src1, *src2, *src3, *dest;
	uint32 val[4];
	uint32 t1,t2,t3,t4,t5,t6,t7,t8,t9;

	if( filter == TEXTURE_ENHANCEMENT_WITH_SMOOTH_FILTER_3 || filter == TEXTURE_ENHANCEMENT_WITH_SMOOTH_FILTER_4 )
	{
		for( y=1; y<height-1; y+=2)
		{
			dest = pdata+y*pitch;
			src1 = pcopy+(y-1)*pitch;
			src2 = src1 + pitch;
			src3 = src2 + pitch;
			for( x=0; x<width; x++)
			{
				for( z=0; z<4; z++ )
				{
					t2 = *((uint8*)(src1+x  )+z);
					t5 = *((uint8*)(src2+x  )+z);
					t8 = *((uint8*)(src3+x  )+z);
					val[z] = ((t2+t8)*mul2+(t5*mul3))>>shift4;
				}
				dest[x] = val[0]|(val[1]<<8)|(val[2]<<16)|(val[3]<<24);
			}
		}
	}
	else
	{
		for( y=0; y<height; y++)
		{
			dest = pdata+y*pitch;
			if( y>0 )
			{
				src1 = pcopy+(y-1)*pitch;
				src2 = src1 + pitch;
			}
			else
			{
				src1 = src2 = pcopy;
			}

			src3 = src2;
			if( y<height-1)	src3 += pitch;

			for( x=1; x<width-1; x++)
			{
				for( z=0; z<4; z++ )
				{
					t1 = *((uint8*)(src1+x-1)+z);
					t2 = *((uint8*)(src1+x  )+z);
					t3 = *((uint8*)(src1+x+1)+z);
					t4 = *((uint8*)(src2+x-1)+z);
					t5 = *((uint8*)(src2+x  )+z);
					t6 = *((uint8*)(src2+x+1)+z);
					t7 = *((uint8*)(src3+x-1)+z);
					t8 = *((uint8*)(src3+x  )+z);
					t9 = *((uint8*)(src3+x+1)+z);
					val[z] = ((t1+t3+t7+t9)*mul1+((t2+t4+t6+t8)*mul2)+(t5*mul3))>>shift4;
				}
				dest[x] = val[0]|(val[1]<<8)|(val[2]<<16)|(val[3]<<24);
			}
		}
	}
	delete [] pcopy;
}

void SmoothFilter_16(uint16 *pdata, uint32 width, uint32 height, uint32 pitch, uint32 filter)
{
	uint32 len=height*pitch;
	uint16 *pcopy = new uint16[len];
	memcpy(pcopy, pdata, len<<1);

	uint16 mul1, mul2, mul3, shift4;
	switch( filter )
	{
	case TEXTURE_ENHANCEMENT_WITH_SMOOTH_FILTER_1:
		mul1=1;
		mul2=2;
		mul3=4;
		shift4=4;
		break;
	case TEXTURE_ENHANCEMENT_WITH_SMOOTH_FILTER_2:
		mul1=1;
		mul2=1;
		mul3=8;
		shift4=4;
		break;
	case TEXTURE_ENHANCEMENT_WITH_SMOOTH_FILTER_3:
		mul1=1;
		mul2=1;
		mul3=2;
		shift4=2;
		break;
	case TEXTURE_ENHANCEMENT_WITH_SMOOTH_FILTER_4:
	default:
		mul1=1;
		mul2=1;
		mul3=6;
		shift4=3;
		break;
	}

	uint32 x,y,z;
	uint16 *src1, *src2, *src3, *dest;
	uint16 val[4];
	uint16 t1,t2,t3,t4,t5,t6,t7,t8,t9;

	if( filter == TEXTURE_ENHANCEMENT_WITH_SMOOTH_FILTER_3 || filter == TEXTURE_ENHANCEMENT_WITH_SMOOTH_FILTER_4 )
	{
		for( y=1; y<height-1; y+=2)
		{
			dest = pdata+y*pitch;
			src1 = pcopy+(y-1)*pitch;
			src2 = src1 + pitch;
			src3 = src2 + pitch;
			for( x=0; x<width; x++)
			{
				for( z=0; z<4; z++ )
				{
					uint32 shift = (z&1)?4:0;
					t2 = (*((uint8*)(src1+x  )+(z>>1)))>>shift;
					t5 = (*((uint8*)(src2+x  )+(z>>1)))>>shift;
					t8 = (*((uint8*)(src3+x  )+(z>>1)))>>shift;
					val[z] = ((t2+t8)*mul2+(t5*mul3))>>shift4;
				}
				dest[x] = val[0]|(val[1]<<4)|(val[2]<<8)|(val[3]<<12);
			}
		}
	}
	else
	{
		for( y=0; y<height; y++)
		{
			dest = pdata+y*pitch;
			if( y>0 )
			{
				src1 = pcopy+(y-1)*pitch;
				src2 = src1 + pitch;
			}
			else
			{
				src1 = src2 = pcopy;
			}

			src3 = src2;
			if( y<height-1)	src3 += pitch;

			for( x=1; x<width-1; x++)
			{
				for( z=0; z<4; z++ )
				{
					uint32 shift = (z&1)?4:0;
					t1 = (*((uint8*)(src1+x-1)+(z>>1)))>>shift;
					t2 = (*((uint8*)(src1+x  )+(z>>1)))>>shift;
					t3 = (*((uint8*)(src1+x+1)+(z>>1)))>>shift;
					t4 = (*((uint8*)(src2+x-1)+(z>>1)))>>shift;
					t5 = (*((uint8*)(src2+x  )+(z>>1)))>>shift;
					t6 = (*((uint8*)(src2+x+1)+(z>>1)))>>shift;
					t7 = (*((uint8*)(src3+x-1)+(z>>1)))>>shift;
					t8 = (*((uint8*)(src3+x  )+(z>>1)))>>shift;
					t9 = (*((uint8*)(src3+x+1)+(z>>1)))>>shift;
					val[z] = ((t1+t3+t7+t9)*mul1+((t2+t4+t6+t8)*mul2)+(t5*mul3))>>shift4;
				}
				dest[x] = val[0]|(val[1]<<4)|(val[2]<<8)|(val[3]<<12);
			}
		}
	}
	delete [] pcopy;
}


CTexture* EnhanceTexture(TxtrCacheEntry *pEntry)
{
	if( pEntry->dwEnhancementFlag == options.textureEnhancement )
	{
		// The texture has already been enhanced
		return NULL;
	}
	else if( options.textureEnhancement == TEXTURE_NO_ENHANCEMENT )
	{
		SAFE_DELETE(pEntry->pEnhancedTexture);
		pEntry->dwEnhancementFlag = TEXTURE_NO_ENHANCEMENT;
		return NULL;
	}

	if( status.primitiveType != PRIM_TEXTRECT && options.bTexRectOnly )
	{
		return NULL;
	}

	DrawInfo srcInfo;	
	if( pEntry->pTexture->StartUpdate(&srcInfo) == false )
	{
		SAFE_DELETE(pEntry->pEnhancedTexture);
		return NULL;
	}

	uint32 realwidth = srcInfo.dwWidth;
	uint32 realheight = srcInfo.dwHeight;
	uint32 nWidth = srcInfo.dwCreatedWidth;
	uint32 nHeight = srcInfo.dwCreatedHeight;

	if( options.textureEnhancement == TEXTURE_SHARPEN_ENHANCEMENT || options.textureEnhancement == TEXTURE_SHARPEN_MORE_ENHANCEMENT )
	{
		if( pEntry->pTexture->GetPixelSize() == 4 )
			SharpenFilter_32((uint32*)srcInfo.lpSurface, nWidth, nHeight, nWidth, options.textureEnhancement);
		else
			SharpenFilter_16((uint16*)srcInfo.lpSurface, nWidth, nHeight, nWidth, options.textureEnhancement);
		pEntry->dwEnhancementFlag = options.textureEnhancement;
		pEntry->pTexture->EndUpdate(&srcInfo);
		SAFE_DELETE(pEntry->pEnhancedTexture);
		return NULL;
	}

	pEntry->dwEnhancementFlag = options.textureEnhancement;
	if( options.bSmallTextureOnly )
	{
		if( nWidth + nHeight > 256 )
		{
			pEntry->pTexture->EndUpdate(&srcInfo);
			SAFE_DELETE(pEntry->pEnhancedTexture);
			pEntry->dwEnhancementFlag = TEXTURE_NO_ENHANCEMENT;
			return NULL;
		}
	}


	CTexture* pSurfaceHandler = NULL;
	if( options.textureEnhancement == TEXTURE_HQ4X_ENHANCEMENT )
	{
		if( nWidth + nHeight > 1024/4 )
		{
			// Don't enhance for large textures
			pEntry->pTexture->EndUpdate(&srcInfo);
			SAFE_DELETE(pEntry->pEnhancedTexture);
			pEntry->dwEnhancementFlag = TEXTURE_NO_ENHANCEMENT;
			return NULL;
		}
		pSurfaceHandler = CDeviceBuilder::GetBuilder()->CreateTexture(nWidth*4, nHeight*4);
	}
	else
	{
		if( nWidth + nHeight > 1024/2 )
		{
			// Don't enhance for large textures
			pEntry->pTexture->EndUpdate(&srcInfo);
			SAFE_DELETE(pEntry->pEnhancedTexture);
			pEntry->dwEnhancementFlag = TEXTURE_NO_ENHANCEMENT;
			return NULL;
		}
		pSurfaceHandler = CDeviceBuilder::GetBuilder()->CreateTexture(nWidth*2, nHeight*2);
	}
	DrawInfo destInfo;
	if(pSurfaceHandler->StartUpdate(&destInfo))
	{
		if( options.textureEnhancement == TEXTURE_2XSAI_ENHANCEMENT )
		{
			if( pEntry->pTexture->GetPixelSize() == 4 )
				Super2xSaI_32((uint32*)(srcInfo.lpSurface),(uint32*)(destInfo.lpSurface), nWidth, realheight, nWidth);
			else
				Super2xSaI_16((uint16*)(srcInfo.lpSurface),(uint16*)(destInfo.lpSurface), nWidth, realheight, nWidth);
		}
		else if( options.textureEnhancement == TEXTURE_HQ2X_ENHANCEMENT )
		{
			if( pEntry->pTexture->GetPixelSize() == 4 )
			{
				hq2x_init(32);
				hq2x_32((uint8*)(srcInfo.lpSurface), srcInfo.lPitch, (uint8*)(destInfo.lpSurface), destInfo.lPitch, nWidth, realheight);
			}
			else
			{
				hq2x_init(16);
				hq2x_16((uint8*)(srcInfo.lpSurface), srcInfo.lPitch, (uint8*)(destInfo.lpSurface), destInfo.lPitch, nWidth, realheight);
			}
		}
		else if( options.textureEnhancement == TEXTURE_LQ2X_ENHANCEMENT )
		{
			if( pEntry->pTexture->GetPixelSize() == 4 )
			{
				hq2x_init(32);
				lq2x_32((uint8*)(srcInfo.lpSurface), srcInfo.lPitch, (uint8*)(destInfo.lpSurface), destInfo.lPitch, nWidth, realheight);
			}
			else
			{
				hq2x_init(16);
				lq2x_16((uint8*)(srcInfo.lpSurface), srcInfo.lPitch, (uint8*)(destInfo.lpSurface), destInfo.lPitch, nWidth, realheight);
			}
		}
		else if( options.textureEnhancement == TEXTURE_HQ4X_ENHANCEMENT )
		{
			if( pEntry->pTexture->GetPixelSize() == 4 )
			{
				hq4x_InitLUTs();
				hq4x_32((uint8*)(srcInfo.lpSurface), (uint8*)(destInfo.lpSurface), realwidth, realheight, nWidth, destInfo.lPitch);
			}
			else
			{
				hq4x_InitLUTs();
				hq4x_16((uint8*)(srcInfo.lpSurface), (uint8*)(destInfo.lpSurface), realwidth, realheight, nWidth, destInfo.lPitch);
			}
		}
		else 
		{
			if( pEntry->pTexture->GetPixelSize() == 4 )
				Texture2x_32( srcInfo, destInfo);
			else
				Texture2x_16( srcInfo, destInfo);
		}

		if( options.textureEnhancementControl >= TEXTURE_ENHANCEMENT_WITH_SMOOTH_FILTER_1 )
		{
			if( options.textureEnhancement != TEXTURE_HQ4X_ENHANCEMENT )
			{
				if( pEntry->pTexture->GetPixelSize() == 4 )
					SmoothFilter_32((uint32*)destInfo.lpSurface, realwidth<<1, realheight<<1, nWidth<<1, options.textureEnhancementControl);
				else
					SmoothFilter_16((uint16*)destInfo.lpSurface, realwidth<<1, realheight<<1, nWidth<<1, options.textureEnhancementControl);
			}
			else
			{
				if( pEntry->pTexture->GetPixelSize() == 4 )
					SmoothFilter_32((uint32*)destInfo.lpSurface, realwidth<<2, realheight<<2, nWidth<<2, options.textureEnhancementControl);
				else
					SmoothFilter_16((uint16*)destInfo.lpSurface, realwidth<<2, realheight<<2, nWidth<<2, options.textureEnhancementControl);
			}
		}

		pSurfaceHandler->EndUpdate(&destInfo);	
	}
	
	pSurfaceHandler->SetOthersVariables();
	pSurfaceHandler->m_dwCreatedTextureWidth = pEntry->pTexture->m_dwCreatedTextureWidth;
	pSurfaceHandler->m_dwCreatedTextureHeight = pEntry->pTexture->m_dwCreatedTextureHeight;

	pEntry->pTexture->EndUpdate(&srcInfo);

	pEntry->pEnhancedTexture = pSurfaceHandler;
	return pSurfaceHandler;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void MirrorEmulator_DrawLine(DrawInfo& destInfo, DrawInfo& srcInfo, LPDWORD pSource, LPDWORD pDest, uint32 nWidth, BOOL bFlipLeftRight)
{
	if(!bFlipLeftRight)
	{
		memcpy(pDest, pSource, nWidth * 4);
	}
	else
	{
		LPDWORD pMaxDest = pDest + nWidth;
		pSource += nWidth - 1;
		for(; pDest < pMaxDest; pDest++, pSource--)
		{
			*pDest = *pSource;
		}
	}
}


void MirrorEmulator_Draw(DrawInfo& destInfo, DrawInfo& srcInfo, uint32 nDestX, uint32 nDestY, BOOL bFlipLeftRight, BOOL bFlipUpDown)
{
	LPBYTE pDest = (LPBYTE)((uint32)destInfo.lpSurface + (destInfo.lPitch * nDestY) + (4 * nDestX));
	LPBYTE pMaxDest = pDest + (destInfo.lPitch * srcInfo.dwHeight);
	LPBYTE pSource = (LPBYTE)(srcInfo.lpSurface);
	if(!bFlipUpDown)
	{
		for(; pDest < pMaxDest; pDest += destInfo.lPitch, pSource += srcInfo.lPitch)
		{
			MirrorEmulator_DrawLine(destInfo, srcInfo, (LPDWORD)pSource, (LPDWORD)pDest, srcInfo.dwWidth, bFlipLeftRight);
		}
	}
	else
	{
		pSource += (srcInfo.lPitch * (srcInfo.dwHeight - 1));
		for(; pDest < pMaxDest; pDest += destInfo.lPitch, pSource -= srcInfo.lPitch)
		{
			MirrorEmulator_DrawLine(destInfo, srcInfo, (LPDWORD)pSource, (LPDWORD)pDest, srcInfo.dwWidth, bFlipLeftRight);
		}
	}
}

CTexture* MirrorTexture(uint32 dwTile, TxtrCacheEntry *pEntry)
{
	if( ((gRDP.tiles[dwTile].bMirrorS) || (gRDP.tiles[dwTile].bMirrorT)) && CGraphicsContext::Get()->m_supportTextureMirror == false )
	{
		if(pEntry->pEnhancedTexture)
		{
			return pEntry->pEnhancedTexture;
		}
		else
		{
			CTexture* pSurfaceHandler = NULL;

			// FIXME: Compute the correct values. 2/2 seems to always work correctly in Mario64
			uint32 nXTimes = gRDP.tiles[dwTile].bMirrorS ? 2 : 1;
			uint32 nYTimes = gRDP.tiles[dwTile].bMirrorT ? 2 : 1;
			
			// For any texture need to use mirror, we should not need to rescale it
			// because texture need to be mirrored must with MaskS and MaskT

			// But again, check me

			//if( pEntry->pTexture->m_bScaledS == false || pEntry->pTexture->m_bScaledT == false)
			//{
			//	pEntry->pTexture->ScaleImageToSurface();
			//}

			DrawInfo srcInfo;	
			if( pEntry->pTexture->StartUpdate(&srcInfo) )
			{
				uint32 nWidth = srcInfo.dwWidth;
				uint32 nHeight = srcInfo.dwHeight;

				pSurfaceHandler = CDeviceBuilder::GetBuilder()->CreateTexture(nWidth * nXTimes, nHeight * nYTimes);
				DrawInfo destInfo;
				if( pSurfaceHandler->StartUpdate(&destInfo) )
				{
					for(uint32 nY = 0; nY < nYTimes; nY++)
					{
						for(uint32 nX = 0; nX < nXTimes; nX++)
						{
							MirrorEmulator_Draw(destInfo, srcInfo, nWidth * nX, nHeight * nY, nX & 0x1, nY & 0x1);
						}
					}

					pSurfaceHandler->EndUpdate(&destInfo);
				}
				
				// fix me, there should be a flag to tell that it is a mirrored texture handler
				// not the original texture handlers, so all texture coordinate should be divided by 2
				pSurfaceHandler->SetOthersVariables();

				pEntry->pTexture->EndUpdate(&srcInfo);			
			}


			pEntry->pEnhancedTexture = pSurfaceHandler;

			return pSurfaceHandler;
		}
	}
	return NULL;
}

void ConvertTextureRGBAtoI(TxtrCacheEntry* pEntry, bool alpha)
{
	DrawInfo srcInfo;	
	if( pEntry->pTexture->StartUpdate(&srcInfo) )
	{
		uint32 *buf;
		uint32 val;
		uint32 r,g,b,a,i;

		for(int nY = 0; nY < srcInfo.dwCreatedHeight; nY++)
		{
			buf = (uint32*)((uint8*)srcInfo.lpSurface+nY*srcInfo.lPitch);
			for(int nX = 0; nX < srcInfo.dwCreatedWidth; nX++)
			{
				val = buf[nX];
				b = (val>>0)&0xFF;
				g = (val>>8)&0xFF;
				r = (val>>16)&0xFF;
				i = (r+g+b)/3;
				a = alpha?(val&0xFF000000):(i<<24);
				buf[nX] = (a|(i<<16)|(i<<8)|i);
			}
		}
		pEntry->pTexture->EndUpdate(&srcInfo);	}
}