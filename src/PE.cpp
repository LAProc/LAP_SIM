/*
   Copyright (c) 2010-2015, Ardavan Pedram and The University of Texas at Austin
   All rights reserved.
   
   This file is part of LAP_SIM and is under the BSD 3-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-3-Clause
*/

#include "PE.h"
using namespace std;

PE::PE()
{
	// TODO Auto-generated constructor stub

}

PE::PE(int row, int col,  double * row_write_bus, double * row_read_bus, double * col_write_bus, double * col_read_bus)
{


  Address_A_New = 0;
  Address_WA_New = 0;


  Address_B_New = 0;
  Address_WB_New = 0;
  Address_WB_Fetch = Kernel_Size;
  //Address_WB_Fetch = 0;

  toA = 'A';
  toB = 'B';
  
  
  toB1= 'O';
  toB2= 'P';
  
  
  Fill_Lower_Buffer_B = false;
  Fill_Lower_Buffer_A = false;
  fetch_C =true;
  fetch_A =false;
  fetch_B =false;
  send_C = false;


	My_Row= row;
	My_Column= col;

	Scratch_Regs_Curr=(double *) malloc( sizeof(double) * (Scratch_Size));
	Scratch_Regs_Next=(double *) malloc( sizeof(double) * (Scratch_Size));

	Read_My_Col_Bus= col_read_bus;
	Read_My_Row_Bus= row_read_bus;

	Write_My_Col_Bus= col_write_bus;
	Write_My_Row_Bus= row_write_bus;
	Cin_Counter=-1;
	Cout_Counter=0;
	Bin_Counter=-2;
    Ain_Counter=0;
	//cout<< "end of PE const"<<endl;
}

PE::~PE()
{
	// TODO Auto-generated destructor stub
}


int PE::Cycle(){

	int i;
	for (i=0; i<Scratch_Size; i++)
		Scratch_Regs_Curr[i]=Scratch_Regs_Next[i];

	ALU.Cycle();
	//Local_Mem.Cycle();

	Counter_Curr=Counter_Next;
//	Address_Curr=Address_Next;

	Write_My_Col_Reg_Curr=Write_My_Col_Reg_Next;
	Write_My_Row_Reg_Curr=Write_My_Row_Reg_Next;

	return Counter_Curr;

}



int PE::Intialize_Local_Mem( double ** Input_matrix, int row_number, int column_number,int offset){

	Local_Mem.Initialize_Register_File(My_Row, My_Column, Input_matrix,row_number, column_number, offset);
	return 0;

}

int PE::Initialize_Local_Mem_New( double ** Input_matrix, int row_number, int column_number,int offset, char matr){

	Local_Mem.Initialize_Register_File_New(My_Row, My_Column, Input_matrix,row_number, column_number, offset, matr);
	return 0;

}

int PE::Flush_Local_Mem_New( double **& Input_matrix, int row_number, int column_number, int offset, char matr){

	Local_Mem.Flush_Register_File_New(Input_matrix,row_number, column_number, offset, matr);
	return 0;

}

int PE::Flush_Local_Mem( double **& Input_matrix, int row_number, int column_number, int offset){

	Local_Mem.Flush_Register_File(Input_matrix,row_number, column_number, offset);
	return 0;

}


int PE::Return_PE_Power(){


	return ALU.Return_FMA_Power_Consumed();

	// and register file and other stuff +State machine;



}



int PE::Dump_Regs(){

	int i;
	cout<<"PE("<<My_Row<<","<<My_Column<<"):"<<endl;

	for (i=0; i < Scratch_Size ;i++)
		cout << "Scratch["<<i<<"]"<<"="<<Scratch_Regs_Curr[i]<<endl;

	// What I am about to BC
	cout<<"Write_My_Col_Reg_Curr="<<Write_My_Col_Reg_Curr<<endl;
	cout<<"Write_My_Row_Reg_Curr="<<Write_My_Row_Reg_Curr<<endl;

	//What I see
	cout<<"Read_My_Col_Bus="<<*Read_My_Col_Bus<<endl;
	cout<<"Read_My_Row_Bus="<<*Read_My_Row_Bus<<endl;


	cout <<"Local_Mem_Address="<<Local_Mem_Address<<endl;
	cout<<"Accumulator="<<ALU.Return_ACC()<<endl;
	return 0 ;

}



