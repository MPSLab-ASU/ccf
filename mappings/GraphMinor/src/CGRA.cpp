/*
 * CGRA.cpp
 *
 *  Created on: Aug 10, 2012
 *  Author: mahdi
 *  Last Edited on: Jan 28, 2017
 *  Author: Shail Dave
 */

#include "CGRA.h"
#include "INTERITERATIONDEPENDENCY.h"
#include <math.h>

extern int map_id;

CGRA::CGRA()
{
  iteration = 0;
  SizeReg = 0;
  SizeX = 0;
  SizeY = 0;
  II = 0;
  ConType = (ConnectionType) 0;

}

CGRA::~CGRA()
{
}

void CGRA::Initialize(int SizeX, int SizeY, int SizeReg, ConnectionType ConType)
{
  this->SizeX = SizeX;
  this->SizeY = SizeY;
  this->SizeReg = SizeReg;
  this->ConType = ConType;
}

void CGRA::SetII(int II)
{
  this->II = II;
  TimeCGRA* newCGRA;

  if(timeCGRAList.size() > 0)
    timeCGRAList.erase(timeCGRAList.begin(),timeCGRAList.end());

  //PEs in the first timing slot are special, keep track of register
  for (int i = 0; i < II; i++)
  {
    newCGRA = new TimeCGRA(SizeX, SizeY, SizeReg, i, ConType);
    timeCGRAList.push_back(newCGRA);
  }
}

void CGRA::ShowMapping()
{
  totalRotatingReg = new unsigned int [SizeX*SizeY];

  std::ofstream kernelFile;
  std::string kernel = "kernel";
  kernel.append(".sch");
  kernelFile.open(kernel.c_str());
  printf("*********************************Kernel Start*********************************\n");
  for (int timeIndex = 0; timeIndex < II; timeIndex++)
  {

    TimeCGRA* cgraTime = timeCGRAList[timeIndex];
    PE* candidatePE;
    cout << "Time:" << timeIndex << endl;
    for (int i = 0; i < SizeX; i++)
    {
      for (int j = 0; j < SizeY; j++)
      {
        if(timeIndex == 0) totalRotatingReg[i*SizeY + j] = 0;
        candidatePE = cgraTime->getPE(i, j);
        if (candidatePE->isBusy())
        {
          int usedReg = candidatePE->NumberOfUsedRegisters();
          if((usedReg < 0) || (usedReg > SizeReg))
            usedReg = 0;
          printf("%10d(%d)", candidatePE->getNode()->get_ID(), usedReg);
          kernelFile << candidatePE->getNode()->get_ID()<<"\t"<< usedReg <<"\t"<< candidatePE->getNode()->get_Sched_Info()->get_Current() << "\n";
          if(((unsigned)usedReg) > totalRotatingReg[i*SizeY + j])
            totalRotatingReg[i*SizeY + j] = usedReg;
        }
        else
        {
          kernelFile << "-1\t0\t0" << "\n";
          printf("         F");
        }
        printf("\t");
      }
      printf("\n");
    }
  }
  printf("*********************************Kernel End*********************************\n");
  kernelFile << II*SizeX*SizeY << "\n";
  kernelFile.close();

  dumpRFConfig("rfConfig.txt");
}

void CGRA::calcRFConfig()
{
  rfConfig = new unsigned int [SizeX*SizeY];

  if(ROTATING_REG_AS_POW_OF_2)
  {
    for (int i = 0; i < SizeX; i++)
      for (int j = 0; j < SizeY; j++)
      {
        int diff = totalRotatingReg[i*SizeY + j];
        if((diff > 2) && (diff & (diff - 1))) {
          rfConfig[i*SizeY + j] = pow(2, ceil(log(diff)/log(2)));

          //In case upscaled unnecessarily. It's hack for few cases.
          if((rfConfig[i*SizeY + j] + numberOfNonrotatingRegPerPE[i*SizeY + j]) > (unsigned) SizeReg)
            rfConfig[i*SizeY + j]= rfConfig[i*SizeY + j]/2;
        }
        else
          rfConfig[i*SizeY + j] = diff;
      }
  }
  else
  {
    for (int i = 0; i < SizeX; i++)
      for (int j = 0; j < SizeY; j++)
        rfConfig[i*SizeY + j] = totalRotatingReg[i*SizeY + j];
  }
}

void CGRA::dumpRFConfig(std::string filename)
{
  if(SizeReg == 0)  return;

  calcRFConfig();

  std::ofstream rfConfigFile;
  rfConfigFile.open(filename.c_str());

  for (int i = 0; i < SizeX; i++)
    for (int j = 0; j < SizeY; j++)
      rfConfigFile << (i*SizeY + j) << "\t" << rfConfig[i*SizeY + j] << "\n";

  rfConfigFile.close();
}

void CGRA::ResetIteration()
{
  iteration = 0;
}

int CGRA::getII()
{
  return II;
}

// Print out compatibility table
void CGRA::GenerateCompatibilityTable(DFG* myDFG)
{
  vector<Node*> nodes = myDFG->getSetOfVertices();

  int nodeSize = (int) nodes.size();
  int* compTable = new int[nodeSize * II * SizeX * SizeY];

  //compatibility table
  Node* tempNode;
  PE* tempPE;
  for (int i = 0; i < nodeSize; i++)
  {
    tempNode = nodes[i];
    for (int j = 0; j < II * SizeX * SizeY; j++)
    {
      tempPE = getPE(j);
      if (tempPE->isAbleExecute(tempNode, II))
      {
        compTable[i * II * SizeX * SizeY + j] = 1;
      }
      else
      {
        compTable[i * II * SizeX * SizeY + j] = 0;
      }
    }
  }

  MappingPair* mp;
  Node* node1;
  mapPairs.clear();
  PE* pe1;

  //Generate the mapping pairs for the compatibility graph
  for (int i1 = 0; i1 < (int) nodes.size(); i1++)
  {
    for (int j1 = 0; j1 < II * SizeX * SizeY; j1++)
    {
      if (compTable[i1 * II * SizeX * SizeY + j1] == 0)
        continue;
      node1 = getNode(i1, nodes);
      pe1 = getPE(j1);
      mp = new MappingPair(node1, pe1);
      if (!contains(mapPairs, mp))
      {
        mapPairs.push_back(mp);
      }
    }
  }
  int tableSize = (int) mapPairs.size();

  //adjacency matrix for the compatibility graph
  table = new char[tableSize * tableSize];
  table_without_r = new char[tableSize * tableSize];

  MappingPair* mp1;
  MappingPair* mp2;

  // Create Directed Binary Arcs Between Nodes Of The Compatibility Graph
  for (int i = 0; i < tableSize; i++)
  {
    mp1 = mapPairs[i];

    for (int j = 0; j < tableSize; j++)
    {
      if (i == j)
      {
        table[i * tableSize + j] = 1;
        table_without_r[i * tableSize + j] = 1;
      }
      else
      {
        mp2 = mapPairs[j];
        table[i * tableSize + j] = Compatible(mp1, mp2, myDFG);
        table_without_r[i * tableSize + j] = Compatible(mp1, mp2, myDFG);
      }
    }
  }
}


/*  template <typename T>
    bool CGRA::GenerateAdjacencyTable(DFG* myDFG, Matrix<T>& adj_h,  Matrix<T>& adj_g, Matrix<T>& m)
    {
    vector<Node*> nodes = myDFG->getSetOfVertices();

    int nodeSize = (int) nodes.size();
    int* compTable = new int[nodeSize * II * SizeX * SizeY];

//compatibility table
Node* tempNode;
PE* tempPE;
for (int i = 0; i < nodeSize; i++)
{
tempNode = nodes[i];
for (int j = 0; j < II * SizeX * SizeY; j++)
{
tempPE = getPE(j);
if (tempPE->isAbleExecute(tempNode, II))
{
compTable[i * II * SizeX * SizeY + j] = 1;
}
else
{
compTable[i * II * SizeX * SizeY + j] = 0;
}
}
}

cout << "passed compatible table: " << (int) nodes.size() * II * SizeX * SizeY << endl; 

MappingPair* mp;
Node* node1;
mapPairs.clear();
PE* pe1;

//Generate the mapping pairs for the compatibility graph
for (int i1 = 0; i1 < (int) nodes.size(); i1++)
{
for (int j1 = 0; j1 < II * SizeX * SizeY; j1++)
{
if (compTable[i1 * II * SizeX * SizeY + j1] == 0)
continue;
node1 = getNode(i1, nodes);
pe1 = getPE(j1);
mp = new MappingPair(node1, pe1);
if (!contains(mapPairs, mp))
{
mapPairs.push_back(mp);
}
}
}

cout << "compatibility graph" << endl;

int tableSize = (int) mapPairs.size();
//adj_g.reshape(tableSize, tableSize); 
MappingPair* mp1;
//MappingPair* mp2;

// Generate the adjacency table from the mapping pairs. MappingPair.size x MappingPair.size
// This is the bigger graph   

//for (int i = 0; i < tableSize; i++)
//  {
//  mp1 = mapPairs[i];
//  for (int j = 0; j < tableSize; j++)
//  {
//   if (i == j)
//   continue;

//   else
//   {
//   mp2 = mapPairs[j];
//   adj_g[i][j] = Compatible2(mp1, mp2, myDFG);
//adj_g_without_r[i][j] = Compatible(mp1, mp2, myDFG);
//  }
//  }
//  }
//adj_g.print_matrix(); 

// cout << "created adj_g" << endl;
// adj_h.reshape(nodeSize, nodeSize);
vector<ARC*> arc = myDFG->getSetOfArcs();
adjacency_list = new list<int>[nodeSize]; 

// Generate the adjacency table of the DFG. nodeSize x nodeSize
//This is a smaller graph
for(int i=0; i < nodeSize; i++)
{
  for(int j=0; j < nodeSize; j++)
  {
    if(i==j)
    { 
      if(!contains_list(adjacency_list[i],j))  
        adjacency_list[i].push_back(j);
      continue;
    }

    if(connected_Nodes2(nodes[i], nodes[j]))
    {
      //    adj_h[i][j] = 1;

      // For the BFS method just create adjacency of the incoming nodes
      // so that the next nodes mapped in BFS is the incoming node.
      // Else create both outgoing and incoming adj list.
      if(MAPPING_POLICY.MAPPING_MODE == 5)
      {
        //if(!contains_list(adjacency_list[i],j))
        adjacency_list[j].push_back(i); // Add w to v’s list.

      }
      else
      {
        if(!contains_list(adjacency_list[i],j))
          adjacency_list[i].push_back(j); // Add w to v’s list.
        if(!contains_list(adjacency_list[j], i))
          adjacency_list[j].push_back(i);
      }


      //if(!contains_list(adjacency_list[i],j)) adjacency_list[i].push_back(j);
      //if(!contains_list(adjacency_list[j],i)) adjacency_list[j].push_back(i);
    }
    //  else
    //    adj_h[i][j] = 0;
  }
}
// cout << "passed adj_h" << endl; 
//cout << endl;
//adj_h.print_matrix();
// Generate m matrix for ullmann's algorithm. nodeSize x MappingPair size.
// Generation is here and the refining function will refine this matrix
m.reshape(nodeSize, tableSize);

for(int i=0; i < nodeSize; i++)
{
  for(int j=0; j < tableSize; j++)
  {
    mp1 = mapPairs[j];
    if(nodes[i]->get_ID() == mp1->getNode()->get_ID())
      m[i][j] = 1;
  }
}
cout << "passed m matrix" << endl;
// cout<<endl;
// m.print_matrix();

return true; 
}*/ 


