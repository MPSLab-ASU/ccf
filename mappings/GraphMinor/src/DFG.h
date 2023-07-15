/*
 * DFG.h
 *
 * Created on: Jul 24, 2012
 * Author: Mahdi Hamzeh

 * Last edited on: 3 December 2016
 * Author: Shail Dave
 */

#ifndef DFG_H_
#define DFG_H_

#include "Node.h"
#include <tuple>

class DFG
{
  private:
    //set of nodes in the graph
    vector<Node*> _node_Set;
    vector<Node*> _node_Set_Ordered;
    vector<Node*> _constants;
    //set of edges in the graph
    vector<ARC*> _ARC_Set;
    vector<ARC*> _Back_ARC_Set;
    vector<std::tuple<int,int,int> > _Const_ARC_Set;
    vector<Node*> Sorted_Ops; 

    // adjacency list for inverse BFs
    list<int> *adjacency_list;

    int MAX_SCHEDULE_LEN;
    int schedule_len;
    int nodeSize;
    //vector<Node*> ready_sorted; 
    //vector<Node*> scheduled;
    vector<Node*> initialized;
    //vector<vector<Node*>> Sccs; 
    list<int>* schedule_list;  

    bool mem_mode;
    int *Total_Resources;
    int *Address_BUS;
    int *Data_BUS;
    
    bool needToLoadConstants;

  public:
    int CGRA_X_Dim;
    int CGRA_Y_Dim;

    //keep track of last edge IDs
    int ARC_Max_Index;

    //keep track of node IDs
    int Node_Max_Index;

    DFG(int x_dim, int y_dim);
    virtual ~DFG();

    DFG* Copy();
    DFG* Copy_With_Schedule_And_Mapping();
    void Copy_Resource_Info(int* totalResources, int* addBus, int* dataBus, int maxSchedLen);

    //return true if it is a reduced node
    bool is_Reduced_Node(Node *current);


    //Insert a new node in the DFG
    void insert_Node(Node *pNode);

    void insert_Constant(Node *pNode);
    //insert a node between pnode and nnode, the distance will go to the arc between pNode and newNode
    bool insert_Node_in_between_input(Node* pNode, Node* nNode, Node* newNode);

    //insert a node between pnode and nnode the distance will go to the arc between newNode and nNode
    bool insert_Node_in_between_output(Node* pNode, Node* nNode, Node* newNode);

    //returns true if DFG has a node with the given ID
    bool has_Node(int number);
    //returns true if DFG has a constant with the given ID
    bool has_Constant(int number);
    // returns if the constant list has a floating point constant.
    bool has_FPConstant();
    //return a node with given ID number
    Node* get_Node(int number);
    // Modified to get the node from the node set.
    Node* get_Node_from_Set(int number);

    //make an edge between two nodes
    void make_Arc(Node* pNin, Node* pNout, int ID, int Distance, DataDepType dep, int oporder);

    void make_Const_Arc(int pNin, int pNout, int oporder);

    //true if given set contains given node
    bool contains(vector<Node*> &nodeset, Node* node);

    //remove a node from given set
    void Remove(vector<Node*> &nodeset, Node* node);
    void Remove(vector<Node*> &nodeset, vector<Node*> &toRemoveNodes);

    void Remove_Repeat_ARCs(vector<ARC*>& arcset);
    vector<ARC*> Remove_ARCs(); 
    void Remove_Redundant_Arcs(); 
    //delete a node from node set
    int delete_Node(Node *pNode);

    //remove and edge from edge set
    void Remove_Arc(ARC* arc);
    //add an edge to edge set
    void Add_Arc(ARC* arc);
    //remove the arc between two nodes and from arc set
    void Remove_Arc(Node *pNode, Node *nNode);
    //get the edge between two nodes and from edge set
    ARC* get_Arc(Node *pNode, Node *nNode);

    //intersect two sets
    vector<Node*> Intersect(vector<Node*> &nodeset1, vector<Node*> &nodeset2);
    //union two sets
    vector<Node*> Union(vector<Node*> &nodeset1, vector<Node*> &nodeset2);
    //subtracts set1-set2
    vector<Node*> Subtract(vector<Node*> &set1, vector<Node*> &set2);
    void Subtract1(vector<Node*> &set1, vector<Node*> &set2);

