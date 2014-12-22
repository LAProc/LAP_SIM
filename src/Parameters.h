/*
   Copyright (c) 2010-2015, Ardavan Pedram and The University of Texas at Austin
   All rights reserved.
   
   This file is part of LAP_SIM and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/

#ifndef PARAMETERS_H_
#define PARAMETERS_H_
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <map>
#include <string>


#define FALSE 0
#define TRUE 1

#define Print_Input_Output 0  // IF you want in and out set it to 1
#define Print_State_Machines 1
#define Print_BLAS 0

extern int Kernel_Size;
extern int Panel_Size;
extern int NumofKernel;


//#define Panel_Size  64//512   // n in paper
//#define Kernel_Size 32   		// m_c or k_c in paper


#define Buffer_local_mem_A    2 //Two means we are doing double buffering for local mem A

//#define NumofKernel    2  //This tells us how many kernel of A we are computing
                          //Basically the row size of Matrix A would be : NumofKernel * Kernel Size

#define LAPU_Size 4			// n_r  in paper
#define FMA_Latency  	7	 //cycles
#define Multiplication_Latency 7 		//cycles
#define Addition_Latency 7		//cycles
#define InvSqrt_Latency 15	//cycles



//#define Mem_Size  (Kernel_Size*Kernel_Size)/(LAPU_Size*LAPU_Size)//SRAM of the PE size  for Cholesky
 #define Mem_Size (((Kernel_Size*Kernel_Size)/(LAPU_Size*LAPU_Size)) + 2*Kernel_Size)
// m_c*k_c + 2*k_c*n_r*n_r

//added by Mochamad

extern int Mem_Size_A;
extern int Mem_Size_B;
extern int Mem_Size_B1;
extern int Mem_Size_B2;


/*#define Mem_Size_A ((Buffer_local_mem_A * Kernel_Size*Kernel_Size)/(LAPU_Size*LAPU_Size))
#define Mem_Size_B1 Kernel_Size //should add B2 also ?
#define Mem_Size_B2 Kernel_Size //should add B2 also ?
*/


#define Scratch_Size 4 //Scratch Pad memory

#define Memory_BW 8*( (2*LAPU_Size*LAPU_Size) /Kernel_Size)
// Counters

//Power
#define FMA_Dynamic  1
#define FMA_Leakage  1


#define InvSqrt_Dynamic 1



#define Bus_Dynamic     1


enum Matrix_Type{
  Mat_A,
  Mat_B,
  Mat_C
};

enum Test_Matrix{
  
  DIAG,
  DIAG_INCR,
  DIAG_MULT,
  ALL0,
  ALL1,
  PRESET,
  UP_RIGHT_TRIU_PRESET,
  UP_LEFT_TRIU_PRESET,
  LO_TRIU_PRESET,
  BANDED,


/*
	DIAG                  : Diagonal with fixed preset
	DIAG_INCR             : Increasing diagonal with Preset
	DIAG_MULT             : Increasing diagonal with multiple of preset
	ALL0                  : All 0 matrix
	ALL1                  : All 1 matrix
  PRESET                : Constant Matrix with preset value
	UP_RIGHT_TRIU_PRESET  : Top Right aligned triangular with preset
	UP_LEFT_TRIU_PRESET   : Top Left aligned triangular with Preset	
  LO_TRIU_PRESET        : Lower triangular with preset
  BANDED                : Banded Matrix
*/

};



enum LAPU_Function { LAPU_Cholesky, LAPU_Trsm, LAPU_Rank_Update};
enum ALU_op {ALU_Add,ALU_Mul, ALU_MAD, ALU_MAC};
#endif /* PARAMETERS_H_ */
