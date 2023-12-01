#include "udp_data.h"
#include "zbc_wifi.h"
#include "out_pins.h"
#include "net_comm.h"
#include "idc.h"
#include "CSerial.h"

// udp
//WiFiUDP Udp;
NetComm _netComm;
IDC _idc;
CSerial _cSerial;

char deviceName[] = "Racing Monitor";
char s_Hello[4] = {'_', 'H', 'L', 'L'};
char s_Telemetry[4] = {'_', 'T', 'M', 'T'};
char s_Monitor[4] = {'_', 'M', 'N', 'T'};



unsigned long lastContact = 0;
const long maxContactDelay = 2000;
const long serialDelay = 5000;
unsigned long lastSerialSend = 0;

bool carConnected = false;

// The function to be called when an '_HLL' packet is received
void HelloCallback(IPAddress address, char *data) {
  _netComm.SendEmptyPacket(&s_Monitor, address);
  lastContact = millis();
  _idc.SendDeviceConnected();
  carConnected = true;
}

// Engine speed comes from -50 to +50
// Wheels -200 to +200
void TelemetryCallback(IPAddress address, char *data) {
  // Serial.println("Telemetry Callback!");
  _idc.WriteToSerial();
  _idc.SendEngineSpeed((int16_t)data[4]);
  _idc.SendWheelsPosition((int16_t)data[5]);
  lastContact = millis();
}


void setup() {
  // put your setup code here, to run once:


  Serial.begin(9600);
  while (!Serial);
  connectWifi(); 

  _idc.Initialize();
  _cSerial.Initialize(0, 1, 2);

  attachInterrupt(3, SerialInterrupt, RISING);
  _netComm.Initialize(25000);

  _netComm.BindSignature(&s_Hello, HelloCallback);
  _netComm.BindSignature(&s_Telemetry, TelemetryCallback);
}

void SerialInterrupt(){
  _cSerial.ReadBit();
}

void loop() {

// Check if we have UDP packets first
_netComm.CheckUdp();

// If we are currently connected with the car, check if the last
// signal received from it is older than the treshold (1 sec).
// If so, we assume the connection was lost
if(carConnected){
  if( millis() - lastContact >= maxContactDelay){ 
    carConnected = false; 
    _idc.SendDeviceDisconnected();
  }
}

// For serial reading
if(millis() - lastSerialSend >= serialDelay){
  //_cSerial.WriteToSerial();
}
}


