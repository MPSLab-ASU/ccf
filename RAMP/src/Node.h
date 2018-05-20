/*
* Node.h
*
* Created on: Jul 24, 2012
* Author: mahdi
*
* Last edited on: 22 Dec 2016
* Author: Shail Dave
*/

#ifndef NODE_H_
#define NODE_H_

#include "ARC.h"
#include "SchedulingInfo.h"
#include "RegisterFile.h"

extern std::map<Node*,vector<Node*> > constrained_store_load_pairs;
extern std::map<Node*,Node*> constrained_ld_succ_pairs;
extern std::map<Node*,Node*> constrained_st_pred_pairs;

extern std::map<int,int> route_rf_nodeid_scheduled_time;
extern std::map<int,int> node_new_inter_it_dist;

class PE;

class Node
{
  private:
    int cuid;
    Instruction_Operation insn; // instruction represented by the node.
    int latency;
    string name;

    vector<ARC*> inCommingArcs;
    vector<ARC*> outGoingArcs;

    bool Load_Output_Address_bus;
    bool Store_Output_Address_bus;
    bool Output_Data_Bus;
    bool Input_Data_Bus;

    Node* related;
    Scheduling_Info* sched_info;

    int index_cycle;
    int low_index_cycle;
    bool index_defined_cycle;

    bool self_loop;
    ARC* loop;
    bool assigned;
    PE* AssignedPE;
    bool predicated;
    bool minimizable;
    int node_height;
    bool needToLoadConstant;

  public:
    //Node();
    virtual ~Node();
    Node(Instruction_Operation ins, int laten, int id, string nodeName);

    Node(Instruction_Operation ins, int laten, int id, bool load, bool store);

    bool is_tentative_load();
    bool is_tentative_store();

    void set_self_loop(ARC* s_loop);
    bool has_self_loop();
    ARC* get_self_loop();

    void set_Instruction(Instruction_Operation ins, int laten, int id);
    int get_ID();
    int get_Latency();
    string getName();
    void set_Latency(int laten);
    Instruction_Operation get_Instruction();

    void set_Load_Address_Generator(Node* next);
    void set_Load_Data_Bus_Read(Node* next);
    void set_Store_Address_Generator(Node* next);
    void set_Store_Data_Bus_Write(Node* next);
    bool is_Load_Address_Generator();
    bool is_Load_Data_Bus_Read();
    bool is_Store_Address_Generator();
    bool is_Store_Data_Bus_Write();
    bool is_Mem_Operation();
    bool is_Address_Assert();

    bool isLargeConst(string &constName);
    void setLoadConstant(bool val);
    bool getLoadConstant();
    unsigned int  hasLargeConst=0;

    //returns true if operation is PHI (SELECT)
    bool is_PHI_Operations();

    Node* get_Related_Node();

    //get the number of incomming edges, it includes predicated inputs too
    int get_Number_of_Pred();

    //get the number of incomming edges excluding predicated edges or Memory dependency
    //(memory dependency for memory ops without  proper knowledge about their locations)
    int get_Number_of_Operands();

    //get the number of outgoing edges
    int get_Number_of_Succ();

    //return the ith incomming egde
    ARC* get_Pred_Arc(int i);

    //return ith outgoing edge
    ARC* get_Succ_Arc(int i);

    //return the set of incomming edges
    vector<Node*> Get_Prev_Nodes();

    //return predecessors with true dependency
    vector<Node*> Get_True_Dependency_Predecessors();

    //return predecessors with true dependency
    vector<Node*> Get_Predicate_Dependency_Predecessor();

    //return predecessors excluding load store address dependencies with distance = 0
    vector<Node*> Get_Predecessors_Exclude_Address_Dependency();

    //return predecessors with true or predicate dependency
    vector<Node*> Get_Explicit_Dependency_Predecessor();


    //return the set of incomming edges no distance
    vector<Node*> Get_Prev_Nodes_Same_Iteration();

    //return predecessors with distance = 0 excluding load store address dependencies with distance = 0
    vector<Node*> Get_Predecessors_Same_Iteration_Exclude_Address_Dependency();

    //return predecessors with true dependency with distance = 0
    vector<Node*> Get_True_Dependency_Predecessors_Same_Iteration();

    //return predecessors with true or predicate dependency with distance = 0
    vector<Node*> Get_Explicit_Dependency_Predecessors_Same_Iteration();

    //return the set of incomming edges with distance
    vector<Node*> Get_Prev_Nodes_Prev_Iteration();

    //return predecessors with true dependency with distance > 0
    vector<Node*> Get_True_Dependency_Predecessors_Prev_Iterations();

    //return predecessors with true or predicate dependency with distance > 0
    vector<Node*> Get_Explicit_Dependency_Predecessors_Prev_Iterations();

    //return the set of outgoing edges
    vector<Node*> Get_Next_Nodes();

    //return successors with true dependency
    vector<Node*> Get_True_Dependency_Successors();

    //return successors with true or predicate dependency
    vector<Node*> Get_Explicit_Dependency_Successors();

    //return the set of outgoing edges no distance
    vector<Node*> Get_Next_Nodes_Same_Iteration();

    //return successors with true dependency in same iteration
    vector<Node*> Get_Predicate_Dependency_Successors_Same_Iteration();

