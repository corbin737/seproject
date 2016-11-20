#include "vc.h"

static enum View {
  Start,
  Game,
  GameOver
} vcView;
typedef enum View View;

const int drawDelay = 50;
const int defStartTickDelay = 100;
const int defGameTickDelay = 18;
const int defGameOverTickDelay = 10;
int tickDelay;

int lastDrawTime, lastTickTime;
tile *head;
tile *onscreen;
int offset;
int lane;
int gameOverHeight;

const int levelDisplayMillis = 2000;
const int levelGraceTicks = 150;
const int levelTickIncrease = 200;
int levelTicks = 1200;

int level;
int levelDisplayStart;
int levelTimer;

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
      if (state.leftBtn == HIGH || state.rightBtn == HIGH) {
        setView(Game);
      }
      break;
    case Game:
      gameTick(state);
      break;
    case GameOver:
      if (gameOverHeight > SCREEN_HEIGHT) {
        setView(Start);
        delay(2000);
      }
      gameOverHeight++;
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
      if (state.bottomSwitch == HIGH) break; // For pausing
      OrbitOledClear();
      if ((currentTime - levelDisplayStart) < levelDisplayMillis) {
        drawLevel();
      } else {
        drawTrack(onscreen, offset);
        if (checkCollision(lane) == 1) {
          setView(GameOver);
        }
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
  tickDelay = defStartTickDelay;
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
  tickDelay = defGameTickDelay;
  level = 1;
  levelDisplayStart = millis();
}

void gameOverInit() {
  gameOverHeight = 0;
  tickDelay = defGameOverTickDelay;
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

void gameTick(HardwareState state) {
  if (state.bottomSwitch == HIGH) return;
  lane = updateCarLane(state.leftBtn, state.rightBtn);
  if ((millis() - levelDisplayStart) < levelDisplayMillis) {
    if (levelTimer != 0) {
      levelTimer = 0;
    }
  } else {
    if (state.topSwitch == HIGH) {
      tickDelay = floor(0.5 * defGameTickDelay * pow(0.7, (level-1)));
    } else {
      tickDelay = floor(defGameTickDelay * pow(0.7, (level-1)));
    }
    if (levelTimer < levelTicks) {
      trackTick(trackPushRandTile);
    } else if (levelTimer < (levelTicks + levelGraceTicks)) {
      trackTick(trackPushBlankTile);
    }
    levelTimer++;
    if (levelTimer == (levelTicks + levelGraceTicks)) {
      levelDisplayStart = millis();
      level++;
      levelTicks += levelTickIncrease;
    }
  }
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

void drawLevel() {
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);
  OrbitOledMoveTo((SCREEN_HEIGHT / 2), (SCREEN_WIDTH / 2));
  OrbitOledDrawString("Level");
  OrbitOledDrawChar(level + '0');
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
