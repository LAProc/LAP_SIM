/*
 * Reg_File.cpp
 *
 *  Created on: Mar 6, 2010
 *      Author: ardavan
 */


#include "Reg_File.h"
using namespace std;

Reg_File::Reg_File()
{
	// TODO Auto-generated constructor stub
	Size=Mem_Size;  // is this dummy code ?

  //Local Memory A and B
	Registers_A= (double *) malloc ( sizeof(double)* (Mem_Size_A));	
	Registers_B= (double *) malloc ( sizeof(double)* (Mem_Size_B) );


}

Reg_File::~Reg_File()
{
	// TODO Auto-generated destructor stub


}


double Reg_File::Reg_Read(int address){

	//cout<<"*********************"<<endl;
	//return Registers[address];
	return 0;
}

double Reg_File::Reg_Read_New(int address, char mem){

  if (mem=='A'){
  	return Registers_A[address];
  }

  if (mem=='B'){
    return Registers_B[address];
  }

  cout<< "invalid address"<<endl;
  return 0;// TODO
}

int Reg_File::Reg_Write_New(int address, double data, char mem){


  if (mem=='A'){
    Registers_A[address]=data;
  }


  if (mem=='B'){
    Registers_B[address]=data;
  }

  cout<< "invalid address"<<endl;
  return 0;
}

int Reg_File::Reg_Write(int address, double data){

	//cout<<"*********************"<<endl;
	//cout<<"writing "<<data<<"in "<<address<<endl;
	//Registers[address]=data;
	  
  	return 0;

}

/*
int Reg_File::Cycle(){


	int i;
	for (i=0; i < Size; i++)
		Registers_Curr[i]=Registers_Next[i];

}
*/
int Reg_File::Initialize_Register_File (int row, int column, double ** A, int row_number, int column_number, int offset){

	//TODO
	return 0;
}

int Reg_File::Initialize_Register_File_New (int row, int column, double ** A, int row_number, int column_number, int offset, char matr){

	// Storing the matrix in the Register file column order

	 int i, j;
	 int l=offset;
	// instead of using the constructor I intialize my_row and my_column here.
	my_row=row;
	my_column=column;

  if(matr=='A'){

  	for (j= my_column; j< column_number; j=j+LAPU_Size)
	  	for (i=my_row; i< row_number; i=i+LAPU_Size){
		  	Registers_A[l]=A[i][j];
			  l++;
			  if (l>Mem_Size_A) cout << "Erorr: Memory overload"<<endl;
		  }

    }
  return 0;
}

int Reg_File::Flush_Register_File ( double **& B, int row_number, int column_number,int offset){

	// Storing the matrix in the Register file column order

	 int i, j;
	 int l=offset;

	// cout<< "flushing register file"<<endl;
	// cout<<"l="<<l<<endl;
	 for (i=my_row; i< row_number; i=i+LAPU_Size)
	for (j= my_column; j< column_number; j=j+LAPU_Size)
		{
			//cout<<"l="<<l<<endl;
			//cout<<Registers[++l]<<endl;
			B[i][j]=Registers_B[l] ;
			l++;
			if (l>Mem_Size) cout << "Error: Memory Underload"<<endl;
		}
	return 0;
}

int Reg_File::Flush_Register_File_New ( double **& B, int row_number, int column_number,int offset, char matr){

	// Storing the matrix in the Register file column order

	 int i, j;
	 int l=offset;

	// cout<< "flushing register file"<<endl;
	// cout<<"l="<<l<<endl;
	
   if(matr=='A'){
   
   for (i=my_row; i< row_number; i=i+LAPU_Size)
  	for (j= my_column; j< column_number; j=j+LAPU_Size)
		{
			//cout<<"l="<<l<<endl;
			//cout<<Registers[++l]<<endl;
			B[i][j]=Registers_A[l] ;
			l++;
			if (l>Mem_Size_A) cout << "Error: Memory Underload"<<endl;
		}
   }
   
   if(matr=='B'){
   
   for (i=my_row; i< row_number; i=i+LAPU_Size)
  	for (j= my_column; j< column_number; j=j+LAPU_Size)
		{
			//cout<<"l="<<l<<endl;
			//cout<<Registers[++l]<<endl;
			B[i][j]=Registers_B[l] ;
			l++;
			if (l>Mem_Size_B) cout << "Error: Memory Underload"<<endl;
		}
   }
   return 0;
}
