#include <windows.h> 
#include <ddraw.h> 
#include <stdio.h>
#include "gfx.h"

LPDIRECTDRAW2			lpDD;
LPDIRECTDRAWSURFACE     DD_Primary;
LPDIRECTDRAWSURFACE     DD_CFB;
LPDIRECTDRAWCLIPPER     lpDDClipper;

int      PixelFormat, FullScreen, ChangingWindow = FALSE;
short    Convet16to16[65536];
long     Convet16to32[65536];
DWORD    ViStatus, ViWidth;
RECT     rcWindow;

// ---------------- Needed for RSP --------------------------

extern "C" {
	GFX_INFO GFXInfo;
	extern char *pRDRAM;
	extern char *pDMEM;
	extern char *pIMEM;
	int DoOnce;
//	int ucodeDetected;
	int MaxDumpCount;
	int MinDumpCount;
	extern void rsp_run();
	extern void rsp_reset();


	extern BOOL Internal_VIDEO_InitiateGFX(GFX_INFO Gfx_Info);
	extern void Internal_VIDEO_RomOpen(void);
	extern void Hack();
	extern void ExternGlobalInitDSP();
	extern void ExternDSPBuildCommandList();
}

void ChangeWinSize ( HWND hWnd, long width, HWND hStatusBar );
void DisplayError  ( HWND hParent, char * Message, ...);
void SetupCFB      ( void );

unsigned short Flip16 (unsigned short ToFlip) {
	__asm {
		mov ax,ToFlip
		xchg ah,al
		mov ToFlip,ax
	}
	return ToFlip;
}

void __cdecl CloseDLL (void) {
	if (FullScreen) { ChangeWindow(); }
    
	return;
	if ( lpDD != NULL ) {
        if ( DD_Primary != NULL ) 
		{
            DD_Primary->Release();
            DD_Primary = NULL;
        }
		if (DD_CFB != NULL) {
			DD_CFB->Release();
			DD_CFB = NULL;
		}
        if (lpDDClipper) {
			lpDDClipper->Release();
			lpDDClipper = NULL;
		}
		lpDD->Release();
        lpDD = NULL;
	}
}

