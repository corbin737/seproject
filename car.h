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
#include "defs.h"
#define middleX TILE_WIDTH/2
#define middleY TILE_HEIGHT/2
#define CAR_RADIUS 3

void getCarPos(int *x, int *y, int lane);

void drawCar(int lane);
int updateCarLane(int leftBtnState, int rightBtnState);
int checkCollision(int lane);


#ifdef __cplusplus
}
#endif

#endif
