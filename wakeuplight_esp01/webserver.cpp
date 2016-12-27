/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

#include "webserver.h"

/* This file contains the HTML code of the homepage. It is generated from a Java project. */
#include "webserver_homepage.h"

Webserver::Webserver(Configuration& configuration, Dimmer& dimmer, LocalClock& localclock, Alarm& alarm) : 
  configuration(configuration), dimmer(dimmer), localclock(localclock), alarm(alarm), server(80) {
}

void Webserver::setup() {
  Serial.println("# Webserver::setup: starting");
  server.on("/", std::bind(&Webserver::handleRoot, this));
  server.on("/L", std::bind(&Webserver::handleCommand, this));
  server.begin();
}

void Webserver::loop() {
  server.handleClient();  
}

void Webserver::handleRoot() {
  server.send(200, "text/html", webserverHomepage);
}

void Webserver::handleCommand() {
  String btn = server.arg("btn");
  if(btn != NULL && btn != "") {
    alarm.deactivate();
    if(btn == "btn_light_full") dimmer.full();
    else if(btn == "btn_light_more") dimmer.increase();
    else if(btn == "btn_light_less") dimmer.decrease();
    else if(btn == "btn_light_off") dimmer.off();
    else if(btn == "btn_alarm_hh_inc") configuration.adjustAlarmHour(true);
    else if(btn == "btn_alarm_hh_dec") configuration.adjustAlarmHour(false);
    else if(btn == "btn_alarm_mm_inc") configuration.adjustAlarmMinute(true);
    else if(btn == "btn_alarm_mm_dec") configuration.adjustAlarmMinute(false);
    else if(btn == "btn_alarm_slower") configuration.adjustAlarmDuration(true);
    else if(btn == "btn_alarm_faster") configuration.adjustAlarmDuration(false);
    else if(btn == "btn_alarm_toggle") {
      if(alarm.isActive()) {
        alarm.deactivate();
      } else {
        alarm.resetLastTriggered();
        configuration.setAlarmEnabled(!configuration.isAlarmEnabled());
      }
    }
  }

  const char* mode;
  if(!localclock.isTimeSet()) mode = modeInit;
  else if(alarm.isActive()) mode = modeActive;
  else if(configuration.isAlarmEnabled()) mode = modeEnabled;
  else mode = modeDisabled;
  
  time_t localTime = localclock.getLocalTime();
  
  snprintf(responseBuffer, 100, 
    "%d:%02d\r\n%d %s %d\r\n%d:%02d\r\n%d:%02d\r\n%s",
    hour(localTime),
    minute(localTime),
    day(localTime),
    monthStr(month(localTime)),
    year(localTime),
    configuration.getAlarmHour(),
    configuration.getAlarmMinute(),
    configuration.getAlarmToHour(),
    configuration.getAlarmToMinute(),
    mode);
  server.send(200, "text/plain", responseBuffer);
}

