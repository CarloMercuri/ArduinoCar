#include <WiFi101.h>

#include <DFRobot_DHT11.h>
#include "joystick.h"
#include "wifi_secrets.h"
#include "udp_data.h"
#include <WiFiUdp.h>

char deviceName[] = "CarloArduino";
char scoutSignature[4] = {'_', 'S', 'C', 'T'};
char trStartSignature[4] = {'_', 'T', 'S', 'P'};
char trStopSignature[4] = {'_', 'T', 'S', 'T'};
IPAddress dataTransferTarget; 
bool dataTransferActive = false;


// Wifi
#define DHT11_PIN 7
DFRobot_DHT11 DHT11ROBOT;
int wifi_status = WL_IDLE_STATUS;     

// Udp
WiFiUDP Udp;
byte packetBuffer[ UDP_PACKET_SIZE ]; //buffer to hold incoming and outgoing packets
byte outboundBuffer [UDP_PACKET_SIZE]; // buffer to send packets

// States
int joystick_x_axis;
int joystick_y_axis;
int temperature;
int humidity;

void setup() {
  analogReference(AR_DEFAULT);
  Serial.begin(9600); // 115200
  //pinMode(JSTICK_AXIS_PIN_X, INPUT_PULLDOWN);
  while (!Serial);
  connectWifi();
  Serial.println("------------------- NEW");
  // start UDP
  Udp.begin(localPort);
}

void loop() {

  // Read all the inputs, and write them to the global variables
  readInputs(); 

  // Check if we have received a packet
  checkUdp();

  // If allowed, send a data packet
  if(dataTransferActive == true) {
    sendDataTransferPacket(temperature, humidity, joystick_x_axis, joystick_y_axis);
  }

  delay(50);



  return;

}

void readInputs(){
    DHT11ROBOT.read(DHT11_PIN);

  temperature = DHT11ROBOT.temperature;
  humidity = DHT11ROBOT.humidity;

  // flooring the value sso we basically get a value from 0 to 100.
  // Easier for everyone
  joystick_x_axis = floor(readJoystickAxis(AXIS_X) / 10) * 2;
  joystick_y_axis = floor(readJoystickAxis(AXIS_Y) / 10) * 2;

    // Serial.print("temp:");
  // Serial.print(DHT11ROBOT.temperature);+
  // Serial.print("  humi:");
  // Serial.println(DHT11ROBOT.humidity);
  //  Serial.println("Joystick X:");
    Serial.println(joystick_x_axis);
    //Serial.println(joystick_y_axis);
}

 void sendDataTransferPacket(int temperature, int humidity, int axis_x, int axis_y){
  // We are writing to the outboundBuffer buffer byte array, so we need to keep track of the index.
  // The structure is shown in the documentation, but in short:
  // [ b0, b1, b2, b3] are the packet signature, in this case _DTP
  // [ b4 ] is the number of variables present in the packet
  // Starting with b5 it repeats the following pattern:
  // 1 byte to store the variable name's string length.
  // x bytes to store the variable name
  // 1 byte to store the variable TYPE (1 = short, 2 = int, 3 = boolean)
  // x bytes to store the variable's value
 
  // Keeping track of where we are in the buffer
  int index = 0;

  // write signature
  outboundBuffer[0] = '_';
  outboundBuffer[1] = 'D';
  outboundBuffer[2] = 'T';
  outboundBuffer[3] = 'P';

  // number of variables, hardcoded for now
  outboundBuffer[4] = 4;
  index = 5;

  //
  // write variables
  //

  ////////// TEMPERATURE ///////////

  // length of "temperature"
  char tmp[] = "temperature";
  size_t tmpSize = strlen(tmp);
  buff_writeIntVariable("temperature", tmpSize, temperature, &index);

  ////////// HUMIDITY ///////////

    // length of "humidity"
  char hum[] = "humidity";
  size_t humSize = strlen(hum);
  buff_writeIntVariable("humidity", humSize, humidity, &index);

  ////////// JOYSTICK X AXIS ///////////

    // length of "xaxis"
  char xa[] = "xaxis";
  size_t xaSize = strlen(xa);
  buff_writeIntVariable("xaxis", xaSize, axis_x, &index);

  ////////// JOYSTICK Y AXIS ///////////

    // length of "yaxis"
  char ya[] = "yaxis";
  size_t yaSize = strlen(ya);
  buff_writeIntVariable("yaxis", yaSize, axis_y, &index);




  sendPacket();
  //Serial.println("Sent tdp");
}

