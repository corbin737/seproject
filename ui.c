#include "ui.h"
#include <stdlib.h>
#define TILE_WIDTH 10
#define TILE_WIDTH_BORDER 1
#define TILE_HEIGHT 21
#define TILE_HEIGHT_BORDER 1
#define BARRICADE_WIDTH 8
#define BARRICADE_HEIGHT 19
#define BARRICADE_BUFFER_X (TILE_WIDTH-BARRICADE_WIDTH)/2
#define BARRICADE_BUFFER_Y (TILE_HEIGHT-BARRICADE_HEIGHT)/2
#define SCREEN_HEIGHT 128
#define SCREEN_WIDTH 32

int screenVerticalLimit (int y) {
  if (y > 127)
    y = 127;
  else if (y < 0)
    y = TILE_HEIGHT_BORDER;
  return y;
}

void draw(tile *onScreen, int offset) {
  int row = 0;
  int tileX, tileY;
  int shift = TILE_HEIGHT - offset;

  ///iterates through on screen elements and draws them on screen
  for (tile *i = onScreen; i != NULL; i = i->next) {
    for (int column = 0; column < sizeof(i->value)/sizeof(i->value[0]); column++) {
      tileX = column * TILE_WIDTH + TILE_WIDTH_BORDER;
      tileY = row * TILE_HEIGHT + TILE_HEIGHT_BORDER;
      
      if (offset != 0)
        tileY -= shift;
      tileY = screenVerticalLimit(tileY);
      OrbitOledMoveTo(tileX, tileY);
      if (i->value[column]) {
        OrbitOledMoveTo(tileX + BARRICADE_BUFFER_X, screenVerticalLimit(tileY + BARRICADE_BUFFER_Y));
        OrbitOledFillRect(tileX + TILE_WIDTH - BARRICADE_BUFFER_X, screenVerticalLimit(tileY + TILE_HEIGHT - BARRICADE_BUFFER_Y));
      }
    }
    row++;
  }



}


