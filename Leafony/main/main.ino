#include <HTTPClient.h>
#include <HardwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>

// == MIC & AMP ==
#define MIC 36               // IO36
#define KHZ 16               // 16kHz
#define MIC_BUFFER_SIZE 512  // マイクバッファサイズ
unsigned int dataMIC = 0;
unsigned int mic_buffer[MIC_BUFFER_SIZE] = {0};
unsigned int mic_buf_idx = 0;

// == ATP3012 & SPEAKER ==
#define AT_baudrate 9600      // 既定値9600bps
#define AT_RX 16              // D9 IO16 - ESP32 RX（ATP3012 TXD）
#define AT_TX 17              // D8 IO17 - ESP32 TX（ATP3012 RXD）
HardwareSerial AquesTalk(2);  // AT3012 UART2接続

// == ESP32 ==
#define BOOT_BUTTON 0  // D3 IO0
const int testMode = 2;// 0_OFF, 1_ALL,NORMAL, 3_ERROR, 4_IMPORTANT, 5_NONE
const char* symbol = ".-=*#/!+";  // 0. 1- 2= 3* 4# 5/ 6! 7+
// WiFi
const char* ssid = "04F-Pluslab";    // SSID  04F-Pluslab
const char* password = "bearspooh";  // PASS  bearspooh
const String serverUrl = "https://mimamo-leafony.onrender.com"; // Server URL

// SD
File dataFile;

void setup() { setupLeafony(); }

void loop() {
  if (!digitalRead(BOOT_BUTTON)) {
    countDown(3);
    // recordMICbufferd(KHZ, 10);  // 10秒間録音
    sleepLeafony();
  }
}

void setupLeafony() {
  if (testMode) Serial.begin(115200);
  analogReadResolution(12);        // 12bit (0-4095)
  analogSetAttenuation(ADC_11db);  // フルレンジ3.3V

  bootReason();
  systemLog("SETUP", "START",2,2);
  setupBootButton();  // BOOTBUTTON
  setupLED();      // LED
  setupMIC();         // MIC
  setupSpeaker();     // SPEAKER
  setupSD();
  // fileOpen("/micdata.csv");
  // connectWiFi();
  systemLog("SETUP", "FINISH",2,2);
  logln();
  systemLog("mimamoLeafony", "START", 4, 4);
  chime();
  speak("mimamori'-foni-.");
}
void sleepLeafony() {
  systemLog("mimamoLeafony", "SLEEP", 4, 4);
  chime(true);
  chime(false);
  speak("dhi-pu/suri'-pu'/mo'-do.");
  
  // fileClose();
  // disconnectWiFi();
  turnOffLED();
  buttonSleep();
  // timerSleep(10 * 1000);
}
