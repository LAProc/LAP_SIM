Linear Algebra processor
Cycle Accurate Simulator version 0.1

Features:
-Single Linear Algebra Core 
-Performing Rank-K Update


This version simulates Matrix Multiplication
Please watch the presentation at:
 http://youtu.be/DdpF7Oj_4cU



The papers regarding this version could be found at.

For citation and memory hierarchy:
"Codesign Tradeoffs for High-Performance, Low-Power Linear Algebra Architectures,” 
IEEE Transactions on Computers (TC) Special Issue on Energy Efficient Computing, 
Volume 61, Issue 12, Page(s) 1724 – 1736, December 2012. 
Ardavan Pedram, Robert van de Geijn, Andreas Gerstlauer 

For more details regarding the core u-architecture:
"A High-performance, Low-power Linear Algebra Core,” 
The 22nd IEEE International Conference on Application-specific Systems, Architectures and Processors ASAP 2011.
Ardavan Pedram, Andreas Gerstlauer, Robert van de Geijn







Simulator details:


Use makefile to make the two main executables.
Matrix_Maker and LAC_SIM

The Matrix_Maker creats sample input matrices and saves them in an output file.
You can add your own routine for making a matrix to it.


The LAC_SIM
The Simulator.cpp is the main file calling the simulator engine.
Takes 5 input parameters
File names to retrieve the input matrices: 
<Matrix A> input A
<Matrix B> input B
<Matrix C_in> input C_in
<Matrix C_out> the output of computed matrix goes here
<Matrix C_Golden> the output of naive nested loop software multiplication goes here
The simulator compares C_out and C_Golden at the end to verify correctness of the results




Parameters.h contains all the parameters of the simulator including
-dimensions 
-local PE memory sizes
-pipeline latencies of the FMA
-register file size

The Simulator Core object is LAPU.cpp

The core contains the 
-Broadcast buses, 
-IO unit.
-PEs,
-Special Function Unit(not used in this version)

The state machine is factored in the LAPU.cpp
The core sends the signals including the iteration and state to each PE and IO unit

The PE.cpp contains the execute function which is called by LAPU.cpp and executes one cycle of the machine.

Each PE contains the 
-FMA(Fused Multiply ADD Unit), 
-Register file, 
-Memory SRAMs,
-Small register file, 
-latches to and from broadcast buses


Feel free to contact with any questions 
ardavan@utexas.edu


I will update this file with FAQ

















