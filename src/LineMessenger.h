/***********
* ESP8266 WiFi add-on for SPRESENSE
* LINE Messaging API Send Message Library
* Copyright(c) @tomorrow56
* All rights reserved.
* ESP8266 baudrate: 115200 baud(fixed)
**********/

#ifndef LineMessenger_h
#define LineMessenger_h

#include "Arduino.h"

class LineMessenger {
public:
  LineMessenger(); // コンストラクタ
  bool connectWiFi(const char* ssid, const char* password); // WiFi接続
  bool sendMessage(const char* message);       // メッセージ送信
  void setAccessToken(const char* token);      // アクセストークン設定

private:
  const char* host = "api.line.me";            // LINE APIホスト
  const char* accessToken;                     // LINEアクセストークン
  bool sendCommand(const char* command, int timeout); // ATコマンド送信
};

#endif