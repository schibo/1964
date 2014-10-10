#include "stdafx.h"	
#include <ocidl.h>
#include <ObjIdl.h>
#include <olectl.h>
#include <vector>

// Function LoadAnImage: accepts a file name and returns a HBITMAP.
// On error, it returns 0.
// The code is borrowed from CodeGuru, thanks very much
// Rice 03/11/2004
HBITMAP LoadAnImage_CPP(char* FileName)
{
	// Use IPicture stuff to use JPG / GIF files
	IPicture* p;
	IStream* s;
	HGLOBAL hG;
	void* pp;
	FILE* fp;
	int fs;
	HBITMAP hB = 0;
	HBITMAP hBB;


	// Read file in memory
	fp = fopen(FileName,"rb");
	if (!fp)
		return NULL;

	fseek(fp,0,SEEK_END);
	fs = ftell(fp);
	fseek(fp,0,SEEK_SET);
	hG = GlobalAlloc(GPTR,fs);
	if (!hG)
	{
		fclose(fp);
		return NULL;
	}
	pp = (void*)hG;
	fread(pp,1,fs,fp);
	fclose(fp);

	// Create an IStream so IPicture can 
	CreateStreamOnHGlobal(hG,FALSE,&s);
	if (!s)
	{
		GlobalFree(hG);
		return NULL;
	}

	OleLoadPicture(s,0,FALSE,IID_IPicture,(void**)&p);

	if (!p)
	{
		s->Release();
		GlobalFree(hG);
		return NULL;
	}
	s->Release();
	GlobalFree(hG);

	p->get_Handle((unsigned int*)&hB);

	// Copy the image. Necessary, because upon p's release,
	// the handle is destroyed.
	hBB = (HBITMAP)CopyImage(hB,IMAGE_BITMAP,0,0,LR_COPYRETURNORG);

	p->Release();
	return hBB;
}

