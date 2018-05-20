/*
* Instruction.cpp
*
* Created on: May 24, 2011
* Author: mahdi
*
* Last edited: 24 May 2017
* Author: Shail Dave
*/

#include "CGRAInstruction.h"

CGRA_Instruction::CGRA_Instruction()
{
}

CGRA_Instruction::CGRA_Instruction(unsigned int Instructionword)
{
	InsWord = Instructionword;
	ENCODE_instruction();
}

CGRA_Instruction::CGRA_Instruction(OPCode opc,bool predic,PEInputMux LMuxSel,  PEInputMux RMuxSel, int RRegAdd1,int RRegAdd2, int WAdd, bool WE, int ImmVal, bool EDMAdd, bool DMData)
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
}

void CGRA_Instruction::decode_CGRA_Instruction(){
	ENCODE_instruction();
}

OPCode CGRA_Instruction::getOpCode()
{
	return opCode;
}

bool CGRA_Instruction::getPredicator()
{
	return Predicator;
}

PEInputMux CGRA_Instruction::getLeftMuxSelector()
{
	return LeftMuxSelector;
}

PEInputMux CGRA_Instruction::getRightMuxSelector()
{
	return RightMuxSelector;
}

int CGRA_Instruction::getReadRegAddress1()
{
	return ReadRegAddress1;
}

int CGRA_Instruction::getReadRegAddress2()
{
	return ReadRegAddress2;
}

int CGRA_Instruction::getWriteRegAddress()
{
	return WriteRegAddress;
}

bool CGRA_Instruction::getWriteRegisterEnable()
{
	return WriteRegisterEnable;
}

int CGRA_Instruction::getImmediateValue()
{
	return ImmediateValue;
}

bool CGRA_Instruction::getSelectDataMemoryAddressBus()
{
	return SelectDataMemoryAddressBus;
}

bool CGRA_Instruction::getSelectDataMemoryDataBus()
{
	return SelectDataMemoryDataBus;
}

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
		case Div:
		opCode= Div;
		break;
		case Rem:
		opCode= Rem;
		break;
		//case Sqrt:
		case LSHR:
		opCode= LSHR;
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
}

unsigned int CGRA_Instruction::getOpCode_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |=opCode;
	InsWord <<= SHIFT_OPCODE;
	InsWord &= INS_OPCODE;
	return InsWord;
}

unsigned int CGRA_Instruction::getPredicator_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |=Predicator;
	InsWord <<= SHIFT_PREDICT;
	InsWord &= INS_PREDICT;
	return InsWord;
}

unsigned int CGRA_Instruction::getLeftMuxSelector_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |=LeftMuxSelector;
	InsWord <<= SHIFT_LMUX;
	InsWord &= INS_LMUX;
	return InsWord;
}

unsigned int CGRA_Instruction::getRightMuxSelector_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |=RightMuxSelector;
	InsWord <<= SHIFT_RMUX;
	InsWord &= INS_RMUX;
	return InsWord;
}

unsigned int CGRA_Instruction::getReadRegAddress1_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |= ReadRegAddress1;
	InsWord <<= SHIFT_R1;
	InsWord &= INS_R1;
	return InsWord;
}

unsigned int CGRA_Instruction::getReadRegAddress2_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |= ReadRegAddress2;
	InsWord <<= SHIFT_R2;
	InsWord &= INS_R2;
	return InsWord;
}

unsigned int CGRA_Instruction::getWriteRegAddress_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |= WriteRegAddress;
	InsWord <<= SHIFT_RW;
	InsWord &= INS_RW;
	return InsWord;
}

unsigned int CGRA_Instruction::getWriteRegisterEnable_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |= WriteRegisterEnable;
	InsWord <<= SHIFT_WE;
	InsWord &= INS_WE;
	return InsWord;
}

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
	return InsWord;
}

unsigned int CGRA_Instruction::getSelectDataMemoryAddressBus_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |= SelectDataMemoryAddressBus;
	InsWord <<= SHIFT_ABUS;
	InsWord &= INS_AB;
	return InsWord;
}

unsigned int CGRA_Instruction::getSelectDataMemoryDataBus_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |= SelectDataMemoryDataBus;
	InsWord <<= SHIFT_DBUS;
	InsWord &= INS_DB;
	return InsWord;
}

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

// Predicated Instruction Format Defined
Pred_Instruction::Pred_Instruction()
{
}

Pred_Instruction::Pred_Instruction(unsigned int Instructionword)
{
	PredInsWord = Instructionword;
	ENCODE_Pred_instruction();
}

Pred_Instruction::Pred_Instruction(PredOPCode popc,PEInputMux LMuxSel, PEInputMux RMuxSel, PEInputMux PMuxSel, int RRegAdd1,int RRegAdd2, int RRegAddP, int ImmVal)
{
	popCode=popc;
	LeftMuxSelector=LMuxSel;
	RightMuxSelector=RMuxSel;
	PredMuxSelector=PMuxSel;
	ReadRegAddress1=RRegAdd1;
	ReadRegAddress2=RRegAdd2;
	ReadRegAddressP=RRegAddP;
	ImmediateValue=ImmVal;
}

Pred_Instruction::~Pred_Instruction()
{
}

PredOPCode Pred_Instruction::getPredOpCode()
{
	return popCode;
}

PEInputMux Pred_Instruction::getLeftMuxSelector()
{
	return LeftMuxSelector;
}

PEInputMux Pred_Instruction::getRightMuxSelector()
{
	return RightMuxSelector;
}

PEInputMux Pred_Instruction::getPredMuxSelector()
{
	return PredMuxSelector;
}

