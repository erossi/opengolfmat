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
#include <avr/eeprom.h>
#include <util/delay.h>
#include "switch.h"
#include "stepping_motor.h"
#include "calibrate.h"
#include "utils.h"
#include "shaker.h"

struct stmotor_t *stmotor;
unsigned int EEMEM EE_zero_level;
uint8_t EEMEM EE_calibrated;

int main (void) {
	unsigned short int i;

	/* Init globals */
        stmotor = malloc(sizeof(struct stmotor_t));
	stmotor->zero = eeprom_read_word(&EE_zero_level);

	/* general port setup */
	PORTB = 0;
	DDRB = 0;
	sw_init();
	util_init();
	shake_init();

	sei();

	/* prepare the port and calibrate if zero is not calibrated
	   or a recalibration is required */
	calibrate_init();

	for (;;) {
		wait_until_ball_on_the_loader();
		goto_bottom();
		wait_until_ball_on_the_T();
		_delay_ms(1000);
		stmotor->flags = 0;
		stmotor_go_to_level();
		i=1;

		while (i) {
			if (sw_ball_on_the_T()) {
				while (sw_user_switch()) {
					stmotor_set_next_level_of_the_T();
					stmotor_go_to_level();
					_delay_ms(2000);
				}

				_delay_ms(1000);
			} else {
				i=0;
			}
		}

		stmotor->flags = 0;
	}

	cli();
	free(stmotor);

	return (0);
}