/*

//
// Functions for smooth bitmap resize
//
// Improvement: float calculations changed to int.
//
// Ivaylo Byalkov, January 24, 2000
// e-mail: ivob@i-n.net
//

///////////////////////////////////////////////////////////

// helper function prototypes
static BITMAPINFO *PrepareRGBBitmapInfo(WORD wWidth, 
                                        WORD wHeight);

static void ShrinkDataInt(BYTE *pInBuff, 
                          WORD wWidth, 
                          WORD wHeight,
                          BYTE *pOutBuff, 
                          WORD wNewWidth, 
                          WORD wNewHeight);

static void EnlargeDataInt(BYTE *pInBuff, 
                           WORD wWidth, 
                           WORD wHeight,
                           BYTE *pOutBuff, 
                           WORD wNewWidth, 
                           WORD wNewHeight);

///////////////////////////////////////////////////////////
// Main resize function

HBITMAP ScaleBitmapInt(HBITMAP hBmp, WORD wNewWidth, WORD wNewHeight)
{
	BITMAP bmp;
	::GetObject(hBmp, sizeof(BITMAP), &bmp);

	// check for valid size
	//if((bmp.bmWidth > wNewWidth && bmp.bmHeight < wNewHeight) || bmp.bmWidth < wNewWidth && bmp.bmHeight > wNewHeight )
	//	return NULL;

	HDC hDC = ::GetDC(NULL);
	BITMAPINFO *pbi = PrepareRGBBitmapInfo((WORD)bmp.bmWidth, 
		(WORD)bmp.bmHeight);
	BYTE *pData = new BYTE[pbi->bmiHeader.biSizeImage];

	::GetDIBits(hDC, hBmp, 0, bmp.bmHeight, pData, pbi, DIB_RGB_COLORS);

	delete pbi;
	pbi = PrepareRGBBitmapInfo(wNewWidth, wNewHeight);
	BYTE *pData2 = new BYTE[pbi->bmiHeader.biSizeImage];

	if(bmp.bmWidth >= wNewWidth 
		&& bmp.bmHeight >= wNewHeight)
		ShrinkDataInt(pData, 
		(WORD)bmp.bmWidth, 
		(WORD)bmp.bmHeight,
		pData2, 
		wNewWidth, 
		wNewHeight);
	else
		EnlargeDataInt(pData, 
		(WORD)bmp.bmWidth, 
		(WORD)bmp.bmHeight,
		pData2, 
		wNewWidth, 
		wNewHeight);

	delete pData;

	HBITMAP hResBmp = ::CreateCompatibleBitmap(hDC, wNewWidth, wNewHeight);

	::SetDIBits(hDC, hResBmp, 0, wNewHeight, pData2, pbi, DIB_RGB_COLORS);
	::ReleaseDC(NULL, hDC);

	delete pbi;
	delete pData2;

	return hResBmp;
}

///////////////////////////////////////////////////////////

BITMAPINFO *PrepareRGBBitmapInfo(WORD wWidth, WORD wHeight)
{
	BITMAPINFO *pRes = new BITMAPINFO;
	::ZeroMemory(pRes, sizeof(BITMAPINFO));
	pRes->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pRes->bmiHeader.biWidth = wWidth;
	pRes->bmiHeader.biHeight = wHeight;
	pRes->bmiHeader.biPlanes = 1;
	pRes->bmiHeader.biBitCount = 24;

	pRes->bmiHeader.biSizeImage = 
		((3 * wWidth + 3) & ~3) * wHeight;

	return pRes;
}

///////////////////////////////////////////////////////////

static int *CreateCoeffInt(int nLen, int nNewLen, BOOL bShrink)
{
	int nSum = 0, nSum2;
	int *pRes = new int[2 * nLen];
	int *pCoeff = pRes;
	int nNorm = (bShrink) 
		? (nNewLen << 12) / nLen : 0x1000;
	int	nDenom = (bShrink)? nLen : nNewLen;

	::ZeroMemory(pRes, 2 * nLen * sizeof(int));
	for(int i = 0; i < nLen; i++, pCoeff += 2)
	{
		nSum2 = nSum + nNewLen;
		if(nSum2 > nLen)
		{
			*pCoeff = ((nLen - nSum) << 12) / nDenom;
			pCoeff[1] = ((nSum2 - nLen) << 12) / nDenom;
			nSum2 -= nLen;
		}
		else
		{
			*pCoeff = nNorm;
			if(nSum2 == nLen)
			{
				pCoeff[1] = -1;
				nSum2 = 0;
			}
		}
		nSum = nSum2;
	}

	return pRes;
}

///////////////////////////////////////////////////////////

void ShrinkDataInt(BYTE *pInBuff, 
				   WORD wWidth, 
				   WORD wHeight,
				   BYTE *pOutBuff, 
				   WORD wNewWidth, 
				   WORD wNewHeight)
{
	BYTE  *pLine = pInBuff, *pPix;
	BYTE  *pOutLine = pOutBuff;
	DWORD dwInLn = (3 * wWidth + 3) & ~3;
	DWORD dwOutLn = (3 * wNewWidth + 3) & ~3;
	int   x, y, i, ii;
	BOOL  bCrossRow, bCrossCol;
	int   *pRowCoeff = CreateCoeffInt(wWidth, 
		wNewWidth, 
		TRUE);
	int   *pColCoeff = CreateCoeffInt(wHeight, 
		wNewHeight, 
		TRUE);
	int   *pXCoeff, *pYCoeff = pColCoeff;
	DWORD dwBuffLn = 3 * wNewWidth * sizeof(DWORD);
	DWORD *pdwBuff = new DWORD[6 * wNewWidth];
	DWORD *pdwCurrLn = pdwBuff, 
		*pdwCurrPix, 
		*pdwNextLn = pdwBuff + 3 * wNewWidth;
	DWORD dwTmp, *pdwNextPix;

	::ZeroMemory(pdwBuff, 2 * dwBuffLn);

	y = 0;
	while(y < wNewHeight)
	{
		pPix = pLine;
		pLine += dwInLn;

		pdwCurrPix = pdwCurrLn;
		pdwNextPix = pdwNextLn;

		x = 0;
		pXCoeff = pRowCoeff;
		bCrossRow = pYCoeff[1] > 0;
		while(x < wNewWidth)
		{
			dwTmp = *pXCoeff * *pYCoeff;
			for(i = 0; i < 3; i++)
				pdwCurrPix[i] += dwTmp * pPix[i];
			bCrossCol = pXCoeff[1] > 0;
			if(bCrossCol)
			{
				dwTmp = pXCoeff[1] * *pYCoeff;
				for(i = 0, ii = 3; i < 3; i++, ii++)
					pdwCurrPix[ii] += dwTmp * pPix[i];
			}
			if(bCrossRow)
			{
				dwTmp = *pXCoeff * pYCoeff[1];
				for(i = 0; i < 3; i++)
					pdwNextPix[i] += dwTmp * pPix[i];
				if(bCrossCol)
				{
					dwTmp = pXCoeff[1] * pYCoeff[1];
					for(i = 0, ii = 3; i < 3; i++, ii++)
						pdwNextPix[ii] += dwTmp * pPix[i];
				}
			}
			if(pXCoeff[1])
			{
				x++;
				pdwCurrPix += 3;
				pdwNextPix += 3;
			}
			pXCoeff += 2;
			pPix += 3;
		}
		if(pYCoeff[1])
		{
			// set result line
			pdwCurrPix = pdwCurrLn;
			pPix = pOutLine;
			for(i = 3 * wNewWidth; i > 0; i--, pdwCurrPix++, pPix++)
				*pPix = ((LPBYTE)pdwCurrPix)[3];

			// prepare line buffers
			pdwCurrPix = pdwNextLn;
			pdwNextLn = pdwCurrLn;
			pdwCurrLn = pdwCurrPix;
			::ZeroMemory(pdwNextLn, dwBuffLn);

			y++;
			pOutLine += dwOutLn;
		}
		pYCoeff += 2;
	}

	delete [] pRowCoeff;
	delete [] pColCoeff;
	delete [] pdwBuff;
} 

///////////////////////////////////////////////////////////

void EnlargeDataInt(BYTE *pInBuff, 
					WORD wWidth, 
					WORD wHeight,
					BYTE *pOutBuff, 
					WORD wNewWidth, 
					WORD wNewHeight)
{
	BYTE  *pLine = pInBuff, 
		*pPix = pLine, 
		*pPixOld, 
		*pUpPix, 
		*pUpPixOld;
	BYTE  *pOutLine = pOutBuff, *pOutPix;
	DWORD dwInLn = (3 * wWidth + 3) & ~3;
	DWORD dwOutLn = (3 * wNewWidth + 3) & ~3;
	int   x, y, i;
	BOOL  bCrossRow, bCrossCol;
	int   *pRowCoeff = CreateCoeffInt(wNewWidth, 
		wWidth, 
		FALSE);
	int   *pColCoeff = CreateCoeffInt(wNewHeight, 
		wHeight, 
		FALSE);
	int   *pXCoeff, *pYCoeff = pColCoeff;
	DWORD dwTmp, dwPtTmp[3];

	y = 0;
	while(y < wHeight)
	{
		bCrossRow = pYCoeff[1] > 0;
		x = 0;
		pXCoeff = pRowCoeff;
		pOutPix = pOutLine;
		pOutLine += dwOutLn;
		pUpPix = pLine;
		if(pYCoeff[1])
		{
			y++;
			pLine += dwInLn;
			pPix = pLine;
		}

		while(x < wWidth)
		{
			bCrossCol = pXCoeff[1] > 0;
			pUpPixOld = pUpPix;
			pPixOld = pPix;
			if(pXCoeff[1])
			{
				x++;
				pUpPix += 3;
				pPix += 3;
			}

			dwTmp = *pXCoeff * *pYCoeff;

			for(i = 0; i < 3; i++)
				dwPtTmp[i] = dwTmp * pUpPixOld[i];

			if(bCrossCol)
			{
				dwTmp = pXCoeff[1] * *pYCoeff;
				for(i = 0; i < 3; i++)
					dwPtTmp[i] += dwTmp * pUpPix[i];
			}

			if(bCrossRow)
			{
				dwTmp = *pXCoeff * pYCoeff[1];
				for(i = 0; i < 3; i++)
					dwPtTmp[i] += dwTmp * pPixOld[i];
				if(bCrossCol)
				{
					dwTmp = pXCoeff[1] * pYCoeff[1];
					for(i = 0; i < 3; i++)
						dwPtTmp[i] += dwTmp * pPix[i];
				}
			}

			for(i = 0; i < 3; i++, pOutPix++)
				*pOutPix = ((LPBYTE)(dwPtTmp + i))[3];

			pXCoeff += 2;
		}
		pYCoeff += 2;
	}

	delete [] pRowCoeff;
	delete [] pColCoeff;
} 

// end src

*/

