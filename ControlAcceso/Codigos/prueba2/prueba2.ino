#include <Arduino.h>
#include "wiring_private.h"

Uart mySerial (&sercom1, 10, 11, SERCOM_RX_PAD_1, UART_TX_PAD_2);

// Attach the interrupt handler to the SERCOM
void SERCOM1_Handler()
{
    mySerial.IrqHandler();
}

void setup() {
  // Reassign pins 13 and 8 to SERCOM (not alt this time)
  pinPeripheral(10, PIO_SERCOM);
  pinPeripheral(11, PIO_SERCOM);

  // Start my new hardware serial
  mySerial.begin(9600);
}

void loop() {
  // Do something with mySerial...
}
