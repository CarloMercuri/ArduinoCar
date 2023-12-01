#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include "gui.h"
#include "idc.h"
#include "CSerial.h"
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const int XP = 6, XM = A2, YP = A1, YM = 7; //ID=0x9341
const int TS_LEFT = 907, TS_RT = 136, TS_TOP = 942, TS_BOT = 139;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button on_btn, off_btn;

gui _gui;
IDC _idc;
CSerial _cSerial;

// int pixel_x, pixel_y;     //Touch_getXY() updates global vars
// bool Touch_getXY(void)
// {
//     TSPoint p = ts.getPoint();
//     pinMode(YP, OUTPUT);      //restore shared pins
//     pinMode(XM, OUTPUT);
//     digitalWrite(YP, HIGH);   //because TFT control pins
//     digitalWrite(XM, HIGH);
//     bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
//     if (pressed) {
//         pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
//         pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
//     }
//     return pressed;
// }

int loopDelay = 1000;

int testpercent = 1;
int _savedPot = 0;

void setup(void)
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    //Serial.print("TFT ID = 0x");
    //Serial.println(ID, HEX);
    //Serial.println("Calibrate for your Touch Panel");
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(1);            //PORTRAIT
    tft.fillScreen(BLACK);
    //on_btn.initButton(&tft,  60, 200, 100, 40, WHITE, CYAN, BLACK, "ON", 2);
    //off_btn.initButton(&tft, 180, 200, 100, 40, WHITE, CYAN, BLACK, "OFF", 2);
    //on_btn.drawButton(false);
    //off_btn.drawButton(false);
    _idc.Initialize();
    //Serial
    _cSerial.Initialize(0, 1, 2);
    attachInterrupt(0, SerialInterrupt, RISING);

    _gui.InitializeGUI(&tft);
    //tft.fillRect(0, 0, 160, 80, RED);
}

void SerialInterrupt(){
  _cSerial.ReadBit();
}

/* two buttons are quite simple
 */
void loop(void)
{
  //
  // Take action based on gui status
  //
  //_gui.UpdateDeviceConnectedStatus(_idc.isDeviceConnected());

  _idc.ReadSerial();
  
  switch(_gui.GuiState()){
    case 0:
      break;

    case 1:
      readEngineSpeed();
      readWheels();
      break;
  }

  //
  // Choose loopDelay
  //
  switch(_gui.GuiState()){
    case 0:
      loopDelay = 1000;
      break;

    case 1:
      loopDelay = 100;
      break;
  }

  delay(loopDelay);
}

void readWheels(){
  
  // int pot = readWheelsPosition();

  //   if(abs(pot - _savedPot) > 5){ 
  //     _savedPot = pot;
  //     if(pot > 512){
  //       _gui.SetEngineSpeed((int)map(pot, 512, 1024, 0, 50));
  //     } else {
  //       _gui.SetEngineSpeed((int)map(pot, 0, 512, -50, 0));
  //     }
  //   }  
}

void readEngineSpeed(){
  
  int pot = _idc.readEngineThrottle();
  Serial.print("Engine Read: ");
  Serial.println(pot);

    if(abs(pot - _savedPot) > 5){ 
      _savedPot = pot;
      if(pot >= 512){
        int m = (int)map(pot, 512, 1024, 0, 50);
        Serial.print("Engine mapped (>0): ");
        Serial.println(m);
        _gui.SetEngineSpeed(m);
      } else {
        int n = (int)map(pot, 0, 511, -50, 0);
         Serial.print("Engine mapped (<0): ");
        Serial.println(n);
        _gui.SetEngineSpeed(n);
      }
    }  
}