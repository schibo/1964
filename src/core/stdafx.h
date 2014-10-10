// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Windows Header Files:
#include <windows.h>

// TODO: reference additional headers your program requires here
#include <process.h>
#include <commdlg.h>
#include <direct.h>
#include <shlobj.h>
#include <shellapi.h>
#include <shlwapi.h>
#include "hardware.h"
#include "globals.h"
#include "debug_option.h"
#include "hle.h"
#include "emulator.h"
#include "r4300i.h"
#include "r4300i_32bit.h"
#include "n64rcp.h"
#include "interrupt.h"
#include "dma.h"
#include "compiler.h"
#include "timer.h"
#include "_memory.h"
#include "1964ini.h"
#include "iPif.h"
#include "dynarec/x86.h"
#include "dynarec/regcache.h"
#include "dynarec/dynaCPU.h"
#include "dynarec/dynaLog.h"
#include "dynarec/dynarec.h"
#include "dynarec/xmm.h"
#include "win32/DLL_Video.h"
#include "win32/DLL_Input.h"
#include "win32/DLL_Rsp.h"
#include "win32/registry.h"
#include "win32/windebug.h"
#include "win32/wingui.h"
#include "romlist.h"
#include "cheatcode.h"
#include "gamesave.h"
#include "fileio.h"
#include "FrameBuffer.h"
#include "kaillera/kaillera.h"
#include "zlib/unzip.h"
#include "Registers.h"

extern N64::CRegisters r;


