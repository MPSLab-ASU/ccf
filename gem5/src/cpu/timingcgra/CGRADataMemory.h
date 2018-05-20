/*
 * DataMemory.h
 *
 *  Created on: May 24, 2011
 *      Author: mahdi
 */

#ifndef DATAMEMORY_H_
#define DATAMEMORY_H_
#include "CGRAdefinitions.h"


class CGRA_DataMemory
{
public:
	CGRA_DataMemory();
	virtual ~CGRA_DataMemory();



	int Read(int address);
	void Write(int address,int inData);
private:



	int* data;


};

#endif /* DATAMEMORY_H_ */
