void connectWiFi() {
  WiFi.begin(ssid, password);
  static unsigned long millis_buf = millis();
  systemLog("WiFi", "Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - millis_buf > WiFi_TIMEOUT_SECOND * 1000) {
      systemLog("WiFi", "Connection Failed", 3, 3);
      sleepLeafony();
    }
  }
  systemLog("WiFi", "Connected", 1, 1);
  systemLog("IP address", String(WiFi.localIP()), 1, 2);
}

void disconnectWiFi() { disconnectWiFi(true); }
void disconnectWiFi(bool t) {
  WiFi.disconnect(t);
  systemLog("WiFi", "disconnected", 1, 1);
}

String sendRequestJson(HttpMethod method, const String &path) {
  return sendRequestJson(method, path, "");
}
String sendRequestJson(HttpMethod method, const String &path,
                       const String &body) {
  HTTPClient httpClient;
  String payload = "";
  int httpResponseCode = -1;
  httpClient.begin(serverUrl + path);
  httpClient.addHeader("Accept", "application/json");

  switch (method) {
    case HTTP_GET:
      httpResponseCode = httpClient.GET();
      break;
    case HTTP_POST:
      httpClient.addHeader("Content-Type", "application/json");
      httpResponseCode = httpClient.POST(body);
      break;
    case HTTP_PATCH:
      httpClient.addHeader("Content-Type", "application/json");
      httpResponseCode = httpClient.sendRequest("PATCH", body);
      break;
    case HTTP_PUT:
      httpClient.addHeader("Content-Type", "application/json");
      httpResponseCode = httpClient.PUT(body);
      break;
    case HTTP_DELETE:
      httpResponseCode = httpClient.sendRequest("DELETE");
      break;
  }

  if (httpResponseCode > 0) {
    payload = httpClient.getString();
    systemLog("HTTP Response code", httpResponseCode, 1, 1);
    if (payload.length() > 0) {
      systemLog("Response payload", payload, 1, 2);
    }
  } else {
    systemLog("HTTP Request Failed", httpResponseCode, 2, 2);
  }

  httpClient.end();
  return payload;
}

String sendFileMultipart(const String &path, const char *filename) {
  if (!fileOpen(filename, FILE_READ)) return String();  // 空文字で失敗

  // "https://" を削除してホスト名だけ取得
  String host = serverUrl;
  if (host.startsWith("https://"))
    host.remove(0, 8);
  else if (host.startsWith("http://"))
    host.remove(0, 7);

  WiFiClientSecure wifiClient;
  wifiClient.setCACert(NULL);  // 証明書チェック無効化（テスト用）
  if (!wifiClient.connect(host.c_str(), 443)) {
    systemLog("HTTP", "Connect Failed", 2, 2);
    dataFile.close();
    return String();  // 接続失敗
  }

  // マルチパートヘッダ・フッタ
  String head = "--" BOUNDARY
                "\r\n"
                "Content-Disposition: form-data; name=\"file\"; filename=\"" +
                String(filename) +
                "\"\r\n"
                "Content-Type: application/octet-stream\r\n\r\n";
  String tail = "\r\n--" BOUNDARY "--\r\n";

  // POSTリクエスト送信
  wifiClient.print("POST " + path + " HTTP/1.1\r\n");
  wifiClient.print("Host: " + host + "\r\n");
  wifiClient.print("Content-Type: multipart/form-data; boundary=" BOUNDARY
                   "\r\n");
  wifiClient.print("Content-Length: " +
                   String(head.length() + dataFile.size() + tail.length()) +
                   "\r\n");
  wifiClient.print("Connection: close\r\n\r\n");

  // 本文送信（ヘッダ部分）
  wifiClient.print(head);

  // ファイル送信（チャンク単位）
  uint8_t buf[1024];
  while (dataFile.available()) {
    size_t n = dataFile.read(buf, sizeof(buf));
    wifiClient.write(buf, n);
  }
  dataFile.close();

  // フッタ送信
  wifiClient.print(tail);

  // レスポンス受信（タイムアウト付き）
  String response = "";
  unsigned long start = millis();
  while (wifiClient.connected() &&
         millis() - start < HTTP_RESPONSE_TIMEOUT_SECOND * 1000) {
    while (wifiClient.available()) {
      char c = wifiClient.read();
      response += c;
      start = millis();  // データ受信でタイマーリセット
    }
  }
  wifiClient.stop();

  // JSON 部分だけ抜き出す
  String body = extractJsonBody(response);

  if (body.length() > 0) {
    systemLog("HTTP JSON Body", body, 1, 2);  // 本文だけログ出力
    return body;                              // 成功時は文字列を返す
  } else {
    systemLog("HTTP", "No JSON Body", 2, 2);
    return String();  // 失敗時は空文字
  }
}

// JSON 部分だけ抜き出す関数
String extractJsonBody(const String &response) {
  int start = response.indexOf('{');    // 最初の '{' を探す
  int end = response.lastIndexOf('}');  // 最後の '}' を探す
  if (start == -1 || end == -1 || end <= start) return "";
  return response.substring(start, end + 1);
}
