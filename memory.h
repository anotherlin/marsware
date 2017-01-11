/* memory.h
 *
 *
 * Copyright (C) 2017 Lin Ke-Fong
 *
 * This code is free, you may do whatever you want with it.
 */

#ifndef __MEMORY_INCLUDED__
#define __MEMORY_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"

/* Memory system implementation is based on information from Eric Quinn's 
 * SMSARCH cartridge archiver documentation.
 */

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
 * be rounded to the nearest power of 2 and mirrored inside the 16k. BIOS is 
 * emulated as a standard ROM as 128k BIOS makes use of the memory mapper.
 * Cartridge RAM must have a minimum size of one mapping (1k) 


and follows the 
 * same rule as ROM
 * nearest power of 2, it 
size is also to be rounded to the nearest power of 2, with a 
 * minimum size of one mapping (1k).
 */

typedef struct {

        uint8_t *content;
        int     size, number_banks, bank_mask, is_loaded;

} ROM;

typedef struct {

        uint8_t content[MAXIMUM_CARTRIDGE_RAM];
	int	size, number_mappings, mapping_mask, is_loaded;

} CART_RAM;

typedef struct {

        uint8_t 	ram[RAM_SIZE];
	ROM		bios;
	ROM		card;
	ROM		cartridge;
	CART_RAM	cart_ram;

	ROM		*rom;
        uint8_t 	*mappings[NUMBER_MAPPINGS];

	int		is_ram_enabled, 
			is_bios_enabled, is_card_enabled, is_cart_enabled,  
			is_cart_ram_mapped;

} MEMORY;

extern void	ResetMemoryMapper (MEMORY *memory);
extern void	WriteFrameControlRegister (MEMORY *memory, int address, int x);

#ifdef __cplusplus
}
#endif

#endif
