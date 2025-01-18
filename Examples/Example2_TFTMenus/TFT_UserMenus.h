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
  void DisplayMenu()
  {
    DisplayMenu(curMenu);
    return;
  }
  //
  //
  //
  void DisplayMenu(int menuIdx)
  {
    // does menu require text redraw (new menu, changed display range)
    bool redrawText = false;
    // location of text
    textPosition[0] = 5;
    textPosition[1] = 0;
    unsigned long currentMillis = millis();

    int linesToDisplay = (tftDisplay->height() - 10)/fontHeight;
    if(menus[menuIdx].showDescription)
    {
      linesToDisplay--;
    }
    // range of selections to display (allow scrolling)
    if((menus[menuIdx].curChoice < priorDisplayRange[0]) || (menus[menuIdx].curChoice > priorDisplayRange[1]))
    {
      currentDisplayRange[0] = ((menus[menuIdx].curChoice - (linesToDisplay - 1)) <= 0) ?  0 : (menus[menuIdx].curChoice - (linesToDisplay - 1));
      currentDisplayRange[1] = (((currentDisplayRange[0] + linesToDisplay) - 1) <= menus[menuIdx].menuLength) ? (currentDisplayRange[0] + linesToDisplay) - 1 : menus[menuIdx].menuLength - 1;
      // erase screen, draw banner
      redrawText = true;
      tftDisplay->setRotation(3);
      tftDisplay->fillScreen(TFT_WHITE);
      Banner();
      SetFont(12);
    }

    curMenu = menuIdx;
    //if((currentDisplayRange[0] != priorDisplayRange[0]) ||
    //   (currentDisplayRange[1] != priorDisplayRange[1]) ||
    //   (curMenu != menuIdx))
    Serial.printf("Displayable Lines %d\n", linesToDisplay);
    Serial.printf("Current selection %d\n", menus[menuIdx].curChoice);
    Serial.printf("Prior display range %d to %d\n",priorDisplayRange[0], priorDisplayRange[1]);
    Serial.printf("Current display range %d to %d\n",currentDisplayRange[0], currentDisplayRange[1]);
//    if(((priorDisplayRange[0] != currentDisplayRange[0]) && (priorDisplayRange[1] != currentDisplayRange[1])) &&
//       (menus[menuIdx].curChoice < currentDisplayRange[0]) ||
//       (menus[menuIdx].curChoice > currentDisplayRange[1]))
//    {
//      // erase screen, draw banner
//      redrawText = true;
//      tftDisplay->setRotation(3);
//      tftDisplay->fillScreen(TFT_WHITE);
//      Banner();
//      SetFont(12);
//    }
//    curMenu = menuIdx;
    priorDisplayRange[0] = currentDisplayRange[0];
    priorDisplayRange[1] = currentDisplayRange[1];
    // display menu
    textPosition[0] = 0;
    textPosition[1] = 5;
    if(menus[menuIdx].showDescription)
    {
      tftDisplay->setTextColor(TFT_BLACK, TFT_WHITE);
      tftDisplay->drawString(menus[menuIdx].menuName, textPosition[0], textPosition[1], GFXFF);
      textPosition[1] += fontHeight;
    }
    for(int menuIdx = currentDisplayRange[0]; menuIdx <= currentDisplayRange[1]; menuIdx++)
    {
      if(menuIdx ==  menus[curMenu].curChoice)
      {
        textPosition[0] = 0;
        tftDisplay->setTextColor(TFT_WHITE, TFT_RED);
        tftDisplay->drawString(">", textPosition[0], textPosition[1], GFXFF);
      }
      else
      {
        textPosition[0] = 0;
        tftDisplay->fillRect(textPosition[0], textPosition[1], tftDisplay->textWidth(">"), fontHeight, TFT_WHITE);
      }
      if(redrawText)
      {
        textPosition[0] = fontWidth;
        sprintf(outStr, "%s",  menus[curMenu].choiceList[menuIdx].description);
        tftDisplay->setTextColor(TFT_BLACK, TFT_WHITE);
        //tftDisplay->fillRect(textPosition[0], textPosition[1], tftDisplay->width(), fontHeight, TFT_WHITE);
        tftDisplay->drawString(outStr, textPosition[0], textPosition[1], GFXFF);
      }
      textPosition[1] += fontHeight;
    }
    return;
  }
  void GetUserInput()
  {
    while(true)
    {
      currentMillis = millis();
      CheckButtons(currentMillis);
      // change selection down, break
      if(buttons[1].buttonReleased)
      {
        buttons[1].buttonReleased = false;
        menus[curMenu].curChoice = menus[curMenu].curChoice + 1 < menus[curMenu].menuLength ? menus[curMenu].curChoice + 1 : 0;
        break;
      }
      // change selection up, break
      else if(buttons[2].buttonReleased)
      {
        buttons[2].buttonReleased = false;
        menus[curMenu].curChoice = menus[curMenu].curChoice - 1 >= 0 ? menus[curMenu].curChoice - 1 : menus[curMenu].menuLength - 1;
        break;
      }
      // selected
      else if(buttons[0].buttonReleased)
      {
        menus[curMenu].choiceList[menus[curMenu].curChoice].action();
        priorDisplayRange[0] = -1;
        priorDisplayRange[1] = -1;
        break;
      }
    }
  }
  //
  // rotate TFT display for right or left hand use
  //
  void RotateDisplay(bool rotateButtons)
  {
    tftDisplay->setRotation(3);//deviceSettings.screenRotation == 0 ? 1 : 3);
    tftDisplay->fillScreen(TFT_WHITE);
    /*
    if(rotateButtons)
    {
      int reverseButtons[BUTTON_COUNT];
      for(int btnIdx = 0; btnIdx < BUTTON_COUNT; btnIdx++)
      {
        reverseButtons[BUTTON_COUNT - (btnIdx + 1)] = buttons[btnIdx].buttonPin;
      }
      for(int btnIdx = 0; btnIdx < BUTTON_COUNT; btnIdx++)
      {
        buttons[btnIdx].buttonPin = reverseButtons[btnIdx];
      }
    }
    */
  }
  //
  // draw the Yamura banner at bottom of TFT display
  //
  void Banner()
  {
    tftDisplay->setTextColor(TFT_BLACK, TFT_YELLOW);
    SetFont(9);
    int xPos = tftDisplay->width()/2;
    int yPos = tftDisplay->height() - fontHeight/2;
    tftDisplay->setTextDatum(BC_DATUM);
    tftDisplay->drawString("  Yamura Motors Menu Demo  ",xPos, yPos, GFXFF);
    tftDisplay->setTextDatum(TL_DATUM);
  }
  //
  // set font for TFT display, update fontHeight used for vertical stepdown by line
  //
  void SetFont(int fontSize)
  {
    switch(fontSize)
    {
      case 9:
        tftDisplay->setFreeFont(FSS9);
        break;
      case 12:
        tftDisplay->setFreeFont(FSS12);
        break;
      case 18:
        tftDisplay->setFreeFont(FSS18);
        break;
      case 24:
        tftDisplay->setFreeFont(FSS24); // was FSS18
        break;
      default:
        tftDisplay->setFreeFont(FSS12);
        break;
    }
    tftDisplay->setTextDatum(TL_DATUM);
    fontHeight = tftDisplay->fontHeight(GFXFF);
    fontWidth = tftDisplay->textWidth("X");
  }
  //
  //
  //
  void Setup(TFT_eSPI* display)
  {
    tftDisplay = display;
    //tftDisplay.init();
    RotateDisplay(true);  
    tftDisplay->invertDisplay(false);
    tftDisplay->setTextDatum(TL_DATUM);
    textPosition[0] = 5;
    textPosition[1] = 0;
    // 0 portrait pins down
    // 1 landscape pins right
    // 2 portrait pins up
    // 3 landscape pins left
    tftDisplay->fillScreen(TFT_BLACK);
    SetFont(12);
    tftDisplay->setTextColor(TFT_WHITE, TFT_BLACK);
    tftDisplay->drawString("TFT User menu demo", textPosition[0], textPosition[1], GFXFF);
    Banner();

    buttons[2].buttonPin = BUTTON_3;
    buttons[1].buttonPin = BUTTON_2; 
    buttons[0].buttonPin = BUTTON_1; 
    for(int idx = 0; idx < BUTTON_COUNT; idx++)
    {
      pinMode(buttons[idx].buttonPin, INPUT_PULLUP);
    }
  }
  //
  //
  //
  void DrawText(char* textToDraw, int x, int y, int textColor, int backColor)
  {
        tftDisplay->setTextColor(textColor, backColor);
        tftDisplay->drawString(textToDraw, x, y, GFXFF);
  }
  //
  // check all buttons for new press or release
  //
  void CheckButtons(unsigned long curTime)
  {
    byte curState;
    for(byte btnIdx = 0; btnIdx < BUTTON_COUNT; btnIdx++)
    {
      curState = digitalRead(buttons[btnIdx].buttonPin);
      if(((curTime - buttons[btnIdx].lastChange) > BUTTON_DEBOUNCE_DELAY) && 
         (curState != buttons[btnIdx].buttonLast))
      {
        buttons[btnIdx].lastChange = curTime;
        if(curState == BUTTON_PRESSED)
        {
          buttons[btnIdx].buttonPressed = 1;
          buttons[btnIdx].buttonReleased = 0;
          buttons[btnIdx].buttonLast = BUTTON_PRESSED;
          buttons[btnIdx].pressDuration = 0;
        }
        else if (curState == BUTTON_RELEASED)
        {
          buttons[btnIdx].buttonPressed =  0;
          buttons[btnIdx].buttonReleased = 1;
          buttons[btnIdx].buttonLast = BUTTON_RELEASED;
          buttons[btnIdx].releaseDuration = 0;
        }
      }
      else
      {
        if(curState == BUTTON_PRESSED)
        {
          buttons[btnIdx].pressDuration = curTime - buttons[btnIdx].lastChange;
        }
        else if (curState == BUTTON_RELEASED)
        {
          buttons[btnIdx].releaseDuration = curTime - buttons[btnIdx].lastChange;
        }
      }
    }
  }  
};

