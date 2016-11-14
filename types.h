#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#define NUMBER_OF_LANES 3

struct tile {
  struct tile *prev;
  struct tile *next;
  bool value[NUMBER_OF_LANES];
};
typedef struct tile tile;

typedef struct {
  int leftBtn;
  int rightBtn;
  int bottomSwitch;
  int topSwitch;
} state;

#endif
