/*
 *  (Previously Mapping through REGIMap Wrapper,
 *  Created on: Jul 6, 2013
 *  Author: Mahdi Hamzeh
 *  Arizona State University)
 *
 *  Now, models RAMP
 *  Author: Shail Dave
 *  Last Modified: Dec 16 2017
 */

#include "RAMP.h"
#include "INTERITERATIONDEPENDENCY.h"
#include <math.h>

RAMP::RAMP(Parser myParser)
{
  //read input files and construct DDG
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
  DFG* copy_DFG_1;
  copy_DFG_1 = original_DFG->Copy();

  int CGRASize = cgra_info.X_Dim*cgra_info.Y_Dim;

  //find lowest II for the original DFG
  string latency_file = copy_DFG_1->Generate_Latency();
  INTERITERATIONDEPENDENCY LCDependency;
  int recMII  = LCDependency.calculate_recMII(myParser.getNodeFileName(), myParser.getEdgeFileName(), latency_file);
  int highest_distance = LCDependency.get_highest_distance_from_inter_iteration_dependencies();
  int node_with_highest_distance = LCDependency.get_node_with_highest_distance_from_inter_iteration_dependencies();
  int low_II = copy_DFG_1->Init_Schedule(0, CGRASize, recMII);

  FATAL((low_II==-1), "Could not find feasible schedule");

  //set the number of available PEs
  int current_number_resources = CGRASize;

  int current_II = low_II;	//start from the lowest II
  debugfile<<"Curerent II: "<<current_II<<"\trec_MII: "<< recMII <<endl;

  int current_II_1,current_II_2 = current_II;

  int id=0;

  //x attempts of mapping
  while (current_II_2 <= MAPPING_POLICY.MAX_MAPPING_ATTEMPTS)
  {
     if(current_II_2 > current_II)
      current_II = current_II_2;

     debugfile << "\nTrying to schedule DDG at II: " << current_II << "\n";
     copy_DFG_1 = original_DFG->Copy();
     //schedule the DDG
     current_II_1 = copy_DFG_1->Schedule(id++, current_number_resources, current_II);
     debugfile << "\nII after scheduling is: " << current_II_1 << "\n";

     /* The following code would be only invoked to schedule operations at feasible II.
     * After starting the attempt at MII, operations would be typically scheduled.
     * If not, it can save the compilation time by avoiding further mapping attempts
     */
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

     // when we start mapping, initially the number of unmapped operations
     // is same as total operations in DDG
     unsigned int last_attempt_unmapped_x = copy_DFG_1->get_number_of_nodes();

     debugfile << "Start Placement for II = " << current_II << endl;

     CGRA myCGRA;
     //initialize configuration of the target CGRA
     myCGRA.Initialize(cgra_info.X_Dim, cgra_info.Y_Dim,cgra_info.R_Size, cgra_info.interconnect);
     myCGRA.SetII(current_II);	//set II
     myCGRA.ResetIteration();

     vector<Node*> leftNodeMapped, leftNodeMapped_x, leftNodeMapped_y;
     leftNodeMapped.clear();
     leftNodeMapped_x.clear();
     leftNodeMapped_y.clear();

     if (myCGRA.MCS2(copy_DFG_1, leftNodeMapped))
     {
       myCGRA.Prolog(copy_DFG_1, highest_distance, node_with_highest_distance);
       myCGRA.ShowMapping();		//all nodes are mapped, show the mapping
       myCGRA.EPIlog(copy_DFG_1);
       debugfile << "MII = " << low_II <<"\t Current II=" << current_II << endl;
       copy_DFG_1->Dump_Edges("DUMP");
       copy_DFG_1->Dump_Nodes("DUMP");
       return;
     }

     // if the number of nodes unplaced operations increased from last attempt, break
     if (leftNodeMapped.size() > last_attempt_unmapped_x)
      continue;

     //sample the number of unmapped nodes
     last_attempt_unmapped_x = (int) leftNodeMapped.size();

     //copy set x to set y, to be attempted over y iterations
     for (unsigned int i = 0; i < last_attempt_unmapped_x; i++) {
       leftNodeMapped_x.push_back(leftNodeMapped[i]);
       debugfile << "Left nodes: " << leftNodeMapped[i]->get_ID() << endl;
     }

     while((int) leftNodeMapped_x.size() > 0)
     {
       //check if node n is still there in the left node list

       //Y attempts are needed to figure out the best possible choice of the rescheduling and mapping
       for (int mappingAttemptCounter = 0; mappingAttemptCounter < 1; mappingAttemptCounter++)
       {
         bool success = false;
         int best_option = -1;
         vector<int> dist_prev, dist_next;

         DFG* copy_DFG_temp_in;
         vector<Node*> prev, next;

         myCGRA.SetII(current_II);	//set II
         myCGRA.ResetIteration();

         copy_DFG_temp_in = copy_DFG_1->Copy_With_Schedule_And_Mapping();
         debugfile << "Left node " << leftNodeMapped_x[0]->get_ID()  << endl;
         if(!copy_DFG_1->check_node_failure_due_to_large_sched_dist(current_II, copy_DFG_1->get_Node(leftNodeMapped_x[0]->get_ID()), dist_prev, dist_next, prev, next))
         {
           debugfile << "Failed due to immediately scheduled nodes " << leftNodeMapped_x[0]->get_ID() << "\tattempt: " << mappingAttemptCounter << "\n";

           vector<Node*> to_change_set;
           to_change_set.push_back(copy_DFG_temp_in->get_Node(leftNodeMapped_x[0]->get_ID()));
           int success = copy_DFG_temp_in->Modulo_ReSchedule_Smart_2(0, current_number_resources, current_II, to_change_set);
           if(success == 0)
            continue;

           // Find Compatibility and perform P&R
           if (myCGRA.MCS2(copy_DFG_temp_in, leftNodeMapped_y))
           {
             success = true;
             myCGRA.Prolog(copy_DFG_temp_in, highest_distance, node_with_highest_distance);
             myCGRA.ShowMapping();
             myCGRA.EPIlog(copy_DFG_temp_in);
             debugfile << "MII = " << low_II <<"\t Current II=" << current_II << endl;
             debugfile << "RegisterFileFailureCount "<< RegisterFileFailures << endl;
             copy_DFG_temp_in->Dump_Edges("DUMP");
             copy_DFG_temp_in->Dump_Nodes("DUMP");
             return;
           }
           else
           {
             if(leftNodeMapped_y.size() > leftNodeMapped_x.size())
             {
               success = false;
             }
             else
             {
               debugfile << "update left nodes.\n";
               leftNodeMapped_x = leftNodeMapped_y;
               debugfile << leftNodeMapped_x.size() << endl;
               success = true;
               break;
             }
           }
         }
         else
         {
           vector<Node*> left_dependent_nodes = prev;
           left_dependent_nodes.insert(left_dependent_nodes.end(), next.begin(), next.end());

           vector<int> distance = dist_prev;
           distance.insert(distance.end(),dist_next.begin(),dist_next.end());

           int pred_node, succ_node;
           vector<int> priorCompatibleNodes;
           Node* leftNode = leftNodeMapped_x[0];

           debugfile << "Failed due to reg requirement for node " << leftNodeMapped_x[0]->get_ID() << "\tattempt: " << mappingAttemptCounter << "\n";

           while(left_dependent_nodes.size() != 0)
           {
             //update the list of previous and next nodes, removing current node.
             // This will allow to keep distantly scheduled nodes.
             // Rescheduling functions will modify all of them for path sharing purpose.
             if(std::find(prev.begin(), prev.end(), left_dependent_nodes[0]) != prev.end())
             {
               pred_node = left_dependent_nodes[0]->get_ID();
               succ_node = leftNode->get_ID();
               prev.erase(prev.begin());
             }
             else if(std::find(next.begin(), next.end(), left_dependent_nodes[0]) != next.end())
             {
               succ_node = left_dependent_nodes[0]->get_ID();
               pred_node = leftNode->get_ID();
               next.erase(next.begin());
             }
             else
               FATAL(true,"Should Not Reach here. What is Reason?\n");

             debugfile << "Rescheduling pair: " << pred_node << " -> " << succ_node << endl;
             bool isLeftMapped = false;
             if(pred_node != succ_node)
              isLeftMapped = is_node_left(leftNodeMapped_x, left_dependent_nodes[0]);

             debugfile << left_dependent_nodes[0]->get_ID() << " is left mapped: " << isLeftMapped << endl;

             if(isLeftMapped)
             {
               left_dependent_nodes.erase(left_dependent_nodes.begin());
               distance.erase(distance.begin());
               continue;
             }

             bool need_to_reschedule = false;

             //If Pair Is Compatible, No Need To Reschedule.
             if(myCGRA.IsCompatible(pred_node,succ_node))
             {
               debugfile << pred_node << " and " << succ_node << " are Compatible.\n";
               //Need to check compatibility among all prior nodes
               for(unsigned i=0; i<priorCompatibleNodes.size(); i++)
               {
                 if((!myCGRA.IsCompatible(pred_node,priorCompatibleNodes[i])) || (!myCGRA.IsCompatible(priorCompatibleNodes[i],succ_node)))
                 {
                   need_to_reschedule = true;
                   break;
                 }
                 debugfile << pred_node << " and " << priorCompatibleNodes[i] << " are Compatible.\n";
                 debugfile << priorCompatibleNodes[i] << " and " << succ_node << " are Compatible.\n";
               }

               if(!need_to_reschedule)
               {
                 debugfile << "Discarding Left node " << leftNodeMapped_x[0]->get_ID()  << endl;
                 priorCompatibleNodes.push_back(left_dependent_nodes[0]->get_ID());
                 left_dependent_nodes.erase(left_dependent_nodes.begin());
                 distance.erase(distance.begin());
                 if(left_dependent_nodes.size() > 0) continue;
               }
             }
             //Otherwise, reshcedule the pair.
             debugfile << "Rescheduling pair: " << pred_node << " -> " << succ_node << endl;
             Node* new_node;
             myCGRA = Smart_Rechedule_Map(leftNode->get_ID(), pred_node, succ_node, copy_DFG_temp_in, last_attempt_unmapped_x, current_II,
             distance[0], prev, next, best_option, leftNodeMapped_y, new_node);

             if(best_option >= 0)
              success = true;
             debugfile << "success of mapping y is " << success << endl;

             if(success == true)
             {
               debugfile << "NodeID is: " << new_node->get_ID() << "\n";
               debugfile << "Success! number of left nodes are: " <<  leftNodeMapped_y.size() << endl;

               for(unsigned int nn=0; nn<leftNodeMapped_y.size(); nn++)
                debugfile << "Left nodes are: " << leftNodeMapped_y[nn]->get_ID() << endl;

               //If no node is left mapped, show mapping
               if(leftNodeMapped_y.size() == 0)
               {
                 //all nodes are mapped, show the mapping
                 myCGRA.Prolog(copy_DFG_temp_in, highest_distance, node_with_highest_distance);
                 myCGRA.ShowMapping();
                 myCGRA.EPIlog(copy_DFG_temp_in);
                 debugfile << "MII = " << low_II <<"\t Current II=" << current_II << endl;
                 debugfile << "RegisterFileFailureCount "<< RegisterFileFailures << endl;
                 copy_DFG_temp_in->Dump_Edges("DUMP");
                 copy_DFG_temp_in->Dump_Nodes("DUMP");
                 return;
               }

               if(left_dependent_nodes.size() > 0)
                left_dependent_nodes.erase(left_dependent_nodes.begin());

               if(distance.size() > 0)
                distance.erase(distance.begin());

               if(leftNode->get_ID() == succ_node) break;

               leftNode = new_node;
               continue;
            }
            else
              break;
          }

         //All pairs for the left node are mapped
         if(success == true)
         {
           // Update the list of left nodes,
           // update attempt number and DFG
           debugfile << "update left nodes.\n";
           copy_DFG_1 = copy_DFG_temp_in->Copy_With_Schedule_And_Mapping();
           debugfile << leftNodeMapped_x.size() << endl;
           if(leftNodeMapped_y.size() == 0)
           {
             //all nodes are mapped, show the mapping
             myCGRA.Prolog(copy_DFG_temp_in, highest_distance, node_with_highest_distance);
             myCGRA.ShowMapping();
             myCGRA.EPIlog(copy_DFG_temp_in);
             debugfile << "MII = " << low_II <<"\t Current II=" << current_II << endl;
             debugfile << "RegisterFileFailureCount "<< RegisterFileFailures << endl;
             copy_DFG_temp_in->Dump_Edges("DUMP");
             copy_DFG_temp_in->Dump_Nodes("DUMP");
             return;
           }
           break;
         }
         else		//continue mapping attempt from beginning of left nodes.
         continue;
       }
     }

     if(leftNodeMapped_y.size() > last_attempt_unmapped_x)
     {
       debugfile << "More nodes unmapped. Incrementnig II by 1\n";
       // Time to increase II if cannot to map
       // unmapped operations in several attempts of rescheduling and mapping
       current_II_2++;
       break;
     }
     else if(leftNodeMapped_y.size() == last_attempt_unmapped_x)
     {
       //Check whether the new node (resutant routing operation) is being unmapped
       std::vector<Node*>::iterator it1, it2;
       bool some_new_node_unmapped = true;
       for(it1 = leftNodeMapped_y.begin(); it1 != leftNodeMapped_y.end(); it1++)
       {
         some_new_node_unmapped = true;
         for(it2 = leftNodeMapped_x.begin(); it2 != leftNodeMapped_x.end(); it2++)
         {
           debugfile << (*it1)->get_ID() << "\t" << (*it2)->get_ID() << "\n";
           if((*it1)->get_ID() == (*it2)->get_ID())
           {
             some_new_node_unmapped = false;
             break;
           }
         }
         if(some_new_node_unmapped) break;
       }
       leftNodeMapped_x = leftNodeMapped_y;
       if(!some_new_node_unmapped) break;
     }
     else
     {
       leftNodeMapped_x = leftNodeMapped_y;
     }
     last_attempt_unmapped_x = leftNodeMapped_y.size();
   }

   // Time to increase II if not able to map
   // remaining operations in several attempts of rescheduling and mapping
   debugfile << "Incrementnig II by 1\n";
   current_II_2++;
   debugfile << "New II is: " << current_II_2 << "\n";
   current_number_resources = CGRASize;
  }
}

