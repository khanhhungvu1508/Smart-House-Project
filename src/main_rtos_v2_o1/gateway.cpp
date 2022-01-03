#include "Arduino.h"
#include "gateway.h"
#include "FAN.h"
#include "_LED.h"

// Save temperature value to send to server
extern float temperature;
// Save humidity value to send to server
extern float humidity;

// Save state of RFID 
extern int stateRFID;

enum RECEIVE_STATE{
  WAITING,
  RECEIVING
};

// Save character received 
char char_rcv;
// Save command received
String command;
// Save state of gateway receiving
RECEIVE_STATE rcv_state = WAITING;

// Initiaization of module
void init_gateway(){
  //Serial.begin(9600);
}

// Function gateway receiving
void gateway_command_received_parse(){
  // If RFID not run and have require from server
  if(stateRFID == -1 && Serial.available() > 0){
    char_rcv = Serial.read();
    switch(rcv_state){
      case WAITING:
        if(char_rcv == '#') {
          rcv_state = RECEIVING;
          Serial.print(char_rcv);
        }
        break;
      case RECEIVING:
        if(char_rcv != '*'){
          Serial.print(char_rcv);
          command += char_rcv;
        }
        else{
          if(command == "FAN_ON"){
            task_FAN_ON_OFF();
          }
          else if(command == "FAN_INC"){
            task_FAN_increaseMode();
          }
          else if(command == "FAN_AUTO"){
            task_FAN_auto();
          }
          else if(command == "LED_ON"){
            toggleLED();
          }
          command = "";
          rcv_state = WAITING;
          Serial.println();
        }
        break;
      default:
        break;
    }
  }
}

void task_gateway_sending(){
  String temp = "*" + String(temperature) + " " + String(humidity) + "#"; 
  Serial.println(temp);
}
