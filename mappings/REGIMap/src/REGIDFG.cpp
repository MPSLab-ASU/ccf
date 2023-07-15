/*
 * REGIDFG.cpp
 *
 *  Created on: Jul 24, 2012
 *  Author: Mahdi Hamzeh
 *  To enable GCC to predicate operation    -ftree-loop-if-convert -O2 to -O4
 *
 *  //bug fixed, to_be_scheduled[i]->get_Sched_Info()->set_Feasible_ASAP(0->j);
 *  //bug fixed, 	Total_Resources[0-> j]++;
 * todo if there is a path of constrain may be we should break it!
 * todo:send schedule windows to sub schedule functions, 
 * if no time slot left, resolve conflicts immediately (modulo smart 2)
 */

#include "REGIDFG.h"

REGI_DFG::~REGI_DFG()
{
}

REGI_DFG::REGI_DFG(int x_dim, int y_dim)
{
  //keep track of last edge IDs
  ARC_Max_Index = 0;

  //keep track of node IDs
  Node_Max_Index = 0;
  if (DEBUG_1)
  {
    printf("DFG to be created\n");
  }
  MAX_SCHEDULE_LEN = 0;
  CGRA_X_Dim = x_dim;
  CGRA_Y_Dim = y_dim;
}

/*
 * Make a copy from current DFG
 */
REGI_DFG* REGI_DFG::Copy()
{
  //copy DFG
  REGI_DFG* retVal;

  retVal = new REGI_DFG(CGRA_X_Dim, CGRA_Y_Dim);
  //retVal->CGRA_Y_Dim = CGRA_Y_Dim;

  REGI_Node* tempNode;

  REGI_Node_Dummy* tempDummyNode;
  //for over set of nodes
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    //is it a routing node?
    tempDummyNode = dynamic_cast<REGI_Node_Dummy*>(_node_Set[i]);
    //yes, add a new routing node to the copy DFG
    if (tempDummyNode != NULL)
    {
      tempDummyNode = new REGI_Node_Dummy(_node_Set[i]->get_Latency(), _node_Set[i]->get_ID());
      retVal->insert_Node(tempDummyNode);
    }
    //no, add a simple node to the copy DFG
    else
    {
      tempNode = new REGI_Node(_node_Set[i]->get_Instruction(), _node_Set[i]->get_Latency(), _node_Set[i]->get_ID());

      retVal->insert_Node(tempNode);
    }
  }
  //for over set of constants
  for (int i = 0; i < (int) _constants.size(); i++)
  {
    tempNode = new REGI_Node(_constants[i]->get_Instruction(), _constants[i]->get_Latency(), _constants[i]->get_ID());

    retVal->insert_Constant(tempNode);
  }

  //copy arcs to copy DFG
  for (int i = 0; i < (int) _ARC_Set.size(); i++)
  {

    retVal->make_Arc(retVal->get_Node(_ARC_Set[i]->get_From_Node()->get_ID()), retVal->get_Node(_ARC_Set[i]->get_To_Node()->get_ID()), _ARC_Set[i]->get_Number(), _ARC_Set[i]->get_Distance(), _ARC_Set[i]->get_Dependency());
  }
  //special nodes need more things to do
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {

    //is it a load node?
    if (_node_Set[i]->is_Load_Address_Generator())
    {
      retVal->get_Node(_node_Set[i]->get_ID())->set_Load_Address_Generator(retVal->get_Node(_node_Set[i]->get_Related_Node()->get_ID()));
    }
    else if (_node_Set[i]->is_Load_Data_Bus_Read())
    {
      retVal->get_Node(_node_Set[i]->get_ID())->set_Load_Data_Bus_Read(retVal->get_Node(_node_Set[i]->get_Related_Node()->get_ID()));
    }
    //it is a store node?
    else if (_node_Set[i]->is_Store_Address_Generator())
    {
      retVal->get_Node(_node_Set[i]->get_ID())->set_Store_Address_Generator(retVal->get_Node(_node_Set[i]->get_Related_Node()->get_ID()));
    }
    else if (_node_Set[i]->is_Store_Data_Bus_Write())
    {
      retVal->get_Node(_node_Set[i]->get_ID())->set_Store_Data_Bus_Write(retVal->get_Node(_node_Set[i]->get_Related_Node()->get_ID()));

    }
  }
  retVal->Node_Max_Index = Node_Max_Index;
  retVal->ARC_Max_Index = ARC_Max_Index;

  return retVal;

}
/*
 * returns true if the node is dummy (routing)
 */
bool REGI_DFG::is_Dummy_Node(REGI_Node *current)
{

  REGI_Node_Dummy* tempDummyNode;

  tempDummyNode = dynamic_cast<REGI_Node_Dummy*>(current);
  if (tempDummyNode != NULL)
  {
    return true;
  }
  return false;
}

/* 
 * returns true if a node is a reduced node
 */
bool REGI_DFG::is_Reduced_Node(REGI_Node *current)
{
  REGI_Reduced_Node* temp_Reduced_Node;

  temp_Reduced_Node = dynamic_cast<REGI_Reduced_Node*>(current);
  if (temp_Reduced_Node != NULL)
  {
    return true;
  }
  return false;
}

/*
 * reset all minimizable nodes to regular node
 */
void REGI_DFG::Reset_Minimized_Nodes()
{
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    _node_Set[i]->Reset_Minimizable();
  }
}

/* 
 * add a node to set of nodes in DFG
 * if index of this node is greater than existing highest ID, increase max ID
 */
void REGI_DFG::insert_Node(REGI_Node *pNode)
{
  //printf("a new node %d is to be inserted\n", pNode->get_ID());
  if (pNode->get_ID() > Node_Max_Index)
  {
    Node_Max_Index = pNode->get_ID();
  }
  _node_Set.push_back(pNode);
}
/* 
 * add a constant to set of constants in DFG
 * if index of this node is greater than existing highest ID, increase max ID
 */
void REGI_DFG::insert_Constant(REGI_Node *pNode)
{
  //printf("a new node %d is to be inserted\n", pNode->get_ID());
  if (pNode->get_ID() > Node_Max_Index)
  {
    Node_Max_Index = pNode->get_ID();
  }
  _constants.push_back(pNode);
}
/*
 *  for arc between pNode -> nNode   =>     pNode -> newNode -> nNode
 *  properties of arc pNode -> nNode is copied to pNode -> newNode
 *  FIXED : prevent inserting a nodes between two related memory nodes 20-2-2014
 */
bool REGI_DFG::insert_Node_in_between_input(REGI_Node* pNode, REGI_Node* nNode, REGI_Node* newNode)
{
  REGI_ARC *arc;
  vector<REGI_Node*> nextNodes;
  vector<REGI_ARC*>::iterator iEdge;
  FATAL(pNode->is_Mem_Operation() && nNode==pNode->get_Related_Node(), "Cannot insert a node in between a memory operation and its related node!");

  for (iEdge = _ARC_Set.begin(); iEdge != _ARC_Set.end(); iEdge++)
  {
    arc = *iEdge;
    //find the arc between pNode -> nNode
    if (arc->get_From_Node()->get_ID() == pNode->get_ID() && arc->get_To_Node()->get_ID() == nNode->get_ID())
    {
      //erase it from arc set
      _ARC_Set.erase(iEdge, iEdge + 1);
      //remove it from pNode arcs
      pNode->remove_Succ_Arc(arc);
      //remove it from nNode arcs
      nNode->remove_Pred_Arc(arc);
      //form an arc between pNode and newNode
      make_Arc(pNode, newNode, ARC_Max_Index++, arc->get_Distance(), arc->get_Dependency());
      //form an arc between newNode and nNode
      make_Arc(newNode, nNode, ARC_Max_Index++, 0, TrueDep);
      //return success
      return true;
    }
  }
  //return failed
  FATAL(true, "Could not find arc between Node %d and %d to delete", pNode->get_ID(), nNode->get_ID());
  return false;
}

/*
 *  for arc between pNode -> nNode   =>     pNode -> newNode -> nNode
 *  properties of arc pNode -> nNode is copied to newNode -> nNode
 *  FIXED : prevent inserting a nodes between two related memory nodes 20-2-2014
 */
bool REGI_DFG::insert_Node_in_between_output(REGI_Node* pNode, REGI_Node* nNode, REGI_Node* newNode)
{
  REGI_ARC *arc;
  vector<REGI_Node*> nextNodes;
  vector<REGI_ARC*>::iterator iEdge;

  FATAL(pNode->is_Mem_Operation() && nNode==pNode->get_Related_Node(), "Cannot insert a node in between a memory operation and its related node!");

  for (iEdge = _ARC_Set.begin(); iEdge != _ARC_Set.end(); iEdge++)
  {
    arc = *iEdge;
    //find the arc between pNode and nNode in arc set
    if (arc->get_From_Node()->get_ID() == pNode->get_ID() && arc->get_To_Node()->get_ID() == nNode->get_ID())
    {
      //delete arc from arc set
      _ARC_Set.erase(iEdge, iEdge + 1);
      //remove it from successors of pNode
      pNode->remove_Succ_Arc(arc);
      //remove it from Predecessors of nNode
      nNode->remove_Pred_Arc(arc);
      //form an arc between pNode and new Node
      make_Arc(pNode, newNode, ARC_Max_Index++, 0, TrueDep);
      //form an arc between newNode and nNode
      make_Arc(newNode, nNode, ARC_Max_Index++, arc->get_Distance(), arc->get_Dependency());
      //return success
      return true;
    }
  }
  FATAL(true, "Could not find arc between Node %d and %d to delete", pNode->get_ID(), nNode->get_ID());
  return false;
}

/*
 * returns true if DFG has a node with the given ID number
 */
  bool
REGI_DFG::has_Node(int number)
{
  vector<REGI_Node*>::iterator iNode1;
  for (iNode1 = _node_Set.begin(); iNode1 != _node_Set.end(); iNode1++)
  {
    if ((*iNode1)->get_ID() == number)
    {
      return true;        }

  }
  return false;
}
/*
 * returns true if DFG has a node with the given ID number
 */
  bool
REGI_DFG::has_Constant(int number)
{
  vector<REGI_Node*>::iterator iNode1;
  for (iNode1 = _constants.begin(); iNode1 != _constants.end(); iNode1++)
  {
    if ((*iNode1)->get_ID() == number)
    {
      return true;
    }

  }
  return false;
}
/*
 * return a node with given ID number
 */
  REGI_Node*
REGI_DFG::get_Node(int number)
{
  vector<REGI_Node*>::iterator iNode1;
  for (iNode1 = _node_Set.begin(); iNode1 != _node_Set.end(); iNode1++)
  {
    if ((*iNode1)->get_ID() == number)
    {
      return *iNode1;
    }

  }
  FATAL(true, "Node with ID=%d is not found", number);
  return NULL;
}

/*
 * make an arc between two nodes
 */
void REGI_DFG::make_Arc(REGI_Node* pNin, REGI_Node* pNout, int ID, int Distance, DataDepType dep)
{
  //if they are already connected, ignore it
  if (pNin->is_Connected_To(pNout))
  {
    return;
  }
  //source and destination are the same and the node has already a self loop, ignore it
  if (pNin->get_ID() == pNout->get_ID() && pNin->has_self_loop())
  {
    return;
  }
  REGI_ARC *pArc;
  //create an arc with given properties
  pArc = new REGI_ARC(ID, Distance, dep);

  //set source and destinations of the arc
  pArc->Set_From_Node(pNin);
  pArc->Set_To_Node(pNout);

  //if source and destination are the same, create a self loop
  if (pNin->get_ID() == pNout->get_ID())
  {
    pNin->set_self_loop(pArc);
  }
  else
  {
    //connect those nodes through this new arc
    pNout->add_Pred_Arc(pArc);
    pNin->add_Succ_Arc(pArc);
  }

  //add the arc to the arc set
  _ARC_Set.push_back(pArc);

  //update the max ARC ID if needed
  if (ARC_Max_Index < ID)
  {
    ARC_Max_Index = ID;
  }
}

/*
 * returns true if given set contains given node
 */
bool REGI_DFG::contains(vector<REGI_Node*> &nodeset, REGI_Node* node)
{
  vector<REGI_Node*>::iterator iNode1;
  for (iNode1 = nodeset.begin(); iNode1 != nodeset.end(); iNode1++)
  {
    //found the node with the same ID
    if ((*iNode1)->get_ID() == node->get_ID())
      return true;

  }
  return false;
}

/*
 * returns true if two given sets have any node in commmon
 */
bool REGI_DFG::Not_Empty_Intersect(vector<REGI_Node*> &nodeset1, vector<REGI_Node*> &nodeset2)
{
  vector<REGI_Node*>::iterator iNode1;
  for (iNode1 = nodeset2.begin(); iNode1 != nodeset2.end(); iNode1++)
  {
    //does nodeset1 contains inode1?
    if (contains(nodeset1, *iNode1))
      return true;
  }
  return false;
}

/*
 * remove node from nodeset
 */
void REGI_DFG::Remove(vector<REGI_Node*> &nodeset, REGI_Node* node)
{
  vector<REGI_Node*>::iterator iNode;
  for (iNode = nodeset.begin(); iNode != nodeset.end(); iNode++)
  {
    if ((*iNode)->get_ID() == node->get_ID())
    {
      nodeset.erase(iNode, iNode + 1);
      return;
    }
  }
}

/* 
 * remove all nodes in toRemmoveNodes from nodeset
 */
void REGI_DFG::Remove(vector<REGI_Node*> &nodeset, vector<REGI_Node*> &toRemoveNodes)
{
  vector<REGI_Node*>::iterator iNode;
  for (iNode = toRemoveNodes.begin(); iNode != toRemoveNodes.end(); iNode++)
  {
    Remove(nodeset, *iNode);
  }
}

/*
 * delete pNode from _nodeset
 */
int REGI_DFG::delete_Node(REGI_Node *pNode)
{
  vector<REGI_Node*>::iterator iNode;

  for (iNode = _node_Set.begin(); iNode != _node_Set.end(); iNode++)
  {
    if ((*iNode)->get_ID() == pNode->get_ID())
    {
      _node_Set.erase(iNode, iNode + 1);
      return 0;
    }
  }
  /*
   * todo: should we raise an error?
   */
  return -1;
}

/*
 * remove arc from from _ARC_Set
 */
void REGI_DFG::Remove_Arc(REGI_ARC* arc)
{
  vector<REGI_ARC*>::iterator iEdge;
  for (iEdge = _ARC_Set.begin(); iEdge != _ARC_Set.end(); iEdge++)
  {
    if (*iEdge == arc)
    {
      _ARC_Set.erase(iEdge, iEdge + 1);
      return;
    }
  }
  //FATAL(true, "Could not find arc %d to delete", arc->get_Number());
}

/*
 * remove arc pNode -> nNode
 */
void REGI_DFG::Remove_Arc(REGI_Node *pNode, REGI_Node *nNode)
{
  REGI_ARC *edge;
  vector<REGI_ARC*>::iterator iEdge;
  for (iEdge = _ARC_Set.begin(); iEdge != _ARC_Set.end(); iEdge++)
  {
    edge = *iEdge;
    //is it the selected arc?
    if (edge->get_From_Node()->get_ID() == pNode->get_ID() && edge->get_To_Node()->get_ID() == nNode->get_ID())
    {
      //remove it from _ARC_Set
      _ARC_Set.erase(iEdge, iEdge + 1);
      //remove it from successors of pNode
      pNode->remove_Succ_Arc(edge);
      //Remove it from predecessors of nNode
      nNode->remove_Pred_Arc(edge);
      return;
    }
  }
  FATAL(true, "Could not find arc between node %d and %d", pNode->get_ID(), nNode->get_ID());
}

/*
 * add an arc to _ARC_Set
 */
void REGI_DFG::Add_Arc(REGI_ARC* arc)
{
  _ARC_Set.push_back(arc);
}

/*
 * add a set of arcs to _ARC_Set
 */
void REGI_DFG::Add_Arcs(vector<REGI_ARC*> &arcs)
{
  vector<REGI_ARC*>::iterator iEdge;
  for (iEdge = arcs.begin(); iEdge != arcs.end(); iEdge++)
  {
    Add_Arc(*iEdge);
  }
}

/*
 * Get the arc pNode -> nNode 
 */
  REGI_ARC*
REGI_DFG::get_Arc(REGI_Node *pNode, REGI_Node *nNode)
{
  REGI_ARC *edge;
  vector<REGI_ARC*>::iterator iEdge;
  for (iEdge = _ARC_Set.begin(); iEdge != _ARC_Set.end(); iEdge++)
  {
    edge = *iEdge;
    if (edge->get_From_Node()->get_ID() == pNode->get_ID() && edge->get_To_Node()->get_ID() == nNode->get_ID())
    {
      return edge;
    }
  }
  return NULL;
}

/*
 * remove a set of arcs from _ARC_Set
 */
void REGI_DFG::Remove_Arcs(vector<REGI_ARC*> &arcs)
{
  vector<REGI_ARC*>::iterator iEdge;
  for (iEdge = arcs.begin(); iEdge != arcs.end(); iEdge++)
  {
    Remove_Arc(*iEdge);
  }
}

/*
 * returns the intersect of nodeset1 and nodeset2
 */
vector<REGI_Node*> REGI_DFG::Intersect(vector<REGI_Node*> &nodeset1, vector<REGI_Node*> &nodeset2)
{
  vector<REGI_Node*> retVal;
  vector<REGI_Node*>::iterator iNode1;
  for (iNode1 = nodeset2.begin(); iNode1 != nodeset2.end(); iNode1++)
  {
    //if nodeset1 contains iNode1 and it is not already in the intersection
    if (contains(nodeset1, *iNode1) && !contains(retVal, *iNode1))
      retVal.push_back(*iNode1);
  }
  return retVal;
}

/*
 * returns the union of nodesset1 and nodeset2
 */
vector<REGI_Node*> REGI_DFG::Union(vector<REGI_Node*> &nodeset1, vector<REGI_Node*> &nodeset2)
{
  vector<REGI_Node*> retVal;
  vector<REGI_Node*>::iterator iNode1;
  for (iNode1 = nodeset1.begin(); iNode1 != nodeset1.end(); iNode1++)
  {
    //add inode1 if it is not already in the union
    if (!contains(retVal, *iNode1))
      retVal.push_back(*iNode1);
  }
  for (iNode1 = nodeset2.begin(); iNode1 != nodeset2.end(); iNode1++)
  {
    //add inode1 if it is not already in the union
    if (!contains(retVal, *iNode1))
      retVal.push_back(*iNode1);
  }
  return retVal;
}

/*
 * returns set1 - set2
 */
vector<REGI_Node*> REGI_DFG::Subtract(vector<REGI_Node*> &set1, vector<REGI_Node*> &set2)
{
  vector<REGI_Node*> retVal;
  vector<REGI_Node*>::iterator iNode1;
  for (iNode1 = set1.begin(); iNode1 != set1.end(); iNode1++)
  {
    //if iNode1 is not in set2, add it
    if (!contains(set2, *iNode1))
      retVal.push_back(*iNode1);
  }
  return retVal;
}

/* 
 * topologicall sort nodes in DFG
 */
queue<REGI_Node*> REGI_DFG::Topological_Sort()
{
  vector<REGI_Node*>::iterator iNode;
  queue<REGI_Node*> visitedQueue;
  queue<REGI_Node*> sorted;
  vector<REGI_Node*> visited;
  vector<REGI_Node*> next;
  vector<REGI_Node*> prevs;
  vector<REGI_Node*> intersect;

  //add _node_set[0] to visited queue
  visitedQueue.push(_node_Set[0]);

  REGI_Node* temp;
  while (true)
  {
    //get the first node in the set of visited nodes
    temp = visitedQueue.front();
    //remove it from visited queue
    visitedQueue.pop();
    //add it to sorted list
    sorted.push(temp);
    //add it to set of visited
    visited.push_back(temp);
    //get set of children nodes to visited node
    next = temp->Get_Next_Nodes();
    //for each child in this set
    for (iNode = next.begin(); iNode != next.end(); iNode++)
    {
      //get set of parents nodes of a child node
      prevs = (*iNode)->Get_Prev_Nodes();
      //intersect with visited set
      intersect = Intersect(visited, prevs);
      //all parents are already visited
      if (intersect.size() == prevs.size())
      {
        //it is ready to be added to visited queue
        visitedQueue.push(*iNode);
      }
    }
    //if all nodes are visited and no node in visited queue
    if (visitedQueue.size() == 0 && (visited.size() == _node_Set.size()))
    {
      break;
    }
    //if visited queue is empty but not all nodes are visited
    if (visitedQueue.size() == 0)
    {
      //find a node that is not visited yet
      for (iNode = _node_Set.begin(); iNode != _node_Set.end(); iNode++)
      {
        if (!contains(visited, (*iNode)))
        {
          visitedQueue.push(*iNode);
        }
      }
    }
  }
  return sorted;
}

/*
 * trojan algorith to find strongly connected nodes in a DFG
 */
void REGI_DFG::Strongly_Connected(vector<vector<REGI_Node*> > &results)
{
  stack<REGI_Node*> S;
  vector<REGI_Node*> SCopy;
  int index = 0;
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    if (!_node_Set[i]->Is_Index_Defined_Cycle())
    {
      Strongly_Connected(_node_Set[i], S, SCopy, index, results);
    }
  }
}

/*
 * find a cycle starting from current node
 */
void REGI_DFG::Strongly_Connected(REGI_Node* currentNode, stack<REGI_Node*> &S, vector<REGI_Node*> &SCopy, int &index, vector<vector<REGI_Node*> > &results)
{
  vector<REGI_Node*> local;
  //set index as index of current node
  currentNode->Set_Index_Cycle(index);
  //set index as low index of current node
  currentNode->Set_Low_Index_Cycle(index);
  //increase index
  index++;
  //push current node to stack
  S.push(currentNode);
  //push current node to copy stack
  SCopy.push_back(currentNode);
  //get set of children of current node
  //FIXME: Verify for memory dependencies 20-2-2014
  vector<REGI_Node*> nextNodes = currentNode->Get_Next_Nodes();
  //for each chile
  for (int i = 0; i < (int) nextNodes.size(); i++)
  {
    //if the node has not been visited yet
    if (!nextNodes[i]->Is_Index_Defined_Cycle())
    {
      //visit it
      Strongly_Connected(nextNodes[i], S, SCopy, index, results);
      //if the low index of the chile is less that currnet node
      if (nextNodes[i]->get_Low_Index_Cycle() < currentNode->get_Low_Index_Cycle())
      {
        //low index of current  = low index of its child [i]
        currentNode->Set_Low_Index_Cycle(nextNodes[i]->get_Low_Index_Cycle());
      }
    }
    //if the child is visited and present in copy stack
    else if (contains(SCopy, nextNodes[i]))
    {
      //index of child[i] < low index of current
      if (nextNodes[i]->get_Index_Cycle() < currentNode->get_Low_Index_Cycle())
      {
        //low index of current = index of child [i]
        currentNode->Set_Low_Index_Cycle(nextNodes[i]->get_Index_Cycle());
      }
    }
  }
  //there is a cycle found for this node
  if (currentNode->get_Index_Cycle() == currentNode->get_Low_Index_Cycle())
  {
    while ((int) S.size() != 0)
    {
      //add nodes in stack to the cycle
      REGI_Node* temp = S.top();
      local.push_back(temp);
      Remove(SCopy, temp);
      S.pop();
      if (currentNode->get_ID() == temp->get_ID())
      {
        break;
      }
    }
    results.push_back(local);
  }
}

/*
 * generate a unique ID for nodes
 */
int REGI_DFG::Get_Unique_Index()
{
  return ++Node_Max_Index;
}

/*
 * return _node_set
 */
vector<REGI_Node*> REGI_DFG::getSetOfVertices()
{
  return _node_Set;
}

