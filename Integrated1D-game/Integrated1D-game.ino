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

const int buttonPin1 = 4;
const int buttonPin2 = 5;
int buttonState1 = 1;
int buttonState2 = 1;

int old_pos1 = 0;
int old_pos2 = 0;
int pos1 = 0;
int pos2 = 0;

uint32_t magenta = 16711935;
//    uint32_t cyan = 3325695;
uint32_t cyan = 38655;
uint32_t white = 16777215;
uint32_t yellow = 16775680;
uint32_t red = 16711780;
uint32_t blue = 255;
    
//int inByte = 0;         // incoming serial byte

static int displaySize = 24;
DisplayBuffer display = DisplayBuffer(displaySize);
Player playerOne =  Player(int(displaySize/4), displaySize, red );             // Adding 2 players to the game
Player playerTwo =  Player(int(3*displaySize/4), displaySize, blue );
Target target1[5] = { Target(int(displaySize/4), magenta),Target(0, 0), Target(0, 0),Target(0, 0),Target(0, 0)  };              // and one target for players to catch.
int size1 = 1;
Target target2[5] = { Target(int(3*displaySize/4), cyan), Target(0, 0),Target(0,0),Target(0, 0),Target(0,0), };
int size2 = 1;

//Score score = new Score(color(0,0,0));   // Used to display winner's color  
int scoreMax = 4;

//Controller controller = Controller(); 

int gameState = 1;
int currentFrame = 0;
int appFramerate = 30;
int numFrames =  30;
uint32_t scoreColor = 0;

bool stunned1 = false;
bool stunned2 = false;
unsigned long timeout1 = 0;
unsigned long timeout2 = 0;
void setup() {
  
  Serial.begin(57600);      // start serial port at 57600 bps:
  pixels.begin();
  pixels.setBrightness(255);                // Set LED brightness 0-255
  LEDsOff();

  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
}



void loop() {
  //read data each loop
    encoder1.tick();
    encoder2.tick();
    pos1 = encoder1.getPosition();
    pos2 = encoder2.getPosition();
//    Serial.print(pos1);
//    Serial.print(' ');
//    Serial.println(pos2);

    if (!stunned1 || millis()-timeout1 > 3000) {
      if (stunned1) {
        playerOne.position = int(displaySize/4);
        stunned1 = false;
      } else {
        playerOne.move(pos1-old_pos1);
      }
      old_pos1 = pos1;
    }
   
    if (!stunned2 || millis()-timeout2 > 3000) {
      if (stunned2) {
        playerTwo.position = int(3*displaySize/4);
        stunned2 = false;
      } else {
        playerTwo.move(pos2-old_pos2);
      }
      old_pos2 = pos2;
    }

    update();

}


void update() {
//  printDisplay(display.displayBuffer);
  switch (gameState) {
    case 1: //Play
      display.clear();
      //set nuetral zone... change to yellow?
      display.setPixel(0, yellow);
      display.setPixel(displaySize-1, yellow);
      display.setPixel(int((displaySize-1)/2),yellow);
      display.setPixel(int((displaySize+1)/2), yellow);
      
      // now add the targets
      for (int i=0; i<size1; i++) {
        display.setPixel(target1[i].position, target1[i].targetColor);
        // check if player two caught player 1's flags
        if (playerTwo.position == target1[i].position)  {
          playerTwo.score++;
          int offset = (2*(size2%2)-1)*int((size2+1)/2);
          target2[size2] = Target(int(3*displaySize/4)+offset, cyan);
          size2 ++;
          gameState = 3;
          colorWipe(playerTwo.playerColor, 0);
          delay(1000);
        }
      }
      for (int i=0; i<size2; i++) {
        display.setPixel(target2[i].position, target2[i].targetColor);
        // check if other player has caught target        
        if (playerOne.position == target2[i].position)  {
          playerOne.score++;
          int offset = (2*(size1%2)-1)*int((size1+1)/2);
          target1[size1] = Target(int(displaySize/4)+offset, magenta);
          size1 ++;
          gameState = 3;
          colorWipe(playerOne.playerColor, 0);
          delay(1000);
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

        if (playerOne.position > int(displaySize/2)) {
          playerOne.position = int(displaySize/4);
          stunned1 = true;
          timeout1 = millis();
        } else {
          playerTwo.position = int(3*displaySize/4);
          stunned2 = true;
          timeout2 = millis();
        }
        gameState = 1;

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

      // BUTTON push to restart
      buttonState1 = digitalRead(buttonPin1);
      buttonState2 = digitalRead(buttonPin2);

      if (!buttonState1 || !buttonState2) {
        // reset game
        playerOne.position = int(displaySize/4);
        playerTwo.position = int(3*displaySize/4);
        size1 = 1;
        size2 = 1;
        gameState = 1;
      }
      break;
      
    // Not used, it's here just for code consistency
    default:
      break;
  }
  show();
}


void show() { 
//  Serial.println("~~~~showing");
  for(int i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, display.displayBuffer[i]);
//    Serial.print(String(display.displayBuffer[i]) + " ");
  }
//  Serial.println();
  pixels.show();
}

void LEDsOff() {
  for(int i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, 0);
      pixels.show();
  }
//  delay(10000);
}

    void colorWipe(uint32_t c, uint32_t wait) {
      for(uint16_t i=0; i<pixels.numPixels(); i++) {
          pixels.setPixelColor(i, c);
          pixels.show();
          delay(wait);
      }
      
    }
void printDisplay(uint32_t buffer[]) {
  Serial.println("displaying buffer: ");
  for (int i=0; i<pixels.numPixels(); i++) {
    Serial.print(buffer[i]);
    Serial.print(" ");
  }
  Serial.println();
}
