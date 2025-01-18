#include "..\..\UserMenus.h"
#define MAIN_MENU 0
#define SETTINGS_MENU 1
#define DRIVERS_MENU 2
#define RESULTS_MENU 3
#define YESNO_MENU 4
#define TIMEMODE_MENU 5
#define UNITS_MENU 6

UserMenus menuSystem;

void setup() 
{
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  for(int idx = 0; idx < 10; idx++)
  {
    digitalWrite(13, !digitalRead(16));
    delay(100);
  }
  digitalWrite(13, LOW);
  delay(1000);
  Serial.println("Menu system example");
  Serial.println("===================");
  Serial.println("Add menus");
  menuSystem.AddMenu("Main Menu", false);
  menuSystem.AddMenu("Settings", false);
  menuSystem.AddMenu("Drivers", false);
  menuSystem.AddMenu("Results", false);
  menuSystem.AddMenu("Yes/No", true);
  menuSystem.AddMenu("12/24", false);
  menuSystem.AddMenu("Units", false);
  // main menu
  Serial.println("Add choices to menu 0");
  menuSystem.menus[MAIN_MENU].AddMenuChoice(0, "Measure Temps", MeasureTemps);
  menuSystem.menus[MAIN_MENU].AddMenuChoice(1, "Brian BMW Z4", DriverSelection);
  menuSystem.menus[MAIN_MENU].AddMenuChoice(2, "Display Temps", DisplayTemps);
  menuSystem.menus[MAIN_MENU].AddMenuChoice(3, "Instant Temp", InstantTemp);
  menuSystem.menus[MAIN_MENU].AddMenuChoice(4, "Display Selected Results", DisplaySelectedResults);
  menuSystem.menus[MAIN_MENU].AddMenuChoice(5, "Settings", Settings);
  // settings submenu
  Serial.println("Add choices to menu 1");
  menuSystem.menus[SETTINGS_MENU].AddMenuChoice(0,  "Set Date/Time", SetDateTime);
  menuSystem.menus[SETTINGS_MENU].AddMenuChoice(1,  "Set Units", SetUnits);
  menuSystem.menus[SETTINGS_MENU].AddMenuChoice(2,  "Invert Screen", InvertScreen);
  menuSystem.menus[SETTINGS_MENU].AddMenuChoice(3,  "Font Size", SetFontSize);
  menuSystem.menus[SETTINGS_MENU].AddMenuChoice(4,  "12 or 24 Hour display", Display12_24);
  menuSystem.menus[SETTINGS_MENU].AddMenuChoice(5,  "Set Temp Bandwidth", SetTempBandwidth);
  menuSystem.menus[SETTINGS_MENU].AddMenuChoice(6,  "Set Temp Delay", SetTempDelay);
  menuSystem.menus[SETTINGS_MENU].AddMenuChoice(7,  "Set Temp Buffer", SetTempBuffer);
  menuSystem.menus[SETTINGS_MENU].AddMenuChoice(8,  "Delete Data", DeleteData);
  menuSystem.menus[SETTINGS_MENU].AddMenuChoice(9,  "IP 192.168.4.1", NoAction);
  menuSystem.menus[SETTINGS_MENU].AddMenuChoice(10, "Password ZoeyDora48375", NoAction);
  menuSystem.menus[SETTINGS_MENU].AddMenuChoice(11, "Save Settings", SaveSettings);
  menuSystem.menus[SETTINGS_MENU].AddMenuChoice(12, "Exit", ExitSettings);
  // driver/car submenu
  Serial.println("Add choices to menu 2");
  menuSystem.menus[DRIVERS_MENU].AddMenuChoice(0, "Brian Z4", SetDriver);
  menuSystem.menus[DRIVERS_MENU].AddMenuChoice(1, "Mark MR2", SetDriver);
  menuSystem.menus[DRIVERS_MENU].AddMenuChoice(2, "Rob Miata", SetDriver);
  // display selected results
  Serial.println("Add choices to menu 3");
  menuSystem.menus[RESULTS_MENU].AddMenuChoice(0, "Brian Z4 1", SelectResults);
  menuSystem.menus[RESULTS_MENU].AddMenuChoice(1, "Brian Z4 2", SelectResults);
  menuSystem.menus[RESULTS_MENU].AddMenuChoice(2, "Brian Z4 3", SelectResults);
  menuSystem.menus[RESULTS_MENU].AddMenuChoice(3, "Brian Z4 4", SelectResults);
  menuSystem.menus[RESULTS_MENU].AddMenuChoice(4, "Brian Z4 5", SelectResults);
  menuSystem.menus[RESULTS_MENU].AddMenuChoice(5, "Brian Z4 6", SelectResults);
  menuSystem.menus[RESULTS_MENU].AddMenuChoice(6, "Brian Z4 7", SelectResults);
  menuSystem.menus[RESULTS_MENU].AddMenuChoice(7, "Brian Z4 7", SelectResults);
  //
  Serial.println("Add choices to menu 4");
  menuSystem.menus[YESNO_MENU].AddMenuChoice(0, "Yes", YesNo);
  menuSystem.menus[YESNO_MENU].AddMenuChoice(1, "No", YesNo);
  //
  Serial.println("Add choices to menu 5");
  menuSystem.menus[TIMEMODE_MENU].AddMenuChoice(0, "12 Hour Display", Display12_24);
  menuSystem.menus[TIMEMODE_MENU].AddMenuChoice(1, "24 Hour Display", Display12_24);
  //
  Serial.println("Add choices to menu 6");
  menuSystem.menus[UNITS_MENU].AddMenuChoice(0, "Degrees F", SelectUnits);
  menuSystem.menus[UNITS_MENU].AddMenuChoice(1, "Degrees C", SelectUnits);

  // set the main menu as current
  menuSystem.SetCurrentMenu(MAIN_MENU);
}

