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

#include <avr/io.h>
#include <util/delay.h>
#include "shaker.h"

/* 1 shake, 0 stop, other shake 1 sec
   pin logic is revesed */
void shake_it(const uint8_t cmd) {
	switch (cmd) {
		case 0:	SHAKE_PORT &= ~_BV(SHAKE_PIN);
			break;
		case 1:	SHAKE_PORT |= _BV(SHAKE_PIN);
			break;
		default:
			shake_it(1);
			_delay_ms(1000);
			shake_it(0);
			break;
	}
}

void shake_init(void) {
	SHAKE_PORT &= ~_BV(SHAKE_PIN);
	SHAKE_DDR |= _BV(SHAKE_PIN);
	shake_it(2);
}

