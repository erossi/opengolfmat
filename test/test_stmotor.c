/*
   Copyright (C) 2009 Enrico Rossi

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Enrico Rossi <e.rossi@tecnobrain.com>
 */

#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "engine.h"
#include "stepping_motor.h"

struct stmotor_t *stmotor;

int main (void) {
	/* Init globals */
        stmotor = malloc(sizeof(struct stmotor_t));

	/* general port setup */
	PORTB = 0;
	DDRB = 0;
	/* must output the OC0 port PB3 on Mega16*/
	stmotor_init(stmotor);

	sei();

	for (;;) {
		stmotor_go_to(400, stmotor);
		_delay_ms(1000);
		stmotor_go_to(800, stmotor);
		_delay_ms(1000);
		stmotor_go_to(900, stmotor);
		_delay_ms(1000);
	}

	cli();

	return (0);
}

