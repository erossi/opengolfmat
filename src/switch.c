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

#include <avr/io.h>
#include <util/delay.h>
#include "switch.h"

/* Reverse logic, see electrical chart */
/* GND means switch has been hit */
uint8_t sw_hit_top(void)
{
	return (!(SW_PIN & _BV(SW_PIN_TOP)));
}

uint8_t sw_hit_bottom(void)
{
	return (!(SW_PIN & _BV(SW_PIN_BOTTOM)));
}

uint8_t sw_hit(void)
{
	return (sw_hit_bottom() || sw_hit_top());
}

uint8_t sw_ball_in_the_loader(void)
{
	return (!(SW_PIN & _BV(SW_PIN_LOADER)));
}

uint8_t sw_ball_on_the_T(void)
{
	return ((SW_PIN & _BV(SW_PIN_BALLOK)));
}

uint8_t sw_user_switch(void)
{
	return ((SW_PIN & _BV(SW_PIN_USERMODE)));
}

uint8_t sw_check_flags(void)
{
	return ((sw_hit_top() || sw_hit_bottom() || sw_ball_in_the_loader() || sw_ball_on_the_T() || sw_user_switch()));
}

/* true if user switch is pressed more than 10 (SW_RECAL_TMOUT) sec. */
uint8_t sw_user_recalibration(void) {
	uint8_t i, j;

	j=1;

	for (i=0; i<SW_RECALIBRATE_TIMEOUT; i++) {
		if (sw_user_switch()) {
			_delay_ms(1000);
		} else {
			i = SW_RECALIBRATE_TIMEOUT;
			j = 0;
		}
	}

	return (j);
}

void sw_init(void)
{
	/* external pull up */
	SW_PORT &= ~(_BV(SW_PIN_BALLOK) | _BV(SW_PIN_LOADER) | _BV(SW_PIN_TOP) | _BV(SW_PIN_BOTTOM) | _BV(SW_PIN_USERMODE));
	/* Direction PIN IN */
	SW_DDR &= ~(_BV(SW_PIN_BALLOK) | _BV(SW_PIN_LOADER) | _BV(SW_PIN_TOP) | _BV(SW_PIN_BOTTOM) | _BV(SW_PIN_USERMODE));
}

