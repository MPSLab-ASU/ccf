/*
* Node.cpp
*
*  Created on: Jul 24, 2012
*  Author: mahdi
*
*  Author: Shail Dave
*  Last Edited on: 22 Dec 2016
*/

#include "Node.h"

#define HAS_ENOUGH_RESOURCES_FOR_LOAD_INSTRUCTION_AT_CYCLE(TIME1, TIME2, ADDRESS_BUS, DATA_BUS, PE_RESOURCES, MAX_RESOURCES, MEM_RESOURCES) \
((ADDRESS_BUS[TIME1] < MEM_RESOURCES) && (DATA_BUS[TIME2] < MEM_RESOURCES) && (PE_RESOURCES[TIME1] < MAX_RESOURCES) && (PE_RESOURCES[TIME2] < MAX_RESOURCES))

#define HAS_ENOUGH_RESOURCES_FOR_STORE_INSTRUCTION_AT_CYCLE(TIME1, ADDRESS_BUS, DATA_BUS,PE_RESOURCES, MAX_RESOURCES, MEM_RESOURCES) \
((ADDRESS_BUS[TIME1] < MEM_RESOURCES) && (DATA_BUS[TIME1] < MEM_RESOURCES) && (PE_RESOURCES[TIME1] < (MAX_RESOURCES) - 1))

#define HAS_ENOUGH_RESOURCES_FOR_REGULAR_INSTRUCTION_AT_CYCLE(TIME, RESOURCES, MAX_RESOURCES) \
(REOSURCES[TIME] < MAX_RESOURCES)

Node::Node(Instruction_Operation ins, int laten, int id, string nodeName = " ")
{
  insn = ins;
  cuid = id;
  latency = laten;
  name = nodeName;
  if (latency < 1)
    debugfile << "latency cannot be less than 1\n";

  Load_Output_Address_bus = false;
  Store_Output_Address_bus = false;
  Output_Data_Bus = false;
  Input_Data_Bus = false;

  related = NULL;
  sched_info = new Scheduling_Info();

  index_cycle = -1;
  low_index_cycle = -1;
  index_defined_cycle = false;

  assigned = false;
  AssignedPE = NULL;
  loop = NULL;
  self_loop = false;
  minimizable = false;

  predicated = false;
}

Node::Node(Instruction_Operation ins, int laten, int id, bool load, bool store)
{
  insn = ins;
  cuid = id;
  latency = laten;
  if (latency < 1)
    printf("latency cannot be less than 1\n");

  Load_Output_Address_bus = false;
  Store_Output_Address_bus = false;
  Output_Data_Bus = false;
  Input_Data_Bus = false;

  related = NULL;
  sched_info = new Scheduling_Info();

  index_cycle = -1;
  low_index_cycle = -1;
  index_defined_cycle = false;

  assigned = false;
  AssignedPE = NULL;
  loop = NULL;
  self_loop = false;
  minimizable = false;

  predicated =false;
}

void Node::delete_self_loop()
{
  self_loop = false;
}

void Node::set_self_loop(ARC* s_loop)
{
  self_loop = true;
  loop = s_loop;
}

bool Node::has_self_loop()
{
  return self_loop;
}

ARC* Node::get_self_loop()
{
  return loop;
}

Node::~Node()
{
  inCommingArcs.clear();
  outGoingArcs.clear();
  delete (sched_info);
}

void Node::set_Instruction(Instruction_Operation ins, int laten, int id)
{
  insn = ins;
  cuid = id;
  latency = laten;
}

int Node::get_ID()
{
  return cuid;
}

int Node::get_Latency()
{
  return latency;
}

void Node::set_Latency(int laten)
{
  latency = laten;
}

Instruction_Operation Node::get_Instruction()
{
  return insn;
}

void Node::set_Load_Address_Generator(Node* next)
{
  Load_Output_Address_bus = true;
  related = next;
  latency = 1;
}

void Node::set_Load_Data_Bus_Read(Node* next)
{
  Input_Data_Bus = true;
  related = next;
  latency = 1;
}

void Node::set_Store_Address_Generator(Node* next)
{
  Store_Output_Address_bus = true;
  related = next;
  latency = 0;
}

void Node::set_Store_Data_Bus_Write(Node* next)
{
  Output_Data_Bus = true;
  related = next;
  latency = 1;
}

bool Node::is_Load_Address_Generator()
{
  return Load_Output_Address_bus;
}

bool Node::is_Load_Data_Bus_Read()
{
  return Input_Data_Bus;
}

bool Node::is_Store_Address_Generator()
{
  return Store_Output_Address_bus;
}

bool Node::is_Store_Data_Bus_Write()
{
  return Output_Data_Bus;
}

bool Node:: is_Address_Assert()
{
  return (is_Load_Address_Generator() || is_Store_Address_Generator());
}

bool Node::is_Mem_Operation()
{
  return (Output_Data_Bus || Store_Output_Address_bus || Input_Data_Bus || Load_Output_Address_bus);
}

bool Node::is_PHI_Operations()
{
  return (insn == select_op);
}

Node* Node::get_Related_Node()
{
  return related;
}

bool Node::isPredicated()
{
  return predicated;
}

//get the number of incomming edges
int Node::get_Number_of_Pred()
{
  return (int) inCommingArcs.size();
}
int Node::get_Number_of_Operands()
{
  int count = 0;
  for (int i = 0; i < (int) inCommingArcs.size(); i++)
  {
    if (inCommingArcs[i]->get_Dependency() == TrueDep)
      count++;
  }
  return count;
}

//get the number of outgoing edges
int Node::get_Number_of_Succ()
{
  return (int) outGoingArcs.size();
}

//return the ith incomming egde
ARC* Node::get_Pred_Arc(int i)
{
  return inCommingArcs[i];
}

