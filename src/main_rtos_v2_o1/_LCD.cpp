#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "_LCD.h"
#include "Arduino.h"

LiquidCrystal_I2C lcd1(0x27, 16, 2);
LiquidCrystal_I2C lcd2(0x26, 16, 2);
LiquidCrystal_I2C lcd3(0x25, 16, 2);

// Use for display degree symbol
byte degree[8] =
{
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};

// Extern from FAN module
extern int mode;

// Extern from DHT11 module
extern float temperature;
extern float humidity;

// Extern from RFID module
extern int display_code;

// Initiaization of module
void init_LCD(){
  lcd1.begin();
  lcd2.begin();
  lcd3.begin();
  lcd1.backlight();
  lcd2.backlight();
  lcd3.backlight();
  lcd1.createChar(1, degree);
}

// LCD 1 display informations of module DHT11
void task_DHT11_display(){
  lcd1.clear();
  lcd1.setCursor(0,0);
  lcd1.print("T = ");
  lcd1.print(temperature);
  lcd1.write(1);
  lcd1.print("C");
  lcd1.setCursor(0,1);
  lcd1.print("H = ");
  lcd1.print(humidity);
  lcd1.print("%");
}

// LCD 2 display information if module FAN
void task_FAN_display(){
  lcd2.clear();
  lcd2.setCursor(0,0);
  switch(mode){
    case 0:
      lcd2.print("FAN: OFF");
      break;
    case 1:
      lcd2.print("FAN: MODE 1");
      break;
    case 2:
      lcd2.print("FAN: MODE 2");
      break;
    case 3:
      lcd2.print("FAN: MODE 3");
      break;
    case 4:
      lcd2.print("FAN: AUTO");
      break;
    default:
      break;
  }
}

// LCD 3 display information if module RFID
void task_RFID_display(){
  switch(display_code){
    case 0:
      lcd3.clear();
      lcd3.setCursor(0,0);
      lcd3.print("Locking");
      break;
    case 1:
      lcd3.clear();
      lcd3.setCursor(0,0);
      lcd3.print("Opening");
      break;
    case 2:
      lcd3.clear();
      lcd3.setCursor(0,0);
      lcd3.print("Add new card");
      break;
    case 4:
      lcd3.clear();
      lcd3.setCursor(0,0);
      lcd3.print("Successfully");
      break;
    case 5:
      lcd3.clear();
      lcd3.setCursor(0,0);
      lcd3.print("Card is exist");
      break;
    case 6:
      lcd3.clear();
      lcd3.setCursor(0,0);
      lcd3.print("Card is wrong");
      break;
    default:
      break;
  }
}
