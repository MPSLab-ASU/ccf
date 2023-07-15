/*
 * CGRA.cpp
 *
 *  Created on: Aug 10, 2012
 *  Author: mahdi
 *  Last Edited on: Jan 28, 2017
 *  Author: Shail Dave
 *
 *  Created Ullman algorithm: Sept, 2019
 *  Author Mahesh Balasubramanian
 */

#include "CGRA.h"
#include "INTERITERATIONDEPENDENCY.h"
#include <numeric>
#include <cmath>
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
          //kernelFile << candidatePE->getNode()->get_ID()<<"\t"<< usedReg <<"\t"<< candidatePE->getNode()->get_Sched_Info()->get_Current() << "\n";
          kernelFile << candidatePE->getNode()->get_ID()<<"\t"<< usedReg <<"\t"<< candidatePE->getNode()->get_Sched_Info()->get_Modulo_Current() << "\n";
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

  template <typename T>
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

  /*for (int i = 0; i < tableSize; i++)
    {
    mp1 = mapPairs[i];
    for (int j = 0; j < tableSize; j++)
    {
    if (i == j)
    continue;

    else
    {
    mp2 = mapPairs[j];
    adj_g[i][j] = Compatible2(mp1, mp2, myDFG);
  //adj_g_without_r[i][j] = Compatible(mp1, mp2, myDFG);
  }
  }
  }*/
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
          //if(!contains_list(adjacency_list[j], i))
          //  adjacency_list[j].push_back(i);
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
}

Matrix<bool> CGRA::GenerateAdjacencyTable2(DFG* myDFG)
{

  vector<Node*> nodes = myDFG->getSetOfVertices();
  int nodeSize = (int) nodes.size();
  Matrix<bool> adj(nodeSize, nodeSize); 
  vector<ARC*> arc = myDFG->getSetOfArcs();

  //for(int i=0; i< (int) arc.size(); i++)
  // cout << arc[i]->get_From_Node()->get_ID() << "\t" <<  arc[i]->get_To_Node()->get_ID() << "\t" << arc[i]->get_Dependency() << endl; 
  for(int i=0; i < nodeSize; i++)
  {
    for(int j=0; j < nodeSize; j++)
    {
      if(i==j)
        continue;

      if(connected_Nodes2(nodes[i], nodes[j]))
        adj[i][j] = true; 
      else
        adj[i][j] = false;
    }
  }

  return adj;
}

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


  template <typename T>
vector<T> CGRA::difference(vector<T> first, vector<T> second)
{
  sort(first.begin(), first.end());
  sort(second.begin(), second.end());
  vector<T> v;

  set_difference(first.begin(), first.end(), second.begin(), second.end(), inserter(v, v.end())); 
  /*for(int i=0; i < (int) first.size(); i++)
    {
    for(int j=0; j < (int) second.size(); j++)
    {
    if(first[i] == second[j])
    continue;
    else
    v.push_back(first[i]);
    }
    }*/

  return v;
}


bool CGRA::contains_row(vector<int>& rows, int row_idx)
{
  for(int i=0;i<(int) rows.size(); i++)
    if(rows[i] != row_idx)
      return false;

  return true;
}

bool CGRA::contains_list(list<int> adjacency_list, int node)
{

  if(adjacency_list.size() == 0)
    return false;

  for(list<int>::iterator i = adjacency_list.begin(); i != adjacency_list.end(); ++i)
    if(*i == node)
      return true;

  return false;

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
  if(TEC[(sched*SizeX)+X][Y] == -100)
    return true;

  return false;
}

bool CGRA::is_already_mapped(Node* node1)
{
  for(int i=0; i<TEC.rows(); i++)
    for(int j=0; j<TEC.cols(); j++)
      if(TEC[i][j] == node1->get_ID())
        return true;
  return false;
}

tuple<int, int, int> CGRA::get_mapped_coordinates(Node* node1)
{
  int x, y, t;
  for(int i=0; i<TEC.rows(); i++)
  {
    for(int j=0; j<TEC.cols(); j++)
    {
      if(TEC[i][j] == node1->get_ID())
      {
        t=i; y=j;
      }  
    }
  }

  x=t%SizeX;
  return make_tuple(x,y,t);
}

int CGRA::get_Node_Index(Node * node1)
{

  for(int i=0; i < (int) _node_Set.size(); i++)
    if(_node_Set[i]->get_ID() == node1->get_ID())
      return i;

  return -1;
}

Node* CGRA::get_Node_from_Set(int number)
{
  for(int i=0; i < (int) _node_Set.size(); i++)
    if(i == number)
      return _node_Set[i];

  FATAL(true, "Node with ID=%d is not found", number);
  printf("Node with ID=%d is not found", number);
  return NULL;

}

int CGRA::get_mapped_mappairs(Node* node1)
{
  int x=0, ret=0;

  for(int i=0; i < (int) _node_Set.size(); i++)
    if(_node_Set[i]->get_ID() == node1->get_ID())
      x = i;

  for(int j=0; j< (int) M.cols(); j++)
    if(M[x][j])
      ret = j;

  return ret;

}


vector<int> CGRA::get_nodes_mapped_to_row( int row )
{
  vector<int> ret; 

  for(int j=0; j < TEC.cols(); j++)
    if(TEC[row][j] >= 0)
      ret.push_back(TEC[row][j]);

  return ret; 
}

bool CGRA::Sanity_M()
{
  int value = 1;
  vector<int> temp;
  for(int i = 0; i < (int) M.rows() ; i++)
  {
    temp = M.row_find(i, value);
    if(!((int) temp.size() == 1))
      return false;

    temp.clear();
  }

  for(int j = 0; j < (int) M.cols(); j++)
  {
    temp = M.col_find(j, value);
    if((int) temp.size() > 1)
      return false;
  }

  return true;

}


template <typename T>
vector<size_t> CGRA::sort_indexes(const vector<T> &v) {

  // initialize original index locations
  vector<size_t> idx(v.size());
  iota(idx.begin(), idx.end(), 0);

  // sort indexes based on comparing values in v
  sort(idx.begin(), idx.end(),
      [&v](size_t i1, size_t i2) {return v[i1] > v[i2];});

  return idx;
}


void CGRA::Remove(vector<int>& nodeset, int value)
{
  vector<int>::iterator iNode;
  for (iNode = nodeset.begin(); iNode != nodeset.end(); iNode++)
  {
    if ((*iNode) == value)
    {
      nodeset.erase(iNode, iNode + 1);
      return;
    }
  }

}


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


// function to update Pred and Succ Indices based on current position.
void CGRA::Update_Pred_Succ_Indices(Node* current)
{
  vector<Node* > P; 
  vector<Node* > S; 
  tie(P,S) = get_Mapped_pred_succ(current);

  //for each pred update its position based on current mapping.
  for(int i=0; i < (int) P.size(); i++)
  {
    //container for mapped predecessor of predecessor to current
    vector<Node* > pred_pred;
    //container for mapped successor of the predecessor to current
    vector<Node* > succ_pred;
    tie(pred_pred,succ_pred) = get_Mapped_pred_succ(P[i]);

    //For each pred get free coordinates mapped.
    vector<MappingPair*> indices;
    int map_case;
    indices.clear();
    vector<int> ind = M.row_find(get_Node_Index(P[i]), 1);
    vector<int> pos;
    tie(indices, pos, map_case) = Get_Free_Coordinates(P[i], pred_pred, succ_pred, ind);

    //indices vector holds the updated array with atleast one position because it is mapped now.
    //if not push empty.
    P[i]->SetMappableIndices(indices); 
  }

  //for each succ update its position based on current mapping.
  for(int i=0; i < (int) S.size(); i++)
  {
    //container for mapped predecessor of successor to current
    vector<Node* > pred_succ;
    //container for mapped successor of the successor to current
    vector<Node* > succ_succ;
    tie(pred_succ,succ_succ) = get_Mapped_pred_succ(S[i]);

    //For each pred get free coordinates mapped.
    vector<MappingPair*> indices;
    int map_case;
    indices.clear();
    vector<int> ind = M.row_find(get_Node_Index(S[i]), 1);
    vector<int> pos;
    tie(indices, pos, map_case) = Get_Free_Coordinates(S[i], pred_succ, succ_succ, ind);

    //indices vector holds the updated array with atleast one position because it is mapped now.
    //if not push empty.
    S[i]->SetMappableIndices(indices);
  }
}


/*  
    1. The main aim of the refine function is to reduce the number of 1's the m' matrix.
    2. We consider mapPairs, incoming outgoing arc to a node and the schedule time to 
    reduce the number of 1's.
    3. The resultant m' should have exactly one 1 in each row 
    4. the resultant m' should have no more one 1 in each column.
 */
