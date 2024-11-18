#include <Arduino.h>
#include <Servo.h>
#include <WiFi.h>
#include <Wire.h>
#include "time.h"
#include "esp_adc_cal.h"

// Timers
unsigned long tUltimoMovimento = 0;

#pragma region Variáveis Captação
const int pinFlexor = 34;
const int pinExtensor = 32;
const int pinPolegar = 35;

// Variáveis de leitura
float fValorSinalFlexor;
float fValorSinalExtensor;
float fValorSinalPolegar;
#pragma endregion

#pragma region Valores para detectar movimento
#pragma region Valores de movimento para o FLEXOR
// const float movFlexFlexao = 704.47;
// const float movFlexExtensao = 235.85;
// const float movFlexPolFlex = 90.66;
// const float movFlexPolExt = 185.16;
// const float movFlexPinca = 762.72;
// const float movFlexJoinha = 342.28;
// const float movFlexFlexaoMax = 2590.04;
// const float movFlexExtensaoMax = 1130.62;
// const float movFlexPolFlexMax = 93.48;
// const float movFlexPolExtMax = 399.63;
// const float movFlexPincaMax = 2352.1;
// const float movFlexJoinhaMax = 1440.88;
// const float movFlexMAMax = 1230.55;
// const float movFlexMIMax = 964.62;

const float movFlexFlexao = 844.93;
const float movFlexExtensao = 412.79;
const float movFlexPolFlex = 226.66;
const float movFlexPolExt = 241.32;
const float movFlexPinca = 591.34;
const float movFlexJoinha = 374.17;
const float movFlexMA = 248.54;
const float movFlexMI = 267.2;
const float movFlexAgarrar = 342.75;

const float movFlexFlexaoMax = 3300;
const float movFlexExtensaoMax = 1346.59;
const float movFlexPolFlexMax = 435.97;
const float movFlexPolExtMax = 572.97;
const float movFlexPincaMax = 3300;
const float movFlexJoinhaMax = 1282.12;
const float movFlexMAMax = 505.27;
const float movFlexMIMax = 671.28;
const float movFlexAgarrarMax = 813.11;
#pragma endregion

#pragma region Valores de movimento para o EXTENSOR
// const float movExtFlexao = 701.16;
// const float movExtExtensao = 633.03;
// const float movExtPolFlex = 307.14;
// const float movExtPolExt = 370.51;
// const float movExtPinca = 998.13;
// const float movExtJoinha = 486.12;
// const float movExtFlexaoMax = 3300;
// const float movExtExtensaoMax  = 3137.22;
// const float movExtPolFlexMax = 3300;
// const float movExtPolExtMax = 2049.3;
// const float movExtPincaMax = 3300;
// const float movExtJoinhaMax = 1998.53;
// const float movExtMAMax = 1256.34;
// const float movExtMIMax = 1228.94;

const float movExtFlexao = 912.51;
const float movExtExtensao = 653.05;
const float movExtPolFlex = 238.93;
const float movExtPolExt = 162.33;
const float movExtPinca = 728.94;
const float movExtJoinha = 516.5;
const float movExtMA = 345.57;
const float movExtMI = 238.47;
const float movExtAgarrar = 1063.39;

const float movExtFlexaoMax = 3300;
const float movExtExtensaoMax = 2222.56;
const float movExtPolFlexMax = 1497.29;
const float movExtPolExtMax = 914.65;
const float movExtPincaMax = 2455.46;
const float movExtJoinhaMax = 3300;
const float movExtMAMax = 1237;
const float movExtMIMax = 1271.65;
const float movExtAgarrarMax = 3300;
#pragma endregion

#pragma region Valores de movimento para o POLEGAR
// const float movPolFlexao = 390.65;
// const float movPolExtensao = 608.49;
// const float movPolPolFlex = 186.39;
// const float movPolPolExt = 865.87;
// const float movPolPinca =  301.55;
// const float movPolJoinha = 420.11;
// const float movPolFlexaoMax = 1284.54;
// const float movPolExtensaoMax = 330;
// const float movPolPolFlexMax = 992.82;
// const float movPolPolExtMax = 3300;
// const float movPolPincaMax = 2115.38;
// const float movPolJoinhaMax = 2103.3;
// const float movPolMAMax = 176.78;
// const float movPolMIMax = 165.2;

const float movPolFlexao = 1183.76;
const float movPolExtensao = 1418.71;
const float movPolPolFlex = 654.36;
const float movPolPolExt = 1000;
const float movPolPinca = 1130.77;
const float movPolJoinha = 897.86;
const float movPolMA = 821.2;
const float movPolMI = 638.47;
const float movPolAgarrar = 834.15;

