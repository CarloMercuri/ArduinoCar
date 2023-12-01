
#include <WiFi101.h>
#include "wifi_secrets.h"

// Step motors
#include <Stepper.h>
#include "stepper_data.h"
#include "EngineController.h"


#include "led_indicator.h"
#include "control_panel.h"

#include "udp_data.h"
#include <WiFiUdp.h>

#include "TripleLED.h"
#include <AccelStepper.h>

// TELEMETRY
IPAddress telemetryTransferTarget; 

// CONTROLLER
struct {
  IPAddress endPoint;
  char* controllerName;
} _activeController;

bool telemetry_transfer_active = false;
int wifi_status = WL_IDLE_STATUS; 

// buttons logic
bool btn_sendHello_pressed = false;

// udp
WiFiUDP Udp;
byte packetBuffer[ UDP_PACKET_SIZE ]; //buffer to hold incoming and outgoing packets
char scoutSignature[4] = {'_', 'S', 'C', 'T'};
char commandSignature[4] = {'_', 'C', 'M', 'D'};
char telemetryMonitorSignature[4] = {'_', 'M', 'N', 'T'};
char telemetryDataSignature[4] = {'_', 'T', 'M', 'T'};
char takeControlSignature[4] = {'_', 'C', 'T', 'R'};

char deviceName[] = "SuperRaceCar";

//byte outboundBuffer [UDP_PACKET_SIZE]; // buffer to send packets

// Car control
EngineController _engines(STEP_WHEELS_1A_PIN, STEP_WHEELS_2A_PIN, STEP_WHEELS_3A_PIN, STEP_WHEELS_4A_PIN, STEP_MOTOR_1A_PIN, STEP_MOTOR_2A_PIN, STEP_MOTOR_3A_PIN, STEP_MOTOR_4A_PIN);
int engineSpeed = 0;
signed char wheelsStatus = 0;


void setup()
{
  //
  // Initialize pins
  //

  // Engine step motor
  pinMode(STEP_MOTOR_1A_PIN, OUTPUT);
  pinMode(STEP_MOTOR_2A_PIN, OUTPUT);
  pinMode(STEP_MOTOR_3A_PIN, OUTPUT);
  pinMode(STEP_MOTOR_4A_PIN, OUTPUT);

  // Wheels step motor
  pinMode(STEP_WHEELS_1A_PIN, OUTPUT);
  pinMode(STEP_WHEELS_2A_PIN, OUTPUT);
  pinMode(STEP_WHEELS_3A_PIN, OUTPUT);
  pinMode(STEP_WHEELS_4A_PIN, OUTPUT);

  // Control panel
  pinMode(BTN_RS_PIN, INPUT);
  pinMode(BTN_TL_PIN, INPUT);
  pinMode(BTN_TR_PIN, INPUT);
  pinMode(L_SL, OUTPUT);
  pinMode(L_RL, OUTPUT);

  Serial.begin(9600);
   while (!Serial);
   Serial.println("");
   Serial.println("");
   Serial.println("");
   Serial.println("");
   Serial.println("");
   Serial.println("");
   Serial.println("");
   Serial.println("");
   Serial.println("");
  connectToWifi();
  Serial.println("------------------- NEW");

  if(wifi_status == WL_CONNECTED){
  }

  digitalWrite(L_SL, HIGH);
  digitalWrite(BTN_RS_PIN, LOW);

  digitalWrite(L_RL, HIGH);
  // start UDP
  Udp.begin(localPort);

}

void loop()
{
  checkUdp();
 //  _engines.SetEngineSpeed(300);
 //stepper1.setSpeed(50);
 //stepper1.runSpeed();
  //motor.step(200);

  if(digitalRead(BTN_TR_PIN) == HIGH){
    //_engines.TurnRight();
    if(!btn_sendHello_pressed){
      sendBroadcastHello();
      btn_sendHello_pressed = true;
    }
  } else {
    btn_sendHello_pressed = false;
  }

  if(digitalRead(BTN_TL_PIN) == HIGH) {
    //_engines.TurnLeft();
    engineSpeed = 50;
  } else {
    engineSpeed = 0;
  }

  if(engineSpeed > 0){
    _engines.RunEngine(engineSpeed);
  }

  if(wheelsStatus > 0){
    _engines.TurnRight();
  }

  if(wheelsStatus < 0) {
    _engines.TurnLeft();
  }

  if(wheelsStatus == 0){
    _engines.CenterWheels();
  }

  if(telemetry_transfer_active)   {
    Serial.println("Should send teleemtry");
    sendTelemetryData();
  }

}

void checkUdp(){
  if ( Udp.parsePacket() ) {
    // Serial.println("packet received");
    // Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i=0; i < 4; i++) {
      // Serial.print(remote[i], DEC);
      if (i < 3) {
        // Serial.print(".");
      }
    }
    // Serial.print(", port ");
    // Serial.println(Udp.remotePort());

    // We've received a packet, read the data from it
    Udp.read(packetBuffer, UDP_PACKET_SIZE); // read the packet into the buffer

    
    processPacket(remote);

    
  } else {
  }

}

