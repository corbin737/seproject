/*******************************************************************************************
 * Program: car.c
 * Author: Corbin Mcelhinney, Kalvin Thye
 * Description: Contains methods involving the car
 *******************************************************************************************/

#include "car.h"

const int middleX = TILE_WIDTH / 2;
const int middleY = TILE_HEIGHT / 2;
const int carRadius = 3;

char car[] = {
  0x00, //Row 1
  0x00, //Row 2
  0x7C, //Row 3
  0xFE, //Row 4
  0xC6, //Row 5
  0x82, //Row 6
  0x82, //Row 7
  0x82, //Row 8
  0x7C, //Row 9
  0x38, //Row 10
  0x38, //Row 11
  0x38, //Row 12
  0xBA, //Row 13
  0xBA, //Row 14
  0xC6, //Row 15
  0x82, //Row 16
  0xFE, //Row 17
  0x7C, //Row 18
  0x38, //Row 19
};



//Access method that returns the coordinates of car
void getCarPos(int *x, int *y, int lane) {
  *x = middleY + TILE_HEIGHT_BORDER;
  *y = lane*TILE_WIDTH + middleX + TILE_WIDTH_BORDER;
}

//draws car on screen given the lane it is on 
void drawCar(int lane) {
  int x = TILE_WIDTH_BORDER + 1;
  int y = lane*TILE_WIDTH + TILE_HEIGHT_BORDER + 1;
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);

  OrbitOledMoveTo(x, y);
  OrbitOledPutBmp(19, 8, car);
}

//draws car on screen given the lane it is on (old)
void drawCarCircle(int lane) {
  int x = middleY + TILE_HEIGHT_BORDER;
  int y = lane*TILE_WIDTH + middleX + TILE_WIDTH_BORDER;
  
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);
  drawFillCircle(x, y, carRadius);
  //OrbitOledUpdate();
}

//updates lane based on user input
int updateCarLaneButton(int newLeftState, int newRightState, int lane) {
  static int oldLeftState = LOW;
  static int oldRightState = LOW;

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

int updateCarLaneAccel(int accel) {
  return (accel * -1) + 1;
}

int checkCollision (int lane) {
  int x = TILE_HEIGHT - 3 + TILE_HEIGHT_BORDER;
  int y = lane*TILE_WIDTH + TILE_WIDTH_BORDER;
  for (int j = x; j > 0; j+=  -TILE_HEIGHT + 11) {
    for (int i = y; i < y+TILE_WIDTH; i++) {
      OrbitOledMoveTo(j, i);
      if (OrbitOledGetPixel() == 1) {
        return 1;
      }
    }
  }

  return 0;
}


/*******************************************************************************************/
//Helper Functions:
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
/*******************************************************************************************/
