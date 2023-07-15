current_II_2: 2	max_mapping_attempts: 100
iteration: 0
after:
0
0
end:
10
6
Feasible II is 2 at 0
Node: 0	sched @ 0
Node: 1	sched @ 2
Node: 2	sched @ 3
Node: 3	sched @ 4
Node: 4	sched @ 2
Node: 5	sched @ 3
Node: 6	sched @ 4
Node: 7	sched @ 5
Node: 8	sched @ 2
Node: 9	sched @ 6
Node: 10	sched @ 6
Node: 11	sched @ 1
Node: 12	sched @ 2
Node: 17	sched @ 1
Node: 27	sched @ 0
Node: 28	sched @ 1
total nodes before routing: 16
total nodes before scheduling: 19
is_modified route and reschedule: 1
Routing Node: 29	ASAP: 3	ALAP: 3
Routing Node: 30	ASAP: 4	ALAP: 4
Routing Node: 31	ASAP: 5	ALAP: 5
Mod Sched time: 1
II : 2
Node 29 is to be scheduled at time 3, modulo time 1
Mod Sched time: 0
II : 2
Node 30 is to be scheduled at time 4, modulo time 0
Mod Sched time: 1
II : 2
Node 31 is to be scheduled at time 5, modulo time 1
modulo_schedule: 1	redo: 0
failure reason: 0
total nodes after scheduling: 19
j: 0	 -> 0 11 13 
j: 1	 -> 1 2 13 
j: 2	 -> 1 2 3 
j: 3	 -> 2 3 7 
j: 4	 -> 4 5 13 
j: 5	 -> 4 5 6 
j: 6	 -> 5 6 7 
j: 7	 -> 3 6 7 10 
j: 8	 -> 8 16 13 
j: 9	 -> 9 10 18 
j: 10	 -> 7 9 10 
j: 11	 -> 0 11 12 
j: 12	 -> 11 12 15 
j: 13	 -> 0 1 4 8 13 
j: 14	 -> 14 15 
j: 15	 -> 14 12 15 
j: 16	 -> 8 16 17 
j: 17	 -> 16 17 18 
j: 18	 -> 17 9 18 
left node size == 0
1  	4  	6  	3  	
0  	-100  	9  	10  	
-100  	12  	-100  	-100  	
8  	27  	30  	-100  	
17  	2  	5  	7  	
-100  	11  	-100  	-100  	
-100  	-100  	31  	-100  	
-100  	28  	-100  	29  	
Total mapping steps: 2
mo1 Node: 0 PE:<1 ,0, 0>
mo1 Node: 1 PE:<0 ,0, 0>
mo1 Node: 2 PE:<0 ,1, 1>
mo1 Node: 3 PE:<0 ,3, 0>
mo1 Node: 4 PE:<0 ,1, 0>
mo1 Node: 5 PE:<0 ,2, 1>
mo1 Node: 6 PE:<0 ,2, 0>
mo1 Node: 7 PE:<0 ,3, 1>
mo1 Node: 8 PE:<3 ,0, 0>
mo1 Node: 9 PE:<1 ,2, 0>
mo1 Node: 10 PE:<1 ,3, 0>
mo1 Node: 11 PE:<1 ,1, 1>
mo1 Node: 12 PE:<2 ,1, 0>
mo1 Node: 17 PE:<0 ,0, 1>
mo1 Node: 27 PE:<3 ,1, 0>
mo1 Node: 28 PE:<3 ,1, 1>
mo1 Node: 29 PE:<3 ,3, 1>
mo1 Node: 30 PE:<3 ,2, 0>
mo1 Node: 31 PE:<2 ,2, 1>

*********************************Prolog Start*********************************
Time:0
         F	         F	         F	         F	
         0	         F	         F	         F	
         F	         F	         F	         F	
         F	        27	         F	         F	
Time:1
        17	         F	         F	         F	
         F	        11	         F	         F	
         F	         F	         F	         F	
         F	        28	         F	         F	
Time:2
         1	         4	         F	         F	
         0	         F	         F	         F	
         F	        12	         F	         F	
         8	        27	         F	         F	
Time:3
        17	         2	         5	         F	
         F	        11	         F	         F	
         F	         F	         F	         F	
         F	        28	         F	        29	
Time:4
         1	         4	         6	         3	
         0	         F	         F	         F	
         F	        12	         F	         F	
         8	        27	        30	         F	
Time:5
        17	         2	         5	         7	
         F	        11	         F	         F	
         F	         F	        31	         F	
         F	        28	         F	        29	
Time:6
         1	         4	         6	         3	
         0	         F	         9	        10	
         F	        12	         F	         F	
         8	        27	        30	         F	
Time:7
        17	         2	         5	         7	
         F	        11	         F	         F	
         F	         F	        31	         F	
         F	        28	         F	        29	

*********************************Prolog End*********************************
*********************************Kernel Start*********************************
Time:0
         1(0)	         4(0)	         6(0)	         3(0)	
         0(0)	         F	         9(0)	        10(0)	
         F	        12(0)	         F	         F	
         8(0)	        27(0)	        30(0)	         F	
Time:1
        17(0)	         2(0)	         5(0)	         7(0)	
         F	        11(0)	         F	         F	
         F	         F	        31(0)	         F	
         F	        28(0)	         F	        29(0)	
*********************************Kernel End*********************************

*********************************Epilog Start*********************************
Time:0
         1	         4	         6	         3	
         F	         F	         9	        10	
         F	        12	         F	         F	
         8	         F	        30	         F	
Time:1
         F	         2	         5	         7	
         F	         F	         F	         F	
         F	         F	        31	         F	
         F	         F	         F	        29	
Time:2
         F	         F	         6	         3	
         F	         F	         9	        10	
         F	         F	         F	         F	
         F	         F	        30	         F	
Time:3
         F	         F	         F	         7	
         F	         F	         F	         F	
         F	         F	        31	         F	
         F	         F	         F	         F	
Time:4
         F	         F	         F	         F	
         F	         F	         9	        10	
         F	         F	         F	         F	
         F	         F	         F	         F	
Time:5

*********************************Epilog End*********************************
