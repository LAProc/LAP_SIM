/*
   Copyright (c) 2010-2015, Ardavan Pedram and The University of Texas at Austin
   All rights reserved.
   
   This file is part of LAP_SIM and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/

#include "LAPU.h"

using namespace std;

LAPU::LAPU()
{
	// TODO Auto-generated constructor stub


	Cycles_Passed=0;
	Counter_Curr=0;
	Counter_Next=1;

	Mc_Counter_Curr=0;
	Mc_Counter_Next=0;

	Kc_Counter_Curr=0;
	Kc_Counter_Next=0;

	N_Counter_Curr=0;
	N_Counter_Next=0;

  Last_Sending = -2; //considering bus delay
  done = false;

  //Last_Sending = 0;


	State_Start= FALSE;

	Size=LAPU_Size;

	Row_Buses_Read= (double *) malloc (sizeof (double) * LAPU_Size);
	Column_Buses_Read= (double *) malloc (sizeof (double) * LAPU_Size);
	Row_Buses_Write= (double *) malloc (sizeof (double) * LAPU_Size);
	Column_Buses_Write= (double *) malloc (sizeof (double) * LAPU_Size);

	PE_Array =(PE **)  malloc ( sizeof (PE *) * (LAPU_Size));
	int i,j;
	for ( i=0; i<Size; i++){
		PE_Array[i]=(PE *)  malloc ( sizeof (PE ) * (LAPU_Size));
		for (j=0; j<Size; j++)
			PE_Array[i][j]= *new PE(i,j, &Row_Buses_Write[i],&Row_Buses_Read[i],
										&Column_Buses_Write[j],&Column_Buses_Read[j]);
	}

	 Sqrt_Unit= new Inv_Sqrt(Row_Buses_Write, Row_Buses_Read, Column_Buses_Write, Column_Buses_Read);
		cout<< "in lapu const"<<endl;

	Mem_IF= new IO(Row_Buses_Write, Row_Buses_Read, Column_Buses_Write, Column_Buses_Read);

	LAPU_Power_Consumed=0;
}



LAPU::~LAPU()
{
	// TODO Auto-generated destructor stub
}

int LAPU::Return_Cycle_Count(){
	return Cycles_Passed;
}
int LAPU::Initialize_Mem( double ** Input_matrix, int row_number, int column_number, int offset){

	for (i=0;i<Size;i++)
		for (j=0;j<Size;j++){
			PE_Array[i][j].Intialize_Local_Mem(Input_matrix,row_number,column_number, offset);

		}
	return 0;

}

int LAPU::Initialize_Mem_New( double ** Input_matrix, int row_number, int column_number, int offset, char matr){

	for (i=0;i<Size;i++)
		for (j=0;j<Size;j++){
			PE_Array[i][j].Initialize_Local_Mem_New(Input_matrix,row_number,column_number, offset, matr);

		}
	return 0;
}

int LAPU::Flush_Mem( double **& Input_matrix, int row_number, int column_number,int offset){

	for (i=0;i<Size;i++)
		for (j=0;j<Size;j++){
			PE_Array[i][j].Flush_Local_Mem(Input_matrix,row_number,column_number, offset);

		}
	return 0;
}

int LAPU::Flush_Mem_New( double **& Input_matrix, int row_number, int column_number,int offset, char matr){

	for (i=0;i<Size;i++)
		for (j=0;j<Size;j++){
			PE_Array[i][j].Flush_Local_Mem_New(Input_matrix,row_number,column_number, offset, matr);

		}
	return 0;
}


int LAPU::Assign_input_Matrix( double **& matrix_A, double **& matrix_B, double **& matrix_C){

	Matrix_A=matrix_A;
	Matrix_B=matrix_B;
	Matrix_C=matrix_C;

	Mem_IF->Assign_input_Matrix(matrix_A, matrix_B, matrix_C);
	return 0;
}




int LAPU::Return_LAPU_Power(){


	for (i=0;i<Size;i++)
		for (j=0;j<Size;j++){
			LAPU_Power_Consumed+=PE_Array[i][j].Return_PE_Power();
		}

	return LAPU_Power_Consumed;

}


int LAPU::Dump_Row_Buses(){

	cout<<"Row_Bus_Write  And ROw Bus Read :"<<endl;

	for (i=0; i <LAPU_Size;i++){
		cout<<"-"<<i<<":"<< Row_Buses_Write[i]<<"  ";
		cout<<""<< Row_Buses_Read[i]<<endl;
	}
	cout<<"****"<<endl;
	return 0;

}


int LAPU::Dump_Column_Buses(){

	cout<< "Column Bus Write"<<endl;
	cout<< "Column Bus Read"<<endl;

	for (i=0; i <LAPU_Size;i++)
		cout<<i<<"| : ";
	cout<<endl;

	for (i=0; i <LAPU_Size;i++)
			cout<<Column_Buses_Write[i]<<" , ";
	cout<<endl;
	for (i=0; i <LAPU_Size;i++)
		cout<<Column_Buses_Read[i]<<" , ";

	cout<<endl;


	cout<<"****"<<endl;
	return 0;


}






int LAPU::Dump_Matmul_SMachine(){

	cout<<"Ma :"<< Ap_Counter_Curr<<endl;
	cout<<"N :"<< N_Counter_Curr<<endl;
	cout<<"Mc"<<Mc_Counter_Curr<<endl;
	cout<<"Kc"<<Kc_Counter_Curr<<endl;

	switch (Matmul_Current_State){

		case Matmul_Init:
			cout<<" Matmul_Init"<<endl;
		break;


		case Matmul_FetchB:
			cout<<" Matmul_FetchB"<<endl;

		break;

		case Matmul_FetchA:
			cout<<" Matmul_FetchA"<<endl;

		break;

		case Matmul_BC0:
			cout<<" Matmul_BC0"<<endl;

		break;

		case Matmul_BC:
			cout<<" Matmul_BC"<<endl;

		break;

		case Matmul_MAC_BC:
			cout<<" Matmul_MAC_BC"<<endl;

		break;

		case Matmul_MAC_Flush:
			cout<<" Matmul_MAC_FLush"<<endl;

		break;

		case Matmul_End:
			cout<<" Matmul_End"<<endl;

		break;


	}

	cout<<"Latency_Count ="<<Latency_Counter_Curr<<endl;


	switch (Matmul_Next_State){

		case Matmul_Init:
			cout<<" Matmul_Init"<<endl;
		break;


		case Matmul_FetchB:
			cout<<" Matmul_FetchB"<<endl;

		break;

		case Matmul_FetchA:
			cout<<" Matmul_FetchA"<<endl;

		break;

		case Matmul_BC0:
			cout<<" Matmul_BC0"<<endl;

		break;

		case Matmul_BC:
			cout<<" Matmul_BC"<<endl;

		break;

		case Matmul_MAC_BC:
			cout<<" Matmul_MAC_BC"<<endl;

		break;
		case Matmul_MAC_Flush:
			cout<<" Matmul_MAC_FLush"<<endl;

		break;

		case Matmul_End:
			cout<<" Matmul_End"<<endl;

		break;

	}




	cout<<endl;
	return 0;

}





int LAPU::Dump_PE_Registers(int row, int column){


 PE_Array[row][column].Dump_Regs();
 return 0;

}


int LAPU::Dump_All_PE_Registers(){


	for (i=0;i<LAPU_Size;i++)
		for (j=0;j<LAPU_Size;j++)
			 PE_Array[i][j].Dump_Regs();
	return 0;
}



int LAPU::Dump_PE_ALU(int row, int column, ALU_op operation_type){

	 PE_Array[row][column].Dump_ALU_Pipeline( operation_type);
	 return 0;
}

int LAPU::Dump_All_PE_ALUs(ALU_op operation_type){

	for (i=0;i<LAPU_Size;i++)
		for (j=0;j<LAPU_Size;j++)
			 PE_Array[i][j].Dump_ALU_Pipeline( operation_type);
	return 0;
}

int LAPU::Dump_Sqrt_Unit(){


	Sqrt_Unit->Dump_Inv_Sqrt_Regs();
	return 0;

}



int LAPU::Cycle(){



//	int i,j;
	for (i=0;i<Size;i++)
		for (j=0;j<Size;j++){
			PE_Array[i][j].Cycle();
		}
	Sqrt_Unit->Cycle();

	Counter_Curr=Counter_Next;
	Latency_Counter_Curr=Latency_Counter_Next;

	Mc_Counter_Curr=Mc_Counter_Next;

	Kc_Counter_Curr=Kc_Counter_Next;

	N_Counter_Curr=N_Counter_Next;

  Ap_Counter_Curr = Ap_Counter_Next;



  Matmul_Current_State=Matmul_Next_State;

	Cycles_Passed++;
	return Cycles_Passed;
}

int LAPU::Drive_Buses(){

	for (i=0; i<LAPU_Size ; i++){
		Row_Buses_Read[i]=Row_Buses_Write[i];
		Column_Buses_Read[i]=Column_Buses_Write[i];

	}

	return 0;
}







/*

int LAPU::Matmul_Rank_D(int Global_index){


	while (1){

		switch(MatMul_Current_State){

//		case







		}








	}

	return 0;
}


*/


