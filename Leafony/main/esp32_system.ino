void setupBootButton() {
  pinMode(BOOT_BUTTON, INPUT);
  systemLog("BUTTON", "OK", 1, 1);
}
void setupTouch(uint8_t touchPin, uint16_t threshold) {
  esp_sleep_enable_touchpad_wakeup();  // タッチセンサーをウェイクアップに設定
  touchAttachInterrupt(touchPin, onTouch, threshold);  // 割り込み設定
  systemLog("TOUCH", "OK" + touchRead(touchPin), 1, 1);
}

esp_reset_reason_t bootReason() {
  esp_reset_reason_t reason = esp_reset_reason();
  const char* title = "Boot Reason";
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
      systemLog(title, "Reset via esp_restart");
      break;
    case ESP_RST_PANIC:
      systemLog(title, "Reset due to exception/panic");
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
  return reason;
}
