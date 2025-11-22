#include "DHT22Sensor.h"

const Event DHT22Sensor::HUMIDITY_CHANGED_EVENT(DHT22Sensor::HUMIDITY_CHANGED_EVENT_ID);
const Event DHT22Sensor::TEMPERATURE_CHANGED_EVENT(DHT22Sensor::TEMPERATURE_CHANGED_EVENT_ID);

DHT22Sensor::DHT22Sensor(int pin, EventHandler* eventHandler)
  : Sensor(pin, eventHandler) {
  pinMode(pin, INPUT);
  dht.setup(pin, DHTesp::DHT22);
}

float DHT22Sensor::getTemperature() {
  return dht.getTemperature();
}

float DHT22Sensor::getHumidity() {
  return dht.getHumidity();
}

TempAndHumidity DHT22Sensor::getTempAndHumidity() {
  return dht.getTempAndHumidity();
}

void DHT22Sensor::updateData() {
  TempAndHumidity data = dht.getTempAndHumidity();
  if (handler) {
    if (abs(data.temperature - temperature) >= 0.5) { // Valida que los cambios sean significativos
      temperature = data.temperature;
      handler->on(TEMPERATURE_CHANGED_EVENT);
    }

    if (abs(data.humidity - humidity) >= 1.0) { // Valida que los cambios sean significativos
      humidity = data.humidity;
      handler->on(HUMIDITY_CHANGED_EVENT);
    }
  } else {
    temperature = data.temperature;
    humidity = data.humidity;
  }
}