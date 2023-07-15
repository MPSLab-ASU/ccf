/*
 * This example shows the double max and min search kernel
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define VECTOR_MAX 100

/*void find_max(int count){
  int vector[VECTOR_MAX] = {
    136, -227, 66, -10, 34, 27, -50, 210, 190, -30,
    -12, -121, -33, 41, 64, -190, 141, -20, 198, -44,
    112, 6, -16, 109, -134, -202, 83, 132, 91, 72,
    242, 15, 100, -139, -155, -167, -231, 62, 79, 223,
    242, 120, -81, 128, -107, -160, -107, -99, 244, -26,
    -225, 238, 224, 126, -157, -5, -71, -29, 91, 215,
    -149, 159, 157, 37, 65, -123, 121, -99, 183, -122,
    -166, -223, 1, 243, -132, 89, -125, 195, -251, 146,
    -129, -194, 15, -127, 134, -136, 228, -161, -108, 229,
    106, -57, -240, -253, 17, -136, -21, 79, 167, 191 };
  

  int a = vector[0];  // Max value
  int b =  INT32_MIN;  // 2nd max value
  int c = 0;  // Max index
  int d = -1;  // 2nd max index

    #pragma CGRA
    for(unsigned i=1; i<count; i++) {
      int temp = vector[i];
      if (temp > a) {  // Largest value found => b<-a, a<-vector[i]
        b = a;
        a = temp;
        d = c;
        c = i;
      } else if (temp > b) { // 2nd largest found
        b = temp;
        d = i;
      }
    }
    printf("Maximum 1 value = %d\n", a);
    printf("Maximum 1 index = %d\n", c);
    printf("Maximum 2 value = %d\n", b);
    printf("Maximum 2 index = %d\n", d);

    return;
    }*/

int main(int32_t argc, char* argv[]){
  if(argc < 2){
    printf("Pass count to program\n");
    return 0;
  }

  int vector[VECTOR_MAX] = {
    136, -227, 66, -10, 34, 27, -50, 210, 190, -30,
    -12, -121, -33, 41, 64, -190, 141, -20, 198, -44,
    112, 6, -16, 109, -134, -202, 83, 132, 91, 72,
    242, 15, 100, -139, -155, -167, -231, 62, 79, 223,
    242, 120, -81, 128, -107, -160, -107, -99, 244, -26,
    -225, 238, 224, 126, -157, -5, -71, -29, 91, 215,
    -149, 159, 157, 37, 65, -123, 121, -99, 183, -122,
    -166, -223, 1, 243, -132, 89, -125, 195, -251, 146,
    -129, -194, 15, -127, 134, -136, 228, -161, -108, 229,
    106, -57, -240, -253, 17, -136, -21, 79, 167, 191 };

  
  int count = atoi(argv[1]);
  count = (count >= 100)? 100:count;  // cap at 100 max

  printf("Find max of %d numbers\n", count);
  //find_max(count);

    int a = vector[0];  // Max value
    int b =  INT32_MIN;  // 2nd max value
    int c = 0;  // Max index
    int d = -1;  // 2nd max index

    #pragma CGRA
    for(unsigned i=1; i<count; i++) {
      int temp = vector[i];
      if (temp > a) {  // Largest value found => b<-a, a<-vector[i]
        b = a;
        a = temp;
        d = c;
        c = i;
      } else if (temp > b) { // 2nd largest found
        b = temp;
        d = i;
      }
    }
    printf("Maximum 1 value = %d\n", a);
    printf("Maximum 1 index = %d\n", c);
    printf("Maximum 2 value = %d\n", b);
    printf("Maximum 2 index = %d\n", d);


  return 0;
}
