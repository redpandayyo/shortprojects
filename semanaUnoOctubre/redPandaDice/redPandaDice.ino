const int ledAzul = 4;
const int ledRojo = 5;
const int botonAzul = 18;  // corresponde al LED azul
const int botonRojo = 19;  // corresponde al LED rojo

int secuencia[100];
int longitudActual = 1;

void encenderLed(int numero, int duracion) {
  int pin = (numero == 0) ? ledAzul : ledRojo;
  digitalWrite(pin, HIGH);
  delay(duracion);
  digitalWrite(pin, LOW);
  delay(150);
}

void mostrarSecuencia() {
  for (int i = 0; i < longitudActual; i++) {
    encenderLed(secuencia[i], 400);
  }
}

// Espera a que el jugador presione un botón, da feedback encendiendo
// el LED correspondiente mientras lo mantiene presionado
int leerBoton() {
  while (true) {
    if (digitalRead(botonAzul) == LOW) {
      digitalWrite(ledAzul, HIGH);
      while (digitalRead(botonAzul) == LOW); // se mantiene encendido mientras lo presiona
      digitalWrite(ledAzul, LOW);
      return 0;
    }
    if (digitalRead(botonRojo) == LOW) {
      digitalWrite(ledRojo, HIGH);
      while (digitalRead(botonRojo) == LOW);
      digitalWrite(ledRojo, LOW);
      return 1;
    }
  }
}

void animacionAcierto() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(ledAzul, HIGH);
    digitalWrite(ledRojo, HIGH);
    delay(150);
    digitalWrite(ledAzul, LOW);
    digitalWrite(ledRojo, LOW);
    delay(150);
  }
}

void animacionFallo() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledRojo, HIGH);
    delay(200);
    digitalWrite(ledRojo, LOW);
    delay(200);
  }
}

void generarSecuencia() {
  longitudActual = 1;
  for (int i = 0; i < 100; i++) {
    secuencia[i] = random(0, 2);
  }
}

// Espera a que presionen el botón azul para arrancar, y hace el
// parpadeo de "inicio" con ambos LEDs
void esperarInicio() {
  while (digitalRead(botonAzul) == HIGH) {
    // no hace nada, solo espera
  }
  while (digitalRead(botonAzul) == LOW); // espera a que lo suelten

  for (int i = 0; i < 3; i++) {
    digitalWrite(ledAzul, HIGH);
    digitalWrite(ledRojo, HIGH);
    delay(200);
    digitalWrite(ledAzul, LOW);
    digitalWrite(ledRojo, LOW);
    delay(200);
  }

  generarSecuencia();
  delay(500);
}

void setup() {
  pinMode(ledAzul, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(botonAzul, INPUT_PULLUP);
  pinMode(botonRojo, INPUT_PULLUP);
  randomSeed(analogRead(34)); // pin sin nada conectado, para variar la semilla

  digitalWrite(ledAzul, LOW);
  digitalWrite(ledRojo, LOW);
}

void loop() {
  esperarInicio();

  bool jugando = true;
  while (jugando) {
    mostrarSecuencia();

    bool correcto = true;
    for (int i = 0; i < longitudActual; i++) {
      int entrada = leerBoton();
      if (entrada != secuencia[i]) {
        correcto = false;
        break;
      }
    }

    if (correcto) {
      animacionAcierto();
      longitudActual++;
      delay(800);
    } else {
      animacionFallo();
      jugando = false; // sale del juego y regresa a esperar el botón azul
    }
  }
}
