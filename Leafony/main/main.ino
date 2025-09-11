#include <HTTPClient.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>

// MIC&VR&LED (LED1,5,6はSD干渉のため利用禁止)
// #define LED1 14  // D6  IO14
#define LED2 15  // D7  IO15
#define LED3 17  // D8  IO17
#define LED4 16  // D9  IO16
// #define LED5 5   // D10 IO5
// #define LED6 23  // D11 IO23
#define MIC 26  // A2  IO26
#define VR 39   // A3  IO39

#define KHZ 16               // 16kHz
#define MIC_BUFFER_SIZE 512  // マイクバッファサイズ

// RTC&microSD
// #define SD_EN 14  // D6  IO14
// #define SS 5      // D10 IO5
// #define MOSI 23   // D11 IO23
// #define MISO 19   // D12 IO19
// #define SCK 18    // D13 IO18

// WiFi Settings
const char* ssid = "04F-Pluslab";
const char* password = "bearspooh";
// Server URL
String serverUrl = "https://mimamo-leafony.onrender.com";

// MIC
unsigned int dataMIC;
unsigned int mic_buffer[MIC_BUFFER_SIZE] = {0};
unsigned int mic_buf_idx = 0;
// SD
File dataFile;

void setup() {
  Serial.begin(115200);
  analogReadResolution(11);        // 12bit (0-4095)
  analogSetAttenuation(ADC_11db);  // フルレンジ3.3V

  Serial.println("----- Start : Leafony -----");
  bootReason();
  setupAllLED();
  setupMIC();
  // connectWiFi();
  setupSD();
  fileOpen("/micdata.csv");

  countDown(3);
  record_MIC(KHZ, 10);  // 10秒間録音
  // WiFi.mode(WIFI_STA);
  fileClose();
}

void loop() {}
