// // == 4-Sensor ==
// // Accelerometer
// #define ACC_ADDRESS 0x19
// #define ACC_RATE LIS3DH_DATARATE_100_HZ
// #define ACC_TOUCH 1  // 0_OFF, 1_SINGLE, 2_DOUBLE
// #define ACC_TH 60    // 40-80 (MAX 127)
// Adafruit_LIS3DH accel = Adafruit_LIS3DH();

// void setupAccel() {
//   bool setAcc = false;
//   for (int i = 0; i < 5; i++) {
//     if (accel.begin(ACC_ADDRESS)) {
//       setAcc = true;
//       break;
//     }
//     delay(10);
//   }
//   if (!setAcc) progStop("Accelerometer Not Found");

//   accel.setRange(LIS3DH_RANGE_2_G);  // Full scale +/- 2G
//   accel.setDataRate(ACC_RATE);       // Data rate

//   setupTapInterrupt();
//   systemLog("ACC", "OK", 1, 1);
// }

// void setupTapInterrupt() {
//   // pinMode(INT_1, INPUT_PULLDOWN);              // 割り込み用ピン
//   accel.setClick(ACC_TOUCH, ACC_TH);  // タッチ検出　シングルタップ，閾値
//   attachInterrupt(digitalPinToInterrupt(INT_1), tap, RISING);
//   tapped = false;
// }

// void showAccel() {
//   accel.read();

//   Serial.print("X [g] = " + String(accel.x_g));
//   Serial.print(", ");
//   Serial.print("Y [g] = " + String(accel.y_g));
//   Serial.print(", ");
//   Serial.print("Z [g] = " + String(accel.z_g));
//   Serial.println("");
// }

// void powerDownAccel() { accel.setDataRate(LIS3DH_DATARATE_POWERDOWN); }