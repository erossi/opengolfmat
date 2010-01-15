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
#include "default.h"
#include "utils.h"

/* led - 0 green, 1 red, 2 both
   onoff - 0 off, 1 on */
void led_ctrl(const uint8_t led, const uint8_t onoff)
{
	uint8_t pin;

	pin = _BV(LED_GREEN_PIN | LED_RED_PIN);

	switch (led) {
		case 0:
			pin = _BV(LED_GREEN_PIN);
			break;
		case 1:
			pin = _BV(LED_RED_PIN);
			break;
	}

	if (onoff)
		LED_PORT |= pin;
	else
		LED_PORT &= ~pin;
}

/* num - blinks number
   led - 0 green, 1 red, 2 both */
void led_blink(uint8_t num, const uint8_t led)
{
	while (num) {
		led_ctrl(led, 1);
		_delay_ms(300);
		led_ctrl(led, 0);
		_delay_ms(300);
		num--;
	}
}

void util_init(void)
{
	/* Switch in, no pullup */
	UTILS_SWITCH_PORT &= ~_BV(UTILS_SWITCH_PIN);
	UTILS_SWITCH_DDR &= ~_BV(UTILS_SWITCH_PIN);

	/* led port out */
	LED_PORT &= ~(_BV(LED_GREEN_PIN) | _BV(LED_RED_PIN));
	LED_DDR |= _BV(LED_GREEN_PIN) | _BV(LED_RED_PIN);

	led_ctrl(0, 1); /* power on */
}
