#include <WiFi.h>
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

  // Crear directorio y archivos de prueba
  createTestFiles();

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
          handleRead(client, request);
        } else if (request.startsWith("WRITE")) {
          handleWrite(client, request);
        } else if (request.startsWith("LIST")) {
          handleList(client, request);
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

  String fileName = request.substring(idx1, idx2);
  int position = request.substring(idx2 + 1, idx3).toInt();
  int length = request.substring(idx3 + 1).toInt();

  File file = SPIFFS.open("/" + fileName, FILE_READ);
  if (!file) {
    client.println("ERROR: File not found");
    return;
  }

  file.seek(position);
  char buffer[length];
  int bytesRead = file.readBytes(buffer, length);

  client.write(buffer, bytesRead);
  file.close();
  client.printf("\n%d\n", bytesRead);
}

void handleWrite(WiFiClient &client, String request) {
  int idx1 = request.indexOf(' ') + 1;
  int idx2 = request.indexOf(' ', idx1);

  String fileName = request.substring(idx1, idx2);
  int length = request.substring(idx2 + 1).toInt();

  File file = SPIFFS.open("/" + fileName, FILE_APPEND);
  if (!file) {
    file = SPIFFS.open("/" + fileName, FILE_WRITE);
  }

  if (!file) {
    client.println("ERROR: Unable to open file for writing");
    return;
  }

  char buffer[length];
  int bytesReceived = client.readBytes(buffer, length);
  int bytesWritten = file.write((uint8_t*)buffer, bytesReceived);

  file.close();
  client.printf("\n%d\n", bytesWritten);
}

void handleList(WiFiClient &client, String request) {
  //int idx1 = request.indexOf(' ') + 1;
  //String path = request.substring(idx1);
  //Serial.println("idx1: " + String(idx1));
  //Serial.println("path: " + String(path));

  File root = SPIFFS.open("/");
  Serial.println("root: " + String(root));
  if (!root) {
    client.println("ERROR: Directory not found");
    return;
  }
  Serial.println("root.isDirectory: " + String(root.isDirectory()));
  if (!root.isDirectory()) {
    client.println("ERROR: Not a directory");
    return;
  }

  File file = root.openNextFile();
  String fileList = "";
  while (file) {
    // Serial.println("archivo " + String(file.name()));
    fileList += String(file.name()) + "\n";
    file = root.openNextFile();
  }
  client.print(fileList);
  Serial.println(fileList);
}


void createFile(const char* path, const char* content) {
  File file = SPIFFS.open(path, FILE_WRITE);
  if (file) {
    file.println(content);
    file.close();
    Serial.printf("Archivo %s creado\n", path);
  } else {
    Serial.printf("Error al crear el archivo %s\n", path);
  }
}

void createTestFiles() {
  createFile("/serverFile.txt", "Este es un archivo de prueba para verificar la lectura desde el ESP32. Se agrega más descripción para agregar mayor longitud de bytes al archivo.");
  createFile("/test1.txt", "Este es el archivo de prueba 1.");
  createFile("/test2.txt", "Este es el archivo de prueba 2.");
}

