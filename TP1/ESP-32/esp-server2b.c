#include <WiFi.h>
WiFiServer server(80);

const char* ssid = "Fibertel WiFi298 2.4GHz";
const char* password = "#";

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
    Serial.println("Cliente conectado");
    while (client.connected()) {
      while (client.available()) {
        char buf[1000]; // Tamaño de buffer de lectura
        size_t len = client.read((uint8_t*)buf, sizeof(buf));
        if (len > 0) {
          Serial.print("Recibidos ");
          Serial.print(len);
          Serial.println(" bytes.");
        }
      }
    }
    client.stop();
    Serial.println("Cliente desconectado");
  }
}

/* Log del print de 1000 bytes y 10000:
Conectando a WiFi...
Conectado a WiFi
IP Address: 192.168.0.143
Servidor iniciado y esperando conexiones...
Cliente conectado
Recibidos 1000 bytes.
Cliente desconectado
Cliente conectado
Recibidos 1000 bytes.
Recibidos 1000 bytes.
Recibidos 872 bytes.
Recibidos 1000 bytes.
Recibidos 436 bytes.
Recibidos 1000 bytes.
Recibidos 436 bytes.
Recibidos 1000 bytes.
Recibidos 1000 bytes.
Recibidos 872 bytes.
Recibidos 1000 bytes.
Recibidos 384 bytes.
Cliente desconectado
Cliente conectado
*/

