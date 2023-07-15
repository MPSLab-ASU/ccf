/*
 * CGRA.h
 *
 *  Created on: Aug 10, 2012
 *  Author: mahdi
 *  Last Edited on: Jan 28, 2017
 *  Author: Shail Dave
 */

#ifndef CGRA_H_
#define CGRA_H_

#include "DFG.h"
#include "Mapper.h"
#include "matrix.h"

class CGRA
{
  public:
    CGRA();
    virtual ~CGRA();

    // Set CGRA target parameters with Xdim,Ydim, Register size, inter-connect type
    void Initialize(int SizeX, int SizeY, int SizeReg, ConnectionType ConType);

    //set II
    void SetII(int II);

    //Print mapping results
    void ShowMapping();

    // Reset iteration number
    void ResetIteration();

    int getII();

    bool MCS2(DFG* myDFG, vector<Node*> &leftNotMapped);

    void Prolog(DFG* myDFG, int highest_distance, int node_with_highest_distance);
    void EPIlog(DFG* myDFG);

    Node* getNode(int tableIndex, vector<Node*> &nodes);
    PE* getPE(int tableIndex);

    //return true if given PEs have same X Y coordination
    bool SameCoordinatePEs(PE* pe1, PE* pe2);

    //return true if given PEs are connected
    bool connectedPEs(PE* pe1, PE* pe2);

    //return true if given nodes are connected
    bool connected_Nodes(Node* node1, Node* node2);

    int Compatible(MappingPair* mp1, MappingPair* mp2, DFG* &myDFG);

    int Physical_Distance(PE* pe1, PE* pe2);
    void Insert_TimeCGRA(TimeCGRA* newCGRA);

    CGRA Copy();
    void CalcPEUtil(float &PEUtil);
    bool IsCompatible(int node1, int node2);
    void ShowCompatibility();	//Print Compatibility Table

    void makeRFTable();
    void getMinMaxReg(int &Rmin, int &Rmax);

    void GenerateCompatibilityTable(DFG* myDFG);

    void UpdateNeedToLoadConstants(DFG* myDFG);
    void UpdateIntraIterationDepReg(DFG* myDFG);
    void UpdateInterIterationDepReg(DFG* myDFG);
    void UpdateSelfReg(DFG* myDFG);

    void GenerateCompGraph(DFG* myDFG);

     // GraphMinor functions
    bool  MinorUtils(vector<Node*> to_be_mapped, vector<Node*> &M, int tableSize, vector<MappingPair*> &mappingPairs);

    tuple<vector<Node*>, vector<Node*> >  get_Mapped_pred_succ(Node* v);

    vector<MappingPair*>  min_map(Node* v, vector<Node*> pred, vector<Node*> succ, int tableSize, vector<MappingPair*> mappedPairs);

    bool  isTECfree(int X, int Y, int sched);

    void  UnAssignTEC(int posX, int posY, int sched_time, int node_id);

    void  AssignTEC(int posX, int posY, int sched_time, int node_id);

    bool  Minor(DFG* myDFG, vector<Node*> &leftNotMapped);

    vector<Node*>  Subtract(vector<Node*> &set1, vector<Node*> &set2);

    Node* get_Node(int number);


  private:
    int II;
    vector<TimeCGRA*> timeCGRAList;
    int SizeX;
    int SizeY;
    int SizeReg;
    ConnectionType ConType;
    int iteration;
    int Scc_iterator = 0 ;

    vector<Node*> _node_Set; 
    //returns true if list1 contains given node
    bool contains(vector<Node*> &nodeset, Node* node);

    //returns true if list1 contains given mappingpair
    bool contains(vector<MappingPair*> &pairs, MappingPair* mp);

    bool containsPE(vector<MappingPair*> &pairs, MappingPair* mp);
    //returns true if list1 contains given nodes in set2
    bool contains(vector<Node*> &nodeset1, vector<Node*> &nodeset2);

    //All valid mapping pairs (operation, resource) for compatibility graph
    vector<MappingPair*> mapPairs;

    // the adjacency of the input DFG
    Matrix<int> A_g;

    // the adjacency of product graph
    Matrix<int> A_h;

    // the intermediate matrix for ullmann
    Matrix<int> M;

    //The time extended CGRA. After a node is mapped it is updated in this matrix.
    Matrix<int> TEC;
    char* table; //Table for compatibility graph (adjacency matrix for the compatibility graph with all valid nodes)
    char* rf_table;
    char* kernel_table;
    char* table_without_r;

    void dumpRFConfig(std::string filename);
    int* numberOfNonrotatingRegPerPE;
    unsigned int* totalRotatingReg;
    unsigned int* rfConfig;
    void calcRFConfig();
};

#endif /* CGRA_H_ */
