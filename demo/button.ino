#define PRESSED LOW
#define RELEASED HIGH

#define NUMBER_OF_BUTTONS 1

int buttonPin[NUMBER_OF_BUTTONS] = {6};

int buttonState[NUMBER_OF_BUTTONS];
int buttonDebounce1[NUMBER_OF_BUTTONS];
int buttonDebounce2[NUMBER_OF_BUTTONS];
int buttonDebounce3[NUMBER_OF_BUTTONS];
int buttonCounter[NUMBER_OF_BUTTONS];
int buttonFlag[NUMBER_OF_BUTTONS];

void init_button_reading(){
  for(int i = 0; i < NUMBER_OF_BUTTONS; i++){
    buttonState[i] = RELEASED;
    buttonDebounce1[i] = RELEASED;
    buttonDebounce2[i] = RELEASED;
    buttonDebounce3[i] = RELEASED;
    buttonCounter[i] = 0;
    buttonFlag[i] = 0;
  }
}

void button_reading(){
  for(int i = 0; i < NUMBER_OF_BUTTONS; i++){
    buttonDebounce1[i] = buttonDebounce2[i];
    buttonDebounce2[i] = buttonDebounce3[i];
    buttonDebounce3[i] = digitalRead(buttonPin[i]);
    if(buttonDebounce1[i] == buttonDebounce2[i] && buttonDebounce2[i] == buttonDebounce3[i]){
      buttonState[i] = buttonDebounce1[i];
      if(buttonState[i] == PRESSED){
        if(buttonCounter[i] == 0) toggleLED(LED_PIN);
        buttonCounter[i]++;
        if((((buttonCounter[i] + 2) * TIMER_CYCLE) % DURATION_FOR_AUTO_OPERATION) == 0){
          buttonFlag[i] = 1;  
          toggleLED(LED_PIN);
        }
        else{
          buttonFlag[i] = 0;
        }
      }
      else{
        buttonCounter[i] = 0;
        buttonFlag[i] = 0;
      }
    }
  }
}
