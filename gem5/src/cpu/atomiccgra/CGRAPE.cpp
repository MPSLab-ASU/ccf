/*
 * PE.cpp
 *
/ * Created on: May 24, 2011
 * Author: mahdi
 *
 * Last edited: 30 March 2018
 * Author: Shail Dave
 *
 * Last edited: 10 July 2019
 * Author: Mahesh Balasubramanian
 * Update: Parameterized Rfsize and CGRA dims
 *
 */

#include "CGRAPE.h"
#include <iomanip>
#include <stdio.h>
#include <iostream>

CGRA_PE::CGRA_PE()
{
  DPRINTF(PE_DEBUG, "CGRAPE constructor1\n");
  Output=0;
  DPRINTF(PE_DEBUG, "CGRAPE constructor1 exit\n"); 

}
CGRA_PE::~CGRA_PE()
{
}

void CGRA_PE::setRF_per_PE(int rs)
{
  DPRINTF(PE_DEBUG, "Setting up RF of size %d for each PE\n", rs); 
  RegFile.setRF(rs); 
  DPRINTF(PE_DEBUG, "Exiting setRF_per_PE()\n");
}

void CGRA_PE::setFPRF_per_PE(int rs)
{
  DPRINTF(PE_DEBUG, "Setting up FPRF of size %d for each PE\n", rs);
  FPRegFile.setFPRF(rs);
  DPRINTF(PE_DEBUG, "Exiting setFPRF_per_PE()\n");
}


int CGRA_PE::getOutput()
{
  DPRINTF(PE_DEBUG, "Inside getOutput()\n");
  return Output;
}

float CGRA_PE::getFPOutput()
{
  DPRINTF(PE_DEBUG, "Inside getFPOutput()\n");
  return FPOutput;
}

bool CGRA_PE::getPredOutput()
{
  DPRINTF(PE_DEBUG, "Inside getFPOutput()\n");
  return OutputP;
}

void CGRA_PE::Fetch(CGRA_Instruction* ins)
{
  DPRINTF(PE_DEBUG, "Inside Fetch()\n");
  //delete ins;
  this->ins = ins;
  DPRINTF(PE_DEBUG, "Exiting Fetch()\n");
}

Datatype CGRA_PE::GetDatatype()
{
  return dt; 
}

bool CGRA_PE::isNOOP(){
  return ins->getOpCode() == NOOP;
}

