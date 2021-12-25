#include "dht_nonblocking.h"
#include "_DHT11_nonblocking.h"
#include "Arduino.h"

#define DHT_SENSOR_TYPE DHT_TYPE_11
#define DHT_SENSOR_PIN 12

DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

extern float temperature; 
extern float humidity;

void init_DHT11_nonblocking(){
  unsigned long startTime = millis();
  while((dht_sensor.measure(&temperature, &humidity) != true) && (millis() - startTime < 500ul)){}
}

bool task_measure_environment_nonblocking()
{
  return dht_sensor.measure(&temperature, &humidity);
} 
