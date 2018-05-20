/*
 * Instruction.h
 *
 * Created on: May 24, 2011
 * Author: mahdi
 *
 * edited: 24 May 2017
 * Auhtor: Shail Dave
 */

#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "CGRAdefinitions.h"

class CGRA_Instruction
{
public:
	CGRA_Instruction();
	CGRA_Instruction(unsigned int InstructionWord);
	CGRA_Instruction(OPCode opc,bool predic,PEInputMux LMuxSel,PEInputMux RMuxSel,\
			int RRegAdd1,int RRegAdd2, int WAdd, bool WE, int ImmVal, bool EDMAdd, bool DMData);

	virtual ~CGRA_Instruction();

	void decode_CGRA_Instruction();

	OPCode getOpCode();
	bool getPredicator();
	PEInputMux getLeftMuxSelector();
	PEInputMux getRightMuxSelector();
	int getReadRegAddress1();
	int getReadRegAddress2();
	int getWriteRegAddress();
	bool getWriteRegisterEnable();
	int getImmediateValue();
	bool getSelectDataMemoryAddressBus();
	bool getSelectDataMemoryDataBus();

	void ENCODE_instruction();

	unsigned int getOpCode_DECODE();
	unsigned int getPredicator_DECODE();
	unsigned int getLeftMuxSelector_DECODE();
	unsigned int getRightMuxSelector_DECODE();
	unsigned int getReadRegAddress1_DECODE();
	unsigned int getReadRegAddress2_DECODE();
	unsigned int getWriteRegAddress_DECODE();
	unsigned int getWriteRegisterEnable_DECODE();
	unsigned int getImmediateValue_DECODE();
	unsigned int getSelectDataMemoryAddressBus_DECODE();
	unsigned int getSelectDataMemoryDataBus_DECODE();

	unsigned int DecodeInstruction(CGRA_Instruction* Ins);

private:
	OPCode opCode;
	bool Predicator;
	PEInputMux LeftMuxSelector;
	PEInputMux RightMuxSelector;
	int ReadRegAddress1;
	int ReadRegAddress2;
	int WriteRegAddress;
	bool WriteRegisterEnable;
	int ImmediateValue;
	bool SelectDataMemoryAddressBus;
	bool SelectDataMemoryDataBus;

	unsigned int InsWord;
};

class Pred_Instruction
{
public:
	Pred_Instruction();
	Pred_Instruction(unsigned int InstructionWord);
	Pred_Instruction(PredOPCode popc,PEInputMux LMuxSel,PEInputMux RMuxSel,PEInputMux PMuxSel,\
			int RRegAdd1,int RRegAdd2, int RRegAddP, int ImmVal);

	virtual ~Pred_Instruction();

	PredOPCode getPredOpCode();
	PEInputMux getLeftMuxSelector();
	PEInputMux getRightMuxSelector();
	PEInputMux getPredMuxSelector();
	int getReadRegAddress1();
	int getReadRegAddress2();
	int getReadRegAddressP();
	int getImmediateValue();

	void ENCODE_Pred_instruction();

	unsigned int getPredOpCode_DECODE();
	unsigned int getPredicator_DECODE();
	unsigned int getLeftMuxSelector_DECODE();
	unsigned int getRightMuxSelector_DECODE();
	unsigned int getPredMuxSelector_DECODE();
	unsigned int getReadRegAddress1_DECODE();
	unsigned int getReadRegAddress2_DECODE();
	unsigned int getReadRegAddressP_DECODE();
	unsigned int getImmediateValue_DECODE();

	unsigned int DecodePredInstruction(Pred_Instruction* Ins);

private:
	PredOPCode popCode;
	PEInputMux LeftMuxSelector;
	PEInputMux RightMuxSelector;
	PEInputMux PredMuxSelector;
	int ReadRegAddress1;
	int ReadRegAddress2;
	int ReadRegAddressP;
	int ImmediateValue;

	unsigned int PredInsWord;
};

#endif /* INSTRUCTION_H_ */
