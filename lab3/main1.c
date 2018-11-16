#include "stdio.h"
#include "malloc.h"

typedef struct node{
  struct node *next;
  int value;
} linked_list;
