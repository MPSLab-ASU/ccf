/*
* definitions.h
*
*  Created on: Jul 16, 2012
*      Author: mahdi
*
*  Last Edited on: Jan 14, 2017
*  Author: Shail Dave
*/

#include <vector>
#include <set>
#include <queue>
#include <stack>
#include "stdio.h"
#include "string.h"

#include "ctime"
#include <typeinfo>
#include "stdlib.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>
#include <list>

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_


using namespace std;

#define MAXINT 10000000
#define INF -100

//Largest Constant Value Allowed As Immediate Field
#define MAX_CONSTANT 4095

#define load_add_latency 1
#define load_data_latency 1
#define store_add_latency 1
#define store_data_latency 1

#define MAX_MEM_RESOURCES cgra_info.PER_ROW_MEM_AVAILABLE*CGRA_X_Dim

#define ROTATING_REG_AS_POW_OF_2 1

typedef enum
{
  Simple_Connection = 0, Diagonal_Connection, Hop_Connection
} ConnectionType;

typedef enum
{
  Simple = 0, LoadAddress, LoadData, StoreAddress, StoreData
} Operations;

enum DataDepType
{
  TrueDep,
  LoadDep,   //an arc between load address assertion node and load bus read node should impose 1 cycle delay between nodes
  StoreDep,  //an arc between store address assertion node and store bus assertion node, should impose 0 cycle
  MemoryDep, //not implemented yet
  PredDep
};

enum Datatype
{
  character,      //0
  int32,          //1
  int16,          //2
  float32,        //3
  float64,        //4
  float16         //5
};

enum Instruction_Operation
{
  add_op,           // 0
  sub_op,           // 1
  mult_op,          // 2
  div_op,           // 3
  shiftl_op,        // 4
  shiftr_op,        // 5
  andop_op,         // 6
  orop_op,          // 7
  xorop_op,         // 8
  cmpSGT_op,        // 9
  cmpEQ_op,         // 10
  cmpNEQ_op,        // 11
  cmpSLT_op,        // 12
  cmpSLEQ_op,       // 13
  cmpSGEQ_op,       // 14
  cmpUGT_op,        // 15
  cmpULT_op,        // 16
  cmpULEQ_op,       // 17
  cmpUGEQ_op,       // 18
  ld_add_op,        // 19
  ld_data_op,       // 20
  st_add_op,        // 21
  st_data_op,       // 22
  ld_add_cond_op,   // 23
  ld_data_cond_op,  // 24
  loopctrl_op,      // 25
  cond_select_op,   // 26
  route_op,         // 27
  llvm_route_op,    // 28
  select_op,        // 29
  constant_op,      // 30
  rem_op,           // 31
  sext_op,          // 32
  shiftr_logical_op,// 33
  rest_op,          // 34
  reduced_op        // 35
};

struct CGRA_Architecture
{
  int X_Dim;
  int Y_Dim;
  unsigned int R_Size;
  ConnectionType interconnect;
  int MAX_NODE_INDEGREE;
  int MAX_NODE_OUTDEGREE;
  int PER_ROW_MEM_AVAILABLE;
  int MAX_PE_INDEGREE; 
  int MAX_PE_OUTDEGREE;
  int MAX_DATA_PER_TIMESLOT;  
};


extern CGRA_Architecture cgra_info;
extern int totalNonrecVars;
extern int RegisterFileFailures;
extern ofstream debugfile;

struct Mapping_Policy
{
  float MAX_MAPPING_ATTEMPTS;
  int CLIQUE_ATTEMPTS;
  int MAX_LATENCY;
  int ENABLE_REGISTERS;
  int MODULO_SCHEDULING_ATTEMPTS;
  int MAPPING_ATTEMPTS_PER_II;
  int MAPPING_MODE;
  int MAX_II;
  int MAX_SCHED_TRY;
  float LAMBDA;
  int IMS;  
};

extern Mapping_Policy MAPPING_POLICY;

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/*
* debugs related to dot prints
*/
#define DOTPRINT_DEBUG 0

#define DEBUG_DOTPRINT(DFG,NAME,ID)                     \
do                                                  \
{                                                   \
  if (DOTPRINT_DEBUG)                             \
  {                                               \
    DFG->Dot_Print_DFG(NAME,ID);                \
  }                                               \
}while(0)

