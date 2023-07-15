/*
 * definitions.h
 *
 *  Created on: Jul 16, 2012
 *      Author: mahdi
 */

//#include "coretypes.h"
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

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_


using namespace std;

#define MAXINT 10000000

//supported operations and associated latencies
/* plus */
#define CGRA_RTX_PLUS 1

/* Operand 0 minus operand 1.  */
#define CGRA_RTX_MINUS 1

/* Minus operand 0.  */
#define CGRA_RTX_NEG 1

/* Multiplication*/
#define CGRA_RTX_MULT 1

/* Multiplication with signed saturation */
#define CGRA_RTX_SS_MULT 1

/* Multiplication with unsigned saturation */
#define CGRA_RTX_US_MULT 1

/* Operand 0 divided by operand 1.  */
#define CGRA_RTX_DIV 1

/* Division with signed saturation */
#define CGRA_RTX_SS_DIV 1

/* Division with unsigned saturation */
#define CGRA_RTX_US_DIV 1

/* Remainder of operand 0 divided by operand 1.  */
#define CGRA_RTX_MOD 1

/* Bitwise operations.  */
#define CGRA_RTX_AND 1

#define CGRA_RTX_IOR 1

#define CGRA_RTX_XOR 1

#define CGRA_RTX_NOT 1

/* shift left */
#define CGRA_RTX_ASHIFT 1
/* rotate left */
#define CGRA_RTX_ROTATE 1
/* arithmetic shift right */
#define CGRA_RTX_ASHIFTRT 1
/* logical shift right */
#define CGRA_RTX_LSHIFTRT 1

/* rotate right */
#define CGRA_RTX_ROTATERT 1

/* Minimum and maximum values of two operands.  We need both signed and
   unsigned forms.  (We cannot use MIN for SMIN because it conflicts
   with a macro of the same name.)   The signed variants should be used
   with floating point.  Further, if both operands are zeros, or if either
   operand is NaN, then it is unspecified which of the two operands is
   returned as the result.  */
#define CGRA_RTX_SMIN 1
#define CGRA_RTX_SMAX 1
#define CGRA_RTX_UMIN 1
#define CGRA_RTX_UMAX 1

/* Addition with signed saturation */
#define CGRA_RTX_SS_PLUS 1 ///* Addition with signed saturation */
/* Addition with unsigned saturation */
#define CGRA_RTX_US_PLUS 1

/* Operand 0 minus operand 1, with signed saturation.  */
#define CGRA_RTX_SS_MINUS 1

/* Negation with signed saturation.  */
#define CGRA_RTX_SS_NEG 1

/* Negation with unsigned saturation.  */
#define CGRA_RTX_US_NEG 1

/* Absolute value with signed saturation.  */
#define CGRA_RTX_SS_ABS 1

/* Shift left with signed saturation.  */
#define CGRA_RTX_SS_ASHIFT 1

/* Shift left with unsigned saturation.  */
#define CGRA_RTX_US_ASHIFT 1

/* Operand 0 minus operand 1, with unsigned saturation.  */
#define CGRA_RTX_US_MINUS 1

#define CGRA_INSN 1
#define CGRA_JUMP 0
#define CGRA_INSN_LIST 0


typedef enum
{
    Simple_Connection = 0, Diagonal_Connection, Hop_Connection
} REGI_ConnectionType;

typedef enum
{
    Simple = 0, LoadAddress, LoadData, StoreAddress, StoreData
} REGI_Operations;

enum DataDepType
{
    TrueDep,
    LoadDep,   //an arc between load address assertion node and load bus read node should impose 1 cycle delay between nodes
    StoreDep,  //an arc between store address assertion node and store bus assertion node, should impose 0 cycle
    MemoryDep, //not implemented yet
    PredDep
};

