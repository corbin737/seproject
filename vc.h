#ifndef VC_H
#define VC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <OrbitOled.h>
#include <OrbitBoosterPackDefs.h>

#include "types.h"
#include "track.h"
#include "ui.h"
#include "car.h"

void vcInit();
void vcLoop(state hardwareState);

#ifdef __cplusplus
}
#endif

#endif
