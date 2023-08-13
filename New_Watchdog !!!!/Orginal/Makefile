OBJS = ip_arp_udp_tcp.o enc28j60.o avrlib/a2d.o avrlib/rprintf.o main.o
CC = avr-gcc
MCU = atmega8
LFUSE = 0x64
HFUSE = 0xd9
INCDIR = -I. -I./avrlib

CPFLAGS = -O1

all: $(OBJS)
	avr-gcc $(OBJS) -o output.o $(INCDIR) -mmcu=$(MCU) $(CPFLAGS) -Wall
	avr-objcopy output.o output.hex -O ihex
	@echo [Done]
	
flash:
	sudo avrdude -c usbasp -p $(MCU) -U flash:w:output.hex

fuse:
	sudo avrdude -c usbasp -p $(MCU) -U lfuse:w:$(LFUSE):m
	sudo avrdude -c usbasp -p $(MCU) -U hfuse:w:$(HFUSE):m
	
rfuse:
	sudo avrdude -cusbasp -p $(MCU) -U lfuse:r:lfuse.bin:r
	sudo avrdude -cusbasp -p $(MCU) -U hfuse:r:hfuse.bin:r
	hexdump lfuse.bin
	hexdump hfuse.bin

	
%.o: %.c
	$(CC) -c $< -Wall $(INCDIR) -mmcu=$(MCU) $(CPFLAGS) -o$@

.S.o:
	$(CC) -c $< -x assembler-with-cpp -Wall $(INCDIR) -mmcu=$(MCU) $(CPFLAGS) -o$@

.c.s:
	$(CC) -c $< -Wall $(INCDIR) -mmcu=$(MCU) $(CPFLAGS) -o$@
	
clean:
	rm -rfv *.o *.hex ./avrlib/*.o