int PE::Dump_ALU_Pipeline(ALU_op operation_type){


	cout<<"PE("<<My_Row<<","<<My_Column<<")"<<endl;
	cout<<"Dumping ALU_Pipeline ..,"<<endl;
	ALU.Dump_Pipeline(operation_type);
	return 0;

	}

//This is recursice function for Address A
//Once it reaches Mem_Size_A, it will go back to 0
int PE::Recurs_Gen_A(int Prev){
  
  if (Prev==0) return 1;
  else return (Recurs_Gen_A(Prev-1) +1)%(Mem_Size_A);
  

}

//Recursive function to generate address in local memory of B
//Will return 0 when it reaches Mem_Size_B

int PE::Recurs_Gen_B(int Prev){
  
  if (!Prev) return 1;
  else return (Recurs_Gen_B(Prev-1) +1)%(Mem_Size_B);
  //else return (Recurs_Gen_B(Prev-1) +1)%(Kernel_Size);

}

int PE::Generate_Address_Signals(int Global_index, int Trsm_index, int Iter_Counter, int Latency_Counter,
								LAPU_Function routine,int LAPU_Current_State, int State_Start){
return 0;

}
//int PE::


//TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
//TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
//TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
//TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO


void PE::Dump_PE_Mem (int amount){


	cout<<"Dumping PE("<<My_Row<<','<<My_Column<<")"<<endl;
	for (int i=0; i<amount; i++)
		cout<<Local_Mem.Reg_Read(i)<<endl;
	cout<<"xxxxxxxxxxxxxx"<<endl;

}




// Load A Row Major  |||||||||
// Load B column major
/*
 *   _
 *   _
 *   _
 *   _
 */
