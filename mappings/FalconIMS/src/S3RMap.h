/* This program finds a mapping using modified SSSRMap's algorithm.
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


class SSSRMap
{

  public:
    SSSRMap() { } //default constructor

    ~SSSRMap() {  } //default destructor

    bool is_node_left(vector<Node*> leftnodes, Node* node)
    {
      for(std::vector<Node*>::iterator it = leftnodes.begin(); it != leftnodes.end(); ++it)
      {
        if((*it)->get_ID() == node->get_ID())
          return true;
      }
      return false;
    }


    tuple<bool, bool, int, int, DFG*> Route_And_Reschedule(DFG* original, DFG* copy_DFG, int id, int number_of_resources, int II, int last_time, int* Used_Resources)
    {
      //vector<int> dist_prev, dist_next;
      //bool modified = false;
      DFG* copy_DFG_temp_in;
      //vector<Node*> prev, next;
      copy_DFG_temp_in = copy_DFG->Copy_With_Schedule_And_Mapping(); // Copy_With_Schedule_And_Mapping()
      vector<Node*> nodes =  copy_DFG_temp_in->getSetOfVertices();
      vector<Node*> to_be_balanced;

      // After schedyling there maybe unncessary routing nodes added that will skew the results.

      for(int i=0; i< (int) nodes.size(); i++)
      {
        vector<int> dist_prev, dist_next;
        vector<Node*> prev, next;
        if(copy_DFG->check_large_sched_dist(II, nodes[i], dist_prev, dist_next, prev, next))
          to_be_balanced.push_back(nodes[i]); 
      }

      bool is_modified = false;

      cout << "total nodes before routing: " << copy_DFG_temp_in->getSetOfVertices().size() << endl;
      vector<Node*> route_nodes;
      int unique_id = copy_DFG_temp_in->get_Unique_ID(); 
      while( (int)to_be_balanced.size() > 0)
      {
        vector<Node*> change_set;
        change_set.push_back(to_be_balanced[0]);

        int success = copy_DFG_temp_in->Modulo_Route_ReSchedule(unique_id, number_of_resources, II, change_set, route_nodes, last_time);
        //to_be_balanced = copy_DFG_temp_in->Subtract(to_be_balanced, change_set);    
        copy_DFG_temp_in->Subtract1(to_be_balanced, change_set);

        if(success==1)
          is_modified = true;
      }


      copy_DFG_temp_in->Dot_Print_DFG_With_Schedule("MS", 0);
    _FATAL("check the Success_MS\n\n");      

      cout << "total nodes before scheduling: " << copy_DFG_temp_in->getSetOfVertices().size() << endl;
      cout << "is_modified route and reschedule: " << is_modified << endl;
      int redo_attempt = -1; 
      bool redo = true;

      redo = false;
      redo_attempt++; 
      nodes.clear();
      nodes =  copy_DFG_temp_in->getSetOfVertices();

      //copy_DFG_temp_in->Prune_Routing(original, II);

      vector<Node*> nodes_1 = copy_DFG_temp_in->Subtract(nodes, route_nodes);


      cout << "Nodes: " << (int) nodes.size() << "\tNodes_1: " << (int) nodes_1.size() << endl;

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

      cout << "modulo_schedule: " << modulo_schedule << "\tredo: " << redo << endl;
      cout << "failure reason: " << success << endl;
      cout << "total nodes after scheduling: " << copy_DFG_temp_in->getSetOfVertices().size() << endl;

      if(success==0)
      {
        cout << "Success Usage: " << endl;
        for(int ii=0; ii<II; ii++)
          cout << "PEs[" << ii << "]:" << Used_Resources[ii] << endl;   
        copy_DFG_temp_in->Dot_Print_DFG_With_Schedule("MS", 0);
        _FATAL("check the Success_MS\n\n"); 
      }

      //if(success == 2)
      //exit(1);
      return make_tuple(is_modified, redoII, new_II, success, copy_DFG_temp_in); 
    }

    bool Map(Parser myParser) 
    {
      //read input files and construct DFG
      DFG* original_DFG= new DFG(cgra_info.X_Dim,cgra_info.Y_Dim);
      bool status = myParser.ParseDFG(original_DFG);

      if (!status) //if there is an error in input files, exit
        return false;

      original_DFG->CGRA_X_Dim=cgra_info.X_Dim;
      original_DFG->CGRA_Y_Dim=cgra_info.Y_Dim;

      //DEBUG_DOTPRINT(original_DFG,"Input", 0);

      // Preprocessing is needed if all nodes are immediate and making high outdegree.
      // Moreover, recomputation can be better than routing.
      // TODO: This may be better decided during (re)mapping and rescheduling of operations.
      original_DFG->PreProcess();

      // If no register file is available to PEs,
      // Pre-process to load the nonrecurring values
      original_DFG->collectRegReq(cgra_info.R_Size);
      original_DFG->loadAllLargeConstants(cgra_info.R_Size);

      original_DFG->Dot_Print_DFG("Input", 0); 
      //exit(1);
      //Make a copy of the original DFG
      DFG* copy_DFG_1; //  *tempDFG;
      copy_DFG_1 = original_DFG->Copy_With_Schedule_And_Mapping();

      int CGRASize = cgra_info.X_Dim*cgra_info.Y_Dim;

      int height = copy_DFG_1->set_height_based_priority(); 

      if(!copy_DFG_1->is_Executable(height, CGRASize))
        _FATAL("The architecture is not compatible to accelerate the loop!!!!!"); 

      //find lowest II for the original DFG
      string latency_file = copy_DFG_1->Generate_Latency();
      INTERITERATIONDEPENDENCY LCDependency;
      int recMII  = LCDependency.calculate_recMII(myParser.getNodeFileName(), myParser.getEdgeFileName(), latency_file);
      int highest_distance = LCDependency.get_highest_distance_from_inter_iteration_dependencies();
      int node_with_highest_distance = LCDependency.get_node_with_highest_distance_from_inter_iteration_dependencies();
      cout << "highest distance: " << highest_distance << endl;
      cout << "node_with_highest_distance: " << node_with_highest_distance << endl;
      //exit(1);
      int low_II = copy_DFG_1->Init_Schedule(0, CGRASize, recMII);

      cout << "Initial schedule success with low_II: " << low_II << endl;
      FATAL((low_II==-1), "Could not find feasible schedule");

      cout << "low_II: " << low_II << endl;
      copy_DFG_1->Dot_Print_DFG("Input", 0);
      //bool isFeasible = copy_DFG_1->isFeasible_ASAP_for_architecture_constraint(low_II, CGRASize);   

      //if(!isFeasible)
      //  _FATAL("The DFG cannot be scheduled for the given architecture resources!! Please increase the resources.\n"); 

      cout << "Passed feasible" << endl;
      //set the number of available PEs
      int current_number_resources = CGRASize;

      int current_II = low_II;  //start from the lowest II
      debugfile<<"Starting II: "<<current_II<<"\trec_MII: "<< recMII <<endl;

      int current_II_1=-1,current_II_2 = current_II;

      int id=0; 
      int new_II; 
      int mappingCounter=0;
      cout << "current_II_2: " << current_II_2 << "\tmax_mapping_attempts: " << MAPPING_POLICY.MAX_MAPPING_ATTEMPTS << endl;
      //while (current_II_2 <= MAPPING_POLICY.MAX_MAPPING_ATTEMPTS)
      //copy_DFG_1 = original_DFG->Copy();
      //int Mapping_Attempts = 10 * current_II; 
      bool is_modified; 
      DFG* modified_DFG;
      int error_code = -100;
      vector<int> res_MII;
      //int estimated_res; 
      int *PEs = new int[MAPPING_POLICY.MAX_LATENCY];
      int *addr_bus = new int[MAPPING_POLICY.MAX_LATENCY];
      int *data_bus = new int [MAPPING_POLICY.MAX_LATENCY]; 
      while(current_II_2 <= MAPPING_POLICY.MAX_II)
      {
        //copy_DFG_1 =  original_DFG->Copy();

        int lambda = (int) (MAPPING_POLICY.LAMBDA * (CGRASize * current_II_2 * (int) copy_DFG_1->getSetOfVertices().size() ));
    
        if(lambda==0)
          lambda=1; 

        cout << "lambda: " << lambda << endl;
        int cnt=0; 

        while(cnt < lambda)
        {
          cnt++; 
          bool redoII = true;   
          bool ec0 = false, ec1 = false, ec2 = false, ec4 = false; 
          while(redoII && cnt < lambda)
          {
            cout << "while redoII " << endl;
            ec1 = false; ec2 = false; ec4 = false; 
            // cnt++; 
            DFG* copy_DFG_2; // = copy_DFG_1->Copy();       
            cout << "completed copy" << endl;

            //DFG* modified_DFG; 
            current_II_1 = -1;


            redoII = false; 
            if(current_II_2 > current_II)
              current_II = current_II_2;

            int schedule_len;
            int j=-1;
            for(int i=0; i < current_II; i++)
            {
              PEs[i] = 0;
              addr_bus[i] = 0;
              data_bus[i] = 0;
            }
            //copy_DFG_2->Dot_Print_DFG("debug", 6);

            while (current_II_1 < 0  && j <=MAPPING_POLICY.MODULO_SCHEDULING_ATTEMPTS ) // j <= MAPPING_POLICY.MODULO_SCHEDULING_ATTEMPTS) // I am testing j now.
            {  
              DFG* temp_DFG =  copy_DFG_1->Copy(); //copy_DFG_1->Copy_With_Schedule_And_Mapping()
              j++; 
              cout << "iteration: " << j << endl;
              //current_II_1 = copy_DFG_2->Schedule_1(id++, current_number_resources, current_II, schedule_len, PEs, addr_bus, data_bus);             
              current_II_1 = temp_DFG->Schedule_1(id++, current_number_resources, current_II, schedule_len, PEs, addr_bus, data_bus);
              //cout << "current_II_1 from inner: " << current_II_1 << endl;
              if(current_II_1 > 0)
              {   
                cout << "current_II_1 from inner: " << current_II_1 << endl;
                copy_DFG_2 = temp_DFG->Copy_With_Schedule_And_Mapping(); 
              }
            }

            //_FATAL("hi!"); 
            if(current_II_1 < 0)
            {
              redoII = true;
              debugfile << "failure to get a random schedule for " << j << " times." << endl;
              j = 0 ;
              current_II_2++;
              debugfile << "New II is: " << current_II_2 << "\n";
              current_number_resources = CGRASize;
              mappingCounter++;
              if(current_II_2 > MAPPING_POLICY.MAX_II)
                break;
              continue;
            }

            //_FATAL("Got a positive II: %d, %d", current_II_1, j);
            //int routing_nodes;
            copy_DFG_2->Dot_Print_DFG("debug", 7);
            //bool feasible;
            //int error; 
            //copy_DFG_2->print_schedule_list(current_II_1); 
            //tie(feasible, error) = copy_DFG_2->isFeasibleII(current_II_1, current_II_2, estimated_res, current_number_resources, routing_nodes);  
            /*if(!feasible)
              {
              res_MII.push_back(estimated_res); 
              cout << "Is not feasible: " << error << endl;
              cout << "total routing nodes: " << routing_nodes << endl;  
              ec4 = true;
            //error_code = 4; 
            // _FATAL("Is not feasible\n");
            redoII = true; 
            continue; 
            //break;
            }*/ 


            debugfile << "\nII after scheduling is: " << current_II_1 << "\n";
            vector<Node*> nodes = copy_DFG_2->getSetOfVertices();

            //for(int jj=0;jj<(int) nodes.size(); jj++)
            //  cout << nodes[jj]->get_ID() << " is scheduled at " << nodes[jj]->get_Sched_Info()->get_Current() << endl;

            is_modified=false;
            //int error_code;  
            tie(is_modified, redoII, new_II, error_code, modified_DFG) = Route_And_Reschedule(original_DFG, copy_DFG_2, id, current_number_resources, current_II, schedule_len, PEs);
            // cout << "is_modified: " << is_modified << "\tredoII: " << redoII << "\tnew_II: " << new_II << endl;

            // if redoII is false, the error code should be 0 which means we found a schedule! 
            if(redoII && (error_code == 2))
            {
              //current_II_2 = new_II; 
              //if(error_code == 1)
              ec1 = true; 
              // else if(error_code == 2)
              //   ec2 = true; 

              cout << "Error Code: " << error_code << endl;
              copy_DFG_2->Reset_ASAP_Feasible_Schedule();
              copy_DFG_2->Reset_ASAP_Feasible_Schedule();
              copy_DFG_2->Reset_ALAP_Schedule(); 
              copy_DFG_2->Reset_ASAP_Schedule();
              copy_DFG_2->Reset_Current_Schedule();
              continue;
            }
            else if(redoII && (error_code == 3))
            {
              cout << "Error Code: " << error_code << endl;
              //exit(1);
            }
            else
              break;   
          }
          //if(redoII)
          //  FATAL(true, "redo is true "); 
          //if(cnt == 10)
          //  FATAL(true, "cnt is 10");

          if(redoII || cnt == lambda)
          {     

            if(ec4) error_code = 4;
            if(ec0) error_code = 0; 
            //  cout << "ec1: " << ec1 << "\tec2: " << ec2 << endl;
            cout << "error_code: " << error_code << endl;
            if(error_code == 2) 
            {
              debugfile << "failure from ec 2 incrementing II." << endl;
              current_II_2++;
              debugfile << "New II is: " << current_II_2 << "\n";
              current_number_resources = CGRASize;
              res_MII.clear();
              mappingCounter++;
              continue;
            }
            else if(error_code == 3)
            {
              cout <<"Exited because of ec3" << endl; 
              continue; 
              //exit(1);
            }
            else if(error_code == 4)
            {
              debugfile << "failure from ec 4 incrementing II." << endl;
              current_II_2++;
              for(int i=0; i < (int) res_MII.size() ; i++)
                cout << "routing res_MII: " << res_MII[i] << endl;

              //_FATAL("No feasible for 10 times\n"); 
              if((int)res_MII.size() > 0)
              {
                int v = *min_element(res_MII.begin(), res_MII.end());
                if(v > current_II_2)
                  current_II_2 = v;
                else
                  current_II_2++; 
              }
              else
              {
                current_II_2++;
                debugfile << "Ec 4 addition New II is: " << current_II_2 << "\n";
                current_number_resources = CGRASize;
                mappingCounter++;
                continue;
              }
            }
            else if(error_code == 5)
            {
              cout <<"Exited because of ec5" << endl;
              exit(1);
            }
            else
            { 
              cout << "No feasible II for this II found" << endl;
              debugfile << "failure from ec 2 incrementing II." << endl;
              current_II_2++;
              debugfile << "New II is: " << current_II_2 << "\n";
              current_number_resources = CGRASize;
              res_MII.clear();
              mappingCounter++;
              continue;
            }

          } 
          cout << "redoII:" << redoII << "\tcnt: " << cnt << endl;
          debugfile << "Start Placement for II = " << current_II << endl;

          CGRA myCGRA;
          //initialize configuration of the target CGRA
          myCGRA.Initialize(cgra_info.X_Dim, cgra_info.Y_Dim,cgra_info.R_Size, cgra_info.interconnect);
          myCGRA.SetII(current_II);  //set II
          myCGRA.ResetIteration();

          debugfile << "Past CGRA reset and init" << endl;

          vector<Node*> leftNodeMapped;
          leftNodeMapped.clear();
          if(myCGRA.SubGraph_iso(modified_DFG, leftNodeMapped))
          {
            myCGRA.Prolog(modified_DFG, highest_distance, node_with_highest_distance);
            myCGRA.ShowMapping();    //all nodes are mapped, show the mapping
            myCGRA.EPIlog(modified_DFG);
            debugfile << "MII = " << low_II <<"\t Current II=" << current_II << endl; 
            modified_DFG->Dump_Edges("DUMP");
            modified_DFG->Dump_Nodes("DUMP");
            return true;
          }
          //else
          //  exit(1);

          if(cnt < lambda)
            continue;

          debugfile << "Incrementnig II by 1\n";
          current_II_2++;
          debugfile << "New II is: " << current_II_2 << "\n";
          current_number_resources = CGRASize;
          mappingCounter++;
        } // end of while(cnt < lambda)
      } // end of while(II_2 <= MAPPING ATTEMPTS)
      return false; 
    }

};
#endif
