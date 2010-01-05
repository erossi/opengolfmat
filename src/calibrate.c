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
#include <avr/eeprom.h>
#include <util/delay.h>
#include "switch.h"
#include "stepping_motor.h"
#include "utils.h"
#include "calibrate.h"

/* Global variable and pointer to be used */
/* inside the ISR routine */

extern struct stmotor_t *stmotor;
extern unsigned int EEMEM EE_zero_level;
extern uint8_t EEMEM EE_calibrated;

void goto_bottom(void)
{
	/* if ball is on the T grave error */
	wait_until_ball_is_gone();
	stmotor->abs_position=CAL_MAXSTEPS;

	/* you MUST select the case before going to 0
	   ball on the loader can not be true near 0 */
	if (sw_ball_on_the_loader()) {
		stmotor_go_to(0);
		wait_until_ball_on_the_T();
	} else {
		stmotor_go_to(0);
	}

	stmotor_exit_from_switch();
	stmotor->abs_position=0;
}

void calibrate_bottom_and_top(void)
{
	goto_bottom();
	stmotor_go_to(CAL_MAXSTEPS);
	stmotor_exit_from_switch();
	stmotor->top=stmotor->abs_position;
}

/* Set the level of the mat and calculate the low, mid and high
   point of the launcher */

uint8_t calibrate_zero(void)
{
	calibrate_bottom_and_top();
	stmotor_slow_check_zero();
	stmotor->zero=stmotor->abs_position;
	stmotor_set_levels_of_the_T();

	if (stmotor->high_level < stmotor->top) {
		stmotor->flags = 0;
		eeprom_write_word(&EE_zero_level, stmotor->zero);
		eeprom_write_byte(&EE_calibrated, 71);
		return(1); /* ok */
	} else {
		return(0); /* calibration invalid */
	}
}

void check_and_recalibrate(void)
{
	if (sw_user_recalibration())
		calibrate_zero();
}

void calibrate_init(void)
{
	uint8_t calibrated;

	stmotor_init();
	calibrated = eeprom_read_byte(&EE_calibrated);

	if (calibrated == 71) {
		check_and_recalibrate();
	} else {
		/* if uncalibrated */
		while (calibrated != 71) {
			/* wait for user switch and calibrate zero */
			while (!sw_user_switch()) {
				led_blink(2,2);
				_delay_ms(1000);
			}

			if (calibrate_zero()) {
				_delay_ms(1000); /* ??? */
				calibrated = 71;
			}
		}
	}

	calibrate_bottom_and_top();
}

