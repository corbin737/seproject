#include "types.h"
#include "track.h"
#include "ui.h"
#include <OrbitOled.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>

tile *head;
tile *onscreen;
int offset;

void setup() {
  // put your setup code here, to run once:
  OrbitOledInit();
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);
  
  head = trackCreate();
  for (int i = 0; i < 7; i++) {
      head = trackPushRandTile(head);
  }
  onscreen = head->next;
  offset = 0;
  int counter = 0;
}

void loop() {
  if (offset >= 21) {
  offset %= 21;
  head = trackPushRandTile(head);
  free(head->next->next->next->next->next->next->next);
  }
  if (offset == 1)
  onscreen = onscreen->prev;
  
  OrbitOledClear();
  draw(onscreen, offset);
  delay(200);
  offset++;


}
