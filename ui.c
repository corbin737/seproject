#include "ui.h"
#include <stdlib.h>
#include <stdio.h>
#include <OrbitOled.h>
#include <OrbitOledGrph.h>
#include "track.h"

#define TILE_WIDTH 10
#define TILE_WIDTH_BORDER 1
#define TILE_HEIGHT 21
#define TILE_HEIGHT_BORDER 1
#define BARRICADE_WIDTH 8
#define BARRICADE_HEIGHT 19
#define BARRICADE_BUFFER_X (TILE_WIDTH-BARRICADE_WIDTH)/2
#define BARRICADE_BUFFER_Y (TILE_HEIGHT-BARRICADE_HEIGHT)/2
#define SCREEN_HEIGHT 127
#define SCREEN_WIDTH 32

int screenVerticalLimit (int y) {
  if (y > 127)
    y = 127;
  else if (y < 0)
    y = TILE_HEIGHT_BORDER;
  return y;
}

//void OrbitOledMoveTo(int a, int b) {
//  printf("Move Cursor to:(%d, %d)\n", a, b);
//}
//
//void OrbitOledFillRect(int a, int b) {
//  printf("Draw to: (%d, %d)\n", a, b);
//}

void draw(tile *onScreen, int offset) {
  int row = 0;
  int tileX, tileY;
  int shift = TILE_HEIGHT - offset;

  ///iterates through on screen elements and draws them on screen
  for (tile *i = onScreen; i != NULL && row < 7; i = i->next) {
    if ((SCREEN_HEIGHT - row * TILE_HEIGHT - TILE_HEIGHT_BORDER) < 0)
    break;
    for (int column = 0; column < sizeof(i->value)/sizeof(i->value[0]); column++) {
      tileX = SCREEN_HEIGHT - row * TILE_HEIGHT - TILE_HEIGHT_BORDER;
      tileY = column * TILE_WIDTH + TILE_WIDTH_BORDER;
      if (offset != 0)
        tileX += shift;
      tileX = screenVerticalLimit(tileX);
      OrbitOledMoveTo(tileX, tileY);
      if (i->value[column]) {
        OrbitOledMoveTo(screenVerticalLimit(tileX - BARRICADE_BUFFER_X), tileY + BARRICADE_BUFFER_Y);
        OrbitOledFillRect(screenVerticalLimit(tileX - TILE_WIDTH + BARRICADE_BUFFER_X), tileY + TILE_HEIGHT - BARRICADE_BUFFER_Y);
      }
    }
    row++;
  }

  OrbitOledUpdate();
}

/*int main(void) {
  tile *test = tileCreate();
  test = trackPopTile(test);
  test = trackPopTile(test);

  for (tile *i = test; i != NULL; i = i->next) {
      printf("%d\n", i->value[0]);
  }
}*/