///////////////////////////////////////////////////////////

// helper function prototypes
static BITMAPINFO *PrepareRGBBitmapInfo(WORD wWidth, WORD wHeight);
static void ShrinkData(BYTE *pInBuff, WORD wWidth, WORD wHeight,
					   BYTE *pOutBuff, WORD wNewWidth, WORD wNewHeight, 
					   WORD wLeft, WORD wTop, WORD wNewImagePitch);
static void EnlargeData(BYTE *pInBuff, WORD wWidth, WORD wHeight,
						BYTE *pOutBuff, WORD wNewWidth, WORD wNewHeight,
						WORD wLeft, WORD wTop, WORD wNewImagePitch);

///////////////////////////////////////////////////////////
// Main resize function

HBITMAP ScaleBitmapDouble(HBITMAP hBmp, WORD wNewWidth, WORD wNewHeight, BOOL bKeepAspectRatio)
{
	BITMAP bmp;
	::GetObject(hBmp, sizeof(BITMAP), &bmp);

	// check for valid size
	if ((bmp.bmWidth > wNewWidth && bmp.bmHeight < wNewHeight) 	|| bmp.bmWidth < wNewWidth 	&& bmp.bmHeight > wNewHeight )
	{
		if( !bKeepAspectRatio )
		{
			bKeepAspectRatio = TRUE;
			//return NULL;
		}
	}

	HDC hDC = ::GetDC(NULL);
	BITMAPINFO *pbi = PrepareRGBBitmapInfo((WORD)bmp.bmWidth, (WORD)bmp.bmHeight);

	BYTE *pData = new BYTE[pbi->bmiHeader.biSizeImage];

	::GetDIBits(hDC, hBmp, 0, bmp.bmHeight, pData, pbi, DIB_RGB_COLORS);

	delete pbi;
	pbi = PrepareRGBBitmapInfo(wNewWidth, wNewHeight);
	BYTE *pData2 = new BYTE[pbi->bmiHeader.biSizeImage];
	memset(pData2,0xFF,pbi->bmiHeader.biSizeImage);

	if( !bKeepAspectRatio )
	{
		if(bmp.bmWidth >= wNewWidth && bmp.bmHeight >= wNewHeight)
			ShrinkData(pData, (WORD)bmp.bmWidth, (WORD)bmp.bmHeight, pData2, wNewWidth, wNewHeight, 0, 0, wNewWidth);
		else
			EnlargeData(pData, (WORD)bmp.bmWidth, (WORD)bmp.bmHeight, pData2, wNewWidth, wNewHeight, 0, 0, wNewWidth);
	}
	else
	{
		float scaleX = wNewWidth / (float)bmp.bmWidth;
		float scaleY = wNewHeight / (float)bmp.bmHeight;

		if( scaleX > scaleY )
		{
			WORD width = (WORD)(bmp.bmWidth*scaleY);
			WORD left = (wNewWidth - width)/2;
			if( scaleY<1 )
				ShrinkData(pData, (WORD)bmp.bmWidth, (WORD)bmp.bmHeight, pData2, width, wNewHeight, left, 0, wNewWidth);
			else
				EnlargeData(pData, (WORD)bmp.bmWidth, (WORD)bmp.bmHeight, pData2, width, wNewHeight, left, 0, wNewWidth);
		}
		else
		{
			WORD height = (WORD)(bmp.bmHeight*scaleX);
			WORD top = (wNewHeight - height)/2;
			if( scaleX<1 )
				ShrinkData(pData, (WORD)bmp.bmWidth, (WORD)bmp.bmHeight, pData2, wNewWidth, height, 0, top, wNewWidth);
			else
				EnlargeData(pData, (WORD)bmp.bmWidth, (WORD)bmp.bmHeight, pData2, wNewWidth, height, 0, top, wNewWidth);
		}
	}

	delete [] pData;

	HBITMAP hResBmp = ::CreateCompatibleBitmap(hDC, wNewWidth, wNewHeight);

	::SetDIBits(hDC, hResBmp, 0, wNewHeight, pData2, pbi, DIB_RGB_COLORS);
	::ReleaseDC(NULL, hDC);

	delete pbi;
	delete [] pData2;

	return hResBmp;
}

