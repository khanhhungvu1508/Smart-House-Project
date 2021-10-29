
#define RELEASED HIGH
#define PRESSED LOW
#define NUMBER_OF_BUTTONS 1

#define DURATION_FOR_AUTO_OPERATION_1 2000
#define DURATION_FOR_INTERVAL_OPERATION_1 2000

enum Flag{
  BEING_RELEASED,
  BE_PRESSED,
  BEING_PRESSED,
  BEING_PRESSED_MORE_1,
  BEING_PRESSED_MORE_1_INTERVAL
};

#define TIMER_CYCLE 10

int buttonPin[NUMBER_OF_BUTTONS] = {6};

int buttonDebounce1[NUMBER_OF_BUTTONS];
int buttonDebounce2[NUMBER_OF_BUTTONS];
int buttonDebounce3[NUMBER_OF_BUTTONS];
int buttonCounter[NUMBER_OF_BUTTONS];
int buttonFlag[NUMBER_OF_BUTTONS];

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
          if(buttonCounter[i] + 2 >= (DURATION_FOR_AUTO_OPERATION_1 / 10)){
            buttonFlag[i] = BEING_PRESSED_MORE_1;
            if((((buttonCounter[i] + 2) * TIMER_CYCLE - DURATION_FOR_AUTO_OPERATION_1) % DURATION_FOR_INTERVAL_OPERATION_1) == 0){
              buttonFlag[i] = BEING_PRESSED_MORE_1_INTERVAL;
              buttonCounter[i] = DURATION_FOR_AUTO_OPERATION_1-  2;
            }
          } 
        }
        // For button ...
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
      case BE_PRESSED:
        toggleLED(13);
        break;
      case BEING_PRESSED_MORE_1:
        break;
      case BEING_PRESSED_MORE_1_INTERVAL:
        toggleLED(13);
        break;
      default:
        break;
    }
  }
}
