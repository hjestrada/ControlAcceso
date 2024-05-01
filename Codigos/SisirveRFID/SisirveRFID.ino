#include <SoftwareSerial.h>

const int BUFFER_SIZE = 14;
const int DATA_TAG_SIZE = 8;

SoftwareSerial ssrfid(11, 10);

uint8_t buffer[BUFFER_SIZE];
int buffer_index = 0;

const long TagsAlmacenados[] = {3856164,6287263,6494543,6236645,3862252,3861665, 11463764, 6251477, 11482160, 6250364, 6289464, 6236167, 3233189, 3859906, 3856153, 3928577, 6494497, 6469736, 11256247, 6244441};
long tagsDetectados[10];
int tagsDetectadosIndex = 0;

// Salida de potencia.
int SalidaLedAzul = 12;
int SalidaLedRojo = 13;
int SalidaRele = 4;
long tag;

void setup() {
  ssrfid.begin(9600);
  ssrfid.listen();
  pinMode(SalidaRele, OUTPUT);
  pinMode(SalidaLedAzul, OUTPUT);
  pinMode(SalidaLedRojo, OUTPUT);
  Serial.println("INICIALIZACIÓN COMPLETADA");
  digitalWrite(SalidaRele, HIGH);
}

void loop() {
  if (ssrfid.available() > 0) {
    bool call_extract_tag = false;

    int ssvalue = ssrfid.read();
    if (ssvalue == -1) {
      return;
    }

    if (ssvalue == 2) {
      buffer_index = 0;
    } else if (ssvalue == 3) {
      call_extract_tag = true;
    }

    if (buffer_index >= BUFFER_SIZE) {
      Serial.println("Error: ¡Desbordamiento de búfer detectado!");
      return;
    }

    buffer[buffer_index++] = ssvalue;

    if (call_extract_tag) {
      if (buffer_index == BUFFER_SIZE) {
        tag = extract_tag();

        if (!tag_detected(tag)) {
          tagsDetectados[tagsDetectadosIndex++] = tag;

          for (int i = 0; i < sizeof(TagsAlmacenados) / sizeof(TagsAlmacenados[0]); i++) {
            if (tag == TagsAlmacenados[i]) {
              activar_salida();
              break;
            }
          }
        } else {
          Serial.println("Error: Tag duplicado detectado.");
          forceReset();
        }

        buffer_index = 0;
      } else {
        buffer_index = 0;
        return;
      }
    }
  }
}

long extract_tag() {
  uint8_t *msg_data_tag = buffer + 3;
  long tag = hexstr_to_value(msg_data_tag, DATA_TAG_SIZE);
  Serial.print("Etiqueta extraída: ");
  Serial.println(tag);
  return tag;
}

long hexstr_to_value(uint8_t *str, unsigned int length) {
  char copy[length + 1];
  memcpy(copy, (char *)str, length);
  copy[length] = '\0';

  long value = strtol(copy, NULL, 16);

  return value;
}

bool tag_detected(long current_tag) {
  for (int i = 0; i < tagsDetectadosIndex; i++) {
    if (current_tag == tagsDetectados[i]) {
      return true;
    }
  }
  return false;
}

void activar_salida() {
  digitalWrite(SalidaRele, LOW);
  digitalWrite(SalidaLedAzul, HIGH);
  Serial.println("Abriendo Puerta");
  delay(10000);
  digitalWrite(SalidaRele, HIGH);
  digitalWrite(SalidaLedAzul, LOW);
  Serial.println("Cerrando Puerta");
}

void forceReset() {
  asm volatile ("  jmp 0");
}
