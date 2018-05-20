/*
 * Instruction.h
 *
 *  Created on: May 24, 2011
 *      Author: mahdi
 */



#include "CGRAInstruction.h"


class CGRADecoder{

public:
	CGRADecoder();

	virtual ~CGRADecoder();

private:
	bool ready_to_fetch;
	int counter;
	CGRA_Instruction **inst;

public:
	bool is_Ready();

	void Add_More_PE_Instruction(CGRA_Instruction *ins);
	void Make_Ready_Next();

	bool is_fetch_completed();

	CGRA_Instruction** get_instructions();


};

