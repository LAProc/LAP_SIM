/*
   Copyright (c) 2010-2015, Ardavan Pedram and The University of Texas at Austin
   All rights reserved.
   
   This file is part of LAP_SIM and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/

#ifndef IO_H_
#define IO_H_
#include "Parameters.h"
class IO
{
public:
	IO();
	IO(double *& Row_Buses_Write,double *& Row_Buses_Read,double *& Column_Buses_Write,double *& Column_Buses_Read);

	int Assign_input_Matrix( double **& matrix_A, double **& matrix_B, double **& matrix_C);
	int IO_Execute();

	int IO_Read_Mem();
	int IO_Write_Mem();

	int IO_Execute_Matmul (int Global_index, int N, int Mc, int Kc, int Ma, int  Matmul_Current_State, int Latency_Counter_Curr);

	virtual ~IO();

private:


	int data_transfer_total;

	double * Read_Col_Buses , * Read_Row_Buses;
	double * Write_Col_Buses, * Write_Row_Buses;


	double ** Matrix_A;
	double ** Matrix_B;
	double ** Matrix_C;

	double    Buffer_A[LAPU_Size][LAPU_Size];
	double    Buffer_B[LAPU_Size][LAPU_Size];
	double    Buffer_Cin[LAPU_Size][LAPU_Size];
	double    Buffer_Cout[LAPU_Size][LAPU_Size];

	int Cin_Counter;
	int Cout_Counter;
	int Bin_Counter;
  int Ain_Counter;

  //added by Mochamad
	int Done;
  int last_x;
  int last_y;
  int FcA;

  bool fetch_C;
  bool fetch_B;
  bool fetch_A;
  bool send_C;
  
  int Aip_fetch_counter;

};

#endif /* IO_H_ */
