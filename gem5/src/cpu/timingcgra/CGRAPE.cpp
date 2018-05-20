/*
 * PE.cpp
 *
 *  Created on: May 24, 2011
 *      Author: mahdi
 */

#include "CGRAPE.h"
#include <iomanip>

int PE_Count = 0;
int II_Count = 0;

CGRA_PE::CGRA_PE()
{
	RegFile=new CGRA_RegisterFile();
	Output=0;
	CycleCounter=0;
	config_boundary = REGFILESIZE/2;
	// TODO Auto-generated constructor stub

}

CGRA_PE::~CGRA_PE()
{

	// TODO Auto-generated destructor stub
}

int CGRA_PE::getOutput()
{
	return Output;
}

void CGRA_PE::Fetch(CGRA_Instruction* ins)
{
	this->ins = ins;
	//DM->
}
void CGRA_PE::Decode()
{
	//	printf("CGRA: leftmuxselector: %d\n",ins->getLeftMuxSelector());

    int reg = ins->getReadRegAddress1();
	if((ins->getOpCode())!=NOOP)
	{
		switch (ins->getLeftMuxSelector())
		{
            case Register:
                
                //for hybrid RF
                //if(ins->getSelectDataMemoryAddressBus())
                    //reg += REGFILESIZE;
                
                Input1 = RegFile->Read(reg, GetConfigBoundary() );
				break;
			case Left:
				Input1 = LeftNeighbour->getOutput();
				break;
			case Right:
				Input1 = RightNeighbour->getOutput();
				break;
			case Up:
				Input1 = UpNeighbour->getOutput();
				break;
			case Down:
				Input1 = DownNeighbour->getOutput();
				break;
			case DataBus:
				Input1 = MemoryPort->getOutput();
                printf("\n******** DB INPUT1 %d************\n",Input1);
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
				Input2 = RegFile->Read(ins->getReadRegAddress2(), GetConfigBoundary() );
				break;
			case Left:
				Input2 = LeftNeighbour->getOutput();
				break;
			case Right:
				Input2 = RightNeighbour->getOutput();
				break;
			case Up:
				Input2 = UpNeighbour->getOutput();
				break;
			case Down:
				Input2 = DownNeighbour->getOutput();
				break;
			case DataBus:
				Input2 = MemoryPort->getOutput();
                printf("\n******** DB INPUT2 %d************\n",Input2);
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
}

void CGRA_PE::Execute()
{
	int ins_opcode=ins->getOpCode();
	int predicate_bit = ins->getPredicator();
/*
    if(Len != Prolog)
    {
    if(PE_Count == ((CGRA_XDim * CGRA_YDim)-1))
    {
        PE_Count = 0;
        if(II_Count == (II-1))
            II_Count = 0;
        else
            II_Count = II_Count + 1;
    }
    else
        PE_Count = PE_Count + 1;

    if((II_Count == (II-1)) & (PE_Count == ((CGRA_XDim*CGRA_YDim)-1)))
        KernelCount--;
    }
*/
//	if(ins_opcode != 7)
//		//printf("\n******** IN CGRA PE. Opcode: %d************\n",ins_opcode); 
	switch (ins_opcode)
	{
		case Add:
			Output=Input1+Input2;
			printf("\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
			printf("\n******** SUM IN THIS PE %d************\n",Output); 
			break;
		case Sub:
			Output=Input1-Input2;
			printf("\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
			printf("\n******** SUBTRACTION IN THIS PE %d************\n",Output);
			break;
		case Mult:
			Output=Input1*Input2;
			printf("\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
			printf("\n******** PRODUCT IN THIS PE %d************\n",Output);
			break;
		case AND:
			Output=Input1&Input2;
			printf("\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
			printf("\n******** AND IN THIS PE %d************\n",Output);
			break;
		case OR:
			Output=Input1|Input2;
			printf("\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
			printf("\n******** OR IN THIS PE %d************\n",Output);
			break;
		case XOR:
			Output=Input1^Input2;
			printf("\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
			printf("\n******** XOR IN THIS PE %d************\n",Output);
			break;
		case cgraASR:
			Output=Input1>>Input2;
			printf("\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
			printf("\n******** ASR IN THIS PE %d************\n",Output);
			break;
		case cgraASL:
			Output=Input1<<Input2;
			printf("\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
			printf("\n******** ASL IN THIS PE %d************\n",Output);
			break;
		case GT:
			Output=Input1>Input2;
			printf("\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
			printf("\n******** Greater Than IN THIS PE %d************\n",Output);
			break;
		case LT:
			Output=Input1<Input2;
			printf("\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
			printf("\n******** Less Than IN THIS PE %d************\n",Output);
			break;
		case EQ:
			Output=Input1==Input2;
			printf("\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
			printf("\n******** EQUALS IN THIS PE %d************\n",Output);
			break;
		case NEQ:
			Output=Input1!=Input2;
			printf("\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
			printf("\n******** NOT EQUALS IN THIS PE %d************\n",Output);
			break;
		case LDi:
		case LDMi:
		case LDUi:
			if(predicate_bit == 1) {
				SetConfigBoundary(Input1);
				printf("\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
				printf("\n******** LDI IN THIS PE. Setting Configuration as %d************\n",Input1);
			}
			else {		
				Output=Input1;
				printf("\nInput1 = %d\tInput2 = %d\n",Input1, Input2);
				printf("\n******** LDI IN THIS PE %d************\n",Output);
			}
			break;
		case NOOP:
			printf("CGRA: NOOP.Execute()\n");
			break;
		default:
			throw new CGRAException("Unknown CGRA Opcode");
	}

	if (ins->getSelectDataMemoryAddressBus())
	{
        printf("\n*********Setting Address %x ******\n",(unsigned int)Output);
		MemoryPort->setAddress(Output);
		MemoryPort->Read();
	}
	if (ins->getSelectDataMemoryDataBus())
	{
        printf("\n******** DB Output %d************\n",Output);
        MemoryPort->setData(Output);
	}
	if (ins->getWriteRegisterEnable())
    {

  //      printf("\n************** WE *****************\n");

        int writeRegisterNumber = ins->getWriteRegAddress();
/*
        if(ins_opcode==LDi)
        {
            writeRegisterNumber = writeRegisterNumber + REGFILESIZE;
            RegFile->Write(writeRegisterNumber,Output);
        }
        else if(ins_opcode==LDMi)
        {
            writeRegisterNumber = writeRegisterNumber + REGFILESIZE;
            Output=(Output<<12) | ( RegFile->Read(writeRegisterNumber));
            RegFile->Write(writeRegisterNumber,Output);
        }
        else if(ins_opcode==LDUi)
        {
            writeRegisterNumber = writeRegisterNumber + REGFILESIZE;
            Output=(Output<<24) | ( RegFile->Read(writeRegisterNumber));
            RegFile->Write(writeRegisterNumber,Output);
        }
        else
            RegFile->Write(writeRegisterNumber,Output);
	}
*/

        if(ins_opcode==LDMi)
        {
            Output=(Output<<12) | ( RegFile->Read(writeRegisterNumber, GetConfigBoundary() ));
        }
        else if(ins_opcode==LDUi)
        {
            Output=(Output<<24) | ( RegFile->Read(writeRegisterNumber, GetConfigBoundary() ));
	}

        RegFile->Write(writeRegisterNumber,Output, GetConfigBoundary() );
}
	advanceTime();

    if(ins_opcode==EQ || ins_opcode==NEQ || ins_opcode==GT || ins_opcode==LT)
    {
       //write the result to the controller bus
       printf("\nCOMPARE INSTRUCTION OUTPUT = %d\n",Output);

        if(ins_opcode == EQ || ins_opcode == NEQ)
    	    *(this->Controller_Reg) = ~Output;
        else
            *(this->Controller_Reg) = Output;
    }
     //   cout << "Kernelcount " << KernelCount << endl;
       // if(KernelCount < 1)	//If it is 1, then this is last run //Actually We Don't Rely On This.
        //	    *(this->Controller_Reg)=0;

      // KernelCount--;  	
    

}

void CGRA_PE::WriteBack()
{
	if (ins->getSelectDataMemoryDataBus())
	{
		MemoryPort->Write();
	}
}
void CGRA_PE::SetNeighbours(CGRA_PE* Left,CGRA_PE* Right,CGRA_PE* Up,CGRA_PE* Down)
{
	this->LeftNeighbour=Left;
	this->RightNeighbour=Right;
	this->UpNeighbour=Up;
	this->DownNeighbour=Down;

}
void CGRA_PE::setII(int II)
{
	this->II=II;
}
void CGRA_PE::SetCGRA_MemoryPort(CGRA_MemoryPort *MP)
{
	this->MemoryPort=MP;
}
void CGRA_PE::SetController_Reg(int* reg)
{
	this->Controller_Reg=reg;
}
void CGRA_PE::advanceTime()
{
	CycleCounter++;
//    printf("\nCyclecounter %d II %d\n",CycleCounter,II);
	if (CycleCounter%II==0)
	{
 //     printf("\nCALLING NEXT ITERATION\n");
		RegFile->NextIteration(GetConfigBoundary());
	}

}

void CGRA_PE::D_ReadRegfile()
{
	cout <<"R0:"<<setw(6)<< RegFile->Read(0, GetConfigBoundary())<<"\t"\
		<<"R1:"<<setw(6)<< RegFile->Read(1, GetConfigBoundary())<<"\t"\
		<<"R2:"<<setw(6)<< RegFile->Read(2, GetConfigBoundary())<<"\t"\
		<<"R3:"<<setw(6)<< RegFile->Read(3, GetConfigBoundary())<<"\t"\
		<<"Output:"<<setw(6)<<Output<<"\n";
	//cout<< "Input1:"<<Input1<<"\t"<<"Input2:"<<Input2<<endl;
}

void CGRA_PE::D_WriteRegfile(int address, int value)
{
	RegFile->Write(address,value, GetConfigBoundary());
}

void CGRA_PE::SetKernelCount(int KernelCount)
{
	this->KernelCount=KernelCount;
}

int CGRA_PE::GetConfigBoundary()
{
	return this->config_boundary;
}

void CGRA_PE::SetConfigBoundary(int config)
{
	this->config_boundary = config;
}