void CGRA_PE::Decode()
{
  DPRINTF(PE_DEBUG, "Inside Decode()\n");

  //DPRINTF(CGRA_Detailed, "ins: %lx\n", ins->getInsWord()); 
  //cout << "predictor bit: " << ins->getPredicator() << "\topcode: " << ins->getOpCode() << endl; 
  // we don't need to separately decode fields in HW implementation
  //ins->decode_CGRA_Instruction();
  dt = ins->getDatatype();

  if(dt == character || dt == int16 || dt == int32)
  {
    if((ins->getOpCode()!=NOOP))
    {
      switch (ins->getLeftMuxSelector())
      {
        case Register:
          DPRINTF(CGRA_Detailed,"\n******** DB INPUT1 ReadAddress = %d************\n",ins->getReadRegAddress1());
          Input1 = RegFile.Read(ins->getReadRegAddress1());
          break;
        case Left:
          Input1 = (*leftIn);
          break;
        case Right:
          Input1 = *(rightIn);
          break;
        case Up:
          Input1 = *(upIn);
          break;
        case Down:
          Input1 = *(downIn);
          break;
        case DataBus:
          Input1 = *(dataBs);
          DPRINTF(CGRA_Detailed,"\n******** DB INPUT1 = %d************\n",Input1);
          break;
        case Immediate:
          Input1=ins->getImmediateValue();
          break;
        case Self:
          Input1=Output;
          break;
        default:
          throw new CGRAException("CGRA Left Mux selector out of range");
      }

      switch (ins->getRightMuxSelector())
      {
        case Register:
          DPRINTF(CGRA_Detailed,"\n******** DB INPUT2 ReadAddress = %d************\n",ins->getReadRegAddress2());
          Input2 = RegFile.Read(ins->getReadRegAddress2());
          break;
        case Left:
          Input2 = (*leftIn);
          break;
        case Right:
          Input2 = *(rightIn);
          break;
        case Up:
          Input2 = *(upIn);
          break;
        case Down:
          Input2 = *(downIn);
          break;
        case DataBus:
          Input2 = *(dataBs);
          DPRINTF(CGRA_Detailed,"\n******** DB INPUT2 = %d************\n",(int)Input2);
          break;
        case Immediate:
          Input2=ins->getImmediateValue();
          break;
        case Self:
          Input2=Output;
          break;
        default:
          throw new CGRAException("CGRA Right Mux selector out of range");
      }
    }

    if(ins->getPredicator() == 1)
    {
      Pred_Instruction temp(ins->DecodeInstruction(ins));
      Pred_Instruction *predIns = &temp;
      switch (predIns->getPredMuxSelector())
      {
        case Register:
          InputP = (bool)RegFile.Read(predIns->getReadRegAddressP());
          break;
	  // 1.9.2022 fix: pred*In -> *In
        case Left:
          InputP = (*leftIn);
          break;
        case Right:
          InputP = (*rightIn);
          break;
        case Up:
          InputP = (*upIn);
          break;
        case Down:
          InputP = (*downIn);
          break;
        case Immediate:
          InputP=(ins->getImmediateValue()==1)? true : false;
          break;
        case Self:
          InputP=(OutputP);
          break;
        default:
          throw new CGRAException("CGRA Pred Mux selector out of range");
      }
    }
  }
  else
  {
    if((ins->getOpCode()!=NOOP))
    {
      if(ins->getOpCode() != address_generator){
	switch (ins->getLeftMuxSelector())
	{
          case Register:
	    DPRINTF(CGRA_Detailed,"\n******** DB FPINPUT1 ReadAddress%d************\n",ins->getReadRegAddress1());
	    FPInput1 = FPRegFile.Read(ins->getReadRegAddress1());
	    break;
          case Left:
	    FPInput1 = (*FPleftIn);
	    break;
          case Right:
	    FPInput1 = *(FPrightIn);
	    break;
          case Up:
	    FPInput1 = *(FPupIn);
	    break;
          case Down:
	    FPInput1 = *(FPdownIn);
	    break;
          case DataBus:
	    FPInput1 = *(FdataBs);
	    DPRINTF(CGRA_Detailed,"\n******** DB FPINPUT1 %f************\n",Input1);
	    break;
          case Immediate:
	    FPInput1=ins->getImmediateValue();
	    break;
          case Self:
	    FPInput1=FPOutput;
	    break;
          default:
	    throw new CGRAException("CGRA Left Mux selector out of range");
	}

	switch (ins->getRightMuxSelector())
	{
	  case Register:
	    DPRINTF(CGRA_Detailed,"\n******** DB FPINPUT2 ReadAddress%d************\n",ins->getReadRegAddress2());
	    FPInput2 = FPRegFile.Read(ins->getReadRegAddress2());
	    break;
	  case Left:
	    FPInput2 = (*FPleftIn);
	    break;
	  case Right:
	    FPInput2 = *(FPrightIn);
	    break;
	  case Up:
	    FPInput2 = *(FPupIn);
	    break;
	  case Down:
	    FPInput2 = *(FPdownIn);
	    break;
	  case DataBus:
	    FPInput2 = *(FdataBs);
	    DPRINTF(CGRA_Detailed,"\n******** DB FPINPUT2 %f************\n",FPInput2);
	    break;
	  case Immediate:
	    FPInput2=(float)(ins->getImmediateValue());
	    break;
	  case Self:
	    FPInput2=FPOutput;
	    break;
	  default:
	    throw new CGRAException("CGRA Right Mux selector out of range");
	}
      
      } else {
	switch (ins->getLeftMuxSelector())
	{
	  case Register:
	    DPRINTF(CGRA_Detailed,"\n******** DB FPINPUT1 ReadAddress%d************\n",ins->getReadRegAddress1());
	    Input1 = FPRegFile.Read(ins->getReadRegAddress1()); // Inspect me: should I be RegFile or FPRegFile?
	    break;
          case Left:
	    Input1 = (*leftIn);
	    break;
          case Right:
	    Input1 = *(rightIn);
	    break;
          case Up:
	    Input1 = *(upIn);
	    break;
          case Down:
	    Input1 = *(downIn);
	    break;
          case DataBus:
	    Input1 = *(dataBs);
	    DPRINTF(CGRA_Detailed,"\n******** DB FPINPUT1 %f************\n",Input1);  // Inspect me: should I be dataBs or FdataBs?
	    break;
          case Immediate:
	    Input1=ins->getImmediateValue();
	    break;
          case Self:
	    Input1=Output;
	    break;
          default:
	    throw new CGRAException("CGRA Left Mux selector out of range");
	}

	switch (ins->getRightMuxSelector())
	{
	  case Register:
	    DPRINTF(CGRA_Detailed,"\n******** DB FPINPUT2 ReadAddress%d************\n",ins->getReadRegAddress2());
	    FPInput2 = FPRegFile.Read(ins->getReadRegAddress2());
	    break;
	  case Left:
	    FPInput2 = (*FPleftIn);
	    break;
	  case Right:
	    FPInput2 = *(FPrightIn);
	    break;
	  case Up:
	    FPInput2 = *(FPupIn);
	    break;
	  case Down:
	    FPInput2 = *(FPdownIn);
	    break;
	  case DataBus:
	    FPInput2 = *(FdataBs);
	    DPRINTF(CGRA_Detailed,"\n******** DB FPINPUT2 %f************\n",FPInput2);
	    break;
	  case Immediate:
	    FPInput2=(float)(ins->getImmediateValue());
	    break;
	  case Self:
	    FPInput2=FPOutput;
	    break;
	  default:
	    throw new CGRAException("CGRA Right Mux selector out of range");
	}
      }
    }

    if(ins->getPredicator() == 1)
    {
      Pred_Instruction temp(ins->DecodeInstruction(ins));
      Pred_Instruction *predIns = &temp;
      switch (predIns->getPredMuxSelector())
      {
        case Register:
          InputP = (bool)(FPRegFile.Read(predIns->getReadRegAddressP()));
          //DPRINTF(CGRA_Detailed,"\n******** INPUTP Reg %f************\n",(FPRegFile.Read(predIns->getReadRegAddressP())));
          break;
        case Left:
          InputP = (*PredleftIn);
          //DPRINTF(CGRA_Detailed,"\n******** INPUTP Left %f************\n",(*FPleftIn));
          break;
        case Right:
          InputP = (*PredrightIn);
          //DPRINTF(CGRA_Detailed,"\n******** INPUTP Right %f************\n",(*FPrightIn));
          break;
        case Up:
          InputP = (*PredupIn);
          //DPRINTF(CGRA_Detailed,"\n******** INPUTP up %f************\n",(*FPupIn));
          break;
        case Down:
          InputP = (*PreddownIn);
          //DPRINTF(CGRA_Detailed,"\n******** INPUTP down %f************\n",(*FPdownIn));
          break;
        case Immediate:
          InputP=(ins->getImmediateValue()==1) ? true: false;
          break;
        case Self:
          InputP=(OutputP);
          //DPRINTF(CGRA_Detailed,"\n******** INPUTP FPOUTPUT %f************\n",(FPOutput));
          break;
        default:
          throw new CGRAException("CGRA Pred Mux selector out of range");
      }
    }
  }  
  DPRINTF(PE_DEBUG, "Exiting Decode()\n");
}


