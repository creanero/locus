#include "minor_functions_2_0.h"


//minor user-defined function for getting 
//the absolute value of a double number  
double absolute_value (double input)
{
	//if the input is less than zero, it is changed to its
	//own negative
	if (input < 0)
	{
		input = - input;
	}
	return (input);
}

//minor user-defined function for getting the absolute 
//value of the difference between 2 double numbers  
double absolute_difference (double input_1, double input_2)
{
	double output = 0;
	
	//subtracts and gets the absolute value of the difference
	output = absolute_value ((input_1 - input_2));
	
	return (output);
}

