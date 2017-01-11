/* config.h
 * Define or comment macros in this header to configure the entire emulator.
 * If compiling with gcc or a compatible compiler, it will try to auto-detect 
 * the settings.
 *
 * Copyright (C) 2017 Lin Ke-Fong
 * 
 * This code is free, you may do whatever you want with it.
 */

#ifndef __CONFIG_INCLUDED__
#define __CONFIG_INCLUDED__

/* Define if your processor is big endian (example: ARM processors). */

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN___
#define IS_BIG_ENDIAN
#endif

/* The VDP emulator can take advantage of 64-bit to move data around faster. */

#if defined(__LP64__) && __LP64__ == 1
#define IS_64BIT
#endif

/* Some processor may not allow unaligned memory access altogether, or doing so
 * may incur a heavy penalty.
 */

#if defined(__x86_64__) || defined(__i386__)
#define ALLOW_UNALIGNED_MEMORY_ACCESS
#endif

#endif
