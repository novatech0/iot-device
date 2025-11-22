#include "MQTTManager.h"

MQTTManager::MQTTManager(const char* ssid, const char* password, const char* mqttServer, uint16_t mqttPort, const char* mqttTopic)
    : ssid(ssid), password(password), mqttServer(mqttServer), mqttPort(mqttPort), mqttTopic(mqttTopic),
      mqttClient(wifiClient), rxBuffer(""), lastReconnectAttempt(0), reconnectInterval(10000) {}

void MQTTManager::begin() {
  WiFi.begin(ssid, password);
  Serial.println("Conectando a WiFi");
  mqttClient.setServer(mqttServer, mqttPort);

  // Define el callback que se ejecuta automáticamente cuando llega un mensaje MQTT
  mqttClient.setCallback([this](char* topic, byte* payload, unsigned int length) {
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    this->rxBuffer = message;
  });

  randomSeed(analogRead(0)); // Inicializa la semilla para random()
}

void MQTTManager::handle() {
  if (WiFi.status() != WL_CONNECTED) {
      unsigned long now = millis();
      if (now - lastReconnectAttempt > reconnectInterval) {
          Serial.println("\nIntentando reconectar a WiFi...");
          WiFi.disconnect();
          WiFi.begin(ssid, password);
          lastReconnectAttempt = now;
      }
      Serial.print(".");
      return;
  }
  if (!mqttClient.connected()) {
      reconnect();
  }
  mqttClient.loop();
}

void MQTTManager::reconnect() {
  // Usar un nombre de cliente único para evitar colisiones en el broker público
  String clientId = "ESP32Client_";
  clientId += String(random(0xffff), HEX); // Sufijo aleatorio
  while (!mqttClient.connected()) {
      Serial.print("Intentando conectar al broker MQTT...");
      if (mqttClient.connect(clientId.c_str())) {
          Serial.println("Conectado a MQTT!");
          mqttClient.subscribe(mqttTopic); // Suscribirse si se requiere
      } else {
          Serial.print("Fallo, rc=");
          Serial.print(mqttClient.state());
          Serial.println(" intentando de nuevo en 5 segundos");
          delay(5000);
      }
  }
}

bool MQTTManager::isConnected() {
    return WiFi.status() == WL_CONNECTED && mqttClient.connected();
}

String MQTTManager::getAddress() {
  if (isConnected()) {
      return WiFi.localIP().toString();
  }
  return "0.0.0.0";
}

bool MQTTManager::sendData(const String& data) {
  if (!isConnected()) {
      Serial.println("No conectado a WiFi/MQTT. No se pueden enviar datos.");
      return false;
  }
  bool result = mqttClient.publish(mqttTopic, data.c_str());
  if (result) {
      Serial.println("Datos publicados en MQTT: " + data);
  } else {
      Serial.println("Error publicando en MQTT");
  }
  return result;
}

void MQTTManager::setData(const String& message) {
  rxBuffer = message;
}

bool MQTTManager::hasData() {
    return rxBuffer.length() > 0;
}

String MQTTManager::receiveData() {
    String data = rxBuffer;
    rxBuffer = ""; // Limpiar el buffer después de leerlo
    return data;
}