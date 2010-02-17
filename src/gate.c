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
#include "gate.h"

/* 1 open, 0 close */
void gate_open(void) {
	GATE_PORT |= _BV(GATE_PIN);
}

void gate_close(void) {
	GATE_PORT &= ~_BV(GATE_PIN);
}

void gate_init(void) {
	GATE_PORT &= ~_BV(GATE_PIN);
	GATE_DDR |= _BV(GATE_PIN);
}

