/*
 * Inv_Sqrt.h
 *
 *  Created on: Mar 5, 2010
 *      Author: ardavan
 */

#ifndef INV_SQRT_H_
#define INV_SQRT_H_


#include "Parameters.h"

class Inv_Sqrt
{
public:
	Inv_Sqrt();
	Inv_Sqrt(double *& Row_Buses_Write,double *& Row_Buses_Read,double *& Column_Buses_Write,double *& Column_Buses_Read);
	Inv_Sqrt(int i);
	virtual ~Inv_Sqrt();

	double Inv_Sqrt_Execute( double input);
	double Sqrt_Execute	( double input);

	int Execute(int Global_index, int Iter_Counter, int Latency_Counter,
							LAPU_Function routine,int LAPU_Current_State, int State_Start);

	int Cycle();


	int Dump_Inv_Sqrt_Regs();
private:
	int Latency;  // in # of pipeline stages

	double * Pipe_Regs_Curr;
	double * Pipe_Regs_Next;

	double * Read_Col_Buses , * Read_Row_Buses;
	double * Write_Col_Buses, * Write_Row_Buses;

};

#endif /* INV_SQRT_H_ */
