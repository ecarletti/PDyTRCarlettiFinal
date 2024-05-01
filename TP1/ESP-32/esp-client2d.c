#include <WiFi.h>
WiFiClient client;

const char* ssid = "Fibertel WiFi298 2.4GHz";
const char* password = "#"; 
const char* host = "192.168.0.143";
int port = 80;
unsigned long bufferSizes[] = {1000, 10000, 100000, 1000000};
int testIndex = 0;

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
  if (testIndex < 4) {
    if (client.connect(host, port)) {
      Serial.print("Conectado al servidor, enviando ");
      Serial.print(bufferSizes[testIndex]);
      Serial.println(" bytes.");

      char *buffer = new char[bufferSizes[testIndex]];
      memset(buffer, 'A', bufferSizes[testIndex]);

      unsigned long startTime = millis();
      client.write((const uint8_t*)buffer, bufferSizes[testIndex]);
      unsigned long endTime = millis();

      delete[] buffer; // Libera la memoria después de enviar
      client.stop();
      
      Serial.print("Datos enviados y conexión cerrada. Tiempo de transmisión: ");
      Serial.print(endTime - startTime);
      Serial.println(" ms.");
      
      testIndex++;
    } else {
      Serial.println("Conexión fallida");
    }
    delay(10000); // Espera 10 segundos antes de la siguiente prueba
  }
  else {
    Serial.println("Ya se enviaron todos los datos!");
    while(1); //  loop cuando todas las pruebas estén completas
  }
}