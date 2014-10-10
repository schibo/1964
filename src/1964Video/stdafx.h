// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(_STDAFX_H_)
#define _STDAFX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define EXPORT				__declspec(dllexport)
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT 0x0400
#define WINVER 0x0400
#include <stdio.h>
#include <windows.h>
#include <windowsx.h>			// Button_* etc
#include <shlwapi.h>
#include <mmsystem.h>
#include <commctrl.h>
#include <ShellAPI.h>
#include <winnt.h>			// For 32x32To64 multiplies
#include <math.h>			// For sqrt()
#include <iostream>
#include <fstream>
#include <istream>

#if _MSC_VER > 1200
#include "xmmintrin.h"
#endif


#include <process.h>

#include <d3d8.h>
#include <d3dx8.h>
#include <d3d8types.h>
#include <D3dx8math.h>

#include <vector>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_CHECK
#define SAFE_CHECK(a)	if( (a) == NULL ) {ErrorMsg("Creater out of memory"); /*throw new exception();*/}
#endif

#include "typedefs.h"
#include "gfx.h"
#include "Video.h"
#include "Config.h"
#include "resource.h"
#include "Debugger.h"
#include "RSP_S2DEX.h"
#include "RSP_Parser.h"

#include "TextureManager.h"
#include "ConvertImage.h"
#include "Texture.h"
#include "DirectXTexture.h"
#include "OGLTexture.h"

#include "CombinerDefs.h"
#include "DecodedMux.h"
#include "OGLDecodedMux.h"
#include "DirectXDecodedMux.h"

#include "blender.h"
#include "DirectXBlender.h"


#include "combiner.h"
#include "GeneralCombiner.h"
#include "DirectXCombiner.h"
#include "CNvTNTCombiner.h"
#include "CNvTNTDirectXCombiner.h"
#include "DirectXCombinerVariants.h"

#include "gl/gl.h"
#include "glext.h"

#include "GraphicsContext.h"
#include "DXGraphicsContext.h"
#include "DeviceBuilder.h"
#include "OGLGraphicsContext.h"

#include "OGLCombiner.h"
#include "OGLExtCombiner.h"
#include "OGLCombinerNV.h"
#include "OGLCombinerTNT2.h"

#include "RenderBase.h"
#include "ExtendedRender.h"
#include "Render.h"
#include "D3DRender.h"
#include "OGLRender.h"
#include "OGLExtRender.h"

#include "resource.h"

#include "icolor.h"

#include "CSortedList.h"
#include "CritSect.h"
#include "Timing.h"

extern WindowSettingStruct windowSetting;

void __cdecl MsgInfo (char * Message, ...);
void __cdecl ErrorMsg (char * Message, ...);

#define MI_INTR_DP          0x00000020  
#define MI_INTR_SP          0x00000001  

extern uint32 g_dwRamSize;

extern uint32 * g_pRDRAMu32;
extern signed char* g_pRDRAMs8;
extern unsigned char *g_pRDRAMu8;

extern GFX_INFO g_GraphicsInfo;


extern char *project_name;
#endif

