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
  int nodeID,node_type;
  std::multimap<int,int> nodes;
  std::multimap<int,int>::iterator it;

  ifstream file1 (argv[1]);
  if(file1.is_open())
  {
    while ( getline(file1,line) )
    {
      istringstream strout(line);
      strout >> nodeID >> node_type;
      nodes.insert(std::pair<int,int>(nodeID,node_type));
    }
  }
  file1.close();
  
  ifstream file2 (argv[2]);
  if(file2.is_open())
  {
    while ( getline(file2,line) )
    {
      istringstream strout(line);
      strout >> nodeID >> node_type;
      if(nodes.count(nodeID) == 0)
          nodes.insert(std::pair<int,int>(nodeID,node_type));
    }
  }
  file2.close();

  for(it=nodes.begin();it!=nodes.end();++it)
  {
    cout<<it->first<<"\t"<<it->second<<endl;
  }

  return 0;
}
