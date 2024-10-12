#ifndef TIMER_H
  #define TIMER_H

namespace kitchenTimer {
  class Timer {
    public:
      Timer();
      Timer(int hours, int minutes, int seconds);
      void set(int hours, int minutes, int seconds);
      void set(unsigned long duration);
      int getHours();
      int getMinutes();
      int getSeconds();
      void start();
      void stop();
      void reset();
      unsigned long getTime();
      void getTimeStr(char *buff, int bufferLen, bool showAll = false);
      static void getTimeStr(char *buff, int bufferLen, int hours, int minutes, int seconds, bool showAll = false);
      int state();
      static const int RUNNING = 1;
      static const int STOPPED = 0;
    private:
      unsigned long secondsTimer;
      unsigned long secondsPast;
      unsigned long lastSecondChecked;
      int running = Timer::STOPPED;
      void tick();
  };
}

#endif
