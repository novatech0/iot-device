#include "UltrasonicSensor.h"

const Event UltrasonicSensor::VOLUME_CHANGED_EVENT(UltrasonicSensor::VOLUME_CHANGED_EVENT_ID);

UltrasonicSensor::UltrasonicSensor(int triggerPin, int echoPin, float tankHeight, float tankVolume, EventHandler* handler)
  : Sensor(triggerPin, handler), triggerPin(triggerPin), echoPin(echoPin),
    tankHeight(tankHeight), tankVolume(tankVolume), currentVolume(0.0f) {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void UltrasonicSensor::setTankHeight(float height) {
  tankHeight = height;
}
void UltrasonicSensor::setTankVolume(float volume) {
  tankVolume = volume;
}

float UltrasonicSensor::getDistance() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout
  if (duration == 0) return tankHeight; // falló la lectura, asume valor previo
  return duration / 58; // cm
}

float UltrasonicSensor::getWaterHeight() {
  float distance = getDistance();
  float height = tankHeight - distance;
  return height < 0 ? 0 : height;
}

float UltrasonicSensor::getVolume() {
  float height = getWaterHeight();
  // volumen proporcional al porcentaje de altura respecto al total
  return tankVolume * (height / tankHeight); 
}

void UltrasonicSensor::updateData() {
  float newVolume = getVolume();
  if (abs(newVolume - currentVolume) >= 1.0) {
    currentVolume = newVolume;
    if (handler) {
      handler->on(VOLUME_CHANGED_EVENT);
    }
  }
}
