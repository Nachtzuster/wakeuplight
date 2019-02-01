#include "button.h"

Button::Button(Configuration& configuration, Light& light, Alarm& alarm) :
  configuration(configuration), light(light), alarm(alarm) {
}

void Button::setup() {
   pinMode(buttonPin, INPUT);
}

void Button::loop() {
  if (sampleTime == 255) {
    sampleTime = 0;
    int touchValue = digitalRead(buttonPin);
    if (touchValue == HIGH)
    {
      touched = 1;
    }
    else
    {
      if (touched) {
        touched = 0;
        if (alarm.isActive()) {
          alarm.deactivate();
        }
        else {
          switch(lightState) {
            case 0: 
              light.increaseTo(0.55f);
              lightState++;
              break;
            case 1:  
              light.increaseTo(0.7f);
              lightState++;
              break;
            case 2:  
              light.increaseTo(0.85f);
              lightState++;
              break;
            case 3:
              light.increaseTo(0.0f);
              lightState = 0;
              break;
          }
        }
      }
    }
  }
  else sampleTime++;
}
bool Button::isHeldDown() {
  int count = 10;
  while (digitalRead(buttonPin) == HIGH && count > 0) {
    count--;
    delay(100);
  }
  return count==0;
}
