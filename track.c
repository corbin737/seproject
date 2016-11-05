#include "track.h"

void setTileValue(bool val[NUMBER_OF_LANES]) {
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

tile* getTile() {
    tile *t = malloc(sizeof(tile));
    t->next = NULL;
    t->prev = NULL;
    for (int i = 0; i < NUMBER_OF_LANES; i++) {
        t->value[i] = false;
    }

    setTileValue(t->value);

    return t;
}

tile *trackCreate() {
    srand((unsigned)time(NULL));
    return NULL;
}

tile *trackPushRandTile(tile *oldHead) {
    tile *newHead = getTile();
    newHead->next = oldHead;
    if (oldHead) {
        oldHead->prev = newHead;
    }
    return newHead;
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
