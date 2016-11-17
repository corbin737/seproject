#include "track.h"

#define BARRICADE_WIDTH 4
#define BARRICADE_HEIGHT 17

const int tileWidthBorder = 1;
const int tileHeightBorder = 1;
const int barricadeBufferX = (TILE_HEIGHT-BARRICADE_HEIGHT) / 2;
const int barricadeBufferY = (TILE_WIDTH-BARRICADE_WIDTH) / 2;

typedef enum {
  Random,
  Blank,
  Full
} TileCreateType;

tile* trackPushTile(tile* oldHead, TileCreateType createType);

tile *trackPushRandTile(tile *oldHead) {
    return trackPushTile(oldHead, Random);
}

tile *trackPushBlankTile(tile *oldHead) {
  return trackPushTile(oldHead, Blank);
}

tile *trackPushFullTile(tile *oldHead) {
  return trackPushTile(oldHead, Full);
}

tile *trackCreate() {
    srand(millis());
    return NULL;
}

tile *trackPopTile(tile *head) {
    if (head == NULL) return NULL;
    if (head->next == NULL) {
        free(head);
        return NULL;
    }

    tile *lastTile = head;
    while (lastTile->next != NULL) {
        lastTile = lastTile->next;
    }
    lastTile->prev->next = NULL;
    free(lastTile);
    return head;
}

tile *trackDelete(tile *head) {
    while (head != NULL) {
        head = trackPopTile(head);
    }
    return NULL;
}

void drawTrack(tile *onScreen, int offset) {
  int row = 0;
  int tileX, tileY;
  int shift = TILE_HEIGHT - offset;

  ///iterates through on screen elements and draws them on screen
  for (tile *i = onScreen; i != NULL; i = i->next) {

    for (int column = 0; column < sizeof(i->value)/sizeof(i->value[0]); column++) {
      tileX = SCREEN_HEIGHT - (row * TILE_HEIGHT) - TILE_HEIGHT - tileHeightBorder;
      tileY = column * TILE_WIDTH + tileWidthBorder;
      if (offset != 0)
        tileX += shift;

      if (TILE_HEIGHT + tileX < 0) //exits if the tile is outside the leftmost screen
         break;

      OrbitOledMoveTo(tileX, tileY);
      if (i->value[column]) {
        OrbitOledMoveTo(tileX + barricadeBufferX, tileY + barricadeBufferY);
        OrbitOledFillRect(tileX + TILE_HEIGHT - barricadeBufferX, tileY + TILE_WIDTH - barricadeBufferY);
      }
    }
    row++;
  }

  OrbitOledUpdate();
}


tile* trackPushTile(tile* oldHead, TileCreateType createType) {
    tile *newHead = malloc(sizeof(tile));
    newHead->next = oldHead;
    newHead->prev = NULL;
    if (oldHead) {
      oldHead->prev = newHead;
    }
    for (int i = 0; i < NUMBER_OF_LANES; i++) {
      newHead->value[i] = false;
    }

    switch(createType) {
    case Random:
      setRandomTileValue(newHead->value);
      break;
    case Blank:
      // Leave as all false
      break;
    case Full:
      for (int i = 0; i < NUMBER_OF_LANES; i++) {
        newHead->value[i] = true;
      }
      break;
    }

    return newHead;
}

void setRandomTileValue(bool val[NUMBER_OF_LANES]) {
    // Requires that NUMBER_OF_LANES == 3
    // Otherwise, changes to logic must be made
    static bool isExit[NUMBER_OF_LANES] = {true, true, true};
    int nExits = 0;
    for (int i = 0; i < NUMBER_OF_LANES; i++)
        if (isExit[i]) nExits++;

    switch (nExits) {
    case 1:
        if (rand() % 2 == 0) {
            // Place one block on non-exit
            int i;
            do i = rand() % 3;
            while (isExit[i]);
            val[i] = true;
        } else {
            // Place two blocks on non-exits
            for (int i = 0; i < NUMBER_OF_LANES; i++) {
                if (!isExit[i]) {
                    val[i] = true;
                }
            }
        }
        break;
    case 2:
        if (isExit[1]) {
            // Adjacent exits
            if (rand() % 2 == 0) {
                // Place one block randomly
                val[rand() % 3] = true;
            } else {
                // Place block on the non-exit
                for (int i = 0; i < NUMBER_OF_LANES; i++)
                    if (!isExit[i]) val[i] = true;
                // Place block on one exit randomly
                int i;
                do i = rand() % 3;
                while (!isExit[i]);
            }
        } else {
            // Nonadjacent exits
            if (rand() % 2 == 0) {
                val[1] = true;
            }
        }
        break;
    case 3:
        if (rand() % 2 == 0) {
            // Place one block randomly
            val[rand() % 3] = true;
        } else {
            // Place two blocks randomly
            int i = rand() % 3;
            int j;
            do j = rand() % 3;
            while (j == i);
            val[i] = true;
            val[j] = true;
        }
        break;
    default:
        break;
    }

    // Update isExit array for next invocation
    for (int i = 0; i < NUMBER_OF_LANES; i++) {
        if (val[i]) {
            isExit[i] = false;
        } else {
            isExit[i] = true;
        }
    }
}