void CGRA::UpdateInterIterationDepReg(DFG* myDFG)
{

  int tableSize = (int) mapPairs.size();
  MappingPair* mp1;
  MappingPair* mp2;
  MappingPair* mp3;

  //iterate through all the mapping pairs
  for (int index1 = 0; index1 < tableSize; index1++)
  {
    mp1 = mapPairs[index1];
    if (mp1->getNode()->has_self_loop())
    {
      continue;
    }
    vector<Node*> cons1 = mp1->getNode()->get_physical_successors_constrain_by_this_modulo(II);

    for (int index2 = 0; index2 < (int) cons1.size(); index2++)
    {
      if (mp1->getNode()->get_ID() == cons1[index2]->get_ID())
        continue;

      //igonore INTRA
      if (myDFG->get_Arc(mp1->getNode(), cons1[index2])->get_Distance() == 0)
        continue;

      for (int index3 = 0; index3 < tableSize; index3++)
      {
        if (index3 == index1)
          continue;

        mp2 = mapPairs[index3];
        if (table[index1 * tableSize + index3] == 0)
          continue;

        int reg_availability = 1;
        // Do a PreScan and See if there are any registers available
        if(cons1[index2]->get_ID() == mp2->getNode()->get_ID() && mp1->SameCoordinationPE(mp2))
        {
          int req_register = myDFG->get_Required_Register(mp1->getNode(), mp2->getNode(), II);

          if(req_register == 0)
            continue;

          if(req_register > (SizeReg - table[index1 * tableSize + index3] + 1)  ) {
            reg_availability = 0;
          }

          if((req_register - 1) > (SizeReg - table[index3 * tableSize + index1] + 1))  {
            reg_availability = 0;
          }

          //check number of registers required for this PE on the INTERMEDIATE PATH
          if(reg_availability == 1) {
            if (mp1->getNode()->get_Sched_Info()->get_Modulo_Current() < mp2->getNode()->get_Sched_Info()->get_Modulo_Current())
            {
              for (int index4 = 0; index4 < tableSize; index4++)
              {
                if (index4 == index3 || index4 == index1)
                  continue;

                mp3 = mapPairs[index4];
                if (!mp1->SameCoordinationPE(mp3))
                  continue;

                if (table[index1 * tableSize + index4] == 0)
                  continue;

                if (mp1->getNode()->get_Sched_Info()->get_Modulo_Current() < mp3->getNode()->get_Sched_Info()->get_Modulo_Current()
                    && mp3->getNode()->get_Sched_Info()->get_Modulo_Current() < mp2->getNode()->get_Sched_Info()->get_Modulo_Current())
                {
                  if(req_register > (SizeReg - table[index4 * tableSize + index1] + 1)  )
                    reg_availability = 0;
                }
                else
                {
                  if((req_register-1) > (SizeReg - table[index4 * tableSize + index1] + 1)  )
                    reg_availability = 0;
                }
              }
            }
            // Node1 is mapped after node2 in the modulo schedule,
            // check the number of registers required for this PE on the cycles
            // except the intermediate path from node 2 to node 1
            else
            {
              for (int index4 = 0; index4 < tableSize; index4++)
              {
                if (index4 == index3 || index4 == index1)
                  continue;

                mp3 = mapPairs[index4];
                if (!mp1->SameCoordinationPE(mp3))
                  continue;

                if (table[index1 * tableSize + index4] == 0 || table[index3 * tableSize + index4] == 0)
                  continue;

                if((mp1->getNode()->get_Sched_Info()->get_Modulo_Current() < mp3->getNode()->get_Sched_Info()->get_Modulo_Current())
                    || (mp3->getNode()->get_Sched_Info()->get_Modulo_Current() < mp2->getNode()->get_Sched_Info()->get_Modulo_Current()))
                {
                  if(req_register > (SizeReg - table[index4 * tableSize + index1] + 1)  )
                    reg_availability = 0;
                }
                else
                {
                  if((req_register-1) > (SizeReg - table[index4 * tableSize + index1] + 1)  )
                    reg_availability = 0;
                }
              }
            }
          }
        }

        if(reg_availability == 0)
          table_without_r[index1 * tableSize + index3] = 0;

        //a successor mapped to the same PE, update number of required registers
        if((cons1[index2]->get_ID() == mp2->getNode()->get_ID() && mp1->SameCoordinationPE(mp2)))
        {
          int req_register = myDFG->get_Required_Register(mp1->getNode(), mp2->getNode(), II);
          table[index1 * tableSize + index3] = table[index1 * tableSize + index3] + req_register;

          table[index3 * tableSize + index1] = table[index3 * tableSize + index1] + req_register - 1;

          //node1 is mapped before node2 in the modulo schedule, update the
          //number of registers required for this PE on the INTERMEDIATE PATH
          if (mp1->getNode()->get_Sched_Info()->get_Modulo_Current() < mp2->getNode()->get_Sched_Info()->get_Modulo_Current())
          {
            for (int index4 = 0; index4 < tableSize; index4++)
            {
              if (index4 == index3 || index4 == index1)
                continue;

              mp3 = mapPairs[index4];
              if (!mp1->SameCoordinationPE(mp3))
                continue;

              if (table[index1 * tableSize + index4] == 0)
                continue;

              if (mp1->getNode()->get_Sched_Info()->get_Modulo_Current() < mp3->getNode()->get_Sched_Info()->get_Modulo_Current()
                  && mp3->getNode()->get_Sched_Info()->get_Modulo_Current() < mp2->getNode()->get_Sched_Info()->get_Modulo_Current())
              {
                table[index4 * tableSize + index1] = table[index4 * tableSize + index1] + req_register;
              }
              else
              {
                table[index4 * tableSize + index1] = table[index4 * tableSize + index1] + req_register - 1;
              }
            }
          }

          // node1 is mapped after node2 in the modulo schedule, update the
          // number of registers required for this PE on the cycles except the
          // intermediate path from node 2 to node 1
          else
          {
            for (int index4 = 0; index4 < tableSize; index4++)
            {
              if (index4 == index3 || index4 == index1)
                continue;

              mp3 = mapPairs[index4];
              if (!mp1->SameCoordinationPE(mp3))
                continue;

              if (table[index1 * tableSize + index4] == 0 || table[index3 * tableSize + index4] == 0)
                continue;

              if ((mp1->getNode()->get_Sched_Info()->get_Modulo_Current() < mp3->getNode()->get_Sched_Info()->get_Modulo_Current())
                  || (mp3->getNode()->get_Sched_Info()->get_Modulo_Current() < mp2->getNode()->get_Sched_Info()->get_Modulo_Current()))
              {
                table[index4 * tableSize + index1] = table[index4 * tableSize + index1] + req_register;
              }
              else
              {
                table[index4 * tableSize + index1] = table[index4 * tableSize + index1] + req_register - 1;
              }
            }
          }
        }
      }
    }
  }
}

