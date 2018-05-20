/*
 * Instruction.cpp
 *
 *  Created on: May 24, 2011
 *      Author: mahdi
 */

#include "CGRAInstruction.h"

CGRA_Instruction::CGRA_Instruction()
{
	// TODO Auto-generated constructor stub

}
CGRA_Instruction::CGRA_Instruction(unsigned int Instructionword){

	InsWord = Instructionword;
	ENCODE_instruction();
}
CGRA_Instruction::CGRA_Instruction(OPCode opc,int predic,PEInputMux LMuxSel,  PEInputMux RMuxSel, int RRegAdd1,int RRegAdd2, int WAdd, bool WE, int ImmVal, bool EDMAdd, bool DMData)
{
	opCode=opc;
	Predicator=predic;
	LeftMuxSelector=LMuxSel;
	RightMuxSelector=RMuxSel;
	ReadRegAddress1=RRegAdd1;
	ReadRegAddress2=RRegAdd2;
	WriteRegAddress=WAdd;
	WriteRegisterEnable=WE;
	ImmediateValue=ImmVal;
	SelectDataMemoryAddressBus=EDMAdd;
	SelectDataMemoryDataBus=DMData;
}

CGRA_Instruction::~CGRA_Instruction()
{
	// TODO Auto-generated destructor stub
}

OPCode CGRA_Instruction::getOpCode()
{
	return opCode;
}

//**********************************************************************
int CGRA_Instruction::getPredicator()
{
	return Predicator;
}
//**********************************************************************
PEInputMux CGRA_Instruction::getLeftMuxSelector()
{
	return LeftMuxSelector;
}
//**********************************************************************
PEInputMux CGRA_Instruction::getRightMuxSelector()
{
	return RightMuxSelector;
}
//**********************************************************************

int CGRA_Instruction::getReadRegAddress1()
{
	return ReadRegAddress1;
}
//*********************************************************************
int CGRA_Instruction::getReadRegAddress2()
{
	return ReadRegAddress2;
}
//**********************************************************************
int CGRA_Instruction::getWriteRegAddress()
{
	return WriteRegAddress;
}

//**********************************************************************

bool CGRA_Instruction::getWriteRegisterEnable()
{
	return WriteRegisterEnable;
}
//**********************************************************************
int CGRA_Instruction::getImmediateValue()
{
	return ImmediateValue;
}
//**********************************************************************
bool CGRA_Instruction::getSelectDataMemoryAddressBus()
{
	return SelectDataMemoryAddressBus;
}
//**********************************************************************

bool CGRA_Instruction::getSelectDataMemoryDataBus()
{
	return SelectDataMemoryDataBus;
}
//**********************************************************************
void CGRA_Instruction::ENCODE_instruction()
{
	switch((InsWord & INS_OPCODE) >> SHIFT_OPCODE){
	case Add:
		opCode = Add;
		break;
	case Sub:
		opCode = Sub;
		break;
	case Mult:
		opCode = Mult;
		break;
	case AND:
		opCode = AND;
		break;
	case OR:
		opCode = OR;
		break;
	case XOR:
		opCode = XOR;
		break;
	case cgraASR:
		opCode = cgraASR;
		break;
	case cgraASL:
		opCode = cgraASL;
		break;
	case NOOP:
		opCode= NOOP;
		break;
	case GT:
		opCode= GT;
		break;
	case LT:
		opCode= LT;
		break;
	case EQ:
		opCode= EQ;
		break;
	case NEQ:
		opCode= NEQ;
		break;
	case LDi:
		opCode= LDi;
		break;
	case LDMi:
		opCode= LDMi;
		break;
	case LDUi:
		opCode= LDUi;
		break;
	}
	Predicator= (InsWord & INS_PREDICT )>>SHIFT_PREDICT;

	switch((InsWord & INS_LMUX ) >> SHIFT_LMUX){
	case Register:
		LeftMuxSelector = Register;
		break;
	case Left:
		LeftMuxSelector = Left;
		break;
	case Right:
		LeftMuxSelector = Right;
		break;
	case Up:
		LeftMuxSelector = Up;
		break;
	case Down:
		LeftMuxSelector = Down;
		break;
	case DataBus:
		LeftMuxSelector = DataBus;
		break;
	case Immediate:
		LeftMuxSelector = Immediate;
		break;
	case Self:
		LeftMuxSelector= Self;
		break;
	}
	switch((InsWord & INS_RMUX ) >> SHIFT_RMUX){
	case Register:
		RightMuxSelector = Register;
		break;
	case Left:
		RightMuxSelector = Left;
		break;
	case Right:
		RightMuxSelector = Right;
		break;
	case Up:
		RightMuxSelector = Up;
		break;
	case Down:
		RightMuxSelector = Down;
		break;
	case DataBus:
		RightMuxSelector = DataBus;
		break;
	case Immediate:
		RightMuxSelector = Immediate;
		break;
	case Self:
		RightMuxSelector= Self;
		break;
	}

	ReadRegAddress1= (InsWord & INS_R1 )>>SHIFT_R1 ;
	ReadRegAddress2= (InsWord & INS_R2 )>>SHIFT_R2 ;
	WriteRegAddress= (InsWord & INS_RW )>>SHIFT_RW ;
	WriteRegisterEnable= (InsWord & INS_WE )>>SHIFT_WE ;
	ImmediateValue= (InsWord & INS_IMMEDIATE )>>SHIFT_IMMEDIATE ;
	SelectDataMemoryAddressBus= (InsWord & INS_AB )>>SHIFT_ABUS ;
	SelectDataMemoryDataBus= (InsWord & INS_DB )>>SHIFT_DBUS ;

	/*printf("Encoded Instruction:\n");
	printf("Opcode = %u\n",opCode);
	printf("Predict = %u\n",Predicator);
	printf("LMUX = %u\n", LeftMuxSelector);
	printf("RMUX = %u\n",RightMuxSelector);
	printf("Imm = %u\n",ImmediateValue);
	printf("R1 = %u\n",ReadRegAddress1);
	printf("R2= %u\n",ReadRegAddress2);
	printf("Rw= %u\n",WriteRegAddress);*/

}