int Pred_Instruction::getReadRegAddress1()
{
	return ReadRegAddress1;
}

int Pred_Instruction::getReadRegAddress2()
{
	return ReadRegAddress2;
}

int Pred_Instruction::getReadRegAddressP()
{
	return ReadRegAddressP;
}

int Pred_Instruction::getImmediateValue()
{
	return ImmediateValue;
}

void Pred_Instruction::ENCODE_Pred_instruction()
{
	switch((PredInsWord & INS_POPCODE) >> SHIFT_POPCODE){
		case setConfigBoundry:
		popCode = setConfigBoundry;
		break;
		case LDi:
		popCode = LDi;
		break;
		case LDMi:
		popCode = LDMi;
		break;
		case LDUi:
		popCode = LDUi;
		break;
		case sel:
		popCode = sel;
		break;
		case address_generator:
		popCode = address_generator;
	}

	switch((PredInsWord & INS_PLMUX ) >> SHIFT_PLMUX){
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
	switch((PredInsWord & INS_PRMUX ) >> SHIFT_PRMUX){
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
	switch((PredInsWord & INS_PPMUX ) >> SHIFT_PPMUX){
		case Register:
		PredMuxSelector = Register;
		break;
		case Left:
		PredMuxSelector = Left;
		break;
		case Right:
		PredMuxSelector = Right;
		break;
		case Up:
		PredMuxSelector = Up;
		break;
		case Down:
		PredMuxSelector = Down;
		break;
		case DataBus:
		PredMuxSelector = DataBus;
		break;
		case Immediate:
		PredMuxSelector = Immediate;
		break;
		case Self:
		PredMuxSelector= Self;
		break;
	}

	ReadRegAddress1= (PredInsWord & INS_PR1 )>>SHIFT_PR1 ;
	ReadRegAddress2= (PredInsWord & INS_PR2 )>>SHIFT_PR2 ;
	ReadRegAddressP= (PredInsWord & INS_PRP )>>SHIFT_PRP ;
	ImmediateValue= (PredInsWord & INS_IMMEDIATE )>>SHIFT_IMMEDIATE ;
}

unsigned int Pred_Instruction::getPredOpCode_DECODE()
{
	unsigned int PredInsWord = 0;
	PredInsWord |=popCode;
	PredInsWord <<= SHIFT_POPCODE;
	PredInsWord &= INS_POPCODE;
	return PredInsWord;
}

unsigned int Pred_Instruction::getPredicator_DECODE()
{
	unsigned int InsWord = 0;
	InsWord |= 1;
	InsWord <<= SHIFT_PREDICT;
	InsWord &= INS_PREDICT;
	return InsWord;
}

unsigned int Pred_Instruction::getLeftMuxSelector_DECODE()
{
	unsigned int PredInsWord = 0;
	PredInsWord |=LeftMuxSelector;
	PredInsWord <<= SHIFT_PLMUX;
	PredInsWord &= INS_PLMUX;
	return PredInsWord;
}

unsigned int Pred_Instruction::getRightMuxSelector_DECODE()
{
	unsigned int PredInsWord = 0;
	PredInsWord |=RightMuxSelector;
	PredInsWord <<= SHIFT_PRMUX;
	PredInsWord &= INS_PRMUX;
	return PredInsWord;
}

unsigned int Pred_Instruction::getPredMuxSelector_DECODE()
{
	unsigned int PredInsWord = 0;
	PredInsWord |=PredMuxSelector;
	PredInsWord <<= SHIFT_PPMUX;
	PredInsWord &= INS_PPMUX;
	return PredInsWord;
}

unsigned int Pred_Instruction::getReadRegAddress1_DECODE()
{
	unsigned int PredInsWord = 0;
	PredInsWord |= ReadRegAddress1;
	PredInsWord <<= SHIFT_PR1;
	PredInsWord &= INS_PR1;
	return PredInsWord;
}

unsigned int Pred_Instruction::getReadRegAddress2_DECODE()
{
	unsigned int PredInsWord = 0;
	PredInsWord |= ReadRegAddress2;
	PredInsWord <<= SHIFT_PR2;
	PredInsWord &= INS_PR2;
	return PredInsWord;
}

unsigned int Pred_Instruction::getReadRegAddressP_DECODE()
{
	unsigned int PredInsWord = 0;
	PredInsWord |= ReadRegAddressP;
	PredInsWord <<= SHIFT_PRP;
	PredInsWord &= INS_PRP;
	return PredInsWord;
}

unsigned int Pred_Instruction::getImmediateValue_DECODE()
{
	unsigned int PredInsWord = 0;
	if(ImmediateValue > 0Xfff){
		ImmediateValue = 0xfff;
	}
	PredInsWord |= ImmediateValue;
	PredInsWord <<= SHIFT_IMMEDIATE;
	PredInsWord &= INS_IMMEDIATE;
	return PredInsWord;
}

unsigned int Pred_Instruction::DecodePredInstruction(Pred_Instruction* Ins)
{
	unsigned int PredInsWord =0;
	PredInsWord |= Ins->getPredOpCode_DECODE();
	PredInsWord |= Ins->getPredicator_DECODE();
	PredInsWord |= Ins->getLeftMuxSelector_DECODE();
	PredInsWord |= Ins->getRightMuxSelector_DECODE();
	PredInsWord |= Ins->getPredMuxSelector_DECODE();
	PredInsWord |= Ins->getReadRegAddress1_DECODE();
	PredInsWord |= Ins->getReadRegAddress2_DECODE();
	PredInsWord |= Ins->getReadRegAddressP_DECODE();
	PredInsWord |= Ins->getImmediateValue_DECODE();
	return PredInsWord;
}
