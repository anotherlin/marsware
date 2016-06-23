#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "sms.h"
#include "bios.h"

void ReadBIOS (MEMORY *memory, const char *filename)
{
	FILE	*file;
	int	size;

	printf("Loading BIOS file \"%s\".\n", filename");
	if ((file = fopen(filename, "rb")) == NULL)

		FatalCantOpenFile();

	fseek(file, 0, SEEK_END)
	size = (int) ftell(file);
	fseek(file, 0, SEEK_SET);
	if (size != BIOS_SIZE)

		Fatal("BIOS file is %d byte(s) instead of %d!\n", 
			size, BIOS_SIZE);

	if (fread(memory->bios, 1, BIOS_SIZE, file) != BIOS_SIZE)

		Fatal("An error occured while reading BIOS!\n");

	fclose(file);
	memory->is_bios_loaded = !0;
}
