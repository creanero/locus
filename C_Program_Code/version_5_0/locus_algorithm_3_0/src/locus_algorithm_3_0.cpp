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
#include "error_checking_1_1.h"

//includes the file name description
#include "file_info_3_1.h"

//includes the structure definitions
#include "struct_definitions_2_0.h"

//includes the mathemathical library
#include <math.h>

//includes the data used to describe the target star/quasar 
//this "defined" method is temporary
#include "target_data_2_1.h"

// includes the time library for use as a metric 
#include <time.h>

//includes a header file for a set of minor useful functions
#include "minor_functions_2_0.h"

using namespace std;

//standard read function as used in other programs
int read(struct star **p_input_array, long int *p_in_num_rows);

//function that will calculate the boundaries for each star
int set_boundaries (
		struct star **p_star_array, 
		struct star *target_star,
		struct bounded_star **p_bounded_star_array, 
		long int *p_num_rows,
		double radius,
		double ra_radius);

//function that will find the intercept points.
int set_intercept (
		struct star **p_star_array, 
		struct star *target_star,
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
int final_write (
		struct star *target, 
		struct intercept *average_point);

int write(struct star *output_array, 
		long int *p_out_num_rows);

int main ()
{

	//time functions to act as a basic metric.  Not that 
	//start_time is initialised to the current time to 
	//start the timer as early as possible
	long int start_time = time(NULL);
	long int  end_time = 0;
	long int  total_time = 0;
	
	
	
	double radius = RADIUS/2;
	double ra_radius = RADIUS/2;
	//note that to allow the program to run, num_rows is
	//initialised to ONE, not ZERO
	long int num_rows = 1;
	long int *p_num_rows = & num_rows;
	long int out_num_rows = 0;
	long int *p_out_num_rows = & out_num_rows;
		
	//int row_index = 0;
	int error = 0;
	
	
	
	

	//this needs to be commented out later
	//Sets the number of rows before mallocing the space for
	//the array.  Note that the READ function mallocs the 
	//space needed anyway, this step is only needed to set up 
	//the double pointer.	
	//num_rows = 10;
		
	struct star *star_array= (struct star *) malloc (num_rows * sizeof (struct star));
	struct star **p_star_array  = &star_array;
	struct star *output_array= {NULL};
	struct star **p_output_array  = &output_array;
	
	//setus up the target star point using the defined terms 
	//in struct_definitions.  This step of the program will 
	//eventually have to be a run-time allocation step,
	// but this will do for now
	struct star *target_star= (struct star *) malloc (sizeof (struct star));
	target_star -> ra = TARGET_RA;
	target_star -> dec = TARGET_DEC;
	target_star -> mag = TARGET_MAG;
	
	
	//this step sets the variabl;e controlling the "radius" 
	//in the RA direction.  Because RA degrees are not
	//equal to dec degrees, it is necessary to calculate from
	//spherical co-ordinate transformations
	ra_radius = radius / cos(target_star ->dec * M_PI /180);
	
	//this prints the RA and DEC radii.  It is a check step
	printf ("\nRadius %lf, Radius in RA: %lf", radius, ra_radius);
	
	
	//reads in the filtered file array
	read (p_star_array, p_num_rows);
	
	//this sets up an array which will hold the boundary points 
	//for the star.  Note that the READ step must have taken place
	//before this step is executed beacuse otherwise insufficient 
	//memory will be allocated (num_rows will equal the initial
	//value
	struct bounded_star *bounded_star_array = (struct bounded_star *) malloc (num_rows * sizeof (struct bounded_star));
	struct bounded_star **p_bounded_star_array = & bounded_star_array;
	
	
	//this sets up the variable that will be used to store the
	//temporary variable for an interception point used to compare each star with its neighbours
	struct intercept *current_intercept = (struct intercept *) malloc (sizeof (struct intercept));
	struct intercept **p_current_intercept =  &current_intercept;
	current_intercept -> ra = 0;
	current_intercept -> dec = 0;
	current_intercept -> score = 0;
	
	//this sets up the variable that will be used to store the
	//best interception point after each comparison
	struct intercept *best_intercept = (struct intercept *) malloc (sizeof (struct intercept));
	struct intercept **p_best_intercept = &best_intercept;
	best_intercept -> ra = 0;
	best_intercept -> dec = 0;
	best_intercept -> score = 0;
	
	
	


	
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
			p_star_array, 
			target_star, 
			p_bounded_star_array, 
			p_num_rows, 
			radius,
			ra_radius);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	//runs the program to make and check the interception
	//points between the boundary lines for each star
	error = set_intercept (
			p_star_array,
			target_star,
			p_bounded_star_array,
			p_best_intercept,
			p_current_intercept,
			p_num_rows,
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
		printf ("\n\nNO INTERCEPT FOUND!");
	}
	

	
	
	
	//runs the writing program that creates the output .fit
	//file for the user.
	error = final_write (target_star, best_intercept);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	error = list_comparison_stars (star_array, p_num_rows, p_output_array, p_out_num_rows, p_best_intercept);
	error = error_checking (error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	//runs the write function and checks it for errors
	error = write(output_array, p_out_num_rows);
	error = error_checking(error);
	if (error != ERR_NO_ERROR)
	{
		return (error);
	}
	
	
	end_time = time(NULL);
	total_time = end_time - start_time;
	printf ("\nThe time taken was %ld", total_time);
	
	return (ERR_NO_ERROR);
}

//this program reads in the informationfrom the supplied fits file
int read(struct star **p_input_array, long int *p_in_num_rows)
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
	
}

