#ifndef VC_H
#define VC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <OrbitOled.h>
#include <OrbitBoosterPackDefs.h>

#include "track.h"
#include "car.h"
#include "defs.h"

typedef struct {
  int leftBtn;
  int rightBtn;
  int bottomSwitch;
  int topSwitch;
} state;

void vcInit();
void vcLoop(state hardwareState);

#ifdef __cplusplus
}
#endif

#endif
