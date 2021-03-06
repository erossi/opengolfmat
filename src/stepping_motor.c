/*
   This file is part of OpenGolfMat
   Copyright (C) 2009-2010 Enrico Rossi

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
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "counter.h"
#include "engine.h"
#include "switch.h"
#include "shaker.h"
#include "utils.h"
#include "wait.h"
#include "gate.h"
#include "stepping_motor.h"

/* Global variable and pointer to be used */
/* inside the ISR routine */

extern struct stmotor_t *stmotor;
extern uint8_t EEMEM EE_disaster;
extern uint16_t EEMEM EE_zero_level;
extern uint8_t EEMEM EE_calibrated;

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

void disaster(void)
{
	stm_stop();
	eeprom_write_byte(&EE_disaster, 71);

	for (;;) {
		led_ctrl(2,1); /* both led on */
	}
}

ISR(TIMER0_COMP_vect)
{
	/* increment the relative step */
	stmotor->rel_position++;

	if (stmotor->rel_position > STM_CRASH_STEPS) {
		disaster();
	}

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

/* WARNING: provide and INT0 routine */
void set_allarm_irq(const uint8_t f)
{
	if (f)
		GICR |= _BV(INT0); /* enable INT0 */
	else
		GICR &= ~_BV(INT0); /* disable INT0 */
}

ISR(INT0_vect)
{
	stm_stop();
	update_abs_position();
	stmotor->flags |= _BV(STM_ALLARM);
	set_allarm_irq(0); /* disable myself */
}

uint8_t allarm_hit_limit(void)
{
	if (sw_hit() || (stmotor->flags & _BV(STM_ALLARM)))
		return(1);
	else
		return(0);
}

uint8_t load_ball(void)
{
	if (sw_ball_in_the_loader()) {
		gate_open();
		wait_until_ball_on_the_T();
		gate_close();
		return(1);
	} else {
		return(0);
	}
}

/* should be removed */
void intrastep_check(void)
{
	int i;
	static uint8_t filter;

	i = stmotor->abs_position - stmotor->rel_position;

	/* Ball on the T && going down && below zero && > 1cm from 0 */
	if (sw_ball_on_the_T() && \
			!(stmotor->flags & _BV(STM_UPDOWN)) && \
			(i < stmotor->zero) && \
			(i > 2000)) {
		if (filter > 10) {
			stm_stop();
			update_abs_position();
			stmotor->flags |= _BV(STM_ALLARM);
			disaster();
		} else {
			filter++;
		}
	} else {
		filter = 0;
		_delay_us(COUNTER_DELAY_LOOP);
	}
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
void set_direction(const uint8_t dir)
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

		/*
		while ((stmotor->rel_position < steps) && !allarm_hit_limit())
			intrastep_check();
		*/
		while ((stmotor->rel_position < steps) && !allarm_hit_limit())
			_delay_us(COUNTER_DELAY_LOOP);

		update_abs_position();
	}

	return(allarm_hit_limit());
}

/*
   This move will NOT check for switches, It will move the cursor for
   exit from a switch hit situation like calibration.
   As the switch open the cursor continues to move for an extra
   steps to be sure we're out from the switch hit tollerance.
 */
void stmotor_exit_from_switch(void)
{
	/* use only in a switch hit case */
	if (sw_hit()) { 
		stmotor->rel_position = 0;

		if (sw_hit_bottom())
			set_direction(1); /* go up */
		else
			set_direction(0);

		stm_start(); /* start slow */

		while (sw_hit()) {
			if (stmotor->rel_position > STM_EXIT_FROM_SWITCH_MAX_STEPS)
				disaster();

			_delay_us(COUNTER_DELAY_LOOP);
		}

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
	set_allarm_irq(1); /* enable top or bottom allarm */

	/* decide if there is enought distance to accellerate or set slow speed */
	if (remaining_steps > COUNTER_STARTSTOP_STEPS) {
		remaining_steps -= COUNTER_STARTSTOP_STEPS;
		accellerate();
	        run_for_x_steps(remaining_steps);
	        decellerate();
	} else
		run_for_x_steps(remaining_steps);

	set_allarm_irq(0); /* disable switch control */
	stm_stop();
}

void stm_go_to_level(void) {
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

void stm_park_the_T(void)
{
	/* parking the T on zero if no ball in the loader */
	if (!sw_ball_in_the_loader() && (stmotor->abs_position != stmotor->zero) && !sw_ball_on_the_T()) {
		stmotor_go_to(stmotor->zero);
	}
}

void bottom_calibrate(void)
{
	stmotor_exit_from_switch();
	stmotor->abs_position=0;
	/* Clear the bottom calibrate bit from flags */
	stmotor->flags &= ~_BV(STM_CLB_BOTTOM);
}

void top_calibrate(void)
{
	stmotor_exit_from_switch();

	/* if bottom is calibrated then set the top value */
	if (!(stmotor->flags & _BV(STM_CLB_BOTTOM))) {
		stmotor->top=stmotor->abs_position;
		/* Clear the top calibrate bit from flags */
		stmotor->flags &= ~_BV(STM_CLB_TOP);
	}
}

void stm_go_to_bottom(void)
{
	stmotor->abs_position=CAL_MAXSTEPS;
	stmotor_go_to(0);

	/* We should never reach the 0 */
	if (stmotor->abs_position == 0)
		disaster();

	bottom_calibrate();
}

void stm_go_to_top(void)
{
	stmotor_go_to(CAL_MAXSTEPS);

	/* We should never reach the MAX */
	if (stmotor->abs_position == CAL_MAXSTEPS)
		disaster();

	top_calibrate();
}

void stm_reload(void)
{
	/* prerequisite condition */
	if (sw_ball_in_the_loader() && !sw_ball_on_the_T()) {
		stm_go_to_bottom();

		/* if the ball to be loaded isn't vanished */
		if (sw_ball_in_the_loader()) {
			/* and some idiot may have dropped
			   a ball on the T */
			if (!sw_ball_on_the_T())
				load_ball();

			stm_go_to_level();
		} else {
			if (sw_ball_on_the_T())
				stm_go_to_level();
			else
				stm_park_the_T();
		}
	}
}

void calibrate_bottom_and_top(void)
{
	stm_go_to_bottom();
	stm_go_to_top();
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

void startup_check_switches(void)
{
	if (sw_hit()) {
		if (sw_hit_bottom()) {
			bottom_calibrate();
		} else {
			top_calibrate(); /* just exit from switch */
		}
	}
}

void calibrate_init(void)
{
	uint8_t calibrated;

	calibrated = eeprom_read_byte(&EE_calibrated);
	startup_check_switches();

	if (calibrated == 71)
		if (sw_user_recalibration())
			calibrated = 0;

	/* if uncalibrated */
	while (calibrated != 71) {
		/* wait for user switch and calibrate zero */
		while (!sw_user_switch()) {
			led_blink(2,2);
			_delay_ms(1000);
		}

		if (calibrate_zero()) {
			calibrated = 71;
			led_ctrl(0,1); /* green on */
		}
	}

	/* if bottom or top has to be calibrated */
	if (stmotor->flags & _BV(STM_CLB_BOTTOM) || stmotor->flags & _BV(STM_CLB_TOP))
	calibrate_bottom_and_top();
}

void stm_init(void)
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
	calibrate_init();
}

