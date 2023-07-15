/*
 * DFG.cpp
 *
 *  Created on: Mar 8, 2013
 *  Last Modified: May 12, 2017
 *  Author: Shail Dave
 */

#include "DFG.h"
#include "llvm/Support/raw_ostream.h"

#define debug 0

using namespace llvm;

/*************************************ARC*************************************/
ARC::ARC(int ID, int Inter_Iteration_Distance,int operandOrder)
{
  this->ID = ID;
  this->Inter_Iteration_Distance = Inter_Iteration_Distance;
  this->operandOrder = operandOrder;
}

ARC::~ARC()
{

}
void ARC::Set_From_Node(NODE* from)
{
  this->from = from;
}

// Set the successor (sink) node for the edge
void ARC::Set_To_Node(NODE* to)
{
  this->to = to;
}

//Set dependency type
void ARC::Set_Dependency_Type(DataDepType dep)
{
  this->dependency = dep;
}

//Get dependency type
DataDepType ARC::Get_Dependency_Type()
{
  return dependency;
}

// Get the source node of this edge
NODE* ARC::get_From_Node()
{
  return from;
}

// Get the successor (sink) node of this edge
NODE* ARC::get_To_Node()
{
  return to;
}

// Return Edge ID
int ARC::get_ID()
{
  return ID;
}

//Return edge distance
int ARC::Get_Inter_Iteration_Distance()
{
  return Inter_Iteration_Distance;
}

void ARC::Set_Inter_Iteration_Distance(int inter)
{
  Inter_Iteration_Distance = inter;
}

// Return operandOrder
int ARC::GetOperandOrder()
{
  return operandOrder;
}

void ARC::SetOperandOrder(int i)
{
  operandOrder = i;
}


/************************************NODE************************************/
NODE::NODE(Instruction_Operation ins, int laten, int id, std::string name, Value* Node_Ins)
{
  Operation = ins;
  cuid = id;
  this->name = name;
  if (name == "")
  {
    std::ostringstream os;
    os << id;
    this->name = os.str();
  }
  latency = laten;
  this->Node_Ins = Node_Ins;
  related = NULL;

  loop = NULL;
  self_loop = false;

  load_alignment = 0;
}

void NODE::set_self_loop(ARC* s_loop)
{
  self_loop = true;
  loop = s_loop;
}

bool NODE::has_self_loop()
{
  return self_loop;
}

NODE::~NODE()
{
  inCommingArcs.clear();
  outGoingArcs.clear();
}

void NODE::set_Instruction(Instruction_Operation ins, int laten, int id, std::string name, Value* Node_Ins)
{
  Operation = ins;
  this->name = name;
  cuid = id;
  latency = laten;
  this->Node_Ins = Node_Ins;
}

int NODE::get_ID()
{
  return cuid;
}

int NODE::get_Latency()
{
  return latency;
}

std::string NODE::get_Name()
{
  return name;
}

void NODE::set_Name(std::string new_name){
  name = new_name;
}

Value* NODE::get_LLVM_Instruction()
{
  return Node_Ins;
}

void NODE::set_Latency(int laten)
{
  latency = laten;
}

Instruction_Operation NODE::get_Instruction()
{
  return Operation;
}

unsigned NODE::getAlignment()
{
  return load_alignment;
}


void NODE::setDatatype( Datatype dt)
{
  ins_datatype = dt;  
}

Datatype NODE::getDatatype()
{
  return ins_datatype; 
}

void NODE::set_Load_Address_Generator(NODE* next)
{
  related = next;
}

void NODE::setAlignment(unsigned alignment)
{
  load_alignment = alignment;
}

void NODE::set_Load_Data_Bus_Read(NODE* next)
{

  related = next;
}

void NODE::set_Store_Address_Generator(NODE* next)
{
  related = next;
}

void NODE::set_Store_Data_Bus_Write(NODE* next)
{
  related = next;
}

bool NODE::is_Load_Address_Generator()
{
  return (Operation == ld_add);

}

bool NODE::is_Load_Data_Bus_Read()
{
  return (Operation == ld_data);

}

bool NODE::is_Store_Address_Generator()
{
  return (Operation == st_add);
}

bool NODE::is_Store_Data_Bus_Write()
{
  return (Operation == st_data);

}

bool NODE::is_Mem_Operation()
{
  return ((Operation == ld_add) || (Operation == ld_data) || (Operation == st_add) || (Operation == st_data));
}

bool NODE::is_Add_Operation(){
  return ((Operation == add));
}

