/*
 * RegisterFile.h
 *
 *  Created on: May 24, 2011
 *      Author: mahdi
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
private:
	int *data;

	int distance;


};

#endif /* REGISTERFILE_H_ */
