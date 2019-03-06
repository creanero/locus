//============================================================================
// Name        : sdss_extract_4_1.cpp
// Author      : Oisin Creaner
// Date		   : 24th June 2009
// Copyright   : (c) Oisin Creaner, ITT Dublin 2007-2009
// Description : Extraction of relevant information from SDSS caltaogue
//============================================================================

// this program is written to extract the relevant information
// from SDSS-supplied FITS files.

// First it must access the information about
// the SDSS-supplied FITS file which has previously been 
// created by sdss_diagnose. 

// It must then check through this file for the requred 
// parameters as supplied by the user (in this version 
// to be hard-coded.)  

// Finally, it must open the SDSS-supplied FITS file and 
// extract the relevant information and then re-save this 
// as a streamlined .FITS file 

// Error checking routines are being applied, but these will 
// require more rigourous planning and design than is currently 
// implemented



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
#include "struct_definitions_5_0.h"

//includes the flags definitions
#include "flags_definitions_5_0.h"

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

int make_cti_file_name(char *file_name, char *cti_file_name);
int make_local_file_name(char *file_name, char *local_file_name);


//control function that calls the functions that extract 
//the information from the info file
int cti_extract (long int *p_in_num_rows, 
		int *p_num_columns, 
		struct cti_entry *cti_info, 
		int *p_ra_col, 
		int *p_dec_col, 
		int *p_mag_col,
		int *p_status_col,
		int *p_objc_flags_col,
		int *p_objc_flags2_col,
		int *p_objc_type_col,
		char *cti_file_name);

//gets the information from the sdss_info.cti file 
int cti_get_info (long int *g_p_in_num_rows, 
		int *g_p_num_columns, 
		FILE *g_fp_info, 
		struct cti_entry **p_cti_info);

// function that checks if the needed info is present
int cti_find (int *p_num_columns, 
		struct cti_entry *cti_info, 
		int *p_ra_col, 
		int *p_dec_col, 
		int *p_mag_col,
		int *p_status_col,
		int *p_objc_flags_col,
		int *p_objc_flags2_col,
		int *p_objc_type_col);

//control function that calls the functions that extract the
//new internal fits file from the existing file
int internal_fits_extract(long int *p_in_num_rows, 
		int *p_num_columns, 
		int *p_ra_col, 
		int *p_dec_col, 
		int *p_mag_col,
		int *p_status_col,
		int *p_objc_flags_col,
		int *p_objc_flags2_col,
		int *p_objc_type_col,
		char *file_name,
		char *local_file_name);


//this function reads the selected information from the fits file
int internal_fits_read (fitsfile *p_sdss_fits, 
		int *p_status, 
		struct star *star_array, 
		long int *p_in_num_rows, 
		int *p_ra_col, 
		int *p_dec_col, 
		int *p_mag_col,
		long int *p_out_num_rows, 
		int *p_status_col,
		int *p_objc_flags_col,
		int *p_objc_flags2_col,
		int *p_objc_type_col);

//this function writes the selected information to the new fits file
int internal_fits_write (fitsfile *p_out_fits, 
		int *p_status, 
		struct star *star_array, 
		long int *p_in_num_rows);



//Declaration of global variables (as a test run)

//string sdss_fit = SDSS_FIT ;




