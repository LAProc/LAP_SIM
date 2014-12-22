/*
   Copyright (c) 2010-2015, Ardavan Pedram and The University of Texas at Austin
   All rights reserved.
   
   This file is part of LAP_SIM and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/

   
#include "Parameters.h"
#ifndef REG_FILE_H_
#define REG_FILE_H_

class Reg_File
{
public:
	Reg_File(int row, int column);
	Reg_File();
	virtual ~Reg_File();

	double Reg_Read(int address);
	int Reg_Write (int address, double data);
	
  double Reg_Read_New(int address, char mem);
	int Reg_Write_New (int address, double data, char mem);
	
  int Initialize_Register_File (int row, int column, double ** A, int row_number, int column_number, int offset);

  int Initialize_Register_File_New (int row, int column, double ** A, int row_number, int column_number, int offset, char matr);
		//TODO update so it can append new matrices to the register file
	int Flush_Register_File(double **& B, int row_number, int column_number, int offset);
	int Flush_Register_File_New(double **& B, int row_number, int column_number, int offset, char matr);
// 	int Cycle();

private:

	double * Registers;
	// no next no current, just one copy of the whole register file
	int Size;
	int my_row;
	int my_column;

  //added by Mochamad
  
  double *Registers_A, *Registers_B, *Registers_B1, *Registers_B2;

};

#endif /* REG_FILE_H_ */
