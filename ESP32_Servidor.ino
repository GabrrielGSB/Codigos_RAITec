#include <WiFi.h>

// Configurações do ponto de acesso (AP)
const char *ssid = "ESP32_Server";
const char *password = "123456789";

// Porta do servidor
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(3000);
 
  Serial.println("Access Trying to start");
  WiFi.softAP(ssid, password);
  Serial.println("Access point started");
  
  // Inicia o servidor
  server.begin();
  Serial.println("Server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  
}

void loop() 
{
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client Connected");
    while (client.connected()) {
      if (client.available()) {
        String msg = client.readStringUntil('\n');
        Serial.println("Received: " + msg);

        // Envia uma resposta para o cliente
        client.println("Message received");
      }
    }
    client.stop();
    Serial.println("Client Disconnected");
  }
  // else 
  // {
  //   while (client.connected() == false)
  //   {
  //     Serial.println("Esperando o cliente...");
  //     delay(10000);
  //   }
  // }
}