unsigned CGRA_PE::IExecute()
{
  DPRINTF(PE_DEBUG, "Inside Execute()\n");
  int ins_opcode=ins->getOpCode();
  bool predicate_bit = ins->getPredicator();
  bool LE_bit = ins->getLE();
  unsigned branch_offset = 0;
  
  /*DPRINTF(CGRA_Detailed, "Predictor bit: %d\t Opcode: %d\n" , (int) predicate_bit, ins_opcode); 
  if(ins->getLeftMuxSelector() == Register || ins->getRightMuxSelector() == Register){
    DPRINTF(CGRA_Execute, "Register file content:\n"
  }*/

  if(!predicate_bit)
  {
    switch (ins_opcode)
    {
      case Add:
        Output=Input1+Input2;
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** SUM IN THIS PE %d************\n",Output);
        break;
      case Sub:
        Output=Input1-Input2;
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** SUBTRACTION IN THIS PE %d************\n",Output);
        break;
      case Mult:
        Output=Input1*Input2;
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** PRODUCT IN THIS PE %d************\n",Output);
        break;
      case AND:
        Output=Input1&Input2;
        OutputP=(bool)(Input1&Input2); 
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** AND IN THIS PE %d************\n",Output);
        break;
      case OR:
        Output=(Input1|Input2);
        OutputP=(bool)(Input1|Input2); 
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** OR IN THIS PE %d************\n",Output);
        break;
      case XOR:
        Output=(Input1^Input2);
        OutputP=(bool)(Input1^Input2); 
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** XOR IN THIS PE %d************\n",Output);
        break;
      case cgraASR:
        Output=(Input1>>Input2);
        OutputP=(bool)(Input1>>Input2);
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** ASR IN THIS PE %d************\n",Output);
        break;
      case cgraASL:
        Output=(Input1<<Input2);
        OutputP=(bool)(Input1<<Input2);
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** ASL IN THIS PE %d************\n",Output);
        break;
      case GT:
        Output=Input1>Input2;
        OutputP=(bool)(Input1>Input2);
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** Greater Than IN THIS PE %d************\n",OutputP);
        break;
      case LT:
        Output=Input1<Input2;
        OutputP=(bool)(Input1<Input2); 
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** Less Than IN THIS PE %d************\n",OutputP);
        break;
      case EQ:
        Output=Input1==Input2;
        OutputP=(bool)(Input1==Input2);
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** EQUALS IN THIS PE %d************\n",OutputP);
        break;
      case NEQ:
        Output=Input1!=Input2;
        OutputP=(bool)(Input1!=Input2);
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** NOT EQUALS IN THIS PE %d************\n",OutputP);
        break;
      case Div:
        Output=Input1/Input2;
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** DIV IN THIS PE %d************\n",Output);
        break;
      case Rem:
        Output=Input1%Input2;
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** REM IN THIS PE %d************\n",Output);
        break;
      case LSHR:
        {
          unsigned unsignedInput1 = (unsigned) Input1;
          Output = (unsigned)(unsignedInput1 >> (unsigned) Input2);
          DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
          DPRINTF(CGRA_Detailed,"\nunsignedInput1 = %d\tInput2 = %d\n",unsignedInput1, Input2);
          DPRINTF(CGRA_Detailed,"\n******** LSHR IN THIS PE %d************\n",Output);
          DPRINTF(CGRA_Detailed,"\n******** LSHR IN THIS PE %u************\n",Output);
          break;
        }
      case NOOP:
        DPRINTF(CGRA_Detailed,"CGRA: NOOP.Execute()\n");
        break;
      default:
        DPRINTF(CGRA_Detailed," 1. Opcode is %ld\n",(unsigned) ins_opcode);
        throw new CGRAException("Unknown CGRA Opcode");
    }

    if (ins->getWriteRegisterEnable())
    {
      //DPRINTF(CGRA_Detailed,"\n************** WE *****************\n");
      int writeRegisterNumber = ins->getWriteRegAddress();

      RegFile.Write(writeRegisterNumber,Output);
      FPRegFile.Write(writeRegisterNumber,(float)Output);
      DPRINTF(CGRA_Detailed,"Writing output %d to register %d\n",Output,writeRegisterNumber);
    }

    if(LE_bit){
      LE_Instruction temp(ins->getInsWord());
      LE_Instruction *LE_Ins = &temp;

      DPRINTF(CGRA_Execute, "LE Branch: %lx\n", LE_Ins->getBranchOffset());
      if(LE_Ins->getBranchOffset() == 0x3ff){
	if(ins_opcode == NEQ || ins_opcode == LT || ins_opcode == AND || ins_opcode == OR || ins_opcode == XOR)
	  (this->Controller_Reg) = Output;  // Output = 0 to exit
	else this->Controller_Reg = !Output;  // Output = 1 to exit
      }
      else{
	if(ins_opcode == NEQ || ins_opcode == LT || ins_opcode == AND || ins_opcode == OR || ins_opcode == XOR) // Fix me: should NEQ be false to branch?
	  branch_offset = (!Output)? LE_Ins->getBranchOffset():0;
	else
	  branch_offset = (Output == 1)? LE_Ins->getBranchOffset():0;
	DPRINTF(CGRA_Detailed, "\n***LE Instruction - branching %d cycles***\n", branch_offset);
      }
    }

    /*else if(ins_opcode==EQ || ins_opcode==NEQ || ins_opcode==GT || ins_opcode==LT)
    //else if(ins_opcode == EQ) // Modified only for simple_loop benchmark!
    {
      //write the result to the controller bus
      //(this->Controller_Reg) = !((Input1 == 1) && (Input2 == 0));
      (this->Controller_Reg) = (ins_opcode == EQ)? !Output:Output;
      }*/
  }
  else
  { 
    //Pred_Instruction temp(ins->DecodeInstruction(ins));
    //Pred_Instruction *predIns = &temp; 
    switch (ins_opcode) //previously predIns->getPredOpCode().
    {
      case setConfigBoundry:
        RegFile.config_boundary = Input1;
        FPRegFile.config_boundary = Input1;
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** Setting Configuration as %d************\n",Input1);
        break;
      case LDi:
        Output=Input1;
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** LDI IN THIS PE %d************\n",Output);
        break;
      case LDMi:
        Output=Input1;
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** LDMI IN THIS PE %d************\n",Output);
        break;
      case LDUi:
        Output=Input1;
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** LDUI IN THIS PE %d************\n",Output);
        break;
      case sel:
        Output = (InputP == true) ? Input1 : Input2;
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\tInputP = %d\n",Input1, Input2, (int)InputP);
        DPRINTF(CGRA_Detailed,"\n******** Selection IN THIS PE %d************\n",Output);
        break;
      case loopexit:
        Output = ((Input1 == 1) && (Input2 == 0));
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** Loop Exit Control IN THIS PE %d************\n",Output);
        break;
      case address_generator:
        Output=Input1;
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** ADDRESS GENERATED IN THIS PE %d************\n",Output);
        break;
      case signExtend:
        {
          bool maskedBit = (Input1 & (1 << (Input2-1)));
          int shiftAmount = ((1 << Input2)-1);
          int signExtendMask = 0xFFFFFFFF - shiftAmount;
          Output=Input1 & shiftAmount;
          Output = (maskedBit == 1) ? (Output + signExtendMask) : Output;
          DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
          DPRINTF(CGRA_Detailed,"\n******** SIGN EXTENDED IN THIS PE %d************\n",Output);
          break;
        }
      default:
        DPRINTF(CGRA_Detailed,"2. Opcode is %ld\n",(unsigned) ins_opcode);
        throw new CGRAException("Unknown CGRA Opcode");
    }

    if (ins->getWriteRegisterEnable())
    {
      DPRINTF(CGRA_Detailed,"\n************** WE *****************\n");

      int writeRegisterNumber = ins->getWriteRegAddress();
      if(ins_opcode==LDMi)
      {
        Output= (Output << 12) | (RegFile.Read(writeRegisterNumber));
      }
      else if(ins_opcode==LDUi)
      {
        Output=((int)Output<<24) | ( (int)RegFile.Read(writeRegisterNumber));
      }
      else if(ins_opcode==loopexit)
      {
        // If LoopExit is True then, controller reg should be false.
	(this->Controller_Reg) = !((Input1 == 1) && (Input2 == 0));
      }

      //TODO: Fix Micro-architecture for P-type (PredMux) rather than controlling write enable.
      if((ins_opcode==LDi) || (ins_opcode==LDMi) || (ins_opcode==LDUi))
      {
        RegFile.Write(writeRegisterNumber,Output);
        FPRegFile.Write(writeRegisterNumber,(float)Output);
        DPRINTF(CGRA_Detailed,"Writing output %d to register %d\n",Output,writeRegisterNumber);
      }
    }

    if(ins_opcode == address_generator)
    {
      if (ins->getSelectDataMemoryAddressBus())
      {
        DPRINTF(CGRA_Detailed,"\n*********Setting Address %lx ******\n",(unsigned int)Output);
        (*addressBs) = Output;
        (*BsStatus) = CGRA_MEMORY_READ;
        (*BsDatatype) = CGRA_MEMORY_INT; 
        (*alignmentBs) = Input2;
      }
    }
  }

  if (ins->getSelectDataMemoryDataBus() && (!predicate_bit) && (!LE_bit))
  {
    DPRINTF(CGRA_Detailed,"\n******** DB Output %d************\n",Output);
    (*dataBs) = Output;
    (*BsDatatype) = CGRA_MEMORY_INT;
  }
  DPRINTF(CGRA_Detailed,"Distance is: %d\n",RegFile.distance);
  DPRINTF(PE_DEBUG, "Exiting Execute()\n");

  return branch_offset;
}


