//#include <Stepper.h>
#include "CarloStepper.h"
#include <Arduino.h>

const int WHEELS_MAX_TURN_RIGHT = 180;
const int WHEELS_MAX_TURN_LEFT = -180;
const int WHEELS_TURN_BLOCKS = 2;
const int ENGINE_FORWARD_STEPS = 2;
const int ENGINE_REVERSE_STEPS = -2;

class EngineController
{
  public:
    /**
    * Constructor
    * Initializes the engine controller with the specified motor type, and pins
    *
    * @param pin1: the Arduino pin for stepper pin1.
    * @param pin2: the Arduino pin for stepper pin2.
    * @param pin3: the Arduino pin for stepper pin3.
    * @param pin4: the Arduino pin for stepper pin4.
    */
    EngineController(int wp1, int wp2, int wp3, int wp4, int ep1, int ep2, int ep3, int ep4);
    void SetEngineSpeed(int speed);
    void TurnRight();
    void TurnLeft();
    void CenterWheels();
    void ResetWheelsCalibration();
    void RunEngine(int speed);
    int GetWheelsPosition();
    

  private:
    CarloStepper *_engineStepper;
    CarloStepper *_wheelsStepper;
    int _wheelsPosition;
    int _engineSpeed;
};