CGRA RAMP::Smart_Rechedule_Map(int left_node_id, int pred_id, int succ_id, DFG* &inDFG, unsigned int last_attempt_unmapped, int II, int distance, vector<Node*> prev, vector<Node*> next,
int &best_option, vector<Node*> &best_leftmapped, Node* &new_node)
{
 //Declare output parameters for possible options to reschedule DFG
 bool  valid_route_rf = false, valid_route = false, valid_mem = false, valid_rec = false;
 float util_route_rf = 0.0, util_route = 0.0, util_mem = 0.0, util_rec = 0.0;
 vector<Node*> leftNodeMapped_route_rf, leftNodeMapped_route, leftNodeMapped_mem, leftNodeMapped_rec;
 Node* new_node_route = NULL;
 Node* new_node_mem = NULL;
 Node* new_node_route_rf = NULL;
 vector<Node*> inserted_mem_nodes;

 best_option = -1;
 float best_util = ((float)cgra_info.X_Dim*cgra_info.Y_Dim + 1.0);
 unsigned int best_last_attempt_unmapped = last_attempt_unmapped;

 //Make a copy of the original DFG
 DFG* copy_DFG_route_rf = inDFG->Copy_With_Schedule_And_Mapping();
 DFG* copy_DFG_route    = inDFG->Copy_With_Schedule_And_Mapping();
 DFG* copy_DFG_rec      = inDFG->Copy_With_Schedule_And_Mapping();
 DFG* copy_DFG_mem      = inDFG->Copy_With_Schedule_And_Mapping();

 CGRA CGRA_route_rf;
 CGRA CGRA_route;
 CGRA CGRA_rec;
 CGRA CGRA_mem;

 CGRA_route_rf.Initialize(cgra_info.X_Dim, cgra_info.Y_Dim, cgra_info.R_Size, cgra_info.interconnect);	//initialize CGRA settings
 CGRA_route.Initialize(cgra_info.X_Dim, cgra_info.Y_Dim, cgra_info.R_Size, cgra_info.interconnect);
 CGRA_rec.Initialize(cgra_info.X_Dim, cgra_info.Y_Dim, cgra_info.R_Size, cgra_info.interconnect);
 CGRA_mem.Initialize(cgra_info.X_Dim, cgra_info.Y_Dim, cgra_info.R_Size, cgra_info.interconnect);

 //Call Functions For Possible Rescheduling Options
 Route_RF_ReSchedule_Map(pred_id, succ_id, II, last_attempt_unmapped, copy_DFG_route_rf, distance, prev, next, valid_route_rf, util_route_rf, leftNodeMapped_route_rf, new_node_route_rf,CGRA_route_rf);
 Route_Reschedule_Map(left_node_id, pred_id, succ_id, II, last_attempt_unmapped, copy_DFG_route, distance, prev, next, valid_route, util_route, leftNodeMapped_route, new_node_route, CGRA_route);
 //Mem_Reschedule_Map(  left_node_id, pred_id, succ_id, II, last_attempt_unmapped, copy_DFG_mem,   distance, prev, next, valid_mem,   util_mem,   leftNodeMapped_mem,   new_node_mem,   CGRA_mem, inserted_mem_nodes);
 //copy_DFG_mem->Rec_Reschedule_Map(    left_node_id, II, last_attempt_unmapped, current_number_resources, copy_DFG_mem, valid_rec,   II_rec,   util_rec,   leftNodeMapped_rec, CGRA_mem);

 debugfile << "modes are: " << valid_route_rf<< "\t" << valid_route << "\t" << valid_mem << "\t" << valid_rec << endl;

 if(valid_route_rf || valid_route || valid_mem || valid_rec)
 {
   //This implies that II'==II and
   // not more operations are being unmapped than the prior attempt
   if(valid_route_rf == true)
   {
     debugfile << "Left node size: " << leftNodeMapped_route_rf.size() << "\t" << best_last_attempt_unmapped << "\tUtility: " << util_route_rf << "\t" << best_util << "\n";
     //If more operations can be mapped at same II, it is best!
     if(leftNodeMapped_route_rf.size() < best_last_attempt_unmapped)
     {
       best_util = util_route_rf;
       best_last_attempt_unmapped = leftNodeMapped_route_rf.size();
       best_option = 0;
     }
     else if(util_route_rf< best_util)	//Else, consider PE utilization
     {
       best_util = util_route_rf;
       best_last_attempt_unmapped = leftNodeMapped_route_rf.size();
       best_option = 0;
     }
     // Since mapping attempt is at a particular II, achieved II should be the same.
     // It will never be less. So, we can compare only on PE utilization or
     // for the more number of operations mapped.
   }

   if(valid_route == true)
   {
     if(leftNodeMapped_route.size() < best_last_attempt_unmapped)
     {
       best_util = util_route;
       best_last_attempt_unmapped = leftNodeMapped_route.size();
       best_option = 1;
     }
     else if((leftNodeMapped_route.size() == best_last_attempt_unmapped) && (util_route < best_util))
     {
       best_util = util_route;
       best_last_attempt_unmapped = leftNodeMapped_route.size();
       best_option = 1;
     }
   }

   if(valid_mem == true)
   {
     if(leftNodeMapped_mem.size() < best_last_attempt_unmapped)
     {
       best_util = util_mem;
       best_last_attempt_unmapped = leftNodeMapped_mem.size();
       best_option = 2;
     }
     else if((leftNodeMapped_mem.size() == best_last_attempt_unmapped) && (util_mem < best_util))
     {
       best_util = util_mem;
       best_last_attempt_unmapped = leftNodeMapped_mem.size();
       best_option = 2;
     }
   }

   if(valid_rec == true)
   {
     if(leftNodeMapped_rec.size() < best_last_attempt_unmapped)
     {
       best_util = util_rec;
       best_last_attempt_unmapped = leftNodeMapped_rec.size();
       best_option = 3;
     }
     else if((leftNodeMapped_rec.size() == best_last_attempt_unmapped) && (util_rec < best_util))
     {
       best_util = util_rec;
       best_last_attempt_unmapped = leftNodeMapped_rec.size();
       best_option = 3;
     }
   }
 }

 // Can Make an appropriate cgra copy function instead of returning a CGRA
 if(best_option != 2)
 copy_DFG_mem->Delete_Mem_Nodes(inserted_mem_nodes);

 debugfile << "Best Option is: " << best_option << "\n";

 switch(best_option)
 {
   case 0:
   {
     inDFG = copy_DFG_route_rf->Copy_With_Schedule_And_Mapping();
     best_leftmapped = leftNodeMapped_route_rf;
     new_node = new_node_route_rf;
     return CGRA_route_rf;
   }
   case 1:
   {
     inDFG = copy_DFG_route->Copy_With_Schedule_And_Mapping();
     best_leftmapped = leftNodeMapped_route;
     new_node = new_node_route;
     return CGRA_route;
   }
   case 2:
   {
     inDFG = copy_DFG_mem->Copy_With_Schedule_And_Mapping();
     best_leftmapped = leftNodeMapped_mem;
     new_node = new_node_mem;
     return CGRA_mem;
   }
   case 3:
   {
     inDFG = copy_DFG_rec->Copy_With_Schedule_And_Mapping();
     best_leftmapped = leftNodeMapped_rec;
     return CGRA_rec;
   }
   default: return CGRA_route;
 }
}


