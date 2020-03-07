#include "Arduino.h"
#include "Player.h"

Player::Player(int pos, uint8_t dispSize, uint8_t red, uint8_t green, uint8_t blue)
{
  position = pos;
  score = 0;
  displaySize = dispSize;
  playerColor = uint32_t(red, green,blue);
}

void Player::move(int dir) {
	position += dir;

	if (position == -1) {
    	position = displaySize - 1;
    } else if (position == displaySize) {
    	position = 0;
    }
}
