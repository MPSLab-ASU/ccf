#include <stdio.h>

/*int factorial(int n){
  int ret = 1;
  int dummy = 0;

  #pragma CGRA
  for(int i=n; i > 1; i--){
      ret *= i;
      dummy++;
  }
 
  return ret;
  }*/

int main(int argc, char* argv[]){
  int n = atoi(argv[1]);
  int fact=1;// = factorial(n);
  int dummy=0;

  #pragma CGRA
  for(int i=n; i > 1; i--){
    if(fact<10000)
      fact *= i;
    //dummy++;
  }
  
  printf("%d! = %d\n", n, fact);
}
