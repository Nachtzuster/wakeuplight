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

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

#include "configuration.h"
#include "ntpclient.h"
#include "localclock.h"
#include "alarm.h"
#include "serialhost.h"
#include "webserver.h"
#include "clock_display.h"
#include "dimmer.h"
#include "button.h"
#include "sound.h"
#include "wms.h"

#define HOSTNAME "wakeuplight"
#define AP_PWD ""
// we can only show 4 characters on the clock display
#define RND_LEN 4
uint8 rnd_bits[RND_LEN];

Configuration configuration;
Dimmer light;
Sound sound;
NTPClient ntpclient(configuration);
LocalClock localclock(configuration, ntpclient);
Alarm alarm(configuration, light, sound, localclock);
Serialhost serialhost(configuration, alarm);
Webserver webserver(configuration, localclock, alarm);
Clockdisplay clockdisplay(localclock);
Button button(configuration, light, alarm);
String ap_pwd = AP_PWD;

void configModeCallback (WiFiManager *myWiFiManager) {
  (void)myWiFiManager;  // silence warning
  Serial.println(F("Entered config mode"));
  clockdisplay.showStatus(WAIT_WIFI_CONF);
  if (String(AP_PWD) == "") {
    uint8 rnd_segments[RND_LEN];
    for (int i=0;  i<RND_LEN; i++) {
      rnd_segments[i] = encodeBitsToSegment(rnd_bits[i]);
    }
    clockdisplay.setSegments(rnd_segments);
    Serial.printf("ap_pwd '%s'\n", ap_pwd.c_str());
  }
}

void initWifi() {
  softap_config ap_config;
  WiFiManager wifiManager;

  if (ap_pwd == "") {
    char rnd_chars[RND_LEN + 1];
    getRandomBits(rnd_bits, RND_LEN);
    encodeBitsToCStr(rnd_bits, rnd_chars, RND_LEN);
    ap_pwd = String(rnd_chars) + HOSTNAME;
  }

  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setConfigPortalTimeout(300);
  wifi_softap_get_config(& ap_config);
  ap_config.authmode = AUTH_WPA2_PSK;
  wifi_softap_set_config(& ap_config);
  if (button.isHeldDown()) {
    Serial.println(F("Button was held down, starting ConfigPortal"));
    wifiManager.startConfigPortal(HOSTNAME, ap_pwd.c_str());
  }
  else clockdisplay.showStatus(WAIT_WIFI_CONN);
  if (!wifiManager.autoConnect(HOSTNAME, ap_pwd.c_str())) {
    Serial.println(F("Failed to connect, restarting"));
    ESP.restart();
  }
  IPAddress ip = WiFi.localIP();
  clockdisplay.showNumberDec(ip.isV4()? ip[2] * 1000 + ip[3] : ip[14] * 1000 + ip[15]);
  delay(500);
}

void setup() {
  Serial.begin(115200);

  light.setup();
  button.setup();
  clockdisplay.setup();
  initWifi();

  // now setup the rest
  sound.setup();
  configuration.setup();
  serialhost.setup();
  ntpclient.setup();
  webserver.setup();
  MDNS.begin(HOSTNAME);
  MDNS.addService("http", "tcp", 80);
}

/* Loop time while idle is 55 microseconds (18 kHz). Handling a web request is pretty slow,
 * takes about .5 seconds. */

void loop() {
  MDNS.update();
  ntpclient.loop();
  alarm.loop();
  serialhost.loop();
  webserver.loop();
  // the display updates causes a sound glitch
  if (!sound.playing) clockdisplay.loop();
  light.loop();
  sound.loop();
  button.loop();
  configuration.loop();
}