void CGRA::UpdateIntraIterationDepReg(DFG* myDFG)
{
  int tableSize = (int) mapPairs.size();
  MappingPair* mp1;
  MappingPair* mp2;
  MappingPair* mp3;

  for (int index1 = 0; index1 < tableSize; index1++)
  {
    mp1 = mapPairs[index1];
    if (mp1->getNode()->has_self_loop())
      continue;

    vector<Node*> cons1 = mp1->getNode()->get_physical_successors_constrain_by_this_modulo(II);

    for (int index2 = 0; index2 < (int) cons1.size(); index2++)
    {
      // ignore INTER-iteration Dependencies
      if (myDFG->get_Arc(mp1->getNode(), cons1[index2])->get_Distance() > 0)
        continue;

      for (int index3 = 0; index3 < tableSize; index3++)
      {
        if (index3 == index1)
          continue;

        mp2 = mapPairs[index3];
        if (table[index1 * tableSize + index3] == 0)
          continue;
        //pre-scan for availability of registers
        int reg_availability = 1;
        if (cons1[index2]->get_ID() == mp2->getNode()->get_ID() && mp1->SameCoordinationPE(mp2))
        {
          int req_register = myDFG->get_Required_Register(mp1->getNode(), mp2->getNode(), II);

          if(req_register == 0)
            continue;

          if(req_register > (SizeReg - table[index1 * tableSize + index3] + 1)  ) {
            reg_availability = 0;
          }

          if((req_register-1) > (SizeReg - table[index3 * tableSize + index1] + 1))  { //Shail as req reg instead of r-1
            reg_availability = 0;
          }

          // node1 is mapped before node2 in the modulo schedule,
          // check the number of registers required for this PE
          // on the INTERMEDIATE PATH
          if(reg_availability == 1) {
            if(mp1->getNode()->get_Sched_Info()->get_Modulo_Current() < mp2->getNode()->get_Sched_Info()->get_Modulo_Current())
            {
              for (int index4 = 0; index4 < tableSize; index4++)
              {
                if (index4 == index3 || index4 == index1)
                  continue;

                mp3 = mapPairs[index4];
                if (!mp1->SameCoordinationPE(mp3))
                  continue;

                if (table[index1 * tableSize + index4] == 0)
                  continue;

                if (mp1->getNode()->get_Sched_Info()->get_Modulo_Current() < mp3->getNode()->get_Sched_Info()->get_Modulo_Current()
                    && mp3->getNode()->get_Sched_Info()->get_Modulo_Current() < mp2->getNode()->get_Sched_Info()->get_Modulo_Current())
                {
                  if(req_register > (SizeReg - table[index4 * tableSize + index1] + 1)  )
                    reg_availability = 0;
                }
                else
                {
                  if((req_register-1) > (SizeReg - table[index4 * tableSize + index1] + 1)  )
                    reg_availability = 0;
                }
              }
            }
            // node1 is mapped after node2 in the modulo schedule,
            // check the number of registers required for this PE
            // on the cycles except the intermediate path from node 2 to node 1
            else
            {
              for (int index4 = 0; index4 < tableSize; index4++)
              {
                if (index4 == index3 || index4 == index1)
                  continue;

                mp3 = mapPairs[index4];
                if (!mp1->SameCoordinationPE(mp3))
                  continue;

                if (table[index1 * tableSize + index4] == 0)
                  continue;

                if (mp1->getNode()->get_Sched_Info()->get_Modulo_Current() < mp3->getNode()->get_Sched_Info()->get_Modulo_Current()
                    || mp3->getNode()->get_Sched_Info()->get_Modulo_Current() < mp2->getNode()->get_Sched_Info()->get_Modulo_Current())
                {
                  if(req_register > (SizeReg - table[index4 * tableSize + index1] + 1)  )
                    reg_availability = 0;
                }
                else
                {
                  if((req_register-1) > (SizeReg - table[index4 * tableSize + index1] + 1)  )
                    reg_availability = 0;
                }
              }
            }
          }
        }

        if(reg_availability == 0)
        {
          table_without_r[index1 * tableSize + index3] = 0;
        }

        if(cons1[index2]->get_ID() == mp2->getNode()->get_ID() && mp1->SameCoordinationPE(mp2))
        {
          int req_register = myDFG->get_Required_Register(mp1->getNode(), mp2->getNode(), II);

          table[index1 * tableSize + index3] = table[index1 * tableSize + index3] + req_register;
          table[index3 * tableSize + index1] = table[index3 * tableSize + index1] + req_register - 1;

          //node1 is mapped before node2 in the modulo schedule,
          // update the number of registers required for this PE on the INTERMEDIATE PATH
          if (mp1->getNode()->get_Sched_Info()->get_Modulo_Current() < mp2->getNode()->get_Sched_Info()->get_Modulo_Current())
          {
            for (int index4 = 0; index4 < tableSize; index4++)
            {
              if (index4 == index3 || index4 == index1)
                continue;

              mp3 = mapPairs[index4];
              if (!mp1->SameCoordinationPE(mp3))
                continue;

              if (table[index1 * tableSize + index4] == 0)
                continue;

              if (mp1->getNode()->get_Sched_Info()->get_Modulo_Current() < mp3->getNode()->get_Sched_Info()->get_Modulo_Current()
                  && mp3->getNode()->get_Sched_Info()->get_Modulo_Current() < mp2->getNode()->get_Sched_Info()->get_Modulo_Current())
              {
                table[index4 * tableSize + index1] = table[index4 * tableSize + index1] + req_register;	//t(n1) < t(ni) < t(n2)
              }
              else
              {
                table[index4 * tableSize + index1] = table[index4 * tableSize + index1] + req_register - 1;	//t(ni) < t(n1) or t(ni) > t(n2)
              }
            }
          }
          //node1 is mapped after node2 in the modulo schedule, update the
          //number of registers required for this PE on the cycles except the
          //intermediate path from node 2 to node 1
          else
          {
            for (int index4 = 0; index4 < tableSize; index4++)
            {
              if (index4 == index3 || index4 == index1)
                continue;

              mp3 = mapPairs[index4];
              if (!mp1->SameCoordinationPE(mp3))
                continue;

              if (table[index1 * tableSize + index4] == 0)
                continue;

              if((mp1->getNode()->get_Sched_Info()->get_Modulo_Current() < mp3->getNode()->get_Sched_Info()->get_Modulo_Current())
                  || (mp3->getNode()->get_Sched_Info()->get_Modulo_Current() < mp2->getNode()->get_Sched_Info()->get_Modulo_Current()))
              {
                table[index4 * tableSize + index1] = table[index4 * tableSize + index1] + req_register;  //t(ni) > t(n1) || t(ni) < t(n2)
              }
              else
              {
                table[index4 * tableSize + index1] = table[index4 * tableSize + index1] + req_register - 1;  //t(n2) < t(ni) < t(n1)
              }
            }
          }
        }
      }
    }
  }
}

void CGRA::UpdateSelfReg(DFG* myDFG)
{
  int tableSize = (int) mapPairs.size();
  MappingPair* mp1;
  MappingPair* mp3;

  //update loop
  for (int index1 = 0; index1 < tableSize; index1++)
  {
    mp1 = mapPairs[index1];
    int req_register = 0;

    if(!myDFG->get_Node(mp1->getNode()->get_ID())->has_self_loop())
      continue;

    //pre-scan for availability of registers
    req_register = mp1->getNode()->get_self_loop()->get_Distance();

    int reg_availability = 1;
    if(req_register > (SizeReg - table[index1 * tableSize + index1] + 1))
    {
      reg_availability = 0;
      table[index1 * tableSize + index1] = 0;
    }

    for (int index4 = 0; index4 < tableSize; index4++)
    {
      if (index4 == index1)
        continue;

      mp3 = mapPairs[index4];
      if(reg_availability == 1) {
        if (!mp1->SameCoordinationPE(mp3))
          continue;
      }
      else
      {
        table[index4 * tableSize + index1] = 0;
        table[index1 * tableSize + index4] = 0;
        continue;
      }

      if (table[index1 * tableSize + index4] == 0)
        continue;

      if(req_register > (SizeReg - table[index4 * tableSize + index1] + 1)  ) {
        reg_availability = 0;
        table_without_r[index4 * tableSize + index1] = 0;
      }
    }

    if(reg_availability == 0)
    {
      table_without_r[index1 * tableSize + index1] = 0;
    }

    table[index1 * tableSize + index1] += req_register;

    // Update To All The PEs Who Access Same Local RF
    for (int index4 = 0; index4 < tableSize; index4++)
    {
      if (index4 == index1)
        continue;

      mp3 = mapPairs[index4];
      if (!mp1->SameCoordinationPE(mp3))
        continue;

      if (table[index1 * tableSize + index4] == 0)
        continue;

      table[index4 * tableSize + index1] += req_register;
    }
  }
}

void CGRA::GenerateCompGraph(DFG* myDFG)
{
  GenerateCompatibilityTable(myDFG);
  UpdateNeedToLoadConstants(myDFG);
  //******************UPDATE INTRA ITERATION REGISTER NUMBERS***************
  UpdateIntraIterationDepReg(myDFG);
  //*************UPDATE INTER ITERATION REGISTER NUMBERS******************
  UpdateInterIterationDepReg(myDFG);
  //Update Self-Loop Register Numbers
  UpdateSelfReg(myDFG);
}

vector<Node*> CGRA::Subtract(vector<Node*> &set1, vector<Node*> &set2)
{
  vector<Node*> retVal;
  vector<Node*>::iterator iNode1;
  for (iNode1 = set1.begin(); iNode1 != set1.end(); iNode1++)
  {
    //if iNode1 is not in set2, add it
    if (!contains(set2, (*iNode1)))
      retVal.push_back(*iNode1);
  }
  return retVal;
}

Node* CGRA::get_Node(int number)
{
  vector<Node*>::iterator iNode1;
  for (iNode1 = _node_Set.begin(); iNode1 != _node_Set.end(); iNode1++)
  {
    if ((*iNode1)->get_ID() == number)
      return *iNode1;
  }
  TEC.print_matrix();

  FATAL(true, "Node with ID=%d is not found", number);
  printf("Node with ID=%d is not found", number);
  return NULL;
}

