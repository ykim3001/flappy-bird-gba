#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/bigbird.h"
#include "images/fbird9999.h"
#include "images/fbirdgameover.h"
#include "images/fbirdplay.h"
#include "images/fbirdsprite.h"
#include "images/fbirdstart.h"
#include "images/pipe2.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"

/* TODO: */
// Add any additional states you need for your app. You are not requried to use
// these specific provided states.
enum gba_state {
  START,
  STARTTEXT,
  INITPLAY,
  PLAY,
  WIN,
  LOSE,
};

char buffer[52];

int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;
  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  enum gba_state state = START;

  struct birdData curBird;
  struct birdData oldBird;
  curBird.x = 10;
  curBird.y = 100;
  oldBird = curBird;
  int randX = randint(0, 240);
  int randY = randint(0, 100);
  int moves = 50;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw
    waitForVBlank();
    switch (state) {
      case START:
        drawFullScreenImageDMA(fbirdstart);
        randX = randint(0, 200);
        randY = randint(0, 100);
        curBird.x = 10;
        curBird.y = 100;
        moves = 50;
        oldBird = curBird;

        state = STARTTEXT;
        break;
        case STARTTEXT:
        drawString(105, 50, "PRESS ENTER KEY TO START", BLACK);

        if (KEY_DOWN(BUTTON_START, currentButtons)) {  
          state = INITPLAY;  
          waitForVBlank(); 
          drawFullScreenImageDMA(fbirdplay);
        }
        break;
      case INITPLAY:
        //drawFullScreenImageDMA(fbirdplay);
        drawImageDMA(randY, randX, BIGBIRD_WIDTH, BIGBIRD_HEIGHT, bigbird);
        //drawImageDMA(oldBird.y, oldBird.x, FBIRDSPRITE_WIDTH, FBIRDSPRITE_HEIGHT, BLACK);
        drawImageDMA(curBird.y, curBird.x, FBIRDSPRITE_WIDTH, FBIRDSPRITE_HEIGHT, fbirdsprite);
        drawImageDMA(130, 200, PIPE2_WIDTH, PIPE2_HEIGHT, pipe2);
        //drawString(150, 110, "USE ARROW KEY TO MOVE", BLACK);
        state = PLAY;
        break;
      case PLAY:
        //drawFullScreenImageDMA(fbirdplay);

        undrawImageDMA(curBird.x, curBird.y, FBIRDSPRITE_WIDTH, FBIRDSPRITE_HEIGHT, fbirdplay);
        undrawImageDMA(oldBird.x, oldBird.y, FBIRDSPRITE_WIDTH, FBIRDSPRITE_HEIGHT, fbirdplay);
        undrawImageDMA(150,5,100,80,fbirdplay);

        drawRectDMA(oldBird.y, oldBird.x, FBIRDSPRITE_WIDTH, FBIRDSPRITE_HEIGHT, WHITE);
        drawImageDMA(curBird.y, curBird.x, FBIRDSPRITE_WIDTH, FBIRDSPRITE_HEIGHT, fbirdsprite);
        //drawImageDMA(130, 200, PIPE2_WIDTH, PIPE2_HEIGHT, pipe2);
        drawString(150, 5, buffer, RED);
        drawString(150, 110, "USE ARROW KEY TO MOVE", BLACK);
        //drawString(5, 0, "PRESS BACKSPACE TO RESET", WHITE);
        oldBird = curBird;
        sprintf(buffer, "Flapping : %d", moves);
        if ((curBird.x > randX - 20) && (curBird.x < randX + 20) && (curBird.y < randY + 20) && (curBird.y > randY - 20)) {
           state = LOSE;
           break;
        }
        if ((curBird.x > 210) && (curBird.y > 80)) {
           state = WIN;
           break;
        }
        
        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
           state = START;
        }
        if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons) ) {
           curBird.y -= 7;
           moves--;
           if (moves == 0) {
            state = LOSE;
           }
           if (curBird.y < 0)
              curBird.y = 0;
        }
        if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons) ) {
           curBird.y += 7;
           moves--;
           if (moves == 0) {
            state = LOSE;
           }
           if (curBird.y + 15 > 160)
               curBird.y = 150;
        }
        if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons) ) {
           curBird.x -= 7;
           moves--;
           if (moves == 0) {
            state = LOSE;
           }
           if (curBird.x < 0)
              curBird.x = 0;
        }
        if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons) ) {
           curBird.x += 7;
           moves--;
           if (moves == 0) {
            state = LOSE;
           }
           if (curBird.x + 15 > 240)
              curBird.x = 225;
        }

        break;
      case WIN:
        waitForVBlank();
        drawFullScreenImageDMA(fbird9999);
        //drawString(34, 50, "PRESS BACKSPACE TO RESET", BLACK);
        if (KEY_DOWN(BUTTON_SELECT, currentButtons))
          state = START;
        break;
      case LOSE:
        waitForVBlank();
        drawFullScreenImageDMA(fbirdgameover);
        //drawString(34, 50, "PRESS BACKSPACE TO RESET", BLACK);
        if (KEY_DOWN(BUTTON_SELECT, currentButtons))
          state = START;
        break;
    }
    previousButtons = currentButtons; // Store the current state of the buttons
  }

  UNUSED(previousButtons); // You can remove this once previousButtons is used

  return 0;
}