///////////////////////////////////////////////////////////

BITMAPINFO *PrepareRGBBitmapInfo(WORD wWidth, WORD wHeight)
{
	BITMAPINFO *pRes = new BITMAPINFO;
	::ZeroMemory(pRes, sizeof(BITMAPINFO));
	pRes->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pRes->bmiHeader.biWidth = wWidth;
	pRes->bmiHeader.biHeight = wHeight;
	pRes->bmiHeader.biPlanes = 1;
	pRes->bmiHeader.biBitCount = 24;
	pRes->bmiHeader.biSizeImage = ((3 * wWidth + 3) & ~3) 
		* wHeight;

	return pRes;
}

///////////////////////////////////////////////////////////

static float *CreateCoeff(int nLen, int nNewLen, BOOL bShrink)
{
	int nSum = 0, nSum2;
	float *pRes = new float[2 * nLen];
	float *pCoeff = pRes;
	float fNorm = (bShrink)? (float)nNewLen / nLen : 1;
	int	  nDenom = (bShrink)? nLen : nNewLen;

	::ZeroMemory(pRes, 2 * nLen * sizeof(float));
	for(int i = 0; i < nLen; i++, pCoeff += 2)
	{
		nSum2 = nSum + nNewLen;
		if(nSum2 > nLen)
		{
			*pCoeff = (float)(nLen - nSum) / nDenom;
			pCoeff[1] = (float)(nSum2 - nLen) / nDenom;
			nSum2 -= nLen;
		}
		else
		{
			*pCoeff = fNorm;
			if(nSum2 == nLen)
			{
				pCoeff[1] = -1;
				nSum2 = 0;
			}
		}
		nSum = nSum2;
	}

	return pRes;
}

