/*
 * REGIDFG.h
 *
 *  Created on: Jul 24, 2012
 *  Author: Mahdi Hamzeh
 */

#ifndef REGIDFG_H_
#define REGIDFG_H_

#include "REGINode.h"
class REGI_DFG
{
  private:
    //set of nodes in the graph
    vector<REGI_Node*> _node_Set;
    vector<REGI_Node*> _node_Set_Ordered;
    vector<REGI_Node*> _constants;
    //set of edges in the graph
    vector<REGI_ARC*> _ARC_Set;

    vector<REGI_ARC*> _Back_ARC_Set;

    int MAX_SCHEDULE_LEN;

    int schedule_len;

  public:
    int CGRA_X_Dim;
    int CGRA_Y_Dim;

    //keep track of last edge IDs
    int ARC_Max_Index;

    //keep track of node IDs
    int Node_Max_Index;

    REGI_DFG(int x_dim, int y_dim);
    virtual ~REGI_DFG();

    REGI_DFG* Copy();

    //*************************** utilitiesS*********************//

    //return true if it is a dummy node
    bool is_Dummy_Node(REGI_Node *current);
    //return true if it is a reduced node
    bool is_Reduced_Node(REGI_Node *current);
    //reset minimized property of all nodes.
    void Reset_Minimized_Nodes();

    //Insert a new node in the DFG
    void insert_Node(REGI_Node *pNode);

    void insert_Constant(REGI_Node *pNode);
    //insert a node between pnode and nnode, the distance will go to the arc between pNode and newNode
    bool insert_Node_in_between_input(REGI_Node* pNode, REGI_Node* nNode, REGI_Node* newNode);

    //insert a node between pnode and nnode the distance will go to the arc between newNode and nNode
    bool insert_Node_in_between_output(REGI_Node* pNode, REGI_Node* nNode, REGI_Node* newNode);

    //returns true if DFG has a node with the given ID
    bool has_Node(int number);
    //returns true if DFG has a constant with the given ID
    bool has_Constant(int number);
    //return a node with given ID number
    REGI_Node* get_Node(int number);

    //make an edge between two nodes
    void make_Arc(REGI_Node* pNin, REGI_Node* pNout, int ID, int Distance, DataDepType dep);

    //true if given set contains given node
    bool contains(vector<REGI_Node*> &nodeset, REGI_Node* node);

    //true if two given sets have any node in commmon
    bool Not_Empty_Intersect(vector<REGI_Node*> &nodeset1, vector<REGI_Node*> &nodeset2);

    //bool contains(vector<int> &set, int value);

    //remove a node from given set
    void Remove(vector<REGI_Node*> &nodeset, REGI_Node* node);

    void Remove(vector<REGI_Node*> &nodeset, vector<REGI_Node*> &toRemoveNodes);

    //delete a node from node set
    int delete_Node(REGI_Node *pNode);

    //remove and edge from edge set
    void Remove_Arc(REGI_ARC* arc);

    //add an edge to edge set
    void Add_Arc(REGI_ARC* arc);

    //add a set of edges to edge set
    void Add_Arcs(vector<REGI_ARC*> &arc);

    //remove the arc between two nodes and from arc set
    void Remove_Arc(REGI_Node *pNode, REGI_Node *nNode);

    //get the edge between two nodes and from edge set
    REGI_ARC* get_Arc(REGI_Node *pNode, REGI_Node *nNode);

    //remove a set of edges
    void Remove_Arcs(vector<REGI_ARC*> &arcs);

    //intersect two sets
    vector<REGI_Node*> Intersect(vector<REGI_Node*> &nodeset1, vector<REGI_Node*> &nodeset2);

    //union two sets
    vector<REGI_Node*> Union(vector<REGI_Node*> &nodeset1, vector<REGI_Node*> &nodeset2);

    //subtracts set1-set2
    vector<REGI_Node*> Subtract(vector<REGI_Node*> &set1, vector<REGI_Node*> &set2);

    //topological sort DFG
    queue<REGI_Node*> Topological_Sort();
    void Strongly_Connected(vector<vector<REGI_Node*> > &results);

    void Strongly_Connected(REGI_Node* currentNode, stack<REGI_Node*> &S, vector<REGI_Node*> &SCopy, int &index, vector<vector<REGI_Node*> > &results);
    //generate a unique ID for nodes
    int Get_Unique_Index();

