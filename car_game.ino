#include "types.h"
#include "track.h"
#include "ui.h"
#include "car.h"
#include <OrbitOled.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>

tile *head;
tile *onscreen;
int offset;
Car *test;

void setup() {
  OrbitOledInit();
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);

  // Populate first tiles visible on screen
  head = trackCreate();
  for (int i = 0; i < 7; i++) {
      head = trackPushRandTile(head);
  }
  onscreen = head->next;
  offset = 0;
}

void loop() {
  if (offset >= 21) {
    // Bottom tile has moved off screen entirely
    offset %= 21;
    head = trackPushRandTile(head);
    head = trackPopTile(head);
  }
  if (offset == 1) {
    // Next tile has moved in on top of screen
    onscreen = onscreen->prev;
  }

  OrbitOledClear();
  draw(onscreen, offset);
  drawCar(test, 1);
  delay(1000 / 30);
    offset++;

}
