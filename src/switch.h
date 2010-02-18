/*
   This file is part of OpenGolfMat
   Copyright (C) 2009, 2010 Enrico Rossi

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
#define SW_PIN_BALLOK PINC0 /* ball on the launcher 1: ball present */
#define SW_PIN_LOADER PINC1 /* ball in the loader tube 1: ball present */
#define SW_PIN_TOP PINC2 /* top switch hit */
#define SW_PIN_BOTTOM PINC3 /* bottom switch hit */
#define SW_PIN_USERMODE PINC4 /* user switch */

/* time to keep user switch closed to recalibrate in sec */
#define SW_RECALIBRATE_TIMEOUT 10

uint8_t sw_hit_top(void);
uint8_t sw_hit_bottom(void);
uint8_t sw_hit(void);
uint8_t sw_ball_in_the_loader(void);
uint8_t sw_ball_on_the_T(void);
uint8_t sw_user_switch(void);
uint8_t sw_user_recalibration(void);
uint8_t sw_check_flags(void);
void sw_init(void);

#endif
