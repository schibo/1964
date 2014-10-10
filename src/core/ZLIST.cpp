/*
* 1964 Copyright (C) 1999-2004 Joel Middendorf, <schibo@emulation64.com> This
* program is free software; you can redistribute it and/or modify it under the
* terms of the GNU General Public License as published by the Free Software
* Foundation; either version 2 of the License, or (at your option) any later
* version. This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
* details. You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. To contact the
* authors: email: schibo@emulation64.com, rice1964@yahoo.com
*/

#include "stdafx.h"

#define pLOAD_UWORD(addr) ((uint32*)(gMS_RDRAM+addr))
#define pLOAD_HWORD(addr) ((uint16*)(gMS_RDRAM+(addr^2)))
#define pLOAD_SWORD(addr) ((_int16*)(gMS_RDRAM+(addr^2)))

struct
{
    DWORD   m_pic;
    int     w;
    int     h;
    DWORD   q1;
    DWORD   q2;
    DWORD   q3;
} jpg_data;

int    pin[6][64];
int    R[256];
int    g[256];
int    b[256];
int    out[16*16];

short *q1;
short *q2;
short *q3;

int zigzag[64]={
   0,  1,  5,  6, 14, 15, 27, 28,
   2,  4,  7, 13, 16, 26, 29, 42,
   3,  8, 12, 17, 25, 30, 41, 43,
   9, 11, 18, 24, 31, 40, 44, 53,
  10, 19, 23, 32, 39, 45, 52, 54,
  20, 22, 33, 38, 46, 51, 55, 60,
  21, 34, 37, 47, 50, 56, 59, 61,
  35, 36, 48, 49, 57, 58, 62, 63
};

int idctcoeffs[8][8] = {
	256,   256,   256,   256,   256,   256,   256,   256,
	251,   213,   142,    50,   -50,  -142,  -213,  -251,
	237,    98,   -98,  -237,  -237,   -98,    98,   237,
	213,   -50,  -251,  -142,   142,   251,    50,  -213,
	181,  -181,  -181,   181,   181,  -181,  -181,   181,
	142,  -251,    50,   213,  -213,   -50,   251,  -142,
	98,  -237,   237,   -98,   -98,   237,  -237,    98,
	50,  -142,   213,  -251,   251,  -213,   142,   -50,
};

void idct8x8(int *dst,int *src,short *quantizer)
{
	int Fu[8];
	int fxy[8][8];
	int x,y,u,i;

	// Reverse zigzag and reverse quantization
	fxy[0][0] = src[0]*quantizer[1]>>3;
	for(u=1;u<64;u++) 
	{
		i=zigzag[u];
		fxy[u/8][u&7]=src[i]*2*quantizer[i^1]>>3;
	}

	// 1D IDCT in X direction
	for(y=0;y<8;y++)
	{
		for(u=0;u<8;u++) Fu[u]=0;
		for(x=0;x<8;x++)
		{
			for(u=0;u<8;u++)
			{
				Fu[u]+=fxy[y][x]*idctcoeffs[x][u];
			}
		}
		for(u=0;u<8;u++) fxy[y][u]=Fu[u]>>8;
	}

	// 1D IDCT in Y direction
	for(x=0;x<8;x++)
	{
		for(u=0;u<8;u++) Fu[u]=0;
		for(y=0;y<8;y++)
		{
			for(u=0;u<8;u++)
			{
				Fu[u]+=fxy[y][x]*idctcoeffs[y][u];
			}
		}
		for(u=0;u<8;u++) fxy[u][x]=Fu[u]>>8;
	}

	// Convert data to 1D array format, and transport the matrix
	for(y=0;y<8;y++) for(x=0;x<8;x++)
	{
		dst[x+y*8]=fxy[y][x];
	}
}

