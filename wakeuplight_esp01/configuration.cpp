/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

#include "configuration.h"

/* During setup, we'll read data from EEPROM and check whether it makes sense. If it doesn't, 
 * we'll overwrite with a sensible default value. */
void Configuration::setup() {
  EEPROM.begin(eepromSize);
  for (int alarmId = 0; alarmId<MAX_ALARMS; alarmId++) {
    boolean corrupt = false;
    alarmList[alarmId].hour = EEPROM.read(addrHour + SIZE_ALARM * alarmId);
    alarmList[alarmId].minute = EEPROM.read(addrMinute + SIZE_ALARM * alarmId);
    if(alarmList[alarmId].hour > 23 || alarmList[alarmId].minute > 59) {
      corrupt = true;
    }
    alarmList[alarmId].hidden = readBoolean(addrHidden + SIZE_ALARM * alarmId, corrupt);
    alarmList[alarmId].repeat = readBoolean(addrRepeat + SIZE_ALARM * alarmId, corrupt);
    alarmList[alarmId].enable = readBoolean(addrEnable + SIZE_ALARM * alarmId, corrupt);

    for (int i=0; i<NUM_DAYS; i++) {
      alarmList[alarmId].days[i] = readBoolean(addrEnableMon + i + SIZE_ALARM * alarmId, corrupt);
    }
    if (corrupt) {
      Serial.println("# Configuration::setup alarm time corrupt");
      Serial.print("alarmId: ");
      Serial.println(alarmId);
      initAlarmDef(alarmId);
    }
  }

  /* Read the alarm duration - if it's invalid, reset to 30 minutes */
  alarmDuration = EEPROM.read(addrDuration);
  if(alarmDuration > 180 || (alarmDuration % 5) > 0) {
    alarmDuration = 30;
    EEPROM.write(addrDuration, alarmDuration);
    EEPROM.commit();
  }
   
  alarmEnabled = false;
}

boolean Configuration::readBoolean(int address, boolean& corrupt) {
  /* Read the status bytes - if it's invalid, reset to 'true' */
  byte eepromByte = EEPROM.read(address);
  boolean ret;
  if (eepromByte !=0 &&  eepromByte !=1) {
    corrupt = true;
    ret = true;
  } else  ret = (boolean)eepromByte;
  return ret;
}

void Configuration::loop() {
  flushEeprom();
}

void Configuration::flushEeprom(){
  if (eepromDirty) {
    if(!EEPROM.commit()) Serial.println("# Configuration::flushEeprom: EEPROM.commit failed");
    else eepromDirty = false;
    yield();
  }
}

byte Configuration::getAlarmDay() {
  return alarmDay;
}

byte Configuration::getAlarmHour() {
  return alarmHour;
}

byte Configuration::getAlarmMinute() {
  return alarmMinute;
}

byte Configuration::getAlarmDuration() {
  return alarmDuration;
}

byte Configuration::getAlarmToHour() {
  return alarmToHour;
}

byte Configuration::getAlarmToMinute() {
  return alarmToMinute;
}

boolean Configuration::isAlarmEnabled() {
  return alarmEnabled;
}

void Configuration::alarmIsTriggered(time_t localTime){
  Serial.println("Configuration::alarmIsTriggered");
  if (!alarmList[nextAlarmId].repeat) setAlarmEnabled(nextAlarmId, false);
  setNextAlarm(localTime);
}

int Configuration::calculateMinuteToNext(time_t localTime, int nextDay, int nextHour, int nextMinute) {
  int dayToGo;
  int hourToGo;
  int minuteToGo;

  // dayOfWeek says first day is 1, so substract 1 to start count at 0
  dayToGo = nextDay - (dayOfWeek(localTime) - 1);
  hourToGo = nextHour - hour(localTime);
  minuteToGo = nextMinute - minute(localTime);

  return minuteToGo + hourToGo * 60 + dayToGo * 24 * 60;
}

boolean Configuration::setNextAlarm(time_t localTime) {
  boolean ret = false;
  // first day is 1, so substract 1 to start count at 0
  int dayofweek = dayOfWeek(localTime) - 1;
  int nextId = -1;  
  int nextDay = -1;
  int minuteToGo = (NUM_DAYS * 24 * 60) + 1;
  
  for (int alarmId = 0; alarmId<MAX_ALARMS; alarmId++) {
    //int maxDay;
    if (!alarmList[alarmId].hidden && alarmList[alarmId].enable){
      int minMinutes;
      int dayCount = 0;
      int dayIndex;
      minMinutes = -1;
      //maxDay = NUM_DAYS + 1;
      do {
        dayIndex = (dayofweek + dayCount)%NUM_DAYS;
        if (alarmList[alarmId].days[dayIndex] || !alarmList[alarmId].repeat) {
          minMinutes = calculateMinuteToNext(localTime, (dayofweek + dayCount), alarmList[alarmId].hour, alarmList[alarmId].minute);
        }
        dayCount++;
      } while (minMinutes <= 0 && dayCount <= NUM_DAYS);
      if (minMinutes > 0 && minMinutes < minuteToGo) {
        minuteToGo = minMinutes;
        nextDay = dayIndex;
        nextId = alarmId;
      }
    }
  }
  // first day is 1;
  //nextDay++;

  if (minuteToGo > 0 && minuteToGo <= NUM_DAYS * 24 * 60) {
    // first day is 1, so add 1;
    alarmDay = nextDay + 1;
    alarmHour = alarmList[nextId].hour;
    alarmMinute = alarmList[nextId].minute;
    nextAlarmId = nextId;
    calculateAlarmTo();
    ret = true;
  }
  alarmEnabled = ret;

  return ret;
}

