#ifndef __RSP_PREFIX__
#define __RSP_PREFIX__

#ifndef BIG_ENDIAN
	#include <windows.h>
#endif // BIG_ENDIAN

// file api and fix function for path
#include <stdio.h>
FILE *File_Open(char *name, char *mode);

#ifndef BIG_ENDIAN

// unsigned types
#define _u64   unsigned __int64   /* 64 bit */
#define _u32   unsigned __int32   /* 32 bit */
#define _u16   unsigned __int16   /* 16 bit */
#define _u8	   unsigned __int8    /*  8 bit */

// signed types
#define _s64   __int64   /* 64 bit */
#define _s32   __int32   /* 32 bit */
#define _s16   __int16   /* 16 bit */
#define _s8	   __int8    /*  8 bit */

#endif // BIG_ENDIAN

#endif // __RSP_PREFIX__
