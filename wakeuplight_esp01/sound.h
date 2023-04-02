#ifndef SOUND_H
#define SOUND_H

#include <AudioFileSourcePROGMEM.h>
#include <AudioGenerator.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>


const int preallocateCodecSize = 29192; // MP3 codec max mem needed, see AudioGeneratorMP3.preAllocSize()

class Sound {
private:
  void *preallocateCodec = NULL;
  bool is_active = false;
  unsigned long int playAt = 0;
  AudioFileSourcePROGMEM *source;
  AudioGenerator *gen;
  AudioOutputI2S *out;
public:
  boolean playing = false;
  void setup();
  void loop();
  void sound();
  void mute();
};

#endif
