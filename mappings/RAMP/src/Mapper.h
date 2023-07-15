/*
* Mapper.h
*
* Created on: Aug 10, 2012
* Author: mahdi
*
* Last edited on: 14 Jan 2017
* Author: Shail Dave
*/

#ifndef MAPPER_H_
#define MAPPER_H_

#include "definitions.h"
#include "Node.h"
#include <cstring>
#include <cassert>

class Cover_Pair
{
  public:
    //NodeID is the index in array mapPairs
    int NodeID;
    int REG;
};

class Mapper
{
  public:
    Mapper(int NoReg,vector<MappingPair*>* mapPairs,int XDim,int YDim);
    virtual ~Mapper();
    int Uncovered_size(vector<int> cover);

    vector<vector<Cover_Pair*> > MapWithRegisterFile(char *table, int tableSize, int cliqueSize);
    vector<int> procedure_1WithRegisterFile(vector<vector<int> > &neighbors, vector<int> cover, char *table, int tableSize);
    vector<int> procedure_2WithRegisterFile(vector<vector<int> > &neighbors, vector<int> cover, int k, char *table, int tableSize);
    bool removableWithRegisterFile(vector<int> &neighbor, vector<int> cover, char *table, int tableSize, int nodeIndex);
    int max_removableWithRegisterFile(vector<vector<int> > &neighbors, vector<int> cover, char *table, int tableSize);

    vector<vector<Cover_Pair*> > MapWithoutRegisterFile(char *table, int tableSize, int cliqueSize);
    vector<int> procedure_1WithoutRegisterFile(vector<vector<int> > &neighbors, vector<int> &cover, char *table, int tableSize);
    vector<int> procedure_2WithoutRegisterFile(vector<vector<int> > &neighbors, vector<int> &cover, int k, char *table, int tableSize);
    bool removableWithoutRegisterFile(vector<int> &neighbor, vector<int> &cover, char *table, int tableSize, int nodeIndex);
    int max_removableWithoutRegisterFile(vector<vector<int> > &neighbors, vector<int> &cover, char *table, int tableSize);

    //this function resets the array tracking the allocation of nonrotating registers
    //required to store nonrecurruing variables for mapping an operation to a PE
    //and initialilizes it based on the vertices of the current clique
    void resetNonrecVariableTracker(vector<int> cover);
    //this function returns the mapping pair corresponding to a vertex in
    //the clique
    MappingPair getMappingPair(int index);

    //this function returns the number of nonrotating registers required to map
    //an operation to a PE, for a mapping pair
    int num_nonrec_vars(int index);

    //this function checks whether there are enough non rotating registers
    //available for performing an address assert operation by this node
    bool enoughNonRotatingRegisters(int index);

    // this function manages the number of nonrecurring variables required
    // for every vertex included in the clique
    void occupyNonRotatingRegister(int index);
    void releaseNonRotatingRegister(int index);
    int getNonRotatingRegisterCount(int index);
    void InitializeNonrecVariableTable();
    int alignPowerOfTwo(int r);

    // array holding nonrecurring variables required for RF
    int* nonrecVar_PE;

  private:
    int NoRegisters;

    //pointer to the vector containing the mapping pairs
    vector<MappingPair*> *mapPairs;
    int XDim,YDim;

    //A hash table informs that whether operation of a mapping pair
    //requires nonrecurring variable or not
    bool* isMappeingPairWithNonrecVariable;

};

#endif /* MAPPER_H_ */
