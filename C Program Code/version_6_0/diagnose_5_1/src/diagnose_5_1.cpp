
/*
 ============================================================================
 Name        : diagnose_4_1.cpp
 Author      : Oisin Creaner
 Date	     : 23rd June 2009
 Copyright   : (c) Oisin Creaner, ITT Dublin 2007-2009
 Description : Diagnosis of SDSS catalogue format
 ============================================================================
 */

//the Diagnosis of SDSS program is written to examine a SDSS-supplied 
//fits file, extract certain relevant points of information and store 
//these in a standard C-readable binary file.  A full software design 
//document is available on the P: drive.

//This program takes a FITS file as a input and creates an output to 
//both the screen and a binary file.  This file will be stored in a 
//new format currently designated .cti (catalogue info)

//version 3 is equipped to deal with user-input filename parameters

//This program is part of the SDSS Access API



// Copyright Notices

// Large ASCII Fonts used for headers are in alphabet font
// from figlet.org.  
// Alphabet font (c) Wendell Hicken 1993
// Figlet (c) Geln Campbell and Ian Chai 1991, 1993 & 1994
// Figlet HTML Front End (c) Jorg Seyfferth 1996-2008


//  H  H EEEE  AA  DDD  EEEE RRRR  
//  H  H E    A  A D  D E    R   R 
//  HHHH EEE  AAAA D  D EEE  RRRR  
//  H  H E    A  A D  D E    R R   
//  H  H EEEE A  A DDD  EEEE R  RR 

//  ******* BASIC HEADER FILES ********

// Required header files

// Required header files
#include <stdio.h>
#include <stdlib.h>

//required for FITS file operations
#include <fitsio.h>

//Allows C++ style i/o
#include <iostream>

//allows for C++ string operations, (Currently disabled)
#include <string.h>

//includes the error reporting header file
#include "error_checking_5_1.h"

//includes the file name definitions file
#include "file_info_5_0.h"

//includes the definitions of the structures
#include "struct_definitions_1_0.h"

using namespace std;







// FFFF U   U N   N  CCC TTTTTT III  OOO  N   N 
// F    U   U NN  N C      TT    I  O   O NN  N 
// FFF  U   U N N N C      TT    I  O   O N N N 
// F    U   U N  NN C      TT    I  O   O N  NN 
// F     UUU  N   N  CCC   TT   III  OOO  N   N 

//  ********** FUNCTION DEFINITIONS***********

//checks the validity of argc and argv
int check_arguments (int num_args, 
					char *arg_one, 
					FILE **p_fp_parameters,
					char *p_file_type);

//prompts the user to input the filename if none is supplied or 
//the one supplied is invalid 
int name_parameter_file (	FILE **p_fp_parameters, 
							char *p_file_type);


//checks the extension of a supplied file against those on a list.
int check_file_type(	char *file_name, 
						char *p_file_type);

//finds and opens the parameters file and finds out what size it is
int get_parameters_file (FILE ** p_fp_parameters, int *p_num_files);

//gets the file names needed at later stage
int get_file_name(char * file_name, FILE ** p_fp_parameters);

//makes a .cti filename out of the .fit file name allocated
int make_cti_file_name(char *file_name, char *cti_file_name);

//This is a control function that calls all of the other 
// functions that diagnose an SDSS entry
int diagnose(char * file_name, char *cti_file_name);

// This function is called by diagnose and downloads the details 
// of the columns from the file and stores them in an array
int get_details(struct cti_entry *cti_info, int *p_n_columns, fitsfile *sdss_ptr);












// M   M  AA  III N   N 
// MM MM A  A  I  NN  N 
// M M M AAAA  I  N N N 
// M   M A  A  I  N  NN 
// M   M A  A III N   N

