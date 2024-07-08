// #include "Wire.h" 
#define bottonMeio 6
#define bottonDireita 3
#define bottonEsquerda 7
#define bottonCima 5
#define bottonBaixo 4

unsigned long tempoAntigo = 0;
unsigned long tempoAtual;
uint8_t intervalo = 250;
uint16_t byteCtrl = 0x0000;

byte byteCtrlL = 0x00;
uint16_t byteCtrlH = 0x0000;
byte acaoAtual     = 0x00;
byte telaAtual      = 0x00;
byte estadoAtual = 0x00;
byte estadoPorta = 0x00;
byte alertaFumaca = 0x00;

void setup()
{
  Serial.begin(9600);

  pinMode(bottonMeio, INPUT);
  pinMode(bottonDireita, INPUT);
  pinMode(bottonEsquerda, INPUT);
  pinMode(bottonCima, INPUT);
  pinMode(bottonBaixo, INPUT);
}
void loop() {
  tempoAtual = millis();
  if (tempoAtual - tempoAntigo >= intervalo)
  {
    tempoAntigo = tempoAtual;
  
    if (digitalRead(bottonMeio) == HIGH) acaoAtual = 0x00;
    if (digitalRead(bottonDireita) == HIGH) acaoAtual = 0x01
    if (digitalRead(bottonBaixo) == HIGH) acaoAtual = 0x02
    if (digitalRead(bottonEsquerda) == HIGH) acaoAtual = 0x03
    if (digitalRead(bottonCima) == HIGH) acaoAtual = 0x04

if (alertaFumaca == 0x01)
{
	telaAtual = 0x09;
	delay(3000);
} else 
{

	  switch (telaAtual)
	  {
	    case 0x00: // Tela atual
	        delay(1000);
	        telaAtual = 0x01;
	        break;
	    case 0x01: // Tela atual
	        estadoAtual = 0x00
	        if (acaoAtual == 0x00) telaAtual = 0x02; 
	        break;
	    case 0x02: // Tela atual
	      switch (estadoAtual)
	      {
		  	case 0x00: // Estado atual
			  	switch (acaoAtual)
			      {
			        case 0x00:
			            telaAtual = 0x03; 
			            break;
			        case 0x01: 
			            telaAtual = 0x03;
			            break;
			        case 0x02;
			            estadoAtual = 0x01;
			            break;
			        case 0x04:
			        	telaAtual = 0x01;
			        	break;
			        case 0x05:
			        	estadoAtual = 0x02;
			        	break;
			      }
			      break;
		  	case 0x01: // Estado atual
		  		switch (acaoAtual)
			      {
			        case 0x00:
			            telaAtual = 0x04; 
			            break;
			        case 0x01: 
			            telaAtual = 0x04;
			            break;
			        case 0x02;
			            estadoAtual = 0x02;
			            break;
			        case 0x04:
			        	telaAtual = 0x01;
			        	break;
			        case 0x05:
			        	estadoAtual = 0x00;
			        	break;
			      }
			      break;
			  case 0x02: // Estado atual
			  	switch (acaoAtual)
			      {
			        case 0x00:
			            telaAtual = 0x05; 
			            break;
			        case 0x01: 
			            telaAtual = 0x05;
			            break;
			        case 0x02;
			            estadoAtual = 0x00;
			            break;
			        case 0x04:
			        	telaAtual = 0x01;
			        	break;
			        case 0x05:
			        	estadoAtual = 0x01;
			        	break;
			      }
			      break;
	      }
	    case 0x03: // Tela atual
	    	switch (estadoPorta)
	    	{
	    		case 0x00:
	    			telaAtual = 0x06;
	    			break;
	    		case 0x01:
	    			telaAtual = 0x07;
	    			delay(1000);
	    			telaAtual = 0x06;
	    		    break;
	    		case 0x02:
	    			telaAtual = 0x08;
	    			delay(1000);
	    			telaAtual = 0x06;
	    			break;
	    	}
	    case 0x04:
	    	break;
	    case 0x05:
	    switch (estadoAtual)
	    {
	    	case 0x00:
	    		switch (acaoAtual)
	    		{
	    			case 0x01:
	    				estadoAtual = 0x01;
	    				break;
	    			case 0x03:
	    				estadoAtual = 0x02;
	    				telaAtual = 0x02;
	    				break;
	    			case 0x02:
	    				estadoAtual = 0x04;
	    				break;
	    		}
	    	case 0x01: // Estado atual 
	    		switch (acaoAtual)
	    		{
	    			case 0x02:
	    				estadoAtual = 0x02;
	    				break;
	    			case 0x03:
	    				estadoAtual = 0x00;
	    				break;	
	    		}
	    	case 0x02: // Estado atual
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
	    	case 0x03: // Estado atual
	    		switch (acaoAtual)
	    		{
	    			case 0x03:
	    				estadoAtual = 0x04;
	    				break;
	    			case 0x04:
	    				estadoAtual = 0x02;
	    				break;
	    		}
	    	case 0x04: // Estado atual
	    		switch (acaoAtual)
	    		{
	    			case 0x01:
	    				estadoAtual = 0x03;
	    				break;
	    			case 0x03:
	    				estadoAtual = 0x02;
	    				telaAtual = 0x02;
	    				break;
	    			case 0x04:
	    				estadoAtual = 0x00;
	    				break;
	    		}
	    	
	    }
	     	
	  }
 }
 
 byteCtrlL = (estadoAtual << 4) | acaoAtual;
 byteCtrlH = (estadoPorta << 6) | (alertaFumaca << 4) | telaAtual; 
 
 byteCtrl = (byteCtrlH << 8) | byteCtrlL;
 
 
   

  uint16_t controlBytes = (byte)byteCtrlL | ((byte)byteCtrlH << 8);

  Serial.println(controlBytes, BIN);
}