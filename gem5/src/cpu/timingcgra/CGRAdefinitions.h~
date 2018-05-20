/*
 * definitions.h
 *
 *  Created on: May 24, 2011
 *      Author: mahdi
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include "CGRAException.h"

#define IMEMSIZE 10000000
#define DMEMSIZE 100000
#define REGFILESIZE 8

#define CGRA_XDim 4
#define CGRA_YDim 4

#define CGRA_MEMORY_READ 1
#define CGRA_MEMORY_WRITE 2

/*Normal Instruction Encoding / Decoding
31	30	29 28|	27   |	26	25	24|	23	22	21|	20	19|	18	17|	16	15|	14|	13|	12 |11	10	9	8	7	6	5	4	3	2	1	0
  OpCode     |Predict|	   LMUX   |    RMUX   |   R1  |	  R2  |   RW  |	WE|	AB|	DB |				Immediate                  
  /write comparison result to controller
  using the last bit as a flag to set the output of the comparison result on the line to the controller (used for supporting the while loop execution)
*/
#define WIDTH_OPCODE		0xf
#define WIDTH_PREDICT		0x1
#define WIDTH_MUX			0x7
#define WIDTH_REGISTER		0x3
#define WIDTH_ENABLE		0x1
#define WIDTH_IMMEDIATE		0xfff

#define SHIFT_OPCODE		28
#define SHIFT_PREDICT		27
#define SHIFT_LMUX			24
#define SHIFT_RMUX			21
#define SHIFT_R1			19
#define SHIFT_R2			17
#define SHIFT_RW			15
#define SHIFT_WE			14
#define SHIFT_ABUS			13
#define SHIFT_DBUS			12
#define SHIFT_IMMEDIATE		00

#define	INS_OPCODE		(WIDTH_OPCODE)<<SHIFT_OPCODE
#define INS_PREDICT		(WIDTH_PREDICT)<<SHIFT_PREDICT
#define INS_LMUX		(WIDTH_MUX)<<SHIFT_LMUX
#define INS_RMUX		(WIDTH_MUX)<<SHIFT_RMUX
#define INS_R1			(WIDTH_REGISTER)<<SHIFT_R1
#define INS_R2			(WIDTH_REGISTER)<<SHIFT_R2
#define INS_RW			(WIDTH_REGISTER)<<SHIFT_RW
#define INS_WE			(WIDTH_ENABLE)<<SHIFT_WE
#define INS_AB			(WIDTH_ENABLE)<<SHIFT_ABUS
#define INS_DB			(WIDTH_ENABLE)<<SHIFT_DBUS
#define INS_IMMEDIATE	(WIDTH_IMMEDIATE)<<SHIFT_IMMEDIATE

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
    LDi,
    LDMi,
    LDUi,
    //COMPARE INSTRUCTIONS
    EQ, //==
    NEQ, // !=
    GT, //>
    LT //<
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

#endif /* DEFINITIONS_H_ */
