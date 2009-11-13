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
test_stmotor_obj = $(test_engine_obj) stepping_motor.o
objects = $(test_stmotor_obj)

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o ../sint.elf main.c $(objects) $(LFLAGS)

engine.o:
	$(CC) $(CFLAGS) -c $(SRCDIR)/engine.c

stepping_motor.o:
	$(CC) $(CFLAGS) -c $(SRCDIR)/stepping_motor.c

uart.o:
	$(CC) $(CFLAGS) -D TXRXI -c uart.c
	 
test_engine: $(test_engine_obj)
	$(CC) $(CFLAGS) -o ../ogmat.elf test_engine.c $(test_engine_obj) $(LFLAGS)

test_stmotor: $(test_stmotor_obj)
	$(CC) $(CFLAGS) -o ../ogmat.elf test_stmotor.c $(test_stmotor_obj) $(LFLAGS)

test_uart: $(objects)
	$(CC) $(CFLAGS) -o ../sint.elf test_uart.c $(objects) $(LFLAGS)

clean:
	$(REMOVE) $(objects)

