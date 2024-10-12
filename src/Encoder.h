#ifndef ENCODER_H
  #define ENCODER_H

  #define DETENT_COUNT 20
  #include <Arduino.h>
  #include <RotaryEncoder.h>

namespace kitchenTimer {
  class Encoder {
    // This encoder will be timer based, with the encoder being the source of the timer
    public:
      Encoder(int pinA, int pinB);
      ~Encoder();
      int getValue();
    private:
      int pinA;
      int pinB;
      RotaryEncoder encoder;
  };
}
#endif
