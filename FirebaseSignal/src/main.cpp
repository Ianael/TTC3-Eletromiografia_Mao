#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include "time.h"
#include "esp_adc_cal.h"

#pragma region Variáveis FireBase
// Lib para informações da geração de Token..
#include "addons/TokenHelper.h"
// RealtimeDB payload e info
#include "addons/RTDBHelper.h"

// Credenciais da rede
#define WIFI_SSID "NOME_WIFI"
#define WIFI_PASSWORD "SENHA_WIFI"

// Chave Firebase
#define API_KEY "CHAVE_API"

// Email e Senha para Login no site
#define USER_EMAIL "EMAIL_USUARIO"
#define USER_PASSWORD "SENHA_USUARIO"

// URL do site
#define DATABASE_URL "https://teste-myoware-default-rtdb.firebaseio.com/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String uid;

// Caminho DB
String databasePath;

int timestamp;
FirebaseJson json;
FirebaseJsonArray arr;

// Cluster de servidores públicos para consulta de tempo
const char *ntpServer = "pool.ntp.org";

// Timers
unsigned long tEnvioAnterior = 0;
unsigned long timerDelay = 150; // Envia dados a cada 150ms
#pragma endregion

#pragma region Variáveis Captação
const int pinFlexor = 34;
const int pinExtensor = 32;
const int pinPolegar = 35;

// Variáveis de leitura
float fValorSinalFlexor;
float fValorSinalExtensor;
float fValorSinalPolegar;
#pragma endregion

// Inicia conexão WiFi
void initWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando ao WiFi ...");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }

  Serial.println(WiFi.localIP());
  Serial.println();
}

// Pega o tempo atual -> epoch time
unsigned long getTime()
{
  time_t now;
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo))
  {
    return (0);
  }

  time(&now);
  return now;
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

  initWiFi();

#pragma region Define configurações e inicia conexão com o Firebase
  configTime(0, 0, ntpServer);

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5; // Tentativas máxima para a geração do token

  Firebase.begin(&config, &auth);

  Serial.println("Obtendo UID do Usuário");
  while ((auth.token.uid) == "")
  {
    Serial.print('.');
    delay(1000);
  }

  uid = auth.token.uid.c_str();
  Serial.print("UID: ");
  Serial.println(uid);
  Serial.println();

  databasePath = "/UsersData/" + uid + "/readings";
#pragma endregion

  pinMode(pinFlexor, INPUT);
  pinMode(pinExtensor, INPUT);
  pinMode(pinPolegar, INPUT);

  delay(4000);
}

void loop()
{
  // Delay necessário entre captações, também representa tempo do pré sinal de acionamento dos músculos antes do movimento
  delay(150);

  // Envia novos dados se diferença entre tEnvioAnterior e timer atual for maior que timerDelay definido
  if (Firebase.ready() && (millis() - tEnvioAnterior > timerDelay || tEnvioAnterior == 0))
  {
    tEnvioAnterior = millis();

    fValorSinalFlexor = analogRead(pinFlexor);
    fValorSinalExtensor = analogRead(pinExtensor);
    fValorSinalPolegar = analogRead(pinPolegar);

    // Converte leitura analógica para mV
    // Sensor Myoware -> Valor rectificado + sinal EMG integrado (0-4095), com precisão de 12bits (4096)
    fValorSinalFlexor = ((fValorSinalFlexor / 4095) * 3.3) * 1000;
    fValorSinalExtensor = ((fValorSinalExtensor / 4095) * 3.3) * 1000;
    fValorSinalPolegar = ((fValorSinalPolegar / 4095) * 3.3) * 1000;

    Serial.println();
    timestamp = getTime();
    Serial.print("time: ");
    Serial.print(timestamp);
    Serial.print(" - Flex: ");
    Serial.print(fValorSinalFlexor);
    Serial.print("mV");
    Serial.print(" - Ext: ");
    Serial.print(fValorSinalExtensor);
    Serial.print("mV");
    Serial.print(" - Pol: ");
    Serial.print(fValorSinalPolegar);
    Serial.println("mV");
    // Serial.print(analogReadMilliVolts(pinFlexor));
    // Serial.println(adc1_get_raw(ADC1_CHANNEL_6));

    json.setJsonData("{\"FlexEmg\":" + String(fValorSinalFlexor) + ", \"ExtEmg\":" + String(fValorSinalExtensor) + ", \"PolEmg\":" + String(fValorSinalPolegar) + ", \"timestamp\":" + String(timestamp) + "}");
    arr.add(json);

    Serial.printf("Enviando json... %s\n", Firebase.RTDB.setArray(&fbdo, databasePath.c_str(), &arr) ? "ok" : fbdo.errorReason().c_str());
  }
}
