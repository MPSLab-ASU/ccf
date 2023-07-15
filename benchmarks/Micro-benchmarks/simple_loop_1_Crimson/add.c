#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

  int iterations = atoi(argv[1]); 

  float a=30, b=10, c = 0; 

  //char d = 'W', e;
  int i; 

  #pragma CGRA
  for(i=0;i<iterations; i++)
  {
      if(i<3)
        c += a+b;
  }

  
  printf("iterations:%d\t, c:%f\n", iterations, c );


}
