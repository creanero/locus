//============================================================================
// Name        : locus_algorithm_2_0.cpp
// Author      : Oisin Creaner
// Date		   : 22nd September 2008
// Copyright   : (c) Oisin Creaner, ITT Dublin 2008
// Description : Updated Locus algorithm for finding target 
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

//required for FITS file operations
#include <fitsio.h>

//Allows C++ style i/o
#include <iostream>

//allows for C++ string operations
#include <string.h>

//includes user-defined error checking routine
#include "error_checking_5_3.h"

//includes the file name description
#include "file_info_5_0.h"

//includes the structure definitions
#include "struct_definitions_5_2.h"

//includes the mathemathical library
#include <math.h>

// includes the time library for use as a metric 
#include <time.h>

//includes a header file for a set of minor useful functions
#include "minor_functions_5_0.h"

#include "quick_sort_5_0.h"



using namespace std;




//semi-standardised function used to read a _local.fit file with
//out_num_rows and RA, Dec and Mag information 
int read(char *file_name, 
		struct star **p_input_array, 
		long int *p_in_num_rows);

//core function of this program: eliminates non-viable candidate stars
int filter (	struct star **p_input_array, 
		long int *p_in_num_rows, 
		struct star **p_output_array, 
		long int *p_out_num_rows, 
		struct star **p_target,
		double *p_fov_size,
		double *p_resolution,
		double *p_max_mag_diff,
		double *p_max_col_diff,
		int colour_number);

//picks out stars that are too close to their neighbours
int sift (struct star **p_input_array, 
		long int *p_num_rows, 
		long int current_index, 
		double resolution, 
		double resolution_ra,
		int colour_number);

//semi-standardised function used to write a _local.fit file with
//out_num_rows and RA, Dec and Mag information 
int write(struct star *output_array, 
		long int *p_out_num_rows);

//finds and opens the parameters file and finds out what size it is
int get_parameters_file (FILE ** p_fp_parameters, int *p_num_files);

//makes a _local.fit file name from the existing .fit
int make_local_file_name(char *file_name, char *local_file_name);


int make_out_file_name(char *file_name, char *out_file_name);

//gets the file names needed at later stage
int get_file_name(char * file_name, FILE ** p_fp_parameters);


//standard read function as used in other programs
//int read(struct star **p_input_array, long int *p_in_num_rows);

//function that will calculate the boundaries for each star
int set_boundaries (
		struct star **p_star_array, 
		struct star **p_target_star,
		struct bounded_star **p_bounded_star_array, 
		long int *p_num_rows, 
		double radius,
		double ra_radius, 
		int colour_number);

//function that will find the intercept points.
int set_intercept (
		struct star **p_star_array, 
		struct star **p_target_star,
		struct bounded_star **p_bounded_star_array, 
		struct intercept **p_best_intercept,
		struct intercept **p_current_intercept,
		long int *p_num_rows,
		double radius,
		double ra_radius);

//function that checks each intersection against each star and
//determines a score for each
int check_intercept (
		struct intercept **p_best_intercept, 
		struct intercept **p_current_intercept, 
		struct star **p_star_array, 
		long int *p_num_rows,
		double radius,
		double ra_radius);

int list_comparison_stars (struct star *input_array, 
		long int *p_in_num_rows, 
		struct star **p_output_array, 
		long int *p_out_num_rows, 
		struct intercept **p_target);

//this is a modified version of an existing program that 
//writes a fits file output
int make_out_fits ( fitsfile **p_fp_out_fits, char *p_file_name);

int populate_output_array(	struct star **p_target, 
							struct intercept **p_best_intercept, 
							struct star **p_output_array,
							struct intercept **p_intercept_array,
							long int *p_out_num_rows);


int write_output_fits(	fitsfile **p_fp_out_fits, 
						struct star **p_output_array, 
						struct intercept **p_intercept_array,
						int colour_number,
						long int *p_total_targets);


/*int write(struct star *output_array, 
		long int *p_out_num_rows);
*/
int locus_main(struct star **p_filtered_array,
		long int *p_filtered_num_rows,
		struct star **p_target_star,
		struct intercept **p_best_intercept,
		double *p_fov_size,
		fitsfile **p_fp_out_fits,
		int colour_number,
		long int *p_total_targets);


int filter_main(	struct star **p_input_array, 
					long int *p_in_num_rows,
					struct star **p_target_star,
					struct star **p_filtered_array,
					long int *p_filtered_num_rows,
					double *p_fov_size,
					double *p_resolution,
					double *p_max_mag_diff,
					double *p_max_col_diff,
					int colour_number);


//checks the validity of argc and argv
int check_arguments (int num_args, 
					char *arg_one, 
					FILE **p_fp_parameters,
					char *p_file_type);

//checks which colour is being used
int check_colour (char *p_colour, int *p_colour_number);

//prompts the user to input the filename if none is supplied or 
//the one supplied is invalid 
int name_parameter_file (	FILE **p_fp_parameters, 
							char *p_file_type);


//checks the extension of a supplied file against those on a list.
int check_file_type(	char *file_name, 
						char *p_file_type);

//gets the telescope information
int get_telescope_parameters (	double *p_fov_size,
								double *p_resolution,
								double *p_max_mag_diff,
								double *p_max_col_diff,
								FILE **p_fp_parameters);

int make_array(	FILE **p_fp_parameters, 
				struct star **p_input_array, 
				long int *p_num_rows,
				long int *p_missing_file_count);

int get_target_info(	FILE **p_fp_parameters, struct star **p_target_star );

int scoring_mechanism(	struct star **p_input_array, 
						long int in_row_index,
						struct star **p_target, 
						double *p_max_mag_diff, 
						double *p_max_col_difference,
						int colour_number);

int colour_comparison (	struct star **p_input_array, 
						long int in_row_index,
						struct star **p_target,
						double *p_max_col_diff, 
						bool *p_colour_flag, 
						int colour_number);

int main (int argc, char *argv[])
{
	int error=ERR_NO_ERROR;
	int status =0;
	//creates a pointer for a file in which to store the information
	//provided by the user
	FILE *fp_parameters;
	FILE **p_fp_parameters = & fp_parameters;
	
	//creates a pointer for the file type
	char file_type = '0';
	char *p_file_type = &file_type;
	
	//creates a pointer to a file to store the output
	char *p_out_file_name= (char *) malloc (FILE_NAME_SIZE * sizeof(char));
	fitsfile *fp_out_fits;
	fitsfile **p_fp_out_fits = &fp_out_fits;

	
	//creates a set of variables to hold the contents of the ppr file
	//the number of targets in a field
	long int num_targets = 0;	
	//the size of a field
	double fov_size = 0;
	//the resolution of the telescope
	double resolution = 0;
	//the maximum permitted magnitude difference
	double max_mag_diff = 0;
	//the maximum permitted colour difference
	double max_col_diff = 0;
	//the number of fields in the ppr file
	long int num_fields = 0;
	//the number of files to make up that field
	//long int num_files = 0;
	//the number of rows in the array to make up a field.  
	//This is initialised to 1 to allow later variables to also be initialised
	long int num_rows = 1;
	long int filtered_num_rows =0;
	long int out_num_rows = 0;
	
	//and their corresponding pointers
	//the number of targets in a field
	long int *p_num_targets = &num_targets;	
	//the size of a field
	double *p_fov_size = &fov_size;
	//the resolution of the telescope
	double *p_resolution = &resolution;
	//the maximum permitted colour difference
	double *p_max_mag_diff = &max_mag_diff;
	//the maximum permitted colour difference
	double *p_max_col_diff = &max_col_diff;
	//the number of fields in the ppr file
	long int *p_num_fields = &num_fields;
	//the number of rows in the array to make up a field.  
	//This is initialised to 1 to allow later variables to also be initialised
	long int *p_num_rows = &num_rows;
	long int *p_filtered_num_rows = &filtered_num_rows;
	long int *p_out_num_rows = &out_num_rows;
	
	//sets up a counter on the total number of rows in the output fits file
	long int total_targets = 0;
	long int *p_total_targets = & total_targets;
	
	
	//creates a variable to look at the colour chosen
	char colour = *argv[2];
	char *p_colour = &colour;
	int colour_number = 0;
	int *p_colour_number = &colour_number;
	
	//creates a variable to handle a count of missing files
	long int missing_file_count = 0;
	long int *p_missing_file_count = &missing_file_count; 
	
	//creates a variable to handle a count of incomplete fields
	long int incomplete_array_count = 0;
	long int *p_incomplete_array_count = &incomplete_array_count; 
	
	
	//creates an index for the number of fields
	long int field_index =0;
	//creates an index for the number of targets
	long int target_index =0;
	
	//Sets the number of rows before mallocing the space for
	//the array.  Note that the READ function mallocs the 
	//space needed anyway, this step is only needed to set up 
	//the double pointer.
		
	struct star *star_array= (struct star *) malloc (num_rows * sizeof (struct star));
	struct star **p_star_array  = &star_array;
	struct star *filtered_array= (struct star *) malloc (num_rows * sizeof (struct star));
	struct star **p_filtered_array  = &filtered_array;
	struct star *output_array= {NULL};
	struct star **p_output_array  = &output_array;

	struct intercept *intercept_array= {NULL};
	struct intercept **p_intercept_array  = &intercept_array;
	int mag_index = 0;
	
	//initialises the variables
	*(p_output_array) = (struct star*) malloc ((1)* sizeof(struct star));
	(*(p_output_array))->ra = 0;
	(*(p_output_array))->dec = 0;
	for(mag_index = 0; mag_index <  NUM_COLOURS; mag_index ++)
	{
		(*(p_output_array))->mag [mag_index] = 0;
	}
	(*(p_output_array))->rating = 0;
	
	*(p_intercept_array) = (struct intercept*) malloc ((1)* sizeof(struct intercept));
	(*(p_intercept_array)) ->ra = 0;
	(*(p_intercept_array)) ->dec = 0;
	(*(p_intercept_array)) ->score = 0;
		

	
	//setus up the target star point using the defined terms 
	//in struct_definitions.  
	struct star *target_star= (struct star *) malloc (sizeof (struct star));
	struct star **p_target_star = &target_star;
	target_star -> ra = 0;
	target_star -> dec =0;
	target_star -> mag[0] = 0;
	target_star -> mag[1] = 0;
	target_star -> mag[2] = 0;
	target_star -> mag[3] = 0;
	target_star -> mag[4] = 0;
	

	
	//this sets up the variable that will be used to store the
	//best interception point after each comparison
	struct intercept *best_intercept = (struct intercept *) malloc (sizeof (struct intercept));
	struct intercept **p_best_intercept = &best_intercept;
	best_intercept -> ra = 0;
	best_intercept -> dec = 0;
	best_intercept -> score = 0;
	
	error = check_arguments (	argc,
								argv[1], 
								p_fp_parameters, 
								p_file_type);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	//if a colour has been specified
	if (argc == 3)
	{
		error = check_colour (p_colour, p_colour_number);
		if (error != ERR_NO_ERROR)
		{
			return (error);
		}
	}
	else //use a default colour
	{
		//picks "r" as the default colour
		*p_colour_number = 2;
	}
	
	//makes the name of the output file from the PPR file
	error = make_out_file_name (argv[1], p_out_file_name);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	//and makes the file
	error = make_out_fits (p_fp_out_fits, p_out_file_name);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	error = get_telescope_parameters (	p_fov_size,
										p_resolution,
										p_max_mag_diff,
										p_max_col_diff,
										p_fp_parameters);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	//reads in the number of fields 
	if (fread (p_num_fields, sizeof(long int), 1, *p_fp_parameters) != 1)
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

	printf("Number of fields in this file = %ld", *p_num_fields);

	for (field_index = 0; field_index < *p_num_fields; field_index ++)
	{
                if(feof(*p_fp_parameters))
                {
                        return (ERR_END_OF_FILE);
                }



		error = make_array(p_fp_parameters, p_star_array, p_num_rows, p_missing_file_count);
		if (error != ERR_NO_ERROR)
		{
			return (error);
		}
		
		if (*p_num_rows > 0)
		{
			//sorts the array input_array by ra (the first element of struct star
			qsort (*p_star_array, *p_num_rows, sizeof (struct star), star_compare);
		}
		
		
		
		//reads in the number of targets
		if (fread (p_num_targets, sizeof(long int), 1, *p_fp_parameters) != 1)
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
		for (target_index = 0; target_index < *p_num_targets; target_index ++)
		{
			error = get_target_info(p_fp_parameters, p_target_star);
			error = error_checking (error);
			if (error != ERR_NO_ERROR)
			{
				return (error);
			}
			
			if (*p_num_rows > 0)
			{
				error = filter_main(p_star_array,
									p_num_rows,
									p_target_star,
									p_filtered_array,
									p_filtered_num_rows,
									p_fov_size,
									p_resolution,
									p_max_mag_diff,
									p_max_col_diff,
									colour_number);
				error = error_checking (error);
				if (error != ERR_NO_ERROR)
				{
					return (error);
				}
				
				error = locus_main (p_filtered_array,
									p_filtered_num_rows,
									p_target_star,
									p_best_intercept,
									p_fov_size,
									p_fp_out_fits,
									colour_number,
									p_total_targets);
				error = error_checking (error);
				if (error != ERR_NO_ERROR)
				{
					return (error);
				}
			}

			else
			{
		        	best_intercept -> ra = 0;
	        		best_intercept -> dec = 0;
		       		best_intercept -> score = 0;

			}		
			error = populate_output_array(	p_target_star, 
											p_best_intercept, 
											p_output_array,  
											p_intercept_array,
											p_out_num_rows);
			error = error_checking (error);
			if (error != ERR_NO_ERROR)
			{
				return (error);
			}
			
			
			(*p_total_targets) ++;
			*p_filtered_num_rows = 0;
		}
	
		if(*p_missing_file_count > 0)
		{
			*p_incomplete_array_count = *p_incomplete_array_count +1;
			
		}
		
		*p_missing_file_count = 0;
		*p_num_rows = 0;
	
	}
	
	
	
	//runs the writing program that creates the output .fit
	//file for the user.
	error = write_output_fits (p_fp_out_fits, p_output_array, p_intercept_array, colour_number, p_out_num_rows);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	

	fits_close_file (*p_fp_out_fits, &status);
	
	if(*p_incomplete_array_count > 0)
	{
		printf("\nWARNING: some files not found.  \n%ld arrays were incomplete.", *p_incomplete_array_count );
		error = ERR_FILE_READ;
	}
	
	
	return (error);
	

}

