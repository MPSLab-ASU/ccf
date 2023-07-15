// Author: Shail Dave

// Last edited: April 5 2022
// Author: Vinh TA
// Update: add support for prolog extension

#include "cgra.h"
#include <string.h>
#include <limits.h>
#include <stdint.h>
#ifdef __cplusplus
#include <iostream>
#endif

//#define DEBUG

//#define usleep(time)  myusleep(time)

//pthread_mutex_t mutex;


__attribute__((noinline))
extern volatile unsigned get_clock(){
  volatile unsigned ret;
  asm("sub r13, r13, #4");
  asm("str r12, [r13]");
  asm("mov r12,%[value]" : :[value]"r" (SYS_CLOCK):);
  asm("mov r0, r12");
  asm("ldr r12, [r13]");
  asm("add r13, r13, #4");
  asm("str r0, [r13]");
  return ret;
}

unsigned prolog_index2 = 0; // for larger and multiple loops, prolog_size array got disturbed, this global replaces prolog_index 

void readInstructionSize(){
#ifdef DEBUG
  printf("Reading instruction size for %d loops.\n", totalLoops);
#endif
  int loopID;
  for(loopID=1; loopID<=totalLoops; loopID++){
    FILE* epi,*pro,*kern;
    char loopno[25];
    char directoryPath[20] = "./CGRAExec/L";
    
    sprintf(loopno,"%d",loopID);
    strcat(directoryPath,loopno);

    char epifile[40] = "";
    char profile[40] = "";
    char kernfile[40] = "";

    strcat(epifile,directoryPath);
    strcat(epifile,"/epilog_ins.bin");
    
    strcat(profile,directoryPath);
    strcat(profile,"/prolog_ins.bin");
    
    strcat(kernfile,directoryPath);
    strcat(kernfile,"/kernel_ins.bin");

    epi=fopen(epifile,"rb");
    pro=fopen(profile,"rb");
    kern=fopen(kernfile,"rb");
    
    if(epi == NULL){
      printf("FATAL: Cannot open file %s\n", epifile);
      exit(1);
    } else if(pro == NULL){
      printf("FATAL: Cannot open file %s\n", profile);
      exit(1);
    } else if(kern == NULL){
      printf("FATAL: Cannot open file %s\n", kernfile);
      exit(1);
    }
    
    int episize,prosize,kernelsize;
    int prolog_extend_size, prolog_version_size;
    fread(&episize,sizeof(int),1,epi);
    fread(&prosize,sizeof(int),1,pro);
    fread(&prolog_extend_size, sizeof(int), 1, pro);
    fread(&prolog_version_size, sizeof(int), 1, pro);
    fread(&kernelsize,sizeof(int),1,kern);
    int final_prolog_size = prosize + prolog_extend_size;

    prolog_size[loopID-1] = final_prolog_size;
    kernel_size[loopID-1] = kernelsize;
    epilog_size[loopID-1] = episize;

    fclose(epi);
    fclose(pro);
    fclose(kern);
  }
}