void __cdecl ChangeWindow (void) {
	static WINDOWPLACEMENT wndpl;
	static HMENU OldMenu = NULL;
	static LONG OldStlye = 0;
    DDSURFACEDESC       ddsd;

	return;
	
	ChangingWindow = TRUE;
	if (FullScreen==false) {//GDI->Full
		FullScreen = true;
		
		wndpl.length = sizeof(wndpl);
		GetWindowPlacement( GFXInfo.hWnd, &wndpl);
		if (GFXInfo.hStatusBar) { ShowWindow(GFXInfo.hStatusBar,SW_HIDE); }
		OldMenu = GetMenu(GFXInfo.hWnd);
		if (OldMenu) { SetMenu(GFXInfo.hWnd,NULL); }
		OldStlye = GetWindowLong(GFXInfo.hWnd,GWL_STYLE);
		SetWindowLong(GFXInfo.hWnd, GWL_STYLE, WS_VISIBLE);
		SetWindowPos(GFXInfo.hWnd, HWND_TOPMOST, 0, 0, 640, 480, SWP_SHOWWINDOW);
		ShowCursor(FALSE);
		if (DD_Primary) { 
			DD_Primary->Release();
			DD_Primary = NULL;
		}
		if (lpDDClipper) { 
			lpDDClipper->Release();
			lpDDClipper = NULL;
		}
		lpDD->SetCooperativeLevel(GFXInfo.hWnd,DDSCL_ALLOWREBOOT|DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN);
		lpDD->SetDisplayMode( 640, 480, 16, NULL, NULL);
	    
		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	    if ( FAILED(lpDD->CreateSurface( &ddsd, &DD_Primary, NULL ))) {
		    ChangeWindow();
			return;
		}
		SetupCFB();
	} else {
		FullScreen = false;
		if (DD_Primary) { 
			DD_Primary->Release();
			DD_Primary = NULL;
		}
		lpDD->SetCooperativeLevel(GFXInfo.hWnd,DDSCL_NORMAL);
		lpDD->RestoreDisplayMode();

		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		if ( FAILED(lpDD->CreateSurface( &ddsd, &DD_Primary, NULL ))) {
			CloseDLL();
		}
		

		if ( FAILED( lpDD->CreateClipper( 0, &lpDDClipper, NULL ) ) ) {
			CloseDLL();
		}

		if ( FAILED( lpDDClipper->SetHWnd( 0, GFXInfo.hWnd ) ) ) {
			CloseDLL();
		}

		if ( FAILED( DD_Primary->SetClipper( lpDDClipper ) ) ) {
			CloseDLL();
		}
		SetupCFB();

		if (GFXInfo.hStatusBar) { ShowWindow(GFXInfo.hStatusBar,SW_SHOW); }
		if (OldMenu) { 
			SetMenu(GFXInfo.hWnd,OldMenu); 
			OldMenu = NULL;
		}
		SetWindowLong(GFXInfo.hWnd, GWL_STYLE, OldStlye);
		SetWindowPos(GFXInfo.hWnd, HWND_NOTOPMOST, wndpl.rcNormalPosition.left, 
			wndpl.rcNormalPosition.top, 640, 480, SWP_NOSIZE|SWP_SHOWWINDOW);
		ChangeWinSize(GFXInfo.hWnd,*GFXInfo.VI_WIDTH_REG==0?640:*GFXInfo.VI_WIDTH_REG, GFXInfo.hStatusBar);
		ShowCursor(TRUE);
	}
	ChangingWindow = FALSE;
}

void ChangeWinSize ( HWND hWnd, long width, HWND hStatusBar ) {
	WINDOWPLACEMENT wndpl;
    RECT rc1, swrect;

	if (FullScreen) { return; }
	wndpl.length = sizeof(wndpl);
	GetWindowPlacement( hWnd, &wndpl);

	if ( hStatusBar != NULL ) {
		GetClientRect( hStatusBar, &swrect );
	    SetRect( &rc1, 0, 0, width, (width >> 2) * 3 + swrect.bottom );
	} else {
	    SetRect( &rc1, 0, 0, width, (width >> 2) * 3 );
	}


    AdjustWindowRectEx( &rc1,GetWindowLong( hWnd, GWL_STYLE ),
		GetMenu( hWnd ) != NULL, GetWindowLong( hWnd, GWL_EXSTYLE ) ); 

    MoveWindow( hWnd, wndpl.rcNormalPosition.left, wndpl.rcNormalPosition.top, 
		rc1.right - rc1.left, rc1.bottom - rc1.top, TRUE );
    
	MoveScreen(wndpl.rcNormalPosition.left, wndpl.rcNormalPosition.top);
   
}

void DisplayError (HWND hParent, char * Message, ...) {
	char Msg[400];
	va_list ap;

	va_start( ap, Message );
	vsprintf( Msg, Message, ap );
	va_end( ap );
	MessageBox(hParent,Msg,"Error",MB_OK|MB_ICONERROR|MB_SETFOREGROUND );
}

void __cdecl DllAbout ( HWND hParent) {
	DisplayError (hParent,"Basic CFB plugin by zilmar");	
}

void __cdecl DrawScreen (void) {
	if (ChangingWindow) { return; }
	if (DD_CFB == NULL) { return; }
	if (DD_Primary == NULL) { return; }
	DD_Primary->Blt( &rcWindow,DD_CFB,NULL,DDBLT_WAIT, NULL);
}

void __cdecl GetDllInfo ( PLUGIN_INFO * PluginInfo ) {
	PluginInfo->Version = 0x0102;
	PluginInfo->Type = PLUGIN_TYPE_GFX;
	sprintf(PluginInfo->Name,"Basic CFB plugin by zilmar");
	PluginInfo->NormalMemory = TRUE;
	PluginInfo->MemoryBswaped = TRUE;
}