//return ith outgoing edge
ARC* Node::get_Succ_Arc(int i)
{
  return outGoingArcs[i];
}

//return the set of incomming edges
vector<Node*> Node::Get_Prev_Nodes()
{
  vector<Node*> retVal;
  int SizeOfIncomming = inCommingArcs.size();
  for (int i = 0; i < SizeOfIncomming; i++)
  {
    retVal.push_back(inCommingArcs[i]->get_From_Node());
  }
  return retVal;
}

//return the set of predecessors with true dependency
vector<Node*> Node::Get_True_Dependency_Predecessors()
{
  vector<Node*> retVal;
  int SizeOfIncomming = inCommingArcs.size();
  for (int i = 0; i < SizeOfIncomming; i++)
  {
    if (inCommingArcs[i]->get_Dependency() == TrueDep)
    {
      retVal.push_back(inCommingArcs[i]->get_From_Node());
    }
  }
  return retVal;
}

vector<Node*> Node::Get_Predecessors_Exclude_Address_Dependency()
{
  vector<Node*> retVal;
  int SizeOfIncomming = inCommingArcs.size();
  for (int i = 0; i < SizeOfIncomming; i++)
  {
    if (inCommingArcs[i]->get_Dependency() != StoreDep && inCommingArcs[i]->get_Dependency() != LoadDep)
    {
      retVal.push_back(inCommingArcs[i]->get_From_Node());
    }
  }
  return retVal;

}

vector<Node*> Node::Get_Explicit_Dependency_Predecessor()
{
  vector<Node*> retVal;
  int SizeOfIncomming = inCommingArcs.size();
  for (int i = 0; i < SizeOfIncomming; i++)
  {
    if (inCommingArcs[i]->get_Dependency() == PredDep ||inCommingArcs[i]->get_Dependency() == TrueDep )
    {
      retVal.push_back(inCommingArcs[i]->get_From_Node());
    }
  }
  return retVal;

}
//return the set of predecessor with predicate dependency
vector<Node*> Node::Get_Predicate_Dependency_Predecessor()
{
  vector<Node*> retVal;
  int SizeOfIncomming = inCommingArcs.size();
  for (int i = 0; i < SizeOfIncomming; i++)
  {
    if (inCommingArcs[i]->get_Dependency() == PredDep)
    {
      retVal.push_back(inCommingArcs[i]->get_From_Node());
    }
  }
  return retVal;
}
//return the set of incomming edges
vector<Node*> Node::Get_Prev_Nodes_Same_Iteration()
{
  vector<Node*> retVal;
  int SizeOfIncomming = inCommingArcs.size();
  for (int i = 0; i < SizeOfIncomming; i++)
  {
    if (inCommingArcs[i]->get_Distance() == 0)
    retVal.push_back(inCommingArcs[i]->get_From_Node());
  }
  return retVal;
}

vector<Node*> Node::Get_Predecessors_Same_Iteration_Exclude_Address_Dependency()
{
  vector<Node*> retVal;
  int SizeOfIncomming = inCommingArcs.size();
  for (int i = 0; i < SizeOfIncomming; i++)
  {
    if (inCommingArcs[i]->get_Dependency() != LoadDep && inCommingArcs[i]->get_Dependency() != StoreDep && inCommingArcs[i]->get_Distance() == 0)
    {
      retVal.push_back(inCommingArcs[i]->get_From_Node());
    }
  }
  return retVal;

}

vector<Node*> Node::Get_Explicit_Dependency_Predecessors_Same_Iteration()
{
  vector<Node*> retVal;
  int SizeOfIncomming = inCommingArcs.size();
  for (int i = 0; i < SizeOfIncomming; i++)
  {
    if ((inCommingArcs[i]->get_Dependency() == TrueDep||inCommingArcs[i]->get_Dependency() == PredDep ) && inCommingArcs[i]->get_Distance() == 0)
    {
      retVal.push_back(inCommingArcs[i]->get_From_Node());
    }
  }
  return retVal;
}


//return the set of predecessors with true dependency with distance = 0
vector<Node*> Node::Get_True_Dependency_Predecessors_Same_Iteration()
{
  vector<Node*> retVal;
  int SizeOfIncomming = inCommingArcs.size();
  for (int i = 0; i < SizeOfIncomming; i++)
  {
    if (inCommingArcs[i]->get_Dependency() == TrueDep && inCommingArcs[i]->get_Distance() == 0)
    {
      retVal.push_back(inCommingArcs[i]->get_From_Node());
    }
  }
  return retVal;
}

//return the set of incomming edges with distance
vector<Node*> Node::Get_Prev_Nodes_Prev_Iteration()
{
  vector<Node*> retVal;
  int SizeOfIncomming = inCommingArcs.size();
  for (int i = 0; i < SizeOfIncomming; i++)
  {
    if (inCommingArcs[i]->get_Distance() > 0)
    retVal.push_back(inCommingArcs[i]->get_From_Node());
  }
  return retVal;
}

//return the set of predecessors with true dependency
vector<Node*> Node::Get_True_Dependency_Predecessors_Prev_Iterations()
{
  vector<Node*> retVal;
  int SizeOfIncomming = inCommingArcs.size();
  for (int i = 0; i < SizeOfIncomming; i++)
  {
    if (inCommingArcs[i]->get_Dependency() == TrueDep && inCommingArcs[i]->get_Distance() > 0 )
    {
      retVal.push_back(inCommingArcs[i]->get_From_Node());
    }
  }
  return retVal;
}

vector<Node*> Node::Get_Explicit_Dependency_Predecessors_Prev_Iterations()
{
  vector<Node*> retVal;
  int SizeOfIncomming = inCommingArcs.size();
  for (int i = 0; i < SizeOfIncomming; i++)
  {
    if ((inCommingArcs[i]->get_Dependency() == TrueDep||inCommingArcs[i]->get_Dependency() == PredDep ) && inCommingArcs[i]->get_Distance() > 0)
    {
      retVal.push_back(inCommingArcs[i]->get_From_Node());
    }
  }
  return retVal;
}