    //return all nodes
    vector<REGI_Node*> getSetOfVertices();

    //return all nodes
    vector<REGI_ARC*> getSetOfArcs();

    void Dot_Print_DFG_With_Schedule(string filename, int ID);

    void Dot_Print_DFG_With_Modulo_Schedule(string filename, int II);

    void Dot_Print_DFG(string filename, int ID);

    void Dump_Nodes(std::string filename);

    void Dump_Edges(std::string filename);


    //print elements of a set
    void PrintSet(vector<REGI_Node*> &set);

    //print elements of a set of sets of nodes
    void PrintSets(vector<vector<REGI_Node*> > &sets);

    //return number of nodes
    int get_number_of_nodes();

    //return number of load operations
    int Count_Load_Operations();
    //return number of load operations
    int Count_Store_Operations();

    //get set of next nodes of given nodes that belong to this iteration
    vector<REGI_Node*> get_next_nodes_same_iteration(vector<REGI_Node*> &nodes);
    //get set of next nodes of given nodes that belong to this iteration and update their latency in a loop (SCC)
    vector<REGI_Node*> get_next_nodes_same_iteration_update_latency(vector<REGI_Node*> &nodes, bool &change);

    //reset all nodes latency in the loop (SCC) in the given set
    void reset_latency_to_nodes(vector<REGI_Node*> &nodes);

    //set of nodes which do not have incoming arcs or they have incoming arcs from previous iterations
    vector<REGI_Node*> get_set_of_start_nodes();

    //set of nodes which do not have incoming arcs or they have incoming arcs from previous iterations,
    //load and store operations with special condition can be in
    vector<REGI_Node*> get_set_of_start_nodes_constraint_scheduling();

    //set of nodes which do not have outgoing arcs or they have outgoing arcs got to next iterations
    vector<REGI_Node*> get_set_of_end_nodes();

    //set of nodes which do not have outgoing arc or they have outgoing arcs got to next iterations
    //load and store operations with special condition can be in
    vector<REGI_Node*> get_set_of_end_nodes_constraint_scheduling();

    void Apply_Indegree_Constraint();

    void Apply_Outdegree_Constraint();

    void Apply_Outdegree_Inter_Iteration_Constraint();

    //*************************** utilitiesS*********************//

    //*************************** Scheduling*********************//
    void PreProcess();

    int Schedule(int id, int number_of_resources, int II);

    int Init_Schedule(int id, int number_of_resources);

    int Modulo_ReSchedule_Backward(int id, int number_of_resources, int II, vector<REGI_Node*> &to_change_set);

    int Modulo_ReSchedule_Forward(int id, int number_of_resources, int II, vector<REGI_Node*> &to_change_set);
    int Modulo_ReSchedule_Smart(int id, int number_of_resources, int II, vector<REGI_Node*> &to_change_set);

    int Modulo_ReSchedule_Smart_2(int id, int number_of_resources, int II, vector<REGI_Node*> &to_change_set);

    int RecMII_Scc(vector<vector<REGI_Node*> > &sets, int &max_index);
    int Longest_Path(vector<REGI_Node*> &nodes, int &II);

    int Get_Schedule_Length();

    void Set_Schedule_Length(int len);

    //ASAP schedule all nodes, returns last node execution time
    int Schedule_ASAP();
    //Feasible ASAP Schedule
    int Schedule_ASAP_Feasible(int number_of_resources);

    //ALAP schedule all nodes
    void Schedule_ALAP(int latest_time);
    //Feasible ASAP Schedule

    bool Schedule_ALAP_Feasible(int latest_time, int number_of_resources);

    //modulo schedule nodes
    bool Modulo_Schedule_Forward(int MII, int Last_time, bool &multiple_constraint, int number_of_resources);

    bool Modulo_Schedule_Backward(int MII, int Last_time, bool &multiple_constraint, int number_of_resources);

    bool Modulo_Schedule_Smart(int MII, int Last_time, bool &multiple_constraint, int number_of_resources);

    bool Modulo_Schedule_Smart_2(int MII, int Last_time, bool &multiple_constraint, int number_of_resources);

