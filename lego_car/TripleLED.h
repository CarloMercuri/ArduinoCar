#include "Arduino.h"

#ifndef TripleLED_h
#define TripleLED_h

class TripleLED
{
public:
 /**
   * Constructor
   * Initializes the data pin to be used for communication with the DHT11 sensor.
   *
   * @param pin: Digital pin number on the Arduino board to which the DHT11 sensor is connected.
   */
  TripleLED(int redPin, int yellowPin, int greenPin);
  void toggleLEDs(bool red, bool yellow, bool green);

private:
  int _redPin;
  int _yellowPin;
  int _greenPin;

};

#endif