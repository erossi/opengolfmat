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
#include <avr/interrupt.h>
#include <util/delay.h>
#include "counter.h"
#include "engine.h"
#include "switch.h"
#include "stepping_motor.h"

/* Global variable and pointer to be used */
/* inside the ISR routine */

extern struct stmotor_t *stmotor;

ISR(TIMER0_COMP_vect)
{
	/* increment the relative step */
	stmotor->rel_position++;

	/* set the compare match flag bit */
	stmotor->flags |= _BV(STM_STEP);
}

void update_step_counter(void)
{
	if (stmotor->flags & _BV(STM_UPDOWN))
		stmotor->abs_position += stmotor->rel_position; /* up */
	else
		stmotor->abs_position -= stmotor->rel_position; /* down */
}

ISR(INT0_vect)
{
	counter_stop();
	engine_stop();
	update_step_counter();
	stmotor->flags |= _BV(SW_ALLARM);
	sw_allarm_irq(0); /* disable myself */
}

void clear_counter_match_flag_bit(void)
{
	stmotor->flags &= ~_BV(STM_STEP); /* clear the flag */
}

void loop_until_counter_match(void)
{
	clear_counter_match_flag_bit();

	while (!(stmotor->flags & _BV(STM_STEP) || sw_allarm()))
		_delay_us(COUNTER_DELAY_LOOP);
}

uint8_t accellerate(void)
{
	uint8_t i;

	if (!sw_allarm()) {
		stmotor->rel_position = 0;

		for (i = COUNTER_TOP_COMPARE; i > COUNTER_BOTTOM_COMPARE; i--) {
			loop_until_counter_match();
			OCR0 = i;
		}

		update_step_counter(); /* adjust abs step counter */
	}

	return(sw_allarm());
}

uint8_t decellerate(void)
{
	uint8_t i;

	if (!sw_allarm()) {
		stmotor->rel_position = 0;

		for (i = COUNTER_BOTTOM_COMPARE; i < COUNTER_TOP_COMPARE; i++) {
			loop_until_counter_match();
			OCR0 = i;
		}

		update_step_counter();
	}

	return(sw_allarm());
}

uint8_t run_for_x_steps(unsigned int steps)
{
	if (!sw_allarm()) {
		stmotor->rel_position = 0;

		while ((stmotor->rel_position < steps) || sw_allarm())
			_delay_us(COUNTER_DELAY_LOOP);

		update_step_counter();
	}

	return(sw_allarm());
}

/* This move will NOT check for switches, use only for calibration */
void stmotor_force_run_for_x_steps(unsigned int steps)
{
	stmotor->rel_position = 0;

	while (stmotor->rel_position < steps)
		_delay_us(COUNTER_DELAY_LOOP);

	update_step_counter();
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

void stmotor_go_to(const int abs_position)
{
	int remaining_steps;

	remaining_steps = (abs_position - stmotor->abs_position);

	/* first decide updown */
	if ( remaining_steps > 0) {
		stmotor->flags |= _BV(STM_UPDOWN); /* go up */
	} else {
		stmotor->flags &= ~_BV(STM_UPDOWN); /* go down */
		remaining_steps = -remaining_steps;
	}

	engine_set_direction(stmotor->flags & _BV(STM_UPDOWN));
	counter_slow_speed();
	engine_start();
	counter_start();
	sw_allarm_irq(1); /* enable top or bottom allarm */

	/* decide if there is enought distance to accellerate etc.. */
	if (remaining_steps > 2 * COUNTER_STARTSTOP_STEPS) {
		remaining_steps -= (2 * COUNTER_STARTSTOP_STEPS);
		accellerate();
	        run_for_x_steps(remaining_steps);
	        decellerate();
	} else
		run_for_x_steps(remaining_steps);

	sw_allarm_irq(0); /* disable */
	counter_stop();
	engine_stop();
	clear_counter_match_flag_bit();
}

void stmotor_init(void)
{
	engine_init();
	sw_init();
	stmotor->flags = 0;
	stmotor->abs_position = 0;
	stmotor->rel_position = 0;

	/* Generate an Interrupt on a compare match */
	TIMSK = _BV(OCIE0);

	/* should calibrate */
}

