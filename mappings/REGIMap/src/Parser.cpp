/*
 * Parser.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: mahdi
 */

#include "Parser.h"

Parser::Parser(string nodeFile, string edgeFile)
{
    this->nodeFile = nodeFile;
    this->edgeFile = edgeFile;

}

Parser::~Parser()
{
    // TODO Auto-generated destructor stub
}

bool Parser::ParseDFG(REGI_DFG* myDFG)
{
    char line[255];
    int node_type;
    char edge_type[3];
    int inN, outN;
    int distance;

    ifstream fin_node;
    ifstream fin_edge;
    ifstream fin_enf;

    Instruction_Operation temp_ins = add_op;

    string temp;

    REGI_Node *pNode;

    int nodeID;
    int arcID = 0;

    bool node_exists=false;

    // Node File
    fin_node.open(nodeFile.c_str());
    if (fin_node.is_open())
    {
        while (fin_node.getline(&line[0], 255, '\n'))
        {

            node_type=0;
            // Node File temp

            istringstream strout(line);
            strout >> nodeID >> node_type;

            temp_ins=(Instruction_Operation)node_type;
            node_exists = myDFG->has_Node(nodeID);
            if (!node_exists)
            {
                pNode = new REGI_Node(temp_ins, 1, nodeID);
                if (temp_ins==constant_op)
                {
                    myDFG->insert_Constant(pNode);
                }
                else
                {
                    myDFG->insert_Node(pNode);
                }
            }
        }
    }
    else
    {
        cout << "Node file not found\n";
        return false;
    }
    fin_node.close();
    temp_ins=add_op;
    // Edge File
    fin_edge.open(edgeFile.c_str());
    REGI_Node* node1, *node2;
    if (fin_edge.is_open())
    {
        while (fin_edge.getline(&line[0], 100, '\n'))
        {
            distance = 0;
            edge_type[0] = 0;
            istringstream strout(line);

            strout >> inN >> outN >> distance >> edge_type;
            if (!strcmp(edge_type, "I"))
            {
                continue;
            }
            if (myDFG->has_Constant(inN)|| myDFG->has_Constant(outN))
            {
                continue;
            }
            node_exists= myDFG->has_Node(inN);
            if (!node_exists)
            {
                node1 = new REGI_Node(temp_ins, 1, inN);
                myDFG->insert_Node(node1);
            }
            else
            {
                node1 = myDFG->get_Node(inN);
            }

            node_exists = myDFG->has_Node(outN);
            if (!node_exists)
            {
                node2 = new REGI_Node(temp_ins, 1, outN);
                myDFG->insert_Node(node2);
            }
            else
            {
                node2 = myDFG->get_Node(outN);
            }

            if (!strcmp(edge_type, "LRE"))
            {
                node1->set_Load_Address_Generator(node2);
                node2->set_Load_Data_Bus_Read(node1);
                myDFG->make_Arc(node1, node2, arcID++, 0, LoadDep);

            }
            else if (!strcmp(edge_type, "SRE"))
            {
                node1->set_Store_Address_Generator(node2);
                node2->set_Store_Data_Bus_Write(node1);
                myDFG->make_Arc(node1, node2, arcID++, 0, StoreDep);

            }
            else if (!strcmp(edge_type, "PRE"))
            {
                myDFG->make_Arc(node1, node2, arcID++, distance, PredDep);
                node2->set_Instruction(select_op,node2->get_Latency(),node2->get_ID());

            }
            else if (!strcmp(edge_type, "MEM"))
            {
                myDFG->make_Arc(node1, node2, arcID++, distance, MemoryDep);
            }
            else
            {
                //todo
                myDFG->make_Arc(node1, node2, arcID++, distance, TrueDep);

            }

        }
    }
    else
    {
        cout << "Edge file was not found\n";
        return false;
    }

    fin_edge.close();

    return true;
}