//return successors with true dependency
vector<Node*> Node::Get_True_Dependency_Successors()
{
  vector<Node*> retVal;
  int SizeOfOutGoing = outGoingArcs.size();
  for (int i = 0; i < SizeOfOutGoing; i++)
  {
    if (outGoingArcs[i]->get_Dependency() == TrueDep)
    {
      retVal.push_back(outGoingArcs[i]->get_To_Node());
    }
  }
  return retVal;
}

//return successors with true or pred  dependency
vector<Node*> Node::Get_Explicit_Dependency_Successors()
{
  vector<Node*> retVal;
  int SizeOfOutGoing = outGoingArcs.size();
  for (int i = 0; i < SizeOfOutGoing; i++)
  {
    if (outGoingArcs[i]->get_Dependency() == TrueDep ||outGoingArcs[i]->get_Dependency() == PredDep )
    {
      retVal.push_back(outGoingArcs[i]->get_To_Node());
    }
  }
  return retVal;
}

//return the set of outgoing edges
vector<Node*> Node::Get_Next_Nodes()
{
  vector<Node*> retVal;
  int SizeOfOutGoing = outGoingArcs.size();
  for (int i = 0; i < SizeOfOutGoing; i++)
  {
    retVal.push_back(outGoingArcs[i]->get_To_Node());
  }
  return retVal;
}

vector<Node*> Node::Get_Next_Nodes_Same_Iteration()
{
  vector<Node*> retVal;
  int SizeOfOutGoing = outGoingArcs.size();
  for (int i = 0; i < SizeOfOutGoing; i++)
  {
    if (outGoingArcs[i]->get_Distance() == 0)
      retVal.push_back(outGoingArcs[i]->get_To_Node());
  }
  return retVal;
}

//return successors with predicate dependency with distance = 0
vector<Node*> Node::Get_Predicate_Dependency_Successors_Same_Iteration()
{
  vector<Node*> retVal;
  int SizeOfOutGoing = outGoingArcs.size();
  for (int i = 0; i < SizeOfOutGoing; i++)
  {
    if (outGoingArcs[i]->get_Dependency() == PredDep && outGoingArcs[i]->get_Distance() == 0)
      retVal.push_back(outGoingArcs[i]->get_To_Node());
  }
  return retVal;
}

//return successors with true dependency with distance = 0
vector<Node*> Node::Get_True_Dependency_Successors_Same_Iteration()
{
  vector<Node*> retVal;
  int SizeOfOutGoing = outGoingArcs.size();
  for (int i = 0; i < SizeOfOutGoing; i++)
  {
    if (outGoingArcs[i]->get_Dependency() == TrueDep && outGoingArcs[i]->get_Distance() == 0)
      retVal.push_back(outGoingArcs[i]->get_To_Node());
  }
  return retVal;
}

//return successors with distance = 0 excluding load store address dependency
vector<Node*> Node::Get_Successors_Same_Iteration_Exclude_Address_Dependency()
{
  vector<Node*> retVal;
  int SizeOfOutGoing = outGoingArcs.size();
  for (int i = 0; i < SizeOfOutGoing; i++)
  {
    if (outGoingArcs[i]->get_Dependency() != LoadDep && outGoingArcs[i]->get_Dependency() != StoreDep  && outGoingArcs[i]->get_Distance() == 0)
      retVal.push_back(outGoingArcs[i]->get_To_Node());
  }
  return retVal;
}

//return successors with true or predicate dependency with distance = 0
vector<Node*> Node::Get_Explicit_Dependency_Successors_Same_Iteration()
{
  vector<Node*> retVal;
  int SizeOfOutGoing = outGoingArcs.size();
  for (int i = 0; i < SizeOfOutGoing; i++)
  {
    if ((outGoingArcs[i]->get_Dependency() == TrueDep ||outGoingArcs[i]->get_Dependency() == PredDep)  && outGoingArcs[i]->get_Distance() == 0)
      retVal.push_back(outGoingArcs[i]->get_To_Node());
  }
  return retVal;
}

//return the set of outgoing edges with distance
vector<Node*> Node::Get_Next_Nodes_Next_Iteration()
{
  vector<Node*> retVal;
  int SizeOfOutGoing = outGoingArcs.size();
  for (int i = 0; i < SizeOfOutGoing; i++)
  {
    if (outGoingArcs[i]->get_Distance() > 0)
      retVal.push_back(outGoingArcs[i]->get_To_Node());
  }
  return retVal;
}

//return successors with true dependency with distance > 0
vector<Node*> Node::Get_True_Dependency_Successors_Next_Iterations()
{
  vector<Node*> retVal;
  int SizeOfOutGoing = outGoingArcs.size();
  for (int i = 0; i < SizeOfOutGoing; i++)
  {
    if (outGoingArcs[i]->get_Dependency() == TrueDep && outGoingArcs[i]->get_Distance() > 0)
      retVal.push_back(outGoingArcs[i]->get_To_Node());
  }
  return retVal;
}

//return successors with true or predicate dependency with distance > 0
vector<Node*> Node::Get_Explicit_Dependency_Successors_Next_Iterations()
{
  vector<Node*> retVal;
  int SizeOfOutGoing = outGoingArcs.size();
  for (int i = 0; i < SizeOfOutGoing; i++)
  {
    if ((outGoingArcs[i]->get_Dependency() == TrueDep ||outGoingArcs[i]->get_Dependency() == PredDep)  && outGoingArcs[i]->get_Distance() > 0)
      retVal.push_back(outGoingArcs[i]->get_To_Node());
  }
  return retVal;
}

