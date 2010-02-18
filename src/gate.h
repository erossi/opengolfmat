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

#ifndef GATE_H
#define GATE_H

/* Control port and pinout */
#define GATE_PORT PORTB
#define GATE_DDR DDRB
#define GATE_PIN PB1

void gate_open(void);
void gate_close(void);
void gate_init(void);

#endif

