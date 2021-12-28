#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "_LCD.h"
#include "Arduino.h"

LiquidCrystal_I2C lcd1(0x27, 16, 2);
LiquidCrystal_I2C lcd2(0x26, 16, 2);
LiquidCrystal_I2C lcd3(0x25, 16, 2);

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

extern int mode;
extern int ACTemperature;
extern int autoTemperature;
extern float temperature;
extern float humidity;
extern int display_code;

void init_LCD(){
  lcd1.begin();
  lcd2.begin();
  lcd3.begin();
  lcd1.backlight();
  lcd2.backlight();
  lcd3.backlight();
  lcd1.createChar(1, degree);
}

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

void task_AC_display(){
  lcd2.clear();
  lcd2.setCursor(0,0);
  if(mode == 0){
    lcd2.print("AC: OFF");
  }
  else if(mode == 1){
    lcd2.print("AC: ");
    lcd2.print(ACTemperature);
  }
  else if(mode == 2){
    lcd2.print("AC: AUTO");
  }
  else if(mode == -1){
    lcd2.print("AUTO: ");
    lcd2.print(autoTemperature);
  }
}

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
    lcd3.print("Card is valid");
    case 6:
    lcd3.clear();
    lcd3.setCursor(0,0);
    lcd3.print("Card is wrong");
    default:
    break;
  }
}
