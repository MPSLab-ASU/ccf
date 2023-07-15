/*
* ARC.h
*
*  Created on: Jul 24, 2012
*      Author: mahdi
*
* Last edited on: 26 Nov 2016
* Author: Shail Dave
*/

#ifndef ARC_H_
#define ARC_H_

#include "definitions.h"

class Node;

class ARC
{
  public:
    ARC(int number, int distance, DataDepType dep, int oporder);
    virtual ~ARC();
    ARC* Copy();

  private:
    //edge from inNode to outNode
    Node* from;
    Node* to;

    vector<Node*> path;
    int number;
    int distance;
    DataDepType dependency;
    int operandorder;
  public:
    //Set the node that this edge is its outgoing edge
    void Set_From_Node(Node* inNode);
    //Set the node that this edge is its incoming edge
    void Set_To_Node(Node* outNode);

    //Set edge distance
    void Set_Distance(int distance);

    //Get the node that this edge is its outgoing edge
    Node* get_From_Node();
    //Get the node that this edge is its incoming edge
    Node* get_To_Node();
    //Return Edge ID
    int get_Number();
    //Return edge distance
    int get_Distance();
    int get_OperandOrder();
    DataDepType get_Dependency();

    void set_path(vector<Node*> newPath);
};

#endif /* ARC_H_ */
