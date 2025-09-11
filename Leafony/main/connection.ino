void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// JSONデータの送信
void sendJson(String path, String json) {
  HTTPClient http;
  http.begin(serverUrl + path);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(json);
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  http.end();
}

void sendMessage(unsigned int* buffer, size_t len) {
  String json = "{\"audio_data\":[";
  for (size_t i = 0; i < len; i++) {
    json += String(buffer[i]);
    if (i < len - 1) json += ",";
  }
  json += "]}";
  connectWiFi();
  sendJson("/api/device/messages", json);
  mic_buf_idx = 0;
  WiFi.disconnect(true);  // WiFi切断
}
