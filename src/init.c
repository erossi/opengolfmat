/*
   This file is part of OpenGolfMat
   Copyright (C) 2009 Enrico Rossi

   OpenGolfMat is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   OpenGolfMat is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Enrico Rossi <e.rossi@tecnobrain.com>
 */

#include <stdint.h>
#include <avr/io.h>
#include "default.h"
#include "stmotor.h"
#include "init.h"

void port_a_init(void)
{
	PORTA = 0;
	DDRA = 0;
}

void port_b_init(void)
{
	/*
	 * pb3: OUT - OC0 Timer counter 0 
	 */

	PORTB = 0;
	DDRB = _BV(PB3);
}

void port_c_init(void)
{
	PORTC = 0;
	DDRC = 0;
}

void port_d_init(void)
{
	/*
	 * pd0-1: xx RS232 rx/tx
	 * pd2: INT0 rx lacrosse PULL-UP software and hardware
	 */

	PORTD = 0;
	DDRD = 0;
}

void port_init(void)
{
	port_a_init();
	port_b_init();
	port_c_init();
	port_d_init();
}

void init(struct stmotor_t *stmotor) {
	port_init();
	stmotor_init(stmotor);
}

