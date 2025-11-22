#ifndef ICOMMUNICATION_H
#define ICOMMUNICATION_H

#include <Arduino.h>

class ICommunication {
public:
    virtual ~ICommunication() {}
    virtual void begin() = 0;
    virtual void handle() = 0;
    virtual bool isConnected() = 0;
    virtual String getAddress() = 0;
    virtual bool sendData(const String& data) = 0;
    virtual bool hasData() = 0;
    virtual String receiveData() = 0;
};

#endif
