#include "WiFi.h"
#include "Drone.h"
// #include <stdlib.h>
#include <esp_task_wdt.h>


WiFiServer server(80);
Drone d;

void sendCore(void *pvParameters);

// Configurações da rede WiFi do servidor (AP)
const char *ssid     = "ESP32_Server";
const char *password = "123456789";

// IP e porta do servidor
const char *host   = "192.168.4.1"; // IP do servidor ESP32
const uint8_t port = 80;

int pass = 0; //COLOCAR EM UM LOCAL MAIS ORGANIZADO

void setup() 
{
  d.MainControlSetup(115200, 14, 27, 26, 25, 33, 32, 17, 5);
  d.MPUconfigSetup();

  //FAZER O GERENCIMENTO DO WIFI COMO OUTRA BIBLIO MAIS ORGANIZADA
  WiFi.softAP(ssid, password);
  Serial.print("Connecting to WiFi");

  server.begin();
  Serial.println("Server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

//--------------------------------------------------------
  // Cria a tarefa 1 no núcleo 0
  xTaskCreatePinnedToCore(sendCore,     // Função da tarefa
                         "sendCore",    // Nome da tarefa
                          2048,        // Tamanho da pilha da tarefa
                          NULL,         // Parâmetro da tarefa
                          1,            // Prioridade da tarefa
                          NULL,         // Handle da tarefa
                          0);           // Núcleo onde a tarefa será executada

  delay(5000);

}

void sendCore(void *pvParameters){
  while(true){
    WiFiClient client = server.available();

    if(pass){ //O COMANDO SÓ VAI RODAR QUANDO TERMINAR UM LOOP PRINCIPAL
      if(client){
         d.sendData(client);
         break;
        pass = 0;
        delay(100);
      }else{
        delay(10);
      }
    }
  }
}

void loop() 
{
  d.MPUgetSignalsLoop();
  d.DisplayPlotterMpuData();
  
  pass = 1; 
}
