#include <WiFi.h>
#include <stdlib.h>  // Necessário para usar a função atof()

// Porta do servidor
WiFiServer server(80);

void LoopCore0(void *pvParameters);

float PID[9]     = {2,0,0, 0.6,3.5,0.03, 1,6,0},
      PIDcopy[9] = {2,0,0, 0.6,3.5,0.03, 1,6,0};
size_t pidSize = sizeof(PID)/sizeof(float);

int    adress, indexInDrone;
float  mod,    modInDrone;
String cmd,    data,       valorPID = "";

// Configurações do ponto de acesso (AP)
const char *ssid     = "ESP32_Server",
           *password = "123456789";


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
    if(Serial.available())
    {
      cmd = Serial.readString();
      if (cmd == "reset\n") copiarLista(PIDcopy, PID, 9);
        
      adress = (cmd[0] - '0') -1;
      mod = atof(&(cmd[2]));

      Serial.printf("\nO comando chamado é: %s\n",cmd);

      PID[adress] += mod;
      for (int i = 0 ; i < pidSize ; i++)
      {
        Serial.printf("%0.2f \n", PID[i]);
      }
    }
    enviarDados();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    // if (Serial.available() > 0)
    // {
    //   controlString = Serial.readStringUntil('\n').c_str();

    //   p[0] = controlString[0];
    //   p[1] = controlString[1];

    //   String parametro(p);

    //   ajuste = atof(controlString+3);

    //   Serial.printf("O parâmetro a ser ajustado: %c/n", parametro);
    //   Serial.printf("O ajuste feito é: %f", ajuste);
    // }
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
}

void copiarLista(float* fonte, float* copia, int tamanhoLista) 
{
  for (int i = 0; i < tamanhoLista; i++) copia[i] = fonte[i];
}

void enviarDados()
{
  valorPID = "";
  for (int i = 0; i < 9; i++) valorPID += (String(PID[i]) + String(" "));  

  Serial.println(valorPID);
}


