/*

 'codesend' hacked from 'send' by @justy
 
 - The provided rc_switch 'send' command uses the form systemCode, unitCode, command
   which is not suitable for our purposes.  Instead, we call 
   send(code, length); // where length is always 24 and code is simply the code
   we find using the RF_sniffer.ino Arduino sketch.

 Usage: ./codesend decimalcode
 (Use RF_Sniffer.ino to check that RF signals are being produced by the RPi's transmitter)
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

    if (strlen(argv[1]) != FERNOTRON_PROTOCOL_LENGTH) {
      printf("Input code has wrong length.  Input: %d,  Expected: %d\n", strlen(argv[1]), FERNOTRON_PROTOCOL_LENGTH);
      return 1;
    }
    else {
      if (wiringPiSetup () == -1) {
        return 1;
      }
      Fernotron myFernotron = Fernotron();
      if (myFernotron.enableTransmit(PIN)) {
        myFernotron.send(argv[1]);
      }
      else {
        printf("Transmit mode can't be enabled. Library seems to be already in receive mode.\n");
        return 1;
      }
    }   
    
    return 0;
}
