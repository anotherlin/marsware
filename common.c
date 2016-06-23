#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

void Warning (const char *message, ...)
{
	va_list	ap;

	va_start(ap, message);
	vfprintf(stderr, message, ap);
	va_end(ap);
}

void Fatal (const char *message, ...)
{
	va_list	ap;

	va_start(ap, message);
	vfprintf(stderr, message, ap);
	va_end(ap);

	exit(EXIT_FAILURE);
}

void FatalCantOpenFile (void)
{
	Fatal("Cannot open file!\n");
}
