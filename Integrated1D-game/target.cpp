#include "Arduino.h"
#include "Target.h"

Target::Target(int pos, uint32_t color)
{
  position = pos;
  targetColor = color;
}
