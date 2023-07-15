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

#include "CGRAFPRegisterFile.h"
//#include "debug/FPRegister_Debug.hh"

CGRA_FPRegisterFile::CGRA_FPRegisterFile()
{
  distance = 0; 
  config_boundary = 0; 
  REGFILESIZE = 4;
  //data_RF.reserve(REGFILESIZE);
}

/*CGRA_FPRegisterFile::CGRA_FPRegisterFile(int regsize)
{
	distance= 0;
  config_boundary = 0;
	REGFILESIZE = regsize;
  data_RF.reserve(REGFILESIZE);
}*/

CGRA_FPRegisterFile::~CGRA_FPRegisterFile()
{
  delete[] data_RF; 
}

void CGRA_FPRegisterFile::setFPRF(int regfilesize)
{
    DPRINTF(FPRegfile_DEBUG, "Inside Register file setRF() %d\n",regfilesize);
    REGFILESIZE = regfilesize;
    //data_RF.reserve(REGFILESIZE);
    data_RF = new float[REGFILESIZE];
    for(int i=0; i < regfilesize; i++)
      data_RF[i] = 0; 
    DPRINTF(FPRegfile_DEBUG, "Exiing Register file setRF()\n");
}

float CGRA_FPRegisterFile::Read(int address)
{
    DPRINTF(FPRegfile_DEBUG, "Inside Register file reading address: %d\n", address);
  	if (address >= REGFILESIZE)
  	{
			DPRINTF(CGRA_Detailed,"Requested Register is: %d\n",address);
  		throw new CGRAException("Register access out of range");
  	}
    //DPRINTF(CGRA_Detailed,"\nIN READ FPREGFILE FPR0 = %f\n",data_RF[0]);
    DPRINTF(CGRA_Detailed,"FPR0: %f\tFPR1: %f\tFPR2: %f\tFPR3: %f\n",data_RF[0],data_RF[1],data_RF[2],data_RF[3]);
    if(address < config_boundary)
    {
      DPRINTF(CGRA_Detailed,"\nREAD: FPREG NUMBER: %d dist: %d\n",address,distance);
	    DPRINTF(CGRA_Detailed,"Rotating FPREGS data: %d\tREG: %d\n",data_RF[(address+distance)%config_boundary],(address+distance)%config_boundary);
      DPRINTF(FPRegfile_DEBUG, "Exiting Register file Read()\n");
    	return data_RF[(address+distance)%config_boundary];
    }
    else
    {
      DPRINTF(CGRA_Detailed,"\nREAD: FPREG NUMBER: %d dist: %d\n",address,distance);
	    DPRINTF(CGRA_Detailed,"Rotating FPREGS data: %d\tREG: %d\n",data_RF[(address+distance)%config_boundary],(address+distance)%config_boundary);
      DPRINTF(FPRegfile_DEBUG, "Exiting Register file Read()\n");
       return data_RF[address];
    }
}

void CGRA_FPRegisterFile::Write(int address, float value)
{
    DPRINTF(FPRegfile_DEBUG, "Inside Register file Write()\n");
  	if (address >= REGFILESIZE)
  	{
	  DPRINTF(CGRA_Detailed,"Requested Write Register is: %d\n",address);
  		throw new CGRAException("Register access out of range");
  	}

    DPRINTF(CGRA_Detailed,"\nWRITE: FPREG NUMBER: %d dist: %d\n",address,distance);
    if(address < config_boundary)
    {
        DPRINTF(CGRA_Detailed,"\nWRITE: FPREG NUMBER: %d dist: %d\n",address,distance);
        DPRINTF(CGRA_Detailed,"\nWRITE PHYS FPREG NUMBER: %d\tdata: %d\n",(address+distance)%config_boundary,value);
        data_RF[(address+distance)%config_boundary] = value;
    }
    else
        data_RF[address] = value;
    DPRINTF(CGRA_Detailed,"FPR0: %f\tFPR1: %f\tFPR2: %f\tFPR3: %d\n",data_RF[0],data_RF[1],data_RF[2],data_RF[3]);
    DPRINTF(FPRegfile_DEBUG, "Exiting Register file Write()\n");
}

void CGRA_FPRegisterFile::NextIteration()
{
  DPRINTF(FPRegfile_DEBUG, "Inside Register file Nextiter()\n");
	distance++;
  if(config_boundary > 0)
	   distance=distance%config_boundary;
  else
     distance=0;
  DPRINTF(FPRegfile_DEBUG, "Exiting Register file NExtiter()\n");
}

void CGRA_FPRegisterFile::ClearRF()
{
  DPRINTF(FPRegfile_DEBUG, "Inside Register file ClearRF()\n");
  for(int i=0; i<REGFILESIZE; i++)
    data_RF[i] = 0;
   //data_RF.push_back(0);
  //data_RF.assign(REGFILESIZE, 0);
  DPRINTF(FPRegfile_DEBUG, "ClearRF() passed clearing RF.\n");
  distance = 0;
  DPRINTF(FPRegfile_DEBUG, "Exitng Register file ClearRF()\n");
}
