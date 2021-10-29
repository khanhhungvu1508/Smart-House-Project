#include "dht_nonblocking.h"
#include "common.h"
#include <LiquidCrystal.h>

DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

void measure_environment( float *temperature, float *humidity )
{
  if( dht_sensor.measure( temperature, humidity ) != true ){}
}

void toggleLED(int ledPin){
  int currentState = digitalRead(ledPin);
  digitalWrite(ledPin, !currentState);
}

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);


void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_1_PIN, INPUT);
  Serial.begin(9600);
  init_button_reading();
  setTimer0(10);
  lcd.begin(16, 2);
}

void loop() {
  float temperature = 1;
  float humidity = 2;
  if(getTimerFlag0() == 1){
    measure_environment( &temperature, &humidity );
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("T = ");
    lcd.print(temperature);
    lcd.print(" deg.C");
    lcd.setCursor(0,1);
    lcd.print("H = ");
    lcd.print(humidity);
    lcd.print(" %");
    setTimer0(2000);
  }
  timer_run();
  button_reading();
  delay(10);
}
