// INTER-DEVICE COMMUNICATION
#include <Arduino.h>

class IDC
{
  public:
     IDC() = default;
    ~IDC() = default;

    void Initialize();
    void WriteToSerial();
    void SendEngineSpeed(int16_t speed);
    void SendWheelsPosition(int16_t position);
    void SendDeviceConnected();
    void SendDeviceDisconnected();
    
  private:
};