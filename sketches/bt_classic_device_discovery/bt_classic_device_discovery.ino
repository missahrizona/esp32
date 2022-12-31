#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include "WiFiCredentials.h"
#include "UUID.h"

using namespace websockets;

WebsocketsClient client;

StaticJsonDocument<256> payload;

UUID uuid;

byte mac[6];

unsigned long ms;

bool websocketConnected;

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  WiFi.macAddress(mac);
  payload["mac_addr"] =  String(mac[0],HEX) + ":" + String(mac[1],HEX) + ":" + String(mac[2],HEX) + ":" + String(mac[3],HEX) + ":" + String(mac[4],HEX) + ":" + String(mac[5],HEX);
  payload["uuid"] = uuid.toCharArray();

  client.setCACert(ssl_ca_cert);
  websocketConnected = client.connect("wss://king-prawn-app-4cz2q.ondigitalocean.app:443");
  // client.setInsecure();
  // bool connected = client.connect("ws://192.168.1.129:8080");

  // get chipId
  uint32_t chipId = 0;
  for(int i=0; i<17; i=i+8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  payload["chip_id"] = String(chipId);

}

void loop() {
  ms = millis();
  if ((WiFi.status() == WL_CONNECTED) && websocketConnected) {
    payload["datapoint"] =  "Message from ESP32";
    payload["current_ms"] = ms;
    char jsonStr[256];
    serializeJson(payload, jsonStr);
    client.send(jsonStr);
  }

  while (true)
  delay(569);

}
