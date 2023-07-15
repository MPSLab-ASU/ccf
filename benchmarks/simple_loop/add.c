#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

  int iterations = atoi(argv[1]); 

  int a=30, b=3, result = 0; 
  int c = 0;
  
  //char d = 'W', e;
  int i; 

  #pragma CGRA
  for(i=0;i<iterations; i++)
  {
    if(i<5)
      c += a+b;
    result++;
  }

  //printf("Address: %x\n", &result);
  printf("iterations:%d\t, c:%d, result:%d\n", iterations, c , result);


}
