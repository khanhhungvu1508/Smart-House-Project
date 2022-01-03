#include "_RFID.h"
#include "Arduino.h"
#include "timer.h"
#include "common.h"
#include "_LCD.h"
#include "scheduler.h"
#include "gateway.h"
#include <stdint.h>


#define MAX_OF_IDS 5
#define NO_OF_CHAR_ID 10

// Array save ID of cards in system, default have two card 
String IDs[MAX_OF_IDS] = {"12345admin", "12345aaaaa"};  
// Number of current cards in system
int curNoOfIDs = 2;
// Save state of RFID module, default is -1 (RFID is not ready to use)
int stateRFID = -1;
// If in admin session isAdmin will be 1, else will be 0, default is 0
bool isAdmin = 0;

// Save ID read from terminal
String id;
// Save character read from terminal
char rcvChar;
// If receive '$'(character before starting to input ID) character it's value will be 1, else will be 0
bool detect_receive_id = 0;
// Use for display information of module in each state
int display_code = 0;

// Save ID of task task_gateway_sending in scheduler
extern uint32_t ID_task_gateway;

// Function end operation of module RFID 
void exitRFID(){
  ID_task_gateway = SCH_Add_Task(task_gateway_sending, 0, PERIOD_GATEWAY_SENDING);
  detect_receive_id = 0;
  isAdmin = 0;
  stateRFID = -1;
  digitalWrite(MUX_PIN, LOW);
  Serial.println(F("\r\nLocking door"));
}

// Initiaization of module
void init_RFID(){
  pinMode(MUX_PIN, OUTPUT);
  digitalWrite(MUX_PIN, LOW);
  Serial.begin(9600);
  display_code = 0;
  task_RFID_display();
}

// Function change state of RFID
void changeStateRFID(){
  // Change to mode 0
  if(stateRFID == -1){
    // Change rx PIN for using RFID module
    digitalWrite(MUX_PIN, HIGH);
    SCH_Delete_Task(ID_task_gateway);
    stateRFID = 0;
    Serial.println(F("Please insert card ID !!!"));
    // Set time out waiting for insert card
    setTimer(0, TIMEOUT_WAITING_INPUT);
  }
  // Change to mode 1
  else if(stateRFID == 2 && isAdmin){
    resetTimer(0);
    stateRFID = 1;
    Serial.println(F("Please insert new card ID !!!"));
    display_code = 2;
    task_RFID_display();
  }
  else if(stateRFID == 1){
    // Change to mode -1(Stop RFID running)
    if(id.length() == 0){
      Serial.println(F("End admin session"));
      exitRFID();
      display_code = 0;
      task_RFID_display();
    }
    else if(id.length() == NO_OF_CHAR_ID){
      int i;
      for(i = 0; i < curNoOfIDs; i++){
        if(id == IDs[i]){
          break;
        }
      }
      // Change to mode 3
      if(i < curNoOfIDs){
        Serial.println(F("\r\nID card is exist"));
        stateRFID = 3;
        display_code = 5;
        task_RFID_display();
        setTimer(0, TIMEOUT_LCD_ANNOUNCEMENT);
      }
      // Change to mode 4
      else{
        IDs[curNoOfIDs++] = id;
        Serial.println(F("\r\nInsert new ID card successfully !!!"));
        stateRFID = 4;
        display_code = 4;
        task_RFID_display();
        setTimer(0, TIMEOUT_LCD_ANNOUNCEMENT);
      }
    }
    // Change to mode 3
    else{
      Serial.println(F("\r\nID card is wrong!!!"));
      stateRFID = 3;
      display_code = 6;
      task_RFID_display();
      setTimer(0, TIMEOUT_LCD_ANNOUNCEMENT);
    }
  }
  // Change to mode -1(Stop RFID running)
  else if(stateRFID != 3){
    exitRFID();
    display_code = 0;
    task_RFID_display();
  }
}

// RFID running base on state of module
void RFID_run(){
    switch(stateRFID){
    case 0: // Waiting insert RFID card
      if(getTimerFlag(0) == 1){
        exitRFID();
        resetTimer(0);
      }
      if(Serial.available() > 0){
        rcvChar =Serial.read();
        if(rcvChar == '$'){
          id = "";
          detect_receive_id = 1;
          resetTimer(0);
          Serial.print(rcvChar);
        }
        else if(detect_receive_id){
          if(id.length() < NO_OF_CHAR_ID){
            Serial.print(rcvChar);
            id += rcvChar;
          }
          if(id.length() == NO_OF_CHAR_ID){
            int i;
            for(i = 0; i < curNoOfIDs; i++){
              if(id == IDs[i]){
                Serial.print(F("\r\nValid ID\r\n"));
                display_code = 1;
                task_RFID_display();
                if(i == 0){
                 Serial.println(F("Start admin session"));
                  isAdmin = 1;
                }
                setTimer(0, TIMEOUT_FOR_RFID_SESSION);
                stateRFID = 2;
                break;
              }
            }
            if(i >= curNoOfIDs){
             Serial.print(F("\r\nInvalid ID\r\n"));
            }
            detect_receive_id = 0;
          }
        }
      }
      break;
    case 1: // Waiting insert new RFID card
      if(Serial.available() > 0){
        rcvChar =Serial.read();
        if(rcvChar == '$'){
          id = "";
          detect_receive_id = 1;
          Serial.print(rcvChar);
        }
        else if(detect_receive_id){
          if(id.length() < NO_OF_CHAR_ID){
            Serial.print(rcvChar);
            id += rcvChar;
          } 
        }
      }
      break; 
    case 2: // Time out waiting for state 0, 3s
      if(getTimerFlag(0) == 1){
        display_code = 0;
        task_RFID_display();
        if(isAdmin == 1){
          Serial.println(F("End admin session"));
        }
        exitRFID();
        resetTimer(0);
      }
      break;
    case 3: // Time out waiting for annoucing fail to insert new card
      if(getTimerFlag(0) == 1){
        id = "";
        display_code = 2;
        task_RFID_display();
        detect_receive_id = 0;
        stateRFID = 1;
        Serial.println(F("Please insert new card ID !!!"));
        resetTimer(0);
      }
      break;
    case 4: // Time out waiting for annoucing insert new card successdully
      if(getTimerFlag(0) == 1){
        display_code = 0;
        task_RFID_display();
        exitRFID();
        resetTimer(0);
      }
      break;
    default:
      break;
    }
}
