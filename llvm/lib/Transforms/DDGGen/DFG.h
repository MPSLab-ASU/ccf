/*
 * DFG.h
 *
 *  Created on: Mar 8, 2013
 *  Last Modified: May 12, 2017
 *  Author: Shail Dave
 */

#include <vector>
#include <set>
#include <queue>
#include <stack>
#include <cstddef>
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "llvm/IR/Instructions.h"

#ifndef DFG_H_
#define DFG_H_

//using namespace llvm;
namespace llvm
{
	class ARC;

	enum DataDepType
	{
		TrueDep, AntiDep, OutputDep, NonDataDep, MemoryDep, BranchDep, PredDep, LiveInDataDep, LiveOutDataDep
	};

	enum Instruction_Operation
	{
		add,
		sub,
		mult,
		div,
		shiftl,
		shiftr,
		andop,
		orop,
		xorop,
		cmpSGT,
		cmpEQ,
		cmpNEQ,
		cmpSLT,
		cmpSLEQ,
		cmpSGEQ,
		cmpUGT,
		cmpULT,
		cmpULEQ,
		cmpUGEQ,
		ld_add,
		ld_data,
		st_add,
		st_data,
		ld_add_cond,
		ld_data_cond,
		loopctrl,
		//nothing,
		cond_select,
		route,
		llvm_route,
		select,
		constant,
		rem,
		sext,
		shiftr_logical,
		rest
	};

	class NODE
	{

		private:
			int cuid;
			Instruction_Operation Operation;

			std::string name;
			Value* Node_Ins; //The instruction represented by the node.
			int latency;
			std::vector<ARC*> inCommingArcs;
			std::vector<ARC*> outGoingArcs;

			NODE* related;

			unsigned load_alignment;

			bool self_loop;
			ARC* loop;

		public:
			virtual ~NODE();

			NODE(Instruction_Operation ins, int laten, int id, std::string name, Value* Node_Ins);

			//set self loop
			void set_self_loop(ARC* s_loop);

			//true if it has self loop
			bool has_self_loop();

			void set_Instruction(Instruction_Operation ins, int laten, int id, std::string name, Value* Node_Ins);
			int get_ID();
			int get_Latency();
			std::string get_Name();
			Value* get_LLVM_Instruction();
			void set_Latency(int laten);
			Instruction_Operation get_Instruction();

			void set_Load_Address_Generator(NODE* next);
			void set_Load_Data_Bus_Read(NODE* next);
			void set_Store_Address_Generator(NODE* next);
			void set_Store_Data_Bus_Write(NODE* next);
			void setAlignment(unsigned alignment);
			unsigned getAlignment();

			bool is_Load_Address_Generator();
			bool is_Load_Data_Bus_Read();
			bool is_Store_Address_Generator();
			bool is_Store_Data_Bus_Write();
			bool is_Mem_Operation();
			bool is_Routing_Operation();

			NODE* get_Related_Node();

			//get the number of incomming edges
			int get_Number_of_Pred();

			//get the number of outgoing edges
			int get_Number_of_Succ();

			//return the set of incomming edges
			std::vector<NODE*> Get_Prev_Nodes();

			//return the set of outgoing edges
			std::vector<NODE*> Get_Next_Nodes();

			//add a new incomming edge
			void add_Pred_Arc(ARC* pred_Arc);

			//add a new outgoing edge
			void add_Succ_Arc(ARC* succ_Arc);

			//remove an incomming edge
			int remove_Pred_Arc(ARC* predArc);

			//remove an outgoing edge
			int remove_Succ_Arc(ARC* succArc);

			//returns true if there is an edge between this node and next node
			bool is_Connected_To(NODE* nextNode);
	};

	class ARC
	{
		public:
			ARC(int ID, int Inter_Iteration_Distance,int operandOrder=0);
			virtual ~ARC();
		private:
			//edge from inNode to outNode
			NODE* from;
			NODE* to;
			DataDepType dependency;

			int ID;
			int Inter_Iteration_Distance;
			int operandOrder;
		public:
			//Set the node that this edge is its outgoing edge
			void Set_From_Node(NODE* inNode);
			//Set the node that this edge is its incoming edge
			void Set_To_Node(NODE* outNode);

			//Set dependency type
			void Set_Dependency_Type(DataDepType dep);

			//Get dependency type
			DataDepType Get_Dependency_Type();

			//Get the node that this edge is its outgoing edge
			NODE* get_From_Node();
			//Get the node that this edge is its incoming edge
			NODE* get_To_Node();
			//Return Edge ID
			int get_ID();
			//Return edge distance
			int Get_Inter_Iteration_Distance();
			//Return operand order
      int GetOperandOrder();
		  void SetOperandOrder(int i);
	};

	class DFG
	{
		private:
			//set of nodes in the graph
			std::vector<NODE*> _node_Set;
			//set of edges in the graph
			std::vector<ARC*> _ARC_Set;

		public:
			int CGRA_Y_Dim;

			//keep track of last edge IDs
			int ARC_Max_Index;

			//keep track of node IDs
			int Node_Max_Index;

			DFG();
			virtual ~DFG();

			//Insert a new node in the DFG
			void insert_Node(NODE *pNode);

			//return a node with given ID number
			NODE* get_Node(int number);

			//return true if DFG contains a node with given name
			NODE* get_Node(Value* ins);

			NODE* get_Node_Mem_Add(Value* ins);
			NODE* get_Node_Mem_Data(Value* ins);

			//make an edge between two nodes
			void make_Arc(NODE* pNin, NODE* pNout, int ID, int Distance, DataDepType dep,int operandOrder=0);

			//delete a node from node set
			int delete_Node(NODE *pNode);

			//remove an edge from edge set
			void Remove_Arc(ARC* arc);

			//remove the arc between two nodes and from arc set
			void Remove_Arc(NODE *pNode, NODE *nNode);

			// from the edge set, get the edge between two nodes
			ARC* get_Arc(NODE *pNode, NODE *nNode);

			//return all nodes
			std::vector<NODE*> getSetOfVertices();
			std::vector<ARC*> getSetOfArcs();

			void Dot_Print_DFG(std::string filename);

			void Dump_Loop(std::string filename);
	};

} /* namespace llvm */
#endif /* DFG_H_ */
