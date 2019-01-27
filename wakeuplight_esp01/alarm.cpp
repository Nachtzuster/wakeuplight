/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

#include "alarm.h"

Alarm::Alarm(Configuration& configuration, Light& light, LocalClock& localclock) :
  configuration(configuration), light(light), localclock(localclock) {
}

void Alarm::loop() {
  if(timeIsSynced){
    if((millis() - lastCheckMillis) > 1000) {
      lastCheckMillis = millis();
      if(active) {
        /* The alarm is currently active. We'll increase the dimmer step to the appropriate level. */
        time_t elapsedSecs = now() - lastTriggered;
        if(elapsedSecs > postSecs) {
          active = false;
          light.increaseTo(0.0f);
        }
        if(elapsedSecs <= rampUpSecs) {
          light.increaseTo((float)elapsedSecs/(float)rampUpSecs);
        }
      } else {
        /* The alarm is currently not active. Check if it's time to become active. */
        if((unsigned long)(now() - lastTriggered) > 60UL) {
          if(configuration.isAlarmEnabled()) {
            rampUpSecs = configuration.getAlarmDuration()*60UL;
            postSecs = rampUpSecs + configuration.getAlarmPostDuration()*60UL;
            /* Need to start light ramp up rampUpSecs before alarm time */
            time_t localTime = localclock.getLocalTime() + rampUpSecs;
            if(dayOfWeek(localTime) == configuration.getAlarmDay() && 
               hour(localTime) == configuration.getAlarmHour() && 
               minute(localTime) == configuration.getAlarmMinute()) {
              active = true;
              lastTriggered = now();
              configuration.alarmIsTriggered(localTime);
            }
          }
        }
      }
    }
  } else {
    if (localclock.isTimeSet()) {
      timeIsSynced = true;    
      configuration.setNextAlarm(localclock.getLocalTime());
    }
  }
}

boolean Alarm::isActive() {
  return active;
}

void Alarm::deactivate() {
  light.increaseTo(0.0f);
  active = false;
}

void Alarm::resetLastTriggered() {
  lastTriggered = 0UL;
}
