/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

/* Class to access all configuration data for the wake-up light. The actual alarm settings that the user may
 * want to change regularly are in EEPROM. Other things like network and timezone settings are hardcoded.  */

/* Timezone logic is inspired to some extent by Jack Christensen's Arduino Timezone library.
 * For several reasons, I decided to roll my own rather than use that library. */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>

enum Month { JANUARY=1, FEBRUARY=2, MARCH=3, APRIL=4, MAY=5, JUNE=6, JULY=7,
  AUGUST=8, SEPTEMBER=9, OCTOBER=10, NOVEMBER=11, DECEMBER=12 };
enum Day { SUNDAY=0, MONDAY=1, TUESDAY=2, WEDNESDAY=3, THURSDAY=4, FRIDAY=5, SATURDAY=6 };
enum Occurence { LAST=-1, FIRST=1, SECOND=2, THIRD=3, FOURTH=4 };
struct TimeChange { Occurence occurence; Day day; Month month; byte hour; int change; };

class Configuration {
private:  
  /* Network settings - change as appropriate. */
  const char* wifiSSID = "XXXXXXXXXXX";  
  const char* wifiPassword = "YYYYYYYYYY";
  boolean dynamicIP = false;
  byte ip[4] = {192,168,178,200};
  byte gateway[4] = {192,168,178,1};
  byte subnet[4] = {255,255,255,0};
  const char* ntpserver = "0.nl.pool.ntp.org";
  unsigned int ntpLocalPort = 2309U;

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
  byte getAlarmHour();
  byte getAlarmMinute();
  byte getAlarmDuration();
  byte getAlarmToHour();
  byte getAlarmToMinute();
  boolean isAlarmEnabled();
  void adjustAlarmHour(boolean increase);
  void adjustAlarmMinute(boolean increase);
  void adjustAlarmDuration(boolean increase);
  void setAlarmEnabled(boolean enable);
  const char* getWifiSSID();
  const char* getWifiPassword();
  boolean isDynamicIP();
  void getIPConfiguration(IPAddress& ip, IPAddress& gateway, IPAddress& subnet);
  unsigned int getNtpLocalPort();
  const char* getNtpServerName();
  int getStandardTimeOffset();
  int getNumberOfTimeChanges();
  TimeChange* getTimeChanges();

private:
  void calculateAlarmTo();
  boolean alarmEnabled;
  byte alarmHour;
  byte alarmMinute;
  byte alarmDuration;
  byte alarmToHour;
  byte alarmToMinute;
  
  static const int addrAlarmEnable = 0;
  static const int addrAlarmHour = 1;
  static const int addrAlarmMinute = 2;
  static const int addrAlarmDuration = 3;
  static const int eepromSize = 4;
};

#endif
