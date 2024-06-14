#include <WiFi.h>

const char* ssid = "Fibertel WiFi298 2.4GHz";
const char* password = "#";
WiFiServer server(80);

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
      if (client.available()) {
        handleTest(client);
      }
    }
    client.stop();
    Serial.println("Cliente desconectado");
  }
}

void handleTest(WiFiClient &client) {
  String request = client.readStringUntil('\n');
  int messageSize = request.toInt();
  
  char buffer[1024];
  int bytesReceived = 0;
  int totalBytesReceived = 0;

  while (totalBytesReceived < messageSize) {
    bytesReceived = client.readBytes(buffer, sizeof(buffer));
    totalBytesReceived += bytesReceived;
    Serial.printf("Recibidos %d bytes\n", bytesReceived);
  }

  for (int i = 0; i < totalBytesReceived; i += sizeof(buffer)) {
    int chunkSize = min((int)sizeof(buffer), totalBytesReceived - i);
    client.write(buffer, chunkSize);
    client.flush();
  }
  
  client.println(totalBytesReceived);
  Serial.printf("Total bytes recibidos: %d\n", totalBytesReceived);
}
