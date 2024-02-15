#include <SoftwareSerial.h>

// Configuración del módulo RFID
#define RFID_RX_PIN 10  // pin 10 de Arduino
#define RFID_TX_PIN 11  // pin 11 de Arduino
SoftwareSerial RFIDSerial(RFID_RX_PIN, RFID_TX_PIN);  // Configuración de la comunicación serial

void setup() {
  Serial.begin(9600);
  RFIDSerial.begin(9600);
}

void loop() {
  if (RFIDSerial.available() > 0) {
    char receivedChar = RFIDSerial.read();
    
    // Comprobar si se ha recibido un inicio de trama
    if (receivedChar == 2) {
      String cardNumber = readCardData();
      Serial.println("Tarjeta RFID detectada. Número: " + cardNumber);
    }
  }
}

String readCardData() {
  String cardData = "";
  
  while (RFIDSerial.available() > 0) {
    char dataChar = RFIDSerial.read();
    
    // Comprobar si se ha recibido un fin de trama
    if (dataChar == 3) {
      break;
    } else {
      cardData += dataChar;
    }
  }
  
  return cardData;
}