//add a new incomming edge
void Node::add_Pred_Arc(ARC* pred_Arc)
{
  if (pred_Arc->get_Dependency() == PredDep)
  {
    //FATAL(predicated, "Cannot Predicate an already predicated operation");
    //predicated=true;
  }
  inCommingArcs.push_back(pred_Arc);
}

//add a new outgoing edge
void Node::add_Succ_Arc(ARC* succ_Arc)
{
  outGoingArcs.push_back(succ_Arc);
}

//remove an incomming edge
int Node::remove_Pred_Arc(ARC* predArc)
{
  vector<ARC*>::iterator iEdge;

  for (iEdge = inCommingArcs.begin(); iEdge != inCommingArcs.end(); iEdge++)
  {
    if ( ((*iEdge)->get_From_Node()->get_ID() == predArc->get_From_Node()->get_ID()) && ((*iEdge)->get_To_Node()->get_ID() == predArc->get_To_Node()->get_ID()) )
    {
      inCommingArcs.erase(iEdge, iEdge + 1);
      return 0;
    }
  }

  if(predArc->get_From_Node()->get_ID() == predArc->get_To_Node()->get_ID())
    return 0;

  return -1;
}

//remove an outgoing edge
int Node::remove_Succ_Arc(ARC* succArc)
{
  vector<ARC*>::iterator iEdge;

  for (iEdge = outGoingArcs.begin(); iEdge != outGoingArcs.end(); iEdge++)
  {
    if ( ((*iEdge)->get_From_Node()->get_ID() == succArc->get_From_Node()->get_ID()) && ((*iEdge)->get_To_Node()->get_ID() == succArc->get_To_Node()->get_ID()) )
    {
      outGoingArcs.erase(iEdge, iEdge + 1);
      return 0;
    }
  }

  if(succArc->get_From_Node()->get_ID() == succArc->get_To_Node()->get_ID())
    return 0;

  return -1;
}

//returns true if there is an edge between this node and next node
bool Node::is_Connected_To(Node* nextNode)
{
  if (nextNode == this && self_loop)
    return true;
  vector<Node*> nextNodes = Get_Next_Nodes();
  for (int i = 0; i < (int) nextNodes.size(); i++)
  {
    if (nextNodes[i]->get_ID() == nextNode->get_ID())
      return true;
  }

  return false;
}

//returns true if there is a physical edge between this node and next node
bool Node::is_Physically_Connected_To(Node* nextNode)
{
  if (nextNode == this && self_loop)
    return true;
  vector<Node*> nextNodes = Get_Explicit_Dependency_Successors();
  for (int i = 0; i < (int) nextNodes.size(); i++)
  {
    if (nextNodes[i]->get_ID() == nextNode->get_ID())
      return true;
  }

  return false;
}

//**************************Scheduling********************************//

Scheduling_Info* Node::get_Sched_Info()
{
  return sched_info;
}

bool Node::is_ready_to_schedule_ASAP(int &time)
{
  time = 0;
  int current_time;
  vector<Node*> prev_nodes = Get_Predecessors_Same_Iteration_Exclude_Address_Dependency();
  for (int i = 0; i < (int) prev_nodes.size(); i++)
  {
    if (!prev_nodes[i]->get_Sched_Info()->is_ASAP_Initiated())
      return false;
    current_time = prev_nodes[i]->get_Sched_Info()->get_ASAP() + prev_nodes[i]->get_Latency();
    if (current_time > time)
      time = current_time;
  }
  return true;
}

bool Node::is_ready_to_schedule_ALAP(int &time)
{
  time = MAXINT;
  int current_time;
  vector<Node*> next_nodes = Get_Successors_Same_Iteration_Exclude_Address_Dependency();
  for (int i = 0; i < (int) next_nodes.size(); i++)
  {
    if (!next_nodes[i]->get_Sched_Info()->is_ALAP_Initiated())
      return false;
    current_time = next_nodes[i]->get_Sched_Info()->get_ALAP() - latency;
    if (current_time < time)
      time = current_time;
  }
  return true;
}

bool Node::is_ready_to_schedule_Feasible_ASAP(int &time, int II)
{
  time = 0;
  int current_time;
  vector<Node*> prev_nodes = Get_Predecessors_Same_Iteration_Exclude_Address_Dependency();
  for (int i = 0; i < (int) prev_nodes.size(); i++)
  {
    if (!prev_nodes[i]->get_Sched_Info()->is_Feasible_ASAP_Initiated())
      return false;
    current_time = prev_nodes[i]->get_Sched_Info()->get_Feasible_ASAP() + prev_nodes[i]->get_Latency();
    if (current_time > time)
      time = current_time;
  }

  if(is_Mem_Operation())
  {
    for (std::map<Node*, vector<Node*> >::iterator it=constrained_store_load_pairs.begin(); it!=constrained_store_load_pairs.end(); ++it)
    {
      vector<Node*> load_address_nodes = it->second;
      for(unsigned int n = 0; n < load_address_nodes.size(); n++) {
        if(load_address_nodes[n]->get_ID() == get_ID()) //We need to Update ASAP of Load Address Node (node2).
        {
          Node* node1 = it->first;
          Node* pred = constrained_st_pred_pairs[node1];

          if (!pred->get_Sched_Info()->is_Feasible_ASAP_Initiated())
            return false;

          int node1_asap = pred->get_Sched_Info()->get_Feasible_ASAP() + pred->get_Latency();
          int node2_asap = node1_asap + node1->get_Latency() - get_Latency();
          if(node2_asap > time)
            time = node2_asap;

          //Check That ASAP of Load Address is In Accordance With ASAP of Successor Node.
          Node* succ = constrained_ld_succ_pairs[load_address_nodes[n]];
          ARC* edge = get_Related_Node()->get_Arc(succ);
          int succ_time = 0;
          int edge_dist = edge->get_Distance();
          if(edge_dist > 0)
            succ_time = succ->get_Sched_Info()->get_Feasible_ASAP() + (edge_dist * II);
          else
            succ_time = succ->get_Sched_Info()->get_Feasible_ASAP();

          int node2_asap_reverse = succ_time - get_Related_Node()->get_Latency() - get_Latency();

          if(node2_asap_reverse > time)
            time = node2_asap_reverse;
        }
      }
    }
  }
  return true;
}

