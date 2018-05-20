/*
 * CGRA_MemoryPort.cpp
 *
 * Created on: Jun 23, 2011
 * Author: mahdi
 *
 * Last edited: 15 March 2017
 * Author: Shail Dave
 */

#include "CGRA_MemoryPort.h"

CGRA_MemoryPort::CGRA_MemoryPort(int* statusInterface, int* DataBus, uint64_t* addressBus)
{
	this->status=statusInterface;
	this->data = DataBus;
	this->address = addressBus;
}

CGRA_MemoryPort::~CGRA_MemoryPort()
{
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
