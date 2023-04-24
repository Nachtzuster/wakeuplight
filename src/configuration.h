/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file LICENSE.txt. */

/* Class to access all configuration data for the wake-up light. The actual alarm settings that the user may
 * want to change regularly are in EEPROM. Other things like network and timezone settings are hardcoded.  */

/* Timezone logic is inspired to some extent by Jack Christensen's Arduino Timezone library.
 * For several reasons, I decided to roll my own rather than use that library. */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <ArduinoJson.h>

#define POST_DURATION 5
#define DURATION_STEP 5
#define MAX_ALARMS 8
#define SIZE_ALARM 12
#define NUM_DAYS 7
#define RESPONSE_BUFFER_SIZE 1450
#define UI_PWD_LEN 15

enum Month { JANUARY=1, FEBRUARY=2, MARCH=3, APRIL=4, MAY=5, JUNE=6, JULY=7,
  AUGUST=8, SEPTEMBER=9, OCTOBER=10, NOVEMBER=11, DECEMBER=12 };
enum Day { SUNDAY=0, MONDAY=1, TUESDAY=2, WEDNESDAY=3, THURSDAY=4, FRIDAY=5, SATURDAY=6 };
enum Occurence { LAST=-1, FIRST=1, SECOND=2, THIRD=3, FOURTH=4 };
struct TimeChange { Occurence occurence; Day day; Month month; byte hour; int change; };

struct AlarmDef {  
  byte hour;
  byte minute;
  boolean hidden;
  boolean repeat;
  boolean enable;
  boolean days[NUM_DAYS];
};

class Configuration {
private:
  const char* ntpserver = "0.pool.ntp.org";
  unsigned int ntpLocalPort = 2309U;

  const char* ui_user = "wakeuplight";

  /* Time zone settings: currently configured for Central European Time (CET, UTC+1), with
   * a change to Central European Summer Time (CEST, UTC+2) between the last Sunday of March
   * and the last Sunday of October. */
  int standardTimeOffset = 60;
  int numberOfTimeChanges = 2;
  TimeChange timeChanges[2] = {
    { .occurence = LAST, .day = SUNDAY, .month = MARCH, .hour = 2, .change = +60 },
    { .occurence = LAST, .day = SUNDAY, .month = OCTOBER, .hour = 3, .change = -60 }
  };

public:
  void setup();
  byte getAlarmDay();
  byte getAlarmHour();
  byte getAlarmMinute();
  byte getSunriseDuration();
  byte getAlarmPostDuration();
  byte getAlarmToHour();
  byte getAlarmToMinute();
  boolean isAlarmEnabled();
  boolean setNextAlarm(time_t localTime);
  void alarmIsTriggered(time_t localTime);
  void adjustAlarmHour(int alarmId, int hour);
  void adjustAlarmMinute(int alarmId, int minute);
  void adjustSunriseDuration(boolean increase);
  void setAlarmHidden(int alarmId, boolean hide);
  void setAlarmEnabled(int alarmId, boolean enable);
  void setAlarmRepeat(int alarmId, boolean repeat);
  void setAlarmEnableDay(int alarmId, int day_number, boolean enable);
  void expandAlarmList();
  void reduceAlarmList(int alarmId);
  void serializeAlarmList(JsonDocument& status);
  void loop();

  void setUiPwd(String pwd);
  String getUiPwd();
  String getUiUser();
  unsigned int getNtpLocalPort();
  const char* getNtpServerName();
  int getStandardTimeOffset();
  int getNumberOfTimeChanges();
  TimeChange* getTimeChanges();

private:
  AlarmDef alarmList[MAX_ALARMS];
  void calculateAlarmTo();
  int calculateMinuteToNext(time_t localTime, int nextDay, int nextHour, int nextMinute);
  void flushEeprom();
  void initAlarmDef(int alarmId);
  boolean readBoolean(int address, boolean& corrupt);
  void getAlarmOrder(int arr[], int n);
  String ui_pwd;
  boolean alarmEnabled;
  byte nextAlarmId;
  byte alarmDay;
  byte alarmHour;
  byte alarmMinute;
  byte sunriseDuration;
  byte alarmToHour;
  byte alarmToMinute;

  static const int addrHour = 0;
  static const int addrMinute = 1;
  static const int addrHidden = 2;
  static const int addrRepeat = 3;
  static const int addrEnable = 4;
  static const int addrEnableMon = 5;
  static const int addrEnableTue = 6;
  static const int addrEnableWed = 7;
  static const int addrEnableThu = 8;
  static const int addrEnableFri = 9;
  static const int addrEnableSat = 10;
  static const int addrEnableSun = 11;
  static const int addrSunriseDuration = SIZE_ALARM * MAX_ALARMS;
  static const int addrUiPwd = addrSunriseDuration + 1;
  static const int eepromSize = (SIZE_ALARM * MAX_ALARMS) + 1 + (UI_PWD_LEN + 1);

  boolean eepromDirty = false;
};

#endif
