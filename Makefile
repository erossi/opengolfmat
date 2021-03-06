MCU = atmega16
BASEDIR = /home/enrico/avr/opengolfmat
export SRCDIR = $(BASEDIR)/src
export TESTDIR = $(BASEDIR)/test

INC = -I /usr/lib/avr/include -I $(SRCDIR)
LIBS = m
OPTLEV = s
FCPU = 4000000UL
export CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV) -D F_CPU=$(FCPU)
export LFLAGS = -l$(LIBS)
export CC = avr-gcc
export REMOVE = rm -f

PRGNAME = ogmat
OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size
DUDE = avrdude -c stk500v1 -p $(MCU) -P /dev/ttyUSB0 -e -U flash:w:$(PRGNAME).hex
objects =

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

test_stmotor: $(TESTDIR)
	$(OBJCOPY) $(PRGNAME).elf $(PRGNAME).hex

test_calibrate: $(TESTDIR)
	$(OBJCOPY) $(PRGNAME).elf $(PRGNAME).hex

clean: $(SRCDIR) $(TESTDIR)
	$(REMOVE) $(PRGNAME).elf $(PRGNAME).hex

