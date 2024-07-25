#include <WiFi.h>

// Configurações do ponto de acesso (AP)
const char *ssid = "ESP32_Server";
const char *password = "123456789";

float PID[9]     = {2,0,0, 0.6,3.5,0.03, 1,6,0},
      PIDcopy[9] = {2,0,0, 0.6,3.5,0.03, 1,6,0};
size_t pidSize = sizeof(PID)/sizeof(float);

int    adress, indexInDrone;
float  mod,    modInDrone;
String cmd,    valorPID = "";


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
  delay(500);
  cmd = "";
  // Serial.println();
  WiFiClient client = server.available();

  if (client) 
  {
    while (client.connected()) 
    {
      prepararComando(client);
      client.println(cmd);
      break;
    }
    client.stop();
  }
}

void prepararComando(WiFiClient client)
{
  if(Serial.available())
  {
    cmd = Serial.readString();
    
    adress = (cmd[0] - '0') -1;
    mod = atof(&(cmd[2]));

    Serial.printf("\nO comando chamado é: %s\n",cmd);

    PID[adress] += mod;
    if (cmd == "r\n") copiarLista(PIDcopy, PID, 9);
    for (int i = 0 ; i < pidSize ; i++)
    {
      Serial.printf("%0.2f \n", PID[i]);
    }
  }
}

void copiarLista(float* fonte, float* copia, int tamanhoLista) 
{
  for (int i = 0; i < tamanhoLista; i++) copia[i] = fonte[i];
}