/*
 * return _ARC_Set
 */
vector<REGI_ARC*> REGI_DFG::getSetOfArcs()
{
  return _ARC_Set;
}

void REGI_DFG::Dump_Nodes(std::string filename)
{

  std::ofstream nodeFile;
  std::string graphname_node = filename;
  graphname_node.append("_node.txt");
  nodeFile.open(graphname_node.c_str());

  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    nodeFile << _node_Set[i]->get_ID() << "\t" << _node_Set[i]->get_Instruction() << "\n";
  }
  nodeFile.close();
}

void REGI_DFG::Dump_Edges(std::string filename)
{
  std::ofstream edgeFile;
  std::string graphname_edge = filename;
  graphname_edge.append("_edge.txt");
  edgeFile.open(graphname_edge.c_str());

  for (int i = 0; i < (int) _ARC_Set.size(); i++)
  {
    if(_ARC_Set[i]->get_Dependency()!=MemoryDep)
    {
      edgeFile << _ARC_Set[i]->get_From_Node()->get_ID() << "\t" << _ARC_Set[i]->get_To_Node()->get_ID()<<"\n";
    }
  }
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    if (_node_Set[i]->is_Load_Address_Generator() || _node_Set[i]->is_Store_Address_Generator())
    {
      edgeFile << _node_Set[i]->get_ID() << "\t" << _node_Set[i]->get_Related_Node()->get_ID()<<"\n";
    }
  }
  edgeFile.close();
}
/*
 * Print DFG in DOT format
 */
void REGI_DFG::Dot_Print_DFG(string filename, int ID)
{
  ofstream dotFile;
  ostringstream graphname;
  graphname << filename.c_str() << "_" << ID << ".dot";
  dotFile.open(graphname.str().c_str());

  dotFile << "digraph " << filename << " { \n{";

  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    //memory operations are blue nodes
    if (_node_Set[i]->is_Mem_Operation())
    {
      dotFile << "\n" << _node_Set[i]->get_ID() << " [color=blue ];\n";
    }
    else
    {

      REGI_Node_Dummy *tempDummyNode = dynamic_cast<REGI_Node_Dummy*>(_node_Set[i]);
      if (tempDummyNode != NULL)
      {
        //routing nodes are green
        dotFile << "\n" << _node_Set[i]->get_ID() << " [color=green ];\n";
      }
      else
      {
        //regular nodes are red
        dotFile << "\n" << _node_Set[i]->get_ID() << " [color=red ];\n";
      }
    }

    if (is_Reduced_Node(_node_Set[i]))
    {
      //reduced nodes are box shape
      dotFile << "\n" << _node_Set[i]->get_ID() << " [shape=box ];\n";
    }

  }
  for (int i = 0; i < (int) _ARC_Set.size(); i++)
  {
    switch (_ARC_Set[i]->get_Dependency())
    {
      case PredDep:
        if (_ARC_Set[i]->get_Distance() == 0)
        {
          dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=bold, color=blue ]\n";
        }
        else
        {
          dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=bold, color=blue, label=" << _ARC_Set[i]->get_Distance() << "] \n";
        }
        break;
      case TrueDep: 
        if (_ARC_Set[i]->get_Distance() == 0)
        {
          dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << "\n";
        }
        else
        {
          dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=bold, color=red, label=" << _ARC_Set[i]->get_Distance() << "] \n";
        }
        break;
      case LoadDep:
      case StoreDep:
        FATAL(_ARC_Set[i]->get_Distance()!=0, "How come load store dependency has distance?");
        dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=dotted, color=blue, label= mem] \n";
        break;
      case MemoryDep:
        dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=dotted, color=green, label=" << _ARC_Set[i]->get_Distance() << "] \n";
        break;
      default:
        FATAL(true, "What kind of dependency can it be?");
        break;
    }
  }

  dotFile << "\n}\n";
  dotFile << "\n}";
  dotFile.close();
}

/*
 * Print DFG with schedule in DOT format
 */
void REGI_DFG::Dot_Print_DFG_With_Schedule(string filename, int ID)
{
  ofstream dotFile;
  ostringstream graphname;
  graphname << filename << "_" << ID<<".dot";
  dotFile.open(graphname.str().c_str());

  dotFile << "digraph " << filename << " { \n{";

  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    if (_node_Set[i]->is_Mem_Operation())
    {
      dotFile << "\n" << _node_Set[i]->get_ID() << " [color=blue ];\n";
    }
    else
    {

      REGI_Node_Dummy *tempDummyNode = dynamic_cast<REGI_Node_Dummy*>(_node_Set[i]);
      if (tempDummyNode != NULL)
      {
        dotFile << "\n" << _node_Set[i]->get_ID() << " [color=green ];\n";
      }
      else
      {
        dotFile << "\n" << _node_Set[i]->get_ID() << " [color=red ];\n";

      }
    }
    if (is_Reduced_Node(_node_Set[i]))
    {
      dotFile << "\n" << _node_Set[i]->get_ID() << " [shape=box ];\n";
    }

  }
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    if (_node_Set[i]->has_self_loop())
    {
      dotFile << _node_Set[i]->get_ID() << " -> " << _node_Set[i]->get_ID() << " [style=bold, color=red, label=" << _node_Set[i]->get_self_loop()->get_Distance() << "] \n";
    }
  }
  for (int i = 0; i < (int) _ARC_Set.size(); i++)
  {

    switch (_ARC_Set[i]->get_Dependency())
    {
      case PredDep:
        if (_ARC_Set[i]->get_Distance() == 0)
        {
          dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=bold, color=blue ]\n";
        }
        else
        {
          dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=bold, color=blue, label=" << _ARC_Set[i]->get_Distance() << "] \n";
        }
        break;
      case TrueDep: 
        if (_ARC_Set[i]->get_Distance() == 0)
        {
          dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << "\n";
        }
        else
        {
          dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=bold, color=red, label=" << _ARC_Set[i]->get_Distance() << "] \n";
        }
        break;
      case LoadDep:
      case StoreDep:
        FATAL(_ARC_Set[i]->get_Distance()!=0, "How come load store dependency has distance?");
        dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=dotted, color=blue, label= mem] \n";
        break;
      case MemoryDep:
        dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=dotted, color=green, label=" << _ARC_Set[i]->get_Distance() << "] \n";
        break;
      default:
        FATAL(true, "What kind of dependency can it be?");
        break;
    }
  }

  dotFile << "\n}\n";

  dotFile << "\n{\n node [shape=plaintext]; \n T0 ";
  for (int i = 1; i < MAX_SCHEDULE_LEN + 1; i++)
  {

    dotFile << " -> T" << i;
  }
  dotFile << ";\n}";

  dotFile << "{ rank = source; \n";
  dotFile << "T" << 0 << ";";
  dotFile << "\n }; \n";

  bool any = true;
  int rank = 0;
  while (any || rank < MAX_SCHEDULE_LEN + 1)
  {

    any = false;
    dotFile << "{ rank = same; \n";
    for (int i = 0; i < (int) _node_Set.size(); i++)
    {
      if (_node_Set[i]->get_Sched_Info()->get_Current() == rank)
      {

        dotFile << _node_Set[i]->get_ID() << "; ";
        any = true;

      }
    }
    if (any)
    {
      dotFile << "T" << rank << ";";
    }
    dotFile << "\n }; \n";
    rank++;

  }
  dotFile << "\n}";
  dotFile.close();
}

/*
 * Print DFG with modulo schedule in DOT format
 */
void REGI_DFG::Dot_Print_DFG_With_Modulo_Schedule(string filename, int II)
{

  ofstream dotFile;
  string graphname = filename;
  filename.append("DFG.dot");
  dotFile.open(filename.c_str());

  dotFile << "digraph " << graphname << " { \n{";

  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    if (_node_Set[i]->is_Mem_Operation())
    {
      dotFile << "\n" << _node_Set[i]->get_ID() << " [color=blue ];\n";
    }
    else
    {

      REGI_Node_Dummy *tempDummyNode = dynamic_cast<REGI_Node_Dummy*>(_node_Set[i]);
      if (tempDummyNode != NULL)
      {
        dotFile << "\n" << _node_Set[i]->get_ID() << " [color=green ];\n";
      }
      else
      {
        dotFile << "\n" << _node_Set[i]->get_ID() << " [color=red ];\n";

      }
    }

  }
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    if (_node_Set[i]->has_self_loop())
    {
      dotFile << _node_Set[i]->get_ID() << " -> " << _node_Set[i]->get_ID() << " [style=bold, color=red, label=" << _node_Set[i]->get_self_loop()->get_Distance() << "] \n";
    }
  }
  for (int i = 0; i < (int) _ARC_Set.size(); i++)
  {
    switch (_ARC_Set[i]->get_Dependency())
    {
      case PredDep:
        if (_ARC_Set[i]->get_Distance() == 0)
        {
          dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=bold, color=blue ]\n";
        }
        else
        {
          dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=bold, color=blue, label=" << _ARC_Set[i]->get_Distance() << "] \n";
        }
        break;
      case TrueDep: 
        if (_ARC_Set[i]->get_Distance() == 0)
        {
          dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << "\n";
        }
        else
        {
          dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=bold, color=red, label=" << _ARC_Set[i]->get_Distance() << "] \n";
        }
        break;
      case LoadDep:
      case StoreDep:
        FATAL(_ARC_Set[i]->get_Distance()!=0, "How come load store dependency has distance?");
        dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=dotted, color=blue, label= mem] \n";
        break;
      case MemoryDep:
        dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=dotted, color=green, label=" << _ARC_Set[i]->get_Distance() << "] \n";
        break;
      default:
        FATAL(true, "What kind of dependency can it be?");
        break;
    }
  }

  dotFile << "\n}\n";

  dotFile << "\n{\n node [shape=plaintext]; \n T0 ";
  for (int i = 1; i < II; i++)
  {

    dotFile << " -> T" << i;
  }
  dotFile << ";\n}";

  bool any = true;
  int rank = 0;
  while (any)
  {

    any = false;
    dotFile << "{ rank = same; \n";
    for (int i = 0; i < (int) _node_Set.size(); i++)
    {
      if (_node_Set[i]->get_Sched_Info()->get_Modulo_Current() == rank)
      {

        dotFile << _node_Set[i]->get_ID() << "; ";
        any = true;

      }
    }
    if (any)
    {
      dotFile << "T" << rank << ";";
    }
    dotFile << "\n }; \n";
    rank++;

  }
  dotFile << "\n}";
  dotFile.close();

}

/*
 * print the set of sets
 */
void REGI_DFG::PrintSets(vector<vector<REGI_Node*> > &sets)
{
  printf("\nCurrent Partition\n");
  for (int i = 0; i < (int) sets.size(); i++)
  {
    PrintSet(sets[i]);
    printf("\n");
  }
  printf("\n");
}

/*
 * return the size of _node_Set
 */
int REGI_DFG::get_number_of_nodes()
{
  return (int) _node_Set.size();
}

/*
 * returns the number of load operations
 */
int REGI_DFG::Count_Load_Operations()
{
  int retval = 0;
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    if (_node_Set[i]->is_Mem_Operation() && _node_Set[i]->is_Load_Data_Bus_Read())
    {
      retval++;
    }
  }
  return retval;
}

/*
 * returns the number of store operations
 */
int REGI_DFG::Count_Store_Operations()
{
  int retval = 0;
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    if (_node_Set[i]->is_Mem_Operation() && _node_Set[i]->is_Store_Data_Bus_Write())
    {
      retval++;
    }
  }
  return retval;
}

/*
 * print nodes in the set
 */
void REGI_DFG::PrintSet(vector<REGI_Node*> &set)
{
  vector<REGI_Node*>::iterator iNode;
  for (iNode = set.begin(); iNode != set.end(); iNode++)
  {
    printf("%d ,", (*iNode)->get_ID());
  }
}

/*
 * return the set of children of set nodes, all from the same iteration as nodes
 */
vector<REGI_Node*> REGI_DFG::get_next_nodes_same_iteration(vector<REGI_Node*> &nodes)
{
  vector<REGI_Node*> retval;
  vector<REGI_Node*> current_next;
  for (int i = 0; i < (int) nodes.size(); i++)
  {
    current_next = nodes[i]->Get_Next_Nodes_Same_Iteration();
    retval = Union(current_next, retval);
  }
  return retval;
}

/*
 * return the set of children of set nodes, consider latency of operations
 */
vector<REGI_Node*> REGI_DFG::get_next_nodes_same_iteration_update_latency(vector<REGI_Node*> &nodes, bool &change)
{
  vector<REGI_Node*> retval;
  vector<REGI_Node*> current_next;
  int next_latency;
  for (int i = 0; i < (int) nodes.size(); i++)
  {
    current_next = nodes[i]->Get_Next_Nodes_Same_Iteration();
    next_latency = nodes[i]->get_Index_Cycle() + nodes[i]->get_Latency();
    for (int j = 0; j < (int) current_next.size(); j++)
    {
      if (current_next[j]->Is_Index_Defined_Cycle())
      {
        if (next_latency > (current_next[j]->get_Index_Cycle()))
        {
          change = true;
          current_next[j]->Set_Index_Cycle(next_latency);
        }
      }
      else
      {
        current_next[j]->Set_Index_Cycle(next_latency);
      }
    }
    retval = Union(current_next, retval);
  }
  return retval;
}

/*
 * reset cycles of the nodes
 */
void REGI_DFG::reset_latency_to_nodes(vector<REGI_Node*> &nodes)
{
  for (int i = 0; i < (int) nodes.size(); i++)
  {
    nodes[i]->Reset_Cycle_Index();
  }
}

/*
 * returns set of nodes with no parent from current cycle
 * not a good function
 */
vector<REGI_Node*> REGI_DFG::get_set_of_start_nodes()
{
  vector<REGI_Node*> retval;
  vector<REGI_Node*> prev_nodes;
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    prev_nodes = _node_Set[i]->Get_Prev_Nodes_Same_Iteration();
    if ((int) prev_nodes.size() == 0)
    {
      retval.push_back(_node_Set[i]);
    }
  }
  return retval;
}

/*
 * find a set of nodes without parents in same iteration
 * FIXME 20-02-2014 load store nodes are removed
 */

vector<REGI_Node*> REGI_DFG::get_set_of_start_nodes_constraint_scheduling()
{
  vector<REGI_Node*> stage_1;
  vector<REGI_Node*> prev_nodes;
  //find set of nodes without parent in current iteration
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    prev_nodes = _node_Set[i]->Get_Prev_Nodes_Same_Iteration();
    if ((int) prev_nodes.size() == 0)
    {
      stage_1.push_back(_node_Set[i]);
    }
  }
  return stage_1;
}

/*
 * find a set of nodes without any child at current iteration
 * FIXME 20-02-2014 load store nodes are removed
 */
vector<REGI_Node*> REGI_DFG::get_set_of_end_nodes_constraint_scheduling()
{
  vector<REGI_Node*> stage_1;
  vector<REGI_Node*> next_nodes;
  //find set of nodes with no child in the same iteration
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    next_nodes = _node_Set[i]->Get_Next_Nodes_Same_Iteration();
    if ((int) next_nodes.size() == 0)
    {
      stage_1.push_back(_node_Set[i]);
    }
  }
  return stage_1;
}
/*
 * return a set of nodes without any child in current iteration
 * not a good function
 */
vector<REGI_Node*> REGI_DFG::get_set_of_end_nodes()
{
  vector<REGI_Node*> retval;
  vector<REGI_Node*> next_nodes;
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    next_nodes = _node_Set[i]->Get_Next_Nodes_Same_Iteration();
    if ((int) next_nodes.size() == 0)
    {
      retval.push_back(_node_Set[i]);
    }
  }
  return retval;
}

/*
 * limits the number of inputs to an operation to 2
 * FIXME: function collecting predecessors is updated 20-2-2014
 */
void REGI_DFG::Apply_Indegree_Constraint()
{

  //current node to check contraint
  REGI_Node* startNode;

  //for all nodes
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    startNode = _node_Set[i];
    //ignore phi operations
    if (startNode->is_PHI_Operations())
      continue;
    //if number of operands is > architectural specification
    //
    FATAL(startNode->get_Number_of_Operands() > cgra_info.MAX_NODE_INDEGREE, "Should never reach here, the number of operands should always respect architecture scpecification");
  }
}

/*
 * limits the number of outputs of an operation to architectural specifications
 * FIXME: function collecting predecessors is updated 20-2-2014
 * Predicated nodes need special attention
 * predicate dependency successors and true dependency successors received different fix step
 */
void REGI_DFG::Apply_Outdegree_Constraint()
{

  REGI_Node_Dummy* newnode;
  REGI_Node* startNode;
  vector<REGI_Node*> nextnodes;
  vector<REGI_Node*> next_true_dep_nodes;
  vector<REGI_Node*> next_predicate_dep_nodes;
  vector<REGI_Node*>::iterator iNode1;
  int count;
  /*
   * check all nodes
   */
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    /*
     * select a node
     */
    startNode = _node_Set[i];
    /*
     * get the set of next nodes
     * we do not care about inter-iteration dependencies which pass though registers fixed 20-2-2014
     */
    next_true_dep_nodes = startNode->Get_True_Dependency_Successors_Same_Iteration();

    next_predicate_dep_nodes = startNode->Get_Predicate_Dependency_Successors_Same_Iteration();

    /*
     * if outdegree > X (given by arch specifications
     */
    if ((int)next_true_dep_nodes.size() > cgra_info.MAX_NODE_OUTDEGREE)
    {
      nextnodes = next_true_dep_nodes;
      /*
       * add a routing node
       */
      newnode = new REGI_Node_Dummy(1, Get_Unique_Index(), startNode);


      if ((int)newnode->get_Number_of_Pred()>0)
      {
        Add_Arc(newnode->get_Pred_Arc(0));
      }
      /*
       * if this node is predicated, so should be the routing node
       */
      if (startNode->isPredicated())
      {
        //we need to start from the first index because we increased another nodes outdegree 
        i = 0;

      }
      /*
       * add it to DFG
       */
      //convert startnode -> nextnodes[0] to startnode -> newnode -> nextnodes[0]
      for (iNode1 = nextnodes.begin(); iNode1 != nextnodes.end(); iNode1++)
      {
        insert_Node_in_between_output(startNode, *iNode1, newnode);
        break;
      }
      nextnodes = startNode->Get_True_Dependency_Successors_Same_Iteration();
      count = 0;

      //convert startnode -> nextnodes[i] to newnode -> nextnodes[i]
      for (iNode1 = nextnodes.begin(); iNode1 != nextnodes.end(); iNode1++)
      {
        if ((*iNode1)->get_ID() == newnode->get_ID())
          continue;
        REGI_ARC *arc = get_Arc(startNode, *iNode1);
        int distance = arc->get_Distance();

        Remove_Arc(startNode, *iNode1);
        make_Arc(newnode, *iNode1, ARC_Max_Index++, distance, arc->get_Dependency());

        count++;
        //outdegree problem resolved?
        if (count == (int) nextnodes.size() - cgra_info.MAX_NODE_OUTDEGREE)
        {
          break;
        }
      }
    }
    /*
     * if outdegree > X (given by arch specifications
     */
    if ((int)next_predicate_dep_nodes.size() > cgra_info.MAX_NODE_OUTDEGREE)
    {
      nextnodes = next_predicate_dep_nodes;
      /*
       * add a routing node
       */
      newnode = new REGI_Node_Dummy(1, Get_Unique_Index(), startNode);

      if ((int)newnode->get_Number_of_Pred()>0)
      {
        Add_Arc(newnode->get_Pred_Arc(0));
      }

      /*
       * if this node is predicated, so should be the routing node
       */
      if (startNode->isPredicated())
      {
        //we need to start from the first index because we increased another nodes outdegree 
        i = 0;
      }
      /*
       * add it to DFG
       */
      //convert startnode -> nextnodes[0] to startnode -> newnode -> nextnodes[0]
      for (iNode1 = nextnodes.begin(); iNode1 != nextnodes.end(); iNode1++)
      {
        insert_Node_in_between_output(startNode, *iNode1, newnode);
        break;
      }
      nextnodes = startNode->Get_Predicate_Dependency_Successors_Same_Iteration();
      count = 0;

      //convert startnode -> nextnodes[i] to newnode -> nextnodes[i]
      for (iNode1 = nextnodes.begin(); iNode1 != nextnodes.end(); iNode1++)
      {
        if ((*iNode1)->get_ID() == newnode->get_ID())
          continue;
        REGI_ARC *arc = get_Arc(startNode, *iNode1);
        int distance = arc->get_Distance();

        Remove_Arc(startNode, *iNode1);
        make_Arc(newnode, *iNode1, ARC_Max_Index++, distance, arc->get_Dependency());

        count++;
        //outdegree problem resolved?
        if (count == (int) nextnodes.size() - cgra_info.MAX_NODE_OUTDEGREE)
        {
          break;
        }
      }
    }
  }
}

/*
 * this function tries to minimize register requirement for inter iteration data dependencies
 * for a node with multiple children in next iterations, find a child with minimal distance
 * make a routing node, and forward the dependencies of other children to this node
 */
void REGI_DFG::Apply_Outdegree_Inter_Iteration_Constraint()
{

  REGI_Node_Dummy* newnode;
  REGI_Node* startNode;
  REGI_Node* selected_node;
  vector<REGI_Node*> nextnodes;
  vector<REGI_Node*>::iterator iNode1;
  //for all nodes
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    startNode = _node_Set[i];
    //if selected node has > 1 child in next iteration(s)
    if ((int) startNode->Get_Next_Nodes_Next_Iteration().size() > 1)
    {
      //get ser of children in next iterations
      nextnodes = startNode->Get_Next_Nodes_Next_Iteration();
      int min_distance = 10000;
      int current_distance;
      //find the one with minimum inter iteration distance
      for (iNode1 = nextnodes.begin(); iNode1 != nextnodes.end(); iNode1++)
      {
        current_distance = get_Arc(startNode, *iNode1)->get_Distance();
        if (current_distance < min_distance)
        {
          min_distance = current_distance;
          selected_node = *iNode1;
        }
        break;

      }
      //create a new routing node
      newnode = new REGI_Node_Dummy(1, Get_Unique_Index(), startNode);

      insert_Node(newnode);

      if ((int)newnode->get_Number_of_Pred()>0)
      {
        Add_Arc(newnode->get_Pred_Arc(0));
      }


      //insert it between current node and its child that has the minimum inter iteration distance
      insert_Node_in_between_input(startNode, selected_node, newnode);

      /*
       * todo July 7,2013 these line are commented because they do not sound right
       */
      /*for (iNode1 = nextnodes.begin(); iNode1 != nextnodes.end(); iNode1++)
        {

        insert_Node_in_between_input(startNode, *iNode1, newnode);
        break;

        }*/
      //forward the other dependecies to the new node
      nextnodes = startNode->Get_Next_Nodes_Next_Iteration();
      for (iNode1 = nextnodes.begin(); iNode1 != nextnodes.end(); iNode1++)
      {
        if ((*iNode1)->get_ID() == newnode->get_ID())
          continue;
        REGI_ARC *arc = get_Arc(startNode, *iNode1);
        int distance = arc->get_Distance();

        Remove_Arc(startNode, *iNode1);
        /*
         * todo
         * make sure the new distance is right!
         */
        make_Arc(newnode, *iNode1, ARC_Max_Index++, distance - min_distance, arc->get_Dependency());
      }
    }
  }
}

/*
 * Preprocess the DFG
 */
void REGI_DFG::PreProcess()
{
  /*
   * limit the number of incoming arcs to nodes to 2
   * for predicated operations though, it can be 3
   */
  Apply_Indegree_Constraint();
  /*
   * Limit the number of outgoing arcs to node to X, X should come from architecture description
   */
  Apply_Outdegree_Constraint();
  /*
   * Minimize register requirements for inter iteration dependencies
   */
  Apply_Outdegree_Inter_Iteration_Constraint();
}

