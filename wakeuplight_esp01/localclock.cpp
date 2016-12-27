/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */
 
#include "localclock.h"

LocalClock::LocalClock(Configuration& configuration, NTPClient& ntpclient) : 
  configuration(configuration), ntpclient(ntpclient) {
}

boolean LocalClock::isTimeSet() {
  return ntpclient.isTimeSet();
}

time_t LocalClock::getLocalTime() {
  if(!isTimeSet()) return 0UL;
  time_t utc = now();
  if(!offsetFirstCalculated || ((millis() - offsetLastCalculatedMillis) > 1000)) calculateLocalOffset(utc);
  return utc + offset;
}

void LocalClock::calculateLocalOffset(time_t utc) {
  int y = year(utc);
  /* Standard with the standard offset. */
  int offsetMinutes = configuration.getStandardTimeOffset();
  /* Apply time change rules. */
  for(int i = 0; i < configuration.getNumberOfTimeChanges(); i++) {
    TimeChange timeChange = configuration.getTimeChanges()[i];
    if(utc >= timeChangeUtc(timeChange, y, offsetMinutes)) {
      offsetMinutes += timeChange.change;
    }
  }
  offset = offsetMinutes*60UL;
}

/* Finds the moment of a time change rule in a given year, in UTC seconds. */
time_t LocalClock::timeChangeUtc(TimeChange timeChange, int y, int offsetMinutes) {
  tmElements_t te;
  te.Year = y - 1970;
  te.Month = timeChange.month;
  te.Day = dayInMonth(timeChange.occurence, timeChange.day, timeChange.month, y);
  te.Hour = timeChange.hour;
  te.Minute = 0;
  te.Second = 0;
  if(te.Day == 0) return -1UL;
  time_t local = makeTime(te);
  return local - offsetMinutes*60UL;
}

/* Finds the day in month (1-31) correspoding to things like the second Wednesday of
 *  April in 2016. 'Occurrence' specifies the number (first, second, etc.) and can
 *  be negative to count backwards (-1 for last etc.) DayOfWeek is 0 for Sunday to 6 for
 *  Saturday. Month is 1-12.
 */
int LocalClock::dayInMonth(int occurence, int dayOfWeek, int m, int y) {
  int count = 0;
  boolean reverse = false;
  if(occurence < 0) {
    occurence = -occurence;
    reverse = true;
  }
  int days = daysInMonth(y, m);
  for(int i = 0; i < days; i++) {
    int dayOfMonth = reverse ? (days - i) : (1 + i);
    if(dayOfWeekFromYMD(y, m, dayOfMonth) == dayOfWeek) count++;
    if(count == occurence) return dayOfMonth;
  }
  Serial.println("# LocalClock::dayInMonth - not found!");
  return 0;
}

/* Returns the number of days in a month, taking into account leap years. */
int LocalClock::daysInMonth(int y, int m) {
  static int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  if(((y % 4) == 0) && (((y % 100) > 0) || ((y % 400) == 0))) days[1] = 29;
  return days[m - 1];
}

/* Sakamoto algorithm to find the day of week given a date. 
 * Input: 1 <= m <= 12, 1 <= d <= 31, returns 0 for Sunday, 1 for Monday etc. */
int LocalClock::dayOfWeekFromYMD(int y, int m, int d) {  
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  y -= m < 3;
  return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}