void RAMP::Route_Reschedule_Map(int left_id, int pred_id, int succ_id, int II, unsigned int last_attempt_unmapped, DFG* &dfg, int distance, vector<Node*> prev, vector<Node*> next, bool &valid_route, float &util_route, vector<Node*> &leftNodeMapped_route, Node* &new_node, CGRA &myCGRA)
{
 debugfile << "\nII: " << II << "\tIn Route Reschedule\n";
 valid_route = false;

 Node* leftnode = dfg->get_Node(left_id);
 Node* pred     = dfg->get_Node(pred_id);
 Node* succ     = dfg->get_Node(succ_id);
 vector<Node*> inserted_nodes;

 int current_number_resources = cgra_info.X_Dim*cgra_info.Y_Dim;
 // At max we can insert routing operations
 // equal to the difference in schedule time of operations (d).
 // Allowing 2*d attempts.
 for(int i=0; i< 2*distance; i++)
 {
   debugfile << "attempt i = " << i << " for node " << leftnode->get_ID() << endl;
   debugfile << "pred is: " <<pred->get_ID() << "\tsucc is: " << succ->get_ID() << endl;
   int inserted_node_id = -1;

   //reschedule the DFG
   int success = dfg->Route_ReSchedule(II, current_number_resources, leftnode, pred, succ, inserted_node_id);
   debugfile << "Routing success: " << success << endl;
   debugfile << inserted_node_id << endl;
   if(success == 0)	//Success shows that it is possible to reschedule the DFG at same II.
   return;
   new_node = dfg->get_Node(inserted_node_id);
   inserted_nodes.push_back(dfg->get_Node(inserted_node_id));

   myCGRA.SetII(II);	//set II
   myCGRA.ResetIteration();

   vector<Node*> leftnotmapped;

   if (myCGRA.MCS2(dfg, leftnotmapped))
   {
     valid_route = true;
     myCGRA.CalcPEUtil(util_route);
     leftNodeMapped_route.clear();
     return;
   }
   else
   {
     valid_route = true;

     //It is Possible That The Pair Pred->Succ is Now Compatible And Inserted Node And Pred,Succ Are Compatible
     if(myCGRA.IsCompatible(pred_id, succ_id))
     {
       debugfile << "Pred and Succ are compatible\n";
       if((myCGRA.IsCompatible(pred_id, inserted_node_id)) && (myCGRA.IsCompatible(inserted_node_id, succ_id)))
       {
         debugfile << "Inserted Node is compatible with both pred and succ nodes\n";
         valid_route = true;
         myCGRA.CalcPEUtil(util_route);
         for(std::vector<Node*>::iterator it = leftnotmapped.begin(); it != leftnotmapped.end(); ++it) {
           if((*it)->get_ID() == inserted_node_id)
           {
             valid_route = false;
             break;
           }
           else
           leftNodeMapped_route.push_back(*it);
         }
         if(valid_route == true)
         break;	//Update DDG For Remaining Nodes (Path Sharing)
       }
       else
       {
         valid_route = false;
         //continue;
       }
     }
     else
     {
       valid_route = false;
       return;
     }

     for(std::vector<Node*>::iterator it = leftnotmapped.begin(); it != leftnotmapped.end(); ++it) {
       if( ((*it)->get_ID() == left_id) || ((*it)->get_ID() == inserted_node_id) )
       {
         debugfile << "left node is: " << (*it)->get_ID() << endl;
         valid_route = false;
         pred = dfg->get_Node(inserted_node_id);
         leftnode = dfg->get_Node(inserted_node_id);
         break;
       }

     }

     if(valid_route == true)
     {
       if(leftnotmapped.size() < last_attempt_unmapped) {
         myCGRA.CalcPEUtil(util_route);
         for(std::vector<Node*>::iterator it = leftnotmapped.begin(); it != leftnotmapped.end(); ++it)
         leftNodeMapped_route.push_back(*it);
         return;
       }
       //Return due to unacceptable outcome if attempt of rescheduling successful, but at cost of increasing number of unmapped nodes
       else {
         valid_route = false;
         return;
       }
     }
     leftnode = dfg->get_Node(inserted_node_id); //Shail Added After Checking Inclusion For Inter-Iteration Dependency
     debugfile << "leftnode is " << leftnode->get_ID() << endl;
   }
 }

 //If reaching here after 2*d attempts, it is failure. Exit
 if(valid_route == false)
 {
   cerr << "Should Not Take So Many Attempts And Reach Here.\n";
   return;
 }

 //If the given pair pred->succ is compatible and set with routnig path, modify the DFG (remaining pairs) for path sharing.
 if(left_id == pred_id)
 {
   Node* predecessor_of_succ;
   vector<ARC*>::iterator iEdge;
   vector<ARC*> ARCSet = dfg->getSetOfArcs();
   ARC *arc;
   bool pred_found = false;
   for (iEdge = ARCSet.begin(); iEdge != ARCSet.end(); iEdge++)
   {   arc = *iEdge;
     if((arc->get_From_Node()->get_Instruction() == (Instruction_Operation) route_op) && (arc->get_To_Node()->get_ID() == succ->get_ID()))
     {
       //Make Sure Predecessor of Succ is Same Route Node As Inserted One.
       if(std::find(inserted_nodes.begin(), inserted_nodes.end(), arc->get_From_Node()) != inserted_nodes.end()) {
         predecessor_of_succ = arc->get_From_Node();
         pred_found = true;
         break;
       }
     }
   }

   if(pred_found == false)
   FATAL(true,"In Route_Reschedule_Map. Should Not Reach Here\n");


   //Iterate through all next nodes if a left node is a predecessor node for far scheduled node(s)
   for(unsigned int i=0; i < next.size(); i++)
   {
     Node* next_node = next[i];

     for (iEdge = ARCSet.begin(); iEdge != ARCSet.end(); iEdge++)
     {
       arc = *iEdge;
       //find the arc between leftnode -> next_node and delete it
       if((arc->get_From_Node()->get_ID() == left_id) && (arc->get_To_Node()->get_ID() == next_node->get_ID()))
       {
         dfg->Remove_Arc(arc->get_From_Node(),arc->get_To_Node());
         dfg->make_Arc(predecessor_of_succ, next_node, dfg->ARC_Max_Index+1, 0, TrueDep,arc->get_OperandOrder());
       }
     }
   }
   new_node = predecessor_of_succ;
 }
 else if(left_id == succ_id)
 {
   Node* successor_of_pred;
   vector<ARC*>::iterator iEdge;
   vector<ARC*> ARCSet = dfg->getSetOfArcs();
   ARC *arc;
   bool succ_found = false;
   for (iEdge = ARCSet.begin(); iEdge != ARCSet.end(); iEdge++)
   {   arc = *iEdge;
     if((arc->get_To_Node()->get_Instruction() == (Instruction_Operation) route_op) && (arc->get_From_Node()->get_ID() == pred->get_ID()))
     {
       //Make Sure Predecessor of Succ is Same Route Node As Inserted One.
       if(std::find(inserted_nodes.begin(), inserted_nodes.end(), arc->get_To_Node()) != inserted_nodes.end()) {
         successor_of_pred = arc->get_To_Node();
         succ_found = true;
         break;
       }
     }
   }

   if(succ_found == false)
   FATAL(true,"In Route_Reschedule_Map. Should Not Reach Here\n");


   //Iterate through all next nodes if a left node is a predecessor node for far scheduled node(s)
   for(unsigned int i=0; i < prev.size(); i++)
   {
     Node* prev_node = prev[i];

     for (iEdge = ARCSet.begin(); iEdge != ARCSet.end(); iEdge++)
     {
       arc = *iEdge;
       //find the arc between leftnode -> next_node and delete it
       if((arc->get_To_Node()->get_ID() == left_id) && (arc->get_From_Node()->get_ID() == prev_node->get_ID()))
       {
         dfg->Remove_Arc(arc->get_From_Node(),arc->get_To_Node());
         dfg->make_Arc(prev_node, successor_of_pred, dfg->ARC_Max_Index+1, 0, TrueDep,arc->get_OperandOrder());
       }
     }
   }
   new_node = successor_of_pred;
 }

 if(dfg->get_ResMII(current_number_resources) > II) {
   valid_route = false;
   return;
 }

 //Instead Make a mapping attempt (update compatibility + left nodes)
 leftNodeMapped_route.clear();
 myCGRA.SetII(II);
 myCGRA.ResetIteration();
 if (myCGRA.MCS2(dfg, leftNodeMapped_route))
 {
   myCGRA.CalcPEUtil(util_route);
 }
 else
 {
   myCGRA.CalcPEUtil(util_route); //No Need To Check Whether Left Nodes Are Increased Or Not.
 }
 debugfile << "Returning\n";
}

 // to do: Make sure to get pair of nodes as input and after mapping, check if both are mapped.
 //Moreover, first left node n1 should be given to reschedule until mapped.
 //Once, n1 and inserted nodes are mapped, n2 should be rescheduled, if unmapped

 /* to do: For memory reschedule and map, need to do two things -
 * 1. Reschedule function should ensure that corresponding loads are scheduled after a store.
 * 2. For correspondnig load/store nodes, Compatibility checking should ensure that whether this condition is met or not.
 *    If not, it should make the node incompatible.
 */

