#!/bin/bash -vx

#this is a script to call the API stage of the project
#call_api_2_0.sh
#Version2.0
#17th May 2010

#this script carries out the diagnose and extract executables on the parameter file 
#specified in the first argument, $1.  and the text file specified as argument $2
#It copies the executables, the parameter file
#the fits files and other relevant data to the node and carries out the executables
#it then copies and registers the local fits files to the LFC and cleans up

#names the parameter file fromt the first argument for clarity of reading
PRM_FILE=$1
PRT_FILE=$2

DEBUG="no"

if [ "x${DEBUG}" = "xyes" ]; then
 echo $PRM_FILE
 echo $PRT_FILE
fi


#defines cosmo root directory
COSMO_ROOT=/grid/cosmo

#defines the root of the project directory structure
PROJ_ROOT=ittd/grid_cdm
PROJ_OUTPUT_ROOT=ittd/grid_cdm_test


#creates directories if no directories exist
if [ ! -d ${PROJ_ROOT} ]; then
 mkdir -p ${PROJ_ROOT}
fi

DIAGNOSE_DIR="${PROJ_ROOT}/release/diagnose"
if [ ! -d ${DIAGNOSE_DIR} ]; then
 mkdir -p ${DIAGNOSE_DIR}
fi


EXTRACT_DIR="${PROJ_ROOT}/release/extract"
if [ ! -d ${EXTRACT_DIR} ]; then
 mkdir -p ${EXTRACT_DIR}
fi

# copies diagnose executable from permanent () to temporary (test)
lcg-cp --vo cosmo lfn:$COSMO_ROOT/$PROJ_ROOT/release/diagnose/diagnose_5_0 file:$PROJ_ROOT/release/diagnose/diagnose_5_0

#and makes it executable
chmod 755 $PROJ_ROOT/release/diagnose/diagnose_5_0

#copies extract executable from permanent (workspace/debug) to temporary (test)
lcg-cp --vo cosmo lfn:$COSMO_ROOT/$PROJ_ROOT/release/extract/extract_5_0 file:$PROJ_ROOT/release/extract/extract_5_0

#and makes it executable
chmod 755 $PROJ_ROOT/release/extract/extract_5_0


#copies Parameter prm and txt files from permanent (workspace/debug) to temporary (test)
#lcg-cp --vo cosmo lfn:$COSMO_ROOT/$PRM_FILE file:$PRM_FILE
#lcg-cp --vo cosmo lfn:$COSMO_ROOT/$PRM_FILE file:$PRT_FILE

#mkdir?


#this line gets the number of lines in the temp file
#this corresponds to the number of files in the directory
NUM_LINES=`wc $PRT_FILE | awk '{print $1}'`

NAME=Stranger
P=p

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


done

#executes diagnose
./$PROJ_ROOT/release/diagnose/diagnose_5_0  $PRM_FILE

#executes extract
./$PROJ_ROOT/release/extract/extract_5_0  $PRM_FILE



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

#use lfc-ls to make see if the file already exists
lfc-ls ${LFC_NAME}
lfc_test_file_exists=$?
if [ x"${lfc_test_file_exists}" != x"0" ]; then
 lcg-cr  --vo cosmo -l lfn:${LFC_NAME} file:${LOCAL_NAME}
 lcg_cr_status=$?
 if [ x"${lcg_cr_status}" != x"0" ]; then
   echo "lcg-cr  --vo cosmo -l lfn:${LFC_NAME} file:${LOCAL_NAME} failed with error ${lcg_cr_status}"
 fi
fi


done


