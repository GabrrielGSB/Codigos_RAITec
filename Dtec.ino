#include "driver/ledc.h"

#define SensorPartida 33
#define SensorFim     25
#define buzzer        27

String nomeEquipe, 
       tempoTentativa,
       comando;

uint16_t intervalo = 2000;

bool sensorPartida,
     sensorFim,
     esperandoPartida   = true,
     carrinhoCompetindo = false,
     equipeCadastrada   = false;

unsigned long tempoAntigo = 0,
              tempoAtual,
              minutes,
              seconds,
              millisecs,
              inicioContagem,
              tempoFinal,
              tempoDecorrido;

const int pwmPin = 27;   // Pino para o sinal PWM
const int pwmChannel = 0; // Canal PWM (de 0 a 15)
const int pwmFreq = 5000; // Frequência em Hz
const int pwmResolution = 8; // Resolução do duty cycle (8 bits, de 0 a 255)

void setup() 
{
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(pwmPin, pwmChannel);
  
  Serial.begin(115200);
  pinMode(SensorPartida, INPUT);
  pinMode(SensorFim,     INPUT);
  pinMode(buzzer,        OUTPUT);
  
  digitalWrite(buzzer, LOW);
}

void loop() 
{
  if (Serial.available())
  {
    tocarMusica(1000, 500);
    while (carrinhoCompetindo == false)
    {
      if (equipeCadastrada == false) cadastrarEquipe();
      esperarPartida();
    }  
    
    inicioContagem = millis();
    esperarTermino();
    obterTempo();
    
    equipeCadastrada   = false;
    carrinhoCompetindo = false;
  }
}

void receberSensor()
{
  if (analogRead(SensorPartida) < 1500) sensorPartida = true;
  else                                 sensorPartida = false;
  
  if (analogRead(SensorFim)     < 1500) sensorFim     = true;
  else                                 sensorFim     = false;
}

void cadastrarEquipe()
{
  receberSensor();

  Serial.println("Cadastrar nome da equipe: ");
  while(!Serial.available()) delay(1); // Enquanto o nome da equipe não estiver definido espera!!
  
  nomeEquipe  = "Equipe cadastrada:";
  nomeEquipe += Serial.readStringUntil(' ');  
  Serial.println(nomeEquipe);
  equipeCadastrada = true;
}

void esperarPartida()
{
  Serial.println("Esperando o Carinho Partir...");

  while (sensorPartida == false) {receberSensor();}
  carrinhoCompetindo = true;
}

void esperarTermino()
{
  Serial.println("Carrinho Começou a Competição!!!");

  for (int i = 0; i > 50; i++);
  {
    tocarMusica(1000, 50);
    delay(100);
  }
  
  while(carrinhoCompetindo == true) 
  {
    receberSensor();

    if (sensorFim == true) carrinhoCompetindo = false;
    delay(1);
  }
}

void obterTempo()
{
  Serial.println("Carrinho completou o percurso!!!");
  tocarMusica(500, 5000);
  delay(100);
  tocarMusica(500, 5000);
  delay(100);
  tempoFinal     = millis();
  tempoDecorrido = tempoFinal - inicioContagem;
  tempoTentativa = "Tempo:";

  minutes   =  tempoDecorrido / 60000;      
  seconds   = (tempoDecorrido % 60000) / 1000;
  millisecs =  tempoDecorrido % 1000;   

  // Formata a string no formato xx:xx:xxx
  if (minutes < 10) tempoTentativa += "0" + String(minutes);  
  else              tempoTentativa += String(minutes);
  
  tempoTentativa += ":";

  if (seconds < 10) tempoTentativa += "0" + String(seconds);  
  else              tempoTentativa += String(seconds);

  tempoTentativa += ":";

  if      (millisecs < 10)  tempoTentativa += "00" + String(millisecs);  
  else if (millisecs < 100) tempoTentativa += "0" + String(millisecs);   
  else                      tempoTentativa += String(millisecs);         
 
  Serial.println(tempoTentativa);
}

void tocarMusica(uint16_t intervalo, uint16_t freq)
{
  tone(buzzer, freq);
  delay(intervalo);
  noTone(buzzer);
}
//void comandarPartida()
//{
//  if (Serial.available()) comando = Serial.readStringUntil(' ');
//  
//  if (comando == "pronto\n") carrinhoCompetindo = true;
//  else                       carrinhoCompetindo = false;
//}

//    int a = analogRead(SensorPartida);
//    int b = analogRead(SensorFim);
//    Serial.println(a);
//    Serial.println(b);
//    Serial.println("");
//    delay(100);
