#include "TripleLED.h"

/**
 * Constructor for the TripleLED class.
 * Initializes the pins to be used for the red, yellow and green leds
 *
 * @param redPin: Digital pin number on the Arduino board to which the red pin is connected.
 * @param yellowPin: Digital pin number on the Arduino board to which the yellow pin is connected.
 * @param greenPin: Digital pin number on the Arduino board to which the green pin is connected.
 */
TripleLED::TripleLED(int redPin, int yellowPin, int greenPin)
{
  _redPin = redPin;
  pinMode(_redPin, OUTPUT);

  _yellowPin = yellowPin;
  pinMode(_yellowPin, OUTPUT);

  _greenPin = greenPin;
  pinMode(_greenPin, OUTPUT);

  digitalWrite(_redPin, LOW);
  digitalWrite(_yellowPin, LOW);
  digitalWrite(_greenPin, LOW);
}

void TripleLED::toggleLEDs(bool red, bool yellow, bool green){

  // Toogle red
  if(red == true){
    digitalWrite(_redPin, HIGH);
  } else {
    digitalWrite(_redPin, LOW);
  }

    // Toogle yellow
  if(yellow == true){
    digitalWrite(_yellowPin, HIGH);
  } else {
    digitalWrite(_yellowPin, LOW);
  }

    // Toogle green
  if(green == true){
    digitalWrite(_greenPin, HIGH);
  } else {
    digitalWrite(_greenPin, LOW);
  }
}