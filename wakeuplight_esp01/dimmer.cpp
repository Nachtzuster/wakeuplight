/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

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
  digitalWrite(enablePin, 1);
  pinMode(pwmPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  analogWriteFreq(dimmerFrequency);
  analogWriteRange(dimmerRange);
  requiredStep = dimmerSteps;
  requiredValue = dimmerValues[requiredStep];
  analogWrite(pwmPin, requiredValue);
  actualValue = requiredValue;
  /* Let PWM stabilize before we enable, to avoid flicker. */
  delay(250); 
  digitalWrite(enablePin, 0);
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
  int step = scale * dimmerSteps;
  if(step > requiredStep) {
    requiredStep = step;
    requiredValue = dimmerValues[requiredStep];
  }
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

