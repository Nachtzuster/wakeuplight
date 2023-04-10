/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file LICENSE.txt. */
 
/* This code contains sections from the Serial Event example (available in
 * the Arduino 1.6.6 IDE), written by by Tom Igoe (2011).
 */

#include "serialhost.h"

Serialhost::Serialhost(Configuration& configuration, Alarm& alarm) : 
  configuration(configuration), alarm(alarm) {
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
  Serial.println("# Unknown command");
}


