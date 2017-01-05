/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

/* This module serves the web interface. The web interface is a single-page application that
 * send Ajax requests. The webserver has two handlers; one for handling the GET request to
 * the homepage, and another for handling the Ajax calls.
 */

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "configuration.h"
#include "dimmer.h"
#include "localclock.h"
#include "alarm.h"
/* If this include is placed before the other #includes, compile-time errors occur. */
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

class Webserver {

public:
  Webserver(Configuration& configuration, Dimmer& dimmer, LocalClock& localclock, Alarm& alarm);
  void setup();
  void loop();

private:
  Configuration& configuration;
  Dimmer& dimmer;
  LocalClock& localclock;
  Alarm& alarm;
  ESP8266WebServer server;
  void handleRoot();
  void handleCommand();  
  void serializeStatus(JsonObject& status);

  const char* modeInit = "INIT";
  const char* modeActive = "ACTIVE";
  const char* modeEnabled = "ENABLED";
  const char* modeDisabled = "DISABLED";
  char responseBuffer[RESPONSE_BUFFER_SIZE];   
};

#endif