bool NODE::is_Mult_Operation(){
  return ((Operation == mult));
}

bool NODE::is_Comp_Operation(){
  return ((Operation > 8 && Operation < 19));
}

bool NODE::is_Sub_Operation(){
  return ((Operation == sub));
}

bool NODE::is_Div_Operation(){
  return ((Operation == division));
}

bool NODE::is_Binary_Operation(){
  return ((Operation > 3 && Operation < 9) || Operation == 34);
}

bool NODE::is_Routing_Operation()
{
  return ((Operation == route));
}

bool NODE::is_Phi_Operation()
{
  return ((Operation == cgra_select));
}

bool NODE::is_ConditionalSelect_Operation()
{
  return ((Operation == cond_select));
}

std::string NODE::Op_To_String(){
  switch(Operation){
  case add: return "Add";
  case sub: return "Subtract";
  case mult: return "Multiply";
  case division: return "Division";
  case shiftl: return "Shiftl";
  case shiftr: return "Shiftr";
  case andop: return "AND";
  case orop: return "OR";
  case xorop: return "XOR";
  case cmpSGT: return "CMPSGT";
  case cmpEQ: return "CMPEQ";
  case cmpNEQ: return "CMPNEQ";
  case cmpSLT: return "CMPSLT";
  case cmpSLEQ: return "CMPSLEQ";
  case cmpSGEQ:	return "CMPSGEQ";
  case cmpUGT:	return "CMPUGT";
  case cmpULT:	return "CMPULT";
  case cmpULEQ:	return "CMPULEQ";
  case cmpUGEQ:	return "CMPUGEQ";
  case ld_add:	return "LD_ADDRESS";
  case ld_data:	return "LD_DATA";
  case st_add:	return "ST_ADDRESS";
  case st_data:	return "ST_DATA";
  case ld_add_cond:	return "LD_ADDRESS_COND";
  case ld_data_cond:	return "LD_DATA_COND";
  case loopctrl:	return "LOOPCTRL";
  case cond_select:	return "COND_SELECT";
  case cgra_select:	return "PHI";
  case rem: return "REM";
  case sext: return "SIGNEXT";
  default: return "";
  }
}

NODE* NODE::get_Related_Node()
{
  return related;
}

// Get the number of incomming edges
int NODE::get_Number_of_Pred()
{
  return (int) inCommingArcs.size();
}

// Get the number of outgoing edges
int NODE::get_Number_of_Succ()
{
  return (int) outGoingArcs.size();
}

//return the set of incomming edges
std::vector<NODE*> NODE::Get_Prev_Nodes()
{
  std::vector<NODE*> retVal;
  int SizeOfIncomming = inCommingArcs.size();
  for (int i = 0; i < SizeOfIncomming; i++)
  {
    retVal.push_back(inCommingArcs[i]->get_From_Node());
  }
  return retVal;
}

//return the set of outgoing edges
std::vector<NODE*> NODE::Get_Next_Nodes()
{
  std::vector<NODE*> retVal;
  int SizeOfOutGoing = outGoingArcs.size();
  for (int i = 0; i < SizeOfOutGoing; i++)
  {
    retVal.push_back(outGoingArcs[i]->get_To_Node());
  }
  return retVal;
}

// Add a new incomming edge
void NODE::add_Pred_Arc(ARC* pred_Arc)
{
  inCommingArcs.push_back(pred_Arc);
}

// Add a new outgoing edge
void NODE::add_Succ_Arc(ARC* succ_Arc)
{
  outGoingArcs.push_back(succ_Arc);
}

// Remove an incomming edge
int NODE::remove_Pred_Arc(ARC* predArc)
{
  std::vector<ARC*>::iterator iEdge;

  for (iEdge = inCommingArcs.begin(); iEdge != inCommingArcs.end(); iEdge++)
  {
    if (*iEdge == predArc)
    {
      inCommingArcs.erase(iEdge, iEdge + 1);
      return 0;
    }
  }

  return -1;
}

//remove an outgoing edge
int NODE::remove_Succ_Arc(ARC* succArc)
{
  std::vector<ARC*>::iterator iEdge;

  for (iEdge = outGoingArcs.begin(); iEdge != outGoingArcs.end(); iEdge++)
  {
    if (*iEdge == succArc)
    {
      outGoingArcs.erase(iEdge, iEdge + 1);
      return 0;
    }
  }

  return -1;
}

