/*
 * DataMemory.cpp
 *
 * Created on: May 24, 2011
 * Author: mahdi
 *
 * Last edited: 15 March 2017
 * Author: Shail Dave
 */

#include "CGRADataMemory.h"
#include <stdio.h>

CGRA_DataMemory::CGRA_DataMemory()
{
}

CGRA_DataMemory::~CGRA_DataMemory()
{
}

void CGRA_DataMemory::setPtr(int *dMemptr)
{
}

int CGRA_DataMemory::Read(int address) {
	if (address >= DMEMSIZE) {
		throw new CGRAException("CGRA Data Memory read address is out of range");
	}
	return data[address];
}


void CGRA_DataMemory::Write(int address, int inData)
{
	if (address >= DMEMSIZE)
	{
		throw new CGRAException("CGRA Data Memory write address is out of range");
	}
	data[address] = inData;
}
