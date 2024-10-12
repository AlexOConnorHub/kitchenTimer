#ifndef SCREEN_H
  #define SCREEN_H

  #include <Adafruit_SSD1306.h>

  #define SCREEN_WIDTH 128
  #define SCREEN_HEIGHT 32
  #define SCREEN_ADDRESS 0x3C


namespace kitchenTimer {
  class Screen {
    public:
      Screen();
      ~Screen();
      void setup();
      void print(const char *text);
      void underline(int pos, int len);
      void show();
      void clear();
    private:
      Adafruit_SSD1306 display;
      char *printCache;
      int printCacheSize = 0;
      bool checkCache(const char *text);
      void cachePrint(const char *text);
  };
}

#endif
