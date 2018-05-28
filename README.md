# CCF

CCF (CGRA Compilation Framework) is an end-to-end prototype demonstrating the code generation and simulation process for CGRA (or Coarse-Grained Reconfigurable Array) accelerators. 
Through CCF infrastructure, the users can simulate acceleration of loops of general-purpose applications on a heterogeneous processor core+CGRAs architecture. 

With LLVM 5.0 as a foundation, the implementation of CCF-compiler includes numerous compiler analysis and transformation passes, along with a customized code generation CGRA back-end. 
The user only needs to mark the performance-critical loops that they want to execute on CGRA, by using the annotation: #pragma CGRA, 
and the CCF-compiler automatically extracts the marked loops and maps them to the CGRA, 
generates code to communicate live data between the processor core and CGRA, pre-load the live values into CGRA registers, 
and generates the machine instructions to configure the PEs to execute the loop, 
and finally generates a binary that will execute on the CCF-simulator.
The CCF-simulator is built by modifying cycle-accurate processor simulator Gem5, 
and it models CGRA as a separate core coupled to ARM Cortex-like processor core with ARMv7a profile.

This open-source platform has been developed at Arizona State University and through CCF, we target accelerating the CGRA research by developing and making accessible a community-wide CGRA compilation infrastructure.
While current release of CCF supports code generation for several performance-critical loops of embedded MiBench benchmark suite, 
we hope to integrate the enhanced functionality in the future releases.


### How does it Work?

The steps for executing a C/C++ application with CCF are as follows:

1.	**Loop Annotation**: Once the programmer has profiled the compute-intensive application, 
and have identified a performance-critical loop, (s)he can annotate it with `#pragma CGRA`. 
Then, the CCF compiler can generate the code for the application's execution on the heterogeneous platform.

