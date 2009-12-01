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

#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "switch.h"
#include "stepping_motor.h"
#include "calibrate.h"
#include "utils.h"

struct stmotor_t *stmotor;

int main (void) {
	/* Init globals */
        stmotor = malloc(sizeof(struct stmotor_t));

	/* general port setup */
	PORTB = 0;
	DDRB = 0;
	calibrate_init();
	util_init();

	/* wait until all the switches are off */
	while (sw_check_flags()) {
		led_blink(1,2);
		_delay_ms(1000);
	}

	/* wait for user switch and calibrate zero */
	while (!sw_user_switch()) {
		led_blink(2,2);
		_delay_ms(1000);
	}

	sei();

	calibrate_zero();
	_delay_ms(1000);
	stmotor->flags = 0;

	for (;;) {
		wait_until_ball_on_the_loader();
		goto_bottom();
		wait_until_ball_on_the_T();
		_delay_ms(1000);
		stmotor->flags = 0;
		stmotor_go_to(stmotor->mid_level);
		wait_until_ball_is_gone();
		_delay_ms(1000);
		stmotor->flags = 0;
	}

	cli();
	free(stmotor);

	return (0);
}

