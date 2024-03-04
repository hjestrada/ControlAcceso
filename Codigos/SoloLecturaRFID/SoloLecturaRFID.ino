#include <SoftwareSerial.h>
#include <EEPROM.h>

// Define los pines RX y TX para la comunicación serial virtual
SoftwareSerial RFIDserial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial con la computadora
  RFIDserial.begin(9600); // Inicializa la comunicación serial virtual para el lector RFID
}

void loop() {
  if (RFIDserial.available() > 0) {
    // Si hay datos disponibles desde el lector RFID
    char incomingChar = RFIDserial.read(); // Lee el carácter recibido

    // Puedes realizar acciones basadas en el carácter recibido aquí
    // Por ejemplo, imprimirlo en el puerto serial de la computadora
    Serial.print("Carácter RFID: ");
    Serial.println(incomingChar);

    // También puedes almacenar el carácter en la EEPROM si es necesario
    // EEPROM.write(dirección, incomingChar);
  }
}
