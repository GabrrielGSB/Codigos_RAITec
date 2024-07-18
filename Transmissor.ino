

/* Projeto Escravo
 * 1.Temperatura
 * 2.Umidade
 * 3.Luminosidade
 * 4.Vibração
 */
//Inclusão de Bibliotecas


#include <SoftwareSerial.h>
//#include "LowPower.h"

SoftwareSerial RF(8,9);

//Sensor de Luminosidade
int valor_luminosidade; 
int pino_ldr = A0;

//Sensor de Vibração
int valor_vibracao; 
int pino_piezo = A2;

int led_inicio = 13;

void setup() {
  
Serial.begin(9600);

RF.begin(4800);

Serial.println("Teste:");

pinMode(led_inicio, OUTPUT);


}

void loop() {

//Ler luminosidade
valor_luminosidade = analogRead(pino_ldr);
Serial.print("Luminosidade:");
Serial.println(valor_luminosidade);
delay(500);

//Ler vibração
valor_vibracao = analogRead(pino_piezo);
valor_vibracao = valor_vibracao;
Serial.print("Vibração:");
Serial.println(valor_vibracao);
delay(500);



//Enviar luminosidade para a central poderosa
String codigo = "B";
enviar_dado(valor_luminosidade,codigo);

delay(5000);

//Enviar vibracao para a central poderosa
codigo = "A";
enviar_dado(valor_vibracao, codigo);

delay(5000);
 
//dormir();


}

void enviar_dado(int medicao, String codigo){

for(int i = 0; i < 5 ; i++){
RF.flush();
RF.println(codigo);
RF.println(medicao);

}

digitalWrite(led_inicio, HIGH);
delay(500);
digitalWrite(led_inicio, LOW);

delay(5000);

  
}

//void dormir () {
//  //Modo Economia Atmega e módulos
//Serial.println("Dormindo");
//
//// ROTINA PARA COLOCAR O MICROCONTROLADOR ATMEGA328 PARA DORMIR
//for (int i=0; i<10; i++){
//
//LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
//
//}
//
//// ROTINA PARA COLOCAR O DHT11 PARA DORMIR
//
//
//Serial.println("Acordei!");
//}
