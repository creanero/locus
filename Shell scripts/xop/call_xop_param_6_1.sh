
##!/bin/bash -vx

#this is a script to call the parameterise stage of the xoplanet part of the project
#the explanet stage requires several layers of dereferencing in order to work.

#at this stage, the program copies the parameterise executable
##############################################################
#  This program is built on the basis of the call qso script #
#           it has not yet been properly modified            #
#        do not attempt to use without much revision         #
##############################################################

#these are the arguments from parameterisation_6_1
#All are commented as it is clearer to actually use the numbers and allow reference here if needed.
csv_short_name=$1 
csv_small_short_name=$2
arg_num_targets=$3
prm_file_path=$4
csv_file_name=$5
fit_file_path=$6
arg_fov_size=$7
arg_resolution=$8
arg_max_mag_diff=$9
arg_max_col_diff=${10}

gunzip ${csv_short_name}.gz

DEBUG="no"

if [ "x${DEBUG}" = "xyes" ]; then
 echo $PPR_FILE
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

PARAMETERISATION_DIR="${PROJ_ROOT}/release/parameterisation"
if [ ! -d ${PARAMETERISATION_DIR} ]; then
 mkdir -p ${PARAMETERISATION_DIR}
fi

# copies diagnose executable from permanent () to temporary (test)
lcg-cp --vo cosmo lfn:$COSMO_ROOT/${PARAMETERISATION_DIR}/parameterisation_6_1 file:${PARAMETERISATION_DIR}/parameterisation_6_1

#and makes it executable
chmod 755 ${PARAMETERISATION_DIR}/parameterisation_6_1


#finds the number of entries in the small CSV file (usually 3000)
NUM_ENTRIES=`wc $csv_small_short_name | awk '{print $1}'`
NUM_ENTRIES=$((NUM_ENTRIES - 1))

#makes a directory to store the PRM/PPR files
#PRM_DIR=`dirname ${prm_file_path}`

#if [ ! -d ${PRM_DIR} ]; then
# mkdir -p ${PRM_DIR}
#fi

if [ ! -d ${prm_file_path} ]; then
 mkdir -p ${prm_file_path}
fi

#runs parameterisation on the csv files
${PARAMETERISATION_DIR}/parameterisation_6_1 ${NUM_ENTRIES} ${prm_file_path}_prm ${csv_small_short_name} ${fit_file_path}
PARAMETERISATION_EXIT_SMALL=$?

PRT_FILE=`ls ${prm_file_path}*.txt`
PRM_FILE=`ls ${prm_file_path}*.prm`


#this line gets the number of lines in the temp file
#this corresponds to the number of files in the directory
NUM_LINES=`wc $PRT_FILE | awk '{print $1}'`

NAME=Stranger
P=p

CP_FAIL_COUNT=0


NUM_LINES_PLUS=`expr ${NUM_LINES} + 1 `

#due to the layout of prm text files, it is necessary to use lines 3 onward
for (( NAME_INDEX = 3 ;  $NAME_INDEX <= $NUM_LINES_PLUS;  NAME_INDEX++  ))

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

#executes parameterise in full
#and redirect its output to null
${PARAMETERISATION_DIR}/parameterisation_6_1 $arg_num_targets ${prm_file_path} ${csv_short_name} ${fit_file_path} ${arg_fov_size} ${arg_resolution} ${arg_max_mag_diff} ${arg_max_col_diff} ${PRM_FILE}
PARAMETERISATION_EXIT=$?

PPR_LIST_FILE=${PROJ_ROOT}/data/ppr_list_file.log

ls ${prm_file_path}*.ppr > ${PPR_LIST_FILE}
#sets up a counter for the number of failed copy and register results
CR_FAIL_COUNT=0

NUM_LINES_LS=`wc $PPR_LIST_FILE | awk '{print $1}'`



NUM_LINES_LS_PLUS=`expr ${NUM_LINES_LS} + 1 `
#transfers local files to permanent storage
for (( NAME_INDEX = 0 ;  $NAME_INDEX <= $NUM_LINES_LS;  NAME_INDEX++  ))
do

#this line gets the filename from the temp file and stores it in the variable name.
PPR_NAME=`sed -n ${NAME_INDEX}p $PPR_LIST_FILE`

#this line gets the directory the file is in
PPR_DIR_NAME=`dirname $PPR_NAME`

#this line gets the root of the filename
PPR_NAME_BASE=`basename ${PPR_NAME} .ppr`

#this line makes the name fo the local file
PRT_NAME=${PPR_DIR_NAME}/${PPR_NAME_BASE}.txt

#makes LFC Dir name
LFC_DIR=${COSMO_ROOT}/${PPR_DIR_NAME}

#this line makes the name of the directory to be stored in on the LFC
LFC_PRT_NAME=${COSMO_ROOT}/${PRT_NAME}

#this line makes the filename it is to be stored in in the LFC
LFC_PPR_NAME=${COSMO_ROOT}/${PPR_NAME}

#use lfc-mkdir and lfc-ls to make a dir if it isn't there
lfc-ls -d ${LFC_DIR}
lfc_test_dir_exists=$?
if [ x"${lfc_test_dir_exists}" != x"0" ]; then
 lfc-mkdir -p ${LFC_DIR}
fi


#use lfc-ls to make see if the file already exists
lfc-ls ${LFC_PPR_NAME}
lfc_test_file_exists=$?
if [ x"${lfc_test_file_exists}" != x"0" ]; then
 lcg-cr  --vo cosmo -l lfn:${LFC_PPR_NAME} file:${PPR_NAME}
 lcg_cr_status=$?
 if [ x"${lcg_cr_status}" != x"0" ]; then
   echo "lcg-cr  --vo cosmo -l lfn:${LFC_PPR_NAME} file:${PPR_NAME} failed with error ${lcg_cr_status}"
   CR_FAIL_COUNT=`expr ${CR_FAIL_COUNT} + 1`
 fi
fi

#use lfc-ls to make see if the file already exists
lfc-ls ${LFC_PRT_NAME}
lfc_test_file_exists=$?
if [ x"${lfc_test_file_exists}" != x"0" ]; then
 lcg-cr  --vo cosmo -l lfn:${LFC_PRT_NAME} file:${PRT_NAME}
 lcg_cr_status=$?
 if [ x"${lcg_cr_status}" != x"0" ]; then
   echo "lcg-cr  --vo cosmo -l lfn:${LFC_PRT_NAME} file:${PRT_NAME} failed with error ${lcg_cr_status}"
   CR_FAIL_COUNT=`expr ${CR_FAIL_COUNT} + 1`
 fi
fi



done


#displays the various possible failure states of the program at the end
echo "Parameterisation small exit code ${PARAMETERISATION_EXIT_SMALL}" >logfile.out
echo "Parameterisation proper exit code ${PARAMETERISATION_EXIT}" >>logfile.out
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