//8734605623405964230785456

unsigned int CGRA_Instruction::getOpCode_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |=opCode;
	InsWord <<= SHIFT_OPCODE;
	InsWord &= INS_OPCODE;
	//////printf("Instruction WORD : OpCode = %u\n",InsWord>>SHIFT_OPCODE);
	return InsWord;
}

//**********************************************************************

unsigned int CGRA_Instruction::getPredicator_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |=Predicator;
	InsWord <<= SHIFT_PREDICT;
	InsWord &= INS_PREDICT;
	//////printf("Instruction WORD : Predict = %u\n",InsWord>>SHIFT_PREDICT);
	return InsWord;
}

//**********************************************************************

unsigned int CGRA_Instruction::getLeftMuxSelector_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |=LeftMuxSelector;
	InsWord <<= SHIFT_LMUX;
	InsWord &= INS_LMUX;
	//////printf("Instruction WORD : LMux = %u\n",InsWord>>SHIFT_LMUX);
	return InsWord;
}

//**********************************************************************


unsigned int CGRA_Instruction::getRightMuxSelector_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |=RightMuxSelector;
	InsWord <<= SHIFT_RMUX;
	InsWord &= INS_RMUX;
	//////printf("Instruction WORD : RMux = %u\n",InsWord>>SHIFT_RMUX);
	return InsWord;
}

//**********************************************************************


unsigned int CGRA_Instruction::getReadRegAddress1_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |= ReadRegAddress1;
	InsWord <<= SHIFT_R1;
	InsWord &= INS_R1;
	//////printf("Instruction WORD : R1 = %u\n",InsWord>>SHIFT_R1);
	return InsWord;
}

//**********************************************************************


unsigned int CGRA_Instruction::getReadRegAddress2_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |= ReadRegAddress2;
	InsWord <<= SHIFT_R2;
	InsWord &= INS_R2;
	//////printf("Instruction WORD : R2 = %u\n",InsWord>>SHIFT_R2);
	return InsWord;
}

//**********************************************************************


unsigned int CGRA_Instruction::getWriteRegAddress_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |= WriteRegAddress;
	InsWord <<= SHIFT_RW;
	InsWord &= INS_RW;
	////printf("Instruction WORD : RW = %u\n",InsWord>>SHIFT_RW);
	return InsWord;
}

//**********************************************************************

unsigned int CGRA_Instruction::getWriteRegisterEnable_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |= WriteRegisterEnable;
	InsWord <<= SHIFT_WE;
	InsWord &= INS_WE;
	////printf("Instruction WORD : WEnable = %u\n",InsWord>>SHIFT_WE);
	return InsWord;
}

//**********************************************************************


unsigned int CGRA_Instruction::getImmediateValue_DECODE()
{
	unsigned int InsWord = 0;
	if(ImmediateValue > 0Xfff){
		printf("ERROR: CAN'T HOLD IMMEDIATE VALUE, SETTING VALUE = 0xfff\n");
		ImmediateValue = 0xfff;
	}
	InsWord |= ImmediateValue;
	InsWord <<= SHIFT_IMMEDIATE;
	InsWord &= INS_IMMEDIATE;
	////printf("Instruction WORD : Immediate = %u\n",InsWord>>SHIFT_IMMEDIATE);
	return InsWord;
}

//**********************************************************************

unsigned int CGRA_Instruction::getSelectDataMemoryAddressBus_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |= SelectDataMemoryAddressBus;
	InsWord <<= SHIFT_ABUS;
	InsWord &= INS_AB;
	////printf("Instruction WORD : ABus = %u\n",InsWord>>SHIFT_ABUS);
	return InsWord;
}

//**********************************************************************


unsigned int CGRA_Instruction::getSelectDataMemoryDataBus_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |= SelectDataMemoryDataBus;
	InsWord <<= SHIFT_DBUS;
	InsWord &= INS_DB;
	////printf("Instruction WORD : DBus = %u\n",InsWord>>SHIFT_DBUS);
	return InsWord;
}
//**********************************************************************

unsigned int CGRA_Instruction::DecodeInstruction(CGRA_Instruction* Ins)
{
	unsigned int InsWord =0;
	InsWord |= Ins->getOpCode_DECODE();
	InsWord |= Ins->getPredicator_DECODE();
	InsWord |= Ins->getLeftMuxSelector_DECODE();
	InsWord |= Ins->getRightMuxSelector_DECODE();
	InsWord |= Ins->getReadRegAddress1_DECODE();
	InsWord |= Ins->getReadRegAddress2_DECODE();
	InsWord |= Ins->getWriteRegAddress_DECODE();
	InsWord |= Ins->getWriteRegisterEnable_DECODE();
	InsWord |= Ins->getSelectDataMemoryAddressBus_DECODE();
	InsWord |= Ins->getSelectDataMemoryDataBus_DECODE();
	InsWord |= Ins->getImmediateValue_DECODE();
	return InsWord;
}