unsigned CGRA_PE::FExecute()
{
  DPRINTF(PE_DEBUG, "Inside Execute()\n");
  int ins_opcode=ins->getOpCode();
  bool predicate_bit = ins->getPredicator();
  bool LE_bit = ins->getLE();
  unsigned branch_offset = 0;
  //DPRINTF(CGRA_Detailed, "Predictor bit: %d\n", (int) predicate_bit); 

  if(!predicate_bit)
  {
    switch (ins_opcode)
    {
      case Add:
        FPOutput=FPInput1+FPInput2;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** SUM IN THIS PE %f************\n",FPOutput);
        break;
      case Sub:
        FPOutput=FPInput1-FPInput2;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** SUBTRACTION IN THIS PE %f************\n",FPOutput);
        break;
      case Mult:
        FPOutput=FPInput1*FPInput2;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** PRODUCT IN THIS PE %f************\n",FPOutput);
        break;
      case AND:
        FPOutput=((int)FPInput1&(int)FPInput2);
        OutputP=(bool)((int)FPInput1&(int)FPInput2);
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** AND IN THIS PE %f************\n",FPOutput);
        break;
      case OR:
        FPOutput=((int)FPInput1|(int)FPInput2);
        OutputP=(bool)((int)FPInput1|(int)FPInput2);
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** OR IN THIS PE %f************\n",FPOutput);
        break;
      case XOR:
        FPOutput=((int)FPInput1^(int)FPInput2);
        OutputP=(bool)((int)FPInput1^(int)FPInput2);
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** XOR IN THIS PE %f************\n",FPOutput);
        break;
      case cgraASR:
        FPOutput=((int)FPInput1>>(int)FPInput2);
        OutputP=(bool)((int)FPInput1>>(int)FPInput2);
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** ASR IN THIS PE %f************\n",FPOutput);
        break;
      case cgraASL:
        FPOutput=((int)FPInput1<<(int)FPInput2);
        OutputP=(bool)((int)FPInput1<<(int)FPInput2);
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** ASL IN THIS PE %f************\n",FPOutput);
        break;
      case GT:
        FPOutput=(float)FPInput1>(float)FPInput2;
        OutputP=(bool)((float)FPInput1>(float)FPInput2);
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** Greater Than IN THIS PE %f************\n",OutputP);
        break;
      case LT:
        FPOutput=FPInput1<FPInput2;
        OutputP=(bool)(FPInput1<FPInput2);
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** Less Than IN THIS PE %f************\n",OutputP);
        break;
      case EQ:
        FPOutput=FPInput1==FPInput2;
        OutputP=(bool)(FPInput1==FPInput2); 
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** EQUALS IN THIS PE %f************\n",OutputP);
        break;
      case NEQ:
        FPOutput=FPInput1!=FPInput2;
        OutputP=(bool)(FPInput1!=FPInput2); 
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** NOT EQUALS IN THIS PE %f************\n",OutputP);
        break;
      case Div:
        FPOutput=FPInput1/FPInput2;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** DIV IN THIS PE %f************\n",FPOutput);
        break;
      case Rem:
        FPOutput=(int)FPInput1%(int)FPInput2;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** REM IN THIS PE %f************\n",FPOutput);
        break;
      case LSHR:
        {
          unsigned unsignedFPInput1 = (unsigned) FPInput1;
          FPOutput = (unsigned)(unsignedFPInput1 >> (unsigned) FPInput2);
          DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
          DPRINTF(CGRA_Detailed,"\nunsignedFPInput1 = %f\tFPInput2 = %f\n",unsignedFPInput1, FPInput2);
          DPRINTF(CGRA_Detailed,"\n******** LSHR IN THIS PE %f************\n",FPOutput);
          DPRINTF(CGRA_Detailed,"\n******** LSHR IN THIS PE %u************\n",FPOutput);
          break;
        }
      case NOOP:
        DPRINTF(CGRA_Detailed,"CGRA: NOOP.Execute()\n");
        break;
      default:
        DPRINTF(CGRA_Detailed," 1. Opcode is %ld\n",(unsigned) ins_opcode);
        throw new CGRAException("Unknown CGRA Opcode");
    }

    if (ins->getWriteRegisterEnable())
    {
      //DPRINTF(CGRA_Detailed,"\n************** WE *****************\n");
      int writeRegisterNumber = ins->getWriteRegAddress();

      RegFile.Write(writeRegisterNumber,(int)FPOutput);
      FPRegFile.Write(writeRegisterNumber,FPOutput);
      DPRINTF(CGRA_Detailed,"Writing output %f to register %d\n",FPOutput,writeRegisterNumber);
    }

    if(LE_bit){
      LE_Instruction temp(ins->DecodeInstruction(ins));
      LE_Instruction *LE_Ins = &temp;

      if(LE_Ins->getBranchOffset() == 0x3ff){
	if(ins_opcode == NEQ || ins_opcode == LT || ins_opcode == GT || ins_opcode == AND || ins_opcode == OR || ins_opcode == XOR)
	  (this->Controller_Reg) = (Output != 0)? 1:0;
	else this->Controller_Reg = (Output == 0)? 1:0;
      }
      else{
	if(ins_opcode == NEQ || ins_opcode == LT || ins_opcode == GT || ins_opcode == AND || ins_opcode == OR || ins_opcode == XOR) // Fix me: should NEQ be false to branch?
	  branch_offset = (Output == 0)? LE_Ins->getBranchOffset():0;
	else
	  branch_offset = (Output != 0)? LE_Ins->getBranchOffset():0;
	DPRINTF(CGRA_Detailed, "\n***LE Instruction - branching %d cycles***\n", branch_offset);
      }
    }

    
    /*if(LE_bit){
      LE_Instruction temp(ins->DecodeInstruction(ins));
      LE_Instruction *LE_Ins = &temp;
      branch_offset = (FPOutput != 0)? LE_Ins->getBranchOffset():0;
      DPRINTF(CGRA_Detailed, "\nLE Instruction - branching %d cycles\n", branch_offset);
    }
    else if(ins_opcode==EQ || ins_opcode==NEQ || ins_opcode==GT || ins_opcode==LT)
    {
      //write the result to the controller bus
      DPRINTF(CGRA_Detailed,"\nCOMPARE INSTRUCTION OUTPUT = %f\n",FPOutput);
      }*/
  }
  else
  {
    //Pred_Instruction temp(ins->DecodeInstruction(ins));
    //Pred_Instruction *predIns = &temp; 
    switch (ins_opcode) //previously predIns->getPredOpCode().
    {
      case setConfigBoundry:
        RegFile.config_boundary = (int)FPInput1;
        FPRegFile.config_boundary = (int)FPInput1;
        DPRINTF(CGRA_Detailed,"\nInput1 = %f\tInput2 = %f\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** Setting Configuration as %f************\n",Input1);
        break;
      case LDi:
        FPOutput=FPInput1;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** LDI IN THIS PE %f************\n",FPOutput);
        break;
      case LDMi:
        FPOutput=FPInput1;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** LDMI IN THIS PE %f************\n",FPOutput);
        break;
      case LDUi:
        FPOutput=FPInput1;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** LDUI IN THIS PE %f************\n",FPOutput);
        break;
      case sel:
        FPOutput = (InputP == true) ? FPInput2 : FPInput1;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\tInputP = %d\n",FPInput1, FPInput2, (int)InputP);
        DPRINTF(CGRA_Detailed,"\n******** Selection IN THIS PE %f************\n",FPOutput);
        break;
      case loopexit:
        FPOutput = ((FPInput1 == 1) && (FPInput2 == 0));
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\tInputP = %f\n",FPInput1, FPInput2, InputP);
        DPRINTF(CGRA_Detailed,"\n******** Loop Exit Control IN THIS PE %f************\n",FPOutput);
        break;
      case address_generator:
        Output=(int)Input1;
        DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
	DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** ADDRESS GENERATED IN THIS PE %d************\n",Output);
        break;
      case signExtend:
        {
          bool maskedBit = ((int)FPInput1 & (1 << ((int)FPInput2-1)));
          int shiftAmount = ((1 << (int)FPInput2)-1);
          int signExtendMask = 0xFFFFFFFF - shiftAmount;
          FPOutput=(int)FPInput1 & shiftAmount;
          FPOutput = (maskedBit == 1) ? ((int)FPOutput + signExtendMask) :(int) FPOutput;
          DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
          DPRINTF(CGRA_Detailed,"\n******** SIGN EXTENDED IN THIS PE %f************\n",FPOutput);
          break;
        }
      default:
        DPRINTF(CGRA_Detailed,"2. Opcode is %ld\n",(unsigned) ins_opcode);
        throw new CGRAException("Unknown CGRA Opcode");
    }

    if (ins->getWriteRegisterEnable())
    {
      DPRINTF(CGRA_Detailed,"\n************** WE FP*****************\n");

      int writeRegisterNumber = ins->getWriteRegAddress();
      if(ins_opcode==LDMi)
      {
        FPOutput= ((int)FPOutput << 12) | ((int)FPRegFile.Read(writeRegisterNumber));
      }
      else if(ins_opcode==LDUi)
      {
        FPOutput=((int)FPOutput<<24) | ( (int)FPRegFile.Read(writeRegisterNumber));
      }
      else if(ins_opcode==loopexit)
      {
        //This should not happen as loopexit dt is int32.
        // If LoopExit is True then, controller reg should be false.
        (this->Controller_Reg) = !((Input1 == 1) && (Input2 == 0));
      }

      //TODO: Fix Micro-architecture for P-type (PredMux) rather than controlling write enable.
      if((ins_opcode==LDi) || (ins_opcode==LDMi))
      {
        RegFile.Write(writeRegisterNumber,(int)FPOutput);
        FPRegFile.Write(writeRegisterNumber,FPOutput);
        DPRINTF(CGRA_Detailed,"Writing output %f to register %d\n",FPOutput,writeRegisterNumber);
      }
      if((ins_opcode==LDUi))
      {
        FLOAT Output1; 
        Output1.raw.mantissa = ((unsigned int)FPOutput & INS_MANTISSA)>>SHIFT_MANTISSA;
        Output1.raw.exponent = ((unsigned int)FPOutput & INS_EXPONENT)>>SHIFT_EXPONENT;
        Output1.raw.sign = ((unsigned int)FPOutput & INS_SIGN)>>SHIFT_SIGN; 
        FPRegFile.Write(writeRegisterNumber,(Output1.f)); 
        RegFile.Write(writeRegisterNumber,(int)(Output1.f));
        DPRINTF(CGRA_Detailed,"LDUI Writing output value %f to FPregister %d\n",Output1.f,writeRegisterNumber);
      }
    }

    if(ins_opcode == address_generator)
    {
      if (ins->getSelectDataMemoryAddressBus())
      {
        DPRINTF(CGRA_Detailed,"\n*********Setting Address %lx ******\n",(unsigned int)Output);
        (*addressBs) = (unsigned int)Output;
        (*BsStatus) = CGRA_MEMORY_READ;
        (*BsDatatype) = CGRA_MEMORY_FP;
        (*alignmentBs) = (int) FPInput2;
      }
    }
  }

  if (ins->getSelectDataMemoryDataBus() && (!predicate_bit))
  {
    DPRINTF(CGRA_Detailed,"\n******** DB FPOutput %f************\n",FPOutput);
    (*FdataBs) = FPOutput;
    (*BsDatatype) = CGRA_MEMORY_FP;
  }
  DPRINTF(CGRA_Detailed,"Distance is: %d\n",RegFile.distance);
  DPRINTF(PE_DEBUG, "Exiting Execute()\n");

  return branch_offset;
}


