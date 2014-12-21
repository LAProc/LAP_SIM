 /*
 * LAPU.h
 *
 *  Created on: Mar 4, 2010
 *      Author: ardavan
 */


#ifndef LAPU_H_
#define LAPU_H_

#include "Parameters.h"
#include "Inv_Sqrt.h"
#include "PE.h"
#include "IO.h"

class LAPU
{
public:
	LAPU();
	virtual ~LAPU();



	int Matmul_Kernel(int Global_index);

	int Cycle();
	int Return_Cycle_Count();
	int Return_LAPU_Power();

/*	int Execute( LAPU_Function Function, int Global_index, int Trsm_index);
	int Cycle();
*/

	int Drive_Buses();

	int Initialize_Mem( double ** Input_matrix, int row_number, int column_number, int offset);

	int Flush_Mem(double **& Input_matrix, int row_number, int column_number, int offset); //TODO who makes decision about the offset/?

	int Initialize_Mem_New( double ** Input_matrix, int row_number, int column_number, int offset, char matr);

	int Flush_Mem_New(double **& Input_matrix, int row_number, int column_number, int offset, char matr); //TODO who makes decision about the offset/?
	
  int Assign_input_Matrix( double **& matrix_A, double **& matrix_B, double **& matrix_C);

	int Dump_PE_Registers(int row, int column);

	int Dump_All_PE_Registers();

	int Dump_PE_ALU(int row, int column, ALU_op operation_type);

	int Dump_All_PE_ALUs(ALU_op operation_type);


	int Dump_Row_Buses();
	int Dump_Column_Buses();


	int Dump_Matmul_SMachine();

	int Dump_Sqrt_Unit();


private:






	enum Matmul_States { Matmul_Init, Matmul_FetchB, Matmul_FetchA, Matmul_BC0, Matmul_BC, Matmul_MAC_BC, Matmul_MAC_Flush, Matmul_End} Matmul_Current_State, Matmul_Next_State;






	//TODO do I need different State enums for different types of operatons?
	int Size;
	int i,j;
	PE  ** PE_Array;

	double ** Matrix_A;
	double ** Matrix_B;
	double ** Matrix_C;

	int Counter_Curr;
	int Counter_Next;

	int Latency_Counter_Curr;
	int Latency_Counter_Next;
 // Added for Matmul

	int Mc_Counter_Curr;
	int Mc_Counter_Next;

	int Kc_Counter_Curr;
	int Kc_Counter_Next;

	int N_Counter_Curr;
	int N_Counter_Next;

  int Ap_Counter_Curr;
  int Ap_Counter_Next;

  int Last_Sending;
  bool done;

///


	bool State_Start;

	int Cycles_Passed;

	double * Row_Buses_Write;
	double * Row_Buses_Read;

	double * Column_Buses_Write;
	double * Column_Buses_Read;

	Inv_Sqrt * Sqrt_Unit;
	IO * Mem_IF;

	int LAPU_Power_Consumed;
};

#endif /* LAPU_H_ */
