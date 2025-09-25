#include <HTTPClient.h>
#include <HardwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <ctype.h>

#include "HttpMethod.h"

// ====================================================================================================
// Objects & Variables
// ====================================================================================================

// == ESP32 ==
#define BOOT_BUTTON 0  // D3 IO0
#define INT_1 27
#define TIMER_SLEEP_MINUTES 15
esp_reset_reason_t err;
esp_sleep_wakeup_cause_t eswc;
const int testMode = 2;  // 0_OFF, 1_ALL,2_NORMAL, 3_ERROR, 4_IMPORTANT, 5_NONE
const char* symbol = ".-=*#/!+";  // 0. 1- 2= 3* 4# 5/ 6! 7+
// Touch Sensor
#define TOUCH_PIN T5  // IO12
#define TOUCH_THRESHOLD 30
volatile bool touched = false;
void IRAM_ATTR onTouch() { touched = true; }  // 割り込みハンドラ
// WiFi
#define BOUNDARY "----ESP32FormBoundary"
#define WiFi_TIMEOUT_SECOND 15
#define HTTP_RESPONSE_TIMEOUT_SECOND 150
const char* ssid = "04F-Pluslab";                           // SSID
const char* password = "bearspooh";                           // PASS
const String serverUrl = "https://mimamo-leafony.onrender.com";  // Server URL

// == MIC & VR & LED ==
// #define MIC_VR_LED //リーフの利用

// == RTC & microSD ==
// SD
bool useSD = true;  // SDカードのフラグ trueで利用
File dataFile;

// == MIC & AMP ==
#define AUDIO_FILE_NAME "mic_data.bin"
#define MIC 36                // IO36
#define KHZ 16                // サンプリング周波数
#define MIC_BUFFER_SIZE 2048  // マイクバッファサイズ
uint16_t mic_buffer[MIC_BUFFER_SIZE] = {0};
uint16_t mic_buf_idx = 0;
uint16_t dataMIC = 0;

//====================================================================================================
// Initialization & Start
//====================================================================================================

void setup() {
  setupLeafony();
  startLeafony();
}

//====================================================================================================

void loop() {
  
  if (touched) {  // touched
    touched = false;
    systemLog("TOUCH", "Detected");

    speak("tsutaetai/messe-jiwo/oshietene.");
    countDown(3);
    recordMICbuffered(KHZ, 10);  // 録音
    speak(sendMessage());

    sleepLeafony();
  }
}

// ====================================================================================================
// initialization
// ====================================================================================================

void setupLeafony() {
  if (testMode) Serial.begin(115200);
  analogReadResolution(12);        // 12bit (0-4095)
  analogSetAttenuation(ADC_11db);  // フルレンジ3.3V
  logln();
  err = bootReason();
  eswc = wakeupCause();
  logln();
  
  systemLog("SETUP", "START");
  setupBootButton();  // BOOTBUTTON
  setupTouch(TOUCH_PIN, TOUCH_THRESHOLD);
  setupLED();      // LED
  setupMIC();      // MIC
  setupSpeaker();  // SPEAKER
  connectWiFi();   // WiFi
  setupSD();       // SD CARD
  systemLog("SETUP", "FINISH");
  logln();
}

//====================================================================================================

void startLeafony() {
  systemLog("mimamoLeafony", "START", 4, 4);
  chime();

  if (eswc == ESP_SLEEP_WAKEUP_TOUCHPAD) {
    if (checkNotices()) {
      speak("messe-ji/a'ri.");

      for (int i = 1;; i++) {
        String message = getMessage();
        if (message == "") {
          break;
        }
        speak("<NUMK VAL=" + String(i) + " COUNTER=kenn>+me");
        speak(message);
      }
      speak("messe-jiwo/okuruniwa.");
      speak("tacchisitekudasai.");
    }
    else {
      speak("dengon/na'shi.");

      speak("tsutaetai/messe-jiwo/oshietene.");
      countDown(3);
      recordMICbuffered(KHZ, 10);  // 録音
      speak(sendMessage());
      sleepLeafony();
    }
    return;
  }
  else if (eswc == ESP_SLEEP_WAKEUP_TIMER) {
    if(checkIsOutside()){
      sleepLeafony(0);  // Finish & Sleep
    }
    if (checkNotices()) {
      speak("messe-jiga/arimasu.");
    } 
    sleepLeafony();  // Finish & Sleep
  }
  else {
    sleepLeafony();  // Finish & Sleep
  }
}

// ====================================================================================================
void sleepLeafony(){
  sleepLeafony(TIMER_SLEEP_MINUTES);
}
void sleepLeafony(int sleepMinutes) {
  systemLog("mimamoLeafony", "SLEEP", 4, 4);
  // speak("dhi-pu/suri-pu'/mo'-do.");
  chime(true);
  chime(false);
  disconnectWiFi(true);
  turnOffSpeaker();
  turnOffLED();
  
  if(sleepMinutes>0){
    timerSleep(sleepMinutes * 60 * 1000ULL);
  }
  touchSleep(TOUCH_PIN, TOUCH_THRESHOLD);
  esp_deep_sleep_start();
}
