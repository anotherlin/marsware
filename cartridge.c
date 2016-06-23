#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "sms.h"
#include "cartridge.h"

#define SEGA_STRING_LENGTH      8
#define NUMBER_HEADER_OFFSETS   3

typedef struct {

        uint8_t sega_string[SEGA_STRING_LENGTH];
        uint8_t reserved[2];
        uint8_t checksum[2];                    /* Little endian. */
        uint8_t product_code_version[3];
        uint8_t region_code_rom_size;

} HEADER;

static const char SEGA_STRING[SEGA_STRING_LENGTH] = "TMR SEGA";

/* Order is important! Look at high offsets first. */

static const int HEADER_OFFSETS[NUMBER_HEADER_OFFSETS] = {

	0x7ff0,
	0x3ff0,
	0x1ff0,

};

static const char *REGIONS[] = {
			
	"Master System Japan",
	"Master System Export",
	"Game Gear Japan",
	"Game Gear Export",
	"Game Gear International",

};

static const int ROM_SIZES[16] = {

	256, 512, 1024, 0,
	0, 0, 0, 0, 
	0, 0, 8, 16, 
	32, 48, 64, 128,

};

static void	zero_fill (uint8_t *p, const uint8_t *q);
static void	mirror (uint8_t *p, int n, int size);
static void 	read_header (SMS *sms, int offset);

void ReadCartridge (MEMORY *memory, const char *filename)
{
	FILE	*file;
	int	size;

	printf("Loading ROM file \"%s\".\n", filename);
        if ((file = fopen(filename, "rb")) == NULL)

                FatalCantOpenFile();

	fseek(file, 0, SEEK_END);
	size = (int) ftell(file);
	fseek(file, 0, SEEK_SET);
	if (!size || size > MAXIMUM_ROM_SIZE) 

		Fatal("ROM is too big or empty (%d bytes)!\n", size);

	memory->rom_size 
		= ((size + BANK_SIZE - 1) >> BANK_SHIFT) << BANK_SHIFT;
	if ((memory->rom = (uint8_t *) malloc(memory->rom_size)) == NULL)
	
		Fatal("Unable to allocate %d bytes for ROM!\n", 
			memory->rom_size);

	if (fread(memory->rom, 1, size, file) != size)

		Fatal("An error occured while reading ROM!\n");

	fclose(file);
	printf("Loaded %d bytes.\n", size);
	if (size < BANK_SIZE) {

		int	p2;
	
		printf("ROM size is smaller than bank size, it will be "
			"rounded to a power of two\n and mirrored inside a "
			"bank.\n");
		if ((p2 = RoundToPower2(size)) < MAPPING_SIZE) 

			p2 = MAPPING_SIZE;

		zero_fill(memory->rom + size, memory->rom + p2);
		mirror(memory->rom, p2, BANK_SIZE);

	} else if (size % BANK_SIZE) {

		Warning("ROM size is not a multiple of bank size.\n");
		zero_fill(memory->rom + size, memory->rom + memory->rom_size);

	}
}

void ReadCartridgeRAM (MEMORY *memory, const char *filename)
{
	FILE	*file;
	int	size, p2;

	printf("Loading cartridge RAM file \"%s\".\n", filename);
        if ((file = fopen(filename, "rb")) == NULL) 

                FatalCantOpenFile();

	fseek(file, 0, SEEK_END);
	size = (int) ftell(file);
	fseek(file, 0, SEEK_SET);
	if (!size || size >= MAXIMUM_CARTRIDGE_RAM) 

		Fatal("Cartridge RAM is too big or empty (%d bytes)!\n", size);

	if (fread(memory->cart_ram, 1, size, file) != size)

		Fatal("An error occured while reading cartridge RAM!\n");

	fclose(file);

	if ((p2 = RoundToPower2(size)) < MAPPING_SIZE) p2 = MAPPING_SIZE;
	zero_fill(memory->cart_ram + size, memory->cart_ram + p2);
	memory->cart_ram_size = p2;
}

void WriteCartridgeRAM (MEMORY *memory, const char *filename)
{
	int	size;
	FILE	*file;

	if (!(size = memory->cart_ram_size))

		return;

	if ((file = fopen(filename, "wb")) == NULL 
	|| fwrite(memory->cart_ram, 1, size, file) != size)

		Fatal("Failed to open or properly write cartridge RAM file "
			"\"%s\"!\n", filename);	

	fclose(file);
}

void FindAndReadHeader (SMS *sms) 
{
	int	i;

	for (i = 0; i < NUMBER_HEADER_OFFSETS; i++)

		if (HEADER_OFFSETS[i] + sizeof(HEADER) 
			<= sms->memory.rom_size
		&& !memcmp(&sms->memory.rom[HEADER_OFFSETS[i]], 
			SEGA_STRING, 
			SEGA_STRING_LENGTH)) {

			read_header(sms, HEADER_OFFSETS[i]);
			return;

		}

	sms->is_game_gear = 0;
	Warning("Unable to find header, emulating a Master System.\n");
}

static void zero_fill (uint8_t *p, const uint8_t *q)
{
	memset(p, 0, q - p);
}

static void mirror (uint8_t *p, int n, int size)
{
	uint8_t	*q, *r;

	for (q = p + n, r = p + size; q != r; q += n)

		memcpy(q, p, n);
}

static void read_header (SMS *sms, int offset) 
{
	HEADER	*header;
	int	region, size_code, size, checksum;

	printf("Header found at offset 0x%.4x.\n", offset);
	header = (HEADER *) &sms->memory.rom[offset];

	printf("Sega string: \"%.*s\".\n", 
		SEGA_STRING_LENGTH, (char *) header->sega_string);

	printf("Product code: %d%.2x%.2x.\n", 
		header->product_code_version[2] >> 4,
		header->product_code_version[1],
		header->product_code_version[0]);
	printf("Version: %d.\n", header->product_code_version[2] & 0xf);

	region = header->region_code_rom_size >> 4;
	if (region >= 3 && region <= 7) {

		printf("Region code: 0x%1x (%s).\n",
			region, REGIONS[region - 3]);
		sms->is_game_gear = region >= 5;
		printf("This is a %s ROM file.\n", 
			sms->is_game_gear ? "Game Gear" : "Master System");

	} else

		Warning("Unknown region code: 0x%1x, "
			"emulating a Master System!\n",
			region);

	size_code = header->region_code_rom_size & 0xf;
	if (!(size = ROM_SIZES[size_code]))

		Warning("Unknown ROM size code: 0x%1x!\n", size_code);

	else {

		printf("ROM size code: 0x%1x (%dKB).\n", size_code, size);
		if ((size <<= 10) != sms->memory.rom_size)

			printf("Note: Code may indicate a smaller size to "
				"speed-up checksum computation.\n");

	}

	checksum = header->checksum[0] | (header->checksum[1] << 8);
	printf("Checksum: 0x%.4x.\n", checksum);
	if (size) {
		
		uint16_t	total;
		int		n, i;

		total = 0;
		n = size_code == 0xd ? 0xbff0 : offset;
		for (i = 0; i < n; i++)

			total += sms->memory.rom[i];

		for (i = 0x8000; i < size; i++)

			total += sms->memory.rom[i];

		printf("Computed checksum: 0x%.4x.\n", (int) total);
		if (checksum != total)

			printf("Note: It is not uncommon for checksum "
				"in header to be incorrect.\n");

	} else

		Warning("Unable to compute checksum because " 
			"ROM size code is unknown!\n");
}