extern "C" 
{
	void rdp_enddl(int val)
	{
		*GFXInfo.DPC_END_REG = val;
		ExternDSPBuildCommandList();
	}

	void DPCStatus(unsigned __int32 reg)
	{
	/* DPC_STATUS_REG read bits */
#define DPC_STATUS_XBUS_DMEM_DMA	0x0000001
#define DPC_STATUS_FREEZE			0x0000002
#define DPC_STATUS_FLUSH			0x0000004

/* DPC_STATUS_REG write bits */
#define DPC_CLR_XBUS_DMEM_DMA	0x0000001
#define DPC_SET_XBUS_DMEM_DMA	0x0000002
#define DPC_CLR_FREEZE			0x0000004
#define DPC_SET_FREEZE			0x0000008
#define DPC_CLR_FLUSH			0x0000010
#define DPC_SET_FLUSH			0x0000020
#define DPC_CLR_TMEM_REG		0x0000040
#define DPC_CLR_PIPEBUSY_REG	0x0000080
#define DPC_CLR_BUFBUSY_REG		0x0000100
#define DPC_CLR_CLOCK_REG		0x0000200

	if ( ( reg & DPC_CLR_XBUS_DMEM_DMA ) != 0) { *GFXInfo.DPC_STATUS_REG &= ~DPC_STATUS_XBUS_DMEM_DMA; }
	if ( ( reg & DPC_SET_XBUS_DMEM_DMA ) != 0) { *GFXInfo.DPC_STATUS_REG |= DPC_STATUS_XBUS_DMEM_DMA;  }
	if ( ( reg & DPC_CLR_FREEZE ) != 0) { *GFXInfo.DPC_STATUS_REG &= ~DPC_STATUS_FREEZE; }
	if ( ( reg & DPC_SET_FREEZE ) != 0) { *GFXInfo.DPC_STATUS_REG |= DPC_STATUS_FREEZE;  }		
	if ( ( reg & DPC_CLR_FLUSH ) != 0) { *GFXInfo.DPC_STATUS_REG &= ~DPC_STATUS_FLUSH; }
	if ( ( reg & DPC_SET_FLUSH ) != 0) { *GFXInfo.DPC_STATUS_REG |= DPC_STATUS_FLUSH;  }
//	if ( ( rsp_reg.r[__RT] & DPC_CLR_TMEM_CTR ) != 0) { DisplayError("RSP: DPC_STATUS_REG: DPC_CLR_TMEM_CTR"); }
//	if ( ( rsp_reg.r[__RT] & DPC_CLR_PIPE_CTR ) != 0) { DisplayError("RSP: DPC_STATUS_REG: DPC_CLR_PIPE_CTR"); }
//	if ( ( rsp_reg.r[__RT] & DPC_CLR_CMD_CTR ) != 0) { DisplayError("RSP: DPC_STATUS_REG: DPC_CLR_CMD_CTR"); }
//	if ( ( rsp_reg.r[__RT] & DPC_CLR_CLOCK_CTR ) != 0) { DisplayError("RSP: DPC_STATUS_REG: DPC_CLR_CLOCK_CTR"); }
	}
}




