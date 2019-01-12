#ifndef LIGHT_H
#define LIGHT_H

class Light {
public:
  Light() {};
  virtual void increaseTo(float scale) {};
  virtual void setup() {};
  virtual void loop() {};
};

#endif
