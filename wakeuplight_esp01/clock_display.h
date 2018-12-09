#ifndef CLOCK_DISPLAY_H
#define CLOCK_DISPLAY_H

#include "configuration.h"
#include "localclock.h"

// https://github.com/avishorp/TM1637.git
#include <TM1637Display.h>

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
  Clockdisplay(LocalClock& localclock);
  void setup();
  void loop();
};

#endif
