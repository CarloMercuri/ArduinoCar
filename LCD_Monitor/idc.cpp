#include "idc.h"
#include <SoftwareSerial.h>


#define rxPin 51
#define txPin 49

// Set up a new SoftwareSerial object
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

char buffer[20];
int bufferIndex = 0;

void IDC::Initialize(){
  pinMode(DEVICE_CONNECTED_PIN, INPUT);
  pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
      // Set the baud rate for the SoftwareSerial object
    mySerial.begin(9600);
}

void IDC::ReadSerial(){
    if (mySerial.available() > 0) {
        buffer[bufferIndex] = mySerial.read();
        bufferIndex++;
        if(bufferIndex > 19) bufferIndex = 19;
    } else {
      bufferIndex = 0;
      Serial.print("From Serial: ");
      Serial.println(buffer);
    }
}

bool IDC::isDeviceConnected() {
  if(digitalRead(DEVICE_CONNECTED_PIN) == HIGH ){
    return true;
  } else {
    return false;
  }
}

int IDC::readEngineThrottle() {
  return analogRead(ENGINE_THROTTLE_PIN);
}

int IDC::readWheelsPosition() {
  return analogRead(WHEELS_POSITION_PIN);
}