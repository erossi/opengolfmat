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
#include "switch.h"

/* Reverse logic, see electrical chart */
/* GND means switch has been hit */
uint8_t sw_hit_top(void)
{
	if (SW_PIN & _BV(SW_PIN_TOP))
		return(0);
	else
		return(1);
}

uint8_t sw_hit_bottom(void)
{
	if (SW_PIN & _BV(SW_PIN_BOTTOM))
		return(0);
	else
		return(1);
}

uint8_t sw_hit(void)
{
	return(sw_hit_bottom() | sw_hit_top());
}

uint8_t sw_ball_in_the_loader(void)
{
	if (SW_PIN & _BV(SW_PIN_LOADER))
		return(0);
	else
		return(1);
}

/* Ball on the launcher */
uint8_t sw_ball_on_the_T(void)
{
	if (SW_PIN & _BV(SW_PIN_BALLOK))
		return(1);
	else
		return(0);
}

uint8_t sw_user_switch(void)
{
	if (SW_PIN & _BV(SW_PIN_USERMODE))
		return(1);
	else
		return(0);
}

uint8_t sw_check_flags(void)
{
	if (sw_hit_top() || sw_hit_bottom() || sw_ball_in_the_loader() || sw_ball_on_the_T() || sw_user_switch())
		return(1);
	else
		return(0);
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

