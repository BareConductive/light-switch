/*******************************************************************************

 Bare Conductive Light Switch Demo
 ---------------------------------
 
 Light_Switch.ino - simple touch light switch with toggle action (touch-on, 
 touch-off)
 
 Bare Conductive code written by Stefan Dzisiewski-Smith and Peter Krige.
 
 This work is licensed under a MIT license https://opensource.org/licenses/MIT
 
 Copyright (c) 2016, Bare Conductive
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*******************************************************************************/

// compiler error handling
#include "Compiler_Errors.h"

#include <MPR121.h>
#include <Wire.h>

#define switchElectrode 11
#define outputPin 13

void setup(){
  
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, LOW);
  
  Serial.begin(9600);
  
  // 0x5C is the MPR121 I2C address on the Bare Touch Board
  if(!MPR121.begin(0x5C)){ 
    Serial.println("error setting up MPR121");  
    switch(MPR121.getError()){
      case NO_ERROR:
        Serial.println("no error");
        break;  
      case ADDRESS_UNKNOWN:
        Serial.println("incorrect address");
        break;
      case READBACK_FAIL:
        Serial.println("readback failure");
        break;
      case OVERCURRENT_FLAG:
        Serial.println("overcurrent on REXT pin");
        break;      
      case OUT_OF_RANGE:
        Serial.println("electrode out of range");
        break;
      case NOT_INITED:
        Serial.println("not initialised");
        break;
      default:
        Serial.println("unknown error");
        break;      
    }
    while(1);
  }
  
  // pin 4 is the MPR121 interrupt on the Bare Touch Board
  MPR121.setInterruptPin(4);

  MPR121.setTouchThreshold(40);     // this is the threshold at which the board senses a touch
                                    // higher values are less sensitive, lower values are more sensitive
                                    // for proximity operation, you could try a value of 6

  MPR121.setReleaseThreshold(20);   // this is the threshold at which the board senses a release
                                    // higher values are less sensitive, lower values are more sensitive
                                    // for proximity operation, you could try a value of 3
                                    // this must ALWAYS be lower than the touch threshold

  // initial data update
  MPR121.updateTouchData();
}

void loop(){
  if(MPR121.touchStatusChanged()){
    MPR121.updateTouchData();
    if(MPR121.isNewTouch(switchElectrode)){
      digitalWrite(outputPin, !digitalRead(outputPin));      
    }  
  }
}
