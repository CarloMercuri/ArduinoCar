#include "gui.h"

#define SCREEN_WIDTH 399
#define SCREEN_HEIGHT 239

int _engineSlider_Height = 180;
int _engineSlider_HalfHeight = 90;
int _engineSlider_Width = 60;
int _engineSlider_x = 20;
int _engineSlider_y = 30;

int _firstScreen_frame = 0;

bool debugConnectedStatus = true;

void gui::InitializeGUI(MCUFRIEND_kbv *_tft){
      tft = _tft;
      _guiState = 0;
      //this->InitializeMonitorStateScreen();
      this->InitializeFirstPage();
    // Main screen background color
   
    // fill
    ///tft->fillRect(2, 2, SCREEN_WIDTH - 4, SCREEN_HEIGHT - 4, GRAY_MID);
}

int gui::GuiState(){
  return _guiState;
}

void gui::InitializeFirstPage() {
  // Clean up first
  tft->fillScreen(BLACK);

  this->printMsgXY(100, 110, 1, YELLOW, &FreeSans12pt7b, "Waiting on Device");
  tft->fillRect(290, 109, 10, 2, BLACK);
  tft->fillRect(290, 109, 2, 2, YELLOW);

  //  this->printMsgXY(80, 170, 1, WHITE, &FreeSans9pt7b, "Devices Status:");
  // this->printMsgXY(230, 170, 1, RED, &FreeSans9pt7b, "DISCONNECTED");
    

}

void gui::UpdateDeviceConnectedStatus(bool deviceConnected){
  if(_guiState == 0){
     if(deviceConnected){
      this->TransitionToState(1);
      return;
     }
      tft->fillRect(290, 109, 10, 2, BLACK);
    for(int i = 0; i < _firstScreen_frame; i++){
       tft->fillRect(290 + (i * 3), 109, 2, 2, YELLOW);
    }
    _firstScreen_frame++;

    if(_firstScreen_frame > 3) _firstScreen_frame = 0;

    // TEMPORARY
    // if(debugConnectedStatus != deviceConnected){
    //   tft->fillRect(230, 150, 180, 70, BLACK);
    //     if(deviceConnected){
    //       this->printMsgXY(230, 170, 1, GREEN, &FreeSans9pt7b, "CONNECTED");
    //     } else {
    //       this->printMsgXY(230, 170, 1, RED, &FreeSans9pt7b, "DISCONNECTED");
    //   }

    //   debugConnectedStatus = deviceConnected;
    // }
   

  } else if (_guiState == 1) {
    if(!deviceConnected){
      this->TransitionToState(0);
    }
  }
}

void gui::TransitionToState(int16_t state){
  if(state == 1){
    // Clean up first
    tft->fillScreen(BLACK);

     this->printMsgXY(100, 110, 1, GREEN, &FreeSans12pt7b, "DEVICE FOUND!");
     delay(1000);
      int x = SCREEN_WIDTH;
     while(x >= 0){
      tft->drawFastVLine(x, 0, SCREEN_HEIGHT, YELLOW); 
      tft->drawFastVLine(x + 1, 0, SCREEN_HEIGHT, BLACK); 
      x--;     
     }

    _guiState = 1;
     this->InitializeMonitorStateScreen();
  } else if(state ==0){
     // Clean up first
    tft->fillScreen(BLACK);
    this->printMsgXY(100, 110, 1, RED, &FreeSans12pt7b, "DEVICE DISCONNECTED");
    _guiState = 0;
    delay(1000);
    this->InitializeFirstPage();

  }
}

void gui::printMsgXY(int16_t x, int16_t y, int16_t size, int16_t color, const GFXfont *f, const char *msg)
{
    tft->setFont(f);
    tft->setCursor(x, y);
    tft->setTextColor(color);
    tft->setTextSize(size);
    tft->print(msg);
}

