WiFiClient client;

const char* ssid = "Fibertel WiFi298 2.4GHz";
const char* password = "#";
const char* host = "192.168.0.143";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");

  Serial.print("Conectando a ");
  Serial.println(host);

  if (!client.connect(host, 80)) {
    Serial.println("Conexión fallida");
    return;
  }

  Serial.println("Conectado al servidor");
  client.println("Hola desde el cliente ESP32");
}

void loop() {
  if (!client.connected()) {
    client.stop();
  }
}