//leftnodeMapped is the set of nodes that are still unmapped
//This function implements the traditional GraphMinor Minor function.
bool CGRA::Minor(DFG* myDFG, vector<Node*> &leftNotMapped)
{

  _node_Set = myDFG->getSetOfVertices();

  //int nodeSize = (int) _node_Set.size();
  MappingPair* mp1;

  //compatibility table
  //Node* tempNode;
  //DFG* H = myDFG->Copy_With_Schedule_And_Mapping();
  GenerateCompGraph(myDFG);
  //GenerateAdjacencyTable(myDFG, A_h, A_g, M);

  vector<Node*> to_be_mapped = myDFG->SortOps();

  //for(int i=0; i < (int) to_be_mapped.size(); i++)
  //  cout << "Node: " << to_be_mapped[i]->get_ID() << "\tCuurent: " << to_be_mapped[i]->get_Sched_Info()->get_Modulo_Current() << endl;

  /*for(int i=0; i < (int) mapPairs.size(); i++)
    cout << "Node:" << mapPairs[i]->getNode()->get_ID() <<"\tX: " << mapPairs[i]->getPE()->getIndexI()  << "\tY: "  << mapPairs[i]->getPE()->getIndexJ() << "\tT: " << mapPairs[i]->getPE()->getIndexT() << endl;*/ 

  vector<Node*> mapped; 

  int tableSize = (int) mapPairs.size();


  /*for(int i=0; i < tableSize; i++)
    {
    for(int j=0; j < tableSize; j++)
    cout << (int) table_without_r[i * tableSize + j] << " "; 

    cout<<endl;
    }

    _FATAL("Lookout!!"); */ 

  //Reshape the TEC and fill witn INF
  TEC.reshape(II * SizeX, SizeY);

  TEC.fill(INF);

  //PERFORM THE MAPPING
  /*if (MAPPING_POLICY.ENABLE_REGISTERS)
    {
    set<MappingPair*> BestIndices; 
    bool success = MinorUtils(to_be_mapped, H, mapped, tableSize, BestIndices);

    vector<Node*> mapped_vec (mapped.begin(), mapped.end());
    if(!success)
    {
    leftNotMapped = Subtract(to_be_mapped, mapped_vec);
    return false;
    }

    for (int i = 0; i < (int) BestIndices.size(); i++)
    {
  //mp1 = mapPairs[(int) mapped_vec[i]->NodeID];
  mp1 = BestIndices[i];
  mp1->getPE()->AssignNode(mp1->getNode());
  int PEindex = (SizeY*mp1->getPE()->getIndexI()) + mp1->getPE()->getIndexJ();
  int diff = numberOfNonrotatingRegPerPE[PEindex]; //-rfConfig[PEindex] + SizeReg;
  mp1->getPE()->AllocateRegister((int) mapped_vec[i]->REG - 1 - diff);
  }

  return true;
  }*/
  //else
  //{

  vector<MappingPair*> BestIndices;
  bool success = MinorUtils(to_be_mapped, mapped, tableSize, BestIndices);

  //vector<Node*> mapped_vec (mapped.begin(), mapped.end());
  if(!success)
  {
    leftNotMapped = Subtract(to_be_mapped, mapped); 
    return false;
  }

  //TEC.print_matrix(); 
  //for(set<MappingPair*>::iterator it = BestIndices.begin(); it != BestIndices.end(); ++it)
  for (int i = 0; i < (int) BestIndices.size(); i++)
  {
    mp1 = BestIndices[i];
    cout << "mo1 Node: " << mp1->getNode()->get_ID() << " PE:<" << mp1->getPE()->getIndexI() << " ,"  << mp1->getPE()->getIndexJ() << ", " << mp1->getPE()->getIndexT() << ">\n"; 
    mp1->getPE()->AssignNode(mp1->getNode());
  }


  return true;
  //}
}

void CGRA::AssignTEC(int posX, int posY, int sched_time, int node_id)
{
  //cout << "before setting\n";
  //TEC.print_matrix();
  TEC[(sched_time*SizeX)+posX][posY] = node_id;
  //cout << "Pased setting\n"; 

}

void CGRA::UnAssignTEC(int posX, int posY, int sched_time, int node_id)
{

  if(TEC[(sched_time*SizeX)+posX][posY] == node_id)
    TEC[(sched_time*SizeX)+posX][posY] = INF;
}

bool CGRA::isTECfree(int X, int Y, int sched)
{
  if(TEC[(sched*SizeX)+X][Y] >= 0)
    return false;

  return true;
}


vector<MappingPair*> CGRA::min_map(Node* v, vector<Node*> pred, vector<Node*> succ, int tableSize, vector<MappingPair*> mappedPairs)
{

  vector<MappingPair*> retVal;
  int v_id = v->get_ID(); 

  retVal.clear(); 

  if((int) pred.size() == 0 && (int) succ.size( ) == 0)
  {

    vector<pair<int, int> > free_pes;
    free_pes = TEC.find_empty_PEs(v->get_Sched_Info()->get_Modulo_Current(), SizeX);
    MappingPair* mp1;



    /*for(int i=0;i < (int) free_pes.size(); i++)
    {
      PE* pe1;
      pe1 = new PE[1];
      pe1->SetIndex(free_pes[i].first % SizeX, free_pes[i].second, v->get_Sched_Info()->get_Modulo_Current(), 0); // set Regfile size as the last value.
      MappingPair* mp1;
      mp1 = new MappingPair(v, pe1);
      if(!containsPE(retVal, mp1) && mp1->getPE()->getIndexT() == v->get_Sched_Info()->get_Modulo_Current()) 
        retVal.push_back(mp1); 
    }*/


    vector<MappingPair*> full_pairs;
    for(int i=0; i < (int) mapPairs.size(); i++)
    {
      mp1 = mapPairs[i]; 
    
      if(mp1->getNode()->get_ID() == v->get_ID()) 
        full_pairs.push_back(mp1);
    }

    for(int i=0; i < (int) free_pes.size(); i++)
    {
      for(int j=0; j < (int)full_pairs.size(); j++)
      {
        if(full_pairs[j]->getPE()->getIndexI() == free_pes[i].first % SizeX && full_pairs[j]->getPE()->getIndexI() == free_pes[i].second)
          retVal.push_back(full_pairs[j]); 
      }
    }
    /*MappingPair* mp1;
      MappingPair* mp2;
      for(int i=0; i < tableSize; i++)
      {
      mp1 = mapPairs[i];
      if(!mp1->getNode()->get_ID() == v_id)
      continue;

      for(int j=0; j < tableSize; j++)
      {
    //Skip self
    if(i==j)
    continue;
    if(table_without_r[i * tableSize + j] == 1)
    {
    if(!isTECfree(mapPairs[j]->getPE()->getIndexI(), mapPairs[j]->getPE()->getIndexJ(), mapPairs[j]->getPE()->getIndexT()))
    continue;
    mp2 = mapPairs[j];
    if(!containsPE(retVal, mp2) && mp2->getPE()->getIndexT() == v->get_Sched_Info()->get_Modulo_Current()) 
    retVal.push_back(mp2);

    }
    }
    }*/ 
    return retVal; 
  } 

  //  vector<Node*> pred, succ;

  set<MappingPair*> retVal1; 

  MappingPair* mp1;
  MappingPair* mp2;


  // Get predecessor connected PES
  vector<MappingPair*> pred_mapPair;

  if((int)pred.size() > 0)
  {
    for(int ii=0 ; ii < (int) pred.size(); ii++)
    {
      int id = pred[ii]->get_ID();
      vector<MappingPair*> pred_pairs; 
      /* get mapped coordinates from mappedPairs vector*/
      int pred_x = 0;
      int pred_y = 0;
      int pred_sched = 0; 

      for(int jj=0; jj < (int)mappedPairs.size(); jj++)
      {
        if(mappedPairs[jj]->getNode()->get_ID() == id)
        {
          pred_x = mappedPairs[jj]->getPE()->getIndexI();
          pred_y = mappedPairs[jj]->getPE()->getIndexJ();
          pred_sched = mappedPairs[jj]->getNode()->get_Sched_Info()->get_Modulo_Current();
          break;
        }

      }      
      for(int i=0; i < tableSize; i++)
      {
        mp1 = mapPairs[i];
        if(mp1->getNode()->get_ID() != id)
          continue;

        if(mp1->getPE()->getIndexI() == pred_x && mp1->getPE()->getIndexJ() == pred_y) // && mp1->getPE()->getIndexT() == pred_sched)
        {
          for(int j=0; j < tableSize; j++)
          {
            if(i==j)
              continue;

            mp2 = mapPairs[j];
            if(table_without_r[i * tableSize + j] == 1 && mp2->getNode()->get_ID() == v_id )
            {
              if(!isTECfree(mapPairs[j]->getPE()->getIndexI(), mapPairs[j]->getPE()->getIndexJ(), mapPairs[j]->getPE()->getIndexT()))
                continue;
              //cout << "mp2 x: " << mp2->getPE()->getIndexI() << "\ty: " << mp2->getPE()->getIndexJ() << endl;
              //pred_mapPair.insert(mp2);
              if(ii==0)
                pred_mapPair.push_back(mp2);
              else
              {
                if(contains(pred_mapPair, mp2))
                  pred_pairs.push_back(mp2);
              }

            }
          }
        }
      }
      if(ii>0)
      {
        pred_mapPair.clear();
        for(int jj=0; jj < (int) pred_pairs.size(); jj++)
          pred_mapPair.push_back(pred_pairs[jj]);
      }

    } 
  }


  // Get succesor connected PES
  //set<MappingPair*> succ_mapPair;
  vector<MappingPair*> succ_mapPair;

  if((int) succ.size() > 0)
  {
    for(int ii=0 ; ii < (int) succ.size(); ii++)
    {
      int id = succ[ii]->get_ID();
      vector<MappingPair*> succ_pairs;
      /* get mapped coordinates from mappedPairs vector*/ 
      int succ_x = 0;  
      int succ_y = 0;
      int succ_sched = 0; 

      for(int jj=0; jj < (int)mappedPairs.size(); jj++)
      {
        if(mappedPairs[jj]->getNode()->get_ID() == id)
        {
          succ_x = mappedPairs[jj]->getPE()->getIndexI(); 
          succ_y = mappedPairs[jj]->getPE()->getIndexJ();
          succ_sched = mappedPairs[jj]->getNode()->get_Sched_Info()->get_Modulo_Current(); 
          break;
        }

      }
      for(int i=0; i < tableSize; i++)
      {
        mp1 = mapPairs[i];
        if(mp1->getNode()->get_ID() != id) 
          continue;

        if(mp1->getPE()->getIndexI() == succ_x && mp1->getPE()->getIndexJ() == succ_y) // && mp1->getPE()->getIndexT() == succ_sched)
        {
          for(int j=0; j < tableSize; j++)
          {
            if(i==j)
              continue;

            mp2 = mapPairs[j];
            if(table_without_r[i * tableSize + j] == 1 && mp2->getNode()->get_ID() == v_id )
            {
              if(!isTECfree(mapPairs[j]->getPE()->getIndexI(), mapPairs[j]->getPE()->getIndexJ(), mapPairs[j]->getPE()->getIndexT()))
                continue;
              if(ii==0)
                succ_mapPair.push_back(mp2);
              else
              {
                if(contains(succ_mapPair, mp2))
                  succ_pairs.push_back(mp2); 
              }
            }
          }
        }
      }

      if(ii>0)
      {
        succ_mapPair.clear();
        for(int jj=0; jj < (int) succ_pairs.size(); jj++)
          succ_mapPair.push_back(succ_pairs[jj]);
      }
    }
  }

  if( (int) pred.size() > 0 && (int) succ.size() > 0)
  {
    for(vector<MappingPair*>::iterator it1 = pred_mapPair.begin(); it1 != pred_mapPair.end(); ++it1)
    {
      for(vector<MappingPair*>::iterator it2 = succ_mapPair.begin(); it2 != succ_mapPair.end(); ++it2)
      {
        if((*it1)->getPE()->getIndexI() == (*it2)->getPE()->getIndexI() && (*it1)->getPE()->getIndexJ() == (*it2)->getPE()->getIndexJ())
        {
          retVal.push_back(*it2);
        }
      }
    }
  }
  else if( (int) pred.size() > 0 && (int) succ.size() == 0)
  {
    for(vector<MappingPair*>::iterator it1 = pred_mapPair.begin(); it1 != pred_mapPair.end(); ++it1)
      retVal.push_back(*it1);
  }
  else if(  (int) pred.size() == 0 && (int) succ.size() > 0) 
  {
    for(vector<MappingPair*>::iterator it2 = succ_mapPair.begin(); it2 != succ_mapPair.end(); ++it2)
      retVal.push_back(*it2);
  }
  //copy(retVal1.begin(), retVal1.end(), retVal.begin());

  return retVal; 
}


