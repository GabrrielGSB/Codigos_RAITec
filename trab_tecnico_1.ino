// #include "Wire.h" 
#define bottonMeio 6
#define bottonDireita 3
#define bottonEsquerda 7
#define bottonCima 5
#define bottonBaixo 4
byte byteCtrlL;
byte byteCtrlH;
unsigned long tempoAntigo = 0;
unsigned long tempoAtual;
uint8_t intervalo = 250;
byte acaoAtual = B00000000;
byte telaAtual = B00000000;
byte estadoAtual = B00000000;

void setup() {
  Serial.begin(9600);

  pinMode(bottonMeio, INPUT);
  pinMode(bottonDireita, INPUT);
  pinMode(bottonEsquerda, INPUT);
  pinMode(bottonCima, INPUT);
  pinMode(bottonBaixo, INPUT);

  byteCtrlL = B00001000;
  byteCtrlH = B00000001;
  
}
void loop() {
  tempoAtual = millis();
  if (tempoAtual - tempoAntigo >= intervalo)
  {
    tempoAntigo = tempoAtual;
  
    if (digitalRead(bottonMeio) == HIGH)
    {
      byteCtrlL = byteCtrlL & B11111000;
    }
    if (digitalRead(bottonDireita) == HIGH)
    {
      byteCtrlL = byteCtrlL | B00000001;
      byteCtrlL = byteCtrlL & B11111001;
    }
    if (digitalRead(bottonBaixo) == HIGH)
    {
      byteCtrlL = byteCtrlL | B00000010;
      byteCtrlL = byteCtrlL & B11111010;
    }
    if (digitalRead(bottonEsquerda) == HIGH)
    {
      byteCtrlL = byteCtrlL | B00000011;
      byteCtrlL = byteCtrlL & B11111011;
    }
    if (digitalRead(bottonCima) == HIGH)
    {
      byteCtrlL = byteCtrlL | B00000100;
      byteCtrlL = byteCtrlL & B11111100;
    }
  }


  switch (telaAtual)
  {
    case B00000000:
      delay(1000);
      telaAtual = B00000001;
      byteCtrlH = byteCtrlH | telaAtual;
      byteCtrlH = byteCtrlH & B11110001;
      break;
    case B00000001:
      byteCtrlL = byteCtrlL & B00001111;
      if (byteCtrlL == B00000000) telaAtual = B00000010; 
      byteCtrlH = byteCtrlH | telaAtual;
      byteCtrlH = byteCtrlH & B11110010;
      break;
    case B00000010:
      byteCtrlL = byteCtrlL & B00001111;
      switch (byteCtrlL)
      {
        case B00000000:
          telaAtual = B00000011; 
          byteCtrlH = byteCtrlH | telaAtual;
          byteCtrlH = byteCtrlH & B11110011;
          break;
        case B00000001;
          estadoAtual = B00010000;
          byteCtrlL = byteCtrlL & B00001111;
          byteCtrlL = byteCtrlL | estadoAtual;
          break;
        case B00010000;
          telaAtual = B00000100; 
          byteCtrlH = byteCtrlH | telaAtual;
          byteCtrlH = byteCtrlH & B11110100;
          break;


      }
  }
  

  uint16_t controlBytes = (byte)byteCtrlL | ((byte)byteCtrlH << 8);

  Serial.println(controlBytes, BIN);
}