/*
 * performs some preliminary scheduling in DFG and returns MII
 */
int REGI_DFG::Init_Schedule(int id, int number_of_resources)
{
  vector < vector<REGI_Node*> > Sccs;
  /*
   * find strongly connected sets
   */
  Strongly_Connected (Sccs);

  DEBUG_SCHEDULING("Size of SCCs is %d", (int ) Sccs.size());

  //PrintSets(Sccs);
  int recMII_index;

  /*
   * find recMII
   */
  int recMII = RecMII_Scc(Sccs, recMII_index);

  DEBUG_SCHEDULING("recMII is %d", recMII);

  /*
   * find resMII
   */
  int resMII = (_node_Set.size() - 1) / number_of_resources;
  resMII++;

  DEBUG_SCHEDULING("resMII without mem operations is %d", resMII);
  /*
   * find memory resource MII
   */
  int MemII = (Count_Load_Operations() * 2 + Count_Store_Operations() - 1) / CGRA_Y_Dim;

  MemII++;

  if (MemII > resMII)
  {
    resMII = MemII;
  }

  DEBUG_SCHEDULING("resMII without mem operations is %d", resMII);

  /*
   * ASAP Schedule
   */
  int last_time = Schedule_ASAP();

  DEBUG_DOTPRINT_ASAP_SCHEDULE("ASAP" , id);

  /*
   * ALAP Schedule
   */
  Schedule_ALAP(last_time);

  DEBUG_DOTPRINT_ALAP_SCHEDULE("ALAP" , id);

  /*
   * Feasible ASAP Schedule
   */
  last_time = Schedule_ASAP_Feasible(number_of_resources);

  MAX_SCHEDULE_LEN = last_time;

  DEBUG_DOTPRINT_FEASIBLE_ASAP_SCHEDULE("Feasible_ASAP" , id);

  /*
   * Feasbile ALAP Schedule
   */
  Schedule_ALAP_Feasible(last_time, number_of_resources);

  DEBUG_DOTPRINT_FEASIBLE_ALAP_SCHEDULE("Feasible_ALAP" , id);

  int MII = recMII;

  if (resMII > MII)
  {
    MII = resMII;
  }
  //return MII
  return MII;
}

/*
 * schedule the DFG
 * id is the schedule attempt ID
 * number of resources is the maximum number of operations at permitted any cycles
 * II is initiation inetrval
 */
int REGI_DFG::Schedule(int id, int number_of_resources, int II)
{

  /*
   * find current resMII relative to available resources
   */
  int resMII = (_node_Set.size() - 1) / number_of_resources;
  resMII++;

  //printf("resMII is %d\n", resMII);
  /*
   * current resMII > current II
   */
  if (resMII > II)
    return resMII;
  /*
   * ASAP Schedule
   */
  int last_time = Schedule_ASAP();

  DEBUG_DOTPRINT_ASAP_SCHEDULE("ASAP" , id);

  /*
   * ALAP Schedule
   */
  Schedule_ALAP(last_time);

  DEBUG_DOTPRINT_ALAP_SCHEDULE("ALAP" , id);

  /*
   * Feasible ASAP Schedule
   */
  last_time = Schedule_ASAP_Feasible(number_of_resources);

  MAX_SCHEDULE_LEN = last_time;

  DEBUG_DOTPRINT_FEASIBLE_ASAP_SCHEDULE("Feasible_ASAP" , id);

  /*
   * Feasbile ALAP Schedule
   */
  if (!Schedule_ALAP_Feasible(last_time, number_of_resources))
  {
    return -1;
  }

  DEBUG_DOTPRINT_FEASIBLE_ALAP_SCHEDULE("Feasible_ALAP" , id);

  bool redo = false;
  while (true)
  {
    /*
     * find a feasible modulo schedule
     */
    if (Modulo_Schedule_Smart_2(II, last_time, redo, number_of_resources))
    {

      schedule_len = last_time;

      DEBUG_SCHEDULING("Feasible II is %d", II);
      DEBUG_DOTPRINT_CURRENT_SCHEDULE("Ready" , id);

      return II;
    }
    /*
     * not valid schedule is found, reset current schedule
     */
    Reset_Current_Schedule();
    if (redo)
    {

      /*
       * reset all schedules and start from feasible scheduling ASAP and ALAP
       */
      Reset_ALAP_Feasible_Schedule();
      Reset_ASAP_Feasible_Schedule();

      last_time = Schedule_ASAP_Feasible(number_of_resources);
      MAX_SCHEDULE_LEN = last_time;

      /*
       * can find any feasbile ALAP schedule?
       */
      if (!Schedule_ALAP_Feasible(last_time, number_of_resources))
      {
        return -1;
      }

      DEBUG_DOTPRINT_FEASIBLE_ALAP_SCHEDULE("Feasible_ALAP" , id);
      DEBUG_SCHEDULING("Multiple Constraint %d\n", II);
    }
    /*
     * no valid schedule is found
     */
    else
    {
      return -1;
    }

  }
  /*
   * should never reach here
   */
  FATAL(true, "How did we get here?");
  return -1;
}

/*
 * todo
 * find a better reschedule
 */
int REGI_DFG::Modulo_ReSchedule_Smart_2(int id, int number_of_resources, int II, vector<REGI_Node*> &to_change_set)
{
  bool redo = false;
  int success = 0;
  int last_time;
  //Dot_Print_DFG_With_Schedule("Reschedule");
  vector<REGI_Node*> reschedule;
  vector<int> resched_time;
  for (int i = 0; i < (int) to_change_set.size(); i++)
  {
    if (to_change_set[i]->get_physical_predecessors_constrain_this_modulo().size() > 0)
    {
      //cout << "PREV " << to_change_set[i]->get_ID() << endl;
      vector<REGI_Node*> prev = to_change_set[i]->get_physical_predecessors_constrain_this_modulo();
      for (int j = 0; j < (int) prev.size(); j++)
      {
        REGI_Node_Dummy* tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index(), prev[j]);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }

        insert_Node(tempDummyNode);
        insert_Node_in_between_input(prev[j], to_change_set[i], tempDummyNode);
      }
      success = 1;
      //cout << "PREV done" << endl;

      continue;
      //cout << "Success" << endl;
    }
    if (to_change_set[i]->get_physical_successors_constrain_by_this_modulo().size() > 0)
    {
      //cout << "Next" << to_change_set[i]->get_ID() << endl;
      vector<REGI_Node*> next = to_change_set[i]->get_physical_successors_constrain_by_this_modulo();
      for (int j = 0; j < (int) next.size(); j++)
      {
        REGI_Node_Dummy* tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index(), to_change_set[i]);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }


        insert_Node(tempDummyNode);
        insert_Node_in_between_input(to_change_set[i], next[j], tempDummyNode);
      }
      success = 1;
      //cout << "Next done" << endl;
      continue;

    }
    if ((to_change_set[i]->get_Sched_Info()->get_Feasible_ALAP() - to_change_set[i]->get_Sched_Info()->get_Feasible_ASAP()) > 0)
    {
      //cout << "resched back" << to_change_set[i]->get_ID() << endl;
      reschedule.push_back(to_change_set[i]);
      resched_time.push_back(to_change_set[i]->get_Sched_Info()->get_Feasible_ALAP() - 1);
      success = 1;
    }
  }
  DEBUG_RESCHEDULE("Rescheduling completed with %d",success);
  if (success == 1)
  {

    Reset_Current_Schedule();
    Reset_ALAP_Feasible_Schedule();
    Reset_ASAP_Feasible_Schedule();
    last_time = Schedule_ASAP_Feasible(number_of_resources);
    if (!Schedule_ALAP_Feasible(last_time, number_of_resources))
    {
      return 0;
    }

    MAX_SCHEDULE_LEN = last_time;
    int alap_time;
    while (true)
    {
      //cout << "apply time change" << endl;

      DEBUG_RESCHEDULE("Rescheduling changes ALAPs");
      for (int i = 0; i < (int) reschedule.size(); i++)
      {
        alap_time = resched_time[i];
        if (reschedule[i]->get_Sched_Info()->get_Feasible_ALAP() < alap_time)
        {
          alap_time = reschedule[i]->get_Sched_Info()->get_Feasible_ALAP();
        }
        reschedule[i]->get_Sched_Info()->set_Feasible_ALAP(alap_time);
      }
      //cout << "apply time done" << endl;
      if (Modulo_Schedule_Smart_2(II, last_time, redo, number_of_resources))
      {

        DEBUG_RESCHEDULE("Rescheduling succeed");
        if (DEBUG_4)
        {
          printf("Feasible II is %d\n", II);
        }
        //Dot_Print_DFG_With_Modulo_Schedule("Modulo_complete" + id, II);
        //Dot_Print_DFG_With_Schedule("Modulo" + id);
        return 1;
      }
      if (redo)
      {
        DEBUG_RESCHEDULE("Rescheduling failed");
        Reset_ALAP_Feasible_Schedule();
        Reset_ASAP_Feasible_Schedule();
        //Reset_ASAP_Schedule();
        //Reset_ALAP_Schedule();
        //last_time = Schedule_ASAP();
        //Schedule_ALAP(last_time);
        last_time = Schedule_ASAP_Feasible(number_of_resources);
        if (!Schedule_ALAP_Feasible(last_time, number_of_resources))
        {
          return 0;
        }
        MAX_SCHEDULE_LEN = last_time;
        Reset_Current_Schedule();
      }
      else
      {
        return 0;
      }
    }
  }
  return 0;

}

/*
 * returns recMII
 */
int REGI_DFG::RecMII_Scc(vector<vector<REGI_Node*> > &sets, int &max_index)
{
  int delay;
  int recMII = 0;
  int II;
  //find the longest path among SCCs
  for (int i = 0; i < (int) sets.size(); i++)
  {
    delay = Longest_Path(sets[i], II);
    delay = (delay - 1) / II;
    if (recMII < delay + 1)
    {
      recMII = delay + 1;
      max_index = i;
    }
  }
  return recMII;
}

/*
 * find longest path for SCCs
 */
int REGI_DFG::Longest_Path(vector<REGI_Node*> &nodes, int &II)
{
  REGI_Node* start_node;
  II = 1;
  vector<REGI_Node*> visited;
  vector<REGI_Node*> next_nodes;
  int longest_path = 1;
  bool root_node = false;
  for (int i = 0; i < (int) nodes.size(); i++)
  {
    start_node = nodes[i];

    for (int j = 0; j < start_node->get_Number_of_Pred(); j++)
    {
      if (start_node->get_Pred_Arc(j)->get_Distance() > 0)
      {
        root_node = true;
        break;
      }
    }
    if (!root_node)
    {
      continue;
    }

    reset_latency_to_nodes(nodes);
    start_node = nodes[i];
    start_node->Set_Index_Cycle(0);

    visited.push_back(start_node);
    bool change = true;
    int prev_size = 0;
    while (change)
    {
      change = false;
      prev_size = (int) visited.size();
      next_nodes = get_next_nodes_same_iteration_update_latency(visited, change);
      next_nodes = Intersect(nodes, next_nodes);
      visited = Union(next_nodes, visited);
      if ((int) visited.size() > prev_size)
      {
        change = true;
      }
    }
    for (int j = 0; j < (int) visited.size(); j++)
    {
      if (start_node == visited[j])
        continue;
      if (visited[j]->is_Connected_To(start_node))
      {
        if (get_Arc(visited[j], start_node)->get_Distance() > 0)
        {
          if (visited[j]->get_Index_Cycle() + visited[j]->get_Latency() > longest_path)
          {
            longest_path = visited[j]->get_Index_Cycle() + visited[j]->get_Latency();
            II = get_Arc(visited[j], start_node)->get_Distance();
          }
        }
      }
    }

  }
  return longest_path;
}

/*
 * returns schedule length
 */
int REGI_DFG::Get_Schedule_Length()
{
  return schedule_len;
}

/*
 * sets schedule length
 */
void REGI_DFG::Set_Schedule_Length(int len)
{
  schedule_len = len;
}

/*
 * Schedule operations as soon as all predecessors are completed
 * FIXME: 20-2-2014
 */
int REGI_DFG::Schedule_ASAP()
{

  DEBUG_SCHEDULING("ASAP Started");
  //set of scheduled operations
  vector<REGI_Node*> scheduled;
  //set of next nodes to schedule
  vector<REGI_Node*> to_be_scheduled;
  //set of nodes not scheduled yet
  vector<REGI_Node*> rest = _node_Set;
  //first select all nodes with no predecessor in current iteration in DFG
  to_be_scheduled = get_set_of_start_nodes();
  //start from cycle 0
  int schedule_time = 0;
  int latest_time = 0;
  //nodes without predecessor are scheduled at cycle 0
  for (int i = 0; i < (int) to_be_scheduled.size(); i++)
  {
    to_be_scheduled[i]->get_Sched_Info()->set_ASAP(0);
    DEBUG_ASAP_SCHEDULING("Node %d is scheduled at time %d", to_be_scheduled[i]->get_ID(), 0);
  }
  //update set of scheduled nodes
  scheduled = Union(scheduled, to_be_scheduled);
  //update set of unscheduled nodes
  rest = Subtract(rest, scheduled);
  //while there is an scheduled node
  while ((int) rest.size() > 0)
  {
    //for all un scheduled node
    for (int i = 0; i < (int) rest.size(); i++)
    {
      //is it ready to be scheduled? if yes, at what cycle?(schedule_time)
      if (rest[i]->is_ready_to_schedule_ASAP(schedule_time))
      {
        //set ASAP schedule for the node
        rest[i]->get_Sched_Info()->set_ASAP(schedule_time);
        DEBUG_ASAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_ID(), schedule_time);
        //update set of scheduled operations
        scheduled.push_back(rest[i]);
        //if this node scheduled as the latest cycle in DFG? if yes, this node determines schedule length
        if (schedule_time > latest_time)
        {
          latest_time = schedule_time;
        }
      }
    }
    //update set of unscheduled operations
    rest = Subtract(rest, scheduled);
  }
  DEBUG_SCHEDULING("ASAP Finished");
  //return schedule length
  return latest_time;
}
/*
 * schedule operations in ASAP manner considering number of available resources
 * change July 13, 2013
 * Instead of Memory Resource, Address BUS AND DATA BUS
 * FIXME: 20-2-2014
 */
int REGI_DFG::Schedule_ASAP_Feasible(int number_of_resources)
{
  int* Total_Resources;
  ;
  //int* Memory_Resources;
  int *Address_BUS;
  int *Data_BUS;

  //total number of PEs at each cycle
  Total_Resources = new int[MAPPING_POLICY.MAX_LATENCY];
  //total number of memory bus at each cycle
  Address_BUS = new int[MAPPING_POLICY.MAX_LATENCY];
  Data_BUS = new int[MAPPING_POLICY.MAX_LATENCY];

  //initialize used resources to 0
  for (int i = 0; i < MAPPING_POLICY.MAX_LATENCY; i++)
  {
    Total_Resources[i] = 0;
    Address_BUS[i] = 0;
    Data_BUS[i] = 0;
  }
  DEBUG_SCHEDULING("Feasbile ASAP Started");
  //set of scheduled operations
  vector<REGI_Node*> scheduled;
  //set of operations to be scheduled
  vector<REGI_Node*> to_be_scheduled;

  vector<REGI_Node*> temp;
  vector<REGI_Node*> temp_2;
  //set of operations not scheduled yet
  vector<REGI_Node*> rest = _node_Set;
  /*
   * initially try to schedule operations that do not have predecessor in this iteration
   * if both operations for a load are candidate to be scheduled,
   * only load address assert and store data assert are present in this set
   */
  to_be_scheduled = get_set_of_start_nodes_constraint_scheduling();

  int schedule_time = 0;
  int schedule_time_2 = 0;
  int latest_time = 0;

  bool change;

  DEBUG_FEASIBLE_ASAP_SCHEDULING("FEASIBLE Init nodes Start");
  //first schedule selected nodes
  for (int i = 0; i < (int) to_be_scheduled.size(); i++)
  {
    //if selected node is load address assert
    if (to_be_scheduled[i]->is_Load_Address_Generator())
    {
      //find a time slot with enough resources to schedule
      for (int j = 0; j < MAPPING_POLICY.MAX_LATENCY; j++)
      {
        //enough PE?
        if (Total_Resources[j] >= number_of_resources)
        {
          continue;
        }
        //any conflict with a node connected with arc with distance > 1
        if (has_Node_Conflict_Feasible_ASAP(to_be_scheduled[i], j))
        {
          continue;
        }
        //any node conflict between load data read and connected nodes to it with arc distance > 1
        if (has_Node_Conflict_Feasible_ASAP(to_be_scheduled[i]->get_Related_Node(), j+1))
        {
          continue;
        }
        //enough memory resource at this cycle and next when data would be available, enough PE available next cycle?
        if (HAS_ENOUGH_RESOURCES_FOR_LOAD_INSTRUCTION_AT_CYCLE(j, j + 1, Address_BUS, Data_BUS, Total_Resources, number_of_resources))
        {
          //if so, allocate those resources
          Address_BUS[j]++;
          Data_BUS[j + 1]++;
          Total_Resources[j]++;
          Total_Resources[j + 1]++;
          //schedule operations
          to_be_scheduled[i]->get_Sched_Info()->set_Feasible_ASAP(j);
          to_be_scheduled[i]->get_Related_Node()->get_Sched_Info()->set_Feasible_ASAP(j + 1);
          //if related node is present
          temp.push_back(to_be_scheduled[i]->get_Related_Node());

          FATAL(contains(to_be_scheduled, to_be_scheduled[i]->get_Related_Node()), "ERROR, the related load node cannot be in schedule list");
          DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is scheduled at time %d", to_be_scheduled[i]->get_ID(), j);
          DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is scheduled at time %d", to_be_scheduled[i]->get_Related_Node()->get_ID(), j + 1);
          //successfully scheduled an operation
          break;
        }
      }
    }
    //if selected node is store address assert
    else if (to_be_scheduled[i]->is_Store_Address_Generator())
    {
      //find a time slot for this node
      for (int j = 0; j < MAPPING_POLICY.MAX_LATENCY; j++)
      {
        //enough PE?
        if (Total_Resources[j] >= number_of_resources)
        {
          continue;
        }
        //conflict with a node connected with arc distance > 1
        if (has_Node_Conflict_Feasible_ASAP(to_be_scheduled[i], j))
        {
          continue;
        }
        //conflict between store address assert and nodes connected to it with distance > 1
        if (has_Node_Conflict_Feasible_ASAP(to_be_scheduled[i]->get_Related_Node(), j))
        {
          continue;
        }
        //there is enough PE resources? and memory bus?
        if (HAS_ENOUGH_RESOURCES_FOR_STORE_INSTRUCTION_AT_CYCLE(j, Address_BUS, Data_BUS, Total_Resources, number_of_resources))
        {
          //allocate resources
          Address_BUS[j]++;
          Data_BUS[j]++;
          Total_Resources[j] = Total_Resources[j] + 2;
          //schedule operations
          to_be_scheduled[i]->get_Sched_Info()->set_Feasible_ASAP(j);
          to_be_scheduled[i]->get_Related_Node()->get_Sched_Info()->set_Feasible_ASAP(j);

          temp.push_back(to_be_scheduled[i]->get_Related_Node());
          FATAL(contains(to_be_scheduled, to_be_scheduled[i]->get_Related_Node()), "ERROR, the related store node cannot be in schedule list");
          DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is scheduled at time %d", to_be_scheduled[i]->get_ID(), j);
          DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is scheduled at time %d", to_be_scheduled[i]->get_Related_Node()->get_ID(), j);
          //successfully scheduled operations
          break;
        }
      }
    }
    //regular node to be scheduled
    else
    {
      //find a time slot
      for (int j = 0; j < MAPPING_POLICY.MAX_LATENCY; j++)
      {
        //enough PE available?
        if (Total_Resources[j] >= number_of_resources)
        {
          continue;
        }
        //has conflict with a node connected with an arc with distance > 1
        if (has_Node_Conflict_Feasible_ASAP(to_be_scheduled[i], j))
        {
          continue;
        }
        //bug fixed, 	Total_Resources[0-> j]++;
        //allocate resource
        Total_Resources[j]++;

        //bug fixed, to_be_scheduled[i]->get_Sched_Info()->set_Feasible_ASAP(0->j);
        //schedule the node
        to_be_scheduled[i]->get_Sched_Info()->set_Feasible_ASAP(j);

        DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is scheduled at time %d", to_be_scheduled[i]->get_ID(), j);
        //successfully schedule the operations
        break;
      }

    }
  }
  DEBUG_FEASIBLE_ASAP_SCHEDULING("FEASIBLE Init nodes Finished");

  //update set of scheduled nodes
  scheduled = Union(temp, to_be_scheduled);
  //update set of unscheduled nodes
  rest = Subtract(rest, scheduled);

  bool bug=false;
  //while there is an unscheduled node
  while ((int) rest.size() > 0)
  {
    //for all nodes
    for (int i = 0; i < (int) rest.size(); i++)
    {
      //any new node schedule? not yet
      change = false;
      //is this node ready to be scheduled? if yes, earliest time?
      if (rest[i]->is_ready_to_schedule_Feasible_ASAP(schedule_time))
      {
        //find the earliest time slot with enough resources
        for (int time = schedule_time; time < MAPPING_POLICY.MAX_LATENCY; time++)
        {
          DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is to be scheduled at time %d from %d", rest[i]->get_ID(), time,MAPPING_POLICY.MAX_LATENCY);
          //enough PE?
          if (Total_Resources[time] < number_of_resources)
          {
            DEBUG_FEASIBLE_ASAP_SCHEDULING("Enough Resources");
            //if node is load address generator
            if (rest[i]->is_Load_Address_Generator())
            {
              //is related node ready to be scheduled? if yes, at what cycle?
              if (rest[i]->get_Related_Node()->is_ready_to_schedule_Feasible_ASAP(schedule_time_2))
              {
                //if related node is ready but at some cycle greater than current time slot
                if (schedule_time_2 > time)
                {
                  //update time slot to schedule the operation
                  //node address should be scheduled at one cycle earlier than load data read
                  time = schedule_time_2 - 2;
                  continue;
                }
                //does node have any conflict with any already schedule node
                if (has_Node_Conflict_Feasible_ASAP(rest[i], time))
                {
                  continue;
                }
                //does load data read has any conflict with scheduled nodes
                if (has_Node_Conflict_Feasible_ASAP(rest[i]->get_Related_Node(), time + 1))
                {
                  continue;
                }
                //is there enough resources available at time and time+1, memory and PE
                if (HAS_ENOUGH_RESOURCES_FOR_LOAD_INSTRUCTION_AT_CYCLE(time, time + 1, Address_BUS, Data_BUS, Total_Resources, number_of_resources))
                {
                  //allocate resources
                  Address_BUS[time]++;
                  Data_BUS[time + 1]++;
                  Total_Resources[time]++;
                  Total_Resources[time + 1]++;
                  //schedule operations
                  rest[i]->get_Sched_Info()->set_Feasible_ASAP(time);
                  rest[i]->get_Related_Node()->get_Sched_Info()->set_Feasible_ASAP(time + 1);
                  //update set of scheduled operations
                  scheduled.push_back(rest[i]->get_Related_Node());
                  scheduled.push_back(rest[i]);
                  //there was a change in schedule
                  change = true;
                  DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_ID(), time);
                  DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_Related_Node()->get_ID(), time + 1);
                  //if current time is greater than schedule length, update schedule length
                  if (time + 1 > latest_time)
                  {
                    latest_time = time + 1;
                  }
                  break;
                }
              }
              else
              {
                break;
              }
            }
            //if node is load data read
            else if (rest[i]->is_Load_Data_Bus_Read())
            {
              //is the related node, (address generation) is ready to be scheduled? at what cycle?
              if (rest[i]->get_Related_Node()->is_ready_to_schedule_Feasible_ASAP(schedule_time_2))
              {
                //if its schedule is greater than time - 1 > change time and try again
                if (schedule_time_2 > (time - 1))
                {
                  time = schedule_time_2;
                  continue;
                }
                //is there any conflict with any scheduled node?
                if (has_Node_Conflict_Feasible_ASAP(rest[i], time))
                {
                  continue;
                }
                //is there any conflict for load address generation with scheduled operations?
                if (has_Node_Conflict_Feasible_ASAP(rest[i]->get_Related_Node(), time - 1))
                {
                  continue;
                }
                //is there enough resources to schedule both operations
                if (HAS_ENOUGH_RESOURCES_FOR_LOAD_INSTRUCTION_AT_CYCLE(time - 1, time, Address_BUS, Data_BUS, Total_Resources, number_of_resources))
                {
                  //allocate resources
                  Data_BUS[time]++;
                  Address_BUS[time - 1]++;
                  Total_Resources[time]++;
                  Total_Resources[time - 1]++;

                  //set schedule
                  rest[i]->get_Sched_Info()->set_Feasible_ASAP(time);
                  rest[i]->get_Related_Node()->get_Sched_Info()->set_Feasible_ASAP(time - 1);
                  //add scheduled operations to set of scheduled nodes
                  scheduled.push_back(rest[i]->get_Related_Node());
                  scheduled.push_back(rest[i]);

                  DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_ID(), time);
                  DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_Related_Node()->get_ID(), time - 1);

                  //set change, new nodes are scheduled
                  change = true;
                  //update schedule length if needed
                  if (time > latest_time)
                  {
                    latest_time = time;
                  }
                  break;
                }
              }
              else
              {
                break;
              }
            }
            //if node is store (data or address)
            else if (rest[i]->is_Store_Address_Generator() || rest[i]->is_Store_Data_Bus_Write())
            {
              //is the related node ready? when?
              DEBUG_FEASIBLE_ASAP_SCHEDULING("Check for Store Related to be Ready");
              if (rest[i]->get_Related_Node()->is_ready_to_schedule_Feasible_ASAP(schedule_time_2))
              {
                //if later, update schedule time and try again
                if (schedule_time_2 > (time))
                {
                  DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d was to be scheduled at time %d", rest[i]->get_ID(), time);
                  DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d was to be scheduled at time %d", rest[i]->get_Related_Node()->get_ID(), schedule_time_2);
                  time = schedule_time_2 - 1;
                  continue;
                }
                //is there node conflict if scheduled at time?
                if (has_Node_Conflict_Feasible_ASAP(rest[i], time))
                {
                  DEBUG_FEASIBLE_ASAP_SCHEDULING("Node Conflict");
                  continue;
                }
                //is there conflict for related node if scheduled at time?
                if (has_Node_Conflict_Feasible_ASAP(rest[i]->get_Related_Node(), time))
                {
                  DEBUG_FEASIBLE_ASAP_SCHEDULING("Related Conflict");
                  continue;
                }
                DEBUG_FEASIBLE_ASAP_SCHEDULING("Check for Memory resources");
                //is there enough resources to schedule both operations
                if (HAS_ENOUGH_RESOURCES_FOR_STORE_INSTRUCTION_AT_CYCLE(time, Address_BUS, Data_BUS, Total_Resources, number_of_resources))
                {
                  //allocate resources
                  Address_BUS[time]++;
                  Data_BUS[time]++;
                  Total_Resources[time] = Total_Resources[time] + 2;

                  //schedule both operations
                  rest[i]->get_Sched_Info()->set_Feasible_ASAP(time);
                  rest[i]->get_Related_Node()->get_Sched_Info()->set_Feasible_ASAP(time);
                  //update set of scheduled operations
                  scheduled.push_back(rest[i]->get_Related_Node());
                  scheduled.push_back(rest[i]);
                  //there was a change, two operations are scheduled
                  change = true;
                  DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_ID(), time);
                  DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_Related_Node()->get_ID(), time);
                  //update schedule length if needed
                  if (time > latest_time)
                  {
                    latest_time = time;
                  }
                  break;
                }
                else
                {
                  DEBUG_FEASIBLE_ASAP_SCHEDULING("Does not have enough Resources Data_BUS %d, Address_BUS %d, PE %d",Address_BUS[time], Data_BUS[time], Total_Resources[time]);
                }

              }
              else
              {
                break;
              }

            }
            //if node is a regular operation
            else
            {
              //is there any conflict with scheduled operations?
              if (has_Node_Conflict_Feasible_ASAP(rest[i], time))
              {
                continue;
              }
              //schedule it at time
              rest[i]->get_Sched_Info()->set_Feasible_ASAP(time);
              //update set of scheduled nodes
              scheduled.push_back(rest[i]);
              //there was a change, a new node is scheduled
              change = true;
              //allocate resources
              Total_Resources[time]++;
              DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_ID(), time);
              //update schedule length if needed
              if (time > latest_time)
              {
                latest_time = time;
              }
              break;
            }
          }
        }
      }
      //update set of unscheduled nodes
      rest = Subtract(rest, scheduled);
      //if there was a change, start from first node in rest again
      if (change)
      {
        break;
      }
    }
    if (!change)
    {
      DEBUG_FEASIBLE_ASAP_SCHEDULING("NO CHANGE SIZE OF REST %d", (int)rest.size());
      for (int i = 0; i < (int) rest.size(); i++)
      { 
        bool ready=rest[i]->is_ready_to_schedule_Feasible_ASAP(schedule_time);
        DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is %d ready to schedule at time %d", rest[i]->get_ID(), ready, schedule_time);
        if (rest[i]->is_Mem_Operation())
        {
          ready = rest[i]->get_Related_Node()->is_ready_to_schedule_Feasible_ASAP(schedule_time_2);
          DEBUG_FEASIBLE_ASAP_SCHEDULING("Related Node %d is %d ready to schedule at time %d", rest[i]->get_Related_Node()->get_ID(), ready, schedule_time_2);
        }
      }
      if (bug)
      {
        exit(1);
      }
      bug=true;
    }

  }
  return 2 * latest_time;
}
/*
 * Schedule operations as late as all successors are luanched
 * FIXME: 20-2-2014
 */

