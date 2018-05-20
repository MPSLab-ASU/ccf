/*
 * DFG.cpp
 *
 * Created on: Jul 24, 2012
 * Author: Mahdi Hamzeh
 *
 * Last edited on: 3 Jan, 2017
 * Author: Shail Dave
 */

#include "DFG.h"
#include "RAMP.h"
#include <math.h>

extern int schedule_id;

DFG::~DFG()
{
}

DFG::DFG(int x_dim, int y_dim)
{
  ARC_Max_Index = 0;
  Node_Max_Index = 0;
  MAX_SCHEDULE_LEN = 0;
  CGRA_X_Dim = x_dim;
  CGRA_Y_Dim = y_dim;
}

/*
 * Make a copy from current DFG
 */
DFG* DFG::Copy()
{
  //copy DFG
  DFG* retVal;
  retVal = new DFG(CGRA_X_Dim, CGRA_Y_Dim);
  Node* tempNode;
  //for over set of nodes
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
      tempNode = new Node(_node_Set[i]->get_Instruction(), _node_Set[i]->get_Latency(), _node_Set[i]->get_ID(), _node_Set[i]->getName());
      tempNode->hasLargeConst = _node_Set[i]->hasLargeConst;
      retVal->insert_Node(tempNode);
  }
  //for over set of constants
  for (int i = 0; i < (int) _constants.size(); i++)
  {
    tempNode = new Node(_constants[i]->get_Instruction(), _constants[i]->get_Latency(), _constants[i]->get_ID(), _node_Set[i]->getName());
    retVal->insert_Constant(tempNode);
  }

  //copy arcs to copy DFG
  for (int i = 0; i < (int) _ARC_Set.size(); i++)
  {
    retVal->make_Arc(retVal->get_Node(_ARC_Set[i]->get_From_Node()->get_ID()), retVal->get_Node(_ARC_Set[i]->get_To_Node()->get_ID()), _ARC_Set[i]->get_Number(), _ARC_Set[i]->get_Distance(), _ARC_Set[i]->get_Dependency(),_ARC_Set[i]->get_OperandOrder());
  }
  //special nodes
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
  retVal->Node_Max_Index  = Node_Max_Index;
  retVal->ARC_Max_Index   = ARC_Max_Index;
  return retVal;
}
/*
 * add a node to set of nodes in DFG
 * if index of this node is greater than existing highest ID, increase max ID
 */
void DFG::insert_Node(Node *pNode)
{
  if (pNode->get_ID() > Node_Max_Index)
    Node_Max_Index = pNode->get_ID();

  _node_Set.push_back(pNode);
}

/*
 * add a constant to set of constants in DFG
 * if index of this node is greater than existing highest ID, increase max ID
 */
void DFG::insert_Constant(Node *pNode)
{
  if (pNode->get_ID() > Node_Max_Index)
    Node_Max_Index = pNode->get_ID();

  _constants.push_back(pNode);
}

/*
 *  for arc between pNode -> nNode   =>     pNode -> newNode -> nNode
 *  properties of arc pNode -> nNode is copied to pNode -> newNode
 */
bool DFG::insert_Node_in_between_input(Node* pNode, Node* nNode, Node* newNode)
{
  ARC *arc;
  vector<Node*> nextNodes;
  vector<ARC*>::iterator iEdge;
  FATAL(pNode->is_Mem_Operation() && nNode==pNode->get_Related_Node(), "Cannot insert a node in between a memory operation and its related node!");

  for (iEdge = _ARC_Set.begin(); iEdge != _ARC_Set.end(); iEdge++)
  {
    arc = *iEdge;
    //find the arc between pNode -> nNode
    if (arc->get_From_Node()->get_ID() == pNode->get_ID() && arc->get_To_Node()->get_ID() == nNode->get_ID())
    {
      _ARC_Set.erase(iEdge, iEdge + 1);
      //remove it from pNode arcs
      if(pNode->remove_Succ_Arc(arc) == -1) debugfile << "Failed to remove Succ arc for node\n.";
      //remove it from nNode arcs
      if(nNode->remove_Pred_Arc(arc) == -1) debugfile << "Failed to remove Pred arc for node\n.";
      //form an arc between pNode and newNode
      make_Arc(pNode, newNode, ++ARC_Max_Index, 0, arc->get_Dependency(),0);
      //form an arc between newNode and nNode
      make_Arc(newNode, nNode, ++ARC_Max_Index, arc->get_Distance(), TrueDep,arc->get_OperandOrder());
      return true;
    }
  }

  if ((nNode->get_ID() == pNode->get_ID()) && (nNode->has_self_loop())) {
      nNode->delete_self_loop();
      pNode->delete_self_loop();
      return true;
  }

  FATAL(true, "in input. Could not find arc between Node %d and %d to delete", pNode->get_ID(), nNode->get_ID());
  return false;
}

/*
 *  for arc between pNode -> nNode   =>     pNode -> newNode -> nNode
 *  properties of arc pNode -> nNode is copied to newNode -> nNode
 */
bool DFG::insert_Node_in_between_output(Node* pNode, Node* nNode, Node* newNode)
{
  ARC *arc;
  vector<Node*> nextNodes;
  vector<ARC*>::iterator iEdge;

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
      make_Arc(pNode, newNode, ++ARC_Max_Index, 0, TrueDep,0);
      //form an arc between newNode and nNode
      make_Arc(newNode, nNode, ++ARC_Max_Index, arc->get_Distance(), arc->get_Dependency(),arc->get_OperandOrder());
      //return success
      return true;
    }
  }

  if ((nNode->get_ID() == pNode->get_ID()) && (nNode->has_self_loop())) {
      nNode->delete_self_loop();
      pNode->delete_self_loop();
      return true;
  }

  FATAL(true, "in output. Could not find arc between Node %d and %d to delete", pNode->get_ID(), nNode->get_ID());
  return false;
}

// returns true if DFG has a node with the given ID number
bool DFG::has_Node(int number)
{
  vector<Node*>::iterator iNode1;
  for (iNode1 = _node_Set.begin(); iNode1 != _node_Set.end(); iNode1++)
  {
    if ((*iNode1)->get_ID() == number)
      return true;
  }
  return false;
}

//returns true if DFG has a node with the given ID number
bool DFG::has_Constant(int number)
{
  vector<Node*>::iterator iNode1;
  for (iNode1 = _constants.begin(); iNode1 != _constants.end(); iNode1++)
  {
    if ((*iNode1)->get_ID() == number)
      return true;
  }
  return false;
}

// return a node with given ID number
Node* DFG::get_Node(int number)
{
  vector<Node*>::iterator iNode1;
  for (iNode1 = _node_Set.begin(); iNode1 != _node_Set.end(); iNode1++)
  {
    if ((*iNode1)->get_ID() == number)
      return *iNode1;
  }
  FATAL(true, "Node with ID=%d is not found", number);
  printf("Node with ID=%d is not found", number);
  return NULL;
}

// make an arc between two nodes
void DFG::make_Arc(Node* pNin, Node* pNout, int ID, int Distance, DataDepType dep, int oporder)
{
  //if they are already connected, ignore it
  if (pNin->is_Connected_To(pNout))
    return;

  //source and destination are the same and the node has already a self loop, ignore it
  if (pNin->get_ID() == pNout->get_ID() && pNin->has_self_loop())
    return;

  ARC *pArc;
  //create an arc with given properties
  pArc = new ARC(ID, Distance, dep, oporder);

  //set source and destinations of the arc
  pArc->Set_From_Node(pNin);
  pArc->Set_To_Node(pNout);

  //if source and destination are the same, create a self loop
  if (pNin->get_ID() == pNout->get_ID())
    pNin->set_self_loop(pArc);
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
    ARC_Max_Index = ID;
}

void DFG::make_Const_Arc(int pNin, int pNout, int oporder)
{
  std::tuple<int,int,int> newArc = std::make_tuple(pNin, pNout, oporder);
  _Const_ARC_Set.push_back(newArc);
}

// returns true if given set contains given node
bool DFG::contains(vector<Node*> &nodeset, Node* node)
{
  vector<Node*>::iterator iNode1;
  for (iNode1 = nodeset.begin(); iNode1 != nodeset.end(); iNode1++)
  {
    //found the node with the same ID
    if ((*iNode1)->get_ID() == node->get_ID())
      return true;
  }
  return false;
}

// remove node from nodeset
void DFG::Remove(vector<Node*> &nodeset, Node* node)
{
  vector<Node*>::iterator iNode;
  for (iNode = nodeset.begin(); iNode != nodeset.end(); iNode++)
  {
    if ((*iNode)->get_ID() == node->get_ID())
    {
      nodeset.erase(iNode, iNode + 1);
      return;
    }
  }
}

// remove all nodes in toRemmoveNodes from nodeset
void DFG::Remove(vector<Node*> &nodeset, vector<Node*> &toRemoveNodes)
{
  vector<Node*>::iterator iNode;
  for (iNode = toRemoveNodes.begin(); iNode != toRemoveNodes.end(); iNode++)
    Remove(nodeset, *iNode);
}

// delete pNode from _nodeset
int DFG::delete_Node(Node *pNode)
{
  vector<Node*>::iterator iNode;

  for (iNode = _node_Set.begin(); iNode != _node_Set.end(); iNode++)
  {
    if ((*iNode)->get_ID() == pNode->get_ID())
    {
      _node_Set.erase(iNode, iNode + 1);
      return 0;
    }
  }
  return -1;
}

// remove arc from from _ARC_Set
void DFG::Remove_Arc(ARC* arc)
{
  vector<ARC*>::iterator iEdge;
  for (iEdge = _ARC_Set.begin(); iEdge != _ARC_Set.end(); iEdge++)
  {
    if ( ((*iEdge)->get_From_Node()->get_ID() == arc->get_From_Node()->get_ID()) && ((*iEdge)->get_To_Node()->get_ID() == arc->get_To_Node()->get_ID()) )
    {
      _ARC_Set.erase(iEdge, iEdge + 1);
      (*iEdge)->get_From_Node()->remove_Succ_Arc(arc);
      (*iEdge)->get_To_Node()->remove_Pred_Arc(arc);
      return;
    }
  }
  FATAL(true, "Could not find arc %d to delete", arc->get_Number());
}

// remove arc pNode -> nNode
void DFG::Remove_Arc(Node *pNode, Node *nNode)
{
  ARC *edge;
  vector<ARC*>::iterator iEdge;
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
  FATAL(true, "in remove arc. Could not find arc between node %d and %d", pNode->get_ID(), nNode->get_ID());
}

// add an arc to _ARC_Set
void DFG::Add_Arc(ARC* arc)
{
  _ARC_Set.push_back(arc);
}

// Get the arc pNode -> nNode
ARC* DFG::get_Arc(Node *pNode, Node *nNode)
{
  ARC *edge;
  vector<ARC*>::iterator iEdge;
  for (iEdge = _ARC_Set.begin(); iEdge != _ARC_Set.end(); iEdge++)
  {
    edge = *iEdge;
    if (edge->get_From_Node()->get_ID() == pNode->get_ID() && edge->get_To_Node()->get_ID() == nNode->get_ID())
      return edge;
  }
  return NULL;
}

// returns the intersect of nodeset1 and nodeset2
vector<Node*> DFG::Intersect(vector<Node*> &nodeset1, vector<Node*> &nodeset2)
{
  vector<Node*> retVal;
  vector<Node*>::iterator iNode1;
  for (iNode1 = nodeset2.begin(); iNode1 != nodeset2.end(); iNode1++)
  {
    //if nodeset1 contains iNode1 and it is not already in the intersection
    if (contains(nodeset1, *iNode1) && !contains(retVal, *iNode1))
      retVal.push_back(*iNode1);
  }
  return retVal;
}

// returns the union of nodesset1 and nodeset2
vector<Node*> DFG::Union(vector<Node*> &nodeset1, vector<Node*> &nodeset2)
{
  vector<Node*> retVal;
  vector<Node*>::iterator iNode1;
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

// returns set1 - set2
vector<Node*> DFG::Subtract(vector<Node*> &set1, vector<Node*> &set2)
{
  vector<Node*> retVal;
  vector<Node*>::iterator iNode1;
  for (iNode1 = set1.begin(); iNode1 != set1.end(); iNode1++)
  {
    //if iNode1 is not in set2, add it
    if (!contains(set2, (*iNode1)))
      retVal.push_back(*iNode1);
  }
  return retVal;
}

// trojan algorithm to find strongly connected nodes in a DFG
void DFG::Strongly_Connected(vector<vector<Node*> > &results)
{
  stack<Node*> S;
  vector<Node*> SCopy;
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
void DFG::Strongly_Connected(Node* currentNode, stack<Node*> &S, vector<Node*> &SCopy, int &index, vector<vector<Node*> > &results)
{
  vector<Node*> local;
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
  vector<Node*> nextNodes = currentNode->Get_Next_Nodes();
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
      Node* temp = S.top();
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

// generate a unique ID for nodes
int DFG::Get_Unique_Index()
{
  return ++Node_Max_Index;
}

// return _node_set
vector<Node*> DFG::getSetOfVertices()
{
  return _node_Set;
}

// return _ARC_Set
vector<ARC*> DFG::getSetOfArcs()
{
  return _ARC_Set;
}

void DFG::Dump_Nodes(std::string filename)
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

void DFG::Dump_Edges(std::string filename)
{
  std::string depname;
  std::ofstream edgeFile;
  std::string graphname_edge = filename;
  graphname_edge.append("_edge.txt");
  edgeFile.open(graphname_edge.c_str());

  for (int i = 0; i < (int) _ARC_Set.size(); i++)
  {
    if(_ARC_Set[i]->get_Dependency() == TrueDep)
      depname = "TRU";
    else if(_ARC_Set[i]->get_Dependency() == LoadDep)
      depname = "LRE";
    else if(_ARC_Set[i]->get_Dependency() == StoreDep)
      depname = "SRE";
    else if(_ARC_Set[i]->get_Dependency() == MemoryDep)
      depname = "MEM";
    else if(_ARC_Set[i]->get_Dependency() == PredDep)
      depname = "PRE";

    if(_ARC_Set[i]->get_Dependency()!=MemoryDep)
    {
      edgeFile << _ARC_Set[i]->get_From_Node()->get_ID() << "\t" << _ARC_Set[i]->get_To_Node()->get_ID() << "\t" << _ARC_Set[i]->get_Distance() << "\t" << depname << "\t" << _ARC_Set[i]->get_OperandOrder() << "\n";
    }
  }
  edgeFile.close();
}

// Print DFG in DOT format
void DFG::Dot_Print_DFG(string filename, int ID)
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
      Node_Dummy *tempDummyNode = dynamic_cast<Node_Dummy*>(_node_Set[i]);
      if (tempDummyNode != NULL)         //routing nodes are green
      {
        dotFile << "\n" << _node_Set[i]->get_ID() << " [color=green ];\n";
      }
      else          //regular nodes are red
      {
        dotFile << "\n" << _node_Set[i]->get_ID() << " [color=red ];\n";
      }
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

// return the size of _node_Set
int DFG::get_number_of_nodes()
{
  return (int) _node_Set.size();
}

// returns the number of load operations
int DFG::Count_Load_Operations()
{
  int retval = 0;
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    if ( (_node_Set[i]->is_Mem_Operation() && _node_Set[i]->is_Load_Data_Bus_Read()) || (_node_Set[i]->is_Mem_Operation() && _node_Set[i]->is_Load_Address_Generator()) )
      retval++;
  }
  return retval;
}

// returns the number of store operations
int DFG::Count_Store_Operations()
{
  int retval = 0;
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    if (_node_Set[i]->is_Mem_Operation() && _node_Set[i]->is_Store_Data_Bus_Write())
      retval++;
  }
  return retval;
}

// return the set of children of set nodes, all from the same iteration as nodes
vector<Node*> DFG::get_next_nodes_same_iteration(vector<Node*> &nodes)
{
  vector<Node*> retval;
  vector<Node*> current_next;
  for (int i = 0; i < (int) nodes.size(); i++)
  {
    current_next = nodes[i]->Get_Next_Nodes_Same_Iteration();
    retval = Union(current_next, retval);
  }
  return retval;
}

// return the set of children of set nodes, consider latency of operations
vector<Node*> DFG::get_next_nodes_same_iteration_update_latency(vector<Node*> &nodes, bool &change)
{
  vector<Node*> retval;
  vector<Node*> current_next;
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

// reset cycles of the nodes
void DFG::reset_latency_to_nodes(vector<Node*> &nodes)
{
  for (int i = 0; i < (int) nodes.size(); i++)
    nodes[i]->Reset_Cycle_Index();
}

// returns set of nodes with no parent from current cycle
vector<Node*> DFG::get_set_of_start_nodes()
{
  vector<Node*> retval;
  vector<Node*> prev_nodes;
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

// find a set of nodes without parents in same iteration
vector<Node*> DFG::get_set_of_start_nodes_constraint_scheduling()
{
  vector<Node*> stage_1;
  vector<Node*> prev_nodes;

  //find set of nodes without parent in current iteration
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    prev_nodes = _node_Set[i]->Get_Prev_Nodes();
    if ((int) prev_nodes.size() == 0)
    {
      //ignore load address and store address nodes due to
      //memory operation on far scheduled dependent nodes
      bool load_address = false;
      if(_node_Set[i]->is_Mem_Operation())
      {
        for (std::map<Node*, vector<Node*> >::iterator it= constrained_store_load_pairs.begin(); it!=constrained_store_load_pairs.end(); ++it)
        {
          if(it->first->get_Related_Node()->get_ID() == _node_Set[i]->get_ID()) {
            load_address = true;
            break;
          }

          vector<Node*> load_nodes = it->second;
          for(unsigned int n=0; n < load_nodes.size(); n++) {
            if(load_nodes[n]->get_ID() == _node_Set[i]->get_ID()) {
              load_address = true;
              break;
            }
          }
        }
      }
      if(!load_address)
        stage_1.push_back(_node_Set[i]);
    }
    else //Make sure all input edges are having recurrency
    {
      int edge_dist = 0;
      int push = 1;
      for(unsigned int j = 0; j < prev_nodes.size(); j++) {
	       edge_dist = get_Arc(prev_nodes[j],_node_Set[i])->get_Distance();
      	 if(edge_dist == 0)
         {
           push = 0;
           break;
         }
      }
      if(push == 1)
        stage_1.push_back(_node_Set[i]);
    }
  }
  return stage_1;
}

// find a set of nodes without any child at current iteration
vector<Node*> DFG::get_set_of_end_nodes_constraint_scheduling()
{
  vector<Node*> stage_1;
  vector<Node*> next_nodes;
  //find set of nodes with no child in the same iteration
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    next_nodes = _node_Set[i]->Get_Next_Nodes_Same_Iteration();
    if ((int) next_nodes.size() == 0)
    {
      //ignore store nodes due to memory operation on far scheduled dependent nodes
      bool store_data = false;
      if(_node_Set[i]->is_Mem_Operation())
      {
        for (std::map<Node*, vector<Node*> >::iterator it=constrained_store_load_pairs.begin(); it!=constrained_store_load_pairs.end(); ++it)
        {
          if((it->first->get_ID() == _node_Set[i]->get_ID()) || (it->first->get_Related_Node()->get_ID() == _node_Set[i]->get_ID()))
          {
            store_data = true;
            break;
          }

        }
      }
      if(!store_data)
        stage_1.push_back(_node_Set[i]);
    }
  }
  return stage_1;
}

// return a set of nodes without any child in current iteration
vector<Node*> DFG::get_set_of_end_nodes()
{
  vector<Node*> retval;
  vector<Node*> next_nodes;
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    next_nodes = _node_Set[i]->Get_Next_Nodes_Same_Iteration();
    if ((int) next_nodes.size() == 0)
    {
      //ignore store nodes due to memory operation on far scheduled dependent nodes
      bool store_data = false;
      if(_node_Set[i]->is_Mem_Operation())
      {
        for (std::map<Node*, vector<Node*> >::iterator it=constrained_store_load_pairs.begin(); it!=constrained_store_load_pairs.end(); ++it)
        {
          if((it->first->get_ID() == _node_Set[i]->get_ID()) || (it->first->get_Related_Node()->get_ID() == _node_Set[i]->get_ID()))
          {
            store_data = true;
            break;
          }
        }
      }
      if(!store_data)
        retval.push_back(_node_Set[i]);
    }
  }
  return retval;
}

