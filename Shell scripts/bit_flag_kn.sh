#!/bin/bash 

#script to perform quasi-bitwise AND operator using shell scripting
#given input of a 16 bit integer (can be modded below) 
#This version defines "bad" flags in the script.  This could be commented out and replaced with an argument

#take the argument as the input
INPUT_FLAGS= ${1}

#defines a variable to be used in the output
OUT_FLAGS=0

#defines the number of bits
NUM_BITS=16

#defines the "bad" flags as per original instructions from KN
#ALL BITS ABOVE 128 are considered bad
BAD_FLAGS=65408 #0b1111111110000000 = 65408 = 0xFF80

#counts down the bits from most significant to least significant
for (( FLAG_INDEX = NUM_BITS ;  $FLAG_INDEX >0;  FLAG_INDEX--  ))
do
	BIT = $(())
	if [ ! -d ${FITS_DIR_NAME} ]; then
		
	fi
done


exit ${OUT_FLAGS}
