/* 
  FernotronSniffer - Application to capture RF-codes from
                     Fernotron remote controls
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
     
     
Fernotron myFernotron;
 

int main(int argc, char *argv[]) {
  
    // This pin is not the first pin on the RPi GPIO header!
    // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
    // for more information.
    int PIN = 2;
		
    if(wiringPiSetup() == -1)
      return 0;
		
    myFernotron = Fernotron();
    if (myFernotron.enableReceive(PIN) == false) {  // Receiver on inerrupt 0 => that is pin #2
      printf("Receive mode can't be enabled. Library seems to be already in transmit mode.\n");
       exit(1);
    }
		
    while(1)
    {
      if (myFernotron.isAvailable()) 
      {
        int* receivedCode = myFernotron.getReceivedCode();
				
        // First, output the code in one single line for commandline use
        for (int i = 0; i < FERNOTRON_PROTOCOL_LENGTH; i++) {
          printf("%d", receivedCode[i]);
        }
        printf("\n");
                
        // Second, output the code in a more readable form
/*        for (int i = 0; i < FERNOTRON_PROTOCOL_LENGTH; i++) {
          if (receivedCode[i] > 4) printf("\n");
          printf("%d,", receivedCode[i]);
          if (receivedCode[i] > 4) printf("\n");
        }
        printf("\n");
 */       
				myFernotron.resetAvailable();
			}
		}
		
  exit(0);

}

