/*
* definitions.h
*
* Created on: May 24, 2011
* Author: mahdi
*
* Last edited: 24 May 2017
* Author: Shail Dave
*/

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include "debug/CGRA.hh"
#include "debug/CGRA_Detailed.hh"
#include "cpu/atomiccgra/base.hh"

#include "CGRAException.h"

#define IMEMSIZE 1024
#define DMEMSIZE 256
#define REGFILESIZE 4

#define CGRA_XDim 4
#define CGRA_YDim 4

#define CGRA_MEMORY_READ 1
#define CGRA_MEMORY_WRITE 2

/*
Instruction Encoding / Decoding for regular instruction format (R-Type)
31	30	29 28|	27   |	26	25	24|	23	22	21|	20	19|	18	17|	16	15|	14|	13 | 12  | 11 10	9	8	7	6	5	4	3	2	1	0 |
  OpCode     |Predict|	   LMUX   |    RMUX   |   R1  |	  R2  |   RW  |	WE| AB |	DB |			Immediate          	  |        /write comparison result to controller

P-Type Instruction Encoding / Decoding
31	30	29 28|	27   |	26	25	24|	23	22	21|	20	19|	18 17 |	16 15	| 14 13 12 | 11 10	9	8	7	6	5	4	3	2	1	0 |
  OpCode     |  1    |	   LMUX   |    RMUX   |   R1  |	  R2  |   RP  |   PMUX 	 |			Immediate						  |
*/

#define WIDTH_OPCODE			0xf
#define WIDTH_PREDICT			0x1
#define WIDTH_MUX					0x7
#define WIDTH_REGISTER		0x3
#define WIDTH_ENABLE			0x1
#define WIDTH_IMMEDIATE		0xfff

#define SHIFT_OPCODE			28
#define SHIFT_PREDICT			27
#define SHIFT_LMUX				24
#define SHIFT_RMUX				21
#define SHIFT_R1					19
#define SHIFT_R2					17
#define SHIFT_RW					15
#define SHIFT_WE					14
#define SHIFT_ABUS				13
#define SHIFT_DBUS				12
#define SHIFT_IMMEDIATE		00

#define	INS_OPCODE		(WIDTH_OPCODE)<<SHIFT_OPCODE
#define INS_PREDICT		(WIDTH_PREDICT)<<SHIFT_PREDICT
#define INS_LMUX			(WIDTH_MUX)<<SHIFT_LMUX
#define INS_RMUX			(WIDTH_MUX)<<SHIFT_RMUX
#define INS_R1				(WIDTH_REGISTER)<<SHIFT_R1
#define INS_R2				(WIDTH_REGISTER)<<SHIFT_R2
#define INS_RW				(WIDTH_REGISTER)<<SHIFT_RW
#define INS_WE				(WIDTH_ENABLE)<<SHIFT_WE
#define INS_AB				(WIDTH_ENABLE)<<SHIFT_ABUS
#define INS_DB				(WIDTH_ENABLE)<<SHIFT_DBUS
#define INS_IMMEDIATE	(WIDTH_IMMEDIATE)<<SHIFT_IMMEDIATE

// P-Type
#define WIDTH_POPCODE		0xf
#define WIDTH_PMUX			0x7
#define WIDTH_PREGISTER	0x3

#define SHIFT_POPCODE		28
#define SHIFT_PLMUX			24
#define SHIFT_PRMUX			21
#define SHIFT_PR1				19
#define SHIFT_PR2				17
#define SHIFT_PRP				15
#define SHIFT_PPMUX			12

#define	INS_POPCODE			(WIDTH_POPCODE)<<SHIFT_POPCODE
#define INS_PLMUX				(WIDTH_PMUX)<<SHIFT_PLMUX
#define INS_PRMUX				(WIDTH_PMUX)<<SHIFT_PRMUX
#define INS_PPMUX				(WIDTH_PMUX)<<SHIFT_PPMUX
#define INS_PR1					(WIDTH_REGISTER)<<SHIFT_PR1
#define INS_PR2					(WIDTH_REGISTER)<<SHIFT_PR2
#define INS_PRP					(WIDTH_REGISTER)<<SHIFT_PRP

enum OPCode
{
	Add=0,
	Sub,
	Mult,
	AND,
	OR,
	XOR,
	//aritmatic shift right
	cgraASR,
	//No Operation
	NOOP,
	//aritmatic shift left
	cgraASL,
	//LOAD IMMEDIATE INSTRUCTIONS
	Div,
	Rem,
	LSHR, //Sqrt,
	//COMPARE INSTRUCTIONS
	EQ, //==
	NEQ, // !=
	GT, //>
	LT //<
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
	Left,
	Right,
	Up,
	Down,
	DataBus,
	Immediate,
	Self
};

//related to debugging messages
#define CGRA_DEBUG 1

#define DEBUG_CGRA(msg...)	\
do {	\
	PRINT_DEBUG(CGRA_DEBUG,msg);	\
} while(0)

#define PRINT_DEBUG(condition,msg...)	\
do {	\
	if((condition)) {	\
		std::cout << msg; \
	}	\
} while(0)

#endif /* DEFINITIONS_H_ */