bool CGRA::refine_M(DFG* myDFG)
{
  _node_Set = myDFG->getSetOfVertices(); 
  nodeSize = (int) _node_Set.size();


  vector<int> load_nodes;
  vector<int> fanin_number;
  vector<int> fanout_number;
  vector<int> priority_number;
  vector<int> map_degree;


  /*for(int i = 0 ; i < nodeSize; i++)
    {
    if(_node_Set[i]->is_Load_Address_Generator())
    {
    load_nodes.push_back(i);
    continue;
    }
    if(_node_Set[i]->is_Load_Data_Bus_Read() || _node_Set[i]->is_Store_Address_Generator() || _node_Set[i]->is_Store_Data_Bus_Write())
    continue;
    fanin_number.push_back(_node_Set[i]->get_Number_of_Operands());
    fanout_number.push_back(_node_Set[i]->get_Number_of_Succ()); 
    priority_number.push_back(i);
    }*/

  //for(int i=0; i < (int) fanin_number.size(); i++)
  //  map_degree.push_back(fanin_number[i]+fanout_number[i]);

  /*for(int i =0 ; i < (int)load_nodes.size() ; i++)
    cout << "loads: " << _node_Set[load_nodes[i]]->get_ID() << endl;

    for(int i=0; i< (int)priority_number.size() ; i++)
    {
    cout << "node: " << _node_Set[priority_number[i]]->get_ID() << "\tdegree: " << fanin_number[i]+fanout_number[i] << endl;

    }

    for (auto i: sort_indexes(map_degree)) {
    cout << i << "\t"  << map_degree[i] << endl;
    }*/
  int mappingCounter=-1;
  bool mappingFound = false;

  int left_nodes_x=0;
  vector<Node*> left_nodes_unmapped_x;
  Matrix<int> TEC_x(TEC.rows(), TEC.cols());
  //vector<Node*> recurrent_nodes = myDFG->get_nodes_with_recurrent_edges_incoming();

  //vector<int> recurrent_idx;

  //for(int j=0; j < (int) recurrent_nodes.size(); j++)
  //  recurrent_idx.push_back(get_Node_Index(recurrent_nodes[j]));

  Max_Search_Space = (int) (MAPPING_POLICY.MAX_MAPPING_ATTEMPTS * II * SizeX * SizeY * nodeSize);
  cout << "Max search space : " << Max_Search_Space << endl;  

  while(mappingCounter < Max_Search_Space)
  {
    mappingCounter++;
    cout << "mappingCounter: " << mappingCounter << endl;
    //Mark all the vertices as not visited;
    visited = new bool[nodeSize];
    for(int i = 0; i < nodeSize; i++)
      visited[i] = false;

    //clear all containers
    left_nodes_unmapped.clear();
    M = copy_M;
    TEC.fill(INF); 
    row_idx.clear();
    Load_nodes.clear();
    vector<int> random_pick;
    int visited_counter=-1;
    //Random selection
    //int node_sel;
    int it = -1; 
    if(MAPPING_POLICY.MAPPING_MODE == 0) // completely random
    {
      it++;
      //cout << "Completely random mapping" << endl; 
      random_pick.clear();
      while(!all_visited(nodeSize))
      {
        visited_counter++; 
        for(int j = 0 ; j < nodeSize; j++)   
        {
          if(!visited[j])
            random_pick.push_back(j);
        }

        int  node_sel = random_pick[rand()%(int)random_pick.size()];
        //cout << "node_sel: " << node_sel << endl;
        BFS_refine_impl(myDFG, node_sel);

      }
    }
    else if(MAPPING_POLICY.MAPPING_MODE == 1)//Priority of nodes having outgoing recurrent edges.
    {
      vector<Node*> recurrent_nodes = myDFG->get_nodes_with_recurrent_edges_incoming();

      vector<int> recurrent_idx;

      for(int j=0; j < (int) recurrent_nodes.size(); j++)
        recurrent_idx.push_back(get_Node_Index(recurrent_nodes[j]));

      while(!all_visited(nodeSize))
      {
        for(int j=0; j < (int) recurrent_idx.size(); j++)
        {
          if(!visited[recurrent_idx[j]])
            BFS_refine_impl(myDFG, recurrent_idx[j]);
        } 
        random_pick.clear();
        for(int j = 0 ; j < nodeSize; j++)
        {
          if(!visited[j])
            random_pick.push_back(j);
        }
        int node_sel = random_pick[rand()%(int)random_pick.size()];
        BFS_refine_impl(myDFG, node_sel);
      }
    }
    else if(MAPPING_POLICY.MAPPING_MODE == 2) // Priority of having incoming recurrent
    {
      vector<Node*> recurrent_nodes = myDFG->get_nodes_with_recurrent_edges_outgoing();

      vector<int> recurrent_idx;

      for(int j=0; j < (int) recurrent_nodes.size(); j++)
        recurrent_idx.push_back(get_Node_Index(recurrent_nodes[j]));

      while(!all_visited(nodeSize))
      {
        for(int j=0; j < (int) recurrent_idx.size(); j++)
        {
          if(!visited[recurrent_idx[j]])
            BFS_refine_impl(myDFG, recurrent_idx[j]);
        }
        random_pick.clear();
        for(int j = 0 ; j < nodeSize; j++)
        {
          if(!visited[j])
            random_pick.push_back(j);
        }
        int node_sel = random_pick[rand()%(int)random_pick.size()];
        BFS_refine_impl(myDFG, node_sel);
      }
    }
    else if(MAPPING_POLICY.MAPPING_MODE == 3) // priority of high fan-in fan-out. 
    {
      vector<int> map_degree;
      vector<int> map_idx;
      for(int i = 0 ; i < nodeSize; i++)
        map_degree.push_back(_node_Set[i]->get_Number_of_Operands()+ _node_Set[i]->get_Number_of_Succ());

      for (auto i: sort_indexes(map_degree))
        map_idx.push_back(i);

      while(!all_visited(nodeSize))
      {
        visited_counter++;
        for(int j = 0 ; j < (int) map_idx.size(); j++)
        {
          if(!visited[map_idx[j]])
            BFS_refine_impl(myDFG,map_idx[j]);
        }
      }
    }
    else if(MAPPING_POLICY.MAPPING_MODE == 4) // Priority of having no incoming nodes
    {

      vector<Node*> starting_nodes = myDFG->get_set_of_start_nodes();

      vector<int> starting_idx;

      for(int j=0; j < (int) starting_nodes.size(); j++)
        starting_idx.push_back(get_Node_Index(starting_nodes[j]));

      while(!all_visited(nodeSize))
      {
        for(int j=0; j < (int) starting_idx.size(); j++)
        {
          if(!visited[starting_idx[j]])
            BFS_refine_impl(myDFG, starting_idx[j]);
        }
        random_pick.clear();
        for(int j = 0 ; j < nodeSize; j++)
        {
          if(!visited[j])
            random_pick.push_back(j);
        }
        if(random_pick.size() > 0 )
        {
          int node_sel = random_pick[rand()%(int)random_pick.size()];
          BFS_refine_impl(myDFG, node_sel);
        }
      }
    }
    else if(MAPPING_POLICY.MAPPING_MODE == 5)  // priority of having no outgoing edges
    {
      cout << "Mapping with BFS with Mode: " << MAPPING_POLICY.MAPPING_MODE << endl;

      vector<Node*> starting_nodes = myDFG->get_set_of_end_nodes();

      cout << "got the starting nodes"  << endl;
      for(int j=0; j < (int) starting_nodes.size(); j++)
        cout << starting_nodes[j]->get_ID() << " "; 
      cout<<endl;
      vector<int> starting_idx;

      for(int j=0; j < (int) starting_nodes.size(); j++)
        starting_idx.push_back(get_Node_Index(starting_nodes[j]));

      for(int j=0; j < (int) starting_nodes.size(); j++)
      {
        cout << "starting node: " << endl;
        cout << starting_idx[j] << " " << endl;
        if(!visited[starting_idx[j]])
          BFS_refine_impl(myDFG, starting_idx[j]);

        if((int) left_nodes_unmapped.size() > 0)
          break; 
        //BFS(starting_idx[j]); 
      }

      vector<int> unvisited_nodes = get_unvisited_nodes(nodeSize);

      if((int) left_nodes_unmapped.size() == 0) 
      {
        vector<int> unvisited_nodes = get_unvisited_nodes(nodeSize); 

        for(int j=0; j < (int) unvisited_nodes.size(); j++)
        {
          cout << "unvisted node: " << endl;
          cout << unvisited_nodes[j] << " " << endl;

          if(!visited[unvisited_nodes[j]])
            BFS_refine_impl(myDFG, unvisited_nodes[j]);

        }
      }
      //cout << "got the starting indices"  << endl;
      /*while(!all_visited(nodeSize))
        {
        for(int j=0; j < (int) starting_idx.size(); j++)
        {
        cout << "inside all visited now: " << starting_nodes[j]->get_ID() << endl;
        if(!visited[starting_idx[j]])
        BFS_refine_impl(myDFG, starting_idx[j]);
        }
      ///exit(1);
      random_pick.clear();
      for(int j = 0 ; j < nodeSize; j++)
      {
      if(!visited[j])
      random_pick.push_back(j);
      }
      if(random_pick.size() > 0 )
      {
      int node_sel = random_pick[rand()%(int)random_pick.size()];
      BFS_refine_impl(myDFG, node_sel);
      }
      }*/
    }

    //int left_nodes_x; 
    //vector<Node*> left_nodes_unmapped_x;

    if((int) left_nodes_unmapped.size() > 0)
    {

      if(mappingCounter == 0)
      {
        left_nodes_x = (int) left_nodes_unmapped.size();  
        left_nodes_unmapped_x.clear();
        TEC_x.fill(0);
        TEC_x = TEC; 
        for(int ii = 0; ii < (int) left_nodes_unmapped.size(); ii++)
          left_nodes_unmapped_x.push_back(left_nodes_unmapped[ii]);
      }
      else
      {
        if((int) left_nodes_unmapped.size() < left_nodes_x)
        {
          left_nodes_unmapped_x.clear(); 
          TEC_x.fill(0); 
          TEC_x = TEC;
          left_nodes_x = (int) left_nodes_unmapped.size(); 
          for(int ii = 0; ii < (int) left_nodes_unmapped.size(); ii++)
            left_nodes_unmapped_x.push_back(left_nodes_unmapped[ii]);
        }
      }

      if(mappingCounter==Max_Search_Space) 
      {
        //  for(int ii=0; ii< (int) left_nodes_unmapped.size(); ii++)
        //    cout << "left node unmapped: " << left_nodes_unmapped[ii]->get_ID() << endl;

        cout << "left nodes vector: " << (int) left_nodes_unmapped_x.size() << endl;
        for(int ii=0; ii< (int) left_nodes_unmapped_x.size(); ii++)
          cout << "left node unmapped: " << left_nodes_unmapped_x[ii]->get_ID() << "\t Instr: " << left_nodes_unmapped_x[ii]->get_Instruction() << endl;

        TEC_x.print_matrix(); 
        cout << "lowest left_nodes: " << left_nodes_x << endl;
        cout << endl;
        cout << "No mapping with " << left_nodes_x << " left nodes. Check debug file" << endl; 
        //FATAL(true, "No mapping with %d left nodes. Check debug file", left_nodes_x) ;
        //exit(1);
      }
      continue;
      //exit(1);
    }
    else
    {
      cout << "left node size == 0" << endl;
      TEC.print_matrix();
      cout<<endl;
      //M.print_matrix();  
      //if(!Sanity_M())
      //  break;
      //else
      // {
      mappingFound = true; 
      break; 
      //}
    }
  }
  cout << "Total mapping steps: " << mappingCounter << endl;

  if(mappingFound)
    return true;  

  return false;
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

vector<MappingPair*> CGRA::Subtract(vector<MappingPair*> &set1, vector<MappingPair*> &set2)
{
  vector<MappingPair*> retVal;
  vector<MappingPair*>::iterator iNode1;
  for (iNode1 = set1.begin(); iNode1 != set1.end(); iNode1++)
  {
    //if iNode1 is not in set2, add it
    if (!contains(set2, (*iNode1)))
      retVal.push_back(*iNode1);
  }
  return retVal;
}

vector<MappingPair*> CGRA::Combine(vector<MappingPair*> set1, vector<MappingPair*> set2)
{
  vector<MappingPair*> retVal;
  vector<MappingPair*>::iterator iPair1;
  vector<MappingPair*>::iterator iPair2;

  for(iPair1 = set1.begin(); iPair1 != set1.end(); iPair1++)
    retVal.push_back(*iPair1); 

  for (iPair2 = set2.begin(); iPair2 != set2.end(); iPair2++)
  {
    if(!contains(set1, *iPair2)) 
      retVal.push_back(*iPair2) ;  

  }




  /*vector<MappingPair*>::iterator iPair2;
    for(iPair2 = set2.begin(); iPair2 != set2.end(); iPair2++)
    {
    for(iPair1 = set1.begin(); iPair1 != set1.end(); iPair1++)
    {
    if((*iPair1)->getNode()->get_ID() == (*iPair2)->getNode()->get_ID())
    {
    retVal.push_back(*iPair2);  
    break; 
    }
    }
    }*/

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
  //TEC.print_matrix();

  FATAL(true, "Node with ID=%d is not found", number);
  printf("Node with ID=%d is not found", number);
  return NULL;
}

/*bool CGRA::map_load_addr(vector<Node*> load_nodes, vector<int> load_visited)
  {
  vector<Node*> unmapped = load_nodes;
  vector<Node*> mapped;
  int load_it = -1; 
  int value = 1; 
  while((int) unmapped.size() > 0)
  {
  load_it++;
  Node* current_node = unmapped[0];
//for (int ii = 0 ; ii < (int) load_visited.size(); ii++)
//  if(load_nodes[ii]->get_ID() == current_node->get_ID())
//    i = ii; 
int i = load_visited[load_it];
int sel; 
vector<int> indices = M.row_find(i, value);


vector<int> potential_pos, mapped_nodes;
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

//if((int) mapped_nodes.size() > 0 && (int) mapped_nodes.size() < SizeY)
//  for(int jj=0; jj < nodeSize; jj++)
//  for(int kk=0; kk < (int) mapped_nodes.size(); kk++)
//  if(_node_Set[jj]->get_ID() == mapped_nodes[kk])
//  if(_node_Set[jj]->is_Store_Address_Generator() || _node_Set[jj]->is_Load_Address_Generator())
//  found_mem=true;


// if(!found_mem)
// potential_pos.push_back(indices[ii]);
}
if(potential_pos.size() > 1)
sel = potential_pos[rand() % (int) potential_pos.size()];
else if(potential_pos.size() == 1)
sel = potential_pos[0];
else
{
//cout << "No potential postions for 1 input load generator and data bus\n";
left_nodes_unmapped.push_back(current_node);
//continue;
return false;
}

M.set_row_zero(i);
M.set_value(i, sel, 1);
AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
visited[i] = true;

mapped.push_back(current_node);
unmapped = Subtract(unmapped, mapped); 
}

return true; 

}*/ 

/*bool CGRA::map_load_data(vector<Node*> load_nodes, vector<int> load_visited)
  {
  vector<Node*> unmapped = load_nodes;
  vector<Node*> mapped;
  int load_it = -1; 
  int value = 1;
  while((int) unmapped.size() > 0)
  {
  load_it++;
  Node* current_node = unmapped[0];
  vector<Node*> incoming = current_node->Get_Prev_Nodes();
  int i = load_visited[load_it];
//for (int ii = 0 ; ii < (int) load_visited.size(); ii++)
//  if(load_nodes[ii]->get_ID() == current_node->get_ID())
//   i = ii;


int row=-1, sel;
vector<int> indices = M.row_find(i, value);
row = TEC.row_find(incoming[0]->get_ID()) % SizeX; 

// if the load addr is visited but not mapped, add this node to the unmapped list and continue. 
if(row == -1)
{
left_nodes_unmapped.push_back(current_node);
continue;
}

vector<int> potential_pos, mapped_nodes;
//int id_row = row_idx[pos];

for(int ii=0; ii < (int) indices.size(); ii++)  
{ 
// check if the PE is free, if not continue to the next position.
if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
continue;

mapped_nodes.clear(); 
mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());


if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
{
if((int) mapped_nodes.size() > 0)
{
for(int jj=0; jj< (int) mapped_nodes.size(); jj++)
if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
break;  
else
potential_pos.push_back(indices[ii]);
}
else
potential_pos.push_back(indices[ii]);
} 

//if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
//  {
//  if(mapped_nodes.size() > 0)
//  {
//  for(int kk=0; kk< nodeSize; kk++)
//  {
//  for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
//  {
//  if(_node_Set[kk]->get_ID() == mapped_nodes[jj])
//  {
//  if(!_node_Set[kk]->is_Load_Data_Bus_Read() && !_node_Set[kk]->is_Store_Data_Bus_Write() && !_node_Set[kk]->is_Store_Address_Generator()) //add connectedPEs for out>0
// {
// potential_pos.push_back(indices[ii]);
// break;
// }
// }
// }
// }

//}
// else    
// potential_pos.push_back(indices[ii]); 
} 
}

if(potential_pos.size() > 1)
  sel = potential_pos[rand() % (int) potential_pos.size()];
else if(potential_pos.size() == 1)
  sel = potential_pos[0];
  else
{ 
  //cout << "No potential postions for 1 input load generator and data bus\n"; 
  left_nodes_unmapped.push_back(current_node);
  //continue;
  cout << "No potential position for " << current_node->get_ID() << endl;
  //_FATAL("No potential postions for %d node:",  current_node->get_ID());    
  return false;
  // _FATAL("No potential postions for 1 input load generator and data bus\n");
}

//if(current_node->get_ID() == 25)
//{
//  cout << "from 0 1: sel is: " << sel << endl;
//  cout << "I row: " << mapPairs[sel]->getPE()->getIndexI() << endl;
//  cout << "row variable: " << row << endl;
// }

M.set_row_zero(i);
M.set_value(i, sel, 1);
AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());  

visited[i] = true;

mapped.push_back(current_node);
unmapped = Subtract(unmapped, mapped); 
}



return true; 
}
  */ 
void CGRA::print_adj(int nodeSize)
{
  for(int j=0; j < nodeSize ; j++)
  {
    cout << "j: " << j  << "\t -> ";
    for(list<int>::iterator i = adjacency_list[j].begin(); i != adjacency_list[j].end(); ++i)
      cout << *i << " ";
    cout<<endl;
  }

}

void CGRA::print_node_id(DFG* myDFG)
{
  cout<<endl;
  cout << "Node ids" << endl;
  vector<Node*> nodes = myDFG->getSetOfVertices(); 
  for(int j=0; j < (int) nodes.size() ; j++)
  {
    cout << "j: " << j  << "\t -> " << nodes[j]->get_ID() << endl;
  }

}

void CGRA::print_visited(int nodeSize)
{

  cout << "visited: "; 
  for(int i = 0 ; i < nodeSize; i++)
    cout << visited[i] << " "; 

  cout<<endl;

}

void CGRA::print_unvisited(int nodeSize)
{

  cout << "unvisited: ";
  for(int i = 0 ; i < nodeSize; i++)
    if(!visited[i])
      cout << get_Node_from_Set(i)->get_ID() << " ";

  cout<<endl;

}

vector<int> CGRA::get_unvisited_nodes(int nodeSize)
{
  vector<int> retVal;
  for(int i = 0 ; i < nodeSize; i++)
    if(!visited[i])
      retVal.push_back(i); 

  return retVal; 
}

bool CGRA::all_visited(int nodeSize)
{
  for(int i = 0 ; i < nodeSize; i++)
    if(!visited[i])
      return false;

  return true;
}

void CGRA::BFS(int s)
{
  // Create a queue for BFS 
  list<int> queue;

  // Mark the current node as visited and enqueue it 
  visited[s] = true;
  queue.push_back(s);

  // 'i' will be used to get all adjacent 
  // vertices of a vertex 
  list<int>::iterator i;

  while(!queue.empty())
  {
    // Dequeue a vertex from queue and print it 
    s = queue.front();
    cout << s << " ";
    queue.pop_front();

    // Get all adjacent vertices of the dequeued 
    // vertex s. If a adjacent has not been visited,  
    // then mark it visited and enqueue it 
    for (i = adjacency_list[s].begin(); i != adjacency_list[s].end(); ++i)
    {
      if (!visited[*i])
      {
        visited[*i] = true;
        queue.push_back(*i);
      }
    }
  }
}

bool CGRA::is_left_mapped(Node *node)
{
  for(int i=0; i<(int) left_nodes_unmapped.size(); i++)
  {
    if(node->get_ID() == left_nodes_unmapped[i]->get_ID())
      return true;
  }

  return false;
}

vector<Node*> CGRA::get_Nodes_from_current_slot(int time)
{
  vector<int> mapped_coordinates = TEC.find_mapped_coordinates(time, SizeX); 
  vector<Node*> mapped_nodes; 

  for(int i=0; i < (int) mapped_coordinates.size(); i++)
    mapped_nodes.push_back(get_Node(mapped_coordinates[i])); 
  return mapped_nodes; 
}

// remove node from nodeset
void CGRA::Remove(vector<Node*> &nodeset, Node* node)
{
  vector<Node*>::iterator iNode;
  for (iNode = nodeset.begin(); iNode != nodeset.end(); iNode++)
  {
    if ((*iNode)->get_ID() == node->get_ID())
    {
      nodeset.erase(iNode, iNode + 1);
      return;
    }
  }
}

void CGRA::Restore(Matrix<int> M2)
{
  assert(TEC.rows() == M2.rows()) ;
  assert(TEC.cols() == M2.cols()) ; 

  for(int r=0; r < M2.rows(); r++)
    for(int c=0; c < M2.cols(); c++)
      TEC[r][c] = M2[r][c]; 

  return; 
}

void CGRA::Restore_timeslot(int time)
{
  for(int i=0; i<SizeX; i++)
    for(int j=0; j<(int) copy_TEC.cols(); j++)
      TEC[(time*SizeX)+i][j] = copy_TEC[(time*SizeX)+i][j];

}

void CGRA::Matrix_Copy(Matrix<int> &M2)
{
  M2.reshape(TEC.rows(), TEC.cols()); 
  assert(TEC.rows() == M2.rows()) ;
  assert(TEC.cols() == M2.cols()) ;

  for(int r=0; r < TEC.rows(); r++)
    for(int c=0; c < TEC.cols(); c++)
      M2[r][c] = TEC[r][c];

  return;

}

// Remap all the nodes in the current timeslot given by the vector
// This function can be called from either the failure node timeslot, 
//  or from the pred and succ for 1-Deep.


bool CGRA::Remap(vector<Node*> nodes_in_timeslot)
{
  //to_be_mapped.clear() ; 
  //for(int i=0; i < (int) nodes_in_timeslot.size(); i++)
  //  to_be_mapped.push_back(nodes_in_timeslot[i]);

  // update the positions of all the nodes in the timeslot before backing

  for(int i=0; i < (int) nodes_in_timeslot.size(); i++)
    Update_Pred_Succ_Indices(nodes_in_timeslot[i]);
  // back up the indices and the current position into temp
  for(int i=0; i < (int) nodes_in_timeslot.size(); i++)
  {
    nodes_in_timeslot[i]->SetTempMappableIndices(nodes_in_timeslot[i]->GetMappedIndices()); 
    nodes_in_timeslot[i]->SetTempCurrentPosition(nodes_in_timeslot[i]->GetCurrentPosition()); 
  }

  // shuffle the nodes.
  //random_shuffle(to_be_mapped.begin(), to_be_mapped.end()); 

  if(Remap(nodes_in_timeslot, (int) nodes_in_timeslot.size()))
    return true;
  else
  {
    // clear out already mapped nodes in the TEC
    TEC.free_current_timeslot(nodes_in_timeslot[0]->get_Sched_Info()->get_Modulo_Current(), SizeX);

    // if remap not successfull back up from temp
    for(int i=0; i < (int) nodes_in_timeslot.size(); i++)
    {
      nodes_in_timeslot[i]->SetMappableIndices(nodes_in_timeslot[i]->GetTempMappedIndices());
      nodes_in_timeslot[i]->SetCurrentPosition(nodes_in_timeslot[i]->GetTempCurrentPosition());
    }
  }

  return false;
}

// This is a linear function to remap the nodes of a given timeslot.
bool CGRA::Remap(vector<Node*> to_be_mapped, int size)
{
  for(int i=0;i<(int) to_be_mapped.size(); i++)
  {
    Node* v= to_be_mapped[i];
    vector<Node*> P;
    vector<Node*> S;
    P.clear(); S.clear(); 
    tie(P,S) = get_Mapped_pred_succ(v);

    vector<MappingPair*> indices;
    int map_case; 
    indices.clear();
    vector<int> ind = M.row_find(get_Node_Index(v), 1);
    vector<int> pos;
    tie(indices, pos, map_case) = Get_Free_Coordinates(v, P, S, ind);

    if((int) indices.size() == 0)
      return false;

    for(int j=0; j < (int) indices.size(); j++)
    {
      if(!isTECfree(indices[j]->getPE()->getIndexI(), indices[j]->getPE()->getIndexJ(), indices[j]->getPE()->getIndexT()))
        continue;

      v->SetCurrentPosition(indices[j]);

      AssignTEC(indices[j]->getPE()->getIndexI(), indices[j]->getPE()->getIndexJ(), indices[j]->getPE()->getIndexT(), indices[j]->getNode()->get_ID());

    }
    if( v->GetCurrentPosition() == NULL)
      return false;
  }

  return true; 

}

/*bool CGRA::Remap(vector<Node*> &to_be_mapped, int size)
  {
  if((int) to_be_mapped.size() == 0)
  return true; 

  Node* v = to_be_mapped.back();
  to_be_mapped.pop_back();

  cout << "Remap v: " << v->get_ID() << endl;

  vector<Node*> P;
  vector<Node*> S;
  tie(P,S) = get_Mapped_pred_succ(v);

  vector<MappingPair*> indices;
  int map_case; 
  indices.clear();
  vector<int> ind = M.row_find(get_Node_Index(v), 1);
  vector<int> pos;
  tie(indices, pos, map_case) = Get_Free_Coordinates(v, P, S, ind);

  if((int) indices.size() == 0)
  {
  to_be_mapped.push_back(v);
  return false;
  }
  v->SetMappableIndices(indices); 
// shuffle the coordinates.
//random_shuffle(indices.begin(), indices.end());   

for(int i=0; i < (int) indices.size(); i++)
{
if(!isTECfree(indices[i]->getPE()->getIndexI(), indices[i]->getPE()->getIndexJ(), indices[i]->getPE()->getIndexT()))
continue;

v->SetCurrentPosition(indices[i]);

AssignTEC(indices[i]->getPE()->getIndexI(), indices[i]->getPE()->getIndexJ(), indices[i]->getPE()->getIndexT(), indices[i]->getNode()->get_ID());

if(Remap(to_be_mapped, size))
return true;
else
{   
UnAssignTEC(indices[i]->getPE()->getIndexI(), indices[i]->getPE()->getIndexJ(), indices[i]->getPE()->getIndexT(), indices[i]->getNode()->get_ID());
}

}


to_be_mapped.push_back(v);
return false;
}*/

bool CGRA::Smart_Map_Recovery_Node_Shallow(Node* n, int map_case)
{
  // First update the predecessor and successor positions of the current node.
  Update_Pred_Succ_Indices(n);

  //cout << "inside smart map node shallow map_case: " << map_case << endl;
  vector<int> ind = M.row_find(get_Node_Index(n), 1);


  //cout << "passed ind" << endl;

  for(int i=0; i < (int) ind.size(); i++)
  {

    if(!isTECfree(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT()))
      continue;

    switch(map_case)  
    {
      case 0:
        {
          vector<int> potential_pos;
          if(n->is_Load_Address_Generator())
          {
            vector<int> mapped_nodes;
            mapped_nodes.clear();
            mapped_nodes = get_nodes_mapped_to_row((mapPairs[ind[i]]->getPE()->getIndexT()*SizeX)+mapPairs[ind[i]]->getPE()->getIndexI());
            //bool found_mem=false;
            if(((int) mapped_nodes.size()) > 0)
            {
              for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
              {
                if(get_Node(mapped_nodes[jj])->is_Load_Address_Generator() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                  break;
                else
                {
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }
            }
            else
            {     
              MappingPair* mappair = mapPairs[ind[i]];
              mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
              AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
              return true;
            }
          }
          else
          {
            //since there are no in and out nodes mapped, no need to find connectedPEs                
            MappingPair* mappair = mapPairs[ind[i]];
            mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
            AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
            return true;
          }
          break; 
        } 
      case 1:
        {
          vector<Node*> P;
          vector<Node*> S;

          tie(P,S) = get_Mapped_pred_succ(n);

          vector<MappingPair*> pred_ind = P[0]->GetMappedIndices();
          MappingPair* pred_pos = P[0]->GetCurrentPosition();

          // support for load_data nodes.
          bool is_load_add = P[0]->is_Load_Address_Generator(); 

          UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());

          for(int j=0; j < (int) pred_ind.size(); j++)
          {
            if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
              continue;


            if(!is_load_add)
            {
              if(connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
              {
                AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                P[0]->SetCurrentPosition(pred_ind[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;

              }
            }
            else
            {
              if(pred_ind[j]->getPE()->getIndexI() == mapPairs[ind[i]]->getPE()->getIndexI())
              {
                AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                P[0]->SetCurrentPosition(pred_ind[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;
              }  
            }
          }
          AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());

          break;
        }
      case 2:
        {
          vector<Node*> P;
          vector<Node*> S;

          tie(P,S) = get_Mapped_pred_succ(n);
          vector<MappingPair*> pred_ind1 = P[1]->GetMappedIndices();
          vector<MappingPair*> pred_ind = P[0]->GetMappedIndices();
          MappingPair* pred_pos = P[0]->GetCurrentPosition();
          MappingPair* pred_pos1 = P[1]->GetCurrentPosition();

          if(connectedPEs(pred_pos->getPE(),mapPairs[ind[i]]->getPE()) && !connectedPEs(pred_pos1->getPE(), mapPairs[ind[i]]->getPE()))
          {
            //cout << "inside if" << endl;
            UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
            for(int j=0; j < (int) pred_ind1.size(); j++)
            {
              if(!isTECfree(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(pred_ind1[j]->getPE(), mapPairs[ind[i]]->getPE()))
              { 
                AssignTEC(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT(), pred_ind1[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                P[1]->SetCurrentPosition(pred_ind1[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;

              }
            }
            AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
          }
          else if(!connectedPEs(pred_pos->getPE(),mapPairs[ind[i]]->getPE()) && connectedPEs(pred_pos1->getPE(), mapPairs[ind[i]]->getPE()))
          {
            //cout << "inside else if" << endl;     
            UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());

            for(int j=0; j < (int) pred_ind.size(); j++)
            {
              if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(pred_ind[j]->getPE(),mapPairs[ind[i]]->getPE()))
              {
                AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                P[0]->SetCurrentPosition(pred_ind[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;
              }
            }
            AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
          }
          else if(!connectedPEs(pred_pos->getPE(),mapPairs[ind[i]]->getPE()) && !connectedPEs(pred_pos1->getPE(), mapPairs[ind[i]]->getPE()))
          {
            //cout << "inside else" << endl;
            UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
            UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            for(int j=0; j < (int) pred_ind1.size(); j++)
            {
              if(!isTECfree(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT()))
                continue;
              for(int k=0; k < (int) pred_ind.size(); k++)
              {
                if(!isTECfree(pred_ind[k]->getPE()->getIndexI(), pred_ind[k]->getPE()->getIndexJ(), pred_ind[k]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(pred_ind[k]->getPE(),mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind1[j]->getPE(), mapPairs[ind[i]]->getPE()))
                {
                  AssignTEC(pred_ind[k]->getPE()->getIndexI(), pred_ind[k]->getPE()->getIndexJ(), pred_ind[k]->getPE()->getIndexT(), pred_ind[k]->getNode()->get_ID());
                  AssignTEC(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT(), pred_ind1[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  P[0]->SetCurrentPosition(pred_ind[k]);
                  P[1]->SetCurrentPosition(pred_ind1[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }
            }
            AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
            AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
          }
          else
          {
            MappingPair* mappair = mapPairs[ind[i]];
            mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
            AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
            return true;
            // _FATAL("should not come here! case 9 shallow node cp1: %d, cp2:%d, cs1: %d\n", connected_p1, connected_p2, connected_p3);
          }
          break;
        }
      case 3:
        {

          vector<Node*> P;
          vector<Node*> S;

          tie(P,S) = get_Mapped_pred_succ(n);

          vector<MappingPair*> pred_ind = P[0]->GetMappedIndices();
          vector<MappingPair*> pred_ind1 = P[1]->GetMappedIndices();
          vector<MappingPair*> pred_ind2 = P[2]->GetMappedIndices();
          MappingPair* pred_pos2 = P[2]->GetCurrentPosition();
          MappingPair* pred_pos1 = P[1]->GetCurrentPosition();
          MappingPair* pred_pos = P[0]->GetCurrentPosition();

          bool connected_p1 = false, connected_p2 = false, connected_p3 = false; 

          if(connectedPEs(pred_pos->getPE(), mapPairs[ind[i]]->getPE()))
            connected_p1 = true;

          // cout << "passed connected_p" << endl;
          if(connectedPEs(pred_pos2->getPE(), mapPairs[ind[i]]->getPE()))
            connected_p3 = true; 

          // cout << "passed connected s1" << endl;
          if(connectedPEs(pred_pos1->getPE(), mapPairs[ind[i]]->getPE()))
            connected_p2 = true;

          //  cout << "pased connected s2" << endl;


          if(connected_p1 && connected_p2 && !connected_p3)
          {
            //cout << "inside 1" << endl;
            UnAssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());

            for(int j=0; j < (int) pred_ind2.size(); j++)
            {
              if(!isTECfree(pred_ind2[j]->getPE()->getIndexI(), pred_ind2[j]->getPE()->getIndexJ(), pred_ind2[j]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(pred_ind2[j]->getPE(), mapPairs[ind[i]]->getPE()))
              {
                AssignTEC(pred_ind2[j]->getPE()->getIndexI(), pred_ind2[j]->getPE()->getIndexJ(), pred_ind2[j]->getPE()->getIndexT(), pred_ind2[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                P[2]->SetCurrentPosition(pred_ind2[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;

              }
            }
            AssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());

          }
          else if(connected_p1 && !connected_p2 && connected_p3)
          {
            //cout << "inside 2" << endl;
            UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());

            for(int j=0; j < (int) pred_ind1.size(); j++)
            {
              if(!isTECfree(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(pred_ind1[j]->getPE(), mapPairs[ind[i]]->getPE()))
              {
                AssignTEC(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT(), pred_ind1[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                P[1]->SetCurrentPosition(pred_ind1[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;
              }
            }
            AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
          }
          else if(!connected_p1 && connected_p2 && connected_p3)
          {
            //cout << "inside 3" << endl;
            UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            for(int j=0; j < (int) pred_ind.size(); j++)
            {
              if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
              {
                AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                P[0]->SetCurrentPosition(pred_ind[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;
              }
            }

            AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
          }
          else if(connected_p1 && !connected_p2 && !connected_p3)
          {
            //cout << "inside 4" << endl;
            UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
            UnAssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());
            for(int j=0; j < (int) pred_ind1.size(); j++)
            {
              if(!isTECfree(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT()))
                continue;
              for(int k=0; k < (int) pred_ind2.size(); k++)
              {
                if(!isTECfree(pred_ind2[k]->getPE()->getIndexI(), pred_ind2[k]->getPE()->getIndexJ(), pred_ind2[k]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(pred_ind2[k]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind1[j]->getPE(), mapPairs[ind[i]]->getPE()))
                {
                  AssignTEC(pred_ind2[k]->getPE()->getIndexI(), pred_ind2[k]->getPE()->getIndexJ(), pred_ind2[k]->getPE()->getIndexT(), pred_ind2[k]->getNode()->get_ID());
                  AssignTEC(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT(), pred_ind1[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  P[2]->SetCurrentPosition(pred_ind2[k]);
                  P[1]->SetCurrentPosition(pred_ind1[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }
            }
            AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
            AssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());

          }
          else if(!connected_p1 && connected_p2 && !connected_p3)
          {
            //cout << "inside 5" << endl;
            UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            UnAssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());
            for(int j=0; j < (int) pred_ind.size(); j++)
            {
              if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                continue;
              for(int k=0; k < (int) pred_ind2.size(); k++)
              {
                if(!isTECfree(pred_ind2[k]->getPE()->getIndexI(), pred_ind2[k]->getPE()->getIndexJ(), pred_ind2[k]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(pred_ind2[k]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                {
                  AssignTEC(pred_ind2[k]->getPE()->getIndexI(), pred_ind2[k]->getPE()->getIndexJ(), pred_ind2[k]->getPE()->getIndexT(), pred_ind2[k]->getNode()->get_ID());
                  AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  P[2]->SetCurrentPosition(pred_ind2[k]);
                  P[0]->SetCurrentPosition(pred_ind[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }
            }
            AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            AssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());

          }
          else if(!connected_p1 && !connected_p2 && connected_p3)
          {
            //cout << "inside 6" << endl;
            UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
            for(int j=0; j < (int) pred_ind.size(); j++)
            {
              if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                continue;
              for(int k=0; k < (int) pred_ind1.size(); k++)
              {
                if(!isTECfree(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(pred_ind1[k]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                {
                  AssignTEC(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT(), pred_ind1[k]->getNode()->get_ID());
                  AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  P[1]->SetCurrentPosition(pred_ind1[k]);
                  P[0]->SetCurrentPosition(pred_ind[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }
            }
            AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
          }
          else if(!connected_p1 && !connected_p2 && !connected_p3)
          {
            //cout << "inside 7" << endl;
            UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
            UnAssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());
            for(int j=0; j < (int) pred_ind.size(); j++)
            {
              if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                continue;
              for(int k=0; k < (int) pred_ind1.size(); k++)
              {
                if(!isTECfree(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT()))
                  continue;
                for(int l=0; l < (int) pred_ind2.size(); l++)
                {
                  if(!isTECfree(pred_ind2[l]->getPE()->getIndexI(), pred_ind2[l]->getPE()->getIndexJ(), pred_ind2[l]->getPE()->getIndexT()))
                    continue;
                  if(connectedPEs(pred_ind1[k]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind2[l]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                  {
                    AssignTEC(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT(), pred_ind1[k]->getNode()->get_ID());
                    AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                    AssignTEC(pred_ind2[l]->getPE()->getIndexI(), pred_ind2[l]->getPE()->getIndexJ(), pred_ind2[l]->getPE()->getIndexT(), pred_ind2[l]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    P[1]->SetCurrentPosition(pred_ind1[k]);
                    P[2]->SetCurrentPosition(pred_ind2[l]);
                    P[0]->SetCurrentPosition(pred_ind[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                  }

                }

              }
            }
            AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
            AssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());
          }
          else
          {
            MappingPair* mappair = mapPairs[ind[i]];
            mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
            AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
            return true;
            // _FATAL("should not come here! case 9 shallow node cp1: %d, cp2:%d, cs1: %d\n", connected_p1, connected_p2, connected_p3);
          } 
          //_FATAL("In Node recovery shallow map_case: %d\n", map_case);
          break;
        }
      case 4:
        {
          cout << "inside case 4" << endl;
          vector<Node*> P;
          vector<Node*> S;

          tie(P,S) = get_Mapped_pred_succ(n);
          vector<MappingPair*> succ_ind = S[0]->GetMappedIndices();
          MappingPair* succ_pos = S[0]->GetCurrentPosition();
          UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

          bool is_load_add = n->is_Load_Address_Generator(); 

          //cout << "succ no: " << S[0]->get_ID() << endl;
          //for(int j=0; j < (int) succ_ind.size(); j++)
          //  cout << "positions: " << succ_ind[j]->getPE()->getIndexI() << " " << succ_ind[j]->getPE()->getIndexJ() << " " << succ_ind[j]->getPE()->getIndexT() << endl;

          for(int j=0; j < (int) succ_ind.size(); j++)
          { 
            if(!isTECfree(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT()))
              continue;

            if(!is_load_add)
            {
              if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[j]->getPE()))
              {  
                AssignTEC(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT(), succ_ind[j]->getNode()->get_ID()); 
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                S[0]->SetCurrentPosition(succ_ind[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true; 

              }
            }
            else
            {
              if(succ_ind[j]->getPE()->getIndexI() == mapPairs[ind[i]]->getPE()->getIndexI())
              {
                AssignTEC(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT(), succ_ind[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                S[0]->SetCurrentPosition(succ_ind[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;
              }
            }
          }
          AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID()); 
          break;
        }
      case 5:
        {
          vector<Node*> P;
          vector<Node*> S;

          bool is_load_add = n->is_Load_Address_Generator(); 
          bool is_load_data = n->is_Load_Data_Bus_Read();  

          tie(P,S) = get_Mapped_pred_succ(n);
          vector<MappingPair*> pred_ind = P[0]->GetMappedIndices();

          vector<MappingPair*> succ_ind = S[0]->GetMappedIndices();
          MappingPair* succ_pos = S[0]->GetCurrentPosition();
          MappingPair* pred_pos = P[0]->GetCurrentPosition();

          if(!is_load_add && !is_load_data)
          {

            if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos->getPE()) && !connectedPEs(pred_pos->getPE(), mapPairs[ind[i]]->getPE()))
            {
              //cout << "case 5 first if" << endl;
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              for(int j=0; j < (int) pred_ind.size(); j++)
              {
                if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                { 
                  //cout << "connected" << endl;
                  AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  P[0]->SetCurrentPosition(pred_ind[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  //TEC.print_matrix(); 
                  //_FATAL("debug"); 
                  return true;

                }
              }
              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            }
            else if(!connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos->getPE()) && connectedPEs(pred_pos->getPE(), mapPairs[ind[i]]->getPE()))
            {    
              //cout << "case 5 else if" << endl; 
              UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

              for(int j=0; j < (int) succ_ind.size(); j++)
              {
                if(!isTECfree(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[j]->getPE()))
                {
                  //cout << "connected" << endl;
                  AssignTEC(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT(), succ_ind[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  S[0]->SetCurrentPosition(succ_ind[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  //TEC.print_matrix(); 
                  //_FATAL("debug");
                  return true;

                }
              }
              AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            }
            else if(!connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos->getPE()) && !connectedPEs(pred_pos->getPE(), mapPairs[ind[i]]->getPE()))
            {
              // cout << "case 5: last else" << endl;
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
              for(int j=0; j < (int) pred_ind.size(); j++)
              {
                if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                  continue;
                for(int k=0; k < (int) succ_ind.size(); k++)
                {
                  if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                  {
                    //cout << "connected" << endl;
                    AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                    AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    S[0]->SetCurrentPosition(succ_ind[k]);;
                    P[0]->SetCurrentPosition(pred_ind[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    //TEC.print_matrix(); 
                    //_FATAL("debug"); 
                    return true;
                  }
                }
              }
              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            }
            else
            {
              MappingPair* mappair = mapPairs[ind[i]];
              mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
              AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
              return true;
              // _FATAL("should not come here! case 9 shallow node cp1: %d, cp2:%d, cs1: %d\n", connected_p1, connected_p2, connected_s1);
            }
          }

          if(is_load_add)
          {
            bool connected_p = connectedPEs(pred_pos->getPE(), mapPairs[ind[i]]->getPE()); 
            bool connected_s = (succ_pos->getPE()->getIndexI() == mapPairs[ind[i]]->getPE()->getIndexI()); 
        
            if(connected_s && !connected_p)
            {
              //cout << "case 5 first if" << endl;
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              for(int j=0; j < (int) pred_ind.size(); j++)
              {
                if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                { 
                  //cout << "connected" << endl;
                  AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  P[0]->SetCurrentPosition(pred_ind[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  //TEC.print_matrix(); 
                  //_FATAL("debug"); 
                  return true;

                }
              }
              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            }
            else if(!connected_s && connected_p)
            {    
              //cout << "case 5 else if" << endl; 
              UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

              for(int j=0; j < (int) succ_ind.size(); j++)
              {
                if(!isTECfree(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT()))
                  continue;

                if(mapPairs[ind[i]]->getPE()->getIndexI() == succ_ind[j]->getPE()->getIndexI())
                {
                  //cout << "connected" << endl;
                  AssignTEC(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT(), succ_ind[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  S[0]->SetCurrentPosition(succ_ind[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  //TEC.print_matrix(); 
                  //_FATAL("debug");
                  return true;

                }
              }
              AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            }
            else if(!connected_p && !connected_s)
            {
              // cout << "case 5: last else" << endl;
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
              for(int j=0; j < (int) pred_ind.size(); j++)
              {
                if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                  continue;
                for(int k=0; k < (int) succ_ind.size(); k++)
                {
                  if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                    continue;

                  if((mapPairs[ind[i]]->getPE()->getIndexI() == succ_ind[k]->getPE()->getIndexI()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                  {
                    //cout << "connected" << endl;
                    AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                    AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    S[0]->SetCurrentPosition(succ_ind[k]);;
                    P[0]->SetCurrentPosition(pred_ind[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    //TEC.print_matrix(); 
                    //_FATAL("debug"); 
                    return true;
                  }
                }
              }
              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            }
            else
            {
              MappingPair* mappair = mapPairs[ind[i]];
              mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
              AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
              return true;
              // _FATAL("should not come here! case 9 shallow node cp1: %d, cp2:%d, cs1: %d\n", connected_p1, connected_p2, connected_s1);
            }
          }
          else if(is_load_data)
          {
            bool connected_p = (pred_pos->getPE()->getIndexI() == mapPairs[ind[i]]->getPE()->getIndexI()); 
            bool connected_s = connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos->getPE()); 
            if(connected_s && !connected_p)
            {
              //cout << "case 5 first if" << endl;
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              for(int j=0; j < (int) pred_ind.size(); j++)
              {
                if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                  continue;

                if(pred_ind[j]->getPE()->getIndexI() == mapPairs[ind[i]]->getPE()->getIndexI())
                { 
                  //cout << "connected" << endl;
                  AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  P[0]->SetCurrentPosition(pred_ind[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  //TEC.print_matrix(); 
                  //_FATAL("debug"); 
                  return true;

                }
              }
              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            }
            else if(!connected_s && connected_p)
            {    
              //cout << "case 5 else if" << endl; 
              UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

              for(int j=0; j < (int) succ_ind.size(); j++)
              {
                if(!isTECfree(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[j]->getPE()))
                {
                  //cout << "connected" << endl;
                  AssignTEC(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT(), succ_ind[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  S[0]->SetCurrentPosition(succ_ind[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  //TEC.print_matrix(); 
                  //_FATAL("debug");
                  return true;

                }
              }
              AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            }
            else if(!connected_s && !connected_p)
            {
              // cout << "case 5: last else" << endl;
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
              for(int j=0; j < (int) pred_ind.size(); j++)
              {
                if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                  continue;
                for(int k=0; k < (int) succ_ind.size(); k++)
                {
                  if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && (pred_ind[j]->getPE()->getIndexI() == mapPairs[ind[i]]->getPE()->getIndexI()))
                  {
                    //cout << "connected" << endl;
                    AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                    AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    S[0]->SetCurrentPosition(succ_ind[k]);;
                    P[0]->SetCurrentPosition(pred_ind[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    //TEC.print_matrix(); 
                    //_FATAL("debug"); 
                    return true;
                  }
                }
              }
              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            }
            else
            {
              MappingPair* mappair = mapPairs[ind[i]];
              mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
              AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
              return true;
              // _FATAL("should not come here! case 9 shallow node cp1: %d, cp2:%d, cs1: %d\n", connected_p1, connected_p2, connected_s1);
            }
          }
          break;
        }
      case 6:
        {
          vector<Node*> P;
          vector<Node*> S;

          tie(P,S) = get_Mapped_pred_succ(n);

          vector<MappingPair*> pred_ind = P[0]->GetMappedIndices();
          vector<MappingPair*> pred_ind1 = P[1]->GetMappedIndices();
          vector<MappingPair*> succ_ind = S[0]->GetMappedIndices();
          MappingPair* succ_pos = S[0]->GetCurrentPosition();
          MappingPair* pred_pos1 = P[1]->GetCurrentPosition();
          MappingPair* pred_pos = P[0]->GetCurrentPosition();

          bool connected_p1 = false, connected_p2 = false, connected_s1 = false; 

          if(connectedPEs(pred_pos->getPE(), mapPairs[ind[i]]->getPE()))
            connected_p1 = true;

          // cout << "passed connected_p" << endl;
          if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos->getPE()))
            connected_s1 = true; 

          // cout << "passed connected s1" << endl;
          if(connectedPEs(pred_pos1->getPE(), mapPairs[ind[i]]->getPE()))
            connected_p2 = true;

          //  cout << "pased connected s2" << endl;


          if(connected_p1 && connected_p2 && !connected_s1)
          {
            //cout << "inside 1" << endl;
            UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

            for(int j=0; j < (int) succ_ind.size(); j++)
            {
              if(!isTECfree(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[j]->getPE()))
              {
                AssignTEC(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT(), succ_ind[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                S[0]->SetCurrentPosition(succ_ind[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;

              }
            }
            AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

          }
          else if(connected_p1 && !connected_p2 && connected_s1)
          {
            //cout << "inside 2" << endl;
            UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());

            for(int j=0; j < (int) pred_ind1.size(); j++)
            {
              if(!isTECfree(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(pred_ind1[j]->getPE(), mapPairs[ind[i]]->getPE()))
              {
                AssignTEC(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT(), pred_ind1[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                P[1]->SetCurrentPosition(pred_ind1[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;
              }
            }
            AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
          }
          else if(!connected_p1 && connected_p2 && connected_s1)
          {
            //cout << "inside 3" << endl;
            UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            for(int j=0; j < (int) pred_ind.size(); j++)
            {
              if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
              {
                AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                P[0]->SetCurrentPosition(pred_ind[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;
              }
            }

            AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
          }
          else if(connected_p1 && !connected_p2 && !connected_s1)
          {
            //cout << "inside 4" << endl;
            UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
            UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            for(int j=0; j < (int) pred_ind1.size(); j++)
            {
              if(!isTECfree(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT()))
                continue;
              for(int k=0; k < (int) succ_ind.size(); k++)
              {
                if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(pred_ind1[j]->getPE(), mapPairs[ind[i]]->getPE()))
                {
                  AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                  AssignTEC(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT(), pred_ind1[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  S[0]->SetCurrentPosition(succ_ind[k]);
                  P[1]->SetCurrentPosition(pred_ind1[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }
            }
            AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
            AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

          }
          else if(!connected_p1 && connected_p2 && !connected_s1)
          {
            //cout << "inside 5" << endl;
            UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            for(int j=0; j < (int) pred_ind.size(); j++)
            {
              if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                continue;
              for(int k=0; k < (int) succ_ind.size(); k++)
              {
                if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                {
                  AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                  AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  S[0]->SetCurrentPosition(succ_ind[k]);
                  P[0]->SetCurrentPosition(pred_ind[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }
            }
            AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

          }
          else if(!connected_p1 && !connected_p2 && connected_s1)
          {
            //cout << "inside 6" << endl;
            UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
            for(int j=0; j < (int) pred_ind.size(); j++)
            {
              if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                continue;
              for(int k=0; k < (int) pred_ind1.size(); k++)
              {
                if(!isTECfree(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(pred_ind1[k]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                {
                  AssignTEC(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT(), pred_ind1[k]->getNode()->get_ID());
                  AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  P[1]->SetCurrentPosition(pred_ind1[k]);
                  P[0]->SetCurrentPosition(pred_ind[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }
            }
            AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
          }
          else if(!connected_p1 && !connected_p2 && !connected_s1)
          {
            //cout << "inside 7" << endl;
            UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
            UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            for(int j=0; j < (int) pred_ind.size(); j++)
            {
              if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                continue;
              for(int k=0; k < (int) pred_ind1.size(); k++)
              {
                if(!isTECfree(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT()))
                  continue;
                for(int l=0; l < (int) succ_ind.size(); l++)
                {
                  if(!isTECfree(succ_ind[l]->getPE()->getIndexI(), succ_ind[l]->getPE()->getIndexJ(), succ_ind[l]->getPE()->getIndexT()))
                    continue;
                  if(connectedPEs(pred_ind1[k]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[l]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                  {
                    AssignTEC(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT(), pred_ind1[k]->getNode()->get_ID());
                    AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                    AssignTEC(succ_ind[l]->getPE()->getIndexI(), succ_ind[l]->getPE()->getIndexJ(), succ_ind[l]->getPE()->getIndexT(), succ_ind[l]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    P[1]->SetCurrentPosition(pred_ind1[k]);
                    S[0]->SetCurrentPosition(succ_ind[l]);
                    P[0]->SetCurrentPosition(pred_ind[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                  }

                }

              }
            }
            AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
            AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
          }
          else
          {
            MappingPair* mappair = mapPairs[ind[i]];
            mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
            AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
            return true;
            // _FATAL("should not come here! case 9 shallow node cp1: %d, cp2:%d, cs1: %d\n", connected_p1, connected_p2, connected_s1);
          } 
          //_FATAL("In Node recovery shallow map_case: %d\n", map_case);
          break;
        }
      case 7:
        {
          //_FATAL("In Node recovery shallow map_case: %d\n", map_case);
          vector<Node*> P;
          vector<Node*> S;

          tie(P,S) = get_Mapped_pred_succ(n);
          vector<MappingPair*> pred_ind = P[0]->GetMappedIndices();
          vector<MappingPair*> pred_ind1 = P[1]->GetMappedIndices();
          vector<MappingPair*> pred_ind2 = P[2]->GetMappedIndices();
          vector<MappingPair*> succ_ind = S[0]->GetMappedIndices();
          MappingPair* succ_pos = S[0]->GetCurrentPosition();
          MappingPair* pred_pos2 = P[2]->GetCurrentPosition();
          MappingPair* pred_pos1 = P[1]->GetCurrentPosition();
          MappingPair* pred_pos = P[0]->GetCurrentPosition();

          bool connected_p = false, connected_s1 = false, connected_p2 = false, connected_p1 = false; 

          if(connectedPEs(pred_pos->getPE(), mapPairs[ind[i]]->getPE()))
            connected_p = true;
          if(connectedPEs(pred_pos1->getPE(), mapPairs[ind[i]]->getPE()))
            connected_p1 = true;
          if(connectedPEs(pred_pos2->getPE(), mapPairs[ind[i]]->getPE()))
            connected_p2 = true; 
          if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos->getPE()))
            connected_s1 = true;

          if(connected_s1)
          {

            if(connected_p && connected_p1 && !connected_p2)
            {
              //cout << "inside 1" << endl;
              UnAssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());

              for(int j=0; j < (int) pred_ind2.size(); j++)
              {
                if(!isTECfree(pred_ind2[j]->getPE()->getIndexI(), pred_ind2[j]->getPE()->getIndexJ(), pred_ind2[j]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(pred_ind2[j]->getPE(), mapPairs[ind[i]]->getPE()))
                {
                  AssignTEC(pred_ind2[j]->getPE()->getIndexI(), pred_ind2[j]->getPE()->getIndexJ(), pred_ind2[j]->getPE()->getIndexT(), pred_ind2[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  P[2]->SetCurrentPosition(pred_ind2[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;

                }
              }
              AssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());

            }
            else if(connected_p && !connected_p1 && connected_p2)
            {
              //cout << "inside 2" << endl;
              UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());

              for(int j=0; j < (int) pred_ind1.size(); j++)
              {
                if(!isTECfree(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(pred_ind1[j]->getPE(), mapPairs[ind[i]]->getPE()))
                {
                  AssignTEC(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT(), pred_ind1[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  P[1]->SetCurrentPosition(pred_ind1[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }
              AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
            }
            else if(!connected_p && connected_p1 && connected_p2)
            {
              //cout << "inside 3" << endl;
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              for(int j=0; j < (int) pred_ind.size(); j++)
              {
                if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                {
                  AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  P[0]->SetCurrentPosition(pred_ind[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }

              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            }
            else if(connected_p && !connected_p1 && !connected_p2)
            {
              //cout << "inside 4" << endl;
              UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
              UnAssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());
              for(int j=0; j < (int) pred_ind2.size(); j++)
              {
                if(!isTECfree(pred_ind2[j]->getPE()->getIndexI(), pred_ind2[j]->getPE()->getIndexJ(), pred_ind2[j]->getPE()->getIndexT()))
                  continue;
                for(int k=0; k < (int) pred_ind1.size(); k++)
                {
                  if(!isTECfree(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(pred_ind1[k]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind2[j]->getPE(), mapPairs[ind[i]]->getPE()))
                  {
                    AssignTEC(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT(), pred_ind1[k]->getNode()->get_ID());
                    AssignTEC(pred_ind2[j]->getPE()->getIndexI(), pred_ind2[j]->getPE()->getIndexJ(), pred_ind2[j]->getPE()->getIndexT(), pred_ind2[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    P[1]->SetCurrentPosition(pred_ind1[k]);
                    P[2]->SetCurrentPosition(pred_ind2[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                  }
                }
              }
              AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
              AssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());

            }
            else if(!connected_p && connected_p1 && !connected_p2)
            {
              //cout << "inside 5" << endl;
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              UnAssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());
              for(int j=0; j < (int) pred_ind.size(); j++)
              {
                if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                  continue;
                for(int k=0; k < (int) pred_ind2.size(); k++)
                {
                  if(!isTECfree(pred_ind2[k]->getPE()->getIndexI(), pred_ind2[k]->getPE()->getIndexJ(), pred_ind2[k]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(pred_ind2[k]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                  {
                    AssignTEC(pred_ind2[k]->getPE()->getIndexI(), pred_ind2[k]->getPE()->getIndexJ(), pred_ind2[k]->getPE()->getIndexT(), pred_ind2[k]->getNode()->get_ID());
                    AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    P[2]->SetCurrentPosition(pred_ind2[k]);
                    P[0]->SetCurrentPosition(pred_ind[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                  }
                }
              }
              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              AssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());

            }
            else if(!connected_p && !connected_p1 && connected_p2)
            {
              //cout << "inside 6" << endl;
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
              for(int j=0; j < (int) pred_ind.size(); j++)
              {
                if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                  continue;
                for(int k=0; k < (int) pred_ind1.size(); k++)
                {
                  if(!isTECfree(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(pred_ind1[k]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                  {
                    AssignTEC(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT(), pred_ind1[k]->getNode()->get_ID());
                    AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    P[1]->SetCurrentPosition(pred_ind1[k]);
                    P[0]->SetCurrentPosition(pred_ind[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                  }
                }
              }
              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
            }
            else if(!connected_p && !connected_p1 && !connected_p2)
            {
              //cout << "inside 7" << endl;
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
              UnAssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());
              for(int j=0; j < (int) pred_ind.size(); j++)
              {
                if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                  continue;
                for(int k=0; k < (int) pred_ind1.size(); k++)
                {
                  if(!isTECfree(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT()))
                    continue;
                  for(int l=0; l < (int) pred_ind2.size(); l++)
                  {
                    if(!isTECfree(pred_ind2[l]->getPE()->getIndexI(), pred_ind2[l]->getPE()->getIndexJ(), pred_ind2[l]->getPE()->getIndexT()))
                      continue;
                    if(connectedPEs(pred_ind1[k]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind2[l]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                    {
                      AssignTEC(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT(), pred_ind1[k]->getNode()->get_ID());
                      AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                      AssignTEC(pred_ind2[l]->getPE()->getIndexI(), pred_ind2[l]->getPE()->getIndexJ(), pred_ind2[l]->getPE()->getIndexT(), pred_ind2[l]->getNode()->get_ID());
                      MappingPair* mappair = mapPairs[ind[i]];
                      mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                      P[1]->SetCurrentPosition(pred_ind1[k]);
                      P[2]->SetCurrentPosition(pred_ind2[l]);
                      P[0]->SetCurrentPosition(pred_ind[j]);
                      AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                      return true;
                    }

                  }

                }
              }
              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
              AssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());
            }
            else
            {
              MappingPair* mappair = mapPairs[ind[i]];
              mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);        
              AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
              return true;
              _FATAL("should not come here! case 9 shallow node\n"); 
              //_FATAL("In Node recovery shallow map_case: %d\n", map_case);
            }


          }
          else
          {
            UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

            for(int jj=0; jj < (int) succ_ind.size(); jj++)
            {
              if(!isTECfree(succ_ind[jj]->getPE()->getIndexI(), succ_ind[jj]->getPE()->getIndexJ(), succ_ind[jj]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[jj]->getPE()))
              { 

                if(connected_p && connected_p1 && !connected_p2)
                {
                  //cout << "inside 1" << endl;
                  UnAssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());

                  for(int j=0; j < (int) pred_ind2.size(); j++)
                  {
                    if(!isTECfree(pred_ind2[j]->getPE()->getIndexI(), pred_ind2[j]->getPE()->getIndexJ(), pred_ind2[j]->getPE()->getIndexT()))
                      continue;

                    if(connectedPEs(pred_ind2[j]->getPE(), mapPairs[ind[i]]->getPE()))
                    {
                      AssignTEC(pred_ind2[j]->getPE()->getIndexI(), pred_ind2[j]->getPE()->getIndexJ(), pred_ind2[j]->getPE()->getIndexT(), pred_ind2[j]->getNode()->get_ID());
                      AssignTEC(succ_ind[jj]->getPE()->getIndexI(), succ_ind[jj]->getPE()->getIndexJ(), succ_ind[jj]->getPE()->getIndexT(), succ_ind[jj]->getNode()->get_ID());
                      S[0]->SetCurrentPosition(succ_ind[jj]);
                      MappingPair* mappair = mapPairs[ind[i]];
                      mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                      P[2]->SetCurrentPosition(pred_ind2[j]);
                      AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                      return true;

                    }
                  }
                  AssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());

                }
                else if(connected_p && !connected_p1 && connected_p2)
                {
                  //cout << "inside 2" << endl;
                  UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());

                  for(int j=0; j < (int) pred_ind1.size(); j++)
                  {
                    if(!isTECfree(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT()))
                      continue;

                    if(connectedPEs(pred_ind1[j]->getPE(), mapPairs[ind[i]]->getPE()))
                    {
                      AssignTEC(pred_ind1[j]->getPE()->getIndexI(), pred_ind1[j]->getPE()->getIndexJ(), pred_ind1[j]->getPE()->getIndexT(), pred_ind1[j]->getNode()->get_ID());
                      AssignTEC(succ_ind[jj]->getPE()->getIndexI(), succ_ind[jj]->getPE()->getIndexJ(), succ_ind[jj]->getPE()->getIndexT(), succ_ind[jj]->getNode()->get_ID());
                      S[0]->SetCurrentPosition(succ_ind[jj]);
                      MappingPair* mappair = mapPairs[ind[i]];
                      mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                      P[1]->SetCurrentPosition(pred_ind1[j]);
                      AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                      return true;
                    }
                  }
                  AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
                }
                else if(!connected_p && connected_p1 && connected_p2)
                {
                  //cout << "inside 3" << endl;
                  UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
                  for(int j=0; j < (int) pred_ind.size(); j++)
                  {
                    if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                      continue;

                    if(connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                    {
                      AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                      AssignTEC(succ_ind[jj]->getPE()->getIndexI(), succ_ind[jj]->getPE()->getIndexJ(), succ_ind[jj]->getPE()->getIndexT(), succ_ind[jj]->getNode()->get_ID());
                      S[0]->SetCurrentPosition(succ_ind[jj]);
                      MappingPair* mappair = mapPairs[ind[i]];
                      mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                      P[0]->SetCurrentPosition(pred_ind[j]);
                      AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                      return true;
                    }
                  }

                  AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
                }
                else if(connected_p && !connected_p1 && !connected_p2)
                {
                  //cout << "inside 4" << endl;
                  UnAssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());
                  UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
                  for(int j=0; j < (int) pred_ind2.size(); j++)
                  {
                    if(!isTECfree(pred_ind2[j]->getPE()->getIndexI(), pred_ind2[j]->getPE()->getIndexJ(), pred_ind2[j]->getPE()->getIndexT()))
                      continue;
                    for(int k=0; k < (int) pred_ind1.size(); k++)
                    {
                      if(!isTECfree(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT()))
                        continue;

                      if(connectedPEs(pred_ind1[k]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind2[j]->getPE(), mapPairs[ind[i]]->getPE()))
                      {
                        AssignTEC(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT(), pred_ind1[k]->getNode()->get_ID());
                        AssignTEC(pred_ind2[j]->getPE()->getIndexI(), pred_ind2[j]->getPE()->getIndexJ(), pred_ind2[j]->getPE()->getIndexT(), pred_ind2[j]->getNode()->get_ID());
                        AssignTEC(succ_ind[jj]->getPE()->getIndexI(), succ_ind[jj]->getPE()->getIndexJ(), succ_ind[jj]->getPE()->getIndexT(), succ_ind[jj]->getNode()->get_ID());
                        S[0]->SetCurrentPosition(succ_ind[jj]);
                        MappingPair* mappair = mapPairs[ind[i]];
                        mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                        P[1]->SetCurrentPosition(pred_ind1[k]);
                        P[2]->SetCurrentPosition(pred_ind2[j]);
                        AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                        return true;
                      }
                    }
                  }
                  AssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());
                  AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());

                }
                else if(!connected_p && connected_p1 && !connected_p2)
                {
                  //cout << "inside 5" << endl;
                  UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
                  UnAssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());
                  for(int j=0; j < (int) pred_ind.size(); j++)
                  {
                    if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                      continue;
                    for(int k=0; k < (int) pred_ind2.size(); k++)
                    {
                      if(!isTECfree(pred_ind2[k]->getPE()->getIndexI(), pred_ind2[k]->getPE()->getIndexJ(), pred_ind2[k]->getPE()->getIndexT()))
                        continue;

                      if(connectedPEs(pred_ind2[k]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                      {
                        AssignTEC(pred_ind2[k]->getPE()->getIndexI(), pred_ind2[k]->getPE()->getIndexJ(), pred_ind2[k]->getPE()->getIndexT(), pred_ind2[k]->getNode()->get_ID());
                        AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                        AssignTEC(succ_ind[jj]->getPE()->getIndexI(), succ_ind[jj]->getPE()->getIndexJ(), succ_ind[jj]->getPE()->getIndexT(), succ_ind[jj]->getNode()->get_ID());
                        S[0]->SetCurrentPosition(succ_ind[jj]);
                        MappingPair* mappair = mapPairs[ind[i]];
                        mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                        P[2]->SetCurrentPosition(pred_ind2[k]);
                        P[0]->SetCurrentPosition(pred_ind[j]);
                        AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                        return true;
                      }
                    }
                  }
                  AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
                  AssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());

                }
                else if(!connected_p && !connected_p1 && connected_p2)
                {
                  //cout << "inside 6" << endl;
                  UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
                  UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
                  for(int j=0; j < (int) pred_ind.size(); j++)
                  {
                    if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                      continue;
                    for(int k=0; k < (int) pred_ind1.size(); k++)
                    {
                      if(!isTECfree(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT()))
                        continue;

                      if(connectedPEs(pred_ind1[k]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                      {
                        AssignTEC(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT(), pred_ind1[k]->getNode()->get_ID());
                        AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                        AssignTEC(succ_ind[jj]->getPE()->getIndexI(), succ_ind[jj]->getPE()->getIndexJ(), succ_ind[jj]->getPE()->getIndexT(), succ_ind[jj]->getNode()->get_ID());
                        S[0]->SetCurrentPosition(succ_ind[jj]);
                        MappingPair* mappair = mapPairs[ind[i]];
                        mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                        P[1]->SetCurrentPosition(pred_ind1[k]);
                        P[0]->SetCurrentPosition(pred_ind[j]);
                        AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                        return true;
                      }
                    }
                  }
                  AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
                  AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
                }
                else if(!connected_p && !connected_p1 && !connected_p2)
                {
                  //cout << "inside 7" << endl;
                  UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
                  UnAssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
                  UnAssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());
                  for(int j=0; j < (int) pred_ind.size(); j++)
                  {
                    if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                      continue;
                    for(int k=0; k < (int) pred_ind1.size(); k++)
                    {
                      if(!isTECfree(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT()))
                        continue;
                      for(int l=0; l < (int) pred_ind2.size(); l++)
                      {
                        if(!isTECfree(pred_ind2[l]->getPE()->getIndexI(), pred_ind2[l]->getPE()->getIndexJ(), pred_ind2[l]->getPE()->getIndexT()))
                          continue;
                        if(connectedPEs(pred_ind1[k]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind2[l]->getPE(), mapPairs[ind[i]]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                        {
                          AssignTEC(pred_ind1[k]->getPE()->getIndexI(), pred_ind1[k]->getPE()->getIndexJ(), pred_ind1[k]->getPE()->getIndexT(), pred_ind1[k]->getNode()->get_ID());
                          AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                          AssignTEC(pred_ind2[l]->getPE()->getIndexI(), pred_ind2[l]->getPE()->getIndexJ(), pred_ind2[l]->getPE()->getIndexT(), pred_ind2[l]->getNode()->get_ID());
                          AssignTEC(succ_ind[jj]->getPE()->getIndexI(), succ_ind[jj]->getPE()->getIndexJ(), succ_ind[jj]->getPE()->getIndexT(), succ_ind[jj]->getNode()->get_ID());
                          S[0]->SetCurrentPosition(succ_ind[jj]);
                          MappingPair* mappair = mapPairs[ind[i]];
                          mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                          P[1]->SetCurrentPosition(pred_ind1[k]);
                          P[2]->SetCurrentPosition(pred_ind2[l]);
                          P[0]->SetCurrentPosition(pred_ind[j]);
                          AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                          return true;
                        }

                      }

                    }
                  }
                  AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
                  AssignTEC(pred_pos1->getPE()->getIndexI(), pred_pos1->getPE()->getIndexJ(), pred_pos1->getPE()->getIndexT(), pred_pos1->getNode()->get_ID());
                  AssignTEC(pred_pos2->getPE()->getIndexI(), pred_pos2->getPE()->getIndexJ(), pred_pos2->getPE()->getIndexT(), pred_pos2->getNode()->get_ID());
                }
                else
                {
                  AssignTEC(succ_ind[jj]->getPE()->getIndexI(), succ_ind[jj]->getPE()->getIndexJ(), succ_ind[jj]->getPE()->getIndexT(), succ_ind[jj]->getNode()->get_ID());
                  S[0]->SetCurrentPosition(succ_ind[jj]);
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);        
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                  _FATAL("should not come here! case 9 shallow node\n"); 
                  //_FATAL("In Node recovery shallow map_case: %d\n", map_case);
                }
              }
            }
            AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
          }

          break;
        }
      case 8:
        {
          //cout << "inside case 8" << endl;
          vector<Node*> P;
          vector<Node*> S;

          tie(P,S) = get_Mapped_pred_succ(n);

          // cout << "P size: " << (int) P.size() << endl;
          //cout << "S.size: " << (int) S.size() << endl;



          vector<MappingPair*> succ_ind1 = S[1]->GetMappedIndices();
          vector<MappingPair*> succ_ind = S[0]->GetMappedIndices();
          MappingPair* succ_pos = S[0]->GetCurrentPosition();
          MappingPair* succ_pos1 = S[1]->GetCurrentPosition();

          if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos->getPE()) && !connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos1->getPE()))
          {
            //cout << "inside if" << endl;
            UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
            for(int j=0; j < (int) succ_ind1.size(); j++)
            {
              if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
              { 
                AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                S[1]->SetCurrentPosition(succ_ind1[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;

              }
            }
            AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
          }
          else if(!connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos1->getPE()))
          {
            //cout << "inside else if" << endl;     
            UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

            for(int j=0; j < (int) succ_ind.size(); j++)
            {
              if(!isTECfree(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[j]->getPE()))
              {
                AssignTEC(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT(), succ_ind[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                S[0]->SetCurrentPosition(succ_ind[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;
              }
            }
            AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
          }
          else if(!connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos->getPE()) && !connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos1->getPE()))
          {
            //cout << "inside else" << endl;
            UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
            UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            for(int j=0; j < (int) succ_ind1.size(); j++)
            {
              if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                continue;
              for(int k=0; k < (int) succ_ind.size(); k++)
              {
                if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
                {
                  AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                  AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  S[0]->SetCurrentPosition(succ_ind[k]);
                  S[1]->SetCurrentPosition(succ_ind1[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }
            }
            AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
            AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
          }
          else
          {
            MappingPair* mappair = mapPairs[ind[i]];
            mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
            AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
            return true;
            // _FATAL("should not come here! case 9 shallow node cp1: %d, cp2:%d, cs1: %d\n", connected_p1, connected_p2, connected_s1);
          }
          break;
        }
      case 9:
        { 
          vector<Node*> P;
          vector<Node*> S;

          tie(P,S) = get_Mapped_pred_succ(n);
          vector<MappingPair*> pred_ind = P[0]->GetMappedIndices();
          vector<MappingPair*> succ_ind1 = S[1]->GetMappedIndices();
          vector<MappingPair*> succ_ind = S[0]->GetMappedIndices();
          MappingPair* succ_pos = S[0]->GetCurrentPosition();
          MappingPair* succ_pos1 = S[1]->GetCurrentPosition();
          MappingPair* pred_pos = P[0]->GetCurrentPosition();

          bool is_load_add = P[0]->is_Load_Address_Generator();

          bool connected_p = false, connected_s1 = false, connected_s2 = false; 

          if(!is_load_add)
          {
            if(connectedPEs(pred_pos->getPE(), mapPairs[ind[i]]->getPE()))
              connected_p = true;
            if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos->getPE()))
              connected_s1 = true; 
            if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos1->getPE()))
              connected_s2 = true;

            if(connected_p && connected_s1 && !connected_s2)
            {
              //cout << "inside 1" << endl;
              UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

              for(int j=0; j < (int) succ_ind1.size(); j++)
              {
                if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
                {
                  AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  S[1]->SetCurrentPosition(succ_ind1[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;

                }
              }
              AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

            }
            else if(connected_p && !connected_s1 && connected_s2)
            {
              //cout << "inside 2" << endl;
              UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

              for(int j=0; j < (int) succ_ind.size(); j++)
              {
                if(!isTECfree(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[j]->getPE()))
                {
                  AssignTEC(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT(), succ_ind[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  S[0]->SetCurrentPosition(succ_ind[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }
              AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            }
            else if(!connected_p && connected_s1 && connected_s2)
            {
              //cout << "inside 3" << endl;
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              for(int j=0; j < (int) pred_ind.size(); j++)
              {
                if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                {
                  AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  P[0]->SetCurrentPosition(pred_ind[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }

              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            }
            else if(connected_p && !connected_s1 && !connected_s2)
            {
              //cout << "inside 4" << endl;
              UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
              UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
              for(int j=0; j < (int) succ_ind1.size(); j++)
              {
                if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                  continue;
                for(int k=0; k < (int) succ_ind.size(); k++)
                {
                  if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
                  {
                    AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                    AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    S[0]->SetCurrentPosition(succ_ind[k]);
                    S[1]->SetCurrentPosition(succ_ind1[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                  }
                }
              }
              AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
              AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

            }
            else if(!connected_p && connected_s1 && !connected_s2)
            {
              //cout << "inside 5" << endl;
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
              for(int j=0; j < (int) pred_ind.size(); j++)
              {
                if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                  continue;
                for(int k=0; k < (int) succ_ind1.size(); k++)
                {
                  if(!isTECfree(succ_ind1[k]->getPE()->getIndexI(), succ_ind1[k]->getPE()->getIndexJ(), succ_ind1[k]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[k]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                  {
                    AssignTEC(succ_ind1[k]->getPE()->getIndexI(), succ_ind1[k]->getPE()->getIndexJ(), succ_ind1[k]->getPE()->getIndexT(), succ_ind1[k]->getNode()->get_ID());
                    AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    S[1]->SetCurrentPosition(succ_ind1[k]);
                    P[0]->SetCurrentPosition(pred_ind[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                  }
                }
              }
              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

            }
            else if(!connected_p && !connected_s1 && connected_s2)
            {
              //cout << "inside 6" << endl;
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
              for(int j=0; j < (int) pred_ind.size(); j++)
              {
                if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                  continue;
                for(int k=0; k < (int) succ_ind.size(); k++)
                {
                  if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                  {
                    AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                    AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    S[0]->SetCurrentPosition(succ_ind[k]);
                    P[0]->SetCurrentPosition(pred_ind[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                  }
                }
              }
              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            }
            else if(!connected_p && !connected_s1 && !connected_s2)
            {
              //cout << "inside 7" << endl;
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
              UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
              for(int j=0; j < (int) pred_ind.size(); j++)
              {
                if(!isTECfree(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT()))
                  continue;
                for(int k=0; k < (int) succ_ind.size(); k++)
                {
                  if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                    continue;
                  for(int l=0; l < (int) succ_ind1.size(); l++)
                  {
                    if(!isTECfree(succ_ind1[l]->getPE()->getIndexI(), succ_ind1[l]->getPE()->getIndexJ(), succ_ind1[l]->getPE()->getIndexT()))
                      continue;
                    if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[l]->getPE()) && connectedPEs(pred_ind[j]->getPE(), mapPairs[ind[i]]->getPE()))
                    {
                      AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                      AssignTEC(pred_ind[j]->getPE()->getIndexI(), pred_ind[j]->getPE()->getIndexJ(), pred_ind[j]->getPE()->getIndexT(), pred_ind[j]->getNode()->get_ID());
                      AssignTEC(succ_ind1[l]->getPE()->getIndexI(), succ_ind1[l]->getPE()->getIndexJ(), succ_ind1[l]->getPE()->getIndexT(), succ_ind1[l]->getNode()->get_ID());
                      MappingPair* mappair = mapPairs[ind[i]];
                      mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                      S[0]->SetCurrentPosition(succ_ind[k]);
                      S[1]->SetCurrentPosition(succ_ind1[l]);
                      P[0]->SetCurrentPosition(pred_ind[j]);
                      AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                      return true;
                    }

                  }

                }
              }
              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
              AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
            }
            else
            {
              MappingPair* mappair = mapPairs[ind[i]];
              mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);        
              AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
              return true;
              _FATAL("should not come here! case 9 shallow node\n"); 
              //_FATAL("In Node recovery shallow map_case: %d\n", map_case);
            }
          }
          else
          {
            // Instead of connected PEs for pred check for connected rows.
            if(pred_pos->getPE()->getIndexI() == mapPairs[ind[i]]->getPE()->getIndexI())
              connected_p = true;
            if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos->getPE()))
              connected_s1 = true;
            if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos1->getPE()))
              connected_s2 = true;  

            if(connected_p)
            {
              if(connected_s1 && !connected_s2)
              {
                UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

                for(int j=0; j < (int) succ_ind1.size(); j++)
                {
                  if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
                  {
                    AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    S[1]->SetCurrentPosition(succ_ind1[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;

                  }
                }
                AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
              }
              else if(!connected_s1 && connected_s2)
              {
                UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

                for(int j=0; j < (int) succ_ind.size(); j++)
                {
                  if(!isTECfree(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[j]->getPE()))
                  {
                    AssignTEC(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT(), succ_ind[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    S[0]->SetCurrentPosition(succ_ind[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                  }
                }
                AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

              }
              else if(!connected_s1 && !connected_s2)
              {
                UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                for(int j=0; j < (int) succ_ind1.size(); j++)
                {
                  if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                    continue;
                  for(int k=0; k < (int) succ_ind.size(); k++)
                  {
                    if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                      continue;

                    if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
                    {
                      AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                      AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                      MappingPair* mappair = mapPairs[ind[i]];
                      mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                      S[0]->SetCurrentPosition(succ_ind[k]);
                      S[1]->SetCurrentPosition(succ_ind1[j]);
                      AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                      return true;
                    }
                  }
                }
                AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

              }
              else
              {
                _FATAL("We should not be here case 9 shallow/non load p=%d, s1=%d, s2=%d\n", connected_p, connected_s1, connected_s2); 
              }
            }
            else
            {
              for(int jj=0; jj < (int) pred_ind.size(); jj++)
              {
                UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
                if(connected_s1 && !connected_s2)
                {
                  UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

                  for(int j=0; j < (int) succ_ind1.size(); j++)
                  {
                    if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                      continue;

                    if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()) && pred_ind[jj]->getPE()->getIndexI() == mapPairs[ind[i]]->getPE()->getIndexI())
                    {
                      AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                      MappingPair* mappair = mapPairs[ind[i]];
                      mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                      S[1]->SetCurrentPosition(succ_ind1[j]);
                      AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                      P[0]->SetCurrentPosition(pred_ind[jj]);
                      AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                      return true;

                    }
                  }
                  AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                }
                else if(!connected_s1 && connected_s2)
                {
                  UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

                  for(int j=0; j < (int) succ_ind.size(); j++)
                  {
                    if(!isTECfree(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT()))
                      continue;

                    if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[j]->getPE()) && pred_ind[jj]->getPE()->getIndexI() == mapPairs[ind[i]]->getPE()->getIndexI())
                    {
                      AssignTEC(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT(), succ_ind[j]->getNode()->get_ID());
                      MappingPair* mappair = mapPairs[ind[i]];
                      mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                      S[0]->SetCurrentPosition(succ_ind[j]);
                      AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                      P[0]->SetCurrentPosition(pred_ind[jj]);
                      AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                      return true;
                    }
                  }
                  AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

                }
                else if(!connected_s1 && !connected_s2)
                {
                  UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                  UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                  for(int j=0; j < (int) succ_ind1.size(); j++)
                  {
                    if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                      continue;
                    for(int k=0; k < (int) succ_ind.size(); k++)
                    {
                      if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                        continue;

                      if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()) && pred_ind[jj]->getPE()->getIndexI() == mapPairs[ind[i]]->getPE()->getIndexI())
                      {
                        AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                        AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                        MappingPair* mappair = mapPairs[ind[i]];
                        mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                        S[0]->SetCurrentPosition(succ_ind[k]);
                        S[1]->SetCurrentPosition(succ_ind1[j]);
                        AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                        P[0]->SetCurrentPosition(pred_ind[jj]);
                        AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                        return true;
                      }
                    }
                  }
                  AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                  AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

                }
                else
                {
                  if(pred_ind[jj]->getPE()->getIndexI() == mapPairs[ind[i]]->getPE()->getIndexI())
                  {
                    AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                    P[0]->SetCurrentPosition(pred_ind[jj]);
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                  }  
                }   
                AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID()); 
              }
            }
          }         
          break;
        }
      case 10:
        {
          _FATAL("In Node recovery shallow map_case: %d\n", map_case);
          break;
        }
      case 11:
        {
          _FATAL("In Node recovery shallow map_case: %d\n", map_case);
          break;
        }
      case 12:
        {
          vector<Node*> P;
          vector<Node*> S;

          tie(P,S) = get_Mapped_pred_succ(n);
          vector<MappingPair*> succ_ind2 = S[2]->GetMappedIndices();
          vector<MappingPair*> succ_ind1 = S[1]->GetMappedIndices();
          vector<MappingPair*> succ_ind = S[0]->GetMappedIndices();
          MappingPair* succ_pos = S[0]->GetCurrentPosition();
          MappingPair* succ_pos1 = S[1]->GetCurrentPosition();
          MappingPair* succ_pos2 = S[2]->GetCurrentPosition();

          bool connected_s = false, connected_s1 = false, connected_s2 = false; 

          if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos2->getPE()))
            connected_s = true;
          if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos->getPE()))
            connected_s1 = true; 
          if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos1->getPE()))
            connected_s2 = true;

          if(connected_s && connected_s1 && !connected_s2)
          {
            //cout << "inside 1" << endl;
            UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

            for(int j=0; j < (int) succ_ind1.size(); j++)
            {
              if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
              {
                AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                S[1]->SetCurrentPosition(succ_ind1[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;

              }
            }
            AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

          }
          else if(connected_s && !connected_s1 && connected_s2)
          {
            //cout << "inside 2" << endl;
            UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

            for(int j=0; j < (int) succ_ind.size(); j++)
            {
              if(!isTECfree(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[j]->getPE()))
              {
                AssignTEC(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT(), succ_ind[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                S[0]->SetCurrentPosition(succ_ind[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;
              }
            }
            AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
          }
          else if(!connected_s && connected_s1 && connected_s2)
          {
            //cout << "inside 3" << endl;
            UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
            for(int j=0; j < (int) succ_ind2.size(); j++)
            {
              if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
              {
                AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                S[2]->SetCurrentPosition(succ_ind2[j]);
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;
              }
            }

            AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
          }
          else if(connected_s && !connected_s1 && !connected_s2)
          {
            //cout << "inside 4" << endl;
            UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
            UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            for(int j=0; j < (int) succ_ind1.size(); j++)
            {
              if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                continue;
              for(int k=0; k < (int) succ_ind.size(); k++)
              {
                if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
                {
                  AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                  AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  S[0]->SetCurrentPosition(succ_ind[k]);
                  S[1]->SetCurrentPosition(succ_ind1[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }
            }
            AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
            AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

          }
          else if(!connected_s && connected_s1 && !connected_s2)
          {
            //cout << "inside 5" << endl;
            UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
            UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
            for(int j=0; j < (int) succ_ind2.size(); j++)
            {
              if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                continue;
              for(int k=0; k < (int) succ_ind1.size(); k++)
              {
                if(!isTECfree(succ_ind1[k]->getPE()->getIndexI(), succ_ind1[k]->getPE()->getIndexJ(), succ_ind1[k]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                {
                  AssignTEC(succ_ind1[k]->getPE()->getIndexI(), succ_ind1[k]->getPE()->getIndexJ(), succ_ind1[k]->getPE()->getIndexT(), succ_ind1[k]->getNode()->get_ID());
                  AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  S[1]->SetCurrentPosition(succ_ind1[k]);
                  S[2]->SetCurrentPosition(succ_ind2[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }
            }
            AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
            AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

          }
          else if(!connected_s && !connected_s1 && connected_s2)
          {
            //cout << "inside 6" << endl;
            UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
            UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            for(int j=0; j < (int) succ_ind2.size(); j++)
            {
              if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                continue;
              for(int k=0; k < (int) succ_ind.size(); k++)
              {
                if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                {
                  AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                  AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                  MappingPair* mappair = mapPairs[ind[i]];
                  mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                  S[0]->SetCurrentPosition(succ_ind[k]);
                  S[2]->SetCurrentPosition(succ_ind2[j]);
                  AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                  return true;
                }
              }
            }
            AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
            AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
          }
          else if(!connected_s && !connected_s1 && !connected_s2)
          {
            //cout << "inside 7" << endl;
            UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
            UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
            for(int j=0; j < (int) succ_ind2.size(); j++)
            {
              if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                continue;
              for(int k=0; k < (int) succ_ind.size(); k++)
              {
                if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                  continue;
                for(int l=0; l < (int) succ_ind1.size(); l++)
                {
                  if(!isTECfree(succ_ind1[l]->getPE()->getIndexI(), succ_ind1[l]->getPE()->getIndexJ(), succ_ind1[l]->getPE()->getIndexT()))
                    continue;
                  if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[l]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                  {
                    AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                    AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                    AssignTEC(succ_ind1[l]->getPE()->getIndexI(), succ_ind1[l]->getPE()->getIndexJ(), succ_ind1[l]->getPE()->getIndexT(), succ_ind1[l]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    S[0]->SetCurrentPosition(succ_ind[k]);
                    S[1]->SetCurrentPosition(succ_ind1[l]);
                    S[2]->SetCurrentPosition(succ_ind2[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                  }

                }

              }
            }
            AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
            AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
            AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
          }
          else
          {
            MappingPair* mappair = mapPairs[ind[i]];
            mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);        
            AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
            return true;
            _FATAL("should not come here! case 9 shallow node\n"); 
            //_FATAL("In Node recovery shallow map_case: %d\n", map_case);
          }
          break;
        }
      case 13:
        {
          vector<Node*> P;
          vector<Node*> S;

          tie(P,S) = get_Mapped_pred_succ(n);
          vector<MappingPair*> succ_ind2 = S[2]->GetMappedIndices();
          vector<MappingPair*> succ_ind1 = S[1]->GetMappedIndices();
          vector<MappingPair*> succ_ind = S[0]->GetMappedIndices();
          vector<MappingPair*> pred_ind = P[0]->GetMappedIndices();
          MappingPair* succ_pos = S[0]->GetCurrentPosition();
          MappingPair* succ_pos1 = S[1]->GetCurrentPosition();
          MappingPair* succ_pos2 = S[2]->GetCurrentPosition();
          MappingPair* pred_pos = P[0]->GetCurrentPosition();

          bool is_load_add = P[0]->is_Load_Address_Generator(); 

          bool connected_s = false, connected_s1 = false, connected_s2 = false, connected_p = false; 

          if(!is_load_add)
          {
            if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos2->getPE()))
              connected_s = true;
            if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos->getPE()))
              connected_s1 = true; 
            if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos1->getPE()))
              connected_s2 = true;
            if(connectedPEs(pred_pos->getPE(), mapPairs[ind[i]]->getPE()))
              connected_p = true;

            if(connected_p)
            {
              if(connected_s && connected_s1 && !connected_s2)
              {
                //cout << "inside 1" << endl;
                UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

                for(int j=0; j < (int) succ_ind1.size(); j++)
                {
                  if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
                  {
                    AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    S[1]->SetCurrentPosition(succ_ind1[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;

                  }
                }
                AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

              }
              else if(connected_s && !connected_s1 && connected_s2)
              {
                //cout << "inside 2" << endl;
                UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

                for(int j=0; j < (int) succ_ind.size(); j++)
                {
                  if(!isTECfree(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[j]->getPE()))
                  {
                    AssignTEC(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT(), succ_ind[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    S[0]->SetCurrentPosition(succ_ind[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                  }
                }
                AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
              }
              else if(!connected_s && connected_s1 && connected_s2)
              {
                //cout << "inside 3" << endl;
                UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                for(int j=0; j < (int) succ_ind2.size(); j++)
                {
                  if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                  {
                    AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    S[2]->SetCurrentPosition(succ_ind2[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                  }
                }

                AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
              }
              else if(connected_s && !connected_s1 && !connected_s2)
              {
                //cout << "inside 4" << endl;
                UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                for(int j=0; j < (int) succ_ind1.size(); j++)
                {
                  if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                    continue;
                  for(int k=0; k < (int) succ_ind.size(); k++)
                  {
                    if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                      continue;

                    if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
                    {
                      AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                      AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                      MappingPair* mappair = mapPairs[ind[i]];
                      mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                      S[0]->SetCurrentPosition(succ_ind[k]);
                      S[1]->SetCurrentPosition(succ_ind1[j]);
                      AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                      return true;
                    }
                  }
                }
                AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

              }
              else if(!connected_s && connected_s1 && !connected_s2)
              {
                //cout << "inside 5" << endl;
                UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                for(int j=0; j < (int) succ_ind2.size(); j++)
                {
                  if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                    continue;
                  for(int k=0; k < (int) succ_ind1.size(); k++)
                  {
                    if(!isTECfree(succ_ind1[k]->getPE()->getIndexI(), succ_ind1[k]->getPE()->getIndexJ(), succ_ind1[k]->getPE()->getIndexT()))
                      continue;

                    if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                    {
                      AssignTEC(succ_ind1[k]->getPE()->getIndexI(), succ_ind1[k]->getPE()->getIndexJ(), succ_ind1[k]->getPE()->getIndexT(), succ_ind1[k]->getNode()->get_ID());
                      AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                      MappingPair* mappair = mapPairs[ind[i]];
                      mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                      S[1]->SetCurrentPosition(succ_ind1[k]);
                      S[2]->SetCurrentPosition(succ_ind2[j]);
                      AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                      return true;
                    }
                  }
                }
                AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

              }
              else if(!connected_s && !connected_s1 && connected_s2)
              {
                //cout << "inside 6" << endl;
                UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                for(int j=0; j < (int) succ_ind2.size(); j++)
                {
                  if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                    continue;
                  for(int k=0; k < (int) succ_ind.size(); k++)
                  {
                    if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                      continue;

                    if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                    {
                      AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                      AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                      MappingPair* mappair = mapPairs[ind[i]];
                      mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                      S[0]->SetCurrentPosition(succ_ind[k]);
                      S[2]->SetCurrentPosition(succ_ind2[j]);
                      AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                      return true;
                    }
                  }
                }
                AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
              }
              else if(!connected_s && !connected_s1 && !connected_s2)
              {
                //cout << "inside 7" << endl;
                UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                for(int j=0; j < (int) succ_ind2.size(); j++)
                {
                  if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                    continue;
                  for(int k=0; k < (int) succ_ind.size(); k++)
                  {
                    if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                      continue;
                    for(int l=0; l < (int) succ_ind1.size(); l++)
                    {
                      if(!isTECfree(succ_ind1[l]->getPE()->getIndexI(), succ_ind1[l]->getPE()->getIndexJ(), succ_ind1[l]->getPE()->getIndexT()))
                        continue;
                      if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[l]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                      {
                        AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                        AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                        AssignTEC(succ_ind1[l]->getPE()->getIndexI(), succ_ind1[l]->getPE()->getIndexJ(), succ_ind1[l]->getPE()->getIndexT(), succ_ind1[l]->getNode()->get_ID());
                        MappingPair* mappair = mapPairs[ind[i]];
                        mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                        S[0]->SetCurrentPosition(succ_ind[k]);
                        S[1]->SetCurrentPosition(succ_ind1[l]);
                        S[2]->SetCurrentPosition(succ_ind2[j]);
                        AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                        return true;
                      }

                    }

                  }
                }
                AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
              }
              else
              {
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);        
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;
                _FATAL("should not come here! case 9 shallow node\n"); 
                //_FATAL("In Node recovery shallow map_case: %d\n", map_case);
              }
            }
            else
            {
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              for(int jj=0; jj < (int) pred_ind.size(); jj++)
              {
                if(connectedPEs(pred_ind[jj]->getPE(), mapPairs[ind[i]]->getPE()))
                {
                  if(connected_s && connected_s1 && !connected_s2)
                  {
                    //cout << "inside 1" << endl;
                    UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

                    for(int j=0; j < (int) succ_ind1.size(); j++)
                    {
                      if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                        continue;

                      if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
                      {
                        AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                        MappingPair* mappair = mapPairs[ind[i]];
                        mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                        S[1]->SetCurrentPosition(succ_ind1[j]);
                        AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                        P[0]->SetCurrentPosition(pred_ind[jj]);
                        AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                        return true;

                      }
                    }
                    AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

                  }
                  else if(connected_s && !connected_s1 && connected_s2)
                  {
                    //cout << "inside 2" << endl;
                    UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

                    for(int j=0; j < (int) succ_ind.size(); j++)
                    {
                      if(!isTECfree(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT()))
                        continue;

                      if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[j]->getPE()))
                      {
                        AssignTEC(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT(), succ_ind[j]->getNode()->get_ID());
                        MappingPair* mappair = mapPairs[ind[i]];
                        mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                        S[0]->SetCurrentPosition(succ_ind[j]);
                        AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                        P[0]->SetCurrentPosition(pred_ind[jj]);
                        AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                        return true;
                      }
                    }
                    AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                  }
                  else if(!connected_s && connected_s1 && connected_s2)
                  {
                    //cout << "inside 3" << endl;
                    UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                    for(int j=0; j < (int) succ_ind2.size(); j++)
                    {
                      if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                        continue;

                      if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                      {
                        AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                        MappingPair* mappair = mapPairs[ind[i]];
                        mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                        S[2]->SetCurrentPosition(succ_ind2[j]);
                        AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                        P[0]->SetCurrentPosition(pred_ind[jj]);
                        AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                        return true;
                      }
                    }

                    AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                  }
                  else if(connected_s && !connected_s1 && !connected_s2)
                  {
                    //cout << "inside 4" << endl;
                    UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                    UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                    for(int j=0; j < (int) succ_ind1.size(); j++)
                    {
                      if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                        continue;
                      for(int k=0; k < (int) succ_ind.size(); k++)
                      {
                        if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                          continue;

                        if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
                        {
                          AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                          AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                          MappingPair* mappair = mapPairs[ind[i]];
                          mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                          S[0]->SetCurrentPosition(succ_ind[k]);
                          S[1]->SetCurrentPosition(succ_ind1[j]);
                          AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                          P[0]->SetCurrentPosition(pred_ind[jj]);
                          AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                          return true;
                        }
                      }
                    }
                    AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                    AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

                  }
                  else if(!connected_s && connected_s1 && !connected_s2)
                  {
                    //cout << "inside 5" << endl;
                    UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                    UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                    for(int j=0; j < (int) succ_ind2.size(); j++)
                    {
                      if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                        continue;
                      for(int k=0; k < (int) succ_ind1.size(); k++)
                      {
                        if(!isTECfree(succ_ind1[k]->getPE()->getIndexI(), succ_ind1[k]->getPE()->getIndexJ(), succ_ind1[k]->getPE()->getIndexT()))
                          continue;

                        if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                        {
                          AssignTEC(succ_ind1[k]->getPE()->getIndexI(), succ_ind1[k]->getPE()->getIndexJ(), succ_ind1[k]->getPE()->getIndexT(), succ_ind1[k]->getNode()->get_ID());
                          AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                          MappingPair* mappair = mapPairs[ind[i]];
                          mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                          S[1]->SetCurrentPosition(succ_ind1[k]);
                          S[2]->SetCurrentPosition(succ_ind2[j]);
                          AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                          P[0]->SetCurrentPosition(pred_ind[jj]);
                          AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                          return true;
                        }
                      }
                    }
                    AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                    AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

                  }
                  else if(!connected_s && !connected_s1 && connected_s2)
                  {
                    //cout << "inside 6" << endl;
                    UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                    UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                    for(int j=0; j < (int) succ_ind2.size(); j++)
                    {
                      if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                        continue;
                      for(int k=0; k < (int) succ_ind.size(); k++)
                      {
                        if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                          continue;

                        if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                        {
                          AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                          AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                          MappingPair* mappair = mapPairs[ind[i]];
                          mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                          S[0]->SetCurrentPosition(succ_ind[k]);
                          S[2]->SetCurrentPosition(succ_ind2[j]);
                          AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                          P[0]->SetCurrentPosition(pred_ind[jj]);
                          AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                          return true;
                        }
                      }
                    }
                    AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                    AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                  }
                  else if(!connected_s && !connected_s1 && !connected_s2)
                  {
                    //cout << "inside 7" << endl;
                    UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                    UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                    UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                    for(int j=0; j < (int) succ_ind2.size(); j++)
                    {
                      if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                        continue;
                      for(int k=0; k < (int) succ_ind.size(); k++)
                      {
                        if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                          continue;
                        for(int l=0; l < (int) succ_ind1.size(); l++)
                        {
                          if(!isTECfree(succ_ind1[l]->getPE()->getIndexI(), succ_ind1[l]->getPE()->getIndexJ(), succ_ind1[l]->getPE()->getIndexT()))
                            continue;
                          if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[l]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                          {
                            AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                            AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                            AssignTEC(succ_ind1[l]->getPE()->getIndexI(), succ_ind1[l]->getPE()->getIndexJ(), succ_ind1[l]->getPE()->getIndexT(), succ_ind1[l]->getNode()->get_ID());
                            MappingPair* mappair = mapPairs[ind[i]];
                            mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                            S[0]->SetCurrentPosition(succ_ind[k]);
                            S[1]->SetCurrentPosition(succ_ind1[l]);
                            S[2]->SetCurrentPosition(succ_ind2[j]);
                            AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                            P[0]->SetCurrentPosition(pred_ind[jj]);
                            AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                            return true;
                          }

                        }

                      }
                    }
                    AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                    AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                    AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                  }
                  else
                  {
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);       
                    AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                    P[0]->SetCurrentPosition(pred_ind[jj]); 
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                    _FATAL("should not come here! case 9 shallow node\n"); 
                    //_FATAL("In Node recovery shallow map_case: %d\n", map_case);
                  }
                }
              }
              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            }
          }
          else
          {
            if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos2->getPE()))
              connected_s = true;
            if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos->getPE()))
              connected_s1 = true; 
            if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_pos1->getPE()))
              connected_s2 = true;
            if(pred_pos->getPE()->getIndexI() == mapPairs[ind[i]]->getPE()->getIndexI())
              connected_p = true;

            if(connected_p)
            {
              if(connected_s && connected_s1 && !connected_s2)
              {
                //cout << "inside 1" << endl;
                UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

                for(int j=0; j < (int) succ_ind1.size(); j++)
                {
                  if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
                  {
                    AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    S[1]->SetCurrentPosition(succ_ind1[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;

                  }
                }
                AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

              }
              else if(connected_s && !connected_s1 && connected_s2)
              {
                //cout << "inside 2" << endl;
                UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

                for(int j=0; j < (int) succ_ind.size(); j++)
                {
                  if(!isTECfree(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[j]->getPE()))
                  {
                    AssignTEC(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT(), succ_ind[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    S[0]->SetCurrentPosition(succ_ind[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                  }
                }
                AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
              }
              else if(!connected_s && connected_s1 && connected_s2)
              {
                //cout << "inside 3" << endl;
                UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                for(int j=0; j < (int) succ_ind2.size(); j++)
                {
                  if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                    continue;

                  if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                  {
                    AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                    S[2]->SetCurrentPosition(succ_ind2[j]);
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                  }
                }

                AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
              }
              else if(connected_s && !connected_s1 && !connected_s2)
              {
                //cout << "inside 4" << endl;
                UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                for(int j=0; j < (int) succ_ind1.size(); j++)
                {
                  if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                    continue;
                  for(int k=0; k < (int) succ_ind.size(); k++)
                  {
                    if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                      continue;

                    if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
                    {
                      AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                      AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                      MappingPair* mappair = mapPairs[ind[i]];
                      mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                      S[0]->SetCurrentPosition(succ_ind[k]);
                      S[1]->SetCurrentPosition(succ_ind1[j]);
                      AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                      return true;
                    }
                  }
                }
                AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

              }
              else if(!connected_s && connected_s1 && !connected_s2)
              {
                //cout << "inside 5" << endl;
                UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                for(int j=0; j < (int) succ_ind2.size(); j++)
                {
                  if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                    continue;
                  for(int k=0; k < (int) succ_ind1.size(); k++)
                  {
                    if(!isTECfree(succ_ind1[k]->getPE()->getIndexI(), succ_ind1[k]->getPE()->getIndexJ(), succ_ind1[k]->getPE()->getIndexT()))
                      continue;

                    if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                    {
                      AssignTEC(succ_ind1[k]->getPE()->getIndexI(), succ_ind1[k]->getPE()->getIndexJ(), succ_ind1[k]->getPE()->getIndexT(), succ_ind1[k]->getNode()->get_ID());
                      AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                      MappingPair* mappair = mapPairs[ind[i]];
                      mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                      S[1]->SetCurrentPosition(succ_ind1[k]);
                      S[2]->SetCurrentPosition(succ_ind2[j]);
                      AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                      return true;
                    }
                  }
                }
                AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

              }
              else if(!connected_s && !connected_s1 && connected_s2)
              {
                //cout << "inside 6" << endl;
                UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                for(int j=0; j < (int) succ_ind2.size(); j++)
                {
                  if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                    continue;
                  for(int k=0; k < (int) succ_ind.size(); k++)
                  {
                    if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                      continue;

                    if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                    {
                      AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                      AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                      MappingPair* mappair = mapPairs[ind[i]];
                      mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                      S[0]->SetCurrentPosition(succ_ind[k]);
                      S[2]->SetCurrentPosition(succ_ind2[j]);
                      AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                      return true;
                    }
                  }
                }
                AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
              }
              else if(!connected_s && !connected_s1 && !connected_s2)
              {
                //cout << "inside 7" << endl;
                UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                for(int j=0; j < (int) succ_ind2.size(); j++)
                {
                  if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                    continue;
                  for(int k=0; k < (int) succ_ind.size(); k++)
                  {
                    if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                      continue;
                    for(int l=0; l < (int) succ_ind1.size(); l++)
                    {
                      if(!isTECfree(succ_ind1[l]->getPE()->getIndexI(), succ_ind1[l]->getPE()->getIndexJ(), succ_ind1[l]->getPE()->getIndexT()))
                        continue;
                      if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[l]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                      {
                        AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                        AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                        AssignTEC(succ_ind1[l]->getPE()->getIndexI(), succ_ind1[l]->getPE()->getIndexJ(), succ_ind1[l]->getPE()->getIndexT(), succ_ind1[l]->getNode()->get_ID());
                        MappingPair* mappair = mapPairs[ind[i]];
                        mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                        S[0]->SetCurrentPosition(succ_ind[k]);
                        S[1]->SetCurrentPosition(succ_ind1[l]);
                        S[2]->SetCurrentPosition(succ_ind2[j]);
                        AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                        return true;
                      }

                    }

                  }
                }
                AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
              }
              else
              {
                MappingPair* mappair = mapPairs[ind[i]];
                mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);        
                AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                return true;
                _FATAL("should not come here! case 9 shallow node\n"); 
                //_FATAL("In Node recovery shallow map_case: %d\n", map_case);
              }
            }
            else
            {
              UnAssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
              for(int jj=0; jj < (int) pred_ind.size(); jj++)
              {
                if(pred_ind[jj]->getPE()->getIndexI() == mapPairs[ind[i]]->getPE()->getIndexI())
                {
                  if(connected_s && connected_s1 && !connected_s2)
                  {
                    //cout << "inside 1" << endl;
                    UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

                    for(int j=0; j < (int) succ_ind1.size(); j++)
                    {
                      if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                        continue;

                      if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
                      {
                        AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                        MappingPair* mappair = mapPairs[ind[i]];
                        mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                        S[1]->SetCurrentPosition(succ_ind1[j]);
                        AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                        P[0]->SetCurrentPosition(pred_ind[jj]);
                        AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                        return true;

                      }
                    }
                    AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

                  }
                  else if(connected_s && !connected_s1 && connected_s2)
                  {
                    //cout << "inside 2" << endl;
                    UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

                    for(int j=0; j < (int) succ_ind.size(); j++)
                    {
                      if(!isTECfree(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT()))
                        continue;

                      if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[j]->getPE()))
                      {
                        AssignTEC(succ_ind[j]->getPE()->getIndexI(), succ_ind[j]->getPE()->getIndexJ(), succ_ind[j]->getPE()->getIndexT(), succ_ind[j]->getNode()->get_ID());
                        MappingPair* mappair = mapPairs[ind[i]];
                        mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                        S[0]->SetCurrentPosition(succ_ind[j]);
                        AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                        P[0]->SetCurrentPosition(pred_ind[jj]);
                        AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                        return true;
                      }
                    }
                    AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                  }
                  else if(!connected_s && connected_s1 && connected_s2)
                  {
                    //cout << "inside 3" << endl;
                    UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                    for(int j=0; j < (int) succ_ind2.size(); j++)
                    {
                      if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                        continue;

                      if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                      {
                        AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                        MappingPair* mappair = mapPairs[ind[i]];
                        mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                        S[2]->SetCurrentPosition(succ_ind2[j]);
                        AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                        P[0]->SetCurrentPosition(pred_ind[jj]);
                        AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                        return true;
                      }
                    }

                    AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                  }
                  else if(connected_s && !connected_s1 && !connected_s2)
                  {
                    //cout << "inside 4" << endl;
                    UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                    UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                    for(int j=0; j < (int) succ_ind1.size(); j++)
                    {
                      if(!isTECfree(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT()))
                        continue;
                      for(int k=0; k < (int) succ_ind.size(); k++)
                      {
                        if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                          continue;

                        if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[j]->getPE()))
                        {
                          AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                          AssignTEC(succ_ind1[j]->getPE()->getIndexI(), succ_ind1[j]->getPE()->getIndexJ(), succ_ind1[j]->getPE()->getIndexT(), succ_ind1[j]->getNode()->get_ID());
                          MappingPair* mappair = mapPairs[ind[i]];
                          mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                          S[0]->SetCurrentPosition(succ_ind[k]);
                          S[1]->SetCurrentPosition(succ_ind1[j]);
                          AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                          P[0]->SetCurrentPosition(pred_ind[jj]);
                          AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                          return true;
                        }
                      }
                    }
                    AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                    AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());

                  }
                  else if(!connected_s && connected_s1 && !connected_s2)
                  {
                    //cout << "inside 5" << endl;
                    UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                    UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                    for(int j=0; j < (int) succ_ind2.size(); j++)
                    {
                      if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                        continue;
                      for(int k=0; k < (int) succ_ind1.size(); k++)
                      {
                        if(!isTECfree(succ_ind1[k]->getPE()->getIndexI(), succ_ind1[k]->getPE()->getIndexJ(), succ_ind1[k]->getPE()->getIndexT()))
                          continue;

                        if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                        {
                          AssignTEC(succ_ind1[k]->getPE()->getIndexI(), succ_ind1[k]->getPE()->getIndexJ(), succ_ind1[k]->getPE()->getIndexT(), succ_ind1[k]->getNode()->get_ID());
                          AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                          MappingPair* mappair = mapPairs[ind[i]];
                          mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                          S[1]->SetCurrentPosition(succ_ind1[k]);
                          S[2]->SetCurrentPosition(succ_ind2[j]);
                          AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                          P[0]->SetCurrentPosition(pred_ind[jj]);
                          AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                          return true;
                        }
                      }
                    }
                    AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                    AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());

                  }
                  else if(!connected_s && !connected_s1 && connected_s2)
                  {
                    //cout << "inside 6" << endl;
                    UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                    UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                    for(int j=0; j < (int) succ_ind2.size(); j++)
                    {
                      if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                        continue;
                      for(int k=0; k < (int) succ_ind.size(); k++)
                      {
                        if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                          continue;

                        if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                        {
                          AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                          AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                          MappingPair* mappair = mapPairs[ind[i]];
                          mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                          S[0]->SetCurrentPosition(succ_ind[k]);
                          S[2]->SetCurrentPosition(succ_ind2[j]);
                          AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                          P[0]->SetCurrentPosition(pred_ind[jj]);
                          AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                          return true;
                        }
                      }
                    }
                    AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                    AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                  }
                  else if(!connected_s && !connected_s1 && !connected_s2)
                  {
                    //cout << "inside 7" << endl;
                    UnAssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                    UnAssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                    UnAssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                    for(int j=0; j < (int) succ_ind2.size(); j++)
                    {
                      if(!isTECfree(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT()))
                        continue;
                      for(int k=0; k < (int) succ_ind.size(); k++)
                      {
                        if(!isTECfree(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT()))
                          continue;
                        for(int l=0; l < (int) succ_ind1.size(); l++)
                        {
                          if(!isTECfree(succ_ind1[l]->getPE()->getIndexI(), succ_ind1[l]->getPE()->getIndexJ(), succ_ind1[l]->getPE()->getIndexT()))
                            continue;
                          if(connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind[k]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind1[l]->getPE()) && connectedPEs(mapPairs[ind[i]]->getPE(), succ_ind2[j]->getPE()))
                          {
                            AssignTEC(succ_ind[k]->getPE()->getIndexI(), succ_ind[k]->getPE()->getIndexJ(), succ_ind[k]->getPE()->getIndexT(), succ_ind[k]->getNode()->get_ID());
                            AssignTEC(succ_ind2[j]->getPE()->getIndexI(), succ_ind2[j]->getPE()->getIndexJ(), succ_ind2[j]->getPE()->getIndexT(), succ_ind2[j]->getNode()->get_ID());
                            AssignTEC(succ_ind1[l]->getPE()->getIndexI(), succ_ind1[l]->getPE()->getIndexJ(), succ_ind1[l]->getPE()->getIndexT(), succ_ind1[l]->getNode()->get_ID());
                            MappingPair* mappair = mapPairs[ind[i]];
                            mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);
                            S[0]->SetCurrentPosition(succ_ind[k]);
                            S[1]->SetCurrentPosition(succ_ind1[l]);
                            S[2]->SetCurrentPosition(succ_ind2[j]);
                            AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                            P[0]->SetCurrentPosition(pred_ind[jj]);
                            AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                            return true;
                          }

                        }

                      }
                    }
                    AssignTEC(succ_pos2->getPE()->getIndexI(), succ_pos2->getPE()->getIndexJ(), succ_pos2->getPE()->getIndexT(), succ_pos2->getNode()->get_ID());
                    AssignTEC(succ_pos->getPE()->getIndexI(), succ_pos->getPE()->getIndexJ(), succ_pos->getPE()->getIndexT(), succ_pos->getNode()->get_ID());
                    AssignTEC(succ_pos1->getPE()->getIndexI(), succ_pos1->getPE()->getIndexJ(), succ_pos1->getPE()->getIndexT(), succ_pos1->getNode()->get_ID());
                  }
                  else
                  {
                    MappingPair* mappair = mapPairs[ind[i]];
                    mapPairs[ind[i]]->getNode()->SetCurrentPosition(mappair);       
                    AssignTEC(pred_ind[jj]->getPE()->getIndexI(), pred_ind[jj]->getPE()->getIndexJ(), pred_ind[jj]->getPE()->getIndexT(), pred_ind[jj]->getNode()->get_ID());
                    P[0]->SetCurrentPosition(pred_ind[jj]); 
                    AssignTEC(mapPairs[ind[i]]->getPE()->getIndexI(), mapPairs[ind[i]]->getPE()->getIndexJ(), mapPairs[ind[i]]->getPE()->getIndexT(), mapPairs[ind[i]]->getNode()->get_ID());
                    return true;
                    //_FATAL("In Node recovery shallow map_case: %d\n", map_case);
                  }
                }
              }
              AssignTEC(pred_pos->getPE()->getIndexI(), pred_pos->getPE()->getIndexJ(), pred_pos->getPE()->getIndexT(), pred_pos->getNode()->get_ID());
            }

          }
          break;
        }
      case 14:
        {
          _FATAL("In Node recovery shallow map_case: %d\n", map_case);
          break;
        }
      case 15:
        {
          _FATAL("In Node recovery shallow map_case: %d\n", map_case);
          break;
        }
      case 16:
        {
          _FATAL("In Node recovery shallow map_case: %d\n", map_case);
          break;
        }
      case 17:
        {
          _FATAL("In Node recovery shallow map_case: %d\n", map_case);
          break;
        }
      case 18:
        {
          _FATAL("In Node recovery shallow map_case: %d\n", map_case);
          break;
        }
      case 19:
        {
          _FATAL("In Node recovery shallow map_case: %d\n", map_case);
          break;
        }
      case 20:
        {
          _FATAL("In Node recovery shallow map_case: %d\n", map_case);
          break;
        }
      case 21:
        {
          _FATAL("In Node recovery shallow map_case: %d\n", map_case);
          break;
        }
      case 22:
        {
          _FATAL("In Node recovery shallow map_case: %d\n", map_case);
          break;
        }
      default:
        {
          _FATAL("In Node recovery shallow map_case: %d\n", map_case);
          break;
        }


    }
  }

  return false;
}

bool CGRA::Smart_Map_Failure_Recovery_Shallow_N(Node* n, int map_case)
{
  Matrix_Copy(copy_TEC);

  nodes_in_current_slot.clear();
  nodes_in_current_slot = get_Nodes_from_current_slot(n->get_Sched_Info()->get_Modulo_Current()); 

  for(int ii = 0; ii < (int) nodes_in_current_slot.size(); ii++)
  {
    TEC.free_current_timeslot(n->get_Sched_Info()->get_Modulo_Current(), SizeX);
    bool success  = Remap(nodes_in_current_slot);

    //TEC.print_matrix();
    //exit(1); 

    if(success)
    {
      bool success1 = Smart_Map_Recovery_Node_Shallow(n, map_case);  

      if(success1)
        return true;
    }
    // if not successfull empty the timeslot and then restore.
    TEC.free_current_timeslot(n->get_Sched_Info()->get_Modulo_Current(), SizeX);
    Restore_timeslot(n->get_Sched_Info()->get_Modulo_Current()); 
  }

  Restore(copy_TEC);
  return false;

}


bool CGRA::Smart_Map_Failure_Recovery_1Deep(Node* n, int map_case)
{
  Matrix_Copy(copy_TEC);

  vector<Node*> P;
  vector<Node*> S;

  tie(P,S) = get_Mapped_pred_succ(n);

  nodes_in_current_slot.clear();
  nodes_in_current_slot = get_Nodes_from_current_slot(n->get_Sched_Info()->get_Modulo_Current());

  if((int) P.size() == 0)
  {
    bool success = false, success1 = false;  
    for(int ii = 0; ii < (int) nodes_in_current_slot.size(); ii++)
    {
      TEC.free_current_timeslot(n->get_Sched_Info()->get_Modulo_Current(), SizeX);
      success  = Remap(nodes_in_current_slot);
      if(success)
      {
        nodes_in_succ_slot.clear();
        nodes_in_succ_slot = get_Nodes_from_current_slot(S[0]->get_Sched_Info()->get_Modulo_Current());
        for(int jj=0; jj < (int) nodes_in_succ_slot.size(); jj++)
        {

          TEC.free_current_timeslot(S[0]->get_Sched_Info()->get_Modulo_Current(), SizeX);
          success1 = Remap(nodes_in_succ_slot); 
          if(success1)
          {
            bool success2 = Smart_Map_Recovery_Node_Shallow(n, map_case);
            if(success2)
              return true; 
          }
          TEC.free_current_timeslot(S[0]->get_Sched_Info()->get_Modulo_Current(), SizeX);
          Restore_timeslot(S[0]->get_Sched_Info()->get_Modulo_Current()); 
          success1 = false; 
        }
      }
      TEC.free_current_timeslot(n->get_Sched_Info()->get_Modulo_Current(), SizeX);
      Restore_timeslot(n->get_Sched_Info()->get_Modulo_Current());
      success = false;
    }
  }
  else if((int)S.size() == 0)
  {
    bool success = false, success1 = false;
    for(int ii = 0; ii < (int) nodes_in_current_slot.size(); ii++)
    {
      TEC.free_current_timeslot(n->get_Sched_Info()->get_Modulo_Current(), SizeX);
      success  = Remap(nodes_in_current_slot);
      if(success)
      {
        nodes_in_pred_slot.clear();
        nodes_in_pred_slot = get_Nodes_from_current_slot(P[0]->get_Sched_Info()->get_Modulo_Current());
        for(int jj=0; jj < (int) nodes_in_pred_slot.size(); jj++)
        {

          TEC.free_current_timeslot(P[0]->get_Sched_Info()->get_Modulo_Current(), SizeX);
          success1 = Remap(nodes_in_pred_slot);
          if(success1)
          {
            bool success2 = Smart_Map_Recovery_Node_Shallow(n, map_case);
            if(success2)
              return true;
          }
          TEC.free_current_timeslot(P[0]->get_Sched_Info()->get_Modulo_Current(), SizeX);
          Restore_timeslot(P[0]->get_Sched_Info()->get_Modulo_Current());
          success1 = false;
        }
      }
      TEC.free_current_timeslot(n->get_Sched_Info()->get_Modulo_Current(), SizeX);
      Restore_timeslot(n->get_Sched_Info()->get_Modulo_Current());
      success = false;
    }
  }
  else
  {
    //cout << " 1 deep mapcase: " << map_case << " inside P size !=0" << endl;

    //cout << "nodes in current slot: " << (int) nodes_in_current_slot.size() << endl;

    bool success = false, success1 = false, success2 = false, success3 = false;

    for(int ii = 0; ii < (int) nodes_in_current_slot.size(); ii++)
    {
      TEC.free_current_timeslot(n->get_Sched_Info()->get_Modulo_Current(), SizeX);
      success  = Remap(nodes_in_current_slot);

      //cout << "success: " << success << endl;

      if(success)
      {
        nodes_in_succ_slot.clear();
        nodes_in_succ_slot = get_Nodes_from_current_slot(S[0]->get_Sched_Info()->get_Modulo_Current());

        //cout << "nodes in succ slot: " << (int) nodes_in_succ_slot.size() << endl;

        for(int jj=0; jj < (int) nodes_in_succ_slot.size(); jj++)
        {

          TEC.free_current_timeslot(S[0]->get_Sched_Info()->get_Modulo_Current(), SizeX);
          success1 = Remap(nodes_in_succ_slot);

          //cout << "success1: " << success << endl;

          if(success1)
          {

            nodes_in_pred_slot.clear();
            //cout << "passed pred clear" << endl;
            nodes_in_pred_slot = get_Nodes_from_current_slot(P[0]->get_Sched_Info()->get_Modulo_Current());


            //cout << "nodes in pred slot: " << (int) nodes_in_pred_slot.size() << endl;


            for(int kk=0; kk < (int) nodes_in_pred_slot.size(); kk++)
            {
              TEC.free_current_timeslot(P[0]->get_Sched_Info()->get_Modulo_Current(), SizeX);
              success2 = Remap(nodes_in_pred_slot);

              //  cout << "success2: " << success << endl;

              if(success2)
              {
                // cout << "inside success3" << endl; 
                success3 = Smart_Map_Recovery_Node_Shallow(n, map_case); 

                if(success3)
                  return true;

                success3 = false; 
              }
              TEC.free_current_timeslot(P[0]->get_Sched_Info()->get_Modulo_Current(), SizeX);
              Restore_timeslot(P[0]->get_Sched_Info()->get_Modulo_Current());
              success2 = false; 
            }
          }
          TEC.free_current_timeslot(S[0]->get_Sched_Info()->get_Modulo_Current(), SizeX);
          Restore_timeslot(S[0]->get_Sched_Info()->get_Modulo_Current());
          success1 = false;
        }
      }
      TEC.free_current_timeslot(n->get_Sched_Info()->get_Modulo_Current(), SizeX);
      Restore_timeslot(n->get_Sched_Info()->get_Modulo_Current());
      success = false; 
    }


  }

  Restore(copy_TEC);
  return false;

}

/*bool CGRA::Smart_Map_Failure_Recovery_Shallow_N_succ(vector<Node*> connected_node1, Node* n)
  {

//cout << "Inside Smart Map failure Recovery Shallow 2 " << endl;

//Matrix<int> copy_TEC(TEC.rows(), TEC.cols());
//copy_TEC = TEC;
Matrix_Copy(copy_TEC);

nodes_in_current_slot.clear();
nodes_in_current_slot = get_Nodes_from_current_slot(n->get_Sched_Info()->get_Modulo_Current());
bool found = false;  

MappingPair *out=NULL, *out1=NULL, *out2=NULL, *out3=NULL, *out4=NULL; 

int succ_size = (int) connected_node1.size(); 

switch(succ_size)
{
case 1:
{
out = connected_node1[0]->GetCurrentPosition();     
break;  
}
case 2:
{
out = connected_node1[0]->GetCurrentPosition();
out1 = connected_node1[1]->GetCurrentPosition();
break;
}
case 3:
{ 
out = connected_node1[0]->GetCurrentPosition();
out1 = connected_node1[1]->GetCurrentPosition();
out2 = connected_node1[2]->GetCurrentPosition();
break;
}
case 4:
{
out = connected_node1[0]->GetCurrentPosition();
out1 = connected_node1[1]->GetCurrentPosition();
out2 = connected_node1[2]->GetCurrentPosition();
out3 = connected_node1[3]->GetCurrentPosition();
break;
}
case 5:
{
out = connected_node1[0]->GetCurrentPosition();
out1 = connected_node1[1]->GetCurrentPosition();
out2 = connected_node1[2]->GetCurrentPosition();
out3 = connected_node1[3]->GetCurrentPosition();
out4 = connected_node1[4]->GetCurrentPosition();  
break;
}
default:
_FATAL("out edges more than 5 are not supported\n");  

}

// instead of trying to remap with all the mapped nodes, unmap all the nodes in the timeslot.
// Remap them in the while loop one by one again with different coordinates (PEs). 
//TEC.free_current_timeslot(n->get_Sched_Info()->get_Modulo_Current(), SizeX);

// This may fall into an infinite loop? 
// if so please have a termination check.

//remap connected nodes and if success try to map the unmapped node.
//node_iterator = 0; 
//Node* start_node = nodes_in_current_slot[0]; 
// node_iterator++; 
// vector<MappingPair*> indices = start_node->GetMappedIndices();
// best_pairs.clear();  

for(int ii = 0; ii < (int) nodes_in_current_slot.size(); ii++)
{
  TEC.free_current_timeslot(n->get_Sched_Info()->get_Modulo_Current(), SizeX);
  bool success  = Remap(nodes_in_current_slot);

  if(success)
  {
    vector<int> ind = M.row_find(get_Node_Index(n), 1);

    for(int jj=0; jj < (int) ind.size(); jj++)
    {
      //cout << "Inside ind shallow 4" << endl; 
      if(!isTECfree(mapPairs[ind[jj]]->getPE()->getIndexI(), mapPairs[ind[jj]]->getPE()->getIndexJ(), mapPairs[ind[jj]]->getPE()->getIndexT()))
        continue;


      bool connection = false;
      switch(succ_size)
      {
        case 1:
          {
            if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()))
              connection = true;
            break;
          }
        case 2:
          {
            if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()))
              connection = true;
            break;
          }
        case 3:
          {
            if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()))
              connection = true;
            break;
          }
        case 4:
          {
            if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out3->getPE()))
              connection = true;
            break;
          }
        case 5:
          {
            if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out3->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out4->getPE()))
              connection = true;
            break;

          }
        default:
          connection = false;
      }

      if(connection)
      {
        //cout << "inside connected and returning 0 shallow 4" << endl;
        MappingPair* mappair = mapPairs[ind[jj]];
        mapPairs[ind[jj]]->getNode()->SetCurrentPosition(mappair);
        //BestIndices.push_back(mapPairs[ind[jj]]);
        //best_pairs.push_back(mapPairs[ind[jj]]);

        //BestIndices = Combine(BestIndices, best_pairs);

        AssignTEC(mapPairs[ind[jj]]->getPE()->getIndexI(), mapPairs[ind[jj]]->getPE()->getIndexJ(), mapPairs[ind[jj]]->getPE()->getIndexT(), mapPairs[ind[jj]]->getNode()->get_ID());
        //cout << endl;
        //TEC.print_matrix(); 
        return true;
      }
    }
  }  
  Restore_timeslot(n->get_Sched_Info()->get_Modulo_Current()); 
}

Restore(copy_TEC); 
return false;  
}*/
// Shallow recovery with n pred and n succ mapped (1-3 pred and 1-5 succ)

/*bool CGRA::Smart_Map_Failure_Recovery_Shallow_N(vector<Node*> connected_node1, vector<Node*> connected_node2, Node* n)
  {

//cout << "Inside Smart Map failure Recovery Shallow 2 " << endl;

//Matrix<int> copy_TEC(TEC.rows(), TEC.cols());
Matrix_Copy(copy_TEC);
nodes_in_current_slot.clear(); 
nodes_in_current_slot = get_Nodes_from_current_slot(n->get_Sched_Info()->get_Modulo_Current());
bool found = false;  

MappingPair* out=NULL, *out1=NULL, *out2=NULL, *out3=NULL, *out4=NULL;
MappingPair* in=NULL, *in1=NULL, *in2=NULL;

int pred_size = (int) connected_node1.size();
int succ_size = (int) connected_node2.size();

switch(pred_size)
{
case 1:
in  = connected_node1[0]->GetCurrentPosition();
break;
case 2:
in = connected_node1[0]->GetCurrentPosition();
in1 = connected_node1[1]->GetCurrentPosition();
break;
case 3:
in = connected_node1[0]->GetCurrentPosition();
in1 = connected_node1[1]->GetCurrentPosition();
in2 = connected_node1[2]->GetCurrentPosition();
break;
default:
_FATAL("More than 3 inputs are not allowed\n");

}

switch(succ_size)
{
case 1:
{
out = connected_node2[0]->GetCurrentPosition();
break;
}
case 2:
{
out = connected_node2[0]->GetCurrentPosition();
out1 = connected_node2[1]->GetCurrentPosition();
break;
}
case 3:
{
out = connected_node2[0]->GetCurrentPosition();
out1 = connected_node2[1]->GetCurrentPosition();
out2 = connected_node2[2]->GetCurrentPosition();
break;
}
case 4:
{
out = connected_node2[0]->GetCurrentPosition();
out = connected_node2[0]->GetCurrentPosition();
out = connected_node2[0]->GetCurrentPosition();
out = connected_node2[0]->GetCurrentPosition();
break;
}
case 5:
{
out = connected_node2[0]->GetCurrentPosition();
out = connected_node2[0]->GetCurrentPosition();
out = connected_node2[0]->GetCurrentPosition();
out = connected_node2[0]->GetCurrentPosition();
out = connected_node2[0]->GetCurrentPosition();
break;
}
default:
_FATAL("Outgoing nodes more than 5 is not supported currently\n");
}



for(int ii = 0; ii < (int) nodes_in_current_slot.size(); ii++)
{
  TEC.free_current_timeslot(n->get_Sched_Info()->get_Modulo_Current(), SizeX);
  bool success = Remap(nodes_in_current_slot);

  if(success)
  {
    vector<int> ind = M.row_find(get_Node_Index(n), 1);

    for(int jj=0; jj < (int) ind.size(); jj++)
    {
      //cout << "Inside ind shallow N" << endl;
      if(!isTECfree(mapPairs[ind[jj]]->getPE()->getIndexI(), mapPairs[ind[jj]]->getPE()->getIndexJ(), mapPairs[ind[jj]]->getPE()->getIndexT()))
        continue;

      bool connected = false, connected1 = false;
      switch(pred_size)
      {
        case 1:
          {
            if(connectedPEs(in->getPE(), mapPairs[ind[jj]]->getPE()))
              connected = true;
            break;
          }
        case 2:
          {
            if(connectedPEs(in->getPE(), mapPairs[ind[jj]]->getPE()) && connectedPEs(in1->getPE(), mapPairs[ind[jj]]->getPE()))      
              connected = true;

            break;

          }
        case 3:
          {
            if(connectedPEs(in->getPE(), mapPairs[ind[jj]]->getPE()) && connectedPEs(in1->getPE(), mapPairs[ind[jj]]->getPE()) &&  connectedPEs(in2->getPE(), mapPairs[ind[jj]]->getPE()))
              connected = true;

            break;

          }
        default:
          connected = false;
      }
      switch(succ_size)
      {
        case 1:
          {
            if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()))
              connected1 = true;
            break;
          }
        case 2:
          {
            if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()))
              connected1 = true;
            break;
          }
        case 3:
          {
            if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()))
              connected1 = true;
            break;
          }
        case 4:
          {
            if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out3->getPE()))
              connected1 = true;
            break;
          }
        case 5:
          {
            if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out3->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out4->getPE()))
              connected1 = true;
            break;
          }
        default:
          connected1 = false;
      } 

      if(connected && connected1)
      {
        MappingPair* mappair = mapPairs[ind[jj]]; 
        mapPairs[ind[jj]]->getNode()->SetCurrentPosition(mappair); 
        AssignTEC(mapPairs[ind[jj]]->getPE()->getIndexI(), mapPairs[ind[jj]]->getPE()->getIndexJ(), mapPairs[ind[jj]]->getPE()->getIndexT(), mapPairs[ind[jj]]->getNode()->get_ID());
        return true; 
      }
    } 
  }
  Restore_timeslot(n->get_Sched_Info()->get_Modulo_Current()); 
}
// if we are here then no mapping for found.
// restore TEC back. 

Restore(copy_TEC);  
return false;  
}

// 1 deep for n successors mapped. Max of 5. no pred mapped.

bool CGRA::Smart_Map_Failure_Recovery_1Deep_N_succ(vector<Node*> connected_node1, Node* n)
{

  //cout << "Inside Smart Map failure Recovery 1Deep 3" << endl;
  //TEC.print_matrix(); 
  //Matrix<int> copy_TEC(TEC.rows(), TEC.cols());
  //copy_TEC = TEC;
  Matrix_Copy(copy_TEC); 
  nodes_in_current_slot.clear(); 
  nodes_in_current_slot = get_Nodes_from_current_slot(n->get_Sched_Info()->get_Modulo_Current()); 

  MappingPair *out=NULL, *out1=NULL, *out2=NULL, *out3=NULL, *out4=NULL; 

  int succ_size = (int) connected_node1.size(); 

  switch(succ_size)
  {
    case 1:
      {
        out = connected_node1[0]->GetCurrentPosition();
        break;
      }
    case 2:
      {
        out = connected_node1[0]->GetCurrentPosition();
        out1 = connected_node1[1]->GetCurrentPosition();
        break;
      }
    case 3:
      {
        out = connected_node1[0]->GetCurrentPosition();
        out1 = connected_node1[1]->GetCurrentPosition();
        out2 = connected_node1[2]->GetCurrentPosition();
        break;
      }
    case 4:
      {
        out = connected_node1[0]->GetCurrentPosition();
        out = connected_node1[0]->GetCurrentPosition();
        out = connected_node1[0]->GetCurrentPosition();
        out = connected_node1[0]->GetCurrentPosition();
        break;
      }
    case 5:
      {
        out = connected_node1[0]->GetCurrentPosition();
        out = connected_node1[0]->GetCurrentPosition();
        out = connected_node1[0]->GetCurrentPosition();
        out = connected_node1[0]->GetCurrentPosition();
        out = connected_node1[0]->GetCurrentPosition();
        break;
      }
    default:
      _FATAL("Outgoing nodes more than 5 is not supported currently\n"); 
  }

  // instead of trying to remap with all the mapped nodes, unmap all the nodes in the timeslot.
  // Remap them in the while loop one by one again with different coordinates (PEs). 
  //TEC.free_current_timeslot(n->get_Sched_Info()->get_Modulo_Current(), SizeX);

  // This may fall into an infinite loop? 
  // if so please have a termination check.

  //remap connected nodes and if success try to map the unmapped node.

  for(int j=0; j < (int) nodes_in_current_slot.size(); j++)
  {
    TEC.free_current_timeslot(n->get_Sched_Info()->get_Modulo_Current(), SizeX);
    bool success = Remap(nodes_in_current_slot);

    if(success)
    {
      //TEC.print_matrix();  
      //cout << endl; 
      vector<int> ind = M.row_find(get_Node_Index(n), 1); 

      for(int jj=0; jj < (int) ind.size(); jj++)
      {
        cout << "Inside ind 1 deep 3" << endl; 
        if(!isTECfree(mapPairs[ind[jj]]->getPE()->getIndexI(), mapPairs[ind[jj]]->getPE()->getIndexJ(), mapPairs[ind[jj]]->getPE()->getIndexT()))
          continue;

        bool connection = false;
        switch(succ_size)
        {
          case 1:
            {
              if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()))
                connection = true;
              break;
            }
          case 2:
            {
              if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && mapPairs[ind[jj]]->getPE(), out1->getPE())
                connection = true;
              break;
            }
          case 3:
            {
              if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && mapPairs[ind[jj]]->getPE(), out1->getPE() && mapPairs[ind[jj]]->getPE(), out2->getPE())
                connection = true;
              break;
            }
          case 4:
            {
              if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && mapPairs[ind[jj]]->getPE(), out1->getPE() && mapPairs[ind[jj]]->getPE(), out2->getPE() && mapPairs[ind[jj]]->getPE(), out3->getPE())
                connection = true;
              break;
            }
          case 5:
            {
              if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && mapPairs[ind[jj]]->getPE(), out1->getPE() && mapPairs[ind[jj]]->getPE(), out2->getPE() && mapPairs[ind[jj]]->getPE(), out3->getPE() && mapPairs[ind[jj]]->getPE(), out4->getPE())
                connection = true;
              break;
            }
          default:
            connection = false;
        }

        if(connection)
        {
          //cout << "inside connected and returning 0 shallow 2" << endl;
          MappingPair* mappair = mapPairs[ind[jj]]; 
          mapPairs[ind[jj]]->getNode()->SetCurrentPosition(mappair);
          AssignTEC(mapPairs[ind[jj]]->getPE()->getIndexI(), mapPairs[ind[jj]]->getPE()->getIndexJ(), mapPairs[ind[jj]]->getPE()->getIndexT(), mapPairs[ind[jj]]->getNode()->get_ID());
          //cout << endl;
          //TEC.print_matrix(); 
          return true; 
        }
        else
        {
          // change succ timeslot
          nodes_in_succ_slot.clear();
          nodes_in_succ_slot = get_Nodes_from_current_slot(connected_node1[0]->get_Sched_Info()->get_Modulo_Current()); 

          for(int kk = 0; kk < (int) nodes_in_pred_slot.size(); kk++)
          {
            TEC.free_current_timeslot(connected_node1[0]->get_Sched_Info()->get_Modulo_Current(), SizeX);
            bool success1 = Remap(nodes_in_succ_slot);

            if(success1)
            {
              bool connected3 = false;
              switch(succ_size)
              {
                case 1:
                  {
                    if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()))
                      connected3 = true;
                    break;
                  }
                case 2:
                  {
                    if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()))
                      connected3 = true;
                    break;
                  }
                case 3:
                  {
                    if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()))
                      connected3 = true;
                    break;
                  }
                case 4:
                  {
                    if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out3->getPE()))
                      connected3 = true;
                    break;
                  }
                case 5:
                  {
                    if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out3->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out4->getPE()))
                      connected3 = true;
                    break;
                  }
                default:
                  connected3 = false;
              } 

              if(connected3)
              {
                MappingPair* mappair = mapPairs[ind[jj]]; 
                mapPairs[ind[jj]]->getNode()->SetCurrentPosition(mappair); 
                AssignTEC(mapPairs[ind[jj]]->getPE()->getIndexI(), mapPairs[ind[jj]]->getPE()->getIndexJ(), mapPairs[ind[jj]]->getPE()->getIndexT(), mapPairs[ind[jj]]->getNode()->get_ID());
                return true; 
              } 

            }
            Restore_timeslot(connected_node1[0]->get_Sched_Info()->get_Modulo_Current());
          }

        }
      } 
    }
    Restore_timeslot(n->get_Sched_Info()->get_Modulo_Current()); 
  }

  // if we are here then no mapping for found.
  // restore TEC back. 
  Restore(copy_TEC);
  return false;  

} 

// for n pred and n succ mapped (1-3 pred and 1-5 succ)
bool CGRA::Smart_Map_Failure_Recovery_1Deep_N(vector<Node*> connected_node1, vector<Node*> connected_node2, Node* n)
{

  //cout << "Inside Smart Map failure Recovery 1Deep N" << endl;
  //TEC.print_matrix(); 

  //Matrix<int> copy_TEC(TEC.rows(), TEC.cols());
  //copy_TEC = TEC;
  Matrix_Copy(copy_TEC); 
  nodes_in_current_slot.clear();
  nodes_in_current_slot = get_Nodes_from_current_slot(n->get_Sched_Info()->get_Modulo_Current()); 


  MappingPair* out=NULL, *out1=NULL, *out2=NULL, *out3=NULL, *out4=NULL; 
  MappingPair* in=NULL, *in1=NULL, *in2=NULL; 

  int pred_size = (int) connected_node1.size(); 
  int succ_size = (int) connected_node2.size(); 

  switch(pred_size)
  {
    case 1:
      in  = connected_node1[0]->GetCurrentPosition();
      break;
    case 2:
      in = connected_node1[0]->GetCurrentPosition();
      in1 = connected_node1[1]->GetCurrentPosition();
      break;
    case 3:
      in = connected_node1[0]->GetCurrentPosition();
      in1 = connected_node1[1]->GetCurrentPosition();
      in2 = connected_node1[2]->GetCurrentPosition();
      break;
    default:
      _FATAL("More than 3 inputs are not allowed\n");

  }

  switch(succ_size)
  {
    case 1:
      {
        out = connected_node2[0]->GetCurrentPosition();
        break;
      }
    case 2:
      {
        out = connected_node2[0]->GetCurrentPosition();
        out1 = connected_node2[1]->GetCurrentPosition();
        break;
      }
    case 3:
      {
        out = connected_node2[0]->GetCurrentPosition();
        out1 = connected_node2[1]->GetCurrentPosition();
        out2 = connected_node2[2]->GetCurrentPosition();
        break;
      }
    case 4:
      {
        out = connected_node2[0]->GetCurrentPosition();
        out = connected_node2[0]->GetCurrentPosition();
        out = connected_node2[0]->GetCurrentPosition();
        out = connected_node2[0]->GetCurrentPosition();
        break;
      }
    case 5:
      {
        out = connected_node2[0]->GetCurrentPosition();
        out = connected_node2[0]->GetCurrentPosition();
        out = connected_node2[0]->GetCurrentPosition();
        out = connected_node2[0]->GetCurrentPosition();
        out = connected_node2[0]->GetCurrentPosition();
        break;
      }
    default:
      _FATAL("Outgoing nodes more than 5 is not supported currently\n");
  } 


  // _FATAL("No. of nodes in the current timeslot %d\n", (int) nodes_in_current_slot.size()); 
  cout << "No. of nodes in the current timeslot " << (int) nodes_in_current_slot.size() << endl;

  for(int ii = 0; ii < (int) nodes_in_current_slot.size(); ii++)
  {
    cout << "Iteration: " << ii << endl;
    TEC.free_current_timeslot(n->get_Sched_Info()->get_Modulo_Current(), SizeX);
    bool success = Remap(nodes_in_current_slot);

    if(success)
    {
      vector<int> ind = M.row_find(get_Node_Index(n), 1);

      for(int jj=0; jj < (int) ind.size(); jj++)
      {
        //cout << "Inside ind shallow N" << endl;
        if(!isTECfree(mapPairs[ind[jj]]->getPE()->getIndexI(), mapPairs[ind[jj]]->getPE()->getIndexJ(), mapPairs[ind[jj]]->getPE()->getIndexT()))
          continue;

        bool connected = false, connected1 = false;
        switch(pred_size)
        {
          case 1:
            {
              if(connectedPEs(in->getPE(), mapPairs[ind[jj]]->getPE()))
                connected = true;
              break;
            }
          case 2:
            {
              if(connectedPEs(in->getPE(), mapPairs[ind[jj]]->getPE()) && connectedPEs(in1->getPE(), mapPairs[ind[jj]]->getPE()))      
                connected = true;

              break;

            }
          case 3:
            {
              if(connectedPEs(in->getPE(), mapPairs[ind[jj]]->getPE()) && connectedPEs(in1->getPE(), mapPairs[ind[jj]]->getPE()) &&  connectedPEs(in2->getPE(), mapPairs[ind[jj]]->getPE()))
                connected = true;

              break;

            }
          default:
            connected = false;
        }
        switch(succ_size)
        {
          case 1:
            {
              if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()))
                connected1 = true;
              break;
            }
          case 2:
            {
              if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()))
                connected1 = true;
              break;
            }
          case 3:
            {
              if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()))
                connected1 = true;
              break;
            }
          case 4:
            {
              if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out3->getPE()))
                connected1 = true;
              break;
            }
          case 5:
            {
              if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out3->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out4->getPE()))
                connected1 = true;
              break;
            }
          default:
            connected1 = false;
        } 

        if(connected && connected1)
        {
          MappingPair* mappair = mapPairs[ind[jj]]; 
          mapPairs[ind[jj]]->getNode()->SetCurrentPosition(mappair); 
          AssignTEC(mapPairs[ind[jj]]->getPE()->getIndexI(), mapPairs[ind[jj]]->getPE()->getIndexJ(), mapPairs[ind[jj]]->getPE()->getIndexT(), mapPairs[ind[jj]]->getNode()->get_ID());
          return true; 
        }
        else if(!connected && connected1)
        {
          cout << "Inside !pred connected and succ connected" << endl;
          // we have a connection for succ and not the pred. Change the pred timeslot
          nodes_in_pred_slot.clear();
          nodes_in_pred_slot = get_Nodes_from_current_slot(connected_node1[0]->get_Sched_Info()->get_Modulo_Current()); 

          cout << "No. of pred nodes: " << (int) nodes_in_pred_slot.size() << endl;

          for(int kk = 0; kk < (int) nodes_in_pred_slot.size(); kk++)
          {
            TEC.free_current_timeslot(connected_node1[0]->get_Sched_Info()->get_Modulo_Current(), SizeX);
            bool success1 = Remap(nodes_in_pred_slot);

            if(!success)
            {
              TEC.print_matrix();
              _FATAL("pred remap not successfull"); 
            }

            if(success1)
            {
              bool connected2 = false, connected3 = false;
              switch(pred_size)
              {
                case 1:
                  {
                    if(connectedPEs(in->getPE(), mapPairs[ind[jj]]->getPE()))
                      connected2 = true;
                    break;
                  }
                case 2:
                  {
                    if(connectedPEs(in->getPE(), mapPairs[ind[jj]]->getPE()) && connectedPEs(in1->getPE(), mapPairs[ind[jj]]->getPE()))      
                      connected2 = true;

                    break;

                  }
                case 3:
                  {
                    if(connectedPEs(in->getPE(), mapPairs[ind[jj]]->getPE()) && connectedPEs(in1->getPE(), mapPairs[ind[jj]]->getPE()) &&  connectedPEs(in2->getPE(), mapPairs[ind[jj]]->getPE()))
                      connected2 = true;

                    break;

                  }
                default:
                  connected2 = false;
              }
              switch(succ_size)
              {
                case 1:
                  {
                    if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()))
                      connected3 = true;
                    break;
                  }
                case 2:
                  {
                    if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()))
                      connected3 = true;
                    break;
                  }
                case 3:
                  {
                    if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()))
                      connected3 = true;
                    break;
                  }
                case 4:
                  {
                    if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out3->getPE()))
                      connected3 = true;
                    break;
                  }
                case 5:
                  {
                    if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out3->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out4->getPE()))
                      connected3 = true;
                    break;
                  }
                default:
                  connected3 = false;
              } 

              if(connected2 && connected3)
              {
                MappingPair* mappair = mapPairs[ind[jj]]; 
                mapPairs[ind[jj]]->getNode()->SetCurrentPosition(mappair); 
                AssignTEC(mapPairs[ind[jj]]->getPE()->getIndexI(), mapPairs[ind[jj]]->getPE()->getIndexJ(), mapPairs[ind[jj]]->getPE()->getIndexT(), mapPairs[ind[jj]]->getNode()->get_ID());
                return true; 
              } 

            }
            Restore_timeslot(connected_node1[0]->get_Sched_Info()->get_Modulo_Current());
          } 
        }
        else if(connected && !connected1)
        {
          cout << "Inside !succ connected and pred connected" << endl;
          cout << "No. of succ nodes: " << (int) nodes_in_pred_slot.size() << endl;
          // we have a connection for pred and not for succ. Change the succ timeslot
          nodes_in_succ_slot.clear();
          nodes_in_succ_slot = get_Nodes_from_current_slot(connected_node2[0]->get_Sched_Info()->get_Modulo_Current()); 

          for(int kk = 0; kk < (int) nodes_in_pred_slot.size(); kk++)
          {
            TEC.free_current_timeslot(connected_node2[0]->get_Sched_Info()->get_Modulo_Current(), SizeX);
            bool success1 = Remap(nodes_in_succ_slot);

            if(success1)
            {
              bool connected2 = false, connected3 = false;
              switch(pred_size)
              {
                case 1:
                  {
                    if(connectedPEs(in->getPE(), mapPairs[ind[jj]]->getPE()))
                      connected2 = true;
                    break;
                  }
                case 2:
                  {
                    if(connectedPEs(in->getPE(), mapPairs[ind[jj]]->getPE()) && connectedPEs(in1->getPE(), mapPairs[ind[jj]]->getPE()))      
                      connected2 = true;

                    break;

                  }
                case 3:
                  {
                    if(connectedPEs(in->getPE(), mapPairs[ind[jj]]->getPE()) && connectedPEs(in1->getPE(), mapPairs[ind[jj]]->getPE()) &&  connectedPEs(in2->getPE(), mapPairs[ind[jj]]->getPE()))
                      connected2 = true;

                    break;

                  }
                default:
                  connected2 = false;
              }
              switch(succ_size)
              {
                case 1:
                  {
                    if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()))
                      connected3 = true;
                    break;
                  }
                case 2:
                  {
                    if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()))
                      connected3 = true;
                    break;
                  }
                case 3:
                  {
                    if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()))
                      connected3 = true;
                    break;
                  }
                case 4:
                  {
                    if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out3->getPE()))
                      connected3 = true;
                    break;
                  }
                case 5:
                  {
                    if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out3->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out4->getPE()))
                      connected3 = true;
                    break;
                  }
                default:
                  connected3 = false;
              } 

              if(connected2 && connected3)
              {
                MappingPair* mappair = mapPairs[ind[jj]]; 
                mapPairs[ind[jj]]->getNode()->SetCurrentPosition(mappair); 
                AssignTEC(mapPairs[ind[jj]]->getPE()->getIndexI(), mapPairs[ind[jj]]->getPE()->getIndexJ(), mapPairs[ind[jj]]->getPE()->getIndexT(), mapPairs[ind[jj]]->getNode()->get_ID());
                return true; 
              } 

            }
            Restore_timeslot(connected_node2[0]->get_Sched_Info()->get_Modulo_Current());
          } 
        }
        else
        {
          cout << "Inside !pred connected and !succ connected" << endl;
          // we do not have connection for both pred and succ. Change both timeslots.
          nodes_in_pred_slot.clear();
          nodes_in_pred_slot = get_Nodes_from_current_slot(connected_node1[0]->get_Sched_Info()->get_Modulo_Current()); 
          cout << "no. of pred nodes: " << (int) nodes_in_pred_slot.size() << endl;

          for(int kk = 0; kk < (int) nodes_in_pred_slot.size(); kk++)
          {
            TEC.free_current_timeslot(connected_node1[0]->get_Sched_Info()->get_Modulo_Current(), SizeX);
            bool success1 = Remap(nodes_in_pred_slot);

            if(success1)
            {
              nodes_in_succ_slot.clear();
              nodes_in_succ_slot = get_Nodes_from_current_slot(connected_node2[0]->get_Sched_Info()->get_Modulo_Current());
              cout << "no. of succ nodes: " << (int) nodes_in_succ_slot.size() << endl;
              for(int ll = 0; ll < (int) nodes_in_succ_slot.size(); ll++)
              {
                TEC.free_current_timeslot(connected_node2[0]->get_Sched_Info()->get_Modulo_Current(), SizeX);
                bool success2 = Remap(nodes_in_succ_slot);
                //TEC.print_matrix(); 
                //exit(1);
                if(success2)
                {
                  bool connected4 = false, connected5 = false;
                  switch(pred_size)
                  {
                    case 1:
                      {
                        if(connectedPEs(in->getPE(), mapPairs[ind[jj]]->getPE()))
                          connected4 = true;
                        break;
                      }
                    case 2:
                      {
                        if(connectedPEs(in->getPE(), mapPairs[ind[jj]]->getPE()) && connectedPEs(in1->getPE(), mapPairs[ind[jj]]->getPE()))      
                          connected4 = true;

                        break;

                      }
                    case 3:
                      {
                        if(connectedPEs(in->getPE(), mapPairs[ind[jj]]->getPE()) && connectedPEs(in1->getPE(), mapPairs[ind[jj]]->getPE()) &&  connectedPEs(in2->getPE(), mapPairs[ind[jj]]->getPE()))
                          connected4 = true;

                        break;

                      }
                    default:
                      connected4 = false;
                  }
                  switch(succ_size)
                  {
                    case 1:
                      {
                        if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()))
                          connected5 = true;
                        break;
                      }
                    case 2:
                      {
                        if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()))
                          connected5 = true;
                        break;
                      }
                    case 3:
                      {
                        if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()))
                          connected5 = true;
                        break;
                      }
                    case 4:
                      {
                        if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out3->getPE()))
                          connected5 = true;
                        break;
                      }
                    case 5:
                      {
                        if(connectedPEs(mapPairs[ind[jj]]->getPE(), out->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out1->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out2->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out3->getPE()) && connectedPEs(mapPairs[ind[jj]]->getPE(), out4->getPE()))
                          connected5 = true;
                        break;
                      }
                    default:
                      connected5 = false;
                  } 

                  if(connected4 && connected5)
                  {
                    MappingPair* mappair = mapPairs[ind[jj]]; 
                    mapPairs[ind[jj]]->getNode()->SetCurrentPosition(mappair); 
                    AssignTEC(mapPairs[ind[jj]]->getPE()->getIndexI(), mapPairs[ind[jj]]->getPE()->getIndexJ(), mapPairs[ind[jj]]->getPE()->getIndexT(), mapPairs[ind[jj]]->getNode()->get_ID());
                    return true; 
                  } 
                }
                Restore_timeslot(connected_node2[0]->get_Sched_Info()->get_Modulo_Current());
              }

            }
            Restore_timeslot(connected_node1[0]->get_Sched_Info()->get_Modulo_Current());
          } 
        }
      } 
    }
    Restore_timeslot(n->get_Sched_Info()->get_Modulo_Current()); 
  }

  // If nothing works out restore TEC to original
  Restore(copy_TEC);  
  return false;  

} 
*/


// The pred and succ vectors are mapped nodes and not the absolute predecessor and successors of v
// based on the  map case diagnoize the failure, like
//  a. if the children are mapped with no connecting PEs
//  b. if the children are mapped with connecting PEs but that connecting PE is occupied. 
//Based on this diagnosis we can call the recovery function. 

bool CGRA::Map_Failure_Diagnostic(Node *v, vector<Node*> pred, vector<Node*> succ, int map_case)
{

  // v is the unmappable node:

  // if map_case == 0 then the failure is due to no free space in the time slot where v can go. 
  //  check if we can remap the other nodes based on the avaialbility of the resources and their mappable vector.
  //  this case should not happen. 

  // if map_case == 1 since reverse BFS this should be mostly due to children and v connected PEs are occupied.
  // check if we can remap some operations in v's timeslot if nothing, then go to child's timeslot and check for remap
  // of child first and if not then check for remap of other operations to get a map for child and then for v. 

  // if map_case = 8 since reverse BFS this should be mostly due to 2 children and v have connected PE occupied, or
  // 2 children are mapped in different PEs so that they do not have a connected PE. We need to diagonize which of these.
  // if it is case 1, then we need to remap the current timeslot nodes to find a free connected PE. If not, remap children
  // to find a free PE or remap children's timeslot to find a free connected PE. 
  // if it case 2, then we need to  take one succ at a time and find other PE slots to find a free connected PE. 
  // On a failure we can go to the next succ and do the same. 

  // for map_cases 12, 16 and 20 are just variants of map_case 8. There are two routines that need to be added post diagostic.
  // 1. if there are connected PEs and not free. 
  // 2. if the succ are not mapped with the connected PEs. 

  switch(map_case)
  {
    case 0: 
      {
        bool success = Smart_Map_Failure_Recovery_Shallow_N(v, map_case);

        if(!success)
        {
          cout << "node shallow 0 not success" << endl;
          //_FATAL("shallow 4 not success"); 
          success = Smart_Map_Failure_Recovery_1Deep(v, map_case) ;
          //TEC.print_matrix(); 
          // _FATAL("1 Deep 0\n"); 
        }

        if(success)
          return success;
        else
        {
          cout << "map case 0" << endl;
          left_nodes_unmapped.push_back(v);
          cout << "passed left nodes" << endl;
          return false;
          _FATAL("in else of case 0:\n");
          // parental smart recovery and if that fails, children smart recovery. 
        }

        //TEC.print_matrix();
        cout << "map case 0" << endl;
        _FATAL("map case 0\n"); 
        break;
      }
    case 1: 
      {
        // TEC.print_matrix();
        cout << "Inside diagnostic case 1: " << endl;
        TEC.print_matrix();
        bool success = Smart_Map_Recovery_Node_Shallow(v, map_case);
        bool success1 = false; 
        if(!success)
        {
          // _FATAL("node shallow 4 not success");
          success = Smart_Map_Failure_Recovery_Shallow_N(v, map_case);
        }
        if(success)
        {
          cout << "success case 1 shallow!!!!" << endl;
          TEC.print_matrix();
        }

        if(!success)
        {
          cout << "node shallow 1 not success" << endl;
          //_FATAL("shallow 4 not success"); 
          success1 = Smart_Map_Failure_Recovery_1Deep(v, map_case);
          TEC.print_matrix();
          //_FATAL("1 Deep case 1\n");
        }

        //if(success1)
        // {
        //TEC.print_matrix();
        // _FATAL("1 Deep success case 1\n");
        // }

        if(success || success1) 
          return success;

        else
        {
          cout << "map case 1" << endl;
          left_nodes_unmapped.push_back(v);
          cout << "passed left nodes" << endl;
          //TEC.print_matrix(); 
          // _FATAL("in else of case 1:\n");
          return false;
          _FATAL("in else of case 1:\n");
          // parental smart recovery and if that fails, children smart recovery. 
        }


        _FATAL("map case 1\n");
        break;
      }
    case 2:
      {
        cout << "Inside diagnostic case 2: " << endl;
        TEC.print_matrix();
        bool success = Smart_Map_Recovery_Node_Shallow(v, map_case);
        bool success1 = false; 
        if(!success)
        {
          // _FATAL("node shallow 4 not success");
          success = Smart_Map_Failure_Recovery_Shallow_N(v, map_case);
        }
        if(success)
        {
          cout << "success case 2 shallow!!!!" << endl;
          TEC.print_matrix();
        }


        if(!success)
        {
          cout << "node shallow 2 not success" << endl;
          //_FATAL("shallow 4 not success"); 
          success1 = Smart_Map_Failure_Recovery_1Deep(v, map_case) ;
          TEC.print_matrix();
          //_FATAL("1 Deep case 2\n");
        }
        // if(success1)
        // {
        //   TEC.print_matrix();
        //   _FATAL("1 Deep success case 2\n");
        // }



        if(success || success1)
        {
          return success;
        }
        else
        {
          cout << "map case 2" << endl;
          left_nodes_unmapped.push_back(v);
          cout << "passed left nodes" << endl;
          //TEC.print_matrix(); 
          //_FATAL("in else of case 2:\n");
          return false;
          _FATAL("in else of case 2:\n");
          // parental smart recovery and if that fails, children smart recovery. 
        }

        //TEC.print_matrix();
        cout << "map case 2" << endl;
        _FATAL("map case 2\n");
        break;
      }
    case 3:
      {
        cout << "Inside diagnostic case 3: " << endl;
        TEC.print_matrix();
        bool success = Smart_Map_Recovery_Node_Shallow(v, map_case);
        bool success1 = false; 
        if(!success)
        {
          // _FATAL("node shallow 4 not success");
          success = Smart_Map_Failure_Recovery_Shallow_N(v, map_case);
        }
        if(success)
        {
          cout << "success case 3 shallow!!!!" << endl;
          TEC.print_matrix();
        }

        if(!success)
        {
          cout << "node shallow 3 not success" << endl;
          //_FATAL("shallow 4 not success"); 
          success1 = Smart_Map_Failure_Recovery_1Deep(v, map_case) ;
          TEC.print_matrix();
          //_FATAL("1 Deep case 3\n");
        }
        //if(success1)
        // {
        //   TEC.print_matrix();
        //   _FATAL("1 Deep success case 3\n");
        // }



        if(success || success1)
        {
          return success;
        }
        else
        {
          cout << "map case 3" << endl;
          left_nodes_unmapped.push_back(v);
          cout << "passed left nodes" << endl;
          //TEC.print_matrix(); 
          //_FATAL("in else of case 2:\n");
          return false;
          _FATAL("in else of case 3:\n");
          // parental smart recovery and if that fails, children smart recovery. 
        }
        _FATAL("map case 3\n");
        break;
      }
    case 4:
      {
        //TEC.print_matrix(); 
        cout << "Inside diagnostic case 4: " << endl;
        TEC.print_matrix();
        bool success = Smart_Map_Recovery_Node_Shallow(v, map_case);
        bool success1  = false;
        if(!success)
        { 
          // _FATAL("node shallow 4 not success");
          success = Smart_Map_Failure_Recovery_Shallow_N(v, map_case);
        }
        if(success)
        {
          cout << "success case 4 shallow!!!!" << endl;
          TEC.print_matrix();
        }

        if(!success)
        {
          cout << "node shallow 4 not success" << endl;
          //_FATAL("shallow 4 not success"); 
          success1 = Smart_Map_Failure_Recovery_1Deep(v, map_case) ;   
          TEC.print_matrix();
          //_FATAL("1 Deep case 4\n");
        }
        // if(success1)
        // {
        //   TEC.print_matrix();
        //   _FATAL("1 Deep success case 4\n");
        // }



        if(success || success1)
        {
          return success;
        }
        else
        {
          cout << "map case 4" << endl;
          left_nodes_unmapped.push_back(v);
          cout << "passed left nodes" << endl;
          //TEC.print_matrix();
          //_FATAL("in else of case 4:\n");
          return false;
          _FATAL("in else of case 4:\n");
          // parental smart recovery and if that fails, children smart recovery. 
        }
        TEC.print_matrix();
        cout << "map case 4" << endl;
        _FATAL("map case 4\n");
        break;
      }
    case 5:
      {
        //TEC.print_matrix(); 
        cout << "Inside map case 5 diagnostic" << endl;
        TEC.print_matrix(); 

        bool success = Smart_Map_Recovery_Node_Shallow(v, map_case);
        bool success1 = false; 

        if(!success)
        {
          cout << "node shallow 5 not success" << endl;
          //    _FATAL("node shallow 5 not success");
          success = Smart_Map_Failure_Recovery_Shallow_N(v, map_case);
        }

        if(success)
        {
          cout << "success case 5 shallow!!!!" << endl;
          TEC.print_matrix();
        }


        if(!success)
        {
          cout << "case 5 shallow N not success" << endl;
          //_FATAL("Remap failure in 5"); 
          success1 = Smart_Map_Failure_Recovery_1Deep(v, map_case); 
          TEC.print_matrix();
          //_FATAL("1 Deep case 5\n");
        }
        // if(success1)
        // {
        //   TEC.print_matrix();
        //   _FATAL("1 Deep success case 5\n");
        // }



        if(success || success1)
        {
          return success; 
        }
        else
        {
          cout << "map case 5 failure\n";
          // _FATAL("Map case 5 failure\n"); 
          left_nodes_unmapped.push_back(v);
          cout << "passed left nodes" << endl;
          //TEC.print_matrix();
          //_FATAL("Map case 5 failure\n");
          return false;
        }
        _FATAL("map case 5 failure\n");

        break;
      }
    case 6:
      {
        cout << "Inside map case 6 diagnostic" << endl;
        TEC.print_matrix(); 

        bool success = Smart_Map_Recovery_Node_Shallow(v, map_case);
        bool success1 = false; 

        if(!success)
        {
          cout << "node shallow 6 not success" << endl;
          //_FATAL("node shallow 5 not success");
          success = Smart_Map_Failure_Recovery_Shallow_N(v, map_case);
        }
        if(success)
        {
          cout << "success case 6 shallow!!!!" << endl;
          TEC.print_matrix();
        }


        if(!success)
        {
          cout << "case 6 shallow N not success" << endl;
          //_FATAL("Remap failure in 5"); 
          success1 = Smart_Map_Failure_Recovery_1Deep(v, map_case);
          TEC.print_matrix();
          //_FATAL("1 Deep case 6\n");
        }
        // if(success1)
        // {
        //   TEC.print_matrix();
        //   _FATAL("1 Deep success case 6\n");
        // }



        if(success || success1)
        {
          return success;
        }
        else
        {
          cout << "map case 6 failure\n";
          // _FATAL("Map case 5 failure\n"); 
          left_nodes_unmapped.push_back(v);
          cout << "passed left nodes" << endl;
          return false;
        }
        _FATAL("map case 6\n");
        break;
      }
    case 7:
      {
        cout << "Inside map case 7 diagnostic" << endl;
        TEC.print_matrix();
        bool success = Smart_Map_Recovery_Node_Shallow(v, map_case);
        bool success1 = false; 

        if(!success)
        {
          cout << "node shallow 7 not success" << endl;
          //_FATAL("node shallow 5 not success");
          success = Smart_Map_Failure_Recovery_Shallow_N(v, map_case);
        }
        if(success)
        {
          cout << "success case 7 shallow!!!!" << endl;
          TEC.print_matrix();
        }


        if(!success)
        {
          cout << "case 7 shallow N not success" << endl;
          //_FATAL("Remap failure in 5"); 
          success1 = Smart_Map_Failure_Recovery_1Deep(v, map_case);
          TEC.print_matrix();
          //_FATAL("1 Deep case 7\n");
        }
        //if(success1)
        // {
        //   TEC.print_matrix();
        //   _FATAL("1 Deep success case 7\n");
        // }



        if(success || success1)
        {
          return success;
        }
        else
        {
          cout << "map case 7 failure\n";
          // _FATAL("Map case 5 failure\n"); 
          left_nodes_unmapped.push_back(v);
          cout << "passed left nodes" << endl;
          return false;
        }

        //_FATAL("map case 7\n");
        break;
      }
    case 8:
      {
        cout << "Inside map case 8 diagnostic" << endl;
        TEC.print_matrix();


        bool success = Smart_Map_Recovery_Node_Shallow(v, map_case);
        bool success1 = false;
        if(!success)
        {
          cout << "node shallow 8 not success" << endl; 
          //_FATAL("node shallow 8 not success");
          success = Smart_Map_Failure_Recovery_Shallow_N(v, map_case);
        }
        //if(success)
        // {
        //   cout << "success case 8 shallow!!!!" << endl;
        //   TEC.print_matrix();
        // }


        if(!success)
        {
          //_FATAL("map case 8\n");
          cout << "case 8 shallow N not success" << endl;
          success1 = Smart_Map_Failure_Recovery_1Deep(v, map_case);
          TEC.print_matrix();
          //_FATAL("1 Deep case 8\n");
        }

        if(success || success1)
        {
          return success;
        }
        else
        {
          cout << "map case 8 failure\n";
          left_nodes_unmapped.push_back(v);
          //cout << "passed left nodes" << endl;
          //  TEC.print_matrix();
          //_FATAL("map case 8 all failed"); 
          cout << "map case 8" << endl;
          return false;
        }

        TEC.print_matrix();
        cout << "map case 8" << endl;
        _FATAL("map case 8\n");
        break;
      }
    case 9:
      {
        cout << "Inside map case 9 diagnostic" << endl;
        TEC.print_matrix();
        bool success = Smart_Map_Recovery_Node_Shallow(v, map_case);
        bool success1 = false;
        if(!success)
        {
          cout << "node shallow 9 not success" << endl;
          //_FATAL("node shallow 9 not success");
          success = Smart_Map_Failure_Recovery_Shallow_N(v, map_case);
        }
        if(success)
        {
          cout << "success case 9 shallow!!!!" << endl;
          TEC.print_matrix();

        }


        if(!success)
        {
          cout << "case 9 shallow N not success" << endl;
          //_FATAL("Shallow failure in 9"); 
          success1 = Smart_Map_Failure_Recovery_1Deep(v, map_case);
          TEC.print_matrix();
          // _FATAL("1 Deep case 9\n");
        }
        // if(success1)
        // {
        //   TEC.print_matrix();
        //   _FATAL("1 Deep success case 9\n");
        //}



        if(success || success1)
        {
          return success;
        }
        else
        {
          cout << "map case 9 failure\n";
          left_nodes_unmapped.push_back(v);
          cout << "passed left nodes" << endl;
          return false;
        }


        TEC.print_matrix();
        cout << "map case 9" << endl;
        // _FATAL("map case 9\n");
        break;
      }
    case 10:
      {
        cout << "Inside map case 10 diagnostic" << endl;
        TEC.print_matrix();
        left_nodes_unmapped.push_back(v);
        /*bool success = Smart_Map_Failure_Recovery_Shallow_N(pred, succ, v);

          if(!success)
          success = Smart_Map_Failure_Recovery_1Deep_N(pred, succ, v);

          if(success)
          return success;
          else
          {
          cout << "map case 8 failure\n";
          left_nodes_unmapped.push_back(v);
          return false;
          }
          cout << "map case 10" << endl;*/
        //_FATAL("map case 10\n");
        return false;
        break;
      }
    case 11:
      {
        TEC.print_matrix();
        cout << "map case 11" << endl;
        left_nodes_unmapped.push_back(v);
        //_FATAL("map case 11\n");
        return false; 
        break;
      }
    case 12:
      {
        cout << "Inside map case 12 diagnostic" << endl;
        TEC.print_matrix();
        bool success = Smart_Map_Recovery_Node_Shallow(v, map_case);
        bool success1 = false; 

        if(!success)
        {
          cout << "node shallow 12 not success" << endl;
          //_FATAL("node shallow 5 not success");
          success = Smart_Map_Failure_Recovery_Shallow_N(v, map_case);
        }
        if(success)
        {
          cout << "success case 12 shallow!!!!" << endl;
          TEC.print_matrix();
        }


        if(!success)
        {
          cout << "case 12 shallow N not success" << endl;
          //_FATAL("Remap failure in 5"); 
          success1 = Smart_Map_Failure_Recovery_1Deep(v, map_case);
          TEC.print_matrix();
          //_FATAL("1 Deep case 12\n");
        }
        // if(success1)
        // {
        //   TEC.print_matrix();
        //   _FATAL("1 Deep success case 12\n");
        // }



        if(success || success1)
        {
          return success;
        }
        else
        {
          cout << "map case 12 failure\n";
          // _FATAL("Map case 5 failure\n"); 
          left_nodes_unmapped.push_back(v);
          cout << "passed left nodes" << endl;
          return false;
        }
        break;
      }
    case 13:
      {
        //TEC.print_matrix();
        //cout << "map case 13 for: " << v->get_ID() << endl;
        cout << "Inside map case 13 diagnostic" << endl;
        TEC.print_matrix();
        bool success = Smart_Map_Recovery_Node_Shallow(v, map_case);
        bool success1 = false; 

        if(!success)
        {
          cout << "node shallow 13 not success" << endl;
          //  //_FATAL("node shallow 5 not success");
          success = Smart_Map_Failure_Recovery_Shallow_N(v, map_case);
        }
        // if(success)
        //   {
        //     cout << "success case 13 shallow!!!!" << endl;
        //     TEC.print_matrix();
        //   }


        if(!success)
        {
          cout << "case 13 shallow N not success" << endl;
          //_FATAL("Remap failure in 5"); 
          success1 = Smart_Map_Failure_Recovery_1Deep(v, map_case);
          TEC.print_matrix();
          //_FATAL("1 Deep case 13\n");
        }
        //  if(success1)
        // {
        //   TEC.print_matrix();
        //   _FATAL("1 Deep success case 13\n");
        // }



        if(success || success1)
        {
          return success;
        }
        else
        {
          cout << "map case 13 failure\n";
          // _FATAL("Map case 5 failure\n"); 
          left_nodes_unmapped.push_back(v);
          cout << "passed left nodes" << endl;
          return false;
        }

        //_FATAL("map case 13\n");
        break;
      }
    case 14:
      {
        TEC.print_matrix();
        cout << "map case 14" << endl;
        left_nodes_unmapped.push_back(v);
        //_FATAL("map case 14\n");
        return false;
        break;
      }
    case 15:
      {
        TEC.print_matrix();
        cout << "map case 15" << endl;
        left_nodes_unmapped.push_back(v);
        //_FATAL("map case 15\n");
        return false;
        break;
      }
    case 16:
      {

        TEC.print_matrix();
        cout << "map case 16" << endl;
        left_nodes_unmapped.push_back(v);
        //_FATAL("map case 16\n");
        return false;
        break;
      }
    case 17:
      {

        TEC.print_matrix();
        cout << "map case 17" << endl;
        left_nodes_unmapped.push_back(v);
        // _FATAL("map case 17\n");
        return false;
        break;
      }
    case 18:
      {
        TEC.print_matrix();
        cout << "map case 18" << endl;
        left_nodes_unmapped.push_back(v);
        // _FATAL("map case 18\n");
        return false;
        break;
      }
    case 19:
      {

        TEC.print_matrix();
        cout << "map case 19" << endl;
        left_nodes_unmapped.push_back(v);
        //_FATAL("map case 19\n");
        return false;
        break;
      }
    case 20:
      {

        TEC.print_matrix();
        cout << "map case 20" << endl;
        left_nodes_unmapped.push_back(v);
        //_FATAL("map case 20\n");
        return false;
        break;
      }
    case 21:
      {
        TEC.print_matrix();
        cout << "map case 21" << endl;
        left_nodes_unmapped.push_back(v);
        //_FATAL("map case 21\n");
        return false;
        break;
      }
    case 22:
      {

        TEC.print_matrix();
        cout << "map case 22" << endl;
        left_nodes_unmapped.push_back(v);
        //_FATAL("map case 22\n");
        return false;
        break;
      }
    case 23:
      {
        TEC.print_matrix();
        cout << "map case 23" << endl;
        left_nodes_unmapped.push_back(v);
        //_FATAL("map case 23\n");
        return false;
        break;
      }   

    default:
      {   

        TEC.print_matrix();
        cout << "map case default" << endl;
        _FATAL("default\n");
      }     

  }

  return true; 
}

tuple<vector<MappingPair*>, vector<int>, int> CGRA::Get_Free_Coordinates(Node* v, vector<Node*> pred, vector<Node*> succ, vector<int> indices)
{
  int map_case = -1; 
  vector<MappingPair*> retVal;


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
            //bool found_mem=false;
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
        retVal.clear(); 
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case); 
      } 

    case 1: 
      {
        //int mp = get_mapped_mappairs(pred[0]);
        MappingPair* mp = pred[0]->GetCurrentPosition();
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
            //bool found_mem=false;
            if(((int) mapped_nodes.size()) > 0)
            {
              for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
              {
                if(get_Node(mapped_nodes[jj])->is_Load_Address_Generator() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                  break;
                else
                {
                  if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()))
                    potential_pos.push_back(indices[ii]);
                }
              }
            }
            else
            {
              if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()))
                potential_pos.push_back(indices[ii]);
            }

          }
        }
        else if(v->is_Load_Data_Bus_Read())
        {
          int row=-1;
          if(pred[0]->is_Load_Address_Generator())
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
                  if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()))
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
                if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()))
                  potential_pos.push_back(indices[ii]);
              }
              else
              {
                if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                  potential_pos.push_back(indices[ii]);
              }
            }
          }
        }
        else if(v->is_Store_Address_Generator())
        {  
          vector<int> mapped_nodes;

          mapped_nodes.clear();

          for(int ii=0; ii<(int) indices.size(); ii++)
          {
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());
            for(int jj=0; jj <(int) mapped_nodes.size(); jj++)
            {
              if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()))
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
              if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()))
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

            if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()))
              potential_pos.push_back(indices[ii]);
          }
        }

        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);  
      }

    case 2: 
      {
        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* mp1 = pred[1]->GetCurrentPosition();
        vector<int> potential_pos;
        if(v->is_Store_Data_Bus_Write())
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
          else if(pred[1]->is_Store_Address_Generator())
          {
            int row = -1;
            row = TEC.row_find(pred[1]->get_ID()) % SizeX;

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
              if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp1->getPE(), mapPairs[indices[ii]]->getPE()))
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

            if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp1->getPE(), mapPairs[indices[ii]]->getPE()))
              potential_pos.push_back(indices[ii]);
          }
        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);

        _FATAL("2 pred mapped with 0 succs\n"); 
        return make_tuple(retVal, potential_pos, map_case); 
      }  
    case 3:
      {
        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* mp1 = pred[1]->GetCurrentPosition();
        MappingPair* mp2 = pred[2]->GetCurrentPosition();      
        vector<int> potential_pos;

        for(int ii=0; ii < (int) indices.size(); ii++)
        {
          // check if the PE is free, if not continue to the next position.
          if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
            continue;

          if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp1->getPE(), mapPairs[indices[ii]]->getPE()) && (connectedPEs(mp2->getPE(), mapPairs[indices[ii]]->getPE())))
            potential_pos.push_back(indices[ii]);
        }
        retVal.clear();
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);
        _FATAL("3 pred mapped with 0 succs\n");
        return make_tuple(retVal, potential_pos, map_case);
      }
    case 4:
      {
        //cout << "inside mapping case 4 for node: " << v->get_ID() << "  succ: " << succ[0]->get_ID() << endl; 
        //int mp = get_mapped_mappairs(succ[0]);
        MappingPair* mp = succ[0]->GetCurrentPosition();
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
            //bool found_mem=false;
            if(((int) mapped_nodes.size()) > 0)
            {
              for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
              {
                if(get_Node(mapped_nodes[jj])->is_Load_Address_Generator() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                  break;
                else
                {
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(), mp->getPE()))
                    potential_pos.push_back(indices[ii]);
                }
              }
            }
            else
            {
              if(connectedPEs(mapPairs[indices[ii]]->getPE(), mp->getPE()))
                potential_pos.push_back(indices[ii]);
            }

          }
        }
        /*else if(v->is_Load_Data_Bus_Read())
          {
          int row=-1;
          row = TEC.row_find(succ[0]->get_ID()) % SizeX; 

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
        if(connectedPEs(mapPairs[indices[ii]]->getPE(), mp->getPE()))
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
        if(connectedPEs(mapPairs[indices[ii]]->getPE(), mp->getPE()))
        potential_pos.push_back(indices[ii]);
        }
        else
        {
        if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
        potential_pos.push_back(indices[ii]);
        }
        }
        }
        }*/
        else if(v->is_Store_Address_Generator())
        {  
          vector<int> mapped_nodes;

          mapped_nodes.clear();

          for(int ii=0; ii<(int) indices.size(); ii++)
          {
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());
            for(int jj=0; jj <(int) mapped_nodes.size(); jj++)
            {
              if(connectedPEs(mapPairs[indices[ii]]->getPE(), mp->getPE()))
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
          if(succ[0]->is_Store_Address_Generator())
          {
            int row = -1;
            row = TEC.row_find(succ[0]->get_ID()) % SizeX;

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
              if(connectedPEs(mapPairs[indices[ii]]->getPE(),mp->getPE()))
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

            if(connectedPEs(mapPairs[indices[ii]]->getPE(), mp->getPE()))
              potential_pos.push_back(indices[ii]);
          }
        }

        /*if((int) potential_pos.size() == 0)
          {
          TEC.print_matrix(); 
          _FATAL("Hey quit from 4 for node: %d with succ %d", v->get_ID(), succ[0]->get_ID()); 
          }*/ 
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);  

      }
    case 5:
      {
        //int out_mp = get_mapped_mappairs(succ[0]);
        //int mp = get_mapped_mappairs(pred[0]);

        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* out_mp = succ[0]->GetCurrentPosition();

        vector<int> potential_pos; 
        if(v->is_Load_Address_Generator())
        {

          int row = TEC.row_find(succ[0]->get_ID()) % SizeX;


          vector<int> mapped_nodes;
          for(int ii=0; ii<(int) indices.size(); ii++)
          {
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;


            mapped_nodes.clear();
            mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());
            // check the row_idx to find the row in this cycle that has been occupied, and select a different row.

            bool found_mem=false;

            if(mapped_nodes.size() > 0)
              for(int jj=0; jj < nodeSize; jj++)
                for(int kk=0; kk < (int) mapped_nodes.size(); kk++)
                  if(_node_Set[jj]->get_ID() == mapped_nodes[kk])
                    if(_node_Set[jj]->is_Store_Address_Generator() || _node_Set[jj]->is_Load_Address_Generator())
                      found_mem=true;

            if(!found_mem)
            {
              if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()))
                if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                  potential_pos.push_back(indices[ii]);
            }
          }
        }
        else if(v->is_Load_Data_Bus_Read())
        {
          vector<int> mapped_nodes;
          int row=-1;
          if(pred[0]->is_Load_Address_Generator())
            row = TEC.row_find(pred[0]->get_ID()) % SizeX;

          for(int ii=0; ii < (int) indices.size(); ii++)
          {
            // check if the PE is free, if not continue to the next position.
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            if((int) mapped_nodes.size() > 0)
            {
              for(int jj=0; jj <(int) mapped_nodes.size(); jj++)
              {
                if(row==-1)
                {
                  if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()))
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

                    if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()))
                      potential_pos.push_back(indices[ii]);

                  }

                }

              }
            }
            else
            {
              if(row==-1)
              {
                if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()))
                  potential_pos.push_back(indices[ii]);
              }
              else
              {
                if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()))
                    potential_pos.push_back(indices[ii]);
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

            if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()))
              potential_pos.push_back(indices[ii]);
          }
        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);
      }
    case 6:
      {
        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* mp1 = pred[1]->GetCurrentPosition();
        MappingPair* out_mp = succ[0]->GetCurrentPosition();

        vector<int> potential_pos;
        for(int ii=0; ii < (int) indices.size(); ii++)
        {
          // check if the PE is free, if not continue to the next position.
          if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
            continue;

          if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp1->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()))
            potential_pos.push_back(indices[ii]);
        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);
        // _FATAL("2 pred mapped with 1 succs\n");
        // return make_tuple(retVal, potential_pos, map_case);
      }
    case 7:
      {
        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* mp1 = pred[1]->GetCurrentPosition();
        MappingPair* mp2 = pred[2]->GetCurrentPosition();
        MappingPair* out_mp = succ[0]->GetCurrentPosition();

        vector<int> potential_pos;
        for(int ii=0; ii < (int) indices.size(); ii++)
        {
          // check if the PE is free, if not continue to the next position.
          if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
            continue;

          if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp1->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp2->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()))
            potential_pos.push_back(indices[ii]);
        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);
        //_FATAL("3 pred mapped with 1 succs\n");
        //return make_tuple(retVal, potential_pos, map_case);
      }
    case 8:
      {
        //_FATAL("0 pred mapped with 2 succs\n");
        //int out_mp = get_mapped_mappairs(succ[0]);
        //int out_mp1 = get_mapped_mappairs(succ[1]);

        MappingPair* out_mp = succ[0]->GetCurrentPosition();
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();

        vector<int> potential_pos;  
        if(v->is_Load_Address_Generator()) 
        {
          int index=-1; 


          int row=-2; //int addr_node = 0; 
          for(int jj=0; jj < (int) succ.size(); jj++)
          {
            if(succ[jj]->is_Load_Data_Bus_Read())
            {
              row = TEC.row_find(succ[jj]->get_ID()) % SizeX;
              //addr_node = jj; 
            }
            else
              index = jj;
          }


          vector<int> mapped_nodes;
          for(int ii=0; ii<(int) indices.size(); ii++)
          {
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;


            mapped_nodes.clear();
            mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());
            // check the row_idx to find the row in this cycle that has been occupied, and select a different row.

            bool found_mem=false;

            if(mapped_nodes.size() > 0)
              for(int jj=0; jj < nodeSize; jj++)
                for(int kk=0; kk < (int) mapped_nodes.size(); kk++)
                  if(_node_Set[jj]->get_ID() == mapped_nodes[kk])
                    if(_node_Set[jj]->is_Store_Address_Generator() || _node_Set[jj]->is_Load_Address_Generator())
                      found_mem=true;

            if(!found_mem)
            {
              if(connectedPEs(mapPairs[indices[ii]]->getPE(), succ[index]->GetCurrentPosition()->getPE()))
                if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                  potential_pos.push_back(indices[ii]);
            }
          }
        }
        else if(v->is_Load_Data_Bus_Read())
        {
          vector<int> mapped_nodes;
          for(int ii=0; ii<(int) indices.size(); ii++)
          {
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            mapped_nodes.clear();
            mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());

            if((int)mapped_nodes.size() > 0)
            {
              for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
              {
                if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                  continue; 

                if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()))         
                  potential_pos.push_back(indices[ii]);
              }
            }
            else
            {
              if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()))
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

            if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()))
              potential_pos.push_back(indices[ii]);  
          }
        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);
      }
    case 9:
      {
        //int mp = get_mapped_mappairs(pred[0]);
        //int out_mp = get_mapped_mappairs(succ[0]);
        //int out_mp1 = get_mapped_mappairs(succ[1]);

        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* out_mp = succ[0]->GetCurrentPosition();
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();

        vector<int> potential_pos; 
        //_FATAL("1 pred mapped with 2 succs\n");
        if(v->is_Load_Address_Generator())
        {
          int row=-2, index=-1;

          for(int jj=0; jj < (int) succ.size(); jj++)
          {
            if(succ[0]->is_Load_Data_Bus_Read())
              row = TEC.row_find(succ[0]->get_ID()) % SizeX;
            else
              index = jj;
          }

          vector<int> mapped_nodes;
          for(int ii=0; ii<(int) indices.size(); ii++)
          {
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;


            mapped_nodes.clear();
            mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());
            // check the row_idx to find the row in this cycle that has been occupied, and select a different row.

            bool found_mem=false;

            if(mapped_nodes.size() > 0)
              for(int jj=0; jj < nodeSize; jj++)
                for(int kk=0; kk < (int) mapped_nodes.size(); kk++)
                  if(_node_Set[jj]->get_ID() == mapped_nodes[kk])
                    if(_node_Set[jj]->is_Store_Address_Generator() || _node_Set[jj]->is_Load_Address_Generator())
                      found_mem=true;

            if(!found_mem)
            {
              if(connectedPEs(mapPairs[indices[ii]]->getPE(), succ[index]->GetCurrentPosition()->getPE()) && connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()))
                if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                  potential_pos.push_back(indices[ii]);

            }
          }

        }
        else if(v->is_Load_Data_Bus_Read())
        {
          int row=-1;
          if(pred[0]->is_Load_Address_Generator())
            row = TEC.row_find(pred[0]->get_ID()) % SizeX;
          vector<int> mapped_nodes;

          for(int ii=0; ii < (int) indices.size(); ii++)
          {
            // check if the PE is free, if not continue to the next position.
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            mapped_nodes.clear();
            mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());


            if((int)mapped_nodes.size() > 0)
            {
              for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
              {
                if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                  continue;

                if(row == -1)
                {
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()))
                    potential_pos.push_back(indices[ii]);
                }
                else
                {
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()))
                    if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                      potential_pos.push_back(indices[ii]);
                }
              }
            }
            else
            {
              if(row == -1)
              {
                if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()))
                  potential_pos.push_back(indices[ii]);
              }
              else
              {

                if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()))
                  if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                    potential_pos.push_back(indices[ii]);
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

            if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()))
              potential_pos.push_back(indices[ii]);
          }
        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);
      }
    case 10:
      {
        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* mp1 = pred[1]->GetCurrentPosition();
        MappingPair* out_mp = succ[0]->GetCurrentPosition();
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();

        vector<int> potential_pos;
        for(int ii=0; ii < (int) indices.size(); ii++)
        {
          // check if the PE is free, if not continue to the next position.
          if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
            continue;

          if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp1->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()))
            potential_pos.push_back(indices[ii]);
        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);

        //_FATAL("2 pred mapped with 2 succs\n");
        //return make_tuple(retVal, potential_pos, map_case);
      }
    case 11:
      {
        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* mp1 = pred[1]->GetCurrentPosition();
        MappingPair* mp2 = pred[2]->GetCurrentPosition(); 
        MappingPair* out_mp = succ[0]->GetCurrentPosition();
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();

        vector<int> potential_pos;
        for(int ii=0; ii < (int) indices.size(); ii++)
        {
          // check if the PE is free, if not continue to the next position.
          if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
            continue;

          if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp1->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp2->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()))
            potential_pos.push_back(indices[ii]);
        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);
        //_FATAL("3 pred mapped with 2 succs\n");
        //return make_tuple(retVal, potential_pos, map_case);
      }
    case 12:
      {
        //_FATAL("0 pred mapped with 3 succs\n");
        //int out_mp = get_mapped_mappairs(succ[0]);
        //int out_mp1 = get_mapped_mappairs(succ[1]);
        //int out_mp2 = get_mapped_mappairs(succ[2]);  

        MappingPair* out_mp = succ[0]->GetCurrentPosition();   
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();
        MappingPair* out_mp2 = succ[2]->GetCurrentPosition();

        vector<int> potential_pos;  
        if(v->is_Load_Data_Bus_Read())
        {
          vector<int> mapped_nodes;
          for(int ii=0; ii<(int) indices.size(); ii++)
          {
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            mapped_nodes.clear();
            mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());

            if((int)mapped_nodes.size() > 0)
            {
              for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
              {
                if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                  continue;

                if(connectedPEs(out_mp->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp1->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp2->getPE(),mapPairs[indices[ii]]->getPE()))
                  potential_pos.push_back(indices[ii]);
              }
            }
            else
            {
              if(connectedPEs(out_mp->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp1->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp2->getPE(),mapPairs[indices[ii]]->getPE()))
                potential_pos.push_back(indices[ii]);
            }
          }

        }
        else
        {
          for(int ii=0; ii< (int) indices.size(); ii++)
          {
            // check if the PE is free, if not continue to the next position.
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()))
              potential_pos.push_back(indices[ii]);

          }
        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);
      }
    case 13:
      {
        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* out_mp = succ[0]->GetCurrentPosition();
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();
        MappingPair* out_mp2 = succ[2]->GetCurrentPosition();
        vector<int> potential_pos; 
        if(v->is_Load_Data_Bus_Read())
        {
          int row = -1;

          if(pred[0]->is_Load_Address_Generator())
            row = TEC.row_find(pred[0]->get_ID()) % SizeX; 
          vector<int> mapped_nodes;
          for(int ii=0; ii<(int) indices.size(); ii++)
          {
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            mapped_nodes.clear();
            mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());


            if((int)mapped_nodes.size() > 0)
            {
              for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
              {
                if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                  continue;

                if(row == -1)
                {
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()))
                    potential_pos.push_back(indices[ii]);
                }
                else
                {
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()))
                    if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                      potential_pos.push_back(indices[ii]);
                }
              }
            }
            else
            {
              if(row == -1)
              {
                if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()))
                  potential_pos.push_back(indices[ii]);
              }
              else
              {

                if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()))
                  if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                    potential_pos.push_back(indices[ii]);
              }
            }

          }

        }
        else 
        {
          for(int ii=0; ii< (int) indices.size(); ii++)
          {
            // check if the PE is free, if not continue to the next position.
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()))
              potential_pos.push_back(indices[ii]);

          }
        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);

        TEC.print_matrix(); 
        _FATAL("1 pred mapped with 3 succs for %d\n", v->get_ID());
        return make_tuple(retVal, potential_pos, map_case);
      }
    case 14:
      {
        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* mp1 = pred[1]->GetCurrentPosition();
        MappingPair* out_mp = succ[0]->GetCurrentPosition();
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();
        MappingPair* out_mp2 = succ[2]->GetCurrentPosition();
        vector<int> potential_pos;

        for(int ii=0; ii< (int) indices.size(); ii++)
        {
          // check if the PE is free, if not continue to the next position.
          if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
            continue;

          if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp1->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()))
            potential_pos.push_back(indices[ii]);

        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);

        _FATAL("2 pred mapped with 3 succs\n");
        return make_tuple(retVal, potential_pos, map_case);
      }
    case 15:
      {
        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* mp1 = pred[1]->GetCurrentPosition();
        MappingPair* mp2 = pred[2]->GetCurrentPosition();
        MappingPair* out_mp = succ[0]->GetCurrentPosition();
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();
        MappingPair* out_mp2 = succ[2]->GetCurrentPosition();
        vector<int> potential_pos;

        for(int ii=0; ii< (int) indices.size(); ii++)
        {
          // check if the PE is free, if not continue to the next position.
          if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
            continue;

          if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp1->getPE(), mapPairs[indices[ii]]->getPE())&& connectedPEs(mp2->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()))
            potential_pos.push_back(indices[ii]);

        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);
        _FATAL("3 pred mapped with 3 succs\n");
        return make_tuple(retVal, potential_pos, map_case);
      }
    case 16:
      {
        // _FATAL("0 pred mapped with 4 succs\n");
        //int out_mp = get_mapped_mappairs(succ[0]);
        //int out_mp1 = get_mapped_mappairs(succ[1]);
        //int out_mp2 = get_mapped_mappairs(succ[2]);
        //int out_mp3 = get_mapped_mappairs(succ[3]);

        MappingPair* out_mp = succ[0]->GetCurrentPosition();
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();
        MappingPair* out_mp2 = succ[2]->GetCurrentPosition();
        MappingPair* out_mp3 = succ[3]->GetCurrentPosition();

        vector<int> potential_pos; 
        if(v->is_Load_Data_Bus_Read())
        {

          vector<int> mapped_nodes;
          for(int ii=0; ii<(int) indices.size(); ii++)
          {
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            mapped_nodes.clear();
            mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());

            if((int)mapped_nodes.size() > 0)
            {
              for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
              {
                if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                  continue;

                if(connectedPEs(out_mp->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp1->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp2->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp3->getPE(),mapPairs[indices[ii]]->getPE()))
                  potential_pos.push_back(indices[ii]);
              }
            }
            else
            {
              if(connectedPEs(out_mp->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp1->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp2->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp3->getPE(),mapPairs[indices[ii]]->getPE()))
                potential_pos.push_back(indices[ii]);
            }

          } 
        }
        else
        {

          for(int ii=0; ii < (int) indices.size(); ii++)

          {
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            if(connectedPEs(out_mp->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp1->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp2->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp3->getPE(),mapPairs[indices[ii]]->getPE()))
              potential_pos.push_back(indices[ii]);
          }

        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);
        return make_tuple(retVal, potential_pos, map_case);
      }
    case 17:
      {

        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* out_mp = succ[0]->GetCurrentPosition();
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();
        MappingPair* out_mp2 = succ[2]->GetCurrentPosition();
        MappingPair* out_mp3 = succ[3]->GetCurrentPosition();
        vector<int> potential_pos;
        if(v->is_Load_Data_Bus_Read())
        {
          int row = -1; 
          if(pred[0]->is_Load_Address_Generator())
            row=TEC.row_find(pred[0]->get_ID()) % SizeX;
          vector<int> mapped_nodes;
          for(int ii=0; ii<(int) indices.size(); ii++)
          {
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            mapped_nodes.clear();
            mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());


            if((int)mapped_nodes.size() > 0)
            {
              for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
              {
                if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                  continue;

                if(row == -1)
                {
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp3->getPE()))
                    potential_pos.push_back(indices[ii]);
                }
                else
                {
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp3->getPE()))
                    if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                      potential_pos.push_back(indices[ii]);
                }
              }
            }
            else
            {
              if(row == -1)
              {
                if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp3->getPE()))
                  potential_pos.push_back(indices[ii]);
              }
              else
              {

                if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp3->getPE()))
                  if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                    potential_pos.push_back(indices[ii]);
              }
            }

          }
        }
        else
        {
          for(int ii=0; ii< (int) indices.size(); ii++)
          {
            // check if the PE is free, if not continue to the next position.
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp3->getPE()))
              potential_pos.push_back(indices[ii]);

          }
        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);
      }
    case 18:
      {

        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* mp1 = pred[1]->GetCurrentPosition();
        MappingPair* out_mp = succ[0]->GetCurrentPosition();
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();
        MappingPair* out_mp2 = succ[2]->GetCurrentPosition();
        MappingPair* out_mp3 = succ[3]->GetCurrentPosition();

        vector<int> potential_pos;
        for(int ii=0; ii< (int) indices.size(); ii++)
        {
          // check if the PE is free, if not continue to the next position.
          if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
            continue;

          if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp1->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp3->getPE()))
            potential_pos.push_back(indices[ii]);

        }  
        retVal.clear();
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);


        _FATAL("2 pred mapped with 4 succs\n");
        return make_tuple(retVal, potential_pos, map_case);
      }
    case 19:
      {
        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* mp1 = pred[1]->GetCurrentPosition();
        MappingPair* mp2 = pred[2]->GetCurrentPosition();
        MappingPair* out_mp = succ[0]->GetCurrentPosition();
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();
        MappingPair* out_mp2 = succ[2]->GetCurrentPosition();
        MappingPair* out_mp3 = succ[3]->GetCurrentPosition();

        vector<int> potential_pos;
        for(int ii=0; ii< (int) indices.size(); ii++)
        {
          // check if the PE is free, if not continue to the next position.
          if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
            continue;

          if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp1->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp2->getPE(), mapPairs[indices[ii]]->getPE())&& connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp3->getPE()))
            potential_pos.push_back(indices[ii]);

        }
        retVal.clear();
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);


        _FATAL("3 pred mapped with 4 succs\n");
        return make_tuple(retVal, potential_pos, map_case);
      }
    case 20:
      {
        // _FATAL("0 pred mapped with 5 succs\n");
        //int out_mp = get_mapped_mappairs(succ[0]);
        //int out_mp1 = get_mapped_mappairs(succ[1]);
        //int out_mp2 = get_mapped_mappairs(succ[2]);
        //int out_mp3 = get_mapped_mappairs(succ[3]);
        //int out_mp4 = get_mapped_mappairs(succ[4]);

        MappingPair* out_mp = succ[0]->GetCurrentPosition();
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();
        MappingPair* out_mp2 = succ[2]->GetCurrentPosition();
        MappingPair* out_mp3 = succ[3]->GetCurrentPosition();
        MappingPair* out_mp4 = succ[4]->GetCurrentPosition();

        vector<int> potential_pos; 
        if(v->is_Load_Data_Bus_Read())
        {
          vector<int> mapped_nodes;
          for(int ii=0; ii<(int) indices.size(); ii++)
          {
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            mapped_nodes.clear();
            mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());


            if((int)mapped_nodes.size() > 0)
            {
              for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
              {
                if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                  continue;

                if(connectedPEs(out_mp->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp1->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp2->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp3->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp4->getPE(),mapPairs[indices[ii]]->getPE()))
                  potential_pos.push_back(indices[ii]);
              }
            }
            else
            {
              if(connectedPEs(out_mp->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp1->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp2->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp3->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp4->getPE(),mapPairs[indices[ii]]->getPE()))
                potential_pos.push_back(indices[ii]);
            }

          }

        }
        else
        {
          for(int ii=0; ii < (int) indices.size(); ii++)
          {
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            if(connectedPEs(out_mp->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp1->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp2->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp3->getPE(),mapPairs[indices[ii]]->getPE()) && connectedPEs(out_mp4->getPE(),mapPairs[indices[ii]]->getPE()))
              potential_pos.push_back(indices[ii]);

          }
        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);
      }
    case 21:
      {
        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* out_mp = succ[0]->GetCurrentPosition();
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();
        MappingPair* out_mp2 = succ[2]->GetCurrentPosition();
        MappingPair* out_mp3 = succ[3]->GetCurrentPosition();
        MappingPair* out_mp4 = succ[4]->GetCurrentPosition();
        vector<int> potential_pos;
        if(v->is_Load_Data_Bus_Read())
        {
          int row = -1;
          if(pred[0]->is_Load_Address_Generator())
            row=TEC.row_find(pred[0]->get_ID()) % SizeX;
          vector<int> mapped_nodes;
          for(int ii=0; ii<(int) indices.size(); ii++)
          {
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            mapped_nodes.clear();
            mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());


            if((int)mapped_nodes.size() > 0)
            {
              for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
              {
                if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                  continue;

                if(row == -1)
                {
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp3->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp4->getPE()))
                    potential_pos.push_back(indices[ii]);
                }
                else
                {
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp3->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp4->getPE()))
                    if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                      potential_pos.push_back(indices[ii]);
                }
              }
            }
            else
            {
              if(row == -1)
              {
                if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp3->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp4->getPE()))
                  potential_pos.push_back(indices[ii]);
              }
              else
              {

                if(connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp3->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp4->getPE()))
                  if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                    potential_pos.push_back(indices[ii]);
              }
            }

          }
        }
        else
        {
          for(int ii=0; ii< (int) indices.size(); ii++)
          {
            // check if the PE is free, if not continue to the next position.
            if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

            if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp3->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp4->getPE()))
              potential_pos.push_back(indices[ii]);

          }
        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);

        _FATAL("1 pred mapped with 5 succs\n");
        return make_tuple(retVal, potential_pos, map_case);
      }
    case 22:
      {
        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* mp1 = pred[1]->GetCurrentPosition();
        MappingPair* out_mp = succ[0]->GetCurrentPosition();
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();
        MappingPair* out_mp2 = succ[2]->GetCurrentPosition();
        MappingPair* out_mp3 = succ[3]->GetCurrentPosition();
        MappingPair* out_mp4 = succ[4]->GetCurrentPosition();
        vector<int> potential_pos;

        for(int ii=0; ii< (int) indices.size(); ii++)
        {
          // check if the PE is free, if not continue to the next position.
          if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
            continue;

          if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp1->getPE(), mapPairs[indices[ii]]->getPE())  && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp3->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp4->getPE()))
            potential_pos.push_back(indices[ii]);

        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);


        _FATAL("2 pred mapped with 5 succs\n");
        return make_tuple(retVal, potential_pos, map_case);
      }
    case 23:
      {
        MappingPair* mp = pred[0]->GetCurrentPosition();
        MappingPair* mp1 = pred[1]->GetCurrentPosition();
        MappingPair* mp2 = pred[2]->GetCurrentPosition();
        MappingPair* out_mp = succ[0]->GetCurrentPosition();
        MappingPair* out_mp1 = succ[1]->GetCurrentPosition();
        MappingPair* out_mp2 = succ[2]->GetCurrentPosition();
        MappingPair* out_mp3 = succ[3]->GetCurrentPosition();
        MappingPair* out_mp4 = succ[4]->GetCurrentPosition();
        vector<int> potential_pos;

        for(int ii=0; ii< (int) indices.size(); ii++)
        {
          // check if the PE is free, if not continue to the next position.
          if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
            continue;

          if(connectedPEs(mp->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp1->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mp2->getPE(), mapPairs[indices[ii]]->getPE())  && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp1->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp2->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp3->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), out_mp4->getPE()))
            potential_pos.push_back(indices[ii]);

        }
        retVal.clear();
        //Get the corresponding mappairs for the potential pos
        for(int ii=0; ii < (int) potential_pos.size(); ii++)
          retVal.push_back(mapPairs[potential_pos[ii]]);

        return make_tuple(retVal, potential_pos, map_case);

        _FATAL("3 pred mapped with 5 succs\n");
        return make_tuple(retVal, potential_pos, map_case);
      }
    default:
      {
        vector<int> potential_pos;
        retVal.clear();
        return make_tuple(retVal, potential_pos, map_case); 
      }
  } 

  _FATAL("Should not reach here!!!\n");
  vector<int> potential_pos;
  return make_tuple(retVal, potential_pos, map_case);

}