/*vector<MappingPair*> CGRA::min_map1(Node* v, vector<Node*> pred, vector<Node*> succ)
  {
  int map_case = -1; 
  vector<MappingPair*> retVal;
  vector<int> indices = M.row_find(*i, value); 

// These steps are not explained in detail in GraphMinor Paper

//   The possible implementation is as follows:
//Get all the possible input and output combinations. 
//For each of these combinations we should now find the 
// connectedPEs pertaining to the mapped Pred and succ nodes.


if(pred.size() == 0 &&  succ.size() == 0)
map_case = 0; 
else if(pred.size() == 1 &&  succ.size() == 0)
map_case = 1;
else if(pred.size() == 2 &&  succ.size() == 0)
map_case = 2; 
else if(pred.size() == 3 &&  succ.size() == 0)
map_case = 3; 
else if(pred.size() == 0 &&  succ.size() == 1)  
map_case = 4;
else if(pred.size() == 1 &&  succ.size() == 1)
map_case = 5;
else if(pred.size() == 2 &&  succ.size() == 1)
map_case = 6;
else if(pred.size() == 3 &&  succ.size() == 1)
map_case = 7;
else if(pred.size() == 0 &&  succ.size() == 2)  
map_case = 8;
else if(pred.size() == 1 &&  succ.size() == 2)
map_case = 9;
else if(pred.size() == 2 &&  succ.size() == 2)
map_case = 10;
else if(pred.size() == 3 &&  succ.size() == 2)
map_case = 11;
else if(pred.size() == 0 &&  succ.size() == 3)  
map_case = 12;
else if(pred.size() == 1 &&  succ.size() == 3)
map_case = 13;
else if(pred.size() == 2 &&  succ.size() == 3)
map_case = 14;
else if(pred.size() == 3 &&  succ.size() == 3)
map_case = 15;
else if(pred.size() == 0 &&  succ.size() == 4)  
map_case = 16;
else if(pred.size() == 1 &&  succ.size() == 4)
map_case = 17;
else if(pred.size() == 2 &&  succ.size() == 4)
map_case = 18;
else if(pred.size() == 3 &&  succ.size() == 4)
map_case = 19;
else if(pred.size() == 0 &&  succ.size() == 5)  
map_case = 20;
else if(pred.size() == 1 &&  succ.size() == 5)
map_case = 21;
else if(pred.size() == 2 &&  succ.size() == 5)
map_case = 22;
else if(pred.size() == 3 &&  succ.size() == 5)
map_case = 23;

switch(map_case)
{
case 0: 
{
vector<int> potential_pos;
potential_pos.clear();
if(v->is_Load_Address_Generator())
{
vector<int> mapped_nodes;
for(int ii=0; ii<(int) indices.size(); ii++)
{
  if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
    continue;

  mapped_nodes.clear();
  mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());
  bool found_mem=false;
  if(((int) mapped_nodes.size()) > 0)
  {
    for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
    {
      if(get_Node(mapped_nodes[jj])->is_Load_Address_Generator() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
        break;
      else
        potential_pos.push_back(indices[ii]);
    }
  }
  else
    potential_pos.push_back(indices[ii]);
}
}
else
{
  for(int ii=0; ii<(int) indices.size(); ii++)
  {
    if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
      continue;

    //since there are no in and out nodes mapped, no need to find connectedPEs                
    potential_pos.push_back(indices[ii]);
  }

}
//Get the corresponding mappairs for the potential pos
for(int ii=0; ii < (int) potential_pos.size(); ii++)
retVal.push_back(potential_pos[ii]);

return retVal; 
} 

case 1: 
{
  int mp = get_mapped_mappairs(pred[0]);
  vector<int> potential_pos;
  potential_pos.clear(); 
  if(v->is_Load_Address_Generator())
  {
    vector<int> mapped_nodes;
    for(int ii=0; ii<(int) indices.size(); ii++)
    {
      if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
        continue;

      mapped_nodes.clear();
      mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());
      bool found_mem=false;
      if(((int) mapped_nodes.size()) > 0)
      {
        for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
        {
          if(get_Node(mapped_nodes[jj])->is_Load_Address_Generator() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
            break;
          else
          {
            if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
              potential_pos.push_back(indices[ii]);
          }
        }
      }
      else
      {
        if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
          potential_pos.push_back(indices[ii]);
      }

    }
  }
  else if(v->is_Load_Data_Bus_Read())
  {
    int row=-1;
    row = TEC.row_find(pred[0]->get_ID()) % SizeX; 

    vector<int> mapped_nodes; 

    for(int ii=0; ii < (int) indices.size(); ii++)
    {
      // check if the PE is free, if not continue to the next position.
      if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
        continue;

      mapped_nodes.clear();
      mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());

      if((int) mapped_nodes.size() > 0)
      {
        for(int jj=0; jj <(int) mapped_nodes.size(); jj++)
        {
          if(row==-1)
          {
            if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
            {
              if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                break;
              else
                potential_pos.push_back(indices[ii]);

            }
          }
          else
          {
            if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
            {
              if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                break;
              else
                potential_pos.push_back(indices[ii]);
            }

          }

        }
      }
      else
      {
        if(row==-1)
        {
          if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
            potential_pos.push_back(indices[ii]);
        }
        else
        {
          if(CmapPairs[indices[ii]]->getPE()->getIndexI() == row)
            potential_pos.push_back(indices[ii]);
        }
      }
    }
  }
  else if(v->is_Store_Address_Generator())
  {  
    vector<int> mapped_nodes;

    mapped_nodes.clear();
    mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());

    for(int ii=0; ii<(int) indices.size(); ii++)
    {
      if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
        continue;


      for(int jj=0; jj <(int) mapped_nodes.size(); jj++)
      {
        if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
        {
          if(get_Node(mapped_nodes[jj])->is_Load_Address_Generator() || get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
            break;
          else
            potential_pos.push_back(indices[ii]);

        }
      }
    }
  }
  else if(v->is_Store_Data_Bus_Write())
  {
    // check if th eincoming mapped node is a store addr. 
    if(pred[0]->is_Store_Address_Generator())
    {
      int row = -1;
      row = TEC.row_find(pred[0]->get_ID()) % SizeX;

      for(int ii=0; ii<(int) indices.size(); ii++)
      {
        if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
          continue;

        //since there are no in and out nodes mapped, no need to find connectedPEs                
        if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
          potential_pos.push_back(indices[ii]);
      }
    }
    else
    {
      for(int ii=0; ii<(int) indices.size(); ii++)
      {
        if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
          continue;

        //since there are no in and out nodes mapped, no need to find connectedPEs                
        if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
          potential_pos.push_back(indices[ii]);
      }
    }
  }
  else
  {
    for(int ii=0; ii < (int) indices.size(); ii++)
    {
      // check if the PE is free, if not continue to the next position.
      if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
        continue;

      if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
        potential_pos.push_back(indices[ii]);
    }
  }

  //Get the corresponding mappairs for the potential pos
  for(int ii=0; ii < (int) potential_pos.size(); ii++)
    retVal.push_back(potential_pos[ii]);

  return retVal;  
}

case 2: 
{
  _FATAL("2 pred mapped with 0 succs\n"); 
  return retVal; 
}  
case 3:
{
  _FATAL("3 pred mapped with 0 succs\n");
  return retVal;
}
case 4:
{
  int out_mp = get_mapped_mappairs(succ[0]);
  vector<int> potential_pos;
  //cout << "node: " << current_node->get_ID() << "\tindices size:" << (int) indices.size() << endl;
  for(int ii=0; ii < (int) indices.size(); ii++)
  {
    // check if the PE is free, if not continue to the next position.
    if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
      continue;

    //cout << "printing after !TEC" << endl;
    if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()))
    {
      potential_pos.push_back(indices[ii]);
    }
  }
  //Get the corresponding mappairs for the potential pos
  for(int ii=0; ii < (int) potential_pos.size(); ii++)
    retVal.push_back(potential_pos[ii]);

  return retVal;
}
case 5:
{
  int out_mp = get_mapped_mappairs(succ[0]);
  int mp = get_mapped_mappairs(pred[0]);

  vector<int> potential_pos; 
  potential_pos.clear(); 
  if(v->is_Load_Data_Bus_Read())
  {
    vector<int> mapped_nodes;
    int row=-1;
    if(pred[0]->is_Load_Address_Generator())
      row = TEC.row_find(incoming_nodes_mapped[0]->get_ID()) % SizeX;

    for(int ii=0; ii < (int) indices.size(); ii++)
    {
      // check if the PE is free, if not continue to the next position.
      if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
        continue;

      mapped_nodes.clear();
      mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());

      for(int jj=0; jj <(int) mapped_nodes.size(); jj++)
      {
        if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()))
        {
          if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
          {   
            if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
              break;
            else
              potential_pos.push_back(indices[ii]);

          }
        }

      }
    }
  } 
  else
  {
    for(int ii=0; ii < (int) indices.size(); ii++)
    {
      // check if the PE is free, if not continue to the next position.
      if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
        continue;

      if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()))
        potential_pos.push_back(indices[ii]);
    }


  }
  //Get the corresponding mappairs for the potential pos
  for(int ii=0; ii < (int) potential_pos.size(); ii++)
    retVal.push_back(potential_pos[ii]);

  return retVal;
}
case 6:
{
  _FATAL("2 pred mapped with 1 succs\n");
  return retVal;
}
case 7:
{
  _FATAL("3 pred mapped with 1 succs\n");
  return retVal;
}
case 8:
{
  _FATAL("0 pred mapped with 2 succs\n");
  return retVal;
}
case 9:
{
  _FATAL("1 pred mapped with 2 succs\n");
  return retVal;
}
case 10:
{
  _FATAL("2 pred mapped with 2 succs\n");
  return retVal;
}
case 11:
{
  _FATAL("3 pred mapped with 2 succs\n");
  return retVal;
}
case 12:
{
  _FATAL("0 pred mapped with 3 succs\n");
  return retVal;
}
case 13:
{
  _FATAL("1 pred mapped with 3 succs\n");
  return retVal;
}
case 14:
{
  _FATAL("2 pred mapped with 3 succs\n");
  return retVal;
}
case 15:
{
  _FATAL("3 pred mapped with 3 succs\n");
  return retVal;
}
case 16:
{
  _FATAL("0 pred mapped with 4 succs\n");
  return retVal;
}
case 17:
{
  _FATAL("1 pred mapped with 4 succs\n");
  return retVal;
}
case 18:
{
  _FATAL("2 pred mapped with 4 succs\n");
  return retVal;
}
case 19:
{
  _FATAL("3 pred mapped with 4 succs\n");
  return retVal;
}
case 20:
{
  _FATAL("0 pred mapped with 5 succs\n");
  return retVal ;
}
case 21:
{
  _FATAL("1 pred mapped with 5 succs\n");
  return retVal;
}
case 22:
{
  _FATAL("2 pred mapped with 5 succs\n");
  return retVal;
}
case 23:
{
  _FATAL("3 pred mapped with 5 succs\n");
  return retVal;
}
default:
{
  return retVal; 
}
} 

_FATAL("Should not reach here!!!\n");

return retVal;

}*/ 


