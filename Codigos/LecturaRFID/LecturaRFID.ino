#include <SoftwareSerial.h>

// Define los pines RX y TX para la comunicación serial virtual
SoftwareSerial RFIDserial(10, 11); // RX, TX

// Pines para LEDs rojo y azul
const int pinLedRojo = 13;  
const int pinLedAzul = 12; 

void setup() {
  Serial.begin(9600);       // Inicia la comunicación serial del Arduino Nano
  RFIDserial.begin(9600);   // Inicia la comunicación serial virtual

  // Configura los pines de los LEDs
  pinMode(pinLedRojo, OUTPUT);
  pinMode(pinLedAzul, OUTPUT);

  Serial.println("Listo para leer tarjetas RFID...");
  digitalWrite(pinLedRojo, HIGH);
}

void loop() {
  if (RFIDserial.available() > 0) {
    // Si hay datos disponibles en el módulo RFID
    char incomingChar = RFIDserial.read(); // Lee el carácter recibido

    if (incomingChar == 2) {
      // Si se detecta el inicio de la trama (ASCII 2), se inicia la lectura del ID del tag
      String tagID = "";

      while (incomingChar != 3) {
        // Lee los caracteres hasta que se detecte el fin de la trama (ASCII 3)
        incomingChar = RFIDserial.read();
        if ((incomingChar != 2) && (incomingChar != 3)) {
          tagID += incomingChar;
        }
      }

      // Imprime el ID del tag RFID en el Monitor Serial
      Serial.println("Tag RFID detectado: " + tagID);

      // Enciende el LED azul
      digitalWrite(pinLedAzul, HIGH);

      // Espera unos segundos
      delay(2000);

      // Apaga el LED azul
      digitalWrite(pinLedAzul, LOW);
    }
  }
}