void REGI_DFG::Schedule_ALAP(int latest_time)
{
  DEBUG_SCHEDULING("ALAP Started");

  //set of scheduled operations
  vector<REGI_Node*> scheduled;
  //set of operations selected to be scheduled
  vector<REGI_Node*> to_be_scheduled;
  //set of unscheduled operations
  vector<REGI_Node*> rest = _node_Set;
  //initially nodes without successor at current iteration are to be scheduled
  to_be_scheduled = get_set_of_end_nodes();

  int schedule_time = 0;
  //schedule those nodes at cycle given as parameter
  for (int i = 0; i < (int) to_be_scheduled.size(); i++)
  {
    to_be_scheduled[i]->get_Sched_Info()->set_ALAP(latest_time);
    DEBUG_ALAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_ID(), latest_time);
  }
  //update set of scheduled operations
  scheduled = Union(scheduled, to_be_scheduled);
  //update set of unscheduled operations
  rest = Subtract(rest, scheduled);
  //while there is an unscheduled operation
  while ((int) rest.size() > 0)
  {
    //pick one
    for (int i = 0; i < (int) rest.size(); i++)
    {
      //is it ready to be schedule? if yes, at which cycle?
      if (rest[i]->is_ready_to_schedule_ALAP(schedule_time))
      {
        //schedule it at scheule_time cycle
        rest[i]->get_Sched_Info()->set_ALAP(schedule_time);
        DEBUG_ALAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_ID(), schedule_time);
        //update set of scheduled operations
        scheduled.push_back(rest[i]);
      }
    }
    //update set of unscheduled operations
    rest = Subtract(rest, scheduled);
  }
  DEBUG_SCHEDULING("ALAP Finished");
}

/*
 * schedule operations in ALAP manner considering number of available resources
 * change July 13, 2013
 * Instead of Memory Resource, Address BUS AND DATA BUS
 * FIXME: 20-2-2014
 */
bool REGI_DFG::Schedule_ALAP_Feasible(int latest_time, int number_of_resources)
{

  int* Total_Resources;
  ;
  //	int* Memory_Resources;
  int* Address_BUS;
  int* Data_BUS;

  Total_Resources = new int[MAPPING_POLICY.MAX_LATENCY];
  //Memory_Resources=new int[MAPPING_POLICY.MAX_LATENCY];
  Address_BUS = new int[MAPPING_POLICY.MAX_LATENCY];
  Data_BUS = new int[MAPPING_POLICY.MAX_LATENCY];
  //initally reset all resources
  for (int i = 0; i < MAPPING_POLICY.MAX_LATENCY; i++)
  {
    Total_Resources[i] = 0;
    Data_BUS[i] = 0;
    Address_BUS[i] = 0;
  }
  DEBUG_SCHEDULING("Feasible ALAP Started");

  //set of scheduled operations
  vector<REGI_Node*> scheduled;
  //set of operations to be scheduled
  vector<REGI_Node*> to_be_scheduled;
  vector<REGI_Node*> temp;
  vector<REGI_Node*> temp_2;
  vector<REGI_Node*> temp_3;
  //unscheduled operations
  vector<REGI_Node*> rest = _node_Set;
  /*
   * initially schedule operations with no successor at this iterations
   * for memory operation, load data read and store address are not present in this set
   */
  to_be_scheduled = get_set_of_end_nodes_constraint_scheduling();
  int schedule_time = 0;
  int schedule_time_2 = 0;
  //keep track of any change in set of scheduled operations
  bool change;

  DEBUG_SCHEDULING("scheduling last nodes");
  //pick a node
  for (int i = 0; i < (int) to_be_scheduled.size(); i++)
  {
    //if node is load bus read
    if (to_be_scheduled[i]->is_Load_Data_Bus_Read())
    {
      //find a time slot to schedule the node
      for (int j = latest_time; j > -1; j--)
      {
        //is there enough PE resource to schedule the node?
        if (Total_Resources[latest_time] >= number_of_resources)
        {
          continue;
        }
        //is there a conflict with scheduled operations
        if (has_Node_Conflict_Feasible_ALAP(to_be_scheduled[i], j))
        {
          continue;
        }
        //is there conflict for load address assert with scheduled operations
        if (has_Node_Conflict_Feasible_ALAP(to_be_scheduled[i]->get_Related_Node(), j - 1))
        {
          continue;
        }
        //is there enough resources to schedule both operations
        if (HAS_ENOUGH_RESOURCES_FOR_LOAD_INSTRUCTION_AT_CYCLE(j - 1, j, Address_BUS, Data_BUS, Total_Resources, number_of_resources))
        {
          //allocate resources
          Address_BUS[j - 1]++;
          Data_BUS[j]++;
          Total_Resources[j]++;
          Total_Resources[j - 1]++;
          //schedule operations
          to_be_scheduled[i]->get_Sched_Info()->set_Feasible_ALAP(j);
          to_be_scheduled[i]->get_Related_Node()->get_Sched_Info()->set_Feasible_ALAP(j - 1);
          //update set of scheduled operations
          temp.push_back(to_be_scheduled[i]->get_Related_Node());
          DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d is scheduled at time %d", to_be_scheduled[i]->get_ID(), j);
          DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d is scheduled at time %d", to_be_scheduled[i]->get_Related_Node()->get_ID(), j - 1);
          break;
        }
      }
    }
    //if node is store data bus read
    else if (to_be_scheduled[i]->is_Store_Data_Bus_Write())
    {
      //pick up a time
      for (int j = latest_time; j > -1; j--)
      {
        //is there enough resources at this cycle
        if (Total_Resources[latest_time] >= number_of_resources)
        {
          continue;
        }
        //is there any conflict with scheduled operations
        if (has_Node_Conflict_Feasible_ALAP(to_be_scheduled[i], j))
        {
          continue;
        }
        //is there any conflict between store address assert and scheduled operations
        if (has_Node_Conflict_Feasible_ALAP(to_be_scheduled[i]->get_Related_Node(), j))
        {
          continue;
        }
        //is there enough resources available for both operations
        if (HAS_ENOUGH_RESOURCES_FOR_STORE_INSTRUCTION_AT_CYCLE(j, Address_BUS, Data_BUS, Total_Resources, number_of_resources))
        {
          //allocate resources
          Address_BUS[j]++;
          Data_BUS[j]++;
          Total_Resources[j] = Total_Resources[j] + 2;
          //schedule the operations
          to_be_scheduled[i]->get_Sched_Info()->set_Feasible_ALAP(j);
          to_be_scheduled[i]->get_Related_Node()->get_Sched_Info()->set_Feasible_ALAP(j);
          //update set of scheduled operations
          temp.push_back(to_be_scheduled[i]->get_Related_Node());
          DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d is scheduled at time %d", to_be_scheduled[i]->get_ID(), j);
          DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d is scheduled at time %d", to_be_scheduled[i]->get_Related_Node()->get_ID(), j);
          break;
        }
      }
    }
    //if it is a regular node
    else
    {
      //find a time slot
      for (int j = latest_time; j > -1; j--)
      {
        //is there enough PE?
        if (Total_Resources[latest_time] >= number_of_resources)
        {
          continue;
        }
        //is there a conflict with scheduled operations
        if (has_Node_Conflict_Feasible_ALAP(to_be_scheduled[i], j))
        {
          continue;
        }
        //allocate resources
        Total_Resources[j]++;
        //schedule the operations
        to_be_scheduled[i]->get_Sched_Info()->set_Feasible_ALAP(j);
        DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d is scheduled at time %d", to_be_scheduled[i]->get_ID(), j);
        break;
      }
    }
  }
  DEBUG_FEASIBLE_ALAP_SCHEDULING("Last nodes Finished");
  //update set of schedule operations
  scheduled = Union(temp, to_be_scheduled);
  //update set of unscheduled operations
  rest = Subtract(rest, scheduled);
  //keep track of the size of scheduled operations
  int prev_size = -1;
  //while there is unscheduled operations
  while ((int) rest.size() > 0)
  {
    //if no change in last attempt, it is unable to make any progress in scheduling
    if (prev_size == (int) rest.size())
    {
      return false;
    }
    //sample the number of unscheduled operations
    prev_size = (int) rest.size();
    //for all nodes
    for (int i = 0; i < (int) rest.size(); i++)
    {
      //assume no change in schedules
      change = false;
      DEBUG_FEASIBLE_ALAP_SCHEDULING("check if Node %d is ready to be scheduled", rest[i]->get_ID());
      //is it ready to be scheduled? if yes, at what cycle?
      if (rest[i]->is_ready_to_schedule_Feasible_ALAP(schedule_time, MAX_SCHEDULE_LEN))
      {
        DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d is ready to be scheduled", rest[i]->get_ID());
        //find a time slot to schedule
        for (int time = schedule_time; time > -1; time--)
        {
          //is there enough PE?
          if (Total_Resources[time] < number_of_resources)
          {
            //if it is a load adderss assert operation
            if (rest[i]->is_Load_Address_Generator())
            {
              //is related node ready to be scheduled? if yes, when?
              if (rest[i]->get_Related_Node()->is_ready_to_schedule_Feasible_ALAP(schedule_time_2, MAX_SCHEDULE_LEN))
              {
                //bug fixed July 13, 2013
                //time - 1 changed to time + 1
                if (schedule_time_2 < time + 1)
                {
                  time = schedule_time_2;
                  continue;
                }
                //is there a node conflict with scheduled operations
                if (has_Node_Conflict_Feasible_ALAP(rest[i], time))
                {
                  DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d cannot be scheduled at time %d", rest[i]->get_ID(), time);
                  continue;
                }
                //is there node conflict between load data assert and scheduled operations
                if (has_Node_Conflict_Feasible_ALAP(rest[i]->get_Related_Node(), time + 1))
                {
                  DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d cannot be scheduled at time %d", rest[i]->get_Related_Node()->get_ID(), time + 1);
                  continue;
                }
                //is there enough resources for both operations
                if (HAS_ENOUGH_RESOURCES_FOR_LOAD_INSTRUCTION_AT_CYCLE(time, time + 1, Address_BUS, Data_BUS, Total_Resources, number_of_resources))
                {
                  //allocate resources
                  Address_BUS[time]++;
                  Data_BUS[time + 1]++;
                  Total_Resources[time]++;
                  Total_Resources[time + 1]++;
                  //schedule operations
                  rest[i]->get_Sched_Info()->set_Feasible_ALAP(time);
                  rest[i]->get_Related_Node()->get_Sched_Info()->set_Feasible_ALAP(time + 1);
                  //update set of scheduled operations
                  scheduled.push_back(rest[i]->get_Related_Node());
                  scheduled.push_back(rest[i]);

                  DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_ID(), time);
                  DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_Related_Node()->get_ID(), time + 1);
                  //there was a change, 2 new nodes are scheduled
                  change = true;
                  break;
                }
              }
              else
              {
                break;
              }
            }
            //if it is a load bus read
            else if (rest[i]->is_Load_Data_Bus_Read())
            {
              //is related load address assert ready? if yes, what cycle?
              if (rest[i]->get_Related_Node()->is_ready_to_schedule_Feasible_ALAP(schedule_time_2, MAX_SCHEDULE_LEN))
              {
                //related node is ready but at cycle < time - 1
                if (schedule_time_2 < (time - 1))
                {
                  //update schedule cycle and try again
                  time = schedule_time_2 + 2;
                  continue;
                }
                //is there any conflict with scheduled operations
                if (has_Node_Conflict_Feasible_ALAP(rest[i], time))
                {
                  DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d cannot be scheduled at time %d", rest[i]->get_ID(), time);
                  continue;
                }
                //is there conflict between load address assert and scheduled operations?
                if (has_Node_Conflict_Feasible_ALAP(rest[i]->get_Related_Node(), time - 1))
                {
                  DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d cannot be scheduled at time %d", rest[i]->get_Related_Node()->get_ID(), time - 1);
                  continue;
                }
                //is there enough resources for both operations
                if (HAS_ENOUGH_RESOURCES_FOR_LOAD_INSTRUCTION_AT_CYCLE(time - 1, time, Address_BUS, Data_BUS, Total_Resources, number_of_resources))
                {
                  //allocate resources
                  Data_BUS[time]++;
                  Address_BUS[time - 1]++;
                  Total_Resources[time]++;
                  Total_Resources[time - 1]++;
                  //schedule operations
                  rest[i]->get_Sched_Info()->set_Feasible_ALAP(time);
                  rest[i]->get_Related_Node()->get_Sched_Info()->set_Feasible_ALAP(time - 1);
                  //update set of scheduled operations
                  scheduled.push_back(rest[i]->get_Related_Node());
                  scheduled.push_back(rest[i]);

                  DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_ID(), time);
                  DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_Related_Node()->get_ID(), time - 1);
                  //there was a change, 2 new nodes are scheduled
                  change = true;
                  break;
                }
              }
              else
              {
                break;
              }
            }
            //if it is a store, either address or data assert, operation
            else if (rest[i]->is_Store_Address_Generator() || rest[i]->is_Store_Data_Bus_Write())
            {
              //is related node is ready to be scheduled? if yes, when?
              if (rest[i]->get_Related_Node()->is_ready_to_schedule_Feasible_ALAP(schedule_time_2, MAX_SCHEDULE_LEN))
              {
                //related node is ready but at earlier cycle?
                if (schedule_time_2 < (time))
                {
                  time = schedule_time_2 + 1;
                  continue;
                }
                //is there conflict with scheduled noed?
                if (has_Node_Conflict_Feasible_ALAP(rest[i], time))
                {
                  DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d cannot be scheduled at time %d", rest[i]->get_ID(), time);
                  continue;
                }
                //is there conflict between related node and scheduled nodes
                if (has_Node_Conflict_Feasible_ALAP(rest[i]->get_Related_Node(), time))
                {
                  DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d cannot be scheduled at time %d", rest[i]->get_Related_Node()->get_ID(), time);
                  continue;
                }
                //is there enough resources for both nodes?
                if (HAS_ENOUGH_RESOURCES_FOR_STORE_INSTRUCTION_AT_CYCLE(time, Address_BUS, Data_BUS, Total_Resources, number_of_resources))
                {
                  //allocate resources
                  Address_BUS[time]++;
                  Data_BUS[time]++;
                  Total_Resources[time] = Total_Resources[time] + 2;
                  //schedule operations
                  rest[i]->get_Sched_Info()->set_Feasible_ALAP(time);
                  rest[i]->get_Related_Node()->get_Sched_Info()->set_Feasible_ALAP(time);
                  //update set of scheduled operations
                  scheduled.push_back(rest[i]->get_Related_Node());
                  scheduled.push_back(rest[i]);
                  //there was change in schedule
                  change = true;
                  DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_ID(), time);
                  DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_Related_Node()->get_ID(), time);
                  break;
                }
              }
              else
              {
                break;
              }
            }
            //regular nodes
            else
            {
              //is there conflict with scheduled operations
              if (has_Node_Conflict_Feasible_ALAP(rest[i], time))
              {
                DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d cannot be scheduled at time %d", rest[i]->get_ID(), time);
                continue;
              }
              //schedule the operation
              rest[i]->get_Sched_Info()->set_Feasible_ALAP(time);
              //update set of scheduled operations
              scheduled.push_back(rest[i]);
              //there was a  change in set of scheduled operations
              change = true;
              //allocate resource
              Total_Resources[time]++;
              DEBUG_FEASIBLE_ALAP_SCHEDULING("Node %d is scheduled at time %d", rest[i]->get_ID(), time);
              break;
            }
          }
        }
      }
      //if there was a change, update set of unscheduled nodes
      if (change)
      {
        rest = Subtract(rest, scheduled);
        break;
      }
    }
  }
  DEBUG_FEASIBLE_ALAP_SCHEDULING("Feasible ALAP Finished");
  //return success
  return true;
}
/*
 * Modulo scheduling algorithm
 * (1) given II, (2) schedule length,
 * (3)multiple constraint, sets to true if a node is found constrained by multiple predecessor
 * (4) number of available resources at each cycle
 * July 14, 2013
 * todo:send schedule windows to sub schedule functions, if no time slot left, resolve conflicts immediately
 */