//returns true if there is an edge between this node and next node
bool NODE::is_Connected_To(NODE* nextNode)
{
  std::vector<NODE*> nextNodes = Get_Next_Nodes();
  for (int i = 0; i < (int) nextNodes.size(); i++)
  {
    if (nextNodes[i]->get_ID() == nextNode->get_ID())
      return true;
  }

  return false;
}

/*************************************DFG*************************************/
DFG::~DFG()
{
}

DFG::DFG()
{
  //keep track of last edge IDs
  ARC_Max_Index = 0;
  //keep track of node IDs
  Node_Max_Index = 0;
}

void DFG::insert_Node(NODE *pNode)
{
  if (pNode->get_ID() > Node_Max_Index)
  {
    Node_Max_Index = pNode->get_ID();
  }
  _node_Set.push_back(pNode);
}

// Return a node with given ID number
NODE* DFG::get_Node(int number)
{
  std::vector<NODE*>::iterator iNode1;
  for (iNode1 = _node_Set.begin(); iNode1 != _node_Set.end(); iNode1++)
  {
    if ((*iNode1)->get_ID() == number)
      return *iNode1;
  }

  return NULL;
}

NODE* DFG::get_Node(std::string name){
  std::vector<NODE*>::iterator iNode1;
  for (iNode1 = _node_Set.begin(); iNode1 != _node_Set.end(); iNode1++)
    if((*iNode1)->get_Name().compare(name) == 0)
      return (*iNode1);
  return NULL;
}

NODE* DFG::get_Node(Value* ins)
{
  std::vector<NODE*>::iterator iNode1;
  for (iNode1 = _node_Set.begin(); iNode1 != _node_Set.end(); iNode1++)
  {
    if ((*iNode1)->get_LLVM_Instruction() == ins)
      return *iNode1;
  }

  return NULL;
}

NODE* DFG::get_Node_1(Value* ins)
{
  std::vector<NODE*>::iterator iNode1;
  for (iNode1 = _node_Set.begin(); iNode1 != _node_Set.end(); iNode1++)
  {
    if(debug)
    {
      errs() << "iNode: " << *iNode1 << "\tins: " << (*iNode1)->get_Name()  << "\t" << (*iNode1)->get_LLVM_Instruction() << "\n";
      errs() << "\tins: " << ins << "\n"; 
    }
    if ((*iNode1)->get_LLVM_Instruction() == ins)
      return *iNode1;
  }

  return NULL;
}


NODE* DFG::get_Node_Mem_Add(Value* ins)
{
  std::vector<NODE*>::iterator iNode1;
  for (iNode1 = _node_Set.begin(); iNode1 != _node_Set.end(); iNode1++)
  {
    if ((*iNode1)->get_LLVM_Instruction() == ins && ((*iNode1)->get_Instruction() == st_add || (*iNode1)->get_Instruction() == ld_add))
      return *iNode1;
  }

  return NULL;

}

NODE* DFG::get_Node_Mem_Data(Value* ins)
{
  std::vector<NODE*>::iterator iNode1;
  for (iNode1 = _node_Set.begin(); iNode1 != _node_Set.end(); iNode1++)
  {
    if ((*iNode1)->get_LLVM_Instruction() == ins && ((*iNode1)->get_Instruction() == st_data || (*iNode1)->get_Instruction() == ld_data))
      return *iNode1;
  }

  return NULL;
}

//make an edge between two nodes
void DFG::make_Arc(NODE* pNin, NODE* pNout, int ID, int Distance, DataDepType dep, int operandOrder)
{
  if (pNin->is_Connected_To(pNout))
  {
    // Multiplication is exception. Operation can be square
    // and so two similar arcs are possible with different arc ids
    // but with different operandOrder
    ARC *arc1 = get_Arc(pNin,pNout);
    if(arc1 != NULL)
      if(arc1->GetOperandOrder() == operandOrder)
        return;
  }

  if (pNin->get_ID() == pNout->get_ID() && pNin->has_self_loop())
  {
    return;
  }

  ARC *pArc;
  pArc = new ARC(ID, Distance,operandOrder);
  pArc->Set_Dependency_Type(dep);
  pArc->Set_From_Node(pNin);
  pArc->Set_To_Node(pNout);

  if (pNin->get_ID() == pNout->get_ID())
  {
    pNin->set_self_loop(pArc);
  }
  else
  {
    pNout->add_Pred_Arc(pArc);
    pNin->add_Succ_Arc(pArc);
  }

  _ARC_Set.push_back(pArc);

  if (ARC_Max_Index < ID)
  {
    ARC_Max_Index = ID;
  }
}

