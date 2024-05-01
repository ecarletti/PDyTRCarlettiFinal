#include <WiFi.h>
WiFiServer server(80);

const char* ssid = "Fibertel WiFi298 2.4GHz";
const char* password = "#";
unsigned long djb2(unsigned char *str);

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
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Servidor iniciado y esperando conexiones...");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    char buffer[1000];
    size_t n = client.readBytesUntil('\0', buffer, sizeof(buffer) - 1);
    buffer[n] = '\0';

    size_t data_length;
    client.readBytes((char*)&data_length, sizeof(data_length));

    unsigned long buffer_hash;
    client.readBytes((char*)&buffer_hash, sizeof(buffer_hash));

    unsigned long calculated_hash = djb2((unsigned char*)buffer);
    Serial.printf("Buffer hash (calculado): %d", calculated_hash);
    Serial.printf(" - Data Length (n calculado): %d", n);
    
    if (calculated_hash == buffer_hash && data_length == n) {
      client.println("Datos recibidos correctamente.");
    } else {
      client.println("Error en los datos recibidos.");
    }
    client.stop();
  }
}


/* Servidor iniciado y esperando conexiones...
Buffer hash (calculado): 1306728104 - Data Length (n calculado): 999 */