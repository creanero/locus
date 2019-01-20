##!/bin/bash -vx


#assigns the first argument to the text parameter file name
PRT_FILE=$1
DIR_NAME=$2

COSMO_ROOT=/grid/cosmo

#this line gets the number of lines in the temp file
#this corresponds to the number of files in the directory
NUM_LINES=`wc $PRT_FILE | awk '{print $1}'`


#transfers local files to permanent storag
#due to the layout of prm text files, it is necessary to use lines 3 onward
#and use NUM_LINES+1 instead of numlines
for (( NAME_INDEX = 3 ;  $NAME_INDEX <= $NUM_LINES+1;  NAME_INDEX++  ))
do
 
 #this line gets the filename from the temp file and stores it in the variable name.
 FITS_NAME=`sed -n ${NAME_INDEX}p $PRT_FILE`
 LFC_NAME=${COSMO_ROOT}/${FITS_NAME}
 
 SHORT_NAME=`basename $FITS_NAME`
 TMP_FILE_NAME=${DIR_NAME}/${SHORT_NAME}
 ls ${TMP_FILE_NAME} &> /dev/null
 CHECK_TMP_FILE_EXISTS=$?
 if [ x"${CHECK_TMP_FILE_EXISTS}" != x"0" ]; then
  #empty case
  echo "file not found"
 else
  lcg-cr  --vo cosmo -l lfn:${LFC_NAME} file:${TMP_FILE_NAME}
 fi
done



