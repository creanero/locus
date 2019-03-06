//============================================================================
// Name        : parameterisation_4_1_.cpp
// Author      : Oisin Creaner	
// Version     : 4.11 
// Copyright   : (c) ITT Dublin, Oisin Creaner 11th June 2009
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


//includes the error reporting header file
#include "error_checking_4_1_1.h"

//includes the file name definitions file
#include "file_info_4_1.h"

using namespace std;

int check_arguments (int num_args, 
					char *arg_one, 
					FILE **p_fp_parameters,
					char *p_file_type);

int name_parameter_file (	FILE **p_fp_parameters, 
							char *p_file_type);

int create_ppr(FILE **p_fp_parameters);

int create_prm (FILE **p_fp_parameters);

int enter_telescope_info (FILE **p_fp_parameters);

int enter_field_info (	FILE **p_fp_parameters, 
						long int field_index);

int enter_file_name (	char *file_name, 
						FILE **p_fp_parameters, 
						long int file_index,
						long int field_index);

int enter_target_info (	FILE **p_fp_parameters,
						long int target_index,
						long int field_index);

int check_file_type(	char *file_name, 
						char *p_file_type);

int main(int argc, char *argv[]) 
{

	//sets up error handling variable
	int error = ERR_NO_ERROR;
	
	
	//creates a pointer for a file in which to store the information
	//provided by the user
	FILE *fp_parameters;
	FILE **p_fp_parameters = & fp_parameters;
	
	char file_type = 'o';
	char *p_file_type = &file_type;
	
	error = check_arguments (	argc, 
								argv[1], 
								p_fp_parameters, 
								p_file_type);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	if (*p_file_type == 'p')
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

		if (((*p_file_type =='p')||(*p_file_type =='a')))
		{
			if ((*p_fp_parameters = fopen(arg_one, "wb"))==NULL)
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
		else
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

int create_ppr(FILE **p_fp_parameters)
{
	int error = ERR_NO_ERROR;
	
	error = enter_telescope_info ( p_fp_parameters);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	
	//sets up a variable to hold the number of targets 
	//to be listed
	long int num_fields = 0;
	long int field_index = 0;
	
	//asks the user for the number of fields then writes it to the new file
	printf ("\nPlease enter the number of fields you want to submit to the job\t");
	scanf("%ld", &num_fields);
	if (fwrite (&num_fields, sizeof (long int), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	
	for (field_index = 0; field_index < num_fields; field_index ++)
	{
		error = enter_field_info ( p_fp_parameters, field_index);
		error = error_checking (error);
		if (error != ERR_NO_ERROR)
		{
			return (error);
		}
	}

	
	return (error);
	
}

int create_prm (FILE **p_fp_parameters)
{
	int error = ERR_NO_ERROR;
	
	//sets up a character pointer to store the filenames as
	//an array up to 100 characters long
	char *file_name = (char *) malloc (FILE_NAME_SIZE *sizeof (char));
	
	//sets up a variable to hold the number of files 
	//to be listed
	long int num_files = 0;
	long int file_index = 0;
	
	//asks the user for the number of files then writes it to the new file
	printf ("\nPlease enter the number of files you want to submit to the job\t");
	scanf("%ld", &num_files);
	if (fwrite (&num_files, sizeof (long int), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	for (file_index = 0; file_index < num_files; file_index ++)
	{
		error = enter_file_name (file_name, p_fp_parameters, file_index, 0);
	}
	
	
	free (file_name) ;
	
	return (error);
	
}

int enter_telescope_info (FILE **p_fp_parameters)
{
	int error = ERR_NO_ERROR;
	
	//sets up the variables to store the test parameter data
	double angular_size = 0;
	double angular_separation = 0;
	double magnitude_separation = 0;
	double *p_angular_size = &angular_size ;
	double *p_angular_separation = &angular_separation;
	double *p_magnitude_separation = &magnitude_separation;
	
	
	// Asks the user to enter the size of field of view of the telescope
	printf ("\nPlease enter the angular size of the field of view in decimal degrees\t");
	scanf("%lf", p_angular_size);
	if (fwrite (p_angular_size, sizeof (double), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	
	//Asks the user to enter the minumum angular separation between targets
	printf ("\nPlease enter the minimum angular separation of objects in the field of view in decimal degrees\t");
	scanf("%lf", p_angular_separation);
	if (fwrite (p_angular_separation, sizeof (double), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	
	//Asks the user to enter the maximum magnitude variance of the target
	printf ("\nPlease enter the maximum magnitude separation of objects to be considered viable references\t");
	scanf("%lf", p_magnitude_separation);
	if (fwrite (p_magnitude_separation, sizeof (double), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	
	return (error);
}

int enter_field_info ( FILE **p_fp_parameters,
						long int field_index)
{
	int error = ERR_NO_ERROR;
	
	//sets up a character pointer to store the filenames as
	//an array up to 100 characters long
	char *file_name = (char *) malloc (FILE_NAME_SIZE *sizeof (char));
	
	//sets up a variable to hold the number of files 
	//to be listed
	long int num_files = 0;
	long int file_index = 0;
	
	
	//sets up a variable to hold the number of targets 
	//to be listed
	long int num_targets = 0;
	long int target_index = 0;
	
	//asks the user for the number of files then writes it to the new file
	printf ("\nPlease enter the number of files you want to submit for field %ld\t", field_index);
	scanf("%ld", &num_files);
	if (fwrite (&num_files, sizeof (long int), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	
	for (file_index = 0; file_index < num_files; file_index ++)
	{
		error = enter_file_name (file_name, p_fp_parameters, file_index, field_index);
	}
	

	free (file_name) ;
	
	//asks the user for the number of files then writes it to the new file
	printf ("\nPlease enter the number of targets you want to submit for field %ld\t", field_index);
	scanf("%ld", &num_targets);
	if (fwrite (&num_targets, sizeof (long int), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	
	for (target_index = 0; target_index < num_targets; target_index ++)
	{	
		error = enter_target_info (p_fp_parameters, target_index, field_index);
		error = error_checking (error);
		if (error != ERR_NO_ERROR)
		{
			return (error);
		}
	}
	
	return (error);
	
}

int enter_file_name (	char *file_name, 
						FILE **p_fp_parameters, 
						long int file_index,
						long int field_index)
{
	int name_index =0;
	char file_type = 'o';
	char *p_file_type = &file_type;
	


	do
	{
		printf ("Please enter the full path of file #%ld of field #%ld", file_index, field_index);
		scanf ("%s", file_name);
		
		check_file_type (file_name, p_file_type);
		
		
		if (file_type == 'f')
		{
			for (name_index = 0; name_index < FILE_NAME_SIZE; name_index ++)
			{
				if (fwrite ((file_name +name_index), sizeof (char), 1, *p_fp_parameters) != 1)
				{
					return (ERR_WRITE_FAILURE);
				}
			}
		}
		else 
		{
			printf ("\nThat is not a valid fit file!");
			*p_file_type = 'o';
		}
	}
	while (!(file_type =='f'));
		
	return (ERR_NO_ERROR);
}


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








