void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  logln();
  systemLog("WiFi", "Connected", 1, 1);
  systemLog("IP address", String(WiFi.localIP()),1,2);
}
void disconnectWiFi() {
  WiFi.disconnect(true);
  systemLog("WiFi", "disconnected",1,1);
}
// JSONデータの送信
void sendJson(const String &path, const String &json) {
  HTTPClient http;
  http.begin(serverUrl + path);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(json);
  systemLog("HTTP Response code", httpResponseCode,1,1);
  http.end();
}
