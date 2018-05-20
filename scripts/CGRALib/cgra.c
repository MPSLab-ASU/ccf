// Author: Shail Dave

#include "cgra.h"
#include <string.h>
#include <limits.h>

int initializeParameters(unsigned int loopID)
{
  unsigned int i=0;

  *(initCGRA + 7*(loopID-1)) = 0x77e00000;
  *(initCGRA + 7*(loopID-1) + 1) = 0x77e00000; //1st instruction, 0xe7e00000 is the opcode for no operation
  *(initCGRA + 7*(loopID-1) + 2) = 0;	//II
  *(initCGRA + 7*(loopID-1) + 3) = 0;	//Epilog length
  *(initCGRA + 7*(loopID-1) + 4) = 0;	//Prolog length
  *(initCGRA + 7*(loopID-1) + 5) = 0;  //KERNEL COUNTER
  *(initCGRA + 7*(loopID-1) + 6) = 0;	//Live-Variable Store Epilog Lenth

  FILE* epi,*pro,*kern, *count;
  char loopno[25];
  char directoryPath[20] = "./CGRAExec/L";

  sprintf(loopno,"%d",loopID);
  strcat(directoryPath,loopno);

  char epifile[40] = "";
  char profile[40] = "";
  char kernfile[40] = "";
  char liveOutfile[40] = "";

  strcat(epifile,directoryPath);
  strcat(epifile,"/epilog_ins.bin");

  strcat(profile,directoryPath);
  strcat(profile,"/prolog_ins.bin");

  strcat(kernfile,directoryPath);
  strcat(kernfile,"/kernel_ins.bin");

  //Make some error checking for fopen and fread
  epi=fopen(epifile,"rb");
  pro=fopen(profile,"rb");
  kern=fopen(kernfile,"rb");

  int episize,prosize,kernelsize, livevar_st_size;

  fread(&episize,sizeof(int),1,epi);
  fread(&prosize,sizeof(int),1,pro);
  fread(&kernelsize,sizeof(int),1,kern);

  printf("\n**********EPISIZE %d*********\n",episize);
  printf("\n**********PROSIZE %d*********\n",prosize);
  printf("\n**********KERNSIZE %d*********\n",kernelsize);
  printf("\n******SIZE OF UNISGNED INT %d*****\n",sizeof(unsigned int));

  epilog=(unsigned int*)malloc(episize*sizeof(unsigned int));
  prolog=(unsigned int*)malloc(prosize*sizeof(unsigned int));
  kernel=(unsigned int*)malloc(kernelsize*sizeof(unsigned int));

  fread(epilog,sizeof(int),episize,epi);
  fread(prolog,sizeof(int),prosize,pro);
  fread(kernel,sizeof(int),kernelsize,kern);

  strcat(liveOutfile,directoryPath);
  strcat(liveOutfile,"/livevar_st_ins_count.txt");
  count=fopen(liveOutfile,"r");
  fscanf(count, "%d", &livevar_st_size);

  int II = kernelsize/(16);
  int epiLength = episize/16;
  int prolength = prosize/16;
  *(initCGRA + 7*(loopID-1) + 2) = II;
  *(initCGRA + 7*(loopID-1) + 3) = epiLength;
  *(initCGRA + 7*(loopID-1) + 4) = prolength;
  *(initCGRA + 7*(loopID-1) + 6) = livevar_st_size/16;

  fclose(epi);
  fclose(pro);
  fclose(kern);
  fclose(count);

  char loopitfile[40] = "";
  strcat(loopitfile,directoryPath);
  strcat(loopitfile,"/kernel_count.txt");

  int kernelCount = 0;
  count=fopen(loopitfile,"r");
  fscanf(count, "%d", &kernelCount);
  fclose(count);
  printf("Loop Count: %d\n",kernelCount);

  *(initCGRA + 7*(loopID-1) + 5) = kernelCount; 

  printf("From FILE: PROLOGPC= %x, EPILOGPC=%x,  KernelPC=%x\n",(unsigned int)prolog,(unsigned int)epilog,(unsigned int)kernel);

  prologPtr[loopID-1] = (long) prolog;
  epilogPtr[loopID-1] = (long) epilog;
  kernelPtr[loopID-1] = (long) kernel;
	
  return 0;
}

int configureCGRA(unsigned int loopID)
{
  unsigned int i=0;
  int kernelCount = *(initCGRA + 7*(loopID-1) + 5);
  FILE* count;
  char loopno[25];
  char directoryPath[20] = "./CGRAExec/L";
  
  sprintf(loopno,"%d",loopID);
  strcat(directoryPath,loopno);

  if(kernelCount <= 0)
  {
    int newTC = kernelCount + dynamicTCVal;
    *(initCGRA + 7*(loopID-1) + 5) = newTC; 
  }

  char initCGRAfile[40] = "./CGRAExec/L1";
  strcat(initCGRAfile,"/initCGRA.txt");
  count = fopen(initCGRAfile, "wb");
  for(i=0; i<7; i++)
    fprintf(count, "%d\n", *(initCGRA + 7*(loopID-1) + i));
  fprintf(count, "%ld\n", (long) epilogPtr[loopID-1]);
  fprintf(count, "%ld\n", (long) prologPtr[loopID-1]);
  fprintf(count, "%ld\n", (long) kernelPtr[loopID-1]);
  fclose(count);
	
  return 0;
}

void checkTotalLoops( )
{
    char myfile[40] = "./CGRAExec/total_loops.txt";
    FILE* count;
    count = fopen(myfile, "r");
    fscanf(count, "%u", &totalLoops);
    fclose(count);

}

void* runOnCGRA(void* arg)
{

    while(1) {
        while(thread_cond_cgra != 1) usleep(1);
	
        if(thread_exit == 1) { 
 	    asm("mov r11,%[value]" : :[value]"r" (deactivate_CGRA):); 
	    break; 
        }
	
	thread_cond_cgra = 0;
	asm("mov r11,%[value]" : :[value]"r" (activate_CGRA):); 
	usleep(1);
	thread_cond_cpu = 1;
    }
  return NULL;
}

void accelerateOnCGRA(unsigned int loopNo)
{
    int result = 0; 
   
    int configure = configureCGRA(loopNo);
    if(configure == -1)
    {
      printf("Error in configuring CGRA:\n");
    }
    printf("Core will execute loop %u on CGRA\n",loopNo); 

    thread_cond_cpu = 0;
    thread_cond_cgra = 1;

    while(thread_cond_cpu != 1) usleep(1);
}

void deleteCGRA()
{
  printf("deleting cgra\n");

  thread_exit = 1;
  thread_cond_cgra = 1;

  printf("Main thread calling join w/ CGRA thread\n");
  pthread_join(pth, NULL); 
}


void createCGRA()
{
  int result = 0;
  unsigned i=1;

  checkTotalLoops();
  initCGRA  = (int *)malloc(sizeof(int)*7*totalLoops);
  prologPtr = (int *)malloc(sizeof(int)*totalLoops);
  kernelPtr = (int *)malloc(sizeof(int)*totalLoops);
  epilogPtr = (int *)malloc(sizeof(int)*totalLoops);


  for(i = 1; i <= totalLoops; i++)
      initializeParameters(i);

  printf("Main thread calling CGRA thread...\n");
  result = pthread_create(&pth, NULL, runOnCGRA, NULL);
}
