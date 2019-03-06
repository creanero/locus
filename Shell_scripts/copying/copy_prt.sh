
##!/bin/bash -vx

#this is a script to copy files named in a text file from LCF to local machine
#copy_prt.sh
#Version 0.1
#3rd March 2011

#It copies files in prt file $1 to the node 

#names the parameter file fromt the first argument for clarity of reading
PRT_FILE=$1
#defines cosmo root directory
COSMO_ROOT=/grid/cosmo

#defines the root of the project directory structure
PROJ_ROOT=ittd/grid_cdm


#creates directories if no directories exist
if [ ! -d ${PROJ_ROOT} ]; then
 mkdir -p ${PROJ_ROOT}
fi



#this line gets the number of lines in the temp file
#this corresponds to the number of files in the directory
NUM_LINES=`wc $PRT_FILE | awk '{print $1}'`

NAME=Stranger
P=p

CP_FAIL_COUNT=0

#due to the layout of prm text files, it is necessary to use lines 3 onward
for (( NAME_INDEX = 3 ;  $NAME_INDEX <= $NUM_LINES+1;  NAME_INDEX++  ))

do

#this line gets the filename from the temp file and stores it in the variable name.
NAME=`sed -n $NAME_INDEX$P $PRT_FILE`
FITS_DIR_NAME=`dirname $NAME`

if [ ! -d ${FITS_DIR_NAME} ]; then
 mkdir -p ${FITS_DIR_NAME}
fi



#this line copies the named file (NAME)from source to destination
lcg-cp --vo cosmo lfn:$COSMO_ROOT/$NAME file:$NAME
lcg_cp_status=$?
 if [ x"${lcg_cp_status}" != x"0" ]; then
   echo "lcg-cp --vo cosmo lfn:$COSMO_ROOT/$NAME file:$NAME failed with error ${lcg_cp_status}"
   CP_FAIL_COUNT=`expr ${CP_FAIL_COUNT} + 1 `
 fi

done


exit 0

