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
#include <avr/io.h>
#include <util/delay.h>
#include "engine.h"

void engine_start(void) {
	STM_CTRL_PORT |= _BV(STM_PIN_ENABLE);
	_delay_ms(ENGINE_DELAY_MS);
}

void engine_stop(void) {
	_delay_ms(ENGINE_DELAY_MS);
	STM_CTRL_PORT &= ~_BV(STM_PIN_ENABLE);
}

/* reverse logic: remove when the electronic has been fixed
void engine_start(void) {
	STM_CTRL_PORT &= ~_BV(STM_PIN_ENABLE);
	_delay_ms(ENGINE_DELAY_MS);
}
void engine_stop(void) {
	_delay_ms(ENGINE_DELAY_MS);
	STM_CTRL_PORT |= _BV(STM_PIN_ENABLE);
}
*/

void engine_set_direction(const unsigned short int updown) {
	if (updown)
		STM_CTRL_PORT |= _BV(STM_PIN_UPDOWN); /* up */
	else
		STM_CTRL_PORT &= ~_BV(STM_PIN_UPDOWN); /* down */
}

void engine_init(void) {
	STM_CTRL_PORT &= ~(_BV(STM_CTRL_CLK) | _BV(STM_PIN_ENABLE) | _BV(STM_PIN_UPDOWN));
	/* Direction PIN out */
	STM_CTRL_DDR |= _BV(STM_CTRL_CLK) | _BV(STM_PIN_ENABLE) | _BV(STM_PIN_UPDOWN);
}

