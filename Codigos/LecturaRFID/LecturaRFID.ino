#include <SoftwareSerial.h>
#include <EEPROM.h>

// Define los pines RX y TX para la comunicación serial virtual
SoftwareSerial RFIDserial(11, 10); // RX, TX

// Pines para LEDs rojo y azul
const int pinLedRojo = 13;
const int pinLedAzul = 12;
const int pinSalidaD4 = 4; // Agregado el pin D4 para la salida

// Tamaño máximo de la EEPROM para almacenar códigos RFID
const int EEPROM_SIZE = 512;

void setup() {
  Serial.begin(9600);       // Inicia la comunicación serial del Arduino Nano
  RFIDserial.begin(9600);   // Inicia la comunicación serial virtual

  // Configura los pines de los LEDs y la salida D4
  pinMode(pinLedRojo, OUTPUT);
  pinMode(pinLedAzul, OUTPUT);
  pinMode(pinSalidaD4, OUTPUT);

  Serial.println("Listo para leer tarjetas RFID...");

  // Almacena manualmente códigos en la EEPROM
  almacenarEnEEPROM("0011223344");
  almacenarEnEEPROM("AABBCCDDEE");
  almacenarEnEEPROM("FFEEDDCCBB");

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

      // Compara el ID del tag con los almacenados en la EEPROM
      if (compararConEEPROM(tagID)) {
        // Si coincide, activa la salida en el pin D4
        digitalWrite(pinSalidaD4, LOW);

        // Enciende el LED azul
        digitalWrite(pinLedAzul, HIGH);

        // Espera unos segundos
        delay(2000);

        // Apaga el LED azul y la salida D4
        digitalWrite(pinLedAzul, LOW);
        digitalWrite(pinSalidaD4, HIGH);
      }
    }
  }
}

// Función para comparar el ID del tag con los almacenados en la EEPROM
bool compararConEEPROM(String tagID) {
  for (int i = 0; i < EEPROM.length(); i++) {
    if (tagID == leerEEPROM(i)) {
      return true; // Coincide con un código almacenado
    }
  }
  return false; // No coincide con ningún código almacenado
}

// Función para leer un código RFID almacenado en la EEPROM
String leerEEPROM(int address) {
  String tagID = "";
  char character = EEPROM.read(address);
  while (character != '\0') {
    tagID += character;
    address++;
    character = EEPROM.read(address);
  }
  return tagID;
}

// Función para almacenar un código RFID en la EEPROM
void almacenarEnEEPROM(String tagID) {
  int address = 0;

  // Busca la primera posición vacía en la EEPROM (carácter nulo '\0')
  while (EEPROM.read(address) != '\0' && address < EEPROM_SIZE) {
    address++;
  }

  // Almacena el tagID en la EEPROM
  for (int i = 0; i < tagID.length(); i++) {
    EEPROM.write(address + i, tagID[i]);
  }

  // Agrega el carácter nulo al final del tagID en la EEPROM
  EEPROM.write(address + tagID.length(), '\0');
  //Serial.println("Tag RFID almacenado en EEPROM: " + tagID);
}