int LAPU::Matmul_Kernel(int Global_index){

	while (1){

			switch (Matmul_Current_State){

				case Matmul_Init:

					Kc_Counter_Next=(Kc_Counter_Curr+1);
					if (Kc_Counter_Curr==(LAPU_Size-1)){
						Matmul_Next_State=Matmul_FetchB;
						Kc_Counter_Next=0;
						Mc_Counter_Next=0;

					}

					//wait for PEs to get the data
					Latency_Counter_Next=0;
					Latency_Counter_Curr=0;



				break;


				case Matmul_FetchB:

					Kc_Counter_Next=(Kc_Counter_Curr+1);

					if (Kc_Counter_Curr==(Kernel_Size-1)){ //bus latency is 1
						Matmul_Next_State=Matmul_FetchA;
						Kc_Counter_Next=0;
						Mc_Counter_Next=0;

					}

				break;


				case Matmul_FetchA:
          
          //Mochamad --> This was updated by Ardavan
  

					/*
					Kc_Counter_Next=(Kc_Counter_Curr+1) % Kernel_Size;
					Matmul_Next_State=Matmul_FetchA;

					if (Kc_Counter_Curr== (Kernel_Size -1)){

						Mc_Counter_Next=(Mc_Counter_Curr+LAPU_Size)% Kernel_Size;
						//Mc_Counter_Next=(Mc_Counter_Curr+1)% Kernel_Size;

						if (Mc_Counter_Curr==(Kernel_Size-LAPU_Size)){
						//if (Mc_Counter_Curr==(Kernel_Size-1)){
							Matmul_Next_State=Matmul_BC0;
							//Matmul_Next_State=Matmul_End;
							Kc_Counter_Next=0;
							Mc_Counter_Next=0;

						}
					}
					 */


					Kc_Counter_Next=(Kc_Counter_Curr+LAPU_Size) % Kernel_Size;
					Matmul_Next_State=Matmul_FetchA;

					if (Kc_Counter_Curr== (Kernel_Size -LAPU_Size)){

						Mc_Counter_Next=(Mc_Counter_Curr+1)% Kernel_Size;
						//Mc_Counter_Next=(Mc_Counter_Curr+1)% Kernel_Size;

						if (Mc_Counter_Curr==(Kernel_Size-1)){
						//if (Mc_Counter_Curr==(Kernel_Size-1)){
							Matmul_Next_State=Matmul_BC0;
							//Matmul_Next_State=Matmul_End;
							Kc_Counter_Next=0;
							Mc_Counter_Next=0;

						}
					}


				break;

				case Matmul_BC0: // Write_Row_Reg<-SRAM[A(0,0)] in the previous state
					// just BC the 0th
					// and read the 1st from the SRAM
					//Nothing is on the bus
					//Matmul_Next_State=Matmul_End;
					Matmul_Next_State=Matmul_BC;

				break;


				case Matmul_BC:  // it loops equal to bus delay for future

					Matmul_Next_State= Matmul_MAC_BC;
					Kc_Counter_Next=Kc_Counter_Curr+1;

				break;

				case Matmul_MAC_BC:

						Matmul_Next_State=Matmul_MAC_BC;

            /*cout << "Kc and Mc in FSM are" << Kc_Counter_Curr 
                 << " " << Mc_Counter_Curr << endl;
            getchar();*/

            //Added one more inner if statement, for multiple kernels of A

						Kc_Counter_Next=(Kc_Counter_Curr+1) % Kernel_Size;
						if (Kc_Counter_Curr== (Kernel_Size -1)){
							Mc_Counter_Next=(Mc_Counter_Curr+LAPU_Size)% Kernel_Size;
							if (Mc_Counter_Curr==(Kernel_Size-LAPU_Size)){
								N_Counter_Next=(N_Counter_Curr+LAPU_Size)% Panel_Size;
								if (N_Counter_Curr== (Panel_Size -LAPU_Size)){
                  Ap_Counter_Next = (Ap_Counter_Curr + 1)%NumofKernel;
                  if (Ap_Counter_Curr == NumofKernel - 1){
								  	Matmul_Next_State=Matmul_MAC_Flush;
                  }
                }

							}

						}


				break;

				case Matmul_MAC_Flush:
        
          //updated by Mochamad 
          
          //Kc_Counter_Next=(Kc_Counter_Curr+1) % Kernel_Size;
          

          //This is basically, we want to wait for the last 4 elements which are still in the pipeling
          //That is why we wait until it reacehs the FMA Latency
          //If reaches, then move to the end state


					Latency_Counter_Next=Latency_Counter_Curr+1;

					if (Latency_Counter_Curr< (FMA_Latency-1))
						Matmul_Next_State=Matmul_MAC_Flush;
					else{
						Matmul_Next_State=Matmul_End;
						Latency_Counter_Next=0;
					}
				break;


				case Matmul_End:

          //getchar();
        
          //here, we count the last sending
          //If all last 4 elements have been sent, then put the done signal as true
          //Later if done signal is true and the state machine is in Matmul_End, then we return
          //cycle consumption

          if (Last_Sending==LAPU_Size) done =true;
          
          Last_Sending++;

					Kc_Counter_Next=0;
					Kc_Counter_Curr=0;
					Mc_Counter_Curr=0;
					Mc_Counter_Next=0;
					N_Counter_Curr=0;
					N_Counter_Next=0;

				break;


			}

			if (Print_State_Machines==1){
				cout<<"==============================";
				cout<<"Cycle"<<Cycles_Passed<<endl;
			//	char test2;
			//	cout<<"Press Enter"<<endl;
			//	cin>>test2;
				Dump_Matmul_SMachine();
			}
			for (i=0;i<Size;i++)
				for (j=0;j<Size;j++){
					PE_Array[i][j].Execute_Matmul (Global_index, N_Counter_Curr, Mc_Counter_Curr, Kc_Counter_Curr, Ap_Counter_Curr, Matmul_Current_State, Latency_Counter_Curr);
					// We pass this Routine an the current state of this routine to PE
				}

			//Sqrt_Unit->Execute(Global_index, Counter_Curr, Latency_Counter_Curr,
				//	LAPU_Rank_Update, Gemm_Current_State, State_Start ); // TODO fix the input;

			Mem_IF->IO_Execute_Matmul (Global_index, N_Counter_Curr, Mc_Counter_Curr, Kc_Counter_Curr, Ap_Counter_Curr, Matmul_Current_State, Latency_Counter_Curr);

			 if (((Matmul_Current_State==Matmul_End) && done) /*|| Ap_Counter_Curr*/ ){
				 Matmul_Current_State=Matmul_Init;
				 Matmul_Next_State=Matmul_Init;
				 return Cycles_Passed;
			 }
			Dump_Row_Buses();
			Dump_Column_Buses();

			Drive_Buses(); // Can it be a part of Cycle function? I remember I just separated it for readability
			// It does not matter which one comes first Drive_Bus or Cycle;

			/*if (Kc_Counter_Curr==0 && Matmul_Current_State==Matmul_MAC_BC) {
				getchar();
			}*/
			Cycle();

      //if(Ap_Counter_Curr && !Kc_Counter_Curr)getchar();


	    	}
	    return 0;
    }
