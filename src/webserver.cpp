/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file LICENSE.txt. */

#include "webserver.h"


void listDir(const char *dirname) {
  Serial.printf_P(PSTR("Listing directory: %s\n"), dirname);

  Dir root = LittleFS.openDir(dirname);

  while (root.next()) {
    File file = root.openFile("r");
    Serial.printf_P(PSTR("  FILE: %s  SIZE: %d\n"), root.fileName().c_str(), file.size());
    file.close();
 }
}


Webserver::Webserver(Configuration& configuration, LocalClock& localclock, Alarm& alarm) : 
  configuration(configuration), localclock(localclock), alarm(alarm), server(80) {
}

void Webserver::setup() {
  Serial.println(F("# Webserver::setup: starting"));
  LittleFS.begin();
  listDir("/");
  server.serveStatic("/", LittleFS, "/index.html", "max-age=67890");
  server.serveStatic("/wakeuplight.png", LittleFS, "/wakeuplight.png", "max-age=67890");
  server.serveStatic("/wakeuplight.js", LittleFS, "/wakeuplight.js", "max-age=67890");
  server.on("/L", std::bind(&Webserver::handleCommand, this));
  server.begin();
}

void Webserver::loop() {
  server.handleClient();  
}

void Webserver::handleCommand() {
  String return_alarms = server.arg("alarms");
  String inp = server.arg("inp");
  String value = server.arg("val");
  if(inp != NULL && inp != "" && value != NULL && value != "") {
    // changing alarms requires authentication
    if (!server.authenticate(configuration.getUiUser().c_str(), configuration.getUiPwd().c_str())) {
      return server.requestAuthentication();
    }

    int alarmId;
    String control;
    int dash_index = inp.indexOf("-");
    if (dash_index > 1 && dash_index < ((int)inp.length() - 1)) {
      control = inp.substring(0, dash_index);
      alarmId = inp.substring(dash_index + 1).toInt();
      if (alarmId >= MAX_ALARMS || alarmId < 0) alarmId = 0;
    } else {
      control = inp;
      alarmId = 0;
    }

    if (control == "usr_time"){
      int colon_index = value.indexOf(":");
      if (colon_index > 0 && colon_index < (int)value.length()){
        int alarmOrderBefore[MAX_ALARMS];
        int alarmCount = configuration.getAlarmOrder(alarmOrderBefore);

        int hour = value.substring(0, colon_index).toInt();
        int minute = value.substring(colon_index + 1).toInt();
        configuration.adjustAlarmHour(alarmId, hour);
        configuration.adjustAlarmMinute(alarmId, minute);

        int alarmOrderAfter[MAX_ALARMS];
        configuration.getAlarmOrder(alarmOrderAfter);
        for (int alarmPosition = 0; alarmPosition<alarmCount; alarmPosition++){
          if (alarmOrderBefore[alarmPosition] != alarmOrderAfter[alarmPosition]) {
            return_alarms = "1";
            break;
          }
        }
      }
    }
    else if (control == "repeat") configuration.setAlarmRepeat(alarmId, value == "true");
    else if (control == "enable") configuration.setAlarmEnabled(alarmId, value == "true");
    else if (control == "mon") configuration.setAlarmEnableDay(alarmId, 1, value == "true");
    else if (control == "tue") configuration.setAlarmEnableDay(alarmId, 2, value == "true");
    else if (control == "wed") configuration.setAlarmEnableDay(alarmId, 3, value == "true");
    else if (control == "thu") configuration.setAlarmEnableDay(alarmId, 4, value == "true");
    else if (control == "fri") configuration.setAlarmEnableDay(alarmId, 5, value == "true");
    else if (control == "sat") configuration.setAlarmEnableDay(alarmId, 6, value == "true");
    else if (control == "sun") configuration.setAlarmEnableDay(alarmId, 0, value == "true");
    else if(control == "btn_delete"){
      configuration.reduceAlarmList(alarmId);
      return_alarms = "1";
    }
    else if(control == "btn_alarm_add"){
      configuration.expandAlarmList();
      return_alarms = "1";
    }
    else if(control == "btn_sunrise_slower") configuration.adjustSunriseDuration(true);
    else if(control == "btn_sunrise_faster") configuration.adjustSunriseDuration(false);
    else if(control == "btn_alarm_longer") configuration.adjustAlarmDuration(true);
    else if(control == "btn_alarm_shorter") configuration.adjustAlarmDuration(false);
    else if(control == "btn_alarm_dismis") alarm.deactivate();

    time_t localTime = localclock.getLocalTime();
    if (localclock.isTimeSet()) {
      configuration.setNextAlarm(localTime);
    }
  }
  const size_t capacity = JSON_OBJECT_SIZE(11+2) + (JSON_ARRAY_SIZE(MAX_ALARMS) + JSON_OBJECT_SIZE(1) + MAX_ALARMS*JSON_OBJECT_SIZE(SIZE_ALARM));
  DynamicJsonDocument status(capacity);
  serializeStatus(status);
  if(return_alarms != NULL && return_alarms != "") configuration.serializeAlarmList(status);
  serializeJson(status, responseBuffer);
  server.send(200, "application/json", responseBuffer);
}

void Webserver::serializeStatus(JsonDocument& status) {
  time_t localTime = localclock.getLocalTime();
  String mode;
  
  if(!localclock.isTimeSet()) mode = modeInit;
  else if(alarm.isActive()) mode = modeActive;
  else if(configuration.isAlarmEnabled()) mode = modeEnabled;
  else mode = modeDisabled;

  status["status"] = mode;
  status["hour"] = hour(localTime);
  status["minute"] = minute(localTime);
  status["day"] = day(localTime);
  status["month"] = String(monthStr(month(localTime)));
  status["year"] = year(localTime);
  if (mode == modeDisabled) {
    status["next_day"] = "Disabled";
    status["next_hour"] = 0;
    status["next_minute"] = 0;
    status["to_hour"] = 0;
    status["to_minute"] = 0;
  } else {
    int daysToGo = configuration.getAlarmDay() - dayOfWeek(localTime);
    if (daysToGo == 0 && (hour(localTime) * 60 + minute(localTime) < configuration.getAlarmHour() * 60 + configuration.getAlarmMinute())) {
      status["next_day"] = String("Today");
    }
    else if (daysToGo == 1 || daysToGo == -6) {
      status["next_day"] = String("Tomorrow");
    }
    else status["next_day"] = String(dayStr(configuration.getAlarmDay()));
    status["next_hour"] = configuration.getAlarmHour();
    status["next_minute"] = configuration.getAlarmMinute();
    status["sunrise_duration"] = configuration.getSunriseDuration();
    status["alarm_duration"] = configuration.getAlarmDuration();
  }
}
