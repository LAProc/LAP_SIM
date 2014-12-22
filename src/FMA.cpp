/*
   Copyright (c) 2010-2015, Ardavan Pedram and The University of Texas at Austin
   All rights reserved.
   
   This file is part of LAP_SIM and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/

#include "FMA.h"
//#include "Parameters.h"
using namespace std;
FMA::FMA()
{
	// TODO Auto-generated constructor stub
	MAD_Latency=FMA_Latency-1;
	Mul_Latency= Multiplication_Latency-1;
	Add_Latency=Addition_Latency-1;

	MAD_Pipe_Regs_Curr= (double *) malloc ( sizeof (double) * MAD_Latency );
	MAD_Pipe_Regs_Next= (double *) malloc ( sizeof (double) * MAD_Latency );

	MAC_Pipe_Regs_Curr= (double *) malloc ( sizeof (double) * MAD_Latency );
	MAC_Pipe_Regs_Next= (double *) malloc ( sizeof (double) * MAD_Latency );


	Mul_Pipe_Regs_Curr= (double *) malloc ( sizeof (double) * Mul_Latency );
	Mul_Pipe_Regs_Next= (double *) malloc ( sizeof (double) * Mul_Latency );

	Add_Pipe_Regs_Curr= (double *) malloc ( sizeof (double) * Add_Latency );
	Add_Pipe_Regs_Next= (double *) malloc ( sizeof (double) * Add_Latency );

	Power_Consumed=0;
}

FMA::~FMA(){
	// TODO Auto-generated destructor stub
}


double FMA::Execute_MAD(double A, double B, double C){

	Power_Consumed++;

	MAD_Pipe_Regs_Next[0]= A*B+C ;
	int i;
	for (i=1; i< MAD_Latency; i++)
		MAD_Pipe_Regs_Next[i]=MAD_Pipe_Regs_Curr[i-1];

	return MAD_Pipe_Regs_Curr[MAD_Latency-1];
}

double FMA::Execute_Mul(double A, double B){

	Power_Consumed++;

	Mul_Pipe_Regs_Next[0]= A*B ;
	int i;
	for (i=1; i< Mul_Latency; i++)
		Mul_Pipe_Regs_Next[i]=Mul_Pipe_Regs_Curr[i-1];

	return Mul_Pipe_Regs_Curr[Mul_Latency-1];
}

double FMA::Execute_Add(double A, double B, int add_sub){

	Power_Consumed++;
	Add_Pipe_Regs_Next[0]= A + pow (-1, add_sub) *B ;
	int i;
	for (i=1; i< Add_Latency; i++)
		Add_Pipe_Regs_Next[i]=Add_Pipe_Regs_Curr[i-1];

	return Mul_Pipe_Regs_Curr[Add_Latency-1];
}




int FMA::Cycle(){

	int i;

	for (i=0; i < MAD_Latency; i++){
		MAD_Pipe_Regs_Curr[i]=MAD_Pipe_Regs_Next[i];
		MAC_Pipe_Regs_Curr[i]=MAC_Pipe_Regs_Next[i];
	}
	for (i=0; i < Mul_Latency; i++)
		Mul_Pipe_Regs_Curr[i]=Mul_Pipe_Regs_Next[i];

	for (i=0; i < Add_Latency; i++)
		Add_Pipe_Regs_Curr[i]=Add_Pipe_Regs_Next[i];

	Accumulator_Curr=Accumulator_Next;
	return 0;
}



double FMA::Execute_MAC(double A, double B){

	Power_Consumed++;

/*
	if ((A*B)!=0){
		Accumulator_Next= Accumulator_Curr+A+.01*B;
		MAC_Pipe_Regs_Next[0]=A*B+Accumulator_Curr;
	}
	else {
		Accumulator_Next= Accumulator_Curr+A*B;
		MAC_Pipe_Regs_Next[0]=A*B+Accumulator_Curr ;
	}
*/
	Accumulator_Next= Accumulator_Curr+A*B;
	MAC_Pipe_Regs_Next[0]=A*B+Accumulator_Curr;
	int i;
	for (i=1; i< MAD_Latency; i++)
		MAC_Pipe_Regs_Next[i]=MAC_Pipe_Regs_Curr[i-1];

	return MAC_Pipe_Regs_Curr[MAD_Latency-1];



}

void FMA::Load_Accumulator(double data){

	Accumulator_Next=data;


}

double FMA::Return_ACC(){
	return Accumulator_Curr;
}


int FMA::Return_FMA_Power_Consumed(){


	return Power_Consumed;

}

int FMA::Dump_Pipeline( ALU_op operation_type){

	int i;
	switch (operation_type){

		case ALU_Add:
			cout<<"ADD_Pipe"<<endl;
			for (i=0; i < Add_Latency; i++)
				cout<< "Pipe["<<i<<"]="<<Add_Pipe_Regs_Curr[i]<<endl;


		break;

		case ALU_Mul:

			cout<<"Mul_Pipe"<<endl;
			for (i=0; i < Mul_Latency; i++)
				cout<< "Pipe["<<i<<"]="<<Mul_Pipe_Regs_Curr[i]<<endl;

		break;


		case ALU_MAD:

			cout<<"MAD_Pipe"<<endl;
			for (i=0; i < MAD_Latency; i++)
				cout<< "Pipe["<<i<<"]="<<MAD_Pipe_Regs_Curr[i]<<endl;


		break;

		case ALU_MAC:

			cout<<"MAC_Pipe"<<endl;

			for (i=0; i < MAD_Latency; i++)
				cout<< "Pipe["<<i<<"]="<<MAC_Pipe_Regs_Curr[i]<<endl;


		break;

	}

	return 0;

}



