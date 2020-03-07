/*


  Based on this code:
  http://www.arduino.cc/en/Tutorial/SerialCallResponse
*/

#include <RotaryEncoder.h>
#include <Adafruit_NeoPixel.h>
#include "player.h"
#include "target.h"
#include "displayBuffer.h"

#define N_PIXELS  24  // Number of pixels you are using
#define LED_PIN    2  // NeoPixel LED strand is connected to GPIO #2 / D2
Adafruit_NeoPixel  pixels = Adafruit_NeoPixel(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

RotaryEncoder encoder1(A0, A1);
RotaryEncoder encoder2(A2, A3);

int button = 0;
int pos1 = 0;
int pos2 = 0;

//int inByte = 0;         // incoming serial byte

static int displaySize = 24;
DisplayBuffer display = DisplayBuffer(displaySize);
Player playerOne =  Player(int(displaySize/4), displaySize, 255,0,0 );             // Adding 2 players to the game
Player playerTwo =  Player(int(3*displaySize/4), displaySize, 0,0,255 );
Target target1[5] = { Target(int(displaySize/4), 255,0,250),Target(0, 0,0,0), Target(0, 0,0,0),Target(0, 0,0,0),Target(0, 0,0,0)  };              // and one target for players to catch.
int size1 = 1;
Target target2[5] = { Target(int(3*displaySize/4), 0,255,255), Target(0, 0,0,0),Target(0, 0,0,0),Target(0, 0,0,0),Target(0, 0,0,0), };
int size2 = 1;

//Score score = new Score(color(0,0,0));   // Used to display winner's color  
int scoreMax = 5;

//Controller controller = Controller(); 

int gameState = 1;
int currentFrame = 0;
int appFramerate = 30;
int numFrames =  30;
uint32_t scoreColor = 0;
void setup() {
  
  Serial.begin(57600);      // start serial port at 57600 bps:
  pixels.begin();
  pixels.setBrightness(255);                // Set LED brightness 0-255
  LEDsOff();
}



void loop() {
  //read data each loop
    encoder1.tick();
    encoder2.tick();
    pos1 = encoder1.getPosition();
    pos2 = encoder2.getPosition();
//    Serial.print(pos1);
//    Serial.print(pos2);

    update(gameState);

      
}


void update(int gameState) {
  switch (gameState) {
    case 1: //Play
      display.clear();

      //set nuetral zone... change to yellow?
      display.setPixel(0, uint32_t(255,255,255));
      display.setPixel(displaySize-1, uint32_t(255,255,255));
      display.setPixel(int((displaySize-1)/2), uint32_t(255,255,255));
      display.setPixel(int((displaySize+1)/2), uint32_t(255,255,255));

      // now add the targets
      for (int i=0; i<size1; i++) {
        display.setPixel(target1[i].position, target1[i].targetColor);
        // check if player two caught player 1's flags
        if (playerTwo.position == target1[i].position)  {
          playerTwo.score++;
          int offset = (2*(size2%2)-1)*int((size2+1)/2);
          target2[size2] = Target(int(3*displaySize/4)+offset, 0,255,255);
          size2 ++;
          gameState = 3;
        }
      }
      for (int i=0; i<size2; i++) {
        display.setPixel(target2[i].position, target2[i].targetColor);
        // check if other player has caught target        
        if (playerOne.position == target2[i].position)  {
          playerOne.score++;
          int offset = (2*(size1%2)-1)*int((size1+1)/2);
          target1[size1] = Target(int(displaySize/4)+offset, 255,0,250);
          gameState = 3;
        }
      }
      // show all players in the right place, by adding them to display buffer
      display.setPixel(playerOne.position, playerOne.playerColor);
      display.setPixel(playerTwo.position, playerTwo.playerColor);

      if (playerOne.position == playerTwo.position) {
         if (playerOne.position >0 && playerOne.position < int((displaySize-1)/2)) {
           gameState = 2;
         } else if (playerOne.position > int((displaySize+1)/2) && playerOne.position < displaySize-1) {
           gameState = 2; 
         }
      }
      
      break;
    case 2: // Collision
      // animation???
      // We've hit score max, this player wins
//      if (playerOne.score >=  scoreMax)  {
//        scoreColor = playerOne.playerColor;
//        gameState = 4;
//      
//      // We've hit score max, this player wins
//      } else if (playerTwo.score >= scoreMax)  {
//        scoreColor = playerTwo.playerColor;
//        gameState = 4;
//      // We haven't hit the limit yet, keep playing
//      } else {  
        if (playerOne.position > int(displaySize/2)) {
          playerOne.position = int(displaySize/4);
        } else {
          playerTwo.position = int(3*displaySize/4);
        }
        gameState = 1;
//      }
      break;
    case 3: // Capture
      if (playerOne.score >=  scoreMax)  {
        scoreColor = playerOne.playerColor;
        gameState = 4;
      
      // We've hit score max, this player wins
      } else if (playerTwo.score >= scoreMax)  {
        scoreColor = playerTwo.playerColor;
        gameState = 4;
      
      // We haven't hit the limit yet, keep playing
      }  else {  
        playerOne.position = int(displaySize/4);
        playerTwo.position = int(3*displaySize/4);
        gameState = 1;
      }
      break;
    case 4: // Score
      // reset everyone
      playerOne.score = 0;
      playerTwo.score = 0;
      
      //light up w winner color by populating all pixels in buffer with their color
      display.setAllPixels(scoreColor);   

      // BUTTON PUSH TO RESTART?
          
      break;
      
    // Not used, it's here just for code consistency
    default:
      break;
  }
  show();
}


void show() { 
  for(int i = 0; i < displaySize; i++) {
    pixels.setPixelColor(i, display.displayBuffer[i]);
    
  }
  pixels.show();
}

void LEDsOff() {
  for(int i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, 0,0,0);
      pixels.show();
  }
}