int PE::Execute_Matmul (int Global_index, int Bpj_Counter, int Mc, int Kc, int Ap_Counter, int Matmul_Current_State,int Latency_Counter){

	switch(Matmul_Current_State){



		case 0: //Fetch C in RegFile and ACC

			if (My_Row==(Kc-1) ){ // also K>0 stands
				ALU.Load_Accumulator(*Read_My_Col_Bus);
				Scratch_Regs_Next[1]=*Read_My_Col_Bus;
			}

		break;

		case 1://Fetch first Panel B

			if (Kc==0)
			if (My_Row==(LAPU_Size-1)){
				ALU.Load_Accumulator(*Read_My_Col_Bus);
				Scratch_Regs_Next[1]=*Read_My_Col_Bus;
        Address_WB_New = 0;
			}
				// load the target matrix into the accumulator

			// KC should go from 0 to Kernel_Size + LAPU_Size (For blocks of C)
			if(Kc>=1){ // 1 here is the bus cycles
				
        // old part of address generation
        /*Address_WB=(Kernel_Size*Kernel_Size)/(LAPU_Size*LAPU_Size)+(Kc-1);
				Local_Mem.Reg_Write(Address_WB,(*Read_My_Col_Bus));*/
				
        //new address recursion
        //Address_WB_New = Kc-1;
        //Write B to local mem B
        
        //Remember, B1 and B2 is physcally in a dual-ported memory (local B in the paper)
        //I just separated for convinience of double-buffering implementation

        //Local_Mem.Reg_Write_New(Address_WB_New,(*Read_My_Col_Bus), toB1);
        Local_Mem.Reg_Write_New(Address_WB_New,(*Read_My_Col_Bus), toB);

        Address_WB_New = Recurs_Gen_B(Address_WB_New);
			}


		break;


		case 2://Fetch A


			// Bring the last row of B into SRAM of All PEs
			if ( (Kc==0)&& (Mc==0)){

        //old part of address generation

				/*Address_WB=(Kernel_Size*Kernel_Size)/(LAPU_Size*LAPU_Size)+(Kernel_Size-1);
				Local_Mem.Reg_Write(Address_WB,(*Read_My_Col_Bus));
				Address_WA=0;*/
			
        //new part Mochamad
        //This is the last part of fetching B 
        Local_Mem.Reg_Write_New(Address_WB_New,(*Read_My_Col_Bus), toB);
				
        Address_WA_New = 0; //0 technically
      
      }
				/// All should write it to SRAM

			// KC should go from 0 to Kernel_Size + LAPU_Size (For blocks of C)



			else{ // Here we fetch A


					if (Kc%Kernel_Size==0){
            
            // Basically you fetch per row. 
            // The data will be distributed among PEs

					  if (My_Row==((Mc+LAPU_Size-1)%LAPU_Size)){
              
              //Write to A 
              //Basically the same with old code, except that now we generate recursively
              Local_Mem.Reg_Write_New(Address_WA_New,(*Read_My_Col_Bus), toA); 
              Address_WA_New = Recurs_Gen_A(Address_WA_New);

					  }
					}

					else if (My_Row==Mc%LAPU_Size){

            //Remember Mc is kind of "row" counter.
            //That means now we are fetching the 'Mc'th row of A 

						Local_Mem.Reg_Write_New(Address_WA_New,(*Read_My_Col_Bus), toA);
            Address_WA_New = Recurs_Gen_A(Address_WA_New);

					}
			}




		break;




		case 3:// BC0


			if (My_Row==(LAPU_Size-1)){

      
				//new part Mochamad 
        //This is the last fetch of previous A 
				Local_Mem.Reg_Write_New(Address_WA_New,(*Read_My_Col_Bus), toA);
        Address_WA_New = Recurs_Gen_A(Address_WA_New);
        cout<< "Address_WA_New is" << Address_WA_New << endl;
        //getchar();

			}


			//Address_A=0;

      Address_A_New = 0;

				//2: Prepare to BC the Column 0 and Row 0  (Diagonal PE does the row fetch now)
				//Prepare to BC the 0th column and 0th row  (Diagonal PE does the row fetch now)

			/*if (My_Column==0)*/ //Now I need to put every As of PEs in the row_reg_next, not only based on column
      
      
      {

        //new part Mochamad

        //Write my A value to the row register
        //Because I want to broadcast it to neighbouring PEs via row-bus
        
				Write_My_Row_Reg_Next=Local_Mem.Reg_Read_New(Address_A_New, toA);
			  //Address_A_New++;
        Address_A_New = Recurs_Gen_A(Address_A_New);

			}


			//ALU.Load_Accumulator(0);
		break;

		case 4: // BC

			//Kc=0;

			//Kc Starting to count from now on
			//-> BC is the base: (Kc)th Column of A is on the Bus

			//Prepare to BC the 1st column
			//Drive the row Buses for column  0


			//Read B for iteration 0

			//Driving the bus Column 0 and all rows
			if (My_Column==0)
				*Write_My_Row_Bus=Write_My_Row_Reg_Curr;


			//Read B into Reg_File for iteration 0
			//Address_Reg_B_Next=Address_Reg_B_Curr +1;
			
      Address_B_New= 0;    
      Scratch_Regs_Next[0]=Local_Mem.Reg_Read_New(Address_B_New, toB);

		break;

		case 5: // MAC_and_BC


			// Scratch[0] for B
			//Scratch[1] for Cin
			//Scratch[2] for Cout


			//1-Prepare to BC the Column Iteration_Counter+1
			//2-Drive the Bus for Column Iteration_Counter
			//3-perform MAC on Row Iteration_Counter-1 and  Column Iteration_Counter-1

			////Communication part *****
			// Load the next BC into Bus Registers


      //Ap is a counter of in which kernel of A we are now.
      //Remember, we assume we have more than 1 kernel of A 
      //We do below calculation to decide which address in local mem A we are writing
      //Why ? Because we are doing double buffering for A. We fetch the next kernel while still computing
      //the current kernel
      

      if ((Ap_Counter%2)==0) Fill_Lower_Buffer_A = false; //to decide which buffer pos we are writing to .
                                                  //Remember we do double buffering
                                                  //Decide based on Ma, which is the counter of how many kernel we are computing
                                                  //%2 means, we want to do interleaving
      else Fill_Lower_Buffer_A = true;


      if (!Fill_Lower_Buffer_A){
        

          //Assume double buffer mem A is divided into upper part and lower part.
          //Then we fill the local memory starting from the upper part.
          //If the upper part has been full, then we write the lower part.
          //If the lower part has been full, then that means we go back again to upper part
          //This way, fully partitioned for A is realized

          //If we are in the false mode, that means we already filled the buffer fully 
          //Now, move back to the beginning, and prefetch new kernel 

         if(Bpj_Counter!=0 && Mc==0 && Kc==0){ //repeat for every starting computation of next Kernel_Size*4 of B
           //Remember, we put 1 not zero
           //This is because we already write to zero when Mc=Kernel_Size-LAPU_Size, that means when next iteration 
           //starts, we need to write it to 1
          Address_A_New = 1; 
        }
      }


      else {
        
          //If we are in the true mode, that means we refill the lower part of the buffer
        
       if(Bpj_Counter!=0 && Mc==0 && Kc==0) //repeat for every starting computation of N
        //The same reason as above
        Address_A_New = (Kernel_Size * Kernel_Size)/(LAPU_Size*LAPU_Size) + 1;
      }

      
      if (Kc%LAPU_Size==LAPU_Size-1){

       
        //Here, I need to prepare to drive the row bus
        //However, I have conflicting port of mem A, because we are using it for prefetching
        //Hence, I write and prepare the row_reg only in the 3rd cycle

        Write_My_Row_Reg_Next=Local_Mem.Reg_Read_New(Address_A_New, toA);
			  //Address_A_New++;

        
        //Increment address_A
        Address_A_New = Recurs_Gen_A(Address_A_New);


        //This is for corner case when we are finishing 1 kernel computation
        //I remember I got caught by this corner cases last time
        //For the end of Kc, Mc.


        if ( (Kc== Kernel_Size-LAPU_Size-1) && (Mc == Kernel_Size-LAPU_Size)){


          //This is for the case when we are reaching the end of 1 kernel computation of A
          if (Bpj_Counter==Panel_Size-LAPU_Size)
            //Address_A_New = Address_A_New
            ;


          //Below is the case for end of Kc and Mc, but not the end of N
          //That means we will refer to Fill_Lower_Buffer_A
          //Remember that if Fill_Lower_Buffer_A is false, that means we are writing to memA from the beginning again
          //If it is true, then start from the lower part of double buffer
      
          else Address_A_New= (!Fill_Lower_Buffer_A)? 0:(Kernel_Size * Kernel_Size)/(LAPU_Size*LAPU_Size);

        }

      }

			// Driving Bus for each row
			if (My_Column==(Kc%LAPU_Size))
				*Write_My_Row_Bus=Write_My_Row_Reg_Curr;

			//For All read B to Register[0]
			
      // new part mochamad
      
      //Address_B_New = Kc;

      Address_B_New = Recurs_Gen_B(Address_B_New);
     

      //Logic to manage the interleaving of Memory B
      //Basically it says that if it reaches the end of the first buffer,
      //Then depending on the value of Bpj_Counter, we will decide which address to start with next time
      //It would be interleaved base on the value of Bpj_Counter

      if(Address_B_New==Kernel_Size || Address_B_New==0){
        if (((Bpj_Counter/LAPU_Size)%2)==0 && Address_B_New==Kernel_Size){ 
          Address_B_New = 0;
        }
        else if (((Bpj_Counter/LAPU_Size)%2)==1 && Address_B_New==0)
          Address_B_New = Kernel_Size;
      }


      //This is again the same principle with double-buffering of A
      //We are doing double buffering for B also
      //So, we need to decide which location we will write the prefetched B 
      //There is B1 and B2, They are the same size.
      //So basically if Fill_Lower_Buffer_B is false, then write to the first buffer B1
      //If it is true, then write to the second buffer.
      //They will interleave each other
      //Updated December 13 : We will now use dual-ported local memory of B (before it was single-ported 2 banks )

			Scratch_Regs_Next[0]=Local_Mem.Reg_Read_New(Address_B_New, toB)	;


			//EXECUTION


			//TODO does it cover all?
			// reloading Accumulator
			// getting the old result
			// doing MAC
			if (My_Row==0 && My_Column==0){
				cout<<"MAC_IN_A"<<(*Read_My_Row_Bus)<<endl;
				cout<<"MAC_IN_B"<<Scratch_Regs_Curr[0]<<endl;
			}

      
			if ((Kc==FMA_Latency-1)&&(Mc!=0 ||  Bpj_Counter!=0 || Ap_Counter!=0)){ 
      
        //This is when the data is ready from the MAC
        //Write to col reg to drive the bus in the next cycle
        
        Scratch_Regs_Next[3]=ALU.Execute_MAC( Scratch_Regs_Curr[0], (*Read_My_Row_Bus));
				Write_My_Col_Reg_Next=Scratch_Regs_Next[3];
        
			}
			else if (Kc==0){ // Loading accumulator takes a cycle, and we start new panel with kc=1;
				//cout<<"LOADING ACC"<<Scratch_Regs_Curr[1]<<endl;
				ALU.Execute_MAC( Scratch_Regs_Curr[0], (*Read_My_Row_Bus));
				cout <<"LOADIN ACC WITH"<<Scratch_Regs_Curr[1]<<endl;
				ALU.Load_Accumulator(Scratch_Regs_Curr[1]);
        
        //if (My_Row==0 && My_Column==0) getchar();

			}

			else
      { 
        //Keep computing
        ALU.Execute_MAC( Scratch_Regs_Curr[0], (*Read_My_Row_Bus));
        
      }


      // If the last panel of B, then fetch new A


			//Fetch Logic

			// TODO dont forget to reload accumulator; after KC is reset
			// TODO  dont forget to put the result of acc on the column bus write;
				//Fetch Next C

				// Fetch Next B

				// Send C out


      //initialize fetch logic
      if (Kc==0){

				Cin_Counter=-1;
				Bin_Counter=-2;
				Cout_Counter=0;
        Ain_Counter = 0;

        fetch_C = true;
        fetch_B = false;
        fetch_A = false;
        send_C = false;


			}


      //Fetch new C on the first 5 cycles
      

				if ((Cin_Counter< LAPU_Size) && fetch_C){
					//cout<<"COUNTER="<<Cin_Counter<<endl;
					if (Cin_Counter>=0)
						if (My_Row== Cin_Counter){
							Scratch_Regs_Next[1]=*Read_My_Col_Bus;
							cout<<"SAVING CIN"<<*Read_My_Col_Bus<<endl;
							//Read the bus to Regfile
						}

					Cin_Counter++;
          
          if (Cin_Counter == LAPU_Size) {

            //if done fetching Cin, then go fetch_A
            fetch_C = false;
            fetch_A = true;     //next fetch_A
            cout<<"end of fetch_C "<<endl;
          }
				}

        //fetching A

        else if (fetch_A){
							

            if (Ain_Counter < LAPU_Size){

            {
           
              //here, we can only do at Kc%LAPU_Size!=3 
              //Why ? Because at Kc%LAPU_Size==3, I will use local mem A to write to my rowreg
              //Basically Port conflict 
              
             if(Ain_Counter>=0 && (Kc%LAPU_Size!=3)){
                
               //This is basically saying that the number of Aip we fetch will go together with N_counter
               //Why? because for every one iteration of Kc, we fetch LAPU_Size * LAPU_Size elements, fetching greedyly 
               //That means, for one N iteration we fetch LAPU_Size * LAPU_Size * Mc/Kernel_Size
               //Thus, the number of Bpj_Counter would be equal to Kernel_Size when we fetch all of A
               
               if (Bpj_Counter<Kernel_Size){
                if(My_Row == Ain_Counter){
                 Local_Mem.Reg_Write_New(Address_WA_New,(*Read_My_Col_Bus), toA); 
                  Address_WA_New = Recurs_Gen_A(Address_WA_New); 
                }
              }
               
              Ain_Counter++; 
              cout<<"Ain_Counter is  "<< Ain_Counter <<endl;
             }
            }

            /*else {
              Ain_Counter++;

            }*/

            if (Ain_Counter == LAPU_Size) {
                //if done, then next is Sending C
                send_C = true;
                Ain_Counter =0;
                fetch_A = false;
              }

            }
        }


        else if (send_C){   //send C_out to IO

				  
          if (Cout_Counter<LAPU_Size){
					  //cout<<"Sending Cout"<<endl;
					  if (My_Row==Cout_Counter){
						  *Write_My_Col_Bus=Write_My_Col_Reg_Curr; //drive the bus
						  //*Write_My_Col_Bus=Scratch_Regs_Curr[2];
              cout<<"send_C now "<<endl;
              cout<<"My_Col_Bus  "<< *Write_My_Col_Bus << endl;
            }
					  Cout_Counter++;
				    }
         
          //If finished, then next is to fetch B
          send_C = (Cout_Counter==LAPU_Size)? false: true;           
          fetch_B = (Cout_Counter ==LAPU_Size)? true : false;
        }


      else if (fetch_B) {

        //Fetch B Now

				if (Bin_Counter< LAPU_Size){

          cout<<"Fetch_B now "<<endl;

					if (Bin_Counter>=0){

            //Here, we need to decide which buffer of local memory B we need to write
            //Since we have hint of Fill_Lower_Buffer_B, then we can know the correct position
            //if Fill_Lower_Buffer_B is false, that means write to the lower part 
            //If true, then write to the beginning of the buffer

            Local_Mem.Reg_Write_New(Address_WB_Fetch,(*Read_My_Col_Bus), toB);
            
            //Local_Mem.Reg_Write_New(Address_WB_Fetch,(*Read_My_Col_Bus), toB);
            
            Address_WB_Fetch = Recurs_Gen_B(Address_WB_Fetch);
            //printf("Address WB Fetch is %d\n", Address_WB_Fetch);
            //exit(0);

					}

					Bin_Counter++;
				}

			}

		break;

		case 6: // MAC_Flush
			// Nothing left to do;
      // Flushing the MAC

      //This is for the last 16 results. Waiting for the MAC to flush
			if (Latency_Counter<(FMA_Latency-1) )
				ALU.Execute_MAC(Scratch_Regs_Curr[0], (*Read_My_Row_Bus));


			else{

					Scratch_Regs_Next[2]=ALU.Execute_MAC(Scratch_Regs_Curr[0], (*Read_My_Row_Bus));
          //updated by Mochamad  Execution
				  Write_My_Col_Reg_Next=Scratch_Regs_Next[2];
          Cout_Counter = -1;// set bus delay
			}



		break;

		case 7: // End

      //MAC results are ready 
      //Now send it back to IO
      if (Cout_Counter<LAPU_Size){
        
			  if(Cout_Counter>=0){

          if (My_Row==Cout_Counter)
	  					*Write_My_Col_Bus=Write_My_Col_Reg_Curr; //drive the bus
		  				//*Write_My_Col_Bus=Scratch_Regs_Curr[2];
          }
			  Cout_Counter++;
      }

			cout<< "PE("<<My_Row<<","<<My_Column<<")="<<Scratch_Regs_Curr[2]<<endl  ;// Write the matrix back into the Local_Store

		break;


	}

	return 0;

}


