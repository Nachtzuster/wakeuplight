#include "sound.h"

#include "sample_mp3.h"

void Sound::setup() {
  // First, preallocate all the memory needed for the codecs, never to be freed
  preallocateCodec = malloc(preallocateCodecSize);
  if (!preallocateCodec) {
    Serial.printf_P(PSTR("FATAL ERROR:  Unable to preallocate %d bytes for app\n"), preallocateCodecSize);
    while (1) delay(1000); // Infinite halt
  }
  audioLogger = &Serial;
}

void Sound::loop() {
  if (is_active) {
    if (!gen->isRunning()) {
      if (millis() > playAt) {
        source->open(sample_mp3, sizeof(sample_mp3));
        int status = gen->begin(source, out);
        if (status != 1) {
          Serial.printf_P(PSTR("# Sound::loop start failed status: %d\n"), status);
        }
        else playing = true;
      }
    } else {
      if (!gen->loop()) {
        gen->stop();
        playing = false;
        playAt = millis() + random(10000, 20000);
      }
    }
  }
}

void Sound::sound() {
  if (!is_active) {
    gen = new AudioGeneratorMP3(preallocateCodec, preallocateCodecSize);
    out = new AudioOutputI2S();
    source = new AudioFileSourcePROGMEM();
    is_active = true;
  }
}

void Sound::mute() {
  if (is_active) {
    gen->stop();
    /* workaround for bug in 1.9.7: i2s_end() is not being called if output is stopped, so call out->begin()
       should be fixed here: https://github.com/earlephilhower/ESP8266Audio/pull/568 */
    out->begin();
    delete source;
    delete gen;
    delete out;
    source = NULL;
    gen = NULL;
    out = NULL;
    is_active = false;
    playing = false;
    playAt = 0;
  }
}
