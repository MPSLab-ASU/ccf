/*
 * RegisterFile.cpp
 *
 * Created on: May 24, 2011
 * Author: mahdi
 *
 * Last edited: 22 May 2017
 * Author: Shail Dave
 */

#include "CGRARegisterFile.h"

CGRA_RegisterFile::CGRA_RegisterFile()
{
	distance= 0;
  config_boundary = 0;
	for(unsigned i=0;i<REGFILESIZE;i++)
		data_RF[i]=0;
}

CGRA_RegisterFile::~CGRA_RegisterFile()
{
}

int CGRA_RegisterFile::Read(int address)
{
  	if (address >= REGFILESIZE)
  	{
			DPRINTF(CGRA_Detailed,"Requested Register is: %d\n",address);
  		throw new CGRAException("Register access out of range");
  	}
    DPRINTF(CGRA_Detailed,"\nIN READ REGFILE R0 = %d\n",data_RF[0]);
    DPRINTF(CGRA_Detailed,"R0: %d\tR1: %d\tR2: %d\tR3: %d\n",data_RF[0],data_RF[1],data_RF[2],data_RF[3]);
    if(address < config_boundary)
    {
      DPRINTF(CGRA_Detailed,"\nREAD: REG NUMBER: %d dist: %d\n",address,distance);
	    DPRINTF(CGRA_Detailed,"Rotating REGS data: %d\tREG: %d\n",data_RF[(address+distance)%config_boundary],(address+distance)%config_boundary);
    	return data_RF[(address+distance)%config_boundary];
    }
    else
    {
       return data_RF[address];
    }
}

void CGRA_RegisterFile::Write(int address, int value)
{
  	if (address >= REGFILESIZE)
  	{
  		throw new CGRAException("Register access out of range");
  	}

    DPRINTF(CGRA_Detailed,"\nWRITE: REG NUMBER: %d dist: %d\n",address,distance);
    if(address < config_boundary)
    {
        DPRINTF(CGRA_Detailed,"\nWRITE: REG NUMBER: %d dist: %d\n",address,distance);
        DPRINTF(CGRA_Detailed,"\nWRITE PHYS REG NUMBER: %d\tdata: %d\n",(address+distance)%config_boundary,value);
        data_RF[(address+distance)%config_boundary] = value;
    }
    else
        data_RF[address] = value;
    DPRINTF(CGRA_Detailed,"R0: %d\tR1: %d\tR2: %d\tR3: %d\n",data_RF[0],data_RF[1],data_RF[2],data_RF[3]);
}

void CGRA_RegisterFile::NextIteration()
{
	distance++;
  if(config_boundary > 0)
	   distance=distance%config_boundary;
  else
     distance=0;
}

void CGRA_RegisterFile::ClearRF()
{
  for(unsigned i=0; i<REGFILESIZE; i++)
    data_RF[i] = 0;
  distance = 0;
}
