/*
 * RegisterFile.h
 *
 * Created on: May 24, 2011
 * Author: Mahdi
 *
 * Last edited: 15 March 2017
 * Author: Shail Dave
 *
 * Last edited: 10 July 2019
 * Author: Mahesh Balasubramanian
 * Update: Parameterized Rfsize and CGRA dims
 *
 */

#ifndef REGISTERFILE_H_
#define REGISTERFILE_H_

#include "CGRAdefinitions.h"
#include "debug/Regfile_DEBUG.hh"

class CGRA_RegisterFile
{
private:
    //int data_RF[REGFILESIZE];
    int REGFILESIZE;
    //int data_RF[REGFILESIZE];
    //vector<int> data_RF;
    int * data_RF = NULL;
public:
    /*these are made public to be accessed by RegFile.*/
    int distance;
    int config_boundary;
    /*other functions start*/ 
	CGRA_RegisterFile(); //default constructor that sets RFSize to 4.
   // CGRA_RegisterFile(int regfilesize); //Parameterized constructor that takes in RFSize as input. 
    void setRF(int regfilesize); 
	virtual ~CGRA_RegisterFile();
	int Read(int address);
	void Write(int address, int value);
	void NextIteration();
  void ClearRF();
};

#endif /* REGISTERFILE_H_ */
