// #include "Wire.h"
#define bottonMeio     6
#define bottonDireita  3
#define bottonEsquerda 7
#define bottonCima     5
#define bottonBaixo    4

unsigned long tempoAntigo = 0;
unsigned long tempoAtual;
uint16_t  intervalo = 1000;


uint16_t byteCtrl  = 0x0000;
uint16_t byteCtrlH = 0x0000;

byte byteCtrlL    = 0x00;
byte acaoAtual    = 0x00;
byte telaAtual    = 0x00;
byte estadoAtual  = 0x00;
byte estadoPorta  = 0x04;
byte alertaFumaca = 0x00;

bool acaoRealizada = false;

void setup()
{
  Serial.begin(9600);

  pinMode(bottonMeio,     INPUT);
  pinMode(bottonDireita,  INPUT);
  pinMode(bottonEsquerda, INPUT);
  pinMode(bottonCima,     INPUT);
  pinMode(bottonBaixo,    INPUT);
}
void loop()
{
  tempoAtual = millis();
  if (tempoAtual - tempoAntigo >= intervalo)
  {
    tempoAntigo = tempoAtual;
    acaoRealizada = false;
    obterAcaoCotrole();
    criarByteCtrl();
    controleDisplay();
  }
}

void obterAcaoCotrole()
{
  tempoAntigo = tempoAtual;
  if (digitalRead(bottonMeio)     == HIGH)
  {
    acaoRealizada = true;
    acaoAtual = 0x00;
  }
  if (digitalRead(bottonDireita)  == HIGH)
  {
    acaoRealizada = true;
    acaoAtual = 0x01;
  }
  if (digitalRead(bottonBaixo)    == HIGH)
  {
    acaoRealizada = true;
    acaoAtual = 0x02;
  }
  if (digitalRead(bottonEsquerda) == HIGH)
  {
    acaoRealizada = true;
    acaoAtual = 0x03;
  }
  if (digitalRead(bottonCima)     == HIGH)
  {
    acaoRealizada = true;
    acaoAtual = 0x04;
  }

}

void criarByteCtrl()
{
  byteCtrlL = (estadoAtual << 4) | acaoAtual;
  byteCtrlH = (estadoPorta << 5) | (alertaFumaca << 4) | telaAtual;
  byteCtrl  = (byteCtrlH << 8)   | byteCtrlL;
  Serial.print("O byte de controle montado he: ");
  Serial.println(byteCtrl, BIN);

  Serial.print("O valor da Tela Atual he: ");
  Serial.println(telaAtual, BIN);
  Serial.println("");
  Serial.print("O valor do Estado Atual he: ");
  Serial.println(estadoAtual, BIN);
  Serial.println("");
  Serial.print("O valor da Acao Atual he: ");
  Serial.println(acaoAtual, BIN);
  Serial.println("");
  Serial.print("O valor do Estado da porta Atual he: ");
  Serial.println(estadoPorta, BIN);
  Serial.println("");
  Serial.print("O valor do alerta fumaca he: ");
  Serial.println(alertaFumaca, BIN);
}