/*void CGRA_PE::DExecute()
{
  DPRINTF(PE_DEBUG, "Inside Execute()\n");
  int ins_opcode=ins->getOpCode();
  bool predicate_bit = ins->getPredicator(); 
  //DPRINTF(CGRA_Detailed, "Predictor bit: %d\n", (int) predicate_bit); 

  if(!predicate_bit)
  {
    switch (ins_opcode)
    {
      case Add:
        FPOutput=FPInput1+FPInput2;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** SUM IN THIS PE %f************\n",FPOutput);
        break;
      case Sub:
        FPOutput=FPInput1-FPInput2;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** SUBTRACTION IN THIS PE %f************\n",FPOutput);
        break;
      case Mult:
        FPOutput=FPInput1*FPInput2;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** PRODUCT IN THIS PE %f************\n",FPOutput);
        break;
      case AND:
        FPOutput=(int)FPInput1&(int)FPInput2;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** AND IN THIS PE %f************\n",FPOutput);
        break;
      case OR:
        FPOutput=((int)FPInput1|(int)FPInput2);
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** OR IN THIS PE %f************\n",FPOutput);
        break;
      case XOR:
        FPOutput=((int)FPInput1^(int)FPInput2);
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** XOR IN THIS PE %f************\n",FPOutput);
        break;
      case cgraASR:
        FPOutput=((int)FPInput1>>(int)FPInput2);
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** ASR IN THIS PE %f************\n",FPOutput);
        break;
      case cgraASL:
        FPOutput=((int)FPInput1<<(int)FPInput2);
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** ASL IN THIS PE %f************\n",FPOutput);
        break;
      case GT:
        FPOutput=FPInput1>FPInput2;
        OutputP=(bool)(FPInput1>FPInput2);
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** Greater Than IN THIS PE %f************\n",OutputP);
        break;
      case LT:
        FPOutput=FPInput1<FPInput2;
        OutputP=(bool)(FPInput1<FPInput2); 
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** Less Than IN THIS PE %f************\n",OutputP);
        break;
      case EQ:
        FPOutput=FPInput1==FPInput2;
        OutputP=(bool)(FPInput1==FPInput2); 
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** EQUALS IN THIS PE %f************\n",OutputP);
        break;
      case NEQ:
        FPOutput=FPInput1!=FPInput2;
        OutputP=(bool)(FPInput1!=FPInput2);
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** NOT EQUALS IN THIS PE %f************\n",OutputP);
        break;
      case Div:
        FPOutput=FPInput1/FPInput2;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** DIV IN THIS PE %f************\n",FPOutput);
        break;
      case Rem:
        FPOutput=(int)FPInput1%(int)FPInput2;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** REM IN THIS PE %f************\n",FPOutput);
        break;
      case LSHR:
        {
          unsigned unsignedFPInput1 = (unsigned) FPInput1;
          FPOutput = (unsigned)(unsignedFPInput1 >> (unsigned) FPInput2);
          DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
          DPRINTF(CGRA_Detailed,"\nunsignedFPInput1 = %f\tFPInput2 = %f\n",unsignedFPInput1, FPInput2);
          DPRINTF(CGRA_Detailed,"\n******** LSHR IN THIS PE %f************\n",FPOutput);
          DPRINTF(CGRA_Detailed,"\n******** LSHR IN THIS PE %u************\n",FPOutput);
          break;
        }
      case NOOP:
        DPRINTF(CGRA_Detailed,"CGRA: NOOP.Execute()\n");
        break;
      default:
        DPRINTF(CGRA_Detailed," 1. Opcode is %ld\n",(unsigned) ins_opcode);
        throw new CGRAException("Unknown CGRA Opcode");
    }

    if (ins->getWriteRegisterEnable())
    {
      DPRINTF(CGRA_Detailed,"\n************** WE *****************\n");
      int writeRegisterNumber = ins->getWriteRegAddress();

      RegFile.Write(writeRegisterNumber,(int)FPOutput);
      FPRegFile.Write(writeRegisterNumber,FPOutput);
      DPRINTF(CGRA_Detailed,"Writing output %f to register %d\n",FPOutput,writeRegisterNumber);
    }

    if(ins_opcode==EQ || ins_opcode==NEQ || ins_opcode==GT || ins_opcode==LT)
    {
      //write the result to the controller bus
      DPRINTF(CGRA_Detailed,"\nCOMPARE INSTRUCTION OUTPUT = %f\n",FPOutput);
    }
  }
  else
  {
    //Pred_Instruction temp(ins->DecodeInstruction(ins));
    //Pred_Instruction *predIns = &temp; 
    switch (ins_opcode) //previously predIns->getPredOpCode().
    {
      case setConfigBoundry:
        RegFile.config_boundary = (int)FPInput1;
        DPRINTF(CGRA_Detailed,"\nInput1 = %f\tInput2 = %f\n",Input1, Input2);
        DPRINTF(CGRA_Detailed,"\n******** Setting Configuration as %f************\n",Input1);
        break;
      case LDi:
        FPOutput=FPInput1;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** LDI IN THIS PE %f************\n",FPOutput);
        break;
      case LDMi:
        FPOutput=FPInput1;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** LDMI IN THIS PE %f************\n",FPOutput);
        break;
      case LDUi:
        FPOutput=FPInput1;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** LDUI IN THIS PE %f************\n",FPOutput);
        break;
      case sel:
        FPOutput = (InputP == true) ? FPInput2 : FPInput1;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\tInputP = %d\n",FPInput1, FPInput2, (int)InputP);
        DPRINTF(CGRA_Detailed,"\n******** Selection IN THIS PE %f************\n",FPOutput);
        break;
      case loopexit:
        FPOutput = ((FPInput1 == 1) && (FPInput2 == 0));
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\tInputP = %f\n",FPInput1, FPInput2, InputP);
        DPRINTF(CGRA_Detailed,"\n******** Loop Exit Control IN THIS PE %f************\n",FPOutput);
        break;
      case address_generator:
        FPOutput=(int)FPInput1;
        DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
        DPRINTF(CGRA_Detailed,"\n******** ADDRESS GENERATED IN THIS PE %f************\n",FPOutput);
        break;
      case signExtend:
        {
          bool maskedBit = ((int)FPInput1 & (1 << ((int)FPInput2-1)));
          int shiftAmount = ((1 << (int)FPInput2)-1);
          int signExtendMask = 0xFFFFFFFF - shiftAmount;
          FPOutput=(int)FPInput1 & shiftAmount;
          FPOutput = (maskedBit == 1) ? ((int)FPOutput + signExtendMask) :(int) FPOutput;
          DPRINTF(CGRA_Detailed,"\nFPInput1 = %f\tFPInput2 = %f\n",FPInput1, FPInput2);
          DPRINTF(CGRA_Detailed,"\n******** SIGN EXTENDED IN THIS PE %f************\n",FPOutput);
          break;
        }
      default:
        DPRINTF(CGRA_Detailed,"2. Opcode is %ld\n",(unsigned) ins_opcode);
        throw new CGRAException("Unknown CGRA Opcode");
    }

    if (ins->getWriteRegisterEnable())
    {
      DPRINTF(CGRA_Detailed,"\n************** WE FP*****************\n");

      int writeRegisterNumber = ins->getWriteRegAddress();
      if(ins_opcode==LDMi)
      {
        FPOutput= ((int)FPOutput << 12) | ((int)FPRegFile.Read(writeRegisterNumber));
      }
      else if(ins_opcode==LDUi)
      {
        FPOutput=((int)FPOutput<<24) | ( (int)FPRegFile.Read(writeRegisterNumber));
      }
      else if(ins_opcode==loopexit)
      {
        //This should not happen as loopexit dt is int32.
        // If LoopExit is True then, controller reg should be false.
        (this->Controller_Reg) = !((FPInput1 == 1) && (FPInput2 == 0));
      }

      //TODO: Fix Micro-architecture for P-type (PredMux) rather than controlling write enable.
      if((ins_opcode==LDi) || (ins_opcode==LDMi))
      {
        RegFile.Write(writeRegisterNumber,(int)FPOutput);
        FPRegFile.Write(writeRegisterNumber,FPOutput);
        DPRINTF(CGRA_Detailed,"Writing output %f to register %d\n",FPOutput,writeRegisterNumber);
      }
      if((ins_opcode==LDUi))
      {
        FLOAT Output1; 
        Output1.raw.mantissa = ((unsigned int)FPOutput & INS_MANTISSA)>>SHIFT_MANTISSA;
        Output1.raw.exponent = ((unsigned int)FPOutput & INS_EXPONENT)>>SHIFT_EXPONENT;
        Output1.raw.sign = ((unsigned int)FPOutput & INS_SIGN)>>SHIFT_SIGN; 
        RegFile.Write(writeRegisterNumber,(int)(Output1.f)); 
        FPRegFile.Write(writeRegisterNumber,(Output1.f));
        DPRINTF(CGRA_Detailed,"LDUI Writing output %f to register %d\n",FPOutput,writeRegisterNumber);

      }
    }

    if(ins_opcode == address_generator)
    {
      if (ins->getSelectDataMemoryAddressBus())
      {
        DPRINTF(CGRA_Detailed,"\n*********Setting Address %lx ******\n",(unsigned int)Output);
        (*addressBs) = (int)FPOutput;
        (*BsStatus) = CGRA_MEMORY_READ;
        (*BsDatatype) = CGRA_MEMORY_FP;
        (*alignmentBs) = FPInput2;
      }
    }
  }

  if (ins->getSelectDataMemoryDataBus() && (!predicate_bit))
  {
    DPRINTF(CGRA_Detailed,"\n******** DB DOutput %f************\n",FPOutput);
    (*FdataBs) = FPOutput;
    (*BsDatatype) = CGRA_MEMORY_FP; 
  }
  DPRINTF(CGRA_Detailed,"Distance is: %d\n",RegFile.distance);
  DPRINTF(PE_DEBUG, "Exiting Execute()\n");
}*/


