void timerSleep(unsigned long ms) {
  systemLog("SLEEP-MODE", "TIMER - " + String(ms) + " ms", 3, 4);
  esp_sleep_enable_timer_wakeup(ms * 1000ULL);  // ミリ秒単位
}

void touchSleep(uint8_t touchPin, uint16_t threshold) {
  systemLog("SLEEP-MODE", "TOUCH", 3, 4);
  touchRead(touchPin);  // 初期値リセット
}

void ext0Sleep(int gpio, int level) {
  esp_sleep_enable_ext0_wakeup((gpio_num_t)gpio, level);
}

void interruptSleep() {
  systemLog("SLEEP-MODE", "INTERRUPT", 3, 4);
  ext0Sleep(INT_1, HIGH);
}

void buttonSleep() {
  systemLog("SLEEP-MODE", "BOOT-BUTTON", 3, 4);
  ext0Sleep(BOOT_BUTTON, LOW);
}

esp_sleep_wakeup_cause_t wakeupCause() {
  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
  const char* title = "WakeUpCause";
  switch (cause) {
    case ESP_SLEEP_WAKEUP_UNDEFINED:
      systemLog(title, "Undefined (power-on event)");
      break;
    case ESP_SLEEP_WAKEUP_EXT0:
      systemLog(title, "EXT0");
      break;
    case ESP_SLEEP_WAKEUP_EXT1:
      systemLog(title, "EXT1");
      break;
    case ESP_SLEEP_WAKEUP_TIMER:
      systemLog(title, "TIMER");
      break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
      systemLog(title, "TOUCHPAD");
      break;
    case ESP_SLEEP_WAKEUP_ULP:
      systemLog(title, "ULP");
      break;
    default:
      systemLog(title, "Unknown reason");
      break;
  }
  return cause;
}