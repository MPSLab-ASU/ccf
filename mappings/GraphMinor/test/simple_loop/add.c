#include <stdio.h>
#define N 10000

int main() 
{
//int a = 3, b = 5, c= 0;
int i=0;
 
int a[N], b[N], c[N];


for(i=0; i<N; i++)
{
  a[i] = i; 
}

for(i=0; i<N; i++)
{
  b[i] = i+2;
}


for(i=0; i<10; i++)
{
  printf("a[%d]=%d\t b[%d]=%d\n",i,a[i],i,b[i]);
  
} 

#pragma CGRA
for (i=0; i<N; i++)
{
   c[i] = a[i] + b[i]; 
}



for(i=0; i<10; i++)
{
  printf("c[%d]=%d\n",i,c[i]);
}

}
