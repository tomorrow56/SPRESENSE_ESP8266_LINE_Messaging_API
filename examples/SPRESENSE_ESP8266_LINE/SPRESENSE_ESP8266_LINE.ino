/***********
* ESP8266 WiFi add-on for SPRESENSE
* LINE Messaging API Send Message Library
* Copyright(c) @tomorrow56
* All rights reserved.
* ESP8266 baudrate: 115200 baud
**********/

#include "LineMessenger.h"

// WiFiとLINEの設定
const char* ssid = "<your SSID>";
const char* password = "<your Password>";
// 以下からLINEチャネルアクセストークンを取得する
// https://manager.line.biz/
const char* accessToken = "<your LINE Access Token>";

// ライブラリインスタンス作成
LineMessenger line;

void setup() {
  Serial.begin(115200);    // デバッグ用シリアル
  Serial2.begin(115200);   // ESP8266用シリアル
  delay(100);

  line.setAccessToken(accessToken); // アクセストークン設定
  if (line.connectWiFi(ssid, password)) {
    Serial.println("WiFi connected");
    line.sendMessage("ライブラリ化のテスト"); // メッセージ送信
  } else {
    Serial.println("WiFi connection failed");
  }
}

void loop() {
  // 必要に応じて繰り返し送信
  // line.sendMessage("テスト");
  // delay(10000);
}