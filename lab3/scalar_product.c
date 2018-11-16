#include "stdio.h"

int scalar_product(int vector1[], int vector2[], size_t size){
  int result = 0;
  size_t i;
  for(i = 0; i < size; i++){
    result += vector1[i] * vector2[i];
  }
  return result;
}



int main(){
  int vector1[] = {1, 2, 3};
  int vector2[] = {1, 2, 3};
  int result = scalar_product(vector1, vector2, sizeof(vector1) / sizeof(int));
  printf("%d\n", result);
  return 0;
}
