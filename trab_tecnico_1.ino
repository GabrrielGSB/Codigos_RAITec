#include "Wire.h"
#define controlX       A0
#define controlY       A1
#define controlBtt     3


unsigned long tempoAntigo = 0;
unsigned long tempoAtual;
uint16_t  intervalo = 100;


uint16_t byteCtrl    = 0x0000;
uint16_t byteCtrlOld = 0x0000;
uint16_t byteCtrlH   = 0x0000;

byte byteCtrlL    = 0x00;
byte acaoAtual    = 0x00;
byte telaAtual    = 0x00;
byte estadoAtual  = 0x00;
byte estadoPorta  = 0x06;
byte alertaFumaca = 0x00;

bool acaoRealizada  = false;
bool oneBtt       = 0,
     oneUp        = 0, 
     oneDown      = 0, 
     oneLeft      = 0, 
     oneRight     = 0,
     onePastBtt   = 0,
     onePastUp    = 0,
     onePastDown  = 0, 
     onePastLeft  = 0, 
     onePastRight = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(controlX,   INPUT);
  pinMode(controlY,   INPUT);
  pinMode(controlBtt, INPUT);
}
void loop()
{
  tempoAtual = millis();
  if (tempoAtual - tempoAntigo >= intervalo)
  {
    tempoAntigo = tempoAtual;
    acaoRealizada = false;

    byteCtrlOld = byteCtrl;
    obterAcaoCotrole();
    criarByteCtrl();
    controleDisplay();
    if (byteCtrl != byteCtrlOld)
    {
      enviarControle();
    }
    
  }
  
}

void enviarControle()
{
  // delay(1000);
  Wire.begin();
  Wire.beginTransmission(8); 
  Wire.write((byte*)&byteCtrl, sizeof(byteCtrl));
  // Wire.write((byte*)&byteCtrlL, sizeof(byteCtrlL));
  Wire.endTransmission();
  Wire.end();
  // delay(500);
  // Wire.beginTransmission(8);
  
  // Wire.endTransmission();
}

void obterAcaoCotrole()
{
  if (digitalRead(controlBtt) == LOW)
  {
    oneBtt = 1;
    if (onePastBtt == 0)
    {
      acaoRealizada = true;
      acaoAtual = 0x00;
      Serial.println(acaoAtual);
    }
    onePastBtt = oneBtt;
  }
  else
  {
   oneBtt = 0;
   onePastBtt = oneBtt;
  }

  if (analogRead(controlY) < 100)
  {
    oneRight = 1;
    if (onePastRight == 0)
    {
      acaoRealizada = true;
      acaoAtual = 0x01;
      Serial.println(acaoAtual);
    }
    onePastRight = oneRight;
  }
  else
  {
    oneRight = 0;
    onePastRight = oneRight;
  }

  if (analogRead(controlX) < 100)
  {
    oneDown = 1;
    if (onePastDown == 0)
    {
      acaoRealizada = true;
      acaoAtual = 0x02;
      Serial.println(acaoAtual);
    }
    onePastDown = oneDown;
  }
  else
  {
    oneDown = 0;
    onePastDown = oneDown;
  }

  if (analogRead(controlY) > 900)
  {
    oneLeft = 1;
    if (onePastLeft == 0)
    {
      acaoRealizada = true;
      acaoAtual = 0x03;
      Serial.println(acaoAtual);
    }
    onePastLeft = oneLeft;
  }
  else
  {
    oneLeft = 0;
    onePastLeft = oneLeft;
  }

  if (analogRead(controlX) > 900)
  {
    oneUp = 1;
    if (onePastUp == 0)
    {
      acaoRealizada = true;
      acaoAtual = 0x04;
      Serial.println(acaoAtual);
    }
    onePastUp = oneUp;
  }
  else
  {
   oneUp = 0;
   onePastUp = oneUp;
  }


}

void criarByteCtrl()
{
  byteCtrlL = (estadoAtual << 4) | acaoAtual;
  byteCtrlH = (estadoPorta << 5) | (alertaFumaca << 4) | telaAtual;
  byteCtrl  = (byteCtrlH << 8)   | byteCtrlL;
  // Serial.print("O byte de controle montado he: ");
  // Serial.println(byteCtrl, BIN);

  // Serial.print("O valor da Tela Atual he: ");
  // Serial.println(telaAtual, BIN);
  // Serial.println("");
  // Serial.print("O valor do Estado Atual he: ");
  // Serial.println(estadoAtual, BIN);
  // Serial.println("");
  // Serial.print("O valor da Acao Atual he: ");
  // Serial.println(acaoAtual, BIN);
  // Serial.println("");
  // Serial.print("O valor do Estado da porta Atual he: ");
  // Serial.println(estadoPorta, BIN);
  // Serial.println("");
  // Serial.print("O valor do alerta fumaca he: ");
  // Serial.println(alertaFumaca, BIN);
}

