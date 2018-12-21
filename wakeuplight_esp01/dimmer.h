/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

/* This class implements the simple logic of dimming via PWM, without any alarm-like
 * functionality.
 * 
 * Setting the PWM dutycycle based on some value gives a linear control over the
 * resulting light intensity - but the human eye intensity response is more or less
 * logarithmic. This class contains the logic to compensate for that. The light
 * intensity can be set to a certain 'step'. This is converted to a PWM duty cycle such
 * that there experienced light intensity is roughly proportionate to the chosen step.
 * 
 * Methods off/decrease/increase/full are for manual control over light intensity. Method
 * 'increaseTo' is for automatic control during the wakeup cycle. The wake-up light functionality
 * will only increase light intensity, not decrease a previously manually set level. Hence
 * the 'increaseTo' rather than 'setTo' semantic.
 * 
 * This class also allows direct access to the PWM settings via 3 debug methods. This
 * is useful for circuit debugging, but is not used in the normal operations of the wake-up
 * light.
 */

#ifndef DIMMER_H
#define DIMMER_H

#include <Arduino.h>

class Dimmer {

public:
  void setup();
  void loop();
  void off();
  void decrease();
  void increase();
  void full();
  void increaseTo(float scale);
  int steps();
  void debugSetValue(int value);
  void debugSetFrequency(int frequency);
  void debugSetRange(int range);

private:
  const int pwmPin = 0;
  const int enablePin = 2;
  const int deltaSteps = 180;
  int requiredStep;
  int requiredValue;
  int actualValue;
};

#endif
