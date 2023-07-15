/*
 * Mapper.cpp
 *
 *  Created on: Aug 10, 2012
 *      Author: mahdi
 */

#include "Mapper.h"

Mapper::Mapper(int NoReg)
{
    NoRegisters = NoReg;
    // TODO Auto-generated constructor stub

}

Mapper::~Mapper()
{
    // TODO Auto-generated destructor stub
}

int Mapper::Uncovered_size(vector<int> cover)
{
    int count = 0;
    for (int i = 0; i < (int) cover.size(); i++)
        if (cover[i] == 0)
            count++;
    return count;
}

bool Mapper::removableWithoutRegisterFile(vector<int> &neighbor, vector<int> &cover, char *table, int tableSize, int nodeIndex)
{

    for (int i = 0; i < (int) neighbor.size(); i++)
    {
        if (cover[neighbor[i]] > 0)
        {
            return false;
        }

    }
    return true;

}

int Mapper::max_removableWithoutRegisterFile(vector<vector<int> > &neighbors, vector<int> &cover, char *table, int tableSize)
{
    int r = -1, max = -1;
    for (int i = 0; i < (int) cover.size(); i++)
    {
        if (cover[i] == 0 && removableWithoutRegisterFile(neighbors[i], cover, table, tableSize, i) == true)
        {
            vector<int> temp_cover = cover;
            temp_cover[i] = 1;
            int sum = 0;
            for (int j = 0; j < (int) temp_cover.size(); j++)
                if (temp_cover[j] == 0 && removableWithoutRegisterFile(neighbors[j], temp_cover, table, tableSize, j) == true)
                    sum++;
            if (sum > max)
            {
                if (r == -1)
                {
                    max = sum;
                    r = i;
                }
                else if (neighbors[r].size() >= neighbors[i].size())
                {
                    max = sum;
                    r = i;
                }
            }
        }
    }
    return r;
}
vector<int> Mapper::procedure_1WithoutRegisterFile(vector<vector<int> > &neighbors, vector<int> &cover, char *table, int tableSize)
{
    vector<int> temp_cover = cover;
    int r = 0;
    while (r != -1)
    {
        r = max_removableWithoutRegisterFile(neighbors, temp_cover, table, tableSize);
        if (r != -1)
        {
            temp_cover[r] = 1;

        }
    }
    return temp_cover;
}
vector<int> Mapper::procedure_2WithoutRegisterFile(vector<vector<int> > &neighbors, vector<int> &cover, int k, char *table, int tableSize)
{
    int count = 0;
    vector<int> temp_cover = cover;
    vector<int> next_cover;

    for (int i = 0; i < (int) temp_cover.size(); i++)
    {
        if (temp_cover[i] == 0)
        {
            int sum = 0, index;
            for (int j = 0; j < (int) neighbors[i].size(); j++)
            {
                if (temp_cover[neighbors[i][j]] > 0)
                {
                    index = j;
                    sum++;
                }
            }
            if (sum == 1 && cover[neighbors[i][index]] > 0)
            {

                temp_cover[neighbors[i][index]] = 0;
                temp_cover[i] = 1;
                temp_cover = procedure_1WithoutRegisterFile(neighbors, temp_cover, table, tableSize);
                count++;
            }
            if (count > k)
                break;
        }
    }
    return temp_cover;
}

