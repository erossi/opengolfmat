#MCU = atmega16
#BASEDIR = /home/enrico/avr/opengolfmat
#SRCDIR = $(BASEDIR)/src
#INC = -I /usr/lib/avr/include -I $(SRCDIR)
#LIBS = m
#OPTLEV = s
#FCPU = 4000000UL
#CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV) -D F_CPU=$(FCPU)
#LFLAGS = -l$(LIBS)
#CC = avr-gcc
#
#REMOVE = rm -f

init_obj = init.o
test_engine_obj = engine.o
test_stmotor_obj = engine.o switch.o counter.o stepping_motor.o
test_calibrate_obj = $(test_stmotor_obj) calibrate.o
objects = $(test_calibrate_obj)

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o ../sint.elf main.c $(objects) $(LFLAGS)

engine.o:
	$(CC) $(CFLAGS) -c $(SRCDIR)/engine.c

switch.o:
	$(CC) $(CFLAGS) -c $(SRCDIR)/switch.c

counter.o:
	$(CC) $(CFLAGS) -c $(SRCDIR)/counter.c

stepping_motor.o:
	$(CC) $(CFLAGS) -c $(SRCDIR)/stepping_motor.c

calibrate.o:
	$(CC) $(CFLAGS) -c $(SRCDIR)/calibrate.c

uart.o:
	$(CC) $(CFLAGS) -D TXRXI -c uart.c
	 
test_engine: $(test_engine_obj)
	$(CC) $(CFLAGS) -o ../ogmat.elf test_engine.c $(test_engine_obj) $(LFLAGS)

test_stmotor: $(test_stmotor_obj)
	$(CC) $(CFLAGS) -o ../ogmat.elf test_stmotor.c $(test_stmotor_obj) $(LFLAGS)

test_calibrate: $(test_calibrate_obj)
	$(CC) $(CFLAGS) -o ../ogmat.elf test_calibrate.c $(test_calibrate_obj) $(LFLAGS)

test_uart: $(objects)
	$(CC) $(CFLAGS) -o ../sint.elf test_uart.c $(objects) $(LFLAGS)

clean:
	$(REMOVE) $(objects)

