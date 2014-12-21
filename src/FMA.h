/*
 * FMA.h
 *
 *  Created on: Mar 4, 2010
 *      Author: ardavan
 */


#include "Parameters.h"

#ifndef FMA_H_
#define FMA_H_

class FMA
{
public:
	FMA();
	virtual ~FMA();
	double Execute_MAD(double Multiplicand_1, double Multiplicant_2,double Addend);
	double Execute_MAC(double Multiplicand_1, double Multiplicant_2);
	double Execute_Mul(double Multiplicand_1, double Multiplicant_2);
	double Execute_Add(double Addend_1, double Addend_2, int add_sub);
	void Load_Accumulator(double data);
	double Return_ACC();
	int Dump_Pipeline( ALU_op operation_type);

	int Cycle();
	int Return_FMA_Power_Consumed();


private:
	int MAD_Latency;  // in # of pipeline stages
	int Mul_Latency;
	int Add_Latency;

	double * MAD_Pipe_Regs_Curr;
	double * MAD_Pipe_Regs_Next;

	double * MAC_Pipe_Regs_Curr;
	double * MAC_Pipe_Regs_Next;

	double * Mul_Pipe_Regs_Curr;
	double * Mul_Pipe_Regs_Next;

	double * Add_Pipe_Regs_Curr;
	double * Add_Pipe_Regs_Next;

	double Accumulator_Curr;
	double Accumulator_Next;

	int Power_Consumed;

};

#endif /* FMA_H_ */
