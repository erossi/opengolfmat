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

#ifndef STM_H_
#define STM_H_

/*
Set the prescaler used for the timer.

#define TIMER_PRESCALER_1024
#define TIMER_PRESCALER_256
#define TIMER_PRESCALER_64
 */

#define TIMER_PRESCALER_64
#define USEC_DELAY_LOOP 100 /* usec delay for while cycle waiting for steps */

/* Volatile stuff, used into ISR */

#define STM_STEP 0 /* bit 0: compare match, a new step has been done */
#define STM_UPDOWN 1 /* bit 1: go up (1) or down (0) */
#define STM_SW_CALIB 3 /* require calibration */
#define STM_SW_BBNE 4 /* ball basket not empty */
#define STM_SW_BRDY 5 /* ball loaded and ready switch */
#define STM_SW_BTM 6 /* bottom switch hit */
#define STM_SW_TOP 7 /* top switch hit */

/* Accelleration and decelleration top and bottom */
#define STM_TOP_COMPARE 100
#define STM_BOTTOM_COMPARE 30
#define STM_ACCDEC_STEP 1 /* unused */
#define STM_STARTSTOP_STEPS 70 /* TOP - BOTTOM */

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

	/* top counter after calibration */
	unsigned int abs_top;
};

void stmotor_go_to(const int abs_position, struct stmotor_t *stmotor);
void stmotor_init(struct stmotor_t *stmotor);

#endif
