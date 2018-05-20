/*
 * CGRA_MemoryPort.h
 *
 *  Created on: Jun 23, 2011
 *      Author: mahdi
 */

#ifndef CGRA_MEMORYPORT_H_
#define CGRA_MEMORYPORT_H_
#include "CGRADataMemory.h"

class CGRA_MemoryPort
{
public:
	CGRA_MemoryPort(int* statusInteface, int* DataBus, uint64_t* addressBus);
	virtual ~CGRA_MemoryPort();
	int getOutput();
	void setData(int data);
	void setAddress(int address);



	void Read();
	void Write();
private:
	int* status;
	int* data;
	uint64_t* address;


};

#endif /* MEMORYPORT_H_ */
