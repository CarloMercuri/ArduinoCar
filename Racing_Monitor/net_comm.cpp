#include "api/IPAddress.h"
#include "net_comm.h"
#include <Arduino.h>



  char *_pBuffer;
  char _packetBuffer[250]; //buffer to hold incoming and outgoing packets
  char s_Empty[4] = {'_', 'N', 'O', 'N'};

   /**
    * Constructor
    * Initializes the UDP communication channel on the specified port
    */
void NetComm::Initialize(uint16_t port){
  _localPort = port;
  _pBuffer = new char[250];
  _udp.begin(_localPort);

  // Initialize the callbacks save list with empty signatures (_NON means empty)
  for(int i = 0; i < 10; i++){
    memcpy(_callBacks[i].signature, s_Empty, sizeof(s_Empty[0])*4);
    
  }
}

// Checks if we have UDP packets for us. Should be called as often as possible
void NetComm::CheckUdp(){
  if ( _udp.parsePacket() ) {
    IPAddress remote = _udp.remoteIP();

    // We've received a packet, read the data from it
    _udp.read(_pBuffer, 250); // read the packet into the buffer
    
    // Send the signal that a packet has been received to the next function
    processPacket(remote);

    
  } else {
  }

}


// this will be called AFTER the packetBuffer has been set in CheckUdp()
void NetComm::processPacket(IPAddress remoteEP){

  // Needs some optimization
  char packetSignature[4] = {(char)_pBuffer[0], (char)_pBuffer[1], (char)_pBuffer[2], (char)_pBuffer[3]};

  
  this->TryMakeCallback(remoteEP, &packetSignature);

}

// Checks if the received packet has a signature (the first 4 bytes) that matches one of the mapped ones
// char (*p)[4] means that it only accepts a pointer to an array with 4 elements in
void NetComm::TryMakeCallback(IPAddress i, char (*p)[4]) {
  int found = this->CallBacksContain(&(*p));
     if( found != -1){
      // If the signature matches, call the saved function associated with that signature.
      _callBacks[found].callBackFunction(i, _pBuffer);
    }
}

/**
* Checks if the signature is loaded in the callbacks list and returns the index
* Returns -1 if not found
*/
int NetComm::CallBacksContain(char (*p)[4]) {

  // Go through the array of callbacks structs
  for(int i = 0; i < sizeof(_callBacks)/sizeof(_callBacks[0]); i++){
    // Return the index if there's a match
    if(this->compareSignatures(&_callBacks[i].signature, &(*p))){
      return i;
    }
  }

  return -1;
}

// Hardcoded cause I couldnt' get anything else to work properly
bool NetComm::compareSignatures(char (*s1)[4], char (*s2)[4]){

  if((*s1)[0] != (*s2)[0]){
    return false;
  } 

  if((*s1)[1] != (*s2)[1]) {
    return false;
  }
  if((*s1)[2] != (*s2)[2]) {
    return false;
  }
  if((*s1)[3] != (*s2)[3]) {
    return false;
  }
   
   return true;

}

// Sends an empty (signature-only, aka 4 bytes long) packet to the specified address
void NetComm::SendEmptyPacket(char (*p)[4], IPAddress address) {
  _udp.beginPacket(address, _localPort);  
  char data[4];
  data[0] = (*p)[0] ;
  data[1] = (*p)[1] ;
  data[2] = (*p)[2] ;
  data[3] = (*p)[3] ;
  _udp.write(data, 4);
  _udp.endPacket();
}


   /**
    * Binds a function to be called back on a specific signature packet
    *
    * @param signature: the signature to match for the call.
    * @param fn: the function to be called.
    */
void NetComm::BindSignature(char (*signature)[4], std::function<void(IPAddress, char *data)> fn){
  // If the signature is in the callbacks list
  if(this->CallBacksContain(&(*signature)) == -1){
    int spot = this->GetCallbacksEmptySlot();
    // If there is a free spot in the list
    if(spot != -1){
      // Load the signature
      for (int i = 0; i < 4; i++) {
        _callBacks[spot].signature[i] = (*signature)[i];
      }
      _callBacks[spot].callBackFunction = fn;
    }
  }
    
}

//
// DISCLAIMER: This could have been much easier / more simple by using a Dictionary. 
// I have NOT figured out how to make a dictionary (map) work though, and I decided
// not to waste anymore time on it, and i'm using a fixed size array instead.
//

// Checks if there is an empty (unused) spot in the callbacks list, and returns it's index if it finds one
int NetComm::GetCallbacksEmptySlot(){
  for(int i = 0; i < sizeof(_callBacks)/sizeof(_callBacks[0]); i++){
    if(compareSignatures(&_callBacks[i].signature, &s_Empty)){
      return i;
    }
    // if(strlen(_callBacks[i].signature) == 2){ // empty string is 2 in length
    //   return i;
    // }    
  }
  return -1;
}




