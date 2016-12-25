#ifndef __COMMON_INCLUDED__
#define __COMMON_INCLUDED__

/* ARM processors are big endian. */

/* #define IS_BIG_ENDIAN */

/* x86 processors allow non-aligned memory access. */

#define ALLOW_UNALIGNED_MEMORY_ACCESS

/* The VDP renderer can take advantage of a 64-bit processor. */

#define IS_64BIT

/* If compiling with gcc or a compatible compiler, try to auto-detect the 
 * configuration macros.
 */

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN___
#define IS_BIG_ENDIAN
#endif

#if defined(__x86_64__) || defined(__i386__)
#define ALLOW_UNALIGNED_MEMORY_ACCESS
#endif

#ifdef __LP64__
#define IS_64BIT
#endif

#include <stdint.h>

void	Warning (const char *message, ...);
void	Fatal (const char *message, ...);
void	FatalCantOpenFile (void);

#endif