// Decode 1 macro block
void decodeMB(void)
{
	int ci[4][64], cu[64], cv[64];
    int x,y;
    int Y,U,V;

	idct8x8(ci[0],pin[0],q1);
	idct8x8(ci[1],pin[1],q1);
	idct8x8(ci[2],pin[2],q1);
	idct8x8(ci[3],pin[3],q1);
    idct8x8(cu,pin[4],q2);	
    idct8x8(cv,pin[5],q3);

    for(y=0;y<8;y++) for(x=0;x<8;x++)
    {
		// the U, V component
        U=cu[x+y*8];	
		V=cv[x+y*8];
		R[x*2+y*32]=R[x*2+y*32+1]=R[x*2+y*32+16]=R[x*2+y*32+17]=(         291*V ) >> 8;
		g[x*2+y*32]=g[x*2+y*32+1]=g[x*2+y*32+16]=g[x*2+y*32+17]=( -101*U- 148*V ) >> 8;
		b[x*2+y*32]=b[x*2+y*32+1]=b[x*2+y*32+16]=b[x*2+y*32+17]=(  564*U        ) >> 8;
    }

	// the Y component
    for(y=0;y<8;y++) for(x=0;x<8;x++)
    {
		int idx;
		idx = x+y*16;
		Y=ci[0][x+y*8];
		R[idx]+=Y+128;	if(R[idx]<0) R[idx]=0;	if(R[idx]>255) R[idx]=255;
		g[idx]+=Y+128;	if(g[idx]<0) g[idx]=0;	if(g[idx]>255) g[idx]=255;
		b[idx]+=Y+128;	if(b[idx]<0) b[idx]=0;	if(b[idx]>255) b[idx]=255;
		// Converting RGB to R5G5B5X1
		out[idx]= ((R[idx]&0xFFFF)>>3<<11)|((g[idx]&0xFFFF)>>3<<6)|((b[idx]&0xFFFF)>>3<<1)|1;

		Y=ci[1][x+y*8];
		idx += 8;
		R[idx]+=Y+128;	if(R[idx]<0) R[idx]=0;	if(R[idx]>255) R[idx]=255;
		g[idx]+=Y+128;	if(g[idx]<0) g[idx]=0;	if(g[idx]>255) g[idx]=255;
		b[idx]+=Y+128;	if(b[idx]<0) b[idx]=0;	if(b[idx]>255) b[idx]=255;
		// Converting RGB to R5G5B5X1
		out[idx]= ((R[idx]&0xFFFF)>>3<<11)|((g[idx]&0xFFFF)>>3<<6)|((b[idx]&0xFFFF)>>3<<1)|1;

		Y=ci[2][x+y*8];
		idx += 120;
		R[idx]+=Y+128;	if(R[idx]<0) R[idx]=0;	if(R[idx]>255) R[idx]=255;
		g[idx]+=Y+128;	if(g[idx]<0) g[idx]=0;	if(g[idx]>255) g[idx]=255;
		b[idx]+=Y+128;	if(b[idx]<0) b[idx]=0;	if(b[idx]>255) b[idx]=255;
		// Converting RGB to R5G5B5X1
		out[idx]= ((R[idx]&0xFFFF)>>3<<11)|((g[idx]&0xFFFF)>>3<<6)|((b[idx]&0xFFFF)>>3<<1)|1;

		Y=ci[3][x+y*8];
		idx += 8;
		R[idx]+=Y+128;	if(R[idx]<0) R[idx]=0;	if(R[idx]>255) R[idx]=255;
		g[idx]+=Y+128;	if(g[idx]<0) g[idx]=0;	if(g[idx]>255) g[idx]=255;
		b[idx]+=Y+128;	if(b[idx]<0) b[idx]=0;	if(b[idx]>255) b[idx]=255;
		// Converting RGB to R5G5B5X1
		out[idx]= ((R[idx]&0xFFFF)>>3<<11)|((g[idx]&0xFFFF)>>3<<6)|((b[idx]&0xFFFF)>>3<<1)|1;
    }
}

void zlist_uncompress(OSTask_t *task)
{
    int mb;

	memcpy(&jpg_data, (gMS_RDRAM+task->m_data_ptr), sizeof(jpg_data));

    //Hack for Ogre Battle, does not support it
    if (((uint32)jpg_data.m_pic+(uint32)jpg_data.w*(uint32)(768,16*16*2)) >= current_rdram_size)
        return;

	q1 = (short*)(gMS_RDRAM+jpg_data.q1);
	q2 = (short*)(gMS_RDRAM+jpg_data.q2);
	q3 = (short*)(gMS_RDRAM+jpg_data.q3);

    for(mb=0;mb<jpg_data.w; mb++)
    {
		int i;
		int addr = jpg_data.m_pic+mb*768;

		for(i=0;i<8*8*6;i++)
		{
			((int *)pin)[i]=*pLOAD_SWORD(addr+i*2);
		}

        decodeMB();

		for(i=0;i<8*8*4;i++)
		{
			*pLOAD_HWORD(addr+i*2) = ((int *)out)[i];
		}
    }
}

