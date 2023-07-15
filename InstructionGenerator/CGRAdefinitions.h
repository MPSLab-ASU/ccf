/*
 * definitions.h
 *
 *  Created on: May 24, 2011
 *  Author: mahdi
 *
 * Last edited:
 * Author: Shail
 *
 * Last edited: 4 April 2022
 * Autho: Vinh TA
 * Update: Move to 64-bit instruction word with added instruction type (LoopExit)
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define IMEMSIZE 10000000
#define DMEMSIZE 100000
#define REGFILESIZE 4

#define CGRA_XDim 4
#define CGRA_YDim 4

#define CGRA_MEMORY_READ 1
#define CGRA_MEMORY_WRITE 2

#define MAX_CONSTANT 0x3ffffffffUL
#define MAX_BRANCH_OFFSET 1023
#define MAX_BRANCH_CONSTANT 67108863

/*
   Normal Instruction Encoding / Decoding
   31	30	29 28|	27   |	26	25	24|	23	22	21|	20	19|	18	17|	16	15|	14|	13 | 12  | 11 10	9	8	7	6	5	4	3	2	1	0 |
   OpCode     |Predict|	   LMUX   |    RMUX   |   R1  |	  R2  |   RW  |	WE| AB |	DB |			Immediate          	  |        /write comparison result to controller
 */

/*
   P-Type Instruction Encoding / Decoding
   31	30	29 28|	27   |	26	25	24|	23	22	21|	20	19|	18 17 |	16 15	| 14 13 12 | 11 10	9	8	7	6	5	4	3	2	1	0 |
   OpCode     |  1    |	   LMUX   |    RMUX   |   R1  |	  R2  |   RP  |   PMUX 	 |			Immediate						  |
 */

/*
  64-bit instruction word

  Normal Instruction Decode:
  63 62 61 | 60 59 58 57 | 56 | 55 | 54 53 52 | 51 50 49 | 48 47 46 45 | 44 43 42 41 | 40 39 38 37 | 36 | 35 | 34 | 33 ... 0
  DT       |    OpCode   | P  | LE |   LMUX   |   RMUX   |      R1     |      R2     |      RW     | WE | AB | DB | Immediate

  P-Type Instruction Decode:
  63 62 61 | 60 59 58 57 | 56 | 55 | 54 53 52 | 51 50 49 | 48 47 46 45 | 44 43 42 41 | 40 39 38 37 | 36 35 34 | 33 ... 0
  DT       |    OpCode   | 1  | 0  |   LMUX   |   RMUX   |      R1     |      R2     |      RP     |    PMUX  | Immediate

  Loop Exit Instruction Decode:
  63 62 61 | 60 59 58 57 | 56 | 55 | 54 53 52 | 51 50 49 | 48 47 46 45 | 44 43 42 41 | 40 39 38 37 | 36 | 35 ... 26 | 25...
  DT       | OpCode      | 0  | 1  |   LMUX   |   RMUX   |      R1     |      R2     |      RW     | WE |   BrImm   | Imme

  Note: 34-bit immediate is redundant since RF only supports 32-bit data
 */

// the UL must be used for 64-bit ops.
#define WIDTH_DATATYPE          0x7UL 
#define WIDTH_OPCODE		0xfUL
#define WIDTH_PREDICT		0x1UL
#define WIDTH_LE                0x1UL
#define WIDTH_MUX		0x7UL
#define WIDTH_REGISTER	        0xfUL
#define WIDTH_ENABLE		0x1UL
#define WIDTH_IMMEDIATE	        0x3ffffffffUL
#define WIDTH_BRANCH_OFFSET     0x3ffUL
#define WIDTH_LE_IMMEDIATE      0x3ffffffUL

#define SHIFT_DATATYPE          61
#define SHIFT_OPCODE		57
#define SHIFT_PREDICT		56
#define SHIFT_LE                55
#define SHIFT_LMUX		52
#define SHIFT_RMUX		49
#define SHIFT_R1		45
#define SHIFT_R2		41
#define SHIFT_RW		37
#define SHIFT_WE		36
#define SHIFT_ABUS		35
#define SHIFT_DBUS		34
#define SHIFT_IMMEDIATE	        00
#define SHIFT_BRANCH_OFFSET     26

#define INS_DATATYPE                    (WIDTH_DATATYPE)<<SHIFT_DATATYPE
#define	INS_OPCODE			(WIDTH_OPCODE)<<SHIFT_OPCODE
#define INS_PREDICT			(WIDTH_PREDICT)<<SHIFT_PREDICT
#define INS_LE                          (WIDTH_LE)<<SHIFT_LE
#define INS_LMUX			(WIDTH_MUX)<<SHIFT_LMUX
#define INS_RMUX			(WIDTH_MUX)<<SHIFT_RMUX
#define INS_R1				(WIDTH_REGISTER)<<SHIFT_R1
#define INS_R2				(WIDTH_REGISTER)<<SHIFT_R2
#define INS_RW				(WIDTH_REGISTER)<<SHIFT_RW
#define INS_WE				(WIDTH_ENABLE)<<SHIFT_WE
#define INS_AB				(WIDTH_ENABLE)<<SHIFT_ABUS
#define INS_DB				(WIDTH_ENABLE)<<SHIFT_DBUS
#define INS_IMMEDIATE		        (WIDTH_IMMEDIATE)<<SHIFT_IMMEDIATE

