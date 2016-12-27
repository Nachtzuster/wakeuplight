/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

#include "configuration.h"

/* During setup, we'll read data from EEPROM and check whether it makes sense. If it doesn't, 
 * we'll overwrite with a sensible default value. */
void Configuration::setup() {
  EEPROM.begin(eepromSize);
  /* Read the enable byte - if it's invalid, reset to 'false' */
  byte alarmEnableByte = EEPROM.read(addrAlarmEnable);
  if(alarmEnableByte == 0) {
    alarmEnabled = false;
  } else if(alarmEnableByte == 1) {
    alarmEnabled = true;
  } else {
    alarmEnabled = false;
    EEPROM.write(addrAlarmEnable, alarmEnabled ? 1 : 0);
    EEPROM.commit();
  }
  /* Read the alarm time - if it's invalid, reset to 7:00 */
  alarmHour = EEPROM.read(addrAlarmHour);
  alarmMinute = EEPROM.read(addrAlarmMinute);
  if(alarmHour > 23 || alarmMinute > 59 || (alarmMinute % 5) > 0) {
    alarmHour = 7;
    alarmMinute = 0;
    EEPROM.write(addrAlarmHour, alarmHour);
    EEPROM.write(addrAlarmMinute, alarmMinute);
    EEPROM.commit();
  }
  /* Read the alarm duration - if it's invalid, reset to 30 minutes */
  alarmDuration = EEPROM.read(addrAlarmDuration);
  if(alarmDuration > 180 || (alarmDuration % 5) > 0) {
    alarmDuration = 30;
    EEPROM.write(addrAlarmDuration, alarmDuration);
    EEPROM.commit();
  }
  calculateAlarmTo();
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

void Configuration::adjustAlarmHour(boolean increase) {
  alarmHour = (alarmHour + (increase ? 1 : 23)) % 24;
  EEPROM.write(addrAlarmHour, alarmHour);
  EEPROM.commit();
  calculateAlarmTo();
}

void Configuration::adjustAlarmMinute(boolean increase) {
  if(increase) {
    if(alarmMinute < 55) {
      alarmMinute += 5;
      EEPROM.write(addrAlarmMinute, alarmMinute);
    } else {
      alarmMinute = 0;
      alarmHour = (alarmHour + 1) % 24;
      EEPROM.write(addrAlarmHour, alarmHour);
      EEPROM.write(addrAlarmMinute, alarmMinute);
    }
  } else {
    if(alarmMinute > 0) {
      alarmMinute -= 5;
      EEPROM.write(addrAlarmMinute, alarmMinute);
    } else {
      alarmMinute = 55;
      alarmHour = (alarmHour + 23) % 24;
      EEPROM.write(addrAlarmHour, alarmHour);
      EEPROM.write(addrAlarmMinute, alarmMinute);
    }
  }
  EEPROM.commit();
  calculateAlarmTo();
}

void Configuration::adjustAlarmDuration(boolean increase) {
  if(increase && alarmDuration < 175) {
    alarmDuration += 5;
    EEPROM.write(addrAlarmDuration, alarmDuration);
    EEPROM.commit();
  } else if(!increase && alarmDuration > 0) {
    alarmDuration -= 5;
    EEPROM.write(addrAlarmDuration, alarmDuration);
    EEPROM.commit();
  }
  calculateAlarmTo();
}

void Configuration::setAlarmEnabled(boolean enable) {
  if(alarmEnabled != enable) {
    alarmEnabled = enable;
    EEPROM.write(addrAlarmEnable, alarmEnabled ? 1 : 0);
    EEPROM.commit();    
  }
}

void Configuration::calculateAlarmTo() {
  alarmToMinute = alarmMinute + alarmDuration;
  alarmToHour = (alarmHour + (alarmToMinute / 60)) % 24;
  alarmToMinute = alarmToMinute % 60;  
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

void Configuration::getIPConfiguration(IPAddress& ip, IPAddress& gateway, IPAddress& subnet) {
  ip = this->ip;
  gateway = this->gateway;
  subnet = this->subnet;
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

