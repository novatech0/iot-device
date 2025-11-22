#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include "Sensor.h"
#include <Arduino.h>

class UltrasonicSensor : public Sensor {
private:
  int triggerPin;
  int echoPin;
  float tankHeight;
  float tankVolume;
  float currentVolume;

public:
  static const int VOLUME_CHANGED_EVENT_ID = 0;
  static const Event VOLUME_CHANGED_EVENT;

  UltrasonicSensor(int triggerPin, int echoPin, float tankHeight, float tankVolume, EventHandler* handler = nullptr);
  float getDistance();
  float getWaterHeight();
  float getVolume();
  void setTankHeight(float height);
  void setTankVolume(float volume);
  void updateData();
};

#endif
