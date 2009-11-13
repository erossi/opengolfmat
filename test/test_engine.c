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
#include <util/delay.h>
#include "engine.h"

int main (void) {
	/* general port setup */
	PORTB = 0;
	DDRB = 0;
	/* must output the OC0 port PB3 on Mega16*/
	engine_init();

	/* compare match to the top, max slow speed */
	OCR0 = 100;

	/* CTC mode with toggle OC0 on compare match */
	/* prescale to 64 */
	/* start the counter */
	TCCR0 = _BV(WGM01) | _BV(COM00) | _BV(CS01) | _BV(CS00);

	for (;;) {
		engine_set_direction(0); /* down */
		engine_start();
		_delay_ms(5000);
		engine_stop();
		_delay_ms(1000);
		engine_set_direction(1); /* up */
		engine_start();
		_delay_ms(5000);
		engine_stop();
		_delay_ms(1000);
	}

	return (0);
}

