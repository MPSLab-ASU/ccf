/*
 * REGINode.cpp
 *
 *  Created on: Jul 24, 2012
 *      Author: mahdi
 */

#include "REGINode.h"

/*REGI_Node::REGI_Node()
  {
  cuid = -1;
//insn = NULL;

Load_Output_Address_bus = false;
Store_Output_Address_bus = false;
Output_Data_Bus = false;
Input_Data_Bus = false;

related = NULL;

sched_info = new REGI_Scheduling_Info();

/* *************************CycleS************************************ */
/*index_cycle = -1;
  low_index_cycle = -1;
  index_defined_cycle = -1;*/
//**************************CycleE*************************************//
/*tent_load = false;
  tent_store = false;

  }*/
REGI_Node::REGI_Node(Instruction_Operation ins, int laten, int id)
{

    insn = ins;
    cuid = id;
    latency = laten;
    if (latency < 1)
    {
        printf("latency cannot be less than 1\n");
    }
    Load_Output_Address_bus = false;
    Store_Output_Address_bus = false;
    Output_Data_Bus = false;
    Input_Data_Bus = false;

    related = NULL;

    sched_info = new REGI_Scheduling_Info();

    //**************************CycleS*************************************//
    index_cycle = -1;
    low_index_cycle = -1;
    index_defined_cycle = false;
    //**************************CycleE*************************************//
    tent_load = false;
    tent_store = false;

    assigned = false;
    AssignedPE = NULL;
    loop = NULL;
    self_loop = false;
    minimizable = false;


    predicated = false;
    predicating = false;
}

REGI_Node::REGI_Node(Instruction_Operation ins, int laten, int id, bool load, bool store)
{
    insn = ins;
    cuid = id;
    latency = laten;
    if (latency < 1)
    {
        printf("latency cannot be less than 1\n");
    }
    Load_Output_Address_bus = false;
    Store_Output_Address_bus = false;
    Output_Data_Bus = false;
    Input_Data_Bus = false;

    related = NULL;

    sched_info = new REGI_Scheduling_Info();

    //**************************CycleS*************************************//
    index_cycle = -1;
    low_index_cycle = -1;
    index_defined_cycle = false;
    //**************************CycleE*************************************//
    tent_load = load;
    tent_store = store;

    assigned = false;
    AssignedPE = NULL;
    loop = NULL;
    self_loop = false;
    minimizable = false;


    predicated =false;
    predicating = false;
}

bool REGI_Node::is_tentative_load()
{
    return tent_load;
}

bool REGI_Node::is_tentative_store()
{
    return tent_store;
}

void REGI_Node::set_self_loop(REGI_ARC* s_loop)
{
    self_loop = true;
    loop = s_loop;
}

bool REGI_Node::has_self_loop()
{
    return self_loop;
}

REGI_ARC* REGI_Node::get_self_loop()
{
    return loop;
}

REGI_Node::~REGI_Node()
{
    inCommingArcs.clear();
    outGoingArcs.clear();
    delete (sched_info);
}

void REGI_Node::set_Instruction(Instruction_Operation ins, int laten, int id)
{
    insn = ins;
    cuid = id;
    latency = laten;
}

int REGI_Node::get_ID()
{
    return cuid;
}

int REGI_Node::get_Latency()
{
    return latency;
}

void REGI_Node::set_Latency(int laten)
{
    latency = laten;
}

Instruction_Operation REGI_Node::get_Instruction()
{
    return insn;
}

void REGI_Node::set_Load_Address_Generator(REGI_Node* next)
{
    Load_Output_Address_bus = true;
    related = next;
    latency = 1;
}

void REGI_Node::set_Load_Data_Bus_Read(REGI_Node* next)
{
    Input_Data_Bus = true;
    related = next;
    latency = 1;
}

void REGI_Node::set_Store_Address_Generator(REGI_Node* next)
{
    Store_Output_Address_bus = true;
    related = next;
    //this must be 1 so that any dependent instruction (store bus assertion) is to be scheduled at the same cycle as this node
    latency = 0;
}

void REGI_Node::set_Store_Data_Bus_Write(REGI_Node* next)
{
    Output_Data_Bus = true;
    related = next;
    latency = 1;
}

bool REGI_Node::is_Load_Address_Generator()
{
    return Load_Output_Address_bus;

}

bool REGI_Node::is_Load_Data_Bus_Read()
{
    return Input_Data_Bus;

}

bool REGI_Node::is_Store_Address_Generator()
{
    return Store_Output_Address_bus;
}

bool REGI_Node::is_Store_Data_Bus_Write()
{
    return Output_Data_Bus;

}

bool REGI_Node::is_Mem_Operation()
{
    return (Output_Data_Bus || Store_Output_Address_bus || Input_Data_Bus || Load_Output_Address_bus);
}

bool REGI_Node::is_PHI_Operations()
{
    return (insn == select_op);
}

bool REGI_Node::is_Minimizable()
{
    return minimizable;
}
void REGI_Node::set_Minimizable()
{
    minimizable = true;
}
void REGI_Node::Reset_Minimizable()
{
    minimizable = false;
}
REGI_Node* REGI_Node::get_Related_Node()
{
    return related;
}

bool REGI_Node::isPredicated()
{
    return predicated;
}

bool REGI_Node::isPredicating()
{
    return predicating;
}
//get the number of incomming edges
int REGI_Node::get_Number_of_Pred()
{
    return (int) inCommingArcs.size();
}
int REGI_Node::get_Number_of_Operands()
{
    int count = 0;
    for (int i = 0; i < (int) inCommingArcs.size(); i++)
    {
        if (inCommingArcs[i]->get_Dependency() == TrueDep)
        {
            count++;
        }

    }
    return count;
}

//get the number of outgoing edges
int REGI_Node::get_Number_of_Succ()
{
    return (int) outGoingArcs.size();
}

//return the ith incomming egde
REGI_ARC* REGI_Node::get_Pred_Arc(int i)
{
    return inCommingArcs[i];
}

