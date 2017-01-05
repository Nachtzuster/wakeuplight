/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

/* This class provides the core functionality of the wakeup-light. It compares the
 * current time compared to the configured alarm times, and adjusts the light
 * accordingly. Because the configuration, dimmer and local time logic have 
 * been factored out, this class is pretty simple.
 */

#ifndef ALARM_H
#define ALARM_H

#include "configuration.h"
#include "localclock.h"
#include "dimmer.h"

class Alarm {

public:
  Alarm(Configuration& configuration, Dimmer& dimmer, LocalClock& localclock);
  void loop();
  boolean isActive();
  void deactivate();
  void resetLastTriggered();

private:
  Configuration& configuration;  
  Dimmer& dimmer;
  LocalClock& localclock;
  unsigned long lastCheckMillis;
  time_t lastTriggered = 0UL;
  boolean active = false;
  boolean timeIsSynced = false;
  time_t durationSecs;
};

#endif
