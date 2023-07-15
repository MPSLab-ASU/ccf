#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{

  if(argc<3)
  {
    cout << "Usage nodefile FILE1 FILE2";
    return -1;
  }

  string line;
  int nodeID,node_type,datatype;
  int temp1, temp2;
  std::multimap<std::pair<int,int>, int> nodes;
  std::map<int, int> temp; 
  std::multimap<std::pair<int,int>, int>::iterator it; 
  ifstream file1 (argv[1]);
  if(file1.is_open())
  {
    while ( getline(file1,line) )
    {
      istringstream strout(line);
      strout >> nodeID >> node_type >> temp1 >> temp2 >> datatype;
      std::pair<int,int> e1(nodeID,node_type);
      temp.insert(e1); 
      nodes.insert(std::pair<std::pair<int, int>,int>(e1, datatype));
    }
  }
  file1.close();

  ifstream file2 (argv[2]);
  if(file2.is_open())
  {
    while ( getline(file2,line) )
    {
      istringstream strout(line);
      strout >> nodeID >> node_type >> temp1 >> temp2 >> datatype;
      std::pair<int,int> e2(nodeID,node_type); 
      if(temp.count(nodeID) == 0)
        nodes.insert(std::pair<std::pair<int, int>,int>(e2, datatype));

    }
  }
  file2.close();

  for(it=nodes.begin();it!=nodes.end();++it)
  {
    cout<<it->first.first<<"\t"<<it->first.second<<"\t"<<it->second<<endl;
  }

  return 0;
}