void CGRA_PE::WriteBack()
{
  DPRINTF(PE_DEBUG, "Inside WB()\n");
  bool ins_predicate = ins->getPredicator();
  bool LE_bit = ins->getLE();
  if(!ins_predicate && !LE_bit) {

    if (ins->getSelectDataMemoryDataBus())
      {
	(*BsStatus) = CGRA_MEMORY_WRITE;
	dt = ins->getDatatype();
	
	if(dt == character || dt == int16 || dt == int32)
	  (*BsDatatype) = CGRA_MEMORY_INT;
	else
	  (*BsDatatype) = CGRA_MEMORY_FP; 
      }
    DPRINTF(PE_DEBUG, "Exiting WB()\n");
  }
  delete ins;
}

void CGRA_PE::SetNeighbours(int* Left,int* Right,int* Up,int* Down)
{
  DPRINTF(PE_DEBUG, "Inside SetNeighbors()\n");
  leftIn = Left;
  rightIn = Right;
  upIn = Up;
  downIn = Down;
  DPRINTF(PE_DEBUG, "Exiting SetNeighbors()\n");
}

void CGRA_PE::SetFPNeighbours(float* Left,float* Right,float* Up,float* Down)
{
  DPRINTF(PE_DEBUG, "Inside SetFPNeighbors()\n");
  FPleftIn = Left;
  FPrightIn = Right;
  FPupIn = Up;
  FPdownIn = Down;
  DPRINTF(PE_DEBUG, "Exiting SetFPNeighbors()\n");
}