2. **Make**: The user has to just to replace gcc with cgracc (CCF's CGRA Compiler Collection). 
(Often, it decently supports complex makefiles.) Then, typing 'make' will generate the required executable.
The CCF compiler will inform that whether it would be executing the loop on the CGRA or not. 
For example, for the current release of the compilation infrastructure, if the annotated loop contains the system calls, it is not executed on CGRA. 
Or, if the compiler was able to vectorize the code 
(which may imply that the loop can be efficiently accelerated by SIMDization or on chip multi-processors), 
it will not generate the code for CGRA. 
Thus, CCF compiler will inform that why it currently did not generate the code for the CGRA. 
On the other hand, if the CCF compiler generated the code for CGRA, then we can find it in the directory CGRAExec. 
This directory contains information about all the loops compiled for their execution on CGRA.

3. **Simulate Heterogeneous Execution**: 
Our simulation platform is built in gem5, where we have modeled the CGRA as a separate core coupled to ARM Cortex like processor. 
Instead of executing file se.py for system emulation mode, we can execute a file se_hetro.py, which models the heterogeneous execution. 
For example, if we simulate the execution with n=2 cores, 1 core is specified as a CGRA, and another n-1=1 is the processor core. 
If you are interested to do a detailed debug, you can turn on the debug flags, i.e., --debug-flags=CGRA,CGRA detailed. 
Such command will show comprehensively all the details about the status of the CGRA’s micro-architectural components.

Since the CCF is a prototype of emerging general-purpose CGRA accelerator, 
the current release of the CCF supports code generation for few performance critical loops of MiBench, an embedded benchmark suite. 
It also supports execution of all the loops from few MiBench applications.
You can locate these benchmarks under the directory $path/ccf/benchmarks/MiBench.
We plan to support the code generation for more benchmarks.

### Setting-Up the Infrastructure:

Once you clone this repository, you need to give execution access to installation scripts i.e. $path/ccf/install and $path/scripts/install_dependencies. 
Then, just execute the installation script by typing -

$ ./install 

It will install all the packages needed for LLVM and gem5 and will build the toolchain from the source.
Please note that since the current installation is non-interactive, it will install and setup all the necessary tools.
We have verified the infrastructure on Ubuntu (14.04). 
CCF compiler is also supported on Ubuntu 16.04 but, CCF's gem5 based simulator is not compatible with the Ubuntu 16.04 
since it is based on older gem5 version (from the stable branch). 
However, we plan to integrate support for CCF on the latest Ubuntu release.
Moreover, if you encounter any error pertaining to the locations of the framework tools, please feel free to modify the installation script, or to manually fix the paths.


### Implementation of CCF:

The diagram below shows the steps involved in CCF's code generation process. 

<p align="center">
  <img src="http://aviral.lab.asu.edu/wp-content/uploads/2018/05/ccf.png"/ height="500">
</p>

1.  **Extraction of the Annotated Loop**:
    CCF compiler’s front-end (implemented by modifying clang) identifies and extracts the loops from C/C++ code, 
    annotated by the programmer. 
    Then, it generates the intermediate representation (IR). 
    In compiling the application, CCF targets the highest optimization, i.e. optimization level 3, 
    including auto-vectorization enabled. 
    The part of the IR corresponding to the annotated loop contains the metadata so that 
    CCF compiler can perform analysis and transformations on it. CCF compiler analyzes whether it will generate the code for this loop for its execution on CGRA, or not. If it can, it acts on the part of the IR corresponding to the loop, generating the data dependency graph (DDG).
 
 2.	**Generation of DDG and Communication of the Live Data**: An LLVM pass generates the DDG of the loop, 
    which can be visualized using the dot tool. In DDG, the circles show the operations to be performed
    and the arcs show the data dependencies. 
     
    To communicate the necessary variables or the live data, 
    our CCF compiler inserts instructions to manage the data automatically through global variables. 
    This compilation strategy avoids inserting mem copies, rather by adopting a shared memory model. 
    It is because we visualize CGRA accelerator as tightly coupled with the core, 
    at the interface of level 2 cache, or by sharing a scratch-pad with the processor.
    The library call pertaining to the loop execution on CGRA is inserted and IR corresponding to the loop body is purged. 
    This modified IR is then taken to the machine code generation for the CPU.
 
 3.	**Mapping of DDG on the Target CGRA**: The mapping technique maps the DDG on the target CGRA, 
    and generates the prologue, kernel, and the epilogue of the mapping. 
    Mapping process includes iterative modulo scheduling of the DDG 
    and a place and route of scheduled DDG on the CGRA's architectural resources.
    Register allocation is also done during the mapping.
    
4.	**Generation of Machine Instructions**:  ThIs phase generates the machine instructions to configure the PEs,
    to pre-load the live values into CGRA registers during the prologue, 
    and to store live-out data during the epilogue.
    Our current CGRA instruction-set architecture (ISA) supports two different formats, 
    including several important opcodes and including, support for the byte level memory accesses.
    
5.	**Architectural Simulation**: Our simulation platform is built in gem5, 
    where we have modeled the CGRA as a separate core coupled to ARM Cortex like processor.
    CCF simulator allows simulating the execution of the generated code on the core+CGRA heterogeneous platform.    

    We gratefully acknowledge the contributions of these student developers - 
    Dipal Saluja, 
    Mahdi Hamzeh, 
    Mahesh Balasubramanian,
    Shail Dave, and
    Shrihari Rajendran Radhika.
    
### Additional Resources for Getting Started

- [Intro to CGRAs](https://youtu.be/4h2Po78be-Q)
- [A short demo of CCF](https://www.youtube.com/watch?v=iGV1lHsjy4w&t=1s)
- [A short manual on CCF Implementation](https://github.com/cmlasu/ccf/blob/master/CCF%20Manual.pdf)

### Contact Us

For any questions or comments on CCF development, please email us at cmlasu@gmail.com

CML's CGRA Webpage - http://aviral.lab.asu.edu/coarse-grain-reconfigurable-arrays/
