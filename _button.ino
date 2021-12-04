#define PRESSED LOW
#define RELEASED HIGH

#define NUMBER_OF_BUTTONS 4

#define DURATION_FOR_AUTO_OPERATION_1 2000
#define DURATION_FOR_AUTO_OPERATION_2 1000
#define DURATION_FOR_INTERVAL_OPERATION_1 2000

enum STATE{
  BEING_RELEASED,
  BE_PRESSED,
  BEING_PRESSED,
  BEING_PRESSED_MORE_1,
  BE_PRESSED_MORE_1_INTERVAL,
  BE_PRESSED_MORE_2,
  BEING_PRESSED_MORE_2
};

int buttonPin[NUMBER_OF_BUTTONS] = {BUTTON_1_PIN, BUTTON_2_PIN, BUTTON_3_PIN, BUTTON_4_PIN};

int buttonDebounce1[NUMBER_OF_BUTTONS];
int buttonDebounce2[NUMBER_OF_BUTTONS];
int buttonDebounce3[NUMBER_OF_BUTTONS];
int buttonCounter[NUMBER_OF_BUTTONS];
STATE buttonFlag[NUMBER_OF_BUTTONS];

void init_button_reading(){
  for(int i = 0; i < NUMBER_OF_BUTTONS; i++){
    buttonDebounce1[i] = RELEASED;
    buttonDebounce2[i] = RELEASED;
    buttonDebounce3[i] = RELEASED;
    buttonCounter[i] = 0;
    buttonFlag[i] = BEING_RELEASED;
  }
}

void button_reading(){
  for(int i = 0; i < NUMBER_OF_BUTTONS; i++){
    buttonDebounce1[i] = buttonDebounce2[i];
    buttonDebounce2[i] = buttonDebounce3[i];
    buttonDebounce3[i] = digitalRead(buttonPin[i]);
    if(buttonDebounce1[i] == buttonDebounce2[i] && buttonDebounce2[i] == buttonDebounce3[i]){
      if(buttonDebounce1[i] == PRESSED){
        buttonCounter[i]++;
        if(buttonCounter[i] == 1) buttonFlag[i] = BE_PRESSED;
        else buttonFlag[i] = BEING_PRESSED;
        // For button 1
        if(i == 0){
          if((buttonCounter[i] + 1) >= (DURATION_FOR_AUTO_OPERATION_1 / TIMER_CYCLE)){
            buttonFlag[i] = BEING_PRESSED_MORE_1;
            if((((buttonCounter[i] + 1) * TIMER_CYCLE - DURATION_FOR_AUTO_OPERATION_1) % DURATION_FOR_INTERVAL_OPERATION_1) == 0){
              buttonFlag[i] = BE_PRESSED_MORE_1_INTERVAL;
              buttonCounter[i] = (DURATION_FOR_AUTO_OPERATION_1 / TIMER_CYCLE) - 1;
            }
          } 
        }
        // For button 2
        else if(i == 1){
          if((buttonCounter[i] + 1) >= (DURATION_FOR_AUTO_OPERATION_2 / TIMER_CYCLE)){
            if(buttonFlag[i] == BEING_PRESSED){
              buttonFlag[i] = BE_PRESSED_MORE_2;
            }
            else{
              buttonFlag[i] = BEING_PRESSED_MORE_2;
            }
            buttonCounter[i] = DURATION_FOR_AUTO_OPERATION_2 / TIMER_CYCLE + 10;
          }
        }
        // For button 3, 4
        else if(i == 2 || i == 3){
          buttonCounter[i] = 2;
        }
      }
      else{
        buttonCounter[i] = 0;
        buttonFlag[i] = BEING_RELEASED;
      }
    }
  }
}

void fsm_button_reading(){
  for(int i = 0; i < NUMBER_OF_BUTTONS; i++){
    switch(buttonFlag[i]){
      case BEING_RELEASED:
        break;
      case BEING_PRESSED:
        break;
      case BE_PRESSED:
        if(i == 0) digitalWrite(13, !digitalRead(LED_PIN));
        if(i == 1) increaseMode();
        if(i == 2 && mode == 1) increaseACTemperature();
        if(i == 3 && mode == 1) decreaseACTemperature();
        if(i == 2 && mode == -1) increaseACAuto();
        if(i == 3 && mode == -1) decreaseACAuto();
        buttonFlag[i] = BEING_PRESSED;
        break;
      case BEING_PRESSED_MORE_1:
        break;
      case BE_PRESSED_MORE_1_INTERVAL:
        digitalWrite(13, !digitalRead(LED_PIN));
        buttonFlag[i] = BEING_PRESSED_MORE_1;
        break;
      case BEING_PRESSED_MORE_2:
        break;
      case BE_PRESSED_MORE_2:
        changeAutoMode();
        buttonFlag[i] = BEING_PRESSED_MORE_2;
        break;
      default:
        break;
    }
  }
}
