/* target class */
#ifndef Target_h
#define Target_h

#include "Arduino.h"

class Target
{
  public:
    Target(int pos, uint8_t red, uint8_t green, uint8_t blue);
    int position;
    uint32_t targetColor;
};

#endif
