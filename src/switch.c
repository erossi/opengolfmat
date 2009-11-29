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
#include <avr/interrupt.h>
#include "switch.h"

/* Reverse logic, see electrical chart */
/* GND means switch has been hit */
unsigned short int sw_hit_top(void)
{
	if (SW_PIN & _BV(SW_PIN_TOP))
		return(0);
	else
		return(1);
}

unsigned short int sw_hit_bottom(void)
{
	if (SW_PIN & _BV(SW_PIN_BOTTOM))
		return(0);
	else
		return(1);
}

unsigned short int sw_hit(void)
{
	return(sw_hit_bottom() | sw_hit_top());
}

unsigned short int sw_ball_loader_empty(void)
{
	if (SW_PIN & _BV(SW_PIN_LOADER))
		return(0);
	else
		return(1);
}

/* Ball on the launcher */
unsigned short int sw_ball_on_the_launcher(void)
{
	if (SW_PIN & _BV(SW_PIN_BALLOK))
		return(1);
	else
		return(0);
}

unsigned short int sw_user_switch(void)
{
	if (SW_PIN & _BV(SW_PIN_USERMODE))
		return(1);
	else
		return(0);
}

unsigned short int sw_check_flags(void)
{
	if (sw_hit_top() || sw_hit_bottom() || sw_ball_loader_empty() || sw_ball_on_the_launcher())
		return(1);
	else
		return(0);
}

/* WARNING: provide and INT0 routine */
void sw_allarm_irq(const unsigned short int f)
{
	if (f)
		GICR |= _BV(INT0); /* enable INT0 */
	else
		GICR &= ~_BV(INT0); /* disable INT0 */
}

void sw_init(void)
{
	/* external pull up */
	SW_PORT &= ~(_BV(SW_PIN_BALLOK) | _BV(SW_PIN_LOADER) | _BV(SW_PIN_TOP) | _BV(SW_PIN_BOTTOM) | _BV(SW_PIN_USERMODE));
	/* Direction PIN IN */
	SW_DDR &= ~(_BV(SW_PIN_BALLOK) | _BV(SW_PIN_LOADER) | _BV(SW_PIN_TOP) | _BV(SW_PIN_BOTTOM) | _BV(SW_PIN_USERMODE));
}

