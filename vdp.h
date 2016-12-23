// Implement read buffer

#ifndef __VDP_INCLUDED__
#define __VDP_INCLUDED__

#define VRAM_SIZE	(1 << 14)
#define NUMBER_COLORS	32

typedef struct {

	int		is_game_gear;

	/* Data port. */

	int		is_cram_write;
	int		cram_latch;	/* GG only */
	uint8_t		buffer;		/* VRAM read buffer */

	/* Status/control port command. */

	int		status;
	int		is_second_byte;
	int		address;

	/* Note that CRAM is write-only, so we can directly convert to RGB. */

	uint32_t	palette[NUMBER_COLORS];
	int		border_color_index;

	/* VRAM contains character patterns, screen map (background), and 
	 * sprite attibute table. 
	 */

	uint8_t		vram[VRAM_SIZE];
	uint16_t	*screen_map;
	uint8_t		*sprite_attribute_table;
	
	/* Character patterns converted into color indexes. */
	
	uint8_t		patterns[VRAM_SIZE * 2];
	uint8_t		*sprite_patterns;
	
	
	/* */


	int		interrupt_enable;

	int	hsi; // hor scroll inhibit k
	int	vsi;	// vscroll inhibito


	int		sprite_shift;

	
	uint32_t	*frame_buffer;

} VDP;

extern void	ResetVDP (VDP *vdp, int is_game_gear);
extern int	EmulateVDP (VDP *vdp, int elapsed_cycles, int number_cycles);


extern int	ReadVCounter (VDP *vdp);
extern int	ReadHCounter (VDP *vdp);

extern int	ReadVDPData (VDP *vdp);
extern void	WriteVDPData (VDP *vdp, int x);

extern int 	ReadVDPStatus (VDP *vdp);
extern void 	WriteVDPCommand (VDP *vdp, int x);

#endif

