#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "images.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET   -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void clearAndPause(unsigned long ms) {
  display.clearDisplay();
  display.display();
  delay(ms);
}

void showBitmapFrame(const unsigned char *bitmap, unsigned long ms) {
  display.clearDisplay();
  display.drawBitmap(0, 0, bitmap, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.display();
  delay(ms);
}

void showTextFrame(const char *text, uint8_t size, unsigned long ms) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(size);

  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  int16_t x = (SCREEN_WIDTH - w) / 2;
  int16_t y = (SCREEN_HEIGHT - h) / 2;

  display.setCursor(x, y);
  display.print(text);
  display.display();
  delay(ms);
}

void runAnimation() {
  // frame 1: fisheye
  showBitmapFrame(panda_fisheye, 3000);
  clearAndPause(180);

  // frame 2: screaming
  showBitmapFrame(panda_scream, 3000);
  clearAndPause(180);

  // frame 3: final scream
  showTextFrame("Ahhhhh!!!", 2, 3000);
  clearAndPause(450);
}

void setup() {
  Wire.begin(21, 22); // SDA, SCL for common ESP32 boards

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true) { delay(100); }
  }

  display.clearDisplay();
  display.display();

  // Give the OLED a short moment after boot.
  delay(500);
}

void loop() {
  // Loop forever so the image changes are clearly visible.
  runAnimation();
}