    void Strongly_Connected(vector<vector<Node*> > &results);
    void Strongly_Connected(Node* currentNode, stack<Node*> &S, vector<Node*> &SCopy, int &index, vector<vector<Node*> > &results);
    //vector<vector<Node*>> get_Sccs();
    //generate a unique ID for nodes
    int Get_Unique_Index();

    int get_Unique_ID(); 
    //return all nodes
    vector<Node*> getSetOfVertices();
    //return all arcs
    vector<ARC*> getSetOfArcs();

    // Print DFG
    void Dot_Print_DFG(string filename, int ID);

    // Print DFG with Schedule
    void Dot_Print_DFG_With_Schedule(vector<Node*> nodes, string filename, int ID, int unsched_ID);

    // Print DFG with Modulo Schedule
    void Dot_Print_DFG_With_Modulo_Schedule(string filename, int II); 

    void Dot_Print_DFG_With_Modulo_Schedule(vector<Node*> nodes, string filename, int II, int unsched_ID);

    // Dump final nodes into a file
    void Dump_Nodes(std::string filename);

    // Dump final edges into a file
    void Dump_Edges(std::string filename);

    //return number of nodes
    int get_number_of_nodes();

    //return number of load operations
    int Count_Load_Operations();
    //return number of load operations
    int Count_Store_Operations();

    //get set of next nodes of given nodes that belong to this iteration
    vector<Node*> get_next_nodes_same_iteration(vector<Node*> &nodes);
    //get set of next nodes of given nodes that belong to this iteration and update their latency in a loop (SCC)
    vector<Node*> get_next_nodes_same_iteration_update_latency(vector<Node*> &nodes, bool &change);

    //reset all nodes latency in the loop (SCC) in the given set
    void reset_latency_to_nodes(vector<Node*> &nodes);

    // set of nodes with incoming arcs as start_nodes
    vector<Node*> get_set_of_level2_nodes(vector<Node*> &start_node);
    //set of nodes which do not have incoming arcs or they have incoming arcs from previous iterations
    vector<Node*> get_set_of_start_nodes();
    //set of nodes which do not have incoming arcs or they have incoming arcs from previous iterations,
    //load and store operations with special condition can be in
    vector<Node*> get_set_of_start_nodes_constraint_scheduling();
    //set of nodes which do not have outgoing arcs or they have outgoing arcs got to next iterations
    vector<Node*> get_set_of_end_nodes();
    //set of nodes which do not have outgoing arc or they have outgoing arcs got to next iterations
    //load and store operations with special condition can be in
    vector<Node*> get_set_of_end_nodes_constraint_scheduling();

    void loadAllLargeConstants(unsigned int rfSize);
    void loadAllFPConstants();
    bool getLoadConstants();
    void setLoadConstants(bool val);
    void collectRegReq(unsigned int rfSize);

    void GenerateAdjacency();
    bool connected_Nodes(Node *pNode, Node *nNode);
    void print_adj(int nodeSize);
    void DFSUtil(int v, bool visited[], int II, bool &multiple_constraint, int number_of_resources);
    void DFSUtil(int v, bool visited[], int II, bool &multiple_constraint, int number_of_resources, int ASAP, int ALAP, int sched_time); 
    void DFS(int v, int V, bool visited[], int II, bool &multiple_constraint, int number_of_resources);
    bool contains_list(list<int> adjacency_list, int node); 


    void Apply_Indegree_Constraint();
    void Apply_Outdegree_Constraint();
    void Apply_Outdegree_Inter_Iteration_Constraint();

    void PreProcess(); // The preprocess function applies in-degree and and outdegree constraints. 
  
    // The following Balance functon is a preprocess step that goes through all the outgoing arcs
    // of the DFG and if they are more than n cycles apart adds, routing nodes to balance. 
    // After this it calls the PreProcess function. In previous approaches this happens during the 
    // Mapping process. The unmapped nodes are then taken and balanced. Decoupling these steps
    // increases the chances of finding a mapping in the first attempt.

