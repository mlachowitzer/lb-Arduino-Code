//////////////////////////////////////////////////////////////////////////////
//  OneWL - One-Wire Data Logger.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Includes follow
//////////////////////////////////////////////////////////////////////////////

#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <SD.h>
#include <LandBoards_OneWireLogger.h>
#include <Adafruit_GFX.h>      // Core graphics library
#include <Adafruit_ST7735.h>   // Hardware-specific library
//#include <EEPROMAnything.h>    // EEPROM anything
#include <eepromanything.h>

//////////////////////////////////////////////////////////////////////////////
// defines follow
//////////////////////////////////////////////////////////////////////////////

//#define SERIAL_OUT
#undef SERIAL_OUT

// Display specific colors

#define	TFT_RED      0x001F    // Colors are reversed on my display from Adafruit
#define	TFT_BLUE     0xF800
#define TFT_HEIGHT   16        // TFT is 16 lines tall

//////////////////////////////////////////////////////////////////////////////
// enums follow
//////////////////////////////////////////////////////////////////////////////

enum MENUITEMS
{
  STEEP_MENU,
  BOIL_MENU,
  INITOW_MENU,
};
//////////////////////////////////////////////////////////////////////////////
// Global variables follow
//////////////////////////////////////////////////////////////////////////////

uint8_t menuState;  // Used to implement the menuing state machine

uint8_t sensorNumber;
uint8_t sensorAddr;
uint8_t firstRun;
float temps1Wire[32];
float fahrenheit;

// Board Configuration stored in EEPROM
struct IZ_Cfgs
{
  uint8_t bll;       // Backlight level
  uint8_t enableSD;  // Enable the SD card
} 
IZConfigs;

// class initializers - most initialize hardware

OneWireLogger myOneWireLogger;
Adafruit_ST7735 tft = Adafruit_ST7735(LCD_CS, LCD_DC, LCD_RST);  // HW SPI
OneWire  ds(ONE_WIRE);  // on pin 

//////////////////////////////////////////////////////////////////////////////
// the setup routine runs once when you press reset:
//////////////////////////////////////////////////////////////////////////////

void setup() 
{
  // EEPROM access
  EEPROM_readAnything(0, IZConfigs);

  // TFT init
  analogWrite(BACKLIGHT, IZConfigs.bll);
  tft.initR(INITR_BLACKTAB);    // I actually have a black tab on my part
  clearDisplay();
  // RTC init starts up the I2C wire interface
  Wire.begin();
  sensorNumber = 0;

  // Set up the init menu state
  menuState = STEEP_MENU;
}

//////////////////////////////////////////////////////////////////////////////////////
// loop - runs over and over again forever:
//////////////////////////////////////////////////////////////////////////////////////

void loop() 
{
  menuRefresh();
  menuNav();
}