/*
* debugs related to debug messages
*/

#define REST_DEBUG 0
#define DEBUG_1 0
#define DEBUG_2 0
#define DEBUG_3 0
#define DEBUG_4 0

#define SCHEDULE_SMART_DEBUG 0


#define PREPROCESS_DEBUG 0
#define ASAP_DEBUG 0
#define ALAP_DEBUG 0
#define FEASIBLE_ASAP_DEBUG 0
#define FEASIBLE_ALAP_DEBUG 0
#define MODULO_SCHEDULE_DEBUG 0
#define SCHEDULE_DEBUG 0
#define RESCHEDULE_DEBUG 0
#define MAPPING_DEBUG 0
#define CLIEUQ_DEBUG 0
#define COMPATIBILITY_DEBUG 0
#define CONFLICT_DEBUG 0

#define DEBUG_PREPROCESS(msg...)                        \
do                                                  \
{                                                   \
  DEBUG(PREPROCESS,msg);                     \
} while(0)


#define DEBUG_ASAP_SCHEDULING(msg...)                   \
do                                                  \
{                                                   \
  DEBUG(ASAP_DEBUG,msg);                     \
} while(0)


#define DEBUG_ALAP_SCHEDULING(msg...)                   \
do                                                  \
{                                                   \
  DEBUG(ALAP_DEBUG,msg);                     \
} while(0)

#define DEBUG_FEASIBLE_ASAP_SCHEDULING(msg...)                   \
do                                                  \
{                                                   \
  DEBUG(FEASIBLE_ASAP_DEBUG,msg);                     \
} while(0)


#define DEBUG_FEASIBLE_ALAP_SCHEDULING(msg...)                   \
do                                                  \
{                                                   \
  DEBUG(FEASIBLE_ALAP_DEBUG,msg);                     \
} while(0)

#define DEBUG_MODULO_SCHEDULING(msg...)                        \
do                                                  \
{                                                   \
  DEBUG(MODULO_SCHEDULE_DEBUG,msg);                 \
} while(0)

#define DEBUG_CONFLICT(msg...)                        \
do                                                  \
{                                                   \
  DEBUG(CONFLICT_DEBUG,msg);                 \
} while(0)

#define DEBUG_SCHEDULING(msg...)                        \
do                                                  \
{                                                   \
  DEBUG(SCHEDULE_DEBUG,msg);                 \
} while(0)


#define DEBUG_RESCHEDULE(msg...)                        \
do                                                  \
{                                                   \
  DEBUG(RESCHEDULE_DEBUG,msg);               \
} while(0)


#define DEBUG_MAPPING(msg...)                           \
do                                                  \
{                                                   \
  DEBUG(MAPPING_DEBUG,msg);                  \
} while(0)


#define DEBUG_CLIQUE(msg...)                            \
do                                                  \
{                                                   \
  DEBUG(CLIQUE_DEBUG,msg);                   \
} while(0)


#define DEBUG_COMPATIBILITY(msg...)                     \
do                                                  \
{                                                   \
  DEBUG(COMPATIBILITY_DEBUG,msg);            \
} while(0)


#define DEBUG(condition,msg...)                    \
do                                                  \
{                                                   \
  if ((condition))                               \
  {                                               \
    /*_DEBUG("[%s:%d] check failed '%s'",__FILE__, __LINE__,#condition); \ */ \
    _DEBUG(msg);                                \
  }                                               \
} while(0)


#define _DEBUG(msg...)                                         \
do                                                         \
{                                                          \
  fflush(stdout);                                        \
  fprintf(stderr,msg);                                   \
  fprintf(stderr,"\n");                                  \
  fflush(stdout);                                        \
} while(0)

/*
* debugs related to fatal errors
*/


#define FATAL(condition,msg...)                        \
do                                                  \
{                                                   \
  if ((condition))                               \
  {                                               \
    _DEBUG("[%s:%d] FATAL '%s'",__FILE__, __LINE__,#condition); \
    _FATAL(msg);                                \
  }                                               \
} while(0)



#define _FATAL(msg...)                                \
do                                                         \
{                                                          \
  fflush(stdout);                                        \
  fprintf(stderr,msg);                                   \
  fprintf(stderr,"\n");                                  \
  exit(1);                                               \
} while(0)


#endif /* DEFINITIONS_H_ */
