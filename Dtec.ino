// if (tempoAtual - tempoAntigo >= intervalo) 
// {

// }

#include "esp_task_wdt.h"  // Biblioteca para manipular o WDT


#define SensorPartida 15
#define SensorFim     2

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

void setup() 
{
  Serial.begin(115200);
  pinMode(SensorPartida, INPUT);
  pinMode(SensorFim,     INPUT);
}

void loop() 
{
//    int a = analogRead(SensorPartida);
//    int b = analogRead(SensorFim);
//    Serial.println(a);
//    Serial.println(b);
//    Serial.println("");
//    delay(100);
  if (Serial.available())
  {
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

//void comandarPartida()
//{
//  if (Serial.available()) comando = Serial.readStringUntil(' ');
//  
//  if (comando == "pronto\n") carrinhoCompetindo = true;
//  else                       carrinhoCompetindo = false;
//}

void esperarTermino()
{
  Serial.println("Carrinho Começou a Competição!!!");
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
  tempoFinal     = millis();
  tempoDecorrido = tempoFinal - inicioContagem;
  tempoTentativa = "Tempo:";

  minutes   =  tempoDecorrido / 60000;      
  seconds   = (tempoDecorrido % 60000) / 1000;
  millisecs =  tempoDecorrido % 1000;   

  // Formata a string no formato xx:xx:xxx
  if (minutes < 10) tempoTentativa += "0" + String(minutes);  
  else              tempoTentativa += String(minutes);
  
  tempoTentativa += ";";

  if (seconds < 10) tempoTentativa += "0" + String(seconds);  
  else              tempoTentativa += String(seconds);

  tempoTentativa += ";";

  if      (millisecs < 10)  tempoTentativa += "00" + String(millisecs);  
  else if (millisecs < 100) tempoTentativa += "0" + String(millisecs);   
  else                      tempoTentativa += String(millisecs);         
 
  Serial.println(tempoTentativa);
}
