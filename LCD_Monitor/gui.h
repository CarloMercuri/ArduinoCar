#include <MCUFRIEND_kbv.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>

#include <FreeDefaultFonts.h>

#define BLACK   0x0000
#define GRAY    0xa554
#define GRAY_DARK    0x630c
#define GRAY_DARKLIGHT    0x8430
#define GRAY_LIGHT    0xc658
#define GRAY_MID    0xa534
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

class gui {
  public:

    void InitializeGUI(MCUFRIEND_kbv *tft);
    void SetEngineSpeed(int speed);
    void DrawInwardsPanel(int16_t x, int16_t y, int16_t width, int16_t height, int16_t thickness, int16_t color);
    void UpdateDeviceConnectedStatus(bool deviceConnected);

    int GuiState();

  private:
    MCUFRIEND_kbv *tft;
    void printMsgXY(int16_t x, int16_t y, int16_t size, int16_t color, const GFXfont *f, const char *msg);
    void InitializeMonitorStateScreen();
    void InitializeFirstPage();
    void TransitionToState(int16_t);
    int _guiState;
};

