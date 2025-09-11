void progStop() {
  Serial.println("プログラム終了：リセットしてください");
  while (1) {
    blinkAllLED(1, 200);
  }
}

void timerSleep(int ms) {
  Serial.println("Going to sleep for " + String(ms) + " ms");
  turnOffAllLED();
  delay(100);
  esp_sleep_enable_timer_wakeup(ms * 1000);  // ミリ秒単位
  esp_deep_sleep_start();
}

void countDown(int count) {
  for (int i = count; i > 0; i--) {
    Serial.println(String(i) + "...");
    blinkAllLED(1, 500);
    delay(500);
  }
  Serial.println("Start!");
}

void bootReason() {
  esp_reset_reason_t reason = esp_reset_reason();
  Serial.print("Boot reason: ");
  Serial.println("");
  switch (reason) {
    case ESP_RST_UNKNOWN:
      Serial.println("Reset reason can not be determined");
      break;
    case ESP_RST_POWERON:
      Serial.println("Reset due to power-on event");
      break;
    case ESP_RST_EXT:
      Serial.println("Reset by external pin (not applicable for ESP32)");
      break;
    case ESP_RST_SW:
      Serial.println("Software reset via esp_restart");
      break;
    case ESP_RST_PANIC:
      Serial.println("Software reset due to exception/panic");
      break;
    case ESP_RST_INT_WDT:
      Serial.println("Reset (software or hardware) due to interrupt watchdog");
      break;
    case ESP_RST_TASK_WDT:
      Serial.println("Reset due to task watchdog");
      break;
    case ESP_RST_WDT:
      Serial.println("Reset due to other watchdogs");
      break;
    case ESP_RST_DEEPSLEEP:
      Serial.println("Reset after exiting deep sleep mode");
      break;
    case ESP_RST_BROWNOUT:
      Serial.println("Brownout reset (software or hardware)");
      break;
    case ESP_RST_SDIO:
      Serial.println("Reset over SDIO");
      break;
    default:
      Serial.println("Unknown reason");
      break;
  }
  Serial.println("");
}
