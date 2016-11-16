/*******************************************************************************************
 * Program: car_game.ino
 * Author: Corbin Mcelhinney, Kalvin Thye
 * Description: Main program
 *******************************************************************************************/

#include "track.h"
#include "car.h"
#include "vc.h"
#include "defs.h"
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>

const int leftBtnPin = PE_0;
const int rightBtnPin = PD_2;
const int bottomSwitchPin = PA_6;
const int topSwitchPin = PA_7;

state hardwareState;

void setup() {
  OrbitOledInit();
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);

  pinMode(leftBtnPin, INPUT);
  pinMode(rightBtnPin, INPUT);
  pinMode(bottomSwitchPin, INPUT);
  pinMode(topSwitchPin, INPUT);

  vcInit();
}

void loop() {
  hardwareState.leftBtn = digitalRead(leftBtnPin);
  hardwareState.rightBtn = digitalRead(rightBtnPin);
  hardwareState.bottomSwitch = digitalRead(bottomSwitchPin);
  hardwareState.topSwitch = digitalRead(topSwitchPin);
  vcLoop(hardwareState);
}
