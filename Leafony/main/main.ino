#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define ADC_PIN   34
#define SD_CS     5
#define SAMPLE_RATE 16000
#define DURATION    5

// WAVヘッダを書き込み
void writeWavHeader(File &file, uint32_t sampleRate, uint16_t bitsPerSample, uint16_t channels, uint32_t numSamples) {
  uint32_t byteRate = sampleRate * channels * bitsPerSample / 8;
  uint16_t blockAlign = channels * bitsPerSample / 8;
  uint32_t dataSize = numSamples * channels * bitsPerSample / 8;
  uint32_t chunkSize = 36 + dataSize;

  file.write((const uint8_t *)"RIFF", 4);
  file.write((uint8_t *)&chunkSize, 4);
  file.write((const uint8_t *)"WAVE", 4);

  file.write((const uint8_t *)"fmt ", 4);
  uint32_t subChunk1Size = 16;  // PCM
  uint16_t audioFormat = 1;     // PCM
  file.write((uint8_t *)&subChunk1Size, 4);
  file.write((uint8_t *)&audioFormat, 2);
  file.write((uint8_t *)&channels, 2);
  file.write((uint8_t *)&sampleRate, 4);
  file.write((uint8_t *)&byteRate, 4);
  file.write((uint8_t *)&blockAlign, 2);
  file.write((uint8_t *)&bitsPerSample, 2);

  file.write((const uint8_t *)"data", 4);
  file.write((uint8_t *)&dataSize, 4);
}

void setup() {
  Serial.begin(115200);

  if (!SD.begin(SD_CS)) {
    Serial.println("SD Card mount failed");
    while (1);
  }

  File file = SD.open("/record.wav", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file");
    while (1);
  }

  // サンプル数を事前に計算
  uint32_t numSamples = SAMPLE_RATE * DURATION;

  // ヘッダを書き込み
  writeWavHeader(file, SAMPLE_RATE, 16, 1, numSamples);

  Serial.println("Recording...");

  unsigned long startMicros = micros();
  unsigned long sampleInterval = 1000000UL / SAMPLE_RATE;

  for (unsigned long i = 0; i < numSamples; i++) {
    int val = analogRead(ADC_PIN);
    int16_t sample = map(val, 0, 4095, -32768, 32767);

    file.write((uint8_t *)&sample, 2);

    while (micros() - startMicros < sampleInterval);
    startMicros += sampleInterval;
  }

  file.close();
  Serial.println("Recording finished");
}

void loop() {}