vector<vector<Cover_Pair*> > Mapper::MapWithoutRegisterFile(char *table, int tableSize, int cliqueSize)
{

    int best = 0;
    int CountNoImprove = 0;

    int n, i, j, k, K, p, q, r, s, min, counter = 0;
    n = tableSize;
    vector<vector<int> > graph;
    for (i = 0; i < tableSize; i++)
    {
        vector<int> row;
        for (j = 0; j < tableSize; j++)
        {
            row.push_back(table[i * tableSize + j]);
        }
        graph.push_back(row);
    }
    //Find Neighbors
    vector<vector<int> > neighbors;
    for (i = 0; i < (int) graph.size(); i++)
    {
        vector<int> neighbor;
        for (j = 0; j < (int) graph[i].size(); j++)
            if (graph[i][j] == 0)
                neighbor.push_back(j);
        neighbors.push_back(neighbor);
    }
    cout << "Graph has n = " << n << " vertices." << endl;
    //Read maximum size of Clique wanted
    cout << "Find a Clique of size at least k = " << cliqueSize << endl;
    //cin >> K;
    K = cliqueSize;
    k = n - K;
    //Find Cliques
    bool found = false;

    min = n + 1;
    vector<vector<int> > covers;

    vector<int> allcover;
    for (i = 0; i < (int) graph.size(); i++)
        allcover.push_back(0);
    for (i = 0; i < (int) allcover.size(); i++)
    {
        if (found)
            break;
        if (CountNoImprove > MAPPING_POLICY.CLIQUE_ATTEMPTS)
        {
            break;
        }
        counter++;
        //cout << counter << ". ";
        //outfile << counter << ". ";
        //vector<int> currentcover;
        vector<int> cover = allcover;
        cover[i] = 1;

        cover = procedure_1WithoutRegisterFile(neighbors, cover, table, tableSize);
        s = Uncovered_size(cover);
        cout << "Attempt total number " << counter << ", Clique Size: " << n - s << endl;

        if (s < min)
        {
            min = s;
            CountNoImprove = 0;
        }
        else
        {
            CountNoImprove++;
        }

        if (s <= k)
        {
            //outfile << "Clique (" << n - s << "): ";

            //outfile << endl;

            covers.push_back(cover);
            found = true;
            break;
        }

        for (j = 0; j < k; j++)
            cover = procedure_2WithoutRegisterFile(neighbors, cover, j, table, tableSize);
        s = Uncovered_size(cover);
        if (s < min)
        {
            min = s;
            CountNoImprove = 0;
        }
        else
        {
            CountNoImprove++;
        }
        //outfile << "Clique (" << n - s << "): ";

        //outfile << endl;

        cout << "Attempt total number " << counter << ", Clique Size: " << n - s << endl;
        covers.push_back(cover);
        if (s <= k)
        {
            found = true;
            break;
        }
    }
    CountNoImprove = 0;
    //Pairwise Intersections
    for (p = 0; p < (int) covers.size(); p++)
    {
        if (found)
            break;
        if (CountNoImprove > MAPPING_POLICY.CLIQUE_ATTEMPTS)
        {
            break;
        }
        for (q = p + 1; q < (int) covers.size(); q++)
        {
            if (found)
                break;
            if (CountNoImprove > MAPPING_POLICY.CLIQUE_ATTEMPTS)
            {
                break;
            }
            counter++;

            vector<int> cover = allcover;
            for (r = 0; r < (int) cover.size(); r++)
                if ((covers[p][r] > 0) && (covers[q][r] > 0))
                {
                    cover[r] = 1;
                }

            cover = procedure_1WithoutRegisterFile(neighbors, cover, table, tableSize);
            s = Uncovered_size(cover);
            cout << "Attempt total number " << counter << ", Clique Size: " << n - s << endl;
            if (s < min)
            {
                min = s;
                CountNoImprove = 0;
            }
            else
            {
                CountNoImprove++;
            }
            if (s <= k)
            {

                covers.push_back(cover);
                found = true;
                break;
            }
            for (j = 0; j < k; j++)
                cover = procedure_2WithoutRegisterFile(neighbors, cover, j, table, tableSize);
            s = Uncovered_size(cover);
            if (s < min)
            {
                min = s;
                CountNoImprove = 0;
            }
            else
            {
                CountNoImprove++;
            }

            //outfile << "Clique (" << n - s << "): ";

            covers.push_back(cover);
            //outfile << endl;
            cout << "Attempt total number " << counter << ", Clique Size: " << n - s << endl;

            if (s <= k)
            {
                found = true;
                break;
            }

        }
    }

    vector<vector<Cover_Pair*> > retVal;
    vector<Cover_Pair*> current;
    Cover_Pair* temp;
    for (int i = 0; i < (int) covers.size(); i++)
    {
        current.clear();
        for (int j = 0; j < (int) covers[i].size(); j++)
        {
            if (covers[i][j] > 0)
            {
                temp = new Cover_Pair();
                temp->NodeID = j;
                temp->REG = 1;

                current.push_back(temp);
            }
        }
        retVal.push_back(current);
    }
    return retVal;

}

