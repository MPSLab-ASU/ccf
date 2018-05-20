/*
 * definitions.h
 *
 *  Created on: May 24, 2011
 *  Author: mahdi
 *
 * Last edited:
 * Author: Shail
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

#define MAX_CONSTANT 4095

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

#define WIDTH_OPCODE		0xf
#define WIDTH_PREDICT		0x1
#define WIDTH_MUX				0x7
#define WIDTH_REGISTER	0x3
#define WIDTH_ENABLE		0x1
#define WIDTH_IMMEDIATE	0xfff

#define SHIFT_OPCODE		28
#define SHIFT_PREDICT		27
#define SHIFT_LMUX			24
#define SHIFT_RMUX			21
#define SHIFT_R1				19
#define SHIFT_R2				17
#define SHIFT_RW				15
#define SHIFT_WE				14
#define SHIFT_ABUS			13
#define SHIFT_DBUS			12
#define SHIFT_IMMEDIATE	00

#define	INS_OPCODE			(WIDTH_OPCODE)<<SHIFT_OPCODE
#define INS_PREDICT			(WIDTH_PREDICT)<<SHIFT_PREDICT
#define INS_LMUX				(WIDTH_MUX)<<SHIFT_LMUX
#define INS_RMUX				(WIDTH_MUX)<<SHIFT_RMUX
#define INS_R1					(WIDTH_REGISTER)<<SHIFT_R1
#define INS_R2					(WIDTH_REGISTER)<<SHIFT_R2
#define INS_RW					(WIDTH_REGISTER)<<SHIFT_RW
#define INS_WE					(WIDTH_ENABLE)<<SHIFT_WE
#define INS_AB					(WIDTH_ENABLE)<<SHIFT_ABUS
#define INS_DB					(WIDTH_ENABLE)<<SHIFT_DBUS
#define INS_IMMEDIATE		(WIDTH_IMMEDIATE)<<SHIFT_IMMEDIATE

// Predicated
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

enum Instruction_Operation
{
	add,
	sub,
	mult,
	division,
	shiftl,
	shiftr,
	andop,
	orop,
	xorop,
	cmpSGT,
	cmpEQ,
	cmpNEQ,
	cmpSLT,
	cmpSLEQ,
	cmpSGEQ,
	cmpUGT,
	cmpULT,
	cmpULEQ,
	cmpUGEQ,
	ld_add,
	ld_data,
	st_add,
	st_data,
	ld_add_cond,
	ld_data_cond,
	loopctrl,
	cond_select,
	route,
	llvm_route,
	cgra_select,
	constant,
	rem,
	sext,
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
	Left,	//1
	Right,	//2
	Up,	//3
	Down,	//4
	DataBus,//5
	Immediate, //6
	Self	//7
};

#endif /* DEFINITIONS_H_ */
