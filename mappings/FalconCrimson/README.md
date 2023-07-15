# Fast sub-graph isomorphism for CGRA mapping.  



##  Papers:
1. Implementation using modified Ullmann's alogorithm. 
2. Ullmann, Julian R. "An algorithm for subgraph isomorphism." Journal of the ACM (JACM) 23.1 (1976): 31-42.'


## Author
Mahesh Balasubramanian

## Run line for new implementation

~/git_repos/ullmann/Debug/SSSRMap -EDGE loop22_edge.txt -NODE loop22_node.txt -X 4 -Y 4 -R 0 -MSA 10 -MAPII 10 -MAX_MAP 100 -MAX_II 50 -MAP_MODE 0 > debug


## Implementation details


Rough Steps: 
1. get the dfg.
2. Modulo schedule (create schedule)
3. Create architecture feasible schedule. 
-------------------------------

4. Ullmann starts
a. get adjacency of H (query) A_H
b. get adjacency of G (product matrix) A_G
c. get M' (from H and G)
  c1. If the node in H has a matching node_id in G and outgoing arcs in H < or = the node in G then M is 1 otherwise 0.

-----------------------------------

5. Refine or Pruning function for M
a. check for incoming arcs in the same cycle
b. check for outgoing arcs in the same cycle
c. place operations
d. iscompatible with prev nodes and the next node.
e. If more than one positions are feasible, 
  1. i.e., more than 1 1's in M' such that the node id and time are same but different PEs.
  2. & if based on compatibility more than one position is feasible
  3. then collect all the indices and chose a random one.
f. Check the resultant M' matrix to see if there is max of 1 1's per row and atmost 1 1's in the column.
  1. If yes go to next step.
  2. If no then collect all the node_ids of 1's and see which node was not mapped. 
  3. Check the connectivity of the unmapped node and create routing nodes for the incoming edges
  4. check if it less than number of tries.
  5. if yes Reschedule and start step 5 again (because now the DFG is changed).
  6. if no. Increase the II and start from step 2.

-------------------------------------
6. Mapping check
a. See if A_H = M*A_G*M' 
b. if yes then M is the mapping you are done. Print prolog and exit
-----------------------------------------------------------------

## Refine algorithm 

1.def refine():
  - for i in m.rows()
    - collect all the index of 1's;
      - if there are no incoming arcs for the node:
        - pick a rand index and assign 1; assign all others to 0 
        - continue;
      - else
        - collect all the incoming arc;
        - collect the PEs and times;
        - for j in collected 1's
          - if the PE is already assigned:
            - continue
          - else
            - collect all the one with no conflict with predecessors
        - pick a random 1 from no_conflict and assign 1; assign all others to 0;
  - if A_h == M0 * A_g * M0'
    - the it is isomorphic and M0 is the mapping. 
    - Return the corresponding pairs and exit
  - else
    - if all the arcs are sched with 1 cycle apart. 
      - start for loop again, we have to find another M that satisfies this condition.
    - else
      - exit with arc information. (vector of arcs information. we can collect the from, to and dep type.)

- if(refine())  
  - then mapping complete. get the pairs and output prolog
- else
  - get the arc, from and to nodes and dep type
  - add a routing node.
  - modify dfg and start refine again. 
    
  
  
2.  check if all the rows should have 1 1's and all the columns shoud have 1 1's or no 1's. Sanity check for m. 
3. refine is done, the resultant matrix in M0



## TODO:
1. Add nonrecurring registers. 
2. After working nonrecurring regs add recurring regs to the nonrecurr. 
- right now the program works fine without reg files.
