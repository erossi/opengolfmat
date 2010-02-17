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

#ifndef STM_H_
#define STM_H_

/*
Set the prescaler used for the timer.

#define TIMER_PRESCALER_1024
#define TIMER_PRESCALER_256
#define TIMER_PRESCALER_64
 */

#define TIMER_PRESCALER_64

/* Volatile stuff, used into ISR */
/* struct stmotor_t flag */
#define STM_STEP 0 /* bit 0: compare match, a new step has been done */
#define STM_UPDOWN 1 /* bit 1: go up (1) or down (0) */
#define STM_ALLARM 2 /* allarm stop the engine */
#define STM_CLB_BOTTOM 3 /* require bottom calibration */
#define STM_CLB_TOP 4 /* require top calibration */
#define STM_DISASTER 7 /* Something bad happens, stop everything and crash */

/* if abs_position is less than 0 or > CRASH_STEPS then crash */
#define STM_CRASH_STEPS 20000

/* Number of steps to exit backward from a switch hit condition */
#define STM_EXIT_FROM_SWITCH_STEPS 200

/* if exiting from switch should not require this number of steps */
#define STM_EXIT_FROM_SWITCH_MAX_STEPS 500

/* steps between level of the T */
#define STM_STEPS_BETWEEN_LEVELS 1000

/* Number of steps to go backward when we hit a switch */
/* Must be less than COUNTER_STARTSTOP_STEPS */
#define CAL_BACKSTEPS 100

/* Maximum number of steps */
/* Should be > of STM_CRASH_STEPS */
#define CAL_MAXSTEPS 25000

struct stmotor_t {
	/* bit flags */
	volatile uint8_t flags;

	/* absolute value of steps taken */
	/* can be negative before calibration or on an
	   uncalibrated situation */
	/* should not be volatile, please FIX */
	volatile int abs_position;

	/* used in interrupt */
	volatile unsigned int rel_position;

	/* counter after calibration */
	/* zero - the level of the mat */
	/* top - the level when hits the top switch */
	uint16_t zero;
	unsigned int top;

	/* levels ready for the player */
	unsigned int low_level, mid_level, high_level;
	
	/* personal player's level, adjusted by hand */
	unsigned int player_level;

	/* levels 0 low, 1 mid, 2 high, 3 max, 4 player */
	uint8_t level;
};

void disaster(void);
void stmotor_exit_from_switch(void);
void stmotor_slow_check_zero(void);
void stmotor_go_to(const int abs_position);
void stm_go_to_level(void);
void stm_go_to_bottom(void);
void stmotor_set_levels_of_the_T(void);
void stmotor_set_next_level_of_the_T(void);
void stm_park_the_T(void);
void stm_reload(void);
void stm_init(void);

#endif
