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

#ifndef COUNTER_H_
#define COUNTER_H_

/* usec delay for while cycle waiting for steps */
#define COUNTER_DELAY_LOOP 100

/* Accelleration and decelleration top and bottom */
#define COUNTER_TOP_COMPARE 100
#define COUNTER_BOTTOM_COMPARE 30
#define COUNTER_STARTSTOP_STEPS 70 /* TOP - BOTTOM */

void counter_start(void);
void counter_stop(void);
void counter_slow_speed(void);
void counter_high_speed(void);

#endif
