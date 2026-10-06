const int calabaza1A = 4;
const int calabaza1B = 5;
const int calabaza2A = 18;
const int calabaza2B = 19;

int tiempoParpadeo = 300;
int tiempoEntrePasos = 150;
int pausaEntreRondas = 600;

void encenderCalabaza1(bool estado) {
  digitalWrite(calabaza1A, estado);
  digitalWrite(calabaza1B, estado);
}

void encenderCalabaza2(bool estado) {
  digitalWrite(calabaza2A, estado);
  digitalWrite(calabaza2B, estado);
}

void apagarTodas() {
  encenderCalabaza1(LOW);
  encenderCalabaza2(LOW);
}

void secuenciaAlternada() {
  for (int i = 0; i < 4; i++) {
    encenderCalabaza1(HIGH);
    delay(tiempoParpadeo);
    encenderCalabaza1(LOW);
    delay(tiempoEntrePasos);

    encenderCalabaza2(HIGH);
    delay(tiempoParpadeo);
    encenderCalabaza2(LOW);
    delay(tiempoEntrePasos);
  }
  delay(pausaEntreRondas);
}

void secuenciaJuntas() {
  for (int i = 0; i < 3; i++) {
    encenderCalabaza1(HIGH);
    encenderCalabaza2(HIGH);
    delay(tiempoParpadeo);
    apagarTodas();
    delay(tiempoEntrePasos);
  }
  delay(pausaEntreRondas);
}

void setup() {
  pinMode(calabaza1A, OUTPUT);
  pinMode(calabaza1B, OUTPUT);
  pinMode(calabaza2A, OUTPUT);
  pinMode(calabaza2B, OUTPUT);
  apagarTodas();
}

void loop() {
  secuenciaAlternada();
  secuenciaJuntas();
}
