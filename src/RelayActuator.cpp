#include "RelayActuator.h"

const Command RelayActuator::TURN_ON_COMMAND = Command(TURN_ON_COMMAND_ID);
const Command RelayActuator::TURN_OFF_COMMAND = Command(TURN_OFF_COMMAND_ID);

RelayActuator::RelayActuator(int pin, bool initialState, CommandHandler* commandHandler)
  : Actuator(pin, commandHandler), state(initialState) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, state);
}

void RelayActuator::handle(Command command) {
  if (command == TURN_ON_COMMAND) {
      state = true;
      digitalWrite(pin, state);
  } else if (command == TURN_OFF_COMMAND) {
      state = false;
      digitalWrite(pin, state);
  }
  Actuator::handle(command);
}

bool RelayActuator::getState() const {
  return state;
}

void RelayActuator::setState(bool newState) {
  state = newState;
  digitalWrite(pin, state);
}