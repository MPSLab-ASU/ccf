/*
* PE.cpp
*
* Created on: May 24, 2011
* Author: mahdi
*
* Last edited: 30 March 2018
* Author: Shail Dave
*/

#include "CGRAPE.h"
#include <iomanip>
#include <stdio.h>
#include <iostream>

CGRA_PE::CGRA_PE()
{
	Output=0;
}

CGRA_PE::~CGRA_PE()
{
}

int CGRA_PE::getOutput()
{
	return Output;
}

void CGRA_PE::Fetch(CGRA_Instruction* ins)
{
	this->ins = ins;
}

void CGRA_PE::Decode()
{
	// we don't need to separately decode fields in HW implementation
	//ins->decode_CGRA_Instruction();
	if((ins->getOpCode()!=NOOP))
	{
		switch (ins->getLeftMuxSelector())
		{
			case Register:
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
				DPRINTF(CGRA_Detailed,"\n******** DB INPUT1 %d************\n",Input1);
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
				DPRINTF(CGRA_Detailed,"\n******** DB INPUT2 %d************\n",Input2);
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
				InputP = RegFile.Read(predIns->getReadRegAddressP());
				break;
			case Left:
				InputP = (*leftIn);
				break;
			case Right:
				InputP = *(rightIn);
				break;
			case Up:
				InputP = *(upIn);
				break;
			case Down:
				InputP = *(downIn);
				break;
			case Immediate:
				InputP=ins->getImmediateValue();
				break;
			case Self:
				InputP=Output;
				break;
			default:
				throw new CGRAException("CGRA Pred Mux selector out of range");
		}
	}
}

void CGRA_PE::Execute()
{
	int ins_opcode=ins->getOpCode();
	int predicate_bit = ins->getPredicator();

	if(predicate_bit != 1)
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
				DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
				DPRINTF(CGRA_Detailed,"\n******** AND IN THIS PE %d************\n",Output);
				break;
			case OR:
				Output=Input1|Input2;
				DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
				DPRINTF(CGRA_Detailed,"\n******** OR IN THIS PE %d************\n",Output);
				break;
			case XOR:
				Output=Input1^Input2;
				DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
				DPRINTF(CGRA_Detailed,"\n******** XOR IN THIS PE %d************\n",Output);
				break;
			case cgraASR:
				Output=Input1>>Input2;
				DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
				DPRINTF(CGRA_Detailed,"\n******** ASR IN THIS PE %d************\n",Output);
				break;
			case cgraASL:
				Output=Input1<<Input2;
				DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
				DPRINTF(CGRA_Detailed,"\n******** ASL IN THIS PE %d************\n",Output);
				break;
			case GT:
				Output=Input1>Input2;
				DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
				DPRINTF(CGRA_Detailed,"\n******** Greater Than IN THIS PE %d************\n",Output);
				break;
			case LT:
				Output=Input1<Input2;
				DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
				DPRINTF(CGRA_Detailed,"\n******** Less Than IN THIS PE %d************\n",Output);
				break;
			case EQ:
				Output=Input1==Input2;
				DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
				DPRINTF(CGRA_Detailed,"\n******** EQUALS IN THIS PE %d************\n",Output);
				break;
			case NEQ:
				Output=Input1!=Input2;
				DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
				DPRINTF(CGRA_Detailed,"\n******** NOT EQUALS IN THIS PE %d************\n",Output);
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
				DPRINTF(CGRA_Detailed," 1. Opcode is %d\n",(unsigned) ins_opcode);
				throw new CGRAException("Unknown CGRA Opcode");
		}

		if (ins->getWriteRegisterEnable())
		{
			DPRINTF(CGRA_Detailed,"\n************** WE *****************\n");
			int writeRegisterNumber = ins->getWriteRegAddress();

			RegFile.Write(writeRegisterNumber,Output);
			DPRINTF(CGRA_Detailed,"Writing output %d to register %d\n",Output,writeRegisterNumber);
		}

		if(ins_opcode==EQ || ins_opcode==NEQ || ins_opcode==GT || ins_opcode==LT)
		{
			//write the result to the controller bus
			DPRINTF(CGRA_Detailed,"\nCOMPARE INSTRUCTION OUTPUT = %d\n",Output);
		}
	}
	else
	{
		switch (ins_opcode)
		{
			case setConfigBoundry:
				RegFile.config_boundary = Input1;
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
				Output = (InputP == 1) ? Input1 : Input2;
				DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\tInputP = %d\n",Input1, Input2, InputP);
				DPRINTF(CGRA_Detailed,"\n******** Selection IN THIS PE %d************\n",Output);
				break;
			case loopexit:
				Output = ((Input1 == 1) && (Input2 == 0));
				DPRINTF(CGRA_Detailed,"\nInput1 = %d\tInput2 = %d\tInputP = %d\n",Input1, Input2, InputP);
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
				DPRINTF(CGRA_Detailed,"2. Opcode is %d\n",(unsigned) ins_opcode);
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
				Output=(Output<<24) | ( RegFile.Read(writeRegisterNumber));
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
				DPRINTF(CGRA_Detailed,"Writing output %d to register %d\n",Output,writeRegisterNumber);
			}
		}

		if(ins_opcode == address_generator)
		{
			if (ins->getSelectDataMemoryAddressBus())
			{
				DPRINTF(CGRA_Detailed,"\n*********Setting Address %x ******\n",Output);
				(*addressBs) = Output;
				(*BsStatus) = CGRA_MEMORY_READ;
				(*alignmentBs) = Input2;
			}
		}
	}

	if (ins->getSelectDataMemoryDataBus() && (predicate_bit != 1))
	{
		DPRINTF(CGRA_Detailed,"\n******** DB Output %d************\n",Output);
		(*dataBs) = Output;
	}
	DPRINTF(CGRA_Detailed,"Distance is: %d\n",RegFile.distance);
}

void CGRA_PE::WriteBack()
{
	if (ins->getSelectDataMemoryDataBus())
	{
		(*BsStatus) = CGRA_MEMORY_WRITE;
	}
}

void CGRA_PE::SetNeighbours(int* Left,int* Right,int* Up,int* Down)
{
	leftIn = Left;
	rightIn = Right;
	upIn = Up;
	downIn = Down;
}

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
	RegFile.NextIteration();
}

void CGRA_PE::D_WriteRegfile(int address, int value)
{
	RegFile.Write(address,value);
}

void CGRA_PE::ClearRegfile()
{
	RegFile.ClearRF();
}

int * CGRA_PE::getOutputPtr()
{
	return((int*)(&(this->Output)));
}

void CGRA_PE::setDataBus(int * data)
{
	(this->dataBs) = data;
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
