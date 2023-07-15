/*
 * RegisterFile.cpp
 *
 * Created on: May 24, 2011
 * Author: mahdi
 *
 * Last edited: 22 May 2017
 * Author: Shail Dave
 *
 * Last edited: 10 July 2019
 * Author: Mahesh Balasubramanian
 * Update: Parameterized Rfsize and CGRA dims
 *
 */

#include "CGRARegisterFile.h"
#include "debug/Register_Debug.hh"

CGRA_RegisterFile::CGRA_RegisterFile()
{
  distance = 0; 
  config_boundary = 0; 
  REGFILESIZE = 4;
  //data_RF.reserve(REGFILESIZE);
}

/*CGRA_RegisterFile::CGRA_RegisterFile(int regsize)
{
	distance= 0;
  config_boundary = 0;
	REGFILESIZE = regsize;
  data_RF.reserve(REGFILESIZE);
}*/

CGRA_RegisterFile::~CGRA_RegisterFile()
{
  delete[] data_RF; 
}

void CGRA_RegisterFile::setRF(int regfilesize)
{
    DPRINTF(Regfile_DEBUG, "Inside Register file setRF() %d\n",regfilesize);
    REGFILESIZE = regfilesize;
    //data_RF.reserve(REGFILESIZE);
    //delete[] data_RF;
    data_RF = new int[REGFILESIZE];
    for(int i=0; i < regfilesize; i++)
      data_RF[i] = 0; 
    DPRINTF(Regfile_DEBUG, "Exiing Register file setRF()\n");
}

int CGRA_RegisterFile::Read(int address)
{
    DPRINTF(Regfile_DEBUG, "Inside Register file Read()\n");
  	if (address >= REGFILESIZE)
  	{
			DPRINTF(CGRA_Detailed,"Requested Register is: %d\n",address);
  		throw new CGRAException("Register access out of range");
  	}
  	
	DPRINTF(CGRA_Detailed,"R0: %d\tR1: %d\tR2: %d\tR3: %d\n",data_RF[0],data_RF[1],data_RF[2],data_RF[3]);
    if(address < config_boundary)
    {
      DPRINTF(CGRA_Detailed,"\nREAD: REG NUMBER: %d dist: %d\n",address,distance);
	    DPRINTF(CGRA_Detailed,"Rotating REGS data: %d\tREG: %d\n",data_RF[(address+distance)%config_boundary],(address+distance)%config_boundary);
      DPRINTF(Regfile_DEBUG, "Exiting Register file Read()\n");
    	return data_RF[(address+distance)%config_boundary];
    }
    else
    {
      DPRINTF(Regfile_DEBUG, "Exiting Register file Read()\n");
       return data_RF[address];
    }
}

void CGRA_RegisterFile::Write(int address, int value)
{
    DPRINTF(Regfile_DEBUG, "Inside Register file Write()\n");
  	if (address >= REGFILESIZE)
  	{
	  DPRINTF(CGRA_Detailed,"Requested Write Register is: %d\n",address);
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
    DPRINTF(Regfile_DEBUG, "Exiting Register file Write()\n");
}

void CGRA_RegisterFile::NextIteration()
{
  DPRINTF(Regfile_DEBUG, "Inside Register file Nextiter()\n");
	distance++;
  if(config_boundary > 0)
	   distance=distance%config_boundary;
  else
     distance=0;
  DPRINTF(Regfile_DEBUG, "Exiting Register file NExtiter()\n");
}

void CGRA_RegisterFile::ClearRF()
{
  DPRINTF(Regfile_DEBUG, "Inside Register file ClearRF()\n");
  for(int i=0; i<REGFILESIZE; i++)
    data_RF[i] = 0;
   //data_RF.push_back(0);
  //data_RF.assign(REGFILESIZE, 0);
  DPRINTF(Regfile_DEBUG, "ClearRF() passed clearing RF.\n");
  distance = 0;
  DPRINTF(Regfile_DEBUG, "Exitng Register file ClearRF()\n");
}