void loop() 
{
  Serial.println();
  menuSystem.DisplayMenu();
  menuSystem.GetUserInput();
  delay(100);
}
//
// menu choice handlers
//
void MeasureTemps()
{
  Serial.println("MeasureTemps");
  return;
}
void DriverSelection()
{
  Serial.println("DriverSelection");
  menuSystem.SetCurrentMenu(DRIVERS_MENU);
  return;
}
void DisplayTemps()
{
  Serial.println("DisplayTemps");
  return;
}
void InstantTemp()
{
  Serial.println("InstantTemp");
  return;
}
void DisplaySelectedResults()
{
  menuSystem.SetCurrentMenu(RESULTS_MENU);
  return;
}
void Settings()
{
  Serial.println("Settings");
  menuSystem.SetCurrentMenu(SETTINGS_MENU);
  return;
}
void SetDateTime()
{
  Serial.println("Set Date/Time");
  return;
}
void SetUnits()
{
  Serial.println("SetUnits");
  return;
}
void InvertScreen()
{
  Serial.println("InvertScreen");
  return;
}
void SetFontSize()
{
  Serial.println("SetFontSize");
  return;
}
void Display12_24()
{
  Serial.println("Display12_24");
  return;
}
void SetTempBandwidth()
{
  Serial.println("SetTempBandwidth");
  return;
}
void SetTempDelay()
{
  Serial.println("SetTempDelay");
  return;
}
void SetTempBuffer()
{
  Serial.println("SetTempBuffer");
  return;
}
void DeleteData()
{
  while(true)
  {
    menuSystem.DisplayMenu(YESNO_MENU);
    menuSystem.GetUserInput();
    if(menuSystem.GetCurrentSelection() == 0)
    {
      Serial.println("Delete Data");
      break;
    }
    else if(menuSystem.GetCurrentSelection() == 1)
    {
      Serial.println("Cancel Delete Data");
      break;
    }
  }
  menuSystem.SetCurrentMenu(MAIN_MENU);
  return;
}
void SaveSettings()
{
  Serial.println("SaveSettings");
  menuSystem.SetCurrentMenu(MAIN_MENU);

  menuSystem.menus[DRIVERS_MENU].AddMenuChoice(menuSystem.menus[DRIVERS_MENU].menuLength, "Joe Bug Scorpion", SetDriver);

  return;
}
void ExitSettings()
{
  Serial.println("ExitSettings");
  menuSystem.SetCurrentMenu(MAIN_MENU);
  return;
}
void SetDriver()
{
  Serial.print("SetDriver - selected ");

  menuSystem.menus[MAIN_MENU].AddMenuChoice(1,
                                            menuSystem.menus[DRIVERS_MENU].choiceList[menuSystem.GetCurrentSelection()].description, 
                                            DriverSelection);

  Serial.println(menuSystem.GetCurrentSelection());
  menuSystem.SetCurrentMenu(MAIN_MENU);
  return;
}
void SelectResults()
{
  menuSystem.curMenu = MAIN_MENU;
  return;
}
//
void YesNo()
{
  return;
}
void SelectUnits()
{
  Serial.println("SelectUnits");
  return;
}
void NoAction()
{
  return;
}