tuple<vector<Node*>, vector<Node*> > CGRA::get_Mapped_pred_succ(Node* v)
{
  vector<Node*> pred, succ;

  //cout << "from get_Mapped for node: " << v->get_ID() << endl;

  // Block to get all mapped pred.
  {
    vector<Node*> incoming_nodes = v->Get_Prev_Nodes();
    for(int jj = 0; jj < (int) incoming_nodes.size(); jj++)
    {  
      if(TEC.find_element(incoming_nodes[jj]->get_ID()))
      {
        pred.push_back(incoming_nodes[jj]);
      }
    }

  }

  // BLock to get all mapped succ.
  {
    vector<Node*> outgoing_nodes = v->Get_Next_Nodes();
    for(int jj = 0; jj < (int) outgoing_nodes.size(); jj++)
    {
      if(TEC.find_element(outgoing_nodes[jj]->get_ID()))
      {
        succ.push_back(outgoing_nodes[jj]);
      }

    }
  }

  return make_tuple(pred, succ); 

}



bool CGRA::MinorUtils(vector<Node*> to_be_mapped, vector<Node*> &mapped, int tableSize, vector<MappingPair*> &mappingPairs)
{
  if(Subtract(to_be_mapped, mapped).size() == 0)
    return true;

  //cout << "to_be_mapped: " << to_be_mapped.size() << "\t mapped_vec: " << mapped.size() << endl;  

  Node* v = to_be_mapped[Scc_iterator];
  //cout << "Node: " << v->get_ID() << endl; 
  vector<Node*> P, S; 
  tie(P,S) = get_Mapped_pred_succ(v); 

  //Tau is a vector used to get all the connected mapping pairs
  vector<MappingPair*> Tau; 

  //For the head node of SCCs no pred or succ will be mapped. 
  //So consider all possible mapping.

  Tau.clear(); 
  Tau = min_map(v, P, S, tableSize, mappingPairs);   

  //for(int i=0; i < (int) Tau.size(); i++)
  //  cout << "X: " << Tau[i]->getPE()->getIndexI() << "\tY: " << Tau[i]->getPE()->getIndexJ() << "\tT: " << Tau[i]->getPE()->getIndexT() << endl;


  /* No valid connected nodes */
  /* GraphMinor paper deals with only routing nodes addition if Tau is 0 */

  /* Graph minor does not deal with this clearly, as to how the additional nodes added should be rescheduled.
   * We will consider it is rescheduled and mapped in the expand_map function, so that the MinorUtils will deal
   * with forward propogation of mapping. 
   */

  if( (int) Tau.size() == 0)
  {
    //Scc_iterator--; 
    return false;
  }

  for( int i = 0; i < (int) Tau.size(); i++)
  {
    //cout << "Node: " << v->get_ID() << "\ti: " << i << endl;
    //cout << " " << Tau[i]->getPE()->getIndexI() << " " << Tau[i]->getPE()->getIndexJ() << " " << Tau[i]->getPE()->getIndexT() << endl;
    if(!isTECfree(Tau[i]->getPE()->getIndexI(), Tau[i]->getPE()->getIndexJ(), Tau[i]->getPE()->getIndexT()))
        continue;

    AssignTEC(Tau[i]->getPE()->getIndexI(), Tau[i]->getPE()->getIndexJ(), Tau[i]->getPE()->getIndexT(), v->get_ID());
    mapped.push_back(v);
    mappingPairs.push_back(Tau[i]); 
    Scc_iterator++; 
    if(MinorUtils(to_be_mapped, mapped, tableSize, mappingPairs))
    {
      //cout << "inside true node: " << v->get_ID() << endl;  
      //cout << "pos:X" << Tau[i]->getPE()->getIndexI() << "\tY:" << Tau[i]->getPE()->getIndexJ() << "\tT:" << Tau[i]->getPE()->getIndexT() << "\tNode:" << v->get_ID();
      //AssignTEC(Tau[i]->getPE()->getIndexI(), Tau[i]->getPE()->getIndexJ(), Tau[i]->getPE()->getIndexT(), v->get_ID());
      //mapped.insert(v); 
      //mappingPairs.push_back(Tau[i]); 
      return true; 
    }
    else
    {
      //cout << endl;
      //cout << "No PE" << endl;
      //TEC.print_matrix(); 
      UnAssignTEC(Tau[i]->getPE()->getIndexI(), Tau[i]->getPE()->getIndexJ(), Tau[i]->getPE()->getIndexT(), v->get_ID());
      mapped.pop_back();
      mappingPairs.pop_back(); 
      Scc_iterator--;
    }

  }  

  /* No valid connected nodes */ 
  /* GraphMinor paper deals with only routing nodes addition if Tau is 0 */ 

  /* Graph minor does not deal with this clearly, as to how the additional nodes added should be rescheduled.
   * We will consider it is rescheduled and mapped in the expand_map function, so that the MinorUtils will deal
   * with forward propogation of mapping. 
   */

  // The addition of routing nodes and scheduling of those operations are done in graphminor.h
  //Scc_iterator--; 
  return false; 
}