void CGRA::BFS_refine_impl(DFG* myDFG, int s)
{
  // Create a queue for BFS 
  list<int> queue;

  // Mark the current node as visited and enqueue it 
  visited[s] = true;
  queue.push_back(s);

  // 'i' will be used to get all adjacent 
  // vertices of a vertex 
  list<int>::iterator i;

  while(!queue.empty())
  {
    // Dequeue a vertex from queue and print it 
    s = queue.front();
    //cout << s << " ";
    queue.pop_front();

    Node* v = _node_Set[s];

    if(is_already_mapped(v))
    {
      continue;
    }

    vector<Node*> P;
    vector<Node*> S; 

    tie(P,S) = get_Mapped_pred_succ(v);
    vector<MappingPair*> Tau;
    Tau.clear(); 
    int map_case;
    vector<int> indices = M.row_find(s, 1);
    vector<int> pos; 
    tie(Tau, pos, map_case) = Get_Free_Coordinates(v, P, S, indices);       

    int sel = 0 ; 
    if((int) Tau.size() == 1)
      sel = 0;
    else if((int) Tau.size() > 1)
      sel = rand() % (int) Tau.size(); 
    else if((int) Tau.size() == 0)
    {
      //TEC.print_matrix(); 
      //_FATAL("Cannot find a mapping for %d with map case: %d", v->get_ID(), map_case);
      cout << "Cannot find a mapping for node: " << v->get_ID() << " with map case: " << map_case << endl;
      TEC.print_matrix();
      //_FATAL("Cannot find a mapping for %d with map case: %d", v->get_ID(), map_case);
      // implement the diagnostic function and from that analyze the map_case and call recovery. 
      int success = Map_Failure_Diagnostic(v, P, S, map_case); 

      if(!success)
        return;
      else
      {
        // if success update the pred succ mappable indices list.
        //TEC.print_matrix();
        //_FATAL("Found mapping for %d\n", v->get_ID()); 
        //Update_Pred_Succ_Indices(P, S, v);
        continue; 
      }
    }
    //M.set_row_zero(s);
    //M.set_value(s, pos[sel], 1);
    v->SetMappableIndices(Tau); 
    v->SetCurrentPosition(Tau[sel]); 
    BestIndices.push_back(Tau[sel]); 
    AssignTEC(Tau[sel]->getPE()->getIndexI(), Tau[sel]->getPE()->getIndexJ(), Tau[sel]->getPE()->getIndexT(), v->get_ID());

    // update predecessor's and successor's Mappable Indices based on the sel value.
    Update_Pred_Succ_Indices(v);     


    // Get all adjacent vertices of the dequeued 
    // vertex s. If a adjacent has not been visited,  
    // then mark it visited and enqueue it 
    for (i = adjacency_list[s].begin(); i != adjacency_list[s].end(); ++i)
    {
      // is i in left nodes if yes,
      // if visited in false set it to true
      // TODO: track this down and fix the error
      /*if(contains(left_nodes_unmapped, _node_Set[*i]))
        {
        if(!visited[*i])
        visited[*i] = true;
        continue;
        }*/ 

      if (!visited[*i])
      {
        visited[*i] = true;
        queue.push_back(*i);

      }
    }
  }
}


