#include "..\..\UserMenus.h"
#include "SPI.h"
#include <TFT_eSPI.h>            // https://github.com/Bodmer/TFT_eSPI Graphics and font library for ST7735 driver chip
#include "Free_Fonts.h"          // Include the header file attached to this sketch


class TFTMenus : public UserMenus
{
public:
  TFT_eSPI* tftDisplay;// = TFT_eSPI();
  int fontHeight;
  int fontWidth;
  int textPosition[2] = {5, 0};
  int screenRotation = 1;
  int currentDisplayRange[2] = {0, 0 };
  int priorDisplayRange[2] = {-1, -1 };
  char* menuTitle = "";
  //
  //
  //
  void DisplayMenu(char* title)
  {
    menuTitle = title;
    DisplayMenu(curMenu, title);
    return;
  }
  //
  //
  //
  void DisplayMenu(int menuIdx, char* title)
  {
    // does menu require text redraw (new menu, changed display range)
    bool redrawText = false;
    if((strlen(menuTitle) == 0) && (strlen(title) > 0))
    {
      menuTitle = title;
    }
    // location of text
    textPosition[0] = 5;
    textPosition[1] = 0;
    unsigned long currentMillis = millis();

    int linesToDisplay = (tftDisplay.height() - 10)/fontHeight;
    if(strlen(menuTitle) > 0)
    {
      linesToDisplay--;
    }
    // range of selections to display (allow scrolling)
    currentDisplayRange[0] = ((menus[menuIdx].curChoice - (linesToDisplay - 1)) <= 0) ?  0 : (menus[menuIdx].curChoice - (linesToDisplay - 1));
    currentDisplayRange[1] = ((currentDisplayRange[0] + linesToDisplay) < menus[menuIdx].menuLength) ? (currentDisplayRange[0] + linesToDisplay) : menus[menuIdx].menuLength;

    if((currentDisplayRange[0] != priorDisplayRange[0]) ||
       (currentDisplayRange[1] != priorDisplayRange[1]) ||
       (curMenu != menuIdx))
    {
      // erase screen, draw banner
      redrawText = true;
      tftDisplay.setRotation(3);
      tftDisplay.fillScreen(TFT_WHITE);
      Banner();
      SetFont(12);
    }
    curMenu = menuIdx;
    priorDisplayRange[0] = currentDisplayRange[0];
    priorDisplayRange[1] = currentDisplayRange[1];
    // display menu
    textPosition[0] = 0;
    textPosition[1] = 5;
    if(strlen(menuTitle) > 0)
    {
        tftDisplay.setTextColor(TFT_BLACK, TFT_WHITE);
        tftDisplay.drawString(menuTitle, textPosition[0], textPosition[1], GFXFF);
        textPosition[1] += fontHeight;
    }
    for(int menuIdx = currentDisplayRange[0]; menuIdx < currentDisplayRange[1]; menuIdx++)
    {
      if(menuIdx ==  menus[curMenu].curChoice)
      {
        textPosition[0] = 0;
        tftDisplay.setTextColor(TFT_WHITE, TFT_RED);
        tftDisplay.drawString(">", textPosition[0], textPosition[1], GFXFF);
      }
      else
      {
        textPosition[0] = 0;
        tftDisplay.fillRect(textPosition[0], textPosition[1], tftDisplay.textWidth(">"), fontHeight, TFT_WHITE);
      }
      if(redrawText)
      {
        textPosition[0] = fontWidth;
        sprintf(outStr, "%s",  menus[curMenu].choiceList[menuIdx].description);
        tftDisplay.setTextColor(TFT_BLACK, TFT_WHITE);
        //tftDisplay.fillRect(textPosition[0], textPosition[1], tftDisplay.width(), fontHeight, TFT_WHITE);
        tftDisplay.drawString(outStr, textPosition[0], textPosition[1], GFXFF);
      }
      textPosition[1] += fontHeight;
    }
    return;
  }
  //void GetUserInput()
  //{
  //}
  //
  // rotate TFT display for right or left hand use
  //
  void RotateDisplay(bool rotateButtons)
  {
    tftDisplay.setRotation(3);//deviceSettings.screenRotation == 0 ? 1 : 3);
    tftDisplay.fillScreen(TFT_WHITE);
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
    tftDisplay.setTextColor(TFT_BLACK, TFT_YELLOW);
    SetFont(9);
    int xPos = tftDisplay.width()/2;
    int yPos = tftDisplay.height() - fontHeight/2;
    tftDisplay.setTextDatum(BC_DATUM);
    tftDisplay.drawString("  Yamura Motors Menu Demo  ",xPos, yPos, GFXFF);
    tftDisplay.setTextDatum(TL_DATUM);
  }
  //
  // set font for TFT display, update fontHeight used for vertical stepdown by line
  //
  void SetFont(int fontSize)
  {
    switch(fontSize)
    {
      case 9:
        tftDisplay.setFreeFont(FSS9);
        break;
      case 12:
        tftDisplay.setFreeFont(FSS12);
        break;
      case 18:
        tftDisplay.setFreeFont(FSS18);
        break;
      case 24:
        tftDisplay.setFreeFont(FSS24); // was FSS18
        break;
      default:
        tftDisplay.setFreeFont(FSS12);
        break;
    }
    tftDisplay.setTextDatum(TL_DATUM);
    fontHeight = tftDisplay.fontHeight(GFXFF);
    fontWidth = tftDisplay.textWidth("X");
  }
  //
  //
  //
  void Setup(TFT_eSPI* display)
  {
    tftDisplay = display;
    //tftDisplay.init();
    RotateDisplay(true);  
    tftDisplay.invertDisplay(false);
    tftDisplay.setTextDatum(TL_DATUM);
    textPosition[0] = 5;
    textPosition[1] = 0;
    // 0 portrait pins down
    // 1 landscape pins right
    // 2 portrait pins up
    // 3 landscape pins left
    tftDisplay.fillScreen(TFT_BLACK);
    SetFont(12);
    tftDisplay.setTextColor(TFT_WHITE, TFT_BLACK);
    tftDisplay.drawString("TFT User menu demo", textPosition[0], textPosition[1], GFXFF);

    Banner();
  }
  //
  //
  //
  void DrawText(char* textToDraw, int x, int y, int textColor, int backColor)
  {
        tftDisplay.setTextColor(textColor, backColor);
        tftDisplay.drawString(textToDraw, x, y, GFXFF);
  }
};

