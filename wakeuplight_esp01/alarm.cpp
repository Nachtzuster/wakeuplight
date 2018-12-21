/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

#include "alarm.h"

Alarm::Alarm(Configuration& configuration, Pixel& light, LocalClock& localclock) : 
  configuration(configuration), light(light), localclock(localclock) {
}

void Alarm::loop() {
  if(timeIsSynced){
    if((millis() - lastCheckMillis) > 1000) {
      lastCheckMillis = millis();
      if(active) {
        /* The alarm is currently active. We'll increase the dimmer step to the appropriate level. */
        time_t elapsedSecs = now() - lastTriggered;
        if(elapsedSecs > durationSecs) {
          active = false;
          light.increaseTo(1.0f);
        } else {
          light.increaseTo((float)elapsedSecs/(float)durationSecs);
        }    
      } else {
        /* The alarm is currently not active. Check if it's time to become active. */
        if((now() - lastTriggered) > 60UL) {
          if(configuration.isAlarmEnabled()) {
            time_t localTime = localclock.getLocalTime();
            if(dayOfWeek(localTime) == configuration.getAlarmDay() && 
               hour(localTime) == configuration.getAlarmHour() && 
               minute(localTime) == configuration.getAlarmMinute()) {
              active = true;
              durationSecs = configuration.getAlarmDuration()*60UL;
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
