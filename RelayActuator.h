#ifndef RELAY_H
#define RELAY_H

#include "Actuator.h"
#include <Arduino.h>

class RelayActuator : public Actuator {
private:
  bool state;
public:
  static const int TURN_ON_COMMAND_ID = 1;
  static const int TURN_OFF_COMMAND_ID = 2;
  static const Command TURN_ON_COMMAND;
  static const Command TURN_OFF_COMMAND;

  RelayActuator(int pin, bool initialState = false, CommandHandler* commandHandler = nullptr);
  void handle(Command command) override;
  bool getState() const;
  void setState(bool newState);
};

#endif