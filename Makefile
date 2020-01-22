CC=/usr/local/bin/avr-gcc
DEVICE=attiny84
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=$(DEVICE)
OBJ2HEX=/usr/local/bin/avr-objcopy 
PROG=/usr/local/bin/avrdude
TARGET=binary
FILES=main.c
CLK=1000000

.DEFAULT_GOAL = build

build: 
	# compile the source files
	$(CC) $(CFLAGS) $(FILES) -o $(TARGET).out -DF_CPU=$(CLK)

	# convert the output into a hex file
	$(OBJ2HEX) -j .text -j .data -O ihex $(TARGET).out $(TARGET).hex

	# upload the hex file to the AVR
	$(PROG) -c usbtiny -p $(DEVICE) -U flash:w:$(TARGET).hex

fuse:
	# sets the fuses for full swing oscillator, no clock divide
	$(PROG) -c usbtiny -p $(DEVICE) -U lfuse:w:0xe7:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m
