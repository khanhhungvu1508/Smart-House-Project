#include <DHT.h>
#include "_DHT11.h"
#include "Arduino.h"

const int DHTPIN = 12;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

extern float temperature; 
extern float humidity;

void init_DHT11(){
  dht.begin();
}

void task_measure_environment(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if(!isnan(t) && !isnan(h)){
    (temperature) = t;
    (humidity) = h;
  }
}
