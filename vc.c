#include "vc.h"

static enum View {
  Start = 0,
  Game = 1,
  GameOver = 2
} vcView;
typedef enum View View;

const int drawDelay = 50;
const int defaultGameTickDelay = 30;
int tickDelay;

int lastDrawTime, lastTickTime;
tile *head;
tile *onscreen;
int offset;
int lane;
int gameOverHeight;

void setView(View view);

void vcInit() {
  setView(Start);
}

void vcLoop(HardwareState state) {
  int currentTime = millis();
  if (currentTime - lastTickTime > tickDelay) {
    lastTickTime = currentTime;
    switch(vcView) {
    case Start:
      trackTick(trackPushFullTile);
      startTick(state.leftBtn, state.rightBtn);
      break;
    case Game:
      if (state.bottomSwitch == HIGH) break;
      if (state.topSwitch == HIGH) {
        tickDelay = 10;
      } else {
        tickDelay = defaultGameTickDelay;
      }
      trackTick(trackPushRandTile);
      carTick(state.leftBtn, state.rightBtn);
      break;
    case GameOver:
      gameOverTick();
      break;
    }
  }
  if (currentTime - lastDrawTime > drawDelay) {
    lastDrawTime = currentTime;
    switch(vcView) {
    case Start:
      OrbitOledClear();
      drawTrack(onscreen, offset);
      drawStart();
      break;
    case Game:
      if (state.bottomSwitch == HIGH) break;
      OrbitOledClear();
      drawTrack(onscreen, offset);
      if (checkCollision(lane) == 1) {
        setView(GameOver);
      }
      drawCar(lane);
      break;
    case GameOver:
      drawGameOver(gameOverHeight);
    }
    OrbitOledUpdate();
  }
}

void startInit() {
  if (head != NULL) {
    head = trackDelete(head);
  }
  head = trackCreate();
  for (int i = 0; i < 7; i++) {
    head = trackPushFullTile(head);
  }
  onscreen = head->next;
  offset = 0;
  tickDelay = 100;
}

void gameInit() {
  if (head != NULL) {
    head = trackDelete(head);
  }
  head = trackCreate();
  for (int i = 0; i < 7; i++) {
    head = trackPushBlankTile(head);
  }
  onscreen = head->next;
  offset = 0;
  tickDelay = defaultGameTickDelay;
}

void gameOverInit() {
  gameOverHeight = 0;
  tickDelay = 10;
}

void startTick(int leftBtnState, int rightBtnState) {
  if (leftBtnState == HIGH || rightBtnState == HIGH) {
    setView(Game);
  }
}

void trackTick(tile *(*pushTile)(tile *)) {
  // Update track model
  offset++;
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
}

void carTick(int leftBtnState, int rightBtnState) {
  lane = updateCarLane(leftBtnState, rightBtnState);
}

void gameOverTick() {
  if (gameOverHeight > SCREEN_HEIGHT) {
    setView(Start);
    delay(2000);
  }
  gameOverHeight++;
}

void drawGameOver(int height) {
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);
  OrbitOledMoveTo(SCREEN_HEIGHT, 0);
  OrbitOledFillRect(SCREEN_HEIGHT - gameOverHeight, SCREEN_WIDTH);
}

void drawStart() {
  int bannerWidth = 64;

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
}

void setView(View view) {
  vcView = view;
  switch(vcView) {
  case Start:
    startInit();
    break;
  case Game:
    gameInit();
    break;
  case GameOver:
    gameOverInit();
    break;
  }
}
