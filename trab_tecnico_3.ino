#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN   21  // SDA
#define RST_PIN  22 // RST
#define comando1 27
#define comando2 26
#define comando3 25
#define comando4 33

bool corretaTAG = 0;

unsigned long tempoAntigo = 0;
unsigned long tempoAtual;
uint16_t  intervalo = 2000;

MFRC522 mfrc522(SS_PIN, RST_PIN); // Cria um objeto MFRC522

void setup() {
  Serial.begin(115200); // Inicializa a comunicação serial
  delay(2000);
  SPI.begin();          // Inicializa o barramento SPI
  mfrc522.PCD_Init();   // Inicializa o MFRC522

  pinMode(comando1, OUTPUT);
  pinMode(comando2, OUTPUT);
  pinMode(comando3, OUTPUT);
  pinMode(comando4, OUTPUT);

  digitalWrite(comando1, LOW);
  digitalWrite(comando2, LOW);
  digitalWrite(comando3, LOW);
  digitalWrite(comando4, LOW);
}

void loop() 
{
   tempoAtual = millis();
  if (tempoAtual - tempoAntigo >= intervalo) // Taxa de atualização do loop de controle
  {
    tempoAntigo = tempoAtual;
    if (verificaTAG()) 
    {
      digitalWrite(comando1, HIGH);
      delay(5000);
    }
    else               digitalWrite(comando1, LOW);
  }
}

bool verificaTAG()
{
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
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  mfrc522.PICC_HaltA();

  return corretaTAG;
}