int locus_main (	struct star **p_filtered_array,
					long int *p_filtered_num_rows,
					struct star **p_target_star,
					struct intercept **p_best_intercept,
					double *p_fov_size,
					fitsfile **p_fp_out_fits,
					int colour_number,
					long int *p_total_targets)
{
	int error = ERR_NO_ERROR;


	
	double radius = *p_fov_size/2; 
	double ra_radius = radius / absolute_value(cos((*p_target_star) ->dec * M_PI /180));
	

	//note that to allow the program to run, num_rows is
	//initialised to ONE, not ZERO
	//long int out_num_rows = 0;
	//long int *p_out_num_rows = & out_num_rows;
		
	//int row_index = 0;
	
	
	
	//this prints the RA and DEC radii.  It is a check step
	printf ("\nRadius %lf, Radius in RA: %lf", radius, ra_radius);
	
	

	
	//this sets up an array which will hold the boundary points 
	//for the star.  Note that the READ step must have taken place
	//before this step is executed beacuse otherwise insufficient 
	//memory will be allocated (num_rows will equal the initial
	//value
	struct bounded_star *bounded_star_array = (struct bounded_star *) malloc (*p_filtered_num_rows * sizeof (struct bounded_star));
	struct bounded_star **p_bounded_star_array = & bounded_star_array;
	
	
	//this sets up the variable that will be used to store the
	//temporary variable for an interception point used to compare each star with its neighbours
	struct intercept *current_intercept = (struct intercept *) malloc (sizeof (struct intercept));
	struct intercept **p_current_intercept =  &current_intercept;
	current_intercept -> ra = 0;
	current_intercept -> dec = 0;
	current_intercept -> score = 0;
	
	
	(*p_best_intercept) -> ra = 0;
        (*p_best_intercept) -> dec = 0;
        (*p_best_intercept) -> score = 0;



	
	//was used to randomly seed an array
	// ******************KEEP THIS for later use *******
	
	
	//for (row_index = 0; row_index <num_rows; row_index ++)
	//{
	//	(star_array + row_index )-> ra = TARGET_RA-((ra_radius*2)-((rand()+0.0)/RAND_MAX)*(ra_radius * 4));
	//	(star_array + row_index ) -> dec = TARGET_DEC-((radius *2)-((rand()+0.0)/RAND_MAX)*(radius*4));
	//	(star_array + row_index ) -> mag = ((rand()+0.0)/RAND_MAX) *10.0;
	//	printf("\n%lf\t%lf\t%lf",
	//			(star_array + row_index )-> ra ,
	//			(star_array + row_index )-> dec,
	//			(star_array + row_index )-> mag
	//			);
	//}
	

	//runs the program to create the array which sets boundaries 
	//on each star.
	//
	error = set_boundaries (
			p_filtered_array, 
			p_target_star, 
			p_bounded_star_array, 
			p_filtered_num_rows, 
			radius,
			ra_radius, 
			colour_number);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	//runs the program to make and check the interception
	//points between the boundary lines for each star
	error = set_intercept (
			p_filtered_array,
			p_target_star,
			p_bounded_star_array,
			p_best_intercept,
			p_current_intercept,
			p_filtered_num_rows,
			radius,
			ra_radius);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	
	//prints off a final score or a warning if this is unsuccesful
	if ((*p_best_intercept)->score > 0)
	{
		printf ("\n\nThe final best intercept point is ra: %lf, dec %lf, with rating %lf", 
				(*p_best_intercept) ->ra, 
				(*p_best_intercept) ->dec, 
				(*p_best_intercept) -> score);
	}
	else // no intercepts!
	{
		printf ("\n\nNO INTERCEPT FOUND!\n");
	}
	

	

	
	//These two functions have been commented out because they are not used
	/*error = list_comparison_stars (star_array, p_num_rows, p_output_array, p_out_num_rows, p_best_intercept);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}*/
	
	/*
	//runs the write function and checks it for errors
	error = write(output_array, p_out_num_rows);
	error = error_checking(error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}*/
	
	

	return (ERR_NO_ERROR);
}

//this program reads in the informationfrom the supplied fits file
/*int read(struct star **p_input_array, long int *p_in_num_rows)
{
	//sets up error control and status monitoring routines
	int error = ERR_NO_ERROR;
	int status = 0;
	int *p_status = &status;
	
	
	//variable that stores the fitsfile pointer
	fitsfile *p_input_fits;
	
	//control variable for dealing with the rows of the file
	long int row_index = 0;

	
	// sets up variables to contain the columns in which the 
	//relevant information can be found
	int ra_col = 1;
	int *p_ra_col = &ra_col;
	int dec_col = 2;
	int *p_dec_col = &dec_col;
	int mag_col = 3;
	int *p_mag_col = &mag_col;
	
	// will want a cti file for this too, I think.
	
	fits_open_table(&p_input_fits, FILTERED_FIT, READONLY, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	
	fits_get_num_rows (p_input_fits, p_in_num_rows, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("\nNumber of rows checked: total %ld\n", *p_in_num_rows);
	
	*p_input_array = (struct star *) malloc  (*p_in_num_rows * sizeof (struct star));
	if (*p_input_array == NULL)
	{
		return (ERR_OUT_OF_MEMORY);
	}
	else
	{
		printf ("\nInput memory allocation step succesfully completed\n");
	}

	
	printf("\nStarting FITS read step.\n");
	for (row_index = 1; row_index <= *p_in_num_rows; row_index++)
	{
		//reads ra, dec and mag into the array
		
		
		fits_read_col (p_input_fits, TDOUBLE, *p_ra_col, row_index, 1, 1, 0, &(((*p_input_array)+row_index-1)->ra), 0, p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		
				
		//gets dec
		fits_read_col (p_input_fits, TDOUBLE, *p_dec_col, row_index, 1, 1, 0, &(((*p_input_array)+row_index-1)->dec), 0, p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		
		//gets mag
		fits_read_col (p_input_fits, TDOUBLE, *p_mag_col, row_index, 1, 1, 0, &(((*p_input_array)+row_index-1)->mag), 0, p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
	}	
	
	fits_close_file (p_input_fits, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	
	printf ("\nRead stage complete\n");
	
	return (error);
	
}*/

