/*******************************************************************************
 * Program: car.c
 * Author: Corbin Mcelhinney, Kalvin Thye
 * Description: Contains methods involving the car
 * Last Modified: November 24, 2016
 ******************************************************************************/

#include "car.h"

#define CAR_WIDTH 8
#define CAR_HEIGHT 17
#define CAR_WIDTH_BUFFER (TILE_WIDTH - CAR_WIDTH)/2;
#define CAR_HEIGHT_BUFFER (TILE_HEIGHT - CAR_HEIGHT)/2;
#define CAR_CRASH_BUFFER 2
#define CAR_FRONT_DETECTION 3
#define CAR_BACK_DETECTION 5

//Number of ticks before the next animation of the car crash occurs
#define CAR_CRASH_TICK 10

char car[] = {
  0b01111100,
  0b11111110,
  0b11000110,
  0b10000010,
  0b10000010,
  0b10000010,
  0b01111100,
  0b00111000,
  0b00111000,
  0b00111000,
  0b10111010,
  0b10111010,
  0b11000110,
  0b10000010,
  0b11111110,
  0b01111100,
  0b00111000
};

char carCrash0[] = {
  0b01111100,
  0b11111110,
  0b11000110,
  0b10000010,
  0b10000010,
  0b10000010,
  0b01111100,
  0b00111000,
  0b00111000,
  0b00111000,
  0b10111010,
  0b10111010,
  0b11000110,
  0b10000010,
  0b11111110
};

char carCrash1[] = {
  0b01111100,
  0b11111110,
  0b11000110,
  0b10000010,
  0b10000010,
  0b01111100,
  0b00111000,
  0b01111110,
  0b11011010,
  0b10000010,
  0b11000110,
  0b11111110,
  0b01111100
};

//draws the car given the lane it is on
void drawCar(int lane) {
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);

  int x = SCREEN_HEIGHT_BORDER + CAR_HEIGHT_BUFFER;
  int y = lane * TILE_WIDTH + SCREEN_WIDTH_BORDER + CAR_WIDTH_BUFFER;

  OrbitOledMoveTo(x, y);
  OrbitOledPutBmp(CAR_HEIGHT, CAR_WIDTH, car);
}

//draws car crash animation given the ticks since the crash
void drawCarCrash(int lane, int tick) {
  if (tick <= CAR_CRASH_TICK * 2) {
    OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
    OrbitOledSetDrawMode(modOledSet);

    int x = SCREEN_HEIGHT_BORDER + CAR_HEIGHT_BUFFER;
    int y = lane*TILE_WIDTH + SCREEN_WIDTH_BORDER + CAR_WIDTH_BUFFER;



    //draw first animation, mid crash
    if (tick <= CAR_CRASH_TICK) {
      eraseCar(lane);
      OrbitOledMoveTo(x + CAR_CRASH_BUFFER, y);
      OrbitOledPutBmp(CAR_HEIGHT - CAR_CRASH_BUFFER, CAR_WIDTH, carCrash0);
    } else {
    //draw crashed car
    eraseCar(lane);
      OrbitOledMoveTo(x + CAR_CRASH_BUFFER * 2, y);
      OrbitOledPutBmp(CAR_HEIGHT - CAR_CRASH_BUFFER * 2, CAR_WIDTH, carCrash1);
    }

  }
}


//updates lane based on user input
int updateCarLaneButton(int newLeftState, int newRightState, int lane) {
  static int oldLeftState = LOW;
  static int oldRightState = LOW;

  if (newLeftState == HIGH && oldLeftState == LOW) {
    oldLeftState = newLeftState;
    oldRightState = newRightState;
    // Move left
    lane--;
    if (lane < 0) lane = 0;
    return lane;
  }
  if (newRightState == HIGH && oldRightState == LOW) {
    oldLeftState = newLeftState;
    oldRightState = newRightState;
    // Move right
    lane++;
    if (lane > 2) lane = 2;
    return lane;
  }

  // No lane moving necessary
  oldLeftState = newLeftState;
  oldRightState = newRightState;

  return lane;
}

int updateCarLaneAccel(int accel) {
  if (accel > 2.5) {
      return 2;
  } else if (accel < -2.5) {
    return 0;
  }
  return 1;
}

//checks if car has crashed
int checkCollision (int lane) {
  int x = TILE_HEIGHT - CAR_FRONT_DETECTION + SCREEN_HEIGHT_BORDER;
  int y = lane*TILE_WIDTH + SCREEN_WIDTH_BORDER;
  for (int x0 = x; x0 > 0; x0 -= x + CAR_BACK_DETECTION + SCREEN_HEIGHT_BORDER) {
    for (int y0 = y; y0 < y + TILE_WIDTH; y0++) {
      OrbitOledMoveTo(x0, y0);
      if (OrbitOledGetPixel() == 1) {
        return 1;
      }
    }
  }

  return 0;
}


/******************************************************************************/
// Helper Functions:

//erases the area where the car is
void eraseCar(int lane) {
  int x = SCREEN_HEIGHT_BORDER;
  int y = lane * TILE_WIDTH + SCREEN_WIDTH_BORDER;
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnBlank));
  OrbitOledSetDrawMode(modOledSet);

  OrbitOledMoveTo(x, y);
  OrbitOledFillRect(x + 17, y + 8);
}

/*******************************************************************************************/