BOOL __cdecl InitiateGFX (GFX_INFO Gfx_Info) {

	GFXInfo = Gfx_Info;
    LPDIRECTDRAW        BasiclpDD;
    DDSURFACEDESC       ddsd;
	DDPIXELFORMAT format;
	unsigned char Red, Green, Blue, Alpha;
	unsigned short Color16;
	unsigned long  Color32;
	int count;
	static int initvariables = 0;

	if (!initvariables)
	{
		pIMEM  = (char*)GFXInfo.IMEM;
		pRDRAM = (char*)GFXInfo.RDRAM;
		pDMEM  = (char*)GFXInfo.DMEM;
		rsp_reset();
	}

	ExternGlobalInitDSP();
//	Internal_VIDEO_InitiateGFX(GFXInfo);

	{
	GFXInfo = Gfx_Info;
    LPDIRECTDRAW        BasiclpDD;
    DDSURFACEDESC       ddsd;
	DDPIXELFORMAT format;
	unsigned char Red, Green, Blue, Alpha;
	unsigned short Color16;
	unsigned long  Color32;
	int count;

	FullScreen = FALSE;
    if ( FAILED(DirectDrawCreate( NULL, &BasiclpDD, NULL ))) {
        CloseDLL();
		return FALSE;
    }

    if ( FAILED(BasiclpDD->SetCooperativeLevel( GFXInfo.hWnd, DDSCL_NORMAL ))) {
        CloseDLL();
		return FALSE;
    }

    if( FAILED(BasiclpDD->QueryInterface( IID_IDirectDraw2, (LPVOID *)&lpDD ))) {
        CloseDLL();
		return FALSE;
	}
	
	BasiclpDD->Release();

    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    if ( FAILED(lpDD->CreateSurface( &ddsd, &DD_Primary, NULL ))) {
        CloseDLL();
		return FALSE;
    }
	

    if ( FAILED( lpDD->CreateClipper( 0, &lpDDClipper, NULL ) ) ) {
        CloseDLL();
		return FALSE;
    }

    if ( FAILED( lpDDClipper->SetHWnd( 0, GFXInfo.hWnd ) ) ) {
        CloseDLL();
		return FALSE;
    }

    if ( FAILED( DD_Primary->SetClipper( lpDDClipper ) ) ) {
        CloseDLL();
		return FALSE;
    }
    
	format.dwSize = sizeof(DDPIXELFORMAT);
	if (DD_Primary->GetPixelFormat(&format) != DD_OK) {
        CloseDLL();
		return FALSE;
    }
	
	PixelFormat = format.dwRGBBitCount;
	switch (PixelFormat) {
	case 8:
		DisplayError(Gfx_Info.hWnd,"Unhandled Pixel Depth 8 bit");
		return FALSE;
		break;
	case 16:
		if ( format.dwGBitMask == 0x07E0 ) {
			for (count = 0; count < 65536; count ++) {
				if (GFXInfo.MemoryBswaped) {
					Color16 = (unsigned short)count;
				} else {
					Color16 = Flip16((unsigned short)count);
				}
				Red   = (Color16 & 0xF800) >> 11;
				Green = (Color16 & 0x07C0) >> 6;
				Blue  = (Color16 & 0x003E) >> 1;
				Alpha = (Color16 & 0x0001);
				Color16  = Red << 11;
				Color16 |= Green << 6;
				Color16 |= Blue;
				*(unsigned short *)(&Convet16to16[count]) = Color16;
			}
		} else {
			for (count = 0; count < 65536; count ++) {
				if (GFXInfo.MemoryBswaped) {
					Color16 = (unsigned short)count;
				} else {
					Color16 = Flip16((unsigned short)count);
				}
				Red   = (Color16 & 0xF800) >> 11;
				Green = (Color16 & 0x07C0) >> 6;
				Blue  = (Color16 & 0x003E) >> 1;
				Alpha = (Color16 & 0x0001);
				Color16  = Red << 10;
				Color16 |= Green << 5;
				Color16 |= Blue;
				*(unsigned short *)(&Convet16to16[count]) = Color16;
			}
		}
		break;
	case 24:
		DisplayError(Gfx_Info.hWnd,"Unhandled Pixel Depth 24 bit");
		return FALSE;
		break;
	case 32:
		for (count = 0; count < 65536; count ++) {
			if (GFXInfo.MemoryBswaped) {
				Color16 = (unsigned short)count;
			} else {
				Color16 = Flip16((unsigned short)count);
			}
			Color16 = (unsigned short)count;
			Red   = (Color16 & 0xF800) >> 11;
			Green = (Color16 & 0x07C0) >> 6;
			Blue  = (Color16 & 0x003E) >> 1;
			Alpha = (Color16 & 0x0001);
			Color32  = Alpha << 27;
			Color32 |= Red << 19;
			Color32 |= Green << 11;
			Color32 |= Blue << 3;
			*(unsigned long *)(&Convet16to32[count]) = Color32;
		}
		break;
	}
	
	DD_CFB  = NULL;
	return TRUE;	
	}

	return TRUE;
}

