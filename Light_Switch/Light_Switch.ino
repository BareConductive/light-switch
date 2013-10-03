/**********************************************************
       Bare Conductive Light Switch Demo for Kickstarter
 **********************************************************
  
  Code is by Stefan Dzisiewski-Smith  

***********************************************************/

// enable serial
//#define SERIAL_MODE

// serial rate
#define baudRate 57600

// touch shield includes
#include "mpr121_setup.h"
#include <Wire.h>

// touch shield variables
int irqpin = 3;

#define touchPin 11
#define outputPin 11

unsigned int touchStatus=0;
unsigned int lastTouchStatus=0; 

void setup(){
  pinMode(irqpin, INPUT);
  digitalWrite(irqpin, HIGH); //enable pullup resistor
  
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, HIGH);
  
  #ifdef SERIAL_MODE
    Serial.begin(baudRate);
    Serial.println("Light Switch Demo"); 
    while(!Serial);
  #endif
  
  Wire.begin();

  mpr121_setup();
}

void loop(){
   readRawInputs();
   #ifdef SERIAL_MODE 
     //Serial.println(touchStatus, BIN); 
   #endif

   if(((touchStatus>>touchPin)&1) && !((lastTouchStatus>>touchPin)&1)){
     digitalWrite(outputPin, !digitalRead(outputPin));  
   }
   
   lastTouchStatus = touchStatus;

}

void readRawInputs(){
  
    uint8_t MSB, LSB;
    uint16_t touchedVals;
    int i, returned;
    
    Wire.requestFrom(MPR121_ADDR, 2);
    LSB = Wire.read();
    MSB = Wire.read();
    
    touchStatus = (unsigned int)LSB + ((unsigned int)MSB<<8);

}



// helper functions to keep the above code clean

boolean checkInterrupt(void){
  return digitalRead(irqpin);
}
