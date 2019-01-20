#include "quick_sort_5_0.h"
/*
int quick_sort_star (struct star **p_struct_star, long int *p_num_rows)
{
	//for test purposes
	int print_index = 0;
	
	//for error checking purposes
	int error = ERR_NO_ERROR;
	
	printf ("/nBeginning the quicksort stage/n");
	error = quick_sort_star_run (p_struct_star, 0, *p_num_rows -1);
	
	//for test purposes only
	printf ("\nNo.\tRA\t\tDec\t\tMag\n");
	for (print_index = 0; print_index < *p_num_rows; print_index ++)
	{
		printf ("\n%d.\t%lf\t%lf\t%lf", (print_index +1), 
				((*p_struct_star) + print_index)->ra, 
				((*p_struct_star) + print_index)->dec, 
				((*p_struct_star) + print_index)->mag);
	}
	
	return (error);
}
*/
/*
int quick_sort_star_run (struct star **p_struct_star, long int beginning, long int ending)
{
	long int rising_index = 0;
	long int falling_index = 0;
	struct star *pivot = (struct star *) malloc (sizeof (struct star));
	struct star *temp = (struct star *) malloc (sizeof (struct star));
	
	rising_index = beginning;
	falling_index = ending;

	pivot = (*p_struct_star )+ ((beginning+ending)/2);
	
	do
	{
		//goes up through the array until it finds an entry whose 
		//ra is not less than that of the pivot
		while (((*p_struct_star)+rising_index)->ra < pivot->ra 
				&& rising_index <ending) rising_index ++;
		//goes down through the array until it finds an entry whose 
		//ra is not greater than that of the pivot
		while (((*p_struct_star)+falling_index)->ra > pivot->ra 
				&& falling_index >beginning) falling_index --;
		
		//id the indices haven't passed one another yet, swap those
		//elements.
		if (rising_index <= falling_index)
		{
			temp->ra = ((*p_struct_star) + rising_index) -> ra;
			temp->dec = ((*p_struct_star) + rising_index) -> dec;
			temp->mag = ((*p_struct_star) + rising_index) -> mag;
			
			
			((*p_struct_star) + rising_index)->ra = ((*p_struct_star) + falling_index)->ra;
			((*p_struct_star) + rising_index)->dec = ((*p_struct_star) + falling_index)->dec;
			((*p_struct_star) + rising_index)->mag = ((*p_struct_star) + falling_index)->mag;
			
			
			((*p_struct_star) + falling_index)->ra = temp->ra;
			((*p_struct_star) + falling_index)->dec = temp->dec;
			((*p_struct_star) + falling_index)->mag = temp->mag;
			rising_index ++;
			falling_index --;
			
		}
	}
	//keep doing this loop while the rising index is less than 
	//or equal to the falling one.  i.e stop doing the loop if
	//the rising index passes the falling one.
	while (rising_index <= falling_index);
	
	//if the falling index has not passed the beginning, 
	//recursively sort the array between the beginning and 
	//the falling index
	if (falling_index > beginning)
	{
		quick_sort_star_run (p_struct_star, beginning, falling_index);
	}
	
	//if the rising index has not passed the end, 
	//recursively sort the array between the rising 
	//index and the end
	if (rising_index < ending)
	{
		quick_sort_star_run (p_struct_star, rising_index, ending);
	}
	
	return (ERR_NO_ERROR);
}
*/
int star_compare (const void *star_1, const void *star_2)
{
	double difference = 0.0;
	difference = *((double *)star_1) - *((double*) star_2 );
	if (difference <0)
	{
		return (-1);
	}
	else if (difference > 0)
	{
		return (1);
		
	}
	else //(difference = 0)
	{
		return (0);
	}
}