void CGRA::BFS_refine_impl_1(DFG* myDFG, int s)
{
  // Create a queue for BFS 
  list<int> queue;
  // Mark the current node as visited and enqueue it 
  //visited[s] = true;
  queue.push_back(s);

  // 'i' will be used to get all adjacent 
  // vertices of a vertex 
  list<int>::iterator i;

  while(!queue.empty())
  {
    // Dequeue a vertex from queue and print it 
    s = queue.front();
    //cout << s << " ";
    queue.pop_front();

    // Get all adjacent vertices of the dequeued 
    // vertex s. If a adjacent has not been visited,  
    // then mark it visited and enqueue it 
    for (i = adjacency_list[s].begin(); i != adjacency_list[s].end(); ++i)
    {
      // is i in left nodes if yes,
      // if visited in false set it to true
      // TODO: track this down and fix the error
      if(contains(left_nodes_unmapped, _node_Set[*i]))
      {
        if(!visited[*i])
          visited[*i] = true;
        continue;
      }

      if (!visited[*i])
      {
        visited[*i] = true;
        queue.push_back(*i);

        Node* current_node = _node_Set[*i];

        // Is is to make sure the node is already mapped or not.
        // If the current node is mapped conitnue to the next node,
        // This check is important since I see same assigned two,
        //  different PEs.

        if(is_already_mapped(current_node))
          continue; 

        int value = 1;
        //int resources = SizeX * SizeY;
        int sel;

        vector<int> indices = M.row_find(*i, value);
        vector<int> selected_indices, zero_indices; //selected indices is a vector for recomputation purposes.

        //container for incoming and outgoing mapped 
        vector<Node*> pred; 
        vector<Node*> succ;  
        // get mapped incoming nodes
        // for(int ii = 0; ii < _node_Set[*i]->get_Number_of_Pred(); ii++)
        {
          vector<Node*> incoming_nodes = _node_Set[*i]->Get_Prev_Nodes();
          for(int jj = 0; jj < (int) incoming_nodes.size(); jj++) 
          {
            for(int k = 0 ; k < (int)_node_Set.size(); k++)
            {
              if(incoming_nodes[jj]->get_ID() == _node_Set[k]->get_ID() && visited[k] == true)
              {
                pred.push_back(incoming_nodes[jj]);
                break;
              }
            }
          }
        }

        // get mapped outgoing nodes
        //for(int ii = 0; ii < _node_Set[*i]->get_Number_of_Succ(); ii++)
        {
          vector<Node*> outgoing_nodes = _node_Set[*i]->Get_Next_Nodes();
          for(int jj = 0; jj < (int) outgoing_nodes.size(); jj++)
          {
            for(int k = 0 ; k < (int) _node_Set.size(); k++)
            {
              if(outgoing_nodes[jj]->get_ID() == _node_Set[k]->get_ID() && visited[k] == true)
              {
                succ.push_back(outgoing_nodes[jj]);
                break; 
              }
            }
          }
        }

        //Node* current_node = _node_Set[*i];

        //cout << "current_node: " << current_node->get_ID() << "\toutgoing mapped: " << (int) succ.size() << "\tincoming nodes: " << (int) pred.size() << endl; 

        if(current_node->is_Load_Address_Generator() && succ.size() > 2)
          _FATAL("Load address generator has more than 2 out-edges, please inspect this!!!\n"); 
        //cout << "Current Node: " << current_node->get_ID() << endl; 
        //if(current_node->is_Store_Address_Generator() || current_node->is_Store_Data_Bus_Write())
        //  _FATAL("store nodes are not supproted yet\n");
        if(succ.size() == 0)
        {
          if(pred.size() == 0)
          {
            if(current_node->is_Load_Address_Generator())
            {
              Load_nodes.push_back(current_node); 
              //row_id = sel % SizeX ;

              vector<int> potential_pos, mapped_nodes;
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());
                //bool found_mem=false;
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


              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }

              //if(current_node->get_ID() == 24)
              //{
              //  cout << "from 0 0: sel is: " << sel << endl;
              //  cout << "I row: " << mapPairs[sel]->getPE()->getIndexI() << endl;
              //cout << "row variable: " << row << endl;
              //}

              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());



              // check this  Oct 11. - Mahesh
              /*if(row_idx.size() == 0)
                {
                row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
                M.set_row_zero(*i);
                M.set_value(*i, sel, 1);
                AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
                }
              // this if condition is for multiple load addr gen nodes. 
              else if(!contains_row(row_idx, mapPairs[sel]->getPE()->getIndexI()))
              {
              // if the node is a load address, it can take up one PE in one row.
              // no two load nodes can take up a single row.
              // this is checked by having selected PE,time and a row of the PE and time.
              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1); 
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());    
              }
              else
              {
              cout << "0 input mapped for node: " << current_node->get_ID() << endl;
              cout << "input mapped 1 and load addr generator for node: " << current_node->get_ID() << "  with row_idx size: " << (int) row_idx.size() << endl;
              for(int jj=0;jj<(int)row_idx.size(); jj++)
              cout << "row_idx:" << row_idx[jj] << endl;

              for(int jj=0;jj<(int) potential_pos.size(); jj++)
              cout << "potential pos: " << potential_pos[jj] << endl;
              cout << "sel: " << sel << endl;
              cout << "mppairs: " << mapPairs[sel]->getPE()->getIndexI() << endl;
              _FATAL("Should not come here. This means the load addr balacing per cycles was not done\n"); //this is fatal because the load balancing should be done during scheduling. 
              }*/
            }
            else if(current_node->is_Load_Data_Bus_Read()) // do not map the load data bus read first. it needs to find the same row as load addr.
            {
              //int row;
              //row = TEC.row_find(pred[0]->get_ID()) % SizeX;

              // if the load addr is visited but not mapped, add this node to the unmapped list and continue. 
              //if(row == -1)
              // {
              //   left_nodes_unmapped.push_back(current_node);
              //   continue;
              //}

              //cout << "0 0" << endl;
              //visited[*i] = false;
              //continue;

              vector<int> potential_pos, mapped_nodes; 
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());


                //if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                // {
                if((int) mapped_nodes.size() > 0)
                {
                  for(int jj=0; jj< (int) mapped_nodes.size(); jj++)
                    if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                      break;
                    else
                      potential_pos.push_back(indices[ii]);
                }
                else
                  potential_pos.push_back(indices[ii]);
                //}
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }

              //if(current_node->get_ID() == 25)
              // {
              //   cout << "from 0 0: sel is: " << sel << endl;
              //   cout << "I row: " << mapPairs[sel]->getPE()->getIndexI() << endl;
              //cout << "row variable: " << row << endl;
              // }


              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
            else if(current_node->is_Store_Address_Generator())
            {
              vector<int> potential_pos;
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;


                if((int)store_addr_row_idx.size() > 0)
                {
                  for(int it=0; it < (int) store_addr_row_idx.size(); it++)
                  {
                    if(mapPairs[indices[ii]]->getPE()->getIndexI() != store_addr_row_idx[it])
                    {
                      potential_pos.push_back(indices[ii]);
                    }
                  }
                }
                else
                {
                  //since there are no in and out nodes mapped, no need to find connectedPEs                
                  potential_pos.push_back(indices[ii]);
                }
              }
              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }

              if(store_addr_row_idx.size() == 0)
              {   
                store_addr_row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
                M.set_row_zero(*i);
                M.set_value(*i, sel, 1);
                AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
              } 
              else if(!contains_row(store_addr_row_idx, mapPairs[sel]->getPE()->getIndexI()))
              {
                store_addr_row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
                M.set_row_zero(*i);
                M.set_value(*i, sel, 1);
                AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
              }
              else
                _FATAL("0 outgoing 0 incoming, store addr node... Inspect!!!\n");
            }
            else if(current_node->is_Store_Data_Bus_Write())
            {
              vector<int> potential_pos;
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                //since there are no in and out nodes mapped, no need to find connectedPEs                
                potential_pos.push_back(indices[ii]);
              }
              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
            else // this else if for rest of the nodes other than load addr gen and load data bus that has no input operands.  
            {
              vector<int> potential_pos;
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                //since there are no in and out nodes mapped, no need to find connectedPEs                
                potential_pos.push_back(indices[ii]);
              }
              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID()); 
            }
          } // end of if(incoming == 0)
          else if(pred.size() == 1)
          {
            int mp = get_mapped_mappairs(pred[0]); 

            if(current_node->is_Load_Address_Generator())
            {
              Load_nodes.push_back(current_node); 
              //row_id = sel % SizeX ;

              vector<int> potential_pos, mapped_nodes;
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());
                //bool found_mem=false;
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

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }

              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());

              // check this  Oct 11. - Mahesh
              /*if(row_idx.size() == 0)
                {
                row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
                M.set_row_zero(*i);
                M.set_value(*i, sel, 1);
                AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
                }
              // this if condition is for multiple load addr gen nodes. 
              else if(!contains_row(row_idx, mapPairs[sel]->getPE()->getIndexI()))
              {
              //if the node is a load address, it can take up one PE in one row.
              // no two load nodes can take up a single row.
              // this is checked by having selected PE,time and a row of the PE and time.

              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1); 
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());    
              }
              else {
              cout << "input mapped 1 and load addr generator for node: " << current_node->get_ID() << "  with row_idx size: " << (int) row_idx.size() << endl;
              for(int jj=0;jj<(int)row_idx.size(); jj++)
              cout << "row_idx:" << row_idx[jj] << endl;

              for(int jj=0;jj<(int) potential_pos.size(); jj++)
              cout << "potential pos: " << potential_pos[jj] << endl;
              cout << "sel: " << sel << endl;
              cout << "mapPairs sel row: " << mapPairs[sel]->getPE()->getIndexI() << endl;
              _FATAL("Should not come here. This means the load addr balacing per cycles was not done\n"); //this is fatal because the load balancing should be done during scheduling. 
              }*/
            }
            else if(current_node->is_Load_Data_Bus_Read())
            {
              if(!pred[0]->is_Load_Address_Generator())
                _FATAL("The only incoming to Data bus read that is mapped should be Load addr gen\n");

              //int pos=-1; 
              //for(int ii=0; ii < (int) _node_Set.size(); ii++) 
              //  if(_node_Set[ii]->get_ID() == pred[0]->get_ID())  
              //    pos++;
              int row=-1;
              row = TEC.row_find(pred[0]->get_ID()) % SizeX; 

              // if the load addr is visited but not mapped, add this node to the unmapped list and continue. 
              //if(row == -1)
              //{
              //  left_nodes_unmapped.push_back(current_node);
              //  continue;
              // }

              vector<int> potential_pos, mapped_nodes;
              //int id_row = row_idx[pos];

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
                          continue;
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
                    if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                      potential_pos.push_back(indices[ii]);
                  }  
                }


                /*if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                  {
                  if((int) mapped_nodes.size() > 0)
                  {
                  for(int jj=0; jj< (int) mapped_nodes.size(); jj++)
                  if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                  break;
                  else
                  potential_pos.push_back(indices[ii]);
                  }
                  else
                  potential_pos.push_back(indices[ii]);
                  }*/

                /*  mapped_nodes.clear(); 
                    mapped_nodes = get_nodes_mapped_to_row(mapPairs[indices[ii]]->getPE()->getIndexI());

                    if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                    {
                    if(mapped_nodes.size() > 0)
                    {
                    for(int kk=0; kk< nodeSize; kk++)
                    {
                    for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                    {
                    if(_node_Set[kk]->get_ID() == mapped_nodes[jj])
                    {
                    if(!_node_Set[kk]->is_Load_Data_Bus_Read() && !_node_Set[kk]->is_Store_Data_Bus_Write() && !_node_Set[kk]->is_Store_Address_Generator()) //add connectedPEs for out>0
                    {
                    potential_pos.push_back(indices[ii]);
                    break;
                    }
                    }
                    }
                    }

                    }
                    else    
                    potential_pos.push_back(indices[ii]); 
                    }*/
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              { 
                //cout << "No potential postions for 1 input load generator and data bus\n"; 
                left_nodes_unmapped.push_back(current_node);
                continue;
                // _FATAL("No potential postions for 1 input load generator and data bus\n");
              }

              //if(current_node->get_ID() == 25)
              //{
              //  cout << "from 0 1: sel is: " << sel << endl;
              //  cout << "I row: " << mapPairs[sel]->getPE()->getIndexI() << endl;
              //  cout << "row variable: " << row << endl;
              // }

              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());  
            }
            else if(current_node->is_Store_Address_Generator())
            {
              vector<int> potential_pos;
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;


                if((int)store_addr_row_idx.size() > 0)
                {
                  for(int it=0; it < (int) store_addr_row_idx.size(); it++)
                  {
                    if(mapPairs[indices[ii]]->getPE()->getIndexI() != store_addr_row_idx[it])
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
              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }

              if(store_addr_row_idx.size() == 0)
              {   
                store_addr_row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
                M.set_row_zero(*i);
                M.set_value(*i, sel, 1);
                AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
              } 
              else if(!contains_row(store_addr_row_idx, mapPairs[sel]->getPE()->getIndexI()))
              {
                store_addr_row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
                M.set_row_zero(*i);
                M.set_value(*i, sel, 1);
                AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
              }
              else
                _FATAL("0 outgoing 0 incoming, store addr node... Inspect!!!\n"); 

            }
            else if(current_node->is_Store_Data_Bus_Write())
            {
              vector<int> potential_pos;
              // check if th eincoming mapped node is a store addr. 
              if(pred[0]->is_Store_Address_Generator())
              {
                int row = -1;
                row = TEC.row_find(pred[0]->get_ID()) % SizeX;

                // if the store addr is visited but not mapped, add this node to the unmapped list and continue. 
                if(row == -1)
                {
                  left_nodes_unmapped.push_back(current_node);
                  continue;
                }
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
              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
            else // all the other nodes with one input operation mapped.  
            {   
              // get the PE and time the pred was mapped from TEC. 
              //int mp = get_mapped_mappairs(pred[0]);

              vector<int> potential_pos;

              for(int ii=0; ii < (int) indices.size(); ii++)
              {
                // check if the PE is free, if not continue to the next position.
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
                  potential_pos.push_back(indices[ii]); 

              }


              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()]; 
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {   
                //cout << "No potential postions for 0 incoming 0 outgoing node: " << current_node->get_ID() << endl; 
                left_nodes_unmapped.push_back(current_node);
                continue;
              }

              // check if the PE is free, if not choose another compatible PE
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);

              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }

          }
          else if(pred.size() == 2)
          {
            // get coordinates of pred
            int mp= get_mapped_mappairs(pred[0]);
            int mp1 = get_mapped_mappairs(pred[1]);

            if(current_node->is_Store_Data_Bus_Write())
            {
              vector<int> potential_pos; 
              int row=-1, index=-1;
              for(int jj=0; jj < (int) pred.size(); jj++)
              {
                if(pred[jj]->is_Store_Address_Generator())
                {
                  row = TEC.row_find(pred[jj]->get_ID()) % SizeX;

                  // if the load addr is visited but not mapped, add this node to the unmapped list and continue. 
                  if(row == -1)
                  {
                    left_nodes_unmapped.push_back(current_node);
                    continue;
                  }
                }
                else
                  index=jj;
              }

              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                //since there are no in and out nodes mapped, no need to find connectedPEs                
                if(mapPairs[indices[ii]]->getPE()->getIndexI() == row) // select the row of mapped store addr
                  if(connectedPEs(mapPairs[index]->getPE(), mapPairs[indices[ii]]->getPE())) // get a compatible PE with the other node PE index. 
                    potential_pos.push_back(indices[ii]);
              }
              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
            else // for all other nodes.
            {
              vector<int> potential_pos;

              for(int ii=0; ii < (int) indices.size(); ii++)
              {
                // check if the PE is free, if not continue to the next position.
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp1]->getPE(), mapPairs[indices[ii]]->getPE()))
                  potential_pos.push_back(indices[ii]);
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 0 outgoing and 2 incoming node: " << current_node->get_ID() << endl;
                left_nodes_unmapped.push_back(current_node);
                continue;
                // _FATAL("No potential postions for 1 input other nodes\n");
              }

              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);

              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
          }
          else if(pred.size() == 3)
          {
            // get coordinates of pred
            int mp = get_mapped_mappairs(pred[0]); 
            int mp1 = get_mapped_mappairs(pred[1]);
            int mp2 = get_mapped_mappairs(pred[2]);

            vector<int> potential_pos;


            for(int ii=0; ii < (int) indices.size(); ii++)
            {
              // check if the PE is free, if not continue to the next position.
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp1]->getPE(), mapPairs[indices[ii]]->getPE()) 
                  && connectedPEs(mapPairs[mp2]->getPE(), mapPairs[indices[ii]]->getPE())) 
                potential_pos.push_back(indices[ii]);
            }


            if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
            else if(potential_pos.size() == 1)
              sel = potential_pos[0];
            else
            {
              //cout << "No potential postions for all 3. \n";
              left_nodes_unmapped.push_back(current_node);
              continue;
            }
            M.set_row_zero(*i);
            M.set_value(*i, sel, 1);

            AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
          }
          else
          {
            //cout << "We are here for node: " << current_node->get_ID() << " with incoming mapped: " << pred.size() << endl;
            _FATAL("should not come here. In outgoing nodes == 0\n");
          }

        }// end of if outgoing == 0
        else if(succ.size() == 1)
        {
          // container holding the mapPairs of the outgoing node.
          int out_mp = get_mapped_mappairs(succ[0]);  
          if(pred.size() == 0)
          {
            if(current_node->is_Load_Address_Generator())
            {

              //if(succ[0]->is_Load_Data_Bus_Read())
              //{
              //  cout << "current load node: " << current_node->get_ID() << "---> " << succ[0]->get_ID() << endl;  
              // _FATAL("The load bus should always be mapped after the load addr.... Check the nodes!!\n"); 
              //}

              Load_nodes.push_back(current_node); 
              //row_id = sel % SizeX ;
              int row=-2;
              if(succ[0]->is_Load_Data_Bus_Read())
                row = TEC.row_find(succ[0]->get_ID()) % SizeX;


              //if(row==-1) // outgoing load bus read was not mapped.
              // {
              //   left_nodes_unmapped.push_back(current_node);
              //   continue;
              // }

              vector<int> potential_pos, mapped_nodes;
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;
                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());
                //bool found_mem=false;
                if(((int) mapped_nodes.size()) > 0)
                {
                  for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                  {
                    if(row > 0)
                    {
                      if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                      {
                        if(get_Node(mapped_nodes[jj])->is_Load_Address_Generator() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                          break;
                        else
                          potential_pos.push_back(indices[ii]);
                      }
                    }
                    else
                    {
                      if(get_Node(mapped_nodes[jj])->is_Load_Address_Generator() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                        break;
                      else
                      {
                        if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()))
                          potential_pos.push_back(indices[ii]);   
                      }

                    }
                  }
                }
                else
                {
                  if(row > 0)
                  {
                    if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)   
                      potential_pos.push_back(indices[ii]);
                  }
                  else
                  {
                    if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()))
                      potential_pos.push_back(indices[ii]);
                  }
                }

              }

              //if(current_node->get_ID() == 24)
              //{
              //  cout << "from 0 1: sel is: " << sel << endl;
              //  cout << "I row: " << mapPairs[sel]->getPE()->getIndexI() << endl;
              //  cout << "row vairable: " << row << endl;  
              // }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }

              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());


              // check this  Oct 11. - Mahesh
              /*if(row_idx.size() == 0)
                {
                row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
                M.set_row_zero(*i);
                M.set_value(*i, sel, 1);
                AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
                }
              // this if condition is for multiple load addr gen nodes. 
              else if(!contains_row(row_idx, mapPairs[sel]->getPE()->getIndexI()))
              {
              // if the node is a load address, it can take up one PE in one row.
              // no two load nodes can take up a single row.
              // this is checked by having selected PE,time and a row of the PE and time.

              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1); 
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());    
              }
              else
              {
              cout << "0 input mapped for node: " << current_node->get_ID() << endl;
              cout << "input mapped 1 and load addr generator for node: " << current_node->get_ID() << "  with row_idx size: " << (int) row_idx.size() << endl;
              for(int jj=0;jj<(int)row_idx.size(); jj++)
              cout << "row_idx:" << row_idx[jj] << endl;

              for(int jj=0;jj<(int) potential_pos.size(); jj++)
              cout << "potential pos: " << potential_pos[jj] << endl;
              cout << "sel: " << sel << endl;
              cout << "mppairs: " << mapPairs[sel]->getPE()->getIndexI() << endl;
              _FATAL("Should not come here. This means the load addr balacing per cycles was not done\n"); //this is fatal because the load balancing should be done during scheduling. 
              }*/
            }
            else if(current_node->is_Load_Data_Bus_Read())
            { 
              // This should not happen, bus read should always be mapped after the load addr.
              //cout << "outgo 1 and incoming 0" << endl;
              //visited[*i] = false;  
              //continue;
              //int row;
              //row = TEC.row_find(pred[0]->get_ID()) % SizeX;

              // if the load addr is visited but not mapped, add this node to the unmapped list and continue. 
              //if(row == -1)
              // {
              //   left_nodes_unmapped.push_back(current_node);
              //   continue;
              // }
              vector<int> potential_pos, mapped_nodes;
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                /*mapped_nodes.clear();
                  mapped_nodes = get_nodes_mapped_to_row(mapPairs[indices[ii]]->getPE()->getIndexI());
                  if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()))
                  {
                  if(mapped_nodes.size() > 0)
                  {
                  for(int kk=0; kk<nodeSize; kk++)
                  {
                  for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                  {
                  if(_node_Set[kk]->get_ID() == mapped_nodes[jj])
                  {
                  if(!_node_Set[kk]->is_Load_Data_Bus_Read() && !_node_Set[kk]->is_Store_Data_Bus_Write() && !_node_Set[kk]->is_Store_Address_Generator()) //add connectedPEs for out>0
                  {
                  potential_pos.push_back(indices[ii]);
                  break;
                  }
                  }
                  }
                  }
                  }
                  else
                  {
                  potential_pos.push_back(indices[ii]);
                  }
                  }*/
                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());


                //if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                //{
                if((int) mapped_nodes.size() > 0)
                {
                  for(int jj=0; jj< (int) mapped_nodes.size(); jj++)
                    if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                      break; 
                    else
                    {
                      if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()))
                        potential_pos.push_back(indices[ii]);
                    }
                }
                else
                {
                  if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()))
                    potential_pos.push_back(indices[ii]);
                }
                //}
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              //if(current_node->get_ID() == 25)
              //{
              //  cout << "from 1 0: sel is: " << sel << endl;
              //  cout << "I row: " << mapPairs[sel]->getPE()->getIndexI() << endl;
              //cout << "row variable: " << row << endl;
              //}


              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
            else if(current_node->is_Store_Address_Generator())
            {
              vector<int> potential_pos; 
              int row=-1;
              row = TEC.row_find(succ[0]->get_ID()) % SizeX;

              // if the store addr is visited but not mapped, add this node to the unmapped list and continue. 
              if(row == -1)
              {
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                //since there are no in and out nodes mapped, no need to find connectedPEs                
                if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                  potential_pos.push_back(indices[ii]);
              }
              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
            else if(current_node->is_Store_Data_Bus_Write())
            {
              _FATAL("Store bus write should not have outgoing node. Inspect!!!\n"); 

            }
            else // for all other nodes  
            { 

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
                  //cout << endl;
                  //cout << "1 out 0 in node out: " << mapPairs[out_mp]->getPE()->getIndexI() << "  " << mapPairs[out_mp]->getPE()->getIndexJ() << "  " << mapPairs[out_mp]->getPE()->getIndexT() << "  index: " << mapPairs[indices[ii]]->getPE()->getIndexI() << "  " << mapPairs[indices[ii]]->getPE()->getIndexJ() << "  " << mapPairs[indices[ii]]->getPE()->getIndexT() << endl;
                  potential_pos.push_back(indices[ii]);
                }
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 outgoing 0 incoming node: " << current_node->get_ID() << endl ;
                //TEC.print_matrix();
                //_FATAL("No potential postions for 1 outgoing 0 incoming node: %d\n", current_node->get_ID());
                //exit(1);
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              //cout << "1 out 0 in node: " << current_node->get_ID() << "\tpotential pos: " << potential_pos.size() << "\tselected:" << mapPairs[sel]->getPE()->getIndexI() <<  "," << mapPairs[sel]->getPE()->getIndexJ() << "," <<  mapPairs[sel]->getPE()->getIndexT() << endl;
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);

              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID()); 
            }
          }
          else if(pred.size() == 1)
          {
            int mp = get_mapped_mappairs(pred[0]);
            if(current_node->is_Load_Address_Generator())
            {
              //if(succ[0]->is_Load_Data_Bus_Read())
              //{
              //  cout << "current load mode: " << current_node->get_ID() << "-->" << succ[0]->get_ID() << endl;
              // _FATAL("This should not happed. Inspect!!\n"); 
              //}
              Load_nodes.push_back(current_node); 
              //row_id = sel % SizeX ;

              int row=-2;
              if(succ[0]->is_Load_Data_Bus_Read())
                row = TEC.row_find(succ[0]->get_ID()) % SizeX;


              //if(row==-2) // outgoing load bus read was not mapped.
              //{
              //  left_nodes_unmapped.push_back(current_node);
              //  continue;
              // }

              vector<int> potential_pos, mapped_nodes;
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;


                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());
                // check the row_idx to find the row in this cycle that has been occupied, and select a different row.

                bool found_mem=false;

                if(mapped_nodes.size() > 0)
                  for(int jj=0; jj < nodeSize; jj++)
                    for(int kk=0; kk < (int) mapped_nodes.size(); kk++)
                      if(_node_Set[jj]->get_ID() == mapped_nodes[kk])
                        if(_node_Set[jj]->is_Store_Address_Generator() || _node_Set[jj]->is_Load_Address_Generator())
                          found_mem=true;

                if(!found_mem)
                {
                  if(row==-2)
                  {
                    if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()))
                      potential_pos.push_back(indices[ii]);
                  }
                  else
                  {
                    if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()))
                      if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                        potential_pos.push_back(indices[ii]);
                  }
                }



                /*if(row == -2) // this means the outgoing mapped did not have load data bus read
                  {
                  if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
                  {
                  if(mapped_nodes.size() > 0)
                  {
                  for(int ii=0; ii<(int) mapped_nodes.size(); ii++)
                  {
                  for(int jj=0; jj < nodeSize; jj++)
                  {
                  if(_node_Set[jj]->get_ID() == mapped_nodes[ii])
                  {
                  if(!_node_Set[jj]->is_Load_Data_Bus_Read() && !_node_Set[jj]->is_Store_Address_Generator()) //add connectedPEs for out>0
                  {
                  potential_pos.push_back(indices[ii]);
                  break;
                  }
                  }
                  }
                  }

                  }
                  else
                  potential_pos.push_back(indices[ii]);
                  }
                  }
                  else // this means outgoing mapped had load data bus read and we found the mapped row.
                  {
                  if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()))
                  {
                  if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                  {
                  if(mapped_nodes.size() > 0)
                  {
                  for(int ii=0; ii<(int) mapped_nodes.size(); ii++)
                  {
                  for(int jj=0; jj < nodeSize; jj++)
                  {
                  if(_node_Set[jj]->get_ID() == mapped_nodes[ii])
                  {
                  if(!_node_Set[jj]->is_Load_Data_Bus_Read() && !_node_Set[jj]->is_Store_Address_Generator()) //add connectedPEs for out>0
                  {
                  potential_pos.push_back(indices[ii]);
                  break;
                  }
                  }
                  }
                  }

                  }
                  else
                  potential_pos.push_back(indices[ii]);
                  }
                  }
                  }*/
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }

              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());


              // check this  Oct 11. - Mahesh
              /*if(row_idx.size() == 0)
                {
                row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
                M.set_row_zero(*i);
                M.set_value(*i, sel, 1);
                AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
                }
              // this if condition is for multiple load addr gen nodes. 
              else if(!contains_row(row_idx, mapPairs[sel]->getPE()->getIndexI()))
              {
              // if the node is a load address, it can take up one PE in one row.
              // no two load nodes can take up a single row.
              // this is checked by having selected PE,time and a row of the PE and time.

              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1); 
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());    
              }
              else
              {
              cout << "0 input mapped for node: " << current_node->get_ID() << endl;
              cout << "input mapped 1 and load addr generator for node: " << current_node->get_ID() << "  with row_idx size: " << (int) row_idx.size() << endl;
              for(int jj=0;jj<(int)row_idx.size(); jj++)
              cout << "row_idx:" << row_idx[jj] << endl;

              for(int jj=0;jj<(int) potential_pos.size(); jj++)
              cout << "potential pos: " << potential_pos[jj] << endl;
              cout << "sel: " << sel << endl;
              cout << "mppairs: " << mapPairs[sel]->getPE()->getIndexI() << endl;
              _FATAL("Should not come here. This means the load addr balacing per cycles was not done\n"); //this is fatal because the load balancing should be done during scheduling. 
              }*/
            }
            else if(current_node->is_Load_Data_Bus_Read())
            {
              //int pos=-1; 
              //for(int ii=0; ii < (int) _node_Set.size(); ii++) 
              // if(_node_Set[ii]->get_ID() == pred[0]->get_ID())  
              //   pos++;
              int row=-1;
              row = TEC.row_find(pred[0]->get_ID()) % SizeX;

              // if the load addr is visited but not mapped, add this node to the unmapped list and continue. 
              if(row == -1)
              {
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              vector<int> potential_pos, mapped_nodes;
              //int id_row = row_idx[pos];

              // the incoming load addr generator should be mapped and set row_idx. Additional check.
              //assert(row_idx.size() > 0);

              for(int ii=0; ii < (int) indices.size(); ii++)
              {
                // check if the PE is free, if not continue to the next position.
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());

                if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()))
                {
                  if(mapPairs[indices[ii]]->getPE()->getIndexI() == row) // row_idx[pos]
                  {
                    if(mapped_nodes.size() > 0)
                    {
                      for(int kk=0; kk<nodeSize; kk++)
                      {
                        for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                        {
                          if(_node_Set[kk]->get_ID() == mapped_nodes[jj])
                          {
                            if(!_node_Set[kk]->is_Load_Data_Bus_Read() && !_node_Set[kk]->is_Store_Data_Bus_Write() && !_node_Set[kk]->is_Store_Address_Generator()) //add connectedPEs for out>0
                            {
                              potential_pos.push_back(indices[ii]);
                              break;
                            }
                          }
                        }
                      }
                    }
                    else
                      potential_pos.push_back(indices[ii]);
                  }
                }
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              { 
                //cout << "No potential postions for 1 input load generator and data bus\n"; 
                left_nodes_unmapped.push_back(current_node);
                continue;
              }

              // check if the PE is free, if not choose another compatible PE 
              /*if(!isTECfree(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT()))
                {
                vector<int> used_mapPairs;
                used_mapPairs.push_back(sel);
                vector<int> potential_pairs = difference(potential_pos, used_mapPairs);

                if((int) potential_pairs.size() == 1)
                sel = potential_pairs[0];
                else if((int) potential_pairs.size() > 1)
                {
                int temp = rand() % potential_pairs.size();
                sel = potential_pairs[temp];
                }
                }*/
              //if(current_node->get_ID() == 25)
              // {
              //   cout << "from 1 1: sel is: " << sel << endl;
              //   cout << "I row: " << mapPairs[sel]->getPE()->getIndexI() << endl;
              //   cout << "row variable: " << row << endl;
              //}



              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);

              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());  
            }
            else if(current_node->is_Store_Address_Generator())
            {
              vector<int> potential_pos;
              int row=-1;
              row = TEC.row_find(succ[0]->get_ID()) % SizeX;

              // if the store addr is visited but not mapped, add this node to the unmapped list and continue. 
              if(row == -1)
              {
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                //since there are no in and out nodes mapped, no need to find connectedPEs                
                if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                  if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
                    potential_pos.push_back(indices[ii]);
              }
              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());              
            }
            else // all the other nodes with one input operations.  
            {   
              vector<int> potential_pos;

              for(int ii=0; ii < (int) indices.size(); ii++)
              {
                // check if the PE is free, if not continue to the next position.
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()))
                  potential_pos.push_back(indices[ii]); 
              }

              //cout << "1 out 1 in node: " << current_node->get_ID() << "\tpotential pos: " << potential_pos.size() << endl;

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()]; 
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {   
                //cout << "No potential postions for 1 outgoing 1 incoming node: " << current_node->get_ID() << endl ; 
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);

              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }

          }
          else if(pred.size() == 2)
          {
            int mp= get_mapped_mappairs(pred[0]);
            int mp1 = get_mapped_mappairs(pred[1]);

            vector<int> potential_pos;

            for(int ii=0; ii < (int) indices.size(); ii++)
            {
              // check if the PE is free, if not continue to the next position.
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()))
                potential_pos.push_back(indices[ii]); 
            }

            if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
            else if(potential_pos.size() == 1)
              sel = potential_pos[0];
            else
            {
              //cout << "No potential postions for 2 incoming and 1 outgoing node: " << current_node->get_ID() << endl;
              //TEC.print_matrix(); 
              //_FATAL("No potential postions for 2 incoming and 1 outgoing node: %d", current_node->get_ID()); 
              left_nodes_unmapped.push_back(current_node);
              continue;
            }

            M.set_row_zero(*i);
            M.set_value(*i, sel, 1);

            AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
          }
          else if(pred.size() == 3)
          {
            int mp = get_mapped_mappairs(pred[0]); 
            int mp1 = get_mapped_mappairs(pred[1]);
            int mp2 = get_mapped_mappairs(pred[2]);

            vector<int> potential_pos;


            for(int ii=0; ii < (int) indices.size(); ii++)
            {
              // check if the PE is free, if not continue to the next position.
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp2]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE())) 
                potential_pos.push_back(indices[ii]);
            }

            if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
            else if(potential_pos.size() == 1)
              sel = potential_pos[0];
            else
            {
              //cout << "No potential postions for all 3. \n";
              left_nodes_unmapped.push_back(current_node);
              continue;
            }
            M.set_row_zero(*i);
            M.set_value(*i, sel, 1);

            AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
          }
          else
            _FATAL("should not come here. In outgoing nodes == 1\n");
        } // end of outgoing size == 1
        else if(succ.size() == 2)
        { 
          int out_mp = get_mapped_mappairs(succ[0]); 
          int out_mp1 = get_mapped_mappairs(succ[1]); 
          if(pred.size() == 0)
          {
            if(current_node->is_Load_Address_Generator())
            {


              int index=-1; 
              //for(int jj=0; jj < (int) succ.size(); jj++)
              //{
              //  if(succ[jj]->is_Load_Data_Bus_Read())
              //  {
              //    index = jj ;
              //    //cout << "current load node: " << current_node->get_ID() << "---> " << succ[jj]->get_ID() << endl;
              //    //_FATAL("The load bus should always be mapped after the load addr.... Check the nodes!!\n");

              // }
              // }

              Load_nodes.push_back(current_node);
              //sel = rand() % resources; // redo this section.  

              int row=-2; //int addr_node = 0; 
              for(int jj=0; jj < (int) succ.size(); jj++)
              {
                if(succ[jj]->is_Load_Data_Bus_Read())
                {
                  row = TEC.row_find(succ[jj]->get_ID()) % SizeX;
                  //    addr_node = jj; 
                }
                else
                  index = jj;
              }


              //if(row==-1) // outgoing load bus read was not mapped.
              //{
              //  left_nodes_unmapped.push_back(current_node);
              //  continue;
              // }

              vector<int> potential_pos, mapped_nodes;
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;


                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());
                // check the row_idx to find the row in this cycle that has been occupied, and select a different row.

                bool found_mem=false;

                if(mapped_nodes.size() > 0)
                  for(int jj=0; jj < nodeSize; jj++)
                    for(int kk=0; kk < (int) mapped_nodes.size(); kk++)
                      if(_node_Set[jj]->get_ID() == mapped_nodes[kk])
                        if(_node_Set[jj]->is_Store_Address_Generator() || _node_Set[jj]->is_Load_Address_Generator())
                          found_mem=true;

                if(!found_mem)
                {
                  //if(row==-2)
                  //{
                  //  if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()))
                  //    potential_pos.push_back(indices[ii]);
                  //}
                  //else
                  //{
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[get_mapped_mappairs(succ[index])]->getPE()))
                    if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                      potential_pos.push_back(indices[ii]);
                  //}
                }

                /*if(row == -2) // this means the outgoing mapped did not have load data bus read
                  {
                  if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp1]->getPE(), mapPairs[indices[ii]]->getPE()))
                  {
                  if(mapped_nodes.size() > 0)
                  {
                  for(int ii=0; ii<(int) mapped_nodes.size(); ii++)
                  {
                  for(int jj=0; jj < nodeSize; jj++)
                  {
                  if(_node_Set[jj]->get_ID() == mapped_nodes[ii])
                  {
                  if(!_node_Set[jj]->is_Load_Data_Bus_Read() && !_node_Set[jj]->is_Store_Address_Generator()) //add connectedPEs for out>0
                  {
                  potential_pos.push_back(indices[ii]);
                  break;
                  }
                  }
                  }
                  }

                  }
                  else
                  potential_pos.push_back(indices[ii]);
                  }
                  }
                  else // this means outgoing mapped had load data bus read and we found the mapped row.
                  {
                  if(connectedPEs(mapPairs[get_mapped_mappairs(succ[index])]->getPE(), mapPairs[indices[ii]]->getPE()))
                  {
                  if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                  {
                  if(mapped_nodes.size() > 0)
                  {
                  for(int ii=0; ii<(int) mapped_nodes.size(); ii++)
                  {
                  for(int jj=0; jj < nodeSize; jj++)
                  {
                  if(_node_Set[jj]->get_ID() == mapped_nodes[ii])
                  {
                  if(!_node_Set[jj]->is_Load_Data_Bus_Read() && !_node_Set[jj]->is_Store_Address_Generator()) //add connectedPEs for out>0
                  {
                  potential_pos.push_back(indices[ii]);
                  break;
                  }
                  }
                  }
                  }

                  }
                  else
                  potential_pos.push_back(indices[ii]);
                  }
                  }
                  }*/
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for all 3. \n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }

              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());


              /*if(row_idx.size() == 0)
                {
                row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
                M.set_row_zero(*i);
                M.set_value(*i, sel, 1);
                AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
                }
              // this if condition is for multiple load addr gen nodes. 
              else if(!contains_row(row_idx, mapPairs[sel]->getPE()->getIndexI()))
              {
              // if the node is a load address, it can take up one PE in one row.
              // no two load nodes can take up a single row.
              // this is checked by having selected PE,time and a row of the PE and time.
              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
              }
              else
              {
              cout << "0 input mapped for node: " << current_node->get_ID() << endl;
              cout << "input mapped 1 and load addr generator for node: " << current_node->get_ID() << "  with row_idx size: " << (int) row_idx.size() << endl;
              for(int jj=0;jj<(int)row_idx.size(); jj++)
              cout << "row_idx:" << row_idx[jj] << endl;

              for(int jj=0;jj<(int) potential_pos.size(); jj++)
              cout << "potential pos: " << potential_pos[jj] << endl;
              cout << "sel: " << sel << endl;
              cout << "mppairs: " << mapPairs[sel]->getPE()->getIndexI() << endl;
              _FATAL("Should not come here. This means the load addr balacing per cycles was not done\n"); //this is fatal because the load balancing should be done during scheduling. 
              }*/

            }
            else if(current_node->is_Load_Data_Bus_Read())
            {
              // this should not happen. Load addr should be mapped first
              //cout << "outgo 2 incoming 0" << endl;
              //visited[*i] = false;
              //continue;

              vector<int> potential_pos, mapped_nodes;
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());

                if((int)mapped_nodes.size() > 0)
                {
                  for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                  {
                    if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                      continue; 

                    if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()))         
                      potential_pos.push_back(indices[ii]);
                  }
                }
                else
                {
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()))
                    potential_pos.push_back(indices[ii]);
                }

                /*if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()))
                  {
                  if(mapped_nodes.size() > 0)
                  {
                  for(int kk=0; kk<nodeSize; kk++)
                  {
                  for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                  {
                  if(_node_Set[kk]->get_ID() == mapped_nodes[jj])
                  {
                  if(!_node_Set[kk]->is_Load_Data_Bus_Read() && !_node_Set[kk]->is_Store_Data_Bus_Write() && !_node_Set[kk]->is_Store_Address_Generator()) //add connectedPEs for out>0
                  {
                  potential_pos.push_back(indices[ii]);
                  break;
                  }
                  }
                  }
                  }
                  }
                  else
                  {              
                  potential_pos.push_back(indices[ii]);
                  }
                  }*/
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());

            }
            else // this else if for rest of the nodes other than load addr gen that has no input operands mapped but 2 outgoing mapped.  
            { 
              selected_indices.clear();
              zero_indices.clear();

              vector<int> potential_pos;
              for(int ii=0; ii < (int) indices.size(); ii++)
              {   
                // check if the PE is free, if not continue to the next position.
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()))
                  potential_pos.push_back(indices[ii]);  
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for all 3. \n";
                //TEC.print_matrix();
                //_FATAL("No ptential position for outgoing 2 and incoming 0 for node: %d",  current_node->get_ID()); 
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);

              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID()); 
            }
          } // end of if(incoming == 0)
          else if(pred.size() == 1)
          {

            if(current_node->is_Load_Address_Generator())
            {

              int mp = get_mapped_mappairs(pred[0]);
              //  for(int jj=0; jj < (int) succ.size(); jj++)
              //  {
              //    if(succ[jj]->is_Load_Data_Bus_Read())
              //    {
              //      cout << "current load node: " << current_node->get_ID() << "-->" << succ[jj]->get_ID() << endl;
              //      _FATAL("This should not happen.. Inspect, load addr should be mapped before...\n");
              //    }
              //  }

              Load_nodes.push_back(current_node); 


              int row=-2, index=-1;

              for(int jj=0; jj < (int) succ.size(); jj++)  
              {
                if(succ[0]->is_Load_Data_Bus_Read())
                  row = TEC.row_find(succ[0]->get_ID()) % SizeX;
                else
                  index = jj; 
              }


              //if(row==-1) // outgoing load bus read was not mapped.
              //{
              //   left_nodes_unmapped.push_back(current_node);
              //   continue;
              // }

              vector<int> potential_pos, mapped_nodes;
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;


                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());
                // check the row_idx to find the row in this cycle that has been occupied, and select a different row.

                bool found_mem=false;

                if(mapped_nodes.size() > 0)
                  for(int jj=0; jj < nodeSize; jj++)
                    for(int kk=0; kk < (int) mapped_nodes.size(); kk++)
                      if(_node_Set[jj]->get_ID() == mapped_nodes[kk])
                        if(_node_Set[jj]->is_Store_Address_Generator() || _node_Set[jj]->is_Load_Address_Generator())
                          found_mem=true;

                if(!found_mem)
                {
                  //if(row==-2)
                  //{
                  //  if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
                  //    potential_pos.push_back(indices[ii]);
                  // }
                  // else
                  //{
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[get_mapped_mappairs(succ[index])]->getPE()) && connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
                    if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                      potential_pos.push_back(indices[ii]);
                  //}
                }


                /*if(row == -2) // this means the outgoing mapped did not have load data bus read
                  {
                  if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
                  {
                  if(mapped_nodes.size() > 0)
                  {
                  for(int ii=0; ii<(int) mapped_nodes.size(); ii++)
                  {
                  for(int jj=0; jj < nodeSize; jj++)
                  {
                  if(_node_Set[jj]->get_ID() == mapped_nodes[ii])
                  {
                  if(!_node_Set[jj]->is_Load_Data_Bus_Read() && !_node_Set[jj]->is_Store_Address_Generator()) //add connectedPEs for out>0
                  {
                  potential_pos.push_back(indices[ii]);
                  break;
                  }
                  }
                  }
                  }

                  }
                  else
                  potential_pos.push_back(indices[ii]);
                  }
                  }
                  else // this means outgoing mapped had load data bus read and we found the mapped row.
                  {
                  if(connectedPEs(mapPairs[get_mapped_mappairs(succ[index])]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
                  {
                  if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                  {
                  if(mapped_nodes.size() > 0)
                  {
                  for(int ii=0; ii<(int) mapped_nodes.size(); ii++)
                  {
                  for(int jj=0; jj < nodeSize; jj++)
                  {
                  if(_node_Set[jj]->get_ID() == mapped_nodes[ii])
                  {
                  if(!_node_Set[jj]->is_Load_Data_Bus_Read() && !_node_Set[jj]->is_Store_Address_Generator()) //add connectedPEs for out>0
                  {
                  potential_pos.push_back(indices[ii]);
                  break;
                  }
                  }
                  }
                  }

                  }
                  else
                  potential_pos.push_back(indices[ii]);
                  }
                  }
                  }*/
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for all 3. \n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }

              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());


              /*if(row_idx.size() == 0)
                {
                row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
                M.set_row_zero(*i);
                M.set_value(*i, sel, 1);
                AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
                }
              // this if condition is for multiple load addr gen nodes. 
              else if(!contains_row(row_idx, mapPairs[sel]->getPE()->getIndexI()))
              {
              // if the node is a load address, it can take up one PE in one row.
              // no two load nodes can take up a single row.
              // this is checked by having selected PE,time and a row of the PE and time.
              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
              }
              else
              {
              cout << "0 input mapped for node: " << current_node->get_ID() << endl;
              cout << "input mapped 1 and load addr generator for node: " << current_node->get_ID() << "  with row_idx size: " << (int) row_idx.size() << endl;
              for(int jj=0;jj<(int)row_idx.size(); jj++)
              cout << "row_idx:" << row_idx[jj] << endl;

              for(int jj=0;jj<(int) potential_pos.size(); jj++)
              cout << "potential pos: " << potential_pos[jj] << endl;
              cout << "sel: " << sel << endl;
              cout << "mppairs: " << mapPairs[sel]->getPE()->getIndexI() << endl;
              _FATAL("Should not come here. This means the load addr balacing per cycles was not done\n"); //this is fatal because the load balancing should be done during scheduling. 
              }*/

            }
            else if(current_node->is_Load_Data_Bus_Read())
            {
              // for load bus reads it be mapped to any PE but the row should be same as Load addr. 

              //int pos=-1; 
              //for(int ii=0; ii < (int) _node_Set.size(); ii++) 
              //  if(_node_Set[ii]->get_ID() == pred[0]->get_ID())  
              //    pos++;

              int row=-1; 
              row = TEC.row_find(pred[0]->get_ID()) % SizeX;

              // if the load addr is visited but not mapped, add this node to the unmapped list and continue. 
              //if(row == -1)
              //{
              //  left_nodes_unmapped.push_back(current_node);
              //  continue;
              // }

              vector<int> potential_pos, mapped_nodes;
              //int id_row = row_idx[pos];

              for(int ii=0; ii < (int) indices.size(); ii++)
              {
                // check if the PE is free, if not continue to the next position.
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());


                if((int)mapped_nodes.size() > 0)
                {
                  for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                  {
                    if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                      continue;

                    if(row == -1)
                    {
                      if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()))
                        potential_pos.push_back(indices[ii]);
                    }
                    else
                    {
                      if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()))
                        if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                          potential_pos.push_back(indices[ii]);
                    }
                  }
                }
                else
                {
                  if(row == -1)
                  {
                    if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()))
                      potential_pos.push_back(indices[ii]);
                  }
                  else
                  {

                    if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()))
                      if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                        potential_pos.push_back(indices[ii]);
                  }
                }

                /*if(mapPairs[indices[ii]]->getPE()->getIndexI() == row) // row_idx[pos]
                  {
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()))
                  {
                  if(mapped_nodes.size() > 0)
                  {
                  for(int kk=0; kk<nodeSize; kk++)
                  {
                  for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                  {
                  if(_node_Set[kk]->get_ID() == mapped_nodes[jj])
                  {
                  if(!_node_Set[kk]->is_Load_Data_Bus_Read() && !_node_Set[kk]->is_Store_Data_Bus_Write() && !_node_Set[kk]->is_Store_Address_Generator()) //add connectedPEs for out>0
                  {
                  potential_pos.push_back(indices[ii]); 
                  break;
                  }
                  }
                  }
                  }
                  }
                  else
                  potential_pos.push_back(indices[ii]);
                  }
                  }*/
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              { 
                //cout << "No potential postions for 1 input load generator and data bus\n"; 
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());  
            }
            else // all the other nodes with one input operation mapped.  
            {    
              int mp = get_mapped_mappairs(pred[0]);
              vector<int> potential_pos;

              for(int ii=0; ii < (int) indices.size(); ii++)
              {
                // check if the PE is free, if not continue to the next position.
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()))
                  potential_pos.push_back(indices[ii]); 
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()]; 
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {   
                //cout << "No potential postions for 3 outgoing and 1 incoming node: " << current_node->get_ID() << endl;
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }

          }
          else if(pred.size() == 2)
          {
            int mp = get_mapped_mappairs(pred[0]);
            int mp1 = get_mapped_mappairs(pred[1]);

            vector<int> potential_pos;

            for(int ii=0; ii < (int) indices.size(); ii++)
            {
              // check if the PE is free, if not continue to the next position.
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                continue;      

              if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()))
                potential_pos.push_back(indices[ii]);
            }

            if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
            else if(potential_pos.size() == 1)
              sel = potential_pos[0];
            else
            {
              //cout << "No potential postions for 3 outgoing and 2 incoming node: " << current_node->get_ID() << endl;
              left_nodes_unmapped.push_back(current_node);
              continue;
            }
            M.set_row_zero(*i);
            M.set_value(*i, sel, 1);
            AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
          }
          else if(pred.size() == 3)
          {
            int mp = get_mapped_mappairs(pred[0]);
            int mp1 = get_mapped_mappairs(pred[1]);
            int mp2 = get_mapped_mappairs(pred[2]);

            vector<int> potential_pos;

            for(int ii=0; ii < (int) indices.size(); ii++)
            {

              // check if the PE is free, if not continue to the next position.
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp2]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE())) 
                potential_pos.push_back(indices[ii]);
            }


            if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
            else if(potential_pos.size() == 1)
              sel = potential_pos[0];
            else
            {
              //cout << "No potential postions for all 3. \n";
              left_nodes_unmapped.push_back(current_node);
              continue;
            }
            M.set_row_zero(*i);
            M.set_value(*i, sel, 1);
            AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
          }
          else
            _FATAL("should not come here. In incoming nodes == 3\n");
        }
        else if(succ.size() == 3)
        { 
          int out_mp = get_mapped_mappairs(succ[0]); 
          int out_mp1 = get_mapped_mappairs(succ[1]); 
          int out_mp2 = get_mapped_mappairs(succ[2]);
          if(pred.size() == 0)
          {
            /*If the DFG exhibits three outgoing nodes from a load addr node add a section*/

            if(current_node->is_Load_Address_Generator())
            {
              _FATAL("Add this section if the DFG exhibits this.... --> Load addr with 3 outgoing mapped\n");
              /*for(int jj=0; jj < (int) succ.size(); jj++)
                {
                if(succ[jj]->is_Load_Data_Bus_Read())
                {
                cout << "current load node: " << current_node->get_ID() << "-->" << succ[jj]->get_ID() << endl;
                _FATAL("Should not come here. Inspect!! Load bus read should be mapped after load addr.\n");
                }

                }

                Load_nodes.push_back(current_node);
                vector<int> potential_pos;

                for(int ii=0; ii < (int) indices.size(); ii++)
                {
              // check if the PE is free, if not continue to the next position.
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

              if((int)row_idx.size() > 0)
              {
              for(int it=0; it < (int) row_idx.size(); it++)
              {
              if(mapPairs[indices[ii]]->getPE()->getIndexI() != row_idx[it])
              {
              if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp1]->getPE(), mapPairs[indices[ii]]->getPE()) &&  connectedPEs(mapPairs[out_mp2]->getPE(), mapPairs[indices[ii]]->getPE()))
              {
              potential_pos.push_back(indices[ii]);
              }
              break;
              }
              }
              }
              else
              {
              if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp2]->getPE(), mapPairs[indices[ii]]->getPE()))
              potential_pos.push_back(indices[ii]);
              }
              }


              if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
              sel = potential_pos[0];
              else
              {
              //cout << "No potential postions for all 3. \n";
              left_nodes_unmapped.push_back(current_node);
              continue;
              }*/

              /*if((int) row_idx.size() == 0)
                {
                row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
                M.set_row_zero(*i);
                M.set_value(*i, sel, 1);
                AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
                }
              // this if condition is for multiple load addr gen nodes. 
              else if(!contains_row(row_idx, mapPairs[sel]->getPE()->getIndexI()))
              {
              //if the node is a load address, it can take up one PE in one row.
              // no two load nodes can take up a single row.
              // this is checked by having selected PE,time and a row of the PE and time.
              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
              }
              else
              {
              cout << "0 input mapped for node: " << current_node->get_ID() << endl;
              cout << "input mapped 1 and load addr generator for node: " << current_node->get_ID() << "  with row_idx size: " << (int) row_idx.size() << endl;
              for(int jj=0;jj<(int)row_idx.size(); jj++)
              cout << "row_idx:" << row_idx[jj] << endl;

              for(int jj=0;jj<(int) potential_pos.size(); jj++)
              cout << "potential pos: " << potential_pos[jj] << endl;
              cout << "sel: " << sel << endl;
              cout << "mppairs: " << mapPairs[sel]->getPE()->getIndexI() << endl;
              _FATAL("Should not come here. This means the load addr balacing per cycles was not done\n"); //this is fatal because the load balancing should be done during scheduling. 
              }*/

            }
            else if(current_node->is_Load_Data_Bus_Read())
            {
              //should not come here. make unvisited and continue.
              //visited[*i] = false;
              //continue;

              vector<int> potential_pos, mapped_nodes;
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());

                if((int)mapped_nodes.size() > 0)
                {
                  for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                  {
                    if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                      continue;

                    if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()))
                      potential_pos.push_back(indices[ii]);
                  }
                }
                else
                {
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()))
                    potential_pos.push_back(indices[ii]);
                }
                /*    if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()))
                      {
                      if(mapped_nodes.size() > 0)
                      {
                      for(int kk=0; kk<nodeSize; kk++)
                      {
                      for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                      {
                      if(_node_Set[kk]->get_ID() == mapped_nodes[jj])
                      {
                      if(!_node_Set[kk]->is_Load_Data_Bus_Read() && !_node_Set[kk]->is_Store_Data_Bus_Write() && !_node_Set[kk]->is_Store_Address_Generator()) //add connectedPEs for out>0
                      {
                      potential_pos.push_back(indices[ii]);
                      break;
                      }
                      }
                      }
                      }
                      }
                      else
                      {              
                      potential_pos.push_back(indices[ii]);
                      }
                      }*/
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
            else // for all other nodes;
            {
              vector<int> potential_pos;

              for(int ii=0; ii< (int) indices.size(); ii++)
              {
                // check if the PE is free, if not continue to the next position.
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp2]->getPE()))
                  potential_pos.push_back(indices[ii]);

              }
              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for all 3. \n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
          }
          else if(pred.size() == 1)
          {
            if(current_node->is_Load_Address_Generator())
            {
              _FATAL("Add this section if the DFG exhibits this.... --> Load addr with 3 outgoing and 1 incoming mapped.\n");
              /*int mp = get_mapped_mappairs(pred[0]);
                for(int jj=0; jj < (int) succ.size(); jj++)
                {
                if(succ[jj]->is_Load_Data_Bus_Read())
                {
                cout << "current load node: " << current_node->get_ID() << "-->" << succ[jj]->get_ID() << endl;
                _FATAL("Should not come here. Inspect!! Load bus read should be mapped after load addr.\n");
                }

                }

                Load_nodes.push_back(current_node);
                vector<int> potential_pos;

                for(int ii=0; ii < (int) indices.size(); ii++)
                {
              // check if the PE is free, if not continue to the next position.
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

              if((int)row_idx.size() > 0)
              {
              for(int it=0; it < (int) row_idx.size(); it++)
              {
              if(mapPairs[indices[ii]]->getPE()->getIndexI() != row_idx[it])
              {
              if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp1]->getPE(), mapPairs[indices[ii]]->getPE()) &&  connectedPEs(mapPairs[out_mp2]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
              {
              potential_pos.push_back(indices[ii]);
              }
              break;
              }
              }
              }
              else
              {
              if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp2]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
              potential_pos.push_back(indices[ii]);
              }
              }

              if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
              sel = potential_pos[0];
              else
              {
              //cout << "No potential postions for all 3. \n";
              left_nodes_unmapped.push_back(current_node);
              continue;
              }*/

              /*if((int) row_idx.size() == 0)
                {
                row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
                M.set_row_zero(*i);
                M.set_value(*i, sel, 1);
                AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
                }
              // this if condition is for multiple load addr gen nodes. 
              else if(!contains_row(row_idx, mapPairs[sel]->getPE()->getIndexI()))
              {
              // if the node is a load address, it can take up one PE in one row.
              // no two load nodes can take up a single row.
              // this is checked by having selected PE,time and a row of the PE and time.
              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
              }
              else
              {
              cout << "0 input mapped for node: " << current_node->get_ID() << endl;
              cout << "input mapped 1 and load addr generator for node: " << current_node->get_ID() << "  with row_idx size: " << (int) row_idx.size() << endl;
              for(int jj=0;jj<(int)row_idx.size(); jj++)
              cout << "row_idx:" << row_idx[jj] << endl;

              for(int jj=0;jj<(int) potential_pos.size(); jj++)
              cout << "potential pos: " << potential_pos[jj] << endl;
              cout << "sel: " << sel << endl;
              cout << "mppairs: " << mapPairs[sel]->getPE()->getIndexI() << endl;
              _FATAL("Should not come here. This means the load addr balacing per cycles was not done\n"); //this is fatal because the load balancing should be done during scheduling. 
              }*/

            }    
            else if(current_node->is_Load_Data_Bus_Read())
            {
              int mp = get_mapped_mappairs(pred[0]);
              //int pos=-1; 
              //for(int ii=0; ii < (int) _node_Set.size(); ii++) 
              // if(_node_Set[ii]->get_ID() == pred[0]->get_ID())  
              //  pos++;

              vector<int> potential_pos, mapped_nodes;
              //int id_row = row_idx[pos];
              int row = -1;
              row = TEC.row_find(pred[0]->get_ID()) % SizeX;

              // if the load addr is visited but not mapped, add this node to the unmapped list and continue. if the row -1 then the load addr was not MAPPED. 
              //if(row == -1)
              // {
              //   left_nodes_unmapped.push_back(current_node);
              //   continue;
              // }


              for(int ii=0; ii < (int) indices.size(); ii++)
              {
                // check if the PE is free, if not continue to the next position.
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());

                if((int)mapped_nodes.size() > 0)
                {
                  for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                  {
                    if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                      continue;

                    if(row==-1)
                    {
                      if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()))
                        potential_pos.push_back(indices[ii]);
                    }
                    else
                    {
                      if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()))
                        if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                          potential_pos.push_back(indices[ii]);

                    }
                  }
                }
                else
                {
                  if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()))
                    if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                      potential_pos.push_back(indices[ii]);

                }

                /* if(mapPairs[indices[ii]]->getPE()->getIndexI() == row) //row_idx[pos]
                   { 
                   if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()))
                   {
                   if(mapped_nodes.size() > 0)
                   {
                   for(int kk=0; kk<nodeSize; kk++)
                   {
                   for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                   {
                   if(_node_Set[kk]->get_ID() == mapped_nodes[jj])
                   {
                   if(!_node_Set[kk]->is_Load_Data_Bus_Read() && !_node_Set[kk]->is_Store_Data_Bus_Write() && !_node_Set[kk]->is_Store_Address_Generator()) //add connectedPEs for out>0
                   {
                   potential_pos.push_back(indices[ii]);
                   break;
                   }
                   }
                   }
                   }

                   }
                   else
                   potential_pos.push_back(indices[ii]);
                   }
                   }*/
              }


              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              { 
                //cout << "No potential postions for 1 input load generator and data bus\n"; 
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);

              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());  
            }
            else // all the other nodes with one input operations.  
            {   
              int mp = get_mapped_mappairs(pred[0]);
              vector<int> potential_pos;

              for(int ii=0; ii < (int) indices.size(); ii++)
              {
                // check if the PE is free, if not continue to the next position.
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;


                if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()))
                  potential_pos.push_back(indices[ii]); 
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()]; 
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {   
                //cout << "No potential postions for 3 outgoing and 1 incoming node: " << current_node->get_ID() << endl;
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
          }
          else if(pred.size() == 2)
          {
            int mp = get_mapped_mappairs(pred[0]);
            int mp1 = get_mapped_mappairs(pred[1]);

            vector<int> potential_pos;

            for(int ii=0; ii < (int) indices.size(); ii++)
            {
              // check if the PE is free, if not continue to the next position.
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()))
                potential_pos.push_back(indices[ii]);
            }

            if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
            else if(potential_pos.size() == 1)
              sel = potential_pos[0];
            else
            {
              //cout << "No potential postions for 3 outgoing and 2 incoming node: " << current_node->get_ID() << endl;
              left_nodes_unmapped.push_back(current_node);
              continue;
            }
            M.set_row_zero(*i);
            M.set_value(*i, sel, 1);
            AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
          }
          else if(pred.size() == 3)
          {
            int mp = get_mapped_mappairs(pred[0]); 
            int mp1 = get_mapped_mappairs(pred[1]);
            int mp2 = get_mapped_mappairs(pred[2]);

            vector<int> potential_pos;

            for(int ii=0; ii < (int) indices.size(); ii++)
            {
              // check if the PE is free, if not continue to the next position.
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp2]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE())) 
                potential_pos.push_back(indices[ii]);
            }


            if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
            else if(potential_pos.size() == 1)
              sel = potential_pos[0];
            else
            {
              //cout << "No potential postions for all 3. \n";
              left_nodes_unmapped.push_back(current_node);
              continue;
            }
            M.set_row_zero(*i);
            M.set_value(*i, sel, 1);
            AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
          }
          else
            _FATAL("should not come here. In outgoing nodes == 3\n");
        }
        else if(succ.size() == 4)
        { 
          int out_mp = get_mapped_mappairs(succ[0]); 
          int out_mp1 = get_mapped_mappairs(succ[1]); 
          int out_mp2 = get_mapped_mappairs(succ[2]);
          int out_mp3 = get_mapped_mappairs(succ[3]);          

          if(pred.size() == 0)
          { 
            if(current_node->is_Load_Address_Generator())
            {
              _FATAL("Add this section if the DFG exhibits this.... --> Load outgoing 4 and incoming 0 mapped.\n");
              /*for(int jj=0; jj < (int) succ.size(); jj++)
                {
                if(succ[jj]->is_Load_Data_Bus_Read())
                {
                cout << "current load node: " << current_node->get_ID() << "---> " << succ[jj]->get_ID() << endl;
                _FATAL("The load bus should always be mapped after the load addr.... Check the nodes!!\n");

                }
                }

                Load_nodes.push_back(current_node);
              //sel = rand() % resources; // redo this section.  
              //row_id = sel % SizeX ;

              vector<int> potential_pos;

              for(int ii=0; ii < (int) indices.size(); ii++)
              {
              // check if the PE is free, if not continue to the next position.
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

              if((int)row_idx.size() > 0)
              {
              for(int it=0; it < (int) row_idx.size(); it++)
              {
              if(mapPairs[indices[ii]]->getPE()->getIndexI() != row_idx[it])
              {
              if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp2]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp3]->getPE(), mapPairs[indices[ii]]->getPE()))
              {
              potential_pos.push_back(indices[ii]);
              }
              break;
              }
              }
              }
              else
              {
              if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp2]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp3]->getPE(), mapPairs[indices[ii]]->getPE()))
              potential_pos.push_back(indices[ii]);
              }
              }


              if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
              sel = potential_pos[0];
              else
              {
              //cout << "No potential postions for all 3. \n";
              left_nodes_unmapped.push_back(current_node);
              continue;
              }

              if(row_idx.size() == 0)
              {
              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
              }
              // this if condition is for multiple load addr gen nodes. 
              else if(!contains_row(row_idx, mapPairs[sel]->getPE()->getIndexI()))
              {
              // if the node is a load address, it can take up one PE in one row.
              // no two load nodes can take up a single row.
              // this is checked by having selected PE,time and a row of the PE and time.
              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
              else
              {
                cout << "0 input mapped for node: " << current_node->get_ID() << endl;
                cout << "input mapped 1 and load addr generator for node: " << current_node->get_ID() << "  with row_idx size: " << (int) row_idx.size() << endl;
                for(int jj=0;jj<(int)row_idx.size(); jj++)
                  cout << "row_idx:" << row_idx[jj] << endl;

                for(int jj=0;jj<(int) potential_pos.size(); jj++)
                  cout << "potential pos: " << potential_pos[jj] << endl;
                cout << "sel: " << sel << endl;
                cout << "mppairs: " << mapPairs[sel]->getPE()->getIndexI() << endl;
                _FATAL("Should not come here. This means the load addr balacing per cycles was not done\n"); //this is fatal because the load balancing should be done during scheduling. 
              }*/

            }
            else if(current_node->is_Load_Data_Bus_Read())
            {
              //visited[*i] = false;
              //continue;

              vector<int> potential_pos, mapped_nodes;
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());

                if((int)mapped_nodes.size() > 0)
                {
                  for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                  {
                    if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                      continue;

                    if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()))
                      potential_pos.push_back(indices[ii]);
                  }
                }
                else
                {
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()))
                    potential_pos.push_back(indices[ii]);
                }

                /*if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()))
                  {
                  if(mapped_nodes.size() > 0)
                  {
                  for(int kk=0; kk<nodeSize; kk++)
                  {
                  for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                  {
                  if(_node_Set[kk]->get_ID() == mapped_nodes[jj])
                  {
                  if(!_node_Set[kk]->is_Load_Data_Bus_Read() && !_node_Set[kk]->is_Store_Data_Bus_Write() && !_node_Set[kk]->is_Store_Address_Generator()) //add connectedPEs for out>0
                  {
                  potential_pos.push_back(indices[ii]);
                  break;
                  }
                  }
                  }
                  }
                  }
                  else
                  {              
                  potential_pos.push_back(indices[ii]);
                  }
                  }*/
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());

            }
            else // for all other nodes.
            {
              vector<int> potential_pos;           

              for(int ii=0; ii < (int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()))
                  potential_pos.push_back(indices[ii]);

              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for all 3. \n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }

              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID()); 
            }
          } // end of incoming size == 0 for 4 outgo
          else if(pred.size() == 1)
          {
            if(current_node->is_Load_Address_Generator())   
            {
              _FATAL("Add this section if the DFG exhibits this.... --> Load 4 outgoing and 1 incoming\n");
              /*int mp = get_mapped_mappairs(pred[0]);
                for(int jj=0; jj < (int) succ.size(); jj++)
                {
                if(succ[jj]->is_Load_Data_Bus_Read())
                {
                cout << "current load node: " << current_node->get_ID() << "---> " << succ[jj]->get_ID() << endl;
                _FATAL("The load bus should always be mapped after the load addr.... Check the nodes!!\n");

                }
                }

                Load_nodes.push_back(current_node);
              //sel = rand() % resources; // redo this section.  
              //row_id = sel % SizeX ;

              vector<int> potential_pos;

              for(int ii=0; ii < (int) indices.size(); ii++)
              {
              // check if the PE is free, if not continue to the next position.
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

              if((int)row_idx.size() > 0)
              {
              for(int it=0; it < (int) row_idx.size(); it++)
              {
              if(mapPairs[indices[ii]]->getPE()->getIndexI() != row_idx[it])
              {
              if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp2]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp3]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE())) 
              {
              potential_pos.push_back(indices[ii]);
              }
              break;
              }
              }
              }
              else
              {
              if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp2]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp3]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
              potential_pos.push_back(indices[ii]);
              }
              }

              if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
              sel = potential_pos[0];
              else
              {
              //cout << "No potential postions for all 3. \n";
              left_nodes_unmapped.push_back(current_node);
              continue;
              }

              if(row_idx.size() == 0)
              {
              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
              }
              // this if condition is for multiple load addr gen nodes. 
              else if(!contains_row(row_idx, mapPairs[sel]->getPE()->getIndexI()))
              {
              // if the node is a load address, it can take up one PE in one row.
              // no two load nodes can take up a single row.
              // this is checked by having selected PE,time and a row of the PE and time.
              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
              else
              {
                cout << "0 input mapped for node: " << current_node->get_ID() << endl;
                cout << "input mapped 1 and load addr generator for node: " << current_node->get_ID() << "  with row_idx size: " << (int) row_idx.size() << endl;
                for(int jj=0;jj<(int)row_idx.size(); jj++)
                  cout << "row_idx:" << row_idx[jj] << endl;

                for(int jj=0;jj<(int) potential_pos.size(); jj++)
                  cout << "potential pos: " << potential_pos[jj] << endl;
                cout << "sel: " << sel << endl;
                cout << "mppairs: " << mapPairs[sel]->getPE()->getIndexI() << endl;
                _FATAL("Should not come here. This means the load addr balacing per cycles was not done\n"); //this is fatal because the load balancing should be done during scheduling. 
              }*/

            }
            else if(current_node->is_Load_Data_Bus_Read())
            {

              int mp = get_mapped_mappairs(pred[0]);
              //int pos=-1; 
              //for(int ii=0; ii < (int) _node_Set.size(); ii++) 
              //  if(_node_Set[ii]->get_ID() == pred[0]->get_ID())  
              //    pos++;

              int row=-1;
              row = TEC.row_find(pred[0]->get_ID()) % SizeX;

              // if the load addr is visited but not mapped, add this node to the unmapped list and continue. 
              //if(row == -1)
              //{
              //  left_nodes_unmapped.push_back(current_node);
              //  continue;
              // }


              vector<int> potential_pos, mapped_nodes;
              //int id_row = row_idx[pos];

              for(int ii=0; ii < (int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());


                if((int)mapped_nodes.size() > 0)
                {
                  for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                  {
                    if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                      continue;

                    if(row == -1)
                    {
                      if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()))
                        potential_pos.push_back(indices[ii]);
                    }
                    else
                    {
                      if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()))
                        if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                          potential_pos.push_back(indices[ii]);

                    }
                  }
                }
                else
                {
                  if(row == -1)
                  {
                    if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()))
                      potential_pos.push_back(indices[ii]);
                  }
                  else
                  {
                    if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()))
                      if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                        potential_pos.push_back(indices[ii]);

                  }

                }


                /* if(mapPairs[indices[ii]]->getPE()->getIndexI() == row) // row_idx[pos]
                   {
                   if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp2]->getPE()), connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()))
                   {

                   if(mapped_nodes.size() > 0)
                   {
                   for(int kk=0; kk<nodeSize; kk++)
                   {
                   for(int jj=0; jj <(int) mapped_nodes.size(); jj++)
                   {
                   if(_node_Set[kk]->get_ID() == mapped_nodes[jj])
                   {
                   if(!_node_Set[kk]->is_Load_Data_Bus_Read() && !_node_Set[kk]->is_Store_Data_Bus_Write() && !_node_Set[kk]->is_Store_Address_Generator()) //add connectedPEs for out>0
                   {
                   potential_pos.push_back(indices[ii]);
                   break;
                   }
                   }
                   }
                   }
                   }
                   else
                   potential_pos.push_back(indices[ii]);
                   }
                   }*/
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              { 
                //cout << "No potential postions for 1 input load generator and data bus\n"; 
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
            else // all the other nodes with one input operation mapped.  
            {   
              // get the PE and time the pred was mapped from TEC.  
              int mp = get_mapped_mappairs(pred[0]);
              vector<int> potential_pos;

              for(int ii=0; ii < (int) indices.size(); ii++)
              { 
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()))
                  potential_pos.push_back(indices[ii]); 
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()]; 
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {   
                //cout << "No potential postions for 3 outgoing and 1 incoming node: " << current_node->get_ID() << endl;
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }

          }
          else if(pred.size() == 2)
          {
            int mp= get_mapped_mappairs(pred[0]);
            int mp1 = get_mapped_mappairs(pred[1]);

            vector<int> potential_pos;

            for(int ii=0; ii < (int) indices.size(); ii++)
            {
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()))
                potential_pos.push_back(indices[ii]);
            }
            if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
            else if(potential_pos.size() == 1)
              sel = potential_pos[0];
            else
            {
              //cout << "No potential postions for 3 outgoing and 2 incoming node: " << current_node->get_ID() << endl;
              left_nodes_unmapped.push_back(current_node);
              continue;
            }
            M.set_row_zero(*i);
            M.set_value(*i, sel, 1);
            AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
          }
          else if(pred.size() == 3)
          {
            int mp = get_mapped_mappairs(pred[0]); 
            int mp1 = get_mapped_mappairs(pred[1]);
            int mp2 = get_mapped_mappairs(pred[2]);

            vector<int> potential_pos;

            for(int ii=0; ii < (int) indices.size(); ii++)
            {
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp2]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(), mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE())) 
                potential_pos.push_back(indices[ii]);
            }


            if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
            else if(potential_pos.size() == 1)
              sel = potential_pos[0];
            else
            {
              //cout << "No potential postions for all 3. \n";
              left_nodes_unmapped.push_back(current_node);
              continue;
            }
            M.set_row_zero(*i);
            M.set_value(*i, sel, 1);
            AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
          }
          else
            _FATAL("should not come here. In outgoing nodes == 3\n");
        }
        else if(succ.size() == 5)
        { 
          int out_mp = get_mapped_mappairs(succ[0]); 
          int out_mp1 = get_mapped_mappairs(succ[1]); 
          int out_mp2 = get_mapped_mappairs(succ[2]);
          int out_mp3 = get_mapped_mappairs(succ[3]);
          int out_mp4 = get_mapped_mappairs(succ[4]);          

          if(pred.size() == 0)
          {
            /* if you find load addr with this config, add code*/

            if(current_node->is_Load_Address_Generator())
            {   
              _FATAL("Add this section if the DFG exhibits this.... --> Load 5 outgoing and 0 incoming\n");
              /*for(int jj=0; jj < (int) succ.size(); jj++)
                {
                if(succ[jj]->is_Load_Data_Bus_Read())
                {
                cout << "current load node: " << current_node->get_ID() << "---> " << succ[jj]->get_ID() << endl;
                _FATAL("The load bus should always be mapped after the load addr.... Check the nodes!!\n");

                }
                }

                Load_nodes.push_back(current_node);
              //sel = rand() % resources; // redo this section.  
              //row_id = sel % SizeX ;

              vector<int> potential_pos;

              for(int ii=0; ii < (int) indices.size(); ii++)
              {
              // check if the PE is free, if not continue to the next position.
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

              if((int)row_idx.size() > 0)
              {
              for(int it=0; it < (int) row_idx.size(); it++)
              {
              if(mapPairs[indices[ii]]->getPE()->getIndexI() != row_idx[it])
              {
              if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp2]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp3]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp4]->getPE(), mapPairs[indices[ii]]->getPE()))
              {
              potential_pos.push_back(indices[ii]);
              }
              break;
              }
              }
              }
              else
              {
              if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp2]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp3]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp4]->getPE(), mapPairs[indices[ii]]->getPE()))
              potential_pos.push_back(indices[ii]);
              }
              }


              if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
              sel = potential_pos[0];
              else
              {
              //cout << "No potential postions for all 3. \n";
              left_nodes_unmapped.push_back(current_node);
              continue;
              }

              if(row_idx.size() == 0)
              {
              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
              }
              // this if condition is for multiple load addr gen nodes. 
              else if(!contains_row(row_idx, mapPairs[sel]->getPE()->getIndexI()))
              {
              ///if the node is a load address, it can take up one PE in one row.
              // no two load nodes can take up a single row.
              // this is checked by having selected PE,time and a row of the PE and time.
              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
              else
              {
                cout << "0 input mapped for node: " << current_node->get_ID() << endl;
                cout << "input mapped 1 and load addr generator for node: " << current_node->get_ID() << "  with row_idx size: " << (int) row_idx.size() << endl;
                for(int jj=0;jj<(int)row_idx.size(); jj++)
                  cout << "row_idx:" << row_idx[jj] << endl;

                for(int jj=0;jj<(int) potential_pos.size(); jj++)
                  cout << "potential pos: " << potential_pos[jj] << endl;
                cout << "sel: " << sel << endl;
                cout << "mppairs: " << mapPairs[sel]->getPE()->getIndexI() << endl;
                _FATAL("Should not come here. This means the load addr balacing per cycles was not done\n"); //this is fatal because the load balancing should be done during scheduling. 
              } */

            }
            else if(current_node->is_Load_Data_Bus_Read())
            {
              //visited[*i] = false;
              //continue;

              vector<int> potential_pos, mapped_nodes;
              for(int ii=0; ii<(int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());


                if((int)mapped_nodes.size() > 0)
                {
                  for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                  {
                    if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                      continue;

                    if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp4]->getPE()))
                      potential_pos.push_back(indices[ii]);
                  }
                }
                else
                {
                  if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp4]->getPE()))
                    potential_pos.push_back(indices[ii]);
                }

                /* if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp4]->getPE()))
                   {
                   if(mapped_nodes.size() > 0)
                   {
                   for(int kk=0; kk<nodeSize; kk++)
                   {
                   for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                   {
                   if(_node_Set[kk]->get_ID() == mapped_nodes[jj])
                   {
                   if(!_node_Set[kk]->is_Load_Data_Bus_Read() && !_node_Set[kk]->is_Store_Data_Bus_Write() && !_node_Set[kk]->is_Store_Address_Generator()) //add connectedPEs for out>0
                   {
                   potential_pos.push_back(indices[ii]);
                   break;
                   }
                   }
                   }
                   }
                   }
                   else
                   {              
                   potential_pos.push_back(indices[ii]);
                   }
                   }*/
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 1 input load generator and data bus\n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            } 
            else //for all other nodes;
            {
              vector<int> potential_pos;

              for(int ii=0; ii < (int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp4]->getPE()))
                  potential_pos.push_back(indices[ii]);

              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {
                //cout << "No potential postions for 5 outgoing 0 incoming. \n";
                left_nodes_unmapped.push_back(current_node);
                continue;
              }

              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
          } // end of incoming size == 0
          else if(pred.size() == 1)
          {
            if(current_node->is_Load_Address_Generator())
            {
              _FATAL("Add this section if the DFG exhibits this.... --> Load 5 outgoing and 1 incoming mapped.\n");
              /*int mp = get_mapped_mappairs(pred[0]);
                for(int jj=0; jj < (int) succ.size(); jj++)
                {
                if(succ[jj]->is_Load_Data_Bus_Read())
                {
                cout << "current load node: " << current_node->get_ID() << "---> " << succ[jj]->get_ID() << endl;
                _FATAL("The load bus should always be mapped after the load addr.... Check the nodes!!\n");

                }
                }

                Load_nodes.push_back(current_node);
              //sel = rand() % resources; // redo this section.  
              //row_id = sel % SizeX ;

              vector<int> potential_pos;

              for(int ii=0; ii < (int) indices.size(); ii++)
              {
              // check if the PE is free, if not continue to the next position.
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
              continue;

              if((int)row_idx.size() > 0)
              {
              for(int it=0; it < (int) row_idx.size(); it++)
              {
              if(mapPairs[indices[ii]]->getPE()->getIndexI() != row_idx[it])
              {
              if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp2]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp3]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp4]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
              {
              potential_pos.push_back(indices[ii]);
              }
              break;
              }
              }
              }
              else
              {
              if(connectedPEs(mapPairs[out_mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp2]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp3]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[out_mp4]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()))
              potential_pos.push_back(indices[ii]);
              }
              }

              if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
              sel = potential_pos[0];
              else
              {
              //cout << "No potential postions for all 3. \n";
              left_nodes_unmapped.push_back(current_node);
              continue;
              }

              if(row_idx.size() == 0)
              {
              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
              }
              // this if condition is for multiple load addr gen nodes. 
              else if(!contains_row(row_idx, mapPairs[sel]->getPE()->getIndexI()))
              {
              //if the node is a load address, it can take up one PE in one row.
              // no two load nodes can take up a single row.
              // this is checked by having selected PE,time and a row of the PE and time.
              row_idx.push_back(mapPairs[sel]->getPE()->getIndexI());
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }
              else
              {
                cout << "0 input mapped for node: " << current_node->get_ID() << endl;
                cout << "input mapped 1 and load addr generator for node: " << current_node->get_ID() << "  with row_idx size: " << (int) row_idx.size() << endl;
                for(int jj=0;jj<(int)row_idx.size(); jj++)
                  cout << "row_idx:" << row_idx[jj] << endl;

                for(int jj=0;jj<(int) potential_pos.size(); jj++)
                  cout << "potential pos: " << potential_pos[jj] << endl;
                cout << "sel: " << sel << endl;
                cout << "mppairs: " << mapPairs[sel]->getPE()->getIndexI() << endl;
                _FATAL("Should not come here. This means the load addr balacing per cycles was not done\n"); //this is fatal because the load balancing should be done during scheduling. 
              }*/
            }
            else if(current_node->is_Load_Data_Bus_Read())
            {
              //int pos=-1; 
              //for(int ii=0; ii < (int) _node_Set.size(); ii++) 
              //  if(_node_Set[ii]->get_ID() == pred[0]->get_ID())  
              //    pos++;
              int row = -1;
              row = TEC.row_find(pred[0]->get_ID()) % SizeX;

              // if the load addr is visited but not mapped, add this node to the unmapped list and continue. 
              //if(row == -1)
              // {
              //   left_nodes_unmapped.push_back(current_node);
              //   continue;
              // }

              vector<int> potential_pos, mapped_nodes;
              //int id_row = row_idx[pos];
              for(int ii=0; ii < (int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;

                mapped_nodes.clear();
                mapped_nodes = get_nodes_mapped_to_row((mapPairs[indices[ii]]->getPE()->getIndexT()*SizeX)+mapPairs[indices[ii]]->getPE()->getIndexI());

                if((int)mapped_nodes.size() > 0)
                {
                  for(int jj=0; jj < (int) mapped_nodes.size(); jj++)
                  {
                    if(get_Node(mapped_nodes[jj])->is_Load_Data_Bus_Read() || get_Node(mapped_nodes[jj])->is_Store_Data_Bus_Write() || get_Node(mapped_nodes[jj])->is_Store_Address_Generator())
                      continue;

                    if(row==-1)
                    {
                      if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp4]->getPE()))
                        potential_pos.push_back(indices[ii]);
                    }
                    else
                    {
                      if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp4]->getPE()))
                        if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                          potential_pos.push_back(indices[ii]);
                    }
                  }
                }
                else
                {
                  if(row==-1)
                  {
                    if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp4]->getPE()))
                      potential_pos.push_back(indices[ii]);
                  }
                  else
                  {
                    if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp4]->getPE()))
                      if(mapPairs[indices[ii]]->getPE()->getIndexI() == row)
                        potential_pos.push_back(indices[ii]);
                  }
                }


                /*  if(mapPairs[indices[ii]]->getPE()->getIndexI() == row) //row_idx[pos]
                    {
                    if(connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()), connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp4]->getPE()))
                    {
                    if(mapped_nodes.size() > 0)
                    {
                    for(int kk=0; kk<nodeSize; kk++)
                    {
                    for(int jj=0; jj < (int) mapped_nodes.size() ; jj++)
                    {
                    if(_node_Set[kk]->get_ID() == mapped_nodes[jj])
                    {
                    if(!_node_Set[kk]->is_Load_Data_Bus_Read() && !_node_Set[kk]->is_Store_Data_Bus_Write() && !_node_Set[kk]->is_Store_Address_Generator()) //add connectedPEs for out>0
                    {
                    potential_pos.push_back(indices[ii]);
                    break;
                    }
                    }
                    }
                    }
                    }
                    else
                    potential_pos.push_back(indices[ii]);
                    }
                    }*/
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()];
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              { 
                //cout << "No potential postions for 1 input load generator and data bus\n"; 
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());  
            }
            else // all the other nodes with one input operations.  
            {   
              int mp = get_mapped_mappairs(pred[0]);
              vector<int> potential_pos;

              for(int ii=0; ii < (int) indices.size(); ii++)
              {
                if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                  continue;            

                if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp4]->getPE()))
                  potential_pos.push_back(indices[ii]); 
              }

              if(potential_pos.size() > 1)
                sel = potential_pos[rand() % (int) potential_pos.size()]; 
              else if(potential_pos.size() == 1)
                sel = potential_pos[0];
              else
              {   
                //cout << "No potential postions for 5 outgoing and 1 incoming node: " << current_node->get_ID() << endl;
                left_nodes_unmapped.push_back(current_node);
                continue;
              }
              M.set_row_zero(*i);
              M.set_value(*i, sel, 1);
              AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
            }

          }
          else if(pred.size() == 2)
          {
            // get coordinates of pred[0]
            int mp= get_mapped_mappairs(pred[0]);
            int mp1 = get_mapped_mappairs(pred[1]);
            // get coordinates of pred[1]

            vector<int> potential_pos;

            for(int ii=0; ii < (int) indices.size(); ii++)
            {
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp4]->getPE()))
                potential_pos.push_back(indices[ii]);
            }

            if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
            else if(potential_pos.size() == 1)
              sel = potential_pos[0];
            else
            {
              //cout << "No potential postions for 3 outgoing and 2 incoming node: " << current_node->get_ID() << endl;
              left_nodes_unmapped.push_back(current_node);
              continue;
            }
            M.set_row_zero(*i);
            M.set_value(*i, sel, 1);
            AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
          }
          else if(pred.size() == 3)
          {
            // get coordinates of pred
            int mp = get_mapped_mappairs(pred[0]); 
            int mp1 = get_mapped_mappairs(pred[1]);
            int mp2 = get_mapped_mappairs(pred[2]);

            vector<int> potential_pos;


            for(int ii=0; ii < (int) indices.size(); ii++)
            {
              if(!isTECfree(mapPairs[indices[ii]]->getPE()->getIndexI(), mapPairs[indices[ii]]->getPE()->getIndexJ(), mapPairs[indices[ii]]->getPE()->getIndexT()))
                continue;

              if(connectedPEs(mapPairs[mp]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp1]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[mp2]->getPE(), mapPairs[indices[ii]]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp1]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp2]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp3]->getPE()) && connectedPEs(mapPairs[indices[ii]]->getPE(),mapPairs[out_mp4]->getPE())) 
                potential_pos.push_back(indices[ii]);
            }

            if(potential_pos.size() > 1)
              sel = potential_pos[rand() % (int) potential_pos.size()];
            else if(potential_pos.size() == 1)
              sel = potential_pos[0];
            else
            {
              //cout << "No potential postions for all 3. \n";
              left_nodes_unmapped.push_back(current_node);
              continue;
            }
            M.set_row_zero(*i);
            M.set_value(*i, sel, 1);
            AssignTEC(mapPairs[sel]->getPE()->getIndexI(), mapPairs[sel]->getPE()->getIndexJ(), mapPairs[sel]->getPE()->getIndexT(), current_node->get_ID());
          }
          else
            _FATAL("should not come here. In outgoing nodes == 3\n");
        }

        else
          _FATAL("Check this condition, where 5 or more outgoing nodes are mapped.\n");
      } // end of if nor visited
    } // end of for adjacency list
  } // end of while queue not empty
}

