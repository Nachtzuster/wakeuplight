/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file LICENSE.txt. */

/* This class converts the system time to a local time, taking into
 * account the time offset and possible daylight savings rules, which
 * are retrieved from the Configuration object. */

#ifndef LOCALCLOCK_H
#define LOCALCLOCK_H

#include "configuration.h"
#include "ntpclient.h"

class LocalClock {

public:
  LocalClock(Configuration& configuration, NTPClient& ntpclient);
  boolean isTimeSet();
  time_t getLocalTime();

private:
  Configuration& configuration;
  NTPClient& ntpclient;
  boolean offsetFirstCalculated = false;
  unsigned long offsetLastCalculatedMillis;
  time_t offset;
  void calculateLocalOffset(time_t utc);
  time_t timeChangeUtc(TimeChange timeChange, int y, int offsetMinutes);
  int dayInMonth(int occurence, int dayOfWeek, int m, int y);
  int daysInMonth(int y, int m);
  int dayOfWeekFromYMD(int y, int m, int d);
};

#endif