void __cdecl MoveScreen (int xpos, int ypos) {
    RECT swrect;
	
	GetClientRect( GFXInfo.hWnd, &rcWindow );
    ClientToScreen( GFXInfo.hWnd, ( LPPOINT )&rcWindow );
    ClientToScreen( GFXInfo.hWnd, ( LPPOINT )&rcWindow + 1 );
	if ( GFXInfo.hStatusBar != NULL && !FullScreen) {	
		GetClientRect( GFXInfo.hStatusBar, &swrect );
		rcWindow.bottom = rcWindow.bottom - swrect.bottom;
	}
}

	extern "C" {BOOL rsp_step_count(void);}
void Update();

void __cdecl ProcessDList (void) {

	while (rsp_step_count()); //return;
	Hack();
	Update();
	//GFXInfo.CheckInterrupts();
}

void __cdecl RomClosed (void) {
}

void __cdecl RomOpen (void) {
	Internal_VIDEO_RomOpen();
}

//begin mupen stuff
void SetupCFB (void ) {
    DDSURFACEDESC       ddsd;

	if (*GFXInfo.VI_STATUS_REG == 0) { return; }
	if (*GFXInfo.VI_WIDTH_REG == 0) { return; }

	if (DD_CFB != NULL) {
		DD_CFB->Release();
        DD_CFB = NULL;
	}

	if ( lpDD == NULL ) { return; }

    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY ;
    ddsd.dwWidth = *GFXInfo.VI_WIDTH_REG ;
    ddsd.dwHeight = (*GFXInfo.VI_WIDTH_REG >> 2) * 3;
 
    if ( lpDD->CreateSurface( &ddsd, &DD_CFB, NULL ) != DD_OK ) {
        DD_CFB = NULL;
        CloseDLL();
		return;
    }
}

class Color16
{
   float r;
   float g;
   float b;
   float a;
   
 public:
   Color16() {}
   ~Color16() {}
   
   Color16(const Color16 &c)
     {
	r = c.r;
	g = c.g;
	b = c.b;
	a = c.a;
     }
   
   Color16 operator*(float n)
     {
	Color16 temp;
	temp.r = n*r;
	temp.g = n*g;
	temp.b = n*b;
	return temp;
     }
   
   Color16 operator+(const Color16& c)
     {
	Color16 temp;
	temp.r = r+c.r;
	temp.g = g+c.g;
	temp.b = b+c.b;
	return temp;
     }
   
   Color16& operator=(int c)
     {
	r = (c >> 11) & 0x1F;
	g = (c >> 6) & 0x1F;
	b = (c >> 1) & 0x1F;
	a = c&1;
	return *this;
     }
   
   Color16& operator=(short c)
     {
	r = (c >> 11) & 0x1F;
	g = (c >> 6) & 0x1F;
	b = (c >> 1) & 0x1F;
	a = c&1;
	return *this;
     }
   
   operator int()
     {
	return ((int)r << 11) | ((int)g << 6) | ((int)b << 1);
     }
};
#define S16 1

    DDSURFACEDESC       ddsd;
