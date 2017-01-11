/* common.h
 * Common types and functions declarations.
 *
 * Copyright (C) 2017 Lin Ke-Fong
 *
 * This code is free, you may do whatever you want with it.
 */

#ifndef __COMMON_INCLUDED__
#define __COMMON_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern void	Warning (const char *message, ...);
extern void	Fatal (const char *message, ...);
extern void	FatalCantOpenFile (void);

#ifdef __cplusplus
}
#endif

#endif
