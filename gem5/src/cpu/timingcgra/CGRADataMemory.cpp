/*
 * DataMemory.cpp
 *
 *  Created on: May 24, 2011
 *      Author: mahdi
 */
#define CGRADEBUG

#include "CGRADataMemory.h"

CGRA_DataMemory::CGRA_DataMemory()
{

	data = new int[DMEMSIZE];
	// TODO Auto-generated constructor stub

}

CGRA_DataMemory::~CGRA_DataMemory()
{
	delete(data);
	// TODO Auto-generated destructor stub
}


int CGRA_DataMemory::Read(int address) {
	if (address >= DMEMSIZE) {
		throw new CGRAException("CGRA Data Memory read address is out of range");
	}
#ifdef CGRADEBUG
	//cout << "Reading DMem : Addr:" << address <<"\t"<<"Data:" << data[address] <<"\n" << endl;
#endif //DEBUG
	return data[address];
}


void CGRA_DataMemory::Write(int address, int inData)
{
	if (address >= DMEMSIZE)
	{
		throw new CGRAException("CGRA Data Memory write address is out of range");
	}
#ifdef CGRADEBUG
	//cout << "Writing DMem : Addr:" << address <<"\t"<<"Data:" << inData <<"\n" << endl;
#endif //DEBUG
	data[address] = inData;
}
