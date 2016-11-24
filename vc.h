/*******************************************************************************************
 * Program: vc.h
 * Author: Corbin Mcelhinney, Kalvin Thye
 * Description: Header file for view controller
 * Last Modified: November 24, 2016
 *******************************************************************************************/

#ifndef VC_H
#define VC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <math.h>
#include <OrbitOled.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>
#include <stdbool.h>

#include "track.h"
#include "car.h"
#include "defs.h"

typedef struct {
  int leftBtn;
  int rightBtn;
  int bottomSwitch;
  int topSwitch;
  int accel;
  int tivaBtn;
} HardwareState;

void vcInit();
void vcLoop(HardwareState state);

#ifdef __cplusplus
}
#endif

#endif
