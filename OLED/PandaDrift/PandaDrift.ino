#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "images.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define PIN_LDR_DO 35

#define TIEMPO_CONFIRMACION 1500 // ms que debe mantenerse estable el cambio de luz antes de actuar

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool hayOscuridadConfirmada = false;
bool candidato = false;
unsigned long momentoCambioCandidato = 0;
bool animacionActiva = false;

bool leerOscuridad() {
  return digitalRead(PIN_LDR_DO) == HIGH;
}

void setup() {
  Wire.begin(21, 22);
  Wire.setClock(400000);
  pinMode(PIN_LDR_DO, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true) { delay(100); }
  }

  display.clearDisplay();
  display.display();

  hayOscuridadConfirmada = leerOscuridad();
  candidato = hayOscuridadConfirmada;
  momentoCambioCandidato = millis();
}

void reproducirAnimacionUnaVez() {
  for (uint16_t i = 0; i < SHIFT_FRAME_COUNT; i++) {
    bool lecturaActual = leerOscuridad();
    if (lecturaActual != candidato) {
      candidato = lecturaActual;
      momentoCambioCandidato = millis();
    }

    if (candidato != hayOscuridadConfirmada &&
        millis() - momentoCambioCandidato >= TIEMPO_CONFIRMACION) {
      hayOscuridadConfirmada = candidato;
      animacionActiva = false;
      display.clearDisplay();
      display.display();
      return;
    }

    display.clearDisplay();

    const unsigned char* frame =
      (const unsigned char*)pgm_read_ptr(&shift_frames[i]);

    display.drawBitmap(
      0, 0,
      frame,
      SHIFT_FRAME_WIDTH,
      SHIFT_FRAME_HEIGHT,
      SSD1306_WHITE
    );

    display.display();
    delay(SHIFT_FRAME_DELAY);
  }
}

void loop() {
  bool lecturaActual = leerOscuridad();

  if (lecturaActual != candidato) {
    candidato = lecturaActual;
    momentoCambioCandidato = millis();
  }

  if (candidato != hayOscuridadConfirmada &&
      millis() - momentoCambioCandidato >= TIEMPO_CONFIRMACION) {
    hayOscuridadConfirmada = candidato;

    if (!hayOscuridadConfirmada) {
      animacionActiva = false;
      display.clearDisplay();
      display.display();
    }
  }

  if (hayOscuridadConfirmada) {
    animacionActiva = true;
    reproducirAnimacionUnaVez();
  } else {
    delay(100);
  }
}
