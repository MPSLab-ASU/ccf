/*
* PE.h
*
* Created on: May 24, 2011
* Author: mahdi
*
* Last edited: 22 May 2017
* Author: Shail Dave
*
* Last edited: 10 July 2019
* Author: Mahesh Balasubramanian
* Updates: 1. Parameterized Rfsize and CGRA dims.  
           2. Added support for FP execution.
           3. Added support fot FP registers.
*
*/

#include "CGRARegisterFile.h"
#include "CGRAFPRegisterFile.h"
#include "CGRAInstruction.h"
#include "debug/PE_DEBUG.hh"
#include "debug/CGRA_Execute.hh"
//#include "CGRA_MemoryPort.h"

#ifndef PE_H_
#define PE_H_

class CGRA_RegisterFile;
class CGRA_FPRegisterFile; 
class CGRA_Instruction;

class CGRA_PE
{
private:
    int regsize;
    bool Controller_Reg;

    // inputs for internal use of the PEs
    int Input1;
    int Input2;
    //Support for Predicated Operations or P-type instructions
    bool InputP;
    bool OutputP;
    int Output;
    unsigned config_boundary;

    // FP inputs and outputs
    float FPInput1;
    float FPInput2;
    float FPOutput;
  
    CGRA_RegisterFile RegFile;
    CGRA_FPRegisterFile FPRegFile;
    CGRA_Instruction* ins; 

    //Int lines
    int *leftIn;
    int *rightIn;
    int *upIn;
    int *downIn;

    // FP lines  
    float *FPleftIn;
    float *FPrightIn;  
    float *FPupIn;
    float *FPdownIn;

    // Pred lines
    bool *PredleftIn;
    bool *PredrightIn;
    bool *PredupIn;
    bool *PreddownIn; 

    //:TODO
    //for diagonal
    /*int *leftupIn;
    int *rightupIn;
    int *leftdownIn;
    int *rightdownIn;*/ 

    //for one hop
    /*int *onehopx1In;
    int *onehopx2In; 
    int *onehopy1In;
    int *onehopy2In;*/

    int* BsStatus;  
    int* BsDatatype; 
    float* FdataBs;
    int* dataBs;
    uint64_t* addressBs;
    unsigned* alignmentBs;
    Datatype dt; 

public:
	CGRA_PE(); //default constructor to set the RF size 4 per PE.
	virtual ~CGRA_PE();

	void Fetch(CGRA_Instruction* ins);
	void Decode();
	unsigned IExecute();
  unsigned FExecute();
  void DExecute();
	void WriteBack();

	int* getOutputPtr();
  float* getFPOutputPtr();
  bool *getPredOutputPtr(); 
	float getFPOutput();
  int getOutput();
  bool getPredOutput();   

	void SetNeighbours(int* Left, int* Right, int* Up, int* Down);
  void SetFPNeighbours(float* Left, float* Right, float* Up, float* Down);
  void SetPredNeighbours(bool *Left, bool *Right, bool *Up, bool *Down); 

  //:TODO diagonal and other connection styles.
  //for diagonal connections
  /*void SetDiagonalNeighbors(int *LeftUp, int *RightUp, int *LeftDown, int *RightDown);
  void SetLTCornerDiagonal(int *RightDown, int *LeftUp);
  void SetRTCornerDiagonal(int *LeftDown, int *RightUp);
  void SetTopRowDiagonal(int *LeftDown, int *RightDown);
  void SetRBCornerDiagonal(int *LeftUp, int *RightDown);
  void SetRightColDiagonal(int *LeftUp, int *LeftDown);
  void SetLBCornerDiagonal(int *LeftDown, int *RightUp);
  void SetBottomRowDiagonal(int *LeftUp, int *RightUp);
  void SetLeftColDiagonal(int *RightUp, int *RightDown);
  void SetRegularDiagonal(int *LeftUp, int *RightUp, int *LeftDown, int *RightDown); */
  
  //for one hop connections
  //void SetHopNeighbors(int* OneHopX1, int * OneHopeX2, int* OneHopY1, int* OneHopY2);

	void advanceTime();
  void setRF_per_PE(int rs);
  void setFPRF_per_PE(int rs);
	void SetController_Reg();
	bool getController_Reg();
	void D_WriteRegfile(int address, int value);
	int GetConfigBoundary();
	void SetConfigBoundary(int config);
	void ClearRegfile();
  Datatype GetDatatype(); 

	void setDataBus(int * data);
  void setFDataBus(float * data);
	void setAddressBus(uint64_t * addr);
	void setRWStatusBus(int * status);
	void setAlignmentBus(unsigned *alignment);
  void setDatatypeBus(int * dt);

  bool isNOOP();
};


#endif /* PE_H_ */
