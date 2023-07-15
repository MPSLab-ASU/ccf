/*
 * REGIARC.h
 *
 *  Created on: Jul 24, 2012
 *      Author: mahdi
 */
//#include "REGINode.h"

#ifndef REGIARC_H_
#define REGIARC_H_

#include "definitions.h"

class REGI_Node;



class REGI_ARC
{
    public:
        REGI_ARC(int number, int distance, DataDepType dep);
        virtual ~REGI_ARC();
        REGI_ARC* Copy();

    private:
        //edge from inNode to outNode
        REGI_Node* from;
        REGI_Node* to;

        vector<REGI_Node*> path;
        int number;
        int distance;
        DataDepType dependency;
    public:
        //Set the node that this edge is its outgoing edge
        void Set_From_Node(REGI_Node* inNode);
        //Set the node that this edge is its incoming edge
        void Set_To_Node(REGI_Node* outNode);

        //Set edge distance
        void Set_Distance(int distance);

        //Get the node that this edge is its outgoing edge
        REGI_Node* get_From_Node();
        //Get the node that this edge is its incoming edge
        REGI_Node* get_To_Node();
        //Return Edge ID
        int get_Number();
        //Return edge distance
        int get_Distance();

        DataDepType get_Dependency();

        void set_path(vector<REGI_Node*> newPath);
        void clear_path();
        vector<REGI_Node*> Get_Path();
};

#endif /* REGIARC_H_ */