enum Reduced_Node_Type
{
    two_2_one,
    three_2_one

};
enum Instruction_Operation
{
    add_op,
    sub_op,
    mult_op,
    div_op,
    shiftl_op,
    shiftr_op,
    andop_op,
    orop_op,
    xorop_op,
    cmpSGT_op,
    cmpEQ_op,
    cmpNEQ_op,
    cmpSLT_op,
    cmpSLEQ_op,
    cmpSGEQ_op,
    cmpUGT_op,
    cmpULT_op,
    cmpULEQ_op,
    cmpUGEQ_op,
    ld_add_op,
    ld_data_op,
    st_add_op,
    st_data_op,
    ld_add_cond_op,
    ld_data_cond_op,
    st_add_cond_op,
    st_data_cond_op,
    route_op,
    llvm_route_op,
    select_op,
    constant_op,
    loopctrl_op,
    rest_op,
    reduced_op

};


/*
 *      We hold the CGRA Architecture information here
 */
struct CGRA_Architecture
{
    int X_Dim;
    int Y_Dim;
    int R_Size;
    REGI_ConnectionType interconnect;
    int MAX_NODE_INDEGREE;
    int MAX_NODE_OUTDEGREE;
    int PER_ROW_MEM_AVAILABLE;
};

extern CGRA_Architecture cgra_info;


/*
 *      We hold mapping polocies here 
 *
 */