//leftnodeMapped is the set of nodes that are still unmapped
bool CGRA::MCS2(DFG* myDFG, vector<Node*> &leftNotMapped)
{
  vector<Node*> nodes = myDFG->getSetOfVertices();

  int nodeSize = (int) nodes.size();
  MappingPair* mp1;

  //compatibility table
  Node* tempNode;
  for (int i = 0; i < nodeSize; i++)
  {
    tempNode = nodes[i];
    debugfile << "Node " << tempNode->get_ID() << " is scheduled at " << tempNode->get_Sched_Info()->get_Current() << " ASAP: " << tempNode->get_Sched_Info()->get_Feasible_ASAP() << "  ALAP: " << tempNode->get_Sched_Info()->get_Feasible_ALAP()  << " Mod: " << tempNode->get_Sched_Info()->get_Modulo_Current() << endl;
  }

  GenerateCompGraph(myDFG);

  int tableSize = (int) mapPairs.size();

  vector<int> indecis;
  vector<Cover_Pair*> BestIndecis;

  Mapper myMapper(SizeReg,&mapPairs,SizeX,SizeY);
  //A vector of mappings that will be generated by the mapper
  vector<vector<Cover_Pair*> > results;

  //PERFORM THE MAPPING
  if (MAPPING_POLICY.ENABLE_REGISTERS)
  {
    results = myMapper.MapWithRegisterFile(table, tableSize, nodeSize);
  }
  else
  {
    results = myMapper.MapWithoutRegisterFile(table, tableSize, nodeSize);
  }

  int maxindex = 0;
  int maxSize = 0;

  //find the clique of the maximum size
  for (int i = 0; i < (int) results.size(); i++)
  {
    vector<Cover_Pair*> current = results[i];
    if ((int) current.size() > maxSize)
    {
      maxSize = current.size();
      maxindex = i;
    }
  }
  BestIndecis = results[maxindex];

  bool nodefound;
  //find the unmapped nodes
  for (int i = 0; i < (int) nodes.size(); i++)
  {
    nodefound = false;
    for (int j = 0; j < (int) BestIndecis.size(); j++)
    {
      mp1 = mapPairs[(int) BestIndecis[j]->NodeID];
      if (mp1->getNode()->get_ID() == nodes[i]->get_ID())
      {
        nodefound = true;
        break;
      }
    }

    if (!nodefound)
      leftNotMapped.push_back(nodes[i]);
  }

  if (maxSize == nodeSize)
  {
    //Update Number of Nonrotating Registers Mapped Per PE in case of R > 0
    numberOfNonrotatingRegPerPE = new int[SizeX * SizeY];
    for(int rr = 0; rr < (SizeX*SizeY); rr++)
      numberOfNonrotatingRegPerPE[rr] = myMapper.nonrecVar_PE[rr];

    for (int i = 0; i < (int) BestIndecis.size(); i++)
    {
      mp1 = mapPairs[(int) BestIndecis[i]->NodeID];
      mp1->getPE()->AssignNode(mp1->getNode());
      int PEindex = (SizeY*mp1->getPE()->getIndexI()) + mp1->getPE()->getIndexJ();
      int diff = numberOfNonrotatingRegPerPE[PEindex]; //-rfConfig[PEindex] + SizeReg;
      mp1->getPE()->AllocateRegister((int) BestIndecis[i]->REG - 1 - diff);
    }

    return true;
  }
  else
  {
    return false;
  }
}

void CGRA::Prolog(DFG* myDFG, int highest_distance, int node_with_highest_distance)
{
  std::ofstream prologFile;
  std::string prolog = "prolog";
  prolog.append(".sch");
  prologFile.open(prolog.c_str());
  int total=0;
  int t = -1;
  bool toBreak = false;

  int unrolled_time = 0;

  if(highest_distance > 0)
    unrolled_time = myDFG->get_Node(node_with_highest_distance)->get_Sched_Info()->get_Current() + highest_distance*II;

  cout << "\n*********************************Prolog Start*********************************\n";
  while (true)
  {
    toBreak = true;
    for (int timeIndex = 0; timeIndex < II; timeIndex++)
    {
      t++;
      TimeCGRA* cgraTime = timeCGRAList[timeIndex];
      PE* candidatePE;
      cout << "Time:" << t << endl;
      for (int i = 0; i < SizeX; i++)
      {
        for (int j = 0; j < SizeY; j++)
        {
          candidatePE = cgraTime->getPE(i, j);
          if (candidatePE->isBusy())
          {
            if (candidatePE->getNode()->get_Sched_Info()->get_Current() <= t)
            {
              printf("%10d", candidatePE->getNode()->get_ID());
              prologFile << candidatePE->getNode()->get_ID() << "\n";
              total++;
            }
            else
            {
              printf("         F");
              prologFile << "-1" << "\n";
              toBreak = false;
              total++;
            }
          }
          else
          {
            printf("         F");
            prologFile << "-1" << "\n";
            total++;
          }
          printf("\t");
        }
        printf("\n");
      }
    }
    if((toBreak) && (t >= unrolled_time))
    {
      break;
    }
  }
  cout << "\n*********************************Prolog End*********************************\n";
  prologFile << total << "\n";
  prologFile.close();
}

void CGRA::EPIlog(DFG* myDFG)
{

  std::ofstream epilogFile;
  std::string epilog = "epilog";
  epilog.append(".sch");
  epilogFile.open(epilog.c_str());

  int total=0;
  int t = -1;
  bool toBreak = false;
  bool nextBreak = true;
  cout << "\n*********************************Epilog Start*********************************\n";
  while (nextBreak)
  {
    for (int timeIndex = 0; timeIndex < II; timeIndex++)
    {
      t++;
      toBreak = true;
      TimeCGRA* cgraTime = timeCGRAList[timeIndex];
      PE* candidatePE;
      cout << "Time:" << t << endl;
      for (int i = 0; i < SizeX; i++)
      {
        for (int j = 0; j < SizeY; j++)
        {
          candidatePE = cgraTime->getPE(i, j);
          if (candidatePE->isBusy())
          {
            if (candidatePE->getNode()->get_Sched_Info()->get_Current() > t)
            {
              toBreak = false;
            }
          }
        }
      }
      if (toBreak)
      {
        nextBreak = false;
        break;
      }
      for (int i = 0; i < SizeX; i++)
      {
        for (int j = 0; j < SizeY; j++)
        {
          candidatePE = cgraTime->getPE(i, j);
          if (candidatePE->isBusy())
          {
            if (candidatePE->getNode()->get_Sched_Info()->get_Current() > t)
            {
              printf("%10d", candidatePE->getNode()->get_ID());
              epilogFile << candidatePE->getNode()->get_ID() << "\n";
              total++;
            }
            else
            {
              printf("         F");
              epilogFile << "-1" << "\n";
              total++;
              toBreak = false;
            }
          }
          else
          {
            printf("         F");
            epilogFile << "-1" << "\n";
            total++;
          }
          printf("\t");
        }
        printf("\n");
      }
    }
  }
  cout << "\n*********************************Epilog End*********************************\n";
  epilogFile << total << "\n";
  epilogFile.close();
}

Node* CGRA::getNode(int tableIndex, vector<Node*> &nodes)
{
  return nodes[tableIndex];
}

PE* CGRA::getPE(int index)
{
  int CurrentII = index / (SizeX * SizeY);
  int XIndex = (index % (SizeX * SizeY)) / SizeY;
  int YIndex = index % SizeY;
  return timeCGRAList[CurrentII]->getPE(XIndex, YIndex);
}

bool CGRA::SameCoordinatePEs(PE* pe1, PE* pe2)
{
  int iIndex1 = pe1->getIndexI();
  int jIndex1 = pe1->getIndexJ();

  int iIndex2 = pe2->getIndexI();
  int jIndex2 = pe2->getIndexJ();
  if (iIndex1 == iIndex2 && jIndex1 == jIndex2)
  {
    return true;
  }
  return false;
}

bool CGRA::contains(vector<Node*> &nodeset, Node* node)
{
  vector<Node*>::iterator iNode1;
  for (iNode1 = nodeset.begin(); iNode1 != nodeset.end(); iNode1++)
  {
    if ((*iNode1)->get_ID() == node->get_ID())
      return true;
  }
  return false;
}

bool CGRA::contains(vector<MappingPair*> &pairs, MappingPair* mp)
{
  vector<MappingPair*>::iterator iPair;
  for (iPair = pairs.begin(); iPair != pairs.end(); iPair++)
  {
    if ((*iPair)->Match(mp))
      return true;
  }
  return false;
}

bool CGRA::containsPE(vector<MappingPair*> &pairs, MappingPair* mp)
{
  vector<MappingPair*>::iterator iPair;
  for (iPair = pairs.begin(); iPair != pairs.end(); iPair++)
  {
    if ((*iPair)->MatchPE(mp))
      return true;
  }
  return false;
}


bool CGRA::contains(vector<Node*> &nodeset1, vector<Node*> &nodeset2)
{
  vector<Node*>::iterator iNode1;
  for (iNode1 = nodeset2.begin(); iNode1 != nodeset2.end(); iNode1++)
  {
    if (contains(nodeset1, *iNode1))
      return true;
  }
  return false;
}

bool CGRA::connectedPEs(PE* pe1, PE* pe2)
{
  int iIndex1 = pe1->getIndexI();
  int jIndex1 = pe1->getIndexJ();
  int tIndex1 = pe1->getIndexT();

  int iIndex2 = pe2->getIndexI();
  int jIndex2 = pe2->getIndexJ();
  int tIndex2 = pe2->getIndexT();

  int deltaI, deltaJ, deltaT;

  if (tIndex1 > tIndex2)
  {
    if (tIndex1 != II - 1 || tIndex2 != 0)
    {
      if (iIndex1 != iIndex2 || jIndex1 != jIndex2)
        return false;
    }
    deltaT = 1;
  }
  else
  {
    deltaT = tIndex2 - tIndex1;
  }

  if (deltaT > 1)
  {
    if (iIndex1 != iIndex2 || jIndex1 != jIndex2)
      return false;
  }

  if (iIndex1 > iIndex2)
  {
    deltaI = iIndex1 - iIndex2;
  }
  else
  {
    deltaI = iIndex2 - iIndex1;
  }

  if (jIndex1 > jIndex2)
  {
    deltaJ = jIndex1 - jIndex2;
  }
  else
  {
    deltaJ = jIndex2 - jIndex1;
  }

  if (deltaI == SizeX - 1 && SizeX > 1)
  {
    deltaI = 1;
  }

  if (deltaJ == SizeY - 1 && SizeY > 1)
  {
    deltaJ = 1;
  }

  if (deltaI + deltaJ > 1)
  {
    return false;
  }

  return true;
}

