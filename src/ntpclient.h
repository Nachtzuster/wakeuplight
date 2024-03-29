/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file LICENSE.txt. */

/* This class gets the (UTC) time via an NTP server, and sets the system clock
 * accordingly. */

#ifndef NTPCLIENT_H
#define NTPCLIENT_H

#include <Arduino.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "configuration.h"

class NTPClient {

public:
  explicit NTPClient(Configuration& configuration);
  void setup();
  void loop();
  boolean isTimeSet();

private:
  unsigned long sendDelay = 1000;
  Configuration& configuration;
  WiFiUDP udp;
  static const int ntpPacketSize = 48;
  byte packetBuffer[ntpPacketSize];
  boolean packetSent = false;
  unsigned long lastPacketSentMillis = 0;
  boolean timeSet = false;
  void sendNTPpacket();
  void receiveNTPpacket();
};

#endif
