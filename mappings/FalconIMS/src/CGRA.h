/*
* CGRA.h
*
*  Created on: Aug 10, 2012
*  Author: mahdi
*  Last Edited on: Jan 28, 2017
*  Author: Shail Dave
*/

#ifndef CGRA_H_
#define CGRA_H_

#include "DFG.h"
#include "Mapper.h"
#include "matrix.h"

class CGRA
{
public:
  CGRA();
  virtual ~CGRA();

  // Set CGRA target parameters with Xdim,Ydim, Register size, inter-connect type
  void Initialize(int SizeX, int SizeY, int SizeReg, ConnectionType ConType);

  //set II
  void SetII(int II);

  //Print mapping results
  void ShowMapping();

  // Reset iteration number
  void ResetIteration();

  int getII();

  bool MCS2(DFG* myDFG, vector<Node*> &leftNotMapped);
  bool FALCON(DFG* myDFG, vector<Node*> &leftNotMapped);

  void Prolog(DFG* myDFG, int highest_distance, int node_with_highest_distance);
  void EPIlog(DFG* myDFG);

  Node* getNode(int tableIndex, vector<Node*> &nodes);
  PE* getPE(int tableIndex);

  //return true if given PEs have same X Y coordination
  bool SameCoordinatePEs(PE* pe1, PE* pe2);

  //return true if given PEs are connected, pe1 and pe2 are time sensitive.
  // pe1 time should be in the previous cycle to pe2 for correct functionality
  // TODO: Needs fixing.
  bool connectedPEs(PE* pe1, PE* pe2);

  //return true if given nodes are connected
  bool connected_Nodes(Node* node1, Node* node2);
  bool connected_Nodes2(Node* node1, Node* node2);

  int Compatible(MappingPair* mp1, MappingPair* mp2, DFG* &myDFG);
  bool Compatible2(MappingPair* mp1, MappingPair* mp2, DFG* &myDFG);

  int Physical_Distance(PE* pe1, PE* pe2);
  void Insert_TimeCGRA(TimeCGRA* newCGRA);

  CGRA Copy();
  void CalcPEUtil(float &PEUtil);
  bool IsCompatible(int node1, int node2);
  void ShowCompatibility();	//Print Compatibility Table

  void makeRFTable();
  void getMinMaxReg(int &Rmin, int &Rmax);

  void GenerateCompatibilityTable(DFG* myDFG);
  //Matrix<bool> GenerateAdjacencyTable(DFG* myDFG);
  
  template <typename T> bool GenerateAdjacencyTable(DFG* myDFG, Matrix<T>&, Matrix<T>&, Matrix<T>&);
  Matrix<bool> GenerateAdjacencyTable2(DFG* myDFG);

  void UpdateNeedToLoadConstants(DFG* myDFG);
  void UpdateIntraIterationDepReg(DFG* myDFG);
  void UpdateInterIterationDepReg(DFG* myDFG);
  void UpdateSelfReg(DFG* myDFG);

  void GenerateCompGraph(DFG* myDFG);

private:
  int II;
  vector<TimeCGRA*> timeCGRAList;
  int SizeX;
  int SizeY;
  int SizeReg;
  ConnectionType ConType;
  int iteration;

  int Max_Search_Space = 0;

  //returns true if list1 contains given node
  bool contains(vector<Node*> &nodeset, Node* node);

  //returns true if list1 contains given mappingpair
  bool contains(vector<MappingPair*> &pairs, MappingPair* mp);

  //returns true if list1 contains given nodes in set2
  bool contains(vector<Node*> &nodeset1, vector<Node*> &nodeset2);

  //return true if the row already has a Load address node;
  bool contains_row(vector<int>& row, int row_id);
 
  // check if a node is already there in an adjacency list
  bool contains_list(list<int> adj, int node);
   
  // returns the set_difference between the indices.
  template <typename T>
  vector<T> difference(vector<T> first, vector<T> second); 

  void Remove(vector<int>& input, int value);

  void Remove(vector<Node*> &nodeset, Node* node); 

  tuple<vector<Node*>, vector<Node*> > get_Mapped_pred_succ(Node* v);

  void Update_Pred_Succ_Indices(Node* current);

  template <typename T> vector<size_t> sort_indexes(const vector<T> &v);

  //All valid mapping pairs (operation, resource) for compatibility graph
  vector<MappingPair*> mapPairs;

  char* table; //Table for compatibility graph (adjacency matrix for the compatibility graph with all valid nodes)
  char* rf_table;
  char* kernel_table;
  char* table_without_r;

  //adjacency list representation of the input DFG
  list<int>* adjacency_list;
  
  // pointer of arrays for all nodes visited.
  bool *visited;

  // the adjacency of the input DFG
  Matrix<int> A_g;
  
  // the adjacency of product graph
  Matrix<int> A_h;
  
  // the intermediate matrix for ullmann
  Matrix<int> M;
  
  // Store best indices
  //vector<MappingPair*> BestIndices;  
 
  // Store localized best indices
  //vector<MappingPair*> best_pairs;
 
  // intermediate matrix backup
  Matrix<int> copy_M;
 
