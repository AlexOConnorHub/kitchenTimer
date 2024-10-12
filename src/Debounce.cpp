#include "Debounce.h"
#include <Arduino.h>

namespace kitchenTimer {
  Debounce::Debounce(int pin) :
    pin(pin) {
    pinMode(pin, INPUT_PULLUP);
  }

  int Debounce::check() {
    int reading = digitalRead(pin);

    if (reading != lastReadState) {
      lastReadState = reading;
      lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
      currentState = lastReadState;
    }
    return currentState;
  }
}