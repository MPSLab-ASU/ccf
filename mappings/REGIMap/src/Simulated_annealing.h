/*
 * Simulated_annealing.h
 *
 *  Created on: Jan 30, 2013
 *      Author: mahdi
 */

#ifndef SIMULATED_ANNEALING_H_
#define SIMULATED_ANNEALING_H_

#include "Parser.h"
#include <time.h>



void showMapping(REGI_DFG* dfg, CGRA* myCGRA)
{
    vector<REGI_Node*> nodes = dfg->getSetOfVertices();
    vector<REGI_Node*> temp;
    vector<REGI_ARC*> arcs = dfg->getSetOfArcs();
    REGI_PE* candidatePE;
    printf("*********************************Kernel Start*********************************\n");
    for (int timeIndex = 0; timeIndex < myCGRA->getII(); timeIndex++)
    {

        cout << "Time:" << timeIndex << endl;
        for (int i = 0; i < cgra_info.X_Dim; i++)
        {
            for (int j = 0; j < cgra_info.Y_Dim; j++)
            {
                candidatePE = myCGRA->getPE(timeIndex * cgra_info.X_Dim * cgra_info.Y_Dim + i * cgra_info.Y_Dim + j);
                if (candidatePE->isBusy())
                {
                    for (int k = 0; k < (int) nodes.size(); k++)
                    {
                        if (nodes[k]->getPE() == candidatePE)
                        {
                            printf("%10d(t=%d,R=%d)", nodes[k]->get_ID(), nodes[k]->get_Sched_Info()->get_Current(),
                                    candidatePE->NumberOfUsedRegisters());
                        }
                    }
                    for (int k = 0; k < (int) arcs.size(); k++)
                    {
                        temp = arcs[k]->Get_Path();
                        for (int temp_index = 0; temp_index < (int) temp.size(); temp_index++)
                        {
                            if (temp[temp_index]->getPE() == candidatePE)
                            {
                                printf("%10d(t=%d,R=%d)", temp[temp_index]->get_ID(),
                                        temp[temp_index]->get_Sched_Info()->get_Current(),
                                        candidatePE->NumberOfUsedRegisters());
                            }
                        }
                    }
                }
                else
                {
                    printf("         F");
                }
                //printf("|\t");
            }
            printf("\n");

        }

    }
    printf("*********************************Kernel End*********************************\n");
}

void showSet(vector<REGI_Node*> set)
{
    int index = 0;

    cout << "\nset sizes start\n";
    int size = set.size();
    for (index = 0; index < size; index++)
    {

        //cout<<((Node*)set[index1])->getNumber()<<",L"<<((Node*)set[index1])->getLevel()<<",";
        cout << ((REGI_Node*) set[index])->get_ID() << ",";

    }
    cout << "\nset sizes end\n";

}

//done
int PlacementCost(REGI_Node *current, int temperature, CGRA* myCGRA)
{
    int cost = 0;
    int II = myCGRA->getII();
    int last_index = II * cgra_info.X_Dim * cgra_info.Y_Dim;
    for (int i = 0; i < last_index; i++)
    {
        if (myCGRA->getPE(i)->getOverUsedNumber() > 1)
        {
            cost += myCGRA->getPE(i)->getOverUsedNumber();
        }
        if (myCGRA->getPE(i)->NumberOfUsedRegisters() > cgra_info.R_Size)
            cost += myCGRA->getPE(i)->NumberOfUsedRegisters() - cgra_info.R_Size;
    }

    return cost;
}

void Allocate_Registers(CGRA* myCGRA, int point_1, int point_2, int point_3, int number_1, int number_2, REGI_PE* PE)
{
    REGI_PE* temp_PE;

    if (point_1 == point_2)
    {
        for (int j = 0; j < point_3; j++)
        {
            temp_PE = myCGRA->getPE((j) * (cgra_info.X_Dim * cgra_info.Y_Dim) + PE->getIndexI() * cgra_info.Y_Dim + PE->getIndexJ());
            temp_PE->AllocateRegister(temp_PE->NumberOfUsedRegisters() + number_1);
        }
    }
    else
    {
        for (int j = 0; j < point_1; j++)
        {
            temp_PE = myCGRA->getPE((j) * (cgra_info.X_Dim * cgra_info.Y_Dim) + PE->getIndexI() * cgra_info.Y_Dim + PE->getIndexJ());
            temp_PE->AllocateRegister(temp_PE->NumberOfUsedRegisters() + number_1);
        }
        for (int j = point_1; j < point_2; j++)
        {
            temp_PE = myCGRA->getPE((j) * (cgra_info.X_Dim * cgra_info.Y_Dim) + PE->getIndexI() * cgra_info.Y_Dim + PE->getIndexJ());
            temp_PE->AllocateRegister(temp_PE->NumberOfUsedRegisters() + number_2);
        }
        for (int j = point_2; j < point_3; j++)
        {
            temp_PE = myCGRA->getPE((j) * (cgra_info.X_Dim * cgra_info.Y_Dim) + PE->getIndexI() * cgra_info.Y_Dim + PE->getIndexJ());
            temp_PE->AllocateRegister(temp_PE->NumberOfUsedRegisters() + number_1);
        }
    }
}

