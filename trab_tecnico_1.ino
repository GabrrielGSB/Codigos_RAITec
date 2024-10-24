#include "Wire.h"
#define controlX       A0
#define controlY       A1
#define fumaca         A2
#define Isensor        A3
#define controlBtt     3
#define lamp1          4                                                                                                                        
#define lamp2          5
#define lamp3          6
#define lamp4          7
#define ESPcomand      8
#define controlPorta   13


unsigned long tempoAntigo = 0;
unsigned long tempoAtual;

uint16_t  intervalo = 100,
          danger;

uint32_t byteCtrl    = 0;
uint32_t byteCtrlOld = 0;
uint32_t byteCtrlH   = 0;
uint32_t CtrlLamp    = 0;

float corrente = 15.454;

byte byteCtrlL    = 0x00,
     acaoAtual    = 0x00,
     telaAtual    = 0x00,
     telaOld      = 0x00,
     estadoAtual  = 0x00,
     estadoPorta  = 0x04,
     alertaFumaca = 0x00;

byte dados_para_enviar[5];

bool acaoRealizada  = false;
bool oneBtt         = 0,
     oneUp          = 0, 
     oneDown        = 0, 
     oneLeft        = 0, 
     oneRight       = 0,
     onePastBtt     = 0,
     onePastUp      = 0,
     onePastDown    = 0, 
     onePastLeft    = 0, 
     onePastRight   = 0;
bool lampOn1 = 0,
     lampOn2 = 0,
     lampOn3 = 0,
     lampOn4 = 0;
bool ESPcom;

int sen;

void setup()
{
  Serial.begin(9600);

  pinMode(controlX,     INPUT);
  pinMode(controlY,     INPUT);
  pinMode(controlBtt,   INPUT);
  pinMode(fumaca,       INPUT);
  pinMode(ESPcomand,    INPUT);
  pinMode(Isensor,      INPUT);
  pinMode(lamp1,        OUTPUT);
  pinMode(lamp2,        OUTPUT);
  pinMode(lamp3,        OUTPUT);
  pinMode(lamp4,        OUTPUT);
  pinMode(controlPorta, OUTPUT);

  digitalWrite(lamp1,        LOW);
  digitalWrite(lamp2,        LOW); 
  digitalWrite(lamp3,        LOW);
  digitalWrite(lamp4,        LOW);
  digitalWrite(controlPorta, LOW);
  


}

void loop()
{
  tempoAtual = millis();
  if (tempoAtual - tempoAntigo >= intervalo) // Taxa de atualização do loop de controle
  {
    tempoAntigo = tempoAtual;
    acaoRealizada = false;
    
    byteCtrlOld = byteCtrl;
    detectarFumaca();
    detectarPorta();
    estadoLampadas();
    obterAcaoCotrole();
    calcularCorrente(); 
    criarByteCtrl();
    controleDisplay();
    if (byteCtrl != byteCtrlOld) enviarControle();
    Serial.println(byteCtrl,HEX);
  }
}

void criarByteCtrl()
{
  byte* ItoByte = (byte*)&corrente;
   
  dados_para_enviar[0] = (estadoAtual << 4) | acaoAtual;
  dados_para_enviar[1] = (estadoPorta << 5) | (alertaFumaca << 4) | telaAtual;
  dados_para_enviar[2] = CtrlLamp;
  dados_para_enviar[3] = ItoByte[0];
  dados_para_enviar[4] = ItoByte[1];
  
//  byteCtrlL = (estadoAtual << 4) | acaoAtual; 
//  byteCtrlH = (CtrlLamp    << 8) | (estadoPorta << 5) | (alertaFumaca << 4) | telaAtual; 
//  byteCtrl  = (byteCtrlH   << 8) | byteCtrlL;
}

void enviarControle()
{
  Wire.begin();
  Wire.beginTransmission(8); 
  Wire.write(dados_para_enviar, sizeof(dados_para_enviar));
  Wire.endTransmission();
  Wire.end();
}

void calcularCorrente() 
{
  //- Para 30A, sensibilidade = 0.066;
  //- Para 20A, sensibilidade = 0.100;
  //- Para 5A,  sensibilidade = 0.185;
  long somaDasCorrentes = 0, mediaDasCorrentes;
  
  for (int i = 0; i < 1000; i++) 
  {
    somaDasCorrentes += pow((analogRead(A3) - 509), 2);
    delay(1);
  }
  
  mediaDasCorrentes = sqrt(somaDasCorrentes / 1000.0);
  corrente = (mediaDasCorrentes) * (5.000) / (1023.000 * 0.100);
  
//  if (mediaDasCorrentes == 1) return 0;
}