// Delete a node from node set
int DFG::delete_Node(NODE *pNode)
{
  int retval=0;
  for (std::vector<NODE*>::iterator it = _node_Set.begin(); it!=_node_Set.end(); ++it)
  {
    if ((*it)==pNode)
    {
      _node_Set.erase(it, it+1);
      retval=1;
      break;
    }
  }

  for (std::vector<ARC*>::iterator it = _ARC_Set.begin(); it!=_ARC_Set.end(); ++it)
  {
    if ((*it)->get_From_Node()==pNode)
    {
      (*it)->get_To_Node()->remove_Pred_Arc(*it);
      _ARC_Set.erase(it, it+1);
      it=_ARC_Set.begin();
      continue;
    }

    if ((*it)->get_To_Node()==pNode)
    {
      (*it)->get_From_Node()->remove_Succ_Arc(*it);
      _ARC_Set.erase(it, it+1);
      it=_ARC_Set.begin();
      continue;
    }
  }

  return retval;
}

// Remove an edge from edge set
void DFG::Remove_Arc(ARC* arc)
{
  std::vector<ARC*>::iterator iEdge;
  for (iEdge = _ARC_Set.begin(); iEdge != _ARC_Set.end(); iEdge++)
  {
    if (*iEdge == arc)
    {
      _ARC_Set.erase(iEdge, iEdge + 1);
      break;

    }
  }
}

// Remove the arc between two nodes. Delete it from arc set
void DFG::Remove_Arc(NODE *pNode, NODE *nNode)
{
  ARC *edge;
  std::vector<ARC*>::iterator iEdge;
  for (iEdge = _ARC_Set.begin(); iEdge != _ARC_Set.end(); iEdge++)
  {
    edge = *iEdge;
    if (edge->get_From_Node()->get_ID() == pNode->get_ID() && edge->get_To_Node()->get_ID() == nNode->get_ID())
    {
      _ARC_Set.erase(iEdge, iEdge + 1);
      pNode->remove_Succ_Arc(edge);
      nNode->remove_Pred_Arc(edge);
      return;
    }
  }
}