  // the time extended CGRA. After a node is mapped it is updated in this matrix.
  //Matrix<int> TEC;

  //Matrix<int> copy_TEC;
  
  // the container that keeps track of which row a load node occupies in a timeframe.
  vector<int> row_idx; 
 
  // the container that keeps track of which row a store addr node occupies in a timeframe.  
  vector<int> store_addr_row_idx;

    // the container that keeps track of which row a store data node occupies in a timeframe.  
  vector<int> store_data_row_idx;
 
  // container to have all the load addr nodes. 
  vector<Node*> Load_nodes;

  // container to have all the store addr nodes.
  vector<Node*> St_nodes;

  // the total set of nodes in the DFG. 
  vector<Node*> _node_Set;

  // total nodes
  int nodeSize;

  // the total set of arcs in the DFG
  vector<ARC*> _arc_set;
  
  // container for nodes that were not able to be mapped.
  vector<Node*> left_nodes_unmapped;
  
  // container for nodes with recurrent outgoing edges that are unmapped
  vector<Node*> recurrent_left_nodes;
 
  // containers to store loop carried dependent nodes.
  typedef pair<Node*, vector<Node*> > lcd_pairs;
  set<lcd_pairs> set_lcd;

  void dumpRFConfig(std::string filename);
  int* numberOfNonrotatingRegPerPE;
  unsigned int* totalRotatingReg;
  unsigned int* rfConfig;

  // private functions
  void calcRFConfig();
  bool refine_M(DFG* myDFG);
  void AssignTEC(int posX, int posY, int sched_time, int node_id);
  void  UnAssignTEC(int posX, int posY, int sched_time, int node_id);
  bool isTECfree(int X, int Y, int sched);
  bool is_already_mapped(Node* node1);
  tuple<int, int, int> get_mapped_coordinates(Node * node1); 
  int get_Node_Index(Node * node1);
  Node* get_Node_from_Set(int number);  
  int get_mapped_mappairs(Node* node1);
  vector<int> get_nodes_mapped_to_row( int row ); 
  void refine_lcd(DFG* myDFG, vector<Node*>& inter_nodes);
  
  // This function checks if M matrix after refining has exactly one 1's per row, and zero or one 1's per column of M. If not returns false. 
  bool Sanity_M();
  tuple<vector<MappingPair*>,vector<int>, int> Get_Free_Coordinates(Node* v, vector<Node*> pred, vector<Node*> succ, vector<int> indices); 
  void BFS(int s); 
  void BFS_refine_impl(DFG* myDFG, int s);
  void BFS_refine_impl_1(DFG* myDFG, int s);
  void print_adj(int nodeSize);
  void print_node_id(DFG* myDFG); 
  void print_visited(int nodeSize);
  void print_unvisited(int nodeSize);
  vector<int> get_unvisited_nodes(int nodeSize);
  bool all_visited(int nodeSize);
  //bool map_load_addr(vector<Node*> load_nodes, vector<int> load_visited);
  //bool map_load_data(vector<Node*> load_nodes, vector<int> load_visited);
  Node* get_Node(int number); 
  vector<Node*> Subtract(vector<Node*> &set1, vector<Node*> &set2);
  vector<MappingPair*> Subtract(vector<MappingPair*> &set1, vector<MappingPair*> &set2);
  vector<MappingPair*> Combine(vector<MappingPair*> set1, vector<MappingPair*> set2);
  bool is_left_mapped(Node *node); 


  //FALCON failure diagnostic
  bool Map_Failure_Diagnostic(Node *v, vector<Node*> pred, vector<Node*> succ, int map_case);


  bool Smart_Map_Recovery_Node_Shallow(Node* n, int map_case);

  // 5. shallow for n pred and n succ mapped.
  bool Smart_Map_Failure_Recovery_Shallow_N(Node* n, int map_case);

  // 5. 1 Deep for n pred and n succ
  bool Smart_Map_Failure_Recovery_1Deep(Node* n, int map_case);


  vector<Node*> get_Nodes_from_current_slot(int time);
  
  // Remap for current time slot 
  bool Remap(vector<Node*> nodes_in_timeslot); 
  bool Remap(vector<Node*> nodes_in_timeslot, int size);
 
  int Search_Space_Compute();  
  void Restore(Matrix<int> M2);
  void Restore_timeslot(int time); 
  void Matrix_Copy(Matrix<int>& M2); 
  // Global iterators
  int node_iterator = 0; 
  int node_iterator1 = 0; 
  int node_iterator2 = 0;

  // GLobal timeslot managers
  vector<Node*> nodes_in_current_slot;
  vector<Node*> nodes_in_pred_slot; 
  vector<Node*> nodes_in_succ_slot;

  // Store best indices
  vector<MappingPair*> BestIndices;

  // Store localized best indices
  vector<MappingPair*> best_pairs;

  // For storing best pairs during Remaping
  vector<MappingPair*> best_pairs_remap;

   Matrix<int> TEC;

  Matrix<int> copy_TEC;

  vector<Node*> to_be_mapped; 
}; 

#endif /* CGRA_H_ */
