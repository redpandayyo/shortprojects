#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ANCHO_PANTALLA 128
#define ALTO_PANTALLA 64
#define OLED_RESET -1
#define PIN_BOTON 5

Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, OLED_RESET);

int radio = 10;
const int radioMax = 30;
bool explotando = false;

void setup() {
  Wire.begin(21, 22);
  pinMode(PIN_BOTON, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }
  display.clearDisplay();
}

void dibujarCara(int r) {
  int cx = ANCHO_PANTALLA / 2;
  int cy = ALTO_PANTALLA / 2;

  display.clearDisplay();
  display.drawCircle(cx, cy, r, SSD1306_WHITE);

  // Ojos: se separan un poco entre sí mientras crece
  int separacionOjos = r / 3;
  display.fillCircle(cx - separacionOjos, cy - r / 3, 2, SSD1306_WHITE);
  display.fillCircle(cx + separacionOjos, cy - r / 3, 2, SSD1306_WHITE);

  // Sonrisa: se deforma un poco (más recta) mientras se infla
  int anchoSonrisa = r / 2;
  display.drawLine(cx - anchoSonrisa, cy + r / 3, cx + anchoSonrisa, cy + r / 3, SSD1306_WHITE);

  display.display();
}

void efectoExplosion() {
  int cx = ANCHO_PANTALLA / 2;
  int cy = ALTO_PANTALLA / 2;

  for (int r = radioMax; r <= radioMax + 40; r += 8) {
    display.clearDisplay();
    for (int angulo = 0; angulo < 360; angulo += 30) {
      float rad = angulo * 3.14159 / 180.0;
      int x2 = cx + r * cos(rad);
      int y2 = cy + r * sin(rad);
      display.drawLine(cx, cy, x2, y2, SSD1306_WHITE);
    }
    display.display();
    delay(100);
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  if (explotando) {
    delay(1000);       // pausa antes de reiniciar
    radio = 10;        // regresa la cara a su tamaño inicial
    explotando = false;
    return;
  }

  dibujarCara(radio);

  if (digitalRead(PIN_BOTON) == LOW) {
    radio++;
    if (radio >= radioMax) {
      efectoExplosion();
      explotando = true;
    }
    delay(40);
  }
}
