#include "neo_pix.h"

Pixel::Pixel() :
  strip(NUM_LEDS){
}
void Pixel::setup(){
  strip.Begin();
  strip.Show();
}
void Pixel::loop(){
}

void Pixel::increaseTo(float scale) {
  if (scale > 1.0f) scale = 1.0f;
  if (scale < 0.0f) scale = 0.0f;
  HsbColor color;
  if (scale < 0.333f) {
    color = HsbColor::LinearBlend<NeoHueBlendShortestDistance>(dark, red, scale * 3.0f);
  }
  else if ((scale >= 0.333f && scale < 0.666f)){
    color = HsbColor::LinearBlend<NeoHueBlendShortestDistance>(red, yellow, (scale - 0.333f) * 3.0f);
  }
  else {
    color = HsbColor::LinearBlend<NeoHueBlendShortestDistance>(yellow, white, (scale - 0.666f) * 3.0f);
  }
  for (int i=0; i<NUM_LEDS; i++){
    strip.SetPixelColor(i, color);
  }
  strip.Show();
  yield();
}
