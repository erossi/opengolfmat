MCU = atmega16
INC = -I /usr/lib/avr/include
LIBS = m
OPTLEV = s
export CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV)
export LFLAGS = -l$(LIBS)
export CC = avr-gcc

PRGNAME = ogmat
OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size
DUDE = avrdude -c stk500v1 -p $(MCU) -P /dev/ttyUSB0 -e -U flash:w:$(PRGNAME).hex
REMOVE = rm -f
objects =
SRCDIR = src
TESTDIR = test

.PHONY: clean indent $(SRCDIR) $(TESTDIR)
.SILENT: help
.SUFFIXES: .c, .o

all: $(SRCDIR)
	$(OBJCOPY) $(PRGNAME).elf $(PRGNAME).hex

program:
	$(DUDE)

$(SRCDIR):
	@echo $(MAKECMDGOALS)
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(TESTDIR):
	@echo $(MAKECMDGOALS)
	$(MAKE) -C $@ $(MAKECMDGOALS)

test_engine: $(TESTDIR)
	$(OBJCOPY) $(PRGNAME).elf $(PRGNAME).hex

clean: $(SRCDIR) $(TESTDIR)
	$(REMOVE) $(PRGNAME).elf $(PRGNAME).hex

