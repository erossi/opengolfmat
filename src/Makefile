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
engine_obj = engine.o
stmotor_obj = stepping_motor.o $(engine_obj)
phone_obj = cell.o uart_isr.o uart.o 
synth_obj = sht11.o init.o adc.o anemometer.o synth.o davis.o
objects = $(stmotor_obj)

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o ../sint.elf main.c $(objects) $(LFLAGS)

uart.o:
	$(CC) $(CFLAGS) -D TXRXI -c uart.c
	 
clean: $(SUBDIRS)
	$(REMOVE) $(objects)

indent:
	indent *.c *.h
