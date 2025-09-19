// == MIC & VR & LED Leaf (LED2, VR以外は干渉のため禁止) ==
// #define LED1 14  // D6  IO14 -SD
#define LED2 15  // D7  IO15
// #define LED3 17  // D8  IO17 -SPK
// #define LED4 16  // D9  IO16 -SPK
// #define LED5 5   // D10 IO5 -SD
// #define LED6 23  // D11 IO23 -SD
// #define MIC 26 //A0 IO26
#define VR 39  // A3  IO39

void setupLED() {
#ifdef LED1
  pinMode(LED1, OUTPUT);
#endif
#ifdef LED2
  pinMode(LED2, OUTPUT);
#endif
#ifdef LED3
  pinMode(LED3, OUTPUT);
#endif
#ifdef LED4
  pinMode(LED4, OUTPUT);
#endif
#ifdef LED5
  pinMode(LED5, OUTPUT);
#endif
#ifdef LED6
  pinMode(LED6, OUTPUT);
#endif
  turnOffLED();
  systemLog("LED", "OK", 1, 1);
}
void turnOnLED(const int &ledNum) { digitalWrite(ledNum, HIGH); }
void turnOffLED(const int &ledNum) { digitalWrite(ledNum, LOW); }
void turnOnLED() {
#ifdef LED1
  turnOnLED(LED1);
#endif
#ifdef LED2
  turnOnLED(LED2);
#endif
#ifdef LED3
  turnOnLED(LED3);
#endif
#ifdef LED4
  turnOnLED(LED4);
#endif
#ifdef LED5
  turnOnLED(LED5);
#endif
#ifdef LED6
  turnOnLED(LED6);
#endif
}
void turnOffLED() {
#ifdef LED1
  turnOffLED(LED1);
#endif
#ifdef LED2
  turnOffLED(LED2);
#endif
#ifdef LED3
  turnOffLED(LED3);
#endif
#ifdef LED4
  turnOffLED(LED4);
#endif
#ifdef LED5
  turnOffLED(LED5);
#endif
#ifdef LED6
  turnOffLED(LED6);
#endif
}
void blinkLED(const int &times, const int &interval) {
  for (int i = 0; i < times; i++) {
    turnOnLED();
    delay(interval / 2);
    turnOffLED();
    delay(interval / 2);
  }
}