void CGRA_PE::SetPredNeighbours(bool *Left,bool *Right,bool *Up, bool* Down)
{
  DPRINTF(PE_DEBUG, "Inside SetPredNeighbors()\n");
  PredleftIn = Left;
  PredrightIn = Right;
  PredupIn = Up;
  PreddownIn = Down;
  DPRINTF(PE_DEBUG, "Exiting SetPredNeighbors()\n");

}

//FOR DIOAGONAL CONNECTIONS
/*void CGRA_PE::SetDiagonalNeighbors(int *LeftUp, int *RightUp, int *LeftDown, int *RightDown)
  {
  leftupIn = LeftUp;
  rightupIn = RightUp;
  leftdownIn = LeftDown; 
  rightdownIn = RightDown;
  }

  void CGRA_PE::SetLTCornerDiagonal(int *RightDown, int *LeftUp)
  {
  SetDiagonalNeighbors(LeftUp, 0, 0, RightDown);
  }

  void CGRA_PE::SetRTCornerDiagonal(int *LeftDown, int *RightUp)
  {
  SetDiagonalNeighbors(0, RightUp, LeftDown, 0);
  }

  void CGRA_PE::SetTopRowDiagonal(int *LeftDown, int *RightDown)
  {
  SetDiagonalNeighbors(0, 0, LeftDown, RightDown);
  }

  void CGRA_PE::SetRBCornerDiagonal(int *LeftUp, int *RightDown)
  {
  SetDiagonalNeighbors(LeftUp, 0, 0, RightDown);
  }

  void CGRA_PE::SetRightColDiagonal(int *LeftUp, int *LeftDown)
  {
  SetDiagonalNeighbors(LeftUp, 0, LeftDown, 0);
  }

  void CGRA_PE::SetLBCornerDiagonal(int *LeftDown, int *RightUp)
  {
  SetDiagonalNeighbors(0, RightUp, LeftDown, 0);
  }

  void CGRA_PE::SetBottomRowDiagonal(int *LeftUp, int *RightUp)
  {
  SetDiagonalNeighbors(LeftUp, RightUp, 0, 0);
  }

  void CGRA_PE::SetLeftColDiagonal(int *RightUp, int *RightDown)
  {
  SetDiagonalNeighbors(0, RightUp, 0, RightDown);
  }

  void CGRA_PE::SetRegularDiagonal(int *LeftUp, int *RightUp, int *LeftDown, int *RightDown)
  {
  SetDiagonalNeighbors(LeftUp, RightUp, LeftDown, RightDown);
  }

// FOR ONE HOP CONNECTION
void CGRA_PE::SetHopNeighbors(int* OneHopX1, int* OneHopX2, int* OneHopY1, int *OneHopY2)
{
onehopx1In = OneHopX1;
onehopx2In = OneHopX2;
onehopy1In = OneHopY1;
onehopy2In = OneHopY2; 
}*/ 

