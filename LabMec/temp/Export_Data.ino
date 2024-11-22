#include <WiFi.h>
#include <Wire.h>

const char* ssid = "nome_da_rede";
const char* password = "senha_da_rede";
const char* host = "IP_da_rede";
const uint16_t port = 5000;

WiFiClient client;

float RateRoll, RatePitch, RateYaw;                                   //variaveis da taxa de giro
float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;  //variaveis de calibragem
float RateCalibrationNum;                                             //variavel de controle dos valores ja utilizados

//função para pegar as medidas RateRoll, Pitch e Yaw --------------------------------------------
void gyro_signals(void){

  Wire.beginTransmission(0x68); //endereço do proprio mpu6050
  Wire.write(0x1A); //endereço do filtro
  Wire.write(0x05); //frequencia 10 Hz
  Wire.endTransmission();

  Wire.beginTransmission(0x68);
  Wire.write(0x1B); //sensibilidade
  Wire.write(0x08); //FS_SEL=1 (1°/s = 65.5 LSB)
  Wire.endTransmission();

  Wire.beginTransmission(0x68);
  Wire.write(0x43); //endereço das medições do sensor
  Wire.endTransmission();

  Wire.requestFrom(0x68, 6); //solicita 6 bytes e guarda nas variaveis
  int16_t GyroX=Wire.read()<<8|Wire.read();
  int16_t GyroY=Wire.read()<<8|Wire.read();
  int16_t GyroZ=Wire.read()<<8|Wire.read();
  //convertendo de LSB para °/s
  RateRoll=(float)GyroX/65.5;
  RatePitch=(float)GyroY/65.5;
  RateYaw=(float)GyroZ/65.5;
}
 //--------------------------------------------------------------------------------
void MPU_Setup(){

  Wire.setClock(400000); //por especificação, clock em 400 kHz
  Wire.begin(); //inicia a biblioteca wire
  delay(250); //delay pro mpu iniciar
  
  Wire.beginTransmission(0x68); //coloca o mpu em power mode
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  //pega 2000 medidas de cada taxa de giro
  for(RateCalibrationNum=0; RateCalibrationNum<2000; RateCalibrationNum++){
    gyro_signals();
    RateCalibrationRoll+=RateRoll;
    RateCalibrationPitch+=RatePitch;
    RateCalibrationYaw+=RateYaw;
    delay(1);
  }
  //taxa de giro media dos eixos x, y, z
  RateCalibrationRoll/=2000;
  RateCalibrationPitch/=2000;
  RateCalibrationYaw/=2000;
}

//---------------------------------------------------------------------------------
void MPU_Loop(){
  
  gyro_signals(); //pega as medidas do sensor

  //calibragem das medidas
  RateRoll-=RateCalibrationRoll;
  RatePitch-=RateCalibrationPitch;
  RateYaw-=RateCalibrationYaw;
}

//--------------------------------------------------------------------------------------
void setup(){ 

    Serial.begin(115200);
    WiFi.printDiag(Serial);
    WiFi.begin(ssid, password);

    //MPU_Setup(); //inicializações do MPU

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando ao WiFi...");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Conectado ao WiFi!");
    } else {
        Serial.println("Falha ao conectar ao WiFi. Verifique as configurações.");
    }
}

//---------------------------------------------------------------------------------
void loop(){

    //Serial.println(WiFi.status());
    //WL_NO_SHIELD        = 255,
    //WL_IDLE_STATUS      = 0,
    //WL_NO_SSID_AVAIL    = 1,
    //WL_SCAN_COMPLETED   = 2,
    //WL_CONNECTED        = 3,
    //WL_CONNECT_FAILED   = 4,
    //WL_CONNECTION_LOST  = 5,
    //WL_DISCONNECTED     = 6

    //Serial.println(client.connected());

    if (client.connected()) {

      Serial.println("Conectado ao WiFi!!!");

        //MPU_Loop(); //calcula RateRoll, RatePitch, RateYaw

        RateRoll  = 1;
        RatePitch = 2;
        RateYaw   = 3;

        //int sensorValue = analogRead(34);  // Exemplo de leitura de um sensor
        client.printf("Rate_(Roll, Pitch, Yaw): %f %f %f\n", RateRoll, RatePitch, RateYaw);  // Envia dados formatados
        delay(50);  // Intervalo de 50 ms
    } else {

        Serial.println("Desconectado, tentando reconectar...");
        client.connect(host, port);
    }
}
