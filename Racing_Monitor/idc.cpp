#include "idc.h"
#include "out_pins.h"
#include <wiring_private.h>


// Serial2 pin and pad definitions (in Arduino files Variant.h & Variant.cpp)
#define PIN_SERIAL2_RX       (1ul)                // Pin description number for PIO_SERCOM on D1
#define PIN_SERIAL2_TX       (0ul)                // Pin description number for PIO_SERCOM on D0
#define PAD_SERIAL2_TX       (UART_TX_PAD_0)      // SERCOM pad 0 TX
#define PAD_SERIAL2_RX       (SERCOM_RX_PAD_1)    // SERCOM pad 1 RX

// Instantiate the Serial2 class
Uart Serial2(&sercom3, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);

  void IDC::Initialize(){
      Serial2.begin(115200);          // Begin Serial2 
      pinPeripheral(0, PIO_SERCOM);   // Assign pins 0 & 1 SERCOM functionality
      pinPeripheral(1, PIO_SERCOM);
      // pinMode(PINOUT_DEVICE, OUTPUT);
      // pinMode(PINOUT_ENGINE, OUTPUT);
      // pinMode(PINOUT_WHEELS, OUTPUT);
      digitalWrite(PINOUT_DEVICE, LOW);
      Serial.println("InterDevice Connections initialized");
  }

  void SERCOM3_Handler()    // Interrupt handler for SERCOM3
  {
    Serial2.IrqHandler();
  }

  void IDC::WriteToSerial(){
    Serial.println("Writing to serial");
     Serial2.write('_'); 
     Serial2.write('T'); 
     Serial2.write('M'); 
     Serial2.write('T'); 
     Serial2.write(30); 
     Serial2.write(-50); 
  }

// void ReadSerial(){
//   if (Serial2.available()){        // Check if incoming data is available  
//     byte byteRead = Serial2.read();    // Read the most recent byte
//     Serial2.write(byteRead);           // Echo the byte back out on the serial port
//   }
// }

  void IDC::SendEngineSpeed(int16_t speed){
      //Serial.println("Sending engine speed");
      int r = map(speed, -50, 50, 0, 1024);
      analogWrite(PINOUT_ENGINE, map(speed, -50, 50, 0, 1024));
  }

  void IDC::SendWheelsPosition(int16_t position){
    //Serial.println("Sending wheels speed");
    analogWrite(PINOUT_WHEELS, map(position, -200, 200, 0, 1024));
  }

  void IDC::SendDeviceConnected(){
    Serial.println("Device Connected");
    digitalWrite(PINOUT_DEVICE, HIGH);
  }

  void IDC::SendDeviceDisconnected(){
    Serial.println("Device disconnected");
        digitalWrite(PINOUT_DEVICE, LOW);
  }