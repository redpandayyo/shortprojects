#include <Arduino.h>
#include <TFT_eSPI.h>
#include "peanut_gb.h"
#include "rom.h"

// Instancia de la pantalla TFT
TFT_eSPI tft = TFT_eSPI();

// Pines para los 6 botones (ESP32 de 30 Pines)
#define PIN_UP    13
#define PIN_DOWN  12
#define PIN_LEFT  14
#define PIN_RIGHT 27
#define PIN_A     33
#define PIN_START 26

struct gb_s gb;

// Memoria RAM del cartucho (32 KB requeridos por Pokémon Rojo para descomprimir sprites)
#define CART_RAM_SIZE 32768
uint8_t cart_ram[CART_RAM_SIZE];

// Lectura de la ROM desde la memoria Flash
uint8_t gb_rom_read(struct gb_s *gb, const uint_fast32_t addr) {
    return pgm_read_byte(&(gb_rom[addr]));
}

// Lectura de la RAM del cartucho (donde Pokémon busca el sprite descomprimido)
uint8_t gb_cart_ram_read(struct gb_s *gb, const uint_fast32_t addr) {
    if (addr < CART_RAM_SIZE) {
        return cart_ram[addr];
    }
    return 0xFF;
}

// Escritura de la RAM del cartucho (donde el juego descomprime a Oak y los Pokémon)
void gb_cart_ram_write(struct gb_s *gb, const uint_fast32_t addr, const uint8_t val) {
    if (addr < CART_RAM_SIZE) {
        cart_ram[addr] = val;
    }
}

void gb_error(struct gb_s *gb, const enum gb_error_e gb_err, const uint16_t val) {
    Serial.printf("Error emulador: %d, val: %d\n", gb_err, val);
}

// Dibujado en pantalla ST7789 escalado a 240x216
void lcd_draw_line(struct gb_s *gb, const uint8_t pixels[160], const uint_fast8_t line) {
    static uint16_t line_buffer[240];

    // Paleta estándar de 4 tonos de Game Boy (0: Blanco, 1: Gris claro, 2: Gris oscuro, 3: Negro)
    const uint16_t palette[4] = {
        TFT_WHITE,
        0xC618, // Gris claro
        0x7BE0, // Gris oscuro
        TFT_BLACK
    };

    // Los dos bits menos significativos (pixels[x] & 0x03) contienen el color ya procesado por Peanut-GB
    int dst_x = 0;
    for (int x = 0; x < 160; x += 2) {
        uint16_t c0 = palette[pixels[x] & 0x03];
        uint16_t c1 = palette[pixels[x + 1] & 0x03];

        line_buffer[dst_x++] = c0;
        line_buffer[dst_x++] = c0;
        line_buffer[dst_x++] = c1;
    }

    // Centrado vertical: 216 px en pantalla de 240 px (12 px de margen superior e inferior)
    uint16_t start_y = 12 + (line * 3) / 2;

    // Enviar línea a la pantalla
    tft.pushImage(0, start_y, 240, 1, line_buffer);

    // Duplicar línea intercalada para mantener la proporción de escala 1.5x
    if ((line % 2 == 1) && (start_y + 1 < 228)) {
        tft.pushImage(0, start_y + 1, 240, 1, line_buffer);
    }
}

void setup() {
    Serial.begin(115200);

    // Limpiar el búfer de RAM del cartucho al inicio
    memset(cart_ram, 0x00, sizeof(cart_ram));

    // Configurar botones con resistencia Pull-Up interna
    pinMode(PIN_UP, INPUT_PULLUP);
    pinMode(PIN_DOWN, INPUT_PULLUP);
    pinMode(PIN_LEFT, INPUT_PULLUP);
    pinMode(PIN_RIGHT, INPUT_PULLUP);
    pinMode(PIN_A, INPUT_PULLUP);
    pinMode(PIN_START, INPUT_PULLUP);

    // Inicializar pantalla TFT
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);

    // Inicializar Peanut-GB
    enum gb_init_error_e ret = gb_init(&gb, gb_rom_read, gb_cart_ram_read, gb_cart_ram_write, gb_error, NULL);
    if (ret != GB_INIT_NO_ERROR) {
        Serial.printf("Error al iniciar Peanut-GB: %d\n", ret);
        while (1);
    }

    gb_init_lcd(&gb, lcd_draw_line);
    Serial.println("¡Emulador iniciado con RAM de descompresión activa!");
}

void loop() {
    // Lectura de los 6 botones
    gb.direct.joypad_bits.a      = digitalRead(PIN_A);
    gb.direct.joypad_bits.start  = digitalRead(PIN_START);
    gb.direct.joypad_bits.right  = digitalRead(PIN_RIGHT);
    gb.direct.joypad_bits.left   = digitalRead(PIN_LEFT);
    gb.direct.joypad_bits.up     = digitalRead(PIN_UP);
    gb.direct.joypad_bits.down   = digitalRead(PIN_DOWN);

    // Procesar fotograma
    gb_run_frame(&gb);
}