void updateScreen()
{
	short* SurfBuf = (short*)ddsd.lpSurface;
//   if (!bpp) return;
   if (!*GFXInfo.VI_WIDTH_REG) return;
   int h_end = *GFXInfo.VI_H_START_REG & 0x3FF;
   int h_start = (*GFXInfo.VI_H_START_REG >> 16) & 0x3FF;
   int v_end = *GFXInfo.VI_V_START_REG & 0x3FF;
   int v_start = (*GFXInfo.VI_V_START_REG >> 16) & 0x3FF;
   float scale_x = ((int)*GFXInfo.VI_X_SCALE_REG & 0xFFF) / 1024.0f;
   float scale_y = (((int)*GFXInfo.VI_Y_SCALE_REG & 0xFFF)>>1) / 1024.0f;
   
   short *im16 = (short*)((char*)GFXInfo.RDRAM +
			  (*GFXInfo.VI_ORIGIN_REG & 0x7FFFFF));
   
	short *buf16 = (short*)ddsd.lpSurface; //schibo modified. used to be getScreenPointer();
	

   int minx = (640-(h_end-h_start))/2;
   int maxx = 640-minx;
   int miny = (480-(v_end-v_start))/2;
   int maxy = 480-miny;
   float px, py;
   py=0;
   if ((*GFXInfo.VI_STATUS_REG & 0x30) == 0x30) // not antialiased
     {
	for (int j=0; j<480; j++)
	  {
	     if (j < miny || j > maxy)
	       for (int i=0; i<640; i++)
		 buf16[j*640+i] = 0;
	     else
	       {
		  px=0;
		  for (int i=0; i<640; i++)
		    {
		       if (i < minx || i > maxx)
			 buf16[j*640+i] = 0;
		       else
			 {
			    buf16[j*640+i] = 
			      im16[((int)py*(*GFXInfo.VI_WIDTH_REG)+(int)px)^S16]>>1;
			    px += scale_x;
			 }
		    }
		  py += scale_y;
	       }
	  }
     }
   else
     {
	for (int j=0; j<480; j++)
	  {
	     if (j < miny || j > maxy)
	       for (int i=0; i<640; i++)
		 buf16[j*640+i] = 0;
	     else
	       {
		  px=0;
		  for (int i=0; i<640; i++)
		    {
		       if (i < minx || i > maxx)
			 buf16[j*640+i] = 0;
		       else
			 {
			    bool xint = (px - (int)px) == 0.0f, yint = (py - (int)py) == 0.0f;
			    if (xint && yint)
			      {
				 buf16[j*640+i] = 
				   im16[((int)py*(*GFXInfo.VI_WIDTH_REG)+(int)px)^S16]>>1;
			      }
			    else if (yint)
			      {
				 Color16 l,r;
				 int w = *GFXInfo.VI_WIDTH_REG;
				 l=im16[((int)py*w+(int)px)^S16];
				 r=im16[((int)py*w+(int)(px+1.0f))^S16];
				 buf16[j*640+i] = 
				   (int)(l*(1.0f-(px-(int)px))+r*(px-(int)px))>>1;
			      }
			    else if (xint)
			      {
				 Color16 t,b;
				 int w = *GFXInfo.VI_WIDTH_REG;
				 t=im16[((int)py*w+(int)px)^S16];
				 b=im16[((int)(py+1)*w+(int)px)^S16];
				 buf16[j*640+i] = 
				   (int)(t*(1-(py-(int)py))+b*(py-(int)py))>>1;
			      }
			    else
			      {
				 Color16 t,b,l,r;
				 int w = *GFXInfo.VI_WIDTH_REG;
				 l=im16[((int)py*w+(int)px)^S16];
				 r=im16[((int)py*w+(int)(px+1))^S16];
				 t=l*(1-(px-(int)px))+r*(px-(int)px);
				 l=im16[((int)(py+1)*w+(int)px)^S16];
				 r=im16[((int)(py+1)*w+(int)(px+1))^S16];
				 b=l*(1-(px-(int)px))+r*(px-(int)px);
				 buf16[j*640+i] = 
				   (int)(t*(1-(py-(int)py))+b*(py-(int)py))>>1;
			      }
			    px += scale_x;
			 }
		    }
		  py += scale_y;
	       }
	  }
     }
//   blit();
}
//end mupen stuff

