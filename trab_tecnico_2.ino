#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>

#define LCD_RD A0
#define LCD_WR A1
#define LCD_CD A2
#define LCD_CS A3
#define LCD_RESET A4

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

//DEFINIÇÃO DAS CORES UTILIZADAS
#define WHITE  0xFFFF
#define YELLOW 0xFFE0
#define RED    0xF800
#define BLUE   0x001F
#define BLACK  0x0000
#define CYAN   0x07FF
#define PURPLE 0xDA70D6
#define GRAY   0xD3D3D3
#define GREEN  0x07E0
//-------------------------------

#define CORRENTE_BAR_X      5
#define CORRENTE_BAR_Y      70
#define CORRENTE_BAR_WIDTH  200
#define CORRENTE_BAR_HEIGHT 20

#define POTENCIA_BAR_X      5
#define POTENCIA_BAR_Y      170
#define POTENCIA_BAR_WIDTH  200
#define POTENCIA_BAR_HEIGHT 20

float correnteRMS = 20.0;

uint32_t byteCtrl  = 0;
uint16_t telaAtual = 0;

uint32_t lowByte  = 0,
         highByte = 0, 
         lampByte = 0,
         dummy    = 0;

byte acaoAtual     = 0x00;
byte estadoAtual   = 0x00;
byte estadoPorta   = 0x04;
byte alertaFumaca  = 0x00;
byte estadoLampada = 0x00;

unsigned long tempoAntigo = 0;
unsigned long tempoAtual;
uint16_t intervalo = 2000;

void setup() 
{
  //CONFIGURAÇÕES INICIAIS DO DISPLAY
  Wire.begin(8); // Endereço do escravo é 8
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  tft.begin(0x9341);
  tft.setRotation(2);
  tft.fillScreen(WHITE);
  //---------------------------------
//  exibirLCD(correnteRMS);
  Intro();
}

void loop(){}

void controleDisplay()
{
  telaAtual     = ( byteCtrl >> 8)  & 0xF;
  estadoAtual   = ((byteCtrl >> 4)  & 0xF) + 1;
  alertaFumaca  = ( byteCtrl >> 12) & 0x1;
  estadoPorta   = ( byteCtrl >> 13) & 0x3;
  estadoLampada = ( byteCtrl >> 16) & 0xF; 
  Serial.println(alertaFumaca);

  if(alertaFumaca == 1) FumacaDetectada();
  else
  {
    if (telaAtual == 0) Intro(); 
    if (telaAtual == 1) Intro2();
    if (telaAtual == 2) Menus(estadoAtual);
    if (telaAtual == 3) PortaFechada();
    if (telaAtual == 4) exibirLCD(correnteRMS);
    if (telaAtual == 5) Lampadas(estadoAtual);
    if (telaAtual == 6) PortaFechada();
    if (telaAtual == 7) PortaAberta();
    if (telaAtual == 8) PortaTrancada();
  }
}

void receiveEvent(int howMany)
{
  
  lowByte  = Wire.read();
  highByte = Wire.read(); 
  lampByte = Wire.read();
  dummy    = Wire.read();
 
  byteCtrl = (lampByte << 16) | (highByte << 8) | lowByte;

  Serial.print("byte recebido: ");
  Serial.println(byteCtrl, HEX);

  while (Wire.available()) 
  {
    Wire.readBytes((byte*)&correnteRMS, sizeof(correnteRMS));
    Serial.println(correnteRMS);
  }
  tft.begin(0x9341);
  
  controleDisplay();
}

void Intro() 
{
  tft.setRotation(2);
  tft.fillScreen(WHITE);
  tft.setCursor(10, 75);
  tft.setTextColor(PURPLE);
  tft.setTextSize(4);
  tft.println("RAITec");

  tft.setCursor(20, 120);
  tft.setTextColor(PURPLE);
  tft.setTextSize(4);
  tft.println("APRESENTA");

  delay(1500);

  int centerX = tft.width() / 1.9;
  int centerY = tft.height() / 1.4;

  int centerX1 = tft.width() / 2.2;
  int centerY1 = tft.height() / 1.5;

  // Largura e altura das letras "L" ajustadas
  int widthL     = 40;   // Largura de cada "L"
  int heightL    = 60;  // Altura de cada "L"
  int spacing    = 1;
  int spacing1   = 1;    // Espaçamento vertical entre as letras "L"
  int thickness  = 14;  // Espessura das linhas
  int thickness1 = 14;
  // Desenha o primeiro "L" (normal)
  tft.fillRoundRect(centerX - widthL / 5, centerY - heightL - spacing / 2, thickness, heightL, 5, CYAN);   // Linha horizontal
  tft.fillRoundRect(centerX - widthL / 5, centerY - spacing / 2 - thickness, widthL, thickness, 5, CYAN);  // Linha verticaç

  // Desenha o segundo "L" (invertido)
  tft.fillRoundRect(centerX1 - widthL / 5, centerY1 + spacing1, thickness1, heightL, 5, PURPLE);             // Linha horizontal invertida
  tft.fillRoundRect(centerX1 - widthL / 2 - thickness, centerY1 + spacing1, widthL, thickness1, 5, PURPLE);  // Linha vertical
}

