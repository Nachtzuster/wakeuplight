/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

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

#include "configuration.h"
#include "dimmer.h"
#include "wificlient.h"
#include "ntpclient.h"
#include "localclock.h"
#include "alarm.h"
#include "serialhost.h"
#include "webserver.h"

Configuration configuration;
Dimmer dimmer;
WifiClient wificlient(configuration);
NTPClient ntpclient(configuration, wificlient);
LocalClock localclock(configuration, ntpclient);
Alarm alarm(configuration, dimmer, localclock);
Serialhost serialhost(configuration, dimmer, alarm);
Webserver webserver(configuration, dimmer, localclock, alarm);

void setup() {
  Serial.begin(115200);
  configuration.setup();
  dimmer.setup();
  wificlient.setup();
  ntpclient.setup();
  serialhost.setup();
  webserver.setup();
}

/* Loop time while idle is 55 microseconds (18 kHz). Handling a web request is pretty slow,
 * takes about .5 seconds. */
void loop() {
  dimmer.loop();
  wificlient.loop();
  ntpclient.loop();
  alarm.loop();
  serialhost.loop();
  webserver.loop();
}

