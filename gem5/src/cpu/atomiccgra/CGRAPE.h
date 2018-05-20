/*
* PE.h
*
* Created on: May 24, 2011
* Author: mahdi
*
* Last edited: 22 May 2017
* Author: Shail Dave
*/

#include "CGRARegisterFile.h"
#include "CGRAInstruction.h"
#include "CGRA_MemoryPort.h"

#ifndef PE_H_
#define PE_H_

class CGRA_RegisterFile;
class CGRA_Instruction;

class CGRA_PE
{
public:
	CGRA_PE();
	virtual ~CGRA_PE();

	void Fetch(CGRA_Instruction* ins);
	void Decode();
	void Execute();
	void WriteBack();

	int * getOutputPtr();
	int getOutput();

	void SetNeighbours(int* Left, int* Right, int* Up, int* Down);
	void advanceTime();
	void SetController_Reg();
	bool getController_Reg();
	void D_WriteRegfile(int address, int value);
	int GetConfigBoundary();
	void SetConfigBoundary(int config);
	void ClearRegfile();

	void setDataBus(int * data);
	void setAddressBus(uint64_t * addr);
	void setRWStatusBus(int * status);
	void setAlignmentBus(unsigned *alignment);
private:
	bool Controller_Reg;
	int Input1;
	int Input2;
	//Support for Predicated Operations or P-type instructions
	int InputP;
	int Output;
	unsigned config_boundary;

	CGRA_RegisterFile RegFile;
	CGRA_Instruction* ins;

	int *leftIn;
	int *rightIn;
	int *upIn;
	int *downIn;

	int* BsStatus;
	int* dataBs;
	uint64_t* addressBs;
	unsigned* alignmentBs;
};

#endif /* PE_H_ */