// limits the number of inputs to an operation to 2
void DFG::Apply_Indegree_Constraint()
{
  //current node to check contraint
  Node* startNode;
  //for all nodes
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    startNode = _node_Set[i];
    //ignore phi operations
    if (startNode->is_PHI_Operations())
      continue;
    //if number of operands is > architectural specification
    FATAL(startNode->get_Number_of_Operands() > cgra_info.MAX_NODE_INDEGREE, "Should never reach here, the number of operands should always respect architecture scpecification");
  }
}

// limits the number of outputs of an operation to architectural specifications
void DFG::Apply_Outdegree_Constraint()
{
  Node_Dummy* newnode;
  Node* startNode;
  vector<Node*> nextnodes;
  vector<Node*> next_true_dep_nodes;
  vector<Node*> next_predicate_dep_nodes;
  vector<Node*>::iterator iNode1;
  int count;
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    startNode = _node_Set[i];
    next_true_dep_nodes = startNode->Get_True_Dependency_Successors_Same_Iteration();
    next_predicate_dep_nodes = startNode->Get_Predicate_Dependency_Successors_Same_Iteration();

    // if outdegree > X (given by arch specifications
    if ((int)next_true_dep_nodes.size() > cgra_info.MAX_NODE_OUTDEGREE)
    {
      nextnodes = next_true_dep_nodes;
      // add a routing node
      newnode = new Node_Dummy(1, Get_Unique_Index(), startNode);
      if ((int)newnode->get_Number_of_Pred()>0)
        Add_Arc(newnode->get_Pred_Arc(0));

      // if this node is predicated, so should be the routing node
      if (startNode->isPredicated())
      {
        //we need to start from the first index because we increased outdegree of another node
        i = 0;
      }
      // add it to DFG
      //convert startnode -> nextnodes[0] to startnode -> newnode -> nextnodes[0]
      for (iNode1 = nextnodes.begin(); iNode1 != nextnodes.end(); iNode1++)
      {
        insert_Node_in_between_output(startNode, *iNode1, newnode);
        if(!has_Node(newnode->get_ID()))
          insert_Node(newnode);
        break;
      }
      nextnodes = startNode->Get_True_Dependency_Successors_Same_Iteration();
      count = 0;

      //convert startnode -> nextnodes[i] to newnode -> nextnodes[i]
      for (iNode1 = nextnodes.begin(); iNode1 != nextnodes.end(); iNode1++)
      {
        if ((*iNode1)->get_ID() == newnode->get_ID())
          continue;
        ARC *arc = get_Arc(startNode, *iNode1);
        int distance = arc->get_Distance();
        int oporder = arc->get_OperandOrder();
        Remove_Arc(startNode, *iNode1);
        make_Arc(newnode, *iNode1, ++ARC_Max_Index, distance, arc->get_Dependency(), oporder);
        count++;
        //outdegree problem resolved?
        if (count == (int) nextnodes.size() - cgra_info.MAX_NODE_OUTDEGREE)
          break;
      }
    }

    // if outdegree > X (given by arch specifications
    if ((int)next_predicate_dep_nodes.size() > cgra_info.MAX_NODE_OUTDEGREE)
    {
      nextnodes = next_predicate_dep_nodes;
      // add a routing node
      newnode = new Node_Dummy(1, Get_Unique_Index(), startNode);
      if ((int)newnode->get_Number_of_Pred()>0)
        Add_Arc(newnode->get_Pred_Arc(0));

      // if this node is predicated, so should be the routing node
      if (startNode->isPredicated())
      {
        //we need to start from the first index because we increased outdegree of another node
        i = 0;
      }
      // add it to DFG
      //convert startnode -> nextnodes[0] to startnode -> newnode -> nextnodes[0]
      for (iNode1 = nextnodes.begin(); iNode1 != nextnodes.end(); iNode1++)
      {
        insert_Node_in_between_output(startNode, *iNode1, newnode);
        if(!has_Node(newnode->get_ID()))
          insert_Node(newnode);
        break;
      }
      nextnodes = startNode->Get_Predicate_Dependency_Successors_Same_Iteration();
      count = 0;

      //convert startnode -> nextnodes[i] to newnode -> nextnodes[i]
      for (iNode1 = nextnodes.begin(); iNode1 != nextnodes.end(); iNode1++)
      {
        if ((*iNode1)->get_ID() == newnode->get_ID())
          continue;
        ARC *arc = get_Arc(startNode, *iNode1);
        int distance = arc->get_Distance();
        Remove_Arc(startNode, *iNode1);
        make_Arc(newnode, *iNode1, ++ARC_Max_Index, distance, arc->get_Dependency(),0);
        count++;
        //outdegree problem resolved?
        if (count == (int) nextnodes.size() - cgra_info.MAX_NODE_OUTDEGREE)
          break;
      }
    }
  }
}

/*
 * this function tries to minimize register requirement for inter iteration data dependencies
 * for a node with multiple children in next iterations, find a child with minimal distance
 * make a routing node, and forward the dependencies of other children to this node
 */
void DFG::Apply_Outdegree_Inter_Iteration_Constraint()
{
  Node_Dummy* newnode;
  Node* startNode;
  Node* selected_node=NULL;
  vector<Node*> nextnodes;
  vector<Node*>::iterator iNode1;
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
      newnode = new Node_Dummy(1, Get_Unique_Index(), startNode);
      insert_Node(newnode);

      if ((int)newnode->get_Number_of_Pred()>0)
        Add_Arc(newnode->get_Pred_Arc(0));

      //insert it between current node and its child that has the minimum inter iteration distance
      insert_Node_in_between_input(startNode, selected_node, newnode);

      //forward the other dependecies to the new node
      nextnodes = startNode->Get_Next_Nodes_Next_Iteration();
      for (iNode1 = nextnodes.begin(); iNode1 != nextnodes.end(); iNode1++)
      {
        if ((*iNode1)->get_ID() == newnode->get_ID())
          continue;
        ARC *arc = get_Arc(startNode, *iNode1);
        int distance = arc->get_Distance();
        int operandorder = arc->get_OperandOrder();
        Remove_Arc(startNode, *iNode1);
        make_Arc(newnode, *iNode1, ++ARC_Max_Index, distance - min_distance, arc->get_Dependency(),operandorder);
      }
    }
  }
}

// Preprocess the DFG
void DFG::PreProcess()
{
   /* limit the number of incoming arcs to nodes to 2
   * for predicated operations though, it can be 3
   */
  Apply_Indegree_Constraint();
   // Limit the number of outgoing arcs to node to X,
   // X should come from architecture description
  Apply_Outdegree_Constraint();
  // Minimize register requirements for inter iteration dependencies
  Apply_Outdegree_Inter_Iteration_Constraint();
}

// performs some preliminary scheduling in DFG and returns MII
int DFG::Init_Schedule(int id, int number_of_resources, int rec_MII)
{
  vector < vector<Node*> > Sccs;
  // find strongly connected sets
  Strongly_Connected (Sccs);
  DEBUG_SCHEDULING("Size of SCCs is %d", (int ) Sccs.size());

  int recMII_index;
  // find recMII
  int recMII = RecMII_Scc(Sccs, recMII_index);
  DEBUG_SCHEDULING("recMII is %d", recMII);

  // find resMII
  int resMII = get_ResMII(number_of_resources);
  DEBUG_SCHEDULING("resMII without mem operations is %d", resMII);

  // find memory resource MII
  int MemII = get_MemMII();
  if (MemII > resMII)
    resMII = MemII;
  DEBUG_SCHEDULING("resMII without mem operations is %d", resMII);

  int MII = recMII;
  if (resMII > MII)
    MII = resMII;

  // recMII is calculated through separate file and integrated here
  if(rec_MII > MII)
    MII = rec_MII;

  // ASAP Schedule
  int last_time = Schedule_ASAP();
  vector<Node*> set = _node_Set;
  Schedule_ALAP(last_time);

  last_time = Schedule_ASAP_Feasible(number_of_resources, MII);
  MAX_SCHEDULE_LEN = last_time;
  Schedule_ALAP_Feasible(last_time, number_of_resources, MII);

  return MII;
}

/*
 * schedule the DFG
 * id is the schedule attempt ID
 * number of resources is the maximum number of operations at permitted any cycles
 * II is initiation inetrval
 */
int DFG::Schedule(int id, int number_of_resources, int II)
{
  // find current resMII relative to available resources
  int resMII = (_node_Set.size() - 1) / number_of_resources;
  resMII++;

  if (resMII > II)
    return resMII;

  // ASAP Schedule

  int last_time = Schedule_ASAP();
  vector<Node*> set = _node_Set;
  Schedule_ALAP(last_time);

  last_time = Schedule_ASAP_Feasible(number_of_resources, II);
  MAX_SCHEDULE_LEN = last_time;
  if (!Schedule_ALAP_Feasible(last_time, number_of_resources, II))
  {
    return -1;
  }

  bool redo = false;
  while (true)
  {
    // find a feasible modulo schedule
    debugfile <<  _node_Set.size() << endl;
    bool modulo_schedule = Modulo_Schedule(II, last_time, redo, number_of_resources);
    debugfile <<  _node_Set.size() << endl;
    if (modulo_schedule)
    {
      schedule_len = last_time;
      debugfile << "Feasible II is " << II << "\tSchedule Len is " << schedule_len << "\n";
      DEBUG_SCHEDULING("Feasible II is %d", II);
      return II;
    }

    // not valid schedule is found, reset current schedule
    Reset_Current_Schedule();
    if (redo)
    {
      debugfile << "Redoing ASAP and ALAP\n";
      // reset all schedules and start from feasible scheduling ASAP and ALAP
      Reset_ALAP_Feasible_Schedule();
      Reset_ASAP_Feasible_Schedule();

      last_time = Schedule_ASAP_Feasible(number_of_resources,II);
      MAX_SCHEDULE_LEN = last_time;

      if (!Schedule_ALAP_Feasible(last_time, number_of_resources, II))
      {
        return -1;
      }

      DEBUG_SCHEDULING("Multiple Constraint %d\n", II);
    }
    // no valid schedule is found
    else
    {
      return -1;
    }
  }

  FATAL(true, "How did we get here?");
  return -1;
}

