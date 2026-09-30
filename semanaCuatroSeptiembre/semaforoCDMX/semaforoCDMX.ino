#define PIN_ROJO 15
#define PIN_AMARILLO 2
#define PIN_VERDE 18

void apagarTodos() {
  digitalWrite(PIN_ROJO, LOW);
  digitalWrite(PIN_AMARILLO, LOW);
  digitalWrite(PIN_VERDE, LOW);
}

void setup() {
  pinMode(PIN_ROJO, OUTPUT);
  pinMode(PIN_AMARILLO, OUTPUT);
  pinMode(PIN_VERDE, OUTPUT);
  apagarTodos();
}

void cicloNormal() {
  apagarTodos();
  digitalWrite(PIN_VERDE, HIGH);
  delay(2000);

  apagarTodos();
  digitalWrite(PIN_AMARILLO, HIGH);
  delay(800);

  apagarTodos();
  digitalWrite(PIN_ROJO, HIGH);
  delay(2000);
}

void oscilacionRojoAmarillo() {
  for (int i = 0; i < 6; i++) {
    apagarTodos();
    digitalWrite(PIN_ROJO, HIGH);
    delay(400);
    apagarTodos();
    digitalWrite(PIN_AMARILLO, HIGH);
    delay(400);
  }
}

int ciclosCompletados = 0;

void loop() {
  cicloNormal();
  ciclosCompletados++;

  if (ciclosCompletados % 2 == 0) {
    oscilacionRojoAmarillo();
  }
}