bool REGI_DFG::Modulo_Schedule_Smart_2(int II, int Last_Time, bool &multiple_constraint, int number_of_resources)
{

  //initially no node is constrained by multiple predecessor
  multiple_constraint = false;
  //total available resources
  int Total_Resources[II];
  //memory resources
  //int Memory_Resources[II];
  int Address_BUS[II];
  int Data_BUS[II];

  //
  int tempII = II;
  //initially no resource is used
  for (int i = 0; i < II; i++)
  {
    Total_Resources[i] = 0;
    Data_BUS[i] = 0;
    Address_BUS[i] = 0;
  }
  DEBUG_SCHEDULING("Modulo Started");
  //set of scheduled nodes
  vector<REGI_Node*> scheduled;
  //priority list of nodes is constructed
  //we use similar to simulated annealing priority list
  vector<REGI_Node*> to_be_scheduled = Simulated_Annealing_SortOps(tempII, number_of_resources);

  vector<REGI_Node*> ready_sorted;

  int current_size;
  int schedule_time;
  int current_schedule_time;
  //	int current_schedule_time_2;
  int modulo_schedule_time;
  //	int modulo_schedule_time_2;

  vector<REGI_Node*> temp_1;
  vector<REGI_Node*> temp_2;
  //if a node is found constrained by multiple predecessor, a routing node is inserted to remove one contrain
  REGI_Node_Dummy* tempDummyNode;

  //while there is a node to schedule
  while ((int) to_be_scheduled.size() > 0)
  {
    //how many node are to be scheduled, sample current size
    current_size = (int) to_be_scheduled.size();
    //select a node with the highest priority
    ready_sorted = get_sorted_ready_list_modulo_backward(to_be_scheduled);

    DEBUG_MODULO_SCHEDULING("size of ready list %d", (int ) ready_sorted.size());
    //selected node should be scheduled before!!
    if (ready_sorted[0]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      DEBUG_MODULO_SCHEDULING("Selected node %d is already scheduled!", (int ) ready_sorted[0]->get_ID());
      continue;
    }
    //schedule windows from feasible ASAP and feasible ALAP
    //we schedule backward
    schedule_time = ready_sorted[0]->get_Sched_Info()->get_Feasible_ALAP();        // - ready_sorted[0]->get_Sched_Info()->get_slack();
    int end = ready_sorted[0]->get_Sched_Info()->get_Feasible_ASAP();
    for (int i = schedule_time; i >= end; i--)
    {
      //selected time slot to schedule the node
      current_schedule_time = i;
      //selected modulo time slot to schedule the node
      modulo_schedule_time = i % II;
      //corner case, when II == 1, all nodes are to be schedule at modulo cycle 0
      if (II == 1)
      {
        modulo_schedule_time = 0;
      }
      DEBUG_MODULO_SCHEDULING("Node %d is to be scheduled at %d, modulo time %d", ready_sorted[0]->get_ID(), current_schedule_time, modulo_schedule_time);

      // if selected not is a load address assert
      if (ready_sorted[0]->is_Load_Address_Generator())
      {
        //assume node is not constrained by multiple predecessor
        bool multiple_constraint_schedule = false;
        //schedule both load nodes
        bool success = Schedule_Load_Address(ready_sorted[0], current_schedule_time, II, multiple_constraint_schedule, Address_BUS, Data_BUS, Total_Resources, number_of_resources, scheduled);
        //if failed due to multiple contrain for a node
        if (multiple_constraint_schedule)
        {
          multiple_constraint = true;
          return false;
        }
        //if succeed, update list of nodes
        if (success)
        {
          scheduled.push_back(ready_sorted[0]);
          scheduled.push_back(ready_sorted[0]->get_Related_Node());
          DEBUG_MODULO_SCHEDULING("Node %d is scheduled at time %d, modulo time %d", ready_sorted[0]->get_ID(), ready_sorted[0]->get_Sched_Info()->get_Current(), ready_sorted[0]->get_Sched_Info()->get_Modulo_Current());
          DEBUG_MODULO_SCHEDULING("Node %d is scheduled at time %d, modulo time %d", ready_sorted[0]->get_Related_Node()->get_ID(), ready_sorted[0]->get_Related_Node()->get_Sched_Info()->get_Current(), ready_sorted[0]->get_Related_Node()->get_Sched_Info()->get_Modulo_Current());
          break;
        }
        else
        {
          continue;
        }
      }
      //if node is load data bus reas
      else if (ready_sorted[0]->is_Load_Data_Bus_Read())
      {
        //assume no multiple constrain for a node
        bool multiple_constraint_schedule = false;

        //schedule both nodes
        bool success = Schedule_Load_Data(ready_sorted[0], current_schedule_time, II, multiple_constraint_schedule, Address_BUS, Data_BUS, Total_Resources, number_of_resources, scheduled);
        //failed due to multiple constrain?
        if (multiple_constraint_schedule)
        {
          multiple_constraint = true;
          return false;
        }
        //if succeed, update set of nodes
        if (success)
        {
          scheduled.push_back(ready_sorted[0]);
          scheduled.push_back(ready_sorted[0]->get_Related_Node());
          DEBUG_MODULO_SCHEDULING("Node %d is scheduled at time %d, modulo time %d", ready_sorted[0]->get_ID(), ready_sorted[0]->get_Sched_Info()->get_Current(), ready_sorted[0]->get_Sched_Info()->get_Modulo_Current());
          DEBUG_MODULO_SCHEDULING("Node %d is scheduled at time %d, modulo time %d", ready_sorted[0]->get_Related_Node()->get_ID(), ready_sorted[0]->get_Related_Node()->get_Sched_Info()->get_Current(), ready_sorted[0]->get_Related_Node()->get_Sched_Info()->get_Modulo_Current());
          break;
        }
        else
        {
          continue;
        }

      }
      //if node is store operation
      else if (ready_sorted[0]->is_Store_Address_Generator() || ready_sorted[0]->is_Store_Data_Bus_Write())
      {
        //assume no multi constrain
        bool multiple_constraint_schedule = false;
        //schedule both store operations
        bool success = Schedule_Store_Address_Data(ready_sorted[0], current_schedule_time, II, multiple_constraint_schedule, Address_BUS, Data_BUS, Total_Resources, number_of_resources,scheduled);

        //if failed due to multiple constrain nodes
        if (multiple_constraint_schedule)
        {
          multiple_constraint = true;
          return false;
        }
        //if succeed, update set of lists
        if (success)
        {
          scheduled.push_back(ready_sorted[0]);
          scheduled.push_back(ready_sorted[0]->get_Related_Node());
          if (SCHEDULE_SMART_DEBUG)
          {
            DEBUG_MODULO_SCHEDULING("Node %d is scheduled at time %d, modulo time %d\n", ready_sorted[0]->get_ID(), ready_sorted[0]->get_Sched_Info()->get_Current(), ready_sorted[0]->get_Sched_Info()->get_Modulo_Current());
            DEBUG_MODULO_SCHEDULING("Node %d is scheduled at time %d, modulo time %d", ready_sorted[0]->get_Related_Node()->get_ID(), ready_sorted[0]->get_Related_Node()->get_Sched_Info()->get_Current(),ready_sorted[0]->get_Related_Node()->get_Sched_Info()->get_Modulo_Current());
          }
          break;
        }
        else
        {
          continue;
        }
      }
      //regular nodes
      else
      {
        if (Total_Resources[modulo_schedule_time] < (number_of_resources))
        {
          REGI_Node* node_in_conflict_path=NULL;
          //check if there is constrain conflict with scheduled nodes
          if (has_node_conflict_with_scheduled_nodes(ready_sorted[0], current_schedule_time, modulo_schedule_time, scheduled, node_in_conflict_path))
          {
            //if schedule window is empty, resolve conflict here
            //_DEBUG("Node %d is found in conflict",(node_in_conflict_path)->get_ID());
            if (current_schedule_time == end || II==1)
            {
              //find out the predecessor and successor between nodes
              if (ready_sorted[0]->is_Connected_To(node_in_conflict_path))
              {
                //create a routing node
                tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(),ready_sorted[0]);
                if ((int) tempDummyNode->get_Number_of_Pred()>0)
                {
                  Add_Arc( tempDummyNode->get_Pred_Arc(0));
                }


                //insert it
                insert_Node(tempDummyNode);
                //insert routing node to resolve conflict
                if (insert_Node_in_between_output(ready_sorted[0], node_in_conflict_path, tempDummyNode))
                {

                  multiple_constraint = true;
                  return false;
                }
              }
              else
              {
                //create a routing node
                tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path);
                if ((int) tempDummyNode->get_Number_of_Pred()>0)
                {
                  Add_Arc( tempDummyNode->get_Pred_Arc(0));
                }


                //insert it
                insert_Node(tempDummyNode);

                if (insert_Node_in_between_output(node_in_conflict_path, ready_sorted[0], tempDummyNode))
                {

                  multiple_constraint = true;
                  return false;
                }
              }

              FATAL(true, "node %d constrains multiple successor but we could not insert a node between it and one of constrained successor", ready_sorted[0]->get_ID());
              return false;
            }
            continue;

          }
          //if node constrains multiple nodes
          if (constrains_multiple(ready_sorted[0], current_schedule_time, node_in_conflict_path, scheduled))
          {

            //create a routing node
            tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(),ready_sorted[0] );
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }


            //insert it
            insert_Node(tempDummyNode);

            //insert routing in between
            if (insert_Node_in_between_output(ready_sorted[0], node_in_conflict_path, tempDummyNode))
            {
              multiple_constraint = true;
              return false;
            }

            FATAL(true, "node %d constrains multiple successor but we could not insert a node between it and one of constrained successor", ready_sorted[0]->get_ID());
            return false;
          }
          //if multiple successor are scheduled such that the related node constrain them
          if (has_multiple_constrains(ready_sorted[0], current_schedule_time, node_in_conflict_path, scheduled))
          {
            //create a routing node
            tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path );
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }


            insert_Node(tempDummyNode);

            //insert between them
            if (insert_Node_in_between_output(node_in_conflict_path, ready_sorted[0], tempDummyNode))
            {
              multiple_constraint = true;
              return false;
            }

            FATAL(true, "node %d constrains multiple successor but we could not insert a node between it and one of constrained successor", ready_sorted[0]->get_ID());
            return false;
          }
          //schedule the new node
          ready_sorted[0]->get_Sched_Info()->set_Current(current_schedule_time, II);
          //allocate resources
          Total_Resources[modulo_schedule_time]++;
          //add to list of scheduled nodes
          scheduled.push_back(ready_sorted[0]);
          DEBUG_MODULO_SCHEDULING("Node %d is scheduled at time %d, modulo time %d", ready_sorted[0]->get_ID(), current_schedule_time, modulo_schedule_time);
          break;
        }
        else
        {
          DEBUG_MODULO_SCHEDULING("Not enough resources available at %d PE used %d", schedule_time, Total_Resources[modulo_schedule_time]);
        }

      }
    }

    to_be_scheduled = Subtract(to_be_scheduled, scheduled);
    if (current_size == (int) to_be_scheduled.size())
      return false;
  }
  return true;

}
/*
 * Schedule a load address assert instruction along with related load data read
 * (1) load address assert instruction, (2) time slot to schedule the operation
 * (3) current II, (4) sets true if node is constrained by multiple predecessor
 * (5) address bus resource table, (6) data bus resource table
 * (7) PE resource table, (8) max number of available resources at each cycle
 * (9) scheduled operations
 * FIXME: 20-2-2014
 *
 */
bool REGI_DFG::Schedule_Load_Address(REGI_Node* selected_node, int schedule_time, int II, bool &multiple_constraint, int* address_bus, int* data_bus, int* PE_resources, int max_resources, vector<REGI_Node*> &scheduled_nodes)
{
  //schedule cycle for related node
  int current_schedule_time_2;
  //modulo scheduled cycle for related node
  int modulo_schedule_time_2;
  //modulo scheduled cycle for node
  int modulo_schedule_time;
  //check if the related node is ready to schedule, if yes, when?
  if (selected_node->get_Related_Node()->is_ready_to_schedule_modulo_backward(current_schedule_time_2))
  {
    //if related node does not have any successor in this iteration, it can be scheduled at any cycle!!!
    if ((int) selected_node->get_Related_Node()->Get_Successors_Same_Iteration_Exclude_Address_Dependency().size() == 0)
    {
      //schedule data read at next cycle
      current_schedule_time_2 = schedule_time + 1;
      //no node can be scheduled at any cycle more than MAX LEN
      if (current_schedule_time_2 > MAX_SCHEDULE_LEN)
        return false;
    }
    //if load data read should be schedule at > schedule_time + 1, we are following ALAP policy
    //so we schedule it earlier!!
    //can be dangerous node might be pushed back to < 0!!!!
    if (current_schedule_time_2 > schedule_time + 1)
    {
      //pull schedule time to proper earlier time
      current_schedule_time_2 = schedule_time + 1;
    }
    //if load address is ahead of data read, pull back load address schedule time
    if (current_schedule_time_2 < schedule_time + 1)
    {
      //pull schedule time to proper earlier time
      schedule_time = current_schedule_time_2 - 1;
    }
    //modulo schedule time for both load instructions
    modulo_schedule_time_2 = current_schedule_time_2 % II;
    modulo_schedule_time = schedule_time % II;
    //if II == 1, all instructions are executed at modulo time 0
    if (II == 1)
    {
      modulo_schedule_time_2 = 0;
      modulo_schedule_time = 0;
    }
    //it is scheduled a cycle after selected node

    //check if there is enough resources to schedule both nodes
    //bug fixed: first parameter was schedule time instead of modulo schedule time
    //if we have enough resources
    if (HAS_ENOUGH_RESOURCES_FOR_LOAD_INSTRUCTION_AT_CYCLE(modulo_schedule_time, modulo_schedule_time_2, address_bus, data_bus, PE_resources, max_resources))
    {
      //there is no schedule conflict if it is scheduled at this time
      //July 14, 2013 -> new conflict function instead of has_Node_Conflict_Current_new(selected_node, schedule_time, II)
      //node to insert in case of multiple constrained nodes
      REGI_Node_Dummy* tempDummyNode;
      REGI_Node* node_in_conflict_path=NULL;
      if (has_node_conflict_with_scheduled_nodes(selected_node, schedule_time, modulo_schedule_time, scheduled_nodes, node_in_conflict_path))
      {
        //if this is the last time slot or II==1, conflict has be take care of at this stage
        if (selected_node->get_Sched_Info()->get_Feasible_ASAP()==schedule_time || II==1)
        {
          //find out the predecessor and successor between nodes
          if (selected_node->is_Connected_To(node_in_conflict_path))
          {
            //create a routing node
            tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(), selected_node);
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }


            //insert it
            insert_Node(tempDummyNode);

            //insert routing node to resolve conflict
            if (insert_Node_in_between_output(selected_node, node_in_conflict_path, tempDummyNode))
            {
              multiple_constraint = true;
              return false;
            }
          }
          else
          {  
            //create a routing node
            tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(), node_in_conflict_path);
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }


            //insert it
            insert_Node(tempDummyNode);

            if (insert_Node_in_between_output(node_in_conflict_path, selected_node, tempDummyNode))
            {
              multiple_constraint = true;
              return false;
            }
          }
        }
        return false;
      }
      //is there a conflict between related node and scheduled nodes?
      if (has_node_conflict_with_scheduled_nodes(selected_node->get_Related_Node(), current_schedule_time_2, modulo_schedule_time_2, scheduled_nodes, node_in_conflict_path))
      {
        //if this is the last time slot or II==1, conflict has be take care of at this stage
        if (selected_node->get_Related_Node()->get_Sched_Info()->get_Feasible_ASAP()==current_schedule_time_2 || II==1)
        {
          //find out the predecessor and successor between nodes
          if (selected_node->get_Related_Node()->is_Connected_To(node_in_conflict_path))
          { 
            //create a routing node
            tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(),selected_node->get_Related_Node() );
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }


            //insert it
            insert_Node(tempDummyNode);

            //insert routing node to resolve conflict
            if (insert_Node_in_between_output(selected_node->get_Related_Node(), node_in_conflict_path, tempDummyNode))
            {

              multiple_constraint = true;
              return false;
            }
          }
          else
          {                  
            //create a routing node
            tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(), node_in_conflict_path);
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }


            //insert it
            insert_Node(tempDummyNode);

            if (insert_Node_in_between_output(node_in_conflict_path, selected_node->get_Related_Node(), tempDummyNode))
            {
              multiple_constraint = true;
              return false;
            }
          }

        }
        return false;
      }
      //hold successor constrained by this
      //if multiple successor are scheduled such that this node constrain them
      if (constrains_multiple(selected_node, schedule_time, node_in_conflict_path, scheduled_nodes))
      {
        //construct a routing node
        tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index(), selected_node);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }


        //insert it
        insert_Node(tempDummyNode);
        //insert it between this and selected constrained node
        if (insert_Node_in_between_output(selected_node, node_in_conflict_path, tempDummyNode))
        {
          //there was multiple constrained node, let caller know
          multiple_constraint = true;
          //scheduling did not succeed
          return false;
        }
        FATAL(true, "node %d constrains multiple successor but we could not insert a node between it and one of constrained successor", selected_node->get_ID());
        return false;
      }
      //if multiple successor are scheduled such that the related node constrain them
      if (constrains_multiple(selected_node->get_Related_Node(), current_schedule_time_2, node_in_conflict_path, scheduled_nodes))
      {
        //construct a routing node
        tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index(),selected_node->get_Related_Node() );
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }


        //insert it
        insert_Node(tempDummyNode);
        //insert it between this and selected constrained node
        if (insert_Node_in_between_output(selected_node->get_Related_Node(), node_in_conflict_path, tempDummyNode))
        {
          //there was multiple constrained node, let caller know
          multiple_constraint = true;
          //scheduling did not succeed
          return false;
        }
        FATAL(true, "node %d constrains multiple successor but we could not insert a node between it and one of constrained successor", selected_node->get_Related_Node()->get_ID());
        return false;
      }
      //if constrains multiple predecessors constrained this node
      if (has_multiple_constrains(selected_node, schedule_time, node_in_conflict_path, scheduled_nodes))
      {

        tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path );
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }


        insert_Node(tempDummyNode);
        if (insert_Node_in_between_output(node_in_conflict_path, selected_node, tempDummyNode))
        {
          multiple_constraint = true;
          return false;
        }
        FATAL(true, "node %d is constrained by multiple predecessors but we could not insert a node between it and one of constrainers", selected_node->get_ID());
        return false;
      }
      //if multiple predecessors constrained the related node
      if (has_multiple_constrains(selected_node->get_Related_Node(), current_schedule_time_2, node_in_conflict_path, scheduled_nodes))
      {

        tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }


        insert_Node(tempDummyNode);
        if (insert_Node_in_between_output(node_in_conflict_path, selected_node->get_Related_Node(),  tempDummyNode))
        {
          multiple_constraint = true;
          return false;
        }
        FATAL(true, "node %d is constrained by multiple predecessors but we could not insert a node between it and one of constrainers", selected_node->get_Related_Node()->get_ID());
        return false;
      }
      //everything is checked, schedule both operations
      selected_node->get_Sched_Info()->set_Current(schedule_time, II);
      selected_node->get_Related_Node()->get_Sched_Info()->set_Current(current_schedule_time_2, II);
      //allocate resources
      address_bus[modulo_schedule_time]++;
      data_bus[modulo_schedule_time_2]++;
      PE_resources[modulo_schedule_time]++;
      PE_resources[modulo_schedule_time_2]++;
      return true;
    }
    else
    {
      DEBUG_MODULO_SCHEDULING("Not enough resources available at %d PE used %d Add_bus used %d Data_bus used %d  and at %d PE used %d Add_bus used %d Data_bus used %d", schedule_time, address_bus[modulo_schedule_time],
          data_bus[modulo_schedule_time], PE_resources[modulo_schedule_time], current_schedule_time_2, address_bus[modulo_schedule_time_2], data_bus[modulo_schedule_time_2], PE_resources[modulo_schedule_time_2]);
    }
  }
  else
  {
    DEBUG_MODULO_SCHEDULING("Related node %d is not ready yet", selected_node->get_Related_Node()->get_ID())
      ;
  }

  return false;

}
/*
 * Schedule a load data assert instruction along with related load address assert
 * (1) load data assert instruction, (2) time slot to schedule the operation
 * (3) current II, (4) sets true if node is constrained by multiple predecessor
 * (5) address bus resource table, (6) data bus resource table
 * (7) PE resource table, (8) max number of available resources at each cycle
 * (9) scheduled operations
 * FIXME:20-2-2014
 */
bool REGI_DFG::Schedule_Load_Data(REGI_Node* selected_node, int schedule_time, int II, bool &multiple_constraint, int* address_bus, int* data_bus, int* PE_resources, int max_resources, vector<REGI_Node*> &scheduled_nodes)
{
  //schedule cycle for related node
  int current_schedule_time_2;
  //modulo scheduled cycle for related node
  int modulo_schedule_time_2;
  //modulo scheduled cycle for node
  int modulo_schedule_time;
  //check if the related node is ready to schedule, if yes, when?
  if (selected_node->get_Related_Node()->is_ready_to_schedule_modulo_backward(current_schedule_time_2))
  {
    //if related node does not have any successor in this iteration, it can be scheduled at any cycle!!!
    if ((int) selected_node->get_Related_Node()->Get_Next_Nodes_Same_Iteration().size() == 0)
    {
      //schedule data read at next cycle
      current_schedule_time_2 = schedule_time - 1;
    }
    //if load data read should be schedule at > schedule_time + 1, we are following ALAP policy
    //so we schedule it earlier!!
    if (current_schedule_time_2 > schedule_time - 1)
    {
      //pull schedule time of it to proper earlier time
      current_schedule_time_2 = schedule_time - 1;
    }
    //can be dangerous 
    if (current_schedule_time_2 < schedule_time - 1)
    {
      //pull schedule time to proper earlier time
      schedule_time = current_schedule_time_2 + 1;
    }
    //modulo schedule time for both load instructions
    modulo_schedule_time_2 = current_schedule_time_2 % II;
    modulo_schedule_time = schedule_time % II;
    //if II == 1, all instructions are executed at modulo time 0
    if (II == 1)
    {
      modulo_schedule_time_2 = 0;
      modulo_schedule_time = 0;
    }
    if (HAS_ENOUGH_RESOURCES_FOR_LOAD_INSTRUCTION_AT_CYCLE(modulo_schedule_time_2, modulo_schedule_time, address_bus, data_bus, PE_resources, max_resources))
    {

      //there is no schedule conflict if it is scheduled at this time
      //July 14, 2013 -> new conflict function instead of has_Node_Conflict_Current_new(selected_node, schedule_time, II)
      //node to insert in case of multiple constrained nodes
      REGI_Node_Dummy* tempDummyNode;
      REGI_Node* node_in_conflict_path=NULL;
      if (has_node_conflict_with_scheduled_nodes(selected_node, schedule_time, modulo_schedule_time, scheduled_nodes, node_in_conflict_path))
      {
        //if this is the last time slot or II==1, conflict has be take care of at this stage
        if (selected_node->get_Sched_Info()->get_Feasible_ASAP()==schedule_time || II==1)
        {
          //find out the predecessor and successor between nodes
          if (selected_node->is_Connected_To(node_in_conflict_path))
          {
            //create a routing node
            tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(),selected_node );
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }


            //insert it
            insert_Node(tempDummyNode);

            //insert routing node to resolve conflict
            if (insert_Node_in_between_output(selected_node, node_in_conflict_path, tempDummyNode))
            {
              multiple_constraint = true;
              return false;
            }
          }
          else
          {
            //create a routing node
            tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path);
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }


            //insert it
            insert_Node(tempDummyNode);

            if (insert_Node_in_between_output(node_in_conflict_path, selected_node, tempDummyNode))
            {
              multiple_constraint = true;
              return false;
            }
          }
        }
        return false;
      }
      //is there a conflict between related node and scheduled nodes?
      if (has_node_conflict_with_scheduled_nodes(selected_node->get_Related_Node(), current_schedule_time_2, modulo_schedule_time_2, scheduled_nodes, node_in_conflict_path))
      {
        //if this is the last time slot or II==1, conflict has be take care of at this stage
        if (selected_node->get_Related_Node()->get_Sched_Info()->get_Feasible_ASAP()==current_schedule_time_2 || II==1)
        {
          //find out the predecessor and successor between nodes
          if (selected_node->get_Related_Node()->is_Connected_To(node_in_conflict_path))
          {
            //create a routing node
            tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(), selected_node->get_Related_Node());
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }


            //insert it
            insert_Node(tempDummyNode);

            //insert routing node to resolve conflict
            if (insert_Node_in_between_output(selected_node->get_Related_Node(), node_in_conflict_path, tempDummyNode))
            {
              multiple_constraint = true;
              return false;
            }
          }
          else
          {
            //create a routing node
            tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path );
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }


            //insert it
            insert_Node(tempDummyNode);

            if (insert_Node_in_between_output(node_in_conflict_path, selected_node->get_Related_Node(), tempDummyNode))
            {
              multiple_constraint = true;
              return false;
            }
          }

        }
        return false;
      }

      //hold successor constrained by this
      //if multiple successor are scheduled such that this node constrain them
      if (constrains_multiple(selected_node, schedule_time, node_in_conflict_path, scheduled_nodes))
      {
        //construct a routing node
        tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index(),selected_node);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }


        //insert it
        insert_Node(tempDummyNode);
        //insert it between this and selected constrained node
        if (insert_Node_in_between_output(selected_node, node_in_conflict_path, tempDummyNode))
        {
          //there was multiple constrained node, let caller know
          multiple_constraint = true;
          //scheduling did not succeed
          return false;
        }
        FATAL(true, "node %d constrains multiple successor but we could not insert a node between it and one of constrained successor", selected_node->get_ID());
        return false;
      }
      //if multiple successor are scheduled such that the related node constrain them
      if (constrains_multiple(selected_node->get_Related_Node(), current_schedule_time_2, node_in_conflict_path, scheduled_nodes))
      {
        //construct a routing node
        tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index(),selected_node->get_Related_Node());
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }


        //insert it
        insert_Node(tempDummyNode);
        //insert it between this and selected constrained node
        if (insert_Node_in_between_output(selected_node->get_Related_Node(), node_in_conflict_path, tempDummyNode))
        {
          //there was multiple constrained node, let caller know
          multiple_constraint = true;
          //scheduling did not succeed
          return false;
        }
        FATAL(true, "node %d constrains multiple successor but we could not insert a node between it and one of constrained successor", selected_node->get_Related_Node()->get_ID());
        return false;
      }
      //if constrains multiple predecessors constrained this node
      if (has_multiple_constrains(selected_node, schedule_time, node_in_conflict_path, scheduled_nodes))
      {

        tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index(), node_in_conflict_path);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }


        insert_Node(tempDummyNode);
        if (insert_Node_in_between_output(node_in_conflict_path,selected_node,  tempDummyNode))
        {
          multiple_constraint = true;
          return false;
        }
        FATAL(true, "node %d is constrained by multiple predecessors but we could not insert a node between it and one of constrainers", selected_node->get_ID());
        return false;
      }
      //if multiple predecessors constrained the related node
      if (has_multiple_constrains( selected_node->get_Related_Node(),current_schedule_time_2,  node_in_conflict_path, scheduled_nodes))
      {

        tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index(), node_in_conflict_path);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }


        insert_Node(tempDummyNode);
        if (insert_Node_in_between_output( node_in_conflict_path,selected_node->get_Related_Node(), tempDummyNode))
        {
          multiple_constraint = true;
          return false;
        }
        FATAL(true, "node %d is constrained by multiple predecessors but we could not insert a node between it and one of constrainers", selected_node->get_Related_Node()->get_ID());
        return false;
      }
      //everything is checked, schedule both operations
      selected_node->get_Sched_Info()->set_Current(schedule_time, II);
      selected_node->get_Related_Node()->get_Sched_Info()->set_Current(current_schedule_time_2, II);
      //allocate resources
      address_bus[modulo_schedule_time_2]++;
      data_bus[modulo_schedule_time]++;
      PE_resources[modulo_schedule_time]
        ++;
      PE_resources[modulo_schedule_time_2]++;
      return true;

    }
    else
    {
      DEBUG_MODULO_SCHEDULING("Not enough resources available at %d PE used %d address_bus used %d data_bus used %d and at %d PE used %d address_bus used %d data_bus used %d", schedule_time, address_bus[modulo_schedule_time],
          data_bus[modulo_schedule_time], PE_resources[modulo_schedule_time], current_schedule_time_2, address_bus[modulo_schedule_time_2], data_bus[modulo_schedule_time_2], PE_resources[modulo_schedule_time_2]);
    }
  }
  else
  {

    DEBUG_MODULO_SCHEDULING("Related node %d is not ready yet", selected_node->get_Related_Node()->get_ID());

  }
  return false;

}
/*
 * Schedule a store instruction along with related node
 * (1) store address/data  assert instruction, (2) time slot to schedule the operation
 * (3) current II, (4) sets true if node is constrained by multiple predecessor
 * (5) address bus resource table, (6) data bus resource table
 * (7) PE resource table, (8) max number of available resources at each cycle
 * (9) scheduled operations
 * FIXME: 20-2-2014
 */