int set_boundaries (
		struct star **p_star_array, 
		struct star *target_star,
		struct bounded_star **p_bounded_star_array, 
		long int *p_num_rows, 
		double radius,
		double ra_radius)
{
	
	//sets an index value that will be used to control a for loop
	int star_index = 0;
	
	printf ("\n\nra\t\tdec\t\tmag\t\tb_ra\t\tb_ra_dir\tb_dec\t\tb_dec_dir");
	for (star_index = 0; star_index < (*p_num_rows); star_index++)
	{
		//prints the basic details about the star before being run
		printf("\n%lf\t%lf\t%lf",
				((*p_star_array) + star_index)-> ra ,
				((*p_star_array) + star_index)-> dec,
				((*p_star_array) + star_index)-> mag
				);
	
		// if the comparison star is lower in ra than the target
		// put the boundary line above the comparison star with 
		// the line pointing "down"
		if (((*p_star_array) + star_index)-> ra < target_star -> ra)
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
		if (((*p_star_array) + star_index)-> dec < target_star -> dec)
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
		
	}
	
	
	return (ERR_NO_ERROR);
}


//USES THE BOUNDARY LINES TO FIND THE INTERCEPT POINTS OF THE LOCI

int set_intercept (
		struct star **p_star_array, 
		struct star *p_target_star,
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
			(*p_current_intercept) -> score = (*p_current_intercept) -> score +1;
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

int list_comparison_stars (struct star *input_array, 
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
}


//standardised finla writing stage
int final_write (struct star *target, struct intercept * average_point)
{
	int error = ERR_NO_ERROR;
	int status = 0;
	int *p_status = &status;
	
	//creates the pointer to the fits file to be created
	fitsfile *p_out_fits;
	
	//defines a number of columns and rows variable for checking purposes
	int num_cols = 0;
	int *p_num_cols = & num_cols;
	long int num_rows = 0;
	long int *p_num_rows= &num_rows;
	
	//declares an index for control of writing and 
	//column numbers the array is to write to
	int row_index =1;
	int new_ra_col = 1;
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
	
	fits_create_file (&p_out_fits, FINAL_FIT_OW, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}	
	
	fits_create_tbl (p_out_fits, BINARY_TBL, 0, 1, &p_ra_name, &p_form, NULL, NULL, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("\nNew FITS table created!\n");
	fits_insert_col (p_out_fits, new_dec_col, p_dec_name, p_form, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	fits_insert_col (p_out_fits, new_mag_col, p_mag_name, p_form, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	fits_insert_col (p_out_fits, new_av_ra_col, p_av_ra_name, p_form, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	fits_insert_col (p_out_fits, new_av_dec_col, p_av_dec_name, p_form, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	fits_insert_col (p_out_fits, new_av_rating_col, p_av_rating_name, p_form, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}

	//confirms the number of columns in the file
	fits_get_num_cols (p_out_fits, p_num_cols, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("\nColumns set up.  Number of Columns is %d\n", *p_num_cols);

	//sets up the rows
	fits_insert_rows(p_out_fits, 0, 1, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	
	//checks the number of rows and columns
	fits_get_num_rows (p_out_fits, p_num_rows, p_status);
	printf("\nNumber of Rows setup.  Number of Rows is %ld. \n", *p_num_rows);
	
	printf ("\nTgRA\t\tTgDec\t\tTgMag\t\tAvRA\t\tAvDec\t\tRating\n");
	
	fits_write_col (p_out_fits, TDOUBLE, new_ra_col, row_index, 1, 1, &(target->ra), p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("%lf\t", target->ra);
		
	fits_write_col (p_out_fits, TDOUBLE, new_dec_col, row_index, 1, 1, &(target->dec), p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("%lf\t", target->dec);
	
	fits_write_col (p_out_fits, TDOUBLE, new_mag_col, row_index, 1, 1, &(target->mag), p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("%lf\t",target->mag);
	
	fits_write_col (p_out_fits, TDOUBLE, new_av_ra_col, row_index, 1, 1, &(average_point->ra), p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("%lf\t", average_point->ra);
		
	fits_write_col (p_out_fits, TDOUBLE, new_av_dec_col, row_index, 1, 1, &(average_point->dec), p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("%lf\t", average_point->dec);
	fits_write_col (p_out_fits, TDOUBLE, new_av_rating_col, row_index, 1, 1, &(average_point->score), p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("%lf\n", average_point->score);

	return (error);
}

int write(struct star *output_array, long int *p_out_num_rows)
{
	int error = ERR_NO_ERROR;
	int status = 0;
	int *p_status = &status;
	
	//creates the pointer to the fits file to be created
	fitsfile *p_out_fits;// = (NULL);
	
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
	fits_create_file (&p_out_fits, FINAL_LIST_OW, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}	
	
	//ffcrtb();
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
	fits_insert_col (p_out_fits, 3, p_mag_name, p_form, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}

	//confirms the number of columns in the file
	fits_get_num_cols (p_out_fits, p_num_cols, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	printf ("\nColumns set up.  Number of Columns is %d\n", *p_num_cols);

	//sets up the rows
	fits_insert_rows(p_out_fits, 0, *p_out_num_rows, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	
	//checks the number of rows and columns
	fits_get_num_rows (p_out_fits, p_num_rows, p_status);
	printf("\nNumber of Rows setup.  Number of Rows is %ld. \n", *p_num_rows);
	
	printf ("\nNo.\tRA\t\tDec\t\tMag\t\n");
	for (row_index = 1; row_index <= *p_num_rows; row_index++)
	{
		printf ("%d.\t", (row_index));

		fits_write_col (p_out_fits, TDOUBLE, new_ra_col, row_index, 1, 1, &((output_array+row_index-1)->ra), p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		printf ("%lf\t", (output_array + row_index-1)->ra);
		
		
		
		fits_write_col (p_out_fits, TDOUBLE, new_dec_col, row_index, 1, 1, &((output_array+row_index-1)->dec), p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		printf ("%lf\t", (output_array + row_index-1)->dec);
		
		
		
		fits_write_col (p_out_fits, TDOUBLE, new_mag_col, row_index, 1, 1, &((output_array+row_index-1)->mag), p_status);
		if (*p_status!=0)
		{
			return(ERR_FITS_ERROR);
		}
		printf ("%lf\n",(output_array + row_index-1)->mag);
	}


	
	
	
	
	
	
	//closes the fitsfile
	fits_close_file (p_out_fits, p_status);
	if (*p_status!=0)
	{
		return(ERR_FITS_ERROR);
	}
	
	printf ("\nWrite stage complete\n");
	return (error);
	
}