const float movPolFlexaoMax = 3300;
const float movPolExtensaoMax = 3300;
const float movPolPolFlexMax = 1508.57;
const float movPolPolExtMax = 3300;
const float movPolPincaMax = 3300;
const float movPolJoinhaMax = 1913.11;
const float movPolMAMax = 2304.76;
const float movPolMIMax = 1098.39;
const float movPolAgarrarMax = 1505.35;
#pragma endregion
#pragma endregion

#pragma region Pinos servos
Servo servo;
const int pinServoFlexMA = 21;
const int pinServoFlexMI = 19;
const int pinServoPolegar = 18;
const int sFBFlexMA = 25;
const int sFBFlexMI = 26;
const int sFBPol = 27;
#pragma endregion

#pragma region Valores de calibração dos servos
int iMinPos;
int iMaxPos;
int iMinFeedback;
int iMaxFeedback;
const int iTolerancia = 20;
const int iPosInicial = 100;
const int iTmpDetecBarreira = 3000;
#pragma endregion

// Define valor de feedback máximo e mínimo para o servo
// Desta forma é possível interpolar os valores das posições intermediárias.
void Calibrar(int feedbackPin, int minPos, int maxPos, int pin)
{
  servo.write(pin, minPos);
  iMinPos = minPos;

  delay(2000);

  iMinFeedback = analogRead(feedbackPin);

  servo.write(pin, maxPos);
  iMaxPos = maxPos;

  delay(2000);

  iMaxFeedback = analogRead(feedbackPin);
}

void Mover(int fbMI, int posMI, int pinMI, int fbMA, int posMA, int pinMA, int fbP, int posP, int pinP)
{
  // Caso posição enviada seja maior que ou menor que os valores permitidos, trava na posição inicial simulando um desligamento.
  if (posMI > iMaxPos || posMI > iMaxPos || posP > iMaxPos || posMI < iMinPos || posMI < iMinPos || posP < iMinPos)
  {
    servo.write(pinP, iPosInicial);
    servo.write(pinMI, iPosInicial);
    servo.write(pinMA, iPosInicial);

    while (true)
    {
      Serial.println("Um valor de posição fora do permitido foi detectado. Desligado!");
    }
  }

  // Calcula o valor desejado para última posição
  int posDesejadaP = map(posP, iMinPos, iMaxPos, iMinFeedback, iMaxFeedback);
  int posDesejadaMI = map(posMI, iMinPos, iMaxPos, iMinFeedback, iMaxFeedback);
  int posDesejadaMA = map(posMA, iMinPos, iMaxPos, iMinFeedback, iMaxFeedback);

  servo.write(pinP, posP);
  servo.write(pinMI, posMI);
  servo.write(pinMA, posMA);

  long tmr = millis();
  int posAnteriorP = abs(analogRead(fbP));
  int posAnteriorMA = abs(analogRead(fbMA));
  int posAnteriorMI = abs(analogRead(fbMI));
  bool bBarreira = false;

  bool bManterPosPol = false;
  if (abs(analogRead(fbP) - posDesejadaP) > iTolerancia)
    bManterPosPol = true;

  bool bManterPosMA = false;
  if (abs(analogRead(fbMA) - posDesejadaMA) > iTolerancia)
    bManterPosMA = true;

  bool bManterPosMI = false;
  if (abs(analogRead(fbMI) - posDesejadaMI) > iTolerancia)
    bManterPosMI = true;

  bool bManter = false;
  if (bManterPosPol && bManterPosMA && bManterPosMI)
    bManter = true;

  // Espera até chegar a posição, caso posição não seja alterada dentro do tempo definido em iTmpDetecBarreira retorna
  while ((abs(analogRead(fbP) - posDesejadaP) > iTolerancia || abs(analogRead(fbMI) - posDesejadaMI) > iTolerancia || abs(analogRead(fbMA) - posDesejadaMA) > iTolerancia) && !bManter)
  {
    if (posAnteriorP >= abs(analogRead(fbP)) && (millis() - tmr) > iTmpDetecBarreira && !bManterPosPol)
    {
      Serial.println("Barreira detectada pelo Polegar!");
      bBarreira = true;
    }

    if (posAnteriorMI >= abs(analogRead(fbMI)) && (millis() - tmr) > iTmpDetecBarreira && !bManterPosMI)
    {
      Serial.println("Barreira detectada pelo conjunto Médio-Indicador!");
      bBarreira = true;
    }

    if (posAnteriorMA >= abs(analogRead(fbMA)) && (millis() - tmr) > iTmpDetecBarreira && !bManterPosMA)
    {
      Serial.println("Barreira detectada pelo conjunto Mínimo-Anelar!");
      bBarreira = true;
    }

    if (bBarreira)
      return;

    posAnteriorP = abs(analogRead(fbP));
    posAnteriorMI = abs(analogRead(fbMI));
    posAnteriorMA = abs(analogRead(fbMA));
  }
}

