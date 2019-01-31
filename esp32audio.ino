#include <Arduino.h>
#include <WiFi.h>
#include <driver/adc.h>

#define AUDIO_BUFFER_MAX 512

uint8_t audioBuffer[AUDIO_BUFFER_MAX];
uint8_t transmitBuffer[AUDIO_BUFFER_MAX];
uint32_t bufferPointer = 0;

const char* ssid     = "SuhankoFamily";
const char* password = "fsjmr112";
const char* host     = "192.168.1.2";

bool transmitNow = false;

WiFiClient client;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux); // para rodar um código crítico sem ser interrompido.
  int adcVal = adc1_get_voltage(ADC1_CHANNEL_0); // faz a leitura do ADC
  if (adcVal > 3800){ //3500 sem gain em gnd
    adcVal = 4095;
  }
  uint8_t value = map(adcVal, 0 , 4095, 0, 255);  // mapeamento para 8 bits
  audioBuffer[bufferPointer] = value; // armazenamento do valor
  bufferPointer++;

 // Ação no preenchimento do buffer
  if (bufferPointer == AUDIO_BUFFER_MAX) {
    bufferPointer = 0;
    memcpy(transmitBuffer, audioBuffer, AUDIO_BUFFER_MAX); // transfere o buffer
    transmitNow = true; // flag para envio do buffer
  }
  portEXIT_CRITICAL_ISR(&timerMux); // prioridade no código crítico
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("MY IP address: ");
  Serial.println(WiFi.localIP());

  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_11db); //ADC 1 canal 0 GPIO36

  const int port = 8888;
  while (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(1000);
  }

  Serial.println("connected to server");

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 125, true);
  timerAlarmEnable(timer);

}

void loop() {
  // aguarda pela ordem de envio
  if (transmitNow) {
    transmitNow = false;
    client.write((const uint8_t *)audioBuffer, sizeof(audioBuffer));
  }
}

