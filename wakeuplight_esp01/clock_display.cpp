#include "clock_display.h"

Clockdisplay::Clockdisplay(LocalClock& localclock) :
  localclock(localclock), displ(ClockPin, DataPin) {
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
  displ.setBrightness(round(sqrt(light)/4.0));
}
