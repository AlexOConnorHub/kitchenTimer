#ifndef DEBOUNCE_H
  #define DEBOUNCE_H
#include <Arduino.h>

#define DEBOUNCE_DELAY 25

namespace kitchenTimer {
  class Debounce {
    public:
      Debounce(int pin);
      int check();
    private:
      int pin;
      int lastReadState = HIGH;
      int currentState = HIGH;
      unsigned long lastDebounceTime;
  };
}

#endif