// Getting the variable name's pointer to the start of the char array, the length of it,
// the value contained by the variable and a pointer to the index of where we are in the buffer
void buff_writeIntVariable(const char* varName, size_t varName_length, int value, int *index){

  // Add variable name length
  outboundBuffer[*index] = varName_length;
*index = *index + 1;



  //add the variable name, smart mode but not working

   for(int i = 0; i < varName_length; ++i) {

        outboundBuffer[*index + i] = (char)varName[i];
        //*index = *index + 1;

    }

    *index = *index + varName_length;

  // for(int i = 0; i < varName_length; i++) {
  //   //Serial.println(varName[i]);
  //   outboundBuffer[*index + i] = varName[i];
  //   *index = *index + 1;
  // }
  
  // Add the variable name to the buffer, hardcoded mode.

  // if(varName[0] == 't'){
  //   writeTemperature(&*index);
  // }

  //   if(varName[0] == 'h'){
  //   writeHumidity(&*index);
  // }

  //   if(varName[0] == 'x'){
  //   writeX_Axis(&*index);
  // }

  //   if(varName[0] == 'y'){
  //   writeY_Axis(&*index);
  // }
     

  outboundBuffer[*index] = 1; // type short (2 bytes int)

  *index = *index + 1;

  // Add the value itself

  int tmp = value;
  for(int i=0;i<2;++i)
  {
    outboundBuffer[*index + i]=(tmp& 0xFF);
    tmp=tmp>>8;
    *index = *index + 1;
  }
}

// Hardcoded way to add the word "temperature" to the buffer array.
void writeTemperature(int *index){
  outboundBuffer[*index] = 't';
     *index = *index + 1;
  outboundBuffer[*index] = 'e';
     *index = *index + 1;
  outboundBuffer[*index] = 'm';
     *index = *index + 1;
  outboundBuffer[*index] = 'p';
     *index = *index + 1;
  outboundBuffer[*index] = 'e';
     *index = *index + 1;
  outboundBuffer[*index] = 'r';
     *index = *index + 1;
  outboundBuffer[*index] = 'a';
     *index = *index + 1;
  outboundBuffer[*index] = 't';
     *index = *index + 1;
  outboundBuffer[*index] = 'u';
     *index = *index + 1;
  outboundBuffer[*index] = 'r';
     *index = *index + 1;
  outboundBuffer[*index] = 'e';
     *index = *index + 1;
}

// Hardcoded way to add the word "humidity" to the buffer array.
void writeHumidity(int *index){
  outboundBuffer[*index] = 'h';
     *index = *index + 1;
  outboundBuffer[*index] = 'u';
     *index = *index + 1;
  outboundBuffer[*index] = 'm';
     *index = *index + 1;
  outboundBuffer[*index] = 'i';
     *index = *index + 1;
  outboundBuffer[*index] = 'd';
     *index = *index + 1;
  outboundBuffer[*index] = 'i';
     *index = *index + 1;
  outboundBuffer[*index] = 't';
     *index = *index + 1;
  outboundBuffer[*index] = 'y';
     *index = *index + 1; 
}

// Hardcoded way to add the word "xaxis" to the buffer array.
void writeX_Axis(int *index) {
  outboundBuffer[*index] = 'x';
     *index = *index + 1;
  outboundBuffer[*index] = 'a';
     *index = *index + 1;
  outboundBuffer[*index] = 'x';
     *index = *index + 1;
  outboundBuffer[*index] = 'i';
     *index = *index + 1;
  outboundBuffer[*index] = 's';
     *index = *index + 1;         

}

// Hardcoded way to add the word "yaxis" to the buffer array.
void writeY_Axis(int *index) {
  outboundBuffer[*index] = 'y';
     *index = *index + 1;
  outboundBuffer[*index] = 'a';
     *index = *index + 1;
  outboundBuffer[*index] = 'x';
     *index = *index + 1;
  outboundBuffer[*index] = 'i';
     *index = *index + 1;
  outboundBuffer[*index] = 's';
     *index = *index + 1;         
}

void checkUdp(){
  if ( Udp.parsePacket() ) {
    Serial.println("packet received");
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i=0; i < 4; i++) {
      Serial.print(remote[i], DEC);
      if (i < 3) {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // We've received a packet, read the data from it
    Udp.read(packetBuffer, UDP_PACKET_SIZE); // read the packet into the buffer

    
    processPacket(remote);

    
  } else {
  }

}

// this will be called AFTER the packetBuffer has been set
void processPacket(IPAddress remoteEP){

  char packetSignature[4] = {(char)packetBuffer[0], (char)packetBuffer[1], (char)packetBuffer[2], (char)packetBuffer[3]};


  if(compareSignatures(packetSignature, scoutSignature)){
      sendScoutResponsePacket(remoteEP);
      return;
  } 

   if(compareSignatures(packetSignature, trStartSignature)){
      startTransfer(remoteEP);
      return;
  } 

   if(compareSignatures(packetSignature, trStopSignature)){
      stopTransfer();
      return;
  } 
}

// Set the target, and start transfering data
void startTransfer(IPAddress remoteEP){
  dataTransferTarget = remoteEP;
  dataTransferActive = true;
}

void stopTransfer(){
  dataTransferActive = false;
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

void sendPacket(){
  
   Udp.beginPacket(dataTransferTarget, localPort);  
   Udp.write(outboundBuffer, UDP_PACKET_SIZE);
   Udp.endPacket();
}

int readJoystickAxis(int axis){
  switch(axis){
    case AXIS_X: 
      return analogRead(JSTICK_AXIS_PIN_X);
      break;
    case AXIS_Y:
        
      return analogRead(JSTICK_AXIS_PIN_Y);
      break;
  }
}

void connectWifi(){
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
