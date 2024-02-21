/* Author: Frederik Slos <frederik.slos@gmail.com>, 2019.
 * This code is in the public domain, please see file LICENSE.txt. */

#include "clock_display.h"

Clockdisplay::Clockdisplay(LocalClock& localclock) :
  localclock(localclock), displ(ClockPin, DataPin), lastCheckMillis(0) {
}

void Clockdisplay::setup() {
  pinMode(A0, INPUT);
  displ.setBrightness(7);
}

void Clockdisplay::loop() {
  if((millis() - lastCheckMillis) > 1000) {
    lastCheckMillis = millis();
    if (localclock.isTimeSet()) {
      setBrightness();
      colon = !colon;
      time_t localTime = localclock.getLocalTime();
      displ.showNumberDecEx(hour(localTime) * 100 + minute(localTime),(colon ? 0 : 0x40), true);
    }
  }
}

void Clockdisplay::setBrightness() {
  unsigned int brightness = 0;
  unsigned int raw = analogRead(A0);
  yield();
  if (raw < 1024) brightness = 1;
  if (raw < 800) brightness = 2;
  if (raw < 300) brightness = 7;
  // Serial.printf_P(PSTR("# Clockdisplay::setBrightness raw: %d brightness: %d\n"), raw, brightness);
  displ.setBrightness(brightness);
}

void Clockdisplay::showStatus(int status) {
  switch(status) {
    case WAIT_WIFI_CONF:
      displ.setSegments(SEG_WIFI_CONF);
      break;
    case WAIT_WIFI_CONN:
      displ.setSegments(SEG_WIFI_CONN);
      break;
    case WAIT_NTP_RESP:
      displ.setSegments(SEG_NTP_RESP);
      break;
  }
}

void Clockdisplay::setSegments(uint8_t segments[]) {
  displ.setSegments(segments);
}

void Clockdisplay::showNumberDec(int num) {
  // mod 10000 to show only the 4 lowest digits
  displ.showNumberDec(num % 10000, true);
}
