

class CSerial
{
  public:
    void Initialize(int clockPin, int receivePin, int sendPin);
    void WriteToSerial(char *data, int16_t length);
    void ReadBit();
    
  private:
    int _clockPin = 0;
    int _sendPin = 1;
    int _receivePin = 2;
    const long newSerialDataDelay = 5;
    unsigned long lastSerialIncoming = 0;
};