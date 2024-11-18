#include "WiFi.h"

const char *ssid     = "ESP32_Server";
const char *password = "123456789";
const char *host     = "192.168.4.1"; 
const uint8_t port   =  80;

struct Data{
    int motor1, motor2, motor3, motor4, tempo;

    float roll, pitch, kRoll, kPitch;

  

  void setMotors(int m1, int m2, int m3, int m4){
    motor1 = m1;
    motor2 = m2;
    motor3 = m3;
    motor4 = m4;
  }

  void setAngle(float r, float p, float kR, float kP, int t){
    roll   = r;
    pitch  = p;
    kRoll  = kR;
    kPitch = kP;
    tempo   = t;
  }
  
};

WiFiClient client;

void setup() 
{
  Serial.begin(115200);
  conectarServidor();
}

void loop() 
{
  receberDados(100);
}

void conectarServidor()
{
  // Conectar ao Wi-Fi do servidor
  WiFi.begin(ssid, password);
  Serial.println("Conectando ao Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) 
  {
      delay(1000);
      Serial.print(".");
  }
  Serial.println("Conectado ao Wi-Fi");
}

void receberDados(uint16_t intervalo)
{
  if (client.connect(host, port)) 
  { 
    // Esperar receber os dados
    while (client.available() < sizeof(Data)){}
    
    // Ler dados binários
    Data dadosRecebidos;
    client.read((uint8_t*)&dadosRecebidos, sizeof(dadosRecebidos));
    
    // Exibir os dados recebidos
    //Serial.printf("AngleRoll: %.4f°, AnglePitch: %.4f°, Tempo: %lu\n",
                  
   
    Serial.printf("Motor1: %d, Motor2: %d, Motor3: %d, Motor4: %d, Angle_Roll: %f, Angle_Pitch: %f, Kalman_Angle_Roll: %f, Kalman_Angle_Pitch: %f, Tempo: %d \n",
                  dadosRecebidos.motor1, dadosRecebidos.motor2,
                  dadosRecebidos.motor3, dadosRecebidos.motor4,
                  dadosRecebidos.roll, dadosRecebidos.pitch, dadosRecebidos.kRoll, dadosRecebidos.kPitch, dadosRecebidos.tempo);
    
    delay(intervalo); // Aguarde antes de tentar novamente
  }
  else
  { 
    Serial.println("Falha na conexão com o servidor");
    delay(1000);
  }  
}