    //schedule a load address operation, checking resources, load data, etc. If succeeds, return true, else false
    //if there are multiple constrained for the nodes or thy constrain multiple nodes, set multiple_constraint
    bool Schedule_Load_Address(REGI_Node* selected_node, int schedule_time, int II, bool &multiple_constraint, int* address_bus, int *data_bus, int* PE_resources, int max_resources, vector<REGI_Node*> &scheduled_nodes);

    bool Schedule_Load_Data(REGI_Node* selected_node, int schedule_time, int II, bool &multiple_constraint, int* address_bus, int* data_bus, int* PE_resources, int max_resources, vector<REGI_Node*> &scheduled_nodes);

    bool Schedule_Store_Address_Data(REGI_Node* selected_node, int schedule_time, int II, bool &multiple_constraint, int* address_bus, int* data_bus, int* PE_resources, int max_resources, vector<REGI_Node*> &scheduled_nodes);

    //reset all nodes ASAP schedule time
    void Reset_ASAP_Schedule();

    //reset all nodes ALAP schedule time
    void Reset_ALAP_Schedule();

    //reset all nodes Feasible ASAP schedule time
    void Reset_ASAP_Feasible_Schedule();

    //reset all nodes Feasible ALAP schedule time
    void Reset_ALAP_Feasible_Schedule();

    //reset all nodes Current schedule time
    void Reset_Current_Schedule();

    //get set of nodes to be scheduled sorted by their slack, lowest first
    //		vector<REGI_Node*> get_sorted_ready_list_modulo_forward(vector<REGI_Node*> list);

    vector<REGI_Node*> get_sorted_ready_list_modulo_backward(vector<REGI_Node*> list);

    /*
       bool Prev_Node_Other_Iterations_Feasible_ASAP_Scheduled_at_Time(REGI_Node* current_node, int current_time);

       bool Next_Node_Other_Iterations_Feasible_ALAP_Scheduled_at_Time(REGI_Node* current_node, int current_time);

       bool Prev_Node_Other_Iterations_Feasible_Scheduled_at_Time(REGI_Node* current_node, int current_time);
       */

    vector<REGI_Node*> get_Current_Scheduled_Nodes(vector<REGI_Node*> inSet);

    //returns all nodes connected with this node must be placed to the same physical PE
    vector<REGI_Node*> get_All_Nodes_Constrained_by_Current(REGI_Node* current_node, int time);

    //returns all nodes connected with this node must be placed to the same physical PE
    vector<REGI_Node*> get_Intra_Iteration_Related_Nodes_Current(REGI_Node* current_node, int time);

    //returns all nodes connected connected to this node with inter-iteration dependency
    vector<REGI_Node*> get_Inter_Iteration_Related_Nodes(REGI_Node* current_node);

    /*
     * changes on July 14, 2013
     */

    bool has_node_conflict_with_scheduled_nodes(REGI_Node* current, int schedule_time, int modulo_schedule, vector<REGI_Node*> &scheduled_nodes, REGI_Node* &node_in_conflict_path);

    bool has_node_forward_conflict_with_scheduled_nodes(REGI_Node* current, int schedule_time, int modulo_schedule, vector<REGI_Node*> &scheduled_nodes, REGI_Node* &first_node_in_conflict_path);

    bool has_node_backward_conflict_with_scheduled_nodes(REGI_Node* current, int schedule_time, int modulo_schedule, vector<REGI_Node*> &scheduled_nodes, REGI_Node* &last_node_in_conflict_path);

    bool has_forward_conflict(REGI_Node* src, int scr_schedule_cycle, int src_modulo_cycle, REGI_Node* target, int target_schedule_cycle, int target_modulo_cycle, REGI_Node* &first_node_in_conflict_path);

    bool has_backward_conflict(REGI_Node* src, int scr_schedule_cycle, int src_modulo_cycle, REGI_Node* target, int target_schedule_cycle, int target_modulo_cycle, REGI_Node* &first_node_in_conflict_path);

    bool traverse_constrain_path_recursive(REGI_Node* src, REGI_Node* target, vector<REGI_Node*> &visited);

    bool has_multiple_constrains(REGI_Node* current, int schedule_time, REGI_Node* &last_node_in_constrain_path, vector<REGI_Node*> &scheduled_nodes);

    bool constrains_multiple(REGI_Node* current, int schedule_time, REGI_Node* &first_node_in_constrain_path, vector<REGI_Node*> &scheduled_nodes);

