#include <stdio.h>
#include <stdlib.h>

/*int sum(int count){                                                                                                                              
  int ret=0;
  //int vec[10] = {1,2,3,4,5,6,7,8,9,10};
  
  #pragma CGRA 
  for(int i=0; i<count; i++)
    ret += i;

  return ret;
  }*/

int power(int base, int pow){
  if(pow == 0) return 1;
  printf("Base = %d - Pow = %d\n", base, pow);
  int ret = base;
  #pragma CGRA
  for(int i = 1; i < pow - 1; i++)
    ret *= base;
  return ret;
}

int main(int argc, char *argv[])
{
  int pow = atoi(argv[1]);
  int ret = power(2, pow);
  //int ret = sum(pow);
  
  printf("***** %d^%d = %d *****\n", 2, pow, ret);

}
