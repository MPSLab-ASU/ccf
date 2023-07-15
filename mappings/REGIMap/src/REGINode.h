/*
 * REGINode.h
 *
 *  Created on: Jul 24, 2012
 *      Author: mahdi
 */

#ifndef REGI_NODE_H_
#define REGI_NODE_H_



#include "REGIARC.h"
#include "REGISchedulingInfo.h"
#include "REGIREGISTERFILE.h"

//class REGI_ARC;
class REGI_PE;
//class REGI_Node;

class REGI_Node
{

    private:
        int cuid;
        /* The insn represented by the node.  */
        Instruction_Operation insn;

        int latency;

        vector<REGI_ARC*> inCommingArcs;
        vector<REGI_ARC*> outGoingArcs;

        bool tent_load, tent_store;

        bool Load_Output_Address_bus;
        bool Store_Output_Address_bus;
        bool Output_Data_Bus;
        bool Input_Data_Bus;

        REGI_Node* related;

        REGI_Scheduling_Info* sched_info;

        //**************************CycleS*************************************//
        int index_cycle;
        int low_index_cycle;
        bool index_defined_cycle;
        //**************************CycleE*************************************//

        bool self_loop;
        REGI_ARC* loop;

        //**************************MappingS*************************************//
        //if node is assigned to a PE, it is true
        bool assigned;

        //assigned PE
        REGI_PE* AssignedPE;
        //**************************MappingE*************************************//
        //**************************Predication*************************************//
        bool predicated;
        bool predicating;
        //**************************Predication*************************************//

        //**************************MinimizationS*************************************//
        bool minimizable;

        //**************************MinimizationE*************************************//

    public:
        //REGI_Node();
        virtual ~REGI_Node();
        REGI_Node(Instruction_Operation ins, int laten, int id);

        REGI_Node(Instruction_Operation ins, int laten, int id, bool load, bool store);

        bool is_tentative_load();
        bool is_tentative_store();

        void set_self_loop(REGI_ARC* s_loop);
        bool has_self_loop();
        REGI_ARC* get_self_loop();

        void set_Instruction(Instruction_Operation ins, int laten, int id);
        int get_ID();
        int get_Latency();
        void set_Latency(int laten);
        Instruction_Operation get_Instruction();

        void set_Load_Address_Generator(REGI_Node* next);
        void set_Load_Data_Bus_Read(REGI_Node* next);
        void set_Store_Address_Generator(REGI_Node* next);
        void set_Store_Data_Bus_Write(REGI_Node* next);
        bool is_Load_Address_Generator();
        bool is_Load_Data_Bus_Read();
        bool is_Store_Address_Generator();
        bool is_Store_Data_Bus_Write();
        bool is_Mem_Operation();

        //returns true if operation is PHI (SELECT)
        bool is_PHI_Operations();

        bool is_Minimizable();
        void set_Minimizable();
        void Reset_Minimizable();

        REGI_Node* get_Related_Node();

        //get the number of incomming edges, it includes predicated inputs too
        int get_Number_of_Pred();

        //get the number of incomming edges excluding predicated edges or Memory dependency
        //(memory dependency for memory ops without  proper knowledge about their locations)
        int get_Number_of_Operands();

        //get the number of outgoing edges
        int get_Number_of_Succ();

        //return the ith incomming egde
        REGI_ARC* get_Pred_Arc(int i);

        //return ith outgoing edge
        REGI_ARC* get_Succ_Arc(int i);

        //return the set of incomming edges
        vector<REGI_Node*> Get_Prev_Nodes();

       //return predecessors with true dependency
        vector<REGI_Node*> Get_True_Dependency_Predecessors();

        //return predecessors with true dependency
        vector<REGI_Node*> Get_Predicate_Dependency_Predecessor();

        //return predecessors excluding load store address dependencies with distance = 0 
        vector<REGI_Node*> Get_Predecessors_Exclude_Address_Dependency();

        //return predecessors with true or predicate dependency
        vector<REGI_Node*> Get_Explicit_Dependency_Predecessor();


        //return the set of incomming edges no distance
        vector<REGI_Node*> Get_Prev_Nodes_Same_Iteration();

        //return predecessors with distance = 0 excluding load store address dependencies with distance = 0 
        vector<REGI_Node*> Get_Predecessors_Same_Iteration_Exclude_Address_Dependency();

