#include <dht_nonblocking.h>
#include "main.h"
#include <LiquidCrystal.h>

DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

int timer_flag = 0, timer_counter = 0, TIMER_CYCLE = 10;

void setTimer0(int duration)
{
  timer_counter = duration / TIMER_CYCLE;
  timer_flag = 0;
}

void timer_run()
{
  timer_counter--;
  if (timer_counter == 0)
    timer_flag = 1;
}


float temperature = 1;
float humidity = 2;

static bool measure_environment( float *temperature, float *humidity )
{
  return dht_sensor.measure( temperature, humidity );
}

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_1_PIN, INPUT);
  Serial.begin(9600);
  init_button_reading();
  setTimer0(10);
  lcd.begin(16, 2);

  cli();//stop interrupts
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  /*
    We have initial frequency is 16 * 10^6 Hz
    We need time = 10ms --> Frequency = 100Hz
    We use prescaler = 256 and counter = 625.
    Since counter begin at 0-based index so OCR1A = 624
    Prove:
      We have Freq_out = Freq_init / (prescaler * (counter + 1)) = (16 * 10^6) / (256 * (624 + 1)) = 100Hz
  */
  OCR1A = 624;// = (16 * 10^6) / (100 * 256) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 bits for 256 prescaler
  TCCR1B |= 1 << CS12;  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();//allow interrupts
}

ISR(TIMER1_COMPA_vect){
  button_reading();
  timer_run();
  measure_environment(&temperature, &humidity);
}

void loop() {
  fsm_button_reading();
  if(timer_flag == 1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("T = ");
    lcd.print(temperature);
    lcd.setCursor(0,1);
    lcd.print("H = ");
    lcd.print(humidity);
    setTimer0(10000);
  }
  
 /* button_reading();
  timer_run();
  Serial.print("Time run = ");
  Serial.println(millis());
  delay(10);*/
}
