// LED1,LED5,LED6
// SDカードとピン番号干渉のため利用禁止

void setupAllLED() {
  // pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  // pinMode(LED5, OUTPUT);
  // pinMode(LED6, OUTPUT);
  turnOffAllLED();
}
void turnOnLED(int ledNumber) { digitalWrite(ledNumber, HIGH); }
void turnOffLED(int ledNumber) { digitalWrite(ledNumber, LOW); }
void turnOnAllLED() {
  // turnOnLED(LED1);
  turnOnLED(LED2);
  turnOnLED(LED3);
  turnOnLED(LED4);
  // turnOnLED(LED5);
  // turnOnLED(LED6);
}
void turnOffAllLED() {
  // turnOffLED(LED1);
  turnOffLED(LED2);
  turnOffLED(LED3);
  turnOffLED(LED4);
  // turnOffLED(LED5);
  // turnOffLED(LED6);
}
void blinkAllLED(int times, int interval) {
  for (int i = 0; i < times; i++) {
    turnOnAllLED();
    delay(interval);
    turnOffAllLED();
    delay(interval);
  }
}