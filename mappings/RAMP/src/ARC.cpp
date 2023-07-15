/*
 * ARC.cpp
 *
 *  Created on: Jul 24, 2012
 *      Author: mahdi
 *
 * Last edited on: 26 Nov 2016
 * Author: Shail Dave
 */

#include "ARC.h"

ARC::ARC(int number, int distance, DataDepType dep, int oporder)
{
    this->number = number;
    this->distance = distance;
    from = NULL;
    to = NULL;
    dependency = dep;
    this->operandorder = oporder;
}

ARC::~ARC()
{
}

ARC* ARC::Copy()
{
    ARC* new_ARC = new ARC(number, distance,dependency,operandorder);
    new_ARC->Set_From_Node(from);
    new_ARC->Set_To_Node(to);
    new_ARC->set_path(path);
    return new_ARC;
}

//Set the node that this edge is its outgoing edge
void ARC::Set_From_Node(Node* from)
{
    this->from = from;
}

//Set the node that this edge is its incoming edge
void ARC::Set_To_Node(Node* to)
{
    this->to = to;
}

//Set edge distance
void ARC::Set_Distance(int distance)
{
    this->distance = distance;
}

//Get the node that this edge is its outgoing edge
Node* ARC::get_From_Node()
{
    return from;
}

//Get the node that this edge is its incoming edge
Node* ARC::get_To_Node()
{
    return to;
}

//Return Edge ID
int ARC::get_Number()
{
    return number;
}

//Return edge distance
int ARC::get_Distance()
{
    return distance;
}

DataDepType ARC::get_Dependency()
{
    return dependency;
}

void ARC::set_path(vector<Node*> newPath)
{
    path.clear();
    path = newPath;
}

int ARC::get_OperandOrder()
{
    return operandorder;
}
