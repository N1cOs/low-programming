#include "stdio.h"
#include "math.h"

int is_prime(unsigned long n){
  if(n < 2){
    return 0;
  }
  else if(n == 2 || n == 3){
    return 1;
  }
  else if(n % 2 == 0 || n % 3 == 0){
    return 0;
  }
  long limit = ceil(sqrt(n)) + 1;
  long i = 1;
  while((6 * i - 1) < limit){
    if(n % (6 * i + 1) == 0 || n % (6 * i - 1) == 0){
      return 0;
    }
    i++;
  }
  return 1;
}

int main(){
  //1125899839733759
  //18014398241046527 - 1.8s; 0.5s;
  // unsigned long number = 18014398241046527;
  unsigned long number;
  scanf("%lu", &number);
  if(is_prime(number)){
    puts("Your number is prime");
  }
  else{
    puts("Your number isn't prime");
  }
  return 0;
}
