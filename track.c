/*******************************************************************************
 * Program: track.c
 * Author: Corbin McElhanney, Kalvin Thye
 * Description: Functions generating and drawing the track
 * Last Modified: November 24, 2016
 ******************************************************************************/
#include "track.h"

#define BARRICADE_WIDTH 4
#define BARRICADE_HEIGHT 17
const int barricadeHeightBuffer  = (TILE_HEIGHT-BARRICADE_HEIGHT)/2;
const int barricadeWidthBuffer  = (TILE_WIDTH-BARRICADE_WIDTH)/2;

typedef enum {
  Random,
  Blank,
  Full
} TileCreateType;

// Helper function
tile* trackPushTile(tile* oldHead, TileCreateType createType);

// Adds a randomly generated tile to head
tile *trackPushRandTile(tile *oldHead) {
  return trackPushTile(oldHead, Random);
}

// Adds a tile with no obstacles to head
tile *trackPushBlankTile(tile *oldHead) {
  return trackPushTile(oldHead, Blank);
}

// Adds a tile with all three obstacles to head
tile *trackPushFullTile(tile *oldHead) {
  return trackPushTile(oldHead, Full);
}

// Creates an empty track model
tile *trackCreate() {
  srand(millis());
  return NULL;
}

// Frees last tile in model
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

// Frees a given track model
tile *trackDelete(tile *head) {
    while (head != NULL) {
        head = trackPopTile(head);
    }
    return NULL;
}

// Draws tiles to the screen starting at onscreen until
// the whole screen is filled
void drawTrack(tile *onScreen, int offset) {
  int row = 0;
  int tileX, tileY;
  int shift = TILE_HEIGHT - offset;

  ///iterates through on screen elements and draws them on screen
  for (tile *i = onScreen; i != NULL; i = i->next) {

    for (int column = 0; column < sizeof(i->value)/sizeof(i->value[0]); column++) {
      tileX = SCREEN_HEIGHT - (row * TILE_HEIGHT) - TILE_HEIGHT - SCREEN_HEIGHT_BORDER;
      tileY = column * TILE_WIDTH + SCREEN_WIDTH_BORDER;
      if (offset != 0)
        tileX += shift;

      if (TILE_HEIGHT + tileX < 0) //exits if the tile is outside the leftmost screen
         break;

      OrbitOledMoveTo(tileX, tileY);
      if (i->value[column]) {
        OrbitOledMoveTo(tileX + barricadeHeightBuffer, tileY + barricadeWidthBuffer);
        OrbitOledDrawRect(tileX + TILE_HEIGHT - barricadeHeightBuffer, tileY + TILE_WIDTH - barricadeWidthBuffer);
        OrbitOledMoveTo(tileX + barricadeHeightBuffer + 2, tileY + barricadeWidthBuffer + 2);
        OrbitOledFillRect(tileX + TILE_HEIGHT - barricadeHeightBuffer - 2, tileY + TILE_WIDTH - barricadeWidthBuffer - 2);
      }
    }
    row++;
  }
}

// Adds a tile of the given type to head
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

// Sets a random tile pattern
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