bool Node::is_ready_to_schedule_Feasible_ALAP(int &time, int MAX_Schedule, int II)
{
  time = MAXINT;
  int current_time;
  vector<Node*> next_nodes = Get_Successors_Same_Iteration_Exclude_Address_Dependency();
  int prev_nodes_size = Get_Prev_Nodes().size(); //Get_Prev_Nodes_Same_Iteration().size();
  if(is_Load_Data_Bus_Read())
    prev_nodes_size = get_Related_Node()->Get_Prev_Nodes().size(); //Get_Prev_Nodes_Same_Iteration().size();
  if ((int) next_nodes.size() > 0)
  {
    for (int i = 0; i < (int) next_nodes.size(); i++)
    {
      if (!next_nodes[i]->get_Sched_Info()->is_Feasible_ALAP_Initiated())
        return false;

      current_time = next_nodes[i]->get_Sched_Info()->get_Feasible_ALAP() - latency;
      if (current_time < time)
        time = current_time;
    }
    if(!is_PHI_Operations() && (!is_Load_Data_Bus_Read() || (prev_nodes_size > 0)))
    {
      current_time = get_Sched_Info()->get_Feasible_ASAP() + II - latency; // + (MAX_Schedule/2);
      if (current_time < time)
        time = current_time;
    }
  }
  else
    time = get_Sched_Info()->get_Feasible_ASAP() + II - latency;

  if ((time - II + latency) > get_Sched_Info()->get_Feasible_ASAP()) {
    get_Sched_Info()->set_Feasible_ASAP((time - II + latency));
    if(is_Load_Data_Bus_Read())
      get_Related_Node()->get_Sched_Info()->set_Feasible_ASAP((time - II + latency - get_Related_Node()->get_Latency()));
  }

  return true;

  if(is_Mem_Operation())
  {
    for (std::map<Node*, vector<Node*> >::iterator it=constrained_store_load_pairs.begin(); it!=constrained_store_load_pairs.end(); ++it) {
      if(it->first->get_ID() == get_ID()) //We need to Update ALAP of Store Data/Address Node (node1).
      {
        //Iterate through all load nodes corresponding to a store for far scheduled nodes
        vector<Node*> load_address_nodes = it->second;
        for(unsigned int n=0; n < load_address_nodes.size(); n++)  {
          Node* node2 = load_address_nodes[n];
          Node* succ = constrained_ld_succ_pairs[node2];

          if (!succ->get_Sched_Info()->is_Feasible_ALAP_Initiated())
            return false;

          //Iterate through all load nodes corresponding to a store for far scheduled nodes
          vector<Node*> load_address_nodes = it->second;
          for(unsigned int n=0; n < load_address_nodes.size(); n++)  {
            Node* node2 = load_address_nodes[n];
            Node* succ = constrained_ld_succ_pairs[node2];
            int time = get_Sched_Info()->get_Feasible_ALAP();
            ARC* edge = node2->get_Related_Node()->get_Arc(succ);
            int succ_time = 0;
            if(edge->get_Distance() > 0)
              succ_time = succ->get_Sched_Info()->get_Feasible_ALAP() + (edge->get_Distance() * II);
            else
              succ_time = succ->get_Sched_Info()->get_Feasible_ALAP();
            int node1_alap = succ_time - node2->get_Related_Node()->get_Latency() - get_Latency(); //can be changed as max(st_add, st_data) latency.
            if(node1_alap < time)
            {
              get_Sched_Info()->set_Feasible_ALAP(node1_alap);
              get_Related_Node()->get_Sched_Info()->set_Feasible_ALAP(node1_alap);
            }
          }
        }
      }
    }
  }
}

void Node::Update_ASAP_Feasible(int* add_bus, int* data_bus, int* total_resources, int number_of_resources, int mem_resources)
{
  if(is_Load_Address_Generator())
  {
    Node* loadData     = get_Related_Node();
    vector<Node*> succ = loadData->Get_Next_Nodes();
    int earliestTimeofSucc  = 10000;
    for(unsigned int i = 0; i < succ.size(); i++)
    {
      if(succ[i]->get_Sched_Info()->get_Feasible_ASAP() < earliestTimeofSucc)
        earliestTimeofSucc = succ[i]->get_Sched_Info()->get_Feasible_ASAP();
    }

    int loadDataASAP = earliestTimeofSucc - loadData->get_Latency();
    int loadAddASAP  = loadDataASAP - get_Latency();

    if((get_Sched_Info()->get_Feasible_ASAP() > loadAddASAP) && (HAS_ENOUGH_RESOURCES_FOR_LOAD_INSTRUCTION_AT_CYCLE(loadAddASAP, loadDataASAP, add_bus, data_bus, total_resources, number_of_resources, mem_resources)))
    {
      get_Sched_Info()->set_Feasible_ASAP(loadAddASAP);
      loadData->get_Sched_Info()->set_Feasible_ASAP(loadDataASAP);
    }
  }
}

