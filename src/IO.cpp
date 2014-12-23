/*
   Copyright (c) 2010-2015, Ardavan Pedram and The University of Texas at Austin
   All rights reserved.
   
   This file is part of LAP_SIM and is under the BSD 3-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-3-Clause
*/

#include "IO.h"
using namespace std;
IO::IO()
{
	// TODO Auto-generated constructor stub
	data_transfer_total=0;
}

IO::~IO()
{
	// TODO Auto-generated destructor stub
}


IO::IO(double *& Row_Buses_Write,double *& Row_Buses_Read,double *& Column_Buses_Write,double *& Column_Buses_Read){


	Read_Col_Buses= Column_Buses_Read;
	Read_Row_Buses= Row_Buses_Read;
	Write_Col_Buses=Column_Buses_Write;
	Write_Row_Buses=Row_Buses_Write;

	Cin_Counter=0;
	Cout_Counter=-2;
	Bin_Counter=0;
  Ain_Counter = 0;

  last_x =0;
  last_y =0;

  fetch_C = true;
  fetch_B = false;
  fetch_A = false;
  send_C = false;

  FcA = 0;
  Aip_fetch_counter = 0;

}

int IO::Assign_input_Matrix( double **& matrix_A, double **& matrix_B, double **& matrix_C){

	Matrix_A=matrix_A;
	Matrix_B=matrix_B;
	Matrix_C=matrix_C;
  return 0;

}


