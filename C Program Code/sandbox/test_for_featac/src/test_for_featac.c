/*
 ============================================================================
 Name        : test_for_featac.c
 Author      : Oisin Creaner
 Version     :
 Copyright   : free to use
 Description : getting a substring of a code
 ============================================================================
 */



#include <stdio.h>
#include <stdlib.h>

//must include this!
#include <string.h>

//includes error checking variables out of habit. 
#define ERR_NO_ERROR 0
#define ERR_STRING_ERROR 1

int extract_code (char *code_string, char *exit_string);

int main() {
	//probably don't need 50
	char *code_string = (char *) malloc(50*sizeof(char));
	char *exit_string = (char *) malloc(50*sizeof(char));
	
	int code_length = 0;

	//obviously, these won't be manually entered but passed by a function
	printf ("Enter a string");
	scanf ("%s", code_string);
	
	//this can probably go, just for illustrative purposes
	printf("\ncode entered was %s", code_string);
	
	//this is the important function
	extract_code (code_string, exit_string);
	
	//strlen is much better than sizeof for these purposes
	code_length = strlen (exit_string);
	
	//a simple if-else setup works here
	if (code_length == 3)
	{
		//the printouts are obviously to be replaced with whatever destination
		//you have in mind for your data!
		printf ("\nCode %s placed in destination 1 (for length 3 strings)", exit_string);
		return (ERR_NO_ERROR);
	}
	else if (code_length == 5)
	{
		//the printouts are obviously to be replaced with whatever destination
		//you have in mind for your data!
		printf ("\nCode %s placed in destination 2 (for length 5 strings)", exit_string);
		return (ERR_NO_ERROR);
	}
	//I have a habit of putting an empty case in for completeness
	else //code does not match parameters
	{
		printf ("\nERROR: CODE DOES NOT MATCH PARAMETERS");
		return (ERR_STRING_ERROR);
	}
	
	return (ERR_NO_ERROR);
}

//the main function here
int extract_code (char *code_string, char *exit_string)
{
	//the sscanf scans the string to find the given text
	//if it is not found, it will return an error
	if (sscanf(code_string, "\t\t\t\tcode=%s", exit_string)!=1)
	{
		return (ERR_STRING_ERROR);
	}
	return (ERR_NO_ERROR);
}