void Node::Update_ASAP_for_Phi(int II)
{
  if(!is_PHI_Operations())
    return;

  int sched_time = 0;
  int current_time = 0;
  vector<Node*> prev_nodes = Get_Prev_Nodes();
  for (int i = 0; i < (int) prev_nodes.size(); i++)
  {
    if (!prev_nodes[i]->get_Sched_Info()->is_ASAP_Initiated())
      return;

    ARC* edge = prev_nodes[i]->get_Arc(this);
    int edge_dist = edge->get_Distance();
    if(edge_dist > 0)
      current_time = prev_nodes[i]->get_Sched_Info()->get_ASAP() + prev_nodes[i]->get_Latency() - (edge_dist * II);

    if (current_time > sched_time)
      sched_time = current_time;
  }
  get_Sched_Info()->set_Feasible_ASAP(sched_time);
}

bool Node::is_ready_to_schedule_modulo_backward(int &time)
{
  time = get_Sched_Info()->get_Feasible_ALAP();
  int current_time = get_Sched_Info()->get_Feasible_ALAP();
  if (outGoingArcs.size() == 0)
    return true;

  vector<Node*> next_nodes = Get_Successors_Same_Iteration_Exclude_Address_Dependency();
  for (int i = 0; i < (int) next_nodes.size(); i++)
  {
    if (!next_nodes[i]->get_Sched_Info()->is_CURRENT_Initiated())
      return false;

    current_time = next_nodes[i]->get_Sched_Info()->get_Current() - get_Latency();
    if (current_time < time)
      time = current_time;
  }
  return true;
}

//Methods for cycle detection
void Node::Set_Index_Cycle(int index)
{
  this->index_cycle = index;
  index_defined_cycle = true;
}

int Node::get_Index_Cycle()
{
  return index_cycle;
}

void Node::Set_Low_Index_Cycle(int index)
{
  this->low_index_cycle = index;
}

int Node::get_Low_Index_Cycle()
{
  return low_index_cycle;
}

bool Node::Is_Index_Defined_Cycle()
{
  return index_defined_cycle;
}

void Node::Reset_Cycle_Index()
{
  index_cycle = -1;
  low_index_cycle = -1;
  index_defined_cycle = false;
}

//Methods corresponding to Mapping of the node
void Node::Assign2PE(PE* pe)
{
  this->AssignedPE = pe;
  assigned = true;
}

PE* Node::getPE()
{
  return this->AssignedPE;
}

bool Node::isFree()
{
  return !assigned;
}

bool Node::isMapped()
{
  return assigned;
}

void Node::Reset()
{
  assigned = false;
  AssignedPE = NULL;
}

vector<Node*> Node::get_physical_successors_constrain_by_this_modulo(int II)
{
  vector<Node*> retVal;
  int SizeOfIncomming = outGoingArcs.size();

  for (int i = 0; i < SizeOfIncomming; i++)
  {
    if (outGoingArcs[i]->get_Dependency()!= TrueDep && outGoingArcs[i]->get_Dependency()!= PredDep)
      continue;

    int distance1 = 0;
    int edge_distance = outGoingArcs[i]->get_Distance();
    if(edge_distance > 0)
    {
      distance1 = outGoingArcs[i]->get_To_Node()->get_Sched_Info()->get_Current() - this->get_Sched_Info()->get_Current();
      distance1 += II*edge_distance;
    }
    int distance2 = outGoingArcs[i]->get_To_Node()->get_Sched_Info()->get_Current() - this->get_Sched_Info()->get_Current();
    if((distance1 > this->get_Latency()) || (distance2 > this->get_Latency()))
      retVal.push_back(outGoingArcs[i]->get_To_Node());
  }

  if(has_self_loop())
    retVal.push_back(get_self_loop()->get_To_Node());

  return retVal;
}

vector<Node*> Node::get_physical_predecessors_constrain_this_modulo(int II)
{
  vector<Node*> retVal;
  int SizeOfIncomming = inCommingArcs.size();
  for (int i = 0; i < SizeOfIncomming; i++)
  {
    if (inCommingArcs[i]->get_Dependency()!= TrueDep && inCommingArcs[i]->get_Dependency()!= PredDep)
      continue;

    int distance1 = 0;
    int edge_distance = inCommingArcs[i]->get_Distance();
    if(edge_distance > 0)
    {
      distance1 = this->get_Sched_Info()->get_Current() - inCommingArcs[i]->get_From_Node()->get_Sched_Info()->get_Current();
      distance1 += II*edge_distance;
    }
    int distance2 = this->get_Sched_Info()->get_Current() - inCommingArcs[i]->get_From_Node()->get_Sched_Info()->get_Current();

    if((distance1 > this->get_Latency()) || (distance2 > inCommingArcs[i]->get_From_Node()->get_Latency()))
      retVal.push_back(inCommingArcs[i]->get_From_Node());
  }
  return retVal;
}

bool Node::get_dist_physical_successors_constrain_by_this_modulo(int II, vector<int> &distance, vector<Node*> &nodes)
{
  int SizeOfIncomming = outGoingArcs.size();
  bool retVal = false;

  for (int i = 0; i < SizeOfIncomming; i++)
  {
    if (outGoingArcs[i]->get_Dependency()!= TrueDep && outGoingArcs[i]->get_Dependency()!= PredDep)
      continue;

    int distance1 = 0;
    int edge_distance = outGoingArcs[i]->get_Distance();
    if(edge_distance > 0)
    {
      distance1 = outGoingArcs[i]->get_To_Node()->get_Sched_Info()->get_Current() - this->get_Sched_Info()->get_Current();
      distance1 += II*edge_distance;
    }
    int distance2 = outGoingArcs[i]->get_To_Node()->get_Sched_Info()->get_Current() - this->get_Sched_Info()->get_Current();
    if((distance1 > this->get_Latency()) || (distance2 > this->get_Latency()) || (edge_distance > 0))
    {
      int temp = (distance1 > distance2) ? distance1 : distance2;
      //sort the pairs in terms of scheduling time or distance2 (for path sharing purposes)
      //if vectors are empty, push simply
      if(distance.size() == 0)
      {
        distance.push_back(temp);
        nodes.push_back(outGoingArcs[i]->get_To_Node());
      }
      else if(distance.size() == 1)
      {
        if(distance[0] < temp)
        {
          distance.insert(distance.begin()+1,1,temp);
          nodes.insert(nodes.begin()+1,1,outGoingArcs[i]->get_To_Node());
        }
        else
        {
          distance.insert(distance.begin(),1,temp);
          nodes.insert(nodes.begin(),1,outGoingArcs[i]->get_To_Node());
        }
      }
      else	//iterate through the pair of vectors and insert in ascending order of distance
      {
        unsigned int j=0;
        for (j = 0; j < distance.size()-1; j++) {
          if((distance[j+1] > temp) && (distance[j] < temp))
          {
            distance.insert(distance.begin()+j+1,1,temp);
            nodes.insert(nodes.begin()+j+1,1,outGoingArcs[i]->get_To_Node());
          }
        }
        if(j == distance.size())
        {
          distance.insert(distance.begin()+j+1,1,temp);
          nodes.insert(nodes.begin()+j+1,1,outGoingArcs[i]->get_To_Node());
        }
      }
      retVal = true;
    }
  }

  if(has_self_loop())
  {
    distance.insert(distance.end(), get_self_loop()->get_Distance());
    nodes.insert(nodes.end(),1,get_self_loop()->get_To_Node());
    retVal = true;
  }

  return retVal;
}