//this function tries to map the unmapped nodes, this may modify the DFG and
//attempt a scheduling
int DFG::Modulo_ReSchedule_Smart_2(int id, int number_of_resources, int II, vector<Node*> &to_change_set)
{
  bool redo = false;
  int success = 0;
  int last_time;
  vector<Node*> reschedule;
  vector<int> resched_time;

  debugfile << "Left Nodes Are: ";
  for (int i = 0; i < (int) to_change_set.size(); i++)
	debugfile << to_change_set[i]->get_ID() << "\t";
  debugfile << "\n";

  for (int i = 0; i < (int) to_change_set.size(); i++)
  {
    if (to_change_set[i]->get_physical_predecessors_constrain_this_modulo(II).size() > 0)
    {
      vector<Node*> prev = to_change_set[i]->get_physical_predecessors_constrain_this_modulo(II);
      for (int j = 0; j < (int) prev.size(); j++)
      {
        Node_Dummy* tempDummyNode = new Node_Dummy(1, Get_Unique_Index(), prev[j]);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }

        insert_Node(tempDummyNode);
        insert_Node_in_between_input(prev[j], to_change_set[i], tempDummyNode);
      }
      success = 1;
      continue;
    }

    if (to_change_set[i]->get_physical_successors_constrain_by_this_modulo(II).size() > 0)
    {
      vector<Node*> next = to_change_set[i]->get_physical_successors_constrain_by_this_modulo(II);
      for (int j = 0; j < (int) next.size(); j++)
      {
        Node_Dummy* tempDummyNode = new Node_Dummy(1, Get_Unique_Index(), to_change_set[i]);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }
        insert_Node(tempDummyNode);
        insert_Node_in_between_input(to_change_set[i], next[j], tempDummyNode);
      }
      success = 1;
      continue;
    }

    if ((to_change_set[i]->get_Sched_Info()->get_Feasible_ALAP() - to_change_set[i]->get_Sched_Info()->get_Feasible_ASAP()) > 0)
    {
      debugfile << "resched back" << to_change_set[i]->get_ID() << endl;
      reschedule.push_back(to_change_set[i]);
      resched_time.push_back(to_change_set[i]->get_Sched_Info()->get_Feasible_ALAP() - 1);
      success = 1;
    }
  }
  DEBUG_RESCHEDULE("Rescheduling completed with %d",success);
  vector<Node*> set = _node_Set;

  if (success == 1)
  {
    Reset_Current_Schedule();
    Reset_ALAP_Feasible_Schedule();
    Reset_ASAP_Feasible_Schedule();
    last_time = Schedule_ASAP_Feasible(number_of_resources, II);

    if (!Schedule_ALAP_Feasible(last_time, number_of_resources, II))
    {
      return 0;
    }

    MAX_SCHEDULE_LEN = last_time;
    int alap_time;
    while (true)
    {
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

      if (Modulo_Schedule(II, last_time, redo, number_of_resources))
      {
        DEBUG_RESCHEDULE("Rescheduling succeed");
        if (DEBUG_4)
        {
          printf("Feasible II is %d\n", II);
        }
        return 1;
      }
      if (redo)
      {
        DEBUG_RESCHEDULE("Rescheduling failed");
        Reset_ALAP_Feasible_Schedule();
        Reset_ASAP_Feasible_Schedule();

        last_time = Schedule_ASAP_Feasible(number_of_resources, II);

        if (!Schedule_ALAP_Feasible(last_time, number_of_resources, II))
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

// returns recMII
int DFG::RecMII_Scc(vector<vector<Node*> > &sets, int &max_index)
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

// find longest path for SCCs
int DFG::Longest_Path(vector<Node*> &nodes, int &II)
{
  Node* start_node;
  II = 1;
  vector<Node*> visited;
  vector<Node*> next_nodes;
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
      continue;

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

// Schedule operations as soon as all predecessors are completed
int DFG::Schedule_ASAP()
{

  DEBUG_SCHEDULING("ASAP Started");
  //set of scheduled operations
  vector<Node*> scheduled;
  //set of next nodes to schedule
  vector<Node*> to_be_scheduled;
  //set of nodes not scheduled yet
  vector<Node*> rest = _node_Set;
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
          latest_time = schedule_time;
      }
    }
    //update set of unscheduled operations
    rest = Subtract(rest, scheduled);
  }
  DEBUG_SCHEDULING("ASAP Finished");
  //return schedule length
  return latest_time;
}

// schedule operations in ASAP manner considering number of available resources
int DFG::Schedule_ASAP_Feasible(int number_of_resources, int II)
{
  Total_Resources = new int[MAPPING_POLICY.MAX_LATENCY];      //total number of PEs at each cycle
  Address_BUS     = new int[MAPPING_POLICY.MAX_LATENCY];      //total number of memory bus at each cycle
  Data_BUS        = new int[MAPPING_POLICY.MAX_LATENCY];

  //initialize used resources to 0
  for (int i = 0; i < MAPPING_POLICY.MAX_LATENCY; i++)
  {
    Total_Resources[i]  = 0;
    Address_BUS[i]      = 0;
    Data_BUS[i]         = 0;
  }
  DEBUG_SCHEDULING("Feasbile ASAP Started");
  //set of scheduled operations
  vector<Node*> scheduled;
  //set of operations to be scheduled
  vector<Node*> to_be_scheduled;

  vector<Node*> temp;
  vector<Node*> temp_2;
  //set of operations not scheduled yet
  vector<Node*> rest_old = _node_Set;
  vector<Node*> not_scheduled;
  vector<Node*> phi_rescheduled;
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

  DEBUG_FEASIBLE_ASAP_SCHEDULING("Hi. FEASIBLE Init nodes Start");
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
        continue;
        //any conflict with a node connected with arc with distance > 1
        if (has_Node_Conflict_Feasible_ASAP(to_be_scheduled[i], j))
        continue;
        //any node conflict between load data read and connected nodes to it with arc distance > 1
        if (has_Node_Conflict_Feasible_ASAP(to_be_scheduled[i]->get_Related_Node(), j+1))
        continue;
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
      not_scheduled.push_back(to_be_scheduled[i]);
      not_scheduled.push_back(to_be_scheduled[i]->get_Related_Node());
      continue;
    }
    //regular node to be scheduled
    else
    {
      if (to_be_scheduled[i]->is_PHI_Operations())
      {
        //Phi Nodes Can Not Be There At Very Beginning, It should be near the successor
        phi_rescheduled.push_back(to_be_scheduled[i]);
      }

      //find a time slot
      for (int j = 0; j < MAPPING_POLICY.MAX_LATENCY; j++)
      {
        //enough PE available?
        if (Total_Resources[j] >= number_of_resources)
          continue;

        //has conflict with a node connected with an arc with distance > 1
        if (has_Node_Conflict_Feasible_ASAP(to_be_scheduled[i], j))
          continue;

        //allocate resource
        Total_Resources[j]++;

        //schedule the node
        to_be_scheduled[i]->get_Sched_Info()->set_Feasible_ASAP(j);
        DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is scheduled at time %d", to_be_scheduled[i]->get_ID(), j);
        //successfully schedule the operations
        break;
      }
    }
  }
  DEBUG_FEASIBLE_ASAP_SCHEDULING("FEASIBLE Init nodes Finished");

  /*
    Update Schedule Time of Phi Nodes.
    Ensure that ASAP time of PHI is immediately above to all successors
    */
  for(unsigned int ii=0; ii < phi_rescheduled.size(); ii++)
    phi_rescheduled[ii]->Update_ASAP_for_Phi(II);

  //update set of scheduled nodes
  scheduled = Union(temp, to_be_scheduled);
  //update set of unscheduled nodes
  rest_old = Subtract(rest_old, scheduled);
  rest_old = Union(rest_old, not_scheduled);

  //Sort The Nodes Such That Store Nodes Get Priority In List Before Loads Or Any Other. Than Loads And Then Regular (For Memory Operations)
  vector<Node*> rest = sort_rest_asap_nodes_for_memory(rest_old);

  bool detect=false;
  //while there is an unscheduled node
  while ((int) rest.size() > 0)
  {
    //for all nodes
    for (int i = 0; i < (int) rest.size(); i++)
    {
      //any new node schedule? not yet
      change = false;
      //is this node ready to be scheduled? if yes, earliest time?
      if (rest[i]->is_ready_to_schedule_Feasible_ASAP(schedule_time, II))
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
              if (rest[i]->get_Related_Node()->is_ready_to_schedule_Feasible_ASAP(schedule_time_2, II))
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
                  continue;

                //does load data read has any conflict with scheduled nodes
                if (has_Node_Conflict_Feasible_ASAP(rest[i]->get_Related_Node(), time + 1))
                  continue;

                int node2_asap_reverse = -1000;
                for (std::map<Node*, vector<Node*> >::iterator it=constrained_store_load_pairs.begin(); it!=constrained_store_load_pairs.end(); ++it)
                {
                  vector<Node*> load_address_nodes = it->second;
                  for(unsigned int n = 0; n < load_address_nodes.size(); n++) {
                    if(load_address_nodes[n]->get_ID() == rest[i]->get_ID()) //We need to Update ASAP of Load Address Node (node2).
                    {
                      //Check That ASAP of Load Address is In Accordance With ASAP of Successor Node.
                      Node* succ = constrained_ld_succ_pairs[load_address_nodes[n]];
                      ARC* edge = get_Arc(rest[i]->get_Related_Node(),succ);
                      int succ_time = 0;
                      if(edge->get_Distance() > 0)
                        succ_time = succ->get_Sched_Info()->get_Feasible_ASAP() + edge->get_Distance()*II;
                      else
                        succ_time = succ->get_Sched_Info()->get_Feasible_ASAP();

                      node2_asap_reverse = succ_time - load_address_nodes[n]->get_Related_Node()->get_Latency() - rest[i]->get_Latency();
                      break;
                    }
                  }
                  if(node2_asap_reverse != -1000)
                    break;
                }

                if(node2_asap_reverse > time)
                  continue;

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
                    latest_time = time + 1;
                  break;
                }
              }
              else
                break;
            }
            //if node is load data read
            else if (rest[i]->is_Load_Data_Bus_Read())
            {
              //is the related node, (address generation) is ready to be scheduled? at what cycle?
              if (rest[i]->get_Related_Node()->is_ready_to_schedule_Feasible_ASAP(schedule_time_2, II))
              {
                //if its schedule is greater than time - 1 > change time and try again
                if (schedule_time_2 > (time - 1))
                {
                  time = schedule_time_2;
                  continue;
                }
                //is there any conflict with any scheduled node?
                if (has_Node_Conflict_Feasible_ASAP(rest[i], time))
                  continue;

                //is there any conflict for load address generation with scheduled operations?
                if (has_Node_Conflict_Feasible_ASAP(rest[i]->get_Related_Node(), time - 1))
                  continue;

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
                    latest_time = time;
                  break;
                }
              }
              else
                break;
            }
            //if node is store (data or address)
            else if (rest[i]->is_Store_Address_Generator() || rest[i]->is_Store_Data_Bus_Write())
            {
              //is the related node ready? when?
              DEBUG_FEASIBLE_ASAP_SCHEDULING("Check for Store Related to be Ready");
              if (rest[i]->get_Related_Node()->is_ready_to_schedule_Feasible_ASAP(schedule_time_2, II))
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
                    latest_time = time;
                  break;
                }
                else
                  DEBUG_FEASIBLE_ASAP_SCHEDULING("Does not have enough Resources Data_BUS %d, Address_BUS %d, PE %d",Address_BUS[time], Data_BUS[time], Total_Resources[time]);
              }
              else
                break;
            }
            //if node is a regular operation
            else
            {
              //is there any conflict with scheduled operations?
              if (has_Node_Conflict_Feasible_ASAP(rest[i], time))
                continue;

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
                latest_time = time;
              break;
            }
          }
        }
      }
      //update set of unscheduled nodes
      rest = Subtract(rest, scheduled);
      //if there was a change, start from first node in rest again
      if (change)
        break;
    }

    if (!change)
    {
      DEBUG_FEASIBLE_ASAP_SCHEDULING("NO CHANGE SIZE OF REST %d", (int)rest.size());
      for (int i = 0; i < (int) rest.size(); i++)
      {
        bool ready=rest[i]->is_ready_to_schedule_Feasible_ASAP(schedule_time, II);
        DEBUG_FEASIBLE_ASAP_SCHEDULING("Node %d is %d ready to schedule at time %d", rest[i]->get_ID(), ready, schedule_time);
        if (rest[i]->is_Mem_Operation())
        {
          ready = rest[i]->get_Related_Node()->is_ready_to_schedule_Feasible_ASAP(schedule_time_2, II);
          DEBUG_FEASIBLE_ASAP_SCHEDULING("Related Node %d is %d ready to schedule at time %d", rest[i]->get_Related_Node()->get_ID(), ready, schedule_time_2);
        }
      }
      if (detect)
        exit(1);
      detect=true;
    }
  }

  for(unsigned int ii=0; ii < _node_Set.size(); ii++)
    _node_Set[ii]->Update_ASAP_Feasible(Address_BUS,Data_BUS,Total_Resources,number_of_resources,MAX_MEM_RESOURCES);

  return 2 * latest_time;
}

