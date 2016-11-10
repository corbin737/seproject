#ifndef MENU_H
#define MENU_H

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

void menuInit();
void menuLoop(state hardwareState);

#ifdef __cplusplus
}
#endif

#endif
