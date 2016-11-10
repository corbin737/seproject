#ifndef CAR_H
#define CAR_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
  int x;
  int y;
  int radius;
} Car; 

void drawCar(Car *c, int lane);
int updateCarLane(int leftBtnState, int rightBtnState);

#ifdef __cplusplus
}
#endif
  
#endif
