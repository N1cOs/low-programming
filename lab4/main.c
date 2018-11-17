#include "stdio.h"
#include "malloc.h"
#include "limits.h"
#include "list.h"

void foreach(linked_list *list, void (*function)(int)){
  int length = list_length(list);
  int i;
  for(i = 0; i < length; i++){
    function(list_get(list, i));
  }
}

linked_list *map(linked_list *source_list, int (*function)(int)){
  int length = list_length(source_list);
  linked_list* new_list = malloc(sizeof(linked_list));
  new_list->value = function(list_get(source_list, 0));
  int i;
  for(i = 1; i < length; i++){
    list_add_back(&new_list, function(list_get(source_list, i)));
  }
  return new_list;
}

int foldl(int accumulator, int (*function)(int, int), linked_list *list){
  int length = list_length(list);
  int i;
  for(i = 0; i < length; i++){
    accumulator = function(list_get(list, i), accumulator);
  }
  return accumulator;
}

linked_list *map_mut(linked_list *list, int (*function)(int)){
  int length = list_length(list);
  int i;
  for(i = 0; i < length; i++){
    linked_list *node = list_node_at(list, i);
    node->value = function(node->value);
  }
  return list;
}

linked_list *iterate(int initial_value, int length, int(*function)(int)){
  linked_list* new_list = malloc(sizeof(linked_list));
  new_list->value = initial_value;
  new_list->next_element = NULL;
  int i;
  int accum = initial_value;
  for(i = 1; i < length; i++){
    accum = function(accum);
    list_add_back(&new_list, accum);
  }
  return new_list;
}

int save(linked_list *list, char* filename){
  FILE *file = fopen(filename, "w");
  if(file == NULL){
    return 0;
  }
  int length = list_length(list);
  int i;
  for(i = 0; i < length; i++){
    fprintf(file, "%d ", list_get(list, i));
  }
  fclose(file);
  return 1;
}

int load(linked_list *list, char* filename){
  FILE *file = fopen(filename, "r");
  if(file == NULL){
    return 0;
  }
  int length = list_length(list);
  int i;
  for(i = 0; i < length; i++){
    int value;
    fscanf(file, "%d ", &value);
    list_node_at(list, i)->value = value;
  }
  fclose(file);
  return 1;
}

int serialize(linked_list *list, char* filename){
  FILE *file = fopen(filename, "wb");
  if(file == NULL){
    return 0;
  }
  int length = list_length(list);
  int i;
  for(i = 0; i < length; i++){
    int element = list_get(list, i);
    fwrite(&element, sizeof(int), 1, file);
  }
  fclose(file);
  return 1;
}

int deserialize(linked_list *list, char *filename){
  FILE *file = fopen(filename, "rb");
  if(file == NULL){
    return 0;
  }
  int length = list_length(list);
  int i;
  for(i = 0; i < length; i++){
    int *address = &list_node_at(list, i)->value;
    fread(address, sizeof(int), 1, file);
  }
  fclose(file);
  return 1;
}

void print_spaces(int element){
  printf("%d ", element);
}

void print_new_line(int element){
  printf("%d\n", element);
}

int square(int element){
  return element * element;
}

int cube(int element){
  return element * element * element;
}

int sum(int a, int b){
  return a + b;
}

int min(int a, int b){
  return a < b ? a : b;
}

int max(int a, int b){
  return a > b ? a : b;
}

int abs(int a){
  return a > 0 ? a : -a;
}

int pow_two(int value){
  return value * 2;
}

int do_nothing(int value){
  return value;
}

int main(){
  linked_list *list = NULL;
  int value;
  while(scanf(" %d ", &value) != EOF){
    if(!list){
      list = (linked_list*) malloc(sizeof(linked_list));
      list->value = value;
    }
    else{
      list_add_front(&list, value);
    }
  }
  puts("\n---------------------------Output---------------------------");
  foreach(list, print_spaces);
  printf("\n");
  foreach(list, print_new_line);
  linked_list *square_list = map(list, square);
  linked_list *cube_list = map(list, cube);
  printf("%s", "Square elements: ");
  foreach(square_list, print_spaces);
  printf("\n");
  printf("%s", "Cube elements: ");
  foreach(cube_list, print_spaces);
  printf("\n");
  // list_free(square_list);
  // list_free(cube_list);
  printf("Sum of elements - %d\n", foldl(0, sum, list));
  printf("Max value - %d\n", foldl(INT_MIN, max, list));
  printf("Min value - %d\n", foldl(INT_MAX, min, list));
  printf("%s", "Absolute values: ");
  linked_list *copy_list = map(list, do_nothing);
  foreach(map_mut(copy_list, abs), print_spaces);
  printf("\nPowers of two: ");
  foreach(iterate(1, 10, pow_two), print_spaces);
  printf("\n");
  save(list, "list.txt");
  load(list, "list.txt");
  printf("%s\n", "After loading from file");
  foreach(list, print_spaces);
  serialize(list, "list.bin");
  deserialize(list, "list.bin");
  printf("\n%s\n", "After deserializing");
  foreach(list, print_spaces);
  printf("\n");
  return 0;
}
