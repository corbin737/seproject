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
}

void loop() {
  // put your main code here, to run repeatedly: 
  OrbitOledClear();
  draw(onscreen, offset);
  OrbitOledUpdate();
  delay(5000);
  onscreen = onscreen->prev;
  offset += 5;
  OrbitOledClear();
  draw(onscreen, offset);
  OrbitOledUpdate();
  delay(5000);
  offset += 5;
  OrbitOledClear();
  draw(onscreen, offset);
  OrbitOledUpdate();
  delay(5000);
}
