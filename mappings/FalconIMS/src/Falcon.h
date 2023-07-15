/* This program finds a mapping using modified Falcon's algorithm.
 * Diverting from the previously used Clique based algorithm, this
 * method checks for a random mapping of each node based on MRRG. 
 * On a failure to map the node, Falcon tries to map the node, by
 * remapping some nodes in the timeslots.
 *
 * Author: Mahesh Balasubramanian
 * Date  : Sept 13, 2019
 * Note  : All the functions are implemented in header file. 
 * This file includes support for 2 scheduling algorithms 
 */        


#ifndef FALCON_H
#define FALCON_H

#include "Parser.h"
#include "INTERITERATIONDEPENDENCY.h"
#include <math.h>

class Falcon
{
  public:
    Falcon() { } //default constructor

    ~Falcon() {  } //default destructor

    // This function adds routing nodes and schedules them to the DFG
    bool Route_And_Reschedule(DFG* copy_DFG, int id, int II, int number_of_resources)
    { 
      vector<Node*> nodes =  copy_DFG->getSetOfVertices();
      vector<Node*> to_be_balanced;

      // Find the nodes to be balanced
      for(int i=0; i< (int) nodes.size(); i++)
      {
        vector<int> dist_prev, dist_next;
        vector<Node*> prev, next;
        if(copy_DFG->check_large_sched_dist(II, nodes[i], dist_prev, dist_next, prev, next))
          to_be_balanced.push_back(nodes[i]); 
      }

      for(int i=0; i< (int) nodes.size(); i++)
      {
        vector<int> dist_prev, dist_next;
        vector<Node*> prev, next;
        if(copy_DFG->check_large_sched_dist(II, nodes[i], dist_prev, dist_next, prev, next))
          to_be_balanced.push_back(nodes[i]); 
      }

      vector<Node*> route_nodes;
      int unique_id = copy_DFG->get_Unique_ID();
      bool success = false; 
      while( (int)to_be_balanced.size() > 0)
      {
        vector<Node*> change_set;
        change_set.push_back(to_be_balanced[0]);

        // Add routing nodes including path sharing.
        success = copy_DFG->Modulo_Insert_Route(unique_id, number_of_resources,change_set, route_nodes);
        //to_be_balanced = copy_DFG->Subtract(to_be_balanced, change_set);    
        copy_DFG->Subtract1(to_be_balanced, change_set);
      }
    
      if(success)
        debugfile << "Successfully inserted routing nodes to the scheduled DFG." << endl;
      //Schedule routing nodes.  
      bool modulo_schedule=false;
      int attempt=0;
      
      //atempt until you can get a valid schedule for the routing nodes. 
      while(!modulo_schedule && attempt<=MAPPING_POLICY.MODULO_SCHEDULING_ATTEMPTS)
      { 
        debugfile << "attempt=" << attempt << endl; 
        attempt++;
          // Schedule the added routing nodes. Need not pass addr and data buses as the routing nodes will not use them. 
          modulo_schedule = copy_DFG->Modulo_Schedule_Routing(number_of_resources, route_nodes);

      }

      if(!modulo_schedule)
        return false; 
   
      copy_DFG->Dot_Print_DFG_With_Schedule("schedule", 1); 
      //_FATAL("Look at the schedule_1.dot\n\n");
      return true;  
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

      // Preprocessing is needed if all nodes are immediate and making high outdegree.
      // Moreover, recomputation can be better than routing.
      // TODO: This may be better decided during (re)mapping and rescheduling of operations.
      original_DFG->PreProcess();

      //if(original_DFG->has_FPConstant())
      //{
      // Load the FP constants from memory. This is to maintain the precision.
      //  original_DFG->loadAllFPConstants(); 
      //}

      // If no register file is available to PEs,
      // Pre-process to load the nonrecurring values
      original_DFG->collectRegReq(cgra_info.R_Size);
      original_DFG->loadAllLargeConstants(cgra_info.R_Size);

      //Make a copy of the original DFG
      DFG* copy_DFG_1; //  *tempDFG;
      copy_DFG_1 = original_DFG->Copy();

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
      int low_II = copy_DFG_1->Init_Schedule(0, CGRASize, recMII);

      FATAL((low_II==-1), "Could not find feasible schedule");

      int current_number_resources = CGRASize;
      int current_II = low_II;  //start from the lowest II 

      debugfile<<"Starting II: "<<current_II<<"\trec_MII: "<< recMII <<endl;

      int current_II_1=-1;

      int id=0; 
      int mappingCounter=0;

      // Based on the prefered scheduling select IMS or CRIMSON
      // IMS Schedule
      if(MAPPING_POLICY.IMS == 1)
      {
        while(current_II <= MAPPING_POLICY.MAX_II)
        {
          copy_DFG_1->SetII(current_II); 
          //schedule the DFG
          current_II_1 = copy_DFG_1->Schedule(id++, current_number_resources);
          //The following code would be only invoked to schedule operations at feasible II.
          // After starting the attempt at MII, operations would be typically scheduled.
          // If not, it can save the compilation time by avoiding further mapping attempts

          // if no feasible schedule is found
          if (current_II_1 == -1)
          {
            //reduce number of available PEs in CGRA
            current_number_resources--;

            //if the number of PEs is less than 2, it is time to increase II
            if (current_number_resources < 2)
            {
              //reset the number of available PEs to the size of CGRA
              current_number_resources = CGRASize;
              current_II++;
            }
            continue;
          }
          //if II obtained after scheduling > current II, increase II
          else if (current_II_1 > current_II)
          {
            //reset the number of available PEs to the size of CGRA
            current_number_resources = CGRASize;
            current_II++;
            continue;
          }
 
           // Add route and schedule the routing nodes. If success move to mapping.
          if(Route_And_Reschedule(copy_DFG_1, id, current_II, current_number_resources))
          { 
            debugfile << "Start Placement for II = " << current_II << endl;

            CGRA myCGRA;
            //initialize configuration of the target CGRA
            myCGRA.Initialize(cgra_info.X_Dim, cgra_info.Y_Dim,cgra_info.R_Size, cgra_info.interconnect);
            myCGRA.SetII(current_II);  //set II
            myCGRA.ResetIteration();

            vector<Node*> leftNodeMapped;
            leftNodeMapped.clear();

            if (myCGRA.FALCON(copy_DFG_1, leftNodeMapped))
            {
              myCGRA.Prolog(copy_DFG_1, highest_distance, node_with_highest_distance);
              myCGRA.ShowMapping();    //all nodes are mapped, show the mapping
              myCGRA.EPIlog(copy_DFG_1);
              debugfile << "MII = " << low_II <<"\t Current II=" << current_II << endl;
              copy_DFG_1->Dump_Edges("DUMP");
              copy_DFG_1->Dump_Nodes("DUMP");
              copy_DFG_1->Dot_Print_DFG_With_Modulo_Schedule("Success_MS_", current_II);
              return true;
            }          
            debugfile << "Incrementing II by 1\n";
            current_II++;
            debugfile << "New II is: " << current_II << "\n";
            current_number_resources = CGRASize;
            mappingCounter++;
          }
        }
      }
      // CRIMSON Schedule
      else
      {
        cout << "II before while: " << current_II << endl; 
        while(current_II <= MAPPING_POLICY.MAX_II)
        {
          // Fraction of scheduling space based on user input.
          int lambda = (int) (MAPPING_POLICY.LAMBDA * (CGRASize * current_II * (int) copy_DFG_1->getSetOfVertices().size() ));
          int cnt=0; 

          cout << "II set in while " << current_II << endl;
          copy_DFG_1->SetII(current_II);

          while(cnt < lambda)
          {
            cnt++; 
            DFG* copy_DFG_2 = copy_DFG_1->Copy();
            current_II_1 = -1;
            int j=0; 

            // we may not find a successfull random schedule at first. Try j times.
            while (current_II_1 < 0  && j <=MAPPING_POLICY.MODULO_SCHEDULING_ATTEMPTS)      
            {
              if(j > 0) copy_DFG_2 =  copy_DFG_1->Copy();
              j++; 
              current_II_1 = copy_DFG_2->Schedule_CRIMSON(id++, current_number_resources); 
            }

            // If we do not get a valid schedule, increase the II and try again.
            if(current_II_1 == -1)
            {
              debugfile << "failure to get a random schedule for " << j << " times." << endl;
              j = 0 ;
              current_II++;
              debugfile << "New II is: " << current_II << "\n";
              mappingCounter++;
              if(current_II > MAPPING_POLICY.MAX_II)
                break;
              continue;
            }
            //if II obtained after scheduling > current II, increase II
            else if (current_II_1 > current_II)
            {
              //reset the number of available PEs to the size of CGRA
              current_number_resources = CGRASize;
              current_II++;
              continue;
            }
            copy_DFG_2->Dot_Print_DFG_With_Schedule("schedule", 0);
            //_FATAL("Look at the schedule_0.dot\n\n");
            // Add route and schedule the routing nodes. If success move to mapping.
            if(Route_And_Reschedule(copy_DFG_2, id, current_II, current_number_resources))
            {
              debugfile << "Start Placement for II = " << current_II << endl;

              CGRA myCGRA;
              //initialize configuration of the target CGRA
              myCGRA.Initialize(cgra_info.X_Dim, cgra_info.Y_Dim,cgra_info.R_Size, cgra_info.interconnect);
              myCGRA.SetII(current_II);  //set II
              myCGRA.ResetIteration();

              vector<Node*> leftNodeMapped;
              leftNodeMapped.clear();

              if (myCGRA.FALCON(copy_DFG_2, leftNodeMapped))
              {
                myCGRA.Prolog(copy_DFG_2, highest_distance, node_with_highest_distance);
                myCGRA.ShowMapping();    //all nodes are mapped, show the mapping
                myCGRA.EPIlog(copy_DFG_2);
                debugfile << "MII = " << low_II <<"\t Current II=" << current_II << endl;
                copy_DFG_2->Dump_Edges("DUMP");
                copy_DFG_2->Dump_Nodes("DUMP");
                copy_DFG_2->Dot_Print_DFG_With_Modulo_Schedule("Success_MS_", current_II);
                return true;
              }

              // If no valid mapping is found, check for another random schedule before incre,menting the II.
              if(cnt < lambda)
                continue;

              debugfile << "Incrementing II by 1\n";
              current_II++;
              debugfile << "New II is: " << current_II << "\n";
              current_number_resources = CGRASize;
              mappingCounter++;
            }
          }
        }
      }
      return false;
    }
}; 


#endif
