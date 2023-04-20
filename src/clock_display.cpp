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
  unsigned int light = 1024 - analogRead(A0);
  yield();
  /* max brightness is 7, so make sure not to go over */
  unsigned int brightness = round(sqrt(light)/4.57);
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
