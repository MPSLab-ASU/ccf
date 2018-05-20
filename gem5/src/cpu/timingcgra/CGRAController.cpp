/*
 * Controller.cpp
 *
 *  Created on: Jul 11, 2011
 *      Author: mahdi
 */

#include "CGRAController.h"


CGRA_Controller::CGRA_Controller()
{
	// TODO Auto-generated constructor stub
	PC=0;

}

CGRA_Controller::~CGRA_Controller()
{
	// TODO Auto-generated destructor stub
}

void CGRA_Controller::IncreasePC()
{
	PC=PC+Len;
}

void CGRA_Controller::UpdatePCKernel()
{

	if ((PC-Prolog*Len)==(II*Len))
	{
		PC=Prolog*Len;
	}
}

int CGRA_Controller::getPC()
{
	return PC;
}
void CGRA_Controller::SetLen(int len)
{
	this->Len=len;
}

void CGRA_Controller::SetII(int II)
{
	this->II=II;
}
void CGRA_Controller::SetProlog(int prolog)
{
	this->Prolog=prolog;

}
void CGRA_Controller::SetEpilog(int epilog)
{
	this->Epilog=epilog;
}


bool CGRA_Controller::isProlog()
{
	return (PC<Prolog*Len);
}

bool CGRA_Controller::isOver()
{
	return (PC>=(Prolog+II+Epilog)*Len);
}

