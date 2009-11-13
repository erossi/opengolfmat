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
#include "engine.h"
#include "default.h"
#include "stepping_motor.h"

/* Global variable and pointer to be used */
/* inside the ISR routine */

extern struct stmotor_t *stmotor;

ISR(TIMER0_COMP_vect) {
	/* increment the relative step */
	stmotor->rel_position++;

	/* set the compare match flag bit */
	stmotor->flags |= _BV(STM_STEP);
}

void start_counter(void) {
	/* CTC counter mode, prescale 64, start counter */
	TCCR0 = _BV(WGM01) | _BV(COM00) | _BV(CS01) | _BV(CS00);
}

void stop_counter(void) {
	/* disable everything, maybe too much */
	TCCR0 = 0;
}

void set_slow_speed(void) {
	OCR0 = STM_TOP_COMPARE;
}

void update_step_counter(struct stmotor_t *stmotor) {
	if (stmotor->flags & _BV(STM_UPDOWN))
		stmotor->abs_position += stmotor->rel_position; /* up */
	else
		stmotor->abs_position -= stmotor->rel_position; /* down */
}

void clear_counter_match_flag_bit(struct stmotor_t *stmotor) {
	stmotor->flags &= ~_BV(STM_STEP); /* clear the flag */
}

void loop_until_counter_match(struct stmotor_t *stmotor) {
	clear_counter_match_flag_bit(stmotor);

	while (!(stmotor->flags & _BV(STM_STEP)))
		_delay_us(USEC_DELAY_LOOP);
}

void accellerate(struct stmotor_t *stmotor) {
	uint8_t i;

	stmotor->rel_position = 0;

	for (i = STM_TOP_COMPARE; i > STM_BOTTOM_COMPARE; i--) {
		loop_until_counter_match(stmotor);
		OCR0 = i;
	}

	update_step_counter(stmotor); /* adjust abs step counter */
}

void decellerate(struct stmotor_t *stmotor) {
	uint8_t i;

	stmotor->rel_position = 0;

	for (i = STM_BOTTOM_COMPARE; i < STM_TOP_COMPARE; i++) {
		loop_until_counter_match(stmotor);
		OCR0 = i;
	}

	update_step_counter(stmotor);
}

void run_for_x_steps(unsigned int steps, struct stmotor_t *stmotor) {
	stmotor->rel_position = 0;

	while (stmotor->rel_position < steps)
		_delay_us(USEC_DELAY_LOOP);
	
	update_step_counter(stmotor);
}

void stmotor_go_to(const int abs_position, struct stmotor_t *stmotor) {
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
	set_slow_speed();
	engine_start();
	start_counter();

	/* decide if there is enought distance to accellerate etc.. */
	if (remaining_steps > 2 * STM_STARTSTOP_STEPS) {
		remaining_steps -= (2 * STM_STARTSTOP_STEPS);
		accellerate(stmotor);
		run_for_x_steps(remaining_steps, stmotor);
		decellerate(stmotor);
	} else
		run_for_x_steps(remaining_steps, stmotor);

	stop_counter();
	engine_stop();
	clear_counter_match_flag_bit(stmotor);
}

void stmotor_init(struct stmotor_t *stmotor) {
	engine_init();
	stmotor->flags = 0;
	stmotor->abs_position = 0;
	stmotor->rel_position = 0;

	/* Generate an Interrupt on a compare match */
	TIMSK = _BV(OCIE0);

	/* should calibrate */
}

