/* player class */
#ifndef Player_h
#define Player_h

#include "Arduino.h"

class Player
{
  public:
    Player(int pos, uint8_t dispSize, uint8_t red, uint8_t green, uint8_t blue);
    void move(int dir);
    int position;
    int score;
    int displaySize;
    uint32_t playerColor;
};

#endif