int GetPosicao(int feedbackPin)
{
  return map(analogRead(feedbackPin), iMinFeedback, iMaxFeedback, iMinPos, iMaxPos);
}

// Inicializa configurações e conexão com FireBase
void setup()
{
  Serial.begin(115200);

  // Define precisão das portas analógicas do myoware
  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_11db); // pino 32
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_11db); // pino 34
  adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_11db); // pino 35

  pinMode(pinFlexor, INPUT);
  pinMode(pinExtensor, INPUT);
  pinMode(pinPolegar, INPUT);

  servo.write(pinServoFlexMA, iPosInicial);
  servo.write(pinServoFlexMI, iPosInicial);
  servo.write(pinServoPolegar, iPosInicial);

  Serial.println("Calibrando servos....");
  Calibrar(sFBFlexMI, 0, 160, pinServoFlexMI);
  Calibrar(sFBFlexMA, 0, 160, pinServoFlexMA);
  Calibrar(sFBPol, 0, 160, pinServoPolegar);
  Serial.println("Calibração finalizada!");

  servo.write(pinServoPolegar, iPosInicial);
  servo.write(pinServoFlexMI, iPosInicial);
  servo.write(pinServoFlexMA, iPosInicial);

  delay(1000);
}

void loop()
{
  // Delay necessário entre captações, também representa tempo do pré sinal de acionamento dos músculos antes do movimento
  delay(1000);

  tUltimoMovimento = millis();

  fValorSinalFlexor = analogRead(pinFlexor);
  fValorSinalExtensor = analogRead(pinExtensor);
  fValorSinalPolegar = analogRead(pinPolegar);

  // Converte leitura analógica para mV
  // Sensor Myoware -> Valor rectificado + sinal EMG integrado, com precisão de 12bits (4096)
  fValorSinalFlexor = ((fValorSinalFlexor / 4095) * 3.3) * 1000;
  fValorSinalExtensor = ((fValorSinalExtensor / 4095) * 3.3) * 1000;
  fValorSinalPolegar = ((fValorSinalPolegar / 4095) * 3.3) * 1000;

  Serial.println();
  Serial.print(" - Flex: ");
  Serial.print(fValorSinalFlexor);
  Serial.print("mV");
  Serial.print(" - Ext: ");
  Serial.print(fValorSinalExtensor);
  Serial.print("mV");
  Serial.print(" - Pol: ");
  Serial.print(fValorSinalPolegar);
  Serial.println("mV");
  
  if((fValorSinalFlexor >= movFlexAgarrar && fValorSinalFlexor <= movFlexAgarrarMax) && (fValorSinalExtensor >= movExtAgarrar && fValorSinalExtensor <= movExtAgarrarMax) && (fValorSinalPolegar >= movPolAgarrar && fValorSinalPolegar <= movPolAgarrarMax))
  {
    Serial.println("Agarrar");
    Mover(sFBFlexMI, 140, pinServoFlexMI, sFBFlexMA, 135, pinServoFlexMA, sFBPol, 135, pinServoPolegar);
    Serial.print("Tempo (ms): ");
    Serial.println(millis() - tUltimoMovimento);
    tUltimoMovimento = millis();
  }
  else if ((fValorSinalFlexor >= movFlexFlexao && fValorSinalFlexor <= movFlexFlexaoMax) && (fValorSinalExtensor >= movExtFlexao && fValorSinalExtensor <= movExtFlexaoMax) && (fValorSinalPolegar >= movPolFlexao && fValorSinalPolegar <= movPolFlexaoMax))
  {
    Serial.println("Flexão Completa");
    Mover(sFBFlexMI, 150, pinServoFlexMI, sFBFlexMA, 150, pinServoFlexMA, sFBPol, 150, pinServoPolegar);
    Serial.print("Tempo (ms): ");
    Serial.println(millis() - tUltimoMovimento);
    tUltimoMovimento = millis();
  }
  else if ((fValorSinalFlexor >= movFlexPinca && fValorSinalFlexor <= movFlexPincaMax) && (fValorSinalExtensor >= movExtPinca && fValorSinalExtensor <= movExtPincaMax) && (fValorSinalPolegar >= movPolPinca && fValorSinalPolegar <= movPolPincaMax))
  {
    Serial.println("Pinça");
    Mover(sFBFlexMI, 120, pinServoFlexMI, sFBFlexMA, 120, pinServoFlexMA, sFBPol, 120, pinServoPolegar);
    Serial.print("Tempo (ms): ");
    Serial.println(millis() - tUltimoMovimento);
    tUltimoMovimento = millis();
  }
  else if ((fValorSinalFlexor >= movFlexExtensao && fValorSinalFlexor <= movFlexExtensaoMax) && (fValorSinalExtensor >= movExtExtensao && fValorSinalExtensor <= movExtExtensaoMax) && (fValorSinalPolegar >= movPolExtensao && fValorSinalPolegar <= movPolExtensaoMax))
  {
    Serial.println("Extensão");
    Mover(sFBFlexMI, 0, pinServoFlexMI, sFBFlexMA, 0, pinServoFlexMA, sFBPol, 0, pinServoPolegar);
    Serial.print("Tempo (ms): ");
    Serial.println(millis() - tUltimoMovimento);
    tUltimoMovimento = millis();
  }
  else if ((fValorSinalFlexor >= movFlexJoinha && fValorSinalFlexor <= movFlexJoinhaMax) && (fValorSinalExtensor >= movExtJoinha && fValorSinalExtensor <= movExtJoinhaMax) && (fValorSinalPolegar >= movPolJoinha && fValorSinalPolegar <= movPolJoinhaMax))
  {
    Serial.println("Joinha");
    Mover(sFBFlexMI, 150, pinServoFlexMI, sFBFlexMA, 150, pinServoFlexMA, sFBPol, 0, pinServoPolegar);
    Serial.print("Tempo (ms): ");
    Serial.println(millis() - tUltimoMovimento);
    tUltimoMovimento = millis();
  }
  else if ((fValorSinalFlexor >= movFlexMA && fValorSinalFlexor <= movFlexMAMax) && (fValorSinalExtensor >= movExtMA && fValorSinalExtensor <= movExtMAMax) && (fValorSinalPolegar >= movPolMA && fValorSinalPolegar <= movPolMAMax))
  {
    Serial.println("Flexão Mínimo-Anelar");
    Mover(sFBFlexMI, 0, pinServoFlexMI, sFBFlexMA, 150, pinServoFlexMA, sFBPol, 0, pinServoPolegar);
    Serial.print("Tempo (ms): ");
    Serial.println(millis() - tUltimoMovimento);
    tUltimoMovimento = millis();
  }
  else if ((fValorSinalFlexor >= movFlexPolFlex && fValorSinalFlexor <= movFlexPolFlexMax) && (fValorSinalExtensor >= movExtPolFlex && fValorSinalExtensor <= movExtPolFlexMax) && (fValorSinalPolegar >= movPolPolFlex && fValorSinalPolegar <= movPolPolFlexMax))
  {
    Serial.println("Flexão Polegar");
    Mover(sFBFlexMI, iPosInicial, pinServoFlexMI, sFBFlexMA, iPosInicial, pinServoFlexMA, sFBPol, 150, pinServoPolegar);
    Serial.print("Tempo (ms): ");
    Serial.println(millis() - tUltimoMovimento);
    tUltimoMovimento = millis();
  }
  else if ((fValorSinalFlexor >= movFlexMI && fValorSinalFlexor <= movFlexMIMax) && (fValorSinalExtensor >= movExtMI && fValorSinalExtensor <= movExtMIMax) && (fValorSinalPolegar >= movPolMI && fValorSinalPolegar <= movPolMIMax))
  {
    Serial.println("Flexão Médio-Indicador!");
    Mover(sFBFlexMI, 150, pinServoFlexMI, sFBFlexMA, 0, pinServoFlexMA, sFBPol, 0, pinServoPolegar);
    Serial.print("Tempo (ms): ");
    Serial.println(millis() - tUltimoMovimento);
    tUltimoMovimento = millis();
  }
  else if ((fValorSinalFlexor >= movFlexPolExt && fValorSinalFlexor <= movFlexPolExtMax) && (fValorSinalExtensor >= movExtPolExt && fValorSinalExtensor <= movExtPolExtMax) && (fValorSinalPolegar >= movPolPolExt && fValorSinalPolegar <= movPolPolExtMax))
  {
    Serial.println("Extensão Polegar");
    Mover(sFBFlexMI, iPosInicial, pinServoFlexMI, sFBFlexMA, iPosInicial, pinServoFlexMA, sFBPol, 0, pinServoPolegar);
    Serial.print("Tempo (ms): ");
    Serial.println(millis() - tUltimoMovimento);
    tUltimoMovimento = millis();
  }
  else
  {
    Serial.println("Repouso");
    Mover(sFBFlexMI, iPosInicial, pinServoFlexMI, sFBFlexMA, iPosInicial, pinServoFlexMA, sFBPol, iPosInicial, pinServoPolegar);
    Serial.print("Tempo (ms): ");
    Serial.println(millis() - tUltimoMovimento);
    tUltimoMovimento = millis();
  }
}
