/* Author: Frederik Slos <frederik.slos@gmail.com>, 2019.
 * This code is in the public domain, please see file LICENSE.txt. */

#ifndef BUTTON_H
#define BUTTON_H

#include "configuration.h"
#include "light.h"
#include "alarm.h"

class Button {
private:
  const uint8_t buttonPin = 16;
  int sampleTime = 0;
  int touched = 0;
  int lightState = 0;
  Configuration& configuration;  
  Light& light;
  Alarm& alarm;
public:
  Button(Configuration& configuration, Light& light, Alarm& alarm);
  void setup();
  void loop();
};

#endif
