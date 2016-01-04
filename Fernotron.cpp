/*
  Fernotron - Library for remote control of Fernotron shutters
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
#include <stdio.h>
#include <string.h>


// Transmit Timimings
#define SHORT       400
#define LONG        800
#define XLONG       3200
#define XXLONG      58000
#define XXXLONG     70500

// Receive lower and upper threshold
#define SHORT_LT    250
#define SHORT_UT    550
#define LONG_LT     650
#define LONG_UT     1000
#define XLONG_LT    3000
#define XLONG_UT    3500
#define XXLONG_LT   57000
#define XXLONG_UT   59000
#define XXXLONG_LT  69500
#define XXXLONG_UT  71500


int Fernotron::timings[FERNOTRON_MAX_CHANGES];
int Fernotron::receivedCode[FERNOTRON_PROTOCOL_LENGTH];
bool Fernotron::codeAvailable = false;


Fernotron::Fernotron() {
  nReceiverInterrupt = -1;
  nTransmitterPin = -1;
  setRepeatTransmit(3);
}


bool Fernotron::enableTransmit(int transmitterPin) {
  if (nReceiverInterrupt == -1) {
    nTransmitterPin = transmitterPin;
    pinMode(nTransmitterPin, OUTPUT);
    return true;
  }
  else {
    return false;
  }
}


void Fernotron::setRepeatTransmit(int repeatTransmit) {
  nRepeatTransmit = repeatTransmit;
}


void Fernotron::send(char* code) {

  if (nTransmitterPin == -1) {
    printf("Transmit mode not enabled\n");
    return; 
  }
  else if (strlen(code) != FERNOTRON_PROTOCOL_LENGTH) {
    printf("Input code has wrong length.  Input: %d,  Expected: %d\n", strlen(code), FERNOTRON_PROTOCOL_LENGTH);
    return; 
  }
  else {
    int highLength = 0;
    int lowLength = 0;

      for (int r = 0; r < nRepeatTransmit; r++) {
        for(int i = 0; i < FERNOTRON_PROTOCOL_LENGTH; i++) { 
          switch(code[i]){
            case '1': // LS+HS
              lowLength = SHORT;
              highLength = SHORT;
              break;
            case '2': // LL+HS
              lowLength = LONG;
               highLength = SHORT;
              break;
            case '3': // LS+HL
              lowLength = SHORT;
              highLength = LONG;           
              break;
            case '4': // LL+HL
              lowLength = LONG;
              highLength = LONG;           
              break;
            case '5': // LXL+HS
              lowLength = XLONG;
              highLength=SHORT;           
              break;          
            case '6': // LXL+HL
              lowLength = XLONG;
              highLength = LONG;           
              break;      
            case '7': // LXXL+HS
              lowLength = XXLONG;
              highLength = SHORT; 
              break;            
            case '8': // LXXXL+HS
              lowLength = XXXLONG;
              highLength = SHORT; 
              break;            
          }
          digitalWrite(nTransmitterPin,LOW);     
          delayMicroseconds(lowLength);         
          digitalWrite(nTransmitterPin, HIGH);     
          delayMicroseconds(highLength); 
        }
      }      
    digitalWrite(nTransmitterPin, LOW);
  }
}

bool  Fernotron::enableReceive(int interrupt) {
  if (nTransmitterPin == -1 && nReceiverInterrupt == -1) {
    nReceiverInterrupt = interrupt;
    Fernotron::codeAvailable = false;
    wiringPiISR(nReceiverInterrupt, INT_EDGE_BOTH, &handleInterrupt);
    return true;
  }
  else {
    return false;
  }
}


bool Fernotron::isAvailable() {
  return Fernotron::codeAvailable;
}


void Fernotron::resetAvailable() {
  Fernotron::codeAvailable = false;
}


int* Fernotron::getReceivedCode() {
    return Fernotron::receivedCode;
}


int* Fernotron::getReceivedRawdata() {
    return Fernotron::timings;
}


bool Fernotron::receiveProtocol(int changeCount) {
  
  // Encoding used by the transmitter:
  //  LOW short + HIGH short    1
  //  LOW long + HIGH short     2
  //  LOW short + HIGH long     3
  //  LOW long + HIGH long      4
  //  LOW Xlong + HIGH short    5
  //  LOW Xlong + HIGH long     6  
  //  LOW XXlong + HIGH short   7
  //  LOW XXXlong + HIGH short  8
  bool success = true;
  if (changeCount + 1 != FERNOTRON_PROTOCOL_LENGTH * 2) {
    printf("Error in protocol length. Expected: %d,  Received: %d\n", FERNOTRON_PROTOCOL_LENGTH * 2, changeCount );
    success = false;
  }
  else {
	int low, high;
    for (int i = 0; i < changeCount && success ; i=i+2) {
      low=Fernotron::timings[i];
      high=Fernotron::timings[i+1];
     
      if (low >= SHORT_LT && low <= SHORT_UT && high >= SHORT_LT && high <= SHORT_UT)
        receivedCode[i/2] = 1;
      else if (low >= LONG_LT && low <= LONG_UT && high >= SHORT_LT && high <= SHORT_UT)
        receivedCode[i/2] = 2;
      else if (low >= SHORT_LT && low <= SHORT_UT && high >= LONG_LT && high <= LONG_UT)
        receivedCode[i/2] = 3;
      else if (low >= LONG_LT && low <= LONG_UT && high >= LONG_LT && high <= LONG_UT)
        receivedCode[i/2] = 4;
      else if (low >= XLONG_LT && low <= XLONG_UT && high >= SHORT_LT && high <= SHORT_UT)
        receivedCode[i/2] = 5;
      else if (low >= XLONG_LT && low <= XLONG_UT && high >= LONG_LT && high <= LONG_UT)
        receivedCode[i/2] = 6;
      else if (low >= XXLONG_LT && low <= XXLONG_UT && high >= SHORT_LT && high <= SHORT_UT)
        receivedCode[i/2] = 7;
      else if (low >= XXXLONG_LT && low <= XXXLONG_UT && high >= SHORT_LT && high <= SHORT_UT)
        receivedCode[i/2] = 8;
      else {
        // error
        printf("Unexpected signal duration. Index: %d  Low: %d, High: %d\n", i, low, high );
        receivedCode[i/2] = 0;
        success = false;
      }
    }
  }
  Fernotron::codeAvailable = success;
  return Fernotron::codeAvailable;
}


void Fernotron::handleInterrupt() {
  static int duration = 0;
  static int changeCount = 0;
  static long lastTime = 0;
  static int repeatCount = 0;

  long time = micros();
  duration = time - lastTime;

  if ( (duration > XXLONG_LT && duration < XXLONG_UT)  ||  (duration > XXXLONG_LT && duration < XXXLONG_UT)) {
    repeatCount++;
    changeCount--;

    if (repeatCount == 2 && Fernotron::codeAvailable == false) { // in case there is an existing code available, don't overwrite is until the client has read it.
      if (receiveProtocol(changeCount) == false) {
        //failed
      }
      repeatCount = 0;
    }
    changeCount = 0;
  } 
  
  if (changeCount >= FERNOTRON_MAX_CHANGES) {
    changeCount = 0;
    repeatCount = 0;
  }
  
  Fernotron::timings[changeCount++] = duration;
  lastTime = time;  
}


