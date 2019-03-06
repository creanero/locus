//============================================================================
// Name        : error_checking_1_1.cpp
// Author      : Oisin Creaner
// Date		   : 14th May 2008
// Copyright   : (c) Oisin Creaner, ITT Dublin 2008
// Description : Program to check output from functions for errors
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

using namespace std;



//  EEEE RRRR  RRRR   OOO  RRRR       CCC H  H EEEE  CCC K  K 
//  E    R   R R   R O   O R   R     C    H  H E    C    K K  
//  EEE  RRRR  RRRR  O   O RRRR      C    HHHH EEE  C    KK   
//  E    R R   R R   O   O R R       C    H  H E    C    K K  
//  EEEE R  RR R  RR  OOO  R  RR ____ CCC H  H EEEE  CCC K  K 
//                               ____                         
                                                          


int error_checking (int error)
{
	if (error == ERR_NO_ERROR) 
	{
		printf("\nno error reported at this stage\n");
		return (error);
	}
	else if (error > ERR_NO_ERROR)
	{
		fits_report_error (stdout, error);
	}
		
	else if (error == ERR_OUT_OF_MEMORY)
	{
		printf ("\nThere is insufficient memory available\n");
		
		return (error);
	}
	else if (error == ERR_END_OF_FILE)
	{
		printf ("\nThe file pointer has reached the end of the file prematurely.\n");
		
		return (error);
	}
	else if (error == ERR_FILE_READ)
	{
		printf ("\nThere has been a file read error\n");
		
		return (error);
	}
	else if (error == ERR_CANNOT_OPEN)
	{
		printf ("\nThe file cannot be opened for some reason\n");
		
		return (error);
	}
	else if (error == ERR_CANNOT_CLOSE)
	{
		printf ("\nThe file cannot be closed for some reason\n");
		
		return (error);
	}
	else if (error == ERR_PAR_NOT_FOUND)
	{
		printf ("\nA key parameter was not located in the CTI info file\n");
		
		return (error);
	}	
	else if (error == ERR_WRITE_FAILURE)
	{
		printf ("\nThe program could not write to the selected file!\n");
		
		return (error);
	}	
	else if (error == ERR_INVALID_COLUMN)
	{
		printf ("\nThe program found an invalid column name!\n");
		
		return (error);
	}	
	else if (error == ERR_COLUMNS_EQUAL)
	{
		printf ("\nCannot have two parameters in the same column!\n");
		
		return (error);
	}
	else if (error == ERR_FITS_ERROR)
	{
		printf ("\nfits has returned an error status!\n");
		
		return (error);
	}
	else if (error == ERR_NOT_FOUND)
	{
		printf ("\nNo Suitable value found!\n");
		
		return (error);
	}
	else
	{
		printf ("\nUnknown error encountered!\n");
		return (error);
	}
	return (error);	
}



