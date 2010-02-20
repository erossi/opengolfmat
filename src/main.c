/*
   This file is part of OpenGolfMat
   Copyright (C) 2009, 2010 Enrico Rossi

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
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "switch.h"
#include "wait.h"
#include "stepping_motor.h"
#include "utils.h"
#include "shaker.h"
#include "gate.h"

struct stmotor_t *stmotor;
uint16_t EEMEM EE_zero_level;
uint8_t EEMEM EE_calibrated;
uint8_t EEMEM EE_disaster;

void check_for_disaster(void)
{
	uint8_t dd;

	dd = eeprom_read_byte(&EE_disaster);

	while (dd == 71) {
		led_ctrl(2, 1); /* red and green on */

		if (sw_user_recalibration()) {
			dd = 0;
			eeprom_write_byte(&EE_disaster, 0);
			led_ctrl(2, 0); /* leds off */
		}
	}
}

void wait_for_the_strike(void) {
	uint8_t i;

	i=2;

	while (i) {
		if (sw_ball_on_the_T()) {
			led_blink(0, 1); /* green led blink */

			while (sw_user_switch()) {
				stmotor_set_next_level_of_the_T();
				stm_go_to_level();
				/* may a blink be good ? */
				_delay_ms(2000);
			}
		} else {
			i--;
			_delay_ms(100);
		}
	}

	led_ctrl(0, 1); /* power on */
}

int main (void) {
	/* Init globals */
        stmotor = malloc(sizeof(struct stmotor_t));
	stmotor->zero = eeprom_read_word(&EE_zero_level);

	/* general port setup */
	PORTB = 0;
	DDRB = 0;
	sw_init();
	util_init();
	shake_init();
	gate_init();
	check_for_disaster();

	sei();

	/* prepare the port and calibrate if zero is not calibrated
	   or a recalibration is required */
	stm_init();

	for (;;) {
		stm_park_the_T();
		wait_until_ball_in_the_loader();
		stm_reload();
		wait_for_the_strike();
	}

	cli();
	free(stmotor);

	return (0);
}

