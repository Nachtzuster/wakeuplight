/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */
 
/* This code contains sections from the Serial Event example (available in
 * the Arduino 1.6.6 IDE), written by by Tom Igoe (2011).
 */

#include "serialhost.h"

Serialhost::Serialhost(Configuration& configuration, Dimmer& dimmer, Alarm& alarm) : 
  configuration(configuration), dimmer(dimmer), alarm(alarm) {
}

void Serialhost::setup() {
  commandString.reserve(200);  
}

void Serialhost::loop() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    commandString += inChar;
    if (inChar == '\n') {
      commandComplete = true;
    }
  }
  if(commandComplete) {
    handleSerialCommand();
    commandString = "";
    commandComplete = false;
  }    
}

void Serialhost::handleSerialCommand() {
  alarm.deactivate();
  commandString.trim();
  if(commandString.startsWith("DEBUG:FREQ=")) {
    int freq = commandString.substring(5).toInt();
    dimmer.debugSetFrequency(freq);
    Serial.println("# Serialhost::handleSerialCommand: Changed PWM frequency to ");
    Serial.println(freq);
  } else if(commandString.startsWith("DEBUG:RANGE=")) {
    int range = commandString.substring(6).toInt();
    dimmer.debugSetRange(range);
    Serial.println("# Serialhost::handleSerialCommand: Changed PWM range to ");
    Serial.println(range);
  } else if(commandString.startsWith("DEBUG:VALUE=")) {
    int value = commandString.substring(6).toInt();
    dimmer.debugSetValue(value);
    Serial.println("# Serialhost::handleSerialCommand: Changed PWM value to ");
    Serial.println(value);
  } else {
    Serial.println("# Unknown command");
  }
}


