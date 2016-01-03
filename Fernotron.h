/*
  RCSwitch - Arduino libary for remote control outlet switches
  Copyright (c) 2011 Suat Özgür.  All right reserved.

  Contributors:
  - Andre Koehler / info(at)tomate-online(dot)de
  - Gordeev Andrey Vladimirovich / gordeev(at)openpyro(dot)com
  
  Project home: http://code.google.com/p/rc-switch/

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
#ifndef _Fernotron_h
#define _Fernotron_h

#include <wiringPi.h>
#include <stdint.h>

#define NULL 0
#define CHANGE 1
  
#ifdef __cplusplus
extern "C"{
#endif
  
typedef uint8_t boolean;
typedef uint8_t byte;
  
#if !defined(NULL)
#endif

#ifdef __cplusplus
}
#endif


// Maximum High/Low changes within a valid protocol packet
#define FERNOTRON_MAX_CHANGES 300  // actually just 280
#define FERNOTRON_PROTOCOL_LENGTH 140


class Fernotron {

  public:
    Fernotron();
 
    // send related methods
    bool enableTransmit(int nTransmitterPin);
    void setRepeatTransmit(int nRepeatTransmit);
    void send (char* code);    

    // receive related methods
    bool enableReceive(int interrupt);
    bool isAvailable();
    void resetAvailable();
    int* getReceivedCode();
    int* getReceivedRawdata();
  
  private:
    int nTransmitterPin;
    int nRepeatTransmit;

    int nReceiverInterrupt;
    static bool receiveProtocol(int changeCount);
    static void handleInterrupt();
    
    static int timings[FERNOTRON_MAX_CHANGES];
    static int receivedCode[FERNOTRON_PROTOCOL_LENGTH];
    static bool codeAvailable;
    
};

#endif
