#include "Encoder.h"
#include <Arduino.h>
#include <RotaryEncoder.h>

namespace kitchenTimer {
  Encoder::Encoder(int pinA, int pinB) : pinA(pinA), pinB(pinB), encoder(RotaryEncoder(pinA, pinB)) {
    attachInterrupt(digitalPinToInterrupt(pinA), [this]() { encoder.tick(); }, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pinB), [this]() { encoder.tick(); }, CHANGE);
  }

  Encoder::~Encoder() {
    detachInterrupt(digitalPinToInterrupt(pinA));
    detachInterrupt(digitalPinToInterrupt(pinB));
  }

  int Encoder::getValue() {
    encoder.tick();
    return encoder.getPosition();
  }

}