///////////////////////////////////////////////////////////

#define F_DELTA		0.0001f

void ShrinkData(BYTE *pInBuff, WORD wWidth, WORD wHeight, 
				BYTE *pOutBuff, WORD wNewWidth, WORD wNewHeight, 
				WORD wLeft, WORD wTop, WORD wNewImagePitch)
{
	BYTE  *pLine = pInBuff, *pPix;
	BYTE  *pOutLine = pOutBuff;
	DWORD dwInLn = (3 * wWidth + 3) & ~3;
	DWORD dwOutLn = (3 * wNewImagePitch + 3) & ~3;
	int   x, y, i, ii;
	BOOL  bCrossRow, bCrossCol;
	float *pRowCoeff = CreateCoeff(wWidth, wNewWidth, TRUE);
	float *pColCoeff = CreateCoeff(wHeight, wNewHeight, TRUE);
	float fTmp, *pXCoeff, *pYCoeff = pColCoeff;
	DWORD dwBuffLn = 3 * wNewWidth * sizeof(float);
	float *fBuff = new float[6 * wNewWidth];
	float *fCurrLn = fBuff, 
		*fCurrPix, 
		*fNextLn = fBuff + 3 * wNewWidth, 
		*fNextPix;

	::ZeroMemory(fBuff, 2 * dwBuffLn);

	y = 0;
	while(y < wNewHeight)
	{
		pPix = pLine;
		pLine += dwInLn;

		fCurrPix = fCurrLn;
		fNextPix = fNextLn;

		x = 0;
		pXCoeff = pRowCoeff;
		bCrossRow = pYCoeff[1] > F_DELTA;
		while(x < wNewWidth)
		{
			fTmp = *pXCoeff * *pYCoeff;
			for(i = 0; i < 3; i++)
				fCurrPix[i] += fTmp * pPix[i];
			bCrossCol = pXCoeff[1] > F_DELTA;
			if(bCrossCol)
			{
				fTmp = pXCoeff[1] * *pYCoeff;
				for(i = 0, ii = 3; i < 3; i++, ii++)
					fCurrPix[ii] += fTmp * pPix[i];
			}

			if(bCrossRow)
			{
				fTmp = *pXCoeff * pYCoeff[1];
				for(i = 0; i < 3; i++)
					fNextPix[i] += fTmp * pPix[i];
				if(bCrossCol)
				{
					fTmp = pXCoeff[1] * pYCoeff[1];
					for(i = 0, ii = 3; i < 3; i++, ii++)
						fNextPix[ii] += fTmp * pPix[i];
				}
			}

			if(fabs(pXCoeff[1]) > F_DELTA)
			{
				x++;
				fCurrPix += 3;
				fNextPix += 3;
			}

			pXCoeff += 2;
			pPix += 3;
		}

		if(fabs(pYCoeff[1]) > F_DELTA)
		{
			// set result line
			fCurrPix = fCurrLn;
			pPix = pOutBuff+(y+wTop)*dwOutLn;
			for(i = wLeft*3; i < 3*(wLeft+wNewWidth); i++)
			{
				pPix[i] = (BYTE)fCurrPix[i-wLeft*3];
			}

			// prepare line buffers
			fCurrPix = fNextLn;
			fNextLn = fCurrLn;
			fCurrLn = fCurrPix;
			::ZeroMemory(fNextLn, dwBuffLn);

			y++;
		}
		pYCoeff += 2;
	}

	delete [] pRowCoeff;
	delete [] pColCoeff;
	delete [] fBuff;
} 

///////////////////////////////////////////////////////////