int initializeParameters(unsigned int loopID)
{
#ifdef DEBUG
  printf("cgra.c: Initialize Parameters - loopID: %d\n", loopID);
#endif
  unsigned int i=0;

  *(initCGRA + initCGRA_size*(loopID-1)) = 0x77e00000;
  *(initCGRA + initCGRA_size*(loopID-1) + 1) = 0x77e00000; //1st instruction, 0xe7e00000 is the opcode for no operation
  *(initCGRA + initCGRA_size*(loopID-1) + 2) = 0;	//II
  *(initCGRA + initCGRA_size*(loopID-1) + 3) = 0;	//Epilog length
  *(initCGRA + initCGRA_size*(loopID-1) + 4) = 0;	//Prolog length
  *(initCGRA + initCGRA_size*(loopID-1) + 5) = 0;   //KERNEL COUNTER
  *(initCGRA + initCGRA_size*(loopID-1) + 6) = 0;	//Live-Variable Store Epilog Lenth
  *(initCGRA + initCGRA_size*(loopID-1) + 7) = 0;   //Prolog versions' length

  FILE* epi,*pro,*kern, *count, *config;
  char loopno[25];
  char directoryPath[20] = "./CGRAExec/L";

  sprintf(loopno,"%d",loopID);
  strcat(directoryPath,loopno);

  char epifile[40] = "";
  char profile[40] = "";
  char kernfile[40] = "";
  char liveOutfile[40] = "";
  char configfile[40] = ""; 

  strcat(epifile,directoryPath);
  strcat(epifile,"/epilog_ins.bin");

  strcat(profile,directoryPath);
  strcat(profile,"/prolog_ins.bin");

  strcat(kernfile,directoryPath);
  strcat(kernfile,"/kernel_ins.bin");

  strcat(configfile,directoryPath);
  strcat(configfile,"/CGRA_config.txt");
  //Make some error checking for fopen and fread
  epi=fopen(epifile,"rb");
  pro=fopen(profile,"rb");
  kern=fopen(kernfile,"rb");
  config=fopen(configfile, "r");

  int episize,prosize,kernelsize, livevar_st_size;
  int prolog_extend_size, prolog_version_size;
  fread(&episize,sizeof(int),1,epi);
  fread(&prosize,sizeof(int),1,pro);
  fread(&prolog_extend_size, sizeof(int), 1, pro);
  fread(&prolog_version_size, sizeof(int), 1, pro);
  fread(&kernelsize,sizeof(int),1,kern);

#ifdef DEBUG
  printf("\n**********EPISIZE: %d*********\n",episize);
  printf("\n**********PROSIZE: %d - PRO_EXT_SIZE: %d - PRO_VERSION_SIZE: %d*********\n",prosize, prolog_extend_size, prolog_version_size);
  printf("\n**********KERNSIZE: %d*********\n",kernelsize);
#endif

  int final_prolog_size = prosize + prolog_extend_size;

  int prolog_index = 0, kernel_index = 0, epilog_index = 0;
  for(i = 1; i<loopID; i++){
    prolog_index += prolog_size[i-1];
    kernel_index += kernel_size[i-1];
    epilog_index += epilog_size[i-1];
  }

#ifdef DEBUG
  printf("Prolog_index: %d - kernel_index: %d - epilog_index: %d\n", prolog_index, kernel_index, epilog_index);
#endif
  
  fread(&epilog[epilog_index],sizeof(unsigned long long),episize,epi);
  fread(&prolog[prolog_index2],sizeof(unsigned long long),final_prolog_size,pro);
  fread(&kernel[kernel_index],sizeof(unsigned long long),kernelsize,kern);

  strcat(liveOutfile,directoryPath);
  strcat(liveOutfile,"/livevar_st_ins_count.txt");
  count=fopen(liveOutfile,"r");
  fscanf(count, "%d", &livevar_st_size);

  char line[256];
  int XDim=0, YDim=0;
  int iter=0; 

  while(fgets(line, sizeof(line), config))
  {
    iter++;
    if(iter==1)
      XDim = atoi(line);
    else if(iter==2)
      YDim = atoi(line);
    else
      break;
  }

#ifdef DEBUG
  printf("\n************XDIM and YDim are %d, %d\n", XDim, YDim);
#endif
  int II = kernelsize/(XDim*YDim);
  int epiLength = episize/(XDim*YDim);
  int prolength = prosize/(XDim*YDim);
  *(initCGRA + initCGRA_size*(loopID-1) + 2) = II;
  *(initCGRA + initCGRA_size*(loopID-1) + 3) = epiLength;
  *(initCGRA + initCGRA_size*(loopID-1) + 4) = prolength;
  *(initCGRA + initCGRA_size*(loopID-1) + 6) = livevar_st_size/(XDim*YDim);
  *(initCGRA + initCGRA_size*(loopID-1) + 7) = prolog_extend_size/(XDim*YDim);
  *(initCGRA + initCGRA_size*(loopID-1) + 8) = prolog_version_size/(XDim*YDim);

  fclose(epi);
  fclose(pro);
  fclose(kern);
  fclose(count);
  fclose(config);

  //printf("Printing prolog:\n");
  //for(i=0; i<prosize; i++)
  //printf("%d: %lx - %llx\n", i, &prolog[i], prolog[i]);

  char loopitfile[40] = "";
  strcat(loopitfile,directoryPath);
  strcat(loopitfile,"/kernel_count.txt");

  int kernelCount = 0;
  count=fopen(loopitfile,"r");
  fscanf(count, "%d", &kernelCount);
  fclose(count);

  *(initCGRA + initCGRA_size*(loopID-1) + 5) = kernelCount; 

#ifdef DEBUG
  printf("From FILE: PROLOGPC= %lx, EPILOGPC=%lx,  KernelPC=%lx\n",(unsigned long) (&prolog[prolog_index2]),(unsigned long) (&epilog[epilog_index]),(unsigned long) (&kernel[kernel_index]));
#endif

  prologPtr[loopID-1] = (unsigned long long) (&prolog[prolog_index2]);
  epilogPtr[loopID-1] = (unsigned long long) (&epilog[epilog_index]);
  kernelPtr[loopID-1] = (unsigned long long) (&kernel[kernel_index]);

  prolog_index2 += final_prolog_size;
  return 0;
}