    // TODO: Add register routability to the nodes in BalanceDFG function. 

    //bool BalanceDFG(int& id, int number_of_resources, int& II); //implicitly calls schedule after DFG modification.
    //bool is_balanced(int II);

    int Schedule(int id, int number_of_resources, int II);
    int Schedule_1(int id, int number_of_resources, int II, int &schedule_len, int* PEs, int *address_bus, int *data_bus);
    int Schedule_2(int id, int number_of_resources, int II);
    int Init_Schedule(int id, int number_of_resources, int rec_MII);
    int Modulo_ReSchedule_Backward(int id, int number_of_resources, int II, vector<Node*> &to_change_set);
    int Modulo_ReSchedule_Forward(int id, int number_of_resources, int II, vector<Node*> &to_change_set);
    int Modulo_ReSchedule_Smart(int id, int number_of_resources, int II, vector<Node*> &to_change_set);
    int Modulo_ReSchedule_Smart_2(int id, int number_of_resources, int II, vector<Node*> &to_change_set);
    int Modulo_ReSchedule_Smart_3(int id, int number_of_resources, int II, vector<Node*> &to_change_set); 
    int Modulo_Route_ReSchedule(int &id, int number_of_resources, int II, vector<Node*> &to_change_set, vector<Node*> &route);
    tuple<bool, int> isFeasibleII(int II, int &modII, int &resMII, int number_of_resources, int &routing_nodes);
    void print_schedule_list(int II); 
    vector<Node*> get_Sorted_Ops(); 

    // This routine takes in the resource constraint II and the number of resources 
    //  and finds if the resources constraint asap is feasible for the architecture resources. 
    bool isFeasible_ASAP_for_architecture_constraint(int II, int number_of_resources); 

    int RecMII_Scc(vector<vector<Node*> > &sets, int &max_index);
    int Longest_Path(vector<Node*> &nodes, int &II);

    //ASAP schedule all nodes, returns last node execution time
    int Schedule_ASAP();
    //Feasible ASAP Schedule
    int Schedule_ASAP_Feasible(int number_of_resources, int II);

    //ALAP schedule all nodes
    void Schedule_ALAP(int latest_time);
    //Feasible ASAP Schedule
    bool Schedule_ALAP_Feasible(int latesttime, int number_of_resources, int II);

    // Random Modulo Schedule
    bool Random_Modulo_Schedule(int MII, int Last_time, bool &multiple_constraint, int number_of_resources);

    //modulo schedule nodes
    bool Modulo_Schedule(int MII, int Last_time, bool &multiple_constraint, int number_of_resources);
    
    // modulo schedule modification with Schedule ASAP and ALAP
     bool Modulo_Schedule(int MII, int Last_time, bool &multiple_constraint, int number_of_resources, int *PEs, int* addr_bus, int* data_bus);

        // modulo schedule modification
     bool Modulo_Schedule_2(int MII, int Last_time, bool &multiple_constraint, int number_of_resources, int *PEs, int* addr_bus, int* data_bus);

      // Schedule routing operations
      tuple<bool, int, int> Modulo_Schedule_Routing(int II, int Last_Time, bool &multiple_constraint, int number_of_resources, vector<Node*> to_be_scheduled, int* PEs); 

    // Return the parent nodes that have children that needs to share path.
    vector<Node*> Nodes_Path_Sharing();

    //Given a parent this function computes path sharing.
    void Path_Sharing_Algo(int* PEs, Node* parent);

    //schedule a load address operation, checking resources, load data, etc. If succeeds, return true, else false
    //if there are multiple constrained for the nodes or thy constrain multiple nodes, set multiple_constraint
    bool Schedule_Load_Address(Node* selected_node, int schedule_time, int II, bool &multiple_constraint, int* address_bus, int *data_bus, int* PE_resources, int max_resources, vector<Node*> &scheduled_nodes);
    bool Schedule_Load_Data(Node* selected_node, int schedule_time, int II, bool &multiple_constraint, int* address_bus, int* data_bus, int* PE_resources, int max_resources, vector<Node*> &scheduled_nodes);
    bool Schedule_Store_Address_Data(Node* selected_node, int schedule_time, int II, bool &multiple_constraint, int* address_bus, int* data_bus, int* PE_resources, int max_resources, vector<Node*> &scheduled_nodes);

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
    //Set the current schedule as the alap schedule
    void Reset_Schedule(vector<Node*> &nodes);

