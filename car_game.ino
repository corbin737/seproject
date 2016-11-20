/*******************************************************************************************
 * Program: car_game.ino
 * Author: Corbin Mcelhinney, Kalvin Thye
 * Description: Main program
 *******************************************************************************************/

#include <OrbitOledGrph.h>
#include "vc.h"

const int leftBtnPin = PE_0;
const int rightBtnPin = PD_2;
const int bottomSwitchPin = PA_6;
const int topSwitchPin = PA_7;

HardwareState state;

void setup() {
  pinMode(leftBtnPin, INPUT);
  pinMode(rightBtnPin, INPUT);
  pinMode(bottomSwitchPin, INPUT);
  pinMode(topSwitchPin, INPUT);

  OrbitOledInit();
  vcInit();

}

void loop() {
  state.leftBtn = digitalRead(leftBtnPin);
  state.rightBtn = digitalRead(rightBtnPin);
  state.bottomSwitch = digitalRead(bottomSwitchPin);
  state.topSwitch = digitalRead(topSwitchPin);
  vcLoop(state);
}
