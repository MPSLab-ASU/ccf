/*
 * Mapper.h
 *
 *  Created on: Aug 10, 2012
 *      Author: mahdi
 */

#ifndef MAPPER_H_
#define MAPPER_H_


#include "definitions.h"

class Cover_Pair
{
    public:
        int NodeID;
        int REG;
};

class Mapper
{
    public:
        Mapper(int NoReg);
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

    private:
        int NoRegisters;

};

#endif /* MAPPER_H_ */
