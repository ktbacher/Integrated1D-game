#include "Arduino.h"
#include "DisplayBuffer.h"

DisplayBuffer::DisplayBuffer(int dispSize)
{
  displaySize = dispSize;
  uint32_t displayBuffer[dispSize];
  uint32_t initColor = uint32_t(0); 
  for(int i = 0; i < displaySize; i++) { 
//    Serial.print(i);   
    displayBuffer[i] = initColor; 
  }
}

void DisplayBuffer::clear() {
//  Serial.print(displaySize);
  for(int i = 0; i < displaySize; i++) { 
//    Serial.print(i);   
    displayBuffer[i] = initColor; 
//    Serial.println(displayBuffer[i]);
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
