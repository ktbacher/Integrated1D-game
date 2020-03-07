/* player class */
#ifndef DisplayBuffer_h
#define DisplayBuffer_h

#include "Arduino.h"

class DisplayBuffer
{
  public:
    DisplayBuffer(int dispSize );
    void clear();
    void setPixel(int  _index, uint32_t _c);
    void setAllPixels(uint32_t _c);
    uint32_t displayBuffer[];
  private:
    int displaySize;
    uint32_t initColor;

    // also need to add color
};

#endif