// from edge set, get the edge between two nodes
ARC* DFG::get_Arc(NODE *pNode, NODE *nNode)
{
  ARC *edge;
  std::vector<ARC*>::iterator iEdge;
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

std::vector<ARC*> DFG::getSetOfArcs()
{
  return _ARC_Set;
}

std::vector<NODE*> DFG::getSetOfVertices(){
  return _node_Set;
}

void DFG::Dot_Print_DFG(std::string filename)
{
  std::ofstream dotFile;
  std::string graphname = filename;
  filename.append("DFG.dot");
  dotFile.open(filename.c_str());
  dotFile << "digraph " << graphname << " { \n{";

  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    if (_node_Set[i]->is_Mem_Operation())
    {
      dotFile << "\n" << _node_Set[i]->get_ID() << " [color=blue, label=\"" << _node_Set[i]->get_Name() << "\\n" << _node_Set[i]->Op_To_String() << "\"];\n";
    }
    else if (_node_Set[i]->is_Routing_Operation())
    {
      dotFile << "\n" << _node_Set[i]->get_ID() << " [color=green ];\n";
    }
    else if (_node_Set[i]->is_Phi_Operation())
    {
      dotFile << "\n" << _node_Set[i]->get_ID() << " [shape=box, color=red, label=\"" << _node_Set[i]->get_Name() << "\\n" << _node_Set[i]->Op_To_String() << "\"];\n";
    }
    else if (_node_Set[i]->is_ConditionalSelect_Operation())
    {
      dotFile << "\n" << _node_Set[i]->get_ID() << " [shape=diamond, color=gray, label=\"" << _node_Set[i]->get_Name() << "\\n" << _node_Set[i]->Op_To_String() << "\"];\n";
    }
    else if(_node_Set[i]->is_Add_Operation() || _node_Set[i]->is_Sub_Operation()){
      dotFile << "\n" << _node_Set[i]->get_ID() << " [shape=polygon, color=purple, label=\"" << _node_Set[i]->get_Name() << "\\n" << _node_Set[i]->Op_To_String() << "\"];\n";
    }
    else if(_node_Set[i]->is_Mult_Operation() || _node_Set[i]->is_Div_Operation()){
      dotFile << "\n" << _node_Set[i]->get_ID() << " [shape=house, color=purple, label=\"" << _node_Set[i]->get_Name() << "\\n" << _node_Set[i]->Op_To_String() << "\"];\n";
    }
    else if(_node_Set[i]->is_Comp_Operation()){
      dotFile << "\n" << _node_Set[i]->get_ID() << " [shape=trapezium, color=orange, label=\"" << _node_Set[i]->get_Name() << "\\n" << _node_Set[i]->Op_To_String() << "\"];\n";
    }
    else if(_node_Set[i]->is_Binary_Operation()){
      dotFile << "\n" << _node_Set[i]->get_ID() << " [shape=triangle, color=purple, label=\"" << _node_Set[i]->get_Name() << "\\n" << _node_Set[i]->Op_To_String() << "\"];\n";
    }
    else
    {
      dotFile << "\n" << _node_Set[i]->get_ID() << " [color=black, label=\"" << _node_Set[i]->get_Name() << "\\n" << _node_Set[i]->Op_To_String() << "\"];\n";
    }
  }

  for (int i = 0; i < (int) _ARC_Set.size(); i++)
  {
    if (_ARC_Set[i]->Get_Dependency_Type() == PredDep)
    {
      if (_ARC_Set[i]->Get_Inter_Iteration_Distance() == 0)
      {
        dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [color=green ]\n";
      }
      else
      {
        dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=bold, color=blue, label=" << _ARC_Set[i]->Get_Inter_Iteration_Distance() << "] \n";
      }

    }
    else if ((_ARC_Set[i]->Get_Dependency_Type() == LiveInDataDep) || (_ARC_Set[i]->Get_Dependency_Type() == LiveOutDataDep))
    {
      unsigned alignment = 4;
      if(_ARC_Set[i]->Get_Dependency_Type() == LiveInDataDep)
      {
        alignment = _ARC_Set[i]->get_From_Node()->getAlignment();
      }
      else
        alignment = _ARC_Set[i]->get_To_Node()->getAlignment();
      dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [color=orange, label=" << alignment << "] \n";
    }
    else if(_ARC_Set[i]->Get_Dependency_Type() == LoopControlDep){
      dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [color=blueviolet]\n";
    }
    else if(_ARC_Set[i]->get_From_Node()->get_Instruction() == constant)
    {
      dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [color=gray]\n";
    }
    else if (_ARC_Set[i]->Get_Inter_Iteration_Distance() == 0)
    {
      dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << "\n";
    }
    else
    {
      dotFile << _ARC_Set[i]->get_From_Node()->get_ID() << " -> " << _ARC_Set[i]->get_To_Node()->get_ID() << " [style=bold, color=red, label=" << _ARC_Set[i]->Get_Inter_Iteration_Distance() << "] \n";
    }
  }

  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    if (_node_Set[i]->is_Load_Address_Generator() || _node_Set[i]->is_Store_Address_Generator())
    {
      dotFile << _node_Set[i]->get_ID() << " -> " << _node_Set[i]->get_Related_Node()->get_ID() << " [style=dotted, color=blue, label=\"" << _node_Set[i]->getAlignment() << "\"]\n";
    }
  }
  dotFile << "\n}\n";

  dotFile << "\n}";
  dotFile.close();
}

