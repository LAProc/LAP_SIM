//============================================================================
// Name        : Simulator.cpp
// Author      : Ardavan Pedram
// Version     :
// Copyright   : All rights reserved for FLAME @ UTEXAS
// Description : Simulator in C++, Ansi-style
//============================================================================



#include <iostream>
#include <iomanip>
#include <fstream>
#include <assert.h>
#include "Parameters.h"
#include "LAPU.h"

using namespace std;



int Global_Gemm( double **& A, double **& B, double **& C);
void print_matrix(int rows, int columns, double ** matrix_A);
void GEMM_Simulation(LAPU * Accelerator);
void dump_matrix_file(int rows, int columns, double ** matrix_A, ofstream& A_file, char type, int precision);
void Read_Matrix(ifstream& input, double **& matrix);
void make_test_matrix(int rows, int columns, double **& test_matrix, int preset, char type);
void Check_Matrix(ifstream& A_input, ifstream& B_input, ifstream& C_input);





int Kernel_Size;
int Panel_Size;
int NumofKernel;
int Mem_Size_A;
int Mem_Size_B;
int Mem_Size_B1;
int Mem_Size_B2;


int Rows_A, Cols_A;
int Rows_B, Cols_B;
int Rows_C, Cols_C;

double ** matmul_example_A;
double ** matmul_example_B;
double ** matmul_example_C;
double ** matmul_example_C_orig;


LAPU * Accelerator;



ofstream C_out_file;
ofstream C_golden_file;
string C_out_name,C_golden_name;

int main(int argc, char* argv[] ) {
	cout << "!!!Linear Algebra Processor Simulator Version 0.1 !!!" << endl; // prints !!!Hello World!!!
	cout << "!!!Current Version: GEMM Rank-K update with a single Linear Algebra Core !!!" << endl;
    cout << "!!!Please read the readme file and watch the video for more details !!!" << endl;
    // prints !!!Hello World!!!

//	std::cout.unsetf ( std::ios::floatfield );                // floatfield not set
//	std::cout.precision(5);
	std::setprecision(4);



	int x=0;

  //char *prgName =NULL;
  //char *inFile = NULL;
  //char *outFile = NULL;

  int numofarg = argc;


  /*prgName   = argv[0];
  inFile = argv[1];
  outFile = argv[2];*/

    if (numofarg!=6)
        {
          printf("File names to retrieve the input matrices: <Matrix A> <Matrix B> <Matrix C_in> <Matrix C_out> <Matrix C_Golden> \n");
          return 0;
        }

    ifstream A_input(argv[1]);
    ifstream B_input(argv[2]);
    ifstream C_input(argv[3]);

    C_out_name=argv[4];
    C_golden_name=argv[5];


    C_out_file.open (argv[4]);

      if (!A_input) {
          cout << "Cannot open matrix A "  <<endl;
          exit(0);
      }



      if (!B_input) {
          cout << "Cannot open matrix B " <<endl;
          exit(0);
      }

      if (!C_input) {
          cout << "Cannot open matrix C " <<endl;
          exit(0);
      }


      if (! (C_out_file.is_open())){
          cout<<"Error, failed to open file"<< argv[3]<<endl;
          return 0;
      }
      
      //Check and Collect Meta Data (Rows and Columns)
      //Will set Kernel_Size, Panel_Size etc if succeeds
      Check_Matrix(A_input, B_input, C_input);

      //Set up the memory size
      Mem_Size_A = (Buffer_local_mem_A * Kernel_Size*Kernel_Size)/(LAPU_Size*LAPU_Size);
      //Two means double buffering
      Mem_Size_B = Kernel_Size * 2;
      


      //Create an accelerator Object
      Accelerator= new LAPU();
      
      /*
      printf("Mem_Size A is %d\n", Mem_Size_A);
      printf("Mem_Size B1 is %d\n", Mem_Size_B1);
      printf("Mem_Size B2 is %d\n", Mem_Size_B2);
      */

      //Read Input files
      Read_Matrix(A_input, matmul_example_A);
      Read_Matrix(B_input, matmul_example_B);
      Read_Matrix(C_input, matmul_example_C);
      
      //Buffer C original
      ifstream C_input_orig(argv[3]);
      Read_Matrix(C_input_orig, matmul_example_C_orig);

      //Triger Simulation
	    GEMM_Simulation(Accelerator);

      //Generate Golden reference
      for(int i = 0; i<Rows_A; i++)
        for(int j=0; j<Cols_B; j++)
          for(int k=0; k<Kernel_Size; k++)
            matmul_example_C_orig[i][j] += matmul_example_A[i][k] * matmul_example_B[k][j];
  
    
    //Put Golden Reference in a file
    C_golden_file.open (argv[5]);
    dump_matrix_file(Rows_A, Panel_Size, matmul_example_C_orig, C_golden_file, 'C',4);

  

  cout<<"Executing command DIFF between ..."<<endl;
  int i=system (("diff "+C_out_name+" "+C_golden_name).c_str());
  cout<<"The value Diff result was:"<<i<<endl;
    
	return 0;
}

