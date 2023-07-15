/* This program finds a mapping using modified GraphMinor's algorithm.
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


class GraphMinor
{

  public:
    GraphMinor() { } //default constructor

    ~GraphMinor() {  } //default destructor


    tuple<int, DFG*> Route_And_Reschedule(DFG* copy_DFG, int id, int number_of_resources, int II, int last_time, int* Used_Resources)
    {
      //vector<int> dist_prev, dist_next;
      bool modified = false;
      DFG* copy_DFG_temp_in;
      //vector<Node*> prev, next;
      copy_DFG_temp_in = copy_DFG->Copy_With_Schedule_And_Mapping();
      vector<Node*> nodes =  copy_DFG_temp_in->getSetOfVertices();
      vector<Node*> to_be_balanced;

      // After schedyling there maybe unncessary routing nodes added that will skew the results.
      //copy_DFG_temp_in->Dot_Print_DFG("debug", 22); 
      // copy_DFG_temp_in->Prune_Routing(original, II);
      //copy_DFG_temp_in->Dot_Print_DFG("debug", 23); 
      //exit(1);

      for(int i=0; i< (int) nodes.size(); i++)
      {
        vector<int> dist_prev, dist_next;
        vector<Node*> prev, next;
        if(copy_DFG->check_large_sched_dist(II, nodes[i], dist_prev, dist_next, prev, next))
          to_be_balanced.push_back(nodes[i]); 
      }

      bool is_modified = false;
      //for(int i=0; i< (int) to_be_balanced.size(); i++)
      //cout << "to_be_balanced: " << to_be_balanced[i]->get_ID() << endl;

      cout << "total nodes before routing: " << copy_DFG_temp_in->getSetOfVertices().size() << endl;
      copy_DFG_temp_in->Dot_Print_DFG("debug", 0);
      vector<Node*> route_nodes;
      int unique_id = copy_DFG_temp_in->get_Unique_ID();
      while( (int)to_be_balanced.size() > 0)
      {
        vector<Node*> change_set;
        change_set.push_back(to_be_balanced[0]);

        int success = copy_DFG_temp_in->Modulo_Route_ReSchedule(unique_id, number_of_resources, II, change_set, route_nodes);
        //to_be_balanced = copy_DFG_temp_in->Subtract(to_be_balanced, change_set);    
        copy_DFG_temp_in->Subtract1(to_be_balanced, change_set);

        if(success==1)
          is_modified = true;
      }

      cout << "total nodes before scheduling: " << copy_DFG_temp_in->getSetOfVertices().size() << endl;
      cout << "is_modified route and reschedule: " << is_modified << endl;
      int redo_attempt = -1; 
      bool redo = true;

      redo = false;
      redo_attempt++; 
      nodes.clear();
      nodes =  copy_DFG_temp_in->getSetOfVertices();

      //copy_DFG_temp_in->Prune_Routing(original, II);
      copy_DFG_temp_in->Dot_Print_DFG("debug", 1);

      //vector<Node*> nodes_1 = copy_DFG_temp_in->Subtract(nodes, route_nodes);


      //cout << "Nodes: " << (int) nodes.size() << "\tNodes_1: " << (int) nodes_1.size() << endl;

      for(int i=0; i < (int) route_nodes.size(); i++)
        cout << "Routing Node: " << route_nodes[i]->get_ID() << "\tASAP: " << route_nodes[i]->get_Sched_Info()->get_Feasible_ASAP() << "\tALAP: " <<   route_nodes[i]->get_Sched_Info()->get_Feasible_ASAP() << endl;

      //exit(1);
      //copy_DFG_temp_in->Reset_Schedule(nodes_1); 
      //bool modulo_schedule = copy_DFG_temp_in->Modulo_Schedule(II, last_time, redo, number_of_resources);
      bool modulo_schedule; 
      int new_II, success;
      bool redo_route = true; 
      int route_sched_try = -1;
      while(redo_route && route_sched_try < 1 )
      {  
        route_sched_try++; 
        redo_route = false;
        // Schedule the added routing nodes. Need not pass addr and data buses as the routing nodes will not use them. 
        tie(modulo_schedule, new_II, success) = copy_DFG_temp_in->Modulo_Schedule_Routing(II, last_time, redo, number_of_resources, route_nodes, Used_Resources);

        if(!modulo_schedule)  
        {
          cout << "success: " << success << endl;
          //_FATAL("Cannot find a schedule  %d\n", success);
        } 

      }
      bool redoII= false; 
      if(!modulo_schedule)
        redoII = true; 

      copy_DFG_temp_in->Dot_Print_DFG("MS", 2);
      cout << "modulo_schedule: " << modulo_schedule << "\tredo: " << redo << endl;
      cout << "failure reason: " << success << endl;
      cout << "total nodes after scheduling: " << copy_DFG_temp_in->getSetOfVertices().size() << endl;

      if(success==0)
      {
        cout << "Success Usage: " << endl;
        for(int ii=0; ii<II; ii++)
          cout << "PEs[" << ii << "]:" << Used_Resources[ii] << endl;   
        copy_DFG_temp_in->Dot_Print_DFG_With_Modulo_Schedule("Success_MS_", new_II);  
      }

      //if(success == 2)
      //exit(1);
      //return make_tuple(is_modified, redoII, new_II, success, copy_DFG_temp_in); 
      return make_tuple(success, copy_DFG_temp_in); 
    }


    bool Map(Parser myParser)
    {
      DFG* original_DFG= new DFG(cgra_info.X_Dim,cgra_info.Y_Dim);
      /*
       * read input files and construct DFG
       */
      bool status = myParser.ParseDFG(original_DFG);

      //if there is an error in input files, exit
      if (!status)
      {
        return false;
      }


      original_DFG->CGRA_X_Dim=cgra_info.X_Dim;
      original_DFG->CGRA_Y_Dim=cgra_info.Y_Dim;

      DEBUG_DOTPRINT(original_DFG,"Input", 0);

      /*
       * Pre-process DFG
       * Fix indegree 
       * Fix out degree
       */
      original_DFG->PreProcess();
      original_DFG->collectRegReq(cgra_info.R_Size);
      original_DFG->loadAllLargeConstants(cgra_info.R_Size);

      if(original_DFG->has_FPConstant())
      {
        // Load the FP constants from memory. This is to maintain the precision.
        original_DFG->loadAllFPConstants();
      }

      /*
       * Make a copy of the original DFG
       */
      DFG* copy_DFG_1;
      copy_DFG_1 = original_DFG->Copy();


      vector<Node*> copy_nodes = copy_DFG_1->getSetOfVertices(); 
  

      int CGRASize = cgra_info.X_Dim*cgra_info.Y_Dim;

      /* 
       * find lowest II for the original DFG
       */
      string latency_file = copy_DFG_1->Generate_Latency();
      INTERITERATIONDEPENDENCY LCDependency;
      int recMII  = LCDependency.calculate_recMII(myParser.getNodeFileName(), myParser.getEdgeFileName(), latency_file);
      int highest_distance = LCDependency.get_highest_distance_from_inter_iteration_dependencies();
      int node_with_highest_distance = LCDependency.get_node_with_highest_distance_from_inter_iteration_dependencies();
      int low_II = copy_DFG_1->Init_Schedule(0, CGRASize, recMII);


      /* 
       * set the number of available resources
       */
      int current_number_resources = CGRASize;

      FATAL((low_II==-1), "Could not find feasible schedule");

      /*
       * start from the lowest II
       */
      int current_II = low_II;


      int current_II_1;

      int id=0;

      int *PEs = new int[MAPPING_POLICY.MAX_LATENCY];
      int *addr_bus = new int[MAPPING_POLICY.MAX_LATENCY];
      int *data_bus = new int [MAPPING_POLICY.MAX_LATENCY];

      while (current_II < MAPPING_POLICY.MAX_MAPPING_ATTEMPTS)
      {
        copy_DFG_1 = original_DFG->Copy();

        /*
         * minimize DFG with conditions
         * it looks buggy
         */
        //copy_DFG_1->Reduce_DFG();


        //DEBUG_DOTPRINT(copy_DFG_1,"Minimized",id);

        /*
         * schedule the DFG
         */

        int schedule_len; 
        current_II_1 = copy_DFG_1->Schedule_1(id++, current_number_resources, current_II, schedule_len, PEs, addr_bus, data_bus);

        /* 
         * no feasible schedule is found
         */
        if (current_II_1 == -1)
        {
          /*
           * reduce number of available resources in CGRA
           */

          current_number_resources--;
          /*
           * if the number of resources is less than 2, it is time to increase II
           */
          if (current_number_resources < 2)
          {
            /*
             * reset the number of available resources to the original number of available resources in CGRA
             */
            current_number_resources = CGRASize;
            current_II++;
          }
          continue;
        }
        /*
         * if II found after scheduling > current II, it is time to increase II
         */
        else if (current_II_1 > current_II)
        {
          /*
           * reset the number of available resources to the original number of available resources in CGRA
           */
          current_number_resources = CGRASize;
          current_II++;
          continue;
        }

        /*
         * when we start mapping, initially the number of unmapped nodes in size of nodes
         */
        int last_attempt_unmapped = copy_DFG_1->get_number_of_nodes();

        cout << "Start Placement for II = " <<  current_II << endl;

        for (int mappingAttemptCounter = 0; mappingAttemptCounter < 2; mappingAttemptCounter++)
        {
          CGRA myCGRA;

          /*
           * initialize CGRA settings
           */
          myCGRA.Initialize(cgra_info.X_Dim, cgra_info.Y_Dim,cgra_info.R_Size, cgra_info.interconnect);

          /*
           * set II 
           */
          myCGRA.SetII(current_II);
          myCGRA.ResetIteration();

          vector<Node*> leftNodeMapped;
          /*
           * place and register allocated for DFG
           */
          if(mappingAttemptCounter == 1)
          {
            copy_DFG_1->Dot_Print_DFG_With_Modulo_Schedule("current",current_II);
            //exit(1);
          }

          if (myCGRA.Minor(copy_DFG_1, leftNodeMapped))
          {
            myCGRA.Prolog(copy_DFG_1, highest_distance, node_with_highest_distance);
            myCGRA.ShowMapping();    //all nodes are mapped, show the mapping
            myCGRA.EPIlog(copy_DFG_1);
            debugfile << "MII = " << low_II <<"\t Current II=" << current_II << endl;
            copy_DFG_1->Dump_Edges("DUMP");
            copy_DFG_1->Dump_Nodes("DUMP");
            return true;
          }
          else
          {
            /*
             * if the number of nodes unplaced increased from last attempt, break
             */
            if ((int) leftNodeMapped.size() > last_attempt_unmapped)
            {
              break;
            }
            else
            {
              /*
               * sample the number of unmapped nodes
               */
              last_attempt_unmapped = (int) leftNodeMapped.size();

              vector<Node*> change_set;
              vector<Node*> nodes = copy_DFG_1->getSetOfVertices(); 
              /* 
               * Expand only the predecessor and reschedule the DFG
               */
              int success; //  = copy_DFG_1->Modulo_ReSchedule_Smart_2(id++, current_number_resources, current_II, change_set);
              //int schedule_len; 
              tie(success, copy_DFG_1) = Route_And_Reschedule(copy_DFG_1, id, current_number_resources, current_II, schedule_len, PEs);

              /*
               * failed to reschedule, break
               */
              if (success != 0)
              {
                break;
              }
            }

          }
        } 
        /* 
         * could not find mapping, increase the II and find a schedule again
         */
        debugfile << "Incrementnig II by 1\n";
        current_II++;
        debugfile << "New II is: " << current_II << "\n";
        current_number_resources = CGRASize;

      }

      return false;
    }

};
#endif
