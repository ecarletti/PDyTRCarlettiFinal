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

  server.begin();  // Inicia el servidor después de conectarse a WiFi
  Serial.println("Servidor iniciado y esperando conexiones...");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Cliente conectado");
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
    }
    client.stop();
    Serial.println("Cliente desconectado");
  }
}
