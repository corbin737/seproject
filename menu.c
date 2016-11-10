#include "menu.h"

#include <OrbitOled.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>

tile *head;
tile *onscreen;
int offset;
int lane;

static enum MenuPage
{
  Start = 0,
  Game = 1,
  GameOver = 2
} menuPage = Start;

void menuInit() {
  startInit();
}

void menuLoop(state hardwareState) {
  int delayInMillis;
  int leftBtnState = hardwareState.leftBtn;
  int rightBtnState = hardwareState.rightBtn;
  switch(menuPage) {
  case Start:
    trackLoopWithPushFunc(trackPushFullTile);
    startLoop(leftBtnState, rightBtnState);
    delayInMillis = 1000 / 8;
    break;
  case Game:
    trackLoopWithPushFunc(trackPushRandTile);
    carLoop(leftBtnState, rightBtnState);
    delayInMillis = 1000 / 30;
    break;
  }

  delay(delayInMillis);
}

void carLoop(int leftBtnState, int rightBtnState) {
  lane = updateCarLane(leftBtnState, rightBtnState);
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);
  drawCar(lane);
}

void startInit() {
  head = trackCreate();
  for (int i = 0; i < 7; i++) {
      head = trackPushFullTile(head);
  }
  onscreen = head->next;
  offset = 0;
}
void startLoop(int leftBtnState, int rightBtnState) {
  int bannerWidth = 64;

  if (leftBtnState == HIGH || rightBtnState == HIGH) {
    menuPage = Game;
    head = trackDelete(head);
    trackInit();
  } else {
    OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnBlank));
    OrbitOledSetDrawMode(modOledAnd);
    OrbitOledMoveTo((SCREEN_HEIGHT / 2) - (bannerWidth / 2), 0);
    OrbitOledFillRect((SCREEN_HEIGHT / 2) + (bannerWidth / 2), SCREEN_WIDTH);
  
    OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
    OrbitOledSetDrawMode(modOledSet);
    OrbitOledMoveTo((SCREEN_HEIGHT / 2) - (bannerWidth / 2), 0);
    OrbitOledDrawString("Press");
  
    OrbitOledMoveTo((SCREEN_HEIGHT / 2) - (bannerWidth / 2), SCREEN_WIDTH * 1 / 3);
    OrbitOledDrawString("Button");
  
    OrbitOledMoveTo((SCREEN_HEIGHT / 2) - (bannerWidth / 2), SCREEN_WIDTH * 2 / 3);
    OrbitOledDrawString("to Begin");
  
    OrbitOledUpdate();
  }
}

void trackInit() {
  // Populate first tiles visible on screen
  head = trackCreate();
  for (int i = 0; i < 7; i++) {
      head = trackPushBlankTile(head);
  }
  onscreen = head->next;
  offset = 0;
}

void trackLoopWithPushFunc(tile *(*pushTile)(tile *)) {
  // Update track model
  if (offset >= 21) {
    // Bottom tile has moved off screen entirely
    offset %= 21;
    head = trackPopTile(head);
    head = pushTile(head);
  }
  if (offset == 1) {
    // Next tile has moved in on top of screen
    onscreen = onscreen->prev;
  }

  OrbitOledClear();
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);
  draw(onscreen, offset);
  offset++;
}
void trackLoop() {
  trackLoopWithPushFunc(trackPushRandTile);
}

