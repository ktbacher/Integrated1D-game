/* target class */
#ifndef Target_h
#define Target_h

#include "Arduino.h"

class Target
{
  public:
    Target(int pos, uint32_t color);
    int position;
    uint32_t targetColor;
};

#endif
