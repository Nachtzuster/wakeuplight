#ifndef BUTTON_H
#define BUTTON_H

#include "configuration.h"
#include "neo_pix.h"
#include "alarm.h"

class Button {
private:
  const uint8_t buttonPin = 16;
  int sampleTime = 0;
  int touched = 0;
  unsigned long lastCheckMillis = 0;
  int lightState = 0;
  Configuration& configuration;  
  Pixel& light;
  Alarm& alarm;
  //LocalClock& localclock;
public:
  Button(Configuration& configuration, Pixel& light, Alarm& alarm);
  void setup();
  void loop();
};

#endif
