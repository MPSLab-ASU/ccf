/*
 * CGRA_MemoryPort.cpp
 *
 *  Created on: Jun 23, 2011
 *      Author: mahdi
 */

#include "CGRA_MemoryPort.h"

CGRA_MemoryPort::CGRA_MemoryPort(int* statusInterface, int* DataBus, uint64_t* addressBus)
{
	// TODO Auto-generated constructor stub
	this->status=statusInterface;
	this->data = DataBus;
	this->address = addressBus;

}

CGRA_MemoryPort::~CGRA_MemoryPort()
{
	// TODO Auto-generated destructor stub
}

int CGRA_MemoryPort::getOutput()
{
	return (*data);
}
void CGRA_MemoryPort::setData(int data)
{
	(*this->data)=data;
}
void CGRA_MemoryPort::setAddress(int address)
{
	(*this->address)=address;
}

void CGRA_MemoryPort::Read()
{
	(*status)=CGRA_MEMORY_READ;
}
void CGRA_MemoryPort::Write()
{
	(*status)=CGRA_MEMORY_WRITE;
}
