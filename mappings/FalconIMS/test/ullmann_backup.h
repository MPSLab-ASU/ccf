/* This program finds a mapping using modified Ullmann's algorithm.
 * Diverting from the previously used Clique based algorithm this, 
 * method is predominantly used for large graph-subgraph isomorphism.
 *
 * Author: Mahesh Balasubramanian
 * Date  : Sept 13, 2019
 * Note  : All the functions are implemented in header file.  
 */        


#ifndef ULLMANN_H
#define ULLMANN_H

#include "Parser.h"
#include "INTERITERATIONDEPENDENCY.h"
#include <math.h>


class Ullmann
{

  public:
    Ullmann() { } //default constructor

    ~Ullmann() {  } //default destructor

    bool is_node_left(vector<Node*> leftnodes, Node* node)
    {
      for(std::vector<Node*>::iterator it = leftnodes.begin(); it != leftnodes.end(); ++it)
      {
        if((*it)->get_ID() == node->get_ID())
          return true;
      }
      return false;
    }


    tuple<bool, bool, int, DFG*> Route_And_Reschedule(DFG* copy_DFG, int id, int number_of_resources, int II, int last_time)
    {
      //vector<int> dist_prev, dist_next;
      bool modified = false;
      DFG* copy_DFG_temp_in;
      //vector<Node*> prev, next;
      copy_DFG_temp_in = copy_DFG->Copy_With_Schedule_And_Mapping();
      vector<Node*> nodes =  copy_DFG_temp_in->getSetOfVertices();
      vector<Node*> to_be_balanced;
      for(int i=0; i< (int) nodes.size(); i++)
      {
        vector<int> dist_prev, dist_next;
        vector<Node*> prev, next;
        if(copy_DFG->check_large_sched_dist(II, nodes[i], dist_prev, dist_next, prev, next))
          to_be_balanced.push_back(nodes[i]); 
      }

      bool is_modified = false;
      for(int i=0; i< (int) to_be_balanced.size(); i++)
        cout << "to_be_balanced: " << to_be_balanced[i]->get_ID() << endl;

      cout << "total nodes before routing: " << copy_DFG_temp_in->getSetOfVertices().size() << endl;
      copy_DFG_temp_in->Dot_Print_DFG("debug", 0);
      vector<Node*> route_nodes;
      while(to_be_balanced.size() > 0)
      {
        vector<Node*> change_set;
        change_set.push_back(to_be_balanced[0]);

        int success = copy_DFG_temp_in->Modulo_Route_ReSchedule(id++, number_of_resources, II, change_set, route_nodes);
        to_be_balanced = copy_DFG_temp_in->Subtract(to_be_balanced, change_set);    

        if(success==1)
          is_modified = true;
      }

      cout << "total nodes before scheduling: " << copy_DFG_temp_in->getSetOfVertices().size() << endl;
      cout << "is_modified: " << is_modified << endl;
      int redo_attempt = -1; 
      bool redo = true;
      //if(is_modified && redo_attempt < MAPPING_POLICY.MODULO_SCHEDULING_ATTEMPTS && redo)
      //{
        redo = false;
        redo_attempt++; 
        nodes.clear();
        nodes =  copy_DFG_temp_in->getSetOfVertices();

        copy_DFG_temp_in->Dot_Print_DFG("debug", 1);

        vector<Node*> nodes_1 = copy_DFG_temp_in->Subtract(nodes, route_nodes);
        copy_DFG_temp_in->Reset_Schedule(nodes_1); 
        //bool modulo_schedule = copy_DFG_temp_in->Modulo_Schedule(II, last_time, redo, number_of_resources);
        bool quit = false;
        bool modulo_schedule; 
        int new_II;
        tie(modulo_schedule, new_II) = copy_DFG_temp_in->Modulo_Schedule_Routing(II, last_time, redo, number_of_resources, nodes_1);
        copy_DFG_temp_in->Dot_Print_DFG("MS", 2);
        cout << "modulo_schedule: " << modulo_schedule << "\tredo: " << redo << endl;
      //}
      cout << "total nodes after scheduling: " << copy_DFG_temp_in->getSetOfVertices().size() << endl;
      exit(1);
      return make_tuple(is_modified, modulo_schedule, new_II, copy_DFG_temp_in); 
    }