int configureCGRA(unsigned int loopID)
{
#ifdef DEBUG
  printf("configureCGRA loop %d\n", loopID);
#endif
  unsigned int i=0;
  int kernelCount = *(initCGRA + initCGRA_size*(loopID-1) + 5);
  FILE* count;
  char loopno[25];
  char directoryPath[20] = "./CGRAExec/L";
  
  sprintf(loopno,"%d",loopID);
  strcat(directoryPath,loopno);

  // Fix me: kernelCount is only given when loop CGRA invoked whilst configureCGRA is at load time of the application, need to write kernelCount to initCGRA.txt file at execution time for gem5 to read in
  if(kernelCount <= 0)
  {
    int newTC = kernelCount + dynamicTCVal;
    *(initCGRA + initCGRA_size*(loopID-1) + 5) = newTC; 
  }

  //char initCGRAfile[40] = "./CGRAExec/L1";
  //strcat(initCGRAfile,"/initCGRA.txt");
  //count = fopen(initCGRAfile, "wb");
  strcat(directoryPath,"/initCGRA.txt");
  count = fopen(directoryPath, "wb");
  
  for(i=0; i<7; i++)
    fprintf(count, "%d\n", *(initCGRA + initCGRA_size*(loopID-1) + i));
  fprintf(count, "%ld\n", (unsigned long long) epilogPtr[loopID-1]);
  fprintf(count, "%ld\n", (unsigned long long) prologPtr[loopID-1]);
  fprintf(count, "%ld\n", (unsigned long long) kernelPtr[loopID-1]);
  fprintf(count, "%d\n", *(initCGRA + initCGRA_size*(loopID-1) + 7));
  fprintf(count, "%d\n", *(initCGRA + initCGRA_size*(loopID-1) + 8));
  fclose(count);

  /*FILE* execLoop = fopen("./CGRAExec/LoopID.txt", "wb");
  fprintf(execLoop, "L%d", loopID);
  fclose(execLoop);*/
  
  //printf("Exiting cgra.c configureCGRA\n");
  return 0;
}

void checkTotalLoops( )
{
#ifdef DEBUG
  printf("checkTotalLoops\n");
#endif
    char myfile[40] = "./CGRAExec/total_loops.txt";
    FILE* count;
    count = fopen(myfile, "r");
    fscanf(count, "%u", &totalLoops);
    fclose(count);
}

//__attribute__((noinline))
__inline void* runOnCGRA()
{
#ifdef DEBUG
  printf("\n\nrunOnCGRA\n");
#endif
  asm("sub r13, r13, #4");
  asm("str r12, [r13]");
  asm("mov r12,%[value]" : :[value]"r" (CGRA_ACTIVATE):);
  asm("ldr r12, [r13]");
  asm("add r13, r13, #4");
  return NULL;
}

