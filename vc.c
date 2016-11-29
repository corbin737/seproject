/*******************************************************************************
 * Program: vc.c
 * Author: Corbin McElhanney, Kalvin Thye
 * Description: View Controller that communicates our model of the game with the display
 * Last Modified: November 24, 2016
 ******************************************************************************/

#include "vc.h"

static enum View {
  Start,
  Game,
  GameOver
} vcView;
typedef enum View View;

// Drawing and tick controls
int lastDrawTime, lastTickTime;
int tickDelay;
const int drawDelay = 50;
const int defStartTickDelay = 100;
const int defGameTickDelay = 18;
const int defGameOverTickDelay = 10;
const int levelDisplayMillis = 2000;
const int levelGraceTicks = 150;

int levelTicks = 1200;

// Game state variables
tile *head;
tile *onScreen;
int offset;
int lane;
bool paused;
int score;
int level;

// Draw state variables
int levelDisplayStart;
int levelTimer;
int gameOverHeight;

// Game progression
const int levelTickIncrease = 200;
const double levelDelayFactor = 0.8;
const double speedMultiplier = 0.5;

// Helper functions
void startInit();
void gameInit();
void gameOverInit();
void trackTick(tile *(*pushTile)(tile *));
void gameTick(int leftBtn, int rightBtn, int accel, int controlSwitch, int speedSwitch);
void pauseTick(int btn);
void drawGameOver();
void drawStart();
void drawLevel();
void setView(View view);

// Initializes the view controller
void vcInit() {
  setView(Start);
}

// Executes drawing and logic (tick) operations at defined intervals
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
      pauseTick(state.tivaBtn);
      if (paused) break;
      gameTick(state.leftBtn, state.rightBtn, state.accel, state.bottomSwitch, state.topSwitch);
      break;
    case GameOver:
      if (gameOverHeight > SCREEN_HEIGHT + 2) {
        setView(Start);
        delay(3000);
      }
      gameOverHeight++;
      break;
    }
  }

  if (currentTime - lastDrawTime > drawDelay) {
    lastDrawTime = currentTime;
    switch(vcView) {
    case Start:
      OrbitOledClearBuffer();
      drawTrack(onScreen, offset);
      drawStart();
      break;
    case Game:
      if (paused) return;
      OrbitOledClearBuffer();
      if ((currentTime - levelDisplayStart) < levelDisplayMillis) {
        drawLevel();
      } else {
        drawTrack(onScreen, offset);
        if (checkCollision(lane) == 1) {
          setView(GameOver);
        }
      }
      drawCar(lane);
      break;
    case GameOver:
      drawGameOver();
    }
    OrbitOledUpdate();
  }
}
/******************************************************************************/
// Helper Functions:

// Initializes global variables for Start view
void startInit() {
  if (head != NULL) {
    head = trackDelete(head);
  }
  head = trackCreate();
  for (int i = 0; i < NUMBER_OF_TILES + 1; i++) {
    head = trackPushFullTile(head);
  }
  onScreen = head->next;
  offset = 0;
  tickDelay = defStartTickDelay;
}

// Initializes global variables for Game view
void gameInit() {
  if (head != NULL) {
    head = trackDelete(head);
  }
  head = trackCreate();
  for (int i = 0; i < NUMBER_OF_TILES + 1; i++) {
    head = trackPushBlankTile(head);
  }
  onScreen = head->next;
  offset = 0;
  tickDelay = defGameTickDelay;
  level = 1;
  score = 0;
  lane = 1;
  levelDisplayStart = millis();
  paused = false;
}

// Initializes global variables for GameOver view
void gameOverInit() {
  gameOverHeight = 0;
  tickDelay = defGameOverTickDelay;
}

// Progresses track model by one pixel, using pushTile to
// add more track if necessary
void trackTick(tile *(*pushTile)(tile *)) {
  // Update track model
  offset++;
  if (offset >= TILE_HEIGHT) {
    // Bottom tile has moved off screen entirely
    offset %= TILE_HEIGHT;
    head = trackPopTile(head);
    head = pushTile(head);
  }
  if (offset == 1) {
    // Next tile has moved in on top of screen
    onScreen = onScreen->prev;
  }
}

// Updates global game variables based on user input and time elapsed
void gameTick(int leftBtn, int rightBtn, int accel, int controlSwitch, int speedSwitch) {
  if (controlSwitch == HIGH) {
    lane = updateCarLaneAccel(accel);
  } else {
    lane = updateCarLaneButton(leftBtn, rightBtn, lane);
  }

  if ((millis() - levelDisplayStart) < levelDisplayMillis) {
    levelTimer = 0;
  } else {
    score++;
    if (speedSwitch == HIGH) {
      tickDelay = floor(speedMultiplier * defGameTickDelay * pow(levelDelayFactor, (level-1)));
    } else {
      tickDelay = floor(defGameTickDelay * pow(levelDelayFactor, (level-1)));
    }
    if (levelTimer < levelTicks) {
      // Normal tick
      trackTick(trackPushRandTile);
    } else if (levelTimer < (levelTicks + levelGraceTicks)) {
      // Add blank tiles until screen is all blank
      trackTick(trackPushBlankTile);
    } else if (levelTimer >= (levelTicks + levelGraceTicks)) {
      // Trigger next level
      levelDisplayStart = millis();
      level++;
      levelTicks += levelTickIncrease;
    }
    levelTimer++;
  }
}

// Updates pause global variable based on user input
void pauseTick(int btn) {
  static int oldState = HIGH;
  if (btn == LOW && oldState == HIGH) {
    paused = !paused;
  }
  oldState = btn;
}

// Draws a frame of the GameOver view based on global variables
void drawGameOver() {
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);
  drawCarCrash(lane, gameOverHeight);
  OrbitOledMoveTo(SCREEN_HEIGHT, 0);
  OrbitOledFillRect(SCREEN_HEIGHT - gameOverHeight - 1, SCREEN_WIDTH);

  if (gameOverHeight > SCREEN_HEIGHT - 5) {
    // Draw game over text
    char score_str[5];
    itoa(score, score_str, 10);
    OrbitOledSetDrawMode(modOledXor);
    OrbitOledMoveTo(SCREEN_HEIGHT / 2 - 35, SCREEN_WIDTH / 2 - 10);
    OrbitOledDrawString("Game Over");
    OrbitOledMoveTo(SCREEN_HEIGHT / 2 - 35, SCREEN_WIDTH / 2);
    OrbitOledDrawString(score_str);
  }
}

// Clears space for and draws start text
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

// Draws text declaring a new level
void drawLevel() {
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);
  OrbitOledMoveTo((SCREEN_HEIGHT / 2), (SCREEN_WIDTH / 2));
  OrbitOledDrawString("Level ");
  OrbitOledDrawChar(level + '0');
}

// Updates global view variable and initializes the new view
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
