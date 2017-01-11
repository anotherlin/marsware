CC = gcc
CFLAGS = -c -Wall

.c.o: 
	$(CC) $(CFLAGS) $<

OBJECT_FILES = common.o memory.o

all: $(OBJECT_FILES) # marsware

clean:
	rm -f *.o marsware

common.o: common.c common.h

memory.o: memory.c memory.h common.h config.h

#vdp.o: vdp.c vdp.h common.h

#z80emu.o: z80emu.c z80emu.h z80config.h z80user.h \
#j	instructions.h macros.h tables.h \
#	sms.h

#cartridge.o: cartridge.c cartridge.h memory.h sms.h

#sms.o: sms.c sms.h

#marsware.o: marsware.c cartridge.h sms.h 

#memory.o vdp.o z80emu.o sms.o cartridge.o marsware.o 

marsware: $(OBJECT_FILES)
#	gcc $(OBJECT_FILES) -o marsware