void gui::DrawInwardsPanel(int16_t x, int16_t y, int16_t width, int16_t height, int16_t thickness, int16_t color) {

  //
  //          c2 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1
  //          c2 c2 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1
  //          c2 c2 c2 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1 c1
  //          c2 c2 c2                                  c1 c1 c1
  //          c2 c2 c2                                  c1 c1 c1 
  //          c2 c2 c2                                  c1 c1 c1
  //          c2 c2 c2                                  c1 c1 c1
  //          c2 c2 c2                                  c1 c1 c1
  //          c2 c2 c2                                  c1 c1 c1
  //          c2 c2 c2                                  c1 c1 c1
  //          c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c1 c1
  //          c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c1
  //          c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2 c2


   tft->fillRect(x, y, width, height, color);


     // top
       for(int i = 0; i < thickness; i++){
          tft->drawFastHLine(x, y + i, width, GRAY_LIGHT);  //0, 0  0,1 0,2
       }

    // right
       for(int i = 0; i < thickness; i++){
          tft->drawFastVLine(x + width - i, y, height, GRAY_LIGHT);  //15,0 14,0 13,0
      }
   

     // left
       for(int i = 0; i < thickness; i++){
          tft->drawFastVLine(x + i, y + i, height - i, GRAY_DARKLIGHT ); // 0,0 1,1 2,2
       }

    // bottom
      for(int i = 0; i < thickness; i++){
        tft->drawFastHLine(x, y + height - i, width - i, GRAY_DARKLIGHT);  // 0,15  0,14 0,13       
      }
}
// +50 to -50
void gui::SetEngineSpeed(int speed){
    // tft->fillRect(140, 100, 40, 20, BLACK);
    //  tft->setTextColor(0xFFFF, BLACK);
    //         tft->setCursor(140, 100);
    //         tft->print(speed);

  if(speed == 0){
     tft->fillRect(_engineSlider_x, _engineSlider_y, _engineSlider_Width, _engineSlider_Height, WHITE);
     return;
  }
  int bX = _engineSlider_x;
  int bY = _engineSlider_y;
  int w = _engineSlider_Width;
  int hH = _engineSlider_HalfHeight;
  int pP = ((float)_engineSlider_Height / (float)100) * abs(speed);

  //
  //    bX, bY
  //      ______________
  //     |              |
  //     |              |
  //     |              |
  //     |              |
  //     |              |
  //     |              |
  //     |______________| -> hH (half height)
  //     |              |
  //     |              |
  //     |              |
  //     |              |
  //     |              |
  //     |              |
  //     |______________| 
  //     <------ w ------->
  //
  
  if(speed > 0){
     tft->fillRect(bX, bY, w, hH - pP, WHITE);
     tft->fillRect(bX, bY + hH - pP, w, pP, RED);
     tft->fillRect(bX, bY + hH, w, hH, WHITE);
     return;
  }

  tft->fillRect(bX, bY, w, hH, WHITE);
  tft->fillRect(bX, bY + hH, w, pP, RED);
  tft->fillRect(bX, bY + hH + pP, w, hH - pP, WHITE);
}

void gui::InitializeMonitorStateScreen(){
   tft->fillScreen(GRAY_MID);

  

    // top
    tft->drawFastHLine(0, 0, SCREEN_WIDTH, GRAY_DARK);
    tft->drawFastHLine(1, 1, SCREEN_WIDTH - 2, GRAY_DARK);
    tft->drawFastHLine(2, 2, SCREEN_WIDTH - 4, GRAY_DARK);

    // right
    tft->drawFastVLine(SCREEN_WIDTH, 0, SCREEN_HEIGHT, GRAY_DARK);
    tft->drawFastVLine(SCREEN_WIDTH - 1, 1, SCREEN_HEIGHT - 2, GRAY_DARK);
    tft->drawFastVLine(SCREEN_WIDTH - 2, 2, SCREEN_HEIGHT - 4, GRAY_DARK);

    // bottom
    tft->drawFastHLine(0, SCREEN_HEIGHT, SCREEN_WIDTH - 1, GRAY_LIGHT);
    tft->drawFastHLine(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH - 2, GRAY_LIGHT);
    tft->drawFastHLine(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH - 3, GRAY_LIGHT);

    // left
    tft->drawFastVLine(0, 0, SCREEN_HEIGHT, GRAY_LIGHT );
    tft->drawFastVLine(1, 1, SCREEN_HEIGHT - 2, GRAY_LIGHT );
    tft->drawFastVLine(2, 2, SCREEN_HEIGHT - 3, GRAY_LIGHT );

      // Overskrift
    tft->fillRect(90, 10, 10, 20, GREEN);
    tft->fillRect(100, 10, 10, 20, WHITE);
    tft->fillRect(110, 10, 10, 20, RED);
    this->printMsgXY(125, 27, 1, YELLOW, &FreeSans12pt7b, "Power Racing");
    tft->fillRect(280, 10, 10, 20, GREEN);
    tft->fillRect(290, 10, 10, 20, WHITE);
    tft->fillRect(300, 10, 10, 20, RED);

    //
    // Engine throttle panel
    //

    int16_t m_Panel_x = 75;
    int16_t m_Panel_y = 50;
    int16_t m_Panel_thickness = 3;
    int16_t m_Panel_width = 40;
    int16_t m_Panel_height = 140;

    //
    // Wheels position panel
    //

    int16_t w_Panel_x = 25;
    int16_t w_Panel_y = 190;
    int16_t w_Panel_thickness = 3;
    int16_t w_Panel_width = 140;
    int16_t w_Panel_height = 40;


    this->DrawInwardsPanel(m_Panel_x, m_Panel_y, m_Panel_width, m_Panel_height, m_Panel_thickness, WHITE);
    this->DrawInwardsPanel(w_Panel_x, w_Panel_y, w_Panel_width, w_Panel_height, w_Panel_thickness, WHITE);

    _engineSlider_x = m_Panel_x + 3;
    _engineSlider_y = m_Panel_y + 3;
    _engineSlider_Width = m_Panel_width - (m_Panel_thickness * 2) + 1;
    _engineSlider_Height = m_Panel_height - (m_Panel_thickness * 2);
    _engineSlider_HalfHeight = _engineSlider_Height / 2;

    tft->fillRect(_engineSlider_x, _engineSlider_y, _engineSlider_Width, _engineSlider_Height, WHITE);
    this->SetEngineSpeed(0);

}