/*
 * RegisterFile.cpp
 *
 *  Created on: May 24, 2011 Author: mahdi
 *  Modified on: June 14, 2016 Author: Shail
 */

#include "CGRARegisterFile.h"

CGRA_RegisterFile::CGRA_RegisterFile()
{
    /* Previously
    //0 to REGFILESIZE-1: rotating
    //REGFILESIZE to 2*REGFILESIZE-1: fixed
	data = new int[2*REGFILESIZE];
	distance=0;
	for(int i=0;i<2*REGFILESIZE;i++)
	{
		data[i]=0;
	}
	// TODO Auto-generated constructor stub
    */
 	data = new int[REGFILESIZE];
	distance=0;
	for(int i=0;i<REGFILESIZE;i++)
	{
		data[i]=0;
	}
}

CGRA_RegisterFile::~CGRA_RegisterFile()
{
	delete(data);
	// TODO Auto-generated destructor stub
}

int CGRA_RegisterFile::Read(int address, int config)
{
        /* Previously
	if (address >= 2*REGFILESIZE)
	{
		throw new CGRAException("Register access out of range");
	}

    printf("\nREG NUMBER: %d dist: %d\n",address,distance);
    printf("\nIN READ REGFILE\n"); 

    //0 to REGFILESIZE-1: rotating
    //REGFILESIZE to 2*REGFILESIZE-1: fixed
    if(address < REGFILESIZE)
    {
        printf("\nREAD PHYS REG NUMBER: %d\n",(address+distance)%REGFILESIZE);
	printf("\nRotating REGS data: %d\n",data[(address+distance)%REGFILESIZE]); 
    	return data[(address+distance)%REGFILESIZE];
    }
    else
    {
	printf("\nREAD NONROTATING PHYS REG NUMBER: %d With Data: %d\n",address,data[address]);
        return data[address];
    } 
    */

	if (address >= REGFILESIZE)
	{
		printf("Requested Register is: %d\n",address);
		throw new CGRAException("Register access out of range");
	}

    printf("\nREG NUMBER: %d dist: %d\n",address,distance);
    printf("\nIN READ REGFILE\n"); 
    printf("R0: %d\tR1: %d\tR2: %d\tR3: %d\n",data[0],data[1],data[2],data[3]);
    if(address < config)
    {
        printf("\nREAD PHYS REG NUMBER: %d\n",(address+distance)%config);
	printf("\nRotating REGS data: %d\n",data[(address+distance)%config]); 
    	return data[(address+distance)%config];
    }
    else
    {
	printf("\nREAD NONROTATING PHYS REG NUMBER: %d With Data: %d\n",address,data[address]);
        return data[address];
    } 
}
void CGRA_RegisterFile::Write(int address, int value, int config)
{
/*
	if (address >= 2*REGFILESIZE)
	{
		throw new CGRAException("Register access out of range");
	}

    //0 to REGFILESIZE-1: rotating
    //REGFILESIZE to 2*REGFILESIZE-1: fixed

    if(address < REGFILESIZE)
    {
        printf("\nWRITE PHYS REG NUMBER: %d\n",(address+distance)%REGFILESIZE);
	    data[(address+distance)%REGFILESIZE] = value;
    }
    else
        data[address] = value;	 
*/
	printf("Requested Register is: %d\n",address);
	if (address >= REGFILESIZE)
	{
		throw new CGRAException("Register access out of range");
	}

    if(address < config)
    {
        printf("\nWRITE PHYS REG NUMBER: %d\n",(address+distance)%config);
        data[(address+distance)%config] = value;
    }
    else
        data[address] = value;	 
    printf("R0: %d\tR1: %d\tR2: %d\tR3: %d\n",data[0],data[1],data[2],data[3]);

}
void CGRA_RegisterFile::NextIteration(int config)
{
	distance++;
	distance=distance%config;
        //if(distance==0) printf("\nDistance is 0.\n");
 	//printf("\nIN NEXT ITERATION\n"); 

}
