CC = gcc
CFLAGS = -c -Wall

.c.o: 
	$(CC) $(CFLAGS) $<

all: marsware

clean:
	rm -f *.o

common.o: common.c common.h

memory.o: memory.c memory.h common.h

vdp.o: vdp.c vdp.h common.h

z80emu.o: z80emu.c z80emu.h instructions.h macros.h tables.h z80io.h sms.h

cartridge.o: cartridge.c cartridge.h memory.h sms.h

sms.o: sms.c sms.h

marsware.o: marsware.c cartridge.h sms.h 

OBJECT_FILES = common.o memory.o vdp.o z80emu.o sms.o cartridge.o marsware.o 

marsware: $(OBJECT_FILES)
	gcc $(OBJECT_FILES) -o marsware
