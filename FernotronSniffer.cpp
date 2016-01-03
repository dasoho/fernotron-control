/*
  RF_Sniffer
  
  Hacked from http://code.google.com/p/rc-switch/
  
  by @justy to provide a handy RF code sniffer
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