void controleDisplay()
{
  if (alertaFumaca == 0x01)
  {
    telaAtual = 0x09;
    delay(3000);
  } else
  {
    if (acaoRealizada == false)Serial.println("NENHUMA ACAO DETECTADA!");
    else                       Serial.println("ACAO DETECTADA, FEZENDO O CONTROLE...");
    
    switch (telaAtual)
    {
      case 0x00: // Tela atual
        Serial.println("INTRODUÇÃO 1");
        delay(1000);
        if ((acaoAtual == 0x00) && (acaoRealizada == true)) telaAtual = 0x01;
        break;

      case 0x01: // Tela atual
        estadoAtual = 0x00;
        Serial.println("INTRODUÇÃO 2");
        if ((acaoAtual == 0x00) && (acaoRealizada == true)) telaAtual = 0x02;
        break;

      case 0x02: // Tela atual
        Serial.println("MENU PRINCIPAL");
        switch (estadoAtual)
        {
          case 0x00: // Estado atual
            Serial.println("ICONE DA PORTA");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x00:
                  telaAtual   = 0x03;   //Selecionar o Icone
                  break;
                case 0x01:
                  telaAtual   = 0x03;   //Selecionar o Icone
                  break;
                case 0x02:
                  estadoAtual = 0x01;   //Icone abaixo
                  break;
                case 0x03:
                  telaAtual   = 0x01;   //Voltar pro menu anterior
                  break;
                case 0x04:
                  estadoAtual = 0x02;   //Icone acima
                  break;
              }
            }
            break;
          case 0x01: // Estado atual
            Serial.println("ICONE DA MEDICAO");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x00:
                  telaAtual   = 0x04; //Selecionar o Icone
                  break;
                case 0x01:
                  telaAtual   = 0x04; //Selecionar o Icone
                  break;
                case 0x02:
                  estadoAtual = 0x02; //Icone abaixo
                  break;
                case 0x03:
                  telaAtual   = 0x01; //Volta para o menu anterior
                  break;
                case 0x04:
                  estadoAtual = 0x00; //Icone acima
                  break;
              }
            }
            break;
          case 0x02: // Estado atual
            Serial.println("ICONE DAS LAMPADAS");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x00:
                  telaAtual   = 0x05; //Selecionar o Icone
                  break;
                case 0x01:
                  telaAtual   = 0x05; //Selecionar o Icone
                  break;
                case 0x02:
                  estadoAtual = 0x00; //Icone abaixo
                  break;
                case 0x03:
                  telaAtual   = 0x01; //Volta para o menu anterior
                  break;
                case 0x04:
                  estadoAtual = 0x01; //Icone acima
                  break;
              }
            }
            break;
        }
        break;

      case 0x03: // Tela atual
        Serial.println("MENU DAS PORTAS");
        if ((acaoAtual == 0x03) && acaoRealizada == true) telaAtual = 0x02;
        switch (estadoPorta)
        {
          case 0x00:
            Serial.println("Porta Fechada");
            telaAtual = 0x06;
            break;
          case 0x01:
            Serial.println("Porta Aberta");
            telaAtual = 0x07;
            delay(1000);
            Serial.println("Porta Fechada");
            telaAtual = 0x06;
            break;
          case 0x02:
            Serial.println("Porta Trancada");
            telaAtual = 0x08;
            delay(1000);
            Serial.println("Porta Fechada");
            telaAtual = 0x06;
            break;
        }
        break;

      case 0x04: // Tela atual
        if ((acaoAtual == 0x03) && acaoRealizada == true) telaAtual = 0x02;
        Serial.println("MENU DO CONSUMO");
        break;

      case 0x05: // Tela atual
        // estadoAtual = 0x00;
        Serial.println("MENU DAS LAMPADAS");
        switch (estadoAtual)
        {
          case 0x00:
            Serial.println("LAMPADA 1");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x01:
                  estadoAtual = 0x01;
                  break;
                case 0x03:
                  estadoAtual = 0x02; //Voltar para o menu principal
                  telaAtual   = 0x02; //Voltar para o seu Icone 
                  break;
                case 0x02:
                  estadoAtual = 0x04;
                  break;
              }
            }
            break;
          case 0x01: // Estado atual
            Serial.println("LAMPADA 2");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x02:
                  estadoAtual = 0x02;
                  break;
                case 0x03:
                  estadoAtual = 0x00;
                  break;
              }
            }
            break;
          case 0x02: // Estado atual
            Serial.println("LAMPADA 3");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x02:
                  estadoAtual = 0x03;
                  break;
                case 0x03:
                  estadoAtual = 0x00;
                  break;
                case 0x04:
                  estadoAtual = 0x01;
                  break;
              }
            }
            break;
          case 0x03: // Estado atual
            Serial.println("LAMPADA 4");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x03:
                  estadoAtual = 0x04;
                  break;
                case 0x04:
                  estadoAtual = 0x02;
                  break;
              }
            }
            break;
          case 0x04: // Estado atual
            Serial.println("LAMPADA 5");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x01:
                  estadoAtual = 0x03;
                  break;
                case 0x03:
                  estadoAtual = 0x02; // Voltar ao menu anterior
                  telaAtual   = 0x02; // Voltar ao seu Icone
                  break;
                case 0x04:
                  estadoAtual = 0x00;
                  break;
              }
            }
            break;
        }
        break;
    }
  }
}