int main(int argc, char *argv[]) 
{
	// creates an error-haldling parameters
	int error = ERR_NO_ERROR;
	
	//creates a variable for extracting the information of file names
	char *file_name = (char *) malloc (FILE_NAME_SIZE * sizeof(char));
	char *cti_file_name = (char *) malloc (FILE_NAME_SIZE * sizeof(char));
	
	//creates a pointer for a file in which to store the information
	//provided by the user
	FILE *fp_parameters;
	FILE **p_fp_parameters = & fp_parameters;
	
	//creates a variable for the number of files and 
	//a corresponding pointer
	int num_files =0;
	int *p_num_files = & num_files;
	int file_index = 0;
	
	char file_type = 'o';
	char *p_file_type = &file_type;
	
	//creates a variable to handle a count of missing files
	long int missing_file_count = 0;
	
	error = check_arguments (	argc, 
								argv[1], 
								p_fp_parameters, 
								p_file_type);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	
	error = get_parameters_file ( p_fp_parameters, p_num_files);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	for(file_index = 0; file_index < num_files; file_index ++)
	{
		error=ERR_NO_ERROR;
		//get the next file name
		error = get_file_name(file_name, p_fp_parameters);
		
		error= error_checking (error);
		if (error != ERR_NO_ERROR)
		{
			return (error);
		}
		
		error = make_cti_file_name(file_name, cti_file_name);
			
		error= error_checking (error);
		if (error != ERR_NO_ERROR)
		{
			return (error);
		}
		
		//diagnose the details of the file
		error = diagnose(file_name, cti_file_name);	

		//this allows the code to skip over missing files without exiting
		error= error_checking (error);
		if (error != ERR_FITS_ERROR)
		{	
			if (error != ERR_NO_ERROR)
			{
				return (error);
			}
		}
		else
		{
			error= ERR_NO_ERROR;
			missing_file_count++;
			printf("\n%ld out of %d files are missing so far." , missing_file_count, (file_index+1));
		}
	}

	
	free(file_name);
	free (cti_file_name);
	if(missing_file_count > 0)
	{
		printf("\nWARNING: SOME FILES WERE NOT AVAILABLE");
		return (ERR_FITS_ERROR);
	}
	return (error);
}


