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
#include <util/delay.h>
#include "switch.h"
#include "stepping_motor.h"
#include "calibrate.h"

/* Global variable and pointer to be used */
/* inside the ISR routine */

extern struct stmotor_t *stmotor;

void goto_bottom(void)
{
	stmotor->abs_position=CAL_MAXSTEPS;

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

	if (stmotor->high_level < stmotor->top)
		return(1); /* ok */
	else
		return(0); /* calibration invalid */

}

/*
uint8_t calibrate(void)
{
	uint8_t error = 0;

	if (!sw_allarm()) {
		calibrate_bottom();
		calibrate_top();
		goto_zero();
	}

	if (sw_top()) {
		calibrate_at_top();
		calibrate_bottom();
		goto_zero();
	}

	if (sw_bottom()) {
		calibrate_at_bottom();
		calibrate_top();
		goto_zero();
	}
}
*/

void calibrate_init(void)
{
	stmotor_init();
}

