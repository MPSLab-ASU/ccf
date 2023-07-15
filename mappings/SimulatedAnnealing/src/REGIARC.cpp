/*
 * REGIARC.cpp
 *
 *  Created on: Jul 24, 2012
 *      Author: mahdi
 */

#include "REGIARC.h"

REGI_ARC::REGI_ARC(int number, int distance, DataDepType dep)
{
    this->number = number;
    this->distance = distance;
    from = NULL;
    to = NULL;
    dependency = dep;
}
REGI_ARC::~REGI_ARC()
{
}

REGI_ARC* REGI_ARC::Copy()
{
    REGI_ARC* new_ARC = new REGI_ARC(number, distance,dependency);
    new_ARC->Set_From_Node(from);
    new_ARC->Set_To_Node(to);
    new_ARC->set_path(path);
    return new_ARC;
}
//Set the node that this edge is its outgoing edge
void REGI_ARC::Set_From_Node(REGI_Node* from)
{
    this->from = from;
}
//Set the node that this edge is its incoming edge
void REGI_ARC::Set_To_Node(REGI_Node* to)
{
    this->to = to;
}

//Set edge distance
void REGI_ARC::Set_Distance(int distance)
{
    this->distance = distance;
}

//Get the node that this edge is its outgoing edge
REGI_Node* REGI_ARC::get_From_Node()
{
    return from;
}
//Get the node that this edge is its incoming edge
REGI_Node* REGI_ARC::get_To_Node()
{
    return to;
}
//Return Edge ID
int REGI_ARC::get_Number()
{
    return number;
}
//Return edge distance
int REGI_ARC::get_Distance()
{
    return distance;
}

DataDepType REGI_ARC::get_Dependency()
{
    return dependency;
}
void REGI_ARC::set_path(vector<REGI_Node*> newPath)
{
    path.clear();
    path = newPath;
}
void REGI_ARC::clear_path()
{
    path.clear();
}
vector<REGI_Node*> REGI_ARC::Get_Path()
{
    return path;
}
