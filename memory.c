#include <stdio.h>

#include "common.h"
#include "memory.h"

void ResetMemoryMapper (MEMORY *memory)
{
	int	i, n;
	uint8_t	*p, **q, **r;
		
	memory->number_rom_banks = memory->rom_size / BANK_SIZE;
	memory->rom_banks_mask = RoundToPower2(memory->number_rom_banks) - 1;
	if (!memory->rom_banks_mask) memory->rom_banks_mask = 1;
	
	memory->ram[0xffc] = 0;

	memory->is_cart_ram_mapped = 0;

	n = memory->rom_size > (1 << 15) ? 2 : 1;
	memory->mappings[0] = memory->rom;
	WriteFrameControlRegister(memory, 0xfffd, 0);
	WriteFrameControlRegister(memory, 0xfffe, 1);
	WriteFrameControlRegister(memory, 0xffff, n);

	p = memory->ram;
	q = &memory->mappings[3 * MAPPINGS_PER_BANK];
	r = q + MAPPINGS_PER_BANK / 2;
	for (i = 0; i < MAPPINGS_PER_BANK / 2; i++) {

		*q++ = p;
		*r++ = p;
		p += MAPPING_SIZE;

	}
}

void WriteFrameControlRegister (MEMORY *memory, int address, int x)
{
	int	n;
	uint8_t	*p, **q;

	memory->ram[address & (RAM_SIZE - 1)] = x;
	if (address == 0xfffc) {

		/* Slot 2 cane be switched between cartridge RAM or ROM. */

		if (x & 0x8) {

			n = MAPPINGS_PER_BANK;
			q = &memory->mappings[2 * MAPPINGS_PER_BANK];
			if (memory->cart_ram_size < BANK_SIZE) {

				uint8_t	*r;

				p = memory->cart_ram;
				r = p + memory->cart_ram_size;
				do {
	
					*q++ = p;
					if ((p += MAPPING_SIZE) == r)

						p = memory->cart_ram;

				} while (--n);
		
			} else {

				p = (x & 0x4) 
				&& memory->cart_ram_size > BANK_SIZE
					? memory->cart_ram + BANK_SIZE
					: memory->cart_ram;
				do {

					*q++ = p;
					p += MAPPING_SIZE;

				} while (--n);

			}
			memory->is_cart_ram_mapped = !0;

			return;

		} else if (!memory->is_cart_ram_mapped)

			return;

		else {

			/* Set frame 2 back to previous ROM bank. */

			memory->is_cart_ram_mapped = 0;
			address = 0xffff;
			x = memory->ram[0xfff];

		}

	}

	/* In the very unlikely case that ROM size is not a power of two and
	 * we are trying to access a non-present bank, mirror it.
	 */

	if ((x &= memory->rom_banks_mask) > memory->number_rom_banks) 
	
		x &= memory->rom_banks_mask >> 1;

	p = &memory->rom[x << BANK_SHIFT];
	if (!(address -= 0xfffd)) {

		n = MAPPINGS_PER_BANK - 1;
		p += MAPPING_SIZE;
		q = &memory->mappings[1];

	} else {

		n = MAPPINGS_PER_BANK;
		q = &memory->mappings[address * MAPPINGS_PER_BANK];

	}
	do {

		*q++ = p;
		p += MAPPING_SIZE; 

	} while (--n);
}

int RoundToPower2 (int n)
{
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	return n + 1;
}
