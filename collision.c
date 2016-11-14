/*******************************************************************************************
 * Program: collision.c
 * Author: Corbin Mcelhinney, Kalvin Thye
 * Description: Contains methods that detects collisions
 *******************************************************************************************/
  
#include <stdlib.h>
#include "collision.h"
#include "ui.h"

int checkCollision (int lane) {
  int x = middleY + TILE_HEIGHT_BORDER;
  int y = lane*TILE_WIDTH + TILE_WIDTH_BORDER;
  for (int i = y; i < y+TILE_WIDTH  ; i++) {
    OrbitOledMoveTo(x, i);
    if (OrbitOledGetPixel() == 1) {
      return 1;
    }
  }
  return 0;
}


