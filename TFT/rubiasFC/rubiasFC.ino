#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>
#include "images.h"

#define PIN_MIC_OUT 32

TFT_eSPI tft = TFT_eSPI();

bool tftOutput(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  if (y >= tft.height()) return false;
  tft.pushImage(x, y, w, h, bitmap);
  return true;
}

void reproducirVideo() {
  for (uint16_t i = 0; i < VIDEO_FRAME_COUNT; i++) {
    const uint8_t* jpg = (const uint8_t*)pgm_read_ptr(&video_frames[i]);
    uint16_t jpgSize = pgm_read_word(&video_frame_sizes[i]);

    uint32_t frameStart = millis();
    TJpgDec.drawJpg(0, 0, jpg, jpgSize);

    uint32_t elapsed = millis() - frameStart;
    if (elapsed < VIDEO_FRAME_DELAY) {
      delay(VIDEO_FRAME_DELAY - elapsed);
    }
  }

  // El video ya terminó: se limpia la pantalla y queda en negro esperando
  tft.fillScreen(TFT_BLACK);
}

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(PIN_MIC_OUT, INPUT);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(tftOutput);
}

void loop() {
  int sonido = digitalRead(PIN_MIC_OUT);

  if (sonido == HIGH) {
    Serial.println("Sonido detectado, reproduciendo...");
    reproducirVideo();
    Serial.println("Video terminado, esperando sonido de nuevo");
    delay(500);
  } else {
    delay(50);
  }
}
