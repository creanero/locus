//============================================================================
// Name        : parameterisation_6_0_.cpp
// Author      : Oisin Creaner	
// Version     : 6.0 
// Copyright   : (c) ITT Dublin, Oisin Creaner 13th October 2010
// Description : Program to create a file list and target specs as a parameter 
//============================================================================


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

#include <iostream>


//required for FITS file operations
#include <fitsio.h>

//includes the error reporting header file
#include "error_checking_5_2.h"

//includes the file name definitions file
#include "file_info_5_0.h"

//includes the struct definitions file
#include "struct_definitions_5_2.h"

using namespace std;

int check_arguments (int num_args, 
		char *p_arg_num_targets, 
		char *p_prm_file_path,
		char *p_csv_file_name, 
		char *p_fit_file_path, 	
		char *p_arg_fov_size,
		char *p_arg_resolution,
		char *p_arg_max_mag_diff,
		char *p_arg_max_col_diff,
		char *p_prm_file_name,
		FILE **p_fp_csv,
		char *p_file_type,
		long int *p_num_targets,
		double *p_fov_size,
		double *p_resolution,
		double *p_max_mag_diff,
		double *p_max_col_diff,
		FILE **p_fp_prm );


int name_parameter_file (	FILE **p_fp_parameters, 
							char *p_file_type);


int name_csv_file (	FILE **p_fp_csv, 
							char *p_file_type);

//int create_ppr(FILE **p_fp_parameters);

int create_prm (	char *p_prm_file_path, 
					char *p_fit_file_path,
					FILE **p_fp_csv, 
					long int *p_num_targets,
					long int *p_csv_index,
					bool *p_end_of_csv);

int enter_telescope_info (	double *p_fov_size,
								double *p_resolution,
								double *p_max_mag_diff,
								double *p_max_col_diff,
								FILE **p_fp_prm);

int enter_field_info ( 	FILE **p_fp_prm,
						FILE **p_fp_txt,
						FILE **p_fp_csv, 
						char *p_fit_file_path);


int enter_xop_field_info ( 	FILE **p_fp_prm,
						FILE **p_fp_txt,
						FILE **p_fp_csv, 
						FILE **p_fp_xop_prm,
						char *p_fit_file_path);

//this program reads in the informationfrom the supplied fits file
int read(char *file_name, struct star **p_input_array, long int *p_in_num_rows);

int enter_file_name (	char *file_name, 
						FILE **p_fp_csv,
						FILE **p_fp_parameters,
						char *p_fit_file_path);

//ppr functions commented out
/*int enter_target_info (	FILE **p_fp_parameters,
						long int target_index,
						long int field_index);*/

int check_file_type(char *file_name, 
					char *p_file_type);

int create_ppr(		char *p_prm_file_path, 
					char *p_fit_file_path,
					FILE **p_fp_csv,
					long int *p_num_targets,	
					double *p_fov_size,
					double *p_resolution,
					double *p_max_mag_diff,
					double *p_max_col_diff,
					long int *p_csv_index,
					bool *p_end_of_csv);

/*int create_xop_ppr(		char *p_prm_file_path, 
						char *p_fit_file_path,
						FILE **p_fp_csv,
						long int *p_num_targets,	
						double *p_fov_size,
						double *p_resolution,
						double *p_max_mag_diff,
						double *p_max_col_diff,
						long int *p_csv_index,
						bool *p_end_of_csv);*/

int create_xop_ppr(		char *p_prm_file_path, 
						char *p_fit_file_path,
						FILE **p_fp_csv,
						FILE **p_fp_xop_prm,
						long int *p_num_targets,	
						double *p_fov_size,
						double *p_resolution,
						double *p_max_mag_diff,
						double *p_max_col_diff,
						long int *p_csv_index,
						bool *p_end_of_csv);