bool REGI_DFG::Schedule_Store_Address_Data(REGI_Node* selected_node, int schedule_time, int II, bool &multiple_constraint, int* address_bus, int* data_bus, int* PE_resources, int max_resources, vector<REGI_Node*> &scheduled_nodes)
{
  //schedule cycle for related node
  int current_schedule_time_2;
  //modulo scheduled cycle for related node
  int modulo_schedule_time_2;
  //modulo scheduled cycle for node
  int modulo_schedule_time;
  //check if the related node is ready to schedule, if yes, when?
  if (selected_node->get_Related_Node()->is_ready_to_schedule_modulo_backward(current_schedule_time_2))
  {
    //if related node does not have any successor in this iteration, it can be scheduled at any cycle!!!
    if ((int) selected_node->get_Related_Node()->Get_Next_Nodes_Same_Iteration().size() == 0)
    {
      current_schedule_time_2 = schedule_time;
    }
    //synch schedule time
    if (current_schedule_time_2 > schedule_time)
    {
      current_schedule_time_2 = schedule_time;
    }
    if (current_schedule_time_2 < schedule_time)
    {
      schedule_time = current_schedule_time_2;
    }

    //modulo schedule time for both load instructions
    modulo_schedule_time_2 = current_schedule_time_2 % II;
    modulo_schedule_time = schedule_time % II;
    //if II == 1, all instructions are executed at modulo time 0
    if (II == 1)
    {
      modulo_schedule_time_2 = 0;
      modulo_schedule_time = 0;
    }

    //if we have enough resources
    if (HAS_ENOUGH_RESOURCES_FOR_STORE_INSTRUCTION_AT_CYCLE(modulo_schedule_time, address_bus, data_bus, PE_resources, max_resources))
    {
      //node to insert in case of multiple constrained nodes
      REGI_Node_Dummy* tempDummyNode;
      //there is no schedule conflict if it is scheduled at this time
      REGI_Node* node_in_conflict_path=NULL;
      if (has_node_conflict_with_scheduled_nodes(selected_node, schedule_time, modulo_schedule_time, scheduled_nodes, node_in_conflict_path))
      {
        //if this is the last time slot or II==1, conflict has be take care of at this stage
        if (selected_node->get_Sched_Info()->get_Feasible_ASAP()==schedule_time || II==1)
        {
          //find out the predecessor and successor between nodes
          if (selected_node->is_Connected_To(node_in_conflict_path))
          {
            //create a routing node
            tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(), selected_node);
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }


            //insert it
            insert_Node(tempDummyNode);

            //insert routing node to resolve conflict
            if (insert_Node_in_between_output(selected_node, node_in_conflict_path, tempDummyNode))
            {
              multiple_constraint = true;
              return false;
            }
          }
          else
          {
            //create a routing node
            tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path);
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }


            //insert it
            insert_Node(tempDummyNode);

            if (insert_Node_in_between_output(node_in_conflict_path, selected_node, tempDummyNode))
            {
              multiple_constraint = true;
              return false;
            }
          }
        }
        return false;
      }
      //is there a conflict between related node and scheduled nodes?
      if (has_node_conflict_with_scheduled_nodes(selected_node->get_Related_Node(), current_schedule_time_2, modulo_schedule_time_2, scheduled_nodes, node_in_conflict_path))
      {
        //if this is the last time slot or II==1, conflict has be take care of at this stage
        if (selected_node->get_Related_Node()->get_Sched_Info()->get_Feasible_ASAP()==current_schedule_time_2 || II==1)
        {
          //find out the predecessor and successor between nodes
          if (selected_node->get_Related_Node()->is_Connected_To(node_in_conflict_path))
          {
            //create a routing node
            tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(), selected_node->get_Related_Node());
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }


            //insert it
            insert_Node(tempDummyNode);


            //insert routing node to resolve conflict
            if (insert_Node_in_between_output(selected_node->get_Related_Node(), node_in_conflict_path, tempDummyNode))
            {
              multiple_constraint = true;
              return false;
            }
          }
          else
          {
            //create a routing node
            tempDummyNode= new REGI_Node_Dummy(1, Get_Unique_Index(), node_in_conflict_path);
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }


            //insert it
            insert_Node(tempDummyNode);

            if (insert_Node_in_between_output(node_in_conflict_path, selected_node->get_Related_Node(), tempDummyNode))
            {
              multiple_constraint = true;
              return false;
            }
          }
        }
        return false;
      }
      //hold successor constrained by this
      //if multiple successor are scheduled such that this node constrain them
      if (constrains_multiple(selected_node, schedule_time, node_in_conflict_path, scheduled_nodes))
      {
        //construct a routing node
        tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index(),selected_node );
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }


        //insert it
        insert_Node(tempDummyNode);
        //insert it between this and selected constrained node
        if (insert_Node_in_between_output(selected_node, node_in_conflict_path, tempDummyNode))
        {
          //there was multiple constrained node, let caller know
          multiple_constraint = true;
          //scheduling did not succeed
          return false;
        }
        FATAL(true, "node %d constrains multiple successor but we could not insert a node between it and one of constrained successor", selected_node->get_ID());
        return false;
      }
      //if multiple successor are scheduled such that the related node constrain them
      if (constrains_multiple(selected_node->get_Related_Node(), current_schedule_time_2, node_in_conflict_path, scheduled_nodes))
      {
        //construct a routing node
        tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index(), selected_node->get_Related_Node());
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }


        //insert it
        insert_Node(tempDummyNode);

        //insert it between this and selected constrained node
        if (insert_Node_in_between_output(selected_node->get_Related_Node(), node_in_conflict_path, tempDummyNode))
        {
          //there was multiple constrained node, let caller know
          multiple_constraint = true;
          //scheduling did not succeed
          return false;
        }
        FATAL(true, "node %d constrains multiple successor but we could not insert a node between it and one of constrained successor", selected_node->get_Related_Node()->get_ID());
        return false;
      }
      //if constrains multiple predecessors constrained this node
      if (has_multiple_constrains(selected_node, schedule_time, node_in_conflict_path, scheduled_nodes))
      {

        tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index(), node_in_conflict_path);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }


        insert_Node(tempDummyNode);
        if (insert_Node_in_between_output(node_in_conflict_path,selected_node,  tempDummyNode))
        {
          multiple_constraint = true;
          return false;
        }
        FATAL(true, "node %d is constrained by multiple predecessors but we could not insert a node between it and one of constrainers", selected_node->get_ID());
        return false;
      }
      //if multiple predecessors constrained the related node
      if (has_multiple_constrains(selected_node->get_Related_Node(), current_schedule_time_2, node_in_conflict_path, scheduled_nodes))
      {

        tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }


        insert_Node(tempDummyNode);
        if (insert_Node_in_between_output(node_in_conflict_path, selected_node->get_Related_Node(), tempDummyNode))
        {
          multiple_constraint = true;
          return false;
        }
        FATAL(true, "node %d is constrained by multiple predecessors but we could not insert a node between it and one of constrainers", selected_node->get_Related_Node()->get_ID());
        return false;
      }
      //everything is checked, schedule both operations
      selected_node->get_Sched_Info()->set_Current(schedule_time, II);
      selected_node->get_Related_Node()->get_Sched_Info()->set_Current(current_schedule_time_2, II);
      //allocate resources
      address_bus[modulo_schedule_time]++;
      data_bus[modulo_schedule_time_2]++;
      PE_resources[modulo_schedule_time]++;
      PE_resources[modulo_schedule_time_2]++;
      return true;
    }
    else
    {

      DEBUG_MODULO_SCHEDULING("Not enough resources available at %d PE used %d Add_bus used %d Data_bus used %d and at %d PE used %d Add_bus used %d Data_bus used %d", schedule_time, address_bus[modulo_schedule_time], data_bus[modulo_schedule_time], PE_resources[modulo_schedule_time], current_schedule_time_2, address_bus[modulo_schedule_time_2], data_bus[modulo_schedule_time_2], PE_resources[modulo_schedule_time_2]);
    }

  }
  else
  {
    DEBUG_MODULO_SCHEDULING("Related node %d is not ready yet", selected_node->get_Related_Node()->get_ID());
  }
  return false;
}

void REGI_DFG::Reset_ASAP_Schedule()
{
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    _node_Set[i]->get_Sched_Info()->reset_ASAP();
  }
}

//reset all nodes ALAP schedule time
void REGI_DFG::Reset_ALAP_Schedule()
{
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    _node_Set[i]->get_Sched_Info()->reset_ALAP();
  }
}

void REGI_DFG::Reset_ASAP_Feasible_Schedule()
{
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    _node_Set[i]->get_Sched_Info()->reset_Feasible_ASAP();
  }
}

void REGI_DFG::Reset_ALAP_Feasible_Schedule()
{
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    _node_Set[i]->get_Sched_Info()->reset_Feasible_ALAP();

  }
}

void REGI_DFG::Reset_Current_Schedule()
{
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    _node_Set[i]->get_Sched_Info()->reset_Current();
  }
}

/*
 *  construct a list of ready to schedule operations
 *  operations in this list are sorted by their slack 
 *  (1) list is the set of given nodes to the function
 * note load address generator and store data bus assert are never returned in ready list
 * possibly can cause a bug in rare cases if all remaining nodes depend on scheduling one of above nodes
 */
vector<REGI_Node*> REGI_DFG::get_sorted_ready_list_modulo_backward(vector<REGI_Node*> list)
{
  //list of nodes ready to be scheduled
  vector<REGI_Node*> ready_list;
  vector<REGI_Node*> sorted_list;

  int time;
  for (int i = 0; i < (int) list.size(); i++)
  {

    //ignore the node if it is load address generator or store bus assert
    if (list[i]->is_Load_Address_Generator() || list[i]->is_Store_Address_Generator())
    {
      continue;
    }
    //is it is a memory type operation but the related not is not ready yet
    if (list[i]->is_Load_Data_Bus_Read() || list[i]->is_Store_Data_Bus_Write())
    {
      if (!list[i]->get_Related_Node()->is_ready_to_schedule_modulo_backward(time))
      {
        continue;
      }
    }
    //for the ready nodes, assign a schedule window between current time and ASAP instead of ALAP to ASAP
    if (list[i]->is_ready_to_schedule_modulo_backward(time))
    {
      list[i]->get_Sched_Info()->set_Feasible_ALAP(time);
      //why???
      if (time == MAXINT)
      {
        REGI_DEBUG(time==MAXINT, "operation is assigned to an unjustified cycle");
        list[i]->get_Sched_Info()->set_slack(0);
      }
      else
      {
        list[i]->get_Sched_Info()->update_slack(time);
      }
      ready_list.push_back(list[i]);
    }
  }

  int min_slack = MAXINT;
  int min_index;
  //sort the ready list by the slack
  while ((int) ready_list.size() > 0)
  {
    min_slack = MAXINT;
    for (int i = 0; i < (int) ready_list.size(); i++)
    {
      if (ready_list[i]->get_Sched_Info()->get_slack() < min_slack)
      {
        min_slack = ready_list[i]->get_Sched_Info()->get_slack();
        min_index = i;
      }
    }
    sorted_list.push_back(ready_list[min_index]);
    ready_list = Subtract(ready_list, sorted_list);

  }
  return sorted_list;
}
/*
 *  return list of current scheduled nodes
 *  (1) inset is the given set of nodes
 */
vector<REGI_Node*> REGI_DFG::get_Current_Scheduled_Nodes(vector<REGI_Node*> inSet)
{
  vector<REGI_Node*> visited;

  for (int i = 0; i < (int) inSet.size(); i++)
  {
    if (inSet[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      visited.push_back(inSet[i]);
    }
  }
  return visited;
}
/*
 * this functions finds the set of all nodes that should be mapped to the same physical PE where current node * is to be mapped. This function finds PE conflict when nodes are scheduled to prevent possible placement 
 * conflict
 * (1) given node (2) schedule cycle of the node
 */
vector<REGI_Node*> REGI_DFG::get_All_Nodes_Constrained_by_Current(REGI_Node* current_node, int time)
{

  //visited nodes through inter-iteration dependencies
  vector<REGI_Node*> visited_inter;
  //visited though intra-iteration dependency where dependent nodes are scheduled on two non-consecutive cycles
  vector<REGI_Node*> visited_intra;
  //to be visited through inter iteration
  vector<REGI_Node*> to_visit_inter;
  //to be visited though intera iteration dependency
  vector<REGI_Node*> to_visit_intra;
  vector<REGI_Node*> temp_1;
  vector<REGI_Node*> temp_2;
  REGI_Node* temp_node;

  //get all connected nodes to current node with inter-iteration dependency
  temp_1 = get_Inter_Iteration_Related_Nodes(current_node);

  //get set of nodes in this set which are already scheduled (current)
  //theses nodes must be mapped on same physical PE
  to_visit_inter = get_Current_Scheduled_Nodes(temp_1);

  //get set of all nodes connected with regular dependency however scheduled in non-consecutive cycles
  //theses nodes must be mapped on same physical PE
  to_visit_intra = get_Intra_Iteration_Related_Nodes_Current(current_node, time);

  visited_intra.push_back(current_node);
  visited_inter.push_back(current_node);

  while ((int) to_visit_inter.size() > 0 || (int) to_visit_intra.size() > 0)
  {
    if ((int) to_visit_inter.size() > 0)
    {
      temp_node = to_visit_inter[0];
      visited_inter.push_back(temp_node);
      temp_1 = get_Intra_Iteration_Related_Nodes_Current(temp_node, temp_node->get_Sched_Info()->get_Current());
      temp_1 = Subtract(temp_1, visited_inter);
      to_visit_intra = Union(to_visit_intra, temp_1);

    }
    if ((int) to_visit_intra.size() > 0)
    {
      temp_node = to_visit_intra[0];
      visited_intra.push_back(temp_node);
      temp_1 = get_Intra_Iteration_Related_Nodes_Current(temp_node, temp_node->get_Sched_Info()->get_Current());
      temp_1 = Subtract(temp_1, visited_intra);
      to_visit_intra = Union(to_visit_intra, temp_1);
      temp_1 = get_Inter_Iteration_Related_Nodes(temp_node);
      temp_1 = get_Current_Scheduled_Nodes(temp_1);
      temp_1 = Subtract(temp_1, to_visit_inter);
      to_visit_inter = Union(to_visit_inter, temp_1);
    }
    to_visit_inter = Subtract(to_visit_inter, visited_inter);
    to_visit_intra = Subtract(to_visit_intra, visited_intra);
  }
  visited_inter = Union(visited_intra, visited_inter);
  temp_1.clear();
  temp_1.push_back(current_node);
  visited_inter = Subtract(visited_inter, temp_1);
  return visited_inter;
}
/*
 * This function finds all nodes connected to current node with inter-iteration dependency arc, these 
 * nodes are to be mapped to the same physical PE
 * FIXME: Related nodes are only the one with true dependency or predicate dependency
 * 20-2-2014
 */
vector<REGI_Node*> REGI_DFG::get_Inter_Iteration_Related_Nodes(REGI_Node* current_node)
{
  vector<REGI_Node*> visited;
  vector<REGI_Node*> rest;
  vector<REGI_Node*> temp_1;
  vector<REGI_Node*> temp_2;
  REGI_Node* temp_node;
  temp_1 = current_node->Get_Explicit_Dependency_Predecessors_Prev_Iterations();
  temp_2 = current_node->Get_Explicit_Dependency_Successors_Next_Iterations();
  rest = Union(temp_2, temp_1);
  while ((int) rest.size() > 0)
  {
    temp_node = rest[0];
    visited.push_back(temp_node);
    Remove(rest, temp_node);
    temp_1 = temp_node->Get_Explicit_Dependency_Predecessors_Prev_Iterations();
    temp_1 = Subtract(temp_1, visited);
    rest = Union(rest, temp_1);
    temp_1 = temp_node->Get_Explicit_Dependency_Successors_Next_Iterations();
    temp_1 = Subtract(temp_1, visited);
    rest = Union(rest, temp_1);
  }
  return visited;
}
/*
 * This function finds all nodes connected to current node with regular arc, these nodes are not scheduled 
 * in consecutive cycles so must be mapped to same physical PEs
 */
vector<REGI_Node*> REGI_DFG::get_Intra_Iteration_Related_Nodes_Current(REGI_Node* current_node, int time)
{

  vector<REGI_Node*> rest;
  vector<REGI_Node*> temp_1;
  vector<REGI_Node*> temp_2;

  temp_1 = current_node->Get_Prev_Nodes_Same_Iteration();
  for (int i = 0; i < (int) temp_1.size(); i++)
  {
    if (temp_1[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      if (time - temp_1[i]->get_Sched_Info()->get_Current() > 1)
      {
        rest.push_back(temp_1[i]);
      }
    }
  }
  temp_2 = current_node->Get_Next_Nodes_Same_Iteration();
  for (int i = 0; i < (int) temp_2.size(); i++)
  {
    if (temp_2[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      if (temp_2[i]->get_Sched_Info()->get_Current() - time > 1)
      {
        rest.push_back(temp_2[i]);
      }
    }
  }

  return rest;

}
/*
 * find node conflict (2 nodes that should be mapped to same physical PE) between already scheduled 
 * operations and current node; to be used in FEASIBLE_ASAP_SCHEDULE only
 * (1) node for which check for conflict (2) time to schedule the node
 */
bool REGI_DFG::has_Node_Conflict_Feasible_ASAP(REGI_Node* current_node, int time)
{
  vector<REGI_Node*> related = get_Inter_Iteration_Related_Nodes(current_node);
  for (int i = 0; i < (int) related.size(); i++)
  {
    if (related[i]->get_Sched_Info()->is_Feasible_ASAP_Initiated())
    {

      if (related[i]->get_Sched_Info()->get_Feasible_ASAP() == time)
      {
        return true;
      }
    }
  }
  return false;
}
/*
 * find node conflict (2 nodes that should be mapped to same physical PE) between already scheduled 
 * operations and current node; to be used in FEASIBLE_ALAP_SCHEDULE only
 * (1) node for which check for conflict (2) time to schedule the node
 */

bool REGI_DFG::has_Node_Conflict_Feasible_ALAP(REGI_Node* current_node, int time)
{
  vector<REGI_Node*> related = get_Inter_Iteration_Related_Nodes(current_node);
  for (int i = 0; i < (int) related.size(); i++)
  {
    if (related[i]->get_Sched_Info()->is_Feasible_ALAP_Initiated())
    {
      if (related[i]->get_Sched_Info()->get_Feasible_ALAP() == time)
      {
        return true;
      }
    }
  }
  return false;
}
/*
 * find if there is a conflict with node src node to be scheduled with all scheduled nodes
 * if one is found, returns true
 */
bool REGI_DFG::has_node_conflict_with_scheduled_nodes(REGI_Node* current, int schedule_time, int modulo_schedule, vector<REGI_Node*> &scheduled_nodes, REGI_Node* &node_in_conflict_path)
{
  if (has_node_forward_conflict_with_scheduled_nodes(current, schedule_time, modulo_schedule, scheduled_nodes,node_in_conflict_path))
  {
    return true;
  }
  if (has_node_backward_conflict_with_scheduled_nodes(current, schedule_time, modulo_schedule, scheduled_nodes, node_in_conflict_path))
  {
    return true;
  }
  return false;
}
/*
 * check if current node that is to be scheduled has conflict in forward direction with already scheduled nodes
 * if so return true and first node in conflict path
 */
bool REGI_DFG::has_node_forward_conflict_with_scheduled_nodes(REGI_Node* current, int schedule_time, int modulo_schedule, vector<REGI_Node*> &scheduled_nodes, REGI_Node* &first_node_in_conflict_path)
{
  for (int i = 0; i < (int) scheduled_nodes.size(); i++)
  {
    if (has_forward_conflict(current, schedule_time, modulo_schedule, scheduled_nodes[i], scheduled_nodes[i]->get_Sched_Info()->get_Current(), scheduled_nodes[i]->get_Sched_Info()->get_Modulo_Current(), first_node_in_conflict_path))
    {
      return true;
    }
  }
  return false;
}
/*
 * check if current node that is to be scheduled has conflict in backward direction with already scheduled nodes
 * if so return true and last node in conflict path
 */
bool REGI_DFG::has_node_backward_conflict_with_scheduled_nodes(REGI_Node* current, int schedule_time, int modulo_schedule, vector<REGI_Node*> &scheduled_nodes, REGI_Node* &last_node_in_conflict_path)
{
  for (int i = 0; i < (int) scheduled_nodes.size(); i++)
  {
    if (has_backward_conflict(scheduled_nodes[i], scheduled_nodes[i]->get_Sched_Info()->get_Current(), scheduled_nodes[i]->get_Sched_Info()->get_Modulo_Current(), current, schedule_time, modulo_schedule, last_node_in_conflict_path))
    {
      return true;
    }
  }
  return false;
}
/*
 * recursively explored the DFG in forward direction
 * only travers nodes that are already scheduled, and are consrtained by src
 * finds a contrain conflict
 * (1) src, a scheduled node (2) target node
 * (3) set of all visited nodes
 * FIXME: travers only through actual arcs 20-2-2014
 *
 */
bool REGI_DFG::traverse_constrain_path_recursive(REGI_Node* src, REGI_Node* target, vector<REGI_Node*> &visited)
{
  //if we reached to target, there is a path
  if (src == target)
    return true;
  //if already scheduled, return false
  if (contains(visited, src))
  {
    return false;
  }
  //add src to set of visited nodes
  visited.push_back(src);
  //find successors of scr in this iteration
  vector<REGI_Node*> successors = src->Get_Explicit_Dependency_Successors_Same_Iteration();
  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) successors.size(); i++)
  {
    //ignore unscheduled operations
    if (!successors[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      continue;
    }
    //check if src constrains next node
    if (src->get_Sched_Info()->get_Current() < successors[i]->get_Sched_Info()->get_Current() - src->get_Latency())
    {
      //can reach target from next?
      if (traverse_constrain_path_recursive(successors[i], target, visited))
      {
        return true;
      }
    }
  }
  //get next nodes in next iterations
  successors = src->Get_Explicit_Dependency_Successors_Next_Iterations();
  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) successors.size(); i++)
  {
    //ignore unscheduled operations
    if (!successors[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      continue;
    }
    //can reach target from next?
    if (traverse_constrain_path_recursive(successors[i], target, visited))
    {
      return true;
    }
  }
  return false;
}

/*
 * find a constrain conflict between src and target, src is not scheduled yet but target is scheduled
 * this function explores the DFG graph in forward direction to find a conflict
 * (1) src node (2) time src node is scheduled (3) modulo scheduled cycle of src
 * (4) target node (5) target schedule cycle (6) target modulo cycle
 * (7) if there is a conflict, first node in conflict path
 * FIXME: travers only through actual arcs 20-2-2014
 */
bool REGI_DFG::has_forward_conflict(REGI_Node* src, int src_schedule_cycle, int src_modulo_cycle, REGI_Node* target, int target_schedule_cycle, int target_modulo_cycle, REGI_Node* &first_node_in_conflict_path)
{
  //temp storage to travers DFG
  vector<REGI_Node*> visited;
  //find successors of scr in this iteration
  vector<REGI_Node*> successors = src->Get_Explicit_Dependency_Successors_Same_Iteration();
  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) successors.size(); i++)
  {
    //ignore unscheduled operations
    if (!successors[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      continue;
    }
    //check if src constrains next node
    if (src_schedule_cycle < successors[i]->get_Sched_Info()->get_Current() - src->get_Latency())
    {
      //can reach target from next?
      if (traverse_constrain_path_recursive(successors[i], target, visited))
      {
        //if reachable, must be scheduled at different modulo cycle!
        if (src_modulo_cycle == target_modulo_cycle)
        {
          first_node_in_conflict_path = successors[i];
          return true;
        }
      }
      //clear visited nodes
      visited.clear();
    }
  }
  //get next nodes in next iterations
  successors = src->Get_Explicit_Dependency_Successors_Next_Iterations();
  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) successors.size(); i++)
  {
    //ignore unscheduled operations
    if (!successors[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      continue;
    }
    //can reach target from next?
    if (traverse_constrain_path_recursive(successors[i], target, visited))
    {
      //if reachable, must be scheduled at different modulo cycle!
      if (src_modulo_cycle == target_modulo_cycle)
      {
        first_node_in_conflict_path = successors[i];
        return true;
      }
    }
    //clear set of visited nodes
    visited.clear();
  }
  return false;
}

