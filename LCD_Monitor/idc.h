// Inter-Device Connections
#include <Arduino.h>

#define DEVICE_CONNECTED_PIN 35
#define ENGINE_THROTTLE_PIN 10
#define WHEELS_POSITION_PIN 11

class IDC
{
  public:
    IDC() = default;
    ~IDC() = default;

    void Initialize();
    void ReadSerial();
    bool isDeviceConnected();
    int readEngineThrottle();
    int readWheelsPosition();

  private:
};