int set_boundaries (
		struct star **p_star_array, 
		struct star **p_target_star,
		struct bounded_star **p_bounded_star_array, 
		long int *p_num_rows, 
		double radius,
		double ra_radius, 
		int colour_number)
{
	
	//sets an index value that will be used to control a for loop
	int star_index = 0;
	
	printf ("\n\nra\t\tdec\t\tmag\t\tb_ra\t\tb_ra_dir\tb_dec\t\tb_dec_dir\tmag_shorter\tmag_longer\tscore");
	for (star_index = 0; star_index < (*p_num_rows); star_index++)
	{
		//prints the basic details about the star before being run
		printf("\n%lf\t%lf\t%lf",
				((*p_star_array) + star_index)-> ra ,
				((*p_star_array) + star_index)-> dec,
				((*p_star_array) + star_index)-> mag[colour_number]
				);
	
		// if the comparison star is lower in ra than the target
		// put the boundary line above the comparison star with 
		// the line pointing "down"
		if (((*p_star_array) + star_index)-> ra < (*p_target_star) -> ra)
		{
			((*p_bounded_star_array) + star_index) -> bounded_ra = ((*p_star_array) + star_index)-> ra  + ra_radius;
			((*p_bounded_star_array) + star_index) -> bounded_ra_dir = 0;
		}
		// if the comparison star is higher in ra than the target
		// put the boundary line below the comparison star with 
		// the line pointing "up"
		else
		{
			((*p_bounded_star_array) + star_index) -> bounded_ra = ((*p_star_array) + star_index)-> ra  - ra_radius;
			((*p_bounded_star_array) + star_index) -> bounded_ra_dir = 1;
		}
	
		// if the comparison star is lower in ra than the target
		// put the boundary line above the comparison star with 
		// the line pointing "down"	
		if (((*p_star_array) + star_index)-> dec < (*p_target_star) -> dec)
		{
			((*p_bounded_star_array) + star_index) -> bounded_dec= ((*p_star_array) + star_index)-> dec  + radius;
			((*p_bounded_star_array) + star_index) -> bounded_dec_dir = 0;
		}
		// if the comparison star is higher in dec than the target
		// put the boundary line below the comparison star with 
		// the line pointing "up"
		else
		{
			((*p_bounded_star_array) + star_index) -> bounded_dec= ((*p_star_array) + star_index)-> dec  - radius;
			((*p_bounded_star_array) + star_index) -> bounded_dec_dir = 1;
		}
	
		
		//prints the components of the bounded_star array 
		printf("\t%lf\t%d\t\t%lf\t%d", 
				((*p_bounded_star_array) + star_index)  -> bounded_ra,  
				((*p_bounded_star_array) + star_index)  -> bounded_ra_dir,  
				((*p_bounded_star_array) + star_index)  -> bounded_dec,  
				((*p_bounded_star_array) + star_index)  -> bounded_dec_dir
				);
		
		if(colour_number == 0)
		{
			printf("\t\tno shorter\t%lf", ((*p_star_array) + star_index)->mag[colour_number +1]);
		}
		else if(colour_number == NUM_COLOURS -1)
		{
			printf("\t\t%lf\tno longer", ((*p_star_array) + star_index)->mag[colour_number -1]);
		}		
		else //if(colour_number in the middle)
		{
			printf("\t\t%lf\t%lf", ((*p_star_array) + star_index)->mag[colour_number -1], ((*p_star_array) + star_index)->mag[colour_number +1]);
		}
		printf("\t%lf", ((*p_star_array) + star_index)->rating);
		
	}
	
	
	return (ERR_NO_ERROR);
}


//USES THE BOUNDARY LINES TO FIND THE INTERCEPT POINTS OF THE LOCI

int set_intercept (
		struct star **p_star_array, 
		struct star **p_target_star,
		struct bounded_star **p_bounded_star_array, 
		struct intercept **p_best_intercept,
		struct intercept **p_current_intercept,
		long int *p_num_rows,
		double radius,
		double ra_radius)
{
	int one_index = 0;
	int two_index = 0;
	double soft_radius = radius +0.0001;
	double soft_ra_radius = ra_radius +0.0001;
	
	for (one_index = 0; one_index <(*p_num_rows - 1); one_index ++)
	{
		printf ("\nComparing star #%d", one_index);
		for (two_index = one_index+1; two_index < *p_num_rows; two_index ++)
		{
			printf ("\n\t\tWith star #%d", two_index);
			if ((*p_bounded_star_array + one_index) ->bounded_ra < (*p_bounded_star_array + two_index) ->bounded_ra)
			{
				if ((*p_bounded_star_array + one_index) ->bounded_dec < (*p_bounded_star_array + two_index) ->bounded_dec)
				{
					if ((*p_bounded_star_array + one_index) ->bounded_ra_dir ==1)
					{
						if ((*p_bounded_star_array + two_index) ->bounded_dec_dir == 0)
						{
							(*p_current_intercept) -> ra = (*p_bounded_star_array + two_index) ->bounded_ra; 
							(*p_current_intercept) -> dec = (*p_bounded_star_array + one_index) ->bounded_dec;
							(*p_current_intercept) -> score = 0;
							check_intercept (p_best_intercept, p_current_intercept, p_star_array, p_num_rows, soft_radius, soft_ra_radius);
						}
						else
						{
							// NO INTERCEPT
						}
					}
					//NOTE NOT "ELSE" AS CAN HAVE MULTIPLE INTERCEPTS!
					if ((*p_bounded_star_array + two_index) ->bounded_ra_dir == 0)
					{
						if ((*p_bounded_star_array + one_index) ->bounded_dec_dir == 1)
						{
							(*p_current_intercept) -> ra = (*p_bounded_star_array + one_index) ->bounded_ra; 
							(*p_current_intercept) -> dec = (*p_bounded_star_array + two_index) ->bounded_dec;
							(*p_current_intercept) -> score = 0;
							check_intercept (p_best_intercept, p_current_intercept, p_star_array, p_num_rows, soft_radius, soft_ra_radius);
						}
						else
						{
							//NO INTERCEPT
						}
					}
				}
				else // ((*p_bounded_star_array + one_index) ->bounded_dec > (*p_bounded_star_array + two_index) ->bounded_dec)
			
				{
					if ((*p_bounded_star_array + one_index) ->bounded_ra_dir ==1)
					{
						if ((*p_bounded_star_array + two_index) ->bounded_dec_dir == 1)
						{
							(*p_current_intercept) -> ra = (*p_bounded_star_array + two_index) ->bounded_ra; 
							(*p_current_intercept) -> dec = (*p_bounded_star_array + one_index) ->bounded_dec;
							(*p_current_intercept) -> score = 0;
							check_intercept (p_best_intercept, p_current_intercept, p_star_array, p_num_rows, soft_radius, soft_ra_radius);
						}
						else
						{
							// NO INTERCEPT
						}
					}
					//NOTE NOT "ELSE" AS CAN HAVE MULTIPLE INTERCEPTS!
					if ((*p_bounded_star_array + two_index) ->bounded_ra_dir == 0)
					{
						if ((*p_bounded_star_array + one_index) ->bounded_dec_dir == 0)
						{
							(*p_current_intercept) -> ra = (*p_bounded_star_array + one_index) ->bounded_ra; 
							(*p_current_intercept) -> dec = (*p_bounded_star_array + two_index) ->bounded_dec;
							(*p_current_intercept) -> score = 0;
							check_intercept (p_best_intercept, p_current_intercept, p_star_array, p_num_rows, soft_radius, soft_ra_radius);
						}
						else
						{
							//NO INTERCEPT
						}
					}
				}
			}
			
			
			else //((*p_bounded_star_array + one_index) ->bounded_ra > (*p_bounded_star_array + two_index) ->bounded_ra)
			{
				if ((*p_bounded_star_array + one_index) ->bounded_dec < (*p_bounded_star_array + two_index) ->bounded_dec)
				{
					if ((*p_bounded_star_array + one_index) ->bounded_ra_dir == 0)
					{
						if ((*p_bounded_star_array + two_index) ->bounded_dec_dir == 0)
						{
							(*p_current_intercept) -> ra = (*p_bounded_star_array + one_index) ->bounded_ra; 
							(*p_current_intercept) -> dec = (*p_bounded_star_array + two_index) ->bounded_dec;
							(*p_current_intercept) -> score = 0;
							check_intercept (p_best_intercept, p_current_intercept, p_star_array, p_num_rows, soft_radius, soft_ra_radius);
						}
						else
						{
							// NO INTERCEPT
						}
					}
					//NOTE NOT "ELSE" AS CAN HAVE MULTIPLE INTERCEPTS!
					if ((*p_bounded_star_array + two_index) ->bounded_ra_dir == 1)
					{
						if ((*p_bounded_star_array + one_index) ->bounded_dec_dir == 1)
						{
							(*p_current_intercept) -> ra = (*p_bounded_star_array + two_index) ->bounded_ra; 
							(*p_current_intercept) -> dec = (*p_bounded_star_array + one_index) ->bounded_dec;
							(*p_current_intercept) -> score = 0;
							check_intercept (p_best_intercept, p_current_intercept, p_star_array, p_num_rows, soft_radius, soft_ra_radius);
						}
						else
						{
							//NO INTERCEPT
						}
					}
				}
				else // ((*p_bounded_star_array + one_index) ->bounded_dec > (*p_bounded_star_array + two_index) ->bounded_dec)
			
				{
					if ((*p_bounded_star_array + one_index) ->bounded_ra_dir ==0)
					{
						if ((*p_bounded_star_array + two_index) ->bounded_dec_dir == 1)
						{
							(*p_current_intercept) -> ra = (*p_bounded_star_array + two_index) ->bounded_ra; 
							(*p_current_intercept) -> dec = (*p_bounded_star_array + one_index) ->bounded_dec;
							(*p_current_intercept) -> score = 0;
							check_intercept (p_best_intercept, p_current_intercept, p_star_array, p_num_rows, soft_radius, soft_ra_radius);
						}
						else
						{
							// NO INTERCEPT
						}
					}
					//NOTE NOT "ELSE" AS CAN HAVE MULTIPLE INTERCEPTS!
					if ((*p_bounded_star_array + two_index) ->bounded_ra_dir == 1)
					{
						if ((*p_bounded_star_array + one_index) ->bounded_dec_dir == 0)
						{
							(*p_current_intercept) -> ra = (*p_bounded_star_array + one_index) ->bounded_ra; 
							(*p_current_intercept) -> dec = (*p_bounded_star_array + two_index) ->bounded_dec;
							(*p_current_intercept) -> score = 0;
							check_intercept (p_best_intercept, p_current_intercept, p_star_array, p_num_rows, soft_radius, soft_ra_radius);
						}
						else
						{
							//NO INTERCEPT
						}
					}
				}
			}
		}
	}
	
	return (ERR_NO_ERROR);
}
// CHECKS THE INTERCEPT POINT TO GIVE IT A SCORE

