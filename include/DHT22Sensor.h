#ifndef DHT22SENSOR_H
#define DHT22SENSOR_H

#include "Sensor.h"
#include <DHTesp.h>
#include <Arduino.h>

class DHT22Sensor : public Sensor {
private:
  float humidity;
  float temperature;
  DHTesp dht;
public:
  static const int HUMIDITY_CHANGED_EVENT_ID = 1;
  static const int TEMPERATURE_CHANGED_EVENT_ID = 2;
  static const Event HUMIDITY_CHANGED_EVENT;
  static const Event TEMPERATURE_CHANGED_EVENT;
  DHT22Sensor(int pin, EventHandler* eventHandler = nullptr);
  float getHumidity();
  float getTemperature();
  TempAndHumidity getTempAndHumidity();
  void updateData();
};

#endif