    vector<Node*> get_sorted_ready_list_modulo_backward(vector<Node*> list);
    vector<Node*> get_Current_Scheduled_Nodes(vector<Node*> inSet);
    //returns all nodes connected with this node must be placed to the same physical PE
    vector<Node*> get_All_Nodes_Constrained_by_Current(Node* current_node, int time);
    //returns all nodes connected with this node must be placed to the same physical PE
    vector<Node*> get_Intra_Iteration_Related_Nodes_Current(Node* current_node, int time);
    //returns all nodes connected connected to this node with inter-iteration dependency
    vector<Node*> get_Inter_Iteration_Related_Nodes(Node* current_node);
    // returns all nodes with outgoing recurrent edges distance > 0
    vector<Node*> get_nodes_with_recurrent_edges_outgoing();
    // returns all nodes with incoming recurrent edges distance > 0
    vector<Node*> get_nodes_with_recurrent_edges_incoming();

    bool has_node_conflict_with_scheduled_nodes(int II, Node* current, int schedule_time, int modulo_schedule, vector<Node*> &scheduled_nodes, Node* &node_in_conflict_path);
    bool has_node_forward_conflict_with_scheduled_nodes(int II, Node* current, int schedule_time, int modulo_schedule, vector<Node*> &scheduled_nodes, Node* &first_node_in_conflict_path);
    bool has_node_backward_conflict_with_scheduled_nodes(int II, Node* current, int schedule_time, int modulo_schedule, vector<Node*> &scheduled_nodes, Node* &last_node_in_conflict_path);
    bool has_forward_conflict(int II, Node* src, int scr_schedule_cycle, int src_modulo_cycle, Node* target, int target_schedule_cycle, int target_modulo_cycle, Node* &first_node_in_conflict_path);
    bool has_backward_conflict(int II, Node* src, int scr_schedule_cycle, int src_modulo_cycle, Node* target, int target_schedule_cycle, int target_modulo_cycle, Node* &first_node_in_conflict_path);

    bool traverse_constrain_path_recursive(Node* src, Node* target, vector<Node*> &visited);

    bool has_multiple_constrains(int II, Node* current, int schedule_time, Node* &last_node_in_constrain_path, vector<Node*> &scheduled_nodes);
    bool constrains_multiple(int II, Node* current, int schedule_time, Node* &first_node_in_constrain_path, vector<Node*> &scheduled_nodes);
    bool constrains(int II, Node* src, int scr_schedule_cycle, Node* target, int target_schedule_cycle, int target_modulo_cycle, Node* &first_node_in_conflict_path);
    bool is_constrained(int II, Node* src, int scr_schedule_cycle, int src_modulo_cycle, Node* target, int target_schedule_cycle, Node* &last_node_in_conflict_path);

    bool has_Node_Conflict_Feasible_ASAP(Node* current_node, int time);
    bool has_Node_Conflict_Feasible_ALAP(Node* current_node, int time);

    //update the number of required registers between node1 and node2
    int get_Required_Register(Node* node1, Node* node2, int II);
    //*************************** Scheduling*********************//

    vector<Node*> Get_SortOps(int &MII, int number_of_resources);

    vector<Node*> SortOps(); 

    vector<Node*> Get_SortOps_1(int &MII, int number_of_resources, vector< vector<Node*> >& Sccs);

    //all SCCs, not necessarily seperated sets
    void Sort_Sccs(vector<vector<Node*> > &sets);
    //all SCCs, not necessarily seperated sets
    void Sort_Ops_by_Depth(vector<Node*> &set);
    //gets nodes in path between existing SCCs and the new SCC
    vector<Node*> get_path_between(vector<vector<Node*> > &visited, vector<Node*> &to_visit);

