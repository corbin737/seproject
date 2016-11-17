/*******************************************************************************************
 * Program: car.h
 * Author: Corbin Mcelhinney, Kalvin Thye
 * Description: Header file that contains methods involving the car
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

void getCarPos(int *x, int *y, int lane);
void drawCar(int lane);
int updateCarLane(int leftBtnState, int rightBtnState);
int checkCollision(int lane);

#ifdef __cplusplus
}
#endif

#endif
