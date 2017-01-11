#ifndef __CARTRIDGE_INCLUDED__
#define __CARTRIDGE_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

extern void	ReadCartridge (MEMORY *memory, const char *filename);
extern void 	ReadBIOS (MEMORY *memory, const char *filename)

extern void	ReadCartridgeRAM (MEMORY *memory, const char *filename);
extern void	WriteCartridgeRAM (MEMORY *memory, const char *filename);

/* See the "ROM header" page on smspower.org's development wiki for details. */ 

extern void	FindAndReadHeader (SMS *sms);

#ifdef __cplusplus
}
#endif

#endif
