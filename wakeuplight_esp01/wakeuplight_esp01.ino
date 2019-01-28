/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file LICENSE.txt. */

/* This code is for building a wake-up light (sometimes called a light alarm) with
 * an ESP-01 module. The program gets time via NTP, and converts this to local time.
 * When an (EEPROM-stored) alarm time is reached, it will turn on a light slowly (PWM).
 * A webserver enables control over the alarm settings, and manual control over the
 * current light settings. A serial interface is available to directly access the PWM
 * settings for hardware circuit debugging.
 * 
 * The code has been split into 8 classes with a single function (each with a .cpp and
 * a .h file). This main program just wires them together.
 * 
 * Known bug: the light will flicker when alarm settings are being changes while the
 * light is in a dimmed state (more than 0% and less than 100%). This has something
 * to do with the EEPROM.commit() call to save the alarm settings interfering with
 * the PWM logic.
 */

#include <ESP8266mDNS.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

#include "configuration.h"
#include "ntpclient.h"
#include "localclock.h"
#include "alarm.h"
#include "serialhost.h"
#include "webserver.h"
#include "clock_display.h"
#include "neo_pix.h"
#include "button.h"

Configuration configuration;
Pixel light;
NTPClient ntpclient(configuration);
LocalClock localclock(configuration, ntpclient);
Alarm alarm(configuration, light, localclock);
Serialhost serialhost(configuration, alarm);
Webserver webserver(configuration, localclock, alarm);
Clockdisplay clockdisplay(localclock);
Button button(configuration, light, alarm);

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  clockdisplay.showStatus(WAIT_WIFI_CONF);
}

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);
  configuration.setup();
  light.setup();
  clockdisplay.setup();
  button.setup();
  serialhost.setup();
  if (button.isHeldDown()) {
    Serial.println("button was held down, resetting wifi settings");
    wifiManager.resetSettings();
  }
  else clockdisplay.showStatus(WAIT_WIFI_CONN);
  wifiManager.autoConnect("wakeuplight", "wakeuplight");
  clockdisplay.showStatus(WAIT_NTP_RESP);
  ntpclient.setup();
  webserver.setup();
  MDNS.begin("wakeuplight");
  MDNS.addService("http", "tcp", 80);
}

/* Loop time while idle is 55 microseconds (18 kHz). Handling a web request is pretty slow,
 * takes about .5 seconds. */

void loop() {
  ntpclient.loop();
  alarm.loop();
  serialhost.loop();
  webserver.loop();
  clockdisplay.loop();
  light.loop();
  button.loop();
  configuration.loop();
}
