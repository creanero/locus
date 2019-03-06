
##!/bin/bash -vx

#this is a script to copy both local and raw versions of the same file to the 
#local machine for test purposes
#copy_both_files_1_0.sh
#Version 1.0
#9th September 2010

#names the parameter file fromt the first argument for clarity of reading
PRT_FILE=$1

#defines cosmo root directory
COSMO_ROOT=/grid/cosmo

#defines the root of the project directory structure
PROJ_ROOT=ittd/grid_cdm
PROJ_OUTPUT_ROOT=ittd/grid_cdm_test


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

#transfers local files to permanent storage
#lfc-cr?
#due to the layout of prm text files, it is necessary to use lines 3 onward
for (( NAME_INDEX = 3 ;  $NAME_INDEX <= $NUM_LINES+1;  NAME_INDEX++  ))
do

#this line gets the filename from the temp file and stores it in the variable name.
FITS_NAME=`sed -n $NAME_INDEX$P $PRT_FILE`

#this line gets the directory the file is in
FITS_DIR_NAME=`dirname $FITS_NAME`

#this line gets the root of the filename
FITS_NAME_BASE=`basename ${FITS_NAME} .fit`

#this line makes the name fo the local file
LOCAL_NAME=${FITS_DIR_NAME}/${FITS_NAME_BASE}_local.fit

#this line makes the name of the directory to be stored in on the LFC
LFC_DIR=`echo ${COSMO_ROOT}/${FITS_DIR_NAME} | sed 's/raw/local/g'`

#this line makes the filename it is to be stored in in the LFC
LFC_NAME=`echo ${COSMO_ROOT}/${LOCAL_NAME} | sed 's/raw/local/g'`

#use lfc-mkdir and lfc-ls to make a dir if it isn't there
lfc-ls -d ${LFC_DIR}
lfc_test_dir_exists=$?
if [ x"${lfc_test_dir_exists}" != x"0" ]; then
 lfc-mkdir -p ${LFC_DIR}
fi

#sets up a counter for the number of failed copy and register results
CR_FAIL_COUNT=0

#use lfc-ls to make see if the file already exists
lfc-ls ${LFC_NAME}
lfc_test_file_exists=$?
if [ x"${lfc_test_file_exists}" != x"0" ]; then
 lcg-cp  --vo cosmo lfn:${LFC_NAME} file:${LOCAL_NAME}
 lcg_cr_status=$?
 if [ x"${lcg_cr_status}" != x"0" ]; then
   echo "lcg-cp  --vo cosmo lfn:${LFC_NAME} file:${LOCAL_NAME} failed with error ${lcg_cr_status}"
   CR_FAIL_COUNT=`expr ${CR_FAIL_COUNT} + 1`

 fi
fi


done

#displays the various possible failure states of the program at the end
echo "Number of failed lgc-cp commands on Raw files ${CP_FAIL_COUNT}"
echo "Number of failed lcg-cp commands on Local files ${CR_FAIL_COUNT}"


#checks the error states and returns an error if needed.
if [ x"${CR_FAIL_COUNT}" != x"0" ]; then
 exit ${CR_FAIL_COUNT}
fi
if [ x"${CP_FAIL_COUNT}" != x"0" ]; then
 exit ${CP_FAIL_COUNT}

exit 0

