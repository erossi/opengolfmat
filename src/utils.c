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

/* num - blinks number
   led - 0 green, 1 red, 2 both
   async - 0 no, 1 yes
 */

void led_blink(uint8_t num, const uint8_t led)
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

	while (num) {
		LED_PORT |= pin;
		_delay_ms(300);
		LED_PORT &= ~pin;
		_delay_ms(300);
		num--;
	}
}

void wait_for_click(void)
{
	loop_until_bit_is_clear(UTILS_SWITCH_PORT, UTILS_SWITCH_PIN);
	led_blink(1,0);
	_delay_ms(1000);
}

uint8_t check_for_click(void)
{
	if (bit_is_clear(UTILS_SWITCH_PORT, UTILS_SWITCH_PIN)) {
		led_blink(1,0);
		_delay_ms(1000);
		return (1);
	} else
		return (0);
}

void util_init(void)
{
	/* Switch in */
	UTILS_SWITCH_PORT &= ~_BV(UTILS_SWITCH_PIN);
	UTILS_SWITCH_DDR &= ~_BV(UTILS_SWITCH_PIN);

	/* no internel pullup */
	LED_PORT &= ~(_BV(LED_GREEN_PIN) | _BV(LED_RED_PIN));
	LED_PORT |= _BV(LED_GREEN_PIN) | _BV(LED_RED_PIN);
}
