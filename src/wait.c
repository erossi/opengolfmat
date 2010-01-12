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

#include <avr/io.h>
#include <util/delay.h>
#include "shaker.h"
#include "switch.h"
#include "wait.h"

void wait_until_ball_on_the_loader(void)
{
	uint8_t i;

	for (i=0; i<2; i++) {
		while (!sw_ball_on_the_loader())
			if (sw_user_switch())
				shake_it(2); /* shake 1 sec */
			else 
				_delay_ms(WAIT_LOADER);

		_delay_ms(WAIT_LOADER);
	}
}

void wait_until_ball_on_the_T(void)
{
	uint8_t i;

	for (i=0; i<2; i++) {
		while (!sw_ball_on_the_T())
			_delay_ms(WAIT_T);

		_delay_ms(WAIT_T);
	}
}

void wait_until_ball_is_gone(void)
{
	uint8_t i;

	for (i=0; i<2; i++) {
		while (sw_ball_on_the_T())
			_delay_ms(WAIT_GONE);

		_delay_ms(WAIT_GONE);
	}
}