struct Mapping_Policy
{
    int REGIMap;
    int REGI_MAPPING_ATTEMPTS;
    int CLIQUE_ATTEMPTS;
    int SIMULATED_ANNEALING;
    int MAX_TEMPERATURE;
    int TIME_THRESHOLD;
    int MAX_LATENCY;
    int ENABLE_REGISTERS;
    float MAX_MAPPING_ATTEMPTS;
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

#define DOTPRINT_ASAP_SCHEDULE_DEBUG 1
#define DOTPRINT_ALAP_SCHEDULE_DEBUG 1
#define DOTPRINT_FEASIBLE_ASAP_SCHEDULE_DEBUG 0
#define DOTPRINT_FEASIBLE_ALAP_SCHEDULE_DEBUG 1
#define DOTPRINT_CURRENT_SCHEDULE_DEBUG 1

#define DEBUG_DOTPRINT(DFG,NAME,ID)                     \
    do                                                  \
{                                                   \
    if (DOTPRINT_DEBUG)                             \
    {                                               \
        DFG->Dot_Print_DFG(NAME,ID);                \
    }                                               \
}while(0)

#define DEBUG_DOTPRINT_ASAP_SCHEDULE(NAME,ID)               \
    do                                                      \
{                                                       \
    if (DOTPRINT_ASAP_SCHEDULE_DEBUG)                   \
    {                                                   \
        for (int i = 0; i < (int) _node_Set.size(); i++)    \
        {                                                   \
            _node_Set[i]->get_Sched_Info()->set_Current(_node_Set[i]->get_Sched_Info()->get_ASAP(), 100);                                                         \
        }                                                   \
        Dot_Print_DFG_With_Schedule(NAME,ID);               \
        Reset_Current_Schedule();                           \
    }                                                       \
}while(0)

#define DEBUG_DOTPRINT_ALAP_SCHEDULE(NAME,ID)               \
    do                                                      \
{                                                       \
    if (DOTPRINT_ALAP_SCHEDULE_DEBUG)                   \
    {                                                   \
        for (int i = 0; i < (int) _node_Set.size(); i++)    \
        {                                                   \
            _node_Set[i]->get_Sched_Info()->set_Current(_node_Set[i]->get_Sched_Info()->get_ALAP(), 100);                                                         \
        }                                                   \
        Dot_Print_DFG_With_Schedule(NAME,ID);               \
        Reset_Current_Schedule();                           \
    }                                                       \
}while(0)

#define DEBUG_DOTPRINT_FEASIBLE_ASAP_SCHEDULE(NAME,ID)          \
    do                                                           \
{                                                            \
    if (DOTPRINT_FEASIBLE_ASAP_SCHEDULE_DEBUG)               \
    {                                                        \
        for (int i = 0; i < (int) _node_Set.size(); i++)    \
        {                                                   \
            _node_Set[i]->get_Sched_Info()->set_Current(_node_Set[i]->get_Sched_Info()->get_Feasible_ASAP(), 100);                                                         \
        }                                                   \
        Dot_Print_DFG_With_Schedule(NAME,ID);                  \
        Reset_Current_Schedule();                           \
    }                                                       \
}while(0)

#define DEBUG_DOTPRINT_FEASIBLE_ALAP_SCHEDULE(NAME,ID)                  \
    do                                                      \
{                                                       \
    if (DOTPRINT_FEASIBLE_ALAP_SCHEDULE_DEBUG)                   \
    {                                                   \
        for (int i = 0; i < (int) _node_Set.size(); i++)    \
        {                                                   \
            _node_Set[i]->get_Sched_Info()->set_Current(_node_Set[i]->get_Sched_Info()->get_Feasible_ALAP(), 100);                                                         \
        }                                                   \
        Dot_Print_DFG_With_Schedule(NAME,ID);               \
        Reset_Current_Schedule();                           \
    }                                                       \
}while(0)

#define DEBUG_DOTPRINT_CURRENT_SCHEDULE(NAME,ID)               \
    do                                                      \
{                                                       \
    if (DOTPRINT_CURRENT_SCHEDULE_DEBUG)                \
    {                                                   \
        Dot_Print_DFG_With_Schedule(NAME,ID);            \
    }                                                   \
}while(0)

/*
 * debugs related to debug messages 
 */

#define REST_DEBUG 0
#define DEBUG_1 0
#define DEBUG_2 0
#define DEBUG_3 0
#define DEBUG_4 0

#define SCHEDULE_FEASIBLE_ASAP_DEBUG 0
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


#define DEBUG_PREPROCESS(msg...)                        \
    do                                                  \
{                                                   \
    REGI_DEBUG(PREPROCESS,msg);                     \
} while(0)


#define DEBUG_ASAP_SCHEDULING(msg...)                   \
    do                                                  \
{                                                   \
    REGI_DEBUG(ASAP_DEBUG,msg);                     \
} while(0)


#define DEBUG_ALAP_SCHEDULING(msg...)                   \
    do                                                  \
{                                                   \
    REGI_DEBUG(ALAP_DEBUG,msg);                     \
} while(0)

#define DEBUG_FEASIBLE_ASAP_SCHEDULING(msg...)                   \
    do                                                  \
{                                                   \
    REGI_DEBUG(FEASIBLE_ASAP_DEBUG,msg);                     \
} while(0)


#define DEBUG_FEASIBLE_ALAP_SCHEDULING(msg...)                   \
    do                                                  \
{                                                   \
    REGI_DEBUG(FEASIBLE_ALAP_DEBUG,msg);                     \
} while(0)

#define DEBUG_MODULO_SCHEDULING(msg...)                        \
    do                                                  \
{                                                   \
    REGI_DEBUG(MODULO_SCHEDULE_DEBUG,msg);                 \
} while(0)


#define DEBUG_SCHEDULING(msg...)                        \
    do                                                  \
{                                                   \
    REGI_DEBUG(SCHEDULE_DEBUG,msg);                 \
} while(0)


#define DEBUG_RESCHEDULE(msg...)                        \
    do                                                  \
{                                                   \
    REGI_DEBUG(RESCHEDULE_DEBUG,msg);               \
} while(0)


#define DEBUG_MAPPING(msg...)                           \
    do                                                  \
{                                                   \
    REGI_DEBUG(MAPPING_DEBUG,msg);                  \
} while(0)


#define DEBUG_CLIQUE(msg...)                            \
    do                                                  \
{                                                   \
    REGI_DEBUG(CLIQUE_DEBUG,msg);                   \
} while(0)


#define DEBUG_COMPATIBILITY(msg...)                     \
    do                                                  \
{                                                   \
    REGI_DEBUG(COMPATIBILITY_DEBUG,msg);            \
} while(0)


#define REGI_DEBUG(condition,msg...)                    \
    do                                                  \
{                                                   \
    if ((condition))                               \
    {                                               \
        _DEBUG("[%s:%d] check failed '%s'",__FILE__, __LINE__,#condition); \
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