int check_arguments (int num_args, 
					char *arg_one, 
					FILE **p_fp_parameters,
					char *p_file_type)
{
	int error = ERR_NO_ERROR;
	

	//if no arguments have been entered, ask the user to enter a filename
	if (num_args == 1)
	{
		printf ("\nYou have not entered a valid file name.");

			error = name_parameter_file (p_fp_parameters, p_file_type);
			error = error_checking (error);
			if (error != ERR_NO_ERROR)
			{
				return (error);
			}

	}
	//if an argument has been entered, attempt to use it.
	else if (num_args == 2)
	{
		printf("\nThe file you have selected is %s", arg_one);
		//if it is not a valid file name, attempt to request a new filename
		check_file_type (arg_one, p_file_type);

		if ((*p_file_type =='a'))
		{
			if ((*p_fp_parameters = fopen(arg_one, "rb"))==NULL)
				{
					printf ("\nThat is not a valid selection! Please try again!");
					error = name_parameter_file (p_fp_parameters, p_file_type);
					error = error_checking (error);
					if (error != ERR_NO_ERROR)
					{
						return (error);
					}	
				}
		}	
		else //(num_args >2)
		{
			error = name_parameter_file (p_fp_parameters, p_file_type);
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

int name_parameter_file (	FILE **p_fp_parameters, 
							char *p_file_type)
{
	int error = ERR_NO_ERROR;
	
	//creates a variable for the filetype choosen
	char file_name [FILE_NAME_SIZE] = "0";
		


	
	//Asks the user for the name of the file to be created
	do
	{
		printf ("\nPlease Enter the full path of the parameter file you wish to use\t");
		scanf ("%s", &file_name [0]);
		
		check_file_type (file_name, p_file_type);

		if (*p_file_type =='a')
		{
			if ((*p_fp_parameters = fopen(&file_name[0], "rb"))==NULL)
			{
				printf ("\nThere was an error opening that file! Please try again!");
				*p_file_type = 'o';		
			}
			
			else
			{
				printf("\n\nFile accepted.\n file path is\t %s", file_name);
			}
		}
		else
		{
			printf ("\nThat file is not a valid parameter file! Please try again!");
			*p_file_type = 'o';	
		}
	}
	while (!(*p_file_type =='a'));
	
	return (error);
}


int check_file_type(	char *file_name, 
						char *p_file_type)
{
	int error = ERR_NO_ERROR; 
	//creates a pointer to a string to be used to check the 
	//filename for output purposes
	char *fit_extension ;//= (char *) malloc (10*sizeof(char));
	fit_extension = ".fit";
	char *fits_extension;// = (char *) malloc (10*sizeof(char));
	fits_extension = ".fits";
	char *ppr_extension;// = (char *) malloc (10*sizeof(char));
	ppr_extension = ".ppr";
	char *prm_extension;// = (char *) malloc (10*sizeof(char));
	prm_extension = ".prm";
	char *csv_extension;// = (char *) malloc (10*sizeof(char));
	csv_extension = ".csv";
	char *dot;// = (char *) malloc (10* sizeof (char));
	dot = "dot";
	
	if ((dot = strchr (&file_name[0], '.'))== NULL)
	{
		printf("\nthe file %s is of unknown file type", file_name);
		*p_file_type = 'o';
	}
	else if (!strncmp (dot, fit_extension, 5))
	{
		printf("\nthe file %s is of file type %s", file_name, fit_extension);
		*p_file_type = 'f';
	}
	else if (!strncmp (dot, fits_extension, 5))
	{
		printf("\nthe file %s is of file type %s", file_name, fits_extension);
		*p_file_type = 'f';
	}
	else if (!strncmp (dot, ppr_extension, 5))
	{
		printf("\nthe file %s is of file type %s", file_name, ppr_extension);
		*p_file_type = 'p';
	}
	else if(!strncmp (dot, prm_extension, 5))
	{
		printf("\nthe file %s is of file type %s", file_name, prm_extension);
		*p_file_type = 'a';
	}
	else if(!strncmp (dot, csv_extension, 5))
	{
		printf("\nthe file %s is of file type %s", file_name, csv_extension);
		*p_file_type = 'c';
	}
	else
	{
		printf("\nthe file %s is of unknown file type", file_name);
		*p_file_type = 'o';
	}
	
	//cleans up memory leaks
	//free(fit_extension);
	
	//free(fits_extension);
	
	//free(ppr_extension);
	
	//free(prm_extension);
	
	//free(csv_extension);
	
	//free(dot);
	
	return (error);
}



int get_parameters_file (FILE ** p_fp_parameters, int *p_num_files)
{
	int error = ERR_NO_ERROR;

	
	//reads in the number of files as parameters 
	if (fread (p_num_files, sizeof(long int), 1, *p_fp_parameters) != 1)
	{
		if(feof(*p_fp_parameters))
		{
			return (ERR_END_OF_FILE);
		}
		else
		{
			return (ERR_FILE_READ);
		}
	}
	
	return (error);
}


//gets the file names needed at later stage
int get_file_name(char * file_name, FILE ** p_fp_parameters)
{
	int error = ERR_NO_ERROR;
	int name_index = 0;
	for (name_index = 0; name_index < FILE_NAME_SIZE; name_index ++)
	{
		if (fread ((file_name +name_index), sizeof (char), 1, * p_fp_parameters) != 1)
		{
			return (ERR_FILE_READ);
		}
	}
	
	return (error);
}

int make_cti_file_name(char *file_name, char *cti_file_name)
{
	int error = ERR_NO_ERROR;
	int name_index = 0;
	
	//creates a pointer to a string to be used to extend the 
	//filename for output purposes
	char *output_extension = (char *) malloc (5*sizeof(char));
	output_extension = ".cti";
	char *dot = (char *) malloc (2* sizeof (char));
	
	for (name_index = 0; name_index < FILE_NAME_SIZE; name_index ++)
	{
		*(cti_file_name + name_index) = *(file_name + name_index);
	}
	//finds the "dot" in the file name so the 
	//filename can be changed for output
	dot =strchr (cti_file_name, '.');
	*dot = '\0';
	
	//puts the .cti extension on the filename for output purposes
	strcat (cti_file_name, output_extension);

	//free (output_extension);
	//free (dot);
	
	return (error);
	
}

int diagnose(char *file_name, char *cti_file_name)
{
	// creates an error control variable
	int error = ERR_NO_ERROR;
	
	
	//status is used by fitsio routines for error handling
	int status = 0;
	int *p_status = &status;

	//these control the rows and columns in the read-in file, and will be used for
	//checking each row in turn
	long int n_rows = 0;
	int n_columns = 0;
	long int *p_n_rows = &n_rows;
	int *p_n_columns = &n_columns;
	
	
	
	
	// creates a pointer to a CTI entry structure
	// this will be used to form an array of structures that 
	// contains the information about the catalogue
	struct cti_entry *cti_info;
	
	
	// creates a pointer to a fitsfile
	fitsfile *sdss_ptr;
	
	//opens the FITS file sdss_file.fit  
	fits_open_table(&sdss_ptr, file_name, READONLY, p_status);
	if (*p_status!=0)
	{
		return(*p_status);
	}
	

	
	//opens a file in which to store the informationtaken from the catalogue
	FILE *fp_info;
	if ((fp_info = fopen(cti_file_name, "wb"))==NULL)
	{
		return (ERR_CANNOT_OPEN);
	}
	

	
	//Gets the number of rows and Columns in the fits File previously opened
	//puts these into the cti file and prints them to the screen
	fits_get_num_rows(sdss_ptr, p_n_rows, p_status);
	if (*p_status!=0)
	{
		return(*p_status);
	}
	printf("\nThe Number of Rows is: %ld\n", *p_n_rows);
	
	
	fits_get_num_cols(sdss_ptr, p_n_columns, p_status);
	if (*p_status!=0)
	{
		return(*p_status);
	}
	printf("\nThe Number of Columns is: %d\n", *p_n_columns);
	
	//prints a row accross the top to describe each column
	printf("No.\tName\t\t\ttypecode\trepeat\t\twidth\n\n");
	
	
	cti_info = (struct cti_entry *) malloc ((*p_n_columns * sizeof (struct cti_entry)));
	if (cti_info == NULL)
	{
		return (ERR_OUT_OF_MEMORY);
	}
	
	error = get_details (cti_info, p_n_columns, sdss_ptr);

	if(error != ERR_NO_ERROR)
	{
		return (error);
	}
	

	if (fwrite (p_n_rows, sizeof (long int), 1, fp_info) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	if(fwrite (p_n_columns, sizeof (int), 1, fp_info) !=1)
	{
		return (ERR_WRITE_FAILURE);
	}
	
	if(fwrite (cti_info, sizeof (struct cti_entry), (*p_n_columns), fp_info) !=( (unsigned int)*p_n_columns))
	{
		return (ERR_WRITE_FAILURE);
	}
	
	//closes the CTI file and checks to see if 
	//there are errors in this file 
	if (fclose(fp_info) != 0)
	{
		error = ERR_CANNOT_CLOSE;	
	}
	
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	//closes the fits file
	fits_close_file (sdss_ptr, p_status);
	if (*p_status!=0)
	{
		return(*p_status);
	}
	
	//free (cti_info);
	
return (error);
}


int get_details (struct cti_entry * cti_info, int *p_n_columns, fitsfile *sdss_ptr)
{
	// creates an error value that can be returned
	int error = ERR_NO_ERROR;
	int status = 0;
	int *p_status = &status;
	
	
	//creates variables for temproaily storing the data about column type and width
	int typecode = 0;
	long int repeat = 0;
	long int width = 0;
	
	//creates a string for temporarily storing the column names
	char col_name[NAME_SIZE] = {};
	char col_template = '*';
	
	//creates an index for the name of the column
	int name_index = 0;
	
	//creates an index variable for the column 
	int column_index;
	
	//creates a blankspace character variable for file i/o purposes
	char blank = ' ';
	char *p_blank = & blank;
	
	//get the column name, number and details for each of the columns 
	for(column_index = 1; column_index < (*p_n_columns +1) ; column_index++ )
	{
		//writes the column number to the screen and file 
		(cti_info + column_index -1) ->col_number = column_index;
		printf ("\n%d.\t", (cti_info + column_index -1) ->col_number);	
		
		//gets the column name, then writes it to the screen and files
		fits_get_colname (sdss_ptr, CASEINSEN, &col_template, &col_name[0] , &column_index, p_status);

		for (name_index=0; (name_index < NAME_SIZE) && (col_name[name_index] != '\0'); name_index++)
		{
			
			(cti_info + column_index -1) ->col_name[name_index] =col_name [name_index];
			printf("%c", (cti_info + column_index -1)->col_name[name_index] );
		}
		
		
		//pads the remainder of the space of NAME_SIZE with spaces 
		//to properly format the display and file size
		for (name_index=name_index; (name_index < NAME_SIZE); name_index++)
		{
			(cti_info + column_index -1) ->col_name[name_index] = *p_blank;
			printf("%c", (cti_info + column_index -1) ->col_name[name_index]);
		}
		
		//gets the detailed information about each column, and prints those to the screen.
		fits_get_coltype(sdss_ptr, column_index, &typecode, &repeat, &width, p_status);

		
		(cti_info + column_index -1) ->type_code = typecode;
		(cti_info + column_index -1) ->repeat = repeat;
		(cti_info + column_index -1) ->width = width;
		printf ("\t%d\t\t%ld\t\t%ld", (cti_info + column_index -1) ->type_code, (cti_info + column_index -1) ->repeat, (cti_info + column_index -1) ->width );

	}
	
	return (error);
}