// Schedule operations as late as all successors are luanched
void DFG::Schedule_ALAP(int latest_time)
{
  DEBUG_SCHEDULING("ALAP Started");
  //set of scheduled operations
  vector<Node*> scheduled;
  //set of operations selected to be scheduled
  vector<Node*> to_be_scheduled;
  //set of unscheduled operations
  vector<Node*> rest = _node_Set;
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

int DFG::get_total_resources(int t)
{
  return Total_Resources[t];
}

int DFG::get_address_bus_resources(int t)
{
  return Address_BUS[t];
}

int DFG::get_data_bus_resources(int t)
{
  return Data_BUS[t];
}

void DFG::set_total_resources(int t, int n)
{
  Total_Resources[t] = n;
}

void DFG::set_address_bus_resources(int t, int n)
{
  Address_BUS[t] = n;
}

void DFG::set_data_bus_resources(int t, int n)
{
  Data_BUS[t] = n;
}

// schedule operations in ALAP manner considering number of available resources
bool DFG::Schedule_ALAP_Feasible(int latesttime, int number_of_resources, int II)
{
  int latest_time = latesttime;
  Total_Resources = new int[MAPPING_POLICY.MAX_LATENCY];
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
  vector<Node*> scheduled;
  //set of operations to be scheduled
  vector<Node*> to_be_scheduled;
  vector<Node*> temp;
  vector<Node*> temp_2;
  vector<Node*> temp_3;
  //unscheduled operations
  vector<Node*> rest = _node_Set;
  // initially schedule operations with no successor at this iterations
  // for memory operation, load data read and store address are not present in this set
  to_be_scheduled = get_set_of_end_nodes_constraint_scheduling();

  int schedule_time = 0;
  int schedule_time_2 = 0;
  //keep track of any change in set of scheduled operations
  bool change;

  DEBUG_SCHEDULING("scheduling last nodes");
  //pick a node
  for (int i = 0; i < (int) to_be_scheduled.size(); i++)
  {
    latest_time = (to_be_scheduled[i]->get_Sched_Info()->get_Feasible_ASAP()) + II - 1;
    //if node is load bus read
    if (to_be_scheduled[i]->is_Load_Data_Bus_Read())
    {
      //find a time slot to schedule the node
      for (int j = latest_time; j > -1; j--)
      {
        //is there enough PE resource to schedule the node?
        if (Total_Resources[latest_time] >= number_of_resources)
          continue;
        //is there a conflict with scheduled operations
        if (has_Node_Conflict_Feasible_ALAP(to_be_scheduled[i], j))
          continue;
        //is there conflict for load address assert with scheduled operations
        if (has_Node_Conflict_Feasible_ALAP(to_be_scheduled[i]->get_Related_Node(), j - 1))
          continue;
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
          continue;
        //is there any conflict with scheduled operations
        if (has_Node_Conflict_Feasible_ALAP(to_be_scheduled[i], j))
          continue;
        //is there any conflict between store address assert and scheduled operations
        if (has_Node_Conflict_Feasible_ALAP(to_be_scheduled[i]->get_Related_Node(), j))
          continue;
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
          continue;
        //is there a conflict with scheduled operations
        if (has_Node_Conflict_Feasible_ALAP(to_be_scheduled[i], j))
          continue;
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
      return false;

    //sample the number of unscheduled operations
    prev_size = (int) rest.size();
    //for all nodes
    for (int i = 0; i < (int) rest.size(); i++)
    {
      //assume no change in schedules
      change = false;
      DEBUG_FEASIBLE_ALAP_SCHEDULING("check if Node %d is ready to be scheduled", rest[i]->get_ID());
      //is it ready to be scheduled? if yes, at what cycle?
      if (rest[i]->is_ready_to_schedule_Feasible_ALAP(schedule_time, MAX_SCHEDULE_LEN, II))
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
              if (rest[i]->get_Related_Node()->is_ready_to_schedule_Feasible_ALAP(schedule_time_2, MAX_SCHEDULE_LEN, II)) //II, latesttime
              {
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
                  rest[i]->get_Sched_Info()->set_Feasible_ASAP(time + 1 - II);
                  rest[i]->get_Related_Node()->get_Sched_Info()->set_Feasible_ASAP(time + 1 + 1 - II);
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
                break;
            }
            //if it is a load bus read
            else if (rest[i]->is_Load_Data_Bus_Read())
            {
              //is related load address assert ready? if yes, what cycle?
              if (rest[i]->get_Related_Node()->is_ready_to_schedule_Feasible_ALAP(schedule_time_2, MAX_SCHEDULE_LEN, II))
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
                //For Loads Check When The Successors Are Scheduled And Update ASAP and ALAP
                //Iterate Through Successors And Get Earliest Time

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
                  rest[i]->get_Sched_Info()->set_Feasible_ASAP(time+1-II);
                  rest[i]->get_Related_Node()->get_Sched_Info()->set_Feasible_ASAP(time - 1 + 1 - II);
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
                break;
            }
            //if it is a store, either address or data assert, operation
            else if (rest[i]->is_Store_Address_Generator() || rest[i]->is_Store_Data_Bus_Write())
            {
              //is related node is ready to be scheduled? if yes, when?
              if (rest[i]->get_Related_Node()->is_ready_to_schedule_Feasible_ALAP(schedule_time_2, MAX_SCHEDULE_LEN, II))
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
  return true;
}

/*
 * Modulo scheduling algorithm
 * (1) given II, (2) schedule length,
 * (3)multiple constraint, sets to true if a node is found constrained by multiple predecessor
 * (4) number of available resources at each cycle
 */
bool DFG::Modulo_Schedule(int II, int Last_Time, bool &multiple_constraint, int number_of_resources)
{
  multiple_constraint = false;  //initially no node is constrained by multiple predecessor
  int Total_Resources[II];	    //total available resources
  int Address_BUS[II];		      //memory resources
  int Data_BUS[II];
  int tempII = II;

  for (int i = 0; i < II; i++) {	  //initially no resource is used
    Total_Resources[i] = 0;
    Data_BUS[i] = 0;
    Address_BUS[i] = 0;
  }

  vector<Node*> scheduled;	  //set of scheduled nodes
  //priority list of nodes is constructed.
  vector<Node*> to_be_scheduled = Get_SortOps(tempII, number_of_resources);
  vector<Node*> ready_sorted;

  int current_size, schedule_time, current_schedule_time;
  int modulo_schedule_time;

  vector<Node*> temp_1;
  vector<Node*> temp_2;

  //if a node is found constrained by multiple predecessor, a routing node is inserted to remove one contrain
  Node_Dummy* tempDummyNode;

  //while there is a node to schedule
  while ((int) to_be_scheduled.size() > 0)
  {
    //how many node are to be scheduled, sample current size
    current_size = (int) to_be_scheduled.size();
    //select a node with the highest priority
    ready_sorted = get_sorted_ready_list_modulo_backward(to_be_scheduled);

    if (ready_sorted[0]->get_Sched_Info()->is_CURRENT_Initiated())	    	    //selected node should not be scheduled before!!
    {
      FATAL(true, "Selected node %d is already scheduled!", (int ) ready_sorted[0]->get_ID());
      continue;
    }

    //schedule windows from feasible ALAP to feasible ASAP. We schedule backward
    schedule_time = ready_sorted[0]->get_Sched_Info()->get_Feasible_ALAP();
    int end = ready_sorted[0]->get_Sched_Info()->get_Feasible_ASAP();
    for (int i = schedule_time; i >= end; i--)
    {
      current_schedule_time = i;	      //selected time slot to schedule the node
      modulo_schedule_time = i % II;	  //selected modulo time slot to schedule the node
      //corner case, when II == 1, all nodes are to be schedule at modulo cycle 0
      if (II == 1)
        modulo_schedule_time = 0;

      DEBUG_MODULO_SCHEDULING("Node %d is to be scheduled at %d, modulo time %d", ready_sorted[0]->get_ID(), current_schedule_time, modulo_schedule_time);
      // if selected not is a load address assert
      if (ready_sorted[0]->is_Load_Address_Generator())
      {
        //assume node is not constrained by multiple predecessors
        bool multiple_constraint_schedule = false;
        //schedule both load nodes
        bool success = Schedule_Load_Address(ready_sorted[0], current_schedule_time, II, multiple_constraint_schedule, Address_BUS, Data_BUS, Total_Resources, number_of_resources, scheduled);
        //if failed due to multiple contraints for a node
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
          continue;
      }
      //if node is load data bus read
      else if (ready_sorted[0]->is_Load_Data_Bus_Read())
      {
        //assume no multiple constraints for a node
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
          continue;
      }
      //if node is store operation
      else if (ready_sorted[0]->is_Store_Address_Generator() || ready_sorted[0]->is_Store_Data_Bus_Write())
      {
        //assume no multiple constraints
        bool multiple_constraint_schedule = false;
        //schedule both store operations
        bool success = Schedule_Store_Address_Data(ready_sorted[0], current_schedule_time, II, multiple_constraint_schedule, Address_BUS, Data_BUS, Total_Resources, number_of_resources,scheduled);

        //if failed due to multiple constraints
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
          continue;
      }
      //regular nodes
      else
      {
        if (Total_Resources[modulo_schedule_time] < (number_of_resources))
        {
          Node* node_in_conflict_path=NULL;
          //check if there is constrain conflict with scheduled nodes
          if (has_node_conflict_with_scheduled_nodes(II, ready_sorted[0], current_schedule_time, modulo_schedule_time, scheduled, node_in_conflict_path))
          {
            //if schedule window is empty, resolve conflict here
            DEBUG_MODULO_SCHEDULING("Node %d is found in conflict",(node_in_conflict_path)->get_ID());
            if (current_schedule_time == end || II==1)
            {
              //find out the predecessor and successor between nodes
              if (ready_sorted[0]->is_Connected_To(node_in_conflict_path))
              {
                //create a routing node
                tempDummyNode= new Node_Dummy(1, Get_Unique_Index(),ready_sorted[0]);
                if ((int) tempDummyNode->get_Number_of_Pred()>0)
                {
                  Add_Arc( tempDummyNode->get_Pred_Arc(0));
                }

                insert_Node(tempDummyNode); //insert it
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
                tempDummyNode= new Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path);
                if ((int) tempDummyNode->get_Number_of_Pred()>0)
                {
                  Add_Arc( tempDummyNode->get_Pred_Arc(0));
                }

                insert_Node(tempDummyNode); //insert it
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
          //if a node constrains multiple nodes
          if (constrains_multiple(II,ready_sorted[0], current_schedule_time, node_in_conflict_path, scheduled))
          {
            //create a routing node
            tempDummyNode= new Node_Dummy(1, Get_Unique_Index(),ready_sorted[0] );
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }
            insert_Node(tempDummyNode); //insert it

            //insert routing in between
            if (insert_Node_in_between_output(ready_sorted[0], node_in_conflict_path, tempDummyNode))
            {
              multiple_constraint = true;
              return false;
            }

            FATAL(true, "node %d constrains multiple successor but we could not insert a node between it and one of constrained successor", ready_sorted[0]->get_ID());
            return false;
          }
          //if multiple successors are scheduled such that the related node constrain them
          if (has_multiple_constrains(II, ready_sorted[0], current_schedule_time, node_in_conflict_path, scheduled))
          {
            //create a routing node
            tempDummyNode= new Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path );
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }

            insert_Node(tempDummyNode); //insert it to DDG
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
          DEBUG_MODULO_SCHEDULING("Not enough resources available at %d PE used %d", schedule_time, Total_Resources[modulo_schedule_time]);
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
 *
 */
bool DFG::Schedule_Load_Address(Node* selected_node, int schedule_time, int II, bool &multiple_constraint, int* address_bus, int* data_bus, int* PE_resources, int max_resources, vector<Node*> &scheduled_nodes)
{
  int current_schedule_time_2;	//schedule cycle for related node
  int modulo_schedule_time_2;	  //modulo scheduled cycle for related node
  int modulo_schedule_time;	    //modulo scheduled cycle for node

  //check if the related node is ready to schedule, if yes, when?
  if (selected_node->get_Related_Node()->is_ready_to_schedule_modulo_backward(current_schedule_time_2))
  {
    //if related node does not have any successor in this iteration, it can be scheduled at any cycle!!!
    if ((int) selected_node->get_Related_Node()->Get_Successors_Same_Iteration_Exclude_Address_Dependency().size() == 0)
    {
      current_schedule_time_2 = schedule_time + 1;	   //schedule data read at next cycle
      if (current_schedule_time_2 > MAX_SCHEDULE_LEN)	 //no node can be scheduled at any cycle more than MAX LEN
        return false;
    }
    //if load data read should be schedule at > schedule_time + 1, we are following ALAP policy so we schedule it earlier!! //can be dangerous node might be pushed back to < 0!!!!
    if (current_schedule_time_2 > schedule_time + 1)
      current_schedule_time_2 = schedule_time + 1;    //pull schedule time to proper earlier time
    //if load address is ahead of data read, pull back load address schedule time
    if (current_schedule_time_2 < schedule_time + 1)
      schedule_time = current_schedule_time_2 - 1;	      //pull schedule time to proper earlier time

    modulo_schedule_time_2 = current_schedule_time_2 % II;	    //modulo schedule time for both load instructions
    modulo_schedule_time = schedule_time % II;

    if (II == 1)	    //if II == 1, all instructions are executed at modulo time 0
    {
      modulo_schedule_time_2 = 0;
      modulo_schedule_time = 0;
    }

    //it is scheduled a cycle after selected node
    //check if there is enough resources to schedule both nodes
    if (HAS_ENOUGH_RESOURCES_FOR_LOAD_INSTRUCTION_AT_CYCLE(modulo_schedule_time, modulo_schedule_time_2, address_bus, data_bus, PE_resources, max_resources))
    {
      //there is no schedule conflict if it is scheduled at this time
      //node to insert in case of multiple constrained nodes
      Node_Dummy* tempDummyNode;
      Node* node_in_conflict_path=NULL;
      if (has_node_conflict_with_scheduled_nodes(II, selected_node, schedule_time, modulo_schedule_time, scheduled_nodes, node_in_conflict_path))
      {
        //if this is the last time slot or II==1, conflict has be take care of at this stage
        if (selected_node->get_Sched_Info()->get_Feasible_ASAP()==schedule_time || II==1)
        {
          //find out the predecessor and successor between nodes
          if (selected_node->is_Connected_To(node_in_conflict_path))
          {
            //create a routing node
            tempDummyNode= new Node_Dummy(1, Get_Unique_Index(), selected_node);
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
              Add_Arc( tempDummyNode->get_Pred_Arc(0));

            insert_Node(tempDummyNode);	  //insert it

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
            tempDummyNode= new Node_Dummy(1, Get_Unique_Index(), node_in_conflict_path);
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
              Add_Arc( tempDummyNode->get_Pred_Arc(0));

            insert_Node(tempDummyNode);	    //insert it

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
      if (has_node_conflict_with_scheduled_nodes(II, selected_node->get_Related_Node(), current_schedule_time_2, modulo_schedule_time_2, scheduled_nodes, node_in_conflict_path))
      {
        //if this is the last time slot or II==1, conflict has be take care of at this stage
        if (selected_node->get_Related_Node()->get_Sched_Info()->get_Feasible_ASAP()==current_schedule_time_2 || II==1)
        {
          //find out the predecessor and successor between nodes
          if (selected_node->get_Related_Node()->is_Connected_To(node_in_conflict_path))
          {
            //create a routing node
            tempDummyNode= new Node_Dummy(1, Get_Unique_Index(),selected_node->get_Related_Node() );
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
              Add_Arc( tempDummyNode->get_Pred_Arc(0));

            insert_Node(tempDummyNode);	   //insert it to DDG

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
            tempDummyNode= new Node_Dummy(1, Get_Unique_Index(), node_in_conflict_path);
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }

            insert_Node(tempDummyNode); //insert it to DDG

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
      if (constrains_multiple(II, selected_node, schedule_time, node_in_conflict_path, scheduled_nodes))
      {
        //construct a routing node
        tempDummyNode = new Node_Dummy(1, Get_Unique_Index(), selected_node);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
          Add_Arc( tempDummyNode->get_Pred_Arc(0));

        insert_Node(tempDummyNode);	  //insert it to DDG
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
      if (constrains_multiple(II, selected_node->get_Related_Node(), current_schedule_time_2, node_in_conflict_path, scheduled_nodes))
      {
        //construct a routing node
        tempDummyNode = new Node_Dummy(1, Get_Unique_Index(),selected_node->get_Related_Node() );
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
          Add_Arc( tempDummyNode->get_Pred_Arc(0));

        insert_Node(tempDummyNode);	  //insert it to DDG
        //insert it between this and selected constrained node
        if (insert_Node_in_between_output(selected_node->get_Related_Node(), node_in_conflict_path, tempDummyNode))
        {
          multiple_constraint = true;	          //there was multiple constrained node, let caller know
          return false;			          //scheduling did not succeed
        }
        FATAL(true, "node %d constrains multiple successor but we could not insert a node between it and one of constrained successor", selected_node->get_Related_Node()->get_ID());
        return false;
      }
      //if constrains multiple predecessors constrained this node
      if (has_multiple_constrains(II,selected_node, schedule_time, node_in_conflict_path, scheduled_nodes))
      {
        tempDummyNode = new Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path );
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
      if (has_multiple_constrains(II, selected_node->get_Related_Node(), current_schedule_time_2, node_in_conflict_path, scheduled_nodes))
      {
        tempDummyNode = new Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path);
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
      DEBUG_MODULO_SCHEDULING("Not enough resources available at %d PE used %d Add_bus used %d Data_bus used %d  and at %d PE used %d Add_bus used %d Data_bus used %d",
		    schedule_time, address_bus[modulo_schedule_time], data_bus[modulo_schedule_time], PE_resources[modulo_schedule_time], current_schedule_time_2,
		    address_bus[modulo_schedule_time_2], data_bus[modulo_schedule_time_2], PE_resources[modulo_schedule_time_2]);
    }
  }
  else
  {
    DEBUG_MODULO_SCHEDULING("Related node %d is not ready yet", selected_node->get_Related_Node()->get_ID());
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
 */
bool DFG::Schedule_Load_Data(Node* selected_node, int schedule_time, int II, bool &multiple_constraint, int* address_bus, int* data_bus, int* PE_resources, int max_resources, vector<Node*> &scheduled_nodes)
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
      //node to insert in case of multiple constrained nodes
      Node_Dummy* tempDummyNode;
      Node* node_in_conflict_path=NULL;
      if (has_node_conflict_with_scheduled_nodes(II, selected_node, schedule_time, modulo_schedule_time, scheduled_nodes, node_in_conflict_path))
      {
        //if this is the last time slot or II==1, conflict has be take care of at this stage
        if (selected_node->get_Sched_Info()->get_Feasible_ASAP()==schedule_time || II==1)
        {
          //find out the predecessor and successor between nodes
          if (selected_node->is_Connected_To(node_in_conflict_path))
          {
            //create a routing node
            tempDummyNode= new Node_Dummy(1, Get_Unique_Index(),selected_node );
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
            tempDummyNode= new Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path);
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }

            insert_Node(tempDummyNode); //insert it to DDG
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
      if (has_node_conflict_with_scheduled_nodes(II, selected_node->get_Related_Node(), current_schedule_time_2, modulo_schedule_time_2, scheduled_nodes, node_in_conflict_path))
      {
        //if this is the last time slot or II==1, conflict has be take care of at this stage
        if (selected_node->get_Related_Node()->get_Sched_Info()->get_Feasible_ASAP()==current_schedule_time_2 || II==1)
        {
          //find out the predecessor and successor between nodes
          if (selected_node->get_Related_Node()->is_Connected_To(node_in_conflict_path))
          {
            //create a routing node
            tempDummyNode= new Node_Dummy(1, Get_Unique_Index(), selected_node->get_Related_Node());
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }

            insert_Node(tempDummyNode); // Insert it to DDG

            //insert a routing node to resolve the conflict
            if (insert_Node_in_between_output(selected_node->get_Related_Node(), node_in_conflict_path, tempDummyNode))
            {
              multiple_constraint = true;
              return false;
            }
          }
          else
          {
            //create a routing node
            tempDummyNode= new Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path );
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }

            insert_Node(tempDummyNode); //insert it to DDG
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
      if (constrains_multiple(II, selected_node, schedule_time, node_in_conflict_path, scheduled_nodes))
      {
        //construct a routing node
        tempDummyNode = new Node_Dummy(1, Get_Unique_Index(),selected_node);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }

        insert_Node(tempDummyNode); //insert it to DDG
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
      if (constrains_multiple(II, selected_node->get_Related_Node(), current_schedule_time_2, node_in_conflict_path, scheduled_nodes))
      {
        //construct a routing node
        tempDummyNode = new Node_Dummy(1, Get_Unique_Index(),selected_node->get_Related_Node());
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }

        insert_Node(tempDummyNode); //insert it to DDG
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
      if (has_multiple_constrains(II, selected_node, schedule_time, node_in_conflict_path, scheduled_nodes))
      {
        tempDummyNode = new Node_Dummy(1, Get_Unique_Index(), node_in_conflict_path);
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
      if (has_multiple_constrains(II, selected_node->get_Related_Node(),current_schedule_time_2,  node_in_conflict_path, scheduled_nodes))
      {
        tempDummyNode = new Node_Dummy(1, Get_Unique_Index(), node_in_conflict_path);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }

        insert_Node(tempDummyNode); // Insert it to DDG
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
    DEBUG_MODULO_SCHEDULING("Related node %d is not ready yet", selected_node->get_Related_Node()->get_ID());

  return false;
}

/*
 * Schedule a store instruction along with related node
 * (1) store address/data  assert instruction, (2) time slot to schedule the operation
 * (3) current II, (4) sets true if node is constrained by multiple predecessor
 * (5) address bus resource table, (6) data bus resource table
 * (7) PE resource table, (8) max number of available resources at each cycle
 * (9) scheduled operations
 */
bool DFG::Schedule_Store_Address_Data(Node* selected_node, int schedule_time, int II, bool &multiple_constraint, int* address_bus, int* data_bus, int* PE_resources, int max_resources, vector<Node*> &scheduled_nodes)
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
      Node_Dummy* tempDummyNode;
      //there is no schedule conflict if it is scheduled at this time
      Node* node_in_conflict_path=NULL;
      if (has_node_conflict_with_scheduled_nodes(II, selected_node, schedule_time, modulo_schedule_time, scheduled_nodes, node_in_conflict_path))
      {
        //if this is the last time slot or II==1, conflict has be take care of at this stage
        if (selected_node->get_Sched_Info()->get_Feasible_ASAP()==schedule_time || II==1)
        {
          //find out the predecessor and successor between nodes
          if (selected_node->is_Connected_To(node_in_conflict_path))
          {
            //create a routing node
            tempDummyNode= new Node_Dummy(1, Get_Unique_Index(), selected_node);
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }

            insert_Node(tempDummyNode); // Insert it
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
            tempDummyNode= new Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path);
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }

            insert_Node(tempDummyNode); //insert it to DDG
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
      if (has_node_conflict_with_scheduled_nodes(II, selected_node->get_Related_Node(), current_schedule_time_2, modulo_schedule_time_2, scheduled_nodes, node_in_conflict_path))
      {
        //if this is the last time slot or II==1, conflict has be take care of at this stage
        if (selected_node->get_Related_Node()->get_Sched_Info()->get_Feasible_ASAP()==current_schedule_time_2 || II==1)
        {
          //find out the predecessor and successor between nodes
          if (selected_node->get_Related_Node()->is_Connected_To(node_in_conflict_path))
          {
            //create a routing node
            tempDummyNode= new Node_Dummy(1, Get_Unique_Index(), selected_node->get_Related_Node());
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }

            insert_Node(tempDummyNode); // Insert it
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
            tempDummyNode= new Node_Dummy(1, Get_Unique_Index(), node_in_conflict_path);
            if ((int) tempDummyNode->get_Number_of_Pred()>0)
            {
              Add_Arc( tempDummyNode->get_Pred_Arc(0));
            }

            insert_Node(tempDummyNode); //insert it
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
      if (constrains_multiple(II, selected_node, schedule_time, node_in_conflict_path, scheduled_nodes))
      {
        //construct a routing node
        tempDummyNode = new Node_Dummy(1, Get_Unique_Index(),selected_node );
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }

        insert_Node(tempDummyNode); //insert it
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
      if (constrains_multiple(II,selected_node->get_Related_Node(), current_schedule_time_2, node_in_conflict_path, scheduled_nodes))
      {
        //construct a routing node
        tempDummyNode = new Node_Dummy(1, Get_Unique_Index(), selected_node->get_Related_Node());
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }

        insert_Node(tempDummyNode); //insert it
        //insert it between this and selected constrained node
        if (insert_Node_in_between_output(selected_node->get_Related_Node(), node_in_conflict_path, tempDummyNode))
        {
          multiple_constraint = true;
          //scheduling did not succeed
          return false;
        }
        FATAL(true, "node %d constrains multiple successor but we could not insert a node between it and one of constrained successor", selected_node->get_Related_Node()->get_ID());
        return false;
      }
      //if constrains multiple predecessors constrained this node
      if (has_multiple_constrains(II,selected_node, schedule_time, node_in_conflict_path, scheduled_nodes))
      {

        tempDummyNode = new Node_Dummy(1, Get_Unique_Index(), node_in_conflict_path);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }

        insert_Node(tempDummyNode); //insert it
        if (insert_Node_in_between_output(node_in_conflict_path,selected_node,  tempDummyNode))
        {
          multiple_constraint = true;
          return false;
        }
        FATAL(true, "node %d is constrained by multiple predecessors but we could not insert a node between it and one of constrainers", selected_node->get_ID());
        return false;
      }
      //if multiple predecessors constrained the related node
      if (has_multiple_constrains(II, selected_node->get_Related_Node(), current_schedule_time_2, node_in_conflict_path, scheduled_nodes))
      {
        tempDummyNode = new Node_Dummy(1, Get_Unique_Index(),node_in_conflict_path);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
        {
          Add_Arc( tempDummyNode->get_Pred_Arc(0));
        }

        insert_Node(tempDummyNode); //insert it
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
      DEBUG_MODULO_SCHEDULING("Not enough resources available at %d PE used %d Add_bus used %d Data_bus used %d and at %d PE used %d Add_bus used %d Data_bus used %d", schedule_time, address_bus[modulo_schedule_time], data_bus[modulo_schedule_time], PE_resources[modulo_schedule_time], current_schedule_time_2, address_bus[modulo_schedule_time_2], data_bus[modulo_schedule_time_2], PE_resources[modulo_schedule_time_2]);
  }
  else
    DEBUG_MODULO_SCHEDULING("Related node %d is not ready yet", selected_node->get_Related_Node()->get_ID());

  return false;
}

void DFG::Reset_ASAP_Schedule()
{
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    _node_Set[i]->get_Sched_Info()->reset_ASAP();
  }
}

//reset ALAP schedule time of all nodes
void DFG::Reset_ALAP_Schedule()
{
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    _node_Set[i]->get_Sched_Info()->reset_ALAP();
  }
}

void DFG::Reset_ASAP_Feasible_Schedule()
{
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    _node_Set[i]->get_Sched_Info()->reset_Feasible_ASAP();
  }
}

void DFG::Reset_ALAP_Feasible_Schedule()
{
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    _node_Set[i]->get_Sched_Info()->reset_Feasible_ALAP();
  }
}

void DFG::Reset_Current_Schedule()
{
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    _node_Set[i]->get_Sched_Info()->reset_Current();
  }
}

/*
 *  construct a list of ready to schedule operations
 *  operations in this list are sorted by their slack
 *  input list is the set of given nodes to the function
 * note load address generator and store data bus assert are never returned in ready list
 * possibly can cause a bug in rare cases if all remaining nodes depend on scheduling one of above nodes
 */
vector<Node*> DFG::get_sorted_ready_list_modulo_backward(vector<Node*> list)
{
  //list of nodes ready to be scheduled
  vector<Node*> ready_list;
  vector<Node*> sorted_list;

  int time;
  for (int i = 0; i < (int) list.size(); i++)
  {
    //ignore the node if it is load address generator or store bus assert
    if (list[i]->is_Load_Address_Generator() || list[i]->is_Store_Address_Generator())
      continue;

    //is it is a memory type operation but the related not is not ready yet
    if (list[i]->is_Load_Data_Bus_Read() || list[i]->is_Store_Data_Bus_Write())
    {
      if (!list[i]->get_Related_Node()->is_ready_to_schedule_modulo_backward(time))
        continue;
    }
    //for the ready nodes, assign a schedule window between current time and ASAP instead of ALAP to ASAP
    if (list[i]->is_ready_to_schedule_modulo_backward(time))
    {
      list[i]->get_Sched_Info()->set_Feasible_ALAP(time);
      if (time == MAXINT)
      {
        DEBUG(time==MAXINT, "operation is assigned to an unjustified cycle");
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
  int min_index = 0;
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

//  return list of current scheduled nodes
vector<Node*> DFG::get_Current_Scheduled_Nodes(vector<Node*> inSet)
{
  vector<Node*> visited;

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
 * this functions finds the set of all nodes that should be mapped to
 * the same physical PE where current node * is to be mapped.
 * This function finds PE conflict when nodes are scheduled to prevent
 * possible placement conflict
 * inputs are: (1) given node (2) schedule cycle of the node
 */
vector<Node*> DFG::get_All_Nodes_Constrained_by_Current(Node* current_node, int time)
{
  //visited nodes through inter-iteration dependencies
  vector<Node*> visited_inter;
  //visited though intra-iteration dependency where dependent nodes
  // are scheduled on two non-consecutive cycles
  vector<Node*> visited_intra;
  //to be visited through inter iteration
  vector<Node*> to_visit_inter;
  //to be visited though intera iteration dependency
  vector<Node*> to_visit_intra;
  vector<Node*> temp_1;
  vector<Node*> temp_2;
  Node* temp_node;

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
 * This function finds all nodes connected to current node with
 * inter-iteration dependency arc, these nodes are to be mapped to
 * the same physical PE */
vector<Node*> DFG::get_Inter_Iteration_Related_Nodes(Node* current_node)
{
  vector<Node*> visited;
  vector<Node*> rest;
  vector<Node*> temp_1;
  vector<Node*> temp_2;
  Node* temp_node;
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
 * This function finds all nodes connected to current node with regular arc,
 * these nodes are not scheduled in consecutive cycles
 * so must be mapped to same physical PEs
 */
vector<Node*> DFG::get_Intra_Iteration_Related_Nodes_Current(Node* current_node, int time)
{
  vector<Node*> rest;
  vector<Node*> temp_1;
  vector<Node*> temp_2;

  temp_1 = current_node->Get_Prev_Nodes_Same_Iteration();
  for (int i = 0; i < (int) temp_1.size(); i++)
  {
    if (temp_1[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      if (time - temp_1[i]->get_Sched_Info()->get_Current() > 1)
        rest.push_back(temp_1[i]);
    }
  }

  temp_2 = current_node->Get_Next_Nodes_Same_Iteration();
  for (int i = 0; i < (int) temp_2.size(); i++)
  {
    if (temp_2[i]->get_Sched_Info()->is_CURRENT_Initiated())
    {
      if (temp_2[i]->get_Sched_Info()->get_Current() - time > 1)
        rest.push_back(temp_2[i]);
    }
  }

  return rest;
}

/*
 * find node conflict (2 nodes that should be mapped to same physical PE)
 * between already scheduled operations and current node;
 * to be used in FEASIBLE_ASAP_SCHEDULE only
 * inputs: (1) node for which check for conflict (2) time to schedule the node
 */
bool DFG::has_Node_Conflict_Feasible_ASAP(Node* current_node, int time)
{
  vector<Node*> related = get_Inter_Iteration_Related_Nodes(current_node);
  for (int i = 0; i < (int) related.size(); i++)
  {
    if (related[i]->get_Sched_Info()->is_Feasible_ASAP_Initiated())
    {
      if (related[i]->get_Sched_Info()->get_Feasible_ASAP() == time)
        return true;
    }
  }
  return false;
}

/*
 * find node conflict (2 nodes that should be mapped to same physical PE)
 * between already scheduled operations and current node;
 * to be used in FEASIBLE_ALAP_SCHEDULE only
 * inputs: (1) node for which check for conflict (2) time to schedule the node
 */
bool DFG::has_Node_Conflict_Feasible_ALAP(Node* current_node, int time)
{
  vector<Node*> related = get_Inter_Iteration_Related_Nodes(current_node);
  for (int i = 0; i < (int) related.size(); i++)
  {
    if (related[i]->get_Sched_Info()->is_Feasible_ALAP_Initiated())
    {
      if (related[i]->get_Sched_Info()->get_Feasible_ALAP() == time)
        return true;
    }
  }
  return false;
}

/*
 * find if there is a conflict with node src node to be scheduled with all scheduled nodes
 * if one is found, returns true
 */
bool DFG::has_node_conflict_with_scheduled_nodes(int II, Node* current, int schedule_time, int modulo_schedule, vector<Node*> &scheduled_nodes, Node* &node_in_conflict_path)
{
  if (has_node_forward_conflict_with_scheduled_nodes(II, current, schedule_time, modulo_schedule, scheduled_nodes,node_in_conflict_path))
  {
    DEBUG_CONFLICT("Node %d has forward conflict with node %d",current->get_ID(),node_in_conflict_path->get_ID());
    return true;
  }
  if (has_node_backward_conflict_with_scheduled_nodes(II, current, schedule_time, modulo_schedule, scheduled_nodes, node_in_conflict_path))
  {
    DEBUG_CONFLICT("Node %d has backward conflict with node %d",current->get_ID(),node_in_conflict_path->get_ID());
    return true;
  }
  return false;
}

/*
 * check if current node that is to be scheduled has conflict in forward direction with already scheduled nodes
 * if so return true and first node in conflict path
 */
bool DFG::has_node_forward_conflict_with_scheduled_nodes(int II, Node* current, int schedule_time, int modulo_schedule, vector<Node*> &scheduled_nodes, Node* &first_node_in_conflict_path)
{
  for (int i = 0; i < (int) scheduled_nodes.size(); i++)
  {
    if (has_forward_conflict(II, current, schedule_time, modulo_schedule, scheduled_nodes[i], scheduled_nodes[i]->get_Sched_Info()->get_Current(), scheduled_nodes[i]->get_Sched_Info()->get_Modulo_Current(), first_node_in_conflict_path))
      return true;
  }
  return false;
}

/*
 * check if current node that is to be scheduled has conflict in backward direction with already scheduled nodes
 * if so return true and last node in conflict path
 */
bool DFG::has_node_backward_conflict_with_scheduled_nodes(int II, Node* current, int schedule_time, int modulo_schedule, vector<Node*> &scheduled_nodes, Node* &last_node_in_conflict_path)
{
  for (int i = 0; i < (int) scheduled_nodes.size(); i++)
  {
    if (has_backward_conflict(II, scheduled_nodes[i], scheduled_nodes[i]->get_Sched_Info()->get_Current(), scheduled_nodes[i]->get_Sched_Info()->get_Modulo_Current(), current, schedule_time, modulo_schedule, last_node_in_conflict_path))
      return true;
  }
  return false;
}

/*
 * recursively explored the DFG in forward direction
 * only travers nodes that are already scheduled, and are consrtained by src
 * finds a contrain conflict
 * function arguments: (1) src, a scheduled node (2) target node
 * (3) set of all visited nodes
 */
bool DFG::traverse_constrain_path_recursive(Node* src, Node* target, vector<Node*> &visited)
{
  //if we reached to target, there is a path
  if (src == target)
    return true;
  //if already scheduled, return false
  if (contains(visited, src))
    return false;

  //add src to set of visited nodes
  visited.push_back(src);
  //find successors of scr in this iteration
  vector<Node*> successors = src->Get_Explicit_Dependency_Successors_Same_Iteration();
  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) successors.size(); i++)
  {
    //ignore unscheduled operations
    if (!successors[i]->get_Sched_Info()->is_CURRENT_Initiated())
      continue;

    //check if src constrains next node
    if (src->get_Sched_Info()->get_Current() < successors[i]->get_Sched_Info()->get_Current() - src->get_Latency())
    {
      //can reach target from next?
      if (traverse_constrain_path_recursive(successors[i], target, visited))
        return true;
    }
  }
  //get next nodes in next iterations
  successors = src->Get_Explicit_Dependency_Successors_Next_Iterations();
  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) successors.size(); i++)
  {
    //ignore unscheduled operations
    if (!successors[i]->get_Sched_Info()->is_CURRENT_Initiated())
      continue;
    //can reach target from next?
    if (traverse_constrain_path_recursive(successors[i], target, visited))
      return true;
  }
  return false;
}

/*
 * find a constrain conflict between src and target, src is not scheduled yet but target is scheduled
 * this function explores the DFG graph in forward direction to find a conflict
 * (1) src node (2) time src node is scheduled (3) modulo scheduled cycle of src
 * (4) target node (5) target schedule cycle (6) target modulo cycle
 * (7) if there is a conflict, first node in conflict path
 */
bool DFG::has_forward_conflict(int II, Node* src, int src_schedule_cycle, int src_modulo_cycle, Node* target, int target_schedule_cycle, int target_modulo_cycle, Node* &first_node_in_conflict_path)
{
  //temp storage to travers DFG
  vector<Node*> visited;
  //find successors of src in this iteration
  vector<Node*> successors = src->Get_Explicit_Dependency_Successors_Same_Iteration();
  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) successors.size(); i++)
  {
    //ignore unscheduled operations
    if (!successors[i]->get_Sched_Info()->is_CURRENT_Initiated())
      continue;
    //check if src constrains next node
    if (src_schedule_cycle < successors[i]->get_Sched_Info()->get_Current() - src->get_Latency())
    {
      //can reach target from next?
      if (traverse_constrain_path_recursive(successors[i], target, visited))
      {
        //check and update if target node has recurrency
        ARC* edge = get_Arc(src,successors[i]);
        if(successors[i]->get_ID() != target->get_ID())
          edge = get_Arc(successors[i],target);
        int updated_target_schedule_cycle = target_schedule_cycle;
        unsigned  distance = 0;
        if(edge == NULL)
        {
          debugfile << "1. src is: " << src->get_ID() << "-" << src_schedule_cycle << "\ttarget is: " << target->get_ID() << "-" << target_schedule_cycle << endl;
          debugfile << "1. Edge points to NULL!\n";
        }
        else
        {
          distance = edge->get_Distance();
        }
        if(distance > 0) updated_target_schedule_cycle += distance * II;
        //if reachable, must be scheduled at different modulo cycle! (only if they are from same iteration)
        if( (src_modulo_cycle == target_modulo_cycle) && (src_schedule_cycle >= updated_target_schedule_cycle))
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
      continue;
    //can reach target from next?
    if (traverse_constrain_path_recursive(successors[i], target, visited))
    {
    	//check and update if target node has recurrency
    	ARC* edge = get_Arc(src,successors[i]);
      unsigned distance = 0;
      if(edge == NULL)
      {
        debugfile << "2. Edge points to NULL!\n";
        debugfile << successors[i]->get_ID() << " 2. src is: " << src->get_ID() << "-" << src_schedule_cycle << "\ttarget is: " << target->get_ID() << "-" << target_schedule_cycle << endl;
      }
      else
      {
        distance = edge->get_Distance();
      }
    	int updated_target_schedule_cycle = target_schedule_cycle;
    	if(distance > 0) updated_target_schedule_cycle += distance * II;
      //if reachable, must be scheduled at different modulo cycle! (only if they are from same iteration)
      if( (src_modulo_cycle == target_modulo_cycle) && (src_schedule_cycle >= updated_target_schedule_cycle))
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
 * find a constrain conflict between src and target, target is not scheduled yet but target is scheduled
 * this function explores the DFG graph in forward direction to find a conflict
 * (1) src node (2) time src node is scheduled (3) modulo scheduled cycle of src
 * (4) target node (5) target schedule cycle (6) target modulo cycle
 * (7) if there is a conflict, first node in conflict path
 * FIXME: only travers through actual arcs 20-2-2014
 */
bool DFG::has_backward_conflict(int II, Node* src, int src_schedule_cycle, int src_modulo_cycle, Node* target, int target_schedule_cycle, int target_modulo_cycle, Node* &last_node_in_conflict_path)
{
  //temp storage to travers DFG
  vector<Node*> visited;
  //find successors of scr in this iteration
  vector<Node*> predecessors = target->Get_Explicit_Dependency_Predecessors_Same_Iteration();
  int updated_target_schedule_cycle = target_schedule_cycle;

  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) predecessors.size(); i++)
  {
    //ignore unscheduled operations
    if (!predecessors[i]->get_Sched_Info()->is_CURRENT_Initiated())
      continue;

    //check if src constrains next node
    if (predecessors[i]->get_Sched_Info()->get_Current() + predecessors[i]->get_Latency() < updated_target_schedule_cycle)
    {
      //can reach target from next?
      if (traverse_constrain_path_recursive(src, predecessors[i], visited))
      {
        //check and update if target node has recurrency
      	ARC* edge = get_Arc(src,target);
      	if(edge->get_Distance() > 0)
          updated_target_schedule_cycle += edge->get_Distance() * II;

        //if reachable, must be scheduled at different modulo cycle!
        if((src_modulo_cycle == target_modulo_cycle) && (src_schedule_cycle >= updated_target_schedule_cycle))
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
    	// iterate to all possible inter-iteration edges and
      // find max. inter-iteration distance, if any.
    	ARC* edge = get_Arc(predecessors[i],target);
    	if(edge->get_Distance() > 0) updated_target_schedule_cycle += edge->get_Distance() * II;

      //if reachable, must be scheduled at different modulo cycle!
      if((src_modulo_cycle == target_modulo_cycle) && (src_schedule_cycle >= updated_target_schedule_cycle))
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
bool DFG::constrains_multiple(int II, Node* current, int schedule_time, Node* &first_node_in_constrain_path, vector<Node*> &scheduled_nodes)
{
  int counter=0;
  for (int i=0; i <(int)scheduled_nodes.size(); i++)
  {
    if (constrains(II, current, schedule_time, scheduled_nodes[i], scheduled_nodes[i]->get_Sched_Info()->get_Current(), scheduled_nodes[i]->get_Sched_Info()->get_Modulo_Current(),first_node_in_constrain_path))
      counter++;

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
bool DFG::has_multiple_constrains(int II, Node* current, int schedule_time, Node* &last_node_in_constrain_path, vector<Node*> &scheduled_nodes)
{
  int counter = 0;
  for (int i = 0; i < (int) scheduled_nodes.size(); i++)
  {
    if (is_constrained(II, scheduled_nodes[i], scheduled_nodes[i]->get_Sched_Info()->get_Current(), scheduled_nodes[i]->get_Sched_Info()->get_Modulo_Current(), current, schedule_time, last_node_in_constrain_path))
      counter++;

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
bool DFG::constrains(int II, Node* src, int src_schedule_cycle, Node* target, int target_schedule_cycle, int target_modulo_cycle, Node* &first_node_in_conflict_path)
{
  //temp storage to travers DFG
  vector<Node*> visited;
  //find successors of scr in this iteration
  vector<Node*> successors = src->Get_Next_Nodes_Same_Iteration();
  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) successors.size(); i++)
  {
    //ignore unscheduled operations
    if (!successors[i]->get_Sched_Info()->is_CURRENT_Initiated())
      continue;
    //check if src constrains next node
    int updated_successor_cycle = successors[i]->get_Sched_Info()->get_Current();
    ARC* edge = get_Arc(src,successors[i]);
    if(edge->get_Distance() > 0)
      updated_successor_cycle += edge->get_Distance() * II;

    if (src_schedule_cycle < updated_successor_cycle - src->get_Latency())
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
      continue;
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
bool DFG::is_constrained(int II, Node* src, int scr_schedule_cycle, int src_modulo_cycle, Node* target, int target_schedule_cycle, Node* &last_node_in_conflict_path)
{
  //temp storage to travers DFG
  vector<Node*> visited;
  //find successors of scr in this iteration
  vector<Node*> predecessors = target->Get_Prev_Nodes_Same_Iteration();
  //check if all next nodes are schedule so that this node constrains them
  for (int i = 0; i < (int) predecessors.size(); i++)
  {
    //ignore unscheduled operations
    if (!predecessors[i]->get_Sched_Info()->is_CURRENT_Initiated())
      continue;

    //check if src constrains next node
    int updated_target_schedule_cycle = target_schedule_cycle;
    ARC* edge = get_Arc(predecessors[i],src);
    unsigned distance = 0;
    if(edge != NULL)
      distance = edge->get_Distance();
    if(distance > 0)
      updated_target_schedule_cycle += distance * II;
    if (predecessors[i]->get_Sched_Info()->get_Current() + predecessors[i]->get_Latency() < updated_target_schedule_cycle)
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
      continue;

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
 * returns the number of required register between two operations
 * node1 and node2 and given II
 */

int DFG::get_Required_Register(Node* node1, Node* node2, int II)
{
  int inter = 0;
  int intra = 0;
  ARC *arc = get_Arc(node1, node2);
  unsigned e = 0;
  if(arc != NULL)
    e = arc->get_Distance();
  //is it inter iteration(loop carried) dependency
  if (e > 0)
  {
    //TODO find rec_dist for node2 from vector and update e.
    std::map<int,int>::iterator it1 = node_new_inter_it_dist.find(node1->get_ID());
    std::map<int,int>::iterator it2 = node_new_inter_it_dist.find(node2->get_ID());

    //node1 is scheduled before node2
    if (node1->get_Sched_Info()->get_Current() <= node2->get_Sched_Info()->get_Current())
    {
      inter = (int) ceil((float)(node2->get_Sched_Info()->get_Current() - node1->get_Sched_Info()->get_Current()) / (float)II);
      inter = inter + e;
    }
    else
    {
      inter = (int) ceil((float) (node1->get_Sched_Info()->get_Current() - node2->get_Sched_Info()->get_Current()) / (float) II);
      inter = e - inter + 1;

      if((node1->get_Sched_Info()->get_Current() - node2->get_Sched_Info()->get_Current()) == ((int)(e*II - 1)))
	     inter=0;
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

vector<Node*> DFG::Get_SortOps(int &MII, int number_of_resources)
{

  int recMII = 0, resMII = 0;
  vector < vector<Node*> > Sccs;

  vector < vector<Node*> > Sorted_Sccs;
  Strongly_Connected (Sccs);
  Sort_Sccs(Sccs);

  if (Sccs.size() > 0)
    recMII = Sccs[0][0]->get_Sched_Info()->get_SCC_Delay();

  resMII = (_node_Set.size() - 1) / number_of_resources;
  resMII++;

  if (recMII > resMII)
  {
    MII = recMII;
  }
  else
    MII = resMII;

  vector<Node*> set = _node_Set;
  Reset_ASAP_Schedule();
  int last_time = Schedule_ASAP();

  Reset_ALAP_Schedule();
  Schedule_ALAP(last_time);

  vector < vector<Node*> > Visited_Sccs;
  vector<Node*> Current_nodes_in_path;

  for (int i = 0; i < (int) Sccs.size(); i++)
  {
    if ((int) Sccs[i].size() == 1)
      continue;

    Current_nodes_in_path = get_path_between(Visited_Sccs, Sccs[i]);
    for (int j = 0; j < (int) Visited_Sccs.size(); j++)
      Current_nodes_in_path = Subtract(Current_nodes_in_path, Visited_Sccs[j]);

    Visited_Sccs.push_back(Current_nodes_in_path);
    Current_nodes_in_path = order_nodes_SCC(Sccs[i]);
    for (int j = 0; j < (int) Visited_Sccs.size(); j++)
      Current_nodes_in_path = Subtract(Current_nodes_in_path, Visited_Sccs[j]);
    Visited_Sccs.push_back(Current_nodes_in_path);
  }
  vector<Node*> unordered_nodes = _node_Set;
  for (int j = 0; j < (int) Visited_Sccs.size(); j++)
    unordered_nodes = Subtract(unordered_nodes, Visited_Sccs[j]);

  Node* current;
  int max_time = -1;
  vector<Node*> rest;
  vector<Node*> temp;
  vector<Node*> backward_sweep_visited;
  vector<Node*> forward_sweep_visited;
  vector<Node*> backward_sweep_tovisit;
  vector<Node*> forward_sweep_tovisit;

  Node* backward_current;
  Node* forward_current;

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
  vector<Node*> retVal;

  for (int j = 0; j < (int) Visited_Sccs.size(); j++)
  {
    retVal = Union(retVal, Visited_Sccs[j]);
    rest = Subtract(rest, Visited_Sccs[j]);
  }
  retVal = Union(retVal, rest);

  //Wrongly initating some node. So reset it.
  for (int i = 0; i < (int) rest.size(); i++)
  {
    retVal[i]->get_Sched_Info()->reset_Current();
  }

  return retVal;
}

void DFG::Sort_Sccs(vector<vector<Node*> > &sets)
{
  vector<Node*> temp;
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
void DFG::Sort_Ops_by_Depth(vector<Node*> &set)
{
  Node* temp;
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

vector<Node*> DFG::get_path_between(vector<vector<Node*> > &visited, vector<Node*> &to_visit)
{
  vector<Node*> retval;
  vector<Node*> temp_set;
  vector<Node*> temp_set_2;
  vector<Node*> temp_set_3;
  vector<Node*> temp_set_4;

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
              retval.push_back(temp_set[k]);
          }
        }
      }
    }
  }
  return retval;
}

vector<Node*> DFG::get_all_successors(Node* &current)
{
  vector<Node*> temp_set;
  vector<Node*> temp_set_2;
  vector<Node*> visited;
  Node* temp_node;
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

vector<Node*> DFG::get_all_predecessors(Node* &current)
{
  vector<Node*> temp_set;
  vector<Node*> temp_set_2;
  vector<Node*> visited;
  Node* temp_node;
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

vector<Node*> DFG::order_nodes_SCC(vector<Node*> &scc)
{
  vector<Node*> retVal = scc;
  Node* temp_node;
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

string DFG::Generate_Latency()
{
  ofstream myfile;
  string filename = "node_latency.txt";
  myfile.open(filename.c_str());
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    myfile << _node_Set[i]->get_ID() << "\t" << _node_Set[i]->get_Latency() << "\n";
  }
  myfile.close();
  return filename;
}

void DFG::insert_loads_mem(Node* node1, Node* node2, vector<Node*> &inserted_node_ids)
{
  //Make sure node1 or node2 does not represent memory operation
  //TODO What if they are memory nodes
  Node* store_data=NULL;

  std::map<Node*,Node*>::iterator it;
  for(it=constrained_st_pred_pairs.begin(); it!=constrained_st_pred_pairs.end(); ++it)
  {
    if(it->second->get_ID() == node1->get_ID())
    {
      store_data = it->first;
      break;
    }
  }

  Node* load_address  = new Node((Instruction_Operation) ld_add_op,  load_add_latency,   Get_Unique_Index(),"");
  Node* load_data     = new Node((Instruction_Operation) ld_data_op, load_data_latency,  Get_Unique_Index(),"");

  //Set Load Store Characteristics
  load_address->set_Load_Address_Generator(load_data);
  load_data->set_Load_Data_Bus_Read(load_address);
  ++totalNonrecVars;

  //Insert All Nodes
  insert_Node(load_address);
  insert_Node(load_data);

  vector<Node*> load_node_vector;
  std::map<Node*, vector<Node*> >::iterator it1 = constrained_store_load_pairs.find(get_Node(store_data->get_ID()));
  if(it1 != constrained_store_load_pairs.end()) {
    load_node_vector = it1->second;
    constrained_store_load_pairs.erase(it1);
  }

  load_node_vector.push_back(load_address);
  constrained_store_load_pairs.insert(std::pair<Node*,vector<Node*> >(get_Node(store_data->get_ID()),load_node_vector));
  constrained_ld_succ_pairs.insert(std::pair<Node*,Node*>(get_Node(load_address->get_ID()),node2));

  ARC *arc;
  vector<Node*> nextNodes;
  vector<ARC*>::iterator iEdge;

  for (iEdge = _ARC_Set.begin(); iEdge != _ARC_Set.end(); iEdge++)
  {
    arc = *iEdge;
    //find the arc between node1 -> node2
    if (arc->get_From_Node()->get_ID() == node1->get_ID() && arc->get_To_Node()->get_ID() == node2->get_ID())
    {
      _ARC_Set.erase(iEdge, iEdge + 1);	      //erase it from arc set
      if(node1->remove_Succ_Arc(arc) == -1)
        debugfile << "Failed to remove Succ arc for node\n.";
      if(node2->remove_Pred_Arc(arc) == -1)
        debugfile << "Failed to remove Pred arc for node\n.";

      //form an arc between load nodes and node2
      make_Arc(load_address, load_data, ++ARC_Max_Index, 0, LoadDep, 0);
      make_Arc(load_data, node2, ++ARC_Max_Index, 0, TrueDep, arc->get_OperandOrder());
      break;
    }
  }

  inserted_node_ids.push_back(load_address);
  inserted_node_ids.push_back(load_data);
}

void DFG::insert_store_loads_in_between_input(Node* node1, Node* node2, vector<Node*> &inserted_node_ids)
{
  //Make sure node1 or node2 does not represent memory operation
  Node* store_address = new Node((Instruction_Operation) st_add_op,  store_add_latency,  Get_Unique_Index(),"");
  Node* store_data    = new Node((Instruction_Operation) st_data_op, store_data_latency, Get_Unique_Index(),"");
  Node* load_address  = new Node((Instruction_Operation) ld_add_op,  load_add_latency,   Get_Unique_Index(),"");
  Node* load_data     = new Node((Instruction_Operation) ld_data_op, load_data_latency,  Get_Unique_Index(),"");

  //Set Load Store Characteristics
  load_address->set_Load_Address_Generator(load_data);
  load_data->set_Load_Data_Bus_Read(load_address);
  ++totalNonrecVars;

  store_address->set_Store_Address_Generator(store_data);
  store_data->set_Store_Data_Bus_Write(store_address);
  ++totalNonrecVars;

  //Insert All Nodes
  insert_Node(load_address);
  insert_Node(load_data);
  insert_Node(store_address);
  insert_Node(store_data);

  vector<Node*> load_node_vector;
  std::map<Node*, vector<Node*> >::iterator it1 = constrained_store_load_pairs.find(get_Node(store_data->get_ID()));
  if(it1 != constrained_store_load_pairs.end()) {
    load_node_vector = it1->second;
    constrained_store_load_pairs.erase(it1);
  }
  load_node_vector.push_back(load_address);
  constrained_store_load_pairs.insert(std::pair<Node*,vector<Node*> >(get_Node(store_data->get_ID()),load_node_vector));
  constrained_ld_succ_pairs.insert(std::pair<Node*,Node*>(get_Node(load_address->get_ID()),node2));
  constrained_st_pred_pairs.insert(std::pair<Node*,Node*>(get_Node(store_data->get_ID()),node1));

  ARC *arc;
  vector<Node*> nextNodes;
  vector<ARC*>::iterator iEdge;

  for (iEdge = _ARC_Set.begin(); iEdge != _ARC_Set.end(); iEdge++)
  {
    arc = *iEdge;
    //find the arc between node1 -> node2
    if (arc->get_From_Node()->get_ID() == node1->get_ID() && arc->get_To_Node()->get_ID() == node2->get_ID())
    {
      _ARC_Set.erase(iEdge, iEdge + 1);	      //erase it from arc set
      if(node1->remove_Succ_Arc(arc) == -1)
        debugfile << "Failed to remove Succ arc for node\n.";
      if(node2->remove_Pred_Arc(arc) == -1)
        debugfile << "Failed to remove Pred arc for node\n.";

      //form an arc between node1 and store nodes
      make_Arc(store_address, store_data, ++ARC_Max_Index, 0, StoreDep, 0);
      make_Arc(node1, store_data, ++ARC_Max_Index, 0, TrueDep, 0);
      //form an arc between load nodes and node2
      make_Arc(load_address, load_data, ++ARC_Max_Index, 0, LoadDep, 0);
      make_Arc(load_data, node2, ++ARC_Max_Index, arc->get_Distance(), arc->get_Dependency(), arc->get_OperandOrder()); //Previously TRU, 0
      break;
    }
  }

  inserted_node_ids.push_back(load_address);
  inserted_node_ids.push_back(load_data);
  inserted_node_ids.push_back(store_address);
  inserted_node_ids.push_back(store_data);

  //return success
  return;
}

bool DFG::check_node_failure_due_to_large_sched_dist(int II, Node* leftnode, vector<int> &distance1, vector<int> &distance2, vector<Node*> &prev, vector<Node*> &next)
{
  bool dist1 = leftnode->get_dist_physical_predecessors_constrain_this_modulo(II,distance1,prev);
  bool dist2 = leftnode->get_dist_physical_successors_constrain_by_this_modulo(II,distance2,next);

  if((dist1 == true) || (dist2 == true))
    return true;
  else
    return false;
}

int DFG::Mem_Reschedule(int II, int number_of_resources, Node* node, Node* othernode, vector<Node*> &inserted_node_ids)
{
  int success = 0;

  if (node->get_physical_predecessors_constrain_this_modulo(II).size() > 0)
  {
    //Assuming effect for one pair. Reconsider based on failed from incompatibility.
    //Otherwise, need to do path sharing for multiple - both load and store.
    vector<Node*> prev = node->get_physical_predecessors_constrain_this_modulo(II);
    for (int j = 0; j < (int) prev.size(); j++) {
      if(othernode->get_ID() == prev[j]->get_ID())
        insert_store_loads_in_between_input(prev[j],node,inserted_node_ids);
    }
    success = 1;
  }
  else if(node->get_physical_successors_constrain_by_this_modulo(II).size() > 0)
  {
    vector<Node*> next = node->get_physical_successors_constrain_by_this_modulo(II);
    for (int j = 0; j < (int) next.size(); j++) {
      if(othernode->get_ID() == next[j]->get_ID())
        insert_store_loads_in_between_input(node,next[j],inserted_node_ids);
    }
    success = 1;
  }
  else
    FATAL(true,"Should not reach here. What is the reason?\n");

  if(node->get_ID() == othernode->get_ID())
  {
    node->delete_self_loop();
    othernode->delete_self_loop();
  }

  int retVal = Reschedule(II, number_of_resources, success, 3, {}, {});
  return retVal;
}

// this function tries to map the unmapped nodes, this may modify the DFG and attempt a scheduling
// It is (Register-Aware) Routing by PEs.
int DFG::Route_ReSchedule(int II, int number_of_resources, Node* node, Node* pred, Node* succ, int &inserted_node_id)
{
  int success = 0;

  vector<Node*> reschedule;
  vector<int> resched_time;

  if (succ->get_physical_predecessors_constrain_this_modulo(II).size() > 0)
  {
    vector<Node*> prev = succ->get_physical_predecessors_constrain_this_modulo(II);
    for (int j = 0; j < (int) prev.size(); j++)
    {
      //Add Routing Node Only Between pred->node
      if(prev[j]->get_ID() == pred->get_ID())
      {
        Node_Dummy* tempDummyNode = new Node_Dummy(1, Get_Unique_Index(), prev[j]);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
          Add_Arc( tempDummyNode->get_Pred_Arc(0));

        insert_Node(tempDummyNode);
        insert_Node_in_between_output(prev[j], succ, tempDummyNode);
        inserted_node_id = tempDummyNode->get_ID();

        debugfile << "1. Inserted DummyNode " << tempDummyNode->get_ID() << " between nodes " << prev[j]->get_ID() << " and " << succ->get_ID() << "\n";
      }
    }
    success = 1;
  }
  else if (pred->get_physical_successors_constrain_by_this_modulo(II).size() > 0)
  {
    vector<Node*> next = pred->get_physical_successors_constrain_by_this_modulo(II);
    for (int j = 0; j < (int) next.size(); j++)
    {
      //Add Routing Node Only Between node->succ
      if(next[j]->get_ID() == succ->get_ID())
      {
        Node_Dummy* tempDummyNode = new Node_Dummy(1, Get_Unique_Index(), node);
        if ((int) tempDummyNode->get_Number_of_Pred()>0)
          Add_Arc( tempDummyNode->get_Pred_Arc(0));

        insert_Node(tempDummyNode);
        insert_Node_in_between_output(pred, next[j], tempDummyNode);

        inserted_node_id = tempDummyNode->get_ID();
        debugfile << "2. Inserted DummyNode " << tempDummyNode->get_ID() << " between nodes " << pred->get_ID() << " and " << next[j]->get_ID() << "\n";
      }
    }
    success = 1;
  }
  else if ((node->get_Sched_Info()->get_Feasible_ALAP() - node->get_Sched_Info()->get_Feasible_ASAP()) > 0)
  {
    debugfile << "resched back " << node->get_ID() << endl;
    reschedule.push_back(node);
    resched_time.push_back(node->get_Sched_Info()->get_Feasible_ALAP() - 1);
    inserted_node_id = node->get_ID();
    success = 1;
  }
  else
    FATAL(true,"Should not reach here. What is the reason?\n");

  if(pred->get_ID() == succ->get_ID())
  {
    pred->delete_self_loop();
    succ->delete_self_loop();
  }

  int retVal = Reschedule(II, number_of_resources, success,2, reschedule, resched_time);
  return retVal;
}

// Depending on the routing mode, reschedule the modified graph.
int DFG::Reschedule(int II, int number_of_resources, int success, int mode, vector<Node*> reschedule, vector<int> resched_time)
{
  bool redo = false;
  int last_time;
  //Make sure there are enough resources left are rescheduling
  if(get_ResMII(number_of_resources) > II)
    return 0;

  if(get_MemMII() > II)
    return 0;

  vector<Node*> set = _node_Set;

  if (success == 1)
  {
    debugfile << "in reschedule. success = 1\n";
    Reset_Current_Schedule();
    Reset_ALAP_Feasible_Schedule();
    Reset_ASAP_Feasible_Schedule();
    last_time = Schedule_ASAP_Feasible(number_of_resources, II);

    if(last_time == 0)
      return 0;

    if (!Schedule_ALAP_Feasible(last_time, number_of_resources, II))
    return 0;

    if(mode == 1) {
      for (std::map<int,int>::iterator it=route_rf_nodeid_scheduled_time.begin(); it!=route_rf_nodeid_scheduled_time.end(); ++it) {
        int node_id = it->first;
        int sched_time = it->second;

        if(get_Node(node_id)->get_Sched_Info()->get_Feasible_ALAP() != sched_time) {
          get_Node(node_id)->get_Sched_Info()->set_Feasible_ALAP(sched_time);
          get_Node(node_id)->get_Sched_Info()->set_Current(sched_time, II);
          get_Node(node_id)->get_Sched_Info()->set_Feasible_ASAP(sched_time - II + 1);
        }
      }
    }

    MAX_SCHEDULE_LEN = last_time;
    int alap_time;
    debugfile << "II in ASAP Feasible is " << II << "\tLast Time is " << last_time << "\tResources are " << number_of_resources << endl;

    while (true)
    {
      for (int i = 0; i < (int) reschedule.size(); i++)
      {
        alap_time = resched_time[i];
        if (reschedule[i]->get_Sched_Info()->get_Feasible_ALAP() < alap_time)
          alap_time = reschedule[i]->get_Sched_Info()->get_Feasible_ALAP();
        reschedule[i]->get_Sched_Info()->set_Feasible_ALAP(alap_time);
      }

      if (Modulo_Schedule(II, last_time, redo, number_of_resources))
      {
        debugfile << "Feasible II is " << II << endl;
        return 1;
      }
      return 0;
    }
  }
  return 0;
}

DFG* DFG::Copy_With_Schedule_And_Mapping()
{
  //copy DDG
  DFG* retVal;
  retVal = new DFG(CGRA_X_Dim, CGRA_Y_Dim);
  Node* tempNode;

  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    if (_node_Set[i]->get_Instruction() == (Instruction_Operation) route_op)
      tempNode = new Node(_node_Set[i]->get_Instruction(), _node_Set[i]->get_Latency(), _node_Set[i]->get_ID(), _node_Set[i]->getName());
    else
      tempNode = new Node(_node_Set[i]->get_Instruction(), _node_Set[i]->get_Latency(), _node_Set[i]->get_ID(), _node_Set[i]->getName());

    tempNode->hasLargeConst = _node_Set[i]->hasLargeConst;
    //Copy other schedule and map related properties of a node
    tempNode->get_Sched_Info()->set_ASAP(_node_Set[i]->get_Sched_Info()->get_ASAP());
    tempNode->get_Sched_Info()->set_ALAP(_node_Set[i]->get_Sched_Info()->get_ALAP());
    tempNode->get_Sched_Info()->set_Feasible_ASAP(_node_Set[i]->get_Sched_Info()->get_Feasible_ASAP());
    tempNode->get_Sched_Info()->set_Feasible_ALAP(_node_Set[i]->get_Sched_Info()->get_Feasible_ALAP());
    tempNode->get_Sched_Info()->set_Height(_node_Set[i]->get_Sched_Info()->get_Height());
    tempNode->get_Sched_Info()->set_Current(_node_Set[i]->get_Sched_Info()->get_Current(), _node_Set[i]->get_Sched_Info()->get_II());
    tempNode->get_Sched_Info()->set_SCC_Delay(_node_Set[i]->get_Sched_Info()->get_SCC_Delay());
    tempNode->get_Sched_Info()->set_slack(_node_Set[i]->get_Sched_Info()->get_slack());

    tempNode->Set_Index_Cycle(_node_Set[i]->get_Index_Cycle());
    tempNode->Set_Low_Index_Cycle(_node_Set[i]->get_Low_Index_Cycle());

    if(_node_Set[i]->isMapped()) tempNode->Assign2PE(_node_Set[i]->getPE());
    retVal->insert_Node(tempNode);
  }

  for (int i = 0; i < (int) _constants.size(); i++)
  {
    tempNode = new Node(_constants[i]->get_Instruction(), _constants[i]->get_Latency(), _constants[i]->get_ID(), _constants[i]->getName());
    retVal->insert_Constant(tempNode);
  }

  for (int i = 0; i < (int) _ARC_Set.size(); i++)
    retVal->make_Arc(retVal->get_Node(_ARC_Set[i]->get_From_Node()->get_ID()), retVal->get_Node(_ARC_Set[i]->get_To_Node()->get_ID()), _ARC_Set[i]->get_Number(), _ARC_Set[i]->get_Distance(), _ARC_Set[i]->get_Dependency(),_ARC_Set[i]->get_OperandOrder());

  //special nodes
  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    //is it a load node?
    if (_node_Set[i]->is_Load_Address_Generator())
      retVal->get_Node(_node_Set[i]->get_ID())->set_Load_Address_Generator(retVal->get_Node(_node_Set[i]->get_Related_Node()->get_ID()));
    else if (_node_Set[i]->is_Load_Data_Bus_Read())
      retVal->get_Node(_node_Set[i]->get_ID())->set_Load_Data_Bus_Read(retVal->get_Node(_node_Set[i]->get_Related_Node()->get_ID()));
    //it is a store node?
    else if (_node_Set[i]->is_Store_Address_Generator())
      retVal->get_Node(_node_Set[i]->get_ID())->set_Store_Address_Generator(retVal->get_Node(_node_Set[i]->get_Related_Node()->get_ID()));
    else if (_node_Set[i]->is_Store_Data_Bus_Write())
      retVal->get_Node(_node_Set[i]->get_ID())->set_Store_Data_Bus_Write(retVal->get_Node(_node_Set[i]->get_Related_Node()->get_ID()));
  }
  retVal->Node_Max_Index  = Node_Max_Index;
  retVal->ARC_Max_Index   = ARC_Max_Index;
  retVal->Copy_Resource_Info(Total_Resources, Address_BUS, Data_BUS,MAX_SCHEDULE_LEN);

  return retVal;
}

int DFG::get_ResMII(int number_of_resources)
{
  int resMII = (_node_Set.size() - 1) / number_of_resources;
  resMII++; //This is equivalent to ceiling operation
  return resMII;
}

int DFG::get_MemMII()
{
  int MemMII = (Count_Load_Operations()  + Count_Store_Operations() - 1) / CGRA_Y_Dim;
  MemMII++;
  return MemMII;
}

bool DFG::get_Mem_Mode()
{
  return mem_mode;
}

void DFG::set_Mem_Mode(bool mode)
{
  mem_mode = mode;
}

vector<Node*> DFG::sort_rest_asap_nodes_for_memory(vector<Node*> nodeset)
{
  vector<Node*> regular_priority;
  vector<Node*> regular_rest;
  vector<Node*> store_priority;
  vector<Node*> store_rest;
  vector<Node*> load;
  vector<Node*> sorted;

  vector<Node*>::iterator iNode;
  for (iNode = nodeset.begin(); iNode != nodeset.end(); iNode++)
  {
    if ((*iNode)->is_Load_Address_Generator() || (*iNode)->is_Load_Data_Bus_Read())
      load.push_back(*iNode);
    else if ((*iNode)->is_Store_Address_Generator() || (*iNode)->is_Store_Data_Bus_Write())
    {
      //Assumption: Store can be always delayed except in case of load-after-store for a recurrence
      //Otherwise, in DDG generation, compiler will ensure that value is transformed into recurrence,
      //So DDG formation should estimate the behavior and ensure functionality (i.e. timing)
      store_priority.push_back(*iNode);
    }
    else
      regular_priority.push_back(*iNode);
  }

  /* include individual stores only if it is on critical path i.e. tight store needs to be made
     In routing, it should be taken care that if there are multiple successors
     then direct those with less height through a new routing node.
     Same happens for separate recomputation.
     */
  sorted = Union(store_priority, regular_priority);
  sorted = Union(sorted, load);
  return sorted;
}

void DFG::Delete_Mem_Nodes(vector<Node*> nodeset)
{
  vector<Node*>::iterator iNode;
  for (iNode = nodeset.begin(); iNode != nodeset.end(); iNode++)
  {
    std::map<Node*, vector<Node*> >::iterator it1;

    //Check And Delete Store Nodes
    it1 = constrained_store_load_pairs.find(get_Node((*iNode)->get_ID()));
    if(it1 != constrained_store_load_pairs.end()) {
      constrained_store_load_pairs.erase(it1);
    }
    else
    {
      //Check And Delete Load Nodes Pair
      bool found = false;
      for (it1 = constrained_store_load_pairs.begin(); it1 != constrained_store_load_pairs.end(); it1++)
      {
        vector<Node*> load_nodes = it1->second;
        int store_data_id = it1->first->get_ID();
        std::vector<Node*>::iterator it2;
        for(it2 = load_nodes.begin(); it2 != load_nodes.end(); it2++) {
          if(((*it2)->get_ID()) == ((*iNode)->get_ID())) {
            load_nodes.erase(it2);
            constrained_store_load_pairs.erase(it1);
            constrained_store_load_pairs.insert(std::pair<Node*,vector<Node*> >(get_Node(store_data_id),load_nodes));
            found = true;
            break;
          }
        }
        if(found)
          break;
      }
    }

    std::map<Node*,Node*>::iterator it3 = constrained_ld_succ_pairs.find(get_Node((*iNode)->get_ID()));
    if(it3 != constrained_ld_succ_pairs.end()) {
      constrained_ld_succ_pairs.erase(it3);
    }

    std::map<Node*,Node*>::iterator it4 = constrained_st_pred_pairs.find(get_Node((*iNode)->get_ID()));
    if(it4 != constrained_st_pred_pairs.end()) {
      constrained_st_pred_pairs.erase(it4);
    }
  }
}

int DFG::check_reschedule_of_mem_nodes(int pred_id, vector<Node*> leftnotmapped, int number_of_resources, int II, bool &changed, Node* &nodeToBeMapped, vector<Node*> &inserted_route_nodes)
{
  vector<Node*> nextnodes = get_Node(pred_id)->Get_Next_Nodes();
  vector<Node*> next_immediates;
  vector<Node*> inserted_mem_nodes_immediates;
  changed = false;
  int success = 0;
  nodeToBeMapped=NULL;

  for(std::vector<Node*>::iterator it = nextnodes.begin(); it != nextnodes.end(); ++it) {
    if((*it)->is_Store_Address_Generator() || (*it)->is_Store_Data_Bus_Write()) {

      //can delay the store nodes, make sure it works right.
      //Similarly, in routing mode make sure that it works well, if multiple successors are not scheduled.
      //Then just route the value. Recomputation in its mode can check whether it needs to duplicate.
      //Some node height based mechanism can help.

      //If Store Nodes Are Mapped Then Ignore
      if(std::find(leftnotmapped.begin(), leftnotmapped.end(), (*it)) == leftnotmapped.end())
        continue;

      //TODO: Generate error in case of store scheduled immediately after the value.
      //TODO: Route adress value to store address, in case of failure.

      //Check if there is corresponding load, otherwise skip
      std::map<Node*, vector<Node*> >::iterator it1 = constrained_store_load_pairs.find((*it));
      if(it1 != constrained_store_load_pairs.end()) {
        changed = true;
        vector<Node*> load_nodes = it1->second;
        std::vector<Node*>::iterator it2;
        int nearest_load_sched_time = 10000;
        for(it2 = load_nodes.begin(); it2 != load_nodes.end(); it2++)
        {
          if((*it2)->get_Sched_Info()->get_Current() < nearest_load_sched_time)
            nearest_load_sched_time = (*it2)->get_Sched_Info()->get_Current();
        }

        nodeToBeMapped = get_Node((*it)->get_ID()); //store data node
        Node* output_data=get_Node(pred_id);
        std::map<Node*,Node*>::iterator it_output_data = constrained_st_pred_pairs.find((*it));
        if(it_output_data != constrained_st_pred_pairs.end())
          output_data = it_output_data->second;
        int output_data_time = get_Node(output_data->get_ID())->get_Sched_Info()->get_Current();
        int current_time = output_data_time + output_data->get_Latency();
        int st_data_time = get_Node((*it)->get_ID())->get_Sched_Info()->get_Current();

        if(st_data_time == output_data_time)
          FATAL(true,"Shoud not reach here. Debug!");

        int possible_delay_to_store_later = nearest_load_sched_time - current_time;

        //Determine which immediate nodes can be rescheduled
        vector<Node*> succ = output_data->Get_Next_Nodes();
        vector<Node*>::iterator it3;
        vector<Node*> remaining_succ;

        //TODO Try to find a routing node and see whether data can be forwarded by delaying store (
        //Should happen while generating store)

        // Lack of routing node implies that data cannot be routed to st_data
        // and DFG should be modified in some manner.
        for(it3 = succ.begin(); it3 != succ.end(); it3++)
        {
          // if node has another input other than from the prior data
          // (which also needs to be stored), and
          // if are scheduled already earlier, can't change them.

          //erase st_data node
          if((*it3)->get_ID() == (*it)->get_ID())
            continue;

          if(((*it3)->get_Sched_Info()->get_Current() < st_data_time) && ((*it3)->get_Number_of_Succ() > 1))
            continue;

          remaining_succ.push_back((*it3));
        }

        //sort the rest of the node
        if(remaining_succ.size() > 1)
        {
          for(it3 = remaining_succ.begin(); it3 != remaining_succ.end()-1; it3++)
          {
            for(vector<Node*>::iterator it4 = it3+1; it4 != remaining_succ.end(); it4++)
            {
              if((*it3)->get_Sched_Info()->get_Current() > (*it4)->get_Sched_Info()->get_Current())
                iter_swap(it3,it4);
            }
          }
        }

        int current_total_resources;
        int current_address_bus_resources;
        int current_data_bus_resources;

        //Update resources information in DFG by removing schedule of remaining_succ
        for(unsigned int ii=0; ii < remaining_succ.size(); ii++)
        {
          int temp_time = remaining_succ[ii]->get_Sched_Info()->get_Current();
          current_total_resources = get_total_resources(temp_time);
          set_total_resources(temp_time,current_total_resources-1);

          if(remaining_succ[ii]->is_Address_Assert())
          {
            current_address_bus_resources = get_address_bus_resources(temp_time);
            set_address_bus_resources(temp_time,current_address_bus_resources-1);
          }

          if(remaining_succ[ii]->is_Store_Data_Bus_Write() || remaining_succ[ii]->is_Load_Data_Bus_Read())
          {
            current_data_bus_resources = get_data_bus_resources(temp_time);
            set_data_bus_resources(temp_time,current_data_bus_resources-1);
          }
        }

        //Unschedule current store node and related address/data node
        set_total_resources(st_data_time,get_total_resources(st_data_time)-2);
        set_address_bus_resources(st_data_time,get_address_bus_resources(st_data_time)-1);
        set_data_bus_resources(st_data_time,get_data_bus_resources(st_data_time)-1);

        bool rescheduled = false;
        Node* current_data_node = output_data;

        while(possible_delay_to_store_later > 0) //Store Must Occur Before Load
        {
          //Check for resources at mapping time and modify DFG
          current_total_resources = get_total_resources(current_time);
          current_address_bus_resources = get_address_bus_resources(current_time);
          current_data_bus_resources = get_data_bus_resources(current_time);
          int available_resources = number_of_resources - current_total_resources;
          if(available_resources <= 0)
          FATAL(true,"Resources unavailable. Debug Rescheduling!");
          //This will add only 1 routing node and few successors.
          //Once all successors are scheduled, store nodes will be scheduled.
          if(remaining_succ.size() > 0)
          {
            switch (available_resources)
            {
              case 1:
              { //only routing node
                Node_Dummy* routeNode = new Node_Dummy(1, Get_Unique_Index(), current_data_node);
                if ((int) routeNode->get_Number_of_Pred()>0)
                  Add_Arc(routeNode->get_Pred_Arc(0));

                insert_Node(routeNode);

                for(unsigned int ii=0; ii < remaining_succ.size(); ii++)
                {
                  insert_Node_in_between_output(current_data_node, remaining_succ[ii], routeNode);
                  debugfile << ii << "\tInserted Routing Node " << routeNode->get_ID() << " between nodes " << current_data_node->get_ID() << " and " << remaining_succ[ii]->get_ID() << "\n";
                }
                //Change the output data to store data node
                insert_Node_in_between_output(current_data_node, get_Node((*it)->get_ID()), routeNode);
                current_data_node = routeNode;
                inserted_route_nodes.push_back(routeNode);
                set_total_resources(current_time,current_total_resources+1);
                rescheduled = true;
                break;
              }
              case 2:
              { //schedule a successor and a routing node
                remaining_succ.erase(remaining_succ.begin());
                Node_Dummy* routeNode = new Node_Dummy(1, Get_Unique_Index(), current_data_node);
                if ((int) routeNode->get_Number_of_Pred()>0)
                  Add_Arc(routeNode->get_Pred_Arc(0));

                insert_Node(routeNode);

                for(unsigned int ii=0; ii < remaining_succ.size(); ii++) {
                  insert_Node_in_between_output(current_data_node, remaining_succ[ii], routeNode);
                  debugfile << ii << "\tInserted Routing Node " << routeNode->get_ID() << " between nodes " << current_data_node->get_ID() << " and " << remaining_succ[ii]->get_ID() << "\n";
                }
                //Change the output data to store data node
                insert_Node_in_between_output(current_data_node, get_Node((*it)->get_ID()), routeNode);
                current_data_node = routeNode;
                inserted_route_nodes.push_back(routeNode);
                set_total_resources(current_time,current_total_resources+2);
                rescheduled = true;
                break;
              }
              case 3 ... 1000:
              { //schedule two successors and a routing node
                unsigned int succ_to_be_scheduled = 1;
                if(remaining_succ.size() >= 2)
                  succ_to_be_scheduled = 2;

                for(unsigned int ii=0; ii < succ_to_be_scheduled; ii++)
                  remaining_succ.erase(remaining_succ.begin());

                Node_Dummy* routeNode = new Node_Dummy(1, Get_Unique_Index(), current_data_node);
                if ((int) routeNode->get_Number_of_Pred()>0)
                  Add_Arc(routeNode->get_Pred_Arc(0));

                insert_Node(routeNode);
                for(unsigned int ii=0; ii < remaining_succ.size(); ii++) {
                  insert_Node_in_between_output(current_data_node, remaining_succ[ii], routeNode);
                  debugfile << ii << "\tInserted Routing Node " << routeNode->get_ID() << " between nodes " << current_data_node->get_ID() << " and " << remaining_succ[ii]->get_ID() << "\n";
                }
                //Change the output data to store data node
                insert_Node_in_between_output(current_data_node, get_Node((*it)->get_ID()), routeNode);
                current_data_node = routeNode;
                inserted_route_nodes.push_back(routeNode);
                set_total_resources(current_time,current_total_resources+1+succ_to_be_scheduled);
                rescheduled = true;
                break;
              }
              default: FATAL(true,"You should not reach here. Denug!");
                break;
            }
          }

          if(remaining_succ.size() > 0)
          {
            debugfile << remaining_succ.size() << " successors remaining. Delay for store is " << possible_delay_to_store_later << "\ttime: " << current_time << "\tresources: " << current_total_resources << endl;
            possible_delay_to_store_later--;
            current_time++;
            rescheduled = false;
            continue;
          }

          if(HAS_ENOUGH_RESOURCES_FOR_STORE_INSTRUCTION_AT_CYCLE(current_time, Address_BUS, Data_BUS, Total_Resources, number_of_resources))
          {
            rescheduled = true;
            debugfile << "Rescheduled store operation at time:" << current_time << endl;
            set_total_resources(current_time,get_total_resources(current_time)+2);
            set_address_bus_resources(current_time,get_address_bus_resources(current_time)+1);
            set_data_bus_resources(current_time,get_data_bus_resources(current_time)+1);
            break;
          }
          else
          {
            if(!rescheduled)
            {
              //insert a routing node
              Node_Dummy* routeNode = new Node_Dummy(1, Get_Unique_Index(), current_data_node);
              if ((int) routeNode->get_Number_of_Pred()>0)
                Add_Arc(routeNode->get_Pred_Arc(0));

              insert_Node(routeNode);
              insert_Node_in_between_output(current_data_node, get_Node((*it)->get_ID()), routeNode);
              debugfile << "Inserted Routing Node " << routeNode->get_ID() << " between nodes " << current_data_node->get_ID() << " and " << (*it)->get_ID() << "\n";
              current_data_node = routeNode;
              inserted_route_nodes.push_back(routeNode);
              if(get_total_resources(current_time) < number_of_resources)
                set_total_resources(current_time,get_total_resources(current_time)+1);
              else
                FATAL(true,"Resources occupied. Debug Rescheduling.");
            }

            possible_delay_to_store_later--;
            current_time++;
            rescheduled = false;
            continue;
          }
        }

        //Otherwise Need To Increase II
        if(!rescheduled)
        return 0;
      }
    }
  }

  success = Reschedule(II,number_of_resources,1,3,{},{});
  return success;
}

void DFG::loadAllLargeConstants(unsigned int rfSize)
{
  string constName = "";
  //Iterate through all large constants
  for(unsigned int i=0; i < _node_Set.size(); i++)
    if(_node_Set[i]->getLoadConstant())
    {
      //Remove Constant Node from DFG and Insert Load Nodes
      //First find out the successor node of the large constant
      vector<std::tuple<int,int,int> >::iterator it;
      for(it = _Const_ARC_Set.begin(); it != _Const_ARC_Set.end(); it++)
        if(std::get<1>(*it) == _node_Set[i]->get_ID()) {
          debugfile << "inserting loads for largeConstants:\t" << std::get<0>(*it) << "\t" << std::get<1>(*it) << "\t" << std::get<2>(*it) << endl;

          Node* load_address  = new Node((Instruction_Operation) ld_add_op,  load_add_latency,   Get_Unique_Index(),"ld_add_"+constName);
          Node* load_data     = new Node((Instruction_Operation) ld_data_op, load_data_latency,  Get_Unique_Index(),"ld_data_"+constName);

          //Set Load Store Characteristics
          load_address->set_Load_Address_Generator(load_data);
          load_data->set_Load_Data_Bus_Read(load_address);

          //Insert All Nodes
          insert_Node(load_address);
          insert_Node(load_data);

          //form an arc between load nodes and node2
          make_Arc(load_address, load_data, ++ARC_Max_Index, 0, LoadDep, 0);
          make_Arc(load_data, _node_Set[i], ++ARC_Max_Index, 0, TrueDep, std::get<2>(*it));

          _node_Set[i]->hasLargeConst--;

          if(_node_Set[i]->hasLargeConst <= rfSize) {
            _node_Set[i]->setLoadConstant(false);
            break;
          }
        }
        else if(std::get<0>(*it) == _node_Set[i]->get_ID())
        {
          debugfile << "inserting store for liveOuts:\t" << std::get<0>(*it) << "\t" << std::get<1>(*it) << "\t" << std::get<2>(*it) << endl;

          Node* store_address  = new Node((Instruction_Operation) st_add_op,  store_add_latency,   Get_Unique_Index(),"st_add_"+constName);
          Node* store_data     = new Node((Instruction_Operation) st_data_op, store_data_latency,  Get_Unique_Index(),"st_data_"+constName);

          //Set Load Store Characteristics
          store_address->set_Store_Address_Generator(store_data);
          store_data->set_Store_Data_Bus_Write(store_address);

          //Insert All Nodes
          insert_Node(store_address);
          insert_Node(store_data);

          //form an arc between node2 and store nodes
          make_Arc(store_address, store_data, ++ARC_Max_Index, 0, StoreDep, 0);
          make_Arc(_node_Set[i], store_data, ++ARC_Max_Index, 0, TrueDep, std::get<2>(*it));

          _node_Set[i]->hasLargeConst--;

          if(_node_Set[i]->hasLargeConst <= rfSize) {
            _node_Set[i]->setLoadConstant(false);
            break;
          }
        }
    }
}

bool DFG::getLoadConstants()
{
  return needToLoadConstants;
}

void DFG::setLoadConstants(bool val)
{
  needToLoadConstants = val;
}

void DFG::Copy_Resource_Info(int* totalResources, int* addBus, int* dataBus, int maxSchedLen)
{
  Total_Resources = new int[MAPPING_POLICY.MAX_LATENCY];
  Data_BUS        = new int[MAPPING_POLICY.MAX_LATENCY];
  Address_BUS     = new int[MAPPING_POLICY.MAX_LATENCY];

  for(int i=0; i < maxSchedLen; i++)
  {
    Total_Resources[i] = totalResources[i];
    Data_BUS [i]       = dataBus[i];
    Address_BUS[i]     = addBus[i];
  }
}

// Collect Register requirements for live variables
void DFG::collectRegReq(unsigned int rfSize)
{
  string constName = "";
  for(unsigned int i=0; i < _constants.size(); i++)
  {
    if(_constants[i]->isLargeConst(constName)) {
      vector<std::tuple<int,int,int> >::iterator it;
      for(it = _Const_ARC_Set.begin(); it != _Const_ARC_Set.end(); it++)
      if(std::get<0>(*it) == _constants[i]->get_ID())
      get_Node(std::get<1>(*it))->hasLargeConst++;
      /* Register reservation for live data management is not explicitly expressed in mapping.
        TODO: Currently, we only reserve register for store data.
        Assuming that a PE would have a register free to store the address
        for the store operation (don't know PE before mapping)
        */
      else if(std::get<1>(*it) == _constants[i]->get_ID())
        get_Node(std::get<0>(*it))->hasLargeConst++;
    }
    else {  //Remove constant from ConstArcSet
      vector<std::tuple<int,int,int> >::iterator it;
      unsigned int n = _Const_ARC_Set.size();
      for(unsigned int j = 0; j < n; j++)
      if(std::get<0>(*(_Const_ARC_Set.begin()+j)) == _constants[i]->get_ID()) {
        _Const_ARC_Set.erase(_Const_ARC_Set.begin()+j);
        n--;
      }
    }
  }

  /* TODO: Separate management of register allocation for live-out
  Currently, accomodating as a live-in constraint */
  for(unsigned int i=0; i < _node_Set.size(); i++)
  {
    _node_Set[i]->setLoadConstant(false);
    if(_node_Set[i]->hasLargeConst > rfSize)
    _node_Set[i]->setLoadConstant(true);
  }
}
