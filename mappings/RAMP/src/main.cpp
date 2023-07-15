// Author: Mahdi Hamzeh

// Last edited: 4 March 2017
// Author: Shail Dave

#include "math.h"
#include "RAMP.h"

#define DEBUG 0
std::map<Node*,vector<Node*> > constrained_store_load_pairs;

// This contains pair of a store_data node and load_address node
// which are inserted as part of memory mode of rescheduling nodes scheduled far.
std::map<Node*,Node*> constrained_ld_succ_pairs;
std::map<Node*,Node*> constrained_st_pred_pairs;

std::map<int,int> route_rf_nodeid_scheduled_time;
std::map<int,int> node_new_inter_it_dist;

CGRA_Architecture cgra_info;
Mapping_Policy  MAPPING_POLICY;
int totalNonrecVars;
int RegisterFileFailures = 0;
int schedule_id=0;
int map_id=0;

ofstream debugfile;
std::string debugfilename = "debugfile";

int main(int argc, char *argv[])
{
  debugfile.open(debugfilename.c_str());
  //  setup cgra configuration
  cgra_info.interconnect=Simple_Connection;
  cgra_info.MAX_NODE_INDEGREE=2;
  cgra_info.MAX_NODE_OUTDEGREE=5;

  // setup mapping configurations
  MAPPING_POLICY.CLIQUE_ATTEMPTS=4;
  MAPPING_POLICY.MAX_LATENCY=200;
  MAPPING_POLICY.ENABLE_REGISTERS=1;
  MAPPING_POLICY.MAX_MAPPING_ATTEMPTS=50;

  Parser *myParser;

  // Default inputs and settings
  string node_file;
  string edge_file;
  cgra_info.X_Dim = 4;
  cgra_info.Y_Dim = 4;
  cgra_info.R_Size= 4;
  cgra_info.PER_ROW_MEM_AVAILABLE=1;
  totalNonrecVars=0;

  // process program args
  size_t found;

  if (argc > 1)
  {
    string node_arg = "-NODE";
    string edge_arg = "-EDGE";
    string mode_arg = "-MODE"; 
    string xdim_arg = "-X";
    string ydim_arg = "-Y";
    string reg_arg = "-R";
    string mod_sched_attempts = "-MSA";
    string map_attempts_II = "-MAPII";
    string max_mapping_attempts = "-MAX_MAP";
    string mapping_mode = "-MAP_MODE";
    string max_ii  = "-MAX_II";
    string lambda = "-LAMBDA";
    for (int count = 1; count < argc; count++)
    {
      found = node_arg.find(argv[count]);
      if (found == 0)
      {
        node_file = argv[count + 1];
        count = count + 1;
        continue;
      }
      found = edge_arg.find(argv[count]);
      if (found == 0)
      {
        edge_file = argv[count + 1];
        count = count + 1;
        continue;
      }
      found = xdim_arg.find(argv[count]);
      if (found == 0)
      {
        cgra_info.X_Dim = atoi(argv[count + 1]);
        count = count + 1;
        continue;
      }
      found = ydim_arg.find(argv[count]);
      if (found == 0)
      {
        cgra_info.Y_Dim = atoi(argv[count + 1]);
        count = count + 1;
        continue;
      }
      found = reg_arg.find(argv[count]);
      if (found == 0)
      {
        cgra_info.R_Size = atoi(argv[count + 1]);
        if(cgra_info.R_Size == 0)
        MAPPING_POLICY.ENABLE_REGISTERS = 0;
        count = count + 1;
        continue;
      }
      found = mod_sched_attempts.find(argv[count]);
      if(found == 0)
      {
        MAPPING_POLICY.MODULO_SCHEDULING_ATTEMPTS = atoi(argv[count + 1]);
        count = count + 1;
        continue;
      }
      found = map_attempts_II.find(argv[count]);
      if(found == 0)
      {
        MAPPING_POLICY.MAPPING_ATTEMPTS_PER_II = atoi(argv[count + 1]);
        count = count + 1;
        continue;
      }
      found = max_mapping_attempts.find(argv[count]);
      if(found == 0)
      {
        MAPPING_POLICY.MAX_MAPPING_ATTEMPTS = atof(argv[count + 1]);

        count = count + 1;
        continue;
      }
      found = mapping_mode.find(argv[count]);
      if(found == 0)
      {
        MAPPING_POLICY.MAPPING_MODE = atoi(argv[count + 1]);
        count = count + 1;
        continue;
      }
      found = max_ii.find(argv[count]);
      if(found == 0)
      {
        MAPPING_POLICY.MAX_II = atoi(argv[count + 1]);     
    
        count = count + 1;
        continue;
      }
      found = lambda.find(argv[count]); 
      if(found == 0)
      {
        MAPPING_POLICY.LAMBDA =  atof(argv[count + 1]);
    
         if(MAPPING_POLICY.LAMBDA > 1)
          _FATAL("MAPPING_POLICY.LAMBDA is percetntage and should be less than 1\n");      
  
        count = count + 1;
        continue; 
      }
      //cout << "UNKNOWN Argument\n" << node_arg << endl << edge_arg << endl<< xdim_arg << endl << ydim_arg << endl << reg_arg << endl;
      //break;
      cout << "UNKNOWN Argument:\t" << argv[count] << "\t\n";
    }
  }

  //Parser processes input files
  myParser = new Parser(node_file.c_str(), edge_file.c_str());
  srand(time(NULL));

  debugfile << "Mapping has started" << endl;
  RAMP RAMP(*myParser);

  debugfile << "Mapping is completed" << endl;
  return 0;
}