/*
 * todo if there is a path of constrain may be we should break it!
 * find a constrain conflict between src and target, target is not scheduled yet but target is scheduled
 * this function explores the DFG graph in forward direction to find a conflict
 * (1) src node (2) time src node is scheduled (3) modulo scheduled cycle of src
 * (4) target node (5) target schedule cycle (6) target modulo cycle
 * (7) if there is a conflict, first node in conflict path
 * FIXME: only travers through actual arcs 20-2-2014
 */
bool REGI_DFG::has_backward_conflict(REGI_Node* src, int src_schedule_cycle, int src_modulo_cycle, REGI_Node* target, int target_schedule_cycle, int target_modulo_cycle, REGI_Node* &last_node_in_conflict_path)
{
  //temp storage to travers DFG
  vector<REGI_Node*> visited;
  //find successors of scr in this iteration
  vector<REGI_Node*> predecessors = target->Get_Explicit_Dependency_Predecessors_Same_Iteration();
  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) predecessors.size(); i++)
  {
    //ignore unscheduled operations
    if (!predecessors[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      continue;
    }
    //check if src constrains next node
    if (predecessors[i]->get_Sched_Info()->get_Current() + predecessors[i]->get_Latency() < target_schedule_cycle)
    {
      //can reach target from next?
      if (traverse_constrain_path_recursive(src, predecessors[i], visited))
      {
        //if reachable, must be scheduled at different modulo cycle!
        if (src_modulo_cycle == target_modulo_cycle)
        {
          last_node_in_conflict_path = predecessors[i];
          return true;
        }
      }
      //clear visited nodes
      visited.clear();
    }
  }
  //get next nodes in next iterations
  predecessors = target->Get_Explicit_Dependency_Predecessors_Prev_Iterations();
  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) predecessors.size(); i++)
  {
    //ignore unscheduled operations
    if (!predecessors[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      continue;
    }
    //can reach target from next?
    if (traverse_constrain_path_recursive(src, predecessors[i], visited))
    {

      //if reachable, must be scheduled at different modulo cycle!
      if (src_modulo_cycle == target_modulo_cycle)
      {
        last_node_in_conflict_path = predecessors[i];
        return true;
      }
    }
    //clear set of visited nodes
    visited.clear();
  }
  return false;
}
/*
 * check if current node that is to be scheduled has conflict in forward direction with already scheduled nodes
 * if so return true and first node in conflict path
 */
bool REGI_DFG::constrains_multiple(REGI_Node* current, int schedule_time, REGI_Node* &first_node_in_constrain_path, vector<REGI_Node*> &scheduled_nodes)
{
  int counter=0;
  for (int i=0; i <(int)scheduled_nodes.size(); i++)
  {
    if (constrains(current, schedule_time, scheduled_nodes[i], scheduled_nodes[i]->get_Sched_Info()->get_Current(), scheduled_nodes[i]->get_Sched_Info()->get_Modulo_Current(),first_node_in_constrain_path))
    {
      counter++;
    }
    if (counter > 2)
    {
      return true;
    }
  }
  return false;
}
/*
 * check if current node that is to be scheduled has conflict in
 * backward direction with already scheduled nodes
 * if so return true and last node in conflict path
 */
bool REGI_DFG::has_multiple_constrains(REGI_Node* current, int schedule_time, REGI_Node* &last_node_in_constrain_path, vector<REGI_Node*> &scheduled_nodes)
{
  int counter = 0;
  for (int i = 0; i < (int) scheduled_nodes.size(); i++)
  {
    if (is_constrained(scheduled_nodes[i], scheduled_nodes[i]->get_Sched_Info()->get_Current(), scheduled_nodes[i]->get_Sched_Info()->get_Modulo_Current(), current, schedule_time, last_node_in_constrain_path))
    {
      counter++;
    }
    if (counter > 2)
    {
      return true;
    }
  }
  return false;
}
/*
 * find constrain path between src and target
 * target is already scheduled but src is not
 * if a path is found, first_node_in_conflict_path is the first node in this path
 */

bool REGI_DFG::constrains(REGI_Node* src, int src_schedule_cycle, REGI_Node* target, int target_schedule_cycle, int target_modulo_cycle, REGI_Node* &first_node_in_conflict_path)
{
  //temp storage to travers DFG
  vector<REGI_Node*> visited;
  //find successors of scr in this iteration
  vector<REGI_Node*> successors = src->Get_Next_Nodes_Same_Iteration();
  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) successors.size(); i++)
  {
    //ignore unscheduled operations
    if (!successors[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      continue;
    }
    //check if src constrains next node
    if (src_schedule_cycle < successors[i]->get_Sched_Info()->get_Current() - src->get_Latency())
    {
      //can reach target from next?
      if (traverse_constrain_path_recursive(successors[i], target, visited))
      {
        first_node_in_conflict_path = successors[i];
        return true;
      }
      //clear visited nodes
      visited.clear();
    }
  }
  //get next nodes in next iterations
  successors = src->Get_Next_Nodes_Next_Iteration();
  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) successors.size(); i++)
  {
    //ignore unscheduled operations
    if (!successors[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      continue;
    }
    //can reach target from next?
    if (traverse_constrain_path_recursive(successors[i], target, visited))
    {
      first_node_in_conflict_path = successors[i];
      return true;
    }
    //clear set of visited nodes
    visited.clear();
  }
  return false;
}

/*
 * find constrain path between src and target
 * src is already scheduled but target is not
 * if a path is found, last_node_in_conflict_path is the last node in this path
 */
bool REGI_DFG::is_constrained(REGI_Node* src, int scr_schedule_cycle, int src_modulo_cycle, REGI_Node* target, int target_schedule_cycle, REGI_Node* &last_node_in_conflict_path)
{
  //temp storage to travers DFG
  vector<REGI_Node*> visited;
  //find successors of scr in this iteration
  vector<REGI_Node*> predecessors = target->Get_Prev_Nodes_Same_Iteration();
  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) predecessors.size(); i++)
  {
    //ignore unscheduled operations
    if (!predecessors[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      continue;
    }
    //check if src constrains next node
    if (predecessors[i]->get_Sched_Info()->get_Current() + predecessors[i]->get_Latency() < target_schedule_cycle)
    {
      //can reach target from next?
      if (traverse_constrain_path_recursive(src, predecessors[i], visited))
      {
        last_node_in_conflict_path = predecessors[i];
        return true;
      }
      //clear visited nodes
      visited.clear();
    }
  }
  //get next nodes in next iterations
  predecessors = target->Get_Prev_Nodes_Prev_Iteration();
  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) predecessors.size(); i++)
  {
    //ignore unscheduled operations
    if (!predecessors[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      continue;
    }
    //can reach target from next?
    if (traverse_constrain_path_recursive(src, predecessors[i], visited))
    {
      last_node_in_conflict_path = predecessors[i];
      return true;
    }
    //clear set of visited nodes
    visited.clear();
  }
  return false;
}
/*
 * return a modulo cycle of give input time
 */


int REGI_DFG::get_Modulo_Next_Cycle(int time, int II)
{
  if (II == 1)
  {
    return 0;
  }
  return ((time + 1) % II);
}
/*
 * return the modulo cycle of the previous cycle represented by time
 */
int REGI_DFG::get_Modulo_Prev_Cycle(int time, int II)
{
  if (II == 1)
  {
    return 0;
  }
  if ((time - 1) < 0)
  {
    return II - 1;
  }
  else
    return ((time - 1) % II);
}
/*
 * returns the number of required register between two operations
 * node1 and node2 and given II
 */

int REGI_DFG::get_Required_Register(REGI_Node* node1, REGI_Node* node2, int II)
{
  int inter = 0;
  int intra = 0;
  //is it inter iteration(loop carried) dependency
  if (get_Arc(node1, node2)->get_Distance() > 0)
  {

    //node1 is scheduled before node2
    if (node1->get_Sched_Info()->get_Current() < node2->get_Sched_Info()->get_Current())
    {
      inter = 1;
      //reg=dis-(T(n2)-T(n1))/II
      inter = inter - (int) ((node2->get_Sched_Info()->get_Current() - node1->get_Sched_Info()->get_Current()) / II);
      inter = inter + get_Arc(node1, node2)->get_Distance();
    }
    else
    {

      //reg=dis-(T(n1)-T(n2))/II
      inter = ((node1->get_Sched_Info()->get_Current() - node2->get_Sched_Info()->get_Current() - 1) / II);
      inter++;
      inter = inter + get_Arc(node1, node2)->get_Distance();

    }
    FATAL(inter<0, "The number of required register cannot be negative");
    return inter;
  }
  //intra iteration dependency
  else
  {
    if (node2->get_Sched_Info()->get_Current() - node1->get_Sched_Info()->get_Current()==1)
    {
      return 0;
    }
    else
    {
      intra = (int) ((node2->get_Sched_Info()->get_Current() - node1->get_Sched_Info()->get_Current() - 1) / II);
      intra++;
      return intra;
    }
  }
}

//*************************** Simulated Annealing *********************//
void REGI_DFG::Set_Height()
{
  bool no_change = true;
  bool ready = false;
  vector<REGI_Node*> temp_nodes;
  int max_time = 0;
  while (true)
  {
    for (int i = 0; i < (int) _node_Set.size(); i++)
    {
      if (!_node_Set[i]->get_Sched_Info()->is_Height_Initiated())
      {
        ready = true;
        temp_nodes = _node_Set[i]->Get_Next_Nodes_Same_Iteration();
        if ((int) temp_nodes.size() == 0)
        {
          _node_Set[i]->get_Sched_Info()->set_Height(0);
        }
        else
        {

          for (int j = 0; j < (int) temp_nodes.size(); j++)
          {
            if (!temp_nodes[j]->get_Sched_Info()->is_Height_Initiated())
            {
              ready = false;
              break;
            }
            if (temp_nodes[j]->get_Sched_Info()->get_Height() > max_time)
            {
              max_time = temp_nodes[j]->get_Sched_Info()->get_Height();
            }

          }
          if (ready)
          {
            _node_Set[i]->get_Sched_Info()->set_Height(max_time + _node_Set[i]->get_Latency());
            no_change = false;

          }

        }
      }
    }
    if (no_change)
    {
      break;
    }
  }
}

vector<REGI_Node*> REGI_DFG::Simulated_Annealing_SortOps(int &MII, int number_of_resources)
{

  int recMII = 0, resMII = 0;
  vector < vector<REGI_Node*> > Sccs;

  vector < vector<REGI_Node*> > Sorted_Sccs;
  Strongly_Connected (Sccs);
  //PrintSets(Sccs);
  Sort_Sccs(Sccs);
  //PrintSets(Sccs);
  if (Sccs.size() > 0)
  {
    recMII = Sccs[0][0]->get_Sched_Info()->get_SCC_Delay();
  }
  resMII = (_node_Set.size() - 1) / number_of_resources;
  resMII++;

  if (recMII > resMII)
  {
    MII = recMII;
  }
  else
    MII = resMII;

  Reset_ASAP_Schedule();
  int last_time = Schedule_ASAP();

  DEBUG_DOTPRINT_ASAP_SCHEDULE("ASAP" , -1);

  Reset_ALAP_Schedule();
  Schedule_ALAP(last_time);

  DEBUG_DOTPRINT_ALAP_SCHEDULE("ALAP", -1);

  vector < vector<REGI_Node*> > Visited_Sccs;
  vector<REGI_Node*> Current_nodes_in_path;

  for (int i = 0; i < (int) Sccs.size(); i++)
  {
    if ((int) Sccs[i].size() == 1)
      continue;
    Current_nodes_in_path = get_path_between(Visited_Sccs, Sccs[i]);
    for (int j = 0; j < (int) Visited_Sccs.size(); j++)
    {
      Current_nodes_in_path = Subtract(Current_nodes_in_path, Visited_Sccs[j]);
    }
    Visited_Sccs.push_back(Current_nodes_in_path);
    Current_nodes_in_path = order_nodes_SCC(Sccs[i]);
    for (int j = 0; j < (int) Visited_Sccs.size(); j++)
    {
      Current_nodes_in_path = Subtract(Current_nodes_in_path, Visited_Sccs[j]);
    }
    Visited_Sccs.push_back(Current_nodes_in_path);

  }
  vector<REGI_Node*> unordered_nodes = _node_Set;
  for (int j = 0; j < (int) Visited_Sccs.size(); j++)
  {
    unordered_nodes = Subtract(unordered_nodes, Visited_Sccs[j]);
  }

  REGI_Node* current;
  int max_time = -1;
  vector<REGI_Node*> rest;
  vector<REGI_Node*> temp;
  vector<REGI_Node*> backward_sweep_visited;
  vector<REGI_Node*> forward_sweep_visited;
  vector<REGI_Node*> backward_sweep_tovisit;
  vector<REGI_Node*> forward_sweep_tovisit;

  REGI_Node* backward_current;
  REGI_Node* forward_current;

  while ((int) unordered_nodes.size() > 0)
  {
    max_time = -1;
    for (int j = 0; j < (int) unordered_nodes.size(); j++)
    {
      if (unordered_nodes[j]->get_Sched_Info()->get_ASAP() > max_time)
      {
        current = unordered_nodes[j];
        max_time = current->get_Sched_Info()->get_ASAP();
      }
    }
    rest.push_back(current);
    backward_sweep_visited.push_back(current);
    forward_sweep_visited.push_back(current);

    backward_sweep_tovisit = current->Get_Prev_Nodes_Same_Iteration();

    Sort_Ops_by_Depth(backward_sweep_tovisit);
    rest = Union(rest, backward_sweep_tovisit);
    forward_sweep_tovisit = backward_sweep_tovisit;
    while (true)
    {
      while ((int) backward_sweep_tovisit.size() > 0)
      {
        backward_current = backward_sweep_tovisit[0];
        backward_sweep_visited.push_back(backward_current);
        temp = backward_current->Get_Prev_Nodes_Same_Iteration();
        Sort_Ops_by_Depth(temp);
        rest = Union(rest, temp);
        backward_sweep_tovisit = Union(backward_sweep_tovisit, temp);
        forward_sweep_tovisit = Union(forward_sweep_tovisit, temp);
        backward_sweep_tovisit = Subtract(backward_sweep_tovisit, backward_sweep_visited);
        forward_sweep_tovisit = Subtract(forward_sweep_tovisit, forward_sweep_visited);
      }
      while ((int) forward_sweep_tovisit.size() > 0)
      {
        forward_current = forward_sweep_tovisit[0];
        forward_sweep_visited.push_back(forward_current);
        temp = forward_current->Get_Next_Nodes_Same_Iteration();
        Sort_Ops_by_Depth(temp);
        rest = Union(rest, temp);
        backward_sweep_tovisit = Union(backward_sweep_tovisit, temp);
        forward_sweep_tovisit = Union(forward_sweep_tovisit, temp);
        backward_sweep_tovisit = Subtract(backward_sweep_tovisit, backward_sweep_visited);
        forward_sweep_tovisit = Subtract(forward_sweep_tovisit, forward_sweep_visited);
      }
      if ((int) forward_sweep_tovisit.size() == 0 && (int) backward_sweep_tovisit.size() == 0)
      {
        break;
      }
    }
    unordered_nodes = Subtract(unordered_nodes, rest);

  }
  vector<REGI_Node*> retVal;

  for (int j = 0; j < (int) Visited_Sccs.size(); j++)
  {
    retVal = Union(retVal, Visited_Sccs[j]);
    rest = Subtract(rest, Visited_Sccs[j]);
  }
  retVal = Union(retVal, rest);
  return retVal;

}

void REGI_DFG::Sort_Sccs(vector<vector<REGI_Node*> > &sets)
{
  vector<REGI_Node*> temp;
  int delay;
  int II;
  for (int i = 0; i < (int) sets.size(); i++)
  {
    delay = Longest_Path(sets[i], II);
    delay = (delay - 1) / II;
    delay++;
    sets[i][0]->get_Sched_Info()->set_SCC_Delay(delay);

  }

  for (int i = 0; i < (int) sets.size(); i++)
  {
    for (int j = i + 1; j < (int) sets.size(); j++)
    {
      if (sets[j][0]->get_Sched_Info()->get_SCC_Delay() > sets[i][0]->get_Sched_Info()->get_SCC_Delay())
      {
        temp = sets[i];
        sets[i] = sets[j];
        sets[j] = temp;
      }
    }

  }
}

//all SCCs, not necessarily seperated sets
void REGI_DFG::Sort_Ops_by_Depth(vector<REGI_Node*> &set)
{
  REGI_Node* temp;
  for (int i = 0; i < (int) set.size(); i++)
  {
    for (int j = i + 1; j < (int) set.size(); j++)
    {
      if (set[j]->get_Sched_Info()->get_ASAP() > set[i]->get_Sched_Info()->get_ASAP())
      {
        temp = set[i];
        set[i] = set[j];
        set[j] = temp;
      }
    }

  }

}

vector<REGI_Node*> REGI_DFG::get_path_between(vector<vector<REGI_Node*> > &visited, vector<REGI_Node*> &to_visit)
{
  vector<REGI_Node*> retval;

  vector<REGI_Node*> temp_set;
  vector<REGI_Node*> temp_set_2;
  vector<REGI_Node*> temp_set_3;
  vector<REGI_Node*> temp_set_4;

  for (int i = 0; i < (int) to_visit.size(); i++)
  {
    //all successors of selected node
    temp_set = get_all_successors(to_visit[i]);
    for (int j = 0; j < (int) visited.size(); j++)
    {
      //this successors have any node in common with a SCC
      temp_set_2 = Intersect(temp_set, visited[j]);
      if ((int) temp_set_2.size() > 0)
      {
        //if so, find all common nodes
        for (int k = 0; k < (int) temp_set.size(); k++)
        {
          temp_set_3 = get_all_successors(temp_set[k]);
          temp_set_4 = Intersect(temp_set_3, visited[j]);
          if ((int) temp_set_4.size() > 0)
          {
            if (!contains(retval, temp_set[k]))
            {
              retval.push_back(temp_set[k]);
            }
          }

        }

      }
    }

    //all predecessors of selected node
    temp_set = get_all_predecessors(to_visit[i]);
    for (int j = 0; j < (int) visited.size(); j++)
    {
      //this predecessors have any node in common with a SCC
      temp_set_2 = Intersect(temp_set, visited[j]);
      if ((int) temp_set_2.size() > 0)
      {
        //if so, find all common nodes
        for (int k = 0; k < (int) temp_set.size(); k++)
        {
          temp_set_3 = get_all_predecessors(temp_set[k]);
          temp_set_4 = Intersect(temp_set_3, visited[j]);
          if ((int) temp_set_4.size() > 0)
          {
            if (!contains(retval, temp_set[k]))
            {
              retval.push_back(temp_set[k]);
            }
          }

        }

      }
    }

  }
  return retval;
}

