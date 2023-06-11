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

#include <stdint.h>
#include <avr/io.h>
#include "switch.h"

/*
   Base mat yet calibrated
     led blink
     wait for first calibration.
     calibrazione tappeto, zero e max
   
   boot calibrated
   pc0 - pc4

   *00000 - wait with missing balls allarm!
   1X00X - goto TOP and wait the ball is gone.
   01000 - goto BOTTOM, load situation.
   00100 - 

   funzionamento ideale,
   accendione non calibrata -> resta fermo fino a calibrazione tappeto, blionk both led.

   accensione calibrata tt a 0, resta in zero ed aspetta, lampeggia solo warning:
     o ti spengono -> fine
     o ti calibrano -> calibrazione tappeto
     o ti caricano -> scendi a bottom, sali ed aspetta il tiro, continua fino a che ci sono palline, dopo vai a ZERO e riprendi come all'accensione.

   */
