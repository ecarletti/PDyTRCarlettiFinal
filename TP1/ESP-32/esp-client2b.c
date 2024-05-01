#include <WiFi.h>
WiFiClient client;

const char* ssid = "Fibertel WiFi298 2.4GHz";
const char* password = "#";
const char* host = "192.168.0.143";
int port = 80;
unsigned long bufferSizes[] = {1000, 10000, 100000, 1000000};
int testIndex = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a WiFi");
}

void loop() {
  if (testIndex < 4) {
    if (client.connect(host, port)) {
      Serial.print("Conectado al servidor, enviando ");
      Serial.print(bufferSizes[testIndex]);
      Serial.println(" bytes.");

      // Envía el buffer de prueba
      char *buffer = new char[bufferSizes[testIndex]];
      memset(buffer, 'A', bufferSizes[testIndex]);
      client.write((const uint8_t*)buffer, bufferSizes[testIndex]);
      delete[] buffer; // Libera la memoria después de enviar
      client.stop();
      
      Serial.println("Datos enviados y conexión cerrada.");
      testIndex++;
    } else {
      Serial.println("Conexión fallida");
    }
    delay(10000); // Espera 10 segundos antes de la siguiente prueba
  }
  else
  {
    Serial.println("Ya se enviaron todos los datos!");
  }
}

/*
Log Print:
Conectado al servidor, enviando 1000 bytes.
Datos enviados y conexión cerrada.
Conectado al servidor, enviando 10000 bytes.
Datos enviados y conexión cerrada.
Conectado al servidor, enviando 100000 bytes.
Datos enviados y conexión cerrada.
Conectado al servidor, enviando 1000000 bytes.

abort() was called at PC 0x4013801b on core 1


Backtrace: 0x4008359d:0x3ffca710 0x4008b689:0x3ffca730
 0x40090a01:0x3ffca750 0x4013801b:0x3ffca7d0 
 0x40138062:0x3ffca7f0 0x40137fc3:0x3ffca810 
 0x40138316:0x3ffca830 0x40138b31:0x3ffca850 0x400d27cf:0x3ffca870 0x400d52cd:0x3ffca890

ELF file SHA256: 199d0cebc2fbe35a
*/