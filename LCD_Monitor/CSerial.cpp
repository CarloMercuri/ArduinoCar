#include <stdint.h>
#include "CSerial.h"
#include <Arduino.h>

unsigned char mask[] = {1, 2, 4, 8, 16, 32, 64, 128};


void CSerial::Initialize(int clockPin, int receivePin, int sendPin){
  pinMode(_clockPin, OUTPUT);
  pinMode(_sendPin, OUTPUT);
  pinMode(_receivePin, INPUT);
  digitalWrite(_clockPin, LOW);
  
}

void CSerial::ReadBit() {
  Serial.print("Received: ");
  if(_receivePin == HIGH){
    Serial.println("1");
  } else {
    Serial.println("0");
  }
}

void CSerial::WriteToSerial(char *data, int16_t length){
  for(int i = 0; i < length; i++){
    // Go through each byte in the array
     for(int j = 0; j < 8; j++){
       if((data[i] && mask[j]) == 0){
          digitalWrite(_sendPin, LOW);          
       } else {
          digitalWrite(_sendPin, HIGH);    
       }

        delayMicroseconds(150);
        digitalWrite(_clockPin, HIGH);
        delay(1);
        digitalWrite(_clockPin, LOW);
     }
    
  }
}