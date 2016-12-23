#ifndef __COMMON_INCLUDED__
#define __COMMON_INCLUDED__

#ifdef _MSC_VER

typedef unsigned __int8		uint8_t;
typedef unsigned __int16	uint16_t;
typedef unsigned __int32	uint32_t;

#ifdef IS_64BIT
typedef unsigned __int64	uint64_t;
#endif

#else

#include <stdint.h>

#endif

void	Warning (const char *message, ...);
void	Fatal (const char *message, ...);
void	FatalCantOpenFile (void);

#endif
