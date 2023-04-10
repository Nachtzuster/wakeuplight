#ifndef NEO_PIX_H
#define NEO_PIX_H

#include <NeoPixelBus.h>
#include "light.h"
#include "configuration.h"
#include "localclock.h"

#define NUM_LEDS 49

class Pixel : public Light {
private:
  NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart1Ws2813Method> strip;

  HsbColor dark = HsbColor(0.0f,1.0f,0.01f);
  HsbColor red = HsbColor(0.02f,0.9f,0.12f);
  HsbColor yellow = HsbColor(0.10f,0.8f,0.25f);
  HsbColor white = HsbColor(0.15f,0.5f,1.0f);

public:
  Pixel();
  void increaseTo(float scale);
  void setup();
  void loop();
};

#endif
