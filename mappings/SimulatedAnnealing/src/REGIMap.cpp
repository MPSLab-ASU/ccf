/*
 * REGIMap Wrapper
 *
 *  Created on: Jul 6, 2013
 *  Author: Mahdi Hamzeh
 *  Arizona State University
 */
#include "REGIMap.h"


/*
 * the main body of regimap algorithm
 * parse input files
 * preprocess DFG
 * schedule DFG
 * place and register allocated
 *  if succeed return
 *  else reschedule
 */
REGIMap::REGIMap(Parser myParser)
{
    REGI_DFG* original_DFG= new REGI_DFG(cgra_info.X_Dim,cgra_info.Y_Dim);
    /*
     * read input files and construct DFG
     */
    bool status = myParser.ParseDFG(original_DFG);

    //if there is an error in input files, exit
    if (!status)
    {
        return ;
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

    /*
     * Make a copy of the original DFG
     */
    REGI_DFG* copy_DFG_1;
    copy_DFG_1 = original_DFG->Copy();


    int CGRASize = cgra_info.X_Dim*cgra_info.Y_Dim;

    /* 
     * find lowest II for the original DFG
     */
    int low_II = copy_DFG_1->Init_Schedule(0, CGRASize);


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
    while (true)
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
        current_II_1 = copy_DFG_1->Schedule(id++, current_number_resources, current_II);

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

        DEBUG_MAPPING("Start Placement for II = %d" , current_II);

        for (int mappingAttemptCounter = 0; mappingAttemptCounter < MAPPING_POLICY.REGI_MAPPING_ATTEMPTS; mappingAttemptCounter++)
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

            vector<REGI_Node*> leftNodeMapped;
            /*
             * place and register allocated for DFG
             */
            copy_DFG_1->Dot_Print_DFG_With_Schedule("current",id);
            if (myCGRA.MCS2(copy_DFG_1, leftNodeMapped))
             {
                /*
                 * all nodes are mapped, show the mapping
                 */
                myCGRA.ShowMapping();
                myCGRA.EPIlog(copy_DFG_1);
                cout << "MII = " << low_II <<"\t Current II=" << current_II << endl; 
                copy_DFG_1->Dump_Edges("REGIDUMP");
				copy_DFG_1->Dump_Nodes("REGIDUMP");
                return;
            }

            else
             {
                //showSet(leftNodeMapped);
                //myCGRA.ShowMapping();
                //exit(1);
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

                    /* 
                     * reschedule the DFG
                     */
                    int success = copy_DFG_1->Modulo_ReSchedule_Smart_2(id++, current_number_resources, current_II, leftNodeMapped);
                    /*
                     * failed to reschedule, break
                     */
                    if (success == 0)
                    {
                        break;
                    }
                }
            }
        } 
        /* 
         * could not find mapping, decrease the number of available resources
         */
        current_number_resources--;

    }

}
