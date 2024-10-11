#include <ESP32Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 21   // SDA
#define RST_PIN 22  // RST
#define comando1 27
#define porta    26  // porta
#define comando3 25
#define motor    33

bool corretaTAG = 0,
     estadoPorta = 0,
     estadoPortaOld = 0;

unsigned long tempoAntigo = 0;
unsigned long tempoAtual;
uint16_t intervalo = 2000;
uint8_t  contPorta = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Cria um objeto MFRC522
Servo motorPorta;

void setup() {
  Serial.begin(115200);  // Inicializa a comunicação serial
  delay(2000);
  SPI.begin();         // Inicializa o barramento SPI
  mfrc522.PCD_Init();  // Inicializa o MFRC522

  pinMode(comando1, OUTPUT);
  pinMode(porta, INPUT);
  pinMode(comando3, OUTPUT);
  pinMode(motor, OUTPUT);

  digitalWrite(comando1, LOW);
  digitalWrite(comando3, LOW);
 
  estadoPortaOld = 1;
}

void loop() {
  tempoAtual = millis();
  if (tempoAtual - tempoAntigo >= intervalo)  // Taxa de atualização do loop de controle
  {
    tempoAntigo = tempoAtual;
    if (verificaTAG()) {
      digitalWrite(comando1, HIGH);
      delay(5000);
    } else digitalWrite(comando1, LOW);
  }
  controlePorta();

//  Serial.println(porta.read());
}

void controlePorta() 
{
//  estadoPorta = !estadoPorta;
  estadoPorta = digitalRead(porta);
  
  if (estadoPorta != estadoPortaOld) 
  {
    
    estadoPortaOld = estadoPorta;
    if ( (estadoPorta == 0) && (contPorta >= 1) ) 
    {
      motorPorta.attach(33);
      motorPorta.write(1);
      delay(750);
      motorPorta.detach();
      delay(2000);
    }

    if ( (estadoPorta == 1) && (contPorta >= 1) ) 
    {
      motorPorta.attach(33);
      motorPorta.write(200);
      delay(750);
      motorPorta.detach();
      delay(2000);
    }
    contPorta++;
  }
}

bool verificaTAG() {
  // Verifica se há uma nova tag presente
  corretaTAG = true;
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return !corretaTAG;
  }

  // Seleciona uma das tags
  if (!mfrc522.PICC_ReadCardSerial()) {
    return !corretaTAG;
  }

  // Imprime o UID da tag
  Serial.print("UID da tag: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  mfrc522.PICC_HaltA();

  return corretaTAG;
}


//  fechada = 50;
//  aberto = 100;
//
//  if (estadoPorta != estadoPortaOld) {
//    if (estadoPorta == 0) {
//      for (int i = fechada; i == aberto; i++) {  // abre
//        porta.write(i);
//        delay(100);
//      }
//      estadoPortaOld = estadoPorta;
//    }
//    if (estadoPorta == 1) {
//      for (int j = aberto; j == fechada; j--) {  // fecha
//        porta.write(j);
//        delay(100);
//      }
//      estadoPortaOld = estadoPorta;
//    }
//  }
