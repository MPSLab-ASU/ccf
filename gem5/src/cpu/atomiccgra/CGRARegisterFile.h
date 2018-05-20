/*
 * RegisterFile.h
 *
 * Created on: May 24, 2011
 * Author: Mahdi
 *
 * Last edited: 15 March 2017
 * Author: Shail Dave
 */

#ifndef REGISTERFILE_H_
#define REGISTERFILE_H_

#include "CGRAdefinitions.h"

class CGRA_RegisterFile
{
public:
	CGRA_RegisterFile();
	virtual ~CGRA_RegisterFile();
	int Read(int address);
	void Write(int address, int value);
	void NextIteration();
  void ClearRF();
	int  config_boundary;
	int distance;
private:
	int data_RF[REGFILESIZE];
};

#endif /* REGISTERFILE_H_ */
