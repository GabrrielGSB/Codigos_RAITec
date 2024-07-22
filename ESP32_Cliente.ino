#include <WiFi.h>

// Configurações da rede WiFi do servidor (AP)
const char *ssid = "ESP32_Server";
const char *password = "123456789";

// IP e porta do servidor
const char *host = "192.168.4.1"; // IP do servidor ESP32
const uint16_t port = 80;

WiFiClient client;

void setup() {
  Serial.begin(115200);

  delay(3000);
  // Conecta-se à rede WiFi do servidor
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected");

  // Conecta-se ao servidor
  if (!client.connect(host, port)) {
    Serial.println("Connection to server failed");
    return;
  }

  // Envia uma mensagem para o servidor
  client.println("Primeira conexão com o servidor");

  // Recebe a resposta do servidor
  while (client.connected()) {
    if (client.available()) {
      String response = client.readStringUntil('\n');
      Serial.println("Server response: " + response);
      break;
    }
  }

  client.stop();
}

void loop() 
{
  delay(5000);
  if (!client.connect(host, port)) {
    Serial.println("Connection to server failed");
    return;
  }

  // Envia uma mensagem para o servidor
  client.println("Conexões REPETIDAS COM O SERVIDOR");

  // Recebe a resposta do servidor
  while (client.connected()) {
    if (client.available()) {
      String response = client.readStringUntil('\n');
      Serial.println("Server response: " + response);
      break;
    }
  }

  client.stop();
}

