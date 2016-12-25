#include "common.h"
#include "vdp.h"

#define SMS_CRAM_SIZE	(1 << 5)
#define GG_CRAM_SIZE	(1 << 6)

/* VDP status flags. */

#define S_INT_FLAG	(1 << 7)
#define S_OVR_FLAG	(1 << 6)
#define S_COL_FLAG	(1 << 5)

/* VDP registers. */
			
#define R0_VSI_FLAG	0x80	
#define R0_HSI_FLAG	0x40	
#define R0_LCB_FLAG	0x20
#define R0_IE1_FLAG	0x10
#define R0_SS_FLAG	0x08

#define R1_DIS_FLAG	0x40
#define R1_IE_FLAG	0x20
#define R1_SZ_FLAG	0x02

#define R2_SM_MASK	0x0e
#define R2_SM_SHIFT	1

#define R5_SPAT_MASK	0x7e
#define R5_SPAT_SHIFT	7

#define R6_SP_MASK	0x04
#define R6_SP_SHIFT	11

#define R7_BC_MASK	0x0f

#define BIT_MASK	0x08080808
			
static const uint32_t BIT_PATTERNS[16] = {

	0x00000000,
	0x00000008,
	0x00000800,
	0x00000808,

	0x00080000,
	0x00080008,
	0x00080800,
	0x00080808,

	0x08000000,
	0x08000008,
	0x08000800,
	0x08000808,

	0x08080000,
	0x08080008,
	0x08080800,
	0x08080808,

};

static void	write_register (VDP *vdp, int r, int x);

void ResetVDP (VDP *vdp, int is_game_gear)
{
	vdp->is_game_gear = is_game_gear;
	vdp->is_second_byte = 0;
	vdp->border_color_index = 0;
}

int ReadVCounter (VDP *vdp)
{
	return 0;
}

int ReadHCounter (VDP *vdp)
{
	return 0;
}
		
int ReadVDPData (VDP *vdp)
{
	int	x;
	
	x = vdp->buffer;
	vdp->address = (vdp->address + 1) & (VRAM_SIZE - 1);	
	vdp->buffer = vdp->vram[vdp->address];
	vdp->is_second_byte = 0;
	return x;
}

void WriteVDPData (VDP *vdp, int x)
{
	if (!vdp->is_cram_write) {

		/* Write to VRAM. Immediately decode patterns to color indexes.
		 * This will also convert data written to screen map and sprite
		 * attribute table. This does not harm. 
		 */

		int		shift;
		uint32_t	*p, m, u, v;

		vdp->vram[vdp->address] = x;
		shift = vdp->address & 0x3;
		p = (uint32_t *) &vdp->patterns[
			(vdp->address & (VRAM_SIZE - 0x4)) >> 1];
		m = BIT_MASK >> shift;
		u = BIT_PATTERNS[(x >> 4) & 0xf] >> shift;
		v = BIT_PATTERNS[x & 0xf] >> shift;
		p[0] = (p[0] & m) | u;
		p[1] = (p[1] & m) | v;

	/* check invalidation */

	} else if (vdp->is_game_gear){

		/* Game Gear has 12-bit RGB using a latching mechanism. */

		if (vdp->address & 1) {

			int		rgb;
			uint32_t	color;

			rgb = (x << 8) | vdp->cram_latch;
			color = (rgb & 0xf) << 20;
			color |= (rgb & 0xf0) << 8;
			color |= (rgb >> 4) & 0xf0;
			vdp->palette[
				(vdp->address & (GG_CRAM_SIZE - 1)) >> 1]
				= color;

		} else

			vdp->cram_latch = x;

	} else {

		uint32_t	color;

		color = (x & 0x3) << 22; 
		color |= (x & 0x0c) << 10;
		color |= (x << 2) & 0xc0;
		vdp->palette[vdp->address & (SMS_CRAM_SIZE - 1)] = color;

	}
	vdp->address = (vdp->address + 1) & (VRAM_SIZE - 1);
	vdp->is_second_byte = 0;
	vdp->buffer = x;
}

int ReadVDPStatus (VDP *vdp)
{
	int	status;

	status = vdp->status;
	vdp->status = 0;
	vdp->is_second_byte = 0;
	return status;
}

void WriteVDPCommand (VDP *vdp, int x)
{
	int	first_byte, opcode;

	if (!vdp->is_second_byte) {

		vdp->address = (vdp->address & 0x3f00) | x;
		vdp->is_second_byte = 1;
		return;
	}

	first_byte = vdp->address & 0xff;
	vdp->address = first_byte | ((x & 0x3f) << 8);
	if ((opcode = x >> 6) == 0x2) {

		vdp->is_cram_write = 0;
		write_register(vdp, x & 0xf, first_byte);

	} else if (!opcode) {

		vdp->is_cram_write = 0;
		vdp->buffer = vdp->vram[vdp->address];

	} else

		vdp->is_cram_write = opcode == 0x3;

	vdp->is_second_byte = 0;
}

static void write_register (VDP *vdp, int r, int x)
{
	uint8_t	*p;

	if (r == 5)

		p = vdp->vram + ((x & R5_SPAT_MASK) << R5_SPAT_SHIFT);

	else if (r == 6) {

		p = vdp->vram + ((x & R6_SP_MASK) << R6_SP_SHIFT);

	} else if (r == 7) {

		vdp->border_color_index = x & R7_BC_MASK;


	}
}
