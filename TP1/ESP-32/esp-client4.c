#include <WiFi.h>

const char* ssid = "Fibertel WiFi298 2.4GHz";
const char* password = "#";
const char* host = "192.168.0.143";
const int port = 21;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");
  connectToServer();
}

void connectToServer() {
  if (client.connect(host, port)) {
    Serial.println("Conectado al servidor");
    //sendCommand("ls");
  } else {
    Serial.println("Fallo al conectar al servidor");
  }
}

void loop() {
  if (client.connected() && client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    sendCommand(command);
  }
}

void sendCommand(String command) {
  client.println(command);
  Serial.print("Command enviado: ");
  Serial.println(command);
}