//to do: Whenever success is zero, delete corresponding inserted store and load nodes from all 3 map vectors
void RAMP::Mem_Reschedule_Map(int node_id, int pred_id, int succ_id, int II, unsigned int last_attempt_unmapped, DFG* &dfg, int distance, vector<Node*> prev, vector<Node*> next,
				   bool &valid_mem, float &util_mem, vector<Node*> &leftNodeMapped_mem, Node* &new_node, CGRA &myCGRA, vector<Node*> &inserted_mem_nodes)
{
	debugfile << "\nII: " << II << "\tIn Memory Reschedule\n";

	valid_mem = false;
	Node* leftnode = dfg->get_Node(node_id);
	Node* othernode;

	if(node_id == pred_id)
 	    othernode = dfg->get_Node(succ_id);
	else
 	    othernode = dfg->get_Node(pred_id);

	int current_number_resources = cgra_info.X_Dim*cgra_info.Y_Dim;

	vector<Node*> inserted_nodes;

	//Reschedule With Adding Store Nodes To To The Predecessor Node And Load To Successor Node
	/* to do: 3 Cases
    1. If distance = 0 or immediate, return invalid
    2. Else, if pred node is load data, just load
    3. else store and load
    ensure path sharing
    */

	int success = dfg->Mem_Reschedule(II, current_number_resources, leftnode, othernode, inserted_nodes);
	inserted_mem_nodes = inserted_nodes;
	debugfile << "Reschedule success is: " << success << "\n";
  //Success shows that it is possible to reschedule the DFG at same II.
	if(success == 0) {
		dfg->Delete_Mem_Nodes(inserted_nodes);
	  return;
	}

	myCGRA.SetII(II);	//set II
	myCGRA.ResetIteration();

	vector<Node*> leftnotmapped;
	if (myCGRA.MCS2(dfg, leftnotmapped))
	{
		valid_mem = true;
		leftNodeMapped_mem.clear();
		myCGRA.CalcPEUtil(util_mem);
		return;
	}
	else
	{
		//If Immediate Successors of Pred Is Not Mapped, Load Them/Route. If More Than One, Insert In List of Store/Load.
		vector<Node*> nextnodes = dfg->get_Node(pred_id)->Get_Next_Nodes();
		vector<Node*> next_immediates;
		Node* load_data = NULL;
    bool modified_immediate_nodes = false;
		vector<Node*> inserted_mem_nodes_immediates;

    bool changed;
    Node* nodeToBeMapped;
    vector<Node*> inserted_route_nodes;

    success = dfg->check_reschedule_of_mem_nodes(pred_id,leftnotmapped,current_number_resources,II,changed,nodeToBeMapped,inserted_route_nodes);
    if(changed)
    {
      if(success == 0)
        return;

      inserted_nodes = dfg->Union(inserted_nodes,inserted_route_nodes);

      //Try Mapping and check whether the store node(s) is mapped
      leftnotmapped.clear();
      if (myCGRA.MCS2(dfg, leftnotmapped))
    	{
    		valid_mem = true;
    		leftNodeMapped_mem.clear();
    		myCGRA.CalcPEUtil(util_mem);
    		return;
    	}

      vector<Node*>::iterator it;
      if(std::find(leftnotmapped.begin(), leftnotmapped.end(), nodeToBeMapped) != leftnotmapped.end())
      {
        dfg->Delete_Mem_Nodes(inserted_nodes);
        return;
      }
      else
      {
        //Store Node is scheduled
        valid_mem = true;
        leftNodeMapped_mem.clear();
    		leftNodeMapped_mem = leftnotmapped;
    		myCGRA.CalcPEUtil(util_mem);
        return;
      }
    }

    //Following code needs a pass

    // to do: If cannot map store nodes, try to map only store node and loading successors as the nodes
    //Can be clubbed with following routines

    //Check whether other immediate nodes are mapped or not
    //In case of not being mapped, load the data from the predecessor
    for(std::vector<Node*>::iterator it = nextnodes.begin(); it != nextnodes.end(); ++it) {
      if( ((*it)->get_Sched_Info()->get_Current() - dfg->get_Node(pred_id)->get_Sched_Info()->get_Current())
      <= (dfg->get_Node(pred_id)->get_Latency() + store_data_latency) )
      {
        //Instead check for compatibility
        if(std::find(leftnotmapped.begin(), leftnotmapped.end(), (*it)) != leftnotmapped.end())
        {
          if(!modified_immediate_nodes) //Insert Load Node For First Immediate Node
          {
            modified_immediate_nodes = true;
            dfg->insert_loads_mem(dfg->get_Node(pred_id),(*it),inserted_mem_nodes_immediates);
            inserted_mem_nodes.insert(inserted_mem_nodes.end(), inserted_mem_nodes_immediates.begin(), inserted_mem_nodes_immediates.end());
            load_data = inserted_mem_nodes_immediates[1];
          }
          else
          {	//Share load data node as predecessor to other input nodes (path sharing)
            Node* next_node = (*it);
            vector<ARC*> ARCSet = dfg->getSetOfArcs();
            ARC *arc;
            vector<ARC*>::iterator iEdge;
            for (iEdge = ARCSet.begin(); iEdge != ARCSet.end(); iEdge++)
            {
              arc = *iEdge;
              //find the arc between pred -> next_node and delete it
              if((arc->get_From_Node()->get_ID() == pred_id) && (arc->get_To_Node()->get_ID() == next_node->get_ID()))
              {
                dfg->Remove_Arc(arc->get_From_Node(),arc->get_To_Node());
                dfg->make_Arc(load_data, next_node, dfg->ARC_Max_Index+1, 0, TrueDep, arc->get_OperandOrder());
              }
            }
          }
        }
      }
    }

		//Check Res and MemII and exit if II' > II Or Cannot Reschedule
		if(modified_immediate_nodes)
		{
			int success = dfg->Reschedule(II, current_number_resources,1,3,{},{});
			if(success == 0) {
			    dfg->Delete_Mem_Nodes(inserted_nodes);
			    return;
			}
		}

		inserted_nodes = dfg->Union(inserted_nodes, inserted_mem_nodes_immediates);

		for(std::vector<Node*>::iterator it = leftnotmapped.begin(); it != leftnotmapped.end(); ++it) {
			debugfile << "left node is: " << (*it)->get_ID() << endl;
			if((*it)->get_ID() == succ_id) {
			  vector<Node*> to_change_set;
			  to_change_set.push_back(dfg->get_Node(succ_id));
  			  int success = dfg->Modulo_ReSchedule_Smart_2(0, current_number_resources, II, to_change_set);
	 		    if(success == 0) {
 	      		  dfg->Delete_Mem_Nodes(inserted_nodes);
			        return;
			    }
			}
		}

		myCGRA.MCS2(dfg, leftnotmapped);
    //Iterate Through Inserted Nodes And Check Whether They Need To Be Rescheduled/Routed
    for(std::vector<Node*>::iterator it = leftnotmapped.begin(); it != leftnotmapped.end(); ++it) {
      std::vector<Node*>::iterator it2;
      for(it2 = inserted_nodes.begin(); it2 != inserted_nodes.end(); it2++) {
        if((*it2)->get_ID() == (*it)->get_ID()) {
          vector<Node*> to_change_set;
          to_change_set.push_back(dfg->get_Node((*it2)->get_ID()));
          int success = dfg->Modulo_ReSchedule_Smart_2(0, current_number_resources, II, to_change_set);
          if(success == 0) {
            dfg->Delete_Mem_Nodes(inserted_nodes);
            return;
          }
        }
      }
    }

		inserted_nodes.push_back(leftnode);
		inserted_nodes.push_back(othernode);
		debugfile << "Going for new mapping attempt\n";
	  leftnotmapped.clear();

		myCGRA.SetII(II);	//set II
		myCGRA.ResetIteration();
		if (myCGRA.MCS2(dfg, leftnotmapped))
		{
		  valid_mem = true;
		  leftNodeMapped_mem.clear();
		  myCGRA.CalcPEUtil(util_mem);
		  return;
		}

    //Check If All Inserted Nodes And Given Pair Are Compatible/Mapped. If So, Update Other Pairs for Path Sharing.
    for(std::vector<Node*>::iterator it = inserted_nodes.begin(); it != inserted_nodes.end(); ++it) {
      if(std::find(leftnotmapped.begin(), leftnotmapped.end(), (*it)) != leftnotmapped.end())
      {
        dfg->Delete_Mem_Nodes(inserted_nodes);
        return;
      }
    }

    //If Compatible Then See For Mapped and Left Nodes.
    //Update Other Pairs. Give Inserted Node As Output.
    //If the given pair pred->succ is compatible and set with routnig path, modify the DFG (remaining pairs) for path sharing.
    if(node_id == pred_id)
    {
      Node* predecessor_of_succ = load_data;
      vector<ARC*>::iterator iEdge;
      vector<ARC*> ARCSet = dfg->getSetOfArcs();
      ARC *arc;

      //Iterate through all next nodes if a left node is a predecessor node for far scheduled node(s)
      for(unsigned int i=0; i < next.size(); i++)
      {
        Node* next_node = next[i];
        for (iEdge = ARCSet.begin(); iEdge != ARCSet.end(); iEdge++)
        {
          arc = *iEdge;
          //find the arc between leftnode -> next_node and delete it
          if((arc->get_From_Node()->get_ID() == node_id) && (arc->get_To_Node()->get_ID() == next_node->get_ID()))
          {
            dfg->Remove_Arc(arc->get_From_Node(),arc->get_To_Node());
            dfg->make_Arc(predecessor_of_succ, next_node, dfg->ARC_Max_Index+1, 0, TrueDep,arc->get_OperandOrder());
          }
        }
      }
      new_node = predecessor_of_succ;
    }

    //Check II and exit if needed.
    if((dfg->get_ResMII(current_number_resources) > II) ||(dfg->get_MemMII() > II)) {
      dfg->Delete_Mem_Nodes(inserted_nodes);
      return;
    }

		//Map and return (like in routing)
		leftNodeMapped_mem.clear();
		myCGRA.SetII(II);	//set II
		myCGRA.ResetIteration();
		myCGRA.MCS2(dfg, leftNodeMapped_mem);
		myCGRA.CalcPEUtil(util_mem);
	}
}


