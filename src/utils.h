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

#ifndef UTILS_H
#define UTILS_H

#define UTILS_SWITCH_PORT PORTB
#define UTILS_SWITCH_DDR DDRB
#define UTILS_SWITCH_PIN PIN0
#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED_GREEN_PIN PB6
#define LED_RED_PIN PB7

/* led 0 green, led 1 red */
void led_blink(uint8_t num, const uint8_t led);
void wait_for_click(void);
uint8_t check_for_click(void);
void util_init(void);

#endif