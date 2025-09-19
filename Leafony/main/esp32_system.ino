void setupBootButton() {
  pinMode(BOOT_BUTTON, INPUT);
  systemLog("BUTTON", "OK", 1, 1);
}

void timerSleep(const int& ms) {
  systemLog("SLEEP", String(ms) + " ms",3,4);
  esp_sleep_enable_timer_wakeup(ms * 1000ULL);  // ミリ秒単位
  esp_deep_sleep_start();
}
void buttonSleep(){
  systemLog("SLEEP","Until button pressed",3,4);
  esp_sleep_enable_ext0_wakeup((gpio_num_t)BOOT_BUTTON,LOW);
  esp_deep_sleep_start();
}

void bootReason() {
  esp_reset_reason_t reason = esp_reset_reason();
  const char* title = "Boot reason";
  logln();
  switch (reason) {
    case ESP_RST_UNKNOWN:
      systemLog(title, "Reset reason can not be determined");
      break;
    case ESP_RST_POWERON:
      systemLog(title, "Reset due to power-on event");
      break;
    case ESP_RST_EXT:
      systemLog(title, "Reset by external pin (not applicable for ESP32)");
      break;
    case ESP_RST_SW:
      systemLog(title, "Software reset via esp_restart");
      break;
    case ESP_RST_PANIC:
      systemLog(title, "Software reset due to exception/panic");
      break;
    case ESP_RST_INT_WDT:
      systemLog(title,
                "Reset (software or hardware) due to interrupt watchdog");
      break;
    case ESP_RST_TASK_WDT:
      systemLog(title, "Reset due to task watchdog");
      break;
    case ESP_RST_WDT:
      systemLog(title, "Reset due to other watchdogs");
      break;
    case ESP_RST_DEEPSLEEP:
      systemLog(title, "Reset after exiting deep sleep mode");
      break;
    case ESP_RST_BROWNOUT:
      systemLog(title, "Brownout reset (software or hardware)");
      break;
    case ESP_RST_SDIO:
      systemLog(title, "Reset over SDIO");
      break;
    default:
      systemLog(title, "Unknown reason");
      break;
  }
  logln();
}
