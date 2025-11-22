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

  float temperatureThreshold = 30.0; // °C
  float humidityThreshold = 25.0;    // %
  static constexpr float TANK_MIN_VOLUME_THRESHOLD = 5.0; // %
  static constexpr float TANK_HEIGHT_CM = 100.0;       // cm
  static constexpr float TANK_AREA_CM2 = 1000.0;       // cm²
  static constexpr float TANK_TOTAL_VOLUME_LITERS = (TANK_HEIGHT_CM * TANK_AREA_CM2) / 1000.0;
  float calculateTankVolumePercent(float distanceCm);

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

  void updateSensors();
  void connectEdge();

  DHT22Sensor& getDHT();
  RelayActuator& getRelay();
  UltrasonicSensor& getUltrasonic();
};

#endif