    bool constrains(REGI_Node* src, int scr_schedule_cycle, REGI_Node* target, int target_schedule_cycle, int target_modulo_cycle, REGI_Node* &first_node_in_conflict_path);

    bool is_constrained(REGI_Node* src, int scr_schedule_cycle, int src_modulo_cycle, REGI_Node* target, int target_schedule_cycle, REGI_Node* &last_node_in_conflict_path);

    /*
     * changes on July 14, 2013
     */

    bool has_Node_Conflict_Feasible_ASAP(REGI_Node* current_node, int time);
    bool has_Node_Conflict_Feasible_ALAP(REGI_Node* current_node, int time);

    //bool has_Node_Conflict_Current_old(REGI_Node* current_node, int time, int II);

    //		bool has_Node_Conflict_Current_new(REGI_Node* current_node, int time, int II);

    //		bool fix_Node_Conflict_Current(REGI_Node* current_node, int time, int II);

    //		bool cause_Node_Conflict_Current(REGI_Node* current_node, int time, int II);

    //		REGI_Node* get_Node_Conflict_Current(REGI_Node* current_node, int time, int II);

    int get_Modulo_Next_Cycle(int time, int II);
    int get_Modulo_Prev_Cycle(int time, int II);

    //void Update_Required_Registers(int II);

    //update the number of required registers between node1 and node2
    int get_Required_Register(REGI_Node* node1, REGI_Node* node2, int II);
    //*************************** Scheduling*********************//

    //*************************** Simulated Annealing *********************//

    void Set_Height();

    vector<REGI_Node*> Simulated_Annealing_SortOps(int &MII, int number_of_resources);

    //all SCCs, not necessarily seperated sets
    void Sort_Sccs(vector<vector<REGI_Node*> > &sets);

    //all SCCs, not necessarily seperated sets
    void Sort_Ops_by_Depth(vector<REGI_Node*> &set);

    //gets nodes in path between existing SCCs and the new SCC
    vector<REGI_Node*> get_path_between(vector<vector<REGI_Node*> > &visited, vector<REGI_Node*> &to_visit);

    //nodes in the same iteration
    vector<REGI_Node*> get_all_predecessors(REGI_Node* &current);

    //nodes in the same iteration
    vector<REGI_Node*> get_all_successors(REGI_Node* &current);

    vector<REGI_Node*> order_nodes_SCC(vector<REGI_Node*> &scc);

    int Simulated_Annealing_MII();

    //*************************** Simulated Annealing *********************//

    //*************************** DFG Minimization *********************//

    void Update_Minimizable_Nodes();

    void Reduce_DFG();

    //*************************** DFG Minimization *********************//
};

#define HAS_ENOUGH_RESOURCES_FOR_LOAD_INSTRUCTION_AT_CYCLE(TIME1, TIME2, ADDRESS_BUS, DATA_BUS, PE_RESOURCES, MAX_RESOURCES) \
  ((ADDRESS_BUS[TIME1] < cgra_info.PER_ROW_MEM_AVAILABLE * CGRA_Y_Dim) && (DATA_BUS[TIME2] < cgra_info.PER_ROW_MEM_AVAILABLE * CGRA_Y_Dim) && (PE_RESOURCES[TIME1] < MAX_RESOURCES) && (PE_RESOURCES[TIME2] < MAX_RESOURCES))

#define HAS_ENOUGH_RESOURCES_FOR_STORE_INSTRUCTION_AT_CYCLE(TIME1, ADDRESS_BUS, DATA_BUS,PE_RESOURCES, MAX_RESOURCES) \
  ((ADDRESS_BUS[TIME1] < cgra_info.PER_ROW_MEM_AVAILABLE * CGRA_Y_Dim) && (DATA_BUS[TIME1] < cgra_info.PER_ROW_MEM_AVAILABLE * CGRA_Y_Dim) && (PE_RESOURCES[TIME1] < (MAX_RESOURCES) - 1))

#define HAS_ENOUGH_RESOURCES_FOR_REGULAR_INSTRUCTION_AT_CYCLE(TIME, RESOURCES, MAX_RESOURCES) \
  (REOSURCES[TIME] < MAX_RESOURCES)

#endif /* REGIDFG_H_ */
