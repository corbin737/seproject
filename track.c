#include "track.h"

tile* getTile() {
    tile *t = malloc(sizeof(tile));
//    assert(t != NULL);
    t->next = NULL;
    t->prev = NULL;
    for (int i = 0; i < NUMBER_OF_LANES; i++) {
        t->value[i] = false;
    }

    // Basic initialization for now
    // Implement more complex formula later
    t->value[0] = true;

    return t;
}

tile *trackCreate() {
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
