#include "types.h"
#include "track.h"
#include "ui.h"
#include "car.h"
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>

tile *head;
tile *onscreen;
int offset;
Car *test;

const int leftBtnPin = PE_0;
const int rightBtnPin = PD_2;
int leftBtnState;
int rightBtnState;
int lane;

void setup() {
  OrbitOledInit();
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);


  pinMode(leftBtnPin, INPUT); 
  pinMode(rightBtnPin, INPUT);


  // Populate first tiles visible on screen
  head = trackCreate();
  for (int i = 0; i < 7; i++) {
      head = trackPushRandTile(head);
  }
  onscreen = head->next;
  offset = 0;
}

void loop() {
  
  // Update track model
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

  // Update car model
  leftBtnState = digitalRead(leftBtnPin);
  rightBtnState = digitalRead(rightBtnPin);
  lane = updateCarLane(leftBtnState, rightBtnState);

  OrbitOledClear();
  draw(onscreen, offset);
  drawCar(test, lane);
  delay(1000 / 30);
  offset++;
}
