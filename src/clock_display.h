/* Author: Frederik Slos <frederik.slos@gmail.com>, 2019.
 * This code is in the public domain, please see file LICENSE.txt. */

#ifndef CLOCK_DISPLAY_H
#define CLOCK_DISPLAY_H

#include "configuration.h"
#include "localclock.h"

// https://github.com/avishorp/TM1637.git
#include <TM1637Display.h>

#define WAIT_WIFI_CONF 1
#define WAIT_WIFI_CONN 2
#define WAIT_NTP_RESP  3

const uint8_t SEG_WIFI_CONF[] = {SEG_D, SEG_D, SEG_D, SEG_D};
const uint8_t SEG_WIFI_CONN[] = {SEG_D|SEG_G, SEG_D|SEG_G, SEG_D|SEG_G, SEG_D|SEG_G};
const uint8_t SEG_NTP_RESP[] = {SEG_D|SEG_G|SEG_A, SEG_D|SEG_G|SEG_A, SEG_D|SEG_G|SEG_A, SEG_D|SEG_G|SEG_A};

class Clockdisplay {
private:
  LocalClock& localclock;
  const uint8_t DataPin = 13;
  const uint8_t ClockPin = 14;
  TM1637Display displ;
  unsigned long lastCheckMillis;
  bool colon = false;
  void setBrightness();

public:
  explicit Clockdisplay(LocalClock& localclock);
  void showStatus(int status);
  void setSegments(uint8_t segments[]);
  void setup();
  void loop();
};

#endif
