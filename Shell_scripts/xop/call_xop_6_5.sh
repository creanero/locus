
##!/bin/bash -vx

#this is a script to call the Quasar stage of the project
#call_xop_6_5.sh
#Version 6.5
#13th December 2011
#this script carries out the diagnose and extract executables on the parameter file 
#specified in the first argument, $1.  and the text file specified as argument $2
#it uses the colour specified with a character in the third argument 
#and copies the results to a directory in the LFC speficied by the fourth
#It copies the executables, the parameter file
#the fits files and other relevant data to the node and carries out the executables
#it then copies and registers theoutput fits files to the LFC and cleans up

#names the parameter file fromt the first argument for clarity of reading
PPR_FILE=$1
PRT_FILE=$2
COLOUR=$3
RUN_NAME=$4

DEBUG="no"

if [ "x${DEBUG}" = "xyes" ]; then
 echo $PPR_FILE
 echo $PRT_FILE
fi


#defines cosmo root directory
COSMO_ROOT=/grid/cosmo

#defines the root of the project directory structure
PROJ_ROOT=ittd/grid_cdm

#creates directories if no directories exist
if [ ! -d ${PROJ_ROOT} ]; then
 mkdir -p ${PROJ_ROOT}
fi

LOCUS_ALGORITHM_DIR="${PROJ_ROOT}/release/locus_algorithm"
if [ ! -d ${LOCUS_ALGORITHM_DIR} ]; then
 mkdir -p ${LOCUS_ALGORITHM_DIR}
fi

PPR_DIR=`dirname ${PPR_FILE}`
if [ ! -d ${PPR_DIR} ]; then
 mkdir -p ${PPR_DIR}
fi

# copies diagnose executable from permanent () to temporary (test)
lcg-cp --vo cosmo lfn:$COSMO_ROOT/${LOCUS_ALGORITHM_DIR}/locus_algorithm_6_5 file:${LOCUS_ALGORITHM_DIR}/locus_algorithm_6_5

#and makes it executable
chmod 755 ${LOCUS_ALGORITHM_DIR}/locus_algorithm_6_5


#copies Parameter prm and txt files from permanent (workspace/debug) to temporary (test)
lcg-cp --vo cosmo lfn:$COSMO_ROOT/$PPR_FILE file:$PPR_FILE
lcg-cp --vo cosmo lfn:$COSMO_ROOT/$PRT_FILE file:$PRT_FILE

#mkdir?


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
#this name is the name of the PPR and TXT files
PRT_FITS_NAME=`sed -n $NAME_INDEX$P $PRT_FILE`

#this line gets the directory the original file is in
PRT_FITS_DIR_NAME=`dirname $PRT_FITS_NAME` 

#this line gets the root of the filename
PRT_FITS_NAME_BASE=`basename ${PRT_FITS_NAME} .fit`

#this line makes the name fo the local file
BASE_LOCAL_FITS_NAME=${PRT_FITS_NAME_BASE}_local.fit

#this line makes the local catalogue directory name
LFC_FITS_LOCAL_DIR_NAME=`echo ${COSMO_ROOT}/${PRT_FITS_DIR_NAME} | sed 's/raw/local/g'`

#this line si included even though it is unnecessary so the code is easier to interpres
NODE_FITS_DIR_NAME=${PRT_FITS_DIR_NAME}

#this line makes the name fo the local file
NODE_FITS_NAME=${NODE_FITS_DIR_NAME}/${BASE_LOCAL_FITS_NAME}

#this line makes the filename it is to be stored in in the LFC
LFC_FITS_NAME=${LFC_FITS_LOCAL_DIR_NAME}/${BASE_LOCAL_FITS_NAME}

#use mkdir and ls to make a dir if it isn't there
ls -d ${NODE_FITS_DIR_NAME} &> /dev/null
test_dir_exists=$?
if [ x"${test_dir_exists}" != x"0" ]; then
 mkdir -p ${NODE_FITS_DIR_NAME}
fi


#this line copies the named file (NAME)from source to destination
lcg-cp --vo cosmo lfn:${LFC_FITS_NAME} file:${NODE_FITS_NAME}
lcg_cp_status=$?
 if [ x"${lcg_cp_status}" != x"0" ]; then
   echo "lcg-cp --vo cosmo lfn:${LFC_FITS_NAME} file:${NODE_FITS_NAME} failed with error ${lcg_cp_status}"
   CP_FAIL_COUNT=`expr ${CP_FAIL_COUNT} + 1 `
 fi



done

#executes algorithm
#and redirect its output to null
./${LOCUS_ALGORITHM_DIR}/locus_algorithm_6_5  ${PPR_FILE} ${COLOUR} &>/dev/null

LOCUS_ALGORITHM_EXIT=$?


LFC_OUT_DIR_NAME=${COSMO_ROOT}/${PROJ_ROOT}/data/output/${RUN_NAME}

#this line gets the directory the file is in
PPR_DIR_NAME=`dirname $PPR_FILE`

#this line gets the root of the filename
PPR_NAME_BASE=`basename ${PPR_FILE} .ppr`

#this makes the short name of the output file
OUT_NAME=${PPR_NAME_BASE}_out.fit

#this line makes the full name fo the local file
NODE_OUT_NAME=${PPR_DIR_NAME}/${OUT_NAME}

#this line makes the filename it is to be stored in in the LFC
LFC_OUT_NAME=`echo ${LFC_OUT_DIR_NAME}/${OUT_NAME}`

#use lfc-mkdir and lfc-ls to make a dir if it isn't there
lfc-ls -d ${LFC_OUT_DIR_NAME}
lfc_test_dir_exists=$?
if [ x"${lfc_test_dir_exists}" != x"0" ]; then
 lfc-mkdir -p ${LFC_OUT_DIR_NAME}
fi

#sets up a counter for the number of failed copy and register results
CR_FAIL_COUNT=0

#use lfc-ls to make see if the file already exists
lfc-ls ${LFC_OUT_NAME}
lfc_test_file_exists=$?
if [ x"${lfc_test_file_exists}" != x"0" ]; then
 lcg-cr  --vo cosmo -l lfn:${LFC_OUT_NAME} file:${NODE_OUT_NAME}
 lcg_cr_status=$?
 if [ x"${lcg_cr_status}" != x"0" ]; then
   echo "lcg-cr  --vo cosmo -l lfn:${LFC_OUT_NAME} file:${NODE_OUT_NAME} failed with error ${lcg_cr_status}"
   CR_FAIL_COUNT=`expr ${CR_FAIL_COUNT} + 1`
 fi
fi


#displays the various possible failure states of the program at the end
echo "Locus Algorithm exit code ${LOCUS_ALGORITHM_EXIT}" >logfile.out
echo "Number of failed lgc-cp commands ${CP_FAIL_COUNT}" >> logfile.out
echo "Number of failed lcg-cr commands ${CR_FAIL_COUNT}" >> logfile.out


#checks the error states and returns an error if needed.

if [ x"${LOCUS_ALGORITHM_EXIT}" != x"0" ]; then
 exit ${LOCUS_ALGORITHM_EXIT}
fi
if [ x"${CR_FAIL_COUNT}" != x"0" ]; then
 exit ${CR_FAIL_COUNT}
fi
if [ x"${CP_FAIL_COUNT}" != x"0" ]; then
 exit ${CP_FAIL_COUNT}
fi


exit 0