int main(int argc, char *argv[]) 
{

	//sets up error handling variable
	int error = ERR_NO_ERROR;
	
	
	//creates a pointer for a the csv file to be read
	FILE *fp_csv;
	FILE **p_fp_csv = & fp_csv;
	
	//creates a pointer for a the prm file to be read (XOP ppr generation)
	FILE *fp_xop_prm;
	FILE **p_fp_xop_prm = & fp_xop_prm;
	
	char file_type = 'o';
	char *p_file_type = &file_type;
	
	//sets up a variable to hold the number of files 
	//to be listed
	long int num_targets = 0;
	long int *p_num_targets = &num_targets;
	long int csv_index = 0;
	long int *p_csv_index = & csv_index;
	
	//sets up a boolean flag to watch for the end of the csv file
	bool end_of_csv = 0;
	bool *p_end_of_csv = & end_of_csv;
	
	//replaces the argv[] with named parameters
	char *p_arg_num_targets   = argv[1];
	char *p_prm_file_path     = argv[2];
	char *p_csv_file_name     = argv[3];
	char *p_fit_file_path     = argv[4];
	char *p_arg_fov_size      = argv[5];
	char *p_arg_resolution    = argv[6];
	char *p_arg_max_mag_diff  = argv[7];
	char *p_arg_max_col_diff  = argv[8];
	char *p_xop_prm_file_name = argv[9];
	

	double fov_size = 0;
	double resolution = 0;
	double max_mag_diff = 0;
	double max_col_diff = 0;
	
	double *p_fov_size = &fov_size;
	double *p_resolution = &resolution;
	double *p_max_mag_diff = &max_mag_diff;
	double *p_max_col_diff = &max_col_diff;
	
	
	//checks the arguments to main and opens the CSV file
	error = check_arguments (	argc, 
								p_arg_num_targets,  
								p_prm_file_path, 
								p_csv_file_name, 
								p_fit_file_path,	
								p_arg_fov_size,
								p_arg_resolution,
								p_arg_max_mag_diff,
								p_arg_max_col_diff,
								p_xop_prm_file_name,
								p_fp_csv, 
								p_file_type,
								p_num_targets,	
								p_fov_size,
								p_resolution,
								p_max_mag_diff,
								p_max_col_diff,
								p_fp_xop_prm);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	if (*p_file_type == 'a')
	{
		
		if (fscanf (fp_csv, "run,rerun,camcol,field\n")!=0)
		{
			printf("\nThe CSV file does not contain the appropriate information.");
			printf("\nThe CSV file must contain run,rerun,camcol,field\n");
						
			return (ERR_FILE_READ);
		}
		
		do
		{
			printf("\nCreating prm files now");
			
			error = create_prm(	p_prm_file_path, 
								p_fit_file_path,
								p_fp_csv, 
								p_num_targets,
								p_csv_index,
								p_end_of_csv);
			
			if (error != ERR_NO_ERROR)
			{
				return (error);
			}
			
			csv_index = csv_index + num_targets;
			/*if (fclose(fp_parameters) != 0)
			{
				error = ERR_CANNOT_CLOSE;	
			}*/
		}
		while (end_of_csv == 0);
		
		fclose (*p_fp_csv);
	}
	
	else if (*p_file_type == 'p')
	{
		
		if (fscanf (fp_csv, "specObjID,No of Fields for Quasar,RUN,RERUN,CAMCOL,FIELD,Quasar RA,Quasar DEC,u magnitude,g magnitude,r magnitude,i magnitude,z magnitude\n")!=0)
			//if (fscanf (fp_csv, "No of Fields for Quasar,RUN,RERUN,CAMCOL,FIELD,Quasar RA,Quasar DEC,u magnitude,g magnitude,r magnitude,i magnitude,z magnitude\n")!=0)

		{
			printf("\nThe CSV file does not contain the appropriate information.");
			return (ERR_FILE_READ);
		}
		
		do
		{
			printf("\nCreating ppr files now");
			
			error = create_ppr(	p_prm_file_path, 
								p_fit_file_path,
								p_fp_csv,
								p_num_targets,	
								p_fov_size,
								p_resolution,
								p_max_mag_diff,
								p_max_col_diff,
								p_csv_index,
								p_end_of_csv);
			
			if (error != ERR_NO_ERROR)
			{
				return (error);
			}
			
			csv_index = csv_index + num_targets;
			/*if (fclose(fp_parameters) != 0)
			{
				error = ERR_CANNOT_CLOSE;	
			}*/
		}
		while (end_of_csv == 0);
		
		fclose (*p_fp_csv);
	}

	
	else if (*p_file_type == 'x')
	{
		
		if (fscanf (fp_csv, "NoFields,Field Number,ROW,REF Field ID,REF RUN,REF RERUN,REF CAMCOL,REF FIELD,RUN,RERUN,CAMCOL,FIELD\n")!=0)
			//if (fscanf (fp_csv, "No of Fields for Quasar,RUN,RERUN,CAMCOL,FIELD,Quasar RA,Quasar DEC,u magnitude,g magnitude,r magnitude,i magnitude,z magnitude\n")!=0)

		{
			printf("\nThe CSV file does not contain the appropriate information.");
			return (ERR_FILE_READ);
		}
		
		do
		{
			printf("\nCreating ppr files now");
			
			error = create_xop_ppr(	p_prm_file_path, 
									p_fit_file_path,
									p_fp_csv,
									p_fp_xop_prm,
									p_num_targets,	
									p_fov_size,
									p_resolution,
									p_max_mag_diff,
									p_max_col_diff,
									p_csv_index,
									p_end_of_csv);
			
			if (error != ERR_NO_ERROR)
			{
				return (error);
			}
			
			csv_index = csv_index + num_targets;
			/*if (fclose(fp_parameters) != 0)
			{
				error = ERR_CANNOT_CLOSE;	
			}*/
		}
		while (end_of_csv == 0);
		
		fclose (*p_fp_csv);
	}
	else
	{
		printf("\nThe CSV file does not contain the appropriate information.");
		return (ERR_FILE_READ);
	}
	//old code retained for future use.  Potentially "arg_five" can be p/a?
	/*if (*p_file_type == 'p')
	{
		error = create_ppr(p_fp_parameters);
		if (error != ERR_NO_ERROR)
		{
			return (error);
		}
		
		if (fclose(fp_parameters) != 0)
		{
			error = ERR_CANNOT_CLOSE;	
		}
	}
	else if (*p_file_type == 'a')
	{
		error = create_prm(p_fp_parameters);
		
		if (error != ERR_NO_ERROR)
		{
			return (error);
		}
		
		if (fclose(fp_parameters) != 0)
		{
			error = ERR_CANNOT_CLOSE;	
		}
	}
	else
	{
		return (ERR_WRONG_FILE_TYPE);
	}*/


	
	return (error);
}

