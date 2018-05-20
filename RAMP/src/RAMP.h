/*
*  Models RAMP algorithm
*  Author: Shail Dave
*  Last Modified: Nov 26 2017
*  Arizona State University
*/


#ifndef RAMP_H_
#define RAMP_H_


#include "Parser.h"

class RAMP
{
public:
  RAMP(Parser myParser);

  //Rescheduling and mapping functions
  CGRA Smart_Rechedule_Map(int left_node_id, int pred_id, int succ_id, DFG* &inDFG, unsigned int last_attempt_unmapped, int II, int distance, vector<Node*> prev, vector<Node*> next, int &best_option, vector<Node*> &best_leftmapped, Node* &new_node);

  void Route_Reschedule_Map(int left_id, int pred_id, int succ_id, int II, unsigned int last_attempt_unmapped, DFG* &dfg, int distance, vector<Node*> prev, vector<Node*> next, bool &valid_route, float &util_route, vector<Node*> &leftNodeMapped_route, Node* &new_node, CGRA &myCGRA);

  void Route_RF_ReSchedule_Map(int pred_id, int succ_id, int II, unsigned int last_attempt_unmapped, DFG* &dfg, int distance, vector<Node*> prev, vector<Node*> next, bool &valid_route_rf, float &util_route_rf, vector<Node*> &leftNodeMapped_route_rf, Node* &new_node, CGRA &myCGRA);

  void Mem_Reschedule_Map(int node_id, int pred_id, int succ_id, int II, unsigned int last_attempt_unmapped, DFG* &dfg, int distance, vector<Node*> prev, vector<Node*> next, bool &valid_mem, float &util_mem, vector<Node*> &leftNodeMapped_mem, Node* &new_node, CGRA &myCGRA, vector<Node*> &inserted_mem_nodes);

  bool is_node_left(vector<Node*> leftnodes, Node* node);
};

#endif /* RAMP_H_ */
