#include "Arduino.h"
#include "Player.h"

Player::Player(int pos, uint8_t dispSize, uint32_t color)
{
  position = pos;
  score = 0;
  displaySize = dispSize;
  playerColor = color;
}

void Player::move(int dir) {
	position += dir;

	if (position == -1) {
    	position = displaySize - 1;
    } else if (position == displaySize) {
    	position = 0;
    }
}