void RAMP::Route_RF_ReSchedule_Map(int pred_id, int succ_id, int II, unsigned int last_attempt_unmapped, DFG* &dfg, int distance, vector<Node*> prev, vector<Node*> next, bool &valid_route_rf, float &util_route_rf, vector<Node*> &leftNodeMapped_route_rf, Node* &new_node, CGRA &myCGRA)
{
  debugfile << "\nII: " << II << "\tIn Route Through RF Reschedule\n";
  valid_route_rf = false;
  if(cgra_info.R_Size == 0) return;

  Node* leftnode = dfg->get_Node(pred_id);
  Node* pred     = dfg->get_Node(pred_id);
  Node* succ     = dfg->get_Node(succ_id);
  vector<Node*> inserted_nodes;
  vector<Node*>leftnotmapped;

  int Rmin = 0;
  int Rmax = 0;
  int current_number_resources = cgra_info.X_Dim*cgra_info.Y_Dim;

  myCGRA.SetII(II);
  myCGRA.ResetIteration();
  myCGRA.MCS2(dfg, leftnotmapped);

  int success = 0, found = 0;
  int temp_inserted_node = 0, new_left_node_id = 0, sched_time = 0;
  DFG* temp_DFG = NULL;

  route_rf_nodeid_scheduled_time.clear();

  Node_Dummy* routeReadNode;
  Node_Dummy* routeWriteNode;

  int rec_dist = dfg->get_Arc(dfg->get_Node(leftnode->get_ID()),dfg->get_Node(succ->get_ID()))->get_Distance();

  //For Self Loop.
  if(pred_id == succ_id) {
    rec_dist = pred->get_self_loop()->get_Distance();
    pred->delete_self_loop();
    succ->delete_self_loop();
  }

  for(int i=0; i < distance; i++)
  {
    //Get Distance Between pred and succ
    debugfile << "left node is: " << leftnode->get_ID() << "\tsucc: " << succ->get_ID() << endl;
    int dist = 0;

    if(rec_dist == 0)
    dist = dfg->get_Node(leftnode->get_ID())->get_dist(dfg->get_Node(succ->get_ID()),II);
    leftnotmapped.clear();

    if((dist <= 1) && (rec_dist == 0))
    FATAL(true,"You should Not Reach Here. Debug!");

    //For dist > 1
    myCGRA.makeRFTable();
    myCGRA.getMinMaxReg(Rmin,Rmax);

    pred = dfg->get_Node(leftnode->get_ID());

    int pred_time = pred->get_Sched_Info()->get_Current();
    int succ_time = succ->get_Sched_Info()->get_Current();
    if(rec_dist > 0)
      succ_time = succ->get_Sched_Info()->get_Current() + rec_dist*II;

    int start_time = pred_time + Rmax*II - 1;
    if(start_time > (succ_time - 1))
      start_time = succ_time - 1;

    int end_time  = pred->get_Sched_Info()->get_Current() + 1;

    if((Rmin == 0) || (II < 2)) {
      //to do: Verify For R=0 At Some Later Stage.
      start_time =  pred->get_Sched_Info()->get_Current() + 1;
      end_time   =  pred->get_Sched_Info()->get_Current() + 1;
    }

    debugfile << "Rmin = " << Rmin << "\tRmax = " << Rmax << endl;
    debugfile << "start_time: " << start_time << "\tend_time: " << end_time << endl;

    debugfile << "Printing ARCs\n";
    vector<ARC*> ARCSet = dfg->getSetOfArcs();
    vector<ARC*>::iterator iEdge;

    for(sched_time = start_time; sched_time >= end_time; sched_time--)
    {
      temp_DFG = dfg->Copy_With_Schedule_And_Mapping();
      found = 0;
      success = 0;

      //Add A Node To Read From RF
      routeReadNode = new Node_Dummy(1, temp_DFG->Get_Unique_Index(), leftnode);
      if ((int) routeReadNode->get_Number_of_Pred()>0)
      temp_DFG->Add_Arc(routeReadNode->get_Pred_Arc(0));

      temp_DFG->insert_Node(routeReadNode);
      temp_DFG->insert_Node_in_between_output(temp_DFG->get_Node(leftnode->get_ID()), temp_DFG->get_Node(succ->get_ID()), routeReadNode);
      temp_inserted_node = routeReadNode->get_ID();

      debugfile << "At time: " << sched_time << "  Inserted Route Read Node " << temp_inserted_node << " between nodes " << leftnode->get_ID() << " and " << succ->get_ID() << "\n";

      std::map<int,int>::iterator it = route_rf_nodeid_scheduled_time.begin();
      route_rf_nodeid_scheduled_time.insert (it, std::pair<int,int>(temp_inserted_node,sched_time));

      //Check Can We Schedule
      success = temp_DFG->Reschedule(II, current_number_resources,1,1,{},{});

      //Delete entry temporarily from map of node and their timing
      route_rf_nodeid_scheduled_time.erase (temp_inserted_node);

      if(success == 0)
      {
        debugfile << "Did not reschedule the node successfully\n";
        continue;
      }

      //If Inter-Iteration Dependency, Update The Distance.
      node_new_inter_it_dist.clear();
      if(rec_dist > 0) {
        int new_inter_iteration_dist = (succ_time - sched_time)/II;
        new_inter_iteration_dist++;
        node_new_inter_it_dist.insert(std::pair<int,int>(temp_inserted_node,new_inter_iteration_dist));
      }

      //Check Can We Map
      myCGRA.ResetIteration();
      myCGRA.SetII(II);
      myCGRA.GenerateCompGraph(temp_DFG);

      if((myCGRA.IsCompatible(leftnode->get_ID(),temp_inserted_node) == false) && (myCGRA.IsCompatible(temp_inserted_node,succ->get_ID()) == false)) {
        debugfile << "1. mapping success = 0.\t" << temp_inserted_node << " is not compatible with " << succ->get_ID() << "\n";
        success = 0;
        continue;
      }

      if(myCGRA.IsCompatible(leftnode->get_ID(),temp_inserted_node) == false) {
        debugfile << "2. mapping success = 0" << leftnode->get_ID() << " is not compatible with " << temp_inserted_node << "\n";
        success = 0;
        continue;
      }

      new_left_node_id = temp_inserted_node;
      int dist_new = temp_DFG->get_Node(temp_inserted_node)->get_dist(temp_DFG->get_Node(succ->get_ID()),II);
      debugfile << dist_new << endl;
      int dist_next = 0;
      int routeReadTo = succ_id;

      if((myCGRA.IsCompatible(temp_inserted_node,succ->get_ID()) == true) && (dist_new <= II)) // && (dist_new > 1))	//If within II see whether we can map as it is
      {
        if(next.size() > 0)
        {
          dist_next = next[0]->get_Sched_Info()->get_Current() - temp_DFG->get_Node(temp_inserted_node)->get_Sched_Info()->get_Current();
          routeReadTo = next[0]->get_ID();
          debugfile << "routeReadTo = " << routeReadTo << endl;
        }
      }

      if((dist_new >= II) || ((dist_next > 1) && (dist_next <= II)) || (myCGRA.IsCompatible(temp_inserted_node,succ->get_ID()) == false))
      {
        //Insert A Routing Node Which Will Carry Forward The Value Read by tempDummyNode
        routeWriteNode = new Node_Dummy(1, temp_DFG->Get_Unique_Index(), routeReadNode);
        if ((int) routeWriteNode->get_Number_of_Pred()>0)
        temp_DFG->Add_Arc(routeWriteNode->get_Pred_Arc(0));

        temp_DFG->insert_Node(routeWriteNode);

        if((dist_next > 1) && (dist_next <= II)) {
          ARC* temp_edge = temp_DFG->get_Arc(temp_DFG->get_Node(leftnode->get_ID()),temp_DFG->get_Node(routeReadTo));
          temp_DFG->Remove_Arc(temp_DFG->get_Node(leftnode->get_ID()),temp_DFG->get_Node(routeReadTo));
          temp_DFG->make_Arc(routeWriteNode, temp_DFG->get_Node(routeReadTo), temp_DFG->ARC_Max_Index+1, temp_edge->get_Distance(), temp_edge->get_Dependency(), temp_edge->get_OperandOrder());
          debugfile << "At time: " << sched_time+1 << "  Inserted Route Write Node " << routeWriteNode->get_ID() << " between nodes " << routeReadNode->get_ID() << " and " << routeReadTo << "\n";
        }
        else {
          temp_DFG->insert_Node_in_between_output(temp_DFG->get_Node(routeReadNode->get_ID()), temp_DFG->get_Node(routeReadTo), routeWriteNode);
          debugfile << "At time: " << sched_time+1 << "  Inserted Route Write Node " << routeWriteNode->get_ID() << " between nodes " << routeReadNode->get_ID() << " and " << routeReadTo << "\n";
        }

        std::map<int,int>::iterator it = route_rf_nodeid_scheduled_time.begin();
        route_rf_nodeid_scheduled_time.insert (it, std::pair<int,int>(temp_inserted_node,sched_time));
        route_rf_nodeid_scheduled_time.insert (it, std::pair<int,int>(routeWriteNode->get_ID(),sched_time+1));

        //Check Reschedule And Compatibility
        //Check Can We Schedule
        success = temp_DFG->Reschedule(II, current_number_resources,1,1,{},{});

        //Delete entry temporarily from map of node and their timing
        route_rf_nodeid_scheduled_time.erase (temp_inserted_node);
        route_rf_nodeid_scheduled_time.erase (routeWriteNode->get_ID());

        debugfile << "Now, rescheduling success = " << success << "\n";
        if(success == 0)
        continue;

        //Check, if predecessor's and successor's timing is changed => we need to change time of newly inserted node by skipping current iteration of schedule time
        if((pred->get_Sched_Info()->get_Current() != temp_DFG->get_Node(pred->get_ID())->get_Sched_Info()->get_Current()) ||
        (succ->get_Sched_Info()->get_Current() != temp_DFG->get_Node(succ->get_ID())->get_Sched_Info()->get_Current())) {
          debugfile << "3. mapping success = 0\n";
          success = 0;
          continue;
        }

        //Check Can We Map
        myCGRA.ResetIteration();
        myCGRA.SetII(II);
        myCGRA.GenerateCompGraph(temp_DFG);

        if((myCGRA.IsCompatible(routeReadNode->get_ID(),routeWriteNode->get_ID()) == false) && (myCGRA.IsCompatible(routeWriteNode->get_ID(),routeReadTo) == false)) {
          debugfile << "4. mapping success = 0\n";
          success = 0;
          continue;
        }

        new_left_node_id = routeWriteNode->get_ID();
      }
      else
      {
        // If route read node not added or erased perviously, just add it back!
        if(route_rf_nodeid_scheduled_time.find(temp_inserted_node) == route_rf_nodeid_scheduled_time.end())
        {
          std::map<int,int>::iterator itt = route_rf_nodeid_scheduled_time.begin();
          route_rf_nodeid_scheduled_time.insert (itt, std::pair<int,int>(temp_inserted_node,sched_time));
        }
      }

      if(success)
      {
        myCGRA.ResetIteration();
        myCGRA.SetII(II);
        if (myCGRA.MCS2(temp_DFG, leftnotmapped))
        {
          debugfile << "Done  successfully with spill through RF\n";
          new_node = temp_DFG->get_Node(new_left_node_id);
          debugfile << "New node is: " << new_node->get_ID() << "\n";
          valid_route_rf = true;
          myCGRA.CalcPEUtil(util_route_rf);
          leftNodeMapped_route_rf.clear();
          dfg = temp_DFG->Copy_With_Schedule_And_Mapping();
          return;
        }
        else
        {
          for(std::vector<Node*>::iterator it = leftnotmapped.begin(); it != leftnotmapped.end(); ++it) {
            if((*it)->get_ID() == temp_inserted_node)
            {
              debugfile << "left node is: " << (*it)->get_ID() << endl;
              success = 0;
            }
          }

          //If Compatible For new_left_node_id, exit.
          if((myCGRA.IsCompatible(new_left_node_id, succ->get_ID()) == true) && (success == 1)) {
            debugfile << "Mapped node successfully\n";
            found = 1;
            break;
          }

          debugfile << "5. mapping success = " << success << "\n";
          if(success == 0)
            continue;
          else
          {
            leftnode = temp_DFG->get_Node(new_left_node_id);
            dfg = temp_DFG->Copy_With_Schedule_And_Mapping();
            std::map<int,int>::iterator it = route_rf_nodeid_scheduled_time.begin();
            route_rf_nodeid_scheduled_time.insert (it, std::pair<int,int>(temp_inserted_node,sched_time));
            if(new_left_node_id != temp_inserted_node) route_rf_nodeid_scheduled_time.insert (it, std::pair<int,int>(new_left_node_id,sched_time+1));
            break;
          }
        }
      }
    }

    if(success == 0) {
      valid_route_rf = false;
      return;
    }

    if(found) {
      debugfile << "found! mapped node successfully\n";
      if(leftnotmapped.size() <= last_attempt_unmapped) {
        for(std::vector<Node*>::iterator it = leftnotmapped.begin(); it != leftnotmapped.end(); ++it)
        {
          leftNodeMapped_route_rf.push_back(*it);
          debugfile << "Left node is: " << (*it)->get_ID() << "\n";
        }
        myCGRA.CalcPEUtil(util_route_rf);
        dfg = temp_DFG->Copy_With_Schedule_And_Mapping();
        valid_route_rf = true;
        break;
      }
      else
      {
        valid_route_rf = false;
        return;
      }
    }
  }

  new_node = dfg->get_Node(new_left_node_id);
  debugfile << "new_node is: " << new_node->get_ID() << "\n";

  //Once Done With The Given Pair, Connect Last Inserted Node To Other Successors  (Path Sharing)
  if(next.size() > 0)
  {
    Node* predecessor_of_succ;
    vector<ARC*>::iterator iEdge;
    vector<ARC*> ARCSet = temp_DFG->getSetOfArcs();
    ARC *arc;
    bool pred_found = false;

    for (iEdge = ARCSet.begin(); iEdge != ARCSet.end(); iEdge++)
    {
      arc = *iEdge;

      if((arc->get_From_Node()->get_Instruction() == (Instruction_Operation) route_op) && (arc->get_To_Node()->get_ID() == succ->get_ID()))
      {
        //Make Sure Predecessor of Succ is Same Route Node As Inserted One.
        std::map<int,int>::iterator ittt = route_rf_nodeid_scheduled_time.begin();
        while(ittt != route_rf_nodeid_scheduled_time.end())
        {
          debugfile << ittt->first << "\t" << ittt->second << "\n";
          ittt++;
        }
        if(route_rf_nodeid_scheduled_time.find(arc->get_From_Node()->get_ID()) != route_rf_nodeid_scheduled_time.end()) {
          predecessor_of_succ = arc->get_From_Node();
          pred_found = true;
          break;
        }
      }
    }

    if(pred_found == false)
    FATAL(true,"In Route_RF_ReSchedule_Map. Should Not Reach Here\n");

    //Iterate through all next nodes if a left node is a predecessor node for far scheduled node(s)
    for(unsigned int i=0; i < next.size(); i++)
    {
      Node* next_node = next[i];
      for (iEdge = ARCSet.begin(); iEdge != ARCSet.end(); iEdge++)
      {
        arc = *iEdge;
        //find the arc between leftnode -> next_node and delete it
        if((arc->get_From_Node()->get_ID() == leftnode->get_ID()) && (arc->get_To_Node()->get_ID() == next_node->get_ID()))
        {
          temp_DFG->Remove_Arc(arc->get_From_Node(),arc->get_To_Node());
          temp_DFG->make_Arc(predecessor_of_succ, next_node, temp_DFG->ARC_Max_Index+1, 0, TrueDep,arc->get_OperandOrder());
        }
      }
    }
    new_node = dfg->get_Node(predecessor_of_succ->get_ID());

    if(temp_DFG->get_ResMII(current_number_resources) > II) {
      valid_route_rf = false;
      return;
    }

    //Make a mapping attempt (update compatibility + left nodes)
    leftNodeMapped_route_rf.clear();
    myCGRA.ResetIteration();
    myCGRA.SetII(II);
    myCGRA.MCS2(temp_DFG, leftNodeMapped_route_rf);
    myCGRA.CalcPEUtil(util_route_rf);
    dfg = temp_DFG->Copy_With_Schedule_And_Mapping();
    debugfile << "Returning\n";
  }
}

//iterate through left nodes and see if node is present or not
bool RAMP::is_node_left(vector<Node*> leftnodes, Node* node)
{
  for(std::vector<Node*>::iterator it = leftnodes.begin(); it != leftnodes.end(); ++it)
  {
    if((*it)->get_ID() == node->get_ID())
    return true;
  }
  return false;
}
