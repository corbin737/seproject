#ifndef TRACK_H
#define TRACK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <stdbool.h>
#include "ui.h"
#include "types.h"

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