void Intro2() 
{
  tft.setRotation(2);
  tft.fillScreen(BLACK);
  const int numStars = 100;
  int starX[numStars], starY[numStars];

  // Initialize star positions
  for (int i = 0; i < numStars; i++) {
    starX[i] = random(tft.width());
    starY[i] = random(tft.height());
 }

  tft.fillScreen(BLACK);

  // Draw stars with twinkling
  for (int i = 0; i < numStars; i++) {
    tft.drawPixel(starX[i], starY[i], WHITE);
    if (random(100) < 5) {
      tft.drawPixel(starX[i], starY[i], BLACK);
      delay(50);
      tft.drawPixel(starX[i], starY[i], WHITE);
    }
  }
  // Text with drop shadow (red text)
  tft.setTextSize(4);
  tft.setTextColor(RED);  // Set text color to RED
  tft.setCursor(50, 60);
  tft.println("A CASA");
  tft.setCursor(40, 95);
  tft.println("MONSTRO");
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(40, 140);
  tft.println("UM PROJETO IOT");
  // Draw white outline for the button
  tft.drawRoundRect(35, 185, 170, 90, 15, WHITE);  // White outline with larger size
  // Draw the red button
  tft.fillRoundRect(40, 190, 160, 80, 10, RED);  // Raio de 10 pixels
  
  tft.setTextSize(3);
  tft.setCursor(60, 220);
  tft.setTextColor(WHITE);
  tft.println(" START ");
}

void Menus(uint16_t estado) 
{
  // DESENHO DAS ESTRELAS-----------------------
  const int numStars = 100;
  int starX[numStars], starY[numStars];

  // Initialize star positions
  for (int i = 0; i < numStars; i++) {
    starX[i] = random(tft.width());
    starY[i] = random(tft.height());
  }
  tft.setRotation(2);
  tft.fillScreen(BLACK);
  // Draw stars with twinkling
  for (int i = 0; i < numStars; i++) {
    tft.drawPixel(starX[i], starY[i], WHITE);
    if (random(100) < 5) {
      tft.drawPixel(starX[i], starY[i], BLACK);
      delay(50);
      tft.drawPixel(starX[i], starY[i], WHITE);
    }
  }
  //-------------------------------------------
  // DESENHO DOS ÍCONES DO MENU-------------------

  // Contorno branco para o botão "Portas"
  switch (estado)
  {
    case 1:
      tft.drawRoundRect(35, 5, 170, 90, 15, WHITE);  
      break;
    case 2:
      tft.drawRoundRect(35, 115, 170, 90, 15, WHITE);
      break;
    case 3:
      tft.drawRoundRect(35, 225, 170, 90, 15, WHITE);
      break;
  }
  
  // Botão "Portas"
  tft.fillRoundRect(40, 10, 160, 80, 10, RED);
  tft.setTextSize(3);
  tft.setCursor(50, 40);
  tft.setTextColor(WHITE);
  tft.println(" Portas ");

  
  // Botão "Corrente"
  tft.fillRoundRect(40, 120, 160, 80, 10, RED);
  tft.setTextSize(3);
  tft.setCursor(33, 150);
  tft.setTextColor(WHITE);
  tft.println(" Corrente ");

  
  // Botão "Lampadas"
  tft.fillRoundRect(40, 230, 160, 80, 10, RED);
  tft.setTextSize(3);
  tft.setCursor(30, 260);
  tft.setTextColor(WHITE);
  tft.println(" Lampadas ");
  //----------------------------------------------
}

void PortaFechada() 
{
  tft.setRotation(2);
  tft.fillScreen(BLACK);
  tft.setTextColor(RED);
  tft.setTextSize(3);
  tft.setCursor(5, 100);
  tft.println("Porta");

  tft.setTextColor(RED);
  tft.setTextSize(3);
  tft.setCursor(96, 100);
  tft.println("Trancada");

  tft.setTextColor(RED);
  tft.setTextSize(2);
  tft.setCursor(30, 150);
  tft.println("Insira o Cartao");
}

