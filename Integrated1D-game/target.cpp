#include "Arduino.h"
#include "Target.h"

Target::Target(int pos, uint8_t red, uint8_t green, uint8_t blue)
{
  position = pos;
  targetColor = uint32_t(red, green, blue);
}
