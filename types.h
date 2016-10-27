#ifndef TYPES_H
#define TYPES_H

typedef struct {
  bool left;
  bool middle;
  bool right;
} tile;

struct node {
  struct node *prev;
  struct node *next;
  tile value;
};
typedef struct node node;

#endif