bool CGRA::FALCON(DFG* myDFG, vector<Node*> &leftNotMapped)
{
  //a. get adjacency of H (query) A_H
  //b. get adjacency of G (product matrix) A_G
  //c. get M' (from H and G)
  //c1. If the node in H has a matching node_id in G and outgoing arcs in H < or = the node in G then M is 1 otherwise 0.

  //debugfile << "Inside subgraph iso" << endl; 
  //b. THe comptability "table" is the product graph.

  vector<Node*> nodes = myDFG->getSetOfVertices();
  myDFG->Dot_Print_DFG("iso_graph", 1);   

  nodeSize = (int) nodes.size();
  MappingPair* mp1;
  cout<< "Passed A_g init" << endl;
  //bool success = 
  GenerateAdjacencyTable(myDFG, A_h, A_g, M);

  cout << "Generate Adj matrix" << endl;

  print_adj(nodeSize);
  print_node_id(myDFG);  
  //print_adj_node(nodeSize); 
  //exit(1);

  copy_M.reshape(M.rows(), M.cols()); 
  copy_M = M; 

  //bool mapping_found = false;
  TEC.reshape(II * SizeX, SizeY);

  //TEC.print_matrix();
  //exit(1);

  //cout << "inside while" << endl;
  bool refine_success = refine_M(myDFG);

  if(refine_success)
  { 
    for(int i=0; i < nodeSize; i++)
    {
      mp1 = nodes[i]->GetCurrentPosition(); 
      cout << "mo1 Node: " << mp1->getNode()->get_ID() << " PE:<" << mp1->getPE()->getIndexI() << " ,"  << mp1->getPE()->getIndexJ() << ", " << mp1->getPE()->getIndexT() << ">\n";
      mp1->getPE()->AssignNode(mp1->getNode());
    }
    /*for (int i = 0; i < (int) BestIndices.size(); i++)
      {
      mp1 = BestIndices[i];
      cout << "mo1 Node: " << mp1->getNode()->get_ID() << " PE:<" << mp1->getPE()->getIndexI() << " ,"  << mp1->getPE()->getIndexJ() << ", " << mp1->getPE()->getIndexT() << ">\n";  
      mp1->getPE()->AssignNode(mp1->getNode());
      }*/
    return true; 
  }
  else
  {
    for(int i=0; i<(int) left_nodes_unmapped.size(); i++)
      leftNotMapped.push_back(left_nodes_unmapped[i]);
    return false;
  }
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
    debugfile << "Node " << tempNode->get_ID() << " is scheduled at " << tempNode->get_Sched_Info()->get_Current() << endl;
  }

  GenerateCompGraph(myDFG);

  int tableSize = (int) mapPairs.size();

  /*ofstream adjgraph;
    adjgraph.open("graph.txt"); 
    adjgraph << tableSize << "\n";
    for (int i=0; i<tableSize; i++)
    {
    for(int j=0; j<tableSize; j++)
    adjgraph << " " << (int) table[i][j];
    adjgraph << "\n";
    }
    adjgraph.close();*/

  vector<int> indecis;
  vector<Cover_Pair*> BestIndecis;

  Mapper myMapper(SizeReg,&mapPairs,SizeX,SizeY);
  //A vector of mappings that will be generated by the mapper
  vector<vector<Cover_Pair*> > results;

  //PERFORM THE MAPPING
  if (MAPPING_POLICY.ENABLE_REGISTERS)
  {
    results = myMapper.MapWithRegisterFile(table, tableSize, nodeSize, II);
  }
  else
  {
    results = myMapper.MapWithoutRegisterFile(table, tableSize, nodeSize, II);
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

bool CGRA::connected_Nodes2(Node* node1, Node* node2)
{
  if (node1->is_Physically_Connected2_To(node2))
    return true;

  ARC* arc = node1->get_Arc(node2);
  //if(arc!=NULL) cout << "arc type: " << arc->get_Dependency() << endl;

  if(arc!=NULL && arc->get_Dependency() == LoadDep) 
    return true;

  vector<Node*> cons = node1->get_physical_successors_constrain_by_this_modulo(II);
  if (contains(cons, node2))
  {
    return true;
  }

  return false;
}


bool CGRA::Compatible2(MappingPair* mp1, MappingPair* mp2, DFG* &myDFG)
{

  //cout << "mp1: <" << mp1->getNode()->get_ID() << "," << mp1->getPE()->getIndexJ() << "," << mp1->getPE()->getIndexT() << ">"<<endl;
  //cout << "mp2: <" << mp2->getNode()->get_ID() << "," << mp2->getPE()->getIndexJ() << "," << mp2->getPE()->getIndexT() << ">"<<endl; 

  if (mp1->getNode()->get_ID() == mp2->getNode()->get_ID() || mp1->getPE() == mp2->getPE())
    return false;
  if ((connected_Nodes2(mp1->getNode(), mp2->getNode()) && connectedPEs(mp1->getPE(), mp2->getPE())))
    return true;

  //|| (connected_Nodes(mp2->getNode(), mp1->getNode()) && connectedPEs(mp2->getPE(), mp1->getPE())))

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