void EnlargeData(BYTE *pInBuff, WORD wWidth, WORD wHeight,
				 BYTE *pOutBuff, WORD wNewWidth, WORD wNewHeight,
				 WORD wLeft, WORD wTop, WORD wNewImagePitch)
{
	BYTE  *pLine = pInBuff, *pPix = pLine, *pPixOld, *pUpPix, *pUpPixOld;
	BYTE  *pOutLine = pOutBuff, *pOutPix;
	DWORD dwInLn = (3 * wWidth + 3) & ~3;
	DWORD dwOutLn = (3 * wNewImagePitch + 3) & ~3;
	int   x, y, i;
	BOOL  bCrossRow, bCrossCol;
	float *pRowCoeff = CreateCoeff(wNewWidth, wWidth, FALSE);
	float *pColCoeff = CreateCoeff(wNewHeight, wHeight, FALSE);
	float fTmp, fPtTmp[3], *pXCoeff, *pYCoeff = pColCoeff;

	y = 0;
	pOutLine += (dwOutLn*wTop);

	while(y < wHeight)
	{
		bCrossRow = pYCoeff[1] > F_DELTA;
		x = 0;
		pXCoeff = pRowCoeff;
		pOutPix = pOutLine+wLeft*3;
		pOutLine += dwOutLn;
		pUpPix = pLine;
		if(fabs(pYCoeff[1]) > F_DELTA)
		{
			y++;
			pLine += dwInLn;
			pPix = pLine;
		}

		while(x < wWidth)
		{
			bCrossCol = pXCoeff[1] > F_DELTA;
			pUpPixOld = pUpPix;
			pPixOld = pPix;
			if(fabs(pXCoeff[1]) > F_DELTA)
			{
				x++;
				pUpPix += 3;
				pPix += 3;
			}
			fTmp = *pXCoeff * *pYCoeff;
			for(i = 0; i < 3; i++)
				fPtTmp[i] = fTmp * pUpPixOld[i];
			if(bCrossCol)
			{
				fTmp = pXCoeff[1] * *pYCoeff;
				for(i = 0; i < 3; i++)
					fPtTmp[i] += fTmp * pUpPix[i];
			}
			if(bCrossRow)
			{
				fTmp = *pXCoeff * pYCoeff[1];
				for(i = 0; i < 3; i++)
					fPtTmp[i] += fTmp * pPixOld[i];
				if(bCrossCol)
				{
					fTmp = pXCoeff[1] * pYCoeff[1];
					for(i = 0; i < 3; i++)
						fPtTmp[i] += fTmp * pPix[i];
				}
			}
			for(i = 0; i < 3; i++, pOutPix++)
				*pOutPix = (BYTE)fPtTmp[i];
			pXCoeff += 2;
		}
		pYCoeff += 2;
	}

	delete [] pRowCoeff;
	delete [] pColCoeff;
} 

// end src

extern HBITMAP LoadAnImage(char* FileName)
{
	return LoadAnImage_CPP(FileName);
}

extern HBITMAP ScaleBitmap(HBITMAP hBmp, WORD wNewWidth, WORD wNewHeight, BOOL bKeepAspectRatio)
{
	return ScaleBitmapDouble(hBmp, wNewWidth, wNewHeight, bKeepAspectRatio);
}

void EraseWhiteSpaceAtRight(char *str)
{
	int len=strlen(str);
	while( len>0 )
	{ 
		if( str[len-1] == ' ' )
		{
			str[len-1]=0;
			len--;
		}
		else 
			break;
	}
	} 

void reformatName(char *name)
	{ 
	char* leftbrace = strchr(name, '(');
	if( leftbrace != NULL )
		{
		*leftbrace = 0;
		}

	_strlwr(name);

	int len = strlen(name);
	for( int i=0; i<len; i++ )
		{
		if( name[i] < 'A' || (name[i] > 'Z' && name[i] < 'a' ) || name[i] > 'z' )
			name[i] = ' ';
		}

	EraseWhiteSpaceAtRight(name);
}

std::vector<BitmapFileInfo> bitmapFiles;