    //return successors with true dependency in same iteration
    vector<Node*> Get_True_Dependency_Successors_Same_Iteration();

    //return successors with distance = 0 excluding load store address dependencies with distance = 0
    vector<Node*> Get_Successors_Same_Iteration_Exclude_Address_Dependency();

    //return successors with true or predicate dependency
    vector<Node*> Get_Explicit_Dependency_Successors_Same_Iteration();

    //return the set of outgoing edges with distance
    vector<Node*> Get_Next_Nodes_Next_Iteration();

    //return successors with true dependency in next iteration(s)
    vector<Node*> Get_True_Dependency_Successors_Next_Iterations();

    //return successors with true ot predicate dependency in next iteration
    vector<Node*> Get_Explicit_Dependency_Successors_Next_Iterations();

    //add a new incomming edge
    void add_Pred_Arc(ARC* pred_Arc);

    //add a new outgoing edge
    void add_Succ_Arc(ARC* succ_Arc);

    //remove an incomming edge
    int remove_Pred_Arc(ARC* predArc);

    //remove an outgoing edge
    int remove_Succ_Arc(ARC* succArc);

    //returns true if there is an edge between this node and next node
    bool is_Connected_To(Node* nextNode);

    //returns true if there is a physical edge between this node and next node
    bool is_Physically_Connected_To(Node* nextNode);

    //**************************Scheduling********************************//
    Scheduling_Info* get_Sched_Info();

    bool is_ready_to_schedule_ASAP(int &time);
    bool is_ready_to_schedule_ALAP(int &time);
    bool is_ready_to_schedule_Feasible_ASAP(int &time, int II);
    bool is_ready_to_schedule_Feasible_ALAP(int &time, int MAX_Schedule, int II);
    bool is_ready_to_schedule_modulo_backward(int &time);
    void Update_ASAP_Feasible(int* add_bus, int* data_bus, int* total_resources, int number_of_resources, int mem_resources);
    void Update_ASAP_for_Phi(int II);

    //Cycle detection
    void Set_Index_Cycle(int index);
    int get_Index_Cycle();
    void Set_Low_Index_Cycle(int index);
    int get_Low_Index_Cycle();
    bool Is_Index_Defined_Cycle();
    void Reset_Cycle_Index();

    //returns true if this node is predicated
    bool isPredicated();

    //Mapping related functions
    //assign node to a PE
    void Assign2PE(PE *pe);

    //get assigned PE
    PE* getPE();

    //true if the node is not assigned to a PE
    bool isFree();

    bool isMapped();

    //Reset and un assign it
    void Reset();


    vector<Node*> get_physical_successors_constrain_by_this_modulo(int II);
    vector<Node*> get_physical_predecessors_constrain_this_modulo(int II);
    bool get_dist_physical_successors_constrain_by_this_modulo(int II, vector<int> &distance, vector<Node*> &nodes);
    bool get_dist_physical_predecessors_constrain_this_modulo(int II, vector<int> &distance, vector<Node*> &nodes);
    int get_dist(Node* node, int II);

    ARC* get_Arc(Node* node2);
    void delete_self_loop();
    int get_node_height();
    void set_node_height(int n);
};

class Node_Dummy: public Node
{
  public:
    ~Node_Dummy();
    Node_Dummy(int laten, int id);
    Node_Dummy(int laten, int id, Node* orig);
};

class PE
{
  public:
    PE();
    virtual ~PE();

    //****************************GeoS****************************//

    //set the i,j,t indicis of PE + register file len
    void SetIndex(int iIndex, int jIndex, int tIndex, int regLen);
    //get i index of PE
    int getIndexI();
    //get j index of PE
    int getIndexJ();
    //get t index of PE
    int getIndexT();

    bool SameCoordination(PE* nPE);
    //****************************GeoE****************************//

    //****************************MappingS****************************//
    //true if PE is assigned to a node
    bool isBusy();

    //assign a node to execute
    void AssignNode(Node *node);

    //use one register
    bool isRegisterAvailable();

    int NumberOfAvailableRegisters();

    unsigned int NumberOfUsedRegisters();

    void OccupyRegister();

    void AllocateRegister(int n);

    bool ReleaseRegister();

    void ReleaseRegister(int i);

    //reset the PE
    void Reset();

    //get the node assigned to PE
    Node* getNode();

    bool isAbleExecute(Node* node, int II);
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
    Node* node;

    //i,j,t indecis of PE
    int iIndex, jIndex, tIndex;

    //Register file
    REGISTER_FILE* RegFile;

    Operations ops;

    int NoMappedOps;
};

class MappingPair
{
  public:

    MappingPair(Node* inNode, PE* inPE);

    Node* getNode();
    PE* getPE();
    bool SameCoordinationPE(MappingPair* mPair);

    bool Match(MappingPair* mPair);

    bool is_Address_Assert();

  private:
    Node* node;
    PE* pe;

};

class TimeCGRA
{
  public:

    TimeCGRA(int SizeX, int SizeY, int RegSize, int time, ConnectionType ConType);
    virtual ~TimeCGRA();
    void Reset();

    PE* getPE(int i, int j);

  private:
    PE **PEs;
    int SizeX;
    int SizeY;
    int time;
    ConnectionType ConType;
};

#endif /* NODE_H_ */
