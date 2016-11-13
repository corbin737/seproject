#include <stdlib.h>
#include "collision.h"
#include "car.h"
#include "ui.h"

int checkCollision (int lane) {
  int x = middleY + TILE_HEIGHT_BORDER;
  int y = lane*TILE_WIDTH + TILE_WIDTH_BORDER;
  for (int i = y; i < y+TILE_WIDTH; i++) {
    OrbitOledMoveTo(x, i);
    if (OrbitOledGetPixel() == 1) {
      return 0;
    }
  }
  return 1;
}

//int checkCollision(tile *onScreen, int offset, int lane) {
//  int x = 0;
//  int y = 0;
//  int *x0 = &x;
//  int *y0 = &y;
//  getCar(x0, y0, lane);
//  
//  tile *lastTile = getLastTile(onScreen, offset);
//  tile *secondLastTile = lastTile->prev;
//  int tileX, tileY;
//  int row = 1;
//
//  //for (tile *k = lastTile; k->next != lastTile; k->prev) {
//      for (int column = 0; column < sizeof(lastTile->value)/sizeof(lastTile->value[0]); column++) {
//        tileX = row*TILE_HEIGHT + TILE_HEIGHT_BORDER;
//        tileY = column * TILE_WIDTH + TILE_WIDTH_BORDER;
//        
//        tileX -= offset;
//          
//        if (lastTile->value[column] == 1) {    
//          if ((tileX + BARRICADE_BUFFER_X - TILE_HEIGHT) <= x && x <= tileX - BARRICADE_BUFFER_X) {
//            //if (tileY <= y && y <= tileY + TILE_WIDTH - BARRICADE_BUFFER_Y) 
//             return 1; 
//          }
//        }
//      }
//      row++;
//    //}
//    return 0;
//}

