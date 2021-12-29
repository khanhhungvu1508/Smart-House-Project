#include "_RFID.h"
#include "Arduino.h"
#include "timer.h"
#include "common.h"
#include "_LCD.h"
#include "scheduler.h"

#define MAX_OF_IDS 5
#define NO_OF_CHAR_ID 10

String IDs[MAX_OF_IDS] = {"12345admin", "12345aaaaa"};  
int curNoOfIDs = 2;
int stateRFID = 0;
bool isAdmin = 0;

String id;
char rcvChar;
int display_code = 0;

void init_RFID(){
  Serial.begin(9600);
  Serial.println("Please insert card ID !!!");
  display_code = 0;
  SCH_Add_Task(task_RFID_display, 0, 0);
}

void clearBuffer(){
  char temp[10];
  Serial.readBytesUntil("\0", temp, 10);
}

void changeStateRFID(){
  if(stateRFID == 2 && isAdmin) {
    resetTimer(0);
    stateRFID = 1;
    clearBuffer();
    Serial.println("Please insert new card ID !!!");
    display_code = 2;
    SCH_Add_Task(task_RFID_display, 0, 0);
  }
  else if(stateRFID == 1){
    if(id.length() == 0){
      stateRFID = 0;
      isAdmin = 0;
      Serial.println("End admin session");
      display_code = 0;
      SCH_Add_Task(task_RFID_display, 0, 0);
      clearBuffer();
      Serial.println("Please insert card ID !!!");
    }
    else if(id.length() == NO_OF_CHAR_ID){
      int i;
      for(i = 0; i < curNoOfIDs; i++){
        if(id == IDs[i]){
          break;
        }
      }
      if(i < curNoOfIDs){
        Serial.println("\r\nID card is valid");
        stateRFID = 3;
        display_code = 5;
        SCH_Add_Task(task_RFID_display, 0, 0);
        setTimer(0, 500);
      }
      else{
        IDs[curNoOfIDs++] = id;
        Serial.println("\r\nInsert new ID card successfully !!!");
        isAdmin = 0;
        stateRFID = 4;
        display_code = 4;
        SCH_Add_Task(task_RFID_display, 0, 0);
        setTimer(0, 500);
      }
    }
    else{
      Serial.println("\r\nID card is wrong!!!");
      isAdmin = 0;
      display_code = 6;
      SCH_Add_Task(task_RFID_display, 0, 0);
      stateRFID = 3;
    }
  }
  else{
    stateRFID = 0;
    display_code = 0;
    SCH_Add_Task(task_RFID_display, 0, 0);
    clearBuffer();
    Serial.println("Please insert card ID !!!");
  }
  id = "";
}

void RFID_run(){
    switch(stateRFID){
    case 0:
      if(Serial.available() > 0){
        if(id.length() < NO_OF_CHAR_ID){
          rcvChar = Serial.read();
          Serial.print(rcvChar);
          id += rcvChar;
        }
      }
      else if(id.length() == NO_OF_CHAR_ID){
          int i;
          for(i = 0; i < curNoOfIDs; i++){
            if(id == IDs[i]){
              Serial.print("\r\nValid ID\r\n");
              display_code = 1;
              SCH_Add_Task(task_RFID_display, 0, 0);
              if(i == 0){
                Serial.println("Start admin session");
                isAdmin = 1;
              }
              setTimer(0, TIMEOUT_FOR_RFID_SESSION);
              stateRFID = 2;
              break;
            }
          }
          if(i >= curNoOfIDs){
            Serial.print("\r\nInvalid ID\r\n");
          }
          id = "";
      }
      break;
    case 1:
      if(Serial.available() > 0){
        if(id.length() < NO_OF_CHAR_ID){
          rcvChar = Serial.read();
          Serial.print(rcvChar);
          id += rcvChar;
        }
      }
      break;
    case 2:
      if(getTimerFlag(0) == 1){
        display_code = 0;
        SCH_Add_Task(task_RFID_display, 0, 0);
        stateRFID = 0;
        if(isAdmin == 1){
          Serial.println("End admin session");
          isAdmin = 0;
        }
        clearBuffer();
        Serial.println("Please insert card ID !!!");
      }
      break;
    case 3:
      if(getTimerFlag(0) == 1){
        display_code = 2;
        SCH_Add_Task(task_RFID_display, 0, 0);
        stateRFID = 1;
        clearBuffer();
        Serial.println("Please insert new card ID !!!");
      }
      break;
    case 4:
      if(getTimerFlag(0) == 1){
        display_code = 0;
        SCH_Add_Task(task_RFID_display, 0, 0);
        stateRFID = 0;
        clearBuffer();
        Serial.println("Please insert card ID !!!");
      }
      break;
    default:
      break;
    }
}