        //return predecessors with true dependency with distance = 0 
        vector<REGI_Node*> Get_True_Dependency_Predecessors_Same_Iteration();

        //return predecessors with true or predicate dependency with distance = 0
        vector<REGI_Node*> Get_Explicit_Dependency_Predecessors_Same_Iteration();

        //return the set of incomming edges with distance
        vector<REGI_Node*> Get_Prev_Nodes_Prev_Iteration();

        //return predecessors with true dependency with distance > 0
        vector<REGI_Node*> Get_True_Dependency_Predecessors_Prev_Iterations();

        //return predecessors with true or predicate dependency with distance > 0
        vector<REGI_Node*> Get_Explicit_Dependency_Predecessors_Prev_Iterations();
        
        //return the set of outgoing edges
        vector<REGI_Node*> Get_Next_Nodes();

        //return successors with true dependency
        vector<REGI_Node*> Get_True_Dependency_Successors();

        //return successors with true or predicate dependency
        vector<REGI_Node*> Get_Explicit_Dependency_Successors();

        //return the set of outgoing edges no distance
        vector<REGI_Node*> Get_Next_Nodes_Same_Iteration();

        //return successors with true dependency in same iteration
        vector<REGI_Node*> Get_Predicate_Dependency_Successors_Same_Iteration();

        //return successors with true dependency in same iteration
        vector<REGI_Node*> Get_True_Dependency_Successors_Same_Iteration();

        //return successors with distance = 0 excluding load store address dependencies with distance = 0 
        vector<REGI_Node*> Get_Successors_Same_Iteration_Exclude_Address_Dependency();


        //return successors with true or predicate dependency
        vector<REGI_Node*> Get_Explicit_Dependency_Successors_Same_Iteration();

        //return the set of outgoing edges with distance
        vector<REGI_Node*> Get_Next_Nodes_Next_Iteration();

        //return successors with true dependency in next iteration(s)
        vector<REGI_Node*> Get_True_Dependency_Successors_Next_Iterations();

        //return successors with true ot predicate dependency in next iteration
        vector<REGI_Node*> Get_Explicit_Dependency_Successors_Next_Iterations();

        //add a new incomming edge
        void add_Pred_Arc(REGI_ARC* pred_Arc);

        //add a new outgoing edge
        void add_Succ_Arc(REGI_ARC* succ_Arc);

        //remove an incomming edge
        int remove_Pred_Arc(REGI_ARC* predArc);

        //remove an outgoing edge
        int remove_Succ_Arc(REGI_ARC* succArc);

        //returns true if there is an edge between this node and next node
        bool is_Connected_To(REGI_Node* nextNode);

        //returns true if there is a physical edge between this node and next node
        bool is_Physically_Connected_To(REGI_Node* nextNode);


        //**************************Scheduling********************************//
        REGI_Scheduling_Info* get_Sched_Info();

        bool is_ready_to_schedule_ASAP(int &time);

        bool is_ready_to_schedule_ALAP(int &time);

        bool is_ready_to_schedule_Feasible_ASAP(int &time);

        bool is_ready_to_schedule_Feasible_ALAP(int &time, int MAX_Schedule);

        bool is_ready_to_schedule_modulo(int &time);

        bool is_ready_to_schedule_modulo_forward(int &time);

        bool is_ready_to_schedule_modulo_backward(int &time);

        //if a node has dependency to more than one node scheduled at time < time-1
        bool has_multiple_constrains_modulo_at_time(int time, vector<REGI_Node*> &constrainers);

        bool has_multiple_constrains_modulo_backward_at_time(int time, vector<REGI_Node*> &constrainers);

        bool constrains_multiple_modulo_backward_at_time(int time, vector<REGI_Node*> &constrainers);

        //**************************Scheduling********************************//

        //**************************cycle DetectionS********************************//
        void Set_Index_Cycle(int index);

        int get_Index_Cycle();

        void Set_Low_Index_Cycle(int index);

        int get_Low_Index_Cycle();

        bool Is_Index_Defined_Cycle();

        void Reset_Cycle_Index();
        //**************************cycle DetectionE********************************//
        //**************************Predication*************************************//
        //returns true if this node is predicated
        bool isPredicated();
        //returns true if this node is predicating
        bool isPredicating();
        //**************************Predication*************************************//

        //**************************MappingS*************************************//

        //assign node to a PE
        void Assign2PE(REGI_PE *pe);

        //get assigned PE
        REGI_PE* getPE();