void CGRA_PE::SetController_Reg()
{
  Controller_Reg = true;
}

bool CGRA_PE::getController_Reg()
{
  return this->Controller_Reg;
}

void CGRA_PE::advanceTime()
{
  DPRINTF(PE_DEBUG, "Inside advanceTime()\n");
  RegFile.NextIteration();
  FPRegFile.NextIteration();
  DPRINTF(PE_DEBUG, "Exiting advanceTime()\n");
}

void CGRA_PE::D_WriteRegfile(int address, int value)
{
  DPRINTF(PE_DEBUG, "Inside WriteRegFile()\n");
  RegFile.Write(address,value);
  DPRINTF(PE_DEBUG, "Exiting WriteRegFile()\n");
}

void CGRA_PE::ClearRegfile()
{
  DPRINTF(PE_DEBUG, "Inside clearRegfile()\n");
  RegFile.ClearRF();
  DPRINTF(PE_DEBUG, "Exiting clearRegfile()\n");
}

float *CGRA_PE::getFPOutputPtr()
{
  return((float*)(&(this->FPOutput)));
}

int *CGRA_PE::getOutputPtr()
{
  return((&(this->Output)));
}

bool *CGRA_PE::getPredOutputPtr()
{
  return((&(this->OutputP)));
}

void CGRA_PE::setDataBus(int * data)
{
  (this->dataBs) = data;
}

void CGRA_PE::setFDataBus(float * data)
{
  (this->FdataBs) = data;
}

void CGRA_PE::setAddressBus(uint64_t * addr)
{
  (this->addressBs) = addr;
}

void CGRA_PE::setRWStatusBus(int * status)
{
  (this->BsStatus) = status;
}

void CGRA_PE::setAlignmentBus(unsigned *alignment)
{
  (this->alignmentBs) = alignment;
}

void CGRA_PE::setDatatypeBus(int * dt)
{
  (this->BsDatatype) = dt; 
}

