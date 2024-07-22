#include "WiFi.h"
#include <HTTPClient.h>

//----------------------------------------

// Defining LED PINs on the ESP32 Board.
#define On_Board_LED_PIN  2

// Declaração das funções das tarefas
void Task1(void *pvParameters);
void Task2(void *pvParameters);

//----------------------------------------SSID and PASSWORD of your WiFi network.
const char* ssid = "brisa-2848347";  //--> Your wifi name
const char* password = "1thnhbc6"; //--> Your wifi password
//----------------------------------------

// Google script Web_App_URL.
String Web_App_URL = "https://script.google.com/macros/s/AKfycbxQKjw64xI9rLzCCJd9Se_tlBwewjM_AuZRE-NmVB80pFwkioswAy0Y36UZWb6Ntg-pRg/exec";

String Switch_1_State = "on";
String Switch_2_State = "on";

String Status_Read_Sensor = "";
float Temp = 10.2;
int Humd = 22;


void setup() {
  Serial.begin(115200);
  delay(1000); // Pequeno atraso para garantir que o monitor serial esteja pronto

   //----------------------------------------Set Wifi to STA mode
  Serial.println();
  Serial.println("-------------");
  Serial.println("WIFI mode : STA");
  WiFi.mode(WIFI_STA);
  Serial.println("-------------");
  //---------------------------------------- 

  //----------------------------------------Connect to Wi-Fi (STA).
  Serial.println();
  Serial.println("------------");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  //:::::::::::::::::: The process of connecting ESP32 with WiFi Hotspot / WiFi Router.
  // The process timeout of connecting ESP32 with WiFi Hotspot / WiFi Router is 20 seconds.
  // If within 20 seconds the ESP32 has not been successfully connected to WiFi, the ESP32 will restart.
  // I made this condition because on my ESP32, there are times when it seems like it can't connect to WiFi, so it needs to be restarted to be able to connect to WiFi.

  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(On_Board_LED_PIN, HIGH);
    delay(250);
    digitalWrite(On_Board_LED_PIN, LOW);
    delay(250);
    if (connecting_process_timed_out > 0) connecting_process_timed_out--;
    if (connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
  }

  digitalWrite(On_Board_LED_PIN, LOW);
  
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("------------");
  //::::::::::::::

  // Cria a tarefa 1 no núcleo 0
  xTaskCreatePinnedToCore(
    Task1,        // Função da tarefa
    "Task1",      // Nome da tarefa
    10000,        // Tamanho da pilha da tarefa
    NULL,         // Parâmetro da tarefa
    1,            // Prioridade da tarefa
    NULL,         // Handle da tarefa
    0);           // Núcleo onde a tarefa será executada

  // Cria a tarefa 2 no núcleo 1
  xTaskCreatePinnedToCore(
    Task2,        // Função da tarefa
    "Task2",      // Nome da tarefa
    10000,        // Tamanho da pilha da tarefa
    NULL,         // Parâmetro da tarefa
    1,            // Prioridade da tarefa
    NULL,         // Handle da tarefa
    1);           // Núcleo onde a tarefa será executada

  Serial.println("Tarefas criadas.");
  delay(2000);

}

void loop() {
  // O loop principal está vazio, pois as tarefas estão sendo executadas nos núcleos
}

// Definição da tarefa 1
void Task1(void *pvParameters) 
{
  while (1) 
  {
     if (WiFi.status() == WL_CONNECTED)
    {
      digitalWrite(On_Board_LED_PIN, HIGH);

      // Create a URL for sending or writing data to Google Sheets.
      String Send_Data_URL = Web_App_URL + "?sts=write";
      Send_Data_URL += "&srs=" + Status_Read_Sensor;
      Send_Data_URL += "&temp=" + String(Temp);
      Send_Data_URL += "&humd=" + String(Humd);
      Send_Data_URL += "&swtc1=" + Switch_1_State;
      Send_Data_URL += "&swtc2=" + Switch_2_State;

      Serial.println();
      Serial.println("-------------");
      Serial.println("Send data to Google Spreadsheet...");
      Serial.print("URL : ");
      Serial.println(Send_Data_URL);

      //::::::::::::::::::The process of sending or writing data to Google Sheets.
        // Initialize HTTPClient as "http".
        HTTPClient http;
    
        // HTTP GET Request.
        http.setTimeout(15000); // Tempo de espera de 15 segundos

        http.begin(Send_Data_URL.c_str());
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    
        // Gets the HTTP status code.
        int httpCode = http.GET(); 
        Serial.print("HTTP Status Code : ");
        Serial.println(httpCode);
  
      // Getting response from google sheets.
      if (httpCode > 0) 
      {
        String payload = http.getString();
        
        if (payload.length() > 0) 
        {
          Serial.println("Payload : " + payload);
        } 
        else 
        {
          Serial.println("Payload is empty");
        }

      }
      else 
      {
        Serial.println("Error on HTTP request");
      }
      http.end();
      //::::::::::::::::::
      
      digitalWrite(On_Board_LED_PIN, LOW);
      Serial.println("-------------");
    
      //---------------------------------------
      Serial.println("Executando Tarefa 1 no núcleo 0");
      vTaskDelay(1000 / portTICK_PERIOD_MS); // Aguarda 1 segundo
    }
  }
}

// Definição da tarefa 2
void Task2(void *pvParameters) {
  while (1) {
    Serial.println("Executando Tarefa 2 no núcleo 1");
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Aguarda 2 segundos
  }
}
