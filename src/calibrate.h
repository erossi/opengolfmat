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

#ifndef CALIBRATE_H_
#define CALIBRATE_H_

/* Number of steps to go backward when we hit a switch */
/* Must be less than COUNTER_STARTSTOP_STEPS */
#define CAL_BACKSTEPS 100

/* Maximum number of steps */
#define CAL_MAXSTEPS 20000

void calibrate_init(void);
void calibrate_bottom(void);
void calibrate_bottom_and_top(void);
void calibrate_zero(void);

#endif