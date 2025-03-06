/***********
* ESP8266 WiFi add-on for SPRESENSE
* LINE Messaging API Send Message Library
* Copyright(c) @tomorrow56
* All rights reserved.
* ESP8266 baudrate: 115200 baud(fixed)
**********/

#include "LineMessenger.h"

LineMessenger::LineMessenger() {
  accessToken = nullptr; // 初期値
}

void LineMessenger::setAccessToken(const char* token) {
  accessToken = token;
}

bool LineMessenger::connectWiFi(const char* ssid, const char* password) {
  // ESP8266リセット
  if (!sendCommand("AT+RST", 2000)) return false;
  // ステーションモード設定
  if (!sendCommand("AT+CWMODE=1", 1000)) return false;
  // WiFi接続
  String connectCmd = "AT+CWJAP=\"";
  connectCmd += ssid;
  connectCmd += "\",\"";
  connectCmd += password;
  connectCmd += "\"";
  return sendCommand(connectCmd.c_str(), 10000);
}

bool LineMessenger::sendMessage(const char* message) {
  // TCP接続開始
  String cipStart = "AT+CIPSTART=\"TCP\",\"";
  cipStart += host;
  cipStart += "\",443";
  if (!sendCommand(cipStart.c_str(), 5000)) {
    Serial.println("TCP connection failed");
    return false;
  }

  // JSONペイロード構築
  String payload = "{\"messages\":[{\"type\":\"text\",\"text\":\"";
  payload += message;
  payload += "\"}]}";

  // HTTPリクエスト構築
  String request = "POST /v2/bot/message/broadcast HTTP/1.1\r\n";
  request += "Host: ";
  request += host;
  request += "\r\n";
  request += "Authorization: Bearer ";
  request += accessToken;
  request += "\r\n";
  request += "Content-Type: application/json\r\n";
  request += "Content-Length: ";
  request += String(payload.length());
  request += "\r\n\r\n";
  request += payload;

  // データ長指定
  String cipSend = "AT+CIPSEND=";
  cipSend += String(request.length());
  if (!sendCommand(cipSend.c_str(), 2000)) {
    Serial.println("CIPSEND failed");
    return false;
  }

  // リクエスト送信
  bool success = sendCommand(request.c_str(), 5000);
  if (success) {
    Serial.println("Message sent successfully");
  } else {
    Serial.println("Failed to send message");
  }

  // 接続終了
  sendCommand("AT+CIPCLOSE", 1000);
  return success;
}

bool LineMessenger::sendCommand(const char* command, int timeout) {
  Serial2.println(command);
  Serial.print("Sent: ");
  Serial.println(command);
  long start = millis();
  bool success = false;
  while (millis() - start < timeout) {
    if (Serial2.available()) {
      String response = Serial2.readStringUntil('\n');
      Serial.println(response);
      if (response.indexOf("OK") != -1) success = true;
      else if (response.indexOf("ERROR") != -1 || response.indexOf("FAIL") != -1) return false;
    }
  }
  delay(100);
  return success;
}