__inline void accelerateOnCGRA(unsigned int loopNo)
{
#ifdef DEBUG
  printf("\n\naccelerateOnCGRA\n");
#endif
    //int result = 0; 
    //int initializeloop = initializeParameters(loopNo);
    //int configure = configureCGRA(loopNo);
    //if(DEBUG) printf("configure = %d, loopNo = %d\n", configure, loopNo);
    /*if(configure == -1)
    {
      	printf("Error in configuring CGRA:\n");
	}*/
#ifdef DEBUG
    printf("Core will execute loop %u on CGRA\n",loopNo);
#endif

    /*const char *code = "\x00\x00\x00\x00\x00\x68";
    int (*func)();
    func = (int (*)()) code;
    (int)(*
    
    //__asm call code;*/
    //asm("mov s30,%[value]" : :[value]"r" (loopNo):);
    asm("sub r13, r13, #4");
    asm("str r11, [r13]");
    asm("mov r11, %[value]" : :[value]"r" (loopNo):);
    runOnCGRA();
    asm("ldr r11, [r13]");
    asm("add r13, r13, #4");
    
    //while(thread_cond_cpu != 1) {
    //	usleep(1);
    //  }
    
}

void deleteCGRA()
{
#ifdef DEBUG
  printf("\ndeleting cgra\n");
#endif
  /*
  thread_exit = 1;
  thread_cond_cgra = 1;

  printf("Main thread calling join w/ CGRA thread\n");
  //pthread_join(pth, NULL); 
  //printf("Exiting cgra.c deleteCGRA\n"); */
}


void createCGRA()
{
#ifdef DEBUG
  printf("createCGRA\n");
#endif
  int result = 0;
  unsigned i=1;
  
  checkTotalLoops();
  initCGRA  = (int *)malloc(sizeof(int)*initCGRA_size*totalLoops);
  prologPtr = (int *)malloc(sizeof(int)*totalLoops);
  kernelPtr = (int *)malloc(sizeof(int)*totalLoops);
  epilogPtr = (int *)malloc(sizeof(int)*totalLoops);
  prolog_size = (int *)malloc(sizeof(int)*totalLoops);
  kernel_size =	(int *)malloc(sizeof(int)*totalLoops);
  epilog_size =	(int *)malloc(sizeof(int)*totalLoops);

  readInstructionSize();
  int total_size[3] = {0,0,0}; // Size of prolog/kernel/epilog for all loops                                                  
  for(i=1; i<=totalLoops; i++){
    total_size[0] += prolog_size[i-1];
    total_size[1] += kernel_size[i-1];
    total_size[2] += epilog_size[i-1];
  }

  mallopt(M_MMAP_THRESHOLD, total_size[0]*sizeof(unsigned long long)*2);  // This tries to avoid memory allocation in stack when requesting over 128kB
  
  prolog=(unsigned long long*)malloc(total_size[0]*sizeof(unsigned long long));
  kernel=(unsigned long long*)malloc(total_size[1]*sizeof(unsigned long long));
  epilog=(unsigned long long*)malloc(total_size[2]*sizeof(unsigned long long));

#ifdef DEBUG
  printf("Total prolog: %d - total kernel: %d - total epilog: %d\nbase prologPC: %lx - base kernelPC: %lx - base epilogPC: %lx\n", total_size[0], total_size[1], total_size[2], prolog, kernel, epilog);
#endif

  for(i = 1; i <= totalLoops; i++)
  {
    initializeParameters(i);
    configureCGRA(i);
  }

  printf("Done loading CGRA @ cycle %d\n", get_clock());
  
  /*int total_size[3] = {0,0,0}; // Size of prolog/kernel/epilog for all loops
  for(i=1; i<=totalLoops; i++){
    total_size[0] += prolog_size[i-1];
    total_size[1] += kernel_size[i-1];
    total_size[2] += epilog_size[i-1];
  }
  
  printf("Total prolog: %d - total kernel: %d - total epilog: %d\n", total_size[0], total_size[1], total_size[2]);

  prolog=(unsigned long long*)malloc(total_size[0]*sizeof(unsigned long long));
  kernel=(unsigned long long*)malloc(total_size[1]*sizeof(unsigned long long));
  epilog=(unsigned long long*)malloc(total_size[2]*sizeof(unsigned long long));*/

    
  //pthread_mutex_init(&mutex, NULL);
  //printf("Main thread calling CGRA thread...\n");
  //asm("mov r8,%[value]" : :[value]"r" (CPU_idle):);

  //printf("ASM CPU_IDLE Instruction completed\n");
  //result = pthread_create(&pth, NULL, (void*) &runOnCGRA, NULL); 
  //printf("\n\nresult = %d\n\n", result);
  //fflush(NULL);
}
