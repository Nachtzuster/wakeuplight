/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

#ifndef WIFICLIENT_H
#define WIFICLIENT_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "configuration.h"

class WifiClient {

public:
  WifiClient(Configuration& configuration);
  void setup();
  void loop();
  boolean isConnected();

private:
  Configuration& configuration;  
  boolean connected;
  unsigned long lastPolledMillis;
  unsigned long pollCount;
};

#endif