void processTakeControl(IPAddress remoteEP){
  int nameLength = (int)packetBuffer[4];
  char controllerName[nameLength] = "";

  for (int i = 0; i < nameLength; i++) {
    controllerName[i] = packetBuffer[5 + i];
  }


  _activeController.endPoint = remoteEP;
  strncpy(_activeController.controllerName, controllerName, sizeof(_activeController.controllerName));
}

void processRemoteCommand(IPAddress remoteEP){
  if(remoteEP != _activeController.endPoint){
    return;
  }

  int _engineSpeed = (int)packetBuffer[4];
  signed char _wheels = (signed char)packetBuffer[5];
  int _reset = (int)packetBuffer[6];

  engineSpeed = _engineSpeed;
  wheelsStatus = _wheels;

  Serial.println("Engine Speed: ");
  Serial.print(engineSpeed);
  Serial.println("Wheels status: ");
  Serial.println(wheelsStatus);

  // Serial.println(_engineSpeed);
  // Serial.println(_wheels);
  // Serial.println(_reset);

  if(_reset > 0){
    _engines.ResetWheelsCalibration();
  }
}


// this will be called AFTER the packetBuffer has been set
void processPacket(IPAddress remoteEP){

  char packetSignature[4] = {(char)packetBuffer[0], (char)packetBuffer[1], (char)packetBuffer[2], (char)packetBuffer[3]};


  if(compareSignatures(packetSignature, scoutSignature)){
      sendScoutResponsePacket(remoteEP);
      return;
  } 

    if(compareSignatures(packetSignature, commandSignature)){
      processRemoteCommand(remoteEP);
      return;
  } 

    if(compareSignatures(packetSignature, takeControlSignature)){
      processTakeControl(remoteEP);
      return;
  }

  if(compareSignatures(packetSignature, telemetryMonitorSignature)){
    Serial.println("TMT");
    telemetryTransferTarget = remoteEP;
    telemetry_transfer_active = true;
  }

  //  if(compareSignatures(packetSignature, trStartSignature)){
  //     startTransfer(remoteEP);
  //     return;
  // } 

  //  if(compareSignatures(packetSignature, trStopSignature)){
  //     stopTransfer();
  //     return;
  // } 
}

void sendTelemetryData() {
  byte data[64];

  // signature
  char signature[] = "_TMT";

  data[0] = signature[0];  
  data[1] = signature[1];  
  data[2] = signature[2];  
  data[3] = signature[3];

  data[4] = engineSpeed;
  data[5] = _engines.GetWheelsPosition();



  data[6] = strlen(_activeController.controllerName);

  for (int i = 0; i < strlen(_activeController.controllerName) ; i++) {
    data[6 + i] = _activeController.controllerName[i];
  }




   Udp.beginPacket(telemetryTransferTarget, localPort);  
   Udp.write(data, 64);
   Udp.endPacket();

   Serial.println("Sent telemetry");
}


void sendScoutResponsePacket(IPAddress remoteEP) {
  // Packet Structure:
  // [ b0, b1, b2, b3 ] packet signature (_SRP)
  // [ b4 ] device name's string length
  // [ b5, ... ] device name

  Serial.println("Sending Scout Response");


  byte data[UDP_PACKET_SIZE];

  // signature
  char signature[] = "_SRP";

  data[0] = signature[0];  
  data[1] = signature[1];  
  data[2] = signature[2];  
  data[3] = signature[3];

  // length of the device name
  data[4] = (char)strlen(deviceName);

  // device name
  for(int i = 0; i < strlen(deviceName); i++){
    data[i + 5] = deviceName[i];
  }


   Udp.beginPacket(remoteEP, localPort);  
   Udp.write(data, UDP_PACKET_SIZE);
   Udp.endPacket();
}

void sendBroadcastHello(){
  
  Serial.println("Sending Broadcast Hello");
  //192.168.1.117
  IPAddress broadcastIp(255, 255, 255, 255);
  byte data[64];

  // signature
  char signature[] = "_HLL";

  data[0] = signature[0];  
  data[1] = signature[1];  
  data[2] = signature[2];  
  data[3] = signature[3];

  // length of the device name
  data[4] = (char)strlen(deviceName);

  // device name
  for(int i = 0; i < strlen(deviceName); i++){
    data[i + 5] = deviceName[i];
  }


   Udp.beginPacket(broadcastIp, localPort);  
   Udp.write(data, 64);
   Udp.endPacket();
}

// Again, manual mode because automatic (strcmp or for loop) don't seem to work.
bool compareSignatures(char s1[], char s2[]){

  if(s1[0] != s2[0]){
    return false;
  } 

  if(s1[1] != s2[1]) {
    return false;
  }
  if(s1[2] != s2[2]) {
    return false;
  }
  if(s1[3] != s2[3]) {
    return false;
  }
   
   return true;

}


void connectToWifi(){
  // attempt to connect to Wifi network:
  while (wifi_status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(wifi_ssid);
    // Connect to WPA/WPA2 network:
    wifi_status = WiFi.begin(wifi_ssid, wifi_pass);

    // wait a bit for connection:
    delay(3000);
  }

  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");

  Serial.println("----------------------------------------");
  printData();
  Serial.println("----------------------------------------");
}

void printData() {
  Serial.println("Board Information:");
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println();
  Serial.println("Network Information:");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

}

