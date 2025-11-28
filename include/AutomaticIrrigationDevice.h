#ifndef AUTOMATIC_IRRIGATION_DEVICE_H
#define AUTOMATIC_IRRIGATION_DEVICE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include "Device.h"
#include "DHT22Sensor.h"
#include "RelayActuator.h"
#include "UltrasonicSensor.h"
#include "ICommunication.h"

class AutomaticIrrigationDevice : public Device {
private:
  DHT22Sensor dht22Sensor;
  RelayActuator relayActuator;
  UltrasonicSensor ultrasonicSensor;
  ICommunication* comm; // Comunicación (WiFiManager, MQTTManager u otra);

  static constexpr int CROP_ID = 1;
  static constexpr float INITIAL_TANK_VOLUME = 100.0;
  static constexpr float INITIAL_TANK_HEIGHT = 100.0;
  static constexpr float TANK_MIN_VOLUME_THRESHOLD = 5.0; // %
  float temperatureThreshold = 30.0; // °C
  float humidityThreshold = 25.0;    // %
  float tankVolume = INITIAL_TANK_VOLUME; // L
  float tankHeight = INITIAL_TANK_HEIGHT; // cm
  
public:
  static const int DHT22_PIN = 18;
  static const int RELAY_PIN = 19;
  static const int TRIGGER_PIN = 12;
  static const int ECHO_PIN = 14;

  AutomaticIrrigationDevice(
    ICommunication* communication,
    int dht22Pin = DHT22_PIN,
    int relayPin = RELAY_PIN,
    int trigPin = TRIGGER_PIN,
    int echoPin = ECHO_PIN
  );

  void on(Event event) override;
  void handle(Command command) override;
  void handleVolumeChange();
  void handleEnvironmentalChange();
  void setTankParameters(float newHeight, float newVolume);
  void setTemperatureThreshold(float newTemperatureThreshold);
  void setHumidityThreshold(float newHumidityThreshold);

  void updateSensors();
  void connectEdge();
  
  DHT22Sensor& getDHT();
  RelayActuator& getRelay();
  UltrasonicSensor& getUltrasonic();
};

#endif
