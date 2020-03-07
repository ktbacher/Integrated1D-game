#include "Arduino.h"
#include "DisplayBuffer.h"

DisplayBuffer::DisplayBuffer(int dispSize)
{
  displaySize = dispSize;
  uint32_t displayBuffer[dispSize];
  uint32_t initColor = uint32_t(0, 0, 0); 
}

void DisplayBuffer::clear() {

  for(int i = 0; i < displaySize; i++) {    
    displayBuffer[i] = initColor; 
  }
}

// Color a specific pixel in the buffer
void DisplayBuffer::setPixel(int  _index, uint32_t _c) {
    displayBuffer[_index]  = _c;
}

void DisplayBuffer::setAllPixels(uint32_t _c) {
     
    for(int i = 0; i < displaySize; i++) { 
      setPixel(i, _c); 
    }
  }