void detectarPorta()
{
  ESPcom = digitalRead(ESPcomand);
  if (ESPcom) 
  {
    estadoPorta = 0x05;
    digitalWrite(controlPorta, HIGH);
  }
  else
  {
    estadoPorta = 0x06;
    digitalWrite(controlPorta, LOW);
  }
}

void detectarFumaca()
{
  danger = analogRead(fumaca);
  if (danger >= 500) alertaFumaca = 0x01;
  else               alertaFumaca = 0x00;          
}

void estadoLampadas()
{
  if (lampOn1) CtrlLamp = (CtrlLamp | 0x1);
  else         CtrlLamp = (CtrlLamp & 0xE);
  if (lampOn2) CtrlLamp = (CtrlLamp | 0x2);
  else         CtrlLamp = (CtrlLamp & 0xD);
  if (lampOn3) CtrlLamp = (CtrlLamp | 0x4);
  else         CtrlLamp = (CtrlLamp & 0xB);
  if (lampOn4) CtrlLamp = (CtrlLamp | 0x8);
  else         CtrlLamp = (CtrlLamp & 0x7);

  if (lampOn1) digitalWrite(lamp1, LOW);
  else         digitalWrite(lamp1, HIGH );
  if (lampOn2) digitalWrite(lamp2, LOW);
  else         digitalWrite(lamp2, HIGH );
  if (lampOn3) digitalWrite(lamp3, LOW);
  else         digitalWrite(lamp3, HIGH );
  if (lampOn4) digitalWrite(lamp4, LOW);
  else         digitalWrite(lamp4, HIGH );
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

void controleDisplay()
{
  if (telaAtual != 0x09) telaOld = telaAtual;
  if (alertaFumaca == 0x01)
  {
    telaAtual = 0x09;
    Serial.println("ALERTA DE FUMAÇA!!!");
    criarByteCtrl();
    enviarControle();
    delay(850);
  } else
  {
    telaAtual = telaOld;
    if (acaoRealizada == false)Serial.println("NENHUMA ACAO DETECTADA!");
    else                       Serial.println("ACAO DETECTADA, FEZENDO O CONTROLE...");
    
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
              break;
            case 0x05:
              Serial.println("Porta Aberta");
              telaAtual = 0x07;
              break;
            // case 0x06:
            //   Serial.println("Porta Trancada");
            //   telaAtual = 0x08;
            //   break;
          }
        }
        break;

      case 0x04: // Tela atual
        if ((acaoAtual == 0x03) && acaoRealizada == true) telaAtual = 0x02;
        Serial.println("MENU DO CONSUMO");
        break;

      case 0x05: // Tela atual
        Serial.println("MENU DAS LAMPADAS");
        switch (estadoAtual)
        {
          case 0x02: // Estado atual
            Serial.println("LAMPADA 1");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x00:
                  lampOn1 = !lampOn1;
                  break; 
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
                case 0x04:
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
                case 0x00:
                  lampOn2 = !lampOn2;
                  break; 
                case 0x02:
                  estadoAtual = 0x00;
                  break;
                case 0x03:
                  estadoAtual = 0x02;
                  break;
                case 0x01:
                  estadoAtual = 0x02;
                  break;
                case 0x04:
                  estadoAtual = 0x00;
                  break;
              }
            }
            break;

          case 0x00: // Estado atual
            Serial.println("LAMPADA 3");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x00:
                  lampOn3 = !lampOn3;
                  break; 
                case 0x03:
                  estadoAtual = 0x01;
                  break;
                case 0x04:
                  estadoAtual = 0x03;
                  break;
                case 0x01:
                  estadoAtual = 0x01;
                  break;
                case 0x02:
                  estadoAtual = 0x03;
              }
            }
            break;

          case 0x01: // Estado atual
            Serial.println("LAMPADA 4");
            if (acaoRealizada == true)
            {
              switch (acaoAtual)
              {
                case 0x00:
                  lampOn4 = !lampOn4;
                  break; 
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
                case 0x02:
                  estadoAtual = 0x02;
              }
            }
            break;
        }
        break;

      case 0x06: 
        // if (estadoPorta == 0x05) telaAtual = 0x07;
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
        criarByteCtrl();
        enviarControle();
        delay(3500);
        telaAtual = 0x03;
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