void DFG::Dump_Loop(std::string filename)
{
  //DIPAL VERSION
  std::ofstream edgeFile,nodeFile;
  std::string graphname_edge = filename;
  graphname_edge.append("_edge.txt");
  edgeFile.open(graphname_edge.c_str());

  std::string graphname_node = filename;
  graphname_node.append("_node.txt");
  nodeFile.open(graphname_node.c_str());

  for (int i = 0; i < (int) _node_Set.size(); i++)
  {
    if(debug)
      errs() << "we are in node set\n";
    unsigned alignment = 0;
    if (_node_Set[i]->is_Load_Address_Generator())
    {
      if(debug) {
        errs() << "load: " << _node_Set[i]->get_ID() << "\t" << _node_Set[i]->get_Name() << "\n";
        errs() << "related: " << _node_Set[i]->get_Related_Node()->get_ID() << "\n";
      }
      edgeFile << _node_Set[i]->get_ID() << "\t" << _node_Set[i]->get_Related_Node()->get_ID() << "\t" << "0" << "\t" << "LRE" << "\t0" << "\n";
      alignment = _node_Set[i]->getAlignment();
    }
    if (_node_Set[i]->is_Store_Address_Generator())
    {
      if(debug)
        errs() << "store\n";
      edgeFile << _node_Set[i]->get_ID() << "\t" << _node_Set[i]->get_Related_Node()->get_ID() << "\t" << "0" << "\t" << "SRE" << "\t0" << "\n";
      alignment = _node_Set[i]->getAlignment();
    }
    if(_node_Set[i]->get_Instruction() == constant)
    {
      if(_node_Set[i]->get_Name().find("gVar") == 0 || _node_Set[i]->get_Name().find("gPtr") == 0)
      {
        Datatype dt; 
        std::vector<NODE*> get_Nodes;
        if((_node_Set[i]->Get_Next_Nodes()).size() != 0)
        {
          get_Nodes.clear();
          get_Nodes = _node_Set[i]->Get_Next_Nodes(); 
          dt = (get_Nodes[0])->getDatatype(); 
        }
        else if((_node_Set[i]->Get_Prev_Nodes()).size() != 0)
        {
          get_Nodes.clear();
          get_Nodes = _node_Set[i]->Get_Prev_Nodes();
          dt = (get_Nodes[0])->getDatatype();
        }

        _node_Set[i]->setDatatype(dt); 
      } 
      alignment = _node_Set[i]->getAlignment();

    }

    // Do not dump liveout. Allow Live-In
    if((_node_Set[i]->get_Instruction() == constant) && (_node_Set[i]->get_Number_of_Pred() > 0) && (_node_Set[i]->get_Number_of_Succ() <= 0))
      continue;

    if((_node_Set[i]->get_Instruction() == cgra_select) && (_node_Set[i]->get_Number_of_Pred() > 3))
    {
      if(debug) errs() << "nodefile node: " << _node_Set[i]->get_ID() << "\tpred:" << _node_Set[i]->get_Number_of_Pred() << "\n";
      nodeFile << _node_Set[i]->get_ID() << "\t" << cond_select << "\t" << _node_Set[i]->get_Name() << "\t" << alignment << "\t" << _node_Set[i]->getDatatype() << "\n";
    }
    else
    {
      // if([i]->get_Instruction() == constant)
      //  nodeFile << [i]->get_ID() << "\t" << _node_Set[i]->get_Instruction() << "\t" << _node_Set[i]->get_Name() << "\t" << alignment << "\t0" << "\n";
      //else
      nodeFile << _node_Set[i]->get_ID() << "\t" << _node_Set[i]->get_Instruction() << "\t" << _node_Set[i]->get_Name() << "\t" << alignment << "\t" << _node_Set[i]->getDatatype() << "\n";
    }
  }

  for (int i = 0; i < (int) _ARC_Set.size(); i++)
  {
    if ((_ARC_Set[i]->Get_Dependency_Type() == LiveOutDataDep)) // && (_ARC_Set[i]->get_To_Node()->get_Instruction() == constant))
      continue;

    if (_ARC_Set[i]->Get_Dependency_Type() == PredDep)
    {
      edgeFile << _ARC_Set[i]->get_From_Node()->get_ID() << "\t" << _ARC_Set[i]->get_To_Node()->get_ID() << "\t" << _ARC_Set[i]->Get_Inter_Iteration_Distance() << "\t" << "PRE"<<"\t" <<_ARC_Set[i]->GetOperandOrder() << "\n";

    }
    else if(_ARC_Set[i]->Get_Dependency_Type() == LiveInDataDep)
    {
      edgeFile << _ARC_Set[i]->get_From_Node()->get_ID() << "\t" << _ARC_Set[i]->get_To_Node()->get_ID() << "\t" << _ARC_Set[i]->Get_Inter_Iteration_Distance() << "\t" << "LIE"<<"\t" <<_ARC_Set[i]->GetOperandOrder() << "\n";
    }
    else if(_ARC_Set[i]->Get_Dependency_Type() == LoopControlDep)
      edgeFile << _ARC_Set[i]->get_From_Node()->get_ID() << "\t" << _ARC_Set[i]->get_To_Node()->get_ID() << "\t" << _ARC_Set[i]->Get_Inter_Iteration_Distance() << "\t" << "LCE"<<"\t" <<_ARC_Set[i]->GetOperandOrder() << "\n";
    else
    {
      edgeFile << _ARC_Set[i]->get_From_Node()->get_ID() << "\t" << _ARC_Set[i]->get_To_Node()->get_ID() << "\t" << _ARC_Set[i]->Get_Inter_Iteration_Distance() << "\t" << "TRU"<<"\t"<< _ARC_Set[i]->GetOperandOrder() << "\n";
    }
  }

  edgeFile.close();
  nodeFile.close();
}

///make a new node from instruction BI and adds it to DFe* namespace llvm */