void PortaAberta() 
{
  tft.setRotation(2);
  tft.fillScreen(BLACK);
  tft.setTextColor(GREEN);
  tft.setTextSize(4);
  tft.setCursor(50, 70);
  tft.println("Acesso");

  tft.setTextColor(GREEN);
  tft.setTextSize(4);
  tft.setCursor(27, 100);
  tft.println("Liberado");

  tft.setTextColor(RED);
  tft.setTextSize(2);
  tft.setCursor(40, 160);
  tft.println("Seja Bem-Vindo");

  //   for (int i = 0; i < 5; i++) {
  //     tft.drawFastHLine(0, i, tft.width(), BLUE);        // Linhas horizontais superiores
  //     tft.drawFastHLine(0, tft.height() - 1 - i, tft.width(), BLUE); // Linhas horizontais inferiores
  //     tft.drawFastVLine(i, 0, tft.height(), BLUE);        // Linhas verticais esquerdas
  //     tft.drawFastVLine(tft.width() - 1 - i, 0, tft.height(), BLUE); // Linhas verticais direitas
  //   }

  int x = 120;
  int y = 240;
  int size = 20;

  // Rosto
  tft.fillCircle(x, y, size, YELLOW);
  tft.drawCircle(x, y, size, BLACK);

  // Olhos
  int eyeSize = size / 5;
  tft.fillCircle(x - size / 3, y - size / 4, eyeSize, BLACK);
  tft.fillCircle(x + size / 3, y - size / 4, eyeSize, BLACK);


  int lockRadius = 13;
  int lockCenterX = 122.5;
  int lockCenterY = 240;
  int lockStartAngle = 180;
  int lockEndAngle = 360;

  for (int angle = lockStartAngle; angle <= lockEndAngle; angle++) {
    float radians = angle * PI / 460;
    int lx1 = lockCenterX + lockRadius * cos(radians);
    int ly1 = lockCenterY + lockRadius * sin(radians);
    int lx2 = lockCenterX + (lockRadius - 1) * cos(radians);  // A linha tem 1 pixel de largura, então o raio é reduzido em 1
    int ly2 = lockCenterY + (lockRadius - 1) * sin(radians);
    tft.drawLine(lx1, ly1, lx2, ly2, BLACK);
  }
}

void PortaTrancada() 
{
  tft.setRotation(2);
  tft.fillScreen(BLACK);

  tft.setTextColor(RED);
  tft.setTextSize(4);
  tft.setCursor(50, 80);
  tft.println("Acesso");

  tft.setTextColor(RED);
  tft.setTextSize(4);
  tft.setCursor(50, 125);
  tft.println("NEGADO");

  tft.setTextColor(RED);
  tft.setTextSize(2);
  tft.setCursor(30, 180);
  tft.println("Insira Novamente");

  // Desenha o corpo do cadeado
  tft.fillRoundRect(105, 240, 35, 35, 5, YELLOW);
  tft.drawRoundRect(105, 240, 35, 35, 5, BLACK);

  // Desenha a haste vertical do cadeado
  //tft.fillRect(120, 260, 7, 9, WHITE);

  // Desenha o arco de fechadura
  int lockRadius = 13;
  int lockCenterX = 122.5;
  int lockCenterY = 240;
  int lockStartAngle = 180;
  int lockEndAngle = 360;

  for (int angle = lockStartAngle; angle <= lockEndAngle; angle++) 
  {
    float radians = angle * PI / 180.0;
    int lx1 = lockCenterX + lockRadius * cos(radians);
    int ly1 = lockCenterY + lockRadius * sin(radians);
    int lx2 = lockCenterX + (lockRadius - 1) * cos(radians);  // A linha tem 1 pixel de largura, então o raio é reduzido em 1
    int ly2 = lockCenterY + (lockRadius - 1) * sin(radians);
    tft.drawLine(lx1, ly1, lx2, ly2, WHITE);
  }
}

void FumacaDetectada() 
{
  tft.setRotation(2);
  int x = 110;
  int y = 230;
  int size = 90;

  tft.fillScreen(BLACK);
  tft.setTextColor(RED);
  tft.setTextSize(4);
  tft.setCursor(60, 90);
  tft.println("SMOKE");

  tft.setTextColor(RED);
  tft.setTextSize(4);
  tft.setCursor(25, 120);
  tft.println("DETECTED");

  tft.fillTriangle(120, 190, 50, 298, 190, 298, YELLOW);
  tft.drawTriangle(120, 190, 50, 298, 190, 298, RED);

  tft.setTextColor(BLACK);
  tft.setTextSize(10);
  tft.setCursor(95,215);
  tft.println("!");
}

