#include "vc.h"
#include "car.h"

#include <math.h>
#include <OrbitOled.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>


tile *head;
tile *onscreen;
int offset;
int lane;
int fpks;

int gameOverHeight;

static enum View
{
  Start = 0,
  Game = 1,
  GameOver = 2
} vcView = Start;

void vcInit() {
  startInit();
}

void vcLoop(state hardwareState) {
  int leftBtnState = hardwareState.leftBtn;
  int rightBtnState = hardwareState.rightBtn;
  int bottomSwitchState = hardwareState.bottomSwitch;
  int topSwitchState = hardwareState.topSwitch;
  switch(vcView) {
  case Start:
    trackLoopWithPushFunc(trackPushFullTile);
    fpks = 8000;
    startLoop(leftBtnState, rightBtnState);
    break;
  case Game:
    if (bottomSwitchState == HIGH) break;
    trackLoopWithPushFunc(trackPushRandTile);
    if (checkCollision(lane) == 1) {
      vcView = GameOver;
      gameOverInit();
    }
    fpks += 50;
    if (topSwitchState == HIGH) fpks += 100;
    carLoop(leftBtnState, rightBtnState);
    break;
  case GameOver:
    gameOverLoop();
    fpks = 50000;
    break;
  }

  delay(1000000 / fpks);
}

void gameOverLoop() {
  if (gameOverHeight > SCREEN_HEIGHT) {
    startInit();
    vcView = Start;
    delay(3000);
  }

  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);
  OrbitOledMoveTo(SCREEN_HEIGHT - gameOverHeight, 0);
  OrbitOledLineTo(SCREEN_HEIGHT - gameOverHeight, SCREEN_HEIGHT);
  OrbitOledUpdate();
  gameOverHeight++;

}

void gameOverInit() {
  gameOverHeight = 0;
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
    vcView = Game;
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
  fpks = 40000;
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
  drawTrack(onscreen, offset);
  offset++;
}
void trackLoop() {
  trackLoopWithPushFunc(trackPushRandTile);
}