int check_intercept (
		struct intercept **p_best_intercept, 
		struct intercept **p_current_intercept, 
		struct star **p_star_array, 
		long int *p_num_rows,
		double radius,
		double ra_radius)
{
	int star_number = 0;
	printf ("\tcurrent intercept: ra: %lf, dec: %lf",
			(*p_current_intercept) -> ra,
			(*p_current_intercept) -> dec);
	for(star_number = 0; star_number < *p_num_rows; star_number ++)
	{
		//if the absolute value of the distance between the
		//current intercept and a given target star is less
		//than the permitted radius, add to the score
		if (absolute_difference ((*p_current_intercept) -> ra, (*p_star_array + star_number)-> ra) < ra_radius
				&& absolute_difference ((*p_current_intercept) -> dec, (*p_star_array + star_number)-> dec) < radius)
		{
			//(*p_current_intercept) -> score ++;
			(*p_current_intercept) -> score = (*p_current_intercept) -> score +((*p_star_array + star_number)-> rating);
		}
		/*if ((*p_current_intercept) -> ra > (*p_star_array + star_number)-> ra)
		{
			if ((*p_current_intercept) -> dec > (*p_star_array + star_number)-> dec)
			{
				if ((((*p_current_intercept) -> ra - (*p_star_array + star_number)-> ra) < ra_radius) 
						&& (((*p_current_intercept) -> dec - (*p_star_array + star_number)-> dec) < radius))
				{
					(*p_current_intercept) -> score ++;
				}
			}
			else // ((*p_current_intercept) -> dec < (*p_star_array + star_number)-> dec)
			{
				if ((((*p_current_intercept) -> ra - (*p_star_array + star_number)-> ra) < ra_radius) 
						&& (-((*p_current_intercept) -> dec - (*p_star_array + star_number)-> dec) < radius))
				{
					(*p_current_intercept) -> score ++;
				}
			}
		}
		else //((*p_current_intercept) -> ra < (*p_star_array + star_number)-> ra)
		{

			if ((*p_current_intercept) -> dec > (*p_star_array + star_number)-> dec)
			{
				if ((-((*p_current_intercept) -> ra - (*p_star_array + star_number)-> ra) < ra_radius) 
						&& (((*p_current_intercept) -> dec - (*p_star_array + star_number)-> dec) < radius))
				{
					(*p_current_intercept) -> score ++;
				}
			}
			else // ((*p_current_intercept) -> dec < (*p_star_array + star_number)-> dec)
			{
				if ((-(((*p_current_intercept)) -> ra - (*p_star_array + star_number)-> ra) < ra_radius) 
						&& (-((*p_current_intercept) -> dec - (*p_star_array + star_number)-> dec) < radius))
				{
					(*p_current_intercept) -> score ++;
				}
			}
		}*/
	}	
	
	

	if (((*p_current_intercept) -> score )> ((*p_best_intercept) ->score))
	{
		(*p_best_intercept) -> ra = (*p_current_intercept) -> ra;
		(*p_best_intercept) -> dec = (*p_current_intercept) -> dec;
		(*p_best_intercept) -> score = (*p_current_intercept) -> score;
		printf ("\tNew best intercept: score: %lf",
				(*p_best_intercept) -> score);
	}
	else // not a new best
	{
		printf ("\tScore: %lf, best: %lf",	
				(*p_current_intercept) -> score,
				(*p_best_intercept)->score);
	}
	return (ERR_NO_ERROR);
}

//have commented out this function because it isn't needed
//can reintroduce or redevelop later
/*int list_comparison_stars (struct star *input_array, 
		long int *p_in_num_rows, 
		struct star **p_output_array, 
		long int *p_out_num_rows, 
		struct intercept **p_target)
{
	//sets up error handling routine
	int error = ERR_NO_ERROR;
	
	//sets up three indices to handle tranfers
	int in_row_index = 0;
	int temp_row_index = 0;
	int out_row_index = 0;
	
	
	double ra_distance = 0;
	double dec_distance = 0;
	double radius = RADIUS/2; //allows a radius of 10 arcminutes
	double ra_radius = radius /(2* absolute_value(cos((*p_target) ->dec * M_PI /180)));

	double soft_radius = radius +0.0001;
	double soft_ra_radius = ra_radius +0.0001;
	
	
	//creates a new array to hold up to the complete contents of the old one.
	struct star *temp_array;
	
	
	
	printf("\nStarting filter stage\n");
	
	//allocates memory to hold the new array
	temp_array = (struct star *) malloc  (*p_in_num_rows * sizeof (struct star));
	if (temp_array == NULL)
	{
		return (ERR_OUT_OF_MEMORY);
	}
	else
	{
		printf ("\nTemporary Memory allocation step succesfully completed\n");
	}
	

	
	printf ("\nNo.\tRA\t\tDec\t\tMag\t\tDistance\tAccepted?\n");
	for (in_row_index = 0; in_row_index < *p_in_num_rows; in_row_index++)
	{
		

		printf ("%d.\t%lf\t%lf\t%lf", 
				(in_row_index +1), 
				(input_array + in_row_index)->ra, 
				(input_array + in_row_index)->dec, 
				(input_array + in_row_index)->mag);

		
		//if the magnitude is in a range either side of the target
		//if //(((input_array + in_row_index)->mag > target ->mag - MAG_DIFFERENCE)
			//	&&(input_array + in_row_index)->mag < target ->mag + MAG_DIFFERENCE)
		
			//calculates the difference in RA in absolute terms
			ra_distance =  absolute_difference((input_array + in_row_index)->ra , (*p_target) ->ra);
				
			//calculates the difference in DEC in absolute terms
			dec_distance =  absolute_difference((input_array + in_row_index)->dec , (*p_target) ->dec);
			
			if ((ra_distance<soft_ra_radius)&&(dec_distance<soft_radius))
			{
				//printf("\t%lf",distance);
				printf("\tACCEPTED");
				
				//if all is well, adds the details from the input array into the temproary one
				(temp_array + temp_row_index) ->ra = (input_array + in_row_index)->ra;
				(temp_array + temp_row_index) ->dec = (input_array + in_row_index)->dec;
				(temp_array + temp_row_index) ->mag = (input_array + in_row_index)->mag;
				temp_row_index++;
				
				//increments the size the new array will be
				*p_out_num_rows = *p_out_num_rows +1;
				printf ("\t%ld\n", *p_out_num_rows);
			}
			else
			{
				//printf("\t%lf",distance);
				printf("\tREJECTED, out of FoV\n");
			}
		
	}
	
	
	//allocates only enough memory to hold the stars that have met the criteria
	if (*p_out_num_rows > 0)
	{
		*p_output_array = (struct star *) malloc  (*p_out_num_rows * sizeof (struct star));
		if (*p_output_array == NULL)
		{
			return (ERR_OUT_OF_MEMORY);
		}
		else
		{
			printf ("\nMemory allocation step for output array succesfully completed\n");
		}
		
		for (out_row_index = 0; out_row_index < *p_out_num_rows; out_row_index++)
			{
				//assigns the contents of the temporary array into the output one.
				(*p_output_array + out_row_index) ->ra = (temp_array + out_row_index)->ra;
				(*p_output_array + out_row_index) ->dec = (temp_array + out_row_index)->dec;
				(*p_output_array + out_row_index) ->mag = (temp_array + out_row_index)->mag;
			}
		}	
	else
	{
		return (ERR_NOT_FOUND);
	}
	
	free (temp_array);
	
	return (error);
}*/