void Allocate_Registers(REGI_DFG* dfg, CGRA* myCGRA, REGI_Node *prev_node, REGI_PE* prev_PE, REGI_Node *next_node, REGI_PE* next_PE)
{

    int delta_inc_reg = 0;
    int load_time;
    int no_registers = dfg->get_Arc(prev_node, next_node)->get_Distance();
    int distance = myCGRA->Physical_Distance(prev_PE, next_PE);
    int point_1, point_2, reg_number_1, reg_number_2;

    if (prev_node->get_Sched_Info()->get_Current() <= next_node->get_Sched_Info()->get_Current())
    {

        delta_inc_reg = (next_node->get_Sched_Info()->get_Current() - prev_node->get_Sched_Info()->get_Current() - distance) / myCGRA->getII();
        no_registers = no_registers - delta_inc_reg;
        load_time = (next_node->get_Sched_Info()->get_Current() - distance) % (myCGRA->getII());
        load_time = (load_time + myCGRA->getII()) % myCGRA->getII();

        if ((next_node->get_Sched_Info()->get_Current() % myCGRA->getII()) >= (prev_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
        {

            if (load_time <= (next_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
            {

                if (load_time < (prev_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
                {
                    point_1 = load_time;
                    point_2 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    reg_number_1 = no_registers;
                    reg_number_2 = no_registers - 1;

                }
                else
                {
                    point_1 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    point_2 = load_time;
                    reg_number_1 = no_registers;
                    reg_number_2 = no_registers + 1;

                }

            }
            else
            {
                point_1 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                point_2 = load_time;
                reg_number_1 = no_registers - 1;
                reg_number_2 = no_registers;

            }
        }
        else        //if ( (next_node->get_Sched_Info()->get_Current() % myCGRA->getII()) < (prev_node->get_Sched_Info()->get_Current() % myCGRA->getII()) )
        {

            if (load_time <= (next_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
            {

                point_1 = load_time;
                point_2 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                reg_number_1 = no_registers - 1;
                reg_number_2 = no_registers - 2;

            }
            else
            {
                if (load_time < (prev_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
                {
                    point_1 = load_time;
                    point_2 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    reg_number_1 = no_registers - 2;
                    reg_number_2 = no_registers - 3;

                }
                else
                {
                    point_1 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    point_2 = load_time;
                    reg_number_1 = no_registers - 2;
                    reg_number_2 = no_registers - 1;

                }
            }

        }

    }
    else
    {

        delta_inc_reg = (prev_node->get_Sched_Info()->get_Current() - next_node->get_Sched_Info()->get_Current() + distance) / myCGRA->getII();
        no_registers = no_registers + delta_inc_reg;
        load_time = (next_node->get_Sched_Info()->get_Current() - distance) % (myCGRA->getII());
        load_time = (load_time + myCGRA->getII()) % myCGRA->getII();

        if ((next_node->get_Sched_Info()->get_Current() % myCGRA->getII()) <= (prev_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
        {

            if (load_time <= (next_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
            {
                point_1 = load_time;
                point_2 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                reg_number_1 = no_registers;
                reg_number_2 = no_registers - 1;
            }
            else
            {
                if (load_time < (prev_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
                {
                    point_1 = load_time;
                    point_2 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    reg_number_1 = no_registers - 1;
                    reg_number_2 = no_registers - 2;

                }
                else
                {
                    point_1 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    point_2 = load_time;
                    reg_number_1 = no_registers - 1;
                    reg_number_2 = no_registers;

                }
            }
        }
        else
        {

            if (load_time <= (next_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
            {

                if (load_time <= (prev_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
                {
                    point_1 = load_time;
                    point_2 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    reg_number_1 = no_registers;
                    reg_number_2 = no_registers - 1;

                }
                else
                {
                    point_1 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    point_2 = load_time;
                    reg_number_1 = no_registers;
                    reg_number_2 = no_registers + 1;

                }

            }
            else
            {
                point_1 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                point_2 = load_time;
                reg_number_1 = no_registers - 1;
                reg_number_2 = no_registers;

            }

        }

    }

    Allocate_Registers(myCGRA, point_1, point_2, myCGRA->getII(), reg_number_1, reg_number_2, prev_node->getPE());
}

void Random_Initial_Place_Route(CGRA* myCGRA, REGI_DFG* dfg)
{
    int len = dfg->Schedule_ASAP();
    dfg->Schedule_ALAP(len);
    dfg->Set_Schedule_Length(len);
    vector<REGI_Node*> nodes = dfg->getSetOfVertices();
    vector<REGI_Node*> todo_nodes;
    vector<REGI_Node*> temp;

    REGI_Node* current_node;
    REGI_PE* current_PE;
    REGI_PE* temp_PE;
    int II = myCGRA->getII();
    int schedule;
    for (int i = 0; i < (int) nodes.size(); i++)
    {

        current_node = nodes[i];
        schedule = current_node->get_Sched_Info()->get_ALAP() % II;
        current_node->get_Sched_Info()->set_Current(current_node->get_Sched_Info()->get_ALAP(), II);
        current_PE = myCGRA->getPE(schedule * (cgra_info.X_Dim * cgra_info.Y_Dim));
        current_PE->AssignNode(current_node);
        if (current_node->has_self_loop())
        {

            for (int index = 0; index < II; index++)
            {
                temp_PE = myCGRA->getPE(index * (cgra_info.X_Dim * cgra_info.Y_Dim));
                temp_PE->AllocateRegister(temp_PE->NumberOfUsedRegisters() + current_node->get_self_loop()->get_Distance());
            }

        }

    }
    for (int i = 0; i < (int) nodes.size(); i++)
    {

        current_node = nodes[i];
        schedule = current_node->get_Sched_Info()->get_ALAP() % II;
        current_node->get_Sched_Info()->set_Current(current_node->get_Sched_Info()->get_ALAP(), II);
        current_PE = myCGRA->getPE(schedule * (cgra_info.X_Dim * cgra_info.Y_Dim));
        //current_node->Assign2PE(current_PE);
        temp = current_node->Get_Next_Nodes_Same_Iteration();
        for (int j = 0; j < (int) temp.size(); j++)
        {
            int delta_reg = temp[j]->get_Sched_Info()->get_ALAP() - current_node->get_Sched_Info()->get_ALAP();
            //prev_nodes[i]->getPE()->AllocateRegister(prev_nodes[i]->getPE()->NumberOfUsedRegisters() + no_registers);
            if (delta_reg > 1)
            {
                for (int k = 0; k < delta_reg; k++)
                {
                    REGI_PE* temp_PE = myCGRA->getPE(((schedule + k) % myCGRA->getII()) * (cgra_info.X_Dim * cgra_info.Y_Dim));
                    temp_PE->AllocateRegister(temp_PE->NumberOfUsedRegisters() + 1);
                }
            }

        }
        todo_nodes = current_node->Get_Next_Nodes();
        temp = dfg->Subtract(todo_nodes, temp);

        for (int j = 0; j < (int) temp.size(); j++)
        {

            Allocate_Registers(dfg, myCGRA, current_node, current_node->getPE(), temp[j], temp[j]->getPE());
        }

    }

}

void Release_Registers(CGRA* myCGRA, int point_1, int point_2, int point_3, int number_1, int number_2, REGI_PE* PE)
{

    REGI_PE* temp_PE;
    if (point_1 == point_2)
    {
        for (int j = 0; j < point_3; j++)
        {
            temp_PE = myCGRA->getPE((j) * (cgra_info.X_Dim * cgra_info.Y_Dim) + PE->getIndexI() * cgra_info.Y_Dim + PE->getIndexJ());
            temp_PE->ReleaseRegister(number_1);
        }
    }
    else
    {
        for (int j = 0; j < point_1; j++)
        {
            temp_PE = myCGRA->getPE((j) * (cgra_info.X_Dim * cgra_info.Y_Dim) + PE->getIndexI() * cgra_info.Y_Dim + PE->getIndexJ());
            temp_PE->ReleaseRegister(number_1);
        }
        for (int j = point_1; j < point_2; j++)
        {
            temp_PE = myCGRA->getPE((j) * (cgra_info.X_Dim * cgra_info.Y_Dim) + PE->getIndexI() * cgra_info.Y_Dim + PE->getIndexJ());
            temp_PE->ReleaseRegister(number_2);
        }
        for (int j = point_2; j < point_3; j++)
        {
            temp_PE = myCGRA->getPE((j) * (cgra_info.X_Dim * cgra_info.Y_Dim) + PE->getIndexI() * cgra_info.Y_Dim + PE->getIndexJ());
            temp_PE->ReleaseRegister(number_1);
        }
    }
}

void Release_Registers(REGI_DFG* dfg, CGRA* myCGRA, REGI_Node *prev_node, REGI_PE* prev_PE, REGI_Node *next_node, REGI_PE* next_PE)
{

    int delta_inc_reg = 0;
    int load_time;
    int no_registers = dfg->get_Arc(prev_node, next_node)->get_Distance();
    int distance = myCGRA->Physical_Distance(prev_PE, next_PE);
    int point_1, point_2, reg_number_1, reg_number_2;

    if (prev_node->get_Sched_Info()->get_Current() <= next_node->get_Sched_Info()->get_Current())
    {

        delta_inc_reg = (next_node->get_Sched_Info()->get_Current() - prev_node->get_Sched_Info()->get_Current() - distance) / myCGRA->getII();
        no_registers = no_registers - delta_inc_reg;
        load_time = (next_node->get_Sched_Info()->get_Current() - distance) % (myCGRA->getII());
        load_time = (load_time + myCGRA->getII()) % myCGRA->getII();

        if ((next_node->get_Sched_Info()->get_Current() % myCGRA->getII()) >= (prev_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
        {

            if (load_time <= (next_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
            {

                if (load_time < (prev_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
                {
                    point_1 = load_time;
                    point_2 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    reg_number_1 = no_registers;
                    reg_number_2 = no_registers - 1;

                }
                else
                {
                    point_1 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    point_2 = load_time;
                    reg_number_1 = no_registers;
                    reg_number_2 = no_registers + 1;

                }

            }
            else
            {
                point_1 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                point_2 = load_time;
                reg_number_1 = no_registers - 1;
                reg_number_2 = no_registers;

            }
        }
        else        //if ( (next_node->get_Sched_Info()->get_Current() % myCGRA->getII()) < (prev_node->get_Sched_Info()->get_Current() % myCGRA->getII()) )
        {

            if (load_time <= (next_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
            {

                point_1 = load_time;
                point_2 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                reg_number_1 = no_registers - 1;
                reg_number_2 = no_registers - 2;

            }
            else
            {
                if (load_time < (prev_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
                {
                    point_1 = load_time;
                    point_2 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    reg_number_1 = no_registers - 2;
                    reg_number_2 = no_registers - 3;

                }
                else
                {
                    point_1 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    point_2 = load_time;
                    reg_number_1 = no_registers - 2;
                    reg_number_2 = no_registers - 1;

                }
            }

        }

    }
    else
    {

        delta_inc_reg = (prev_node->get_Sched_Info()->get_Current() - next_node->get_Sched_Info()->get_Current() + distance) / myCGRA->getII();
        no_registers = no_registers + delta_inc_reg;
        load_time = (next_node->get_Sched_Info()->get_Current() - distance) % (myCGRA->getII());
        load_time = (load_time + myCGRA->getII()) % myCGRA->getII();

        if ((next_node->get_Sched_Info()->get_Current() % myCGRA->getII()) <= (prev_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
        {

            if (load_time <= (next_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
            {
                point_1 = load_time;
                point_2 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                reg_number_1 = no_registers;
                reg_number_2 = no_registers - 1;
            }
            else
            {
                if (load_time < (prev_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
                {
                    point_1 = load_time;
                    point_2 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    reg_number_1 = no_registers - 1;
                    reg_number_2 = no_registers - 2;

                }
                else
                {
                    point_1 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    point_2 = load_time;
                    reg_number_1 = no_registers - 1;
                    reg_number_2 = no_registers;

                }
            }
        }
        else
        {

            if (load_time <= (next_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
            {

                if (load_time <= (prev_node->get_Sched_Info()->get_Current() % myCGRA->getII()))
                {
                    point_1 = load_time;
                    point_2 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    reg_number_1 = no_registers;
                    reg_number_2 = no_registers - 1;

                }
                else
                {
                    point_1 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                    point_2 = load_time;
                    reg_number_1 = no_registers;
                    reg_number_2 = no_registers + 1;

                }

            }
            else
            {
                point_1 = prev_node->get_Sched_Info()->get_Current() % myCGRA->getII();
                point_2 = load_time;
                reg_number_1 = no_registers - 1;
                reg_number_2 = no_registers;

            }

        }

    }

    Release_Registers(myCGRA, point_1, point_2, myCGRA->getII(), reg_number_1, reg_number_2, prev_node->getPE());

}

//redo
void Undo_Previous_Placement(CGRA* myCGRA, REGI_DFG* dfg, REGI_Node *current)
{
    int pred_size = current->get_Number_of_Pred();
    vector<REGI_Node*> temp_vec;
    REGI_ARC* temp_arc;
    REGI_PE* temp_PE;
    if (current->has_self_loop())
    {
        int Xindex = current->getPE()->getIndexI();
        int Yindex = current->getPE()->getIndexJ();
        for (int index = 0; index < myCGRA->getII(); index++)
        {
            temp_PE = myCGRA->getPE(index * (cgra_info.X_Dim * cgra_info.Y_Dim) + Xindex * cgra_info.Y_Dim + Yindex);

            temp_PE->ReleaseRegister(current->get_self_loop()->get_Distance());

        }
    }

    vector<REGI_Node*> prev_nodes = current->Get_Prev_Nodes_Same_Iteration();
    int delta_time;
    int distance;
    int delta_reg;

    for (int i = 0; i < (int) prev_nodes.size(); i++)
    {
        delta_time = current->get_Sched_Info()->get_Current() - prev_nodes[i]->get_Sched_Info()->get_Current();
        distance = myCGRA->Physical_Distance(current->getPE(), prev_nodes[i]->getPE());
        delta_reg = 1;

        //register needed
        if (delta_time > distance + 1)
        {

            delta_reg = delta_time - distance;
            //prev_nodes[i]->getPE()->AllocateRegister(prev_nodes[i]->getPE()->NumberOfUsedRegisters() + no_registers);
            for (int j = 0; j < delta_reg; j++)
            {
                temp_PE = myCGRA->getPE(
                        ((prev_nodes[i]->getPE()->getIndexT() + j) % myCGRA->getII()) * (cgra_info.X_Dim * cgra_info.Y_Dim)
                        + prev_nodes[i]->getPE()->getIndexI() * cgra_info.Y_Dim + prev_nodes[i]->getPE()->getIndexJ());
                temp_PE->ReleaseRegister(1);
            }

        }

    }

    vector<REGI_Node*> next_nodes = current->Get_Next_Nodes_Same_Iteration();

    for (int i = 0; i < (int) next_nodes.size(); i++)
    {

        delta_time = next_nodes[i]->get_Sched_Info()->get_Current() - current->get_Sched_Info()->get_Current();
        distance = myCGRA->Physical_Distance(current->getPE(), next_nodes[i]->getPE());
        delta_reg = 1;
        if (delta_time > distance + 1)
        {

            delta_reg = delta_time - distance;
            ///prev_nodes[i]->getPE()->AllocateRegister(prev_nodes[i]->getPE()->NumberOfUsedRegisters() + no_registers);
            for (int j = 0; j < delta_reg; j++)
            {
                temp_PE = myCGRA->getPE(
                        ((current->getPE()->getIndexT() + j) % myCGRA->getII()) * (cgra_info.X_Dim * cgra_info.Y_Dim)
                        + current->getPE()->getIndexI() * cgra_info.Y_Dim + current->getPE()->getIndexJ());
                temp_PE->ReleaseRegister(1);
            }

        }
        //temp = next_nodes[i];

    }

    vector<REGI_Node*> prev_nodes_all = current->Get_Prev_Nodes();

    prev_nodes = dfg->Subtract(prev_nodes_all, prev_nodes);

    // predecessor nodes prev iterations
    for (int i = 0; i < (int) prev_nodes.size(); i++)
    {
        Release_Registers(dfg, myCGRA, prev_nodes[i], prev_nodes[i]->getPE(), current, current->getPE());

    }

    vector<REGI_Node*> next_nodes_all = current->Get_Next_Nodes();
    next_nodes = dfg->Subtract(next_nodes_all, next_nodes);

    for (int i = 0; i < (int) next_nodes.size(); i++)
    {

        Release_Registers(dfg, myCGRA, current, current->getPE(), next_nodes[i], next_nodes[i]->getPE());

    }

    //undo predecessor nodes
    for (int i = 0; i < pred_size; i++)
    {
        temp_arc = current->get_Pred_Arc(i);
        temp_vec = temp_arc->Get_Path();

        for (int j = 0; j < (int) temp_vec.size(); j++)
        {
            temp_vec[j]->getPE()->ReleasePE();
            delete (temp_vec[j]);
        }
        temp_arc->clear_path();
    }

    int succ_size = current->get_Number_of_Succ();
    //undo successor nodes

    for (int i = 0; i < succ_size; i++)
    {
        temp_arc = current->get_Succ_Arc(i);
        //current->getPE()->ReleaseRegister(temp_arc->get_Distance());
        temp_vec = temp_arc->Get_Path();
        for (int j = 0; j < (int) temp_vec.size(); j++)
        {
            temp_vec[j]->getPE()->ReleasePE();
            delete (temp_vec[j]);
        }
        temp_arc->clear_path();

    }
    current->getPE()->ReleasePE();
    //undo self loops

}

//done (check)
bool Acceptable_Placement(CGRA* myCGRA, REGI_DFG* dfg, REGI_Node *current, REGI_PE* newPE, int schedule)
{
    vector<REGI_Node*> prev_nodes = current->Get_Prev_Nodes_Same_Iteration();

    //undo predecessor nodes
    for (int i = 0; i < (int) prev_nodes.size(); i++)
    {
        if (prev_nodes[i]->get_Sched_Info()->get_Current() >= schedule)
            return false;
        if (myCGRA->Physical_Distance(newPE, prev_nodes[i]->getPE()) > schedule - prev_nodes[i]->get_Sched_Info()->get_Current())
        {
            return false;
        }

        //check if load of operations
        //int to_load = schedule - prev_nodes[i]->get_Sched_Info()->get_Current() - myCGRA->Physical_Distance(newPE, prev_nodes[i]->getPE());
        //if ( (to_load + prev_nodes[i]->get_Sched_Info()->get_Current()) % myCGRA->getII() == schedule % myCGRA->getII() )
        /*int number_of_additional_nodes = myCGRA->Physical_Distance(newPE, prev_nodes[i]->getPE()) - 1;
          int this_exec_time=schedule% myCGRA->getII();
          if ( number_of_additional_nodes > 0 )
          {
          this_exec_time = schedule - number_of_additional_nodes;
          this_exec_time = this_exec_time % myCGRA->getII();
          }
          if ( ((prev_nodes[i]->get_Sched_Info()->get_Current()) % myCGRA->getII()) == this_exec_time )
          {
          return false;
          }*/
    }

    //undo successor nodes
    vector<REGI_Node*> next_nodes = current->Get_Next_Nodes_Same_Iteration();
    for (int i = 0; i < (int) next_nodes.size(); i++)
    {

        if (next_nodes[i]->get_Sched_Info()->get_Current() <= schedule)
            return false;
        if (myCGRA->Physical_Distance(newPE, next_nodes[i]->getPE()) > next_nodes[i]->get_Sched_Info()->get_Current() - schedule)
            return false;
        //int to_load = next_nodes[i]->get_Sched_Info()->get_Current() - schedule - myCGRA->Physical_Distance(newPE, next_nodes[i]->getPE());

        //if ( (to_load + schedule) % myCGRA->getII() == next_nodes[i]->get_Sched_Info()->get_Current() % myCGRA->getII() )
        /*int number_of_additional_nodes = myCGRA->Physical_Distance(newPE, next_nodes[i]->getPE()) - 1;
          int next_exec_time=next_nodes[i]->get_Sched_Info()->get_Current()% myCGRA->getII();
          if ( number_of_additional_nodes > 0 )
          {
          next_exec_time = next_nodes[i]->get_Sched_Info()->get_Current() - number_of_additional_nodes;
          next_exec_time = next_exec_time % myCGRA->getII();
          }
          if ( ((schedule) % myCGRA->getII()) == next_exec_time )
          {
          return false;
          }*/

    }

    vector<REGI_Node*> prev_nodes_all = current->Get_Prev_Nodes();

    prev_nodes = dfg->Subtract(prev_nodes_all, prev_nodes);

    //undo predecessor nodes
    for (int i = 0; i < (int) prev_nodes.size(); i++)
    {
        if (myCGRA->Physical_Distance(newPE, prev_nodes[i]->getPE()) > (myCGRA->getII() - 1))
        {
            return false;
        }
        /*if ( ((prev_nodes[i]->get_Sched_Info()->get_Current()) % myCGRA->getII()) == ((schedule - myCGRA->Physical_Distance(newPE, prev_nodes[i]->getPE())) % myCGRA->getII()) )
          {
          return false;
          }*/
        if (prev_nodes[i]->get_Sched_Info()->get_Current() > schedule)
        {
            if (myCGRA->Physical_Distance(newPE, prev_nodes[i]->getPE())
                    > (((dfg->get_Arc(prev_nodes[i], current)->get_Distance()) * myCGRA->getII())
                        - prev_nodes[i]->get_Sched_Info()->get_Current() + schedule))
            {
                return false;
            }
        }
        else
        {
            if (myCGRA->Physical_Distance(newPE, prev_nodes[i]->getPE())
                    > (((dfg->get_Arc(prev_nodes[i], current)->get_Distance()) * myCGRA->getII())
                        + prev_nodes[i]->get_Sched_Info()->get_Current() - schedule))
            {
                return false;
            }
        }

    }

    //undo successor nodes
    vector<REGI_Node*> next_nodes_all = current->Get_Next_Nodes();
    next_nodes = dfg->Subtract(next_nodes_all, next_nodes);

    for (int i = 0; i < (int) next_nodes.size(); i++)
    {

        if (myCGRA->Physical_Distance(newPE, next_nodes[i]->getPE()) > (myCGRA->getII()) - 1)
        {
            return false;
        }
        /*if ( ((schedule) % myCGRA->getII()) == ((next_nodes[i]->get_Sched_Info()->get_Current() - myCGRA->Physical_Distance(newPE, next_nodes[i]->getPE())) % myCGRA->getII()) )
          {
          return false;
          }*/
        if (schedule > next_nodes[i]->get_Sched_Info()->get_Current())
        {
            if (myCGRA->Physical_Distance(newPE, next_nodes[i]->getPE())
                    > (((dfg->get_Arc(current, next_nodes[i])->get_Distance()) * myCGRA->getII())
                        + next_nodes[i]->get_Sched_Info()->get_Current() - schedule))
            {
                return false;
            }
        }
        else
        {
            if (myCGRA->Physical_Distance(newPE, next_nodes[i]->getPE())
                    > (((dfg->get_Arc(current, next_nodes[i])->get_Distance()) * myCGRA->getII())
                        - next_nodes[i]->get_Sched_Info()->get_Current() + schedule))
            {
                return false;
            }
        }

    }

    return true;

}

void Place_Route_new(CGRA* myCGRA, REGI_DFG* dfg, REGI_Node *current, REGI_PE* newPE, int schedule)
{
    //current->Assign2PE(newPE);
    current->get_Sched_Info()->set_Current(schedule, myCGRA->getII());
    newPE->AssignNode(current);
    if (current->has_self_loop())
    {
        REGI_PE* temp_PE;
        int Xindex = newPE->getIndexI();
        int Yindex = newPE->getIndexJ();
        for (int index = 0; index < myCGRA->getII(); index++)
        {
            temp_PE = myCGRA->getPE(index * (cgra_info.X_Dim * cgra_info.Y_Dim) + Xindex * cgra_info.Y_Dim + Yindex);
            temp_PE->AllocateRegister(temp_PE->NumberOfUsedRegisters() + current->get_self_loop()->get_Distance());

        }
    }
    vector<REGI_Node*> prev_nodes = current->Get_Prev_Nodes_Same_Iteration();

    REGI_Node_Dummy* temp_2;
    int delta_time;
    int distance;
    int delta_reg;

    int X_Pos, Y_Pos, T_Pos;
    REGI_PE* temp_PE;
    REGI_PE* temp_prev_PE;

    // predecessor nodes

    for (int i = 0; i < (int) prev_nodes.size(); i++)
    {
        delta_time = schedule - prev_nodes[i]->get_Sched_Info()->get_Current();
        distance = myCGRA->Physical_Distance(newPE, prev_nodes[i]->getPE());
        vector<REGI_Node*> path;
        delta_reg = 0;
        temp_prev_PE = prev_nodes[i]->getPE();

        //register needed
        if (delta_time > distance + 1)
        {
            delta_reg = 1;

            delta_reg = delta_time - distance;
            //prev_nodes[i]->getPE()->AllocateRegister(prev_nodes[i]->getPE()->NumberOfUsedRegisters() + no_registers);
            for (int j = 0; j < delta_reg; j++)
            {
                temp_PE = myCGRA->getPE(
                        ((prev_nodes[i]->getPE()->getIndexT() + j) % myCGRA->getII()) * (cgra_info.X_Dim * cgra_info.Y_Dim)
                        + prev_nodes[i]->getPE()->getIndexI() * cgra_info.Y_Dim + prev_nodes[i]->getPE()->getIndexJ());
                temp_PE->AllocateRegister(temp_PE->NumberOfUsedRegisters() + 1);
            }
            if (distance > 0)
            {

                temp_2 = new REGI_Node_Dummy( 1, dfg->Get_Unique_Index());
                path.push_back(temp_2);
                temp_2->get_Sched_Info()->set_Current(delta_reg + prev_nodes[i]->get_Sched_Info()->get_Current(), myCGRA->getII());
                T_Pos = (prev_nodes[i]->get_Sched_Info()->get_Current() + delta_reg) % (myCGRA->getII());
                temp_PE = myCGRA->getPE(
                        (T_Pos) * (cgra_info.X_Dim * cgra_info.Y_Dim) + prev_nodes[i]->getPE()->getIndexI() * cgra_info.Y_Dim
                        + prev_nodes[i]->getPE()->getIndexJ());
                //temp_2->Assign2PE(temp_PE);
                temp_PE->AssignNode(temp_2);
                temp_prev_PE = temp_PE;
            }

        }
        else if (delta_time > 1 && delta_time == distance + 1)
        {
            temp_2 = new REGI_Node_Dummy( 1, dfg->Get_Unique_Index());
            path.push_back(temp_2);
            temp_2->get_Sched_Info()->set_Current(current->get_Sched_Info()->get_Current() - distance, myCGRA->getII());
            T_Pos = (current->get_Sched_Info()->get_Current() - distance) % (myCGRA->getII());
            temp_PE = myCGRA->getPE(
                    (T_Pos) * (cgra_info.X_Dim * cgra_info.Y_Dim) + prev_nodes[i]->getPE()->getIndexI() * cgra_info.Y_Dim
                    + prev_nodes[i]->getPE()->getIndexJ());
            //temp_2->Assign2PE(temp_PE);
            temp_PE->AssignNode(temp_2);
            temp_prev_PE = temp_PE;
        }

        for (int j = 1; j < distance; j++)
        {
            temp_2 = new REGI_Node_Dummy(1, dfg->Get_Unique_Index());
            path.push_back(temp_2);
            temp_2->get_Sched_Info()->set_Current(j + prev_nodes[i]->get_Sched_Info()->get_Current() + delta_reg, myCGRA->getII());
            T_Pos = (j + prev_nodes[i]->get_Sched_Info()->get_Current() + delta_reg) % (myCGRA->getII());

            while (true)
            {
                X_Pos = rand() % cgra_info.X_Dim;
                Y_Pos = rand() % cgra_info.Y_Dim;
                temp_PE = myCGRA->getPE((T_Pos) * (cgra_info.X_Dim * cgra_info.Y_Dim) + X_Pos * cgra_info.Y_Dim + Y_Pos);
                if (myCGRA->Physical_Distance(newPE, temp_PE) <= (distance - j) && myCGRA->connectedPEs(temp_prev_PE, temp_PE))
                {
                    //temp_2->Assign2PE(temp_PE);
                    temp_PE->AssignNode(temp_2);
                    temp_prev_PE = temp_PE;
                    break;
                }
            }

        }
        dfg->get_Arc(prev_nodes[i], current)->set_path(path);
        path.clear();

    }

    // successor nodes
    vector<REGI_Node*> next_nodes = current->Get_Next_Nodes_Same_Iteration();

    for (int i = 0; i < (int) next_nodes.size(); i++)
    {

        delta_time = next_nodes[i]->get_Sched_Info()->get_Current() - schedule;
        distance = myCGRA->Physical_Distance(newPE, next_nodes[i]->getPE());
        vector<REGI_Node*> path;
        delta_reg = 0;
        temp_prev_PE = newPE;
        if (delta_time > distance + 1)
        {
            delta_reg = 1;

            delta_reg = delta_time - distance;
            //prev_nodes[i]->getPE()->AllocateRegister(prev_nodes[i]->getPE()->NumberOfUsedRegisters() + no_registers);
            for (int j = 0; j < delta_reg; j++)
            {
                temp_PE = myCGRA->getPE(
                        ((newPE->getIndexT() + j) % myCGRA->getII()) * (cgra_info.X_Dim * cgra_info.Y_Dim) + newPE->getIndexI() * cgra_info.Y_Dim
                        + newPE->getIndexJ());
                temp_PE->AllocateRegister(temp_PE->NumberOfUsedRegisters() + 1);
            }
            if (distance > 0)
            {
                temp_2 = new REGI_Node_Dummy( 1, dfg->Get_Unique_Index());
                path.push_back(temp_2);
                temp_2->get_Sched_Info()->set_Current(delta_reg + schedule, myCGRA->getII());
                T_Pos = (delta_reg + schedule) % (myCGRA->getII());
                temp_PE = myCGRA->getPE((T_Pos) * (cgra_info.X_Dim * cgra_info.Y_Dim) + newPE->getIndexI() * cgra_info.Y_Dim + newPE->getIndexJ());
                //temp_2->Assign2PE(temp_PE);
                temp_PE->AssignNode(temp_2);
                temp_prev_PE = temp_PE;
            }
        }
        else if (delta_time > 1 && delta_time == distance + 1)
        {
            temp_2 = new REGI_Node_Dummy( 1, dfg->Get_Unique_Index());
            path.push_back(temp_2);
            temp_2->get_Sched_Info()->set_Current(next_nodes[i]->get_Sched_Info()->get_Current() - distance, myCGRA->getII());
            T_Pos = (next_nodes[i]->get_Sched_Info()->get_Current() - distance) % (myCGRA->getII());
            temp_PE = myCGRA->getPE((T_Pos) * (cgra_info.X_Dim * cgra_info.Y_Dim) + newPE->getIndexI() * cgra_info.Y_Dim + newPE->getIndexJ());
            //temp_2->Assign2PE(temp_PE);
            temp_PE->AssignNode(temp_2);
            temp_prev_PE = temp_PE;
        }
        //temp = next_nodes[i];
        for (int j = 1; j < distance; j++)
        {
            temp_2 = new REGI_Node_Dummy( 1, dfg->Get_Unique_Index());
            path.push_back(temp_2);
            temp_2->get_Sched_Info()->set_Current(j + schedule + delta_reg, myCGRA->getII());
            T_Pos = (j + schedule + delta_reg) % (myCGRA->getII());

            while (true)
            {
                X_Pos = rand() % cgra_info.X_Dim;
                Y_Pos = rand() % cgra_info.Y_Dim;
                temp_PE = myCGRA->getPE((T_Pos) * (cgra_info.X_Dim * cgra_info.Y_Dim) + X_Pos * cgra_info.Y_Dim + Y_Pos);
                if (myCGRA->Physical_Distance(next_nodes[i]->getPE(), temp_PE) <= distance - j && myCGRA->connectedPEs(temp_prev_PE, temp_PE))
                {
                    //temp_2->Assign2PE(temp_PE);
                    temp_PE->AssignNode(temp_2);
                    temp_prev_PE = temp_PE;
                    break;
                }
            }

        }
        dfg->get_Arc(current, next_nodes[i])->set_path(path);
        path.clear();

    }

    vector<REGI_Node*> prev_nodes_all = current->Get_Prev_Nodes();

    prev_nodes = dfg->Subtract(prev_nodes_all, prev_nodes);

    int load_time;
    // predecessor nodes prev iterations
    for (int i = 0; i < (int) prev_nodes.size(); i++)
    {
        vector<REGI_Node*> path;
        Allocate_Registers(dfg, myCGRA, prev_nodes[i], prev_nodes[i]->getPE(), current, newPE);
        distance = myCGRA->Physical_Distance(prev_nodes[i]->getPE(), newPE);
        if (distance > 0)
        {
            load_time = (schedule - distance) % (myCGRA->getII());
            load_time = (load_time + myCGRA->getII()) % myCGRA->getII();
            temp_2 = new REGI_Node_Dummy( 1, dfg->Get_Unique_Index());
            path.push_back(temp_2);
            temp_2->get_Sched_Info()->set_Current(load_time, myCGRA->getII());
            T_Pos = (load_time) % (myCGRA->getII());
            temp_PE = myCGRA->getPE(
                    (T_Pos) * (cgra_info.X_Dim * cgra_info.Y_Dim) + prev_nodes[i]->getPE()->getIndexI() * cgra_info.Y_Dim
                    + prev_nodes[i]->getPE()->getIndexJ());
            //temp_2->Assign2PE(temp_PE);
            temp_PE->AssignNode(temp_2);
            temp_prev_PE = temp_PE;
            for (int j = 1; j < distance; j++)
            {
                temp_2 = new REGI_Node_Dummy( 1, dfg->Get_Unique_Index());
                path.push_back(temp_2);
                temp_2->get_Sched_Info()->set_Current(j + load_time, myCGRA->getII());
                T_Pos = (j + load_time) % (myCGRA->getII());

                while (true)
                {
                    X_Pos = rand() % cgra_info.X_Dim;
                    Y_Pos = rand() % cgra_info.Y_Dim;
                    temp_PE = myCGRA->getPE((T_Pos) * (cgra_info.X_Dim * cgra_info.Y_Dim) + X_Pos * cgra_info.Y_Dim + Y_Pos);
                    if (myCGRA->Physical_Distance(newPE, temp_PE) <= distance - j && myCGRA->connectedPEs(temp_prev_PE, temp_PE))
                    {
                        //temp_2->Assign2PE(temp_PE);
                        temp_PE->AssignNode(temp_2);
                        temp_prev_PE = temp_PE;
                        break;
                    }
                }

            }
        }
        dfg->get_Arc(prev_nodes[i], current)->set_path(path);
        path.clear();
    }

    //undo successor nodes
    vector<REGI_Node*> next_nodes_all = current->Get_Next_Nodes();
    next_nodes = dfg->Subtract(next_nodes_all, next_nodes);

    for (int i = 0; i < (int) next_nodes.size(); i++)
    {

        vector<REGI_Node*> path;
        Allocate_Registers(dfg, myCGRA, current, newPE, next_nodes[i], next_nodes[i]->getPE());

        distance = myCGRA->Physical_Distance(newPE, next_nodes[i]->getPE());
        load_time = (next_nodes[i]->get_Sched_Info()->get_Current() - distance) % (myCGRA->getII());
        load_time = (load_time + myCGRA->getII()) % myCGRA->getII();
        if (distance > 0)
        {
            temp_2 = new REGI_Node_Dummy( 1, dfg->Get_Unique_Index());
            path.push_back(temp_2);
            temp_2->get_Sched_Info()->set_Current(load_time, myCGRA->getII());
            T_Pos = (load_time) % (myCGRA->getII());
            temp_PE = myCGRA->getPE((T_Pos) * (cgra_info.X_Dim * cgra_info.Y_Dim) + newPE->getIndexI() * cgra_info.Y_Dim + newPE->getIndexJ());
            //temp_2->Assign2PE(temp_PE);
            temp_PE->AssignNode(temp_2);
            temp_prev_PE = temp_PE;
            for (int j = 1; j < distance; j++)
            {
                temp_2 = new REGI_Node_Dummy( 1, dfg->Get_Unique_Index());
                path.push_back(temp_2);
                temp_2->get_Sched_Info()->set_Current(j + load_time, myCGRA->getII());
                T_Pos = (j + load_time) % (myCGRA->getII());

                while (true)
                {
                    X_Pos = rand() % cgra_info.X_Dim;
                    Y_Pos = rand() % cgra_info.Y_Dim;
                    temp_PE = myCGRA->getPE((T_Pos) * (cgra_info.X_Dim * cgra_info.Y_Dim) + X_Pos * cgra_info.Y_Dim + Y_Pos);
                    if (myCGRA->Physical_Distance(next_nodes[i]->getPE(), temp_PE) <= distance - j
                            && myCGRA->connectedPEs(temp_prev_PE, temp_PE))
                    {
                        //temp_2->Assign2PE(temp_PE);
                        temp_PE->AssignNode(temp_2);
                        temp_prev_PE = temp_PE;
                        break;
                    }
                }

            }
        }
        dfg->get_Arc(current, next_nodes[i])->set_path(path);
        path.clear();

    }

}

//done
bool Evaluate_Cost(int new_cost, int old_cost, double temp)
{
    if (new_cost < old_cost)
        return true;
    if (temp <= 0)
        return false;
    double prob = exp((old_cost - new_cost) / temp);
    double prob1 = (rand() % 1000) / (double) 1000;

    if (prob1 < prob)
        return true;
    return false;
}
//done
bool Placement_Completed(CGRA* myCGRA)
{
    int II = myCGRA->getII();
    int last_index = II * cgra_info.X_Dim * cgra_info.Y_Dim;
    for (int i = 0; i < last_index; i++)
    {
        if (myCGRA->getPE(i)->getOverUsedNumber() - 1 > 0)
            return false;
        if (myCGRA->getPE(i)->NumberOfUsedRegisters() > cgra_info.R_Size)
            return false;
    }

    return true;
}
bool Run_Out_of_Time(clock_t &Time)
{
    clock_t end = clock();
    double elapsed = ((double) (end - Time)) / CLOCKS_PER_SEC;
    if (elapsed > MAPPING_POLICY.TIME_THRESHOLD)
        return true;
    return false;
}
void Simulated_Annealing(Parser myParser)
{

    /* Temperature PArameter */
    double Current_Temperature = MAPPING_POLICY.MAX_TEMPERATURE;
    /* Temperature PArameter */

    int Random_Pos_to_Try = cgra_info.X_Dim * cgra_info.Y_Dim;
    int X_Pos, Y_Pos, T_Pos, Z_Pos;

    clock_t Time;

    bool ValidMapping = false;
    int old_cost, new_cost;
    bool accepted = false;

    REGI_Node* Current_Node;

    REGI_DFG* original_DFG=new REGI_DFG(cgra_info.X_Dim,cgra_info.Y_Dim);

    bool status  = myParser.ParseDFG(original_DFG);

    if (!status)
    {
        return;
    }

    //original_DFG->cgra_info.Y_Dim = cgra_info.Y_Dim;
    int MII;
    int resources = cgra_info.X_Dim * cgra_info.Y_Dim;

    vector<REGI_Node*> sorted_ops = original_DFG->Simulated_Annealing_SortOps(MII, resources);
    int current_II = MII;

    int list_index;
    cout << "***********II=" << current_II << "**************\n";
    while (current_II < 51)
    {

        CGRA myCGRA;

        myCGRA.Initialize(cgra_info.X_Dim, cgra_info.Y_Dim, cgra_info.R_Size, (REGI_ConnectionType) 0);

        myCGRA.SetII(current_II);
        list_index = 0;

        ValidMapping = false;
        Current_Temperature = MAPPING_POLICY.MAX_TEMPERATURE;
        Random_Initial_Place_Route(&myCGRA, original_DFG);
        //showMapping(original_DFG, &myCGRA);

        Time = clock();
        while (true)
        {
            Current_Node = sorted_ops[list_index];
            accepted = false;
            for (int i = 0; i < Random_Pos_to_Try * current_II * current_II; i++)
            {
                X_Pos = rand() % cgra_info.X_Dim;
                Y_Pos = rand() % cgra_info.Y_Dim;
                T_Pos = rand() % (3 * original_DFG->Get_Schedule_Length() + 1);
                Z_Pos = T_Pos % current_II;
                if (current_II == 1)
                {
                    Z_Pos = 0;
                }
                if (!Acceptable_Placement(&myCGRA, original_DFG, Current_Node,
                            myCGRA.getPE(Z_Pos * (cgra_info.X_Dim * cgra_info.Y_Dim) + X_Pos * cgra_info.Y_Dim + Y_Pos), T_Pos))
                    continue;
                old_cost = PlacementCost(Current_Node, Current_Temperature, &myCGRA);

                Undo_Previous_Placement(&myCGRA, original_DFG, Current_Node);
                //showMapping(original_DFG, &myCGRA);

                Place_Route_new(&myCGRA, original_DFG, Current_Node,
                        myCGRA.getPE((T_Pos % current_II) * (cgra_info.X_Dim * cgra_info.Y_Dim) + X_Pos * cgra_info.Y_Dim + Y_Pos), T_Pos);
                //showMapping(original_DFG, &myCGRA);

                new_cost = PlacementCost(Current_Node, Current_Temperature, &myCGRA);
                accepted = Evaluate_Cost(new_cost, old_cost, Current_Temperature);
                if (accepted)
                {
                    break;
                }
            }
            if (!accepted)
            {
                if (Run_Out_of_Time(Time))
                {
                    showMapping(original_DFG, &myCGRA);
                    break;
                }
                else
                {

                    continue;
                }
            }
            if (Placement_Completed(&myCGRA))
            {
                ValidMapping = true;
                break;
            }
            if (Run_Out_of_Time(Time))
            {
                showMapping(original_DFG, &myCGRA);
                break;
            }

            Current_Temperature = Current_Temperature * 0.9999999;
            list_index++;
            if (list_index == (int) sorted_ops.size())
            {

                list_index = 0;
            }

        }
        if (ValidMapping)
        {
            showMapping(original_DFG, &myCGRA);
            cout << "********************Mapping Complete at II= " << current_II << "***********************\n"; 
            break;
        }
        current_II++;
        cout << "***********II=" << current_II << "**************\n";

    }
    showSet(sorted_ops);
}

#endif /* SIMULATED_ANNEALING_H_ */
