MCU = atmega16
INC = -I /usr/lib/avr/include
LIBS = m
OPTLEV = s
FCPU = 4000000UL
CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV) -D F_CPU=$(FCPU)
LFLAGS = -l$(LIBS)
CC = avr-gcc

REMOVE = rm -f

init_obj = init.o
stmotor_obj = engine.o switch.o counter.o stepping_motor.o wait.o
objects = $(stmotor_obj) utils.o shaker.o

.PHONY: clean
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o ../ogmat.elf main.c $(objects) $(LFLAGS)

uart.o:
	$(CC) $(CFLAGS) -D TXRXI -c uart.c
	 
clean: $(SUBDIRS)
	$(REMOVE) $(objects) main.o

