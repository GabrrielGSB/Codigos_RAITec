#include "WiFi.h"
#include "Drone.h"
#include <stdlib.h>  // Inclui a biblioteca padrão para atof


WiFiClient client;
Drone d;

void LoopCore0(void *pvParameters);

// Configurações da rede WiFi do servidor (AP)
const char *ssid = "ESP32_Server";
const char *password = "123456789";

// IP e porta do servidor
const char *host = "192.168.4.1"; // IP do servidor ESP32
const uint16_t port = 80;

void setup() {
  d.MainControlSetup(115200,32,33,25,26,5,18,19,21); 
  while (!Serial){}

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  // --------------------------------------------
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado!!!");

  // Conecta-se ao servidor
  if (!client.connect(host, port)) 
  {
    Serial.println("Conexão com o Servidor Falhou ;---;");
    return;
  }

  // Envia uma mensagem para o servidor
  client.println("SE VC NÃO CONSEGUIR NINGUEM CONSEGUE :)");

  // Recebe a resposta do servidor
  while (client.connected()) 
  {
    if (client.available()) 
    {
      String response = client.readStringUntil('\n');
      Serial.println("Server response: " + response);
      break;
    }
  }

  client.stop();
//--------------------------------------------------------
  // Cria a tarefa 1 no núcleo 0
  xTaskCreatePinnedToCore(LoopCore0,    // Função da tarefa
                         "LoopCore0",   // Nome da tarefa
                          10000,        // Tamanho da pilha da tarefa
                          NULL,         // Parâmetro da tarefa
                          1,            // Prioridade da tarefa
                          NULL,         // Handle da tarefa
                          0);           // Núcleo onde a tarefa será executada

  delay(5000);

}

void loop() 
{
  d.MPUgetSignalsLoop();
  d.MainControlLoop();
//  delay(5000);
//  if (!client.connect(host, port)) {
//    Serial.println("Connection to server failed");
//    return;
//  }
//
//  // Envia uma mensagem para o servidor
//  client.println("Conexões REPETIDAS COM O SERVIDOR");
//
//  // Recebe a resposta do servidor
//  while (client.connected()) {
//    if (client.available()) {
//      String response = client.readStringUntil('\n');
//      Serial.println("Server response: " + response);
//      break;
//    }
//  }
//
//  client.stop();
}

void LoopCore0(void *pvParameters) 
{
  while(1)
  {
    Serial.println("Executando Tarefa 1 no núcleo 0");
    vTaskDelay(6000 / portTICK_PERIOD_MS); // Ag
  }
}