        //true if the node is not assigned to a PE
        bool isFree();

        bool isMapped();

        //Reset and un assign it
        void Reset();

        //void SetRequiredRegister(int num);

        //int GetRequiredRegister();

        vector<REGI_Node*> get_prev_operations_constrain_this_modulo();

        vector<REGI_Node*> get_next_operations_constrain_by_this_modulo();
        
        vector<REGI_Node*> get_physical_successors_constrain_by_this_modulo();
        
        vector<REGI_Node*> get_physical_predecessors_constrain_this_modulo();

        //**************************MappingE*************************************//

};

class REGI_Node_Dummy: public REGI_Node
{
    public:
        ~REGI_Node_Dummy();
        REGI_Node_Dummy(int laten, int id);
        REGI_Node_Dummy(int laten, int id, REGI_Node* orig);
};

class REGI_Reduced_Node: public REGI_Node
{

    public:
        REGI_Reduced_Node(int id, REGI_Node* left,REGI_Node* right, REGI_Node* phi, vector<REGI_ARC*>&to_remove, vector<REGI_ARC*>&to_add);
        REGI_Reduced_Node(int id, REGI_Node* left,REGI_Node* right, REGI_Node* share, vector<REGI_ARC*>&to_remove, vector<REGI_ARC*>&to_add, bool dummy);
        ~REGI_Reduced_Node();
    private:
        Reduced_Node_Type type;
        REGI_Node* left;
        REGI_Node* right;
        REGI_Node* phi;
    public:
        vector<REGI_Node*> Get_Left_Inputs_Sorted_by_Arc();
        vector<REGI_Node*> Get_Right_Inputs_Sorted_by_Arc();


};
class REGI_PE
{
    public:
        REGI_PE();
        virtual ~REGI_PE();

        //****************************GeoS****************************//

        //set the i,j,t indicis of PE + register file len
        void SetIndex(int iIndex, int jIndex, int tIndex, int regLen);
        //get i index of PE
        int getIndexI();
        //get j index of PE
        int getIndexJ();
        //get t index of PE
        int getIndexT();

        bool SameCoordination(REGI_PE* nPE);
        //****************************GeoE****************************//

        //****************************MappingS****************************//
        //true if PE is assigned to a node
        bool isBusy();

        //assign a node to execute
        void AssignNode(REGI_Node *node);

        //use one register
        bool isRegisterAvailable();

        int NumberOfAvailableRegisters();

        int NumberOfUsedRegisters();

        void OccupyRegister();

        void AllocateRegister(int n);

        bool ReleaseRegister();

        void ReleaseRegister(int i);

        //reset the PE
        void Reset();

        //get the node assigned to PE
        REGI_Node* getNode();

        bool isAbleExecute(REGI_Node* node, int II);
        //****************************MappingE****************************//

        //****************************Simulated Annealing****************************//
        bool isOverUsed();

        int getOverUsedNumber();

        //assign a node to execute
        void ReleasePE();

        //****************************Simulated Annealing****************************//

    private:
        //****************************MappingS****************************//
        //true when it is assigned to a node
        bool busy;
        //the node executed in PE
        REGI_Node* node;
        //****************************MappingE****************************//

        //****************************GeoS****************************//
        //i,j,t indecis of PE
        int iIndex, jIndex, tIndex;
        //****************************GeoE****************************//

        //****************************RegisterFileS****************************//
        //Register file
        REGI_REGISTER_FILE* RegFile;

        REGI_Operations ops;

        //****************************RegisterFileE****************************//

        //****************************Simulated Annealing****************************//
        int NoMappedOps;
        //****************************Simulated Annealing****************************//

};

class MappingPair
{
    public:

        MappingPair(REGI_Node* inNode, REGI_PE* inPE);

        REGI_Node* getNode();
        REGI_PE* getPE();
        bool SameCoordinationPE(MappingPair* mPair);

        bool Match(MappingPair* mPair);

    private:
        REGI_Node* node;
        REGI_PE* pe;

};

class TimeCGRA
{
    public:

        TimeCGRA(int SizeX, int SizeY, int RegSize, int time, REGI_ConnectionType ConType);
        virtual ~TimeCGRA();
        void Reset();

        REGI_PE* getPE(int i, int j);

    private:
        REGI_PE **PEs;
        int SizeX;
        int SizeY;
        int time;
        REGI_ConnectionType ConType;
};

#endif /* REGI_NODE_H_ */