bool CGRA::connected_Nodes(Node* node1, Node* node2)
{
  if (node1->is_Physically_Connected_To(node2))
    return true;

  vector<Node*> cons = node1->get_physical_successors_constrain_by_this_modulo(II);
  if (contains(cons, node2))
  {
    return true;
  }

  return false;
}

int CGRA::Compatible(MappingPair* mp1, MappingPair* mp2, DFG* &myDFG)
{
  if (mp1->getNode()->get_ID() == mp2->getNode()->get_ID() || mp1->getPE() == mp2->getPE())
    return 0;
  else
  {
    // constraints
    vector<Node*> cons1 = mp1->getNode()->get_physical_successors_constrain_by_this_modulo(II);
    vector<Node*> cons2 = mp2->getNode()->get_physical_successors_constrain_by_this_modulo(II);
    bool are_sched_far = false;

    Node* node1;
    Node* node2;

    if (contains(cons1, mp2->getNode()))
    {
      node1 = mp1->getNode();
      node2 = mp2->getNode();
      are_sched_far = true;
    }
    else if(contains(cons2, mp1->getNode()))
    {
      node1 = mp2->getNode();
      node2 = mp1->getNode();
      are_sched_far = true;
    }

    if (are_sched_far)
    {
      unsigned int req_register = myDFG->get_Required_Register(node1,node2, II) + mp1->getNode()->hasLargeConst + mp2->getNode()->hasLargeConst;
      //This will be complex when other RF structures (e.g. shared RF) are included.
      if(req_register <= cgra_info.R_Size)
      {
        if (!SameCoordinatePEs(mp1->getPE(), mp2->getPE())) {
          return 0;
        }
      }
      else
      {
        return 0;
      }
    }

    if ((connected_Nodes(mp1->getNode(), mp2->getNode()) && !connectedPEs(mp1->getPE(), mp2->getPE()))
        || (connected_Nodes(mp2->getNode(), mp1->getNode()) && !connectedPEs(mp2->getPE(), mp1->getPE())))
      return 0;

    if (mp1->getPE()->getIndexI()==mp2->getPE()->getIndexI() && mp1->getPE()->getIndexT()==mp2->getPE()->getIndexT() && mp1->getNode()->is_Mem_Operation() && mp2->getNode()->is_Mem_Operation())
    {
      if ((mp1->getNode()->is_Load_Address_Generator() || mp1->getNode()->is_Store_Address_Generator())&& (mp2->getNode()->is_Load_Address_Generator() || mp2->getNode()->is_Store_Address_Generator()) && (mp1->getNode()->get_Related_Node()->get_ID()!=mp2->getNode()->get_ID()))
        return 0;

      if ((mp1->getNode()->is_Load_Data_Bus_Read() || mp1->getNode()->is_Store_Data_Bus_Write() ) && (mp2->getNode()->is_Load_Data_Bus_Read() || mp2->getNode()->is_Store_Data_Bus_Write())&& (mp1->getNode()->get_Related_Node()->get_ID()!=mp2->getNode()->get_ID()))
        return 0;
    }

    if (mp1->getNode()->is_Mem_Operation() && mp2->getNode()->is_Mem_Operation())
    {
      if(mp1->getNode()->get_Related_Node()->get_ID()==mp2->getNode()->get_ID())
      {
        if (mp1->getPE()->getIndexI()!=mp2->getPE()->getIndexI())
          return 0;
      }
    }
  }

  return 1;
}

int CGRA::Physical_Distance(PE* pe1, PE* pe2)
{
  int iIndex1 = pe1->getIndexI();
  int jIndex1 = pe1->getIndexJ();

  int iIndex2 = pe2->getIndexI();
  int jIndex2 = pe2->getIndexJ();

  int deltaI, deltaJ;
  int deltaI_2, deltaJ_2;

  if (iIndex1 > iIndex2)
  {
    deltaI = iIndex1 - iIndex2;
    deltaI_2 = (SizeX - iIndex1) + iIndex2;
  }
  else
  {
    deltaI = iIndex2 - iIndex1;
    deltaI_2 = (SizeX - iIndex2) + iIndex1;

  }
  if (jIndex1 > jIndex2)
  {
    deltaJ = jIndex1 - jIndex2;
    deltaJ_2 = (SizeY - jIndex1) + jIndex2;
  }
  else
  {
    deltaJ = jIndex2 - jIndex1;
    deltaJ_2 = (SizeY - jIndex2) + jIndex1;
  }

  if (deltaI > deltaI_2)
    deltaI = deltaI_2;

  if (deltaJ > deltaJ_2)
  {
    deltaJ = deltaJ_2;
  }

  return deltaJ + deltaI;
}

void CGRA::Insert_TimeCGRA(TimeCGRA* newCGRA)
{
  timeCGRAList.push_back(newCGRA);
}

CGRA CGRA::Copy()
{
  CGRA myCGRA;

  myCGRA.Initialize(SizeX, SizeY, SizeReg, ConType);
  myCGRA.SetII(getII());
  myCGRA.ResetIteration();

  int size = timeCGRAList.size();
  for (int i = 0; i < size; i++)
  {
    TimeCGRA* cgraTime = timeCGRAList[i];
    myCGRA.Insert_TimeCGRA(cgraTime);
  }

  return myCGRA;
}

void CGRA::CalcPEUtil(float &PEUtil)
{
  int busyPEs = 0;
  for (int timeIndex = 0; timeIndex < II; timeIndex++)
  {
    TimeCGRA* cgraTime = timeCGRAList[timeIndex];
    PE* candidatePE;
    for (int i = 0; i < SizeX; i++)
    {
      for (int j = 0; j < SizeY; j++)
      {
        candidatePE = cgraTime->getPE(i, j);
        if (candidatePE->isBusy())
          busyPEs++;
      }
    }
  }
  PEUtil = ((float)(busyPEs))/((float)II);
}

bool CGRA::IsCompatible(int node1, int node2)
{
  bool retVal = false;
  int tableSize = (int) mapPairs.size();

  MappingPair* mp1;
  MappingPair* mp2;

  for (int i = 0; i < tableSize; i++)
  {
    mp1 = mapPairs[i];

    if(mp1->getNode()->get_ID() != node1)
      continue;

    // iterate to all vertices with operation same as node2 and
    // see if there is a compatibility
    for (int j = 0; j < tableSize; j++)
    {
      mp2 = mapPairs[j];
      if(mp2->getNode()->get_ID() != node2)
        continue;

      if(table_without_r[i * tableSize + j] > 0)
      {
        retVal = true;
        return retVal;
      }
    }
  }

  return retVal;
}

void CGRA::makeRFTable()
{
  rf_table = new char[II * SizeX * SizeY];
  kernel_table = new char[II * SizeX * SizeY];
  for (int timeIndex = 0; timeIndex < II; timeIndex++)
  {
    TimeCGRA* cgraTime = timeCGRAList[timeIndex];
    PE* candidatePE;

    for (int i = 0; i < SizeX; i++)
    {
      for (int j = 0; j < SizeY; j++)
      {
        candidatePE = cgraTime->getPE(i, j);
        if (candidatePE->isBusy())
        {
          rf_table[timeIndex*SizeX*SizeY + SizeY*i + j] =  (int) candidatePE->NumberOfUsedRegisters();
          kernel_table[timeIndex*SizeX*SizeY + SizeY*i + j] = (int) candidatePE->getNode()->get_ID();
        }
        else
        {
          rf_table[timeIndex*SizeX*SizeY + SizeY*i + j] = (int) 0;
          kernel_table[timeIndex*SizeX*SizeY + SizeY*i + j] = -1;
        }
      }
    }
  }
}

void CGRA::getMinMaxReg(int &Rmin, int &Rmax)
{
  int reg_min_pe;
  int r_min = 65000;
  int r_max = 0;

  for(int pe_no = 0; pe_no < SizeX*SizeY; pe_no++)
  {
    reg_min_pe = 65000;	//Assign High Random Value
    for(int time = 0; time < II; time++)
    {
      int reg = rf_table[time*SizeX*SizeY + pe_no];

      if(reg < 0) FATAL(true,"Should not reach here.");

      if(reg < reg_min_pe)
        reg_min_pe = reg;
    }

    if(reg_min_pe < r_min)
      r_min = reg_min_pe;
    else if(reg_min_pe > r_max)
      r_max = reg_min_pe;
  }

  Rmin = SizeReg - r_max;
  Rmax = SizeReg - r_min;
}

void CGRA::UpdateNeedToLoadConstants(DFG* myDFG)
{
  int tableSize = (int) mapPairs.size();
  MappingPair* mp1;

  // iterate through all the mapping pairs
  for (int index1 = 0; index1 < tableSize; index1++)
  {
    mp1 = mapPairs[index1];
    Node* currentNode = mp1->getNode();

    if(currentNode->hasLargeConst == 0)
      continue;

    // Set Compatibility zero for placing the node
    for(int index2 = 0; index2 < tableSize; index2++)
    {
      if(index1 != index2)
        continue;

      table[index1 * tableSize + index2] += currentNode->hasLargeConst;
    }

    //Update To All The PEs Who Access Same Local RF
    for (int index4 = 0; index4 < tableSize; index4++)
    {
      if (index4 == index1)
        continue;

      MappingPair* mp2 = mapPairs[index4];
      if (!mp1->SameCoordinationPE(mp2))
        continue;

      if (table[index1 * tableSize + index4] == 0)
        continue;

      table[index4 * tableSize + index1] += currentNode->hasLargeConst;
    }
  }
}
