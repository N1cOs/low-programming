#ifndef _LIST_H_
#define _LIST_H_

#include "mem.h"
#include "limits.h"

typedef struct linked_list{
  struct linked_list *next_element;
  int value;
} linked_list;

linked_list *list_create(int value);

void list_add_front(linked_list **list, int value);

void list_add_back(linked_list **list, int value);

void list_free(linked_list *list);

int list_length(linked_list *list);

linked_list *list_node_at(linked_list *list, int index);

int list_get(linked_list *list, int index);

unsigned long list_sum(linked_list *list);
#endif
