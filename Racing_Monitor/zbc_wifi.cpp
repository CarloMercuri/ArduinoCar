#include "zbc_wifi.h"
#include "wifi_secrets.h"

int wifi_status = WL_IDLE_STATUS; 

void connectWifi() {
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
  Serial.println("");

}
