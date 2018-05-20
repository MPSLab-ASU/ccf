/*
 * PE.h
 *
 *  Created on: May 24, 2011
 *      Author: mahdi
 */

#include "CGRARegisterFile.h"
#include "CGRAInstruction.h"
#include "CGRA_MemoryPort.h"


#ifndef PE_H_
#define PE_H_

#define CGRA_DEBUG

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

	int getOutput();

	void SetNeighbours(CGRA_PE* Left,CGRA_PE* Right,CGRA_PE* Up,CGRA_PE* Down);

	void setII(int II);
	void SetKernelCount(int KernelCount);
	void advanceTime();
	void SetCGRA_MemoryPort(CGRA_MemoryPort *MP);
	void SetController_Reg(int* reg);
	void D_ReadRegfile();
	void D_WriteRegfile(int address, int value);
	int GetConfigBoundary();
	void SetConfigBoundary(int config);

private:
	CGRA_RegisterFile* RegFile;
	int CycleCounter;
	int II;
	int Input1;
	int Input2;
	int Output;
	int KernelCount;
	int config_boundary;

	CGRA_Instruction* ins;

	CGRA_PE* LeftNeighbour;
	CGRA_PE* RightNeighbour;
	CGRA_PE* UpNeighbour;
	CGRA_PE* DownNeighbour;

	CGRA_MemoryPort *MemoryPort;

    int* Controller_Reg;
};

#endif /* PE_H_ */
