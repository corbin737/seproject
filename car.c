#include <OrbitBoosterPackDefs.h>
#include "car.h"
#include "ui.h"


void drawCar(int lane) {
  int x = middleY + TILE_HEIGHT_BORDER;
  int y = lane*TILE_WIDTH + middleX + TILE_WIDTH_BORDER;

  drawFillCircle(x, y, CAR_RADIUS);
  
  OrbitOledUpdate();
}

void getCar(int *x, int *y, int lane) {
  *x = middleY + TILE_HEIGHT_BORDER;
  *y = lane*TILE_WIDTH + middleX + TILE_WIDTH_BORDER;

  
}

int updateCarLane(int newLeftState, int newRightState) {
  static int oldLeftState = LOW;
  static int oldRightState = LOW;
  static int lane = 1;

  if (newLeftState == HIGH && oldLeftState == LOW) {
    oldLeftState = newLeftState;
    oldRightState = newRightState;
    // Move left
    lane--;
    if (lane < 0) lane = 0;
    return lane;
  }
  if (newRightState == HIGH && oldRightState == LOW) {
    oldLeftState = newLeftState;
    oldRightState = newRightState;
    // Move right
    lane++;
    if (lane > 2) lane = 2;
    return lane;
  }

  // No lane moving necessary
  oldLeftState = newLeftState;
  oldRightState = newRightState;
  return lane;
}


int findSwitch (int r) {
  int x0 = 0;
  int y0 = -1*r;
  int d = 2*(x0+1)*(x0+1)+ y0*y0 + (y0+1)*(y0+1) - 2*r*r;

  while (y0 == -1*r) {
    if (0 <= x0 && x0 < r &&  -1*r <=  y0 && y0 <= 0) { 
        if (d <= 0) {
          x0++;
          y0=y0;
        } else if (d > 0) {
          x0++;
          y0++;
        }
          d = 2*(x0+1)*(x0+1)+ y0*y0 + (y0+1)*(y0+1) - 2*r*r;
          if (y0 != -1*r) return x0-1;
    }
  }
}


void drawCircle(int x, int y, int r) {
  
  int turn = findSwitch(r);
  int x0 = -1*turn;
  int y0 = -1*r;
  int d = 2*(x0+1)*(x0+1)+ y0*y0 + (y0+1)*(y0+1) - 2*r*r;
  
  for (int i = 0 ; i <r*r*r; i++) {
    OrbitOledMoveTo(x0+ x, y0+ y);
    OrbitOledDrawPixel();
 
    
    if (-1*turn-1 < x0 && x0 < r &&  -1*r <=  y0 && y0 < -1*turn) { ///first quadrant
      d = 2*(x0+1)*(x0+1)+ y0*y0 + (y0+1)*(y0+1) - 2*r*r;
      if (d <= 0) {
        x0++;
        y0=y0;
      } else if (d > 0) {
        x0++;
        y0++;
      }
        
      } else if (0 <= x0 && x0 <= r  && (-1*turn-1 <= y0) && y0 < r) {
        d = x0*x0+ (x0-1)*(x0-1)+ 2*(y0+1)*(y0+1) - 2*r*r;
        if (d <= 0) {
        x0=x0;
        y0++;
      } else if (d > 0) {
        x0--;
        y0++;
      }
    } else if (-1*r < x0 && x0 <= turn+1 && turn <= y0 && y0 <= r) {
      d = 2*(x0-1)*(x0-1)+ y0*y0 + (y0-1)*(y0-1) - 2*r*r;
      if (d <= 0) {
        x0--;
        y0=y0;
      } else if (d > 0) {
        x0--;
        y0--;
      }
    } else if (-1*r <- x0 && x0 < -1* turn && -1*r < y0 && y0 <= turn+1){
      d = x0*x0+ (x0+1)*(x0+1)+ 2*(y0-1)*(y0-1) - 2*r*r;
        if (d <= 0) {
        x0=x0;
        y0--;
      } else if (d > 0) {
        x0++;
        y0--;
      }
    }


  }
}


int drawFillCircle(int x, int y, int r) {
  int turn = findSwitch(r);
  int x0 = -1*turn;
  int y0 = -1*r;
  int d = 2*(x0+1)*(x0+1)+ y0*y0 + (y0+1)*(y0+1) - 2*r*r;
  
  for (int i = 0 ; i <r*r*r; i++) {
    OrbitOledMoveTo(x0+ x, y0+ y);
    OrbitOledLineTo(x,y);
    
    if (-1*turn-1 < x0 && x0 < r &&  -1*r <=  y0 && y0 < -1*turn) { ///first quadrant
      d = 2*(x0+1)*(x0+1)+ y0*y0 + (y0+1)*(y0+1) - 2*r*r;
      if (d <= 0) {
        x0++;
        y0=y0;
      } else if (d > 0) {
        x0++;
        y0++;
      }
        
      } else if (0 <= x0 && x0 <= r  && (-1*turn-1 <= y0) && y0 < r) {
        d = x0*x0+ (x0-1)*(x0-1)+ 2*(y0+1)*(y0+1) - 2*r*r;
        if (d <= 0) {
        x0=x0;
        y0++;
      } else if (d > 0) {
        x0--;
        y0++;
      }
    } else if (-1*r < x0 && x0 <= turn+1 && turn <= y0 && y0 <= r) {
      d = 2*(x0-1)*(x0-1)+ y0*y0 + (y0-1)*(y0-1) - 2*r*r;
      if (d <= 0) {
        x0--;
        y0=y0;
      } else if (d > 0) {
        x0--;
        y0--;
      }
    } else if (-1*r <- x0 && x0 < -1* turn && -1*r < y0 && y0 <= turn+1){
      d = x0*x0+ (x0+1)*(x0+1)+ 2*(y0-1)*(y0-1) - 2*r*r;
        if (d <= 0) {
        x0=x0;
        y0--;
      } else if (d > 0) {
        x0++;
        y0--;
      }
    }


  }
  OrbitOledMoveTo(x, y);
  OrbitOledDrawPixel();
  drawCircle(x, y, r-1);
 
}