// Predicated
// the UL here is to supress the warning. 
#define WIDTH_PDATATYPE         0x3UL
#define WIDTH_POPCODE		0xfUL
#define WIDTH_PMUX		0x7UL
#define WIDTH_PREGISTER	        0xfUL

#define SHIFT_PDATATYPE         61
#define SHIFT_POPCODE		57
#define SHIFT_PLMUX		52
#define SHIFT_PRMUX		49
#define SHIFT_PR1		45
#define SHIFT_PR2		41
#define SHIFT_PRP		37
#define SHIFT_PPMUX		34

#define INS_PDATATYPE                   (WIDTH_PDATATYPE)<<SHIFT_PDATATYPE
#define	INS_POPCODE			(WIDTH_POPCODE)<<SHIFT_POPCODE
#define INS_PLMUX			(WIDTH_PMUX)<<SHIFT_PLMUX
#define INS_PRMUX			(WIDTH_PMUX)<<SHIFT_PRMUX
#define INS_PPMUX			(WIDTH_PMUX)<<SHIFT_PPMUX
#define INS_PR1				(WIDTH_REGISTER)<<SHIFT_PR1
#define INS_PR2				(WIDTH_REGISTER)<<SHIFT_PR2
#define INS_PRP				(WIDTH_REGISTER)<<SHIFT_PRP

// Floating point immediate double precision encoding
#define SHIFT_SIGN      63
#define SHIFT_EXPONENT  52
#define SHIFT_MANTISSA  00

#define WIDTH_SIGN      0x1UL
#define WIDTH_EXPONENT  0x7ffUL
#define WIDTH_MANTISSA  0xfffffffffffffUL

#define INS_SIGN      (WIDTH_SIGN)<<SHIFT_SIGN
#define INS_EXPONENT  (WIDTH_EXPONENT)<<SHIFT_EXPONENT
#define INS_MANTISSA  (WIDTH_MANTISSA)<<SHIFT_MANTISSA


#define _DEBUG(msg...)                                         \
  do                                                         \
{                                                          \
  fflush(stdout);                                        \
  fprintf(stderr,msg);                                   \
  fprintf(stderr,"\n");                                  \
  fflush(stdout);                                        \
} while(0)

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


enum Instruction_Operation
{
  add,  // 0
  sub,
  mult,
  division,
  shiftl,
  shiftr,
  andop,
  orop,
  xorop,
  cmpSGT,
  cmpEQ,  // 10
  cmpNEQ,
  cmpSLT,
  cmpSLEQ,
  cmpSGEQ,
  cmpUGT,
  cmpULT,
  cmpULEQ,
  cmpUGEQ,
  ld_add,
  ld_data,  // 20
  st_add,
  st_data,
  ld_add_cond,
  ld_data_cond,
  loopctrl,
  cond_select,
  route,
  llvm_route,
  cgra_select,
  constant,  // 30
  rem,
  sext,
  bitcast,
  shiftr_logical,
  rest
};


enum OPCode
{
  Add=0,
  Sub,	//1
  Mult,	//2
  AND,	//3
  OR,	//4
  XOR,	//5
  //aritmatic shift right
  cgraASR,	//6
  //No Operation
  NOOP,	//7
  //aritmatic shift left
  cgraASL,	//8
  Div,	//9
  Rem,	//a
  LSHR, //Sqrt,	//b
  //COMPARE INSTRUCTIONS
  EQ, //==	//c
  NEQ, // !=	//d
  GT, //>	//e
  LT //<	//f
};

enum PredOPCode
{
  setConfigBoundry=0,
  LDi,
  LDMi,
  LDUi,
  sel,
  loopexit,
  address_generator,
  nop,
  signExtend
};

enum PEInputMux
{
  Register=0,
  Left,	        //1
  Right,	      //2
  Up,	          //3
  Down,	        //4
  DataBus,      //5
  Immediate,    //6
  Self	        //7
};

enum Datatype
{
  character,      //0
  int32,          //1
  int16,          //2
  float32,        //3
  float64,        //4
  float16,        //5
  empty1,         //6
  empty2          //7
};

typedef union {

    float f;
    struct
    {

        // Order is important. 
        // Here the members of the union data structure 
        // use the same memory (32 bits). 
        // The ordering is taken 
        // from the LSB to the MSB. 
        /*unsigned int mantissa : 23;
        unsigned int exponent : 8;
        unsigned int sign : 1;*/
      unsigned long mantissa : 52;
      unsigned int exponent: 11;
      unsigned int sign: 1;

    } raw;
} FLOAT;

#endif /* DEFINITIONS_H_ */
