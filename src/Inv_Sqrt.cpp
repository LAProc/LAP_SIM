/*
   Copyright (c) 2010-2015, Ardavan Pedram and The University of Texas at Austin
   All rights reserved.
   
   This file is part of LAP_SIM and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/

#include "Inv_Sqrt.h"
using namespace std;

Inv_Sqrt::Inv_Sqrt(){}


Inv_Sqrt::Inv_Sqrt(double *& Row_Buses_Write,double *& Row_Buses_Read,double *& Column_Buses_Write,double *& Column_Buses_Read)
{
	Latency=InvSqrt_Latency;// TODO Auto-generated constructor stub
	Pipe_Regs_Curr=(double *) malloc ( sizeof(double)* Latency) ;
	Pipe_Regs_Next=(double *) malloc ( sizeof(double)* Latency) ;

	Read_Col_Buses= Column_Buses_Read;
	Read_Row_Buses= Row_Buses_Read;
	Write_Col_Buses=Row_Buses_Write;
	Write_Row_Buses=Column_Buses_Write;

}

Inv_Sqrt::~Inv_Sqrt()
{
	// TODO Auto-generated destructor stub
}

double Inv_Sqrt::Inv_Sqrt_Execute(double input){

	Pipe_Regs_Next[0]= pow(input,- 0.5);
	int i;
	for (i=1; i< Latency; i++)
		Pipe_Regs_Next[i]=Pipe_Regs_Curr[i-1];


	return Pipe_Regs_Curr[Latency-1];
}


int Inv_Sqrt::Cycle(){

	int i;
	for (i=0; i< Latency; i++)
		Pipe_Regs_Curr[i]=Pipe_Regs_Next[i];
	return 0;
}


int Inv_Sqrt::Execute (int Global_index, int Iter_Counter, int Latency_Counter,
		LAPU_Function routine,int LAPU_Current_State, int State_Start){

		switch (routine){

		case LAPU_Cholesky:

			switch (LAPU_Current_State){
				case 2: //  Chol_Inv_Sqrt

				//	if (Latency_Counter< (FMA_Latency-1))
						Inv_Sqrt_Execute( Read_Row_Buses[Iter_Counter]);
				/*	else{

						Write_Col_Buses[Iter_Counter]=Inv_Sqrt_Execute( Read_Row_Buses[Iter_Counter]);

						// cout<<"BUZZZZZZZZZZZZZZZZZZZZZZZZZZ"<<endl;
						// cout<<Inv_Sqrt_Execute( Read_Row_Buses[Iter_Counter])<<endl;

						 cout <<"Write_Col_Buses[Iter_Counter]"<<Write_Col_Buses[Iter_Counter]<<endl;
						 Write_Row_Buses[Iter_Counter]=Write_Col_Buses[Iter_Counter];
					}
				*/
				case 3: //Chol_BC_Invqrt:

					Write_Col_Buses[Iter_Counter]=Pipe_Regs_Curr[Latency-1];
					Write_Row_Buses[Iter_Counter]=Write_Col_Buses[Iter_Counter];

				break;
			}
		break;

		default: ;


		}

		return 0;

}


int Inv_Sqrt::Dump_Inv_Sqrt_Regs(){



	cout<<"Inv_Sqrt:....."<<endl;

//	cout<<"Write_My_Col_Reg_Curr="<<Write_My_Col_Reg_Curr<<endl;
//	cout<<"Write_My_Row_Reg_Curr="<<Write_My_Row_Reg_Curr<<endl;

	//What I see
//	cout<<"Read_My_Col_Bus="<<*Read_My_Col_Bus<<endl;
//	cout<<"Read_My_Row_Bus="<<*Read_My_Row_Bus<<endl;

	int i;
	for (i=0; i< Latency; i++)
			cout<<"Pipe_Regs_Curr["<<i<<"]="<<Pipe_Regs_Curr[i]<<endl;




	return 0;

}




