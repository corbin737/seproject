/*******************************************************************************************
 * Program: car.c
 * Author: Corbin McElhanney, Kalvin Thye
 * Description: Contains methods involving the car
 * Last Modified: November 24, 2016
 *******************************************************************************************/

#include "car.h"

#define CAR_WIDTH 8
#define CAR_HEIGHT 17

const int carWidthBuffer = (TILE_WIDTH - CAR_WIDTH) / 2;
const int carHeightBuffer = (TILE_HEIGHT - CAR_HEIGHT) / 2;
const int carCrashDecrement = 2;
const int carFrontDetection = 3;
const int carBackDetection = 5;

// Number of ticks before the next animation of the car crash occurs
const int carCrashTicks = 10;

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

// Helper function
void eraseCar(int lane);

// Draws the car given the lane it is on
void drawCar(int lane) {
  int x = TILE_HEIGHT_BORDER + carHeightBuffer;
  int y = lane * TILE_WIDTH + TILE_WIDTH_BORDER + carWidthBuffer;

  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);
  OrbitOledMoveTo(x, y);
  OrbitOledPutBmp(CAR_HEIGHT, CAR_WIDTH, car);
}

// Draws car crash animation given the ticks since the crash
void drawCarCrash(int lane, int tick) {
  if (tick <= carCrashTicks * 2) {
    int x = TILE_HEIGHT_BORDER + carHeightBuffer;
    int y = lane*TILE_WIDTH + TILE_WIDTH_BORDER + carWidthBuffer;

    OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
    OrbitOledSetDrawMode(modOledSet);

    if (tick <= carCrashTicks) {
      // Draw first animation, mid crash
      eraseCar(lane);
      OrbitOledMoveTo(x + carCrashDecrement, y);
      OrbitOledPutBmp(CAR_HEIGHT - carCrashDecrement, CAR_WIDTH, carCrash0);
    } else {
      // Draw crashed car
      eraseCar(lane);
      OrbitOledMoveTo(x + carCrashDecrement * 2, y);
      OrbitOledPutBmp(CAR_HEIGHT - carCrashDecrement * 2, CAR_WIDTH, carCrash1);
    }
  }
}

// Updates lane based on button input
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

// Updates car lane based on accelerometer input
int updateCarLaneAccel(int accel) {
  if (accel > 2.5) {
      return 2;
  } else if (accel < -2.5) {
    return 0;
  }
  return 1;
}

// Checks if car has crashed
int checkCollision (int lane) {
  int x = TILE_HEIGHT - carFrontDetection + TILE_HEIGHT_BORDER;
  int y = lane*TILE_WIDTH + TILE_WIDTH_BORDER;
  for (int x0 = x; x0 > 0; x0 -= x + carBackDetection + TILE_HEIGHT_BORDER) {
    for (int y0 = y; y0 < y + TILE_WIDTH; y0++) {
      OrbitOledMoveTo(x0, y0);
      if (OrbitOledGetPixel() == 1) {
        return 1;
      }
    }
  }

  return 0;
}


/*******************************************************************************************/
// Helper Functions:

// Erases the area where the car is
void eraseCar(int lane) {
  int x = TILE_HEIGHT_BORDER;
  int y = lane * TILE_WIDTH + TILE_WIDTH_BORDER;
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnBlank));
  OrbitOledSetDrawMode(modOledSet);

  OrbitOledMoveTo(x, y);
  OrbitOledFillRect(x + 17, y + 8);
}
