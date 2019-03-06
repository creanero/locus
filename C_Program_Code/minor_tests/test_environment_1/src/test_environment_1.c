/*
 ============================================================================
 Name        : test_environment_1.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	char *p_file_name = malloc (100*sizeof(char));
	
	printf("\n%s\n", argv[1]); /* prints !!!Hello World!!! */
	
	sprintf (p_file_name, "$%s", argv[1]);
	
	fopen(p_file_name, "wb");
	
	free (p_file_name);
	
	return EXIT_SUCCESS;
}
