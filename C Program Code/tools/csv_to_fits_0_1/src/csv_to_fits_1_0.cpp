//============================================================================
// Name        : fits_to_csv_1_0.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

// Required header files
#include <stdio.h>
#include <stdlib.h>

//required for FITS file operations
#include <fitsio.h>

//Allows C++ style i/o
#include <iostream>

//allows for C++ string operations
#include <string.h>

//includes user-defined error checking routine
#include "error_checking_5_1.h"

//includes the file name info header
#include "file_info_5_0.h"

//includes the structure definitions
#include "struct_definitions_7_0_analysis.h"

//includes the flags definitions
#include "flags_definitions_5_0.h"

using namespace std;

int check_arguments (int num_args, 
					char *arg_one, 
					char *p_file_type);

int check_file_type(	char *file_name, 
						char *p_file_type);

int read_file (char *file_name);

int print_table_entry (fitsfile *p_fits, 
						int column_index, 
						int row_index, 
						int typecode, 
						int repeat);

int main(int argc, char *argv[]) {

	int error = ERR_NO_ERROR;
	char file_type = '0';
	char *p_file_type = &file_type;
	
	
	error = check_arguments (argc,
							argv[1],
							p_file_type);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	if (*p_file_type=='t')
	{
		//text_loop (argv[1]);
	}
	
	else if (*p_file_type ='f')
	{
		read_file (argv[1]);
	}
	
	
	
	return error;
	
	
}

int check_arguments (int num_args, 
					char *arg_one, 
					char *p_file_type)
{
	
	int error = ERR_NO_ERROR;
		

		//if no arguments have been entered, ask the user to enter a filename
		if (num_args == 1)
		{
			printf ("\nYou have not entered a valid file name.");

			printf ("\nPlease Enter the path of the file you wish to convert.");
			
			arg_one = (char *)(malloc(FILE_NAME_SIZE*sizeof(char)));
			
			scanf("%s",arg_one);

				error = check_arguments(2,//the above scanf statement must have acquired an argument
										arg_one,
										p_file_type);
				error = error_checking (error);
				if (error != ERR_NO_ERROR)
				{
					return (error);
				}

		}
		
		//if arguments have been entered, check they are valid
		else if (num_args == 2)
		{
			//printf("\nThe file you have selected is %s", arg_one);
			//if it is not a valid file name, attempt to request a new filename
			check_file_type (arg_one, p_file_type);
			
			if (*p_file_type =='f')
			{
				return (ERR_NO_ERROR);
			}
			
			if (*p_file_type =='t')
			{
				return (ERR_NO_ERROR);
			}
			
			else
			{
				printf ("\nThat file is not a valid file for conversion! Please try again!");
				*p_file_type = 'o';	
				printf ("\nPlease Enter the path of the file you wish to convert.");
							
				arg_one = (char *)malloc(FILE_NAME_SIZE*sizeof(char));
				
				scanf("%s",arg_one);

				error = check_arguments(2,//the above scanf statement must have acquired an argument
										arg_one,
										p_file_type);
				error = error_checking (error);
				if (error != ERR_NO_ERROR)
				{
					return (error);
				}
			}

		}
		//empty else cale for future use
		else
		{
			return (ERR_TOO_MANY_ARGS);
		}
		return (ERR_NO_ERROR);
	
}

int check_file_type(	char *file_name, 
						char *p_file_type)
{
	int error = ERR_NO_ERROR; 
	//creates a pointer to a string to be used to check the 
	//filename for output purposes
	char *fit_extension = (char *) malloc (10*sizeof(char));
	fit_extension = ".fit";
	char *fits_extension = (char *) malloc (10*sizeof(char));
	fits_extension = ".fits";
	char *ppr_extension = (char *) malloc (10*sizeof(char));
	ppr_extension = ".ppr";
	char *prm_extension = (char *) malloc (10*sizeof(char));
	prm_extension = ".prm";
	char *txt_extension = (char *) malloc (10*sizeof(char));
	txt_extension = ".txt";
	char *dot = (char *) malloc (10* sizeof (char));
	dot = "dot";
	
	if ((dot = strchr (&file_name[0], '.'))== NULL)
	{
		*p_file_type = 'o';
	}
	else if (!strncmp (dot, fit_extension, 5))
	{
		*p_file_type = 'f';
	}
	else if (!strncmp (dot, fits_extension, 5))
	{
		*p_file_type = 'f';
	}
	else if (!strncmp (dot, ppr_extension, 5))
	{
		*p_file_type = 'p';
	}
	else if(!strncmp (dot, prm_extension, 5))
	{
		*p_file_type = 'a';
	}
	else if(!strncmp (dot, txt_extension, 5))
	{
		*p_file_type = 't';
	}
	else
	{
		*p_file_type = 'o';
	}
	
	//free (fit_extension);
	//free (fits_extension);
	//free (prm_extension);
	//free (ppr_extension);
	//free (txt_extension);
	
	return (error);
}