bool Node::get_dist_physical_predecessors_constrain_this_modulo(int II, vector<int> &distance, vector<Node*> &nodes)
{
  int SizeOfIncomming = inCommingArcs.size();
  bool retVal = false;

  for (int i = 0; i < SizeOfIncomming; i++)
  {
    if (inCommingArcs[i]->get_Dependency()!= TrueDep && inCommingArcs[i]->get_Dependency()!= PredDep)
      continue;

    int distance1 = 0;
    int edge_distance = inCommingArcs[i]->get_Distance();
    if(edge_distance > 0)
    {
      distance1 = this->get_Sched_Info()->get_Current() - inCommingArcs[i]->get_From_Node()->get_Sched_Info()->get_Current();
      distance1 += II*edge_distance;
    }

    int distance2 = this->get_Sched_Info()->get_Current() - inCommingArcs[i]->get_From_Node()->get_Sched_Info()->get_Current();
    if((distance1 > this->get_Latency()) || (distance2 > inCommingArcs[i]->get_From_Node()->get_Latency()) || (edge_distance > 0))
    {
      int temp = (distance1 > distance2) ? distance1 : distance2;
      //sort the pairs in terms of scheduling time or distance2 (for path sharing purposes)
      //if vectors are empty, push simply
      if(distance.size() == 0)
      {
        distance.push_back(temp);
        nodes.push_back(inCommingArcs[i]->get_From_Node());
      }
      else if(distance.size() == 1)
      {
        if(distance[0] < temp)
        {
          distance.insert(distance.begin()+1,1,temp);
          nodes.insert(nodes.begin()+1,1,inCommingArcs[i]->get_From_Node());
        }
        else
        {
          distance.insert(distance.begin(),1,temp);
          nodes.insert(nodes.begin(),1,inCommingArcs[i]->get_From_Node());
        }
      }
      else	//iterate through the pair of vectors and insert in ascending order of distance
      {
        unsigned int j=0;
        for (j = 0; j < distance.size()-1; j++) {
          if((distance[j+1] > temp) && (distance[j] < temp))
          {
            distance.insert(distance.begin()+j+1,1,temp);
            nodes.insert(nodes.begin()+j+1,1,inCommingArcs[i]->get_From_Node());
          }
        }
        if(j == distance.size()) {
          distance.insert(distance.begin()+j+1,1,temp);
          nodes.insert(nodes.begin()+j+1,1,inCommingArcs[i]->get_From_Node());
        }
      }

      retVal = true;
    }
  }

  return retVal;
}

int Node::get_node_height()
{
  return node_height;
}

void Node::set_node_height(int n)
{
  node_height = n;
}

int Node::get_dist(Node* node, int II)
{
  int retVal = -1;
  int SizeOfoutGoing = outGoingArcs.size();
  for (int i = 0; i < SizeOfoutGoing; i++)
  {
    if (outGoingArcs[i]->get_Dependency()!= TrueDep && outGoingArcs[i]->get_Dependency()!= PredDep)
      continue;

    if(outGoingArcs[i]->get_To_Node()->get_ID() != node->get_ID())
      continue;

    if(outGoingArcs[i]->get_Distance() > 0)
      retVal = (node->get_Sched_Info()->get_Current() + (II * outGoingArcs[i]->get_Distance()) - this->get_Sched_Info()->get_Current());
    else
      retVal = (node->get_Sched_Info()->get_Current() - this->get_Sched_Info()->get_Current());
  }

  if(retVal == -1)
  {
     cout << "nodeIDs: " << get_ID() << "->" << node->get_ID() << endl;
     FATAL(true,"No Connection between nodes. Should not reach here.");
  }
  return retVal;
}

//get Arc node1->node2
ARC* Node::get_Arc(Node* node2)
{
  ARC* temp;
  ARC* retVal=NULL;

  for(int i=0; i< get_Number_of_Succ(); i++) {
    temp = get_Succ_Arc(i);
    if(temp->get_To_Node()->get_ID() == node2->get_ID()) {
      retVal = temp;
      break;
    }
  }

  return retVal;
}

Node_Dummy::~Node_Dummy()
{
}

Node_Dummy::Node_Dummy(int laten, int id) :
Node(route_op, laten, id)
{
}

