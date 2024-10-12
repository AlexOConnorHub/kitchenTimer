#include "timer.h"
#include <Arduino.h>
#include <stdio.h>
#include <limits.h>

namespace kitchenTimer {
  Timer::Timer() {};

  Timer::Timer(int hours, int minutes, int seconds) {
    this->set(hours, minutes, seconds);
  };

  void Timer::set(int hours, int minutes, int seconds) {
    this->secondsTimer = hours * 3600 + minutes * 60 + seconds;
  };

  void Timer::set(unsigned long duration) {
    this->secondsTimer = duration;
  };

  int Timer::getHours() {
    return this->secondsTimer / 3600;
  }

  int Timer::getMinutes() {
    return (this->secondsTimer % 3600) / 60;
  }

  int Timer::getSeconds() {
    return (this->secondsTimer % 60);
  }

  void Timer::start() {
    if (this->running) {
      return;
    }
    tick();
    this->running = Timer::RUNNING;
  };

  void Timer::stop() {
    if (!this->running) {
      return;
    }
    tick();
    this->running = Timer::STOPPED;
  };

  void Timer::reset() {
    this->secondsTimer = 0;
    this->secondsPast = 0;
    this->running = Timer::STOPPED;
  };

  unsigned long Timer::getTime() {
    tick();
    return this->secondsTimer - this->secondsPast;
  }

  void Timer::getTimeStr(char *buff, int bufferLen, bool showAll) {
    unsigned long time = this->getTime();
    int hours = (int)(time / 3600);
    int minutes = ((int)time % 3600) / 60;
    int seconds = (int)time % 60;
    Timer::getTimeStr(buff, bufferLen, hours, minutes, seconds, showAll);
  }

  void Timer::getTimeStr(char *buff, int bufferLen, int  hours, int minutes, int seconds, bool showAll) {
    if (showAll) {
      snprintf(buff, bufferLen, "%02d:%02d:%02d", hours, minutes, seconds);
    } else {
      if (hours > 0) {
        snprintf(buff, bufferLen, "%02d:%02d", hours, minutes);
      } else {
        snprintf(buff, bufferLen, "%02d:%02d", minutes, seconds);
      }
    }
  }

  int Timer::state() {
    return this->running;
  }

  void Timer::tick() {
    unsigned long currentSecond = millis() / 1000;
    if (this->running) {
      unsigned long secondsElapsed;
      if (currentSecond < this->lastSecondChecked) {
        secondsElapsed = (ULONG_MAX - this->lastSecondChecked) + currentSecond;
      } else {
        secondsElapsed = currentSecond - this->lastSecondChecked;
      }
      this->secondsPast += secondsElapsed;
    }
    this->lastSecondChecked = currentSecond;
  }
}
