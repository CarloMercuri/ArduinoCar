#include "EngineController.h"



/**
 * Constructor for the TripleLED class.
 * Initializes the pins to be used for the red, yellow and green leds
 *
 * @param redPin: Digital pin number on the Arduino board to which the red pin is connected.
 * @param yellowPin: Digital pin number on the Arduino board to which the yellow pin is connected.
 * @param greenPin: Digital pin number on the Arduino board to which the green pin is connected.
 */
EngineController::EngineController(int wp1, int wp2, int wp3, int wp4, int ep1, int ep2, int ep3, int ep4)
{
  _wheelsStepper = new CarloStepper(2048, wp1, wp3, wp2, wp4); 
  _engineStepper = new CarloStepper(2048, ep1, ep3, ep2, ep4); 
  _wheelsStepper->setSpeed(10);
  _engineStepper->setSpeed(15);
  _wheelsPosition = 0;
  
}

int EngineController::GetWheelsPosition(){
  return _wheelsPosition;
}

void EngineController::SetEngineSpeed(int speed){
  _engineStepper->setSpeed(speed);
}

void EngineController::RunEngine(int speed){
  if(_engineSpeed != speed){
    _engineSpeed = speed;
    _engineStepper->setSpeed((int)map(abs(speed), 0, 50, 0, 14));
  }
  
  if(speed > 0){
    _engineStepper->step(ENGINE_FORWARD_STEPS);
  } else if (speed < 0) {
  _engineStepper->step(ENGINE_REVERSE_STEPS);
  }
 
}

/*
* Calibrates the wheel position and sets the position to centered.
* USAGE: Manually set the wheels to center position, then use this function
*/
void EngineController::ResetWheelsCalibration(){
 _wheelsPosition = 0;
}

void EngineController::CenterWheels(){
  if(_wheelsPosition == 0) return;

  if(_wheelsPosition < 0){
    _wheelsStepper->step(abs(_wheelsPosition));
  } else {
    _wheelsStepper->step(-_wheelsPosition);
  }
}

void EngineController::TurnRight(){
  // Serial.println(_wheelsPosition);
  // Serial.println(WHEELS_MAX_TURN_RIGHT);
  // Serial.println("-------------");
  // Don't do anything if it's already at max turn
  if(_wheelsPosition >= WHEELS_MAX_TURN_RIGHT){
    return;
  }

  // Otherwise, we need to check if it can perform a normal step group (50 steps) without hitting the cap
  // If the cap is 200, we could be at 180 and by doing a normal 50 steps group we would exceed the limit
  // So in that case, we would only turn 20
  if(_wheelsPosition + WHEELS_TURN_BLOCKS > WHEELS_MAX_TURN_RIGHT) {
      int partialStep = WHEELS_MAX_TURN_RIGHT - _wheelsPosition;
      _wheelsStepper->step(partialStep);
      Serial.println("Partial");
      Serial.println(partialStep);
      Serial.println("--------");
      _wheelsPosition = _wheelsPosition + partialStep;
  } else { // Otherwise just perform a normal step group
    _wheelsStepper->step(WHEELS_TURN_BLOCKS);
      Serial.println("Normal");
      Serial.println(WHEELS_TURN_BLOCKS);
      Serial.println("--------");
    _wheelsPosition = _wheelsPosition + WHEELS_TURN_BLOCKS;
  }
  Serial.println(_wheelsPosition);
}

void EngineController::TurnLeft(){
 if(_wheelsPosition <= WHEELS_MAX_TURN_LEFT){
    return;
  }

  if(_wheelsPosition - WHEELS_TURN_BLOCKS < WHEELS_MAX_TURN_LEFT) {
      int partialStep = abs(WHEELS_MAX_TURN_LEFT + _wheelsPosition);
      _wheelsStepper->step(-partialStep);
      _wheelsPosition = _wheelsPosition - partialStep;
  } else {
    _wheelsStepper->step(-WHEELS_TURN_BLOCKS);
    _wheelsPosition = _wheelsPosition - WHEELS_TURN_BLOCKS;
  }
}

