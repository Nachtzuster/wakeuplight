/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

#include "alarm.h"

Alarm::Alarm(Configuration& configuration, Dimmer& dimmer, LocalClock& localclock) : 
  configuration(configuration), dimmer(dimmer), localclock(localclock) {
}

void Alarm::loop() {
  if((millis() - lastCheckMillis) > 1000) {
    lastCheckMillis = millis();
    if(active) {
      /* The alarm is currently active. We'll increase the dimmer step to the appropriate level. */
      time_t elapsedSecs = now() - lastTriggered;
      if(elapsedSecs > durationSecs) {
        active = false;
        dimmer.full();
      } else {
        dimmer.increaseTo(elapsedSecs*dimmer.steps()/durationSecs);
      }    
    } else {
      /* The alarm is currently not active. Check if it's time to become active. */
      if((now() - lastTriggered) > 60UL) {
        if(configuration.isAlarmEnabled() && localclock.isTimeSet()) {
          time_t localTime = localclock.getLocalTime();
          if(hour(localTime) == configuration.getAlarmHour() && minute(localTime) == configuration.getAlarmMinute()) {
            active = true;
            durationSecs = configuration.getAlarmDuration()*60UL;
            lastTriggered = now();
          }        
        }
      }
    }
  }
}

boolean Alarm::isActive() {
  return active;
}

void Alarm::deactivate() {
  active = false;
}

void Alarm::resetLastTriggered() {
  lastTriggered = 0UL;
}

