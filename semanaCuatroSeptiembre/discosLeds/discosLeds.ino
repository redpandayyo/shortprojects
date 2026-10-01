const int leds[] = {4, 5, 13, 14, 18, 19, 23, 25};
const int numLeds = 8;

int tiempoPaso = 150;        // velocidad del barrido
int tiempoEntreRondas = 300; // pausa al bloquear cada LED
int pausaEntrePatrones = 800;

void apagarTodos() {
  for (int i = 0; i < numLeds; i++) digitalWrite(leds[i], LOW);
}

void patronAcumulativo() {
  bool bloqueado[numLeds] = {false};

  for (int ronda = 0; ronda < numLeds; ronda++) {
    int finRonda = numLeds - 1 - ronda;

    for (int i = 0; i <= finRonda; i++) {
      if (!bloqueado[i]) digitalWrite(leds[i], HIGH);
      delay(tiempoPaso);
      if (!bloqueado[i]) digitalWrite(leds[i], LOW);
    }

    bloqueado[finRonda] = true;
    digitalWrite(leds[finRonda], HIGH); // se queda encendido
    delay(tiempoEntreRondas);
  }

  delay(pausaEntrePatrones);
  apagarTodos();
}

void patronOndaCentro() {
  int mitad = numLeds / 2;
  apagarTodos();

  for (int offset = 0; offset < mitad; offset++) {
    apagarTodos();
    int izq = mitad - 1 - offset;
    int der = mitad + offset;
    digitalWrite(leds[izq], HIGH);
    digitalWrite(leds[der], HIGH);
    delay(tiempoPaso * 2);
  }

  delay(pausaEntrePatrones);
  apagarTodos();
}

void setup() {
  for (int i = 0; i < numLeds; i++) pinMode(leds[i], OUTPUT);
  apagarTodos();
}

void loop() {
  patronAcumulativo();
  patronOndaCentro();
}
