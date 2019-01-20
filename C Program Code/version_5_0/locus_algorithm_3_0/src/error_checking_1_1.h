//============================================================================
// Name        : error_checking_1_1.h
// Author      : Oisin Creaner
// Date		   : 15th May 2008
// Copyright   : (c) Oisin Creaner, ITT Dublin 2008
// Description : Header file that defines the error numbers used
//============================================================================


#ifndef ERROR_CHECKING_1_1_H_
#define ERROR_CHECKING_1_1_H_

// uses #define to create error reporting numbers
// Again, would like this to be in a header, for 
// the sake of universal application

#define ERR_NO_ERROR 0

// API ERRORS -(1000 - 1999)
#define ERR_OUT_OF_MEMORY -1001
#define ERR_END_OF_FILE -1002
#define ERR_FILE_READ -1003
#define ERR_CANNOT_OPEN -1004
#define ERR_CANNOT_CLOSE -1005
#define ERR_PAR_NOT_FOUND -1006
#define ERR_WRITE_FAILURE -1007
#define ERR_INVALID_COLUMN -1008
#define ERR_COLUMNS_EQUAL -1009
#define ERR_FITS_ERROR -1010

// Pipeline Errors -(2000 - 2999)
#define ERR_NOT_FOUND -2001

int error_checking (int error);

#endif /*ERROR_CHECKING_1_1_H_*/
