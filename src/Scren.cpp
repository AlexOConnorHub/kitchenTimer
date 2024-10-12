#include "Screen.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdint.h>
#include <string.h>

namespace kitchenTimer {
  Screen::Screen() {
    display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
  }

  Screen::~Screen() {
    if (printCache) {
      free(printCache);
    }
  }

  void Screen::setup() {
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      for(;;);
    }

    display.setTextColor(SSD1306_WHITE);
    display.clearDisplay();
    display.display();
  }

  void Screen::print(const char *text) {
    int16_t x, y;
    uint16_t w, h;

    for (int i = 4; i > 0; i--) {
      display.setTextSize(i);
      display.getTextBounds(text, 0, 0, &x, &y, &w, &h);
      if (w < SCREEN_WIDTH && h < SCREEN_HEIGHT) {
        break;
      }
    }

    display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);
    display.print(text);

    cachePrint(text);
  }

  void Screen::underline(int pos, int len) {
    if (pos < 0 || pos >= printCacheSize) {
      return;
    }
    if (pos + len > printCacheSize) {
      len = printCacheSize - pos;
    }

    int16_t x, y;
    uint16_t w, h;
    display.getTextBounds(printCache, 0, 0, &x, &y, &w, &h);
    int lineStart = (SCREEN_WIDTH - w) / 2 + x;
    char tmp[2];
    for (int i = 0; i < pos; i++) {
      sprintf(tmp, "%c", printCache[i]);
      display.getTextBounds(tmp, 0, 0, &x, &y, &w, &h);
      lineStart += w;
    }
    int lineEnd = lineStart;
    for (int i = 0; i < len; i++) {
      sprintf(tmp, "%c", printCache[pos + i]);
      display.getTextBounds(tmp, 0, 0, &x, &y, &w, &h);
      lineEnd += w;
    }
    int lineY = (SCREEN_HEIGHT - h) / 2 + h;
    int length = lineEnd - lineStart;
    display.drawFastHLine(lineStart, lineY, length, SSD1306_WHITE);
    display.drawFastHLine(lineStart, lineY + 1, length, SSD1306_WHITE);
  }

  void Screen::clear() {
    display.clearDisplay();
  }

  void Screen::show() {
    display.display();
  }

  bool Screen::checkCache(const char *text) {
    if (printCacheSize != strlen(text)) {
      return false;
    }

    return strcmp(printCache, text) == 0;
  }

  void Screen::cachePrint(const char *text) {
    int toCacheLen = strlen(text);
    if (toCacheLen > printCacheSize) {
      free(printCache);
      printCache = (char *)malloc(toCacheLen);
      printCacheSize = toCacheLen;
    }
    strcpy(printCache, text);
  }
}
