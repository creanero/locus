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