void Configuration::adjustAlarmHour(int alarmId, int hour) {
  alarmList[alarmId].hour = hour % 24;
  EEPROM.write(addrHour + SIZE_ALARM * alarmId, alarmList[alarmId].hour);
  eepromDirty = true;
}

void Configuration::adjustAlarmMinute(int alarmId, int minute) {
  alarmList[alarmId].minute = minute % 60;
  EEPROM.write(addrMinute + SIZE_ALARM * alarmId, alarmList[alarmId].minute);
  eepromDirty = true;
}

void Configuration::adjustAlarmDuration(boolean increase) {
  if(increase && alarmDuration < 175) {
    alarmDuration += 5;
    EEPROM.write(addrDuration, alarmDuration);
    eepromDirty = true;
  } else if(!increase && alarmDuration > 0) {
    alarmDuration -= 5;
    EEPROM.write(addrDuration, alarmDuration);
    eepromDirty = true;
  }
}

void Configuration::setAlarmHidden(int alarmId, boolean hide) {
  alarmList[alarmId].hidden = hide;
  EEPROM.write(addrHidden + SIZE_ALARM * alarmId, (byte)hide);
  eepromDirty = true;
}

void Configuration::setAlarmEnabled(int alarmId, boolean enable) {
  alarmList[alarmId].enable = enable;
  EEPROM.write(addrEnable + SIZE_ALARM * alarmId, (byte)enable);
  eepromDirty = true;
}

void Configuration::setAlarmRepeat(int alarmId, boolean repeat) {
  alarmList[alarmId].repeat = repeat;
  EEPROM.write(addrRepeat + SIZE_ALARM * alarmId, (byte)repeat);
  eepromDirty = true;
}

void Configuration::setAlarmEnableDay(int alarmId, int day_number, boolean enable) {
  alarmList[alarmId].days[day_number] = enable;
  EEPROM.write(addrEnableMon + day_number + SIZE_ALARM * alarmId, (byte)enable);
  eepromDirty = true;
}

void Configuration::expandAlarmList(){
  int alarmId = 0;
  while (alarmId < MAX_ALARMS && !alarmList[alarmId].hidden){
    alarmId++;
    }
  if (alarmList[alarmId].hidden){
    initAlarmDef(alarmId);
    alarmList[alarmId].hidden = false;
    EEPROM.write(addrHidden + SIZE_ALARM * alarmId, (byte)false);
    eepromDirty = true;
  }  
}

void Configuration::reduceAlarmList(int alarmId){
  alarmList[alarmId].hidden = true;
  EEPROM.write(addrHidden + SIZE_ALARM * alarmId, (byte)true);
  eepromDirty = true;
}

void Configuration::serializeAlarmList(JsonObject& status){
  JsonArray& alarms = status.createNestedArray("alarms");

  for (int alarmId = 0; alarmId<MAX_ALARMS; alarmId++){
    if (!alarmList[alarmId].hidden){
      JsonObject& alarm = alarms.createNestedObject();
      alarm["id"] = alarmId;
      alarm["hour"] = alarmList[alarmId].hour;
      alarm["minute"] = alarmList[alarmId].minute;
      alarm["repeat"] = alarmList[alarmId].repeat;
      alarm["enable"] = alarmList[alarmId].enable;
      alarm["mon"] = alarmList[alarmId].days[1];
      alarm["tue"] = alarmList[alarmId].days[2];
      alarm["wed"] = alarmList[alarmId].days[3];
      alarm["thu"] = alarmList[alarmId].days[4];
      alarm["fri"] = alarmList[alarmId].days[5];
      alarm["sat"] = alarmList[alarmId].days[6];
      alarm["sun"] = alarmList[alarmId].days[0];
    }
  }
}

void Configuration::calculateAlarmTo() {
  alarmToMinute = alarmMinute + alarmDuration;
  alarmToHour = (alarmHour + (alarmToMinute / 60)) % 24;
  alarmToMinute = alarmToMinute % 60;  
}

void Configuration::initAlarmDef(int alarmId){
  adjustAlarmHour(alarmId, 7);
  adjustAlarmMinute(alarmId, 0);
  setAlarmHidden(alarmId, true);
  setAlarmEnabled(alarmId, true);
  setAlarmRepeat(alarmId, true);
  for (int i=0; i<NUM_DAYS ; i++){
    setAlarmEnableDay(alarmId, i, true);
  }
}

const char* Configuration::getWifiSSID() {
  return wifiSSID;
}

const char* Configuration::getWifiPassword() {
  return wifiPassword;
}

boolean Configuration::isDynamicIP() {
  return dynamicIP;
}

void Configuration::getIPConfiguration(IPAddress& ip, IPAddress& gateway, IPAddress& subnet, IPAddress& dns1, IPAddress& dns2) {
  ip = this->ip;
  gateway = this->gateway;
  subnet = this->subnet;
  dns1 = this->dns1;
  dns2 = this->dns2;
}

unsigned int Configuration::getNtpLocalPort() {
  return ntpLocalPort;
}

const char* Configuration::getNtpServerName() {
  return ntpserver;
}

int Configuration::getStandardTimeOffset() {
  return standardTimeOffset;
}

int Configuration::getNumberOfTimeChanges() {
  return numberOfTimeChanges;
}

TimeChange* Configuration::getTimeChanges() {
  return timeChanges;
}
