#define TIMER_CYCLE 10

#define LED_PIN 13
#define BUTTON_1_PIN 8
#define BUTTON_2_PIN 9
#define BUTTON_3_PIN 10
#define BUTTON_4_PIN 11

float temperature = 0;
float humidity = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  
  Serial.begin(9600);
  
  init_button_reading();
  init_LCD();
  init_timer_interrupt();
  init_timer();
  init_DHT11_sensor(&temperature, &humidity);
  init_AC();
}

void loop() {
  fsm_button_reading();
  AC_run();
  if(getTimerFlag(1) == 1){
    measure_environment(&temperature, &humidity);
    setTimer(1, 1000); 
  }
  
  if(getTimerFlag(0) == 1){
    DHT11_LCD_display(temperature, humidity);
    setTimer(0, 500);
  }
  /*
  button_reading();
  timer_run();
  delay(10);
  */
}
///*
ISR(TIMER1_COMPA_vect){
  button_reading();
  timer_run();
}
//*/
