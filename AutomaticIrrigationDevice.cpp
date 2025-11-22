#include "AutomaticIrrigationDevice.h"

AutomaticIrrigationDevice::AutomaticIrrigationDevice(
  ICommunication* communication,
  int dht22Pin,
  int relayPin,
  int trigPin,
  int echoPin
) : dht22Sensor(dht22Pin, this),
    relayActuator(relayPin, false, this),
    ultrasonicSensor(trigPin, echoPin, TANK_HEIGHT_CM, TANK_AREA_CM2, this),
    comm(communication)
{}

void AutomaticIrrigationDevice::on(Event event) {
  if (event == UltrasonicSensor::VOLUME_CHANGED_EVENT) {
    handleVolumeChange();
  } else if (event == DHT22Sensor::TEMPERATURE_CHANGED_EVENT || event == DHT22Sensor::HUMIDITY_CHANGED_EVENT) {
    handleEnvironmentalChange();
  }
}

void AutomaticIrrigationDevice::handle(Command command) {
  if (command == RelayActuator::TURN_ON_COMMAND || 
      command == RelayActuator::TURN_OFF_COMMAND) {
    Serial.printf("Relay state: %d\n", relayActuator.getState());
  }
}

void AutomaticIrrigationDevice::handleVolumeChange() {
  float volume = ultrasonicSensor.getVolume();
  float volumePercent = (volume / TANK_TOTAL_VOLUME_LITERS) * 100.0;

  Serial.printf("Volumen actualizado: %.2fL (%.1f%%)\n", volume, volumePercent);

  if (volumePercent <= TANK_MIN_VOLUME_THRESHOLD) {
    if (relayActuator.getState()) {
      relayActuator.handle(RelayActuator::TURN_OFF_COMMAND);
      Serial.print("Nivel crítico de agua. Riego cancelado.");
    } else {
      Serial.print("Nivel crítico de agua. Riego ya está desactivado.");
    }
  }
}

void AutomaticIrrigationDevice::handleEnvironmentalChange() {
  float temperature = dht22Sensor.getTemperature();
  float humidity = dht22Sensor.getHumidity();
  float volume = ultrasonicSensor.getVolume();
  float volumePercent = (volume / TANK_TOTAL_VOLUME_LITERS) * 100.0;

  Serial.printf("Lectura ambiental -> Temp: %.2f°C | Hum: %.2f%% | Vol: %.2fL (%.1f%%)\n",
                temperature, humidity, volume, volumePercent);
  if (volumePercent <= TANK_MIN_VOLUME_THRESHOLD) {
    Serial.print("Nivel de agua crítico. Se cancela el riego.");
    return;
  }
  bool hasInvalidConditions = (temperature >= temperatureThreshold || humidity <= humidityThreshold);
  if (hasInvalidConditions) {
    relayActuator.handle(RelayActuator::TURN_ON_COMMAND);
    Serial.println("Condición anómala: Activando riego.");
  }
  else {
    relayActuator.handle(RelayActuator::TURN_OFF_COMMAND);
    Serial.println("Condición normal: Riego desactivado.");
  }
}

void AutomaticIrrigationDevice::updateSensors() {
  dht22Sensor.updateData();
  ultrasonicSensor.updateData();
}

void AutomaticIrrigationDevice::connectEdge() {
  if (comm && comm->isConnected()) {
    // Recibimiento de datos
    if (comm->hasData()) {
      String incoming = comm->receiveData();
      JsonDocument config;
      DeserializationError err = deserializeJson(config, incoming);

      if (!err) {
        if (config.containsKey("temperature_max")) {
          temperatureThreshold = config["temperature_max"].as<float>();
          Serial.printf("Nuevo límite temperatura: %.2f°C\n", temperatureThreshold);
          AutomaticIrrigationDevice::handleEnvironmentalChange();
        }
        if (config.containsKey("humidity_min")) {
          humidityThreshold = config["humidity_min"].as<float>();
          Serial.printf("Nuevo límite humedad: %.2f%%\n", humidityThreshold);
          AutomaticIrrigationDevice::handleEnvironmentalChange();
        }
      } else {
        Serial.println("Error al parsear JSON recibido.");
      }
    }

    // Envio de datos
    float temp = dht22Sensor.getTemperature();
    float hum = dht22Sensor.getHumidity();
    float volume = ultrasonicSensor.getVolume();

    JsonDocument data;
    data["sender"] = "device";
    data["temperature"] = temp;
    data["humidity"] = hum;
    data["volume"] = volume;
    String resource;
    serializeJson(data, resource);

    if (comm->sendData(resource)) {
      Serial.println("Datos enviados al servidor.");
    } else {
      Serial.println("Error enviando datos.");
    }
  }
}

DHT22Sensor& AutomaticIrrigationDevice::getDHT() {
  return dht22Sensor;
}

RelayActuator& AutomaticIrrigationDevice::getRelay() {
  return relayActuator;
}

UltrasonicSensor& AutomaticIrrigationDevice::getUltrasonic() {
  return ultrasonicSensor;
}