#include <LiquidCrystal_I2C.h>
#include <Wire.h>


LiquidCrystal_I2C lcd1(0x27, 16, 2);
LiquidCrystal_I2C lcd2(0x26, 16, 2);

void init_LCD(){
  lcd1.begin();
  lcd2.begin();
  lcd1.backlight();
  lcd2.backlight();
}

void DHT11_LCD_display(float temperature, float humidity){
  lcd1.clear();
  lcd1.setCursor(0,0);
  lcd1.print("T = ");
  lcd1.print(temperature);
  lcd1.setCursor(0,1);
  lcd1.print("H = ");
  lcd1.print(humidity);
}

void AC_display(int mode, int ACTemperature, int autoTemperature){
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
