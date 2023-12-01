#include "bsp/include/nm_bsp.h"
#include <iostream>
#include <string>
#include <functional>


#include <WiFiUdp.h>

#define BUFFER_ARRAY_SIZE 250

struct CallBackData {
  std::function<void(IPAddress, char* data)> callBackFunction;
  char signature[4];
} ;

class NetComm
{
public:
  NetComm() = default;
  ~NetComm() = default;

  void Initialize(uint16_t port);
  void CheckUdp();
  void BindSignature(char (*signature)[4], std::function<void(IPAddress, char *data)> fn);
  void SendEmptyPacket(char (*p)[4], IPAddress address);

  

private:
  uint16_t _localPort;
  int CallBacksContain(char (*p)[4]);
  CallBackData _callBacks[10];
  void TryMakeCallback(IPAddress i, char (*p)[4]); 
  WiFiUDP _udp;
  void processPacket(IPAddress remoteEP);
  bool compareSignatures(char (*s1)[4], char (*s2)[4]);
  int GetCallbacksEmptySlot();
};