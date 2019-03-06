#ifndef STRUCT_DEFINITIONS_5_0_H_
#define STRUCT_DEFINITIONS_5_0_H_

// cannot make variable-sized arrays under normal cirucmstances.
// would rather this be in a header or similar
#define NAME_SIZE 16

// defines terms to be used in the process of accessing data.
// SDSS has 5 colours, so 5 colours must be read in and out.
// Since ITTD is in Ireland, colour will be spelt with a U
#define NUM_COLOURS 5



//   CCC  AA  TTTTTT  AA  L     OOO   GGG  U   U EEEE 
//  C    A  A   TT   A  A L    O   O G     U   U E    
//  C    AAAA   TT   AAAA L    O   O G  GG U   U EEE  
//  C    A  A   TT   A  A L    O   O G   G U   U E    
//   CCC A  A   TT   A  A LLLL  OOO   GGG   UUU  EEEE 

//  **********CATALOGUE ENTRY DEFINITIONS *********
//creates a structure in the shape of data contained in a 
//.cti catalogue information entry as is used in sdss_info.cti

struct cti_entry
{
	int col_number;
	char col_name [NAME_SIZE];
	int type_code;
	long int repeat;
	long int width;		
};



//  SSS  TTTTTT  AA  RRRR  
// S       TT   A  A R   R 
//  SSS    TT   AAAA RRRR  
//     S   TT   A  A R R   
// SSSS    TT   A  A R  RR 
       
// **********INTERNAL STELLAR INFORMATION STRUCTURE *****

//creates a structure that will be used to store 
//the relevant information about a star.
struct star {
double ra;
double dec;
double mag[NUM_COLOURS];
double mag_err[NUM_COLOURS];
};

#endif /*STRUCT_DEFINITIONS_5_0_H_*/
