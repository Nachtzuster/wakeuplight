/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file LICENSE.txt. */

#ifndef SERIALHOST_H
#define SERIALHOST_H

/* Provides the possibility to issue some debug commands - not used for normal
 * operations. */

#include <Arduino.h>
#include "configuration.h"
#include "dimmer.h"
#include "wificlient.h"
#include "localclock.h"
#include "alarm.h"

class Serialhost {

public:
  Serialhost(Configuration& configuration, Alarm& alarm);
  void setup();
  void loop();

private:
  Configuration& configuration;
  Alarm& alarm;
  String commandString = "";        
  boolean commandComplete = false;
  void handleSerialCommand();
};

#endif
