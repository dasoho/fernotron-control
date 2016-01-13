/* 
  FernotronSend - Application to send RF-codes to Fernotron shutters.
  See README.md for usage.
  The basis of this code has been taken from:
  https://github.com/sui77/rc-switch
  
  Project home: https://github.com/dasoho/fernotron-control

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Fernotron.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
     

int main(int argc, char *argv[]) {
  
    // This pin is not the first pin on the RPi GPIO header!
    // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
    // for more information.
    int PIN = 0;

    if (argc < 2 || argc > 3) {
      printf("Wrong number of parameters\n1. Fernotron code\n2. Number of resends (optional)");
      return 1;
    }
    else    if (strlen(argv[1]) != FERNOTRON_PROTOCOL_LENGTH) {
      printf("Input code has wrong length.  Input: %d,  Expected: %d\n", strlen(argv[1]), FERNOTRON_PROTOCOL_LENGTH);
      return 1;
    }
    else {
      if (wiringPiSetup () == -1) {
        return 1;
      }
      Fernotron myFernotron = Fernotron();
      if (myFernotron.enableTransmit(PIN)) {
        if (argc == 3){
          myFernotron.setRepeatTransmit(atoi(argv[2]));
        }
        myFernotron.send(argv[1]);
      }
      else {
        printf("Transmit mode can't be enabled. Library seems to be already in receive mode.\n");
        return 1;
      }
    }   
    
    return 0;
}
