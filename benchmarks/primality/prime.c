#include <stdio.h>

int prime_check(int num){
  int ret=1;

  #pragma CGRA
  for(int i=2; i< (num/2 +1); i++){
    if(num%i == 0) ret = 0;
  }

  return ret;
}

int main(int argc, char* argv[]){
  int num = atoi(argv[1]);
  printf("Checking %d for prime\n", num);
  
  int prime = prime_check(num);
  if(prime) printf("%d is prime\n", num);
  else printf("%d is not prime\n", num);

  return 0;
}
