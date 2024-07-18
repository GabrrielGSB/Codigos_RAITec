#include <HardwareSerial.h>

HardwareSerial RxSerial(2);

int valor_recebido;
int valor_recebido_1;
int valor_recebido_2;
int valor_recebido_3;
int valor_recebido_4;
int valor_recebido_5;


void setup() 
{  
  Serial.begin(115200);
  RxSerial.begin(4800,SERIAL_8N1,16,17);
}

void loop() 
{
  String received = "";
  while (RxSerial.available())
  {
    received = char(RxSerial.read());
    Serial.println(received);
    
    if(received == "A")
    {
      received = char(RxSerial.read());
      valor_recebido_1 = atoi(received.c_str());   
      while(valor_recebido_1 == 0 )
      {
        received = char(RxSerial.read());
        delay(1);  
        valor_recebido_1 = atoi(received.c_str());                 
      }
      delay(1);
      
      received = char(RxSerial.read());
      valor_recebido_2 = atoi(received.c_str());   
      delay(1);

      received = char(RxSerial.read());
      valor_recebido_3 = atoi(received.c_str());   
      delay(1);

      received = char(RxSerial.read());
      valor_recebido_4 = atoi(received.c_str());   
      delay(1);

      received = char(RxSerial.read());
      valor_recebido_5 = atoi(received.c_str());   
      delay(1);
      
//      valor_recebido_1 = valor_recebido_1*1000;
//      valor_recebido_2 = valor_recebido_2*100;
//      valor_recebido_3 = valor_recebido_3*10;
//      valor_recebido = valor_recebido_4 + valor_recebido_3 + valor_recebido_2 + valor_recebido_1;
//      Serial.print("Valor recebido de vibração:");
//      Serial.println(valor_recebido);
      Serial.print(valor_recebido_1);
      Serial.print(valor_recebido_2);
      Serial.print(valor_recebido_3);
      Serial.print(valor_recebido_4);
      Serial.println(valor_recebido_5);
    }
  }
}
