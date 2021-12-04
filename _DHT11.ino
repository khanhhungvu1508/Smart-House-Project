#include "dht_nonblocking.h"

#define DHT_SENSOR_TYPE DHT_TYPE_11
#define DHT_SENSOR_PIN 12

DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

void init_DHT11_sensor(float *temperature, float *humidity){
  while(dht_sensor.measure( temperature, humidity ) != true){}
}

bool measure_environment(float *temperature, float *humidity)
{
  return dht_sensor.measure(temperature, humidity);
} 