Node_Dummy::Node_Dummy(int laten, int id, Node* orig) :
Node(route_op, laten, id)
{
  if (orig->isPredicated())
  {
    //get predicate input
    vector<Node*> predic_inputs = orig->Get_Predicate_Dependency_Predecessor();

    for(int i=0;i< orig->get_Number_of_Operands(); i++)
    {
      ARC *arc = orig->get_Pred_Arc(1);
      if (arc->get_From_Node()==predic_inputs[0])
      {
        ARC *pArc = new ARC(id+1, arc->get_Distance(), arc->get_Dependency(), arc->get_OperandOrder());
        //set source and destinations of the arc
        pArc->Set_From_Node(predic_inputs[0]);
        pArc->Set_To_Node(this);
        add_Pred_Arc(pArc);
      }
    }
  }
}

PE::PE()
{
  busy = false;
  this->iIndex = 0;
  this->jIndex = 0;
  this->tIndex = 0;
  RegFile = NULL;
  ops = (Operations) 0;
  node = NULL;
  NoMappedOps = 0;
}

PE::~PE()
{
}

void PE::SetIndex(int iIndex, int jIndex, int tIndex, int RegLen)
{
  this->iIndex = iIndex;
  this->jIndex = jIndex;
  this->tIndex = tIndex;
  RegFile = new REGISTER_FILE(RegLen);
}

int PE::getIndexI()
{
  return iIndex;
}

int PE::getIndexJ()
{
  return jIndex;
}

int PE::getIndexT()
{
  return tIndex;
}

bool PE::SameCoordination(PE* nPE)
{
  if (iIndex == nPE->getIndexI() && jIndex == nPE->getIndexJ())
  {
    return true;
  }
  return false;
}

void PE::AssignNode(Node *node)
{
  this->node = node;
  this->node->Assign2PE(this);
  NoMappedOps++;
  busy = true;
}

bool PE::isRegisterAvailable()
{
  return RegFile->isAvailable();
}

int PE::NumberOfAvailableRegisters()
{
  return RegFile->getAvailableNumber();
}

unsigned int PE::NumberOfUsedRegisters()
{
  return RegFile->get_No_Used_Register();
}

void PE::OccupyRegister()
{
  RegFile->Assign();
}

void PE::AllocateRegister(int n)
{
  RegFile->Allocate(n);
}

bool PE::ReleaseRegister()
{
  return RegFile->Reset();
}

void PE::ReleaseRegister(int i)
{
  RegFile->Release(i);
}

bool PE::isBusy()
{
  return busy;
}

void PE::Reset()
{
  this->node->Reset();
  busy = false;
}

Node* PE::getNode()
{
  return node;
}

bool PE::isAbleExecute(Node* node, int II)
{
  // to do: insert a check whether PE supports operation of a node. PE will store supported operations.
  if ((node->get_Sched_Info()->get_Modulo_Current()) != this->getIndexT())
    return false;
  return true;
}

bool PE::isOverUsed()
{
  if (NoMappedOps > 1)
    return true;
  return false;
}

int PE::getOverUsedNumber()
{
  return NoMappedOps;
}

void PE::ReleasePE()
{
  if (NoMappedOps == 0)
  {
    cout << "Error, not assigned yet\n";
  }
  NoMappedOps--;
  if (NoMappedOps == 0)
  {
    busy = false;
  }
}

MappingPair::MappingPair(Node* inNode, PE* inPE)
{
  this->node = inNode;
  this->pe = inPE;
}

Node* MappingPair::getNode()
{
  return node;
}

bool MappingPair::is_Address_Assert()
{
  return node->is_Address_Assert();
}

PE* MappingPair::getPE()
{
  return pe;
}

bool MappingPair::SameCoordinationPE(MappingPair* mPair)
{
  return (this->getPE()->SameCoordination(mPair->getPE()));
}

bool MappingPair::Match(MappingPair* mPair)
{
  if (mPair->getNode()->get_ID() != node->get_ID())
  {
    return false;
  }
  if (mPair->getPE() != pe)
  {
    return false;
  }

  return true;
}

TimeCGRA::TimeCGRA(int SizeX, int SizeY, int RegSize, int time, ConnectionType ConType)
{
  this->SizeX = SizeX;
  this->SizeY = SizeY;
  this->time = time;
  PEs = new PE*[SizeX];

  for (int i = 0; i < SizeX; i++)
    PEs[i] = new PE[SizeY];

  for (int i = 0; i < SizeX; i++)
    for (int j = 0; j < SizeY; j++)
      PEs[i][j].SetIndex(i, j, time, RegSize);

  this->ConType = ConType;
}

TimeCGRA::~TimeCGRA()
{
  if (PEs != NULL)
    delete[] PEs;
}

void TimeCGRA::Reset()
{
  for (int i = 0; i < SizeX; i++)
    for (int j = 0; j < SizeY; j++)
      PEs[i][j].Reset();
}

PE* TimeCGRA::getPE(int i, int j)
{
  return &PEs[i][j];
}

bool Node::isLargeConst(string &constName)
{
  if(get_Instruction() != constant_op)
    return false;

  string nodeName = getName();
  char* str = new char[nodeName.length()+1];
  strcpy(str,nodeName.c_str());

  //Part below indicates dynamic constants (i.e. available at runtime, prior to loop execution)
  if(std::find_if(nodeName.begin(), nodeName.end(), ::isdigit) == nodeName.end()) {
    constName = nodeName;
    return true;
  }
  else
  {
    std::size_t found = nodeName.find("ConstInt");
    if (found==std::string::npos)
    return true;
  }

  //If it is static constant, extract value and check if it exceeds largest value supplied as immediate
  unsigned int result=0;
  sscanf(str, "%*[^0123456789]%d",&result);
  if(result > MAX_CONSTANT) {
    constName = std::to_string(result);
    return true;
  }
  else
  return false;
}

string Node::getName()
{
  return name;
}

void Node::setLoadConstant(bool val)
{
  needToLoadConstant = val;
}

bool Node::getLoadConstant()
{
  return needToLoadConstant;
}
