#ifndef CAR_H
#define CAR_H

#ifdef __cplusplus
extern "C"
{
#endif

#define middleX TILE_WIDTH/2
#define middleY TILE_HEIGHT/2
#define CAR_RADIUS 3

void drawCar(int lane);
int updateCarLane(int leftBtnState, int rightBtnState);

#ifdef __cplusplus
}
#endif

#endif
