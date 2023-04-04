/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file LICENSE.txt. */

#include "dimmer.h"

/* This is a generated file containing constants that have been calculated by a separate Java program. */
#include "dimmer_pwm.h"

/* We want the 'default' setting of the Dimmer on startup to be fully on. Also, there shouldn't be any flicker during
 * startup. That's somewhat hard, because on an ESP-01 we only have GPIO-0 and GPIO-2 available. These must be pulled
 * up during startup, and are driven by the ESP for a short moment during startup. Also, during a first AnalogWrite there
 * is some instability on a pin.
 * 
 * This is solves as follows: the hardware circuit is designed to fire only is GPIO-0 and 2 are low. We first make sure
 * that they're both outputting a digital high rather than just being pulled high. Then we start the PWM on pin 0, and
 * let it stabilize. Then we bring pin 2 to low as well. In this scheme, pin 0 is the 'pwmPin' and pin 2 the 'enablePin'.
 */
void Dimmer::setup() {
  digitalWrite(pwmPin, 1);
  if (enablePin >= 0) digitalWrite(enablePin, 1);
  pinMode(pwmPin, OUTPUT);
  if (enablePin >= 0) pinMode(enablePin, OUTPUT);
  analogWriteFreq(dimmerFrequency);
  analogWriteRange(dimmerRange);
  if (initial_full_on) full(); else off();
  loop();
  /* Let PWM stabilize before we enable, to avoid flicker. */
  delay(250);
  if (enablePin >= 0) digitalWrite(enablePin, 0);
}

void Dimmer::loop() {
  if(requiredValue != actualValue) {
    actualValue = requiredValue;
    analogWrite(pwmPin, actualValue);  
  }
}

void Dimmer::off() {
  requiredStep = 0;
  requiredValue = dimmerValues[requiredStep];
}

void Dimmer::decrease() {
  requiredStep -= deltaSteps;
  if(requiredStep < 0) requiredStep = 0;
  requiredValue = dimmerValues[requiredStep];
}

void Dimmer::increase() {
  requiredStep += deltaSteps;
  if(requiredStep > dimmerSteps) requiredStep = dimmerSteps;
  requiredValue = dimmerValues[requiredStep];
}

void Dimmer::full() {
  requiredStep = dimmerSteps;
  requiredValue = dimmerValues[requiredStep];
}

void Dimmer::increaseTo(float scale) {
  if (scale > 1.0f) scale = 1.0f;
  if (scale < 0.0f) scale = 0.0f;
  requiredStep = scale * dimmerSteps;
  requiredValue = dimmerValues[requiredStep];
}

int Dimmer::steps() {
  return dimmerSteps;
}

void Dimmer::debugSetValue(int value) {
  analogWrite(pwmPin, value);  
  requiredValue = value;
  actualValue = value;
}

void Dimmer::debugSetFrequency(int frequency) {
  analogWriteFreq(frequency);  
}

void Dimmer::debugSetRange(int range) {
  analogWriteRange(range);  
}
