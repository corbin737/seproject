#include "ui.h"
#include <stdlib.h>
#include <stdio.h>
#include <OrbitOled.h>
#include <OrbitOledGrph.h>
#include "track.h"



//void drawTrack(tile *onScreen, int offset) {
//  int row = 0;
//  int tileX, tileY;
//  int shift = TILE_HEIGHT - offset;
//
//  ///iterates through on screen elements and draws them on screen
//  for (tile *i = onScreen; i != NULL; i = i->next) {
//
//    for (int column = 0; column < sizeof(i->value)/sizeof(i->value[0]); column++) {
//      tileX = SCREEN_HEIGHT - (row * TILE_HEIGHT) - TILE_HEIGHT - TILE_HEIGHT_BORDER;
//      tileY = column * TILE_WIDTH + TILE_WIDTH_BORDER;
//      if (offset != 0)
//        tileX += shift;
//
//      if (TILE_HEIGHT + tileX < 0) //exits if the tile is outside the leftmost screen
//         break;
//
//      OrbitOledMoveTo(tileX, tileY);
//      if (i->value[column]) {
//        OrbitOledMoveTo(tileX + BARRICADE_BUFFER_X, tileY + BARRICADE_BUFFER_Y);
//        OrbitOledFillRect(tileX + TILE_HEIGHT - BARRICADE_BUFFER_X, tileY + TILE_WIDTH - BARRICADE_BUFFER_Y);
//      }
//    }
//    row++;
//  }
//
//  OrbitOledUpdate();
//}