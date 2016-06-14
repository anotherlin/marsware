#ifndef __SMS_INCLUDED__
#define __SMS_INCLUDED__

#include "common.h"
#include "memory.h"
#include "vdp.h"
#include "z80emu.h"

#define SMS_P1_UP	(1 << 0)
#define SMS_P1_DOWN	(1 << 1)
#define SMS_P1_LEFT	(1 << 2)
#define SMS_P1_RIGHT	(1 << 3)
#define SMS_P1_SW1	(1 << 4)
#define SMS_P1_SW2	(1 << 5)
#define SMS_P2_UP	(1 << 6)
#define SMS_P2_DOWN	(1 << 7)

#define SMS_P2_LEFT	(1 << 8)
#define SMS_P2_RIGHT	(1 << 9)
#define SMS_P2_SW1	(1 << 10)
#define SMS_P2_SW2	(1 << 11)
#define SMS_RESET	(1 << 12)

typedef struct SMS {
	
	int		is_game_gear;
	int		is_domestic;	/* Emulate a Japanese model. */

	Z80_STATE	z80;
	MEMORY		memory;
	VDP		vdp;

	/* SN PSG + Yamaha */
	int		input;

} SMS;

extern uint8_t	ReadPort (SMS *sms, int port);
extern void	WritePort (SMS *sms, int port, int x);

#endif
