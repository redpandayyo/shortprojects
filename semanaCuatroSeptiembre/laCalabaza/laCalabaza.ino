#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "images.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

void setup() {
  Wire.begin(21, 22);

  // Ayuda a actualizar la OLED mas rapido.
  Wire.setClock(400000);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  for (uint16_t i = 0; i < GATOS_FRAME_COUNT; i++) {

    display.clearDisplay();

    const unsigned char* frame =
      (const unsigned char*)pgm_read_ptr(&gatos_frames[i]);

    display.drawBitmap(
      0,
      0,
      frame,
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      SSD1306_WHITE
    );

    display.display();

    delay(GATOS_FRAME_DELAY);
  }
}