bool Mapper::removableWithRegisterFile(vector<int> &neighbor, vector<int> cover, char *table, int tableSize, int nodeIndex)
{

    for (int i = 0; i < (int) neighbor.size(); i++)
    {
        if (cover[neighbor[i]] > 0)
        {
            return false;
        }

    }
    int edgeCount = 0;
    for (int i = 0; i < tableSize; i++)
    {

        if (cover[i] > 0)
        {

            if (cover[i] + table[i * tableSize + nodeIndex] - 2 > NoRegisters)
            {
                //cout << "Not Enough Register\n";
                return false;
            }
            edgeCount = edgeCount + table[nodeIndex * tableSize + i] - 1;
            if (edgeCount > NoRegisters)
            {
                //cout << "Not Enough Register\n";
                return false;
            }
        }

    }
    return true;

}

int Mapper::max_removableWithRegisterFile(vector<vector<int> > &neighbors, vector<int> cover, char *table, int tableSize)
{
    int r = -1, max = -1;
    for (int i = 0; i < (int) cover.size(); i++)
    {
        if (cover[i] == 0 && removableWithRegisterFile(neighbors[i], cover, table, tableSize, i) == true)
        {
            vector<int> temp_cover = cover;
            temp_cover[i] = (int) table[i * tableSize + i];
            for (int j = 0; j < tableSize; j++)
            {
                if (i == j)
                    continue;
                if (temp_cover[j] > 0)
                {

                    temp_cover[j] = temp_cover[j] + table[j * tableSize + i] - 1;
                    temp_cover[i] = temp_cover[i] + table[i * tableSize + j] - 1;

                }

            }
            int sum = 0;
            for (int j = 0; j < (int) temp_cover.size(); j++)
                if (temp_cover[j] == 0 && removableWithRegisterFile(neighbors[j], temp_cover, table, tableSize, j) == true)
                    sum++;
            if (sum > max)
            {
                if (r == -1)
                {
                    max = sum;
                    r = i;
                }
                else if (neighbors[r].size() >= neighbors[i].size())
                {
                    max = sum;
                    r = i;
                }
            }
        }
    }
    return r;
}
vector<int> Mapper::procedure_1WithRegisterFile(vector<vector<int> > &neighbors, vector<int> cover, char *table, int tableSize)
{
    vector<int> temp_cover = cover;
    int r = 0;
    while (r != -1)
    {

        r = max_removableWithRegisterFile(neighbors, temp_cover, table, tableSize);
        if (r != -1)
        {
            temp_cover[r] = table[r * tableSize + r];
            for (int i = 0; i < tableSize; i++)
            {
                if (i == r)
                    continue;
                if (temp_cover[i] > 0)
                {
                    if (table[i * tableSize + r] < 1)
                    {
                        cout << "Error Not Connected Nodes in procedure_1\n";
                    }
                    temp_cover[i] = temp_cover[i] + table[i * tableSize + r] - 1;
                    temp_cover[r] = temp_cover[r] + table[r * tableSize + i] - 1;
                    if (temp_cover[i] < 1 || temp_cover[r] < 1)
                    {
                        cout << "Error, negative value in proc 1\n";
                    }
                }

            }

        }
    }
    return temp_cover;
}
vector<int> Mapper::procedure_2WithRegisterFile(vector<vector<int> > &neighbors, vector<int> cover, int k, char *table, int tableSize)
{
    int count = 0;
    vector<int> temp_cover = cover;
    vector<int> next_cover;

    for (int i = 0; i < (int) temp_cover.size(); i++)
    {
        if (temp_cover[i] == 0)
        {
            int sum = 0, index;
            for (int j = 0; j < (int) neighbors[i].size(); j++)
            {
                if (temp_cover[neighbors[i][j]] > 0)
                {
                    index = j;
                    sum++;
                }
            }
            if (sum == 1 && cover[neighbors[i][index]] > 0)
            {

                next_cover = temp_cover;

                bool RegNoEnough = false;
                for (int j = 0; j < tableSize; j++)
                {

                    if (j == neighbors[i][index])
                        continue;
                    if (next_cover[j] > 0)
                    {
                        next_cover[j] = next_cover[j] - (table[j * tableSize + neighbors[i][index]]) + 1;

                    }
                    if (next_cover[j] < 0)
                    {
                        cout << "Error, negative value in proc 2 for prev\n";
                    }
                }
                next_cover[neighbors[i][index]] = 0;

                next_cover[i] = table[i * tableSize + i];
                for (int j = 0; j < tableSize; j++)
                {
                    if (j == i)
                        continue;
                    if (next_cover[j] > 0)
                    {
                        next_cover[i] = next_cover[i] + table[i * tableSize + j] - 1;
                        next_cover[j] = next_cover[j] + table[j * tableSize + i] - 1;

                    }
                    if (next_cover[i] < 0)
                    {
                        cout << "Error, negative value in proc 2 for i\n";
                    }
                    if (next_cover[i] > NoRegisters)
                    {
                        RegNoEnough = true;
                        break;
                    }
                }

                if (RegNoEnough)
                    continue;

                temp_cover = next_cover;

                temp_cover = procedure_1WithRegisterFile(neighbors, temp_cover, table, tableSize);

                count++;
            }
            if (count > k)
                break;
        }
    }
    return temp_cover;
}