void controleDisplay()
{
  if (alertaFumaca == 0x01)
  {
    telaAtual = 0x09;
    Serial.println("ALERTA DE FUMAÇA!!!");
    criarByteCtrl();
    enviarControle();
    delay(850);
  } else
  {
    if (acaoRealizada == false)Serial.println("NENHUMA ACAO DETECTADA!");
    else                       Serial.println("ACAO DETECTADA, FEZENDO O CONTROLE...");
    
    if (telaAtual == 0x06) telaAtual = 0x03;
    switch (telaAtual)
    {
      case 0x00: // Tela atual
        Serial.println("INTRODUÇÃO 1");
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
        if (acaoAtual == 0x03) 
        {
          telaAtual = 0x02;
          estadoAtual = 0x00;
        }
        else
        {
        switch (estadoPorta)
        {
          case 0x04:
            Serial.println("Porta Fechada");
            telaAtual = 0x06;
//            criarByteCtrl();
//            enviarControle();
//            telaAtual = 0x03;
            break;
          case 0x05:
            Serial.println("Porta Aberta");
            telaAtual = 0x07;
//            criarByteCtrl();
//            enviarControle();
//            delay(5000);
//            Serial.println("Porta Fechada");
//            telaAtual = 0x06;
//            criarByteCtrl();
//            enviarControle();
            break;
          case 0x06:
            Serial.println("Porta Trancada");
            telaAtual = 0x08;
//            criarByteCtrl();
//            enviarControle();
//            delay(5000);
//            Serial.println("Porta Fechada");
//            telaAtual = 0x06;
//            criarByteCtrl();
//            enviarControle();
            break;
        }
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
          case 0x02:
            Serial.println("LAMPADA 1");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x01:
                  estadoAtual = 0x03;
                  break;
                case 0x03:
                  estadoAtual = 0x02; //Voltar para o menu principal
                  telaAtual   = 0x02; //Voltar para o seu Icone 
                  break;
                case 0x02:
                  estadoAtual = 0x01;
                  break;
              }
            }
            break;
          case 0x03: // Estado atual
            Serial.println("LAMPADA 2");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x02:
                  estadoAtual = 0x04;
                  break;
                case 0x03:
                  estadoAtual = 0x02;
                  break;
              }
            }
            break;
          case 0x04: // Estado atual
            Serial.println("LAMPADA 3");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x02:
                  estadoAtual = 0x00;
                  break;
                case 0x03:
                  estadoAtual = 0x02;
                  break;
                case 0x04:
                  estadoAtual = 0x03;
                  break;
              }
            }
            break;
          case 0x00: // Estado atual
            Serial.println("LAMPADA 4");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x03:
                  estadoAtual = 0x01;
                  break;
                case 0x04:
                  estadoAtual = 0x04;
                  break;
              }
            }
            break;
          case 0x01: // Estado atual
            Serial.println("LAMPADA 5");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x01:
                  estadoAtual = 0x00;
                  break;
                case 0x03:
                  estadoAtual = 0x02; // Voltar ao menu anterior
                  telaAtual   = 0x02; // Voltar ao seu Icone
                  break;
                case 0x04:
                  estadoAtual = 0x02;
                  break;
              }
            }
            break;
        }
        break;

      case 0x06: 
        if (acaoRealizada == true)
        {
          switch (acaoAtual)
          {
            case 0x03:
              telaAtual   = 0x02;
              estadoAtual = 0x00; 
              break;  
          }
        }
        break;
        
      case 0x07: 
        if (acaoRealizada == true)
        {
          switch (acaoAtual)
          {
            case 0x03:
              telaAtual = 0x02;
              estadoAtual = 0x00; 
              break;  
          }
        }
        break;
      case 0x08: 
        if (acaoRealizada == true)
        {
          switch (acaoAtual)
          {
            case 0x03:
              telaAtual = 0x02;
              estadoAtual = 0x00; 
              break;  
          }
        }
        break;    
    }
          
  }
    
}
