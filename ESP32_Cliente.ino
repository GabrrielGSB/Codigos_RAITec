#include <WiFi.h>

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

WiFiClient client;

void setup() {
  Serial.begin(115200);
  while(!Serial){}
  // Conecta-se à rede WiFi do servidor
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected");
}

void loop() 
{
  delay(500);
  cmd = "";
  // Serial.println();
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

  adress = (cmd[0] - '0') -1;
  mod = atof(&(cmd[2]));

  PID[adress] += mod;
  if (cmd == "r") copiarLista(PIDcopy, PID, 9);
  for (int i = 0 ; i < pidSize ; i++) Serial.printf("%0.2f \n", PID[i]);
}

void copiarLista(float* fonte, float* copia, int tamanhoLista) 
{
  for (int i = 0; i < tamanhoLista; i++) copia[i] = fonte[i];
}