// M   M  AA  III N   N 
// MM MM A  A  I  NN  N 
// M M M AAAA  I  N N N 
// M   M A  A  I  N  NN 
// M   M A  A III N   N
int main(int argc, char *argv[])  {
	//creates a set of pointers to hold the critical information 
	//about the FITS file No. Rows, no. Columns and the names 
	//of each. Will dynamically create an array of cti_entry 
	//sturctures, which will affect these pointers by reference
	struct cti_entry *cti_info = NULL; 
	long int in_num_rows = 0;
	long int *p_in_num_rows = &in_num_rows;
	int num_columns = 0;
	int *p_num_columns = & num_columns;
	
	// sets up variables to contain the columns in which the 
	//relevant information can be found
	int ra_col = 0;
	int *p_ra_col = &ra_col;
	int dec_col = 0;
	int *p_dec_col = &dec_col;
	int mag_col = 0;
	int *p_mag_col = &mag_col;
	int status_col = 0;
	int *p_status_col = &status_col;
	int objc_flags_col = 0;
	int *p_objc_flags_col = &objc_flags_col;
	int objc_flags2_col = 0;
	int *p_objc_flags2_col = &objc_flags2_col;
	int objc_type_col = 0;
	int *p_objc_type_col = &objc_type_col;
	
	// Sets up a variable to be used for error checking routines
	int error = ERR_NO_ERROR;
	
	//creates a variable for extracting the information of file names
	char *file_name = (char *) malloc (FILE_NAME_SIZE * sizeof(char));
	char *cti_file_name = (char *) malloc (FILE_NAME_SIZE * sizeof(char));
	char *local_file_name = (char *) malloc (FILE_NAME_SIZE * sizeof(char));
	
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
	
	
	error = get_parameters_file (p_fp_parameters, p_num_files);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	for(file_index = 0; file_index < num_files; file_index ++)
	{
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
		
		error = make_local_file_name(file_name, local_file_name);
			
		error= error_checking (error);
		if (error != ERR_NO_ERROR)
		{
			return (error);
		}

		//runs the CTI Extract function and checks it for errors 
		error = cti_extract (p_in_num_rows, 
				p_num_columns, 
				cti_info, 
				p_ra_col, 
				p_dec_col, 
				p_mag_col, 
				p_status_col,
				p_objc_flags_col,
				p_objc_flags2_col,
				p_objc_type_col,
				cti_file_name);

		error = error_checking(error);
		//this allows the code to skip over missing files without exiting
		
		if (error != ERR_CANNOT_OPEN)
		{
			if (error != ERR_NO_ERROR)
			
			{
			return (error);
			}

			//Runs the FITS extract Function and checks it for errors
			error = internal_fits_extract(
					p_in_num_rows, 
					p_num_columns, 
					p_ra_col, 
					p_dec_col, 
					p_mag_col,
					p_status_col,
					p_objc_flags_col,
					p_objc_flags2_col,
					p_objc_type_col,
					file_name,
					local_file_name);
			error = error_checking(error);
			//this allows the code to skip over missing files without exiting
			if (error!= ERR_FITS_ERROR)
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
		else
		{
			error= ERR_NO_ERROR;
			missing_file_count++;
			printf("\n%ld out of %d files are missing so far." , missing_file_count, (file_index+1));
		}
		
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
	char *fit_extension = (char *) malloc (10*sizeof(char));
	fit_extension = ".fit";
	char *fits_extension = (char *) malloc (10*sizeof(char));
	fits_extension = ".fits";
	char *ppr_extension = (char *) malloc (10*sizeof(char));
	ppr_extension = ".ppr";
	char *prm_extension = (char *) malloc (10*sizeof(char));
	prm_extension = ".prm";
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
	else
	{
		*p_file_type = 'o';
	}
	
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
	
	
	return (error);
	
}












int make_local_file_name(char *file_name, char *local_file_name)
{
	int error = ERR_NO_ERROR;
	int name_index = 0;
	
	//creates a pointer to a string to be used to extend the 
	//filename for output purposes
	char *output_extension = (char *) malloc (11*sizeof(char));
	output_extension = "_local.fit";
	char *dot = (char *) malloc (2* sizeof (char));
	
	//*local_file_name = '!';
	
	for (name_index = /*1*/0; name_index < FILE_NAME_SIZE; name_index ++)
	{
		*(local_file_name + name_index) = *(file_name + name_index);
	}
	//finds the "dot" in the file name so the 
	//filename can be changed for output
	dot =strchr (local_file_name, '.');
	*dot = '\0';
	
	//puts the .cti extension on the filename for output purposes
	strcat (local_file_name, output_extension);

	
	return (error);
	
}



// EEEE X   X TTTTTT RRRR   AA   CCC TTTTTT 
// E     X X    TT   R   R A  A C      TT   
// EEE    X     TT   RRRR  AAAA C      TT   
// E     X X    TT   R R   A  A C      TT   
// EEEE X   X   TT   R  RR A  A  CCC   TT   

int cti_extract (long int *p_in_num_rows, 
		int *p_num_columns, 
		struct cti_entry *cti_info, 
		int *p_ra_col, 
		int *p_dec_col, 
		int *p_mag_col,
		int *p_status_col,
		int *p_objc_flags_col,
		int *p_objc_flags2_col,
		int *p_objc_type_col,
		char *cti_file_name)
{
	//creates a double pointer to cti_info 
	struct cti_entry **p_cti_info = & cti_info;
	
	
	//opens the information file
	FILE *fp_info;
	if ((fp_info = fopen(cti_file_name, "rb"))==NULL)
	{
		return (ERR_CANNOT_OPEN);
	}
	else
	{
		printf ("\nCTI File opened.\n");
	}
	//passes the info pointer into a function which 
	//reads in the information from a file.  The Pointer is 
	//modified by the function.  For error trapping, the function
	// returns an int value which can be assigend to certain 
	//error reporting routines
	int error = cti_get_info (p_in_num_rows, p_num_columns, fp_info, p_cti_info);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	else
	{
		printf ("\nCTI Info accessed successfully.\n");
	}
	
	
	// sends the columns information to a function which will
	// check to see if the required columns are present
	error = cti_find (p_num_columns, 
			cti_info, 
			p_ra_col, 
			p_dec_col, 
			p_mag_col,
			p_status_col,
			p_objc_flags_col,
			p_objc_flags2_col,
			p_objc_type_col);

	error = error_checking(error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	else
	{
		printf ("\nCTI parameters all found.\n");
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
	else
	{
		printf ("\nCTI access stage complete.\n");
	}
	
	//finishes by returning the error value
	return (error);
}






//  GGG  EEEE TTTTTT     III N   N FFFF  OOO  
// G     E      TT        I  NN  N F    O   O 
// G  GG EEE    TT        I  N N N FFF  O   O 
// G   G E      TT        I  N  NN F    O   O 
//  GGG  EEEE   TT   ____III N   N F     OOO  
int cti_get_info (long int *g_p_in_num_rows, 
		int *g_p_num_columns, 
		FILE *g_fp_info, 
		struct cti_entry **g_cti_info)
{
	
	//reads in the number of rows in the file from the 
	if (fread (g_p_in_num_rows, sizeof(long int), 1, g_fp_info) != 1)
	{
		if(feof(g_fp_info))
		{
			return (ERR_END_OF_FILE);
		}
		else
		{
			return (ERR_FILE_READ);
		}
	}
	else
	{
		printf ("\nNumber of rows successfully read:\t%ld\n", *g_p_in_num_rows);
	}
	
	//reads in the number of columns in the file from the 
	if (fread (g_p_num_columns, sizeof( int), 1, g_fp_info) != 1)
	{
		if(feof(g_fp_info))
		{
			return (ERR_END_OF_FILE);
		}
		else
		{
			return (ERR_FILE_READ);
		}
	}
	else
	{
		printf ("\nNumber of columns successfully read:\t%d\n", *g_p_num_columns);
	}
	
	//dynamically allocates enough space to store the 
	//information as an array of cti_entries
	
	 
	*g_cti_info = (struct cti_entry *) malloc (*g_p_num_columns * sizeof (struct cti_entry));
	if (g_cti_info == NULL)
	{
		return (ERR_OUT_OF_MEMORY);
	}
	else
	{
		printf ("\nCTI Memory allocation step succesfully completed\n");
	}
	
	
	//reads the cti_entry information into the array
	if (fread (*g_cti_info, sizeof(struct cti_entry), *g_p_num_columns, g_fp_info) != (unsigned int)*g_p_num_columns)
	{
		if(feof(g_fp_info))
		{
			return (ERR_END_OF_FILE);
		}
		else
		{
			return (ERR_FILE_READ);
		}
	}
	else
	{
		printf ("\nCTI Entries successfully read into array\n");
	}
	
	
	return (ERR_NO_ERROR);
	
}




//  FFFF III N   N DDD  
//  F     I  NN  N D  D 
//  FFF   I  N N N D  D 
//  F     I  N  NN D  D 
//  F    III N   N DDD  
                    
int cti_find (int *p_num_columns, 
		struct cti_entry *cti_info, 
		int *p_ra_col, 
		int *p_dec_col, 
		int *p_mag_col,
		int *p_status_col,
		int *p_objc_flags_col,
		int *p_objc_flags2_col,
		int *p_objc_type_col)

{
	
	// sets up variables to be used in control loops 
	// to search for the columns  
	int col_num_index = 0;
	int temp_name_index = 0;
	int name_check_index = 0;
	int found = 0;
	
	
	// sets up temporary variables to store the contents of pointers
	char t_col_name [NAME_SIZE] ="              ";
	char t_ra [NAME_SIZE+1] = "ra              ";
	char t_dec [NAME_SIZE+1] = "dec             ";
	char t_mag [NAME_SIZE+1] = "counts_model    ";
	char t_status [NAME_SIZE+1] = "status          ";
	char t_objc_flags [NAME_SIZE+1] = "objc_flags      ";
	char t_objc_flags2 [NAME_SIZE+1] = "objc_flags2     ";
	char t_objc_type [NAME_SIZE+1] = "objc_type       ";
	
	// finds ra
	for (col_num_index = 0; (col_num_index < *p_num_columns) && (found == 0); col_num_index ++)
	{
		//makes the temporary name array equal to the name in the  
		for (temp_name_index = 0; (temp_name_index < NAME_SIZE); temp_name_index ++)
		{
			t_col_name [temp_name_index] = (cti_info + col_num_index)->col_name [temp_name_index];

		}
		do// using do-while to allow check to be made at the end
		{
			if ((t_col_name[name_check_index] == t_ra[name_check_index]))
			{
				found = 1;
			}
			else
			{
				found = 0;
			}
			name_check_index ++;
		}
		while ((name_check_index < NAME_SIZE) && (found !=0 ));
		name_check_index = 0;
		if (found == 1)
		{
			*p_ra_col = (cti_info + col_num_index)->col_number;
			//*p_ra_col = col_num_index;
		}
	}
	if (found == 0)
	{
		return (ERR_PAR_NOT_FOUND);
	}
	else if (*p_ra_col > *p_num_columns)
	{
		return (ERR_INVALID_COLUMN);
	}
	else
	{
		printf ("\nRA column number found:\t%d\n", *p_ra_col);
	}
	

	//resets the found parameter
	col_num_index = 0;
	temp_name_index = 0;
	name_check_index = 0;
	found = 0;

	
	// finds dec
	for (col_num_index = 0; (col_num_index < *p_num_columns) && (found == 0); col_num_index ++)
	{
		//makes the temporary name array equal to the name in the  
		for (temp_name_index = 0; (temp_name_index < NAME_SIZE) && ((cti_info + col_num_index)->col_name[temp_name_index]!= '\0'); temp_name_index ++)
		{
			t_col_name [temp_name_index] = (cti_info + col_num_index)->col_name [temp_name_index];

		}
		do// (name_index = 0; (name_index < NAME_SIZE) && (t_dec[name_index]!= '\0'); name_index ++)
		{
			if ((t_col_name[name_check_index] == t_dec[name_check_index]))
			{
				found = 1;
			}
			else
			{
				found = 0;
			}
			name_check_index ++;
		}
		while ((name_check_index < NAME_SIZE)&& found !=0);
		//do-while requires that the index be reset
		name_check_index = 0;
		if (found == 1)
		{
			*p_dec_col = (cti_info + col_num_index)->col_number;
			//*p_dec_col = col_num_index;
		}
	}
	if (found == 0)
	{
		return (ERR_PAR_NOT_FOUND);
	}
	else if (*p_dec_col > *p_num_columns)
	{
		return (ERR_INVALID_COLUMN);
	}
	else
	{
		printf ("\ndec column number found:\t%d\n", *p_dec_col);
	}
	

	//resets the found parameter
	col_num_index = 0;
	temp_name_index = 0;
	name_check_index = 0;
	found = 0;	
	
	// finds mag
	for (col_num_index = 0; (col_num_index < *p_num_columns) && (found == 0); col_num_index ++)
	{
		//makes the temporary name array equal to the name in the  
		for (temp_name_index = 0; (temp_name_index < NAME_SIZE); temp_name_index ++)
		{
			t_col_name [temp_name_index] = (cti_info + col_num_index)->col_name [temp_name_index];

		}
		do// (name_index = 0; (name_index < NAME_SIZE) && (t_mag[name_index]!= '\0'); name_index ++)
		{
			if ((t_col_name[name_check_index] == t_mag[name_check_index]))
			{
				found = 1;
			}
			else
			{
				found = 0;
			}
			name_check_index ++;
		}
		while ((name_check_index < NAME_SIZE)  && found !=0);
		// resets the index after the do-while loop
		name_check_index = 0;
		if (found == 1)
		{
			*p_mag_col = (cti_info + col_num_index)->col_number;
		}
	}
	if (found == 0)
	{
		return (ERR_PAR_NOT_FOUND);
	}
	else if (*p_mag_col > *p_num_columns)
	{
		return (ERR_INVALID_COLUMN);
	}
	else
	{
		printf ("\nmag column number found:\t%d\n", *p_mag_col);
	}
	//resets the found parameter
	col_num_index = 0;
	temp_name_index = 0;
	name_check_index = 0;
	found = 0;	
	
	// finds status
	for (col_num_index = 0; (col_num_index < *p_num_columns) && (found == 0); col_num_index ++)
	{
		//makes the temporary name array equal to the name in the  
		for (temp_name_index = 0; (temp_name_index < NAME_SIZE); temp_name_index ++)
		{
			t_col_name [temp_name_index] = (cti_info + col_num_index)->col_name [temp_name_index];

		}
		do// (name_index = 0; (name_index < NAME_SIZE) && (t_status[name_index]!= '\0'); name_index ++)
		{
			if ((t_col_name[name_check_index] == t_status[name_check_index]))
			{
				found = 1;
			}
			else
			{
				found = 0;
			}
			name_check_index ++;
		}
		while ((name_check_index < NAME_SIZE)  && found !=0);
		// resets the index after the do-while loop
		name_check_index = 0;
		if (found == 1)
		{
			*p_status_col = (cti_info + col_num_index)->col_number;
		}
	}
	if (found == 0)
	{
		return (ERR_PAR_NOT_FOUND);
	}
	else if (*p_status_col > *p_num_columns)
	{
		return (ERR_INVALID_COLUMN);
	}
	else
	{
		printf ("\nstatus column number found:\t%d\n", *p_status_col);
	}
	
	//resets the found parameter
	col_num_index = 0;
	temp_name_index = 0;
	name_check_index = 0;
	found = 0;
	
	// finds objc_flags
	for (col_num_index = 0; (col_num_index < *p_num_columns) && (found == 0); col_num_index ++)
	{
		//makes the temporary name array equal to the name in the  
		for (temp_name_index = 0; (temp_name_index < NAME_SIZE); temp_name_index ++)
		{
			t_col_name [temp_name_index] = (cti_info + col_num_index)->col_name [temp_name_index];

		}
		do// using do-while to allow check to be made at the end
		{
			if ((t_col_name[name_check_index] == t_objc_flags[name_check_index]))
			{
				found = 1;
			}
			else
			{
				found = 0;
			}
			name_check_index ++;
		}
		while ((name_check_index < NAME_SIZE) && (found !=0 ));
		name_check_index = 0;
		if (found == 1)
		{
			*p_objc_flags_col = (cti_info + col_num_index)->col_number;
			//*p_objc_flags_col = col_num_index;
		}
	}
	if (found == 0)
	{
		return (ERR_PAR_NOT_FOUND);
	}
	else if (*p_objc_flags_col > *p_num_columns)
	{
		return (ERR_INVALID_COLUMN);
	}
	else
	{
		printf ("\nobjc_flags column number found:\t%d\n", *p_objc_flags_col);
	}
	

	//resets the found parameter
	col_num_index = 0;
	temp_name_index = 0;
	name_check_index = 0;
	found = 0;

	// finds objc_flags2
	for (col_num_index = 0; (col_num_index < *p_num_columns) && (found == 0); col_num_index ++)
	{
		//makes the temporary name array equal to the name in the  
		for (temp_name_index = 0; (temp_name_index < NAME_SIZE); temp_name_index ++)
		{
			t_col_name [temp_name_index] = (cti_info + col_num_index)->col_name [temp_name_index];

		}
		do// using do-while to allow check to be made at the end
		{
			if ((t_col_name[name_check_index] == t_objc_flags2[name_check_index]))
			{
				found = 1;
			}
			else
			{
				found = 0;
			}
			name_check_index ++;
		}
		while ((name_check_index < NAME_SIZE) && (found !=0 ));
		name_check_index = 0;
		if (found == 1)
		{
			*p_objc_flags2_col = (cti_info + col_num_index)->col_number;
			//*p_objc_flags2_col = col_num_index;
		}
	}
	if (found == 0)
	{
		return (ERR_PAR_NOT_FOUND);
	}
	else if (*p_objc_flags2_col > *p_num_columns)
	{
		return (ERR_INVALID_COLUMN);
	}
	else
	{
		printf ("\nobjc_flags2 column number found:\t%d\n", *p_objc_flags2_col);
	}
	

	//resets the found parameter
	col_num_index = 0;
	temp_name_index = 0;
	name_check_index = 0;
	found = 0;

	// finds objc_type
		for (col_num_index = 0; (col_num_index < *p_num_columns) && (found == 0); col_num_index ++)
		{
			//makes the temporary name array equal to the name in the  
			for (temp_name_index = 0; (temp_name_index < NAME_SIZE); temp_name_index ++)
			{
				t_col_name [temp_name_index] = (cti_info + col_num_index)->col_name [temp_name_index];

			}
			do// using do-while to allow check to be made at the end
			{
				if ((t_col_name[name_check_index] == t_objc_type[name_check_index]))
				{
					found = 1;
				}
				else
				{
					found = 0;
				}
				name_check_index ++;
			}
			while ((name_check_index < NAME_SIZE) && (found !=0 ));
			name_check_index = 0;
			if (found == 1)
			{
				*p_objc_type_col = (cti_info + col_num_index)->col_number;
				//*p_objc_type_col = col_num_index;
			}
		}
		if (found == 0)
		{
			return (ERR_PAR_NOT_FOUND);
		}
		else if (*p_objc_type_col > *p_num_columns)
		{
			return (ERR_INVALID_COLUMN);
		}
		else
		{
			printf ("\nType column number found:\t%d\n", *p_objc_type_col);
		}
		

		//resets the found parameter
		col_num_index = 0;
		temp_name_index = 0;
		name_check_index = 0;
		found = 0;





	if ((*p_ra_col == *p_dec_col ) || (*p_ra_col == *p_mag_col) || (*p_dec_col == *p_mag_col))
	{
		return (ERR_COLUMNS_EQUAL);
	}
	else
	{
		return (ERR_NO_ERROR);
	}
	
	return (ERR_NO_ERROR);
}



//work in progress


int internal_fits_extract(long int *p_in_num_rows, 
		int *p_num_columns, 
		int *p_ra_col, 
		int *p_dec_col, 
		int *p_mag_col,
		int *p_status_col,
		int *p_objc_flags_col,
		int *p_objc_flags2_col,
		int *p_objc_type_col,
		char *file_name,
		char *local_file_name)
{
	
	//creates file pointers and error reporting controls
	fitsfile *p_sdss_fits;
	int status = 0;
	int *p_status = &status;
	fitsfile *p_out_fits;
	int error = ERR_NO_ERROR;
	
	//creates a counter for the number of rows to be output
	//as some may be filtered
	long int out_num_rows = 0;
	long int *p_out_num_rows = &out_num_rows;
	
	//creates name and format files for the new table
	char ra_name[NAME_SIZE] = "ra";
	char *p_ra_name = & ra_name[0];
	char dec_name[NAME_SIZE] = "dec";
	char *p_dec_name = & dec_name[0];
	char mag_name[NAME_SIZE] = "mag";
	char *p_mag_name = & mag_name[0];
	char form[3] = "1D";
	char *p_form = &form[0];

	char form_mag[3] = "5D";
	char *p_form_mag = &form_mag[0];
	
	//debugging step
	int num_cols = 0;
	int *p_num_cols = & num_cols;
	
	//creates a pointer for a filename to allow overwriting of 
	//files
	char *local_file_name_ow = (char *) malloc (FILE_NAME_SIZE * sizeof (char)); 
	*local_file_name_ow = '!';
	*(local_file_name_ow +1)= '\0';
	
	//allocates memory for an array of star structures based
	//on the number of rows
	struct star *star_array = (struct star *) malloc  (*p_in_num_rows * sizeof (struct star));
	if (star_array == NULL)
	{
		return (ERR_OUT_OF_MEMORY);
	}
	else
	{
		printf ("\nMemory allocation step succesfully completed\n");
	}
	//opens the FITS file sdss_file.fit  
	fits_open_table(&p_sdss_fits, file_name, READONLY, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	//runs the FITS file read function and error checks it
	error= internal_fits_read(p_sdss_fits, 
			p_status, 
			star_array, 
			p_in_num_rows, 
			p_ra_col, 
			p_dec_col, 
			p_mag_col, 
			p_out_num_rows,
			p_status_col,
			p_objc_flags_col,
			p_objc_flags2_col,
			p_objc_type_col);

	if (error != ERR_NO_ERROR)
	{
		return (error);
	}	
	//closes the fits file
	fits_close_file (p_sdss_fits, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	
	
	strcat(local_file_name_ow, local_file_name);
	//local_file_name = local_file_name_ow;
	
	//creates the new fits file that will be used to store the stripped file
	fits_create_file (&p_out_fits, local_file_name_ow,p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}	
	printf ("\nNew FITS file created!\n");
	
	fits_create_tbl (p_out_fits, BINARY_TBL, 0, 1, &p_ra_name, &p_form, NULL, NULL, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("\nNew FITS table created!\n");
	fits_insert_col (p_out_fits, 2, p_dec_name, p_form, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	fits_insert_col (p_out_fits, 3, p_mag_name, p_form_mag, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}

	fits_get_num_cols (p_out_fits, p_num_cols, p_status);
	printf ("\nColumns set up.  Number of Columns is %d\n", *p_num_cols);
	
	error = internal_fits_write (p_out_fits, p_status,star_array, p_out_num_rows);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	//closes the second fitsfile
	fits_close_file (p_out_fits, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	
	return (error);
}















int internal_fits_read (fitsfile *p_sdss_fits, 
		int *p_status, 
		struct star *star_array, 
		long int *p_in_num_rows, 
		int *p_ra_col, 
		int *p_dec_col, 
		int *p_mag_col,
		long int *p_out_num_rows, 
		int *p_status_col,
		int *p_objc_flags_col,
		int *p_objc_flags2_col,
		int *p_objc_type_col)
{
	int error = ERR_NO_ERROR;
	int row_index = 0;
	int sdss_status = 0;
	int good_status = GOOD_STATUS;
	int bad_status = BAD_STATUS;
	int sdss_objc_flags = 0;
	int good_objc_flags = GOOD_OBJC_FLAGS;
	int bad_objc_flags = BAD_OBJC_FLAGS;
	int sdss_objc_flags2 = 0;
	//int good_objc_flags2 = GOOD_OBJC_FLAGS2;  //not used because there are no known required flags_2
	int bad_objc_flags2 = BAD_OBJC_FLAGS2;
	int sdss_objc_type = 0;
	int good_objc_type = GOOD_OBJC_TYPE;
	
	
	
	printf("\nStarting SDSS FITS read step.\n");
	for (row_index = 1; row_index <= *p_in_num_rows; row_index++)
	{	
		fits_read_col 
		(p_sdss_fits, //fitsfile
				TINT, //datatype
				*p_status_col, //colnum
				row_index, //firstrow
				1, //firstelem
				1, //nelem
				0, //nulval
				&sdss_status, //array (or address of value) 
				0, //anynul
				p_status);//status
		//fits_report_error (stdout, *p_status);
		printf("\nSDSS_status bit checked, value:0x%x", sdss_status);
		//if the status includes only good flags and no bad flags
		fits_read_col 
		(p_sdss_fits, //fitsfile
				TINT, //datatype
				*p_objc_flags_col, //colnum
				row_index, //firstrow
				1, //firstelem
				1, //nelem
				0, //nulval
				&sdss_objc_flags, //array (or address of value) 
				0, //anynul
				p_status);//status
		//fits_report_error (stdout, *p_status);
		printf("\nsdss_objc_flags bit checked, value:0x%x", sdss_objc_flags);
		
		fits_read_col 
		(p_sdss_fits, //fitsfile
				TINT, //datatype
				*p_objc_flags2_col, //colnum
				row_index, //firstrow
				1, //firstelem
				1, //nelem
				0, //nulval
				&sdss_objc_flags2, //array (or address of value) 
				0, //anynul
				p_status);//status
		//fits_report_error (stdout, *p_status);
		printf("\nsdss_objc_flags2 bit checked, value:0x%x", sdss_objc_flags2);
		
		fits_read_col 
		(p_sdss_fits, //fitsfile
				TINT, //datatype
				*p_objc_type_col, //colnum
				row_index, //firstrow
				1, //firstelem
				1, //nelem
				0, //nulval
				&sdss_objc_type, //array (or address of value) 
				0, //anynul
				p_status);//status
		//fits_report_error (stdout, *p_status);
		printf("\nsdss_objc_type bit checked, value:%d", sdss_objc_type);
		
		//if the status includes only good flags and no bad flags
		if (((sdss_status & good_status)&& !(sdss_status & bad_status)) &&((sdss_objc_flags & good_objc_flags)&& !(sdss_objc_flags & bad_objc_flags)) &&(/*(sdss_objc_flags2 & good_objc_flags2)&&*/ !(sdss_objc_flags2 & bad_objc_flags2)) && (sdss_objc_type == good_objc_type))
		{		
			printf("\nSDSS_status status bits checked: good");
			
			{
				printf ("\nreading star %ld into memory", *p_out_num_rows + 1);
				//reads ra, dec and mag into the array
				fits_read_col (p_sdss_fits, TDOUBLE, *p_ra_col, row_index, 1, 1, 0, &((star_array+*p_out_num_rows)->ra), 0, p_status);
				if (*p_status!=0)
				{
					return(ERR_FITS_ERROR);
				}
				
				
				//gets dec
				fits_read_col (p_sdss_fits, TDOUBLE, *p_dec_col, row_index, 1, 1, 0, &((star_array+*p_out_num_rows)->dec), 0, p_status);
				if (*p_status!=0)
				{
					return(ERR_FITS_ERROR);
				}
				
				//gets mag
				fits_read_col (p_sdss_fits, TDOUBLE, *p_mag_col, row_index, 1, NUM_COLOURS, 0, ((star_array+*p_out_num_rows)->mag), 0, p_status);
				if (*p_status!=0)
				{
					return(ERR_FITS_ERROR);
				}
				//increases the count of valid stars
				*p_out_num_rows = *p_out_num_rows +1;
				
			}
		}
		else
		{
			printf("\nSDSS_status status bits checked: bad");
		}
	}	
	
	printf ("\nReading from SDSS Fits file complete\n");
	return (error);
}

int internal_fits_write (fitsfile *p_out_fits, int *p_status, struct star *star_array, long int *p_in_num_rows)
{
	int error = ERR_NO_ERROR;
	int row_index = 0;
	int new_ra_col = 1;
	int new_dec_col = 2;
	int new_mag_col = 3;

	long int in_num_rows_2 = 0;
	long int *p_in_num_rows_2 = &in_num_rows_2;
	fits_insert_rows(p_out_fits, 0, *p_in_num_rows, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	
	fits_get_num_rows (p_out_fits, p_in_num_rows_2, p_status);
	printf("\nNumber of Rows setup.  Number of Rows is %ld. \n", *p_in_num_rows);
	
	
	printf ("\nNo.\tRA\t\tDec\t\tMagU\t\tMagG\t\tMagR\t\tMagI\t\tMagZ\t\n");
	for (row_index = 1; row_index <= *p_in_num_rows; row_index++)
	{
		printf ("%d.\t", (row_index));

		fits_write_col (p_out_fits, TDOUBLE, new_ra_col, row_index, 1, 1, &((star_array+row_index-1)->ra), p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		printf ("%lf\t", (star_array + row_index-1)->ra);
		fits_write_col (p_out_fits, TDOUBLE, new_dec_col, row_index, 1, 1, &((star_array+row_index-1)->dec), p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		printf ("%lf\t", (star_array + row_index-1)->dec);
		fits_write_col (p_out_fits, TDOUBLE, new_mag_col, row_index, 1, NUM_COLOURS, ((star_array+row_index-1)->mag), p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		printf ("%lf\t%lf\t%lf\t%lf\t%lf\t\n",(star_array + row_index-1)->mag[0],(star_array + row_index-1)->mag[1],(star_array + row_index-1)->mag[2],(star_array + row_index-1)->mag[3],(star_array + row_index-1)->mag[4]);
	}
	
	printf("\nFinal file writing step complete.\n");
	return (error);
}



