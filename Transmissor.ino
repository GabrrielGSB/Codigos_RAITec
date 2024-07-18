

/* Projeto Escravo
 * 1.Temperatura
 * 2.Umidade
 * 3.Luminosidade
 * 4.Vibração
 */
//Inclusão de Bibliotecas

#include <SoftwareSerial.h>


SoftwareSerial RF(8,9);

String codigo;

void setup() {
  
Serial.begin(9600);
RF.begin(4800);
pinMode(led_inicio, OUTPUT);
}

void loop() 
{
  //Enviar luminosidade para a central poderosa
  codigo = "B";
  enviar_dado(valor_luminosidade,codigo);
  
  delay(5000);
}

void enviar_dado(int medicao, String codigo)
{
  for(int i = 0; i < 5 ; i++)
  {
    RF.flush();
    RF.println(codigo);
    RF.println(medicao);
  }
  delay(5000);
}
