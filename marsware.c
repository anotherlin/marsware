#include <stdlib.h>


#include "common.h"
#include "memory.h"

#include "sms.h"

#include "cartridge.h"

int main (int argc, char *argv[]) 
{
	SMS	sms;

	if (argc == 2)

		ReadCartridge(&sms.memory, argv[1]);

	else

		ReadCartridge(&sms.memory, "../roms/psycho_fox.sms");

	FindAndReadHeader(&sms);

	

	ResetMemoryMapper(&sms.memory);

	return EXIT_SUCCESS;
}

/*

1) Parse command line
2) Read ROM file
3) read savegame
4) init everything
5) go
6) write savegame



*/
