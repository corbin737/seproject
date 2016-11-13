/*******************************************************************************************
 * Program: car_game.ino
 * Author: Corbin Mcelhinney, Kalvin Thye
 * Description: Main program
 *******************************************************************************************/

#include "types.h"
#include "track.h"
#include "ui.h"
#include "car.h"
#include "menu.h"
#include "collision.h"
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>

const int leftBtnPin = PE_0;
const int rightBtnPin = PD_2;

state hardwareState;

void setup() {
  OrbitOledInit();
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);

  pinMode(leftBtnPin, INPUT);
  pinMode(rightBtnPin, INPUT);

  menuInit();
}

void loop() {
  hardwareState.leftBtn = digitalRead(leftBtnPin);
  hardwareState.rightBtn = digitalRead(rightBtnPin);
  menuLoop(hardwareState);
}
