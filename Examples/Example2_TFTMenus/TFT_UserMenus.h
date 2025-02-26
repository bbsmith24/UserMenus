#ifndef TFT_UserMenus_h
#define TFT_UserMenus_h
#include "..\..\UserMenus.h"
#include "SPI.h"
#include <TFT_eSPI.h>            // https://github.com/Bodmer/TFT_eSPI Graphics and font library for ST7735 driver chip
#include "Free_Fonts.h"          // Include the header file attached to this sketch

class TFTMenus : public UserMenus
{
  // user inputs
  #define BUTTON_COUNT 3
  #define BUTTON_1     13  // select
  #define BUTTON_2     12  // down
  #define BUTTON_3     27  // up
  #define BUTTON_RELEASED 0
  #define BUTTON_PRESSED  1
  #define BUTTON_DEBOUNCE_DELAY   20   // ms

public:
  // button debounce structure
  struct UserButton
  {
    int buttonPin = 0;
    bool buttonReleased = false;
    bool buttonPressed =  false;
    byte buttonLast =     BUTTON_RELEASED;
    unsigned long pressDuration = 0;
    unsigned long releaseDuration = 0;
    unsigned long lastChange = 0;
  };
  // button structure list
  UserButton buttons[BUTTON_COUNT];
  
  TFT_eSPI* tftDisplay;
  int fontHeight;
  int fontWidth;
  int textPosition[2] = {5, 0};
  int screenRotation = 1;
  int currentDisplayRange[2] = {0, 0 };
  int priorDisplayRange[2] = {-1, -1 };
  char* menuTitle = "";
  unsigned long currentMillis = 0;
  //
  //
  //
  void DisplayMenu();
  //
  //
  //
  void DisplayMenu(int menuIdx);
  //
  void GetUserInput();
  //
  // rotate TFT display for right or left hand use
  //
  void RotateDisplay(bool rotateButtons);
  //
  // draw the Yamura banner at bottom of TFT display
  //
  void Banner();
  //
  // set font for TFT display, update fontHeight used for vertical stepdown by line
  //
  void SetFont(int fontSize);
  //
  //
  //
  void Setup(TFT_eSPI* display);
  //
  //
  //
  void DrawText(char* textToDraw, int x, int y, int textColor, int backColor);
  //
  // check all buttons for new press or release
  //
  void CheckButtons(unsigned long curTime);
};
#endif