//standardised finla writing stage
int make_out_fits ( fitsfile **p_fp_out_fits, char *p_file_name)
{
	int error = ERR_NO_ERROR;
	int status = 0;
	int *p_status = &status;
	
	
	//defines a number of columns and rows variable for checking purposes
	int num_cols = 0;
	int *p_num_cols = & num_cols;
	//long int num_rows = 0;
	//long int *p_num_rows= &num_rows;
	
	//declares an index for control of writing and 
	//column numbers the array is to write to
	//int row_index =1;
	//int new_ra_col = 1;
	int new_dec_col = 2;
	int new_mag_col = 3;
	int new_av_ra_col = 4;
	int new_av_dec_col = 5;
	int new_av_rating_col = 6;
	
	//creates name and format files for the new table
	char ra_name[NAME_SIZE] = "ra";
	char *p_ra_name = & ra_name[0];
	char dec_name[NAME_SIZE] = "dec";
	char *p_dec_name = & dec_name[0];
	char mag_name[NAME_SIZE] = "mag";
	char *p_mag_name = & mag_name[0];
	char av_ra_name[NAME_SIZE] = "av_ra";
	char *p_av_ra_name = & av_ra_name[0];
	char av_dec_name[NAME_SIZE] = "av_dec";
	char *p_av_dec_name = & av_dec_name[0];
	char av_rating_name[NAME_SIZE] = "rating";
	char *p_av_rating_name = & av_rating_name[0];
	char form[3] = "1D";
	char *p_form = &form[0];


	char form_mag[3] = "5D";
	char *p_form_mag = &form_mag[0];
	
	fits_create_file (p_fp_out_fits, p_file_name, p_status);
	if (*p_status!=0)
	{
		return (*p_status);
		//return(ERR_FITS_ERROR);
	}	
	
	fits_create_tbl (*p_fp_out_fits, BINARY_TBL, 0, 1, &p_ra_name, &p_form, NULL, NULL, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("\nNew FITS table created!\n");
	fits_insert_col (*p_fp_out_fits, new_dec_col, p_dec_name, p_form, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	fits_insert_col (*p_fp_out_fits, new_mag_col, p_mag_name, p_form_mag, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	fits_insert_col (*p_fp_out_fits, new_av_ra_col, p_av_ra_name, p_form, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	fits_insert_col (*p_fp_out_fits, new_av_dec_col, p_av_dec_name, p_form, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	fits_insert_col (*p_fp_out_fits, new_av_rating_col, p_av_rating_name, p_form, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}

	//confirms the number of columns in the file
	fits_get_num_cols (*p_fp_out_fits, p_num_cols, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("\nColumns set up.  Number of Columns is %d\n", *p_num_cols);

	return (error);
}



//this program puts data into the output array
int populate_output_array(	struct star **p_target, 
							struct intercept **p_best_intercept, 
							struct star **p_output_array,
							struct intercept **p_intercept_array,
							long int *p_out_num_rows)
{
	int error = ERR_NO_ERROR;

	
	//an index that is used to copy the old data into the new index
	long int copying_index = 0;
		
	//sets up a set of structures to hold the old arrays when the arriays are incremented
	struct star *output_array_2= {NULL};
	struct star **p_output_array_2 = &output_array_2;
	struct intercept *intercept_array_2= {NULL};
	struct intercept **p_intercept_array_2 = &intercept_array_2;
	
	int mag_index = 0;
	
	//initialises the variables
	*(p_output_array_2) = (struct star*) malloc ((1)* sizeof(struct star));
	(*(p_output_array_2))->ra = 0;
	(*(p_output_array_2))->dec = 0;
	for(mag_index = 0; mag_index <  NUM_COLOURS; mag_index ++)
	{
		(*(p_output_array_2))->mag [mag_index] = 0;
	}
	(*(p_output_array_2))->rating = 0;
	
	*(p_intercept_array_2) = (struct intercept*) malloc ((1)* sizeof(struct intercept));
	(*(p_intercept_array_2)) ->ra = 0;
	(*(p_intercept_array_2)) ->dec = 0;
	(*(p_intercept_array_2)) ->score = 0;
	
	*p_output_array_2 = *p_output_array;
	*p_intercept_array_2 = *p_intercept_array;
	
	//increments the number of rows
	*p_out_num_rows = (*p_out_num_rows) +1;

		
	*(p_output_array) = (struct star*) malloc ((*p_out_num_rows)* sizeof(struct star));
	*(p_intercept_array) = (struct intercept*) malloc ((*p_out_num_rows)* sizeof(struct intercept));
	
	for (copying_index = 0; copying_index<((*p_out_num_rows)-1); copying_index ++)
	{
		*(*(p_output_array) +copying_index) = *((*p_output_array_2) + copying_index);
		*(*(p_intercept_array) +copying_index) = *((*p_intercept_array_2) + copying_index);
	}

	*(*(p_output_array) +((*p_out_num_rows)-1)) = **p_target;
	*(*(p_intercept_array) +((*p_out_num_rows)-1)) = **p_best_intercept;
	

	
		

	return (error);
	
}






int write_output_fits(	fitsfile **p_fp_out_fits, 
						struct star **p_output_array, 
						struct intercept **p_intercept_array,
						int colour_number,
						long int *p_out_num_rows)

{
	int error = ERR_NO_ERROR;
	int status = 0;
	int *p_status = &status;
	
	
	/*//defines a number of columns and rows variable for checking purposes
	int num_cols = 0;
	int *p_num_cols = & num_cols;
	long int num_rows = 0;
	long int *p_num_rows= &num_rows;
	*/
	//declares an index for control of writing and 
	//column numbers the array is to write to
	long int output_index =0;
	long int row_index = output_index +1;
	int new_ra_col = 1;
	int new_dec_col = 2;
	int new_mag_col = 3;
	int new_av_ra_col = 4;
	int new_av_dec_col = 5;
	int new_av_rating_col = 6;
	
	int start_row = 0;
	


	//adds a row to the file (note that the row is added at the start, 
	//so the out file will be in reverse order to the input file
	fits_insert_rows(*p_fp_out_fits, start_row, *p_out_num_rows, p_status);
	//fits_insert_rows(*p_fp_out_fits, 0, 1, p_status);
	if (*p_status!=0)
	{
		if(*p_status == 202)
		{
			fits_report_error (stdout, error);
			*p_status = 0;
		}
		else
		{
			return(*p_status);
		}
		
	}
	/*//checks the number of rows and columns
	fits_get_num_rows (*p_fp_out_fits, p_num_rows, p_status);
	printf("\nNumber of Rows setup.  Number of Rows is %ld. \n", *p_num_rows);
	*/
	printf ("\nTgRA\t\tTgDec\t\tTgMag\t\tAvRA\t\tAvDec\t\tRating\n");
	
	for(output_index = 0 ; output_index < *p_out_num_rows ; output_index ++)
	{
		row_index = output_index +1;
		
		fits_write_col (*p_fp_out_fits, TDOUBLE, new_ra_col, row_index, 1, 1, &(((*p_output_array)+output_index)->ra), p_status);
		if (*p_status!=0)
		{
			if(*p_status == 202)
			{
				fits_report_error (stdout, error);
				*p_status = 0;
			}
			else
			{
				return(*p_status);
			}
			
		}
		printf ("%lf\t", ((*p_output_array)+output_index)->ra);
			
		fits_write_col (*p_fp_out_fits, TDOUBLE, new_dec_col, row_index, 1, 1, &(((*p_output_array)+output_index)->dec), p_status);
		if (*p_status!=0)
		{
			if(*p_status == 202)
			{
				fits_report_error (stdout, error);
				*p_status = 0;
			}
			else
			{
				return(*p_status);
			}
			
		}
		printf ("%lf\t", ((*p_output_array)+output_index)->dec);
		
		fits_write_col (*p_fp_out_fits, TDOUBLE, new_mag_col, row_index, 1, NUM_COLOURS, &(((*p_output_array)+output_index)->mag), p_status);
		if (*p_status!=0)
		{
			if(*p_status == 202)
			{
				fits_report_error (stdout, error);
				*p_status = 0;
			}
			else
			{
				return(*p_status);
			}
			
		}
		printf ("%lf\t",((*p_output_array)+output_index)->mag[colour_number]);
		
		fits_write_col (*p_fp_out_fits, TDOUBLE, new_av_ra_col, row_index, 1, 1, &(((*p_intercept_array)+output_index)->ra), p_status);
		if (*p_status!=0)
		{
			if(*p_status == 202)
			{
				fits_report_error (stdout, error);
				*p_status = 0;
			}
			else
			{
				return(*p_status);
			}
			
		}
		printf ("%lf\t", ((*p_intercept_array)+output_index)->ra);
			
		fits_write_col (*p_fp_out_fits, TDOUBLE, new_av_dec_col, row_index, 1, 1, &(((*p_intercept_array)+output_index)->dec), p_status);
		if (*p_status!=0)
		{
			if(*p_status == 202)
			{
				fits_report_error (stdout, error);
				*p_status = 0;
			}
			else
			{
				return(*p_status);
			}
			
		}
		printf ("%lf\t", ((*p_intercept_array)+output_index)->dec);
		fits_write_col (*p_fp_out_fits, TDOUBLE, new_av_rating_col, row_index, 1, 1, &(((*p_intercept_array)+output_index)->score), p_status);
		if (*p_status!=0)
		{
			if(*p_status == 202)
			{
				fits_report_error (stdout, error);
				*p_status = 0;
			}
			else
			{
				return(*p_status);
			}
			
		}
		printf ("%lf\n", ((*p_intercept_array)+output_index)->score);
	}

	return (error);
}
/*
int write(struct star *output_array, long int *p_out_num_rows)
{
	int error = ERR_NO_ERROR;
	int status = 0;
	int *p_status = &status;
	
	//creates the pointer to the fits file to be created
	fitsfile **p_fp_out_fits;// = (NULL);
	
	//defines a number of columns and rows variable for checking purposes
	int num_cols = 0;
	int *p_num_cols = & num_cols;
	long int num_rows = 0;
	long int *p_num_rows= &num_rows;
	
	//declares an index for control of writing and 
	//column numbers the array is to write to
	int row_index =0;
	int new_ra_col = 1;
	int new_dec_col = 2;
	int new_mag_col = 3;
	
	//creates name and format files for the new table
	char ra_name[NAME_SIZE] = "ra";
	char *p_ra_name = & ra_name[0];
	char dec_name[NAME_SIZE] = "dec";
	char *p_dec_name = & dec_name[0];
	char mag_name[NAME_SIZE] = "mag";
	char *p_mag_name = & mag_name[0];
	char form[3] = "1D";
	char *p_form = &form[0];
	
	if(*p_out_num_rows == 0)
	{
		return (ERR_NOT_FOUND);
	}
	//ffinit()
	fits_create_file (*p_fp_out_fits, FINAL_LIST_OW, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}	
	
	//ffcrtb();
	fits_create_tbl (*p_fp_out_fits, BINARY_TBL, 0, 1, &p_ra_name, &p_form, NULL, NULL, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("\nNew FITS table created!\n");
	fits_insert_col (*p_fp_out_fits, 2, p_dec_name, p_form, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	fits_insert_col (*p_fp_out_fits, 3, p_mag_name, p_form, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}

	//confirms the number of columns in the file
	fits_get_num_cols (*p_fp_out_fits, p_num_cols, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("\nColumns set up.  Number of Columns is %d\n", *p_num_cols);

	//sets up the rows
	fits_insert_rows(*p_fp_out_fits, 0, *p_out_num_rows, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	
	//checks the number of rows and columns
	fits_get_num_rows (*p_fp_out_fits, p_num_rows, p_status);
	printf("\nNumber of Rows setup.  Number of Rows is %ld. \n", *p_num_rows);
	
	printf ("\nNo.\tRA\t\tDec\t\tMag\t\n");
	for (row_index = 1; row_index <= *p_num_rows; row_index++)
	{
		printf ("%d.\t", (row_index));

		fits_write_col (*p_fp_out_fits, TDOUBLE, new_ra_col, row_index, 1, 1, &((output_array+row_index-1)->ra), p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		printf ("%lf\t", (output_array + row_index-1)->ra);
		
		
		
		fits_write_col (*p_fp_out_fits, TDOUBLE, new_dec_col, row_index, 1, 1, &((output_array+row_index-1)->dec), p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		printf ("%lf\t", (output_array + row_index-1)->dec);
		
		
		
		fits_write_col (*p_fp_out_fits, TDOUBLE, new_mag_col, row_index, 1, 1, &((output_array+row_index-1)->mag), p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		printf ("%lf\n",(output_array + row_index-1)->mag);
	}


	
	
	
	
	
	
	//closes the fitsfile
	fits_close_file (*p_fp_out_fits, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	
	printf ("\nWrite stage complete\n");
	return (error);
	
}
*/

int filter_main (	struct star **p_input_array, 
					long int *p_in_num_rows,
					struct star **p_target_star,
					struct star **p_filtered_array,
					long int *p_filtered_num_rows,
					double *p_fov_size,
					double *p_resolution,
					double *p_max_mag_diff,
					double *p_max_col_diff,
					int colour_number)
{
	
	
	//sets up error control and status monitoring routines
	int error = ERR_NO_ERROR;
	
		
	/*//sets up long ints to hold the number or rows in each array
	long int out_num_rows =0;
	long int *p_out_num_rows = & out_num_rows;
	long int num_rows_to_date = 0;
	long int *p_num_rows_to_date = &num_rows_to_date;
	
	
	
	
	*p_in_num_rows = *p_num_rows_to_date;
	*/
	
	printf("\nRead stage complete, calling filter stage");
	


	
	//runs the filter function and checks it for errors
	error = filter(	p_input_array, 
					p_in_num_rows, 
					p_filtered_array, 
					p_filtered_num_rows, 
					p_target_star,
					p_fov_size,
					p_resolution,
					p_max_mag_diff,
					p_max_col_diff,
					colour_number);
	error = error_checking(error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	

	
	
	
	return (error);
}


/*
int get_parameters_file (FILE ** p_fp_parameters, int *p_num_files)
{
	int error = ERR_NO_ERROR;
	//opens the parameters file

	if ((*p_fp_parameters = fopen(API_PARAMETERS, "rb"))==NULL)
	{
		return (ERR_CANNOT_OPEN);
	}
	else
	{
		printf ("\nparameters File opened.\n");
	}

	
	//reads in the number of files as parameters 
	if (fread (p_num_files, sizeof(int), 1, *p_fp_parameters) != 1)
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

*/










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




int make_local_file_name(char *file_name, char *local_file_name)
{
	int error = ERR_NO_ERROR;
	int name_index = 0;
	
	//creates a pointer to a string to be used to extend the 
	//filename for output purposes
	char *output_extension = (char *) malloc (11*sizeof(char));
	output_extension = "_local.fit";
	char *dot = (char *) malloc (2* sizeof (char));
	
	for (name_index = 0; name_index < FILE_NAME_SIZE; name_index ++)
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
int make_out_file_name(char *file_name, char *out_file_name)
{
	int error = ERR_NO_ERROR;
	int name_index = 0;
	
	//creates a pointer to a string to be used to extend the 
	//filename for output purposes
	char *output_extension = (char *) malloc (11*sizeof(char));
	output_extension = "_out.fit";
	char *dot = (char *) malloc (2* sizeof (char));
	char *ow_extension = (char *) malloc ((FILE_NAME_SIZE+2)* sizeof (char));
	//*ow_extension = '\\';
	*(ow_extension)= '!';
	*(ow_extension +1)= '\0';
	
	for (name_index = 0; name_index < FILE_NAME_SIZE; name_index ++)
	{
		*(out_file_name + name_index) = *(file_name + name_index);
	}
	//finds the "dot" in the file name so the 
	//filename can be changed for output
	dot =strchr (out_file_name, '.');
	*dot = '\0';
	
	//puts the _out.fit extension on the filename for output purposes
	strcat (out_file_name, output_extension);

	//puts the .cti extension on the filename for output purposes
	strcat (ow_extension, out_file_name);
	
	out_file_name = ow_extension;
	
	printf("\nFits File to be created is %s",out_file_name);
	
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


int filter (	struct star **p_input_array, 
				long int *p_in_num_rows, 
				struct star **p_output_array, 
				long int *p_out_num_rows, 
				struct star **p_target,
				double *p_fov_size,
				double *p_resolution,
				double *p_max_mag_diff,
				double *p_max_col_diff,
				int colour_number)
{
	//sets up error handling routine
	int error = ERR_NO_ERROR;
	
	//sets up three indices to handle tranfers
	long int in_row_index = 0;
	long int temp_row_index = 0;
	long int out_row_index = 0;
	
	
	double ra_distance = 0;
	double dec_distance = 0;
	double radius = *p_fov_size; 
	double ra_radius = radius / absolute_value(cos((*p_target) ->dec * M_PI /180));
	
	//creates a new array to hold up to the complete contents of the old one.
	struct star *temp_array;
	
	//sets up variables for the sifting stage
	double resolution = *p_resolution; 

	double resolution_ra = resolution/ absolute_value(cos((*p_target) ->dec * M_PI /180));
	

	
	//sets up a flag to monitor the colour
	bool colour_flag = 0;
	bool *p_colour_flag = & colour_flag;
	
	//nearly sure this is wrong
	//struct star **p_input_array = & input_array;
	
	
	printf("\nStarting filter stage\n");
	
	//allocates memory to hold the new array
	temp_array = (struct star *) malloc  (*p_in_num_rows * sizeof (struct star));
	if (temp_array == NULL)
	{
		return (ERR_OUT_OF_MEMORY);
	}
	else
	{
		printf ("\nTemporary Memory allocation step succesfully completed\n");
	}
	

	
	//printf ("\nNo.\tRA\t\tDec\t\tMag\t\tDistance\tAccepted?\n");
	for (in_row_index = 0; in_row_index < *p_in_num_rows; in_row_index++)
	{
		
		//runs the sifting algorithm
		error = sift (	p_input_array, 
						p_in_num_rows, 
						in_row_index, 
						resolution, 
						resolution_ra, 
						colour_number);
		if (error != ERR_NO_ERROR)
		{
			return (error);
		}
		/*
		printf ("%ld.\t%lf\t%lf\t%lf\t%lf\t%lf", 
				(in_row_index +1), 
				((*p_input_array) + in_row_index)->ra, 
				((*p_input_array) + in_row_index)->dec, 
				((*p_input_array) + in_row_index)->mag[colour_number -1],
				((*p_input_array) + in_row_index)->mag[colour_number],
				((*p_input_array) + in_row_index)->mag[colour_number +1]);
*/
		if (((*p_input_array) + in_row_index)->rating !=0)
		{
			//if the magnitude is in a range either side of the target
			if ((absolute_difference ((((*p_input_array) + in_row_index)->mag[colour_number]) , (*p_target) ->mag[colour_number]) < *p_max_mag_diff))
			{
				//calculates the difference in RA in absolute terms
				ra_distance =  absolute_difference(((*p_input_array) + in_row_index)->ra , (*p_target) ->ra);
					
				//calculates the difference in DEC in absolute terms
				dec_distance =  absolute_difference(((*p_input_array) + in_row_index)->dec , (*p_target) ->dec);
				
				if ((ra_distance<ra_radius)&&(dec_distance<radius))
				{
					
					colour_flag = 0;
					
					error = colour_comparison (	p_input_array,
												in_row_index, 
												p_target,  
												p_max_col_diff, 
												p_colour_flag,
												colour_number);
					error = error_checking(error);
					if (error != ERR_NO_ERROR)
					{
						return (error);
					}
					if (colour_flag != 0)
					{
						//printf("\t%lf",distance);
						
						error = scoring_mechanism(	p_input_array,
													in_row_index,  
													p_target, 
													p_max_mag_diff, 
													p_max_col_diff,
													colour_number);
						error = error_checking(error);
						if (error != ERR_NO_ERROR)
						{
							return (error);
						}
	
						// printf("\tACCEPTED, score = %lf", ((*p_input_array) + in_row_index)->rating);
						/*printf ("%ld.\t%lf\t%lf\t%lf\t%lf\t%lf", 
										(in_row_index +1), 
										((*p_input_array) + in_row_index)->ra, 
										((*p_input_array) + in_row_index)->dec, 
										((*p_input_array) + in_row_index)->mag[colour_number -1],
										((*p_input_array) + in_row_index)->mag[colour_number],
										((*p_input_array) + in_row_index)->mag[colour_number +1]);*/
						//if all is well, adds the details from the input array into the temproary one
						(temp_array + temp_row_index) ->ra = ((*p_input_array) + in_row_index)->ra;
						(temp_array + temp_row_index) ->dec = ((*p_input_array) + in_row_index)->dec;
						(temp_array + temp_row_index) ->mag [0] = ((*p_input_array) + in_row_index)->mag [0];
						(temp_array + temp_row_index) ->mag [1] = ((*p_input_array) + in_row_index)->mag [1];
						(temp_array + temp_row_index) ->mag [2] = ((*p_input_array) + in_row_index)->mag [2];
						(temp_array + temp_row_index) ->mag [3] = ((*p_input_array) + in_row_index)->mag [3];
						(temp_array + temp_row_index) ->mag [4] = ((*p_input_array) + in_row_index)->mag [4];
						(temp_array + temp_row_index) ->rating = ((*p_input_array) + in_row_index)->rating;
						temp_row_index++;
						
						//increments the size the new array will be
						*p_out_num_rows = *p_out_num_rows +1;
						//printf ("\t%ld\n", *p_out_num_rows);
						
						//resets colour_flag
						colour_flag = 0;
					}
					else //colour flag = 0 
					{
						colour_flag = 0;

						//printf("\tREJECTED, wrong colour\n");
						
					}
				}
				else
				{
					//printf("\t%lf",distance);
					//printf("\tREJECTED, out of FoV\n");
				}
			}
			else 
			{
				//printf("\t\t\tREJECTED, Magnitude out of range\n");
			}
		}
		else //rating 0
		{
			//printf("\t\t\tREJECTED, Too Close together\n");
		}

	}
	
	
	//allocates only enough memory to hold the stars that have met the criteria
	if (*p_out_num_rows > 0)
	{
		*p_output_array = (struct star *) malloc  (*p_out_num_rows * sizeof (struct star));
		if (*p_output_array == NULL)
		{
			return (ERR_OUT_OF_MEMORY);
		}
		else
		{
			printf ("\nMemory allocation step for output array succesfully completed\n");
		}
		
		for (out_row_index = 0; out_row_index < *p_out_num_rows; out_row_index++)
			{
				//assigns the contents of the temporary array into the output one.
				(*p_output_array + out_row_index) ->ra = (temp_array + out_row_index)->ra;
				(*p_output_array + out_row_index) ->dec = (temp_array + out_row_index)->dec;
				(*p_output_array + out_row_index) ->mag [0] = (temp_array + out_row_index)->mag [0];
				(*p_output_array + out_row_index) ->mag [1] = (temp_array + out_row_index)->mag [1];
				(*p_output_array + out_row_index) ->mag [2] = (temp_array + out_row_index)->mag [2];
				(*p_output_array + out_row_index) ->mag [3] = (temp_array + out_row_index)->mag [3];
				(*p_output_array + out_row_index) ->mag [4] = (temp_array + out_row_index)->mag [4];
				(*p_output_array + out_row_index) ->rating = (temp_array + out_row_index)->rating;
			}
		}	
	else
	{
		return (ERR_NOT_FOUND);
	}
	
	return (error);
}

int sift (struct star **p_input_array, 
		long int *p_num_rows, 
		long int current_index, 
		double resolution, 
		double resolution_ra,
		int colour_number)
{
	int sifting_index = 0;
	double mag_difference = 0;
	double dec_difference = 0;
	
	//for stars higher than the current index, within the array and 
	//within resolution of the current star in RA terms
	for (sifting_index = current_index +1; 
	sifting_index < *p_num_rows && 
	resolution_ra > (((*p_input_array) + sifting_index) -> ra )- (((*p_input_array) + current_index)->ra);
	sifting_index ++)
	{
		dec_difference = absolute_difference((((*p_input_array) + sifting_index) -> dec ), (((*p_input_array) + current_index)->dec));
		
		//if they are also within 5 FWHM of the current star in
		//dec terms and within 5 magnitudes of it.
		if (resolution > dec_difference )
		{
			mag_difference = absolute_difference (((*p_input_array) + sifting_index) -> mag[colour_number] , ((*p_input_array) + current_index) -> mag[colour_number]);
			if(mag_difference < 5)
			{
				//sets both stars to error values to be filtered 
				//at the next step
				((*p_input_array) + sifting_index) ->rating = 0;
				((*p_input_array) + current_index) ->rating = 0;
			}

		}
	}
	return (ERR_NO_ERROR);
}
/*
int write(struct star *output_array, long int *p_out_num_rows)
{
	int error = ERR_NO_ERROR;
	int status = 0;
	int *p_status = &status;
	
	//creates the pointer to the fits file to be created
	fitsfile *p_fp_out_fits;// = (NULL);
	
	//defines a number of columns and rows variable for checking purposes
	int num_cols = 0;
	int *p_num_cols = & num_cols;
	long int num_rows = 0;
	long int *p_num_rows= &num_rows;
	
	//declares an index for control of writing and 
	//column numbers the array is to write to
	int row_index =0;
	int new_ra_col = 1;
	int new_dec_col = 2;
	int new_mag_col = 3;
	
	//creates name and format files for the new table
	char ra_name[NAME_SIZE] = "ra";
	char *p_ra_name = & ra_name[0];
	char dec_name[NAME_SIZE] = "dec";
	char *p_dec_name = & dec_name[0];
	char mag_name[NAME_SIZE] = "mag";
	char *p_mag_name = & mag_name[0];
	char form[3] = "1D";
	char *p_form = &form[0];
	
	if(*p_out_num_rows == 0)
	{
		return (ERR_NOT_FOUND);
	}
	//ffinit()
	fits_create_file (p_fp_out_fits, FILTERED_FIT_OW, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}	
	
	//ffcrtb();
	fits_create_tbl (*p_fp_out_fits, BINARY_TBL, 0, 1, &p_ra_name, &p_form, NULL, NULL, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("\nNew FITS table created!\n");
	fits_insert_col (*p_fp_out_fits, 2, p_dec_name, p_form, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	fits_insert_col (*p_fp_out_fits, 3, p_mag_name, p_form, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}

	//confirms the number of columns in the file
	fits_get_num_cols (*p_fp_out_fits, p_num_cols, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("\nColumns set up.  Number of Columns is %d\n", *p_num_cols);

	//sets up the rows
	fits_insert_rows(*p_fp_out_fits, 0, *p_out_num_rows, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	
	//checks the number of rows and columns
	fits_get_num_rows (*p_fp_out_fits, p_num_rows, p_status);
	printf("\nNumber of Rows setup.  Number of Rows is %ld. \n", *p_num_rows);
	
	printf ("\nNo.\tRA\t\tDec\t\tMag\t\n");
	for (row_index = 1; row_index <= *p_num_rows; row_index++)
	{
		printf ("%d.\t", (row_index));

		fits_write_col (*p_fp_out_fits, TDOUBLE, new_ra_col, row_index, 1, 1, &((output_array+row_index-1)->ra), p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		printf ("%lf\t", (output_array + row_index-1)->ra);
		
		
		
		fits_write_col (*p_fp_out_fits, TDOUBLE, new_dec_col, row_index, 1, 1, &((output_array+row_index-1)->dec), p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		printf ("%lf\t", (output_array + row_index-1)->dec);
		
		
		
		fits_write_col (*p_fp_out_fits, TDOUBLE, new_mag_col, row_index, 1, 1, &((output_array+row_index-1)->mag), p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		printf ("%lf\n",(output_array + row_index-1)->mag);
	}


	
	
	
	
	
	
	//closes the fitsfile
	fits_close_file (*p_fp_out_fits, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	
	printf ("\nWrite stage complete\n");
	return (error);
	
}*/



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

			/*error = name_parameter_file (p_fp_parameters, p_file_type);
			error = error_checking (error);
			if (error != ERR_NO_ERROR)
			{
				return (error);
			}*/

	}
	//if an argument has been entered, attempt to use it.
	else if ((num_args==2) || (num_args == 3))
	{
		printf("\nThe file you have selected is %s", arg_one);
		//if it is not a valid file name, attempt to request a new filename
		check_file_type (arg_one, p_file_type);

		if ((*p_file_type =='p'))
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
		else //(num_args >3)
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


int check_colour (char *p_colour, int *p_colour_number)
{
	int error = ERR_NO_ERROR; 
	
	if ((*p_colour == 'u')|| (*p_colour == 'U'))
	{
		*p_colour_number = 0;
	}
	else if ((*p_colour == 'g')|| (*p_colour == 'G'))
	{
		*p_colour_number = 1;
	}
	else if ((*p_colour == 'r')|| (*p_colour == 'R'))
	{
		*p_colour_number = 2;
	}
	else if ((*p_colour == 'i')|| (*p_colour == 'I'))
	{
		*p_colour_number = 3;
	}
	else if ((*p_colour == 'z')|| (*p_colour == 'Z'))
	{
		*p_colour_number = 4;
	}
	
	else
	{
		printf ("\nSelected colour is invalid!");
		return (ERR_COLOUR_ERROR);
	}
	
	return (error);
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

int get_telescope_parameters (	double *p_fov_size,
								double *p_resolution,
								double *p_max_mag_diff,
								double *p_max_col_diff,
								FILE **p_fp_parameters)
{
	int error = ERR_NO_ERROR;

	
	//reads in the size of the FoVs 
	if (fread (p_fov_size, sizeof(double), 1, *p_fp_parameters) != 1)
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
	
	//reads in the resolution 
	if (fread (p_resolution, sizeof(double), 1, *p_fp_parameters) != 1)
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
	
	//reads in the maximum variance in magnitude 
	if (fread (p_max_mag_diff, sizeof(double), 1, *p_fp_parameters) != 1)
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
	
	//reads in the maximum variance in colour 
	if (fread (p_max_col_diff, sizeof(double), 1, *p_fp_parameters) != 1)
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

int make_array(	FILE **p_fp_parameters, 
				struct star **p_input_array, 
				long int *p_num_rows,
				long int *p_missing_file_count)
{
	int error = ERR_NO_ERROR;

	//the number of files to make up that field
	long int num_files = 0;

	//the number of files to make up that field
	long int *p_num_files = &num_files;
	
	//an index that is used to track through this field
	long int file_index = 0;
	
	//an index that is used to track through this field
	long int input_index = 0;
	
	//sets up long ints to hold the number or rows in each array
	long int in_num_rows =0;
	long int *p_in_num_rows = & in_num_rows;
	long int num_rows_to_date = 0;
	long int *p_num_rows_to_date = &num_rows_to_date;
	
	//creates a variable for extracting the information of file names
	char *file_name = (char *) malloc (FILE_NAME_SIZE * sizeof(char));
	char *local_file_name = (char *) malloc (FILE_NAME_SIZE * sizeof(char));
		
	//sets up a structure for temporarily holding the input 
	struct star *input_array_2;
	struct star **p_input_array_2 = &input_array_2;
	struct star *reading_array;
	struct star **p_reading_array = &reading_array;
	
	
	//reads in the number of fields 
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
	
	//this line is for visualisation generation
	printf ("\nRA\t\tDec\t\tMag (ugriz)");
	
	for(file_index = 0; file_index < num_files; file_index ++)
		{
			//get the next file name from the parameters file
			error = get_file_name(file_name, p_fp_parameters);
			//and runs the error checking routine	
			error= error_checking (error);
			if (error != ERR_NO_ERROR)
			{
				return (error);
			}
			
			//makes a _local.fit name from the file name given in the 
			//parameters file.
			error = make_local_file_name(file_name, local_file_name);
			//and runs the error checking routine		
			error= error_checking (error);
			if (error != ERR_NO_ERROR)
			{
				return (error);
			}
		
			
			*p_in_num_rows = 0;
		
			//runs the read function and checks it for errors
			error =	read(local_file_name, p_reading_array, p_in_num_rows);
		
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
				(*p_missing_file_count)= (*p_missing_file_count)+1;
				printf("\n%ld out of %ld files are missing so far." , *p_missing_file_count, (file_index+1));
			}
			
			*(p_input_array) = (struct star*) malloc ((*p_num_rows_to_date +*p_in_num_rows)* sizeof(struct star));
			
			for (input_index = 0; input_index<*p_num_rows_to_date; input_index ++)
			{
				*(*(p_input_array) +input_index) = *(input_array_2 +input_index);
			}
			for (input_index = 0; input_index<*p_in_num_rows; input_index ++)
			{
				*(*(p_input_array) +input_index + *p_num_rows_to_date) = *(reading_array +input_index);
			}
			//free (reading_array);
			if(file_index >0)
			{
				//free (input_array_2);
			}
			*p_num_rows_to_date = *p_num_rows_to_date + *p_in_num_rows;
			*p_input_array_2 = *(p_input_array);
			
		
		
		}
	*p_in_num_rows = *p_num_rows_to_date;
	*p_num_rows = *p_in_num_rows;
	return (error);
	
}

int get_target_info(	FILE **p_fp_parameters, struct star **p_target_star )
{
	printf("\ntarget object");
	//reads in the number of fields 
	if (fread (&(*p_target_star)->ra, sizeof(double), 1, *p_fp_parameters) != 1)
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
	printf("\tprobe ra: %lf", (*p_target_star)->ra);
	//reads in the number of fields 
	if (fread (&(*p_target_star)->dec, sizeof(double), 1, *p_fp_parameters) != 1)
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
	printf("\tprobe dec: %lf", (*p_target_star)->dec);
	//reads in the number of fields 
	if (fread (&(*p_target_star)->mag[0], sizeof(double), 1, *p_fp_parameters) != 1)
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
	printf("\tu: %lf", (*p_target_star)->mag[0]);
	//reads in the number of fields 
	if (fread (&(*p_target_star)->mag[1], sizeof(double), 1, *p_fp_parameters) != 1)
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
	printf("\tg: %lf", (*p_target_star)->mag[1]);
	//reads in the number of fields 
	if (fread (&(*p_target_star)->mag[2], sizeof(double), 1, *p_fp_parameters) != 1)
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
	printf("\tr: %lf", (*p_target_star)->mag[2]);
	//reads in the number of fields 
	if (fread (&(*p_target_star)->mag[3], sizeof(double), 1, *p_fp_parameters) != 1)
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
	printf("\ti: %lf", (*p_target_star)->mag[3]);
	//reads in the number of fields 
	if (fread (&(*p_target_star)->mag[4], sizeof(double), 1, *p_fp_parameters) != 1)
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
	printf("\tz: %lf", (*p_target_star)->mag[4]);
	//reads in the number of fields 
	if (fread (&(*p_target_star)->rating, sizeof(double), 1, *p_fp_parameters) != 1)
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
	
	return (ERR_NO_ERROR);
	
}

int scoring_mechanism(	struct star **p_input_array, 
						long int in_row_index,
						struct star **p_target, 
						double *p_max_mag_diff, 
						double *p_max_col_diff,
						int colour_number)
{
	int error = ERR_NO_ERROR;
	/*
	//scoring mechanism not implemented.  This is a placeholder function
	
	((*p_input_array) + in_row_index) ->rating = 1;
	
	return (error);
	*/
	/*
	 double score = 0;
	  
	 double mag_diff = 0;
	  
	  mag_diff = absolute_difference ((((*p_input_array) + in_row_index) -> mag[colour_number]), (((*p_target) -> mag [colour_number])));
	  //Potential scoring mechanism.  Might or might not be viable
	  printf ("magnitude_difference %lf", mag_diff);
	  //calculates score based on difference and normalises to max permitted difference
	  score = (*p_max_mag_diff - mag_diff)/(*p_max_mag_diff);
	  printf ("score %lf", score);
	  
	  if(score >= 0)
	  {
	  
	  ((*p_input_array) + in_row_index) ->rating = score;
	  }
	  else
	  {
	  ((*p_input_array) + in_row_index) ->rating = 0;
	  }
	  
	  return (error);
	  
	 */
	
	/*
	 * 
	 * //allow for colour difference?
	 * 
	 * Use the function below (colour comparison) as basis)
	 * 
	 * 
	 * 
	 */
	

	//sets up variables and flags to study colour difference
	double col_longer = 0;
	double col_shorter = 0;
	double col_target_longer = 0;
	double col_target_shorter = 0;
	double col_comparison_longer = 0;
	double col_comparison_shorter = 0;
	
	double score = 0;
	double score_shorter = 0;
	double score_longer = 0;
	
	if ((colour_number < 0)||(colour_number >= NUM_COLOURS))
	{
		return (ERR_COLOUR_ERROR);
	}
	//checks if the user has picked the first colour
	if (colour_number == 0)
	{
		//first colour is longest (eg u in ugriz)
		//so there is no longer colour: use shorter only
		col_shorter =  ((((*p_input_array) + in_row_index)->mag[colour_number]) - (((*p_input_array) + in_row_index)->mag[colour_number+1]));
		col_target_shorter =  (((*p_target)->mag[colour_number] )- ((*p_target)->mag[colour_number+1]));
		col_comparison_shorter = absolute_difference (col_shorter, col_target_shorter);
		
		if (col_comparison_shorter < *p_max_col_diff)
		{
		  score = (*p_max_col_diff - col_comparison_shorter)/(*p_max_col_diff);
		  //printf ("\tscore %lf", score);
			  ((*p_input_array) + in_row_index)->rating = score;
			//(((*p_input_array) + in_row_index)->rating) = score;
		}
	}
	//or if the user has picked the last colour
	else if (colour_number == NUM_COLOURS -1)
	{
		//last colour is shortest (eg z in ugriz)
		//so there is no shorter colour: use longer only
		col_longer =  ((((*p_input_array) + in_row_index)->mag[colour_number]) - (((*p_input_array) + in_row_index)->mag[colour_number-1]));
		col_target_longer =  (((*p_target)->mag[colour_number] )- ((*p_target)->mag[colour_number-1]));
		col_comparison_longer = absolute_difference (col_longer, col_target_longer);
		
		if (col_comparison_longer < *p_max_col_diff)
		{
			  score = (*p_max_col_diff - col_comparison_longer)/(*p_max_col_diff);
			  //printf ("\tscore %lf", score);
				  

			  ((*p_input_array) + in_row_index)->rating = score;
			//(((*p_input_array) + in_row_index)->rating) = score;
		}
	}
	else //otherwise they have picked one of the middle colour numbers
	{
		//with the middle colours, you can get both longer and shorter wavelengths to compare with
		col_shorter =  ((((*p_input_array) + in_row_index)->mag[colour_number]) - (((*p_input_array) + in_row_index)->mag[colour_number+1]));
		col_target_shorter =  (((*p_target)->mag[colour_number] )- ((*p_target)->mag[colour_number+1]));
		col_comparison_shorter = absolute_difference (col_shorter, col_target_shorter);
		
		col_longer =  ((((*p_input_array) + in_row_index)->mag[colour_number]) - (((*p_input_array) + in_row_index)->mag[colour_number-1]));
		col_target_longer =  (((*p_target)->mag[colour_number] )- ((*p_target)->mag[colour_number-1]));
		col_comparison_longer = absolute_difference (col_longer, col_target_longer);
		
		if ((col_comparison_shorter < *p_max_col_diff)&&(col_comparison_longer < *p_max_col_diff))
		{
			  score_shorter = (*p_max_col_diff - col_comparison_shorter)/(*p_max_col_diff);
			  score_longer = (*p_max_col_diff - col_comparison_longer)/(*p_max_col_diff);
			  score = score_longer * score_shorter;
			  //printf ("\tshorter: %lf \t longer: %lf\tmaximum: %lf\tscore %lf", col_comparison_shorter, col_comparison_longer, *p_max_col_diff, score);
				  

			  ((*p_input_array) + in_row_index)->rating = score;
			//(((*p_input_array) + in_row_index)->rating) = score;
		}
	}
	return (error	);
	
	
}

int colour_comparison (	struct star **p_input_array, 
						long int in_row_index, 
						struct star **p_target,
						double *p_max_col_diff,
						bool *p_colour_flag, 
						int colour_number)
{
	//sets up variables and flags to study colour difference
	double col_longer = 0;
	double col_shorter = 0;
	double col_target_longer = 0;
	double col_target_shorter = 0;
	double col_comparison_longer = 0;
	double col_comparison_shorter = 0;
	
	if ((colour_number < 0)||(colour_number >= NUM_COLOURS))
	{
		return (ERR_COLOUR_ERROR);
	}
	//checks if the user has picked the first colour
	if (colour_number == 0)
	{
		//first colour is longest (eg u in ugriz)
		//so there is no longer colour: use shorter only
		col_shorter =  ((((*p_input_array) + in_row_index)->mag[colour_number]) - (((*p_input_array) + in_row_index)->mag[colour_number+1]));
		col_target_shorter =  (((*p_target)->mag[colour_number] ) - ((*p_target) ->mag[colour_number+1]));
		col_comparison_shorter = absolute_difference (col_shorter, col_target_shorter);
		
		if (col_comparison_shorter < *p_max_col_diff)
		{
			*p_colour_flag = 1;
		}
	}
	//or if the user has picked the last colour
	else if (colour_number == NUM_COLOURS -1)
	{
		//last colour is shortest (eg z in ugriz)
		//so there is no shorter colour: use longer only
		col_longer =  ((((*p_input_array) + in_row_index)->mag[colour_number]) - (((*p_input_array) + in_row_index)->mag[colour_number-1]));
		col_target_longer =  (((*p_target)->mag[colour_number]) - ((*p_target)->mag[colour_number-1]));
		col_comparison_longer = absolute_difference (col_longer, col_target_longer);
		
		if (col_comparison_longer < *p_max_col_diff)
		{
			*p_colour_flag = 1;
		}
	}
	else //otherwise they have picked one of the middle colour numbers
	{
		//with the middle colours, you can get both longer and shorter wavelengths to compare with
		col_shorter =  ((((*p_input_array) + in_row_index)->mag[colour_number]) - (((*p_input_array) + in_row_index)->mag[colour_number+1]));
		col_target_shorter =  (((*p_target)->mag[colour_number] )- ((*p_target)->mag[colour_number+1]));
		col_comparison_shorter = absolute_difference (col_shorter, col_target_shorter);
		
		col_longer =  ((((*p_input_array) + in_row_index)->mag[colour_number]) - (((*p_input_array) + in_row_index)->mag[colour_number-1]));
		col_target_longer =  (((*p_target)->mag[colour_number] )- ((*p_target)->mag[colour_number-1]));
		col_comparison_longer = absolute_difference (col_longer, col_target_longer);

		if ((col_comparison_shorter < *p_max_col_diff)&&(col_comparison_longer < *p_max_col_diff))
		{
			*p_colour_flag = 1;
		}
	}
	return (ERR_NO_ERROR);
	
}

