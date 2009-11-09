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

#ifndef STM_ENGINE_H_
#define STM_ENGINE_H_

/* Control port and bit */
#define STM_CTRL_PORT PORTA
#define STM_CTRL_DDR DDRA
#define STM_PIN_ENABLE 0 /* 1 enable the engine */
#define STM_PIN_UPDOWN 1 /* 1 go up */

#define ENGINE_DELAY_MS 100 /* delay from enable engine and start moving */

void engine_start(void);
void engine_stop(void);
void engine_set_direction(const unsigned short int updown);
void engine_init(void);

#endif
