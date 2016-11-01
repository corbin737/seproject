#ifndef TRACK_H
#define TRACK_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "types.h"

tile *trackCreate();
tile *trackPushRandTile(tile *head);
tile *trackPopTile(tile *head);
tile *trackDelete(tile *head);

#endif