// Build the image file list
void BuildImageFileList()
{
	if( bitmapFiles.size() == 0 )
	{
		char			romfilename[_MAX_PATH];
		char			drive[_MAX_DRIVE], dir[_MAX_DIR];
		char			filename[_MAX_FNAME], ext[_MAX_EXT];
		char			searchpath[_MAX_PATH];
		char			path[_MAX_PATH];
		HANDLE			findfirst;
		WIN32_FIND_DATA libaa;

		strcpy(path, directories.boxart_directory_to_use);
		strcpy(searchpath, directories.boxart_directory_to_use);
		strcat(searchpath, "*.*");

		findfirst = FindFirstFile(searchpath, &libaa);
		if(findfirst == INVALID_HANDLE_VALUE)
		{
			return;
		}

		SetStatusBarText(0, TranslateStringByString("Updating Box Art Image File List ..."));
		do
		{
			strcpy(romfilename, path);
			strcat(romfilename, libaa.cFileName);
			_splitpath(romfilename, drive, dir, filename, ext);
			_strlwr(ext);	/* Convert file extension to lower case */

			if( stricmp(ext, ".bmp") == 0 || stricmp(ext, ".jpg") == 0 || stricmp(ext, ".gif") == 0	)
			{
				BitmapFileInfo info;
				strcpy(info.fullPath, romfilename);
				strcpy(info.filename, filename);
				strcpy(info.shortname, filename);
				reformatName(info.shortname);
				bitmapFiles.push_back(info);
			}
		} while(FindNextFile(findfirst, &libaa));
	}
}
char seps[] = " ,()[]\'\"&!-";

int MatchGameTo1ImageFile(int idx, char *gamename, char *romfilename, char *shortname, char *title, int countrycode)
{
	// Going to use the gamename, romfilename and gametitle to match to image filenames
	// romfilename does not have extension

	BitmapFileInfo &info = bitmapFiles[idx];

	int res = 0;

	if( strcmp(info.shortname, shortname) == 0 )
		res += 1000;
	else if( strcmp(info.shortname, gamename) == 0 )
		res += 1000;
	else
	{

		if( strstr(romfilename, info.filename) != NULL )
			res += 100;
		
		if( strstr(info.filename, romfilename) != NULL )
			res += 100;

		// Matching string token
		char wholename[_MAX_PATH];
		strcpy(wholename, shortname);
		char *token;
		token = strtok( wholename, seps );
		while( token != NULL )
		{
			// this token
			if( strstr(info.shortname,token) != NULL )
				res += strlen(token);

			// Go to the next token
			token = strtok( NULL, seps );
		}

				// Matching string token
		strcpy(wholename, info.shortname);
		token = strtok( wholename, seps );
		while( token != NULL )
		{
			// this token
			if( strstr(shortname, token) != NULL )
				res += strlen(token);

			// Go to the next token
			token = strtok( NULL, seps );
		}
	}

	// Matching the country code
	int tv_system;
	char country_name[30];
	CountryCodeToCountryName_and_TVSystem(countrycode, country_name, &tv_system);
	char str1[10],str2[10],str3[10],str4[10];
	sprintf(str1,"[%c",country_name[0]); _strlwr(str1);
	sprintf(str2,"(%c",country_name[0]); _strlwr(str2);
	sprintf(str3,"%c]",country_name[0]); _strlwr(str3);
	sprintf(str4,"%c)",country_name[0]); _strlwr(str4);

	if( strstr(str1,romfilename) != NULL )
		res += 50;
	else if( strstr(str2,romfilename) != NULL )
		res += 50;
	else if( strstr(str3,romfilename) != NULL )
		res += 50;
	else if( strstr(str4,romfilename) != NULL )
		res += 50;


	return res;
}

int MatchGameToImageFiles(char *gamename, char *romfilename, char *shortname, char *title, int countrycode)
{
	int numOfImages = bitmapFiles.size();
	int i;
	int *matchings = new int[numOfImages];
	memset(matchings,0,sizeof(int)*numOfImages);

	for( i=0; i<numOfImages; i++ )
	{
		matchings[i] = MatchGameTo1ImageFile(i, gamename, romfilename, shortname, title, countrycode);
	}

	int maxMatch = 0;
	int maxMatchIdx = -1;
	for( i=0; i<numOfImages; i++ )
	{
		if( matchings[i] > maxMatch )
		{
			maxMatchIdx = i;
			maxMatch = matchings[i];
		}
	}

	delete [] matchings;

	if( maxMatchIdx < 0 )
	{
		// Cannot find any matched image file
		return -1;
	}
	else
	{
		TRACE2("Match: %s to %s",romfilename, bitmapFiles[maxMatchIdx].filename);
		return maxMatchIdx;
	}
}