void print_matrix(int rows, int columns, double ** matrix_A){

	int i,j;
	//cout.precision(2);

	for (i=0;i<rows;i++){
		for (j=0;j<columns;j++){
			cout<< fixed<< left  <<matrix_A[i][j]<<" , ";
      if ((i+1)%(rows/NumofKernel)==0 && j==columns-1) {
        cout<<endl;
        cout << "********************end of a matrix********************" << endl;
      }
		}
		cout<<endl;

    if (i%4==3) cout << endl;
	}


}




void dump_matrix_file(int rows, int columns, double ** matrix_A, ofstream& A_file, char type, int precision){

	int i,j;

  	A_file.precision(precision);


	A_file<<"Matrix"<<type<< "is below. With dimension"<<rows<<"x"<<columns<<endl;

	for (i=0;i<rows;i++){

		for (j=0;j<columns;j++){
			A_file<< fixed << left<<matrix_A[i][j]/*-matmul_example_C_orig[i][j]*/<<' ';
		}

    A_file<<endl;

	}

	A_file<<endl<<"****************the end****************************"<<endl;
	A_file.close();

}






int Global_Gemm( double **& A, double **& B, double **& C){

	int Global_index=0;
//	int Row_Block_index=0;
//	int Column_Block_index=0;

	Accelerator->Assign_input_Matrix(A,B,C);
	Accelerator->Matmul_Kernel(0);

	return Accelerator->Return_Cycle_Count();
}





void GEMM_Simulation(LAPU * Accelerator){

	int x;

  x=Global_Gemm(matmul_example_A,matmul_example_B,matmul_example_C);

  dump_matrix_file(Rows_A, Panel_Size, matmul_example_C, C_out_file, 'C',4);

	cout<<"end of matmul"<<endl;
	cout<<"Cycles="<<x <<endl;

}





void Check_Matrix(ifstream& A_input, ifstream& B_input, ifstream& C_input){



	A_input.seekg (0, A_input.beg);
	B_input.seekg (0, B_input.beg);
	C_input.seekg (0, C_input.beg);
  
  //Get rows and columns A
  A_input >> Rows_A;
  A_input >> Cols_A;

  printf("Row A is %d\n", Rows_A);
  printf("Col A is %d\n", Cols_A);

  //Get rows and columns B
  B_input >> Rows_B;
  B_input >> Cols_B;
  
  printf("Row B is %d\n", Rows_B);
  printf("Col B is %d\n", Cols_B);

  //Get rows and columns C
  C_input >> Rows_C;
  C_input >> Cols_C;
  
  printf("Row C is %d\n", Rows_C);
  printf("Col C is %d\n", Cols_C);

  //Assertion check
  assert(Cols_A==Rows_B);
  assert(Cols_B==Cols_C);
  assert(Rows_C==Rows_A);

  printf("Pass Assertion \n");

  //Assign Kernel_Size based on meta data
  Kernel_Size = Cols_A;
  Panel_Size = Cols_B;
  NumofKernel = Rows_A/Cols_A;



}


void Read_Matrix(ifstream& input, double **& matrix ){

  //open file pointer
  //ifstream in(type);



  if (!input) {
      cout << "Cannot open matrix "<<endl;
      exit(0);
  }

  int Rows, Cols ;

	input.seekg (0, input.beg);


  //read number of kernel
  input >> Rows;

  //read panel size
  input >> Cols;


  int rows = Rows;
  int columns = Cols;

  //printf("Row is %d\n", Rows);
  //printf("Col is %d\n", Cols);

  matrix=(double **)  malloc ( sizeof (double *) * (rows));

  for ( int i=0; i<rows; i++){
    matrix[i]=(double *)  malloc ( sizeof (double ) * (columns));
    for (int j=0; j<columns; j++){
        input>> matrix[i][j];
        //printf("matrix %d [%d][%d] = %lf\n" ,i, j, matrix[i][j]);
    }

  }


  input.close();
}










