/*
 * CGRA.cpp
 *
 *  Created on: Aug 10, 2012
 *      Author: mahdi
 */

//#include "definitions.h"
#include "CGRA.h"

CGRA::CGRA()
{
  iteration = 0;
  //	Threshold = 0;
  SizeReg = 0;
  SizeX = 0;
  SizeY = 0;
  II = 0;
  ConType = (REGI_ConnectionType) 0;

}

CGRA::~CGRA()
{
  int size = timeCGRAList.size();
  for (int i = 0; i < size; i++)
  {
    delete (timeCGRAList[i]);
  }
}

void CGRA::Initialize(int SizeX, int SizeY, int SizeReg, REGI_ConnectionType ConType)
{

  this->SizeX = SizeX;
  this->SizeY = SizeY;
  this->SizeReg = SizeReg;
  this->ConType = ConType;

}

void CGRA::IncreaseII()
{
  II++;
  for (int i = 0; i < (int) timeCGRAList.size(); i++)
  {

    timeCGRAList[i]->Reset();

  }
  TimeCGRA newCGRA(SizeX, SizeY, SizeReg, II, ConType);
  timeCGRAList.push_back(&newCGRA);
}

void CGRA::SetII(int II)
{
  this->II = II;
  TimeCGRA* newCGRA;

  //PEs in the first timing slot are special, keep track of register

  for (int i = 0; i < II; i++)
  {
    newCGRA = new TimeCGRA(SizeX, SizeY, SizeReg, i, ConType);
    timeCGRAList.push_back(newCGRA);
  }
}

