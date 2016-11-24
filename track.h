/*******************************************************************************
 * Program: track.h
 * Author: Corbin Mcelhinney, Kalvin Thye
 * Description: Header file that contains functions for creating/drawing track
 * Last Modified: November 24, 2016
 ******************************************************************************/
#ifndef TRACK_H
#define TRACK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <stdbool.h>
#include "defs.h"

struct tile {
  struct tile *prev;
  struct tile *next;
  bool value[NUMBER_OF_LANES];
};
typedef struct tile tile;

tile *trackCreate();
tile *trackPushRandTile(tile *head);
tile *trackPushBlankTile(tile *head);
tile *trackPushFullTile(tile *head);
tile *trackPopTile(tile *head);
tile *trackDelete(tile *head);

void drawTrack(tile *onScreen, int offset);

#ifdef __cplusplus
}
#endif

#endif