void Update (void) {

	DWORD count, x;
	short* SurfBuf, *buffer;
	
	if (DD_CFB == NULL) { return; }
	
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_LPSURFACE | DDSD_PITCH;
	if (DD_CFB->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL) != DD_OK) {
		//DisplayError(GFXInfo.hWnd,"Failed To lock Surface");
		return;
	}

updateScreen();

	SurfBuf = (short*)ddsd.lpSurface;


    if(PixelFormat == 32)
    {
      unsigned char *surf = (unsigned char *)SurfBuf;
      
      for (int i=0; i<640*480; i++)
      {
        surf[640*480*4-(i+1)*4+2] = (SurfBuf[640*480-(i+1)] & 0x7C00) >> 7;
        surf[640*480*4-(i+1)*4+1] = (SurfBuf[640*480-(i+1)] & 0x03E0) >> 2;
        surf[640*480*4-(i+1)*4+0] = (SurfBuf[640*480-(i+1)] & 0x001F) << 3;
      } 
    }
    else
    {    
      for (int i=0;i<640*480;i++)
      {
        SurfBuf[i] = ((SurfBuf[i] << 1) &~ 0x1F ) | (SurfBuf[i] & 0x1F);
      }
    }

/*
	if (PixelFormat == 16) {
		for ( count = 0 ; count < ((*GFXInfo.VI_WIDTH_REG >> 2) * 3); count ++ ) {
			for ( x = 0; x < (*GFXInfo.VI_WIDTH_REG << 1); x += 4, buffer+=4 ) {
				if (GFXInfo.MemoryBswaped) {
					*(unsigned short *)(SurfBuf + x) = Convet16to16[*(unsigned short *)(buffer + 2)];	
					*(unsigned short *)(SurfBuf + x + 2) = Convet16to16[*(unsigned short *)buffer];	
				} else {
					*(unsigned short *)(SurfBuf + x) = Convet16to16[*(unsigned short *)buffer];	
					*(unsigned short *)(SurfBuf + x + 2) = Convet16to16[*(unsigned short *)(buffer + 2)];	
				}
			}
			SurfBuf+=ddsd.lPitch;
		}
	} else {
		for ( count = 0 ; count < ((*GFXInfo.VI_WIDTH_REG >> 2) * 3); count ++ ) {
			for ( x = 0; x < (*GFXInfo.VI_WIDTH_REG << 2); x += 8, buffer+=4 ) {
				if (GFXInfo.MemoryBswaped) {
					*(unsigned long *)(SurfBuf + x) = Convet16to32[*(unsigned short *)(buffer + 2)];	
					*(unsigned long *)(SurfBuf + x + 4) = Convet16to32[*(unsigned short *)buffer];	
				} else {
					*(unsigned long *)(SurfBuf + x) = Convet16to32[*(unsigned short *)buffer];	
					*(unsigned long *)(SurfBuf + x + 4) = Convet16to32[*(unsigned short *)(buffer + 2)];	
				}
			}
			SurfBuf+=ddsd.lPitch;
		}
	}
*/
	DD_CFB->Unlock(ddsd.lpSurface);
	DrawScreen();
}

void __cdecl ViStatusChanged (void) {
	if (*GFXInfo.VI_STATUS_REG != ViStatus) {
		SetupCFB();
		ViStatus = *GFXInfo.VI_STATUS_REG; 
	}
}

void __cdecl ViWidthChanged (void) {
	if (*GFXInfo.VI_WIDTH_REG != ViWidth) {
		SetupCFB();
		ViWidth = *GFXInfo.VI_WIDTH_REG;
		ChangeWinSize(GFXInfo.hWnd,ViWidth, GFXInfo.hStatusBar);
	}
}
