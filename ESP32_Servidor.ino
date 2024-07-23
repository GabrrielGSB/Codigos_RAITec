#include <WiFi.h>
// Porta do servidor
WiFiServer server(80);

void LoopCore0(void *pvParameters);

String mensagem;

// Configurações do ponto de acesso (AP)
const char *ssid     = "ESP32_Server";
const char *password = "123456789";

void setup() 
{
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

  xTaskCreatePinnedToCore(LoopCore0,    // Função da tarefa
                         "LoopCore0",   // Nome da tarefa
                          10000,        // Tamanho da pilha da tarefa
                          NULL,         // Parâmetro da tarefa
                          1,            // Prioridade da tarefa
                          NULL,         // Handle da tarefa
                          0);           // Núcleo onde a tarefa será executada
                        
}

void LoopCore0(void *pvParameters)
{
  while(1)
  {
    if (Serial.available() > 0)
    {
      controlString = Serial.readStringUntil('\n');
      Serial.println(controlString);
    }
    
    vTaskDelay(250 / portTICK_PERIOD_MS);
  }
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
