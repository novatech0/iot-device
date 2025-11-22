#include "UltrasonicSensor.h"

const Event UltrasonicSensor::VOLUME_CHANGED_EVENT(UltrasonicSensor::VOLUME_CHANGED_EVENT_ID);

UltrasonicSensor::UltrasonicSensor(int triggerPin, int echoPin, float tankHeight, float tankArea, EventHandler* handler)
  : Sensor(triggerPin, handler), triggerPin(triggerPin), echoPin(echoPin),
    tankHeight(tankHeight), tankArea(tankArea), currentVolume(0.0f) {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

float UltrasonicSensor::getDistance() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout de 5 minutos para la recepcion de datos (puede cambiar con altura de tanque) 
  if (duration == 0) return tankHeight; // falló la lectura, asume tanque vacío
  return duration / 58; // se devuelve la distancia en cm
}

float UltrasonicSensor::getWaterHeight() {
  float distance = getDistance();
  float height = tankHeight - distance;
  return height < 0 ? 0 : height;
}

float UltrasonicSensor::getVolume() {
  float height = getWaterHeight();
  return height * tankArea / 1000.0; // cm3 a litros
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
