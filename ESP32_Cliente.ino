
#include <WiFi.h>
#include "Drone.h"


WiFiClient client;
Drone d;

unsigned long previousMillis = 0;  // armazenará o último valor de millis()
const long interval = 2000;  
//
//void LoopCore0(void *pvParameters);

float PID[9]     = {2,0,0, 0.6,3.5,0.03, 1,6,0},
      PIDcopy[9] = {2,0,0, 0.6,3.5,0.03, 1,6,0},
      mod;

size_t pidSize = sizeof(PID)/sizeof(float);

int adress;

// Configurações da rede WiFi do servidor (AP)
const char *ssid = "ESP32_Server";
const char *password = "123456789";

// IP e porta do servidor
const char *host = "192.168.4.1"; // IP do servidor ESP32
const uint16_t port = 80;

String cmd;



void setup() 
{
  d.MainControlSetup(115200,32,33,25,26,19,18,5,17); 
//  Serial.begin(115200);
 
  while(!Serial){}
  // Conecta-se à rede WiFi do servidor
  WiFi.begin(ssid, password);
  
  Serial.print("Conectando ao WiFi");

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado!!!");

//  xTaskCreatePinnedToCore(LoopCore0,  // Função da tarefa
//                         "LoopCore0", // Nome da tarefa
//                          10000,      // Tamanho da pilha da tarefa
//                          NULL,       // Parâmetro da tarefa
//                          1,          // Prioridade da tarefa
//                          NULL,       // Handle da tarefa
//                          0);         // Núcleo onde a tarefa será executada
}

//void LoopCore0(void *pvParameters) 
//{
//  while(1)
//  {
//    vTaskDelay(500 / portTICK_PERIOD_MS);
//    cmd = "";
//    // Serial.println();
//    if (!client.connect(host, port)) 
//    {
//      Serial.println("Connection to server failed");
//      return;
//    }
//  
//    while (client.connected()) 
//    {
//      if (client.available()) 
//      {
//        cmd = client.readStringUntil('\n');
//        Serial.println("Mensagem do Servidor: " + cmd);
//        break;
//      }
//    }
//  
//    client.stop();
//    processarComando(cmd);
//  }
//}

void loop()
{
  unsigned long currentMillis = millis();
  cmd = "";
  d.MPUgetSignalsLoop();
  d.MainControlLoop();
//  d.DisplaySerialMpuData();

  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    if (!client.connect(host, port)) 
      {
        Serial.println("Connection to server failed");
        return;
      }
    
      while (client.connected()) 
      {
        if (client.available()) 
        {
          cmd = client.readStringUntil('\n');
          Serial.println("Mensagem do Servidor: " + cmd);
          break;
        }
      }
  
  
    client.stop();
    processarComando(cmd);
   }

}

void processarComando(String comando)
{
  adress = (cmd[0] - '0')-1;
  mod = atof(&(cmd[2]));

  PID[adress] += mod;
  if (adress == 65) copiarLista(PIDcopy, PID, 9);
  d.updatePID(adress, mod);

  for (int i = 0 ; i < pidSize ; i++) Serial.printf("%0.2f \n", PID[i]);    
}

void copiarLista(float* fonte, float* copia, int tamanhoLista) 
{
  for (int i = 0; i < tamanhoLista; i++) copia[i] = fonte[i];
}
