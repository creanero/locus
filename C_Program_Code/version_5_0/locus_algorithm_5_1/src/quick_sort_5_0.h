#ifndef QUICK_SORT_2_0_H_
#define QUICK_SORT_2_0_H_

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

//needed to define the structures
#include "struct_definitions_5_2.h"

//needed to allow for error checking routines
#include "error_checking_5_3.h"

/*int quick_sort_star (struct star **p_struct_star, long int *p_num_rows);

int quick_sort_star_run (struct star **p_struct_star, long int beginning, long int ending);
*/
int star_compare (const void *, const void *);

#endif /*QUICK_SORT_2_0_H_*/
