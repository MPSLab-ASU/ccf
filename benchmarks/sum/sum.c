#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  int ret=0;
  int count = atoi(argv[1]);

#pragma CGRA
  for(int i=0; i<=count; i++)
    if(i<count)
      ret += i;

  printf("Sum from 0 to %d = %d\n", count, ret);

  return 1;
}
