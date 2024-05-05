#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <WiFiClient.h>
#include <WiFiServer.h>


const char* ssid = "Fibertel WiFi298 2.4GHz";
const char* password = "#";
WiFiServer server(21);

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
    Serial.printf("Listando: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
        Serial.println("Error al abrir el directorio");
        return;
    }
    if (!root.isDirectory()) {
        Serial.println("No es un directorio");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels) {
                listDir(fs, file.name(), levels - 1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void setup() {
  Serial.begin(115200);
  //if (!SPIFFS.begin(true)) {
    //Serial.println("An error has occurred while mounting SPIFFS");
    //return;
  //}
  //listDir(SPIFFS, "/", 0);  // Listar directorio


  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("WiFi conectado.");
  server.begin();
  Serial.println("Servidor FTP iniciado en puerto 21");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        String command = client.readStringUntil('\n');
        //client.print("comando recibido loop: ");
        //client.println(command);
        handleCommand(command, client);
      }
    }
    client.stop();
  }
}

void handleLsCommand(WiFiClient& client) {
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            client.print("DIR : ");
        } else {
            client.print("FILE: ");
        }
        client.println(file.name());
        file = root.openNextFile();
    }
}

void handleGetCommand(const String& command, WiFiClient& client) {
    String filename = command.substring(4);
    File file = SPIFFS.open(filename, "r");
    if (!file) {
        client.println("Archivo no encontrado");
    } else {
        while (file.available()) {
            client.write(file.read());
        }
        file.close();
    }
}

void handleDelCommand(const String& command, WiFiClient& client) {
    String filename = command.substring(4);
    if (SPIFFS.remove(filename)) {
        client.println("Archivo eliminado");
    } else {
        client.println("Fallo al eliminar");
    }
}

void handleCreateCommand(const String& command, WiFiClient& client) {
    int firstSpace = command.indexOf(' ');
    String filename = command.substring(firstSpace + 1);
    File file = SPIFFS.open(filename, FILE_WRITE);
    if (!file) {
        client.println("Error al crear el archivo");
        return;
    }
    file.print("Nuevo archivo");
    file.close();
    client.println("Archivo creado");
}


void handleCommand(String command, WiFiClient& client) {
    client.print("Command recibido: ");
    client.println(command);

    if (command.startsWith("ls")) {
        handleLsCommand(client);
    } else if (command.startsWith("get ")) {
        handleGetCommand(command, client);
    } else if (command.startsWith("del ")) {
        handleDelCommand(command, client);
    } else if (command.startsWith("create ")) {
        handleCreateCommand(command, client);
    }  else {
        client.println("Command no reconocido");
    }
}