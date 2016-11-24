/*******************************************************************************************
 * Program: car.h
 * Author: Corbin McElhanney, Kalvin Thye
 * Description: Header file that contains methods involving the car
 * Last Modified: November 24, 2016
 *******************************************************************************************/

#ifndef CAR_H
#define CAR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <OrbitOled.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>
#include "defs.h"

void drawCar(int lane);
void drawCarCrash(int lane, int counter);
int updateCarLaneButton(int newLeftState, int newRightState, int lane);
int updateCarLaneAccel(int accel);
int checkCollision(int lane);

#ifdef __cplusplus
}
#endif

#endif