void Lampadas(uint16_t estado) {
  tft.fillScreen(BLACK);
  // Grossura de cada Pixel
  int wallThickness = 10;

  // Desenho da borda
  tft.fillRect(0,   0, 240, wallThickness,      WHITE);    // Superior
  tft.fillRect(0,   0,      wallThickness, 320, WHITE);    // Esquerdo
  tft.fillRect(230, 0,      wallThickness, 320, WHITE);  // Direito
  tft.fillRect(0, 310, 240, wallThickness,      WHITE);  // Baixo

  tft.fillRect(170, 10,  wallThickness, 10, WHITE);  // Vertical
  tft.fillRect(130, 10,  wallThickness, 10, WHITE);  // Vertical

  tft.fillRect(170, 50,      wallThickness, 40, WHITE);   // Vertical
  tft.fillRect(180, 80,  50, wallThickness,     WHITE);   // Horizontal
  tft.fillRect(170, 90,      wallThickness, 70, WHITE);   // Vertical
  tft.fillRect(210, 150, 20, wallThickness,     WHITE);  // Horizontal

  tft.fillRect(130, 160, 50, wallThickness,      WHITE);  // Horizontal
  tft.fillRect(130, 160,     wallThickness, 100, WHITE);  // Vertical
  tft.fillRect(130, 290,     wallThickness, 30,  WHITE);  // Vertical

  tft.fillRect(10,  120, 130, wallThickness, WHITE);  // Horizontal
  tft.fillRect(130, 50,  wallThickness, 70,  WHITE);   // Vertical

  // Lâmpadas
  tft.fillCircle(70, 70, 10, RED);    // Quarto
  tft.fillCircle(70, 220, 10, GRAY);  // SALA
  tft.fillCircle(205, 50, 10, BLUE);    // Quarto de serviço
  tft.fillCircle(185, 220, 10, CYAN);   // Cozinha

  // Indicação de lâmpada ligada
  auto drawRays = [](int x, int y) {
    tft.drawLine(x, y - 15, x, y - 20,YELLOW);
    tft.drawLine(x, y + 15, x, y + 20, YELLOW);
    tft.drawLine(x - 15, y, x - 20, y, YELLOW);
    tft.drawLine(x + 15, y, x + 20, y, YELLOW);
    tft.drawLine(x - 10, y - 10, x - 15, y - 15, YELLOW);
    tft.drawLine(x + 10, y - 10, x + 15, y - 15, YELLOW);
    tft.drawLine(x - 10, y + 10, x - 15, y + 15, YELLOW);
    tft.drawLine(x + 10, y + 10, x + 15, y + 15, YELLOW);
  };


  if ((estadoLampada & 0x1) == 0x1) drawRays(70, 70);
  if ((estadoLampada & 0x8) == 0x8) drawRays(70, 220);
  if ((estadoLampada & 0x2) == 0x2) drawRays(205, 50);
  if ((estadoLampada & 0x4) == 0x4) drawRays(185, 220);
  switch (estado)
  {
    case 3:

    
      tft.drawCircle(70, 70, 15, YELLOW);
        // Desenha raios de luz
      break;
    case 2:
      tft.drawCircle(70, 220, 15, YELLOW);
        // Desenha raios de luz
      break;
    case 4:
      tft.drawCircle(205, 50, 15, YELLOW);
        // Desenha raios de luz
      break;
    case 1:
      tft.drawCircle(185, 220, 15, YELLOW);
        
      break;
  }
}

void exibirLCD(float correnteRMS) 
{
  tft.setRotation(2);
  tft.fillScreen(BLACK);

  tft.setCursor(0, 35);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Corrente: ");

  tft.setCursor(110, 35);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print(correnteRMS);

  desenharBarraNivel(correnteRMS, CORRENTE_BAR_X, CORRENTE_BAR_Y, CORRENTE_BAR_WIDTH, CORRENTE_BAR_HEIGHT, GREEN);

  tft.setCursor(0, 140);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Potencia: ");

  Potencia(correnteRMS);
}

void desenharBarraNivel(float valor, int x, int y, int width, int height, uint16_t color) 
{
  valor = constrain(valor, 0, 30);
  tft.drawRect(x, y, width, height, color);
  int barFillWidth = map(valor, 0, 30, 0, width);
  tft.fillRect(x + 1, y + 1, barFillWidth - 1, height - 1, color);
}

void Potencia(float correnteRMS)
{
  float Potencia =  correnteRMS * 220.00;
   Potencia = constrain(Potencia, 0,6000);
  int potenciaBarFillWidth = map(Potencia, 0, 6000, 0, POTENCIA_BAR_WIDTH);
  desenharBarraNivel(Potencia, POTENCIA_BAR_X, POTENCIA_BAR_Y, potenciaBarFillWidth, POTENCIA_BAR_HEIGHT, CYAN);

  tft.drawRect(POTENCIA_BAR_X - 1, POTENCIA_BAR_Y - 1, POTENCIA_BAR_WIDTH + 2, POTENCIA_BAR_HEIGHT + 2, CYAN);

  tft.setCursor(110, 140);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print(Potencia);
}