vector<REGI_Node*> REGI_DFG::get_all_successors(REGI_Node* &current)
{
  vector<REGI_Node*> temp_set;
  vector<REGI_Node*> temp_set_2;
  vector<REGI_Node*> visited;
  REGI_Node* temp_node;
  temp_set = current->Get_Next_Nodes_Same_Iteration();
  while ((int) temp_set.size() > 0)
  {
    temp_node = temp_set[0];
    visited.push_back(temp_node);
    temp_set_2 = temp_node->Get_Next_Nodes_Same_Iteration();
    temp_set = Union(temp_set, temp_set_2);
    temp_set = Subtract(temp_set, visited);
  }
  return visited;

}
vector<REGI_Node*> REGI_DFG::get_all_predecessors(REGI_Node* &current)
{
  vector<REGI_Node*> temp_set;
  vector<REGI_Node*> temp_set_2;
  vector<REGI_Node*> visited;
  REGI_Node* temp_node;
  temp_set = current->Get_Prev_Nodes_Same_Iteration();
  while ((int) temp_set.size() > 0)
  {
    temp_node = temp_set[0];
    visited.push_back(temp_node);
    temp_set_2 = temp_node->Get_Prev_Nodes_Same_Iteration();
    temp_set = Union(temp_set, temp_set_2);
    temp_set = Subtract(temp_set, visited);
  }
  return visited;

}

vector<REGI_Node*> REGI_DFG::order_nodes_SCC(vector<REGI_Node*> &scc)
{
  vector<REGI_Node*> retVal = scc;
  REGI_Node* temp_node;
  for (int i = 0; i < (int) retVal.size(); i++)
  {
    for (int j = i; j < (int) retVal.size(); j++)
    {
      if (retVal[j]->get_Sched_Info()->get_ALAP() > retVal[i]->get_Sched_Info()->get_ALAP())
      {
        temp_node = retVal[i];
        retVal[i] = retVal[j];
        retVal[j] = temp_node;
      }
    }

  }
  return retVal;

}

//*************************** Simulated Annealing *********************//

//*************************** DFG Minimization *********************//
/*
 * identify set of nodes that can potentially be packed and form dual-issue nodes
 */
void REGI_DFG::Update_Minimizable_Nodes()
{
  //initially no node is minimizable
  Reset_Minimized_Nodes();
  bool change;
  while (true)
  {
    change = false;
    bool all_minimizable = true;
    vector<REGI_Node*> next_nodes;
    for (vector<REGI_Node*>::const_iterator it = _node_Set.begin(); it!= _node_Set.end(); ++it)
    {
      //assume the selected node is minimizable unless proved otherwise
      all_minimizable = true;
      if ((*it)->is_Minimizable())
      {
        continue;
      }
      //phi nodes are minimizable
      if ((*it)->is_PHI_Operations())
      {
        (*it)->set_Minimizable();
        change = true;
        continue;
      }
      //reduce nodes are minimizable
      if (is_Reduced_Node(*it))
      {
        (*it)->set_Minimizable();
        change = true;
        continue;
      }
      if ((*it)->is_Mem_Operation())
      {

        //get relatd node
        REGI_Node* related = (*it)->get_Related_Node();
        //get next nodes
        next_nodes = (*it)->Get_Next_Nodes();
        for (vector<REGI_Node*>::const_iterator nextIt = next_nodes.begin(); nextIt != next_nodes.end(); ++nextIt)
        {
          //if one of next nodes is note minimizable, this node is not minimizable too
          if (!(*nextIt)->is_Minimizable())
          {
            all_minimizable = false;
          }
          //todo, why if dependency is pred, node is not minimizable?
          if (get_Arc(*it, *nextIt)->get_Dependency() == PredDep)
          {
            all_minimizable = false;
          }
        }
        //get next nodes to the related
        next_nodes = related->Get_Next_Nodes();
        for (vector<REGI_Node*>::const_iterator nextIt = next_nodes.begin(); nextIt != next_nodes.end(); ++nextIt)
        {

          //if one of next nodes is note minimizable, this node is not minimizable too
          if (!(*nextIt)->is_Minimizable())
          {
            all_minimizable = false;
          }
          //todo, why if dependency is pred, node is not minimizable?
          if (get_Arc(related, *nextIt)->get_Dependency() == PredDep)
          {
            all_minimizable = false;
          }

        }
        if (all_minimizable)
        {
          (*it)->set_Minimizable();
          change = true;
          continue;
        }
      }

      next_nodes = (*it)->Get_Next_Nodes();
      if ((int) next_nodes.size() == 0)
        continue;
      for (vector<REGI_Node*>::const_iterator nextIt = next_nodes.begin(); nextIt != next_nodes.end(); ++nextIt)
      {
        if (!(*nextIt)->is_PHI_Operations() && !is_Reduced_Node(*nextIt))
        {
          all_minimizable = false;
        }
        if (get_Arc((*it), *nextIt)->get_Dependency() == PredDep)
        {
          all_minimizable = false;
        }
      }
      if (all_minimizable)
      {
        (*it)->set_Minimizable();
        change = true;
      }
    }
    if (!change)
    {
      break;
    }
  }
}

void REGI_DFG::Reduce_DFG()
{
  while (true)
  {
    Init_Schedule(0, CGRA_Y_Dim * CGRA_X_Dim);
    Update_Minimizable_Nodes();
    bool minimizable;
    bool change = false;
    for (int i = 0; i < (int) _node_Set.size(); i++)
    {
      if (_node_Set[i]->is_PHI_Operations())
      {

        REGI_DEBUG(true,  "Inside PHI %d", _node_Set[i]->get_ID());
        minimizable = true;
        REGI_Node* left = NULL;
        REGI_Node* right = NULL;
        vector<REGI_Node*> inputs = _node_Set[i]->Get_Prev_Nodes();
        for (int j = 0; j < (int) inputs.size(); j++)
        {

          REGI_DEBUG(true,  "startg");
          if (get_Arc(inputs[j], _node_Set[i])->get_Dependency() == PredDep)
          {

            REGI_DEBUG(true,  "PRED");
            continue;
          }
          //we cannot reduce a reduced node

          REGI_DEBUG(true,  "checking");
          if (is_Reduced_Node(inputs[j]) || !inputs[j]->is_Minimizable())
          {
            minimizable = false;
            break;
          }
          if (left == NULL)
          {
            left = inputs[j];
            REGI_DEBUG(true,  "LEFT");
            continue;
          }
          if (right == NULL)
          {
            right = inputs[j];
            REGI_DEBUG(true,  "Right");
            continue;
          }
          cout << "ERROR in DFG Minimization\n";
        }

        REGI_DEBUG(true,  "DONE");
        if (right==NULL ||  left==NULL)
        {
          REGI_DEBUG(true,  "Right");
          continue;
        }
        //is reducable if it is minimizable and right and left has only arc to phi node
        if (minimizable && right->get_Number_of_Succ() == 1 && left->get_Number_of_Succ() == 1)
        {

          REGI_DEBUG(true,  "To Minimize");
          Dot_Print_DFG("BCHNG", 0);
          REGI_Reduced_Node* newNode;
          vector<REGI_ARC*> to_remove;
          vector<REGI_ARC*> to_add;
          newNode = new REGI_Reduced_Node(Get_Unique_Index(), left, right, _node_Set[i], to_remove, to_add);
          delete_Node(_node_Set[i]);
          insert_Node(newNode);
          delete_Node(left);
          delete_Node(right);
          Remove_Arcs(to_remove);
          Add_Arcs(to_add);

          REGI_DEBUG(true,  "Updated");
          Dot_Print_DFG("ACHNG", 0);
          change = true;
          i = i - 1;
          continue;

        };

      }
      else if (is_Reduced_Node(_node_Set[i]))
      {

        REGI_DEBUG(true,  "Inside reduced %d", _node_Set[i]->get_ID());
        REGI_Reduced_Node* tempNode = dynamic_cast<REGI_Reduced_Node*>(_node_Set[i]);

        cout<< "Casted\n";
        vector<REGI_Node*> left_inputs = tempNode->Get_Left_Inputs_Sorted_by_Arc();
        cout << "Left inputs\n";
        vector<REGI_Node*> right_inputs = tempNode->Get_Right_Inputs_Sorted_by_Arc();
        cout << "Right inputs\n";
        //check if left or right is empty should not happen
        if ((int) left_inputs.size() == 0 || (int) right_inputs.size() == 0)
        {
          continue;
        }

        for (int j = 0; j < (int) left_inputs.size(); j++)
        {

          int best_cost = 100;
          int best_index = -1;
          int cost;
          //we cannot reduce a reduced node or not minimizable one
          if (!contains(_node_Set, left_inputs[j]) || !left_inputs[j]->is_Minimizable() || left_inputs[j]->get_Number_of_Succ() > 1)
          {
            continue;
          }
          //find the best suitable right not to reduce with
          for (int k = 0; k < (int) right_inputs.size(); k++)
          {

            //if right node actually exist and it is minimizable
            if (!contains(_node_Set, right_inputs[k]) || !right_inputs[k]->is_Minimizable() || right_inputs[k]->get_Number_of_Succ() > 1)
            {
              continue;
            }
            //number of input arcs and cost are proportional, the more inputs the worse cost
            cost = right_inputs[k]->get_Number_of_Pred();
            //find the overlap of schedule between left and right
            int asap = MAX(right_inputs[k]->get_Sched_Info()->get_Feasible_ASAP(),
                left_inputs[j]->get_Sched_Info()->get_Feasible_ASAP());
            int alap = MIN(right_inputs[k]->get_Sched_Info()->get_Feasible_ALAP(),
                left_inputs[j]->get_Sched_Info()->get_Feasible_ALAP());
            int overlap = alap - asap;
            //the more overlap, the better cost
            cost = cost - overlap;
            //this cost is better? so save it as best
            if (cost < best_cost)
            {
              best_cost = cost;
              best_index = k;
            }

          }
          //did we find any suitable right node to reduce with
          if (best_index != -1)
          {

            REGI_Reduced_Node* newNode;
            vector<REGI_ARC*> to_remove;
            vector<REGI_ARC*> to_add;
            newNode = new REGI_Reduced_Node(Get_Unique_Index(), left_inputs[j], right_inputs[best_index], _node_Set[i], to_remove, to_add, true);
            insert_Node(newNode);

            //delet reduced node from node list
            delete_Node(left_inputs[j]);
            //delet reduced node from node list
            delete_Node(right_inputs[best_index]);

            //delet reduced node from left list
            Remove(left_inputs, left_inputs[j]);
            //delet reduced node from right list
            Remove(right_inputs, right_inputs[best_index]);

            Remove_Arcs(to_remove);

            Add_Arcs(to_add);

            change = true;

            j = j - 1;
            continue;
          }
          break;

        }

      }
    }
    if (!change)
    {
      break;
    }
  }
}
//*************************** DFG Minimization *********************//
//old functions not used anymore
/*
   bool REGI_DFG::has_Node_Conflict_Current_new(REGI_Node* current_node, int time, int II)
   {
   vector<REGI_Node*> related = get_All_Nodes_Constrained_by_Current(current_node, time);
   int time_to_check = time % II;
   if (II == 1)
   {
   time_to_check = 0;
   }
   for (int i = 0; i < (int) related.size(); i++)
   {
   if (related[i]->get_Sched_Info()->is_CURRENT_Initiated())
   {
   if ((related[i]->get_Sched_Info()->get_Modulo_Current()) == time_to_check)
   {

   return true;

   }
   }
   }
   return false;
   }

   vector<REGI_Node*> REGI_DFG::get_sorted_ready_list_modulo_forward(vector<REGI_Node*> list)
   {
   vector<REGI_Node*> ready_list;
   vector<REGI_Node*> sorted_list;

   int time;
   for (int i = 0; i < (int) list.size(); i++)
   {
   if (list[i]->is_ready_to_schedule_modulo(time))
   {
//list[i]->get_Sched_Info()->set_CURRENT(time);
list[i]->get_Sched_Info()->set_slack(time);
ready_list.push_back(list[i]);
}
}
int min_slack = MAXINT;
int min_index;

while ((int) ready_list.size() > 0)
{
min_slack = MAXINT;
for (int i = 0; i < (int) ready_list.size(); i++)
{
if (ready_list[i]->get_Sched_Info()->get_slack() < min_slack)
{
min_slack = ready_list[i]->get_Sched_Info()->get_slack();
min_index = i;
}
}
sorted_list.push_back(ready_list[min_index]);
ready_list = Subtract(ready_list, sorted_list);

}
return sorted_list;

}

bool REGI_DFG::cause_Node_Conflict_Current(REGI_Node* current_node, int time, int II)
{
//get all related nodes

vector<REGI_Node*> related = get_All_Nodes_Constrained_by_Current(current_node, time);
REGI_Node* tempDummyNode;

for (int i = 0; i < (int) related.size(); i++)
{
if (related[i]->get_Sched_Info()->is_CURRENT_Initiated())
{
  for (int j = i + 1; j < (int) related.size(); j++)
  {
    if (related[j]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      //if they conflict with each other
      if ((related[i]->get_Sched_Info()->get_Modulo_Current()) == (related[j]->get_Sched_Info()->get_Modulo_Current()))
      {
        //check if the first one is connected to this node
        if (current_node->is_Connected_To(related[i]))
        {
          tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index());
          insert_Node(tempDummyNode);
          insert_Node_in_between_input(current_node, related[i], tempDummyNode);
          return true;
        }
        //check if the second one is connected to this node
        if (current_node->is_Connected_To(related[j]))
        {
          tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index());
          insert_Node(tempDummyNode);
          insert_Node_in_between_input(current_node, related[j], tempDummyNode);
          return true;
        }
        else
        {
          //get all connected nodes (intra) scheduled apart
          vector<REGI_Node*> temp = get_Intra_Iteration_Related_Nodes_Current(current_node, time);
          for (int k = 0; k < (int) temp.size(); k++)
          {
            if (current_node->is_Connected_To(temp[k]))
            {
              vector<REGI_Node*> related_2 = get_All_Nodes_Constrained_by_Current(temp[k], temp[k]->get_Sched_Info()->get_Current());
              //if one inter-iteration related node constrains the first conflicted one
              if (contains(related_2, related[i]))
              {
                tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index());
                insert_Node(tempDummyNode);
                insert_Node_in_between_input(current_node, temp[k], tempDummyNode);
                return true;

              }
              //if one inter-iteration related node constrains the second conflicted one
              if (contains(related_2, related[j]))
              {
                tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index());
                insert_Node(tempDummyNode);
                insert_Node_in_between_input(current_node, temp[k], tempDummyNode);
                return true;

              }
            }

          }
          //get all connected nodes (inter-iteration)
          temp = get_Inter_Iteration_Related_Nodes(current_node);
          temp = get_Current_Scheduled_Nodes(temp);
          for (int k = 0; k < (int) temp.size(); k++)
          {
            if (current_node->is_Connected_To(temp[k]))
            {
              vector<REGI_Node*> related_2 = get_All_Nodes_Constrained_by_Current(temp[k], temp[k]->get_Sched_Info()->get_Current());
              //check if one inter-iteration connected node constrains the first one
              if (contains(related_2, related[i]))
              {
                tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index());
                insert_Node(tempDummyNode);
                insert_Node_in_between_input(current_node, temp[k], tempDummyNode);
                return true;

              }
              //check if one inter-iteration connected node constrains the second one
              if (contains(related_2, related[j]))
              {
                tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index());
                insert_Node(tempDummyNode);
                insert_Node_in_between_input(current_node, temp[k], tempDummyNode);
                return true;

              }
            }
            else if (temp[k]->is_Connected_To(current_node))
            {
              vector<REGI_Node*> related_2 = get_All_Nodes_Constrained_by_Current(temp[k], temp[k]->get_Sched_Info()->get_Current());
              //check if one inter-iteration connected node constrains the first one
              if (contains(related_2, related[i]))
              {
                tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index());
                insert_Node(tempDummyNode);
                insert_Node_in_between_input(temp[k], current_node, tempDummyNode);
                return true;

              }
              //check if one inter-iteration connected node constrains the second one
              if (contains(related_2, related[j]))
              {
                tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index());
                insert_Node(tempDummyNode);
                insert_Node_in_between_input(temp[k], current_node, tempDummyNode);
                return true;

              }
            }
          }

        }

      }

    }
  }

}
}
return false;
}

bool REGI_DFG::fix_Node_Conflict_Current(REGI_Node* current_node, int time, int II)
{
  REGI_Node* tempDummyNode;
  int time_to_check = (time % II);
  if (II == 1)
  {
    time_to_check = 0;
  }
  vector<REGI_Node*> related = get_All_Nodes_Constrained_by_Current(current_node, time);
  for (int i = 0; i < (int) related.size(); i++)
  {
    if (related[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      if ((related[i]->get_Sched_Info()->get_Modulo_Current()) == time_to_check)
      {

        if (current_node->is_Connected_To(related[i]))
        {
          tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index());
          insert_Node(tempDummyNode);
          insert_Node_in_between_input(current_node, related[i], tempDummyNode);
          return true;
        }
        else
        {
          vector<REGI_Node*> temp = get_Intra_Iteration_Related_Nodes_Current(current_node, time);
          for (int j = 0; j < (int) temp.size(); j++)
          {
            if (current_node->is_Connected_To(temp[j]))
            {

              vector<REGI_Node*> related_2 = get_All_Nodes_Constrained_by_Current(temp[j], temp[j]->get_Sched_Info()->get_Current());
              if (contains(related_2, related[i]))
              {
                tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index());
                insert_Node(tempDummyNode);
                insert_Node_in_between_input(current_node, temp[j], tempDummyNode);
                return true;

              }
            }

          }
          temp = get_Inter_Iteration_Related_Nodes(current_node);
          temp = get_Current_Scheduled_Nodes(temp);
          for (int j = 0; j < (int) temp.size(); j++)
          {
            if (current_node->is_Connected_To(temp[j]))
            {
              vector<REGI_Node*> related_2 = get_All_Nodes_Constrained_by_Current(temp[j], temp[j]->get_Sched_Info()->get_Current());
              if (contains(related_2, related[i]))
              {
                tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index());
                insert_Node(tempDummyNode);
                insert_Node_in_between_input(current_node, temp[j], tempDummyNode);
                return true;

              }
            }
            else if (temp[j]->is_Connected_To(current_node))
            {
              vector<REGI_Node*> related_2 = get_All_Nodes_Constrained_by_Current(current_node, temp[j]->get_Sched_Info()->get_Current());
              if (contains(related_2, related[i]))
              {
                tempDummyNode = new REGI_Node_Dummy(1, Get_Unique_Index());
                insert_Node(tempDummyNode);
                insert_Node_in_between_input(temp[j], current_node, tempDummyNode);
                return true;

              }
            }

          }
        }
      }
    }
  }
  return false;

}


bool REGI_DFG::has_Node_Conflict_Current_old(REGI_Node* current_node, int time, int II)
{

  vector<REGI_Node*> related = get_Inter_Iteration_Related_Nodes(current_node);
  int time_to_check = time % II;
  if (II == 1)
  {
    time_to_check = 0;

  }
  for (int i = 0; i < (int) related.size(); i++)
  {
    if (related[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      if ((related[i]->get_Sched_Info()->get_Modulo_Current()) == time_to_check)
      {

        return true;

      }
    }
  }
  related = get_Intra_Iteration_Related_Nodes_Current(current_node, time);
  for (int i = 0; i < (int) related.size(); i++)
  {
    if (related[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      if (related[i]->get_Sched_Info()->get_Current() > time)
      {
        if ((related[i]->get_Sched_Info()->get_Current() - time > 1) && (related[i]->get_Sched_Info()->get_Modulo_Current()) == time_to_check)
        {
          return true;
        }
      }
      else if (related[i]->get_Sched_Info()->get_Current() < time)
      {
        if ((time - related[i]->get_Sched_Info()->get_Current() > 1) && (related[i]->get_Sched_Info()->get_Modulo_Current()) == time_to_check)
        {
          return true;
        }
      }
      else
      {
        return true;
      }

    }
  }
  return false;
}

REGI_Node* REGI_DFG::get_Node_Conflict_Current(REGI_Node* current_node, int time, int II)
{
  vector<REGI_Node*> related = get_Inter_Iteration_Related_Nodes(current_node);
  int time_to_check = time % II;
  if (II == 1)
  {
    time_to_check = 0;
  }
  for (int i = 0; i < (int) related.size(); i++)
  {
    if (related[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      if ((related[i]->get_Sched_Info()->get_Modulo_Current()) == time_to_check)
      {

        return related[i];

      }
    }
  }
  related = get_Intra_Iteration_Related_Nodes_Current(current_node, time);
  for (int i = 0; i < (int) related.size(); i++)
  {
    if (related[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      if (related[i]->get_Sched_Info()->get_Current() > time)
      {
        if ((related[i]->get_Sched_Info()->get_Current() - time > 1) && (related[i]->get_Sched_Info()->get_Modulo_Current()) == time_to_check)
        {
          return related[i];
        }
      }
      else if (related[i]->get_Sched_Info()->get_Current() < time)
      {
        if ((time - related[i]->get_Sched_Info()->get_Current() > 1) && (related[i]->get_Sched_Info()->get_Modulo_Current()) == time_to_check)
        {
          return related[i];
        }
      }
      else
      {
        return related[i];
      }

    }
  }
  return NULL;
}

void REGI_DFG::Update_Required_Registers(int II)
{

  REGI_Node* currentNode;
  //REGI_Node* lastNodeIntra;
  REGI_Node* lastNodeInter;
  vector<REGI_Node*> nextnodes;
  vector<REGI_Node*> nextofnext;

  bool constrainer = false;
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    constrainer = false;
    if ( (int) _node_Set[i]->get_next_operations_constrain_by_this_modulo().size() > 0 )
    {
      currentNode = _node_Set[i];

      nextnodes = currentNode->get_next_operations_constrain_by_this_modulo();

      int lastlevel = 0;
      int maxIterationDistance = 0;

      if ( constrainer )
      {
        for (int j = 0; j < (int) nextnodes.size(); j++)
        {

          if ( get_Arc(currentNode, nextnodes[j])->get_Distance() > maxIterationDistance )
          {
            maxIterationDistance = get_Arc(currentNode, nextnodes[j])->get_Distance();
            lastNodeInter = nextnodes[j];
          }

          if ( get_Arc(currentNode, nextnodes[j])->get_Distance() == 0 && nextnodes[j]->get_Sched_Info()->get_Current() > lastlevel )
          {
            lastlevel = nextnodes[j]->get_Sched_Info()->get_Current();
            //lastNodeIntra = nextnodes[j];
          }
        }
      }

      int intra = 0;
      int inter = 0;

      if ( maxIterationDistance > 0 )
      {
        if ( (currentNode->get_Sched_Info()->get_Modulo_Current()) < lastNodeInter->get_Sched_Info()->get_Modulo_Current() )
        {
          inter = 1;
        }
        inter = inter - (int) ((lastNodeInter->get_Sched_Info()->get_Current() - currentNode->get_Sched_Info()->get_Current()) / II);
        inter = inter + maxIterationDistance;
        if ( inter < 0 )
        {
          cout << "ERROR IN NUMBER OF REGISTERS at REGI_DFG::Update_Required_Registers(int II)" << endl;
        }
      }
      if ( currentNode->has_self_loop() )
      {
        if ( currentNode->get_self_loop()->get_Distance() > inter )
        {
          inter = currentNode->get_self_loop()->get_Distance();
        }
      }
      if ( lastlevel > 0 )
      {
        intra = (int) ((lastNodeInter->get_Sched_Info()->get_Current() - currentNode->get_Sched_Info()->get_Current()) / II);
        if ( (currentNode->get_Sched_Info()->get_Modulo_Current()) < (lastNodeInter->get_Sched_Info()->get_Modulo_Current()) )
        {
          intra = intra + 1;
        }
      }
      if ( inter > intra )
      {
        currentNode->SetRequiredRegister(inter);

      }
      else
      {
        currentNode->SetRequiredRegister(intra);

      }
    }

  }

}


*/

