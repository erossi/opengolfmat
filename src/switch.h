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

#ifndef SWITCH_H_
#define SWITCH_H_

/* Control port and pinout */
#define SW_PORT PORTC
#define SW_DDR DDRC
#define SW_PIN PINC /* we read the port with pin */
#define SW_PIN_BALLOK 0 /* PCX: ball on the launcher 1: ball present */
#define SW_PIN_LOADER 1 /* PCX: ball in the loader tube 1: ball present */
#define SW_PIN_TOP 2 /* PCX: top switch hit */
#define SW_PIN_BOTTOM 3 /* PCX: bottom switch hit */
#define SW_PIN_USERMODE 4 /* user switch */

/* switch flags bits mostly negate the input pin */
#define SW_ALLARM 0 /* One of the top or bottom has been hit */
#define SW_CALIB 1 /* require calibration */

unsigned short int sw_allarm(void);
unsigned short int sw_user_switch(void);
void sw_allarm_irq(const unsigned short int f);
void sw_init(void);

#endif