int IO::IO_Execute_Matmul (int Global_index, int N, int Mc, int Kc, int Ap, int  Matmul_Current_State, int Latency_Counter_Curr){

	int i,j,k;



	switch( Matmul_Current_State){

		case 0: //Init: fetch C in Regfile


			for (i=0;i<LAPU_Size;i++) // write on all buses the rows in steps
				Write_Col_Buses[i]=Matrix_C[Kc][i];


		break;


		case 1: // Fetch B

			for (i=0;i<LAPU_Size;i++)
				Write_Col_Buses[i]=Matrix_B[Kc][i];


		break;

		case 2:

			for (i=0;i<LAPU_Size;i++)
				//Write_Col_Buses[i]=Matrix_A[Mc+Kc%LAPU_Size][(Kc/LAPU_Size)*LAPU_Size+i];
				Write_Col_Buses[i]=Matrix_A[Mc][(Kc/LAPU_Size)*LAPU_Size+i];

			//cout <<"on the bus"<<Matrix_A[Mc+Kc%LAPU_Size][(Kc/LAPU_Size)*LAPU_Size+i-1]<<endl;
		break;


		case 3:

		break;

		case 4:

		break;
		case 5:

			/*if ((Kc>=0) && (Kc< (Kernel_Size/3))){
				//reset all counters
				Cin_Counter=0;
				Cout_Counter=-2; // -2*(delay of bus);
				Bin_Counter=0;


					// read Cin instantly (just now)
					for (i=0;i<LAPU_Size;i++)
						for (j=0;j<LAPU_Size;j++){

							Buffer_Cin[i][j]=Matrix_C[((Mc+LAPU_Size)%Kernel_Size)+i]
													 [(N+ ((Mc+LAPU_Size)/Kernel_Size)*LAPU_Size)% Panel_Size+j]; //TODO;

					}
				Done=0;
			}*/
		
      //Reset all counters here 

      if (Kc==0 || (Kc==1 && Mc==0 && N==0 && Ap==0)){
				//reset all counters
				Cin_Counter=0;
				Cout_Counter=-2; // -2*(delay of bus);
				Bin_Counter=0;
        Ain_Counter = 0;

        fetch_C = true;
        fetch_B = false;
        fetch_A = false;
        send_C = false;

        if (N==0 && Mc==0 && Ap!=0) Aip_fetch_counter = 0;
       
        //FcA is a counter of how many A we have fetched


					// read Cin instantly (just now)
					for (i=0;i<LAPU_Size;i++)
						for (j=0;j<LAPU_Size;j++){
              

              //This is the case where we are in the end finishing one panel of B
              if(N==Panel_Size-LAPU_Size && Mc==Kernel_Size-LAPU_Size){
            
                //If we are not in the last kernel of A
                if (Ap!= NumofKernel-1)
							    Buffer_Cin[i][j]=Matrix_C[((Mc+LAPU_Size)%Kernel_Size)+i + (Ap+1)*Kernel_Size]
													 [(N+ ((Mc+LAPU_Size)/Kernel_Size)*LAPU_Size)% Panel_Size+j]; //TODO;
              
              }

              else{ //When we are in the last kernel of A 

							  Buffer_Cin[i][j]=Matrix_C[((Mc+LAPU_Size)%Kernel_Size)+i + Ap*Kernel_Size]
													 [(N+ ((Mc+LAPU_Size)/Kernel_Size)*LAPU_Size)% Panel_Size+j]; //TODO;
              }

					}

				
         /* for (i=0;i<LAPU_Size; i++)
						Write_Col_Buses[i]=Buffer_Cin[Cin_Counter][i];
        
        Cin_Counter++;*/

				Done=0;
			}

			if (fetch_C ){


        // Send C to the Col Buses
        // And simultaneously put A in the buffer to send next time


				if (Done==0){
					Done++;
					// read A instantly in the beginning of the second period
				  
          for (i=0;i<LAPU_Size;i++)
						for (j=0;j<LAPU_Size;j++){

              if (N<Kernel_Size){
              
              //Assuming that every Kc Iteration we fetch LAPU_Size*LAPU_Size elements of A, then 
              //we only need to fetch A as many as Kernel_Size*Kernel_Size
              //And for every one N iteration, we fetch Kernel_Size 
              //That is why, then we only need to set N<Kernel_Size to fetch Kernel_Size*Kernel_Size A

							Buffer_A[i][j]=Matrix_A[((Ap+1)%NumofKernel)*Kernel_Size + N + i][Mc+j]; //TODO;
              cout << "Buffer A is " << Buffer_A[i][j] << endl;
              //getchar();
              }
						}

				}

				//Send the Cin on the buses cycle by cycle;
				if (Cin_Counter< LAPU_Size){

					cout<<"Sending CIN "<<endl;
					for (i=0;i<LAPU_Size; i++){
						Write_Col_Buses[i]=Buffer_Cin[Cin_Counter][i];
					  cout<<Write_Col_Buses[i]<<endl;
          }
					Cin_Counter++;
				}

        if (Cin_Counter==LAPU_Size) {
          fetch_A = true;
          fetch_C = false;
          Done = 2;
        }
      }

			else if (fetch_A){      //fetching A

        //Send A to Col buses

        if (Ain_Counter<LAPU_Size && ((Kc%LAPU_Size)!=2)){
					//cout <<"Cout_Counter"<<Cout_Counter<<endl;
          //
          //FcA is a counter to keep track how many A have we sent for one kernel of A
          //This will be reset to 0 everytime we calculate new kernel of A
          //If it is equal to (Kernel_Size*Kernel_Size)/(LAPU_Size * LAPU_Size)
          //Then that means we are good. We prefetched all the data for the next iteration
          //Updated : FcA is replaced by Aip_fetch_Counter

				 if(Aip_fetch_counter!= (Kernel_Size*Kernel_Size)/(LAPU_Size * LAPU_Size)){
          if (Ain_Counter>=0){
					  for (i=0;i<LAPU_Size; i++){
						  Write_Col_Buses[i]=Buffer_A[Ain_Counter][i];
              cout <<"Ain from IO is"<< Write_Col_Buses[i] << endl;
            }
					  Ain_Counter++;
            cout<<"fetch_A now "<<endl;
            cout<<"Aip_fill_Counter now is " << Aip_fetch_counter <<endl;
            //getchar();
				  }

          else Aip_fetch_counter++; //increment FcA after all As have been saved in all PEs
         }

         else Ain_Counter++;
          
         if(Ain_Counter==LAPU_Size){
         
            send_C = true;
            Ain_Counter = 0;
            fetch_A = false;
         }

        }
			}


			else if(send_C) { // if in the third period

        //Get Cout and simultaneously prepare B for the next time


        if(Done==2){
          Done =0;

          //prefetch B
          
					for (i=0;i<LAPU_Size;i++)
						for (j=0;j<LAPU_Size;j++){
              Buffer_B[i][j]=Matrix_B[Mc+i][(N+LAPU_Size)% Panel_Size+j];
          } 
        }
        
        //Read result from LAPU
        if(Cout_Counter<LAPU_Size){

          if(Cout_Counter>=0){
            cout << "Saving COUT " <<endl;
            for(i=0; i<LAPU_Size; i++)
              Buffer_Cout[Cout_Counter][i] = Read_Col_Buses[i];
          }

          Cout_Counter++;

        }

       if(Cout_Counter==LAPU_Size) {
          fetch_B = true;
          send_C = false;
          Done = 3;
        }
      }


      //Send B to Col Buses
      //At the same time, Store Cout to matrix C
      //
      else if(fetch_B){
      
        if(Done==3){
        
          Done = 0;
          
          for (i=0; i<LAPU_Size;i++)
            for (j=0; j<LAPU_Size; j++){

            //The case when we finish one MC iteration
            if ((Mc==0) && (N!=0) && ((N%Panel_Size)!=0)){
              
              //for every new Mc
              Matrix_C[((Kernel_Size-LAPU_Size)%Kernel_Size)+i +Ap*Kernel_Size]
                [(N-LAPU_Size)+j] =Buffer_Cout[i][j];
              

              if ((((Kernel_Size-LAPU_Size)%Kernel_Size)+i)==(N-LAPU_Size)+j){
              cout << "Mc 0 but N is not, Buffer_Cout[i][j] is " <<  Buffer_Cout[i][j] << endl;
              //getchar();
              }
            }

            //This is the case when we finish one kernel iteration of A
            else if ( Ap!=0 && Mc==0 && N==0){ 

              //When we need to store  the last results of previous kernel
              Matrix_C[((Kernel_Size-LAPU_Size)%Kernel_Size)+i + (Ap-1)*Kernel_Size]
                [((Panel_Size-LAPU_Size)%Panel_Size)+j] =Buffer_Cout[i][j]; 
              //chuui

              if ((((Kernel_Size-LAPU_Size)%Kernel_Size)+i)==((Kernel_Size-LAPU_Size)%Kernel_Size)+j){
              cout << "last 4X4 Buffer_Cout[i][j] is j " << Buffer_Cout[i][j] << endl;
              //getchar();
              }
            
            }
            
            else if (Mc>0){
              //This is for general case 
              Matrix_C[Mc-LAPU_Size+i + Ap*Kernel_Size][N+j] =Buffer_Cout[i][j];
            
            }

            else if (Mc==0 && (N%Kernel_Size)==0 && N!=0){

            //I think this is for the corner case when Panel_Size == Kernel_Size
            Matrix_C[Kernel_Size-LAPU_Size + i + Ap*Kernel_Size][N-LAPU_Size+j] = Buffer_Cout[i][j];

            cout << "Mc 0 but N is not, Buffer_Cout[i][j] is " <<  Buffer_Cout[i][j] << endl;
              //getchar();


            
            }
          }

        }


				if (Bin_Counter< LAPU_Size){
					//cout << "GGGGGGGGGGGGGGG"<<endl;

					cout<<"Sending B"<<endl;
					for (i=0;i<LAPU_Size; i++)
						Write_Col_Buses[i]=Buffer_B[Bin_Counter][i];

					Bin_Counter++;
				}

        if (Bin_Counter ==LAPU_Size){
          fetch_C = false;
          fetch_B = false;
          fetch_A = false;
          send_C = false;
        }

			}
		break;

		case 6: // Mac_Flush
      
      //This is just waiting for PEs to send the last 16 elements (results)
      //The delay from PEs
      Cout_Counter = -2;
      last_x = Kernel_Size - LAPU_Size;
      last_y = Panel_Size - LAPU_Size;

    break;

    case 7: //The end

    //Here, when the PEs are ready, get the last 16 values from the column buses.
    if (Cout_Counter<LAPU_Size){

      if (Cout_Counter>=0){

	  	  for (i=0;i<LAPU_Size;i++)
		  		Matrix_C[last_x+ (NumofKernel-1)*Kernel_Size][last_y+i] = Read_Col_Buses[i];
   
        cout<< "last_x " << last_x <<endl;
        cout<< "Ap and Kernelsize " << Ap  << Kernel_Size <<endl;
        //getchar();

        last_x++;
      }

      Cout_Counter++;

    }

    break;

    

	}


  return 0;


}
