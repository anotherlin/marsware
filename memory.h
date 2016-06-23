#ifndef __MEMORY_INCLUDED__
#define __MEMORY_INCLUDED__

/* Memory system implementation is based on information from Eric Quinn's 
 * SMSARCH cartridge archiver documentation.
 */

#define BIOS_SIZE		(1 << 13)
#define MAXIMUM_ROM_SIZE        (1 << 22)
#define MAXIMUM_CARTRIDGE_RAM   (1 << 15)
#define RAM_SIZE                (1 << 13)

/* Memory is divided in "mappings", this allows for fast implementation of 
 * mirroring and unpaged regions.
 */

#define MAPPING_SHIFT		10
#define MAPPING_SIZE		(1 << MAPPING_SHIFT)
#define NUMBER_MAPPINGS		((1 << 16) / MAPPING_SIZE)

#define BANK_SHIFT		14
#define BANK_SIZE		(1 << BANK_SHIFT)
#define MAPPINGS_PER_BANK	(BANK_SIZE / MAPPING_SIZE)

#define GET_POINTER(memory, address)			\
(							\
        (memory)->mappings[((address) >> MAPPING_SHIFT)	\
                & (NUMBER_MAPPINGS - 1)]		\
        + ((address) & (MAPPING_SIZE - 1))		\
)

#define MEMORY_READ_BYTE(memory, address)	\
(						\
	*GET_POINTER((memory), (address))	\
)

#define MEMORY_WRITE_BYTE(memory, address, x)				\
{									\
        int     high;							\
									\
        if ((high = (address) & 0xf000) >= 0xc000) {			\
									\
                if ((address) < 0xfffc)					\
									\
			*GET_POINTER((memory), (address)) = x;		\
									\
		else							\
									\
                        WriteFrameControlRegister(			\
				(memory), (address), x);		\
									\
        } else if ((memory)->is_cart_ram_mapped && high >= 0x8000)	\
									\
                *GET_POINTER((memory), (address)) = x;			\
}

/* ROM size must be a multiple of 16k bank. If smaller than 16k, then it is to
 * be rounded to the nearest power of 2 and mirrored inside the 16k. Cartridge 
 * RAM size is also to be rounded to the nearest power of 2, with a minimum 
 * size of a mapping.
 */

typedef struct {

	uint8_t bios[BIOS_SIZE];
	int	is_bios_loaded;

        uint8_t *rom;
        int     rom_size, number_rom_banks, rom_banks_mask, is_rom_loaded;

        uint8_t ram[RAM_SIZE];

        uint8_t cart_ram[MAXIMUM_CARTRIDGE_RAM];
	int	cart_ram_size;

        uint8_t *mappings[NUMBER_MAPPINGS];

	/* if rom_size < 32k, rom is enabled if card enabled, otherwise
  	 * simulate as cartridge.
  	 */

	int	is_bios_enabled, is_rom_enabled, is_ram_enabled;
	int	is_cart_ram_mapped;

} MEMORY;

extern void	ResetMemoryMapper (MEMORY *memory);
extern void	WriteFrameControlRegister (MEMORY *memory, int address, int x);

extern int	RoundToPower2 (int n);

#endif
