#include <cstdlib>
#include <Arduino.h>
#include <HardwareSerial.h>

//
//
//
class Menu
{
public:
  // function to handle menu selection
  typedef void (*ChoiceHandler) ();

  // an individual choice in menu
  // text description
  // action handler function pointer
  struct MenuChoice
  {
    char description[32];  // description of selection
    ChoiceHandler action; // action handler
  };
  // info about this menu  
  char menuName[32];           // title
  int menuLength = 0;          // number of choices
  int curChoice = 0;           // current choice

  // list of menu choices for this menu
  MenuChoice* choiceList;
  //
  // add a choice to menu
  // passing in an existing index updates the menu choice
  //
  void AddMenuChoice(int choiceIdx, String desc, ChoiceHandler handler)
  {
    if(menuLength == 0)
    {
      choiceList = (MenuChoice*)malloc(sizeof(MenuChoice));
      menuLength++;
    }
	else
	{
	  while(choiceIdx >= menuLength)
      {
        choiceList = (MenuChoice*)realloc(choiceList, ((menuLength + 1) * sizeof(MenuChoice)));
		menuLength++;
      }
	}
    // add/update description and handler
    strcpy(choiceList[choiceIdx].description, desc.c_str());
    choiceList[choiceIdx].action = handler;
    return;
  }
  //
  // remove a choice in a menu
  // passing in an existing index to remove
  //
  void RemoveMenuChoice(int choiceIdx)
  {
    if((choiceIdx < 0) || (choiceIdx >= menuLength))
	{
		return;
	}
    while(choiceIdx < (menuLength - 1))
    {
        strcpy(choiceList[choiceIdx].description, choiceList[choiceIdx + 1].description);
        choiceList[choiceIdx].action = choiceList[choiceIdx + 1].action;
    }
    menuLength--;
    choiceList = (MenuChoice*)realloc(choiceList, ((menuLength + 1) * sizeof(MenuChoice)));
    return;
  }
  //
  // constructor
  //
  Menu()
  {
    strcpy(menuName, "Main Menu");
  }
};
class UserMenus
{
public:
  Menu* menus;
  int menuCount = 0;
  int curMenu = 0;
  char outStr[128];
  //
  // display current menu
  //
//  virtual void DisplayMenu(char* title = "");
  virtual void DisplayMenu(char* title)
  {
	DisplayMenu(curMenu, title);
    return;
  }
  //
  // display selected menu
  //
//  virtual void DisplayMenu(menuIdx = 0, char* title = "");
  virtual void DisplayMenu(int menuIdx, char* title)
  {
    curMenu = menuIdx;
	if(strlen(title) > 0)
	{
      Serial.println(title);
	}
    for(int idx = 0; idx < menus[menuIdx].menuLength; idx++)
    {
      Serial.print(menus[menuIdx].curChoice == idx ? ">" : " ");
      Serial.println(menus[menuIdx].choiceList[idx].description);
    }
    return;
  }
  //
  //
  //
  virtual void GetUserInput()
  {
    String readChar;
    int readVal;
    Serial.flush();
    while(true)
	{
      while(!Serial.available())
      { }
      readChar = Serial.readStringUntil('\n');
      readVal = atoi(readChar.c_str());
      while(Serial.available())
      { 
        Serial.flush();
      }
      if((readChar == "D") || (readChar == "d"))
      {
        menus[curMenu].curChoice = menus[curMenu].curChoice + 1 < menus[curMenu].menuLength ? menus[curMenu].curChoice + 1 : 0;
		DisplayMenu("");
      }
      else if((readChar == "U") || (readChar == "u"))
      {
        menus[curMenu].curChoice = menus[curMenu].curChoice - 1 >= 0 ? menus[curMenu].curChoice - 1 : menus[curMenu].menuLength - 1;
		DisplayMenu("");
      }
      else// if((readChar == "S") || (readChar == "s"))
      { 
        menus[curMenu].choiceList[menus[curMenu].curChoice].action();
	    break;
      }
      Serial.flush();
	}
    return;
  }
  //
  //
  //
  virtual void Setup()
  {
	  // no setup required for Serial version
  }
  //
  //
  //
  virtual void Banner()
  {
	  // no banner required for Serial version
  }

  //
  //
  //
  void AddMenu(char* menuTitle)
  {
    if(menuCount == 0)
    {
      menus = (Menu*)malloc(sizeof(Menu));
    }
    else
    {
      menus = (Menu*)realloc(menus, ((menuCount + 1) * sizeof(Menu)));
    }
    strcpy(menus[menuCount].menuName, menuTitle);
    menus[menuCount].menuLength = 0;
    menus[menuCount].curChoice = 0;
    menuCount++;
    return;
  }
  //
  //
  //
  int GetCurrentSelection()
  {
    return menus[curMenu].curChoice;
  }
  //
  //
  //
  void SetCurrentSelection(int currentSelection)
  {
    menus[curMenu].curChoice = currentSelection;
    return;
  }
  //
  //
  //
  int GetCurrentMenu()
  {
    return curMenu;
  }
  //
  //
  //
  void SetCurrentMenu(int currentMenu)
  {
    menus[currentMenu].curChoice = 0;
    curMenu = currentMenu;
    return;
  }
  //
  //
  //
  void ListChoices(int forMenu)
  {
    Serial.print("Menu ");
    Serial.println(menus[forMenu].menuName);
    for(int choiceIdx = 0; choiceIdx < menus[forMenu].menuLength; choiceIdx++)
    {
      Serial.print("Choice ");
      Serial.print(choiceIdx);
      Serial.print(" text ");
      Serial.println(menus[forMenu].choiceList[choiceIdx].description);
    }
    return;
  }
};