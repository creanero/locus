//============================================================================
// Name        : parameter_reader_4_0.cpp
// Author      : Oisin Creaner
// Version     : 
// Copyright   : Your copyright notice
// Description : Program to read the information from a parameter file  
//============================================================================

#include <iostream>

// Required header files
#include <stdio.h>
#include <stdlib.h>

#include <iostream>


//includes the error reporting header file
#include "error_checking_1_1.h"

#include "file_info_3_0.h"

using namespace std;


int read_target_info (	double *p_ra, 
						double *p_dec,
						double *p_mag,
						double *p_angular_size,
						double *p_angular_separation,
						double *p_magnitude_separation,
						FILE **p_fp_parameters);

int enter_file_name (	FILE **p_fp_parameters,
						char *file_name);

int read_file_names (	int *p_num_files, 
						int *p_file_index, 
						char *file_name, 
						FILE **p_fp_parameters);


int main() {
	
	//sets up error handling variable
	int error = ERR_NO_ERROR;
	
	
	//sets up a variable to hold the number of files 
	//to be listed
	int num_files = 0;
	int file_index = 0;
	int *p_num_files = &num_files ;
	int *p_file_index = &file_index;
	
	//sets up double variables to store the target data
	double ra = 0;
	double dec = 0;
	double mag = 0;
	double *p_ra = &ra;
	double *p_dec = &dec;
	double *p_mag = &mag;
	
	
	//sets up the variables to store the test parameter data
	double angular_size = 0;
	double angular_separation = 0;
	double magnitude_separation = 0;
	double *p_angular_size = &angular_size;
	double *p_angular_separation = &angular_separation;
	double *p_magnitude_separation = &magnitude_separation;
	
	
	//sets up a character pointer to store the filenames as
	//an array up to 100 characters long
	char *file_name = (char *) malloc (FILE_NAME_SIZE *sizeof (char));
	
	
	//creates a pointer for a file in which to store the information
	//provided by the user
	FILE *fp_parameters;
	FILE **p_fp_parameters = & fp_parameters;

	error= enter_file_name (p_fp_parameters, file_name);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	
	error = read_target_info (p_ra, p_dec, p_mag, p_angular_size, p_angular_separation, p_magnitude_separation, p_fp_parameters);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	error = read_file_names (p_num_files, p_file_index, file_name, p_fp_parameters);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	return (error);
}

int enter_file_name (	FILE **p_fp_parameters,
						char *file_name)
{
	int file_name_flag = 0;
	
	//Asks the user for the name of the file to be created
	do
	{
		printf ("\nPlease Enter the full path of the parameter file you wish to open\t");
		scanf ("%s", &file_name [0]);
		if ((*p_fp_parameters = fopen(&file_name[0], "rb"))==NULL)
			{
				printf ("\nThat is not a valid selection! Please try again!");
				file_name_flag = 0;		
			}
		
		else
		{
			printf("\n\nFile accepted.\n file path is\t %s", file_name);
			file_name_flag = 1;
		}
	}
	while (file_name_flag == 0);
	return (ERR_NO_ERROR);
}

int read_target_info (	double *p_ra, 
						double *p_dec,
						double *p_mag,
						double *p_angular_size,
						double *p_angular_separation,
						double *p_magnitude_separation,
						FILE **p_fp_parameters)
{
	//Reads in the target RA and prints it to the screen
	if (fread (p_ra, sizeof (double), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	printf ("\nThe target Right Ascension is %lf decimal degrees", *p_ra);

	//Reads in the target Declination and prints it to the screen
	if (fread (p_dec, sizeof (double), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	printf ("\nThe target declination is %lf decimal degrees", *p_dec);
	
	//Reads in the target magnitude and prints it to the screen
	if (fread (p_mag, sizeof (double), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	printf ("\nThe target magnitude is %lf", *p_mag);
	
	//Reads in the angular size of the field of view and prints it to the screen
	if (fread (p_angular_size, sizeof (double), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	printf ("\nThe the size of the field of view is %lf decimal degrees", *p_angular_size);

	
	//Reads in the angular separation and prints it to the screen
	if (fread (p_angular_separation, sizeof (double), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	printf ("\nThe minimum angular separation allowed for a target object is %lf decimal degrees", *p_angular_separation);

	
	//reads in the magnitude separation and prints it to the screen
	if (fread (p_magnitude_separation, sizeof (double), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	printf ("\nThe mangitude separation allowed for a target object is %lf", *p_magnitude_separation);
	
	return (ERR_NO_ERROR);

}


int read_file_names (int *p_num_files, int *p_file_index, char *file_name, FILE **p_fp_parameters)
{
	int name_index = 0;
	
	if (fread (p_num_files, sizeof (int), 1, *p_fp_parameters) != 1)
	{
		return (ERR_WRITE_FAILURE);
	}
	printf ("\nThe number of files in the parameter set is %d", *p_num_files);
	
	for (*p_file_index = 0; *p_file_index < *p_num_files; *p_file_index ++)
	{
		for (name_index = 0; name_index < FILE_NAME_SIZE; name_index ++)
		{
			if (fread ((file_name +name_index), sizeof (char), 1, *p_fp_parameters) != 1)
			{
				return (ERR_WRITE_FAILURE);
			}
		}
		printf ("\nThe full path for file #%d is %s", *p_file_index, file_name);
		
	}
	
	return (ERR_NO_ERROR);
	
}