void CGRA::ShowMapping()
{

  std::ofstream kernelFile;
  std::string kernel = "kernel";
  kernel.append(".sch");
  kernelFile.open(kernel.c_str());
  printf("*********************************Kernel Start*********************************\n");
  for (int timeIndex = 0; timeIndex < II; timeIndex++)
  {
    TimeCGRA* cgraTime = timeCGRAList[timeIndex];
    REGI_PE* candidatePE;
    cout << "Time:" << timeIndex << endl;
    for (int i = 0; i < SizeX; i++)
    {
      for (int j = 0; j < SizeY; j++)
      {
        candidatePE = cgraTime->getPE(i, j);
        if (candidatePE->isBusy())
        {
          printf("%10d(%d)", candidatePE->getNode()->get_ID(), candidatePE->NumberOfUsedRegisters());
          kernelFile << candidatePE->getNode()->get_ID()<<"\t"<<candidatePE->NumberOfUsedRegisters() <<"\t"<< candidatePE->getNode()->get_Sched_Info()->get_Current() << "\n";
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

}

void CGRA::DotPrintMapping(string filename)
{
  ofstream dotFile;
  filename.append("Mapping.dot");
  dotFile.open(filename.c_str());
  dotFile << "digraph CGRAGraph { ";

  for (int timeIndex = 0; timeIndex < II - 1; timeIndex++)
  {
    TimeCGRA* cgraTime = timeCGRAList[timeIndex];

    for (int i = 0; i < SizeX; i++)
    {
      for (int j = 0; j < SizeY; j++)
      {

        dotFile << cgraTime->getPE(i, j)->getIndexI() << cgraTime->getPE(i, j)->getIndexJ() << cgraTime->getPE(i, j)->getIndexT()
          << " -> ";

        dotFile << timeCGRAList[(timeIndex + 1) % II]->getPE(i, j)->getIndexI()
          << timeCGRAList[(timeIndex + 1) % II]->getPE(i, j)->getIndexJ()
          << timeCGRAList[(timeIndex + 1) % II]->getPE(i, j)->getIndexT() << ";\n";

        dotFile << cgraTime->getPE(i, j)->getIndexI() << cgraTime->getPE(i, j)->getIndexJ() << cgraTime->getPE(i, j)->getIndexT()
          << " -> ";

        dotFile << timeCGRAList[(timeIndex + 1) % II]->getPE((i + 1) % SizeX, j)->getIndexI()
          << timeCGRAList[(timeIndex + 1) % II]->getPE((i + 1) % SizeX, j)->getIndexJ()
          << timeCGRAList[(timeIndex + 1) % II]->getPE((i + 1) % SizeX, j)->getIndexT() << ";\n";

        dotFile << cgraTime->getPE(i, j)->getIndexI() << cgraTime->getPE(i, j)->getIndexJ() << cgraTime->getPE(i, j)->getIndexT()
          << " -> ";

        dotFile << timeCGRAList[(timeIndex + 1) % II]->getPE(i, (j + 1) % SizeY)->getIndexI()
          << timeCGRAList[(timeIndex + 1) % II]->getPE(i, (j + 1) % SizeY)->getIndexJ()
          << timeCGRAList[(timeIndex + 1) % II]->getPE(i, (j + 1) % SizeY)->getIndexT() << ";\n";

        int iIndex = i;
        if (i == 0)
        {
          iIndex = SizeX;
        }
        dotFile << cgraTime->getPE(i, j)->getIndexI() << cgraTime->getPE(i, j)->getIndexJ() << cgraTime->getPE(i, j)->getIndexT()
          << " -> ";

        dotFile << timeCGRAList[(timeIndex + 1) % II]->getPE((iIndex - 1) % SizeX, j)->getIndexI()
          << timeCGRAList[(timeIndex + 1) % II]->getPE((iIndex - 1) % SizeX, j)->getIndexJ()
          << timeCGRAList[(timeIndex + 1) % II]->getPE((iIndex - 1) % SizeX, j)->getIndexT() << ";\n";

        int jIndex = j;
        if (j == 0)
        {
          jIndex = SizeY;
        }

        dotFile << cgraTime->getPE(i, j)->getIndexI() << cgraTime->getPE(i, j)->getIndexJ() << cgraTime->getPE(i, j)->getIndexT()
          << " -> ";

        dotFile << timeCGRAList[(timeIndex + 1) % II]->getPE(i, (jIndex - 1) % SizeY)->getIndexI()
          << timeCGRAList[(timeIndex + 1) % II]->getPE(i, (jIndex - 1) % SizeY)->getIndexJ()
          << timeCGRAList[(timeIndex + 1) % II]->getPE(i, (jIndex - 1) % SizeY)->getIndexT() << ";\n";

      }
    }
  }

  for (int timeIndex = 0; timeIndex < II; timeIndex++)
  {
    TimeCGRA* cgraTime = timeCGRAList[timeIndex];

    for (int i = 0; i < SizeX; i++)
    {
      for (int j = 0; j < SizeY; j++)
      {
        if (cgraTime->getPE(i, j)->isBusy())
        {
          dotFile << cgraTime->getPE(i, j)->getIndexI() << cgraTime->getPE(i, j)->getIndexJ()
            << cgraTime->getPE(i, j)->getIndexT();

          dotFile << " [ color = lightblue, label =\"" << cgraTime->getPE(i, j)->getNode()->get_ID() << "\"];\n";
        }
        else
        {
          //dotFile << "[ ;\n";
        }

      }
    }
  }

  for (int timeIndex = 0; timeIndex < II - 1; timeIndex++)
  {
    TimeCGRA* cgraTime = timeCGRAList[timeIndex];
    dotFile << "{ rank = same; \n";
    for (int i = 0; i < SizeX; i++)
    {
      for (int j = 0; j < SizeY; j++)
      {

        dotFile << cgraTime->getPE(i, j)->getIndexI() << cgraTime->getPE(i, j)->getIndexJ() << cgraTime->getPE(i, j)->getIndexT()
          << "; ";

      }
    }
    dotFile << "}; \n";
  }

  dotFile << "}";
  dotFile.close();

}

/*void CGRA::SetThreshold(int thres)
  {
  this->Threshold = thres;
  }
  */
void CGRA::ResetIteration()
{
  iteration = 0;
}

int CGRA::getII()
{
  return II;
}

bool CGRA::MCS2(REGI_DFG* myDFG, vector<REGI_Node*> &leftNotMapped)
{
  //myDFG->DotPrintDFGWithLevel("ToMap");
  vector<REGI_Node*> nodes = myDFG->getSetOfVertices();

  int nodeSize = (int) nodes.size();

  int* RegisterCount = new int[SizeX * SizeY];

  for (int i = 0; i < SizeX * SizeY; i++)
  {
    RegisterCount[i] = 0;
  }

  int* compTable = new int[nodeSize * II * SizeX * SizeY];

  REGI_Node* tempNode;
  REGI_PE* tempPE;
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

  vector<MappingPair*> mapPairs;
  MappingPair* mp;
  REGI_Node* node1;

  REGI_PE* pe1;

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
  /*
     if (RANDOMSORTPLACEMENT)
     sortRandomMappingPairs(mapPairs, myDFG);

     if (RELATEDSORTPLACEMENT)
     sortRelatedMappingPairs(mapPairs, myDFG);
     */
  int tableSize = (int) mapPairs.size();
  char* table = new char[tableSize * tableSize];
  MappingPair* mp1;
  MappingPair* mp2;
  MappingPair* mp3;

  for (int i = 0; i < tableSize; i++)
  {
    mp1 = mapPairs[i];

    for (int j = 0; j < tableSize; j++)
    {
      if (i == j)
      {
        table[i * tableSize + j] = 1;
      }
      else
      {
        mp2 = mapPairs[j];
        table[i * tableSize + j] = Compatible(mp1, mp2);
      }

    }
  }

  //Dot_Print_Comp_Graph("Placement", table, tableSize, mapPairs);
  if (MAPPING_POLICY.ENABLE_REGISTERS)
  {
    //update inter
    for (int index1 = 0; index1 < tableSize; index1++)
    {
      mp1 = mapPairs[index1];
      if (mp1->getNode()->has_self_loop())
      {
        continue;
      }
      vector<REGI_Node*> cons1 = mp1->getNode()->get_physical_successors_constrain_by_this_modulo();

      for (int index2 = 0; index2 < (int) cons1.size(); index2++)
      {

        if (mp1->getNode()->get_ID() == cons1[index2]->get_ID())
        {
          continue;
        }
        if (myDFG->get_Arc(mp1->getNode(), cons1[index2])->get_Distance() == 0)
        {
          continue;
        }

        for (int index3 = 0; index3 < tableSize; index3++)
        {

          if (index3 == index1)
          {
            continue;
          }
          mp2 = mapPairs[index3];
          if (table[index1 * tableSize + index3] == 0)
          {
            continue;
          }
          if (cons1[index2]->get_ID() == mp2->getNode()->get_ID() && mp1->SameCoordinationPE(mp2))
          {
            int req_register = myDFG->get_Required_Register(mp1->getNode(), mp2->getNode(), II);

            table[index1 * tableSize + index3] = table[index1 * tableSize + index3] + req_register;

            table[index3 * tableSize + index1] = table[index3 * tableSize + index1] + req_register - 1;
            //node1<node2

            if (mp1->getNode()->get_Sched_Info()->get_Modulo_Current()
                < mp2->getNode()->get_Sched_Info()->get_Modulo_Current())
            {
              for (int index4 = 0; index4 < tableSize; index4++)
              {
                if (index4 == index3 || index4 == index1)
                  continue;
                mp3 = mapPairs[index4];
                if (!mp1->SameCoordinationPE(mp3))
                {
                  continue;
                }
                if (table[index1 * tableSize + index4] == 0)
                {
                  continue;
                }
                if (mp1->getNode()->get_Sched_Info()->get_Modulo_Current()
                    < mp3->getNode()->get_Sched_Info()->get_Modulo_Current()
                    && mp3->getNode()->get_Sched_Info()->get_Modulo_Current()
                    < mp2->getNode()->get_Sched_Info()->get_Modulo_Current())
                {
                  table[index4 * tableSize + index1] = table[index4 * tableSize + index1] + req_register;
                }
                else
                {
                  table[index4 * tableSize + index1] = table[index4 * tableSize + index1] + req_register
                    - 1;
                }

              }

            }
            //node2<node1
            else
            {
              for (int index4 = 0; index4 < tableSize; index4++)
              {
                if (index4 == index3 || index4 == index1)
                  continue;
                mp3 = mapPairs[index4];
                if (!mp1->SameCoordinationPE(mp3))
                {
                  continue;
                }
                if (table[index1 * tableSize + index4] == 0 || table[index3 * tableSize + index4] == 0)
                {
                  continue;
                }
                if (mp1->getNode()->get_Sched_Info()->get_Modulo_Current()
                    < mp3->getNode()->get_Sched_Info()->get_Modulo_Current()
                    && mp3->getNode()->get_Sched_Info()->get_Modulo_Current()
                    < mp2->getNode()->get_Sched_Info()->get_Modulo_Current())
                {
                  table[index4 * tableSize + index1] = table[index4 * tableSize + index1] + req_register
                    - 1;

                }
                else
                {
                  table[index4 * tableSize + index1] = table[index4 * tableSize + index1] + req_register;
                }

              }
            }

          }

        }

      }
    }
    //update intra
    for (int index1 = 0; index1 < tableSize; index1++)
    {
      mp1 = mapPairs[index1];
      if (mp1->getNode()->has_self_loop())
      {
        continue;
      }
      vector<REGI_Node*> cons1 = mp1->getNode()->get_physical_successors_constrain_by_this_modulo();

      for (int index2 = 0; index2 < (int) cons1.size(); index2++)
      {

        if (myDFG->get_Arc(mp1->getNode(), cons1[index2])->get_Distance() > 0)
        {
          continue;
        }
        for (int index3 = 0; index3 < tableSize; index3++)
        {
          if (index3 == index1)
            continue;
          mp2 = mapPairs[index3];
          if (table[index1 * tableSize + index3] == 0)
          {
            continue;
          }
          if (cons1[index2]->get_ID() == mp2->getNode()->get_ID() && mp1->SameCoordinationPE(mp2))
          {
            int req_register = myDFG->get_Required_Register(mp1->getNode(), mp2->getNode(), II);

            table[index1 * tableSize + index3] = table[index1 * tableSize + index3] + req_register;

            table[index3 * tableSize + index1] = table[index3 * tableSize + index1] + req_register - 1;
            //node1<node2

            if (mp1->getNode()->get_Sched_Info()->get_Modulo_Current()
                < mp2->getNode()->get_Sched_Info()->get_Modulo_Current())
            {
              for (int index4 = 0; index4 < tableSize; index4++)
              {
                if (index4 == index3 || index4 == index1)
                  continue;
                mp3 = mapPairs[index4];
                if (!mp1->SameCoordinationPE(mp3))
                {
                  continue;
                }
                if (table[index1 * tableSize + index4] == 0)
                {
                  continue;
                }
                if (mp1->getNode()->get_Sched_Info()->get_Modulo_Current()
                    < mp3->getNode()->get_Sched_Info()->get_Modulo_Current()
                    && mp3->getNode()->get_Sched_Info()->get_Modulo_Current()
                    < mp2->getNode()->get_Sched_Info()->get_Modulo_Current())
                {
                  table[index4 * tableSize + index1] = table[index4 * tableSize + index1] + req_register;

                }

              }

            }
            //node2<node1
            else
            {
              for (int index4 = 0; index4 < tableSize; index4++)
              {
                if (index4 == index3 || index4 == index1)
                  continue;
                mp3 = mapPairs[index4];
                if (!mp1->SameCoordinationPE(mp3))
                {
                  continue;
                }
                if (table[index1 * tableSize + index4] == 0)
                {
                  continue;
                }
                if (mp1->getNode()->get_Sched_Info()->get_Modulo_Current()
                    < mp3->getNode()->get_Sched_Info()->get_Modulo_Current()
                    && mp3->getNode()->get_Sched_Info()->get_Modulo_Current()
                    < mp2->getNode()->get_Sched_Info()->get_Modulo_Current())
                {
                  continue;

                }
                else
                {
                  table[index4 * tableSize + index1] = table[index4 * tableSize + index1] + req_register;
                }

              }
            }

          }

        }

      }
    }
    //update loop
    for (int index1 = 0; index1 < tableSize; index1++)
    {
      mp1 = mapPairs[index1];
      int req_register = 0;
      if (mp1->getNode()->has_self_loop())
      {

        req_register = mp1->getNode()->get_self_loop()->get_Distance();

      }
      else
      {
        continue;

      }
      table[index1 * tableSize + index1] += req_register;

      for (int index4 = 0; index4 < tableSize; index4++)
      {

        if (index4 == index1)
        {
          continue;
        }
        mp3 = mapPairs[index4];
        if (!mp1->SameCoordinationPE(mp3))
        {
          continue;
        }
        if (table[index1 * tableSize + index4] == 0)
        {
          continue;
        }
        table[index4 * tableSize + index1] += req_register;

      }

    }
  }
  //Dot_Print_Comp_Graph("Placement_Reg", table, tableSize, mapPairs);

  //DumpCompGraph(table, tableSize, "Compatibility");
  char* tableAnd = new char[tableSize];

  for (int i = 0; i < tableSize; i++)
  {
    tableAnd[i] = 1;
  }
  vector<int> indecis;
  vector<Cover_Pair*> BestIndecis;

  /*
  //
  int bestIndex = 0;
  int upper = nodeSize;

  for (int i = 0; i < 1; i++)
  {
  //cout << i << endl;
  if (bestCost == nodeSize)
  {
  break;
  }
  indecis.clear();
  ResetIteration();
  current = Map2(table, i, tableAnd, indecis, tableSize, copybest,
  copybest, nodeSize, 1, mapPairs, upper, RegisterCount);
  if (current > bestCost)
  {
  bestCost = current;
  //		bestIndex = i;
  BestIndecis = indecis;

  }
  }
  */
  Mapper myMapper(SizeReg);
  vector<vector<Cover_Pair*> > results;

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
  //if (DEBUG)
  /*{
    for (int i = 0; i < (int) BestIndecis.size(); i++)
    {
    mp1 = mapPairs[(int) BestIndecis[i]];
    cout << "\n Node " << mp1->getNode()->getNumber() << ","
    << mp1->getPE()->getIndexT() << ","
    << mp1->getPE()->getIndexI() << ","
    << mp1->getPE()->getIndexJ() << "\n";
    }
    cout << bestCost << endl;
    }*/
  bool nodefound = false;
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
    {
      leftNotMapped.push_back(nodes[i]);

    }

  }
  for (int i = 0; i < (int) BestIndecis.size(); i++)
  {
    mp1 = mapPairs[(int) BestIndecis[i]->NodeID];
    mp1->getPE()->AssignNode(mp1->getNode());
    mp1->getPE()->AllocateRegister((int) BestIndecis[i]->REG - 1);
    //cout<<BestIndecis[i]->REG<<"\t";

  }
  if (maxSize == nodeSize)
  {

    Prolog(myDFG);
    return true;
  }
  else
  {
    return false;
  }

}

void CGRA::Prolog(REGI_DFG* myDFG)
{
  std::ofstream prologFile;
  std::string prolog = "prolog";
  prolog.append(".sch");
  prologFile.open(prolog.c_str());
  int total=0;
  int t = -1;
  bool toBreak = false;
  cout << "\n*********************************Prolog Start*********************************\n";
  while (true)
  {
    for (int timeIndex = 0; timeIndex < II; timeIndex++)
    {
      t++;
      toBreak = true;
      TimeCGRA* cgraTime = timeCGRAList[timeIndex];
      REGI_PE* candidatePE;
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
    if (toBreak)
    {
      break;
    }
  }
  cout << "\n*********************************Prolog End*********************************\n";
  prologFile << total << "\n";
  prologFile.close();
}

void CGRA::EPIlog(REGI_DFG* myDFG)
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
      REGI_PE* candidatePE;
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

REGI_Node* CGRA::getNode(int tableIndex, vector<REGI_Node*> &nodes)

{

  return nodes[tableIndex];

}

REGI_PE* CGRA::getPE(int index)
{

  int CurrentII = index / (SizeX * SizeY);

  int XIndex = (index % (SizeX * SizeY)) / SizeY;
  int YIndex = index % SizeY;
  return timeCGRAList[CurrentII]->getPE(XIndex, YIndex);

}

bool CGRA::SameCoordinatePEs(REGI_PE* pe1, REGI_PE* pe2)
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

bool CGRA::contains(vector<REGI_Node*> &nodeset, REGI_Node* node)
{

  vector<REGI_Node*>::iterator iNode1;
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

bool CGRA::contains(vector<REGI_Node*> &nodeset1, vector<REGI_Node*> &nodeset2)
{

  vector<REGI_Node*>::iterator iNode1;
  for (iNode1 = nodeset2.begin(); iNode1 != nodeset2.end(); iNode1++)
  {
    if (contains(nodeset1, *iNode1))
      return true;

  }
  return false;

}

bool CGRA::connectedPEs(REGI_PE* pe1, REGI_PE* pe2)
{

  //TODO should be done according to settings

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
      {
        return false;
      }
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
    {
      return false;
    }
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

bool CGRA::connected_Nodes(REGI_Node* node1, REGI_Node* node2)
{
  if (node1->is_Physically_Connected_To(node2))
    return true;

  vector<REGI_Node*> cons = node1->get_physical_successors_constrain_by_this_modulo();
  if (contains(cons, node2))
  {
    return true;
  }

  return false;
}

int CGRA::Compatible(MappingPair* mp1, MappingPair* mp2)
{
  if (mp1->getNode()->get_ID() == mp2->getNode()->get_ID() || mp1->getPE() == mp2->getPE())
  {
    return 0;
  }
  else
  {
    //constraint part
    vector<REGI_Node*> cons1 = mp1->getNode()->get_physical_successors_constrain_by_this_modulo();
    vector<REGI_Node*> cons2 = mp2->getNode()->get_physical_successors_constrain_by_this_modulo();
    if (contains(cons1, mp2->getNode()) || contains(cons2, mp1->getNode()))
    {
      if (!SameCoordinatePEs(mp1->getPE(), mp2->getPE()))
        return 0;
    }

    if ((connected_Nodes(mp1->getNode(), mp2->getNode()) && !connectedPEs(mp1->getPE(), mp2->getPE()))
        || (connected_Nodes(mp2->getNode(), mp1->getNode()) && !connectedPEs(mp2->getPE(), mp1->getPE())))
    {
      return 0;
    }
    if (mp1->getPE()->getIndexI()==mp2->getPE()->getIndexI() && mp1->getPE()->getIndexT()==mp2->getPE()->getIndexT() && mp1->getNode()->is_Mem_Operation() && mp2->getNode()->is_Mem_Operation())
    {
      if ((mp1->getNode()->is_Load_Address_Generator() || mp1->getNode()->is_Store_Address_Generator())&& (mp2->getNode()->is_Load_Address_Generator() || mp2->getNode()->is_Store_Address_Generator()) && (mp1->getNode()->get_Related_Node()->get_ID()!=mp2->getNode()->get_ID()))
      {
        //DEBUG_SCHEDULING("Node %d assigned to PE(%d,%d) not compatible with Node %d assigend to PE(%d,%d)", mp1->getNode()->get_ID(), mp1->getPE()->getIndexI(), mp1->getPE()->getIndexJ(), mp2->getNode()->get_ID(), mp2->getPE()->getIndexI(), mp2->getPE()->getIndexJ());
        return 0;
      }
      if ((mp1->getNode()->is_Load_Data_Bus_Read() || mp1->getNode()->is_Store_Data_Bus_Write() ) && (mp2->getNode()->is_Load_Data_Bus_Read() || mp2->getNode()->is_Store_Data_Bus_Write())&& (mp1->getNode()->get_Related_Node()->get_ID()!=mp2->getNode()->get_ID()))
      {
        //DEBUG_SCHEDULING("Node %d assigned to PE(%d,%d) not compatible with Node %d assigend to PE(%d,%d)", mp1->getNode()->get_ID(), mp1->getPE()->getIndexI(), mp1->getPE()->getIndexJ(), mp2->getNode()->get_ID(), mp2->getPE()->getIndexI(), mp2->getPE()->getIndexJ());
        return 0;
      }
    }
    if (mp1->getNode()->is_Mem_Operation() && mp2->getNode()->is_Mem_Operation())
    {
      if(mp1->getNode()->get_Related_Node()->get_ID()==mp2->getNode()->get_ID())
      {
        if (mp1->getPE()->getIndexI()!=mp2->getPE()->getIndexI())
        {
          return 0;
        }
      }
    }


  }
  return 1;
}

void CGRA::Dot_Print_Comp_Graph(string filename, char *table, int size, vector<MappingPair*> &mapPairs)
{
  ofstream dotFile;
  string graphname = filename;
  filename.append("comp.dot");
  dotFile.open(filename.c_str());

  dotFile << "graph " << graphname << "  \n{";

  for (int i = 0; i < size; i++)
  {
    for (int j = i; j < size; j++)
    {
      if (table[i * size + j] > 0)
      {
        dotFile << i << " -- " << j <<";\n";//<< " [label=\"(" << (int) table[i * size + j] << "," << (int) table[j * size + i] << ")\"] \n";
      }
    }
    //dotFile << i << " [label= \" " << mapPairs[i]->getNode()->get_ID() << ",(" << mapPairs[i]->getPE()->getIndexT() << ","
    //                << mapPairs[i]->getPE()->getIndexI() << "," << mapPairs[i]->getPE()->getIndexJ() << ")\"];\n";

  }
  dotFile << "\n}\n";

  dotFile.close();
}
int CGRA::Physical_Distance(REGI_PE* pe1, REGI_PE* pe2)
{
  //TODO should be done according to settings

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