int check_arguments (int num_args, 
					char *p_arg_num_targets, 
					char *p_prm_file_path,
					char *p_csv_file_name, 
					char *p_fit_file_path, 	
					char *p_arg_fov_size,
					char *p_arg_resolution,
					char *p_arg_max_mag_diff,
					char *p_arg_max_col_diff,
					char *p_xop_prm_file_name,
					FILE **p_fp_csv,
					char *p_file_type,
					long int *p_num_targets,
					double *p_fov_size,
					double *p_resolution,
					double *p_max_mag_diff,
					double *p_max_col_diff,
					FILE **p_fp_xop_prm)
{
	int error = ERR_NO_ERROR;
	

	//if no arguments have been entered, ask the user to enter a filename
	if (num_args <= 4)
	{
		printf ("\nYou have not entered a valid set of parameters.");

			error = name_csv_file (p_fp_csv, p_file_type);
			error = error_checking (error);
			if (error != ERR_NO_ERROR)
			{
				return (error);
			}

	}
	//if an argument has been entered, attempt to use it.
	else if (num_args == 5)
	{
		if( sscanf (p_arg_num_targets, "%ld", p_num_targets) !=1)
		{
			//asks the user for the number of files then writes it to the new file
			printf ("\nThe number of files enteres is invalid\t");
			return (ERR_BAD_NUMERIC);
			//scanf("%ld", p_num_targets);
		}

		while (*p_num_targets <1)
		{
			if (*p_num_targets < 0)
			{
				*p_num_targets = 0 - *p_num_targets;
			}
			if (*p_num_targets ==0)
			{
				//asks the user for the number of files then writes it to the new file
				printf ("\nThe number of targets entered is invalid\t");
				return (ERR_BAD_NUMERIC);
				//scanf("%ld", p_num_targets);
			}
		}
		
		printf("\nYou have seleted %ld fits files per prm file", *p_num_targets);

		
		printf("\nThe file path you have selected for the root of the parameter files is %s", p_prm_file_path);
		//if it is not a valid file name, attempt to request a new filename
		
		printf("\nThe csv file you have selected for is %s", p_csv_file_name);
		//if it is not a valid file name, attempt to request a new filename
		
		check_file_type (p_csv_file_name, p_file_type);

		if ((*p_file_type =='c'))
		{
			if ((*p_fp_csv = fopen(p_csv_file_name, "r"))==NULL)
				{
					printf ("\nthis file failed to open! Please try again!");
					error = name_csv_file (p_fp_csv, p_file_type);
					error = error_checking (error);
					if (error != ERR_NO_ERROR)
					{
						return (error);
					}	
				}
		}
		*p_file_type = 'a';
	}
	//if an argument has been entered, attempt to use it.
	else if (num_args == 9)
	{
		if( sscanf (p_arg_num_targets, "%ld", p_num_targets) !=1)
		{
			//asks the user for the number of files then writes it to the new file
			printf ("\nthe number of fields entered was invalid!\t");
			return (ERR_BAD_NUMERIC);
			//scanf("%ld", p_num_targets);
		}

		while (*p_num_targets <1)
		{
			if (*p_num_targets < 0)
			{
				*p_num_targets = 0 - *p_num_targets;
			}
			if (*p_num_targets ==0)
			{
				//asks the user for the number of files then writes it to the new file
				printf ("\nThe number of fields submitted was invalid\t");
				return (ERR_BAD_NUMERIC);
				//scanf("%ld", p_num_targets);
			}
		}
		
		printf("\nYou have seleted %ld fits files per prm file", *p_num_targets);

		
		printf("\nThe file path you have selected for the root of the parameter files is %s", p_prm_file_path);
		//if it is not a valid file name, attempt to request a new filename
		
		printf("\nThe csv file you have selected for is %s", p_csv_file_name);
		//if it is not a valid file name, attempt to request a new filename
		
		check_file_type (p_csv_file_name, p_file_type);

		if ((*p_file_type =='c'))
		{
			if ((*p_fp_csv = fopen(p_csv_file_name, "r"))==NULL)
				{
					printf ("\nthis file failed to open! Please try again!");
					error = name_csv_file (p_fp_csv, p_file_type);
					error = error_checking (error);
					if (error != ERR_NO_ERROR)
					{
						return (error);
					}	
				}
		}
		else
		{
			printf ("\nYou have not entered a valid set of parameters.");

				error = name_csv_file (p_fp_csv, p_file_type);
				error = error_checking (error);
				if (error != ERR_NO_ERROR)
				{
					return (error);
				}

		}
		
		printf("\nThe file path you have selected for the root of the fits files is %s", p_fit_file_path);
		
		
		//reads the field of view in from the command line
		if( sscanf (p_arg_fov_size, "%lf", p_fov_size) !=1)
		{
			//asks the user for the number of files then writes it to the new file
			printf ("\nThe Field of view size selected was invalid\t");
			return (ERR_BAD_NUMERIC);
		}
		
		printf("\nThe Field of View size selected was %lf degrees", *p_fov_size);

		
		
		
		//reads the resolution in fromt he command line
		if( sscanf (p_arg_resolution, "%lf", p_resolution) !=1)
		{
			//asks the user for the number of files then writes it to the new file
			printf ("\nThe Resolution selected was invalid\t");
			return (ERR_BAD_NUMERIC);
		}
		
		printf("\nThe Resolution selected was %lf degrees", *p_resolution);		
		
		
		
		
		//reads the maximum magnitude difference in from the command line
		if( sscanf (p_arg_max_mag_diff, "%lf", p_max_mag_diff) !=1)
		{
			//asks the user for the number of files then writes it to the new file
			printf ("\nThe Maximum magnitude difference selected was invalid\t");
			return (ERR_BAD_NUMERIC);
		}
		
		printf("\nThe Maximum magnitude difference selected was %lf ", *p_max_mag_diff);	
		
		
		
		//reads the maximum colour difference in from the command line
		if( sscanf (p_arg_max_col_diff, "%lf", p_max_col_diff) !=1)
		{
			//asks the user for the number of files then writes it to the new file
			printf ("\nThe Maximum colour difference selected was invalid\t");
			return (ERR_BAD_NUMERIC);
		}
		
		printf("\nThe Maximum Colour Difference selected was %lf ", *p_max_col_diff);		
		
		
		//sets the file type pointer to "p" to ensure this is processed as a ppr file
		*p_file_type = 'p';
		
	}	
	
	
	
	
	
	
	
	
	
	
	else if (num_args == 10)
	{
		if( sscanf (p_arg_num_targets, "%ld", p_num_targets) !=1)
		{
			//asks the user for the number of files then writes it to the new file
			printf ("\nthe number of fields entered was invalid!\t");
			return (ERR_BAD_NUMERIC);
			//scanf("%ld", p_num_targets);
		}

		while (*p_num_targets <1)
		{
			if (*p_num_targets < 0)
			{
				*p_num_targets = 0 - *p_num_targets;
			}
			if (*p_num_targets ==0)
			{
				//asks the user for the number of files then writes it to the new file
				printf ("\nThe number of fields submitted was invalid\t");
				return (ERR_BAD_NUMERIC);
				//scanf("%ld", p_num_targets);
			}
		}
		
		printf("\nYou have seleted %ld fits files per prm file", *p_num_targets);

		
		printf("\nThe file path you have selected for the root of the parameter files is %s", p_prm_file_path);
		//if it is not a valid file name, attempt to request a new filename
		
		printf("\nThe csv file you have selected for is %s", p_csv_file_name);
		//if it is not a valid file name, attempt to request a new filename
		
		check_file_type (p_csv_file_name, p_file_type);

		if ((*p_file_type =='c'))
		{
			if ((*p_fp_csv = fopen(p_csv_file_name, "r"))==NULL)
				{
					printf ("\nthis file failed to open! Please try again!");
					error = name_csv_file (p_fp_csv, p_file_type);
					error = error_checking (error);
					if (error != ERR_NO_ERROR)
					{
						return (error);
					}	
				}
		}
		else
		{
			printf ("\nYou have not entered a valid set of parameters.");

				error = name_csv_file (p_fp_csv, p_file_type);
				error = error_checking (error);
				if (error != ERR_NO_ERROR)
				{
					return (error);
				}

		}
		
		printf("\nThe file path you have selected for the root of the fits files is %s", p_fit_file_path);
		
		
		//reads the field of view in from the command line
		if( sscanf (p_arg_fov_size, "%lf", p_fov_size) !=1)
		{
			//asks the user for the number of files then writes it to the new file
			printf ("\nThe Field of view size selected was invalid\t");
			return (ERR_BAD_NUMERIC);
		}
		
		printf("\nThe Field of View size selected was %lf degrees", *p_fov_size);

		
		
		
		//reads the resolution in fromt he command line
		if( sscanf (p_arg_resolution, "%lf", p_resolution) !=1)
		{
			//asks the user for the number of files then writes it to the new file
			printf ("\nThe Resolution selected was invalid\t");
			return (ERR_BAD_NUMERIC);
		}
		
		printf("\nThe Resolution selected was %lf degrees", *p_resolution);		
		
		
		
		
		//reads the maximum magnitude difference in from the command line
		if( sscanf (p_arg_max_mag_diff, "%lf", p_max_mag_diff) !=1)
		{
			//asks the user for the number of files then writes it to the new file
			printf ("\nThe Maximum magnitude difference selected was invalid\t");
			return (ERR_BAD_NUMERIC);
		}
		
		printf("\nThe Maximum magnitude difference selected was %lf ", *p_max_mag_diff);	
		
		
		
		//reads the maximum colour difference in from the command line
		if( sscanf (p_arg_max_col_diff, "%lf", p_max_col_diff) !=1)
		{
			//asks the user for the number of files then writes it to the new file
			printf ("\nThe Maximum colour difference selected was invalid\t");
			return (ERR_BAD_NUMERIC);
		}
		
		printf("\nThe Maximum Colour Difference selected was %lf ", *p_max_col_diff);		
		
		
		check_file_type (p_xop_prm_file_name, p_file_type);

		if ((*p_file_type =='a'))
		{
			if ((*p_fp_xop_prm = fopen(p_xop_prm_file_name, "r"))==NULL)
				{
					printf ("\nThe prm file failed to open!");
					/*error = name_csv_file (p_fp_csv, p_file_type);
					error = error_checking (error);
					if (error != ERR_NO_ERROR)
					{
						return (error);
					}	*/
				}
		}
		else
		{
			printf ("\nYou have not entered a valid set of parameters.");

				/*error = name_csv_file (p_fp_csv, p_file_type);
				error = error_checking (error);
				if (error != ERR_NO_ERROR)
				{
					return (error);
				}*/

		}
		
		//sets the file type pointer to "x" to ensure this is processed as an XOP ppr file
		*p_file_type = 'x';
		
	}
	
	
	
	
	
	
	
	else if (num_args > 10)
	{
		printf ("\nYou have not entered a valid set of parameters.");
		printf ("\nValid parameters are in the following format.");
		printf ("\nNumber of files per prm.  Root of prm file names.  CSV file name. Root of FITS file paths");

			/*error = name_parameter_file (p_fp_parameters, p_file_type);
			error = error_checking (error);
			if (error != ERR_NO_ERROR)
			{
				return (error);
			}*/
		return (ERR_PAR_NOT_FOUND);

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
		printf ("\nPlease Enter the full path of the parameter file you wish to create\t");
		scanf ("%s", &file_name [0]);
		
		check_file_type (file_name, p_file_type);

		if (((*p_file_type =='p')||(*p_file_type =='a')))
		{
			if ((*p_fp_parameters = fopen(&file_name[0], "wb"))==NULL)
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
	while (!((*p_file_type =='p')||(*p_file_type =='a')));
	
	return (error);
}


int name_csv_file (	FILE **p_fp_csv, 
					char *p_file_type)
{
	int error = ERR_NO_ERROR;
	
	//creates a variable for the filetype choosen
	char file_name [FILE_NAME_SIZE] = "0";
		


	
	//Asks the user for the name of the file to be created
	do
	{
		printf ("\nPlease Enter the full path of the csv file you wish to access\t");
		scanf ("%s", &file_name [0]);
		
		check_file_type (file_name, p_file_type);

		if (((*p_file_type =='c')))
		{
			if ((*p_fp_csv = fopen(&file_name[0], "r"))==NULL)
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
	while (!((*p_file_type =='c')));
	
	return (error);
}
int create_ppr(		char *p_prm_file_path, 
					char *p_fit_file_path,
					FILE **p_fp_csv,
					long int *p_num_targets,	
					double *p_fov_size,
					double *p_resolution,
					double *p_max_mag_diff,
					double *p_max_col_diff,
					long int *p_csv_index,
					bool *p_end_of_csv)
{
	int error = ERR_NO_ERROR;
	
	
	//sets up a character pointer to store the filenames as
	//an array up to 100 characters long
	char *file_name = (char *) malloc (FILE_NAME_SIZE *sizeof (char));
	char *txt_name = (char *) malloc (FILE_NAME_SIZE *sizeof (char));
	
	//creates a name tag for the end number of the file
	long int end_number = (*p_csv_index+*p_num_targets);
	
	
	//creates the string of the file name of the PRM file needed, using indices
	sprintf (file_name, "%s_%ld_%ld.ppr", p_prm_file_path, *p_csv_index, end_number) ;
	
	printf("\nCreating a prm file named %s", file_name);

	
	//creates a pointer for a the prm file to be created
	FILE *fp_prm;
	FILE **p_fp_prm = & fp_prm;
	
	*p_fp_prm = fopen(file_name, "wb");
	
	
	if	(*p_fp_prm ==NULL)
	{
		printf ("\nCannot open prm file %s", file_name);
		return (ERR_CANNOT_OPEN);
	}
	
	//creates the string of the file name of the PRM file needed, using indices
	sprintf (txt_name, "%s_%ld_%ld.txt", p_prm_file_path, *p_csv_index, end_number) ;
	
	printf("\nCreating a txt file named %s", txt_name);

	//creates a pointer for a the prm file to be created
	FILE *fp_txt;
	FILE **p_fp_txt = & fp_txt;
	
	*p_fp_txt = fopen(txt_name, "w");
	
	
	if	(*p_fp_txt ==NULL)
	{
		printf ("\nCannot open prm file %s", txt_name);
		return (ERR_CANNOT_OPEN);
	}
	

	error = enter_telescope_info (	p_fov_size, 
									p_resolution,
									p_max_mag_diff,
									p_max_col_diff,
									p_fp_prm);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	
	//sets up a variable to hold the number of targets 
	//to be listed
	long int num_fields = *p_num_targets;
	long int field_index = 0;

	if (fwrite (&num_fields, sizeof (long int), 1, *p_fp_prm) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	
	//prints the number of fields
	printf("\n%ld", num_fields);
	
				
	
	//goes through all the fields and gets the infoabout the fields
	while ( field_index < *p_num_targets && (!feof(*p_fp_csv)) )
	{

		error = enter_field_info ( 	p_fp_prm,
									p_fp_txt,
									p_fp_csv, 
									p_fit_file_path);
		error = error_checking (error);
		if (error != ERR_NO_ERROR)
		{
			return (error);
		}
		field_index ++;
	
	}
	if (feof(*p_fp_csv))
	{
		*p_end_of_csv = 1;
		rewind(*p_fp_csv);
		error = enter_telescope_info (	p_fov_size, 
										p_resolution,
										p_max_mag_diff,
										p_max_col_diff,
										p_fp_prm);
		error = error_checking (error);
		if (error != ERR_NO_ERROR)
		{
			return (error);
		}
		if (fwrite (&field_index, sizeof (long int), 1, *p_fp_prm) != 1)
		{
			return (ERR_WRITE_FAILURE);
		}
		printf("\nThis parameter file closed early.  Only %ld entries in this file", field_index);
	}
	
	return (error);
	
}



int create_xop_ppr(		char *p_prm_file_path, 
						char *p_fit_file_path,
						FILE **p_fp_csv,
						FILE **p_fp_xop_prm,
						long int *p_num_targets,	
						double *p_fov_size,
						double *p_resolution,
						double *p_max_mag_diff,
						double *p_max_col_diff,
						long int *p_csv_index,
						bool *p_end_of_csv)
{
	int error = ERR_NO_ERROR;
	
	
	//sets up a character pointer to store the filenames as
	//an array up to 100 characters long
	char *file_name = (char *) malloc (FILE_NAME_SIZE *sizeof (char));
	char *txt_name = (char *) malloc (FILE_NAME_SIZE *sizeof (char));
	
	//creates a name tag for the end number of the file
	long int end_number = (*p_csv_index+*p_num_targets);
	
	
	//creates the string of the file name of the PRM file needed, using indices
	sprintf (file_name, "%s_%ld_%ld.ppr", p_prm_file_path, *p_csv_index, end_number) ;
	
	printf("\nCreating a prm file named %s", file_name);

	
	//creates a pointer for a the prm file to be created
	FILE *fp_prm;
	FILE **p_fp_prm = & fp_prm;
	
	*p_fp_prm = fopen(file_name, "wb");
	
	
	if	(*p_fp_prm ==NULL)
	{
		printf ("\nCannot open prm file %s", file_name);
		return (ERR_CANNOT_OPEN);
	}
	
	//creates the string of the file name of the PRM file needed, using indices
	sprintf (txt_name, "%s_%ld_%ld.txt", p_prm_file_path, *p_csv_index, end_number) ;
	
	printf("\nCreating a txt file named %s", txt_name);

	//creates a pointer for a the prm file to be created
	FILE *fp_txt;
	FILE **p_fp_txt = & fp_txt;
	
	*p_fp_txt = fopen(txt_name, "w");
	
	
	if	(*p_fp_txt ==NULL)
	{
		printf ("\nCannot open prm file %s", txt_name);
		return (ERR_CANNOT_OPEN);
	}
	

	error = enter_telescope_info (	p_fov_size, 
									p_resolution,
									p_max_mag_diff,
									p_max_col_diff,
									p_fp_prm);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	
	//sets up a variable to hold the number of targets 
	//to be listed
	long int num_fields = *p_num_targets;
	long int field_index = 0;

	if (fwrite (&num_fields, sizeof (long int), 1, *p_fp_prm) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	
	//prints the number of fields
	printf("\n%ld", num_fields);
	
				
	
	//goes through all the fields and gets the infoabout the fields
	while ( field_index < *p_num_targets && (!feof(*p_fp_csv)) )
	{

		error = enter_xop_field_info ( 	p_fp_prm,
										p_fp_txt,
										p_fp_csv, 
										p_fp_xop_prm,
										p_fit_file_path);
		error = error_checking (error);
		if (error != ERR_NO_ERROR)
		{
			return (error);
		}
		field_index ++;
	
	}
	if (feof(*p_fp_csv))
	{
		*p_end_of_csv = 1;
		rewind(*p_fp_csv);
		error = enter_telescope_info (	p_fov_size, 
										p_resolution,
										p_max_mag_diff,
										p_max_col_diff,
										p_fp_prm);
		error = error_checking (error);
		if (error != ERR_NO_ERROR)
		{
			return (error);
		}
		if (fwrite (&field_index, sizeof (long int), 1, *p_fp_prm) != 1)
		{
			return (ERR_WRITE_FAILURE);
		}
		printf("\nThis parameter file closed early.  Only %ld entries in this file", field_index);
	}
	
	return (error);
	
}


int create_prm (	char *p_prm_file_path, 
					char *p_fit_file_path,
					FILE **p_fp_csv, 
					long int *p_num_targets,
					long int *p_csv_index,
					bool *p_end_of_csv)
{
	int error = ERR_NO_ERROR;
	
	// creates an index for going through the PRM files/entries in the csv file
	long int prm_index = 0;
	

	
	//sets up a character pointer to store the filenames as
	//an array up to 150 characters long
	char *file_name = (char *) malloc (FILE_NAME_SIZE *sizeof (char));
	char *txt_name = (char *) malloc (FILE_NAME_SIZE *sizeof (char));
	
	//creates a name tag for the end number of the file
	long int end_number = (*p_csv_index+*p_num_targets);
	
	
	//creates the string of the file name of the PRM file needed, using indices
	sprintf (file_name, "%s_%ld_%ld.prm", p_prm_file_path, *p_csv_index, end_number) ;
	
	printf("\n\nCreating a prm file named %s", file_name);

	
	//creates a pointer for a the prm file to be created
	FILE *fp_prm;
	FILE **p_fp_prm = & fp_prm;
	
	*p_fp_prm = fopen(file_name, "wb");
	
	
	if	(*p_fp_prm ==NULL)
	{
		printf ("\nCannot open prm file %s", file_name);
		return (ERR_CANNOT_OPEN);
	}
	
	//creates the string of the file name of the PRM file needed, using indices
	sprintf (txt_name, "%s_%ld_%ld.txt", p_prm_file_path, *p_csv_index, end_number) ;
	
	printf("\nCreating a txt file named %s", txt_name);

	//creates a pointer for a the prm file to be created
	FILE *fp_txt;
	FILE **p_fp_txt = & fp_txt;
	
	*p_fp_txt = fopen(txt_name, "w");
	
	
	if	(*p_fp_txt ==NULL)
	{
		printf ("\nCannot open prm file %s", txt_name);
		return (ERR_CANNOT_OPEN);
	}
	
	
	printf("\nWriting to file.");
	if (fwrite (p_num_targets, sizeof (long int), 1, *p_fp_prm) != 1)
	{
		printf ("\nCannot write to prm file %s", file_name);
		return (ERR_WRITE_FAILURE);
	}
	printf("\n%ld", *p_num_targets);
	fprintf(*p_fp_txt, "\n%ld", *p_num_targets);
	
	
	while ( prm_index < *p_num_targets && (!feof(*p_fp_csv)) )
	{
		
		error = enter_file_name (file_name, p_fp_csv, p_fp_prm, p_fit_file_path);
		error = error_checking (error);
		if (error != ERR_NO_ERROR)
		{
			return (error);
		}
		prm_index ++;
		printf("\n%s", file_name);
		fprintf(*p_fp_txt, "\n%s", file_name);
	}
	if (feof(*p_fp_csv))
	{
		*p_end_of_csv = 1;
		rewind(*p_fp_prm);
		if (fwrite (&prm_index, sizeof (long int), 1, *p_fp_prm) != 1)
		{
			return (ERR_WRITE_FAILURE);
		}
		printf("\nThis parameter file closed early.  Only %ld entries in this file", prm_index);
	}
	
	fclose (*p_fp_prm);
	fclose (*p_fp_txt);
	
	//free (file_name) ;
	//free (txt_name);
	
	return (error);
	
}


int enter_telescope_info (	double *p_fov_size,
							double *p_resolution,
							double *p_max_mag_diff,
							double *p_max_col_diff,
							FILE **p_fp_prm)
{
	int error = ERR_NO_ERROR;
	//this function takes writes the "telescope" parameters into the ppr file.  
	//note that id does not write to the text file.
	
	//writes in the field of view size parameter
	if (fwrite (p_fov_size, sizeof (double), 1, *p_fp_prm) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	
	printf ("\n%lf", *p_fov_size);
	
	//writes in the resolutio (angular separation) parameter
	if (fwrite (p_resolution, sizeof (double), 1, *p_fp_prm) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	
	printf ("\t%lf", *p_resolution);
	
	//writes the maximum magnitude separation permitted for a star to be accepted
	if (fwrite (p_max_mag_diff, sizeof (double), 1, *p_fp_prm) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	
	printf ("\t%lf", *p_max_mag_diff);
	//writes the maximum colour separation permitted for a star to be accepted
	if (fwrite (p_max_col_diff, sizeof (double), 1, *p_fp_prm) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	
	printf ("\t%lf", *p_max_col_diff);
	
	return (error);
}

int enter_field_info ( 	FILE **p_fp_prm,
						FILE **p_fp_txt,
						FILE **p_fp_csv, 
						char *p_fit_file_path)
{
	int error = ERR_NO_ERROR;
	
	//sets up a character pointer to store the filenames as
	//an array up to 100 characters long
	char *file_name = (char *) malloc (FILE_NAME_SIZE *sizeof (char));
	
	//sets up a variable to hold the number of files 
	//to be listed
	long int num_files = 0;
	long int file_index = 0;
	
	//sets up an index for writing the name of the file to the ppr
	long int name_index = 0;
	
	//sets up a variable to hold the number of targets in a given field
	//note that in the current version of the software, this will always be 1
	long int num_targets = 1;
	
	
	//sets up variables to hold the field descriptors
	//and target information
	long int specObjID = 0;
	long int run = 0;
	long int rerun = 0;
	long int camcol= 0;
	long int field = 0;
	struct star *target= (struct star *) malloc (sizeof (struct star));
	target -> ra = 0;
	target -> dec = 0;
	target -> mag[0] = 0;
	target -> mag[1] = 0;
	target -> mag[2] = 0;
	target -> mag[3] = 0;
	target -> mag[4] = 0;
	target -> rating = 0;
	
	if (fscanf (*p_fp_csv, "%ld,%ld,%ld,%ld,%ld,%ld,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
							&specObjID,
							&num_files,
							&run, 
							&rerun, 
							&camcol, 
							&field,
							&target -> ra,
							&target -> dec,
							&target -> mag[0],
							&target -> mag[1],
							&target -> mag[2],
							&target -> mag[3],
							&target -> mag[4])==13)
					/*if (fscanf (*p_fp_csv, "%ld,%ld,%ld,%ld,%ld,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
								//&specObjID,
								&num_files,
								&run, 
								&rerun, 
								&camcol, 
								&field,
								&target -> ra,
								&target -> dec,
								&target -> u_mag,
								&target -> g_mag,
								&target -> r_mag,
								&target -> i_mag,
								&target -> z_mag)==12)*/
					
	{
		//writes the number of files needed for the target
		if (fwrite ((&num_files), sizeof (long int), 1, *p_fp_prm) != 1)
		{
			return (ERR_WRITE_FAILURE);
		}
		
		printf("\n%ld", num_files);
		
		
		//makes a string containing the file name of the first fit file
		sprintf(file_name, "%s/%ld/%ld/calibChunks/%ld/tsObj-%06ld-%ld-%ld-%04ld.fit", p_fit_file_path, run, rerun, camcol, run, camcol, rerun, field);
		//sprintf(file_name, "%s/tsObj-%06ld-%ld-%ld-%04ld.fit", p_fit_file_path, run, camcol, rerun, field);
		
		//and writes it into the ppr file
		for (name_index = 0; name_index < FILE_NAME_SIZE; name_index ++)
		{
			if (fwrite ((file_name +name_index), sizeof (char), 1, *p_fp_prm) != 1)
			{
				return (ERR_WRITE_FAILURE);
			}
		}

		fprintf(*p_fp_txt, "\n%s", file_name);
		printf("\n%s", file_name);
		
		//reads the second and subsequent entries into the file
		for (file_index = 1; file_index < num_files; file_index ++)
		{
			//reads in from the CSV file the details it needs for the fields
			if (fscanf (*p_fp_csv, "%ld,%ld,%ld,%ld,%ld,%ld,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
						&specObjID,
						&num_files,
						&run, 
						&rerun, 
						&camcol, 
						&field,
						&target -> ra,
						&target -> dec,
						&target -> mag[0],
						&target -> mag[1],
						&target -> mag[2],
						&target -> mag[3],
						&target -> mag[4])==13)
				/*if (fscanf (*p_fp_csv, "%ld,%ld,%ld,%ld,%ld,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
							//&specObjID,
							&num_files,
							&run, 
							&rerun, 
							&camcol, 
							&field,
							&target -> ra,
							&target -> dec,
							&target -> u_mag,
							&target -> g_mag,
							&target -> r_mag,
							&target -> i_mag,
							&target -> z_mag)==12)*/
				{
					//makes a string containing the file name of the next fit file
					sprintf(file_name, "%s/%ld/%ld/calibChunks/%ld/tsObj-%06ld-%ld-%ld-%04ld.fit", p_fit_file_path, run, rerun, camcol, run, camcol, rerun, field);
					//sprintf(file_name, "%s/tsObj-%06ld-%ld-%ld-%04ld.fit", p_fit_file_path, run, camcol, rerun, field);
				
					//and writes it into the ppr file
					for (name_index = 0; name_index < FILE_NAME_SIZE; name_index ++)
					{
						if (fwrite ((file_name +name_index), sizeof (char), 1, *p_fp_prm) != 1)
						{
							return (ERR_WRITE_FAILURE);
						}
					}

					fprintf(*p_fp_txt, "\n%s", file_name);
					printf("\n%s", file_name);
				
				}
			else
			{
				printf("The csv file was not read appropriately");
				return (ERR_FILE_READ);
			}

		}

		//writes in the "number of targets in this field" 
		//which for the moment will be one.
		if (fwrite ((&num_targets), sizeof (long int), 1, *p_fp_prm) != 1)
		{
			return (ERR_WRITE_FAILURE);
		}
		printf("\n%ld", num_targets);
		
		if (fwrite ((target), sizeof (struct star), 1, *p_fp_prm) != 1)
		{
			return (ERR_WRITE_FAILURE);
		}
		printf("\n%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t",
				target->ra,
				target->dec,
				target->mag[0],
				target->mag[1],
				target->mag[2],
				target->mag[3],
				target->mag[4]);
		/*if (fwrite ((specObjID), sizeof (long int), 1, *p_fp_prm) != 1)
		{
			return (ERR_WRITE_FAILURE);
		}
		printf("\n%ld", specObjID);
		*/
	}
	else
	{
		printf("The csv file was not read appropriately");
		return (ERR_FILE_READ);
	}

	return (error);
	
}



int enter_xop_field_info ( 	FILE **p_fp_prm,
						FILE **p_fp_txt,
						FILE **p_fp_csv, 
						FILE **p_fp_xop_prm,
						char *p_fit_file_path)
{
	int error = ERR_NO_ERROR;
	
	//sets up a character pointer to store the filenames as
	//an array up to 100 characters long
	char *file_name = (char *) malloc (FILE_NAME_SIZE *sizeof (char));

	
	//sets up a character pointer to store the filenames as
	//an array up to 100 characters long
	char *target_file_name = (char *) malloc (FILE_NAME_SIZE *sizeof (char));
	
	//sets up a variable to hold the number of files 
	//to be listed
	long int num_files = 0;
	long int file_index = 0;
	
	//sets up an index for writing the name of the file to the ppr
	long int name_index = 0;
	
	//sets up a variable to hold the number of targets in a given field
	long int num_targets = 1;
	long int *p_num_targets = &num_targets;
	
	//sets up a variable to hold the number of targets in a given field
	long int num_fields = 1;
	long int *p_num_fields = &num_fields;	
	//sets up a variable to hold the number of targets in a given field
	long int field_number= 1;
	long int *p_field_number = &field_number;
	
	//sets up variables to hold the field descriptors
	//and target information
	long int row = 0;
	long int run = 0;
	long int rerun = 0;
	long int camcol= 0;
	long int field = 0;
	long int target_run = 0;
	long int target_rerun = 0;
	long int target_camcol= 0;
	long int target_field = 0;
	struct star *star_array= (struct star *) malloc (num_targets * sizeof (struct star));
	struct star **p_star_array  = &star_array;
	star_array -> ra = 0;
	star_array -> dec = 0;
	star_array -> mag[0] = 0;
	star_array -> mag[1] = 0;
	star_array -> mag[2] = 0;
	star_array -> mag[3] = 0;
	star_array -> mag[4] = 0;
	star_array -> rating = 0;
	
	int star_index = 0;
	
	struct star *target= (struct star *) malloc (sizeof (struct star));
	target -> ra = 0;
	target -> dec = 0;
	target -> mag[0] = 0;
	target -> mag[1] = 0;
	target -> mag[2] = 0;
	target -> mag[3] = 0;
	target -> mag[4] = 0;
	target -> rating = 0;
	
	if (fscanf (*p_fp_csv, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
				p_num_fields,
				p_field_number,
				&row,
				&run,
				&rerun, 
				&camcol, 
				&field,
				&num_files,
				&target_run,
				&target_rerun,
				&target_camcol,
				&target_field)==12	)
		
					
	{
		//writes the number of files needed for the target
		if (fwrite ((&num_files), sizeof (long int), 1, *p_fp_prm) != 1)
		{
			return (ERR_WRITE_FAILURE);
		}
		
		printf("\n%ld", num_files);
		
		
		//makes a string containing the file name of the first fit file
		sprintf(file_name, "%s/%ld/%ld/calibChunks/%ld/tsObj-%06ld-%ld-%ld-%04ld.fit", p_fit_file_path, run, rerun, camcol, run, camcol, rerun, field);
		//sprintf(file_name, "%s/tsObj-%06ld-%ld-%ld-%04ld.fit", p_fit_file_path, run, camcol, rerun, field);
		
		//and writes it into the ppr file
		for (name_index = 0; name_index < FILE_NAME_SIZE; name_index ++)
		{
			if (fwrite ((file_name +name_index), sizeof (char), 1, *p_fp_prm) != 1)
			{
				return (ERR_WRITE_FAILURE);
			}
		}

		fprintf(*p_fp_txt, "\n%s", file_name);
		printf("\n%s", file_name);
		
		//reads the second and subsequent entries into the file
		for (file_index = 1; file_index < num_files; file_index ++)
		{
			//reads in from the CSV file the details it needs for the fields
			
			if (fscanf (*p_fp_csv, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
						&row,
						&run,
						&rerun, 
						&camcol, 
						&field,
						&num_files,
						&target_run,
						&target_rerun,
						&target_camcol,
						&target_field)==10)
				{
					//makes a string containing the file name of the next fit file
					sprintf(file_name, "%s/%ld/%ld/calibChunks/%ld/tsObj-%06ld-%ld-%ld-%04ld.fit", p_fit_file_path, run, rerun, camcol, run, camcol, rerun, field);
					//sprintf(file_name, "%s/tsObj-%06ld-%ld-%ld-%04ld.fit", p_fit_file_path, run, camcol, rerun, field);
				
					//and writes it into the ppr file
					for (name_index = 0; name_index < FILE_NAME_SIZE; name_index ++)
					{
						if (fwrite ((file_name +name_index), sizeof (char), 1, *p_fp_prm) != 1)
						{
							return (ERR_WRITE_FAILURE);
						}
					}

					fprintf(*p_fp_txt, "\n%s", file_name);
					printf("\n%s", file_name);
				
				}
			else
			{
				printf("The csv file was not read appropriately");
				return (ERR_FILE_READ);
			}

		}
		

		//makes a string containing the file name of the next fit file
		sprintf(target_file_name, "%s/%ld/%ld/calibChunks/%ld/tsObj-%06ld-%ld-%ld-%04ld.fit", 
					p_fit_file_path, 
					target_run, 
					target_rerun, 
					target_camcol, 
					target_run, 
					target_camcol, 
					target_rerun, 
					target_field);

		read(target_file_name, p_star_array, p_num_targets);
		

		//writes in the "number of targets in this field" 
		if (fwrite ((p_num_targets), sizeof (long int), 1, *p_fp_prm) != 1)
		{
			return (ERR_WRITE_FAILURE);
		}
		printf("\n%ld", *p_num_targets);
		
		if (fwrite ((*p_star_array), sizeof (struct star), *p_num_targets, *p_fp_prm) != 1)
		{
			return (ERR_WRITE_FAILURE);
		}
		for (star_index = 0; star_index < *p_num_targets; star_index ++)
		{
			
			printf("\n%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t",
					((*p_star_array)+star_index)->ra,
					((*p_star_array)+star_index)->dec,
					((*p_star_array)+star_index)->mag[0],
					((*p_star_array)+star_index)->mag[1],
					((*p_star_array)+star_index)->mag[2],
					((*p_star_array)+star_index)->mag[3],
					((*p_star_array)+star_index)->mag[4]);
		}
		/*if (fwrite ((specObjID), sizeof (long int), 1, *p_fp_prm) != 1)
		{
			return (ERR_WRITE_FAILURE);
		}
		printf("\n%ld", specObjID);
		*/
	}
	else
	{
		printf("The csv file was not read appropriately");
		return (ERR_FILE_READ);
	}

	return (error);
	
}

//this program reads in the informationfrom the supplied fits file
int read(char *file_name, struct star **p_input_array, long int *p_in_num_rows)
{
	//sets up error control and status monitoring routines
	int error = ERR_NO_ERROR;
	int status = 0;
	int *p_status = &status;
	
	
	
	fitsfile *p_input_fits;
	
	int row_index = 0;

	
	// sets up variables to contain the columns in which the 
	//relevant information can be found
	int ra_col = 1;
	int *p_ra_col = &ra_col;
	int dec_col = 2;
	int *p_dec_col = &dec_col;
	int mag_col = 3;
	int *p_mag_col = &mag_col;
	
	// will want a cti file for this too, I think.
	
	fits_open_table(&p_input_fits, file_name, READONLY, p_status);
	if (*p_status!=0)
	{
		//return(*p_status);
		return(ERR_FITS_ERROR);
	}
	
	fits_get_num_rows (p_input_fits, p_in_num_rows, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	//printf ("Number of rows checked: total %ld\n", *p_in_num_rows);
	
	*p_input_array = (struct star *) malloc  (*p_in_num_rows * sizeof (struct star));
	if (*p_input_array == NULL)
	{
		return (ERR_OUT_OF_MEMORY);
	}
	else
	{
		//printf ("\nInput memory allocation step succesfully completed\n");
	}

	
	//printf("\nStarting FITS read step.\n");
	for (row_index = 1; row_index <= *p_in_num_rows; row_index++)
	{
		//reads ra, dec and mag into the array
		
		
		fits_read_col (p_input_fits, TDOUBLE, *p_ra_col, row_index, 1, 1, 0, &(((*p_input_array)+row_index-1)->ra), 0, p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		printf("\n%lf", (((*p_input_array)+row_index-1)->ra));
				
		//gets dec
		fits_read_col (p_input_fits, TDOUBLE, *p_dec_col, row_index, 1, 1, 0, &(((*p_input_array)+row_index-1)->dec), 0, p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		printf("\t%lf", (((*p_input_array)+row_index-1)->dec));
		
		//gets mag
		fits_read_col (p_input_fits, TDOUBLE, *p_mag_col, row_index, 1, NUM_COLOURS, 0, &(((*p_input_array)+row_index-1)->mag), 0, p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		printf("\t%lf\t%lf\t%lf\t%lf\t%lf", 
				(((*p_input_array)+row_index-1)->mag[0]), 
				(((*p_input_array)+row_index-1)->mag[1]), 
				(((*p_input_array)+row_index-1)->mag[2]), 
				(((*p_input_array)+row_index-1)->mag[3]), 
				(((*p_input_array)+row_index-1)->mag[4]));
		
		//initialises rating
		((*p_input_array)+row_index-1)->rating = 1;
		
		
	}	
	
	fits_close_file (p_input_fits, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	
	//printf ("\nRead stage complete\n");
	
	return (error);
	
}

int enter_file_name (	char *file_name,
						FILE **p_fp_csv,
						FILE **p_fp_prm,
						char *p_fit_file_path)
{
	int name_index =0;
	
	//sets up variables to hold the field descriptors
	long int run = 0;
	long int rerun = 0;
	long int camcol= 0;
	long int field = 0;

	if (fscanf (*p_fp_csv, "%ld,%ld,%ld,%ld\n", &run, &rerun, &camcol, &field)==4)
	{

		sprintf(file_name, "%s/%ld/%ld/calibChunks/%ld/tsObj-%06ld-%ld-%ld-%04ld.fit", p_fit_file_path, run, rerun, camcol, run, camcol, rerun, field);
		//sprintf(file_name, "%s/tsObj-%06ld-%ld-%ld-%04ld.fit", p_fit_file_path, run, camcol, rerun, field);

		for (name_index = 0; name_index < FILE_NAME_SIZE; name_index ++)
		{
			if (fwrite ((file_name +name_index), sizeof (char), 1, *p_fp_prm) != 1)
			{
				return (ERR_WRITE_FAILURE);
			}
		}
		
	}

		
	return (ERR_NO_ERROR);
}

/*
int enter_target_info (	FILE **p_fp_parameters,
						long int target_index,
						long int field_index)
{
	//sets up double variables to store the target data
	double ra = 0;
	double dec = 0;
	double mag = 0;
	double *p_ra = &ra;
	double *p_dec = &dec;
	double *p_mag = &mag;
	
	//asks the user to enter the RA of the target
	printf ("\nPlease enter the RA of target #%ld of field #%ld in decimal degrees\t", target_index, field_index);
	scanf("%lf", p_ra);
	if (fwrite (p_ra, sizeof (double), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}

	// Asks the user to enter the Dec of the target 
	printf ("\nPlease enter the Dec of target #%ld of field #%ld in decimal degrees\t", target_index, field_index);
	scanf("%lf", p_dec);
	if (fwrite (p_dec, sizeof (double), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	
	// Asks the user to enter the Magnitude  of the target
	printf ("\nPlease enter the magnitude of target #%ld of field #%ld \t", target_index, field_index);
	scanf("%lf", p_mag);
	if (fwrite (p_mag, sizeof (double), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	


	return (ERR_NO_ERROR);
}
*/

int check_file_type(	char *file_name, 
						char *p_file_type)
{
	int error = ERR_NO_ERROR; 
	//creates a pointer to a string to be used to check the 
	//filename for output purposes
	char *fit_extension;// = (char *) malloc (10*sizeof(char));
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
	else if(!strncmp (dot, csv_extension, 5))
	{
		*p_file_type = 'c';
	}
	else
	{
		*p_file_type = 'o';
	}
	
	
	//free(fit_extension);
	
	//free(fits_extension);
	
	//free(ppr_extension);
	
	//free(prm_extension);
	
	//free(csv_extension);
	
	//free(dot);
	
	return (error);
}








