#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include "ICommunication.h"

class MQTTManager : public ICommunication {
private:
    const char* ssid;
    const char* password;
    const char* mqttServer;
    uint16_t mqttPort;
    const char* mqttTopic;
    WiFiClient wifiClient;
    PubSubClient mqttClient;
    String rxBuffer;
    unsigned long lastReconnectAttempt;
    const unsigned long reconnectInterval;
    void reconnect();
    void setData(const String& message);
public:
    MQTTManager(const char* ssid, const char* password, const char* mqttServer, uint16_t mqttPort, const char* mqttTopic);
    void begin() override;
    void handle() override;
    bool isConnected() override;
    String getAddress() override;
    bool sendData(const String& data) override;
    bool hasData() override;
    String receiveData() override;
};

#endif