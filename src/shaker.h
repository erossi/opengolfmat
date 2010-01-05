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

#ifndef SHAKER_H
#define SHAKER_H_

/* Control port and pinout */
#define SHAKE_PORT PORTB
#define SHAKE_DDR DDRB
#define SHAKE_PIN 2

/*
#define CMDOUT_LED_OK 6
#define CMDOUT_LED_ALLARM 7
*/

void shake_it(const unsigned short int cmd);
void shake_init(void);

#endif