vector<vector<Cover_Pair*> > Mapper::MapWithRegisterFile(char *table, int tableSize, int cliqueSize)
{

    int best = 0;
    int CountNoImprove = 0;

    int n, i, j, k, K, p, q, r, s, min, counter = 0;
    n = tableSize;
    vector<vector<int> > graph;
    for (i = 0; i < tableSize; i++)
    {
        vector<int> row;
        for (j = 0; j < tableSize; j++)
        {
            row.push_back(table[i * tableSize + j]);
        }
        graph.push_back(row);
    }
    //Find Neighbors
    vector<vector<int> > neighbors;
    for (i = 0; i < (int) graph.size(); i++)
    {
        vector<int> neighbor;
        for (j = 0; j < (int) graph[i].size(); j++)
            if (graph[i][j] == 0)
                neighbor.push_back(j);
        neighbors.push_back(neighbor);
    }
    cout << "Graph has n = " << n << " vertices." << endl;
    //Read maximum size of Clique wanted
    cout << "Find a Clique of size at least k = " << cliqueSize << endl;
    //cin >> K;
    K = cliqueSize;
    k = n - K;
    //Find Cliques
    bool found = false;

    min = n + 1;
    vector<vector<int> > covers;

    vector<int> allcover;
    for (i = 0; i < (int) graph.size(); i++)
        allcover.push_back(0);
    for (i = 0; i < (int) allcover.size(); i++)
    {
        if (found)
            break;
        if (CountNoImprove > MAPPING_POLICY.CLIQUE_ATTEMPTS)
        {
            break;
        }
        counter++;
        //cout << counter << ". ";
        //outfile << counter << ". ";
        //vector<int> currentcover;
        vector<int> cover = allcover;
        cover[i] = table[i * tableSize + i];

        cover = procedure_1WithRegisterFile(neighbors, cover, table, tableSize);
        s = Uncovered_size(cover);
        cout << "Attempt total number " << counter << ", Clique Size: " << n - s << endl;
        if (s < min)
        {
            min = s;
            CountNoImprove = 0;
        }
        else
        {
            CountNoImprove++;
        }

        if (s <= k)
        {
            //outfile << "Clique (" << n - s << "): ";

            //outfile << endl;

            covers.push_back(cover);
            found = true;
            break;
        }

        for (j = 0; j < k; j++)
        {
            cover = procedure_2WithRegisterFile(neighbors, cover, j, table, tableSize);

        }

        s = Uncovered_size(cover);
        if (s < min)
        {
            min = s;
            CountNoImprove = 0;
        }
        else
        {
            CountNoImprove++;
        }

        cout << "Attempt total number " << counter << ", Clique Size: " << n - s << endl;

        covers.push_back(cover);
        if (s <= k)
        {
            found = true;

            break;
        }
    }
    CountNoImprove = 0;
    //Pairwise Intersections
    for (p = 0; p < (int) covers.size(); p++)
    {
        if (found)
            break;
        if (CountNoImprove > MAPPING_POLICY.CLIQUE_ATTEMPTS)
        {
            break;
        }
        for (q = p + 1; q < (int) covers.size(); q++)
        {
            if (found)
                break;
            if (CountNoImprove > MAPPING_POLICY.CLIQUE_ATTEMPTS)
            {
                break;
            }
            counter++;

            vector<int> cover = allcover;
            for (r = 0; r < (int) cover.size(); r++)
            {
                if ((covers[p][r] > 0) && (covers[q][r] > 0))

                {
                    cover[r] = table[r * tableSize + r];
                }
            }

            for (int index1 = 0; index1 < tableSize; index1++)
            {
                if (cover[index1] > 0)
                {

                    for (int index2 = 0; index2 < tableSize; index2++)
                    {
                        if (index1 == index2)
                            continue;
                        if (cover[index2] > 0)
                        {
                            cover[index1] = cover[index1] + table[index1 * tableSize + index2] - 1;
                        }
                    }
                }
            }
            cover = procedure_1WithRegisterFile(neighbors, cover, table, tableSize);

            s = Uncovered_size(cover);
            cout << "Attempt total number " << counter << ", Clique Size: " << n - s << endl;
            if (s < min)
            {
                CountNoImprove = 0;
                min = s;
            }
            else
            {
                CountNoImprove++;
            }
            if (s <= k)
            {
                //outfile << "Clique (" << n - s << "): ";

                //outfile << endl;

                covers.push_back(cover);

                found = true;
                break;
            }
            for (int j = 0; j < k; j++)
                cover = procedure_2WithRegisterFile(neighbors, cover, j, table, tableSize);

            s = Uncovered_size(cover);
            if (s < min)
            {
                CountNoImprove = 0;
                min = s;
            }
            else
            {
                CountNoImprove++;
            }
            cout << "Attempt total number " << counter << ", Clique Size: " << n - s << endl;

            //outfile << "Clique (" << n - s << "): ";

            covers.push_back(cover);

            //outfile << endl;

            if (s <= k)
            {
                found = true;
                break;
            }

        }
    }
    Cover_Pair* temp_cover;
    vector<vector<Cover_Pair*> > retVal;
    vector<Cover_Pair*> current;
    for (int i = 0; i < (int) covers.size(); i++)
    {
        current.clear();
        cout << "\n";

        for (int j = 0; j < (int) covers[i].size(); j++)
        {
            if (covers[i][j] > 0)
            {
                temp_cover = new Cover_Pair();

                temp_cover->NodeID = j;
                temp_cover->REG = covers[i][j];

                current.push_back(temp_cover);
            }
        }
        retVal.push_back(current);
    }
    return retVal;

}
