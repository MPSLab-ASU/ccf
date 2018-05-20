/*
 * Instruction.cpp
 *
 * Created on: May 24, 2011
 * Author: mahdi
 *
 * Last edited: 15 March 2017
 * Author: Shail Dave
 */

#include "CGRADecoder.h"

CGRADecoder::CGRADecoder()
{
	inst = new CGRA_Instruction*[CGRA_XDim*CGRA_YDim];
	ready_to_fetch=true;
	counter=0;
}

CGRADecoder::~CGRADecoder()
{
}

bool CGRADecoder::is_Ready()
{
	return ready_to_fetch;
}

bool CGRADecoder::is_fetch_completed()
{
	return !ready_to_fetch;
}

void CGRADecoder::Add_More_PE_Instruction(CGRA_Instruction *ins)
{
	inst[counter]=ins;
	counter++;
	if (counter==CGRA_XDim)
	{
		ready_to_fetch=false;
	}
}

void CGRADecoder::Make_Ready_Next()
{
	counter=0;
	ready_to_fetch=true;
}

CGRA_Instruction** CGRADecoder::get_instructions()
{
	return inst;
}
