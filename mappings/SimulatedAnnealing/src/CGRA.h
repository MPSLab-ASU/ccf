/*
 * CGRA.h
 *
 *  Created on: Aug 10, 2012
 *      Author: mahdi
 */

#ifndef CGRA_H_
#define CGRA_H_


#include "REGIDFG.h"
#include "Mapper.h"


class CGRA
{
    public:
        CGRA();
        virtual ~CGRA();

        //initialize CGRA with Xdim,Ydim, Register size, connection type
        void Initialize(int SizeX, int SizeY, int SizeReg, REGI_ConnectionType ConType);

        //Increase II, add a new set of PEs
        void IncreaseII();

        //set II
        void SetII(int II);

        //Print mapping results
        void ShowMapping();

        void DotPrintMapping(string filename);

        //Set the maximum possible tries during mapping
        //		void SetThreshold(int thres);

        //Reset Iteration number
        void ResetIteration();

        int getII();

        /********************************************Reg Aware Mapping S****************************************************/

        bool MCS2(REGI_DFG* myDFG, vector<REGI_Node*> &leftNotMapped);

        bool MCS3(REGI_DFG* myDFG, vector<REGI_Node*> &leftNotMapped);

        void Prolog(REGI_DFG* myDFG);

        void EPIlog(REGI_DFG* myDFG);

        REGI_Node* getNode(int tableIndex, vector<REGI_Node*> &nodes);

        REGI_PE* getPE(int tableIndex);

        //return true if given PEs have same X Y coordination
        bool SameCoordinatePEs(REGI_PE* pe1, REGI_PE* pe2);

        //return true if given PEs are connected
        bool connectedPEs(REGI_PE* pe1, REGI_PE* pe2);

        //return true if given nodes are connected
        bool connected_Nodes(REGI_Node* node1, REGI_Node* node2);

        int Compatible(MappingPair* mp1, MappingPair* mp2);

        int countNodes(char* tableAnd, vector<MappingPair*> &mapPairs, int index, int len);

        /********************************************Reg Aware Mapping E****************************************************/

        void Dot_Print_Comp_Graph(string filename, char *table, int size, vector<MappingPair*> &mapPairs);

        /******************************************Sim Annealing ******************************************/

        int Physical_Distance(REGI_PE* pe1, REGI_PE* pe2);

        /******************************************Sim Annealing ******************************************/

    private:
        int II;
        vector<TimeCGRA*> timeCGRAList;
        int SizeX;
        int SizeY;
        int SizeReg;
        REGI_ConnectionType ConType;
        int iteration;
        //int Threshold;

        /*******************************************Set Manipulate S****************************************************/

        //returns true if list1 contains given node
        bool contains(vector<REGI_Node*> &nodeset, REGI_Node* node);

        //returns true if list1 contains given mappingpair
        bool contains(vector<MappingPair*> &pairs, MappingPair* mp);

        //returns true if list1 contains given nodes in set2
        bool contains(vector<REGI_Node*> &nodeset1, vector<REGI_Node*> &nodeset2);
        /********************************************Reg Aware Mapping E****************************************************/
};

#endif /* CGRA_H_ */
