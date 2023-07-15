#include <stdio.h>


int main(int argc, char *argv[])
{

  int iterations = atoi(argv[1]);
	int i=0, j=0;
	int a=4, b=100, c=29, d=0, e=0;
  printf("iterations: %d\n", iterations);

	#pragma CGRA	
	for(i=0;i<iterations; i++) {

		a+=20;
	
	  if(a<100)
     	if(i>3)	
		   d+=c+10;
	}

	printf("a: %d\n c:%d\n d:%d\n", a,c,d);



   	return 0;
}
