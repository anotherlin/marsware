#ifndef __SMS_INCLUDED__
#define __SMS_INCLUDED__

#include "common.h"
#include "memory.h"
#include "vdp.h"
#include "z80emu.h"

typedef struct SMS {
	
	int		is_game_gear;

	Z80_STATE	z80;
	MEMORY		memory;
	VDP		vdp;

} SMS;

uint8_t	ReadPort (SMS *sms, int port);
void	WritePort (SMS *sms, int port, int x);

#endif
