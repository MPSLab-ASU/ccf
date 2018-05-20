/*
 * Controller.h
 *
 *  Created on: Jul 11, 2011
 *      Author: mahdi
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

class CGRA_Controller
{
public:
	CGRA_Controller();
	virtual ~CGRA_Controller();
	void IncreasePC();
	void UpdatePCKernel();
	int getPC();
	void SetLen(int len);
	void SetII(int II);
	void SetProlog(int prolog);
	void SetEpilog(int epilog);

	bool isProlog();

	bool isOver();



private:
	int PC;
	int II;
	int Epilog;
	int Prolog;
	int Len;
};

#endif /* CONTROLLER_H_ */
