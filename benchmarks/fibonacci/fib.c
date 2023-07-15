#include <stdio.h>

/*int fibonacci(int count){
  int ret=1;
  int prev=1;
  #pragma CGRA
  for(int i=1; i<count-1; i++){
    if(i < count){
      int temp = ret+prev;
      prev = ret;
      ret = temp;
    }
  }

  return ret;
  }*/

int main(int argc, char* argv[]){
  int count = atoi(argv[1]);
  printf("Calculating fibonacci for %d numbers\n", count);

  //int fib = fibonacci(count);

  int ret=1;
  int prev=1;
  #pragma CGRA
  for(int i=1; i<count-1; i++){
    //if(i < count){
      int temp = ret+prev;
      prev = ret;
      ret = temp;
      //}
  }
  
  
  printf("Result: %d\n", ret);
}