int read_file (char *file_name)
{
	//creates file pointers and error reporting controls
	fitsfile *p_fits;
	int status = 0;
	int *p_status = &status;
	int error = ERR_NO_ERROR;
	
	//these control the rows and columns in the read-in file, and will be used for
	//checking each row in turn
	long int n_rows = 0;
	int n_columns = 0;
	long int *p_n_rows = &n_rows;
	int *p_n_columns = &n_columns;
	
	//creates variables for temproaily storing the data about column type and width
	int typecode = 0;
	long int repeat = 0;
	long int width = 0;

	
	//creates a string for temporarily storing the column names
	char col_name[NAME_SIZE] = "               ";
	char col_template = '*';
	
	
	//creates an index variable for the columns and rows
	int column_index = 0;
	int row_index = 0;
	int repeat_index = 0;

	int col_holder = 0;
	int *p_col_holder = &col_holder;
		
	//opens the FITS file  
	fits_open_table(&p_fits, file_name, READONLY, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	
	
	//Gets the number of rows and Columns in the fits File previously opened
	fits_get_num_rows(p_fits, p_n_rows, p_status);
	if (*p_status!=0)
	{
		return(*p_status);
	}
	
	
	fits_get_num_cols(p_fits, p_n_columns, p_status);
	if (*p_status!=0)
	{
		return(*p_status);
	}
	
	
	printf("\nNo.,");
	
	//get the column name, number and details for each of the columns 
	for(column_index = 1; column_index < (n_columns +1) ; column_index++ )
	{
		//printf("%d,", column_index);
		//col_holder = column_index;
		//p_col_holder = &col_holder;
		*p_status = COL_NOT_UNIQUE;
		//gets the column name, then writes it to the screen and files
		fits_get_colname (p_fits, CASEINSEN, &col_template, &col_name[0], p_col_holder, p_status);
		if (*p_status!=COL_NOT_UNIQUE)
		{
			return(*p_status);
		}
		
		*p_status = 0;
				
		//gets the detailed information about each column, and prints those to the screen.
		fits_get_coltype(p_fits, column_index, &typecode, &repeat, &width, p_status);

		if (repeat > 1)
		{
			for(repeat_index = 0; repeat_index < repeat ; repeat_index++ )
			{
				printf("%s[%d],", &col_name[0], repeat_index);
			}
		}
		else
		{
			printf("%s,", &col_name[0]);
		}


	}
	
	for(row_index = 1; row_index < (*p_n_rows +1) ; row_index++ )
	{
		printf("\n%d,", row_index);
		for(column_index = 1; column_index < (*p_n_columns +1) ; column_index++ )
		{
					
			//gets the detailed information about each column, and prints those to the screen.
			fits_get_coltype(p_fits, column_index, &typecode, &repeat, &width, p_status);

			error = print_table_entry (p_fits, column_index, row_index, typecode, repeat);
			error = error_checking (error);
			if (error != ERR_NO_ERROR)
			{
				return (error);
			}
		}

	
	}
	
	fits_close_file(p_fits, p_status);
	
	
	return (error);
}

int print_table_entry (fitsfile *p_fits, 
						int column_index, 
						int row_index, 
						int typecode, 
						int repeat)
{
	int error = ERR_NO_ERROR;
	
	
	//creates placeholders for various possible datatypes
	float float_holder = 0;
	int int_holder = 0;
	long long_holder = 0;
	char char_holder = 0;
	double double_holder = 0;
	int status = 0;
	int *p_status = & status;
	

	long int repeat_index = 0;
	
	if (typecode == TFLOAT)
	{
		for(repeat_index = 1; repeat_index < (repeat+1) ; repeat_index++ )
		{ 
			fits_read_col (p_fits, typecode, column_index, row_index, repeat_index, 1, 0, &float_holder, 0, p_status);
			printf("%f,", float_holder);
			
		}
	}
	
	else if (typecode == TINT)
	{
		for(repeat_index = 1; repeat_index < (repeat+1) ; repeat_index++ )
		{ 
			fits_read_col (p_fits, typecode, column_index, row_index, repeat_index, 1, 0, &int_holder, 0, p_status);
			printf("%d,", int_holder);
			
		}
	}
	
	else if (typecode == TLONG)
	{
		for(repeat_index = 1; repeat_index < (repeat+1) ; repeat_index++ )
		{ 
			fits_read_col (p_fits, typecode, column_index, row_index, repeat_index, 1, 0, &long_holder, 0, p_status);
			printf("%ld,", long_holder);
			
		}
	}
	
	else if (typecode == TBYTE)
	{
		for(repeat_index = 1; repeat_index < (repeat+1) ; repeat_index++ )
		{ 
			fits_read_col (p_fits, typecode, column_index, row_index, repeat_index, 1, 0, &char_holder, 0, p_status);
			printf("%c,", char_holder);
			
		}
	}
	else if (typecode == TDOUBLE)
	{
		for(repeat_index = 1; repeat_index < (repeat+1) ; repeat_index++ )
		{ 
			fits_read_col (p_fits, typecode, column_index, row_index, repeat_index, 1, 0, &double_holder, 0, p_status);
			printf("%lf,", double_holder);
			
		}
	}
	else
	{
		for(repeat_index = 1; repeat_index < (repeat+1) ; repeat_index++ )
		{ 
			printf("ERROR,");
			
		}
	}
	
	
	return (error);
}