// Find the image according to the game name and game filename
int GetImageFileIndex(int index)
{
	INI_ENTRY &ini = *(romlist[index]->pinientry);
	ROMLIST_ENTRY &rom = *romlist[index];

	char drive[_MAX_DRIVE], dir[_MAX_DIR];
	char filename[_MAX_FNAME], ext[_MAX_EXT];
	char shortname[_MAX_FNAME];

	_splitpath(rom.romfilename, drive, dir, filename, ext);
	strcpy(shortname, filename);
	reformatName(shortname);
	_strlwr(filename);

	return MatchGameToImageFiles(ini.Game_Name, filename, shortname, ini.Alt_Title, ini.countrycode);
}

// Build the ImageList
// Destroy the image list and the image-game map
std::vector<BitmapIdxMap> bitmapIdxMaps;

int FindInBitmapMaps(int index)
{
	INI_ENTRY &ini = *(romlist[index]->pinientry);

	int count = bitmapIdxMaps.size();
	for( int i=0; i<count; i++ )
	{
		if( bitmapIdxMaps[i].crc1 == ini.crc1 && bitmapIdxMaps[i].crc2 == ini.crc2 )
		{
			return i;
		}
	}

	return -1;
}

extern void ClearImageListMap(void)
{
	bitmapIdxMaps.clear();
	TRACE0("Clear the Image List Map");
}

extern void RemoveImageListMap(int index)
{
	int imageListMapIdx = FindInBitmapMaps(index);
	if( imageListMapIdx >= 0 )
	{
		bitmapIdxMaps[imageListMapIdx].crc1 = 0xFFFFFFFF;
		bitmapIdxMaps[imageListMapIdx].crc2 = 0xFFFFFFFF;
	}
}

extern int GetFullBitmapIndex(int index)
{
	int imageListMapIdx=0;
	int idx=0;
	char msg[256];

	BuildImageFileList();

	sprintf(msg, "%s for %s", TranslateStringByString("Loading Box Art Images ..."), romlist[index]->pinientry->Game_Name);
	SetStatusBarText(0, msg);
	imageListMapIdx = FindInBitmapMaps(index);
	if( imageListMapIdx >= 0 )
	{
		return bitmapIdxMaps[imageListMapIdx].idxInImageList;
	}
	else
	{
		int imageFileIdx=0;
		HBITMAP bitmap2=NULL, bitmap=NULL;
		INI_ENTRY &ini = *(romlist[index]->pinientry);
		BitmapIdxMap map;
		char file[_MAX_PATH] = {0};
		BOOL bLoadFromFile=TRUE;

		if( strlen(ini.iconFilename) >= 0 && PathFileExists(ini.iconFilename) )
		{
			strcpy(file, ini.iconFilename);
		}
		else
		{
			imageFileIdx = GetImageFileIndex(index);
			if( imageFileIdx >= 0 && PathFileExists(bitmapFiles[imageFileIdx].fullPath) )
			{
				strcpy(file, bitmapFiles[imageFileIdx].fullPath);
			}
			else
			{
				bLoadFromFile = FALSE;
			}
		}

		if( bLoadFromFile )
		{
			try {
				bitmap = LoadAnImage(file);
				if( bitmap != NULL )
				{
					bitmap2 = ScaleBitmap(bitmap,rlstatus.iconWidth, rlstatus.iconHeight, TRUE);
				}
				else
				{
					bitmap2 = NULL;
				}
			}
			catch(...)
			{
				TRACE1("Exception when loading bitmap file: %s", file);
				if( bitmap2 != NULL ) DeleteObject(bitmap2);
				if( bitmap != NULL ) DeleteObject(bitmap);
				bitmap = NULL;
				bitmap2 = NULL;
			}
		}
		else
		{
			bitmap = NULL;
			bitmap2 = NULL;
		}

		if( bitmap2 == NULL )
		{
			bitmap = LoadBitmap(gui.hInst,MAKEINTRESOURCE(IDB_N64LOGO));
			bitmap2 = ScaleBitmap(bitmap,rlstatus.iconWidth, rlstatus.iconHeight, TRUE);
		}

		//idx = ImageList_Add(rlstatus.hLargeBitmaps, bitmap2, NULL );
		idx = ImageList_Add(rlstatus.hLargeBitmaps, bitmap2, NULL );
		map.crc1 = ini.crc1;
		map.crc2 = ini.crc2;
		map.idxInImageList = idx;
		map.bitmapIndex = imageFileIdx;
		bitmapIdxMaps.push_back(map);

		if( idx < 0 )
		{
			TRACE0("Error to load image");
		}

		if( bitmap2 != NULL ) DeleteObject(bitmap2);
		if( bitmap != NULL ) DeleteObject(bitmap);
	}

	return idx;
}


