/* Author: Frederik Slos <frederik.slos@gmail.com>, 2019.
 * This code is in the public domain, please see file LICENSE.txt. */

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