    //nodes in the same iteration
    vector<Node*> get_all_predecessors(Node* &current);

    //nodes in the same iteration
    vector<Node*> get_all_successors(Node* &current);

    vector<Node*> order_nodes_SCC(vector<Node*> &scc);

    //*************************** Generate Latency of All Nodes *********************//
    string Generate_Latency();

    int get_ResMII(int number_of_resources);
    int get_MemMII();

    //Check whether left node is failing to be mapped due to large schedule distance to/from successor/predecessor
    bool check_node_failure_due_to_large_sched_dist(int II, Node* leftnode, vector<int> &distance1, vector<int> &distance2, vector<Node*> &prev, vector<Node*> &next);

    bool check_large_sched_dist(int II, Node* leftnode, vector<int> &distance1, vector<int> &distance2, vector<Node*> &prev, vector<Node*> &next);
    
    int Reschedule(int II, int number_of_resources, int success, int mode, vector<Node*> reschedule,  vector<int> resched_time);

    //void Route_Reschedule_Map(Node* node, int II, int last_attempt_unmapped, int current_number_resources, bool &valid_reg, int &II_reg, int &util_reg, vector<Node*> &leftNodeMapped_reg);
    int Route_ReSchedule(int II, int number_of_resources, Node* node, Node* pred, Node* succ, int &inserted_node_id);

    int Mem_Reschedule(int II, int number_of_resources, Node* node, Node* othernode, vector<Node*> &inserted_node_ids);
    void set_Mem_Mode(bool mode); //may be we dont need this. Instead, clear maps for mem mode, once done.
    bool  get_Mem_Mode();

    void insert_store_loads_in_between_input(Node* node1, Node* node2, vector<Node*> &inserted_node_ids);

    vector<Node*> sort_rest_asap_nodes_for_memory(vector<Node*> nodeset);

    void prune_redundant_arcs();

    void insert_loads_mem(Node* node1, Node* node2, vector<Node*> &inserted_node_ids);

    void Delete_Mem_Nodes(vector<Node*> nodeset);

    int get_total_resources(int t);

    int get_address_bus_resources(int t);

    int get_data_bus_resources(int t);

    void set_total_resources(int t, int n);

    void set_address_bus_resources(int t, int n);

    void set_data_bus_resources(int t, int n);

    int check_reschedule_of_mem_nodes(int pred_id, vector<Node*> leftnotmapped, int number_of_resources, int II, bool &changed, Node* &nodeToBeMapped, vector<Node*> &inserted_route_nodes);
};

#define HAS_ENOUGH_RESOURCES_FOR_LOAD_INSTRUCTION_AT_CYCLE(TIME1, TIME2, ADDRESS_BUS, DATA_BUS, PE_RESOURCES, MAX_RESOURCES) \
  ((ADDRESS_BUS[TIME1] < cgra_info.PER_ROW_MEM_AVAILABLE * CGRA_X_Dim) && (DATA_BUS[TIME2] < cgra_info.PER_ROW_MEM_AVAILABLE * CGRA_X_Dim) && (PE_RESOURCES[TIME1] < MAX_RESOURCES) && (PE_RESOURCES[TIME2] < MAX_RESOURCES))

#define HAS_ENOUGH_RESOURCES_FOR_STORE_INSTRUCTION_AT_CYCLE(TIME1, ADDRESS_BUS, DATA_BUS,PE_RESOURCES, MAX_RESOURCES) \
  ((ADDRESS_BUS[TIME1] < cgra_info.PER_ROW_MEM_AVAILABLE * CGRA_X_Dim) && (DATA_BUS[TIME1] < cgra_info.PER_ROW_MEM_AVAILABLE * CGRA_X_Dim) && (PE_RESOURCES[TIME1] < (MAX_RESOURCES) - 1))

#define HAS_ENOUGH_RESOURCES_FOR_REGULAR_INSTRUCTION_AT_CYCLE(TIME, RESOURCES, MAX_RESOURCES) \
  (REOSURCES[TIME] < MAX_RESOURCES)

#endif /* DFG_H_ */
