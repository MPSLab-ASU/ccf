# CCF

## Overview 

CCF (**C**GRA **C**ompilation and Simulation **F**ramework) is an end-to-end prototype that generates machine code for CGRAs (Coarse-Grained Reconfigurable Arrays) and simulate their execution and performance. Through CCF, users can benchmark how CGRAs accelerate kernels designed for general-purpose applications. Currently, CCF can accelerate a number of loops from three benchmark suites, Parboil, MiBench, and Rodinia. CCFv2.0 is also refferred to as CCF 20.04 because this framework has been built and verified on Ubuntu 20.04. 

This framework can be seen in three parts: Scheduling and Mapping, Instruction Generation, and Simulation. The Scheduling and Mapping part of CCF is built upon the foundations of the cross-compiler LLVM 13, and the Simulation part of CCF is built upon the cycle-accurate computer architecture simulation gem5. Instruction Generation is what bridges the two parts together.

With LLVM 13 as a foundation, the implementation of CCF-compiler includes numerous compiler analysis and transformation passes, along with a customized code generation CGRA back-end. The user only needs to mark the performance-critical loops that they want to execute on CGRA, by using the annotation: #pragma CGRA, and the CCF-compiler automatically extracts the marked loops and maps them to the CGRA, generates code to communicate live data between the processor core and CGRA, pre-load the live values into CGRA registers, and generates the machine instructions to configure the PEs to execute the loop, and finally generates a binary that will execute on the CCF-simulator. The CCF-simulator is built by modifying cycle-accurate processor simulator Gem5, and it models CGRA as a separate core coupled to ARM Cortex-like processor core with ARMv7a profile. This open-source platform has been developed at Arizona State University and through CCF, we target accelerating the CGRA research by developing and making accessible a community-wide CGRA compilation infrastructure.


## How to install

### Disclaimer on building from source
CCFv2.0 does not require you to have sudo privileges to install. Because of this, you must set up your environment such that you can build binaries from source.

If you have set your environment properly, then you can skip to section "Editing `install.sh`". If not, continue following the instructions below.

### How to build from source

In order to build for source, you need to create a directory where you will store the CCF binaries. We will assume that you would like to make said directory in your root directory. We will also name that directory as `sw`, and we will create it using the command below.

```
cd ~
mkdir sw
```

After making this directory, we now need to change our environment variables such that BASH (or ZSH) is able to find the binaries installed in the directory. To do this, open the file `~/.bashrc` (or `~/.zshrc`) using your preferred text editor, and add the following lines:

```
export PATH="$SW/bin:$PATH"
expore LD_LIBRARY_PATH="$SW/lib:$LD_LIBRARY_PATH"
```

where `$SW` refers to the **full path** of directory `~/sw`. You can find the full path by running the following commands:

```
cd ~/sw
pwd
```

Once you have edited `~/.bashrc` (or `~/.zshrc`), run `source ~/.bashrc` (or `source ~/.zshrc`).

You have made the necessary changes to your environment and are now able to install CCF.

### Editing `install.sh`
Open `install.sh` using your preferred text editor, and go to line 5 (e.g the line with `install_path=""`). Add the complete path of where you would like to install your binaries in between the quotation marks. Afterwards, save the file and run the command `bash install.sh`. This should allow you install CCF. 


We gratefully acknowledge the contributions of these student developers - 
    Dipal Saluja, 
    Mahdi Hamzeh, 
    Mahesh Balasubramanian,
    Shail Dave,
    Shrihari Rajendran Radhika, and
    Quoc Long Vinh Ta.
    
### Additional Resources

- [Intro to CGRAs](https://youtu.be/4h2Po78be-Q)
- [A short demo of CCFv1.0](https://www.youtube.com/watch?v=iGV1lHsjy4w&t=1s)
- [A manual on CCF implementation](https://github.com/MPSLab-ASU/ccf/blob/master/CCFv2.0_Manual.pdf)
- [Tutorial](https://github.com/MPSLab-ASU/ccf/blob/main/CCFv2.0_Tutorial.pptx)
- [Paper](https://past.date-conference.com/system/files/file/date18/ubooth/121371.pdf)
- [BibTex](https://github.com/MPSLab-ASU/ccf/blob/main/BibTex.bib)
- [CGRA research @ MPSLab-ASU](https://labs.engineering.asu.edu/mps-lab/research/cgra/)
- [CGRA research papers](https://labs.engineering.asu.edu/mps-lab/publications/?tsr=&tps_button=Search&yr=&type=&tgid=17&auth=)
