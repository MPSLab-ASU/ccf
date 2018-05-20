/*
* DataMemory.h
*
* Created on: May 24, 2011
* Author: mahdi
*
* Last edited: 15 March 2017
* Author: Shail Dave
*/

#include "CGRAdefinitions.h"
#include <stdint.h>

#ifndef DATAMEMORY_H_
#define DATAMEMORY_H_

class CGRA_DataMemory
{
public:
	CGRA_DataMemory();
	virtual ~CGRA_DataMemory();

	void setPtr(int *dMemptr);
	int Read(int address);
	void Write(int address,int inData);
private:
	int* data;
};

#endif /* DATAMEMORY_H_ */