    void Map(Parser myParser) 
    {
      //read input files and construct DFG
      DFG* original_DFG= new DFG(cgra_info.X_Dim,cgra_info.Y_Dim);
      bool status = myParser.ParseDFG(original_DFG);

      if (!status) //if there is an error in input files, exit
        return;

      original_DFG->CGRA_X_Dim=cgra_info.X_Dim;
      original_DFG->CGRA_Y_Dim=cgra_info.Y_Dim;

      DEBUG_DOTPRINT(original_DFG,"Input", 0);

      // Preprocessing is needed if all nodes are immediate and making high outdegree.
      // Moreover, recomputation can be better than routing.
      // TODO: This may be better decided during (re)mapping and rescheduling of operations.
      original_DFG->PreProcess();

      // If no register file is available to PEs,
      // Pre-process to load the nonrecurring values
      original_DFG->collectRegReq(cgra_info.R_Size);
      original_DFG->loadAllLargeConstants(cgra_info.R_Size);

      //Make a copy of the original DFG
      DFG* copy_DFG_1, *copy_DFG_2, *tempDFG;
      copy_DFG_1 = original_DFG->Copy();

      int CGRASize = cgra_info.X_Dim*cgra_info.Y_Dim;

      //find lowest II for the original DFG
      string latency_file = copy_DFG_1->Generate_Latency();
      INTERITERATIONDEPENDENCY LCDependency;
      int recMII  = LCDependency.calculate_recMII(myParser.getNodeFileName(), myParser.getEdgeFileName(), latency_file);
      int highest_distance = LCDependency.get_highest_distance_from_inter_iteration_dependencies();
      int node_with_highest_distance = LCDependency.get_node_with_highest_distance_from_inter_iteration_dependencies();
      //cout << "highest distance: " << highest_distance << endl;
      //cout << "node_with_highest_distance: " << node_with_highest_distance << endl;
      //exit(1);
      int low_II = copy_DFG_1->Init_Schedule(0, CGRASize, recMII);

      FATAL((low_II==-1), "Could not find feasible schedule");

      //set the number of available PEs
      int current_number_resources = CGRASize;

      int current_II = low_II;  //start from the lowest II
      debugfile<<"Starting II: "<<current_II<<"\trec_MII: "<< recMII <<endl;

      int current_II_1=-1,current_II_2 = current_II;

      int id=0;
      bool redoII = true;
      int new_II; 
      int mappingCounter=0;
      cout << "current_II_2: " << current_II_2 << "\tmax_mapping_attempts: " << MAPPING_POLICY.MAX_MAPPING_ATTEMPTS << endl;
      //while (current_II_2 <= MAPPING_POLICY.MAX_MAPPING_ATTEMPTS)
      //copy_DFG_1 = original_DFG->Copy();
      //int Mapping_Attempts = 10 * current_II; 
      while(current_II_2 <= MAPPING_POLICY.MAX_II)
      {
        copy_DFG_1 = original_DFG->Copy();
        copy_DFG_2 = copy_DFG_1->Copy();

        if(current_II_2 > current_II)
          current_II = current_II_2;

        debugfile << "\nTrying to schedule DFG at II: " << current_II << "\n";
        copy_DFG_1->Dot_Print_DFG("original", 0);
        int org_size = copy_DFG_1->getSetOfVertices().size();

        //schedule the DFG 
        //Schedule_2 attempts scheduling try times(10) to find a schedule and updates the ASAP and ALAP.
        int schedule_len=0, old_schedule_len=0;
        int map_attempt_per_II = -1;
        bool no_sched = false;
        while(map_attempt_per_II <= MAPPING_POLICY.MAPPING_ATTEMPTS_PER_II)
        {
          map_attempt_per_II++; 
          int modulo_sched_attempts = 0;


          //while(modulo_sched_attempts <= MAPPING_POLICY.MODULO_SCHEDULING_ATTEMPTS)
          while(modulo_sched_attempts < 1)
          {
            current_II_1 = -1;
            //copy_DFG_2 = copy_DFG_1;
            modulo_sched_attempts++;
            int attempt = -1;
            int sched_try = -1;
            /*while(current_II_1 < 0 && sched_try < MAPPING_POLICY.MAX_SCHED_TRY)
            {
              sched_try++;
              if(sched_try == MAPPING_POLICY.MAX_SCHED_TRY)
              {
                no_sched = true;
                break;
              }
              copy_DFG_2 = copy_DFG_1->Copy();//->Copy_With_Schedule_And_Mapping();
              current_II_1 = copy_DFG_2->Schedule_1(id++, current_number_resources, current_II, schedule_len, org_size);
              cout << "current_II_1: " << current_II_1 << "\tsched len:" << schedule_len << endl;
            }*/
            
            while(current_II_1 < 0)
            {
              //copy_DFG_2 = copy_DFG_1->Copy();
              current_II_1 = copy_DFG_2->Schedule_1(id++, current_number_resources, current_II, schedule_len, org_size);              
            }
            //cout << "mod_attempts: " << modulo_sched_attempts << endl;
            cout << "current_II_1: " << current_II_1 << endl; // "\tsched len:" << schedule_len << endl;  
            //if(no_sched)
            //  break;
            //exit(1);
            /*if(modulo_sched_attempts == 0) 
              {
              old_schedule_len = schedule_len;
              cout << "old_sched: " << old_schedule_len << "\tsched_len: " << schedule_len << "\tattempt: 0"<< endl;
            //cout << "current_II_1: " << current_II_1 << endl;
            tempDFG = copy_DFG_2->Copy_With_Schedule_And_Mapping();
            }
            else
            {
            if(old_schedule_len > schedule_len)
            {
            old_schedule_len = schedule_len;
            cout << "old_sched: " << old_schedule_len << "\tsched_len: " << schedule_len << endl;
            tempDFG = copy_DFG_2->Copy_With_Schedule_And_Mapping(); 
            }

            }*/
          }

          //if(current_II_1 == -1)
          //{  
          //  debugfile << "No Schedule found for modulo scheduling attempts: " << modulo_sched_attempts << endl;
          //  continue;
         // }
         // if(current_II_1 == -1)
         // {
         //   debugfile << "No Schedule found for mapping attempts per II:  " << map_attempt_per_II << endl;
         //   continue;
         // }
          // tempDFG has the lowest schedule len scheduling.
          //copy_DFG_2 = tempDFG->Copy_With_Schedule_And_Mapping(); 
          cout << "After first scheduling II: " <<  current_II_1 << "\tsched_len: " << old_schedule_len << endl;

          debugfile << "\nII after scheduling is: " << current_II_1 << "\n";
          vector<Node*> nodes = copy_DFG_2->getSetOfVertices();

          for(int jj=0;jj<(int) nodes.size(); jj++)
            cout << nodes[jj]->get_ID() << " is scheduled at " << nodes[jj]->get_Sched_Info()->get_Current() << endl;

          bool is_modified=false; 
          DFG* modified_DFG;
          
          tie(is_modified, redoII, new_II, modified_DFG) = Route_And_Reschedule(copy_DFG_2, id, current_number_resources, current_II, schedule_len);
          
          if(redoII)
          {
            current_II_2 = new_II;
            continue; 
          }

          bool no_schedule = false;
          if(is_modified)
          {
            vector<Node*> nodes_1 = modified_DFG->getSetOfVertices();
            for(int jj=0;jj<(int) nodes_1.size(); jj++)
              cout << "Balancing Node " << nodes_1[jj]->get_ID() << " is scheduled at " << nodes_1[jj]->get_Sched_Info()->get_Current() << endl;

            for(int jj=0;jj<(int) nodes_1.size(); jj++)
              if(nodes_1[jj]->get_Sched_Info()->get_Current() == -1)
                no_schedule = true;

          }
          //if any node's schedule is -1 there is not schedule found so break;
          //if(no_schedule)
          //  break;

          //exit(1);
          /* The following code would be only invoked to schedule operations at feasible II.
           * After starting the attempt at MII, operations would be typically scheduled.
           * If not, it can save the compilation time by avoiding further mapping attempts
           */
          // if no feasible schedule is found
          //if (current_II_1 == -1) // current_II_1 
          //{
          //reduce number of available PEs in CGRA
          //current_number_resources--;

          //if the number of PEs is less than 2, it is time to increase II
          // if (current_number_resources < 2)
          // {
          //reset the number of available PEs to the size of CGRA
          //   current_number_resources = CGRASize;
          //   current_II++;
          // }
          // continue;
          //}
          //if II obtained after scheduling > current II, increase II
          //else if (current_II_1 > current_II) //current_II_1
          //{
          //reset the number of available PEs to the size of CGRA
          //  current_number_resources = CGRASize;
          //  current_II++;
          //  continue;
          //}

          // when we start mapping, initially the number of unmapped operations
          // is same as total operations in DDG
          //int last_attempt_unmapped_x = copy_DFG_1->get_number_of_nodes();

          debugfile << "Start Placement for II = " << current_II << endl;

          CGRA myCGRA;
          //initialize configuration of the target CGRA
          myCGRA.Initialize(cgra_info.X_Dim, cgra_info.Y_Dim,cgra_info.R_Size, cgra_info.interconnect);
          myCGRA.SetII(current_II);  //set II
          myCGRA.ResetIteration();

          debugfile << "Past CGRA reset and init" << endl;

          vector<Node*> leftNodeMapped;
          leftNodeMapped.clear();
          //Add Ullmann algorithm details here.
          //if (myCGRA.SubGraph_iso(copy_DFG_1, leftNodeMapped))
          if(myCGRA.SubGraph_iso(modified_DFG, leftNodeMapped))
          {
            //myCGRA.Prolog(copy_DFG_1, highest_distance, node_with_highest_distance);
            myCGRA.Prolog(modified_DFG, highest_distance, node_with_highest_distance);
            myCGRA.ShowMapping();    //all nodes are mapped, show the mapping
            //myCGRA.EPIlog(copy_DFG_1);
            myCGRA.EPIlog(modified_DFG);
            debugfile << "MII = " << low_II <<"\t Current II=" << current_II << endl;
            //copy_DFG_1->Dump_Edges("DUMP");
            //copy_DFG_1->Dump_Nodes("DUMP");
            modified_DFG->Dump_Edges("DUMP");
            modified_DFG->Dump_Nodes("DUMP");
            return;
          }
          //for(int jj=0;jj<(int) nodes.size(); jj++)
          //  nodes[jj]->get_Sched_Info()->reset_all();

          // Time to increase II if not able to map
          // remaining operations in several attempts of rescheduling and mapping
          if(no_sched)
          {
            no_sched = false;
            break;
          }
        }

        debugfile << "Incrementnig II by 1\n";
        current_II_2++;
        debugfile << "New II is: " << current_II_2 << "\n";
        current_number_resources = CGRASize;
        mappingCounter++;  
      }
    }
};
#endif
