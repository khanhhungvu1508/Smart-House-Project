#define AC_PIN 3

#define NO_OF_MODE 3
#define MAX_AC_TEMPERATURE 32
#define MIN_AC_TEMPERATURE 16

int mode = 0;
int ACTemperature = MAX_AC_TEMPERATURE;
int ACSpeed = 0;
int autoTemperature = 20;

void increaseMode(){
  if(mode == -1) mode = 2;
  else{
    mode++;
    if(mode >= NO_OF_MODE - 1){
      mode = 0;
    }
  }
  AC_display(mode, ACTemperature, autoTemperature);
}

void changeAutoMode(){
  mode = -1;
  AC_display(mode, ACTemperature, autoTemperature);
}

void decreaseACTemperature(){
  ACTemperature--;
  if(ACTemperature < MIN_AC_TEMPERATURE){
    ACTemperature = MAX_AC_TEMPERATURE;
  }
  AC_display(mode, ACTemperature, autoTemperature);
}

void increaseACTemperature(){
  ACTemperature++;
  if(ACTemperature > MAX_AC_TEMPERATURE){
    ACTemperature = MIN_AC_TEMPERATURE;
  }
  AC_display(mode, ACTemperature, autoTemperature);
}

void decreaseACAuto(){
  autoTemperature--;
  if(autoTemperature < MIN_AC_TEMPERATURE){
    autoTemperature = MAX_AC_TEMPERATURE;
  }
  AC_display(mode, ACTemperature, autoTemperature);
}

void increaseACAuto(){
  autoTemperature++;
  if(autoTemperature > MAX_AC_TEMPERATURE){
    autoTemperature = MIN_AC_TEMPERATURE;
  }
  AC_display(mode, ACTemperature, autoTemperature);
}

void init_AC(){
  AC_display(mode, ACTemperature, autoTemperature);
}

void AC_run(){
  switch(mode){
    case 0: // OFF
      ACSpeed = 0;
      break;
    case 1: // MANUAL
      ACSpeed = 255 - (205 * (ACTemperature - MIN_AC_TEMPERATURE) / (MAX_AC_TEMPERATURE - MIN_AC_TEMPERATURE));
      break;
    case -1:
    case 2: // AUTO
      if(temperature <= autoTemperature) ACTemperature = autoTemperature;
      else{
         ACTemperature = MIN_AC_TEMPERATURE;
      }
      ACSpeed = 255 - (205 * (ACTemperature - MIN_AC_TEMPERATURE) / (MAX_AC_TEMPERATURE - MIN_AC_TEMPERATURE));
      break;
    default:
      break;
  }
  analogWrite(AC_PIN, ACSpeed);
}
