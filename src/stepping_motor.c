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
#include "shaker.h"
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

void update_abs_position(void)
{
	if (stmotor->flags & _BV(STM_UPDOWN))
		stmotor->abs_position += stmotor->rel_position; /* up */
	else
		stmotor->abs_position -= stmotor->rel_position; /* down */

	stmotor->rel_position = 0;
}

void clear_counter_match_flag_bit(void)
{
	stmotor->flags &= ~_BV(STM_STEP); /* clear the flag */
}

/* start moving slowly */
void stm_start(void) {
	shake_it(1); /* start the shaker */
	counter_slow_speed(); /* set slow speed */
	engine_start(); /* energize the motor */
	counter_start(); /* start */
}

/* stop now */
void stm_stop(void) {
	counter_stop(); /* stop */
	engine_stop(); /* shutdown the motor */
	clear_counter_match_flag_bit(); /* erase remaining steps */
	shake_it(0); /* stop the shaker */
}

ISR(INT0_vect)
{
	/* old
	counter_stop();
	engine_stop();
	*/
	stm_stop();
	update_abs_position();
	stmotor->flags |= _BV(STM_ALLARM);
	sw_allarm_irq(0); /* disable myself */
}

unsigned short int allarm_hit_limit(void)
{
	if (sw_hit() || (stmotor->flags & _BV(STM_ALLARM)))
		return(1);
	else
		return(0);
}

void loop_until_counter_match(uint8_t ntimes)
{
	while (ntimes) {
		clear_counter_match_flag_bit();

		while (!(stmotor->flags & _BV(STM_STEP) || allarm_hit_limit()))
			_delay_us(COUNTER_DELAY_LOOP);

		ntimes--;
	}
}

/* 1 up, 0 down */
void set_direction(const unsigned short int dir)
{
	if (dir)
		stmotor->flags |= _BV(STM_UPDOWN); /* go up */
	else
		stmotor->flags &= ~_BV(STM_UPDOWN); /* go down */

	engine_set_direction(dir);
}

uint8_t accellerate(void)
{
	uint8_t i;

	if (!allarm_hit_limit()) {
		stmotor->rel_position = 0;

		for (i = COUNTER_TOP_COMPARE; i > COUNTER_BOTTOM_COMPARE; i--) {
			loop_until_counter_match(COUNTER_LOOP_TIMES);
			OCR0 = i;
		}

		update_abs_position(); /* adjust abs step counter */
	}

	return(allarm_hit_limit());
}

uint8_t decellerate(void)
{
	uint8_t i;

	if (!allarm_hit_limit()) {
		stmotor->rel_position = 0;

		for (i = COUNTER_BOTTOM_COMPARE; i < COUNTER_TOP_COMPARE; i++) {
			loop_until_counter_match(COUNTER_LOOP_TIMES);
			OCR0 = i;
		}

		update_abs_position();
	}

	return(allarm_hit_limit());
}

uint8_t run_for_x_steps(unsigned int steps)
{
	if (!allarm_hit_limit()) {
		stmotor->rel_position = 0;

		while ((stmotor->rel_position < steps) && !allarm_hit_limit())
			_delay_us(COUNTER_DELAY_LOOP);

		update_abs_position();
	}

	return(allarm_hit_limit());
}

/* This move will NOT check for switches, use only for calibration */
void stmotor_exit_from_switch(void)
{
	/* use only in a switch hit case */
	if (sw_hit()) { 
		if (sw_hit_bottom())
			set_direction(1); /* go up */
		else
			set_direction(0);

		stm_start(); /* start slow */

		while (sw_hit())
			_delay_us(COUNTER_DELAY_LOOP);

		update_abs_position();

		/* run for more N steps without allarms */
		stmotor->rel_position = 0;

		while (stmotor->rel_position < STM_EXIT_FROM_SWITCH_STEPS)
			_delay_us(COUNTER_DELAY_LOOP);

		update_abs_position(); /* used in calibrate the top */
		stm_stop();
		stmotor->flags &= ~_BV(STM_ALLARM);
	}
}

void stmotor_slow_check_zero(void)
{
	/* use only in a switch hit case */
	if (sw_user_switch()) { 
		stm_start();

		while (sw_user_switch() && !allarm_hit_limit())
			_delay_us(COUNTER_DELAY_LOOP);

		update_abs_position();
		stm_stop();
		stmotor->flags &= ~_BV(STM_ALLARM);
	}
}

void stmotor_go_to(const int abs_position)
{
	int remaining_steps;

	remaining_steps = (abs_position - stmotor->abs_position);

	/* first decide updown */
	if ( remaining_steps > 0)
		set_direction(1); /* go up */
	else {
		set_direction(0); /* go down */
		remaining_steps = -remaining_steps;
	}

	stm_start();
	sw_allarm_irq(1); /* enable top or bottom allarm */

	/* decide if there is enought distance to accellerate or set slow speed */
	if (remaining_steps > COUNTER_STARTSTOP_STEPS) {
		remaining_steps -= COUNTER_STARTSTOP_STEPS;
		accellerate();
	        run_for_x_steps(remaining_steps);
	        decellerate();
	} else
		run_for_x_steps(remaining_steps);

	sw_allarm_irq(0); /* disable switch control */
	stm_stop();
}

void stmotor_go_to_level(void) {
	switch (stmotor->level) {
		case 0: stmotor_go_to(stmotor->low_level);
			break;
		case 1: stmotor_go_to(stmotor->mid_level);
			break;
		case 2: stmotor_go_to(stmotor->high_level);
			break;
		case 3: stmotor_go_to(stmotor->top);
			break;
		case 4: stmotor_go_to(stmotor->zero);
			break;
		default: stmotor_go_to(stmotor->player_level);
			 break;
	}
}

void stmotor_set_levels_of_the_T(void)
{
	stmotor->low_level = stmotor->zero + STM_STEPS_BETWEEN_LEVELS;
	stmotor->mid_level = stmotor->low_level + STM_STEPS_BETWEEN_LEVELS;
	stmotor->high_level = stmotor->mid_level + STM_STEPS_BETWEEN_LEVELS;
	stmotor->level = 1;
}

void stmotor_set_next_level_of_the_T(void)
{
	if (stmotor->level < 3)
		stmotor->level++;
	else
		stmotor->level = 0;
}

void stmotor_init(void)
{
	engine_init();
	stmotor->flags = _BV(STM_CLB_BOTTOM) | _BV(STM_CLB_TOP);
	stmotor->abs_position = 0; /* It's not your duty */
	stmotor->rel_position = 0;
	/* zero level defined elsewhere */
	stmotor_set_levels_of_the_T();

	/* Generate an Interrupt on a compare match */
	TIMSK = _BV(OCIE0);

	/* should calibrate */
}

