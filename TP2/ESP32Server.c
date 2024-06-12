#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>

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

  if (!SPIFFS.begin(true)) {
    Serial.println("Error al montar SPIFFS");
    return;
  }

  // Crear un archivo de prueba
  File file = SPIFFS.open("/serverFile.txt", FILE_WRITE);
  if (!file) {
    Serial.println("Error al crear archivo de prueba");
    return;
  }
  file.println("Este es un archivo de prueba para verificar la lectura desde el ESP32. Se agrega mas descripcion para agregar mayor longitud de bytes al file.");
  file.close();
  Serial.println("Archivo de prueba creado");

  server.begin();
  Serial.println("Servidor iniciado y esperando conexiones...");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Cliente conectado");
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\n');
        Serial.println("Solicitud recibida: " + request);

        if (request.startsWith("READ")) {
          Serial.println("Solicitud READ");
          handleRead(client, request);
        } else if (request.startsWith("WRITE")) {
          Serial.println("Solicitud WRITE");
          handleWrite(client, request);
        }
      }
    }
    client.stop();
    Serial.println("Cliente desconectado");
  }
}

void handleRead(WiFiClient &client, String request) {
  int idx1 = request.indexOf(' ') + 1;
  int idx2 = request.indexOf(' ', idx1);
  int idx3 = request.indexOf(' ', idx2 + 1);

  Serial.println("idx1: " + String(idx1));
  Serial.println("idx2: " + String(idx2));
  Serial.println("idx3: " + String(idx3));

  if (idx1 == 0 || idx2 == -1 || idx3 == -1) {
    Serial.println("Error: índices no encontrados en la solicitud");
    client.println("ERROR: Invalid request format");
    return;
  }

  String fileName = request.substring(idx1, idx2);
  int position = request.substring(idx2 + 1, idx3).toInt();
  int length = request.substring(idx3 + 1).toInt();

  Serial.println("fileName: " + fileName);
  Serial.println("position: " + String(position));
  Serial.println("length: " + String(length));

  File file = SPIFFS.open("/" + fileName, FILE_READ);
  if (!file) {
    Serial.println("ERROR: File not found");
    client.println("ERROR: File not found");
    return;
  }

  file.seek(position);
  char buffer[length];
  int bytesRead = file.readBytes(buffer, length);

  client.write(buffer, bytesRead);
  file.close();
  client.printf("\n%d\n", bytesRead);
  Serial.printf("\nBytes leídos: %d\n", bytesRead);
}

void handleWrite(WiFiClient &client, String request) {
  int idx1 = request.indexOf(' ') + 1;
  int idx2 = request.indexOf(' ', idx1);

  String fileName = request.substring(idx1, idx2);
  int length = request.substring(idx2 + 1).toInt();

  Serial.println("fileName: " + fileName);
  Serial.println("length: " + String(length));

  File file = SPIFFS.open("/" + fileName, FILE_APPEND);
  if (!file) {
    file = SPIFFS.open("/" + fileName, FILE_WRITE);
  }

  if (!file) {
    Serial.println("ERROR: Unable to open file for writing");
    client.println("ERROR: Unable to open file for writing");
    return;
  }

  char buffer[length];
  int bytesReceived = client.readBytes(buffer, length);
  int bytesWritten = file.write((uint8_t*)buffer, bytesReceived);

  file.close();
  client.printf("\n%d\n", bytesWritten);
  Serial.printf("\nBytes escritos: %d\n", bytesWritten);
}