//return ith outgoing edge
REGI_ARC* REGI_Node::get_Succ_Arc(int i)
{
    return outGoingArcs[i];
}

//return the set of incomming edges
vector<REGI_Node*> REGI_Node::Get_Prev_Nodes()
{
    vector<REGI_Node*> retVal;
    int SizeOfIncomming = inCommingArcs.size();
    for (int i = 0; i < SizeOfIncomming; i++)
    {
        retVal.push_back(inCommingArcs[i]->get_From_Node());
    }
    return retVal;
}
//return the set of predecessors with true dependency
vector<REGI_Node*> REGI_Node::Get_True_Dependency_Predecessors()
{
    vector<REGI_Node*> retVal;
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

vector<REGI_Node*> REGI_Node::Get_Predecessors_Exclude_Address_Dependency()
{
    vector<REGI_Node*> retVal;
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


vector<REGI_Node*> REGI_Node::Get_Explicit_Dependency_Predecessor()
{
    vector<REGI_Node*> retVal;
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
vector<REGI_Node*> REGI_Node::Get_Predicate_Dependency_Predecessor()
{
    vector<REGI_Node*> retVal;
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
vector<REGI_Node*> REGI_Node::Get_Prev_Nodes_Same_Iteration()
{
    vector<REGI_Node*> retVal;
    int SizeOfIncomming = inCommingArcs.size();
    for (int i = 0; i < SizeOfIncomming; i++)
    {
        if (inCommingArcs[i]->get_Distance() == 0)
            retVal.push_back(inCommingArcs[i]->get_From_Node());
    }
    return retVal;
}


vector<REGI_Node*> REGI_Node::Get_Predecessors_Same_Iteration_Exclude_Address_Dependency()
{
    vector<REGI_Node*> retVal;
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


vector<REGI_Node*> REGI_Node::Get_Explicit_Dependency_Predecessors_Same_Iteration()
{
    vector<REGI_Node*> retVal;
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
vector<REGI_Node*> REGI_Node::Get_True_Dependency_Predecessors_Same_Iteration()
{
    vector<REGI_Node*> retVal;
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
vector<REGI_Node*> REGI_Node::Get_Prev_Nodes_Prev_Iteration()
{
    vector<REGI_Node*> retVal;
    int SizeOfIncomming = inCommingArcs.size();
    for (int i = 0; i < SizeOfIncomming; i++)
    {
        if (inCommingArcs[i]->get_Distance() > 0)
            retVal.push_back(inCommingArcs[i]->get_From_Node());
    }
    return retVal;
}
//return the set of predecessors with true dependency
vector<REGI_Node*> REGI_Node::Get_True_Dependency_Predecessors_Prev_Iterations()
{
    vector<REGI_Node*> retVal;
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

vector<REGI_Node*> REGI_Node::Get_Explicit_Dependency_Predecessors_Prev_Iterations()
{
    vector<REGI_Node*> retVal;
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
vector<REGI_Node*> REGI_Node::Get_True_Dependency_Successors()
{
    vector<REGI_Node*> retVal;
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
vector<REGI_Node*> REGI_Node::Get_Explicit_Dependency_Successors()
{
    vector<REGI_Node*> retVal;
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
vector<REGI_Node*> REGI_Node::Get_Next_Nodes()
{
    vector<REGI_Node*> retVal;
    int SizeOfOutGoing = outGoingArcs.size();
    for (int i = 0; i < SizeOfOutGoing; i++)
    {
        retVal.push_back(outGoingArcs[i]->get_To_Node());
    }
    return retVal;
}

vector<REGI_Node*> REGI_Node::Get_Next_Nodes_Same_Iteration()
{
    vector<REGI_Node*> retVal;
    int SizeOfOutGoing = outGoingArcs.size();
    for (int i = 0; i < SizeOfOutGoing; i++)
    {
        if (outGoingArcs[i]->get_Distance() == 0)
        {
            retVal.push_back(outGoingArcs[i]->get_To_Node());
        }
    }
    return retVal;
}

//return successors with predicate dependency with distance = 0
vector<REGI_Node*> REGI_Node::Get_Predicate_Dependency_Successors_Same_Iteration()
{
    vector<REGI_Node*> retVal;
    int SizeOfOutGoing = outGoingArcs.size();
    for (int i = 0; i < SizeOfOutGoing; i++)
    {
        if (outGoingArcs[i]->get_Dependency() == PredDep && outGoingArcs[i]->get_Distance() == 0)
        {
            retVal.push_back(outGoingArcs[i]->get_To_Node());
        }
    }
    return retVal;
}
//return successors with true dependency with distance = 0
vector<REGI_Node*> REGI_Node::Get_True_Dependency_Successors_Same_Iteration()
{
    vector<REGI_Node*> retVal;
    int SizeOfOutGoing = outGoingArcs.size();
    for (int i = 0; i < SizeOfOutGoing; i++)
    {
        if (outGoingArcs[i]->get_Dependency() == TrueDep && outGoingArcs[i]->get_Distance() == 0)
        {
            retVal.push_back(outGoingArcs[i]->get_To_Node());
        }
    }
    return retVal;
}

//return successors with distance = 0 excluding load store address dependency
vector<REGI_Node*> REGI_Node::Get_Successors_Same_Iteration_Exclude_Address_Dependency()
{
    vector<REGI_Node*> retVal;
    int SizeOfOutGoing = outGoingArcs.size();
    for (int i = 0; i < SizeOfOutGoing; i++)
    {
        if (outGoingArcs[i]->get_Dependency() != LoadDep && outGoingArcs[i]->get_Dependency() != StoreDep  && outGoingArcs[i]->get_Distance() == 0)
        {
            retVal.push_back(outGoingArcs[i]->get_To_Node());
        }
    }
    return retVal;
}

//return successors with true or predicate dependency with distance = 0
vector<REGI_Node*> REGI_Node::Get_Explicit_Dependency_Successors_Same_Iteration()
{
    vector<REGI_Node*> retVal;
    int SizeOfOutGoing = outGoingArcs.size();
    for (int i = 0; i < SizeOfOutGoing; i++)
    {
        if ((outGoingArcs[i]->get_Dependency() == TrueDep ||outGoingArcs[i]->get_Dependency() == PredDep)  && outGoingArcs[i]->get_Distance() == 0)
        {
            retVal.push_back(outGoingArcs[i]->get_To_Node());
        }
    }
    return retVal;
}
//return the set of outgoing edges with distance
vector<REGI_Node*> REGI_Node::Get_Next_Nodes_Next_Iteration()
{
    vector<REGI_Node*> retVal;
    int SizeOfOutGoing = outGoingArcs.size();
    for (int i = 0; i < SizeOfOutGoing; i++)
    {
        if (outGoingArcs[i]->get_Distance() > 0)
        {
            retVal.push_back(outGoingArcs[i]->get_To_Node());
        }
    }
    return retVal;
}

//return successors with true dependency with distance > 0
vector<REGI_Node*> REGI_Node::Get_True_Dependency_Successors_Next_Iterations()
{
    vector<REGI_Node*> retVal;
    int SizeOfOutGoing = outGoingArcs.size();
    for (int i = 0; i < SizeOfOutGoing; i++)
    {
        if (outGoingArcs[i]->get_Dependency() == TrueDep && outGoingArcs[i]->get_Distance() > 0)
        {
            retVal.push_back(outGoingArcs[i]->get_To_Node());
        }
    }
    return retVal;
}

//return successors with true or predicate dependency with distance > 0
vector<REGI_Node*> REGI_Node::Get_Explicit_Dependency_Successors_Next_Iterations()
{
    vector<REGI_Node*> retVal;
    int SizeOfOutGoing = outGoingArcs.size();
    for (int i = 0; i < SizeOfOutGoing; i++)
    {
        if ((outGoingArcs[i]->get_Dependency() == TrueDep ||outGoingArcs[i]->get_Dependency() == PredDep)  && outGoingArcs[i]->get_Distance() > 0)
        {
            retVal.push_back(outGoingArcs[i]->get_To_Node());
        }
    }
    return retVal;
}

//add a new incomming edge
void REGI_Node::add_Pred_Arc(REGI_ARC* pred_Arc)
{
    if (pred_Arc->get_Dependency() == PredDep)
    {
        FATAL(predicated, "Cannot Predicate an already predicated operation");
        predicated=true;
    }
    inCommingArcs.push_back(pred_Arc);
}

//add a new outgoing edge
void REGI_Node::add_Succ_Arc(REGI_ARC* succ_Arc)
{
    if (succ_Arc->get_Dependency() == PredDep)
    {
        predicating = true;
    }

    outGoingArcs.push_back(succ_Arc);
}

//remove an incomming edge
int REGI_Node::remove_Pred_Arc(REGI_ARC* predArc)
{
    vector<REGI_ARC*>::iterator iEdge;

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
int REGI_Node::remove_Succ_Arc(REGI_ARC* succArc)
{
    vector<REGI_ARC*>::iterator iEdge;

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
bool REGI_Node::is_Connected_To(REGI_Node* nextNode)
{
    if (nextNode == this && self_loop)
    {
        return true;
    }
    vector<REGI_Node*> nextNodes = Get_Next_Nodes();
    for (int i = 0; i < (int) nextNodes.size(); i++)
    {
        if (nextNodes[i]->get_ID() == nextNode->get_ID())
        {
            return true;
        }
    }

    return false;
}

//returns true if there is a physical edge between this node and next node
bool REGI_Node::is_Physically_Connected_To(REGI_Node* nextNode)
{
    if (nextNode == this && self_loop)
    {
        return true;
    }
    vector<REGI_Node*> nextNodes = Get_Explicit_Dependency_Successors();
    for (int i = 0; i < (int) nextNodes.size(); i++)
    {
        if (nextNodes[i]->get_ID() == nextNode->get_ID())
        {
            return true;
        }
    }

    return false;
}

//**************************Scheduling********************************//

REGI_Scheduling_Info* REGI_Node::get_Sched_Info()
{
    return sched_info;
}

bool REGI_Node::is_ready_to_schedule_ASAP(int &time)
{
    time = 0;
    int current_time;
    vector<REGI_Node*> prev_nodes = Get_Predecessors_Same_Iteration_Exclude_Address_Dependency();
    for (int i = 0; i < (int) prev_nodes.size(); i++)
    {
        if (!prev_nodes[i]->get_Sched_Info()->is_ASAP_Initiated())
        {
            return false;
        }
        current_time = prev_nodes[i]->get_Sched_Info()->get_ASAP() + prev_nodes[i]->get_Latency();
        if (current_time > time)
        {
            time = current_time;
        }
    }
    return true;
}

bool REGI_Node::is_ready_to_schedule_ALAP(int &time)
{
    time = MAXINT;
    int current_time;
    vector<REGI_Node*> next_nodes = Get_Successors_Same_Iteration_Exclude_Address_Dependency();
    for (int i = 0; i < (int) next_nodes.size(); i++)
    {
        if (!next_nodes[i]->get_Sched_Info()->is_ALAP_Initiated())
        {
            return false;
        }
        current_time = next_nodes[i]->get_Sched_Info()->get_ALAP() - latency;
        if (current_time < time)
        {
            time = current_time;
        }
    }
    return true;
}

bool REGI_Node::is_ready_to_schedule_Feasible_ASAP(int &time)
{
    time = 0;
    int current_time;
    vector<REGI_Node*> prev_nodes = Get_Predecessors_Same_Iteration_Exclude_Address_Dependency();
    for (int i = 0; i < (int) prev_nodes.size(); i++)
    {
        if (!prev_nodes[i]->get_Sched_Info()->is_Feasible_ASAP_Initiated())
        {
            return false;
        }
        current_time = prev_nodes[i]->get_Sched_Info()->get_Feasible_ASAP() + prev_nodes[i]->get_Latency();
        if (current_time > time)
        {
            time = current_time;
        }
    }
    return true;
}

bool REGI_Node::is_ready_to_schedule_Feasible_ALAP(int &time, int MAX_Schedule)
{
    time = MAXINT;
    int current_time;
    vector<REGI_Node*> next_nodes = Get_Successors_Same_Iteration_Exclude_Address_Dependency();
    if ((int) next_nodes.size() > 0)
    {

        for (int i = 0; i < (int) next_nodes.size(); i++)
        {
            if (!next_nodes[i]->get_Sched_Info()->is_Feasible_ALAP_Initiated())
            {
                //printf("Node %d is not scheduled yet\n", next_nodes[i]->get_ID());

                return false;
            }
            current_time = next_nodes[i]->get_Sched_Info()->get_Feasible_ALAP() - latency;
            if (current_time < time)
            {
                time = current_time;
            }
        }
        return true;
    }
    else
    {
        time = MAX_Schedule - latency;
        return true;
    }
}
bool REGI_Node::is_ready_to_schedule_modulo(int &time)
{
    time = 0;
    int current_time;
    if (inCommingArcs.size() == 0)
    {
        time = 0;
        return true;
    }
    vector<REGI_Node*> prev_nodes = Get_Predecessors_Same_Iteration_Exclude_Address_Dependency();
    if ((int) prev_nodes.size() != 0)
    {
        for (int i = 0; i < (int) prev_nodes.size(); i++)
        {
            if (!prev_nodes[i]->get_Sched_Info()->is_CURRENT_Initiated())
            {
                return false;
            }
            current_time = prev_nodes[i]->get_Sched_Info()->get_Current() + prev_nodes[i]->get_Latency();
            if (current_time > time)
            {
                time = current_time;
            }
        }
    }
    else
    {
        prev_nodes = Get_Predecessors_Exclude_Address_Dependency();
        for (int i = 0; i < (int) prev_nodes.size(); i++)
        {
            if (!prev_nodes[i]->get_Sched_Info()->is_CURRENT_Initiated())
            {
                return false;
            }

        }
        time = 0;
    }

    return true;
}

bool REGI_Node::is_ready_to_schedule_modulo_backward(int &time)
{

    time = get_Sched_Info()->get_Feasible_ALAP();
    int current_time = get_Sched_Info()->get_Feasible_ALAP();
    if (outGoingArcs.size() == 0)
    {
        return true;
    }
    vector<REGI_Node*> next_nodes = Get_Successors_Same_Iteration_Exclude_Address_Dependency();
    for (int i = 0; i < (int) next_nodes.size(); i++)
    {
        if (!next_nodes[i]->get_Sched_Info()->is_CURRENT_Initiated())
        {
            return false;
        }
        current_time = next_nodes[i]->get_Sched_Info()->get_Current() - get_Latency();
        if (current_time < time)
        {
            time = current_time;
        }
    }
    return true;

}

bool REGI_Node::has_multiple_constrains_modulo_backward_at_time(int time, vector<REGI_Node*> &constrainers)
{

    constrainers.clear();
    constrainers = Get_Next_Nodes_Next_Iteration();
    int counter = (int) constrainers.size();
    vector<REGI_Node*> next_nodes = Get_Next_Nodes_Same_Iteration();

    vector<REGI_Node*> prev_next_node;

    for (int i = 0; i < (int) next_nodes.size(); i++)
    {
        if (!next_nodes[i]->get_Sched_Info()->is_CURRENT_Initiated())
        {
            continue;
        }
        if (time < next_nodes[i]->get_Sched_Info()->get_Current() - get_Latency())
        {
            next_nodes[i]->has_multiple_constrains_modulo_at_time(next_nodes[i]->get_Sched_Info()->get_Current(), prev_next_node);
            if ((int) prev_next_node.size() > 0)
            {
                constrainers.push_back(next_nodes[i]);
                counter++;
            }
        }

    }

    if (counter > 1)
    {
        return true;

    }
    return false;

}


bool REGI_Node::has_multiple_constrains_modulo_at_time(int time, vector<REGI_Node*> &constrainers)
{

    int counter = 0;
    constrainers.clear();
    vector<REGI_Node*> prev_nodes = Get_Prev_Nodes_Same_Iteration();
    for (int i = 0; i < (int) prev_nodes.size(); i++)
    {
        if (!prev_nodes[i]->get_Sched_Info()->is_CURRENT_Initiated())
        {
            continue;
        }
        if (time > prev_nodes[i]->get_Sched_Info()->get_Current() + prev_nodes[i]->get_Latency())
        {
            counter++;
            constrainers.push_back(prev_nodes[i]);
        }
    }
    if (counter > 1)
    {
        return true;
    }
    return false;
}

bool REGI_Node::constrains_multiple_modulo_backward_at_time(int time, vector<REGI_Node*> &constrainers)
{
    int counter = 0;
    constrainers.clear();
    vector<REGI_Node*> next_nodes = Get_Next_Nodes_Same_Iteration();

    vector<REGI_Node*> prev_next_node;

    if ((int) next_nodes.size() != 0)
    {
        for (int i = 0; i < (int) next_nodes.size(); i++)
        {
            if (!next_nodes[i]->get_Sched_Info()->is_CURRENT_Initiated())
            {
                continue;
            }
            if (time < next_nodes[i]->get_Sched_Info()->get_Current() - get_Latency())
            {
                constrainers.push_back(next_nodes[i]);
                counter++;

            }

        }
    }
    if (counter > 1)
    {
        return true;

    }
    return false;
}
//**************************Scheduling********************************//

//**************************cycle DetectionS********************************//
void REGI_Node::Set_Index_Cycle(int index)
{
    this->index_cycle = index;
    index_defined_cycle = true;
}

int REGI_Node::get_Index_Cycle()
{
    return index_cycle;
}

void REGI_Node::Set_Low_Index_Cycle(int index)
{
    this->low_index_cycle = index;
}

int REGI_Node::get_Low_Index_Cycle()
{
    return low_index_cycle;
}

bool REGI_Node::Is_Index_Defined_Cycle()
{
    return index_defined_cycle;
}

void REGI_Node::Reset_Cycle_Index()
{
    index_cycle = -1;
    low_index_cycle = -1;
    index_defined_cycle = false;
}

//**************************MappingS*************************************//

void REGI_Node::Assign2PE(REGI_PE* pe)
{

    //cout<<"Node "<<number<<" is assigned to PE "<<pe->getIndexI()<<","<< pe->getIndexJ()<<","<<pe->getIndexT()<<endl;

    this->AssignedPE = pe;
    assigned = true;

}

REGI_PE* REGI_Node::getPE()
{
    return this->AssignedPE;
}

bool REGI_Node::isFree()
{
    return !assigned;
}

bool REGI_Node::isMapped()
{
    return assigned;
}

void REGI_Node::Reset()
{
    assigned = false;
    AssignedPE = NULL;
    //cout<<"Node "<<number<<" is unmapped"<<endl;
}

/*void REGI_Node::SetRequiredRegister(int num)
  {
  }

  int REGI_Node::GetRequiredRegister()
  {
  return 1;
  }
  */
vector<REGI_Node*> REGI_Node::get_prev_operations_constrain_this_modulo()
{
    vector<REGI_Node*> retVal;
    int SizeOfIncomming = inCommingArcs.size();
    for (int i = 0; i < SizeOfIncomming; i++)
    {
        if (inCommingArcs[i]->get_Distance() > 0
                || (this->get_Sched_Info()->get_Current() - inCommingArcs[i]->get_From_Node()->get_Sched_Info()->get_Current()) > 1)
        {
            retVal.push_back(inCommingArcs[i]->get_From_Node());
        }
    }
    return retVal;

}

vector<REGI_Node*> REGI_Node::get_next_operations_constrain_by_this_modulo()
{
    vector<REGI_Node*> retVal;
    int SizeOfIncomming = outGoingArcs.size();
    for (int i = 0; i < SizeOfIncomming; i++)
    {
        if (outGoingArcs[i]->get_Distance() > 0
                || (outGoingArcs[i]->get_To_Node()->get_Sched_Info()->get_Current() - this->get_Sched_Info()->get_Current() > 1))
        {
            retVal.push_back(outGoingArcs[i]->get_To_Node());
        }
    }
    return retVal;

}
vector<REGI_Node*> REGI_Node::get_physical_successors_constrain_by_this_modulo()
{
    vector<REGI_Node*> retVal;
    int SizeOfIncomming = outGoingArcs.size();
    for (int i = 0; i < SizeOfIncomming; i++)
    {
        if (outGoingArcs[i]->get_Dependency()!= TrueDep && outGoingArcs[i]->get_Dependency()!= PredDep)
        {
            continue;
        }
        if (outGoingArcs[i]->get_Distance() > 0
                || (outGoingArcs[i]->get_To_Node()->get_Sched_Info()->get_Current() - this->get_Sched_Info()->get_Current() > 1))
        {
            retVal.push_back(outGoingArcs[i]->get_To_Node());
        }
    }
    return retVal;

}

vector<REGI_Node*> REGI_Node::get_physical_predecessors_constrain_this_modulo()
{
    vector<REGI_Node*> retVal;
    int SizeOfIncomming = inCommingArcs.size();
    for (int i = 0; i < SizeOfIncomming; i++)
    {
        if (inCommingArcs[i]->get_Dependency()!= TrueDep && inCommingArcs[i]->get_Dependency()!= PredDep)
        {
            continue;
        }
        if (inCommingArcs[i]->get_Distance() > 0
                || (this->get_Sched_Info()->get_Current() - inCommingArcs[i]->get_From_Node()->get_Sched_Info()->get_Current()) > 1)
        {
            retVal.push_back(inCommingArcs[i]->get_From_Node());
        }
    }
    return retVal;

}

//**************************MappingE*************************************//

//**************************cycle DetectionE********************************//

REGI_Node_Dummy::~REGI_Node_Dummy()
{

}
REGI_Node_Dummy::REGI_Node_Dummy(int laten, int id) :
    REGI_Node(route_op, laten, id)
{
}
REGI_Node_Dummy::REGI_Node_Dummy(int laten, int id, REGI_Node* orig) :
    REGI_Node(route_op, laten, id)
{ 
    if (orig->isPredicated())
    {
        //get predicate input
        vector<REGI_Node*> predic_inputs = orig->Get_Predicate_Dependency_Predecessor();

        for(int i=0;i< orig->get_Number_of_Operands(); i++)
        {

            REGI_ARC *arc = orig->get_Pred_Arc(1);
            if (arc->get_From_Node()==predic_inputs[0])
            {
                REGI_ARC *pArc = new REGI_ARC(id+1, arc->get_Distance(), arc->get_Dependency());


                //set source and destinations of the arc
                pArc->Set_From_Node(predic_inputs[0]);
                pArc->Set_To_Node(this);
                add_Pred_Arc(pArc);

            }
        }
    }
}

REGI_Reduced_Node::~REGI_Reduced_Node()
{
}
REGI_Reduced_Node::REGI_Reduced_Node(int id, REGI_Node* left, REGI_Node* right, REGI_Node* phi, vector<REGI_ARC*>&to_remove, vector<REGI_ARC*>&to_add) :
    REGI_Node(reduced_op, max(max(left->get_Latency(), right->get_Latency()), phi->get_Latency()), id)
{
    this->left = left;
    this->right = right;
    this->phi = phi;
    int cur_id = id + 1;
    vector<REGI_Node*> temp_nodes;
    vector<REGI_ARC*> temp_arcs;
    REGI_ARC* temp_arc;
    int max_distance = -1;
    //first we fix loop//if any of those nodes have loop this node should have loop
    if (left->has_self_loop())
    {

        max_distance = left->get_self_loop()->get_Distance();
    }
    if (right->has_self_loop())
    {
        if (right->get_self_loop()->get_Distance() > max_distance)
            max_distance = right->get_self_loop()->get_Distance();
    }
    if (phi->has_self_loop())
    {
        if (phi->get_self_loop()->get_Distance() > max_distance)
            max_distance = phi->get_self_loop()->get_Distance();
    }
    if (phi->is_Connected_To(left))
    {
        for (int i = 0; i < (int) phi->get_Number_of_Succ(); i++)
        {
            if (phi->get_Succ_Arc(i)->get_To_Node() == left)
            {
                if (phi->get_Succ_Arc(i)->get_Distance() > max_distance)
                {
                    max_distance = phi->get_Succ_Arc(i)->get_Distance();
                }
            }
        }

    }
    if (phi->is_Connected_To(right))
    {
        for (int i = 0; i < (int) phi->get_Number_of_Succ(); i++)
        {
            if (phi->get_Succ_Arc(i)->get_To_Node() == right)
            {
                if (phi->get_Succ_Arc(i)->get_Distance() > max_distance)
                {
                    max_distance = phi->get_Succ_Arc(i)->get_Distance();
                }
            }
        }
    }
    if (max_distance > -1)
    {
        temp_arc = new REGI_ARC(cur_id++, max_distance, TrueDep);
        temp_arc->Set_From_Node(this);
        temp_arc->Set_To_Node(this);
        this->set_self_loop(temp_arc);
        to_add.push_back(temp_arc);
    }
    //incomming arcs
    //remove arcs ends in left and change them so that they end in this (we do not touch left incomming arcs)
    temp_nodes = left->Get_Prev_Nodes();
    for (int i = 0; i < (int) temp_nodes.size(); i++)
    {
        temp_nodes[i]->remove_Succ_Arc(left->get_Pred_Arc(i));
        to_remove.push_back(left->get_Pred_Arc(i));
        if (temp_nodes[i] == phi)
        {
            continue;
        }
        if (!temp_nodes[i]->is_Connected_To(this))
        {
            temp_arc = new REGI_ARC(cur_id++, left->get_Pred_Arc(i)->get_Distance(), left->get_Pred_Arc(i)->get_Dependency());
            temp_arc->Set_From_Node(temp_nodes[i]);
            temp_arc->Set_To_Node(this);
            this->add_Pred_Arc(temp_arc);
            temp_nodes[i]->add_Succ_Arc(temp_arc);
            to_add.push_back(temp_arc);
        }
    }
    //remove arcs ends in right and change them so that they end in this (we do not touch right incomming arcs)
    temp_nodes = right->Get_Prev_Nodes();
    for (int i = 0; i < (int) temp_nodes.size(); i++)
    {
        temp_nodes[i]->remove_Succ_Arc(right->get_Pred_Arc(i));
        to_remove.push_back(right->get_Pred_Arc(i));
        if (temp_nodes[i] == phi)
        {
            continue;
        }
        if (!temp_nodes[i]->is_Connected_To(this))
        {
            temp_arc = new REGI_ARC(cur_id++, right->get_Pred_Arc(i)->get_Distance(), right->get_Pred_Arc(i)->get_Dependency());
            temp_arc->Set_From_Node(temp_nodes[i]);
            temp_arc->Set_To_Node(this);
            this->add_Pred_Arc(temp_arc);
            temp_nodes[i]->add_Succ_Arc(temp_arc);
            to_add.push_back(temp_arc);
        }
    }
    //remove arcs ends in phi and change them so that they end in this (we do not touch phi incomming arcs)
    temp_nodes = phi->Get_Prev_Nodes();
    for (int i = 0; i < (int) temp_nodes.size(); i++)
    {

        to_remove.push_back(phi->get_Pred_Arc(i));
        if (temp_nodes[i] == left || temp_nodes[i] == right)
            continue;
        temp_nodes[i]->remove_Succ_Arc(phi->get_Pred_Arc(i));
        if (!temp_nodes[i]->is_Connected_To(this))
        {
            temp_arc = new REGI_ARC(cur_id++, phi->get_Pred_Arc(i)->get_Distance(), phi->get_Pred_Arc(i)->get_Dependency());
            temp_arc->Set_From_Node(temp_nodes[i]);
            temp_arc->Set_To_Node(this);
            this->add_Pred_Arc(temp_arc);
            temp_nodes[i]->add_Succ_Arc(temp_arc);
            to_add.push_back(temp_arc);
        }
    }

    //remove arcs start from left and change them so that they start in this (we do not touch left outgoing arcs)
    temp_nodes = left->Get_Next_Nodes();
    for (int i = 0; i < (int) temp_nodes.size(); i++)
    {
        to_remove.push_back(left->get_Succ_Arc(i));
        if (temp_nodes[i] == phi)
            continue;
        temp_nodes[i]->remove_Succ_Arc(left->get_Succ_Arc(i));
        if (!is_Connected_To(temp_nodes[i]))
        {
            temp_arc = new REGI_ARC(cur_id++, left->get_Succ_Arc(i)->get_Distance(), left->get_Succ_Arc(i)->get_Dependency());
            temp_arc->Set_From_Node(this);
            temp_arc->Set_To_Node(temp_nodes[i]);
            temp_nodes[i]->add_Pred_Arc(temp_arc);
            this->add_Succ_Arc(temp_arc);
            to_add.push_back(temp_arc);
        }
    }
    //remove arcs start from right and change them so that they start in this (we do not touch right outgoing arcs)
    temp_nodes = right->Get_Next_Nodes();
    for (int i = 0; i < (int) temp_nodes.size(); i++)
    {
        to_remove.push_back(right->get_Succ_Arc(i));
        if (temp_nodes[i] == phi)
            continue;
        temp_nodes[i]->remove_Succ_Arc(right->get_Succ_Arc(i));
        if (!is_Connected_To(temp_nodes[i]))
        {
            temp_arc = new REGI_ARC(cur_id++, right->get_Succ_Arc(i)->get_Distance(), right->get_Succ_Arc(i)->get_Dependency());
            temp_arc->Set_From_Node(this);
            temp_arc->Set_To_Node(temp_nodes[i]);
            temp_nodes[i]->add_Pred_Arc(temp_arc);
            this->add_Succ_Arc(temp_arc);
            to_add.push_back(temp_arc);
        }
    }
    //remove arcs start from phi and change them so that they start in this (we do not touch phi outgoing arcs)
    temp_nodes = phi->Get_Next_Nodes();
    for (int i = 0; i < (int) temp_nodes.size(); i++)
    {
        temp_nodes[i]->remove_Succ_Arc(phi->get_Succ_Arc(i));
        to_remove.push_back(phi->get_Succ_Arc(i));
        if (temp_nodes[i]==left || temp_nodes[i]==right)
        {
            continue;
        }
        if (!is_Connected_To(temp_nodes[i]))
        {
            temp_arc = new REGI_ARC(cur_id++, phi->get_Succ_Arc(i)->get_Distance(), phi->get_Succ_Arc(i)->get_Dependency());
            temp_arc->Set_From_Node(this);
            temp_arc->Set_To_Node(temp_nodes[i]);
            temp_nodes[i]->add_Pred_Arc(temp_arc);
            this->add_Succ_Arc(temp_arc);
            to_add.push_back(temp_arc);
        }
    }

}

REGI_Reduced_Node::REGI_Reduced_Node(int id, REGI_Node* left, REGI_Node* right, REGI_Node* next, vector<REGI_ARC*>&to_remove, vector<REGI_ARC*>&to_add,
        bool dummy) :
    REGI_Node(reduced_op, max(left->get_Latency(), right->get_Latency()), id)
{
    this->left = left;
    this->right = right;
    int cur_id = id + 1;
    vector<REGI_Node*> temp_nodes;
    vector<REGI_ARC*> temp_arcs;
    REGI_ARC* temp_arc;
    int max_distance = -1;
    //first we fix loop//if any of those nodes have loop this node should have loop
    if (left->has_self_loop())
    {

        max_distance = left->get_self_loop()->get_Distance();
    }
    if (right->has_self_loop())
    {
        if (right->get_self_loop()->get_Distance() > max_distance)
            max_distance = right->get_self_loop()->get_Distance();
    }

    if (max_distance > -1)
    {
        temp_arc = new REGI_ARC(cur_id++, max_distance, TrueDep);
        temp_arc->Set_From_Node(this);
        temp_arc->Set_To_Node(this);
        this->set_self_loop(temp_arc);
        to_add.push_back(temp_arc);
    }

    //remove arcs ends in left and change them so that they end in this (we do not touch left incomming arcs)
    temp_nodes = left->Get_Prev_Nodes();
    for (int i = 0; i < (int) temp_nodes.size(); i++)
    {
        temp_nodes[i]->remove_Succ_Arc(left->get_Pred_Arc(i));
        to_remove.push_back(left->get_Pred_Arc(i));
        if (!temp_nodes[i]->is_Connected_To(this))
        {
            temp_arc = new REGI_ARC(cur_id++, left->get_Pred_Arc(i)->get_Distance(), left->get_Pred_Arc(i)->get_Dependency());
            temp_arc->Set_From_Node(temp_nodes[i]);
            temp_arc->Set_To_Node(this);
            this->add_Pred_Arc(temp_arc);
            temp_nodes[i]->add_Succ_Arc(temp_arc);
            to_add.push_back(temp_arc);
        }
    }
    //remove arcs ends in right and change them so that they end in this (we do not touch right incomming arcs)
    temp_nodes = right->Get_Prev_Nodes();
    for (int i = 0; i < (int) temp_nodes.size(); i++)
    {
        temp_nodes[i]->remove_Succ_Arc(right->get_Pred_Arc(i));
        to_remove.push_back(right->get_Pred_Arc(i));
        if (!temp_nodes[i]->is_Connected_To(this))
        {
            temp_arc = new REGI_ARC(cur_id++, right->get_Pred_Arc(i)->get_Distance(), right->get_Pred_Arc(i)->get_Dependency());
            temp_arc->Set_From_Node(temp_nodes[i]);
            temp_arc->Set_To_Node(this);
            this->add_Pred_Arc(temp_arc);
            temp_nodes[i]->add_Succ_Arc(temp_arc);
            to_add.push_back(temp_arc);
        }
    }

    //remove arcs start from left and change them so that they start in this (we do not touch left outgoing arcs)
    temp_nodes = left->Get_Next_Nodes();
    for (int i = 0; i < (int) temp_nodes.size(); i++)
    {
        temp_nodes[i]->remove_Succ_Arc(left->get_Succ_Arc(i));
        to_remove.push_back(left->get_Succ_Arc(i));
        if (!is_Connected_To(temp_nodes[i]))
        {
            temp_arc = new REGI_ARC(cur_id++, left->get_Succ_Arc(i)->get_Distance(), left->get_Succ_Arc(i)->get_Dependency());
            temp_arc->Set_From_Node(this);
            temp_arc->Set_To_Node(temp_nodes[i]);
            temp_nodes[i]->add_Pred_Arc(temp_arc);
            this->add_Succ_Arc(temp_arc);
            to_add.push_back(temp_arc);
        }
    }
    //remove arcs start from right and change them so that they start in this (we do not touch right outgoing arcs)
    temp_nodes = right->Get_Next_Nodes();
    for (int i = 0; i < (int) temp_nodes.size(); i++)
    {
        temp_nodes[i]->remove_Succ_Arc(right->get_Succ_Arc(i));
        to_remove.push_back(right->get_Succ_Arc(i));
        if (!is_Connected_To(temp_nodes[i]))
        {
            temp_arc = new REGI_ARC(cur_id++, right->get_Succ_Arc(i)->get_Distance(), right->get_Succ_Arc(i)->get_Dependency());
            temp_arc->Set_From_Node(this);
            temp_arc->Set_To_Node(temp_nodes[i]);
            temp_nodes[i]->add_Pred_Arc(temp_arc);
            this->add_Succ_Arc(temp_arc);
            to_add.push_back(temp_arc);
        }
    }

}

vector<REGI_Node*> REGI_Reduced_Node::Get_Left_Inputs_Sorted_by_Arc()
{
    vector<REGI_Node*> retVal = left->Get_Prev_Nodes();
    REGI_Node* temp;
    for (int i = 0; i < (int) retVal.size(); i++)
    {
        for (int j = i + 1; j < (int) retVal.size(); j++)
        {
            if (retVal[i]->get_Number_of_Pred() < retVal[j]->get_Number_of_Pred())
            {
                temp = retVal[i];
                retVal[i] = retVal[j];
                retVal[j] = temp;
            }
        }
    }
    return retVal;

}
vector<REGI_Node*> REGI_Reduced_Node::Get_Right_Inputs_Sorted_by_Arc()
{
    vector<REGI_Node*> retVal = right->Get_Prev_Nodes();
    REGI_Node* temp;
    for (int i = 0; i < (int) retVal.size(); i++)
    {
        for (int j = i + 1; j < (int) retVal.size(); j++)
        {
            if (retVal[i]->get_Number_of_Pred() < retVal[j]->get_Number_of_Pred())
            {
                temp = retVal[i];
                retVal[i] = retVal[j];
                retVal[j] = temp;
            }
        }
    }
    return retVal;
}
REGI_PE::REGI_PE()
{
    busy = false;
    this->iIndex = 0;
    this->jIndex = 0;
    this->tIndex = 0;
    RegFile = NULL;
    ops = (REGI_Operations) 0;
    node = NULL;
    NoMappedOps = 0;

}

REGI_PE::~REGI_PE()
{
}

//****************************GeoS****************************//

void REGI_PE::SetIndex(int iIndex, int jIndex, int tIndex, int RegLen)
{

    this->iIndex = iIndex;
    this->jIndex = jIndex;
    this->tIndex = tIndex;
    RegFile = new REGI_REGISTER_FILE(RegLen);
}

int REGI_PE::getIndexI()
{
    return iIndex;

}

int REGI_PE::getIndexJ()
{
    return jIndex;
}

int REGI_PE::getIndexT()
{
    return tIndex;

}

bool REGI_PE::SameCoordination(REGI_PE* nPE)
{
    if (iIndex == nPE->getIndexI() && jIndex == nPE->getIndexJ())
    {
        return true;
    }
    return false;
}
//****************************GeoE****************************//

//****************************MappingS****************************//

void REGI_PE::AssignNode(REGI_Node *node)
{

    this->node = node;
    this->node->Assign2PE(this);
    NoMappedOps++;
    busy = true;
}

bool REGI_PE::isRegisterAvailable()
{
    return RegFile->isAvailable();
}

int REGI_PE::NumberOfAvailableRegisters()
{
    return RegFile->getAvailableNumber();
}
int REGI_PE::NumberOfUsedRegisters()
{
    return RegFile->get_No_Used_Register();
}

void REGI_PE::OccupyRegister()
{
    //cout<< "PE at:("<<iIndex<<","<<jIndex<<","<<tIndex<<") Uses Reg, # of available reg is"<<RegFile->getAvailableNumber()<<endl;
    RegFile->Assign();

}
void REGI_PE::AllocateRegister(int n)
{
    RegFile->Allocate(n);
}
bool REGI_PE::ReleaseRegister()
{
    //cout<< "PE at:("<<iIndex<<","<<jIndex<<","<<tIndex<<") Release Reg, # of available reg is"<<RegFile->getAvailableNumber()<<endl;
    return RegFile->Reset();

}
void REGI_PE::ReleaseRegister(int i)
{
    //cout<< "PE at:("<<iIndex<<","<<jIndex<<","<<tIndex<<") Release Reg, # of available reg is"<<RegFile->getAvailableNumber()<<endl;
    RegFile->Release(i);

}
bool REGI_PE::isBusy()
{
    return busy;
}

void REGI_PE::Reset()
{
    this->node->Reset();
    //	this->node = NULL;
    busy = false;
}

REGI_Node* REGI_PE::getNode()
{
    return node;
}

bool REGI_PE::isAbleExecute(REGI_Node* node, int II)
{

    if ((node->get_Sched_Info()->get_Modulo_Current()) != this->getIndexT())
    {
        return false;

    }
    return true;
}

//****************************MappingE****************************//

bool REGI_PE::isOverUsed()
{
    if (NoMappedOps > 1)
        return true;
    return false;
}
int REGI_PE::getOverUsedNumber()
{
    return NoMappedOps;
}

void REGI_PE::ReleasePE()
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
MappingPair::MappingPair(REGI_Node* inNode, REGI_PE* inPE)
{
    this->node = inNode;
    this->pe = inPE;
}

REGI_Node* MappingPair::getNode()
{
    return node;
}

REGI_PE* MappingPair::getPE()
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

TimeCGRA::TimeCGRA(int SizeX, int SizeY, int RegSize, int time, REGI_ConnectionType ConType)
{
    this->SizeX = SizeX;
    this->SizeY = SizeY;
    this->time = time;
    PEs = new REGI_PE*[SizeX];
    for (int i = 0; i < SizeX; i++)
    {

        PEs[i] = new REGI_PE[SizeY];

    }
    for (int i = 0; i < SizeX; i++)
    {
        for (int j = 0; j < SizeY; j++)
        {
            PEs[i][j].SetIndex(i, j, time, RegSize);
        }
    }
    this->ConType = ConType;

}

TimeCGRA::~TimeCGRA()
{
    if (PEs != NULL)
    {

        delete[] PEs;

    }
}

void TimeCGRA::Reset()
{
    for (int i = 0; i < SizeX; i++)
    {
        for (int j = 0; j < SizeY; j++)
        {
            PEs[i][j].Reset();
        }
    }
}

REGI_PE* TimeCGRA::getPE(int i, int j)

{
    return &PEs[i][j];
}
