/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

#include "wificlient.h"

WifiClient::WifiClient(Configuration& configuration) : 
  configuration(configuration) {
}

void WifiClient::setup() {
  Serial.print("# WifiClient::setup: Connecting to ");
  Serial.println(configuration.getWifiSSID());
  WiFi.begin(configuration.getWifiSSID(), configuration.getWifiPassword());
  if(!configuration.isDynamicIP()) {
    IPAddress ip, gateway, subnet, dns1, dns2;
    configuration.getIPConfiguration(ip, gateway, subnet, dns1, dns2);
    WiFi.config(ip, gateway, subnet, dns1, dns2);
  }
  connected = false;
  lastPolledMillis = millis() - 1000;
  pollCount = 0;
}

void WifiClient::loop() {
  if(!connected) {
    if(millis() - lastPolledMillis > (pollCount < 10 ? 1000 : 60000)) {
      lastPolledMillis = millis();
      pollCount++;
      if(WiFi.status() == WL_CONNECTED) {
        connected = true;
        Serial.println("# WifiClient::loop: WiFi connected");
        Serial.print("# WifiClient::loop: IP address: ");
        Serial.println(WiFi.localIP());  
      } else {
        Serial.println("# WifiClient::loop: WiFi not connected yet");
      }
    }
  }
}

boolean WifiClient::isConnected() {
  return connected;
}
