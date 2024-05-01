#include <WiFi.h>
WiFiClient client;

const char* ssid = "Fibertel WiFi298 2.4GHz";
const char* password = "#";
const char* host = "192.168.0.143";
const int port = 80;
const int bufferSize = 1000; // Tamaño del buffer

unsigned long djb2(unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

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
if (client.connect(host, port)) {
    Serial.println("Generando buffer ...");
    char buffer[bufferSize];
    memset(buffer, 'e', bufferSize - 1);
    buffer[bufferSize - 1] = '\0';

    // Calcula el hash y envía datos
    unsigned long buffer_hash = djb2((unsigned char*)buffer);
    size_t data_length = strlen(buffer);
    Serial.printf("Buffer hash: %d", buffer_hash);
    Serial.printf(" - Data Length: %d", data_length);
    
    client.write((const uint8_t*)buffer, data_length);
    client.write((const uint8_t*)&data_length, sizeof(data_length));
    client.write((const uint8_t*)&buffer_hash, sizeof(buffer_hash));
    delay(1000); // Se agrego para esperar repuesta del server
    // Recibe confirmación del servidor
    if (client.available()) {
      char serverResponse[30];
      int n = client.readBytes(serverResponse, sizeof(serverResponse) - 1);
      serverResponse[n] = '\0';
      //Serial.println("serverResponse");
      Serial.println(serverResponse);
    }

    client.stop();
  } else {
    Serial.println("Conexión fallida");
  }
  delay(10000); // Espera antes de la próxima conexión
}