#include "stdio.h"

struct test{
  int a;
};

int main(){
  struct test test;
  test.a = 5;
  struct test *ptr_test = &test;
  printf("%lu\n", ptr_test+a);
  printf("%lu\n", &test.a);
}
